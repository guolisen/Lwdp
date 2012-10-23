/*! \file XModuleImpl.h
 *  \brief Implement plugin's export functions and x3CreateObject.
 *  \note Include this file in one and only one CPP file of your plugin project.\n
 *        If you don't want to use XModuleMacro.h and this file, then you can use XComCreator.h file.
 *  \author Zhang Yun Gui, X3 C++ PluginFramework
 *  \date   2011.06.30
 */
#ifndef X3_PLUGINIMPL_MODULEIMPL_H_
#define X3_PLUGINIMPL_MODULEIMPL_H_
 

#ifdef LWDP_MODULE_IMPL
using NLwdp::LWDP_MODULE_LOG;
using NLwdp::_lw_applLog;

static NLwdp::Ix_Module*    s_lwdpModule;


#ifdef LWDP_PLUGIN_CONTEXT
//! A export function used by the plugin manager.
/*! \ingroup _GROUP_PLUGIN_CORE2_
    \param pFactory the plugin manager object, skip if NULL.
    \param hModule DLL handle of this plugin, skip if NULL.
    \return the module object in this plugin.
    \see x3InternalCreateObject, x3GetClassEntryTable
*/
PLUGINCALL NLwdp::Ix_Module* GetModuleInterface(NLwdp::Ix_ObjectFactory* pFactory, NLwdp::MODULEID hModule)
{	
	if (pFactory || hModule)
    {
		ASSERT_CHECK_RET(LWDP_MODULE_LOG, NULL, 
						 s_lwdpModule, "s_lwdpModule is NULL!");
    	
        s_lwdpModule->Initialize(pFactory, hModule);
    }
	
    return s_lwdpModule;
}

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;

//! Return DLL handle of the current plugin module.
//! Using it can check how many objects are used by other modules.
MODID emptyModid("");
const MODID& GetModuleModId()
{
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, emptyModid, 
					 s_lwdpModule, "s_lwdpModule is NULL!");

    return s_lwdpModule->GetModuleMODID();
}

Ix_ObjectFactory* GetObjectFactory()
{
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, NULL, 
					 s_lwdpModule, "s_lwdpModule is NULL!");

    return s_lwdpModule->GetObjectFactory();
}

Ix_Module* GetCurrentModule()
{
    return s_lwdpModule;
}


bool IsCreatorRegister(const LWCLSID& clsid)
{
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, NULL, 
					 s_lwdpModule, "s_lwdpModule is NULL!");

    Ix_ObjectFactory* pFactory = s_lwdpModule->GetObjectFactory();
    return pFactory && pFactory->IsCreatorRegister(clsid);
}

//! Object factory function used by Cx_Interface or Cx_Ptr.
/*! \ingroup _GROUP_PLUGIN_CORE2_
    Autoptr classes (Cx_Interface and Cx_Ptr) will call it. \n
    If you don't want to use this file, then you can use XComCreator.h file.
    \param clsid class unique id, must be valid.
    \param iid interface id, see DEFINE_IID.
    \param ppv Pass in a valid address and return a new object.
    \return 0 or failed.
    \see x3InternalCreateObject, Ix_ObjectFactory, x3IsCreatorRegister
*/
Ix_Object* CreateObject(const LWCLSID& clsid, X3IID iid, Ix_Object** ppv)
{
    if (NULL == ppv)
        return NULL;
    *ppv = NULL;

	ASSERT_CHECK_RET(LWDP_MODULE_LOG, NULL, 
					 s_lwdpModule, "s_lwdpModule is NULL!");

    int32_ ret = s_lwdpModule->InternalCreateObject(clsid.str(), iid, ppv, GetModuleModId());
	Ix_Object* pObj = NULL;
    if (ret != 0)
    {
        Ix_ObjectFactory* pFactory = s_lwdpModule->GetObjectFactory();
        if (pFactory)
        {
            pObj = pFactory->CreateObject(clsid, iid, ppv, GetModuleModId());
        }
    }

    return pObj;
}



EXTERN_C_END;
LWDP_NAMESPACE_END;

#endif

#endif


#endif // X3_PLUGINIMPL_MODULEIMPL_H_
