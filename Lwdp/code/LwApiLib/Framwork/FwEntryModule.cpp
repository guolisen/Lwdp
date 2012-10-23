 
#define LWDP_MODULE_IMPL
#include <LwDp.h>
#include <PluginInc.h>

#include "Cx_FwIntf.h"
#include <LwApiLib/Framwork/FwEntry/FrameworkEntry.h>

LWDP_NAMESPACE_BEGIN;
//EXTERN_C_BEGIN;


XBEGIN_DEFINE_CLASS()
    XDEFINE_CLASSMAP_ENTRY_Singleton(MODID_FwIntf, CLSID_FwIntf, Cx_FwIntf)
XEND_DEFINE_CLASS_SYS();



static X3MODULEINFO s_module_local = {__T("FwIntf"), LWDP_TOP_DOMAIN, PlugLevel::LEVEL0, iPLUGIN_CONTEXT, 
									  iPLUGIN_VERSION, UNICODE_TAG, (MODULEID)-1, MODID_FwIntf}; 
class INTER_MOD_NAME(FwIntf) : public Cx_Module
{ 
public: 	
    INTER_MOD_NAME(FwIntf)() 
    { 
        m_moduleInfo = &s_module_local; 
        m_classEntry = s_classes_local; 
    };
	
	DEC_INIT_FUN();
	DEC_UNINIT_FUN();
}; 

EXTERN_C_BEGIN;
LWRESULT RegisteFwModule(Ix_Module** pModule)
{
	s_lwdpModule = new INTER_MOD_NAME(FwIntf); 
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_MALLOC_MEMORY_ERROR, s_lwdpModule, "RegisteFwModule new Module Error!");
	*pModule = s_lwdpModule;
	return Fw_RegisteModule(s_lwdpModule); 
}
EXTERN_C_END;

// Optional function to initialize this plugin when loaded by the plugin manager.
DEF_INIT_FUN(FwIntf)
{
	printf("FwIntf InitializePlugin\n");
    return LWDP_OK;
}

// Optional function to free this plugin when unloaded by the plugin manager.
DEF_UNINIT_FUN(FwIntf)
{
	printf("FwIntf UninitializePlugin\n");
    return LWDP_OK;
}



//EXTERN_C_END;
LWDP_NAMESPACE_END;

