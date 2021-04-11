/*
 * File: fg_rules_config.c
 *
 * Module Description
 *   Handles rules based configuration for the Floodgate filtering enginer.
 *   Reads rules from a config file (or simulated config file), parses
 *   rules and updates the filtering engine based on these rules.  APIs are
 *   also provided to allow remote management of the rules (i.e., CLI, Web
 *   or other interface.
 *
 *   The config file consists of two types of entries.  Config string and
 *   rule string.  Both are comma separated strings. The format of the strings
 *   is:
 *   "C, 1, ETH_ADDR_FILTER, NONE"
 *   "R, 6, BLACKLIST, ENABLED, UDP_PORT, {6000, 6003, 6006}"
 *
 *    Byte 1: C - config string or R - rule string.
 *
 *    Config strings specify the type of filtering performed for each of the
 *    filtering categories.  This is for static filtering only.  Format of
 *    config string is:
 *    C, N, NAME, VALUE
 *    C - literal character 'C' for a config string
 *    N - Rule Number
 *    NAME - Rule Name.  Supported values are: {ETH_ADDR_FILTER, ETH_FRAME_FILTER,
 *            IP_SRC_ADDR_FILTER, IP_DST_ADDR_FILTER, IP_PROTOCOL_FILTER,
 *            TCP_PORT_FILTER, UDP_PORT_FILTER, ICMP_TYPE_FILTER}
 *    VALUE - Rule value. Supported values are: {NONE, BLACKLIST, WHITELIST}
 *
 *    Rule strings contain the actual filtering rules. The rules define the
 *    actual filtering criteria. If rules strings are enabled they must
 *    be consistent with the config strings for that type of filtering.  I.e.,
 *    if BLACKLIST filtering is enabled for UDP Ports, then a rule string
 *    that is enabled for WHITELIST filtering of UDP ports will result in an
 *    error.  If there is a such a rule string and it is DISABLED, then the
 *    string will be stored but the rule will not be processed.
 *    This is for static filtering only.  Format of a rule string is:
 *    config string is:
 *    R, N, TYPE, ENABLED, NAME, VALUES
 *    R - literal character 'R' for a rule string
 *    N - Rule Number
 *    TYPE - Rule type. Supported types are: {NONE, BLACKLIST, WHITELIST}
 *    ENABLED - Rule enabled. Supported values are: {ENABLED, DISABLED}
 *    NAME - Rule Name.  Supported values are: {IP_SRC_ADDR, IP_PROTOCOL,
 *            TCP_PORT, UDP_PORT }
 *    VALUES - Rule Values. Format is {N1, N2, N3, N4} where each vaulue is a
 *             port number, protocol number or IP address in dot notation.
 *
 * Module entry points:
 *   int fg_read_rules_config_file(void)
 *   void fg_get_rule_string (INT16 rule_number, char *string)
 *
 * Internal functions:
 *   UINT8 fg_extract_next_field (char *string, char *buffer)
 *   UINT8 fg_extract_value_list (char *string, char *buffer)
 *   void fg_parse_config_string (char *string, UINT16 *number,
 *        char *rule_name, char *rule_value)
 *   void fg_parse_rule_string (char *string, UINT16 *number, char *rule_type,
 *        char *rule_enabled, char *rule_name, char *rule_values)
 *   void fg_set_config_value (UINT8 *value, char *rule_value)
 *   void fg_process_ip_addr_rule(char *string, UINT16 rule_number, char *rule_type,
 *        char *rule_enabled, char *rule_values)
 *   void fg_process_ip_protocol_rule(char *string, UINT16 rule_number, char *rule_type,
 *        char *rule_enabled, char *rule_values)
 *   void fg_process_tcp_port_rule(char *string, UINT16 rule_number, char *rule_type,
 *        char *rule_enabled, char *rule_values)
 *   void fg_process_udp_port_rule(char *string, UINT16 rule_number, char *rule_type,
 *        char *rule_enabled, char *rule_values)
 *   void fg_process_eth_frame_rule(char *string, UINT16 rule_number, char *rule_type,
 *        char *rule_enabled, char *rule_values);
 *   void fg_process_eth_addr_rule(char *string, UINT16 rule_number, char *rule_type,
 *        char *rule_enabled, char *rule_values);
 *   void fg_dump_config_rules()
 *   void fg_save_rule(char *string, UINT16 rule_number, UINT8 rule_type, UINT8 rule_enabled,
 *        UINT8 rule_name)
 *
 * Copyright 2009-2011  Icon Laboratories, Inc.
 *
 * This program is the property of Icon Laboratories, Inc. and
 * its contents are proprietary information of Icon Laboratories, Inc.
 * No part of this software program is to be disclosed to anyone.  The
 * use of this software is subject to the license agreement between
 * the users of this softare and Icon Laboratories, Inc.  Use of this
 * software without a license agreement in prohibited.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "ZGATE_Types.h"
#include "fg_zilog.h"
#include "fg_packet.h"
#include "fg_user_inc.h"
#include "fg_firewall.h"


/****************************************************************************/
/* Variables used by this module                                            */
/****************************************************************************/

