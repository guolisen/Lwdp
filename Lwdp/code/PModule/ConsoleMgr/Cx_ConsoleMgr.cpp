/*! \file Cx_ConsoleMgr.cpp
 *  \brief ConsoleMgr Module Impl
 *  \author Guolisen, LwDp
 *  \date   2012.10.17
 */
 
#include <LwDp.h>
#include <PluginInc.h>
#include <iostream>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>

#include "ConsoleMgrDef.h"
#include "Cx_ConsoleMgr.h"



LWDP_NAMESPACE_BEGIN;


Cx_ConsoleMgr::Cx_ConsoleMgr()
{
}

Cx_ConsoleMgr::~Cx_ConsoleMgr()
{
}

LWRESULT Cx_ConsoleMgr::Init()
{
	lw_log_info(LWDP_LUA_LOG, __T("Cx_ConsoleMgr::Init OK!"));



	return LWDP_OK;
}


LWDP_NAMESPACE_END;

