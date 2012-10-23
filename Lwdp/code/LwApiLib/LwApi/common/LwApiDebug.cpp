// Copyright (C), 2006��2011, Potevio Institute of Technology.
/************************************************************
*  File name:   LwApiDebug.cpp  	
*  Author:      guolisen
*  Version:   	v1.0
*  Create Date: 2011-08-12
***********************************************************/
// 2011.08.12: Create By Guolisen
#if 0
#include <LwDp.h>
//#include <Platform/LwApi/LwApi.h>


LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;





#if 0



LWRESULT Lw_Log_Event(x3LogType debug_level, const char_* debug_file, long_ code_line, const tchar_* debug_message, ...)                     \
{             
    tchar_ *buf = new tchar_[4096];
    ASSERT_CHECK_RET(LWDP_MALLOC_MEMORY_ERROR, (buf != NULL), "Malloc Memory Error!");
    
    va_list  args;
    va_start(args, debug_message);
    vsprintf(buf, debug_message, args);
    va_end(args);

    Cx_Interface<Ix_LogManager> pIFManager(x3::CLSID_LogManager);   
    if (pIFManager.IsNotNull())                                 
    {                                                                                                    
        pIFManager->WriteLog(debug_level, buf,__T"", debug_file, code_line); 
    }

    return LWDP_OK;
}

#define X3LOG_EVENT_2(name, extra, type, file, line)            \
    do {                                                        \
    Cx_Interface<Ix_LogManager> pIFManager(x3::CLSID_LogManager);   \
    if (pIFManager.IsNotNull())                                 \
    {                                                           \
        std::wostringstream _buf;                               \
        _buf << extra;                                          \
        pIFManager->WriteLog(type, name, _buf.str().c_str(), file, line); \
    }} while (0)

//! Output a ANSI string with additional context info.
/*!
    \ingroup _GROUP_PLUGIN_LOG_
    \param name ANSI string, the first tchar_ is '@' and leading as 'Module:StrID' format.
    \param extra additional context info, can be ANSI string or other type number.
    \param type logging type, see x3LogType.
    \param file source file name, __FILE__
    \param line code line, __LINE__
*/
#define X3LOG_EVENT_ANSI(name, extra, type, file, line)         \
    do {                                                        \
    Cx_Interface<Ix_LogManager> pIFManager(x3::CLSID_LogManager);   \
    if (pIFManager.IsNotNull())                                 \
    {                                                           \
        std::ostringstream _buf;                                \
        _buf << extra;                                          \
        pIFManager->WriteLog(type, name, _buf.str().c_str(), file, line); \
    }} while (0)


#endif

EXTERN_C_END;
LWDP_NAMESPACE_END;
    

#endif
