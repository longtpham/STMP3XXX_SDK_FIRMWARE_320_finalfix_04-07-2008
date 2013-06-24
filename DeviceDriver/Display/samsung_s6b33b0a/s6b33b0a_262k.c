////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2004-2006
//
// File        : s6b33b0a_262k.c 
// Description : LCD module driver
////////////////////////////////////////////////////////////////////////////////
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

WORD _reentrant DisplayGetContrast(void);


_reentrant void HALDisplaySendCommand(WORD wCommand)
{
	LCD_RD_GPDO = TRUE;
	LCD_DC_GPDO = FALSE;
	LCD_CS_GPDO = FALSE;
	_MultiNop();	//slight pause
	LCD_WR_GPDO = FALSE;
	_MultiNop();	//slight pause

	if (gTUNER_STFM1000) 
	{
//#ifdef TUNER_STFM1000
        LCD_DATA_D00 = wCommand & 1;
        LCD_DATA_D03 = (wCommand>>3) & 1;
        LCD_DATA_D04 = (wCommand>>4) & 1;
//#endif 
    }
	LCD_DATA_DO	= wCommand;	
	_MultiNop();	//10ns pause
	LCD_WR_GPDO = TRUE;   
}

_reentrant void HALDisplaySendData(WORD wXAddressControl, WORD wYAddressControl, WORD wWidth, WORD wHeight, WORD* pBuffer, WORD wDisplayOperation)
{
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
					HALDisplaySendPixelData(0x03FFFF,wDisplayOperation);
				}
				bitMask = bitMask >> 1;	//shift the bitmask for the next bit
			}
		}

	}
}

_reentrant void HALDisplaySendPixelData(WORD wData, WORD wDisplayOperation){
	
	if(wDisplayOperation & (DISPLAY_BUFFER | DISPLAY_TEXT)){	//this is used to control foreground color in realtime
		if((wData & 0x03FFFF) == 0){	//is it black?
			wData = g_ForegroundColor;	
		}
		else if((wData & 0x03FFFF) == 0x03FFFF){	//is it white?
			wData = g_BackgroundColor;
		}
	}			
	
	if(wDisplayOperation & DISPLAY_INVERT){
		wData = ~wData;	
	}	
	
    wData = ((wData & (WORD)0x000FFF) >> 1) | ((wData >> 2) & 0x00F800);

	LCD_RD_GPDO = TRUE;
	LCD_DC_GPDO = TRUE;
	LCD_CS_GPDO = FALSE;
	_MultiNop();	//slight pause
	LCD_WR_GPDO = FALSE;
	_MultiNop();	//slight pause

    if (gTUNER_STFM1000) 
    {
//#ifdef TUNER_STFM1000
	    LCD_DATA_D00 = (wData>>(8+0)) & 1;  
	    LCD_DATA_D03 = (wData>>(8+3)) & 1;
	    LCD_DATA_D04 = (wData>>(8+4)) & 1;
//#endif   
    }
	LCD_DATA_DO	= wData >> 8;	
	_MultiNop();	//10ns pause
	LCD_WR_GPDO = TRUE;
	_MultiNop();	//slight pause
	LCD_WR_GPDO = FALSE;
	_MultiNop();	//slight pause

    if (gTUNER_STFM1000) 
    {
//#ifdef TUNER_STFM1000
	    LCD_DATA_D00 = wData & 1;  
	    LCD_DATA_D03 = (wData>>3) & 1;
	    LCD_DATA_D04 = (wData>>4) & 1;
//#endif   
    }
	LCD_DATA_DO	= wData;	
	_MultiNop();	//10ns pause
	LCD_WR_GPDO = TRUE;
}

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
    default:
        break;
    }
}


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