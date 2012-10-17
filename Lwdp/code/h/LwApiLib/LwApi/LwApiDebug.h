/*! \file LogHelper.h
 *  \brief Define logging macros such as X3LOG_WARNING and X3LogGroup.
 *  \author Zhang Yun Gui, X3 C++ PluginFramework
 *  \date   2011.07.01
 */
#ifndef X3_LOG_LOGHELPER_H_
#define X3_LOG_LOGHELPER_H_

#include "XComPtr.h"
#include "Ix_LogManager.h"

#include <sstream>      // std::wostringstream

// X3LOG_DEBUG(msg)
// X3LOG_DEBUG2(name, extra)
// X3LOG_INFO(msg)
// X3LOG_INFO2(name, extra)
// X3LOG_WARNING(msg)
// X3LOG_WARNING2(name, extra)
// X3LOG_ERROR(msg)
// X3LOG_ERROR2(name, extra)
// X3LOG_FATAL(msg)
// X3LOG_FATAL2(name, extra)
// X3LOG_EVENT_ANSI(name, extra, type, file, line)
//
// x3::RegisterLogObserver(Ix_LogObserver* observer)
// x3::UnRegisterLogObserver(Ix_LogObserver* observer)
// X3LogGroup(msg)


//! Output a debug info.
/*!
    \ingroup _GROUP_PLUGIN_LOG_
    \param msg logging text, can be UNICODE string or other type number (not ANSI string).
        if the first tchar_ is '@' and leading as 'Module:StrID' format,
        then the manager will translate into localization text.
    \see X3LOG_DEBUG2, X3LogGroup
*/
#define X3LOG_DEBUG(msg)  \
    X3LOG_EVENT_(msg, x3LogType_Debug, __FILE__, __LINE__)


//! Output a debug info with additional context info.
/*!
    \ingroup _GROUP_PLUGIN_LOG_
    \param name UNICODE string, the first tchar_ is '@' and leading as 'Module:StrID' format.
    \param extra additional context info, can be UNICODE string or other type number (not ANSI string).
    \see X3LOG_INFO, X3LOG_DEBUG, X3LogGroup
*/
#define X3LOG_DEBUG2(name, extra)     \
    X3LOG_EVENT_2(name, extra, x3LogType_Debug, __FILE__, __LINE__)


//! Output a normal info.
/*!
    \ingroup _GROUP_PLUGIN_LOG_
    \param msg logging text, can be UNICODE string or other type number (not ANSI string).
        if the first tchar_ is '@' and leading as 'Module:StrID' format,
        then the manager will translate into localization text.
    \see X3LOG_INFO2, X3LogGroup
*/
#define X3LOG_INFO(msg)   \
    X3LOG_EVENT_(msg, x3LogType_Info, __FILE__, __LINE__)


//! Output a normal info with additional context info.
/*!
    \ingroup _GROUP_PLUGIN_LOG_
    \param name UNICODE string, the first tchar_ is '@' and leading as 'Module:StrID' format.
    \param extra additional context info, can be UNICODE string or other type number (not ANSI string).
    \see X3LOG_WARNING2, X3LOG_INFO, X3LogGroup
*/
#define X3LOG_INFO2(name, extra)  \
    X3LOG_EVENT_2(name, extra, x3LogType_Info, __FILE__, __LINE__)


//! Output a warnning info.
/*!
    \ingroup _GROUP_PLUGIN_LOG_
    \param msg logging text, can be UNICODE string or other type number (not ANSI string).
        if the first tchar_ is '@' and leading as 'Module:StrID' format,
        then the manager will translate into localization text.
    \see X3LOG_WARNING2, X3LOG_INFO2, X3LogGroup
*/
#define X3LOG_WARNING(msg)        \
    X3LOG_EVENT_(msg, x3LogType_Warning, __FILE__, __LINE__)


//! Output a warnning info with additional context info.
/*!
    \ingroup _GROUP_PLUGIN_LOG_
    \param name UNICODE string, the first tchar_ is '@' and leading as 'Module:StrID' format.
    \param extra additional context info, can be UNICODE string or other type number (not ANSI string).
    \see X3LOG_ERROR2, X3LogGroup
*/
#define X3LOG_WARNING2(name, extra)   \
    X3LOG_EVENT_2(name, extra, x3LogType_Warning, __FILE__, __LINE__)


//! Output a error info.
/*!
    \ingroup _GROUP_PLUGIN_LOG_
    \param msg logging text, can be UNICODE string or other type number (not ANSI string).
        if the first tchar_ is '@' and leading as 'Module:StrID' format,
        then the manager will translate into localization text.
    \see X3LOG_ERROR2, X3LogGroup
*/
#define X3LOG_ERROR(msg)  \
    X3LOG_EVENT_(msg, x3LogType_Error, __FILE__, __LINE__)


//! Output a error info with additional context info.
/*!
    \ingroup _GROUP_PLUGIN_LOG_
    \param name UNICODE string, the first tchar_ is '@' and leading as 'Module:StrID' format.
    \param extra additional context info, can be UNICODE string or other type number (not ANSI string).
    \see X3LOG_WARNING2, X3LogGroup
*/
#define X3LOG_ERROR2(name, extra)     \
    X3LOG_EVENT_2(name, extra, x3LogType_Error, __FILE__, __LINE__)


