/*! \file LwErrMgr.h
 *  \brief Error Manager Module
 *  \author Guolisen, LwDp
 *  \date   2011.08.12
 */

#ifndef LW_ERRNO_MGR_H_
#define LW_ERRNO_MGR_H_


LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;


typedef struct _tag_err_message{
    int32_ errno;
    const tchar_* errTag;
    const tchar_* errInfo;
}LWDP_ERRNO_MESSAGE;


EXTERN_C_END;
LWDP_NAMESPACE_END;
#endif




 