/****************************************************************************/
/* Globals                                                                  */
/****************************************************************************/
extern Fg_rules_config_info g_fg_rules;
Fg_filtering_rule *g_fg_first_rule;
Fg_filtering_rule *g_fg_last_rule;

/****************************************************************************/
/* Local function prototypes                                                */
/****************************************************************************/
static UINT8 fg_extract_next_field (char *string, char *buffer);
static UINT8 fg_extract_value_list (char *string, char *buffer);
static void fg_parse_config_string (char *string, UINT16 *number, char *rule_name,
	     char *rule_value);
static void fg_parse_rule_string (char *string, UINT16 *number, char *rule_type,
	     char *rule_enabled, char *rule_name, char *rule_values);
static void fg_set_config_value (UINT8 *value, char *rule_value);
static void fg_process_ip_addr_rule(char *string, UINT16 rule_number,
	     char *rule_type, char *rule_enabled, char *rule_values);
static void fg_process_ip_protocol_rule(char *string, UINT16 rule_number, char *rule_type,
             char *rule_enabled, char *rule_values);
static void fg_process_tcp_port_rule(char *string, UINT16 rule_number, char *rule_type,
             char *rule_enabled, char *rule_values);
static void fg_process_udp_port_rule(char *string, UINT16 rule_number, char *rule_type,
             char *rule_enabled, char *rule_values);
static void fg_dump_config_rules(void);
static void fg_save_rule(char *string, UINT16 rule_number, UINT8 rule_type, UINT8 rule_enabled,
             UINT8 rule_name);
static void fg_process_eth_frame_rule(char *string, UINT16 rule_number, char *rule_type,
             char *rule_enabled, char *rule_values);
static void fg_process_eth_addr_rule(char *string, UINT16 rule_number, char *rule_type,
             char *rule_enabled, char *rule_values);

/****************************************************************************/
/* Globals                                                                  */
/****************************************************************************/ 
extern Fg_config_info g_fg_config_info;

/*****************************************************************************
 *
 * Function: fg_extract_next_field (char *string, char *buffer)
 *
 * Description:
 *   This function extracts the next field from string and copies it into
 *   buffer.  Fields are comma separated. 
 *
 * Parameters:
 *  char *string - the string from which to read the next value.
 *  char *buffer - Buffer into which the next value is copied. 
 *
 * Return value: UINT8
 *   The number of bytes in the string that were consumed in reading the
 *   next field. 
 *
 *****************************************************************************/
UINT8 fg_extract_next_field (char *string, char *buffer)
{
    UINT8 i, j;
    UINT16 len;

    i = 0; j = 0;
    len = strlen(string);

    /* move past any white space */
    while (((string[i] == ' ') || (string[i] == ',')) && (i < len))
       i++;

    while ((string[i] != ',') && (i < len))
    {
       buffer[j] = string[i];
       i++; j++;
    }
    buffer[j] = '\0';
    return (i);
}

/*****************************************************************************
 *
 * Function: fg_extract_value_list (char *string, char *buffer)
 *
 * Description:
 *   This function extracts the value list from string and copies it into
 *   buffer.  Value list is enclosed in braces {}
 *
 * Parameters:
 *  char *string - the string from which to read the next value.
 *  char *buffer - Buffer into which the next value is copied. 
 *
 * Return value: UINT8
 *   The number of bytes in the string that were consumed in reading the
 *   next field. 
 *
 *****************************************************************************/
UINT8 fg_extract_value_list (char *string, char *buffer)
{
    UINT8 i, j;
    UINT16 len;

    i = 0; j = 0;
    len = strlen(string);


    /* move past any white space */
    while (((string[i] == ' ') || (string[i] == ',')) && (i < len))
       i++;

    while ((string[i] != '}') && (i < len))
    {
       buffer[j] = string[i];
       i++; j++;
    }
    buffer[j] = '}';
    buffer[j+1] = '\0';
    return (i+1);
    
}

/*****************************************************************************
 *
 * Function: fg_parse_config_string (char *string, UINT16 *number, char *rule_name,
	                     char *rule_value)
 *
 * Description:
 *   This function parses a config string, extracting the rule number, rule
 *   name and rule value. 
 *
 * Parameters:
 *  char *string - pointer to the config rule string.
 *  UINT16  *number - rule number is copies here. 
 *  char *rule_name - buffer into which the rule name is copied.
 *  char *rule_value - buffer into which the rule value is copied.
 *
 * Return value: None
 *
 *****************************************************************************/
void fg_parse_config_string (char *string, UINT16 *number, char *rule_name,
	                     char *rule_value)
{
   INT16 i;
   UINT16 len;
   char buffer[250]; // working buffer

   /* string format should be
    * "C, 1, ETH_ADDR_FILTER, NONE"
    */
   len = strlen(string);
   i = 3; //set i to point rule number

   while (i < len)
   {
       // get the rule number
       i += fg_extract_next_field (&string[i], buffer);
       *number = atoi(buffer);

       // get the rule name
       i += fg_extract_next_field (&string[i], rule_name);
       
       // get the rule value
       i += fg_extract_next_field (&string[i], rule_value);

   }
 
}

