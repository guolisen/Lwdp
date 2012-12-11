#ifndef TSFRONTEND_INTERFACE_H
#define TSFRONTEND_INTERFACE_H

#include <Interface/Ix_Object.h>
#include "../Interface/TSFrontend/Id_TSFrontend.h"

namespace NLwdp {

INTERFACE_BEGIN(TSFrontend)
    virtual void Foo(long_& value) = 0;
INTERFACE_END()

}
#endif // TCPSERVER_INTERFACE_H
