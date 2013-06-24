    nolist
    include 'sysmacro.asm'
    include 'const.xref'
    list
    global  pack_string

    OPT     OP
    extern packed_set

    org p,"pack_string":
;///////////////////////////////////////////////////////////////////////////////
;> Name: pack_string
;
;  Type: Function
;  Description: _asmfunc pack_string(BYTE _Y * _Y pUnpacked, BYTE _Y * _Y pPacked)
;               
;  Inputs: 
;      BYTE _Y * _Y pUnpacked  (r0):   unpacked string to move into packed buffer pPacked
;  _packed BYTE _Y * _Y pPacked (r4):   pointer to packed buffer to move string to
;  Outputs:
;       none
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////
pack_string
    push    ssh
    move    r0,r5       ;put our unpacked string pointer in r5
    move    r4,r0       ;
    move    y:<Const_000000,a
    push    a1
_copybyte
    move    y:(r5),b    ;get our new char into b
    jsr     packed_set  ;set the packed string byte
    pop     a0          
    inc     a           ;increment our index into the packed string
    push    a0
    move    a0,a
    move    y:(r5)+,b   ;increment our index into the unpacked string (and get the byte to
    tst     b           ;see if we're done
    jne     _copybyte
    pop     a           ;pop off our iterator to balance the stack
    pop     ssh
    nop
    rts
