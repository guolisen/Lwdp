/*! \file   ZmqBackendDef.h
 *  \brief  ZmqBackendDef Tag Define
 *  \author Guolisen, Lwdp PluginFramework
 *  \date   2012.02.03
 */
#ifndef LWDP_ZMQBACKEND_DEFINE_H_
#define LWDP_ZMQBACKEND_DEFINE_H_

#define LW_ZMQBACKEND_FRONTEND_TARGET 		__T("tcp://*:12136")
#define LW_ZMQBACKEND_BACKEND_TARGET   		__T("tcp://*:12137")
#define LW_ZMQBACKEND_WORKTHREAD_TARGET   	__T("tcp://localhost:12137")
#define LW_ZMQBACKEND_WORKTHREAD_NUM   		5
#define LW_ZMQBACKEND_CTRL_CLIENT_TARGET   	__T("inproc://ctrl_to_workthread")


#define LW_ZMQBACKEND_DB_IP_DEFAULT 		__T("127.0.0.1")
#define LW_ZMQBACKEND_DB_USER_DEFAULT   	__T("root")
#define LW_ZMQBACKEND_DB_PASSWORD        __T("lifesgood")
#define LW_ZMQBACKEND_DB_SELECT_DBNAME   __T("test")
#define LW_ZMQBACKEND_DB_PORT_DEFAULT  	3306 

 
#define LW_ZMQBACKEND_MODULE_NAME  			__T("ZmqBackend")

#define LW_ZMQBACKEND_FRONTEND_TARGET_NAME  __T("FrontendTarget")
#define LW_ZMQBACKEND_BACKEND_TARGET_NAME  	__T("BackendTarget")
#define LW_ZMQBACKEND_WORKTHREAD_NUM_NAME  	__T("WorkThreadNum")
#define LW_ZMQBACKEND_WORKTHREAD_TARGET_NAME  	__T("WorkThreadConnect")
#define LW_ZMQBACKEND_CTRL_CLIENT_NAME  	__T("CtrlConnect")


#define LW_ZMQBACKEND_COMMAND_SENDTOWORKER_NAME __T("sendToWork")
#define LW_ZMQBACKEND_COMMAND_SENDTOWORKER_INFO __T("Command to Backend WorkThread")

#define LW_ZMQBACKEND_DB_IP_NAME  		__T("DbIp")
#define LW_ZMQBACKEND_DB_USER_NAME  		__T("DbUser")
#define LW_ZMQBACKEND_DB_PASSWORD_NAME  	__T("DbPassword")
#define LW_ZMQBACKEND_DB_SELECT_DB_NAME  __T("DbSelect")
#define LW_ZMQBACKEND_DB_PORT_NAME  		__T("DbPort")
#endif // LWDP_CONFIG_TAG_DEFINE_H_





