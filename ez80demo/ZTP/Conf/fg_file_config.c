/*
 * File: fg_file_config.c
 *
 * Module Description:
 *    This file reads the threshold config file and sets up threshold based
 *    configuration as specified by the config file.
 *
 * Module entry points:
 *    INT8 fg_read_threshold_config_file(void)
 *    void fg_parse_global_string (char *string_ptr)
 *
 * Internal functions:
 *    void fg_parse_threshold_string (char *string, char *label, char *value)
 *
 * Description:
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
#include <string.h>
#include <ctype.h>

#include "ZGATE_Types.h"
#include "fg_zilog.h"
#include "fg_packet.h"
#include "fg_user_inc.h"
#include "fg_firewall.h"



/****************************************************************************/
/* Globals                                                                  */
/****************************************************************************/
extern Fg_rules_config_info g_fg_rules;
extern Fg_fw_stats g_fg_fw_stats;
extern Fg_config_info g_fg_config_info;

/****************************************************************************/
/* Local function prototypes                                                */
/****************************************************************************/
static void fg_parse_threshold_string (char *string, char *label, char *value);


/*****************************************************************************
 *
 * Function: fg_parse_threshold_string (char *string, char *label, char *value)
 *
 * Description:
 *   This function parses a threshold config string.  It assumes the the
 *   format of the string is LABEL=VALUE
 *
 * Parameters:
 *   char *string - pointer to the string to parse
 *   char *label  - pointer to a buffer in which to place the label
 *   char *value  - pointer to a buffer in which to place the value
 *
 * Return value:
 *   None
 *
 *****************************************************************************/
void fg_parse_threshold_string (char *string, char *label, char *value)
{
   UINT16 i;
   UINT16 index = 0;
   UINT16 len;

   len = strlen(string);

   for (i = 0; i < len; i++)
   {
       if (string[i] == '=')
       {
          index = i;
          break;
       }
   }
   if (index != 0)
   {
      strncpy (label, string, index);
      strncpy (value, &string[index+1], len-index);
   }
}

/*****************************************************************************
 *
 * Function: fg_parse_global_string()
 *
 * Description:
 *   This function is passed a line from the config file containing info a
 *   global config paramter.  It parses the string, and sets the config
 *   information based on the information in the string.
 *
 * Parameters:
 *   char *string = the string to be parsed
 *
 *
 *****************************************************************************/
void fg_parse_global_string (char *string_ptr)
{
    INT8  int_result;
    char label [80];
    char value [80];
	char *string;
    int i, len;

	// skip any leadding spaces
	i = 0;
	len = strlen(string_ptr);
 	while ((string_ptr[i] == ' ') && (i < len))
	{
		i++;
	}
    string = &string_ptr[i];


    memset (label, 0, 80);
    memset (value, 0, 80);
    fg_parse_threshold_string (string, label, value);
    if (strcmp (label, "LOG_TO_SCREEN") == 0)
    {
       if (strcmp (value, "ON") == 0)
       {
          g_fg_config_info.log_to_screen = TRUE;
          if (g_fg_config_info.debug_level  > 1)
          {
                printf ("Log to screen = TRUE\n");
	      }
       }
       else
       {
          if (g_fg_config_info.debug_level  > 1)
	      {
                printf ("Log to screen = FALSE\n");
	      }
       }
    }
	else if (strcmp (label, "LOGGING") == 0)
    {
       if (strcmp (value, "ON") == 0)
       {
          g_fg_config_info.log_to_file = TRUE;
          if (g_fg_config_info.debug_level  > 1)
          {
                printf ("Log to screen = TRUE\n");
	      }
       }
       else
       {
          g_fg_config_info.log_to_file = FALSE;
          if (g_fg_config_info.debug_level  > 1)
          {
                printf ("Log to screen = FALSE\n");
          }
       }
    }
    else if (strcmp (label, "INTERVAL") == 0)
    {
	   if (g_fg_config_info.debug_level  > 1)
       {
             printf ("Interval = %s\n", value);
	   }
       g_fg_config_info.interval = atoi(value);
    }
    else if (strcmp (label, "HW_THRESHOLD") == 0)
    {
	   if (g_fg_config_info.debug_level  > 1) 
	   {
	      printf ("High Water threshold = %s\n", value);
	   }
       g_fg_config_info.hw_threshold = atoi(value);
    }
    else if (strcmp (label, "LW_THRESHOLD") == 0)
    {
	   if (g_fg_config_info.debug_level  > 1) 
	   {
	      printf ("Low Water threshold = %s\n", value);
	   }
       g_fg_config_info.qtr_threshold = atoi(value);
    }
    else if (strcmp (label, "MAX_TH_LOGFILE_SIZE") == 0)
    {
  	   if (g_fg_config_info.debug_level  > 1)
	   {
          printf ("Max threshold logfile size = %s\n", value);
	   }
       if (((atoi(value) > 1000)) || (atoi(value) == 0))
	   {
          printf ("Bad value for max threshold logfile size = %s\n", value);
	   }
       else
       {
          g_fg_config_info.max_th_logfile_size = atoi(value);		  
       }
    }
    else if (strcmp (label, "MAX_LOGFILE_SIZE") == 0)
    {
       if (g_fg_config_info.debug_level  > 1)
       {
          printf ("Max logfile size = %s\n", value);
	   }
       if (((atoi(value) > 1000)) || (atoi(value) == 0))
	   {
          printf ("Bad value for max logfile size = %s\n", value);
	   }
       else
       {
          g_fg_config_info.max_logfile_size = atoi(value);		  
       }
    }
    else
    {
       if (g_fg_config_info.debug_level  > 1)
	   {
	      printf ("Unexpected label %s \n", label);
       }
    }

}
#if 0
/*****************************************************************************
 *
 * Function: fg_read_threshold_config_file()
 *
 * Description:
 *   This function reads the threshold config file.  It reads the file a line
 *   at a time, parses each line, and sets the config information based on
 *   the values in the file. 
 *
 * Parameters:
 *   None
 *
 * Return value: Int
 *   0 : Success
 *  -1 : An error occured
 *
 *****************************************************************************/
