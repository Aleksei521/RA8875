/************************************************************************************************
** File:        ZGATE_Shell_Cmds.c
** Description: Shell commands to display and/or modify ZGATE configuration.
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

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZThread.h"
#include "shell.h"
#include "netif.h"
#include "socket.h"

#include "ZGATE.h"


char rule_string[2048];


extern void nzgateDisplay(RZK_DEVICE_CB_t *);

void nzgate_statsDisplay(RZK_DEVICE_CB_t *dev);
void nzgate_eth_configDisplay(RZK_DEVICE_CB_t *dev);
void nzgate_ip_configDisplay(RZK_DEVICE_CB_t *dev);
void nzgate_tcp_configDisplay(RZK_DEVICE_CB_t *dev);
void nzgate_udp_configDisplay(RZK_DEVICE_CB_t *dev);
void nzgate_th_configDisplay(RZK_DEVICE_CB_t *dev);
void nzgate_configDisplay(RZK_DEVICE_CB_t *dev);
void nzgate_addremoveCmd(RZK_DEVICE_CB_t *dev, UINT16 nargs, INT8 *args[]);
void nzgate_updateCmd(RZK_DEVICE_CB_t *dev, INT8 *args[]);
void nzgate_eth_addrConfig(RZK_DEVICE_CB_t *dev, UINT16 nargs, INT8 *args[]);
void nzgate_eth_frameConfig(RZK_DEVICE_CB_t *dev, UINT16 nargs, INT8 *args[]);
void nzgate_ip_addrConfig(RZK_DEVICE_CB_t *dev, UINT16 nargs, INT8 *args[]);
void nzgate_ip_protConfig(RZK_DEVICE_CB_t *dev, UINT16 nargs, INT8 *args[]);
void nzgate_tcp_portConfig(RZK_DEVICE_CB_t *dev, UINT16 nargs, INT8 *args[]);
void nzgate_udp_portConfig(RZK_DEVICE_CB_t *dev, UINT16 nargs, INT8 *args[]);
void nzgate_loggingDisplay(RZK_DEVICE_CB_t *dev);
void nzgate_loggingFile(RZK_DEVICE_CB_t *dev, INT8 *args[]);
void nzgate_loggingScreen(RZK_DEVICE_CB_t *dev, INT8 *args[]);
void nzgate_loggingSize(RZK_DEVICE_CB_t *dev, INT8 *args[]);
void nzgate_th_loggingSize(RZK_DEVICE_CB_t *dev, INT8 *args[]);



INT16 x_zg_show(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[])
{
   if (nargs != 2)
   {
      xc_fprintf(stdin,"Usage: zg_show <STATS|ETH|IP|TCP|UDP|TH|ALL>\n");
      return 0;
   }   

   /* Display config overview info */
    nzgate_configDisplay (stdin);
   
   if (xc_strcasecmp(args[1], "STATS") == 0)
   {
      nzgate_statsDisplay(stdin);
   }
   else if (xc_strcasecmp(args[1], "ETH") == 0)
   {
      nzgate_eth_configDisplay(stdin);
   }
   else if (xc_strcasecmp(args[1], "IP") == 0)
   {
      nzgate_ip_configDisplay(stdin);
   }
   else if (xc_strcasecmp(args[1], "TCP") == 0) 
   {
      nzgate_tcp_configDisplay(stdin);
   }
   else if (xc_strcasecmp(args[1], "UDP") == 0)
   {
      nzgate_udp_configDisplay(stdin);
   }
   else if (xc_strcasecmp(args[1], "TH") == 0) 
   {
      nzgate_th_configDisplay(stdin);
   }
   else if (xc_strcasecmp(args[1], "ALL") == 0) 
   {
      nzgate_eth_configDisplay(stdin);
      nzgate_ip_configDisplay(stdin);
      nzgate_tcp_configDisplay(stdin);
      nzgate_udp_configDisplay(stdin);
      nzgate_th_configDisplay(stdin);
   }
   else if ((xc_strcasecmp(args[1], "help") == 0) || (xc_strcasecmp(args[1], "?") == 0)) 
   {
      xc_fprintf(stdin,"Usage: zg_show <STATS|ETH|IP|TCP|UDP|TH|ALL>\n");
      return 0;
   }
   else
   {
      xc_fprintf(stdin,"Usage: zg_show <STATS|ETH|IP|TCP|UDP|TH|ALL>\n");
      return 0;
   }
   return 0;
}


