/*! \file LwDpVersion.h
 *  \brief Version Head
 *  \author Guolisen, LwDp
 *  \date   2011.07.14
 */

#ifndef LW_VERSION_DEF_H_
#define LW_VERSION_DEF_H_



LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;

typedef uint32_ VER_TYPE;

////////////////////////////////////////////////////
//  LWDP Version
////////////////////////////////////////////////////

#define _LWDP_VERSION_MAJOR 1
#define _LWDP_VERSION_MINOR 0
#define _LWDP_VERSION_MAINT 0

/* For print Logo*/

#define LWDP_RUNTIME_NAME    __T("LwDp")
#define LWDP_RUNTIME_VERSION __T("1.0.0")


/* there is now a space between the runtime name, and the version */

#define LWDP_VERSION  LWDP_RUNTIME_NAME __T(" ") LWDP_RUNTIME_VERSION

////////////////////////////////////////
//                                    //
//  |AA AA BB CC|                     //
//  A = _LWDP_PLUGIN_VERSION_MAJOR    //
//  B = _LWDP_PLUGIN_VERSION_MINOR    //
//  C = _LWDP_PLUGIN_VERSION_MAINT    //
//                                    //
////////////////////////////////////////
const VER_TYPE iLWDP_VERSION = (_LWDP_VERSION_MAJOR << 16) |
							   (_LWDP_VERSION_MINOR << 8) |
							   (_LWDP_VERSION_MAINT);


////////////////////////////////////////////////////
//  Plugin Version
////////////////////////////////////////////////////

#define _LWDP_PLUGIN_VERSION_MAJOR 1
#define _LWDP_PLUGIN_VERSION_MINOR 0
#define _LWDP_PLUGIN_VERSION_MAINT 0


#define LWDP_RUNTIME_PLUGIN_NAME    __T("LwDp Plugin")
#define LWDP_RUNTIME_PLUGIN_VERSION __T("1.0.0")


#define LWDP_PLUGIN_VERSION LWDP_RUNTIME_PLUGIN_NAME __T(" ") LWDP_RUNTIME_PLUGIN_VERSION

////////////////////////////////////////
//                                    //
//  |AA AA BB CC|                     //
//  A = _LWDP_PLUGIN_VERSION_MAJOR    //
//  B = _LWDP_PLUGIN_VERSION_MINOR    //
//  C = _LWDP_PLUGIN_VERSION_MAINT    //
//                                    //
////////////////////////////////////////
const VER_TYPE iPLUGIN_VERSION = (_LWDP_PLUGIN_VERSION_MAJOR << 16) |
							   	 (_LWDP_PLUGIN_VERSION_MINOR << 8) |
								 (_LWDP_PLUGIN_VERSION_MAINT);

#if __DCC__ || (__GNUCC__ >= 3) || _WIN32
#define C99_VARIADIC_MACROS
#endif

EXTERN_C_END;
LWDP_NAMESPACE_END;
#endif




 





