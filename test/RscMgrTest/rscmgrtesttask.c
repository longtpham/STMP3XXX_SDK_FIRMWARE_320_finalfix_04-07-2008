#include "kernel.h"
#include "resource.h"
#include "messages.h"
#include "ddildl.h"


// LCD defines
#define LCD_X_PIXELS    98
#define LCD_Y_PIXELS    64

extern WORD g_wNumDrives;
extern WORD stlogo[];

void _reentrant SwizzleText(_packed BYTE *pText);
void _reentrant LcdClear(WORD wPosX, WORD wPosY, WORD wLength, WORD wHeight);
void _reentrant PrintStringAddr(WORD wPosX, WORD wPosY, _packed BYTE *pMsgString);
void _reentrant TestLcdPrintRange(void);
void _reentrant TestLcdPrintString(void);
void _reentrant TestLcdPrintNumber(void);
void _reentrant TestLcdSetFont(void);

void _reentrant PrintStringAddr(WORD wPosX, WORD wPosY, _packed BYTE *pMsgString);
void _reentrant PrintRangeRsrc(WORD wPosX, WORD wPosY, WORD wRsrc);
void _reentrant PrintRangeInvRsrc(WORD wPosX, WORD wPosY, WORD wRsrc);
void _reentrant PrintRangeAddr(WORD wPosX, WORD wPosY, WORD *pData);
void _reentrant PrintRangeInvAddr(WORD wPosX, WORD wPosY, WORD *pData);
void _reentrant PrintStringRsrc(WORD wPosX, WORD wPosY, WORD wRsrc);
void _reentrant PrintNumber(WORD wMsgXPos, WORD wMsgYPos, WORD wNum, WORD wWidth, WORD wLeadChar);
void _reentrant SetFont(WORD wFontIndex);

_packed BYTE bStartTest[] = "RSC MGR TEST";
_packed BYTE bEndTest[]   = "RSC MGR END TEST";
_packed BYTE bFont[]      = "Vary Font Index";
_packed BYTE bNum[]       = "Print Numbers";
_packed BYTE bPrint[]     = "Test Print Str Option";
_packed BYTE bImg[]       = "Display Images";

_packed BYTE bLetter1[]       = "ABCDEFGHIJKLMN";
_packed BYTE bLetter2[]       = "OPQRSTUVWXYZ";
_packed BYTE bLetter3[]       = "abcdefghijklmn";
_packed BYTE bLetter4[]       = "opqrstuvwxyz";
_packed BYTE bLetter5[]       = "0123456789";

extern WORD g_wCapCheckCounter;


#if 0
// removal allowed redef error to stop and link worked.
// stub functions
void EnterNonReentrantSection(void)
{
}
void LeaveNonReentrantSection(void)
{
}
//----------------------------------
#endif



// Inputs: 
// Outputs: 
// Notes:   
_reentrant void Task0EntryPoint(void)
{
    SwizzleText(bStartTest);
    SwizzleText(bEndTest);
    SwizzleText(bFont);
    SwizzleText(bNum);
    SwizzleText(bPrint);
    SwizzleText(bImg);
    SwizzleText(bLetter1);
    SwizzleText(bLetter2);
    SwizzleText(bLetter3);
    SwizzleText(bLetter4);
    SwizzleText(bLetter5);


    while(1)
    {

        // Clear Display
        LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);
        SysWait(10);

        PrintStringAddr(11, 30, bStartTest);
        SysWait(3000);

        // Display Test Title
        LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);
        SysWait(1000);
        PrintStringAddr(1, 1, bLetter1);
        PrintStringAddr(1, 11, bLetter2);
        PrintStringAddr(1, 21, bLetter3);
        PrintStringAddr(1, 31, bLetter4);
        PrintStringAddr(1, 41, bLetter5);
        SysWait(10000);

        // Test Procedures
        TestLcdPrintRange();
        TestLcdPrintString();
        TestLcdPrintNumber();
        TestLcdSetFont();
        
        LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);
        SysWait(10);
        PrintStringAddr(4, 30, bEndTest);

        SysWait(-1);

    }

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
    SysWait(3000);

    // LCD_PRINT_RANGE_RSRC
    LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);
    PrintRangeRsrc(4, 20, RSRC_SPLASH_SCREEN);
    SysWait(3000);

    // LCD_PRINT_RANGE_INV_RSRC
    LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);
    PrintRangeInvRsrc(4, 20, RSRC_SPLASH_SCREEN);
    SysWait(3000);

    // LCD_PRINT_RANGE_ADDR
    LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);
    PrintRangeAddr(0, 16, stlogo);
    SysWait(3000);

    // LCD_PRINT_RANGE_INV_ADDR
    PrintRangeInvAddr(0, 16, stlogo);
    SysWait(3000);

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
    SysWait(3000);

    // LCD_PRINT_STRING_RSRC
    LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);
    PrintStringRsrc(5, 30, RSRC_STRING);
    SysWait(3000);

    return;
}

void _reentrant TestLcdPrintNumber(void){

    LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);
    PrintStringAddr(10, 20, bNum);
    PrintNumber(10, 35, 0x100, 3, '0');
    SysWait(3000);

    return;
}

void _reentrant TestLcdSetFont(void)
{
    LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);
    PrintStringAddr(7, 10, bFont);
    SysWait(3000);

    SetFont(2);
    PrintStringAddr(7, 32, bFont);
    SysWait(3000);

    SetFont(3);
    PrintStringAddr(7, 55, bFont);
    SysWait(3000);

    return;
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

