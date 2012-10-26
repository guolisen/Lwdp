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

const int MAX_MML_USER = 15;    
const int MAX_MML_WNDNUM = 16; 
const int MAX_MML_MMLNUM = 6; 
const int MAX_MML_STATNUM = 8; 
const int MML_NAME_LEN = 16;   
const int MML_TOTAL_CMD_NUM = 512;
const int MML_VALUE_LEN = 64;   
const int MML_PARA_NUM = 16; 
const int MML_POINTER_SIZE = 4;
const int STAT_NAME_NUM = 4; 
const int MAX_WINPTR_NUM = 128;
const int MAX_STATPTR_NUM = 128;


struct MemoryBuf
{
    char*    PhyMemPtr;    
    char*    HeadPtr;      
    char*    TailPtr;      
#ifdef DATACOLLECT_DEBUG
    int      BusyNum;
    int      FreeNum;
#endif
} ;


struct MmlDosCmd
{
    char*    NamePtr;
    FUNCPTR  FnPtr;
};


struct MmlCallBack
{
    FUNCPTR  FnPtr;     
    void*    ObjPtr;       
    int      MsgBufNum;         
};


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
    CmdStr   CmdName[3];   
    int      ParaNum;       
    MmlPara  CmdPara[MML_PARA_NUM];    
};


struct  DosCmdType
{
    int      ParaNum;      
    CmdStr   Para[MML_PARA_NUM];
};


struct MmlCmdItem 
{
    CmdStr   CmdName[3];  
    FUNCPTR  FnPtr;     
    void*    ObjPtr;    
    int      RegisterNum; 
};
struct MmlCmdList 
{
    int      TotalNum;
    MmlCmdItem CmdItem[MML_TOTAL_CMD_NUM];
};


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
    int      WinId;     
    int      PipeId;        
    int      SockId;        
    MmlStatus Status;       
    int      Period;       
    WDOG_ID  WdTimer;       
    char     Prompt[MML_NAME_LEN];
    char     IpAddr[20];    
    char     kit_telnet_connect;       
    bool    connect;         

};


enum MmlCrlOrdType
{
    TOADDMML,           
    TOADDSTAT,          
    TOADDDBG,           
    TOACTIVE,           
    TOPAUSE,           
    TOTIMER,           
    TOTIMEOUT          
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
    MmlCrlOrdType OrdCode;  
    MmlOrdMsg     OrdMsg;       
};
 
struct MmlMsg
{
    int      indexId;             
    int      CmdIndex;          
    int      ParaNum;        
    MmlPara  CmdPara[MML_PARA_NUM];   
};

 

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
    StatName    mStatName[STAT_NAME_NUM];    
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

 
    static MemoryBuf    mDataBuf;
#if defined(SW_CPB)||defined(MCU_SW)
 
    static MemoryBuf    mBBDDataBuf;
#endif
 
    static MemoryBuf    mMsgBuf;
   
    static MemoryBuf    mStatBuf;

                    
    static MmlCallBack  mMmlCallBack[MAX_MML_USER];
                     
    static MmlCmdList*  mMmlCmdPtr;
                    
    static MmlDosCmd    mDosCmd[16];


    
                        
    static MmlWinCfg    mMmlCtrl[MAX_MML_MMLNUM];
 
    static MmlWinCfg    mMmlStat[MAX_MML_STATNUM];
                   
    static MmlWinCfg    mMmlCnfg[MAX_MML_WNDNUM];

                     
    static int          mWndConnNum;
 
    static int          mMmlConnNum;
 
    static int          mStatConnNum;
 
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
