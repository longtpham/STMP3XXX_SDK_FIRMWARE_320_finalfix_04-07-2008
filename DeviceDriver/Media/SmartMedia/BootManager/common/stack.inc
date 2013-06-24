;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2001
;
; Filename: stack.inc
; Description: Macros used for Software Stack (not Hardware System Stack)
;///////////////////////////////////////////////////////////////////////////////




;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           push
;
;   Type:           Macro
;
;   Description:    Push a 24-bit value onto the data stack
;
;   Inputs:         Usage : push reg
;                   reg = any core register that is 24-bits or less
;
;   Outputs:        none
;                   
;   Notes:          For every push, there must be a corresponding pop to keep
;                   data stack balanced.
;
;   SeeAlso:        pop
;<
;///////////////////////////////////////////////////////////////////////////////
push    macro   reg
    move    reg,x:(r7)+
    endm

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           pop
;
;   Type:           Macro
;
;   Description:    Pop a 24-bit value from the data stack
;
;   Inputs:         Usage : pop reg
;                   reg = any core register that is 24-bits or less
;
;   Outputs:        none
;                   
;   Notes:          For every pop, there must be a corresponding push to keep
;                   data stack balanced.
;
;   SeeAlso:        push
;<
;///////////////////////////////////////////////////////////////////////////////
pop     macro   reg
    move    x:-(r7),reg
    endm
