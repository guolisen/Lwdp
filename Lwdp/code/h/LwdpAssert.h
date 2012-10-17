/*! \file LwDpAssert.h
 *  \brief Assert Tools
 *  \author Guolisen, LwDp
 *  \date   2011.07.14
 */

#ifndef LWDP_ASSERT_H_
#define LWDP_ASSERT_H_

LWDP_NAMESPACE_BEGIN;


#ifdef LWDP_DEBUG_MACRO
#ifdef C99_VARIADIC_MACROS
#define ASSERT_CHECK_RET(cat, ret, a, ...) \
    if(!(a)) \
	{ \
	    lw_log_crit(cat, "!!!ASSERT_CHECK_RET Find Error!!!"); \
	    lw_log_crit(cat, __VA_ARGS__); \
		return (ret); \
	}

#define ASSERT_CHECK_VOID(cat, a, ...) \
    if(!(a)) \
	{ \
		lw_log_alert(cat, "!!!ASSERT_CHECK_HALT Find Error!!!"); \
	    lw_log_alert(cat, __VA_ARGS__); \
		return; \
	}
	
#define ASSERT_CHECK_HALT(cat, a, ...) \
    if(!(a)) \
	{ \
		lw_log_alert(cat, "!!!ASSERT_CHECK_HALT Find Error!!!"); \
	    lw_log_alert(cat, __VA_ARGS__); \
    	while(1) \
        { \
            Api_TaskDelay(1000); \
        } \
	}
    
#else  //C99_VARIADIC_MACROS
#define ASSERT_CHECK_RET(cat, ret, a, fmt...) \
    if(!(a)) \
	{ \
	    lw_log_crit(cat, "!!!ASSERT_CHECK_RET Find Error!!!"); \
	    lw_log_crit(cat, fmt); \
		return (ret); \
	}

#define ASSERT_CHECK_VOID(cat, a, fmt...) \
    if(!(a)) \
	{ \
		lw_log_alert(cat, "!!!ASSERT_CHECK_HALT Find Error!!!"); \
	    lw_log_alert(cat, fmt); \
		return; \
	}

#define ASSERT_CHECK_HALT(cat, a, fmt...) \
    if(!(a)) \
	{ \
		lw_log_alert(cat, "!!!ASSERT_CHECK_HALT Find Error!!!"); \
	    lw_log_alert(cat, fmt); \
    	while(1) \
        { \
            Api_TaskDelay(1000); \
        } \
	}
    
#endif //C99_VARIADIC_MACROS
#else
#define ASSERT_CHECK_RET(cat, ret, a, ...)  
#define ASSERT_CHECK_HALT(cat, a, ...)  
#endif //LWDP_DEBUG_MACRO


LWDP_NAMESPACE_END;

#endif




 

