
#include "Ztypes.h"
#include "socket.h"


extern INT udp_read( udp_Socket *s, 
					UINT8 *datap, 
					INT maxlen,
					struct sockaddr_in *pStFrom
				    );

extern INT16 _udp_close( udp_Socket *ds );

/*****************************************************************************
 * Function Name			:	udp_socket()
 * 
 * Return Values			:	pointer to udp_socket structure
 *
 * Formal Parameters		:	None
 *				
 * Global variables used    	:   	None
 *								
 * Description				:	This function returns a UDP CB to the
 *							application
 ****************************************************************************/
extern udp_Socket* 	udp_socket() ;

/*****************************************************************************
 * Function Name			:	udp_send()
 * 
 * Return Values			:	None
 *
 * Formal Parameters		:	pointer to udp_socket structure
 *							pointer to the data to be sent
 *							Length of the data to be sent
 *				
 * Global variables used    	:   	None
 *								
 * Description				:	This function sends data specified to the
 *							destination
 ****************************************************************************/
extern INT udp_send( 	udp_Socket* udpSocket, 
				UINT8 *dp, 
				INT len
		     ) ;
extern INT udp_open( udp_Socket *s, 
				UINT16 lport, 
				UINT32 ina, 
				UINT16 port, 
				dataHandler_t datahandler
			    );
extern INT16 _udp_write( 	udp_Socket *s, 
					UINT8 *datap, 
					INT len, 
					UINT16 offset
				) ;

/*****************************************************************************
 * Function Name			:	block_udp()
 * 
 * Return Values			:	None
 *
 * Formal Parameters		:	pointer to udp_Socket structure
 *				
 * Global variables used    	:   	None
 *								
 * Description				:	This function changes the operational
 *							mode of UDP to blocking.
 ****************************************************************************/
extern void block_udp(udp_Socket* tcpSocket ,UINT32 userTimer) ;