/*
 * OstreamAppender.cpp
 *
 * Copyright 2000, LifeLine Networks BV (www.lifeline.nl). All rights reserved.
 * Copyright 2000, Bastiaan Bakker. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#include "PortabilityImpl.hh"
#ifdef LOG4CPP_HAVE_UNISTD_H
#    include <unistd.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <log4cpp/OstreamAppender.hh>

namespace log4cpp {
#ifdef _WIN32
	#define LOG_SET_COLOR(color) \
	{ \
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); \
        SetConsoleTextAttribute(h, color); \
	}
#else
	#define LOG_SET_COLOR(color)      
#endif

void LogMgr_SetColorEnter(std::string& msg, int dbLevel)
{
#ifdef _WIN32
    switch(dbLevel)
    {
        case Priority::FATAL:
            LOG_SET_COLOR(0x5|FOREGROUND_INTENSITY);
            break;
        case Priority::ALERT:
            LOG_SET_COLOR(0x3|FOREGROUND_INTENSITY);
            break;
        case Priority::CRIT:
            LOG_SET_COLOR(0x6|FOREGROUND_INTENSITY);
            break;
        case Priority::ERROR:
            LOG_SET_COLOR(FOREGROUND_RED|FOREGROUND_INTENSITY);
            break;
        case Priority::WARN:
            LOG_SET_COLOR(0x7|FOREGROUND_INTENSITY);
            break;
        case Priority::NOTICE:
            LOG_SET_COLOR(0xA|FOREGROUND_INTENSITY);
            break;
        case Priority::INFO:
            LOG_SET_COLOR(0x7);
            break;
        case Priority::DEBUG:
            LOG_SET_COLOR(0x8|FOREGROUND_INTENSITY);
            break;
    };
#else

#endif
}

void LogMgr_SetColorLeave()
{
#ifdef _WIN32
    LOG_SET_COLOR(0x7);
#endif
}

    OstreamAppender::OstreamAppender(const std::string& name, std::ostream* stream) : 
        LayoutAppender(name),
        _stream(stream) {
    }
    
    OstreamAppender::~OstreamAppender() {
        close();
    }

    void OstreamAppender::close() {
        // empty
    }

    void OstreamAppender::_append(const LoggingEvent& event) {
		std::string msg = _getLayout().format(event);
		LogMgr_SetColorEnter(msg, event.priority);	
        (*_stream) << msg;
		LogMgr_SetColorLeave();
        if (!_stream->good()) {
            // XXX help! help!
        }
    }

    bool OstreamAppender::reopen() {
        return true;
    }      
}
