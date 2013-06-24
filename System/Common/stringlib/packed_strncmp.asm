    nolist
    include 'sysmacro.asm'
    include 'const.xref'
    list

    OPT OP

    extern GetByteFromArrayInY

    global  packed_strncmp

    org p,"packed_strncmp":
;///////////////////////////////////////////////////////////////////////////////
;> Name: strncmp
;
;  Type: Function
;  Description: _asmfunc int strncmp(_packed BYTE _Y * _Y pLeft, _packed BYTE _Y * _Y pRight, int count)
;               compares pLeft to pRight, up to count characters.  
;
;  Inputs: 
;          _packed BYTE _Y * _Y pDest   (r0):   packed buffer destination string
;          _packed BYTE _Y * _Y pSource (r4):   packed buffer source string
;           count               (a1):   maximum number of characters plus null to compare
;  Outputs:
;           (a1)                    :   <0 if pLeft is "less than" pRight in the ascii collating sequence
;                                       0 if the strings are equal
;                                       >0 if pLeft is "greater than" pRight in the ascii collating sequence
;  Notes:   ascii friendly.  
;
;<
;///////////////////////////////////////////////////////////////////////////////
packed_strncmp
    push    ssh
    move    a1,r2               ;save off the count

    move    r0,r1               ;put the pLeft into r1
    move    y:<Const_000000,r3  ;current index.
_topofloop
    move    r2,x0               ;see if we're at our terminal count
    move    r3,a                
    cmp     x0,a
    jge     _exit
    ;get the right
    move    r4,r0
    move    r3,a
    jsr     GetByteFromArrayInY
    push    a1
    ;get the right
    move    r1,r0
    move    r3,a
    jsr     GetByteFromArrayInY
    pop     x0                   
    sub     x0,a
    jne     _exit
    move    x0,b
    tst     b
    jeq     _exit
    lua    (r3)+,r3
    move    r2,b
    tst     b
    jeq     _exit
    jmp     _topofloop
_exit
    pop     ssh
    nop
    rts
