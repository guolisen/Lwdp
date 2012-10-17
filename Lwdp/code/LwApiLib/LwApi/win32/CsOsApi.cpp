/*==============================================================================

FILE:     CsOsApi.cpp

DESCRIPTION:

    This module implements vxWorks Os functions under windows enviroment.



    liulj Initial Version 03 08 05

MODIFICATION:

==============================================================================*/



//==============================================================================

//                    Includes and Variable Definitions

//==============================================================================
 


//==============================================================================

// Include Files 

//==============================================================================
#include <LwApiLib/LwApi/win32/CsOsApi.h>


  
#ifdef WIN32

//==============================================================================

// Type Declarations

//==============================================================================
#ifndef BASE_PORT
#define BASE_PORT 8800
#endif

class TimerService

{

public:

    TimerService();

    ~TimerService();

    static HANDLE TimerInst;

};



//==============================================================================

// global Variable Definitions  

//==============================================================================

//for create Timer thread

static TimerService gTimerService;



//initial message queue

static Q_INFO qpool[MAX_QUEUES] = {0};


static PipePool ppool[MAX_PIPES] =

{{0,(BASE_PORT+10),0},{0,(BASE_PORT+11),0},{0,(BASE_PORT+12),0},{0,(BASE_PORT+13),0},

 {0,(BASE_PORT+14),0},{0,(BASE_PORT+15),0},{0,(BASE_PORT+16),0},{0,(BASE_PORT+17),0},

 {0,(BASE_PORT+18),0},{0,(BASE_PORT+19),0},{0,(BASE_PORT+20),0},{0,(BASE_PORT+21),0},

 {0,(BASE_PORT+22),0},{0,(BASE_PORT+23),0},{0,(BASE_PORT+24),0},{0,(BASE_PORT+25),0},{0,(BASE_PORT+26),0},{0,(BASE_PORT+27),0},{0,(BASE_PORT+28),0},

{0,(BASE_PORT+29),0},{0,(BASE_PORT+30),0},{0,(BASE_PORT+31),0},{0,(BASE_PORT+32),0},{0,(BASE_PORT+33),0},{0,(BASE_PORT+34),0},{0,(BASE_PORT+35),0},{0,(BASE_PORT+36),0},{0,(BASE_PORT+37),0},

{0,(BASE_PORT+38),0},{0,(BASE_PORT+39),0}

};//port value may be need to change

//==============================================================================

// Static Variable Definitions  

//==============================================================================

HANDLE TimerService::TimerInst = NULL;



//==============================================================================

// WatchDog Timer Functions

//==============================================================================



//Timer thread function

void TimerTask()

{

    while(true)

    ::SleepEx(INFINITE, TRUE);

}

TimerService::TimerService()

{

    TimerInst = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)

    TimerTask, NULL, 0, NULL);

    SetThreadPriority(TimerInst, THREAD_PRIORITY_HIGHEST);

}



//terminate Timer thread CallBack function

void  CALLBACK TimerThreadExit(DWORD dwExitCode) 

{ 

    printf("close timer thread\n");

    ExitThread(dwExitCode); 

} 

TimerService::~TimerService()

{

    QueueUserAPC((PAPCFUNC)TimerThreadExit, TimerInst, 0); 

    WaitForSingleObject(TimerInst, INFINITE);

    CloseHandle(TimerInst); 

}



WDOG_ID wdCreate()

{

    WDOG_ID tmp_ptr = new _WdTimer;



    if(tmp_ptr != NULL)

    {

    tmp_ptr->TimerId = CreateWaitableTimer(NULL, FALSE, NULL);

    }

    return tmp_ptr;

}



//wdStart CallBack function

void CALLBACK StartTimer(int para)

{

    WDOG_ID wd_tmp = (WDOG_ID)para;

    LARGE_INTEGER   timeout;

    

    CancelWaitableTimer(wd_tmp->TimerId); 

    timeout.QuadPart = -(wd_tmp->Interval);

    timeout.QuadPart *=10000; 

    SetWaitableTimer(wd_tmp->TimerId, &timeout, 0,

    (PTIMERAPCROUTINE)wd_tmp->FuncPtr, (int*)wd_tmp->Para, TRUE); 

}

