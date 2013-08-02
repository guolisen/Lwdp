#include <LwDp.h>
#include <PluginInc.h>
#include <sstream>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>
#include <Interface/DbMgr/Ix_DbMgr.h>
#include <Interface/CommonUtilMgr/Ix_CommonUtilMgr.h>

#include "../Interface/ZmqBackend/Ix_ZmqBackend.h"
#include "../Interface/ACDevice/Ix_ACDevice.h"

#include "../Common/ExternalInterface.h"
#include "../Common/InternalInterface.h"
#include "../Common/ACInterface.h"

#include "ACDeviceDef.h"
#include "Cx_ACDevice.h"

#include <time.h>


std::string Cx_ACDevice::mInitSql   = std::string("");
std::string Cx_ACDevice::mConfigSql = std::string("");
std::string Cx_ACDevice::mCardSql   = std::string("");

XPropertyTable Cx_ACDevice::gateInfoTable;

uint32_     processPageSize  = LW_ACDEVICE_BULK_PAGESIZE_DEFAULT;
uint32_     gThreadCount     = LW_ACDEVICE_THREAD_NUM_DEFAULT;


typedef std::vector<std::string> RET_LIST;

struct WORKTHREAD_PARAM
{
	uint32_ startPos;
	uint32_ length;
	TS_REQ_SERVER_MSG* msg;
	Cx_ACDevice* object;
	DBHandle dbHandle;
};

void* work_thread(void* arg)
{	
	if(!arg)
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::ERR, 
		           	   "ACD Work Thread Parameter is NULL");
		return NULL;
	}

	WORKTHREAD_PARAM* ptmpDomain = (WORKTHREAD_PARAM*)arg;	
	WORKTHREAD_PARAM Domain;
	Domain.startPos = ptmpDomain->startPos;
	Domain.length   = ptmpDomain->length;
	Domain.msg      = ptmpDomain->msg;
	ASSERT_CHECK(Domain.msg != 0);
	Domain.object   = ptmpDomain->object;
	ASSERT_CHECK(Domain.object != 0);
	Domain.dbHandle = ptmpDomain->dbHandle;
	DBHandle workDbHandle = Domain.dbHandle;
	ASSERT_CHECK(workDbHandle != 0);
	free(ptmpDomain);
	ptmpDomain = NULL;


	TS_REQ_SERVER_MSG* zmqMsg = (TS_REQ_SERVER_MSG*)Domain.msg;
	TS_DEVICE_BULK_DATA_REQ_BODY* msgBody = (TS_DEVICE_BULK_DATA_REQ_BODY*)zmqMsg->customMsgBody;
	Cx_ACDevice* acdObj = Domain.object;

	uint32_ inum = Domain.length;
	uint32_ pageSize = (processPageSize > Domain.length)? Domain.length: processPageSize;
	uint32_ reTimes  = (inum / pageSize);
		    reTimes += (inum%pageSize)>0?1:0;

	uint32_ blockNum  = 0;
	RET_LIST* errList = new RET_LIST;
	ASSERT_CHECK(errList != 0);
	uint32_ newLen = sizeof(TS_REQ_SERVER_MSG) + 
					 sizeof(TS_DEVICE_CARD_DATA_REQ_BODY);
	uint8_* fakeMsg = new uint8_[newLen];
	ASSERT_CHECK(fakeMsg != 0);
	memset(fakeMsg, 0, newLen * sizeof(uint8_));
	TS_DEVICE_CARD_DATA_REQ_BODY* tmpBody = (TS_DEVICE_CARD_DATA_REQ_BODY*)msgBody->cardDataEntry;

	LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::NOTICE, 
					"Worker TotleNum: %d reTimes: %d pageSize:%d start: %d", inum, reTimes, pageSize, Domain.startPos);
	uint32_ nowSize = inum;
	for(blockNum = 0; blockNum<reTimes; blockNum++)
	{
		uint32_ start = blockNum * pageSize + Domain.startPos;

		if(nowSize < pageSize)
			pageSize = nowSize;
		
		for(uint32_ i=start; i<start+pageSize; ++i)
		{
			memset(fakeMsg, 0, newLen * sizeof(uint8_));
			LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::INFO,
						   "[Received] REQ:%x REQCODE: %x, cardId: %s sceneryId: %s cardType: %x actionId: %x checkinTime: %x", 
					       std::string((char_*)zmqMsg->deviceId, sizeof(zmqMsg->deviceId)).c_str(), 
					       ntohl(zmqMsg->msgCode), 
					       std::string((char_*)tmpBody[i].cardId, sizeof(tmpBody[i].cardId)).c_str(),
						   std::string((char_*)tmpBody[i].sceneryId, sizeof(tmpBody[i].sceneryId)).c_str(),
					       ntohl(tmpBody[i].cardType), ntohl(tmpBody[i].actionId), ntohl(tmpBody[i].checkinTime));

			TS_REQ_SERVER_MSG* fakeCardMsg = (TS_REQ_SERVER_MSG*)fakeMsg;
			memcpy(fakeCardMsg->deviceId, zmqMsg->deviceId, sizeof(fakeCardMsg->deviceId));
			fakeCardMsg->msgCode  = zmqMsg->msgCode;
			memcpy(fakeCardMsg->customMsgBody, &tmpBody[i], sizeof(TS_DEVICE_CARD_DATA_REQ_BODY));

			Data_Ptr tmpData;
			tmpData.reset();
			uint32_  len = 0;
			if(acdObj->DeviceCardDataMsgProcess(workDbHandle, fakeMsg, sizeof(TS_REQ_SERVER_MSG) + sizeof(TS_DEVICE_CARD_DATA_REQ_BODY),
										        tmpData, len) != LWDP_OK)
			{
				errList->push_back(std::string((char_*)tmpBody[i].cardId, sizeof(tmpBody[i].cardId)));
			}
	
		}

		nowSize -= pageSize;
	}

	DELETE_MULTIPLE(fakeMsg);
	GET_OBJECT_RET(DbMgr, iDbMgr, NULL);
	iDbMgr->Close(workDbHandle);

	if(errList->empty())
	{
		DELETE_SINGLE(errList);
		return NULL;
	}

	return errList;
}

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
	
	XPropertys propThreadNum;
	iConfigMgr->GetModulePropEntry(LW_ACDEVICE_MODULE_NAME, LW_ACDEVICE_BULK_THREAD_NUM_NAME, propThreadNum);
	if(!propThreadNum[0].propertyText.empty())
	{
		gThreadCount = atol(propThreadNum[0].propertyText.c_str());
	}
	else
	{
		LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <BulkThreadNum> In Config File, Default(%d)", 
					   gThreadCount);
	}	
	
	XPropertys propDomainSize;
	iConfigMgr->GetModulePropEntry(LW_ACDEVICE_MODULE_NAME, LW_ACDEVICE_BULK_PAGESIZE_NAME, propDomainSize);
	if(!propDomainSize[0].propertyText.empty())
	{
		processPageSize = atol(propDomainSize[0].propertyText.c_str());
	}
	else
	{
		LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <BulkPageSize> In Config File, Default(%s)", 
					   processPageSize);
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


static std::string Data2String(const char_* cstr, uint32_ cstr_len)
{
	if(!cstr)
		return static_cast<std::string>("");
	int i = 0;
	for(i=0; i<cstr_len; ++i)
	{
		if(cstr[i] == 0)
			break;
	}

	if(i != cstr_len)
	{
		return std::string(cstr, strlen(cstr));
	}
	else
	{
		return std::string(cstr, cstr_len);
	}
}



