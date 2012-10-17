/*! \file ScriptEngine.h
 *  \brief Define Script run
 *  \author Guolisen, Lwdp
 *  \date   2011.11.04
 */
#ifndef _SCRIPT_ENGINE_H
#define _SCRIPT_ENGINE_H

#include <Interface/ScriptMgr/Ix_ScriptMgr.h>
#include <boost/shared_ptr.hpp>
#include "ScriptTagDef.h"


LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;

struct VarEntry{
	tstring varName;
	tstring varValue;
	EP::WORDTYPE varType;
	
	VarEntry():varName(""), varValue(""), varType(EP::NOTYPE){}
};

typedef std::list<VarEntry> Var_List;

class ScriptEngine;
class Ix_SysAction
{
public:
	Ix_SysAction(){}
	virtual ~Ix_SysAction(){}

	virtual LWRESULT GetActionName(tstring& action_name) = 0;
	virtual LWRESULT DoAction(const TagEntry_Ptr& element, const tstring& domain, const CODE_BUF& sub_code, tstring& ret_str, ScriptEngine* pSEngine) = 0;
};


typedef  boost::shared_ptr<Ix_SysAction> SysAction_Ptr;
typedef  std::list<SysAction_Ptr> SysAction_Set;

class ExprParse
{
public:
	ExprParse(){}
	virtual ~ExprParse(){}
	virtual LWRESULT ParseBool(const tstring& expr_str, bool& ret_bool, ScriptEngine* pSEngine);
	virtual LWRESULT ParseStr(const tstring& expr_str, ScriptEngine* pSEngine, tstring& ret_str);

	virtual tstring  whichType(const tstring& word_str, EP::WORDTYPE& type);
	virtual tstring  DelBlank(const tstring& word_str);
	virtual tstring  IsString(const tstring& word_str);
	virtual tstring  IsFigure(const tstring& word_str);
	virtual tstring  IsVar(const tstring& word_str);
	virtual tstring  IsExpr(const tstring& word_str);
};


class ScriptEngine
{
public:
    ScriptEngine();
    virtual ~ScriptEngine(){};

	virtual LWRESULT RunScript(const CODE_BUF& script_code, const tstring& domain, tstring& ret_value);

public:
	virtual LWRESULT scriptRunTag(const TagTree_Ptr& script_code, const tstring& domain, tstring& ret_value);
	virtual LWRESULT getVarValue(const tstring& var_name, tstring& var_value, EP::WORDTYPE& var_type);
	virtual LWRESULT getVarValueRecursion(const tstring& var_name, tstring& var_value, EP::WORDTYPE& var_type);
	virtual LWRESULT setVarValue(const tstring& var_name, const tstring& var_value);
	virtual LWRESULT addVarValue(const tstring& var_name, const tstring& var_value);


protected:
	Var_List 		mScriptContext;
	SysAction_Set	mSysActionSet;
};


class AVar : public Ix_SysAction
{
public:
	AVar():mActionName(LW_ACTION_VARS_TAG){}
	virtual ~AVar(){}

	virtual LWRESULT GetActionName(tstring& action_name){action_name = mActionName; return LWDP_OK;};
	virtual LWRESULT DoAction(const TagEntry_Ptr& element, const tstring& domain, const CODE_BUF& sub_code, tstring& ret_str, ScriptEngine* pSEngine);

protected:
	tstring mActionName;
};


class AIfElse : public Ix_SysAction, 
				   public ExprParse
{
public:
	AIfElse():mActionName(LW_ACTION_IFELSE_TAG){}
	virtual ~AIfElse(){}

	virtual LWRESULT GetActionName(tstring& action_name){action_name = mActionName; return LWDP_OK;};
	virtual LWRESULT DoAction(const TagEntry_Ptr& element, const tstring& domain, const CODE_BUF& sub_code, tstring& ret_str, ScriptEngine* pSEngine);

protected:
	tstring mActionName;
};

class AElse : public Ix_SysAction
{
public:
	AElse():mActionName(LW_ACTION_ELSE_TAG){}
	virtual ~AElse(){}

	virtual LWRESULT GetActionName(tstring& action_name){action_name = mActionName; return LWDP_OK;};
	virtual LWRESULT DoAction(const TagEntry_Ptr& element, const tstring& domain, const CODE_BUF& sub_code, tstring& ret_str, ScriptEngine* pSEngine){return LWDP_OK;};

protected:
	tstring mActionName;
};

