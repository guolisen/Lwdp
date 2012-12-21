/*! \file DbMgrErrno.h
 *  \brief DbMgr Module Errno Define
 *  \author Guolisen, LwDp
 *  \date   2012.10.17
 */


ERRNO_DB (OPEN_DB_ERROR,            (M_dbmgr | 1), "Open Database Error!")
ERRNO_DB (PING_DB_ERROR,            (M_dbmgr | 2), "Ping Database Error!")
ERRNO_DB (SHUTDOWN_DB_ERROR,        (M_dbmgr | 3), "Shutdown Database Error!")
ERRNO_DB (REBOOT_DB_ERROR,          (M_dbmgr | 4), "Reboot Database Error!")
ERRNO_DB (GET_QUERY_OBJ_ERROR,      (M_dbmgr | 5), "Get Db_Query Object Error!")
ERRNO_DB (REAL_QUERY_ERROR,         (M_dbmgr | 6), "Real Query Error!")



