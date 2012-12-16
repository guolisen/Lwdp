/*! \file Cx_TimerMgr.h
 *  \brief TimerMgr Module Impl
 *  \author Guolisen, LwDp
 *  \date   2012.12.15
 */
 
#ifndef CX_TIMER_MANAGER_H
#define CX_TIMER_MANAGER_H

#include <boost/shared_ptr.hpp>
#include <list>
#include <Interface/TimerMgr/Ix_TimerMgr.h>
//#include <Interface/ScriptMgr/Ix_ScriptMgr.h>
#include "TimerMgrDef.h"

#include <iostream>

LWDP_NAMESPACE_BEGIN;

struct TIMER_ENTRY
{
	void* arg;
	TIMER_CALLBACK callBack;

	TIMER_ENTRY()
	{
		arg = NULL;
		callBack = NULL;
	};

	TIMER_ENTRY(const TIMER_ENTRY& other)
	{
		arg = other.arg;
		callBack = other.callBack;
	}
};

typedef std::map<TimerHandle, TIMER_ENTRY> TIMER_MAP;

class Cx_TimerMgr
    : public Ix_TimerMgr
{
    X3BEGIN_CLASS_DECLARE(Cx_TimerMgr)
        X3DEFINE_INTERFACE_ENTRY(Ix_TimerMgr)
    X3END_CLASS_DECLARE()
protected:
	Cx_TimerMgr();
	virtual ~Cx_TimerMgr();

protected:
	virtual LWRESULT Init();
	virtual TimerHandle CreateTimer(TIMER_CALLBACK call_back, void* arg, double_ delay, double_ repeat);
	virtual LWRESULT    DestoryTimer(TimerHandle timer_handle);

	virtual LWRESULT TimerStart(TimerHandle timer_handle);
	virtual LWRESULT TimerStop(TimerHandle timer_handle);
	
public:
	static TIMER_MAP mTimerMap;
};


LWDP_NAMESPACE_END;

#endif
