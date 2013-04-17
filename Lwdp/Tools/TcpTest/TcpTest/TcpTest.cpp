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

#include <iconv.h>


using namespace std;
#define SERVPORT 12135 /*服务器监听端口号*/
#define DEST_IP  "10.3.18.56"
//#define DEST_IP  "127.0.0.1"
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



int StrConvert(char_* from_charset,
				char_* to_charset,
				char * inbuf,
				int    inlen,
				char * outbuf,
				int    outlen)
{
	if(!from_charset || !to_charset || !inbuf || !outbuf || !inlen || !outlen)
		return -1;

	iconv_t cd;
	int rc = 0;
	char **pin  = &inbuf;
	char **pout = &outbuf;

	cd = iconv_open(to_charset, from_charset);
	if(!cd) 
		return -1;
	
	memset(outbuf, 0, outlen);
	if((rc=iconv(cd, (const char **)pin, (size_t *)&inlen, pout, (size_t *)&outlen)) == -1) 
	{
		iconv_close(cd);
		return -1;
	}
	iconv_close(cd);

	return 0;
}


int uIndex = 0;
int Init_Send(int socketFd)
{
	printf("### Init_Send ###\n");
	char buf[MAXDATASIZE];
	int len = sizeof(TS_REQ_SERVER_MSG) + 
			  sizeof(TS_DEVICE_INIT_REQ_BODY);
	TS_REQ_SERVER_MSG* tdata = (TS_REQ_SERVER_MSG*)malloc(len);
	memset(tdata, 0, len);
	tdata->msgLength = htonl(len);
	int devIdx = within(5);
	memcpy(tdata->deviceId, deviceIdSet[devIdx], strlen(deviceIdSet[devIdx]));
	tdata->msgCode = htonl(TS_DEVICE_INIT_REQ_MSG);
	
	TS_DEVICE_INIT_REQ_BODY* body = (TS_DEVICE_INIT_REQ_BODY*)tdata->customMsgBody;
	body->checkResult = htonl(0);
	body->deviceAction = htonl(1);
	body->sceneryId[0] = '1';
	body->sceneryId[1] = '2';
	body->sceneryId[2] = '3';
	body->sceneryId[3] = '4';
	body->sceneryId[4] = '5';
	body->sceneryId[5] = '6';
	body->sceneryId[6] = '7';
	body->sceneryId[7] = '8';

	strcpy(body->cardKey, "AAAAAAAA");
	strcpy(body->checkResultInfo, "I'm OK!");	

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
		TS_RSP_SERVER_MSG* retTcpMsg = (TS_RSP_SERVER_MSG*)buf;
		//TS_SERVER_INIT_RSP_BODY*  retBody = (TS_SERVER_INIT_RSP_BODY*)retTcpMsg->customMsgBody;
		printf( "[Received] LEN: %d RSPCODE: %x, Status Str: %s\n", 
			    ntohl(retTcpMsg->msgLength),
				ntohl(retTcpMsg->rspCode),
				retTcpMsg->rspMsg);
	}

	return 0;
}


int Config_Send(int socketFd)
{
	printf("### Config_Send ###\n");
	char buf[MAXDATASIZE];
	int len = sizeof(TS_REQ_SERVER_MSG);
	TS_REQ_SERVER_MSG* tdata = (TS_REQ_SERVER_MSG*)malloc(len);
	memset(tdata, 0, len);
	tdata->msgLength = htonl(len);

	int devIdx = within(5);
	memcpy(tdata->deviceId, deviceIdSet[devIdx], strlen(deviceIdSet[devIdx]));
	tdata->msgCode = htonl(TS_DEVICE_CONFIG_REQ_MSG);

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
		TS_RSP_SERVER_MSG* retTcpMsg     = (TS_RSP_SERVER_MSG*)buf;
		TS_DEV_CONFIG_RSP_BODY*  retBody = (TS_DEV_CONFIG_RSP_BODY*)retTcpMsg->customMsgBody;

		printf( "[Received] LEN: %d RSPCODE: %x RspMsg: %s\n", 
			    ntohl(retTcpMsg->msgLength),
				ntohl(retTcpMsg->rspCode),
				retTcpMsg->rspMsg);

		printf( "[Received] deviceType: %x\n", ntohl(retBody->deviceAction));
		printf( "[Received] sceneryId: %s\n", std::string((char*)retBody->sceneryId, 8).c_str());
	}

	return 0;
}

#include <time.h>
int Status_Send(int socketFd)
{
	printf("### Status_Send ###\n");
	char buf[MAXDATASIZE];
	int len = sizeof(TS_REQ_SERVER_MSG) + 
			  sizeof(TS_DEV_STATUS_REQ_BODY);
	TS_REQ_SERVER_MSG* tdata = (TS_REQ_SERVER_MSG*)malloc(len);
	memset(tdata, 0, len);
	tdata->msgLength = htonl(len);

	int devIdx = within(5);
	memcpy(tdata->deviceId, deviceIdSet[devIdx], strlen(deviceIdSet[devIdx]));
	tdata->msgCode = htonl(TS_DEVICE_HEART_BEAT_REQ_MSG);

	TS_DEV_STATUS_REQ_BODY* body = (TS_DEV_STATUS_REQ_BODY*)tdata->customMsgBody;
	body->statusCode = htonl(0x1234);

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
		TS_RSP_SERVER_MSG* retTcpMsg     = (TS_RSP_SERVER_MSG*)buf;
		//TS_DEV_STATUS_RSP_BODY*  retBody = (TS_DEV_STATUS_RSP_BODY*)retTcpMsg->customMsgBody;

		printf( "[Received] LEN: %d RSPCODE: %x MSG: %s\n", 
			    ntohl(retTcpMsg->msgLength),
				ntohl(retTcpMsg->rspCode), 
				retTcpMsg->rspMsg);
	}

	return 0;
}

