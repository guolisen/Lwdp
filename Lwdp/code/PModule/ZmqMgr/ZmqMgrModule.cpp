/*! \file ZmqMgrModule.cpp
 *  \brief ZmqMgr Module 
 *  \author Guolisen, LwDp
 *  \date   2012.12.03
 */


 
#define LWDP_MODULE_IMPL

#include <LwDp.h>
#include <PluginInc.h>
#include <iostream>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
//#include <Interface/ScriptMgr/Ix_ScriptMgr.h>
#include "Cx_ZmqMgr.h"

LWDP_NAMESPACE_BEGIN;

XBEGIN_DEFINE_CLASS()
	XDEFINE_CLASSMAP_ENTRY_Singleton(MODID_ZmqMgr, CLSID_ZmqMgr, Cx_ZmqMgr)
    XDEFINE_CLASSMAP_ENTRY(MODID_ZmqMgr, CLSID_ZMessage, Cx_ZMessage)
XEND_DEFINE_CLASS_SYS();


DEF_MODULE_INFO_BEGIN(ZmqMgr, LWDP_TOP_DOMAIN, PlugLevel::LEVEL0, 0);
	DEC_INIT_FUN();
	DEC_UNINIT_FUN();
DEF_MODULE_INFO_END(ZmqMgr);


// Optional function to initialize this plugin when loaded by the plugin manager.
DEF_INIT_FUN(ZmqMgr)
{
	printf("ZmqMgr InitializePlugin\n");
	//Cx_Interface<Ix_ScriptMgr> iScript(CLSID_ScriptMgr);
	//iScript->RegisteAction(new APrint);

	//GET_OBJECT_RET(ZmqMgr, iZmqMgr, LWDP_GET_OBJECT_ERROR);
	//RINOK(iZmqMgr->Init());
	
    return LWDP_OK;
}

// Optional function to free this plugin when unloaded by the plugin manager.
DEF_UNINIT_FUN(ZmqMgr)
{
	printf("ZmqMgr UninitializePlugin\n");
    return LWDP_OK;
}

LWDP_NAMESPACE_END;

