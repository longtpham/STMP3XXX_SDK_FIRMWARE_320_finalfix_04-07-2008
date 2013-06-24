#include "types.h"
#include "messages.h"
#include "sysmem.h"
#include "project.h"
#include "resource.h"
#include "SysResourceApi.h"
#include "regsclkctrl.h"
#include "regspwm.h"	//for backlight circuitry
//#include "display.h"
#include "..\displaydriver_color.h"
#include "s6b33b0a.h"
#include "..\..\..\System\MsgModules\Hardware\Tuner\STFM1000\STFM1000.h"

INT  gTUNER_STFM1000 = 0;
#ifdef CLCD_16BIT
INT  gi16BitOn = 0; // default to not 16bit mode
#endif //#ifdef CLCD_16BIT

#ifndef STMP_BUILD_PLAYER
#include "displaydrv_color.h"
#define HALDisplaySendCommand USBMSC_HALDisplaySendCommand
#define HALDisplaySendPixelData USBMSC_HALDisplaySendPixelData

extern WORD g_ForegroundColor;
extern WORD g_BackgroundColor;

_reentrant void HALDisplaySendCommand(WORD wCommand);
_reentrant void HALDisplaySendPixelData(WORD wData, WORD wDisplayOperation);
#endif

#ifdef FUNCLET
#pragma asm
    extern SysCallFunclet
#pragma endasm    
static void EmptyFunction (void){}  // this is required to get the 1st funclet org to work 
                             		// it costs 1 word in system space
#endif


#ifdef FUNCLET
    #pragma asm
        global FHALDisplayInit
        FHALDisplayInit:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_HAL_DISPLAY_INIT

        org p,"SYSFUNCLET_HALDisplayInit_P":
        dc      RSRC_FUNCLET_HAL_DISPLAY_INIT
    #pragma endasm

