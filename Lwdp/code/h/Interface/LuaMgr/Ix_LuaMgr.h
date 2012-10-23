#ifndef LUAMGR_INTERFACE_H
#define LUAMGR_INTERFACE_H

#include <Interface/Ix_Object.h>
#include "Id_LuaMgr.h"

LWDP_NAMESPACE_BEGIN;


INTERFACE_BEGIN(LuaMgr)
	virtual LWRESULT Init() = 0;

	virtual LWRESULT DoFile(const tstring& file_name) = 0;
	virtual LWRESULT LoadFile(const tstring& file_name) = 0;
	virtual LWRESULT LoadLuaLibrary(const tstring& file_name) = 0;

	virtual LWRESULT DoBuffer(const char* buf) = 0;
	virtual LWRESULT LoadBuffer(const char* buf) = 0;
	virtual LWRESULT LoadLibraryBuffer(const char* buf) = 0;


	virtual LWRESULT RegisteFuction() = 0;
	virtual LWRESULT Call() = 0;

	virtual LWRESULT ResetStack() = 0;

	
INTERFACE_END();


LWDP_NAMESPACE_END;

#endif // LOGMGR_INTERFACE_H
