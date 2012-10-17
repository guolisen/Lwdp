#include <Lwdp.h>
#include <PluginInc.h>

//LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;

#if 0
// Optional function to initialize this plugin when loaded by the plugin manager.
bool DEF_INIT_FUN(ExampleTest)()
{
	printf("LwInitializePlugin\n");
    return true;
}

// Optional function to free this plugin when unloaded by the plugin manager.
void LwUninitializePlugin()
{
	printf("LwUninitializePlugin\n");
}

#endif
EXTERN_C_END;
//LWDP_NAMESPACE_END;