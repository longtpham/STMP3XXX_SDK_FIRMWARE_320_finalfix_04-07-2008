#include "types.h"
#include "messages.h"
#include "project.h"
#include "resource.h"
#include "kernel.h"
#include "system.h"
#include "displaydrv_color.h"
#include "sysresource.h"
#include "displaydriver_color.h"
#include "usbmsc_colorlcdsal.h"

//These Two Globals are used to control the color of text on screen.  Change them via the LCD_SET_COLOR message.
int g_ForegroundColor = RGB(0xFF,0x0,0x0);	//make the fonts red by default
int g_BackgroundColor = RGB(0xFF,0xFF,0xFF);	//background is white by default  

_reentrant void SALDisplayUpdate(void)
{
	//do nothing
}


_reentrant void SALDisplayInit(void)
{
	//do nothing
}



/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant void SALHandleMessage(Message  *pMsg)
//
//   Type:          Function 
//
//   Description:   Handles messages passed from the Display Module
//
//   Inputs:        pMsg --message passed from the Display Module                
//
//  Notes:          Avoid placing message handlers which are used often here.  Since this is
//                  funcletized frequent messages handled here will cause latency.
//<
//////////////////////////////////////////////////////////////////////////////////////////
// Does not match prototype. Stmp00003899
//_reentrant void SALHandleMessage(struct CMessage  *pMsg)
_reentrant void SALHandleMessage(Message  *pMsg)
{
	INT Param1,Param2;
	
	Param1 = pMsg->m_wParams[0];
	Param2 = pMsg->m_wParams[1];

    //switch(pMsg->m_uMsg[0])
	switch(pMsg->m_wMessage)
    {
    case LCD_SET_COLOR:
		g_ForegroundColor = Param1;
		g_BackgroundColor = Param2;
		break;
    default://Any unhandled messages are sent to the HAL
        HALHandleMessage(pMsg);
        break;
    }
}

_reentrant void SALDisplayPrintBitmapAddress   (INT x,INT y,void _Y *pAddress ,INT bInvert)
{	//need to modify to support the new resource type.
	int iSourceWidth,iSourceHeight,iSourceColorInfo;
	WORD* pBuffer;
	pBuffer = (WORD*) pAddress;
	iSourceColorInfo = pBuffer[0];
	iSourceWidth = pBuffer[1];
	iSourceHeight = pBuffer[2];

	if(iSourceColorInfo != MONOCHROME_BITMAP_RESOURCE){	//check to see if it is monochrome or color
		bInvert |= DISPLAY_COLOR;
	}
	else{
		bInvert |= DISPLAY_MONOCHROME;
	}

	HALDisplaySendData(x,y, iSourceWidth, iSourceHeight, &pBuffer[3], bInvert);
}


_reentrant void SALDisplayClearRange(INT x,INT y,WORD iSourceWidth,WORD iSourceHeight)
{
	HALDisplaySendData(x,y, iSourceWidth, iSourceHeight, 0, DISPLAY_CLEAR | DISPLAY_COLOR);	
}

_reentrant INT SALDisplayGetWidth(void)
{
    return LCD_X_SIZE;
}
_reentrant INT SALDisplayGetHeight(void)
{
    return LCD_Y_SIZE;
}