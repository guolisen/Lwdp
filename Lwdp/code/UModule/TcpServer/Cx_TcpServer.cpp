#include <Lwdp.h>
#include <PluginInc.h>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>

#include "../Interface/TcpServer/Ix_TcpServer.h"
#include "../Interface/TSFrontend/Ix_TSFrontend.h"
#include "../Interface/ZmqBackend/Ix_ZmqBackend.h"
#include "../Interface/ACDevice/Ix_ACDevice.h"

#include "TcpServerErrno.h"
#include "Cx_TcpServer.h"

Cx_TcpServer::Cx_TcpServer()
{
	printf("TcpServer Create!\n");
}

Cx_TcpServer::~Cx_TcpServer()
{
	printf("TcpServer Delete!\n");
}

void* thread_run_frondend(void* arg)
{
	GET_OBJECT_RET(TSFrontend, iTSFrontend, 0);
	RINOKR(iTSFrontend->RunServer(), 0);

	return NULL;
}
void* thread_run_backend(void* arg)
{
	GET_OBJECT_RET(ZmqBackend, iZmqBackend, 0);
	RINOKR(iZmqBackend->RunServer(), 0);
	
	return NULL;
}
LWRESULT Cx_TcpServer::Init()
{
	GET_OBJECT_RET(TSFrontend, iTSFrontend, LWDP_GET_OBJECT_ERROR);
	RINOK(iTSFrontend->Init());

	GET_OBJECT_RET(ZmqBackend, iZmqBackend, LWDP_GET_OBJECT_ERROR);
	RINOK(iZmqBackend->Init());

	return LWDP_OK;
}
LWRESULT Cx_TcpServer::RunServer()
{
	pthread_t t;
	int result;

	//Frontend
	result = pthread_create(&t, NULL, thread_run_frondend, NULL);
	if(result != 0){
		LWDP_LOG_PRINT("TCPSERVER", LWDP_LOG_MGR::ERR, 
					   "Can't Create Thread Ret: %d\n", result);
		return TCPSERVER::LWDP_CREATE_WORK_THREAD_ERR;
	}

	result = pthread_detach(t);
	if(result != 0){
		LWDP_LOG_PRINT("TCPSERVER", LWDP_LOG_MGR::ERR, 
					   "Can't Detach Thread Ret: %d\n", result);
		return TCPSERVER::LWDP_CREATE_DETACH_THREAD_ERR;
	}

	//Backend
	result = pthread_create(&t, NULL, thread_run_backend, NULL);
	if(result != 0){
		LWDP_LOG_PRINT("TCPSERVER", LWDP_LOG_MGR::ERR, 
					   "Can't Create Thread Ret: %d\n", result);
		return TCPSERVER::LWDP_CREATE_WORK_THREAD_ERR;
	}

	result = pthread_detach(t);
	if(result != 0){
		LWDP_LOG_PRINT("TCPSERVER", LWDP_LOG_MGR::ERR, 
					   "Can't Detach Thread Ret: %d\n", result);
		return TCPSERVER::LWDP_CREATE_DETACH_THREAD_ERR;
	}
	
	return LWDP_OK;
}

LWRESULT Cx_TcpServer::DestoryServer()
{

	return LWDP_OK;
}

