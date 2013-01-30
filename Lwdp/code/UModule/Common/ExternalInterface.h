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
	TS_SERVER_TCP_MSG_LEN_ERROR = 1,  //�ṹ������Ϣ�����ֶ���ֵ����ʵ�ʽ�����ݳ���
	TS_SERVER_RECV_TIMEOUT_ERROR
};

typedef struct stru_tcp_server_msg
{
	uint32_ msgLength;  //��Ϣ�峤��  �˳���Ϊ�����Ϣ���ȣ�����msgLength����ĳ���
	uint32_ statusCode; //��ϢTcpServer���󷵻���
	uint8_  tcpMsgBody[0];  //��Ϣ��
}TS_TCP_SERVER_MSG;



////////////////////////////////////////////
// Zmq Msg Body
////////////////////////////////////////////
typedef struct stru_zmq_server_msg
{
	char_   deviceId[TS_GATE_ID_STRU_LEN];
	uint32_ msgCode;
	uint8_  customMsgBody[0];  //��Ϣ��
}TS_ZMQ_SERVER_MSG;


// SERVER ERROR MSG CODE
#define TS_SYSTEM_MSG_BASE    0x00000000L //Module:0x00 Code0x000000
enum TS_SYSTEM_MSG_ENUM
{
	TS_SERVER_OK = TS_SYSTEM_MSG_BASE,
	TS_SERVER_UNKNOW_MSG, //�ͻ��˷��͵���ϢΪδ֪����
	TS_SERVER_MSG_BODY_ERR //�ͻ��˷��͵���ϢΪδ֪����
	
};

typedef struct stru_server_err_body
{	
	uint32_  errMsgCode; //��Ϣ���ͽ��
	char_    errData[128];  // ���ܵĴ�����Ϣ�ַ�
}TS_SERVER_ERROR_BODY;


LWDP_NAMESPACE_END;

#endif











