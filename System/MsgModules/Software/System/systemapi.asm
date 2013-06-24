;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2003
;
; Filename:     systemapi.asm
; Description:  STMP3 System Module API
;               System module functions such as init, shutdown, and battery life.
;///////////////////////////////////////////////////////////////////////////////

    section SYSMOD_SystemApi

;///////////////////////////////////////////////////////////////////////////////
;  Macros
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "sysmacro.asm"
    include "systrace.asm"
    list

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "sysequ.inc"
    include "sysspeed.inc"
    include "hwequ.inc"
    include "sysresources.inc"
    include "msgequ.inc"
    include "buttons.inc"
    include "decoderequ.inc"
    include "resource.inc"
    list

;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

    GLOBAL   SystemShutdownFinal
    GLOBAL   SysGetVolume

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "sysmem.xref"
    include "sysmisc.xref"
    include "stack.xref"
    include "stackmem.xref"
    include "syspostmsg.xref"
    include "sysirq.xref"
    include "systime.xref"
    include "button.xref"
    include "const.xref"
    list

    ; milestone3msgs.asm
    EXTERN   DecoderGetStatus
    EXTERN   SysFlushCoderMemory
    EXTERN   SysSpeedSysShutDown
    EXTERN   ModuleSetTimer
      
    EXTERN   SysSaveSettings
    
    EXTERN  MIX_LVOL_MASK
    EXTERN  MIX_RVOL_MASK
    EXTERN  MIN_RVOL
    EXTERN  MixerMasterVol
    EXTERN  MixerStatus
    EXTERN  VOL_REG_BIT

;    EXTERN  SysRTCShutdown
    EXTERN  EmptyFunction
    extern  FPowerDownHeadPhones
    EXTERN  SysPostMessage 
    extern  FSetStickyBit 

 if (@DEF('FUNCLET'))           
    extern  SysCallFunclet
 endif

;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////


;///////////////////////////////////////////////////////////////////////////////
;   X Memory
;///////////////////////////////////////////////////////////////////////////////

      org x,"SYSMOD_SystemApi_X":

;///////////////////////////////////////////////////////////////////////////////
;   Y Memory
;///////////////////////////////////////////////////////////////////////////////

      org y,"SYSMOD_SystemApi_Y":

; data for SystemShutdown
Sys_ShuttingDownFlag    dc      0
Sys_StartTimeHigh       dc      0
Sys_StartTimeLow        dc      0

; data for SystemShutdownFinal
Ssf_ThisIsReallyIt      dc      0

; location to store the IPR value
SaveIPR                 dc      0

      org y,"SYSMOD_SystemApi_mod_Y":


;///////////////////////////////////////////////////////////////////////////////
;   P Memory
;///////////////////////////////////////////////////////////////////////////////

     org p,"SYSMOD_SystemApi_P":

;///////////////////////////////////////////////////////////////////////////////
;> Name:        SysGetVolume
;
;  Type:        Function
;
;  Description: This function returns the volume level in a1.
;               If muted due to FadeOut:
;                   Return SW register value (MixerMasterVol)
;               Else:
;                   Return HW register value (HW_MIXMASTERVR)
;
;  Inputs:      None.
;
;  Outputs:     a1 = Current Volume.
;
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////

SysGetVolume

    if (@DEF('FUNCLET'))           
   
       CallFunclet RSRC_FUNCLET_SYSGETVOLUME
   
       org p,"SYSFUNCLET_SysGetVolume_P":
       
       dc      RSRC_FUNCLET_SYSGETVOLUME           ; Funclet Resource number
   
    endif

    btst    #VOL_REG_BIT,x:MixerStatus
    jcs     _UseMixerMasterVolume

    move    x:HW_MIXMASTERVR,a
    move    x:HW_MIXMASTERVR,b
    jmp     _ProcessVolume

_UseMixerMasterVolume
    move    x:MixerMasterVol,a
    move    x:MixerMasterVol,b

