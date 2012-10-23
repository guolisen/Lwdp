/*! \file LwLogErrno.h
 *  \brief Log Module Errno Define
 *  \author Guolisen, LwDp
 *  \date   2011.08.12
 */


ERRNO_DB (LOGMSG_LEVEL_LOW,            (M_logMsg | 1), "The Message LEVEL is Low")
ERRNO_DB (LOGMSG_OUTPUT_FD_ERROR,      (M_logMsg | 2), "The Output Port is ERROR or NONE")
ERRNO_DB (LOGMSG_CATEGORY_ERROR,       (M_logMsg | 3), "The CATEGORY is Larger then Max Number Or is Not CATEGORY")
ERRNO_DB (LOGMSG_LOGLEVEL_ERROR,       (M_logMsg | 3), "Log Level is 8bit, Or The Leve is Error")


