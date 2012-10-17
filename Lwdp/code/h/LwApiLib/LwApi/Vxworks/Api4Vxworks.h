/*! \file Api4Vxworks.h
 *  \brief Included by LwApi.h to implement Vxworks Api.
 *  \author Guolisen, LwDp
 *  \date   2011.9.4
 */
#ifndef API_FOR_VXWORKS_H
#define API_FOR_VXWORKS_H

#define KEEP_PROCESS_ALIVE
#include <vxWorks.h>
#include <taskLib.h>
#include <wdLib.h>
#include <inetLib.h>
#include <msgQLib.h>
#include <semLib.h>
#include <hostLib.h>
#include <sockLib.h>
#include <ioLib.h>
#include <iosLib.h>
#include <pipeDrv.h>
#include <sysLib.h>
#include <tickLib.h>
#include <selectLib.h>
#include <stdio.h> 
#include <string.h>
#include <rebootLib.h>
#include <loadlib.h>
#include <ramDrv.h>
#include <dosFsLib.h>
#include <ifLib.h>
#include <time.h>

#define opens 	open
#define reads 	read
#define writes 	write
#define closes 	close

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;

#define VXWORKS_IMPL_API(api) VXW_##api


void VXWORKS_IMPL_API(TaskDelay)(ulong_ tick);
void VXWORKS_IMPL_API(HaltSystem)();


#define Api_TaskDelay               VXWORKS_IMPL_API(TaskDelay)
#define Api_HaltSystem              VXWORKS_IMPL_API(HaltSystem)


EXTERN_C_END;
LWDP_NAMESPACE_END;

#endif // API_FOR_VXWORKS_H


