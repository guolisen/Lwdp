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

#include "PerfMgrDef.h"
#include "Cx_PerfMgr_Cps.h"



LWDP_NAMESPACE_BEGIN;


Cx_PerfMgr_Cps::Cx_PerfMgr_Cps():mExtFps(0),mFrames(0),mUpdateTime(1000000)
{
#ifdef LWDP_PLATFORM_DEF_WIN32
	QueryPerformanceCounter(&mTvLast); 
#else 
	gettimeofday (&mTvLast, NULL);
#endif
}

Cx_PerfMgr_Cps::~Cx_PerfMgr_Cps()
{

}

void Cx_PerfMgr_Cps::Reset()
{
	mExtFps = 0;
	mFrames = 0;
	mUpdateTime = 0;
}
	
double Cx_PerfMgr_Cps::GetCps()
{
	return mExtFps;
}
void   Cx_PerfMgr_Cps::Update()
{
#ifdef LWDP_PLATFORM_DEF_WIN32
	mFrames++;
	LARGE_INTEGER tvNow;
	QueryPerformanceCounter(&tvNow); 
	uint32_ diffTime = (uint32_)(tvNow.QuadPart - mTvLast.QuadPart); //us

    if(diffTime > mUpdateTime)
    {
    	mExtFps = ((double)mFrames / (double)diffTime) * 1000000.0;
        QueryPerformanceCounter(&mTvLast); 
        mFrames = 0;
    }
#else 
	mFrames++;
	struct timeval tvNow;

	gettimeofday (&tvNow, NULL);

	uint32_ diffTime = (tvNow.tv_sec - mTvLast.tv_sec) * 1000000 +
		               (tvNow.tv_usec - mTvLast.tv_usec);

    if(diffTime > mUpdateTime)
    {
    	mExtFps = ((double)mFrames / (double)diffTime) * 1000000.0;

        gettimeofday(&mTvLast, NULL);

        mFrames = 0;
    }
#endif

}
/////////////////////////////////////////////////////////


void Cx_PerfMgr_timer::Start()
{

}

double Cx_PerfMgr_timer::Now()
{
	return 0.0;
}
double Cx_PerfMgr_timer::Stop()
{
	return 0.0;
}
	

LWDP_NAMESPACE_END;

