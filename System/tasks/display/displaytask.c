#include "types.h"
#include "kernel.h"
#include "messages.h"
#include "displaydrv.h"

extern _P WORD * _P _lc_u_p_Disp_start;
extern _Y WORD * _Y _lc_u_y_Disp_Init_OVLY_copy_start;
extern _Y WORD * _Y _lc_u_y_Disp_Init_OVLY_copy_end;
extern _Y WORD * _Y _lc_u_y_Disp_Code_OVLY_copy_start;
extern _Y WORD * _Y _lc_u_y_Disp_Code_OVLY_copy_end;

#if USBMSC_BUILD
extern _reentrant void CopyCodeIntoBootmanagerArea(_Y WORD * _Y pYCopyStart, 
                _P WORD * _P pPCopyStart, WORD wYCopySize);
#endif

void _reentrant DisplayTask(void)
{
    Message  Msg;
    unsigned long    lTimeout;
    INT              error;
    int i;
    INT Param1,Param2,Param3,Param4,Param5;//these are here as code optimizations
    int bInvert;

#if USBMSC_BUILD
    unsigned int CopySize = (unsigned int)(&_lc_u_y_Disp_Init_OVLY_copy_end-&_lc_u_y_Disp_Init_OVLY_copy_start);
    CopyCodeIntoBootmanagerArea((_Y WORD *)&_lc_u_y_Disp_Init_OVLY_copy_start, 
                                (_P WORD * _P)&_lc_u_p_Disp_start, CopySize);
#endif
    DisplayInit();
#ifdef USBMSC_BUILD
    CopySize = (unsigned int)(&_lc_u_y_Disp_Code_OVLY_copy_end-&_lc_u_y_Disp_Code_OVLY_copy_start);
        
    CopyCodeIntoBootmanagerArea((_Y WORD *)&_lc_u_y_Disp_Code_OVLY_copy_start, 
                                (_P WORD * _P)&_lc_u_p_Disp_start, CopySize);
#endif

    while(1)
    {
        SysWaitOnMessage(-1,&Msg);
        
        Param1 = Msg.m_wParams[0];
        Param2 = Msg.m_wParams[1];
        Param3 = Msg.m_wParams[2];    
        Param4 = Msg.m_wParams[3];    
        Param5 = Msg.m_wParams[4];    
        bInvert = 0;

        switch(Msg.m_wMessage)
        {
        case LCD_CLEAR_RANGE:
            DisplayClearRange(Param1,Param2,Param3,Param4);
            break;
        case LCD_PRINT_RANGE_INV_RSRC:
            bInvert=DISPLAY_INVERT;
        case LCD_PRINT_RANGE_RSRC:
            DisplayPrintBitmapResource(Param1,Param2,Param3,bInvert);
            break;
        case LCD_PRINT_RANGE_INV_ADDR:
            bInvert=DISPLAY_INVERT;
        case LCD_PRINT_RANGE_ADDR:
            DisplayPrintBitmapAddress(Param1,Param2,(void*)Param3,bInvert);
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
            DisplayPrintTime(Param1/*x*/,Param2/*y*/,Param3/*minutes*/,Param4/*seconds*/,bInvert);
            break;
        case LCD_PRINT_TIME_LONG_INV:
            bInvert=DISPLAY_INVERT;
        case LCD_PRINT_TIME_LONG:
            DisplayPrintLongTime(Param1/*x*/,Param2/*y*/,Param3/*hours*/,Param4/*minutes*/,Param5/*seconds*/,bInvert);
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
        default://any unhandled messages are sent to the SAL
            DisplayHandleMessage(&Msg);
            break;
        }
        DisplayUpdateDisplay();

    }
}

