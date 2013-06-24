;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2001
;
;  File        : usbbrownoutirq.asm
;  Description : DCC Brownout Interrupt Handler
;///////////////////////////////////////////////////////////////////////////////

        section UsbBrownout

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

     GLOBAL    UsbSysBrownOutIsr

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

     nolist
     include "const.xref"
     list
     EXTERN  LowBattery

;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////

    if (!@def(VBATT_BO_MV))
; brownout trip point in mV
VBATT_BO_MV     equ     830
    endif

; BATT_BO_CNTS is used when monitoring the actual voltage before shutdown
; bo level / (mv/bit) = # cnts at bo level (mV level / (resolution/127))
BATT_BO_CNTS    equ     ((VBATT_BO_MV*127)/2700)

NUM_BATT_READS  equ 	100

;///////////////////////////////////////////////////////////////////////////////
;  X memory
;///////////////////////////////////////////////////////////////////////////////


;///////////////////////////////////////////////////////////////////////////////
;  Y memory
;///////////////////////////////////////////////////////////////////////////////

;Modeled after sysbrownout.asm for player
     org     y,"UsbBrownout_Y":
;Storage for the stack pointer register
OldSP     	dc    0
OldR7		dc    0
OldM7		dc    0       
BrownoutStack   ds    20

;///////////////////////////////////////////////////////////////////////////////
;   P Memory
;///////////////////////////////////////////////////////////////////////////////

      org     p,"UsbBrownout_P":

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           UsbSysBrownOutIsr
;
;   Type:           Function
;
;   Description:    Brownout interrupt handler
;
;   Inputs:         none
;
;   Outputs:        none
;                   
;   Notes:          ?
;<
;///////////////////////////////////////////////////////////////////////////////

UsbSysBrownOutIsr
   ; Clear the DM bit (double-precision multiply mode, bit 6) of the Mode Register
   ; This will be returned to its previous state when the RTI occurs (since the SR
   ; will be popped back off System Stack Low (SSL).
   ; Accumulator behavior is unpredictable when the DM bit is set.
   ; Next cycle after clearing it should NOT be an ALU instruction.
   andi #$BF,mr   

;   bset #HW_GP_B16_BITPOS,x:HW_GP0DOR
   move  m7,y:OldM7
   move  y:<Const_ffffff,m7
   move  r7,y:OldR7  
   move  #BrownoutStack,r7
   move  sp,y:OldSP

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

   ; if battery brownout enabled, debounce it -- if fast glitches 
   btst    #HW_LRADC_RES_BATLRBR_BITPOS,x:HW_LRADC_RES
   jcs     _BattBOQualifier  

   ; verify this is a IRQB interrupt
   btst    #HW_RCR_IRQB_BITPOS,x:HW_RCR
   jcs     _Done

   ; if it's not a battery brownout; powerdown immediately. Battery BO should
   ; occur before VDDD or VDDIO

_Shutdown
    ; turn off the DCDC converter
    bset    #HW_CCR_PWDN_BITPOS,x:HW_CCR    	; If using the DCDC converter then	                                        
	                                        ; the part will turn off.  If not
	                                        ; using the DCDC converter then
	                                        ; the part will just kinda stop
	                                        ; working :)
_loop  jmp _loop				; must stop here


;;;;;;;;;;;;;;;;;; Battery Brownout ;;;;;;;;;;;;;;;;;;;;
_BattBOQualifier
   ; assuming a 68uF cap on battery line, (dt/dv = 13.6uS/0.1V)
   ; if there was a battery drop out, we need to quickly decide
   ; because the xtal can be affected by battery droops before the Vddd BO will trip
   ;  
   ; sample instantaneous voltage X times and decide within 13.6uS to shutdown
   ; USB clock should be running at 48MHz 
   ; 652 clks to shutdown -- 
   ;
   ; LRADC samples @ 4.096MHz/4 = 1.024MHz (976.5nS)
   ; -- @48MHz (20.83nS/dclk) -- 46 clks between reads
   ;    with a 6 clk loop, need to read 8 times, to get 2 samples
   ;
   ; 100 Read loops will take 600 clks will take 12.5uS and will take 12 LRADC samples
   ;
   ;    bset #HW_GP_B17_BITPOS,x:HW_GP0DOR

   ;;;; save stack in case it's a false alarm
    push a2
    push a0
    push b2
    push b1
    push b0
    push y0
    push r0
      
    clr     a   #>LOW_RES_ADC_BATT_READ_MASK,y0
    clr     b	; accumulator
    move    #>(BATT_BO_CNTS*NUM_BATT_READS),x0
    move    #HW_LRADC_RES,r0

_ReadBatt
    do #NUM_BATT_READS,_CompareBatt
    move    x:(r0),a1  		       ; read lradc result  (2 clks)
    and     y0,a1                      ; mask battery cnts  (2 clks)
    add     a,b  		       ; add to accumulator (2 clks)
_CompareBatt

    cmp     x0,b
;    bclr #HW_GP_B17_BITPOS,x:HW_GP0DOR
    jle     _Shutdown

   ; battery level ok so continue -- restore stack
    pop r0
    pop y0
    pop b0
    pop b1
    pop b2
    pop a0
    pop a2

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


;   bclr #HW_GP_B16_BITPOS,x:HW_GP0DOR
 
    rti

    endsec

