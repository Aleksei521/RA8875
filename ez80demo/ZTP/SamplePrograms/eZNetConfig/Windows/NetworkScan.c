/*!
*\file  NetworkScan.c
*\brief This files contains code for supporting the Network Scan on windows side.
*/
/*	    ZILOG DISCLAIMER
*      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY 
*      KIND,EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
*      WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
*      PURPOSE AND NONINFRINGEMENT. 
*      IN NO EVENT SHALL ZILOG, INC BE LIABLE FOR ANY CLAIM, DAMAGES
*      OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
*      OTHERWISE,ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
*      SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*        Copyright (C) 2006 by  ZILOG, Inc.
*        All Rights Reserved
*/

/* Includes */
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>

/*	 Constant Definition */
#define BUFFSIZE 100
#define MULTICAST_GROUP "226.2.2.2"
#define MULTICAST_PORT	4111
#ifndef IP_MULTICAST_IF 
/* 
 * The following constants are taken from include/netinet/in.h
 * in Berkeley Software Distribution version 4.4. Note that these 
 * values *DIFFER* from the original values defined by Steve Deering 
 * as described in "IP Multicast Extensions for 4.3BSD UNIX related 
 * systems (MULTICAST 1.2 Release)". It describes the extensions 
 * to BSD, SunOS and Ultrix to support multicasting, as specified
 * by RFC-1112. 
 */ 
#define IP_MULTICAST_IF 9 /* set/get IP multicast interface */ 
#define IP_MULTICAST_TTL 10 /* set/get IP multicast TTL */
#define IP_MULTICAST_LOOP 11 /* set/get IP multicast loopback */ 
#define IP_ADD_MEMBERSHIP 12 /* add (set) IP group membership */ 
#define IP_DROP_MEMBERSHIP 13 /* drop (set) IP group membership */ 
#define IP_DEFAULT_MULTICAST_TTL 1 
#define IP_DEFAULT_MULTICAST_LOOP 1 
#define IP_MAX_MEMBERSHIPS 20 
/* The structure used to add and drop multicast addresses */ 
typedef struct ip_mreq { 
  struct in_addr imr_multiaddr; /* multicast group to join */
  struct in_addr imr_interface; /* interface to join on */
}IP_MREQ; 
#endif 

/* Global Variable Declaration/Definition */
char achInBuf[BUFFSIZE]; 
char achOutBuf[BUFFSIZE]; 
HANDLE			  ReceiverHndle ;

/* Function Declaration */
void *Receiver(void *arg );

/*!
*\brief This function sends the discovery command on port 4111 to multicast group 
*		  226.2.2.2 and waits for replies from the members of that group.
*/
void discovery( void )
{
	int nRet; 
	SOCKET hSock; 
	struct sockaddr_in stSourceAddr, stDestAddr; 
	WSADATA wsaData;
	LPDWORD			  ID=NULL;
	DWORD			  Return_Stat=0;
	int wait=2000;

	if (WSAStartup(0x202,&wsaData) == SOCKET_ERROR) 
	{
		printf("WSAStartup failed with error %d\n",WSAGetLastError());
		WSACleanup();
		exit(1);
	}	

	hSock = socket(PF_INET, SOCK_DGRAM, 0); 
	if (hSock == INVALID_SOCKET) {
		printf("Error opening Socket. Error No: %d\n",WSAGetLastError());
		WSACleanup();
		exit(1);
	} 

  stSourceAddr.sin_family = PF_INET; 
  stSourceAddr.sin_port = htons(MULTICAST_PORT); 
  stSourceAddr.sin_addr.s_addr = INADDR_ANY; 

  nRet = bind (hSock,(struct sockaddr FAR *)&stSourceAddr,sizeof(struct sockaddr)); 
  if (nRet == SOCKET_ERROR) { 
		printf("Error Binding Socket. Error No: %d\n",WSAGetLastError());
		WSACleanup();
		exit(1);     
  } 

  stDestAddr.sin_family = PF_INET; 
  stDestAddr.sin_addr.s_addr = inet_addr (MULTICAST_GROUP);
  stDestAddr.sin_port = htons (MULTICAST_PORT);
  
  strcpy(achOutBuf,"discover");

  nRet = sendto (hSock, (char FAR *)achOutBuf,strlen(achOutBuf), 0,(struct sockaddr FAR *) &stDestAddr,
          sizeof(struct sockaddr)); 
	if (nRet == SOCKET_ERROR) { 
		printf("Error Sending Data. Error No.  %d \n",WSAGetLastError());
 
	} 

	ReceiverHndle = CreateThread( NULL, 0 ,( LPTHREAD_START_ROUTINE  )
 							Receiver , (LPVOID)hSock , 0 , ID );

	Return_Stat =  WaitForSingleObject( ReceiverHndle , wait );

	TerminateThread( ReceiverHndle , 0);
	closesocket(hSock);
	fflush(stdin);

}

/*!
*\brief This function receives the replies from the  multicast group 226.2.2.2 and 
*		  dosplays the configuration.
*/
void *Receiver(void * arg )
{
	short SocktFd = (short)arg;
	struct sockaddr_in client;
	short index=0;
	char *MacPtr=NULL,*IPPtr=NULL;

	int len = sizeof(struct sockaddr),ret;

	printf("\nNO.   NAME.             MACC ADDR             IP ADDR\n\n");
	
	while( 1)
	{
		memset(achInBuf,00,BUFFSIZE);
		ret = recvfrom (SocktFd,achInBuf,BUFFSIZE,(short)0,(struct sockaddr *)&client,&len);
		if (ret > 0)
		{
			
			if(!strncmp(achInBuf,"discover",8))
			{
				achInBuf[ret+1]=0;
				MacPtr = strchr(&achInBuf[9],'$');
				if( !MacPtr )
					continue;
				*MacPtr=0;
				MacPtr++;
				IPPtr = strchr(MacPtr,'$');
				if(!IPPtr)
					continue;

				*IPPtr=0;
				IPPtr++;
				printf("%-5d %-15s   %-16s     %-16s\n",index++,&achInBuf[9],MacPtr,IPPtr);
			}
		}
		else
		{
			printf("Error Receiving  Data\n");
			exit(1);
		}
	};
}

/* End of File */