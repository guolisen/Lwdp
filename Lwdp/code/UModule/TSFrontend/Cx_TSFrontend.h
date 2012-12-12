#ifndef _CX_TSFRONTEND_H
#define _CX_TSFRONTEND_H
//#pragma once

using namespace NLwdp;
#include <Interface/EventMgr/Ix_EventMgr.h>
#include "../Interface/TSFrontend/Ix_TSFrontend.h"


class Cx_TSFrontend: public Ix_TSFrontend
{
    X3BEGIN_CLASS_DECLARE(Cx_TSFrontend)
        X3DEFINE_INTERFACE_ENTRY(Ix_TSFrontend)
    X3END_CLASS_DECLARE();
protected:
	Cx_TSFrontend();
	virtual ~Cx_TSFrontend();

protected:
    // From Ix_TSFrontend
	virtual LWRESULT Init();
	virtual LWRESULT RunServer();
	virtual LWRESULT DestoryServer();

protected:
    virtual void setNonblock(SOCKET fd);
    virtual void setReuseaddr(SOCKET fd);
    virtual void setAddress(const char* ip, int port, struct sockaddr_in* addr);
    virtual std::string addressToString(struct sockaddr_in* addr);
    virtual LWRESULT createTcpServer(int port);

protected:
	WatcherHandle mIoWatcher;
	SOCKET mServerSocket;
};


#endif

