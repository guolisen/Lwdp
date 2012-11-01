#define LWDP_MODULE_IMPL
#include <LwDp.h>
#include <PluginInc.h>
#include <iostream>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/ScriptMgr/Ix_ScriptMgr.h>
#include "Cx_DctMgr.h"

LWDP_NAMESPACE_BEGIN;

XBEGIN_DEFINE_CLASS()
    XDEFINE_CLASSMAP_ENTRY_Singleton(MODID_DctMgr, CLSID_DctMgr, Cx_DctMgr)
XEND_DEFINE_CLASS_SYS();


DEF_MODULE_INFO_BEGIN(DctMgr, LWDP_TOP_DOMAIN, PlugLevel::LEVEL0, 0);
	DEC_INIT_FUN();
	DEC_UNINIT_FUN();
DEF_MODULE_INFO_END(DctMgr);


// Optional function to initialize this plugin when loaded by the plugin manager.
DEF_INIT_FUN(DctMgr)
{
	printf("DctMgr InitializePlugin\n");
	//Cx_Interface<Ix_ScriptMgr> iScript(CLSID_ScriptMgr);
	//iScript->RegisteAction(new APrint);

	GET_OBJECT_RET(DctMgr, iDctMgr, LWDP_GET_OBJECT_ERROR);
	RINOK(iDctMgr->Init());
	
    return LWDP_OK;
}

// Optional function to free this plugin when unloaded by the plugin manager.
DEF_UNINIT_FUN(DctMgr)
{
	printf("DctMgr UninitializePlugin\n");
    return LWDP_OK;
}

LWDP_NAMESPACE_END;

