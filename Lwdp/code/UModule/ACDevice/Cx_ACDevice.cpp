#include <Lwdp.h>
#include <PluginInc.h>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>

#include "../Interface/ZmqBackend/Ix_ZmqBackend.h"
#include "../Interface/ACDevice/Ix_ACDevice.h"

#include "../Common/ExternalInterface.h"
#include "../Common/InternalInterface.h"

#include "Cx_ACDevice.h"

Cx_ACDevice::Cx_ACDevice()
{
	printf("ACDevice Create!\n");
}

Cx_ACDevice::~Cx_ACDevice()
{
	printf("ACDevice Delete!\n");
}

LWRESULT Cx_ACDevice::Init()
{
	GET_OBJECT_RET(ZmqBackend, iZmqBackend, LWDP_GET_OBJECT_ERROR);

	MsgDelegate regFun = MakeDelegate(this, &Cx_ACDevice::MsgProcess);
	RINOK(iZmqBackend->RegisteZmqMsg(TS_DEVICE_INIT_REQ_MSG, regFun));
	return LWDP_OK;
}

LWRESULT Cx_ACDevice::MsgProcess(const uint8_* ret_msg, uint32_ ret_msg_len, 
					                 Data_Ptr& send_msg, uint32_& send_msg_len)
{
	//returnMsg.deviceId = zMsg->deviceId;
	//returnMsg.msgCode  = TS_SERVER_UNKNOW_MSG;

	uint8_* tmpstr = (uint8_*)new char[strlen("Hello ACD CallBack Client!") + 1];
	strcpy((char*)tmpstr, "Hello ACD CallBack Client!");
	Data_Ptr tmpData(tmpstr);
	send_msg = tmpData;
	send_msg_len = strlen("Hello ACD CallBack Client!") + 1;

	return LWDP_OK;
}

