    nolist
    include 'sysmacro.asm'
    include 'const.xref'
    list

    OPT OP

    global  strncmp

    org p,"strncmp":
;///////////////////////////////////////////////////////////////////////////////
;> Name: strncmp
;
;  Type: Function
;  Description: _asmfunc BYTE _Y * _Y strncat(BYTE _Y * _Y pLeft, BYTE _Y * _Y pRight, int count)
;               compares pLeft to pRight, up to count characters.  
;
;  Inputs: 
;          BYTE _Y * _Y pDest   (r0):   unpacked buffer destination string
;          BYTE _Y * _Y pSource (r4):   unpacked buffer source string
;           count               (a1):   maximum number of characters plus null to copy            
;  Outputs:
;           (a1)                    :   <0 if pLeft is "less than" pRight in the ascii collating sequence
;                                       0 if the strings are equal
;                                       >0 if pLeft is "greater than" pRight in the ascii collating sequence
;  Notes:   ascii and Unicode-3 friendly.  
;           Does not create a new buffer
;<
;///////////////////////////////////////////////////////////////////////////////
strncmp
    move    a1,a                ;clear out a
    move    y:<Const_000001,x1
_topofloop
    clr     b
    tst     a                   ;see if we're done counting
    jeq     _exit               ;if not, go to the next character
    move    y:(r0)+,x0          ;get the two values
    move    y:(r4)+,y0
    add     x0,b                ;add the left to b
    sub     y0,b                ;subtract the right
    tst     b                   ;if its not zero, the strings aren't equal
    jne     _exit               ;so lets exit
    move    x0,b                ;if we've gotten here, both characters are identical    
    tst     b                   ;lets just check one and see if its a null
    jeq     _exit               ;if so, we're at the end of the string, exit.
    sub     x1,a                ;decrement the count
    jmp     _topofloop
_exit
    tfr     b,a                 ;lets put the value in b (-,0,+) in A for return value
    rts
