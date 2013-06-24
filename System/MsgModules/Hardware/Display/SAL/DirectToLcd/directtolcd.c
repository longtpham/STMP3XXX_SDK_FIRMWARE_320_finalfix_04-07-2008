#include "types.h"
//#include "hwequ.h"  
#include "messages.h"
#include "project.h"
#include "resource.h"
#include "displaydrv.h"
#include "sysmem.h"
#include "sysequ.h"
#include "sysresources.h"
//#include "sysresourceapi.h"
#include "displaydriver.h"
//#include "exec.h"

//#include "..\..\displaymodule.h"

extern INT g_iMaskLeft;
extern INT g_iMaskRight;
extern INT g_iMaskTop;
extern INT g_iMaskBottom;

_reentrant void PushMask(INT left, INT top, INT right, INT bottom);
_reentrant void PopMask(void);


_reentrant void SALDisplayInit(void)
{//Nothing to do here
}

// Does not match prototype. Stmp00003899
//_reentrant void SALHandleMessage(struct CMessage  *pMsg)
_reentrant void SALHandleMessage(Message  *pMsg)
{

    //switch(pMsg->m_uMsg[0])
	switch(pMsg->m_wMessage)
    {
    //Nothing to do here
    default://Any unhandled messages are sent to the HAL
        HALHandleMessage(pMsg);
        break;
    }
}

_reentrant void SALDisplayUpdate(void)
{//Nothing to do here
}




_reentrant void SALDisplayPrintBitmapAddress   (INT x,INT y,void _Y *pAddress ,INT bInvert)
{
//    INT i;
    INT iDestinationY;
    INT iDestinationX;
//    INT iSourceY;
//    INT iSourceX;
    INT iSourceWidth;
    INT iSourceHeight;
    INT iFarRight;
    INT iFarBottom;
    INT iSourceStartingByte=0;
    INT iWidth;
    INT iHeight;
    INT iStartPage;
    iSourceWidth  = ((WORD*)pAddress)[0];
    iSourceHeight = ((WORD*)pAddress)[1];

    y-=y%8;//lets move it up to be byte aligned, if it isn't already.

    iFarRight  = x+iSourceWidth-1;
    iFarBottom = y+iSourceHeight-1;

    PushMask(x,y,x+iSourceWidth,y+iSourceHeight);

    if(iFarRight>= g_iMaskLeft && x< g_iMaskRight)
    {
        if(iFarBottom >=g_iMaskTop && y < g_iMaskBottom)
        {//at least part of us must be in the mask region
            if(y<g_iMaskTop)
            {
                iDestinationY = g_iMaskTop;
                iSourceStartingByte += (((g_iMaskTop-y)/8)*iSourceWidth);
            }
            else
            {
                iDestinationY = y;
            }

            if(iFarBottom>g_iMaskBottom)
                iFarBottom = g_iMaskBottom;

            if(x<g_iMaskLeft)
            {
                iDestinationX = g_iMaskLeft;
                iSourceStartingByte += (g_iMaskLeft - x);
            }
            else
            {
                iDestinationX = x;
            }


            if(iFarRight>g_iMaskRight)
                iFarRight = g_iMaskRight;
           
            iStartPage = iDestinationY/8;
            iHeight = ((iFarBottom+8)/8) - iStartPage;
            iWidth  = 1+iFarRight-iDestinationX;
            for(y=iStartPage;y<iStartPage+iHeight;y++)
            {
                HALDisplaySendData(y,iDestinationX,iWidth,iSourceStartingByte,((WORD*)pAddress)+2,bInvert);
                iSourceStartingByte = iSourceStartingByte+iSourceWidth;
            }

        }
    }
    PopMask();
}


_reentrant void SALDisplayClearRange(INT x,INT y,WORD iSourceWidth,WORD iSourceHeight)
{

    g_wUserScratchY[0]=iSourceWidth;
    g_wUserScratchY[1]=iSourceHeight;

    SALDisplayPrintBitmapAddress(x,y,&g_wUserScratchY,DISPLAY_CLEAR);
}

_reentrant INT SALDisplayGetWidth(void)
{
    return LCD_X_SIZE;
}
_reentrant INT SALDisplayGetHeight(void)
{
    return LCD_Y_SIZE;
}
