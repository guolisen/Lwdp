// Copyright 2008-2011 Guolisen, guolisen@hotmail.com
// 
// 2011.11.10 Create File By Guolisen
#include <LwDp.h>
#include <PluginInc.h>

#include <Interface/ConfigMgr/Ix_XmlMgr.h>
#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include "Cx_ConfigMgr.h"


LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;


LWRESULT Cx_ConfigMgr::AddConfigSrc(Ix_ConfigSrc* config_src)
{
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_PARAMETER_ERROR, (config_src), "Cx_ConfigMgr::AddConfigSrc Para Pointor is NULL");

	CONFIGDATA_LIST::iterator iter;
	tstring SrcName = config_src->GetConfigNameStr();
	FOREACH_STL(iter, mConfigDataList)
	{
		if(iter->cfgSrcName == SrcName)
			return LWDP_CONFIG_SRC_EXIST_ERROR;
	}

	PC_DATA* xmlData = config_src->LoadConfigData();
	if(!xmlData)
		return LWDP_GET_CONFIG_DATA_ERROR;

	GET_OBJECT(XmlMgr, iXmlMgr, LWDP_GET_OBJECT_ERROR);
	if (iXmlMgr->XmlParse((XML_BUFFER*)xmlData) != LWDP_OK)
	{
		config_src->ReleseConfigData();
		return LWDP_PARSE_XML_DATA_ERROR;
	}

	CfgSrcEntry entry;
	entry.cfgSrcName = config_src->GetConfigNameStr();
	entry.cfgSrcData = Cx_Ptr(iXmlMgr);
	mConfigDataList.push_back(entry);
	config_src->ReleseConfigData();
	return LWDP_OK;
}

LWRESULT Cx_ConfigMgr::DelConfigSrc(const tstring& config_src_name)
{
	CONFIGDATA_LIST::iterator iter;
	FOREACH_STL(iter, mConfigDataList)
	{
		if(iter->cfgSrcName == config_src_name)
			break;
	}

	if(iter != mConfigDataList.end())
		mConfigDataList.erase(iter);
	
	return LWDP_OK;
}

LWRESULT Cx_ConfigMgr::GetModulePropertys(const TAG_STR module_or_moid, const CPropertyForm* property_form, int32_ num_form_entry, XPropertys& propertys)
{
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_PARAMETER_ERROR, (!module_or_moid.empty()), "Cx_ConfigMgr::GetModulePropertys Para Error");
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_PARAMETER_ERROR, (property_form), "Cx_ConfigMgr::GetModulePropertys Para Error");

	//find config data in all Src
	LWRESULT stat = LWDP_CANNOT_FIND_CONFIG_ERROR;
	CONFIGDATA_LIST::iterator iter;
	FOREACH_STL(iter, mConfigDataList)
	{
		NX::Ele moduleTag(LW_CONFIG_MODULE_TAG);
		NX::Ele propTag(LW_CONFIG_PROPERTY_TAG);
		NX::EXPR nameXPath  = moduleTag.FilterExpr(NX::Attr(LW_CONFIG_NAME_ATTR) == NX::V(module_or_moid)) / propTag ;
		NX::EXPR modidXPath = moduleTag.FilterExpr(NX::Attr(LW_CONFIG_MODID_ATTR) == NX::V(module_or_moid)) / propTag ;

		Cx_Interface<Ix_XmlMgr> pXmlMgr(iter->cfgSrcData);
		ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_GET_OBJECT_ERROR, pXmlMgr.P(), "Can NOT Get XmlMgr Pointer!");
		if((stat = pXmlMgr->XmlGetElement(nameXPath, (CTagForm*)property_form, num_form_entry, propertys)) != LWDP_OK)
		{
			if((stat = pXmlMgr->XmlGetElement(modidXPath, (CTagForm*)property_form, num_form_entry, propertys)) != LWDP_OK)
				return stat;
		}
	}

	return LWDP_OK;
}

LWRESULT Cx_ConfigMgr::GetModulePropEntry(const TAG_STR module_or_moid, const TAG_STR property_name, XPropertys& property_entry)
{
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_PARAMETER_ERROR, (!module_or_moid.empty()), "Cx_ConfigMgr::GetModulePropertys Para Error");

	//! find config data in all Src
	LWRESULT stat = LWDP_CANNOT_FIND_CONFIG_ERROR;
	CONFIGDATA_LIST::iterator iter;
	FOREACH_STL(iter, mConfigDataList)
	{
		NX::Ele moduleTag(LW_CONFIG_MODULE_TAG);
		NX::Ele propTag(LW_CONFIG_PROPERTY_TAG);
		NX::EXPR nameXPath  = moduleTag.FilterExpr(NX::Attr(LW_CONFIG_NAME_ATTR) == NX::V(module_or_moid)) / propTag ;
		NX::EXPR modidXPath = moduleTag.FilterExpr(NX::Attr(LW_CONFIG_MODID_ATTR) == NX::V(module_or_moid)) / propTag ;

		CTagForm propSelect = {property_name, NTagType::kSimple};
		Cx_Interface<Ix_XmlMgr> pXmlMgr(iter->cfgSrcData);
		ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_GET_OBJECT_ERROR, pXmlMgr.P(), "Can NOT Get XmlMgr Pointer!");
		if((stat = pXmlMgr->XmlGetElement(nameXPath, &propSelect, 1, property_entry)) != LWDP_OK)
		{
			if((stat = pXmlMgr->XmlGetElement(modidXPath, &propSelect, 1, property_entry)) != LWDP_OK)
				return stat;
		}
	}

	return LWDP_OK;
}

