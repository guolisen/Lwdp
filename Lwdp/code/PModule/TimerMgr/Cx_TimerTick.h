/*! \file Cx_TimerTick.h
 *  \brief TimerTick Module Impl
 *  \author Guolisen, LwDp
 *  \date   2012.12.15
 */
 
#ifndef CX_TIMER_TICK_MANAGER_H
#define CX_TIMER_TICK_MANAGER_H

#include <boost/shared_ptr.hpp>
#include <list>
#include <Interface/TimerMgr/Ix_TimerMgr.h>
//#include <Interface/ScriptMgr/Ix_ScriptMgr.h>
#include "TimerMgrDef.h"

#include <iostream>

LWDP_NAMESPACE_BEGIN;


#ifdef LWDP_PLATFORM_DEF_WIN32

class Cx_TimerTick
    : public Ix_TimerTick
{
    X3BEGIN_CLASS_DECLARE(Cx_TimerTick)
        X3DEFINE_INTERFACE_ENTRY(Ix_TimerTick)
    X3END_CLASS_DECLARE()
protected:
	Cx_TimerTick();
	virtual ~Cx_TimerTick();

protected:
	virtual bool SetOption(const std::string& strKey, const void* pValue);

	/** Resets timer */
	virtual void Reset();

	/** Returns milliseconds since initialisation or last reset */
	virtual uint32_ GetMilliseconds();

	/** Returns microseconds since initialisation or last reset */
	virtual uint32_ GetMicroseconds();

	/** Returns milliseconds since initialisation or last reset, only CPU time measured */
	virtual uint32_ GetMillisecondsCPU();

	/** Returns microseconds since initialisation or last reset, only CPU time measured */
	virtual uint32_ GetMicrosecondsCPU();
	
private:
	clock_t mZeroClock;

    DWORD mStartTick;
	LONGLONG mLastTime;
    LARGE_INTEGER mStartTime;
    LARGE_INTEGER mFrequency;

	DWORD_PTR mTimerMask;
};

//#elif LWDP_PLATFORM_DEF_LINUX
#else

class Cx_TimerTick
    : public Ix_TimerTick
{
    X3BEGIN_CLASS_DECLARE(Cx_TimerTick)
        X3DEFINE_INTERFACE_ENTRY(Ix_TimerTick)
    X3END_CLASS_DECLARE()
protected:
	Cx_TimerTick();
	virtual ~Cx_TimerTick();

protected:
    bool SetOption(const std::string& strKey, const void* pValue);

	/** Resets timer */
	void Reset();

	/** Returns milliseconds since initialisation or last reset */
	uint32_ GetMilliseconds();

	/** Returns microseconds since initialisation or last reset */
	uint32_ GetMicroseconds();

	/** Returns milliseconds since initialisation or last reset, only CPU time measured */
	uint32_ GetMillisecondsCPU();

	/** Returns microseconds since initialisation or last reset, only CPU time measured */
	uint32_ GetMicrosecondsCPU();

private:
	struct timeval start;
	clock_t zeroClock;

};


#endif
LWDP_NAMESPACE_END;

#endif
