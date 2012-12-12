/*! \file Cx_Watcher.h
 *  \brief Watcher Impl
 *  \author Guolisen, LwDp
 *  \date   2012.12.10
 */
 
#ifndef CX_WATCHER_IMPL_H
#define CX_WATCHER_IMPL_H

LWDP_NAMESPACE_BEGIN;

//IO
class Cx_WatcherIO : public Ix_Watcher
{
public:
	Cx_WatcherIO(struct ev_loop* loop): mLoop(loop){};

public:
	virtual LWRESULT Init(WATCHER_CALLBACK call_back, va_list argp);
	virtual LWRESULT WatcherStart();
	virtual LWRESULT WatcherStop();

public:
	static void* GetTypeData(CBHandle call_back, LWEV::CALLBACK_DATA_TYPE type);
	
protected:
	ev_io mWatcher;
	struct ev_loop* mLoop;
	WATCHER_CALLBACK mCallBack;

};
class IOWatcherFactory : public Ix_WatcherFactory
{
public:
	virtual Ix_Watcher* CreateObject(struct ev_loop* loop){return (Ix_Watcher*)new Cx_WatcherIO(loop);};
	virtual uint32_ GetType() {return (uint32_)LWEV::WATCHER_IO;};

};

//Timer
class Cx_WatcherTimer : public Ix_Watcher
{
public:
	Cx_WatcherTimer(struct ev_loop* loop): mLoop(loop){};

public:
	virtual LWRESULT Init(WATCHER_CALLBACK call_back, va_list argp);
	virtual LWRESULT WatcherStart();
	virtual LWRESULT WatcherStop();

public:
	static void* GetTypeData(CBHandle call_back, LWEV::CALLBACK_DATA_TYPE type);

protected:
	ev_timer mWatcher;
	struct ev_loop* mLoop;

};
class TimerWatcherFactory : public Ix_WatcherFactory
{
public:
	virtual Ix_Watcher* CreateObject(struct ev_loop* loop){return (Ix_Watcher*)new Cx_WatcherTimer(loop);};
	virtual uint32_ GetType() {return (uint32_)LWEV::WATCHER_TIMER;};

};

LWDP_NAMESPACE_END;
#endif