void nzgate_configDisplay(RZK_DEVICE_CB_t *dev)
{
    INT16 filtering_on;
   
   filtering_on = ZGATE_filtering_on();
   if (filtering_on == TRUE)
   {
       xc_fprintf (dev,"\nZGATE filtering enabled\n");
   }
   else
   {
       xc_fprintf (dev,"\nZGATE filtering disabled\n");      
   }
}

void nzgate_statsDisplay(RZK_DEVICE_CB_t *dev)
{
   
   /* print the header */
    xc_fprintf (dev,"\nZGATE filtering statistics - packets processed by ZGATE filters\n\n");
    xc_fprintf (dev,"\n\t\tEthernet filter    IP filter   UDP filter   TCP filter\n");

    /* retrieve and print the received packet stats */   
   ZGATE_get_received_stats (&rule_string[0]);
    xc_fprintf (dev,"Packets processed    ");
    xc_fprintf (dev,"%s", rule_string);

    /* retrieve and print the blocked packet stats */   
   ZGATE_get_blocked_stats (&rule_string[0]);
    xc_fprintf (dev,"Packets blocked      ");
    xc_fprintf (dev,"%s", rule_string);
   
    xc_fprintf (dev,"\n\n");
}

   
void nzgate_eth_configDisplay(RZK_DEVICE_CB_t *dev)
{
    INT16 filter_type;
   
    xc_fprintf (dev,"\nEthernet filtering configuration\n");

   filter_type = ZGATE_eth_frame_filtering_type();
   if (filter_type == ZGATE_FILTER_DISABLED)
   {
        xc_fprintf (dev,"\nEthernet frame filtering disabled\n");
   }
   else if (filter_type == ZGATE_BLACKLIST_FILTERING)
   {
      ZGATE_build_eth_frame_list(&rule_string[0]);
       xc_fprintf (dev,"Ethernet frame blacklist\n");
       xc_fprintf (dev,"%s\n", rule_string);
   }   
   else
   {
      ZGATE_build_eth_frame_list(&rule_string[0]);
       xc_fprintf (dev,"Ethernet frame whitelist\n");
       xc_fprintf (dev,"%s\n", rule_string);      
   }      

   filter_type = ZGATE_eth_addr_filtering_type();
   if (filter_type == ZGATE_FILTER_DISABLED)
   {
        xc_fprintf (dev,"\nEthernet address filtering disabled\n");
   }
   else if (filter_type == ZGATE_BLACKLIST_FILTERING)
   {
      ZGATE_build_eth_addr_list(&rule_string[0]);
       xc_fprintf (dev,"Ethernet address blacklist\n");
       xc_fprintf (dev,"%s\n", rule_string);
   }   
   else
   {
      ZGATE_build_eth_addr_list(&rule_string[0]);
       xc_fprintf (dev,"Ethernet address whitelist\n");
       xc_fprintf (dev,"%s\n", rule_string);   
   }

    xc_fprintf (dev,"\n");
}


void nzgate_ip_configDisplay(RZK_DEVICE_CB_t *dev)
{
    INT16 filter_type;
   
    xc_fprintf (dev,"\nIP filtering configuration\n");

   filter_type = ZGATE_ip_prot_filtering_type();
   if (filter_type == ZGATE_FILTER_DISABLED)
   {
        xc_fprintf (dev,"\nIP protocol filtering disabled\n");
   }
   else if (filter_type == ZGATE_BLACKLIST_FILTERING)
   {
      xc_fprintf (dev,"IP protocol blacklist\n");
      ZGATE_build_ip_protocol_list(&rule_string[0]);
       xc_fprintf (dev,"%s\n", rule_string);
   }
   else
   {
      xc_fprintf (dev,"IP protocol whitelist\n");
      ZGATE_build_ip_protocol_list(&rule_string[0]);
       xc_fprintf (dev,"%s\n", rule_string);
   }

   filter_type = ZGATE_ip_addr_filtering_type();
   if (filter_type == ZGATE_FILTER_DISABLED)
   {
        xc_fprintf (dev,"\nIP address filtering disabled\n");
   }
   else if (filter_type == ZGATE_BLACKLIST_FILTERING)
   {
       xc_fprintf (dev,"IP address blacklist\n");
      ZGATE_build_ip_addr_list(&rule_string[0]);
       xc_fprintf (dev,"%s\n", rule_string);
   }
   else
   {
       xc_fprintf (dev,"IP address whitelist\n");
      ZGATE_build_ip_addr_list(&rule_string[0]);
       xc_fprintf (dev,"%s\n", rule_string);
   }
      
    xc_fprintf (dev,"\n");
}


