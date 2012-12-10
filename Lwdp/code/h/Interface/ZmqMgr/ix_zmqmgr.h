#ifndef ZMQMGR_INTERFACE_H
#define ZMQMGR_INTERFACE_H

#include <Interface/Ix_Object.h>
#include "Id_ZmqMgr.h"

LWDP_NAMESPACE_BEGIN;

#define LWDP_POLLIN 1
#define LWDP_POLLOUT 2
#define LWDP_POLLERR 4

/*  Socket types.                                                             */ 
#define LWDP_PAIR 0
#define LWDP_PUB 1
#define LWDP_SUB 2
#define LWDP_REQ 3
#define LWDP_REP 4
#define LWDP_DEALER 5
#define LWDP_ROUTER 6
#define LWDP_PULL 7
#define LWDP_PUSH 8
#define LWDP_XPUB 9
#define LWDP_XSUB 10

/*  Deprecated aliases                                                        */
#define LWDP_XREQ LWDP_DEALER
#define LWDP_XREP LWDP_ROUTER

/*  Socket options.                                                           */
#define LWDP_AFFINITY 4
#define LWDP_IDENTITY 5
#define LWDP_SUBSCRIBE 6
#define LWDP_UNSUBSCRIBE 7
#define LWDP_RATE 8
#define LWDP_RECOVERY_IVL 9
#define LWDP_SNDBUF 11
#define LWDP_RCVBUF 12
#define LWDP_RCVMORE 13
#define LWDP_FD 14
#define LWDP_EVENTS 15
#define LWDP_TYPE 16
#define LWDP_LINGER 17
#define LWDP_RECONNECT_IVL 18
#define LWDP_BACKLOG 19
#define LWDP_RECONNECT_IVL_MAX 21
#define LWDP_MAXMSGSIZE 22
#define LWDP_SNDHWM 23
#define LWDP_RCVHWM 24
#define LWDP_MULTICAST_HOPS 25
#define LWDP_RCVTIMEO 27
#define LWDP_SNDTIMEO 28
#define LWDP_IPV4ONLY 31
#define LWDP_LAST_ENDPOINT 32
#define LWDP_ROUTER_MANDATORY 33
#define LWDP_TCP_KEEPALIVE 34
#define LWDP_TCP_KEEPALIVE_CNT 35
#define LWDP_TCP_KEEPALIVE_IDLE 36
#define LWDP_TCP_KEEPALIVE_INTVL 37
#define LWDP_TCP_ACCEPT_FILTER 38
#define LWDP_DELAY_ATTACH_ON_CONNECT 39
#define LWDP_XPUB_VERBOSE 40


/*  Message options                                                           */
#define LWDP_MORE 1

/*  Send/recv options.                                                        */
#define LWDP_DONTWAIT 1
#define LWDP_SNDMORE 2

/*  Deprecated aliases                                                        */
#define LWDP_NOBLOCK LWDP_DONTWAIT
#define LWDP_ROUTER_BEHAVIOR LWDP_ROUTER_MANDATORY

/******************************************************************************/
/*  0MQ socket events and monitoring                                          */
/******************************************************************************/

/*  Socket transport events (tcp and ipc only)                                */
#define LWDP_EVENT_CONNECTED 1
#define LWDP_EVENT_CONNECT_DELAYED 2
#define LWDP_EVENT_CONNECT_RETRIED 4

#define LWDP_EVENT_LISTENING 8
#define LWDP_EVENT_BIND_FAILED 16

#define LWDP_EVENT_ACCEPTED 32
#define LWDP_EVENT_ACCEPT_FAILED 64

#define LWDP_EVENT_CLOSED 128
#define LWDP_EVENT_CLOSE_FAILED 256
#define LWDP_EVENT_DISCONNECTED 512


typedef void* ContextHandle; 
typedef void* SocketHandle; 

typedef void (LWDP_FREE_FUN) (void *data, void *hint);
typedef LWDP_FREE_FUN free_fun;

typedef struct
{
    void *socket;
#if defined _WIN32
    SOCKET fd;
#else
    int fd;
#endif
    short events;
    short revents;
} LWDP_POLLITEM_T;


INTERFACE_BEGIN(ZMessage)
    virtual LWRESULT InitZMessage() = 0;
    virtual LWRESULT InitZMessage (uint32_ size_) = 0;
    virtual LWRESULT InitZMessage (void *data_, uint32_ size_, free_fun *ffn_,
                                     void *hint_ = NULL) = 0;

	virtual LWRESULT SetValue(void* data_, uint32_ size_) = 0;
    virtual LWRESULT Rebuild () = 0;
    virtual LWRESULT Rebuild (uint32_ size_) = 0;
    virtual LWRESULT Rebuild (void *data_, uint32_ size_, free_fun *ffn_,
                            void *hint_ = NULL) = 0;

    virtual LWRESULT Move (Cx_Interface<Ix_ZMessage>& msg_) = 0;
    virtual LWRESULT Copy (Cx_Interface<Ix_ZMessage>& msg_) = 0;
    virtual void* Data () = 0;
    virtual const void* Data () const = 0;
    virtual uint32_ Size () const = 0;
	
INTERFACE_END();


INTERFACE_BEGIN(ZmqMgr)
	virtual LWRESULT Init() = 0;

	virtual ContextHandle GetNewContext() = 0;

	virtual LWRESULT CloseContext(ContextHandle context_handle) = 0;

	virtual SocketHandle GetNewSocket(ContextHandle &context_, int type_) = 0;

    virtual LWRESULT CloseSocket(SocketHandle socket_v) = 0;

	//Poll
    virtual int32_ Poll (LWDP_POLLITEM_T* items_, int32_ nitems_, long_ timeout_) = 0;

	//Socket
    virtual LWRESULT Setsockopt (SocketHandle socket_v, int32_ option_, 
		                     const void* optval_, uint32_ optvallen_) = 0;
    virtual LWRESULT Getsockopt (SocketHandle socket_v, int32_ option_, 
						     void* optval_, uint32_* optvallen_) = 0;

    virtual LWRESULT Bind (SocketHandle socket_v, const char_* addr_) = 0;

    virtual LWRESULT Connect (SocketHandle socket_v, const char_* addr_) = 0;
    virtual bool Connected(SocketHandle socket_v) = 0;

    virtual uint32_ Send (SocketHandle socket_v, const void* buf_, uint32_ len_, int32_ flags_ ) = 0;
    virtual bool Send (SocketHandle socket_v, Cx_Interface<Ix_ZMessage>& msg_, int32_ flags_) = 0;
	virtual bool Sendmore (SocketHandle socket_v, const std::string & string) = 0;
	
    virtual uint32_ Recv (SocketHandle socket_v, void* buf_, uint32_ len_, int32_ flags_) = 0;
    virtual bool Recv (SocketHandle socket_v, Cx_Interface<Ix_ZMessage>& msg_, int32_ flags_) = 0;
    virtual std::string Recv (SocketHandle socket_v, int32_ flags_) = 0;

INTERFACE_END();


LWDP_NAMESPACE_END;

#endif // ZMQMGR_INTERFACE_H
