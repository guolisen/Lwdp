/*! \file Ix_ConfigMgr.h
 *  \brief Define Config Manager 
 *  \author Guolisen, Lwdp
 *  \date   2011.11.04
 */
#ifndef IX_CONFIG_MANAGER_H
#define IX_CONFIG_MANAGER_H

#include <list>
#include <Interface/Ix_Object.h>
#include <boost/shared_ptr.hpp>

#include "Id_ConfigMgr.h"
#include <Interface/ConfigMgr/Ix_XmlMgr.h>
#include "ConfigTagDef.h"

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;

class Ix_ConfigSrc
{
public:
    virtual PC_DATA* LoadConfigData() = 0;
	virtual LWRESULT SaveConfigData(PC_DATA* buffer) = 0;
    virtual LWRESULT ReleseConfigData() = 0;
	
    virtual int32_   GetConfigDataSize() = 0;
    virtual const tchar_*  GetConfigNameStr() = 0;
};


typedef XmlResults   	XPropertys;
typedef XmlResultsSet   XPropertyTable;
typedef CTagForm   		CPropertyForm;
typedef Xml_Ptr  		Script_Ptr;


INTERFACE_BEGIN(ConfigMgr)
	//
    virtual LWRESULT AddConfigSrc(Ix_ConfigSrc* config_src) = 0;

	virtual LWRESULT DelConfigSrc(const tstring& config_src_name) = 0;

	virtual LWRESULT GetModulePropertys(const TAG_STR 			module_or_moid, 
		 						        const CPropertyForm* 	property_form, 
									    int32_ 					num_form_entry, 
									    XPropertys& 			propertys) = 0;

	virtual LWRESULT GetModulePropEntry(const TAG_STR module_or_moid, 
				  				        const TAG_STR property_name, 
									    XPropertys&    propertys) = 0;

    virtual LWRESULT GetModuleTable(const TAG_STR  module_or_moid, 
									const TAG_STR  table_name, 
									XPropertyTable& property_table) = 0;

	virtual LWRESULT GetModuleScript(const TAG_STR module_or_moid, const TAG_STR script_name, Script_Ptr& script_buff) = 0;
	//
INTERFACE_END();



EXTERN_C_END;
LWDP_NAMESPACE_END;

#endif // IX_CONFIG_MANAGER_H
