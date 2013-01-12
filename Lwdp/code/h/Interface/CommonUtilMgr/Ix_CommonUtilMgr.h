#ifndef COMMONUTILMGR_INTERFACE_H
#define COMMONUTILMGR_INTERFACE_H

#include <Interface/Ix_Object.h>
#include "Id_CommonUtilMgr.h"
#include <Interface/PerfMgr/Ix_PerfMgr.h>

LWDP_NAMESPACE_BEGIN;

typedef void (*PROCESS_FUNC_PAIR)();
 
struct CallBackStru
{
	PROCESS_FUNC_PAIR createFunc;
	PROCESS_FUNC_PAIR destoryFunc;

	CallBackStru(PROCESS_FUNC_PAIR create, PROCESS_FUNC_PAIR destory): 
		            createFunc(create),destoryFunc(destory)
	{

	};

	CallBackStru(const CallBackStru& other)
	{
		createFunc  = other.createFunc;
		destoryFunc = other.destoryFunc;
	};

};

class ProcessController
{
public:
	ProcessController(CallBackStru call_back): mCb(call_back)
	{
		if(mCb.createFunc)
			mCb.createFunc();
	}
	virtual ~ProcessController()
	{
		if(mCb.destoryFunc)
			mCb.destoryFunc();
	}

	CallBackStru mCb;
};

#ifdef LWDP_PLATFORM_DEF_WIN32
class lw_mutex
{
public:
    inline lw_mutex ()
    {
        InitializeCriticalSection (&cs);
    }

    inline ~lw_mutex ()
    {
        DeleteCriticalSection (&cs);
    }

    inline void lock ()
    {
        EnterCriticalSection (&cs);
    }

    inline void unlock ()
    {
        LeaveCriticalSection (&cs);
    }

private:

    CRITICAL_SECTION cs;

    //  Disable copy construction and assignment.
    lw_mutex (const lw_mutex&);
    void operator = (const lw_mutex&);
};

#else
class lw_mutex
{
public:
    inline lw_mutex ()
    {
        int rc = pthread_mutex_init (&mutex, NULL);
        ASSERT_CHECK_HALT(LWDP_MODULE_LOG, (rc), "lw_mutex init Error");
    }

    inline ~lw_mutex ()
    {
        int rc = pthread_mutex_destroy (&mutex);
        ASSERT_CHECK_HALT(LWDP_MODULE_LOG, (rc), "lw_mutex destory Error");
    }

    inline void lock ()
    {
        int rc = pthread_mutex_lock (&mutex);
        ASSERT_CHECK_HALT(LWDP_MODULE_LOG, (rc), "lw_mutex lock Error");
    }

    inline void unlock ()
    {
        int rc = pthread_mutex_unlock (&mutex);
        ASSERT_CHECK_HALT(LWDP_MODULE_LOG, (rc), "lw_mutex unlock Error");
    }

private:

    pthread_mutex_t mutex;

    // Disable copy construction and assignment.
    lw_mutex (const lw_mutex&);
    const lw_mutex &operator = (const lw_mutex&);
};
#endif

class StatisticFigures
{
public:
	StatisticFigures():mMaxIntervel(0), 
                         mMinIntervel(0xFFFFFFFF),
                         mAvgIntervel(0), mTotle(0), mTimes(0)
	{

	}
	virtual ~StatisticFigures()
	{

	}
	
	uint32_ GetMax()
	{
		return mMaxIntervel;
	}
	uint32_ GetMin()
	{
		return mMinIntervel;
	}
	uint32_ GetAvg()
	{
		return mAvgIntervel;
	}

	void Update(uint32_ figures)
	{
		mutex.lock();

		if(figures > mMaxIntervel)
			mMaxIntervel = figures;

		if(figures < mMinIntervel)
			mMinIntervel = figures;

		mTimes++;
		mTotle += figures;
		if(5 == mTimes)
		{
			mAvgIntervel = mTotle / 5;
			mTimes = 0;
			mTotle = 0;
		}
		mutex.unlock();
	}	
protected:
	uint32_ mMaxIntervel;
	uint32_ mMinIntervel;
	uint32_ mAvgIntervel;
	lw_mutex mutex;

	long_   mTotle;
	uint32_ mTimes;	
};



INTERFACE_BEGIN(CommonUtilMgr)
	virtual LWRESULT Init() = 0;
	
INTERFACE_END();


LWDP_NAMESPACE_END;

#endif // LOGMGR_INTERFACE_H