LWRESULT Cx_ACDevice::DeviceInitMsgProcess(DBHandle db_handle, const uint8_* ret_msg, uint32_ ret_msg_len, 
								      Data_Ptr& send_msg, uint32_& send_msg_len)
{
	if(!ret_msg)
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::ERR, 
					   "Cx_ACDevice::DeviceInitMsgProcess Param ret_msg is NULL");
		return LWDP_PARAMETER_ERROR;
	}
	
	TS_REQ_SERVER_MSG* zmqMsg = (TS_REQ_SERVER_MSG*)ret_msg;
	TS_DEVICE_INIT_REQ_BODY* msgBody = (TS_DEVICE_INIT_REQ_BODY*)zmqMsg->customMsgBody;
	if(!msgBody)
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::WARNING, 
				       "Msg(%d) Body Error", zmqMsg->msgCode);
		
		uint8_* errMsg = new uint8_[sizeof(TS_RSP_SERVER_MSG)] ;
		ASSERT_CHECK_RET(LWDP_PLUGIN_LOG, LWDP_MALLOC_MEMORY_ERROR, errMsg, 
						 "Cx_ACDevice::DeviceInitMsgProcess alloc memory error");

		memset(errMsg, 0, sizeof(TS_RSP_SERVER_MSG));
		TS_RSP_SERVER_MSG* errStru = (TS_RSP_SERVER_MSG*)errMsg;
		errStru->msgLength = htonl(sizeof(TS_RSP_SERVER_MSG));
		errStru->rspCode   = htonl(TS_SERVER_MSG_BODY_ERR);
		memcpy(errStru->rspMsg, "Body Empty", 11);

		Data_Ptr tmpData(errMsg);
		send_msg = tmpData;
		send_msg_len = sizeof(TS_RSP_SERVER_MSG);
		return LWDP_OK;
	}

	//check
	LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::INFO,
				   "[Received] REQ:%s REQCODE: %x, checkResult: %x deviceType: %x sceneryId: %s cardKey: %s checkInfo: %s", 
			       Data2String(zmqMsg->deviceId, sizeof(zmqMsg->deviceId)).c_str(), 
			       ntohl(zmqMsg->msgCode), ntohl(msgBody->checkResult), ntohl(msgBody->deviceAction), 
				   Data2String(msgBody->sceneryId, sizeof(msgBody->sceneryId)).c_str(),
				   Data2String(msgBody->cardKey, sizeof(msgBody->cardKey)).c_str(),
				   Data2String(msgBody->checkResultInfo, sizeof(msgBody->checkResultInfo)).c_str());


	std::string clientScId = Data2String(msgBody->sceneryId, sizeof(msgBody->sceneryId));
	uint32_ checkRes = ntohl(msgBody->checkResult);
	if(checkRes)
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::WARNING, 
				       "Device(%s) SelfCheck Error(%d)", clientScId.c_str(), checkRes);
		
		uint8_* errMsg = new uint8_[sizeof(TS_RSP_SERVER_MSG)] ;
		ASSERT_CHECK_RET(LWDP_PLUGIN_LOG, LWDP_MALLOC_MEMORY_ERROR, errMsg, 
						 "Cx_ACDevice::DeviceInitMsgProcess alloc memory error");

		memset(errMsg, 0, sizeof(TS_RSP_SERVER_MSG));
		TS_RSP_SERVER_MSG* errStru = (TS_RSP_SERVER_MSG*)errMsg;
		errStru->msgLength = htonl(sizeof(TS_RSP_SERVER_MSG));
		errStru->rspCode   = htonl(TS_SERVER_FORBIDDEN_DEV_SETUP);
		memcpy(errStru->rspMsg, "forbidden setup", 15);

		Data_Ptr tmpData(errMsg);
		send_msg = tmpData;
		send_msg_len = sizeof(TS_RSP_SERVER_MSG);
		return LWDP_OK;
	}

	char_ buffer[2048] = {0};
	std::string deviceIdStr = Data2String(zmqMsg->deviceId, sizeof(zmqMsg->deviceId));
	Api_snprintf(buffer, 2047, Cx_ACDevice::mInitSql.c_str(), 
		         deviceIdStr.c_str());

	GET_OBJECT_RET(DbMgr, iDbMgr, LWDP_GET_OBJECT_ERROR);
	Cx_Interface<Ix_DbQuery> gateQuery;
	LWRESULT queryRes = 0;
	if((queryRes = iDbMgr->QuerySQL(db_handle, buffer, gateQuery)) != LWDP_OK)
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::ERR, 
				       "Msg(%d) DB Select Table Error", ntohl(zmqMsg->msgCode));
		
		uint8_* errMsg = new uint8_[sizeof(TS_RSP_SERVER_MSG)] ;
		ASSERT_CHECK_RET(LWDP_PLUGIN_LOG, LWDP_MALLOC_MEMORY_ERROR, errMsg, 
						 "Cx_ACDevice::DeviceInitMsgProcess alloc memory error");

		memset(errMsg, 0, sizeof(TS_RSP_SERVER_MSG));
		TS_RSP_SERVER_MSG* errStru = (TS_RSP_SERVER_MSG*)errMsg;
		errStru->msgLength = htonl(sizeof(TS_RSP_SERVER_MSG)); 
		errStru->rspCode   = htonl(TS_SERVER_DB_ERR);
		memcpy(errStru->rspMsg, "Db Error", 9);

		Data_Ptr tmpData(errMsg);
		send_msg = tmpData;
		send_msg_len = sizeof(TS_RSP_SERVER_MSG);

		return LWDP_OK;
	}

	if(gateQuery->NumRow() == 0)
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::WARNING, 
			       "Can't Find Device ID(%s)", 
			       Data2String(zmqMsg->deviceId, sizeof(zmqMsg->deviceId)).c_str());

		uint8_* returnMsg = new uint8_[sizeof(TS_RSP_SERVER_MSG)];
		ASSERT_CHECK_RET(LWDP_PLUGIN_LOG, LWDP_MALLOC_MEMORY_ERROR, returnMsg, "Malloc Error");

		memset(returnMsg, 0, sizeof(TS_RSP_SERVER_MSG));
		TS_RSP_SERVER_MSG* retStru = (TS_RSP_SERVER_MSG*)returnMsg;
		retStru->msgLength = htonl(sizeof(TS_RSP_SERVER_MSG));
		retStru->rspCode   = htonl(TS_SERVER_ID_ERROR); //
		memcpy(retStru->rspMsg, "Can't Find Device ID", 21);

		Data_Ptr tmpData(returnMsg);
		send_msg = tmpData;
		send_msg_len = sizeof(TS_RSP_SERVER_MSG);
		
		return LWDP_OK;
	}

	uint32_ retCode = TS_SERVER_OK;
	char_*  retStr  = "Check OK!";
	std::string scidCol   = Cx_ACDevice::gateInfoTable[LW_ACDEVICE_GATE_INFO_SCENERY_ID_COL].propertyText;
	std::string scidValue = gateQuery->GetStringField(scidCol, "");
	ASSERT_CHECK_HALT(LWDP_PLUGIN_LOG, !scidValue.empty(), "scidValue Empty");
	if(scidValue != clientScId)
	{
		retCode = TS_SERVER_CHECK_OK_RECONFIG;
		retStr = "SceneryId Reconfig";
	}

	//std::string positionCol   = Cx_ACDevice::gateInfoTable[LW_ACDEVICE_GATE_INFO_POSITION_COL].propertyText;
	//std::string positionValue = gateQuery->GetStringField(positionCol, "");
	//ASSERT_CHECK_HALT(LWDP_PLUGIN_LOG, !positionValue.empty(), "positionValue Empty");
	//uint32_ posInt = atol(positionValue.c_str());
	//if(posInt != msgBody->position)
	//{
	//	retCode = TS_SERVER_CHECK_OK_RECONFIG;
	//	retStr  = "Position Reconfig";
	//}

	
