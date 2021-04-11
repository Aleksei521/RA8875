/************************************************************************************************
** File:        ZGATE.h
** Description: Main header file for ZGATE Firewall.
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

#ifndef _ZGATE_H_
#define _ZGATE_H_





// Return values to specify if packet should be forwarded or not
#define ZGATE_FORWARD_PACKET        1
#define ZGATE_DO_NOT_FORWARD_PACKET 0




// Defines for filtering type
#define ZGATE_FILTER_DISABLED       0
#define ZGATE_BLACKLIST_FILTERING   1
#define ZGATE_WHITELIST_FILTERING   2
 

// defines for static filtering lists (used in list management API functions)
#define ZGATE_ETH_FRAME_LIST        1
#define ZGATE_ETH_ADDR_LIST         2
#define ZGATE_IP_PROT_LIST          3
#define ZGATE_IP_ADDR_LIST          4
#define ZGATE_TCP_PORT_LIST         5
#define ZGATE_UDP_PORT_LIST         6


 // Ethernet framing
#define ETH_ALEN                    6
typedef unsigned char ether_addr_t[ETH_ALEN];




/*
 * ZGATE configuration srings 
 */
extern const char   * ZGATE_ConfigStrings[];


/****************************************************************************/
/* Function prototypes for filtering callbacks                              */
/****************************************************************************/
INT8 ZGATE_st_filter_eth (void * eth_frame_hdr);


/****************************************************************************/
/* Function prototypes for floodgate configuration                          */
/****************************************************************************/
void ZGATE_initialize(void);
void ZGATE_AddShellCmds( void );
void ZGATE_WebInit( void );



/****************************************************************************/
/* Function prototypes for CLI operation && configuration                   */
/****************************************************************************/
UINT8 eth_string_to_num (char * buff, ether_addr_t eth_addr);
INT8 inet_pton(INT8 af, const char *src, unsigned char *dst);

/****************************************************************************/
/* Function prototypes for CLI operations                                   */
/****************************************************************************/
void ZGATE_build_UDP_port_list(char *buff);
void ZGATE_build_TCP_port_list(char *buff);
void ZGATE_build_ip_addr_list(char *buff);
void ZGATE_build_ip_protocol_list(char *buff);
void ZGATE_build_eth_addr_list(char *buff);
void ZGATE_build_eth_frame_list(char *buff);
void ZGATE_get_received_stats (char *buff);
void ZGATE_get_blocked_stats (char *buff);
void ZGATE_get_th_config_string(char *buff);
INT16 ZGATE_eth_frame_filtering_type(void);
INT16 ZGATE_eth_addr_filtering_type(void);
INT16 ZGATE_ip_prot_filtering_type(void);
INT16 ZGATE_ip_addr_filtering_type(void);
INT16 ZGATE_tcp_port_filtering_type(void);
INT16 ZGATE_udp_port_filtering_type(void);
INT16 ZGATE_th_filtering_on(void);
INT16 ZGATE_filtering_on(void);
INT16 ZGATE_set_th_interval(INT32 value);
INT16 ZGATE_set_th_HW(INT32 value);
INT16 ZGATE_set_th_LW(INT32 value);
INT16 ZGATE_add_tcp_port(INT32 value);
INT16 ZGATE_remove_tcp_port(INT32 value);
INT16 ZGATE_add_udp_port(INT32 value);
INT16 ZGATE_remove_udp_port(INT32 value);
INT16 ZGATE_add_eth_addr(ether_addr_t value);
INT16 ZGATE_remove_eth_addr(ether_addr_t value);
INT16 ZGATE_add_eth_frame(INT32 value);
INT16 ZGATE_remove_eth_frame(INT32 value);
INT16 ZGATE_add_ip_addr(INT32 value);
INT16 ZGATE_remove_ip_addr(INT32 value);
INT16 ZGATE_add_ip_prot(INT32 value);
INT16 ZGATE_remove_ip_prot(INT32 value);
INT16 ZGATE_get_list_size (UINT8 table);
INT16 ZGATE_save_config_changes_to_persistent( void );
void ZGATE_use_default_config( void );
void ZGATE_enable_logging_to_screen( void );
void ZGATE_disable_logging_to_screen( void );
void ZGATE_enable_logging_to_file( void );
void ZGATE_disable_logging_to_file( void );
INT16 ZGATE_set_max_logfile_size( INT32 logfile_size, INT32 th_logfile_size );
void ZGATE_get_logging_config(char *buff);


/*
 * ZGATE packet filtering interface
 */
#define ZGATE_INPUT_PATH            1  // Specifies this is a received packet
#define ZGATE_OUTPUT_PATH           2  // Specifies that this is a packet to be sent
typedef BOOL (* IP_FILTER_FUNC)( void * pIpHdr, UINT8 Direction );


#endif
