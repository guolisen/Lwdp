/*! \file Cx_ZmqMgr.cpp
 *  \brief ZmqMgr Module Impl
 *  \author Guolisen, LwDp
 *  \date   2012.12.03
 */
#include <LwDpPlatform.h>
#ifdef LWDP_PLATFORM_DEF_WIN32
#include <winsock2.h>
#endif

#include <LwDp.h>
#include <PluginInc.h>
#include <iostream>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>

#include "ZmqTagDef.h"
#include "Cx_ZmqMgr.h"



LWDP_NAMESPACE_BEGIN;



///////////////////////////////////////////
Cx_ZMessage::Cx_ZMessage()
{
}

Cx_ZMessage::~Cx_ZMessage()
{
    int rc = zmq_msg_close (&msg);
    ASSERT_CHECK_VOID(LWDP_ZMQ_LOG, (rc == 0), "Close ZMessage Error");
}

LWRESULT Cx_ZMessage::InitZMessage()
{
    int rc = zmq_msg_init (&msg);
    if (rc != 0)
	{
		PLATFORM_LOG(LWDP_ZMQ_LOG, LWDP_LOG_ERR, "ZMQ: %s", zmq_strerror (rc));
		return LWDP_ZMESSAGE_INIT_ERROR;
	}

	return LWDP_OK;
}

LWRESULT Cx_ZMessage::InitZMessage (uint32_ size_)
{
    int rc = zmq_msg_init_size (&msg, size_);
    if (rc != 0)
	{
		PLATFORM_LOG(LWDP_ZMQ_LOG, LWDP_LOG_ERR, "ZMQ: %s", zmq_strerror (rc));
		return LWDP_ZMESSAGE_INIT_ERROR;
	}
	
	return LWDP_OK;
}

LWRESULT Cx_ZMessage::InitZMessage (void *data_, uint32_ size_, 
									free_fun *ffn_, void *hint_)
{
    int rc = zmq_msg_init_data (&msg, data_, size_, ffn_, hint_);
    if (rc != 0)
	{
		PLATFORM_LOG(LWDP_ZMQ_LOG, LWDP_LOG_ERR, "ZMQ: %s", zmq_strerror (rc));
		return LWDP_ZMESSAGE_INIT_ERROR;
	}
	
	return LWDP_OK;
}                                    

LWRESULT Cx_ZMessage::SetValue(void* data_, uint32_ size_)
{
	int rc = zmq_msg_close (&msg);
	if (rc != 0)
	{
		PLATFORM_LOG(LWDP_ZMQ_LOG, LWDP_LOG_ERR, "ZMQ: %s", zmq_strerror (rc));
		return LWDP_ZMESSAGE_MSG_CLOSE_ERROR;
	}

    int ret = zmq_msg_init_size (&msg, size_);
	{
		PLATFORM_LOG(LWDP_ZMQ_LOG, LWDP_LOG_ERR, "ZMQ: %s", zmq_strerror (rc));
		return LWDP_ZMESSAGE_MSG_INIT_ERROR;
	}
	
	memcpy(zmq_msg_data(&msg), data_, size_);
	return LWDP_OK;
}

LWRESULT Cx_ZMessage::Rebuild ()
{
    int rc = zmq_msg_close (&msg);
    if (rc != 0)
	{
		PLATFORM_LOG(LWDP_ZMQ_LOG, LWDP_LOG_ERR, "ZMQ: %s", zmq_strerror (rc));
		return LWDP_ZMESSAGE_MSG_CLOSE_ERROR;
	}

    rc = zmq_msg_init (&msg);
    if (rc != 0)
	{
		PLATFORM_LOG(LWDP_ZMQ_LOG, LWDP_LOG_ERR, "ZMQ: %s", zmq_strerror (rc));
		return LWDP_ZMESSAGE_MSG_INIT_ERROR;
	}

	return LWDP_OK;	
}	

LWRESULT Cx_ZMessage::Rebuild (uint32_ size_)
{
    int rc = zmq_msg_close (&msg);
    if (rc != 0)
	{
		PLATFORM_LOG(LWDP_ZMQ_LOG, LWDP_LOG_ERR, "ZMQ: %s", zmq_strerror (rc));
		return LWDP_ZMESSAGE_MSG_CLOSE_ERROR;
	}
    rc = zmq_msg_init_size (&msg, size_);
    if (rc != 0)
	{
		PLATFORM_LOG(LWDP_ZMQ_LOG, LWDP_LOG_ERR, "ZMQ: %s", zmq_strerror (rc));
		return LWDP_ZMESSAGE_MSG_INIT_ERROR;
	}

	return LWDP_OK;	
}	

