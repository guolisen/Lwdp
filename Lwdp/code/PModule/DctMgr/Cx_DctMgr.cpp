
#include <Lwdp.h>
#include <PluginInc.h>
#include <iostream>

#include "DataCollectTool.h"
#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>

#include "DctTagDef.h"
#include "Cx_DctMgr.h"


LWDP_NAMESPACE_BEGIN;

class DctAppender :public ILwdpAppender
{
public:	
	DctAppender(){}
	virtual ~DctAppender(){}

	virtual void Init(const AppenderConfig& config)
	{
		printf("DctAppender Init(WinId: %s)\n", config[__T("WinId")].c_str());
		mWinId = atol(config[__T("WinId")].c_str());
	}
	virtual void _append(const tstring& log_message)
	{
		tstring retMsg = log_message + "\r\n";
		DataCollectTool::MmlPrintf(mWinId, retMsg.c_str());
	}

protected:
	int mWinId;

};

class DctAppenderFactory :public ILwdpAppenderFactory
{
public:	
	DctAppenderFactory(){}
	virtual ~DctAppenderFactory(){}
	
	virtual ILwdpAppender* CreateObject()
	{
		return (ILwdpAppender*)new DctAppender;
	}
	virtual const tstring _getAppenderName()
	{
		return static_cast<tstring>("REMOTE");
	}
};

Cx_DctMgr::Cx_DctMgr()
{
}

Cx_DctMgr::~Cx_DctMgr()
{
}

LWRESULT Cx_DctMgr::Init()
{
    DataCollectTool * dct_ptr = new DataCollectTool();
	if(dct_ptr->MmlInit())
	{
		LWDP_LOG_PRINT("Test", LWDP_LOG_MGR::ERR, "Init DctMgr Error");
		return LWDP_ERROR;
	}

	GET_OBJECT(LogMgr, iLogMgr, LWDP_GET_OBJECT_ERROR);
	RINOK(iLogMgr->RegisteAppender(MyAppenderFctory_Ptr(new DctAppenderFactory)));

	return LWDP_OK;
}



LWDP_NAMESPACE_END;

