;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2006
;
;  File        : sysusb.asm
;  Description : routines used to setup/monitor usb hardware
;///////////////////////////////////////////////////////////////////////////////

    section     SYSFUN_Usb

;///////////////////////////////////////////////////////////////////////////////
;  Macros
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "sysmacro.asm"
    list

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////
    include "hwequ.inc"
    include "resource.inc"
 if (@DEF('FUNCLET'))           
    extern    SysCallFunclet
 endif

	
;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////
     global UsbDetectInit
     global DetectUsb
     global UsbConnected


;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////
     extern SysStopDecoder
     extern SysStopEncoder
     extern FPowerDownHeadPhones
     extern SysSaveSettings
     extern IcollClear
	 extern Fg_bServiceDCDC
	 
 if @def('TUNER_STFM1000')
     extern FSTFM1000TunerSafePowerDown
 endif
	 extern SysLoadFATWrite
	 extern FSaveUserTime
 if (@def('DEVICE_3500'))
     extern FSetStickyBit
 endif

 if (@DEF('BATTERY_CHARGE'))
	 extern FBatteryChargeStopCharging
 endif
     extern SysSpeedSysShutDown
     
 if (@DEF('AUDIBLE'))
     extern MsgMenuUsbConnected
     extern SysPostMessage
 endif
     include "const.xref"
     
     include "sysspeed.inc" ; To solve the hanging at USB connection problem with MBCMA/Multimeida/Lyric patch STMP00011043
     
;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////



;///////////////////////////////////////////////////////////////////////////////
;   P Memory
;///////////////////////////////////////////////////////////////////////////////
    org p,"SYSFUN_SysUsb_P":



;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           UsbDetectInit
;
;   Type:           Function
;
;   Description:    Initialize Usb Detect GPIO pin
;
;   Inputs:         none
;
;   Outputs:        none
;                   
;   Notes:          none
;<
;///////////////////////////////////////////////////////////////////////////////
UsbDetectInit
    rts


;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           DetectUsb
;
;   Type:           Function
;
;   Description:    Detects connection with host via USB, shuts down the system
;                   and reboots. 
;
;   Inputs:         none
;
;   Outputs:        none
;                   
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
DetectUsb
    if (@def('DEVICE_3500'))
      

    ;Check if power source is +5V
    ;If power is not from +5V => USB is not connected.
    ;If power is from +5V => USB connected or Charger Connected
    btst    #HW_USBCSR_VBUSSENSE_BITPOS,x:HW_USBCSR
    jcc     _usb_not_connected
    
    if (@def('DCDC_POWER_TRANSFER'))
    ;Also, wait until ServiceDCDC() has completed. If g_bServiceDCDC=1, then
    ;ServiceDCDC() has not completed. 
    move    x:Fg_bServiceDCDC,x0
	clr	    a
	cmp     x0,a
	jne		_usb_not_connected 
    endif

	;If the plugged in detector is already enabled, skip delay
	btst #HW_USBCSR_PLUGGEDIN_EN_BITPOS,x:HW_USBCSR
	jcs  _test_pluggedin

    ; Enable USB plug in circuit
    bset   #HW_USBCSR_PLUGGEDIN_EN_BITPOS,x:HW_USBCSR
     
    ; The circuitry takes up to 1MSec to stabilize
    clr     a
    move    #>10000,a0
_SettlingDelay  ; Each loop is 3 clocks
    dec     a
    jne     _SettlingDelay

_test_pluggedin
;Debounce D+/- to prevent false resets.    
    clr     a
	move    #>$000001,x0   ;set x0=1

    do      #24,_usb_samp      ; Insert NOP's on next line to reduce the "sample rate" 
         nop
         btst    #HW_USBCSR_PLUGGEDIN_BITPOS,x:HW_USBCSR                    
         jcc     _sample0
         add     x0,a		   ; Add 1 if pluggedin = 1
_sample0
         nop                   ;  Padding for the "do" instruction (pipeline effects)
_usb_samp
    move   #>$000013,x0	   	  ;if count is greater than 19, USB Connected 
    cmp x0,a
    jlt     _usb_not_connected

 if (@def('AUDIBLE'))
	;if an audible decoder is loaded, send a message to menus to save the position
    ;file.  if not, call UsbConnected to switch to USB mode.
    move   #>RSRC_AUDIBLE_ACELPDEC_CODE,x0
    move   y:DecoderResourcePtr,a
    cmp   x0,a
    jeq   _post_usbconnected
    
    move   #>RSRC_AUDIBLE_DECMOD_CODE,x0
    move   y:DecoderResourcePtr,a
    cmp   x0,a
    jeq   _post_usbconnected    
    ;an audible decoder is not loaded, switch to USB mode now.
    jsr     UsbConnected
    