_reentrant void  HALDisplayInit_Funclet (void)
{
#else
_reentrant void HALDisplayInit(void)
{
#endif
    int i=0,j;
	int numDataCommands=0;
    WORD *LCDInitSequence;
	WORD temp;
	WORD iStatusOne,iStatusTwo;
	LCDInitSequence = (WORD *) &g_wUserScratchY;

#ifdef CLCD_16BIT
    //***** set GPIO as input first
    LCD_DATA_D00_GPDOE  =  FALSE; 
	LCD_DATA_D00_DI     =  TRUE;
    LCD_DATA_D00_GPEN   =  TRUE; 	  
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    LCD_DATA_D00_GPPWR  = TRUE;    
#endif
	
    LCD_DATA_D03_GPDOE  =  FALSE; 
	LCD_DATA_D03_DI     =  TRUE;
    LCD_DATA_D03_GPEN   =  TRUE; 
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    LCD_DATA_D03_GPPWR = TRUE;    
#endif
	
    LCD_DATA_D04_GPDOE  =  FALSE; 
	LCD_DATA_D04_DI     =  TRUE;
    LCD_DATA_D04_GPEN   =  TRUE;   	    
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    LCD_DATA_D04_GPPWR = TRUE; 	
#endif
    //***** Turn off the 16-bit interface first for STFM1000TestPresence()
    CLCD_16BIT_SWITCH       =  CLCD_16BIT_SWITCH_16BIT_OFF;
    CLCD_16BIT_SWITCH_GPDOE =  TRUE; 	    
    CLCD_16BIT_SWITCH_GPEN  =  TRUE;   	    
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
	CLCD_16BIT_SWITCH_GPPWR = TRUE; 	
#endif
#endif //#ifdef CLCD_16BIT

   #ifdef STFM1000_LCD
  	STFM1000TestPresence();	// For Player & USBMSC, set gTUNER_STFM1000 if STFM1000 present 
   #else
	gTUNER_STFM1000 = 1;	// Default to STFM1000 LCD configuration for Tests
   #endif

    // TO ALWAYS FORCE OLD LCD BOARD LAYOUT, DEFINE THIS MACRO IN LINE BELOW. (test apps need this)
//#define NON_STFM1000_LCD 1
   #ifdef NON_STFM1000_LCD
	gTUNER_STFM1000 = 0;	// Use NON STFM1000 LCD configuration for Tests
   #endif

    //set up the GPIO required, start with the reset pin
    LCD_RESET_GPDO  = TRUE;
    LCD_RESET_GPDOE = TRUE;
    LCD_RESET_GPEN  = TRUE;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    LCD_RESET_GPPWR = TRUE;
#endif
    LCD_DC_GPDO     = TRUE;
    LCD_DC_GPDOE    = TRUE;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    LCD_DC_GPPWR    = TRUE;
#endif
    LCD_DC_GPEN     = TRUE;

#ifdef CLCD_16BIT
    //***** Turn on the 16-bit interface after STFM1000TestPresence()
    CLCD_16BIT_SWITCH   =  CLCD_16BIT_SWITCH_16BIT_OFF;
    LCD_DATA_D00_GPDOE  =  TRUE; 
    LCD_DATA_D03_GPDOE  =  TRUE; 
    LCD_DATA_D04_GPDOE  =  TRUE; 
#endif //#ifdef CLCD_16BIT

	LCD_RESET_GPDO  = FALSE;		//reset the controller...need at least 10 us for reset

    LCD_CS_GPDO     = FALSE;
    LCD_CS_GPDOE    = TRUE;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    LCD_CS_GPPWR    = TRUE;
#endif
    LCD_CS_GPEN     = TRUE;

    LCD_RD_GPDO    = TRUE;
    LCD_RD_GPDOE   = TRUE;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    LCD_RD_GPPWR   = TRUE;
#endif
    LCD_RD_GPEN    = TRUE;

    LCD_WR_GPDO    = TRUE;
    LCD_WR_GPDOE   = TRUE;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    LCD_WR_GPPWR   = TRUE;
#endif
    LCD_WR_GPEN    = TRUE;



#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    LCD_BLON_GPPWR   = TRUE;
#endif
    LCD_BLON_GPEN    = FALSE;	//configure as PWM0 not GPIO
	HW_CCR.U |= ACK_AND_XTAL_ENABLE_MASK;		//ACKEN and XTLEN
	HW_PWM_CSR.I = PWM_ENABLE_MASK; //MASTER ENABLE to Start clocks
	PWMA = PWM_SETUPA;
#ifdef STMP_BUILD_PLAYER 
#ifdef BATTERY_CHARGE
	PWMB = PWM_SETUPB;
#else
	PWMB = PWM_SETUPB_SLOW_PWM;
#endif
#else
	PWMB = PWM_SETUPB;
#endif	


#ifdef CLCD_8BIT
	if (gTUNER_STFM1000)
	{   LCD_DATA_D00        =  TRUE;       
	    LCD_DATA_D00_GPDOE  =  TRUE; 
	    LCD_DATA_D00_GPEN   =  TRUE; 	  
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
	    LCD_DATA_D00_GPPWR  = TRUE;    
#endif
	
	    LCD_DATA_D03       =  TRUE;     
	    LCD_DATA_D03_GPDOE =  TRUE; 
	    LCD_DATA_D03_GPEN  =  TRUE; 
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
	    LCD_DATA_D03_GPPWR = TRUE;    
#endif
	
	    LCD_DATA_D04       =  TRUE; 
	    LCD_DATA_D04_GPDOE =  TRUE; 	    
	    LCD_DATA_D04_GPEN  =  TRUE;   	    
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
		LCD_DATA_D04_GPPWR = TRUE; 	
#endif

	  	LCD_DATA_DO     = 0xe6;     // 1110 0110 except bits 0, 3, 4  
	    LCD_DATA_DOE    = 0xe6;     // 1110 0110 except bits 0, 3, 4
	    LCD_DATA_EN     = 0xe6;     // 1110 0110 except bits 0, 3, 4	 
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
	    LCD_DATA_PWR    = 0xff;
#endif
	}	 
	else // non STFM1000 layout
    {   LCD_DATA_DO     = 0xff;    
	    LCD_DATA_DOE    = 0xff;
	    LCD_DATA_EN     = 0xff;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    	LCD_DATA_PWR    = 0xff;
#endif
     }
#else //#ifdef CLCD_8BIT
    //******* 16-bit interface is already turned on before.
    LCD_DATA_DO     = 0xffe6;    
    LCD_DATA_DOE    = 0xffe6;
    LCD_DATA_EN     = 0xffff;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    LCD_DATA_PWR    |= 0xffff;
#endif
#endif //#ifdef CLCD_8BIT
//********************************************************************************

    LCD_RESET_GPDO  = TRUE;

#pragma asm
	rep #4095
	nop
	nop
#pragma endasm

    SysLoadResource(RSRC_LCD_INIT_SEQ, RS_ROOT_HANDLE, RSRC_TYPE_DATA,
                        LCDInitSequence,TARGET_MEM_Y,65*3);

    LCDInitSequence[LCDInitSequence[i++]+1]=0;//put a zero in the last item, just to make sure our loop below will terminate

    while(LCDInitSequence[i])
    {
    	HALDisplaySendCommand(LCDInitSequence[i] & 0x0000FF);	//mask off the num commands bitfield
    	numDataCommands =  LCDInitSequence[i++] >> 8;
    	j = 0;
    	while(numDataCommands){
			HALDisplaySendCommand(LCDInitSequence[i + j++]);
			numDataCommands--;
		}
		i+=j;
    }
	
	i = LCD_X_SIZE * (LCD_Y_SIZE + 1);
	while(i){
#ifndef STMP_BUILD_PLAYER
		HALDisplaySendPixelData(g_BackgroundColor,0);	  //send background pixel data to the display
#else
		HALDisplaySendPixelData(RGB(0,0,0),0);	  //send black pixel data to the display
#endif
 		i--;
 	}
	
	//LCD_BLON_GPDO    = TRUE;		//turn on the BL in the end otherwise STMP will crash
	//turn on PWM for the backlight
	HW_PWM_CSR.I |= CLCD_PWM_ENABLE_MASK;

}
#ifdef FUNCLET
    #pragma asm
        ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
        org p,".ptexts6b33b0ainit":
    #pragma endasm
#endif

#ifndef STMP_BUILD_PLAYER
_reentrant void HALDisplaySendCommand(WORD wCommand)
{
	//LCD_RD_GPDO = TRUE;
	LCD_DC_GPDO = FALSE;
	LCD_CS_GPDO = FALSE;
	//_MultiNop();	//slight pause
	LCD_WR_GPDO = FALSE;
	//_MultiNop();	//slight pause
#ifdef CLCD_8BIT
	if (gTUNER_STFM1000) 
	{
        LCD_DATA_D00 = wCommand & 0x01;
        LCD_DATA_D03 = (wCommand & 0x08) ? 1 : 0;
        LCD_DATA_D04 = (wCommand & 0x10) ? 1 : 0;
    }
#endif

#ifdef CLCD_16BIT
	if (gTUNER_STFM1000) 
	{
        LCD_DATA_D00 = wCommand & 0x01;
        LCD_DATA_D03 = (wCommand & 0x08) ? 1 : 0;
        LCD_DATA_D04 = (wCommand & 0x10) ? 1 : 0;
    }
#endif

	LCD_DATA_DO	= wCommand;	
	//_MultiNop();	//10ns pause
#ifdef CLCD_16BIT
	_MultiNop();_MultiNop();_MultiNop();_MultiNop();
#endif //#ifdef CLCD_16BIT
	LCD_WR_GPDO = TRUE;
	LCD_CS_GPDO = TRUE;  
}

_reentrant void HALDisplaySendPixelData(WORD wData, WORD wDisplayOperation){
	
	if(wDisplayOperation & (DISPLAY_BUFFER | DISPLAY_TEXT)){	//this is used to control foreground color in realtime
		if((wData & 0x000FFF) == 0){	//is it black?
			wData = g_ForegroundColor;	
		}
		else if((wData & 0x000FFF) == 0xFFF){	//is it white?
			wData = g_BackgroundColor;
		}
	}			
	
	if(wDisplayOperation & DISPLAY_INVERT){
		wData = ~wData;	
	}	
	

	//LCD_RD_GPDO = TRUE;
	LCD_DC_GPDO = TRUE;
	LCD_CS_GPDO = FALSE;
	//_MultiNop();	//slight pause
	LCD_WR_GPDO = FALSE;
	//_MultiNop();	//slight pause
#ifdef CLCD_8BIT
    if (gTUNER_STFM1000) 
    {
        LCD_DATA_D00 = (wData & 0x0100) ? 1 : 0;
        LCD_DATA_D03 = (wData & 0x0800) ? 1 : 0;
        LCD_DATA_D04 = (wData & 0x1000) ? 1 : 0;
    }
	LCD_DATA_DO	= wData >> 8;	
	_MultiNop();	//10ns pause
	LCD_WR_GPDO = TRUE;
	_MultiNop();	//slight pause
	LCD_WR_GPDO = FALSE;
	_MultiNop();	//slight pause
    if (gTUNER_STFM1000) 
    {
        LCD_DATA_D00 = wData & 0x01;
        LCD_DATA_D03 = (wData & 0x08) ? 1 : 0;
        LCD_DATA_D04 = (wData & 0x10) ? 1 : 0;
    }
#endif

#ifdef CLCD_16BIT
    if (gTUNER_STFM1000) 
    {
        LCD_DATA_D00 = wData & 0x01;
        LCD_DATA_D03 = (wData & 0x08) ? 1 : 0;
        LCD_DATA_D04 = (wData & 0x10) ? 1 : 0;
    }
#endif

	LCD_DATA_DO	= wData;	
	//_MultiNop();	//10ns pause
	LCD_WR_GPDO = TRUE;
	LCD_CS_GPDO = TRUE;
}
#endif

