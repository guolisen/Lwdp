#ifndef COMMONUTILMGR_INTERFACE_H
#define COMMONUTILMGR_INTERFACE_H

#include <Interface/Ix_Object.h>
#include "Id_CommonUtilMgr.h"
#include <Interface/PerfMgr/Ix_PerfMgr.h>
#include <Interface/TimerMgr/Ix_TimerMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>

LWDP_NAMESPACE_BEGIN;

#if (defined (LWDP_PLATFORM_DEF_WIN32))
#   define randof(num)  (int)((float)(num) * rand () / (RAND_MAX + 1.0))
#else
#   define randof(num)  (int)((float)(num) * random () / (RAND_MAX + 1.0))
#endif


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

class TimerCounter
{
public:
	TimerCounter(std::string pos_str, double_ top_second)
	{
		mPos = pos_str;
		mUSecond = top_second;
		GET_OBJECT_VOID(PerfMgr_timer, iPerfMgr_timer);
		mPerfMgr_timer = iPerfMgr_timer;
		mPerfMgr_timer->Start();
	}
	virtual ~TimerCounter()
	{
		double_ psecond = (double_)mPerfMgr_timer->End() / 1000.0;

		if(psecond > mUSecond)
		{
			LWDP_LOG_PRINT("CT", LWDP_LOG_MGR::INFO, 
						   "%s Process Time(%.03f s)", 
						   mPos.c_str(), psecond);
		}
	}

	Cx_Interface<Ix_PerfMgr_timer> mPerfMgr_timer;
	std::string mPos;
	double_ mUSecond;
};

#ifdef LWDP_DEBUG_MACRO
#define DEBUG_TIME_COUNTER(msg, top_second) \
	TimerCounter __TC_counter(msg, top_second);
#else
#define DEBUG_TIME_COUNTER(msg, top_second)
#endif

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
        ASSERT_CHECK_HALT(LWDP_MODULE_LOG, (rc == 0), "lw_mutex init Error");
    }

    inline ~lw_mutex ()
    {
        int rc = pthread_mutex_destroy (&mutex);
        ASSERT_CHECK_HALT(LWDP_MODULE_LOG, (rc == 0), "lw_mutex destory Error");
    }

    inline void lock ()
    {
        int rc = pthread_mutex_lock (&mutex);
        ASSERT_CHECK_HALT(LWDP_MODULE_LOG, (rc == 0), "lw_mutex lock Error");
    }

    inline void unlock ()
    {
        int rc = pthread_mutex_unlock (&mutex);
        ASSERT_CHECK_HALT(LWDP_MODULE_LOG, (rc == 0), "lw_mutex unlock Error");
    }

private:

    pthread_mutex_t mutex;

    // Disable copy construction and assignment.
    lw_mutex (const lw_mutex&);
    const lw_mutex &operator = (const lw_mutex&);
};
#endif


class lw_mutex_class
{
public:
	lw_mutex_class(pthread_mutex_t* mutex)
	{
		mMutex = mutex;
		ASSERT_CHECK(mMutex != 0);
		pthread_mutex_lock(mMutex);
	};
	virtual ~lw_mutex_class()
	{
		pthread_mutex_unlock(mMutex);
	};

	pthread_mutex_t* mMutex;
};

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


class LwdpProgressBar {
public:
	LwdpProgressBar(unsigned int _n=0) : n(_n), pct(0), cur(0), width(80) {}
	void reset() { pct = 0; cur = 0; }
	void start() { 
		#ifdef WIN32
			assert(QueryPerformanceFrequency(&g_llFrequency) != 0);
		#endif
		startTime = osQueryPerfomance();
		setPct(0); 
	}
	void finish() { 
		setPct(1); 
	}
	unsigned long  operator+=( unsigned long increment )
	//  Effects: Display appropriate progress tic if needed.
	{
		if (cur >= n) return cur;
		mutex.lock();
		cur += increment;
		float step = ((float)cur)/n;
		mutex.unlock();
		if(step >= 1.0)
			return cur;
		setPct(step);
		return cur;
	}

	unsigned long  operator++() { return operator+=( 1 ); }

