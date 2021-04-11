/*
 * Copyright 2016, ZiLOG Inc.
 * All Rights Reserved
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of ZiLOG Inc., and might
 * contain proprietary, confidential and trade secret information of
 * ZiLOG, our partners and parties from which this code has been licensed.
 * 
 * The contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of ZiLOG Inc.
 */
 /*
 * Copyright 2001, Metro Link, Inc.
 * All Rights Reserved
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Metro Link, Inc. and may
 * contain proprietary, confidential and trade secret information of
 * Metro Link, Inc. and/or its partners.
 *  
 * The contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of Metro Link, Inc.
 *
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include"ZSysgen.h"
#include"ZTypes.h"
#include"ZThread.h"
#include "ZClock.h"
#include "ZRegion.h"
#include "socket.h"
#include "ZTPtcp.h"
#include "netif.h"

#define MAX_BUF_SIZE		1200
#define LocalPort			5000
#define ListenQueueSize		1
#define ITERATIONS			20


INT8 WriteBuff[MAX_BUF_SIZE];
extern RZK_DEVICE_CB_t * CONSOLE;
extern void networkInit(void);

void IPAddrDisplay(RZK_DEVICE_CB_t *dev)
{
    INT i=0,k;
    INT8 nif_ip[16];	
    struct netif *pstNetif; 	
    xc_fprintf (dev,"\n\nIFace\tIP address\tDef Gtway\tstate\ttype\t H/W address\n");	
    	
       pstNetif=&nif[0];
	xc_fprintf (dev,"\n%-5d\t",i);   
	xc_fprintf (dev,"%-14s  ",inet_ntoa(nif_ip, pstNetif->ni_ip));
	xc_fprintf (dev,"%-14s  ",inet_ntoa(nif_ip, pstNetif->ni_defGtwy));
	if (pstNetif->ni_state == NIS_UP)
		xc_fprintf (dev,"%-7s","UP");
	else
		xc_fprintf (dev,"%-7s","DOWN");
	xc_fprintf (dev,"%-9s"," Ethernet ");
	for(k=0;k<6;k++)
	{
		if (k<5)
			xc_fprintf (dev,"%-2x:",pstNetif->ni_hwa.ha_addr[k]);
		else
			xc_fprintf (dev,"%-2x",pstNetif->ni_hwa.ha_addr[k]);
		}

    xc_fprintf (dev,"\n\n");
}

INT ZTPAppEntry(void)
{

	INT16	sockfd,socketfd1,addrlen=sizeof(struct sockaddr_in);
	struct sockaddr_in server,client;
	INT16 retval,i=0;
	INT8 *demostackAp;

	networkInit();

	sockfd	=socket(AF_INET,SOCK_STREAM,0);
	if (sockfd < 0 )
    {
		printf("Error opening end point");
		return -1;
    }

	server.sin_family	=AF_INET;			
	server.sin_port	=htons(LocalPort); 
	server.sin_addr.s_addr	=INADDR_ANY;

	if( bind(sockfd,(struct sockaddr *)&server,sizeof(struct sockaddr_in)) < 0 )
	{
		printf("Error In Binding Socket\n");
		return -1;
	}

	if( listen(sockfd,ListenQueueSize) != 0 )
	{
		printf("Error In Listening on Socket\n");
		return -1;
	}
	IPAddrDisplay(CONSOLE);
	printf("\n Simple ZTP TCP Demo \n");
	printf(" No of Iterations :%d Size of data :%d \n",ITERATIONS,MAX_BUF_SIZE);
	while( 1 )
	{
		printf("\n Listening on port :%d \n",LocalPort);
		socketfd1=accept(sockfd,(struct sockaddr *)&client,&addrlen);
		if(socketfd1 < 0)
		{
			printf("Error In opening the socket\n");
			return -1;
		}
		
		for(i=0;i<ITERATIONS;i++)
		{
	
			if( (retval = recv(socketfd1,WriteBuff,MAX_BUF_SIZE,0)) <=  0 )
 		    {
				printf("\n Failed to Read ");
				break;
			}
	
			if( (retval = send(socketfd1,WriteBuff,MAX_BUF_SIZE,tcp_FlagPUSH)) < 0 )
			{
				printf("\n Failed to Write ");
				break;
			}
	
		}
		
		printf("Finished Data Transfer\n");
		close_s(socketfd1);
	}
	close_s(sockfd);
	return(1);
}
