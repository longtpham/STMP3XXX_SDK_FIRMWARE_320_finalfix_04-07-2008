    nolist
    include 'sysmacro.asm'
    include 'const.xref'
    list

    global  strncpy

    org p,"strncpy":
;///////////////////////////////////////////////////////////////////////////////
;> Name: strncpy
;
;  Type: Function
;  Description: _asmfunc void strncpy(char _Y * _Y pDest, BYTE _Y * _Y pSource, int iCount)
;               copies pDest to pSource including the NULL terminator, up to iCount characters.
;               if iCount characters is reached before the end, the NULL is not concatenated.
;  Inputs: 
;          BYTE _Y * _Y pDest   (r0):   unpacked buffer destination string
;          BYTE _Y * _Y pSource (r4):   unpacked buffer source string
;          count                (a1):   max characters to copy into pDest.
;  Outputs:
;  Notes:   ascii and Unicode-3 friendly.  
;<
;///////////////////////////////////////////////////////////////////////////////
strncpy
    push a1
_topofloop
    pop a
    tst a
    jeq _exit

    move y:(r4)+,b
    move b,y:(r0)+
    tst b
    jeq _exit

    move a1,a0
    dec a
    push a0

    jmp _topofloop
_exit
    rts

