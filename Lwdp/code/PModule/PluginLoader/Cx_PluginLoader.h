// Copyright 2008-2011 Zhang Yun Gui, rhcad@hotmail.com
// http://sourceforge.net/projects/x3c/

// author: Guolisen

#ifndef _X3_CORE_PLUGINLOADER_H
#define _X3_CORE_PLUGINLOADER_H

#include "Cx_ObjectFactory.h"
#include <Interface/PluginLoader/Ix_PluginLoader.h>
#include <Interface/PluginLoader/Ix_PluginLoader2.h>
#include <Interface/PluginLoader/Ix_PluginDelayLoad.h>

LWDP_NAMESPACE_BEGIN;

class Cx_PluginLoader
    : public Cx_ObjectFactory
    , public Ix_PluginLoader
    , public Ix_PluginLoader2
    , public Ix_PluginDelayLoad
{
    X3BEGIN_CLASS_DECLARE(Cx_PluginLoader)
        X3DEFINE_INTERFACE_ENTRY(Ix_PluginLoader)
        X3DEFINE_INTERFACE_ENTRY(Ix_PluginLoader2)
        X3DEFINE_INTERFACE_ENTRY(Ix_PluginDelayLoad)
        X3USE_INTERFACE_ENTRY(Cx_ObjectFactory)
    X3END_CLASS_DECLARE();
public:
    Cx_PluginLoader();
    virtual ~Cx_PluginLoader();

    MODULEID GetMainModuleHandle();

    // From Ix_PluginLoader
    //
    virtual LWRESULT LoadPluginFile(const tchar_* files);

    virtual LWRESULT InitializePlugins();

	virtual LWRESULT CheckPlugin(const Ix_Module* pModule);

    virtual LWRESULT RegisterPlugin(const Ix_Module* pModule);

    virtual LWRESULT RegisterFrameWork(Ix_Module* pModule);

    virtual LWRESULT LoadPlugin(const MODID& modid);

    virtual LWRESULT UnloadPlugin(const MODID& modid, bool force_unload = false);

    virtual LWRESULT UnloadPluginAll(bool force_unload = false);
	
    // From Ix_PluginLoader2
    //
    virtual long_ GetPluginCount();
    virtual bool GetPluginFileName(long_ index, MODULEID& hdll, tstring& filename);

    // From Ix_PluginDelayLoad
    //
    virtual void AddObserverPlugin(MODULEID hdll, const tchar_* obtype);
    virtual void FireFirstEvent(const tchar_* obtype);

private:
    Cx_PluginLoader(const Cx_PluginLoader&);
    void operator=(const Cx_PluginLoader&);

    bool ClearModuleItems(const MODID& modid);

    //long_  InLoadPlugins(const std::vector<MODID>& modids);
    int32_ GetPluginIndex(const MODID& modid);
    virtual bool LoadDelayPlugin(const tchar_* filename);
    bool LoadPluginOrDelay(const tchar_* pluginFile);
    bool BuildPluginCache(const tchar_* pluginFile);
    bool LoadPluginCache(const tchar_* pluginFile);
    bool LoadCacheFile(const tchar_* pluginFile);
    bool LoadClsids(CLSIDS& clsids, const tchar_* pluginFile);
    bool SaveClsids(const CLSIDS& clsids, const tchar_* pluginFile);
    bool SaveClsids();

private:
    MODULEID	m_instance;
    tchar_		m_clsfile[LWDP_MAX_PATH];
//    Cx_Ptr		m_cache;
};

LWDP_NAMESPACE_END;
#endif // _X3_CORE_PLUGINLOADER_H