LWRESULT Cx_ZMessage::Rebuild (void *data_, uint32_ size_, free_fun *ffn_,
                            void *hint_)
{
    int rc = zmq_msg_close (&msg);
    if (rc != 0)
	{
		PLATFORM_LOG(LWDP_ZMQ_LOG, LWDP_LOG_ERR, "ZMQ: %s", zmq_strerror (rc));
		return LWDP_ZMESSAGE_MSG_CLOSE_ERROR;
	}
    rc = zmq_msg_init_data (&msg, data_, size_, ffn_, hint_);
    if (rc != 0)
	{
		PLATFORM_LOG(LWDP_ZMQ_LOG, LWDP_LOG_ERR, "ZMQ: %s", zmq_strerror (rc));
		return LWDP_ZMESSAGE_MSG_INIT_ERROR;
	}
	
	return LWDP_OK;
}                           

LWRESULT Cx_ZMessage::Move (Cx_Interface<Ix_ZMessage>& msg_)
{
	Cx_ZMessage* zm = dynamic_cast<Cx_ZMessage*>(msg_.P()); 
	ASSERT_CHECK_RET(LWDP_ZMQ_LOG, LWDP_PARAMETER_ERROR, zm, "Cx_ZMessage::Move Para error, there is not a Cx_ZMessage obj");
    int rc = zmq_msg_move (&msg, &(zm->msg));
    if (rc != 0)
	{
		PLATFORM_LOG(LWDP_ZMQ_LOG, LWDP_LOG_ERR, "ZMQ: %s", zmq_strerror (rc));
		return LWDP_ZMESSAGE_MSG_MOVE_ERROR;
	}

	return LWDP_OK;
}	
LWRESULT Cx_ZMessage::Copy (Cx_Interface<Ix_ZMessage>& msg_)
{
	Cx_ZMessage* zm = dynamic_cast<Cx_ZMessage*>(msg_.P()); 
	ASSERT_CHECK_RET(LWDP_ZMQ_LOG, LWDP_PARAMETER_ERROR, zm, "Cx_ZMessage::Copy Para error, there is not a Cx_ZMessage obj");
    int rc = zmq_msg_copy (&msg, &(zm->msg));
    if (rc != 0)
	{
		PLATFORM_LOG(LWDP_ZMQ_LOG, LWDP_LOG_ERR, "ZMQ: %s", zmq_strerror (rc));
		return LWDP_ZMESSAGE_MSG_COPY_ERROR;
	}

	return LWDP_OK;
}

void* Cx_ZMessage::Data ()
{
    return zmq_msg_data (&msg);
}
const void* Cx_ZMessage::Data () const
{
    return zmq_msg_data (const_cast<zmq_msg_t*>(&msg));
}

uint32_ Cx_ZMessage::Size () const
{
    return zmq_msg_size (const_cast<zmq_msg_t*>(&msg));
}


///////////////////////////////////////////////////
Cx_ZmqMgr::Cx_ZmqMgr()
{
}

Cx_ZmqMgr::~Cx_ZmqMgr()
{
}

LWRESULT Cx_ZmqMgr::Init()
{
	PLATFORM_LOG(LWDP_ZMQ_LOG, LWDP_LOG_INFO, __T("Cx_ZmqMgr::Init OK!"));	

	return LWDP_OK;
}

ContextHandle Cx_ZmqMgr::GetNewContext()
{
	return (ContextHandle)zmq_ctx_new();
}
LWRESULT Cx_ZmqMgr::CloseContext(ContextHandle context_handle)
{
    if (context_handle == NULL)
        return LWDP_PARAMETER_ERROR;
    int rc = zmq_ctx_destroy(context_handle);
    ASSERT_CHECK_RET(LWDP_ZMQ_LOG, LWDP_ZMQ_CLOSE_CONEXT_ERROR, (rc == 0), "Close Context Error");

	return LWDP_OK;
}

SocketHandle Cx_ZmqMgr::GetNewSocket(ContextHandle &context_, int type_)
{
    return (SocketHandle)zmq_socket((void *)context_, type_);
}

LWRESULT Cx_ZmqMgr::CloseSocket(SocketHandle socket_v)
{
    if (socket_v == NULL)
        return LWDP_PARAMETER_ERROR;
    int rc = zmq_close(socket_v);
    ASSERT_CHECK_RET(LWDP_ZMQ_LOG, LWDP_ZMQ_CLOSE_SOCKET_ERROR, (rc == 0), "Close socket Error");

	return LWDP_OK;
}


//poll
int32_ Cx_ZmqMgr::Poll (LWDP_POLLITEM_T* items_, int32_ nitems_, long_ timeout_)
{
    return zmq_poll ((zmq_pollitem_t*)items_, nitems_, timeout_);
}

//Socket

