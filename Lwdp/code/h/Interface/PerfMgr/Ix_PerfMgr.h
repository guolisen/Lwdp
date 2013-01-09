#ifndef PERFMGR_INTERFACE_H
#define PERFMGR_INTERFACE_H

#include <Interface/Ix_Object.h>
#include "Id_PerfMgr.h"

LWDP_NAMESPACE_BEGIN;
 

INTERFACE_BEGIN(PerfMgr)
	virtual LWRESULT Init() = 0;
	
INTERFACE_END();


LWDP_NAMESPACE_END;

#endif // LOGMGR_INTERFACE_H
