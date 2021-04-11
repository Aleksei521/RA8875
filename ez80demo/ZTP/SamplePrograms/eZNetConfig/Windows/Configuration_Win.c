/*!
*\file  Configuration_Win.c
*\brief This files contains code for supporting the Remote Configuration on the Windows side.
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
#define SERVER_PORT	4000

/* Global Variable Declaration/Definition */
char *enable = "Enable";
char *disable = "Disable";
unsigned char	g_MACADDR[20];
unsigned short	g_DHCP;
unsigned char	g_IPADDR[20];
unsigned char	g_SUBNET[20];
unsigned char	g_GATEWAY[20];
unsigned char   g_TIMESERVER[20];
unsigned char   g_DNSSERVER[20];
unsigned short	g_HTTP;
unsigned short	g_IGMP;
unsigned short	g_TELNET;
unsigned short	g_SNMPD;
unsigned short	g_TIMED;
unsigned short	g_SHELLTTY;
unsigned short  g_DUMMY;
char SendBuffer[50];
typedef struct Configuration{
	unsigned char	MACADDR[6];
	unsigned char	DHCP;
	unsigned long	IPADDR;
	unsigned long	SUBNET;
	unsigned long	GATEWAY;
	unsigned long	TIMESERVER;
	unsigned long	DNSSERVER;
	unsigned char	HTTP;
	unsigned char	IGMP;
	unsigned char	TELNET;
	unsigned char	SNMPD;
	unsigned char	TIMED;
	unsigned char	SHELLTTY;
	unsigned char	DUMMY;
}Configuration_t;

/* External Function Declaration */
extern char *AccessString ;
extern char *ActionString1;
extern char *ActionString2;
extern char *ActionString3;


