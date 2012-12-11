#include <Lwdp.h>
#include <PluginInc.h>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>
#include <Interface/ZmqMgr/Ix_ZmqMgr.h>
#include <Interface/EventMgr/Ix_EventMgr.h>

#include "../Interface/TSFrontend/Ix_TSFrontend.h"

#include "Cx_TSFrontend.h"
#include "TSFrontendErrno.h"

Cx_TSFrontend::Cx_TSFrontend()
{
	printf("TSFrontend Create!\n");
}

Cx_TSFrontend::~Cx_TSFrontend()
{
	printf("TSFrontend Delete!\n");
}


void thread_callback(int socket)
{
	printf("!!!!!thread_callback Call Back!!!!\n");
}

void io_callback(void *loop, void *w, int revents)
{
	printf("!!!!!io_callback Call Back!!!!\n");
}

LWRESULT Cx_TSFrontend::Init()
{
	GET_OBJECT_RET(EventMgr, iEventMgr, 0);




	RINOK(iEventMgr->InitLoop(0));
	mIoWatcher = iEventMgr->CreateWatcher(LWEV::WATCHER_IO, (WATCHER_CALLBACK)io_callback, 1.0, 1.0);
	if(!mIoWatcher)
	{
		LWDP_LOG_PRINT("TCPSERVER", LWDP_LOG_MGR::ERR, "iEventMgr->CreateWatcher Create IO Watcher Error");
		return TSFRONTEND::LWDP_CREATE_IO_WATCHER_ERR;
	}

	//Start Watcher
	iEventMgr->WatcherStart(mIoWatcher);

	
	
	return LWDP_OK;
}
LWRESULT Cx_TSFrontend::RunServer()
{
	return iEventMgr->RunLoop(0);
}



