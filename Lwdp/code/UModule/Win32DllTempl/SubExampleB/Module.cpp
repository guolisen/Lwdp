 
#define LWDP_MODULE_IMPL
#include <Lwdp.h>
#include <PluginInc.h>

#include "Cx_ExampleB.h"
  

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;


XBEGIN_DEFINE_CLASS()
    XDEFINE_CLASSMAP_ENTRY(MODID_ExampleTestB, CLSID_ExampleB, Cx_ExampleB)
XEND_DEFINE_CLASS_SYS()



DEF_MODULE_INFO_BEGIN(ExampleTestB, LWDP_TOP_DOMAIN __T("::ExampleTest"), PlugLevel::LEVEL1, 0);
	DEC_INIT_FUN();
	DEC_UNINIT_FUN();
DEF_MODULE_INFO_END(ExampleTestB);




// Optional function to initialize this plugin when loaded by the plugin manager.
DEF_INIT_FUN(ExampleTestB)
{
	printf("ExampleTestB InitializePlugin\n");
    return LWDP_OK;
}

// Optional function to free this plugin when unloaded by the plugin manager.
DEF_UNINIT_FUN(ExampleTestB)
{
	printf("ExampleTestB UninitializePlugin\n");
    return LWDP_OK;
}

EXTERN_C_END;
LWDP_NAMESPACE_END;
