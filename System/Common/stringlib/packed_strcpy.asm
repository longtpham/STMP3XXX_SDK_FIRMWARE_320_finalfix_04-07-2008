    nolist
    include 'sysmacro.asm'
    include 'const.xref'
    list

    global  packed_strcpy

    extern  packed_strcat

    org p,"packed_strcpy":
;///////////////////////////////////////////////////////////////////////////////
;> Name: packed_strcpy
;
;  Type: Function
;  Description: _asmfunc _packed BYTE _Y * _Y packed_strcpy( _packed BYTE _Y * _Y pDest, _packed BYTE _Y * _Y pSrc)
;
;  Inputs: 
;       _packed BYTE _Y * _Y pDest  (r0):   Packed string to copy to
;       _packed BYTE _Y * _Y pSrc   (r4):   Packed string to copy from
;  Outputs:
;       r0                              :   pointer to the destination string
;  Notes: 
;       This will always null terminate the destination
;<
;///////////////////////////////////////////////////////////////////////////////
packed_strcpy
    clr     b                   ;make the dest string 0
    move    b,y:(r0)            
    jmp     packed_strcat       ;cat the source string onto the destination string
