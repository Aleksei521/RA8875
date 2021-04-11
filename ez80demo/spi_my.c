#include "spi_my.h"
#include <stdio.h>
#include <ez80.h>
#include "ZTypes.h"
#include "ZSysgen.h"
#include "ZDevice.h"
#include "ZInterrupt.h"
#include "ZThread.h"
#include "ZSemaphore.h"

RZK_SEMAPHOREHANDLE_t SpiSemaphore;
RZK_THREADHANDLE_t ghSPI_isr;
#define SPI_ISR_STACK_SIZE		1024	/** stack size for the thread */
#define SPI_ISR_PRIORITY	6	    /** thread priority */

#define ENABLE_PIN_IRQ do{PB_ALT2|=0x40;}while(0)
#define DISABLE_PIN_IRQ do{PB_ALT2&=0xDF;}while(0)
#define SPI_SLAVE_PIN_SELECT do{PB_DR&=0xEF;}while(0) 
#define SPI_SLAVE_PIN_DESELECT do{PB_DR|=0x10;}while(0) 

#define IRQ_EN				 0x80
#define SPI_EN				 0x20
#define MASTER_EN			 0x10
#define CPOL				 0x08
#define CPHA				 0x04

#define SPIF				 0x80
#define WCOL				 0x40
#define MODF				 0x10

#define SPI_BUSY		     0x01

#define SPICMD_STATUS 1

#define SPICMD_STATUS_reset	8
#define SPICMD_STATUS_calibrate	4
#define SPICMD_STATUS_untouch	2
#define SPICMD_STATUS_touch	1
//STATUS output bytes
//STATUS bits
//0-touch detect
//1- untouch detect
//2- 1-calibrated 0-uncalibrated
//3- reset status 1-reset irq
//4-7 осталось количество точек в буфере
//IRQ enable registr
//0-touch irq bit
//1- untouch irq bit
//X point LSByte
//X point MSbyte
//Y point LSByte
//Y point MSByte
#define SPICMD_CALIBRATE 2
//SPICMD_CALIBRATE input bytes
//number point (1..5),0-reset calibrate
//display X point LSByte
//display X point MSByte
//display Y point LSByte
//display Y point MSByte
//touch X point LSByte
//touch X point MSByte
//touch Y point LSByte
//touch Y point MSByte
#define SPICMD_IRQENABLE 3
//IRQ ENABLE 1-ENABLE IRQ
#define SPICMD_IRQDISABLE 4
//input bytes
//IRQ ENABLE 1-DISABLE IRQ
#define SPICMD_SLEEP 5
#define SPICMD_RESET 6
volatile UINT8 spi_tx_buf[16];
volatile UINT8 spi_rx_buf[16];
volatile UINT8 *spi_tx_ptr;
volatile UINT8 *spi_rx_ptr;
volatile UINT8 spi_tx_cnt;
volatile UINT8 spi_rx_cnt;
void (*p_touch_gui)(INT16 x,INT16 y,UINT8 state);

enum {SpiIdle,SpiStatus,SpiCalibrate,SpiIrqEnable,SpiIrqDisable,SpiSleep,SpiReset} volatile e_spi_state=SpiIdle;

extern void spi_pin_isr(void);
extern void spi_isr(void);
void SpiIsrTask(void);

