    nolist
    include 'sysmacro.asm'
    include 'const.xref'
    list

    global  packed_strncat

    extern  packed_strlen
    extern  packed_get
    extern  packed_set

    OPT OP

    org p,"packed_strncat":
;///////////////////////////////////////////////////////////////////////////////
;> Name: strncat
;
;  Type: Function
;  Description: _asmfunc _packed BYTE _Y * _Y packed_strncat(_packed BYTE _Y * _Y pDest, _packed BYTE _Y * _Y pSource, int count)
;               appends up to Count characters of pRight to pLeft, and adds the terminating null.
;
;  Inputs: 
;          _packed BYTE _Y * _Y pDest   (r0):   packed buffer destination string
;          _packed BYTE _Y * _Y pSource (r4):   packed buffer source string
;           count                       (a1):   maximum number of characters plus null to copy            
;  Outputs:
;           (r0)                    :   pointer to pDest
;  Notes:   Does not create a new buffer
;           Currently, does not add the terminating NULL if count runs out!
;<
;///////////////////////////////////////////////////////////////////////////////
packed_strncat
    push    SSH 
    push    r0
    push    r4
    push    a1
    jsr     packed_strlen
    pop     r3          ;count
    pop     r4          ;pSource
    pop     r1          ;pDest
    move    a1,r2       ;pDest index
    move    y:<Const_000000,r5 ;pSource Index
_copy
    move    r3,a        ;see if we have more to copy
    tst     a
    jeq     _exit
    move    r4,r0       ;get the source
    move    r5,a
    jsr     packed_get
    tfr     a,b
    push    b
    move    r2,a        ;put it into the destination
    move    r1,r0
    jsr     packed_set
    lua     (r3)-,r3    ;decrement the count
    lua     (r2)+,r2    ;increment the destination index
    lua     (r5)+,r5    ;increment the source index
    pop     b
    tst     b
    jne     _copy
_exit    
    pop     SSH
    move    r1,r0       ;copy pDest back to r0
    rts
