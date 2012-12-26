/*! \file FrameworkEntry.h
 *  \brief Common Framework Api
 *  \author Guolisen, LwDp
 *  \date   2011.07.14
 */

#ifndef LW_ENTRY_FRM_H_
#define LW_ENTRY_FRM_H_

#include <Interface/Ix_Object.h>
#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
 
#include "FwTagDef.h"

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;
class Ix_Module;
class Ix_ObjectFactory;
//==================================================
//            Framework Function Api 
//==================================================
LWRESULT Fw_Init(Ix_ConfigSrc* config_src, uint32_ entry_count);
LWRESULT Fw_Start();
LWRESULT Fw_Stop();

LWRESULT Fw_RegisteModule(Ix_Module* plugin_module);
LWRESULT Fw_LoadModule(const tchar_* file_name, const Ix_ObjectFactory* plugin_factory, MODID& modid, Ix_Module** pModuleObj);
LWRESULT Fw_UnLoadModule(const MODID& modid);

const tchar_*    Fw_GetModuleName(const MODID& modid);
Ix_Module* 		 Fw_GetModulePtr(const MODID& modid);


//
#define MAX_PLUGIN_NUM	 50
typedef  int32_ TABCAT;
typedef struct stru_module_entry{
	const tchar_* module_name;
	MODID		  module_modid;
	Ix_Module*    module_ptr;

	stru_module_entry(): module_name(NULL), 
							 module_modid(NULL), 
							 module_ptr(NULL){}

	stru_module_entry(const stru_module_entry& other)   //��������
	{
		module_name  = other.module_name;
		module_modid = other.module_modid;
		module_ptr 	 = other.module_ptr;
	};

	stru_module_entry& operator=(const stru_module_entry& rhs)  
	{
		module_name  = rhs.module_name;
		module_modid = rhs.module_modid;
		module_ptr 	 = rhs.module_ptr;
 
		return *this;
	}
}MODULE_ENTRY;

struct stru_module_table
{
	struct stru_module_entry* moduleEntry[MAX_PLUGIN_NUM];
	uint32_	tableSize;
	pthread_mutex_t tableLock;

	stru_module_table(): tableSize(0)
	{
		memset(moduleEntry, NULL, sizeof(struct stru_module_entry*) * MAX_PLUGIN_NUM);
		pthread_mutex_init(&tableLock, NULL);
	}


private:
	stru_module_table(const stru_module_table& other);
	stru_module_table& operator=(const stru_module_table& rhs);

};

EXTERN_C_END;
LWDP_NAMESPACE_END;
#endif




 


