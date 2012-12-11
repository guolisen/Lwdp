//#pragma once
#ifndef CX_ACDEVICE_H
#define CX_ACDEVICE_H

using namespace NLwdp;
#include "../Interface/ACDevice/Ix_ACDevice.h"


class Cx_ACDevice: public Ix_ACDevice
{
    X3BEGIN_CLASS_DECLARE(Cx_ACDevice)
        X3DEFINE_INTERFACE_ENTRY(Ix_ACDevice)
    X3END_CLASS_DECLARE()
protected:
	Cx_ACDevice();
	virtual ~Cx_ACDevice();

private:
    // From Ix_ACDevice
    virtual void Foo(long_& value);

};

#endif
