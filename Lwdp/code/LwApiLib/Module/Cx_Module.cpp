// Copyright (C), 2006��2011, Potevio Institute of Technology.
/************************************************************
*  File name:   LwLogMessage.cpp  	
*  Author:      guolisen
*  Version:   	v1.0
*  Create Date: 2011-08-11
***********************************************************/
// 2011.08.11: Create By Guolisen

#include <iostream>
#include <algorithm>
#include <LwDp.h>
#include <PluginInc.h>

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;

//! Initialize this plugin. Used by x3GetModuleInterface().
void Cx_Module::Initialize(Ix_ObjectFactory* pFactory, MODULEID hModule)
{
    bool changed = false;

    if (m_pFactory != pFactory)
    {
        m_pFactory = pFactory;
        changed = true;
    }
    if (hModule && !m_hModule)
    {
        m_hModule = hModule;
        changed = true;
    }

    if (changed)
    {
        Cx_ModuleItem::InitModuleItems(GetClassCount(MIN_SINGLETON_TYPE));
    }
}


void Cx_Module::ClearModuleItems()
{
    Cx_ModuleItem::ClearModuleItems();
}
long_ Cx_Module::GetUnfreeObjectCount()
{
    long n, nTotal = 0;
    X3CLASSENTRY* pCls = m_classEntry;

    for (; pCls->pfnObjectCreator; ++pCls)
    {
        PFNXGetObjectCount pfn = pCls->pfnGetObjectCount;
        if (pfn && (n = (*pfn)()) > 0)
        {
            nTotal += n;

#if 0
#ifdef OutputDebugString
            OutputDebugStringA("> Unfree: ");
            OutputDebugStringA(pCls->className);
#ifdef TRACE1
            TRACE1(", %d", n);
#endif
            OutputDebugStringA("!\n");
#endif // OutputDebugString
#endif
        }
    }

    return nTotal;
}

X3MODULEINFO* Cx_Module::GetModuleInfo() const {return m_moduleInfo;}
const tchar_* Cx_Module::GetModuleName() const {return m_moduleInfo->className;}
MODID&        Cx_Module::GetModuleMODID() const {return m_moduleInfo->modId;}
Ix_ObjectFactory* Cx_Module::GetObjectFactory() const { return m_pFactory; }
MODULEID	  Cx_Module::GetModuleInstance() const { return m_hModule; }	

LWRESULT Cx_Module::GetModuleType(TPLUGIN_TYPE& module_type) const
{
	module_type = m_moduleInfo->pluginType;
	return 0;
}
LWRESULT Cx_Module::GetModuleDomain(TPLUGIN_CODE_DOMAIN* module_domain) const
{
	*module_domain = m_moduleInfo->codeDomain;
	return 0;
}
LWRESULT Cx_Module::GetModuleLevel(TPLUGIN_LEVEL& module_level) const
{
	module_level = m_moduleInfo->pluginLevel;
	return 0;
}

int32_ Cx_Module::GetClassEntryTable(int32_* pBuildInfo, uint32_* pEntrySize,
											X3CLASSENTRY* pTable, int32_ nMaxCount)

{
    if (pBuildInfo)
    {
        *pBuildInfo = 0;
#ifdef _MSC_VER
        *pBuildInfo |= (4 | (_MSC_VER << 8));
#endif
#ifdef __GNUC__
        *pBuildInfo |= (8 | (__GNUC__ << 8));
#endif
#ifdef _DEBUG
        *pBuildInfo |= 1;
#endif
    }

    uint32_ nEntrySize = sizeof(X3CLASSENTRY);

    if (pEntrySize)
    {
        nEntrySize = nEntrySize < *pEntrySize ? nEntrySize : *pEntrySize;
        nEntrySize = nEntrySize ? nEntrySize : sizeof(X3CLASSENTRY);
        *pEntrySize = nEntrySize;
    }

    int32_ nClassCount = GetClassCount(0);

    if (NULL == pTable)
    {
        return nClassCount;
    }

    nClassCount = nClassCount < nMaxCount ? nClassCount : nMaxCount;
    for (int32_ i = 0; i < nClassCount; i++)
    {
        memcpy((byte_*)pTable + i * nEntrySize, m_classEntry + i, nEntrySize);
    }

    return nClassCount;
}

long_ Cx_Module::GetClassCount(byte_ minType)
{
    int32_ nClassCount = 0;
    X3CLASSENTRY* pCls = m_classEntry;

    for (; pCls->pfnObjectCreator; ++pCls)
    {
        if (pCls->type >= minType)
        {
            nClassCount++;
        }
    }

    return nClassCount;
}

bool Cx_Module::CanUnloadPlugin()
{
    X3CLASSENTRY* pCls = m_classEntry;

    for (; pCls->pfnObjectCreator; ++pCls)
    {
        PFNXRefCountByOthers pfn = pCls->pfnRefCountByOthers;
        long_ n = 0;

        if (pfn && (n = (*pfn)()) > 0)
        {
            return false;
        }
    }

    return true;
}
	
int32_ Cx_Module::InternalCreateObject(const tchar_* clsid, X3IID iid, 
                            			  Ix_Object** ppv, const MODID& fromModule)
{
    if (NULL == ppv)
        return -1;
    *ppv = NULL;

    LWCLSID clsid2(clsid);
    X3CLASSENTRY* pCls = m_classEntry;

    for (; pCls->pfnObjectCreator; ++pCls)
    {
        if (clsid2 == pCls->clsid)
        {
            *ppv = (*pCls->pfnObjectCreator)(iid, fromModule);
            return 0;
        }
    }

    return -1;
}




EXTERN_C_END;
LWDP_NAMESPACE_END;
    



