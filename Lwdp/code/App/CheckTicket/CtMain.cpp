
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

#include <../UModule/Interface/TSFrontend/Ix_TSFrontend.h>
#include <../UModule/Interface/TcpServer/Ix_TcpServer.h>


#include <vector>
#include <set>
#include <iostream>
#include <algorithm>
#include <boost/shared_ptr.hpp>

#include "CtDef.h"





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


LWRESULT addBlackList(const std::string& card_no)
{
	GET_OBJECT_RET(DbMgr, iDbMgr, 0);
	GET_OBJECT_RET(DbQuery, iDbQuery, 0);
	char tmpStr[2048] = {0};
	Api_snprintf(tmpStr, 2048, 
		         "UPDATE sc_card \
		          SET status = 5 \
		          WHERE card_no = '%s'", card_no.c_str());
	int32_ affLine = iDbMgr->ExecSQL(tmpStr);
	if(affLine <= 0)
	{
		LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::DEBUG, 
			           "Update sc_card (%s)", 
			           tmpStr);
		LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::WARNING, 
			           "Can't Find card_no(%s) From Table sc_card", 
			           card_no.c_str());
		Api_snprintf(tmpStr, 2048, 
			         "insert into sc_card  \
			          (card_no, card_status)\
			          values ('%s', 5)", card_no.c_str());
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

	GET_OBJECT_RET(ConfigMgr, iConfigMgr, 0);

	std::string strDbIp 		= std::string(LW_CT_DB_IP_DEFAULT);
	std::string strDbUserName 	= std::string(LW_CT_DB_USER_DEFAULT);
	std::string strDbPassword 	= std::string(LW_CT_DB_PASSWORD);
	std::string strDbName 		= std::string(LW_CT_DB_SELECT_DBNAME);
	uint32_     DbPort 		    = LW_CT_DB_PORT_DEFAULT;
	
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


	std::string selectFromCards;
	XPropertys propSelectFromCards;
	iConfigMgr->GetModulePropEntry(LW_CT_MODULE_NAME, LW_CT_DB_PORT_NAME, propSelectFromCards);
	if(!propDbPort[0].propertyText.empty())
	{
		//selectFromCards = ;
	}
	else
	{
		LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <DbPort> In Config File, Default(%d)", DbPort);
	}	
	

	time_t timep = time(NULL);
	struct tm checkTime = {0};
	localtime_r(&timep, &checkTime); 

	//char_ endTime[1024] = {0};
	//strftime(endTime, 1024, "%Y-%m-%d", &checkTime);
	//char_ startTime[1024] = {0};
	//checkTime.tm_mday -= 1;
	//strftime(startTime, 1024, "%Y-%m-%d", &checkTime);

	checkTime.tm_mday += 1;
	char_ endTime[1024] = {0};
	strftime(endTime, 1024, "%Y-%m-%d", &checkTime);
	char_ startTime[1024] = {0};
	checkTime.tm_mday -= 1;
	strftime(startTime, 1024, "%Y-%m-%d", &checkTime);

	char tmpStr[2048] = {0};
	int pageSize = 1000;
	for(int i = 0; i<10; i++)
	{
		GET_OBJECT_RET(DbQuery, iDbQuery, 0);
		int start = i * pageSize;
		Api_snprintf(tmpStr, 2048, 
			         "SELECT card_no,scenic_id \
			         FROM sc_swiping \
			         WHERE create_time >= DATE_FORMAT('2013-01-13',' %%Y-%%m-%%d ') AND \
			         	   create_time < DATE_FORMAT('2013-01-14',' %%Y-%%m-%%d ') \
			         LIMIT %d,%d", start, pageSize);
		LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::DEBUG, 
					   tmpStr);
		iDbMgr->QuerySQL(tmpStr, iDbQuery);

		uint32_ inum = iDbQuery->NumRow();
		LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::DEBUG, 
			           "Num: %d", inum);
		while(!iDbQuery->Eof())
		{
			std::string dev_card_no = iDbQuery->GetStringField("card_no", "");
			std::string dev_scenic_id = iDbQuery->GetStringField("scenic_id", "");

			Api_snprintf(tmpStr, 2048, 
				         "SELECT scenic_id \
				         FROM sc_card_scenic \
				         WHERE card_no = '%s'", dev_card_no.c_str());
			GET_OBJECT_RET(DbQuery, iScDbQuery, 0);
			iDbMgr->QuerySQL(tmpStr, iScDbQuery);
			uint32_ iscnum = iScDbQuery->NumRow();
			if(iscnum >= 1)
			{
				while(!iScDbQuery->Eof())
				{
					std::string true_scenic_id = iScDbQuery->GetStringField("scenic_id", "");
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
		}
	}


	LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::NOTICE, 
		           "Check OK!!");

	//GET_OBJECT_RET(ConsoleMgr, iConsoleMgr, 0);
	//iConsoleMgr->RunConsole();

	system("pause");
	return 0;
}
