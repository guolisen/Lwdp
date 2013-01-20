#include <LwDp.h>
#include <PluginInc.h>

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>
#include <Interface/ZmqMgr/Ix_ZmqMgr.h>

#include <Interface/CommonUtilMgr/Ix_CommonUtilMgr.h>

#include "../Interface/ZmqBackend/Ix_ZmqBackend.h"
#include "../Interface/TcpServer/Ix_TcpServer.h"

#include "../Common/ExternalInterface.h"
#include "../Common/InternalInterface.h"

#include "ZmqBackendDef.h"
#include "ZmqBackendErrno.h"
#include "Cx_ZmqBackend.h"


ContextHandle Cx_ZmqBackend::mContext;
SocketHandle  Cx_ZmqBackend::mCtrlend;


Cx_ZmqBackend::Cx_ZmqBackend()
{
	printf("ZmqBackend Create!\n");
}

Cx_ZmqBackend::~Cx_ZmqBackend()
{
	printf("ZmqBackend Delete!\n");
}

static void s_set_id (void *socket)
{
	GET_OBJECT_VOID(ZmqMgr, iZmqMgr);
    char identity [10];
    sprintf (identity, "%04X-%04X", randof (0x10000), randof (0x10000));
    iZmqMgr->Setsockopt(socket, LWDP_IDENTITY, identity, strlen (identity));
}

std::string strDbIp 		= std::string(LW_ZMQBACKEND_DB_IP_DEFAULT);
std::string strDbUserName 	= std::string(LW_ZMQBACKEND_DB_USER_DEFAULT);
std::string strDbPassword 	= std::string(LW_ZMQBACKEND_DB_PASSWORD);
std::string strDbName 		= std::string(LW_ZMQBACKEND_DB_SELECT_DBNAME);
uint32_     DbPort 		    = LW_ZMQBACKEND_DB_PORT_DEFAULT;


