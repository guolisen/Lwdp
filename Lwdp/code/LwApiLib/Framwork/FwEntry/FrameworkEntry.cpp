// Copyright (C), 2006��2011, Potevio Institute of Technology.
/************************************************************
*  File name:   FrameworkEntry.cpp  	
*  Author:      guolisen
*  Version:   	v1.0
*  Create Date: 2011-08-11
***********************************************************/
// 2011.08.11: Create By Guolisen

#include <LwDp.h>
#include <PluginInc.h>
#include <Interface/PluginLoader/Ix_PluginLoader.h>
#include <Interface/ScriptMgr/Ix_ScriptMgr.h>
#include <Interface/ConfigMgr/Ix_Configmgr.h>
#include <Interface/FwEntry/Ix_FwIntf.h>

#include <LwApiLib/Framwork/FwEntry/FrameworkEntry.h>

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;

typedef struct tag_SysModule
{
	const MODID 	modid;
	const tchar_* 	moduleName;
	const tchar_*   directory;
}SYS_MODULE;


extern "C" LWRESULT RegisteFwModule(Ix_Module** pModule);

static const SYS_MODULE    gPlatformModule[]    = {{MODID_ConfigMgr, "ConfigMgr", CFG_CONFIGMGR_DIR_STR}};
static const uint32_ gPlatformModuleCount = sizeof(gPlatformModule) / sizeof(SYS_MODULE);
CPluginManager* gPluginMgr;

LWRESULT Fw_Init(Ix_ConfigSrc* config_src, uint32_ entry_count)
{
	ASSERT_CHECK_RET(LWDP_API_LOG, LWDP_PARAMETER_ERROR, (config_src), "Fw_Init() Parameter Error");

#ifdef defined(_WIN32) || defined(WIN32)

	int res = FALSE;
	res = pthread_win32_process_attach_np(); 
	if(!res) //if false
	{
		lw_log_err(LWDP_MODULE_LOG, "pThread Init Error!");
		return LWDP_ERROR;
	}
	lw_log_info(LWDP_MODULE_LOG, "pThread Init OK!");
#endif

	gPluginMgr = new CPluginManager;
	ASSERT_CHECK_RET(LWDP_API_LOG, LWDP_MALLOC_MEMORY_ERROR, (gPluginMgr), "Fw_Init() gPluginMgr new Error");
	RINOK(gPluginMgr->LoadPluginManager(CFG_PLUGINLOADER_DIR_STR));

	//! ���ģ�����
	Ix_Module* fwModule = NULL;
	RINOK(RegisteFwModule(&fwModule));
	gPluginMgr->GetPluginLoader()->RegisterFrameWork(fwModule);
	//! Framework Module Init
	RINOK(gPluginMgr->GetPluginLoader()->InitializePlugins());

	LWRESULT stat = LWDP_ERROR;
	for(uint32_ i=0; i<gPlatformModuleCount; ++i)
	{
		stat = gPluginMgr->GetPluginLoader()->LoadPluginFile(gPlatformModule[i].directory);
		if(stat != LWDP_OK)
			lw_log_debug(LWDP_MODULE_LOG, "Cannot Load Platform Plugin File(%s)!", gPlatformModule[i].directory);
		else
			lw_log_debug(LWDP_MODULE_LOG, "Load Platform Plugin File From External(%s)!", gPlatformModule[i].directory);

		RINOK(gPluginMgr->GetPluginLoader()->LoadPlugin(gPlatformModule[i].modid));
		lw_log_notice(LWDP_MODULE_LOG, "Load Platform Plugin(%s) OK!", gPlatformModule[i].moduleName);
	}
	
	//! Platform Module Init
	RINOK(gPluginMgr->GetPluginLoader()->InitializePlugins());
		
    //Cx_Interface<Ix_ConfigMgr> iConfigMgr(CLSID_ConfigMgr);
	GET_OBJECT_RET(ConfigMgr, iConfigMgr, LWDP_GET_OBJECT_ERROR);
	for(uint32_ i=0; i<entry_count; ++i)
		RINOK(iConfigMgr->AddConfigSrc(&config_src[i]));

	//Temp Load
	XPropertyTable moduleTable;
	RINOK(iConfigMgr->GetModuleTable(LW_CONFIG_LWDP_TAG, LW_CONFIG_MODULETABLE_TAG, moduleTable));

	for(uint32_ i=0; moduleTable[i].ThereIs; ++i)
	{
		XPropertys prop;
		iConfigMgr->GetModulePropEntry(moduleTable[i][LW_CONFIG_NAME_ATTR], LW_CONFIG_MODULE_DIR_TAG, prop);
		if(!prop[0].propertyText.empty())
		{
			lw_log_debug(LWDP_MODULE_LOG, "--> Start Load Plugin (%s)!", prop[0].propertyText.c_str());
			stat = gPluginMgr->GetPluginLoader()->LoadPluginFile(prop[0].propertyText.c_str());
			if(stat != LWDP_OK)
				lw_log_debug(LWDP_MODULE_LOG, "Cannot Load Plugin File(%s)!", prop[0].propertyText.c_str());
			else
				lw_log_debug(LWDP_MODULE_LOG, "Load Plugin File From External(%s)!", prop[0].propertyText.c_str());
		}
		MODID tmpModid(moduleTable[i][LW_CONFIG_MODID_ATTR].c_str());
		RINOK(gPluginMgr->GetPluginLoader()->LoadPlugin(tmpModid));
		lw_log_notice(LWDP_MODULE_LOG, "--> Load Platform Plugin(%s) OK!", moduleTable[i][LW_CONFIG_NAME_ATTR].c_str());
	}

	//User Module Init
	RINOK(gPluginMgr->GetPluginLoader()->InitializePlugins());

    return LWDP_OK;
}

