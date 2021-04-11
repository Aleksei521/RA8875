/************************************************************************************
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
 ************************************************************************************/
/************************************************************************************
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
 ************************************************************************************/
/* shell.c - shell */
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZDevice.h"
#include "ZThread.h"
#include "shell.h"

#include "zfsapi.h"
#include "rtc.h"

extern RZK_DEVICE_CB_t*                                  CONSOLE ;
extern UINT RZK_MAX_DCB ;
static	INT8	errhd[] = "Syntax error\n";/* global error messages	*/
//LOCAL	char	fmt2[]  = "[%d]\n";

/** For ZTP Commands */
struct cmdent	*ShellCommands = ( struct cmdent *) NULL;
static UINT8	DefaultCommandsAdded = ( UINT8 ) FALSE;
UINT8 	g_commandType = SHELL_ZTP ;

void GetString( RZK_DEVICE_CB_t * dev, INT8 *pbuf, UINT max_len, INT* len )
{
	INT ctr = 0 ;
	INT8 *pshbuf = pbuf ;
	INT8 ch ;
	*len = 0 ;

	while( TRUE )
	{
		RZKDevRead( dev, &ch, 1 ) ;
		// Now check for the control characters
		if( ch == '\n' || ch == '\r' )
		{
			//come out of the loop:
			*pshbuf = 0 ;
			*len = ctr ;
			ch = '\n' ;
			RZKDevWrite( dev, &ch, 1 ) ;
			ch = '\r' ;
			RZKDevWrite( dev, &ch, 1 ) ;
			break ;
		}
		else if( ch == '\b' )
		{
			if( ctr > 0 )
			{
				INT8 new_ch ;
				*(pshbuf-1) = 0 ;
				pshbuf-- ;
				ctr-- ;
				new_ch = ch ;
				RZKDevWrite( dev, &ch, 1 ) ;
				ch = ' ' ;
				RZKDevWrite( dev, &ch, 1 ) ;
				RZKDevWrite( dev, &new_ch, 1 ) ;
			}
		}
		else
		{
			if( ctr < (max_len - 1 ) )
			{
				*pshbuf = ch ;
				pshbuf++ ;
				ctr++ ;
				RZKDevWrite( dev, &ch, 1 ) ;
			}
		}
	}

	return ;
}




/** @memo Make commands available to shell sessions.
 * @doc This routine adds the commands contained in the cmdent array to the
 * global list of commands which are available to shell sessions.
 *
 * @args (struct cmdent *cmds, int ncmds)
 * @param cmds An array of cmdent structures.  The cnext member of each entry will be ignored, as it will be modified when the commands are added to the global list.  The cmds[] array should be allocated from heap memory via getmem(); it should not be a local variable that is located on the stack of the caller.
 * @param ncmds The number of entries in the cmds array.
 */
void
shell_add_commands
(
   struct cmdent *cmds,
   UINT16	         ncmds,
   UINT8			 bShType
)   
{
	struct	cmdent	*toadd, *cur, *prev, *cmdSet ;
	UINT16   	i;

	cmdSet = ShellCommands ;

	for( i = 0 ; i < ncmds ; i++ )
	{
		toadd = &cmds[i];
		prev = ( struct	cmdent	*) NULL;
	
		for( cur = cmdSet; cur ; cur = cur->cnext)
		{
			if( strcmp( cur->cmdnam , toadd->cmdnam ) >= 0 )
				break;
			prev = cur;
		}

		/* Skip this one if it's already in the list */
		if( cur && !strcmp( cur->cmdnam, toadd->cmdnam ) )
				continue;

		toadd->cnext = cur;
		if( prev )
			prev->cnext = toadd;


		/* 
		 * Reset ShellCommands to this entry if this is now the 
		 * 1st one.
		 */
		if( cmdSet == cur)
			cmdSet = toadd;
	}

	ShellCommands = cmdSet;
}
		
/*------------------------------------------------------------------------
 *  shell  -  Xinu shell with file redirection and background processing
 *------------------------------------------------------------------------
 */
