#ifndef _CSOSAPI_H
#define _CSOSAPI_H
/*==============================================================================

FILE:     CsOsApi.h

DESCRIPTION:

    This module contains provides vxWorks functions interfaces under windows

    enviroment



MODIFICATION:

    liulj Initial Version 03 08 05

==============================================================================*/



#ifdef _WIN32_WINNT

 #undef _WIN32_WINNT

 #define _WIN32_WINNT 0x0600

#else

 #define _WIN32_WINNT 0x0600

#endif



/*============================================================================*/

/*================Includes and Public Data Declarations=======================*/

/*============================================================================*/



/*============================================================================*/

/*=Include Files==============================================================*/

/*============================================================================*/

#include <windows.h>

#include <stdio.h>

#include <stdlib.h>

#include <time.h>

#include <conio.h>

#include <winsock.h>

#include <string.h>

#include <memory.h>



/*============================================================================*/

/*=Constant / Define Declarations=============================================*/

/*============================================================================*/

#define OK 0

#ifdef ERROR

    #undef ERROR

#endif

#define ERROR -1



#define NO_WAIT (0)

#define WAIT_FOREVER (-1)

#define MSG_PRI_NORMAL (0) 

#define MSG_PRI_URGENT (1) 

#define MSG_Q_FIFO (0x00)

#define MSG_Q_PRIORITY (0x01) 

#define MAX_QUEUES (1024)



#define MAX_PIPES (30)

#ifndef O_RDWR
	#define O_RDWR (2)
#endif

#define FIONMSGS (17)



#define MAX_DOGS (512)

#define SEM_Q_PRIORITY (0x1)

#define SEM_Q_FIFO (0x0)

/*added by suyu begin 2007-5-23*/
#define SEM_DELETE_SAFE         0x04    /* owner delete safe */

#define SEM_INVERSION_SAFE     0x08    /* no priority inversion */
/*added by suyu end 2007-5-23*/
#define MAX_SEMS (1024)





/*============================================================================*/

/*=Type Declarations==========================================================*/

/*============================================================================*/

#ifdef __cplusplus

typedef int         (*FUNCPTR) (...);  /* ptr to function returning int */

#else

typedef int         (*FUNCPTR) ();       /* ptr to function returning int */

#endif/* _cplusplus */



typedef int STATUS ;

typedef unsigned int UINT ;



typedef struct _Q_INFO

{

    UINT MaxMsgNum;

    UINT MaxMsgLen;

    int  Options;

    UINT ReadPos;

    UINT WritePos;

    char* MsgQPtr;

} Q_INFO, * LPQ_INFO, * MSG_Q_ID;



typedef struct _WdTimer

{

    int Interval;/*µ¥λ1ms*/

    HANDLE TimerId;

    FUNCPTR FuncPtr;

    int Para;

} * WDOG_ID;



typedef HANDLE SEM_ID;

typedef enum 

{ 

    SEM_FULL=1,

    SEM_EMPTY=0 

} SEM_B_STATE;



typedef struct _PipePool

{

    int SockId;

    int PortId;

    int MsgNum;

} PipePool;



/*============================================================================*/

/*============================Macro Definitions===============================*/

/*============================================================================*/

#define FOREVER for(;;)

#define KEEP_PROCESS_ALIVE while ( getch()!=27 )



#ifdef  __cplusplus

