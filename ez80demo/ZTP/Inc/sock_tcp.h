#include "ZTPTCP.h"


extern void  _tcp_close( tcp_Socket* tcpSocket );
extern void tcp_abort( tcp_Socket *s );
extern INT tcp_read( 	tcp_Socket *s, 
					UINT8 *datap, 
					INT maxlen
				   ) ;
extern INT tcp_write( 	tcp_Socket *s, 
					UINT8 *dp, 
					INT len
				    ) ;
extern INT tcp_open( 	tcp_Socket 		*s, 
				UINT16 			lport, 
				UINT32 			ina, 
				UINT16 			port, 
				dataHandler_t 	datahandler
			   );
extern VOID tcp_send( tcp_Socket *s );

