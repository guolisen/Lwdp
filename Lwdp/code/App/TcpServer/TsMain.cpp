
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

#ifdef TS_DAEMON_TAG
#include <unistd.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>

int init_daemon(void)
{
	int pid;
	int i;

	if((pid=fork()) > 0)
		exit(0);//是父进程，结束父进程
	else if(pid < 0)
		return -1;//fork失败，退出
	//是第一子进程，后台继续执行
	int npid = setsid();
	if(npid < -1)
	{
		return -1;
	}
	//第一子进程成为新的会话组长和进程组长
	//并与控制终 端分离
	//if((pid=fork()) > 0)
	//	exit(0);//是第一子进程，结束第一子进程
	//else if(pid < 0)
	//	exit(1);//fork失败，退出

	int sfpid = open("/var/run/tsd.pid", O_WRONLY|O_CREAT|O_TRUNC, 0777);

	char str[100] = {0};
	memset(str, 0, 100);
	pid = getpid();
	sprintf(str, "%d", pid);
	write(sfpid, str, strlen(str));
	close(sfpid);

	//是第二子进程，继续
	//第二子进程不再是会话组长
	for(i=0;i< NOFILE;++i)
	//关闭打开的文件描述符
		close(i);

	chdir("/usr/local/TcpServer"); //改变工作目录到/tmp
	umask(0);//重设 文件创建掩模

	return 0;
}
#endif

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


int gLog_handle = 0;
int32_ main(int argc, char* argv[])
{
#ifdef TS_DAEMON_TAG
	gLog_handle = open("/usr/local/TcpServer/log/platform.log", O_RDWR|O_CREAT|O_APPEND, 0777);
	if(gLog_handle <= 0)
	{
		printf("open log file err!\n");
		return 123;
	}

	dup2(gLog_handle, STDIN_FILENO);
	dup2(gLog_handle, STDOUT_FILENO);
	dup2(gLog_handle, STDERR_FILENO);

	int dret = init_daemon();
	if(dret)
	{
		return -1;
	}
	//while(1)
	//{
	//	sleep(1000);

	//}
#endif
	LWRESULT stat = LWDP_ERROR;
#ifndef WIN32
	char szWorkDir[260] = {0};
	getcwd(szWorkDir, 260);
	printf("%s\n", szWorkDir);
#endif

#if defined(LWDP_PLATFORM_DEF_WIN32)
	char* configDir = "../../../../bin/xml/ConfigExternal.xml";
#elif defined(LWDP_PLATFORM_DEF_LINUX)
	char* configDir = "xml/LinuxConfigExternal.xml";
#elif defined(TS_DAEMON_TAG)
	char* configDir = "/usr/local/TcpServer/xml/LinuxConfigExternal.xml";
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
		//system("pause");
		return -1;
		
	}
  
	GET_OBJECT_RET(TcpServer, iTcpServer, 0);
	LWRESULT ret  = iTcpServer->Init();
	if(ret != LWDP_OK)
	{
		lw_log_err(LWDP_MODULE_LOG, "TcpServer Init Error(0x%x)!", ret);
		//system("pause");
		return -1;
	} 
	LWRESULT ret2 = iTcpServer->RunServer();
	if(ret2 != LWDP_OK)
	{
		lw_log_err(LWDP_MODULE_LOG, "TcpServer RunServer Error(0x%x)!", ret2);
		//system("pause");
		return -1;
	} 

#ifdef TS_DAEMON_TAG
	while(1)
	{
		sleep(1000);
	}
#else
	GET_OBJECT_RET(ConsoleMgr, iConsoleMgr, 0);
	iConsoleMgr->RunConsole();
#endif
	//system("pause");
	return 0;}
