/*! \file Cx_CommonUtilMgr.cpp
 *  \brief CommonUtilMgr Module Impl
 *  \author Guolisen, LwDp
 *  \date   2012.10.17
 */
 
#include <LwDp.h>
#include <PluginInc.h>
#include <iostream>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>

#include "CommonUtilMgrDef.h"
#include "Cx_CommonUtilMgr.h"



LWDP_NAMESPACE_BEGIN;


Cx_CommonUtilMgr::Cx_CommonUtilMgr()
{
}

Cx_CommonUtilMgr::~Cx_CommonUtilMgr()
{
}

LWRESULT Cx_CommonUtilMgr::Init()
{
	lw_log_info(LWDP_LUA_LOG, __T("Cx_CommonUtilMgr::Init OK!"));

	srand(time(NULL));

	return LWDP_OK;
}


LWDP_NAMESPACE_END;

