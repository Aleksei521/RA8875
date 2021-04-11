#include <stdio.h>
#include <string.h>
#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZThread.h"
#include "ZDevice.h"
#include "ZInterrupt.h"
#include <ez80.h>
#include "Adafruit_RA8875.h"
#include "ZEventgroup.h"
#include "ugui.h"
#include "spi_my.h"

#define RA8875_CMD_REG      (*(volatile unsigned char*)0xD00001)
#define RA8875_DATA_REG   (*(volatile unsigned char*)0xD00000)

RZK_THREADHANDLE_t ghRA88775,/*ghRA8875wait_isr,*/ghRA8875int_isr;
#define STACK_SIZE		2048	/** stack size for the thread */
#define RA8875_PRIORITY	5	    /** thread priority */
#define RA8875Isr_PRIORITY	3	    /** thread priority */
RZK_EVENTHANDLE_t ehRA8875int;

volatile const char* i_text_ptr;
UINT8 i_isFont;
UG_GUI mygui;
UG_OBJECT testOBJ[16];
UG_BUTTON testBut[16];
UG_WINDOW testWND;
#define TestIntFont

void HwIni(void);
void SwIni();
void RA8875Task(void);
void RA8875intIsrTask(void);
//void RA8875waitIsrTask(void);
void displayOn(UINT8 on);
void PLLinit(void);
void PWM1config(UINT8 on, UINT8 clock);
void PWM2config(UINT8 on, UINT8 clock);
void PWMsetup(UINT8 pw,UINT8 on, UINT8 clock); 
void backlight(UINT8 on);
void brightness(UINT8 val);
void writeReg(UINT8 reg, UINT8 data);
UINT8 readReg(UINT8 reg);
void  writeCommand(UINT8 d);
void  writeData(UINT8 d);
UINT8 waitPoll(UINT8 regname, UINT8 waitflag);
void graphicsMode(void);
void textMode(void);
void textSetCursor(UINT16 x, UINT16 y);
void textTransparent(UINT16 foreColor);
void textWrite(const char* buf);
void textColor(UINT16 foreColor, UINT16 bgColor);
void textEnlarge(UINT8 scale);
void fillScreen(UINT16 color);
void rectHelper(INT16 x, INT16 y, INT16 w, INT16 h, UINT16 color, UINT8 filled);
void circleHelper(INT16 x0, INT16 y0, INT16 r, UINT16 color, UINT8 filled);
void ellipseHelper(INT16 xCenter, INT16 yCenter, INT16 longAxis, INT16 shortAxis, UINT16 color, UINT8 filled);
void triangleHelper(INT16 x0, INT16 y0, INT16 x1, INT16 y1, INT16 x2, INT16 y2, UINT16 color, UINT8 filled);
void curveHelper(INT16 xCenter, INT16 yCenter, INT16 longAxis, INT16 shortAxis, UINT8 curvePart, UINT16 color, UINT8 filled);
void rectCornerHelper(INT16 x, INT16 y, INT16 w, INT16 h, UINT16 cir_x, UINT16 cir_y, UINT16 color, UINT8 filled);
void setXY(UINT16 x, UINT16 y);
void pushPixels(UINT32 num, UINT16 p);
void DrawPixel(INT16 x, INT16 y, UINT16 color);
void DrawNextHPixel(UINT16 color);
void DrawLine(INT16 x0, INT16 y0, INT16 x1, INT16 y1, UINT16 color);
void fillRect(void);
void testButton(UG_MESSAGE* msg);

extern void ra8875int_isr(void);//PD3
//extern void ra8875wait_isr(void);//PD6
extern void InitUART(void);

void switchArial16(void);
void switchTimes16(void);
void switchArial24(void);
void switchTimes24(void);
void switchArial32(void);
void switchTimes32(void);

void RA8875Ini(void)
{
	DDF_STATUS_t status;
	InitUART();
	//spi_open(UG_TouchUpdate);
	RZKInstallInterruptHandler(ra8875int_isr, PORTD3_IVECT);
	//RZKInstallInterruptHandler(ra8875wait_isr, PORTD6_IVECT);
	
	ghRA8875int_isr = RZKCreateThreadEnhanced(( RZK_NAME_t *) "iRA8875int",
					(RZK_PTR_t)RA8875intIsrTask,
					NULL,
					STACK_SIZE,
					RA8875Isr_PRIORITY,
					0, 
					RZK_THREAD_PREEMPTION | RZK_THREAD_INTERRUPT,0  ) ; 
	if( ghRA8875int_isr == NULL )
	{
		printf("\nUnable to create the RA8875 interrupt thread "); 
		RZKFormatError( RZKGetErrorNum() ) ;
		return ;
	}
    
	/*ghRA8875wait_isr = RZKCreateThreadEnhanced(( RZK_NAME_t []) "iRA8875wait",
					(RZK_PTR_t)RA8875waitIsrTask,
					NULL,
					STACK_SIZE,
					RA8875Isr_PRIORITY,
					0, 
					RZK_THREAD_PREEMPTION | RZK_THREAD_INTERRUPT,0  ) ; 
	if( ghRA8875wait_isr == NULL )
	{
		printf("\nUnable to create the RA8875 interrupt thread "); 
		RZKFormatError( RZKGetErrorNum() ) ;
		return ;
	}*/

	ghRA88775 = RZKCreateThreadEnhanced(( RZK_NAME_t *) "RA8875",
					(RZK_PTR_t)RA8875Task,
					NULL,
					STACK_SIZE,
					RA8875_PRIORITY,
					0, 
					RZK_THREAD_PREEMPTION | RZK_THREAD_AUTOSTART,0  ) ; 
	if( ghRA88775 == NULL )
	{
		printf("\nUnable to create the RA8875 thread "); 
		RZKFormatError( RZKGetErrorNum() ) ;
		return ;
	}    
	ehRA8875int=RZKCreateEventGroup(( RZK_NAME_t * )"eRA8875int",1);
	if( ehRA8875int == NULL )
	{
		printf("\nUnable to create the event group RA887int "); 
		RZKFormatError( RZKGetErrorNum() ) ;
		return ;
	}  
}