INT16 shell(RZK_DEVICE_CB_t *	dev)
{
	struct	shvars	Shl;
	INT16	ntokens;
	UINT16	i, j;
	INT    len;
	struct cmdent	*com;
	INT8	*outnam, *innam;
	RZK_DEVICE_CB_t *	stdin, *stdout, *stderr;
	UINT8	backgnd;
	INT8	ch;

	// ZFS shell variables
	INT8 *pshbuf ;
//	UINT ctr = 0 ;

	//

	/** MEENAKSHI added the following */
	INT8	mach[265] ;
	INT8 cwd_path[256];

//	PID	child;
	if(!DefaultCommandsAdded) {
		DefaultCommandsAdded = TRUE;
		shell_add_commands(defaultcmds, ndefaultcmds,SHELL_ZTP); // The default command set should also include hte FS commands
	}
	
	/** Make the current working directory as root */
	/** MEENAKSHI ADDED NEWLY */
	if( ZFSChdir((INT8*)"/") != ZFSERR_SUCCESS )
	{
	}
	printf( "\n");
	while (TRUE) {

		strcpy(mach, "eZ80 %");
		memset(cwd_path, 0x00, sizeof(cwd_path));
		
		if( ZFSGetCwd((INT8*)cwd_path) == ZFSERR_SUCCESS )
		{
			sprintf(mach+4, "%s %%", cwd_path );
		}

		printf("%s", mach);			
		stdin = stdout = stderr = dev;

		// read a character by character and then pass it to check for various characters.

		pshbuf = &Shl.shbuf[0] ;
		len = 0 ;
		//ctr = 0 ;

		GetString( dev, pshbuf, SHBUFLEN, &len ) ;


		if( len == 0 )
			continue ;

		if (len == ZTP_EOF)
			break;
			
		if (len <= (-1))
        		break;
			
                ntokens=lexan(&Shl, Shl.shbuf);
		if (ntokens == (-1)) 
		{
			printf( errhd);
			continue;
		} 
		else if (ntokens == 0)
			continue;
		outnam = innam = ( INT8 * ) NULL;
		backgnd = FALSE;

		/* handle '&' */
		if (Shl.shtktyp[ntokens-1] == '&') 
		{
			ntokens-- ;
			backgnd = TRUE;
		}

		/* scan tokens, accumulating length;  handling redirect	*/
		for (len=0,i=0 ; i<ntokens ; ) 
		{
			if ((ch = Shl.shtktyp[i]) == '&') 
			{
				ntokens = -1;
				break;
			} 
			else if (ch == '>') 
			{
				if (outnam != NULL || i >= --ntokens) 
				{
					ntokens = -1;
					break;
				}
				outnam = Shl.shtok[i+1];
				for (ntokens--,j=i ; j<ntokens ; j++) 
				{
					Shl.shtktyp[j] = Shl.shtktyp[j+2];
					Shl.shtok  [j] = Shl.shtok  [j+2];
				}
				continue;
			}
			else if (ch == '<') 
			{
				if (innam != NULL || i >= --ntokens) 
				{
					ntokens = -1;
					break;
				}
				innam = Shl.shtok[i+1];
				for (ntokens--,j=i ; j < ntokens ; j++) 
				{
					Shl.shtktyp[j] = Shl.shtktyp[j+2];
					Shl.shtok  [j] = Shl.shtok  [j+2];
				}
				continue;
			}
			else 
			{
				 len += strlen(Shl.shtok[i++]);
			}
		}
		if (ntokens <= 0) 
		{
			printf( errhd);
			continue;
		}

		/* Look up command in table */
		for (com=ShellCommands; com; com=com->cnext) 
		{  
			if ( strcmp( com->cmdnam, Shl.shtok[0] ) == 0)
				break;
		}

		if (!com)
		{
			printf( "%s: not found\n", Shl.shtok[0]);
			continue;
		}
		
		/* handle built-in commands with procedure call */
		if (com->cbuiltin) 
		{
			if (innam != NULL || outnam != NULL || backgnd)
				printf( errhd);
			else if ( (*(com->cproc))(&Shl, stdin, stdout,
				stderr, ntokens, (INT8**)Shl.shtok) == SHEXIT)
				break;
			continue;
		}	
	}

	return(OK);
}



