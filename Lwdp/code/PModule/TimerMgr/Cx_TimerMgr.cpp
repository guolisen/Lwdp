/*! \file Cx_TimerMgr.cpp
 *  \brief TimerMgr Module Impl
 *  \author Guolisen, LwDp
 *  \date   2012.10.17
 */
 
#include <LwDp.h>
#include <PluginInc.h>
#include <iostream>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>

#include "TimerMgrDef.h"
#include "Cx_TimerMgr.h"



LWDP_NAMESPACE_BEGIN;

TIMER_MAP Cx_TimerMgr::mTimerMap;


Cx_TimerMgr::Cx_TimerMgr()
{
}

Cx_TimerMgr::~Cx_TimerMgr()
{
}

LWRESULT Cx_TimerMgr::Init()
{
	lw_log_info(LWDP_TIMER_LOG, __T("Cx_TimerMgr::Init OK!"));



	return LWDP_OK;
}

void timer_callback(LoopHandle loop, CBHandle w, int revents)
{
	GET_OBJECT_VOID(EventMgr, iEventMgr);
	WatcherHandle wh = iEventMgr->CBToW(w);
	TIMER_MAP::iterator iter = Cx_TimerMgr::mTimerMap.find((TimerHandle)wh);
	if(iter != Cx_TimerMgr::mTimerMap.end())
	{
		iter->second.callBack(iter->second.arg);
	}
}

TimerHandle Cx_TimerMgr::CreateTimer(TIMER_CALLBACK call_back, void* arg, double_ delay, double_ repeat)
{
	GET_OBJECT_RET(EventMgr, iEventMgr, NULL);
	WatcherHandle wh = iEventMgr->CreateWatcher(LWEV::WATCHER_TIMER, timer_callback, delay, repeat);
	if(!wh)
	{
		PLATFORM_LOG(LWDP_TIMER_LOG, LWDP_LOG_ERR, "Get Timer Watcher Error!");
		return NULL;
	}

	TIMER_ENTRY timerEntry;
	timerEntry.arg = arg;
	timerEntry.callBack = call_back;

	Cx_TimerMgr::mTimerMap.insert(std::make_pair((TimerHandle)wh, timerEntry));

	return (TimerHandle)wh;
}

LWRESULT Cx_TimerMgr::DestoryTimer(TimerHandle timer_handle)
{
	GET_OBJECT_RET(EventMgr, iEventMgr, NULL);
	TIMER_MAP::iterator iter = Cx_TimerMgr::mTimerMap.find(timer_handle);
	if(iter != Cx_TimerMgr::mTimerMap.end())
	{
		TimerStop(timer_handle);
		Cx_TimerMgr::mTimerMap.erase(iter);

		return iEventMgr->DestoryWatcher((WatcherHandle)timer_handle);
	}

	return LWDP_OK;
}

LWRESULT Cx_TimerMgr::TimerStart(TimerHandle timer_handle)
{
	GET_OBJECT_RET(EventMgr, iEventMgr, NULL);
	return iEventMgr->WatcherStart((WatcherHandle)timer_handle);
}

LWRESULT Cx_TimerMgr::TimerStop(TimerHandle timer_handle)
{
	GET_OBJECT_RET(EventMgr, iEventMgr, NULL);
	return iEventMgr->WatcherStop((WatcherHandle)timer_handle);
}

LWDP_NAMESPACE_END;

