/*! \file   MyAppender.h
 *  \brief  MyAppender
 *  \author Guolisen, Lwdp PluginFramework
 *  \date   2012.02.06
 */
#ifndef LWDP_MY_APPENDER_H_
#define LWDP_MY_APPENDER_H_

#include <LwApiLib/ComLib/log4cpp/Portability.hh>
#include <string>
#include <iostream>
#include <LwApiLib/ComLib/log4cpp/LayoutAppender.hh>

#include <LwDp.h>
#include <Interface/LogMgr/Ix_LogMgr.h>

namespace log4cpp {

    /**
     * MyAppender appends LoggingEvents to Self Define.
     **/
class LOG4CPP_EXPORT MyAppender : public LayoutAppender {
public:
    MyAppender(NLwdp::MyAppender_Ptr pAppender);
    virtual ~MyAppender();
    
    virtual bool reopen();
    virtual void close();

protected:
    virtual void _append(const LoggingEvent& event);
	
protected:
	NLwdp::MyAppender_Ptr mApp_ptr;		
};
}


#endif // LWDP_MY_APPENDER_H_