#if 0
	//ÈôÓÉ·þÎñÆ÷ÅäÖÃÃÅ½û»ú½ø³ö½ÇÉ«£¬ËùÓÐÅäÖÃÐèÔÚ·þÎñÆ÷Î¬»¤£¬²»¹»Áé»î
	std::string typeCol   = Cx_ACDevice::gateInfoTable[LW_ACDEVICE_GATE_INFO_TYPE_COL].propertyText;
	std::string typeValue = gateQuery->GetStringField(typeCol, "");
	ASSERT_CHECK_HALT(LWDP_PLUGIN_LOG, !typeValue.empty(), "typeValue Empty");
	uint32_ typeInt = atol(typeValue.c_str());
	if(typeInt != msgBody->deviceAction)
	{
		retCode = TS_SERVER_CHECK_OK_RECONFIG;
		retStr  = "Gate Type Reconfig";
	}
#endif

	uint8_* returnMsg = new uint8_[sizeof(TS_RSP_SERVER_MSG)];
	ASSERT_CHECK_RET(LWDP_PLUGIN_LOG, LWDP_MALLOC_MEMORY_ERROR, returnMsg, "Malloc Error");

	memset(returnMsg, 0, sizeof(TS_RSP_SERVER_MSG));
	TS_RSP_SERVER_MSG* retStru = (TS_RSP_SERVER_MSG*)returnMsg;
	retStru->msgLength = htonl(sizeof(TS_RSP_SERVER_MSG));
	retStru->rspCode   = htonl(retCode); //
	memcpy(retStru->rspMsg, retStr, strlen(retStr));

	Data_Ptr tmpData(returnMsg);
	send_msg = tmpData;
	send_msg_len = sizeof(TS_RSP_SERVER_MSG);
	
	return LWDP_OK;
}


LWRESULT Cx_ACDevice::DeviceConfigMsgProcess(DBHandle db_handle,const uint8_* ret_msg, uint32_ ret_msg_len, 
								      Data_Ptr& send_msg, uint32_& send_msg_len)
{
	if(!ret_msg)
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::ERR, 
					   "Cx_ACDevice::DeviceInitMsgProcess Param ret_msg is NULL");
		return LWDP_PARAMETER_ERROR;
	}
	
	uint8_* returnMsg = NULL;
	Data_Ptr tmpData;
	TS_REQ_SERVER_MSG* zmqMsg = (TS_REQ_SERVER_MSG*)ret_msg;
	GET_OBJECT_RET(CommonUtilMgr, iCommonUtilMgr, LWDP_GET_OBJECT_ERROR);

	//check
	LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::INFO,
				   "[Received] REQ:%s REQCODE: %x", 
			       Data2String(zmqMsg->deviceId, sizeof(zmqMsg->deviceId)).c_str(), 
			       ntohl(zmqMsg->msgCode));

	char_ buffer[2048] = {0};
	Api_snprintf(buffer, 2047, Cx_ACDevice::mConfigSql.c_str(), 
				 Data2String(zmqMsg->deviceId, sizeof(zmqMsg->deviceId)).c_str());

	LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::DEBUG,
				   "Config Select Db Str(%s)", buffer);

	GET_OBJECT_RET(DbMgr, iDbMgr, LWDP_GET_OBJECT_ERROR);
	Cx_Interface<Ix_DbQuery> gateQuery;
	LWRESULT queryRes = 0;
	uint32_ retCode = 0;
	char_*  retMsg  = "Sucess!";
	//iDbMgr->Ping();
	if((queryRes = iDbMgr->QuerySQL(db_handle, buffer, gateQuery)) != LWDP_OK)
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::ERR, 
				       "Msg(%d) DB Select(%s) Table Error", 
				       ntohl(zmqMsg->msgCode), buffer);
		
		retCode = TS_SERVER_DB_ERR;
		retMsg  = "DB Read Error";
		goto ERR_RET;
	}

	if(gateQuery->NumRow() == 0)
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::ERR, 
				       "Msg(%d) Can't Find id(%d) Select(%s) Table Error", 
				       ntohl(zmqMsg->msgCode), 
				       Data2String(zmqMsg->deviceId, sizeof(zmqMsg->deviceId)).c_str(), 
				       buffer);
		
		retCode = TS_SERVER_ID_ERROR;
		retMsg  = "Unknow Device";
		
		goto ERR_RET;
	}

	returnMsg = new uint8_[sizeof(TS_RSP_SERVER_MSG) +
						   sizeof(TS_DEV_CONFIG_RSP_BODY)];
	{
		ASSERT_CHECK_RET(LWDP_PLUGIN_LOG, LWDP_MALLOC_MEMORY_ERROR, returnMsg, "Malloc Error");

		memset(returnMsg, 0, sizeof(TS_RSP_SERVER_MSG) + 
							 sizeof(TS_DEV_CONFIG_RSP_BODY));
		TS_RSP_SERVER_MSG* retStru = (TS_RSP_SERVER_MSG*)returnMsg;
		retStru->msgLength = htonl(sizeof(TS_RSP_SERVER_MSG) +
						           sizeof(TS_DEV_CONFIG_RSP_BODY));
		retStru->rspCode   = TS_SERVER_OK; //
		memcpy(retStru->rspMsg, "Sucess!", 8);
		TS_DEV_CONFIG_RSP_BODY* retBody = (TS_DEV_CONFIG_RSP_BODY*)retStru->customMsgBody;
		
		//scenery
		std::string scidCol   = Cx_ACDevice::gateInfoTable[LW_ACDEVICE_GATE_INFO_SCENERY_ID_COL].propertyText;
		std::string scidValue = gateQuery->GetStringField(scidCol, "");
		ASSERT_CHECK_HALT(LWDP_PLUGIN_LOG, !scidValue.empty(), "scidValue Empty");
		memcpy(retBody->sceneryId, scidValue.c_str(), sizeof(retBody->sceneryId));

		//position
		//std::string positionCol   = Cx_ACDevice::gateInfoTable[LW_ACDEVICE_GATE_INFO_POSITION_COL].propertyText;
		//std::string positionValue = gateQuery->GetStringField(positionCol, "");
		//ASSERT_CHECK_HALT(LWDP_PLUGIN_LOG, !positionValue.empty(), "positionValue Empty");
		//retBody->sceneryPostion = atol(positionValue.c_str());

		//type
		std::string typeCol   = Cx_ACDevice::gateInfoTable[LW_ACDEVICE_GATE_INFO_TYPE_COL].propertyText;
		std::string typeValue = gateQuery->GetStringField(typeCol, "");
		ASSERT_CHECK_HALT(LWDP_PLUGIN_LOG, !typeValue.empty(), "typeValue Empty");
		retBody->deviceAction = htonl(atol(typeValue.c_str()));

	}

	tmpData.reset(returnMsg);
	send_msg = tmpData;
	send_msg_len = sizeof(TS_RSP_SERVER_MSG) + 
				   sizeof(TS_DEV_CONFIG_RSP_BODY);

	return LWDP_OK;
	
