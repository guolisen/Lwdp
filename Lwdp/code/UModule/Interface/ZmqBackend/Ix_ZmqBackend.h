#ifndef ZMQ_BACKEND_INTERFACE_H
#define ZMQ_BACKEND_INTERFACE_H

#include <Interface/Ix_Object.h>
#include "../Interface/ZmqBackend/Id_ZmqBackend.h"
#include <LwApiLib/ComLib/FastDelegate/FastDelegate.h>

using namespace fastdelegate;

namespace NLwdp {

typedef FastDelegate2<int32_, std::string, std::string> MsgDelegate; 

INTERFACE_BEGIN(ZmqBackend)
	virtual LWRESULT Init() = 0;
	virtual LWRESULT RunServer() = 0;
	virtual LWRESULT DestoryServer() = 0;
	virtual LWRESULT RegisteZmqMsg(uint32_ msg_code, MsgDelegate& msg_delegate) = 0;
	virtual LWRESULT CallBackZmqMsg(const std::string& recv_msg, std::string& ret_data) = 0;	
INTERFACE_END()

}
#endif // TCPSERVER_INTERFACE_H
