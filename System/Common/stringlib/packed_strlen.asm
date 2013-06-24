    nolist
    include 'sysmacro.asm'
    include 'const.xref'
    list

    global  packed_strlen

    OPT     OP

    extern  packed_get

    org p,"packed_strlen":
;///////////////////////////////////////////////////////////////////////////////
;> Name: packed_strlen
;
;  Type: Function
;  Description: _asmfunc int packed_strlen( _packed BYTE _Y * _Y p)

;  Inputs: 
;       _packed BYTE _Y * _Y p  (r0):   Packed string to get length of
;  Outputs:
;       a1                          :   length of string pointed to by p
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////
packed_strlen
    push    ssh
    move    y:<Const_000000,r6
_top    
    move    r6,a
    jsr     packed_get
    tst     a
    jeq     _exit
    lua     (r6)+,r6
    jmp     _top 
_exit
    pop     ssh
    move    r6,a
    rts
