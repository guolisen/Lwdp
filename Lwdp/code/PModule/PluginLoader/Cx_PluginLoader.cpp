// Copyright 2008-2011 Guolisen, guolisen@hotmail.com
// 
// author: Guolisen
#include <LwDp.h>
#include <PluginInc.h>
#include <Interface/FwEntry/Ix_FwIntf.h>
#include "Cx_PluginLoader.h"


LWDP_NAMESPACE_BEGIN;


Cx_PluginLoader::Cx_PluginLoader()
    : m_instance(NULL)
{
    m_clsfile[0] = 0;
}
Cx_PluginLoader::~Cx_PluginLoader()
{
}

MODULEID Cx_PluginLoader::GetMainModuleHandle()
{
    return m_instance;
}

LWRESULT Cx_PluginLoader::LoadPluginFile(const tchar_* files)
{
	ASSERT_CHECK_RET(LWDP_API_LOG, LWDP_PARAMETER_ERROR, files, "Cx_PluginLoader::LoadPluginFile Para File Name Empty!");

	MODID modid;
	Ix_Module* pModule = NULL; 
	Cx_Interface<Ix_FwIntf> iFwIntf(CLSID_FwIntf);
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_OBJECT_POINTER_IS_NULL, !iFwIntf.IsNull(), 
		             "Cx_PluginLoader::LoadPluginFile Get Object(FwIntf) Error");	
	RINOK(iFwIntf->Api_Fw_LoadModule(files, this, modid, &pModule));
	
	return LWDP_OK;
}

LWRESULT Cx_PluginLoader::InitializePlugins()
{
    long_ count = 0;

    for (long_ i = 0; i < (long_)m_modules.size(); i++)
    {
        if (m_modules[i]->inited)
        {
            continue;
        }

		Ix_Module* pModule = m_modules[i]->module;
	    ASSERT_CHECK_RET(LWDP_MODULE_LOG, 0, (pModule), "Module Pointor is NULL");
		RINOK(pModule->InitializePlugin());
		
        count++;
        m_modules[i]->inited = true;
    }

    return LWDP_OK;
}

int32_ Cx_PluginLoader::GetPluginIndex(const MODID& modid)
{
    int32_ i = (int32_)m_modules.size();

    while (--i >= 0)
    {
		if(m_modules[i])
		{
			if(modid == m_modules[i]->modid)
			{
				break;
			}
		}
    }

    return i;
}

LWRESULT Cx_PluginLoader::CheckPlugin(const Ix_Module* pModule)
{
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_PARAMETER_ERROR, (pModule), "CheckPlugin Para Error");
	X3MODULEINFO* pInfo = pModule->GetModuleInfo();
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_POINTER_IS_NULL, (pInfo), "CheckPlugin pInfo Pointer is NULL");

	if(pInfo->pluginVersion > iPLUGIN_VERSION)
	{
		lw_log_err(LWDP_MODULE_LOG, "Plugin(%s) Version is so High!", pModule->GetModuleName());
		return LWDP_PLUGINMGR_PLUGINVERSION_IS_HIGH;
	}

	if(pInfo->unicodeTag != UNICODE_TAG)
	{
		lw_log_warning(LWDP_MODULE_LOG, "Plugin(%s)'s Unicode(%d) is different from Framwork!", pModule->GetModuleName(), pInfo->unicodeTag);
		return LWDP_PLUGINMGR_PLUGIN_UNICODE_IS_DIFF;
	}
	
	return LWDP_OK;
}

LWRESULT Cx_PluginLoader::RegisterFrameWork(Ix_Module* pModule)
{
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_PARAMETER_ERROR, (pModule), "Cx_PluginLoader::RegisterFrameWork Para Error");

    int32_ existIndex = GetPluginIndex(pModule->GetModuleMODID());

    if (existIndex >= 0 && m_modules[existIndex]->modid.empty())
    {
        return LWDP_PLUGINMGR_MODULE_ALREADY_LOAD;
    }

	LWRESULT stat = LWDP_ERROR;
    if (pModule)
    {	
    	pModule->Initialize(this, 0);
        if(RegisterPlugin(pModule) == LWDP_OK)
        {
            int32_ moduleIndex = FindModule(pModule->GetModuleMODID());

        	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_PLUGINMGR_MODULE_INDEX_ERROR, (moduleIndex >= 0), "Module Index Error");        
        	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_PLUGINMGR_MODULE_INDEX_ERROR, (existIndex < 0 || existIndex == moduleIndex), "Module Index Error");        
        }
    }
    else
    {
    	return LWDP_PLUGINMGR_LOADMODULE_ERROR;
    }

    return LWDP_OK;
}

