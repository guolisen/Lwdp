/*! \file ConnectPool.h
 *  \brief DbMgr Module Impl
 *  \author Guolisen, LwDp
 *  \date   2013.01.15
 */

#ifndef DB_CONNECT_POOL
#define DB_CONNECT_POOL
#include <mysql.h> 

struct ConnectVal
{
	MYSQL*  connect;	
	int32_ index;

	ConnectVal()
	{
		connect = NULL;
		index   = -1;
	};

	ConnectVal(const ConnectVal& other)
	{
		connect = other.connect;	
		index   = other.index;
	};
	ConnectVal& operator=(const ConnectVal& right)
	{
		connect = right.connect;	
		index   = right.index;
	};
};

typedef std::vector<MYSQL*> CONNECT_POOL_SET;
typedef std::vector<uint32_> CONNECT_POOL_INDEX;

	
class ConnectPool
{
public:
	ConnectPool(const std::string& host, 
		            const std::string& user, 
		            const std::string& passwd, 
		            const std::string& db,
	 				int32_ port, long_ client_flag):mHost(host), 
	 				                                mUser(user), 
	 				                                mPasswd(passwd),
	 				                                mDbStr(db),
	 				                                mPort(port),
	 				                                mClientFlag(client_flag){};
	virtual ~ConnectPool()
	{
		LWRESULT ret = DestoryPool();
		ASSERT_CHECK(ret == LWDP_OK);
	};

	virtual LWRESULT ConnectToDb(uint32_ min_connect, uint32_ max_connect);
	virtual LWRESULT GetConnect(ConnectVal& ret_connect, int32_ timeout);
	virtual LWRESULT ReleaseConnect(const ConnectVal& release_connect);
	virtual LWRESULT DestoryConnect(const ConnectVal& destory_connect);
	virtual LWRESULT DestoryPool();

protected:
	virtual MYSQL*   openDb();

protected:
	std::string  mHost;
	std::string  mUser;
	std::string  mPasswd;
	std::string  mDbStr;
	int32_       mPort;
	long_        mClientFlag;	

protected:
	uint32_ mMaxConnNum;
	uint32_ mMinConnNum;
	CONNECT_POOL_INDEX mIdleIndex;
	CONNECT_POOL_SET   mConnectSet;
	lw_mutex mutex;
};



#endif

