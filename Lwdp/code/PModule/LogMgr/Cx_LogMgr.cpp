#include <LwDp.h>
#include <PluginInc.h>
#include <iostream>

#include <LwApiLib/ComLib/log4cpp/Portability.hh>
#include <LwApiLib/ComLib/log4cpp/Category.hh>
#include <LwApiLib/ComLib/log4cpp/Appender.hh>
#include <LwApiLib/ComLib/log4cpp/FileAppender.hh>
#include <LwApiLib/ComLib/log4cpp/OstreamAppender.hh>
#include <LwApiLib/ComLib/log4cpp/Layout.hh>
#include <LwApiLib/ComLib/log4cpp/PatternLayout.hh>
#include <LwApiLib/ComLib/log4cpp/Priority.hh>
#include <LwApiLib/ComLib/log4cpp/NDC.hh>
#include <LwApiLib/ComLib/log4cpp/RollingFileAppender.hh>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/LuaMgr/Ix_LuaMgr.h>

#include "MyAppender.h"
#include "LogTagDef.h"
#include "Cx_LogMgr.h"
#include "tolua++.h"

int  tolua_LogMgr_open (lua_State* tolua_S);

LWDP_NAMESPACE_BEGIN;



Cx_LogMgr::Cx_LogMgr()
{
	mInAppender.clear();
}

Cx_LogMgr::~Cx_LogMgr()
{
}

LWRESULT Cx_LogMgr::Init()
{
	GET_OBJECT_RET(LuaMgr, iLuaMgr, LWDP_GET_OBJECT_ERROR);
	iLuaMgr->RegisteFuction((void*)tolua_LogMgr_open);

	XPropertyTable levelTable;
	log4cpp::Appender* appender = NULL;
	GET_OBJECT_RET(ConfigMgr, iConfigMgr, LWDP_GET_OBJECT_ERROR);

	RINOK(iConfigMgr->GetModuleTable(LW_LOGMGR_MODULE_NAME, LW_LOGMGR_MODULE_TABLE_NAME, levelTable));
	for(uint32_ i=0; levelTable[i].ThereIs; ++i)
	{
		tstring logTypeStr = levelTable[i][LW_LOGMGR_TABLE_TYPE_TAG];
		//Get Appender
		if(logTypeStr == static_cast<tstring>(LW_LOGMGR_TYPE_STD_TAG))
		{
			appender = new log4cpp::OstreamAppender("default", &std::cout); 
			ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_MALLOC_MEMORY_ERROR, (appender), "Can not new OstreamAppender!");
		}
		else if(logTypeStr == static_cast<tstring>(LW_LOGMGR_TYPE_FILE_TAG))
		{
			tstring fileName = levelTable[i][LW_LOGMGR_TABLE_FILE_NAME_TAG];
			if(fileName.empty())
			{
				fileName = levelTable[i][LW_LOGMGR_TABLE_MODULE_NAME_TAG] + __T(".LOG");
			}
			appender = new log4cpp::FileAppender("default", fileName.c_str());
			ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_MALLOC_MEMORY_ERROR, (appender), "Can not new FileAppender!");			
		}
		else if(logTypeStr == static_cast<tstring>(LW_LOGMGR_TYPE_ROLFILE_TAG))
		{
			tstring fileName = levelTable[i][LW_LOGMGR_TABLE_FILE_NAME_TAG];
			if(fileName.empty())
			{
				fileName = levelTable[i][LW_LOGMGR_TABLE_MODULE_NAME_TAG] + __T(".LOG");
			}

			ulong_ fileSize = 0x100000;
			tstring fileSizeStr = levelTable[i][LW_LOGMGR_TABLE_FILE_SIZE_TAG];
			if(!fileSizeStr.empty())
			{
				fileSize = (ulong_)atof(fileSizeStr.c_str());
			}
			appender = new log4cpp::RollingFileAppender("default", fileName.c_str(), fileSize);
			ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_MALLOC_MEMORY_ERROR, (appender), "Can not new RollingFileAppender!");
		}
		else 
		{
			continue;
		}

		//Set Log Pattern
		XPropertys prop;
		iConfigMgr->GetModulePropEntry(LW_LOGMGR_MODULE_NAME, LW_LOGMGR_LOG_PATTERN_TAG, prop);
		log4cpp::PatternLayout* layout = new log4cpp::PatternLayout();
		try {	
			if(!prop[0].propertyText.empty())
				layout->setConversionPattern(prop[0].propertyText);
		} catch(log4cpp::ConfigureFailure& f) {
			lw_log_err(LWDP_MODULE_LOG, "Set LOG Pattern(%s) Error: %s", prop[0].propertyText.c_str(), f.what());
		}	
		appender->setLayout(layout);

		//Set Category
		tstring moduleName = levelTable[i][LW_LOGMGR_TABLE_MODULE_NAME_TAG]; 
		if(log4cpp::Category::exists(moduleName))
		{
			lw_log_debug(LWDP_MODULE_LOG, "Log Module() Has Exist!", moduleName.c_str());
			continue;
		}
		
		log4cpp::Category& category = log4cpp::Category::getInstance(moduleName);
		category.addAppender(appender);

		//Set Level
		log4cpp::Priority::PriorityLevel level = log4cpp::Priority::INFO;
		tstring levelStr = levelTable[i][LW_LOGMGR_TABLE_LEVEL_ATTR_TAG];
		if(!levelStr.empty())
			level = getLevel(levelStr);
		else
		{
			prop.Clear();
			iConfigMgr->GetModulePropEntry(LW_LOGMGR_MODULE_NAME, LW_LOGMGR_GLOBAL_LEVEL_TAG, prop);
			if(!prop[0].propertyText.empty())
				level = getLevel(prop[0].propertyText);
			else
				level = log4cpp::Priority::INFO;
		}
		category.setPriority(level);

	}

	return LWDP_OK;
}