/*****************************************************************************
 *
 * Function: fg_parse_rule_string (char *string, UINT16 *number, char *rule_name,
	              char *rule_enabled, char *rule_name, char *rule_values)
 *
 * Description:
 *   This function parses a config string, extracting the rule number, rule
 *   name and rule value. 
 *
 * Parameters:
 *  char *string - pointer to the config rule string.
 *  UINT16  *number - rule number is copies here. 
 *  char *rule_enabled - buffer into which the rule enabled value is copied.
 *  char *rule_name    - buffer into which the rule name is copied.
 *  char *rule_value   - buffer into which the rule value is copied.
 *
 * Return value: None
 *
 *****************************************************************************/
void fg_parse_rule_string (char *string, UINT16 *number, char *rule_type,
	       char *rule_enabled, char *rule_name, char *rule_values)
{
   INT16 i;
   UINT16 len;
   char buffer[250]; // working buffer

   
   /* string format should be
    * "R, 1, BLACKLIST, ENABLED, IP_ADDR, {192.0.1.50, 192.0.1.78, 192.0.1.111, 192.0.1.112, 192.0.1.145}"
    */
   len = strlen(string);
   i = 3; //set i to point rule number

   while (i < len)
   {
       // get the rule number
       i += fg_extract_next_field (&string[i], buffer);
       *number = atoi(buffer);

       // get the rule type
       i += fg_extract_next_field (&string[i], rule_type);

       // get the rule value
       i += fg_extract_next_field (&string[i], rule_enabled);

       // get the rule value
       i += fg_extract_next_field (&string[i], rule_name);

       // get the rule value
       i += fg_extract_value_list (&string[i], rule_values);

   }

}

/*****************************************************************************
 *
 * Function: fg_set_config_value (UINT8 *value, char *rule_value)
 *
 * Description:
 *   This function converts the config value string read from the config file
 *   into the corresponding enumerated value.
 *
 * Parameters:
 *  UINT8  *value - Number rule value 
 *  char *rule_value - string rule value. 
 *
 * Return value: None
 *
 *****************************************************************************/
void fg_set_config_value (UINT8 *value, char *rule_value)
{
   if (strcmp (rule_value, "NONE") == 0)
   {
      *value = FG_FILTER_DISABLED;
   }
   else if(strcmp (rule_value, "BLACKLIST") == 0)
   {
      *value = FG_BLACKLIST_FILTERING;
   }
   else if(strcmp (rule_value, "WHITELIST") == 0)
   {
      *value = FG_WHITELIST_FILTERING;
   }
   else
   {
     if (g_fg_config_info.debug_level  > 1)
     {
       printf ("BAD VALUE IN CONFIG FILE %s\n", rule_value);
     }
   }
}

/*****************************************************************************
 *
 * Function: fg_process_ip_addr_rule (char *string, UINT16 rule_number,
 *               char *rule_type, char *rule_enabled, char *rule_values)
 *
 * Description:
 *   This function is called to process an IP address rule.  The function
 *   will validate all the values that were read, verify that the filtering
 *   type matches the global configuration (i.e. if floodgate is configured
 *   to do whitelist filtering on IP addresses, then this must be a
 *   whitelist filtering rule).  If the validation tests all pass, then
 *   the function fg_update_ip_addr_config() is called to add the IP addresses
 *   to the filtering configuration and the rule is saved. 
 *
 * Parameters:
 *  char *string - pointer to the rule string.  Saved for CLI display.
 *  UINT16 rule_number - the rule number
 *  char *rule_type - the rule type string
 *  char *rule_enabled - the rule enabled string
 *  char *rule_value - string rule value. 
 *
 * Return value: None
 *
 *****************************************************************************/
void fg_process_ip_addr_rule(char *string, UINT16 rule_number, char *rule_type,
	 char *rule_enabled, char *rule_values)
{
   UINT8 enabled;
   UINT8 type = FG_FILTER_DISABLED;

    if (strcmp (rule_enabled, "ENABLED") == 0)
   {
      enabled = FG_FILTER_ON;
   }
   else if(strcmp (rule_enabled, "DISABLED") == 0)
   {
      enabled = FG_FILTER_OFF;
   }
   else
   {
      enabled = FG_FILTER_OFF;
      if (g_fg_config_info.debug_level  > 1)
      {
         printf ("Bad value for ENABLED/DISABLED = %s\n", rule_enabled);
      }
   }

   if (strcmp (rule_type, "BLACKLIST") == 0)
   {
      type = FG_BLACKLIST_FILTERING;
   }
   else if(strcmp (rule_type, "WHITELIST") == 0)
   {
      type = FG_WHITELIST_FILTERING;
   }
   else
   {
       if (g_fg_config_info.debug_level  > 1)
       {
	   printf ("Bad value for type = %s\n", rule_type);
       }
   }

   // Make sure this rule matches the global config
   if (type != g_fg_rules.ip_src_addr_filtering)
   {
       if (g_fg_config_info.debug_level  > 1)
       {
          printf ("Rule conflicts with global config, rule will be ignored \n");
       }
   }
   else if (enabled == FG_FILTER_ON)
   {
       fg_update_ip_addr_config (type, rule_values);
   }
   
   fg_save_rule(string, rule_number, type, enabled, FG_IP_ADDR_RULE);

}