void nzgate_tcp_configDisplay(RZK_DEVICE_CB_t *dev)
{
    INT16 filter_type;
   
    xc_fprintf (dev,"\nTCP filtering configuration\n");

   filter_type = ZGATE_tcp_port_filtering_type();
   if (filter_type == ZGATE_FILTER_DISABLED)
   {
        xc_fprintf (dev,"\nTCP port filtering disabled\n");
      return;
   }

   if (filter_type == ZGATE_FILTER_DISABLED)
   {
        xc_fprintf (dev,"\nTCP port filtering disabled\n");
      return;
   }
   
   if (filter_type == ZGATE_BLACKLIST_FILTERING)
   {
      xc_fprintf (dev,"TCP port blacklist\n");   
   }   
   else
   {
      xc_fprintf (dev,"TCP port whitelist\n");   
   }

   ZGATE_build_TCP_port_list(&rule_string[0]);
    xc_fprintf (dev,"%s\n", rule_string);

    xc_fprintf (dev,"\n");
}


void nzgate_udp_configDisplay(RZK_DEVICE_CB_t *dev)
{
   INT16 filter_type;

    xc_fprintf (dev,"\nUDP filtering configuration\n");
   
   filter_type = ZGATE_udp_port_filtering_type();
   if (filter_type == ZGATE_FILTER_DISABLED)
   {
        xc_fprintf (dev,"\nUDP port filtering disabled\n");
      return;
   }
   
   if (filter_type == ZGATE_BLACKLIST_FILTERING)
   {
      xc_fprintf (dev,"UDP port blacklist\n");   
   }   
   else
   {
      xc_fprintf (dev,"UDP port whitelist\n");   
   }
   
   ZGATE_build_UDP_port_list(&rule_string[0]);
    xc_fprintf (dev,"%s\n", rule_string);
      
    xc_fprintf (dev,"\n");
}


void nzgate_th_configDisplay(RZK_DEVICE_CB_t *dev)
{
   INT16 filtering_on;
   
   filtering_on = ZGATE_th_filtering_on();
   if (filtering_on == FALSE)
   {
        xc_fprintf (dev,"\nThreshold filtering disabled\n");
   }
   else
   {
      xc_fprintf (dev,"\nThreshold filtering configuration\n");
      ZGATE_get_th_config_string (&rule_string[0]);
      xc_fprintf (dev, "%s\n", rule_string);      
   }
   
    xc_fprintf (dev,"\n");
}



INT16 x_zg_config(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[])
{
   if (nargs < 4)
   {
      xc_fprintf(stdin,"Usage: zg_config <add|remove|set> <PARAMETER> <VALUE(S)>\n");
        if ((xc_strcasecmp(args[1], "help") == 0) || (xc_strcasecmp(args[1], "?") == 0))
      {
         xc_fprintf(stdin,"zg_config <set> <interval|hw|lw> <value>\n");
         xc_fprintf(stdin,"This command sets the threshold filtering interval, high water threshold or low water thereshold\n\n");
         
         xc_fprintf(stdin,"zg_config <add|remove> <PARAMETER> <VALUE(S)>\n");
         xc_fprintf(stdin,"PARAMETER = <eth_addr|eth_frame|ip_addr|ip_prot|tcp_port|udp_port>\n");
         xc_fprintf(stdin,"VALUE(s) is a list of one or more items of the appropriate type separated by spaces.\n");
         xc_fprintf(stdin,"This command adds or remove values from the specified list\n\n");
      }
      return 0;
   }   


   if ((xc_strcasecmp(args[1], "ADD") == 0) || (xc_strcasecmp(args[1], "REMOVE") == 0))
   {
      nzgate_addremoveCmd(stdin, nargs, args);
   }
   else if (xc_strcasecmp(args[1], "SET") == 0)
   {
      nzgate_updateCmd(stdin, args);
   }
   else
   {
      xc_fprintf(stdin,"Usage: zg_config <add|remove|set> <PARAMETER> <VALUE(S)>\n");
      return 0;
   }
   return 0;
}

