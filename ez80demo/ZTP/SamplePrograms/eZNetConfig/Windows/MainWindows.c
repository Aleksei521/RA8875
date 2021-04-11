/*!
*\file  MainWindows.c
*\brief This files contains code for supporting the main menue on windows side.
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

/* Global Variable Declaration/Definition */
char *AccessString = "Authentication";
char *ActionString1="FIRMWA";
char *ActionString2="QUERRY";
char *ActionString3="SETPAR";

/* External Function Declaration */
extern void discovery( void );
extern void FirmWareUpdate(void);
extern void Configuration(void );

/*!
*\brief This function displays the main menu
*/
void main( void )
{
	WSADATA wsaData;
	unsigned short  Loopbreak;
	char Option;

	if (WSAStartup(0x202,&wsaData) == SOCKET_ERROR) 
	{
		printf("WSAStartup failed with error %d\n",WSAGetLastError());
		WSACleanup();
		exit(1);
	}	

	while(1)
	{
		do
		{
			printf("\n\n1 - NETWORK SCAN  \n2 - CONFIGURATION \n3 - FIRMWARE UPDATION \n4 - EXIT\nEnter the Option > ");
			fflush(stdin);
			scanf("%c",&Option);
			fflush(stdin);

			if(Option == '4' )
				exit(0);

			if( (Option == (char)'3') || (Option == (char)'2') || (Option == (char)'1') )
			{
				break;
			}


		}while(1);


		if( Option == (char)'1' )
		{
			printf("\nNETWORK SCAN\n");
			printf("===============\n");
			discovery();
		}
		else if(Option == (char)'2' )
		{
			printf("\nCONFIGURATION\n");
			printf("================\n");
			Configuration();
		}
		else if(Option == (char)'3' )
		{
			printf("\nFIRMWARE UPDATION\n");
			printf("===================\n");
			FirmWareUpdate();
		}
	}
}

/* End of File */