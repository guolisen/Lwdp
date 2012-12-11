 
#define LWDP_MODULE_IMPL
#include <Lwdp.h>
#include <PluginInc.h>

#include "../Interface/TSFrontend/Ix_TSFrontend.h"
#include "Cx_TSFrontend.h"
  

LWDP_NAMESPACE_BEGIN;



XBEGIN_DEFINE_CLASS()
    XDEFINE_CLASSMAP_ENTRY_Singleton(MODID_TSFrontend, CLSID_TSFrontend, Cx_TSFrontend)
XEND_DEFINE_CLASS_SYS()



DEF_MODULE_INFO_BEGIN(TSFrontend, LWDP_TOP_DOMAIN, PlugLevel::LEVEL1, 0);
	DEC_INIT_FUN();
	DEC_UNINIT_FUN();
DEF_MODULE_INFO_END(TSFrontend);




// Optional function to initialize this plugin when loaded by the plugin manager.
DEF_INIT_FUN(TSFrontend)
{
	printf("TSFrontend InitializePlugin\n");
    return LWDP_OK;
}

// Optional function to free this plugin when unloaded by the plugin manager.
DEF_UNINIT_FUN(TSFrontend)
{
	printf("TSFrontend UninitializePlugin\n");
    return LWDP_OK;
}




LWDP_NAMESPACE_END;
