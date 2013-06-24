////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename: lcdtest.c
// Description: File System Test Program
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//   Includes and external references
////////////////////////////////////////////////////////////////////////////////

#include "hwequ.h"
#include "resource.h"
#include "messages.h"
#include "lcdtest.h"
#include "lcdsupport.h"
#include "types.h"
#include "ddildl.h"
#include "sysresourceapi.h"

#define LCDMIN_CONT 0
#define LCDMAX_CONT 100
#define DRIVE_TAG_RESOURCE_BIN_RAM 0x10

extern WORD stlogo[];
extern WORD g_wNumDrives;
extern WORD g_wNumMedia;

extern BYTE s_btHandle_PDM;
extern BYTE s_bthandle_SDM;
extern BYTE g_btHandlePGM;
extern BYTE g_btHandleSGMs;
extern BYTE g_btHandleScript00;

_packed BYTE bStartTest[] = "LCD TEST SUITE";
_packed BYTE bEndTest[]   = "All LCD API Tested";
_packed BYTE bEndTest1[]  = "LCD Tests Passed";
_packed BYTE bScroll[]    = "Scroll LCD Display";
_packed BYTE bScroll1[]   = "Scrolling Display";
_packed BYTE bFont[]      = "Vary Font Index";
_packed BYTE bFont1[]     = "Font 1";
_packed BYTE bFont2[]     = "Font 2";
_packed BYTE bCPage1[]    = "Codepage 1";
_packed BYTE bCPage2[]    = "Codepage 2";
_packed BYTE bCPageHello[]    = {0xc4,0xe3,0xba,0xc3};     //these are Chinese characters representing Ni Hao
_packed BYTE bCPage[]     = "Vary Codepage";
_packed BYTE bNum[]       = "Print Numbers";
_packed BYTE bTime[]      = "LCD Time Display";
_packed BYTE bContr[]     = "Test LCD Contrast";
_packed BYTE bPrint[]     = "Test Print Str Option";
_packed BYTE bImg[]       = "Display Images";

void *g_pCurrentTask = NULL;
WORD *Kernel_pTempStack;
INT g_JanusDrive = -1;

// Added additional stub symbols. These were necessary to support the fix for
// defects STMP00006219 && STMP00006678.

#pragma asm
     org p,"IRQTemp":
    global FSysGetIrqLevel
    global FSysSetIrqLevel
FSysGetIrqLevel
FSysSetIrqLevel
FEnterNonReentrantSection
FLeaveNonReentrantSection
GetIrqL
SetIrqL
    rts
#pragma endasm

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

long int  FSGetDWord(void *buffer, int Offset,int MemoryType)
{
    return -1;
}

void main(void)
{

   int i;
   
    // Init the media
    for(i = 0 ; i < g_wNumMedia ; i++)
    {
        if(MediaInit(i) != SUCCESS)
        {
            SystemHalt();
        }
    }        

    // Discover the Media and its drives
    for(i = 0 ; i < g_wNumMedia ; i++)
    {    
        if(MediaDiscoverAllocation(i) != SUCCESS)
        {
            SystemHalt();
        }
    }        


    // Init All Drives
    for(i=0;i<g_wNumDrives;i++)
    {
        if(DriveInit(i) != SUCCESS)
        {
        }
    }

    SysSetResourceTag(DRIVE_TAG_RESOURCE_BIN_RAM);

    // Initialization Procedures
    DisplayModuleInit(ModuleTableDisplay);
    SwizzleText(bStartTest);
    SwizzleText(bEndTest);
    SwizzleText(bEndTest1);
    SwizzleText(bScroll);
    SwizzleText(bScroll1);
    SwizzleText(bFont);
    SwizzleText(bFont1);
    SwizzleText(bFont2);
    SwizzleText(bCPage1);
    SwizzleText(bCPage);
    SwizzleText(bCPage2);
    SwizzleText(bCPageHello);
    SwizzleText(bNum);
    SwizzleText(bTime);
    SwizzleText(bContr);
    SwizzleText(bPrint);
    SwizzleText(bImg);


    for(i = 0 ; i < 2 ; i++)
    {    
        // Clear Display
        LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);

        // Display Test Title
        PrintStringAddr(11, 30, bStartTest);
        Delay(100000);

        // Test Procedures
        TestLcdPrintRange();
        TestLcdPrintString();

        TestLcdContrast();
        TestLcdSetFont();
        TestLcdCodepage();
        TestLcdPrintNumber();
        TestLcdPrintTime();

        // End of Test
        LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);
        PrintStringAddr(4, 30, bEndTest);
        PrintStringAddr(7, 40, bEndTest1);
        
        // Will switch resource drive to NAND
        SysSetResourceTag(DRIVE_TAG_RESOURCE_BIN);

        s_btHandle_PDM = 0;
        s_bthandle_SDM = 0;
        g_btHandlePGM = 0;
        g_btHandleSGMs = 0;
        g_btHandleScript00 = 0;
        
    }

    while(1);

}


