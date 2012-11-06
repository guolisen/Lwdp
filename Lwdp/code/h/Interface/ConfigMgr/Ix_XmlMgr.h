/*! \file Ix_XmlMgr.h
 *  \brief Define Xml Manager 
 *  \author Guolisen, Lwdp
 *  \date   2011.11.04
 */
#ifndef IX_XML_MANAGER_H
#define IX_XML_MANAGER_H

#include <LwDp.h>
#include <list>
#include <string>
#include <map>
#include <Interface/Ix_Object.h>
#include <boost/shared_ptr.hpp>
#include "Id_ConfigMgr.h"

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;

typedef  tstring TAG_STR;
typedef  int8_   PC_DATA;

/////////////////////////////////////////////////////////////////////
namespace NX{

class XNodeBase 
{
	friend class Ele;
public:
	XNodeBase():mTag(""){};

	XNodeBase(const tchar_* tag_str):mTag(tag_str){};
	
	XNodeBase(const tstring& tag_str):mTag(tag_str){};

	XNodeBase(const XNodeBase& other):mTag(other.mTag){};
	virtual ~XNodeBase(){};

	virtual XNodeBase operator/(const XNodeBase& s2)
	{
		tstring tmpStr(s2.mTag);
		if(s2.mTag[0] == '/' && s2.mTag[1] == '/')
		{
			tmpStr = s2.mTag.substr(2);
		}
	
		if(mTag[0] == '/' && mTag[1] == '/')
			return mTag + static_cast<tstring>("/") + tmpStr;
		else
			return static_cast<tstring>("//") + mTag + static_cast<tstring>("/") + tmpStr;
	}
	virtual XNodeBase operator/(const tstring& s2)
	{
		tstring tmpStr(s2);
		if(s2[0] == '/' && s2[1] == '/')
		{
			tmpStr = s2.substr(2);
		}

		if(mTag[0] == '/' && mTag[1] == '/')
			return mTag + static_cast<tstring>("/") + s2;
		else
			return static_cast<tstring>("//") + mTag + static_cast<tstring>("/") + s2;
	}

	virtual const tstring operator+(const XNodeBase& s2)
	{
		return mTag + s2.mTag;
	}
	virtual const XNodeBase operator=(const XNodeBase& s2)
		{ printf("====%s : %s\n", s2.mTag.c_str(), mTag.c_str()); return (mTag = s2.mTag); }
		
	virtual const XNodeBase operator==(const XNodeBase& s2)
		{ return mTag + static_cast<tstring>("=") + s2.mTag; }
	
	virtual const XNodeBase operator<(const XNodeBase& s2)
		{ return mTag + static_cast<tstring>("<") + s2.mTag; }

	virtual const XNodeBase operator>(const XNodeBase& s2)
		{ return mTag + static_cast<tstring>(">") + s2.mTag; }
		
	virtual const XNodeBase operator!=(const XNodeBase& s2)
		{ return mTag + static_cast<tstring>("!=") + s2.mTag; }

	virtual LWRESULT SetTag(const tstring& tag_str){mTag = tag_str; return LWDP_OK;};
	virtual tstring& GetTag(){return mTag;};


protected:
	std::string mTag;
			
};

typedef XNodeBase N;
typedef XNodeBase EXPR;

class Ele : public XNodeBase
{
public:
	Ele():XNodeBase(""){}
	
	Ele(const tstring& tag_str)
	{
		tstring tmpStr;
		if(tag_str[0] == '/' && tag_str[1] == '/')
		{
			mTag = tag_str;
		}
		else
		{
			mTag = static_cast<tstring>("//") + tag_str;
		}
	}

	Ele(const Ele& other)
	{
		printf("Ele Copy :%s\n", other.mTag.c_str());
		mTag = other.mTag;
	}
	
	virtual Ele& operator=(const Ele& other)
	{
		printf("Ele operator= :%s\n", other.mTag.c_str());
		mTag = other.mTag;
		return *this;
	}
	virtual Ele operator/(const Ele& s2)
	{
		tstring tmpStr(s2.mTag);
		if(s2.mTag[0] == '/' && s2.mTag[1] == '/')
		{
			tmpStr = s2.mTag.substr(2);
		}
	
		if(mTag[0] == '/' && mTag[1] == '/')
			return mTag + static_cast<tstring>("/") + tmpStr;
		else
			return static_cast<tstring>("//") + mTag + static_cast<tstring>("/") + tmpStr;
	}

	virtual Ele FilterExpr(const XNodeBase& filter_expr)
	{ 
		return Ele(mTag + static_cast<tstring>("[") + filter_expr.mTag + static_cast<tstring>("]"));
	};

};

const Ele AllEleStar("*");

class V : public XNodeBase
{
public:
	V():XNodeBase("''"){}
	
	V(const tstring& tag_str):XNodeBase(static_cast<tstring>("'") + tag_str + static_cast<tstring>("'")){}

};

class Attr : public XNodeBase
{
public:
	Attr():XNodeBase("@"){}
	
