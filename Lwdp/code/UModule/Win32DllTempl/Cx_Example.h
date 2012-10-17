//#pragma once

using namespace NLwdp;
#include "../Interface/Example/Ix_Example.h"

class Cx_Example
    : public Ix_Example
    , public Ix_Example2
{
    X3BEGIN_CLASS_DECLARE(Cx_Example)
        X3DEFINE_INTERFACE_ENTRY(Ix_Example)
        X3DEFINE_INTERFACE_ENTRY(Ix_Example2)
    X3END_CLASS_DECLARE()
protected:
	Cx_Example();
	virtual ~Cx_Example();

private:
    // From Ix_Example
    virtual void Foo(long_& value);

    // From Ix_Example2
    virtual void Foo2(long_& value);
};