ERR_RET:
	uint8_* returnErrMsg = new uint8_[sizeof(TS_RSP_SERVER_MSG)];
	ASSERT_CHECK_RET(LWDP_PLUGIN_LOG, LWDP_MALLOC_MEMORY_ERROR, returnErrMsg, "Malloc Error");

	memset(returnErrMsg, 0, sizeof(TS_RSP_SERVER_MSG));
	TS_RSP_SERVER_MSG* retStru = (TS_RSP_SERVER_MSG*)returnErrMsg;
	retStru->msgLength = htonl(sizeof(TS_RSP_SERVER_MSG));
	retStru->rspCode   = htonl(retCode); //
	iCommonUtilMgr->StrConvert("gb2312","utf-8", retMsg,
	                           strlen(retMsg),
	                           retStru->rspMsg, sizeof(retStru->rspMsg));


	Data_Ptr tmpErrData(returnErrMsg);
	send_msg = tmpErrData;
	send_msg_len = sizeof(TS_RSP_SERVER_MSG);
	return LWDP_OK;
}

/*
enum TS_SERVER_HB_MSG_RESAULT_ENUM
{	
	TS_SERVER_HB_ERROR = 1, //Ê§ï¿½ï¿½
};

typedef struct stru_dev_status_req_body
{	
	uint32_ statusCode;  //×´Ì¬ï¿½ï¿½
}TS_DEV_STATUS_REQ_BODY;

typedef struct stru_dev_status_rsp_body
{	
	uint32_  msgResult; //ï¿½ï¿½Ï¢ï¿½ï¿½ï¿½Í½ï¿½ï¿?
	char_    msgResultData[32];  // ï¿½ï¿½ï¿½ÜµÄ´ï¿½ï¿½ï¿½ï¿½ï¿½Ï¢ï¿½Ö·ï¿½
	uint32_  oprationCode;  //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	uint32_  appendDataLength; //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ý³ï¿½ï¿½ï¿?
	uint8_   appendData[1];
}TS_DEV_STATUS_RSP_BODY;

*/
LWRESULT Cx_ACDevice::DeviceHBMsgProcess(DBHandle db_handle,const uint8_* ret_msg, uint32_ ret_msg_len, 
								      Data_Ptr& send_msg, uint32_& send_msg_len)
{
	if(!ret_msg)
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::ERR, 
					   "Cx_ACDevice::DeviceInitMsgProcess Param ret_msg is NULL");
		return LWDP_PARAMETER_ERROR;
	}
	
	TS_REQ_SERVER_MSG* zmqMsg = (TS_REQ_SERVER_MSG*)ret_msg;
	TS_DEV_STATUS_REQ_BODY* msgBody = (TS_DEV_STATUS_REQ_BODY*)zmqMsg->customMsgBody;
	if(!msgBody)
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::WARNING, 
				       "Msg(%d) Body Error", ntohl(zmqMsg->msgCode));
		
		uint8_* errMsg = new uint8_[sizeof(TS_RSP_SERVER_MSG)] ;
		memset(errMsg, 0, sizeof(TS_RSP_SERVER_MSG));
		TS_RSP_SERVER_MSG* errStru = (TS_RSP_SERVER_MSG*)errMsg;
		errStru->msgLength = htonl(sizeof(TS_RSP_SERVER_MSG));
		errStru->rspCode   = htonl(TS_SERVER_MSG_BODY_ERR);
		memcpy(errStru->rspMsg, "Body Empty", 11);

		Data_Ptr tmpData(errMsg);
		send_msg = tmpData;
		send_msg_len = sizeof(TS_RSP_SERVER_MSG);
		return LWDP_OK;
	}

	//check
	LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::INFO,
				   "[Received] REQ:%s REQCODE: %x, statusCode: %x", 
			       Data2String(zmqMsg->deviceId, sizeof(zmqMsg->deviceId)).c_str(), 
			       ntohl(zmqMsg->msgCode), msgBody->statusCode);


	uint8_* returnMsg = new uint8_[sizeof(TS_RSP_SERVER_MSG) + 
								   sizeof(TS_DEV_STATUS_RSP_BODY)];
	ASSERT_CHECK_RET(LWDP_PLUGIN_LOG, LWDP_MALLOC_MEMORY_ERROR, returnMsg, "Malloc Error");

	memset(returnMsg, 0, sizeof(TS_RSP_SERVER_MSG) + 
					     sizeof(TS_DEV_STATUS_RSP_BODY));
	TS_RSP_SERVER_MSG* retStru = (TS_RSP_SERVER_MSG*)returnMsg;
	retStru->msgLength = htonl(sizeof(TS_RSP_SERVER_MSG) + 
					           sizeof(TS_DEV_STATUS_RSP_BODY));
	retStru->rspCode   = htonl(TS_SERVER_OK); //
	memcpy(retStru->rspMsg, "Sucess!", 8);	

	Data_Ptr tmpData(returnMsg);
	send_msg = tmpData;
	send_msg_len = sizeof(TS_RSP_SERVER_MSG) + 
				   sizeof(TS_DEV_STATUS_RSP_BODY);
	
	return LWDP_OK;
}

/*
enum TS_CARD_DATA_MSG_RESAULT_ENUM
{	
	TS_SERVER_WRITE_DATA_ERROR = 1, //Ð´ï¿½ï¿½Ý¿ï¿½Ê§ï¿½ï¿?
	TS_SERVER_UNKONW_ERROR,     //ï¿½è±¸IDÎ´Öªï¿½ï¿½ï¿½ï¿½
};

typedef struct stru_dev_card_data_msg
{
	uint8_   cardId[8];  
    uint8_   sceneryId[8];
	uint32_  cardType;   //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	uint32_  actionId;   //ï¿½ï¿½ÎªID
	time_t   checkinTime;
}TS_DEVICE_CARD_DATA_REQ_BODY;

typedef struct stru_dev_card_data_rsp_msg
{	
	uint32_  msgResult; //ï¿½ï¿½Ï¢ï¿½ï¿½ï¿½Í½ï¿½ï¿?
	char_    msgResultData[32];  // ï¿½ï¿½ï¿½ÜµÄ´ï¿½ï¿½ï¿½ï¿½ï¿½Ï¢ï¿½Ö·ï¿½
} TS_DEVICE_CARD_DATA_RSP_BODY;


*/

