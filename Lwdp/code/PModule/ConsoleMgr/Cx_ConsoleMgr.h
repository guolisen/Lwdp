/*! \file Cx_ConsoleMgr.h
 *  \brief ConsoleMgr Module Impl
 *  \author Guolisen, LwDp
 *  \date   2012.12.15
 */
 
#ifndef CX_CONSOLE_MANAGER_H
#define CX_CONSOLE_MANAGER_H

#include <boost/shared_ptr.hpp>
#include <list>
#include <Interface/ConsoleMgr/Ix_ConsoleMgr.h>
//#include <Interface/ScriptMgr/Ix_ScriptMgr.h>
#include "ConsoleMgrDef.h"

#include <iostream>


LWDP_NAMESPACE_BEGIN;



class Cx_ConsoleMgr
    : public Ix_ConsoleMgr
{
    X3BEGIN_CLASS_DECLARE(Cx_ConsoleMgr)
        X3DEFINE_INTERFACE_ENTRY(Ix_ConsoleMgr)
    X3END_CLASS_DECLARE()
protected:
	Cx_ConsoleMgr();
	virtual ~Cx_ConsoleMgr();

protected:
	virtual LWRESULT Init();
	virtual LWRESULT RunConsole();
	virtual LWRESULT RegisteCommand(const std::string& command_str, ConsoleCBDelegate command_fun, 
								          const std::string& command_info);

protected:
	virtual LWRESULT CallBackCommand(COMMAND_LINE& command_line);	
	virtual LWRESULT PraseCommandLine(const std::string& command_str, COMMAND_LINE& command_line, 
		                                     const std::string& div_char);

protected:
	virtual int32_ ConsoleListCom(COMMAND_LINE& command_line);
	
protected:
	COMMAND_MAP	mCommandMap;
};


LWDP_NAMESPACE_END;

#endif
