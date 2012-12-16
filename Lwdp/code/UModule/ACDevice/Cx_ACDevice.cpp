#include <Lwdp.h>
#include <PluginInc.h>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>

#include "../Interface/ZmqBackend/Ix_ZmqBackend.h"
#include "../Interface/ACDevice/Ix_ACDevice.h"

#include "../Common/ExternalInterface.h"
#include "../Common/InternalInterface.h"

#include "Cx_ACDevice.h"

Cx_ACDevice::Cx_ACDevice()
{
	printf("ACDevice Create!\n");
}

Cx_ACDevice::~Cx_ACDevice()
{
	printf("ACDevice Delete!\n");
}

LWRESULT Cx_ACDevice::Init()
{
	GET_OBJECT_RET(ZmqBackend, iZmqBackend, LWDP_GET_OBJECT_ERROR);


	MsgDelegate regFun = MakeDelegate(this, &Cx_ACDevice::DeviceInitMsgProcess);
	RINOK(iZmqBackend->RegisteZmqMsg(TS_DEVICE_INIT_REQ_MSG, regFun));

	regFun = MakeDelegate(this, &Cx_ACDevice::DeviceConfigMsgProcess);
	RINOK(iZmqBackend->RegisteZmqMsg(TS_DEVICE_CONFIG_REQ_MSG, regFun));

	regFun = MakeDelegate(this, &Cx_ACDevice::DeviceHBMsgProcess);
	RINOK(iZmqBackend->RegisteZmqMsg(TS_DEVICE_HEART_BEAT_REQ_MSG, regFun));

	regFun = MakeDelegate(this, &Cx_ACDevice::DeviceCardDataMsgProcess);
	RINOK(iZmqBackend->RegisteZmqMsg(TS_DEVICE_CARD_DATA_REQ_MSG, regFun));

	regFun = MakeDelegate(this, &Cx_ACDevice::DeviceBulkDataMsgProcess);
	RINOK(iZmqBackend->RegisteZmqMsg(TS_DEVICE_BULK_DATA_REQ_MSG, regFun));
	
	return LWDP_OK;
}

LWRESULT MsgProcess(const uint8_* ret_msg, uint32_ ret_msg_len, 
					                 Data_Ptr& send_msg, uint32_& send_msg_len)
{
	//returnMsg.deviceId = zMsg->deviceId;
	//returnMsg.msgCode  = TS_SERVER_UNKNOW_MSG;

	uint8_* tmpstr = (uint8_*)new char[strlen("Hello ACD CallBack Client!") + 1];
	strcpy((char*)tmpstr, "Hello ACD CallBack Client!");
	Data_Ptr tmpData(tmpstr);
	send_msg = tmpData;
	send_msg_len = strlen("Hello ACD CallBack Client!") + 1;

	return LWDP_OK;
}

