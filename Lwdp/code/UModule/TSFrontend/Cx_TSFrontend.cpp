#include <LwDp.h>
#include <PluginInc.h>
#include <fcntl.h>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>
#include <Interface/ZmqMgr/Ix_ZmqMgr.h>
#include <Interface/EventMgr/Ix_EventMgr.h>
#include <Interface/PerfMgr/Ix_PerfMgr.h>
#include <Interface/CommonUtilMgr/Ix_CommonUtilMgr.h>

#include "../Common/ExternalInterface.h"
#include "../Interface/TSFrontend/Ix_TSFrontend.h"

#include "Cx_TSFrontend.h"
#include "TSFrontendErrno.h"
#include "TSFrontendDef.h"


uint32_ gRecvBufMaxLen = LW_TSFRONTEND_RECV_MAX_LEN;
uint32_ gSendTimeout = LW_TSFRONTEND_SEND_TIMEOUT;
uint32_ gRecvTimeout = LW_TSFRONTEND_RECV_TIMEOUT;
std::string gConnStr = std::string(LW_TSFRONTEND_CONNECT_TARGET);
uint32_ gThreadNum = 0;
pthread_mutex_t gThreadNum_mutex = PTHREAD_MUTEX_INITIALIZER;
uint32_ gThreadMax = 0;

Cx_Interface<Ix_PerfMgr_Cps> gIoPerSecond;
Cx_Interface<Ix_PerfMgr_Cps> gThreadPerCreate;
Cx_Interface<Ix_PerfMgr_Cps> gThreadPerRelease;
Cx_Interface<Ix_PerfMgr_Cps> gThreadMbps;
StatisticFigures gProcessStatic;

void createCountThread()
{
	pthread_mutex_lock(&gThreadNum_mutex);
	++gThreadNum;
	if(gThreadNum > gThreadMax)
	{
		gThreadMax = gThreadNum;
	}
	pthread_mutex_unlock(&gThreadNum_mutex);
	
	gThreadPerCreate->Update();
}
void destoryCountThread()
{
	pthread_mutex_lock(&gThreadNum_mutex);
	if(gThreadNum > 0)
		--gThreadNum;
	pthread_mutex_unlock(&gThreadNum_mutex);
	gThreadPerRelease->Update();
}
CallBackStru cb(createCountThread, destoryCountThread);

class TimerProcessController: public ProcessController
{
public:
	TimerProcessController(CallBackStru call_back, 
		                        StatisticFigures* static_figures): ProcessController(call_back),
		                                                           mStat(static_figures)
	{
		GET_OBJECT_VOID(PerfMgr_timer, iPerfMgr_timer);
		mPerfMgr_timer = iPerfMgr_timer;
		mPerfMgr_timer->Start();
	}
	virtual ~TimerProcessController()
	{
		ASSERT_CHECK_VOID(LWDP_MODULE_LOG, mStat, "StatisticFigures object is NULL");
		mStat->Update(mPerfMgr_timer->End());
	}

	StatisticFigures* mStat;
	Cx_Interface<Ix_PerfMgr_timer> mPerfMgr_timer;
};

	
Cx_TSFrontend::Cx_TSFrontend()
{
	//printf("TSFrontend Create!\n");
}

Cx_TSFrontend::~Cx_TSFrontend()
{
	//printf("TSFrontend Delete!\n");
}

