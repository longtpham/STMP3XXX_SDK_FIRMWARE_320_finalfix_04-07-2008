;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000
;
; System Resource stuff
;///////////////////////////////////////////////////////////////////////////////

    IF (!@def(SYSRESOURCES_INC))
SYSRESOURCES_INC equ 1

;///////////////////////////////////////////////////////////////////////////////
; Resource Structs
;///////////////////////////////////////////////////////////////////////////////

; Standard resource header
RSRC_HEADER_SIZE                equ     3
RSRC_HEADER_RESOURCE_NUMBER     equ     0
RSRC_HEADER_BYTE_COUNT          equ     1
RSRC_HEADER_RESOURCE_TYPE       equ     2

; Button resource
RSRC_BUTTON_NUM_MSGS            equ     0
RSRC_BUTTON_START_MSGS          equ     RSRC_BUTTON_NUM_MSGS+1

; Button timing resource
RSRC_BUTTON_TIMING_SIZE         equ     2
RSRC_BUTTON_TIMING_START        equ     0
RSRC_BUTTON_TIMING_DEBOUNCE     equ     0
RSRC_BUTTON_TIMING_HOLD         equ     RSRC_BUTTON_TIMING_DEBOUNCE+1

; ButtonPhysicalMap resource
RSRC_BUTTON_PHYS_MAP_NUM_ROWS   equ     0
RSRC_BUTTON_PHYS_MAP_OFF_ROWS   equ     RSRC_BUTTON_PHYS_MAP_NUM_ROWS+1
RSRC_BUTTON_PHYS_MAP_NUM_COLS   equ     RSRC_BUTTON_PHYS_MAP_OFF_ROWS+1
RSRC_BUTTON_PHYS_MAP_OFF_COLS   equ     RSRC_BUTTON_PHYS_MAP_NUM_COLS+1
RSRC_BUTTON_PHYS_MAP_OFF_BNUM   equ     RSRC_BUTTON_PHYS_MAP_OFF_COLS+1
RSRC_BUTTON_PHYS_MAP_NUM_NSBS   equ     RSRC_BUTTON_PHYS_MAP_OFF_BNUM+1
RSRC_BUTTON_PHYS_MAP_OFF_NSBS   equ     RSRC_BUTTON_PHYS_MAP_NUM_NSBS+1
RSRC_BUTTON_PHYS_MAP_OFF_NSBN   equ     RSRC_BUTTON_PHYS_MAP_OFF_NSBS+1
RSRC_BUTTON_PHYS_MAP_HOLD_SPT   equ     RSRC_BUTTON_PHYS_MAP_OFF_NSBN+1
RSRC_BUTTON_PHYS_MAP_HOLD_MASK  equ     RSRC_BUTTON_PHYS_MAP_HOLD_SPT+1

; ButtonEventMap resource
RSRC_BUTTON_EVENT_MAP_NUM_EVNT  equ     0

; Timer resource
RSRC_TIMER_SIZE                 equ     5
RSRC_TIMER_START                equ     0
RSRC_TIMER_NUMBER               equ     0
RSRC_TIMER_COUNT_VALUE          equ     RSRC_TIMER_NUMBER+1
RSRC_TIMER_CONTROL_VALUE        equ     RSRC_TIMER_COUNT_VALUE+1
RSRC_TIMER_CONTROL_REG          equ     RSRC_TIMER_CONTROL_VALUE+1
RSRC_TIMER_COUNT_REG            equ     RSRC_TIMER_CONTROL_REG+1

; Interrupt resource
RSRC_INTERRUPT_SIZE                     equ     12
RSRC_INTERRUPT_START                    equ     0
RSRC_INTERRUPT_ICL_EN                   equ     0
RSRC_INTERRUPT_ICL_EN_MASK              equ     RSRC_INTERRUPT_ICL_EN+1
RSRC_INTERRUPT_ICL_PRIOR                equ     RSRC_INTERRUPT_ICL_EN_MASK+1
RSRC_INTERRUPT_ICL_PRIOR_MASK           equ     RSRC_INTERRUPT_ICL_PRIOR+1
RSRC_INTERRUPT_ICL_PRIOR_CLEAR_MASK     equ     RSRC_INTERRUPT_ICL_PRIOR_MASK+1
RSRC_INTERRUPT_ICL_STEER                equ     RSRC_INTERRUPT_ICL_PRIOR_CLEAR_MASK+1
RSRC_INTERRUPT_ICL_STEER_MASK           equ     RSRC_INTERRUPT_ICL_STEER+1
RSRC_INTERRUPT_ICL_STEER_CLEAR_MASK     equ     RSRC_INTERRUPT_ICL_STEER_MASK+1
RSRC_INTERRUPT_ICL_STATUS               equ     RSRC_INTERRUPT_ICL_STEER_CLEAR_MASK+1
RSRC_INTERRUPT_ICL_STATUS_MASK          equ     RSRC_INTERRUPT_ICL_STATUS+1
RSRC_INTERRUPT_VECTOR_ADDRESS           equ     RSRC_INTERRUPT_ICL_STATUS_MASK+1
RSRC_INTERRUPT_ISR_ADDRESS              equ     RSRC_INTERRUPT_VECTOR_ADDRESS+1


;///////////////////////////////////////////////////////////////////////////////
; Resource Types
;///////////////////////////////////////////////////////////////////////////////
RSRC_TYPE_NULL                  equ     $000000
RSRC_TYPE_STRING                equ     $000001
RSRC_TYPE_DATA                  equ     $000002
RSRC_TYPE_BITMAP                equ     $000003
RSRC_TYPE_BUTTON                equ     $000004
RSRC_TYPE_BUTTON_TIMING         equ     $000005
RSRC_TYPE_TIMER                 equ     $000006
RSRC_TYPE_INTERRUPT             equ     $000007
RSRC_TYPE_SYSTEM_INTERRUPT      equ     $000008
RSRC_TYPE_NESTED_RESOURCE       equ     $000009

;///////////////////////////////////////////////////////////////////////////////
; Pre-defined resources
;///////////////////////////////////////////////////////////////////////////////
RSRC_NULL                       equ     $000000     ; RSRC_TYPE_NULL


    ENDIF ;IF (!@def(SYSRESOURCES_INC))
