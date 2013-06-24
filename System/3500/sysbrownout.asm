;///////////////////////////////////////////////////////////////////////////////
;
;  File        : sysbrownout.asm
;  Description : Brownout System Routines
;
;  Copyright (c) SigmaTel, Inc. Unpublished
; 
;  SigmaTel, Inc.
;  Proprietary  Confidential
; 
;  This source code and the algorithms implemented therein constitute
;  confidential information and may comprise trade secrets of SigmaTel, Inc.
;  or its associates, and any use thereof is subject to the terms and
;  conditions of the Confidential Disclosure Agreement pursuant to which this
;  source code was originally received.
;
;///////////////////////////////////////////////////////////////////////////////

    page    255,255,0

    section SYSFUN_Brownout

;///////////////////////////////////////////////////////////////////////////////
;   Macros
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "sysmacro.asm"
    list

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "sysequ.inc"
    include "hwequ.inc"
    include "project.inc"
    list
        
;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

    global  SysBrownOutIsr
;   global  SysRTCShutdown

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////
    nolist
    include "const.xref"
    list
    extern  FLowResAdcBattResult
    extern  FLowResAdcBattAvg

    extern  SaveSystemStack
    extern  RestoreSystemStack

	
;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////

    if (!@def(VBATT_BO_MV))
; brownout trip point in mV
VBATT_BO_MV     equ     830
    endif

	extern FbattBrownoutCount

NUM_BATT_READS  equ 	7

;///////////////////////////////////////////////////////////////////////////////
;   X Memory
;///////////////////////////////////////////////////////////////////////////////
        
    org     x,"SYSFUN_Brownout_X":

;///////////////////////////////////////////////////////////////////////////////
;   Y Memory
;///////////////////////////////////////////////////////////////////////////////

    org     y,"SYSFUN_Brownout_Y":
;Storage for the stack pointer register
OldSP       dc    0
OldR7       dc    0
OldM7       dc    0       
BrownoutStack   ds    20 
;///////////////////////////////////////////////////////////////////////////////
;   P Memory
;///////////////////////////////////////////////////////////////////////////////

    org     p,"SYSFUN_Brownout_P":

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysBrownOutIsr
;
;   Type:           Interrupt Service Routine
;
;   Description:    If IRQB is generated one of the brownouts occurred
;		    Vddd, Vddio, Battery -- if not battery shutdown immediately
;
;                   If battery brownout, debounce first, because noice when using DC/DC
;                   debounce counter is clear in main loop if >2
;
;   Inputs:         none
;
;   Outputs:        none
;                   
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
SysBrownOutIsr

   ; Clear the DM bit (double-precision multiply mode, bit 6) of the Mode Register
   ; This will be returned to its previous state when the RTI occurs (since the SR
   ; will be popped back off System Stack Low (SSL).
   ; Accumulator behavior is unpredictable when the DM bit is set.
   ; Next cycle after clearing it should NOT be an ALU instruction.
   andi #$BF,mr

   ;test point
   ;bset #HW_GP_B17_BITPOS,x:HW_GP0DOR

   move  m7,y:OldM7
   move  y:<Const_ffffff,m7
   move  r7,y:OldR7  
   move  #BrownoutStack,r7
   move  sp,y:OldSP

   ; Save Accumulator completely
   push   a0
   push   a1
   push   a2

   ; save other registers used
   push   x0

; clear out 2 levels of hw stack for do loops in _BattBOQualifier   
   move   sp,a 
   move   y:<Const_000003,x0
   cmp    x0,a 
   jlt     _sp_is_cleared

   push    ssl          ; need 2 levels for do loop
   push    ssh
   push    ssl        
   push    ssh
_sp_is_cleared

   ; --------------------------------------------------------------------------------
   ; if battery brownout enabled, debounce it -- if fast glitches 
   ; Assumption made here that brownout event is the threshold assigned to IRQ_EVENT0
   move    x:HW_BATT_RESULT,x0
   btst    #HW_BATT_RESULT_IRQ_EVENT0_BITPOS,x0
   jcs     _BattBOQualifier  

   ; verify this is a IRQB interrupt
   move    x:HW_RCR,x0
   btst    #HW_RCR_IRQB_BITPOS,x0
   jcs     _Done
   
   ; Check if this was a VDDIO brown out and shut down if it was.
   move    x:HW_DCDC_VDDIO,x0
   btst    #HW_DCDC_VDDIO_BROWNOUT_STATUS_BITPOS,x0
   jcs     _Shutdown

   ; if not a batt brownout put headphones into class A mode to reduce current draw
   bclr    #HW_HPCTRL_HPCLASSAB_BITPOS,x:HW_HPCTRL

   ; Go make sure that this was a real brownout and not just a glitch.
   jmp     _CoreBOQualifier
   ;------------------------

_Shutdown
    ; turn off the DCDC converter
    bclr    #HW_MIXTBR_PSRN_BITPOS,x:HW_MIXTBR  ;Must Clear PSRN bit first
    bset    #HW_CCR_PWDN_BITPOS,x:HW_CCR    ; If using the DCDC converter then                                          
                                            ; the part will turn off.  If not
                                            ; using the DCDC converter then
                                            ; the part will just kinda stop
                                            ; working :)
_loop  jmp _loop                            ; must stop here


