/***********************************************************************************
 * File       : cfileapi.h
 * Description: This file contains the "C" file API prototypes to use
 * Author     : 
 * Created on : 25-APR-2004
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
 *********************************************************************************/

#ifndef _CFILEAPI_H_
#define _CFILEAPI_H_


#include <stdio.h>
#include "ztypes.h"
// specific macros that are used in 'C' file APIs for various operations
/** Added for IAR */
#ifdef _IAR_CODE
#define SEEK_CUR    0
#define SEEK_END    1
#define SEEK_SET    2
#endif

// typedefinitions that are used in 'C' file APIs
typedef void FILE ;

#ifndef _IAR_CODE

#ifndef SIZE_T_DEFINED
#define SIZE_T_DEFINED
typedef int size_t ;
#endif // SIZE_T_DEFINED

#endif // _IAR_CODE

//API's

FILE *fopen( const INT8 *filename, const INT8 *mode );
INT fclose( FILE *stream );
INT fputc( INT c, FILE *stream );
INT fputs( const INT8 *string, FILE *stream );
INT fgetc( FILE *stream );
INT8 *fgets( INT8 *string, INT n, FILE *stream );
size_t fread( void *buffer, size_t size, size_t count, FILE *stream );
size_t fwrite( const void *buffer, size_t size, size_t count, FILE *stream );
INT fseek( FILE *stream, INT32 offset, INT origin );
INT32 ftell( FILE *stream );
INT feof( FILE *stream );



#endif //_CFILEAPI_H_