void _reentrant LcdClear(WORD wPosX, WORD wPosY, WORD wLength, WORD wHeight)
{
    SysPostMessage(6,LCD_CLEAR_RANGE,wPosX,wPosY,wLength,wHeight);
    SysWait(0);
}

void _reentrant PrintStringAddr(WORD wPosX, WORD wPosY, _packed BYTE *pMsgString)
{
    SysPostMessage(5,LCD_PRINT_STRING_ADDR,wPosX,wPosY,pMsgString);
}

void _reentrant PrintRangeRsrc(WORD wPosX, WORD wPosY, WORD wRsrc)
{
    SysPostMessage(5, LCD_PRINT_RANGE_RSRC, wPosX, wPosY, wRsrc);
}

void _reentrant PrintRangeInvRsrc(WORD wPosX, WORD wPosY, WORD wRsrc)
{
    SysPostMessage(5, LCD_PRINT_RANGE_INV_RSRC, wPosX, wPosY, wRsrc);
}


void _reentrant PrintRangeAddr(WORD wPosX, WORD wPosY, WORD *pData)
{
    SysPostMessage(5, LCD_PRINT_RANGE_ADDR, wPosX, wPosY, pData);
}


void _reentrant PrintRangeInvAddr(WORD wPosX, WORD wPosY, WORD *pData)
{
    SysPostMessage(5, LCD_PRINT_RANGE_INV_ADDR, wPosX, wPosY, pData);
}

void _reentrant PrintStringRsrc(WORD wPosX, WORD wPosY, WORD wRsrc)
{
    SysPostMessage(5,LCD_PRINT_STRING_RSRC,wPosX,wPosY,wRsrc);
}

void _reentrant PrintNumber(WORD wMsgXPos, WORD wMsgYPos, WORD wNum, WORD wWidth, WORD wLeadChar)
{
    SysPostMessage(7,LCD_PRINT_NUMBER,wMsgXPos,wMsgYPos,wNum,wWidth,wLeadChar);
}

void _reentrant SetFont(WORD wFontIndex)
{
    SysPostMessage(3,LCD_SET_FONT,wFontIndex);
}

#pragma asm
;///////////////////////////////////////////////////////////////////////////////
;> Name: DivideByThree
;
;  Type: Function
;  Description:     WORD  DivideByThree(WORD wVal, WORD _Y *pRemainder);
;  Inputs:          wVal (in A)          - The value to divde by 3
;                   pRemainder (in R0)   - Pointer for returning the remainder
;  Outputs:         Return (in A) the quotient: wVal/3
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////
FDivideByThree

OneThird equ 0.333333333333
Three    equ 3


    move    a1,x0                           ;put the operand into x0
    clr     a               #>OneThird,y0 
    mpy     x0,y0,b         x0,a0           ;b1 = wVal/3,  a0 = wVal

    move    b1,x0                        	;x0 = wVal/3
    asl     a               #>Three,y0      ;right shift it, so the MAC will be correct, y0 = 3
    mac     -x0,y0,a               	        ;a = 3 * (wVal/3) - wVal   (the remainder)

    asr     a                               ; get the right shift, now

    move    a0,y:(r0)                       ;return remainder through y:(r0)
    move    x0,a                            ;and quotient in A
    rts

    
    
    org    y,"lcdTest_Y":

    ;global stlogo
    ;global Fstlogo
    
Fstlogo
stlogo  dc      $000060   ; 96 pixels wide
        dc      $000020   ; 32 pixels tall
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $c08000
        dc      $e1e0e0
        dc      $efe7e3
        dc      $ffffff
        dc      $f3fbff
        dc      $e3e3e3
        dc      $e3e3e3
        dc      $e3e3e3
        dc      $e3e3e3
        dc      $e7e3e3
        dc      $276fef
        dc      $000303
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $4040c0
        dc      $404040
        dc      $0000c0
        dc      $000000
        dc      $000080
        dc      $000000
        dc      $800000
        dc      $808080
        dc      $808080
        dc      $000000
        dc      $000000
        dc      $000080
        dc      $000000
        dc      $038100
        dc      $ff0f07
        dc      $f8bcfe
        dc      $40e0f0
        dc      $008000
        dc      $000000
        dc      $000000
        dc      $404040
        dc      $40c040
        dc      $404040
        dc      $000000
        dc      $800000
        dc      $808080
        dc      $008080
        dc      $000000
        dc      $800000
        dc      $000000
        dc      $000000

        dc      $000000
        dc      $12121b
        dc      $121212
        dc      $00001e
        dc      $000000
        dc      $00001f
        dc      $000000
        dc      $1f0000
        dc      $101010
        dc      $1d1414
        dc      $000000
        dc      $000000
        dc      $06031f
        dc      $c68c0c
        dc      $78ffe3
        dc      $ff1e3c
        dc      $0307ff
        dc      $0e1811
        dc      $0b090b
        dc      $10180e
        dc      $000000
        dc      $000000
        dc      $001f00
        dc      $000000
        dc      $000000
        dc      $1f0000
        dc      $161616
        dc      $001010
        dc      $000000
        dc      $1f0000
        dc      $101010
        dc      $001010

        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $100000
        dc      $1e1c18
        dc      $1b1f1f
        dc      $181819
        dc      $ff1818
        dc      $18187f
        dc      $181818
        dc      $181818
        dc      $181818
        dc      $1e1c1c
        dc      $18181c
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
#pragma endasm