;;;;;;;;;;;;;;;;;; Battery Brownout ;;;;;;;;;;;;;;;;;;;;
_BattBOQualifier
   ; assuming a 68uF cap on battery line, (dt/dv = 13.6uS/0.1V)
   ; if there was a battery drop out, we need to quickly decide
   ; because the xtal can be affected by battery droops before the Vddd BO will trip
   ; 
   ; The implementation prior to SDK2.105 takes too long to monitor the battery and
   ; the long term average was used making it susceptible to ungraceful shutdowns 
   ; due to battery "fall out" case
   ; 
   ; sample instantaneous voltage X times and decide within 13.6uS to shutdown
   ; only sum results to reduce time -- don't have time to find out/change clock speed and could be
   ; anywhere between 10MHz and 60MHz, so assume worst case 10MHz (100nS/dclk) 
   ; 136 clks to shutdown -- 
   ;     @10MHz: 27 reads, and 3 gpio bit manipulations (timing) takes 13.4uS to shutdown
   ;
   ; LRADC samples @ 4.096MHz/4 = 1.024MHz (976.5nS) -- another tradeoff is number of new samples taken during
   ;    X reads; also dependent upon clock speed, worse case is 60MHz (16.66nS/dclk) -- 58 clks between reads
   ;    with a 6 clk loop, need to read 10 times, to get 2 samples at 60MHz
   ;
   ; test point     
   ; bclr #HW_GP_B16_BITPOS,x:HW_GP0DOR

   ;;;; save stack in case it's a false alarm
    push b2
    push b1
    push b0
    push y0
    push r0
      
    ; --------------------------------------------------------------------------------
    ;equivalent to:     move    #>(BATT_BO_CNTS*NUM_BATT_READS),x0
    ;Assumes battery threshold was previously programmed
    clr     a
    move    y:FbattBrownoutCount,y0         ;Grab the precalculated brownout count
    move    #>(NUM_BATT_READS*256),x0       ;Shift the scalar by 8 bits, because of result's bit position in the register
    mpy     x0,y0,a                         ;Multiply expected brownout count by scalar
    asr     a                               ;Shift right once for integer math
    move    a0,x0                           ;Store in x0

    ;x0 now contains expected sum of NUM_BATT_READS number of results, shifted left by 8 bits
    ;to match the sum generated in _ReadBatt, to which it will be compared

    clr     a   #>LOW_RES_ADC_BATT_READ_MASK,y0
    clr     b
    move    #HW_BATT_RESULT,r0

_ReadBatt
    do #NUM_BATT_READS,_CompareBatt
    move    x:(r0),a1              ; read lradc result  (2 clks)
    and     y0,a1                  ; mask battery cnts  (2 clks)
    add     a,b                    ; add to accumulator (2 clks)
_CompareBatt

    cmp     x0,b
    ;test point
    bclr #HW_GP_B17_BITPOS,x:HW_GP0DOR
    jle     _Shutdown
    
    ; battery level ok so continue -- restore stack
    pop r0
    pop y0
    pop b0
    pop b1
    pop b2

    jmp     _Done

;;;;;;;;;;;;;;;;;; Vddd Brownout ;;;;;;;;;;;;;;;;;;;;;;;        
_CoreBOQualifier
   ; headphone shorts can cause the core to droop but not the battery
   ; if get vddd brownout, turn off headphone amp (class A is negligible current draw w/o the startup issues)
   ; check the battery voltage and if the battery is not drooping then continue
   ; battery will not droop during a headphone short if the short detect circuit is enabled
   ; bclr #HW_GP_B17_BITPOS,x:HW_GP0DOR

    ;Use the current averages value, unless it's zero
    move    y:FLowResAdcBattAvg,x0
    clr     a
    cmp     x0,a
    jne     _CheckVoltage
    ;At startup, the averaged value may not be available yet. If not, get instantaneous value.
    move    y:FLowResAdcBattResult,x0
    
_CheckVoltage        ; voltage to use is in x0
    ; if brownout level is less than actual voltage, don't shutdown yet
    move    y:FbattBrownoutCount,a
    cmp     x0,a
    jgt     _Shutdown

    ;bset #HW_GP_B17_BITPOS,x:HW_GP0DOR

    ; no brownout so return amp back to class AB
    bset    #HW_HPCTRL_HPCLASSAB_BITPOS,x:HW_HPCTRL     

_Done
    ; Restore the system stack 2 levels
    move    y:OldSP,a                    ; original, saved sp
    move   y:<Const_000003,x0
    cmp    x0,a 
    jlt     _sp_is_restored

    pop     ssh
    pop     ssl
    pop     ssh
    pop     ssl
_sp_is_restored

    ; Restore all the scratch registers used
    pop     x0
    ; Restore the Accumulator
    pop     a2
    pop     a1
    pop     a0
    move  y:OldSP,sp
    move  y:OldM7,m7
    move  y:OldR7,r7  

   ; test point
   ;bset #HW_GP_B16_BITPOS,x:HW_GP0DOR
 
    rti

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysRTCShutdown
;
;   Type:           Subroutine
;
;   Description:    In order to prevent corruption of RTC values, the CKRST bit
; 			can only be cleared within 0.5mS of HW_RTCLOW updates.
;
;   Inputs:         none
;
;   Outputs:        none
;                   
;   Notes:          Enabled in SDK2.105 & discovered that ROM clears CCR during
;			POST, so if using RTC when powerdown, MUST disable POST
;			bootmode.
;
;                   Uses a do loop
;<
;///////////////////////////////////////////////////////////////////////////////
;SysRTCShutdown
;rts
;safe RTC Shutdown is not needed on 35XX.

    endsec
        
