// Copyright 2008-2011 Guolisen, guolisen@hotmail.com
// 
// 2011.11.10 Create File By Guolisen

#include <LwDp.h>
#include <PluginInc.h>

#include <Interface/ConfigMgr/Ix_XmlMgr.h>
#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/ScriptMgr/Ix_ScriptMgr.h>
#include "ScriptEngine.h"
#include "ScriptAction.h"


LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;

LWRESULT APrint::DoAction(const ParamList& param_list, const tstring& value, tstring& ret_str)
{
	ParamList tmpList = param_list;
	if(tmpList[0].value == __T("INFO") || 
		((tmpList[0].name == LW_ACTION_PRINT_PARA_LEVEL) && tmpList[0].value.empty()))
	{
		if(tmpList.size() == 1)
			lw_log_info(LWDP_MODULE_LOG, value.c_str());
		else if(tmpList.size() == 2)
			lw_log_info(LWDP_MODULE_LOG, value.c_str(), GETWORD(tmpList[1].value).c_str());
		else if(tmpList.size() == 3)
			lw_log_info(LWDP_MODULE_LOG, value.c_str(), GETWORD(tmpList[1].value).c_str(), 
														GETWORD(tmpList[2].value).c_str());
		else if(tmpList.size() == 4)
			lw_log_info(LWDP_MODULE_LOG, value.c_str(), GETWORD(tmpList[1].value).c_str(), 
														GETWORD(tmpList[2].value).c_str(),
														GETWORD(tmpList[3].value).c_str());

		else if(tmpList.size() == 5)
			lw_log_info(LWDP_MODULE_LOG, value.c_str(), GETWORD(tmpList[1].value).c_str(), 
														GETWORD(tmpList[2].value).c_str(),
														GETWORD(tmpList[3].value).c_str(),
														GETWORD(tmpList[4].value).c_str());

	
		else if(tmpList.size() == 6)
			lw_log_info(LWDP_MODULE_LOG, value.c_str(), GETWORD(tmpList[1].value).c_str(), 
														GETWORD(tmpList[2].value).c_str(),
														GETWORD(tmpList[3].value).c_str(),
														GETWORD(tmpList[4].value).c_str(),
														GETWORD(tmpList[5].value).c_str());

	

	}
	else if(tmpList[0].value == __T("ERROR"))
	{
		if(tmpList.size() == 1)
			lw_log_err(LWDP_MODULE_LOG, value.c_str());
		else if(tmpList.size() == 2)
			lw_log_err(LWDP_MODULE_LOG, value.c_str(), GETWORD(tmpList[1].value).c_str());
		else if(tmpList.size() == 3)
			lw_log_err(LWDP_MODULE_LOG, value.c_str(), GETWORD(tmpList[1].value).c_str(), 
													   GETWORD(tmpList[2].value).c_str());
		else if(tmpList.size() == 4)
			lw_log_err(LWDP_MODULE_LOG, value.c_str(), GETWORD(tmpList[1].value).c_str(), 
													   GETWORD(tmpList[2].value).c_str(),
													   GETWORD(tmpList[3].value).c_str());

		else if(tmpList.size() == 5)
			lw_log_err(LWDP_MODULE_LOG, value.c_str(), GETWORD(tmpList[1].value).c_str(), 
													   GETWORD(tmpList[2].value).c_str(),
													   GETWORD(tmpList[3].value).c_str(),
													   GETWORD(tmpList[4].value).c_str());

	
		else if(tmpList.size() == 6)
			lw_log_err(LWDP_MODULE_LOG, value.c_str(), GETWORD(tmpList[1].value).c_str(), 
													   GETWORD(tmpList[2].value).c_str(),
													   GETWORD(tmpList[3].value).c_str(),
													   GETWORD(tmpList[4].value).c_str(),
													   GETWORD(tmpList[5].value).c_str());
	}

	return LWDP_OK;
}




EXTERN_C_END;
LWDP_NAMESPACE_END;



