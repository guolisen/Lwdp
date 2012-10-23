/*! \file LwLogMessage.h
 *  \brief Platform Log Module
 *  \author Guolisen, LwDp
 *  \date   2011.08.31
 */

#ifndef LW_LOG_MESSAGE_H_
#define LW_LOG_MESSAGE_H_

 
LWDP_NAMESPACE_BEGIN
EXTERN_C_BEGIN

#define LOG_LINE_STR_LEN  1024

#define LWDP_CAT_SHIFT	8
#define LWDP_CAT_MASK	((uint32_)(-(1 << LWDP_CAT_SHIFT)))



typedef enum _LWDP_LOG_CAT_NUM
{
#undef LOG_DB
#undef LOG_DB_LAST
#define LOG_DB(x, y, z)		    x##_LOG_NUM,
#define LOG_DB_LAST(x, y, z)	x##_LOG_NUM,
#include <lwlogCategories.h>
    LWDP_LOG_NUMCATS
} LWDP_LOG_CAT_NUM;

typedef enum _LWDP_LOG_CAT
{
#undef LOG_DB
#undef LOG_DB_LAST
#define LOG_DB(x, y, z)		x##_LOG = (x##_LOG_NUM << LWDP_CAT_SHIFT),
#define LOG_DB_LAST(x, y, z)	x##_LOG = (x##_LOG_NUM << LWDP_CAT_SHIFT)
#include <lwlogCategories.h>
} LWDP_LOG_CAT;

typedef struct lwdp_categoryInfo
{
    int32_ fd;
    uint32_ level;
} LWDP_CATEGORY_INFO;



/* Priority levels */


#define LWDP_LOG_DEBUG	    7 /* Messages containing information normally of use */
			  /* only when debugging */
#define LWDP_LOG_INFO	    6 /* Informational messages */
#define LWDP_LOG_NOTICE	    5 /* Non-error conditions, which may require special */
			  /* handling */
#define LWDP_LOG_WARNING	4 /* Warning messages */
#define LWDP_LOG_ERR		3 /* Errors */
#define LWDP_LOG_CRIT	    2 /* Critical conditions, such as hard device errors */
#define LWDP_LOG_ALERT	    1 /* A condition which requires immediate corrective */
			  /* action */
#define LWDP_LOG_EMERG	    0 /* Panic condition. The system is unstable. */


#define	LOG_UPTO(pri)	((1 << ((pri)+1)) - 1)	/* all priorities through pri */

/* Logging levels - convenience values for logging all levels
 * up to a specified level.
 */

#define DEBUG_LEVEL	    LOG_UPTO (LWDP_LOG_DEBUG)
#define INFO_LEVEL	    LOG_UPTO (LWDP_LOG_INFO)
#define NOTICE_LEVEL	LOG_UPTO (LWDP_LOG_NOTICE)
#define WARNING_LEVEL	LOG_UPTO (LWDP_LOG_WARNING)
#define ERR_LEVEL	    LOG_UPTO (LWDP_LOG_ERR)
#define ERROR_LEVEL	    ERR_LEVEL		/* deprecated */
#define CRIT_LEVEL	    LOG_UPTO (LWDP_LOG_CRIT)
#define ALERT_LEVEL	    LOG_UPTO (LWDP_LOG_ALERT)
#define EMERG_LEVEL	    LOG_UPTO (LWDP_LOG_EMERG)
#define DO_NOT_LOG	    0x00

/* We share priority levels (although not level names) with syslog() */

#define LOG_DEBUG_BIT		(1 << LWDP_LOG_DEBUG)
#define LOG_INFO_BIT		(1 << LWDP_LOG_INFO)
#define LOG_NOTICE_BIT		(1 << LWDP_LOG_NOTICE)
#define LOG_WARNING_BIT		(1 << LWDP_LOG_WARNING)
#define LOG_ERR_BIT		    (1 << LWDP_LOG_ERR)
#define LOG_CRIT_BIT		(1 << LWDP_LOG_CRIT)
#define LOG_ALERT_BIT		(1 << LWDP_LOG_ALERT)
#define LOG_EMERG_BIT		(1 << LWDP_LOG_EMERG)



