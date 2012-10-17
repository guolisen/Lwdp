#ifndef EXAMPLE_INTERFACE_H
#define EXAMPLE_INTERFACE_H

#include <Interface/Ix_Object.h>
#include "ID_Example.h"

namespace NLwdp {
INTERFACE_BEGIN(ExampleA)
    virtual void FooA(long_& value) = 0;
INTERFACE_END()

}
#endif // EXAMPLE_INTERFACE_H
