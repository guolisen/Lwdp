#ifndef EVENTMGR_INTERFACE_H
#define EVENTMGR_INTERFACE_H

#include <Interface/Ix_Object.h>
#include "Id_EventMgr.h"

LWDP_NAMESPACE_BEGIN;

/* flag bits for ev_default_loop and ev_loop_new */
enum {
	/* the default */
	LWFLAG_AUTO      = 0x00000000U, /* not quite a mask */
	/* flag bits */
	LWFLAG_NOENV     = 0x01000000U, /* do NOT consult environment */
	LWFLAG_FORKCHECK = 0x02000000U, /* check for a fork in each iteration */
	/* debugging/feature disable */
	LWFLAG_NOINOTIFY = 0x00100000U, /* do not attempt to use inotify */
	LWFLAG_SIGNALFD  = 0x00200000U, /* attempt to use signalfd */
	LWFLAG_NOSIGMASK = 0x00400000U  /* avoid modifying the signal mask */
};

/* method bits to be ored together */
enum {
	LWBACKEND_SELECT  = 0x00000001U, /* about anywhere */
	LWBACKEND_POLL    = 0x00000002U, /* !win */
	LWBACKEND_EPOLL   = 0x00000004U, /* linux */
	LWBACKEND_KQUEUE  = 0x00000008U, /* bsd */
	LWBACKEND_DEVPOLL = 0x00000010U, /* solaris 8 */ /* NYI */
	LWBACKEND_PORT    = 0x00000020U, /* solaris 10 */
	LWBACKEND_ALL     = 0x0000003FU, /* all known backends */
	LWBACKEND_MASK    = 0x0000FFFFU  /* all future backends */
};

enum {
	LWRUN_NOWAIT = 1, /* do not block/wait */
	LWRUN_ONCE   = 2  /* block *once* only */
};

namespace LWEV{
enum WATCHER_TYPE
{
	WATCHER_IO,
	WATCHER_TIMER	
};
};
 
typedef void (*WATCHER_CALLBACK)(void *loop, void *w, int revents);
typedef void* WatcherHandle;


INTERFACE_BEGIN(EventMgr)
	//main loop
	virtual LWRESULT InitLoop(uint32_ flags) = 0;
	virtual LWRESULT RunLoop(uint32_ flags) = 0;

	//watcher
	virtual WatcherHandle CreateWatcher(LWEV::WATCHER_TYPE watcher_type, WATCHER_CALLBACK call_back, ...) = 0;
	virtual LWRESULT DestoryWatcher(WatcherHandle watcher_handle) = 0;

	virtual LWRESULT WatcherStart(WatcherHandle watcher_handle) = 0;
	virtual LWRESULT WatcherStop(WatcherHandle watcher_handle) = 0;

	
INTERFACE_END();


LWDP_NAMESPACE_END;

#endif // LOGMGR_INTERFACE_H
