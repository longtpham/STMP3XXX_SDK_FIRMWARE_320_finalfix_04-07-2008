;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2001
;
;  File        : usbbrownout.asm
;  Description : Brownout System Routines for USBMSC
;///////////////////////////////////////////////////////////////////////////////

    ;page    255,255,0

    section SYSFUN_Brownout
	;section UsbBrownout

;///////////////////////////////////////////////////////////////////////////////
;   Macros
;///////////////////////////////////////////////////////////////////////////////

;    nolist
    include "sysmacro.asm"
;    list

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////

;    nolist
;    include "sysequ.inc"
;    include "hwequ.inc"
;    include "project.inc"

	include "regsclkctrl.inc"
	include "regslradc.inc"
    include "regsgpflash.inc"
;    list
        
;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

    global  SysBrownOutIsr
;    global  SysRTCShutdown

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


    if (@def('DEVICE_3500'))
	extern FbattBrownoutCount
    else ;3410

; BATT_BO_CNTS is used when monitoring the actual voltage before shutdown
; bo level / (mv/bit) = # cnts at bo level (mV level / (resolution/127))
BATT_BO_CNTS    equ     ((VBATT_BO_MV*127)/2700)

    endif ;if (@def('DEVICE_3500'))

NUM_BATT_READS  equ 	27

;///////////////////////////////////////////////////////////////////////////////
;   X Memory
;///////////////////////////////////////////////////////////////////////////////
        
    org     x,"SYSFUN_Brownout_X":

;///////////////////////////////////////////////////////////////////////////////
;   Y Memory
;///////////////////////////////////////////////////////////////////////////////

    org     y,"SYSFUN_Brownout_Y":
;Storage for the stack pointer register
OldSP     	dc    0
OldR7		dc    0
OldM7		dc    0       
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
;_asmfunc void SysBrownOutIsr()
SysBrownOutIsr

   ; Clear the DM bit (double-precision multiply mode, bit 6) of the Mode Register
   ; This will be returned to its previous state when the RTI occurs (since the SR
   ; will be popped back off System Stack Low (SSL).
   ; Accumulator behavior is unpredictable when the DM bit is set.
   ; Next cycle after clearing it should NOT be an ALU instruction.
   andi #$BF,mr

   move  m7,y:OldM7
   move  y:<Const_ffffff,m7
   move  r7,y:OldR7  
   move  #BrownoutStack,r7
   move  sp,y:OldSP
   
   ; If a brownout occurs, we must write protect the part.  In the case of a 
   ; Renesas part, this same line will reset the part which is the safe state.
   bclr    #HW_GPFLASH_CSR1_WP0_BITPOS,x:HW_GPFLASH_CSR1R
   bset    #HW_GPFLASH_CSR1_MUX_BITPOS,x:HW_GPFLASH_CSR1R
   
   
; clear out 2 levels of hw stack for do loops in RTCShutdown & _BattBOQualifier   
   push   a1
   push   x0
   move   sp,a 
   move   y:<Const_000003,x0
   cmp    x0,a 
   jlt     _sp_is_cleared

   push    ssl			; need 2 levels for do loop
   push    ssh
   push    ssl   	  
   push    ssh
_sp_is_cleared

   if (@def('DEVICE_3500'))
   ; --------------------------------------------------------------------------------
   ; if battery brownout enabled, debounce it -- if fast glitches 
   ; Assumption made here that brownout event is the threshold assigned to IRQ_EVENT0
   bclr    #HW_BATT_RESULT_IRQ_EVENT0_BITPOS,x:HW_BATT_RESULT
   jcs     _BattBOQualifier  

   ; verify this is a IRQB interrupt
   btst    #HW_RCR_IRQB_BITPOS,x:HW_RCR
   jcs     _Done

   else ;3410
   ; --------------------------------------------------------------------------------
   ; if battery brownout enabled, debounce it -- if fast glitches 
   btst    #HW_LRADC_RES_BATLRBR_BITPOS,x:HW_LRADC_RES
   jcs     _BattBOQualifier  

   ; verify this is a IRQB interrupt
   btst    #HW_RCR_IRQB_BITPOS,x:HW_RCR
   jcs     _Done

   endif ;if (@def('DEVICE_3500'))

_Shutdown
    ; turn off the DCDC converter
    bset    #HW_CCR_PWDN_BITPOS,x:HW_CCR    ; If using the DCDC converter then	                                        
	                                        ; the part will turn off.  If not
	                                        ; using the DCDC converter then
	                                        ; the part will just kinda stop
	                                        ; working :)
