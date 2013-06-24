	org     p,".ptextrtcaccess":

	global FReadRTC
	global ReadRTC
;	global FClearRTC
;	global ClearRTC
	global FSetRTC
	global FInitRTC
	global InitRTC
;	global FenableAlarm
 ;	global enableAlarm
;	global FClearWakeBit
;	global ClearWakeBit
	include "regsrtc.inc"
	include "sysmacro.asm"
	include "regsclkctrl.inc"
	include "regsdcdc.inc"
	include "resource.inc"
    include "regsgpio.inc"

;externs
 if (@DEF('FUNCLET'))           
    extern    SysCallFunclet
 endif

    org     p,".ptextrtcaccess":
;////////////////////////////////////////////////////////////////////////////////
;//
;//>  Name:           ReadRTC
;//
;//   Type:           Function
;//
;//   Description:    Returns the current value of the RTC safely
;//                   
;//   Inputs:         none 
;//
;//   Outputs:        a1:a0 contain the value of the RTC as an unsigned long
;//<
;////////////////////////////////////////////////////////////////////////////////
FReadRTC:
ReadRTC:
    
 if (@DEF('FUNCLET'))           

    ;If using funclet, function will be executed via funclet kernel
    CallFunclet RSRC_FUNCLET_READ_RTC

    org p,"SYSFUNCLET_ReadRTC_P":
    
    dc      RSRC_FUNCLET_READ_RTC           ; Funclet Resource number

 endif
    push    sr
    bset    #9,sr
    bset    #8,sr  
    nop                 ;pipeline to make sure we don't get interrupted    
	nop
	nop
	nop
	nop	
	move x:HW_RTC_MSECONDS1,a1
	move x:HW_RTC_MSECONDS0,a0
    
    
    pop     sr
    rts	

  if (@DEF('FUNCLET'))

    ;If using funclet, switch counter back to original here
    org    p,".ptextrtcaccess":
    
  endif

;////////////////////////////////////////////////////////////////////////////////
;//
;//>  Name:           InitRTC
;//
;//   Type:           Function
;//
;//   Description:    Initializes the RTC to run at 1kHz
;//                   
;//   Inputs:         none 
;//
;//   Outputs:        
;//<
;////////////////////////////////////////////////////////////////////////////////
;funcletize
FInitRTC:
InitRTC:
 if (@DEF('FUNCLET'))           

    ;If using funclet, function will be executed via funclet kernel
    CallFunclet RSRC_FUNCLET_INIT_RTC

    org p,"SYSFUNCLET_InitRTC_P":
    
    dc      RSRC_FUNCLET_INIT_RTC           ; Funclet Resource number

 endif


    push    sr
    bset    #9,sr
    bset    #8,sr  
    nop                 ;pipeline to make sure we don't get interrupted    
	nop
	nop
	nop
	nop
	bset #>HW_DCDC_PERSIST_SLEEP_XTAL_ENABLE_BITPOS,x:HW_DCDC_PERSIST
	bset #>HW_DCDC_PERSIST_UPDATE_BITPOS,x:HW_DCDC_PERSIST			;update the master register in VDDXTAL clock and power domain with the shadow register contents
;I  found that the DCDC persistent register must be manually updated unlike the RTC persistent register...	
	bclr #>HW_DCDC_PERSIST_UPDATE_BITPOS,x:HW_DCDC_PERSIST
	bset #HW_CCR_CKRST_BITPOS,x:HW_CCR	;init clock logic
	bset #HW_CCR_XTLEN_BITPOS,x:HW_CCR	;digital clock enable
	bset #HW_CCR_ACKEN_BITPOS,x:HW_CCR	;analog clock enable
    move #>$10000,x0					;alow the clock to settle
	rep x0
    nop	
	bclr    #HW_RTC_CSR_SFTRST_BITPOS,x:HW_RTC_CSR		;release the copy controller
	rep x0
    nop

; Copy Controller Check - analog to digital
    ; Regs usage
    ; A  - Most recent RTC reading
    ; B  - misc.
    ; X0 - Count; max of 5 kicks
    ; Y0 - Previous RTC value
