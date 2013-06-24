#include "types.h"
#include "messages.h"
#include "project.h"
#include "resource.h"
#include "sysresourceapi.h"
#include "displaydrv_color.h"
#include "..\displaydriver_color.h"
#include "s6b33b0a.h"

#pragma optimize 1


extern WORD g_ForegroundColor;
extern WORD g_BackgroundColor;
extern INT  gTUNER_STFM1000;

#ifdef CLCD_16BIT
extern INT  gi16BitOn;
extern BOOL g_blIsTB2Fm;
#endif //#ifdef CLCD_16BIT

#ifdef USBMSC_BUILD
extern _P WORD * _P _lc_u_p_usbmsc_lcd_battery_start;
extern _Y WORD * _Y _lc_u_y_Usbmsc_lcd_OVLY_copy_start;
extern _Y WORD * _Y _lc_u_y_Usbmsc_lcd_OVLY_copy_end;
extern _Y WORD * _Y _lc_u_y_s6b33b0a_65k_usbmsc_OVLY_copy_start;
extern _Y WORD * _Y _lc_u_y_s6b33b0a_65k_usbmsc_OVLY_copy_end;

extern _reentrant void CopyCodeIntoBootmanagerArea(_Y WORD * _Y pYCopyStart, 
                _P WORD * _P pPCopyStart, WORD wYCopySize);

_reentrant void HALDisplaySuspendIn_wrapper(void);
_reentrant void HALDisplaySuspendOut_wrapper(void);
#endif //#ifdef USBMSC_BUILD

WORD _reentrant DisplayGetContrast(void);

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
#else
	if (gTUNER_STFM1000 && !gi16BitOn)
#endif
	{
        LCD_DATA_D00 = wCommand & 0x01;
        LCD_DATA_D03 = (wCommand & 0x08) ? 1 : 0;
        LCD_DATA_D04 = (wCommand & 0x10) ? 1 : 0;
    }
	LCD_DATA_DO	= wCommand;	
	//_MultiNop();	//10ns pause
#ifdef CLCD_16BIT
	_MultiNop();_MultiNop();_MultiNop();_MultiNop();
#endif //#ifdef CLCD_16BIT
	LCD_WR_GPDO = TRUE;   
	LCD_CS_GPDO = TRUE;
}

