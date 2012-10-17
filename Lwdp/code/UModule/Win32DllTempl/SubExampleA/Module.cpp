 
#define LWDP_MODULE_IMPL
#include <Lwdp.h>
#include <PluginInc.h>

#include "Cx_ExampleA.h"
  

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;


XBEGIN_DEFINE_CLASS()
    XDEFINE_CLASSMAP_ENTRY(MODID_ExampleTestA, CLSID_ExampleA, Cx_ExampleA)
XEND_DEFINE_CLASS_SYS()



DEF_MODULE_INFO_BEGIN(ExampleTestA, LWDP_TOP_DOMAIN __T("::ExampleTest"), PlugLevel::LEVEL1, 0);
	DEC_INIT_FUN();
	DEC_UNINIT_FUN();
DEF_MODULE_INFO_END(ExampleTestA);




// Optional function to initialize this plugin when loaded by the plugin manager.
DEF_INIT_FUN(ExampleTestA)
{
	printf("ExampleTestA InitializePlugin\n");
    return LWDP_OK;
}

// Optional function to free this plugin when unloaded by the plugin manager.
DEF_UNINIT_FUN(ExampleTestA)
{
	printf("ExampleTestA UninitializePlugin\n");
    return LWDP_OK;
}



EXTERN_C_END;
LWDP_NAMESPACE_END;
