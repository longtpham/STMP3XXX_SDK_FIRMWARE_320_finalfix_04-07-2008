;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2005
;
; Filename: systimers.inc
; Description: Equates for systimers
;///////////////////////////////////////////////////////////////////////////////


 IF (!@DEF(SYSTIMERS_INC))
SYSTIMERS_INC   equ 1

;Equates used        
DECODER_TIMER_NUM   equ HW_TIMER_NUMBER_0
BUTTON_TIMER_NUM    equ HW_TIMER_NUMBER_1
SYS_TIMER_NUM       equ HW_TIMER_NUMBER_2

 ENDIF   ;IF (!@DEF(SYSTIMERS_INC))
