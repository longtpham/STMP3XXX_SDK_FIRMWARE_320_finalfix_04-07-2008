;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2001
;
; Filename: systimers.inc
; Description: Equates for systimers
;///////////////////////////////////////////////////////////////////////////////


 IF (!@DEF(SYSTIMERS_INC))
SYSTIMERS_INC   equ 1

;HW_TIMER_NUMBER_0                       equ     0
;HW_TIMER_NUMBER_1                       equ     1
;HW_TIMER_NUMBER_2                       equ     2
;HW_TIMER_NUMBER_3                       equ     3

;    global HW_TIMER_NUMBER_0
;    global HW_TIMER_NUMBER_1
;    global HW_TIMER_NUMBER_2        
;    global DECODER_TIMER_NUM
;    global BUTTON_TIMER_NUM
;    global SYS_TIMER_NUM

;Equates used        
DECODER_TIMER_NUM   equ HW_TIMER_NUMBER_0
BUTTON_TIMER_NUM    equ HW_TIMER_NUMBER_1
SYS_TIMER_NUM       equ HW_TIMER_NUMBER_2

 ENDIF   ;IF (!@DEF(SYSTIMERS_INC))
