////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename: lcdsupport.c
// Description: LCD Support functions
////////////////////////////////////////////////////////////////////////////////

#include "lcdsupport.h"
#include "messages.h"


///////////////////////////////////////////////////////////////////////////////
//
//>  Name:           RetrieveLcdMessage
//
//   Type:           Function
//
//   Description:    Loads the LCD module to process all messages in its queue
//                   
//
//   Inputs:         none
//
//   Outputs:        none
//                   
//   Notes:          none
//<
///////////////////////////////////////////////////////////////////////////////
void _reentrant RetrieveLcdMessage(void)
{
    #pragma asm
        nolist
        extern  FDisplayQueueDescriptor
        extern  FDisplayProcessQueue
        list
        
        move    #FDisplayQueueDescriptor,r0
        move    #-1,m0
        jsr     FDisplayProcessQueue
    #pragma endasm
}    


///////////////////////////////////////////////////////////////////////////////
//
//>  Name:           Delay
//
//   Type:           Function
//
//   Description:    Waits 
//                   
//
//   Inputs:         none
//
//   Outputs:        none
//                   
//   Notes:          none
//<
///////////////////////////////////////////////////////////////////////////////
void _reentrant Delay(WORD wDelay)
{
    WORD wTemp = 100;
    do
    {

        do
        {
            wTemp--;
        }while(wTemp);

        wTemp = 100;     
        wDelay--;
    }while(wDelay);    
}    




///////////////////////////////////////////////////////////////////////////////
//
//>  Name:           SwizzleText
//
//   Type:           Function
//
//   Description:    Swap byte 2 and byte 0 of string words pointed by arg pText  
//                   
//
//   Inputs:         pText = Pointer to the string to swizzle
//
//   Outputs:        none
//                   
//   Notes:          none
//<
///////////////////////////////////////////////////////////////////////////////

void _reentrant SwizzleText(_packed BYTE *pText)
{
    int bDone=FALSE;
    unsigned int *pTmp=(unsigned int*)pText;
    unsigned int tmp;
    while(!bDone)
    {
        tmp = *pTmp;
        tmp = ((tmp & 0x0000ff)<<16) | ((tmp & 0x00ff00)) | ((tmp & 0xff0000)>>16);
        *pTmp++=tmp;
        if(!(tmp&0x0000ff) || !(tmp&0x00ff00)  || !(tmp&0xff0000))
            bDone = TRUE;
    }
}


    
///////////////////////////////////////////////////////////////////////////
// Function calls for all the Messaging Module API
//
// Used to transfer data to the system through SysPostMsg 
///////////////////////////////////////////////////////////////////////////


void _reentrant LcdClear(WORD wPosX, WORD wPosY, WORD wLength, WORD wHeight)
{
	SysPostMessage(6,LCD_CLEAR_RANGE,wPosX,wPosY,wLength,wHeight);
    RetrieveLcdMessage();
}


void _reentrant SetContrast(WORD wContrastVal)
{
	SysPostMessage(3,LCD_SET_CONTRAST,wContrastVal);
    RetrieveLcdMessage();
}

 
void _reentrant IncContrast(void)
{
	SysPostMessage(2, LCD_INC_CONTRAST);
    RetrieveLcdMessage();
}


void _reentrant DecContrast(void)
{
	SysPostMessage(2, LCD_DEC_CONTRAST);
    RetrieveLcdMessage();
}

    
void _reentrant PrintStringRsrc(WORD wPosX, WORD wPosY, WORD wRsrc)
{
	SysPostMessage(5,LCD_PRINT_STRING_RSRC,wPosX,wPosY,wRsrc);
	RetrieveLcdMessage();
}


void _reentrant PrintStringAddr(WORD wPosX, WORD wPosY, _packed BYTE *pMsgString)
{
	SysPostMessage(5,LCD_PRINT_STRING_ADDR,wPosX,wPosY,pMsgString);
	RetrieveLcdMessage();
}


void _reentrant PrintRangeRsrc(WORD wPosX, WORD wPosY, WORD wRsrc)
{
	SysPostMessage(5, LCD_PRINT_RANGE_RSRC, wPosX, wPosY, wRsrc);
	RetrieveLcdMessage();
}


void _reentrant PrintRangeInvRsrc(WORD wPosX, WORD wPosY, WORD wRsrc)
{
	SysPostMessage(5, LCD_PRINT_RANGE_INV_RSRC, wPosX, wPosY, wRsrc);
	RetrieveLcdMessage();
}


void _reentrant PrintRangeAddr(WORD wPosX, WORD wPosY, WORD *pData)
{
	SysPostMessage(5, LCD_PRINT_RANGE_ADDR, wPosX, wPosY, pData);
	RetrieveLcdMessage();
}


void _reentrant PrintRangeInvAddr(WORD wPosX, WORD wPosY, WORD *pData)
{
	SysPostMessage(5, LCD_PRINT_RANGE_INV_ADDR, wPosX, wPosY, pData);
	RetrieveLcdMessage();
}


void _reentrant ScrollDisplay(INT wLRdist, INT wUDdist)
{
	SysPostMessage(4,LCD_SCROLL_DISPLAY,wLRdist,wUDdist);
	RetrieveLcdMessage();
}


void _reentrant SetFont(WORD wFontIndex)
{
	SysPostMessage(3,LCD_SET_FONT,wFontIndex);
	RetrieveLcdMessage();
}


void _reentrant PrintNumber(WORD wMsgXPos, WORD wMsgYPos, WORD wNum, WORD wWidth, WORD wLeadChar)
{
	SysPostMessage(7,LCD_PRINT_NUMBER,wMsgXPos,wMsgYPos,wNum,wWidth,wLeadChar);
	RetrieveLcdMessage();
}


void _reentrant PrintTime(WORD wMsgXPos, WORD wMsgYPos, WORD wMinutes, WORD wSeconds)
{
	SysPostMessage(6,LCD_PRINT_TIME,wMsgXPos,wMsgYPos,wMinutes,wSeconds);
	RetrieveLcdMessage();
}