_loop  jmp _loop							; must stop here


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
   ; Because the RTCShutdown takes so long, it is bypassed in the battery brownout case
   ; The battery is either slowly dying or has fallen out, so the RTC will be corrupted
   ; when the battery is removed anyway. It is more important to stop execution before the xtal 
   ; loses power rather than try to save RTC.  
   ;
   ; sample instantaneous voltage X times and decide within 13.6uS to shutdown
   ; only sum results to reduce time -- don't have time to find out/change clock speed and could be
   ; anywhere between 10MHz and 60MHz, so assume worst case 10MHz (100nS/dclk) 
   ; 136 clks to shutdown -- 
   ;     @10MHz: 27 reads, no rtcshutdown, and 3 gpio bit manipulations (timing) takes 13.4uS to shutdown
   ;
   ; LRADC samples @ 4.096MHz/4 = 1.024MHz (976.5nS) -- another tradeoff is number of new samples taken during
   ;	X reads; also dependent upon clock speed, worse case is 60MHz (16.66nS/dclk) -- 58 clks between reads
   ;    with a 6 clk loop, need to read 10 times, to get 2 samples at 60MHz
   ;     

   ;;;; save stack in case it's a false alarm
    push a2
    push a0
    push b2
    push b1
    push b0
    push y0
    push r0
      
    if (@def('DEVICE_3500'))
    ; --------------------------------------------------------------------------------
	;equivalent to:     move    #>(BATT_BO_CNTS*NUM_BATT_READS),x0
	;Assumes battery threshold was previously programmed
	clr		a
    move    y:FbattBrownoutCount,y0			;Grab the precalculated brownout count
	move	#>(NUM_BATT_READS*256),x0		;Shift the scalar by 8 bits, because of result's bit position in the register
	mpy		x0,y0,a							;Multiply expected brownout count by scalar
	asr		a								;Shift right once for integer math
	move	a0,x0							;Store in x0

	;x0 now contains expected sum of NUM_BATT_READS number of results, shifted left by 8 bits
	;to match the sum generated in _ReadBatt, to which it will be compared

    clr     a   #>LOW_RES_ADC_BATT_READ_MASK,y0
    clr     b
    move    #HW_BATT_RESULT,r0

    else ;3410
    ; --------------------------------------------------------------------------------
    clr     a   #>LOW_RES_ADC_BATT_READ_MASK,y0
    clr     b	; accumulator
    move    #>(BATT_BO_CNTS*NUM_BATT_READS),x0
    move    #HW_LRADC_RES,r0

    endif ;if (@def('DEVICE_3500'))

_ReadBatt
    do #NUM_BATT_READS,_CompareBatt
    move    x:(r0),a1  		       ; read lradc result  (2 clks)
    and     y0,a1                  ; mask battery cnts  (2 clks)
    add     a,b  		           ; add to accumulator (2 clks)
_CompareBatt

    cmp     x0,b
    jle     _Shutdown

    ; battery level ok so continue -- restore stack
    pop r0
    pop y0
    pop b0
    pop b1
    pop b2
    pop a0
    pop a2

    ;jmp     _Done

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

    pop     x0
    pop     a1
    move  y:OldSP,sp
    move  y:OldM7,m7
    move  y:OldR7,r7  

    rti


    endsec
        
