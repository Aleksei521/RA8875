
#ifndef _HTTPAUTH_H_
#define _HTTPAUTH_H_



#include <stdio.h>
#include "ztypes.h"

#ifndef SSL_OS_RZK
#define SSL_OS_RZK		//VG for MD5 defines of RZK
#endif

#define HASHLEN 16
typedef INT8 HASH[HASHLEN];
#define HASHHEXLEN 32
typedef INT8 HASHHEX[HASHHEXLEN+1];


/* calculate H(A1) as per HTTP Digest spec */
extern void DigestCalcHA1(
     UINT8 * pszUserName,
     UINT8 * pszRealm,
     UINT8 * pszPassword,
     UINT8 * SessionKey
    );

/* calculate request-digest/response-digest as per HTTP Digest spec */
extern void DigestCalcResponse(
     UINT8 *  HA1,           /* H(A1) */
     UINT8 * pszNonce,       /* nonce from server */
     UINT8 * pszNonceCount,  /* 8 hex digits */
     UINT8 * pszCNonce,      /* client nonce */
     UINT8 * pszQop,         /* qop-value: "", "auth", "auth-int" */
     UINT8 * pszMethod,      /* method from the request */
     UINT8 * pszDigestUri,   /* requested URL */
     UINT8 * Response      /* request-digest or response-digest */
    );	

extern void CvtHex(
     UINT8* Bin,
     UINT8* Hex,
	 INT16 len
    );


#endif