std::string addressToString(const struct sockaddr_in* addr)
{
    char* ip = inet_ntoa(addr->sin_addr);
	if(!ip)
		ip = (char*)"NULL";
    char port[32];
    sprintf(port, "%d", ntohs(addr->sin_port));
    std::string r;
    r=r+"("+ip+":"+port+")";
    return r;
}
ContextHandle  gContextHandle = NULL;
void* thread_callback(void* vfd)
{
	LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::DEBUG, "Thread Callback fd(%x)!", vfd);

	TimerProcessController NumCount(cb, &gProcessStatic);
	if(!vfd)
	{
		LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::ERR, 
					   "Thread Param is NULL");
		return NULL;
	}

	GET_OBJECT_RET(ZmqMgr, iZmqMgr, NULL);
	Cx_Interface<Ix_ZMessage> iZMessage;
	unsigned int accept_conn = *(unsigned int*)vfd;
	free(vfd);

	int     more 		= 0;
	uint32_ errRetCode 	= 0;
	char_*  errRetMsgStr  = ""; 
	uint32_ more_size 	= sizeof(more);
	uint32_ indexSend 	= 0;
	uint32_ indexRecv 	= 0;
	uint8_* sendBuf 	= NULL;
	uint32_ headLen     = 0;  
	uint32_ sendLength 	= 0;
	TS_REQ_SERVER_MSG* clientMsg = NULL;
	uint32_ zmq_ret_len = 0;
	SocketHandle requester = NULL;
	//////////////////////////////////////////////////////////////
	// Recv Tcp Message from Client
	//////////////////////////////////////////////////////////////
    int32_  ret_len 	= 0;
    uint32_ totleSize 	= gRecvBufMaxLen;
	uint8_* recvBuf 	= (uint8_*)malloc(totleSize * sizeof(uint8_));
	ASSERT_CHECK_RET(LWDP_PLUGIN_LOG, NULL, recvBuf, "Malloc Recv Buffer Error!");
	uint32_ recvCheckPacketLen = 0;
	
	while(1)
	{
		//Recv Data Length
		ret_len = recv(accept_conn, (char *)recvBuf+indexRecv, totleSize-indexRecv, 0);
		if(ret_len == 0)
		{
			LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::WARNING, 
						   "Remote Socket Closed fd(%x)", accept_conn);
			goto ERR_TCP_TAG;
		}
		else if(ret_len < 0)
		{
#ifdef LWDP_PLATFORM_DEF_WIN32
			errno = WSAGetLastError();
			if(errno == EAGAIN || errno == WSAEWOULDBLOCK)
#else
			if (errno == EAGAIN || errno == EWOULDBLOCK) 
#endif	
			{
				Api_TaskDelay(1);
				continue;
			}
			else
			{
				LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::WARNING, 
							   "Remote Socket Closed fd(%x)", accept_conn);

				goto ERR_TCP_TAG;
			}
		}

		//first read
		if(!recvCheckPacketLen)
		{
			clientMsg = (TS_REQ_SERVER_MSG*)recvBuf;
			totleSize = ntohl(clientMsg->msgLength);
			recvCheckPacketLen++;
			if(totleSize > gRecvBufMaxLen)
			{
				errRetCode   = TS_SERVER_TCP_MSG_LEN_ERROR;
				errRetMsgStr = "Msg Too Long";
				LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::WARNING, 
							   "Tcp Recv Packet is Too Long: recvlen(%d) the Max Packet Length is(%d)", 
							   totleSize, gRecvBufMaxLen);		

				goto ERR_RET_MSG_TAG;
			}

			gThreadMbps->Update(totleSize);
		}

		LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::NOTICE, 
					   "Tcp Recv Packet Size: (%d)", ret_len);		
		if(((uint32_)ret_len + indexRecv) < totleSize)
		{
			indexRecv += ret_len;
			//Api_TaskDelay(1);
			continue;
		}
		break;
	};

	LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::NOTICE, 
				   "Recv Client Message: (%d)", ntohl(clientMsg->msgLength));


	requester = iZmqMgr->GetNewSocket(gContextHandle, LWDP_REQ);
	RINOKR(iZmqMgr->Connect(requester, gConnStr.c_str()), NULL);

	//Set Option
	{
		//Set Send Timeout
		//RINOKR(iZmqMgr->Setsockopt (requester, LWDP_SNDTIMEO, 
		//                           &gSendTimeout, sizeof(gSendTimeout)), NULL);

		//Set Recv Timeout
		//RINOKR(iZmqMgr->Setsockopt (requester, LWDP_RCVTIMEO, 
		//                           &gRecvTimeout, sizeof(gRecvTimeout)), NULL);
	}

	//////////////////////////////////////////////////////////////
	// Send Message to ZMQ
	//////////////////////////////////////////////////////////////

	headLen = 0;
	zmq_ret_len = iZmqMgr->Send(requester, recvBuf + headLen, totleSize - headLen, 0);
	if(zmq_ret_len != totleSize - headLen)
	{	
		LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::ERR, 
					   "Send Message Timeout or Error fd(%x) ret(%d) send(%d)", accept_conn, totleSize, zmq_ret_len);

		errRetCode   = TS_SERVER_TCP2ZMQ_SEND_ERROR;
		errRetMsgStr = "Tcp Zmq Send Error";

		goto ERR_RET_MSG_TAG;	
	}

	//////////////////////////////////////////////////////////////
	// Recv Message from ZMQ
	//////////////////////////////////////////////////////////////
	while (1) 
	{
		GET_OBJECT_RET(ZMessage, iMsg, 0);
        // Process all parts of the message
        iMsg->InitZMessage();
        iZmqMgr->Recv(requester, iMsg, 0);
		
		iZmqMgr->Getsockopt(requester, LWDP_RCVMORE, &more, &more_size);
		if (!more)
		{
			iZMessage = iMsg;
		    break; // Last message part
		}
		Api_TaskDelay (1); 
	}
	iZmqMgr->CloseSocket(requester);
	
	if(!iZMessage->Size())
	{	
		LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::ERR, 
					   "Recv ZMQ Message Timeout or Error fd(%x) ret(%d)", accept_conn, ret_len);

		errRetCode   = TS_SERVER_ZMQ2TCP_RECV_TIMEOUT;
		errRetMsgStr = "Tcp Zmq Recv Error";

		goto ERR_RET_MSG_TAG;	
	}

	LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::NOTICE, 
				   "Recv Message from ZMQ: Len (%d)", iZMessage->Size());
	
	//////////////////////////////////////////////////////////////
	// Tcp Send to Client
	//////////////////////////////////////////////////////////////
	sendBuf    = (uint8_ *)iZMessage->Data();
	sendLength = iZMessage->Size();
	while(1)
	{
		//Send Data Length
		ret_len = send(accept_conn, (char *)sendBuf + indexSend, sendLength - indexSend, 0);
		if(ret_len == 0)
		{
			LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::WARNING, 
						   "send err Remote Socket Closed ret(%x)", ret_len);
			goto ERR_ZMQ_TAG;
		}
		else if(ret_len < 0)
		{
#ifdef LWDP_PLATFORM_DEF_WIN32
			errno = WSAGetLastError();
			if(errno == EAGAIN || errno == WSAEWOULDBLOCK)
#else
			if (errno == EAGAIN || errno == EWOULDBLOCK) 
#endif	
			{
				Api_TaskDelay(1);
				continue;
			}
			else
			{
				LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::WARNING, 
							   "send Remote Socket Closed ret(%x)", ret_len);
				goto ERR_ZMQ_TAG;
			}
		}

		if(((uint32_)ret_len + indexSend) < iZMessage->Size())
		{
			indexSend += ret_len;
			//Api_TaskDelay(1);
			continue;
		}
			
		break;
	};