log4cpp::Priority::PriorityLevel Cx_LogMgr::getLevel(const tstring& level_str)
{
	if(level_str == "DEBUG")
	{
		return log4cpp::Priority::DEBUG;
	}
	else if(level_str == "INFO")
	{
		return log4cpp::Priority::INFO;
	}
	else if(level_str == "NOTICE")
	{
		return log4cpp::Priority::NOTICE;
	}
	else if(level_str == "WARNING")
	{
		return log4cpp::Priority::WARN;
	}
	else if(level_str == "ERROR")
	{
		return log4cpp::Priority::ERROR;
	}
	else if(level_str == "CRIT")
	{
		return log4cpp::Priority::CRIT;
	}
	else if(level_str == "ALERT")
	{
		return log4cpp::Priority::ALERT;
	}
	else if(level_str == "EMERG")
	{
		return log4cpp::Priority::EMERG;
	}
		
	return log4cpp::Priority::NOTICE;
}

log4cpp::Priority::PriorityLevel Cx_LogMgr::getLevel(LWDP_LOG_MGR::LwdpLogLevel level_enum)
{
	switch(level_enum)
	{
		case LWDP_LOG_MGR::DEBUG:
			return log4cpp::Priority::DEBUG;
		case LWDP_LOG_MGR::INFO:
			return log4cpp::Priority::INFO;
		case LWDP_LOG_MGR::NOTICE:
			return log4cpp::Priority::NOTICE;
		case LWDP_LOG_MGR::WARNING:
			return log4cpp::Priority::WARN;
		case LWDP_LOG_MGR::ERR:
			return log4cpp::Priority::ERROR;
		case LWDP_LOG_MGR::CRIT:
			return log4cpp::Priority::CRIT;
		case LWDP_LOG_MGR::ALERT:
			return log4cpp::Priority::ALERT;
		case LWDP_LOG_MGR::EMERG:
			return log4cpp::Priority::EMERG;
		default:
			return log4cpp::Priority::NOTICE;
	};

	return log4cpp::Priority::NOTICE;
}