/*
enum TS_INIT_MSG_RESAULT_ENUM
{	
	TS_SERVER_CHECK_OK_RECONFIG = 1, //验证成功，但须重配参数
	TS_SERVER_ID_ERROR,     //设备ID未知错误
	TS_SERVER_TYPE_ERROR,   //设备类型错误
	TS_SERVER_UNKNOW_ERROR //未知错误，具体见附加信息
};
typedef struct stru_device_init_req_body
{	
	uint32_ deviceType;  //设备类型
	uint8_  sceneryId[8];  //景点ID
	uint32_ checkResult; //设备自检结果
}TS_DEVICE_INIT_REQ_BODY;

typedef struct stru_server_init_rsp_body
{	
	uint32_  msgResult; //消息发送结果
	char_    msgResultData[32];  // 可能的错误信息字符串
	uint32_  appendDataLength; //附加数据长度
	uint8_*  appendData;
}TS_SERVER_INIT_RSP_BODY;
typedef struct stru_zmq_server_msg
{
	uint32_ deviceId;
	uint32_ msgCode;
	uint8_  customMsgBody[0];  //消息体
}TS_ZMQ_SERVER_MSG;
*/
LWRESULT Cx_ACDevice::DeviceInitMsgProcess(const uint8_* ret_msg, uint32_ ret_msg_len, 
								      Data_Ptr& send_msg, uint32_& send_msg_len)
{
	if(!ret_msg)
	{
		LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::ERR, 
					   "Cx_ACDevice::DeviceInitMsgProcess Param ret_msg is NULL");
		return LWDP_PARAMETER_ERROR;
	}
	
	TS_ZMQ_SERVER_MSG* zmqMsg = (TS_ZMQ_SERVER_MSG*)ret_msg;
	TS_DEVICE_INIT_REQ_BODY* msgBody = (TS_DEVICE_INIT_REQ_BODY*)zmqMsg->customMsgBody;
	if(!msgBody)
	{
		LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::WARNING, 
				   "Unknow Request Msg(%d)", zmqMsg->msgCode);
		
		uint8_* errMsg = new uint8_[sizeof(TS_ZMQ_SERVER_MSG)] ;
		TS_ZMQ_SERVER_MSG* errStru = (TS_ZMQ_SERVER_MSG*)errMsg;
		errStru->deviceId = zmqMsg->deviceId;
		errStru->msgCode  = TS_SERVER_MSG_BODY_ERR;

		Data_Ptr tmpData(errMsg);
		send_msg = tmpData;
		send_msg_len = sizeof(TS_ZMQ_SERVER_MSG);
		return LWDP_OK;
	}

	//check
	LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::INFO,
				   "[Received] REQ:%x REQCODE: %x, checkResult: %x deviceType: %x sceneryId: %s", 
			       zmqMsg->deviceId, zmqMsg->msgCode, msgBody->checkResult, msgBody->deviceType, msgBody->sceneryId);

	uint8_* returnMsg = new uint8_[sizeof(TS_ZMQ_SERVER_MSG) + 
								   sizeof(TS_SERVER_INIT_RSP_BODY)];
	ASSERT_CHECK_RET(LWDP_PLUGIN_LOG, LWDP_MALLOC_MEMORY_ERROR, returnMsg, "Malloc Error");

	TS_ZMQ_SERVER_MSG* retStru = (TS_ZMQ_SERVER_MSG*)returnMsg;
	retStru->deviceId = zmqMsg->deviceId;
	retStru->msgCode  = TS_SERVER_INIT_RSP_MSG; //
	TS_SERVER_INIT_RSP_BODY* retBody = (TS_SERVER_INIT_RSP_BODY*)retStru->customMsgBody;
	retBody->msgResult = TS_SERVER_OK;
	memcpy(retBody->msgResultData, "Check OK!", 10);

	Data_Ptr tmpData(returnMsg);
	send_msg = tmpData;
	send_msg_len = sizeof(TS_ZMQ_SERVER_MSG) + 
				   sizeof(TS_SERVER_INIT_RSP_BODY);
	
	return LWDP_OK;
}