ERR_RET_MSG_TAG:
	TS_RSP_SERVER_MSG errRetMsg;
	memset(&errRetMsg, 0, sizeof(errRetMsg));
	errRetMsg.msgLength = htonl(sizeof(TS_RSP_SERVER_MSG));
	errRetMsg.rspCode   = htonl(errRetCode);
	memcpy(errRetMsg.rspMsg, errRetMsgStr, TS_RET_MSG_LEN);    

	send(accept_conn, (char *)&errRetMsg, sizeof(TS_RSP_SERVER_MSG), 0);

ERR_ZMQ_TAG:
	//iZmqMgr->CloseSocket(requester);
	//iZmqMgr->CloseContext(context);
	
ERR_TCP_TAG:
	free(recvBuf);

	if(accept_conn)
	{
#ifdef LWDP_PLATFORM_DEF_WIN32
		int rc = closesocket(accept_conn);
#else
		::close (accept_conn);
#endif
	}
	
	Api_TaskDelay(1);
	return NULL;
}


void io_callback(LoopHandle loop, CBHandle w, int revents)
{
	gIoPerSecond->Update();
	LWDP_LOG_PRINT( "TSFRONTEND", LWDP_LOG_MGR::DEBUG, 
					"Io Callback Watcher(%x)!", w);

	Cx_Interface<Ix_EventMgr> (iEventMgr)(CLSID_EventMgr); 
	if((iEventMgr).IsNull()) 
	{ 
		LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::ERR, 
					   "Can't Get Module(%s) Pointer!(%s, %d)", 
					   "EventMgr", __FILE__, __LINE__);
		return; 
	}

	SOCKET* vfd = (SOCKET *)iEventMgr->GetCallBackData(w, LWEV::WATCHER_IO, LWEV::WATCHER_IO_FD);
	if(!vfd)
	{
		LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::ERR, 
					   "Get io_callback CallBack fd Error");
		return;
	}

	SOCKET fd = *vfd;
