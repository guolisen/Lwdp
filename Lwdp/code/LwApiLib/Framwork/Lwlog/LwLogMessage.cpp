// Copyright (C), 2006��2011, Potevio Institute of Technology.
/************************************************************
*  File name:   LwLogMessage.cpp  	
*  Author:      guolisen
*  Version:   	v1.0
*  Create Date: 2011-08-11
***********************************************************/
// 2011.08.11: Create By Guolisen
#include <wchar.h>
#include <LwDp.h>
//#include "../Common/func_s.h"
#include <LwApi.h>
#include <LwApiLib/Framwork/Lwlog/LwLogMessage.h>

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;

static const tchar_ * logPriNames [8] =
{
    __T("M"),
    __T("A"),
    __T("C"),
    __T("E"),
    __T("W"),
    __T("N"),
    __T("I"),
    __T("D")
};


/* List of category name strings, e.g. RIPNG_LOG -> "RIPNG" */

static const tchar_ * logCatNames [] =
    {
#undef LOG_DB
#undef LOG_DB_LAST
#define LOG_DB(x, y, z) __T(#x),
#define LOG_DB_LAST(x, y, z) __T(#x)
#include <lwlogCategories.h>
    };

LWDP_CATEGORY_INFO logcatInfo [] =
    {
#undef LOG_DB
#undef LOG_DB_LAST
#define LOG_DB(x, y, z) {y,  z},
#define LOG_DB_LAST(x, y, z) {y,  z}
#include <lwlogCategories.h>
    };

#define CAT_INFO(x) logcatInfo


/****************************************************************************
*  Function:       LwdpLoggerInit
*  Description:    
*  Input:          ��
*  Output:         // ����������˵��
*  Return:         // �����ֵ��˵��	
*  Others:         // ����˵��
*****************************************************************************/
LWRESULT Api_LoggerFdSet (int32_  category, int32_  fd)
{
    LWDP_CATEGORY_INFO * pInfo = logcatInfo;
    
    if (category & ~LWDP_CAT_MASK)
	    return LWDP_LOGMSG_CATEGORY_ERROR;

    category = LOG_CAT (category);

    if ((category < 0) || (category >= LWDP_LOG_NUMCATS))
	    return LWDP_LOGMSG_CATEGORY_ERROR;

    pInfo [category].fd = fd;

    return LWDP_OK;
}

/****************************************************************************
*  Function:       LwdpLogLevelChange
*  Description:    
*  Input:          ��
*  Output:         // ����������˵��
*  Return:         // �����ֵ��˵��	
*  Others:         // ����˵��
*****************************************************************************/
LWRESULT Api_LogLevelSet (int32_ category, uint32_ mask)
{
    LWDP_CATEGORY_INFO * pInfo = logcatInfo;

    if (category & ~LWDP_CAT_MASK)
	    return LWDP_LOGMSG_CATEGORY_ERROR;

    /* do some sanity checks */
    category = LOG_CAT (category);

    if ((uint32_)category >= LWDP_LOG_NUMCATS)
	    return LWDP_LOGMSG_CATEGORY_ERROR;

    if (mask & ~0xff) /* allow any combination of the 8 priority levels */
	    return LWDP_LOGMSG_LOGLEVEL_ERROR;

    pInfo [category].level &= ~0xff;
    pInfo [category].level |= mask;

    lw_log_info (category, __T("Changed Category \"%s\" logging mask to 0x%02x"),
	             logCatNames [category], mask);

    return LWDP_OK;
}

/****************************************************************************
*  Function:       LwdpLogLevelGet
*  Description:    
*  Input:          ��
*  Output:         // ����������˵��
*  Return:         // �����ֵ��˵��	
*  Others:         // ����˵��
*****************************************************************************/
LWRESULT Api_LogLevelGet (int32_ category, uint32_* log_level)
{
    LWDP_CATEGORY_INFO * pInfo = logcatInfo;

    //ASSERT_CHECK_RET(LWDP_LOG, LWDP_ERROR, log_level!=NULL, "file is ERROR");
    //ASSERT_CHECK_HALT(LWDP_LOG, log_level!=NULL, "file is HALT ERROR");

    if(!log_level)
        return LWDP_PARAMETER_ERROR;

    if (category & ~LWDP_CAT_MASK)
	    return LWDP_LOGMSG_CATEGORY_ERROR;

    /* do some sanity checks */
    category = LOG_CAT (category);

    if ((uint32_)category >= LWDP_LOG_NUMCATS)
	    return LWDP_LOGMSG_CATEGORY_ERROR;

    *log_level = pInfo[category].level & 0xff;

    lw_log_debug(category, __T("Get Module(%s) Log Level(%x)"),
	             logCatNames [category], *log_level);

    return LWDP_OK;

}


