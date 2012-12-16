#ifndef CONSOLEMGR_INTERFACE_H
#define CONSOLEMGR_INTERFACE_H

#include <Interface/Ix_Object.h>
#include "Id_ConsoleMgr.h"

LWDP_NAMESPACE_BEGIN;
 

INTERFACE_BEGIN(ConsoleMgr)
	virtual LWRESULT Init() = 0;
	
INTERFACE_END();


LWDP_NAMESPACE_END;

#endif // 