#ifdef LWDP_PLATFORM_DEF_WIN32
	fd = _get_osfhandle (*(SOCKET*)vfd);
#endif

    struct sockaddr_in addr;
    socklen_t addr_size = sizeof(addr);
	int* accept_conn = (int*)malloc(sizeof(int));
	while ((*accept_conn = accept(fd, (struct sockaddr *)&addr, &addr_size)) < 0)
	{
		if (errno == EAGAIN) 
		{
			//these are transient, so don't log anything.
			continue; 
		}
		else
		{
			LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::ERR, 
						   "Accept Error.");
			return;
		}
	}

    std::string r = addressToString(&addr);
	LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::DEBUG, 
				   "ACCEPT Clinet IP(%s)", r.c_str());

	//Api_TaskDelay(10);
	pthread_t t;
	int result;

	result = pthread_create(&t, NULL, thread_callback, accept_conn);
	if(result != 0){
		LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::ERR, 
					   "Can't Create Thread Ret: %d\n", result);
		return;
	}

	result = pthread_detach(t);
	if(result != 0){
		LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::ERR, 
					   "Can't Detach Thread Ret: %d\n", result);
		return;
	}
	
}


LWRESULT Cx_TSFrontend::Init()
{
	GET_OBJECT_RET(PerfMgr_Cps, iPerfMgr_Cps_Io, LWDP_GET_OBJECT_ERROR);
	gIoPerSecond = iPerfMgr_Cps_Io;
	GET_OBJECT_RET(PerfMgr_Cps, iPerfMgr_Cps_Create, LWDP_GET_OBJECT_ERROR);
	gThreadPerCreate  = iPerfMgr_Cps_Create;
	GET_OBJECT_RET(PerfMgr_Cps, iPerfMgr_Cps_Destory, LWDP_GET_OBJECT_ERROR);
	gThreadPerRelease = iPerfMgr_Cps_Destory;
	GET_OBJECT_RET(PerfMgr_Cps, iPerfMgr_Cps_MBPS, LWDP_GET_OBJECT_ERROR);
	gThreadMbps = iPerfMgr_Cps_MBPS;

	GET_OBJECT_RET(ConfigMgr, iConfigMgr, LWDP_GET_OBJECT_ERROR);

	XPropertys propMax;
	iConfigMgr->GetModulePropEntry(LW_TSFRONTEND_MODULE_NAME, LW_TSFRONTEND_BUF_MAX_LEN_NAME, propMax);

	if(!propMax[0].propertyText.empty())
	{
		gRecvBufMaxLen = atol(propMax[0].propertyText.c_str());
	}
	else
	{
		LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <RecvBufMaxLen> In Config File, Default(%d)", gRecvBufMaxLen);
	}

	//Connect
	XPropertys propConnet;
	iConfigMgr->GetModulePropEntry(LW_TSFRONTEND_MODULE_NAME, LW_TSFRONTEND_CONNECT_TARGET_NAME, propConnet);

	if(!propConnet[0].propertyText.empty())
	{
		gConnStr = propConnet[0].propertyText;
	}
	else
	{
		LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <ConnetTarget> In Config File, Default(%s)", gConnStr.c_str());
	}

	//Set Send Timeout
	XPropertys propSend;
	iConfigMgr->GetModulePropEntry(LW_TSFRONTEND_MODULE_NAME, LW_TSFRONTEND_SEND_TIMEOUT_NAME, propSend);

	if(!propSend[0].propertyText.empty())
	{
		gSendTimeout = atol(propSend[0].propertyText.c_str());
	}
	else
	{
		LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <SendTimeout> In Config File, Default(%d)", gSendTimeout);
	}

	//Set Recv Timeout
	XPropertys propRecv;
	iConfigMgr->GetModulePropEntry(LW_TSFRONTEND_MODULE_NAME, LW_TSFRONTEND_RECV_TIMEOUT_NAME, propRecv);

	if(!propRecv[0].propertyText.empty())
	{
		gRecvTimeout = atol(propRecv[0].propertyText.c_str());
	}
	else
	{
		LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <RecvTimeout> In Config File, Default(%d)", gRecvTimeout);
	}

	
	XPropertys propPort;
	iConfigMgr->GetModulePropEntry(LW_TSFRONTEND_MODULE_NAME, LW_TSFRONTEND_MODULE_PORT_NAME, propPort);

	uint32_ portNum = LW_TSFRONTEND_SERVER_PORT;
	if(!propPort[0].propertyText.empty())
	{
		portNum = atol(propPort[0].propertyText.c_str());
	}
	else
	{
		LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <ServerPort> In Config File, Default(%d)", portNum);
	}

	RINOK(createTcpServer(portNum));

	GET_OBJECT_RET(EventMgr, iEventMgr, 0);
	RINOK(iEventMgr->InitLoop(0));
	mIoWatcher = iEventMgr->CreateWatcher(LWEV::WATCHER_IO, (WATCHER_CALLBACK)io_callback, mServerSocket, LW_READ);
	if(!mIoWatcher)
	{
		LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::ERR, "iEventMgr->CreateWatcher Create IO Watcher Error");
		return TSFRONTEND::LWDP_CREATE_IO_WATCHER_ERR;
	}

	GET_OBJECT_RET(ZmqMgr, iZmqMgr, -1);
	gContextHandle = iZmqMgr->GetNewContext();
	//Start Watcher
	iEventMgr->WatcherStart(mIoWatcher);
	
	Cx_Interface<Ix_ConsoleMgr> iConsoleMgr(CLSID_ConsoleMgr);
	if(!iConsoleMgr.IsNull())
	{
		ConsoleCBDelegate regFun = MakeDelegate(this, &Cx_TSFrontend::ConsoleGetTsInfo);
		RINOK(iConsoleMgr->RegisteCommand(LW_TSFRONTEND_COMMAND_GET_INFO_NAME, regFun,
				                          LW_TSFRONTEND_COMMAND_GET_MSG));
	}

	return LWDP_OK;
}

