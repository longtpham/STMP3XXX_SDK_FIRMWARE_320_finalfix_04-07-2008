;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2001
;
; Filename:     mixer.asm
; Description:  STMP3 Mixer Module
;///////////////////////////////////////////////////////////////////////////////

    opt     mex

    section MIXMOD_Mixer

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include  "sysequ.inc"
    include  "msgequ.inc"
    include  "hwequ.inc"
    include  "resource.inc"
    include  "project.inc"
    include  "sysmacro.asm"
    list
    page    132,60,0,0,0

;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

    GLOBAL   MixerProcessQueue
    GLOBAL   MixerInit
    GLOBAL   MixerMasterFadeOut
    GLOBAL   MixerMasterFadeIn
    GLOBAL   MixerMasterMute
    GLOBAL   MixerMasterVol,FMixerMasterVol
    GLOBAL   MixerMasterBal,FMixerMasterBal
    GLOBAL   MIX_LVOL_MASK
    GLOBAL   MIX_RVOL_MASK
    GLOBAL   MIN_RVOL
    GLOBAL   MixerStatus
    GLOBAL   VOL_REG_BIT

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

        include   "syspostmsg.xref"
        include   "sysmem.xref"
        include   "systime.xref"
        include   "const.xref"

    if (@DEF('FUNCLET'))           
       extern    SysCallFunclet
    endif
    
;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////

MIX_MUTE_MASK   equ     $008000         ;Mixer Mute bit mask
MIX_LVOL_MASK   equ     $001f00         ;Mixer Left Channel Volume setting mask
MIX_RVOL_MASK   equ     $00001f         ;Mixer Right Channel Volume setting mask
MIX_INV_VOLMSK  equ     $ffe0e0         ;Mixer Inverse Volume mask
MIN_LVOL_MASK   equ     $006000         ;Mixer Left Channel Volume mask for value beyond min
MIN_RVOL_MASK   equ     $0000e0         ;Mixer Right Channel Volume mask for value beyond min
MIX_MIN_VOL     equ     $001f1f         ;Mixer Master Volume min left and right channel setting
                                        
MIN_LVOL        equ     $001f00         ;Minimum left channel volume
MIN_RVOL        equ     $00001f         ;Minimum right channel volume

    if (@def('DEVICE_3400')||@def('DEVICE_3300'))
    ; limit volume on 3400 due to instability 
MAX_LVOL        equ     $000500         ;Maximum left channel volume   (-7.5 dB)
MAX_RVOL        equ     $000005         ;Maximum right channel volume  (-7.5 dB)
    else
    ; full scale for 3410	
MAX_LVOL        equ     $000000         ;Maximum left channel volume   (0 dB)
MAX_RVOL        equ     $000000         ;Maximum right channel volume  (0 dB)
    endif

MAX_RBAL        equ    -31
MAX_LBAL        equ     31

;///////////////////////////////////////////////////////////////////////////////
;  X memory
;///////////////////////////////////////////////////////////////////////////////

    org     x,"MIXSYS_Mixer_X":

;Global variable: MixerMaster Volume setting before MixerMasterFadeOut routine
;was called.  This value will be used when MixerMasterFadeIn routine is called.
FMixerMasterVol
MixerMasterVol  dc      $000F0F         ; Default is half volume

;--------------------------------------------------------------------------------
VOL_REG_BIT     equ     0
VOL_MUTED       equ     1               ; Volume MUTEd due to VOL down button push
SYS_INIT_VOL_MUTE equ   2               ; Indicates the system init mute 
;--------------------------------------------------------------------------------
;   Bit 0 - 0 Volume Increments and Decrements are done in register
;           1 Volume Increments and Decrements are done in global var MixerMasterVol
;   Bit 1 - Volume Muted due to VOL down button push
;   Bit 2 - Indicates the system init mute. Initial value is set.
;           Cleared by first fade in and never reset.  

MixerStatus           dc   $000005      ; Volume Register Bit and sys init mute bit set


;///////////////////////////////////////////////////////////////////////////////
;  Y memory
;///////////////////////////////////////////////////////////////////////////////

    org y,"MIXSYS_Mixer_Y":

;All the way to the right is -31. All the way to the left is 31.
;Centered between left and right is 0.
FMixerMasterBal
MixerMasterBal   dc   $000000           ; Default is centered

    org     y,"MIXMOD_Mixer_settings_Y":
    SaveRange  MixerMasterVol,1
    SaveRange  MixerMasterBal,1

;///////////////////////////////////////////////////////////////////////////////
;   Code
;///////////////////////////////////////////////////////////////////////////////

    org p,"MIXMOD_Mixer_overlay_P":  

    ; Resource number
    dc      RSRC_MIXMOD_CODE

;///////////////////////////////////////////////////////////////////////////////
;> Name:        MixerInit
;
;  Type:        Function
;
;  Description: The mixer module's initial entry point
;
;  Inputs:      none
;  Outputs:     A contains the wait word specifying what events should trigger
;               calling this module's process routine.
;
;  Notes:       
;<
;///////////////////////////////////////////////////////////////////////////////

MixerInit
    move    #>EVENT_MESSAGE,A ; wait to call the mixer process func until msg available
    rts

;///////////////////////////////////////////////////////////////////////////////
;> Name:        MixerProcessQueue
;
;  Type:        Function
;
;  Description: This function processes the messages sent to the Mixer.
;               These messages include those for incrementing, decrementing, muting, 
;               unmuting, and setting levels for:
;                   Master Volume, Mic, Line, FM, DAC, and ADC.
;               A Record Select message can also be processed for choosing the recording source.
;
;  Inputs:      r0 = pointer to MixerQueueDescriptor
;               m0 = -1 for linear addressing mode
;
;  Outputs:     a1 word: This module requests to be called again when it has a message.
;
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////

