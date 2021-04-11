#include "ZTypes.h"


/** FTP APIs **/
extern INT ftp_connect(INT8 * server_name, INT server_port, RZK_DEVICE_CB_t *stdout);
extern INT16 do_a_ftp_command( 	RZK_DEVICE_CB_t * 	device, 
									UINT16	nargs,
									INT8		*args[]
							    );
extern INT do_programatic_login(
									RZK_DEVICE_CB_t * 		stdin, 
									RZK_DEVICE_CB_t * 		stdout, 
									INT8 	*username, 
									INT8 	*passwd
							   ) ;
