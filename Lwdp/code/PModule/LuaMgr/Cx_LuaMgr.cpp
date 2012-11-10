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
#include <Interface/LuaMgr/Ix_LuaMgr.h>

#include "LuaTagDef.h"
#include "Cx_LuaMgr.h"




LWDP_NAMESPACE_BEGIN;

LIB_REGISTE_FUNC_LIST Cx_LuaMgr::mRegisteFuncList;

Cx_LuaMgr::Cx_LuaMgr():mState(NULL)
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
	if(mState)
	{
		delete mState;
		mState = NULL;
	}

	return LWDP_OK;
}
LWRESULT Cx_LuaMgr::ResetStack()
{
	Destory();
	mState = new LuaStateOwner(true);
	
	return LWDP_OK;
}

LWRESULT Cx_LuaMgr::LoadLuaLibrary(const tstring& file_name)
{
	if(file_name.empty())
		return LWDP_PARAMETER_ERROR;

	int iret = (*mState)->DoFile(file_name.c_str());
	if(iret)
	{
		LuaStackObject obj = (*mState)->Stack(-1);
		PLATFORM_LOG(LWDP_LUA_LOG, LWDP_LOG_ERR, "%s", obj.GetString());
		(*mState)->Pop();
		return LWDP_LOAD_SCRIPT_ERROR;
	}
		
	return LWDP_OK;
}



LWRESULT Cx_LuaMgr::DoFile(const tstring& file_name)
{
	if(file_name.empty())
		return LWDP_PARAMETER_ERROR;

	int iret = (*mState)->DoFile(file_name.c_str());
	if(iret)
	{
		LuaStackObject obj = (*mState)->Stack(-1);
		PLATFORM_LOG(LWDP_LUA_LOG, LWDP_LOG_ERR, "%s", obj.GetString());
		(*mState)->Pop();
		return LWDP_LOAD_SCRIPT_ERROR;
	}

	return LWDP_OK;
}

LWRESULT Cx_LuaMgr::LoadFile(const tstring& file_name)
{
	if(file_name.empty())
		return LWDP_PARAMETER_ERROR;
#if 0	
	if(luaL_loadfile(mL, file_name.c_str()))
	{
		PLATFORM_LOG(LWDP_LUA_LOG, LWDP_LOG_ERR, "%s", lua_tostring(mL, -1));	
		lua_pop(mL, 1);
		return LWDP_LOAD_SCRIPT_ERROR;
	}
#endif
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
	LuaStackObject obj = (*mState)->StackTop();
	lua_State* tmpStack = obj.GetCState();
	FOREACH_STL(iter, mRegisteFuncList)
	{
		TOLUA_OPEN openfunc = (TOLUA_OPEN)*iter;
		openfunc(tmpStack);
	}

	return LWDP_OK;
}

LWDP_NAMESPACE_END;