	Attr(const tstring& tag_str):XNodeBase(static_cast<tstring>("@") + tag_str){}

};

struct FunInfo{
	const int32_  para_num;
	const tchar_* fun_name;
	const tchar_* fun_patten;
};

const FunInfo gFunInfoTable[] = {{1, __T("count"), 		__T("count(%s)")},
								 {2, __T("position"), 	__T("%s[%s]")},
								 {3, __T("concat"), 	__T("concat(%s, %s, %s)")}};
const int32_  gNumFunTable = sizeof(gFunInfoTable)/sizeof(FunInfo);

class Fun : public XNodeBase
{
public:
	Fun():XNodeBase(""){}
	
	Fun(const tstring& tag_str):XNodeBase(tag_str){}
	
	virtual const tstring operator()(const tstring& s1)
	{
		for(int32_ i=0; i<gNumFunTable; ++i)
		{
			if(Api_tcscmp((char_*)gFunInfoTable[i].fun_name, (const char_*)mTag.c_str()) == 0)
			{
				if(gFunInfoTable[i].para_num != 1)
					return static_cast<tstring>("");
				
				tchar_ buffer[LWDP_MAX_PATH] = {0};
				Api_sprintf(buffer, gFunInfoTable[i].fun_patten, s1.c_str());
				return static_cast<tstring>(buffer);
			}
		}
		return static_cast<tstring>("");
	}

	virtual const tstring operator()(XNodeBase& s1)
	{
		return operator()(s1.GetTag());
	}
	
	virtual const tstring operator()(const tstring& s1, const tstring& s2)
	{
		for(int32_ i=0; i<gNumFunTable; ++i)
		{
			if(Api_tcscmp(gFunInfoTable[i].fun_name, mTag.c_str()) == 0)
			{
				if(gFunInfoTable[i].para_num != 2)
					return static_cast<tstring>("");
				
				tchar_ buffer[LWDP_MAX_PATH] = {0};
				Api_sprintf(buffer, gFunInfoTable[i].fun_patten, s1.c_str(), s2.c_str());
				return static_cast<tstring>(buffer);
			}
		}
		return static_cast<tstring>("");
	}

	virtual const tstring operator()(XNodeBase& s1, XNodeBase& s2)
	{
		return operator()(s1.GetTag(), s2.GetTag());
	}

	virtual const tstring operator()(const tstring& s1, const tstring& s2, const tstring& s3)
	{
		for(int32_ i=0; i<gNumFunTable; ++i)
		{
			if(Api_tcscmp(gFunInfoTable[i].fun_name, mTag.c_str()) == 0)
			{
				if(gFunInfoTable[i].para_num != 3)
					return static_cast<tstring>("");
				
				tchar_ buffer[LWDP_MAX_PATH] = {0};
				Api_sprintf(buffer, gFunInfoTable[i].fun_patten, s1.c_str(), s2.c_str(), s3.c_str());
				return static_cast<tstring>(buffer);
			}
		}
		return static_cast<tstring>("");
	}

	virtual const tstring operator()(XNodeBase& s1, XNodeBase& s2, XNodeBase& s3)
	{
		return operator()(s1.GetTag(), s2.GetTag(), s3.GetTag());
	}

};
static Fun  pos("position");
static Fun  count("count");
static Fun  concat("concat");

};


/////////////////////////////////////////////////////////////////////

namespace NTagType
{
	enum TagTypeEnum
	{
		kSimple,
	};
};

struct CTagForm
{
	const tstring    	  IDString;
	NTagType::TagTypeEnum Type;
};


struct CAttributeStru
{
	tstring AttribName;
	tstring AttribValue;
	CAttributeStru():AttribName(""), AttribValue(""){}

	CAttributeStru(const CAttributeStru& other)
	{
		AttribName  = other.AttribName;
		AttribValue = other.AttribValue;
	}
};

typedef boost::shared_ptr<CAttributeStru> Attrib_Ptr;
typedef std::vector<Attrib_Ptr>           ATTRIB_VECTOR;

class Cx_XmlMgr;
struct CTagEntry
{
	friend class Cx_XmlMgr;
	bool    ThereIs;
	tstring propertyName;
	tstring propertyText;
	tstring parentName;
	
	CTagEntry(const CTagEntry& other)
	{
		ThereIs = other.ThereIs;
		propertyName = other.propertyName;
		propertyText = other.propertyText;
		parentName   = other.parentName;
		mAttribute.clear();
		for(uint32_ i=0; i<other.mAttribute.size(); ++i)
		{
			mAttribute.push_back(other.mAttribute[i]);
		}
	};

	CTagEntry(): ThereIs(false), 
				    propertyName(""), 
				    propertyText(""),
				    parentName("")
				    {mAttribute.clear();}
						  

	const tstring operator[](const tstring& attr_name) const
	{
		for(uint32_ iter = 0; iter < mAttribute.size(); ++iter)
		{
			if(mAttribute[iter]->AttribName == attr_name)
				return mAttribute[iter]->AttribValue;
		}

		return static_cast<tstring>("");  //Return empty str
	}
	const CAttributeStru operator[](const uint32_ index) const
	{
		if(index < mAttribute.size())
			return (*mAttribute[index]);

		return CAttributeStru();  //Return empty str
	}

