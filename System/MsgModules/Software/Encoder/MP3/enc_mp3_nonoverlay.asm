;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2004
; Filename: enc_mp3_nonoverlay.asm
; STMP3 Mp3 Module (CANNOT be overlayed)
;///////////////////////////////////////////////////////////////////////////////
    section ENCAPP_EncMp3NonOverlay


;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

   GLOBAL   EncMp3ModuleTimerDisable
   GLOBAL   EncMp3ModuleIsr
   GLOBAL   EncMp3ModuleIsrInit
   GLOBAL   Mp3EncoderIsrState
   GLOBAL   Mp3EncoderIsrEntryCount
   GLOBAL   Mp3EncCheckDiskSpace
   GLOBAL   Mp3EncCalcMaxRecordTime
   GLOBAL   Mp3EncCalcHrsMinSec 
   GLOBAL   Mp3EncoderIsrSR,Fg_wMp3EncoderIsrSR
;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

   EXTERN   EncoderCSR
   EXTERN   EncoderSR
   EXTERN   MP3EncIsrSp
   EXTERN   MP3EncIsrFlags
   EXTERN   MP3EncoderSinkBufAddrPtr
   EXTERN   MP3EncoderSinkBufSizePtr
   EXTERN   SysDisableTimer
    
   EXTERN   SysClearTimerIrq
   EXTERN   EncoderTimerNum

   EXTERN   MP3EncoderSampleCountHigh    
   EXTERN   MP3EncoderSampleCountLow    
   EXTERN   MP3EncBytesWrittenHigh
   EXTERN   MP3EncBytesWrittenLow
   EXTERN   MP3EncWriteFailCount
   extern   FSamplesPerFrame
   extern   EncoderIsrEntryCount
   extern   MP3EncoderBufAddrPtr
   extern   MP3EncoderBufSizePtr
   extern   MP3EncoderSourceBufDesc
   extern   Mp3Enc_NumChannels
   extern   MP3EncoderSinkBufDesc
   extern   FEncodeAudio
   extern   MP3EncoderSinkBufModPtr
   extern   MP3EncoderSinkBufTailPtr
   extern   AdcDriverDisablePort

            

   EXTERN   Fg_wEncCurrentDeviceNum    
   EXTERN   EncFileHandle
    
   EXTERN   FileWrite
   EXTERN   SysAllLinear
   EXTERN   Fg_wEncNumberOfChannels
   EXTERN   GetEmptySpace
   EXTERN   GetFilledSpace

   EXTERN   FDetectSilenceInit
   EXTERN   SilenceProcess
   EXTERN   FSilenceSR
   EXTERN   FComputeDCOffsetInit     ;stmp6964
   EXTERN   FComputeDCOffset         ;stmp6964
   EXTERN   Fg_wEncSamplingRate
   EXTERN   MP3EncoderBufTailPtr
   EXTERN   MP3EncoderBufModPtr
   EXTERN   g_wEncoderMode

    ; TODO - Get rid of this stuff!!!! Used only for Mp3 dev.
;   EXTERN   CurrEncIsrCount
    
   EXTERN  DiskBytesFree

    if ("ENCPCM"=="TRUE")
       EXTERN   MonoTo16BitMonoShiftTable
       EXTERN   MonoTo16BitMonoMaskTable
    endif   ; if ("ENCPCM"=="TRUE")


    
    EXTERN  PCMEncSaveSR
    EXTERN  PCMEncSavePC

    EXTERN  gEncoderWriteStatus
    EXTERN  EncoderFileWriteIsrSp
    
    EXTERN  EncodingFileWriteFlag
    
    USE_JSR: EQU 0
    USE_OLD: EQU 0
    TRACE_PCM: EQU 0
    
    

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////
    nolist
    include "project.inc"
    include "sysequ.inc"
    include "hwequ.inc"
    include "adcequ.inc"
    include "msgequ.inc"
    include "decoderequ.inc"
    include "encMp3equ.inc"
    include "sysmacro.asm"
    include "systrace.asm"
    include "stack.xref"
    include "stackmem.xref"
    include "const.xref"
    list
    page    132,60,0,0,0

