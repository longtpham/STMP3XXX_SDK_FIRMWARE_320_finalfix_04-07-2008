;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2001
;
; Filename:         const.asm
; Description:      Short Memory Constants
;                   This file contains allocation of commonly used constants.  They are placed
;                   at y:$0 by an absolute origin statement.  This ensures they are lolcated in
;                   short dsp memory.  Any portion of code using the constants listed in this
;                   file should use the following method:
;
;                   move y:<Const_00ffff,m0  ; uses one dsp program word
;
;                   instead of
;
;                   move #$ffffff,m0        ; uses two dsp program words
;
; 
;                   It is a requirement that the "<" short operator be used to force short
;                   addressing.  The org statement should not use a memory counter, but should
;                   be absolutely set to 0.  This will put one more trigger in place to ensure
;                   the constants stay in short memory.
;
;                   The section should not exceed 32 words in size.
;
;                   The full list of constants defined in short memory are:
;
;                   Const_000000
;                   Const_000001
;                   Const_000002
;                   Const_000004
;                   Const_000008
;                   Const_000010
;                   Const_000020
;                   Const_000040
;                   Const_000080
;                   Const_000100
;                   Const_000200
;                   Const_000400
;                   Const_000800
;                   Const_001000
;                   Const_002000
;                   Const_004000
;                   Const_008000
;                   Const_010000
;                   Const_020000
;                   Const_040000
;                   Const_080000
;                   Const_100000
;                   Const_200000
;                   Const_400000
;                   Const_800000
;                   Const_000003 
;                   Const_000005 
;                   Const_00000c 
;                   Const_000018 
;                   Const_000fff
;                   Const_7fffff 
;                   Const_ffffff 
;                   Fconst_zero   
;                   const_zero    
;                   Fconst_one    
;                   const_one     
;                   Fconst_two    
;                   const_two     
;                   Fconst_three  
;                   const_three   
;                   Fconst_four   
;                   const_four    
;                   Fconst_eight  
;                   const_eight   
;                   Fconst_12
;                   const_12      
;                   Fconst_24
;                   const_24      
;                   Fconst_32     
;                   const_32      
;                   Fconst_minus_1
;                   const_minus_1 
;                   Ffract_one    
;                   fract_one     
;
;///////////////////////////////////////////////////////////////////////////////

    section     GLOBAL_CONSTANTS

;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

    GLOBAL  BitShiftTableStart
    GLOBAL  BitShiftTableEnd
    GLOBAL  Const_000000
    GLOBAL  Const_000001
    GLOBAL  Const_000002
    GLOBAL  Const_000004
    GLOBAL  Const_000008
    GLOBAL  Const_000010
    GLOBAL  Const_000020
    GLOBAL  Const_000040
    GLOBAL  Const_000080
    GLOBAL  Const_000100
    GLOBAL  Const_000200
    GLOBAL  Const_000400
    GLOBAL  Const_000800
    GLOBAL  Const_001000
    GLOBAL  Const_002000
    GLOBAL  Const_004000
    GLOBAL  Const_008000
    GLOBAL  Const_010000
    GLOBAL  Const_020000
    GLOBAL  Const_040000
    GLOBAL  Const_080000
    GLOBAL  Const_100000
    GLOBAL  Const_200000
    GLOBAL  Const_400000
    GLOBAL  Const_800000
    GLOBAL  Const_000003 
    GLOBAL  Const_000005 
    GLOBAL  Const_00000c 
    GLOBAL  Const_000018 
    GLOBAL  Const_000fff
    GLOBAL  Const_7fffff 
    GLOBAL  Const_ffffff 

    ; Parthus compatability
    GLOBAL  Fconst_zero   
    GLOBAL  const_zero    
    GLOBAL  Fconst_one    
    GLOBAL  const_one     
    GLOBAL  Fconst_two    
    GLOBAL  const_two     
    GLOBAL  Fconst_three  
    GLOBAL  const_three   
    GLOBAL  Fconst_four   
    GLOBAL  const_four    
    GLOBAL  Fconst_eight  
    GLOBAL  const_eight   
    GLOBAL  Fconst_12
    GLOBAL  const_12 
    GLOBAL  Fconst_16     
    GLOBAL  Fconst_24
    GLOBAL  const_24      
    GLOBAL  Fconst_32     
    GLOBAL  const_32      
    GLOBAL  Fconst_minus_1
    GLOBAL  const_minus_1 
    GLOBAL  Ffract_one    
    GLOBAL  fract_one     
    
