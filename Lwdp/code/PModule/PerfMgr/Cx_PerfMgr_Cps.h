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
#include <Interface/CommonUtilMgr/Ix_CommonUtilMgr.h>

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
	uint32_ mTvLast;
	
	double  mExtFps;
	uint32_ mFrames;
	uint32_ mUpdateTime;
};



class Cx_PerfMgr_timer
    : public Ix_PerfMgr_timer
{
    X3BEGIN_CLASS_DECLARE(Cx_PerfMgr_timer)
        X3DEFINE_INTERFACE_ENTRY(Ix_PerfMgr_timer)
    X3END_CLASS_DECLARE()
public:
	Cx_PerfMgr_timer();
	virtual ~Cx_PerfMgr_timer();

protected:
	virtual void    Start();
	virtual uint32_ Now();
	virtual uint32_ End();
	virtual void    Reset();
	
protected:	
	uint32_ mStartTick;

};


LWDP_NAMESPACE_END;

#endif
