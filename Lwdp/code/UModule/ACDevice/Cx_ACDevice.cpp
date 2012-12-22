#include <Lwdp.h>
#include <PluginInc.h>
#include <sstream>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>
#include <Interface/DbMgr/Ix_DbMgr.h>

#include "../Interface/ZmqBackend/Ix_ZmqBackend.h"
#include "../Interface/ACDevice/Ix_ACDevice.h"

#include "../Common/ExternalInterface.h"
#include "../Common/InternalInterface.h"
#include "../Common/ACInterface.h"

#include "ACDeviceDef.h"
#include "Cx_ACDevice.h"

std::string Cx_ACDevice::mInitSql   = std::string("");
std::string Cx_ACDevice::mConfigSql = std::string("");
std::string Cx_ACDevice::mCardSql   = std::string("");


XPropertyTable Cx_ACDevice::gateInfoTable;

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
	GET_OBJECT_RET(ConfigMgr, iConfigMgr, LWDP_GET_OBJECT_ERROR);
	Cx_ACDevice::mInitSql = std::string(LW_ACDEVICE_SELECT_DEVICE_DEFAULT);
	XPropertys propSelectForInit;
	iConfigMgr->GetModulePropEntry(LW_ACDEVICE_MODULE_NAME, LW_ACDEVICE_SELECT_FOR_INIT, propSelectForInit);
	if(!propSelectForInit[0].propertyText.empty())
	{
		Cx_ACDevice::mInitSql = propSelectForInit[0].propertyText;
	}
	else
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <InitSql> In Config File, Default(%s)", 
					   Cx_ACDevice::mInitSql.c_str());
	}

	RINOK(iConfigMgr->GetModuleTable(LW_ACDEVICE_MODULE_NAME, LW_ACDEVICE_GATE_INFO_TABLE, 
									 Cx_ACDevice::gateInfoTable));

	Cx_ACDevice::mConfigSql = std::string(LW_ACDEVICE_CONFIG_DEVICE_DEFAULT);
	XPropertys propConfigSql;
	iConfigMgr->GetModulePropEntry(LW_ACDEVICE_MODULE_NAME, LW_ACDEVICE_CONFIG_SQL_STR, propConfigSql);
	if(!propConfigSql[0].propertyText.empty())
	{
		Cx_ACDevice::mConfigSql = propConfigSql[0].propertyText;
	}
	else
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <ConfigSql> In Config File, Default(%s)", 
					   Cx_ACDevice::mConfigSql.c_str());
	}

	Cx_ACDevice::mCardSql = std::string(LW_ACDEVICE_INSERT_CARD_DATA_DEFAULT);
	XPropertys propCardSql;
	iConfigMgr->GetModulePropEntry(LW_ACDEVICE_MODULE_NAME, LW_ACDEVICE_CARD_SQL_STR, propCardSql);
	if(!propCardSql[0].propertyText.empty())
	{
		Cx_ACDevice::mCardSql = propCardSql[0].propertyText;
	}
	else
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <CardSql> In Config File, Default(%s)", 
					   Cx_ACDevice::mCardSql.c_str());
	}

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
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::ERR, 
					   "Cx_ACDevice::DeviceInitMsgProcess Param ret_msg is NULL");
		return LWDP_PARAMETER_ERROR;
	}
	
	TS_ZMQ_SERVER_MSG* zmqMsg = (TS_ZMQ_SERVER_MSG*)ret_msg;
	TS_DEVICE_INIT_REQ_BODY* msgBody = (TS_DEVICE_INIT_REQ_BODY*)zmqMsg->customMsgBody;
	if(!msgBody)
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::WARNING, 
				       "Msg(%d) Body Error", zmqMsg->msgCode);
		
		uint8_* errMsg = new uint8_[sizeof(TS_ZMQ_SERVER_MSG) + sizeof(TS_SERVER_ERROR_BODY)] ;
		ASSERT_CHECK_RET(LWDP_PLUGIN_LOG, LWDP_MALLOC_MEMORY_ERROR, errMsg, 
						 "Cx_ACDevice::DeviceInitMsgProcess alloc memory error");

		memset(errMsg, 0, sizeof(TS_ZMQ_SERVER_MSG) + sizeof(TS_SERVER_ERROR_BODY));
		TS_ZMQ_SERVER_MSG* errStru = (TS_ZMQ_SERVER_MSG*)errMsg;
		errStru->deviceId = zmqMsg->deviceId;
		errStru->msgCode  = TS_SERVER_MSG_BODY_ERR;
		TS_SERVER_ERROR_BODY* errBody = (TS_SERVER_ERROR_BODY*)errStru->customMsgBody;
		errBody->errMsgCode = zmqMsg->msgCode;
		memcpy(errBody->errData, "Body Empty", 11);

		Data_Ptr tmpData(errMsg);
		send_msg = tmpData;
		send_msg_len = sizeof(TS_ZMQ_SERVER_MSG) + sizeof(TS_SERVER_ERROR_BODY);
		return LWDP_OK;
	}

	//check
	LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::INFO,
				   "[Received] REQ:%x REQCODE: %x, checkResult: %x deviceType: %x sceneryId: %s", 
			       zmqMsg->deviceId, zmqMsg->msgCode, msgBody->checkResult, msgBody->deviceType, msgBody->sceneryId);

	std::ostringstream tmps;
	tmps << Cx_ACDevice::mInitSql <<"'" << zmqMsg->deviceId << "'";
	std::string strTemp = tmps.str();
	LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::DEBUG,
				   "Select Db Str(%s)", strTemp.c_str());

	GET_OBJECT_RET(DbMgr, iDbMgr, LWDP_GET_OBJECT_ERROR);
	Cx_Interface<Ix_DbQuery> gateQuery;
	LWRESULT queryRes = 0;
	if((queryRes = iDbMgr->QuerySQL(strTemp, gateQuery)) != LWDP_OK)
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::ERR, 
				       "Msg(%d) DB Select Table Error", zmqMsg->msgCode);
		
		uint8_* errMsg = new uint8_[sizeof(TS_ZMQ_SERVER_MSG) + sizeof(TS_SERVER_ERROR_BODY)] ;
		ASSERT_CHECK_RET(LWDP_PLUGIN_LOG, LWDP_MALLOC_MEMORY_ERROR, errMsg, 
						 "Cx_ACDevice::DeviceInitMsgProcess alloc memory error");

		memset(errMsg, 0, sizeof(TS_ZMQ_SERVER_MSG) + sizeof(TS_SERVER_ERROR_BODY));
		TS_ZMQ_SERVER_MSG* errStru = (TS_ZMQ_SERVER_MSG*)errMsg;
		errStru->deviceId = zmqMsg->deviceId;
		errStru->msgCode  = TS_SERVER_DB_ERR;
		TS_SERVER_ERROR_BODY* errBody = (TS_SERVER_ERROR_BODY*)errStru->customMsgBody;
		errBody->errMsgCode = zmqMsg->msgCode;
		memcpy(errBody->errData, "Db Error", 9);

		Data_Ptr tmpData(errMsg);
		send_msg = tmpData;
		send_msg_len = sizeof(TS_ZMQ_SERVER_MSG) + sizeof(TS_SERVER_ERROR_BODY);

		return LWDP_OK;
	}

	if(gateQuery->NumRow() == 0)
	{
		uint8_* returnMsg = new uint8_[sizeof(TS_ZMQ_SERVER_MSG) + 
									   sizeof(TS_SERVER_INIT_RSP_BODY)];
		ASSERT_CHECK_RET(LWDP_PLUGIN_LOG, LWDP_MALLOC_MEMORY_ERROR, returnMsg, "Malloc Error");

		memset(returnMsg, 0, sizeof(TS_ZMQ_SERVER_MSG) + 
							 sizeof(TS_SERVER_INIT_RSP_BODY));
		TS_ZMQ_SERVER_MSG* retStru = (TS_ZMQ_SERVER_MSG*)returnMsg;
		retStru->deviceId = zmqMsg->deviceId;
		retStru->msgCode  = TS_SERVER_INIT_RSP_MSG; //
		TS_SERVER_INIT_RSP_BODY* retBody = (TS_SERVER_INIT_RSP_BODY*)retStru->customMsgBody;
		retBody->msgResult = TS_SERVER_ID_ERROR;
		memcpy(retBody->msgResultData, "Can't Find ID", 14);

		Data_Ptr tmpData(returnMsg);
		send_msg = tmpData;
		send_msg_len = sizeof(TS_ZMQ_SERVER_MSG) + 
					   sizeof(TS_SERVER_INIT_RSP_BODY);
		
		return LWDP_OK;
	}

	uint32_ retCode = TS_SERVER_OK;
	char_*  retStr  = "Check OK!";
	std::string scidCol   = Cx_ACDevice::gateInfoTable[LW_ACDEVICE_GATE_INFO_SCENERY_ID_COL].propertyText;
	std::string scidValue = gateQuery->GetStringField(scidCol, "");
	ASSERT_CHECK_HALT(LWDP_PLUGIN_LOG, !scidValue.empty(), "scidValue Empty");
	if(scidValue != std::string((char_ *)msgBody->sceneryId, sizeof(msgBody->sceneryId)))
	{
		retCode = TS_SERVER_CHECK_OK_RECONFIG;
		retStr = "sceneryId reconfig";
	}

	std::string positionCol   = Cx_ACDevice::gateInfoTable[LW_ACDEVICE_GATE_INFO_POSITION_COL].propertyText;
	std::string positionValue = gateQuery->GetStringField(positionCol, "");
	ASSERT_CHECK_HALT(LWDP_PLUGIN_LOG, !positionValue.empty(), "positionValue Empty");
	uint32_ posInt = atol(positionValue.c_str());
	if(posInt != msgBody->position)
	{
		retCode = TS_SERVER_CHECK_OK_RECONFIG;
		retStr  = "position reconfig";
	}

	std::string typeCol   = Cx_ACDevice::gateInfoTable[LW_ACDEVICE_GATE_INFO_TYPE_COL].propertyText;
	std::string typeValue = gateQuery->GetStringField(typeCol, "");
	ASSERT_CHECK_HALT(LWDP_PLUGIN_LOG, !typeValue.empty(), "typeValue Empty");
	uint32_ typeInt = atol(typeValue.c_str());
	if(typeInt != msgBody->deviceType)
	{
		retCode = TS_SERVER_CHECK_OK_RECONFIG;
		retStr  = "gate type reconfig";
	}
	
	uint8_* returnMsg = new uint8_[sizeof(TS_ZMQ_SERVER_MSG) + 
								   sizeof(TS_SERVER_INIT_RSP_BODY)];
	ASSERT_CHECK_RET(LWDP_PLUGIN_LOG, LWDP_MALLOC_MEMORY_ERROR, returnMsg, "Malloc Error");

	memset(returnMsg, 0, sizeof(TS_ZMQ_SERVER_MSG) + 
						 sizeof(TS_SERVER_INIT_RSP_BODY));
	TS_ZMQ_SERVER_MSG* retStru = (TS_ZMQ_SERVER_MSG*)returnMsg;
	retStru->deviceId = zmqMsg->deviceId;
	retStru->msgCode  = TS_SERVER_INIT_RSP_MSG; //
	TS_SERVER_INIT_RSP_BODY* retBody = (TS_SERVER_INIT_RSP_BODY*)retStru->customMsgBody;
	retBody->msgResult = retCode;
	memcpy(retBody->msgResultData, retStr, strlen(retStr));

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
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::ERR, 
					   "Cx_ACDevice::DeviceInitMsgProcess Param ret_msg is NULL");
		return LWDP_PARAMETER_ERROR;
	}
	
	Data_Ptr tmpData;
	TS_ZMQ_SERVER_MSG* zmqMsg = (TS_ZMQ_SERVER_MSG*)ret_msg;

	//check
	LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::INFO,
				   "[Received] REQ:%x REQCODE: %x", 
			       zmqMsg->deviceId, zmqMsg->msgCode);

	std::ostringstream tmps;
	tmps << Cx_ACDevice::mConfigSql <<"'" << zmqMsg->deviceId << "'";
	std::string strTemp = tmps.str();
	LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::DEBUG,
				   "Config Select Db Str(%s)", strTemp.c_str());

	GET_OBJECT_RET(DbMgr, iDbMgr, LWDP_GET_OBJECT_ERROR);
	Cx_Interface<Ix_DbQuery> gateQuery;
	LWRESULT queryRes = 0;
	uint32_ retCode = 0;
	char_*  retMsg  = "NO ERROR!";
	if((queryRes = iDbMgr->QuerySQL(strTemp, gateQuery)) != LWDP_OK)
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::ERR, 
				       "Msg(%d) DB Select(%s) Table Error", 
				       zmqMsg->msgCode, strTemp.c_str());
		
		retCode = TS_SERVER_DB_ERR;
		retMsg  = "Db Error";
		goto ERR_RET;
	}

	if(gateQuery->NumRow() == 0)
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::ERR, 
				       "Msg(%d) Can't Find id(%d) Select(%s) Table Error", 
				       zmqMsg->msgCode, zmqMsg->deviceId, strTemp.c_str());
		
		retCode = TS_SERVER_ID_ERROR;
		retMsg  = "Can't Find ID";
		
		goto ERR_RET;
	}

	uint8_* returnMsg = new uint8_[sizeof(TS_ZMQ_SERVER_MSG) + 
							   sizeof(TS_DEV_CONFIG_RSP_BODY)];

	{
		ASSERT_CHECK_RET(LWDP_PLUGIN_LOG, LWDP_MALLOC_MEMORY_ERROR, returnMsg, "Malloc Error");

		memset(returnMsg, 0, sizeof(TS_ZMQ_SERVER_MSG) + 
							 sizeof(TS_DEV_CONFIG_RSP_BODY));
		TS_ZMQ_SERVER_MSG* retStru = (TS_ZMQ_SERVER_MSG*)returnMsg;
		retStru->deviceId = zmqMsg->deviceId;
		retStru->msgCode  = TS_SERVER_CONFIG_RSP_MSG; //
		TS_DEV_CONFIG_RSP_BODY* retBody = (TS_DEV_CONFIG_RSP_BODY*)retStru->customMsgBody;
		retBody->msgResult = TS_SERVER_OK;
		memcpy(retBody->msgResultData, "OK!", 4);

		//scenery
		std::string scidCol   = Cx_ACDevice::gateInfoTable[LW_ACDEVICE_GATE_INFO_SCENERY_ID_COL].propertyText;
		std::string scidValue = gateQuery->GetStringField(scidCol, "");
		ASSERT_CHECK_HALT(LWDP_PLUGIN_LOG, !scidValue.empty(), "scidValue Empty");
		memcpy(retBody->sceneryId, scidValue.c_str(), 8);

		//position
		std::string positionCol   = Cx_ACDevice::gateInfoTable[LW_ACDEVICE_GATE_INFO_POSITION_COL].propertyText;
		std::string positionValue = gateQuery->GetStringField(positionCol, "");
		ASSERT_CHECK_HALT(LWDP_PLUGIN_LOG, !positionValue.empty(), "positionValue Empty");
		retBody->sceneryPostion = atol(positionValue.c_str());

		//type
		std::string typeCol   = Cx_ACDevice::gateInfoTable[LW_ACDEVICE_GATE_INFO_TYPE_COL].propertyText;
		std::string typeValue = gateQuery->GetStringField(typeCol, "");
		ASSERT_CHECK_HALT(LWDP_PLUGIN_LOG, !typeValue.empty(), "typeValue Empty");
		retBody->deviceType = atol(typeValue.c_str());

		//deviceId
		retBody->deviceId = retStru->deviceId;  //设备类型	
		memcpy(retBody->sceneryDomainId, "1234567", 8);
	}

	tmpData.reset(returnMsg);
	send_msg = tmpData;
	send_msg_len = sizeof(TS_ZMQ_SERVER_MSG) + 
				   sizeof(TS_DEV_CONFIG_RSP_BODY);

	return LWDP_OK;
	
