#include <Lwdp.h>
#include <PluginInc.h>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>

#include "../Interface/ZmqBackend/Ix_ZmqBackend.h"

#include "Cx_ZmqBackend.h"

Cx_ZmqBackend::Cx_ZmqBackend()
{
	printf("ZmqBackend Create!\n");
}

Cx_ZmqBackend::~Cx_ZmqBackend()
{
	printf("ZmqBackend Delete!\n");
}



LWRESULT Cx_ZmqBackend::Init()
{

	return LWDP_OK;
}
LWRESULT Cx_ZmqBackend::RunServer()
{

	return LWDP_OK;
}


