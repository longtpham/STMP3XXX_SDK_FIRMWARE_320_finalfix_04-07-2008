    nolist
    include 'sysmacro.asm'
    include 'const.xref'
    list

    extern  packed_strncmp

    global  packed_strcmp

    org p,"packed_strcmp":
;///////////////////////////////////////////////////////////////////////////////
;> Name: strncmp
;
;  Type: Function
;  Description: _asmfunc int strcmp(BYTE _Y * _Y pLeft, BYTE _Y * _Y pRight)
    ;               compares pLeft to pRight.  
;
;  Inputs: 
;          _packed BYTE _Y * _Y pDest   (r0):   packed buffer to compare
;          _packed BYTE _Y * _Y pSource (r4):   packed buffer to compare
;
;  Outputs:
;           (a1)                    :   <0 if pLeft is "less than" pRight in the ascii collating sequence
;                                       0 if the strings are equal
;                                       >0 if pLeft is "greater than" pRight in the ascii collating sequence
;  Notes:   
;           uses packed_strncmp to accomplish its job.
;<
;///////////////////////////////////////////////////////////////////////////////
packed_strcmp
    move    y:<Const_ffffff,a
    jmp     packed_strncmp
