/*! \file Cx_ConfigMgr.h
 *  \brief Define Config Manager 
 *  \author Guolisen, Lwdp
 *  \date   2011.11.04
 */
#ifndef CX_CONFIG_MANAGER_H
#define CX_CONFIG_MANAGER_H

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include "LwApiLib/ComLib/tinyxpath/xpath_static.h"


LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;

struct CfgSrcEntry
{
	tstring cfgSrcName;
	Cx_Ptr  cfgSrcData;
};

typedef std::list<CfgSrcEntry> CONFIGDATA_LIST;

class Cx_ConfigMgr : public Ix_ConfigMgr
{
    X3BEGIN_CLASS_DECLARE(Cx_ConfigMgr)
        X3DEFINE_INTERFACE_ENTRY(Ix_ConfigMgr)
    X3END_CLASS_DECLARE();

protected:
    Cx_ConfigMgr(){mConfigDataList.clear();};
    virtual ~Cx_ConfigMgr(){}

	virtual LWRESULT RegisteToLua();

	virtual LWRESULT AddConfigSrc(Ix_ConfigSrc* config_src);

	virtual LWRESULT DelConfigSrc(const tstring& config_src_name);

	virtual LWRESULT GetModulePropertys(const TAG_STR module_or_moid, 
										      const CPropertyForm* property_form, 
											  int32_ num_form_entry, 
											  XPropertys& propertys);
	
	virtual LWRESULT GetModulePropEntry(const TAG_STR module_or_moid, 
										      const TAG_STR property_name, 
											  XPropertys&    propertys);

	virtual LWRESULT GetModuleTable(const TAG_STR  module_or_moid, 
										 const TAG_STR  table_name, 
										 XPropertyTable& property_table);

	virtual LWRESULT GetModuleScript(const TAG_STR module_or_moid, const TAG_STR script_name, Script_Ptr& script_buff);


private:
	CONFIGDATA_LIST mConfigDataList;  
};


EXTERN_C_END;
LWDP_NAMESPACE_END;

#endif // IX_CONFIG_MANAGER_H

