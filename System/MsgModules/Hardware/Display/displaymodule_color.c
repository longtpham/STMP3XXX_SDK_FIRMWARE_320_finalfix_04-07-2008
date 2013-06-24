////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2004-2006
//
// File        : displaymodule_color.c
// Description : LCD display driver
////////////////////////////////////////////////////////////////////////////////
#include "types.h"

#include "messages.h"
#include "project.h"
#include "stringlib.h"
#include "sysmem.h"
#include "sysequ.h"
#include "sysresourceapi.h"
#include "exec.h"
#include "displaymodule_color.h"

#pragma optimize 1

#include "displaydriver_color.h"
#include "unicode.h"
#include "dbcs.h"

#ifdef SPECTRUM_ANAL 
#include "spectrogram.h"
#include "display.h"
#endif

#define MAX_MASKS 5
#define MAX_DIGITS 10

//#include "display.h"


struct ModuleEntry *g_pDisplayModuleEntry = NULL;


_reentrant BITMAP *DisplayLoadBitmap(WORD iResource);

_reentrant void DisplayPrintStringAddressUnicode   (INT x,INT y,void *pAddress ,INT bInvert, INT iFrameBufferID);
_reentrant void DisplayPrintStringResourceUnicode  (INT x,INT y,WORD wResource,INT bInvert, INT iFrameBufferID);
_reentrant void DisplayPrintNumber  (INT x,INT y,INT iNumber    ,INT iDigits    ,INT iFiller    ,INT bInvert, INT iFrameBufferID);
_reentrant void DisplayPrintTime    (INT x,INT y,INT iMinutes   ,INT iSeconds                   ,INT bInvert, INT iFrameBufferID);
_reentrant void DisplayPrintLongTime(INT x,INT y,INT iHours     ,INT iMinutes   ,INT iSeconds   ,INT bInvert, INT iFrameBufferID);
_reentrant void DisplaySetFont(INT iFont);
_reentrant int DisplayPrintCharUnicode   (INT  x,INT y,WORD wChar,INT bInvert, INT iFrameBufferID);
_reentrant void DisplayUpdateDisplay(void);
_reentrant void DisplayHandleMessage(Message *pMsg);

//_reentrant void DisplayPrint1DigitHourTime(INT x,INT y,INT iHours, INT iMinutes ,INT iSeconds   ,INT bInvert, INT iFrameBufferID); //Stmp002970

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