#define LOG_PLATFORM_BIT	0x100
#define LOG_DECOR	        0x80 /* log priority level and facility */
#define LOG_LOC		        0x40 /* log file name or function and line number */

#ifdef ENABLE_LOG_TIME
#define LOG_TIME	        0x20 /* log a timestamp */
#endif
#define LOG_ERRNO	        0x10 /* log errno */



#ifndef _LOG_DEC_DFLT
#define _LOG_DEC_DFLT	    ((LOG_DECOR|LOG_LOC) << 8)
#endif

#ifndef _LOG_LVL_DFLT
#define _LOG_LVL_DFLT	    DEBUG_LEVEL
#endif

#ifndef LWDP_LOG_LOCATION
#define LWDP_LOG_LOCATION	LWDP_LOG_LOC_FUNC
#endif
/*
 * In the priority/category argument to _lw_applLog(),
 *
 *  Bits 0 - 2 are the priority
 *  Bits 3 - 7 are decoration bits.
 *  Bits 8 - 31 identify the category (Facility)
 *
 *  Bit 0 is the least significant bit here.
 */

#define PRI_CAT(pri, cat) ((cat) | (pri))

#define LOG_CAT(x) ((uint32_)(x) >> LWDP_CAT_SHIFT)

#define LOG_NAME_MAX_STR 255


/* Define LOG_ENABLE_MASK as 0 to disable at compile time
 * all application logging.  This might be done if a target platform
 * has no available console or other file to which it can log.
 * Define LOG_ENABLE_MASK as a collection of the LOG_*_BIT values above
 * to enable particular logging levels.
 */

#if !defined (LOG_ENABLE_MASK)

#if FALSE  /* change to TRUE if a logging destination is not available */

#define LOG_ENABLE_MASK (0)

#elif TRUE  /* change to TRUE to do full logging */

#define LOG_ENABLE_MASK (DEBUG_LEVEL | LOG_PLATFORM_BIT)

#else

#define LOG_ENABLE_MASK (LOG_NOTICE_BIT | \
						 LOG_WARNING_BIT | \
						 LOG_ERR_BIT | \
						 LOG_CRIT_BIT | \
						 LOG_ALERT_BIT | \
						 LOG_EMERG_BIT | \
						 LOG_PLATFORM_BIT)


#endif
#endif /* !defined (LOG_ENABLE_MASK) */


/* function declarations */

LWRESULT Api_LoggerFdSet (int32_ category, int32_ fd);
LWRESULT Api_LogLevelSet (int32_ category, uint32_ mask);
LWRESULT Api_LogLevelGet (int32_ category, uint32_* log_level);
uint32_  Api_LogDecorationChange (int32_ category, uint32_ disableMask, uint32_ enableMask);
void     Api_LoggerInfoShow ();
LWRESULT _lw_applLog(int32_ priCat, const char_ * file, int32_ line, const tchar_* fmt, ...);


/* XXX verify exact version numbers for _HAVE__FUNCTION__ and _HAVE__func__ */

#if defined (__GNUC__) || (defined (__DCC__) && __VERSION_NUMBER__ >= 5200)
#define LWDP_HAVE__FUNCTION__
#endif

#if (GCC_VERSION >= 30200)
#define LWDP_HAVE__func__
#endif

#define LWDP_LOG_LOC_FILE	0      /* The default */
#define LWDP_LOG_LOC_FUNC	1
#define LWDP_LOG_LOC_NONE	2

#if LWDP_LOG_LOCATION == LWDP_LOG_LOC_NONE
#define _LWDP_LOG_LOCATION	NULL, 0
#define _LWDP_LOC_UNIT	NULL
#elif (LWDP_LOG_LOCATION == LWDP_LOG_LOC_FUNC) && defined (LWDP_HAVE__func__)
#define	_LWDP_LOG_LOCATION	__func__, __LINE__
#define _LWDP_LOC_UNIT	__func__
#elif (LWDP_LOG_LOCATION == LWDP_LOG_LOC_FUNC) && defined (LWDP_HAVE__FUNCTION__)
#define	_LWDP_LOG_LOCATION	__FUNCTION__, __LINE__
#define _LWDP_LOC_UNIT	__FUNCTION__
#else
#define	_LWDP_LOG_LOCATION	__FILE__, __LINE__
#define _LWDP_LOC_UNIT	__FILE__
#endif


