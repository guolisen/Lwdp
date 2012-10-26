#ifndef _DATACOLLECTTOOL_H
#define _DATACOLLECTTOOL_H
/*==============================================================================
Copyright (c) 2002, CPIT. All Rights Reserved.

FILENAME: DataCollectTool.h

DESCRIPTION:
    This module contains data structure definitions, and data collect functions 
    interfaces 

HISTORY:

Date           CR No      Person        Description
----------  ------------  ------       -------------
03 10 23         1        liulj        Initial Version 
04-7-8           2        WangLi       Verify Version
==============================================================================*/

//==============================================================================
//                Includes and Public Data Declarations
//==============================================================================

//==============================================================================
// Include Files
//==============================================================================

//==============================================================================
// Constant / Define Declarations
//==============================================================================
#include <LwDp.h>

using namespace NLwdp;

const int MAX_MML_USER = 15;    /*֧�ֵ����ע���û���*/
const int MAX_MML_WNDNUM = 16;  /*ͬʱ֧�ֵ�������ڵ�����*/
const int MAX_MML_MMLNUM = 6; /*ͬʱ֧�ֵ�MML���ӵ�����*/
const int MAX_MML_STATNUM = 8; /*ͬʱ֧�ֵĲ���ͳ�����ӵ�����*/
const int MML_NAME_LEN = 16;    /*MML������,������,������ʾ���*/
const int MML_TOTAL_CMD_NUM = 512;/*���֧�ֵ�MML�������*/
const int MML_VALUE_LEN = 64;   /*MML�������ֵ����*/
const int MML_PARA_NUM = 16; /*MML����Dos����������*/
const int MML_POINTER_SIZE = 4;
const int STAT_NAME_NUM = 4; 
const int MAX_WINPTR_NUM = 128;
const int MAX_STATPTR_NUM = 128;

//#define LOCALDEBUG            //����MmlPrintf()ʱ��ʵ�ֵ���printf����
//#define DATACOLLECT_DEBUG     //��ӡ������Ϣ
//#define BASE_PORT             //���ӵĶ˿ڻ���Ĭ��ֵ��8800
//#define HW_TICK_1MS           //Ӳ����һ���ӵ�tick��Ĭ��ֵ��16384
//==============================================================================
// Type Declarations
//==============================================================================

/*�ڴ���?�������*/
struct MemoryBuf
{
    char*    PhyMemPtr;     /*�����������׵�ַ*/
    char*    HeadPtr;       /*���������ͷָ��*/
    char*    TailPtr;       /*���������βָ��*/
#ifdef DATACOLLECT_DEBUG
    int      BusyNum;
    int      FreeNum;
#endif
} ;

/*��Dos�����*/
struct MmlDosCmd
{
    char*    NamePtr;
    FUNCPTR  FnPtr;
};

/*MML����ص�����ע���ṹ*/
struct MmlCallBack
{
    FUNCPTR  FnPtr;     /*MML����ص�����ָ��*/
    void*    ObjPtr;        /*����ʵ��ָ��*/
    int      MsgBufNum;         /*��ʹ��MsgBuf����*/
};

/*MML��������ṹ*/
struct CmdStr
{
    char     Cmd[MML_NAME_LEN];
};
struct MmlPara 
{
    char     Para[MML_NAME_LEN];
    char     Value[MML_VALUE_LEN];
};
struct MmlCmdType
{
    CmdStr   CmdName[3];    /*[0]MML������,[1]��������1,[2]��������2*/
    int      ParaNum;       /*MML����������*/
    MmlPara  CmdPara[MML_PARA_NUM];/* MML������� */    
};

/*��Dos��������ṹ*/
struct  DosCmdType
{
    int      ParaNum;       /*(��Dos����+����)����*/
    CmdStr   Para[MML_PARA_NUM];/*[0]��Dos������,�����������*/
};

/*MML����ע���*/
struct MmlCmdItem 
{
    CmdStr   CmdName[3];    /*[0]MML������,[1]��������1,[2]��������2*/
    FUNCPTR  FnPtr;     /*MML����ܺ���API*/
    void*    ObjPtr;          /*����ָ��*/
    int      RegisterNum;   /*MMLע��� */
};
struct MmlCmdList 
{
    int      TotalNum;
    MmlCmdItem CmdItem[MML_TOTAL_CMD_NUM];
};

/*MML����ܿ��ƿ�*/
enum MmlStatus  
{
    IDLE, 
    CONNECT,
    ACTIVE, 
    PAUSE,
    RELEASE,
    ERR
};

