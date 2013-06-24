    nolist
    include 'sysmacro.asm'
    include 'const.xref'
    list

    global  packed_strncpy

    extern  packed_strncat

    org p,"packed_strncpy":
;///////////////////////////////////////////////////////////////////////////////
;> Name: packed_strncpy
;
;  Type: Function
;  Description: _asmfunc _packed BYTE _Y * _Y packed_strncpy( _packed BYTE _Y * _Y pDest, _packed BYTE _Y * _Y pSrc,int count)
;
;  Inputs: 
;       _packed BYTE _Y * _Y pDest  (r0):   Packed string to copy to
;       _packed BYTE _Y * _Y pSrc   (r4):   Packed string to copy from
;                      int   iCount (a1):   number of bytes to copy
;  Outputs:
;       r0                              :   pointer to the destination string
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////
packed_strncpy
    clr     b
    move    b,y:(r0)
    jmp     packed_strncat
