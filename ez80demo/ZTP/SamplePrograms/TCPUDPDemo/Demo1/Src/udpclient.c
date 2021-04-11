/*
 * File       :     udpclient.c
 *
 * Description:  This file handles the data transfer between 
				 UDP client and UDP server on eZ80 using sockets.
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
#define	WIN32_LEAN_AND_MEAN
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <conio.h>
#include <winbase.h>
#include <malloc.h>
int sockfd;
#define MAX_BUFF_SIZE 1000
void main(void)
{

	struct sockaddr_in server,local;
	int retval,fromlen,recvtemp=MAX_BUFF_SIZE;
	char *Buffer,serverip[80];
	WSADATA wsaData;
	unsigned short port;
	fromlen = sizeof(local);
	memset(&server,0,sizeof(server));
	Buffer = (char *)malloc(MAX_BUFF_SIZE);
	printf("\nEnter the server's IP address:");
	scanf("%s", serverip);
	printf("\nEnter the server port number:");
	scanf("%d",&port);	
	server.sin_family = AF_INET;
	server.sin_addr.S_un.S_addr = inet_addr(serverip);
	server.sin_port = htons(port);
	if (WSAStartup(0x202,&wsaData) == SOCKET_ERROR) 
	{
		printf("WSAStartup failed with error %d\n",WSAGetLastError());
		WSACleanup();
		exit(1);
	}
	sockfd	= socket(AF_INET,SOCK_DGRAM,0);
	retval =0;		
	recvtemp = MAX_BUFF_SIZE;
	while(recvtemp >0 )
	{
		retval = sendto(sockfd, Buffer, recvtemp, 0, (struct sockaddr *)&server, sizeof(struct sockaddr));
		if (retval <0)
		{
			fprintf(stderr,"sendto() failed: error %d\n",WSAGetLastError());
			WSACleanup();
			closesocket(sockfd);
			exit(1);
		}
		recvtemp -= retval;
	}
	printf("\n###########################################");
	printf("\nSent %d bytes of data to the server",retval);
	fromlen = sizeof(struct sockaddr);
	recvtemp = MAX_BUFF_SIZE; 
	while(recvtemp >0 )
	{
		retval = recvfrom(sockfd, Buffer, recvtemp, 0, (struct sockaddr *)&local, &fromlen);
		if (retval <0)
		{
			fprintf(stderr,"recvfrom() failed: error %d\n",WSAGetLastError());
			WSACleanup();
			closesocket(sockfd);
			exit(1);
		}
		recvtemp -= retval;
	}
	printf("\nReceived %d bytes of data from the server",retval);
	printf("\n###########################################\n");
	closesocket(sockfd);

}

