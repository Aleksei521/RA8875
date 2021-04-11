/*!
*\file  Discovery.c
*\brief This files contains code for supporting the Network Scan.
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

*        Copyright (C) 2016 by  ZILOG, Inc.
*        All Rights Reserved
*/

/* Local Includes */
#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZThread.h"
#include "socket.h"
#include "domain.h"
#include "shell.h"
#include "igmp.h"
#include "netif.h"

/*	 Constant Definition */
#define MAX_BUF_SIZ	100	 /* Size of the buffer to hold the querry */
#define MULTICAST_GROUP "226.2.2.2"	/* Multicast group IP addreses */
#define MULTICAST_PORT	4111	  /* UDP port number on which to listen for querry */

/* Global Variable Declaration/Definition */
char DataBuff[MAX_BUF_SIZ]; /* Buffer to hold the querry */
char writeip[20];	  /* Buffer to hold the IP addresses of the querrying system */

/* External Variable Declaration */
extern RZK_DEVICE_CB_t * CONSOLE;
extern INT8*		eZ80_name;

/*!
*\brief This function joins ZTP to an IGMP group. It opens a UDP port on 4111 and 
*		  listens for the Network Scan command on the. It replies with the details 
*		  of the system such as the IP, MAC addresses and Name of the system.
*/
void discovery(void)
{
	short	sockfd,addrlen=sizeof(struct sockaddr_in);
	short len=0;
	struct sockaddr_in server,client;
	char addr[25]={0,};
	INT8 nif_ip[16],nif_emac[20];
	UINT16 k=0;
	UINT32 mcGroup = 0 ;
	struct netif *pstNetif;
	
	mcGroup = htonl(inet_addr (MULTICAST_GROUP)) ;
	hgjoin (0, mcGroup, 2) ;

	sockfd = socket(AF_INET,SOCK_DGRAM,0);

	server.sin_family = AF_INET;
	server.sin_port	= intel16(MULTICAST_PORT); 
	server.sin_addr.s_addr = INADDR_ANY;
	
	bind(sockfd,(struct sockaddr *)&server,sizeof(struct sockaddr_in));
	pstNetif=&nif[0];

	while(1)
	{
		len = recvfrom (sockfd,DataBuff,MAX_BUF_SIZ,(short)0,(struct sockaddr *)&client,&addrlen);
		{
			if (len > 0)
			{
				inet_ntoa(addr,client.sin_addr.s_addr);
			}
			else
			{
				xc_fprintf(CONSOLE,"Error Receiving  Data\n");
				break;
			}
		}
		
		if(!strncmp(DataBuff,"discover",8))
		{
			inet_ntoa(nif_ip, pstNetif->ni_ip);
			xc_sprintf (nif_emac,"%-2x:%-2x:%-2x:%-2x:%-2x:%-2x",pstNetif->ni_hwa.ha_addr[0],
			pstNetif->ni_hwa.ha_addr[1],pstNetif->ni_hwa.ha_addr[2],pstNetif->ni_hwa.ha_addr[3],
			pstNetif->ni_hwa.ha_addr[4],pstNetif->ni_hwa.ha_addr[5]);
			xc_sprintf(DataBuff,"discover$%s$%s$%s",eZ80_name,nif_emac,nif_ip);
			addrlen=sizeof(struct sockaddr_in);
			len = strlen(DataBuff);
			len = sendto (sockfd,DataBuff,len,0,(struct sockaddr *)&client,addrlen);
			if (len <0 )
			{
				xc_fprintf(CONSOLE,"Error Sending Data\n");
				break;
			}

		}
	}
	close_s(sockfd);
}

/* End of File */