void nzgate_addremoveCmd(RZK_DEVICE_CB_t *dev, UINT16 nargs, INT8 *args[])
{
   INT32 value = 0;
   INT16 ret_val;

    if (xc_strcasecmp(args[2], "eth_addr") == 0)
   {
      nzgate_eth_addrConfig (dev, nargs, args);
   }
    else if (xc_strcasecmp(args[2], "eth_frame") == 0)
   {
      nzgate_eth_frameConfig(dev, nargs, args);
   }
    else if (xc_strcasecmp(args[2], "ip_addr") == 0)
   {
      nzgate_ip_addrConfig(dev, nargs, args);
   }
    else if (xc_strcasecmp(args[2], "ip_prot") == 0)
   {
      nzgate_ip_protConfig(dev, nargs, args);
   }
    else if (xc_strcasecmp(args[2], "tcp_port") == 0)
   {
      nzgate_tcp_portConfig(dev, nargs, args);   
   }
    else if (xc_strcasecmp(args[2], "udp_port") == 0)
   {
      nzgate_udp_portConfig(dev, nargs, args);      
   }
    else
   {
      xc_fprintf(dev,"Usage: zg_config %s <eth_addr|eth_frame|ip_addr|ip_prot|tcp_port|udp_port> <value(s)>\n", args[1]);   
   }      
}

void nzgate_eth_addrConfig(RZK_DEVICE_CB_t *dev, UINT16 nargs, INT8 *args[])
{
   ether_addr_t value = 0;
   INT16 ret_val;
   int i;

    if (xc_strcasecmp(args[1], "add") == 0)
   {
      for (i = 3; i < nargs; i++)
      {   
         ret_val = eth_string_to_num(args[i], &value);
         if (ret_val == FALSE)
         {
            xc_fprintf(dev,"Usage: zg_config add eth_addr invalid parameter %s\n", args[i]);         
            return;
         }   
         ret_val = ZGATE_add_eth_addr(value);
         if (ret_val == FALSE)
         {
            xc_fprintf(dev,"zg_config add eth_addr %s failed\n", args[i]);         
         }   
      }   
   }
    else if (xc_strcasecmp(args[1], "remove") == 0)
   {
      for (i = 3; i < nargs; i++)
      {   
         ret_val = eth_string_to_num(args[i], &value);
         if (ret_val == FALSE)
         {
            xc_fprintf(dev,"Usage: zg_config remove eth_addr invalid parameter %s\n", args[i]);         
            return;
         }   
         ret_val = ZGATE_remove_eth_addr(value);
         if (ret_val == FALSE)
         {
            xc_fprintf(dev,"zg_config remove eth_addr %s failed\n", args[i]);         
         }   
      }
   }
    else   
   {
      xc_fprintf(dev,"Usage: zg_config <add|remove> eth_addr <value(s)> \n");      
   }      
}


void nzgate_eth_frameConfig(RZK_DEVICE_CB_t *dev, UINT16 nargs, INT8 *args[])
{
   INT32 value = 0;
   INT16 ret_val;
    int i;
   
    if (xc_strcasecmp(args[1], "add") == 0)
   {
      for (i = 3; i < nargs; i++)
      {   
         value = atoi (args[i]);
         if (value == 0)
         {   
            xc_fprintf(dev,"zg_config add eth_frame invalid parameter %s\n", args[i]);         
         }
         else
         {
            ret_val = ZGATE_add_eth_frame(value);
            if (ret_val == FALSE)
            {
               xc_fprintf(dev,"zg_config add eth_frame %s failed\n", args[i]);         
            }   
         }   
      }   
   }
    else if (xc_strcasecmp(args[1], "remove") == 0)
   {
      for (i = 3; i < nargs; i++)
      {   
         value = atoi (args[i]);
         if (value == 0)
         {   
            xc_fprintf(dev,"zg_config remove eth_frame invalid parameter %s\n", args[i]);         
         }
         else
         {
            ret_val = ZGATE_remove_eth_frame(value);
            if (ret_val == FALSE)
            {
               xc_fprintf(dev,"zg_config remove eth_frame %s failed\n", args[i]);         
            }   
         }   
      }
   }
    else
   {
      xc_fprintf(dev,"Usage: zg_config <add|remove> eth_frame <value(s)> \n");      
   }      
}


