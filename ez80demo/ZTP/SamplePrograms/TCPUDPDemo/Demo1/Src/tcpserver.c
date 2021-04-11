/*
 * File       :     tcpserver.c
 *
 * Description:  This file handles the data transfer between 
				 TCP server and TCP client on eZ80 using sockets.
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
	unsigned short port;
	
	struct sockaddr_in server,from;
	int retval=0,fromlen,ret,temp =0;
	unsigned int rd = 0;
	WSADATA wsaData;
	SOCKET  socketfd,conn_socket;
	if (WSAStartup(0x202,&wsaData) == SOCKET_ERROR) 
	{
		printf("WSAStartup failed with error %d\n",WSAGetLastError());
		WSACleanup();
		exit(1);
	}
	printf("\nEnter the Port number to Listen:");
	scanf("%d",&port);
	memset(&server,0,sizeof(server));
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	socketfd = socket(AF_INET,SOCK_STREAM,0); 
	if (socketfd <0 ) 
	{
		printf("\nError Opening socket: Error %d",WSAGetLastError());
		WSACleanup();
		return ;
	}			
	if( bind(socketfd,(struct sockaddr*)&server, sizeof(server) ) == SOCKET_ERROR)
	{
		fprintf(stderr, "bind() failed with eorror %d\n", WSAGetLastError());
		WSACleanup();
		getch();
		return ;
	}
	printf("\n###########################################");
	printf("\nServer is listening on port %d",port);
	if(listen(socketfd, 1) == SOCKET_ERROR)
	{
		fprintf(stderr, "listn() failed with error %d\n", WSAGetLastError());
		WSACleanup();
		getch();
		return ;
	}
	fromlen = sizeof(from);

	conn_socket = accept(socketfd, (struct sockaddr*)&from, &fromlen);
	if(conn_socket == INVALID_SOCKET)
	{   
		fprintf(stderr,"accept() error %d\n", WSAGetLastError());
		WSACleanup();
		exit(1);
	}
	
	ret = MAX_BUF_SIZE;
	while(ret >0)
	{
		retval = recv(conn_socket,Buffer,ret,0);
		if ( retval < 0 )
		{
			printf ("\nFailed at recv");
  			return ;
		}
		ret=ret - retval;
		temp = temp + retval;
	}
	printf("\nReceived %d bytes from the client",temp);
	ret = MAX_BUF_SIZE;
	while(ret >0)
	{
		retval = send(conn_socket,Buffer,ret,0);
		if ( retval < 0 )
		{
			printf ("\nFailed at send");
  		    return ;
		}
		ret=ret - retval;
	}
	printf("\nSent %d bytes to the client",retval);
	printf("\n###########################################\n");
	closesocket(conn_socket);
	closesocket(socketfd);
	
}

