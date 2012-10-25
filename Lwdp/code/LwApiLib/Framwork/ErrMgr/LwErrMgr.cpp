// Copyright (C), 2006��2011, Potevio Institute of Technology.
/************************************************************
*  File name:   LwLogMessage.cpp  	
*  Author:      guolisen
*  Version:   	v1.0
*  Create Date: 2011-08-11
***********************************************************/
// 2011.08.11: Create By Guolisen

#include <LwDp.h>
#include <LwApiLib/Framwork/ErrMgr/LwErrMgr.h>

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;

#if 0
static LWDP_ERRNO_MESSAGE platformErrInfo[] = 
{
#undef  ERRNO_DB
#define ERRNO_DB(x, y, z) {y, __T("LWDP_")__T(#x), __T(z)},
    #include <LwMErrnoBase.h> 
    {-1, __T("ENUM END"), __T("NONE")}
};


LWRESULT ShowErrnoInfo()
{
    int32_ i = 0;

    printf ("\nErrNo        errTag       errInfo\n");
    
    for (i = 0; i < LWDP_ERRNO_NUMCATS; i++)
    {
        printf ("%-15x  %10d     %10s\n",
                platformErrInfo[i].errno, platformErrInfo[i].errTag);
    }

    return LWDP_OK;
}
#endif


int Test()
{
    return LWDP_OK;
}
EXTERN_C_END;
LWDP_NAMESPACE_END;
    