LWRESULT Cx_ConfigMgr::GetModuleTable(const TAG_STR module_or_moid, const TAG_STR table_name, XPropertyTable& property_table)
{
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_PARAMETER_ERROR, (!module_or_moid.empty()), "Cx_ConfigMgr::GetModulePropertys Para Error");
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_PARAMETER_ERROR, (!table_name.empty()), "Cx_ConfigMgr::GetModulePropertys Para Error");
	
	//find config data in all Src
	LWRESULT stat = LWDP_CANNOT_FIND_CONFIG_ERROR;
	CONFIGDATA_LIST::reverse_iterator riter; //use last Config Src
	for(riter = mConfigDataList.rbegin(); riter != mConfigDataList.rend(); ++riter)
	{
		NX::Ele moduleTag(LW_CONFIG_MODULE_TAG);
		NX::Ele tableSetTag(LW_CONFIG_TABLESET_TAG);
		NX::Ele tableTag(LW_CONFIG_TABLE_TAG);
		NX::EXPR nameXPath  = moduleTag.FilterExpr(NX::Attr(LW_CONFIG_NAME_ATTR) == NX::V(module_or_moid)) / tableSetTag / 
								tableTag.FilterExpr(NX::Attr(LW_CONFIG_NAME_ATTR) == NX::V(table_name));
		NX::EXPR modidXPath = moduleTag.FilterExpr(NX::Attr(LW_CONFIG_MODID_ATTR) == NX::V(module_or_moid)) / tableSetTag / 
								tableTag.FilterExpr(NX::Attr(LW_CONFIG_NAME_ATTR) == NX::V(table_name));

		Cx_Interface<Ix_XmlMgr> pXmlMgr(riter->cfgSrcData);
		ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_GET_OBJECT_ERROR, pXmlMgr.P(), "Can NOT Get XmlMgr Pointer!");
		if((stat = pXmlMgr->XmlGetElementSet(nameXPath, property_table)) != LWDP_OK)
		{
			if((stat = pXmlMgr->XmlGetElementSet(modidXPath, property_table)) != LWDP_OK)
				continue;
		}
		break;
	}

	return stat;
}

LWRESULT Cx_ConfigMgr::GetModuleScript(const TAG_STR module_or_moid, const TAG_STR script_name, Script_Ptr& script_buff)
{
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_PARAMETER_ERROR, (!module_or_moid.empty()), "Cx_ConfigMgr::GetModuleScript Para Error");
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_PARAMETER_ERROR, (!script_name.empty()), "Cx_ConfigMgr::GetModuleScript Para Error");
	
	//find config data in all Src
	LWRESULT stat = LWDP_CANNOT_FIND_CONFIG_ERROR;
	TiXmlPrinter printer;
	CONFIGDATA_LIST::reverse_iterator riter; //use last Config Src
	for(riter = mConfigDataList.rbegin(); riter != mConfigDataList.rend(); ++riter)
	{
		NX::Ele moduleTag(LW_CONFIG_MODULE_TAG);
		NX::Ele scriptTag(LW_CONFIG_SCRIPT_TAG);
		NX::EXPR nameXPath  = moduleTag.FilterExpr(NX::Attr(LW_CONFIG_NAME_ATTR) == NX::V(module_or_moid)) / 
								scriptTag.FilterExpr(NX::Attr(LW_CONFIG_NAME_ATTR) == NX::V(script_name));
		NX::EXPR modidXPath = moduleTag.FilterExpr(NX::Attr(LW_CONFIG_MODID_ATTR) == NX::V(module_or_moid)) / 
								scriptTag.FilterExpr(NX::Attr(LW_CONFIG_NAME_ATTR) == NX::V(script_name));

		Cx_Interface<Ix_XmlMgr> pXmlMgr(riter->cfgSrcData);
		ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_GET_OBJECT_ERROR, pXmlMgr.P(), "Can NOT Get XmlMgr Pointer!");

		if((stat = pXmlMgr->XmlGetElementBuf(nameXPath, script_buff)) == LWDP_OK)
			break;

		if((stat = pXmlMgr->XmlGetElementBuf(modidXPath, script_buff)) == LWDP_OK)
			break;
	}

	return stat;
}


EXTERN_C_END;
LWDP_NAMESPACE_END;


