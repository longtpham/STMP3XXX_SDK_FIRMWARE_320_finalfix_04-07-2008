;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2001
;
;  File        :  sysheadphoneshort.asm
;  Description :  ?
;///////////////////////////////////////////////////////////////////////////////

    page    255,255,0

    section SYSFUN_HeadPhoneShort

;///////////////////////////////////////////////////////////////////////////////
;   Macros
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////
    
    nolist
    include "sysequ.inc"
    include "hwequ.inc"
    include "sysmacro.asm"
    include "const.xref"
    list
        
;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////
    
    global  SysHeadPhoneShortIsr
    global  CheckHeadPhoneShort

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////
    extern  SysGetCurrentTime
        
;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////

HEADPHONE_SHORT_STATUS_BIT   equ 0
HEADPHONE_SHORT_TIMEOUT_BIT  equ 1

;///////////////////////////////////////////////////////////////////////////////
;   X Memory
;///////////////////////////////////////////////////////////////////////////////
        
    org     x,"SYSFUN_HeadPhoneShort_X":

;///////////////////////////////////////////////////////////////////////////////
;   Y Memory
;///////////////////////////////////////////////////////////////////////////////
        
    org     y,"SYSFUN_HeadPhoneShort_Y":

HeadPhoneShortStatus    dc  0           ;Bit 0 -> 0 : HeadPhone OK
                                        ;         1 : HeadPhone Short
                                        ;Bit 1 -> 1 : Timing out before turning back on