//! Output a fatal error info.
/*!
    \ingroup _GROUP_PLUGIN_LOG_
    \param msg logging text, can be UNICODE string or other type number (not ANSI string).
        if the first tchar_ is '@' and leading as 'Module:StrID' format,
        then the manager will translate into localization text.
    \see X3LOG_FATAL2, X3LogGroup
*/
#define X3LOG_FATAL(msg)      \
    X3LOG_EVENT_(msg, x3LogType_Fatal, __FILE__, __LINE__)


//! Output a fatal error info with additional context info.
/*!
    \ingroup _GROUP_PLUGIN_LOG_
    \param name UNICODE string, the first tchar_ is '@' and leading as 'Module:StrID' format.
    \param extra additional context info, can be UNICODE string or other type number (not ANSI string).
    \see X3LOG_ERROR2, X3LogGroup
*/
#define X3LOG_FATAL2(name, extra)     \
    X3LOG_EVENT_2(name, extra, x3LogType_Fatal, __FILE__, __LINE__)

//! namespace for X3 C++ PluginFramework
namespace x3 {

//! Register a logging output observer.
/*!
    \ingroup _GROUP_PLUGIN_LOG_
    \see x3::UnRegisterLogObserver
*/
inline bool RegisterLogObserver(Ix_LogObserver* observer)
{
    Cx_Interface<Ix_LogManager> pIFManager(x3::CLSID_LogManager);
    if (pIFManager.IsNotNull())
    {
        return pIFManager->RegisterObserver(observer, GetModuleModId());
    }
    return false;
}

//! Unregister a logging output observer.
/*!
    \ingroup _GROUP_PLUGIN_LOG_
    \see x3::RegisterLogObserver
*/
inline void UnRegisterLogObserver(Ix_LogObserver* observer)
{
    Cx_Interface<Ix_LogManager> pIFManager(x3::CLSID_LogManager);
    if (pIFManager.IsNotNull())
    {
        pIFManager->UnRegisterObserver(observer);
    }
}

} // x3

//! Helper class for logging group, auto begin and end group.
/*! Use this class to define variable in local function, eg:
    \code
    void MyFunc()
    {
        X3LogGroup group(__T"@MyPlugin:GROUP_MYNAME");
        X3LOG_INFO2(__T"@MyPlugin:IDS_DO_THING", value);
        ...
    }
    \endcode
    \ingroup _GROUP_PLUGIN_LOG_
*/
class X3LogGroup
{
public:
    //! Constructor, auto begin a new logging group.
    /*!
        \param msg group message text.
            if the first tchar_ is '@' and leading as 'Module:StrID' format
            then the manager will translate into localization text using Ix_StringTable.
        \param extra additional context info.
    */
    X3LogGroup(const tchar_* msg, const tchar_* extra = NULL)
    {
        Cx_Interface<Ix_LogManager> pIFManager(x3::CLSID_LogManager);
        if (pIFManager.IsNotNull())
        {
            pIFManager->PushGroup(msg, extra);
        }
    }
    //! Destructor, auto end the logging group.
    ~X3LogGroup()
    {
        Cx_Interface<Ix_LogManager> pIFManager(x3::CLSID_LogManager);
        if (pIFManager.IsNotNull())
        {
            pIFManager->PopGroup();
        }
    }
};

#ifdef _MSC_VER                 // hide warnings
#pragma warning(disable:4127)   // conditional expression is constant
#endif

#define X3LOG_EVENT_(msg, type, file, line)                     \
    do {                                                        \
    Cx_Interface<Ix_LogManager> pIFManager(x3::CLSID_LogManager);   \
    if (pIFManager.IsNotNull())                                 \
    {                                                           \
        std::wostringstream _buf;                               \
        _buf << msg;                                            \
        pIFManager->WriteLog(type, _buf.str().c_str(),__T"", file, line); \
    }
    }} while (0)

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


#define XCLSID              LWCLSID
#define CAutoLogGroup       X3LogGroup
#define CLogObserverImpl    Cx_LogObserverImpl

#define kLogType            x3LogType
#define kLogType_Info       x3LogType_Info
#define kLogType_Warning    x3LogType_Warning
#define kLogType_Error      x3LogType_Error

#define LOG_DEBUG           X3LOG_DEBUG
#define LOG_DEBUG2          X3LOG_DEBUG2
#define LOG_INFO            X3LOG_INFO
#define LOG_INFO2           X3LOG_INFO2
#define LOG_WARNING         X3LOG_WARNING
#define LOG_WARNING2        X3LOG_WARNING2
#define LOG_ERROR           X3LOG_ERROR
#define LOG_ERROR2          X3LOG_ERROR2
#define LOG_FATAL           X3LOG_FATAL
#define LOG_FATAL2          X3LOG_FATAL2
#define X3LogGroup          X3LogGroup

#define CConfigIOSection    Cx_ConfigSection
#define CConfigTransaction  Cx_ConfigTransaction

using namespace x3;


#endif // X3_LOG_LOGHELPER_H_
