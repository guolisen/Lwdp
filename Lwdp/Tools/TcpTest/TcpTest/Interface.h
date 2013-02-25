/*! \file   ExternalInterface.h
 *  \brief  Interface
 *  \author Guolisen, LwDp
 *  \date   2012.12.14
 */
 
#ifndef AA_EXTERNAL_INTERFACE_H_AA
#define AA_EXTERNAL_INTERFACE_H_AA

#define TS_GATE_ID_STRU_LEN      32
#define TS_RET_MSG_LEN           128

//-------------------------------------------------------
//Module Define

#define TS_SYSTEM_MODULE   0
#define TS_ACDEVICE_MODULE 1

#define GET_MSG_BASE(module_num) ((module_num) << 24)  //Module:module_num Code0x000000

// Input
//-----------------------------------------------------------
// Req Msg Head
typedef struct stru_req_server_msg
{
	uint32_ msgLength;  //ï¿½ï¿½Ï¢ï¿½å³¤ï¿½ï¿½  ï¿½Ë³ï¿½ï¿½ï¿½Îªï¿½ï¿½ï¿½ï¿½ï¿½Ï¢ï¿½ï¿½ï¿½È£ï¿½ï¿½ï¿½ï¿½ï¿½msgLengthï¿½ï¿½ï¿½ï¿½Ä³ï¿½ï¿½ï¿½
	char_   deviceId[TS_GATE_ID_STRU_LEN];
	uint32_ msgCode;
	uint8_  customMsgBody[0];  //ï¿½ï¿½Ï¢ï¿½ï¿½
}TS_REQ_SERVER_MSG;


// Output
//-----------------------------------------------------------
// Rsp Msg Head

typedef struct stru_rsp_server_msg
{
	uint32_ msgLength;  //ï¿½ï¿½Ï¢ï¿½å³¤ï¿½ï¿½  ï¿½Ë³ï¿½ï¿½ï¿½Îªï¿½ï¿½ï¿½ï¿½ï¿½Ï¢ï¿½ï¿½ï¿½È£ï¿½ï¿½ï¿½ï¿½ï¿½msgLengthï¿½ï¿½ï¿½ï¿½Ä³ï¿½ï¿½ï¿½
	uint32_ rspCode;
	char_   rspMsg[TS_RET_MSG_LEN]; 
	uint8_  customMsgBody[0];  //ï¿½ï¿½Ï¢ï¿½ï¿½
}TS_RSP_SERVER_MSG;


// SERVER RSP MSG CODE
#define TS_SYSTEM_MSG_BASE   GET_MSG_BASE(TS_SYSTEM_MODULE)   //0x00000000L Module:0x00 Code0x000000
enum TS_SYSTEM_MSG_ENUM
{
	TS_SERVER_OK = TS_SYSTEM_MSG_BASE,
		
	TS_SERVER_TCP_MSG_LEN_ERROR,
	TS_SERVER_RECV_TIMEOUT_ERROR,
	TS_SERVER_UNKNOW_MSG, //ï¿½Í»ï¿½ï¿½Ë·ï¿½ï¿½Íµï¿½ï¿½ï¿½Ï¢ÎªÎ´Öªï¿½ï¿½ï¿½ï¿½
	TS_SERVER_MSG_BODY_ERR, //ï¿½Í»ï¿½ï¿½Ë·ï¿½ï¿½Íµï¿½ï¿½ï¿½Ï¢ÎªÎ´Öªï¿½ï¿½ï¿½ï¿½
};

//-------------------------------------------------------

#define TS_CARD_ID_STRU_LEN      32
#define TS_SCENERY_ID_STRU_LEN   32
#define TS_GATECHECK_MSG_BASE    GET_MSG_BASE(TS_ACDEVICE_MODULE)  //0x01000000 Module:0x01 Code0x000000

/////////////////////////////////////////////////////////////
// Input
//-----------------------------------------------------------
// AC Req Msg Code
////////////////////////////////////////////////////////////

enum TS_GATECHECK_MSG_ENUM
{
	//ACDevice
	TS_DEVICE_INIT_REQ_MSG = TS_GATECHECK_MSG_BASE,
	TS_DEVICE_CONFIG_REQ_MSG,
	TS_DEVICE_HEART_BEAT_REQ_MSG,
	TS_DEVICE_CARD_DATA_REQ_MSG,
	TS_DEVICE_BULK_DATA_REQ_MSG,
};


