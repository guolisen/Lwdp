// TcpTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstdlib>  
#include <iostream>  
#include <time.h>  
#include <stdio.h> 
#include <winsock2.h>
#include <windows.h>
#include <process.h>
#include <iostream>
#include "def.h"
#include "interface.h"


using namespace std;
#define SERVPORT 12135 /*服务器监听端口号*/
#define DEST_IP  "127.0.0.1"
#define MAXDATASIZE 1024

#define within(num) (int) ((float) (num) * rand () / (RAND_MAX + 1.0))
#define S_LEN 1024
typedef int (*msgFun)(int);

int Init_Send(int sockfd);
int Config_Send(int sockfd);
int Status_Send(int sockfd);
int CardData_Send(int sockfd);
int BulkData_Send(int sockfd);

msgFun msgSet[] = {Init_Send, Config_Send, Status_Send, CardData_Send, BulkData_Send};
char* deviceIdSet[] = {"1001100000001", "1001100000002", "1001100000003", "1001100000004", "1001100000005"};


int uIndex = 0;
int Init_Send(int socketFd)
{
	printf("### Init_Send ###\n");
	char buf[MAXDATASIZE];
	int len = sizeof(TS_TCP_SERVER_MSG) + 
		      sizeof(TS_ZMQ_SERVER_MSG) + 
			  sizeof(TS_DEVICE_INIT_REQ_BODY);
	TS_TCP_SERVER_MSG* tdata = (TS_TCP_SERVER_MSG*)malloc(len);
	memset(tdata, 0, len);
	tdata->msgLength = len;
	tdata->statusCode = 0;
	TS_ZMQ_SERVER_MSG* data = (TS_ZMQ_SERVER_MSG*)tdata->tcpMsgBody;

	int devIdx = within(5);
	memcpy(data->deviceId, deviceIdSet[devIdx], strlen(deviceIdSet[devIdx]));
	data->msgCode = TS_DEVICE_INIT_REQ_MSG;
	TS_DEVICE_INIT_REQ_BODY* body = (TS_DEVICE_INIT_REQ_BODY*)data->customMsgBody;
	body->checkResult = 0;
	body->deviceType = 1;
	body->sceneryId[0] = '1';
	body->sceneryId[1] = '2';
	body->sceneryId[2] = '3';
	body->sceneryId[3] = '4';
	body->sceneryId[4] = '5';
	body->sceneryId[5] = '6';
	body->sceneryId[6] = '7';
	body->sceneryId[7] = '8';

	body->position = 12;
	if (send(socketFd, (char *)tdata, len, 0) == -1)		
	{
		int errn = WSAGetLastError();
		if(errn == WSAEWOULDBLOCK)
		{
			Sleep(1);
			free(tdata);
			return -1;
		}

		cout<<errn<<endl;
		closesocket(socketFd); 
		Sleep(1000);
		free(tdata);
		return -1;
	}
	else
	{
		//cout<<uIndex++<<":Dev init TCP package is sended"<<endl;
		
	}

	free(tdata);
	int recvbytes=0;
	if ((recvbytes=recv(socketFd, buf, MAXDATASIZE, 0)) !=-1)
	{ 
		TS_TCP_SERVER_MSG* retTcpMsg = (TS_TCP_SERVER_MSG*)buf;
		TS_ZMQ_SERVER_MSG* retMsg = (TS_ZMQ_SERVER_MSG*)retTcpMsg->tcpMsgBody;
		TS_SERVER_INIT_RSP_BODY*  retBody = (TS_SERVER_INIT_RSP_BODY*)retMsg->customMsgBody;
		printf( "[Received] LEN: %d REQ:%x REQCODE: %x, res: %x Status Str: %s\n", retTcpMsg->msgLength,
				retMsg->deviceId, retMsg->msgCode, retBody->msgResult, retBody->msgResultData);
	}

	return 0;
}


