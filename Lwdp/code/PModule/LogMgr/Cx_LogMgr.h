#ifndef CX_LOG_MANAGER_H
#define CX_LOG_MANAGER_H

#include <LwDp.h>
#include <boost/shared_ptr.hpp>
#include <list>
#include <Interface/LogMgr/Ix_LogMgr.h>
#include <Interface/ConsoleMgr/Ix_ConsoleMgr.h>

LWDP_NAMESPACE_BEGIN;


typedef  std::list<MyAppender_Ptr> MyAppender_List;

	
namespace AT{
enum AppenderType
{
	STD,
	FILE,
	ROLFILE,
	REMOTE
};
};

class Cx_LogMgr
    : public Ix_LogMgr
{
    X3BEGIN_CLASS_DECLARE(Cx_LogMgr)
        X3DEFINE_INTERFACE_ENTRY(Ix_LogMgr)
    X3END_CLASS_DECLARE()
protected:
	Cx_LogMgr();
	virtual ~Cx_LogMgr();

private:
	LWRESULT Init();

private:
    // From Ix_LogMgr
	virtual LWRESULT RegisteAppender(MyAppenderFctory_Ptr appender_fctory);
	virtual void LogPrint(const tstring& module_name, 
						    LWDP_LOG_MGR::LwdpLogLevel level, 
						    const tchar_* file_name,
						    long line,
						    const char* stringFormat, ...);

	virtual LWRESULT LogSwitch(bool switch_tag);
private:
	virtual int32_ ConsoleLogSwitch(COMMAND_LINE& command_line);
	virtual int32_ ConsoleLogLevel(COMMAND_LINE& command_line);
	virtual log4cpp::Priority::PriorityLevel getLevel(const tstring& level_str);
	virtual log4cpp::Priority::PriorityLevel getLevel(LWDP_LOG_MGR::LwdpLogLevel level_enum);

private:
	MyAppender_List mInAppender;

public:
	static int32_ mLogSwitch;
};


LWDP_NAMESPACE_END;

#endif