LWRESULT Cx_LogMgr::RegisteAppender(MyAppenderFctory_Ptr appender_fctory)
{
	XPropertyTable levelTable;
	GET_OBJECT_RET(ConfigMgr, iConfigMgr, LWDP_GET_OBJECT_ERROR);
	RINOK(iConfigMgr->GetModuleTable(LW_LOGMGR_MODULE_NAME, LW_LOGMGR_MODULE_TABLE_NAME, levelTable));
	for(uint32_ i=0; levelTable[i].ThereIs; ++i)
	{ 
		if(levelTable[i][LW_LOGMGR_TABLE_TYPE_TAG] == appender_fctory->_getAppenderName())
		{
			log4cpp::Appender* logAppender = NULL;
			MyAppender_Ptr pAppender(appender_fctory->CreateObject());
			pAppender->Init(levelTable[i]);
			logAppender = new log4cpp::MyAppender(pAppender);
			ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_MALLOC_MEMORY_ERROR, (logAppender), "Can not new MyAppender(%s)!", appender_fctory->_getAppenderName().c_str());

			//Set Log Pattern
			XPropertys prop;
			iConfigMgr->GetModulePropEntry(LW_LOGMGR_MODULE_NAME, LW_LOGMGR_LOG_PATTERN_TAG, prop);
			log4cpp::PatternLayout* layout = new log4cpp::PatternLayout();
			try {	
				if(!prop[0].propertyText.empty())
					layout->setConversionPattern(prop[0].propertyText);
			} catch(log4cpp::ConfigureFailure& f) {
				lw_log_err(LWDP_MODULE_LOG, "Set LOG Pattern(%s) Error: %s", prop[0].propertyText.c_str(), f.what());
			}	
			logAppender->setLayout(layout);

			//Set Category
			tstring moduleName = levelTable[i][LW_LOGMGR_TABLE_MODULE_NAME_TAG];
			if(log4cpp::Category::exists(moduleName))
			{
				lw_log_debug(LWDP_MODULE_LOG, "Log Module() Has Exist!", moduleName.c_str());
				return LWDP_OK;
			}

			log4cpp::Category& category = log4cpp::Category::getInstance(moduleName);
			category.addAppender(logAppender);

			//Set Level
			log4cpp::Priority::PriorityLevel level = log4cpp::Priority::INFO;
			tstring levelStr = levelTable[i][LW_LOGMGR_TABLE_LEVEL_TAG];
			if(!levelStr.empty())
				level = getLevel(levelStr);
			else
			{
				prop.Clear();
				iConfigMgr->GetModulePropEntry(LW_LOGMGR_MODULE_NAME, LW_LOGMGR_GLOBAL_LEVEL_TAG, prop);
				if(!prop[0].propertyText.empty())
					level = getLevel(prop[0].propertyText);
				else
					level = log4cpp::Priority::INFO;
			}
			category.setPriority(level);

		}
	}

	return LWDP_OK;
}


#if defined(_MSC_VER)
    #define VSNPRINTF _vsnprintf
#else
#ifdef LOG4CPP_HAVE_SNPRINTF
    #define VSNPRINTF vsnprintf
#else
/* use alternative snprintf() from http://www.ijs.si/software/snprintf/ */

#define VSNPRINTF Api_snprintf

#endif // LOG4CPP_HAVE_SNPRINTF
#endif // _MSC_VER

void Cx_LogMgr::LogPrint(const tstring& module_name, 
						    LWDP_LOG_MGR::LwdpLogLevel level, 
						    const tchar_* file_name,
						    long line,
						    const char* stringFormat, ...)
{
	if(module_name.empty() || !stringFormat)
		return;
	
	if(!log4cpp::Category::exists(module_name))
	{
		lw_log_warning(LWDP_MODULE_LOG, "LogMgr Can't Find Module(%s) Category Info", module_name.c_str());
		return;
	}

	ulong_ size = 1024;
	tstring formatStr = "";
	char* buffer = new char[size];
    va_list  args;
    va_start(args, stringFormat);	
	while (1) {
	    int n = VSNPRINTF(buffer, size, stringFormat, args);
                
	    // If that worked, return a string.
	    if ((n > -1) && (static_cast<ulong_>(n) < size)) {
			formatStr =  static_cast<tstring>(buffer);
			break;
	    }
                
	    // Else try again with more space.
            size = (n > -1) ?
                n + 1 :   // ISO/IEC 9899:1999
                size * 2; // twice the old size
                
	    DELETE_MULTIPLE(buffer);
	    buffer = new char[size];
	}
	DELETE_MULTIPLE(buffer);
    va_end(args);

#ifdef USE_FILENAME_LINENUM_LOG
 	if(file_name)
 	{
 		tchar_  numStr[1024] = {0};
		tchar_* fileName = Api_PathFindFileName(file_name);
		Api_sprintf(numStr, __T(" [%s:%d]"), fileName, line);
		formatStr = formatStr + static_cast<tstring>(numStr);
 	}
#endif

	log4cpp::Category& category = log4cpp::Category::getInstance(module_name);
	category.log(getLevel(level), formatStr);

}


LWDP_NAMESPACE_END;

