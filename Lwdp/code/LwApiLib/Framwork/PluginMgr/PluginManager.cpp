/*! \file PluginManager.h
 *  \brief Define plugin manager wrapper class
 *  \author Zhang Yun Gui, X3 C++ PluginFramework
 *  \date   2011.06.30
 */

#include <Lwdp.h>
#include <PluginInc.h>

#include <Interface/PluginLoader/Ix_ObjectFactory.h>
#include <Interface/PluginLoader/Ix_PluginLoader.h>
#include <Interface/PluginLoader/Ix_PluginModuleMgr.h>

#include <LwFramework.h>
#include <XComPtr.h>


/*! \ingroup _GROUP_PLUGIN_CORE_
 *  \brief Plugin manager wrapper class
 *  \see   Ix_ObjectFactory, Ix_PluginLoader
 */

LWDP_NAMESPACE_BEGIN;

//! Unload plugin manager and plugins
void CPluginManager::Unload(bool force_unload)
{
    Ix_PluginLoader* pLoader = GetPluginLoader();
    if (pLoader != NULL)
    {
        if(pLoader->UnloadPluginAll(force_unload) != LWDP_OK)
			return;
    }
		
    if (Handle() != NULL)
    {
        FreeLibrary(Handle());
        Handle() = NULL;
        Factory() = NULL;
    }
}

//! Load plugin manager.
/*!
    \param subdir PluginManagerX3's path, absolute path or relative to instance.
    \param instance used if subdir is a relative path.
    \return true if plugin manager is loaded.
*/
LWRESULT CPluginManager::LoadPluginManager(const tchar_* file_name)
{
    if (Factory())
    {
        Unload();
    }

	MODID modid;
	Ix_Module* pModule = NULL;
	LWRESULT status = LWDP_ERROR;
	if((status = LoadObjectFactory()) == LWDP_PLUGMGR_GETM_PTR_ERROR)
	{
	    if(Fw_LoadModule(file_name, NULL, modid, &pModule) != LWDP_OK)
		{
			return LWDP_PLUGMGR_LOADMODULE_ERROR;
		}
				
		status = LoadObjectFactory();		
	}
	
	return status;
}

LWRESULT CPluginManager::LoadObjectFactory()
{
	if(Factory())
    {
        Unload();
    }
	
	Ix_Module* pModule = Fw_GetModulePtr(MODID_PluginLoader);
	if(!pModule)
	{
		return LWDP_PLUGMGR_GETM_PTR_ERROR;
	}	

	Ix_PluginMgrModule* pMgr = dynamic_cast<Ix_PluginMgrModule*>(pModule);
	if(!pMgr)
		return LWDP_PLUGMGR_NOT_PLUGINMGR;

	Factory() = pMgr->GetRegisterBank();

	if(!Factory())
		return LWDP_PLUGMGR_GET_FACTORY_ERROR;

	Handle() = pModule->GetModuleInstance();
	pModule->Initialize(Factory(), NULL);
	
	return pModule->InitializePlugin();
}

//! Return the object creator.
Ix_ObjectFactory* CPluginManager::GetObjectFactory()
{
    Ix_ObjectFactory* p = Factory();
    return p? p: NULL;
}

//! Return the plugin loading object.
Ix_PluginLoader* CPluginManager::GetPluginLoader()
{
    Cx_Interface<Ix_PluginLoader> factory(GetObjectFactory());
	ASSERT_CHECK_HALT(LWDP_MODULE_LOG, factory.P(), "CPluginManager::GetPluginLoader Factory is Empty!! HALT SYSTEM!!!!");
    return factory.P();
}

LWDP_NAMESPACE_END;


