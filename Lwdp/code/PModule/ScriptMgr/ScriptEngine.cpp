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

ScriptEngine::ScriptEngine()
{
	mScriptContext.clear();

	mSysActionSet.push_back(SysAction_Ptr(new AVar));
	mSysActionSet.push_back(SysAction_Ptr(new AIfElse));
	mSysActionSet.push_back(SysAction_Ptr(new AElse));
	mSysActionSet.push_back(SysAction_Ptr(new AComput));	
	mSysActionSet.push_back(SysAction_Ptr(new ADec));
	mSysActionSet.push_back(SysAction_Ptr(new AInc));
	mSysActionSet.push_back(SysAction_Ptr(new AWhile));
	mSysActionSet.push_back(SysAction_Ptr(new ABreak));
	mSysActionSet.push_back(SysAction_Ptr(new AContinue));
};

LWRESULT ScriptEngine::RunScript(const CODE_BUF& script_code, const tstring& domain, tstring& ret_value)
{
	return scriptRunTag(script_code, domain, ret_value);
}

LWRESULT ScriptEngine::setVarValue(const tstring& var_name, const tstring& var_value)
{
	if(LW_SCRIPT_VAR_TAG == var_name[0])
	{
		LWRESULT stat = LWDP_ERROR;
		Var_List::iterator iter; 
		FOREACH_STL(iter, mScriptContext)
		{
			if(var_name == iter->varName)
			{
				ExprParse ep;
				EP::WORDTYPE type = EP::NOTYPE;
				iter->varValue = ep.whichType(var_value, iter->varType);

				if(EP::VAR == type)
				{
					RINOK(getVarValueRecursion(var_value, iter->varValue, iter->varType));
				}

				return LWDP_OK;
			}
		}
	}

	return LWDP_SCRIPT_ACTION_IS_NOT_VAR_ERROR;
}

LWRESULT ScriptEngine::addVarValue(const tstring& var_name, const tstring& var_value)
{
	if(LW_SCRIPT_VAR_TAG == var_name[0])
	{
		Var_List::iterator iter; 
		FOREACH_STL(iter, mScriptContext)
		{
			if(var_name == iter->varName)
				return LWDP_SCRIPT_ACTION_VAR_EXIST_ERROR;
		}

		VarEntry tmpVar;
		tmpVar.varName  = var_name;
		ExprParse ep;
		tmpVar.varValue = ep.whichType(var_value, tmpVar.varType);

		if(EP::VAR == tmpVar.varType)
		{
			RINOK(getVarValueRecursion(var_value, tmpVar.varValue, tmpVar.varType));
		}
		
		mScriptContext.push_back(tmpVar);
		return LWDP_OK;
	}

	return LWDP_SCRIPT_ACTION_IS_NOT_VAR_ERROR;
}

LWRESULT ScriptEngine::getVarValue(const tstring& var_name, tstring& var_value, EP::WORDTYPE& var_type)
{
	if(LW_SCRIPT_VAR_TAG == var_name[0])
	{
		Var_List::iterator iter; 
		FOREACH_STL(iter, mScriptContext)
		{
			if(var_name == iter->varName)
			{
				var_value = iter->varValue;
				var_type  = iter->varType; 
				return LWDP_OK;
			}
		}
	}

	return LWDP_SCRIPT_ACTION_IS_NOT_VAR_ERROR;
}

LWRESULT ScriptEngine::getVarValueRecursion(const tstring& var_name, tstring& var_value, EP::WORDTYPE& var_type)
{
	if(LW_SCRIPT_VAR_TAG == var_name[0])
	{
		LWRESULT stat = LWDP_ERROR;
		Var_List::iterator iter; 
		FOREACH_STL(iter, mScriptContext)
		{
			if(var_name == iter->varName)
			{
				ExprParse ep;
				tstring IsVarName = ep.IsVar(iter->varValue);
				if(!IsVarName.empty())
				{
					RINOK(getVarValueRecursion(IsVarName, iter->varValue, iter->varType));
				}
				else
				{
					var_value = iter->varValue;
					var_type  = iter->varType; 
				}
				
				return LWDP_OK;
			}
		}
	}

	return LWDP_SCRIPT_ACTION_IS_NOT_VAR_ERROR;
}