/******************************************************* 
 * TestLcdPrintRange
 *
 * LCD Messaging Module API tested:
 *      - LCD_PRINT_RANGE_RSRC
 *      - LCD_PRINT_RANGE_INV_RSRC
 *      - LCD_PRINT_RANGE_ADDR
 *      - LCD_PRINT_RANGE_INV_ADDR
 *******************************************************/
   
void _reentrant TestLcdPrintRange(void){
    LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);
    PrintStringAddr(12, 30, bImg);
    Delay(50000);

    // LCD_PRINT_RANGE_RSRC
    LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);
    PrintRangeRsrc(4, 20, RSRC_SPLASH_SCREEN);
    Delay(50000);

    // LCD_PRINT_RANGE_INV_RSRC
    LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);
    PrintRangeInvRsrc(4, 20, RSRC_SPLASH_SCREEN);
    Delay(50000);

    // LCD_PRINT_RANGE_ADDR
    LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);
    PrintRangeAddr(0, 16, stlogo);
    Delay(50000);

    // LCD_PRINT_RANGE_INV_ADDR
    PrintRangeInvAddr(0, 16, stlogo);
    Delay(50000);

    return;
}


/******************************************************* 
 * TestLcdPrintString
 *
 * LCD Messaging Module API tested:
 *      - LCD_CLEAR_RANGE
 *      - LCD_PRINT_STRING_RSRC
 *      - LCD_PRINT_STRING_ADDR
 *******************************************************/

void _reentrant TestLcdPrintString(void){

    // LCD_CLEAR_RANGE
    LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);
    // LCD_PRINT_STRING_ADDR
    PrintStringAddr(2, 30, bPrint);
    Delay(50000);

    // LCD_PRINT_STRING_RSRC
    LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);
    PrintStringRsrc(5, 30, RSRC_STRING);
    Delay(50000);

    return;
}

/******************************************************* 
 * TestLcdScrollDisplay
 *
 * LCD Messaging Module API tested:
 *      - LCD_SCROLL_DISPLAY
 *******************************************************/
    
void _reentrant TestLcdScrollDisplay(void){
    INT i;
    
    LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);
    PrintStringAddr(10, 30, bScroll);
    Delay(50000);
    
    LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);
    PrintStringAddr(12, 30, bScroll1);
    Delay(50000);

    for(i=0;i<0x20;i++){
        ScrollDisplay(0, 2);
        Delay(10000);
    }

    ScrollDisplay(10, -20);
    Delay(50000);

    return;
}


/******************************************************* 
 * TestLcdContrast
 *
 * LCD Messaging Module API tested:
 *      - LCD_SET_CONTRAST
 *      - LCD_INC_CONTRAST
 *      - LCD_DEC_CONTRAST
 *******************************************************/

void _reentrant TestLcdContrast(void){
    INT i;
    //LCDMIN_CONT & LCDMAX_CONT are local variables and not system variables

    LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);
    PrintStringAddr(7, 33, bContr);
    Delay(50000);

    SetContrast(LCDMIN_CONT);
    Delay(50000);

    for(i=LCDMIN_CONT; i<=LCDMAX_CONT; i++){
        IncContrast();
        Delay(5000);
    }

    for(i=LCDMIN_CONT; i<=LCDMAX_CONT; i++){
        DecContrast();
        Delay(5000);
    }
    
    SetContrast(0x200);
    Delay(50000);   
    SetContrast(LCDMAX_CONT/2);
    Delay(50000);   
    LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);

    return;
}


/******************************************************* 
 * TestLcdSetFont
 *
 * LCD Messaging Module API tested:
 *      - LCD_SET_FONT
 *******************************************************/

void _reentrant TestLcdSetFont(void){
    LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);
    PrintStringAddr(7, 10, bFont);
    Delay(10000);

    SetFont(1);
    PrintStringAddr(7, 32, bFont);
    Delay(10000);

    SetFont(3);
    PrintStringAddr(7, 55, bFont);
    Delay(50000);

    return;
}

