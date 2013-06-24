#include "types.h"

#include "messages.h"
#include "project.h"
#include "stringlib.h"
#include "sysmem.h"
#include "sysequ.h"
#include "sysresourceapi.h"
#include "exec.h"
#include "displaymodule.h"

#ifdef SPECTRUM_ANAL 
#include "spectrogram.h"
#include "display.h"
#endif



#pragma optimize 1

#include "displaydriver.h"
#include "unicode.h"
#include "dbcs.h"

#define MAX_MASKS 5
#define MAX_DIGITS 10

//#include "display.h"

struct ModuleEntry *g_pDisplayModuleEntry = NULL;


_reentrant BITMAP *DisplayLoadBitmap(WORD iResource);

_reentrant void DisplayPrintStringAddressUnicode   (INT x,INT y,void *pAddress ,INT bInvert);
_reentrant void DisplayPrintStringResourceUnicode  (INT x,INT y,WORD wResource,INT bInvert);
_reentrant void DisplayPrintNumber  (INT x,INT y,INT iNumber    ,INT iDigits    ,INT iFiller    ,INT bInvert);
_reentrant void DisplayPrintTime    (INT x,INT y,INT iMinutes   ,INT iSeconds                   ,INT bInvert);
_reentrant void DisplayPrintLongTime(INT x,INT y,INT iHours     ,INT iMinutes   ,INT iSeconds   ,INT bInvert);
_reentrant void DisplaySetFont(INT iFont);
_reentrant int DisplayPrintCharUnicode   (INT  x,INT y,WORD wChar,INT bInvert);
_reentrant void DisplayUpdateDisplay(void);
_reentrant void DisplayPrint1DigitHourTime(INT x,INT y,INT iHours     ,INT iMinutes   ,INT iSeconds   ,INT bInvert);
_reentrant void DisplayHandleMessage(Message *pMsg);



_reentrant void DisplayPrint1DigitHourTime(INT x,INT y,INT iHours, INT iMinutes ,INT iSeconds   ,INT bInvert); //Stmp002970

//***********************************************************************************************
// Function: DisplayModuleInit()
// Inputs:   pointer to module table entry for this module
// Outputs:  Returns EventWait value specifying when to call this module's process function next.
// Notes:  
//***********************************************************************************************
int _reentrant DisplayModuleInit(struct ModuleEntry *pEntry)
{
    union WaitMask returnMask;  

    g_pDisplayModuleEntry=pEntry;

    SALDisplayInit();
    HALDisplayInit();
    DisplaySetFont(0);

    //Tell ModuleTableEntry.wait to call this module's process function next when it has a msg
    returnMask.I=0x000000 | 2; // EVENT_MESSAGE
    return returnMask.I;  
}

_reentrant void DisplayClearRange           (INT  x,INT  y,WORD cx         ,WORD cy    );
_reentrant void DisplayPrintBitmapResource  (INT  x,INT  y,WORD wResource  ,INT bInvert);
_reentrant void DisplayPrintStringResource  (INT  x,INT  y,WORD wResource  ,INT bInvert);
_reentrant void DisplayPrintStringAddress   (INT  x,INT  y,void *pAddress  ,INT bInvert);
 
_reentrant void DisplaySetFont(INT);