LWRESULT Cx_ZmqMgr::Setsockopt (SocketHandle socket_v, int32_ option_, 
	                     const void* optval_, uint32_ optvallen_)
{
    int rc = zmq_setsockopt (socket_v, option_, optval_, optvallen_);
    if (rc != 0)
	{
		PLATFORM_LOG(LWDP_ZMQ_LOG, LWDP_LOG_ERR, "ZMQ: %s", zmq_strerror (rc));
		return LWDP_ZMQ_SETSOCKOPT_ERROR;
	}
	
	return LWDP_OK;
}	                     
LWRESULT Cx_ZmqMgr::Getsockopt (SocketHandle socket_v, int32_ option_, 
					     void* optval_, uint32_* optvallen_)
{
    int rc = zmq_getsockopt (socket_v, option_, optval_, optvallen_);
    if (rc != 0)
	{
		PLATFORM_LOG(LWDP_ZMQ_LOG, LWDP_LOG_ERR, "ZMQ: %s", zmq_strerror (rc));
		return LWDP_ZMQ_GETSOCKOPT_ERROR;
	}
	
	return LWDP_OK;
}
LWRESULT Cx_ZmqMgr::Bind (SocketHandle socket_v, const char_* addr_)
{
    int rc = zmq_bind (socket_v, addr_);
    if (rc != 0)
	{
		PLATFORM_LOG(LWDP_ZMQ_LOG, LWDP_LOG_ERR, "ZMQ: %s", zmq_strerror (rc));
		return LWDP_ZMQ_BIND_ERROR;
	}
	
	return LWDP_OK;
}
LWRESULT Cx_ZmqMgr::Connect (SocketHandle socket_v, const char_* addr_)
{
    int rc = zmq_connect (socket_v, addr_);
    if (rc != 0)
	{
		PLATFORM_LOG(LWDP_ZMQ_LOG, LWDP_LOG_ERR, "ZMQ: %s", zmq_strerror (rc));
		return LWDP_ZMQ_CONNECT_ERROR;
	}
	
	return LWDP_OK;
}	
bool Cx_ZmqMgr::Connected(SocketHandle socket_v)
{
	return true;
}

uint32_ Cx_ZmqMgr::Send (SocketHandle socket_v, const void* buf_, uint32_ len_, int32_ flags_ )
{
    int nbytes = zmq_send (socket_v, buf_, len_, flags_);
    if (nbytes >= 0)
        return (uint32_) nbytes;
    if (zmq_errno () == EAGAIN)
        return 0;
	
    return 0;
}

bool Cx_ZmqMgr::Send (SocketHandle socket_v, Cx_Interface<Ix_ZMessage>& msg_, int32_ flags_)
{
	Cx_ZMessage* zm = dynamic_cast<Cx_ZMessage*>(msg_.P()); 
	ASSERT_CHECK_RET(LWDP_ZMQ_LOG, false, zm, "Cx_ZmqMgr::Send Para error, there is not a Cx_ZMessage obj");

    int nbytes = zmq_sendmsg (socket_v, &(zm->msg), flags_);
    if (nbytes >= 0)
        return true;
    if (zmq_errno () == EAGAIN)
        return false;

    return false;
}

bool Cx_ZmqMgr::Sendmore (SocketHandle socket_v, const std::string & string)
{
	return false;
}
uint32_ Cx_ZmqMgr::Recv (SocketHandle socket_v, void* buf_, uint32_ len_, int32_ flags_)
{
    int nbytes = zmq_recv (socket_v, buf_, len_, flags_);
    if (nbytes >= 0)
        return (uint32_) nbytes;
    if (zmq_errno () == EAGAIN)
        return 0;

    return 0;
}

bool Cx_ZmqMgr::Recv (SocketHandle socket_v, Cx_Interface<Ix_ZMessage>& msg_, int32_ flags_)
{
	Cx_ZMessage* zm = dynamic_cast<Cx_ZMessage*>(msg_.P()); 
	ASSERT_CHECK_RET(LWDP_ZMQ_LOG, false, zm, "Cx_ZmqMgr::Recv Para error, there is not a Cx_ZMessage obj");
    int nbytes = zmq_recvmsg (socket_v, &(zm->msg), flags_);
    if (nbytes >= 0)
        return true;
    if (zmq_errno () == EAGAIN)
        return false;

    return false;
}

std::string Cx_ZmqMgr::Recv (SocketHandle socket_v, int32_ flags_)
{
	GET_OBJECT_RET(ZMessage, iZMessage, std::string(""));
    iZMessage->InitZMessage();
    Recv(socket_v, iZMessage, 0);

    return std::string(static_cast<char*>(iZMessage->Data()), iZMessage->Size());
}

LWDP_NAMESPACE_END;