LWRESULT Cx_ACDevice::DeviceCardDataMsgProcess(DBHandle db_handle,const uint8_* ret_msg, uint32_ ret_msg_len, 
								      Data_Ptr& send_msg, uint32_& send_msg_len)
{
	if(!ret_msg)
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::ERR, 
					   "Cx_ACDevice::DeviceInitMsgProcess Param ret_msg is NULL");
		return LWDP_PARAMETER_ERROR;
	}

	std::string carIdStr 	= "";
	std::string scenicIdStr = "";
	std::string ticketTypeStr = "";
	std::string ticketTypeTab = "";
	int32_      checkAction = -1;
	uint32_     cardType    = 0;
	uint32_     ticketType  = 0;
	
	char_  buffer[3072] = {0};
	int32_ cardStatus 	= -1;

	int32_  queryRes = 0;
	char_ 	bufTime[1024] = {0};
	uint32_ retCode  = 0;
	char_*  retMsg   = "Sucess!";
	
	time_t timep 		= time(NULL);//ntohl(msgBody->checkinTime);
	struct tm checkTime = {0};
	GET_OBJECT_RET(CommonUtilMgr, iCommonUtilMgr, LWDP_GET_OBJECT_ERROR);
	GET_OBJECT_RET(DbMgr, iDbMgr, LWDP_GET_OBJECT_ERROR);

	LWRESULT funStatus = 0;
	TS_REQ_SERVER_MSG* zmqMsg = (TS_REQ_SERVER_MSG*)ret_msg;
	TS_DEVICE_CARD_DATA_REQ_BODY* msgBody = (TS_DEVICE_CARD_DATA_REQ_BODY*)zmqMsg->customMsgBody;
	if(!msgBody)
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::WARNING, 
				       "Msg(%d) Body Error", ntohl(zmqMsg->msgCode));
		
		retCode   = TS_SERVER_MSG_BODY_ERR;
		retMsg    = "Body Empty";
		funStatus = LWDP_ERROR;
		goto RET_TAG;
	}

	//check
	carIdStr    = Data2String((char_*)msgBody->cardId, sizeof(msgBody->cardId));
	scenicIdStr = Data2String((char_*)msgBody->sceneryId, sizeof(msgBody->sceneryId));
	checkAction = ntohs(msgBody->actionId);
	cardType    = ntohs(msgBody->cardType);
	ticketTypeStr = carIdStr.substr(LW_ACDEVICE_TICKET_TYPE_POS, LW_ACDEVICE_TICKET_TYPE_LENGTH); 
	ticketType  = atol(ticketTypeStr.c_str());

	
	LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::INFO,
				   "[Received] REQ:%s REQCODE: %x, cardId: %s sceneryId: %s cardType: %x ticketType: %x actionId: %x checkinTime: %x", 
			       Data2String((char_*)zmqMsg->deviceId, sizeof(zmqMsg->deviceId)).c_str(), 
			       ntohl(zmqMsg->msgCode), 
				   carIdStr.c_str(), 
			       scenicIdStr.c_str(), 
			       ntohs(msgBody->cardType), ticketType, ntohs(msgBody->actionId), ntohl(msgBody->checkinTime));

	if(!checkAction) //CheckIn
	{
		if((retCode=cardCheckIn(db_handle, carIdStr, cardType, ticketType, scenicIdStr, cardStatus, &retMsg)) != LWDP_OK)
		{
			LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::WARNING, 
					       "CheckIn Card Info Error", ntohl(zmqMsg->msgCode));
			funStatus = retCode;
			goto RET_TAG;
		}
	}
	else
	{
		if((retCode=cardCheckOut(db_handle, carIdStr, cardType, ticketType, scenicIdStr, cardStatus, &retMsg)) != LWDP_OK)
		{
			LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::WARNING, 
					       "CheckOut Card Info Error", ntohl(zmqMsg->msgCode));
			funStatus = retCode;
			goto RET_TAG;
		}
	}

	//Insert Card Status
	GET_SWIPING_TAB(ticketType, ticketTypeTab);
	localtime_r(&timep, &checkTime); 
	strftime(bufTime, 1024, "%Y-%m-%d %H:%M:%S", &checkTime);
	Api_snprintf(buffer, 3071, Cx_ACDevice::mCardSql.c_str(), ticketTypeTab.c_str(),
															   carIdStr.c_str(), 
															   std::string((char_*)zmqMsg->deviceId, sizeof(zmqMsg->deviceId)-1).c_str(),
				  											   std::string((char_*)msgBody->sceneryId, sizeof(msgBody->sceneryId)-1).c_str(),
				  								               ntohs(msgBody->actionId),
				  								               bufTime);
				  								               //msgBody->checkinTime);

	LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::DEBUG,
				   "Select Db Str(%s)", buffer);

	//iDbMgr->Ping();

	if((queryRes = iDbMgr->ExecSQL(db_handle, buffer)) != 1)
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::ERR, 
				       "Msg(%d) DB Insert(%s) Table Error", 
				       ntohl(zmqMsg->msgCode), buffer);

		retCode = TS_SERVER_DB_ERR;
		retMsg  = "Insert Error";
		funStatus = LWDP_ERROR;
	}


RET_TAG:
	uint8_* returnMsg = new uint8_[sizeof(TS_RSP_SERVER_MSG)];
	ASSERT_CHECK_RET(LWDP_PLUGIN_LOG, LWDP_MALLOC_MEMORY_ERROR, returnMsg, "Malloc Error");

	memset(returnMsg, 0, sizeof(TS_RSP_SERVER_MSG));
	TS_RSP_SERVER_MSG* retStru = (TS_RSP_SERVER_MSG*)returnMsg;
	retStru->msgLength = htonl(sizeof(TS_RSP_SERVER_MSG));
	retStru->rspCode   = htonl(retCode); //
	iCommonUtilMgr->StrConvert("gb2312","utf-8", retMsg,
	                           strlen(retMsg),
	                           retStru->rspMsg, sizeof(retStru->rspMsg));

	Data_Ptr tmpData(returnMsg);
	send_msg = tmpData;
	send_msg_len = sizeof(TS_RSP_SERVER_MSG);
	
	return funStatus;
}



/*
enum TS_BULK_DATA_MSG_RESAULT_ENUM
{	
	TS_SERVER_BULK_WRITE_DATA_ERROR = 1, //Ð´ï¿½ï¿½Ý¿ï¿½Ê§ï¿½ï¿?
	TS_SERVER_BULK_UNKONW_ERROR,     //ï¿½è±¸IDÎ´Öªï¿½ï¿½ï¿½ï¿½
};

typedef struct stru_dev_bulk_data_msg
{  
	uint32_  cardDataCount;   //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä¿ï¿½ï¿½ï¿½ï¿?
	uint8_*  cardDataEntry;   //ï¿½ï¿½ÎªID
}TS_DEVICE_BULK_DATA_REQ_BODY;

typedef struct stru_dev_bulk_data_rsp_msg
{	
	uint32_  msgResult; //ï¿½ï¿½Ï¢ï¿½ï¿½ï¿½Í½ï¿½ï¿?
	char_    msgResultData[32];  // ï¿½ï¿½ï¿½ÜµÄ´ï¿½ï¿½ï¿½ï¿½ï¿½Ï¢ï¿½Ö·ï¿½
	uint32_  errorEntryNum;
	uint8_   errCardId[1]; //errorEntryNum * 8
} TS_DEVICE_BULK_DATA_RSP_BODY;
*/

