/*!
*\file  Config_Flash_Server.c
*\brief This files contains code for supporting the Remote Configuration on the ZTP side.
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
#include "netif.h"

/*	 Constant Definition */
#define SERVER_PORT1 4000	 /* Port on which the Config server listens */

/* Global Variable Declaration/Definition */
char *AccessString = "Authentication";	 /* This is the Password for the authentication */
char RetBuffer[10]="ABCDEFGHI"; /*	Dummy return buffer */
char Action1[7]="QUERRY";	 /* String command for the Network scan */
char Action2[7]="SETPAR";	 /* String command for new settings */
char Action3[7]="FIRMWA";	 /* String command for querying current settings */

/* External Function Declaration */
extern RZK_DEVICE_CB_t * CONSOLE;
extern INT16 Configuration( INT16,INT16 );
extern INT16 IntFlashUpdater( INT16	);
extern void discovery(void);

/*!
*\brief This function opens a TCP port 4000 and listens for connection. On connection
*		  first the client is authenticated and then the command is parsed. Based on the 
*		  command received the appropriate function is called. 
*/
void Config_Flash_Server( void )
{
	INT8 *Buffer;
	INT16 i,retval;
	INT16	sockfd,socketfd1,addrlen=sizeof(struct sockaddr_in);
	INT16 size = 400;
	INT16 LocalPort = SERVER_PORT1;
	struct sockaddr_in server,client;
	unsigned int temp=0;
	UINT8 Error =1;
	RZK_THREADHANDLE_t ErrorHandle=NULL;
	UINT IntLen=0, ExtLen=0;

	Buffer = (INT8 *)malloc( (COUNT_t) size );
	if( Buffer == NULL )
	{
		xc_fprintf(CONSOLE,"\nERROR No memory");
		return ;
	}

	sockfd	=socket(AF_INET,SOCK_STREAM,0);
	if (sockfd < 0 )
    {
		xc_fprintf(CONSOLE,"\nError opening port");
		free(Buffer);
		return ;
    }

	server.sin_family	=AF_INET;			
	server.sin_port	=htons(LocalPort); 
	server.sin_addr.s_addr	=INADDR_ANY;

	if( bind(sockfd,(struct sockaddr *)&server,sizeof(struct sockaddr_in)) < 0 )
	{
		xc_fprintf(CONSOLE,"\nError In Binding Socket");
		close_s(sockfd);
		free(Buffer);
		return ;
	}

	if( listen(sockfd,1) != 0 )
	{
		xc_fprintf(CONSOLE,"\nError In Listening on Socket");
		close_s(sockfd);
		free(Buffer);
		return ;
	}

	while (1)
	{
		socketfd1=accept(sockfd,(struct sockaddr *)&client,&addrlen);
		if(socketfd1 < 0)
		{
			xc_fprintf(CONSOLE,"\nError In Accept");
			close_s(sockfd);
			free(Buffer);
			return ;
		}
	
		/* This is to prevent any unknown client for connecting and updating the firmware */
		if( (retval = recv(socketfd1,Buffer,strlen(AccessString),0)) <=  0 )
	    {
			xc_fprintf(CONSOLE,"\nFailed to Receive");
			Error=3;
			goto ErrLable;
		}
	
		/* Veryfy the password string */
		if( strncmp(Buffer,AccessString,(strlen(AccessString)-1) ))
		{
			xc_fprintf(CONSOLE,"\nUnknown Client"); 
			Error=3;
			goto ErrLable;
		}
	
		/* Send the dummy string back to the client */
		if( (retval = send(socketfd1,RetBuffer,10,tcp_FlagPUSH)) <=  0 )
			{
			xc_fprintf(CONSOLE,"\n Failed to Send");
			Error=3;
			goto ErrLable;
		}

		/* Receive the command from the client */
		if( (retval = recv(socketfd1,Buffer,20,0) ) <=  0 )
		{
			xc_fprintf(CONSOLE,"\n Failed to Receive");
			goto ErrLable;
		}
	
		/* If the received command is Configuration Querry */
		if( !strncmp(Buffer,Action1,(strlen(Action1)-1) ))
		{
//			xc_fprintf(CONSOLE,"\nConfiguration Querry"); 
			Configuration(socketfd1 ,2 );
		}
		/* If the received command is Set New Configuration */
		else if( !strncmp(Buffer,Action2,(strlen(Action2)-1) ))
		{
//			xc_fprintf(CONSOLE,"\nNew Configuration Settings");
			Configuration(socketfd1 ,1 );
	
		}
		/* If the received command is Update new firmware */
		else if( !strncmp(Buffer,Action3,(strlen(Action3)-1) ))
		{
//			xc_fprintf(CONSOLE,"\nFirmWare Updation");
			IntFlashUpdater(socketfd1);
		}
		else
		{
			xc_fprintf(CONSOLE,"\nUnknown Option"); 
			Error=3;
			goto ErrLable;
		}
	
	
ErrLable:
		close_s(socketfd1);
	}// while(1)
		
}


/*!
*\brief This function creates two server threads for supporting the Remote configuration.
*		  "Discovery" thread replies for the network scan command. "FlashConfig" thread
*		  supports the configuration querry and setting new configuration.  
*/

void Flash_config_Discovery( void )
{
 	RZK_THREADHANDLE_t DiscHndle ;

	DiscHndle = RZKCreateThreadEnhanced((UINT8*)"Discovery",discovery, NULL,2048, 25, 1,
			RZK_THREAD_PREEMPTION|RZK_THREAD_AUTOSTART,0,0) ;
	DiscHndle = RZKCreateThreadEnhanced((UINT8*)"FlashConfig",Config_Flash_Server, NULL,2048, 20, 1,
			RZK_THREAD_PREEMPTION|RZK_THREAD_AUTOSTART,0,0) ;
}

/* End of File */