void SwIni(void)
{
  /* Timing values */
	UINT8 pixclk;
	UINT8 hsync_start;
	UINT8 hsync_pw;
	UINT8 hsync_finetune;
	UINT8 hsync_nondisp;
	UINT8 vsync_pw; 
	UINT16 vsync_nondisp;
	UINT16 vsync_start;

	PLLinit();
	writeReg(RA8875_SYSR, RA8875_SYSR_16BPP | RA8875_SYSR_MCU8);


  /* Set the correct values for the display being used */  
 // if (_size == RA8875_480x272) 
 // {
 //   pixclk          = RA8875_PCSR_PDATL | RA8875_PCSR_4CLK;
 //   hsync_nondisp   = 10;
 //   hsync_start     = 8;
 //   hsync_pw        = 48;
 //   hsync_finetune  = 0;
 //   vsync_nondisp   = 3;
 //   vsync_start     = 8;
 //   vsync_pw        = 10;
 // } 
 // else // (_size == RA8875_800x480)
 // {
    pixclk          = RA8875_PCSR_PDATL | RA8875_PCSR_2CLK;
    hsync_nondisp   = 32;//32
    hsync_start     = 24;//24
    hsync_pw        = 8;
    hsync_finetune  = 2;//2
    vsync_nondisp   = 13;//13
    vsync_start     = 23;//23
    vsync_pw        = 10;
 // }

	_brightness=60;
	_textScale=0;
  
	writeReg(RA8875_PCSR, pixclk);
	RZKSuspendThread(ghRA88775,1);
  
  /* Horizontal settings registers */
	writeReg(RA8875_HDWR, (_width / 8) - 1);                          // H width: (HDWR + 1) * 8 = 480
	writeReg(RA8875_HNDFTR, RA8875_HNDFTR_DE_HIGH + hsync_finetune);
	writeReg(RA8875_HNDR, (hsync_nondisp - hsync_finetune - 2)/8);    // H non-display: HNDR * 8 + HNDFTR + 2 = 10
	writeReg(RA8875_HSTR, hsync_start/8 - 1);                         // Hsync start: (HSTR + 1)*8 
	writeReg(RA8875_HPWR, RA8875_HPWR_LOW + (hsync_pw/8 - 1));        // HSync pulse width = (HPWR+1) * 8
  
  /* Vertical settings registers */
	writeReg(RA8875_VDHR0, (UINT16)(_height - 1) & 0xFF);
	writeReg(RA8875_VDHR1, (UINT16)(_height - 1) >> 8);
	writeReg(RA8875_VNDR0, vsync_nondisp-1);                          // V non-display period = VNDR + 1
	writeReg(RA8875_VNDR1, vsync_nondisp >> 8);
	writeReg(RA8875_VSTR0, vsync_start-1);                            // Vsync start position = VSTR + 1
	writeReg(RA8875_VSTR1, vsync_start >> 8);
	writeReg(RA8875_VPWR, RA8875_VPWR_LOW + vsync_pw - 1);            // Vsync pulse width = VPWR + 1
  
  /* Set active window X */
	writeReg(RA8875_HSAW0, 0);                                        // horizontal start point
	writeReg(RA8875_HSAW1, 0);
	writeReg(RA8875_HEAW0, (UINT16)(_width - 1) & 0xFF);            // horizontal end point
	writeReg(RA8875_HEAW1, (UINT16)(_width - 1) >> 8);
  
  /* Set active window Y */
	writeReg(RA8875_VSAW0, 0);                                        // vertical start point
	writeReg(RA8875_VSAW1, 0);  
	writeReg(RA8875_VEAW0, (UINT16)(_height - 1) & 0xFF);           // horizontal end point
	writeReg(RA8875_VEAW1, (UINT16)(_height - 1) >> 8);
  
  /* ToDo: Setup touch panel? */
	writeReg(RA8875_TPCR0, 0); 
  /* Clear the entire window */
	writeReg(RA8875_INTC2, 0);//RA8875_INTC2_TP
	writeReg(RA8875_INTC1, 0);
	writeReg(RA8875_MCLR, RA8875_MCLR_START | RA8875_MCLR_FULL);
	RZKSuspendThread(ghRA88775,50); 
	backlight(1);
	displayOn(1);
}

