/*! \file Ix_PluginDelayLoad.h
 *  \brief Define plugin delay-loading interface: Ix_PluginDelayLoad
 *  \author Zhang Yun Gui, X3 C++ PluginFramework
 *  \date   2011.02.08
 */
#ifndef X3_MANAGER_IPLUGINDELAYLOAD_H_
#define X3_MANAGER_IPLUGINDELAYLOAD_H_


#include <Interface/Ix_Object.h>
#include "ID_PluginLoader.h"

LWDP_NAMESPACE_BEGIN;


/*! \ingroup _GROUP_PLUGIN_CORE_
 *  \interface Ix_PluginDelayLoad
 *  \brief Plugin delay-loading interface.
 *  \see   x3::CLSID_PluginDelayLoad
 */
class Ix_PluginDelayLoad : public Ix_Object
{
public:
    DEFINE_IID(Ix_PluginDelayLoad)

    //! Register a plugin as observer owner.
    virtual void AddObserverPlugin(MODULEID hdll, const tchar_* obtype) = 0;

    //! Load observer plugins when the event is first fired.
    virtual void FireFirstEvent(const tchar_* obtype) = 0;
};

LWDP_NAMESPACE_END;
#endif // X3_MANAGER_IPLUGINDELAYLOAD_H_
