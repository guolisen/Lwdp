/*! \file   XModuleDefine.h
 *  \brief  Define macros of class factory registry.
 *  \author Guolisen,  LWDP  Framework
 *  \date   2011.10.19
 */
#ifndef LWDP_MODULE_DEFINE_H_
#define LWDP_MODULE_DEFINE_H_

#include "Cx_Object.h"
#include "Cx_Singletonobject.h"
 
LWDP_NAMESPACE_BEGIN;

#define INTER_MOD_NAME(module)  Inter_##module

#define DEF_MODULE_INFO_BEGIN(module, codeDomain, plugin_level, plugin_type) \
	static X3MODULEINFO s_module_local = {__T(#module), codeDomain, (plugin_level), plugin_type, iPLUGIN_VERSION, UNICODE_TAG, (MODULEID)-1, MODID_##module}; \
	class INTER_MOD_NAME(module) : public Cx_Module \
	{ \
	public: \
	    INTER_MOD_NAME(module)() \
        { \
	        m_moduleInfo = &s_module_local; \
            m_classEntry = s_classes_local; \
        } 

#define DEF_MODULE_INFO_END(module) \
}; \
REGISTER_MODULE(module); 



#define DEF_FUN_INT(t, d, f)  t  d::f()


#define DEF_INIT_FUN(module)  DEF_FUN_INT(LWRESULT, INTER_MOD_NAME(module), InitializePlugin)
#define DEF_UNINIT_FUN(module)  DEF_FUN_INT(LWRESULT, INTER_MOD_NAME(module), UninitializePlugin)

#define DEC_FUN_INT(t, f)  virtual t f()
#define DEC_INIT_FUN()  DEC_FUN_INT(LWRESULT, InitializePlugin)
#define DEC_UNINIT_FUN()  DEC_FUN_INT(LWRESULT, UninitializePlugin)




extern LWRESULT Fw_RegisteModule(Ix_Module* plugin_module);

#define REGISTER_MODULE_NAME(x) CRegister ## x
#define MODULE_NAMESPACE(x) NLwdp_ ## x

#ifndef LWDP_PLUGIN_CONTEXT
#define REGISTER_MODULE(module) struct REGISTER_MODULE_NAME(module) { \
    REGISTER_MODULE_NAME(module)() { s_lwdpModule = new INTER_MOD_NAME(module); Fw_RegisteModule(s_lwdpModule); }}; \
	namespace MODULE_NAMESPACE(module){REGISTER_MODULE_NAME(module) gRegisterModule; Ix_Module* pLocalModule = s_lwdpModule;};
#else
#define REGISTER_MODULE(module)   struct REGISTER_MODULE_NAME(module) { \
    REGISTER_MODULE_NAME(module)() { s_lwdpModule = new INTER_MOD_NAME(module); }}; \
    namespace MODULE_NAMESPACE(module){REGISTER_MODULE_NAME(module) gRegisterModule; Ix_Module* pLocalModule = s_lwdpModule;};
#endif


LWDP_NAMESPACE_END;

#endif // LWDP_MODULE_DEFINE_H_
