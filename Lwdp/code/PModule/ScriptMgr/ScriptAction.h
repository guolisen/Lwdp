/*! \file ScriptAction.h
 *  \brief Define Config Manager 
 *  \author Guolisen, Lwdp
 *  \date   2011.11.04
 */
#ifndef _SCRIPT_ACTION_H
#define _SCRIPT_ACTION_H

#include <Interface/ScriptMgr/Ix_ScriptMgr.h>
#include <boost/shared_ptr.hpp>
#include "ScriptTagDef.h"

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;

 
class APrint : public Ix_Action
{
public:
	APrint():mActionName(LW_ACTION_PRINT_TAG){}
	virtual ~APrint(){}

	virtual LWRESULT GetActionName(tstring& action_name){action_name = mActionName; return LWDP_OK;};
	virtual LWRESULT DoAction(const ParamList& param_list, const tstring& value, tstring& ret_str);


protected:
	tstring mActionName;
};


EXTERN_C_END;
LWDP_NAMESPACE_END;

#endif // CX_SCRIPT_MANAGER_H


