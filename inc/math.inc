;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000
;
; Math defs        
;///////////////////////////////////////////////////////////////////////////////

    IF (!@def(MATH_INC))
MATH_INC equ 1

; Fractional notation:
;   bbsii
;       bb = number of bits
;       s
;           Q = signed
;           U = unsigned
;       ii = number of integer bits
;       

F24Q00_ONE_THIRD    equ     $2AAAAB     ; roundup[(1/3)*(2^(24-1))]

    ENDIF ; IF (!@def(MATH_INC))

