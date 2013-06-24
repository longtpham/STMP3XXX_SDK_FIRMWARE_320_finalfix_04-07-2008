;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000
;
; Buttons        
;///////////////////////////////////////////////////////////////////////////////

    IF (!@def(BUTTONS_INC))
BUTTONS_INC equ 1

; TODO - abstract this for changes in PLL frequency.
; 
BUTTON_TIMER_COUNT        equ    $0C3500		;Systimers.asm

;BTL_BUTTON_TABLE_SIZE     equ    50				;button.asm
;BTL_BUTTON_PHYS_MAP_SIZE  equ    57				;button.asm
;BTL_BUTTON_EVENT_MAP_SIZE equ    23				;button.asm

; Button Flags
;BUTTON_ON_OFF       equ     0

; Button IDs
;BUTTON_NOT_FOUND    equ     $0000


; Button Event IDs
;BUTTON_EVENT_00     equ     $0000
;BUTTON_EVENT_01     equ     $0001
;BUTTON_EVENT_02     equ     $0002
;BUTTON_EVENT_03     equ     $0003
;BUTTON_EVENT_04     equ     $0004
;BUTTON_EVENT_05     equ     $0005
;BUTTON_EVENT_06     equ     $0006
;BUTTON_EVENT_07     equ     $0007
;BUTTON_EVENT_08     equ     $0008
;BUTTON_EVENT_09     equ     $0009
;BUTTON_EVENT_10     equ     $000A
;BUTTON_EVENT_11     equ     $000B
;BUTTON_EVENT_12     equ     $000C
;BUTTON_EVENT_13     equ     $000D
;BUTTON_EVENT_14     equ     $000E
;BUTTON_EVENT_15     equ     $000F
;BUTTON_EVENT_16     equ     $0010
;BUTTON_EVENT_17     equ     $0011
;BUTTON_EVENT_18     equ     $0012
;BUTTON_EVENT_19     equ     $0013
;BUTTON_EVENT_20     equ     $0014
;BUTTON_EVENT_21     equ     $0015
;BUTTON_EVENT_22     equ     $0016

; Last Event supported
;LAST_BUTTON_EVENT_SUPPORTED	equ	BUTTON_EVENT_19

    ENDIF ;IF (!@def(BUTTONS_INC))
