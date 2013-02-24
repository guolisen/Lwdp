#ifndef LUAMGR_INTERFACE_H
#define LUAMGR_INTERFACE_H

#include <LwApiLib/ComLib/Lua/LuaPlus.h>
#include <Interface/Ix_Object.h>
#include "Id_LuaMgr.h"


using namespace LuaPlus;

LWDP_NAMESPACE_BEGIN;

typedef void* LUACONTEXT; 

INTERFACE_BEGIN(LuaMgr)
	virtual LWRESULT Init() = 0;

	virtual LUACONTEXT CreateConext() = 0;
	virtual LWRESULT CloseContext(LUACONTEXT context) = 0;

	virtual LWRESULT DoFile(LUACONTEXT context, const tstring& file_name) = 0;
	virtual LWRESULT LoadFile(LUACONTEXT context, const tstring& file_name) = 0;
	virtual LWRESULT LoadLuaLibrary(LUACONTEXT context, const tstring& file_name) = 0;

	virtual LWRESULT DoBuffer(LUACONTEXT context, const char* buf) = 0;
	virtual LWRESULT LoadBuffer(LUACONTEXT context, const char* buf) = 0;
	virtual LWRESULT LoadLibraryBuffer(LUACONTEXT context, const char* buf) = 0;

	virtual LWRESULT RegisteFuction(void* func) = 0;
	virtual LWRESULT ResetStack() = 0;
	virtual LWRESULT LoadAllLib(LUACONTEXT context) = 0;

	
	virtual LWRESULT Call(LUACONTEXT context, std::string func_name) = 0;


INTERFACE_END();


LWDP_NAMESPACE_END;

#endif // LOGMGR_INTERFACE_H
