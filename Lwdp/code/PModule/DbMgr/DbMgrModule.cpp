/*! \file DbMgrModule.cpp
 *  \brief DbMgr Module 
 *  \author Guolisen, LwDp
 *  \date   2012.10.17
 */
 
#define LWDP_MODULE_IMPL
#include <LwDp.h>
#include <PluginInc.h>
#include <iostream>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
//#include <Interface/ScriptMgr/Ix_ScriptMgr.h>
#include "Cx_DbMgr.h"

LWDP_NAMESPACE_BEGIN;

XBEGIN_DEFINE_CLASS()
    XDEFINE_CLASSMAP_ENTRY_Singleton(MODID_DbMgr, CLSID_DbMgr, Cx_DbMgr)
XEND_DEFINE_CLASS_SYS();


DEF_MODULE_INFO_BEGIN(DbMgr, LWDP_TOP_DOMAIN, PlugLevel::LEVEL0, 0);
	DEC_INIT_FUN();
	DEC_UNINIT_FUN();
DEF_MODULE_INFO_END(DbMgr);


// Optional function to initialize this plugin when loaded by the plugin manager.
DEF_INIT_FUN(DbMgr)
{
	printf("DbMgr InitializePlugin\n");
	//Cx_Interface<Ix_ScriptMgr> iScript(CLSID_ScriptMgr);
	//iScript->RegisteAction(new APrint);
	
    return LWDP_OK;
}

// Optional function to free this plugin when unloaded by the plugin manager.
DEF_UNINIT_FUN(DbMgr)
{
	printf("DbMgr UninitializePlugin\n");
    return LWDP_OK;
}

LWDP_NAMESPACE_END;