void* worker_task (void *args)
{
	GET_OBJECT_RET(DbMgr, iDbMgr, 0);	
	LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::INFO, 
				   "Connect to Db Server Ip:%s User:%s DbName:%s Port:%d", 
				   strDbIp.c_str(), strDbUserName.c_str(), strDbName.c_str(), DbPort);
	DBHandle dbHandle = iDbMgr->Open(strDbIp.c_str(), 
		                             strDbUserName.c_str(), 
		                             strDbPassword.c_str(), 
		                             strDbName.c_str(), 
		                             DbPort, 0);
	
	if(!dbHandle)
	{
		LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::INFO, 
				       "Worke Thread Connect Db Error!");
		return NULL;
	}
	
	LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::INFO, 
			       "Worke Thread Connect Db Ok!");


	GET_OBJECT_RET(ZmqMgr, iZmqMgr, 0);

 	////////////////////////////////////////////////
	ContextHandle  context = (ContextHandle)Cx_ZmqBackend::mContext;
	SocketHandle responder = iZmqMgr->GetNewSocket(context, LWDP_REP);
	s_set_id(responder);

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
	if(iZmqMgr->Connect(responder, strWorkThread.c_str()) != LWDP_OK)
	{
		LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::ERR, 
					   "Connect to Backend Error(%s)", strWorkThread.c_str());
		return NULL;
	}

	/////////////////////////////////////////////
	SocketHandle ctrlClient = iZmqMgr->GetNewSocket(context, LWDP_SUB);
	s_set_id(ctrlClient);
	//ctrl_client
	std::string strCtrlClient = std::string(LW_ZMQBACKEND_CTRL_CLIENT_TARGET);
	XPropertys  propCtrlClient;
	iConfigMgr->GetModulePropEntry(LW_ZMQBACKEND_MODULE_NAME, LW_ZMQBACKEND_CTRL_CLIENT_NAME, propCtrlClient);

	if(!propCtrlClient[0].propertyText.empty())
	{
		strCtrlClient = propCtrlClient[0].propertyText;
	}
	else
	{
		LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <CtrlConnect> In Config File, Default(%s)", strCtrlClient.c_str());
	}
	
	if(iZmqMgr->Connect(ctrlClient, strCtrlClient.c_str()) != LWDP_OK)
	{
		LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::ERR, 
					   "Connect to Backend Ctrl Error(%s)", strCtrlClient.c_str());
		return NULL;
	}
	iZmqMgr->Setsockopt(ctrlClient, LWDP_SUBSCRIBE, "", 0);

	// Initialize poll set
	LWDP_POLLITEM_T items [] = {
	    { responder, 0, LWDP_POLLIN, 0 },
	    { ctrlClient, 0, LWDP_POLLIN, 0 }
	};

	// Switch messages between sockets
	Cx_Interface<Ix_ZMessage> iZMessage;
    int more = 0; // Multipart detection
    uint32_ more_size = sizeof (more);
	while (1) {
		more = 0;
	    iZmqMgr->Poll(items, 2, -1);
	    if (items [0].revents & LWDP_POLLIN) 
		{
			while (1) 
			{
				GET_OBJECT_RET(ZMessage, iTmpMsg, 0);
				// Wait for next request from client
		        // Process all parts of the message
				iTmpMsg->InitZMessage();
		        iZmqMgr->Recv(responder, iTmpMsg, 0);
				
		        LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::DEBUG,
		       	               "Work Thread Revc(%d)", iTmpMsg->Size());
				iZmqMgr->Getsockopt(responder, LWDP_RCVMORE, &more, &more_size);
				if (!more)
				{
					iZMessage = iTmpMsg;
				    break; // Last message part
				}
				//Api_TaskDelay (1); 
			}

			
			LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::NOTICE, 
						   "ZMQ Server Received request: [%d]", iZMessage->Size());

			// Do some 'work'
			GET_OBJECT_RET(ZmqBackend, iZmqBackend, 0);
			Data_Ptr sendData;
			sendData.reset();
			uint32_ sendLen = 0;
			LWRESULT res = iZmqBackend->CallBackZmqMsg(dbHandle, (uint8_*)iZMessage->Data(), iZMessage->Size(), sendData, sendLen);
			if(res != LWDP_OK)
			{
				LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::ERR, 
							   "CallBackZmqMsg ret Error(%x)", res);
				//continue;
			}

			if(sendLen < 0)
			{
				LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::ERR, 
							   "Zmq Send Data Length is too Small(%d)", sendLen);

				//continue;
			}
			// Send reply back to client
			GET_OBJECT_RET(ZMessage, iZSMessage, 0);
			iZSMessage->InitZMessage();
			iZSMessage->SetValue(sendData.get(), sendLen);
			iZmqMgr->Send(responder, iZSMessage, 0);
			
			//Api_TaskDelay(1);
	    }
		if (items [1].revents & LWDP_POLLIN) 
		{
			while (1) 
			{
				GET_OBJECT_RET(ZMessage, iTmpMsg, 0);
				// Wait for next request from client
		        // Process all parts of the message
				iTmpMsg->InitZMessage();
		        iZmqMgr->Recv(ctrlClient, iTmpMsg, 0);
				
		        //LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::DEBUG,
		       	//               "Work Thread Revc(%d)", iTmpMsg->Size());
				iZmqMgr->Getsockopt(ctrlClient, LWDP_RCVMORE, &more, &more_size);
				if (!more)
				{
					iZMessage = iTmpMsg;
				    break; // Last message part
				}
				//Api_TaskDelay (1); 
			}

			// Do some 'work'
			GET_OBJECT_RET(ZmqBackend, iZmqBackend, 0);
			iZmqBackend->CallBackCtrl((const char_ *)iZMessage->Data(), iZMessage->Size());
		
		}
	}

	iZmqMgr->CloseSocket(responder);
	iZmqMgr->CloseContext(context);
	
	return 0;
}

