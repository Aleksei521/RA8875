/*
 * File       :     ZTPUserDetails.c
 *
 * Description:  This file contains the supporting functions to
 *			  add, delete and Search user login details
 *
 * Copyright 2004 ZiLOG Inc.  ALL RIGHTS RESERVED.
 *
 * This file contains unpublished confidential and proprietary information
 * of ZiLOG, Inc.
 * NO PART OF THIS WORK MAY BE DUPLICATED, STORED, PUBLISHED OR DISCLOSED 
 * IN ANY FORM WITHOUT THE PRIOR WRITTEN CONSENT OF ZiLOG, INC.
 * This is not a license and no use of any kind of this work is authorized
 * in the absence of a written license granted by ZiLOG, Inc. in ZiLOG's 
 * sole discretion 
 */

/*\
 *=========================================================================*
 *							Revision History
 *=========================================================================*
 * [1] MK on 25th January, 2005
 * As the protocols that require login name and password
 * has the common code base, this file has been created newly
 * and moved part of the code from x_ftpusers.c to this file.
 *
 * [2]
 * CR 5131, MK on 20th January, 2005
 * Changed the addUserInfo implementation to save the user information
 * in the file rather than the storing in a static array.
 *
 * Also changed searchUser and deleteUserInfo() for similar changes.
 *
 * [3]
 * MK on 20th January, 2005
 * Rearranged the code, the size of each line is limited to 80 characters 
 *
 * [4]
 * MK on 25th January, 2005
 * Removed unnecessary Header file inclusions.
 *
 * [5]
 * MK on 6th February, 2005
 * Code is kept under the macro RZKFS when the
 * user name and password details have to be stored
 * in a File. If File system is not used, static arrays
 * are used to store the password and user name.
\*/

/** Standard Include Files */
#include <string.h>
/** RZK Include Files */
#include "ZTypes.h"

/** Shell Includes */
#include "shell.h"
/** File System Includes */
#include "Cfileapi.h"
/** App Includes */
#include "ZTPuserdetails.h"

extern RZK_STATE_t RZKDisablePreemption( void );
extern void RZKRestorePreemption(RZK_STATE_t);

extern  RZK_DEVICE_CB_t*   CONSOLE;

#ifdef RZKFS

/** GLOBAL VARIABLES ***/
/* MK$CR#5131$ 20Jan2005 */
UINT32 DEF_FILE_SIZE ;
INT8 *UsrListFileName = "ZTPUsrDtls.txt";

