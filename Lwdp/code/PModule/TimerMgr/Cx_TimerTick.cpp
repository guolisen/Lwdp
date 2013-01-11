/*! \file Cx_TimerTick.cpp
 *  \brief TimerTick Module Impl
 *  \author Guolisen, LwDp
 *  \date   2012.10.17
 */
 
#include <LwDp.h>
#include <PluginInc.h>
#include <iostream>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>

#include "TimerMgrDef.h"
#include "Cx_TimerTick.h"



LWDP_NAMESPACE_BEGIN;


#ifdef LWDP_PLATFORM_DEF_WIN32
Cx_TimerTick::Cx_TimerTick():mTimerMask(0)
{
	Reset();
}

Cx_TimerTick::~Cx_TimerTick()
{
}

bool Cx_TimerTick::SetOption(const std::string& strKey, const void* pValue)
{
	if ( strKey == "QueryAffinityMask" )
	{
		// Telling timer what core to use for a timer read
		DWORD newTimerMask = * static_cast < const DWORD * > ( pValue );

		// Get the current process core mask
		DWORD_PTR procMask;
		DWORD_PTR sysMask;
		GetProcessAffinityMask(GetCurrentProcess(), &procMask, &sysMask);

		// If new mask is 0, then set to default behavior, otherwise check
		// to make sure new timer core mask overlaps with process core mask
		// and that new timer core mask is a power of 2 (i.e. a single core)
		int tmp = (newTimerMask & (newTimerMask-1));
		if( ( newTimerMask == 0 ) ||
			( ( ( newTimerMask & procMask ) != 0 ) && (tmp == 0) ) )
		{
			mTimerMask = newTimerMask;
			return true;
		}
	}

	return false;
}
/** Resets timer */
void Cx_TimerTick::Reset()
{
    // Get the current process core mask
	DWORD_PTR procMask;
	DWORD_PTR sysMask;
	GetProcessAffinityMask(GetCurrentProcess(), &procMask, &sysMask);

	// If procMask is 0, consider there is only one core available
	// (using 0 as procMask will cause an infinite loop below)
	if (procMask == 0)
		procMask = 1;

	// Find the lowest core that this process uses
	if( mTimerMask == 0 )
	{
		mTimerMask = 1;
		while( ( mTimerMask & procMask ) == 0 )
		{
			mTimerMask <<= 1;
		}
	}

	HANDLE thread = GetCurrentThread();

	// Set affinity to the first core
	DWORD_PTR oldMask = SetThreadAffinityMask(thread, mTimerMask);

	// Get the constant frequency
	QueryPerformanceFrequency(&mFrequency);

	// Query the timer
	QueryPerformanceCounter(&mStartTime);
	mStartTick = GetTickCount();

	// Reset affinity
	SetThreadAffinityMask(thread, oldMask);

	mLastTime = 0;
	mZeroClock = clock();
}
/** Returns milliseconds since initialisation or last reset */
uint32_ Cx_TimerTick::GetMilliseconds()
{
    LARGE_INTEGER curTime;

	HANDLE thread = GetCurrentThread();

	// Set affinity to the first core
	DWORD_PTR oldMask = SetThreadAffinityMask(thread, mTimerMask);

	// Query the timer
	QueryPerformanceCounter(&curTime);

	// Reset affinity
	SetThreadAffinityMask(thread, oldMask);

    LONGLONG newTime = curTime.QuadPart - mStartTime.QuadPart;
    
    // scale by 1000 for milliseconds
    unsigned long newTicks = (unsigned long) (1000 * newTime / mFrequency.QuadPart);

    // detect and compensate for performance counter leaps
    // (surprisingly common, see Microsoft KB: Q274323)
    unsigned long check = GetTickCount() - mStartTick;
    signed long msecOff = (signed long)(newTicks - check);
    if (msecOff < -100 || msecOff > 100)
    {
        // We must keep the timer running forward :)
        LONGLONG adjust = (std::min)(msecOff * mFrequency.QuadPart / 1000, newTime - mLastTime);
        mStartTime.QuadPart += adjust;
        newTime -= adjust;

        // Re-calculate milliseconds
        newTicks = (unsigned long) (1000 * newTime / mFrequency.QuadPart);
    }

    // Record last time for adjust
    mLastTime = newTime;

    return newTicks;
}
/** Returns microseconds since initialisation or last reset */
uint32_ Cx_TimerTick::GetMicroseconds()
{
    LARGE_INTEGER curTime;

	HANDLE thread = GetCurrentThread();

	// Set affinity to the first core
	DWORD_PTR oldMask = SetThreadAffinityMask(thread, mTimerMask);

	// Query the timer
	QueryPerformanceCounter(&curTime);

	// Reset affinity
	SetThreadAffinityMask(thread, oldMask);

	LONGLONG newTime = curTime.QuadPart - mStartTime.QuadPart;
    
    // get milliseconds to check against GetTickCount
    unsigned long newTicks = (unsigned long) (1000 * newTime / mFrequency.QuadPart);
    
    // detect and compensate for performance counter leaps
    // (surprisingly common, see Microsoft KB: Q274323)
    unsigned long check = GetTickCount() - mStartTick;
    signed long msecOff = (signed long)(newTicks - check);
    if (msecOff < -100 || msecOff > 100)
    {
        // We must keep the timer running forward :)
        LONGLONG adjust = (std::min)(msecOff * mFrequency.QuadPart / 1000, newTime - mLastTime);
        mStartTime.QuadPart += adjust;
        newTime -= adjust;
    }

    // Record last time for adjust
    mLastTime = newTime;

    // scale by 1000000 for microseconds
    unsigned long newMicro = (unsigned long) (1000000 * newTime / mFrequency.QuadPart);

    return newMicro;
}
/** Returns milliseconds since initialisation or last reset, only CPU time measured */
uint32_ Cx_TimerTick::GetMillisecondsCPU()
{
	clock_t newClock = clock();
	return (unsigned long)( (float)( newClock - mZeroClock ) / ( (float)CLOCKS_PER_SEC / 1000.0 ) ) ;
}
/** Returns microseconds since initialisation or last reset, only CPU time measured */
uint32_ Cx_TimerTick::GetMicrosecondsCPU()
{
	clock_t newClock = clock();
	return (unsigned long)( (float)( newClock - mZeroClock ) / ( (float)CLOCKS_PER_SEC / 1000000.0 ) ) ;
}