/*****************************************************************************
 *
 * Function: fg_process_ip_protocol_rule (char *string, UINT16 rule_number,
 *               char *rule_type, char *rule_enabled, char *rule_values)
 *
 * Description:
 *   This function is called to process an IP address rule.  The function
 *   will validate all the values that were read, verify that the filtering
 *   type matches the global configuration (i.e. if floodgate is configured
 *   to do whitelist filtering on IP protocols, then this must be a
 *   whitelist filtering rule).  If the validation tests all pass, then 
 *   the function fg_update_ip_protocol_config() is called to add the IP
 *   protocol values to the filtering configuration and the rule is saved. 
 *
 * Parameters:
 *  char *string - pointer to the rule string.  Saved for CLI display.
 *  UINT16 rule_number - the rule number
 *  char *rule_type - the rule type string
 *  char *rule_enabled - the rule enabled string
 *  char *rule_value - string rule value. 
 *
 * Return value: None
 *
 *****************************************************************************/
void fg_process_ip_protocol_rule(char *string, UINT16 rule_number, char *rule_type,
	char *rule_enabled, char *rule_values)
{
   UINT8 enabled;
   UINT8 type = FG_FILTER_DISABLED;

    
   if (strcmp (rule_enabled, "ENABLED") == 0)
   {
      enabled = FG_FILTER_ON;
   }
   else if(strcmp (rule_enabled, "DISABLED") == 0)
   {
      enabled = FG_FILTER_OFF;
   }
   else
   {
      enabled = FG_FILTER_OFF;
      if (g_fg_config_info.debug_level  > 1)
      {
	  printf ("Bad value for ENABLED/DISABLED = %s\n", rule_enabled);
      }
   }

   if (strcmp (rule_type, "BLACKLIST") == 0)
   {
      type = FG_BLACKLIST_FILTERING;
   }
   else if(strcmp (rule_type, "WHITELIST") == 0)
   {
      type = FG_WHITELIST_FILTERING;
   }
   else
   {
       if (g_fg_config_info.debug_level  > 1)
       {
	   printf ("Bad value for type = %s\n", rule_type);
       }
   }

   // Make sure this rule matches the global config
   if (type != g_fg_rules.ip_protocol_filtering)
   {
       if (g_fg_config_info.debug_level  > 1)
       {
          printf ("Rule conflicts with global config, rule will be ignored \n");
       }
   }
   else if (enabled == FG_FILTER_ON)
   {
       fg_update_ip_protocol_config (type, rule_values);
   }
   fg_save_rule(string, rule_number, type, enabled, FG_IP_PROTOCOL_RULE);
}

/*****************************************************************************
 *
 * Function: fg_process_eth_frame_rule (char *string, UINT16 rule_number,
 *               char *rule_type, char *rule_enabled, char *rule_values)
 *
 * Description:
 *   This function is called to process an ethernet frame rule.  The function
 *   will validate all the values that were read, verify that the filtering
 *   type matches the global configuration (i.e. if floodgate is configured
 *   to do whitelist filtering on ethernet frames, then this must be a
 *   whitelist filtering rule).  If the validation tests all pass, then 
 *   the function fg_update_eth_frame_config() is called to add the Ethernet
 *   frame type values to the filtering configuration and the rule is saved. 
 *
 * Parameters:
 *  char *string - pointer to the rule string.  Saved for CLI display.
 *  UINT16 rule_number - the rule number
 *  char *rule_type - the rule type string
 *  char *rule_enabled - the rule enabled string
 *  char *rule_value - string rule value.
 *
 * Return value: None
 *
 *****************************************************************************/
void fg_process_eth_frame_rule(char *string, UINT16 rule_number, char *rule_type,
	char *rule_enabled, char *rule_values)
{
   UINT8 enabled;
   UINT8 type = FG_FILTER_DISABLED;

    
   if (strcmp (rule_enabled, "ENABLED") == 0)
   {
      enabled = FG_FILTER_ON;
   }
   else if(strcmp (rule_enabled, "DISABLED") == 0)
   {
      enabled = FG_FILTER_OFF;
   }
   else
   {
      enabled = FG_FILTER_OFF;
      if (g_fg_config_info.debug_level  > 1)
      {
	  printf ("Bad value for ENABLED/DISABLED = %s\n", rule_enabled);
      }
   }

   if (strcmp (rule_type, "BLACKLIST") == 0)
   {
      type = FG_BLACKLIST_FILTERING;
   }
   else if(strcmp (rule_type, "WHITELIST") == 0)
   {
      type = FG_WHITELIST_FILTERING;
   }
   else
   {
       if (g_fg_config_info.debug_level  > 1)
       {
	   printf ("Bad value for type = %s\n", rule_type);
       }
   }

   // Make sure this rule matches the global config
   if (type != g_fg_rules.eth_frame_filtering)
   {
       if (g_fg_config_info.debug_level  > 1)
       {
          printf ("Rule conflicts with global config, rule will be ignored \n");
       }
   }
   else if (enabled == FG_FILTER_ON)
   {
       fg_update_eth_frame_config (type, rule_values);
   }
   fg_save_rule(string, rule_number, type, enabled, FG_ETH_FRAME_RULE);
}


