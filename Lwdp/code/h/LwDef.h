/*! \file LwTypeDef.h
 *  \brief Define base type
 *  \author Guolisen, LwDp
 *  \date   2011.07.14
 */

#ifndef LW_DEF_H_
#define LW_DEF_H_

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;


#ifndef LWDP_FUNCPTR_DEFINED
#define LWDP_FUNCPTR_DEFINED
#ifdef __cplusplus
//    typedef LWRESULT  (*FUNCPTR) (...);         /* ptr to function returning LWRESULT */
#else
//    typedef LWRESULT  (*FUNCPTR) ();      /* ptr to func returning LWRESULT */
#endif /* __cplusplus */
#endif /* _FUNCPTR_DEFINED */


#define DELETE_SINGLE(x) \
    if(x) \
    { \
       delete (x); \
       (x) = 0; \
    }

#define DELETE_MULTIPLE(x) \
    if(x) \
    { \
        delete [] (x); \
        (x) = 0; \
    }


////////////////////////////////////////////////////
//  UNICODE
////////////////////////////////////////////////////
#ifdef _UNICODE
const int32_ UNICODE_TAG = 1;
#define __T(x)	    L##x
#else
const int32_ UNICODE_TAG = 0;
#define __T(x)      x
#endif

#if 1
#ifdef _WIN32
    #define open    _open
    #define lseek   _lseek
    #define read    _read
    #define write   _write
    #define close   _close
    // #define SHIELD_UNLINK  _unlink
    #define chsize  _chsize
#else
    #define open    open
    #define lseek   lseek
    #define read    read
    #define write   write
    #define close   close
    // #define SHIELD_UNLINK  unlink
    #define chsize  change_size_not_implemented
#endif
#endif


#ifndef RINOK
#define RINOK(x) { LWRESULT __result__ = (x); if (__result__ != LWDP_OK) return __result__; }
#endif

#ifndef RINOKV
#define RINOKV(x) { LWRESULT __result__ = (x); if (__result__ != LWDP_OK) return; }
#endif

#ifndef RINOKR
#define RINOKR(x, ret) { LWRESULT __result__ = (x); if (__result__ != LWDP_OK) return (ret); }
#endif

#define STD_IN	0
#define STD_OUT	1
#define STD_ERR	2

//#ifndef max
//#define max(a,b) ((a)>(b)?(a):(b))
//#endif
//#ifndef min
//#define min(a,b) ((a)<(b)?(a):(b))
//#endif

#ifndef LWDP_MAX_PATH
#define LWDP_MAX_PATH 256
#endif

 
#define INTERFACE_NAME(name) Ix_ ## name
#define INTERFACE_CLSID_NAME(name) CLSID_ ## name


#define INTERFACE_BEGIN(interface_name) \
	class INTERFACE_NAME(interface_name) : public Ix_Object \
	{ \
	public: \
	    DEFINE_IID(INTERFACE_NAME(interface_name)); 


#define INTERFACE_END()  };


#define FOREACH_STL(iter, ele) \
	for(iter = ele.begin(); iter != ele.end(); ++iter)


#define GET_OBJECT_VOID(Module, Var) \
	Cx_Interface<Ix_##Module> (Var)(CLSID_##Module); \
	if((Var).IsNull()) \
	{ \
		lw_log_err(LWDP_MODULE_LOG, "Can't Get Module(%s) Pointer!(%s, %d)", #Module, __FILE__, __LINE__); \
		return; \
	}

#define GET_OBJECT_RET(Module, Var, Ret) \
	Cx_Interface<Ix_##Module> (Var)(CLSID_##Module); \
	if((Var).IsNull()) \
	{ \
		lw_log_err(LWDP_MODULE_LOG, "Can't Get Module(%s) Pointer!(%s, %d)", #Module, __FILE__, __LINE__); \
		return (Ret); \
	}



#define GET_OBJECT(Module, Var) \
	Cx_Interface<Ix_##Module> (Var)(CLSID_##Module); \
	if((Var).IsNull()) \
	{ \
		lw_log_warning(LWDP_MODULE_LOG, "Can't Get Module(%s) Pointer!(%s, %d)", #Module, __FILE__, __LINE__); \
	}

#define PLATFORM_LOG(cat, level, ...) \
	if(level == 0)	\
		lw_log_emerg(cat, __VA_ARGS__); \
	else if(level == 1)	\
		lw_log_alert(cat, __VA_ARGS__); \
	else if(level == 2)	\
		lw_log_crit(cat, __VA_ARGS__); \
	else if(level == 3)	\
		lw_log_err(cat, __VA_ARGS__); \
	else if(level == 4)	\
		lw_log_warning(cat, __VA_ARGS__); \
	else if(level == 5)	\
		lw_log_notice(cat, __VA_ARGS__); \
	else if(level == 6)	\
		lw_log_info(cat, __VA_ARGS__); \
	else if(level == 7)	\
		lw_log_debug(cat, __VA_ARGS__); 





EXTERN_C_END;
LWDP_NAMESPACE_END;

#endif




 
