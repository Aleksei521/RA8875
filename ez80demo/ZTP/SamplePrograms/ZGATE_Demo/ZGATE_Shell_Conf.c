/************************************************************************************************
** File:        ZGATE_Shell_Conf.c
** Description: Definition and setup of ZGATE shell commands.
**
** Copyright 2012 Zilog Inc. ALL RIGHTS RESERVED.
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

 

extern INT16 x_zg_show(    struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[] );
extern INT16 x_zg_config(  struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[] );
extern INT16 x_zg_logging( struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[] );
extern INT16 x_zg_restore( struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[] );
extern INT16 x_zg_save(    struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[] );



struct cmdent           ZGATE_Cmds[] = 
{
   { "zg_show",   TRUE, (SHELL_CMD)x_zg_show,   NULL, "Displays ZGATE firewall filtering configuration\r\n" },
   { "zg_config", TRUE, (SHELL_CMD)x_zg_config, NULL, "Updates ZGATE firewall filtering configuration\r\n" },
   { "zg_logging",TRUE, (SHELL_CMD)x_zg_logging,NULL, "Updates ZGATE firewall logging configuration\r\n" },
   { "zg_restore",TRUE, (SHELL_CMD)x_zg_restore,NULL, "Restore ZGATE factory default configuration\r\n" },
   { "zg_save",   TRUE, (SHELL_CMD)x_zg_save,   NULL, "Save ZGATE firewall configuration to persisent memory\r\n" }

};

UINT16 nZGATE_Cmds = sizeof(ZGATE_Cmds) / sizeof(struct cmdent);



void
ZGATE_AddShellCmds
(
   void
)
{
   shell_add_commands( ZGATE_Cmds, nZGATE_Cmds );
}