/****************************************************************************
*  Function:       LwdpLogDecorationChange
*  Description:    
*  Input:          ��
*  Output:         // ����������˵��
*  Return:         // �����ֵ��˵��	
*  Others:         // ����˵��
*****************************************************************************/
uint32_  Api_LogDecorationChange (int32_ category, uint32_ disableMask, uint32_ enableMask)
{
    uint32_ level;
    LWDP_CATEGORY_INFO * pInfo = logcatInfo;

    /* do some sanity checks */

    if (category & ~LWDP_CAT_MASK)
	    return (uint32_)LWDP_ERROR;

    category = LOG_CAT (category);

    if ((uint32_)category >= LWDP_LOG_NUMCATS)
	     return (uint32_)LWDP_ERROR;

    disableMask <<= 8;  /* avoid priority bits */
    enableMask <<= 8;

    level = pInfo[category].level;

    pInfo[category].level = (level & ~disableMask) | enableMask;

    lw_log_debug(category, __T("Change Module(%s) Decoration Log Level(New: %x Old: %x)"),
	             logCatNames [category], pInfo[category].level >> 8, (level >> 8));

    return (level >> 8);
}


/****************************************************************************
*  Function:       LwdpLoggerInfoShow
*  Description:    
*  Input:          ��
*  Output:         // ����������˵��
*  Return:         // �����ֵ��˵��	
*  Others:         // ����˵��
*****************************************************************************/
void Api_LoggerInfoShow()
{
    int32_ i;
    LWDP_CATEGORY_INFO * pInfo = logcatInfo;

    printf ("\n"
			"Facility        ( code )    FileDesc  Decoration/Priority\n");
    
    for (i = 0; i < LWDP_LOG_NUMCATS; i++)
    {
        printf ("%-15s (0x%04x)  %10d     %#10x\n",
                logCatNames[i], i << LWDP_CAT_SHIFT,
                pInfo[i].fd, pInfo[i].level);
    }
}

#ifdef _WIN32
	#define LOGMSG_SET_COLOR(color) \
	{ \
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); \
        SetConsoleTextAttribute(h, color); \
	}
#else
	#define LOGMSG_SET_COLOR(color)      
#endif

LWRESULT SetConsoleColorEnter(int32_ dbLevel)
{
#ifdef _WIN32
    switch(dbLevel)
    {
        case LWDP_LOG_EMERG:
            LOGMSG_SET_COLOR(0x5|FOREGROUND_INTENSITY);
            break;
        case LWDP_LOG_ALERT:
            LOGMSG_SET_COLOR(0x3|FOREGROUND_INTENSITY);
            break;
        case LWDP_LOG_CRIT:
            LOGMSG_SET_COLOR(0x6|FOREGROUND_INTENSITY);
            break;
        case LWDP_LOG_ERR:
            LOGMSG_SET_COLOR(FOREGROUND_RED|FOREGROUND_INTENSITY);
            break;
        case LWDP_LOG_WARNING:
            LOGMSG_SET_COLOR(0x7|FOREGROUND_INTENSITY);
            break;
        case LWDP_LOG_NOTICE:
            LOGMSG_SET_COLOR(0xA|FOREGROUND_INTENSITY);
            break;
        case LWDP_LOG_INFO:
            LOGMSG_SET_COLOR(0x7);
            break;
        case LWDP_LOG_DEBUG:
            LOGMSG_SET_COLOR(0x8|FOREGROUND_INTENSITY);
            break;
    };
#endif
    return LWDP_OK;
}

LWRESULT SetConsoleColorLeave()
{
#ifdef _WIN32
    LOGMSG_SET_COLOR(0x7);
#endif
    return LWDP_OK;
}