	// http://stackoverflow.com/questions/3283804/c-get-milliseconds-since-some-date
	int64_ osQueryPerfomance() {
		#ifdef WIN32
		#if 0
			LARGE_INTEGER llPerf = {0};
			HANDLE thread = GetCurrentThread();
			// Set affinity to the first core
			DWORD_PTR oldMask = SetThreadAffinityMask(thread, 1);

			QueryPerformanceCounter(&llPerf);
			SetThreadAffinityMask(thread, oldMask);
			return llPerf.QuadPart * 1000ll / ( g_llFrequency.QuadPart / 1000ll);
		#endif
			GET_OBJECT_RET(TimerTick, iTimerTick, 0);
			return iTimerTick->GetMilliseconds();

		#elif defined(__VXWORKS__)
			struct tm newtime;
			time_t long_time = time(NULL);
			localtime_r(&long_time,&newtime);
			return newtime.tm_sec * 1000000ll + 0;
		#else
			struct timeval stTimeVal;
			gettimeofday(&stTimeVal, NULL);
			return stTimeVal.tv_sec * 1000000ll + stTimeVal.tv_usec;
		#endif
		return 0;
	}

	std::string secondsToString(long long t) {
		int days = (int)(t/86400);
		long long sec = t-days*86400;
		int hours = (int)sec/3600;
		sec -= hours*3600;
		int mins = (int)(sec/60);
		sec -= mins*60;
		char tmp[8];
		std::string out;
		
		if (days) {
			sprintf(tmp, "%dd ", days);
			out += tmp;
		}
		
		if (hours >= 1) {
			sprintf(tmp, "%dh ", hours);
			out += tmp;
		}

		if (mins >= 1) {
			sprintf(tmp, "%dm ", mins);
			out += tmp;
		}
		
		if (sec >= 1) {
			sprintf(tmp, "%ds", (int)sec);
			out += tmp;
		}
		
		if (out.empty())
			out = "0s";
			
		return out;
	}
	
	// Set 0.0-1.0, where 1.0 equals 100%.
	void setPct(float Pct) {
		endTime = osQueryPerfomance();
		char pctstr[5];
		Api_snprintf(pctstr, 5, "%3d%%", (int)(100*Pct));
		// Compute how many tics we can display.
		int nticsMax = (width-27);
		int ntics = (int)(nticsMax*Pct);
		std::string out(pctstr);
		out.append(" [");
		
		if(Pct < 1.0 && ntics >= 2)
		{
			out.append(ntics-2,'=');
			out.append(">>");	
		}
		else
		{
			out.append(ntics,'=');
		}
		out.append(nticsMax-ntics,' ');
		out.append("] ");
		out.append((Pct<1.0) ? "ETA " : "in ");
		// Seconds.
		long long dt = (long long)((endTime-startTime)/1000000.0);
		std::string tstr;
		if (Pct >= 1.0) {
			// Print overall time and newline.
			tstr = secondsToString(dt);
			out.append(tstr);
			if (out.size() < width)
				out.append(width-out.size()-1,' ');

			//out.append("\n");
			std::cout << out << std::endl;
			return;
		} else {
			float eta=999999.;
			if (Pct > 0.0)
				eta = dt*(1.0-Pct)/Pct;
	
			if (eta > 604800.0)
				out.append("> 1 week");
			else {
				tstr = secondsToString((long long)eta);
				out.append(tstr);
			}
		}

		// Pad end with spaces to overwrite previous string that may have been longer.
		if (out.size() < width)
			out.append(width-out.size()-1,' ');
			
		out.append("\r");
		std::cout << out;
		std::cout.flush();
	}

	unsigned int n;
	unsigned int cur;
	unsigned short pct; // Stored as 0-1000, so 2.5% is encoded as 25.
	unsigned char width; // How many chars the entire line can be.
	int64_ startTime, endTime;
	lw_mutex mutex;
	#ifdef WIN32
		LARGE_INTEGER g_llFrequency;
	#endif
};


INTERFACE_BEGIN(CommonUtilMgr)
	virtual LWRESULT Init() = 0;
	
INTERFACE_END();


LWDP_NAMESPACE_END;

#endif // LOGMGR_INTERFACE_H
