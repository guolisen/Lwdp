/*! \file   ACInterface.h
 *  \brief  Interface
 *  \author Guolisen, LwDp
 *  \date   2012.12.14
 */
 
#ifndef __AC_INTERFACE_H
#define __AC_INTERFACE_H


LWDP_NAMESPACE_BEGIN;

#define TS_CARD_ID_STRU_LEN      32
#define TS_SCENERY_ID_STRU_LEN   32

////////////////////////////////////////////
// Client Msg Code
////////////////////////////////////////////

#define TS_GATECHECK_MSG_BASE 0x01000000 //Module:0x01 Code0x000000

enum TS_GATECHECK_MSG_ENUM
{
	//ACDevice
	TS_DEVICE_INIT_REQ_MSG = TS_GATECHECK_MSG_BASE,
	TS_SERVER_INIT_RSP_MSG,
	TS_DEVICE_CONFIG_REQ_MSG,
	TS_SERVER_CONFIG_RSP_MSG,
	TS_DEVICE_HEART_BEAT_REQ_MSG,
	TS_SERVER_HEART_BEAT_RSP_MSG,
	TS_DEVICE_CARD_DATA_REQ_MSG,
	TS_SERVER_CARD_DATA_RSP_MSG,
	TS_DEVICE_BULK_DATA_REQ_MSG,
	TS_SERVER_BULK_DATA_RSP_MSG
};


////////////////////////////////////////////
// Client Resault Msg Code
////////////////////////////////////////////

enum TS_RET_MSG_RESAULT_ENUM
{	
	TS_SERVER_CHECK_OK_RECONFIG = 1, //��֤�ɹ��������������
	TS_SERVER_ID_ERROR,     //�豸IDδ֪����
	TS_SERVER_TYPE_ERROR,   //�豸���ʹ���
	TS_SERVER_UNKNOW_ERROR, //δ֪���󣬾����������Ϣ

	TS_SERVER_DB_ERR,        //���������ݿ����
	TS_SERVER_CONFIG_ERROR, //ʧ��

	TS_SERVER_BULK_THREAD_ERR,
	TS_SERVER_HB_ERROR,

	TS_SERVER_WRITE_DATA_ERROR,

	TS_SERVER_BULK_WRITE_DATA_ERROR
};

////////////////////////////////////////////
// �Ž�������ʼ����Ϣ
////////////////////////////////////////////
typedef struct stru_device_init_req_body
{	
	uint32_ deviceType;  //�豸����
	char_   sceneryId[TS_SCENERY_ID_STRU_LEN];  //����ID
	uint32_ checkResult; //�豸�Լ���
	uint32_ position;  //
}TS_DEVICE_INIT_REQ_BODY;

typedef struct stru_server_init_rsp_body
{	
	uint32_  msgResult; //��Ϣ���ͽ��
	char_    msgResultData[32];  // ���ܵĴ�����Ϣ�ַ���
	uint32_  appendDataLength; //�������ݳ���
	uint8_*  appendData;
}TS_SERVER_INIT_RSP_BODY;


////////////////////////////////////////////
// �豸����������Ϣ
////////////////////////////////////////////

typedef struct stru_dev_config_body
{	
	uint32_  msgResult; //��Ϣ���ͽ��
	char_    msgResultData[32];  // ���ܵĴ�����Ϣ�ַ���
	char_    deviceId[TS_GATE_ID_STRU_LEN];  //�豸����
	uint32_  deviceType;  //�豸����
	char_    sceneryId[TS_SCENERY_ID_STRU_LEN];  //����ID
	//uint8_   sceneryDomainId[8];  //����ID
	uint32_  sceneryPostion;  //ƫ��
}TS_DEV_CONFIG_RSP_BODY;


////////////////////////////////////////////
// ״̬�����Ϣ
////////////////////////////////////////////
typedef struct stru_dev_status_req_body
{	
	uint32_ statusCode;  //״̬��
}TS_DEV_STATUS_REQ_BODY;

typedef struct stru_dev_status_rsp_body
{	
	uint32_  msgResult; //��Ϣ���ͽ��
	char_    msgResultData[32];  // ���ܵĴ�����Ϣ�ַ���
	uint32_  oprationCode;  //������
	uint32_  appendDataLength; //�������ݳ���
	uint8_   appendData[0];
}TS_DEV_STATUS_RSP_BODY;

////////////////////////////////////////////
// ˢ��������Ϣ
////////////////////////////////////////////

typedef struct stru_dev_card_data_msg
{
	char_    cardId[TS_CARD_ID_STRU_LEN];  
    char_    sceneryId[TS_SCENERY_ID_STRU_LEN];
	uint16_  cardType;   //������
	uint16_  actionId;   //��ΪID
	uint32_  checkinTime;
}TS_DEVICE_CARD_DATA_REQ_BODY;

typedef struct stru_dev_card_data_rsp_msg
{	
	uint32_  msgResult; //��Ϣ���ͽ��
	char_    msgResultData[32];  // ���ܵĴ�����Ϣ�ַ���
}TS_DEVICE_CARD_DATA_RSP_BODY;



////////////////////////////////////////////
// ˢ�����������ϱ���Ϣ
////////////////////////////////////////////

typedef struct stru_dev_bulk_data_msg
{  
	uint32_  cardDataCount;   //��������Ŀ����
	uint8_   cardDataEntry[0];   //��ΪID
}TS_DEVICE_BULK_DATA_REQ_BODY;

typedef struct stru_dev_bulk_data_rsp_msg
{	
	uint32_  msgResult; //��Ϣ���ͽ��
	char_    msgResultData[32];  // ���ܵĴ�����Ϣ�ַ���
	uint32_  errorEntryNum;
	uint8_   errCardId[0]; //errorEntryNum * 8
} TS_DEVICE_BULK_DATA_RSP_BODY;

LWDP_NAMESPACE_END;

#endif











