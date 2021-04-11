/************************************************************************************************
** File:        website.c
** Description: Defines and initializes ZGATE configuration website.
**
** Copyright 2016 Zilog Inc. ALL RIGHTS RESERVED.
*
*************************************************************************************************
* The source code in this file was written by an authorized Zilog employee or a licensed 
* consultant. The source code has been verified to the fullest extent possible. 
*
* Permission to use this code is granted on a royalty-free basis. However, users are cautioned to
* authenticate the code contained herein. 
* 
* ZILOG DOES NOT GUARANTEE THE VERACITY OF THIS SOFTWARE; ANY SOFTWARE CONTAINED HEREIN IS
* PROVIDED "AS IS." NO WARRANTIES ARE GIVEN, WHETHER EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
* IMPLIED WARRANTIES OF FITNESS FOR PARTICULAR PURPOSE OR MERCHANTABILITY. IN NO EVENT WILL ZILOG
* BE LIABLE FOR ANY SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES OR ANY LIABILITY IN TORT,
* NEGLIGENCE, OR OTHER LIABILITY INCURRED AS A RESULT OF THE USE OF THE SOFTWARE, EVEN IF ZILOG 
* HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. ZILOG ALSO DOES NOT WARRANT THAT THE USE 
* OF THE SOFTWARE, OR OF ANY INFORMATION CONTAINED THEREIN WILL NOT INFRINGE ANY PATENT, 
* COPYRIGHT, OR TRADEMARK OF ANY THIRD PERSON OR ENTITY.

* THE SOFTWARE IS NOT FAULT-TOLERANT AND IS NOT DESIGNED, MANUFACTURED OR INTENDED FOR USE IN 
* CONJUNCTION WITH ON-LINE CONTROL EQUIPMENT, IN HAZARDOUS ENVIRONMENTS, IN APPLICATIONS 
* REQUIRING FAIL-SAFE PERFORMANCE, OR WHERE THE FAILURE OF THE SOFTWARE COULD LEAD DIRECTLY TO 
* DEATH, PERSONAL INJURY OR SEVERE PHYSICAL OR ENVIRONMENTAL DAMAGE (ALL OF THE FOREGOING, 
* "HIGH RISK ACTIVITIES"). ZILOG SPECIFICALLY DISCLAIMS ANY EXPRESS OR IMPLIED WARRANTY TO HIGH 
* RISK ACTIVITIES.
*
************************************************************************************************/

#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZThread.h"
#include "ZDevice.h"
#include "http.h"
#include "httpd.h"
#include "socket.h"
#include "ZSemaphore.h"
#include "ZGATE_Web.h"


extern  RZK_DEVICE_CB_t*   CONSOLE;
/*
 * ZGATE Web Site Description
 *
 * If a page is a static page, then the type HTTP_PAGE_STATIC should be
 * specified, and a struct staticpage used to describe the page data. If
 * a page is dynamic, then the type HTTP_PAGE_DYNAMIC should be used, and
 * the address of the CGI routine should be provided.
 */
extern const struct staticpage index_htm;
extern const struct staticpage ez80f91_htm;
extern const struct staticpage left_htm;
extern const struct staticpage tcpip_htm;
extern const struct staticpage DevKit_jpg;

extern const struct staticpage ZGATE_htm;
extern const struct staticpage ZGATE_hdr_htm;
extern const struct staticpage ZGATE_jpg;
extern const struct staticpage Firewall_htm;
extern const struct staticpage firewall_jpg;

extern const struct staticpage table_js;

extern INT16 ZGATE_Conf_cgi(struct http_request *request);
extern INT16 Boot_Cfg_cgi(struct http_request *request);

/*
 * Frames
 */
extern const struct staticpage ez80f91f_htm;
extern const struct staticpage ez80f91fdev_htm;
extern const struct staticpage ez80f91fdoc_htm;
extern const struct staticpage tcpipf_htm;
extern const struct staticpage Firewallf_htm;

