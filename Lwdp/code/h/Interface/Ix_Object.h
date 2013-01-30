/*! \file Ix_Object.h
 *  \brief Define Ix_Object (the basic interface) and LWCLSID
 *  \author Zhang Yun Gui, X3 C++ PluginFramework
 *  \date   2011.6.30
 */
#ifndef X3_CORE_IOBJECT_H_
#define X3_CORE_IOBJECT_H_

#include <stdio.h>
#include <stdlib.h>
#include <LwDp.h>

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;

//! interface id of a class.
//! \see DEFINE_IID
typedef long_ X3IID;


  
namespace x3 {

inline long_ hashkey(const tchar_* str)
{
	long_ value = 0;
    while (*str)
		value = (value<<5) + value + *str++;
	return value;
}

} // x3

//! class unique id
/*! \ingroup _GROUP_PLUGIN_CORE_
    \code
    static const LWCLSID CLSID_ExampleTool("97380b78-a3b4-4784-ba07-c928561988c8");
    \endcode
*/
class LWCLSID
{
public:
    LWCLSID()
    {
        m_clsid[0] = 0;
    }

    LWCLSID(const tchar_* clsid)
    {
        Api_tcsncpy(m_clsid, 40, clsid ? clsid : __T(""), 40);
    }

    LWCLSID(const LWCLSID& src)
    {
        Api_tcsncpy(m_clsid, 40, src.m_clsid, 40);
    }

    LWCLSID& operator=(const tchar_* clsid)
    {
        Api_tcsncpy(m_clsid, 40, clsid ? clsid : __T(""), 40);
        return *this;
    }

    LWCLSID& operator=(const LWCLSID& src)
    {
        Api_tcsncpy((tchar_ *)m_clsid, 40, (tchar_ *)src.m_clsid, 40);
        return *this;
    }

    bool operator==(const LWCLSID& src) const
    {
        return Api_tcscmp((tchar_ *)m_clsid, (tchar_ *)src.m_clsid) == 0;
    }

    bool operator!=(const LWCLSID& src) const
    {
        return Api_tcscmp((tchar_ *)m_clsid, (tchar_ *)src.m_clsid) != 0;
    }

    bool operator>(const LWCLSID& src) const
    {
        return Api_tcscmp((tchar_ *)m_clsid, (tchar_ *)src.m_clsid) > 0;
    }

    bool operator<(const LWCLSID& src) const
    {
        return Api_tcscmp((tchar_ *)m_clsid, (tchar_ *)src.m_clsid) < 0;
    }

    const tchar_* str() const
    {
        return m_clsid;
    }

    bool empty() const
    {
        return  m_clsid[0] == 0 || 
				!Api_tcscmp((tchar_ *)m_clsid, (tchar_ *)__T("")) ||
				!Api_tcscmp((tchar_ *)m_clsid, (tchar_ *)__T("00000000-0000-0000-0000-000000000000"));
    }

    bool valid() const
    {
        return m_clsid[0] != 0;
    }

	void clear()
	{
		memset((char_ *)m_clsid, 0, sizeof(m_clsid));
	}
private:
    tchar_ m_clsid[40];
};

typedef LWCLSID MODID;

#define CLSID_DEFINE(clsid, str) \
    static const LWCLSID clsid(str); \
    typedef int32_ dumy_ ## clsid;

#define MODID_DEFINE(modid, str) \
    static const MODID modid(str); \
    typedef int32_ dumy_ ## modid;



//! Declare GetIID() in a interface derived from Ix_Object.
/*!
	\ingroup _GROUP_PLUGIN_CORE_
	\param _Interface interface name.
    \code
    class Ix_Example : public Ix_Object
    {
    public:
        DEFINE_IID(Ix_Example)
        virtual void foo() = 0;
    };
    \endcode
    \see X3BEGIN_CLASS_DECLARE
*/
#define DEFINE_IID(_Interface) \
	_Interface(){} \
    virtual ~_Interface() {}     \
    static X3IID GetIID() { static const X3IID id = x3::hashkey(__T(#_Interface)); return id; }

/*! \interface Ix_Object
 *  \ingroup _GROUP_PLUGIN_CORE2_
 *  \brief The basic interface that all X3 class can support it.
 *  \note  DO NOT call it's function directly.\n You can use Cx_Interface or Cx_Ptr instead of using it.
 *  \see autoptr class: Cx_Ptr, Cx_Interface
 *  \see DEFINE_IID, X3BEGIN_CLASS_DECLARE
 */
class Ix_Object
{
public:
	DEFINE_IID(Ix_Object);
		
    virtual long_ AddRef(const MODID& fromModule) = 0;
    virtual long_ Release(const MODID& fromModule) = 0;
    virtual bool QueryInterface(X3IID iid, Ix_Object** ppv, const MODID& fromModule) = 0;
    virtual const tchar_* GetClassName() const = 0;
};

EXTERN_C_END;
LWDP_NAMESPACE_END;

#endif // X3_CORE_IOBJECT_H_
