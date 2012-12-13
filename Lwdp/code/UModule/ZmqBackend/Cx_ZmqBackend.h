#ifndef CX_ZMQBACKEND_H
#define CX_ZMQBACKEND_H
//#pragma once

using namespace NLwdp;
#include <Interface/LogMgr/Ix_LogMgr.h>
#include <Interface/ZmqMgr/Ix_ZmqMgr.h>
#include "../Interface/ZmqBackend/Ix_ZmqBackend.h"


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

protected:
	ContextHandle mContext;
	SocketHandle  mFrontend;
	SocketHandle  mBackend;

};

#endif

