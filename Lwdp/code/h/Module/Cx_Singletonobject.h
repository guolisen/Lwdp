/*! \file Cx_SingletonObject.h
 *  \brief Define single instance implement template class: Cx_SingletonObject<cls>
 *  \author Zhang Yun Gui, X3 C++ PluginFramework
 *  \date   2011.06.30
 */
#ifndef X3_CX_SINGLETONOBJECT_H_
#define X3_CX_SINGLETONOBJECT_H_

#include <Interface/Ix_Object.h>
#include <Module/XModuleItem.h>

LWDP_NAMESPACE_BEGIN;
 
extern "C" const MODID& GetModuleModId();

/*! \ingroup _GROUP_PLUGIN_CORE2_
 *  \brief Single instance implement template class used by XDEFINE_CLASSMAP_ENTRY_Singleton.
 *  \brief The template parameter is a class which implement a interface.
 *  \internal
 *  \see   Cx_Object, XDEFINE_CLASSMAP_ENTRY_Singleton
 */
template <class ClsType>
class Cx_SingletonObject
    : public ClsType
    , public Cx_ModuleItem
{
protected:
    Cx_SingletonObject(bool bRef = true) : m_refcount(bRef ? 1 : 0)
    {
    }

    virtual ~Cx_SingletonObject()
    {
        if (Instance() == this)
        {
            Instance() = NULL;
        }
    }

protected:
    virtual long_ AddRef(const MODID& from_module)
    {
        if (from_module != GetModuleModId())
        {
            Api_InterlockedIncrement(&RefCountByOthers());
        }
        return Api_InterlockedIncrement(&m_refcount);
    }

    virtual long_ Release(const MODID& from_module)
    {
        if (from_module != GetModuleModId())
        {
            Api_InterlockedDecrement(&RefCountByOthers());
        }
        return Api_InterlockedDecrement(&m_refcount);
    }

    virtual bool QueryInterface(X3IID iid, Ix_Object** ppv, const MODID& from_module)
    {
        return ClsType::DoQueryInterface(this, iid, ppv, from_module);
    }

    virtual const tchar_* GetClassName() const
    {
        return ClsType::DoGetClassName();
    }

public:
    static Ix_Object* CreateObject(X3IID iid, const MODID& from_module)
    {
        if (NULL == Instance())
        {
            Cx_SingletonObject<ClsType>* p = new Cx_SingletonObject<ClsType>(false);

            if (1 == Api_InterlockedIncrement(&Locker()))
            {
                Instance() = p;
                p->AddRef(from_module);
                p->AddModuleItem();
            }
            else
            {
                delete p;   // has created by another thread
            }
            Api_InterlockedDecrement(&Locker());
        }

        Ix_Object* ret = NULL;
        Instance()->QueryInterface(iid, &ret, from_module);

        return ret;
    }

    static long_ GetObjectCount()
    {
        return (Instance() && Instance()->m_refcount > 0) ? 1 : 0;
    }

    static long_ GetRefCountByOthers()
    {
        return RefCountByOthers();
    }

private:
    Cx_SingletonObject(const Cx_SingletonObject&);
    void operator=(const Cx_SingletonObject&);

    long_        m_refcount;

    static Cx_SingletonObject<ClsType>*& Instance()
    {
        static Cx_SingletonObject<ClsType>* s_obj = NULL;
        return s_obj;
    }

    static long_& RefCountByOthers()
    {
        static long_ s_refcount = 0;
        return s_refcount;
    }

    static long_& Locker()
    {
        static long_ s_locker = 0;
        return s_locker;
    }
};

LWDP_NAMESPACE_END;

#endif // X3_CX_SINGLETONOBJECT_H_
