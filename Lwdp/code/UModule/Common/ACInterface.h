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
	TS_SERVER_BULK_DATA_RSP_MSG,
};


////////////////////////////////////////////
// Client Resault Msg Code
////////////////////////////////////////////

enum TS_RET_MSG_RESAULT_ENUM
{	
	TS_SERVER_CHECK_OK_RECONFIG = 1, //验证成功，但须重配参数
	TS_SERVER_ID_ERROR,     //设备ID未知错误
	TS_SERVER_TYPE_ERROR,   //设备类型错误
	TS_SERVER_UNKNOW_ERROR, //未知错误，具体见附加信息

	TS_SERVER_DB_ERR,        //服务器数据库错误
	TS_SERVER_CONFIG_ERROR, //失败

	TS_SERVER_BULK_THREAD_ERR,
	TS_SERVER_HB_ERROR,

	TS_SERVER_WRITE_DATA_ERROR,

	TS_SERVER_BULK_WRITE_DATA_ERROR
};

////////////////////////////////////////////
// 门禁启动初始化消息
////////////////////////////////////////////
typedef struct stru_device_init_req_body
{	
	uint32_ deviceType;  //设备类型
	char_   sceneryId[TS_SCENERY_ID_STRU_LEN];  //景点ID
	uint32_ checkResult; //设备自检结果
	uint32_ position;  //
}TS_DEVICE_INIT_REQ_BODY;

typedef struct stru_server_init_rsp_body
{	
	uint32_  msgResult; //消息发送结果
	char_    msgResultData[32];  // 可能的错误信息字符串
	uint32_  appendDataLength; //附加数据长度
	uint8_*  appendData;
}TS_SERVER_INIT_RSP_BODY;


////////////////////////////////////////////
// 设备参数配置消息
////////////////////////////////////////////

typedef struct stru_dev_config_body
{	
	uint32_  msgResult; //消息发送结果
	char_    msgResultData[32];  // 可能的错误信息字符串
	char_    deviceId[TS_GATE_ID_STRU_LEN];  //设备类型
	uint32_  deviceType;  //设备类型
	char_    sceneryId[TS_SCENERY_ID_STRU_LEN];  //景点ID
	//uint8_   sceneryDomainId[8];  //景区ID
	uint32_  sceneryPostion;  //偏移
}TS_DEV_CONFIG_RSP_BODY;


////////////////////////////////////////////
// 状态检测消息
////////////////////////////////////////////
typedef struct stru_dev_status_req_body
{	
	uint32_ statusCode;  //状态码
}TS_DEV_STATUS_REQ_BODY;

typedef struct stru_dev_status_rsp_body
{	
	uint32_  msgResult; //消息发送结果
	char_    msgResultData[32];  // 可能的错误信息字符串
	uint32_  oprationCode;  //操作码
	uint32_  appendDataLength; //附加数据长度
	uint8_   appendData[0];
}TS_DEV_STATUS_RSP_BODY;

////////////////////////////////////////////
// 刷卡数据消息
////////////////////////////////////////////

typedef struct stru_dev_card_data_msg
{
	char_    cardId[TS_CARD_ID_STRU_LEN];  
    char_    sceneryId[TS_SCENERY_ID_STRU_LEN];
	uint16_  cardType;   //卡类型
	uint16_  actionId;   //行为ID
	time_t   checkinTime;
}TS_DEVICE_CARD_DATA_REQ_BODY;

typedef struct stru_dev_card_data_rsp_msg
{	
	uint32_  msgResult; //消息发送结果
	char_    msgResultData[32];  // 可能的错误信息字符串
}TS_DEVICE_CARD_DATA_RSP_BODY;



////////////////////////////////////////////
// 刷卡数据批量上报消息
////////////////////////////////////////////

typedef struct stru_dev_bulk_data_msg
{  
	uint32_  cardDataCount;   //卡数据条目个数
	uint8_   cardDataEntry[0];   //行为ID
}TS_DEVICE_BULK_DATA_REQ_BODY;

typedef struct stru_dev_bulk_data_rsp_msg
{	
	uint32_  msgResult; //消息发送结果
	char_    msgResultData[32];  // 可能的错误信息字符串
	uint32_  errorEntryNum;
	uint8_   errCardId[0]; //errorEntryNum * 8
} TS_DEVICE_BULK_DATA_RSP_BODY;

LWDP_NAMESPACE_END;

#endif