/*
enum TS_SERVER_CONFIG_MSG_RESAULT_ENUM
{	
	TS_SERVER_CONFIG_ERROR = 1, //失败
};

typedef struct stru_dev_config_body
{	
	uint32_  msgResult; //消息发送结果
	char_    msgResultData[32];  // 可能的错误信息字符串
	uint32_  deviceId;  //设备类型
	uint32_  deviceType;  //设备类型
	uint8_   sceneryId[8];  //景点ID
	uint8_   sceneryDomainId[8];  //景区ID
	uint32_  sceneryPostion;  //偏移
}TS_DEV_CONFIG_BODY;
*/
LWRESULT Cx_ACDevice::DeviceConfigMsgProcess(const uint8_* ret_msg, uint32_ ret_msg_len, 
								      Data_Ptr& send_msg, uint32_& send_msg_len)
{
	if(!ret_msg)
	{
		LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::ERR, 
					   "Cx_ACDevice::DeviceInitMsgProcess Param ret_msg is NULL");
		return LWDP_PARAMETER_ERROR;
	}
	
	TS_ZMQ_SERVER_MSG* zmqMsg = (TS_ZMQ_SERVER_MSG*)ret_msg;

	//check
	LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::INFO,
				   "[Received] REQ:%x REQCODE: %x", 
			       zmqMsg->deviceId, zmqMsg->msgCode);

	uint8_* returnMsg = new uint8_[sizeof(TS_ZMQ_SERVER_MSG) + 
								   sizeof(TS_DEV_CONFIG_RSP_BODY)];
	ASSERT_CHECK_RET(LWDP_PLUGIN_LOG, LWDP_MALLOC_MEMORY_ERROR, returnMsg, "Malloc Error");

	TS_ZMQ_SERVER_MSG* retStru = (TS_ZMQ_SERVER_MSG*)returnMsg;
	retStru->deviceId = zmqMsg->deviceId;
	retStru->msgCode  = TS_SERVER_CONFIG_RSP_MSG; //
	TS_DEV_CONFIG_RSP_BODY* retBody = (TS_DEV_CONFIG_RSP_BODY*)retStru->customMsgBody;
	retBody->msgResult = TS_SERVER_OK;
	memcpy(retBody->msgResultData, "No Error!", 10);
	retBody->deviceId = retStru->deviceId;  //设备类型
	retBody->deviceType = 1;  //设备类型
	memcpy(retBody->sceneryId, "1234567", 8);
	memcpy(retBody->sceneryDomainId, "1234567", 8);
	retBody->sceneryPostion = 1024;  //偏移

	Data_Ptr tmpData(returnMsg);
	send_msg = tmpData;
	send_msg_len = sizeof(TS_ZMQ_SERVER_MSG) + 
				   sizeof(TS_DEV_CONFIG_RSP_BODY);
	
	return LWDP_OK;
}

/*
enum TS_SERVER_HB_MSG_RESAULT_ENUM
{	
	TS_SERVER_HB_ERROR = 1, //失败
};

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
	uint8_   appendData[1];
}TS_DEV_STATUS_RSP_BODY;

*/
LWRESULT Cx_ACDevice::DeviceHBMsgProcess(const uint8_* ret_msg, uint32_ ret_msg_len, 
								      Data_Ptr& send_msg, uint32_& send_msg_len)
{
	if(!ret_msg)
	{
		LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::ERR, 
					   "Cx_ACDevice::DeviceInitMsgProcess Param ret_msg is NULL");
		return LWDP_PARAMETER_ERROR;
	}
	
	TS_ZMQ_SERVER_MSG* zmqMsg = (TS_ZMQ_SERVER_MSG*)ret_msg;
	TS_DEV_STATUS_REQ_BODY* msgBody = (TS_DEV_STATUS_REQ_BODY*)zmqMsg->customMsgBody;
	if(!msgBody)
	{
		LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::WARNING, 
				   "Unknow Request Msg(%d)", zmqMsg->msgCode);
		
		uint8_* errMsg = new uint8_[sizeof(TS_ZMQ_SERVER_MSG)] ;
		TS_ZMQ_SERVER_MSG* errStru = (TS_ZMQ_SERVER_MSG*)errMsg;
		errStru->deviceId = zmqMsg->deviceId;
		errStru->msgCode  = TS_SERVER_MSG_BODY_ERR;

		Data_Ptr tmpData(errMsg);
		send_msg = tmpData;
		send_msg_len = sizeof(TS_ZMQ_SERVER_MSG);
		return LWDP_OK;
	}

	//check
	LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::INFO,
				   "[Received] REQ:%x REQCODE: %x, statusCode: %x", 
			       zmqMsg->deviceId, zmqMsg->msgCode, msgBody->statusCode);

	uint8_* returnMsg = new uint8_[sizeof(TS_ZMQ_SERVER_MSG) + 
								   sizeof(TS_DEV_STATUS_RSP_BODY)];
	ASSERT_CHECK_RET(LWDP_PLUGIN_LOG, LWDP_MALLOC_MEMORY_ERROR, returnMsg, "Malloc Error");

	TS_ZMQ_SERVER_MSG* retStru = (TS_ZMQ_SERVER_MSG*)returnMsg;
	retStru->deviceId = zmqMsg->deviceId;
	retStru->msgCode  = TS_SERVER_HEART_BEAT_RSP_MSG; //
	TS_DEV_STATUS_RSP_BODY* retBody = (TS_DEV_STATUS_RSP_BODY*)retStru->customMsgBody;
	retBody->msgResult = TS_SERVER_OK;
	memcpy(retBody->msgResultData, "No Error!", 10);

	Data_Ptr tmpData(returnMsg);
	send_msg = tmpData;
	send_msg_len = sizeof(TS_ZMQ_SERVER_MSG) + 
				   sizeof(TS_DEV_STATUS_RSP_BODY);
	
	return LWDP_OK;
}

