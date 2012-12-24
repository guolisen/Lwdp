/*! \file   ExternalInterface.h
 *  \brief  Interface
 *  \author Guolisen, LwDp
 *  \date   2012.12.14
 */
 
#ifndef __EXTERNAL_INTERFACE_H
#define __EXTERNAL_INTERFACE_H


LWDP_NAMESPACE_BEGIN;

#define TS_GATE_ID_STRU_LEN      32

////////////////////////////////////////////
// Tcp Msg Body
////////////////////////////////////////////
enum TCP_SERVER_RETURN_CODE
{
	TS_SERVER_TCP_MSG_LEN_ERROR = 1,  //结构体中消息长度字段数值大于实际接收数据长度
	TS_SERVER_RECV_TIMEOUT_ERROR
};

typedef struct stru_tcp_server_msg
{
	uint32_ msgLength;  //消息体长度  此长度为整个消息长度，包括msgLength本身的长度
	uint32_ statusCode; //消息TcpServer错误返回码
	uint8_  tcpMsgBody[0];  //消息体
}TS_TCP_SERVER_MSG;



////////////////////////////////////////////
// Zmq Msg Body
////////////////////////////////////////////
typedef struct stru_zmq_server_msg
{
	char_   deviceId[TS_GATE_ID_STRU_LEN];
	uint32_ msgCode;
	uint8_  customMsgBody[0];  //消息体
}TS_ZMQ_SERVER_MSG;


// SERVER ERROR MSG CODE
#define TS_SYSTEM_MSG_BASE    0x00000000 //Module:0x00 Code0x000000
enum TS_SYSTEM_MSG_ENUM
{
	TS_SERVER_OK = TS_SYSTEM_MSG_BASE,
	TS_SERVER_UNKNOW_MSG, //客户端发送的消息为未知内容
	TS_SERVER_MSG_BODY_ERR, //客户端发送的消息为未知内容
	
};

typedef struct stru_server_err_body
{	
	uint32_  errMsgCode; //消息发送结果
	char_    errData[128];  // 可能的错误信息字符串
}TS_SERVER_ERROR_BODY;


LWDP_NAMESPACE_END;

#endif











