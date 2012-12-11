 
#define LWDP_MODULE_IMPL
#include <Lwdp.h>
#include <PluginInc.h>

#include "../Interface/ZmqBackend/Ix_ZmqBackend.h"
#include "Cx_ZmqBackend.h"
  

LWDP_NAMESPACE_BEGIN;



XBEGIN_DEFINE_CLASS()
    XDEFINE_CLASSMAP_ENTRY_Singleton(MODID_ZmqBackend, CLSID_ZmqBackend, Cx_ZmqBackend)
XEND_DEFINE_CLASS_SYS()



DEF_MODULE_INFO_BEGIN(ZmqBackend, LWDP_TOP_DOMAIN, PlugLevel::LEVEL1, 0);
	DEC_INIT_FUN();
	DEC_UNINIT_FUN();
DEF_MODULE_INFO_END(ZmqBackend);




// Optional function to initialize this plugin when loaded by the plugin manager.
DEF_INIT_FUN(ZmqBackend)
{
	printf("ZmqBackend InitializePlugin\n");
    return LWDP_OK;
}

// Optional function to free this plugin when unloaded by the plugin manager.
DEF_UNINIT_FUN(ZmqBackend)
{
	printf("ZmqBackend UninitializePlugin\n");
    return LWDP_OK;
}




LWDP_NAMESPACE_END;