/*!
*\brief This function gets the options from the user and sends appropriate commands
*/
void Configuration( void )
{

	SOCKET  conn_socket;
	struct sockaddr_in server;
	struct in_addr AddressesIP;
	Configuration_t Data_Per;
	char TempMac,TempMac1,*TempPtr;
	unsigned short i;
	int retval;
	WSADATA wsaData;
	char RetBuffer[50];
	char ServerIPAddr[30];
	char Option=5;
	unsigned long *AddrPtr;


	if(WSAStartup(0x202,&wsaData) == SOCKET_ERROR) 
	{
		printf("WSAStartup failed with error %d\n",WSAGetLastError());
		WSACleanup();
		exit(1);
	}	

	memset(&Data_Per,0x00,sizeof(Configuration_t));
	do
	{
		printf("\nEnter the Option  \n1 - Get eZ80 configuration  \n2 - Set eZ80 configuration   \n3 - Break\nEnter the Option > ");
		fflush(stdin);
		scanf("%c",&Option);
		fflush(stdin);
		if(Option == '3' )
			return;
		if( (Option == (char)'2') || (Option == (char)'1') )
		{
			break;
		}
	}while(1);

	/* Set eZ80 configurations */
	if(Option == (char)'2' )
	{

		printf("\n");
		printf("Enter the Configuration Settings\n\n");
		printf("EMAC Addresses\t\t: ");
		fflush(stdin);
		scanf("%s",g_MACADDR);
		fflush(stdin);
		TempPtr = g_MACADDR;
		for( i=0;i<6;i++)
		{
			sscanf((const char *)TempPtr,"%2x",&TempMac);
			TempMac1=(char)TempMac;
			sprintf(&Data_Per.MACADDR[i],"%1c",TempMac1);
			TempPtr+=3;
		}
		printf("IP Addresses\t\t: ");
		fflush(stdin);
		scanf("%s",g_IPADDR);
		Data_Per.IPADDR=(inet_addr(g_IPADDR));
		printf("Subnet Mask\t\t: ");
		fflush(stdin);
		scanf("%s",g_SUBNET);
		Data_Per.SUBNET=(inet_addr(g_SUBNET));
		printf("Gateway Addresses\t: ");
		fflush(stdin);
		scanf("%s",g_GATEWAY);
		Data_Per.GATEWAY=(inet_addr(g_GATEWAY));
		printf("DNS Server Addresses\t: ");
		fflush(stdin);
		scanf("%s",g_DNSSERVER);
		Data_Per.DNSSERVER=(inet_addr(g_DNSSERVER));
		printf("TIME Server Addresses\t: ");
		fflush(stdin);
		scanf("%s",g_TIMESERVER);
		Data_Per.TIMESERVER=(inet_addr(g_TIMESERVER));

	
		do 
		{
			printf("HTTP (e/d)\t\t: ");
			fflush(stdin);
			scanf("%c",&g_HTTP);
		}while(g_HTTP<'d' || g_HTTP>'e');
		if(g_HTTP == 'e')
			Data_Per.HTTP=1;
		else
			Data_Per.HTTP=0;
		
		do 
		{
			printf("IGMP (e/d)\t\t: ");
			fflush(stdin);
			scanf("%c",&g_IGMP);
		}while(g_IGMP<'d' || g_IGMP>'e');
		if(g_IGMP == 'e')
			Data_Per.IGMP=1;
		else
			Data_Per.IGMP=0;
		
		do 
		{
			printf("TELNET (e/d)\t\t: ");
			fflush(stdin);
			scanf("%c",&g_TELNET);
		}while(g_TELNET<'d' || g_TELNET>'e');
		if(g_TELNET == 'e')
			Data_Per.TELNET=1;
		else
			Data_Per.TELNET=0;
		

		do 
		{
			printf("SNMP (e/d)\t\t: ");
			fflush(stdin);
			scanf("%c",&g_SNMPD);
		}while(g_SNMPD<'d' || g_SNMPD>'e');
		if(g_SNMPD == 'e')
			Data_Per.SNMPD=1;
		else
			Data_Per.SNMPD=0;

		do 
		{
			printf("TIMED (e/d)\t\t: ");
			fflush(stdin);
			scanf("%c",&g_TIMED);
		}while(g_TIMED<'d' || g_TIMED>'e');
		if(g_TIMED == 'e')
			Data_Per.TIMED=1;
		else
			Data_Per.TIMED=0;

		do 
		{
			printf("SHELL (e/d)\t\t: ");
			fflush(stdin);
			scanf("%c",&g_SHELLTTY);
		}while(g_SHELLTTY<'d' || g_SHELLTTY>'e');
		if(g_SHELLTTY == 'e')
			Data_Per.SHELLTTY=1;
		else
			Data_Per.SHELLTTY=0;

		do 
		{
			printf("DHCP (e/d)\t\t: ");
			fflush(stdin);
			scanf("%c",&g_DHCP);
		}while(g_DHCP<'d' || g_DHCP>'e');
		if(g_DHCP == 'e')
			Data_Per.DHCP=1;
		else
			Data_Per.DHCP=0;
		
		/* Windows adds a 1 byte dummy charector after the DHCP for even padding
		   in the structure. So just ignore this byte */ 
		memcpy( (void *)SendBuffer, (const void *)&Data_Per, 7 );
		memcpy( (void *)&SendBuffer[7], (const void *)&Data_Per.IPADDR, 28 );

		printf("\nEnter the IP Address of the eZ80F91\t: ");
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
			exit(1);
		}	

		if (connect(conn_socket,(struct sockaddr*)&server,sizeof(server))== SOCKET_ERROR) 
		{
			printf("connect() failed: %d\n",WSAGetLastError());
			WSACleanup();
			return ;
		}
		  
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
		retval = send(conn_socket,ActionString3,strlen(ActionString3),0);
		if (retval == SOCKET_ERROR) {
			fprintf(stderr,"send() failed: error %d\n",WSAGetLastError());
			closesocket(conn_socket);
			WSACleanup();
			return ;
		}

		Sleep(2000);

		/* Send the configuration settings */
		retval = send(conn_socket,(char *)SendBuffer,34,0);
		if (retval == SOCKET_ERROR) {
			fprintf(stderr,"send() failed: error %d\n",WSAGetLastError());
			closesocket(conn_socket);
			WSACleanup();
			return ;
		}
		printf("\nNew Configuration Enabled  \n");
		closesocket(conn_socket);
		return;
	}
	/* Get eZ80 configuration */
	else if( Option == (char)'1' )
	{
		printf("Get Conf\n");
		printf("\nEnter the IP Address of the eZ80F91\t : ");
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
			exit(1);
		}	

		if (connect(conn_socket,(struct sockaddr*)&server,sizeof(server))== SOCKET_ERROR) 
		{
			printf("connect() failed: %d\n",WSAGetLastError());
			WSACleanup();
			return ;
		}

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
		retval = send(conn_socket,ActionString2,strlen(ActionString2),0);
		if (retval == SOCKET_ERROR) {
			fprintf(stderr,"send() failed: error %d\n",WSAGetLastError());
			closesocket(conn_socket);
			WSACleanup();
			return ;
		}

		Sleep(1000);
		/* Receive the configuration settings from eZ80F91 */
		retval = recv(conn_socket,(char *)&Data_Per,sizeof(Data_Per)+1,0);
		if (retval == SOCKET_ERROR) {

			fprintf(stderr,"recv() failed: error %d\n",WSAGetLastError());
			closesocket(conn_socket);
			WSACleanup();
			return ;
		}
		