int32_ Cx_TSFrontend::ConsoleGetTsInfo(COMMAND_LINE& command_line)
{
	std::cout << "Current Thread Num: " << gThreadNum << std::endl;

	std::cout << "Io Percent: "
			  << gIoPerSecond->GetCps()  << " /s" << std::endl;		
	std::cout << "Create Thread Percent: "
			  << gThreadPerCreate->GetCps()  << " /s" << std::endl;
	std::cout << "Release Thread Percent: "
			  << gThreadPerRelease->GetCps()  << " /s" << std::endl;
	std::cout << "Max Thread Num: "
			  << gThreadMax << std::endl;

	std::cout << "Process Time: " << std::endl
		      << "-----------------------------" << std::endl
			  << gProcessStatic.GetMax() /1000.0 << "s  |  " 
			  << gProcessStatic.GetMin() /1000.0 << "s  |  " 
			  << gProcessStatic.GetAvg() /1000.0 << "s" << std::endl;

	std::cout << "-----------------------------" << std::endl
	          << "KBps: " << gThreadMbps->GetKbps() << "KB/s" << std::endl;
	
	return 0;
}

LWRESULT Cx_TSFrontend::RunServer()
{
	GET_OBJECT_RET(EventMgr, iEventMgr, LWDP_GET_OBJECT_ERROR);
	return iEventMgr->RunLoop(0);
}

