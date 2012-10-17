/*! \file Cx_FwIntf.h
 *  \brief Define Framework Interface 
 *  \author Guolisen, Lwdp
 *  \date   2011.11.04
 */
#ifndef CX_FRAMEWORK_INTERFACE_H
#define CX_FRAMEWORK_INTERFACE_H

#include <Interface/FwEntry/Ix_FwIntf.h>


LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;
class Ix_Module;
class Ix_ObjectFactory;
class Cx_FwIntf : public Ix_FwIntf 
{
    X3BEGIN_CLASS_DECLARE(Cx_FwIntf)
        X3DEFINE_INTERFACE_ENTRY(Ix_FwIntf)
    X3END_CLASS_DECLARE();

protected:
    Cx_FwIntf(){};
    virtual ~Cx_FwIntf(){};

	virtual LWRESULT Api_Fw_Start();
	virtual LWRESULT Api_Fw_Stop();

	virtual LWRESULT Api_Fw_RegisteModule(Ix_Module* plugin_module);
	virtual LWRESULT Api_Fw_LoadModule(const tchar_* file_name, const Ix_ObjectFactory* plugin_factory, MODID& modid, Ix_Module** pModuleObj);
	virtual LWRESULT Api_Fw_UnLoadModule(const MODID& modid);

	virtual const tchar_*    Api_Fw_GetModuleName(const MODID& modid);
	virtual Ix_Module* 		 Api_Fw_GetModulePtr(const MODID& modid);

};


EXTERN_C_END;
LWDP_NAMESPACE_END;

#endif // IX_CONFIG_MANAGER_H


