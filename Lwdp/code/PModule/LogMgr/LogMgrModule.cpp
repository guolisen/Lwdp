 
#define LWDP_MODULE_IMPL
#include <Lwdp.h>
#include <PluginInc.h>
#include <iostream>

#include <LwApiLib/ComLib/log4cpp/Portability.hh>
#include <LwApiLib/ComLib/log4cpp/Category.hh>
#include <LwApiLib/ComLib/log4cpp/Appender.hh>
#include <LwApiLib/ComLib/log4cpp/FileAppender.hh>
#include <LwApiLib/ComLib/log4cpp/OstreamAppender.hh>
#include <LwApiLib/ComLib/log4cpp/Layout.hh>
#include <LwApiLib/ComLib/log4cpp/PatternLayout.hh>
#include <LwApiLib/ComLib/log4cpp/Priority.hh>
#include <LwApiLib/ComLib/log4cpp/NDC.hh>

#include "Cx_LogMgr.h"

LWDP_NAMESPACE_BEGIN;

XBEGIN_DEFINE_CLASS()
    XDEFINE_CLASSMAP_ENTRY_Singleton(MODID_LogMgr, CLSID_LogMgr, Cx_LogMgr)
XEND_DEFINE_CLASS_SYS();


DEF_MODULE_INFO_BEGIN(LogMgr, LWDP_TOP_DOMAIN, PlugLevel::LEVEL0, 0);
	DEC_INIT_FUN();
	DEC_UNINIT_FUN();
DEF_MODULE_INFO_END(LogMgr);


// Optional function to initialize this plugin when loaded by the plugin manager.
DEF_INIT_FUN(LogMgr)
{
	printf("LogMgr InitializePlugin\n");
	GET_OBJECT_RET(LogMgr, iLogMgr, LWDP_GET_OBJECT_ERROR);

	RINOK(iLogMgr->Init());

    return LWDP_OK;
}

// Optional function to free this plugin when unloaded by the plugin manager.
DEF_UNINIT_FUN(LogMgr)
{
	printf("LogMgr UninitializePlugin\n");
    return LWDP_OK;
}

LWDP_NAMESPACE_END;