LWRESULT Cx_ACDevice::DeviceBulkDataMsgProcess(DBHandle db_handle,const uint8_* ret_msg, uint32_ ret_msg_len, 
								      Data_Ptr& send_msg, uint32_& send_msg_len)
{
	if(!ret_msg)
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::ERR, 
					   "Cx_ACDevice::DeviceInitMsgProcess Param ret_msg is NULL");
		return LWDP_PARAMETER_ERROR;
	}
	
	TS_REQ_SERVER_MSG* zmqMsg = (TS_REQ_SERVER_MSG*)ret_msg;
	TS_DEVICE_BULK_DATA_REQ_BODY* msgBody = (TS_DEVICE_BULK_DATA_REQ_BODY*)zmqMsg->customMsgBody;
	if(!msgBody)
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::WARNING, 
				       "Msg(%d) Body Error", ntohl(zmqMsg->msgCode));
		
		uint8_* errMsg = new uint8_[sizeof(TS_RSP_SERVER_MSG)] ;
		memset(errMsg, 0, sizeof(TS_RSP_SERVER_MSG));
		TS_RSP_SERVER_MSG* errStru = (TS_RSP_SERVER_MSG*)errMsg;
		errStru->msgLength = htonl(sizeof(TS_RSP_SERVER_MSG));
		errStru->rspCode   = htonl(TS_SERVER_MSG_BODY_ERR);
		memcpy(errStru->rspMsg, "Body Empty", 11);

		Data_Ptr tmpData(errMsg);
		send_msg = tmpData;
		send_msg_len = sizeof(TS_RSP_SERVER_MSG);
		return LWDP_OK;
	}

	//check	
	LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::NOTICE,
				   "[Received] DEVICEID:%s REQCODE: %x, cardDataCount: %x", 
			       Data2String((char_*)zmqMsg->deviceId, sizeof(zmqMsg->deviceId)).c_str(), 
			       ntohl(zmqMsg->msgCode), ntohl(msgBody->cardDataCount));

	uint32_ tmpLen = sizeof(TS_REQ_SERVER_MSG) + 
		             sizeof(TS_DEVICE_BULK_DATA_REQ_BODY) +
				     sizeof(TS_DEVICE_CARD_DATA_REQ_BODY) * ntohl(msgBody->cardDataCount);
	if(ret_msg_len < tmpLen)
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::ERR,
					   "Request Data is too Small,ret_msg_len(%d) Should(%d) cardDataCount(%d)", 
					   ret_msg_len, tmpLen, ntohl(msgBody->cardDataCount));
		
		uint8_* errMsg = new uint8_[sizeof(TS_RSP_SERVER_MSG)] ;
		memset(errMsg, 0, sizeof(TS_RSP_SERVER_MSG));
		TS_RSP_SERVER_MSG* errStru = (TS_RSP_SERVER_MSG*)errMsg;
		errStru->msgLength = htonl(sizeof(TS_RSP_SERVER_MSG));
		errStru->rspCode   = htonl(TS_SERVER_MSG_BODY_ERR);
		memcpy(errStru->rspMsg, "Body Len too small", 19);

		Data_Ptr tmpData(errMsg);
		send_msg = tmpData;
		send_msg_len = sizeof(TS_RSP_SERVER_MSG);
		
		return LWDP_OK;
	}

	uint32_ retResult    = TS_SERVER_OK;
	char_*  retResultStr = "Check OK!";
	uint32_ retSize      = 0;
	RET_LIST errList;
	std::list<pthread_t>::iterator iter;

	uint32_ thread_num = gThreadCount;
	uint32_ domainSize = 0;
	uint32_ inum = ntohl(msgBody->cardDataCount);
	domainSize = inum / thread_num;
	thread_num += (inum % thread_num)>0?1:0;
		
	LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::NOTICE, 
	           		"TotleNum: %d thread_num: %d domainSize: %d", 
	           		inum, thread_num, domainSize);

	std::list<pthread_t> thrList;
	GET_OBJECT_RET(DbMgr, iDbMgr, NULL);
	uint32_ nowSize = inum;
	for(uint32_ num = 0; num < thread_num; ++num)
	{
		pthread_t t;
		int result = 0;

		DBHandle workDbHandle = iDbMgr->OpenCopy(db_handle);
		if(!workDbHandle)
		{
			LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::ERR, 
			           	   "ACD Thread Connect Mysql Error");
			retResult    = TS_SERVER_DB_ERR;
			retResultStr = "Connect DB Error";
			goto RET_TAG;
		}
	
		WORKTHREAD_PARAM* domain = (WORKTHREAD_PARAM*)malloc(sizeof(WORKTHREAD_PARAM));
		ASSERT_CHECK(domain != 0);
		domain->startPos 	= (num * domainSize);
		domain->length 		= domainSize;
		if(nowSize < domainSize)
		{
			domain->length = nowSize;
		}	
		nowSize -= domainSize;
		domain->dbHandle 	= workDbHandle;
		domain->msg 		= zmqMsg;
		domain->object 		= this;
		result = pthread_create(&t, NULL, work_thread, domain);
		if(result != 0)
		{
			LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::ERR, 
						   "Can't Create Thread Ret: %d\n", result);

			retResult = TS_SERVER_BULK_THREAD_ERR;
			retResultStr = "Bulk Can't Create Thread";
			goto RET_TAG;
		}

		thrList.push_back(t);			
	}

	FOREACH_STL(iter, thrList)
	{
		RET_LIST* work_ret_list;
		int rc = pthread_join(*iter, (void**)&work_ret_list);
		ASSERT_CHECK(rc == 0);
		if(work_ret_list)
		{
			RET_LIST::iterator erriter;
			FOREACH_STL(erriter, (*work_ret_list))
			{
				errList.push_back(*erriter);
			}
	
			DELETE_SINGLE(work_ret_list);
		}
	}

RET_TAG:
	TS_DEVICE_CARD_DATA_REQ_BODY* retTmpBody = NULL;
	retTmpBody = (TS_DEVICE_CARD_DATA_REQ_BODY*)msgBody->cardDataEntry;

	if(!errList.empty())
	{
		retSize = sizeof(TS_RSP_SERVER_MSG) + 
			      sizeof(TS_DEVICE_BULK_DATA_RSP_BODY) +
			      sizeof(retTmpBody->cardId) * errList.size();
	}
	else
	{
		retSize = sizeof(TS_RSP_SERVER_MSG) + 
				  sizeof(TS_DEVICE_BULK_DATA_RSP_BODY);
	}
	
	uint8_* returnMsg = NULL;
	returnMsg = new uint8_[retSize];
	memset(returnMsg, 0, retSize);
	ASSERT_CHECK_RET(LWDP_PLUGIN_LOG, LWDP_MALLOC_MEMORY_ERROR, returnMsg, "Malloc Error");

	TS_RSP_SERVER_MSG* retStru = (TS_RSP_SERVER_MSG*)returnMsg;
	TS_DEVICE_BULK_DATA_RSP_BODY* retBody = (TS_DEVICE_BULK_DATA_RSP_BODY*)retStru->customMsgBody;

	if(!errList.empty())
	{
		retResult    = TS_SERVER_BULK_WRITE_DATA_ERROR;
		retResultStr = "Have Error!";

		retBody->errorEntryNum = errList.size();
		for(uint32_ i = 0; i < retBody->errorEntryNum; ++i)
		{
			memcpy(retBody->errCardId + (sizeof(retTmpBody->cardId) * i), errList[i].data(), sizeof(retTmpBody->cardId));
		}
	}

	retStru->msgLength = htonl(retSize);
	retStru->rspCode   = htonl(retResult); //
	memcpy(retStru->rspMsg, retResultStr, strlen(retResultStr));

	Data_Ptr tmpData(returnMsg);
	send_msg     = tmpData;
	send_msg_len = retSize;
	
	return LWDP_OK;
}

LWRESULT Cx_ACDevice::intArrayToStr(uint32_ int_array[], uint32_ size, std::string& ret_str)
{
#if 0
	std::string formatStr("");
	char_* tmpStrCell = new char_[32];
	for(uint32_ i = 0; i < size; ++i)
	{
		memset(tmpStrCell, 0, 32);
		Api_snprintf(tmpStrCell, 32, "%d", int_array[i]);
		if(!int_array[i])
			break;
		formatStr += std::string(tmpStrCell);
	}

	ret_str = formatStr;
	DELETE_MULTIPLE(tmpStrCell);
#endif
	return LWDP_OK;
}

