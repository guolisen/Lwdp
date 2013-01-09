/*! \file Cx_CommonUtilMgr.h
 *  \brief CommonUtilMgr Module Impl
 *  \author Guolisen, LwDp
 *  \date   2012.12.15
 */
 
#ifndef CX_COMMONUTIL_MANAGER_H
#define CX_COMMONUTIL_MANAGER_H

#include <boost/shared_ptr.hpp>
#include <list>
#include <Interface/CommonUtilMgr/Ix_CommonUtilMgr.h>
//#include <Interface/ScriptMgr/Ix_ScriptMgr.h>
#include "CommonUtilMgrDef.h"

#include <iostream>


LWDP_NAMESPACE_BEGIN;

class Cx_CommonUtilMgr
    : public Ix_CommonUtilMgr
{
    X3BEGIN_CLASS_DECLARE(Cx_CommonUtilMgr)
        X3DEFINE_INTERFACE_ENTRY(Ix_CommonUtilMgr)
    X3END_CLASS_DECLARE()
protected:
	Cx_CommonUtilMgr();
	virtual ~Cx_CommonUtilMgr();

protected:
	LWRESULT Init();
	

};


LWDP_NAMESPACE_END;

#endif
