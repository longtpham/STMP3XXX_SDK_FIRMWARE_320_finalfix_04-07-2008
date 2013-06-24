#include "types.h"
#include "messages.h"
#include "project.h"
#include "resource.h"
#include "displaydrv.h"
#include "sysresource.h"
#include "displaydriver.h"

#ifdef STMP_BUILD_PLAYER
#include "sysmem.h"
#include "sysequ.h"
#endif


#pragma optimize 1

WORD g_bWorkingOnFrame=FALSE;

_reentrant void SALDisplayCopyBitmapToBitmap(INT xDestination, INT yDestination, BITMAP *pSource, INT bInvert, BITMAP *pDestination);

//////////////////////////////////////////////////////////////////////////////
//These two must follow each other!                                         //
BITMAPHEADER  RootBufferHeader={LCD_X_SIZE,LCD_Y_SIZE};                //
unsigned int g_wFrameBuffer[LCD_X_SIZE*(1+(LCD_Y_SIZE-1)/24)]={0x0};          //
//////////////////////////////////////////////////////////////////////////////

BITMAP *  g_pCurrentBuffer = (BITMAP * )&RootBufferHeader;
BITMAP *  g_pRootBuffer    = (BITMAP * )&RootBufferHeader;

_reentrant void SALDisplayInit(void)
{
}

_reentrant void SALHandleMessage(Message  *pMsg)
{

    switch(pMsg->m_wMessage)
    {
    case LCD_BEGIN_FRAME:
        g_bWorkingOnFrame = TRUE;
        break;
    case LCD_END_FRAME:
        g_bWorkingOnFrame = FALSE;
        break;
    case LCD_SET_FRAMEBUFFER:
        if(pMsg->m_wParams[0])
            g_pCurrentBuffer = (BITMAP *)pMsg->m_wParams[0];
        else
            g_pCurrentBuffer = (BITMAP *)&RootBufferHeader;
        break;
    default://Any unhandled messages are sent to the HAL
        HALHandleMessage(pMsg);
        break;
    }
}

_reentrant void SALDisplayUpdate(void)
{
    if(!g_bWorkingOnFrame)
    {   //this can be optimized with dirty bits
        HALDisplaySendData(0,0,LCD_X_SIZE,LCD_X_SIZE*0,g_pRootBuffer->m_wBitmap,DISPLAY_NORMAL);
        HALDisplaySendData(1,0,LCD_X_SIZE,LCD_X_SIZE*1,g_pRootBuffer->m_wBitmap,DISPLAY_NORMAL);
        HALDisplaySendData(2,0,LCD_X_SIZE,LCD_X_SIZE*2,g_pRootBuffer->m_wBitmap,DISPLAY_NORMAL);
        HALDisplaySendData(3,0,LCD_X_SIZE,LCD_X_SIZE*3,g_pRootBuffer->m_wBitmap,DISPLAY_NORMAL);
        HALDisplaySendData(4,0,LCD_X_SIZE,LCD_X_SIZE*4,g_pRootBuffer->m_wBitmap,DISPLAY_NORMAL);
        HALDisplaySendData(5,0,LCD_X_SIZE,LCD_X_SIZE*5,g_pRootBuffer->m_wBitmap,DISPLAY_NORMAL);
        HALDisplaySendData(6,0,LCD_X_SIZE,LCD_X_SIZE*6,g_pRootBuffer->m_wBitmap,DISPLAY_NORMAL);
        HALDisplaySendData(7,0,LCD_X_SIZE,LCD_X_SIZE*7,g_pRootBuffer->m_wBitmap,DISPLAY_NORMAL);
    }
}


_reentrant void SALDisplayPrintBitmapAddress   (INT x,INT y,void _Y *pAddress ,INT bInvert)
{
    SALDisplayCopyBitmapToBitmap(x,y,(void *)pAddress,bInvert, g_pCurrentBuffer);
}


_reentrant INT SALDisplayGetWidth(void)
{
    return ((WORD*)g_pCurrentBuffer)[0];
}
_reentrant INT SALDisplayGetHeight(void)
{
    return ((WORD*)g_pCurrentBuffer)[1];
}



_reentrant void SALDisplayClearRange(INT x,INT y,WORD iSourceWidth,WORD iSourceHeight)
{
    WORD FakeBitmap[2];
    FakeBitmap[0]=iSourceWidth;
    FakeBitmap[1]=iSourceHeight;

    SALDisplayPrintBitmapAddress(x,y,FakeBitmap,2);
}
