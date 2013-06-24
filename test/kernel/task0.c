#include "kernel.h"
#include "resource.h"
#include "messages.h"


_reentrant int Task0Region1Bank2(int a, int b, int *c);

extern WORD g_wCapCheckCounter;

SyncObject g_SyncObject0;
int g_i0 = 0;

_reentrant void Task0EntryPoint(void)
{
    int a;
    Message msg;
    while(1)
    {
        SysWaitOnMessage(-1,&msg);
        if(msg.m_wMessage == 0x010001)
        {
            SysCallFunction(RSRC_REGION1_TASK0_BANK2,Task0Region1Bank2,0,1,&a);
        }
        g_i0++;
        SysPostMessage(7,LCD_PRINT_NUMBER,0,0,g_i0,5,'0');
    }

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


#pragma endasm

