/************************************************************************************
 * File       :     fs_shell_Commands.c
 *
 * Description:		This file contains the implementation of FS 
 *					Shell commands.
 *
 * Copyright 2016 ZiLOG Inc.  ALL RIGHTS RESERVED.
 *
 * This file contains unpublished confidential and proprietary information
 * of ZiLOG, Inc.
 * NO PART OF THIS WORK MAY BE DUPLICATED, STORED, PUBLISHED OR DISCLOSED 
 * IN ANY FORM WITHOUT THE PRIOR WRITTEN CONSENT OF ZiLOG, INC.
 * This is not a license and no use of any kind of this work is authorized
 * in the absence of a written license granted by ZiLOG, Inc. in ZiLOG's 
 * sole discretion 
 ************************************************************************************/

// standard headers
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/** RXK Includes */
#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZDevice.h"
#include "ZThread.h"

/** File system Includes */
#include "ZFSTypes.h"
#include "ZFSApi.h"
#include "ZFSCfg.h"
/** Shell Includes */
#include "Shell.h"
#include "FS_Shell.h"

#include "rtc.h"
#define	COLUMNS	4

#define MAX_FILE_PATH_SIZE	(256)

extern void GetString( RZK_DEVICE_CB_t * dev, INT8 *pbuf, UINT max_len, INT* len ) ;

/** This function separates Directory name and the file Name */
void Shell_ZFSGetDirFileName( INT8		*pathName,
							  INT8		*dirName,
							  INT8		*argv[]
							 )
{
	INT16 len ;
	UINT8 checkForCwd = 1;
	INT8 *path;
	INT16 i = 0;

	/** Get the length */
	len = strlen( argv[1] );
	path = (INT8 *)argv[1];

	/** Get the directory Name alone */
	while( len != 0 )
	{
		if( *path == '/' )
		{
			i = 0;
			checkForCwd = 0;
			path++;
		}
		dirName[i] = *path++;
		i++;
		len--;
	}
	dirName[i] = '\0' ;

	if( checkForCwd == 0 )
	{
		/** Get the path name alone */
		memcpy(pathName, argv[1], strlen(argv[1])-strlen((const INT8 *)dirName));
	}
	else
		strcpy((INT8 *)pathName, "./");
}


/* The following function is used to create a 
   directory given by the user */
INT16 Shell_ZFSMkdir (
	struct shvars *			Shl,
	RZK_DEVICE_CB_t *						stdin,
	RZK_DEVICE_CB_t *						stdout, 
	RZK_DEVICE_CB_t *						stderr, 
	UINT16					nargs,
	INT8					*argv[]
)
{
	INT8 *pathName = ( INT8* ) NULL ;
	INT8 *dirName = ( INT8* ) NULL ;
	ZFS_STATUS_t status;
	INT16	ret_val = ZSHELL_FAILURE;

	/** Validate the number of arguments */
	if( nargs != 2 )
	{
		printf("\nThe syntax of the command is incorrect\n");
		printf("md <directory Name>\n");
		return ZSHELL_FAILURE ;
	}

	pathName = ( INT8* ) malloc( MAX_FILE_PATH_SIZE ) ;
	dirName = ( INT8* ) malloc( MAX_FILE_PATH_SIZE ) ;

	if( pathName == NULL || dirName == NULL )
		goto _end_Shell_ZFSMkdir ;

	memset( pathName, 0x00, MAX_FILE_PATH_SIZE ) ;
	memset( dirName, 0x00, MAX_FILE_PATH_SIZE ) ;
	Shell_ZFSGetDirFileName( pathName, dirName, (INT8 **)argv );

	/** Call the Make directory API of ZFS */
	status = ZFSMkdir( pathName, dirName );
	if( status == ZFSERR_SUCCESS )
	{
		printf("\nA subdirectory %s is created\n", argv[1]);
		ret_val = ZSHELL_SUCCESS ;
		goto _end_Shell_ZFSMkdir ;
	}
	else
	if ( status == ZFSERR_FILE_DIR_ALREADY_EXISTS )
	{
		printf("\nA subdirectory or file %s already exists\n", dirName );
	}
	else if( status == ZFSERR_FILE_DIR_DOES_NOT_EXIST )
	{
		printf("\nThe subdirectory %s does not exist\n", pathName );
	}
	else if( status == ZFSERR_INVALID_FILEDIR_PATH )
	{
		printf("\nThe directory path is invalid\n" );
	}
	else if( status == ZFSERR_INVALID_FILE_DIR_NAME )
	{
		printf("\nThe directory name syntax is incorrect.\n");
	}
	else if( status == ZFSERR_INVALID_VOLUME )
	{
		printf("\nAccessed volume is invalid. Format the volume to make it valid\n");
	}
	else if( status != ZFSERR_SUCCESS )
	{
		printf("\nmd failed to create a directory\n");
	}

_end_Shell_ZFSMkdir:
	if( pathName )
		free( pathName ) ;
	if( dirName )
		free( dirName ) ;

	return ret_val;
}

/* The following function is used to change the 
   current working directory to a new directory given by 
   the user */
