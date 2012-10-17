#ifndef CX_DATACOLLECTTOOL_MANAGER_H
#define CX_DATACOLLECTTOOL_MANAGER_H

#include <boost/shared_ptr.hpp>
#include <list>
#include <Interface/DctMgr/Ix_DctMgr.h>
#include <Interface/ScriptMgr/Ix_ScriptMgr.h>
#include "DctTagDef.h"

LWDP_NAMESPACE_BEGIN;

class Cx_DctMgr
    : public Ix_DctMgr
{
friend class ADctInit;

    X3BEGIN_CLASS_DECLARE(Cx_DctMgr)
        X3DEFINE_INTERFACE_ENTRY(Ix_DctMgr)
    X3END_CLASS_DECLARE()
protected:
	Cx_DctMgr();
	virtual ~Cx_DctMgr();

protected:
	LWRESULT Init();

};


LWDP_NAMESPACE_END;

#endif
