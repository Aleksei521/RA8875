/*
 * File       	:     tcp_api.h
 *
 * Description	:		This file contains the wrapper functions for 
 *					TCP.Instead of calling the socket APIs the user
 *					can call these TCP APIs directly.
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

#include "sock_tcp.h"

/*****************************************************************************
 * Function Name			:	tcp_socket()
 * 
 * Return Values			:	pointer to tcp_Socket structure
 *
 * Formal Parameters		:	None
 *				
 * Global variables used    	:   	None
 *								
 * Description				:	This function returns a TCP CB to the
 *							application
 ****************************************************************************/
extern 	tcp_Socket* 	tcp_socket();


/*****************************************************************************
 * Function Name			:	tcp_open()
 * 
 * Return Values			:	1	-- 	Success
 *							0	--	Error
 *
 * Formal Parameters		:	pointer to tcp_Socket structure
 *							Local port
 *							Destination IP Address
 *							Destination Port
 *							Data Handler if any
 *				
 * Global variables used    	:   	None
 *								
 * Description				:	This function Opens a TCP Connection
 ****************************************************************************/
extern INT tcp_open( 	tcp_Socket 		*s, 
						UINT16 			lport, 
						UINT32 			ina, 
						UINT16 			port, 
						dataHandler_t 	datahandler
					);

/*****************************************************************************
 * Function Name			:	tcp_listen()
 * 
 * Return Values			:	1	-- 	Success
 *							0	--	Error
 *
 * Formal Parameters		:	pointer to tcp_Socket structure
 *							Local port
 *							Destination IP Address
 *							Destination Port
 *							Data Handler if any
 *							Timeout -- 0 for Infinite value
 *				
 * Global variables used    	:   	None
 *								
 * Description				:	This function Opens a TCP Connection
 ****************************************************************************/
extern INT tcp_listen( 	tcp_Socket *s, 
						UINT16 lport, 
						UINT32 ina, 
						UINT16 port, 
						dataHandler_t datahandler, 
						UINT16 timeout
					   ) ;

/*****************************************************************************
 * Function Name			:	tcp_read()
 * 
 * Return Values			:	Number of bytes read if > 0
 *							Error else
 *
 * Formal Parameters		:	pointer to tcp_socket structure
 *							pointer to the data to be received
 *							Length of the data to be received
 *				
 * Global variables used    	:   	None
 *								
 * Description				:	This function receives data from the
 *							destination
 ****************************************************************************/

extern INT tcp_read( 	tcp_Socket *s, 
						UINT8 *datap, 
						INT maxlen
				     ) ;

/*****************************************************************************
 * Function Name			:	tcp_write()
 * 
 * Return Values			:	Number of bytes written if > 0
 *							Error else
 *
 * Formal Parameters		:	pointer to tcp_socket structure
 *							pointer to the data to be sent
 *							Length of the data to be sent
 *				
 * Global variables used    	:   	None
 *								
 * Description				:	This function sends data specified to the
 *							destination
 ****************************************************************************/
extern INT tcp_write( 	tcp_Socket *s, 
						UINT8 *dp, 
						INT len
					    ) ;


/*****************************************************************************
 * Function Name			:	tcp_close()
 * 
 * Return Values			:	None
 *
 * Formal Parameters		:	pointer to tcp_Socket structure
 *				
 * Global variables used    	:   	None
 *								
 * Description				:	This function closes the TCP Connection
 ****************************************************************************/
extern void  tcp_close(	 tcp_Socket* tcpSocket  );



/*****************************************************************************
 * Function Name			:	tcp_established()
 * 
 * Return Values			:	0 -- Failure
 *							1 -- Success
 *
 * Formal Parameters		:	pointer to tcp_Socket structure
 *				
 * Global variables used    	:   	None
 *								
 * Description				:	 This function lets the application know
 							 the status of the TCP socket whether it is in
 							 established state or not.
 							 After calling the tcp_listen / tcp_open user has 
 							 to poll this function to make sure that the socket 
 							 is in established state. When this function 
 							 returns 1, socket is in connected state.
 ****************************************************************************/
extern INT tcp_established( tcp_Socket *s );

/*****************************************************************************
 * Function Name			:	block_tcp()
 * 
 * Return Values			:	None
 *
 * Formal Parameters		:	pointer to tcp_Socket structure
 *				
 * Global variables used    	:   	None
 *								
 * Description				:	This function changes the operational
 *							mode of TCP to blocking.
 ****************************************************************************/
extern void block_tcp(tcp_Socket* tcpSocket );


