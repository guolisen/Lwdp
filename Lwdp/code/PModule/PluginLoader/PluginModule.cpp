// Copyright 2008-2011 Guolisen, guolisen@gmail.com
// http://sourceforge.net/projects/x3c/

#define LWDP_MODULE_IMPL
#include <LwDp.h>
#include <PluginInc.h>
#include <Interface/PluginLoader/Ix_PluginModuleMgr.h>
#include "Cx_PluginLoader.h"

LWDP_NAMESPACE_BEGIN;
//EXTERN_C_BEGIN;

class Cx_PluginLoaderOut : public Cx_Object<Cx_PluginLoader>
{
public:
	Cx_PluginLoaderOut(){}
    virtual ~Cx_PluginLoaderOut() {} 
	
	virtual LWRESULT Init(const Ix_Module* pModule)
	{
	    Cx_ObjectFactory::MODULE moduleInfo;
		moduleInfo.modid  = pModule->GetModuleMODID();
	    moduleInfo.module = (Ix_Module *)pModule;
	    moduleInfo.owned  = (pModule->GetModuleInstance())? true: false;
	    moduleInfo.inited = true; //!< 调用邋PluginModule的Init，时已经初始化

        Cx_ObjectFactory::MODULE* module = new Cx_ObjectFactory::MODULE;
        *module = moduleInfo;
        m_modules.push_back(module);
	
		return LWDP_OK;
	};
    virtual Ix_Object*  CreateObject(const LWCLSID& clsid, X3IID iid, Ix_Object** ppv, const MODID& fromModule)
    {
        //if (x3::CLSID_PluginDelayLoad == clsid)
        //{
       //     return QueryInterface(iid, ppv, fromModule) ? 0 : 1;
       // }
        return Cx_PluginLoader::CreateObject(clsid, iid, ppv, fromModule);
    }

private:
    Cx_PluginLoaderOut(const Cx_PluginLoaderOut&);
    void operator=(const Cx_PluginLoaderOut&);
	
};

static Cx_PluginLoaderOut s_loader;


XBEGIN_DEFINE_CLASS()
    XDEFINE_CLASSMAP_ENTRY(MODID_PluginLoader, CLSID_PluginLoader, Cx_PluginLoader)
XEND_DEFINE_CLASS_SYS();


static X3MODULEINFO s_module_local = {__T("PluginLoader"), LWDP_TOP_DOMAIN, PlugLevel::LEVEL0, iPLUGIN_CONTEXT, 
									  iPLUGIN_VERSION, UNICODE_TAG, (MODULEID)-1, MODID_PluginLoader}; 
class INTER_MOD_NAME(PluginLoader) : public Cx_Module, public Ix_PluginMgrModule
{ 
public: 	
    INTER_MOD_NAME(PluginLoader)() 
    { 
        m_moduleInfo = &s_module_local; 
        m_classEntry = s_classes_local; 
    };

	DEC_INIT_FUN();
	DEC_UNINIT_FUN();

	virtual Ix_ObjectFactory* GetRegisterBank()
	{
		s_loader.Init(this);
		return &s_loader;
	};	
}; 

REGISTER_MODULE(PluginLoader);


// Optional function to initialize this plugin when loaded by the plugin manager.
DEF_INIT_FUN(PluginLoader)
{
	printf("PluginLoader InitializePlugin\n");
    return LWDP_OK;
}

// Optional function to free this plugin when unloaded by the plugin manager.
DEF_UNINIT_FUN(PluginLoader)
{
	printf("PluginLoader UninitializePlugin\n");
    return LWDP_OK;
}


//EXTERN_C_END;
LWDP_NAMESPACE_END;


