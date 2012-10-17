/*! \file Ix_ObjectFactory.h
 *  \brief Define object creator interface: Ix_ObjectFactory
 *  \author Zhang Yun Gui, X3 C++ PluginFramework
 *  \date   2011.06.30
 */
#ifndef X3_CORE_IOBJECTFACTORY_H_
#define X3_CORE_IOBJECTFACTORY_H_

#include <Interface/Ix_Object.h>
#include "ID_PluginLoader.h"

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;
	
/*! \ingroup _GROUP_PLUGIN_CORE_
 *  \interface Ix_ObjectFactory
 *  \brief Object creator interface
 *  \see   Ix_Object, Ix_Module
 */
class Ix_ObjectFactory : public Ix_Object
{
public:
    DEFINE_IID(Ix_ObjectFactory);

    //! Check if the class is loaded.
    virtual bool IsCreatorRegister(const LWCLSID& clsid) = 0;

    //! Create an object with the class id.
    virtual Ix_Object* CreateObject(const LWCLSID& clsid, X3IID iid, Ix_Object** ppv, const MODID& fromModule) = 0;
};

EXTERN_C_END;
LWDP_NAMESPACE_END;

#endif // X3_CORE_IOBJECTFACTORY_H_