ERR_RET:
	uint8_* returnErrMsg = new uint8_[sizeof(TS_ZMQ_SERVER_MSG) + 
								      sizeof(TS_DEV_CONFIG_RSP_BODY)];
	ASSERT_CHECK_RET(LWDP_PLUGIN_LOG, LWDP_MALLOC_MEMORY_ERROR, returnErrMsg, "Malloc Error");

	memset(returnErrMsg, 0, sizeof(TS_ZMQ_SERVER_MSG) + 
					        sizeof(TS_DEV_CONFIG_RSP_BODY));
	TS_ZMQ_SERVER_MSG* retStru = (TS_ZMQ_SERVER_MSG*)returnErrMsg;
	retStru->deviceId = zmqMsg->deviceId;
	retStru->msgCode  = TS_SERVER_CONFIG_RSP_MSG; //
	TS_DEV_CONFIG_RSP_BODY* retBody = (TS_DEV_CONFIG_RSP_BODY*)retStru->customMsgBody;
	retBody->msgResult = retCode;
	memcpy(retBody->msgResultData, retMsg, strlen(retMsg));

	Data_Ptr tmpErrData(returnErrMsg);
	send_msg = tmpErrData;
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
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::ERR, 
					   "Cx_ACDevice::DeviceInitMsgProcess Param ret_msg is NULL");
		return LWDP_PARAMETER_ERROR;
	}
	
	TS_ZMQ_SERVER_MSG* zmqMsg = (TS_ZMQ_SERVER_MSG*)ret_msg;
	TS_DEV_STATUS_REQ_BODY* msgBody = (TS_DEV_STATUS_REQ_BODY*)zmqMsg->customMsgBody;
	if(!msgBody)
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::WARNING, 
				       "Msg(%d) Body Error", zmqMsg->msgCode);
		
		uint8_* errMsg = new uint8_[sizeof(TS_ZMQ_SERVER_MSG) + sizeof(TS_SERVER_ERROR_BODY)] ;
		memset(errMsg, 0, sizeof(TS_ZMQ_SERVER_MSG) + sizeof(TS_SERVER_ERROR_BODY));
		TS_ZMQ_SERVER_MSG* errStru = (TS_ZMQ_SERVER_MSG*)errMsg;
		errStru->deviceId = zmqMsg->deviceId;
		errStru->msgCode  = TS_SERVER_MSG_BODY_ERR;
		TS_SERVER_ERROR_BODY* errBody = (TS_SERVER_ERROR_BODY*)errStru->customMsgBody;
		errBody->errMsgCode = zmqMsg->msgCode;
		memcpy(errBody->errData, "Body Empty", 11);

		Data_Ptr tmpData(errMsg);
		send_msg = tmpData;
		send_msg_len = sizeof(TS_ZMQ_SERVER_MSG) + sizeof(TS_SERVER_ERROR_BODY);
		return LWDP_OK;
	}

	//check
	LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::INFO,
				   "[Received] REQ:%x REQCODE: %x, statusCode: %x", 
			       zmqMsg->deviceId, zmqMsg->msgCode, msgBody->statusCode);


	uint8_* returnMsg = new uint8_[sizeof(TS_ZMQ_SERVER_MSG) + 
								   sizeof(TS_DEV_STATUS_RSP_BODY)];
	ASSERT_CHECK_RET(LWDP_PLUGIN_LOG, LWDP_MALLOC_MEMORY_ERROR, returnMsg, "Malloc Error");

	memset(returnMsg, 0, sizeof(TS_ZMQ_SERVER_MSG) + 
					     sizeof(TS_DEV_STATUS_RSP_BODY));
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
return -1;
	if(!ret_msg)
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::ERR, 
					   "Cx_ACDevice::DeviceInitMsgProcess Param ret_msg is NULL");
		return LWDP_PARAMETER_ERROR;
	}

	LWRESULT funStatus = 0;
	TS_ZMQ_SERVER_MSG* zmqMsg = (TS_ZMQ_SERVER_MSG*)ret_msg;
	TS_DEVICE_CARD_DATA_REQ_BODY* msgBody = (TS_DEVICE_CARD_DATA_REQ_BODY*)zmqMsg->customMsgBody;
	if(!msgBody)
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::WARNING, 
				       "Msg(%d) Body Error", zmqMsg->msgCode);
		
		uint8_* errMsg = new uint8_[sizeof(TS_ZMQ_SERVER_MSG) + sizeof(TS_SERVER_ERROR_BODY)] ;
		memset(errMsg, 0, sizeof(TS_ZMQ_SERVER_MSG) + sizeof(TS_SERVER_ERROR_BODY));
		TS_ZMQ_SERVER_MSG* errStru = (TS_ZMQ_SERVER_MSG*)errMsg;
		errStru->deviceId = zmqMsg->deviceId;
		errStru->msgCode  = TS_SERVER_MSG_BODY_ERR;
		TS_SERVER_ERROR_BODY* errBody = (TS_SERVER_ERROR_BODY*)errStru->customMsgBody;
		errBody->errMsgCode = zmqMsg->msgCode;
		memcpy(errBody->errData, "Body Empty", 11);

		Data_Ptr tmpData(errMsg);
		send_msg = tmpData;
		send_msg_len = sizeof(TS_ZMQ_SERVER_MSG) + sizeof(TS_SERVER_ERROR_BODY);
		return LWDP_ERROR;
	}

	//check
	LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::INFO,
				   "[Received] REQ:%x REQCODE: %x, cardId: %s sceneryId: %s cardType: %x actionId: %x checkinTime: %x", 
			       zmqMsg->deviceId, zmqMsg->msgCode, msgBody->cardId, msgBody->sceneryId, 
			       msgBody->cardType, msgBody->actionId, msgBody->checkinTime);

	char_ buffer[2048] = {0};
	Api_snprintf(buffer, 2047, Cx_ACDevice::mCardSql.c_str(), std::string((char_ *)msgBody->cardId, sizeof(msgBody->cardId)).c_str(), 
				  											   std::string((char_ *)msgBody->sceneryId, sizeof(msgBody->sceneryId)).c_str(),
				  								               zmqMsg->deviceId,
				  								               msgBody->cardType,
				  								               msgBody->actionId,
				  								               msgBody->checkinTime);


	LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::DEBUG,
				   "Select Db Str(%s)", buffer);
	GET_OBJECT_RET(DbMgr, iDbMgr, LWDP_GET_OBJECT_ERROR);
	Cx_Interface<Ix_DbQuery> gateQuery;
	int32_ queryRes = 0;
	uint32_ retCode = 0;
	char_*  retMsg  = "NO ERROR!";
	if((queryRes = iDbMgr->ExecSQL(buffer)) != 1)
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::ERR, 
				       "Msg(%d) DB Insert(%s) Table Error", zmqMsg->msgCode, buffer);

		retCode = TS_SERVER_DB_ERR;
		retMsg  = "Insert Error";
		funStatus = LWDP_ERROR;
	}


	uint8_* returnMsg = new uint8_[sizeof(TS_ZMQ_SERVER_MSG) + 
								   sizeof(TS_DEVICE_CARD_DATA_RSP_BODY)];
	ASSERT_CHECK_RET(LWDP_PLUGIN_LOG, LWDP_MALLOC_MEMORY_ERROR, returnMsg, "Malloc Error");

	memset(returnMsg, 0, sizeof(TS_ZMQ_SERVER_MSG) + 
						 sizeof(TS_DEVICE_CARD_DATA_RSP_BODY));
	TS_ZMQ_SERVER_MSG* retStru = (TS_ZMQ_SERVER_MSG*)returnMsg;
	retStru->deviceId = zmqMsg->deviceId;
	retStru->msgCode  = TS_SERVER_CARD_DATA_RSP_MSG; //
	TS_DEVICE_CARD_DATA_RSP_BODY* retBody = (TS_DEVICE_CARD_DATA_RSP_BODY*)retStru->customMsgBody;
	retBody->msgResult = retCode;
	memcpy(retBody->msgResultData, retMsg, strlen(retMsg));

	Data_Ptr tmpData(returnMsg);
	send_msg = tmpData;
	send_msg_len = sizeof(TS_ZMQ_SERVER_MSG) + 
				   sizeof(TS_DEVICE_CARD_DATA_RSP_BODY);
	
	return funStatus;
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
}TS_DEVICE_BULK_DATA_REQ_BODY;