void nzgate_ip_addrConfig(RZK_DEVICE_CB_t *dev, UINT16 nargs, INT8 *args[])
{
   INT32 value = 0;
   INT16 ret_val;
   int i;
   
    if (xc_strcasecmp(args[1], "add") == 0)
   {
      for (i = 3; i < nargs; i++)
      {         
         ret_val = inet_pton(1, args[i], (void*)&value);
         if (ret_val == 0)
         {
            xc_fprintf(dev,"zg_config add ip_addr invalid parameter %s\n", args[i]);         
            return;
         }
         ret_val = ZGATE_add_ip_addr(value);
         if (ret_val == FALSE)
         {
            xc_fprintf(dev,"zg_config add ip_addr %s failed\n", args[i]);         
         }   
      }
   }
    else if (xc_strcasecmp(args[1], "remove") == 0)
   {
      for (i = 3; i < nargs; i++)
      {            
         ret_val = inet_pton(1, args[i], (void*)&value);
         if (ret_val == 0)
         {
            xc_fprintf(dev,"zg_config remove ip_addr invalid parameter %s\n", args[i]);         
            return;
         }
         ret_val = ZGATE_remove_ip_addr(value);
         if (ret_val == FALSE)
         {
            xc_fprintf(dev,"zg__config remove ip_addr %s failed\n", args[i]);         
         }   
      }
   }
    else
   {
      xc_fprintf(dev,"Usage: zg_config <add|remove> ip_addr <value(s)> \n");      
   }      
}


void nzgate_ip_protConfig(RZK_DEVICE_CB_t *dev, UINT16 nargs, INT8 *args[])
{
   INT32 value = 0;
   INT16 ret_val;
   int i;
   
    if (xc_strcasecmp(args[1], "add") == 0)
   {
      for (i = 3; i < nargs; i++)
      {         
         value = atoi (args[i]);
         if (value == 0)
         {   
            xc_fprintf(dev,"zg_config add ip_prot invalid parameter %s\n", args[i]);         
         }
         else
         {
            ret_val = ZGATE_add_ip_prot(value);
            if (ret_val == FALSE)
            {
               xc_fprintf(dev,"zg_config add ip_prot %s failed\n", args[i]);         
            }
         }
      }
   }
    else if (xc_strcasecmp(args[1], "remove") == 0)
   {
      for (i = 3; i < nargs; i++)
      {         
         value = atoi (args[i]);
         if (value == 0)
         {   
            xc_fprintf(dev,"zg_config remove ip_prot invalid parameter %s\n", args[i]);         
         }
         else
         {
            ret_val = ZGATE_remove_ip_prot(value);
            if (ret_val == FALSE)
            {
               xc_fprintf(dev,"zg_config remove ip_prot %s failed\n", args[i]);         
            }
         }
      }      
   }
    else
   {
      xc_fprintf(dev,"Usage: zg_config <add|remove> ip_prot <value(s)> \n");      
   }      
}



void nzgate_tcp_portConfig(RZK_DEVICE_CB_t *dev, UINT16 nargs, INT8 *args[])
{
   INT32 value = 0;
   INT16 ret_val;
   int i;
   
    if (xc_strcasecmp(args[1], "add") == 0)
   {
      for (i = 3; i < nargs; i++)
      {         
         value = atoi (args[i]);
         if (value == 0)
         {   
            xc_fprintf(dev,"zg_config add tcp_port invalid parameter %s\n", args[i]);         
         }
         else
         {
            ret_val = ZGATE_add_tcp_port(value);
            if (ret_val == FALSE)
            {
               xc_fprintf(dev,"zg_config add tcp_port %s failed\n", args[i]);         
            }
         }   
      }
   }
    else if (xc_strcasecmp(args[1], "remove") == 0)
   {
      for (i = 3; i < nargs; i++)
      {         
         value = atoi (args[i]);
         if (value == 0)
         {   
            xc_fprintf(dev,"zg_config remove tcp_port invalid parameter %s\n", args[i]);         
         }
         else
         {
            ret_val = ZGATE_remove_tcp_port(value);
            if (ret_val == FALSE)
            {
               xc_fprintf(dev,"zg_config remove tcp_port %s failed\n", args[i]);         
            }
         }   
      }
   }
   else
   {
      xc_fprintf(dev,"Usage: zg_config <add|remove> tcp_port <value(s)> \n");      
   }
}



