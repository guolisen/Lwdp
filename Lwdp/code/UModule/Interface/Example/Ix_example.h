#ifndef EXAMPLE_INTERFACE_H
#define EXAMPLE_INTERFACE_H

#include <Interface/Ix_Object.h>
#include "ID_Example.h"

namespace NLwdp {
INTERFACE_BEGIN(Example)
    virtual void Foo(long_& value) = 0;
INTERFACE_END()


INTERFACE_BEGIN(Example2)
    virtual void Foo2(long_& value) = 0;
INTERFACE_END()

}
#endif // EXAMPLE_INTERFACE_H