/******************************************************* 
 * TestLcdCodepage
 *
 *  Tests the switching capability of the codepage
 * 
 *******************************************************/

void _reentrant TestLcdCodepage(void){

    int i;
    SetFont(0);
    LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);
    Delay(10000);
    PrintStringAddr(20, 2, bCPage);

    
    
    for ( i=1; i < 3; i++ )
    {

        // setting these variables to -1 will force the recaching of maps and scripts
        if(s_btHandle_PDM)
            SysCloseResource(s_btHandle_PDM);        

        if(s_bthandle_SDM)
            SysCloseResource(s_bthandle_SDM);        
        if(g_btHandlePGM)
            SysCloseResource(g_btHandlePGM);        
        if(g_btHandleSGMs)
            SysCloseResource(g_btHandleSGMs);
        if(g_btHandleScript00)        
            SysCloseResource(g_btHandleScript00);        

        s_btHandle_PDM = 0;
        s_bthandle_SDM = 0;
        g_btHandlePGM = 0;
        g_btHandleSGMs = 0;
        g_btHandleScript00 = 0;

        // global variables which decide the code pages, codepages get reloaded inside
        // of the unicode.c 
        g_iPDM = RSRC_PDM;
        g_iSDMS = RSRC_SDMS;

        // change the font
        SetFont(0);

        LcdClear(0, 16, LCD_X_PIXELS, 66);
        PrintStringAddr(0, 16, bFont1);
        PrintStringAddr(0, 24, bCPage1);
        PrintStringAddr(0, 32, bCPageHello);
        Delay(50000);

        // Now change the font and codepage.  The chinese characters should be displayed correctly now 
        // due to the codepage mapping them correctly to the corresponding Unicode value using the selected
        // codepage.

        if(s_btHandle_PDM)
            SysCloseResource(s_btHandle_PDM);        
        if(s_bthandle_SDM)
            SysCloseResource(s_bthandle_SDM);        
        if(g_btHandlePGM)
            SysCloseResource(g_btHandlePGM);        
        if(g_btHandleSGMs)
            SysCloseResource(g_btHandleSGMs);
        if(g_btHandleScript00)        
            SysCloseResource(g_btHandleScript00);        

        s_btHandle_PDM = 0;
        s_bthandle_SDM = 0;
        g_btHandlePGM = 0;
        g_btHandleSGMs = 0;
        g_btHandleScript00 = 0;

        g_iPDM = RSRC_PDM_2;
        g_iSDMS = RSRC_SDMS_2;
        SetFont(1);

        LcdClear(0, 16, LCD_X_PIXELS, 66);
        PrintStringAddr(0, 16, bFont2);
        PrintStringAddr(0, 32, bCPage2);
        PrintStringAddr(0, 48, bCPageHello);
        Delay(50000);
    }

    if(s_btHandle_PDM)
        SysCloseResource(s_btHandle_PDM);        
    if(s_bthandle_SDM)
        SysCloseResource(s_bthandle_SDM);        
    if(g_btHandlePGM)
        SysCloseResource(g_btHandlePGM);        
    if(g_btHandleSGMs)
        SysCloseResource(g_btHandleSGMs);
    if(g_btHandleScript00)        
        SysCloseResource(g_btHandleScript00);        

    s_btHandle_PDM = 0;
    s_bthandle_SDM = 0;
    g_btHandlePGM = 0;
    g_btHandleSGMs = 0;
    g_btHandleScript00 = 0;

    g_iPDM = RSRC_PDM;
    g_iSDMS = RSRC_SDMS;

    SetFont(0);
    return;
}


/******************************************************* 
 * TestLcdPrintNumber
 *
 * LCD Messaging Module API tested:
 *      - LCD_PRINT_NUMBER
 *******************************************************/

void _reentrant TestLcdPrintNumber(void){

    LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);
    PrintStringAddr(10, 20, bNum);
    PrintNumber(10, 35, 0x100, 3, '0');
    Delay(50000);

    return;
}


/******************************************************* 
 * TestLcdPrintTime
 *
 * LCD Messaging Module API tested:
 *      - LCD_PRINT_TIME
 *******************************************************/


void _reentrant TestLcdPrintTime(void){

    LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);
    PrintStringAddr(10, 20, bTime);
    PrintTime(10, 35, 10, 12);
    Delay(50000);

    return;
}
