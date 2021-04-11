/************************************************************************************************
** File:        PTR_OFS_SEG.h
** Description: Pointer/ Offset_Segment header file definitions for making compile-time pointer
**              definitions based on CSx or Flash project settings.
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

#ifndef _PTR_OFS_SEG_H_
#define _PTR_OFS_SEG_H_


//#include "BaseTypes.h"


/*
 * The LE_OFS_SEG structure allows a little-endian 24-bit pointer to be accessed
 * as a 16-bit offset and 8-bit segment number (each segment is 64KB long).
 */
typedef struct          LE_OFS_SEG_s
{
   UINT16               Ofs;
   UINT8                Seg;
} LE_OFS_SEG;

/*
 * The PTR_OFS_SEG union allows a 24-bit value to be accessed as an arbitrary
 * pointer (HANDLE) or as an LE_OFS_SEG structure.
 */
typedef union           PTR_OFS_SEG_u
{
   LE_OFS_SEG           x;
   HANDLE               Ptr;
} PTR_OFS_SEG;

/*
 * The MK_PTR_OFS_SEG macro allows a 24-bit pointer to be constructed at
 * compile-time from an 8-bit linker symbol identifying the segment value and a
 * 24-bit constant value identifying an offset relative to the start of the 64KB
 * segment.  At run-time, the segment+offset are used as a normal 24-bit pointer
 * eliminating the need to construct the pointer at run-time.
 */
#define MK_PTR_OFS_SEG( Ofs, Seg )    \
   { {(Ofs)&0xFFFF, (UINT8)((Seg) + ((Ofs) >> 16))} }



/*
 * Use the FLASH_ADDR_U_INIT_PARAM definition to create a pointer based on the
 * value of the Flash upper address byte project setting (only available on 
 * eZ80F9x devices).
 */
extern UINT8 _FLASH_ADDR_U_INIT_PARAM;
#define FLASH_ADDR_U_INIT_PARAM        ((UINT8)&_FLASH_ADDR_U_INIT_PARAM)

/*
 * Use the RAM_ADDR_U_INIT_PARAM definition to create a pointer based on the
 * value of the RAM upper address byte project setting (only available on 
 * eZ80F9x devices).
 */
extern UINT8 _RAM_ADDR_U_INIT_PARAM;
#define RAM_ADDR_U_INIT_PARAM          ((UINT8)&_RAM_ADDR_U_INIT_PARAM)

/*
 * Use the CSx_UBR_INIT_PARAM or CSx_LBR_INIT_PARAM definitions to create a 
 * pointer based on the value of the CS0-CS3 chip select upper or lower bound
 * project setting.
 */
extern UINT8 _CS0_UBR_INIT_PARAM;
#define CS0_UBR_INIT_PARAM             ((UINT8)&_CS0_UBR_INIT_PARAM)

extern UINT8 _CS0_LBR_INIT_PARAM;
#define CS0_LBR_INIT_PARAM             ((UINT8)&_CS0_LBR_INIT_PARAM)

extern UINT8 _CS1_UBR_INIT_PARAM;
#define CS1_UBR_INIT_PARAM             ((UINT8)&_CS1_UBR_INIT_PARAM)

extern UINT8 _CS1_LBR_INIT_PARAM;
#define CS1_LBR_INIT_PARAM             ((UINT8)&_CS1_LBR_INIT_PARAM)

extern UINT8 _CS2_UBR_INIT_PARAM;
#define CS2_UBR_INIT_PARAM             ((UINT8)&_CS2_UBR_INIT_PARAM)

extern UINT8 _CS2_LBR_INIT_PARAM;
#define CS2_LBR_INIT_PARAM             ((UINT8)&_CS2_LBR_INIT_PARAM)

extern UINT8 _CS3_UBR_INIT_PARAM;
#define CS3_UBR_INIT_PARAM             ((UINT8)&_CS3_UBR_INIT_PARAM)

extern UINT8 _CS3_LBR_INIT_PARAM;
#define CS3_LBR_INIT_PARAM             ((UINT8)&_CS3_LBR_INIT_PARAM)



#endif
