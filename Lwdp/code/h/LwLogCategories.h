/*! \file LwLogMessage.h
 *  \brief Platform Log Module
 *  \author Guolisen, LwDp
 *  \date   2011.08.31
 */


LOG_DB (LWDP, 		 STD_OUT,  (_LOG_DEC_DFLT | _LOG_LVL_DFLT))
LOG_DB (LWDP_MODULE, STD_OUT,  (_LOG_DEC_DFLT | _LOG_LVL_DFLT))
LOG_DB (LWDP_API,    STD_OUT,  (_LOG_DEC_DFLT | _LOG_LVL_DFLT))
LOG_DB (LWDP_FW,     STD_OUT,  (_LOG_DEC_DFLT | _LOG_LVL_DFLT))
LOG_DB (LWDP_LOG,    STD_OUT,  (_LOG_DEC_DFLT | _LOG_LVL_DFLT))
LOG_DB (LWDP_LUA,    STD_OUT,  (_LOG_DEC_DFLT | _LOG_LVL_DFLT))


/* must be last */
LOG_DB_LAST (LWDP_GENERAL,	STD_OUT,  (_LOG_DEC_DFLT | _LOG_LVL_DFLT))

