/*! \file Cx_PerfMgr.cpp
 *  \brief PerfMgr Module Impl
 *  \author Guolisen, LwDp
 *  \date   2012.10.17
 */
 
#include <LwDp.h>
#include <PluginInc.h>
#include <iostream>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>

#include "PerfMgrDef.h"
#include "Cx_PerfMgr.h"



LWDP_NAMESPACE_BEGIN;

uint32_ gPerfSwitch = 1;


Cx_PerfMgr::Cx_PerfMgr()
{
}

Cx_PerfMgr::~Cx_PerfMgr()
{
}

LWRESULT Cx_PerfMgr::Init()
{
	lw_log_info(LWDP_LUA_LOG, __T("Cx_PerfMgr::Init OK!"));
	GET_OBJECT_RET(ConfigMgr, iConfigMgr, LWDP_GET_OBJECT_ERROR);

	XPropertys propSwitch;
	iConfigMgr->GetModulePropEntry(LW_PERFMGR_MODULE_NAME, LW_PERFMGR_SWITCH_NAME, propSwitch);
	if(!propSwitch[0].propertyText.empty())
	{
		gPerfSwitch = atol(propSwitch[0].propertyText.c_str());
	}
	else
	{
		LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <PerfSwitch> In Config File, Default(%d)", gPerfSwitch);
	}

	return LWDP_OK;
}


LWDP_NAMESPACE_END;