_reentrant void HALDisplaySendData(WORD wXAddressControl, WORD wYAddressControl, WORD wWidth, WORD wHeight, WORD* pBuffer, WORD wDisplayOperation)
{
#ifdef SMV_Display_Driver	// video size should have been checked in smvmetadata
	int  i,j;
	WORD wSkip;
	WORD dataToSend;
    extern _X INT stc_iVideoWidth;

	if(wYAddressControl > LCD_Y_SIZE || wXAddressControl > LCD_X_SIZE){
		return;	//do not do anything in this case since it will be off screen.
	}

    if((wXAddressControl + wWidth) > stc_iVideoWidth)
    {   wSkip = wXAddressControl + wWidth - stc_iVideoWidth;
        wWidth -= wSkip;
    }else
    {   wSkip = 0;
    }
    
	HALDisplaySendCommand(0x42);
	HALDisplaySendCommand(wYAddressControl + LCD_START_ROW);
	HALDisplaySendCommand(wYAddressControl + wHeight + LCD_START_ROW - 1);	//this is included in area so subtract 1
	//setup horizontal area
	HALDisplaySendCommand(0x43);
	HALDisplaySendCommand(wXAddressControl + LCD_START_COLUMN);
	HALDisplaySendCommand(wXAddressControl + wWidth + LCD_START_COLUMN - 1);

    //LCD_RD_GPDO = TRUE;
    LCD_DC_GPDO = TRUE;
    LCD_CS_GPDO = FALSE;

#ifdef CLCD_16BIT
    if( g_blIsTB2Fm == FALSE )
    {
        for(i=0; i<wHeight; i++)
        {
            for(j=0; j<wWidth; j++)
            {
             	LCD_WR_GPDO = FALSE;
                LCD_DATA_DO	= *pBuffer++;
        	    LCD_WR_GPDO = TRUE;
            }
            if(wSkip > 0)
            {   pBuffer = (WORD *)((WORD)pBuffer + wSkip);
            }
    	}
    }
    else
    {
        for(i=0; i<wHeight; i++)
        {
            for(j=0; j<wWidth; j++)
            {
            	LCD_WR_GPDO = FALSE;
                
                dataToSend = *pBuffer++;

                LCD_DATA_D00 = dataToSend;
                LCD_DATA_D03 = 0;
                LCD_DATA_D04 = 0;

                if( dataToSend & 0x0008 )
                {
                    LCD_DATA_D03 = 1;
                }

                if( dataToSend & 0x0010 )
                {
                    LCD_DATA_D04 = 1;
                }

                LCD_DATA_DO	= dataToSend;
        	    LCD_WR_GPDO = TRUE;
            }
            if(wSkip > 0)
            {   pBuffer = (WORD *)((WORD)pBuffer + wSkip);
            }        
    	}
    }
#else
    for(i=0; i<wHeight; i++)
    {
        for(j=0; j<wWidth; j++)
        {
            dataToSend = *pBuffer;
    		pBuffer++;  //get ready for the next pixel
        	LCD_WR_GPDO = FALSE;
            if (gTUNER_STFM1000) 
            {
                LCD_DATA_D00 = (dataToSend & 0x0100) ? 1 : 0;
                LCD_DATA_D03 = (dataToSend & 0x0800) ? 1 : 0;
                LCD_DATA_D04 = (dataToSend & 0x1000) ? 1 : 0;
            }
            LCD_DATA_DO = dataToSend / 256;
    	    LCD_WR_GPDO = TRUE;
    	    _MultiNop();	//slight pause
    	    LCD_WR_GPDO = FALSE;
            if (gTUNER_STFM1000) 
            {
                LCD_DATA_D00 = (dataToSend & 0x0001) ? 1 : 0;
                LCD_DATA_D03 = (dataToSend & 0x0008) ? 1 : 0;
                LCD_DATA_D04 = (dataToSend & 0x0010) ? 1 : 0;
            }
            LCD_DATA_DO = dataToSend;
    	    LCD_WR_GPDO = TRUE;
        }
        if(wSkip > 0)
        {   pBuffer = (WORD *)((WORD)pBuffer + wSkip);
        }
 	
	}
#endif



 	LCD_CS_GPDO = TRUE;
#else
//the samsung s6b33b0a controller uses the X_address for vertical placement and the Y_Address as Horizontal placement.  The SAL uses
//x for horizontal and y for vertical, so where you would expect an X address below you will find a Y Address and vice-versa.
	int i,j;
	WORD dataToSend;
	WORD numWordsToSkip,bitMask;	//must initialize to zero for clipping to work
	WORD *dataPtr;
	
	if(wYAddressControl > LCD_Y_SIZE || wXAddressControl > LCD_X_SIZE){
		return;	//do not do anything in this case since it will be off screen.	
	}

	numWordsToSkip = 0;
	//need to check limits on x and y address to make sure clipping occurs.
	if((wYAddressControl + wHeight) > LCD_Y_SIZE){		//clip at the bottom?
		wHeight = LCD_Y_SIZE - wYAddressControl;	 
	}
	if((wXAddressControl + wWidth) > LCD_X_SIZE){	//should we clip on the right?
		dataToSend = wWidth;
		wWidth = LCD_X_SIZE - wXAddressControl;	//need to modify the width so we clip at the right 
		numWordsToSkip = dataToSend - wWidth;		//find the delta between old and new width
		//numWordsToSkip = 1+(dataToSend-1)/2;
		//if(dataToSend % 2 && wWidth % 2){	//was it originally an even width???
		//	numWordsToSkip--;
		//}	
	}

	//setup vertical area
	HALDisplaySendCommand(0x42);
	HALDisplaySendCommand(wYAddressControl + LCD_START_ROW);
	HALDisplaySendCommand(wYAddressControl + wHeight + LCD_START_ROW - 1);	//this is included in area so subtract 1
	//setup horizontal area
	HALDisplaySendCommand(0x43);
	HALDisplaySendCommand(wXAddressControl + LCD_START_COLUMN);
	HALDisplaySendCommand(wXAddressControl + wWidth + LCD_START_COLUMN - 1);

	if(wDisplayOperation & DISPLAY_COLOR){  //is the image color?
		for(i=0; i < wHeight;i++){
			for(j=0; j < wWidth; j++){	
				if(wDisplayOperation & DISPLAY_CLEAR){
					dataToSend = g_BackgroundColor;	//if clearing the display then don't read from the buffer
				}
				else{
					dataToSend = *pBuffer;
				    pBuffer++;  //get ready for the next pixel
                }
                HALDisplaySendPixelData(dataToSend,wDisplayOperation);  //send the pixel information to the LCD controller
			}
			pBuffer += numWordsToSkip;	//this is for clipping of the right...just ignore the superfluous data.
		}
	}
	//need to add code to clip on the right hand side of the LCD
	else{
		for(i=0; i < wHeight;i++){
			for(j=0; j < wWidth; j++){
				if(j % 24 == 0){
					bitMask = 0x800000;	//we are going to the next word in the ROW so restore the bitMask	
					dataToSend = *pBuffer++;
				}	
				if(bitMask & dataToSend){	//is the pixel ON?
					HALDisplaySendPixelData(0x000000,wDisplayOperation);		
				}
				else{
					HALDisplaySendPixelData(0x00FFFF,wDisplayOperation);
				}
				bitMask = bitMask >> 1;	//shift the bitmask for the next bit
			}
		}

	}
#endif	
}

