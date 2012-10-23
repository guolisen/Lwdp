/*! \file Ix_PluginMgrModule.h
 *  \brief 
 *  \author Guolisen, Lwdp PluginFramework
 *  \date   2011.10.24
 */
#ifndef LWDP_PLUGIN_MANAGER_MODULE_H_
#define LWDP_PLUGIN_MANAGER_MODULE_H_

#include "Id_PluginLoader.h"

LWDP_NAMESPACE_BEGIN;

class Ix_ObjectFactory;
class Ix_PluginMgrModule
{
public:
	virtual Ix_ObjectFactory* GetRegisterBank() = 0;

};

LWDP_NAMESPACE_END;

#endif // LWDP_PLUGIN_MANAGER_MODULE_H_

