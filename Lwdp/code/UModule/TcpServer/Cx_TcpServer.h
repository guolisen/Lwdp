#ifndef CX_TCPSERVER_H
#define CX_TCPSERVER_H
//#pragma once

using namespace NLwdp;
#include "../Interface/TcpServer/Ix_TcpServer.h"


class Cx_TcpServer: public Ix_TcpServer
{
    X3BEGIN_CLASS_DECLARE(Cx_TcpServer)
        X3DEFINE_INTERFACE_ENTRY(Ix_TcpServer)
    X3END_CLASS_DECLARE()
protected:
	Cx_TcpServer();
	virtual ~Cx_TcpServer();

private:
    // From Ix_TcpServer
	virtual LWRESULT Init();
	virtual LWRESULT RunServer();
	virtual LWRESULT DestoryServer();
};

#endif