_reentrant void DisplayPrintBitmapResource  (INT  x,INT  y,WORD wResource  ,INT bInvert, INT iFrameBufferID);
_reentrant void DisplayPrintStringResource  (INT  x,INT  y,WORD wResource  ,INT bInvert, INT iFrameBufferID);
_reentrant void DisplayPrintStringAddress   (INT  x,INT  y,void *pAddress  ,INT bInvert, INT iFrameBufferID);
//3200Merge+
_reentrant void DisplayPrintBitmapFile(INT x, INT y, void* pFileName, INT bInvert, INT iFrameBufferID);
//3200Merge-
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
    INT Param1,Param2,Param3,Param4,Param5, Param6;//these are here as code optimizations

    int bInvert;
	int *SpectrumAddr;
    WORD cBarChar;

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
            Param6 = Msg.m_uMsg[6];
            bInvert = 0;
            switch(Msg.m_uMsg[0])
            {
            case LCD_CLEAR_RANGE:
                Param5 = DIRECT_TO_LCD_BUFFER;	//old message uses direct-to-lcd method
            case LCD_CLEAR_RANGE_BUFFER:
				SALDisplayClearRange(Param1,Param2,Param3,Param4,Param5);
				break;
            case LCD_PRINT_RANGE_INV_RSRC:
                bInvert=DISPLAY_INVERT;
            case LCD_PRINT_RANGE_RSRC:
                Param4 = DIRECT_TO_LCD_BUFFER;
            case LCD_PRINT_RANGE_INV_RSRC_BUFFER:
				 if(Param4 != DIRECT_TO_LCD_BUFFER){	//we don't want to do this if the message was direct-to-lcd
				 	bInvert=DISPLAY_INVERT;
            	 }
            case LCD_PRINT_RANGE_RSRC_BUFFER:
				DisplayPrintBitmapResource(Param1,Param2,Param3,bInvert,Param4);
				break;
            case LCD_PRINT_RANGE_INV_ADDR:
                bInvert=DISPLAY_INVERT;
            case LCD_PRINT_RANGE_ADDR:
                Param4 = DIRECT_TO_LCD_BUFFER;
            case LCD_PRINT_RANGE_INV_ADDR_BUFFER:
                if(Param4 != DIRECT_TO_LCD_BUFFER){
                	bInvert=DISPLAY_INVERT;
				}
            case LCD_PRINT_RANGE_ADDR_BUFFER:
            	SALDisplayPrintBitmapAddress(Param1,Param2,(void*)Param3,bInvert,Param4);
            	break;
//3200Merge+
            case LCD_PRINT_RANGE_INV_FILE:
                bInvert=DISPLAY_INVERT;
            case LCD_PRINT_RANGE_FILE:
				DisplayPrintBitmapFile(Param1,Param2,(void*)Param3,bInvert,DIRECT_TO_LCD_BUFFER);
				break;
//3200Merge-
            case LCD_PRINT_STRING_INV_RSRC:
                bInvert=DISPLAY_INVERT;
            case LCD_PRINT_STRING_RSRC:
				Param4 = DIRECT_TO_LCD_BUFFER;
			case LCD_PRINT_STRING_INV_RSRC_BUFFER:
				if(Param4 != DIRECT_TO_LCD_BUFFER){
                	bInvert=DISPLAY_INVERT;
				}
			case LCD_PRINT_STRING_RSRC_BUFFER:
            	DisplayPrintStringResource(Param1,Param2,Param3,bInvert,Param4);
            	break;

            case LCD_PRINT_STRING_INV_ADDR:
                bInvert=DISPLAY_INVERT;
            case LCD_PRINT_STRING_ADDR:
            	Param4 = DIRECT_TO_LCD_BUFFER;
  			case LCD_PRINT_STRING_INV_ADDR_BUFFER:
				if(Param4 != DIRECT_TO_LCD_BUFFER){
                	bInvert=DISPLAY_INVERT;
				}
			case LCD_PRINT_STRING_ADDR_BUFFER:
				DisplayPrintStringAddress(Param1,Param2,(void*)Param3,bInvert, Param4);
				break;


//LCD_SCROLL_DISPLAY       equ     MSG_TYPE_LCD+9
//LCD_READ_LCD_ADDR        equ     MSG_TYPE_LCD+10
            case LCD_SET_FONT:
                DisplaySetFont(Param1);
                break;

            case LCD_PRINT_NUMBER_INV:
                bInvert=DISPLAY_INVERT;
            case LCD_PRINT_NUMBER:
            	Param6 = DIRECT_TO_LCD_BUFFER;
			case LCD_PRINT_NUMBER_INV_BUFFER:
				if(Param6 != DIRECT_TO_LCD_BUFFER){
                	bInvert=DISPLAY_INVERT;
				}
			case LCD_PRINT_NUMBER_BUFFER:
                DisplayPrintNumber(Param1/*x*/,Param2/*y*/,Param3/*number*/,Param4/*digits */,Param5/*filler character*/,bInvert, Param6);
				break;

            case LCD_PRINT_TIME_INV:
                bInvert=DISPLAY_INVERT;
            case LCD_PRINT_TIME:
				Param5 = DIRECT_TO_LCD_BUFFER;
			case LCD_PRINT_TIME_INV_BUFFER:
				if(Param5 != DIRECT_TO_LCD_BUFFER){
                	bInvert=DISPLAY_INVERT;
				}
			case LCD_PRINT_TIME_BUFFER:
                DisplayPrintTime(Param1/*x*/,Param2/*y*/,Param3/*minutes*/,Param4/*seconds*/,bInvert, Param5);
                break;


            case LCD_PRINT_TIME_LONG_INV:
                bInvert=DISPLAY_INVERT;
            case LCD_PRINT_TIME_L_1DIG_HR:
            case LCD_PRINT_TIME_LONG:
                Param6 = DIRECT_TO_LCD_BUFFER;
			case LCD_PRINT_TIME_LONG_INV_BUFFER:
				if(Param6 != DIRECT_TO_LCD_BUFFER){
                	bInvert=DISPLAY_INVERT;
				}
			case LCD_PRINT_TIME_L_1DIG_HR_BUFFER:
			case LCD_PRINT_TIME_LONG_BUFFER:
				DisplayPrintLongTime(Param1/*x*/,Param2/*y*/,Param3/*hours*/,Param4/*minutes*/,Param5/*seconds*/,bInvert, Param6);
				break;

            case LCD_PRINT_STRING_UNICODE_INV_ADDR:
                bInvert=DISPLAY_INVERT;
            case LCD_PRINT_STRING_UNICODE_ADDR:
				Param4 = DIRECT_TO_LCD_BUFFER;
            case LCD_PRINT_STRING_UNICODE_INV_ADDR_BUFFER:
				if(Param4 != DIRECT_TO_LCD_BUFFER){
                	bInvert=DISPLAY_INVERT;
				}
			case LCD_PRINT_STRING_UNICODE_ADDR_BUFFER:
                DisplayPrintStringAddressUnicode(Param1,Param2,(void*)Param3,bInvert, Param4);
                break;

            case LCD_PRINT_STRING_UNICODE_INV_RSRC:
                bInvert=DISPLAY_INVERT;
            case LCD_PRINT_STRING_UNICODE_RSRC:
				Param4 = DIRECT_TO_LCD_BUFFER;
			case LCD_PRINT_STRING_UNICODE_INV_RSRC_BUFFER:
				if(Param4 != DIRECT_TO_LCD_BUFFER){
                	bInvert=DISPLAY_INVERT;
				}
			case LCD_PRINT_STRING_UNICODE_RSRC_BUFFER:
			    DisplayPrintStringResourceUnicode(Param1,Param2,Param3,bInvert, Param4);
                break;


            case LCD_PRINT_UNICODE_CHAR_INV:
                bInvert=DISPLAY_INVERT;
            case LCD_PRINT_UNICODE_CHAR:
                Param4 = DIRECT_TO_LCD_BUFFER;
       		case LCD_PRINT_UNICODE_CHAR_INV_BUFFER:
				if(Param4 != DIRECT_TO_LCD_BUFFER){
                	bInvert=DISPLAY_INVERT;
				}
			case LCD_PRINT_UNICODE_CHAR_BUFFER:
            	DisplayPrintCharUnicode   (Param1,Param2,Param3,bInvert, Param4);
            	break;
#ifdef SPECTRUM_ANAL            
            case LCD_DISPLAY_HISTOGRAM:
			 	SpectrumAddr = (int *)Param1;
                cBarChar = '-';
				 for(i=0;i<Param2;i++)
				 {
				 	 histolength = SpectrumAddr[i];
					 histolength =	histolength - 16;
					 if (histolength < 0)
						 histolength = 0;
                     DisplayPrintStringAddress( Param3+(i<<1), LCD_SIZE_Y - histolength, (void*)&cBarChar, 0, DIRECT_TO_LCD_BUFFER );
				 }
                break;
#endif
            case LCD_BEGIN_FRAME:	//do nothing...we need these here so that
			case LCD_END_FRAME:
				break;
            //case LCD_PRINT_TIME_L_1DIG_HR:
			//	Param6 = DIRECT_TO_LCD_BUFFER;
            //case LCD_PRINT_TIME_L_1DIG_HR_BUFFER:
            //    DisplayPrintLongTime(Param1/*x*/,Param2/*y*/,Param3/*hours*/,Param4/*minutes*/,Param5/*seconds*/,bInvert, Param6);
            //    DisplayPrint1DigitHourTime(Param1/*x*/,Param2/*y*/,Param3/*hours*/,Param4/*minutes*/,Param5/*seconds*/,bInvert, Param6);
            //    break;
//            case LCD_SET_BUFFER_WINDOW:
				//not supported yet
//				break;
            default://any unhandled messages are sent to the SAL
                DisplayHandleMessage((Message*)&Msg);
                break;
            }
        }
    }
    DisplayUpdateDisplay();
    return returnMask.I;
}
