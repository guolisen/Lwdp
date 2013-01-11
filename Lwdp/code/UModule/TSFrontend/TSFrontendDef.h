/*! \file   TSFrontendDef.h
 *  \brief  TSFrontendDef Tag Define
 *  \author Guolisen, Lwdp PluginFramework
 *  \date   2012.02.03
 */
#ifndef LWDP_TSFRONTEND_DEFINE_H_
#define LWDP_TSFRONTEND_DEFINE_H_

#define LW_TSFRONTEND_SERVER_PORT  	12135
#define LW_TSFRONTEND_BLOCK  	    128
#define LW_TSFRONTEND_RECV_MAX_LEN  1024
#define LW_TSFRONTEND_SEND_TIMEOUT  5
#define LW_TSFRONTEND_RECV_TIMEOUT  5
#define LW_TSFRONTEND_CONNECT_TARGET  __T("tcp://localhost:5559")
#define LW_TSFRONTEND_RECV_BUFFER_LEN 4096 * 2

#define LW_TSFRONTEND_COMMAND_GET_INFO_NAME  __T("getTsInfo")
#define LW_TSFRONTEND_COMMAND_GET_INFO_INFO  __T("Get Tcp Server Info")

#define LW_TSFRONTEND_MODULE_NAME  			__T("TSFrontend")

#define LW_TSFRONTEND_MODULE_PORT_NAME  	__T("ServerPort")
#define LW_TSFRONTEND_MODULE_BLOCK_NAME  	__T("ListenBlock")
#define LW_TSFRONTEND_BUF_MAX_LEN_NAME  	__T("RecvBufMaxLen")
#define LW_TSFRONTEND_SEND_TIMEOUT_NAME  	__T("SendZmqTimeout")
#define LW_TSFRONTEND_RECV_TIMEOUT_NAME  	__T("RecvZmqTimeout")
#define LW_TSFRONTEND_CONNECT_TARGET_NAME  	__T("ConnetTarget")


#endif // LWDP_CONFIG_TAG_DEFINE_H_





