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

const int MAX_MML_USER = 15;    /*支持的最大注册用户数*/
const int MAX_MML_WNDNUM = 16;  /*同时支持的输出窗口的数量*/
const int MAX_MML_MMLNUM = 6; /*同时支持的MML连接的数量*/
const int MAX_MML_STATNUM = 8; /*同时支持的参数统计连接的数量*/
const int MML_NAME_LEN = 16;    /*MML命令名,参数名,窗口提示符长度*/
const int MML_TOTAL_CMD_NUM = 512;/*最多支持的MML命令个数*/
const int MML_VALUE_LEN = 64;   /*MML命令参数值长度*/
const int MML_PARA_NUM = 16; /*MML及类Dos命令参数个数*/
const int MML_POINTER_SIZE = 4;
const int STAT_NAME_NUM = 4; 
const int MAX_WINPTR_NUM = 128;
const int MAX_STATPTR_NUM = 128;

//#define LOCALDEBUG            //调用MmlPrintf()时，实现的是printf功能
//#define DATACOLLECT_DEBUG     //打印调试信息
//#define BASE_PORT             //连接的端口基数，默认值是8800
//#define HW_TICK_1MS           //硬件的一秒钟的tick数，默认值是16384
//==============================================================================
// Type Declarations
//==============================================================================

/*内存管理缓冲区队列*/
struct MemoryBuf
{
    char*    PhyMemPtr;     /*缓冲区物理首地址*/
    char*    HeadPtr;       /*缓冲区队列头指针*/
    char*    TailPtr;       /*缓冲区队列尾指针*/
#ifdef DATACOLLECT_DEBUG
    int      BusyNum;
    int      FreeNum;
#endif
} ;

/*类Dos命令表*/
struct MmlDosCmd
{
    char*    NamePtr;
    FUNCPTR  FnPtr;
};

/*MML命令回调函数注册表结构*/
struct MmlCallBack
{
    FUNCPTR  FnPtr;     /*MML命令回调函数指针*/
    void*    ObjPtr;        /*对象实例指针*/
    int      MsgBufNum;         /*已使用MsgBuf个数*/
};

/*MML命令解析结构*/
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
    CmdStr   CmdName[3];    /*[0]MML命令名,[1]子命令名1,[2]子命令名2*/
    int      ParaNum;       /*MML命令参数个数*/
    MmlPara  CmdPara[MML_PARA_NUM];/* MML命令参数 */    
};

/*类Dos命令解析结构*/
struct  DosCmdType
{
    int      ParaNum;       /*(类Dos命令+参数)个数*/
    CmdStr   Para[MML_PARA_NUM];/*[0]类Dos命令名,其他命令参数*/
};

/*MML命令注册表*/
struct MmlCmdItem 
{
    CmdStr   CmdName[3];    /*[0]MML命令名,[1]子命令名1,[2]子命令名2*/
    FUNCPTR  FnPtr;     /*MML命令功能函数API*/
    void*    ObjPtr;          /*对象指针*/
    int      RegisterNum;   /*MML注册号 */
};
struct MmlCmdList 
{
    int      TotalNum;
    MmlCmdItem CmdItem[MML_TOTAL_CMD_NUM];
};

/*MML命令功能控制块*/
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
    int      WinId;     /*用户定义的窗口号*/
    int      PipeId;        /*用户的缓冲pipe*/
    int      SockId;        /*用户的输出socket*/
    MmlStatus Status;       /*用于控制采集数据输出方式*/
    int      Period;        /*用于控制定时输出周期*/
    WDOG_ID  WdTimer;       /*用户定义的定时器*/
    char     Prompt[MML_NAME_LEN];/*用户定义的提示符*/
    char     IpAddr[20];    /*用户的Ip地址*/
    char     kit_telnet_connect;       /*该连接是Kit连接还是telnet连接*/
    bool    connect;           /*连接是否存在的标志*/

};

/*MML控制pipe控制命令结构*/
enum MmlCrlOrdType
{
    TOADDMML,           /*加入新的MML连接*/
    TOADDSTAT,          /*加入新的统计连接*/
    TOADDDBG,           /*加入新的Debug连接*/
    TOACTIVE,           /*激活某个用户数据采集输出*/
    TOPAUSE,            /*暂停某个用户数据采集输出*/
    TOTIMER,            /*设置定时输出*/
    TOTIMEOUT           /*超时指令*/
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
    MmlCrlOrdType OrdCode;  /*命令码*/
    MmlOrdMsg     OrdMsg;       /*命令内容*/
};
/*传递给用户的MML消息*/
struct MmlMsg
{
    int      indexId;            /* MML连接的socket号*/
    int      CmdIndex;          /*MML命令索引号*/
    int      ParaNum;       /*MML命令参数个数*/
    MmlPara  CmdPara[MML_PARA_NUM];/* MML命令参数 */    
};

/*  窗口号与连接控制块对应结构*/

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
    StatName    mStatName[STAT_NAME_NUM];    /*参数统计名列表*/
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

Note：
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

    /*数据采集缓冲区*/
    static MemoryBuf    mDataBuf;
#if defined(SW_CPB)||defined(MCU_SW)
    /*为基带专门设置的数据采集缓冲区:4K*100*/  
    static MemoryBuf    mBBDDataBuf;
#endif
    /*MML消息缓冲区*/
    static MemoryBuf    mMsgBuf;
    /*统计数据采集缓冲区*/
    static MemoryBuf    mStatBuf;

    /*MML命令回调函数表*/                       
    static MmlCallBack  mMmlCallBack[MAX_MML_USER];
    /*MML命令注册表*/                       
    static MmlCmdList*  mMmlCmdPtr;
    /*类Dos命令表*/                     
    static MmlDosCmd    mDosCmd[16];


    
    /*数据采集控制窗口信息*/                        
    static MmlWinCfg    mMmlCtrl[MAX_MML_MMLNUM];
    /*统计数据控制窗口信息*/
    static MmlWinCfg    mMmlStat[MAX_MML_STATNUM];
    /*用户数据采集控制块*/                      
    static MmlWinCfg    mMmlCnfg[MAX_MML_WNDNUM];

    /*用户数据采集用户连接个数*/                        
    static int          mWndConnNum;
    /*MML连接个数*/
    static int          mMmlConnNum;
    /*参数统计连接个数*/
    static int          mStatConnNum;
    /*初始化标志*/
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
