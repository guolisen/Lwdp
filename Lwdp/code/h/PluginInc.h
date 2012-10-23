/*! \file PluginInc.h
 *  \brief Include frequently-used files for plugin project.
 *  \author Guolisen
 *  \date   2011.10.17
 */
#ifndef X3PLUGIN_INC_H
#define X3PLUGIN_INC_H

//#define LWDP_PLUGIN_CONTEXT

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;

typedef const tchar_*  TPLUGIN_CODE_DOMAIN;
#define LWDP_TOP_DOMAIN __T("LWDP")

namespace PlugLevel{
enum ENUM_PLUGIN_CODE_LEVEL
{
	LEVEL0 = 0,   //PModule
	LEVEL1 = 1,   //UModule
	LEVEL2 = 2,   
	LEVEL3 = 3   
};
};
typedef PlugLevel::ENUM_PLUGIN_CODE_LEVEL TPLUGIN_LEVEL;

typedef uint32_ TPLUGIN_TYPE;
enum ENUM_PLUGIN_TYPE
{
	EXTERNAL_PLUGIN_TYPE = 0,
	TEMP_PLUGIN_TYPE
};

#ifdef LWDP_PLUGIN_CONTEXT
const uint32_ iPLUGIN_CONTEXT = 0 | (1 << EXTERNAL_PLUGIN_TYPE);
#else
const uint32_ iPLUGIN_CONTEXT = 0 & ~(1 << EXTERNAL_PLUGIN_TYPE);
#endif

EXTERN_C_END;
LWDP_NAMESPACE_END;

#include <Interface/Ix_Object.h>
#include <Module/XClassItem.h> 
#include <Module/XClassMacro.h> 
#include <Module/XModuleMacro.h> 

#include <Interface/PluginLoader/Ix_ObjectFactory.h>
#include <Interface/Ix_Module.h> 

#include <Module/Cx_Module.h>

#include <Module/XComCreator.h>
#include <Module/XModuleImpl.h>
#include <Module/XModuleDefine.h>

#endif // X3PLUGIN_INC_H
