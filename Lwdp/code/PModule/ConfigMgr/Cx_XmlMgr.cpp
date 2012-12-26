#include <LwDp.h>
#include <PluginInc.h>

#include "LwApiLib/ComLib/tinyxpath/xpath_static.h"
#include "Cx_XmlMgr.h"

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;

LWRESULT Cx_XmlMgr::SetXmlFileName(const tstring& xml_file_name)
{
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_PARAMETER_ERROR, (!xml_file_name.empty()), "SetXmlFileName file name empty");
	mXmlFileName = xml_file_name;

	return LWDP_OK;
}

LWRESULT Cx_XmlMgr::XmlParse(const tchar_* xml_file_buffer)
{
	mDocment.reset();
	mDocment.reset(new TiXmlDocument);
	mDocment->Parse((const char *)xml_file_buffer);
	if (mDocment->Error())
	{
		lw_log_err(LWDP_MODULE_LOG,"Cx_XmlMgr::XmlParse Parse Xml Data Error (%s:%s Row:%d Col:%d) !",
				   mDocment->Value(), mDocment->ErrorDesc(), mDocment->ErrorRow()+1, mDocment->ErrorRow()+1);
		return LWDP_PARSE_XML_DATA_ERROR;
	}

	return LWDP_OK;
}

LWRESULT Cx_XmlMgr::XmlPrint()
{
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_XMLMGR_DOCUMENT_EMPTY_ERROR, (!mDocment.get()), "Xml File Member is Empty");

	TiXmlPrinter printer;
	mDocment->Accept(&printer);
	printf("%s\n", printer.CStr());
	return LWDP_OK;
}


LWRESULT Cx_XmlMgr::XmlGetElement(const NX::XNodeBase& 	select_expr, 
								      const CTagForm* 		element_form,
								      int32_ 				form_entry_count,
								      XmlResults&			xml_result) 
{
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_PARAMETER_ERROR, (element_form), "Cx_XmlMgr::XmlGetElement Para Error");

	if(mDocment == XmlDoc_Ptr())
		return LWDP_XMLMGR_XML_DOC_ERROR;
	
	TiXmlElement * XEp_main = mDocment->RootElement();
	NX::XNodeBase tmpNode = select_expr;
	TiXmlNode* propNode = TinyXPath::XNp_xpath_node(XEp_main, tmpNode.GetTag().c_str());
	if(!propNode)
		return LWDP_XMLMGR_ELEMENT_FIND_ERROR;
	
	for(int32_ i=0; i<form_entry_count; ++i)
	{
		nodeRecursion(propNode->FirstChild(), element_form[i].IDString, xml_result);
	}

	return LWDP_OK;
}


LWRESULT Cx_XmlMgr::XmlGetElementSet(const NX::XNodeBase& 	select_expr, 
									      XmlResultsSet&   		xml_set) 
{
	if(mDocment == XmlDoc_Ptr())
		return LWDP_XMLMGR_XML_DOC_ERROR;

	TiXmlElement * XEp_main = mDocment->RootElement();
	NX::XNodeBase tmpNode = select_expr;
	TiXmlNode* tableNode = TinyXPath::XNp_xpath_node(XEp_main, tmpNode.GetTag().c_str());
	if(!tableNode)
		return LWDP_XMLMGR_ELEMENT_FIND_ERROR;

	nodeRecursion(tableNode->FirstChild(), "", xml_set);

	return LWDP_OK;
}

LWRESULT Cx_XmlMgr::XmlGetElementBuf(const NX::XNodeBase& 	select_expr,
									       Xml_Ptr& 		 	xml_buff) 

{
	if(mDocment == XmlDoc_Ptr())
		return LWDP_XMLMGR_XML_DOC_ERROR;

	TiXmlPrinter printer;
	TiXmlElement* XEp_main = mDocment->RootElement();
	NX::XNodeBase tmpNode = select_expr;
	TiXmlNode* tableNode = TinyXPath::XNp_xpath_node(XEp_main, tmpNode.GetTag().c_str());
	if(!tableNode)
		return LWDP_XMLMGR_ELEMENT_FIND_ERROR;

	tableNode->Accept(&printer);
	uint32_ bufferSize = printer.Size(); 
	XML_BUFFER* tmpData = new XML_BUFFER[bufferSize + 1];
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_MALLOC_MEMORY_ERROR, (tmpData), "Cx_XmlMgr::XmlGetElementBuf new Memory Error!");
	memset(tmpData, 0, bufferSize + 1);
	memcpy(tmpData, (char *)printer.CStr(), bufferSize);
	xml_buff = Xml_Ptr(tmpData);
	
	return LWDP_OK;
}

LWRESULT Cx_XmlMgr::XmlParseExprStr(const tstring& expr, tstring& ret_str)
{
	if(mDocment == XmlDoc_Ptr())
		return LWDP_XMLMGR_XML_DOC_ERROR;
	
	TIXML_STRING S_res;
	TiXmlElement * XEp_main = mDocment->RootElement();	
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_POINTER_IS_NULL, XEp_main, "Cx_XmlMgr::XmlParseExprBool Get RootElement Error!");
	S_res = TinyXPath::S_xpath_string (XEp_main, expr.c_str());
	ret_str = static_cast<tstring>(S_res.c_str()); 

	return LWDP_OK;
}

