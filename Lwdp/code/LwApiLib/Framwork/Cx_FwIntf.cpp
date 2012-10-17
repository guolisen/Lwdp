// Copyright (C), 2006гн2011, Potevio Institute of Technology.
/************************************************************
*  File name:   Cx_FwIntf.cpp  	
*  Author:      guolisen
*  Version:   	v1.0
*  Create Date: 2011-08-11
***********************************************************/
// 2011.08.11: Create By Guolisen
#include <Lwdp.h>
#include <PluginInc.h>

//#include <LwApiLib/Framwork/PluginMgr/PluginManager.h>
#include <LwApiLib/Framwork/FwEntry/FrameworkEntry.h>
#include <Interface/FwEntry/Ix_FwIntf.h>
#include "Cx_FwIntf.h"


LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;


LWRESULT Cx_FwIntf::Api_Fw_Start()
{
	return Fw_Start();
}

LWRESULT Cx_FwIntf::Api_Fw_Stop()
{
	return Fw_Stop();
}

LWRESULT Cx_FwIntf::Api_Fw_RegisteModule(Ix_Module* plugin_module)
{
	return Fw_RegisteModule(plugin_module);
}

LWRESULT Cx_FwIntf::Api_Fw_LoadModule(const tchar_* file_name, const Ix_ObjectFactory* plugin_factory, MODID& modid, Ix_Module** pModuleObj)
{
	return Fw_LoadModule(file_name, plugin_factory, modid, pModuleObj);
}

LWRESULT Cx_FwIntf::Api_Fw_UnLoadModule(const MODID& modid)
{
	return Fw_UnLoadModule(modid);
}

const tchar_* Cx_FwIntf::Api_Fw_GetModuleName(const MODID& modid)
{
	return Fw_GetModuleName(modid);
}

Ix_Module* Cx_FwIntf::Api_Fw_GetModulePtr(const MODID& modid)
{
	return Fw_GetModulePtr(modid);
}

EXTERN_C_END;
LWDP_NAMESPACE_END;


