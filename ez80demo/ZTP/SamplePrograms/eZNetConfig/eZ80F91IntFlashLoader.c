/*!
*\file  eZ80F91IntFlashLoader.c
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
#include <stdio.h>
#include <string.h>
#include"ZSysgen.h"
#include"ZTypes.h"
#include"ZThread.h"
#include "ZTimer.h"
#include "ZClock.h"
#include "ZRegion.h"
#include "Shell.h"
#include "socket.h"
#include "ZTPtcp.h"
#include "FlashLib.h"


/*	 Constant Definition */
//#define FIRMWARE_DEBUG	1	 /* Enable/Disable debug */

/* Global Variable Declaration/Definition */
char Length[5];
char RecType[3];
char Adresses1[6];
char Offset[5];
unsigned short OffsetVar=0;
unsigned short AdressesVar=0;
unsigned short LengthVar=0;
unsigned short RecTypeVar=0;
unsigned long FinalAddr=0;

/* External Variable Declaration */
extern RZK_DEVICE_CB_t * CONSOLE;
extern char RetBuffer[];

/*!
*\brief This function erases the internal flash of eZ80F91.
*/
void EraseIntFlash( void )
{
   /*
    * Disable Write-Protection on all 8 blocks of internal Flash and erase 
    * all pages of internal Flash - Do not erase the Information Page.
    */
   IFL_Init( 0 );
   IFL_Erase( FALSE );
}

/*!
*\brief This function is called to update the ZTP firmware. It erases the internal 
*		  flash first and then updates the internal flash with the new firmware.
*/
INT16 IntFlashUpdater( INT16	sockfd )
{
	INT16 i,retval;
	INT8 *Buffer;
	INT16 size = 600;
	UINT8 Error =1;

	xc_fprintf(CONSOLE,"\n ***Updating the ZTP Firmware*** \n");

	Buffer = (INT8 *)malloc( (COUNT_t) size );
	if( Buffer == NULL )
	{
		xc_fprintf(CONSOLE,"\nERROR No memory");
		return 0;
	}

	EraseIntFlash();

	for( ;1;)
	{

		if( (retval = recv(sockfd,Buffer,size,0)) <=  0 )
	    {
			xc_fprintf(CONSOLE,"\n Failed to Receive");
			break;
		}
		strncpy(Length,Buffer,4);
		strncpy(RecType,&Buffer[4],2);
		strncpy(Adresses1,&Buffer[6],5);

		Length[4]=0;
		RecType[2]=0;
		Adresses1[5]=0;
		
		AdressesVar = atol(Adresses1);
		LengthVar = atol(Length);
		RecTypeVar = atoi(RecType);

		if( RecTypeVar == 1 )
		{
#ifdef FIRMWARE_DEBUG
			xc_fprintf(CONSOLE,"End of the Hex Record \n");
#endif
			Error=0;
			break;
		}

		if( RecTypeVar == 0 )
		{
			FinalAddr = AdressesVar + (OffsetVar*0x010000);
#ifdef FIRMWARE_DEBUG
			xc_fprintf(CONSOLE,"RecType: %d VarLength: %d Addresses: %U \n",RecTypeVar,LengthVar,FinalAddr);
#endif
			asm("\tdi");
         IFL_Program( (HANDLE)FinalAddr, &Buffer[12], LengthVar );
			asm("\tei");
		}
		if( RecTypeVar == 4 )
		{
			OffsetVar = *(unsigned short *)&Buffer[12];
			OffsetVar = intel16(OffsetVar);
#ifdef FIRMWARE_DEBUG
			xc_fprintf(CONSOLE,"\nRecType: %d VarLength: %d Addresses: %U ",RecTypeVar,LengthVar,FinalAddr);
			xc_fprintf(CONSOLE," Offset = %d \n\n",OffsetVar);
#endif
		}

		if( (retval = send(sockfd,RetBuffer,10,tcp_FlagPUSH)) <=  0 )
	    {
			xc_fprintf(CONSOLE,"\n Failed to Send");
			break;
		}
	}

ErrLable:
	close_s(sockfd);
	free(Buffer);

	if( Error )
	{
		xc_fprintf(CONSOLE,"\n *** ERROR Updating ZTP ***\n");
		xc_fprintf(CONSOLE,"\n *** Please Update the ZTP again ***\n");
		return 0;
	}
	else
	{
		xc_fprintf(CONSOLE,"\n\nZTP FirmWare Updated");
		xc_fprintf(CONSOLE,"\nSystem Will Reboot now\n");
	}


	RZKSuspendThread( RZKGetCurrentThread(), 2*RZK_SYSTICKS_INSEC ) ;
	asm("di");
	asm("rst 0");
	return 1;
}

/* End of File */