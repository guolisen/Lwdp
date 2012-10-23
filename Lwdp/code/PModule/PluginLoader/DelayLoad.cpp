// Copyright 2008-2011 Zhang Yun Gui, rhcad@hotmail.com
// http://sourceforge.net/projects/x3c/
#include <LwDp.h>
#include <PluginInc.h>
#include "Cx_PluginLoader.h"

LWDP_NAMESPACE_BEGIN;
bool Cx_PluginLoader::LoadPluginOrDelay(const tchar_* pluginFile)
{
#if 0
    if (GetPluginIndex(pluginFile) >= 0)
    {
        return true;
    }

    bool ret = false;

    if (0)
    {
        ret = LoadPluginCache(pluginFile)
            || (LoadPlugin(pluginFile) && (BuildPluginCache(pluginFile) || 1));
    }
    else
    {
        ret = LoadPlugin(pluginFile);
    }

    return ret;
#endif
	return true;
}

bool Cx_PluginLoader::LoadDelayPlugin(const tchar_* filename)
{
#if 0
    bool ret = false;

    if (LoadPlugin(filename))
    {
        int32_ moduleIndex = GetPluginIndex(filename);

        typedef bool (*FUNC_PLUGINLOAD)();
        FUNC_PLUGINLOAD pfn = (FUNC_PLUGINLOAD)Api_GetProcAddress(
            m_modules[moduleIndex]->hdll, "x3InitializePlugin");

        if (pfn && !(*pfn)())
        {
            X3LOG_WARNING2(__T"@PluginManager:IDS_INITPLUGIN_FAIL", filename);
            VERIFY(UnloadPlugin(filename));
        }
        else
        {
            m_modules[moduleIndex]->inited = true;
            BuildPluginCache(filename);
            ret = true;
        }
    }

    return ret;
#endif
	return true;
}

bool Cx_PluginLoader::BuildPluginCache(const tchar_* pluginFile)
{
#if 0
    int32_ moduleIndex = GetPluginIndex(pluginFile);
    ASSERT(moduleIndex >= 0);

    CLSIDS oldids;
    LoadClsids(oldids, pluginFile);

    return oldids != m_modules[moduleIndex]->clsids
        && SaveClsids(m_modules[moduleIndex]->clsids, pluginFile);
#endif
	return true;
}

bool Cx_PluginLoader::LoadPluginCache(const tchar_* filename)
{
#if 0
    int32_ moduleIndex = GetPluginIndex(filename);

    if (moduleIndex >= 0 && !m_modules[moduleIndex]->clsids.empty())
    {
        return true;
    }

    X3CLASSENTRY cls;
    CLSIDS clsids;

    if (!LoadClsids(clsids, filename))
    {
        return false;
    }

    if (moduleIndex < 0)
    {
        MODULE* module = new MODULE;

        module->hdll = NULL;
        module->module = NULL;
        module->owned = false;
        module->inited = false;
        wcsncpy_s(module->filename, LWDP_MAX_PATH, filename, LWDP_MAX_PATH);

        moduleIndex = x3::GetSize(m_modules);
        m_modules.push_back(module);
    }

    memset(&cls, 0, sizeof(cls));
    for (CLSIDS::const_iterator it = clsids.begin(); it != clsids.end(); ++it)
    {
        cls.clsid = *it;
        if (m_clsmap.find(cls.clsid.str()) == m_clsmap.end())
        {
            m_clsmap[cls.clsid.str()] = MAPITEM(cls, moduleIndex);
            m_modules[moduleIndex]->clsids.push_back(cls.clsid);
        }
    }

#endif
    return true;
}

//#include <Xml/Ix_ConfigXml.h>
//#include <Xml/Cx_ConfigSection.h>
//#include <Xml/Ix_ConfigTransaction.h>
//#include <UtilFunc/ConvStr.h>

bool Cx_PluginLoader::LoadCacheFile(const tchar_* pluginFile)
{
#if 0
    bool loaded = false;

    if (m_cache.IsNull() && 0 == m_clsfile[0])
    {
        // Ensure ConfigXml.plugin is loaded.
        wcscpy_s(m_clsfile, LWDP_MAX_PATH, pluginFile);
        Api_PathRemoveFileSpec(m_clsfile);
        Api_PathAppend(m_clsfile,__T"ConfigXml.plugin" PLNEXT);
        loaded = LoadPlugin(m_clsfile);

        Cx_Interface<Ix_ConfigXml> pIFFile(x3::CLSID_ConfigXmlFile);
        if (pIFFile)
        {
            // Get application name.
            Api_GetModuleFileName(m_instance, m_clsfile, LWDP_MAX_PATH);
            tstring appname(Api_PathFindFileName(m_clsfile));

            // Make cache file name.
            wcsncpy_s(m_clsfile, LWDP_MAX_PATH, GetWorkPath().c_str(), LWDP_MAX_PATH);
            Api_PathAppend(m_clsfile, appname.c_str());
            Api_PathRenameExtension(m_clsfile,__T".clsbuf");

            pIFFile->SetFileName(m_clsfile);
            m_cache = pIFFile;
        }
    }

    return loaded;
#endif
	return true;
}

