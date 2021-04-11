/*
 * File       :     socketdemo.c
 *
 * Description:  This file handles the data transfer between 
				 TCP/UDP clients and TCP/UDP servers using sockets.
 *
 * Copyright 2017 ZiLOG Inc.  ALL RIGHTS RESERVED.
 *
 * This file contains unpublished confidential and proprietary information
 * of ZiLOG, Inc.
 * NO PART OF THIS WORK MAY BE DUPLICATED, STORED, PUBLISHED OR DISCLOSED 
 * IN ANY FORM WITHOUT THE PRIOR WRITTEN CONSENT OF ZiLOG, INC.
 * This is not a license and no use of any kind of this work is authorized
 * in the absence of a written license granted by ZiLOG, Inc. in ZiLOG's 
 * sole discretion 
 */

/* standard includes */
#include <stdio.h>
#include "socket.h"
#include <string.h>

/* User includes */
#include"ZSysgen.h"
#include"ZTypes.h"
#include"ZThread.h"
#include "ZClock.h"
#include "ZRegion.h"
#include "SocketDemo.h"
#include "shell.h"

/*  extern declarations  */
extern RZK_DEVICE_CB_t *CONSOLE ;
extern void *malloc( COUNT_t size );
extern void free( void *seg );


#define MAX_BUFF_SIZE_UDP 1000
#define MAX_BUF_SIZE_TCP 4000


void Tcpclient(INT8 *buf,int port)
{
	INT16 tmp,ret=0, retval=0,temp=0,sockfd;
	INT8 *S2Buff;
	struct sockaddr_in server;
	server.sin_family		=	AF_INET;
	server.sin_port			= 	intel16(port); 
	server.sin_addr.s_addr	= 	intel(inet_addr(buf));	
	sockfd	=	socket(AF_INET,SOCK_STREAM,0);
	tmp = connect(sockfd,(struct sockaddr* )&server,sizeof(struct sockaddr_in));
	if ( tmp < 0 )
	{
		xc_fprintf(CONSOLE,"\nCould not connect to Remote host and socket= %d",sockfd);	
		goto end;
	}
	S2Buff = (INT8 *)malloc( (COUNT_t)MAX_BUF_SIZE_TCP);
	if (S2Buff == NULL)
	{
		printf("\nError while allocating memory");
		goto end;
	}
	memset(S2Buff, 'Z', MAX_BUF_SIZE_TCP);
	ret = MAX_BUF_SIZE_TCP;
	while(ret>0)
	{
		retval = send(sockfd,S2Buff,ret,0);
		if ( retval < 0 )
		{
			xc_fprintf(CONSOLE,"\nFailed to send %d", retval);
			goto end1;
		}
		ret=ret - retval;
	}
	xc_fprintf(CONSOLE,"\n################################################");
	xc_fprintf(CONSOLE,"\nSent %d bytes to the server",retval);	
	ret = MAX_BUF_SIZE_TCP;
	while(ret>0)
	{
		retval = recv(sockfd,S2Buff,ret,0);
		if ( retval < 0 )
		{
			xc_fprintf(CONSOLE,"\nFailed to recv %d", retval);
			goto end1;
		}
		temp = temp + retval;
		ret=ret - retval;
	}	
	xc_fprintf(CONSOLE,"\nReceived %d bytes from the server",temp);
	xc_fprintf(CONSOLE,"\n################################################");
	goto end1;
end:
	close_s(sockfd);
	free(buf);
	return ;
end1:
	free(S2Buff);
	free(buf);
	close_s(sockfd);

	return ;
}



void Tcpserver(int port)
{
	INT16 ret,retval=0,temp=0,sockfd ,listen_sockfd,addrlen;
	INT8 *S1Buff, *pBuf;
	struct sockaddr_in server,client;
	sockfd	=	socket(AF_INET,SOCK_STREAM,0);
	server.sin_family		=	AF_INET;
	server.sin_port			= 	intel16(port); 
	server.sin_addr.s_addr	= 	INADDR_ANY;	
	if( bind(sockfd,(struct sockaddr *)&server,sizeof(struct sockaddr_in)) < 0 )
	{
		xc_fprintf(CONSOLE,"\nCouldn't bind");
		goto end3;
	}
	if(listen(sockfd, 1) < 0)
	{
		xc_fprintf(CONSOLE,"\nListen failed ");
		goto end3;
	}
	xc_fprintf(CONSOLE,"\n################################################");
	xc_fprintf(CONSOLE,"\nServer is Listening on port %d",port); 
	addrlen = sizeof(struct sockaddr_in);
	S1Buff = (INT8 *)malloc( (COUNT_t)MAX_BUF_SIZE_TCP);
	if (S1Buff == NULL)
	{
		printf("\nError while allocating memory");
		goto end3;
	}
	listen_sockfd = accept(sockfd,(struct sockaddr *)&client,(INT16 *)&addrlen);
	if( listen_sockfd < 0 ) 
	{
		xc_fprintf(CONSOLE,"\nError in accept");
		goto end3;
	}
	ret = MAX_BUF_SIZE_TCP;
	pBuf = S1Buff;
	while(ret>0)
	{
		retval = recv(listen_sockfd,pBuf,ret,0);
		if ( retval < 0 )
		{
			xc_fprintf(CONSOLE,"\nFailed to recv %d bytes",retval);
			goto end2;
		}
		temp = temp + retval;
		ret=ret - retval;
		pBuf += retval;
	}	
	xc_fprintf(CONSOLE,"\nReceived  the %d bytes from client",temp);
	ret = MAX_BUF_SIZE_TCP;
	pBuf = S1Buff;
	while(ret>0)
	{
		retval = send(listen_sockfd,pBuf,ret,0);
		if ( retval < 0 )
		{
			xc_fprintf(CONSOLE,"\nFailed to send %d bytes",retval);
			goto end2;
		}
		ret=ret - retval;
		pBuf += retval;
	}
	xc_fprintf(CONSOLE,"\nSent %d bytes to client",retval);
	xc_fprintf(CONSOLE,"\n################################################");
	goto end2;
end3:
	close_s(sockfd);
	return ;

end2:
	free(S1Buff);
	close_s(listen_sockfd);
	close_s(sockfd);
  	return;
}


