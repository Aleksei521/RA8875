/*
 * File       :     ZTPUSERDETAILS.h
 *
 * Description:	This file contains the user login details.
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
 */


#ifndef	_ZTPUSERDETAILS_H
#define	_ZTPUSERDETAILS_H

/** Meenakshi added on 4th Feb, 2005, CR 5131, CR 5082 */
#include "ZTPGlob.h"

 
#define ZTP_USR_NAME_LEN 24
#define ZTP_USR_PSWD_LEN 12

typedef struct Userdetails{
INT8 usrName[ZTP_USR_NAME_LEN]	;
INT8 password [ZTP_USR_PSWD_LEN];
}USERDETAILS;

#define ZTP_DUPLICATE_USER  (-3)
#define ZTP_FILE_DOES_NOT_EXIST  (-4)

//#define FILE_SZ 	( FTP_MAX_NO_USERS * sizeof( USERDETAILS ) ) ;


extern INT8 addUserInfo(INT8 *,INT8 *);
extern  INT8 searchUser(INT8 *,INT8 *);
extern  INT8 deleteUserInfo(INT8 *);

#endif	/* _ZTPUSERDETAILS_H */