STATUS wdStart(WDOG_ID wdId, int delay, FUNCPTR pRoutine, int parameter)

{

    if(NULL == wdId)

    return ERROR;

    

    wdId->Interval = delay;

    wdId->FuncPtr = pRoutine;

    wdId->Para = parameter;

    if(QueueUserAPC((PAPCFUNC)StartTimer, TimerService::TimerInst, (int)wdId))

    return OK;

    else return ERROR;

}



//wdCancel CallBack function

void CALLBACK StopTimer(int para)

{

    WDOG_ID wd_tmp = (WDOG_ID)para;

    CancelWaitableTimer(wd_tmp->TimerId); 

}

STATUS wdCancel(WDOG_ID wdId)

{

    if(QueueUserAPC((PAPCFUNC)StopTimer, TimerService::TimerInst, (int)wdId))

    return OK;

    else return ERROR;

}



STATUS wdDelete(WDOG_ID wdId)

{

    if(wdId != NULL)

    {

    CloseHandle(wdId->TimerId);

    delete wdId;

    wdId = NULL;

    }

    return OK;

}


int sysClkRateGet(void)
{
    return 1000;
}

STATUS taskPriorityGet(int tid, int* task_priority)
{
    return OK;        
}

STATUS taskPrioritySet(int tid, int task_priority)
{
    return OK;
}

//==============================================================================

// Semphore Functions

//==============================================================================



SEM_ID semBCreate(int options, SEM_B_STATE initialState)

{

    BOOL flag;

    

    if(SEM_FULL == initialState)

    flag = TRUE;

    else

    flag = FALSE;

    return ::CreateEvent(NULL, FALSE, flag, NULL);

}



struct SEM_C_

{

    SEM_ID semID;

    int count;

};

struct SEM_C_ semCArray[16];



SEM_ID semCCreate(int options, int initialCount)

{

    int i;



    for (i=0;i<16;i++)

    {

        if ( semCArray[i].semID ==0 )

            break;

    }

    if (i==16)

        return NULL;



    semCArray[i].semID =::CreateEvent(NULL, FALSE, FALSE, NULL);



    if (NULL !=semCArray[i].semID)

    {

        if (initialCount >0)

        {

            semCArray[i].count =initialCount;

            ::SetEvent(semCArray[i].semID);

        }

    }



    return semCArray[i].semID;

}



SEM_ID semMCreate(int options)

{

    return ::CreateEvent(NULL, FALSE, TRUE, NULL);

}



STATUS semTake(SEM_ID semId, int timeout)

{

    int i;
    int result;



    if(NULL == semId)

        return ERROR;

    

    if(WAIT_FOREVER == timeout)

        timeout = INFINITE;

    result = ::WaitForSingleObject(semId, (DWORD)timeout);

    if((WAIT_TIMEOUT != result) && (WAIT_FAILED != result))

    {

        for (i=0;i<16;i++)

        {

            if (semCArray[i].semID ==semId )

                break;

        }

        if (i==16 )

            return OK;



        semCArray[i].count--;

        if (semCArray[i].count >0 )

            ::SetEvent(semCArray[i].semID);

        return OK;

    }

    else 

        return ERROR;

}



STATUS semGive(SEM_ID semId)

{

    int i;



    if(NULL == semId)

        return ERROR;

    

    for (i=0;i<16;i++)

    {

        if (semCArray[i].semID ==semId )

            break;

    }

    if (i==16 )

    {

        if(::SetEvent(semId))

            return OK;

        else 

            return ERROR;

    }

    else

    {

        if (semCArray[i].count ==0 )

        {

            if(::SetEvent(semId))

            {

                semCArray[i].count++;

                return OK;

            }

            else 

                return ERROR;

        }

        else

        {

            semCArray[i].count++;

            return OK;

        }

    }

}



STATUS semDelete(SEM_ID semId)

{

    int i;



    if(NULL == semId)

        return ERROR;



    for (i=0;i<16;i++)

    {

        if (semCArray[i].semID ==semId )

            break;

    }

    if (i==16 )

    {

        ::CloseHandle(semId);

        return OK;

    }

    else

    {

        ::CloseHandle(semId);

        semCArray[i].semID =0;

        semCArray[i].count =0;

        return OK;

    }

}