struct MmlWinCfg
{
    int      WinId;     /*�û�����Ĵ��ں�*/
    int      PipeId;        /*�û��Ļ���pipe*/
    int      SockId;        /*�û������socket*/
    MmlStatus Status;       /*���ڿ��Ʋɼ���������ʽ*/
    int      Period;        /*���ڿ��ƶ�ʱ�������*/
    WDOG_ID  WdTimer;       /*�û�����Ķ�ʱ��*/
    char     Prompt[MML_NAME_LEN];/*�û��������ʾ��*/
    char     IpAddr[20];    /*�û���Ip��ַ*/
    char     kit_telnet_connect;       /*��������Kit���ӻ���telnet����*/
    bool    connect;           /*�����Ƿ���ڵı�־*/

};

/*MML����pipe��������ṹ*/
enum MmlCrlOrdType
{
    TOADDMML,           /*�����µ�MML����*/
    TOADDSTAT,          /*�����µ�ͳ������*/
    TOADDDBG,           /*�����µ�Debug����*/
    TOACTIVE,           /*����ĳ���û���ݲɼ����*/
    TOPAUSE,            /*��ͣĳ���û���ݲɼ����*/
    TOTIMER,            /*���ö�ʱ���*/
    TOTIMEOUT           /*��ʱָ��*/
};
struct MmlOrdMsg
{
    int      WinId;
    int      Peroid;
    int      ConSockId;
    char     IpAddr[20];
};
struct MmlCtrlOrd
{
    MmlCrlOrdType OrdCode;  /*������*/
    MmlOrdMsg     OrdMsg;       /*��������*/
};
/*���ݸ��û���MML��Ϣ*/
struct MmlMsg
{
    int      indexId;            /* MML���ӵ�socket��*/
    int      CmdIndex;          /*MML���������*/
    int      ParaNum;       /*MML����������*/
    MmlPara  CmdPara[MML_PARA_NUM];/* MML������� */    
};

/*  ���ں������ӿ��ƿ��Ӧ�ṹ*/

struct winptr
{
    int winNum;
    char  config;
};
struct StatName
{
    char flag;
    char buf[65];
};
struct statptr
{
    int winNum;
    StatName    mStatName[STAT_NAME_NUM];    /*����ͳ�����б�*/
};

//==============================================================================
// Global Data Declarations
//==============================================================================

//==============================================================================
//                            Macro Definitions
//==============================================================================

//==============================================================================
//                            Class Definitions
//==============================================================================
/*==============================================================================
Class:  DataCollectTool

Description:
 This class manages Data Collect task, analyses and executes MML command, prints
 data collect information.

How to use:
 To use this class instantiate a class object. Call MmlInit() before using 
 other functions.

Note��
==============================================================================*/
class DataCollectTool 
{
public:
    DataCollectTool();

    ~DataCollectTool();

    //--------------------------------------------------------------------------
    // Function interfaces for upper App
    //--------------------------------------------------------------------------
    STATUS        MmlInit();

    static STATUS MmlRegister(void* objPtr, FUNCPTR cbFunPtr);

    static STATUS MmlUnRegister(int num);

    static STATUS MmlInstallCmd(char* cmdName, char* fieldName, 
        char* subFieldName, FUNCPTR funPtr,void* ObjPtr,int registerNum);

    static STATUS ProcessMMLMsg(int msgAddr);

    static STATUS MmlGetCmdPara(int handle, int paraIndex, char* paraPtr[]);

    static STATUS MmlPrintf(const char* paraPtr, ...);

    static STATUS MmlPrintf(int winId, const char* paraPtr, ...);

    static STATUS MmlLog(int winId, const char* buf, int length);
    
    static STATUS MmlLogEx(int winId, const char* buf, int length);

    static STATUS MmlLogPrintf(int winId, const char* buf, int length);
#if defined(SW_CPB)||defined(MCU_SW)
    
    static STATUS MmlLogPrintfEx(int winId, const char* buf, int length);
#endif

    static char   StatRegister(int winId, char* ParaName);

    static STATUS StatUnRegister(int winId, signed char StatId);
    
    static STATUS MmlStatistic(int winId, signed char StatId, int paraSize);
    static STATUS MmlStatistic(int winId, signed char StatId, int paraSize, unsigned int coordinate);

    static STATUS MmlPause(int winId);
    
    static STATUS MmlResume(int winId);

    static STATUS MmlTimer(int winId, int period);
    
private:
    //--------------------------------------------------------------------------
    // Function to release resources
    //--------------------------------------------------------------------------
    STATUS        MmlExit();

    //--------------------------------------------------------------------------
    // Functions for data collect buffer
    //--------------------------------------------------------------------------
    int           InitDataBuf(int bufSize, int bufNum, MemoryBuf& memBuf);
    static char*  GetDataBuf(MemoryBuf& memBuf);
    static int    PutDataBuf(char* ptr, MemoryBuf& memBuf);
    void          ExitDataBuf();
    