INT16 Shell_ZFSChdir (
	struct shvars *			Shl,
	RZK_DEVICE_CB_t *						stdin,
	RZK_DEVICE_CB_t *						stdout, 
	RZK_DEVICE_CB_t *						stderr, 
	UINT16					nargs,
	INT8					*argv[]
)
{
	INT8		*pcwd = ( INT8* ) NULL ;
	INT16 ret_val = ZSHELL_FAILURE;
	
	/** Validate the number of arguments */
	if( nargs != 2 )
	{
		printf("\nThe syntax of the command is incorrect\n");
		printf("cd <directory Name>\n");
		return ZSHELL_FAILURE ;
	}

	pcwd = ( INT8* ) malloc( MAX_FILE_PATH_SIZE ) ;
	if( pcwd == NULL )
		goto end_Shell_ZFSChdir ;

	memset( pcwd, 0x00, MAX_FILE_PATH_SIZE ) ;
	/** Call the change directory API of ZFS */
	if ( ZFSChdir( argv[1] ) )	
	{
		printf("\nA subdirectory %s does not exist or accessing an invalid volume\n", argv[1]);
		goto end_Shell_ZFSChdir ;
	}
	if( !ZFSGetCwd(pcwd) )
		printf("\nCWD is changed to %s\n", pcwd);
	
		ret_val = ZSHELL_SUCCESS ;

end_Shell_ZFSChdir :
	if( pcwd )
		free(pcwd);

	return ret_val;

}

/* The following function is used to delete a 
   file given by the user */
INT16 Shell_ZFSDelFile (
	struct shvars *			Shl,
	RZK_DEVICE_CB_t *						stdin,
	RZK_DEVICE_CB_t *						stdout, 
	RZK_DEVICE_CB_t *						stderr, 
	UINT16					nargs,
	INT8					*argv[]
)
{
	/** Validate the number of arguments */
	if( nargs != 2 )
	{
		printf("\nThe syntax of the command is incorrect\n");
		printf("del <file Name>\n");
		return ZSHELL_FAILURE ;
	}
	/** Call the delete directory API of ZFS */
	if( ZFSDelete( argv[1] ) )	
	{
		printf("\nUnable to delete %s or accessing an invalid volume\n", argv[1]);
		return ZSHELL_FAILURE ;
	}
	printf("\nFile %s is deleted\n", argv[1]);
	return ZSHELL_SUCCESS;
}

/** The following function deletes dir*/
INT16 ShellDeleteDir( UINT16   nargs, 
					    INT8   *argv[], 
					    UINT8  delMode,
 					    RZK_DEVICE_CB_t *		stdout
					  )
{
	/** Validate the number of arguments */
	if( nargs != 2 )
	{
		printf("\nThe syntax of the command is incorrect\n");
		printf("deldir <directory Name>\n");
		return ZSHELL_FAILURE ;
	}
	if( ZFSDeleteDir(argv[1],delMode) )	
	{
		printf("\nUnable to delete the directory %s or accessing an invalid volume\n", argv[1] );
		return ZSHELL_FAILURE ;
	}
	printf("\nDirectory %s is deleted\n", argv[1]);
	return ZSHELL_SUCCESS;
}

/* The following function is used to delete a 
   directory specified by the user */
INT16 Shell_ZFSDelDirAll (
	struct shvars *			Shl,
	RZK_DEVICE_CB_t *						stdin,
	RZK_DEVICE_CB_t *						stdout, 
	RZK_DEVICE_CB_t *						stderr, 
	UINT16					nargs,
	INT8					*argv[]
)
{
	return( ShellDeleteDir( nargs, argv, ZFS_TRUE,stdout ) );
}

/* The following function is used to delete  
   the entire directory along with files/subdirectories */
INT16 Shell_ZFSDelDir(
	struct shvars *			Shl,
	RZK_DEVICE_CB_t *						stdin,
	RZK_DEVICE_CB_t *						stdout, 
	RZK_DEVICE_CB_t *						stderr, 
	UINT16					nargs,
	INT8					*argv[]
)
{
	return( ShellDeleteDir( nargs, argv, ZFS_FALSE, stdout) );	
}

/* The following function is used to rename a file
   specified by the user */
INT16 Shell_ZFSRenFile (
	struct shvars *			Shl,
	RZK_DEVICE_CB_t *						stdin,
	RZK_DEVICE_CB_t *						stdout, 
	RZK_DEVICE_CB_t *						stderr, 
	UINT16					nargs,
	INT8					*argv[]
)
{
	ZFS_STATUS_t status ;

	/** Validate the number of arguments */
	if( nargs != 3 )
	{
		printf("\nThe syntax of the command is incorrect\n");
		printf("ren <file Name1> <fileName2>\n");
		return ZSHELL_FAILURE ;
	}
	status = ZFSRename(argv[1], argv[2]);
	if( status == ZFSERR_FILE_DIR_DOES_NOT_EXIST )
	{
		printf("\nThe specified file does not exist\n");
		return ZSHELL_FAILURE;
	}
	else if( status != ZFSERR_SUCCESS )
	{
		printf("\nUnable to rename the file %s or accessing an invalid volume\n", argv[1] );
		return ZSHELL_FAILURE ;
	}	
	printf("\nFile %s is renamed as %s\n", argv[1], argv[2]);
	return ZSHELL_SUCCESS;
}

