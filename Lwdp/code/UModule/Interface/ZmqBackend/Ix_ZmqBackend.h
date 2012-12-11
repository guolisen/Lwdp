#ifndef ZMQ_BACKEND_INTERFACE_H
#define ZMQ_BACKEND_INTERFACE_H

#include <Interface/Ix_Object.h>
#include "../Interface/ZmqBackend/Id_ZmqBackend.h"

namespace NLwdp {

INTERFACE_BEGIN(ZmqBackend)
	virtual LWRESULT Init() = 0;
	virtual LWRESULT RunServer() = 0;
INTERFACE_END()

}
#endif // TCPSERVER_INTERFACE_H
