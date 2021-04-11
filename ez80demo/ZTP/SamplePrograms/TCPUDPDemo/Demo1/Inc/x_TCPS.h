/*Standard includes*/
#include "sockaddr.h"
#include "socket.h"

/* User includes */
#include "ZTypes.h"
#include "ZSysgen.h"
#include "ZThread.h"

/* Standard definitions */
#ifndef ROUNDROBIN_AP
#define ROUNDROBIN_AP       0 // check whether this is OK
#endif
#ifndef STACK_SIZE_AP
#define	STACK_SIZE_AP		4096 // check whether this is OK
#endif

/* external variables */
extern INT8	initstackAp[STACK_SIZE_AP]; // check whether this is OK

/* User definitions */
#define MAX_BUF_SIZE 4096
#define ARGS	4
#define PORT	5000
#define TRUE	1

/* Global variables */

/* Function prototypes */
void TCPS_Duplex(INT16 sockfd, INT8* mode);