/*
** Lua binding: LogMgr
** Generated automatically by tolua++-1.0.92 on 11/01/12 21:22:20.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_LogMgr_open (lua_State* tolua_S);

#include <LwDp.h>
#include <Interface/LogMgr/Ix_LogMgr.h>
#include "Lua_Interface_LogMg.h"
using namespace NLwdp;

enum LogLevel
{
	DEBUG,	/* Messages containing information normally of use */
		/* only when debugging */
	INFO,	/* Informational messages */
	NOTICE,	/* Non-error conditions, which may require special */
		/* handling */
	WARNING,	/* Warning messages */
	ERR,	/* Errors */
	CRIT,	/* Critical conditions, such as hard device errors */
	ALERT,	/* A condition which requires immediate corrective */
		/* action */
	EMERG	/* Panic condition. The system is unstable. */
};

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
}

/* function: LuaLogPrint */
#ifndef TOLUA_DISABLE_tolua_LogMgr_LogMgr_LuaLogPrint00
static int tolua_LogMgr_LogMgr_LuaLogPrint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* module_name = ((const char*)  tolua_tostring(tolua_S,1,0));
  int level = ((int)  tolua_tonumber(tolua_S,2,0));
  const char* file_name = ((const char*)  tolua_tostring(tolua_S,3,0));
  long line = ((long)  tolua_tonumber(tolua_S,4,0));
  const char* stringFormat = ((const char*)  tolua_tostring(tolua_S,5,0));
  {
   LuaLogPrint(module_name,level,file_name,line,stringFormat);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'LuaLogPrint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_LogMgr_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_constant(tolua_S,"DEBUG",DEBUG);
  tolua_constant(tolua_S,"INFO",INFO);
  tolua_constant(tolua_S,"NOTICE",NOTICE);
  tolua_constant(tolua_S,"WARNING",WARNING);
  tolua_constant(tolua_S,"ERR",ERR);
  tolua_constant(tolua_S,"CRIT",CRIT);
  tolua_constant(tolua_S,"ALERT",ALERT);
  tolua_constant(tolua_S,"EMERG",EMERG);
  tolua_module(tolua_S,"LogMgr",0);
  tolua_beginmodule(tolua_S,"LogMgr");
   tolua_function(tolua_S,"LuaLogPrint",tolua_LogMgr_LogMgr_LuaLogPrint00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_LogMgr (lua_State* tolua_S) {
 return tolua_LogMgr_open(tolua_S);
};
#endif