int Config_Send(int socketFd)
{
	printf("### Config_Send ###\n");
	char buf[MAXDATASIZE];
	int len = sizeof(TS_TCP_SERVER_MSG) + 
		      sizeof(TS_ZMQ_SERVER_MSG);
	TS_TCP_SERVER_MSG* tdata = (TS_TCP_SERVER_MSG*)malloc(len);
	memset(tdata, 0, len);
	tdata->msgLength = len;
	tdata->statusCode = 0;
	TS_ZMQ_SERVER_MSG* data = (TS_ZMQ_SERVER_MSG*)tdata->tcpMsgBody;

	int devIdx = within(5);
	memcpy(data->deviceId, deviceIdSet[devIdx], strlen(deviceIdSet[devIdx]));
	data->msgCode = TS_DEVICE_CONFIG_REQ_MSG;

	if (send(socketFd, (char *)tdata, len, 0) == -1)		
	{
		int errn = WSAGetLastError();
		if(errn == WSAEWOULDBLOCK)
		{
			Sleep(1);
			free(tdata);
			return -1;
		}

		cout<<errn<<endl;
		closesocket(socketFd); 
		Sleep(1000);
		free(tdata);
		return -1;
	}
	else
	{
		//cout<<uIndex++<<":Dev init TCP package is sended"<<endl;
		
	}

	free(tdata);
	int recvbytes = 0;
	if ((recvbytes=recv(socketFd, buf, MAXDATASIZE, 0)) !=-1)
	{ 
		TS_TCP_SERVER_MSG* retTcpMsg = (TS_TCP_SERVER_MSG*)buf;
		TS_ZMQ_SERVER_MSG* retMsg = (TS_ZMQ_SERVER_MSG*)retTcpMsg->tcpMsgBody;
		TS_DEV_CONFIG_RSP_BODY*  retBody = (TS_DEV_CONFIG_RSP_BODY*)retMsg->customMsgBody;

		printf( "[Received] LEN: %d REQ:%s REQCODE: %x\n", retTcpMsg->msgLength,
				std::string((char*)retBody->deviceId, sizeof(retBody->deviceId)).c_str(), retMsg->msgCode);

		printf( "[Received] res: %x\n", retBody->msgResult);
		printf( "[Received] msgResultData: %s\n", retBody->msgResultData);
		printf( "[Received] deviceId: %s\n", std::string((char*)retBody->deviceId, sizeof(retBody->deviceId)).c_str());
		printf( "[Received] deviceType: %x\n", retBody->deviceType);
		printf( "[Received] sceneryId: %s\n", std::string((char*)retBody->sceneryId, 8).c_str());
		
		printf( "[Received] sceneryPostion: %x\n", retBody->sceneryPostion);

	}

	return 0;
}
#include <time.h>
int Status_Send(int socketFd)
{
	printf("### Status_Send ###\n");
	char buf[MAXDATASIZE];
	int len = sizeof(TS_TCP_SERVER_MSG) + 
		      sizeof(TS_ZMQ_SERVER_MSG) + 
			  sizeof(TS_DEV_STATUS_REQ_BODY);
	TS_TCP_SERVER_MSG* tdata = (TS_TCP_SERVER_MSG*)malloc(len);
	memset(tdata, 0, len);
	tdata->msgLength = len;
	tdata->statusCode = 0;
	TS_ZMQ_SERVER_MSG* data = (TS_ZMQ_SERVER_MSG*)tdata->tcpMsgBody;

	int devIdx = within(5);
	memcpy(data->deviceId, deviceIdSet[devIdx], strlen(deviceIdSet[devIdx]));
	data->msgCode = TS_DEVICE_HEART_BEAT_REQ_MSG;

	TS_DEV_STATUS_REQ_BODY* body = (TS_DEV_STATUS_REQ_BODY*)data->customMsgBody;
	body->statusCode = 0x1234;

	if (send(socketFd, (char *)tdata, len, 0) == -1)		
	{
		int errn = WSAGetLastError();
		if(errn == WSAEWOULDBLOCK)
		{
			Sleep(1);
			free(tdata);
			return -1;
		}

		cout<<errn<<endl;
		closesocket(socketFd); 
		Sleep(1000);
		free(tdata);
		return -1;
	}
	else
	{
		//cout<<uIndex++<<":Dev init TCP package is sended"<<endl;
		
	}

	free(tdata);
	int recvbytes = 0;
	if ((recvbytes=recv(socketFd, buf, MAXDATASIZE, 0)) !=-1)
	{ 
		TS_TCP_SERVER_MSG* retTcpMsg = (TS_TCP_SERVER_MSG*)buf;
		TS_ZMQ_SERVER_MSG* retMsg = (TS_ZMQ_SERVER_MSG*)retTcpMsg->tcpMsgBody;
		TS_DEV_STATUS_RSP_BODY*  retBody = (TS_DEV_STATUS_RSP_BODY*)retMsg->customMsgBody;

		printf( "[Received] LEN: %d REQ:%x REQCODE: %x\n", retTcpMsg->msgLength,
				retMsg->deviceId, retMsg->msgCode);
	}

	return 0;
}

