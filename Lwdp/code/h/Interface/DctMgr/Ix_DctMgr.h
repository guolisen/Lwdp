#ifndef DCTMGR_INTERFACE_H
#define DCTMGR_INTERFACE_H

#include <Interface/Ix_Object.h>
#include "Id_DctMgr.h"

LWDP_NAMESPACE_BEGIN;


INTERFACE_BEGIN(DctMgr)
	virtual LWRESULT Init() = 0;

INTERFACE_END();


LWDP_NAMESPACE_END;

#endif // LOGMGR_INTERFACE_H
