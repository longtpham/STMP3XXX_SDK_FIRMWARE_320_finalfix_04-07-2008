    nolist
    include 'sysmacro.asm'
    include 'const.xref'
    list

    OPT OP
    
    global  strncat

    org p,"strncat":
;///////////////////////////////////////////////////////////////////////////////
;> Name: strncat
;
;  Type: Function
;  Description: _asmfunc BYTE _Y * _Y strncat(BYTE _Y * _Y pLeft, BYTE _Y * _Y pRight, int count)
;               appends up to Count characters of pRight to pLeft, and adds the terminating null.
;
;  Inputs: 
;          BYTE _Y * _Y pDest   (r0):   unpacked buffer destination string
;          BYTE _Y * _Y pSource (r4):   unpacked buffer source string
;           count               (a1):   maximum number of characters plus null to copy            
;  Outputs:
;           (r0)                    :   pointer to pLeft
;  Notes:   ascii and Unicode-3 friendly.  
;           Does not create a new buffer
;           Currently, does not add the terminating NULL if count runs out!
;<
;///////////////////////////////////////////////////////////////////////////////
strncat
    push    r0
_cyclethroughleftstring
    move    y:(r0)+,b      ;get each word
    tst     b               ;see if its the null terminator
    jne     _cyclethroughleftstring
    move    a1,b 
    clr     a       y:(r0)-    ;back up the pointer
    move    b,a0
_cyclethroughrightstring
    tst     a
    jle     _exit
    dec     a
    move    y:(r4)+,b      
    move    b,y:(r0)+
    tst     b 
    jne     _cyclethroughrightstring
_exit
    pop     r0              ;get the return value back
    rts
