/*!
*\file  IntFlash_Windows.c
*\brief This files contains code for supporting the Remote Firmware updation 
*		  on the Windows side.
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
//#define CLINET_DEBUG	1
#define SERVER_PORT	4000

/* Global Variable Declaration/Definition */
WSADATA wsaData;
SOCKET  conn_socket;
FILE *HexFile;
char HexRecord[600];
int size,count;
char HexFilePath[100];
char ServerIPAddr[30];

/* Function Declaration */
short WriteToeZ80(short len);
short ReadFromFile(void);

/* External Function Declaration */
extern char *AccessString ;
extern char *ActionString1;

/*!
*\brief This function opens a TCP port and connects to eZ80F91 system. Then it 
*		  opens the hex file and updates the firmware of eZ80F91.
*/
void FirmWareUpdate(void)
{
	short len=0,printing=0;
	int retval;
	char First;
	short Length;
	short Data;
	short type;
	short Dummy;
	unsigned int Addresses;
	short i=0;
	char RetBuffer[50];
	struct sockaddr_in server;


	int Length1;
	if(WSAStartup(0x202,&wsaData) == SOCKET_ERROR) 
	{
		printf("WSAStartup failed with error %d\n",WSAGetLastError());
		WSACleanup();
		exit(1);
	}	

	printf("\nEnter the Path of the Hex Record File\t: ");
	scanf("%s",HexFilePath);
	fflush(stdin);

	HexFile = fopen( HexFilePath, "r" );
	if( HexFile == NULL )
	{
		printf("Error : Unable to Open the Hex Record File\n");
		return ;
	}
		
	printf("Enter the IP Address of the EZ80F91 Device\t: ");
	scanf("%s",ServerIPAddr);
	fflush(stdin);

	memset(&server, 0, sizeof(server));
	server.sin_addr.S_un.S_addr = inet_addr(ServerIPAddr);
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

	conn_socket = socket(AF_INET,SOCK_STREAM,0); 
	if (conn_socket <0 ) 
	{
		printf("Client: Error Opening socket: Error %d\n",WSAGetLastError());
		WSACleanup();
		return ;
	}		

	if (connect(conn_socket,(struct sockaddr*)&server,sizeof(server))== SOCKET_ERROR) 
	{
		printf("connect() failed: %d\n",WSAGetLastError());
		WSACleanup();
		return ;
	}
//	printf("Connected to eZ80F91 \n");

	 /* Send the password string */
	retval = send(conn_socket,AccessString,strlen(AccessString),0);
	if (retval == SOCKET_ERROR) {
		fprintf(stderr,"send() failed: error %d\n",WSAGetLastError());
		closesocket(conn_socket);
		WSACleanup();
		return ;
	}

	/* Receive the dummy string */
	retval = recv(conn_socket,RetBuffer,10,0);
	if (retval == SOCKET_ERROR) {

		fprintf(stderr,"recv() failed: error %d\n",WSAGetLastError());
		closesocket(conn_socket);
		WSACleanup();
		return ;
	}

	/* Send the action string */
	retval = send(conn_socket,ActionString1,strlen(ActionString1),0);
	if (retval == SOCKET_ERROR) {
		fprintf(stderr,"send() failed: error %d\n",WSAGetLastError());
		closesocket(conn_socket);
		WSACleanup();
		return ;
	}

	Sleep(2000);


	printf("\nUpdating ZTP FirmWare\n");

	while ( 1 )
	{
#ifndef CLINET_DEBUG
		if(((printing)%400) == 0 )
			printf("\n");
		if(((printing++))%10 == 0 )
			printf(".");

#endif

		fscanf(HexFile,"%c",&First);
		if( First != ':' )
		{
			 printf( "Record error\n" );
			 closesocket(conn_socket);
			 fclose(HexFile);
			 return ;
		}
		fscanf(HexFile,"%2x",&Length);
		fscanf(HexFile,"%4X",&Addresses);
		fscanf(HexFile,"%2x",&type);

		Length1 = Length*2;

		sprintf(HexRecord,"%4d",Length);
		sprintf(&HexRecord[4],"%2d",type);
		sprintf(&HexRecord[6],"%5d",Addresses);
		sprintf(&HexRecord[11],"%c",'$');
#if CLINET_DEBUG
		printf("Addressses %d \n",Addresses);
#endif

		for( i=0;i<Length;i++)
		{
			fscanf(HexFile,"%2x",&Data);
			HexRecord[i+12] = ( char )Data;
		}
		/* Read the CRC and End of line charector & ignore it */
		fscanf(HexFile,"%2x",&Dummy);//CRC
		fscanf(HexFile,"%1x",&Dummy);//End of line charector.
		

		if( type == 0 || type == 01 || type == 04 )
		{
			len = 4+2+5+1+Length;
			/* Send one HEX record at a time */
			retval = send(conn_socket,HexRecord,len,0);
			if (retval == SOCKET_ERROR) {
				fprintf(stderr,"send() failed: error %d\n",WSAGetLastError());
				WSACleanup();
				fclose(HexFile);
				return ;
			}

			/* Receive the dummy string */
			retval = recv(conn_socket,RetBuffer,10,0);
			if (retval == SOCKET_ERROR) {

				if( type == 01 )
				{
					printf("\nFinished Updating ZTP FirmWare\n");
					closesocket(conn_socket);
					WSACleanup();
					fclose(HexFile);
					return ;
				}
				fprintf(stderr,"send() failed: Receive %d\n",WSAGetLastError());
				WSACleanup();
				fclose(HexFile);
				return ;
			}

			if( type == 01 )
			{
				printf("\n\nFinished Updating ZTP FirmWare\n");
				closesocket(conn_socket);
				fclose(HexFile);
				return ;
			}

#if CLINET_DEBUG
			if( type == 04 )
			{
				printf("Type 4 Record\n");
			}
#endif
		}
		else
		{
			printf( "Record Type error\n" );
			closesocket(conn_socket);
			fclose(HexFile);
			return ;
		}
	};
	
}

/* End of File */