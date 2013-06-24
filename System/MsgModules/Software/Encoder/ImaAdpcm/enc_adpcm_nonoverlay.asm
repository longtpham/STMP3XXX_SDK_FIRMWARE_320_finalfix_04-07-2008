;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2003
; Filename: enc_adpcm_nonoverlay.asm
; STMP3 ISR-level Encoder ADPCM IMA Module (CANNOT be overlayed)
;///////////////////////////////////////////////////////////////////////////////
    section ENCAPP_EncAdpcmImaNonOverlay


;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

   GLOBAL   EncAdpcmImaModuleTimerDisable
   GLOBAL   EncAdpcmImaModuleIsr
   GLOBAL   EncAdpcmImaModuleIsrInit
   GLOBAL   EncoderIsrState
   GLOBAL   EncoderIsrEntryCount
   GLOBAL   EncCheckDiskSpace
   GLOBAL   CalcMaxRecordTime
   GLOBAL   CalcHrsMinSec 
  GLOBAL   EncoderIsrSR,Fg_wEncoderIsrSR

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

   EXTERN   EncoderCSR
   EXTERN   EncoderSR
   EXTERN   EncIsrSp
   EXTERN   EncIsrFlags
    
   EXTERN   SysClearTimerIrq
   EXTERN   EncoderTimerNum
   EXTERN   AdcDriverDisablePort

   EXTERN   EncoderSourceBufDesc 
   EXTERN   EncoderBufAddrPtr    
   EXTERN   EncoderBufModPtr     
   EXTERN   EncoderBufSizePtr    
   EXTERN   EncoderBufHeadPtr    
   EXTERN   EncoderBufTailPtr    

   EXTERN   EncoderSampleCountHigh    
   EXTERN   EncoderSampleCountLow    
   EXTERN   EncBytesWrittenHigh
   EXTERN   EncBytesWrittenLow
   EXTERN   EncWriteFailCount
    extern FTotalSamplesPerBlock
    extern Fg_Wave_wBlockAlign
    extern EncoderSinkBuf
    extern AdpcmEncoderPointer
    extern Fg_Wave_wChannels
    extern FIncrement_Counters
 if @DEF('TUNER_STFM1000')
   EXTERN  Fg_ADCsource
   EXTERN  Fg_EncodeStfm
 endif           

   EXTERN   Fg_wEncCurrentDeviceNum    
   EXTERN   AdpcmEncFileHandle
    
   EXTERN   FileWrite
   EXTERN   SysAllLinear

   EXTERN   GetEmptySpace
   EXTERN   GetFilledSpace

    ; TODO - Get rid of this stuff!!!! Used only for adpcm dev.
;   EXTERN   CurrEncIsrCount
    
   EXTERN  DiskBytesFree

    if ("ENCPCM"=="TRUE")
       EXTERN   MonoTo16BitMonoShiftTable
       EXTERN   MonoTo16BitMonoMaskTable
    endif   ; if ("ENCPCM"=="TRUE")

    if ("ENCT1"=="TRUE")
       EXTERN   StopIt
       EXTERN   StopCount
       EXTERN   SysTime
       EXTERN   WriteStartTime     
       EXTERN   ShortestWriteTime  
       EXTERN   LongestWriteTime   
       EXTERN   WriteTimeLogBuffer
       EXTERN   WriteTimeLogBufferPtr    
       EXTERN   AdpcmTimeLogBuffer
       EXTERN   AdpcmTimeLogBufferPtr    
       EXTERN   IsrTimeLogBuffer
       EXTERN   IsrTimeLogBufferPtr    
       EXTERN   IsrStartTime      
       EXTERN   ShortestIsrTime   
       EXTERN   LongestIsrTime    
       EXTERN   AdpcmStartTime    
       EXTERN   ShortestAdpcmTime 
       EXTERN   LongestAdpcmTime  
    endif   ; if ("ENCT1"=="TRUE")

	EXTERN	State
	EXTERN	ColRowBitsDebounced
    
    ;tt 1/6/04 additions    
    EXTERN  pEncoderSinkBufPtr0