/*****************************************************************************
 *
 * Function: fg_process_eth_addr_rule (char *string, UINT16 rule_number,
 *               char *rule_type, char *rule_enabled, char *rule_values)
 *
 * Description:
 *   This function is called to process an ethernet address rule.  The function
 *   will validate all the values that were read, verify that the filtering
 *   type matches the global configuration (i.e. if floodgate is configured
 *   to do whitelist filtering on ethernet addresses, then this must be a
 *   whitelist filtering rule).  If the validation tests all pass, then 
 *   the function fg_update_eth_frame_config() is called to add the Ethernet
 *   address values to the filtering configuration and the rule is saved. 
 *
 * Parameters:
 *  char *string - pointer to the rule string.  Saved for CLI display.
 *  UINT16 rule_number - the rule number
 *  char *rule_type - the rule type string
 *  char *rule_enabled - the rule enabled string
 *  char *rule_value - string rule value. 
 *
 * Return value: None
 *
 *****************************************************************************/
void fg_process_eth_addr_rule(char *string, UINT16 rule_number, char *rule_type,
	char *rule_enabled, char *rule_values)
{
   UINT8 enabled;
   UINT8 type = FG_FILTER_DISABLED;

    
   if (strcmp (rule_enabled, "ENABLED") == 0)
   {
      enabled = FG_FILTER_ON;
   }
   else if(strcmp (rule_enabled, "DISABLED") == 0)
   {
      enabled = FG_FILTER_OFF;
   }
   else
   {
      enabled = FG_FILTER_OFF;
      if (g_fg_config_info.debug_level  > 1)
      {
	  printf ("Bad value for ENABLED/DISABLED = %s\n", rule_enabled);
      }
   }

   if (strcmp (rule_type, "BLACKLIST") == 0)
   {
      type = FG_BLACKLIST_FILTERING;
   }
   else if(strcmp (rule_type, "WHITELIST") == 0)
   {
      type = FG_WHITELIST_FILTERING;
   }
   else
   {
       if (g_fg_config_info.debug_level  > 1)
       {
	   printf ("Bad value for type = %s\n", rule_type);
       }
   }

   // Make sure this rule matches the global config
   if (type != g_fg_rules.eth_addr_filtering)
   {
       if (g_fg_config_info.debug_level  > 1)
       {
          printf ("Rule conflicts with global config, rule will be ignored \n");
       }
   }
   else if (enabled == FG_FILTER_ON)
   {
       fg_update_eth_addr_config (type, rule_values);
   }
   fg_save_rule(string, rule_number, type, enabled, FG_ETH_ADDR_RULE);
}


/*****************************************************************************
 *
 * Function: fg_process_tcp_port_rule (char *string, UINT16 rule_number,
 *               char *rule_type, char *rule_enabled, char *rule_values)
 *
 * Description:
 *   This function is called to process an IP address rule.  The function
 *   will validate all the values that were read, verify that the filtering
 *   type matches the global configuration (i.e. if floodgate is configured
 *   to do whitelist filtering on TCP Ports, then this must be a
 *   whitelist filtering rule).  If the validation tests all pass, then 
 *   the function fg_update_tcp_port_config() is called to add the TCP Port
 *   values to the filtering configuration and the rule is saved. 
 *
 * Parameters:
 *  char *string - pointer to the rule string.  Saved for CLI display.
 *  UINT16 rule_number - the rule number
 *  char *rule_type - the rule type string
 *  char *rule_enabled - the rule enabled string
 *  char *rule_value - string rule value. 
 *
 * Return value: None
 *
 *****************************************************************************/
void fg_process_tcp_port_rule(char *string, UINT16 rule_number, char *rule_type,
            char *rule_enabled, char *rule_values)
{
   UINT8 enabled;
   UINT8 type = FG_FILTER_DISABLED;
  
   if (strcmp (rule_enabled, "ENABLED") == 0)
   {
      enabled = FG_FILTER_ON;
   }
   else if(strcmp (rule_enabled, "DISABLED") == 0)
   {
      enabled = FG_FILTER_OFF;
   }
   else
   {
      enabled = FG_FILTER_OFF;
      if (g_fg_config_info.debug_level  > 1)
      {
         printf ("Bad value for ENABLED/DISABLED = %s\n", rule_enabled);
      }
   }
   
   if (strcmp (rule_type, "BLACKLIST") == 0)
   {
      type = FG_BLACKLIST_FILTERING;
   }
   else if(strcmp (rule_type, "WHITELIST") == 0)
   {
      type = FG_WHITELIST_FILTERING;
   }
   else
   {
       if (g_fg_config_info.debug_level  > 1)
       {
          printf ("Bad value for type = %s\n", rule_type);
       }
   }

   // Make sure this rule matches the global config
   if (type != g_fg_rules.tcp_port_filtering)
   {
       if (g_fg_config_info.debug_level  > 1)
       {
	   printf ("Rule conflicts with global config, rule will be ignored \n");
       }
   }
   else if (enabled == FG_FILTER_ON)
   {
       fg_update_tcp_port_config (type, rule_values);
   }
   fg_save_rule(string, rule_number, type, enabled, FG_TCP_PORT_RULE);
}

