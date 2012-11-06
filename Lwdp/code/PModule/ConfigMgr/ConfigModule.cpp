 
#define LWDP_MODULE_IMPL
#include <LwDp.h>
#include <PluginInc.h>

#include "Cx_ConfigMgr.h"
#include "Cx_XmlMgr.h"

LWDP_NAMESPACE_BEGIN;


XBEGIN_DEFINE_CLASS()
    XDEFINE_CLASSMAP_ENTRY_Singleton(MODID_ConfigMgr, CLSID_ConfigMgr, Cx_ConfigMgr)
	XDEFINE_CLASSMAP_ENTRY(MODID_ConfigMgr, CLSID_XmlMgr, Cx_XmlMgr)
XEND_DEFINE_CLASS_SYS();


DEF_MODULE_INFO_BEGIN(ConfigMgr, LWDP_TOP_DOMAIN, PlugLevel::LEVEL0, 0);
	DEC_INIT_FUN();
	DEC_UNINIT_FUN();
DEF_MODULE_INFO_END(ConfigMgr);



// Optional function to initialize this plugin when loaded by the plugin manager.
DEF_INIT_FUN(ConfigMgr)
{
	printf("ConfigMgr InitializePlugin\n");
	
    return LWDP_OK;
}

// Optional function to free this plugin when unloaded by the plugin manager.
DEF_UNINIT_FUN(ConfigMgr)
{
	printf("ConfigMgr UninitializePlugin\n");
    return LWDP_OK;
}



LWDP_NAMESPACE_END;

