 
#define LWDP_MODULE_IMPL
#include <Lwdp.h>
#include <PluginInc.h>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include "Cx_ScriptMgr.h"

//#include <../UModule/Interface/Example/Ix_Example.h>
#include "ScriptAction.h"

LWDP_NAMESPACE_BEGIN;
//EXTERN_C_BEGIN;


XBEGIN_DEFINE_CLASS()
	XDEFINE_CLASSMAP_ENTRY_Singleton(MODID_ScriptMgr, CLSID_ScriptMgr, Cx_ScriptMgr)
XEND_DEFINE_CLASS_SYS();


DEF_MODULE_INFO_BEGIN(ScriptMgr, LWDP_TOP_DOMAIN, (PlugLevel::LEVEL0), 0);
	DEC_INIT_FUN();
	DEC_UNINIT_FUN();
DEF_MODULE_INFO_END(ScriptMgr);


// Optional function to initialize this plugin when loaded by the plugin manager.
DEF_INIT_FUN(ScriptMgr)
{
	printf("ScriptMgr InitializePlugin\n");
	GET_OBJECT_RET(ScriptMgr, iScript, LWDP_GET_OBJECT_ERROR);
	iScript->RegisteAction(new APrint);

    return LWDP_OK;
}

// Optional function to free this plugin when unloaded by the plugin manager.
DEF_UNINIT_FUN(ScriptMgr)
{
	printf("ScriptMgr UninitializePlugin\n");
    return LWDP_OK;
}

//EXTERN_C_END;
LWDP_NAMESPACE_END;