//==============================================================================

// Task Contrl Functions

//==============================================================================

STATUS taskDelay(int ticks)

{

    ::Sleep(ticks);

    return OK;

}



STATUS taskDelete(int tid)

{

    if(TerminateThread((HANDLE)tid, NULL))

    {

    CloseHandle((HANDLE)tid);

    return OK;

    }

    else return ERROR;

}



STATUS taskSuspend(int tid)

{

    if(NULL == tid) tid = (int)GetCurrentThread();

    if(0xFFFFFFFF != SuspendThread((HANDLE)tid))

    return OK;

    else return ERROR;

}



STATUS taskResume(int tid)

{

    if(0xFFFFFFFF != ResumeThread((HANDLE)tid))

    return OK;

    else return ERROR;

}



static void taskSpawnA(void* parameters)

{
    int taskPara[10];

    FUNCPTR fun_ptr = (FUNCPTR)((int*)parameters)[10];

    memcpy(taskPara,(int*)parameters,10*sizeof(int));    
    free(parameters);
    

    fun_ptr(taskPara[0],taskPara[1],taskPara[2],

    taskPara[3],taskPara[4],taskPara[5],

    taskPara[6],taskPara[7],taskPara[8],

    taskPara[9]);   

}



int taskIdSelf()

{

    return (int)GetCurrentThread();

}



int taskSpawn

(

    char *  name,      /* name of new task (stored at pStackBase) */

    int     priority,  /* priority of new task */

    int     options,   /* task option word */

    int     stackSize, /* size (bytes) of stack needed plus name */

    FUNCPTR entryPt,   /* entry point of new task */

    int     arg1,      /* 1st of 10 req'd task args to pass to func */

    int     arg2,

    int     arg3,

    int     arg4,

    int     arg5,

    int     arg6,

    int     arg7,

    int     arg8,

    int     arg9,

    int     arg10

)

{

    HANDLE hTask = NULL;

    int * iArr = (int*)malloc(11*sizeof(int));

    

    iArr[0] = arg1;

    iArr[1] = arg2;

    iArr[2] = arg3;

    iArr[3] = arg4;

    iArr[4] = arg5;

    iArr[5] = arg6;

    iArr[6] = arg7;

    iArr[7] = arg8;

    iArr[8] = arg9;

    iArr[9] = arg10;

    iArr[10] = (int)entryPt;



    //优先级细化

    hTask = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)

    taskSpawnA, iArr, 0, NULL);
    
    if      (priority <= 50)

    ::SetThreadPriority(hTask, THREAD_PRIORITY_HIGHEST);

    else if(priority <= 80)

    ::SetThreadPriority(hTask, THREAD_PRIORITY_ABOVE_NORMAL);

    else if(priority <= 100)

    ::SetThreadPriority(hTask, THREAD_PRIORITY_NORMAL);

    else if(priority <= 120)

    ::SetThreadPriority(hTask, THREAD_PRIORITY_BELOW_NORMAL);

    else if(priority <= 150)

    ::SetThreadPriority(hTask, THREAD_PRIORITY_LOWEST);

    else 

    ::SetThreadPriority(hTask, THREAD_PRIORITY_IDLE);

    return (int)hTask;

}

STATUS  taskLock()

{

    return OK;

}



STATUS taskUnlock()

{

    return OK;

}



//==============================================================================

// Message Queue Functions

//==============================================================================



MSG_Q_ID msgQCreate

(

    int maxMsgs,     

    int maxMsgLength, 

    int options     

)

{

    for(register int i = 0; i < MAX_QUEUES; i+=1)

    {

    if(qpool[i].MsgQPtr == NULL)

    {

        qpool[i].MaxMsgNum = maxMsgs + 1;

        qpool[i].MaxMsgLen = maxMsgLength;

        qpool[i].Options = 0; // not used

        qpool[i].ReadPos = 0;

        qpool[i].WritePos = 0;

        qpool[i].MsgQPtr = (char*)malloc((maxMsgLength+4)*(qpool[i].MaxMsgNum));

        if(qpool[i].MsgQPtr == NULL) return NULL;

        else return &(qpool[i]);

    }

    }

    return NULL;

}



