/*! \file EventMgrModule.cpp
 *  \brief EventMgr Module 
 *  \author Guolisen, LwDp
 *  \date   2012.10.17
 */
 
#define LWDP_MODULE_IMPL
#include <LwDp.h>
#include <PluginInc.h>
#include <iostream>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
//#include <Interface/ScriptMgr/Ix_ScriptMgr.h>
#include "Cx_EventMgr.h"

LWDP_NAMESPACE_BEGIN;

XBEGIN_DEFINE_CLASS()
    XDEFINE_CLASSMAP_ENTRY(MODID_EventMgr, CLSID_EventMgr, Cx_EventMgr)
XEND_DEFINE_CLASS_SYS();


DEF_MODULE_INFO_BEGIN(EventMgr, LWDP_TOP_DOMAIN, PlugLevel::LEVEL0, 0);
	DEC_INIT_FUN();
	DEC_UNINIT_FUN();
DEF_MODULE_INFO_END(EventMgr);


// Optional function to initialize this plugin when loaded by the plugin manager.
DEF_INIT_FUN(EventMgr)
{
	printf("EventMgr InitializePlugin\n");
	//Cx_Interface<Ix_ScriptMgr> iScript(CLSID_ScriptMgr);
	//iScript->RegisteAction(new APrint);

	//GET_OBJECT_RET(EventMgr, iEventMgr, LWDP_GET_OBJECT_ERROR);
	//RINOK(iEventMgr->Init());
	
    return LWDP_OK;
}

// Optional function to free this plugin when unloaded by the plugin manager.
DEF_UNINIT_FUN(EventMgr)
{
	printf("EventMgr UninitializePlugin\n");
    return LWDP_OK;
}

LWDP_NAMESPACE_END;

