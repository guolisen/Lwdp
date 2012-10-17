//#pragma once

using namespace NLwdp;
#include "../Interface/Example/SubExampleA/Ix_ExampleA.h"

class Cx_ExampleA
    : public Ix_ExampleA
{
    X3BEGIN_CLASS_DECLARE(Cx_ExampleA)
        X3DEFINE_INTERFACE_ENTRY(Ix_ExampleA)
    X3END_CLASS_DECLARE()
protected:
	Cx_ExampleA();
	virtual ~Cx_ExampleA();

private:
    // From Ix_Example
    virtual void FooA(long_& value);
};