/* The following function is used to rename a directory
   specified by the user */
INT16 Shell_ZFSRenDir (
	struct shvars *			Shl,
	RZK_DEVICE_CB_t *						stdin,
	RZK_DEVICE_CB_t *						stdout, 
	RZK_DEVICE_CB_t *						stderr, 
	UINT16					nargs,
	INT8					*argv[]
)
{
	ZFS_STATUS_t status ;

	/** Validate the number of arguments */
	if( nargs != 3 )
	{
		printf("\nThe syntax of the command is incorrect\n");
		printf("rendir <directory Name1> <directory Name2>\n");
		return ZSHELL_FAILURE ;
	}
	status = ZFSRenameDir( argv[1], argv[2] );
	if( status == ZFSERR_FILE_DIR_DOES_NOT_EXIST )
	{
		printf("\nThe specified directory does not exist\n");
		return ZSHELL_FAILURE;
	}
	else if( status != ZFSERR_SUCCESS )
	{
		printf("\nUnable to rename the directory or accessing an invalid volume\n" );
		return ZSHELL_FAILURE ;
	}
	printf("\nDirectory %s is renamed as %s\n", argv[1], argv[2]);
	return ZSHELL_SUCCESS;
}

/** This function separates the directory Name and the
    file Name and gets the size of the file */
INT16 Shell_ZFSGetList(	INT8	 *fileName, 
							INT8     *argv[], 
							UINT32   *size,
							RZK_DEVICE_CB_t *		  stdout
						 )
{

	INT16			filecnt;
	INT16 			fd_cnt = 0;
	INT16 			fd_total_cnt = 0;
	INT8			*pathName ;

	ZFS_FD_LIST_t 	* pfs_fd_list = ( ZFS_FD_LIST_t * ) NULL;
	ZFS_FD_LIST_t 	* ptmp_fs_fd_list = ( ZFS_FD_LIST_t * )NULL;

	UINT8 			found = ZSHELL_FALSE ;
	INT16 ret_val = ZSHELL_SUCCESS;

	pathName = ( INT8 * )malloc( MAX_FILE_PATH_SIZE ) ;
	pfs_fd_list = ( ZFS_FD_LIST_t * )malloc( sizeof( ZFS_FD_LIST_t ) * 8 ) ;
	if( pathName == NULL || pfs_fd_list == NULL )
	{
		ret_val = ZSHELL_FAILURE ;
		goto end_Shell_ZFSGetList ;
	}

	memset( pathName, 0x00, MAX_FILE_PATH_SIZE ) ;
	memset( pfs_fd_list, 0x00, sizeof( ZFS_FD_LIST_t ) * 8 ) ;

	/** Get the directory name and the File name separately */
	Shell_ZFSGetDirFileName( pathName, fileName, (INT8 **)argv );

	do{
		/** List the number of files/dirs in the folder */
		fd_cnt = ZFSList( pathName, pfs_fd_list, fd_total_cnt ) ;
		if( fd_cnt < 0 )
		{
			ret_val = ZSHELL_FAILURE ;
			goto end_Shell_ZFSGetList ;
		}
		fd_total_cnt += fd_cnt ;

		/**  Get the size of the file */
		ptmp_fs_fd_list = pfs_fd_list ;
		for( filecnt = 0 ; filecnt < fd_cnt ; filecnt++ )
		{
			if( ( (~(ptmp_fs_fd_list->fd_type)) & ZFS_FILE_TYPE ) == ZFS_FILE_TYPE )
			{
				if( !strcmp((const INT8 *)fileName,(const INT8 *) ptmp_fs_fd_list->fd_name ) )
				{
					*size = ptmp_fs_fd_list->fd_size ;
					found = ZSHELL_TRUE;
					break;
				}
			}
			ptmp_fs_fd_list++;
		}
		if( found == ZSHELL_TRUE )
			break ;
	}while( fd_cnt != 0 ) ;

	if( found == ZSHELL_FALSE )
	{
		ret_val = ZSHELL_FAILURE ;
		goto end_Shell_ZFSGetList ;
	}

end_Shell_ZFSGetList :
	if( pathName )
		free( pathName ) ;
	if( pfs_fd_list )
		free( pfs_fd_list ) ;
	return ret_val ;
}

/** This function moves/copies the file from one 
    location to another */