/****************************************************************************
*  Function:       _lw_applLog
*  Description:    
*  Input:          ��
*  Output:         // ����������˵��
*  Return:         // �����ֵ��˵��	
*  Others:         // ����˵��
*****************************************************************************/
LWRESULT _lw_applLog(int32_ priCat, const char_ * file, int32_ line, const tchar_ * fmt, ...)
{
    ulong_ category = LOG_CAT (priCat);
    int32_ pri;
    int32_ fd;
    LWDP_CATEGORY_INFO * pCat;
    va_list args;
    tchar_* decoration = NULL;
    tchar_* fileName = NULL;
    int32_ n;
    int32_ err = 1;
    
    if(!file || !fmt)
        return LWDP_PARAMETER_ERROR;
        
    if (category > LOG_CAT(LWDP_GENERAL_LOG))
	    category = LOG_CAT(LWDP_GENERAL_LOG);

    pCat = logcatInfo + category;

    pri = (priCat & 0x7);

    if ((pCat->level & (1 << pri)) == 0)
	    return LWDP_LOGMSG_LEVEL_LOW;

    fd = pCat->fd;  /* should we collect fd inside of logging semaphore ? */
    if (fd < 0)
	    return LWDP_LOGMSG_OUTPUT_FD_ERROR;

    priCat ^= (pCat->level >> 8); /* for LOG_DECOR, LOG_LOC, LOG_TIME etc. */

    //err = errno;

    n = 0;
    decoration = new tchar_[LOG_LINE_STR_LEN];
    if(!decoration)
        return LWDP_MALLOC_MEMORY_ERROR;
    

    memset(decoration, 0, LOG_LINE_STR_LEN);
    if ((priCat & LOG_DECOR) != 0)
	{	
    	//n = Api_snprintf (decoration, LOG_LINE_STR_LEN-1,
        //    		      __T("[%s][%s] "), logPriNames[pri], logCatNames[category]);
        n = Api_sprintf (decoration, 
            		      __T("[%s][%s] "), logPriNames[pri], logCatNames[category]);

	}

    if (file != NULL && (priCat & LOG_LOC) != 0)
	{
		
#ifdef UNICODE
		UString ustr = MultiByteToUnicodeString(file);
		const tchar_* tmpFileName = (const wchar_*)ustr;
#else
		const tchar_* tmpFileName = file;
#endif
		
	    fileName = Api_PathFindFileName(tmpFileName);
        if(!fileName)
            fileName = __T("NO_FILENAME");
    	//n += Api_snprintf (decoration + n, LOG_LINE_STR_LEN - n,
    	//	       			__T("%s:%d: "), fileName, line);
		n += Api_sprintf (decoration + n, 
    		       			__T("%s:%d: "), fileName, line);
	}

#ifdef ENABLE_LOG_TIME
#if 0
    if ((priCat & LOG_TIME) != 0)
	{
	tchar_ timestr [sizeof (ASCBUF)];
	time_t tod = time (NULL);
	size_t buflen = sizeof (ASCBUF);

	ctime_r (&tod, timestr, &buflen);
	timestr [24] = '\0'; /* replace '\n' with '\0' */

	n += snprintf (decoration + n, sizeof (decoration) - n,
		       "%s ", timestr);
	}
#endif    
#endif

    if ((priCat & LOG_ERRNO) != 0 && err != 0)
	{
        tchar_ buf [LOG_NAME_MAX_STR + 1] = {0};

        //strerror_r (err, buf);
        //n += Api_snprintf (decoration + n, LOG_LINE_STR_LEN - n,
		//	__T("[%x:%s] "), err, Api_strlen(buf) ? buf : __T("NO_DESCRIP"));
		n += Api_sprintf (decoration + n, 
			__T("[%x:%s] "), err, Api_strlen(buf) ? buf : __T("NO_DESCRIP"));

	}

    va_start (args, fmt);

    /* The semaphore is used to avoid intermixing of logging output from
     * different tasks.
     */

#ifndef _WRS_KERNEL
#if 0
	if (applUtilLogSem != NULL || (applUtilLogSem = logSemAcquire ()) != NULL)
	{
	    semTake (applUtilLogSem, WAIT_FOREVER);
#endif        
#endif
		//uint32_ fmtLens = Api_vscprintf(fmt, args) + 1;
	    n += Api_vsprintf(decoration+n, fmt, args);
        SetConsoleColorEnter(pri);
#ifdef UNICODE
		n *= 2;
#endif
	    write (fd, decoration, n);
        SetConsoleColorLeave();
	    write (fd, __T("\n"), 1);
        
	    
#ifndef _WRS_KERNEL
#if 0
        semGive (applUtilLogSem);
    }
#endif    
#endif
	
    va_end (args);

    DELETE_MULTIPLE(decoration);
    return LWDP_OK;
}


EXTERN_C_END;
LWDP_NAMESPACE_END;
    

