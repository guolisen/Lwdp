/*! \file Ix_ScriptMgr.h
 *  \brief Define Config Manager 
 *  \author Guolisen, Lwdp
 *  \date   2011.11.04
 */
#ifndef IX_SCRIPT_MANAGER_H
#define IX_SCRIPT_MANAGER_H

#include <list>
#include <Interface/Ix_Object.h>

#include <Interface/ConfigMgr/Ix_XmlMgr.h>
#include <boost/shared_ptr.hpp>
 
#include "Id_ScriptMgr.h"

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;

namespace EP{
enum WORDTYPE
{
	STRING,
	VAR,
	FIGURE,
	EXPRESSION,
	NOTYPE
};
};

struct Param
{
	tstring name;
	tstring value;
	EP::WORDTYPE type;
};

typedef std::vector<Param> ParamList;
typedef TagTree_Ptr  CODE_BUF;
	
class Ix_Action
{
public:
	Ix_Action(){};
	virtual ~Ix_Action(){};
	
	virtual LWRESULT GetActionName(tstring& action_name) = 0;
	virtual LWRESULT DoAction(const ParamList& param_list, const tstring& value, tstring& ret_str) = 0;

};

typedef  boost::shared_ptr<Ix_Action> Action_Ptr;

INTERFACE_BEGIN(ScriptMgr)
	//
    virtual LWRESULT RegisteAction(Ix_Action* action) = 0;

    virtual LWRESULT GetAction(const tstring& action_name, Action_Ptr& action) = 0;

    virtual LWRESULT SyncScriptRun(Script_Ptr script_code) = 0;

    virtual LWRESULT AsyncScriptRun(Script_Ptr script_code) = 0;

	//
INTERFACE_END();



EXTERN_C_END;
LWDP_NAMESPACE_END;

#endif // IX_SCRIPT_MANAGER_H





