#else
Cx_TimerTick::Cx_TimerTick()
{
	Reset();
}

Cx_TimerTick::~Cx_TimerTick()
{
}

bool Cx_TimerTick::SetOption(const String& strKey, const void* pValue)
{
	return true;
}
/** Resets timer */
void Cx_TimerTick::Reset()
{
	zeroClock = clock();
	gettimeofday(&start, NULL);
}

/** Returns milliseconds since initialisation or last reset */
uint32_ Cx_TimerTick::GetMilliseconds()
{
	struct timeval now;
	gettimeofday(&now, NULL);
	return (now.tv_sec-start.tv_sec)*1000+(now.tv_usec-start.tv_usec)/1000;
}

/** Returns microseconds since initialisation or last reset */
uint32_ Cx_TimerTick::GetMicroseconds()
{
	struct timeval now;
	gettimeofday(&now, NULL);
	return (now.tv_sec-start.tv_sec)*1000000+(now.tv_usec-start.tv_usec);

}
/** Returns milliseconds since initialisation or last reset, only CPU time measured */
uint32_ Cx_TimerTick::GetMillisecondsCPU()
{
	clock_t newClock = clock();
	return (unsigned long)((float)(newClock-zeroClock) / ((float)CLOCKS_PER_SEC/1000.0)) ;
}
/** Returns microseconds since initialisation or last reset, only CPU time measured */
uint32_ Cx_TimerTick::GetMicrosecondsCPU()
{
	clock_t newClock = clock();
	return (unsigned long)((float)(newClock-zeroClock) / ((float)CLOCKS_PER_SEC/1000000.0)) ;
}

#endif

LWDP_NAMESPACE_END;