HeadphoneShortTimestampA1 dc  0
HeadphoneShortTimestampA0 dc  0

    
;///////////////////////////////////////////////////////////////////////////////
;   P Memory
;///////////////////////////////////////////////////////////////////////////////

    org     p,"SYSFUN_HeadPhoneShort_P":

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysHeadPhoneShortIsr
;
;   Type:           Interrupt Service Routine
;
;   Description:    Checks if IRQA (headphone short) triggered and if so 
;                   sets a global flag to indicate the short to the main loop.
;
;                   Code ASSUMES mode 01 (hw powers down the amp) initialized
;                   in PowerUpHeadPhones()
;                   Otherwise, put amp into classA mode here
;
;   Inputs:         none 
;
;   Outputs:        ISR sets global flag y:HeadPhoneShortStatus
;                   
;   Notes:          This global flag is polled by the main routine's loop for 
;                   short circuit event handling to save power.
;<
;///////////////////////////////////////////////////////////////////////////////
SysHeadPhoneShortIsr
    ;=============================================================================
    ; NOTE: THIS CODE IS TO PROTECT ACCUMULATOR BIT CHECKS (i.e. jset, etc.) and 
    ; OTHER ACC FUNCTIONS. UNCOMMENT IF CODE IS ADDED THAT UTILIZES THE ACCUMULATOR.
    ;
    ; Clear the DM bit (double-precision multiply mode, bit 6) of the Mode Register
    ; This will be returned to its previous state when the RTI occurs (since the SR
    ; will be popped back off System Stack Low (SSL).
    ; Accumulator behavior is unpredictable when the DM bit is set.
    ; Next cycle after clearing it should NOT be an ALU instruction.
    ;andi #$BF,mr
    ; UNCOMMENT ABOVE LINE IF NECESSARY.
    ;============================================================================

    bset #HEADPHONE_SHORT_STATUS_BIT,y:HeadPhoneShortStatus
    ; small signal levels are still output when headphone is powered down
    ; also short circuit detection is independent for each channel 
    ; this silences the output until the headphone short has recovered 
    bset     #HW_MIXVOLUMER_MUTE_BITPOS,x:HW_MIXMASTERVR
	
;;;;;;;  if using manual mode (11) then uncomment this line
;    bclr    #HW_HPCTRL_HPCLASSAB_BITPOS,x:HW_HPCTRL

_Done
    rti


;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           CheckHeadPhoneShort
;
;   Type:           System Exec Function
;
;   Description:    This function checks a headphone short global flag set in  
;                   irqa isr. 
;
;                   Code ASSUMES mode 01 (hw powers down the amp) initialized
;                   in PowerUpHeadPhones()
;
;                   If irqa occurred start the following sequence:
;                       - put amp into class A mode drawing large current (pop bits cleared)
;                       - reset latch (00  HPHSHORT1:0) which repowers amp
;                       - re-enable hw pwrdown  (01  HPHSHORT1:0)
;                       - wait 400mS for amp to settle at vag 
;                       *** IMPT to wait at least 400mS or false detects can trigger
;                       ***    causing the amp to never recover
;                       - put amp into class AB mode
;
;                   Otherwise, if using mode 11 -- IRQA isr should put amp into class A mode
;                       and the code should be changed to skip changing (HPHSHORT1:0) 
;                       -- see block of code to remove
;
;
;   Inputs:         y:HeadPhoneShortStatus global set by ISR, cleared here.
;                       HEADPHONE_SHORT_STATUS_BIT
;                   y:HeadPhoneShortStatus global set here to start timeout, cleared here.
;                       HEADPHONE_SHORT_TIMEOUT_BIT
;
;   Outputs:        y:HeadPhoneShortStatus global set here to start timeout, cleared here.
;                       HEADPHONE_SHORT_TIMEOUT_BIT
;
;   Notes:          none
;<
;///////////////////////////////////////////////////////////////////////////////
CheckHeadPhoneShort
    ; First check if headphone short circuit handling is needed. 
    ; IRQA ISR sets this HeadPhoneShortStatus flag.
    ; Check if headphone short occurred
    btst    #HEADPHONE_SHORT_STATUS_BIT,y:HeadPhoneShortStatus
    jcc     _CheckForTimeOut

    ; Headphone short circuit IRQA has arrived. Re-clear the flag.
    bclr    #HEADPHONE_SHORT_STATUS_BIT,y:HeadPhoneShortStatus

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;; this section assumes mode 01 -- hw auto power down ;;;;;;;;
;;; if mode 11 - then skip this part and just wait 400mS before returning to class AB

    ;;;;;; put headphone into class A before power up
    bclr    #HW_HPCTRL_HPCLASSAB_BITPOS,x:HW_HPCTRL
	;reset proper short control mode based on heaphone set-up
	;Reset Common HP Amp (SHORTMODE_CM) here if needed 
    bclr    #HW_HPCTRL_SHORTMODE_LR_BITPOS,x:HW_HPCTRL  ; reset latch -- repowers amp
    bset    #HW_HPCTRL_SHORTMODE_LR_BITPOS,x:HW_HPCTRL  ; re-enable hw powerdown
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    ;;;;;; must wait 400mS before reenabling AB mode 
    bset    #HEADPHONE_SHORT_TIMEOUT_BIT,y:HeadPhoneShortStatus    
    ; stamp the time with future time to reenable headphones via AB mode
    jsr     SysGetCurrentTime
    move    y:<Const_000000,x1
    move 	#>400,x0
    add 	x,a    
    move 	a1,y:HeadphoneShortTimestampA1
    move 	a0,y:HeadphoneShortTimestampA0
    rts

_CheckForTimeOut
    btst    #HEADPHONE_SHORT_TIMEOUT_BIT,y:HeadPhoneShortStatus
    jcc     _ShortHandlerDone
        
    move 	y:HeadphoneShortTimestampA1,b
    move 	y:HeadphoneShortTimestampA0,b0

    jsr 	SysGetCurrentTime       ; returns current time in a
    cmp 	a,b                     ; if timeout value > current time
    jgt 	_ShortHandlerDone       ; exit
                                ; else -- timed out so class AB will reenable headphones
    bclr    #HEADPHONE_SHORT_TIMEOUT_BIT,y:HeadPhoneShortStatus

    ; unmute MASTERVR ** dependent upon ISR muting it.
    bclr    #HW_MIXVOLUMER_MUTE_BITPOS,x:HW_MIXMASTERVR

    ;;;;;; put headphone into class AB
    bset 	#HW_HPCTRL_HPCLASSAB_BITPOS,x:HW_HPCTRL

_ShortHandlerDone
    rts

    endsec
        
