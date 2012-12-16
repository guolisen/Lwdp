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
	LWRESULT Init();
	

};


LWDP_NAMESPACE_END;

#endif
