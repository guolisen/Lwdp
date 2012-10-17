/*! \file Ix_FwIntf.h
 *  \brief Define FwEntry Module Interface 
 *  \author Guolisen, Lwdp
 *  \date   2011.11.04
 */
#ifndef IX_FRAMEWORK_INTERFACE_H
#define IX_FRAMEWORK_INTERFACE_H

#include <Interface/Ix_Object.h>
#include "Id_FwIntf.h"

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;

class Ix_Module;
INTERFACE_BEGIN(FwIntf)
	//
	virtual LWRESULT Api_Fw_Start() = 0;
	virtual LWRESULT Api_Fw_Stop() = 0;

	virtual LWRESULT Api_Fw_RegisteModule(Ix_Module* plugin_module) = 0;
	virtual LWRESULT Api_Fw_LoadModule(const tchar_* file_name, const Ix_ObjectFactory* plugin_factory, MODID& modid, Ix_Module** pModuleObj) = 0;
	virtual LWRESULT Api_Fw_UnLoadModule(const MODID& modid) = 0;

	virtual const tchar_* Api_Fw_GetModuleName(const MODID& modid) = 0;
	virtual Ix_Module*    Api_Fw_GetModulePtr(const MODID& modid) = 0;

	//
INTERFACE_END();



EXTERN_C_END;
LWDP_NAMESPACE_END;

#endif // IX_CONFIG_MANAGER_H






