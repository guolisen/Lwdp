#ifndef CONSOLEMGR_INTERFACE_H
#define CONSOLEMGR_INTERFACE_H

#include <Interface/Ix_Object.h>
#include "Id_ConsoleMgr.h"
#include <LwApiLib/ComLib/FastDelegate/FastDelegate.h>

using namespace fastdelegate;

LWDP_NAMESPACE_BEGIN;

typedef std::list<std::string> COMMAND_LINE;
typedef FastDelegate1<const COMMAND_LINE&, int32_> ConsoleCBDelegate; 

typedef struct tag_command_stru 
{
	ConsoleCBDelegate commandFun;
	std::string commandInfo;

	tag_command_stru()
	{
		//commandFun  = NULL;
		commandInfo = std::string("");
	};
	tag_command_stru(const tag_command_stru& other)
	{
		commandFun  = other.commandFun;
		commandInfo = other.commandInfo;
	};
}COMMAND_STRU;

typedef std::map<std::string, COMMAND_STRU> COMMAND_MAP;  



INTERFACE_BEGIN(ConsoleMgr)
	virtual LWRESULT Init() = 0;
	virtual LWRESULT RunConsole() = 0;

	virtual LWRESULT RegisteCommand(const std::string& command_str, ConsoleCBDelegate command_fun, 
							          const std::string& command_info) = 0;


INTERFACE_END();


LWDP_NAMESPACE_END;

#endif // 
