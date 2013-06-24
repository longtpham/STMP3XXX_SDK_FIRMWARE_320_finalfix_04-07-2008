    nolist
    include 'sysmacro.asm'
    include 'const.xref'

    list

    extern GetByteFromArrayInY

    global  packed_get

    org p,"packed_get":
;///////////////////////////////////////////////////////////////////////////////
;> Name: packed_get
;
;  Type: Function
;  Description: _asmfunc BYTE packed_get( _packed BYTE _Y * _Y p,int n);
;               returns the character at index N in a packed string pointed to by p
;  Inputs: 
;       _packed BYTE _Y * _Y p  (r0):   Packed string to extract character from
;       int n                   (a1):   index from which to extract    
;  Outputs:
;       a1                          :   character from the string.
;  Notes: 
;       packed_get maps directly to GetByteFromArrayInY
;<
;///////////////////////////////////////////////////////////////////////////////
packed_get equ GetByteFromArrayInY