MixerProcessQueue

    ; Put the queue modulo into m1
    move    #>BD_BufferModulo,n0
    nop
    move    y:(r0+n0),m1

    ; Put the head pointer into a
    move    #BD_HdPtr,n0
    nop
    move    y:(r0+n0),a

    ; Put the tail pointer into b
    move    #BD_TailPtr,n0
    nop
    move    y:(r0+n0),b

    ; Check if the queue is empty
    jmp     _CheckDone

_ProcessMessage

    ; Put the tail pointer into r1
    move    b1,r1

    ; Save working variables
    push    a1                          ; Head pointer
    push    r0                          ; Pointer to queue descriptor
    push    r1                          ; Tail pointer
    push    m1                          ; Queue modulo

    ; Put the message ID into b
    move    #>MSG_ID,n1
    nop
    move    y:(r1+n1),b

    ; Verify that the message ID is valid by checking message range
    move    #>MIXER_FIRST_MSG_ID,x0
    cmp     x0,b    #>MIXER_LAST_MSG_ID,x1
    jlt     _InvalidMessageID
    cmp     x1,b
    jgt     _InvalidMessageID

    ; Point r1 to the first argument of the message
    move    (r1)+
    move    (r1)+

    ; Clear the upper byte of the message ID
    move    #$00FFFF,x0
    and     x0,b    #_JumpTable,r2

    ; Jump to the message handler
    move    y:<Const_ffffff,m2
    move    b1,n2
    nop
    movem   p:(r2+n2),r3
    nop
    jmp     (r3)

_MasterIncrement
    btst    #VOL_REG_BIT,x:MixerStatus ;Check if mute set due to fade out
    jcs     _UseMixerMasterVolumeInc

    move    #>HW_MIXMASTERVR,r0
    jmp     _IncrementVolume

_UseMixerMasterVolumeInc
    move    #>MixerMasterVol,r0
        
_IncrementVolume        
    jsr     IncrementMasterLevel
    bclr    #VOL_MUTED,x:MixerStatus    ; bit clear allows MixerMasterFadeIn to run
    jmp     _NextMessage

_MasterDecrement
    btst    #VOL_REG_BIT,x:MixerStatus  ; Check if mute was set due to fade out
    jcs     _UseMixerMasterVolumeDec

    move    #>HW_MIXMASTERVR,r0
    jmp     _DecrementVolume

_UseMixerMasterVolumeDec
    move    #>MixerMasterVol,r0

_DecrementVolume            
    jsr     DecrementLevel
    move    x:(r0),x0
    jclr    #15,x0,_gotoNextMsg
    bset    #VOL_MUTED,x:MixerStatus    
    ; Bit set usually prevents MixerMasterFadeIn from running. 
    ; Exception: Initial system mute is always undone by MixerMasterFadeIn.
_gotoNextMsg
    jmp     _NextMessage

_MasterSetLevel
    btst    #VOL_REG_BIT,x:MixerStatus  ; Check if mute was set due to fade out
    jcs     _UseMixerMasterSetLevel

    move    #>HW_MIXMASTERVR,r0
    jmp     _SetLevel

_UseMixerMasterSetLevel
    move    #>MixerMasterVol,r0

_SetLevel
    move    y:(r1),x1                   ; Level
    jsr     SetLevel
    jmp     _NextMessage

_MasterMute
    bset     #HW_MIXVOLUMER_MUTE_BITPOS,x:HW_MIXMASTERVR
    jmp     _NextMessage

_MasterUnmute
    bclr    #HW_MIXVOLUMER_MUTE_BITPOS,x:HW_MIXMASTERVR
    jmp     _NextMessage

_MasterFadeOut
    move    y:(r1),x0                   ; Step delay
    jsr     MixerMasterFadeOut
    jmp     _NextMessage

_MasterFadeIn
    move    y:(r1),x0                   ; Step delay
    jsr     MixerMasterFadeIn
    jmp     _NextMessage

_MasterBalanceRight
    jsr     MixerMasterBalRight
    jmp     _NextMessage

_MasterBalanceLeft
    jsr     MixerMasterBalLeft
    jmp     _NextMessage

_MicIncrement
    move    #>HW_MIXMICINVR,r0
    jsr     IncrementMicLevel
    jmp     _NextMessage

_MicDecrement
    move    #>HW_MIXMICINVR,r0
    jsr     DecrementMicLevel
    jmp     _NextMessage

_MicSetLevel
    move    #>HW_MIXMICINVR,r0
    move    y:(r1),x1                   ; Level
    jsr     SetMicLevel
    jmp     _NextMessage

_MicMute
    bset     #15,x:HW_MIXMICINVR
    jmp     _NextMessage

_MicUnmute
    bclr    #15,x:HW_MIXMICINVR
    jmp     _NextMessage

_MicBoost
    bset     #6,x:HW_MIXMICINVR
    jmp     _NextMessage

_MicUnboost
    bclr    #6,x:HW_MIXMICINVR
    jmp     _NextMessage

_LineIncrement
    move    #>HW_MIXLINE1INVR,r0
    jsr     IncrementLevel
    jmp     _NextMessage

_LineDecrement
    move    #>HW_MIXLINE1INVR,r0
    jsr     DecrementLevel
    jmp     _NextMessage

_LineSetLevel
    move    #>HW_MIXLINE1INVR,r0
    move    y:(r1),x1                   ; Level
    jsr     SetLevel
    jmp     _NextMessage

_LineMute
    bset     #15,x:HW_MIXLINE1INVR
    jmp     _NextMessage

_LineUnmute
    bclr    #15,x:HW_MIXLINE1INVR
    jmp     _NextMessage

_FMIncrement
  if (@DEF('FMTUNER_ON_LINE2_IN'))           
    move    #>HW_MIXLINE2INVR,r0
  else
    move    #>HW_MIXLINE1INVR,r0
  endif
    jsr     IncrementLevel
    jmp     _NextMessage

_FMDecrement
  if (@DEF('FMTUNER_ON_LINE2_IN'))           
    move    #>HW_MIXLINE2INVR,r0
  else
    move    #>HW_MIXLINE1INVR,r0
  endif
    jsr     DecrementLevel
    jmp     _NextMessage