LWRESULT Cx_ZmqBackend::Init()
{
	mMsgDelegateMap.clear();
	GET_OBJECT_RET(ZmqMgr, iZmqMgr, LWDP_GET_OBJECT_ERROR);
	
	Cx_ZmqBackend::mContext  = iZmqMgr->GetNewContext();
	mFrontend = iZmqMgr->GetNewSocket(Cx_ZmqBackend::mContext, LWDP_ROUTER);
	mBackend  = iZmqMgr->GetNewSocket(Cx_ZmqBackend::mContext, LWDP_DEALER);

	GET_OBJECT_RET(ConfigMgr, iConfigMgr, LWDP_GET_OBJECT_ERROR);
	
	XPropertys propDbIp;
	iConfigMgr->GetModulePropEntry(LW_ZMQBACKEND_MODULE_NAME, LW_ZMQBACKEND_DB_IP_NAME, propDbIp);
	if(!propDbIp[0].propertyText.empty())
	{
		strDbIp = propDbIp[0].propertyText;
	}
	else
	{
		LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <DbIp> In Config File, Default(%s)", strDbIp.c_str());
	}

	XPropertys propDbUserName;
	iConfigMgr->GetModulePropEntry(LW_ZMQBACKEND_MODULE_NAME, LW_ZMQBACKEND_DB_USER_NAME, propDbUserName);
	if(!propDbUserName[0].propertyText.empty())
	{
		strDbUserName = propDbUserName[0].propertyText;
	}
	else
	{
		LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <DbUser> In Config File, Default(%s)", strDbUserName.c_str());
	}

	XPropertys propDbPassword;
	iConfigMgr->GetModulePropEntry(LW_ZMQBACKEND_MODULE_NAME, LW_ZMQBACKEND_DB_PASSWORD_NAME, propDbPassword);
	if(!propDbPassword[0].propertyText.empty())
	{
		strDbPassword = propDbPassword[0].propertyText;
	}
	else
	{
		LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <DbPassword> In Config File, Default(%s)", strDbPassword.c_str());
	}

	XPropertys propDbName;
	iConfigMgr->GetModulePropEntry(LW_ZMQBACKEND_MODULE_NAME, LW_ZMQBACKEND_DB_SELECT_DB_NAME, propDbName);
	if(!propDbName[0].propertyText.empty())
	{
		strDbName = propDbName[0].propertyText;
	}
	else
	{
		LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <DbName> In Config File, Default(%s)", strDbName.c_str());
	}

	XPropertys propDbPort;
	iConfigMgr->GetModulePropEntry(LW_ZMQBACKEND_MODULE_NAME, LW_ZMQBACKEND_DB_PORT_NAME, propDbPort);
	if(!propDbPort[0].propertyText.empty())
	{
		DbPort = atol(propDbPort[0].propertyText.c_str());
	}
	else
	{
		LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <DbPort> In Config File, Default(%d)", DbPort);
	}	

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
					   "Can't Find <BackendTarget> In Config File, Default(%s)",
					   strBackend.c_str());
	}

	iZmqMgr->Bind(mBackend, strBackend.c_str());


	//Ctrl
	Cx_ZmqBackend::mCtrlend = iZmqMgr->GetNewSocket(Cx_ZmqBackend::mContext, LWDP_PUB);
	//ctrl_client
	std::string strCtrlClient = std::string(LW_ZMQBACKEND_CTRL_CLIENT_TARGET);
	XPropertys  propCtrlClient;
	iConfigMgr->GetModulePropEntry(LW_ZMQBACKEND_MODULE_NAME, LW_ZMQBACKEND_CTRL_CLIENT_NAME, propCtrlClient);

	if(!propCtrlClient[0].propertyText.empty())
	{
		strCtrlClient = propCtrlClient[0].propertyText;
	}
	else
	{
		LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::WARNING, 
					   "Can't Find <CtrlConnect> In Config File, Default(%s)", 
					   strCtrlClient.c_str());
	}
	
	if(iZmqMgr->Bind(Cx_ZmqBackend::mCtrlend, strCtrlClient.c_str()) != LWDP_OK)
	{
		LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::ERR, 
					   "Bind Ctrl Error(%s)", 
					   strCtrlClient.c_str());
		return NULL;
	}


	Cx_Interface<Ix_ConsoleMgr> iConsoleMgr(CLSID_ConsoleMgr);
	if(!iConsoleMgr.IsNull())
	{
		ConsoleCBDelegate regFun = MakeDelegate(this, &Cx_ZmqBackend::ConsoleSendToWorker);
		RINOK(iConsoleMgr->RegisteCommand(LW_ZMQBACKEND_COMMAND_SENDTOWORKER_NAME, regFun,
				                          LW_ZMQBACKEND_COMMAND_SENDTOWORKER_INFO));
	}


	//////////////////////////////////////////////
	// thread 
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
    int more = 0; // Multipart detection
    uint32_ more_size = sizeof (more);
	while (1) {
		more = 0;
	    iZmqMgr->Poll(items, 2, -1);
	    if (items [0].revents & LWDP_POLLIN) 
		{
	        while (1) 
			{
				GET_OBJECT_RET(ZMessage, iZMessage, LWDP_GET_OBJECT_ERROR);

	            // Process all parts of the message
	        	iZMessage->InitZMessage();
	            iZmqMgr->Recv(mFrontend, iZMessage, 0);

	            LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::DEBUG,
	            			   "Revc From Tcp Frontend(%d)", iZMessage->Size());
				iZmqMgr->Getsockopt(mFrontend, LWDP_RCVMORE, &more, &more_size);
				iZmqMgr->Send(mBackend, iZMessage, more? LWDP_SNDMORE: 0);
                if (!more)
                    break; // Last message part
                Api_TaskDelay (1); 
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

	            //LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::DEBUG,
	            //	           "Revc From Zmq Backend(%d)", iZMessage->Size());
				iZmqMgr->Getsockopt(mBackend, LWDP_RCVMORE, &more, &more_size);
				iZmqMgr->Send(mFrontend, iZMessage, more? LWDP_SNDMORE: 0);
                if (!more)
                    break; // Last message part
                Api_TaskDelay (1);    
            }
         }
		 Api_TaskDelay (1);
	}

	return LWDP_OK;
}

