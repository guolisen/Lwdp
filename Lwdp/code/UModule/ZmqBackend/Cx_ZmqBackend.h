#ifndef CX_ZMQBACKEND_H
#define CX_ZMQBACKEND_H
//#pragma once

using namespace NLwdp;
#include <boost/shared_ptr.hpp>
#include <Interface/LogMgr/Ix_LogMgr.h>
#include <Interface/ZmqMgr/Ix_ZmqMgr.h>
#include "../Interface/ZmqBackend/Ix_ZmqBackend.h"

typedef std::map<uint32_, MsgDelegate> MSG_DELEGATE_MAP;


class Cx_ZmqBackend: public Ix_ZmqBackend
{
    X3BEGIN_CLASS_DECLARE(Cx_ZmqBackend)
        X3DEFINE_INTERFACE_ENTRY(Ix_ZmqBackend)
    X3END_CLASS_DECLARE()
protected:
	Cx_ZmqBackend();
	virtual ~Cx_ZmqBackend();

private:
    // From Ix_ZmqBackend
	virtual LWRESULT Init();
	virtual LWRESULT RunServer();
	virtual LWRESULT DestoryServer();	

private:
	virtual LWRESULT RegisteZmqMsg(uint32_ msg_code, MsgDelegate msg_delegate);	
	virtual LWRESULT CallBackZmqMsg(const uint8_* recv_msg, uint32_ recv_msg_len, 
										  Data_Ptr& ret_data ,uint32_& ret_data_len);	

public:
	static ContextHandle mContext;
protected:

	SocketHandle  mFrontend;
	SocketHandle  mBackend;
	MSG_DELEGATE_MAP mMsgDelegateMap;
};

#endif

