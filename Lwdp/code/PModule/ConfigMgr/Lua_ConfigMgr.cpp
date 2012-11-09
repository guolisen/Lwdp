/*
** Lua binding: ConfigMgr
** Generated automatically by tolua++-1.0.92 on 11/09/12 23:25:35.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_ConfigMgr_open (lua_State* tolua_S);

#include <LwDp.h>
#include <Interface/ConfigMgr/Ix_XmlMgr.h>
#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include "Lua_Interface_ConfigMgr.h"
using namespace NLwdp;

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_XmlResultsSet (lua_State* tolua_S)
{
 XmlResultsSet* self = (XmlResultsSet*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_CTagEntry (lua_State* tolua_S)
{
 CTagEntry* self = (CTagEntry*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_uint32_ (lua_State* tolua_S)
{
 uint32_* self = (uint32_*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_XmlResults (lua_State* tolua_S)
{
 XmlResults* self = (XmlResults*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_CAttributeStru (lua_State* tolua_S)
{
 CAttributeStru* self = (CAttributeStru*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"Script_Ptr");
 tolua_usertype(tolua_S,"XmlResultsSet");
 tolua_usertype(tolua_S,"CTagEntry");
 tolua_usertype(tolua_S,"XmlResults");
 tolua_usertype(tolua_S,"tstring");
 tolua_usertype(tolua_S,"uint32_");
 tolua_usertype(tolua_S,"CAttributeStru");
}

/* get function: AttribName of class  CAttributeStru */
#ifndef TOLUA_DISABLE_tolua_get_CAttributeStru_AttribName
static int tolua_get_CAttributeStru_AttribName(lua_State* tolua_S)
{
  CAttributeStru* self = (CAttributeStru*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AttribName'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->AttribName);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: AttribName of class  CAttributeStru */
#ifndef TOLUA_DISABLE_tolua_set_CAttributeStru_AttribName
static int tolua_set_CAttributeStru_AttribName(lua_State* tolua_S)
{
  CAttributeStru* self = (CAttributeStru*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AttribName'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->AttribName = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: AttribValue of class  CAttributeStru */
#ifndef TOLUA_DISABLE_tolua_get_CAttributeStru_AttribValue
static int tolua_get_CAttributeStru_AttribValue(lua_State* tolua_S)
{
  CAttributeStru* self = (CAttributeStru*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AttribValue'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->AttribValue);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: AttribValue of class  CAttributeStru */
#ifndef TOLUA_DISABLE_tolua_set_CAttributeStru_AttribValue
static int tolua_set_CAttributeStru_AttribValue(lua_State* tolua_S)
{
  CAttributeStru* self = (CAttributeStru*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'AttribValue'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->AttribValue = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  CAttributeStru */
#ifndef TOLUA_DISABLE_tolua_ConfigMgr_CAttributeStru_new00
static int tolua_ConfigMgr_CAttributeStru_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CAttributeStru",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CAttributeStru* tolua_ret = (CAttributeStru*)  Mtolua_new((CAttributeStru)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CAttributeStru");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  CAttributeStru */
#ifndef TOLUA_DISABLE_tolua_ConfigMgr_CAttributeStru_new00_local
static int tolua_ConfigMgr_CAttributeStru_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CAttributeStru",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CAttributeStru* tolua_ret = (CAttributeStru*)  Mtolua_new((CAttributeStru)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CAttributeStru");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: ThereIs of class  CTagEntry */
#ifndef TOLUA_DISABLE_tolua_get_CTagEntry_ThereIs
static int tolua_get_CTagEntry_ThereIs(lua_State* tolua_S)
{
  CTagEntry* self = (CTagEntry*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ThereIs'",NULL);
#endif
  tolua_pushboolean(tolua_S,(bool)self->ThereIs);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: ThereIs of class  CTagEntry */
#ifndef TOLUA_DISABLE_tolua_set_CTagEntry_ThereIs
static int tolua_set_CTagEntry_ThereIs(lua_State* tolua_S)
{
  CTagEntry* self = (CTagEntry*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ThereIs'",NULL);
  if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->ThereIs = ((bool)  tolua_toboolean(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: propertyName of class  CTagEntry */
#ifndef TOLUA_DISABLE_tolua_get_CTagEntry_propertyName
static int tolua_get_CTagEntry_propertyName(lua_State* tolua_S)
{
  CTagEntry* self = (CTagEntry*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'propertyName'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->propertyName,"tstring");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: propertyName of class  CTagEntry */
#ifndef TOLUA_DISABLE_tolua_set_CTagEntry_propertyName
static int tolua_set_CTagEntry_propertyName(lua_State* tolua_S)
{
  CTagEntry* self = (CTagEntry*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'propertyName'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"tstring",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->propertyName = *((tstring*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: propertyText of class  CTagEntry */
#ifndef TOLUA_DISABLE_tolua_get_CTagEntry_propertyText
static int tolua_get_CTagEntry_propertyText(lua_State* tolua_S)
{
  CTagEntry* self = (CTagEntry*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'propertyText'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->propertyText,"tstring");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: propertyText of class  CTagEntry */
#ifndef TOLUA_DISABLE_tolua_set_CTagEntry_propertyText
static int tolua_set_CTagEntry_propertyText(lua_State* tolua_S)
{
  CTagEntry* self = (CTagEntry*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'propertyText'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"tstring",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->propertyText = *((tstring*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: parentName of class  CTagEntry */
#ifndef TOLUA_DISABLE_tolua_get_CTagEntry_parentName
static int tolua_get_CTagEntry_parentName(lua_State* tolua_S)
{
  CTagEntry* self = (CTagEntry*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'parentName'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->parentName,"tstring");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: parentName of class  CTagEntry */
#ifndef TOLUA_DISABLE_tolua_set_CTagEntry_parentName
static int tolua_set_CTagEntry_parentName(lua_State* tolua_S)
{
  CTagEntry* self = (CTagEntry*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'parentName'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"tstring",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->parentName = *((tstring*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  CTagEntry */
#ifndef TOLUA_DISABLE_tolua_ConfigMgr_CTagEntry_new00
static int tolua_ConfigMgr_CTagEntry_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CTagEntry",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CTagEntry* tolua_ret = (CTagEntry*)  Mtolua_new((CTagEntry)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CTagEntry");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  CTagEntry */
#ifndef TOLUA_DISABLE_tolua_ConfigMgr_CTagEntry_new00_local
static int tolua_ConfigMgr_CTagEntry_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CTagEntry",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CTagEntry* tolua_ret = (CTagEntry*)  Mtolua_new((CTagEntry)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CTagEntry");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetAttrByName of class  CTagEntry */
#ifndef TOLUA_DISABLE_tolua_ConfigMgr_CTagEntry_GetAttrByName00
static int tolua_ConfigMgr_CTagEntry_GetAttrByName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const CTagEntry",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const CTagEntry* self = (const CTagEntry*)  tolua_tousertype(tolua_S,1,0);
  const std::string attr_name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetAttrByName'", NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->GetAttrByName(attr_name);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetAttrByName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator[] of class  CTagEntry */
#ifndef TOLUA_DISABLE_tolua_ConfigMgr_CTagEntry__geti00
static int tolua_ConfigMgr_CTagEntry__geti00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const CTagEntry",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const CTagEntry* self = (const CTagEntry*)  tolua_tousertype(tolua_S,1,0);
  unsigned const int index = ((unsigned const int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator[]'", NULL);
#endif
  {
   const CAttributeStru tolua_ret = (const CAttributeStru)  self->operator[](index);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((CAttributeStru)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"const CAttributeStru");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(const CAttributeStru));
     tolua_pushusertype(tolua_S,tolua_obj,"const CAttributeStru");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.geti'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetAttrCount of class  CTagEntry */
#ifndef TOLUA_DISABLE_tolua_ConfigMgr_CTagEntry_GetAttrCount00
static int tolua_ConfigMgr_CTagEntry_GetAttrCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const CTagEntry",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const CTagEntry* self = (const CTagEntry*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetAttrCount'", NULL);
#endif
  {
   uint32_ tolua_ret = (uint32_)  self->GetAttrCount();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((uint32_)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"uint32_");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(uint32_));
     tolua_pushusertype(tolua_S,tolua_obj,"uint32_");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetAttrCount'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  XmlResults */
#ifndef TOLUA_DISABLE_tolua_ConfigMgr_XmlResults_new00
static int tolua_ConfigMgr_XmlResults_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"XmlResults",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   XmlResults* tolua_ret = (XmlResults*)  Mtolua_new((XmlResults)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"XmlResults");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  XmlResults */
#ifndef TOLUA_DISABLE_tolua_ConfigMgr_XmlResults_new00_local
static int tolua_ConfigMgr_XmlResults_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"XmlResults",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   XmlResults* tolua_ret = (XmlResults*)  Mtolua_new((XmlResults)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"XmlResults");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  XmlResults */
#ifndef TOLUA_DISABLE_tolua_ConfigMgr_XmlResults_delete00
static int tolua_ConfigMgr_XmlResults_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XmlResults",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XmlResults* self = (XmlResults*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator[] of class  XmlResults */
#ifndef TOLUA_DISABLE_tolua_ConfigMgr_XmlResults__geti00
static int tolua_ConfigMgr_XmlResults__geti00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XmlResults",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XmlResults* self = (XmlResults*)  tolua_tousertype(tolua_S,1,0);
  unsigned const int it = ((unsigned const int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator[]'", NULL);
#endif
  {
   const CTagEntry tolua_ret = (const CTagEntry)  self->operator[](it);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((CTagEntry)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"const CTagEntry");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(const CTagEntry));
     tolua_pushusertype(tolua_S,tolua_obj,"const CTagEntry");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.geti'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Clear of class  XmlResults */
#ifndef TOLUA_DISABLE_tolua_ConfigMgr_XmlResults_Clear00
static int tolua_ConfigMgr_XmlResults_Clear00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XmlResults",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XmlResults* self = (XmlResults*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Clear'", NULL);
#endif
  {
   self->Clear();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Clear'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Empty of class  XmlResults */
#ifndef TOLUA_DISABLE_tolua_ConfigMgr_XmlResults_Empty00
static int tolua_ConfigMgr_XmlResults_Empty00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XmlResults",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XmlResults* self = (XmlResults*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Empty'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->Empty();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Empty'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  XmlResultsSet */
#ifndef TOLUA_DISABLE_tolua_ConfigMgr_XmlResultsSet_new00
static int tolua_ConfigMgr_XmlResultsSet_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"XmlResultsSet",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   XmlResultsSet* tolua_ret = (XmlResultsSet*)  Mtolua_new((XmlResultsSet)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"XmlResultsSet");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  XmlResultsSet */
#ifndef TOLUA_DISABLE_tolua_ConfigMgr_XmlResultsSet_new00_local
static int tolua_ConfigMgr_XmlResultsSet_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"XmlResultsSet",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   XmlResultsSet* tolua_ret = (XmlResultsSet*)  Mtolua_new((XmlResultsSet)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"XmlResultsSet");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  XmlResultsSet */
#ifndef TOLUA_DISABLE_tolua_ConfigMgr_XmlResultsSet_delete00
static int tolua_ConfigMgr_XmlResultsSet_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"XmlResultsSet",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  XmlResultsSet* self = (XmlResultsSet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: CreateScriptPtr */
#ifndef TOLUA_DISABLE_tolua_ConfigMgr_Script_Ptr_new00
static int tolua_ConfigMgr_Script_Ptr_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Script_Ptr* tolua_ret = (Script_Ptr*)  CreateScriptPtr();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Script_Ptr");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: GetScriptPtr */
#ifndef TOLUA_DISABLE_tolua_ConfigMgr_Script_Ptr_get00
static int tolua_ConfigMgr_Script_Ptr_get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Script_Ptr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Script_Ptr* ptr = ((Script_Ptr*)  tolua_tousertype(tolua_S,1,0));
  {
   char* tolua_ret = (char*)  GetScriptPtr(ptr);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: GetXmlResults */
#ifndef TOLUA_DISABLE_tolua_ConfigMgr_XmlResultsMod_get00
static int tolua_ConfigMgr_XmlResultsMod_get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"tstring",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  tstring* ptr = ((tstring*)  tolua_tousertype(tolua_S,1,0));
  {
   char* tolua_ret = (char*)  GetXmlResults(ptr);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: LuaConfigMgrGetScript */
#ifndef TOLUA_DISABLE_tolua_ConfigMgr_ConfigMgr_GetScript00
static int tolua_ConfigMgr_ConfigMgr_GetScript00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"Script_Ptr",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  char* module_or_moid = ((char*)  tolua_tostring(tolua_S,1,0));
  char* script_name = ((char*)  tolua_tostring(tolua_S,2,0));
  Script_Ptr* script_buf = ((Script_Ptr*)  tolua_tousertype(tolua_S,3,0));
  {
   unsigned int tolua_ret = (unsigned int)  LuaConfigMgrGetScript(module_or_moid,script_name,*script_buf);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetScript'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: LuaConfigMgrGetModulePropEntry */
#ifndef TOLUA_DISABLE_tolua_ConfigMgr_ConfigMgr_GetModulePropEntry00
static int tolua_ConfigMgr_ConfigMgr_GetModulePropEntry00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"XmlResults",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  char* module_or_moid = ((char*)  tolua_tostring(tolua_S,1,0));
  char* property_name = ((char*)  tolua_tostring(tolua_S,2,0));
  XmlResults* propertys = ((XmlResults*)  tolua_tousertype(tolua_S,3,0));
  {
   unsigned int tolua_ret = (unsigned int)  LuaConfigMgrGetModulePropEntry(module_or_moid,property_name,*propertys);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetModulePropEntry'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: LuaConfigMgrGetModuleTable */
#ifndef TOLUA_DISABLE_tolua_ConfigMgr_ConfigMgr_GetModuleTable00
static int tolua_ConfigMgr_ConfigMgr_GetModuleTable00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"XmlResultsSet",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  char* module_or_moid = ((char*)  tolua_tostring(tolua_S,1,0));
  char* table_name = ((char*)  tolua_tostring(tolua_S,2,0));
  XmlResultsSet* property_table = ((XmlResultsSet*)  tolua_tousertype(tolua_S,3,0));
  {
   unsigned int tolua_ret = (unsigned int)  LuaConfigMgrGetModuleTable(module_or_moid,table_name,*property_table);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetModuleTable'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_ConfigMgr_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"CAttributeStru","CAttributeStru","",tolua_collect_CAttributeStru);
  #else
  tolua_cclass(tolua_S,"CAttributeStru","CAttributeStru","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"CAttributeStru");
   tolua_variable(tolua_S,"AttribName",tolua_get_CAttributeStru_AttribName,tolua_set_CAttributeStru_AttribName);
   tolua_variable(tolua_S,"AttribValue",tolua_get_CAttributeStru_AttribValue,tolua_set_CAttributeStru_AttribValue);
   tolua_function(tolua_S,"new",tolua_ConfigMgr_CAttributeStru_new00);
   tolua_function(tolua_S,"new_local",tolua_ConfigMgr_CAttributeStru_new00_local);
   tolua_function(tolua_S,".call",tolua_ConfigMgr_CAttributeStru_new00_local);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"CTagEntry","CTagEntry","",tolua_collect_CTagEntry);
  #else
  tolua_cclass(tolua_S,"CTagEntry","CTagEntry","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"CTagEntry");
   tolua_variable(tolua_S,"ThereIs",tolua_get_CTagEntry_ThereIs,tolua_set_CTagEntry_ThereIs);
   tolua_variable(tolua_S,"propertyName",tolua_get_CTagEntry_propertyName,tolua_set_CTagEntry_propertyName);
   tolua_variable(tolua_S,"propertyText",tolua_get_CTagEntry_propertyText,tolua_set_CTagEntry_propertyText);
   tolua_variable(tolua_S,"parentName",tolua_get_CTagEntry_parentName,tolua_set_CTagEntry_parentName);
   tolua_function(tolua_S,"new",tolua_ConfigMgr_CTagEntry_new00);
   tolua_function(tolua_S,"new_local",tolua_ConfigMgr_CTagEntry_new00_local);
   tolua_function(tolua_S,".call",tolua_ConfigMgr_CTagEntry_new00_local);
   tolua_function(tolua_S,"GetAttrByName",tolua_ConfigMgr_CTagEntry_GetAttrByName00);
   tolua_function(tolua_S,".geti",tolua_ConfigMgr_CTagEntry__geti00);
   tolua_function(tolua_S,"GetAttrCount",tolua_ConfigMgr_CTagEntry_GetAttrCount00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"XmlResults","XmlResults","",tolua_collect_XmlResults);
  #else
  tolua_cclass(tolua_S,"XmlResults","XmlResults","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"XmlResults");
   tolua_function(tolua_S,"new",tolua_ConfigMgr_XmlResults_new00);
   tolua_function(tolua_S,"new_local",tolua_ConfigMgr_XmlResults_new00_local);
   tolua_function(tolua_S,".call",tolua_ConfigMgr_XmlResults_new00_local);
   tolua_function(tolua_S,"delete",tolua_ConfigMgr_XmlResults_delete00);
   tolua_function(tolua_S,".geti",tolua_ConfigMgr_XmlResults__geti00);
   tolua_function(tolua_S,"Clear",tolua_ConfigMgr_XmlResults_Clear00);
   tolua_function(tolua_S,"Empty",tolua_ConfigMgr_XmlResults_Empty00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"XmlResultsSet","XmlResultsSet","XmlResults",tolua_collect_XmlResultsSet);
  #else
  tolua_cclass(tolua_S,"XmlResultsSet","XmlResultsSet","XmlResults",NULL);
  #endif
  tolua_beginmodule(tolua_S,"XmlResultsSet");
   tolua_function(tolua_S,"new",tolua_ConfigMgr_XmlResultsSet_new00);
   tolua_function(tolua_S,"new_local",tolua_ConfigMgr_XmlResultsSet_new00_local);
   tolua_function(tolua_S,".call",tolua_ConfigMgr_XmlResultsSet_new00_local);
   tolua_function(tolua_S,"delete",tolua_ConfigMgr_XmlResultsSet_delete00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"Script_Ptr",0);
  tolua_beginmodule(tolua_S,"Script_Ptr");
   tolua_function(tolua_S,"new",tolua_ConfigMgr_Script_Ptr_new00);
   tolua_function(tolua_S,"get",tolua_ConfigMgr_Script_Ptr_get00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"XmlResultsMod",0);
  tolua_beginmodule(tolua_S,"XmlResultsMod");
   tolua_function(tolua_S,"get",tolua_ConfigMgr_XmlResultsMod_get00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"ConfigMgr",0);
  tolua_beginmodule(tolua_S,"ConfigMgr");
   tolua_function(tolua_S,"GetScript",tolua_ConfigMgr_ConfigMgr_GetScript00);
   tolua_function(tolua_S,"GetModulePropEntry",tolua_ConfigMgr_ConfigMgr_GetModulePropEntry00);
   tolua_function(tolua_S,"GetModuleTable",tolua_ConfigMgr_ConfigMgr_GetModuleTable00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_ConfigMgr (lua_State* tolua_S) {
 return tolua_ConfigMgr_open(tolua_S);
};
#endif