;    EXTERN  EncoderWriteStatus0
;    EXTERN  WriteIndex
;    EXTERN  ProcessIndex
    EXTERN  WriteSinkPointer
    EXTERN  ProcessSinkPointer
    EXTERN  EncoderWritesWaiting
    EXTERN  gEncoderWriteStatus
    
    EXTERN  PCMEncSaveSR
    EXTERN  PCMEncSavePC
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
    include "encadpcmequ.inc"
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
       org     x,"ENCAPP_EncAdpcmImaNonOverlay_X":

Fg_wEncoderIsrSR
EncoderIsrSR		dc	0	; For signalling special conditions to
					; the overlay section of the encoder.
					; Bit flags are same as EncoderSR but
					; are private to the Encoder.

;///////////////////////////////////////////////////////////////////////////////
;   Y Memory
;///////////////////////////////////////////////////////////////////////////////
       org     y,"ENCAPP_EncAdpcmImaNonOverlay_Y":

EncoderIsrEntryCount    dc      0

EncoderIsrState         dc      0

EncoderWriteSaveR6      dc      0

	
;///////////////////////////////////////////////////////////////////////////////
;   Code
;///////////////////////////////////////////////////////////////////////////////
       org     p,"ENCAPP_EncAdpcmImaNonOverlay_P":

;*******************************************************************************
; EncAdpcmImaModuleTimerDisable Routine
;*******************************************************************************
EncAdpcmImaModuleTimerDisable
    ; CANNOT be overlayed, called by ISR.
    rts

;*******************************************************************************
; EncoderFileWrite Routine
;*******************************************************************************
EncoderFileWrite
    rts

;*******************************************************************************
; EncAdpcmImaModuleIsrInit
;*******************************************************************************
EncAdpcmImaModuleIsrInit

    if ("ENCT1"=="TRUE")
        clr     a
        move    #>$0FFFFF,x0
        move    a1,y:StopIt
        move    a1,y:StopCount
        move    a1,y:WriteStartTime     
        move    a1,y:LongestWriteTime   
        move    x0,y:ShortestWriteTime  
        move    a1,y:AdpcmStartTime     
        move    a1,y:LongestAdpcmTime   
        move    x0,y:ShortestAdpcmTime  
        move    a1,y:IsrStartTime     
        move    a1,y:LongestIsrTime   
        move    x0,y:ShortestIsrTime  
    endif   ; if ("ENCT1"=="TRUE")

    if (TRACE_PCM)
      bset #HW_GP_B18_BITPOS,x:HW_GP0PWR    ; pwrpads
      bset #HW_GP_B18_BITPOS,x:HW_GP0ENR    ; enable
      bset #HW_GP_B18_BITPOS,x:HW_GP0DOER   ; output
      bclr #HW_GP_B18_BITPOS,x:HW_GP0DOR    ; make it low 
      
      bset #HW_GP_B19_BITPOS,x:HW_GP0PWR    ; pwrpads
      bset #HW_GP_B19_BITPOS,x:HW_GP0ENR    ; enable
      bset #HW_GP_B19_BITPOS,x:HW_GP0DOER   ; output
      bclr #HW_GP_B19_BITPOS,x:HW_GP0DOR    ; make it low 
    endif
    clr     a
    move    a1,y:EncoderIsrEntryCount
    move    a1,y:EncoderSampleCountHigh
    move    a1,y:EncoderSampleCountLow   
    move    a1,y:EncWriteFailCount
    move    a1,y:EncBytesWrittenHigh 
    move    a1,y:EncBytesWrittenLow  
    move    a1,y:EncIsrFlags
    move    a1,y:EncoderWritesWaiting
    move    a1,y:gEncoderWriteStatus


    bset    #ENC_ISR_STATE_STOPPED,a1
    move    a1,y:EncoderIsrState
