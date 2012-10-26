/*! \file Cx_ScriptMgr.h
 *  \brief Define Config Manager 
 *  \author Guolisen, Lwdp
 *  \date   2011.11.04
 */
#ifndef CX_SCRIPT_MANAGER_H
#define CX_SCRIPT_MANAGER_H

#include <Interface/ScriptMgr/Ix_ScriptMgr.h>
#include <boost/shared_ptr.hpp>
#include "ScriptTagDef.h"

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;


typedef  std::list<Action_Ptr> Action_Set;

class Cx_ScriptMgr : public Ix_ScriptMgr
{
	friend class Ix_Action;

    X3BEGIN_CLASS_DECLARE(Cx_ScriptMgr)
        X3DEFINE_INTERFACE_ENTRY(Ix_ScriptMgr)
    X3END_CLASS_DECLARE();

protected:
	Cx_ScriptMgr(){mActionSet.clear();}
    virtual ~Cx_ScriptMgr(){}

    virtual LWRESULT RegisteAction(Ix_Action* action);

    virtual LWRESULT GetAction(const tstring& action_name, Action_Ptr& action);

    virtual LWRESULT SyncScriptRun(Script_Ptr script_code);

    virtual LWRESULT AsyncScriptRun(Script_Ptr script_code);

protected:
	Action_Set mActionSet;
};

 

EXTERN_C_END;
LWDP_NAMESPACE_END;

#endif // CX_SCRIPT_MANAGER_H

