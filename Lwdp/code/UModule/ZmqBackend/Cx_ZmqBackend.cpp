#include <Lwdp.h>
#include <PluginInc.h>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>
#include <Interface/ZmqMgr/Ix_ZmqMgr.h>

#include "../Interface/ZmqBackend/Ix_ZmqBackend.h"

#include "../Common/ExternalInterface.h"
#include "../Common/InternalInterface.h"

#include "ZmqBackendDef.h"
#include "ZmqBackendErrno.h"
#include "Cx_ZmqBackend.h"

Cx_ZmqBackend::Cx_ZmqBackend()
{
	printf("ZmqBackend Create!\n");
}

Cx_ZmqBackend::~Cx_ZmqBackend()
{
	printf("ZmqBackend Delete!\n");
}

void* worker_task (void *args)
{
	GET_OBJECT_RET(ZmqMgr, iZmqMgr, 0);
 
	ContextHandle  context = iZmqMgr->GetNewContext();
	SocketHandle responder = iZmqMgr->GetNewSocket(context, LWDP_REP);

	GET_OBJECT_RET(ConfigMgr, iConfigMgr, 0);
	//backend
	std::string strWorkThread = std::string(LW_ZMQBACKEND_WORKTHREAD_TARGET);
	XPropertys  propWorkThread;
	iConfigMgr->GetModulePropEntry(LW_ZMQBACKEND_MODULE_NAME, LW_ZMQBACKEND_WORKTHREAD_TARGET_NAME, propWorkThread);

	if(!propWorkThread[0].propertyText.empty())
	{
		strWorkThread = propWorkThread[0].propertyText;
	}
	else
	{
		LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <ConnetTarget> In Config File, Default(%s)", strWorkThread.c_str());
	}

	iZmqMgr->Connect(responder, strWorkThread.c_str());

	while (1) 
	{
		// Wait for next request from client
		std::string retdata = iZmqMgr->Recv(responder, 0);
		LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::INFO, 
					   "ZMQ Server Received request: [%s]", retdata.c_str());

		// Do some 'work'
		GET_OBJECT_RET(ZmqBackend, iZmqBackend, 0);
		std::string sendData;
		LWRESULT res = iZmqBackend->CallBackZmqMsg(retdata, sendData);
		if(res != LWDP_OK)
		{
			LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::ERR, 
						   "CallBackZmqMsg ret Error(%x)", res);
			continue;
		}

		// Send reply back to client
		iZmqMgr->Send(responder, sendData.data(), sendData.size(), 0);
		Sleep(1);
	}
	// We never get here but clean up anyhow

	iZmqMgr->CloseSocket(responder);
	iZmqMgr->CloseContext(context);
	
	return 0;
}



LWRESULT Cx_ZmqBackend::Init()
{
	mMsgDelegateMap.clear();
	GET_OBJECT_RET(ZmqMgr, iZmqMgr, LWDP_GET_OBJECT_ERROR);
	
	mContext  = iZmqMgr->GetNewContext();
	mFrontend = iZmqMgr->GetNewSocket(mContext, LWDP_ROUTER);
	mBackend  = iZmqMgr->GetNewSocket(mContext, LWDP_DEALER);

	GET_OBJECT_RET(ConfigMgr, iConfigMgr, LWDP_GET_OBJECT_ERROR);
	//frontend
	std::string strFrontend = std::string(LW_ZMQBACKEND_FRONTEND_TARGET);
	XPropertys propFrontend;
	iConfigMgr->GetModulePropEntry(LW_ZMQBACKEND_MODULE_NAME, LW_ZMQBACKEND_FRONTEND_TARGET_NAME, propFrontend);

	if(!propFrontend[0].propertyText.empty())
	{
		strFrontend = propFrontend[0].propertyText;
	}
	else
	{
		LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <FrontendTarget> In Config File, Default(%s)", strFrontend.c_str());
	}

	iZmqMgr->Bind(mFrontend, strFrontend.c_str());

	//backend
	std::string strBackend = std::string(LW_ZMQBACKEND_BACKEND_TARGET);
	XPropertys propBackend;
	iConfigMgr->GetModulePropEntry(LW_ZMQBACKEND_MODULE_NAME, LW_ZMQBACKEND_BACKEND_TARGET_NAME, propBackend);

	if(!propBackend[0].propertyText.empty())
	{
		strBackend = propBackend[0].propertyText;
	}
	else
	{
		LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <BackendTarget> In Config File, Default(%s)", strBackend.c_str());
	}

	iZmqMgr->Bind(mBackend, strBackend.c_str());

	int32_ workThreadNum = LW_ZMQBACKEND_WORKTHREAD_NUM;
	XPropertys propWorkThreadNum;
	iConfigMgr->GetModulePropEntry(LW_ZMQBACKEND_MODULE_NAME, LW_ZMQBACKEND_WORKTHREAD_NUM_NAME, propWorkThreadNum);
	if(!propWorkThreadNum[0].propertyText.empty())
	{
		workThreadNum = atol(propWorkThreadNum[0].propertyText.c_str());
	}
	else
	{
		LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <WorkThreadNum> In Config File, Default(%d)", workThreadNum);
	}

	for(int i = 0; i < workThreadNum; ++i)
	{
		pthread_t t;
		int result;

		result = pthread_create(&t, NULL, worker_task, NULL);
		if(result != 0){
			LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::ERR, 
						   "Can't Create Thread Ret: %d", result);
			return ZMQBACKEND::LWDP_CREATE_WORK_THREAD_ERR;
		}

		result = pthread_detach(t);
		if(result != 0){
			LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::ERR, 
						   "Can't Detach Thread Ret: %d", result);
			return ZMQBACKEND::LWDP_CREATE_DETACH_THREAD_ERR;
		}
	}

	return LWDP_OK;
}