static bool canSend(MSG_Q_ID msgQId)

{

    if((msgQId->WritePos+1 == msgQId->ReadPos)

    || ((msgQId->WritePos == msgQId->MaxMsgNum-1) && (msgQId->ReadPos == 0))) 

    return false;

    return true;

}



STATUS msgQSend(MSG_Q_ID msgQId, char* buffer, UINT nBytes, int timeout, int priority)

{

    if(NULL == msgQId->MsgQPtr) return ERROR;

    if(nBytes > msgQId->MaxMsgLen) return ERROR;

    if(timeout == WAIT_FOREVER)

    {

    while(!canSend(msgQId)) taskDelay(1);

    }

    else if(NO_WAIT == timeout)

    {

    if(!canSend(msgQId)) return ERROR;

    }

    else

    {
	register int i;
    for(i = 0; i < timeout; i++)

    {

        if(!canSend(msgQId)) taskDelay(1);

        else break;

    }

    if(i == timeout) return ERROR;

    }

    char* tmp_ptr = msgQId->MsgQPtr+msgQId->WritePos*(msgQId->MaxMsgLen+4);

    memcpy(tmp_ptr+4, buffer, nBytes);

    *(int*)(tmp_ptr) = nBytes;



    msgQId->WritePos = msgQId->WritePos+1;

    if(msgQId->WritePos == msgQId->MaxMsgNum)

    msgQId->WritePos = 0;

    return OK;

}



static bool canReceive(MSG_Q_ID msgQId)

{

    return !(msgQId->ReadPos == msgQId->WritePos);

}



int msgQReceive(MSG_Q_ID msgQId, char* buffer, UINT maxNBytes, int timeout)

{

    if(NULL == msgQId->MsgQPtr) return ERROR;

    if(WAIT_FOREVER == timeout)

    {

    while(!canReceive(msgQId)) taskDelay(1);

    }

    else if(NO_WAIT == timeout)

    {

    if(!canReceive(msgQId)) return ERROR;

    }

    else

    {

	register int i;		
    for(i = 0; i < timeout; i++)

    {

        if(!canReceive(msgQId)) taskDelay(1);

        else break;

    }

    if(i == timeout) return ERROR;

    }



    char* tmp_ptr = msgQId->MsgQPtr+msgQId->ReadPos*(msgQId->MaxMsgLen+4);

    UINT len = *(int *)(tmp_ptr);

    if(len > maxNBytes) 
    {
        msgQId->ReadPos = msgQId->ReadPos+1;
    
        if(msgQId->ReadPos == msgQId->MaxMsgNum)
    
        msgQId->ReadPos = 0;
        
        return ERROR;
    }

    memcpy(buffer, tmp_ptr+4, len);



    msgQId->ReadPos = msgQId->ReadPos+1;

    if(msgQId->ReadPos == msgQId->MaxMsgNum)

    msgQId->ReadPos = 0;

    return len;

}



STATUS msgQDelete(MSG_Q_ID msgQId)

{

    char* pbak = msgQId->MsgQPtr;

    if(NULL == pbak) return ERROR;

    msgQId->MsgQPtr = NULL;

    free(pbak);

    return OK;

}



int msgQNumMsgs(MSG_Q_ID msgQId)

{

    if(NULL == msgQId->MsgQPtr) return ERROR;

    return ((msgQId->WritePos-msgQId->ReadPos+msgQId->MaxMsgNum)%msgQId->MaxMsgNum);

}



//==============================================================================

// Pipe Functions

//==============================================================================

STATUS pipeDevCreate(char* name, int nMessages, int nBytes)

{

    return OK;

}
STATUS pipeDelete(char* pipeName)
{
#ifdef WIN32
    return OK;
#else
    DEV_HDR* pipe_hdr;
    const char* name_ptr;
    
    if (NULL ==pipeName)
        return ERROR;
        
    if (NULL !=(pipe_hdr =iosDevFind(pipeName, &name_ptr)))
    {
        iosDevDelete(pipe_hdr);
        free((void*)pipe_hdr);
        
        return OK;
    }
    else
        return ERROR;
#endif    
}