LWRESULT Fw_Start()
{
	Script_Ptr code_buf; 
	GET_OBJECT_RET(ConfigMgr, iConfigMgr, LWDP_GET_OBJECT_ERROR);
	RINOK(iConfigMgr->GetModuleScript(LW_FW_TOP_TAG, LW_FW_SCRIPT_GLOBAL_INIT_TAG, code_buf));
	
	GET_OBJECT_RET(ScriptMgr, iScriptMgr, LWDP_GET_OBJECT_ERROR);
	RINOK(iScriptMgr->SyncScriptRun(code_buf));

    return LWDP_OK;
}

LWRESULT Fw_Stop()
{
	Script_Ptr code_buf; 
	GET_OBJECT_RET(ConfigMgr, iConfigMgr, LWDP_GET_OBJECT_ERROR);
	RINOK(iConfigMgr->GetModuleScript(LW_FW_TOP_TAG, LW_FW_SCRIPT_GLOBAL_UNINIT_TAG, code_buf));
	
	GET_OBJECT_RET(ScriptMgr, iScriptMgr, LWDP_GET_OBJECT_ERROR);
	RINOK(iScriptMgr->SyncScriptRun(code_buf));

    return LWDP_OK;
}

//BEGIN Api_RegisterModule
MODULE_ENTRY* gModuleTable[MAX_PLUGIN_NUM] = {0};
long_         gModuleNum = 0;

TABCAT findModuleEntryByModid(const MODID& modid)
{
	TABCAT cat = 0;
	ASSERT_CHECK_RET(LWDP_API_LOG, NULL, (gModuleTable!=NULL), __T("gModuleTable is NULL"));
	while(cat < MAX_PLUGIN_NUM)
	{
		if(gModuleTable[cat])
		{
			if(modid == gModuleTable[cat]->module_modid)
				return cat;
		}
		++cat;
	}
    
	return -1;
}
TABCAT findModuleEntryByName(const tchar_* module_name)
{
	TABCAT cat = 0;
	ASSERT_CHECK_RET(LWDP_API_LOG, NULL, (gModuleTable!=NULL), __T("gModuleTable is NULL"));
	while(cat < MAX_PLUGIN_NUM)
	{
		if(gModuleTable[cat])
		{
			if(Api_tcscmp(module_name, gModuleTable[cat]->module_name) == 0)
				return cat;
		}
		++cat;
	}

	return -1;
}
TABCAT findEmptyEntry()
{
	TABCAT cat = 0;
	ASSERT_CHECK_RET(LWDP_API_LOG, NULL, (gModuleTable!=NULL), __T("gModuleTable is NULL"));
	while(cat < MAX_PLUGIN_NUM)
	{
		if(NULL == gModuleTable[cat])
			return cat;

		++cat;
	}

	return -1;
}

LWRESULT Fw_UnLoadModule(const MODID& modid)
{
	TABCAT i = 0;
	ASSERT_CHECK_RET(LWDP_API_LOG, NULL, (gModuleTable!=NULL), __T("gModuleTable is NULL"));
	while(i < MAX_PLUGIN_NUM)
	{
		if(gModuleTable[i])
		{
			if(modid == gModuleTable[i]->module_modid)
			{
				lw_log_debug(LWDP_API_LOG, "Remove Internal Module(%s)", gModuleTable[i]->module_name);
				--gModuleNum;
				gModuleTable[i]->module_name = NULL;
				gModuleTable[i]->module_modid.clear();
				MODULEID dllId = NULL;
				if((dllId=gModuleTable[i]->module_ptr->GetModuleInstance()) != NULL)
				{
					Api_FreeLibrary(dllId);
				}
				gModuleTable[i]->module_ptr = NULL;
				DELETE_SINGLE(gModuleTable[i]);
				break;
			}
		}
		i++;
	}

	return LWDP_OK;
}

