/*! \file ConnectPool.cpp
 *  \brief DbMgr Module Impl
 *  \author Guolisen, LwDp
 *  \date   2013.01.15
 */
 
#include <LwDp.h>
#include "ConnectPool.h"

static pthread_mutex_t setMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  timeOutCond  = PTHREAD_COND_INITIALIZER;


MYSQL* ConnectPool::openDb()
{
	MYSQL* pDb = mysql_init(NULL);
	if(NULL == pDb) 
		goto EXT;

	if(NULL == mysql_real_connect(pDb, mHost, mUser, mPasswd, mDbStr, mPort, NULL, mClientFlag))
		goto EXT;

	char value = 1;
	char timeOut = 5;
	mysql_options(pDb, MYSQL_OPT_CONNECT_TIMEOUT, (char*)(&timeOut) );
	mysql_options(pDb, MYSQL_OPT_RECONNECT, (char *)&value);
	mysql_set_character_set(pDb, "gb2312");
	
	if (mysql_select_db(pDb, mDbStr.c_str()) != 0) 
	{
		mysql_close(pDb);
		pDb = NULL;
		goto EXT;
	}

	return pDb;

EXT:
	LWDP_LOG_PRINT("DbMgr", LWDP_LOG_MGR::ERR, 
					"mysql_real_connect Error(%s)!", mysql_error(pDb));

	//��ʼ��mysql�ṹʧ��
	if (pDb != NULL )
	{
		mysql_close( pDb );
		pDb = NULL;
	}

	return NULL;
	
}

LWRESULT ConnectPool::ConnectToDb(uint32_ min_connect, uint32_ max_connect)
{
	mMaxConnNum = max_connect;
	mMinConnNum = min_connect;

	srand(time(NULL));
	for(int i = 0; i<mMinConnNum; ++i)
	{
		MYSQL* pDb = openDb();
		if(!pDb)
			return LWDP_OPEN_DB_ERROR;
		mConnectSet.push_back(pDb);
		mIdleIndex.push_back(i);
	}
	
	return LWDP_OK;
}

LWRESULT ConnectPool::GetConnect(ConnectVal& ret_connect, int32_ timeout_ms)
{
	
	GET_OBJECT_RET(TimerTick, iTimerTick, LWDP_GET_OBJECT_ERROR);
	uint32_ startTimeout = iTimerTick->GetMilliseconds();
	pthread_mutex_lock(&setMutex);
	while(!mIdleIndex.size())
	{
		if(mConnectSet.size() < mMaxConnNum)
		{
			MYSQL* pDb = openDb();
			if(!pDb)
			{
				pthread_mutex_unlock(&setMutex);
				return LWDP_OPEN_DB_ERROR;
			}
			mConnectSet.push_back(pDb);
			mIdleIndex.push_back(mConnectSet.size() - 1);	
		}
		else if(mConnectSet.size() == mMaxConnNum)
		{
			if(timeout_ms < 0)
			{
				pthread_mutex_unlock(&setMutex);
				return LWDP_GET_POOL_CONNECT_TIMEOUT;
			}
			uint32_ nowTimeout = iTimerTick->GetMilliseconds();
			uint32_ diff = nowTimeout - startTimeout;
			if(diff > timeout_ms)
			{
				pthread_mutex_unlock(&setMutex);
				return LWDP_GET_POOL_CONNECT_TIMEOUT;
			}

			pthread_cond_wait(&timeOutCond, &setMutex); 
			continue;
		}
	}
	
	uint32_ selectIndex = randof(mIdleIndex.size());
	ConnectVal retVal;
	retVal.index   = mIdleIndex[selectIndex];
	retVal.connect = mConnectSet[retVal.index];
	ret_connect = retVal;
	
	return LWDP_OK;
}

LWRESULT ConnectPool::ReleaseConnect(const ConnectVal& release_connect)
{



}
LWRESULT ConnectPool::DestoryConnect(const ConnectVal& destory_connect)
{



}
LWRESULT ConnectPool::DestoryPool()
{



}





