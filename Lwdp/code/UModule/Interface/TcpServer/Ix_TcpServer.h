#ifndef TCPSERVER_INTERFACE_H
#define TCPSERVER_INTERFACE_H

#include <Interface/Ix_Object.h>
#include "../Interface/TcpServer/Id_TcpServer.h"

namespace NLwdp {

INTERFACE_BEGIN(TcpServer)
    virtual void Foo(long_& value) = 0;
INTERFACE_END()

}
#endif // TCPSERVER_INTERFACE_H
