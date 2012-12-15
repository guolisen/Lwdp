/*! \file Cx_TimerMgr.cpp
 *  \brief TimerMgr Module Impl
 *  \author Guolisen, LwDp
 *  \date   2012.10.17
 */
 
#include <LwDp.h>
#include <PluginInc.h>
#include <iostream>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>

#include "TimerMgrDef.h"
#include "Cx_TimerMgr.h"



LWDP_NAMESPACE_BEGIN;


Cx_TimerMgr::Cx_TimerMgr()
{
}

Cx_TimerMgr::~Cx_TimerMgr()
{
}

LWRESULT Cx_TimerMgr::Init()
{
	lw_log_info(LWDP_LUA_LOG, __T("Cx_TimerMgr::Init OK!"));



	return LWDP_OK;
}


LWDP_NAMESPACE_END;