    //--------------------------------------------------------------------------
    // Functions of dealing with socket and pipe 
    //--------------------------------------------------------------------------
    static STATUS MmlBind(int sockfd, int port);
    static STATUS MmlCloseSocket(int sockfd); 
    static STATUS SendToTelnet(int index, char* bufPtr);
    static STATUS SendToPipe(MmlCtrlOrd* ptr);
    static STATUS SuperMmlPrintf(int mode, const char* paraPtr, ...);

    //--------------------------------------------------------------------------
    // Functions for dealing with character
    //--------------------------------------------------------------------------
    static void   MyStrUpper(char* ptr);
    int           IsNormalChar(char c);
    int           DelBlank(char* bufPtr);
    int           HasSemicolon(char* ptr, int len);
    char          GetOneChar(int i);
    int           ProcessChar(char ch);
    int           DeleteChar(int i);
    int           GetOneLine(int i);

    //--------------------------------------------------------------------------
    // Functions for dealing with dos and mml command
    //--------------------------------------------------------------------------
    int           AnalyseDosCmd(char* strPtr, DosCmdType& dosCmdPtr);
    int           AnalyseMmlCmd(char* strPtr, MmlCmdType& mmlCmdPtr);
    int           ExecuteDosCmd(const DosCmdType& dosCmd, int indexId);
    int           ExecuteMmlCmd(const MmlCmdType& mmlCmd, int indexId);

    //--------------------------------------------------------------------------
    // Functions of executing dos command 
    //--------------------------------------------------------------------------
    static int    CmdCfgWin(DosCmdType* dosCmdPtr);
    static void   CmdShowDebugWin();
    static void   CmdPauseWin(DosCmdType* dosCmdPtr);
    static void   CmdResumeWin(DosCmdType* dosCmdPtr);
    static void   CmdTimerWin(DosCmdType* dosCmdPtr);
    static void   CmdReboot();
    static void   CmdHelp();
    static void   CmdQuit();
    static void   PrintDataBuf();
    static int    deleteconnection(DosCmdType* dosCmdPtr);
    static int    CaptureData();
    
    //--------------------------------------------------------------------------
    // Functions for data collect task to use
    //--------------------------------------------------------------------------
    static STATUS MmlTimeout(int winId);
    static void   UserSockFail(int i);
    static void   StatSockFail(int i);
    void          DealMmlCtrlOrd(MmlCtrlOrd* ordPtr);
    void          DealMmlCmd(int i);
    void          DealWatchSock(int i);
    void          DealWatchPipe(int i);
    void          DealStatSock(int i);
    void          DealStatPipe(int i);
    

    //--------------------------------------------------------------------------
    // Functions of TCP listen task and data collect task
    //--------------------------------------------------------------------------
    static STATUS ListenTask();
    static STATUS DataCollectTask(DataCollectTool* appObjPtr);

    /*��ݲɼ�������*/
    static MemoryBuf    mDataBuf;
#if defined(SW_CPB)||defined(MCU_SW)
    /*Ϊ���ר�����õ���ݲɼ�������:4K*100*/  
    static MemoryBuf    mBBDDataBuf;
#endif
    /*MML��Ϣ������*/
    static MemoryBuf    mMsgBuf;
    /*ͳ����ݲɼ�������*/
    static MemoryBuf    mStatBuf;

    /*MML����ص������*/                       
    static MmlCallBack  mMmlCallBack[MAX_MML_USER];
    /*MML����ע���*/                       
    static MmlCmdList*  mMmlCmdPtr;
    /*��Dos�����*/                     
    static MmlDosCmd    mDosCmd[16];


    
    /*��ݲɼ����ƴ�����Ϣ*/                        
    static MmlWinCfg    mMmlCtrl[MAX_MML_MMLNUM];
    /*ͳ����ݿ��ƴ�����Ϣ*/
    static MmlWinCfg    mMmlStat[MAX_MML_STATNUM];
    /*�û���ݲɼ����ƿ�*/                      
    static MmlWinCfg    mMmlCnfg[MAX_MML_WNDNUM];

    /*�û���ݲɼ��û����Ӹ���*/                        
    static int          mWndConnNum;
    /*MML���Ӹ���*/
    static int          mMmlConnNum;
    /*����ͳ�����Ӹ���*/
    static int          mStatConnNum;
    /*��ʼ����־*/
    static int          mInitialized;

    static winptr       WinPtr[MAX_WINPTR_NUM];
    static statptr      StatPtr[MAX_STATPTR_NUM];
    static int          activeId;
    static int          ctrlPipeId;
    static int          mDataCollectTaskId;
    static int          mListenTaskId;
    static int          tty_sockid;
    static int          watch_sockid;
    static int          stat_sockid;
    static SEM_ID   semIdForDataBuf;/*added by suyu for replacing taskLock 2006-12-6*/
};

#endif