/** 
	This function searches the file and returns the 
    contents of the file
**/
UINT8  *getFileContents()
{
	UINT8	*buffer ;
	INT	size = -1;
	FILE    *ftpUsrListFile ;
	
	/** Get the memory */
	buffer = malloc( DEF_FILE_SIZE );
	if( buffer == NULL )
		return ( NULL ) ;

	memset( buffer, 0x00, DEF_FILE_SIZE );
	
	/** Get the size and contents of the file */
	ftpUsrListFile = fopen( UsrListFileName , "r" ) ;
	if( ftpUsrListFile )
	{
		/** Get the size of the file */
		size = fread(buffer ,1, DEF_FILE_SIZE, ftpUsrListFile )  ;
		fclose( ftpUsrListFile );
		
	}
	if( size < (INT)0 )
	{
		//XTLRM freemem( buffer, DEF_FILE_SIZE );
		free(buffer);
		return ( NULL ) ;
	}
	return buffer ;
	
	
}
/**
	Adds the user Information to the file
**/
INT8 addUserInfo(	INT8 	*usrName,
					INT8 	*passwd
			   )
{
	FILE  				*ftpUsrListFile ;
	INT16 				num ;	
	//RK :4th Jan 2006 : Changed the below line during IAR
	//size_t 				size ;
	INT 				size ;
	UINT8  				*buffer;
	USERDETAILS 		*ptrMaxUsers ;

	DEF_FILE_SIZE = ( g_maxNumUsers * sizeof( USERDETAILS ) ) ;
	
	if ( 	(strlen(usrName)>ZTP_USR_NAME_LEN) ||
			(strlen(passwd)>ZTP_USR_PSWD_LEN)  ||
			(usrName == NULL) ||
			(passwd == NULL)
	   )
	{
#ifdef SPIDER_FTP_DEBUG		
		xc_fprintf(CONSOLE,"\nName Error");
#endif
		return ( -1 );
	}
	
        /** If the file does not exist */
	if( ( ftpUsrListFile =  fopen( UsrListFileName, "r" ) ) == NULL )
	{
		USERDETAILS  defaultUser= {0};
		fclose(ftpUsrListFile);
		strcpy( defaultUser.usrName, usrName );
		strcpy( defaultUser.password, passwd );
		ftpUsrListFile = fopen( UsrListFileName, "w" );
		if( ftpUsrListFile )
		size = fwrite( &defaultUser, 1, sizeof( USERDETAILS ) , ftpUsrListFile );
		fclose(ftpUsrListFile);
		return ( ZSHELL_SUCCESS );
	}
	fclose( ftpUsrListFile );

	/** Search the list */
	/** Check for the duplicate Entry */
	if( searchUser( usrName, passwd ) == 0 )
		return (ZTP_DUPLICATE_USER);

	/* MK$CR#5131$ 20Jan2005 */
	/*==============================================*/
	buffer = getFileContents();
	if( !buffer )
		return ( -1 );
	
	ptrMaxUsers = (USERDETAILS *)buffer ;

	/** Check for any free space to add the user details */
	for( num = 0 ; num < g_maxNumUsers; num++ )
	{
		/** Check the space in the file to add the new user details */
		if ( ( ptrMaxUsers->usrName[0] )== 0 )
		{
			/** Copy the user contents to the buffer */
			strcpy(ptrMaxUsers->usrName,usrName);
			strcpy(ptrMaxUsers->password,passwd);

			/** Copy the buffer contents to the file */			
			ftpUsrListFile = fopen(UsrListFileName, "w" );
			size = fwrite(buffer, 1, DEF_FILE_SIZE , ftpUsrListFile ) ;
			if( size < 0 )
				num = g_maxNumUsers;
			break ;			
		}		
		ptrMaxUsers++;
	}
	/** Close the file */
	fclose( ftpUsrListFile );
	/** Free the allocated memory */
	//AJ-XTLRM freemem( buffer, DEF_FILE_SIZE );
	free (buffer);
	/*==============================================*/

	/** Return error if no space */
	if (num==g_maxNumUsers)
	{
#ifdef SPIDER_FTP_DEBUG		
		xc_fprintf(CONSOLE,"\nuser List Full");
#endif
		return ( -1 );
	}

	/** Return Success */
	return ( ZSHELL_SUCCESS );
}

 /*\
  * 
  * The following function searches the user name
  * and the password in the file 
  *
 \*/
INT8  searchUser(	INT8 	*usrName,
					INT8 	*passwd
				   )
{

	INT16 				num ;
	UINT8  				*buffer;
	USERDETAILS 		*ptrMaxUsers ;
	
	if ((strlen(usrName)>ZTP_USR_NAME_LEN) ||
	    (strlen(usrName)==0)||
		(strlen(passwd)>ZTP_USR_PSWD_LEN) ||
		(strlen(passwd)==0) ||
		(usrName == NULL) ||
		(passwd == NULL) 			
	   )

	{
#ifdef SPIDER_FTP_DEBUG		
		xc_fprintf(CONSOLE,"\nName Error");
#endif
		return (-1);
	}

	/* MK$CR#5131$ 20Jan2005 */
	/*==============================================*/
   	buffer = getFileContents();
	if( !buffer )
		return ( ZTP_FILE_DOES_NOT_EXIST ) ;
	
	ptrMaxUsers = (USERDETAILS *)buffer ;
	
	for ( num = 0;num < g_maxNumUsers ; num++)
	{
		if ( ( !strcmp( ptrMaxUsers->usrName,usrName ) )&&
		     ( !strcmp(ptrMaxUsers->password,passwd ) )
		   )
		{
			break ;			
		}
		else
			ptrMaxUsers++;
	}

	//XTLRM freemem( buffer, DEF_FILE_SIZE );
	free (buffer);

	/*==============================================*/
	if ( num==g_maxNumUsers )
	{
#ifdef SPIDER_FTP_DEBUG		
		xc_fprintf(CONSOLE,"\nuser List Full");
#endif
		return ( -1 );
	}
	return ( ZSHELL_SUCCESS );
}