LWRESULT ScriptEngine::scriptRunTag(const TagTree_Ptr& script_code, const tstring& domain, tstring& ret_value)
{
	if(script_code == TagTree_Ptr())
		return LWDP_OK;

	TreePtrList::iterator iter;
	tstring  actionName("");
	tstring  retStr("");
	LWRESULT stat = LWDP_ERROR;
	FOREACH_STL(iter, script_code->subTree)
	{
		actionName = "";
		retStr = "";

		if((*iter)->levelEntry == TagEntry_Ptr())
			break;  //Done

		SysAction_Set::iterator iterSysAct;
		FOREACH_STL(iterSysAct, mSysActionSet)
		{
			(*iterSysAct)->GetActionName(actionName);
			if((*iter)->levelEntry->propertyName == actionName)
			{			
				stat = (*iterSysAct)->DoAction((*iter)->levelEntry, domain, (*iter), retStr, this);
				if(stat != LWDP_OK)
				{
					ret_value = retStr;
					return stat;
				}
				break;
			}
		}

		Param pa;
		ParamList paramList;
		TagEntry_Ptr tmpTagEntry = (*iter)->levelEntry;		
		uint32_ attrNum = tmpTagEntry->GetAttrCount();

		if(iterSysAct == mSysActionSet.end())
		{
			GET_OBJECT_RET(ScriptMgr, iScript, LWDP_GET_OBJECT_ERROR);
			Action_Ptr tmpAction;
			iScript->GetAction((*iter)->levelEntry->propertyName, tmpAction);
			if(tmpAction == Action_Ptr())
			{
				ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_SCRIPT_FIND_ACTION_ERROR, false, 
	 							"Can Not Find the Action(%s)", 
	 							(*iter)->levelEntry->propertyName);
				return LWDP_SCRIPT_FIND_ACTION_ERROR;
			}
		
			for(uint32_ i = 0; i < attrNum; ++i)
			{
				CAttributeStru tmpParam = (*tmpTagEntry)[i];
				if(static_cast<tstring>(LW_SCRIPT_RET_TAG) == tmpParam.AttribName)
				{
					continue;
				}
				
				pa.name  = tmpParam.AttribName;
				if(LW_SCRIPT_VAR_TAG == tmpParam.AttribValue[0])
				{
					RINOK(getVarValueRecursion(tmpParam.AttribValue, pa.value, pa.type));
				}
				else
				{
					ExprParse ep;
					pa.value = ep.whichType(tmpParam.AttribValue, pa.type);
				}
				paramList.push_back(pa);

			}
			
			stat = tmpAction->DoAction(paramList, tmpTagEntry->propertyText, retStr);
			if(stat != LWDP_OK)
			{
				ret_value = retStr;
				return stat;
			}
		}

		CAttributeStru retAttr;
		for(uint32_ i = 0; i < attrNum; ++i)
		{
			CAttributeStru tmpParam = (*tmpTagEntry)[i];
			if(static_cast<tstring>(LW_SCRIPT_RET_TAG) == tmpParam.AttribName)
			{
				retAttr = tmpParam;
				break;
			}
		}
		if(!retAttr.AttribName.empty())
		{
			if(LW_SCRIPT_VAR_TAG == retAttr.AttribValue[0])
			{
				RINOK(setVarValue(retAttr.AttribValue, retStr));
			}
		}
	}

	ret_value = retStr;
	return LWDP_OK;
}

LWRESULT AVar::DoAction(const TagEntry_Ptr& element, const tstring& domain, const CODE_BUF& sub_code, tstring& ret_str, ScriptEngine* pSEngine)
{
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_PARAMETER_ERROR, pSEngine, "AVar::DoAction Parameter Error!");

	if((*element)[0].AttribName != LW_ACTION_VARS_PARA_NAME)
		return LWDP_SCRIPT_ACTION_VAR_NAME_ERROR;

	return pSEngine->addVarValue((*element)[0].AttribValue, element->propertyText);
}

