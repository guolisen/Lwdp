#ifndef _EVENTMGR_INTERFACE_H
#define _EVENTMGR_INTERFACE_H

#include <Interface/Ix_Object.h>
#include "Id_EventMgr.h"

LWDP_NAMESPACE_BEGIN;

/* eventmask, revents, events... */
enum {
  LW_UNDEF    = 0xFFFFFFFF, /* guaranteed to be invalid */
  LW_NONE     =       0x00, /* no events */
  LW_READ     =       0x01, /* ev_io detected read will not block */
  LW_WRITE    =       0x02, /* ev_io detected write will not block */
  LW__IOFDSET =       0x80, /* internal use only */
  LW_IO       =    LW_READ, /* alias for type-detection */
  LW_TIMER    = 0x00000100, /* timer timed out */
#if LW_COMPAT3
  LW_TIMEOUT  =   LW_TIMER, /* pre 4.0 API compatibility */
#endif
  LW_PERIODIC = 0x00000200, /* periodic timer timed out */
  LW_SIGNAL   = 0x00000400, /* signal was received */
  LW_CHILD    = 0x00000800, /* child/pid had status change */
  LW_STAT     = 0x00001000, /* stat data changed */
  LW_IDLE     = 0x00002000, /* event loop is idling */
  LW_PREPARE  = 0x00004000, /* event loop about to poll */
  LW_CHECK    = 0x00008000, /* event loop finished poll */
  LW_EMBED    = 0x00010000, /* embedded event loop needs sweep */
  LW_FORK     = 0x00020000, /* event loop resumed in child */
  LW_CLEANUP  = 0x00040000, /* event loop resumed in child */
  LW_ASYNC    = 0x00080000, /* async intra-loop signal */
  LW_CUSTOM   = 0x01000000, /* for use by user code */
  LW_ERROR    = 0x80000000  /* sent when an error occurs */
};

#define LW_PERSIST                 0x10


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

	enum CALLBACK_DATA_TYPE
	{
		WATCHER_IO_FD
		//WATCHER_TIMER_	
	};
};

typedef void*  WatcherHandle;
typedef void*  CBHandle;
typedef void*  LoopHandle;
typedef void (*WATCHER_CALLBACK)(LoopHandle loop, CBHandle w, int revents);



INTERFACE_BEGIN(EventMgr)
	//main loop
	virtual LWRESULT InitLoop(uint32_ flags) = 0;
	virtual LWRESULT RunLoop(uint32_ flags) = 0;

	//watcher
	virtual WatcherHandle CreateWatcher(LWEV::WATCHER_TYPE watcher_type, WATCHER_CALLBACK call_back, ...) = 0;
	virtual LWRESULT DestoryWatcher(WatcherHandle watcher_handle) = 0;

	virtual LWRESULT WatcherStart(WatcherHandle watcher_handle) = 0;
	virtual LWRESULT WatcherStop(WatcherHandle watcher_handle) = 0;
	virtual void* GetCallBackData(CBHandle cb_handle, LWEV::WATCHER_TYPE watcher_type, LWEV::CALLBACK_DATA_TYPE data_type) = 0;

	
INTERFACE_END();


LWDP_NAMESPACE_END;

#endif // LOGMGR_INTERFACE_H
