
/*
************************************************************************
* File       :     S2TCP_cgi.c
*
* Description:  This file contents  two CGI function and one RZK thread. 
* S2TCP_cgi CGI function read data from html page while SerialRead_cgi 
* upload serial received data to the page.  RZK SerialReadThread reads
* data from serial port.
*
* ZILOG DISCLAIMER
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY 
* KIND,EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
* WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
* PURPOSE AND NONINFRINGEMENT. 
* IN NO EVENT SHALL ZILOG, INC BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
* OTHERWISE,ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

* Copyright (C) 2006-xxxx by  ZILOG, Inc.
* All Rights Reserved
************************************************************************
*/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZThread.h"
#include "ZDevice.h"
#include "xc_lib.h"
#include "http.h"
#include "httpd.h"


/** Constant definitions **/
#define TRUE	1
#define FALSE	0

/** Externs **/
extern const struct staticpage S2TCP_htm;
extern const struct staticpage serial_to_tcp_htm;
extern const struct staticpage call_cgi_htm;
//extern DID SERIAL0;
extern RZK_DEVICE_CB_t*                                  CONSOLE;

/** Global variables **/
char Buffer[400]={0};
char EmptyBuffer[400]={0};
char SerialData[10] ={0};
char serialBuffer[400]={0};
char serialBuffercopy[400]={0};
char DataReady;
char ByteCount=0;

char pagetop[] = "<BODY BGCOLOR=silver> <div align=center></CENTER><br><p><font face=Arial size=4 color=#000040>Serial Data Read Window</font></p><br><div align=center></center><table border=1 width=600><tr><td><p align=left>";

/** Local Function Prototypes **/
void replace_plus(int array_size, char *in_array);
void SerialReadThread();

/*!
 * This CGI function reads the data from TCP 
 * and writes into the Serial port
 *\param   request 
 *\return  This funcion returns OK   
 */
INT16 S2TCP_cgi(struct http_request *request)
{
	int temp;
	char  *str1;
    http_output_reply(request,HTTP_200_OK);
 	xc_sprintf( Buffer, "%s", request->params[0].value );
	temp = strlen(Buffer);
	replace_plus(temp,Buffer);
	printf("<TCP Data>%s",Buffer);
	printf("\n>");
	__http_write(request,(char *)serial_to_tcp_htm.contents,serial_to_tcp_htm.size);

  return( OK );
}

/*!
 * This CGI function uploads the data received 
 * from the serial port to the web page
 *\param   request 
 *\return  This funcion returns OK*/   
 
INT16 SerialRead_cgi(struct http_request *request)
{
    http_output_reply(request,HTTP_200_OK);
	__http_write(request,pagetop,strlen(pagetop));
	__http_write(request,serialBuffercopy,strlen(serialBuffercopy));
	__http_write(request,(char *)call_cgi_htm.contents,call_cgi_htm.size);//

	return( OK );
}

//*****************************************************************//
/*!
 * This function reads the data from the 
 * serial port 
 *\param   None 
 *\return  None   
 */
void SerialReadThread()
{
	INT16 size;
	printf( "\n<Serial Data>");
	while(1)
	{
		size =  RZKDevRead( 
					 CONSOLE,	// device id
					 SerialData,		// buffer pointer where the data need to be stored
					 1	// input-bytes to read, output-bytes read
					); 
	
		if(size < 0 )
		{
			printf( "Error on read %X\n", size);
		}
		else
		{
			extern int putch(int);
			putch(SerialData[0]);
		}

		serialBuffer[ByteCount++]=SerialData[0];		
		if((ByteCount>=sizeof(serialBuffercopy))||(SerialData[0]==0x0D))
		{
			printf("\n");
			memcpy(serialBuffercopy,serialBuffer,sizeof(serialBuffer));
			memset(serialBuffer,0,sizeof(serialBuffer));
			ByteCount=0;
			DataReady=TRUE;	
			printf( "\n<Serial Data>");
		}
	}
}

/*!
 * This function removes the + sign from the given text
 *\param   array_size 	-- Size of the array
 *\param   in_array		-- Array
 *\return  This funcion returns OK   
 */
void replace_plus(int array_size, char *in_array)
{
	int i;
	for(i=0;i<=array_size;i++)
	{
		if(in_array[i]==0x2B)
    	in_array[i]=0x20;
		if(in_array[i]==0x25)
		{
			if((in_array[i+1]>=0x30)&(in_array[i+1]<=0x39))
			{
		    	in_array[i+1]=in_array[i+1]-0x30;
		    	in_array[i+1]=(in_array[i+1] <<4);
			}
			else
			{
		    	in_array[i+1]=in_array[i+1]-0x37;
		    	in_array[i+1]=(in_array[i+1] <<4);
			}
	
			if((in_array[i+2]>=0x30)&(in_array[i+2]<=0x39))
			{
		    	in_array[i+2]=in_array[i+2]-0x30;
		    	in_array[i+2]=(in_array[i+2] & 0x0F);
			}
			else
			{
		    	in_array[i+2]=in_array[i+2]-0x37;
		    	in_array[i+2]=(in_array[i+2] & 0x0F);
			}
			in_array[i+1]=(in_array[i+1] | in_array[i+2]);
			in_array[i]=0x20;
			in_array[i+2]=0x20;
			i++;
			i++;
		}
	}
}


/*******************************************************************************
**********  End of file
********************************************************************************/