int CardData_Send(int socketFd)
{
	printf("### CardData_Send ###\n");
	char buf[MAXDATASIZE];
	int len = sizeof(TS_TCP_SERVER_MSG) + 
		      sizeof(TS_ZMQ_SERVER_MSG) + 
			  sizeof(TS_DEVICE_CARD_DATA_REQ_BODY);
	TS_TCP_SERVER_MSG* tdata = (TS_TCP_SERVER_MSG*)malloc(len);
	memset(tdata, 0, len);
	tdata->msgLength = len;
	tdata->statusCode = 0;
	TS_ZMQ_SERVER_MSG* data = (TS_ZMQ_SERVER_MSG*)tdata->tcpMsgBody;

	int devIdx = within(5);
	memcpy(data->deviceId, deviceIdSet[devIdx], strlen(deviceIdSet[devIdx]));
	data->msgCode = TS_DEVICE_CARD_DATA_REQ_MSG;

	TS_DEVICE_CARD_DATA_REQ_BODY* body = (TS_DEVICE_CARD_DATA_REQ_BODY*)data->customMsgBody;

	char tmpStr[100] = {0};
	unsigned int counter = within(100000000);
	_snprintf(tmpStr, 100, "10011%08d", counter);
	memcpy(body->cardId, tmpStr, strlen(tmpStr));
	memcpy(body->sceneryId, "12345678", 8);
	body->cardType = 0x12;
	body->actionId = within(2);
	body->checkinTime = time(&body->checkinTime);


	if (send(socketFd, (char *)tdata, len, 0) == -1)		
	{
		int errn = WSAGetLastError();
		if(errn == WSAEWOULDBLOCK)
		{
			Sleep(1);
			free(tdata);
			return -1;
		}

		//cout<<errn<<endl;
		closesocket(socketFd); 
		Sleep(1000);
		free(tdata);
		return -1;
	}
	else
	{
	//	cout<<uIndex++<<":Dev init TCP package is sended"<<endl;
		
	}

	free(tdata);
	int recvbytes = 0;
	if ((recvbytes=recv(socketFd, buf, MAXDATASIZE, 0)) !=-1)
	{ 
		TS_TCP_SERVER_MSG* retTcpMsg = (TS_TCP_SERVER_MSG*)buf;
		TS_ZMQ_SERVER_MSG* retMsg = (TS_ZMQ_SERVER_MSG*)retTcpMsg->tcpMsgBody;
		TS_DEVICE_CARD_DATA_RSP_BODY*  retBody = (TS_DEVICE_CARD_DATA_RSP_BODY*)retMsg->customMsgBody;

		printf( "[Received] LEN: %d REQ:%x REQCODE: %x msgResult: %x msgResultData: %s\n", retTcpMsg->msgLength,
				retMsg->deviceId, retMsg->msgCode, retBody->msgResult, retBody->msgResultData);

	}

	return 0;
}

