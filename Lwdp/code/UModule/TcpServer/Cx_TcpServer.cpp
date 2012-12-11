#include <Lwdp.h>
#include <PluginInc.h>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>

#include "../Interface/TcpServer/Ix_TcpServer.h"

#include "Cx_TcpServer.h"

Cx_TcpServer::Cx_TcpServer()
{
	printf("TcpServer Create!\n");
}

Cx_TcpServer::~Cx_TcpServer()
{
	printf("TcpServer Delete!\n");
}



LWRESULT Cx_TcpServer::Init()
{

	return LWDP_OK;
}
LWRESULT Cx_TcpServer::RunServer()
{

	return LWDP_OK;
}


