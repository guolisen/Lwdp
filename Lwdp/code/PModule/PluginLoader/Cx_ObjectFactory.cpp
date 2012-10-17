// Copyright 2008-2011 Guolisen, guolisen@hotmail.com
// 

// author: Guolisen

#include <Lwdp.h>
#include <PluginInc.h>
#include <Interface/FwEntry/Ix_FwIntf.h>

#include "Cx_ObjectFactory.h"


LWDP_NAMESPACE_BEGIN;

Cx_ObjectFactory::Cx_ObjectFactory()
    : m_unloading(0)
{
}

Cx_ObjectFactory::~Cx_ObjectFactory()
{
    for (std::vector<MODULE*>::iterator it = m_modules.begin(); 
        it != m_modules.end(); ++it)
    {
        //x3::SafeDelete(*it);
    }
    m_modules.clear();
}

bool Cx_ObjectFactory::IsCreatorRegister(const LWCLSID& clsid)
{
    return FindEntry(clsid) != NULL;
}

Ix_Object* Cx_ObjectFactory::CreateObject(const LWCLSID& clsid,
                                   X3IID iid,
                                   Ix_Object** ppv,
                                   const MODID& fromModule)
{
    ASSERT_CHECK_RET(LWDP_MODULE_LOG, NULL, (clsid.valid() && ppv != NULL), "clsid Error");
    *ppv = NULL;
		
    int32_ moduleIndex = -1;
    X3CLASSENTRY* pEntry = FindEntry(clsid, &moduleIndex);
#if 0
    if (pEntry && !pEntry->pfnObjectCreator && moduleIndex >= 0)
    {
        if (!LoadDelayPlugin(m_modules[moduleIndex]->filename)
            && 0 == m_unloading)
        {
            CLSMAP::iterator it = m_clsmap.find(clsid.str());
            if (it != m_clsmap.end())
            {
                it->second.second = -1; // next time: moduleIndex = -1
            }
        }
        pEntry = FindEntry(clsid, &moduleIndex);
    }
#endif

    if (pEntry && pEntry->pfnObjectCreator)
    {
    	if(ReferenceCheck(pEntry->modid, fromModule) != LWDP_OK)
			return NULL;
        *ppv = pEntry->pfnObjectCreator(iid, fromModule);
    }

    return *ppv;
}

X3CLASSENTRY* Cx_ObjectFactory::FindEntry(const LWCLSID& clsid,
                                            int32_* moduleIndex)
{
    CLSMAP::iterator it = m_clsmap.find(clsid.str());
    if (moduleIndex)
    {
        *moduleIndex = (it == m_clsmap.end()) ? -1 : it->second.second;
    }
    return (it == m_clsmap.end()) ? NULL : &(it->second.first);
}

LWRESULT Cx_ObjectFactory::ReferenceCheck(const MODID& target_modid, const MODID& srouce_modid)
{
	int32_ srouceIndex = FindModule(srouce_modid);
	if(srouceIndex < 0)
		return LWDP_OK; //Main Programming Call	
	Ix_Module* pSrouceModule = m_modules[srouceIndex]->module;
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_POINTER_IS_NULL, pSrouceModule, 
					 "Cx_ObjectFactory::ReferenceCheck Get Target Module(%s) Error", srouce_modid.str());

	int32_ targetIndex = FindModule(target_modid);
	if(targetIndex < 0)
		return LWDP_PLUGINMGR_NOT_FIND_MODULE;
	Ix_Module* pTargetModule = m_modules[targetIndex]->module;
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_POINTER_IS_NULL, pTargetModule, 
					 "Cx_ObjectFactory::ReferenceCheck Get Target Module(%s) Error", target_modid.str());
		
	//! Level Check
	TPLUGIN_LEVEL sourcrLevel;
	RINOK(pSrouceModule->GetModuleLevel(sourcrLevel));	
	TPLUGIN_LEVEL targetLevel;
	RINOK(pTargetModule->GetModuleLevel(targetLevel));

	if(targetLevel > sourcrLevel)
		return LWDP_PLUGINMGR_PLUGIN_TARGET_LEVEL_HIGH;
	
	//! Domain Check
	TPLUGIN_CODE_DOMAIN sourcrDomain = 0;
	RINOK(pSrouceModule->GetModuleDomain(&sourcrDomain));	
	TPLUGIN_CODE_DOMAIN targetDomain = 0;
	RINOK(pTargetModule->GetModuleDomain(&targetDomain));

	tstring srcStr(sourcrDomain);
	if(srcStr.find(targetDomain) == tstring::npos)
		return LWDP_PLUGINMGR_PLUGIN_TARGET_DOMAIN_ERROR;
	
	return LWDP_OK;
}