INT16 Shell_ZFSCopyMoveFile (
	struct shvars *			Shl,
	RZK_DEVICE_CB_t *						stdin,
	RZK_DEVICE_CB_t *						stdout, 
	RZK_DEVICE_CB_t *						stderr, 
	UINT16					nargs,
	INT8					      *argv[]
)
{
	/** Get the directory name and the file name separately */
	/** Get the size of all the files using ZFSList API present in the directory */
	/** Compare the file sent by the application with the one got from the ZFSList API */
	/** Open the file */
	/** Read the contents into one buffer */
	/** Close the file */
	/** Create a file in the destination directory specified by the application */
	/** Write the contents in to the destination file from the buffer */
	/** Close the file */

	INT8			* fileName ;
	INT8			*path ;
	INT16			len;
	UINT32			numBytes;
	UINT32			size;
	INT8			*destnDir;
	ZFS_STATUS_t	status;
	ZFS_HANDLE_t	fileHandle1, fileHandle2;
	UINT8			slashExist = 0;

	UINT32 readCnt = 0;
	UINT8  *fs_read_buffer;
	INT16 ret_val = ZSHELL_SUCCESS;

	/** Chekck the number of arguments, return error if not valid number  */
	if( nargs != 3 )
	{
		printf("\nThe syntax of the command is incorrect\n");
		printf("%s <src file Name> <destnDirName>\n",argv[0]);
		return ZSHELL_FAILURE ;		
	}

	fileName = ( INT8 * ) malloc( MAX_FILE_PATH_SIZE ) ;
	destnDir = ( INT8 * ) malloc( MAX_FILE_PATH_SIZE ) ;
	fs_read_buffer = ( UINT8 * ) malloc( MAX_FILE_PATH_SIZE ) ;

	if( fileName == NULL || destnDir == NULL || fs_read_buffer == NULL )
	{
		ret_val = ZSHELL_FAILURE ;
		goto end_Shell_ZFSCopyMoveFile ;
	}

	memset( fileName, 0x00, MAX_FILE_PATH_SIZE ) ;
	memset( destnDir, 0x00, MAX_FILE_PATH_SIZE ) ;
	memset( fs_read_buffer, 0x00, MAX_FILE_PATH_SIZE ) ;

	if( Shell_ZFSGetList( fileName, argv, &size, stdout ) )
	{
		printf("\nThe specified file does not exist or accessing an invalid volume\n");
		ret_val = ZSHELL_FAILURE ;
		goto end_Shell_ZFSCopyMoveFile ;
	}

	strcpy((INT8 *)destnDir, (const INT8 *)argv[2]);
	path = destnDir ;
	len = strlen( (const INT8 *)destnDir );
	while( len >= 0 )
	{
		if( *path == '/' && len == 1 )
		{
			slashExist = 1;
		}
		len--;
		path++;
	}

	if( !slashExist )
	{
		strcat((INT8 *)destnDir, "/" );
	}

	/** Open the first file */
	fileHandle1 = ZFSOpen(argv[1], ZFS_READ, ZFS_MODE_BINARY );
	
	/** Open the second file */
	fileHandle2 = ZFSOpen( (INT8 *)strcat((INT8 *)destnDir, (const INT8 *)fileName), ZFS_WRITE, ZFS_MODE_BINARY );
	
	if( fileHandle1 && fileHandle2 )
	{
		while( readCnt < size )
		{
			/**  Read from the first file */
			if( ZFSSeek(fileHandle1, readCnt, ZFS_FILE_BEGIN ) )
			{
				ret_val = ZSHELL_FAILURE ;
				goto end_Shell_ZFSCopyMoveFile ;
			}
			numBytes = ZFSRead(fileHandle1, fs_read_buffer, sizeof( fs_read_buffer ) );

			if( numBytes )
			{
				/** Write to the second file */
				if( ZFSSeek(fileHandle2, readCnt, ZFS_FILE_BEGIN ) )
				{
					ret_val = ZSHELL_FAILURE ;
					goto end_Shell_ZFSCopyMoveFile ;
				}
				
				numBytes = ZFSWrite(fileHandle2, fs_read_buffer, numBytes );
			}

			/** Increment the read count */
			readCnt += numBytes ;			
		}

		if(  ( ZFSClose(fileHandle1) ) || ( ZFSClose(fileHandle2) ) )
		{
			ret_val = ZSHELL_FAILURE ;
			goto end_Shell_ZFSCopyMoveFile ;
		}

		if( strcmp((const INT8 *)argv[0], "move" ) == 0 )
		{
			/** delete the original */
			status = ZFSDelete(argv[1]);			
			if( status )
			{
				ret_val = ZSHELL_FAILURE ;
				goto end_Shell_ZFSCopyMoveFile ;
			}
		}
		else
			printf("\nFile is copied\n");
		ret_val = ZSHELL_SUCCESS ;	
		goto end_Shell_ZFSCopyMoveFile ;
	}
	else
	{
		if( fileHandle1 )
			ZFSClose( fileHandle1);
		if( fileHandle2 )
			ZFSClose( fileHandle2);
		printf("\nFile is not copied, check whether directory exists or not or accessing an invalid volume\n");
		ret_val = ZSHELL_FAILURE ;
		goto end_Shell_ZFSCopyMoveFile ;
	}

end_Shell_ZFSCopyMoveFile :
	if( fileName )
		free( fileName ) ;
	if( destnDir )
		free( destnDir ) ;
	if( fs_read_buffer )
		free( fs_read_buffer ) ;
	return ret_val ;
}