; this will cause FM MSADPCM 32ksample to reboot, so comment out
;   bset    #0,y:EncodingFileWriteFlag
    
    ; Initialize the Enocoder Source & Sink Buffers
    move    y:EncoderBufAddrPtr,r0
    move    #-1,m0
    move    y:EncoderBufSizePtr,x0
    do      x0,_done_source
        clr     a
        move    a,y:(r0)+
_done_source
    rts
    
;*******************************************************************************
; EncAdpcmImaModuleIsr
;*******************************************************************************
EncAdpcmImaModuleIsr

        ; Clear the DM bit (double-precision multiply mode, bit 6) of the Mode Register
        ; This will be returned to its previous state when the RTI occurs (since the SR
        ; will be popped back off System Stack Low (SSL).
        ; Accumulator behavior is unpredictable when the DM bit is set.
        ; Next cycle after clearing it should NOT be an ALU instruction.
        andi #$BF,mr

    move    x0,y:IntL1_SaveR6           ; save x0 state

    ; Check if spirous interrupt. If so exit ...
    move    x:HW_ICLSTATUS0R,x0
    jset    #HW_ICLSTATUS0R_SST13_BITPOS,x0,_ContinueIsr
    ; None of the above, exit interruption
    move    y:IntL1_SaveR6,x0           ; restore x0 state
    rti

_ContinueIsr
    move    y:IntL1_SaveR6,x0           ; restore x0 state
    
  if (TRACE_PCM)
    bset    #19,x:HW_GP0DOR             ; Logic Analyzer - Pin 19 = EncAdpcmIsr  
  endif  

    ; save off all registers
    move    r6,y:IntL1_SaveR6            ; r6 used for return mechanism
    move    #_EncoderSaveRegReturn,r6    ; setup the return address
    jmp     SaveRegs                     ; save the remaining registers
_EncoderSaveRegReturn

    ; save off stack
    move    #_EncoderSaveStackReturn,r6  ; setup the return address
    move    #EncIsrSp,r5                 ; pointer to the SP local save
    jmp     SaveSystemStack              ; save the hardware stack
_EncoderSaveStackReturn

    jsr     SysAllLinear

    ; Check for isr overflow.  This error occurs if this routine is interrupted
    ;   by itself.  This should never happen.  The check is mainly for debug and
    ;   could be removed when dev is complete.
    move    y:EncIsrFlags,x0
    jset    #0,x0,big_trouble

    if ("ENCT1"=="TRUE")
        move    #IsrStartTime,r0
        jsr     WatchTimeStart
    endif   ; if ("ENCT1"=="TRUE")

    ; Isr is active
    bset    #0,x0
    move    x0,y:EncIsrFlags
    
    ; keep track of entries to this isr
    clr     a
    move    y:EncoderIsrEntryCount,a0
    inc     a
    move    a0,y:EncoderIsrEntryCount
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
    move    x0,x:EncoderIsrSR
    
    ; TODO - add recorder change state message here?

    ; if we have enough samples to process then do it
    move    #EncoderSourceBufDesc,r0
    move    #$ffff,m0
    jsr     GetFilledSpace                  ; returns filled space count in A
	move	y:FTotalSamplesPerBlock,x0
    cmp     x0,A
    jlt     _ExitIsr                        ; bail if not enough samples



    move    x:EncoderIsrSR,x0		        ; Check for (already) Out Of Space
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
    ; Base case: bytes/cluster is 16 kiloBytes. /4000 bytes/sec adpcm data =  4.096 sec/cluster
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
    ; Next clear x:EncoderIsrSR EncWarnLowSpace 

    ; x:EncoderSR EncWarningSent bit must not be cleared in the ISR 
    ; It will be cleared when reinitializing the encoder. Avoids race condition.


_warn_low_space
    move    x:EncoderIsrSR,x0			; warn near out of Space.  
    bset    #EncWarnLowSpace,x0 
    move    x0,x:EncoderIsrSR

    ; Check for 'low enough space to stop' condition.
    move    #>ENC_MIN_FREE_CLUSTER_LIM,x0       ; Stop when minimum free cluster limit hit. 
    cmp     x0,a                        	; A is free clusters.
    jgt     _write_ok

_out_of_space
    move    x:EncoderIsrSR,x0			; Out of Space.  Stop recording.
    bset    #EncRanOutOfSpace,x0
    move    x0,x:EncoderIsrSR			; Flag the overlay section to clean up.
    jmp     _skip_write

_clr_space_warn
    ; Remove space warning (clear bit) since free clusters are known to exist above threshold

    move    x:EncoderIsrSR,x0                   ; Clears Encoder ISR SR 'low space warn bit' when space available.
    bclr    #EncWarnLowSpace,x0
    move    x0,x:EncoderIsrSR

_write_ok    
    jsr     AdpcmProcess  
    bclr    #ENCODER_SKIP_WRITE_BIT,y:gEncoderWriteStatus
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
 if @def('TUNER_STFM1000')
    clr	a
    move	y:Fg_ADCsource,a0		; if encoding from Steely Dan, ADC is not running
    dec		a
    jne     _DisableADCdriver
	move    y:Fg_EncodeStfm,a0
	bclr	#0,a0
	move    a0,y:Fg_EncodeStfm
    jmp     _byPassDisablePort
 endif
_DisableADCdriver
    jsr     AdcDriverDisablePort
_byPassDisablePort    
    ; flag for stopped
    move    x:EncoderSR,x0
    bset    #Stop,x0
    bclr    #Play,x0
    move    x0,x:EncoderSR
    
    ; change isr state
    clr     a
    bset    #ENC_ISR_STATE_STOPPED,a1
    move    a1,y:EncoderIsrState

    ; Turn Timer Module Off
    move    #>EncoderTimerNum,x0
    move    #_TimerDisabled,r0
    jmp     SysDisableTimer
_TimerDisabled

    
    jmp     _ExitOnStop

    
;*******************************************************************************
;_StartRecording



;*******************************************************************************
_ExitIsr

    if ("ENCT1"=="TRUE")
        move    #IsrStartTime,r0
        move    #ShortestIsrTime,r1
        move    #LongestIsrTime,r2
        jsr     WatchTimeStop
        move    #IsrTimeLogBufferPtr,r0
        jsr     WatchTimeBuffer
    endif   ; if ("ENCT1"=="TRUE")
    

    ; reset the isr for next time
    move    #>EncoderTimerNum,x0
    move    #_TimerReset,r0
    jmp     SysClearTimerIrq
_ExitOnStop
_TimerReset



;    jsr     WriteToDisk

    ;  if (EncoderWritesWaiting == 0) don't call WriteToDisk
    move    y:EncoderWritesWaiting,a
    move    #0,a2
    tst     a
    jne     _EndEncoderWriteTest
    bset    #ENCODER_SKIP_WRITE_BIT,y:gEncoderWriteStatus    
_EndEncoderWriteTest   
    
    ; restore stack
    move    #EncIsrSp,r5                 ; pointer to the SP local save
    move    #_stackRestored,r6           ; setup the return address
    jmp     RestoreSystemStack           ; restore the hardware stack
_stackRestored

    ; restore all registers
    move    #EncoderRestoreRegReturn,r6  ; setup the return address
    jmp     RestoreRegs                  ; restore registers
EncoderRestoreRegReturn
    move    y:IntL1_SaveR6,r6            ; restore minimal reg set

  if (TRACE_PCM)
    bclr    #19,x:HW_GP0DOR            ; Logic Analyzer - Pin 19 = EncAdpcmIsr
  endif
    ;tt 1/6/04 - Multithread voice record.
    ; Test to see if a file write is in progress
    ; Determines if need to start Encoder File Write Function
   
    bclr    #ENCODER_SKIP_WRITE_BIT,y:gEncoderWriteStatus
    jcs     _End
    
    btst    #ENCODER_WRITE_BUSY_BIT,y:gEncoderWriteStatus
    jcs     _End
        
    ; tt 1/6/04 - Set busy flag.
    bset    #ENCODER_WRITE_BUSY_BIT,y:gEncoderWriteStatus
    
    ; Store off the present Stack Register and PC - take a detour through
    ; WriteToDisk, then reinstate these and continue where we left off. 
    ori     #$3,mr              ; Block interrupts
;    bset    #9,SR                   ; Block interrupts
    movec   ssl,y:PCMEncSaveSR     ;Saves return address and status register
    movec   ssh,y:PCMEncSavePC     ;Call back address to use from WriteToDisk()
    nop
    movec   #WriteToDisk,ssh   ;Forces WriteToDisk call after rti                          
    bclr    #9,SR               ; Re-enable interrupts

_End   
    ; Isr is inactive
    bclr    #0,y:EncIsrFlags
    nop
    
    rti                     ;Int level is dropped to 0 (user level)
                            ;WriteToDisk() operates at user level

big_trouble
    error
    
;///////////////////////////////////////////////////////////////////////////////
;   AdpcmProcess
;   On Entry:
;///////////////////////////////////////////////////////////////////////////////
AdpcmProcess        

    ; tt - 1/6/04 changes to make this a multithreaded voice record implementation  
    clr     b
    move    y:EncoderWritesWaiting,b
    move    #>NUM_ADPCM_BUFS,x0
    move    #0,b2
    cmp     x0,b
    jge     _EndOfAdpcmProcess

;    debug     
    move    y:ProcessSinkPointer,r0     ; Load Sink Pointer for processing.
    nop
    move    y:(r0),r0
    ; tt end of additions.
    
    ; Old method
  if (USE_OLD)
    move    #EncoderSinkBuf,r0
  endif
    move    y:EncoderBufModPtr,m4
    move    y:EncoderBufTailPtr,r4
    move    y:AdpcmEncoderPointer,r6
    move    y:Fg_Wave_wChannels,A
    ; Call the appropriate Encoding algorithm
    jsr     (r6)
    ; update pointers
    move    y:EncoderBufTailPtr,r4
	move	y:FTotalSamplesPerBlock,n4
    nop
    move    (r4)+n4
    move    y:<Const_ffffff,m4
    move    r4,y:EncoderBufTailPtr
    ; Now write to the Disk
    ; Pass the size in Bytes in A

    jsr     SysAllLinear
    jsr     FIncrement_Counters
    
    ; tt 1/6/04 add code for multithread.    
    move    y:ProcessSinkPointer,r0
    ; if (++ProcessSinkPointer >= &pEncoderSinkBufPtr[NUM_ADPCM_BUFS])
    clr     b
    clr     a
    move    (r0)+
    move    r0,b
    move    #pEncoderSinkBufPtr0+NUM_ADPCM_BUFS,a
    cmp     a,b
    jlt     _ContinueWithADPCM_Process
    ;{
    ;   ProcessSinkPointer = &pEncoderSinkBufPtr[0];
        move    #pEncoderSinkBufPtr0,r0
    ;}
_ContinueWithADPCM_Process       
    move    r0,y:ProcessSinkPointer
    ;EncoderWritesWaiting++;
    move    y:EncoderWritesWaiting,a
    move    #>1,x0
    add     x0,a
    move    a1,y:EncoderWritesWaiting
    ; tt - end of changes
    
_EndOfAdpcmProcess    
    nop
    rts

    
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
;    jset    #8,SR,_Problem
;    jclr    #9,SR,_StartOK  
;_Problem
;    debug  

;_StartOK       

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
    
    if ("ENCT1"=="TRUE")    
        move    #WriteStartTime,r0
        jsr     WatchTimeStart
    endif   ; if ("ENCT1"=="TRUE")
    
  if (TRACE_PCM)
    bset    #18,x:HW_GP0DOR            ; Logic Analyzer - Pin 18 = FileWrite
  endif
  
    bset    #0,y:EncodingFileWriteFlag    

; tt - 1/6/04 adding multithread voice record writes.    
    move    y:WriteSinkPointer,r0    ; Load next Sink Buffer to write.
    nop
    move    y:(r0),r0
;tt end of multithread write changes section.
    move    y:Fg_Wave_wBlockAlign,A
    jsr     SysAllLinear                ; for now we're gonna go linear
    move    y:AdpcmEncFileHandle,a2
    bset    #23,a0                      ; x memory
 if (USE_OLD)
    move    #EncoderSinkBuf,r0
 endif
;        debug
    jsr     FileWrite
    jcs     _failed_the_write
    ; keep track of #bytes written
    clr     b               y:EncBytesWrittenHigh,x1
    move    a1,b0
    move    y:EncBytesWrittenLow,x0
    add     x,b
    move    b1,y:EncBytesWrittenHigh
    move    b0,y:EncBytesWrittenLow    
  
    if ("ENCT1"=="TRUE")
        move    #WriteStartTime,r0
        move    #ShortestWriteTime,r1
        move    #LongestWriteTime,r2
        jsr     WatchTimeStop
        move    #WriteTimeLogBufferPtr,r0
        jsr     WatchTimeBuffer
    endif   ; if ("ENCT1"=="TRUE")

; tt 1/6/04 - changes to multithreaded buffer implementation. 
    
    move    y:WriteSinkPointer,r0
    ; if (++WriteSinkPointer >= &pEncoderSinkBufPtr[NUM_ADPCM_BUFS])
    clr     b
    clr     a
    move    (r0)+
    move    r0,b
    move    #pEncoderSinkBufPtr0+NUM_ADPCM_BUFS,a
    cmp     a,b
    jlt     _ContinueWithWriteToDisk_Process
    ;{
    ;   ProcessSinkPointer = &pEncoderSinkBufPtr[0];
        move    #pEncoderSinkBufPtr0,r0
    ;}
_ContinueWithWriteToDisk_Process    
    move    r0,y:WriteSinkPointer
    bclr    #0,y:EncodingFileWriteFlag
     
_return  
_RestoreSystem
    ;Raise Int level to 1 so we can update the BUSY flag without being
    ;interrupted by the Decoder Isr

    bset    #8,sr                       ; Raise interrupt level to 1
    nop                                 ; Those nops make sure that 
    nop                                 ; any interrupt pending is processed
    nop                                 ; DO NOT REMOVED, UNLESS YOU HAVE 
                                        ; A FREE DAY TO DEBUG CODE                                                    
                              
    ; Need to decrement counter.                                 
    ;EncoderWritesWaiting--;
    move    y:EncoderWritesWaiting,b
    move    #>1,y1
    sub     y1,b
    move    b1,y:EncoderWritesWaiting
    
  if (TRACE_PCM)
    bclr    #18,x:HW_GP0DOR            ; Logic Analyzer - Pin 18 = FileWrite
  endif
                                                    
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

; Old Code  
;    nop
;    rts    

; todo - we don't check EncWriteFailCount right now.
_failed_the_write
    clr     a
    move    y:EncWriteFailCount,a0    
    inc     a
    move    a0,y:EncWriteFailCount
    bset    #0,y:EncodingFileWriteFlag
    error
    jmp     _return  

;///////////////////////////////////////////////////////////////////////////////
;   EncCheckDiskSpace          
;       Exit
;           a1 = num bytes high
;           a0 = num bytes low
;           b1 = number of free clusters
;           b0 = number of bytes per cluster
;           Carry clear => enough space to record
;           Carry set => not enough space to record
;///////////////////////////////////////////////////////////////////////////////
EncCheckDiskSpace
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
;   CalcMaxRecordTime          
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
CalcMaxRecordTime
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
; CalcHrsMinSec
;   Entry:
;     a0 = seconds
;   Exit
;     y0 = hours
;     x1 = minutes
;     x0 = seconds
;///////////////////////////////////////////////////////////////////////////////
CalcHrsMinSec
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