_ProcessVolume
    move    #>MIX_LVOL_MASK,x0          ; Select the left volume field
    and     x0,a
    move    a1,x0                       ; Shift the field down 8 bits
    mpy     x0,#8,a
    move    a1,x0                       ; Normalize the value
    move    #>MIN_RVOL,a
    sub     x0,a

    ; Get the right volume value
    move    #>MIX_RVOL_MASK,x0          ; Select the right volume field
    and     x0,b
    move    b1,x0                       ; Normalize the value
    move    #>MIN_RVOL,b
    sub     x0,b

    ; Let's use the largest volume value
    cmp     a,b
    jlt     _Done
    move    b,a

_Done
    rts

      if (@DEF('FUNCLET'))
     
         ;If using funclet, switch counter back to original here
         org    p,"SYSMOD_SystemApi_P":                             
     
      endif

    org p,"SystemApi_P_overlay":
;///////////////////////////////////////////////////////////////////////////////
;> Name:        SystemShutdownFinal 
;
;  Type:        Function
;  Description: Wait for song to stop (if playing) then perform the final shutdown.
;
;  Inputs:      None.
;  Outputs:     None.
;  Notes:       May increase DCLK frequency to shorten shutdown time. 
;               ssf used below abbreviates function name.
;<
;///////////////////////////////////////////////////////////////////////////////
SystemShutdownFinal

    ; Increase DCLK speed for faster shutdown. SysSpeed.asm
    jsr     SysSpeedSysShutDown

    move    y:Ssf_ThisIsReallyIt,a
    tst     a
    jeq     _make_sure_everything_is_done       ; Jumps on the first execution.
    ;---------------------------------------------------------------------------
    ; Shut off the PLL
    bclr    #HW_CCR_CKSRC_BITPOS,x:HW_CCR

    ; give the LCD time to clear
    clr     b                               
    move    y:<Const_7fffff,b0                     
_wait
    dec     b
    jne     _wait

    ; RTC shutdown !!!! required (for 3410 only) to prevent RTC register corruption during dc/dc shutdown !!!!
    ;jsr     SysRTCShutdown ; not needed for STMP35xx so commented out. 
    ; End of time, baby!
    bclr    #HW_MIXTBR_PSRN_BITPOS,x:HW_MIXTBR  ;Must Clear PSRN bit first
    bset    #HW_CCR_PWDN_BITPOS,x:HW_CCR    ; turn off the DCDC converter
    ;   If using the DCDC converter then the part will turn off.  If not
    ;   using the DCDC converter then the part will just kinda stop working :)
    error                ; shouldn't get here. error macro halts or resets
    ;---------------------------------------------------------------------------

_make_sure_everything_is_done
    ;
    ; Check if we're waiting for a song to stop
    ;
    jsr     DecoderGetStatus
    jclr    #Stop,x0,_ssf_exit

    move    y:<Const_000001,x0
    move    x0,y:Ssf_ThisIsReallyIt
    
    ; Power down the headphones to reduce pop.
    jsr     FPowerDownHeadPhones

    ; Zero out the decoder memory area's before loading shutdown code
    jsr     SysFlushCoderMemory

    move	 x:HW_IPR,x0        ;Save IPR temporarily
    move     x0,y:SaveIPR

    move    #>0,x0
    move    x0,x:HW_IPR         ;Disable all interrupts while saving settings
                                ;This will avoid a shutdown of the system
                                ;in case a brownout is detected

    nop                         ;Make sure any pending interrupt will be serviced
    nop                         ;before jumping in SysSaveSettings
    nop


    ; Call the shutdown module
    jsr     SysSaveSettings
    
    ; We're powering down gracefully, so set the SkipCheckdisk bit.
    move    #HW_RTC_PERSIST1,R0
    move    y:<Const_000001,a
    jsr     FSetStickyBit

   move     y:SaveIPR,x0
   move     x0,x:HW_IPR

            
_ssf_exit
    ; post the final shutdown message so we'll get back here.
    move    y:<Const_000002,x0
    move    x0,y:UserScratchY
    move    #>SYSTEM_SHUTDOWN_FINAL,x0
    move    x0,y:UserScratchY+1
    move    #UserScratchY,r0
    move    y:<Const_ffffff,m0
    jsr     SysPostMessage

    rts

    endsec