/* The following function is used to get the 
   current working directory
*/
INT16 Shell_ZFSGetCWD (
	struct shvars *			Shl,
	RZK_DEVICE_CB_t *						stdin,
	RZK_DEVICE_CB_t *						stdout, 
	RZK_DEVICE_CB_t *						stderr, 
	UINT16					nargs,
	INT8					*argv[]
)
{
	INT8 *cwd ;

	if( nargs > 1 )
	{
		printf("\nThe syntax of the command is incorrect\n");
		printf("cwd\n");
		return ZSHELL_FAILURE ;
	}

	cwd = ( INT8 * ) malloc( MAX_FILE_PATH_SIZE ) ;
	if( cwd != NULL )
	{
		memset( cwd, 0x00, MAX_FILE_PATH_SIZE ) ;
		if( !( ZFSGetCwd( cwd) ) )
		{
			printf("\nCWD is %s\n", cwd);
		}
		else
		{
			printf("\nUnable to get the CWD or accessing an invalid volume\n" ) ;
		}
		free( cwd ) ;
	}
	return ZSHELL_SUCCESS;
}

/* The following function is used to list the directories 
   and files present in a folder */
INT16 Shell_ZFSListFileDirs (
	struct shvars *			Shl,
	RZK_DEVICE_CB_t *						stdin,
	RZK_DEVICE_CB_t *						stdout, 
	RZK_DEVICE_CB_t *						stderr, 
	UINT16					nargs,
	INT8					*argv[]
)
{
	INT16 fd_cnt = 0;
	INT16 filecnt;
	INT8 *cwd ;
	UINT32 size;
	UINT16 numOfFiles = 0, numOfDirs=0;
	
	ZFS_FD_LIST_t    *pfs_fd_list, *ptmp_fs_fd_list ; 
	INT16 fd_total_cnt = 0;
	INT8 initialDisplay = 1;
	INT16 ret_val = ZSHELL_FAILURE ;
		
	if( nargs > 2 )
	{
		printf("\nThe syntax of the command is incorrect\n");
		printf("dir\n");
		return ZSHELL_FAILURE ;
	}

	cwd = ( INT8 * ) malloc( MAX_FILE_PATH_SIZE ) ;
	pfs_fd_list = ( ZFS_FD_LIST_t * ) malloc( sizeof( ZFS_FD_LIST_t) * 8 ) ;

	if( cwd == NULL || pfs_fd_list == NULL )
	{
		goto end_Shell_ZFSListFileDirs ;
	}

	memset( cwd, 0x00, MAX_FILE_PATH_SIZE ) ;

	if( nargs == 1 )
	{
		strcpy( (INT8 *)cwd, "./");
	}
	else
		strcpy( (INT8 *)cwd, (const INT8 *)argv[1] );
		
	do 
	{
		/** Get all files in the directory */
		memset( pfs_fd_list, 0x00, 8 * sizeof( ZFS_FD_LIST_t) ) ;
		fd_cnt = ZFSList( cwd, pfs_fd_list, fd_total_cnt ) ;
		if( fd_cnt == 0 )
		{
			if( initialDisplay )
			{
				printf("\nNo files exist\n");
				ret_val = ZSHELL_SUCCESS ;
				goto end_Shell_ZFSListFileDirs ;
			}

		}
		else if (fd_cnt < 0 )
		{
			if( initialDisplay == 1 )
				printf("\nError in getting the directory contents\n");
			else
				ret_val = ZSHELL_SUCCESS ;
			goto end_Shell_ZFSListFileDirs ;
		}			
		if( initialDisplay )
		{
			printf("\n\n\n*******************************************************************************");
			printf("\nDATE\t\tTIME\t\tTYPE\tSIZE(bytes)\tNAME\n");
			printf("*******************************************************************************\n");
			initialDisplay = 0;
		}
		fd_total_cnt += fd_cnt ;
		ptmp_fs_fd_list = pfs_fd_list ;
		
		for( filecnt = 0 ; filecnt < fd_cnt ; filecnt++ )
		{
			/** Date Information */
			printf("\n%02d/%02d/%04d \t", ptmp_fs_fd_list->fd_mon, ptmp_fs_fd_list->fd_day,(((UINT)ptmp_fs_fd_list->fd_century * 100 ) + ptmp_fs_fd_list->fd_year )); 

			/** Time information */
			printf("%02d:%02d:%02d \t", ptmp_fs_fd_list->fd_hrs, ptmp_fs_fd_list->fd_min, ptmp_fs_fd_list->fd_sec);
			
			/**  Get the size of the file */
			if( ( (~(ptmp_fs_fd_list->fd_type)) & ZFS_FILE_TYPE ) == ZFS_FILE_TYPE )
			{
				size = (UINT32 )ptmp_fs_fd_list->fd_size;
#ifdef _IAR_CODE
				printf("\t%ld\t\t", size );
#else				
				printf("\t%u\t\t", size );
#endif				
				numOfFiles++;
			}
			else
			{
				printf("<DIR>\t\t\t");
				numOfDirs++;
			}

			printf("%s",ptmp_fs_fd_list->fd_name);
			ptmp_fs_fd_list++;
			
			
		}
//		if( fd_cnt < 8 )
//			break ;
	}while(fd_cnt > 0 );
	
	printf("\n\n\n\n\t\tNumber of File<s> %d",numOfFiles);
	printf("\n\t\tNumber of Dir<s> %d ",numOfDirs);
	printf("\n*******************************************************************************\n");
end_Shell_ZFSListFileDirs :
	if( cwd )
		free( cwd ) ;
	if( pfs_fd_list )
		free( pfs_fd_list ) ;
	return ret_val ;
}

/* The following function is used to display the contents
   of the specified file */
