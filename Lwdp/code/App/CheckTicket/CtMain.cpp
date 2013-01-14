
#include <LwDp.h>
#include <LwApi.h>
using namespace NLwdp;


#include <Interface/ConfigMgr/Ix_XmlMgr.h>
#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/ScriptMgr/Ix_ScriptMgr.h>
#include <Interface/LuaMgr/Ix_LuaMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>
#include <Interface/ZmqMgr/Ix_ZmqMgr.h>
#include <Interface/EventMgr/Ix_EventMgr.h>
#include <Interface/TimerMgr/Ix_TimerMgr.h>
#include <Interface/ConsoleMgr/Ix_ConsoleMgr.h>
#include <Interface/DbMgr/Ix_DbMgr.h>
#include <Interface/CommonUtilMgr/Ix_CommonUtilMgr.h>

#include <../UModule/Interface/TSFrontend/Ix_TSFrontend.h>
#include <../UModule/Interface/TcpServer/Ix_TcpServer.h>


#include <vector>
#include <set>
#include <iostream>
#include <algorithm>
#include <boost/shared_ptr.hpp>

#include "CtDef.h"



class TimerCounter
{
public:
	TimerCounter(std::string pos_str)
	{
		mPos = pos_str;
		GET_OBJECT_VOID(PerfMgr_timer, iPerfMgr_timer);
		mPerfMgr_timer = iPerfMgr_timer;
		mPerfMgr_timer->Start();
	}
	virtual ~TimerCounter()
	{
		float_ psecond = mPerfMgr_timer->End() / 1000.0;
		LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::INFO, 
					   "%s Process Time(%.03f s)", 
					   mPos.c_str(), psecond);
	}

	Cx_Interface<Ix_PerfMgr_timer> mPerfMgr_timer;
	std::string mPos;
};


class ConfigSrcImp : public Ix_ConfigSrc
{
public:
	ConfigSrcImp(){};
	ConfigSrcImp(const tstring& file_name):mFileName(file_name){};
	virtual ~ConfigSrcImp(){};


    virtual PC_DATA* LoadConfigData();
	virtual LWRESULT SaveConfigData(PC_DATA* buffer){return LWDP_OK;};
    virtual LWRESULT ReleseConfigData(){return LWDP_OK;};
	
    virtual int32_   GetConfigDataSize(){return 0;};
    virtual const tchar_*  GetConfigNameStr(){return "LocalFile";};

protected:
	tstring mFileName;

};

PC_DATA* ConfigSrcImp::LoadConfigData()
{
    int32_ file_len = 0;
    char *buf = NULL;
    FILE *fp = NULL;

    fp = fopen(mFileName.c_str(), "r");
    if(!fp)
    {
        printf( "FlashUtility::WriteBspFile----fopen(%s) error!\n", mFileName.c_str());
        return NULL;
    }
                     
    if(0 != fseek(fp, 0, SEEK_END))                  
    {                                                  
        printf("fseek() returns error!\n");        
        fclose(fp);                               
        return NULL;                              
    }                                                  
    else                                               
    {                                                  
        file_len = ftell(fp);                    
        printf("file %s  length is %d\n", mFileName.c_str(), file_len);
    }                                                  
                                                                               
    if(0 != fseek(fp, 0, SEEK_SET))                  
    {                                                  
        printf("fseek() returns error!\n");        
        fclose(fp);                               
        return NULL;                              
    }

	buf = new char[file_len];
    if ( NULL == buf )  //�����ڴ�
    {
        printf( "FlashUtility::WriteBspFile----malloc(%d) error!\r\n", file_len);
        fclose(fp);
        return NULL;
    }
    memset(buf, 0, file_len);

	fread( buf, 1, file_len, fp);
#if 0
	int tt = 0;
    if (file_len != (tt =fread( buf, 1, file_len, fp)))
	{
        free(buf);
        fclose(fp);
        printf("FlashUtility::WriteBspFile----fread(%s, %ld) err!\r\n", mFileName.c_str(), file_len );
        return NULL;
    }
#endif
    fclose( fp );

	return (PC_DATA*)buf;
}