void spi_open(void(*touch_gui)(INT16 x,INT16 y,UINT8 state))
{
//PB3 SCK
//PB2 ~SS input 10k to vdd
//PB6 MISO
//PB7 MOSI
//ALT2-1,ALT1-0,DDR-1,DR-0
//PB4 ~SS output
//ALT2-0,ALT1-1,DDR-0,DR-1
//PB5 IRQ input active High level
//ALT2-1,ALT1-1,DDR-0,DR-1

//ALT2 or 0xEC and 0xEF	
//ALT1 or 0x30 and 0x33
//DDR or 0xCC and 0xCF
//DR or 0x30 and 0x33
	RZKInstallInterruptHandler( (RZK_FNP_ISR)spi_pin_isr, PORTB5_IVECT ) ;
	RZKInstallInterruptHandler( (RZK_FNP_ISR)spi_isr, SPI_IVECT ) ;
	PB_DR&=0x33;
	PB_DR|=0x30;
	PB_DDR&=0xCF;
	PB_DDR|=0xCC;
	PB_ALT1&=0x33;
	PB_ALT1|=0x30;
	PB_ALT2&=0xEF;
	PB_ALT2|=0xEC;
	SPI_BRG_L=25;
	SPI_BRG_H=0;
	SPI_CTL=0x24;
	SPI_CTL|=IRQ_EN;
	p_touch_gui=touch_gui;
	ghSPI_isr = RZKCreateThreadEnhanced(( RZK_NAME_t *) "SPI_ISR",
					(RZK_PTR_t)SpiIsrTask,
					NULL,
					SPI_ISR_STACK_SIZE,
					SPI_ISR_PRIORITY,
					0, 
					RZK_THREAD_PREEMPTION | RZK_THREAD_INTERRUPT,0  ) ; 
	if( ghSPI_isr == NULL )
	{
		printf("\nUnable to create the SPI interrupt thread "); 
		RZKFormatError( RZKGetErrorNum() ) ;
		return ;
	}
	SpiSemaphore=RZKCreateSemaphore((RZK_NAME_t [])"SPI SEMAPHORE",1,RECV_ORDER_PRIORITY);
	if( SpiSemaphore == NULL )
	{
		printf("\nUnable to create the SPI semaphore "); 
		RZKFormatError( RZKGetErrorNum() ) ;
		return ;
	}
}

void SpiSendCmd(UINT8 cmd)
{
	RZKAcquireSemaphore(SpiSemaphore,MAX_INFINITE_SUSPEND);
	switch(cmd)
	{
		case SpiStatus:
			e_spi_state=SpiStatus;
			break;
		case SpiCalibrate:
			e_spi_state=SpiCalibrate;
			break;
		case SpiIrqEnable:
			e_spi_state=SpiIrqEnable;
			break;
		case SpiIrqDisable:
			e_spi_state=SpiIrqDisable;
			break;
		case SpiSleep:
			e_spi_state=SpiSleep;
			break;
		case SpiReset:
			e_spi_state=SpiReset;
			break;
		default: e_spi_state=SpiIdle;
		}
}
void SpiIsrTask(void)
{
	while(1)
	{
		switch(e_spi_state)
		{
			case SpiStatus:
				p_touch_gui(1,2,3);
				RZKReleaseSemaphore(SpiSemaphore);
				break;
			case SpiCalibrate:
				break;
			case SpiIrqEnable:
				break;
			case SpiIrqDisable:
				break;
			case SpiSleep:
				break;
			case SpiReset:
				break;
		}
		e_spi_state=SpiIdle;
		RZKSuspendInterruptThread();
	}
}
/*
void interrupt spi_isr(void)
{
	if(SPI_SR&SPIF)
	{
		if(spi_tx_cnt)
		{
			spi_tx_cnt--;
			if(spi_tx_cnt)
			{	
				spi_tx_ptr++;
				SPI_TSR=*spi_tx_ptr;
			}
			else
			{
				SPI_SLAVE_PIN_DESELECT;
				asm("NOP");
				asm("NOP");
				asm("NOP");
				asm("NOP");
				SPI_SLAVE_PIN_SELECT;
				SPI_TSR=0xAA;//read marker
			}
		}
		else
		{
			*spi_rx_ptr=SPI_RBR;
			spi_rx_ptr++;
			spi_rx_cnt--;
			if(spi_rx_cnt)
			{
				SPI_TSR=0xAA;//read marker
			}
			else
			{
				SPI_SLAVE_PIN_DESELECT;
				//Обработка
			}
		}
	}
}*/


