    nolist
    include 'sysmacro.asm'
    include 'const.xref'
    list

    extern strncpy

    global  strcpy

    org p,"strcpy":
;///////////////////////////////////////////////////////////////////////////////
;> Name: strcpy
;
;  Type: Function
;  Description: ;_asmfunc void strcpy(BYTE _Y * _Y pDest, BYTE _Y * _Y pSource)
;               copies pDest to pSource including the NULL terminator, 
;
;  Inputs: 
;          BYTE _Y * _Y pDest   (r0):   unpacked buffer destination string
;          BYTE _Y * _Y pSource (r4):   unpacked buffer source string
;  Outputs:
;  Notes:   ascii and Unicode-3 friendly.  
;<
;///////////////////////////////////////////////////////////////////////////////
strcpy
    move    y:<Const_ffffff,a
    jmp     strncpy