LWRESULT Cx_ACDevice::checkCard(DBHandle db_handle,
									    const std::string& carIdStr, 
									    uint32_ card_type,
									    uint32_ ticket_type,
		                                const std::string& sceneryIdStr,
								        int32_& statusCode,
								        char_** retMsg)
{
	char_   buffer[3072] = {0};
	int32_  queryRes 	 = 0;
	std::string cardCol  = "card_no";
	std::string ticketTypeTab = "sc_sale_scenic";
	statusCode = -1;
	*retMsg    = "\xCB\xA2\xBF\xA8\xB3\xC9\xB9\xA6";//"Ë¢¿¨³É¹¦!";

	GET_OBJECT_RET(DbMgr, iDbMgr, LWDP_GET_OBJECT_ERROR);

// Ë¢¿¨Ê±²»ÐèÒª¿¼ÂÇ¿¨±¾ÉíµÄ×´Ì¬£¬ÊÛ¿¨»ò¹ÒÊ§Ê±ÒÑ´¦Àí
#if 0
	//Search From sCard
	if(LW_ACDEVICE_CARD_TYPE_M1 == card_type)
	{
		memset(buffer, 0, 3072 * sizeof(char_));
		Api_snprintf(buffer, 3071, 
			         "SELECT card_status FROM sc_card WHERE card_no = '%s'", 
			         carIdStr.c_str());
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::DEBUG,
					   "Select Card Release Status Str(%s)", buffer);
		
		Cx_Interface<Ix_DbQuery> cardReleaseQuery;
		if((queryRes = iDbMgr->QuerySQL(db_handle, buffer, cardReleaseQuery)) != LWDP_OK)
		{
			LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::ERR, 
					       "Msg DB Select sc_card Status(%s) Table Error", 
					       buffer);

			statusCode = -1;
			*retMsg  = "\xCA\xFD\xBE\xDD\xBF\xE2\xB6\xC1\xD0\xB4\xCA\xA7\xB0\xDC";//"Êý¾Ý¿â¶ÁÐ´Ê§°Ü";
			return TS_SERVER_DB_ERR;
		}

		uint32_ cardReleaseRowNum = cardReleaseQuery->NumRow();
		if(0 == cardReleaseRowNum)
		{
			LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::WARNING, 
					       "Can't Find Card(id: %s, scenicId: %s) Select(%s) Table Error", 
	                       carIdStr.c_str(), 
						   sceneryIdStr.c_str(), 
					       buffer);

			statusCode = -1;
			*retMsg = "\xCE\xB4\xD5\xD2\xB5\xBD\xB4\xCB\xBF\xA8\xD0\xC5\xCF\xA2\x21";//"Î´ÕÒµ½´Ë¿¨ÐÅÏ¢!";
			
			return TS_SERVER_CARD_ERROR;
		}

		std::string statusValue = cardReleaseQuery->GetStringField("card_status", "");
		if(statusValue.empty())
		{	
			*retMsg = "\xCE\xB4\xD6\xAA\xBF\xA8\x21";//"Î´Öª¿¨!";
			return TS_SERVER_CARD_ERROR;
		}
		statusCode = atol(statusValue.c_str());
		if(statusCode != LW_ACDEVICE_CARD_STATUS_RELEASE)
		{
			switch(statusCode)
			{   		
				case LW_ACDEVICE_CARD_STATUS_NOTRELEASE:
				{
					*retMsg = "\xB4\xCB\xBF\xA8\xCE\xB4\xB7\xA2\xD0\xD0\x21";//"´Ë¿¨Î´·¢ÐÐ!";
					break;
				}
				case LW_ACDEVICE_CARD_STATUS_LOST:
				{
					*retMsg = "\xB4\xCB\xBF\xA8\xD2\xD1\xB9\xD2\xCA\xA7\x21";//"´Ë¿¨ÒÑ¹ÒÊ§!";
					break;
				}
				case LW_ACDEVICE_CARD_STATUS_BROKEN:
				{
					*retMsg = "\xB4\xCB\xBF\xA8\xD2\xD1\xCB\xF0\xBB\xB5\x21";//"´Ë¿¨ÒÑËð»µ!";
					break;
				}
				case LW_ACDEVICE_CARD_STATUS_BAN:
				{
					*retMsg = "\xB4\xCB\xBF\xA8\xD4\xDA\xBA\xDA\xC3\xFB\xB5\xA5\xD6\xD0\x21";//"´Ë¿¨ÔÚºÚÃûµ¥ÖÐ!";
					break;
				}
				case LW_ACDEVICE_CARD_STATUS_DISUSE:
				{
					*retMsg = "\xB4\xCB\xBF\xA8\xD2\xD1\xD2\xC5\xC6\xFA\x21";//"´Ë¿¨ÒÑÒÅÆú!";
					break;
				}
				case LW_ACDEVICE_CARD_STATUS_AD_BAN:
				{
					*retMsg = "\xBD\xA8\xD2\xE9\xBC\xD3\xC8\xEB\xBA\xDA\xC3\xFB\xB5\xA5\x21";//"½¨Òé¼ÓÈëºÚÃûµ¥!";
					break;
				}
				default:	
					*retMsg = "\xCE\xB4\xD6\xAA\xBF\xA8\xD7\xB4\xCC\xAC\x21";//"Î´Öª¿¨×´Ì¬!";
			};

			return TS_SERVER_CARD_ERROR;
		}
	}
