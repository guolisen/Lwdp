/*! \file Cx_Watcher.cpp
 *  \brief Watcher  Impl
 *  \author Guolisen, LwDp
 *  \date   2012.12.10
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>;   
#include "Cx_EventMgr.h"
#include "Cx_Watcher.h"


//IO
LWRESULT Cx_WatcherIO::Init(WATCHER_CALLBACK call_back, va_list argp)
{
	int fd = va_arg( argp, int); 
	int event = va_arg( argp, int); 
  
	ev_io_init(&mWatcher, call_back, fd, event);
   
	return LWDP_OK;
}

LWRESULT Cx_WatcherIO::WatcherStart()
{
	ev_io_start(mLoop, &mWatcher);
	return LWDP_OK;
}
LWRESULT Cx_WatcherIO::WatcherStop()
{
	ev_timer_stop(mLoop, &mWatcher);
	return LWDP_OK;
}

//Timer
LWRESULT Cx_WatcherTimer::Init(WATCHER_CALLBACK call_back, va_list argp)
{
	ev_tstamp timeout = va_arg( argp, ev_tstamp); 
	ev_tstamp repeat = va_arg( argp, ev_tstamp); 
	
	ev_timer_init(&mWatcher, call_back, timeout, repeat);
   
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




