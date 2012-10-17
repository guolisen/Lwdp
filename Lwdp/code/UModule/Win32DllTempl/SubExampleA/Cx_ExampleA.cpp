#include <Lwdp.h>
#include <PluginInc.h>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>

#include "Cx_ExampleA.h"

Cx_ExampleA::Cx_ExampleA()
{
	printf("ExampleA Create!\n");
}

Cx_ExampleA::~Cx_ExampleA()
{
	printf("ExampleA Delete!\n");
}

void Cx_ExampleA::FooA(long_& value)
{
    //value = 1;
	Cx_Interface<Ix_ConfigMgr> iConfigMgr(CLSID_ConfigMgr);

	XPropertys prop;
	iConfigMgr->GetModulePropEntry("ExampleTest", "Version", prop);
	printf("ExampleA Foo P Value: %s\n", prop[0].propertyText.c_str());

	printf("ExampleA Foo Value: %d\n", value);
}


