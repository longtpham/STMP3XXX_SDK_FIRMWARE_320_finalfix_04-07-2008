;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2001
;
;  File        : utilasm.asm
;  Description : ASM support functions for all tests
;///////////////////////////////////////////////////////////////////////////////
        section TestUtil
;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////
    nolist
    include "sysequ.inc"                  
    include "const.xref"
    include "systimers.inc"
    include "regstimer.inc"
    list

;*******************************************************************************
;  Macros
;*******************************************************************************

    include "sysmacro.asm"

;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////
    global  VERSION_MAJOR
    global  VERSION_MIDDLE
    global  VERSION_MINOR

    global  modulo
    global  Exec
    global  EmptyFunction
    global  FUserTask

    global  SysBrownOutIsr
    global  SysHeadPhoneShortIsr
    global  HeadPhoneShortStatus
    global  BrownoutStatus

    ; The constants below in systimers.inc, are referenced as extern by systimers.xref.
    ; Therefore, they are made global here to allow the locater to resolve the references
    global  DECODER_TIMER_NUM
    global  BUTTON_TIMER_NUM 
    global  SYS_TIMER_NUM  
    global  HEADPHONE_SHORT_STATUS_BIT

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////
    extern  Fmain
    extern  L1DataStack
    extern  SysAllLinear
    extern  divide

;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////
VERSION_MAJOR       equ     0
VERSION_MIDDLE      equ     0
VERSION_MINOR       equ     1

HEADPHONE_SHORT_STATUS_BIT  equ 0


;///////////////////////////////////////////////////////////////////////////////
;   P Memory
;///////////////////////////////////////////////////////////////////////////////
    org     p,"EXEC_Vectors_P":

    jmp     Exec



    org     p,"TestUtil_P":
    
;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           Empty
;
;   Type:           Function
;
;   Description:    Empty functions needed by the build
;
;   Inputs:         none
;
;   Outputs:        none
;                   
;   Notes:          none
;<
;///////////////////////////////////////////////////////////////////////////////
EmptyFunction
SysBrownOutIsr
SysHeadPhoneShortIsr
    rts

;///////////////////////////////////////////////////////////////////////////////
; modulo
;///////////////////////////////////////////////////////////////////////////////
modulo EQU divide

FUserTask
    dc 0	
;///////////////////////////////////////////////////////////////////////////////
; Exec
;///////////////////////////////////////////////////////////////////////////////
Exec:
    ; ReEnable ONCE for 3410/3500 debug build. Allows improved debugging 
    ; of 3410/3500. In debugger, type reset and go instead of reloading abs repeatedly. 
    ;
    if (@def('DEBUG')) ; Better to ReadModifyWrite.
     move    #>$ff8fff,x0                    
     move    x0,x:$fffe
    endif
    
    move    #L1DataStack,r7
    move    #-1,m7
    jsr     SysAllLinear
    
;;    if (@def('DEVICE_3400')||@def('DEVICE_3300'))
;;    ; Do nothing
;;    else
;;    nolist
;;    include "regs3410.inc"
;;    list
;;      
;;    move    #-1,x0      ;Power Up GPIO pads
;;    move    x0,x:HW_GP0PWR
;;    move    x0,x:HW_GP1PWR
;;    move    x0,x:HW_GP2PWR
;;		
;;    endif

    jsr Fmain
    rts	

 
;///////////////////////////////////////////////////////////////////////////////
; Y
;///////////////////////////////////////////////////////////////////////////////

    org    y,"TestUtil_Y":

HeadPhoneShortStatus
BrownoutStatus
        dc  0
    
    endsec    