_post_usbconnected
    ;post the UsbConnected message and allow menus to process.
    move    #MsgMenuUsbConnected,r0
    move    y:<Const_ffffff,m0
    jsr     SysPostMessage
 else
    jsr     UsbConnected
 endif
    
_usb_not_connected
    rts            

    else  ;DEVICE_3410

    btst    #HW_SPARER_USB_PLUGIN_BITPOS,x:HW_SPARER             

    jcc     _usb_not_connected

    jsr     UsbConnected

_usb_not_connected
    rts

	endif ;if (@def('DEVICE_3500')


;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           UsbConnected
;
;   Type:           Function
;
;   Description:    Shuts down the system and reboots
;
;   Inputs:         none
;
;   Outputs:        none
;                   
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
UsbConnected

 if (@DEF('FUNCLET'))           

    ;If using funclet, function will be executed via funclet kernel
    CallFunclet RSRC_FUNCLET_USBCONNECTED

    org p,"SYSFUNCLET_UsbConnected_P":
    
    dc      RSRC_FUNCLET_USBCONNECTED           ; Funclet Resource number

 endif

 if @def('TUNER_STFM1000')
    ; Steely Dan in low power mode and release I2S pins before stop the encoder and decoder
    jsr     FSTFM1000TunerSafePowerDown
 endif

    ; Stop the decoder and/or the encoder
    jsr     SysStopDecoder
    jsr     SysStopEncoder

    ; To solve the hanging at USB connection problem with MBCMA/Multimeida/Lyric patch STMP00011043
    move    #>SPEED_CLIENT_JPEG_DEC,a
    jsr     FSysSpeedClockFree
    
 if (@DEF('BATTERY_CHARGE'))
    ;turn off charging
	jsr     FBatteryChargeStopCharging
 endif
    ; power down the headphones
    jsr     FPowerDownHeadPhones

    ; disable interrupts in the core
;;;    move    y:<Const_000000,x0       ;Now redundant code since HW_IPR = #>$007000 below
;;;    move    x0,x:HW_IPR
	 
    ; Some interrupts need to be made available for the rest of the code to work.  
    ; TOVERIFY  Have sdk3.0 team review this 2.6 change that allows ecc irq also.
    ;The following interrupts are turned on:
    ; 1. ECC Interrupt: value 004000. This enables Interrupt Line 2 with priority level 0 (added by sdk2.6)
    ; 2. 5V Connect/Disconnect interrupt : value 003000. This enables Interrupt line 1  (was already in sdk3.0)
    ;    with priority level 2
    move    #>$007000,x0    
    move    x0,x:HW_IPR
    	
    ;Save the settings now ...
    jsr     SysLoadFATWrite
    
    jsr     FSaveUserTime
     
    jsr     SysSaveSettings
 if (@def('DEVICE_3500'))
;    // We're powering down gracefully, so set the SkipCheckdisk bit.
;    SetStickyBit((volatile _X WORD *)&HW_RTC_SKIP_CHECKDISK_REG, 
;              HW_RTC_PERSISTENT1_SKIP_CHECKDISK_BITPOS);
    move    #HW_RTC_PERSIST1,R0
    move    y:<Const_000001,a
    jsr     FSetStickyBit
 endif

    ; Tristate the LCD outputs so correct boot mode will be detected
    move    y:<Const_000000,x0
    move    x0,x:HW_GP0ENR

    ; clear out the icoll
    jsr     IcollClear

    jsr SysSpeedSysShutDown     ;Stmp8455 Change to SPEED_MAX to improve startup reliability after doing Reset.

    ; just burn some time letting the boot lines discharg
    ; use nops since the interrupts are disabled.  At 40Mhz
    ; this is about 5ms
    move    y:<Const_200000,x0
    rep     x0
	nop
	
DoProcessorReset
    move    #>$0F07D0,x0
    
    move    x0,x:HW_RCR         ;Resets the processor - Go to STMP3500 ROM code
    nop

    ; shouldn't get here
    jmp     DoProcessorReset   ;Stmp8455 Insurance against the impossible

_usb_not_connected
    rts
    
 if (@DEF('FUNCLET'))

    ;If using funclet, switch counter back to original here
    org p,"SYSFUN_SysUsb_P":
    
 endif
    endsec
