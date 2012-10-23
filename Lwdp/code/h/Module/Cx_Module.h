/*! \file Cx_Module.h
 *  \brief Define the implement class of plugin module. (for internal use only)
 *  \author Zhang Yun Gui, X3 C++ PluginFramework
 *  \date   2010.10.19
 */
#ifndef X3_PLUGINIMPL_MODULE_H_
#define X3_PLUGINIMPL_MODULE_H_

//#include "Ix_Module.h"

/*! \ingroup _GROUP_PLUGIN_CORE2_
 *  \brief the implement class of plugin module.
 *  \note  Each plugin has a object of this class. The object is defined in XModuleImpl.h .
 *  \internal
 */

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;

extern bool LwInitializePlugin();
extern void LwUninitializePlugin();


class Cx_Module : public Ix_Module
{
public:	
    Cx_Module() : m_classEntry(NULL), m_moduleInfo(NULL),m_pFactory(NULL), 
                      m_hModule(NULL){};
    virtual ~Cx_Module(){};

    //! Initialize this plugin. Used by x3GetModuleInterface().
    virtual void Initialize(Ix_ObjectFactory* pFactory, MODULEID hModule);
	virtual LWRESULT InitializePlugin() { return LWDP_OK;};
	virtual LWRESULT UninitializePlugin() { return LWDP_OK;};

public:
    virtual void 	ClearModuleItems();
    virtual long_ 	GetUnfreeObjectCount();
	virtual X3MODULEINFO* GetModuleInfo() const;
	virtual const tchar_* GetModuleName() const;
	virtual MODID&        GetModuleMODID() const;
	virtual LWRESULT GetModuleType(TPLUGIN_TYPE& module_type) const;
	virtual LWRESULT GetModuleDomain(TPLUGIN_CODE_DOMAIN* module_domain) const;
	virtual LWRESULT GetModuleLevel(TPLUGIN_LEVEL& module_level) const;
	
    virtual Ix_ObjectFactory* GetObjectFactory() const;
    virtual MODULEID GetModuleInstance() const;
	virtual int32_   GetClassEntryTable(int32_* pBuildInfo, uint32_* pEntrySize,
                                     X3CLASSENTRY* pTable, int32_ nMaxCount);

	virtual long_ 	GetClassCount(byte_ minType);
	virtual bool 	CanUnloadPlugin();
	virtual int32_  InternalCreateObject(const tchar_* clsid, X3IID iid, 
                                Ix_Object** ppv, const MODID& fromModule);

protected:
//	virtual LWRESULT InternalInitialize() = 0;
		
protected:
    Ix_ObjectFactory*   m_pFactory;
    MODULEID         m_hModule;
	X3MODULEINFO*    m_moduleInfo;
	X3CLASSENTRY*    m_classEntry;

private:
	Cx_Module(const Cx_Module&);
    void operator=(const Cx_Module&);
	
};



EXTERN_C_END;
LWDP_NAMESPACE_END;

#endif // X3_PLUGINIMPL_MODULE_H_
