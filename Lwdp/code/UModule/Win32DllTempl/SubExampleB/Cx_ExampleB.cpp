#include <Lwdp.h>
#include <PluginInc.h>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>

#include "Cx_ExampleB.h"

Cx_ExampleB::Cx_ExampleB()
{
	printf("ExampleB Create!\n");
}

Cx_ExampleB::~Cx_ExampleB()
{
	printf("ExampleB Delete!\n");
}

void Cx_ExampleB::FooB(long_& value)
{
    //value = 1;
	Cx_Interface<Ix_ConfigMgr> iConfigMgr(CLSID_ConfigMgr);

	XPropertys prop;
	iConfigMgr->GetModulePropEntry("ExampleTest", "Version", prop);
	printf("ExampleB Foo P Value: %s\n", prop[0].propertyText.c_str());

	printf("ExampleB Foo Value: %d\n", value);
}