/*****************************************************************************
 *
 * Function: fg_process_udp_port_rule (char *string, UINT16 rule_number,
 *               char *rule_type, char *rule_enabled, char *rule_values)
 *
 * Description:
 *   This function is called to process an IP address rule.  The function
 *   will validate all the values that were read, verify that the filtering
 *   type matches the global configuration (i.e. if floodgate is configured
 *   to do whitelist filtering on UDP Ports, then this must be a
 *   whitelist filtering rule).  If the validation tests all pass, then 
 *   the function fg_update_udp_port_config() is called to add the UDP Port
 *   values to the filtering configuration and the rule is saved.
 *
 * Parameters:
 *  char *string - pointer to the rule string.  Saved for CLI display.
 *  UINT16 rule_number - the rule number
 *  char *rule_type - the rule type string
 *  char *rule_enabled - the rule enabled string
 *  char *rule_value - string rule value. 
 *
 * Return value: None
 *
 *****************************************************************************/
void fg_process_udp_port_rule(char *string, UINT16 rule_number, char *rule_type,
            char *rule_enabled, char *rule_values)
{
   UINT8 enabled;
   UINT8 type = FG_FILTER_DISABLED;
    
     
   if (strcmp (rule_enabled, "ENABLED") == 0)
   {
      enabled = FG_FILTER_ON;
   }
   else if(strcmp (rule_enabled, "DISABLED") == 0)
   {
      enabled = FG_FILTER_OFF;
   }
   else
   {
       enabled = FG_FILTER_OFF;
       if (g_fg_config_info.debug_level  > 1)
       {
	   printf ("Bad value for ENABLED/DISABLED = %s\n", rule_enabled);
       }
   }

   if (strcmp (rule_type, "BLACKLIST") == 0)
   {
      type = FG_BLACKLIST_FILTERING;
   }
   else if(strcmp (rule_type, "WHITELIST") == 0)
   {
      type = FG_WHITELIST_FILTERING;
   }
   else
   {
       if (g_fg_config_info.debug_level  > 1)
       {
	   printf ("Bad value for type = %s\n", rule_type);
       }
   }

   // Make sure this rule matches the global config
   if (type != g_fg_rules.udp_port_filtering)
   {
       if (g_fg_config_info.debug_level  > 1)
       {
	   printf ("Rule conflicts with global config, rule will be ignored \n");
       }
   }
   else if (enabled == FG_FILTER_ON)
   {
       fg_update_udp_port_config (type, rule_values);
   }

   fg_save_rule(string, rule_number, type, enabled, FG_UDP_PORT_RULE);
}

/*****************************************************************************
 *
 * Function: fg_dump_config_rules ()
 *
 * Description:
 *   Walk through the list of config rules and print them all out.  A debug
 *   function. 
 *
 * Parameters: None
 *
 * Return value: None
 *
 *****************************************************************************/
void fg_dump_config_rules(void)
{
    Fg_filtering_rule *fg_rule;

    printf ("Dump config rules \n");
    fg_rule = g_fg_first_rule;
    while (fg_rule != NULL)
    {
	printf ("%s\n", fg_rule->data);
	fg_rule = fg_rule->next;
    }
}


/*****************************************************************************
 *
 * Function: fg_save_rule(char *string, UINT16 rule_number, UINT8 rule_type,
 *                 UINT8 rule_enabled, UINT8 rule_name)
 *
 * Description:
 *   This function adds the rule to the list of filtering rules.  Both the
 *   rule string and the enumerated rule information is added to the list. 
 *
 * Parameters:
 *  char *string - pointer to the rule string.  Saved for CLI display.
 *  UINT16 rule_number - the rule number
 *  UINT8 rule_type - enumerated rule type value
 *  UINT8 rule_enabled - enumerated rule enabled value
 *  UINT8 rule_name - enumerated rule name value 
 *
 * Return value: None
 *
 *****************************************************************************/
 void fg_save_rule(char *string, UINT16 rule_number, UINT8 rule_type, UINT8 rule_enabled,
                  UINT8 rule_name)
{
    Fg_filtering_rule *new_rule;
	
#ifdef ZILOG
	/* With the Zilog build and the corresponding new API for Web/CLI updates, the rule
	 * strings are no longer used after startup */
    return;
#endif	
    new_rule = malloc (sizeof(Fg_filtering_rule));
    new_rule->data = malloc (strlen(string)+1);
    strcpy (new_rule->data, string);
    new_rule->rule_number = rule_number;
    new_rule->rule_type = rule_type;
    new_rule->rule_enabled = rule_enabled;
    new_rule->rule_name = rule_name;
    new_rule->next = NULL;
    if (g_fg_first_rule == NULL)
    {
	g_fg_first_rule = new_rule;
	g_fg_last_rule = new_rule;
    }
    else
    {
	g_fg_last_rule->next = new_rule;
	g_fg_last_rule = new_rule;
    }

}

