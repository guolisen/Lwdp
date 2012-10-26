#include <Lwdp.h>
#include <PluginInc.h>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>

#include "Cx_Example.h"


Cx_Example::Cx_Example()
{
	printf("Example Create!\n");
}

Cx_Example::~Cx_Example()
{
	printf("Example Delete!\n");
}


void *
thr(void * arg)
{
	int i = 5 ;
	GET_OBJECT(LogMgr, iLogMgr, 0);

	while(1)
	{
		iLogMgr->LogPrint("Test4", LWDP_LOG_MGR::DEBUG, __FILE__, __LINE__, "Thr Hello World3!!");
		printf("TTTTTTT %d\n", i++);
		sleep(1);
	}
}

void Cx_Example::Foo(long_& value)
{
    //value = 1;
	Cx_Interface<Ix_ConfigMgr> iConfigMgr(CLSID_ConfigMgr);

	XPropertys prop;
	iConfigMgr->GetModulePropEntry("ExampleTest", "Version", prop);
	printf("Example Foo Value: %s\n", prop[0].propertyText.c_str());

	Cx_Interface<Ix_Example2> iEx2(CLSID_Example);
	
	long_ test = 1345;
	iEx2->Foo2(test);

	pthread_t t;
	int result;

	assert(pthread_create(&t, NULL, thr, NULL) == 0);
	assert(pthread_join(t, (void **)&result) == 0);

}

void Cx_Example::Foo2(long_& value)
{
    //value = 2;
	printf("Example Foo2 Value: %d\n", value);
}