INT16 Shell_ZFSDispFile (
	struct shvars *			Shl,
	RZK_DEVICE_CB_t *						stdin,
	RZK_DEVICE_CB_t *						stdout, 
	RZK_DEVICE_CB_t *						stderr, 
	UINT16					nargs,
	INT8					*argv[]
)
{
	/** Get the directory name and the file name separately */
	/** Get the size of all the files using ZFSList API present in the directory */
	/** Compare the file sent by the application with the one got from the ZFSList API */
	/** Open the file */
	/** Read the contents into one buffer */
	/** Close the file */
	/** Create a file in the destonation directory specified by the application */
	/** Write the contents in to the destination file from the buffer */
	/** Close the file */

	INT8 *fileName ;
	UINT32 numBytes;
	UINT32 size;
	ZFS_HANDLE_t fileHandle1 ;

	UINT32 readCnt = 0;
	UINT8  *fs_read_buffer ;
	INT16 ret_val = ZSHELL_FAILURE ;

	if( nargs != 2 )
	{
		printf("\nThe syntax of the command is incorrect\n");
		printf("type <file Name>\n");
		return ZSHELL_FAILURE ;
	}

	fileName = ( INT8 * )malloc( MAX_FILE_PATH_SIZE );
	fs_read_buffer = ( UINT8 * )malloc( 2 * MAX_FILE_PATH_SIZE ) ;

	if( fileName == NULL || fs_read_buffer == NULL )
	{
		goto end_Shell_ZFSDispFile ;
	}
	if( Shell_ZFSGetList( fileName, argv, &size,stdout ) )
	{
		printf("\nThe specified file does not exist\n");
		goto end_Shell_ZFSDispFile ;
	}

	/** Open the file */
	fileHandle1 = ZFSOpen(argv[1], ZFS_READ, ZFS_MODE_BINARY );
	if( fileHandle1 )
	{
		while( readCnt < size )
		{
//			if ( ZFSSeek(fileHandle1, readCnt, ZFS_FILE_BEGIN ) )
//			{
//				return ZSHELL_FAILURE;
//			}
			numBytes = ZFSRead(fileHandle1, fs_read_buffer, 2 * MAX_FILE_PATH_SIZE );
			if( numBytes > 0 )
			{
				RZKDevWrite( stdout, ( INT8 * ) fs_read_buffer, numBytes ) ; 

//TBD				retVal =  write(stdout, (char*)&fs_read_buffer[0],numBytes ) ;
//				if( retVal < 0  )
//				{
//					printf( "Type failed\n");
//					return ZSHELL_FAILURE ;
//				}
			}
			readCnt += numBytes ;
		}
		ZFSClose(fileHandle1);
		printf("\n");
	}
	ret_val = ZSHELL_SUCCESS;

end_Shell_ZFSDispFile :
	if( fileName )
		free( fileName ) ;
	if( fs_read_buffer )
		free( fs_read_buffer ) ;

	return ret_val ;

}

INT16 Shell_ZFSGetVolumeInfo( struct shvars *			Shl,
								RZK_DEVICE_CB_t *						stdin,
								RZK_DEVICE_CB_t *						stdout, 
								RZK_DEVICE_CB_t *						stderr, 
								UINT16					nargs,
								INT8					*argv[]
							     )

{
	ZFS_STATUS_t vol_cnt ;
	ZFS_STATUS_t status ;
	ZFS_VOL_PARAMS_t *pvol_params, *ptempVal ;//vol_params[ 4 ],

	INT16 ret_val = ZSHELL_FAILURE ;

	vol_cnt = ZFSGetVolumeCount() ;
	if( vol_cnt <= 0 )
	{
	  	printf("\nError\n" ) ;
	  	return ZSHELL_FAILURE;
	}

	pvol_params = ( ZFS_VOL_PARAMS_t * )malloc( sizeof( ZFS_VOL_PARAMS_t ) * vol_cnt ) ;
	if( pvol_params == NULL ) 
	{
		goto end_Shell_ZFSGetVolumeInfo ;
	}

	status = ZFSGetVolumeParams( (INT8*) "EXTF:/", pvol_params, ZFS_TRUE ) ;
	if( status != ZFSERR_SUCCESS )
	{
	 	printf("\nError in retrieving volume information\n" ) ;
		goto end_Shell_ZFSGetVolumeInfo ;
	}
	ptempVal = pvol_params ;
	printf("\n*******************************************************************************" ) ;
	printf("\nVolume Name       Is Valid   Total Space   Free space   Used Space   Dirty Space");
	printf("\n                              (bytes)       (bytes)      (bytes)      (bytes)");
	printf("\n*******************************************************************************" ) ;
	while( vol_cnt > 0 )
	{
		printf("\n%-15s", ptempVal->vol_name ) ;
		printf("%-15s", (ptempVal->is_valid?"VALID":"INVALID") ) ;
#ifdef _IAR_CODE		
		printf("\t%ld", (UINT32)ptempVal->vol_size ) ;
		printf("\t\t%ld", (UINT32)ptempVal->free_space ) ;
		printf("\t%ld", (UINT32)ptempVal->used_space ) ;
		printf("\t\t%ld", (UINT32)ptempVal->dirty_space ) ;
#else
		printf("\t%u", (UINT32)ptempVal->vol_size ) ;
		printf("\t\t%u", (UINT32)ptempVal->free_space ) ;
		printf("\t%u", (UINT32)ptempVal->used_space ) ;
		printf("\t\t%u", (UINT32)ptempVal->dirty_space ) ;
#endif

/*
		printf("%-15s %-10s\t%u\t\t%u\t\t%u\t\t%u", ptempVal->vol_name,
														(ptempVal->is_valid?"VALID":"INVALID"),
														ptempVal->vol_size,
														ptempVal->free_space,
														ptempVal->used_space,
														ptempVal->dirty_space) ;
*/	
		ptempVal++ ;
		vol_cnt-- ;
	}
	printf("\n\n\n*******************************************************************************\n" ) ;
	ret_val = ZSHELL_SUCCESS ;

end_Shell_ZFSGetVolumeInfo :
	if( pvol_params )
		free( pvol_params ) ;
	return ret_val ;
}

