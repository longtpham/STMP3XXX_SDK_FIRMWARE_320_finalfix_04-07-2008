   SECTION Library_Divide
;****************************************************************
;  Motorola 56000 DSP - 7/5/00                      By Jose Pagan
;****************************************************************

;////////////////////////////////////////////////////////////////
;	Includes and external references
;////////////////////////////////////////////////////////////////

    nolist
    include "sysequ.inc"
    INCLUDE 'sysmacro.asm'
    include "const.xref"
    include "errors.inc"

    include "incr.asm"

    list
    page    132,60,0,0,0

;****************************************************************
; Symbol Exports
;****************************************************************
    GLOBAL divide

;****************************************************************
; Code
;****************************************************************
    org     P,"FilesysLibrary_p":

;****************************************************************
; This program will divide divisor (X1:X0) into dividend (A1:A0) and
; store the result in the accumulator (A1:A0).
; The remainder is discarded.
; 48 bit divide routione will not check for 24 bits...
;****************************************************************
; Routine to find the result of a division & truncate the remainder.
;****************************************************************
;New divide. Feel free to fix any bugs; Raks

divide:
   push b0
   push b1
   push b2
   push x0
   push x1
   push y0
   push y1
   push r0
   push r1
   push n1
   push r2
   push n2
; Old Stack was replaced...
;   jsr   push_b
;   jsr push_x
;   jsr push_y
;   jsr push_r1
;  jsr   push_r2

    clr   b                   ; 
    cmp   a,b   y:<Const_000000,r1           ;Check for zero denominator
    jeq   DIV_ZERO_DENOM        ;

    cmp   x0,b y:<Const_000000,r0            ;check for zero numertor
    jeq   DIV_ZERO_NUM      ;

    move  x0,b0               ;Move the numerator into x0


   DIV_ALIGN_NUM:
    asl   b                           ;Align the number in the b register
    jset  #0,b1,DIV_ALIGN_NUM_DONE  ;
    incr  r1                               ;
    jmp   DIV_ALIGN_NUM             ;

   DIV_ALIGN_NUM_DONE:
    asr   b                            ;
    move  a,a0          
   DIV_ALIGN_DENOM:                 ;Align the denominator
    asl   a                         ;
    jset  #0,a1,DIV_ALIGN_DENOM_DONE    ;
    incr  r0                               ;
    jmp   DIV_ALIGN_DENOM             ;

   DIV_ALIGN_DENOM_DONE:
   asr   a                       ;
    move  a0,y0                  ;
                               ;Check the scale values. If the scale for 
   move  r0,a                    ;the Num > the denom then you cannot divide
    move  r1,x1                 ;denom is bigger than num
    sub   x1,a                  ;
    jmi   DIV_DENOM_TOO_BIG    ;

    move  a1,r2
   clr   a
    move  b0,y1
    incr  r2
    clr   b
    move  y1,b0
    move  y:<Const_000000,y1

    do r2,DIV_END_LONG_DIVIDE
       move  y0,a0
      sub   a,b
       jmi   DIV_SHIFT_AND_ADD_0
       jmp    DIV_SHIFT_AND_ADD_1

      DIV_SHIFT_AND_ADD_0:
      add   a,b
       asr   a
       move  a0,y0
       move  y1,a0
       asl   a
       move  a0,y1
       jmp   DIV_PRE_END_LONG_DIVIDE
      DIV_SHIFT_AND_ADD_1:

      asr   a               ;
       move a0,y0           ;
       move y1,a0           ;
       asl      a               ;
       inc      a               ;
       move a0,y1           ;
       jmp      DIV_PRE_END_LONG_DIVIDE
      DIV_PRE_END_LONG_DIVIDE:
       nop
   DIV_END_LONG_DIVIDE:
    
    
    move    r1,a
    tst     a
    jeq     _Do_Not_Rep

   move  r1,x1
    rep   x1
    asr   b

_Do_Not_Rep
    move  b0,a
    move  y1,a0
    jmp   DIV_ON_COMPLETION

   DIV_DENOM_TOO_BIG:
   move  x0,a               ;Set the remainder in place and it's done
   jmp   DIV_ON_COMPLETION  ;
   DIV_ZERO_DENOM:
   move  #ERRZeroDivide,a
   jmp   DIV_ON_COMPLETION
   DIV_ZERO_NUM:
   move  y:<Const_000000,a0
   move  y:<Const_000000,a1
   jmp   DIV_ON_COMPLETION
                                    
   DIV_ON_COMPLETION:
   pop n2
   pop r2
   pop n1
   pop r1
   pop r0
   pop y1
   pop y0
   pop x1
   pop x0
   pop b2
   pop b1
   pop b0

; Old Stack was replaced...
;           jsr     pop_r2
;           jsr     pop_r1
;           jsr     pop_y
;           jsr     pop_x
;           jsr     pop_b
            rts 

;****************************************************************
; End of Sample Program
;****************************************************************
   ENDSEC