int CardData_Send(int socketFd)
{
	printf("### CardData_Send ###\n");
	char buf[MAXDATASIZE];
	int len = sizeof(TS_REQ_SERVER_MSG) + 
			  sizeof(TS_DEVICE_CARD_DATA_REQ_BODY);
	TS_REQ_SERVER_MSG* tdata = (TS_REQ_SERVER_MSG*)malloc(len);
	memset(tdata, 0, len);
	tdata->msgLength = htonl(len);
	tdata->msgCode   = htonl(TS_DEVICE_CARD_DATA_REQ_MSG);
	
	int devIdx = within(5);
	memcpy(tdata->deviceId, deviceIdSet[devIdx], strlen(deviceIdSet[devIdx]));

	TS_DEVICE_CARD_DATA_REQ_BODY* body = (TS_DEVICE_CARD_DATA_REQ_BODY*)tdata->customMsgBody;

	char tmpStr[100] = {0};
	unsigned int counter = within(100000000);

	_snprintf(tmpStr, 100, "6400010110000001");
	memcpy(body->cardId, tmpStr, strlen(tmpStr));
	memcpy(body->sceneryId, "10011001", 8);
	body->cardType = htons(4);
	body->actionId = htons(within(2));
	body->checkinTime = 0;


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
		TS_RSP_SERVER_MSG* retTcpMsg = (TS_RSP_SERVER_MSG*)buf;

		
		char tmp[128];
		memset(tmp, 0 , 128);
		StrConvert("utf-8","gb2312", retTcpMsg->rspMsg,
				   strlen(retTcpMsg->rspMsg),
				   tmp, sizeof(tmp));

		printf( "[Received] LEN: %d RSPCODE: %x msgResultData: %s\n", ntohl(retTcpMsg->msgLength),
				ntohl(retTcpMsg->rspCode), tmp);

	}

	return 0;
}

int gCardNo = 0;
#define CARD_NUM_SEND 1000
int BulkData_Send(int socketFd)
{
	printf("### BulkData_Send ###\n");

	int len = sizeof(TS_REQ_SERVER_MSG) + 
		      sizeof(TS_DEVICE_BULK_DATA_REQ_BODY) + 
			  sizeof(TS_DEVICE_CARD_DATA_REQ_BODY) * CARD_NUM_SEND;
	TS_REQ_SERVER_MSG* tdata = (TS_REQ_SERVER_MSG*)malloc(len);
	memset(tdata, 0, len);
	tdata->msgLength = htonl(len);
	
	int devIdx = within(5);
	memcpy(tdata->deviceId, deviceIdSet[devIdx], strlen(deviceIdSet[devIdx]));
	tdata->msgCode = htonl(TS_DEVICE_BULK_DATA_REQ_MSG);

	TS_DEVICE_BULK_DATA_REQ_BODY* body = (TS_DEVICE_BULK_DATA_REQ_BODY*)tdata->customMsgBody;
	body->cardDataCount = htonl(CARD_NUM_SEND);
	
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
		tmp[gCardNo].cardType = htons(cardtype);
		tmp[gCardNo].actionId = htons(1);
		tmp[gCardNo].checkinTime = htonl(cttick);

		timect = localtime(&cttick);
		timect->tm_min += 5 + within(30);
		cttick = mktime(timect);

		memcpy(tmp[gCardNo + 1].cardId, tmpStr, strlen(tmpStr));
		memcpy(tmp[gCardNo + 1].sceneryId, "1", 2);
		tmp[gCardNo + 1].cardType = htons(cardtype);
		tmp[gCardNo + 1].actionId = htons(0);
		tmp[gCardNo + 1].checkinTime = htonl(cttick);
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
		TS_RSP_SERVER_MSG* retMsg = (TS_RSP_SERVER_MSG*)recvBuf;
		TS_DEVICE_BULK_DATA_RSP_BODY*  retBody = (TS_DEVICE_BULK_DATA_RSP_BODY*)retMsg->customMsgBody;

		printf( "[Received] LEN: %d RSPCODE: %x msgResultData: %s\n", 
			    ntohl(retMsg->msgLength),
			    ntohl(retMsg->rspCode),
			    retMsg->rspMsg);


		if(retMsg->rspCode == TS_SERVER_BULK_WRITE_DATA_ERROR)
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
		CardData_Send(sockfd);
		//BulkData_Send(sockfd);

		int ti = within(1000);
		Sleep(1000);
		closesocket(sockfd); 
	}
	

 
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