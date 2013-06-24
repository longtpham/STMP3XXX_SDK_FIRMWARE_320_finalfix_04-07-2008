    nolist
    include 'sysmacro.asm'
    include 'const.xref'
    list

    global  strlen

    org p,"strlen":
;///////////////////////////////////////////////////////////////////////////////
;> Name: strlen
;
;  Type: Function
;  Description: _asmfunc int strlen(BYTE _Y * _Y pUnpacked)
;               returns length of string (in characters)
;
;  Inputs: 
;          BYTE _Y * _Y pUnpacked (r0):   unpacked buffer to place packed string
;  Outputs:
;        (a1)      : length of string.
;  Notes:   ascii and Unicode-3 friendly.  
;<
;///////////////////////////////////////////////////////////////////////////////
strlen
    move y:<Const_ffffff,a0  ;put our counter at -1 (so the first inc puts it at 0)
_topofloop
    inc     a               ;increment our counter
    move    y:(r0)+,b       ;get the value
    tst b                   ;see if its zero
    jne _topofloop          ;continue if not
    move a0,a
    rts