struct ProcessDomain
{
	uint32_ startPos;
	uint32_ length;
};

std::string strDbIp 		= std::string(LW_CT_DB_IP_DEFAULT);
std::string strDbUserName 	= std::string(LW_CT_DB_USER_DEFAULT);
std::string strDbPassword 	= std::string(LW_CT_DB_PASSWORD);
std::string strDbName 		= std::string(LW_CT_DB_SELECT_DBNAME);
uint32_     DbPort 		    = LW_CT_DB_PORT_DEFAULT;

std::string cardSetCount = LW_CT_CARD_SET_COUNT_DEFAULT;
std::string selectFromCards = LW_CT_CARD_SET_DEFAULT;
std::string selectFromScenic = LW_CT_SCENIC_TABLE_DEFAULT;
std::string cardIdCol   = LW_CT_CARDID_COL_DEFAULT;
std::string scenicIdCol = LW_CT_SCENIC_COL_DEFAULT;

std::string updateCardStatus = LW_CT_UPDATE_CARD_STATUS_DEFAULT;
std::string selectCardStatus = LW_CT_SELECT_CARD_STATUS_DEFAULT;
std::string insertCardStatus = LW_CT_INSERT_CARD_STATUS_DEFAULT;
uint32_     processPageSize  = LW_CT_PAGESIZE_DEFAULT;