int32_ Cx_ObjectFactory::FindModule(const MODID& modid)
{
    if (!modid.valid())
    {
        return -1;
    }

    int32_ i = (int32_)m_modules.size();

    while (--i >= 0 && m_modules[i]->modid != modid)
    {
    }

    return i;
}

Ix_Module* Cx_ObjectFactory::GetModule(const MODID& modid)
{
	if(!modid.valid())
		return NULL;
		
    int32_ index = FindModule(modid);
    if (index >= 0)
    {
        return m_modules[index]->module;
    }

	Cx_Interface<Ix_FwIntf> iFwIntf(CLSID_FwIntf);
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, NULL, !iFwIntf.IsNull(), 
		             "Cx_ObjectFactory::GetModule Get Object(FwIntf) Error");
		
	Ix_Module* moduleEntry = iFwIntf->Api_Fw_GetModulePtr(modid);
	if(!moduleEntry)
	{	
		return NULL;
	}
		
    moduleEntry->Initialize(this, moduleEntry->GetModuleInstance());
    return moduleEntry;
  
}

long_ Cx_ObjectFactory::RegisterClassEntryTable(int32_ moduleIndex)
{
    ASSERT_CHECK_RET(LWDP_MODULE_LOG, -1, (moduleIndex >= 0), "moduleIndex Error");
    //MODULEID hdll = m_modules[moduleIndex]->hdll;
    Ix_Module* pModule = m_modules[moduleIndex]->module;
    ASSERT_CHECK_RET(LWDP_MODULE_LOG, -1, (pModule), "pModule Error");

    if (!m_modules[moduleIndex]->clsids.empty())
    {
        return (long_)(m_modules[moduleIndex]->clsids).size();
    }

    int32_ classCount = pModule->GetClassCount(0);
    if (classCount <= 0)
        return 0;

    std::vector<X3CLASSENTRY> table(classCount);
    uint32_ size = sizeof(X3CLASSENTRY);

    classCount = pModule->GetClassEntryTable(NULL, &size, &table[0], classCount);

    for (int32_ i = 0; i < classCount; i++)
    {
        const X3CLASSENTRY& cls = table[i];

        if (cls.clsid.valid())
        {
            RegisterClass(moduleIndex, cls);
        }
    }

    return classCount;
}

bool Cx_ObjectFactory::RegisterClass(int32_ moduleIndex, const X3CLASSENTRY& cls)
{
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, false, (moduleIndex >= 0 && cls.clsid.valid()), "Para Error");
    X3CLASSENTRY* pOldCls = FindEntry(cls.clsid);

    if (pOldCls && pOldCls->pfnObjectCreator)
    {
        tchar_ msg[1024] = {0};
 		Api_sprintf(msg, 256,
            "The classid '%s' is already registered by '%s', "
            "then '%s' register fail.\n",
            cls.clsid.str(), pOldCls->className, cls.className);
		lw_log_warning(LWDP_MODULE_LOG, msg);
		
        return false;
    }

    m_clsmap[cls.clsid.str()] = MAPITEM(cls, moduleIndex);
    m_modules[moduleIndex]->clsids.push_back(cls.clsid);

    return true;
}

LWRESULT Cx_ObjectFactory::ReleaseModule(const MODID& modid)
{
    int32_ index = FindModule(modid);
    ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_ERROR, (index >= 0), "Get Modid Error");

    MODULE* item = m_modules[index];
    CLSIDS::const_iterator it = item->clsids.begin();

    for (; it != item->clsids.end(); ++it)
    {
        CLSMAP::iterator mit = m_clsmap.find(it->str());
        if (mit != m_clsmap.end())
        {
            m_clsmap.erase(mit);
        }
    }
	
    if (item->owned)
    {   
		Ix_Module* module = item->module;
	    ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_POINTER_IS_NULL, (module), "Module Pointor Error");

		Cx_Interface<Ix_FwIntf> iFwIntf(CLSID_FwIntf);
		ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_OBJECT_POINTER_IS_NULL, !iFwIntf.IsNull(), 
			             "Cx_ObjectFactory::ReleaseModule Get Object(FwIntf) Error");
        iFwIntf->Api_Fw_UnLoadModule(module->GetModuleMODID());	
    }

    // don't remove: m_modules.erase(m_modules.begin() + index);
    item->module = NULL;
    item->clsids.clear();
//	item->modid.clear();
	item->inited = false;
	item->owned  = false;
	memset(item->filename, 0, LWDP_MAX_PATH);

	return LWDP_OK;
}

LWDP_NAMESPACE_END;