LWRESULT Cx_ZmqBackend::DestoryServer()
{
//	iZmqMgr->CloseSocket(mFrontend);
//	iZmqMgr->CloseSocket(mBackend);
//	iZmqMgr->CloseContext(Cx_ZmqBackend::mContext);

	return LWDP_OK;
}

LWRESULT Cx_ZmqBackend::RegisteZmqMsg(uint32_ msg_code, MsgDelegate msg_delegate)
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

LWRESULT Cx_ZmqBackend::CallBackZmqMsg(DBHandle dbHandle, const uint8_* recv_msg, uint32_ recv_msg_len, 
										     Data_Ptr& ret_data ,uint32_& ret_data_len)
{
	if(!recv_msg)
	{
		LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::ERR, 
					   "Cx_ZmqBackend::CallBackZmqMsg Param <recv_msg> is NULL");
		return LWDP_PARAMETER_ERROR;
	}
	TS_ZMQ_SERVER_MSG* zMsg = (TS_ZMQ_SERVER_MSG*)recv_msg;
	MSG_DELEGATE_MAP::iterator it= mMsgDelegateMap.find(zMsg->msgCode);
	if(it != mMsgDelegateMap.end())
	{
		return it->second(dbHandle, recv_msg, recv_msg_len, ret_data, ret_data_len);
	}

	LWDP_LOG_PRINT("ZMQBACKEND", LWDP_LOG_MGR::WARNING, 
			       "Unknow Request Msg(%x)", zMsg->msgCode);
	
	uint8_* errMsg = new uint8_[sizeof(TS_ZMQ_SERVER_MSG) + sizeof(TS_SERVER_ERROR_BODY)] ;
	memset(errMsg, 0, sizeof(TS_ZMQ_SERVER_MSG) + sizeof(TS_SERVER_ERROR_BODY));
	TS_ZMQ_SERVER_MSG* errStru = (TS_ZMQ_SERVER_MSG*)errMsg;
	memcpy(errStru->deviceId, zMsg->deviceId, sizeof(errStru->deviceId));
	errStru->msgCode  = (uint32_)TS_SERVER_UNKNOW_MSG;
	TS_SERVER_ERROR_BODY* errBody = (TS_SERVER_ERROR_BODY*)errStru->customMsgBody;
	errBody->errMsgCode = zMsg->msgCode;
	memcpy(errBody->errData, "Unknow Msg", 11);

	Data_Ptr tmpData(errMsg);
	ret_data     = tmpData;
	ret_data_len = sizeof(TS_ZMQ_SERVER_MSG) + sizeof(TS_SERVER_ERROR_BODY);
	return LWDP_OK;

}

LWRESULT Cx_ZmqBackend::CallBackCtrl(const char_* command_str, uint32_ str_len)
{
	if(!command_str || !str_len)
	{
		return LWDP_ERROR;
	}

	COMMAND_LINE tmpCmdLine;
	Cx_Interface<Ix_ConsoleMgr> iConsoleMgr(CLSID_ConsoleMgr);
	if(!iConsoleMgr.IsNull())
	{
		RINOK(iConsoleMgr->PraseCommandLine(command_str, tmpCmdLine, "@"));
	}

	std::string command = tmpCmdLine[1];
	if(command == "info")
	{
		printf("bingo!\n");
	}

	return LWDP_OK;
}

int32_ Cx_ZmqBackend::ConsoleSendToWorker(COMMAND_LINE& command_line)
{
	GET_OBJECT_RET(ZmqMgr, iZmqMgr, LWDP_GET_OBJECT_ERROR);

	COMMAND_LINE::iterator iter; 
	std::string sendBuf("");
	FOREACH_STL(iter, command_line)
	{
		sendBuf.append(*iter);
		sendBuf.append("@");
	}
	
	LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::DEBUG, 
				   "Command msg(%s) ret(%d)", 
				   sendBuf.c_str());

	int32_ zmq_ret_len = iZmqMgr->Send(Cx_ZmqBackend::mCtrlend, sendBuf.data(), sendBuf.size(), 0);
	if(zmq_ret_len != sendBuf.size())
	{	
		LWDP_LOG_PRINT("TSFRONTEND", LWDP_LOG_MGR::ERR, 
					   "Send Ctrl to WorkThread Timeout or Error msg(%s) ret(%d)", 
					   sendBuf.c_str(), zmq_ret_len);

		return LWDP_ERROR;		
	}

	return LWDP_OK;
}