extern const struct staticpage Eth_Addrf_htm;
extern const struct staticpage Eth_Typef_htm;
extern const struct staticpage IP_Addrf_htm;
extern const struct staticpage IP_Protf_htm;
extern const struct staticpage TCP_Portf_htm;
extern const struct staticpage UDP_Portf_htm;

extern const struct staticpage Threshf_htm;
extern const struct staticpage Boot_Cfgf_htm;
extern const struct staticpage Log_Cfgf_htm;


//VG 
//The below table is used to fill the "Content type:" field in the the 
//HTTP replys. If the web files are added staticaly then "Content type:" is
//filled from the "char * mimetype" member of the Webpage structure.
//But for files added to the filesystem the "Content type:" will be filled 
//based on the file extensions.  
struct mimetype mimetypes[] = {
   {".htm","text/html"},
   {".html","text/html"},
   {".jpg","image/jpg"},
   {".class","application/octect-stream"},
   {".gif","image/gif"},
// If the file extensions does not match any then unknown will be used.
// This should always be at the end.
   {"unknown","unknown"}   
   };

UINT16 NoOfMimeTypes = sizeof(mimetypes)/sizeof(struct mimetype);

RZK_SEMAPHOREHANDLE_t  ZGATE_CgiSem;



Webpage                 website[] = 
{
   {HTTP_PAGE_STATIC, "/",                "text/html", &index_htm },
   {HTTP_PAGE_STATIC, "/index.htm",       "text/html", &index_htm },
   {HTTP_PAGE_STATIC, "/left.htm",        "text/html", &left_htm },
   {HTTP_PAGE_STATIC, "/ZGATE_hdr.htm",   "text/html", &ZGATE_hdr_htm },
   {HTTP_PAGE_STATIC, "/ZGATE.htm",       "text/html", &ZGATE_htm },
   {HTTP_PAGE_STATIC, "/ZGATE.jpg",       "image/jpg", &ZGATE_jpg },
   {HTTP_PAGE_STATIC, "/Firewall.htm",    "text/html", &Firewall_htm },
   {HTTP_PAGE_STATIC, "/firewall.jpg",    "image/jpg", &firewall_jpg },


   {HTTP_PAGE_STATIC, "/ez80f91.htm",     "text/html", &ez80f91_htm },
   {HTTP_PAGE_STATIC, "/tcpip.htm",       "text/html", &tcpip_htm },
   {HTTP_PAGE_STATIC, "/devkit.jpg",      "image/jpg", &DevKit_jpg },


   {HTTP_PAGE_STATIC,  "/Eth_Addr.htm",   "text/html", &Eth_Addr_htm },
   {HTTP_PAGE_STATIC,  "/Eth_Type.htm",   "text/html", &Eth_Type_htm },
   {HTTP_PAGE_STATIC,  "/IP_Addr.htm",    "text/html", &IP_Addr_htm },
   {HTTP_PAGE_STATIC,  "/IP_Prot.htm",    "text/html", &IP_Prot_htm },
   {HTTP_PAGE_STATIC,  "/TCP_Port.htm",   "text/html", &TCP_Port_htm },
   {HTTP_PAGE_STATIC,  "/UDP_Port.htm",   "text/html", &UDP_Port_htm },
   {HTTP_PAGE_STATIC,  "/Thresh.htm",     "text/html", &Thresh_htm },

   {HTTP_PAGE_STATIC,  "/table.js",       "text/html", &table_js },

   {HTTP_PAGE_DYNAMIC, "/Eth_Addr.js",    "text/html", (struct staticpage *)ZGATE_Eth_AddrList_cgi }, 
   {HTTP_PAGE_DYNAMIC, "/Eth_Type.js",    "text/html", (struct staticpage *)ZGATE_Eth_TypeList_cgi }, 
   {HTTP_PAGE_DYNAMIC, "/IP_Addr.js",     "text/html", (struct staticpage *)ZGATE_IP_AddrList_cgi }, 
   {HTTP_PAGE_DYNAMIC, "/IP_Prot.js",     "text/html", (struct staticpage *)ZGATE_IP_ProtList_cgi }, 
   {HTTP_PAGE_DYNAMIC, "/TCP_Ports.js",   "text/html", (struct staticpage *)ZGATE_TCP_PortList_cgi }, 
   {HTTP_PAGE_DYNAMIC, "/UDP_Ports.js",   "text/html", (struct staticpage *)ZGATE_UDP_PortList_cgi }, 
   {HTTP_PAGE_DYNAMIC, "/Thresh.js",      "text/html", (struct staticpage *)ZGATE_Thresh_cgi }, 
   {HTTP_PAGE_DYNAMIC, "/Log_Cfg.js",     "text/html", (struct staticpage *)ZGATE_Log_Cfg_cgi }, 

   {HTTP_PAGE_DYNAMIC, "/ZGATE_Conf",     "text/html", (struct staticpage *)ZGATE_Conf_cgi }, 

   {HTTP_PAGE_DYNAMIC, "/Boot_Cfg_cgi",   "text/html", (struct staticpage *)Boot_Cfg_cgi }, 
   {HTTP_PAGE_STATIC,  "/Boot_Cfg.htm",   "text/html", &Boot_Cfg_htm },
   {HTTP_PAGE_STATIC,  "/Log_Cfg.htm",    "text/html", &Log_Cfg_htm },

   {HTTP_PAGE_STATIC,  "/ez80f91f.htm",   "text/html", &ez80f91f_htm },
   {HTTP_PAGE_STATIC,  "/ez80f91fdev.htm","text/html", &ez80f91fdev_htm },
   {HTTP_PAGE_STATIC,  "/ez80f91fdoc.htm","text/html", &ez80f91fdoc_htm },
   {HTTP_PAGE_STATIC,  "/tcpipf.htm",     "text/html", &tcpipf_htm },
   {HTTP_PAGE_STATIC,  "/Firewallf.htm",  "text/html", &Firewallf_htm },
   {HTTP_PAGE_STATIC,  "/Eth_Addrf.htm",  "text/html", &Eth_Addrf_htm },
   {HTTP_PAGE_STATIC,  "/Eth_Typef.htm",  "text/html", &Eth_Typef_htm },
   {HTTP_PAGE_STATIC,  "/IP_Addrf.htm",   "text/html", &IP_Addrf_htm },
   {HTTP_PAGE_STATIC,  "/IP_Protf.htm",   "text/html", &IP_Protf_htm },
   {HTTP_PAGE_STATIC,  "/TCP_Portf.htm",  "text/html", &TCP_Portf_htm },
   {HTTP_PAGE_STATIC,  "/UDP_Portf.htm",  "text/html", &UDP_Portf_htm },
   {HTTP_PAGE_STATIC,  "/Threshf.htm",    "text/html", &Threshf_htm },
   {HTTP_PAGE_STATIC,  "/Boot_Cfgf.htm",  "text/html", &Boot_Cfgf_htm },
   {HTTP_PAGE_STATIC,  "/Log_Cfgf.htm",   "text/html", &Log_Cfgf_htm },

   {0, NULL, NULL, NULL }
};



void
ZGATE_WebInit
(
   void
)
{
   if( !ZGATE_CgiSem )
   {
      ZGATE_CgiSem = RZKCreateSemaphore( (RZK_NAME_t *)"ZGATE_CGI", 1, RECV_ORDER_FIFO );
      if( ZGATE_CgiSem )
      {
         http_init( http_defmethods, httpdefheaders, website, 80 );
         //httpDigestAuth_init( httpDigestAuth_defmethods, httpdefheaders, website, 80 );
         //httpBasicAuth_init( http_defmethods, httpdefheaders, website, 80 );
      }
   }
}