LWRESULT Cx_TSFrontend::DestoryServer()
{
	return LWDP_OK;
}

void Cx_TSFrontend::setNonblock(SOCKET fd)
{ 
#ifdef LWDP_PLATFORM_DEF_WIN32
	unsigned long arg = 1;
	ioctlsocket (fd, FIONBIO, &arg);
#else
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
#endif	
}
void Cx_TSFrontend::setReuseaddr(SOCKET fd)
{
    int ok = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&ok, sizeof(ok));
}

void Cx_TSFrontend::setAddress(const char* ip, int port, struct sockaddr_in* addr)
{
    memset(addr, 0, sizeof(*addr));
    addr->sin_family=AF_INET;
	addr->sin_addr.s_addr = INADDR_ANY;
    addr->sin_port = htons(port);
}

std::string Cx_TSFrontend::addressToString(struct sockaddr_in* addr)
{
    char* ip = inet_ntoa(addr->sin_addr);
	if(!ip)
		ip = "NULL";
    char port[32];
    sprintf(port, "%d", ntohs(addr->sin_port));
    std::string r;
    r=r+"("+ip+":"+port+")";
    return r;
}

LWRESULT Cx_TSFrontend::createTcpServer(int port)
{
    mServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(mServerSocket <= 0)
	{
		LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::ERR, "Socket Create Failed");
		return TSFRONTEND::LWDP_CREATE_SOCKET_ERR;
	}
	
    setNonblock(mServerSocket);
    setReuseaddr(mServerSocket);
    sockaddr_in addr;
    setAddress("0.0.0.0", port, &addr);
    int ret = bind(mServerSocket, (struct sockaddr*)&addr, sizeof(addr));
    if(ret)
    {
		LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::ERR, 
			           "Bind(port:%d) Socket Failed", port);
		return TSFRONTEND::LWDP_BIND_SOCKET_ERR;
    }
	
	GET_OBJECT_RET(ConfigMgr, iConfigMgr, LWDP_GET_OBJECT_ERROR);
	XPropertys propBlock;
	iConfigMgr->GetModulePropEntry(LW_TSFRONTEND_MODULE_NAME, LW_TSFRONTEND_MODULE_BLOCK_NAME, propBlock);

	uint32_ blockNum = LW_TSFRONTEND_BLOCK;
	if(!propBlock[0].propertyText.empty())
	{
		blockNum = atol(propBlock[0].propertyText.c_str());
	}
	else
	{
		LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <ListenBlock> In Config File, Default(%d)", blockNum);
	}
	
	ret = listen(mServerSocket, blockNum); //
	if(ret)
    {
		LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::ERR, 
			           "Listen Socket Failed");
		return TSFRONTEND::LWDP_LISTEN_SOCKET_ERR;
    }
    return LWDP_OK;
}



