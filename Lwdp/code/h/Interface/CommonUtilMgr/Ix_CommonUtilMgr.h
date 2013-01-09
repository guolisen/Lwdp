#ifndef COMMONUTILMGR_INTERFACE_H
#define COMMONUTILMGR_INTERFACE_H

#include <Interface/Ix_Object.h>
#include "Id_CommonUtilMgr.h"

LWDP_NAMESPACE_BEGIN;
 

INTERFACE_BEGIN(CommonUtilMgr)
	virtual LWRESULT Init() = 0;
	
INTERFACE_END();


LWDP_NAMESPACE_END;

#endif // LOGMGR_INTERFACE_H
