#ifndef EVENTMGR_INTERFACE_H
#define EVENTMGR_INTERFACE_H

#include <Interface/Ix_Object.h>
#include "Id_EventMgr.h"

LWDP_NAMESPACE_BEGIN;
 

INTERFACE_BEGIN(EventMgr)
	virtual LWRESULT Init() = 0;


	
INTERFACE_END();


LWDP_NAMESPACE_END;

#endif // LOGMGR_INTERFACE_H