void Udpclient(INT8 *buf, int port)
{
		
	INT16 retval=0,recvtemp,sockfd,tmp,ret=0,temp=0,fromlen ;
	INT8 *SBuff1;
	struct sockaddr_in server,localserver;
	fromlen = sizeof(struct sockaddr_in);
	server.sin_family		=	AF_INET;
	server.sin_port			= 	intel16(port); 
	server.sin_addr.s_addr	= 	intel(inet_addr(buf));
	sockfd	=socket(AF_INET,SOCK_DGRAM,0);
	tmp = connect(sockfd,(struct sockaddr* )&server,sizeof(struct sockaddr_in));
	
	if(tmp<0)
	{
		xc_fprintf(CONSOLE,"\nCould not connect to Remote host");	
		goto end1;
	}
	
	SBuff1 = (INT8 *)malloc( (COUNT_t)MAX_BUFF_SIZE_UDP);
	if (SBuff1 == NULL)
	{
		printf("\nError while allocating memory");
		goto end1;
	}
	memset(SBuff1, 'Z', MAX_BUFF_SIZE_UDP);
	recvtemp = MAX_BUFF_SIZE_UDP;
	while(recvtemp >0)
	{
		retval = sendto(sockfd,SBuff1,recvtemp,0, (struct sockaddr *)&server, sizeof(struct sockaddr));
		if ( retval < 0 )
		{
			xc_fprintf(CONSOLE,"\nFailed to sendto %d bytes",retval);
			goto end2;
		}
		recvtemp -= retval;
	}
	xc_fprintf(CONSOLE,"\n################################################");
	xc_fprintf(CONSOLE,"\nSent %d bytes to the server",retval);
	recvtemp = MAX_BUFF_SIZE_UDP;
	while(recvtemp >0)
	{
		retval = recvfrom(sockfd, SBuff1, recvtemp, 0, (struct sockaddr *)&localserver, &fromlen);
		if ( retval < 0 )
		{
			xc_fprintf(CONSOLE,"\nFailed to recvd %d bytes",retval);
			goto end2;
		}
		recvtemp -= retval;
		temp = temp + retval;
	}
	xc_fprintf(CONSOLE,"\nReceived  %d bytes data from the server",temp);
	xc_fprintf(CONSOLE,"\n################################################");
	goto end2;
end1:
	close_s(sockfd);
	free(buf);
	return;

end2:
	close_s(sockfd);
	free(buf);
	free(SBuff1);
	return;
}


void Udpserver(int port)
{
	INT16 retval=0,recvtemp,temp=0,sockfd,fromlen ;
	INT8 *SBuff;
	struct sockaddr_in server,localserver;
	fromlen = sizeof(struct sockaddr_in);
	server.sin_family		= AF_INET;
	server.sin_port			= intel16(port); 
	server.sin_addr.s_addr	= INADDR_ANY;
	sockfd	=socket(AF_INET,SOCK_DGRAM,0);
	if( bind(sockfd,(struct sockaddr *)&server,sizeof(struct sockaddr_in)) < 0 )
	{
		xc_fprintf(CONSOLE,"\nCouldn't bind");
		goto end;
	}
	xc_fprintf(CONSOLE,"\n################################################");
	xc_fprintf(CONSOLE,"\nServer is bound to the port %d",port);
	SBuff = (INT8 *)malloc( (COUNT_t)MAX_BUFF_SIZE_UDP);
	if (SBuff == NULL)
	{
		printf("\nError while allocating memory");
		goto end;
	}
	recvtemp = MAX_BUFF_SIZE_UDP;
	while(recvtemp >0)
	{
		retval = recvfrom(sockfd, SBuff, recvtemp, 0, (struct sockaddr *)&localserver, &fromlen);
		if ( retval < 0 )
		{
			xc_fprintf(CONSOLE,"\nFailed to recvd %d bytes",retval);
			goto end1;
		}
		recvtemp -= retval;
		temp = temp + retval;
	}
	xc_fprintf(CONSOLE,"\nReceived  %d bytes data from the client",temp);
	recvtemp = MAX_BUFF_SIZE_UDP;
	while(recvtemp >0)
	{
		retval = sendto(sockfd,SBuff,recvtemp,0, (struct sockaddr *)&localserver, sizeof(struct sockaddr));
		if ( retval < 0 )
		{
			xc_fprintf(CONSOLE,"\nFailed to send %d bytes",retval);
			goto end1;
		}
		recvtemp -= retval;
	}
	xc_fprintf(CONSOLE,"\nSent %d bytes to the client",retval);
	xc_fprintf(CONSOLE,"\n################################################");
	goto end1;
end:
	close_s(sockfd);
	return;
end1:
	close_s(sockfd);
	free(SBuff);
	return;

}



