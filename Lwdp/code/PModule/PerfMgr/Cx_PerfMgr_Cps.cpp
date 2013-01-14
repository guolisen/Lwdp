/*! \file Cx_PerfMgr_Cps.cpp
 *  \brief PerfMgr Module Impl
 *  \author Guolisen, LwDp
 *  \date   2012.10.17
 */
 
#include <LwDp.h>
#include <PluginInc.h>
#include <iostream>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>
#include <Interface/TimerMgr/Ix_TimerMgr.h>

#include "PerfMgrDef.h"
#include "Cx_PerfMgr.h"
#include "Cx_PerfMgr_Cps.h"



LWDP_NAMESPACE_BEGIN;



Cx_PerfMgr_Cps::Cx_PerfMgr_Cps():mExtFps(0), mKb(0),mFrames(0),mUpdateTime(1000000)
{
	GET_OBJECT_VOID(TimerTick, iTimerTick);
	mTvLast = iTimerTick->GetMilliseconds();
}

Cx_PerfMgr_Cps::~Cx_PerfMgr_Cps()
{

}

void Cx_PerfMgr_Cps::Reset()
{
	if(!gPerfSwitch)
		return; 
	GET_OBJECT_VOID(TimerTick, iTimerTick);
	mTvLast = iTimerTick->GetMilliseconds();
	mExtFps = 0;
	mFrames = 0;
	mUpdateTime = 0;
}
	
double Cx_PerfMgr_Cps::GetCps()
{
	if(!gPerfSwitch)
		return 0.0; 
	return mExtFps;
}
void   Cx_PerfMgr_Cps::Update()
{
	if(!gPerfSwitch)
		return; 
	GET_OBJECT_VOID(TimerTick, iTimerTick);
	mFrames++;
	uint32_ tvNow = iTimerTick->GetMilliseconds();
	uint32_ diffTime = (tvNow - mTvLast) * 1000;
		              
    if(diffTime > mUpdateTime)
    {
    	mExtFps = ((double)mFrames / (double)diffTime) * 1000000.0;
        mTvLast = iTimerTick->GetMilliseconds();
        mFrames = 0;
    }
}

double Cx_PerfMgr_Cps::GetKbps()
{
	if(!gPerfSwitch)
		return 0.0; 
	return mKb;
}
void   Cx_PerfMgr_Cps::Update(uint32_ byte)
{
	if(!gPerfSwitch)
		return; 
	GET_OBJECT_VOID(TimerTick, iTimerTick);

	uint32_ tvNow = iTimerTick->GetMilliseconds();
	uint32_ diffTime = (tvNow - mTvLast) * 1000;
		              
    if(diffTime > mUpdateTime)
    {
    	mExtMbps = ((double)mKb / (double)diffTime) * 1000000.0;
        mTvLast = iTimerTick->GetMilliseconds();
        mKb = 0;
    }

	//mutex.lock();
	mKb += byte / 1024.0;
	//mutex.unlock();
}
/////////////////////////////////////////////////////////

Cx_PerfMgr_timer::Cx_PerfMgr_timer():mStartTick(0)
{

}

Cx_PerfMgr_timer::~Cx_PerfMgr_timer()
{

}

void Cx_PerfMgr_timer::Reset()
{
	if(!gPerfSwitch)
		return; 
	GET_OBJECT_VOID(TimerTick, iTimerTick);
	mStartTick   = iTimerTick->GetMilliseconds();
}
void Cx_PerfMgr_timer::Start()
{
	if(!gPerfSwitch)
		return; 
	GET_OBJECT_VOID(TimerTick, iTimerTick);

	mStartTick = iTimerTick->GetMilliseconds();
}

uint32_ Cx_PerfMgr_timer::Now()
{
	if(!gPerfSwitch)
		return 0; 
	GET_OBJECT_RET(TimerTick, iTimerTick, 0);
	uint32_ nowTick = iTimerTick->GetMilliseconds();
	return (nowTick - mStartTick);
}
uint32_ Cx_PerfMgr_timer::End()
{
	if(!gPerfSwitch)
		return 0; 
	GET_OBJECT_RET(TimerTick, iTimerTick, 0);
	uint32_ nowTick = iTimerTick->GetMilliseconds();
	uint32_ retTick = nowTick - mStartTick;
	mStartTick = nowTick;

	return retTick;
}
	

LWDP_NAMESPACE_END;

