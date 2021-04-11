/*!
*\file  RemoteConfig.c
*\brief This files contains code for supporting updation of ZTP firmware to 
*		  eZ80F91 internal flash.
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
#include "DataPersistence.h"
#include "ZTimer.h"
#include "ZTPGlob.h"
#include "netif.h"

/* External Variable Declaration */
extern RZK_DEVICE_CB_t * CONSOLE;
extern struct commonServers csTbl;

/* Global Variable Declaration/Definition */
UINT8	g_MACADDR[6];
UINT8	g_DHCP;
UINT32	g_IPADDR;
UINT32	g_SUBNET;
UINT32	g_GATEWAY;
UINT32	g_TIMESERVER;
UINT32	g_DNSSERVER;
UINT8	g_HTTP;
UINT8	g_IGMP	=	1;
UINT8	g_TELNET;
UINT8	g_SNMPD;
UINT8	g_TIMED;
UINT8	g_SHELLTTY	=	1;

typedef struct Configuration{
	UINT8	MACADDR[6];
	UINT8	DHCP;
	UINT32	IPADDR;
	UINT32	SUBNET;
	UINT32	GATEWAY;
	UINT32	TIMESERVER;
	UINT32	DNSSERVER;
	UINT8	HTTP;
	UINT8	IGMP;
	UINT8	TELNET;
	UINT8	SNMPD;
	UINT8	TIMED;
	UINT8	SHELLTTY;
	UINT8	DUMMY;
}Configuration_t;

/*!
*\brief This function sets/reads new configurations from the Flash Info page
*/
INT16 Configuration( INT16	sockfd,INT16 option )
{
	INT8 *Buffer;
	INT16 i,retval;
	INT16 size = 400;
	struct sockaddr_in server,client;
	unsigned int temp=0;
	UINT8 Error =1;
	UINT IntLen=0, ExtLen=0;
	DATA_PER_t data_per ;
	Configuration_t *data_per_ptr;
	UINT8 * Bufptr1,*Bufptr2;

	GetDataPersistence( &data_per ) ;
	data_per_ptr = (Configuration_t *)&data_per ;


	Buffer = (INT8 *)malloc( (COUNT_t) size );
	if( Buffer == NULL )
	{
		xc_fprintf(CONSOLE,"\nERROR No memory");
		return 0;
	}

	if( option == 1 )
	{
//		xc_fprintf(CONSOLE,"\nNew Configuratuon");
		if( (retval = recv(sockfd,(char *)&data_per,sizeof(data_per),0) ) <=  0 )
		{
			xc_fprintf(CONSOLE,"\n Failed to Receive Configuratuon Settings");
			free(Buffer);
			return 0;
		}

		SetDataPersistence( &data_per ) ;
		close_s(sockfd);
		free(Buffer);

		xc_fprintf(CONSOLE,"\n\nZTP Configuration Updated");
		xc_fprintf(CONSOLE,"\nSystem Will Reboot now\n");
		RZKSuspendThread( RZKGetCurrentThread(), 2*RZK_SYSTICKS_INSEC ) ;
		asm("di");
		asm("rst 0");
	}
	else if(option == 2 )
	{

//		xc_fprintf(CONSOLE,"\nGet Current Configuratuon");

		if( data_per_ptr->HTTP == 0xFF )
			data_per_ptr->HTTP =1;
	
		if( data_per_ptr->IGMP == 0xFF )
			data_per_ptr->IGMP = 1;
	
		if( data_per_ptr->TELNET == 0xFF )
			data_per_ptr->TELNET =1;	
	
		if( data_per_ptr->SNMPD == 0xFF )
			data_per_ptr->SNMPD =1;
	
		if( data_per_ptr->TIMED == 0xFF )
			data_per_ptr->TIMED =1;
	
		if( data_per_ptr->SHELLTTY == 0xFF )
			data_per_ptr->SHELLTTY =1;

		if( data_per_ptr->TIMESERVER == 0xFFFFFFFF )
		{
			g_TIMESERVER=ntohl(inet_addr(&csTbl.timeserver[0]));
			data_per_ptr->TIMESERVER = g_TIMESERVER  ;
		}

		if( data_per_ptr->DNSSERVER == 0xFFFFFFFF )
		{
			g_DNSSERVER=ntohl(inet_addr(&csTbl.dnsserver[0]));
			data_per_ptr->DNSSERVER = g_DNSSERVER;
		}

		if( (retval = send(sockfd,(char *)data_per_ptr,sizeof(struct Configuration),tcp_FlagPUSH)) <=  0 )
	    {
			xc_fprintf(CONSOLE,"\n Failed to Send");
			free(Buffer);
			return 0;
		}

	}
	else
	return 0;

	return 1;

}

/*!
*\brief This function reads the configuration from flash info page and updates the 
*		  corresponding global variables.
*/
void RemoteConfSupport( void )
{

	DATA_PER_t data_per ;
	Configuration_t *data_per_ptr;

	GetDataPersistence( &data_per ) ;
	data_per_ptr = (Configuration_t *)&data_per;

	if( data_per_ptr->TIMESERVER != 0xFFFFFFFF )
		inet_ntoa( &csTbl.timeserver[0],ntohl(data_per_ptr->TIMESERVER) ) ;
	if( data_per_ptr->DNSSERVER != 0xFFFFFFFF )
		inet_ntoa( &csTbl.dnsserver[0],ntohl(data_per_ptr->DNSSERVER) ) ;

	if( data_per_ptr->HTTP == 0xFF )
		g_HTTP=1;
	else
		g_HTTP=data_per_ptr->HTTP;

	if( data_per_ptr->IGMP == 0xFF )
		g_IGMP=1;
	else
		g_IGMP=data_per_ptr->IGMP;

	if( data_per_ptr->TELNET == 0xFF )
		g_TELNET=1;
	else
		g_TELNET=data_per_ptr->TELNET;	

	if( data_per_ptr->SNMPD == 0xFF )
		g_SNMPD=1;
	else
		g_SNMPD=data_per_ptr->SNMPD;	

	if( data_per_ptr->TIMED == 0xFF )
		g_TIMED=1;
	else
		g_TIMED=data_per_ptr->TIMED;

	if( data_per_ptr->SHELLTTY == 0xFF )
		g_SHELLTTY=1;
	else
		g_SHELLTTY=data_per_ptr->SHELLTTY;
	
}

/* End of File */