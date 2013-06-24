    nolist
    include 'sysmacro.asm'
    include 'const.xref'
    list

    extern strncat

    global  strcat


    org p,"strcat":
;///////////////////////////////////////////////////////////////////////////////
;> Name: strcat
;
;  Type: Function
;  Description: _asmfunc BYTE _Y * _Y strncat(BYTE _Y * _Y pLeft, BYTE _Y * _Y pRight, int count)
;               appends  pRight to pLeft, and adds the terminating null.
;
;  Inputs: 
;          BYTE _Y * _Y pDest   (r0):   unpacked buffer destination string
;          BYTE _Y * _Y pSource (r4):   unpacked buffer source string
;  Outputs:
;           (r0)                    :   pointer to pLeft
;  Notes:   ascii and Unicode-3 friendly.  
;           Does not create a new buffer
;           uses strncat to accomplish its job.
;<
;///////////////////////////////////////////////////////////////////////////////
strcat
    move    y:<Const_ffffff,a
    jmp     strncat
