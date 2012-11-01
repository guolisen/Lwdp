/*! \file   Lua_LogMgr.h
 *  \brief  LogMgr Lua Define
 *  \author Guolisen, Lwdp PluginFramework
 *  \date   2012.02.03
 */
#ifndef LWDP_LUA_LOGMGR_H__
#define LWDP_LUA_LOGMGR_H__
#include <LwDp.h>
#include <Interface/Ix_Object.h>
#include <Interface/LogMgr/Ix_LogMgr.h>


LWDP_NAMESPACE_BEGIN;

inline void LuaLogPrint(const tchar_* module_name, 
						    int level, 
						    const tchar_* file_name,
						    long line,
						    const tchar_* stringFormat)
{
	GET_OBJECT(LogMgr, iLogMgr);
	iLogMgr->LogPrint(module_name, (LWDP_LOG_MGR::LwdpLogLevel)level, file_name, line, stringFormat); 
}
	
LWDP_NAMESPACE_END;

#endif // LWDP_CONFIG_TAG_DEFINE_H_