/*
enum TS_CARD_DATA_MSG_RESAULT_ENUM
{	
	TS_SERVER_WRITE_DATA_ERROR = 1, //写数据库失败
	TS_SERVER_UNKONW_ERROR,     //设备ID未知错误
};

typedef struct stru_dev_card_data_msg
{
	uint8_   cardId[8];  
    uint8_   sceneryId[8];
	uint32_  cardType;   //卡类型
	uint32_  actionId;   //行为ID
	time_t   checkinTime;
}TS_DEVICE_CARD_DATA_REQ_BODY;

typedef struct stru_dev_card_data_rsp_msg
{	
	uint32_  msgResult; //消息发送结果
	char_    msgResultData[32];  // 可能的错误信息字符串
} TS_DEVICE_CARD_DATA_RSP_BODY;


*/
LWRESULT Cx_ACDevice::DeviceCardDataMsgProcess(const uint8_* ret_msg, uint32_ ret_msg_len, 
								      Data_Ptr& send_msg, uint32_& send_msg_len)
{
	if(!ret_msg)
	{
		LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::ERR, 
					   "Cx_ACDevice::DeviceInitMsgProcess Param ret_msg is NULL");
		return LWDP_PARAMETER_ERROR;
	}
	
	TS_ZMQ_SERVER_MSG* zmqMsg = (TS_ZMQ_SERVER_MSG*)ret_msg;
	TS_DEVICE_CARD_DATA_REQ_BODY* msgBody = (TS_DEVICE_CARD_DATA_REQ_BODY*)zmqMsg->customMsgBody;
	if(!msgBody)
	{
		LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::WARNING, 
				   "Unknow Request Msg(%d)", zmqMsg->msgCode);
		
		uint8_* errMsg = new uint8_[sizeof(TS_ZMQ_SERVER_MSG)] ;
		TS_ZMQ_SERVER_MSG* errStru = (TS_ZMQ_SERVER_MSG*)errMsg;
		errStru->deviceId = zmqMsg->deviceId;
		errStru->msgCode  = TS_SERVER_MSG_BODY_ERR;

		Data_Ptr tmpData(errMsg);
		send_msg = tmpData;
		send_msg_len = sizeof(TS_ZMQ_SERVER_MSG);
		return LWDP_OK;
	}

	//check
	LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::INFO,
				   "[Received] REQ:%x REQCODE: %x, cardId: %s sceneryId: %s cardType: %x actionId: %x checkinTime: %x", 
			       zmqMsg->deviceId, zmqMsg->msgCode, msgBody->cardId, msgBody->sceneryId, 
			       msgBody->cardType, msgBody->actionId, msgBody->checkinTime);

	uint8_* returnMsg = new uint8_[sizeof(TS_ZMQ_SERVER_MSG) + 
								   sizeof(TS_DEVICE_CARD_DATA_RSP_BODY)];
	ASSERT_CHECK_RET(LWDP_PLUGIN_LOG, LWDP_MALLOC_MEMORY_ERROR, returnMsg, "Malloc Error");

	TS_ZMQ_SERVER_MSG* retStru = (TS_ZMQ_SERVER_MSG*)returnMsg;
	retStru->deviceId = zmqMsg->deviceId;
	retStru->msgCode  = TS_SERVER_CARD_DATA_RSP_MSG; //
	TS_DEVICE_CARD_DATA_RSP_BODY* retBody = (TS_DEVICE_CARD_DATA_RSP_BODY*)retStru->customMsgBody;
	retBody->msgResult = TS_SERVER_OK;
	memcpy(retBody->msgResultData, "Insert OK!", 11);

	Data_Ptr tmpData(returnMsg);
	send_msg = tmpData;
	send_msg_len = sizeof(TS_ZMQ_SERVER_MSG) + 
				   sizeof(TS_DEVICE_CARD_DATA_RSP_BODY);
	
	return LWDP_OK;
}



