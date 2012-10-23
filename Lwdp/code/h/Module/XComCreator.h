/*! \file XComCreator.h
 *  \brief Implement x3CreateObject() to use Cx_Interface. XModuleMacro.h and XModuleImpl.h are not needed.
 *
 *  \note This file is used in projects which need not to
 *        implement interface and want use interface only,
 *        so XModuleMacro.h and XModuleImpl.h are not need.\n
 *        These projects are usual unit-test applications, COM/Active controls,
 *        simple applications or non plugin-system applications. \n
 *
 *    Usage: Include this file in a cpp file such as StdAfx.cpp, so you can use Cx_Interface.\n
 *    - Load plugins using PluginManager.h before using those interfaces.\n
 *    - If just use only one plugin, you can do as following and not need PluginManager.h :\n
 *    define USE_ONE_PLUGIN then include this file, and assign module handle (using LoadLibrary) to g_hPluginDll.
 *
 *  \author Zhang Yun Gui, X3 C++ PluginFramework
 *  \date   2011.06.30
 */
 #ifndef _X3_COMCREATOR_IMPL_H
 #define _X3_COMCREATOR_IMPL_H

#include <LwApiLib/Framwork/PluginMgr/PluginManager.h>

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;


#ifndef LWDP_PLUGIN_CONTEXT

static const MODID& GetModuleModId()
{
	static MODID MODID_LWDP(__T("8A74DBB9-F23A-4a33-83DE-1BACE814BFB6"));
    return MODID_LWDP;
}

static Ix_ObjectFactory* GetObjectFactory()
{
    return CPluginManager::Factory();
}

static Ix_Object* CreateObject(const LWCLSID& clsid, X3IID iid, Ix_Object** ppv)
{
    if (NULL == ppv)
        return NULL;
    *ppv = NULL;

    Ix_ObjectFactory* factory = GetObjectFactory();
    if (NULL == factory)
        return NULL;  // plugins must are already loaded using PluginManager.h

    return factory->CreateObject(clsid, iid, ppv, NULL);
}


#else // LWDP_PLUGIN_CONTEXT

PLUGINCALL Ix_Module* GetModuleInterface(Ix_ObjectFactory* pFactory, MODULEID hModule);
Ix_ObjectFactory*     GetObjectFactory();
const MODID& GetModuleModId();
Ix_Module* 	 GetCurrentModule();
bool 		 IsCreatorRegister(const LWCLSID& clsid);
Ix_Object* 	 CreateObject(const LWCLSID& clsid, X3IID iid, Ix_Object** ppv);


#endif // LWDP_PLUGIN_CONTEXT


EXTERN_C_END;
LWDP_NAMESPACE_END;

#endif // _X3_COMCREATOR_IMPL_H