tstring  ExprParse::whichType(const tstring& word_str, EP::WORDTYPE& type)
{
	tstring retStr = IsExpr(word_str);
	if(!retStr.empty())
	{
		type = EP::EXPRESSION;
		return retStr;
	}

	retStr = IsFigure(word_str);
	if(!retStr.empty())
	{
		type = EP::FIGURE;
		return retStr;
	}
	retStr = IsVar(word_str);
	if(!retStr.empty())
	{
		type = EP::VAR;
		return retStr;
	}
	retStr = IsString(word_str);
	if(!retStr.empty())
	{
		type = EP::STRING;
		return retStr;
	}

	type= EP::NOTYPE;
	return word_str;
}

LWRESULT ExprParse::ParseStr(const tstring& expr_str, ScriptEngine* pSEngine, tstring& ret_str)
{
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_PARAMETER_ERROR, pSEngine, "ExprParse::parseStr Parameter Error!");
	ExprParse ep;
	EP::WORDTYPE varType = EP::NOTYPE;
	tstring tmpStr = ep.whichType(expr_str, varType);
	if(varType != EP::EXPRESSION)
		return LWDP_SCRIPT_ACTION_VALUE_TYPE_ERROR;

	tstring::size_type atPos = 0;
	tstring::size_type atEndPos = 0;
	//tmpStr = tmpStr.substr(1); //È¥µô$
	while(1)
	{
		atPos = tmpStr.find(LW_SCRIPT_VAR_TAG, atPos);
		if(atPos != tstring::npos)
		{	
			for(atEndPos=atPos+1; atEndPos<tmpStr.size(); ++atEndPos)
			{
				if(!Api_isalnum(tmpStr[atEndPos]))
					break;
			}
		
			tstring varValue = "";
			EP::WORDTYPE varType = EP::NOTYPE;
			RINOK(pSEngine->getVarValueRecursion(tmpStr.substr(atPos, (atEndPos-atPos)), varValue, varType));
			
			tmpStr.replace(atPos, atEndPos-atPos, GETWORD(varValue));

			atPos += 1;
			continue;
		}
		break;
	}

	atPos = 0;
	atEndPos = 0;
	while(1)
	{
		atPos = tmpStr.find(LW_SCRIPT_FIGURE_TAG, atPos);
		if(atPos != tstring::npos)
		{	
			for(atEndPos=atPos+1; atEndPos<tmpStr.size(); ++atEndPos)
			{
				if(!Api_isdigit(tmpStr[atEndPos]))
					break;
			}
		
			tstring varValue = "";
			varValue = IsFigure(tmpStr.substr(atPos, atEndPos-atPos)); 
			if(varValue.empty())
				return LWDP_SCRIPT_ACTION_VALUE_TYPE_ERROR;
			
			tmpStr.erase(atPos,1);

			atPos += 1;
			continue;
		}
		break;
	}
	
	ret_str = tmpStr;
	return LWDP_OK;
}


static bool isDigitStr(const tstring& num)
{
	for (uint32_ i = 0; i<num.size(); ++i)
	{
		if(!Api_isdigit(num[i]))
			return false;
	}

	return true;
}

LWRESULT ExprParse::ParseBool(const tstring& expr_str, bool& ret_bool, ScriptEngine* pSEngine)
{
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_PARAMETER_ERROR, pSEngine, "ExprParse::ParseBool Parameter Error!");

	tstring ret_expr = "";
	RINOK(ParseStr(expr_str, pSEngine, ret_expr));

	GETWORD(ret_expr);
	GET_OBJECT_RET(XmlMgr, iXmlMgr, LWDP_GET_OBJECT_ERROR);
	iXmlMgr->XmlParse("<Root/>");
	tstring res;
	iXmlMgr->XmlParseExprStr(ret_expr, res);

	if(res == __T("true"))
	{
		ret_bool = true;
	}
	else if(res == __T("false"))
	{
		ret_bool = false;
	}		
	else if(isDigitStr(res))
	{
		(__T("0") == res)? ret_bool=false: ret_bool=true;
	}
	else
	{
		return LWDP_SCRIPT_ACTION_EXPR_ERROR;
	}	
	
	return LWDP_OK;
}

