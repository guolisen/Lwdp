// Copyright 2008-2011 Guolisen, guolisen@hotmail.com
// 
// 2011.11.10 Create File By Guolisen
#include <LwDp.h>
#include <PluginInc.h>

#include <Interface/ConfigMgr/Ix_XmlMgr.h>
#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/ScriptMgr/Ix_ScriptMgr.h>
#include "Cx_ScriptMgr.h"
#include "ScriptEngine.h"

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;


LWRESULT Cx_ScriptMgr::RegisteAction(Ix_Action* action)
{
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_PARAMETER_ERROR, (action), "Cx_ScriptMgr::RegisteAction Parameter Error!");

	Action_Ptr tmpPtr(action);
	mActionSet.push_back(tmpPtr);

	return LWDP_OK;
}

LWRESULT Cx_ScriptMgr::GetAction(const tstring& action_name, Action_Ptr& action)
{
	tstring tmpStr("");
	Action_Set::iterator iterAct;
	FOREACH_STL(iterAct, mActionSet)
	{
		(*iterAct)->GetActionName(tmpStr);
		if(action_name == tmpStr)
		{			
			action = (*iterAct);
			return LWDP_OK;
		}
	}

	return LWDP_ERROR;
}


LWRESULT Cx_ScriptMgr::SyncScriptRun(Script_Ptr script_code)
{
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_PARAMETER_ERROR, (script_code), "Cx_ScriptMgr::SyncScriptRun Parameter Error!");
	GET_OBJECT(XmlMgr, iXmlMgr, LWDP_GET_OBJECT_ERROR);
	if (iXmlMgr->XmlParse((XML_BUFFER*)script_code.get()) != LWDP_OK)
	{
		return LWDP_PARSE_XML_DATA_ERROR;
	}

	//NX::Ele scriptTag(LW_CONFIG_SCRIPT_TAG);
	NX::EXPR expr = NX::AllEleStar;
	TagTree_Ptr eleTree(new TreeEntry);
	iXmlMgr->XmlGetElementTree(expr, eleTree);

	ScriptEngine* pScriptEngine = new ScriptEngine;
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_MALLOC_MEMORY_ERROR, (pScriptEngine), "Cx_ScriptMgr::SyncScriptRun new ScriptEngine Error!");

	tstring retStr;
	LWRESULT stat = pScriptEngine->RunScript(eleTree, LW_DOMAIN_TOP_TAG, retStr);
	DELETE_SINGLE(pScriptEngine);
	
	return stat;
}


LWRESULT Cx_ScriptMgr::AsyncScriptRun(Script_Ptr script_code)
{
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_PARAMETER_ERROR, (script_code), "Cx_ScriptMgr::AsyncScriptRun Parameter Error!");


	return LWDP_OK;
}



EXTERN_C_END;
LWDP_NAMESPACE_END;



