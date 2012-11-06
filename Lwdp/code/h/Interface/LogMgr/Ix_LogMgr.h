#ifndef LOGMGR_INTERFACE_H
#define LOGMGR_INTERFACE_H

#include <Interface/Ix_Object.h>
#include "Id_LogMgr.h"
#include <Interface/ConfigMgr/Ix_ConfigMgr.h>

LWDP_NAMESPACE_BEGIN;
 
namespace LWDP_LOG_MGR{
	enum LwdpLogLevel
	{
		DEBUG,	/* Messages containing information normally of use */
			/* only when debugging */
		INFO,	/* Informational messages */
		NOTICE,	/* Non-error conditions, which may require special */
			/* handling */
		WARNING,	/* Warning messages */
		ERR,	/* Errors */
		CRIT,	/* Critical conditions, such as hard device errors */
		ALERT,	/* A condition which requires immediate corrective */
			/* action */
		EMERG	/* Panic condition. The system is unstable. */
	};
};

typedef CTagEntry AppenderConfig;
class ILwdpAppender{
public:	
	ILwdpAppender(){}
	virtual ~ILwdpAppender(){}

	virtual void Init(const AppenderConfig& config) = 0;
	virtual void _append(const tstring& log_message) = 0;
};

class ILwdpAppenderFactory{
public:	
	ILwdpAppenderFactory(){}
	virtual ~ILwdpAppenderFactory(){}
	
	virtual ILwdpAppender* CreateObject() = 0;
	virtual const tstring _getAppenderName() = 0;
};

typedef  boost::shared_ptr<ILwdpAppender>  		MyAppender_Ptr;
typedef  boost::shared_ptr<ILwdpAppenderFactory>  MyAppenderFctory_Ptr;


INTERFACE_BEGIN(LogMgr)
	virtual LWRESULT Init() = 0;
	virtual LWRESULT RegisteAppender(MyAppenderFctory_Ptr appender_fctory) = 0;
	virtual void LogPrint(const tstring& module_name, 
						  LWDP_LOG_MGR::LwdpLogLevel level, 
						  const tchar_* file_name,
						  long line,
						  const char* stringFormat, ...) = 0;
INTERFACE_END();

#ifdef C99_VARIADIC_MACROS

#ifdef USE_LOG_MANAGER_DEF
#define LWDP_LOG_PRINT(moudel_name, level, ...) \
	do{ \
		Cx_Interface<Ix_LogMgr> iLogMgr(CLSID_LogMgr); \
		if (iLogMgr.IsNotNull()) \
		{ \
		    iLogMgr->LogPrint(moudel_name, level, __FILE__, __LINE__, __VA_ARGS__); \
		} \
	} while (0);
#else
#define LWDP_LOG_PRINT(moudel_name, level, ...)	do {} while (FALSE)
#endif

#else //C99_VARIADIC_MACROS

#ifdef USE_LOG_MANAGER_DEF
#define LWDP_LOG_PRINT(moudel_name, level, fmt...) \
	do{ \
		Cx_Interface<Ix_LogMgr> iLogMgr(CLSID_LogMgr); \
		if (iLogMgr.IsNotNull()) \
		{ \
		    iLogMgr->LogPrint(moudel_name, level, __FILE__, __LINE__, fmt); \
		} \
	} while (0);
#else
#define LWDP_LOG_PRINT(moudel_name, level, fmt...)	do {} while (FALSE)
#endif

#endif




LWDP_NAMESPACE_END;

#endif // LOGMGR_INTERFACE_H