class AComput : public Ix_SysAction
{
public:
	AComput():mActionName(LW_ACTION_COMPUT_TAG){}
	AComput(const tstring& action_name):mActionName(action_name){}
	virtual ~AComput(){}

	virtual LWRESULT GetActionName(tstring& action_name){action_name = mActionName; return LWDP_OK;};
	virtual LWRESULT DoAction(const TagEntry_Ptr& element, const tstring& domain, const CODE_BUF& sub_code, tstring& ret_str, ScriptEngine* pSEngine);

protected:
	virtual LWRESULT getExpr(const tstring& expr_str, tstring& ret_str, ScriptEngine* pSEngine);
	virtual const tstring getOprStr(){return static_cast<tstring>(__T(""));};	
	virtual const tstring getStepStr(){return static_cast<tstring>(__T(""));};	

protected:
	tstring mActionName;
};


class ADec : public AComput
{
public:
	ADec():AComput(LW_ACTION_DEC_TAG){}
	virtual ~ADec(){}

protected:
	virtual const tstring getOprStr(){return static_cast<tstring>(__T("-"));};	
	virtual const tstring getStepStr(){return static_cast<tstring>(__T("1"));};	

};

class AInc : public AComput
{
public:
	AInc():AComput(LW_ACTION_INC_TAG){}
	virtual ~AInc(){}

protected:
	virtual const tstring getOprStr(){return static_cast<tstring>("+");};	
	virtual const tstring getStepStr(){return static_cast<tstring>(__T("1"));};	
};


class AWhile : public Ix_SysAction
{
public:
	AWhile():mActionName(LW_ACTION_WHILE_TAG){}
	virtual ~AWhile(){}

	virtual LWRESULT GetActionName(tstring& action_name){action_name = mActionName; return LWDP_OK;};
	virtual LWRESULT DoAction(const TagEntry_Ptr& element, const tstring& domain, const CODE_BUF& sub_code, tstring& ret_str, ScriptEngine* pSEngine);

protected:
	tstring mActionName;
};

class ABreak : public Ix_SysAction
{
public:
	ABreak():mActionName(LW_ACTION_BREAK_TAG){}
	virtual ~ABreak(){}

	virtual LWRESULT GetActionName(tstring& action_name){action_name = mActionName; return LWDP_OK;};
	virtual LWRESULT DoAction(const TagEntry_Ptr& element, const tstring& domain, const CODE_BUF& sub_code, tstring& ret_str, ScriptEngine* pSEngine)
	{
		tstring::size_type pos = domain.find_first_of(LW_DOMAIN_WHILE_TAG);
		if(pos == tstring::npos)
			return LWDP_OK;
		ret_str = LW_ACTION_BREAK_RET_TAG;
		return LWDP_ACTION_BREAK_RET;
	};

protected:
	tstring mActionName;
};

class AContinue : public Ix_SysAction
{
public:
	AContinue():mActionName(LW_ACTION_CONTINUE_TAG){}
	virtual ~AContinue(){}

	virtual LWRESULT GetActionName(tstring& action_name){action_name = mActionName; return LWDP_OK;};
	virtual LWRESULT DoAction(const TagEntry_Ptr& element, const tstring& domain, const CODE_BUF& sub_code, tstring& ret_str, ScriptEngine* pSEngine)
	{
		tstring::size_type pos = domain.find_first_of(LW_DOMAIN_WHILE_TAG);
		if(pos == tstring::npos)
			return LWDP_OK;

		ret_str = LW_ACTION_CONTINUE_RET_TAG;
		return LWDP_ACTION_CONTINUE_RET;
	};

protected:
	tstring mActionName;
};


EXTERN_C_END;

static tstring GETWORD(tstring& var) 
{ 
	ExprParse __ep; 
	EP::WORDTYPE __type = EP::NOTYPE; 
	__ep.whichType((var), __type); 	
	if(__type == EP::STRING) 
	{ 
		return ((var)=(var).substr(1, var.size()-2)); 
	} 
	if(__type != EP::NOTYPE) 
	{ 
		return ((var)=(var).substr(1)); 
	} 

	return static_cast<tstring>(var);
}


LWDP_NAMESPACE_END;

#endif // _SCRIPT_ENGINE_H