;///////////////////////////////////////////////////////////////////////////////
;   X Memory
;///////////////////////////////////////////////////////////////////////////////

; Parthus compatability
;    GLOBAL F__x_size
;F__x_size      dc         XSIZE

;///////////////////////////////////////////////////////////////////////////////
;   Y Memory
;///////////////////////////////////////////////////////////////////////////////

    org y,"GLOBAL_CONSTANTS_Y":

BitShiftTableStart
                                        ; Const put to satisfy some table req.
Const_000000
Fconst_zero
const_zero      dc  0

Const_000001
Fconst_one
const_one       dc  1

Const_000002
Fconst_two
const_two       dc  2

Const_000004
Fconst_four
const_four      dc  4

Const_000008
Fconst_eight
const_eight     dc  8

Fconst_16	
Const_000010    dc  $000010

Const_000020
Fconst_32   
const_32        dc  32

Const_000040    dc  $000040

Const_000080    dc  $000080

Const_000100    dc  $000100

Const_000200    dc  $000200

Const_000400    dc  $000400

Const_000800    dc  $000800

Const_001000    dc  $001000

Const_002000    dc  $002000
Const_004000    dc  $004000
Const_008000    dc  $008000
Const_010000    dc  $010000

Const_020000    dc  $020000

Const_040000    dc  $040000

Const_080000    dc  $080000

Const_100000    dc  $100000

Const_200000    dc  $200000

Const_400000    dc  $400000

; The following label for indexing in reverse.
BitShiftTableEnd
Const_800000    dc  $800000
; End of bitShift table

Const_000003
Fconst_three
const_three     dc  3

Const_000005    dc  $000005

Const_00000c
Fconst_12
const_12        dc  12

Const_000018
Fconst_24
const_24        dc  24

Const_000fff    dc  $000fff

Const_ffffff
Fconst_minus_1
const_minus_1   dc  -1

Const_7fffff
Ffract_one
fract_one       dc  $7FFFFF

    comment ~
BitShiftTableStart
Const_000000    dc  $000000         ; Const put to satisfy some table req.
Const_000001    dc  $000001
Const_000002    dc  $000002
Const_000004    dc  $000004
Const_000008    dc  $000008
Const_000010    dc  $000010
Const_000020    dc  $000020
Const_000040    dc  $000040
Const_000080    dc  $000080
Const_000100    dc  $000100
Const_000200    dc  $000200
Const_000400    dc  $000400
Const_000800    dc  $000800
Const_001000    dc  $001000
Const_002000    dc  $002000
Const_004000    dc  $004000
Const_008000    dc  $008000
Const_010000    dc  $010000
Const_020000    dc  $020000
Const_040000    dc  $040000
Const_080000    dc  $080000
Const_100000    dc  $100000
Const_200000    dc  $200000
Const_400000    dc  $400000
; The following label for indexing in reverse.
BitShiftTableEnd
Const_800000    dc  $800000
; End of bitShift table

Const_000003    dc  $000003
Const_000005    dc  $000005
Const_00000c    dc  $00000C
Const_000018    dc  $000018
Const_000fff    dc  $000fff
Const_7fffff    dc  $7fffff
Const_ffffff    dc  $ffffff

; Parthus compatability
const_zero      equ Const_000000
Fconst_zero     equ Const_000000
Fconst_one      equ Const_000001
const_one       equ Const_000001
Fconst_two      equ Const_000002
const_two       equ Const_000002
Fconst_three    equ Const_000003
const_three     equ Const_000003
Fconst_four     equ Const_000003
const_four      equ Const_000003
Fconst_eight    equ Const_000008
const_eight     equ Const_000008
Fconst_minus_1  equ Const_ffffff
const_minus_1   equ Const_ffffff
Ffract_one      equ Const_7fffff
fract_one       equ Const_7fffff
Fconst_12       equ Const_00000c
const_12        equ Const_00000c
Fconst_24       equ Const_000018
const_24        equ Const_000018
Fconst_32       equ Const_000020
const_32        equ Const_000020

    ~
    endsec
