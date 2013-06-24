    nolist
    include 'sysmacro.asm'
    include 'const.xref'
    list

    global  unpack_string

    extern packed_get

    org p,"unpack_string":
;///////////////////////////////////////////////////////////////////////////////
;> Name: unpack_string
;
;  Type: Function
;  Description: _asmfunc unpack_string( BYTE _Y * _Y pPacked,  BYTE _Y * _Y pUnpacked)
;               This function works by moving each byte into a separate word.  
;
;  Inputs: 
;  _packed BYTE _Y * _Y pPacked   (r0):   pointer to packed buffer to move string from
;          BYTE _Y * _Y pUnpacked (r4):   unpacked buffer to place packed string
;  Outputs:
;       none
;  Notes: It is not DBCS aware.
;<
;///////////////////////////////////////////////////////////////////////////////
;
unpack_string
    push    ssh
    move    y:<Const_000000,a    ;setup our iterator
    push    a
_copybyte    
    jsr     packed_get          ;get the byte
    move    a1,y:(r4)           ;put it in our unpacked string    
    pop     a0                  
    inc     a
    push    a0
    move    a0,a
    move    y:(r4)+,b           ;increment our target pointer and 
    tst     b                   ;see if we're done
    jne     _copybyte
    pop     a                   ;pop the iterator off the stack
    pop     ssh
    nop                         
    rts