long long getTimeBase()
{
    return (long long)1;
}

int opens(const char* name,int flags, int mode)

{

    struct sockaddr_in pipe_tmp;

    

    memset(&pipe_tmp, 0x00, sizeof(pipe_tmp));

    int sock_pipe = socket( AF_INET, SOCK_DGRAM, 0 );

    pipe_tmp.sin_family = AF_INET;

	register int i= 0;	
    for(i = 0; i < MAX_PIPES; i++)

    {

    if(ppool[i].SockId == 0)

        break;

    }

    if(i >= MAX_PIPES)

    return ERROR;

    pipe_tmp.sin_port = htons(ppool[i].PortId);

    ppool[i].SockId = sock_pipe;

    pipe_tmp.sin_addr.s_addr = INADDR_ANY;

    if(bind(sock_pipe, (struct sockaddr*)&pipe_tmp, sizeof(pipe_tmp))<0)

    {

    return ERROR;

    }

    return sock_pipe;

}



int reads(int fd, char* buffer, size_t maxbytes)

{

    struct sockaddr_in s;

    int j = sizeof(s);


	register int i= 0;	
    for(i = 0; i < MAX_PIPES; i++)

    {

    if(ppool[i].SockId == fd)

        break;

    }



    if(i >= MAX_PIPES)

    return ERROR;

    ppool[i].MsgNum--;

    return recvfrom(fd, buffer, maxbytes, 0, (struct sockaddr*)&s, &j);

}



int writes (int fd, char* buffer, size_t nbytes)

{

    struct sockaddr_in s;

    

    memset(&s, 0x00, sizeof(s));

    s.sin_family = AF_INET;
	register int i= 0;	
    for(i = 0; i < MAX_PIPES; i++)

    {

    if(ppool[i].SockId == fd)

        break;

    }

    if(i >= MAX_PIPES)

    return ERROR;

    s.sin_port = htons(ppool[i].PortId);

    s.sin_addr.s_addr = inet_addr("127.0.0.1");

    ppool[i].MsgNum++;

    return sendto(fd, buffer, nbytes, 0, (struct sockaddr*)&s, sizeof(s));

}

int ioctl(int fd, int function, int arg)

{

    int* ptr = (int*)arg;

    

    if(FIONMSGS != function)

    return ERROR;
	register int i= 0;	
    for(i = 0; i < MAX_PIPES; i++)

    {

    if(ppool[i].SockId == fd)

        break;

    }

    if(i >= MAX_PIPES)

    return ERROR;

    *ptr = ppool[i].MsgNum;

    return OK;

}

STATUS closes(int fd)

{
	register int i= 0;	
    for(i = 0; i < MAX_PIPES; i++)

    {

    if(ppool[i].SockId == fd)

        break;

    }

    if(i >= MAX_PIPES)

    return ERROR;

    ppool[i].SockId = 0;

    ppool[i].MsgNum = 0;

    return closesocket(fd);

}



INT64 htonll(INT64 para)

{

    char buf1[8];

    char buf2[8];

    int i;

    INT64  revalue=0;



    *(INT64*)(&buf1) = para;



    for(i=1; i<9; i++)

        buf2[8-i] = buf1[i-1];



    revalue = *(INT64*)(&buf2);



/*  memcpy((char*)(&buf1), (char*)(&para), 8);

    for(i=1; i<9; i++)

        buf2[8-i] = buf1[i-1];

    memcpy((char*)(&revalue), (char*)(&buf2), 8);*/



    return revalue;

}





INT64 ntohll(INT64 para)

{

    char buf1[8];

    char buf2[8];

    int i;

    INT64  revalue=0;



    *(INT64*)(&buf1) = para;



    for(i=1; i<9; i++)

        buf2[8-i] = buf1[i-1];



    revalue = *(INT64*)(&buf2);





    return revalue;

}



/*    memcpy((char*)(&buf1), (char*)(&para), 8);

    for(i=0; i<8; i++)

        buf2[8-i] = buf1[i-1];

    memcpy((char*)(&revalue), (char*)(&buf2), 8);



    return revalue;

}*/





#endif/*WIN32*/