; Warning   Copy controller check present for sdk3.101 & above (maybe for 3.100 too), but not yet inserted for sdk2.610 (present but commented with tilda char. 
;  Not including this segment can give unreliable rtc access.  Segment ends after label _RTCValueValid below
    
    jsr ReadRTC ; UpperRTC in a1, LowerRTC in a0
    
    ; check if RTC is less than 90 seconds
    clr b
    move #>90000,b0
    cmp a,b ; if b-a > 0, then RTC is less than 90 seconds, check for a reset case
    jle _RTCValueValid ; else continue
    
    ; wait about 3 ms to let RTC increment					
    move #>30000,x0 
	rep x0
    nop
    
    ; max of 5 copy controller kicks 
    move #>5,x0 ; count = 5

_ReKickCopyController
    ; Save Previous RTC
    move a1,y1 
    move a0,y0
    
    ; Decrement the count
    move #>1,a ; a = 1
    move x0,b  ; b = count
    sub a,b    ; b = count - 1
    jeq _CopyControllerFailed ; after 5 kicks
    move b,x0  ; count = b
        
    ; wait on new regs
_CopyLoop 
    btst    #HW_RTC_CSR_NEWREGS_MSCOUNT_BITPOS,x:HW_RTC_CSR
    jcs     _CopyLoop
    
    ; Kick copy controller
	bset    #HW_RTC_CSR_SFTRST_BITPOS,x:HW_RTC_CSR		;halt the copy controller
    bset    #HW_RTC_CSR_FORCE_BITPOS,x:HW_RTC_CSR       ;force an update
	bclr    #HW_RTC_CSR_SFTRST_BITPOS,x:HW_RTC_CSR		;release the copy controller

    ; wait for ms stale bit to cler
_CopyLoop2
    btst    #>5,x:HW_RTC_CSR
    jcs     _CopyLoop2

    ; Read current RTC and compare to Previous RTC
    jsr ReadRTC ; a = ReadRTC 
    move y0,b0  ; b = PrevRTC
    move y1,b1
    cmp b,a      
    ; is current RTC > previous RTC?
    ; no, then kick copy controller 
    jlt _ReKickCopyController 
    ; yes, then continue
    jmp _RTCValueValid
    
_CopyControllerFailed
    ; if you are here, the copy controller ran
    ; 5 times without returning a valid value
    debug    
        
_RTCValueValid
; End of Copy Controller Check
	move    #>375,x0
_Loop
    btst    #HW_RTC_CSR_NEWREGS_XTALDIV_BITPOS,x:HW_RTC_CSR	;wait UNTIL NEWS_REGS is 0    
    jcs     _Loop    
    move    x0,x:HW_RTC_DIVIDE	;get a 1kHz clock
_LoopIt	
	btst    #HW_RTC_CSR_NEWREGS_PERSIST0_BITPOS,x:HW_RTC_CSR	;wait UNTIL NEWS_REGS is 0    
    jcs     _LoopIt    	
	bclr    #HW_RTC_PERSISTENT0_XTALPDOWN_BITPOS,x:HW_RTC_PERSIST0	;make sure analog is powered during OFF state
	bclr    #HW_RTC_PERSISTENT0_ALARMWAKE_BITPOS,x:HW_RTC_PERSIST0	;this bit is set when we power up from an alarm event so we need to clear it.	
    pop     sr	
    rts

 if (@DEF('FUNCLET'))

    ;If using funclet, switch counter back to original here
    org    p,".ptextrtcaccess":
    
 endif


;////////////////////////////////////////////////////////////////////////////////
;//
;//>  Name:           ClearRTC
;//
;//   Type:           Function
;//
;//   Description:    Makes the RTC MSEC1 and MSEC0 counts both 0x00
;//                   
;//   Inputs:         none 
;//
;//   Outputs:        none
;//<
;////////////////////////////////////////////////////////////////////////////////
;FClearRTC:
;ClearRTC:
;	clr a
;	jsr RTCSet
;	rts


;////////////////////////////////////////////////////////////////////////////////
;//
;//>  Name:           SetRTC
;//
;//   Type:           Function
;//
;//   Description:    Sets RTC count to a1:a0
;//                   
;//   Inputs:         a1:a0 number of milliseconds to set counter to 
;//
;//   Outputs:        none
;//<
;////////////////////////////////////////////////////////////////////////////////
FSetRTC:
SetRTC:
 if (@DEF('FUNCLET'))           

    ;If using funclet, function will be executed via funclet kernel
    CallFunclet RSRC_FUNCLET_SET_RTC

    org p,"SYSFUNCLET_SetRTC_P":
    
    dc      RSRC_FUNCLET_SET_RTC           ; Funclet Resource number

 endif
  	
	push    sr			;save the status register so we can disable interrupts
    bset    #9,sr
    bset    #8,sr    
    nop                 ;pipeline to make sure we don't get interrupted
	nop
	nop
	nop
	nop

; Copy Controller check - digital to analog
    ; Regs usage
    ; A  - Most recent RTC reading
    ; B  - misc.
    ; X0 - Intended RTC value
    ; Y0 - count; max of 5 kicks
    ; Y1 - 1; for decrementing

; Max of 5 kicks
    move   #>5,y0
    move   #>1,y1

; Wait on new regs, then write to digital RTC register    
_Loop2:		
	btst    #HW_RTC_CSR_NEWREGS_MSCOUNT_BITPOS,x:HW_RTC_CSR
	jcs	_Loop2
	move a1,x:HW_RTC_MSECONDS1	
	move a0,x:HW_RTC_MSECONDS0

; Wait for digital to analog copy to complete by checking new regs
_Loop3:		
	btst    #HW_RTC_CSR_NEWREGS_MSCOUNT_BITPOS,x:HW_RTC_CSR 
	jcs	_Loop3

; Save intended RTC value with allowance for 3ms difference
    clr b
    move #>3,b0 ; 3ms difference
    add b,a
    move a1,x1  ; save intended RTC value
    move a0,x0
    
; Force an update, wait on stale reg for copy completion
	bset    #HW_RTC_CSR_SFTRST_BITPOS,x:HW_RTC_CSR		;halt the copy controller
    bset    #HW_RTC_CSR_FORCE_BITPOS,x:HW_RTC_CSR       ;force an update
	bclr    #HW_RTC_CSR_SFTRST_BITPOS,x:HW_RTC_CSR		;release the copy controller    
_Loop4:
    btst    #5,x:HW_RTC_CSR ; wait on ms stale reg
	jcs	_Loop4

; Read back RTC and compare to intended value
    jsr  ReadRTC ; UpperRTC in a1, LowerRTC in a0
    move x1,b1   ; move intended RTC value back to b
    move x0,b0
    sub  a,b
    jge  _RTCWriteSuccessful

; Else the intended RTC is greater than the one read back
;  so the copy failed. If it hasn't been 5 kicks, try again.
    clr  a 
    move y0,a0 ; move count to a
    sub y1,a   ; count - 1
    jeq _RTCWriteFailed ; check for 5 kicks
    move a0,y0 ; save count

    move x1,a1 ; move inteded RTC values back to a
    move x0,a0
    jmp  _Loop2  
    
_RTCWriteSuccessful:    
    pop  sr				;restore the status register
	rts

_RTCWriteFailed
    ; if here the copy controller failed to write to the analog
    ; persistent registers correctly.
    debug

 if (@DEF('FUNCLET'))

    ;If using funclet, switch counter back to original here
    org    p,".ptextrtcaccess":
    
 endif

; The following code is for the RTCALARM
 	
;////////////////////////////////////////////////////////////////////////////////
;//
;//>  Name:           FSetRTCAlarm
;//
;//   Type:           Function
;//
;//   Description:    turn the alarm on to trigger at a1:a0 milliseconds
;//                   
;//   Inputs:         a1:a0 number of milliseconds to set alarm to 
;//
;//   Outputs:        none - not used yet
;//<
;////////////////////////////////////////////////////////////////////////////////
;FSetRTCAlarm:
;SetRTCAlarm:
;  if (@DEF('FUNCLET'))           
;
;    ;If using funclet, function will be executed via funclet kernel
;    CallFunclet RSRC_FUNCLET_SET_RTC_ALARM
;
;    org p,"SYSFUNCLET_SetRTCAlarm_P":
;    
;    dc      RSRC_FUNCLET_SET_RTC_ALARM           ; Funclet Resource number
;
;  endif
; 
;	push    sr			;save the status register so we can disable interrupts
;    bset    #9,sr
;    bset    #8,sr    
;    nop                 ;pipeline to make sure we don't get interrupted
;	nop
;	nop
;	nop
;	nop
;	bset    #HW_RTC_CSR_ALARMINTEN_BITPOS,x:HW_RTC_CSR		;use to enable interrupt on alarm event
;_AlarmLoop1:		
;	btst    #HW_RTC_CSR_NEWREGS_ALRMCOUNT_BITPOS,x:HW_RTC_CSR		;test NEW_REGS flag
;	jcs	_AlarmLoop1
;	move a1,x:HW_RTC_ALARM1	
;	move a0,x:HW_RTC_ALARM0
;_AlarmLoop3:
;	btst    #HW_RTC_CSR_NEWREGS_PERSIST0_BITPOS,x:HW_RTC_CSR
;	jcs	_AlarmLoop3
;	bset #HW_RTC_PERSISTENT0_ALARMWAKE_EN_BITPOS,x:HW_RTC_PERSIST0	
;	bset #HW_RTC_PERSISTENT0_ALARMEN_BITPOS,x:HW_RTC_PERSIST0
;	pop  sr				;restore the status register
;	move #>$10000,x0
;	rep x0
;    nop	
;	rts
;  if (@DEF('FUNCLET'))
;
;    ;If using funclet, switch counter back to original here
;    org    p,".ptextrtcaccess":
;    
;  endif
  
  

;////////////////////////////////////////////////////////////////////////////////
;//
;//>  Name:           FRTCClearWakeBit
;//
;//   Type:           Function
;//
;//   Description:    clears the alarm wake bit after alarm interrupt
;//                   
;//   Inputs:         none 
;//
;//   Outputs:        clears alarm wake in persistent reg 0
;//<
;////////////////////////////////////////////////////////////////////////////////
;FClearWakeBit:
;ClearWakeBit:
;  ; Can't funcletize because it is in an interrupt
;
;  	btst    #HW_RTC_CSR_NEWREGS_PERSIST0_BITPOS,x:HW_RTC_CSR
;	jcs	ClearWakeBit
;	bclr    #HW_RTC_PERSISTENT0_ALARMWAKE_BITPOS,x:HW_RTC_PERSIST0	;doesn't seem to get set, but part will not shutdown if not called after the interrupt occurs
;	bclr #HW_RTC_PERSISTENT0_ALARMWAKE_EN_BITPOS,x:HW_RTC_PERSIST0	;disable the ALARM WAKE.  Be sure to re-enable if you want to wake the part from an alarm.
;   rts 
