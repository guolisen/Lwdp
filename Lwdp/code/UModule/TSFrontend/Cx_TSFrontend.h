#ifndef CX_TSFRONTEND_H
#define CX_TSFRONTEND_H
//#pragma once

using namespace NLwdp;
#include "../Interface/TSFrontend/Ix_TSFrontend.h"


class Cx_TSFrontend: public Ix_TSFrontend
{
    X3BEGIN_CLASS_DECLARE(Cx_TSFrontend)
        X3DEFINE_INTERFACE_ENTRY(Ix_TSFrontend)
    X3END_CLASS_DECLARE()
protected:
	Cx_TSFrontend();
	virtual ~Cx_TSFrontend();

private:
    // From Ix_TSFrontend
    virtual void Foo(long_& value);

};

#endif

