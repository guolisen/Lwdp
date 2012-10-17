/*! \file Cx_XmlMgr.h
 *  \brief  
 *  \author Guolisen, Lwdp PluginFramework
 *  \date   2010.11.8
 */
#ifndef LWDP_CX_XML_MANAGER_H_
#define LWDP_CX_XML_MANAGER_H_



#include <Interface/ConfigMgr/Ix_XmlMgr.h>
#include <boost/shared_ptr.hpp>

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;

typedef boost::shared_ptr<TiXmlDocument>  XmlDoc_Ptr;


class Cx_XmlMgr : public Ix_XmlMgr
{
    X3BEGIN_CLASS_DECLARE(Cx_XmlMgr)
        X3DEFINE_INTERFACE_ENTRY(Ix_XmlMgr)
    X3END_CLASS_DECLARE();
	
protected:
    Cx_XmlMgr() : mXmlFileName(""){};
    virtual ~Cx_XmlMgr() {}

    virtual LWRESULT SetXmlFileName(const tstring& xml_file_name);

	virtual LWRESULT XmlParse(const tchar_* xml_file_buffer);

	virtual LWRESULT XmlPrint();

	virtual LWRESULT XmlGetElement(const NX::XNodeBase& 	select_expr, 
							 		    const CTagForm* 		element_form,
									    int32_ 					form_entry_count,
									    XmlResults&				xml_result);

	virtual LWRESULT XmlGetElementSet(const NX::XNodeBase& select_expr, 
										   XmlResultsSet&   	xml_set);

	virtual LWRESULT XmlGetElementTree(const NX::XNodeBase& select_expr, 
										     TagTree_Ptr&   	  xml_tree);

	virtual LWRESULT XmlGetElementBuf(const NX::XNodeBase& 	select_expr,  
										    Xml_Ptr& 		 		xml_buff);

	virtual LWRESULT XmlParseExprStr(const tstring& expr, tstring& ret_str);

protected:
	virtual LWRESULT nodeRecursion(TiXmlNode* node, const tstring& element_name, XmlResults& xml_result);
	virtual LWRESULT nodeTree(TiXmlNode* node, const tstring& element_name, TagTree_Ptr& xml_tree);

	
#if 0
	virtual LWRESULT XmlAddElement(const tchar_* element_exoression, const tchar_* add_element);

	virtual LWRESULT XmlRemoveElement(const tchar_* element_exoression);

	virtual LWRESULT XmlSetAttrib(const tchar_* element_exoression, const tchar_* attrib, const tchar_* value);

	virtual LWRESULT XmlRemoveAttrib(const tchar_* element_exoression, const tchar_* attrib);

	virtual LWRESULT XmlSetText(const tchar_* element_exoression, const tchar_* change_text);

	virtual LWRESULT XmlRemoveText(const tchar_* element_exoression, const tchar_* text);

	virtual LWRESULT XmlGetText(const tchar_* element_exoression, const TextSet& text_set);

	virtual LWRESULT XmlGetAttrib(const tchar_* element_exoression, const AttribSet& attrib_value_set);

#endif




protected:
	tstring 	  mXmlFileName;
	XmlDoc_Ptr 	  mDocment;
};

EXTERN_C_END;
LWDP_NAMESPACE_END;


#endif // LWDP_CX_XML_MANAGER_H_


