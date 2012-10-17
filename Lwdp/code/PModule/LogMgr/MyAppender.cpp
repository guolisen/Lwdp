#include <lwdp.h>
#include "MyAppender.h"

namespace log4cpp {

	MyAppender::MyAppender(NLwdp::MyAppender_Ptr pAppender):        
LayoutAppender(static_cast<NLwdp::tstring>("default"))
    {
    	mApp_ptr = pAppender;
    }
    
    MyAppender::~MyAppender() {
        close();
    }

    void MyAppender::close() {
        // empty
    }

    void MyAppender::_append(const LoggingEvent& event) {
		mApp_ptr->_append(_getLayout().format(event));
    }

    bool MyAppender::reopen() {
        return true;
    }      
}

