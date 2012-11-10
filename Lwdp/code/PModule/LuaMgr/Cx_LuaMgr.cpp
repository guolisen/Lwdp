/*! \file Cx_luaMgr.cpp
 *  \brief LuaMgr Module Impl
 *  \author Guolisen, LwDp
 *  \date   2012.10.17
 */
 
#include <LwDp.h>
#include <PluginInc.h>
#include <iostream>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>

#include "LuaTagDef.h"
#include "Cx_LuaMgr.h"




LWDP_NAMESPACE_BEGIN;

LIB_REGISTE_FUNC_LIST Cx_LuaMgr::mRegisteFuncList;

Cx_LuaMgr::Cx_LuaMgr():mL(NULL)
{
}

Cx_LuaMgr::~Cx_LuaMgr()
{
	Destory();
}

LWRESULT Cx_LuaMgr::Init()
{
	ResetStack();
	lw_log_info(LWDP_LUA_LOG, __T("Cx_LuaMgr::Init OK!"));

	return LWDP_OK;
}

LWRESULT Cx_LuaMgr::Destory()
{
	if(mL)
	{
		//lua_settop(mL, 0);
		lua_close(mL);//¹Ø±Õ 
		mL = NULL;
	}

	return LWDP_OK;
}
LWRESULT Cx_LuaMgr::ResetStack()
{
	Destory();
	mL = lua_open();

	lua_gc(mL, LUA_GCSTOP, 0);  /* stop collector during initialization */  
	luaL_openlibs(mL);  /* open libraries */  
	lua_gc(mL, LUA_GCRESTART, 0);
	mStackLibPos = lua_gettop(mL);
	return LWDP_OK;
}

int on_error(lua_State *L)
{
	PLATFORM_LOG(LWDP_LUA_LOG, LWDP_LOG_ERR, "%s", lua_tostring(L, -1));	

	return 0;	
}
LWRESULT Cx_LuaMgr::LoadLuaLibrary(const tstring& file_name)
{
	if(file_name.empty())
		return LWDP_PARAMETER_ERROR;

	lua_pushcclosure(mL, (lua_CFunction)on_error, 0);
	int errfunc = lua_gettop(mL);
	
	if(luaL_loadfile(mL, file_name.c_str()) == 0)
	{
		if(lua_pcall(mL, 0, 0, errfunc) != 0)
		{
			lua_pop(mL, 1);
			return LWDP_LOAD_SCRIPT_RUN_ERROR;
		}
	}
	else
	{
		PLATFORM_LOG(LWDP_LUA_LOG, LWDP_LOG_ERR, "%s", lua_tostring(mL, -1));	
		lua_pop(mL, 1);
		return LWDP_LOAD_SCRIPT_ERROR;
	}

	mStackLibPos = lua_gettop(mL);
		
	return LWDP_OK;
}



LWRESULT Cx_LuaMgr::DoFile(const tstring& file_name)
{
	if(file_name.empty())
		return LWDP_PARAMETER_ERROR;

	lua_pushcclosure(mL, (lua_CFunction)on_error, 0);
	int errfunc = lua_gettop(mL);
	
	if(luaL_loadfile(mL, file_name.c_str()) == 0)
	{
		if(lua_pcall(mL, 0, 0, errfunc) != 0)
		{
			lua_pop(mL, 1);
			return LWDP_LOAD_SCRIPT_RUN_ERROR;
		}
	}
	else
	{
		PLATFORM_LOG(LWDP_LUA_LOG, LWDP_LOG_ERR, "%s", lua_tostring(mL, -1));	
		lua_pop(mL, 1);
		return LWDP_LOAD_SCRIPT_ERROR;
	}
	
	return LWDP_OK;
}

LWRESULT Cx_LuaMgr::LoadFile(const tstring& file_name)
{
	if(file_name.empty())
		return LWDP_PARAMETER_ERROR;
	
	if(luaL_loadfile(mL, file_name.c_str()))
	{
		PLATFORM_LOG(LWDP_LUA_LOG, LWDP_LOG_ERR, "%s", lua_tostring(mL, -1));	
		lua_pop(mL, 1);
		return LWDP_LOAD_SCRIPT_ERROR;
	}
	
	return LWDP_OK;
}

LWRESULT Cx_LuaMgr::RegisteFuction(void* func)
{
	if(!func)
		return LWDP_PARAMETER_ERROR;

	TOLUA_OPEN openfunc = (TOLUA_OPEN)func;
	LIB_REGISTE_FUNC_LIST::iterator iter;
	FOREACH_STL(iter, mRegisteFuncList)
	{
		if(*iter == openfunc)
			return LWDP_OK;
	}

	mRegisteFuncList.push_back(openfunc);
	return LWDP_OK;
}

LWRESULT Cx_LuaMgr::LoadAllLib()
{
	XPropertyTable libraryTable;
	GET_OBJECT_RET(ConfigMgr, iConfigMgr, LWDP_GET_OBJECT_ERROR);
	RINOK(iConfigMgr->GetModuleTable(LW_LUAMGR_MODULE_NAME, LW_LUAMGR_LIBRARY_TABLE_NAME, libraryTable));

	for(uint32_ i=0; libraryTable[i].ThereIs; ++i)
	{
		tstring libraryDir = libraryTable[i].propertyText;
		RINOK(LoadLuaLibrary(libraryDir));
	}

	LIB_REGISTE_FUNC_LIST::iterator iter;
	FOREACH_STL(iter, mRegisteFuncList)
	{
		TOLUA_OPEN openfunc = (TOLUA_OPEN)*iter;
		openfunc(mL);
	}

}

LWDP_NAMESPACE_END;

