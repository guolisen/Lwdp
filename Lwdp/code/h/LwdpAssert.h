/*! \file LwDpAssert.h
 *  \brief Assert Tools
 *  \author Guolisen, LwDp
 *  \date   2011.07.14
 */

#ifndef LWDP_ASSERT_H_
#define LWDP_ASSERT_H_
 
LWDP_NAMESPACE_BEGIN;


	

#ifdef LWDP_DEBUG_MACRO

#define ASSERT_CHECK(a) \
    if(!(a)) \
	{ \
	    lw_log_crit(LWDP_MODULE_LOG, "!!!ASSERT_CHECK_RET Find Error!!!"); \
	    lw_log_crit(LWDP_MODULE_LOG, "%s File:%s Line: %d", #a, __FILE__, __LINE__); \
	}

#ifdef C99_VARIADIC_MACROS
//#if 1
#define ASSERT_CHECK_RET(cat, ret, a, ...) \
    if(!(a)) \
	{ \
	    lw_log_crit(cat, "!!!ASSERT_CHECK_RET Find(%s) Line(%d) File(%s) Error!!!", #a, __LINE__, __FILE__); \
	    lw_log_crit(cat, __VA_ARGS__); \
		return (ret); \
	}

#define ASSERT_CHECK_VOID(cat, a, ...) \
    if(!(a)) \
	{ \
	    lw_log_alert(cat, "!!!ASSERT_CHECK_VOID Find(%s) Line(%d) File(%s) Error!!!", #a, __LINE__, __FILE__); \
	    lw_log_alert(cat, __VA_ARGS__); \
		return; \
	}
	
#define ASSERT_CHECK_HALT(cat, a, ...) \
    if(!(a)) \
	{ \
	    lw_log_crit(cat, "!!!ASSERT_CHECK_HALT Find(%s) Line(%d) File(%s) Error!!!", #a, __LINE__, __FILE__); \
	    lw_log_crit(cat, __VA_ARGS__); \
    	while(1) \
        { \
            Api_TaskDelay(1000); \
        } \
	}
    
#else  //C99_VARIADIC_MACROS
#define ASSERT_CHECK_RET(cat, ret, a, fmt...) \
    if(!(a)) \
	{ \
	    lw_log_crit(cat, "!!!ASSERT_CHECK_RET Find(%s) Line(%d) File(%s) Error!!!", #a, __LINE__, __FILE__); \
	    lw_log_crit(cat, fmt); \
		return (ret); \
	}

#define ASSERT_CHECK_VOID(cat, a, fmt...) \
    if(!(a)) \
	{ \
	    lw_log_alert(cat, "!!!ASSERT_CHECK_VOID Find(%s) Line(%d) File(%s) Error!!!", #a, __LINE__, __FILE__); \
	    lw_log_alert(cat, fmt); \
		return; \
	}

#define ASSERT_CHECK_HALT(cat, a, fmt...) \
    if(!(a)) \
	{ \
	    lw_log_crit(cat, "!!!ASSERT_CHECK_HALT Find(%s) Line(%d) File(%s) Error!!!", #a, __LINE__, __FILE__); \
	    lw_log_crit(cat, fmt); \
    	while(1) \
        { \
            Api_TaskDelay(1000); \
        } \
	}
    
#endif //C99_VARIADIC_MACROS
#else
#define ASSERT_CHECK_RET(cat, ret, a, ...)  
#define ASSERT_CHECK_HALT(cat, a, ...)  
#define ASSERT_CHECK_VOID(cat, a, fmt...)
#define ASSERT_CHECK(a)
#endif //LWDP_DEBUG_MACRO


LWDP_NAMESPACE_END;

#endif




 

