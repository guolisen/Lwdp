/*! \file Ix_Module.h
 *  \brief Define the plugin module interface: Ix_Module
 *  \author Zhang Yun Gui, X3 C++ PluginFramework
 *  \date   2010.10.19
 */
#ifndef X3_PLUGINIMPL_IMODULE_H_
#define X3_PLUGINIMPL_IMODULE_H_

#include "Ix_Object.h"

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;

class Ix_ObjectFactory;

/*! \ingroup _GROUP_PLUGIN_CORE_
 *  \interface Ix_Module
 *  \brief the plugin module interface.
 *  \note  Each plugin (DLL) has a Ix_Module object which can get from x3GetCurrentModule().
 *  \see x3GetCurrentModule, x3IsCreatorRegister, Ix_ObjectFactory
 */
class Ix_Module 
{
public:
	DEFINE_IID(Ix_Module);

    virtual void Initialize(Ix_ObjectFactory* pFactory, MODULEID hModule) = 0;

	virtual LWRESULT InitializePlugin() = 0;

	virtual LWRESULT UninitializePlugin() = 0;

    //! Free all single instance objects when this plugin is unloading.
    virtual void ClearModuleItems() = 0;

    //! Return count of unfree objects.
    virtual long_ GetUnfreeObjectCount() = 0;

    //! Return the plugin manager object.
    virtual Ix_ObjectFactory* GetObjectFactory() const = 0;

	//!
	virtual const tchar_*  GetModuleName() const  = 0;
    //!
    virtual MODID&   GetModuleMODID() const = 0;
	 //!
	virtual LWRESULT GetModuleType(TPLUGIN_TYPE& module_type) const = 0;
	 //!
	virtual LWRESULT GetModuleDomain(TPLUGIN_CODE_DOMAIN* module_domain) const = 0;
	 //!
	virtual LWRESULT GetModuleLevel(TPLUGIN_LEVEL& module_level) const = 0;

    virtual int32_	 GetClassEntryTable(int32_* pBuildInfo, uint32_* pEntrySize,
                                             X3CLASSENTRY* pTable, int32_ nMaxCount) = 0;
    virtual long_	 GetClassCount(byte_ minType) = 0;
    virtual bool	 CanUnloadPlugin() = 0;
    virtual int32_	 InternalCreateObject(const tchar_* clsid, X3IID iid, 
                                             Ix_Object** ppv, const MODID& fromModule) = 0;

	//!
	virtual X3MODULEINFO* GetModuleInfo() const = 0;
	
    //! Return DLL handle of this plugin.
    virtual MODULEID GetModuleInstance() const = 0;

};


EXTERN_C_END;
LWDP_NAMESPACE_END;

#endif // X3_PLUGINIMPL_IMODULE_H_