/*
enum TS_BULK_DATA_MSG_RESAULT_ENUM
{	
	TS_SERVER_BULK_WRITE_DATA_ERROR = 1, //写数据库失败
	TS_SERVER_BULK_UNKONW_ERROR,     //设备ID未知错误
};

typedef struct stru_dev_bulk_data_msg
{  
	uint32_  cardDataCount;   //卡数据条目个数
	uint8_*  cardDataEntry;   //行为ID
}TS_DEVICE_BULK_DATA_MSG;

typedef struct stru_dev_bulk_data_rsp_msg
{	
	uint32_  msgResult; //消息发送结果
	char_    msgResultData[32];  // 可能的错误信息字符串
	uint32_  errorEntryNum;
	uint8_   errCardId[1]; //errorEntryNum * 8
} TS_DEVICE_BULK_DATA_RSP_MSG;
*/

LWRESULT Cx_ACDevice::DeviceBulkDataMsgProcess(const uint8_* ret_msg, uint32_ ret_msg_len, 
								      Data_Ptr& send_msg, uint32_& send_msg_len)
{
	if(!ret_msg)
	{
		LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::ERR, 
					   "Cx_ACDevice::DeviceInitMsgProcess Param ret_msg is NULL");
		return LWDP_PARAMETER_ERROR;
	}
	
	TS_ZMQ_SERVER_MSG* zmqMsg = (TS_ZMQ_SERVER_MSG*)ret_msg;
	TS_DEVICE_INIT_REQ_BODY* msgBody = (TS_DEVICE_INIT_REQ_BODY*)zmqMsg->customMsgBody;
	if(!msgBody)
	{
		LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::WARNING, 
				   "Unknow Request Msg(%d)", zmqMsg->msgCode);
		
		uint8_* errMsg = new uint8_[sizeof(TS_ZMQ_SERVER_MSG)] ;
		TS_ZMQ_SERVER_MSG* errStru = (TS_ZMQ_SERVER_MSG*)errMsg;
		errStru->deviceId = zmqMsg->deviceId;
		errStru->msgCode  = TS_SERVER_MSG_BODY_ERR;

		Data_Ptr tmpData(errMsg);
		send_msg = tmpData;
		send_msg_len = sizeof(TS_ZMQ_SERVER_MSG);
		return LWDP_OK;
	}

	//check
	LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::INFO,
				   "[Received] REQ:%x REQCODE: %x, checkResult: %x deviceType: %x sceneryId: %s\n", 
			       zmqMsg->deviceId, zmqMsg->msgCode, msgBody->checkResult, msgBody->deviceType, msgBody->sceneryId);

	uint8_* returnMsg = new uint8_[sizeof(TS_ZMQ_SERVER_MSG) + 
								   sizeof(TS_SERVER_INIT_RSP_BODY)];
	ASSERT_CHECK_RET(LWDP_PLUGIN_LOG, LWDP_MALLOC_MEMORY_ERROR, returnMsg, "Malloc Error");

	TS_ZMQ_SERVER_MSG* retStru = (TS_ZMQ_SERVER_MSG*)returnMsg;
	retStru->deviceId = zmqMsg->deviceId;
	retStru->msgCode  = TS_SERVER_INIT_RSP_MSG; //
	TS_SERVER_INIT_RSP_BODY* retBody = (TS_SERVER_INIT_RSP_BODY*)retStru->customMsgBody;
	retBody->msgResult = TS_SERVER_OK;
	memcpy(retBody->msgResultData, "Check OK!", 10);

	Data_Ptr tmpData(returnMsg);
	send_msg = tmpData;
	send_msg_len = sizeof(TS_ZMQ_SERVER_MSG) + 
				   sizeof(TS_SERVER_INIT_RSP_BODY);
	
	return LWDP_OK;
}