;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////
ENCODER_WRITE_BUSY_BIT      equ 0           ;0 WriteToDisk function not busy
ENCODER_SKIP_WRITE_BIT      equ 1           ;1 Skip Decoder File Read.

;///////////////////////////////////////////////////////////////////////////////
;   X Memory
;///////////////////////////////////////////////////////////////////////////////
       org     x,"ENCAPP_EncMp3NonOverlay_X":

Fg_wMp3EncoderIsrSR
Mp3EncoderIsrSR		dc	0	; For signalling special conditions to
					; the overlay section of the encoder.
					; Bit flags are same as EncoderSR but
					; are private to the Encoder.

;///////////////////////////////////////////////////////////////////////////////
;   Y Memory
;///////////////////////////////////////////////////////////////////////////////
       org     y,"ENCAPP_EncMp3NonOverlay_Y":

Mp3EncoderIsrEntryCount    dc      0

Mp3EncoderIsrState         dc      0

EncoderWriteSaveR6      dc      0

	
;///////////////////////////////////////////////////////////////////////////////
;   Code
;///////////////////////////////////////////////////////////////////////////////
       org     p,"ENCAPP_EncMp3NonOverlay_P":

;*******************************************************************************
; EncMp3ModuleTimerDisable Routine
;*******************************************************************************
EncMp3ModuleTimerDisable
    ; CANNOT be overlayed, called by ISR.
    rts

;*******************************************************************************
; EncoderFileWrite Routine
;*******************************************************************************
EncoderFileWrite
    rts

;*******************************************************************************
; EncMp3ModuleIsrInit
;*******************************************************************************
EncMp3ModuleIsrInit
    clr     a
    move    a1,y:EncoderIsrEntryCount
    move    a1,y:MP3EncoderSampleCountHigh
    move    a1,y:MP3EncoderSampleCountLow   
    move    a1,y:MP3EncWriteFailCount
    move    a1,y:MP3EncBytesWrittenHigh 
    move    a1,y:MP3EncBytesWrittenLow  
    move    a1,y:MP3EncIsrFlags
    move    a1,y:gEncoderWriteStatus

    move    y:Fg_wEncSamplingRate,A
    move    #MP3EncoderSourceBufDesc,r0
    jsr     FComputeDCOffsetInit         ;stmp6964

    move    y:Fg_wEncSamplingRate,A
    move    #MP3EncoderSourceBufDesc,r0
    jsr     FDetectSilenceInit
 
    bset    #ENC_ISR_STATE_STOPPED,a1
    move    a1,y:Mp3EncoderIsrState
    
    ; Initialize the Enocoder Source & Sink Buffers
    move    y:MP3EncoderBufAddrPtr,r0
    move    #-1,m0
    move    y:MP3EncoderBufSizePtr,x0
    do      x0,_done_source
        clr     a
        move    a,y:(r0)+
_done_source


    move    y:MP3EncoderSinkBufAddrPtr,r0
    move    y:MP3EncoderSinkBufSizePtr,x0
    do      x0,_done_sink
        clr     a
        move    a,x:(r0)+
_done_sink



    rts
    
