
#include "Ztypes.h"
#include "socket.h"


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
 * Function Name			:	udp_open()
 * 
 * Return Values			:	None
 *
 * Formal Parameters		:	pointer to udp_socket structure
 *							local port
 *							destination IP Address
 *							destination port
 *							data handler
 *				
 * Global variables used    	:   	None
 *								
 * Description				:	This function opens a UDP connection
 ****************************************************************************/
extern INT udp_open( 	udp_Socket *s, 
						UINT16 lport, 
						UINT32 ina, 
						UINT16 port, 
						dataHandler_t datahandler
					    );

/*****************************************************************************
 * Function Name			:	udp_write()
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
extern INT udp_write( 	udp_Socket* udpSocket, 
						UINT8 *dp, 
						INT len
				       ) ;


/*****************************************************************************
 * Function Name			:	udp_read()
 * 
 * Return Values			:	Number of bytes received if > 0
 *							Error else
 *
 * Formal Parameters		:	pointer to udp_socket structure
 *							pointer to the data to be received
 *							Length of the data to be received
 *				
 * Global variables used    	:   	None
 *								
 * Description				:	This function receives data from the
 *							destination
 ****************************************************************************/
extern INT udp_read( 	udp_Socket* udpSocket, 
						UINT8 *dp, 
						INT len,						
						struct sockaddr_in *pStFrom
				     ) ;
/*****************************************************************************
 * Function Name			:	udp_close()
 * 
 * Return Values			:	0 -- Success
 *
 * Formal Parameters		:	pointer to udp_socket structure
 *				
 * Global variables used    	:   	None
 *								
 * Description				:	This function closes the UDP Connection
 ****************************************************************************/
extern void  udp_close( udp_Socket* udpSocket );

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
extern void block_udp(	udp_Socket* udpSocket ,
						UINT32 userTimer
					   ) ;