void HwIni(void)
{
/*
Инициализируем порты
PD3 Вход прерывания по active low
DR=0,DDR=0,ALT1=1,ALT2=1
Прерывания по active low при инициализации отключены для PD3
DR=0,DDR=0,ALT1=0, ALT2=1	
PD4 Выход на RESET RA8875 open-drain
DR=1,DDR=0,ALT1=1,ALT2=0	
PD5 Выход (1), управляет сигналом ~cs последовательного порта RA8875
DR=1,DDR=0,ALT1=0,ALT2=0
PD6 Вход прерывания по active low	
DR=0,DDR=0,ALT1=1,ALT2=1
Прерывания active low при инициализации отключены для PD6
DR=0,DDR=0,ALT1=0, ALT2=1	
*/
//PD_DR-0A2H,PD_ALT2-0A5H,PD_ALT1-0A4H,PD_DDR-0A3H
#pragma asm
	IN0	A,(0A2H)//PD_DR
	RES 3,A
	SET 4,A
	SET 5,A
	//RES 6,A
	OUT0 (0A2H),A
	IN0	A,(0A5H)//PD_ALT2
	SET 3,A
	RES 4,A
	RES 5,A
	//SET 6,A
	OUT0 (0A5H),A
	IN0	A,(0A4H)//PD_ALT1
	SET 3,A
	SET 4,A
	RES 5,A
	//SET 6,A
	OUT0 (0A4H),A
	IN0	A,(0A3H)//PD_DDR
	RES 3,A
	RES 4,A
	RES 5,A
	//RES 6,A
	OUT0 (0A3H),A
#pragma endasm	
	RZKSuspendThread(ghRA88775,1);
	PD_DR 	&= 0xEF; 
	RZKSuspendThread(ghRA88775,1);
	PD_DR 	|= 0x10; 
	RZKSuspendThread(ghRA88775,1);
	PD_DR 	&= 0xEF; 
	RZKSuspendThread(ghRA88775,1);
	PD_DR 	|= 0x10; 
	RZKSuspendThread(ghRA88775,1);
}

/*typedef struct
{
   UINT8 type;
   UINT8 id;
   UINT8 sub_id;
   UINT8 event;
   void* src;
} UG_MESSAGE;*/

/* Message types */
/*
#define MSG_TYPE_NONE                                 0
#define MSG_TYPE_WINDOW                               1
#define MSG_TYPE_OBJECT                               2
*/
/*
            msg.src = &obj;
            msg.id = obj->type;
            msg.sub_id = obj->id;
            msg.event = obj->event;
*/
void testButton(UG_MESSAGE* msg)
{
	if(msg->type==MSG_TYPE_OBJECT)
	{
		if(msg->id==OBJ_TYPE_BUTTON)
		{
			switch(msg->sub_id)
			{
				case BTN_ID_0:
					
					break;
			}
		}
	}
}

void RA8875Task(void)
{
	char test_buf[256];
	UINT16 i;
	HwIni();
	SwIni();
	UG_Init(&mygui,800,600);
	fillScreen(RA8875_WHITE);

	graphicsMode();
/*	UG_FontSelect(&FONT_12X20);
	UG_ConsoleSetArea(0, 60, 700, 400);
	UG_ConsoleSetBackcolor(C_BLACK);
	UG_ConsoleSetForecolor(C_RED);
	UG_ConsolePutString("Test START>> QWERTYUIOP qwertyuiop\n");
	UG_ConsoleSetForecolor(C_GREEN);
	UG_ConsolePutString("Test END<< ASDFGHJKL asdfghjkl\n");
	RZKSuspendThread(ghRA88775,100);*/
	fillScreen(RA8875_GREEN);
#ifndef TestIntFont	
	circleHelper(20, 20, 10, RA8875_RED,1);
	circleHelper(780, 40, 10, RA8875_RED,1);
	circleHelper(400, 240, 10, RA8875_RED,1);
	circleHelper(40, 440, 10, RA8875_RED,1);
	circleHelper(760, 460, 10, RA8875_RED,1);
	while(1)
	{
	UG_Update();
	RZKSuspendThread(ghRA88775,100);
	}
#endif
#ifdef TestIntFont		
	while(1)
	{
		printf("Windows Create.\n");
		UG_WindowCreate(&testWND, &testOBJ[0], 16,testButton);
		UG_WindowResize( &testWND, 100, 100, 600, 400 );
		UG_WindowSetTitleTextFont( &testWND, &FONT_12X20 );
		UG_WindowSetTitleText( &testWND, "Test Window" );
		UG_ButtonCreate( &testWND, &testBut[0], BTN_ID_0, 20,20 ,120, 70);
		UG_ButtonSetFont( &testWND, BTN_ID_0, &FONT_22X36 );
		UG_ButtonSetText( &testWND, BTN_ID_0, "Test" );
		UG_ButtonShow( &testWND, BTN_ID_0 );
		UG_WindowShow( &testWND);
		UG_Update();
		RZKSuspendThread(ghRA88775,200);
		printf("Windows Delete.\n");
		UG_WindowDelete( &testWND);
		UG_Update();
		RZKSuspendThread(ghRA88775,100);
//		fillScreen(RA8875_RED);
		textMode();
//		textEnlarge(0);
		textColor(RA8875_BLACK,RA8875_WHITE);
//		textTransparent(RA8875_GREEN);

		textSetCursor(0,0);
		switchArial16();
		for(i=0x20;i<0x80;i++) test_buf[i-0x20]=i;
		test_buf[i-0x20]=0;
		textWrite((const char*)test_buf);

		switchTimes16();
		for(i=0x20;i<0x80;i++) test_buf[i-0x20]=i;
		test_buf[i-0x20]=0;
		textWrite((const char*)test_buf);

		textSetCursor(0,100);
		switchArial24();
		for(i=0x20;i<0x80;i++) test_buf[i-0x20]=i;
		test_buf[i-0x20]=0;
		textWrite((const char*)test_buf);

		switchTimes24();
		for(i=0x20;i<0x80;i++) test_buf[i-0x20]=i;
		test_buf[i-0x20]=0;
		textWrite((const char*)test_buf);

		textSetCursor(0,250);
		switchArial32();
		for(i=0x20;i<0x80;i++) test_buf[i-0x20]=i;
		test_buf[i-0x20]=0;
		textWrite((const char*)test_buf);

		switchTimes32();
		for(i=0x20;i<0x80;i++) test_buf[i-0x20]=i;
		test_buf[i-0x20]=0;
		textWrite((const char*)test_buf);
		
		graphicsMode();
		RZKSuspendThread(ghRA88775,100);
		DrawLine(300,300,400,400,RA8875_WHITE);
		rectHelper(600,100,100,100,RA8875_BLACK,0);
		rectHelper(550,150,100,100,RA8875_YELLOW,1);
		rectHelper(450,200,100,100,RA8875_GREEN,0);
		fillRect();
		rectCornerHelper(0,0,200,200,10,10,RA8875_MAGENTA,1);
		circleHelper(400, 240, 100, RA8875_WHITE,1);
		circleHelper(350, 190, 100, RA8875_BLUE,0);
		triangleHelper(50, 50, 100, 100, 150, 20, RA8875_BLACK,0);
		triangleHelper(100, 100, 150, 150, 200, 120, RA8875_MAGENTA,1);
		ellipseHelper(300, 300, 50, 25, RA8875_BLACK,0);
		ellipseHelper(200, 100, 50, 25, RA8875_CYAN,1);
		curveHelper(500, 300, 50, 25, 1, RA8875_BLACK,0);
		curveHelper(550, 350, 50, 25, 2, RA8875_YELLOW,1);
//		RZKSuspendThread(ghRA88775,100);
//		for(i=0;i<480;i++)
//		{
//		setXY(0,i);
//		pushPixels(800,RA8875_GREEN);
//		}
		RZKSuspendThread(ghRA88775,200);
		fillScreen(RA8875_WHITE);
	}
#endif		
	//}
}

