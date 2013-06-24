    nolist
    include 'sysmacro.asm'
    include 'const.xref'
    list

    global  packed_strcat

    extern  packed_strncat

    org p,"packed_strcat":
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
;  Outputs:
;           (r0)                    :   pointer to pDest
;  Notes:   Does not create a new buffer
;<
;///////////////////////////////////////////////////////////////////////////////
packed_strcat
    move    y:<Const_ffffff,a
    jmp     packed_strncat