/*\
 *
 *	Deletes the user Information from the file
 *
\*/
INT8  deleteUserInfo( INT8 *usrName )
{
	INT16 			num ;
	//RK :4th Jan 2006 : Changed the below line during IAR
	//size_t 				size = -1 ;
	INT 				size = -1 ;
	UINT8			*buffer ;
	USERDETAILS 	*ptrMaxUsers ;	
	FILE 			*ftpUsrListFile ;

	if ( usrName == NULL )
	{
#ifdef SPIDER_FTP_DEBUG		
		xc_fprintf(CONSOLE,"\nName Error");
#endif
		return ( -1 );
	}

	/* MK$CR#5131$ 20Jan2005 */
	/*==============================================*/

	/** Get the memory */
	buffer = getFileContents() ;
	if( !buffer )
		return (-1) ;

	ptrMaxUsers = (USERDETAILS*)buffer ;
	
	/** Search the record in the file */
	for( num = 0; num < g_maxNumUsers ; num++ )
	{
		/** If match is found */
		if ( !strcmp( ptrMaxUsers->usrName,usrName ) )
		{
			/** Free the record */
			memset( ptrMaxUsers, 0, sizeof( USERDETAILS ) );
			
			ftpUsrListFile = fopen(UsrListFileName , "w" ) ;
			if( ftpUsrListFile )
			{
				size =  fwrite(buffer, 
						  1, 
						  DEF_FILE_SIZE ,
						 ftpUsrListFile ) ; 
			}
			fclose( ftpUsrListFile ); // MK $ 6-JUN-2007
			break ;
		}
		ptrMaxUsers++;
	}
	//XTLRM freemem( buffer, DEF_FILE_SIZE );
	free (buffer);
	
	
	/*==============================================*/
	if( num == g_maxNumUsers || size < 0  )
	{
#ifdef SPIDER_FTP_DEBUG		
		xc_fprintf(CONSOLE,"\nUser not found");
#endif
		return -2;
	}
        return OK; //RK :17th Jan 2006 : Added during IAR
}

#else

extern INT8  g_szmaxUsrs[][FTP_USR_DATA_SIZE]; 
USERDETAILS	*MaxUsers = (USERDETAILS *)g_szmaxUsrs;

INT8 addUserInfo(INT8 *usrName,INT8 *passwd)
{
	INT16 num=0;
	UINTRMASK msk=0;
	USERDETAILS *ptrMaxUsers=MaxUsers;
	if (	(strlen(usrName)>ZTP_USR_NAME_LEN) ||
		(strlen(passwd)>ZTP_USR_PSWD_LEN) ||
		(usrName == NULL) ||
		(passwd == NULL))
	{
#ifdef SPIDER_FTP_DEBUG		
		xc_fprintf(CONSOLE,"\nName Error");
#endif
		return -1;
	}

	if( searchUser(usrName,passwd ) == 0 )
		return( ZTP_DUPLICATE_USER );
		
	for(num=0;num<g_maxNumUsers;num++)
	{
		if ( (ptrMaxUsers->usrName[0])==0)
		{
			msk=RZKDisablePreemption();
			strcpy(ptrMaxUsers->usrName,usrName);
			strcpy(ptrMaxUsers->password,passwd);
			RZKRestorePreemption(msk);
			return 1;
		}
		ptrMaxUsers++;
	}
	if (num==g_maxNumUsers)
	{
#ifdef SPIDER_FTP_DEBUG		
		xc_fprintf(CONSOLE,"\nuser List Full");
#endif
		return -1;
	}
	return 0;
}

INT8 searchUser(INT8 *usrName,INT8 *passwd)
{
	INT16 num=0;
	USERDETAILS *ptrMaxUsers=MaxUsers;

	if (	(strlen(usrName)>ZTP_USR_NAME_LEN) ||
	    (strlen(usrName)==0)||
		(strlen(passwd)>ZTP_USR_PSWD_LEN) ||
		(strlen(passwd)==0) ||
		(usrName == NULL) ||
		(passwd == NULL) 			
	   )	
	{
		return (-1);
	}
	for (num=0;num<g_maxNumUsers;num++)
	{
		if ((!strcmp(ptrMaxUsers->usrName,usrName))&&(!strcmp(ptrMaxUsers->password,passwd)))
			//return ptrMaxUsers;
			break ;
		else
			ptrMaxUsers++;
	}
	if (num==g_maxNumUsers)
	{
		return ( -1 );
	}

	return ( ZSHELL_SUCCESS ) ;
}

INT8  deleteUserInfo(INT8 *usrName)
{
	INT16 num=0;
	UINTRMASK msk=0;
	USERDETAILS *ptrMaxUsers=MaxUsers;
	if (	(strlen(usrName)>ZTP_USR_NAME_LEN) ||
			(usrName == NULL)  )
	{
		return -1;
	}
	for (num=0;num<g_maxNumUsers;num++)
	{
		if (!strcmp(ptrMaxUsers->usrName,usrName))
		{
			
			msk=RZKDisablePreemption();
			ptrMaxUsers->usrName[0]=0;
			ptrMaxUsers->password[0]=0;
			RZKRestorePreemption(msk);
			return 1;
		}
		ptrMaxUsers++;
	}
	if (num==g_maxNumUsers)
	{
		return -2;
	}
	return 0;
}
#endif