_FMSetLevel
  if (@DEF('FMTUNER_ON_LINE2_IN'))           
    move    #>HW_MIXLINE2INVR,r0
  else
    move    #>HW_MIXLINE1INVR,r0
  endif
    move    y:(r1),x1                   ; Level
    jsr     SetLevel
    jmp     _NextMessage

_FMMute
  if (@DEF('FMTUNER_ON_LINE2_IN'))           
    bset     #15,x:HW_MIXLINE2INVR
  else
    bset     #15,x:HW_MIXLINE1INVR
  endif
    jmp     _NextMessage

_FMUnmute
  if (@DEF('FMTUNER_ON_LINE2_IN'))           
    bclr    #15,x:HW_MIXLINE2INVR
  else
    bclr    #15,x:HW_MIXLINE1INVR
  endif
    jmp     _NextMessage

_DACIncrement
    move    #>HW_MIXDACINVR,r0
    jsr     IncrementLevel
    jmp     _NextMessage

_DACDecrement
    move    #>HW_MIXDACINVR,r0
    jsr     DecrementLevel
    jmp     _NextMessage

_DACSetLevel
    move    #>HW_MIXDACINVR,r0
    move    y:(r1),x1                   ; Level
    jsr     SetLevel
    jmp     _NextMessage

_DACMute
    bset     #15,x:HW_MIXDACINVR
    jmp     _NextMessage

_DACUnmute
    bclr    #15,x:HW_MIXDACINVR
    jmp     _NextMessage

_ADCSelect
    move    y:(r1),x1                   ; Select
    move    x1,x:HW_MIXRECSELR
    jmp     _NextMessage

_ADCIncrement
    move    #>HW_MIXADCGAINR,r0
    jsr     IncrementLevel
    jmp     _NextMessage

_ADCDecrement
    move    #>HW_MIXADCGAINR,r0
    jsr     DecrementLevel
    jmp     _NextMessage

_ADCSetLevel
    move    #>HW_MIXADCGAINR,r0
    move    y:(r1),x1                   ; Level
    jsr     SetLevel
    jmp     _NextMessage

_ADCMute
    bset     #15,x:HW_MIXADCGAINR
    jmp     _NextMessage

_ADCUnmute
    bclr    #15,x:HW_MIXADCGAINR
    jmp     _NextMessage

_RecordSelect
    move    y:(r1)+,x0                  ; left
    move    y:(r1),x1                   ; right
    jsr     RecordSelect
    jmp     _NextMessage

_MixerPowerDown
    ; Used to power down the analog mixer when it is to be bypassed,
    ; or to power it up when it is to be used (FM Tuner, LineIn, etc).
    ; Also known as DAC BYPASS MODE.
    ; Input parameter: 0 = Analog Mixer ON , 1 = PowerDown the analog mixer (BYPASS mode)
    move    y:(r1)+,a                   ; 0 = Turn mixer ON , 1 = Power it down (set the bypass bit)

    ; IGNORE THIS MESSAGE IF YOU ARE A 3400 or 3300!!
    if (!(@def('DEVICE_3400')||@def('DEVICE_3300')))
    tst     a
    jeq     _PowerUp
_PowerDown
    bset    #HW_MIXPWRDNR_PR2_BITPOS,x:HW_MIXPWRDNR
    jmp     _NextMessage
_PowerUp
    bclr    #HW_MIXPWRDNR_PR2_BITPOS,x:HW_MIXPWRDNR
    endif   ; (!(@def('DEVICE_3400')||@def('DEVICE_3300')))

    jmp     _NextMessage

_EnableRightChannel
    bclr     #HW_MIXTBR_PW_ADC_RIGHT_CH_BITPOS,x:HW_MIXTBR
    jmp     _NextMessage

_DisableRightChannel
    bset     #HW_MIXTBR_PW_ADC_RIGHT_CH_BITPOS,x:HW_MIXTBR
    jmp     _NextMessage

_InvalidMessageID
    error

_NextMessage
    ; Restore working variables
    pop     m1                          ; Queue modulo
    pop     r1                          ; Tail pointer
    pop     r0                          ; Pointer to buffer descriptor
    pop     a                           ; Head pointer

    ; Add the message length to the tail pointer
    move    y:(r1),n1
    nop
    move    (r1)+n1
    nop
    move    r1,b

_CheckDone
    ; Check if head pointer equals the tail pointer
    cmp     a,b
    jne     _ProcessMessage

    ; Update the buffer descriptor with the new tail pointer
    move    #>BD_TailPtr,n0
    nop
    move    b1,y:(r0+n0)

    ; I want to be called again when I have a message
    move    #EVENT_MESSAGE,a1
    move   y:<Const_ffffff,m1

    rts

_JumpTable
    dc      _MasterIncrement
    dc      _MasterDecrement
    dc      _MasterSetLevel
    dc      _MasterMute
    dc      _MasterUnmute
    dc      _MasterFadeOut
    dc      _MasterFadeIn
    dc      _MasterBalanceRight
    dc      _MasterBalanceLeft
    dc      _MicIncrement
    dc      _MicDecrement
    dc      _MicSetLevel
    dc      _MicMute
    dc      _MicUnmute
    dc      _MicBoost
    dc      _MicUnboost
    dc      _LineIncrement
    dc      _LineDecrement
    dc      _LineSetLevel
    dc      _LineMute
    dc      _LineUnmute
    dc      _FMIncrement
    dc      _FMDecrement
    dc      _FMSetLevel
    dc      _FMMute
    dc      _FMUnmute
    dc      _DACIncrement
    dc      _DACDecrement
    dc      _DACSetLevel
    dc      _DACMute
    dc      _DACUnmute
    dc      _ADCSelect
    dc      _ADCIncrement
    dc      _ADCDecrement
    dc      _ADCSetLevel
    dc      _ADCMute
    dc      _ADCUnmute
    dc      _RecordSelect
    dc      _MixerPowerDown
    dc      _EnableRightChannel
    dc      _DisableRightChannel