LWRESULT ConfigRead()
{
	GET_OBJECT_RET(ConfigMgr, iConfigMgr, 0);
	
	XPropertys propDbIp;
	iConfigMgr->GetModulePropEntry(LW_CT_MODULE_NAME, LW_CT_DB_IP_NAME, propDbIp);
	if(!propDbIp[0].propertyText.empty())
	{
		strDbIp = propDbIp[0].propertyText;
	}
	else
	{
		LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <DbIp> In Config File, Default(%s)", strDbIp.c_str());
	}

	XPropertys propDbUserName;
	iConfigMgr->GetModulePropEntry(LW_CT_MODULE_NAME, LW_CT_DB_USER_NAME, propDbUserName);
	if(!propDbUserName[0].propertyText.empty())
	{
		strDbUserName = propDbUserName[0].propertyText;
	}
	else
	{
		LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <DbUser> In Config File, Default(%s)", strDbUserName.c_str());
	}

	XPropertys propDbPassword;
	iConfigMgr->GetModulePropEntry(LW_CT_MODULE_NAME, LW_CT_DB_PASSWORD_NAME, propDbPassword);
	if(!propDbPassword[0].propertyText.empty())
	{
		strDbPassword = propDbPassword[0].propertyText;
	}
	else
	{
		LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <DbPassword> In Config File, Default(%s)", strDbPassword.c_str());
	}

	XPropertys propDbName;
	iConfigMgr->GetModulePropEntry(LW_CT_MODULE_NAME, LW_CT_DB_SELECT_DB_NAME, propDbName);
	if(!propDbName[0].propertyText.empty())
	{
		strDbName = propDbName[0].propertyText;
	}
	else
	{
		LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <DbName> In Config File, Default(%s)", strDbName.c_str());
	}

	XPropertys propDbPort;
	iConfigMgr->GetModulePropEntry(LW_CT_MODULE_NAME, LW_CT_DB_PORT_NAME, propDbPort);
	if(!propDbPort[0].propertyText.empty())
	{
		DbPort = atol(propDbPort[0].propertyText.c_str());
	}
	else
	{
		LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <DbPort> In Config File, Default(%d)", DbPort);
	}	


	XPropertys propSelectFromCards;
	iConfigMgr->GetModulePropEntry(LW_CT_MODULE_NAME, LW_CT_CARD_SET_NAME, propSelectFromCards);
	if(!propSelectFromCards[0].propertyText.empty())
	{
		selectFromCards = propSelectFromCards[0].propertyText;
	}
	else
	{
		LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <CardSet> In Config File, Default(%s)", 
					   selectFromCards.c_str());
	}	

	XPropertys propSelectFromScenic;
	iConfigMgr->GetModulePropEntry(LW_CT_MODULE_NAME, LW_CT_SCENIC_SET_NAME, propSelectFromScenic);
	if(!propSelectFromScenic[0].propertyText.empty())
	{
		selectFromScenic = propSelectFromScenic[0].propertyText;
	}
	else
	{
		LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <ScenicSet> In Config File, Default(%s)", 
					   selectFromScenic.c_str());
	}	

	XPropertys propCardIdCol;
	iConfigMgr->GetModulePropEntry(LW_CT_MODULE_NAME, LW_CT_CARDID_COL_NAME, propCardIdCol);
	if(!propCardIdCol[0].propertyText.empty())
	{
		cardIdCol = propCardIdCol[0].propertyText;
	}
	else
	{
		LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <CardIdCol> In Config File, Default(%s)", 
					   cardIdCol.c_str());
	}
	
	XPropertys propScenicIdCol;
	iConfigMgr->GetModulePropEntry(LW_CT_MODULE_NAME, LW_CT_SCENIC_SET_NAME, propScenicIdCol);
	if(!propScenicIdCol[0].propertyText.empty())
	{
		scenicIdCol = propScenicIdCol[0].propertyText;
	}
	else
	{
		LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <ScenicIdCol> In Config File, Default(%s)", 
					   scenicIdCol.c_str());
	}

	XPropertys propUpdateCardStatus;
	iConfigMgr->GetModulePropEntry(LW_CT_MODULE_NAME, LW_CT_UPDATE_CARD_NAME, propUpdateCardStatus);
	if(!propUpdateCardStatus[0].propertyText.empty())
	{
		updateCardStatus = propUpdateCardStatus[0].propertyText;
	}
	else
	{
		LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <UpdateCard> In Config File, Default(%s)", 
					   updateCardStatus.c_str());
	}

	XPropertys propSelectCardStatus;
	iConfigMgr->GetModulePropEntry(LW_CT_MODULE_NAME, LW_CT_SELECT_CARD_NAME, propSelectCardStatus);
	if(!propSelectCardStatus[0].propertyText.empty())
	{
		selectCardStatus = propSelectCardStatus[0].propertyText;
	}
	else
	{
		LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <InsertCard> In Config File, Default(%s)", 
					   selectCardStatus.c_str());
	}
	
	XPropertys propInsertCardStatus;
	iConfigMgr->GetModulePropEntry(LW_CT_MODULE_NAME, LW_CT_INSERT_CARD_NAME, propInsertCardStatus);
	if(!propInsertCardStatus[0].propertyText.empty())
	{
		insertCardStatus = propInsertCardStatus[0].propertyText;
	}
	else
	{
		LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <InsertCard> In Config File, Default(%s)", 
					   insertCardStatus.c_str());
	}

	XPropertys propPageSize;
	iConfigMgr->GetModulePropEntry(LW_CT_MODULE_NAME, LW_CT_PROCESS_PAGE_NAME, propPageSize);
	if(!propPageSize[0].propertyText.empty())
	{
		processPageSize = atol(propPageSize[0].propertyText.c_str());
	}
	else
	{
		LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <InsertCard> In Config File, Default(%d)", 
					   processPageSize);
	}


	XPropertys propCardSetCount;
	iConfigMgr->GetModulePropEntry(LW_CT_MODULE_NAME, LW_CT_CARD_SET_COUNT_NAME, propCardSetCount);
	if(!propCardSetCount[0].propertyText.empty())
	{
		cardSetCount = propCardSetCount[0].propertyText;
	}
	else
	{
		LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <CardSetCount> In Config File, Default(%s)", 
					   cardSetCount.c_str());
	}


	return LWDP_OK;
}