//		printf("Received the configuration\n");
		memcpy(&g_MACADDR[0],&Data_Per,6);
		printf("\n--------------------------------------\n");
		printf("EMAC Addresses\t\t: %1x:%1x:%1x:%1x:%1x:%1x\n",g_MACADDR[0],
					g_MACADDR[1],g_MACADDR[2],g_MACADDR[3],g_MACADDR[4],g_MACADDR[5]);
		AddrPtr = (unsigned long *)((char *)&Data_Per+7);
		AddressesIP.S_un.S_addr = *AddrPtr;
		printf("IP Addresses\t\t: %-40s \n",inet_ntoa(AddressesIP) );
		fflush(stdin);
		AddrPtr = (unsigned long *)((char *)&Data_Per+11);
		AddressesIP.S_un.S_addr = *AddrPtr;
		printf("Subnet Mask\t\t: %-40s \n",inet_ntoa(AddressesIP) );
		fflush(stdin);
		AddrPtr = (unsigned long *)((char *)&Data_Per+15);
		AddressesIP.S_un.S_addr = *AddrPtr;
		printf("Gateway Addresses\t: %s \n",inet_ntoa(AddressesIP) );
		fflush(stdin);
		AddrPtr = (unsigned long *)((char *)&Data_Per+19);
		AddressesIP.S_un.S_addr = *AddrPtr;
		printf("DNS Server Addresses\t: %s \n",inet_ntoa(AddressesIP) );
		fflush(stdin);
		AddrPtr = (unsigned long *)((char *)&Data_Per+23);
		AddressesIP.S_un.S_addr = *AddrPtr;
		printf("TIME Server Addresses\t: %s \n",inet_ntoa(AddressesIP) );

		TempPtr=(unsigned char *)((unsigned char *)&Data_Per+6);

		printf("DHCP\t\t\t: %s \n",*TempPtr ==0x01 ? enable:disable);
		TempPtr=(unsigned char *)((unsigned char *)&Data_Per+27);
		printf("HTTP\t\t\t: %s \n",*TempPtr++ ==0x01 ? enable:disable);
		printf("IGMP\t\t\t: %s \n",*TempPtr++ ==0x01 ? enable:disable);
		printf("TELNET\t\t\t: %s \n",*TempPtr++ ==0x01 ? enable:disable);
		printf("SNMPD\t\t\t: %s \n",*TempPtr++ ==0x01 ? enable:disable);
		printf("TIMED\t\t\t: %s \n",*TempPtr++ ==0x01 ? enable:disable);
		printf("SHELLTTY\t\t: %s \n",*TempPtr ==0x01 ? enable:disable);
		printf("--------------------------------------\n");
		fflush(stdin);
		closesocket(conn_socket);
	}


}

/* End of File */