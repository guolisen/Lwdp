#include <Lwdp.h>
#include <PluginInc.h>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>

#include "../Interface/TSFrontend/Ix_TSFrontend.h"

#include "Cx_TSFrontend.h"

Cx_TSFrontend::Cx_TSFrontend()
{
	printf("TSFrontend Create!\n");
}

Cx_TSFrontend::~Cx_TSFrontend()
{
	printf("TSFrontend Delete!\n");
}




void Cx_TSFrontend::Foo(long_& value)
{

}


