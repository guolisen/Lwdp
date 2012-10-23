/*! \file Cx_Object.h
 *  \brief Define the regular implement template class: Cx_Object<cls>
 *  \author Zhang Yun Gui, X3 C++ PluginFramework
 *  \date   2011.06.30
 */
#ifndef X3_PLUGINIMPL_OBJECT_H_
#define X3_PLUGINIMPL_OBJECT_H_

//#include "Ix_Object.h"

LWDP_NAMESPACE_BEGIN;



extern "C" const MODID& GetModuleModId();

/*! \ingroup _GROUP_PLUGIN_CORE2_
 *  \brief The regular implement template class used by XDEFINE_CLASSMAP_ENTRY.
 *  \note  The template parameter is a class which implement a interface.
 *  \see   Cx_SingletonObject, XDEFINE_CLASSMAP_ENTRY
 *  \internal
 */
template <class ClsType>
class Cx_Object : public ClsType
{
protected:
    Cx_Object() : m_refcount(1)
    {
        Api_InterlockedIncrement(&ObjectCount());
    }

    virtual ~Cx_Object()
    {
        Api_InterlockedDecrement(&ObjectCount());
    }

protected:
    virtual long_ AddRef(const MODID& fromModule)
    {
        if (fromModule != GetModuleModId())
        {
            Api_InterlockedIncrement(&RefCountByOthers());
        }
        return Api_InterlockedIncrement(&m_refcount);
    }

    virtual long_ Release(const MODID& fromModule)
    {
        if (fromModule != GetModuleModId())
        {
            Api_InterlockedDecrement(&RefCountByOthers());
        }

        long_ ret = Api_InterlockedDecrement(&m_refcount);

        if (0 == ret)
        {
            delete this;
        }

        return ret;
    }

    virtual bool QueryInterface(X3IID iid, Ix_Object** ppv, const MODID& fromModule)
    {
        return ClsType::DoQueryInterface(this, iid, ppv, fromModule);
    }

    virtual const tchar_* GetClassName() const
    {
        return ClsType::DoGetClassName();
    }

public:
    static Ix_Object* CreateObject(X3IID iid, const MODID& fromModule)
    {
        Ix_Object* ret = NULL;
        Cx_Object<ClsType>* p = new Cx_Object<ClsType>();

        p->QueryInterface(iid, &ret, fromModule);
        p->Release(fromModule);

        return ret;
    }

    static long_ GetObjectCount()
    {
        return ObjectCount();
    }

    static long_ GetRefCountByOthers()
    {
        return RefCountByOthers();
    }

private:
    Cx_Object(const Cx_Object&);
    void operator=(const Cx_Object&);

    long_        m_refcount;

    static long_& ObjectCount()
    {
        static long_ s_objcount = 0;
        return s_objcount;
    }

    static long_& RefCountByOthers()
    {
        static long_ s_refcount = 0;
        return s_refcount;
    }
};


LWDP_NAMESPACE_END;

#endif // X3_PLUGINIMPL_OBJECT_H_
