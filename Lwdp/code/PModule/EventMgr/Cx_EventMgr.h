/*! \file Cx_EventMgr.h
 *  \brief EventMgr Module Impl
 *  \author Guolisen, LwDp
 *  \date   2012.10.17
 */
 
#ifndef CX_EVENT_MANAGER_H
#define CX_EVENT_MANAGER_H

#include <boost/shared_ptr.hpp>
#include <list>
#include <Interface/EventMgr/Ix_EventMgr.h>
//#include <Interface/ScriptMgr/Ix_ScriptMgr.h>
#include "EventTagDef.h"

#include <iostream>



LWDP_NAMESPACE_BEGIN;

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
	LWRESULT Init();

	
};


LWDP_NAMESPACE_END;

#endif
