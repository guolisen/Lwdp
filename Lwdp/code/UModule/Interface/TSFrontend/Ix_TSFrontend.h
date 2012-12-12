#ifndef TSFRONTEND_INTERFACE_H
#define TSFRONTEND_INTERFACE_H

#include <Interface/Ix_Object.h>
#include "Id_TSFrontend.h"

namespace NLwdp {

INTERFACE_BEGIN(TSFrontend)
	virtual LWRESULT Init() = 0;
	virtual LWRESULT RunServer() = 0;
	virtual LWRESULT DestoryServer() = 0;
INTERFACE_END()

}
#endif // TCPSERVER_INTERFACE_H
