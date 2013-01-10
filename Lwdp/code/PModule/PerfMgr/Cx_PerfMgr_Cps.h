/*! \file Cx_PerfMgr_Cps.h
 *  \brief PerfMgr Module Impl
 *  \author Guolisen, LwDp
 *  \date   2012.12.15
 */
 
#ifndef CX_PERF_CPS_MANAGER_H
#define CX_PERF_CPS_MANAGER_H

#include <boost/shared_ptr.hpp>
#include <list>
#include <Interface/PerfMgr/Ix_PerfMgr.h>
//#include <Interface/ScriptMgr/Ix_ScriptMgr.h>
#include "PerfMgrDef.h"

#include <iostream>


LWDP_NAMESPACE_BEGIN;

class Cx_PerfMgr_Cps
    : public Ix_PerfMgr_Cps
{
    X3BEGIN_CLASS_DECLARE(Cx_PerfMgr_Cps)
        X3DEFINE_INTERFACE_ENTRY(Ix_PerfMgr_Cps)
    X3END_CLASS_DECLARE()
protected:
	Cx_PerfMgr_Cps();
	virtual ~Cx_PerfMgr_Cps();

protected:
	virtual double GetCps();
	virtual void   Update();
	virtual void   Reset();
protected:	
#ifdef LWDP_PLATFORM_DEF_WIN32
	LARGE_INTEGER mTvLast;
#else 	
	struct timeval mTvLast;
#endif

	double  mExtFps;
	uint32_ mFrames;
	uint32_ mUpdateTime;
};


LWDP_NAMESPACE_END;

#endif
