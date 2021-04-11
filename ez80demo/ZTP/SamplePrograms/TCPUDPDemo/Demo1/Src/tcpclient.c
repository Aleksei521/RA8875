
/*
 * File       :     tcpclient.c
 *
 * Description:  This file handles the data transfer between 
				 TCP client and TCP server on eZ80 using sockets.
 *
 * Copyright 2004 ZiLOG Inc.  ALL RIGHTS RESERVED.
 *
 * This file contains unpublished confidential and proprietary information
 * of ZiLOG, Inc.
 * NO PART OF THIS WORK MAY BE DUPLICATED, STORED, PUBLISHED OR DISCLOSED 
 * IN ANY FORM WITHOUT THE PRIOR WRITTEN CONSENT OF ZiLOG, INC.
 * This is not a license and no use of any kind of this work is authorized
 * in the absence of a written license granted by ZiLOG, Inc. in ZiLOG's 
 * sole discretion 
 */


#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#define MAX_BUF_SIZE 4000

char Buffer[MAX_BUF_SIZE];


void main(void)
{
	unsigned short port ;
	int socket_type = SOCK_STREAM;
	struct sockaddr_in server;
	int ret,retval=0,temp =0;
	char serverip[80];
	unsigned int rd = 0;
	WSADATA wsaData;
	SOCKET  conn_socket;
	printf("\nEnter the server's IP address:");
	scanf("%s", serverip);
	printf("\nEnter the server port number:");
	scanf("%d",&port);
	memset(&server,0,sizeof(server));
	server.sin_addr.s_addr=inet_addr(serverip);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	if (WSAStartup(0x202,&wsaData) == SOCKET_ERROR) 
	{
		printf("\nWSAStartup failed with error %d",WSAGetLastError());
		WSACleanup();
		exit(1);
	}	

	conn_socket = socket(AF_INET,socket_type,0); 
	if (conn_socket <0 ) 
	{
		printf("\nClient: Error Opening socket: Error %d\n",WSAGetLastError());
		WSACleanup();
		return ;
			
	}			

	if (connect(conn_socket,(struct sockaddr*)&server,sizeof(server))== SOCKET_ERROR) 
	{
		printf("\nConnect() failed: %d",WSAGetLastError());
		WSACleanup();
		return ;
	}
	
	ret = MAX_BUF_SIZE;
	while(ret>0)
	{
		retval = send(conn_socket,Buffer,ret,0);
		if (retval == SOCKET_ERROR) 
		{
			fprintf(stderr,"send() failed: error %d\n",WSAGetLastError());
			WSACleanup();
			return ;
		}
		ret=ret - retval;
	}
	printf("\n###########################################");
	printf("\nSent  %d bytes data to the server",retval); 
	ret = MAX_BUF_SIZE;
	while(ret>0)
	{
		retval = recv(conn_socket,Buffer,ret,0);
		if (retval == SOCKET_ERROR) 
		{
			fprintf(stderr,"send() failed: error %d\n",WSAGetLastError());
			WSACleanup();
			return ;
		}
		ret=ret - retval;
		temp = temp + retval;
	}
	
	printf("\nReceived  %d bytes data from the server",temp); 
	printf("\n###########################################\n");
	closesocket(conn_socket);

}


