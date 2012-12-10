/*! \file ZmqMgrErrno.h
 *  \brief ZmqMgr Module Errno Define
 *  \author Guolisen, LwDp
 *  \date   2012.12.03
 */


ERRNO_DB (ZMESSAGE_INIT_ERROR,            (M_zmqmgr | 1), "ZMessage init Error!")
ERRNO_DB (ZMESSAGE_MSG_INIT_ERROR,        (M_zmqmgr | 2), "ZMessage zmq_msg_init return Error!")
ERRNO_DB (ZMESSAGE_MSG_CLOSE_ERROR,       (M_zmqmgr | 3), "ZMessage zmq_msg_close return Error!")
ERRNO_DB (ZMESSAGE_MSG_MOVE_ERROR,        (M_zmqmgr | 4), "ZMessage zmq_msg move return Error!")
ERRNO_DB (ZMESSAGE_MSG_COPY_ERROR,        (M_zmqmgr | 5), "ZMessage zmq_msg copy return Error!")
ERRNO_DB (ZMQ_SETSOCKOPT_ERROR,           (M_zmqmgr | 6), "zmq_setsockopt return Error!")
ERRNO_DB (ZMQ_GETSOCKOPT_ERROR,           (M_zmqmgr | 7), "zmq_getsockopt return Error!")
ERRNO_DB (ZMQ_BIND_ERROR,                 (M_zmqmgr | 8), "zmq_bind return Error!")
ERRNO_DB (ZMQ_CONNECT_ERROR,              (M_zmqmgr | 9), "zmq_connect return Error!")
ERRNO_DB (ZMQ_CLOSE_CONEXT_ERROR,         (M_zmqmgr | 10), "zmq close context return Error!")
ERRNO_DB (ZMQ_CLOSE_SOCKET_ERROR,         (M_zmqmgr | 11), "zmq close socket return Error!")