#ifndef LOG_ARGS
#define LOG_ARGS	1
#endif

#ifdef C99_VARIADIC_MACROS

#if (LOG_ENABLE_MASK & LOG_DEBUG_BIT)
#if LOG_ARGS
#define lw_log_debug(cat, ...)					 \
    do  {							 \
	_lw_applLog (PRI_CAT (LWDP_LOG_DEBUG, cat), _LWDP_LOG_LOCATION,     	 \
	      __VA_ARGS__);					 \
	} while (FALSE)
#else
#define lw_log_debug(cat, ...)					\
    do  {							\
	_lw_applLog (PRI_CAT (LWDP_LOG_DEBUG, cat), _LWDP_LOG_LOCATION, "");	\
	} while (FALSE)
#endif /* LOG_ARGS */
#else
#define lw_log_debug(cat, ...) do {} while (FALSE)
#endif

#if (LOG_ENABLE_MASK & LOG_INFO_BIT)
#if LOG_ARGS
#define lw_log_info(cat, ...)					\
    do  {							\
	    _lw_applLog (PRI_CAT (LWDP_LOG_INFO, cat), _LWDP_LOG_LOCATION,     	\
	                 __VA_ARGS__);					\
	} while (FALSE)
#else
#define lw_log_info(cat, ...)					\
    do  {							\
	_lw_applLog (PRI_CAT (LWDP_LOG_INFO, cat), _LWDP_LOG_LOCATION, "");	\
	} while (FALSE)
#endif /* LOG_ARGS */
#else
#define lw_log_info(cat, ...) do {} while (FALSE)
#endif

#if (LOG_ENABLE_MASK & LOG_NOTICE_BIT)
#if LOG_ARGS
#define lw_log_notice(cat, ...)					  \
    do  {							  \
	_lw_applLog (PRI_CAT (LWDP_LOG_NOTICE, cat), _LWDP_LOG_LOCATION,       \
	      __VA_ARGS__);					  \
	} while (FALSE)
#else
#define lw_log_notice(cat, ...)					\
    do  {							\
	_lw_applLog (PRI_CAT (LWDP_LOG_NOTICE, cat), _LWDP_LOG_LOCATION, "");\
	} while (FALSE)
#endif /* LOG_ARGS */
#else
#define lw_log_notice(cat, ...) do {} while (FALSE)
#endif

#if (LOG_ENABLE_MASK & LOG_WARNING_BIT)
#if LOG_ARGS
#define lw_log_warning(cat, ...)					\
    do  {							\
	_lw_applLog (PRI_CAT (LWDP_LOG_WARNING, cat), _LWDP_LOG_LOCATION,	\
	      __VA_ARGS__);					\
	} while (FALSE)
#else
#define lw_log_warning(cat, ...)						\
    do  {								\
	_lw_applLog (PRI_CAT (LWDP_LOG_WARNING, cat), _LWDP_LOG_LOCATION, "");	\
	} while (FALSE)
#endif /* LOG_ARGS */
#else
#define lw_log_warning(cat, ...) do {} while (FALSE)
#endif

#if (LOG_ENABLE_MASK & LOG_ERR_BIT)
#if LOG_ARGS
#define lw_log_err(cat, ...)					 \
    do  {							 \
	_lw_applLog (PRI_CAT (LWDP_LOG_ERR, cat), _LWDP_LOG_LOCATION,     	 \
	      __VA_ARGS__);					 \
	} while (FALSE)
#else
#define lw_log_err(cat, ...)					\
    do  {							\
	_lw_applLog (PRI_CAT (LWDP_LOG_ERR, cat), _LWDP_LOG_LOCATION, "");	\
	} while (FALSE)
#endif /* LOG_ARGS */
#else
#define lw_log_err(cat, ...) do {} while (FALSE)
#endif