bool Cx_PluginLoader::LoadClsids(CLSIDS& clsids, const tchar_* pluginFile)
{
#if 0
    Cx_Interface<Ix_ConfigXml> pIFFile(m_cache);

    clsids.clear();

    if (pIFFile)
    {
        Cx_ConfigSection seclist(pIFFile->GetData()->GetSection(NULL,
           __T"plugins/plugin",__T"filename", Api_PathFindFileName(pluginFile), false));
        seclist = seclist.GetSection(__T"clsids", false);

        for (int32_ i = 0; i < 999; i++)
        {
            Cx_ConfigSection sec(seclist.GetSectionByIndex(__T"clsid", i));
            if (!sec->IsValid())
            {
                break;
            }
            LWCLSID clsid(x3::w2a(sec->GetString(__T"id")).c_str());
            if (clsid.valid() && x3::find_value(clsids, clsid) < 0)
            {
                clsids.push_back(clsid);
            }
        }
    }

    bool has = !clsids.empty();
    if (!has)
    {
        Cx_ConfigSection seclist(pIFFile->GetData()->GetSection(NULL,
           __T"plugins/plugin",__T"filename", Api_PathFindFileName(pluginFile), false));
        seclist = seclist.GetSection(__T"observers", false);
        has = (seclist.GetSectionCount(__T"observer") > 0);
    }

    return has;
#endif
	return true;
}

bool Cx_PluginLoader::SaveClsids(const CLSIDS& clsids, const tchar_* pluginFile)
{
#if 0
    Cx_Interface<Ix_ConfigXml> pIFFile(m_cache);

    if (pIFFile)
    {
        Cx_ConfigSection seclist(pIFFile->GetData()->GetSection(NULL,
           __T"plugins/plugin",__T"filename", Api_PathFindFileName(pluginFile)));

        seclist = seclist.GetSection(__T"clsids");
        seclist.RemoveChildren(__T"clsid");

        for (CLSIDS::const_iterator it = clsids.begin();
            it != clsids.end(); ++it)
        {
            Cx_ConfigSection sec(seclist.GetSection(
               __T"clsid",__T"id", x3::a2w(it->str()).c_str()));

            X3CLASSENTRY* pEntry = FindEntry(*it);
            if (pEntry && pEntry->pfnObjectCreator)
            {
                sec->SetString(__T"class", x3::a2w(pEntry->className).c_str());
            }
        }
    }
#endif
    return true;
}

bool Cx_PluginLoader::SaveClsids()
{
#if 0
    Cx_ConfigTransaction autosave(m_cache);
    return autosave.Submit();
#endif	
	return true;
}

void Cx_PluginLoader::AddObserverPlugin(MODULEID hdll, const tchar_* obtype)
{
#if 0
    tchar_ filename[LWDP_MAX_PATH] = { 0 };
    Cx_Interface<Ix_ConfigXml> pIFFile(m_cache);

    if (pIFFile)
    {
        Api_GetModuleFileName(hdll, filename, LWDP_MAX_PATH);

        Cx_ConfigSection seclist(pIFFile->GetData()->GetSection(NULL,
           __T"observers/observer",__T"type", x3::a2w(obtype).c_str()));
        seclist.GetSection(__T"plugin",__T"filename", Api_PathFindFileName(filename));

        seclist = pIFFile->GetData()->GetSection(NULL,
           __T"plugins/plugin",__T"filename", Api_PathFindFileName(filename));
        seclist.GetSection(__T"observers/observer",__T"type", x3::a2w(obtype).c_str());
    }
#endif	
}

void Cx_PluginLoader::FireFirstEvent(const tchar_* obtype)
{
#if 0
    Cx_Interface<Ix_ConfigXml> pIFFile(m_cache);

    if (pIFFile)
    {
        Cx_ConfigSection seclist(pIFFile->GetData()->GetSection(NULL,
           __T"observers/observer",__T"type", x3::a2w(obtype).c_str(), false));

        for (int32_ i = 0; i < 999; i++)
        {
            Cx_ConfigSection sec(seclist.GetSectionByIndex(__T"plugin", i));
            tstring shortflname(sec->GetString(__T"filename"));

            if (shortflname.empty())
            {
                break;
            }

            int32_ moduleIndex = GetPluginIndex(shortflname.c_str());

            if (moduleIndex >= 0 && !m_modules[moduleIndex]->hdll)
            {
                LoadDelayPlugin(m_modules[moduleIndex]->filename);
            }
        }
    }
#endif	
}

LWDP_NAMESPACE_END;
