/*! \file   XModuleMacro.h
 *  \brief  Define macros of class factory registry, such as XBEGIN_DEFINE_MODULE.
 *  \note   This file and XModuleImpl.h may be included in different CPP files.\n
 *      if you don't want to use this file and XModuleImpl.h, then you can use XComCreator.h file.
 *  \author Zhang Yun Gui, X3 C++ PluginFramework
 *  \date   2010.10.19
 */
#ifndef X3_PLUGINIMPL_MODULEMACRO_H_
#define X3_PLUGINIMPL_MODULEMACRO_H_

// XBEGIN_DEFINE_MODULE()
//     XDEFINE_CLASSMAP_ENTRY(clsid, cls)
//     XDEFINE_CLASSMAP_ENTRY_Singleton(clsid, cls)
// XEND_DEFINE_MODULE()
// XEND_DEFINE_MODULE_DLL()
// XEND_DEFINE_MODULE_MFCEXTDLL()
// XEND_DEFINE_MODULE_MFCDLL()
  
//#include <Lwdp.h>
//#include "XClassItem.h"
#include "Cx_Object.h"
#include "Cx_Singletonobject.h"



LWDP_NAMESPACE_BEGIN;
//EXTERN_C_BEGIN;

//! Begin group of class factory registry.

/*!
    \ingroup _GROUP_PLUGIN_CORE_
    \see XEND_DEFINE_MODULE
    \see XEND_DEFINE_MODULE_DLL, XEND_DEFINE_MODULE_MFCEXTDLL
*/

/*!
    \ingroup _GROUP_PLUGIN_CORE_
    \see XEND_DEFINE_MODULE
    \see XEND_DEFINE_MODULE_DLL, XEND_DEFINE_MODULE_MFCEXTDLL
*/
#define XBEGIN_DEFINE_CLASS()  \
    static X3CLASSENTRY s_classes_local[] = {

//! Register a regular class.
/*!
    \ingroup _GROUP_PLUGIN_CORE_
    \param clsid class unique id, LWCLSID constant.
    \param cls implement class
*/
#define XDEFINE_CLASSMAP_ENTRY(modid, clsid, cls)      \
    X3CLASSENTRY(1, __T("Cx_Object<") __T(#cls) __T(">"), clsid, modid, \
        reinterpret_cast<PFNXObjectCreator>(&Cx_Object<cls>::CreateObject), \
        reinterpret_cast<PFNXGetObjectCount>(&Cx_Object<cls>::GetObjectCount),  \
        reinterpret_cast<PFNXRefCountByOthers>(&Cx_Object<cls>::GetRefCountByOthers)),

//! Register a single instance class.
/*!
    \ingroup _GROUP_PLUGIN_CORE_
    \param clsid class unique id, LWCLSID constant.
    \param cls implement class
*/
#define XDEFINE_CLASSMAP_ENTRY_Singleton(modid, clsid, cls)    \
    X3CLASSENTRY(MIN_SINGLETON_TYPE,   \
        __T("Cx_SingletonObject<") __T(#cls) __T(">"), clsid, modid, \
        reinterpret_cast<PFNXObjectCreator>(&Cx_SingletonObject<cls>::CreateObject),    \
        reinterpret_cast<PFNXGetObjectCount>(&Cx_SingletonObject<cls>::GetObjectCount), \
        reinterpret_cast<PFNXRefCountByOthers>(&Cx_SingletonObject<cls>::GetRefCountByOthers)),

//! End group of class factory registry.
/*!
    \ingroup _GROUP_PLUGIN_CORE_
    \see XEND_DEFINE_MODULE_DLL, XEND_DEFINE_MODULE_MFCEXTDLL
*/
#define XEND_DEFINE_CLASS() \
        X3CLASSENTRY() \
    }; 


#ifdef _MSC_VER                 // hide warnings
#pragma warning(disable:4710)   // inline function not expanded
#endif


//EXTERN_C_END;
LWDP_NAMESPACE_END;

#endif // X3_PLUGINIMPL_MODULEMACRO_H_
