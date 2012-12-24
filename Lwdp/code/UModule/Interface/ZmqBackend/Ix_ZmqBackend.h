#ifndef ZMQ_BACKEND_INTERFACE_H
#define ZMQ_BACKEND_INTERFACE_H

#include <Interface/Ix_Object.h>
#include "Id_ZmqBackend.h"
#include <LwApiLib/ComLib/FastDelegate/FastDelegate.h>
#include <boost/shared_ptr.hpp>

using namespace fastdelegate;

namespace NLwdp {
	
typedef boost::shared_ptr<uint8_>  Data_Ptr;
typedef FastDelegate4<const uint8_*, uint32_, Data_Ptr&, uint32_&, LWRESULT> MsgDelegate; 

INTERFACE_BEGIN(ZmqBackend)
	virtual LWRESULT Init() = 0;
	virtual LWRESULT RunServer() = 0;
	virtual LWRESULT DestoryServer() = 0;
	virtual LWRESULT RegisteZmqMsg(uint32_ msg_code, MsgDelegate msg_delegate) = 0;
	virtual LWRESULT CallBackZmqMsg(const uint8_* recv_msg, uint32_ recv_msg_len, 
								    Data_Ptr& ret_data ,uint32_& ret_data_len) = 0;	
INTERFACE_END()

}
#endif // TCPSERVER_INTERFACE_H
