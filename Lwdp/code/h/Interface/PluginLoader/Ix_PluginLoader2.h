/*! \file Ix_PluginLoader2.h
 *  \brief Define plugin loading interface: Ix_PluginLoader2
 *  \author Zhang Yun Gui, X3 C++ PluginFramework
 *  \date   2011.06.30
 */
#ifndef X3_MANAGER_IPLUGINLOADER2_H_
#define X3_MANAGER_IPLUGINLOADER2_H_

#include <Interface/Ix_Object.h>
#include "Id_PluginLoader.h"
 
LWDP_NAMESPACE_BEGIN;

/*! \ingroup _GROUP_PLUGIN_CORE2_
 *  \interface Ix_PluginLoader2
 *  \brief Plugin loading interface
 *  \see   Ix_PluginLoader, Ix_ObjectFactory, x3GetObjectFactory()
 */
class Ix_PluginLoader2 : public Ix_Object
{
public:
    DEFINE_IID(Ix_PluginLoader2)

    virtual long_ GetPluginCount() = 0;
    virtual bool GetPluginFileName(long_ index, MODULEID& hdll, std::string& filename) = 0;
};

LWDP_NAMESPACE_END;
#endif // X3_MANAGER_IPLUGINLOADER2_H_