; Note: It is important that the entries in the jump table be in the
; same order as the mixer message ID's which are defined in msgequ.inc

;///////////////////////////////////////////////////////////////////////////////
;> Name:        SetLevel
;
;  Type:        Function
;
;  Description: This function sets a volume register to a level specified in the
;               x1 register. If x1 is not within a given range, then the level is
;               automatically limited to the max or min value.
;               
;               The calling routine should be aware that bits 0-4 specify the right
;               channel and bits 8-12 specify the left channel. Bit 15 is the mute
;               bit. This routine will change the Mute bit as indicated by the value
;               of bit 15 in the input level specified in the x1 register.
;
;  Inputs:      r0 = pointer to a volume register (excluding microphone)
;               m0 = linear
;               x1 = level to set volume to
;  Outputs:     Volume adjusted to new level.
;
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////

SetLevel
    ;Is argument left channel value exceed minimum allowed volume setting?
    move    #>MIN_LVOL_MASK,a
    and     x1,a                        ;Zero out every bit in argument except bit[15..13]
    jne     _MixLMin                    ;Is argument value for left channel exceed min. allowed?
    move    #>MIX_LVOL_MASK,a
    and     x1,a                        ;Extract desired left channel volume setting
    jmp     _MixRVol

_MixLMin:
    move    #>MIN_LVOL,a                ;Set left channel to minimum value allowed

_MixRVol:
    ;Is argument right channel value exceed minimum allowed volume setting?
    move    #>MIN_RVOL_MASK,b
    and     x1,b                        ;Zero out every bit in argument except bit[7..5]
    jne     _MixRMin                    ;Is argument value for right channel exceed min. allowed?
    move    #>MIX_RVOL_MASK,b
    and     x1,b                        ;Extract desired right channel volume setting
    jmp     _MixMute

_MixRMin:
    move    #>MIN_RVOL,b                ;Set right channel to minimum value allowed

_MixMute:
    move    b1,y0
    or      y0,a1                       ;Form new left and right channel value for volume reg

    ;Form new volume reg value with the desired mute bit setting
    btst    #HW_MIXVOLUMER_MUTE_BITPOS,x1 ;Is Mute bit of argument set?
    jcc     _MixSet
    bset    #HW_MIXVOLUMER_MUTE_BITPOS,a1 ;Set the mute bit of the new volume value

_MixSet:
    move    a1,x:(r0)                   ;Update desired STMP3400/3410 volume reg with new volume setting
    rts

;///////////////////////////////////////////////////////////////////////////////
;> Name:        SetMicLevel
;
;  Type:        Function
;
;  Description: This function sets the microphone volume register to a level 
;               specified in the x1 register. If x1 is not within a given range, 
;               then the level is automatically limited to the max or min value.
;               
;               The calling routine should be aware that bits 0-4 specify the right
;               channel and bits 8-12 specify the left channel. Bit 15 is the mute
;               bit. This routine will change the Mute bit as indicated by the value
;               of bit 15 in the input level specified in the x1 register.
;
;  Inputs:      r0 = pointer to microphone volume register
;               m0 = linear
;               x1 = level to set mic volume to
;  Outputs:     Microphone volume adjusted to new level.
;
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////

SetMicLevel
    ;Is argument right channel value exceeding minimum allowed volume setting?
    move    y:<Const_000020,b           ;Mask for beyond limit
    and     x1,b                        ;Zero out every bit in argument except bit[7..5]
    jne     _MixRMin                    ;Is argument value for right channel exceed min. allowed?
    move    #>MIX_RVOL_MASK,b
    and     x1,b                        ;Extract desired right channel volume setting
    jmp     _MixMute

_MixRMin:
    move    #>MIN_RVOL,b                ;Set right channel to minimum value allowed

_MixMute:
    ;Form new volume reg value with the desired mute bit setting
    btst    #HW_MIXMICINVR_MUTE_BITPOS,x1 ;Is Mute bit of argument set?
    jcc     _MixBoost
    bset    #HW_MIXMICINVR_MUTE_BITPOS,b1 ;Set the mute bit of the new volume value

_MixBoost
    ;Form new volume reg value with the desired boost bit setting
    btst    #HW_MIXMICINVR_P20DB_BITPOS,x1 ;Is boost bit set?
    jcc     _MixSet
    bset    #HW_MIXMICINVR_P20DB_BITPOS,b1 ;Set the boost bit

_MixSet:
    move    b1,x:(r0)                   ;Update STMP34xx/STMP35xx volume reg with new volume setting
    rts

;///////////////////////////////////////////////////////////////////////////////
;> Name:        IncrementLevel
;
;  Type:        Function
;
;  Description: This function increments a specified volume register, excluding
;               the microphone and master volumes. 
;
;               If Muted:
;                   Unmute and exit
;               Else:
;                   If Max Volume:
;                       Exit
;                   Else:
;                       Increment the Left and Right volume by one level
;
;  Inputs:      r0 = pointer to a volume register (excluding microphone and master)
;               m0 = linear
;  Outputs:     Volume is incremented, mute bit is cleared
;
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////

IncrementLevel
    ; If the mute is on and the left and right volumes are at minimum
    move    #>MIX_MUTE_MASK+MIX_RVOL_MASK+MIX_LVOL_MASK,x0
    move    x:(r0),a
    and     x0,a                        ; If (vol & mask) == mask
    cmp     x0,a
    jne     _Increment

    ; Turn off the mute and exit
    bclr    #15,x:(r0)
    jmp     _Exit