;*******************************************************************************
; EncMp3ModuleIsr
;*******************************************************************************
EncMp3ModuleIsr

        ; Clear the DM bit (double-precision multiply mode, bit 6) of the Mode Register
        ; This will be returned to its previous state when the RTI occurs (since the SR
        ; will be popped back off System Stack Low (SSL).
        ; Accumulator behavior is unpredictable when the DM bit is set.
        ; Next cycle after clearing it should NOT be an ALU instruction.
    andi #$BF,mr

    move    x0,y:IntL1_SaveR6           ; save x0 state

    ; Check if spurious interrupt. If so exit ...
    move    x:HW_ICLSTATUS0R,x0
    jset    #HW_ICLSTATUS0R_SST13_BITPOS,x0,_ContinueIsr
    ; None of the above, exit interruption
    move    y:IntL1_SaveR6,x0           ; restore x0 state
    rti

_ContinueIsr
    move    y:IntL1_SaveR6,x0           ; restore x0 state

    ; save off all registers
    move    r6,y:IntL1_SaveR6            ; r6 used for return mechanism
    move    #_EncoderSaveRegReturn,r6    ; setup the return address
    jmp     SaveRegs                     ; save the remaining registers
_EncoderSaveRegReturn

    ; save off stack
    move    #_EncoderSaveStackReturn,r6  ; setup the return address
    move    #MP3EncIsrSp,r5                 ; pointer to the SP local save
    jmp     SaveSystemStack              ; save the hardware stack
_EncoderSaveStackReturn

    jsr     SysAllLinear

    ; Check for isr overflow.  This error occurs if this routine is interrupted
    ;   by itself.  This should never happen.  The check is mainly for debug and
    ;   could be removed when dev is complete.
    move    y:MP3EncIsrFlags,x0
    jset    #0,x0,big_trouble

    ; Isr is active
    bset    #0,x0
    move    x0,y:MP3EncIsrFlags
    
    ; keep track of entries to this isr
    clr     a
    move    y:Mp3EncoderIsrEntryCount,a0
    inc     a
    move    a0,y:Mp3EncoderIsrEntryCount
    ; TODO - get rid of this!!! dev only!
;    move    a0,y:CurrEncIsrCount

_EncoderStateMachine
;*******************************************************************************
_IsrStateRecording        
    bclr    #ENCODER_SKIP_WRITE_BIT,y:gEncoderWriteStatus        

    move    x:EncoderCSR,x0
    jset    #Stop,x0,_StopRecording

    ; clear the flag
    bclr    #Play,x0
    move    x0,x:EncoderCSR

    ; flag for recording
    move    x:EncoderSR,x0
    bclr    #Stop,x0
    bclr    #Pause,x0
    bset    #Play,x0
    move    x0,x:EncoderSR

    move    #0,x0			; Clear encoder private status flags
    move    x0,x:Mp3EncoderIsrSR

    move    #MP3EncoderSourceBufDesc,r0
    move    y:Fg_wEncNumberOfChannels,a
    jsr     FComputeDCOffset                ;stmp6964 Compute DC bias in ADC and set samples to zero
  
    ; An non-zero return value indicates that further audio processing is not required for the encoder
    ; Parameters passed : SourceBufDesc
    move    #MP3EncoderSourceBufDesc,r0    
    move    y:Fg_wEncNumberOfChannels,a
    jsr     SilenceProcess
    tst     A
    jne     _ExitIsr

    ; If we have enough samples to process then do it
    move    #MP3EncoderSourceBufDesc,r0
    move    #$ffff,m0
    jsr     GetFilledSpace                  ; returns filled space count in A
	move	y:FSamplesPerFrame,B
    move    y:Fg_wEncNumberOfChannels,x0
    rep     x0
    asl     B
    asr     B
    move    B,x0
    cmp     x0,A
    jlt     _ExitIsr                        ; bail if not enough samples

    move    #MP3EncoderSinkBufDesc,r4
    move    #$ffff,m4
    jsr     GetEmptySpace                    ; returns free space count in A
    move    #>500,x0                        ; Worst Case Data per Frame = 480 Words + extras for header, etc.
    cmp     x0,A
    jlt     _ExitIsr                        ; bail if not enough samples. This can lead to dropping of this frame

    move    x:Mp3EncoderIsrSR,x0		        ; Check for (already) Out Of Space
    jset    #EncRanOutOfSpace,x0,_skip_write

    move    y:Fg_wEncCurrentDeviceNum,a2		; Check for about out of space
    jsr     DiskBytesFree
    ;           b1 = number of free clusters
    ;           b0 = number of bytes per cluster    
    jcc     _disk_ok
    error

_disk_ok
    move    b1,a				        ; Number of free clusters
    ;---
    ; Warn when free clusters reaches defined threshold.  DVRWARN
    ; Base case: bytes/cluster is 16 kiloBytes. /4000 bytes/sec Mp3 data =  4.096 sec/cluster
    ; (at 32kbps)
    ; A 3 cluster threshhold warning above the record free cluster limit 
    ; gives ~12 sec warning before recording stops. 
    ;
    ;  Note: Alternate approach checks remaining record seconds but is not worth the system performance  
    ;        hit as this condition is checked in a high frequency periodic timer ISR. Just check free clusters.

    ;move    #>(ENC_FREE_CLUSTERS_WARN_THRESHOLD-1),a  ; DEBUG line only. COMMENT LINE OUT. Forces "low record memory warn" <<<<<

    move    #>ENC_FREE_CLUSTERS_WARN_THRESHOLD,x0 
    cmp     x0,a                                ; A is free clusters.
    jgt     _clr_space_warn                     ; Fall into _warn_low_space if we're below or at to the free clusters warning threshhold.
    ; Next clear x:Mp3EncoderIsrSR EncWarnLowSpace 

    ; x:EncoderSR EncWarningSent bit must not be cleared in the ISR 
    ; It will be cleared when reinitializing the encoder. Avoids race condition.


_warn_low_space
    move    x:Mp3EncoderIsrSR,x0			; warn near out of Space.  
    bset    #EncWarnLowSpace,x0 
    move    x0,x:Mp3EncoderIsrSR

    ; Check for 'low enough space to stop' condition.
    move    #>ENC_MIN_FREE_CLUSTER_LIM,x0       ; Stop when minimum free cluster limit hit. 
    cmp     x0,a                        	; A is free clusters.
    jgt     _write_ok

_out_of_space
    move    x:Mp3EncoderIsrSR,x0			; Out of Space.  Stop recording.
    bset    #EncRanOutOfSpace,x0
    move    x0,x:Mp3EncoderIsrSR			; Flag the overlay section to clean up.
    jmp     _skip_write

_clr_space_warn
    ; Remove space warning (clear bit) since free clusters are known to exist above threshold

    move    x:Mp3EncoderIsrSR,x0                   ; Clears Encoder ISR SR 'low space warn bit' when space available.
    bclr    #EncWarnLowSpace,x0
    move    x0,x:Mp3EncoderIsrSR

_write_ok

    jsr     FEncodeAudio
    bclr    #ENCODER_SKIP_WRITE_BIT,y:gEncoderWriteStatus

    move    #MP3EncoderSinkBufDesc,r0
    move    #$ffff,m0
    jsr     GetFilledSpace                    ; returns free space count in A
    
    tst     A      
    jeq     _skip_write

    jmp     _ExitIsr

_skip_write
    bset    #ENCODER_SKIP_WRITE_BIT,y:gEncoderWriteStatus
    jmp     _ExitIsr
    
_StopRecording
    ; clear the flag
    bclr    #Stop,x0
    move    x0,x:EncoderCSR
    
    bclr    #0,y:EncodingFileWriteFlag
    
    ; stop the ADC
    jsr     AdcDriverDisablePort
    
    ; flag for stopped
    move    x:EncoderSR,x0
    bset    #Stop,x0
    bclr    #Play,x0
    move    x0,x:EncoderSR
    
    ; change isr state
    clr     a
    bset    #ENC_ISR_STATE_STOPPED,a1
    move    a1,y:Mp3EncoderIsrState
    
    ; Turn Timer Module Off
    move    #>EncoderTimerNum,x0
    move    #_TimerDisabled,r0
    jmp     SysDisableTimer
_TimerDisabled

    
    jmp     _ExitOnStop


;*******************************************************************************
_StartRecording



;*******************************************************************************
_ExitIsr
    ; reset the isr for next time
    move    #>EncoderTimerNum,x0
    move    #_TimerReset,r0
    jmp     SysClearTimerIrq
_ExitOnStop
_TimerReset

    ; restore stack
    move    #MP3EncIsrSp,r5                 ; pointer to the SP local save
    move    #_stackRestored,r6           ; setup the return address
    jmp     RestoreSystemStack           ; restore the hardware stack
_stackRestored

    ; restore all registers
    move    #EncoderRestoreRegReturn,r6  ; setup the return address
    jmp     RestoreRegs                  ; restore registers
EncoderRestoreRegReturn
    move    y:IntL1_SaveR6,r6            ; restore minimal reg set

    
    btst    #ENCODER_WRITE_BUSY_BIT,y:gEncoderWriteStatus
    jcs     _End
        
    bclr    #ENCODER_SKIP_WRITE_BIT,y:gEncoderWriteStatus
    jcs     _End
        
    ; tt 1/6/04 - Set busy flag.
    bset    #ENCODER_WRITE_BUSY_BIT,y:gEncoderWriteStatus
    
    ; Store off the present Stack Register and PC - take a detour through
    ; WriteToDisk, then reinstate these and continue where we left off. 
    ori     #$3,mr              ; Block interrupts
    nop                                
    nop                                
    nop                                
    movec   ssl,y:PCMEncSaveSR     ;Saves return address and status register
    movec   ssh,y:PCMEncSavePC     ;Call back address to use from WriteToDisk()
    nop
    movec   #WriteToDisk,ssh   ;Forces WriteToDisk call after rti                          
    bclr    #9,SR               ; Re-enable interrupts

_End   
    ; Isr is inactive
    bclr    #0,y:MP3EncIsrFlags
    nop

    rti                     ;Int level is dropped to 0 (user level)
                            ;WriteToDisk() operates at user level

big_trouble
    error
    
    
;///////////////////////////////////////////////////////////////////////////////
;   WriteToDisk
;///////////////////////////////////////////////////////////////////////////////
WriteToDisk
        ;Here the Interruption level is dropped automatically to 0 (user level)
        ; Clear the DM bit (double-precision multiply mode, bit 6) of the Mode Register
        ; This will be returned to its previous state when the RTI occurs (since the SR
        ; will be popped back off System Stack Low (SSL).
        ; Accumulator behavior is unpredictable when the DM bit is set.
        ; Next cycle after clearing it should NOT be an ALU instruction.        
    andi #$BF,mr
    

    nop
    nop
    nop

    ; save off all registers
    move    r6,y:EncoderWriteSaveR6         ; r6 used for return mechanism
    move    #_EncoderWriteSaveRegReturn,r6  ; setup the return address
    jmp     SaveRegs                        ; save the remaining registers
_EncoderWriteSaveRegReturn
        
    ; save off stack
    move    #_EncoderWriteSaveStackReturn,r6 ; setup the return address
    move    #EncoderFileWriteIsrSp,r5       ; pointer to the SP local save
    jmp     SaveSystemStack                 ; save the hardware stack
_EncoderWriteSaveStackReturn

    move   y:Fg_bPartialRefreshInProgress,x0
    jclr  #0,x0,_Continue_EraseOneSystemDriveBlock
    jmp   _return
_Continue_EraseOneSystemDriveBlock

    move    #MP3EncoderSinkBufDesc,r0
    move    #$ffff,m0
    jsr     GetFilledSpace                    ; returns free space count in A
    
    tst     A      
    jeq     _return

    bset    #0,y:EncodingFileWriteFlag    

    
    ; Stagger File Writes so that space is freed up, for encoder to write data.
    ; This will reduce drop outs.
    move    y:<Const_000200,x0      ; Write at the most equivalent of 512 words
    cmp     x0,A
    tgt     x0,A
    
;------------------------------------------------------------------------------------
;   swap ( B2:B1:B0 -> B0:B1:B2 ) before writting it to a file.
    move    y:MP3EncoderSinkBufModPtr,m0 
    move    y:MP3EncoderSinkBufTailPtr,r0	
    move    y:<Const_000080,x0               ; multiply factor needed
    push    A
    do      A,_swap_msb_lsb
        move    x:(r0),y0                    ; y0 = B2:B1:B0 (bytes from flash)
        move    #>$ff,x1                     ; byte mask
        mpy     y0,x0,a                      ; a = shifted bytes
        and     x1,a                         ; A1 = MSB in LSB position ($00:00:B2)
        move    a1,y1                        ; Y1 = $00:00:B2
        move    a0,y0                        ; Y0 = $B1:B0:00
        mpy     y0,x0,a                      ; A1 = $00:00:B1 A0 = $B0:00:00
        move    a0,b                         ; b1 = $B0:00:00
        or      y1,b                         ; b1 = $B0:00:B2
        and     x1,a                         ; A1 = $00:00:B1
        move    a1,y1                        ; Y1 = $00:00:B1
        mpy     x0,y1,a                      ; A0 = $00:B1:00
        move    a0,y1                        ; Y1 = $00:B1:00
        or      y1,b                         ; b1 = $B0:B1:B2 :: MSB/LSB are swapped
        move    b1,y0
        move    y0,x:(r0)+                   ; save new value
_swap_msb_lsb
    pop     x0                               ; Get back the number of words written
    push    x0
    move    #>3,y0
    mpy     x0,y0,A
    asr     A                                ; Convert to bytes.
    move    A0,A    
    jsr     SysAllLinear                ; for now we're gonna go linear
    move    y:EncFileHandle,a2
    bset    #23,a0                      ; x memory
    move    y:MP3EncoderSinkBufTailPtr,r0
    move    y:MP3EncoderSinkBufModPtr,m0
    jsr     FileWrite

    pop     A
    jcs     _failed_the_write


    move    A,n0
    move    y:MP3EncoderSinkBufTailPtr,r0
    move    y:MP3EncoderSinkBufModPtr,m0
    nop
    move    (r0)+n0
    move    r0,y:MP3EncoderSinkBufTailPtr
    move    y:<Const_ffffff,m0


    move    A,x0
    move    #>3,y0
    mpy     x0,y0,A
    asr     A
    move    A0,A    

    ; keep track of #bytes written
    clr     b               y:MP3EncBytesWrittenHigh,x1
    move    a1,b0
    move    y:MP3EncBytesWrittenLow,x0
    add     x,b
    move    b1,y:MP3EncBytesWrittenHigh
    move    b0,y:MP3EncBytesWrittenLow    




    bclr    #0,y:EncodingFileWriteFlag
     
_return  
_RestoreSystem
    ;Raise Int level to 1 so we can update the BUSY flag without being
    ;interrupted by the Encoder Isr

    bset    #8,sr                       ; Raise interrupt level to 1
    nop                                 ; Those nops make sure that 
    nop                                 ; any interrupt pending is processed
    nop                                 ; DO NOT REMOVED, UNLESS YOU HAVE 
                                        ; A FREE DAY TO DEBUG CODE                                                    
                              


    nop
    nop
    nop
    
                                 
    ; tt 1/6/04 - Clear busy flag.
    bclr    #ENCODER_WRITE_BUSY_BIT,y:gEncoderWriteStatus
        
    ; restore stack
    move    #EncoderFileWriteIsrSp,r5         ; pointer to the SP local save
    move    #_EncoderWriteStackRestored,r6   ; setup the return address
    jmp     RestoreSystemStack              ; restore the hardware stack
_EncoderWriteStackRestored

    move    #_EncoderWriteRestoreRegReturn,r6 ; setup the return address
    jmp     RestoreRegs                     ; restore registers
_EncoderWriteRestoreRegReturn
    move    y:EncoderWriteSaveR6,r6          ; restore minimal reg set
    movec   y:PCMEncSavePC,ssh              ;Force return 
    movec   y:PCMEncSaveSR,ssl
    nop
    rti                            ;This needs to be a RTI to restore the SR

_failed_the_write
    clr     a
    move    y:MP3EncWriteFailCount,a0    
    inc     a
    move    a0,y:MP3EncWriteFailCount
    bclr    #0,y:EncodingFileWriteFlag
    error
    jmp     _return  


;///////////////////////////////////////////////////////////////////////////////
;   Mp3EncCheckDiskSpace          
;       Exit
;           a1 = num bytes high
;           a0 = num bytes low
;           b1 = number of free clusters
;           b0 = number of bytes per cluster
;           Carry clear => enough space to record
;           Carry set => not enough space to record
;///////////////////////////////////////////////////////////////////////////////
Mp3EncCheckDiskSpace
    ; TODO - create a "current disk" variable instead of using the default disk 0
    move    y:Fg_wEncCurrentDeviceNum,a2
    jsr     DiskBytesFree               ; get # bytes available on disk
    jcs     _uh_oh
;           a1 = num bytes high
;           a0 = num bytes low
;           b1 = number of free clusters
;           b0 = number of bytes per cluster
    move    b0,y0                       ; temp save bytes per cluster
    clr     b           b1,y1           ; temp save free clusters
    move    #>ENC_MIN_CLUSTERS,b
    cmp     y1,b
    move    y0,b0                       ; restore bytes per cluster
    move    y1,b1                       ; restore free clusters
    jge     _not_enough_disk_space
    ; we have enough clusters so exit
    bclr    #HW_SR_C_BITPOS,sr
    rts

_not_enough_disk_space
    bset    #HW_SR_C_BITPOS,sr
    rts

_uh_oh
    error

;///////////////////////////////////////////////////////////////////////////////
;   Mp3EncCalcMaxRecordTime          
;       Entry
;           b1 = number of free clusters
;           b0 = number of bytes per cluster
;       Exit
;     y0 = hours
;     x1 = minutes
;     x0 = seconds
;
; seconds per cluster = t
; t = [(bytespercx/nblockalign)*wsamplesperblock]/nsamplespersec
; A = wsamplesperblock/nblockalign/nsamplespersec
; t = bytespercx * A
; so max seconds tm = freecx * t
;///////////////////////////////////////////////////////////////////////////////
Mp3EncCalcMaxRecordTime
    move    y:InvSampleRate,x0
    move    b0,x1                           ; get bytespercluster ready
    mpy     x0,x1,a               b1,x0     ; a = (2^32)*(bytespercx * A)
    do      #5,_end_scale_adjust
        asl     a
        asl     a
_end_scale_adjust
    move    a1,x1                           ; x1 = bytespercx * A = t
    mpy     x0,x1,a                         ; a = (some hacked scale factor)*(freecx * t)
    do      #2,_end_scale_adjust2
        asl     a
        asl     a
_end_scale_adjust2
    rts
    
;///////////////////////////////////////////////////////////////////////////////
; Mp3EncCalcHrsMinSec
;   Entry:
;     a0 = seconds
;   Exit
;     y0 = hours
;     x1 = minutes
;     x0 = seconds
;///////////////////////////////////////////////////////////////////////////////
Mp3EncCalcHrsMinSec
    ; now calculate total minutes
    clr     b               a0,y1   ; y1 = total seconds
    move    #>$022222,x0            ; 1/60 * 2^23
    mpy     x0,y1,a
    move    a1,y0                   ; y0 = total minutes
    ; and total hours
    mpy     x0,y0,a                 ; a1 = hours
    move    a1,x1                   ; x1 = hours
    move    #>60,x0
    mpy     x0,x1,b
    asr     b
    clr     a               b0,x0
    move    y0,a1
    sub     x0,a                    ; a1 = minutes mod hours
    push    a1                      ; save it
    clr     b               #>60,x0
    mpy     y0,x0,a         y1,b
    asr     a
    move    a0,x0
    sub     x0,b                    ; b1 = seconds mod minutes
    move    b1,x0                   ; x0 = seconds
    move    x1,y0                   ; hours
    pop     x1                      ; minutes
    move    #>60,a
    cmp     x0,a
    jne     _no_second_wrap
    move    y:<Const_000000,x0      ; wrap
    clr     b                       ; increment by 1 minute
    move    x1,b0
    inc     b
    move    b0,x1
_no_second_wrap
    cmp     x1,a
    jne     _no_minute_wrap
    move    y:<Const_000000,x1      
    clr     b                       ; increment by 1 hour
    move    y0,b0
    inc     b
    move    b0,y0
_no_minute_wrap
    rts

    endsec