/****************************RA8875intIsrTask**********************************************/
void RA8875intIsrTask(void)
{
	while(1)
	{
		RZKPostToEventGroup(ehRA8875int,1,EVENT_OR);
		RZKSuspendInterruptThread();
	}
}
/*****************************RA8875waitIsrTask*********************************************/
/*void RA8875waitIsrTask(void)
{
	UINTRMASK mInterruptMask;
	while(1)
	{
		RZKSuspendInterruptThread();
	}
}*/
/**************************************************************************/
/*!
      Initialise the PLL
*/
/**************************************************************************/
void PLLinit(void) {
//  if (_size == RA8875_480x272) {
//    writeReg(RA8875_PLLC1, RA8875_PLLC1_PLLDIV1 + 11);
//    RZKSuspendThread(ghRA88775,1);
//    writeReg(RA8875_PLLC2, RA8875_PLLC2_DIV4);
//    RZKSuspendThread(ghRA88775,1);
//  }
//  else /* (_size == RA8875_800x480) */ {
    writeReg(RA8875_PLLC1, RA8875_PLLC1_PLLDIV1 + 11);
    RZKSuspendThread(ghRA88775,1);
    writeReg(RA8875_PLLC2, RA8875_PLLC2_DIV4);
    RZKSuspendThread(ghRA88775,1);
//  }
}
/**************************************************************************/
/*!
      Sets the display in graphics mode (as opposed to text mode)
*/
/**************************************************************************/
void graphicsMode(void) {
//	writeReg(RA8875_MWCR0,0);
//	writeReg(RA8875_MWCR0+1,0);
//	writeReg(RA8875_MWCR0+5,0);
	UINT8 temp;
  writeCommand(RA8875_MWCR0);
  temp = RA8875_DATA_REG;
  temp &= ~RA8875_MWCR0_TXTMODE; // bit #7
  writeData(temp);
}
/**************************************************************************/
/*!
      Sets the display in text mode (as opposed to graphics mode)
*/
/**************************************************************************/
void textMode(void) 
{
	UINT8 temp;
  /* Set text mode */
  writeCommand(RA8875_MWCR0);
  temp = RA8875_DATA_REG;
  temp |= RA8875_MWCR0_TXTMODE; // Set bit 7
  writeData(temp);
  
/* Select the internal (ROM) font */
  writeCommand(0x21);
  temp = RA8875_DATA_REG;
  temp &= ~((1<<7) | (1<<5)); // Clear bits 7 and 5
  writeData(temp);
}
void switchTimes16(void)
{
	writeReg(0x21,0x20);
	writeReg(0x06,0x03);
	writeReg(0x05,0x0);
	writeReg(0x2E,0x02);
	writeReg(0x2F,0x92);
	writeReg(0x4E,24);
	writeReg(0x4F,24);
}
void switchArial16(void)
{
	writeReg(0x21,0x20);
	writeReg(0x06,0x03);
	writeReg(0x05,0x0);
	writeReg(0x2E,0x02);
	writeReg(0x2F,0x91);
	writeReg(0x4E,24);
	writeReg(0x4F,24);
}
void switchTimes24(void)
{
	writeReg(0x21,0x20);
	writeReg(0x06,0x03);
	writeReg(0x05,0x0);
	writeReg(0x2E,0x43);
	writeReg(0x2F,0x92);
	writeReg(0x4E,24);
	writeReg(0x4F,24);
}
void switchArial24(void)
{
	writeReg(0x21,0x20);
	writeReg(0x06,0x03);
	writeReg(0x05,0x0);
	writeReg(0x2E,0x43);
	writeReg(0x2F,0x91);
	writeReg(0x4E,24);
	writeReg(0x4F,24);
}
void switchTimes32(void)
{
	writeReg(0x21,0x20);
	writeReg(0x06,0x03);
	writeReg(0x05,0x0);
	writeReg(0x2E,0x84);
	writeReg(0x2F,0x92);
	writeReg(0x4E,24);
	writeReg(0x4F,24);
}
void switchArial32(void)
{
	writeReg(0x21,0x20);
	writeReg(0x06,0x03);
	writeReg(0x05,0x0);
	writeReg(0x2E,0x84);
	writeReg(0x2F,0x91);
	writeReg(0x4E,24);
	writeReg(0x4F,24);
}
/**************************************************************************/
/*!
      Sets the display in text mode (as opposed to graphics mode)
      
      @args x[in] The x position of the cursor (in pixels, 0..1023)
      @args y[in] The y position of the cursor (in pixels, 0..511)
*/
/**************************************************************************/
void textSetCursor(UINT16 x, UINT16 y) 
{
  /* Set cursor location */
  writeCommand(0x2A);
  writeData(x & 0xFF);
  writeCommand(0x2B);
  writeData(x >> 8);
  writeCommand(0x2C);
  writeData(y & 0xFF);
  writeCommand(0x2D);
  writeData(y >> 8);
}
/**************************************************************************/
/*!
      Sets the fore and background color when rendering text
      
      @args foreColor[in] The RGB565 color to use when rendering the text
      @args bgColor[in]   The RGB565 colot to use for the background
*/
/**************************************************************************/
void textColor(UINT16 foreColor, UINT16 bgColor)
{
	UINT8 temp;
  /* Set Fore Color */
  writeCommand(0x63);
  writeData((foreColor & 0xf800) >> 11);
  writeCommand(0x64);
  writeData((foreColor & 0x07e0) >> 5);
  writeCommand(0x65);
  writeData((foreColor & 0x001f));
  
  /* Set Background Color */
  writeCommand(0x60);
  writeData((bgColor & 0xf800) >> 11);
  writeCommand(0x61);
  writeData((bgColor & 0x07e0) >> 5);
  writeCommand(0x62);
  writeData((bgColor & 0x001f));
  
  /* Clear transparency flag */
  writeCommand(0x22);
  temp = RA8875_DATA_REG;
  temp &= ~(1<<6); // Clear bit 6
  writeData(temp);
}
/**************************************************************************/
/*!
      Sets the text enlarge settings, using one of the following values:
      
      0 = 1x zoom
      1 = 2x zoom
      2 = 3x zoom
      3 = 4x zoom
      
      @args scale[in]   The zoom factor (0..3 for 1-4x zoom)
*/
/**************************************************************************/
void textEnlarge(UINT8 scale)
{
	
	UINT8 temp;
  if (scale > 3) scale = 3;

  /* Set font size flags */
  writeCommand(0x22);
  temp = RA8875_DATA_REG;
  temp &= ~(0xF); // Clears bits 0..3
  temp |= scale << 2;
  temp |= scale;
  writeData(temp);  
	
  _textScale = scale;
  writeReg(0x4E,8*scale);
  writeReg(0x4F,16*scale);
}
/**************************************************************************/
/*!
      Sets the fore color when rendering text with a transparent bg
      
      @args foreColor[in] The RGB565 color to use when rendering the text
*/
/**************************************************************************/
void textTransparent(UINT16 foreColor)
{
	UINT8 temp;
  /* Set Fore Color */
  writeCommand(0x63);
  writeData((foreColor & 0xf800) >> 11);
  writeCommand(0x64);
  writeData((foreColor & 0x07e0) >> 5);
  writeCommand(0x65);
  writeData((foreColor & 0x001f));

  /* Set transparency flag */
  writeCommand(0x22);
  temp = RA8875_DATA_REG;
  temp |= (1<<6); // Set bit 6
  writeData(temp);  
}