LWRESULT Cx_PluginLoader::RegisterPlugin(const Ix_Module* pModule)
{
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_PARAMETER_ERROR, (pModule), "RegisterPlugin Para Error");
    if (FindModule(pModule->GetModuleMODID()) >= 0)
    {
        return false;
    }

    MODULE moduleInfo;
	moduleInfo.modid  = pModule->GetModuleMODID();
    moduleInfo.module = (Ix_Module *)pModule;
    moduleInfo.owned  = (pModule->GetModuleInstance())? true: false;
    moduleInfo.inited = false;

    int32_ moduleIndex = GetPluginIndex(pModule->GetModuleMODID());
    if (moduleIndex >= 0)
    {
    	ASSERT_CHECK_RET(LWDP_MODULE_LOG, false, (m_modules[moduleIndex] != NULL), "Get Plugin Index Error");        
        *m_modules[moduleIndex] = moduleInfo;
    }
    else
    {
        moduleIndex = m_modules.size();
        MODULE* module = new MODULE;
        *module = moduleInfo;
        m_modules.push_back(module);
    }

    RegisterClassEntryTable(moduleIndex);

    return LWDP_OK;
}

LWRESULT Cx_PluginLoader::LoadPlugin(const MODID& modid)
{
    int32_ existIndex = GetPluginIndex(modid);

    if (existIndex >= 0 && m_modules[existIndex]->modid.empty())
    {
        return LWDP_PLUGINMGR_MODULE_ALREADY_LOAD;
    }

	Cx_Interface<Ix_FwIntf> iFwIntf(CLSID_FwIntf);
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_OBJECT_POINTER_IS_NULL, !iFwIntf.IsNull(), 
		             "Cx_PluginLoader::LoadPlugin Get Object(FwIntf) Error");
	Ix_Module* pModule = iFwIntf->Api_Fw_GetModulePtr(modid);
	if(!pModule)
	{
		return LWDP_PLUGINMGR_NOT_FIND_MODULE_ENTRY;
	}

	LWRESULT stat = LWDP_ERROR;
    if (pModule)
    {
    	if((stat = CheckPlugin(pModule)) != LWDP_OK)
    	{
    		if(pModule->GetModuleInstance()) //!< 只有加载的外部模块才Unload
    		{
    			Cx_Interface<Ix_FwIntf> iFwIntf(CLSID_FwIntf);
				ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_OBJECT_POINTER_IS_NULL, !iFwIntf.IsNull(), 
					             "Cx_PluginLoader::LoadPlugin Get Object(FwIntf) Error");
				iFwIntf->Api_Fw_UnLoadModule(pModule->GetModuleMODID());
    		}
			return stat;
    	}
		
    	pModule->Initialize(this, 0);
        if(RegisterPlugin(pModule) == LWDP_OK)
        {
            int32_ moduleIndex = FindModule(pModule->GetModuleMODID());

        	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_PLUGINMGR_MODULE_INDEX_ERROR, (moduleIndex >= 0), "Module Index Error");        
        	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_PLUGINMGR_MODULE_INDEX_ERROR, (existIndex < 0 || existIndex == moduleIndex), "Module Index Error");        
        }
    }
    else
    {
    	return LWDP_PLUGINMGR_LOADMODULE_ERROR;
    }

    return LWDP_OK;
}


LWRESULT Cx_PluginLoader::UnloadPlugin(const MODID& modid, bool force_unload)
{
    int32_ moduleIndex = GetPluginIndex(modid);
	if(moduleIndex < 0)
		return LWDP_PLUGINMGR_NOT_FIND_MODULE;

	Ix_Module* pModule = m_modules[moduleIndex]->module;
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_POINTER_IS_NULL, pModule, "Cx_PluginLoader::UnloadPlugin Get Ix_Module Pointer Error");

	if(!force_unload)
	{
		if(!pModule->CanUnloadPlugin())
			return LWDP_PLUGINMGR_MODULE_USING;
	}
	else
	{
		lw_log_warning(LWDP_MODULE_LOG, "Force Unload Plugin(%s)", pModule->GetModuleName());
	}
	
	pModule->UninitializePlugin();

	MODID tmpModid = pModule->GetModuleMODID();
    ClearModuleItems(tmpModid);
    ReleaseModule(tmpModid);

	return LWDP_OK;
}

LWRESULT Cx_PluginLoader::UnloadPluginAll(bool force_unload)
{
	LWRESULT res  = LWDP_OK;
	LWRESULT stat = LWDP_ERROR;
	for(uint32_ i=0; i<m_modules.size(); ++i)
	{
		Ix_Module* pModule = m_modules[i]->module;
		ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_POINTER_IS_NULL, pModule, "Cx_PluginLoader::UnloadPluginAll Get Ix_Module Pointer Error");

		stat = UnloadPlugin(pModule->GetModuleMODID(), force_unload);
		if(stat != LWDP_OK)
			res = stat;
	}
	
	return res;
}

bool Cx_PluginLoader::ClearModuleItems(const MODID& modid)
{
    Ix_Module* pModule = GetModule(modid);

    if (pModule)
    {
        pModule->ClearModuleItems();
        return true;
    }

    return false;
}

long_ Cx_PluginLoader::GetPluginCount()
{
    return (long_)m_modules.size();
}

bool Cx_PluginLoader::GetPluginFileName(long_ index, MODULEID& hdll, tstring& filename)
{
#if 0
    bool valid = x3::IsValidIndexOf(m_modules, index);

    hdll = valid ? m_modules[index]->hdll : NULL;
    filename = valid ? m_modules[index]->filename :__T"";

    return valid;
#endif
	return true;
}


LWDP_NAMESPACE_END;