_Increment
    ; If the left volume is at maximum (zero)
    move    x:(r0),a
    move    #>MIX_LVOL_MASK,x0
    and     x0,a
    jeq     _Exit                       ; Exit

    ; If the right voulme is at maximum (zero)
    move    x:(r0),b
    move    #>MIX_RVOL_MASK,x0
    and     x0,b
    jeq     _Exit                       ; Exit

    ; Increment the left volume
    move    y:<Const_000100,x0
    sub     x0,a

    ; Increment the right volume
    move    y:<Const_000001,x0
    sub     x0,b

    ; Form new volume reg value while preserving mute bit
    move    b1,y0
    or      y0,a1                       ; Combine the left and right volumes
    move    x:(r0),b
    move    #>MIX_INV_VOLMSK,y1
    and     y1,b                        ;These instruction ensure all non volume bits remain
    move    b1,y1                       ;unchanged.
    or      y1,a1
    move    a1,x:(r0)                   ;Update STMP3400 volume reg with new volume setting

_Exit
    rts

;///////////////////////////////////////////////////////////////////////////////
;> Name:        IncrementMicLevel
;
;  Type:        Function
;
;  Description: This function increments the microphone volume register.
;
;               If Muted & Min Vol:
;                   Unmute and exit
;               Else:
;                   If Max Volume:
;                       Exit
;                   Else:
;                       Increment the Left and Right volume by one level
;
;  Inputs:      r0 = pointer to the microphone volume register
;               m0 = linear
;  Outputs:     Volume is incremented, mute bit is cleared
;
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////

IncrementMicLevel
    ; If the mute is on and the volume is at minimum
    move    #>MIX_MUTE_MASK+MIX_RVOL_MASK,x0
    move    x:(r0),a
    and     x0,a                        ; If (vol & mask) == mask
    cmp     x0,a
    jne     _Increment

    ; Turn off the mute and exit
    bclr    #15,x:(r0)
    jmp     _Exit

_Increment
    ; If the volume is at maximum (zero)
    move    x:(r0),b
    move    #>MIX_RVOL_MASK,x0
    and     x0,b
    jeq     _Exit                       ; Exit

    ; Increment the volume
    move    y:<Const_000001,x0
    sub     x0,b

    ; Form new volume reg value while preserving mute bit and boost bit
    move    b1,a1
    move    x:(r0),b
    move    #>MIX_INV_VOLMSK,y1
    and     y1,b                        ;These instruction ensure all non volume bits remain
    move    b1,y1                       ;unchanged.
    or      y1,a1
    move    a1,x:(r0)                   ;Update STMP3400 volume reg with new volume setting
_Exit
    rts

;///////////////////////////////////////////////////////////////////////////////
;> Name:        IncrementMasterLevel
;
;  Type:        Function
;
;  Description: This function increments the master volume register.
;
;               If Muted & Min Vol:
;                   Unmute and exit
;               Else:
;                   If Max Volume:
;                       Exit
;                   Else:
;                       Increment the Left and Right volume by one level
;
;  Inputs:      r0 = pointer to the master volume register
;               m0 = linear
;  Outputs:     Volume is incremented, mute bit is cleared
;
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////

IncrementMasterLevel
    ; If the mute is on and the left and right volumes are at minimum
    move    #>MIX_MUTE_MASK+MIX_RVOL_MASK+MIX_LVOL_MASK,x0
    move    x:(r0),a
    and     x0,a                        ; If (vol & mask) == mask
    cmp     x0,a
    jne     _Increment

    ; Turn off the mute and exit
    bclr    #15,x:(r0)
    jmp     _Exit

_Increment
    ; If the left volume is at maximum 
    move    x:(r0),a
    move    #>MIX_LVOL_MASK,x0
    and     x0,a    #>MAX_LVOL,x0       ;Discard bits that are not left volume
    move    a1,a                        ;Make sure a0 and a2 are clear
    cmp     x0,a                        ;Compare volume vs max volume
    jle     _Exit
        
    ; If the right volume is at maximum 
    move    x:(r0),b
    move    #>MIX_RVOL_MASK,x0
    and     x0,b    #>MAX_RVOL,x0       ;Discard bits that are not left volume
    cmp     x0,b                        ;Compare volume vs. max volume
    jle     _Exit

    ; Save right and left volumes
    move    a1,y1                       ; Left volume
    move    b1,y0                       ; Right volume

    ; if (right - left) <= balance
    mpy     y1,#8,a                     ; Shift "left" down 8 bits
    sub     a,b                         ; Subtract "left" from "right"
    move    y:MixerMasterBal,a
    cmp     a,b
    jgt     _IncRight

    ; Increment the left volume
    move    y1,a
    move    y:<Const_000100,x0
    sub     x0,a
    move    a1,y1

_IncRight
    ; If (right - left) >= balance
    move    y:MixerMasterBal,a
    cmp     a,b
    jlt     _SaveIt

    ; Increment the right volume
    move    y0,b
    move    y:<Const_000001,x0
    sub     x0,b
    move    b1,y0

_SaveIt
    ; Form new volume reg value while preserving mute bit and boost bit
    move    y1,a
    or      y0,a1                       ; Combine the left and right volumes
    move    x:(r0),b
    move    #>MIX_INV_VOLMSK,y1
    and     y1,b                        ;These instruction ensure all non volume bits remain
    move    b1,y1                       ;unchanged.
    or      y1,a1
    move    a1,x:(r0)                   ;Update STMP3400 volume reg with new volume setting
_Exit
    rts

;///////////////////////////////////////////////////////////////////////////////
;> Name:        DecrementLevel
;
;  Type:        Function
;
;  Description: This function decrements a specified volume register, excluding
;               the microphone. 
;
;               If Min Vol (L&R):
;                   Mute and exit
;               Else:
;                   If Left Vol != Min Volume:
;                       Decrement Left Volume
;                   If Right Vol != Min Volume:
;                       Decrement Right Volume
;
;  Inputs:      r0 = pointer to a volume register (excluding microphone)
;               m0 = linear
;  Outputs:     Volume is decremented
;
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////

DecrementLevel
    ; If the left and right volumes are both at minimum
    move    #>MIX_RVOL_MASK+MIX_LVOL_MASK,x0
    move    x:(r0),a
    and     x0,a                        ; If (vol & mask) == mask
    cmp     x0,a
    jne     _DecLeft

    ; Turn on the mute and exit
    bset    #15,x:(r0)
    jmp     _Exit