tstring  ExprParse::DelBlank(const tstring& word_str)
{
	tstring tmpStr = word_str;
	tstring::size_type blankPos = tmpStr.find_first_not_of(__T(" "));
	if(blankPos != 0)
	{
		tmpStr.erase(0, blankPos);
	}

	blankPos = tmpStr.find_last_not_of(__T(" "));
	if(blankPos != tmpStr.size() - 1)
	{
		tmpStr.erase(blankPos+1, ((tmpStr.size() - 1) - blankPos));
	}

	return tmpStr;
}


tstring  ExprParse::IsString(const tstring& word_str)
{
	tstring tmpStr = DelBlank(word_str);
	if((tmpStr[0] == LW_SCRIPT_STR_TAG) && (tmpStr[tmpStr.size()-1] == LW_SCRIPT_STR_TAG))
		return tmpStr;

	return static_cast<tstring>("");
}

tstring  ExprParse::IsExpr(const tstring& word_str)
{
	tstring tmpStr = DelBlank(word_str);
	if(tmpStr[0] == LW_SCRIPT_EXPR_TAG)
		return tmpStr;

	return static_cast<tstring>("");
}

tstring  ExprParse::IsFigure(const tstring& word_str)
{
	tstring tmpStr = DelBlank(word_str);
	if(tmpStr[0] == LW_SCRIPT_FIGURE_TAG)
		return tmpStr;

	return static_cast<tstring>("");
}


tstring  ExprParse::IsVar(const tstring& word_str)
{
	tstring tmpStr = DelBlank(word_str);
	if(tmpStr[0] == LW_SCRIPT_VAR_TAG)
		return tmpStr;

	return static_cast<tstring>("");
}

LWRESULT AIfElse::DoAction(const TagEntry_Ptr& element, const tstring& domain, const CODE_BUF& sub_code, tstring& ret_str, ScriptEngine* pSEngine)
{
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_PARAMETER_ERROR, pSEngine, "AVar::DoAction Parameter Error!");

	if((*element)[0].AttribName != LW_ACTION_IFELSE_PARA_EXPR)
		return LWDP_SCRIPT_ACTION_IFELSE_ATTR_ERROR;

	bool res_bool = false;
	RINOK(ParseBool((*element)[0].AttribValue, res_bool, pSEngine));

	LWRESULT stat = LWDP_ERROR;
	if(res_bool)
	{
		stat = pSEngine->RunScript(sub_code, domain + "::" + LW_DOMAIN_IF_TAG, ret_str);
		if(stat != LWDP_OK)
			return LWDP_SCRIPT_ACTION_IF_RUN_ERROR;
	}
	else
	{
		TreePtrList::iterator it;
		FOREACH_STL(it, sub_code->subTree)
		{
			if((*it)->levelEntry->propertyName == LW_ACTION_ELSE_TAG)
				break;
		}

		if(it != sub_code->subTree.end())
		{
			stat = pSEngine->RunScript((*it), domain + "::" + LW_DOMAIN_ELSE_TAG, ret_str);
			if(stat != LWDP_OK)
				return LWDP_SCRIPT_ACTION_IF_RUN_ERROR;
		}
	}

	return LWDP_OK;
}