extern "C" {

#endif



/*============================================================================*/

/*=  Network byte sequence  ==================================================*/

/*============================================================================*/



extern     INT64 htonll(INT64 para);

extern  INT64 ntohll(INT64 para);





/*============================================================================*/

/*=WatchDog Timer Functions===================================================*/

/*============================================================================*/

extern WDOG_ID wdCreate() ;



extern STATUS wdStart

(

    WDOG_ID wdId,     /* watchdog ID */

    int     delay,    /* delay count, in ticks */

    FUNCPTR pRoutine, /* routine to call on time-out */

    int     parameter /* parameter with which to call routine */

);



extern STATUS wdDelete

(

    WDOG_ID wdId /* ID of watchdog to delete */

);



extern STATUS wdCancel

(

    WDOG_ID wdId /* ID of watchdog to cancel */

);


extern int sysClkRateGet(void);
extern STATUS taskPriorityGet(int tid, int* task_priority);
extern STATUS taskPrioritySet(int tid, int task_priority);
/*============================================================================*/

/*=Semphore Functions=========================================================*/

/*============================================================================*/

extern SEM_ID semBCreate

(

    int         options,     /* semaphore options */

    SEM_B_STATE initialState /* initial semaphore state */

);



extern STATUS semTake

(

    SEM_ID semId,  /* semaphore ID to take */

    int    timeout /* timeout in ticks */

);



extern STATUS semGive

(

    SEM_ID semId /* semaphore ID to give */

);



extern STATUS semDelete

(

    SEM_ID semId /* semaphore ID to delete */

);



extern SEM_ID semMCreate

(

    int options

);



extern SEM_ID semCCreate

(

    int         options,     /* semaphore options */

    int            initialCount /* initial semaphore count */

);



/*============================================================================*/

/*=Task Contrl Functions======================================================*/

/*============================================================================*/

extern STATUS taskDelay

(

    int ticks /* number of ticks to delay task */

);



extern STATUS taskDelete

(

    int tid /* task ID of task to delete */

);



extern STATUS taskSuspend

(

    int tid /* task ID of task to suspend */

);



extern STATUS taskResume

(

    int tid /* task ID of task to resume */

);



extern int taskIdSelf();



extern int taskSpawn

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

);

extern STATUS taskLock();



extern STATUS taskUnlock();



/*============================================================================*/

/*=Message Queue Functions====================================================*/

/*============================================================================*/

extern MSG_Q_ID msgQCreate

(

    int maxMsgs,      /* max messages that can be queued */

    int maxMsgLength, /* max bytes in a message */

    int options       /* message queue options */

);



extern STATUS msgQDelete

(

    MSG_Q_ID msgQId /* message queue to delete */

);

    

extern STATUS msgQSend

(

    MSG_Q_ID msgQId,  /* message queue on which to send */

    char *   buffer,  /* message to send */

    UINT     nBytes,  /* length of message */

    int      timeout, /* ticks to wait */

    int      priority /* MSG_PRI_NORMAL or MSG_PRI_URGENT */

);



extern int msgQReceive

(

    MSG_Q_ID msgQId,    /* message queue from which to receive */

    char *   buffer,    /* buffer to receive message */

    UINT     maxNBytes, /* length of buffer */

    int      timeout    /* ticks to wait */

);



extern int msgQNumMsgs

(

    MSG_Q_ID msgQId /* message queue to examine */

);



/*============================================================================*/

/*=Pipe Functions=============================================================*/

/*============================================================================*/

extern STATUS pipeDevCreate

(

    char * name,      /* name of pipe to be created */

    int    nMessages, /* max. number of messages in pipe */

    int    nBytes     /* size of each message */

);

extern int opens

(

    const char * name,  /* name of the file to open */

    int          flags, /* O_RDONLY, O_WRONLY, O_RDWR, or O_CREAT */

    int          mode   /* mode of file to create (UNIX chmod style) */

);

extern int reads

(

    int    fd,      /* file descriptor from which to read */

    char * buffer,  /* pointer to buffer to receive bytes */

    size_t maxbytes /* max no. of bytes to read into buffer */

);

extern int writes

(

    int    fd,     /* file descriptor on which to write */

    char * buffer, /* buffer containing bytes to be written */

    size_t nbytes  /* number of bytes to write */

);

extern int ioctl

(

    int fd,       /* file descriptor */

    int function, /* function code */

    int arg       /* arbitrary argument */

);

extern STATUS closes

(

    int fd /* file descriptor to close */

);

STATUS pipeDelete(char* pipeName);

long long getTimeBase();


#ifdef  __cplusplus

}

#endif


#endif