_DecLeft
    ; If the left volume is not at minimum (1F)
    move    x:(r0),a
    move    #>MIN_LVOL,x0
    and     x0,a
    cmp     x0,a
    jeq     _DecRight

    ; Decrement the left volume
    move    y:<Const_000100,x0
    add     x0,a

_DecRight
    ; If the right volume is not at minimum (1F)
    move    x:(r0),b
    move    #>MIN_RVOL,y0
    and     y0,b
    cmp     y0,b
    jeq     _SaveIt

    ; Decrement the right volume
    move    y:<Const_000001,y0
    add     y0,b

_SaveIt
    ; Form new volume reg value while preserving mute bit
    move    b1,y0
    or      y0,a1                       ; Combine the left and right volumes
    move    x:(r0),b
    move    #>MIX_INV_VOLMSK,y1
    and     y1,b                        ;These instruction ensure all non volume bits remain
    move    b1,y1                       ;unchanged.
    or      y1,a1
    move    a1,x:(r0)                   ;Update STMP3400/3410 volume reg with new volume setting

_Exit
    rts

;///////////////////////////////////////////////////////////////////////////////
;> Name:        DecrementMicLevel
;
;  Type:        Function
;
;  Description: This function decrements a the microphone volume register.
;
;               If Min Vol:
;                   Mute and exit
;               Else:
;                   Decrement Mic volume
;
;  Inputs:      r0 = pointer to microphone volume register
;               m0 = linear
;  Outputs:     Volume is decremented
;
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////

DecrementMicLevel
    ; If the microphone volume is at minimum
    move    #>MIX_RVOL_MASK,x0
    move    x:(r0),a
    and     x0,a                        ; If (vol & mask) == mask
    cmp     x0,a
    jne     _Decrement

    ; Turn on the mute and exit
    bset    #15,x:(r0)
    jmp     _Exit

_Decrement
    ; Decrement the mic volume
    move    y:<Const_000001,y0
    add     y0,a

    ; Form new volume reg value while preserving mute bit and boost bit
    move    x:(r0),b
    move    #>MIX_INV_VOLMSK,y1
    and     y1,b                        ;These instruction ensure all non volume bits remain
    move    b1,y1                       ;unchanged.
    or      y1,a1
    move    a1,x:(r0)                   ;Update STMP3400 volume reg with new volume setting

_Exit
    rts

;///////////////////////////////////////////////////////////////////////////////
;> Name:        MixerMasterBalRight
;
;  Type:        Function
;
;  Description: This function will move the balance to the right (decrement the balance).
;               If Left Vol > Right Vol     : Then Inc Right
;               ElseIf Left Vol != Min Vol  : Then Dec Left
;               Else                        : Dec Balance
;
;  Inputs:      None.
;  Outputs:     Balance is adjusted to right.
;
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////

MixerMasterBalRight
    ; Get the left volume
    move    x:HW_MIXMASTERVR,a
    move    #>MIX_LVOL_MASK,x0
    and     x0,a

    ; Get the right volume
    move    x:HW_MIXMASTERVR,b
    move    #>MIX_RVOL_MASK,y0
    and     y0,b

    ; Save right and left volumes
    move    a1,y1                       ; Left volume
    move    b1,y0                       ; Right volume

    ; If left >= right
    mpy     y1,#8,a                     ; Shift "left" down 8 bits
    sub     a,b                         ; Subtract "left" from "right"
    jgt     _IncRight

    ; If the left volume is not at minimum (1F)
    move    y1,a
    move    #>MIN_LVOL,x0
    and     x0,a
    cmp     x0,a
    jeq     _DecBalance

    ; Decrement the left volume
    move    y:<Const_000100,x0
    add     x0,a
    move    a1,y1
    jmp     _SaveIt

_IncRight
    ; if (right - left) <= balance
    move    y:MixerMasterBal,a
    cmp     a,b
    jlt     _SaveIt

    ; Increment the right volume
    move    y0,b
    move    y:<Const_000001,x0
    sub     x0,b
    move    b1,y0

_SaveIt
    ; Form new volume reg value while preserving mute bit
    move    y1,a
    or      y0,a1                       ; Combine the left and right volumes
    move    x:HW_MIXMASTERVR,b
    move    #>MIX_INV_VOLMSK,y1
    and     y1,b                        ;These instruction ensure all non volume bits remain
    move    b1,y1                       ;unchanged.
    or      y1,a1
    move    a1,x:HW_MIXMASTERVR         ;Update STMP3400 volume reg with new volume setting

_DecBalance
    ; If the balance is at maximum right (-31)
    move    y:MixerMasterBal,a
    move    #>MAX_RBAL,x0
    cmp     x0,a
    jeq     _Exit                       ; Exit

    ; Decrement the balance
    move    y:<Const_000001,x0
    sub     x0,a
    move    a1,y:MixerMasterBal

_Exit
    rts

;///////////////////////////////////////////////////////////////////////////////
;> Name:        MixerMasterBalLeft
;
;  Type:        Function
;
;  Description: This function will move the balance to the left (increment the balance).
;               If Right Vol > Left Vol     : Then Inc Left
;               ElseIf Right Vol != Min Vol : Then Inc Balance
;               Else                        : Dec Right
;
;  Inputs:      None.
;  Outputs:     Balance is adjusted to left.
;
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////

MixerMasterBalLeft

    ; Get the left volume
    move    x:HW_MIXMASTERVR,a
    move    #>MIX_LVOL_MASK,x0
    and     x0,a

    ; Get the right volume
    move    x:HW_MIXMASTERVR,b
    move    #>MIX_RVOL_MASK,y0
    and     y0,b

    ; Save right and left volumes
    move    a1,y1                       ; Left volume
    move    b1,y0                       ; Right volume

    ; If right >= left
    mpy     y1,#8,a                     ; Shift "left" down 8 bits
    sub     a,b                         ; Subtract "left" from "right"
    jlt     _IncLeft

    ; If the right volume is not at minimum (1F)
    move    y0,b
    move    #>MIN_RVOL,x0
    and     x0,b
    cmp     x0,b
    jeq     _IncBalance

    ; Decrement the right volume
    move    y:<Const_000001,x0
    add     x0,b
    move    b1,y0
    jmp     _SaveIt

