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


uint32_      gMaxNum = LW_TSFRONTEND_RECV_MAX_LEN;
uint32_ gSendTimeout = LW_TSFRONTEND_SEND_TIMEOUT;
uint32_ gRecvTimeout = LW_TSFRONTEND_RECV_TIMEOUT;
std::string gConnStr = std::string(LW_TSFRONTEND_CONNECT_TARGET);
uint32_ gThreadNum = 0;
pthread_mutex_t gThreadNum_mutex = PTHREAD_MUTEX_INITIALIZER;
uint32_ gThreadMax = 0;

Cx_Interface<Ix_PerfMgr_Cps> gIoPerSecond;
Cx_Interface<Ix_PerfMgr_Cps> gThreadPerCreate;
Cx_Interface<Ix_PerfMgr_Cps> gThreadPerRelease;

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

	
Cx_TSFrontend::Cx_TSFrontend()
{
	printf("TSFrontend Create!\n");
}

Cx_TSFrontend::~Cx_TSFrontend()
{
	printf("TSFrontend Delete!\n");
}

std::string addressToString(struct sockaddr_in* addr)
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

	ProcessController NumCount(cb);
	if(!vfd)
	{
		LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::ERR, 
					   "Thread Param is NULL");
		return NULL;
	}

	GET_OBJECT_RET(ZmqMgr, iZmqMgr, NULL);
	Cx_Interface<Ix_ZMessage> iZMessage;
	int accept_conn = *(int*)vfd;
	free(vfd);

	int more = 0;
	uint32_ more_size = sizeof(more);
	uint32_ indexSend = 0;
	uint32_ indexRecv = 0;
	TS_TCP_SERVER_MSG* sendMsgStru = NULL;
	uint8_* sendBuf = NULL;
	TS_TCP_SERVER_MSG* clientMsg = NULL;
	uint32_ headLen = sizeof(uint32_) * 2;
	uint32_ zmq_ret_len = 0;
	SocketHandle requester = NULL;
	//////////////////////////////////////////////////////////////
	// Recv Tcp Message from Client
	//////////////////////////////////////////////////////////////
    int32_  ret_len = 0;
    uint32_ totleSize = LW_TSFRONTEND_RECV_BUFFER_LEN;
	uint8_* recvBuf = (uint8_*)malloc(totleSize * sizeof(uint8_));
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
				Api_TaskDelay(10);
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
			clientMsg = (TS_TCP_SERVER_MSG*)recvBuf;
			totleSize = clientMsg->msgLength;
			recvCheckPacketLen++;
			if(totleSize > LW_TSFRONTEND_RECV_BUFFER_LEN)
			{
				clientMsg->statusCode = TS_SERVER_TCP_MSG_LEN_ERROR;
				clientMsg->msgLength  = sizeof(TS_TCP_SERVER_MSG);
				send(accept_conn, (char *)clientMsg, clientMsg->msgLength, 0);
				LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::WARNING, 
							   "Tcp Recv Packet is Too Long: recvlen(%d) the Max Packet Length is(%d)", 
							   totleSize, LW_TSFRONTEND_RECV_BUFFER_LEN);		

				goto ERR_TCP_TAG;
			}
		}

		LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::NOTICE, 
					   "Tcp Recv Packet Size: (%d)", ret_len);		
		if((ret_len + indexRecv) < totleSize)
		{
			indexRecv += ret_len;
			Api_TaskDelay(1);
			continue;
		}
		break;
	};

	LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::NOTICE, 
				   "Recv Client Message: (%d)", clientMsg->msgLength);

	
	//if(clientMsg->msgLength > ret_len)
	//{
	//	clientMsg->statusCode = TS_SERVER_TCP_MSG_LEN_ERROR;
	//	clientMsg->msgLength = sizeof(TS_TCP_SERVER_MSG);
	//	send(accept_conn, (char *)clientMsg, clientMsg->msgLength, 0);

	//	goto ERR_TCP_TAG;
	//}

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

	headLen     = sizeof(uint32_) * 2;
	zmq_ret_len = iZmqMgr->Send(requester, recvBuf + headLen, ret_len - headLen, 0);
	if(zmq_ret_len != ret_len - headLen)
	{	
		LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::ERR, 
					   "Send Message Timeout or Error fd(%x) ret(%d) send(%d)", accept_conn, ret_len, zmq_ret_len);

		goto ERR_ZMQ_TAG;		
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

		goto ERR_ZMQ_TAG;		
	}

	LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::NOTICE, 
				   "Recv Message from ZMQ: Len (%d)", iZMessage->Size());
	
	//////////////////////////////////////////////////////////////
	// Tcp Send to Client
	//////////////////////////////////////////////////////////////
	sendBuf = (uint8_ *)malloc(sizeof(TS_TCP_SERVER_MSG) + iZMessage->Size());
	ASSERT_CHECK_RET(LWDP_PLUGIN_LOG, 0, sendBuf, "TSFrontend thread Malloc Send Buf ERROR");
	sendMsgStru = (TS_TCP_SERVER_MSG*)sendBuf;
	sendMsgStru->msgLength = sizeof(TS_TCP_SERVER_MSG) + iZMessage->Size();
	sendMsgStru->statusCode = 0;
	memcpy(sendMsgStru->tcpMsgBody, iZMessage->Data(), iZMessage->Size());
	while(1)
	{
		//Send Data Length
		ret_len = send(accept_conn, (char *)sendMsgStru + indexSend, sendMsgStru->msgLength - indexSend, 0);
		if(ret_len == 0)
		{
			LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::WARNING, 
						   "Remote Socket Closed fd(%x)", accept_conn);
			free(sendBuf);
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
							   "Remote Socket Closed fd(%x)", accept_conn);
				free(sendBuf);
				goto ERR_ZMQ_TAG;
			}
		}

		if(ret_len + indexSend < iZMessage->Size())
		{
			indexSend += ret_len;
			Api_TaskDelay(1);
			continue;
		}
			
		break;
	};

	free(sendBuf);
ERR_ZMQ_TAG:
	//iZmqMgr->CloseSocket(requester);
	//iZmqMgr->CloseContext(context);
	
ERR_TCP_TAG:
	free(recvBuf);

#ifdef LWDP_PLATFORM_DEF_WIN32
	int rc = closesocket(accept_conn);
#else
	::close (accept_conn);
#endif
	Api_TaskDelay(5);
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

	GET_OBJECT_RET(ConfigMgr, iConfigMgr, LWDP_GET_OBJECT_ERROR);

	XPropertys propMax;
	iConfigMgr->GetModulePropEntry(LW_TSFRONTEND_MODULE_NAME, LW_TSFRONTEND_BUF_MAX_LEN_NAME, propMax);

	if(!propMax[0].propertyText.empty())
	{
		gMaxNum = atol(propMax[0].propertyText.c_str());
	}
	else
	{
		LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <RecvBufMaxLen> In Config File, Default(%d)", gMaxNum);
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
				                          LW_TSFRONTEND_COMMAND_GET_INFO_INFO));
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



