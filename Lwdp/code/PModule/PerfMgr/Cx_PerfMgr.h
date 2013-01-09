/*! \file Cx_PerfMgr.h
 *  \brief PerfMgr Module Impl
 *  \author Guolisen, LwDp
 *  \date   2012.12.15
 */
 
#ifndef CX_PERF_MANAGER_H
#define CX_PERF_MANAGER_H

#include <boost/shared_ptr.hpp>
#include <list>
#include <Interface/PerfMgr/Ix_PerfMgr.h>
//#include <Interface/ScriptMgr/Ix_ScriptMgr.h>
#include "PerfMgrDef.h"

#include <iostream>


LWDP_NAMESPACE_BEGIN;

class Cx_PerfMgr
    : public Ix_PerfMgr
{
    X3BEGIN_CLASS_DECLARE(Cx_PerfMgr)
        X3DEFINE_INTERFACE_ENTRY(Ix_PerfMgr)
    X3END_CLASS_DECLARE()
protected:
	Cx_PerfMgr();
	virtual ~Cx_PerfMgr();

protected:
	LWRESULT Init();
	

};


LWDP_NAMESPACE_END;

#endif
