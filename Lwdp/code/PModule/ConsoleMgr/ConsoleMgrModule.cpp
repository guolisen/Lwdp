/*! \file ConsoleMgrModule.cpp
 *  \brief ConsoleMgr Module 
 *  \author Guolisen, LwDp
 *  \date   2012.10.17
 */
 
#define LWDP_MODULE_IMPL
#include <LwDp.h>
#include <PluginInc.h>
#include <iostream>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
//#include <Interface/ScriptMgr/Ix_ScriptMgr.h>
#include "Cx_ConsoleMgr.h"

LWDP_NAMESPACE_BEGIN;

XBEGIN_DEFINE_CLASS()
    XDEFINE_CLASSMAP_ENTRY_Singleton(MODID_ConsoleMgr, CLSID_ConsoleMgr, Cx_ConsoleMgr)
XEND_DEFINE_CLASS_SYS();


DEF_MODULE_INFO_BEGIN(ConsoleMgr, LWDP_TOP_DOMAIN, PlugLevel::LEVEL0, 0);
	DEC_INIT_FUN();
	DEC_UNINIT_FUN();
DEF_MODULE_INFO_END(ConsoleMgr);


// Optional function to initialize this plugin when loaded by the plugin manager.
DEF_INIT_FUN(ConsoleMgr)
{
	printf("ConsoleMgr InitializePlugin\n");
	//Cx_Interface<Ix_ScriptMgr> iScript(CLSID_ScriptMgr);
	//iScript->RegisteAction(new APrint);
	
    return LWDP_OK;
}

// Optional function to free this plugin when unloaded by the plugin manager.
DEF_UNINIT_FUN(ConsoleMgr)
{
	printf("ConsoleMgr UninitializePlugin\n");
    return LWDP_OK;
}

LWDP_NAMESPACE_END;

