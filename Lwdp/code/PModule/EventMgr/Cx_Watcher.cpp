/*! \file Cx_Watcher.cpp
 *  \brief Watcher  Impl
 *  \author Guolisen, LwDp
 *  \date   2012.12.10
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <LwDp.h>
#include <PluginInc.h>
#include <iostream>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>

#include <LwApiLib/ComLib/Libev/ev.h>

#include "EventTagDef.h"
#include "Cx_EventMgr.h"
#include "Cx_Watcher.h"


LWDP_NAMESPACE_BEGIN;

//IO
typedef void (__cdecl *cb_io)(struct ev_loop *,ev_io *,int);

LWRESULT Cx_WatcherIO::Init(WATCHER_CALLBACK call_back, va_list argp)
{
	int fd = va_arg( argp, int); 
	int event = va_arg( argp, int); 

#ifdef LWDP_PLATFORM_DEF_WIN32
	ev_io_init(&mWatcher, (cb_io)call_back, _open_osfhandle (fd, 0), event);
#else
	ev_io_init(&mWatcher, (cb_io)call_back, (fd), event);
#endif
   
	return LWDP_OK;
}

LWRESULT Cx_WatcherIO::WatcherStart()
{
	ev_io_start(mLoop, &mWatcher);
	return LWDP_OK;
}
LWRESULT Cx_WatcherIO::WatcherStop()
{
	ev_io_stop(mLoop, &mWatcher);
	return LWDP_OK;
}


void* Cx_WatcherIO::GetTypeData(CBHandle call_back, LWEV::CALLBACK_DATA_TYPE type)
{
	if(!call_back)
	{
		PLATFORM_LOG(LWDP_EVENT_LOG, LWDP_LOG_ERR, "Cx_WatcherIO::GetTypeData Param is NULL");	
		return NULL;
	}

	ev_io* ioData = (ev_io*)call_back;
	switch(type)
	{
		case LWEV::WATCHER_IO_FD:
			return &(ioData->fd);
			break;
	}

	return NULL;
}



//Timer
typedef void (__cdecl *cb_timer)(struct ev_loop *,ev_timer *,int);

LWRESULT Cx_WatcherTimer::Init(WATCHER_CALLBACK call_back, va_list argp)
{
	ev_tstamp timeout = va_arg( argp, ev_tstamp); 
	ev_tstamp repeat  = va_arg( argp, ev_tstamp); 
	
	ev_timer_init(&mWatcher, (cb_timer)call_back, timeout, repeat);
   
	return LWDP_OK;
}
LWRESULT Cx_WatcherTimer::WatcherStart()
{
	ev_timer_start(mLoop, &mWatcher);
	return LWDP_OK;
}
LWRESULT Cx_WatcherTimer::WatcherStop()
{
	ev_timer_stop(mLoop, &mWatcher);
	return LWDP_OK;
}

void* Cx_WatcherTimer::GetTypeData(CBHandle call_back, LWEV::CALLBACK_DATA_TYPE type)
{
	if(!call_back)
	{
		PLATFORM_LOG(LWDP_EVENT_LOG, LWDP_LOG_ERR, "Cx_WatcherTimer::GetTypeData Param is NULL");	
		return NULL;
	}
	
	return NULL;
}

LWDP_NAMESPACE_END;