#if (LOG_ENABLE_MASK & LOG_CRIT_BIT)
#if LOG_ARGS
#define lw_log_crit(cat, ...)					\
    do  {							\
	_lw_applLog (PRI_CAT (LWDP_LOG_CRIT, cat), _LWDP_LOG_LOCATION,     	\
	      __VA_ARGS__);					\
	} while (FALSE)
#else
#define lw_log_crit(cat, ...)					\
    do  {							\
	_lw_applLog (PRI_CAT (LWDP_LOG_CRIT, cat), _LWDP_LOG_LOCATION, "");	\
	} while (FALSE)
#endif /* LOG_ARGS */
#else
#define lw_log_crit(cat, ...) do {} while (FALSE)
#endif

#if (LOG_ENABLE_MASK & LOG_ALERT_BIT)
#if LOG_ARGS
#define lw_log_alert(cat, ...)					\
    do  {							\
	_lw_applLog (PRI_CAT (LWDP_LOG_ALERT, cat), _LWDP_LOG_LOCATION,     	\
	      __VA_ARGS__);					\
	} while (FALSE)
#else
#define lw_log_alert(cat, ...)					\
    do  {							\
	_lw_applLog (PRI_CAT (LWDP_LOG_ALERT, cat), _LWDP_LOG_LOCATION, "");	\
	} while (FALSE)
#endif /* LOG_ARGS */
#else
#define lw_log_alert(cat, ...) do {} while (FALSE)
#endif

#if (LOG_ENABLE_MASK & LOG_EMERG_BIT)
#if LOG_ARGS
#define lw_log_emerg(cat, ...)					 \
    do  {							 \
	_lw_applLog (PRI_CAT (LWDP_LOG_EMERG, cat), _LWDP_LOG_LOCATION,     	 \
	      __VA_ARGS__);					 \
	} while (FALSE)
#else
#define lw_log_emerg(cat, ...)					\
    do  {							\
	_lw_applLog (PRI_CAT (LWDP_LOG_EMERG, cat), _LWDP_LOG_LOCATION, "");	\
	} while (FALSE)
#endif /* LOG_ARGS */
#else
#define lw_log_emerg(cat, ...) do {} while (FALSE)
#endif

#else  /* !defined (C99_VARIADIC_MACROS) -> assume old GNU variadic syntax */

#if (LOG_ENABLE_MASK & LOG_DEBUG_BIT)
#if LOG_ARGS
#define lw_log_debug(cat, fmt...)						\
    do  {								\
	_lw_applLog (PRI_CAT (LWDP_LOG_DEBUG, cat), _LWDP_LOG_LOCATION, fmt);	\
	} while (FALSE)
#else
#define lw_log_debug(cat, fmt...)						\
    do  {								\
	_lw_applLog (PRI_CAT (LWDP_LOG_DEBUG, cat), _LWDP_LOG_LOCATION, "");		\
	} while (FALSE)
#endif /* LOG_ARGS */
#else
#define lw_log_debug(cat, fmt...) do {} while (FALSE)
#endif

#if (LOG_ENABLE_MASK & LOG_INFO_BIT)
#if LOG_ARGS
#define lw_log_info(cat, fmt...)						\
    do  {								\
	_lw_applLog (PRI_CAT (LWDP_LOG_INFO, cat), _LWDP_LOG_LOCATION, fmt);		\
	} while (FALSE)
#else
#define lw_log_info(cat, fmt...)						\
    do  {								\
	_lw_applLog (PRI_CAT (LWDP_LOG_INFO, cat), _LWDP_LOG_LOCATION, "");		\
	} while (FALSE)
#endif /* LOG_ARGS */
#else
#define lw_log_info(cat, fmt...) do {} while (FALSE)
#endif

#if (LOG_ENABLE_MASK & LOG_NOTICE_BIT)
#if LOG_ARGS
#define lw_log_notice(cat, fmt...)						\
    do  {								\
	_lw_applLog (PRI_CAT (LWDP_LOG_NOTICE, cat), _LWDP_LOG_LOCATION, fmt);	\
	} while (FALSE)
