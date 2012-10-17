//#pragma once

using namespace NLwdp;
#include "../Interface/Example/SubExampleB/Ix_ExampleB.h"

class Cx_ExampleB
    : public Ix_ExampleB
{
    X3BEGIN_CLASS_DECLARE(Cx_ExampleB)
        X3DEFINE_INTERFACE_ENTRY(Ix_ExampleB)
    X3END_CLASS_DECLARE()
protected:
	Cx_ExampleB();
	virtual ~Cx_ExampleB();

private:
    // From Ix_Example
    virtual void FooB(long_& value);
};