_IncLeft
    ; if (right - left) <= balance
    move    y:MixerMasterBal,a
    cmp     a,b
    jgt     _SaveIt

    ; Increment the left volume
    move    y1,a
    move    y:<Const_000100,x0
    sub     x0,a
    move    a1,y1

_SaveIt
    ; Form new volume reg value while preserving mute bit
    move    y1,a
    or      y0,a1                       ; Combine the left and right volumes
    move    x:HW_MIXMASTERVR,b
    move    #>MIX_INV_VOLMSK,y1
    and     y1,b                        ;These instruction ensure all non volume bits remain
    move    b1,y1                       ;unchanged.
    or      y1,a1
    move    a1,x:HW_MIXMASTERVR         ;Update STMP3400 volume reg with new volume setting

_IncBalance
    ; If the balance is at maximum left (31)
    move    y:MixerMasterBal,a
    move    #>MAX_LBAL,x0
    cmp     x0,a
    jeq     _Exit                       ; Exit

    ; Increment the balance
    move    y:<Const_000001,x0
    add     x0,a
    move    a1,y:MixerMasterBal

_Exit
    rts

;///////////////////////////////////////////////////////////////////////////////
;> Name:        RecordSelect
;
;  Type:        Function
;
;  Description: This function is used to select the sources for the left and right
;               record inputs. The ADC allows mix-and-match of the various sources.
;               The sources allowed are the following: mic, FM-in, line-in, and stereo
;               mixer. Left and right record inputs can be set to different sources.
;
;  Inputs:      x0 = left channel record source
;               x1 = right channel record source
;  Outputs:     Record source is set for left and right channels.
;
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////

RecordSelect
    move    x:HW_MIXRECSELR,a           ; get the current reg contents
    move    #>HW_MIXRECSELR_SR_SL_CLRMASK,y0
    and     y0,a                        ; clear out old source values
    or      x0,a                        ; set the left channel
    or      x1,a                        ; set the right channel
    move    a,x:HW_MIXRECSELR           ; make it so
    rts

;///////////////////////////////////////////////////////////////////////////////
;   Permanently resident code
;///////////////////////////////////////////////////////////////////////////////

    org     p,"MIXSYS_Mixer_P":

;///////////////////////////////////////////////////////////////////////////////
;> Name:        MixerMasterFadeOut
;
;  Type:        Function
;
;  Description: This function will fade out the Master Volume. The fade out
;               consists of decreasing the left and/or right channel volume(s)
;               independently as needed until both channels are at their minimum
;               volume settings then the MUTE bit will be set. 
;
;               The original HW_MIXMASTERVR reg content is saved in a global variable at
;               the start of this routine.  This saved global value will be used to
;               restore the HW_MIXMASTERVR reg when the MixerMasterFadeIn routine is
;               executed.
;
;               Assumes MUTE bit is not set when this routine is 1st called and pretend
;               MUTE bit is not set if it is set.
;
;  Inputs:      x0 = Number of milliseconds of delay between fade out steps
;  Outputs: 
;
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////
MixerMasterFadeOut

 if (@DEF('FUNCLET'))           

    ;If using funclet, function will be executed via funclet kernel
    CallFunclet RSRC_FUNCLET_MIXER_MASTER_FADE_OUT

    org p,"SYSFUNCLET_MixerMasterFadeOut_P":
    
    dc      RSRC_FUNCLET_MIXER_MASTER_FADE_OUT           ; Funclet Resource number

 endif


    push    x0                          ; Save the delay value

    move    x:HW_MIXMASTERVR,a
    move    a1,x:MixerMasterVol         ;Save HW_MIXMASTERVR reg value in global variable
	; check for fast audio fade-out, x0 = 0
	clr		b
	cmp		x0,b
    jne     _MoreMixFadeO               ;Continue decreasing left/right volume level
	move	#>$009f1f,x0
	move	x0,x:HW_MIXMASTERVR
	pop		x0
	jmp		_MixFadeOMute

_MoreMixFadeO:
    move    #>MIN_LVOL,x0
    and     x0,a   #>MIN_RVOL,y0        ;Zero out all non ML[4..0] bit of VOLMASTR reg
                                        ;y0 reg should not be changed by this routine from this point
    move    x:HW_MIXMASTERVR,b          ;b reg should only be changed by "add" instruction below
    and     y0,b   y:<Const_000100,x1   ;Zero out all non MR[4..0] bit of HW_MIXMASTERVR reg
                                        ;x1 reg should not be changed by this routine from this point
    move    y:<Const_000001,y1          ;y1 reg should not be changed by this routine from this point

    cmp     x0,a
    jeq     _MixFadeORight              ;Has left channel volume already reach minimum setting?
    add     x1,a                        ;Decrease left channel volume setting by one

_MixFadeORight:
    cmp     y0,b
    jeq     _MixFadeONext               ;Has right channel volume already reach minimum setting?
    add     y1,b                        ;Decrease right channel volume setting by one

_MixFadeONext:                          ;Form new HW_MIXMASTERVR reg value
    move    b1,x0
    or      x0,a1                       ;Form new left and right channel value for HW_MIXMASTERVR reg
    move    a1,x:HW_MIXMASTERVR         ;Update STMP3400 HW_MIXMASTERVR reg with new volume setting

    pop     x0                          ;Restore the delay value

    ;Check to see if both channel are set at minimum volume level
    move    #>MIX_MIN_VOL,x1
    cmp     x1,a                        ;Are both channel volume level at minimum volume setting?
    jeq     _MixFadeOMute

    push    x0                          ; Save the delay value

    jsr     SysDelay

    move    x:HW_MIXMASTERVR,a
    jmp     _MoreMixFadeO               ;Continue decreasing left/right volume level