INT16	Shell_ZFSHelp
(
  	struct shvars *Shl,
	RZK_DEVICE_CB_t *								stdin,
	RZK_DEVICE_CB_t *								stdout, 
	RZK_DEVICE_CB_t *								stderr, 
	UINT16							nargs,
   	INT8	*                    				args[]
)   
{
	UINT16	i;
	struct cmdent	*com;

	/** Meenakshi added newly */
	if( nargs == 1 )
	{
		printf( "Commands are:\n");
		for(i=0,com=ShellCommands; com; com=com->cnext) {
			if(i==0)
				printf( "  ");
			printf( "%-16s", com->cmdnam);
			if(++i == COLUMNS) {
				printf( "\n");
				i=0;
			}
		}
		if(i!=0)
			printf( "\n");
	}
	else /** MEENAKSHI ADDED NEWLY TO add description for each command */
	{
		for( com = ShellCommands ; com ; com = com->cnext )
		{
			if( strcmp( com->cmdnam, (const INT8 *)args[1] ) == 0 )
				break;
		}
		if( !com )
		{
			printf("\nThis command is not supported by the help utility.\n");
		}
		else
		{
			printf( "%s", com->helpDesc );	
		}
	}
	return OK;
}


/** @memo Gets the date
 * @doc {\bf date}
 *
 * This command gets the current date and time 
 * .
 *
 * This function takes no arguments.
 * @name date
 * @args
 * @type
 */
/*------------------------------------------------------------------------
 *  x_getdate  -  (command date) gets the date and time
 *------------------------------------------------------------------------
 */