#else
#define lw_log_notice(cat, fmt...)						\
    do  {								\
	_lw_applLog (PRI_CAT (LWDP_LOG_NOTICE, cat), _LWDP_LOG_LOCATION, "");	\
	} while (FALSE)
#endif /* LOG_ARGS */
#else
#define lw_log_notice(cat, fmt...) do {} while (FALSE)
#endif

#if (LOG_ENABLE_MASK & LOG_WARNING_BIT)
#if LOG_ARGS
#define lw_log_warning(cat, fmt...)					\
    do  {								\
	_lw_applLog (PRI_CAT (LWDP_LOG_WARNING, cat), _LWDP_LOG_LOCATION, fmt);	\
	} while (FALSE)
#else
#define lw_log_warning(cat, fmt...)					\
    do  {								\
	_lw_applLog (PRI_CAT (LWDP_LOG_WARNING, cat), _LWDP_LOG_LOCATION, "");	\
	} while (FALSE)
#endif /* LOG_ARGS */
#else
#define lw_log_warning(cat, fmt...) do {} while (FALSE)
#endif

#if (LOG_ENABLE_MASK & LOG_ERR_BIT)
#if LOG_ARGS
#define lw_log_err(cat, fmt...)						\
    do  {								\
	_lw_applLog (PRI_CAT (LWDP_LOG_ERR, cat), _LWDP_LOG_LOCATION, fmt);		\
	} while (FALSE)
#else
#define lw_log_err(cat, fmt...)						\
    do  {								\
	_lw_applLog (PRI_CAT (LWDP_LOG_ERR, cat), _LWDP_LOG_LOCATION, "");		\
	} while (FALSE)
#endif /* LOG_ARGS */
#else
#define lw_log_err(cat, fmt...) do {} while (FALSE)
#endif

#if (LOG_ENABLE_MASK & LOG_CRIT_BIT)
#if LOG_ARGS
#define lw_log_crit(cat, fmt...)						\
    do  {								\
	_lw_applLog (PRI_CAT (LWDP_LOG_CRIT, cat), _LWDP_LOG_LOCATION, fmt);		\
	} while (FALSE)
#else
#define lw_log_crit(cat, fmt...)						\
    do  {								\
	_lw_applLog (PRI_CAT (LWDP_LOG_CRIT, cat), _LWDP_LOG_LOCATION, "");		\
	} while (FALSE)
#endif /* LOG_ARGS */
#else
#define lw_log_crit(cat, fmt...) do {} while (FALSE)
#endif

#if (LOG_ENABLE_MASK & LOG_ALERT_BIT)
#if LOG_ARGS
#define lw_log_alert(cat, fmt...)						\
    do  {								\
	_lw_applLog (PRI_CAT (LWDP_LOG_ALERT, cat), _LWDP_LOG_LOCATION, fmt);	\
	} while (FALSE)
#else
#define lw_log_alert(cat, fmt...)						\
    do  {								\
	_lw_applLog (PRI_CAT (LWDP_LOG_ALERT, cat), _LWDP_LOG_LOCATION, "");		\
	} while (FALSE)
#endif /* LOG_ARGS */
#else
#define lw_log_alert(cat, fmt...) do {} while (FALSE)
#endif

#if (LOG_ENABLE_MASK & LOG_EMERG_BIT)
#if LOG_ARGS
#define lw_log_emerg(cat, fmt...)						\
    do  {								\
	_lw_applLog (PRI_CAT (LWDP_LOG_EMERG, cat), _LWDP_LOG_LOCATION, fmt);	\
	} while (FALSE)
#else
#define lw_log_emerg(cat, fmt...)						\
    do  {								\
	_lw_applLog (PRI_CAT (LWDP_LOG_EMERG, cat), _LWDP_LOG_LOCATION, "");		\
	} while (FALSE)
#endif /* LOG_ARGS */
#else
#define lw_log_emerg(cat, fmt...) do {} while (FALSE)
#endif

#endif /* !defined (C99_VARIADIC_MACROS) */


EXTERN_C_END
LWDP_NAMESPACE_END
#endif




 