#ifndef SMV_Display_Driver	// smv display doesn't need following routines
_reentrant void HALDisplaySendPixelData(WORD wData, WORD wDisplayOperation){
	if(wDisplayOperation & (DISPLAY_BUFFER | DISPLAY_TEXT)){	//this is used to control foreground color in realtime
		if((wData & 0x00FFFF) == 0){	//is it black?
			wData = g_ForegroundColor;	
		}
		else if((wData & 0x00FFFF) == 0xFFFF){	//is it white?
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
        LCD_DATA_DO	= wData / 256;
    }else
    {
    	//LCD_DATA_DO	= wData >> 8;	
    	LCD_DATA_DO	= wData / 256;
    }

	//_MultiNop();	//10ns pause
	LCD_WR_GPDO = TRUE;
	_MultiNop();	//slight pause
	LCD_WR_GPDO = FALSE;
	//_MultiNop();	//slight pause

    if (gTUNER_STFM1000) 
    {
        LCD_DATA_D00 = wData & 0x01;
        LCD_DATA_D03 = (wData & 0x08) ? 1 : 0;
        LCD_DATA_D04 = (wData & 0x10) ? 1 : 0;
    }
	LCD_DATA_DO	= wData;	
	//_MultiNop();	//10ns pause
	LCD_WR_GPDO = TRUE;
#else //#ifdef CLCD_8BIT
    if (gTUNER_STFM1000 && !gi16BitOn)
    {
        LCD_DATA_D00 = wData & 0x01;
        LCD_DATA_D03 = (wData & 0x08) ? 1 : 0;
        LCD_DATA_D04 = (wData & 0x10) ? 1 : 0;
    }
	LCD_DATA_DO	= wData;	
	LCD_WR_GPDO = TRUE;
#endif //#ifdef CLCD_8BIT
    LCD_CS_GPDO = TRUE;
}
#endif  // smv display doesn't need following routines

#ifdef CLCD_16BIT
#ifndef IGNORE_LCD16BIT_CONTROL
_reentrant void HALDisplay16BitOn(void)
{
    LCD_DATA_D00_GPDOE  =  FALSE; 
    LCD_DATA_D00_DI     =  TRUE;
	
    LCD_DATA_D03_GPDOE  =  FALSE; 
    LCD_DATA_D03_DI     =  TRUE;
	
    LCD_DATA_D04_GPDOE  =  FALSE; 
    LCD_DATA_D04_DI     =  TRUE;

    CLCD_16BIT_SWITCH   =  CLCD_16BIT_SWITCH_16BIT_ON;

    LCD_DATA_DOE       &= 0xff0000;
    LCD_DATA_DIR       &= 0xff0000;
    LCD_DATA_DO        |= 0xffff;
    LCD_DATA_DOE       |= 0xffff;
    LCD_DATA_EN        |= 0xffff;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    LCD_DATA_PWR       |= 0xffff;
#endif

    gi16BitOn = 1;
}

_reentrant void HALDisplay16BitOff(void)
{
    CLCD_16BIT_SWITCH  =  CLCD_16BIT_SWITCH_16BIT_OFF;

    LCD_DATA_D00_DI    =  FALSE;       
    LCD_DATA_D00_GPDOE =  TRUE;

    LCD_DATA_D03_DI    =  FALSE;       
    LCD_DATA_D03_GPDOE =  TRUE;     

    LCD_DATA_D04_DI    =  FALSE;       
    LCD_DATA_D04_GPDOE =  TRUE; 

    LCD_DATA_DOE       = 0xffe6;
    gi16BitOn = 0;
}
#endif //#ifndef IGNORE_LCD16BIT_CONTROL
#endif //#ifdef CLCD_16BIT

_reentrant void HALDisplaySetContrast(INT  iContrast)
{
    //the incoming contrast will saturate against the lower and upper bounds

    if(iContrast < 0)
        g_iContrastPercentage = 0;	//in percent not actual value sent to controller
    else if(iContrast > 100)
        g_iContrastPercentage = 100;
    else
        g_iContrastPercentage = iContrast;

    //contrast is an 'extended' command...takes two command words to execute
    HALDisplaySendCommand(CMD_SET_CONTRAST);
    //HALDisplaySendCommand(iContrast);
    HALDisplaySendCommand(LCD_MIN_CONTRAST+ ((g_iContrastPercentage*(LCD_MAX_CONTRAST-LCD_MIN_CONTRAST))/100));

}

_reentrant void HALDisplayDecContrast(void)
{
#ifdef CONTRAST_CALIBRATION    
    HALDisplaySetContrast(g_iContrastPercentage-LCD_STEPSIZE_CONTRAST);
#else
    HALDisplaySetContrast(DisplayGetContrast()-LCD_STEPSIZE_CONTRAST);
#endif
}

_reentrant void HALDisplayIncContrast(void)
{
#ifdef CONTRAST_CALIBRATION    
    HALDisplaySetContrast(g_iContrastPercentage+LCD_STEPSIZE_CONTRAST);
#else
    HALDisplaySetContrast(DisplayGetContrast()+LCD_STEPSIZE_CONTRAST);
#endif
}

_reentrant void HALDisplayBacklightOn(void)
{
	PWMA = PWM_SETUPA;
}

_reentrant void HALDisplayBacklightOff(void)
{
	PWMA = PWM_SETUPAOFF;
}

_reentrant void HALHandleMessage(Message *pMsg)
{
    switch(pMsg->m_wMessage)
    {
    case LCD_SET_CONTRAST:
        HALDisplaySetContrast(pMsg->m_wParams[0]);
        break;
    case LCD_INC_CONTRAST:
        HALDisplayIncContrast();
        break;
    case LCD_DEC_CONTRAST:
        HALDisplayDecContrast();
        break;
	case LCD_BACKLIGHT_ON:
		HALDisplayBacklightOn();
		break;
	case LCD_BACKLIGHT_OFF:
		HALDisplayBacklightOff();
		break;
#ifdef CLCD_16BIT
#ifndef IGNORE_LCD16BIT_CONTROL
    case LCD_16BIT_ON:
        HALDisplay16BitOn();
        break;        		
    case LCD_16BIT_OFF:
        HALDisplay16BitOff();
        break;
#endif //#ifndef IGNORE_LCD16BIT_CONTROL                		
#endif //#ifdef CLCD_16BIT
    default:
        break;
    }
}

#ifndef PLAYER_BUILD // not exist in PLAYER build

    #ifdef USBMSC_BUILD // only for USBMSC build

_reentrant void HALDisplaySuspendIn(void)
{
    unsigned int CopySize = (unsigned int)(&_lc_u_y_s6b33b0a_65k_usbmsc_OVLY_copy_end-&_lc_u_y_s6b33b0a_65k_usbmsc_OVLY_copy_start);
    
    CopyCodeIntoBootmanagerArea((_Y WORD *)&_lc_u_y_s6b33b0a_65k_usbmsc_OVLY_copy_start, 
                                (_P WORD * _P)&_lc_u_p_usbmsc_lcd_battery_start, CopySize);
  
    HALDisplaySuspendIn_wrapper();
  
    CopySize = (unsigned int)(&_lc_u_y_Usbmsc_lcd_OVLY_copy_end-&_lc_u_y_Usbmsc_lcd_OVLY_copy_start);
        
    CopyCodeIntoBootmanagerArea((_Y WORD *)&_lc_u_y_Usbmsc_lcd_OVLY_copy_start, 
                                (_P WORD * _P)&_lc_u_p_usbmsc_lcd_battery_start, CopySize);
    
}    

_reentrant void HALDisplaySuspendOut(void)
{
    unsigned int CopySize = (unsigned int)(&_lc_u_y_s6b33b0a_65k_usbmsc_OVLY_copy_end-&_lc_u_y_s6b33b0a_65k_usbmsc_OVLY_copy_start);
    
    CopyCodeIntoBootmanagerArea((_Y WORD *)&_lc_u_y_s6b33b0a_65k_usbmsc_OVLY_copy_start, 
                                (_P WORD * _P)&_lc_u_p_usbmsc_lcd_battery_start, CopySize);
  
    HALDisplaySuspendOut_wrapper();
  
    CopySize = (unsigned int)(&_lc_u_y_Usbmsc_lcd_OVLY_copy_end-&_lc_u_y_Usbmsc_lcd_OVLY_copy_start);
        
    CopyCodeIntoBootmanagerArea((_Y WORD *)&_lc_u_y_Usbmsc_lcd_OVLY_copy_start, 
                                (_P WORD * _P)&_lc_u_p_usbmsc_lcd_battery_start, CopySize);
    
}

    #pragma asm
        ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
            org p,".ptexts6b33b0a_65k_usbmsc_overlay":
    #pragma endasm

_reentrant void HALDisplaySuspendIn_wrapper(void)
    #else   // MTP_BUILD (!USBMSC_BUILD && !PLAYER_BUILD)
_reentrant void HALDisplaySuspendIn(void)
    #endif
{
    HW_PWM_CSR.I = 0; //MASTER ENABLE to Start clocks

    LCD_BLON_GPDOE  = FALSE;
    LCD_BLON_GPDO   = FALSE;
    LCD_BLON_GPDI   = TRUE;
    LCD_BLON_GPEN   = TRUE;
    LCD_BLON_GPPWR  = FALSE;
    
    HALDisplaySendCommand(0x50);    //Display off
#ifdef CLCD_8BIT
    HALDisplaySendCommand(0x2D);    //Standby mode on   note: s6b33b0a could not wake up from standby mode if 16bit by experiment
#endif
    LCD_DC_GPDOE    = FALSE;
    LCD_DC_GPDO     = FALSE;
    LCD_DC_GPDI     = TRUE;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    LCD_DC_GPPWR    = FALSE;
#endif

    LCD_CS_GPDOE    = FALSE;
    LCD_CS_GPDO     = FALSE;
    LCD_CS_GPDI     = TRUE;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    LCD_CS_GPPWR    = FALSE;
#endif

    LCD_RD_GPDOE    = FALSE;
    LCD_RD_GPDO     = FALSE;
    LCD_RD_GPDI     = TRUE;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    LCD_RD_GPPWR    = FALSE;
#endif

    LCD_WR_GPDOE    = FALSE;
    LCD_WR_GPDO     = FALSE;
    LCD_WR_GPDI     = TRUE;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    LCD_WR_GPPWR    = FALSE;
#endif

#ifdef CLCD_8BIT
	if (gTUNER_STFM1000)
	{   LCD_DATA_D00_GPDOE  =  FALSE; 
	    LCD_DATA_D00        =  FALSE;       
	    LCD_DATA_D00_DI     =  TRUE;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
	    LCD_DATA_D00_GPPWR  =  FALSE;    
#endif
	
	    LCD_DATA_D03_GPDOE  =  FALSE; 
	    LCD_DATA_D03        =  FALSE;       
	    LCD_DATA_D03_DI     =  TRUE;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
	    LCD_DATA_D03_GPPWR  =  FALSE;    
#endif
	
	    LCD_DATA_D04_GPDOE  =  FALSE; 
	    LCD_DATA_D04        =  FALSE;       
	    LCD_DATA_D04_DI     =  TRUE;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
	    LCD_DATA_D04_GPPWR  =  FALSE;    
#endif
	
	    LCD_DATA_DOE   &= ~(0xe6);  // 1110 0110 except bits 0, 3, 4
	  	LCD_DATA_DO    &= ~(0xe6);  // 1110 0110 except bits 0, 3, 4  
	  	LCD_DATA_DI    |=   0xe6;   // 1110 0110 except bits 0, 3, 4  
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
	    LCD_DATA_PWR   &= ~(0xe6);
#endif
	}	 
	else // non STFM1000 layout
    {   LCD_DATA_DOE   &= ~(0xff);
	    LCD_DATA_DO    &= ~(0xff);
	    LCD_DATA_DI    |=   0xff;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    	LCD_DATA_PWR   &= ~(0xff);
#endif
     }
#else //#ifdef CLCD_8BIT
    LCD_DATA_DOE   &= ~(0xffff);
    LCD_DATA_DO    &= ~(0xffff);
    LCD_DATA_DI    |=   0xffff;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    LCD_DATA_PWR   &= ~(0xffff);
#endif
    CLCD_16BIT_SWITCH_GPDOE = FALSE;
    CLCD_16BIT_SWITCH       = FALSE;
    CLCD_16BIT_SWITCH_GPDI  = TRUE;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    CLCD_16BIT_SWITCH_GPPWR = FALSE;
#endif
#endif //#ifdef CLCD_8BIT
}



    #ifdef USBMSC_BUILD // only for USBMSC build
_reentrant void HALDisplaySuspendOut_wrapper(void)
    #else
_reentrant void HALDisplaySuspendOut(void)
    #endif
{
    LCD_DC_GPDI     = FALSE;
    LCD_DC_GPDO     = TRUE;
    LCD_DC_GPDOE    = TRUE;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    LCD_DC_GPPWR    = TRUE;
#endif

    LCD_CS_GPDI     = FALSE;
    LCD_CS_GPDO     = FALSE;
    LCD_CS_GPDOE    = TRUE;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    LCD_CS_GPPWR    = TRUE;
#endif

    LCD_RD_GPDI     = FALSE;
    LCD_RD_GPDO     = TRUE;
    LCD_RD_GPDOE    = TRUE;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    LCD_RD_GPPWR    = TRUE;
#endif

    LCD_WR_GPDI     = FALSE;
    LCD_WR_GPDO     = TRUE;
    LCD_WR_GPDOE    = TRUE;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    LCD_WR_GPPWR    = TRUE;
#endif

#ifdef CLCD_8BIT
	if (gTUNER_STFM1000)
	{   LCD_DATA_D00_DI     =  FALSE;
	    LCD_DATA_D00        =  TRUE;       
	    LCD_DATA_D00_GPDOE  =  TRUE; 
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
	    LCD_DATA_D00_GPPWR  =  TRUE;    
#endif
	
	    LCD_DATA_D03_DI     =  FALSE;
	    LCD_DATA_D03        =  TRUE;       
	    LCD_DATA_D03_GPDOE  =  TRUE; 
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
	    LCD_DATA_D03_GPPWR  =  TRUE;    
#endif
	
	    LCD_DATA_D04_DI     =  FALSE;
	    LCD_DATA_D04        =  TRUE;       
	    LCD_DATA_D04_GPDOE  =  TRUE; 
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
	    LCD_DATA_D04_GPPWR  =  TRUE;    
#endif
	
	  	LCD_DATA_DI    &= ~(0xe6);  // 1110 0110 except bits 0, 3, 4  
	  	LCD_DATA_DO    |=   0xe6;   // 1110 0110 except bits 0, 3, 4  
	    LCD_DATA_DOE   |=   0xe6;   // 1110 0110 except bits 0, 3, 4
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
	    LCD_DATA_PWR   |=   0xe6;
#endif
	}	 
	else // non STFM1000 layout
    {   LCD_DATA_DI    &= ~(0xff);
	    LCD_DATA_DO    |=   0xff;
	    LCD_DATA_DOE   |=   0xff;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    	LCD_DATA_PWR   |=   0xff;
#endif
     }
#else //#ifdef CLCD_8BIT
    //******* 16-bit interface is already turned on before.
    LCD_DATA_DI    &= ~(0xffff);
    LCD_DATA_DO    |=   0xffff;
    LCD_DATA_DOE   |=   0xffff;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    LCD_DATA_PWR   |=   0xffff;
#endif

    CLCD_16BIT_SWITCH_GPDI  =  FALSE;
    CLCD_16BIT_SWITCH       =  CLCD_16BIT_SWITCH_16BIT_ON;
    CLCD_16BIT_SWITCH_GPDOE =  TRUE; 	    
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
	CLCD_16BIT_SWITCH_GPPWR =  TRUE; 	
#endif
#endif //#ifdef CLCD_8BIT

#ifdef CLCD_8BIT
    HALDisplaySendCommand(0x2C);    //Standby mode off  note: s6b33b0a could not wake up from standby mode if 16bit by experiment
#endif
    HALDisplaySendCommand(0x51);    //Display on

    LCD_BLON_GPDOE  = FALSE;
    LCD_BLON_GPDO   = FALSE;
    LCD_BLON_GPDI   = FALSE;
    LCD_BLON_GPEN   = FALSE;
    LCD_BLON_GPPWR  = TRUE;

    HW_PWM_CSR.I = PWM_ENABLE_MASK; //MASTER ENABLE to Start clocks
}

    #ifdef USBMSC_BUILD // only for USBMSC build
    #pragma asm
        ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
            org p,".ptexts6b33b0a_65k":
    #pragma endasm
    #endif //#ifdef USBMSC_BUILD

#endif //#ifndef PLAYER_BUILD

//_reentrant WORD HALDisplayReadStatus(void)
//{
//    
//	WORD wStatus;
//	LCD_WR_GPDO = TRUE;
//	LCD_DATA_DOE = 0;
//	
//	LCD_RD_GPDO = FALSE;
//	LCD_DC_GPDO = FALSE;
//	LCD_CS_GPDO = FALSE;
//	_MultiNop();	//slight pause
//	_MultiNop();	//slight pause
//	_MultiNop();	//slight pause
//	_MultiNop();	//slight pause
//	_MultiNop();	//slight pause
//	wStatus = LCD_DATA_DIR;
//	LCD_RD_GPDO = TRUE;
//
//	LCD_DATA_DOE = 0xff;	
//   	return wStatus;
//}