LWRESULT Fw_RegisteModule(Ix_Module* plugin_module)
{
	ASSERT_CHECK_RET(LWDP_API_LOG, LWDP_PARAMETER_ERROR, (plugin_module), __T("Api_RegisterModule Para(plugin_module) is NULL"));

	MODID tmpModid = plugin_module->GetModuleMODID();
	TABCAT cat = findModuleEntryByModid(tmpModid);
	if(cat >= 0)
	{
		Fw_UnLoadModule(tmpModid);
	}

	TABCAT emptyEntry = findEmptyEntry();
	if(emptyEntry < 0)
		return LWDP_FWE_MODULE_TABLE_FULL;

	gModuleTable[emptyEntry] = new MODULE_ENTRY;
	ASSERT_CHECK_RET(LWDP_API_LOG, LWDP_MALLOC_MEMORY_ERROR, (gModuleTable[emptyEntry]), __T("new Memory for gModuleTable Error"));

	++gModuleNum;
	gModuleTable[emptyEntry]->module_name  = plugin_module->GetModuleName();
	ASSERT_CHECK_RET(LWDP_API_LOG, LWDP_POINTER_IS_NULL, (gModuleTable[emptyEntry]->module_name), __T("Api_RegisterModule Module Name is NULL"));
	gModuleTable[emptyEntry]->module_modid = plugin_module->GetModuleMODID();
	ASSERT_CHECK_RET(LWDP_API_LOG, LWDP_POINTER_IS_NULL, (gModuleTable[emptyEntry]->module_modid.valid()), __T("Api_RegisterModule Modid is NULL"));
	gModuleTable[emptyEntry]->module_ptr   = plugin_module;
	
	return LWDP_OK;
}

//END Api_RegisterModule 
const tchar_* Fw_GetModuleName(const MODID& modid)
{
	ASSERT_CHECK_RET(LWDP_API_LOG, NULL, (modid.valid()), __T("Api_GetModuleName modid is valid"));
		
	TABCAT targetEntry = findModuleEntryByModid(modid);
	if(targetEntry < 0 || targetEntry >= MAX_PLUGIN_NUM)
		return NULL;

    return gModuleTable[targetEntry]->module_name;
}

Ix_Module* Fw_GetModulePtr(const MODID& modid)
{
	ASSERT_CHECK_RET(LWDP_API_LOG, NULL, (modid.valid()), __T("Api_GetModuleName modid is valid"));
		
	TABCAT targetEntry = findModuleEntryByModid(modid);
	if(targetEntry < 0 || targetEntry >= MAX_PLUGIN_NUM)
		return NULL;

    return gModuleTable[targetEntry]->module_ptr;
}


class Ix_Module;
class Ix_ObjectFactory;
LWRESULT Fw_LoadModule(const tchar_* file_name, const Ix_ObjectFactory* plugin_factory, MODID& modid, Ix_Module** pModuleObj)
{
	if(!file_name)
		return LWDP_PARAMETER_ERROR;
		
	long_ oldNum = gModuleNum;
	char buf[2048] = {0};
	MODULEID dllHandle = Api_LoadLibrary(file_name);
	if(!dllHandle)
		return LWDP_FWE_LOADMODULE_ERROR;

	//for Vxworks
	if(oldNum != gModuleNum)
		return LWDP_OK;

	typedef Ix_Module* (*GET_MODULE_INTF)(const Ix_ObjectFactory* , MODULEID );
    GET_MODULE_INTF pGetModuleIntf = (GET_MODULE_INTF)Api_GetProcAddress(
        							  dllHandle, "GetModuleInterface");

	if(!pGetModuleIntf)
	{
		Api_FreeLibrary(dllHandle);
		return LWDP_FWE_GETFUNCTION_ERROR;
	}

	Ix_Module* pModule = pGetModuleIntf(plugin_factory, dllHandle);
	if(!pModule)
	{
		Api_FreeLibrary(dllHandle);
		return LWDP_FWE_GETMODUEL_ENTRY_ERROR;
	}

	//RegisteModule
	LWRESULT stat = Fw_RegisteModule(pModule);
	if(stat != LWDP_OK)
	{
		Api_FreeLibrary(dllHandle);
		return stat;
	}
	
	*pModuleObj = pModule;
	modid = pModule->GetModuleMODID();
	return LWDP_OK;
}

EXTERN_C_END;
LWDP_NAMESPACE_END;
    
