#define	WIN32_LEAN_AND_MEAN
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <conio.h>
#include <winbase.h>
#include <malloc.h>

/* global variables */
INT socket_type = SOCK_STREAM;
void * thHnd;
INT16 interval;
void * thHnd;
UINT32 threadId;

void TcpThread(SOCKET msgsock);
INT8 choice;
INT loop;
INT nsize;


INT main(void)
{
	INT16 sockfd;
	INT8 serverip[80];
	INT8 *Buffer;
	

	struct sockaddr_in server;
	UINT16 port,i=0,retval=0;

	printf("\n Number of iterations :");
	scanf("%d", &loop);

	printf("\n Size of each packet :");
	scanf("%d", &nsize);

	fflush(stdin);

	
	{

		WSADATA wsaData;

		if (WSAStartup(0x202,&wsaData) == SOCKET_ERROR) 
		{
			printf("WSAStartup failed with error %d\n",WSAGetLastError());
			getch();
			return -1;
		}

		printf("\n Enter the server's IP address :");
		scanf("%s", serverip);

		fflush(stdin);

		printf("\n Enter port number :");
		scanf("%d", &port);

		memset(&server,0,sizeof(server));

		server.sin_family = AF_INET;
		server.sin_addr.S_un.S_addr = inet_addr(serverip);
		server.sin_port = htons(port);

		sockfd				=socket(AF_INET,socket_type,0);
		
		if ( connect(sockfd,(struct sockaddr* )&server,sizeof(struct sockaddr_in)) == SOCKET_ERROR)
		{
			fprintf(stderr, "connect() failed with eorror %d\n", WSAGetLastError());
			closesocket(sockfd);
			WSACleanup();
			getch();
			return -1;
		}

		Buffer = (INT8 *)malloc(nsize);
		memset(Buffer, 'Z', nsize);


		for( i = 0; i < loop; i++ )
		{

			if( ( retval = send(sockfd, Buffer, nsize, 0) ) == SOCKET_ERROR)
			{
				printf("\n Failed to write ");
				closesocket(sockfd);
				getch();
				return 0;
			}
			
			if( ( retval = recv(sockfd, Buffer, nsize, 0) ) == SOCKET_ERROR )
			{
				printf("\n Failed to read ");
				closesocket(sockfd);
				getch();
				return 0;
			}
		}
		closesocket(sockfd);
		printf("\n TCP transaction successful\n");
		printf("\n %d TCP packets of size %d sent & received \n",loop,nsize);

	}
	getch();
	return 1;
}

