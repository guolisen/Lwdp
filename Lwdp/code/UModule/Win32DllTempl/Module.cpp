 
#define LWDP_MODULE_IMPL
#include <Lwdp.h>
#include <PluginInc.h>

#include "Cx_Example.h"
  

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;


XBEGIN_DEFINE_CLASS()
    XDEFINE_CLASSMAP_ENTRY(MODID_ExampleTest, CLSID_Example, Cx_Example)
XEND_DEFINE_CLASS_SYS()



DEF_MODULE_INFO_BEGIN(ExampleTest, LWDP_TOP_DOMAIN, PlugLevel::LEVEL1, 0);
	DEC_INIT_FUN();
	DEC_UNINIT_FUN();
DEF_MODULE_INFO_END(ExampleTest);




// Optional function to initialize this plugin when loaded by the plugin manager.
DEF_INIT_FUN(ExampleTest)
{
	printf("ExampleTest InitializePlugin\n");
    return LWDP_OK;
}

// Optional function to free this plugin when unloaded by the plugin manager.
DEF_UNINIT_FUN(ExampleTest)
{
	printf("ExampleTest UninitializePlugin\n");
    return LWDP_OK;
}



EXTERN_C_END;
LWDP_NAMESPACE_END;
