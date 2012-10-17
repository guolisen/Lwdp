/*! \file Cx_luaMgr.h
 *  \brief LuaMgr Module Impl
 *  \author Guolisen, LwDp
 *  \date   2012.10.17
 */
 
#ifndef CX_LUA_MANAGER_H
#define CX_LUA_MANAGER_H

#include <boost/shared_ptr.hpp>
#include <list>
#include <Interface/LuaMgr/Ix_LuaMgr.h>
//#include <Interface/ScriptMgr/Ix_ScriptMgr.h>
#include "LuaTagDef.h"

#include <iostream>

extern "C" 
{
	#include "../LwApiLib/ComLib/lua/lua.h"
	#include "../LwApiLib/ComLib/lua/lualib.h"
	#include "../LwApiLib/ComLib/lua/lauxlib.h"
};

LWDP_NAMESPACE_BEGIN;

class Cx_LuaMgr
    : public Ix_LuaMgr
{
    X3BEGIN_CLASS_DECLARE(Cx_LuaMgr)
        X3DEFINE_INTERFACE_ENTRY(Ix_LuaMgr)
    X3END_CLASS_DECLARE()
protected:
	Cx_LuaMgr();
	virtual ~Cx_LuaMgr();

protected:
	LWRESULT Init();

	LWRESULT DoFile(const tstring& file_name);
	LWRESULT LoadFile(const tstring& file_name);
	LWRESULT LoadLuaLibrary(const tstring& file_name);

	LWRESULT DoBuffer(const char* buf){return LWDP_OK;};
	LWRESULT LoadBuffer(const char* buf){return LWDP_OK;};
	LWRESULT LoadLibraryBuffer(const char* buf){return LWDP_OK;};


	LWRESULT RegisteFuction(){return LWDP_OK;};
	LWRESULT Call(){return LWDP_OK;};

	LWRESULT ResetStack(){return LWDP_OK;};
	
protected:


protected:
	lua_State* mL;
	int        mStackLibPos;

};


LWDP_NAMESPACE_END;

#endif
