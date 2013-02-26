/*! \file   ExternalInterface.h
 *  \brief  Interface
 *  \author Guolisen, LwDp
 *  \date   2012.12.14
 */
 
#ifndef __EXTERNAL_INTERFACE_H
#define __EXTERNAL_INTERFACE_H


LWDP_NAMESPACE_BEGIN;

#define TS_GATE_ID_STRU_LEN      32
#define TS_RET_MSG_LEN           128

//Module Define

#define TS_SYSTEM_MODULE   0
#define TS_ACDEVICE_MODULE 1

#define GET_MSG_BASE(module_num) ((module_num) << 24)  //Module:module_num Code0x000000

// Input
//-----------------------------------------------------------
// Req Msg Head
typedef struct stru_req_server_msg
{
	uint32_ msgLength;  
	char_   deviceId[TS_GATE_ID_STRU_LEN];
	uint32_ msgCode;
	uint8_  customMsgBody[0];
}TS_REQ_SERVER_MSG;


// Output
//-----------------------------------------------------------
// Rsp Msg Head

typedef struct stru_rsp_server_msg
{
	uint32_ msgLength;  
	uint32_ rspCode;
	char_   rspMsg[TS_RET_MSG_LEN]; 
	uint8_  customMsgBody[0];
}TS_RSP_SERVER_MSG;


// SERVER RSP MSG CODE
#define TS_SYSTEM_MSG_BASE   GET_MSG_BASE(TS_SYSTEM_MODULE)   //0x00000000L Module:0x00 Code0x000000
enum TS_SYSTEM_MSG_ENUM
{
	TS_SERVER_OK = TS_SYSTEM_MSG_BASE,
		
	TS_SERVER_TCP_MSG_LEN_ERROR,
	TS_SERVER_TCP2ZMQ_SEND_ERROR,
	TS_SERVER_ZMQ2TCP_RECV_TIMEOUT,
	TS_SERVER_UNKNOW_MSG,
	TS_SERVER_MSG_BODY_ERR, 
};



LWDP_NAMESPACE_END;

#endif