INT16
Shell_getdatetime
(
	struct shvars *				Shl,
	RZK_DEVICE_CB_t *								stdin,
	RZK_DEVICE_CB_t *								stdout, 
	RZK_DEVICE_CB_t *								stderr, 
	UINT16								nargs,
	INT8 *							args[]
)
{
	TIME Time;
	TIME *pTime = &Time;
	RZK_DEVICE_CB_t *pdev ;
	
	pdev = RZKDevOpen("RTC",0) ;
	if( pdev == NULL )
	{
		printf("\nRTC driver is not present");
		return (-1);
	}
	RZKDevRead(pdev,(INT8 *)pTime,8);

	printf("%-4s, %2d %-4s %4d %3d:%2d:%2d\n",
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
	return(OK);
}

/** @memo Gets the date
 * @doc {\bf date}
 *
 * This command gets the current date and time 
 * .
 *
 * This function takes no arguments.
 * @name date
 * @args
 * @type
 */
/*------------------------------------------------------------------------
 *  x_setdate  -  (command date) sets the date and time
 *------------------------------------------------------------------------
 */
INT16
Shell_setdatetime
(
	struct shvars *				Shl,
	RZK_DEVICE_CB_t *								stdin,
	RZK_DEVICE_CB_t *								stdout, 
	RZK_DEVICE_CB_t *								stderr, 
	UINT16								nargs,
	INT8 *							args[]
)
{
	UINT uint1,uint11,uint2,uint3,uint4,uint5,uint6,uint7;
	TIME Time;
//	UINT32 seconds;
	UINT8 cFlag=0; 
	RZK_DEVICE_CB_t *pdev ;

	if( nargs < 8 )
	{
		printf("\nThe syntax of the command is incorrect\n");
		printf("settime <year> <month> <dayofmonth> <dayofweek> <hrs> <mins> <secs>\n");
		return ZSHELL_FAILURE ;
	}

	sscanf((const INT8 *)args[1],"%6d",&uint1);

	uint11 = uint1%100;
	uint1 = uint1/100;
	
	sscanf((const INT8 *)args[2],"%6d",&uint2);
	if((uint2 > 12) || (uint2 <= 0))
	{
		printf("\nPlease enter month less than or equal to 12\n");
		cFlag = 1;
	}

	sscanf((const INT8 *)args[3],"%6d",&uint3);
	if((uint3 > 31) || (uint3 <= 0))
	{
		printf("\nPlease enter day of month less than or equal to 31\n");
		cFlag = 1;
	}

	sscanf((const INT8 *)args[4],"%6d",&uint4);
	if((uint4 > 7) || (uint4 <= 0))
	{
		printf("\nPlease enter day of week less than or equal to 7, 1-Mon 7-Sun\n");
		cFlag = 1;
	}

	sscanf((const INT8 *)args[5],"%6d",&uint5);
	if(uint5 >= 24)
	{
		printf("\nPlease enter hrs less than or equal to 24\n");
		cFlag = 1;
	}

	sscanf((const INT8 *)args[6],"%6d",&uint6);
	if(uint6 >= 60)
	{
		printf("\nPlease enter minutes less than or equal to 60\n");
		cFlag = 1;
	}

	sscanf((const INT8 *)args[7],"%6d",&uint7);
	if(uint7 >= 60)
	{
		printf("\nPlease enter seconds less than or equal to 60\n");
		cFlag = 1;
	}

	if(cFlag == 1)
		return(-1);
	Time.cent = (UINT8)uint1;
	Time.year = (UINT8)uint11;
	Time.mon = (UINT8)uint2;
	Time.dayOfMonth = (UINT8)uint3;
	Time.dayOfWeek = (UINT8)uint4;
	Time.hrs = (UINT8)uint5;
	Time.minutes = (UINT8)uint6;
	Time.sec = (UINT8)uint7;

	pdev = RZKDevOpen("RTC",0) ;
	if( pdev == NULL )
	{
		printf("\nRTC driver is not present");
		return (-1);
	}

	RZKDevIOCTL( pdev, RTC_RESET_CONTROL, ( void * ) &Time, ( void * ) &Time ) ;
	RZKDevIOCTL( pdev, RTC_SET_ALL, ( void * ) &Time, ( void * ) &Time ) ;
	return(OK);
}


/* The following function is used to delete a 
   file given by the user */
INT16 Shell_ZFSCreateFile (
	struct shvars *			Shl,
	RZK_DEVICE_CB_t *						stdin,
	RZK_DEVICE_CB_t *						stdout, 
	RZK_DEVICE_CB_t *						stderr, 
	UINT16					nargs,
	INT8					*argv[]
)
{
	INT8			*fileName;
	INT			len ;
	INT32			numBytes;
	UINT32			size;
	ZFS_HANDLE_t	fileHandle1;
	INT8 			*buf;
	INT16 ret_val = ZSHELL_FAILURE ;


	/** Chekck the number of arguments, return error if not valid number  */
	if( nargs != 2 )
	{
		printf("\nThe syntax of the command is incorrect\n");
		printf("create <file name>") ;
		return ZSHELL_FAILURE ;		
	}

	fileName = (INT8 *) malloc( MAX_FILE_PATH_SIZE ) ;
	buf = (INT8 *) malloc( MAX_FILE_PATH_SIZE ) ;

	if( fileName == NULL || buf == NULL )
		goto end_Shell_ZFSCreateFile ;
		

	if( !Shell_ZFSGetList( fileName, argv, &size, stdout ) )
	{
		printf("\nFILE IS ALREADY PRESENT, OVERWRITING THE FILE\n" ) ;
	}

	// open the file in write mode.
	fileHandle1 = ZFSOpen(argv[1], ZFS_WRITE, ZFS_MODE_BINARY );
	if( fileHandle1 )
	{
		// get the data
		printf( "\nPlease Type the text upto 256 characters to be stored in the file.") ;
	 	printf( "\nPress ENTER to save the typed text into the file\n" ) ;

		GetString( stdout, (INT8 *)&buf[0], 256, &len ) ;

		if( len != 0 )
		{
			numBytes = ZFSWrite(fileHandle1, ( UINT8* ) &buf[0], len ) ;
			if( numBytes < 0 )
				printf( "\nError in writing..." ) ;
		}

	   	ZFSClose( fileHandle1 ) ;
		printf("\nFile is created\n");
		ret_val = ZSHELL_SUCCESS ;	
	}
	else
	{
		printf( "Entered file name is not correct or accessing an invalid volume\n" ) ;
	}

end_Shell_ZFSCreateFile :
	if( fileName)
		free( fileName ) ;
	if( buf)
		free( buf ) ;

        return (OK);
}



/* The following function is used to delete a 
   file given by the user */
INT16 Shell_ZFSFormat (
	struct shvars *			Shl,
	RZK_DEVICE_CB_t *						stdin,
	RZK_DEVICE_CB_t *						stdout, 
	RZK_DEVICE_CB_t *						stderr, 
	UINT16					nargs,
	INT8					*argv[]
)
{
	ZFS_STATUS_t	status;


	/** Chekck the number of arguments, return error if not valid number  */
	if( nargs != 2 )
	{
		printf("\nThe syntax of the command is incorrect\n");
		printf("format <vol name>") ;
		return ZSHELL_FAILURE ;		
	}

	printf("\nFormatting the volume %s, Please wait...", argv[1]);
	// call the format.
	status = ZFSFormat( argv[1] ) ;
	if( status == ZFSERR_SUCCESS )
	{
		printf("SUCCESSFULL\n");
		return ZSHELL_FAILURE ;
	}
	else
	{
		printf("FAILED\nCouldn't format the volume as it is in use/not present in the system\n");
		return ZSHELL_SUCCESS ;	
	}
}