void nzgate_udp_portConfig(RZK_DEVICE_CB_t *dev, UINT16 nargs, INT8 *args[])
{
   INT32 value = 0;
   INT16 ret_val;
   int i;
   
    if (xc_strcasecmp(args[1], "add") == 0)
   {
      for (i = 3; i < nargs; i++)
      {            
         value = atoi (args[i]);
         if (value == 0)
         {   
            xc_fprintf(dev,"zg_config add udp_port invalid parameter %s\n", args[i]);         
         }
         else
         {
            ret_val = ZGATE_add_udp_port(value);
            if (ret_val == FALSE)
            {
               xc_fprintf(dev,"zg_config add udp_port %s failed\n", args[i]);         
            }
         }
      }      
   }
    else if (xc_strcasecmp(args[1], "remove") == 0)
   {
      for (i = 3; i < nargs; i++)
      {            
         value = atoi (args[i]);
         if (value == 0)
         {   
            xc_fprintf(dev,"zg_config remove udp_port invalid parameter %s\n", args[i]);         
         }
         else
         {
            ret_val = ZGATE_remove_udp_port(value);
            if (ret_val == 0)
            {
               xc_fprintf(dev,"zg_config remove udp_port %s failed\n", args[i]);         
            }
         }   
      }   
   }
    else
   {
      xc_fprintf(dev,"Usage: zg_config <add|remove>  <value(s)> \n");      
   }      
}


void nzgate_updateCmd(RZK_DEVICE_CB_t *dev, INT8 *args[])
{
   INT32 value = 0;
   INT16 ret_val;

    if (xc_strcasecmp(args[2], "interval") == 0)
   {
      value = atoi (args[3]);
      if (value == 0)
      {   
         xc_fprintf(dev,"zgate_config set interval: invalid parameter %s\n", args[3]);         
      }
      else
      {
         ret_val = ZGATE_set_th_interval(value);
         if (ret_val == FALSE)
         {
            xc_fprintf(dev,"Setting threshold filtering interval to %s failed\n", args[3]);         
         }   
      }
   }
    else if (xc_strcasecmp(args[2], "HW") == 0)
   {
      value = atoi (args[3]);
      if (value == 0)
      {   
         xc_fprintf(dev,"zgate_config set HW: invalid parameter %s\n", args[3]);         
      }
      else
      {
         ret_val = ZGATE_set_th_HW(value);
         if (ret_val == FALSE)
         {
            xc_fprintf(dev,"Setting High Water threshold to %s failed\n", args[3]);         
         }
      }   
   }
    else if (xc_strcasecmp(args[2], "LW") == 0)
   {
      value = atoi (args[3]);
      if (value == 0)
      {   
         xc_fprintf(dev,"zgate_config set LW: invalid parameter %s\n", args[3]);         
      }
      else
      {
         ret_val = ZGATE_set_th_LW(value);
         if (ret_val == FALSE)
         {
            xc_fprintf(dev,"Setting Low Water threshold to %s failed\n", args[3]);         
         }
      }   
   }
    else
   {
      xc_fprintf(dev,"Usage: zg_config set <interval N|HW N|LW N>\n");   
   }      
}



INT16 x_zg_restore(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[])
{
   if (nargs != 1) 
   {
      xc_fprintf(stdin,"Usage: zg_restore\n");
      xc_fprintf(stdin,"Restores factory default configuration.  This device must be restarted after\n");
      xc_fprintf(stdin,"the restore command is executed.\n");
      return 0;
   }   
   else       
   {
      ZGATE_use_default_config();
      xc_fprintf(stdin,"ZGATE restored to factory default configuration. Reboot now to begin using the default config\n");
      return 0;      
   }
}


INT16 x_zg_save(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[])
{
   if (nargs != 1) 
   {
      xc_fprintf(stdin,"Usage: zg_save\n");
      xc_fprintf(stdin,"Saves the configuration to persistent storage.\n");
      return 0;
   }   
   else       
   {
      ZGATE_save_config_changes_to_persistent();
      xc_fprintf(stdin,"configuration changes saved to persistent storage.\n");
      return 0;      
   }
}