int gCardNo = 0;
#define CARD_NUM_SEND 1000
int BulkData_Send(int socketFd)
{
	printf("### BulkData_Send ###\n");

	int len = sizeof(TS_TCP_SERVER_MSG) + 
		      sizeof(TS_ZMQ_SERVER_MSG) +
		      sizeof(TS_DEVICE_BULK_DATA_REQ_BODY) + 
			  sizeof(TS_DEVICE_CARD_DATA_REQ_BODY) * CARD_NUM_SEND;
	TS_TCP_SERVER_MSG* tdata = (TS_TCP_SERVER_MSG*)malloc(len);
	memset(tdata, 0, len);
	tdata->msgLength = len;
	tdata->statusCode = 0;
	TS_ZMQ_SERVER_MSG* data = (TS_ZMQ_SERVER_MSG*)tdata->tcpMsgBody;

	int devIdx = within(5);
	memcpy(data->deviceId, deviceIdSet[devIdx], strlen(deviceIdSet[devIdx]));
	data->msgCode = TS_DEVICE_BULK_DATA_REQ_MSG;

	TS_DEVICE_BULK_DATA_REQ_BODY* body = (TS_DEVICE_BULK_DATA_REQ_BODY*)data->customMsgBody;
	body->cardDataCount = CARD_NUM_SEND;
	
	TS_DEVICE_CARD_DATA_REQ_BODY* tmp = (TS_DEVICE_CARD_DATA_REQ_BODY*)body->cardDataEntry;



	for(int gCardNo = 0; gCardNo<CARD_NUM_SEND; )
	{
		int cardtype = within(2);
		struct tm* timect;
		time_t cttick = time(NULL);
		timect = localtime(&cttick);
		timect->tm_mday -= 1;
		timect->tm_min += within(10);
		cttick = mktime(timect);

		char tmpStr[100] = {0};
		unsigned int counter = within(100000000);
		_snprintf(tmpStr, 100, "10011%08d", gCardNo);
		memcpy(tmp[gCardNo].cardId, tmpStr, strlen(tmpStr));
		memcpy(tmp[gCardNo].sceneryId, "1", 2);
		tmp[gCardNo].cardType = cardtype;
		tmp[gCardNo].actionId = 1;
		tmp[gCardNo].checkinTime = cttick;

		timect = localtime(&cttick);
		timect->tm_min += 5 + within(30);
		cttick = mktime(timect);

		memcpy(tmp[gCardNo + 1].cardId, tmpStr, strlen(tmpStr));
		memcpy(tmp[gCardNo + 1].sceneryId, "1", 2);
		tmp[gCardNo + 1].cardType = cardtype;
		tmp[gCardNo + 1].actionId = 0;
		tmp[gCardNo + 1].checkinTime = cttick;
		gCardNo += 2;
	}

	if (send(socketFd, (char *)tdata, len, 0) == -1)		
	{
		int errn = WSAGetLastError();
		if(errn == WSAEWOULDBLOCK)
		{
			Sleep(1);
			free(tdata);
			return -1;
		}

		cout<<errn<<endl;
		closesocket(socketFd); 
		Sleep(1000);
		free(tdata);
		return -1;
	}
	else
	{
		//cout<<uIndex++<<":Dev init TCP package is sended"<<endl;
		
	}

	free(tdata);
	int recvbytes = 0;

	uint32_ indexRecv = 0;
    uint32_ totleSize = 1024*1024;
	uint8_* recvBuf = (uint8_*)malloc(totleSize * sizeof(uint8_));

	while(1)
	{
		if ((recvbytes=recv(socketFd, (char *)recvBuf+indexRecv, totleSize-indexRecv, 0)) == -1)
		{ 
			printf("Connect Interrupt!\n");
			break;
		}
		else if(!recvbytes)
		{
			printf("Recv End!\n");
            break;
		}

		if(recvbytes)
		{
			indexRecv += recvbytes;
			continue;
		}
		break;
	}

	if(indexRecv || recvbytes > 0)
	{
		TS_TCP_SERVER_MSG* retTcpMsg = (TS_TCP_SERVER_MSG*)recvBuf;
		if(retTcpMsg->statusCode)
		{
			printf("Recv TCP Error Message code (%d)\n", retTcpMsg->statusCode);
			return 0;
		}
		TS_ZMQ_SERVER_MSG* retMsg = (TS_ZMQ_SERVER_MSG*)retTcpMsg->tcpMsgBody;
		TS_DEVICE_BULK_DATA_RSP_BODY*  retBody = (TS_DEVICE_BULK_DATA_RSP_BODY*)retMsg->customMsgBody;

		printf( "[Received] LEN: %d REQ:%x REQCODE: %x msgResult: %x msgResultData: %s\n", retTcpMsg->msgLength,
				retMsg->deviceId, retMsg->msgCode, retBody->msgResult, retBody->msgResultData);


		if(retBody->msgResult != 0)
		{
			char* errCardPtr = (char*)(retBody->errCardId);
			for(int i = 0; i < retBody->errorEntryNum; ++i)
			{
				std::string tmps((char*)(errCardPtr + 32*i), 32);
				printf( "[Error] %s\n", tmps.c_str());
			}
		}

	}

	free(recvBuf);
	return 0;
}



unsigned int __stdcall threadfun(void* arg)
{
	int sockfd, recvbytes; 
	char buf[MAXDATASIZE];
	struct hostent *host; 
	struct sockaddr_in serv_addr; 
	struct sockaddr_in dest_addr; /* 目的地址*/  



	while (1)
	//for(int i = 0; i<20; i++)
	{
		if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){ 
			perror("socket创建出错！");
			exit(1);
		}
		serv_addr.sin_family=AF_INET; 
		serv_addr.sin_port=htons(SERVPORT); 
		serv_addr.sin_addr.s_addr = inet_addr(DEST_IP);  
		memset( &(serv_addr.sin_zero),0,8);
		if (connect(sockfd, (struct sockaddr *) &serv_addr, \
			sizeof(struct sockaddr)) == -1) { 
				perror("connect出错！");
				//exit(1);
		}
		char szMsg[] = "hao";
		int nlen = sizeof(serv_addr);

		//int msgIndx = within(5);
		//msgFun runMsg = msgSet[msgIndx];
		
		//runMsg(sockfd);

		//Init_Send(sockfd);
		//Config_Send(sockfd);
		//Status_Send(sockfd);
		//CardData_Send(sockfd);
		BulkData_Send(sockfd);

		Sleep(2000);
		
	}
	
	closesocket(sockfd); 
 
	return 0;
}



int main()
{
	srand(time(0));

		WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(2,0);
	err = WSAStartup(wVersionRequested,&wsaData);
	if (0 != err)
	{
		cout<<"Socket failed";
		return 0;

	}
	if (LOBYTE(wsaData.wVersion)!= 2 || HIBYTE(wsaData.wVersion) != 0)
	{
		WSACleanup();
		return 0;
	}

    int client_nbr;
    for (client_nbr = 0; client_nbr < 1; client_nbr++) {
        HANDLE client;
        client = (HANDLE) _beginthreadex (NULL, 0,
        threadfun, 0, 0 , NULL);
        if (client == 0) {
            printf ("error in _beginthreadex\n");
            return -1;
        }
		Sleep(50);
    }

	system("pause");
	return 0;
}