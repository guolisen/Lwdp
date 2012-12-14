#include <LwDp.h>
#include <PluginInc.h>
#include <fcntl.h>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>
#include <Interface/ZmqMgr/Ix_ZmqMgr.h>
#include <Interface/EventMgr/Ix_EventMgr.h>

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

void* thread_callback(void* vfd)
{
	LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::DEBUG, "Thread Callback fd(%x)!", vfd);

	if(!vfd)
	{
		LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::ERR, 
					   "Thread Param is NULL");
		return NULL;
	}

	int accept_conn = *(int*)vfd;
	free(vfd);


	//////////////////////////////////////////////////////////////
	// Recv Tcp Message from Client
	//////////////////////////////////////////////////////////////
    int ret = 0;
    uint32_ recvLen = 4096;
	uint8_* recvBuf = (uint8_*)malloc(recvLen * sizeof(uint8_));
	ASSERT_CHECK_RET(LWDP_PLUGIN_LOG, NULL, recvBuf, "Malloc Recv Buffer Error!");
	while(1)
	{
		//Recv Data Length
		ret = recv(accept_conn, (char *)recvBuf, recvLen, 0);
		if(ret == 0)
		{
			LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::WARNING, 
						   "Remote Socket Closed fd(%x)", accept_conn);

#ifdef LWDP_PLATFORM_DEF_WIN32
			int rc = closesocket(accept_conn);
#else
			int rc = ::close (accept_conn);
#endif
			return NULL;
		}
		else if(ret < 0)
		{
#ifdef LWDP_PLATFORM_DEF_WIN32
			errno = WSAGetLastError();
			if(errno == EAGAIN || errno == WSAEWOULDBLOCK)
#else
			if (errno == EAGAIN || errno == EWOULDBLOCK) 
#endif	
			{
				Sleep(10);
				continue;
			}
			else
			{
				LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::WARNING, 
							   "Remote Socket Closed fd(%x)", accept_conn);

#ifdef LWDP_PLATFORM_DEF_WIN32
				int rc = closesocket(accept_conn);
#else
				int rc = ::close (accept_conn);
#endif
				return NULL;
			}
		}

		break;
	};

	LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::INFO, 
				   "!!!!!Recv Message: (%s)", recvBuf);	


	GET_OBJECT_RET(ZmqMgr, iZmqMgr, NULL);
	
	ContextHandle  context = iZmqMgr->GetNewContext();
	SocketHandle requester = iZmqMgr->GetNewSocket(context, LWDP_REQ);
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
	
	ret = iZmqMgr->Send(requester, recvBuf, recvLen, 0);
	if(ret != recvLen)
	{	
		LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::ERR, 
					   "Send Message Timeout or Error fd(%x) ret(%d) send(%d)", accept_conn, ret, recvLen);

		free(recvBuf);
#ifdef LWDP_PLATFORM_DEF_WIN32
		int rc = closesocket(accept_conn);
#else
		int rc = ::close (accept_conn);
#endif
		return NULL;		
	}

	//////////////////////////////////////////////////////////////
	// Recv Message from ZMQ
	//////////////////////////////////////////////////////////////
	int more = 0;
	std::string retdata("");
    while (1) 
	{
        // Process all parts of the message
		retdata = iZmqMgr->Recv(requester, 0);	
        
        uint32_ more_size = sizeof (more);
		iZmqMgr->Getsockopt(requester, LWDP_RCVMORE, &more, &more_size);
        if (!more)
            break; // Last message part
        Sleep (1); 
     }

	if(retdata.empty())
	{	
		LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::ERR, 
					   "Recv ZMQ Message Timeout or Error fd(%x) ret(%d)", accept_conn, ret);

		free(recvBuf);
#ifdef LWDP_PLATFORM_DEF_WIN32
		int rc = closesocket(accept_conn);
#else
		int rc = ::close (accept_conn);
#endif
		return NULL;		
	}

	LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::INFO, 
				   "!!!!!Recv ZMQ Message: (%s)", retdata.c_str());
	//////////////////////////////////////////////////////////////
	// Tcp Send to Client
	//////////////////////////////////////////////////////////////
	uint32_ index = 0;
	while(1)
	{
		//Send Data Length
		ret = send(accept_conn, (char *)retdata.data() + index, retdata.size(), 0);
		if(ret == 0)
		{
			LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::WARNING, 
						   "Remote Socket Closed fd(%x)", accept_conn);

#ifdef LWDP_PLATFORM_DEF_WIN32
			int rc = closesocket(accept_conn);
#else
			int rc = ::close (accept_conn);
#endif
			return NULL;
		}
		else if(ret < 0)
		{
#ifdef LWDP_PLATFORM_DEF_WIN32
			errno = WSAGetLastError();
			if(errno == EAGAIN || errno == WSAEWOULDBLOCK)
#else
			if (errno == EAGAIN || errno == EWOULDBLOCK) 
#endif	
			{
				Sleep(1);
				continue;
			}
			else
			{
				LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::WARNING, 
							   "Remote Socket Closed fd(%x)", accept_conn);

#ifdef LWDP_PLATFORM_DEF_WIN32
				int rc = closesocket(accept_conn);
#else
				int rc = ::close (accept_conn);
#endif
				return NULL;
			}
		}

		if(ret + index < retdata.size())
		{
			index += ret;
			Sleep(1);
			continue;
		}
			
		break;
	};

	iZmqMgr->CloseSocket(requester);
	iZmqMgr->CloseContext(context);
	free(recvBuf);
#ifdef LWDP_PLATFORM_DEF_WIN32
	int rc = closesocket(accept_conn);
#else
	int rc = ::close (accept_conn);
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
    int addr_size = sizeof(addr);
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