#endif


	/////////////////////////////////////////////////////////////////////
	GET_CARD_TAB(card_type, cardCol);
	GET_TICKET_TAB(ticket_type, ticketTypeTab);
		
	memset(buffer, 0, 3072 * sizeof(char_));
	Api_snprintf(buffer, 3071, 
		         "SELECT status, ((DATE_FORMAT(NOW(),'%%Y-%%m-%%d') < begin_date) OR \
								(DATE_FORMAT(NOW(),'%%Y-%%m-%%d') > end_date)) DateTimeOut,\
								((DATE_FORMAT(NOW(),'%%H:%%i:%%s') < begin_segment) OR \
								(DATE_FORMAT(NOW(),'%%H:%%i:%%s') > end_segment)) DayTimeOut \
								FROM %s WHERE %s = '%s' AND scenic_id = %s ORDER BY id ASC",  
                 ticketTypeTab.c_str(),
				 cardCol.c_str(), 
				 carIdStr.c_str(), 
			  	 sceneryIdStr.c_str());

	LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::DEBUG,
				   "Select Card Scenic Status Str(%s)", buffer);

	Cx_Interface<Ix_DbQuery> cardQuery;
	if((queryRes = iDbMgr->QuerySQL(db_handle, buffer, cardQuery)) != LWDP_OK)
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::ERR, 
				       "Msg DB Select Card Status(%s) Table Error", 
				       buffer);

		statusCode = -1;
		*retMsg  = "\xCA\xFD\xBE\xDD\xBF\xE2\xB6\xC1\xD0\xB4\xCA\xA7\xB0\xDC"; //"Êý¾Ý¿â¶ÁÐ´Ê§°Ü";
		return TS_SERVER_DB_ERR;
	}

	uint32_ rowNum = cardQuery->NumRow();
	if(0 == rowNum)
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::WARNING, 
				       "Can't Find Card(id: %s, scenicId: %s) Select(%s) Table Error", 
                       carIdStr.c_str(), 
					   sceneryIdStr.c_str(), 
				       buffer);

		statusCode = LW_ACDEVICE_CARD_STATUS_NOTFIND;
		*retMsg = "\xCE\xB4\xD5\xD2\xB5\xBD\xB4\xCB\xBF\xA8\xCF\xFA\xCA\xDB\xD0\xC5\xCF\xA2\x21";//"Î´ÕÒµ½´Ë¿¨ÏúÊÛÐÅÏ¢!";
		
		return TS_SERVER_CARD_ERROR;
	}

	uint32_ i = 0;
	for(i = 0; i < rowNum; ++i, cardQuery->NextRow())
	{
		std::string statusValue = cardQuery->GetStringField("status", "");
		if(statusValue.empty())
		{	
			*retMsg = "\xCE\xB4\xD6\xAA\xBF\xA8\x32\x21";//"Î´Öª¿¨2!";
			continue;
		}
		statusCode = atol(statusValue.c_str());
		if(statusCode != LW_ACDEVICE_CARD_STATUS_NOTUSE)
		{
			switch(statusCode)
			{
				case LW_ACDEVICE_CARD_STATUS_USE:
				{
					*retMsg = "\xB4\xCB\xBF\xA8\xD2\xD1\xCA\xB9\xD3\xC3"; //"´Ë¿¨ÒÑÊ¹ÓÃ";
					continue;
				}
				case LW_ACDEVICE_CARD_STATUS_ABANDON:
				{
					*retMsg = "\xB4\xCB\xBF\xA8\xD2\xD1\xB1\xBB\xD2\xC5\xC6\xFA";//"´Ë¿¨ÒÑ±»ÒÅÆú";
					continue;
				}
				case LW_ACDEVICE_CARD_STATUS_FREEZE:
				{
					*retMsg = "\xB4\xCB\xBF\xA8\xD2\xD1\xB6\xB3\xBD\xE1\x21";//"´Ë¿¨ÒÑ¶³½á!";
					continue;
				}
				default:	
				{
					*retMsg = "\xCE\xB4\xD6\xAA\xBF\xA8\xD7\xB4\xCC\xAC\x21";//"Î´Öª¿¨×´Ì¬!";
					continue;
				}
			};
		}
		else
		{
			std::string isDateTimeOut = cardQuery->GetStringField("DateTimeOut", "");
			std::string isDayTimeOut  = cardQuery->GetStringField("DayTimeOut", "");
			if(isDateTimeOut == "1" && !isDateTimeOut.empty())
			{
				*retMsg = "\xCA\xB9\xD3\xC3\xC6\xDA\xCE\xB4\xB5\xBD\xBB\xF2\xD2\xD1\xB9\xFD\xC6\xDA"; //"Ê¹ÓÃÆÚÎ´µ½»òÒÑ¹ýÆÚ"
				continue;
			}

			if(isDayTimeOut == "1" && !isDayTimeOut.empty())
			{
				*retMsg = "\xCB\xA2\xBF\xA8\xCA\xB1\xBC\xE4\xCE\xB4\xB5\xBD\xBB\xF2\xCB\xA2\xBF\xA8\xD2\xD1\xCD\xA3\xD6\xB9"; //"Ë¢¿¨Ê±¼äÎ´µ½»òË¢¿¨ÒÑÍ£Ö¹"
				continue;
			}
			
			*retMsg = "\xCB\xA2\xBF\xA8\xB3\xC9\xB9\xA6"; //"Ë¢¿¨³É¹¦"
			break;
		}

	}

	if(i == rowNum)
	{
		return TS_SERVER_CARD_ERROR;
	}

	return LWDP_OK;
}
	
LWRESULT Cx_ACDevice::cardCheckIn(DBHandle db_handle, 
									 const std::string& carIdStr, 
									 uint32_ card_type,
									 uint32_ ticket_type,
	                                 const std::string& sceneryIdStr,
									 int32_& statusCode,
									 char_** retMsg)
{
	char_ buffer[3072] = {0};
	int32_  queryRes = 0;
	statusCode  = -1;
	*retMsg  = "\xCB\xA2\xBF\xA8\xB3\xC9\xB9\xA6";//"Ë¢¿¨³É¹¦!";
	LWRESULT retval = 0;
	std::string cardCol = "";
	std::string ticketTypeTab = "";
	if((retval = checkCard(db_handle, carIdStr, card_type, ticket_type, sceneryIdStr, statusCode, retMsg)) != LWDP_OK)
	{
		return retval;
	}

	//ÄêÆ±²»ÐèÒª¸üÐÂ¿¨×´Ì¬
	if(LW_ACDEVICE_TICKET_TYPE_YEAR == ticket_type)
		return LWDP_OK;
	
	//Update Card Status
	GET_CARD_TAB(card_type, cardCol);
	GET_TICKET_TAB(ticket_type, ticketTypeTab);
	
	GET_OBJECT_RET(DbMgr, iDbMgr, LWDP_GET_OBJECT_ERROR);
	memset(buffer, 0 , 3072 * sizeof(char_));
	Api_snprintf(buffer, 3071, "UPDATE %s SET status = 1, update_time = DATE_FORMAT(NOW(),'%%Y-%%m-%%d %%H:%%i:%%s') WHERE %s = '%s' and scenic_id = %s and status = 0",  
								ticketTypeTab.c_str(),
								cardCol.c_str(),
								carIdStr.c_str(), 
							    sceneryIdStr.c_str());
				  					
	LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::DEBUG,
				   "Update Card Status(Checkin) Str(%s)", buffer);

	if((queryRes = iDbMgr->ExecSQL(db_handle, buffer)) != 1)
	{
		LWDP_LOG_PRINT("ACDEVICE", LWDP_LOG_MGR::ERR, 
				       "Msg Update Card status(CheckIn)(%s) Table Error", buffer);

		statusCode = -1;
		*retMsg  = "\xB8\xFC\xD0\xC2\xCA\xFD\xBE\xDD\xBF\xE2\xBF\xA8\xD0\xC5\xCF\xA2\xCA\xA7\xB0\xDC";//"¸üÐÂÊý¾Ý¿â¿¨ÐÅÏ¢Ê§°Ü";

		return TS_SERVER_DB_ERR;
	}
	
	return LWDP_OK;
}

LWRESULT Cx_ACDevice::cardCheckOut(DBHandle db_handle, 
									   const std::string& carIdStr, 
									   uint32_ card_type,
									   uint32_ ticket_type,
	                                   const std::string& sceneryIdStr,
									   int32_& statusCode,
									   char_** retMsg)
{
	char_ buffer[3072] = {0};
	int32_  queryRes = 0;
	statusCode  = -1;
	*retMsg  = "\xCB\xA2\xBF\xA8\xB3\xF6\xC3\xC5\xB3\xC9\xB9\xA6\x21";//"Ë¢¿¨³öÃÅ³É¹¦!";
	LWRESULT retval = 0;
	
	retval = checkCard(db_handle, carIdStr, card_type, ticket_type, sceneryIdStr, statusCode, retMsg);
	if((TS_SERVER_CARD_ERROR == retval) &&
		(LW_ACDEVICE_CARD_STATUS_NOTFIND == statusCode))
	{
		return TS_SERVER_CARD_ERROR;
	}

	if((LW_ACDEVICE_CARD_STATUS_ABANDON == statusCode) || 
		(LW_ACDEVICE_CARD_STATUS_FREEZE == statusCode))
	{
		return TS_SERVER_CARD_ERROR;
	}

	statusCode  = 0;
	*retMsg  = "\xCB\xA2\xBF\xA8\xB3\xF6\xC3\xC5\xB3\xC9\xB9\xA6\x21";//"Ë¢¿¨³öÃÅ³É¹¦!";

	return LWDP_OK;
}