_MixFadeOMute:
    bset    #15,x:HW_MIXMASTERVR        ;Set MUTE bit of HW_MIXMASTERVR reg as final Fade Out step
    bset    #VOL_REG_BIT,x:MixerStatus
    rts

 if (@DEF('FUNCLET'))
 
    ;If using funclet, switch counter back to original here
    org    p,"MIXSYS_Mixer_P":                             

 endif

;///////////////////////////////////////////////////////////////////////////////
;> Name:        MixerMasterFadeIn
;
;  Type:        Function
;
;  Description: This function will fade in the Master Volume to the saved value.
;               The fade in consists of increasing the left and/or right channel volume(s)
;               independently as needed until both channels are restored to the value
;               saved in the global variable by the MixerMasterFadeOut routine when it
;               was executed.
;
;               MUTE bit is cleared automatically if set.
;
;  Inputs:      x0 = Number of milliseconds of delay between fade in steps
;  Outputs: 
;
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////

MixerMasterFadeIn
 if (@DEF('FUNCLET'))           

    ;If using funclet, function will be executed via funclet kernel
    CallFunclet RSRC_FUNCLET_MIXER_MASTER_FADE_IN

    org p,"SYSFUNCLET_MixerMasterFadeIn_P":
    
    dc      RSRC_FUNCLET_MIXER_MASTER_FADE_IN           ; Funclet Resource number

 endif

    move    x:MixerStatus,x1            ; check if VOL_MUTED set due to VOL down

    jclr    #VOL_MUTED,x1,_BeginFadeIn

    ; Allow fade-in to undo mute if this MixerStatus mute case is the first (from 
    ; system init). Note that the default state of SYS_INIT_VOL_MUTE bit is set or true.
    jset    #SYS_INIT_VOL_MUTE,x1,_UndoInitialMute

    bclr    #VOL_REG_BIT,x1
    move    x1,x:MixerStatus
    rts

    ; Clear MixerStatus:VOL_MUTED bit to false as this is the initial system mute. 
    ; Continues to _BeginFadeIn which undoes mute.
_UndoInitialMute
    bclr    #SYS_INIT_VOL_MUTE,x1
    bclr    #VOL_MUTED,x1
    move    x1,x:MixerStatus
    
_BeginFadeIn
    push    x0                          ; Save the delay value

    ;Start fade in by initializing left and right channel to minimum volume setting
    move    #>MIX_MIN_VOL,a
    move    a1,x:HW_MIXMASTERVR

    bclr    #15,x:MixerMasterVol ;Make sure MUTE bit of global variable is cleared
    bclr    #VOL_REG_BIT,x:MixerStatus

_MoreMixFadeI:
    ;Get final fade in left and right channel value from global variable
    move    x:MixerMasterVol,x0         ;Get HW_MIXMASTERVR reg value saved in global variable
    move    #>MIX_LVOL_MASK,a
    and     x0,a                        ;Zero out all non ML[4..0] bit of HW_MIXMASTERVR reg
    move    a1,x1                       ;x1 reg should not be changed by this routine from this point

    move    #>MIX_RVOL_MASK,b
    and     x0,b                        ;Zero out all non ML[4..0] bit of HW_MIXMASTERVR reg
    move    b1,y1                       ;y1 reg should not be changed by this routine from this point

    move    x:HW_MIXMASTERVR,a
    move    #>MIX_LVOL_MASK,x0
    and     x0,a                        ;Zero out all non ML[4..0] bit of HW_MIXMASTERVR reg

    cmp     x1,a   y:<Const_000100,x0   ;Compare current left with target left
                                        ;x0=Value by which to increase volume one higher at a time
    jeq     _MixFadeIRight              ;Has left channel volume reach final desired setting?
    sub     x0,a                        ;Increase left channel volume setting by one

_MixFadeIRight:
    move    x:HW_MIXMASTERVR,b
    move    #>MIX_RVOL_MASK,y0
    and     y0,b                        ;Get current right

    cmp     y1,b   y:<Const_000001,y0   ;Compare current right with target right
    jeq     _MixFadeINext               ;Has right channel volume reach final desired setting?
    sub     y0,b                        ;Increase right channel volume setting by one

_MixFadeINext:                          ;Form new HW_MIXMASTERVR reg value
    move    b1,x0
    or      x0,a1                       ;Form new left and right channel value for HW_MIXMASTERVR reg
    move    a1,x:HW_MIXMASTERVR         ;Update STMP3400/3410 HW_MIXMASTERVR reg with new volume setting

    pop     x0                          ;Restore the delay value

    ;Check to see if both channel are set at desired volume level
    move    x:MixerMasterVol,x1
    cmp     x1,a                        ;Are both channel volume level at desired volume setting?
    jeq     _ExitMixFadeI

    push    x0                          ; Save the delay value
    jsr     SysDelay
    jmp     _MoreMixFadeI               ;Continue decreasing left/right attenuation level (increasing volume)

_ExitMixFadeI:
    rts


 if (@DEF('FUNCLET'))
 
    ;If using funclet, switch counter back to original here
    org    p,"MIXSYS_Mixer_P":                             

 endif

;///////////////////////////////////////////////////////////////////////////////
;> Name:        MixerMasterMute
;
;  Type:        Function
;
;  Description: This function mutes the master volume register in the hardware.
;               The previous value is stored off prior to the muting.
;
;  Inputs:      None.
;  Outputs:     Master volume is muted.
;
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////

MixerMasterMute
    move    x:HW_MIXMASTERVR,a
    move    a1,x:MixerMasterVol         ;Save HW_MIXMASTERVR reg value in global variable
    bset    #HW_MIXVOLUMER_MUTE_BITPOS,x:HW_MIXMASTERVR
    rts

    endsec

