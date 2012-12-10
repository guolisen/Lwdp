/*! \file LwErrnoDef.h
 *  \brief Module Id Define
 *  \author Guolisen, LwDp
 *  \date   2011.08.12
 */

#ifndef LW_ERRNO_DEF_H_
#define LW_ERRNO_DEF_H_


LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;

#define M_errno			    (0x80000000)	
#define M_logMsg    		(1 << 16)
#define M_pluginmanager		(2 << 16)
#define M_pluginloader		(3 << 16)
#define M_api				(4 << 16)
#define M_framework			(5 << 16)
#define M_configmgr			(6 << 16)
#define M_scriptmgr			(7 << 16)
#define M_logmgr			(8 << 16)
#define M_dctmgr			(9 << 16)
#define M_luamgr			(10 << 16)
#define M_zmqmgr			(11 << 16)
#define M_eventmgr			(12 << 16)


enum _ERROR_CAT_NUM
{
#undef  ERRNO_DB
#define ERRNO_DB(x, y, z)		 LWDP_##x = y, 
    #include <LwMErrnoBase.h>  
    LWDP_ERRNO_NUMCATS
};

    
#include <LwApiLib/Framwork/ErrMgr/LwErrMgr.h>
EXTERN_C_END;
LWDP_NAMESPACE_END;
#endif




 

