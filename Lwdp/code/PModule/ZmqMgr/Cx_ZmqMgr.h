/*! \file Cx_ZmqMgr.h
 *  \brief ZmqMgr Module Impl
 *  \author Guolisen, LwDp
 *  \date   2012.12.03
 */
 
#ifndef CX_LUA_MANAGER_H
#define CX_LUA_MANAGER_H

#include <iostream>
//#include <assert>
#include <string>
#include <algorithm>
#include <exception>
#include <boost/shared_ptr.hpp>
#include <list>

#include <Interface/ZmqMgr/Ix_ZmqMgr.h>
#include "ZmqTagDef.h"
#include <ZMQ/include/zmq.h>



LWDP_NAMESPACE_BEGIN;

class Cx_ZMessage
    : public Ix_ZMessage
{
	friend class Cx_ZmqMgr;
	friend class Ix_ZmqMgr;

    X3BEGIN_CLASS_DECLARE(Cx_ZMessage)
        X3DEFINE_INTERFACE_ENTRY(Ix_ZMessage)
    X3END_CLASS_DECLARE()
protected:
	Cx_ZMessage();
	virtual ~Cx_ZMessage();

protected:
    virtual LWRESULT InitZMessage();
    virtual LWRESULT InitZMessage (uint32_ size_);
    virtual LWRESULT InitZMessage (void *data_, uint32_ size_, free_fun *ffn_,
                                     void *hint_ = NULL);

	virtual LWRESULT SetValue(void* data_, uint32_ size_);
    virtual LWRESULT Rebuild ();
    virtual LWRESULT Rebuild (uint32_ size_);
    virtual LWRESULT Rebuild (void *data_, uint32_ size_, free_fun *ffn_,
                            void *hint_ = NULL);

    virtual LWRESULT Move (Cx_Interface<Ix_ZMessage>& msg_);
    virtual LWRESULT Copy (Cx_Interface<Ix_ZMessage>& msg_);
    virtual void* Data ();
    virtual const void* Data () const;
    virtual uint32_ Size () const;

private:
	int32_    mInitTag;
	zmq_msg_t msg;
};

class Cx_ZmqMgr
    : public Ix_ZmqMgr
{
    X3BEGIN_CLASS_DECLARE(Cx_ZmqMgr)
        X3DEFINE_INTERFACE_ENTRY(Ix_ZmqMgr)
    X3END_CLASS_DECLARE()
protected:
	Cx_ZmqMgr();
	virtual ~Cx_ZmqMgr();

protected:
	virtual LWRESULT Init();
	virtual ContextHandle GetNewContext();
	virtual LWRESULT CloseContext(ContextHandle context_handle);

	virtual SocketHandle GetNewSocket(ContextHandle &context_, int type_) ;
    virtual LWRESULT CloseSocket(SocketHandle socket_v);

    //poll
    virtual int32_ Poll (LWDP_POLLITEM_T* items_, int32_ nitems_, long_ timeout_); 

	//Socket
    virtual LWRESULT Setsockopt (SocketHandle socket_v, int32_ option_, 
		                     const void* optval_, uint32_ optvallen_) ;
    virtual LWRESULT Getsockopt (SocketHandle socket_v, int32_ option_, 
						     void* optval_, uint32_* optvallen_) ;

    virtual LWRESULT Bind (SocketHandle socket_v, const char_* addr_) ;

    virtual LWRESULT Connect (SocketHandle socket_v, const char_* addr_) ;
    virtual bool Connected(SocketHandle socket_v) ;

    virtual uint32_ Send (SocketHandle socket_v, const void* buf_, uint32_ len_, int32_ flags_ = 0 ) ;
    virtual bool Send (SocketHandle socket_v, Cx_Interface<Ix_ZMessage>& msg_, int32_ flags_= 0) ;
	virtual bool Sendmore (SocketHandle socket_v, const std::string & string);

    virtual uint32_ Recv (SocketHandle socket_v, void* buf_, uint32_ len_, int32_ flags_= 0) ;
    virtual bool Recv (SocketHandle socket_v, Cx_Interface<Ix_ZMessage>& msg_, int32_ flags_= 0) ;
    virtual std::string Recv (SocketHandle socket_v, int32_ flags_ = 0);


};


LWDP_NAMESPACE_END;

#endif