LWRESULT addBlackList(const std::string& card_no)
{
	GET_OBJECT_RET(DbMgr, iDbMgr, 0);
	
	char tmpStr[2048] = {0};
	int32_ findLine = 0;
	{
		GET_OBJECT_RET(DbQuery, iDbQuery, 0);
		Api_snprintf(tmpStr, 2048, selectCardStatus.c_str(), card_no.c_str());
		iDbMgr->Ping();
		iDbMgr->QuerySQL(tmpStr, iDbQuery);
		findLine = iDbQuery->NumRow();
	}
	if(findLine) 
	{
		Api_snprintf(tmpStr, 2048, updateCardStatus.c_str(), card_no.c_str());
		iDbMgr->Ping();
		int32_ affLine = iDbMgr->ExecSQL(tmpStr);
		//LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::DEBUG, 
		//			   "Update sc_card (%s)", 
		//			   tmpStr);
		if(affLine < 0)
		{

			LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::WARNING, 
				           "Can't Update card_no(%s) From Table sc_card", 
				           card_no.c_str());
			return LWDP_ERROR;
		}
	}
	else
	{
		Api_snprintf(tmpStr, 2048, insertCardStatus.c_str(), card_no.c_str());
		iDbMgr->Ping();
		int32_ intLine = iDbMgr->ExecSQL(tmpStr);
		if(intLine != 1)
		{
			LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::ERR, 
				           "Can't Insert card_no(%s) Errno Black Card", 
				           card_no.c_str());
			return LWDP_ERROR;
		}

	}
	
	return LWDP_OK;
}



void* work_thread(void* arg)
{	
	if(!arg)
	{
		LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::ERR, 
		           	   "Work Thread Parameter is NULL");
		return NULL;
	}

	TimerCounter counter("Work Thread");
		
	ProcessDomain* ptmpDomain = (ProcessDomain*)arg;	
	ProcessDomain Domain;
	Domain.startPos = ptmpDomain->startPos;
	Domain.length   = ptmpDomain->length;
	free(arg);
	ptmpDomain = NULL;
	
	GET_OBJECT_RET(DbMgr, iDbMgr, 0);	

	time_t timep = time(NULL);
	struct tm checkTime = {0};
	localtime_r(&timep, &checkTime); 

	char_ endTime[1024] = {0};
	strftime(endTime, 1024, "%Y-%m-%d", &checkTime);
	char_ startTime[1024] = {0};
	checkTime.tm_mday -= 1;
	strftime(startTime, 1024, "%Y-%m-%d", &checkTime);

	char tmpStr[2048] = {0};
	int32_ inum = Domain.length;
	int32_ pageSize = (processPageSize>Domain.length)? Domain.length: processPageSize;
	int32_ reTimes  = (inum / pageSize) + (inum%pageSize)>0?1:0;

	LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::NOTICE, 
	           		"TotleNum: %d reTimes: %d", inum, reTimes);

	LwdpProgressBar pBar(inum);
	//pBar.start();
	for(int i = 0; i<reTimes; i++)
	{
		GET_OBJECT_RET(DbQuery, iDbQuery, 0);
		int32_ start = i * pageSize + Domain.startPos;
		Api_snprintf(tmpStr, 2048, selectFromCards.c_str(), startTime, "<", endTime, start, pageSize);
		LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::DEBUG, 
					   "%s", tmpStr);
		iDbMgr->Ping();
		iDbMgr->QuerySQL(tmpStr, iDbQuery);

		uint32_ progressCount = 0;
		while(!iDbQuery->Eof())
		{	
			std::string dev_card_no = iDbQuery->GetStringField(cardIdCol, "");
			std::string dev_scenic_id = iDbQuery->GetStringField(scenicIdCol, "");

			Api_snprintf(tmpStr, 2048, selectFromScenic.c_str(), dev_card_no.c_str());
			GET_OBJECT_RET(DbQuery, iScDbQuery, 0);
			iDbMgr->Ping();
			iDbMgr->QuerySQL(tmpStr, iScDbQuery);
			uint32_ iscnum = iScDbQuery->NumRow();
			if(iscnum >= 1)
			{
				while(!iScDbQuery->Eof())
				{
					std::string true_scenic_id = iScDbQuery->GetStringField(scenicIdCol, "");
					if(true_scenic_id == dev_scenic_id)
						break;
				}

				if(iScDbQuery->Eof()) //can't find
				{
					addBlackList(dev_card_no);
				}
			}
			else 
			{
				addBlackList(dev_card_no);
			}
			
			iDbQuery->NextRow();

			progressCount++;
			//if(progressCount >= 200)
				//pBar += 200;
		}
	}


	LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::NOTICE, 
		           "Check OK!!");

	return NULL;
}