	uint32_ GetAttrCount() const {return (uint32_)mAttribute.size();};
protected:
	void SetAttribute(const tstring& attr_name, const tstring& attr_value)
	{
		Attrib_Ptr attr_ptr(new CAttributeStru);
		attr_ptr->AttribName	 = attr_name;
		attr_ptr->AttribValue    = attr_value;

		mAttribute.push_back(attr_ptr);
	}
	
protected:
	ATTRIB_VECTOR mAttribute;
};
  
typedef boost::shared_ptr<CTagEntry>   TagEntry_Ptr;
typedef std::vector<TagEntry_Ptr>      TagEntryList;

class XmlResults
{
public:
	XmlResults(){mProperty.clear();};
	virtual ~XmlResults(){printf("!!!!!!!~XmlResults release!\n");};

	virtual XmlResults& operator=(const XmlResults& other)
	{
		mProperty = other.mProperty;
		return *this;
	}

	virtual const CTagEntry operator[](const tstring& property_name)
	{
		for(uint32_ i=0; i<mProperty.size(); ++i)
		{
			if(property_name == mProperty[i]->propertyName)
				return *mProperty[i];
		}

		CTagEntry emptyPtr;
		return emptyPtr; //return a NULL property
	}

	virtual const CTagEntry operator[](const uint32_ it)
	{
		if(it < mProperty.size())
			return *mProperty[it];
		
		CTagEntry emptyPtr;
		return emptyPtr; //return a NULL property
	}

	virtual LWRESULT AddProperty(TagEntry_Ptr result_entry) 
	{
		TagEntryList::iterator iter;
		FOREACH_STL(iter, mProperty)
		{
			if((*iter)->propertyName == result_entry->propertyName)
			{
				(*iter) = result_entry;
				return LWDP_OK;
			}
		}
		
		mProperty.push_back(result_entry);
		return LWDP_OK;
	}
	virtual void Clear() 
	{
		mProperty.clear();
	}	
	virtual bool Empty(){return mProperty.empty();}	
protected:
	TagEntryList mProperty;
};

class XmlResultsSet : public XmlResults
{
public:
	
	XmlResultsSet(){};
	virtual ~XmlResultsSet(){printf("!!!!!!!~XmlResultsSet release!\n");};

	virtual LWRESULT AddProperty(TagEntry_Ptr result_entry) 
	{
		mProperty.push_back(result_entry);
		return LWDP_OK;
	}

};


struct TreeEntry;
typedef boost::shared_ptr<TreeEntry> TagTree_Ptr;
typedef std::list<TagTree_Ptr> TreePtrList;
struct TreeEntry
{
	TagEntry_Ptr  levelEntry;
	TreePtrList   subTree;
};


/////////////////////////////////////////////////////////////////////

typedef  char_  XML_BUFFER;
typedef  boost::shared_ptr<XML_BUFFER> Xml_Ptr;


INTERFACE_BEGIN(XmlMgr)
	
    virtual LWRESULT SetXmlFileName(const tstring& xml_file_name) = 0;

	virtual LWRESULT XmlParse(const XML_BUFFER* xml_file_buffer) = 0;

	virtual LWRESULT XmlPrint() = 0;

	virtual LWRESULT XmlGetElement(const NX::XNodeBase& 	select_expr, 
								   const CTagForm* 			element_form,
								   int32_ 					form_entry_count,
								   XmlResults&				xml_result) = 0;
	
	virtual LWRESULT XmlGetElementSet(const NX::XNodeBase& 	select_expr, 
									  XmlResultsSet& 		xml_set) = 0;

	virtual LWRESULT XmlGetElementTree(const NX::XNodeBase& select_expr, 
										       TagTree_Ptr& xml_tree) = 0;

	virtual LWRESULT XmlGetElementBuf(const NX::XNodeBase& 	select_expr, 
									  Xml_Ptr& 		 		xml_buff) = 0;

	virtual LWRESULT XmlParseExprStr(const tstring& expr, tstring& ret_str) = 0;

	
#if 0
	virtual LWRESULT XmlAddElement(const tchar_* element_exoression, const tchar_* add_element) = 0;

	virtual LWRESULT XmlRemoveElement(const tchar_* element_exoression) = 0;

	virtual LWRESULT XmlSetAttrib(const tchar_* element_exoression, const tchar_* attrib, const tchar_* value) = 0;

	virtual LWRESULT XmlRemoveAttrib(const tchar_* element_exoression, const tchar_* attrib) = 0;

	virtual LWRESULT XmlSetText(const tchar_* element_exoression, const tchar_* change_text) = 0;

	virtual LWRESULT XmlRemoveText(const tchar_* element_exoression, const tchar_* text) = 0;

	virtual LWRESULT XmlGetText(const tchar_* element_exoression, const TextSet& text_set) = 0;

	virtual LWRESULT XmlGetAttrib(const tchar_* element_exoression, const AttribSet& attrib_value_set) = 0;
#endif

INTERFACE_END();








EXTERN_C_END;
LWDP_NAMESPACE_END;

#endif // IX_XML_MANAGER_H

