/*! \file Ix_PluginLoader.h
 *  \brief Define plugin loading interface: Ix_PluginLoader
 *  \author Zhang Yun Gui, X3 C++ PluginFramework
 *  \date   2011.06.30
 */
#ifndef X3_MANAGER_IPLUGINLOADER_H_
#define X3_MANAGER_IPLUGINLOADER_H_

#include <Interface/Ix_Object.h>
#include "Id_PluginLoader.h"
 
LWDP_NAMESPACE_BEGIN;

/*! \ingroup _GROUP_PLUGIN_CORE_
 *  \interface Ix_PluginLoader
 *  \brief Plugin loading interface
 *  \see   Ix_ObjectFactory
 */
class Ix_Module;
class Ix_PluginLoader : public Ix_Object
{
public:
    DEFINE_IID(Ix_PluginLoader);

    //! Load the specified plugins in the specified path.
    /*!
        \param path relative path or empty, such as 'Plugins'.
        \param files include filename of plugins, separated by spaces or comma, such as '1.dll, 2.dll'.
        \param instance used as relative base.
        \return count of plugins loaded.
        \note need to call InitializePlugins() after.
        \see InitializePlugins, LoadPlugins
    */
    virtual LWRESULT LoadPluginFile(const tchar_* files) = 0;

    //! Initialize all plugins after they are loaded.
    /*! Initialize a plugin by calling it's x3InitializePlugin() export function.\n
        Automatic skip the plugin which has initialized or has not x3InitializePlugin.
        \return count of plugins which has initialized (or has not x3InitializePlugin) this time.
        \see optional export function: OUTAPI bool x3InitializePlugin()
    */
    virtual LWRESULT InitializePlugins() = 0;

    //! Register a plugin (not load dll).
    /*! It can used to static-link dll or main appliction module.
        \param instance the module to register.
        \return if register success.
        \note need to call InitializePlugins() after.
        \see InitializePlugins
    */
    virtual LWRESULT RegisterPlugin(const Ix_Module* pModule) = 0;


    virtual LWRESULT RegisterFrameWork(Ix_Module* pModule) = 0;

    //! Load a plugin.
    /*! Use LoadLibraryEx() and RegisterPlugin() to load.
        \param filename absolute file name or relative to the application.
        \return if load success.
        \note need to call InitializePlugins() after.
        \see InitializePlugins
    */
    virtual LWRESULT LoadPlugin(const MODID& modid) = 0;

    //! Unload a plugin.
    /*! Automatic call plugin's x3CanUnloadPlugin() and x3UninitializePlugin() export functions.
        \param name plugin filename regardless folder, such as 'MyData.plugin.dll'.
        \return if unload success.
        \see optional export function: OUTAPI void x3UninitializePlugin()
    */
    virtual LWRESULT UnloadPlugin(const MODID& modid, bool force_unload = false) = 0;

    virtual LWRESULT UnloadPluginAll(bool force_unload = false) = 0;

};

LWDP_NAMESPACE_END;
#endif // X3_MANAGER_IPLUGINLOADER_H_