int32_ main()
{

	LWRESULT stat = LWDP_ERROR;

#if defined(LWDP_PLATFORM_DEF_WIN32)
	ConfigSrcImp csrc("../../../../bin/xml/CtConfigExternal.xml");
#elif defined(LWDP_PLATFORM_DEF_LINUX)
	//ConfigSrcImp csrc("/home/ptsf/Desktop/tmp/workspace/LwdpGit/Lwdp/code/bin/xml/LinuxConfigExternal.xml");
	ConfigSrcImp csrc("../../../bin/xml/LinuxConfigExternal.xml");
#endif
	stat = Fw_Init(&csrc, 1);
	if(stat != LWDP_OK)
	{
		lw_log_err(LWDP_MODULE_LOG, "Fw_Init Error(0x%x)!", stat);
		system("pause");
		return -1;
		
	} 

	{
		TimerCounter counter("Main Thread");
		RINOKR(ConfigRead(), NULL);

		GET_OBJECT_RET(DbMgr, iDbMgr, 0);	
		LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::INFO, 
					   "Connect to Db Server Ip:%s User:%s DbName:%s Port:%d", 
					   strDbIp.c_str(), strDbUserName.c_str(), strDbName.c_str(), DbPort);
		if(iDbMgr->Open( strDbIp.c_str(), 
				         strDbUserName.c_str(), 
				         strDbPassword.c_str(), 
				         strDbName.c_str(), 
				         DbPort, 0) != LWDP_OK)
		{
			LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::ERR, 
						   "Open Db Error", DbPort);
			return 0;
		}

		iDbMgr->ExecSQL("set interactive_timeout=31536000");
		iDbMgr->ExecSQL("set wait_timeout=31536000");
		iDbMgr->ExecSQL("use `scenic`");

		LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::INFO, 
					   "Connect Db Ok!");

		time_t timep = time(NULL);
		struct tm checkTime = {0};
		localtime_r(&timep, &checkTime); 

		char_ endTime[1024] = {0};
		strftime(endTime, 1024, "%Y-%m-%d", &checkTime);
		char_ startTime[1024] = {0};
		checkTime.tm_mday -= 1;
		strftime(startTime, 1024, "%Y-%m-%d", &checkTime);

		char tmpStr[2048] = {0};
		int32_ inum = 0;
		{
			GET_OBJECT_RET(DbQuery, iCountDbQuery, 0);
			Api_snprintf(tmpStr, 2048, cardSetCount.c_str(), startTime, "<", endTime);

			iDbMgr->Ping();
			iDbMgr->QuerySQL(tmpStr, iCountDbQuery);

			inum = iCountDbQuery->GetIntField("setcount", -1);
			if(inum <= 0)
			{
				LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::NOTICE, 
				           		"%s Card Set is Null All Process Have Finished", 
				           		startTime);
				iDbMgr->Close();
				return 0;
			}
		}

		uint32_ thread_num = 15;
		int32_ domainSize = inum / thread_num + (inum % thread_num);
		
		LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::NOTICE, 
		           		"TotleNum: %d thread_num: %d domainSize: %d", 
		           		inum, thread_num, domainSize);

		std::list<pthread_t> thrList;
		for(uint32_ num = 0; num < thread_num; ++num)
		{
			pthread_t t;
			int result = 0;

			ProcessDomain* domain = (ProcessDomain*)malloc(sizeof(ProcessDomain));
			domain->startPos = (num * domainSize);
			domain->length = domainSize;
			result = pthread_create(&t, NULL, work_thread, domain);
			if(result != 0){
				LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::ERR, 
							   "Can't Create Thread Ret: %d\n", result);
				iDbMgr->Close();
				return 0;
			}

			thrList.push_back(t);			
		}
		
		//GET_OBJECT_RET(ConsoleMgr, iConsoleMgr, 0);
		//iConsoleMgr->RunConsole();

		std::list<pthread_t>::iterator iter;
		FOREACH_STL(iter, thrList)
		{
			int rc = pthread_join(*iter, NULL);
			ASSERT_CHECK(rc == 0);
		}

		iDbMgr->Close();
	}
	
	system("pause");
	return 0;
}
