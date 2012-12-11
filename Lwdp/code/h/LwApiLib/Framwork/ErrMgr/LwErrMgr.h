/*! \file LwErrMgr.h
 *  \brief Error Manager Module
 *  \author Guolisen, LwDp
 *  \date   2011.08.12
 */

#ifndef _LW_ERRNO_MGR_H_
#define _LW_ERRNO_MGR_H_


LWDP_NAMESPACE_BEGIN;
//EXTERN_C_BEGIN;


typedef struct _tag_err_msg{
    int errno;
    char* errTag;
    char* errInfo;
}LWDP_ERRNO_MSG;


//EXTERN_C_END;
LWDP_NAMESPACE_END;
#endif




 