/*****************************************************************************
 *
 * Function: fg_get_rule_string(INT16 rule_number, char *string)
 *
 * Description:
 *   This function returns the string for the rule identified by rule_number.
 *
 * Parameters:
 *  int rule_number - the rule number
 *  char *string - pointer to the rule string.  Saved for CLI display.
 *
 * Return value: None
 *
 *****************************************************************************/
void fg_get_rule_string (UINT16 rule_number, char *string)
{
    Fg_filtering_rule *rule_ptr;

    rule_ptr = g_fg_first_rule;
    sprintf (string, "");

    while (rule_ptr != NULL)
    {
	if (rule_ptr->rule_number == rule_number)
	{
	    strcpy (string, rule_ptr->data);
	    rule_ptr = NULL;
	}
	else
	    rule_ptr = rule_ptr->next;
    }
}

/*****************************************************************************
 *
 * Function: fg_read_rules_config_file()
 *
 * Description:
 *   This function walks through the rules config file, reading the file a
 *   line at a time and processes each rule.
 *
 * Parameters: None
 *
 * Return value: INT8
 *   -1 if an error occured
 *   0 if successful
 *
 *****************************************************************************/
INT8 fg_read_rules_config_file(void)
{
  UINT16 rule_number;
  char rule_name[30];
  char rule_type[30];
  char rule_enabled[30];
  char rule_values[150];
  char string[200];
  int ret_val = 0;
//  INT8 ret_val = EOF;
  FILE *config_file_ptr;


  g_fg_rules.fg_enabled = 1;
#ifdef FG_SPI_FILTER
    g_fg_rules.fg_spi_enabled = TRUE;
#else
    g_fg_rules.fg_spi_enabled = FALSE;
#endif


  /* first open the user config file if it is present */
  config_file_ptr = fopen (FG_USER_RULES_FILE, "r");

  /* If that failed, open the default config file */
  if (config_file_ptr == NULL)
  {
     if (g_fg_config_info.debug_level  > 1)
	     printf ("Opening the user config file failed, open the default config file\n");

     config_file_ptr = fopen (FG_DEFAULT_RULES_FILE, "r");
     if (config_file_ptr == NULL)
     {
        if (g_fg_config_info.debug_level  > 1)
     	   printf ("Error opening %s\n", FG_DEFAULT_RULES_FILE);
     }
  }


  if (config_file_ptr == NULL)
  {
     ret_val = fg_read_string (string);
     if (g_fg_config_info.debug_level  > 2)
     {
        printf ("fg_read_string() returned %d bytes - string = %s \n", ret_val, string);
     }
  }
  else
  {
     //***MCB***ret_val = fscanf(config_file_ptr,"%s",string);
     ret_val = fread( string, 1, 200, config_file_ptr );
     if (g_fg_config_info.debug_level  > 2)
     {
        printf ("fread returned %d bytes.  String = %s\n", ret_val, string);
     }
  }

  //***MCB***while (ret_val != EOF)
   while( ret_val > 0 )
  {
    /* If it is a global rule, process it */
    if (string[0] == 'G')
    {
        fg_parse_global_string(&string[2]);
    }
    /* If it is a comment line, skip it */
    else if (string[0] != '#')
    {
       if (string[0] == 'C')
       {
          fg_parse_config_string(string, &rule_number, &rule_name[0], &rule_values[0]);
       }
       else if (string[0] == 'R')
       {
          fg_parse_rule_string(string, &rule_number, &rule_type[0], &rule_enabled[0], &rule_name[0], &rule_values[0]);
       }

       if (strcmp (rule_name, "ETH_ADDR_FILTER") == 0)
       {
          fg_set_config_value (&g_fg_rules.eth_addr_filtering, rule_values);
       }
       else if (strcmp (rule_name, "ETH_FRAME_FILTER") == 0)
       {
          fg_set_config_value (&g_fg_rules.eth_frame_filtering, rule_values);
       }
       else if (strcmp (rule_name, "IP_SRC_ADDR_FILTER") == 0)
       {
          fg_set_config_value (&g_fg_rules.ip_src_addr_filtering, rule_values);
       }
       //***MCB***else if (strcmp (rule_name, "IP_DEST_ADDR_FILTER") == 0)
       else if (strcmp (rule_name, "IP_DST_ADDR_FILTER") == 0)
       {
       //    set_config_value (&g_fg_rules.ip_dest_addr_filtering, rule_values);
       }
       else if (strcmp (rule_name, "IP_PROTOCOL_FILTER") == 0)
       {
          fg_set_config_value (&g_fg_rules.ip_protocol_filtering, rule_values);
		  // The fg_config_ip_filtering_rules() routine was called earlier, but it needs to be
		  // called again with the proper value for IP Protocol filtering
		  fg_config_ip_filtering_rules();
       }
       else if (strcmp (rule_name, "TCP_PORT_FILTER") == 0)
       {
          fg_set_config_value (&g_fg_rules.tcp_port_filtering, rule_values);
#ifndef FG_MIN
		  // The fg_config_tcp_filtering() routine was called earlier, but it needs to be
		  // called again with the proper value for TCP filtering if we are not in FG_MIN mode
		  fg_init_tcp_filtering();
#endif
       }
       else if (strcmp (rule_name, "UDP_PORT_FILTER") == 0)
       {
          fg_set_config_value (&g_fg_rules.udp_port_filtering, rule_values);
#ifndef FG_MIN
		  // The fg_config_udp_filtering() routine was called earlier, but it needs to be
		  // called again with the proper value for UDP filtering if we are not in FG_MIN mode
		  fg_init_udp_filtering();
#endif
       }
       else if (strcmp (rule_name, "ICMP_TYPE_FILTER") == 0)
       {
          fg_set_config_value (&g_fg_rules.icmp_filtering, rule_values);
       }
       //***MCB***else if (strcmp (rule_name, "IP_SRC_ADDR") == 0)
       else if (strcmp (rule_name, "IP_ADDR") == 0)
       {
          fg_process_ip_addr_rule (string, rule_number, rule_type, rule_enabled, rule_values);
       }
       //***MCB***else if (strcmp (rule_name, "IP_PROTOCOL") == 0)
       else if (strcmp (rule_name, "IP_PROT") == 0)
       {
          fg_process_ip_protocol_rule (string, rule_number, rule_type, rule_enabled, rule_values);
       }
       else if (strcmp (rule_name, "TCP_PORT") == 0)
       {
          fg_process_tcp_port_rule (string, rule_number, rule_type, rule_enabled, rule_values);
       }
       else if (strcmp (rule_name, "UDP_PORT") == 0)
       {
          fg_process_udp_port_rule (string, rule_number, rule_type, rule_enabled, rule_values);
       }
       else if (strcmp (rule_name, "ETH_FRAME") == 0)
       {
          fg_process_eth_frame_rule (string, rule_number, rule_type, rule_enabled, rule_values);
       }
       else if (strcmp (rule_name, "ETH_ADDR") == 0)
       {
          fg_process_eth_addr_rule (string, rule_number, rule_type, rule_enabled, rule_values);
       }

       else
       {
	   if (g_fg_config_info.debug_level  > 1)
	   {
              printf ("Unexpected rule name %s \n", rule_name);
	   }
       }
    }
    memset (string, 0, 160);

  if (config_file_ptr == NULL)
  {
     ret_val = fg_read_string (string);
     if (g_fg_config_info.debug_level  > 1)
     {
        printf ("fg_read_string() - string = %s \n", string);
     }
  }
  else
  {
     //***MCB***ret_val = fscanf(config_file_ptr,"%s",string);
     ret_val = fread( string, 1, 200, config_file_ptr );
     if (g_fg_config_info.debug_level  > 1)
     {
         printf ("fread returned %d bytes.  String = %s\n", ret_val, string);
     }
  }

  }

  if (config_file_ptr != NULL)
     fclose (config_file_ptr);

  
  /* a bit of sanity checking */
  if (g_fg_config_info.interval < 10 || g_fg_config_info.interval > 1800)
     g_fg_config_info.interval = DEFAULT_INTERVAL;
  g_fg_config_info.dbl_interval = g_fg_config_info.interval * 2;

  if (g_fg_config_info.hw_threshold < 100 || g_fg_config_info.hw_threshold > 1000000)
  {
     g_fg_config_info.hw_threshold = DEFAULT_THRESHOLD;
     if (g_fg_config_info.debug_level  > 1)
     {
	 printf ("High water threshold out of range. Using default values.\n");
     }
  }
 
  if ((g_fg_config_info.qtr_threshold < 20) ||
	  ((g_fg_config_info.qtr_threshold * 2) > (g_fg_config_info.hw_threshold)))
  {
		g_fg_config_info.qtr_threshold = g_fg_config_info.hw_threshold / 4;
  }

	
  g_fg_config_info.threeqtr_threshold = g_fg_config_info.hw_threshold - 
	  ((g_fg_config_info.hw_threshold - g_fg_config_info.qtr_threshold) / 4);
  if (g_fg_config_info.debug_level  > 1)
  {
     printf ("HW = %lu, 3/4 = %lu, 1/4 = %lu\n", g_fg_config_info.hw_threshold,
         g_fg_config_info.threeqtr_threshold, g_fg_config_info.qtr_threshold);
  }
  

  if (g_fg_config_info.debug_level  > 1)
  {
	fg_dump_config_rules();
  }


  return (0);

} /* fg_read_rules_config_file() */