typedef struct stru_dev_bulk_data_rsp_msg
{	
	uint32_  msgResult; //消息发送结果
	char_    msgResultData[32];  // 可能的错误信息字符串
	uint32_  errorEntryNum;
	uint8_   errCardId[1]; //errorEntryNum * 8
} TS_DEVICE_BULK_DATA_RSP_BODY;
*/

LWRESULT Cx_ACDevice::DeviceBulkDataMsgProcess(const uint8_* ret_msg, uint32_ ret_msg_len, 
								      Data_Ptr& send_msg, uint32_& send_msg_len)
{
	if(!ret_msg)
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::ERR, 
					   "Cx_ACDevice::DeviceInitMsgProcess Param ret_msg is NULL");
		return LWDP_PARAMETER_ERROR;
	}
	
	TS_ZMQ_SERVER_MSG* zmqMsg = (TS_ZMQ_SERVER_MSG*)ret_msg;
	TS_DEVICE_BULK_DATA_REQ_BODY* msgBody = (TS_DEVICE_BULK_DATA_REQ_BODY*)zmqMsg->customMsgBody;
	if(!msgBody)
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::WARNING, 
				       "Msg(%d) Body Error", zmqMsg->msgCode);
		
		uint8_* errMsg = new uint8_[sizeof(TS_ZMQ_SERVER_MSG) + sizeof(TS_SERVER_ERROR_BODY)] ;
		memset(errMsg, 0, sizeof(TS_ZMQ_SERVER_MSG) + sizeof(TS_SERVER_ERROR_BODY));
		TS_ZMQ_SERVER_MSG* errStru = (TS_ZMQ_SERVER_MSG*)errMsg;
		errStru->deviceId = zmqMsg->deviceId;
		errStru->msgCode  = TS_SERVER_MSG_BODY_ERR;
		TS_SERVER_ERROR_BODY* errBody = (TS_SERVER_ERROR_BODY*)errStru->customMsgBody;
		errBody->errMsgCode = zmqMsg->msgCode;
		memcpy(errBody->errData, "Body Empty", 11);

		Data_Ptr tmpData(errMsg);
		send_msg = tmpData;
		send_msg_len = sizeof(TS_ZMQ_SERVER_MSG) + sizeof(TS_SERVER_ERROR_BODY);
		return LWDP_OK;
	}

	//check	
	LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::INFO,
				   "[Received] REQ:%x REQCODE: %x, cardDataCount: %x", 
			       zmqMsg->deviceId, zmqMsg->msgCode, msgBody->cardDataCount);

	uint32_ tmpLen = sizeof(TS_ZMQ_SERVER_MSG) + sizeof(TS_DEVICE_BULK_DATA_REQ_BODY) +
				     (sizeof(TS_DEVICE_CARD_DATA_REQ_BODY) * msgBody->cardDataCount);
	if(ret_msg_len < tmpLen)
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::ERR,
					   "Request Data is too Small,ret_msg_len(%d) Should(%d) cardDataCount(%d)", 
					   ret_msg_len, tmpLen, msgBody->cardDataCount);
		
		uint8_* errMsg = new uint8_[sizeof(TS_ZMQ_SERVER_MSG) + sizeof(TS_SERVER_ERROR_BODY)] ;
		memset(errMsg, 0, sizeof(TS_ZMQ_SERVER_MSG) + sizeof(TS_SERVER_ERROR_BODY));
		TS_ZMQ_SERVER_MSG* errStru = (TS_ZMQ_SERVER_MSG*)errMsg;
		errStru->deviceId = zmqMsg->deviceId;
		errStru->msgCode  = TS_SERVER_MSG_BODY_ERR;
		TS_SERVER_ERROR_BODY* errBody = (TS_SERVER_ERROR_BODY*)errStru->customMsgBody;
		errBody->errMsgCode = zmqMsg->msgCode;
		memcpy(errBody->errData, "Body Len too small", 19);

		Data_Ptr tmpData(errMsg);
		send_msg = tmpData;
		send_msg_len = sizeof(TS_ZMQ_SERVER_MSG) + sizeof(TS_SERVER_ERROR_BODY);
		
		return LWDP_OK;
	}
	
	uint32_ i = 0;
	std::vector<std::string> errList;
	TS_DEVICE_CARD_DATA_REQ_BODY* tmpBody = NULL;
	for(i=0; i<msgBody->cardDataCount; ++i)
	{
		tmpBody = (TS_DEVICE_CARD_DATA_REQ_BODY*)msgBody->cardDataEntry;

		
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::INFO,
					   "[Received] REQ:%x REQCODE: %x, cardId: %s sceneryId: %s cardType: %x actionId: %x checkinTime: %x", 
				       zmqMsg->deviceId, zmqMsg->msgCode, tmpBody[i].cardId, tmpBody[i].sceneryId, 
				       tmpBody[i].cardType, tmpBody[i].actionId, tmpBody[i].checkinTime);

		uint8_* fakeMsg = new uint8_[sizeof(TS_ZMQ_SERVER_MSG) + 
							    sizeof(TS_DEVICE_CARD_DATA_REQ_BODY)];

		TS_ZMQ_SERVER_MSG* fakeCardMsg = (TS_ZMQ_SERVER_MSG*)fakeMsg;
		fakeCardMsg->deviceId = zmqMsg->deviceId;
		fakeCardMsg->msgCode  = zmqMsg->msgCode;
		memcpy(fakeCardMsg->customMsgBody, &tmpBody[i], sizeof(TS_DEVICE_CARD_DATA_REQ_BODY));

		Data_Ptr tmpData;
		tmpData.reset();
		uint32_  len = 0;
		if(DeviceCardDataMsgProcess(fakeMsg, sizeof(TS_ZMQ_SERVER_MSG) + sizeof(TS_DEVICE_CARD_DATA_REQ_BODY)
							        , tmpData, len) != LWDP_OK)
		{
			errList.push_back(std::string((char_*)tmpBody[i].cardId, sizeof(tmpBody[i].cardId)));
		}
	}

	uint8_* returnMsg = NULL;
	uint32_ retSize = 0;
	if(!errList.empty())
	{
		retSize = sizeof(TS_ZMQ_SERVER_MSG) + 
			      sizeof(TS_DEVICE_BULK_DATA_RSP_BODY) +
			      sizeof(tmpBody->cardId) * errList.size();
	}
	else
	{
		retSize = sizeof(TS_ZMQ_SERVER_MSG) + 
				  sizeof(TS_DEVICE_BULK_DATA_RSP_BODY);
	}
	
	returnMsg = new uint8_[retSize];
	memset(returnMsg, 0, retSize);
	ASSERT_CHECK_RET(LWDP_PLUGIN_LOG, LWDP_MALLOC_MEMORY_ERROR, returnMsg, "Malloc Error");

	TS_ZMQ_SERVER_MSG* retStru = (TS_ZMQ_SERVER_MSG*)returnMsg;
	retStru->deviceId = zmqMsg->deviceId;
	retStru->msgCode  = TS_SERVER_BULK_DATA_RSP_MSG; //
	TS_DEVICE_BULK_DATA_RSP_BODY* retBody = (TS_DEVICE_BULK_DATA_RSP_BODY*)retStru->customMsgBody;
	retBody->msgResult = TS_SERVER_OK;
	memcpy(retBody->msgResultData, "Check OK!", 10);

	if(!errList.empty())
	{
		retBody->msgResult = TS_SERVER_BULK_WRITE_DATA_ERROR;
		memcpy(retBody->msgResultData, "Have Error!", 12);

		retBody->errorEntryNum = errList.size();
		for(int i = 0; i < retBody->errorEntryNum; ++i)
		{
			memcpy(retBody->errCardId + (sizeof(tmpBody->cardId) * i), errList[i].data(), sizeof(tmpBody->cardId));
		}
	}

	Data_Ptr tmpData(returnMsg);
	send_msg     = tmpData;
	send_msg_len = retSize;
	
	return LWDP_OK;
}