/**************************************************************************/
void textWrite(const char* buf)
{
	UINT8 reg;
	RZK_STATUS_t t_status;
	
	if((buf!=NULL)&&((*buf)!=0))
	{
		i_text_ptr=buf;
		i_text_ptr++;
		i_isFont=1;
	/*#pragma asm
		LD A,08H
		OUT0 (0CFH), A
		IN0	A,(0A4H)
		SET 3,A
		OUT0 (0A4H),A
	#pragma endasm*/
		//Clear font Interrupt RA8875
		writeReg(RA8875_INTC2,RA8875_INTC2_FONT);
		//Enable font Interrupt RA8875
		reg=readReg(RA8875_INTC1);
		reg |=RA8875_INTC1_FONT;
		writeReg(RA8875_INTC1,reg);
		writeReg(RA8875_MRWC,*buf);
		t_status=RZKPendOnEventGroup(ehRA8875int,1,MAX_INFINITE_SUSPEND,(EVENT_OR | EVENT_CONSUME));
		if(t_status!=RZKERR_SUCCESS)
		{
			printf("Error event int RA8875\n");
			switch(t_status)
			{
				case RZKERR_OBJECT_DELETED: printf("Object Deleted.\n"); break;
				case RZKERR_TIMEOUT: printf("Timeout.\n"); break;
				case RZKERR_CB_BUSY: printf("Oject BUSY.\n"); break;
				case RZKERR_INVALID_HANDLE: printf("Invalid HANDLE.\n"); break;
				default: printf("Unknown Error!\n");
			}
			
		}
		i_isFont=0;
	}
}
/**************************************************************************/
/*!
      Fills the screen with the spefied RGB565 color

      @args color[in] The RGB565 color to use when drawing the pixel
*/
/**************************************************************************/