INT8 fg_read_threshold_config_file(void)
{
  INT8  int_result;
  FILE *config_file_ptr;
  char label [80];
  char value [80];
  char string[160];
  INT8 ret_val;


  config_file_ptr = fopen (FG_USR_TH_FILE, "r");
  if (config_file_ptr == NULL)
  {
	config_file_ptr = fopen (FG_DEFAULT_TH_FILE, "r");  
  }
  if (config_file_ptr == NULL)
  {
     printf ("Error opening %s\n", FG_DEFAULT_TH_FILE);
     return (-1);
  }

  
  
  
  /* Loop that reads a line from the file, parses the line and
   * sets the config information based on the values read
   */
  //***MCB***ret_val = fscanf(config_file_ptr,"%s",string);
  ret_val = fread( string, 1, 160, config_file_ptr );
  //***MCB***while (ret_val != EOF)
  while( ret_val > 0 )
  {
    /* skip comment lines */
    if (string[0] != '#')
    {
       memset (label, 0, 80);
       memset (value, 0, 80);
       fg_parse_threshold_string (string, label, value);
       if (strcmp (label, "LOG_TO_SCREEN") == 0)
       {
          if (strcmp (value, "ON") == 0)
          {
             g_fg_config_info.log_to_screen = TRUE;
             if (g_fg_config_info.debug_level  > 1)
	     {
                printf ("Log to screen = TRUE\n");
	     }
          }
          else
          {
             if (g_fg_config_info.debug_level  > 1)
	     {
                printf ("Log to screen = FALSE\n");
	     }
          }
       }
	   else if (strcmp (label, "LOGGING") == 0)
       {
          if (strcmp (value, "ON") == 0)
          {
             g_fg_config_info.log_to_file = TRUE;
             if (g_fg_config_info.debug_level  > 1)
			 {
                printf ("Log to screen = TRUE\n");
	         }
          }
          else
          {
             g_fg_config_info.log_to_file = FALSE;
             if (g_fg_config_info.debug_level  > 1)
			 {
                printf ("Log to screen = FALSE\n");
			 }
          }
       }
       else if (strcmp (label, "INTERVAL") == 0)
       {
		  if (g_fg_config_info.debug_level  > 1)
		  {
             printf ("Interval = %s\n", value);
	      }
          g_fg_config_info.interval = atoi(value);
       }
       else if (strcmp (label, "HW_THRESHOLD") == 0)
       {
	      if (g_fg_config_info.debug_level  > 1)
	      {
	         printf ("High Water threshold = %s\n", value);
	      }
          g_fg_config_info.hw_threshold = atoi(value);
       }
       else if (strcmp (label, "MAX_TH_LOGFILE_SIZE") == 0)
       {
  	      if (g_fg_config_info.debug_level  > 1)
	      {
             printf ("Max threshold logfile size = %s\n", value);
	      }
          if (((atoi(value) > 1000)) || (atoi(value == 0)))
	      {
             printf ("Bad value for max threshold logfile size = %s\n", value);
	      }
		  else
		  {
			g_fg_config_info.max_th_logfile_size = atoi(value);		  
		  }
       }
       else if (strcmp (label, "MAX_LOGFILE_SIZE") == 0)
       {
  	      if (g_fg_config_info.debug_level  > 1)
	      {
             printf ("Max logfile size = %s\n", value);
	      }
          if (((atoi(value) > 1000)) || (atoi(value == 0)))
	      {
             printf ("Bad value for max logfile size = %s\n", value);
	      }
		  else
		  {
			g_fg_config_info.max_logfile_size = atoi(value);		  
		  }
       }
       else
       {
          if (g_fg_config_info.debug_level  > 1)
	  {
	      printf ("Unexpected label %s \n", label);
	  }
       }
    }
    memset (string, 0, 160);
    //***MCB***ret_val = fscanf(config_file_ptr,"%s",string);
    ret_val = fread( string, 1, 160, config_file_ptr );
  }

  fclose (config_file_ptr);

  if (g_fg_config_info.max_logfile_size == 0)
     g_fg_config_info.max_logfile_size = MAX_LOGFILE_SIZE;

  //***MCB***if (g_fg_config_info.interval < 10 || g_fg_config_info.interval > 1200)
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
  g_fg_config_info.threeqtr_threshold = (g_fg_config_info.hw_threshold/4)*3;
  g_fg_config_info.qtr_threshold = g_fg_config_info.hw_threshold / 4;
  if (g_fg_config_info.debug_level  > 1)
  {
     printf ("HW = %lu, 3/4 = %lu, 1/4 = %lu\n", g_fg_config_info.hw_threshold,
         g_fg_config_info.threeqtr_threshold, g_fg_config_info.qtr_threshold);
  }

  return (0);

} /* fg_read_config_file() */
#endif
