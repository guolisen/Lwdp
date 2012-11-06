/*! \file   Lua_LogMgr.h
 *  \brief  LogMgr Lua Define
 *  \author Guolisen, Lwdp PluginFramework
 *  \date   2012.02.03
 */
#ifndef LWDP_LUA_LOGMGR_H__
#define LWDP_LUA_LOGMGR_H__
#include <LwDp.h>
#include <Interface/Ix_Object.h>
#include <Interface/ConfigMgr/Ix_XmlMgr.h>
#include <Interface/ConfigMgr/Ix_ConfigMgr.h>

LWDP_NAMESPACE_BEGIN;

inline Script_Ptr* CreateScriptPtr()
{
	return new Script_Ptr();
}

inline char* GetScriptPtr(Script_Ptr* ptr)
{
	return (char*)ptr->get();
}

inline char* GetXmlResults(tstring* ptr)
{
	return (char*)ptr->c_str();
}

inline unsigned int LuaConfigMgrGetModulePropEntry(char* module_or_moid, char* property_name, XmlResults& propertys)
{
	GET_OBJECT(ConfigMgr, iConfigMgr);
	return iConfigMgr->GetModulePropEntry(module_or_moid, property_name, propertys); 
}

inline unsigned int LuaConfigMgrGetModuleTable(char* module_or_moid, char* table_name, XmlResultsSet& property_table)					 
{
	GET_OBJECT(ConfigMgr, iConfigMgr);
	return iConfigMgr->GetModuleTable(module_or_moid, table_name, property_table); 
	
}

inline unsigned int LuaConfigMgrGetScript(char* module_or_moid, char* script_name, Script_Ptr& script_buf)
{
	GET_OBJECT(ConfigMgr, iConfigMgr);
	return iConfigMgr->GetModuleScript(module_or_moid, script_name, script_buf); 
}
	
LWDP_NAMESPACE_END;

#endif // LWDP_CONFIG_TAG_DEFINE_H_





