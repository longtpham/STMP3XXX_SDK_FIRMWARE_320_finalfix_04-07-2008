    nolist
    include 'sysmacro.asm'
    include 'const.xref'
    list

    global  unpack_data

    extern packed_get

    org p,"unpack_data":
;///////////////////////////////////////////////////////////////////////////////
;> Name: unpack_data
;
;  Type: Function
;  Description: _asmfunc unpack_data( BYTE _Y * _Y pPacked,  BYTE _Y * _Y pUnpacked, INT count)
;               This function works by moving each byte into a separate word.  
;
;  Inputs: 
;  _packed BYTE _Y * _Y pPacked   (r0):   pointer to packed buffer to move string from
;          BYTE _Y * _Y pUnpacked (r4):   unpacked buffer to place packed string
;          INT  count             (a1):   number of bytes to unpack
;  Outputs:
;       none
;  Notes: It is not DBCS aware.
;<
;///////////////////////////////////////////////////////////////////////////////
;
unpack_data
    push    ssh
    push    a1                   ; count
    move    y:<Const_000000,a    ;setup our iterator
    push    a
_copybyte    
    jsr     packed_get          ;get the byte
    move    a1,y:(r4)           ;put it in our unpacked string    
    pop     a0                  
    inc     a
    clr     b    y:(r4)+,y0     ; increment the target ptr
    pop     b0                  ; count
    dec     b                   ; decrement the counter
    push    b0
    push    a0
    move    a0,a
    tst     b                   ;see if we're done
    jgt     _copybyte
    pop     a                   ;pop the iterator off the stack
    pop     b0
    pop     ssh
    nop                         
    rts
