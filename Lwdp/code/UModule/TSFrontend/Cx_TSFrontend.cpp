#include <LwDp.h>
#include <PluginInc.h>
#include <fcntl.h>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>
#include <Interface/ZmqMgr/Ix_ZmqMgr.h>
#include <Interface/EventMgr/Ix_EventMgr.h>

#include "../Common/ExternalInterface.h"
#include "../Interface/TSFrontend/Ix_TSFrontend.h"

#include "Cx_TSFrontend.h"
#include "TSFrontendErrno.h"
#include "TSFrontendDef.h"


uint32_      gMaxNum = LW_TSFRONTEND_RECV_MAX_LEN;
uint32_ gSendTimeout = LW_TSFRONTEND_SEND_TIMEOUT;
uint32_ gRecvTimeout = LW_TSFRONTEND_RECV_TIMEOUT;
std::string gConnStr = std::string(LW_TSFRONTEND_CONNECT_TARGET);

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
		ip = "NULL";
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
	uint32_ index = 0;
	TS_TCP_SERVER_MSG* sendMsgStru = NULL;
	uint8_* sendBuf = NULL;
	TS_TCP_SERVER_MSG* clientMsg = NULL;
	uint32_ headLen = sizeof(uint32_) * 2;
	int zmq_ret_len = 0;
	SocketHandle requester = NULL;
	//////////////////////////////////////////////////////////////
	// Recv Tcp Message from Client
	//////////////////////////////////////////////////////////////
    int ret_len = 0;
    uint32_ recvLen = LW_TSFRONTEND_RECV_BUFFER_LEN;
	uint8_* recvBuf = (uint8_*)malloc(recvLen * sizeof(uint8_));
	ASSERT_CHECK_RET(LWDP_PLUGIN_LOG, NULL, recvBuf, "Malloc Recv Buffer Error!");
	while(1)
	{
		//Recv Data Length
		ret_len = recv(accept_conn, (char *)recvBuf, recvLen, 0);
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

		break;
	};

	LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::NOTICE, 
				   "Recv Client Message: (%s)", recvBuf);	

	clientMsg = (TS_TCP_SERVER_MSG*)recvBuf;
	if(clientMsg->msgLength > ret_len)
	{
		clientMsg->statusCode = TS_SERVER_TCP_MSG_LEN_ERROR;
		clientMsg->msgLength = sizeof(TS_TCP_SERVER_MSG);
		send(accept_conn, (char *)clientMsg, clientMsg->msgLength, 0);

		goto ERR_TCP_TAG;
	}

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
					   "Send Message Timeout or Error fd(%x) ret(%d) send(%d)", accept_conn, ret_len, recvLen);

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
		
		uint32_ more_size = sizeof(more);
		iZmqMgr->Getsockopt(requester, LWDP_RCVMORE, &more, &more_size);
		if (!more)
		{
			iZMessage = iMsg;
		    break; // Last message part
		}
		Api_TaskDelay (1); 
	}

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
		ret_len = send(accept_conn, (char *)sendMsgStru + index, sendMsgStru->msgLength - index, 0);
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

		if(ret_len + index < iZMessage->Size())
		{
			index += ret_len;
			Api_TaskDelay(1);
			continue;
		}
			
		break;
	};

	free(sendBuf);
ERR_ZMQ_TAG:
	iZmqMgr->CloseSocket(requester);
	//iZmqMgr->CloseContext(context);
	
ERR_TCP_TAG:
	free(recvBuf);

#ifdef LWDP_PLATFORM_DEF_WIN32
	int rc = closesocket(accept_conn);
#else
	::close (accept_conn);
#endif

	return NULL;
}


void io_callback(LoopHandle loop, CBHandle w, int revents)
{
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
	
	return LWDP_OK;
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
	addr->sin_addr.s_addr = inet_addr(ip);
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
    mServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(mServerSocket <= 0)
	{
		LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::ERR, "Socket Create Failed");
		return TSFRONTEND::LWDP_CREATE_SOCKET_ERR;
	}
	
    setNonblock(mServerSocket);
    setReuseaddr(mServerSocket);
    sockaddr_in addr;
    setAddress("0.0.0.0", port, &addr);
    bind(mServerSocket, (struct sockaddr*)&addr, sizeof(addr));

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
	
    listen(mServerSocket, blockNum); // 
    return LWDP_OK;
}



