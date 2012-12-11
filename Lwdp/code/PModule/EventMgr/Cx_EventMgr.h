/*! \file Cx_EventMgr.h
 *  \brief EventMgr Module Impl
 *  \author Guolisen, LwDp
 *  \date   2012.10.17
 */
 
#ifndef CX_EVENT_MANAGER_H
#define CX_EVENT_MANAGER_H

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <boost/shared_ptr.hpp>
#include <list>
#include <Interface/EventMgr/Ix_EventMgr.h>
//#include <Interface/ScriptMgr/Ix_ScriptMgr.h>
#include "EventTagDef.h"

#include <iostream>
#include <LwApiLib/ComLib/Libev/ev.h>


LWDP_NAMESPACE_BEGIN;

class Ix_Watcher
{
public:
	virtual LWRESULT Init(WATCHER_CALLBACK call_back, va_list argp) = 0;
	virtual LWRESULT WatcherStart() = 0;
	virtual LWRESULT WatcherStop() = 0;
};

class Ix_WatcherFactory
{
public:
	virtual Ix_Watcher* CreateObject(struct ev_loop* loop) = 0;
	virtual uint32_ GetType() = 0;
};


typedef struct stru_watcher_tag
{
	uint32_ watcherType;
	Ix_Watcher* watcherObject;

	stru_watcher_tag(const stru_watcher_tag& other)
	{
		watcherType = other.watcherType;
		watcherObject = other.watcherObject;
	};
}WATCHER_ENTRY;


typedef std::list<WATCHER_ENTRY*> WATCHER_LIST;
typedef boost::shared_ptr<Ix_WatcherFactory> WatcherFactory_Ptr;
typedef std::list<WatcherFactory_Ptr> WATCHER_FACTORY_LIST;

	
class Cx_EventMgr
    : public Ix_EventMgr
{
    X3BEGIN_CLASS_DECLARE(Cx_EventMgr)
        X3DEFINE_INTERFACE_ENTRY(Ix_EventMgr)
    X3END_CLASS_DECLARE()
protected:
	Cx_EventMgr();
	virtual ~Cx_EventMgr();

protected:
	//main loop
	virtual LWRESULT InitLoop(uint32_ flags);
	virtual LWRESULT RunLoop(uint32_ flags);

	//watcher
	virtual WatcherHandle CreateWatcher(LWEV::WATCHER_TYPE watcher_type, WATCHER_CALLBACK call_back, ...);
	virtual LWRESULT DestoryWatcher(WatcherHandle watcher_handle);

	virtual LWRESULT WatcherStart(WatcherHandle watcher_handle);
	virtual LWRESULT WatcherStop(WatcherHandle watcher_handle);

protected:
	int32_ 				mInitTag;
	struct ev_loop* 	mMainLoop;
	WATCHER_LIST 		mWatcherList;
	WATCHER_FACTORY_LIST 	mWatcherFactory;
};


LWDP_NAMESPACE_END;

#endif
