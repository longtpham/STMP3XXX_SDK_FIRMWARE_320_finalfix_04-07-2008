;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2004
;
;  Filename: project.inc
;  Description: 
;///////////////////////////////////////////////////////////////////////////////

    if (!@def(_PROJECT_INC))
_PROJECT_INC                equ     1


;///////////////////////////////////////////////////////////////////////////////
; BOOT SPEED CONFIG & ASSOCIATED BATTERY VOLTAGE CHECK ENABLE
;///////////////////////////////////////////////////////////////////////////////
; Users can enable one of ATTEMPT_FAST_BOOT or FASTEST boot or neither project define below by defining
; values as 0 or 1. 0 results in the boot speed used in previous SDKs.
ATTEMPT_FAST_BOOT    equ 0
ATTEMPT_FASTEST_BOOT equ 0   
; Changing this define to 0 removes Vbat threshholding & unconditionally gives fast boot config above- 
; Also the tests should clear the fast boot config defines to link & boot at normal speed.
SPEED_BOOT_BATTERY_CHECK    equ 0










   endif ; if (!@def(_PROJECT_INC))

