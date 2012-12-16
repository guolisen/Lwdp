#ifndef TIMERMGR_INTERFACE_H
#define TIMERMGR_INTERFACE_H

#include <Interface/Ix_Object.h>
#include "Id_TimerMgr.h"

LWDP_NAMESPACE_BEGIN;
 

INTERFACE_BEGIN(TimerMgr)
	virtual LWRESULT Init() = 0;
	
INTERFACE_END();


LWDP_NAMESPACE_END;

#endif // LOGMGR_INTERFACE_H