LWRESULT AComput::getExpr(const tstring& expr_str, tstring& ret_str, ScriptEngine* pSEngine)
{
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_PARAMETER_ERROR, pSEngine, "AComput::getExpr Parameter Error!");
	ExprParse ep;
	EP::WORDTYPE varType = EP::NOTYPE;
	tstring varValue = "";
	tstring varText  = ep.whichType(expr_str, varType);

	if(varType == EP::VAR)
	{
		RINOK(pSEngine->getVarValueRecursion(varText, varValue, varType));
		if(varType == EP::EXPRESSION)
			return getExpr(varValue, ret_str, pSEngine);
	}
	else if(varType == EP::EXPRESSION)
	{
		RINOK(ep.ParseStr(expr_str, pSEngine, varValue));
	}
	else if(varType == EP::NOTYPE)
	{
		varValue = varText;
	}
	else
	{
		return LWDP_SCRIPT_ACTION_VALUE_TYPE_ERROR;
	}

	ret_str = varValue;

	return LWDP_OK;
}

LWRESULT AComput::DoAction(const TagEntry_Ptr& element, const tstring& domain, const CODE_BUF& sub_code, tstring& ret_str, ScriptEngine* pSEngine)
{
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_PARAMETER_ERROR, pSEngine, "AComput::DoAction Parameter Error!");

	ExprParse ep;
	EP::WORDTYPE varType = EP::NOTYPE;
	tstring varValue = "";
	tstring varText  = ep.whichType(element->propertyText, varType);

	RINOK(getExpr(varText, varValue, pSEngine));
	
	tstring stepStr = getStepStr();
	if(!(*element)[LW_ACTION_COMPUT_PARA_STEP].empty())
	{
		tstring tmpAttrVal = (*element)[LW_ACTION_COMPUT_PARA_STEP];
		stepStr = ep.whichType(tmpAttrVal, varType);
		if(EP::VAR == varType)
		{
			RINOK(pSEngine->getVarValueRecursion(tmpAttrVal, stepStr, varType));
			if(varType != EP::FIGURE)
				return LWDP_SCRIPT_ACTION_VALUE_TYPE_ERROR;
		}
	}
	
	GETWORD(stepStr);
	GETWORD(varValue);
	tstring exprStr = varValue + getOprStr() + stepStr;

	GET_OBJECT_RET(XmlMgr, iXmlMgr, LWDP_GET_OBJECT_ERROR);
	iXmlMgr->XmlParse("<Root/>");
	tstring res;
	iXmlMgr->XmlParseExprStr(exprStr, res);

	tstring figureStr = (static_cast<tstring>(__T(":")) + res);
	if(!(*element)[LW_SCRIPT_RET_TAG].empty())
	{
		ret_str = figureStr;
	}
	else
	{
		varText = ep.whichType(varText, varType);
		if(varType != EP::VAR)
			return LWDP_OK;
		RINOK(pSEngine->setVarValue(varText, figureStr));		
	}

	return LWDP_OK;
}


LWRESULT AWhile::DoAction(const TagEntry_Ptr& element, const tstring& domain, const CODE_BUF& sub_code, tstring& ret_str, ScriptEngine* pSEngine)
{
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_PARAMETER_ERROR, pSEngine, "AWhile::DoAction Parameter Error!");

	if((*element)[0].AttribName != LW_ACTION_WHILE_EXPR_TAG)
		return LWDP_SCRIPT_ACTION_WHILE_ATTR_ERROR;

	ExprParse ep;
	bool res_bool = false;
	LWRESULT stat = LWDP_ERROR;
	while(1)
	{
		RINOK(ep.ParseBool((*element)[0].AttribValue, res_bool, pSEngine));
		if(!res_bool)
		{
			break;
		}
		stat = pSEngine->RunScript(sub_code, domain + "::" + LW_DOMAIN_WHILE_TAG, ret_str);
		if(stat != LWDP_OK)
		{
			if(ret_str == LW_ACTION_BREAK_RET_TAG)
			{
				break;
			}
			else if(ret_str == LW_ACTION_CONTINUE_RET_TAG)
			{
				continue;
			}
			else
			{
				return LWDP_SCRIPT_ACTION_WHILE_RUN_ERROR;
			}
		}
	}

	return LWDP_OK;
}

EXTERN_C_END;
LWDP_NAMESPACE_END;