void fillScreen(UINT16 color)
{  
  rectHelper(0, 0, _width-1, _height-1, color, 1);
}
/**************************************************************************/
/*!
      Helper function for higher level rectangle drawing code
*/
/**************************************************************************/
void rectHelper(INT16 x, INT16 y, INT16 w, INT16 h, UINT16 color, UINT8 filled)
{
  /* Set X */
  writeCommand(0x91);
  writeData(x);
  writeCommand(0x92);
  writeData(x >> 8);
  
  /* Set Y */
  writeCommand(0x93);
  writeData(y); 
  writeCommand(0x94);	   
  writeData(y >> 8);
  
  /* Set X1 */
  writeCommand(0x95);
  writeData(x+w);
  writeCommand(0x96);
  writeData((x+w) >> 8);
  
  /* Set Y1 */
  writeCommand(0x97);
  writeData(y+h); 
  writeCommand(0x98);
  writeData((y+h) >> 8);

  /* Set Color */
  writeCommand(0x63);
  writeData((color & 0xf800) >> 11);
  writeCommand(0x64);
  writeData((color & 0x07e0) >> 5);
  writeCommand(0x65);
  writeData((color & 0x001f));

  /* Draw! */
  writeCommand(RA8875_DCR);
  if (filled)
  {
    writeData(0xB0);
  }
  else
  {
    writeData(0x90);
  }
  
  /* Wait for the command to finish */
  while(waitPoll(RA8875_DCR, RA8875_DCR_LINESQUTRI_STATUS)==0)
  {
	  RZKSuspendThread(ghRA88775,1);
  }
}
/**************************************************************************/
/*!
      Helper function for higher level rectangle with circle corner drawing code
*/
/**************************************************************************/
void rectCornerHelper(INT16 x, INT16 y, INT16 w, INT16 h, UINT16 cir_x, UINT16 cir_y, UINT16 color, UINT8 filled)
{
  /* Set X */
  writeCommand(0x91);
  writeData(x);
  writeCommand(0x92);
  writeData(x >> 8);
  
  /* Set Y */
  writeCommand(0x93);
  writeData(y); 
  writeCommand(0x94);	   
  writeData(y >> 8);
  
  /* Set X1 */
  writeCommand(0x95);
  writeData(x+w);
  writeCommand(0x96);
  writeData((x+w) >> 8);
  
  /* Set Y1 */
  writeCommand(0x97);
  writeData(y+h); 
  writeCommand(0x98);
  writeData((y+h) >> 8);

	/* Set Long and Short Axis */
  writeCommand(0xA1);
  writeData(cir_x);
  writeCommand(0xA2);
  writeData(cir_x >> 8);
  writeCommand(0xA3);
  writeData(cir_y); 
  writeCommand(0xA4);
  writeData(cir_y >> 8);
	
  /* Set Color */
  writeCommand(0x63);
  writeData((color & 0xf800) >> 11);
  writeCommand(0x64);
  writeData((color & 0x07e0) >> 5);
  writeCommand(0x65);
  writeData((color & 0x001f));

  /* Draw! */
  writeCommand(0xA0);
  if (filled)
  {
    writeData(0xE0);
  }
  else
  {
    writeData(0xA0);
  }
  
  /* Wait for the command to finish */
  while(waitPoll(RA8875_ELLIPSE, RA8875_ELLIPSE_STATUS)==0)
  {
	  RZKSuspendThread(ghRA88775,1);
  }
}
/**************************************************************************/
/*!
      Helper function for higher level circle drawing code
*/
/**************************************************************************/
void circleHelper(INT16 x0, INT16 y0, INT16 r, UINT16 color, UINT8 filled)
{
  /* Set X */
  writeCommand(0x99);
  writeData(x0);
  writeCommand(0x9a);
  writeData(x0 >> 8);
  
  /* Set Y */
  writeCommand(0x9b);
  writeData(y0); 
  writeCommand(0x9c);	   
  writeData(y0 >> 8);
  
  /* Set Radius */
  writeCommand(0x9d);
  writeData(r);  
  
  /* Set Color */
  writeCommand(0x63);
  writeData((color & 0xf800) >> 11);
  writeCommand(0x64);
  writeData((color & 0x07e0) >> 5);
  writeCommand(0x65);
  writeData((color & 0x001f));
  
  /* Draw! */
  writeCommand(RA8875_DCR);
  if (filled)
  {
    writeData(RA8875_DCR_CIRCLE_START | RA8875_DCR_FILL);
  }
  else
  {
    writeData(RA8875_DCR_CIRCLE_START | RA8875_DCR_NOFILL);
  }
  
  /* Wait for the command to finish */
  while(waitPoll(RA8875_DCR, RA8875_DCR_CIRCLE_STATUS)==0)
  {
	  RZKSuspendThread(ghRA88775,1);
  }
}
/**************************************************************************/
/*!
      Helper function for higher level triangle drawing code
*/
/**************************************************************************/
void triangleHelper(INT16 x0, INT16 y0, INT16 x1, INT16 y1, INT16 x2, INT16 y2, UINT16 color, UINT8 filled)
{
  /* Set Point 0 */
  writeCommand(0x91);
  writeData(x0);
  writeCommand(0x92);
  writeData(x0 >> 8);
  writeCommand(0x93);
  writeData(y0); 
  writeCommand(0x94);
  writeData(y0 >> 8);

  /* Set Point 1 */
  writeCommand(0x95);
  writeData(x1);
  writeCommand(0x96);
  writeData(x1 >> 8);
  writeCommand(0x97);
  writeData(y1); 
  writeCommand(0x98);
  writeData(y1 >> 8);

  /* Set Point 2 */
  writeCommand(0xA9);
  writeData(x2);
  writeCommand(0xAA);
  writeData(x2 >> 8);
  writeCommand(0xAB);
  writeData(y2); 
  writeCommand(0xAC);
  writeData(y2 >> 8);
  
  /* Set Color */
  writeCommand(0x63);
  writeData((color & 0xf800) >> 11);
  writeCommand(0x64);
  writeData((color & 0x07e0) >> 5);
  writeCommand(0x65);
  writeData((color & 0x001f));
  
  /* Draw! */
  writeCommand(RA8875_DCR);
  if (filled)
  {
    writeData(0xA1);
  }
  else
  {
    writeData(0x81);
  }
  
  /* Wait for the command to finish */
  while(waitPoll(RA8875_DCR, RA8875_DCR_LINESQUTRI_STATUS)==0)
  {
	  RZKSuspendThread(ghRA88775,1);
  }
}
/**************************************************************************/
/*!
      Helper function for higher level ellipse drawing code
*/
/**************************************************************************/
void ellipseHelper(INT16 xCenter, INT16 yCenter, INT16 longAxis, INT16 shortAxis, UINT16 color, UINT8 filled)
{
  /* Set Center Point */
  writeCommand(0xA5);
  writeData(xCenter);
  writeCommand(0xA6);
  writeData(xCenter >> 8);
  writeCommand(0xA7);
  writeData(yCenter); 
  writeCommand(0xA8);
  writeData(yCenter >> 8);

  /* Set Long and Short Axis */
  writeCommand(0xA1);
  writeData(longAxis);
  writeCommand(0xA2);
  writeData(longAxis >> 8);
  writeCommand(0xA3);
  writeData(shortAxis); 
  writeCommand(0xA4);
  writeData(shortAxis >> 8);
  
  /* Set Color */
  writeCommand(0x63);
  writeData((color & 0xf800) >> 11);
  writeCommand(0x64);
  writeData((color & 0x07e0) >> 5);
  writeCommand(0x65);
  writeData((color & 0x001f));
  
  /* Draw! */
  writeCommand(0xA0);
  if (filled)
  {
    writeData(0xC0);
  }
  else
  {
    writeData(0x80);
  }
  
  /* Wait for the command to finish */
  while(waitPoll(RA8875_ELLIPSE, RA8875_ELLIPSE_STATUS)==0)
  {
	  RZKSuspendThread(ghRA88775,1);
  }
}
/**************************************************************************/
/*!
      Helper function for higher level curve drawing code
*/
/**************************************************************************/
void curveHelper(INT16 xCenter, INT16 yCenter, INT16 longAxis, INT16 shortAxis, UINT8 curvePart, UINT16 color, UINT8 filled)
{
  /* Set Center Point */
  writeCommand(0xA5);
  writeData(xCenter);
  writeCommand(0xA6);
  writeData(xCenter >> 8);
  writeCommand(0xA7);
  writeData(yCenter); 
  writeCommand(0xA8);
  writeData(yCenter >> 8);

  /* Set Long and Short Axis */
  writeCommand(0xA1);
  writeData(longAxis);
  writeCommand(0xA2);
  writeData(longAxis >> 8);
  writeCommand(0xA3);
  writeData(shortAxis); 
  writeCommand(0xA4);
  writeData(shortAxis >> 8);
  
  /* Set Color */
  writeCommand(0x63);
  writeData((color & 0xf800) >> 11);
  writeCommand(0x64);
  writeData((color & 0x07e0) >> 5);
  writeCommand(0x65);
  writeData((color & 0x001f));

  /* Draw! */
  writeCommand(0xA0);
  if (filled)
  {
    writeData(0xD0 | (curvePart & 0x03));
  }
  else
  {
    writeData(0x90 | (curvePart & 0x03));
  }
  
  /* Wait for the command to finish */
  while(waitPoll(RA8875_ELLIPSE, RA8875_ELLIPSE_STATUS)==0)
  {
	  RZKSuspendThread(ghRA88775,1);
  }
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void fillRect(void) {
  writeCommand(RA8875_DCR);
  writeData(RA8875_DCR_LINESQUTRI_STOP | RA8875_DCR_DRAWSQUARE);
  writeData(RA8875_DCR_LINESQUTRI_START | RA8875_DCR_FILL | RA8875_DCR_DRAWSQUARE);
  /* Wait for the command to finish */
  while(waitPoll(RA8875_DCR, RA8875_DCR_LINESQUTRI_STATUS)==0)
  {
	  RZKSuspendThread(ghRA88775,1);
  }
}

/**************************************************************************/
/*!
      Sets the current X/Y position on the display before drawing
      
      @args x[in] The 0-based x location
      @args y[in] The 0-base y location
*/
/**************************************************************************/
void setXY(UINT16 x, UINT16 y) {
  writeReg(RA8875_CURH0, x);
  writeReg(RA8875_CURH1, x >> 8);
  writeReg(RA8875_CURV0, y);
  writeReg(RA8875_CURV1, y >> 8);  
}
/**************************************************************************/
/*!
      HW accelerated function to push a chunk of raw pixel data
      
      @args num[in] The number of pixels to push
      @args p[in]   The pixel color to use
*/
/**************************************************************************/
void pushPixels(UINT32 num, UINT16 p) {
	writeCommand(RA8875_MRWC);
  while (num--) {
	RA8875_DATA_REG=p >> 8;
#pragma asm 
	NOP
#pragma endasm	
	RA8875_DATA_REG=p;
  }
}
/**************************************************************************/
/*!
      Draws a single pixel at the specified location

      @args x[in]     The 0-based x location
      @args y[in]     The 0-base y location
      @args color[in] The RGB565 color to use when drawing the pixel
*/
/**************************************************************************/
void DrawPixel(INT16 x, INT16 y, UINT16 color)
{
  writeReg(RA8875_CURH0, x);
  writeReg(RA8875_CURH1, x >> 8);
  writeReg(RA8875_CURV0, y);
  writeReg(RA8875_CURV1, y >> 8);  
  writeCommand(RA8875_MRWC);
  RA8875_DATA_REG=color >> 8;
  RA8875_DATA_REG=color;
}

void DrawNextHPixel(UINT16 color)
{
	writeCommand(RA8875_MRWC);
	RA8875_DATA_REG=color >> 8;
	RA8875_DATA_REG=color;
}

void DrawLine(INT16 x0, INT16 y0, INT16 x1, INT16 y1, UINT16 color)
{
  /* Set X */
  writeCommand(0x91);
  writeData(x0);
  writeCommand(0x92);
  writeData(x0 >> 8);
  
  /* Set Y */
  writeCommand(0x93);
  writeData(y0); 
  writeCommand(0x94);
  writeData(y0 >> 8);
  
  /* Set X1 */
  writeCommand(0x95);
  writeData(x1);
  writeCommand(0x96);
  writeData((x1) >> 8);
  
  /* Set Y1 */
  writeCommand(0x97);
  writeData(y1); 
  writeCommand(0x98);
  writeData((y1) >> 8);
  
  /* Set Color */
  writeCommand(0x63);
  writeData((color & 0xf800) >> 11);
  writeCommand(0x64);
  writeData((color & 0x07e0) >> 5);
  writeCommand(0x65);
  writeData((color & 0x001f));

  /* Draw! */
  writeCommand(RA8875_DCR);
  writeData(0x80);
  
  /* Wait for the command to finish */
  while(waitPoll(RA8875_DCR, RA8875_DCR_LINESQUTRI_STATUS)==0)
  {
  }
}

/**************************************************************************/
/*!
      Turns the display on or off
*/
/**************************************************************************/
void displayOn(UINT8 on) 
{
 if (on) 
   writeReg(RA8875_PWRR, RA8875_PWRR_NORMAL | RA8875_PWRR_DISPON);
 else
   writeReg(RA8875_PWRR, RA8875_PWRR_NORMAL | RA8875_PWRR_DISPOFF);
}

/**************************************************************************/
/*!
		controls the backligh by using PWM engine.
		It handles adafruit board separately
		Parameters:
		on: true(backlight on), false(backlight off)
*/
/**************************************************************************/
void backlight(UINT8 on) //0.69b31 (fixed an issue with adafruit backlight)
{
	if (on == 1){
		PWM1config(1, RA8875_PWM_CLK_DIV16);//setup PWM ch 1 for backlight
		writeReg(RA8875_P1DCR, _brightness);
		writeReg(RA8875_GPIOX, 1);
	} else {
		PWM1config(0, RA8875_PWM_CLK_DIV16);//setup PWM ch 1 for backlight
		writeReg(RA8875_GPIOX, 0);
	}
}

void brightness(UINT8 val) 
{
	_brightness = val;
	writeReg(RA8875_P1DCR, _brightness);
}


/************************* Mid Level ***********************************/


/**************************************************************************/
/*!

*/
/**************************************************************************/
void  writeCommand(UINT8 d) 
{
	RA8875_CMD_REG=d;
}
/**************************************************************************/
/*!

*/
/**************************************************************************/
void  writeData(UINT8 d) 
{
	RA8875_DATA_REG=d;
}
/**************************************************************************/
/*!
      Waits for screen to finish by polling the status!
*/
/**************************************************************************/
UINT8 waitPoll(UINT8 regname, UINT8 waitflag) {
  /* Wait for the command to finish */
    UINT8 temp = readReg(regname);
    if (!(temp & waitflag))
		return 1;
	else
		return 0;
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void PWM1config(UINT8 on, UINT8 clock) {
  if (on) {
    writeReg(RA8875_P1CR, RA8875_P1CR_ENABLE | (clock & 0xF));
  } else {
    writeReg(RA8875_P1CR, RA8875_P1CR_DISABLE | (clock & 0xF));
  }
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void PWM2config(UINT8 on, UINT8 clock) {
  if (on) {
    writeReg(RA8875_P2CR, RA8875_P2CR_ENABLE | (clock & 0xF));
  } else {
    writeReg(RA8875_P2CR, RA8875_P2CR_DISABLE | (clock & 0xF));
  }
}

/************************* Low Level ***********************************/

/**************************************************************************/
/*!

*/
/**************************************************************************/
void writeReg(UINT8 reg, UINT8 data)
{
	RA8875_CMD_REG=reg;
#pragma asm 
	NOP
#pragma endasm	
	RA8875_DATA_REG=data;					//LCD Horizontal Display Width Register
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
UINT8 readReg(UINT8 reg)
{
	RA8875_CMD_REG=reg;
#pragma asm 
	NOP
#pragma endasm	
	return RA8875_DATA_REG;
}


