#include <Lwdp.h>
#include <PluginInc.h>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>
#include <Interface/DbMgr/Ix_DbMgr.h>
#include <Interface/TimerMgr/Ix_TimerMgr.h>

#include "../Interface/TcpServer/Ix_TcpServer.h"
#include "../Interface/TSFrontend/Ix_TSFrontend.h"
#include "../Interface/ZmqBackend/Ix_ZmqBackend.h"
#include "../Interface/ACDevice/Ix_ACDevice.h"

#include "TcpServerDef.h"
#include "TcpServerErrno.h"
#include "Cx_TcpServer.h"

Cx_TcpServer::Cx_TcpServer()
{
	printf("TcpServer Create!\n");
}

Cx_TcpServer::~Cx_TcpServer()
{
	printf("TcpServer Delete!\n");
}

void* thread_run_frondend(void* arg)
{
	GET_OBJECT_RET(TSFrontend, iTSFrontend, 0);
	RINOKR(iTSFrontend->RunServer(), 0);

	return NULL;
}
void* thread_run_backend(void* arg)
{
	GET_OBJECT_RET(ZmqBackend, iZmqBackend, 0);
	RINOKR(iZmqBackend->RunServer(), 0);
	
	return NULL;
}

LWRESULT Cx_TcpServer::Init()
{
	GET_OBJECT_RET(TSFrontend, iTSFrontend, LWDP_GET_OBJECT_ERROR);
	RINOK(iTSFrontend->Init());

	GET_OBJECT_RET(ZmqBackend, iZmqBackend, LWDP_GET_OBJECT_ERROR);
	RINOK(iZmqBackend->Init());

	GET_OBJECT_RET(ConfigMgr, iConfigMgr, LWDP_GET_OBJECT_ERROR);

	std::string strDbIp 		= std::string(LW_TCPSERVER_DB_IP_DEFAULT);
	std::string strDbUserName 	= std::string(LW_TCPSERVER_DB_USER_DEFAULT);
	std::string strDbPassword 	= std::string(LW_TCPSERVER_DB_PASSWORD);
	std::string strDbName 		= std::string(LW_TCPSERVER_DB_SELECT_DBNAME);
	uint32_     DbPort 		    = LW_TCPSERVER_DB_PORT_DEFAULT;
	
	XPropertys propDbIp;
	iConfigMgr->GetModulePropEntry(LW_TCPSERVER_MODULE_NAME, LW_TCPSERVER_DB_IP_NAME, propDbIp);
	if(!propDbIp[0].propertyText.empty())
	{
		strDbIp = propDbIp[0].propertyText;
	}
	else
	{
		LWDP_LOG_PRINT("TCPSERVER", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <DbIp> In Config File, Default(%s)", strDbIp.c_str());
	}

	XPropertys propDbUserName;
	iConfigMgr->GetModulePropEntry(LW_TCPSERVER_MODULE_NAME, LW_TCPSERVER_DB_USER_NAME, propDbUserName);
	if(!propDbUserName[0].propertyText.empty())
	{
		strDbUserName = propDbUserName[0].propertyText;
	}
	else
	{
		LWDP_LOG_PRINT("TCPSERVER", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <DbUser> In Config File, Default(%s)", strDbUserName.c_str());
	}

	XPropertys propDbPassword;
	iConfigMgr->GetModulePropEntry(LW_TCPSERVER_MODULE_NAME, LW_TCPSERVER_DB_PASSWORD_NAME, propDbPassword);
	if(!propDbPassword[0].propertyText.empty())
	{
		strDbPassword = propDbPassword[0].propertyText;
	}
	else
	{
		LWDP_LOG_PRINT("TCPSERVER", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <DbPassword> In Config File, Default(%s)", strDbPassword.c_str());
	}

	XPropertys propDbName;
	iConfigMgr->GetModulePropEntry(LW_TCPSERVER_MODULE_NAME, LW_TCPSERVER_DB_SELECT_DB_NAME, propDbName);
	if(!propDbName[0].propertyText.empty())
	{
		strDbName = propDbName[0].propertyText;
	}
	else
	{
		LWDP_LOG_PRINT("TCPSERVER", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <DbName> In Config File, Default(%s)", strDbName.c_str());
	}

	XPropertys propDbPort;
	iConfigMgr->GetModulePropEntry(LW_TCPSERVER_MODULE_NAME, LW_TCPSERVER_DB_PORT_NAME, propDbPort);
	if(!propDbPort[0].propertyText.empty())
	{
		DbPort = atol(propDbPort[0].propertyText.c_str());
	}
	else
	{
		LWDP_LOG_PRINT("TCPSERVER", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <DbPort> In Config File, Default(%d)", DbPort);
	}	

	GET_OBJECT_RET(DbMgr, iDbMgr, 0);	
	RINOK(iDbMgr->Open(strDbIp.c_str(), strDbUserName.c_str(), strDbPassword.c_str(), strDbName.c_str(), DbPort, 0));

	GET_OBJECT_RET(ACDevice, iACDevice, LWDP_GET_OBJECT_ERROR);
	RINOK(iACDevice->Init());
	
	return LWDP_OK;
}
LWRESULT Cx_TcpServer::RunServer()
{
	pthread_t t;
	int result;

	//Frontend
	result = pthread_create(&t, NULL, thread_run_frondend, NULL);
	if(result != 0){
		LWDP_LOG_PRINT("TCPSERVER", LWDP_LOG_MGR::ERR, 
					   "Can't Create Thread Ret: %d\n", result);
		return TCPSERVER::LWDP_CREATE_WORK_THREAD_ERR;
	}

	result = pthread_detach(t);
	if(result != 0){
		LWDP_LOG_PRINT("TCPSERVER", LWDP_LOG_MGR::ERR, 
					   "Can't Detach Thread Ret: %d\n", result);
		return TCPSERVER::LWDP_CREATE_DETACH_THREAD_ERR;
	}

	//Backend
	result = pthread_create(&t, NULL, thread_run_backend, NULL);
	if(result != 0){
		LWDP_LOG_PRINT("TCPSERVER", LWDP_LOG_MGR::ERR, 
					   "Can't Create Thread Ret: %d\n", result);
		return TCPSERVER::LWDP_CREATE_WORK_THREAD_ERR;
	}

	result = pthread_detach(t);
	if(result != 0){
		LWDP_LOG_PRINT("TCPSERVER", LWDP_LOG_MGR::ERR, 
					   "Can't Detach Thread Ret: %d\n", result);
		return TCPSERVER::LWDP_CREATE_DETACH_THREAD_ERR;
	}
	
	return LWDP_OK;
}

LWRESULT Cx_TcpServer::DestoryServer()
{

	return LWDP_OK;
}

