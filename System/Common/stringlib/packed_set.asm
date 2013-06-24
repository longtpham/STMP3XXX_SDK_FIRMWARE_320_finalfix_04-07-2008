    nolist
    include 'sysmacro.asm'
    include 'const.xref'

    list

    global  packed_set

    extern PutByteInArrayInY

    org p,"packed_set":
;///////////////////////////////////////////////////////////////////////////////
;> Name: packed_set
;
;  Type: Function
;  Description: _asmfunc packed_set( _packed BYTE _Y * _Y p,int n, int newchar);
;               sets the character at position N in packed string p, to newchar
;               a wrapper around the assembly function CP_SetByte
;  Inputs: 
;       _packed BYTE _Y * _Y p  (r0):   Packed string to set character
;       int n                   (a1):   index from where to place character
;       int newchar             (b1):   character to place in new string
;  Outputs:
;  Notes:   packed_set is only a pointer to PutByteInArrayInY
;<
;///////////////////////////////////////////////////////////////////////////////
packed_set  equ     PutByteInArrayInY
