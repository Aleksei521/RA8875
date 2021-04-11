/**********************************************************************************
 * File       : ZSysgen.h
 *
 * Description: Defines the configurable system parameters
 *
 * Copyright 2016 ZiLOG Inc.  ALL RIGHTS RESERVED.
 *
 * This file contains unpublished confidential and proprietary information
 * of ZiLOG, Inc.
 * NO PART OF THIS WORK MAY BE DUPLICATED, STORED, PUBLISHED OR DISCLOSED 
 * IN ANY FORM WITHOUT THE PRIOR WRITTEN CONSENT OF ZiLOG, INC.
 * This is not a license and no use of any kind of this work is authorized
 * in the absence of a written license granted by ZiLOG, Inc. in ZiLOG's 
 * sole discretion 
 *********************************************************************************/


#ifndef _ZSYSGEN_H_
#define _ZSYSGEN_H_

#ifndef RZK_ZNEO
	#ifndef _IAR_CODE
	#define RZK_TOOLSMITH
	#endif
#else
	#define __ZNEO
#endif

// DIFFERENT CONFIGURATION ENABLE/DISABLE
#ifdef RZKPI
#define RZK_PRIORITYINHERITANCE
#endif

#ifndef __ZNEO
	// BUILD OPTIONS
	#ifdef _IAR_CODE

		// BUILD OPTIONS
		#ifdef RZKDBG
			
		#define RZK_DEBUG
		#define RZK_KERNEL_AWARE
		#undef RZK_PERFORMANCE
		#define RZK_STATISTIC
			
		#define RZK_THREAD_CB_SIZE              129
		#define RZK_DEVICE_CB_SIZE              54
		#define RZK_MQ_CB_SIZE                  45
		#define RZK_SEMAPHORE_CB_SIZE           34
		#define RZK_EG_CB_SIZE                  31
		#define RZK_REGION_CB_SIZE              41
		#define RZK_REGIONTAB_CB_SIZE           19
		#define RZK_PARTITION_CB_SIZE           38
		#define RZK_TIMER_CB_SIZE               60
		#define RZK_INTERRUPT_CB_SIZE           53
			
		#else // RZKDBG

		#undef RZK_DEBUG
		#undef RZK_KERNEL_AWARE
		#define RZK_PERFORMANCE
		#undef RZK_STATISTIC
			
		#define RZK_THREAD_CB_SIZE              103
		#define RZK_DEVICE_CB_SIZE              54
		#define RZK_MQ_CB_SIZE                  29
		#define RZK_SEMAPHORE_CB_SIZE           18
		#define RZK_EG_CB_SIZE                  15
		#define RZK_REGION_CB_SIZE              25
		#define RZK_REGIONTAB_CB_SIZE           19
		#define RZK_PARTITION_CB_SIZE           22
		#define RZK_TIMER_CB_SIZE               26
		#define RZK_INTERRUPT_CB_SIZE           27

		#endif // RZKDBG

	#else
		// BUILD OPTIONS
		#ifdef RZKDBG
			
		#define RZK_DEBUG
		#define RZK_KERNEL_AWARE
		#undef RZK_PERFORMANCE
		#define RZK_STATISTIC
			
		#define RZK_THREAD_CB_SIZE              127
		#define RZK_DEVICE_CB_SIZE              54//49
		#define RZK_MQ_CB_SIZE                  53
		#define RZK_SEMAPHORE_CB_SIZE           38
		#define RZK_EG_CB_SIZE                  29
		#define RZK_REGION_CB_SIZE              47
		#define RZK_REGIONTAB_CB_SIZE           18
		#define RZK_PARTITION_CB_SIZE           41
		#define RZK_TIMER_CB_SIZE               54
		#define RZK_INTERRUPT_CB_SIZE           53
			
		#else // RZKDBG

		#undef RZK_DEBUG
		#undef RZK_KERNEL_AWARE
		#define RZK_PERFORMANCE
		#undef RZK_STATISTIC
			
		#define RZK_THREAD_CB_SIZE              102
		#define RZK_DEVICE_CB_SIZE              54//49
		#define RZK_MQ_CB_SIZE                  37
		#define RZK_SEMAPHORE_CB_SIZE           22
		#define RZK_EG_CB_SIZE                  13
		#define RZK_REGION_CB_SIZE              31
		#define RZK_REGIONTAB_CB_SIZE           18
		#define RZK_PARTITION_CB_SIZE           25
		#define RZK_TIMER_CB_SIZE               23
		#define RZK_INTERRUPT_CB_SIZE           28

		#endif // RZKDBG

	#endif
#else
//////////////////////////////////////// FOR ZNEO ////////////////////////////////////////////////////////////////////
		#ifdef RZKDBG
			
				#define RZK_DEBUG
				#define RZK_KERNEL_AWARE
				#undef RZK_PERFORMANCE
				#define RZK_STATISTIC
					
				#define RZK_THREAD_CB_SIZE                  157			
				#define RZK_DEVICE_CB_SIZE              	66			
				#define RZK_MQ_CB_SIZE                  	65			
				#define RZK_SEMAPHORE_CB_SIZE               45  			
				#define RZK_EG_CB_SIZE                  	33			
				#define RZK_REGION_CB_SIZE            	    54			
				#define RZK_REGIONTAB_CB_SIZE               24			
				#define RZK_PARTITION_CB_SIZE               49			
				#define RZK_TIMER_CB_SIZE               	65			
				#define RZK_INTERRUPT_CB_SIZE               63			
			
		#else // RZKDBG

				#undef RZK_DEBUG
				#undef RZK_KERNEL_AWARE
				#define RZK_PERFORMANCE
				#undef RZK_STATISTIC
					
				#define RZK_THREAD_CB_SIZE                  129
				#define RZK_DEVICE_CB_SIZE              	66
				#define RZK_MQ_CB_SIZE                  	49
				#define RZK_SEMAPHORE_CB_SIZE               29
				#define RZK_EG_CB_SIZE                  	17
				#define RZK_REGION_CB_SIZE              	38
				#define RZK_REGIONTAB_CB_SIZE               24
				#define RZK_PARTITION_CB_SIZE               33
				#define RZK_TIMER_CB_SIZE               	29
				#define RZK_INTERRUPT_CB_SIZE               35

		#endif // RZKDBG

#endif
	
#endif /* _ZSYSGEN_H_ */