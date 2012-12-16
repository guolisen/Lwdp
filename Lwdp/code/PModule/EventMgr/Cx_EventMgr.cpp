/*! \file Cx_EventMgr.cpp
 *  \brief EventMgr Module Impl
 *  \author Guolisen, LwDp
 *  \date   2012.10.17
 */
 
#include <LwDp.h>
#include <PluginInc.h>
#include <iostream>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>

#include "EventTagDef.h"
#include "Cx_EventMgr.h"
#include "Cx_Watcher.h"


LWDP_NAMESPACE_BEGIN;


Cx_EventMgr::Cx_EventMgr():	mInitTag(0), mMainLoop(NULL)
{
}

Cx_EventMgr::~Cx_EventMgr()
{
}


//main loop
LWRESULT Cx_EventMgr::InitLoop(uint32_ flags)
{
	if(mInitTag)
	{
		return LWDP_OK;
	}
	lw_log_info(LWDP_EVENT_LOG, __T("Cx_EventMgr::InitLoop OK!"));

	mMainLoop = ev_loop_new(LWBACKEND_SELECT);
	if(!mMainLoop)
	{
		PLATFORM_LOG(LWDP_EVENT_LOG, LWDP_LOG_ERR, "ev_loop_new return null");	
		return LWDP_INIT_LOOP_ERROR;
	}

	WatcherFactory_Ptr ioFactory(new IOWatcherFactory());
	mWatcherFactory.push_back(ioFactory);
	WatcherFactory_Ptr timerFactory(new TimerWatcherFactory());
	mWatcherFactory.push_back(timerFactory);	
			
	mInitTag = 1;
	return LWDP_OK;
}

LWRESULT Cx_EventMgr::RunLoop(uint32_ flags)
{
	ev_run(mMainLoop, flags);
	return LWDP_OK;
}

//watcher
WatcherHandle Cx_EventMgr::CreateWatcher(LWEV::WATCHER_TYPE watcher_type, WATCHER_CALLBACK call_back, ...)
{
	WATCHER_FACTORY_LIST::iterator iter;
	FOREACH_STL(iter, mWatcherFactory)
	{
		if((*iter)->GetType() == (uint32_)watcher_type)
			break;
	}

	if(iter != mWatcherFactory.end())
	{
		WATCHER_ENTRY* tmpEntry = (WATCHER_ENTRY*)malloc(sizeof(WATCHER_ENTRY));
		ASSERT_CHECK_RET(LWDP_EVENT_LOG, NULL, tmpEntry, "new WATCHER_ENTRY Error");
		tmpEntry->watcherType   = watcher_type;
		tmpEntry->watcherObject = (*iter)->CreateObject(mMainLoop);

		va_list arg;  
		va_start(arg, call_back); 
        tmpEntry->watcherObject->Init(call_back, arg);
		va_end(arg);  
		
		mWatcherList.push_back(tmpEntry);

		return (WatcherHandle)tmpEntry;
	}

	return (WatcherHandle)NULL;
}

LWRESULT Cx_EventMgr::DestoryWatcher(WatcherHandle watcher_handle)
{
	WATCHER_LIST::iterator iter;
	FOREACH_STL(iter, mWatcherList)
	{
		if(watcher_handle == (WatcherHandle)(*iter)) 
		{
			WatcherStop(watcher_handle);
			///!!!!!!!!!!!!!!!!!!
			mWatcherList.erase(iter);
			break;
		}
	}

	return LWDP_OK;
}

LWRESULT Cx_EventMgr::WatcherStart(WatcherHandle watcher_handle)
{
	if(!watcher_handle)
		return LWDP_POINTER_IS_NULL;

	WATCHER_ENTRY* tmpEntry = (WATCHER_ENTRY*)watcher_handle;
	if(!tmpEntry->watcherObject)
		return LWDP_WATCHER_OBJECT_NULL_ERROR;
	
	RINOK(tmpEntry->watcherObject->WatcherStart());
	return LWDP_OK;
}

LWRESULT Cx_EventMgr::WatcherStop(WatcherHandle watcher_handle)
{
	if(!watcher_handle)
		return LWDP_POINTER_IS_NULL;

	WATCHER_ENTRY* tmpEntry = (WATCHER_ENTRY*)watcher_handle;
	if(!tmpEntry->watcherObject)
		return LWDP_WATCHER_OBJECT_NULL_ERROR;
	
	RINOK(tmpEntry->watcherObject->WatcherStop());

	return LWDP_OK;
}

void* Cx_EventMgr::GetCallBackData(CBHandle cb_handle, LWEV::WATCHER_TYPE watcher_type, LWEV::CALLBACK_DATA_TYPE data_type)
{
	switch(watcher_type)
	{
		case LWEV::WATCHER_IO:
			return Cx_WatcherIO::GetTypeData(cb_handle, data_type);
			break;
		case LWEV::WATCHER_TIMER:
			return Cx_WatcherTimer::GetTypeData(cb_handle, data_type);
			break;
	};

	return NULL;
}

WatcherHandle Cx_EventMgr::CBToW(CBHandle cb_handle)
{
	WATCHER_LIST::iterator iter;
	FOREACH_STL(iter, mWatcherList)
	{
		if((*iter)->watcherObject->GetWatcher()== cb_handle)
		{	
			WatcherHandle wh = *iter;
			return (WatcherHandle)wh;
		}
	}

	return NULL;
}

CBHandle Cx_EventMgr::WToCB(WatcherHandle w_handle)
{
	WATCHER_ENTRY* watcher_entry = (WATCHER_ENTRY*)w_handle;
	return (CBHandle)watcher_entry->watcherObject->GetWatcher();
}


LWDP_NAMESPACE_END;

