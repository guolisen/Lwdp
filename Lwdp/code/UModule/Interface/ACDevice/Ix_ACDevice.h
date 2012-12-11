#ifndef ACDEVICE_INTERFACE_H
#define ACDEVICE_INTERFACE_H

#include <Interface/Ix_Object.h>
#include "../Interface/ACDevice/Id_ACDevice.h"

namespace NLwdp {

INTERFACE_BEGIN(ACDevice)
    virtual void Foo(long_& value) = 0;
INTERFACE_END()

}
#endif // TCPSERVER_INTERFACE_H
