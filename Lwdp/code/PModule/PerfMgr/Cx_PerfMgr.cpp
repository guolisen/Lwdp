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


Cx_PerfMgr::Cx_PerfMgr()
{
}

Cx_PerfMgr::~Cx_PerfMgr()
{
}

LWRESULT Cx_PerfMgr::Init()
{
	lw_log_info(LWDP_LUA_LOG, __T("Cx_PerfMgr::Init OK!"));



	return LWDP_OK;
}


LWDP_NAMESPACE_END;

