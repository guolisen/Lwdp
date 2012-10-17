/*! \file PluginManager.h
 *  \brief Define plugin manager wrapper class
 *  \author Zhang Yun Gui, X3 C++ PluginFramework
 *  \date   2011.06.30
 */
#ifndef X3_MANAGER_PLUGINMANAGER_H_
#define X3_MANAGER_PLUGINMANAGER_H_

#include <Interface/PluginLoader/Ix_ObjectFactory.h>
#include <Interface/PluginLoader/Ix_PluginLoader.h>

LWDP_NAMESPACE_BEGIN;
/*! \ingroup _GROUP_PLUGIN_CORE_
 *  \brief Plugin manager wrapper class
 *  \see   Ix_ObjectFactory, Ix_PluginLoader
 */
class CPluginManager
{
public:
    CPluginManager(){}
    ~CPluginManager()
    {
        Unload(true);
    }

    //! Unload plugin manager and plugins
    void Unload(bool force_unload = false);

    //! Load plugin manager.
    /*!
        \param subdir PluginManagerX3's path, absolute path or relative to instance.
        \param instance used if subdir is a relative path.
        \return true if plugin manager is loaded.
    */
    LWRESULT LoadPluginManager(const tchar_* subdir);

    //! Load plugin manager and core plugins.
    /*!
        \param subdir plugin path, absolute path or relative to instance.
        \param instance used if subdir is a relative path.
        \return true if any plugin is loaded.
    */
    LWRESULT LoadCorePlugins(const tchar_* subdir);

    //! Return the object creator.
    Ix_ObjectFactory* GetObjectFactory();

    //! Return the object creator.
	LWRESULT LoadObjectFactory();

	//! Return the plugin loading object.
	Ix_PluginLoader* GetPluginLoader();

    //!
    static MODID& PluginMgrModid()
    {
        static MODID pluginModid;
        return pluginModid;
    }

    static Ix_ObjectFactory*& Factory()
    {
        static Ix_ObjectFactory* s_factory = NULL;
        return s_factory;
    }

	static MODULEID& Handle()
    {
        static MODULEID s_moduleid = NULL;
        return s_moduleid;
    }


private:
    CPluginManager(const CPluginManager&);
    void operator=(const CPluginManager&);

};

LWDP_NAMESPACE_END;

#endif // X3_MANAGER_PLUGINMANAGER_H_