INT16 x_zg_logging(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[])
{
   if (!((nargs == 2) || (nargs == 3)))
   {
      xc_fprintf(stdin,"Usage: zg_logging <size 1..1000 | th_size 1..1000 | screen on/off | file on/off | show>\n");
      return 0;
   }   

   if (nargs == 2)
   {
      if (xc_strcasecmp(args[1], "show") == 0)
      {
         nzgate_loggingDisplay(stdin);
         return 0;
      }
      else if ((xc_strcasecmp(args[1], "help") == 0) || (xc_strcasecmp(args[1], "?") == 0)) 
      {
         xc_fprintf(stdin,"Usage: zg_logging <size 1..1000 | th_size 1..1000 | screen on/off | file on/off | show>\n");
         return 0;
      }
      else 
      {
         xc_fprintf(stdin,"Usage: zg_logging <size 1..1000 | th_size 1..1000 | screen on/off | file on/off | show>\n");
         return 0;      
      }
   }
   
   // If we got here, nargs == 3
   if (xc_strcasecmp(args[1], "size") == 0)
   {
      nzgate_loggingSize(stdin, args);
   }
   else if (xc_strcasecmp(args[1], "th_size") == 0)
   {
      nzgate_th_loggingSize(stdin, args);
   }
   else if (xc_strcasecmp(args[1], "file") == 0)
   {
      nzgate_loggingFile(stdin, args);
   }
   else if (xc_strcasecmp(args[1], "screen") == 0) 
   {
      nzgate_loggingScreen(stdin, args);
   }
   else
   {
      xc_fprintf(stdin,"Usage: zg_logging <size 1..1000 | th_size 1..1000 | screen on/off | file on/off | show>\n");
      return 0;
   }
   return 0;
}

void nzgate_loggingDisplay(RZK_DEVICE_CB_t *dev)
{
   
   /* print the header */
    xc_fprintf (dev,"\nZGATE Logging Configuration\n\n");

    /* retrieve and print the received packet stats */   
   ZGATE_get_logging_config (&rule_string[0]);
    xc_fprintf (dev,"%s", rule_string);
}   


void nzgate_loggingFile(RZK_DEVICE_CB_t *dev, INT8 *args[])
{
   
   if (xc_strcasecmp(args[2], "on") == 0)
   {
      ZGATE_enable_logging_to_file();         
   }
   else if (xc_strcasecmp(args[2], "off") == 0)
   {
      ZGATE_disable_logging_to_file();   
   }
   else
   {   
      xc_fprintf(dev,"Usage: zg_logging file <on|off>\n");         
   }
}      



void nzgate_loggingScreen(RZK_DEVICE_CB_t *dev, INT8 *args[])
{
   
   if (xc_strcasecmp(args[2], "on") == 0)
   {
      ZGATE_enable_logging_to_screen();         
   }
   else if (xc_strcasecmp(args[2], "off") == 0)
   {
      ZGATE_disable_logging_to_screen();   
   }
   else
   {   
      xc_fprintf(dev,"Usage: zg_logging file <on|off>\n");         
   }
}
   
void nzgate_loggingSize(RZK_DEVICE_CB_t *dev, INT8 *args[])
{
   INT32 value = 0;
   INT16 ret_val;
   
   value = atoi (args[2]);
   if (value == 0)
   {   
      xc_fprintf(dev,"zg_logging size invalid parameter %s\n", args[2]);         
   }
   else
   {
      ret_val = ZGATE_set_max_logfile_size(value, -1);
      if (ret_val == FALSE)
      {
         xc_fprintf(dev,"zg_logging size %s failed\n", args[2]);         
      }
   }
}      

void nzgate_th_loggingSize(RZK_DEVICE_CB_t *dev, INT8 *args[])
{
   INT32 value = 0;
   INT16 ret_val;
   
   value = atoi (args[2]);
   if (value == 0)
   {   
      xc_fprintf(dev,"zg_logging th_size invalid parameter %s\n", args[2]);         
   }
   else
   {
      ret_val = ZGATE_set_max_logfile_size(-1, value);
      if (ret_val == FALSE)
      {
         xc_fprintf(dev,"zg_logging th_size %s failed\n", args[2]);         
      }
   }
}      
