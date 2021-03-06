/*! \file XModuleItem.h
 *  \brief Define the base class of single instance classes. (for internal use only)
 *  \author Zhang Yun Gui, X3 C++ PluginFramework
 *  \date   2010.10.19
 */
#ifndef X3_PLUGINIMPL_MODULEITEM_H_
#define X3_PLUGINIMPL_MODULEITEM_H_
#include <LwDp.h>

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;

  
/*! \ingroup _GROUP_PLUGIN_CORE2_
 *  \brief the base class of single instance classes.
 *  \internal
 */
class Cx_ModuleItem
{
protected:
    Cx_ModuleItem() : m_index(-1)
    {
    }
    virtual ~Cx_ModuleItem()
    {
    }

    //! Add self to the single instance stack.
    void AddModuleItem()
    {
        m_index = Api_InterlockedDecrement(&Index());   // support concurrent insert.
        if (m_index >= 0)
        {
            Items()[m_index] = this;                // only one thread can do it.
        }
    }

public:
    //! Free all single instance objects when this plugin is unloading.
    static void ClearModuleItems()
    {
        if (Items())
        {
            for (int32_ i = Index(); i < MaxCount(); i++)
            {
                if (i >= 0 && Items()[i] )
                {
                    delete Items()[i];
                    Items()[i] = NULL;
                }
            }
            Index() = 0;

            delete[] Items();
            Items() = NULL;
        }
    }

    //! Init the single instance stack. x3GetModuleInterface will call it.
    static void InitModuleItems(long_ nCount)
    {
        if (NULL == Items() && nCount > 0)
        {
            MaxCount() = (nCount > MaxCount()) ? nCount : MaxCount();
            Index() = MaxCount();
            Items() = new Cx_ModuleItem*[MaxCount()];
        }
    }

private:
    long_    m_index;    // index in the single instance stack.

    static long_& MaxCount()
    {
        static long_ s_count = 0;
        return s_count;
    }
    static long_& Index()
    {
        static long_ s_index = 0;
        return s_index;
    }
    static Cx_ModuleItem**& Items()
    {
        static Cx_ModuleItem** s_pItems = NULL;
        return s_pItems;
    }
};


EXTERN_C_END;
LWDP_NAMESPACE_END;


#endif // X3_PLUGINIMPL_MODULEITEM_H_