LWRESULT Cx_ZmqBackend::RunServer()
{
	// Initialize poll set
	LWDP_POLLITEM_T items [] = {
	    { mFrontend, 0, LWDP_POLLIN, 0 },
	    { mBackend, 0, LWDP_POLLIN, 0 }
	};

	GET_OBJECT_RET(ZmqMgr, iZmqMgr, LWDP_GET_OBJECT_ERROR);

	// Switch messages between sockets
	while (1) {
		
	    int more = 0; // Multipart detection

	    iZmqMgr->Poll(items, 2, -1);
	    if (items [0].revents & LWDP_POLLIN) 
		{
	        while (1) 
			{
				GET_OBJECT_RET(ZMessage, iZMessage, LWDP_GET_OBJECT_ERROR);
	            // Process all parts of the message
	            iZMessage->InitZMessage();
	            iZmqMgr->Recv(mFrontend, iZMessage, 0);
	            
	            uint32_ more_size = sizeof (more);
				iZmqMgr->Getsockopt(mFrontend, LWDP_RCVMORE, &more, &more_size);
				iZmqMgr->Send(mBackend, iZMessage, more? LWDP_SNDMORE: 0);
                if (!more)
                    break; // Last message part
                Sleep (1); 
	         }
	    }
        if (items [1].revents & LWDP_POLLIN) 
		{
            while (1) 
			{
				GET_OBJECT_RET(ZMessage, iZMessage, LWDP_GET_OBJECT_ERROR);
                // Process all parts of the message
				iZMessage->InitZMessage();
	            iZmqMgr->Recv(mBackend, iZMessage, 0);

                size_t more_size = sizeof (more);
				iZmqMgr->Getsockopt(mBackend, LWDP_RCVMORE, &more, &more_size);
				iZmqMgr->Send(mFrontend, iZMessage, more? LWDP_SNDMORE: 0);
                if (!more)
                    break; // Last message part
                Sleep (1);    
            }
         }
		 Sleep (1);
	}

	return LWDP_OK;
}

LWRESULT Cx_ZmqBackend::DestoryServer()
{
//	iZmqMgr->CloseSocket(mFrontend);
//	iZmqMgr->CloseSocket(mBackend);
//	iZmqMgr->CloseContext(mContext);

	return LWDP_OK;
}

LWRESULT Cx_ZmqBackend::RegisteZmqMsg(uint32_ msg_code, MsgDelegate& msg_delegate)
{
	MSG_DELEGATE_MAP::iterator it= mMsgDelegateMap.find(msg_code);
	if(it != mMsgDelegateMap.end())
	{
		LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::WARNING, 
					   "RegisteZmqMsg Msg(%d) has Exist", msg_code);
		return LWDP_OK;
	}

	mMsgDelegateMap.insert(std::make_pair(msg_code, msg_delegate));

	return LWDP_OK;
}

LWRESULT Cx_ZmqBackend::CallBackZmqMsg(const std::string& recv_msg, std::string& ret_data)
{
	TS_ZMQ_SERVER_MSG* zMsg = recv_msg.data();

	return LWDP_OK;
}