/////////////////////////////////////////////////////////////
// Output
//-----------------------------------------------------------
// AC Rsp Msg Code
////////////////////////////////////////////////////////////

enum TS_RET_MSG_RESAULT_ENUM
{	
	TS_SERVER_CHECK_OK_RECONFIG = TS_GATECHECK_MSG_BASE, //ÑéÖ¤³É¹¦£¬µ«ÐëÖØÅä²ÎÊý
	TS_SERVER_ID_ERROR,     //Éè±¸IDÎ´Öª´íÎó
	TS_SERVER_TYPE_ERROR,   //Éè±¸ÀàÐÍ´íÎó
	TS_SERVER_UNKNOW_ERROR, //Î´Öª´íÎó£¬¾ßÌå¼û¸½¼ÓÐÅÏ¢
	TS_SERVER_DB_ERR,        //·þÎñÆ÷Êý¾Ý¿â´íÎó
	TS_SERVER_CONFIG_ERROR, //Ê§°Ü
	TS_SERVER_BULK_THREAD_ERR,
	TS_SERVER_HB_ERROR,
	TS_SERVER_WRITE_DATA_ERROR,
	TS_SERVER_BULK_WRITE_DATA_ERROR
};

////////////////////////////////////////////
// ÃÅ½ûÆô¶¯³õÊ¼»¯ÏûÏ¢
////////////////////////////////////////////
typedef struct stru_device_init_req_body
{	
	uint32_ deviceType;  //Éè±¸ÀàÐÍ
	char_   sceneryId[TS_SCENERY_ID_STRU_LEN];  //¾°µãID
	uint32_ checkResult; //Éè±¸×Ô¼ì½á¹û
}TS_DEVICE_INIT_REQ_BODY;

typedef struct stru_server_init_rsp_body
{	
	uint32_  appendDataLength; //¸½¼ÓÊý¾Ý³¤¶È
	uint8_*  appendData;
}TS_SERVER_INIT_RSP_BODY;


////////////////////////////////////////////
// Éè±¸²ÎÊýÅäÖÃÏûÏ¢
////////////////////////////////////////////

typedef struct stru_dev_config_rsp_body
{	
	char_    deviceId[TS_GATE_ID_STRU_LEN];  //Éè±¸ÀàÐÍ
	uint32_  deviceType;  //Éè±¸ÀàÐÍ
	char_    sceneryId[TS_SCENERY_ID_STRU_LEN];  //¾°µãID
}TS_DEV_CONFIG_RSP_BODY;


////////////////////////////////////////////
// ×´Ì¬¼ì²âÏûÏ¢
////////////////////////////////////////////
typedef struct stru_dev_status_req_body
{	
	uint32_ statusCode;  //×´Ì¬Âë
}TS_DEV_STATUS_REQ_BODY;

typedef struct stru_dev_status_rsp_body
{	
	uint32_  oprationCode;  //²Ù×÷Âë
	uint32_  appendDataLength; //¸½¼ÓÊý¾Ý³¤¶È
	uint8_   appendData[0];
}TS_DEV_STATUS_RSP_BODY;

////////////////////////////////////////////
// Ë¢¿¨Êý¾ÝÏûÏ¢
////////////////////////////////////////////

typedef struct stru_dev_card_data_req_msg
{
	char_    cardId[TS_CARD_ID_STRU_LEN];  
    char_    sceneryId[TS_SCENERY_ID_STRU_LEN];
	uint16_  cardType;   //¿¨ÀàÐÍ
	uint16_  actionId;   //ÐÐÎªID
	uint32_  checkinTime;
}TS_DEVICE_CARD_DATA_REQ_BODY;


////////////////////////////////////////////
// Ë¢¿¨Êý¾ÝÅúÁ¿ÉÏ±¨ÏûÏ¢
////////////////////////////////////////////

typedef struct stru_dev_bulk_data_req_msg
{  
	uint32_  cardDataCount;   //¿¨Êý¾ÝÌõÄ¿¸öÊý
	uint8_   cardDataEntry[0];   //ÐÐÎªID
}TS_DEVICE_BULK_DATA_REQ_BODY;

typedef struct stru_dev_bulk_data_rsp_msg
{	
	uint32_  errorEntryNum;
	uint8_   errCardId[0]; //errorEntryNum * 8
} TS_DEVICE_BULK_DATA_RSP_BODY;






#endif