void DisplayTime( void ) 
{
	TIME Time;
	TIME *pTime = &Time;

	RZK_DEVICE_CB_t *pdev ;

	pdev = RZKDevOpen( "RTC", 0 ) ;
	if( pdev == NULL )
	{
		printf("\nRTC is not present");
		return ;
	}
	
	RZKDevRead(pdev,(INT8 *)pTime,8);

	if((pTime->mon > 12) || (pTime->mon <= 0) ||
	(pTime->dayOfMonth > 31) || (pTime->dayOfMonth <= 0) ||
	(pTime->dayOfWeek > 7) || (pTime->dayOfWeek <= 0) ||
	(pTime->hrs >= 24)  ||	(pTime->minutes >= 60) || 
	(pTime->sec >= 60) )
	{
		printf("\nPlease set date and time in RTC\n");
	}
	else
	{
		printf("\n%-4s, %2d %-4s %4d %3d:%2d:%2d\n",
			((INT)pTime->dayOfWeek == 1)?"Mon":
			((INT)pTime->dayOfWeek == 2)?"Tue":
			((INT)pTime->dayOfWeek == 3)?"Wed":
			((INT)pTime->dayOfWeek == 4)?"Thu":
			((INT)pTime->dayOfWeek == 5)?"Fri":
			((INT)pTime->dayOfWeek == 6)?"Sat":
			((INT)pTime->dayOfWeek == 7)?"Sun":
			"---",
			(INT)pTime->dayOfMonth,
			((INT)pTime->mon == 1)?"Jan":
			((INT)pTime->mon == 2)?"Feb":
			((INT)pTime->mon == 3)?"Mar":
			((INT)pTime->mon == 4)?"Apr":
			((INT)pTime->mon == 5)?"May":
			((INT)pTime->mon == 6)?"Jun":
			((INT)pTime->mon == 7)?"Jul":
			((INT)pTime->mon == 8)?"Aug":
			((INT)pTime->mon == 9)?"Sep":
			((INT)pTime->mon == 10)?"Oct":
			((INT)pTime->mon == 11)?"Nov":
			((INT)pTime->mon == 12)?"Dec":
			"---",			
			((INT)pTime->cent * 100 + (INT)pTime->year),(INT)pTime->hrs, (INT)pTime->minutes,(INT)pTime->sec);
	}

	return ;
}






/** @memo Initialize a shell on a device.
 * @doc This routine adds the commands contained in the cmdent array to the
 * global list of commands which are available to shell sessions.
 *
 * @args (struct cmdent *cmds, int ncmds)
 * @param cmds An array of cmdent structures.  The cnext member of each entry will be ignored, as it will be modified when the commands are added to the global list.  The cmds[] array should be allocated from heap memory via getmem(), or should be a global variable; it should not be a local variable that is located on the stack of the caller.
 * @param ncmds The number of entries in the cmds array.
 */
void Shell_Entry( void )
{
	UINT ctr = 0 ;
	PZFS_VOL_PARAMS_t pvol_params, ptmp_vol ;
	ZFS_STATUS_t status ;
	RZK_DEVICE_CB_t * pDev = CONSOLE ;

	ctr = ZFSGetVolumeCount();
	if( ctr <= 0 )
	{
		printf( "\nError in getting the number of volumes in FileSystem");
		return ;
	}

	pvol_params = ( PZFS_VOL_PARAMS_t ) malloc( sizeof( ZFS_VOL_PARAMS_t ) * ctr ) ;
	if( pvol_params == NULL )
	{
		printf( "\nError in allocating memory");
		return ;
	}


	// Now initialize the FileSystem.
	printf( "\nInitializing the File System, Please wait..." ) ;
	status = ZFSInit( pvol_params ) ;
	if( status == ZFSERR_SUCCESS )
		printf( "Success") ;
	else
	{
		printf( "Failed") ;
		printf("\n Invalid volumes are :") ;
		ptmp_vol = pvol_params ;
		for( ; status > 0; status-- )
		{
			printf("\nVolume Name : %-15s", ptmp_vol->vol_name ) ;
			ptmp_vol++;
		}
		printf("\nFormat the volume(s) to make it valid");
	}

    DisplayTime() ;
	shell( pDev ) ;
	return ;
}

