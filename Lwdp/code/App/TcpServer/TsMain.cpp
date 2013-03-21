
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
#include <../UModule/Interface/TSFrontend/Ix_TSFrontend.h>
#include <../UModule/Interface/TcpServer/Ix_TcpServer.h>

#include <vector>
#include <set>
#include <iostream>
#include <algorithm>
#include <boost/shared_ptr.hpp>


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

const char* HelpStr = "Format:\nTcpMain [config file directory]";

int32_ main(int argc, char* argv[])
{
	LWRESULT stat = LWDP_ERROR;
	char szWorkDir[260] = {0};
	getcwd(szWorkDir, 260);

	printf("%s\n", szWorkDir);
#if defined(LWDP_PLATFORM_DEF_WIN32)
		char* configDir = "../../../../bin/xml/ConfigExternal.xml";
#elif defined(LWDP_PLATFORM_DEF_LINUX)
	char* configDir = "xml/LinuxConfigExternal.xml";
#endif

	if(argc > 1)
	{
		configDir = argv[1];
	}

	ConfigSrcImp csrc(configDir);

	stat = Fw_Init(&csrc, 1);
	if(stat != LWDP_OK)
	{
		lw_log_err(LWDP_MODULE_LOG, "Fw_Init Error(0x%x)!", stat);
		system("pause");
		return -1;
		
	}
  
	GET_OBJECT_RET(TcpServer, iTcpServer, 0);
	LWRESULT ret  = iTcpServer->Init();
	if(ret != LWDP_OK)
	{
		lw_log_err(LWDP_MODULE_LOG, "TcpServer Init Error(0x%x)!", ret);
		system("pause");
		return -1;
	} 
	LWRESULT ret2 = iTcpServer->RunServer();
	if(ret2 != LWDP_OK)
	{
		lw_log_err(LWDP_MODULE_LOG, "TcpServer RunServer Error(0x%x)!", ret2);
		system("pause");
		return -1;
	} 

	GET_OBJECT_RET(ConsoleMgr, iConsoleMgr, 0);
	iConsoleMgr->RunConsole();

	//system("pause");
	return 0;
}
