#ifndef COMMONUTILMGR_INTERFACE_H
#define COMMONUTILMGR_INTERFACE_H

#include <Interface/Ix_Object.h>
#include "Id_CommonUtilMgr.h"

LWDP_NAMESPACE_BEGIN;

typedef void (*PROCESS_FUNC_PAIR)();
 
struct CallBackStru
{
	PROCESS_FUNC_PAIR createFunc;
	PROCESS_FUNC_PAIR destoryFunc;

	CallBackStru(PROCESS_FUNC_PAIR create, PROCESS_FUNC_PAIR destory): 
		            createFunc(create),destoryFunc(destory)
	{

	};

	CallBackStru(const CallBackStru& other)
	{
		createFunc  = other.createFunc;
		destoryFunc = other.destoryFunc;
	};

};

class ProcessController
{
public:
	ProcessController(CallBackStru call_back): mCb(call_back)
	{
		if(mCb.createFunc)
			mCb.createFunc();
	}
	virtual ~ProcessController()
	{
		if(mCb.destoryFunc)
			mCb.destoryFunc();
	}

	CallBackStru mCb;
};




INTERFACE_BEGIN(CommonUtilMgr)
	virtual LWRESULT Init() = 0;
	
INTERFACE_END();


LWDP_NAMESPACE_END;

#endif // LOGMGR_INTERFACE_H