//************************************************************************************************
// Function:  DisplayProcessQueue
// Inputs:    
// Outputs:   
// Notes
//************************************************************************************************
int _reentrant DisplayProcessQueue(union WaitMask Signals,struct MessageQueueDescriptor * pQueue)
{
    union WaitMask returnMask; 
    struct CMessage  Msg;
    unsigned long    lTimeout;
    INT              error;
    int i;
    INT Param1,Param2,Param3,Param4,Param5;//these are here as code optimizations
	int *SpectrumAddr;
    int bInvert;

    returnMask.I=0x000000 | 2;//Initialize it to signal us if we have a message

    if(Signals.B.m_bTimer)
    {
    }
    if(Signals.B.m_bMessage)
    {
        while(GetMessage(pQueue,&Msg))
        {

            Param1 = Msg.m_uMsg[1];
            Param2 = Msg.m_uMsg[2];
            Param3 = Msg.m_uMsg[3];    
            Param4 = Msg.m_uMsg[4];    
            Param5 = Msg.m_uMsg[5];    
            bInvert = 0;
            switch(Msg.m_uMsg[0])
            {
            case LCD_CLEAR_RANGE:
                SALDisplayClearRange(Param1,Param2,Param3,Param4);
                break;
            case LCD_PRINT_RANGE_INV_RSRC:
#define DISPLAY_NORMAL  0
#define DISPLAY_INVERT  1
#define DISPLAY_CLEAR   2 

                bInvert=DISPLAY_INVERT;
            case LCD_PRINT_RANGE_RSRC:
                DisplayPrintBitmapResource(Param1,Param2,Param3,bInvert);
                break;
            case LCD_PRINT_RANGE_INV_ADDR:
                bInvert=DISPLAY_INVERT;
            case LCD_PRINT_RANGE_ADDR:
                SALDisplayPrintBitmapAddress(Param1,Param2,(void*)Param3,bInvert);
                break;
            case LCD_PRINT_STRING_INV_RSRC:
                bInvert=DISPLAY_INVERT;
            case LCD_PRINT_STRING_RSRC:
                DisplayPrintStringResource(Param1,Param2,Param3,bInvert);
                break;
            case LCD_PRINT_STRING_INV_ADDR:
                bInvert=DISPLAY_INVERT;
            case LCD_PRINT_STRING_ADDR:
                DisplayPrintStringAddress(Param1,Param2,(void*)Param3,bInvert);
                break;
//LCD_SCROLL_DISPLAY       equ     MSG_TYPE_LCD+9
//LCD_READ_LCD_ADDR        equ     MSG_TYPE_LCD+10
            case LCD_SET_FONT:
                DisplaySetFont(Param1);
                break;
            case LCD_PRINT_NUMBER_INV:
                bInvert=DISPLAY_INVERT;
            case LCD_PRINT_NUMBER:
                DisplayPrintNumber(Param1/*x*/,Param2/*y*/,Param3/*number*/,Param4/*digits */,Param5/*filler character*/,bInvert);
                break;
            case LCD_PRINT_TIME_INV:
                bInvert=DISPLAY_INVERT;
            case LCD_PRINT_TIME:
#ifndef FAT32TEST
                DisplayPrintTime(Param1/*x*/,Param2/*y*/,Param3/*minutes*/,Param4/*seconds*/,bInvert);
#endif
                break;
            case LCD_PRINT_TIME_LONG_INV:
                bInvert=DISPLAY_INVERT;
            case LCD_PRINT_TIME_LONG:
#ifndef FAT32TEST
                DisplayPrintLongTime(Param1/*x*/,Param2/*y*/,Param3/*hours*/,Param4/*minutes*/,Param5/*seconds*/,bInvert);
#endif
                break;
            case LCD_PRINT_STRING_UNICODE_INV_ADDR:
                bInvert=DISPLAY_INVERT;
            case LCD_PRINT_STRING_UNICODE_ADDR:
                DisplayPrintStringAddressUnicode(Param1,Param2,(void*)Param3,bInvert);
                break;
            case LCD_PRINT_STRING_UNICODE_INV_RSRC:
                bInvert=DISPLAY_INVERT;
            case LCD_PRINT_STRING_UNICODE_RSRC:
                DisplayPrintStringResourceUnicode(Param1,Param2,Param3,bInvert);
                break;

            case LCD_PRINT_UNICODE_CHAR_INV:
                bInvert=DISPLAY_INVERT;
            case LCD_PRINT_UNICODE_CHAR:
                DisplayPrintCharUnicode   (Param1,Param2,Param3,bInvert);
                break;
            case LCD_PRINT_TIME_L_1DIG_HR:
                DisplayPrint1DigitHourTime(Param1/*x*/,Param2/*y*/,Param3/*hours*/,Param4/*minutes*/,Param5/*seconds*/,bInvert);
                break;
#ifdef SPECTRUM_ANAL            
            case LCD_DISPLAY_HISTOGRAM:
			 	SpectrumAddr = (int *)Param1;
				 for(i=0;i<Param2;i++)
				 {
				 	 histolength = SpectrumAddr[i];
					 histolength =	histolength - 16;
					 if (histolength < 0)
						 histolength = 0;
    				 SALDisplayPrintBitmapAddress( Param3+(i<<1), LCD_SIZE_Y - histolength,(void*)histo,0);
				 }
                break;
#endif
            default://any unhandled messages are sent to the SAL
                DisplayHandleMessage((Message*)&Msg);
                break;
            }
        }
    }
    DisplayUpdateDisplay();
    return returnMask.I;
}
