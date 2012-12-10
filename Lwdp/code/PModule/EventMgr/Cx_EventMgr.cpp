/*! \file Cx_EventMgr.cpp
 *  \brief EventMgr Module Impl
 *  \author Guolisen, LwDp
 *  \date   2012.10.17
 */
 
#include <LwDp.h>
#include <PluginInc.h>
#include <iostream>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>

#include "EventTagDef.h"
#include "Cx_EventMgr.h"



LWDP_NAMESPACE_BEGIN;


Cx_EventMgr::Cx_EventMgr()
{
}

Cx_EventMgr::~Cx_EventMgr()
{
}

LWRESULT Cx_EventMgr::Init()
{
	lw_log_info(LWDP_EVENT_LOG, __T("Cx_EventMgr::Init OK!"));



	return LWDP_OK;
}



LWDP_NAMESPACE_END;