LWRESULT Cx_XmlMgr::XmlGetElementTree(const NX::XNodeBase& select_expr, TagTree_Ptr& xml_tree)
{
	if(mDocment == XmlDoc_Ptr())
		return LWDP_XMLMGR_XML_DOC_ERROR;

	TiXmlElement * XEp_main = mDocment->RootElement();
	NX::XNodeBase tmpNode = select_expr;
	TiXmlNode* tableNode = TinyXPath::XNp_xpath_node(XEp_main, tmpNode.GetTag().c_str());
	if(!tableNode)
		return LWDP_XMLMGR_ELEMENT_FIND_ERROR;

	nodeTree(tableNode, "", xml_tree);

	return LWDP_OK;
}

LWRESULT Cx_XmlMgr::nodeRecursion(TiXmlNode* node, const tstring& element_name, XmlResults& xml_result)
{
	if(!node)
	{
		return LWDP_OK;
	}
	TiXmlElement*   tmpEle  = NULL;
	TiXmlAttribute* tmpAttr = NULL;
	for (TiXmlNode* iNode=node; iNode; iNode=iNode->NextSibling())
	{
		if(iNode->ToComment())
			continue;
		//Recursion
		nodeRecursion(iNode->FirstChild(), element_name, xml_result);
		//Find Value	
		tmpEle = iNode->ToElement();
		if(tmpEle)
		{
			if(element_name == static_cast<tstring>(tmpEle->Value()) || element_name.empty())
			{
				//lw_log_debug(LWDP_MODULE_LOG, "Cx_XmlMgr::nodeRecursion Have Found it:%s", tmpEle->Value());
				TagEntry_Ptr tmpEntry(new CTagEntry);

				const char_* tmpStr = NULL;
				if((tmpStr = tmpEle->Value()))
					tmpEntry->propertyName = static_cast<tstring>(tmpEle->Value());

				if((tmpStr = tmpEle->GetText()))
					tmpEntry->propertyText = static_cast<tstring>(tmpEle->GetText());

				tmpEntry->ThereIs = true;

				TiXmlNode* parentPtr = tmpEle->Parent();
				if(!parentPtr)
					tmpEntry->parentName = parentPtr->Value();

				for(tmpAttr = tmpEle->FirstAttribute(); tmpAttr; tmpAttr=tmpAttr->Next())
				{
					tmpEntry->SetAttribute(tmpAttr->Name(), tmpAttr->Value());
				}
				
				xml_result.AddProperty(tmpEntry);
			}
			//lw_log_debug(LWDP_MODULE_LOG, "Cx_XmlMgr::nodeRecursion All:%s", tmpEle->Value());
		}
	
	}

	return LWDP_OK;
}


LWRESULT Cx_XmlMgr::nodeTree(TiXmlNode* node, const tstring& element_name, TagTree_Ptr& xml_tree)
{
	if(!node)
	{
		return LWDP_OK;
	}
	TiXmlElement*   tmpEle  = NULL;
	TiXmlAttribute* tmpAttr = NULL;

	tmpEle = node->ToElement();
	if(tmpEle)
	{
		if(element_name == static_cast<tstring>(tmpEle->Value()) || element_name.empty())
		{		
			//lw_log_debug(LWDP_MODULE_LOG, "Cx_XmlMgr::nodeTree Have Found it:%s", tmpEle->Value());
			TagEntry_Ptr tmpEntry(new CTagEntry);

			const char_* tmpStr = NULL;
			if((tmpStr = tmpEle->Value()))
				tmpEntry->propertyName = static_cast<tstring>(tmpEle->Value());

			if((tmpStr = tmpEle->GetText()))
				tmpEntry->propertyText = static_cast<tstring>(tmpEle->GetText());

			tmpEntry->ThereIs = true;

			TiXmlNode* parentPtr = tmpEle->Parent();
			if(!parentPtr)
				tmpEntry->parentName = parentPtr->Value();

			for(tmpAttr = tmpEle->FirstAttribute(); tmpAttr; tmpAttr=tmpAttr->Next())
			{
				tmpEntry->SetAttribute(tmpAttr->Name(), tmpAttr->Value());
			}
			
			xml_tree->levelEntry = tmpEntry;
		}
	}

	for (TiXmlNode* iNode=node->FirstChild(); iNode; iNode=iNode->NextSibling())
	{	
		if(iNode->ToComment())
			continue;
		TagTree_Ptr subTree(new TreeEntry);
		//Recursion
		nodeTree(iNode, element_name, subTree);
		//Find Value	
		xml_tree->subTree.push_back(subTree);
	}

	return LWDP_OK;
}



#if 0
LWRESULT Cx_XmlMgr::XmlAddElement(const tchar_* element_exoression, const tchar_* add_element)
{

	return LWDP_OK;
}

LWRESULT Cx_XmlMgr::XmlRemoveElement(const tchar_* element_exoression)
{

	return LWDP_OK;
}

LWRESULT Cx_XmlMgr::XmlSetAttrib(const tchar_* element_exoression, const tchar_* attrib, const tchar_* value)
{

	return LWDP_OK;
}

LWRESULT Cx_XmlMgr::XmlRemoveAttrib(const tchar_* element_exoression, const tchar_* attrib)
{

	return LWDP_OK;
}

LWRESULT Cx_XmlMgr::XmlSetText(const tchar_* element_exoression, const tchar_* change_text)
{

	return LWDP_OK;
}

LWRESULT Cx_XmlMgr::XmlRemoveText(const tchar_* element_exoression, const tchar_* text)
{

	return LWDP_OK;
}

LWRESULT Cx_XmlMgr::XmlGetText(const tchar_* element_exoression, const TextSet& text_set)
{

	return LWDP_OK;
}

LWRESULT Cx_XmlMgr::XmlGetAttrib(const tchar_* element_exoression, const AttribSet& attrib_value_set)
{

	return LWDP_OK;
}

#endif



EXTERN_C_END;
LWDP_NAMESPACE_END;
