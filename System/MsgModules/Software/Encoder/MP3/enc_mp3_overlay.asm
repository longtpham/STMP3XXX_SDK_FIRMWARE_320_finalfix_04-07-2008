;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2004
; Filename: enc_mp3_overlay.asm
; STMP3xxx MPEG Audio layer 3 Encoder Module (non real-time overlay portion)
;///////////////////////////////////////////////////////////////////////////////

    include "resource.inc"
    include "project.inc"
    opt     mex

    section ENCMOD_MP3



;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////

; states for EncModuleState
STATE_STOPPED           equ              0
STATE_RECORDING         equ              1

;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

   GLOBAL   EncMp3ProcessQueue
   GLOBAL   EncMp3Init

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

   EXTERN   SysPostMessage
   EXTERN   SysLoadResource
   EXTERN   SysEnableTimer
   EXTERN   SysDisableTimer
   EXTERN   SysGetCurrentTime
   EXTERN   SysAllLinear
   EXTERN   SysSpeedIncrease
   EXTERN   SysSpeedClockFree
   EXTERN   SysFlushCoderMemory
   EXTERN   RestoreSystemStack
   EXTERN   SaveSystemStack
   EXTERN   EncoderTimerNum
   EXTERN   SysSetupEncVectors
   EXTERN   UserScratchX
   EXTERN   UserScratchY

   EXTERN   EncoderProcessPtr
   EXTERN   EncoderCSR
   EXTERN   EncoderSR
   EXTERN   Mp3EncoderIsrSR
   EXTERN   Mp3EncoderIsrState


   EXTERN   AdcDriverIsr
   EXTERN   EncMp3ModuleIsr
   EXTERN   EncMp3ModuleIsrInit
   EXTERN   FSysBatteryGetLevel

   EXTERN   RecStartTimeHigh
   EXTERN   RecStartTimeLow

   EXTERN   MP3EncSignalReturn

   EXTERN   EncFileNameString
   EXTERN   MP3EncFileHandle
   EXTERN   y:Fg_wEncCurrentDeviceNum

   EXTERN   EncAdpcmOvlSave_sp

   ;EXTERN   EncInvSampleRateScale
   EXTERN   EncModuleState

   EXTERN   SysLoadFATWrite
   EXTERN   SysUnLoadFATWrite

   EXTERN  DiskBytesFree
   EXTERN   FILEOpen
   EXTERN   FILEClose
   EXTERN   FILEAppend
   EXTERN   FILECreate
   EXTERN   FILEDelete
   EXTERN   FileWrite
   EXTERN  F_lc_u_e_MP3ENC_P_buf
   EXTERN  F_lc_u_b_MP3ENC_P_buf
   EXTERN  F_lc_u_b_MP3ENC_Y_buf
   EXTERN  F_lc_u_e_MP3ENC_Y_buf
   EXTERN  F_lc_u_b_MP3ENC_X_buf
   EXTERN  F_lc_u_e_MP3ENC_X_buf

   EXTERN   ModuleSetTimer

   EXTERN   MsgMenuSongTimeChange
   EXTERN   SongCurrentMinutes
   EXTERN   SongCurrentSeconds
   EXTERN   MsgDecoderStateChange


    EXTERN  Fg_iEncTotalSeconds
    EXTERN  Fg_iEncCurrentSeconds

   EXTERN   FMp3EncFrameCount


   EXTERN   Fg_iRbrSetting
   EXTERN   Fg_wEncBitRate
   EXTERN   Fg_wEncNumberOfChannels
   EXTERN   Fg_wEncSamplingRate
   EXTERN   Mp3EncClusterTimeScale

   EXTERN  FInitMp3Encoder
   EXTERN  FResetMp3Encoder
   EXTERN  MP3EncoderSourceBufDesc
   EXTERN  MP3EncoderSourceBuf
   EXTERN  MP3EncoderBufHeadPtr
   EXTERN  MP3EncoderBufTailPtr
    EXTERN  MP3EncoderSinkBufModPtr
    EXTERN  MP3EncoderSinkBufTailPtr
   EXTERN  AdcDriverInit
   EXTERN  AdcDriverEnablePort
   EXTERN  MP3EncSignalReturn
   EXTERN  Const_000002
   EXTERN  Const_ffffff
   EXTERN  Const_000003
   EXTERN  Const_000001

   EXTERN  FSilenceSR
   EXTERN  FCheckSilenceBlockCount
   EXTERN  EncoderIsrEntryCount
   EXTERN  MP3EncoderSampleCountHigh
   EXTERN  MP3EncoderSampleCountLow
   EXTERN  MP3EncWriteFailCount
   EXTERN  MP3EncBytesWrittenHigh
   EXTERN  MP3EncBytesWrittenLow
   EXTERN  MP3EncIsrFlags
   EXTERN  FGetMP3Number
  if @def('USE_PLAYLIST3')
   EXTERN  FGetHighestMP3Number
  else
   EXTERN  FGetMP3Number
  endif
   EXTERN  FMP3_CreateFilename
   EXTERN  MsgMenuSongTrackchange
   EXTERN  LoadEncoderCommon
    EXTERN  FileSeek
    EXTERN  EncFileHandle
    EXTERN  FFlushCache
    EXTERN  DriveFlush

	if (@def('USE_PLAYLIST3'))
    EXTERN  FFgetFastHandle
	EXTERN  Fg_CurrentSongFastkey
	endif

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////

    nolist

    include "sysequ.inc"
    include "sysresources.inc"
    include "msgequ.inc"
    include "hwequ.inc"
    include "sysspeed.inc"
    include "decoderequ.inc"
    include "encmp3equ.inc"
    include "sysmacro.asm"
    include "SystemDefs.inc"
    include "msgequ.inc"

    list

    page    132,60,0,0,0

;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////


;///////////////////////////////////////////////////////////////////////////////
;   Code
;///////////////////////////////////////////////////////////////////////////////
      org     p,"ENCMOD_Mp3_overlay_P":

    ; Resource number
    dc      RSRC_ENC_MP3_MOD_CODE

;///////////////////////////////////////////////////////////////////////////////
; Function: EncMp3Init
; Inputs:   none
; Outputs:  Word value in A to place in ModuleTableEntry wait field specifies on
;           what events to call back this module's process function.
; Notes:
;///////////////////////////////////////////////////////////////////////////////
EncMp3Init

    ; init the ModuleTable process pointer
    move    #EncMp3ProcessQueue,x0
    move    x0,y:EncoderProcessPtr

    ; init the module state
    clr     a
    bset    #STATE_STOPPED,a1
    move    a1,x:EncModuleState

    ; come back if we get a message
    move    #>EVENT_MESSAGE,A

    rts

;///////////////////////////////////////////////////////////////////////////////
; Function:  EncMp3ProcessQueue
; Inputs:    r0 = Descriptor for this module's message queue (in y memory)
;            m0 = -1 for linear addressing mode
; Outputs:   word in A1 contains wait value that specifies on what events to
;            call back this module processing function.
; Notes:
;///////////////////////////////////////////////////////////////////////////////

EncMp3ProcessQueue

    ; save off stack
    move    #_stackSaved,r6              ; setup the return address
    move    #EncAdpcmOvlSave_sp,r5      ; pointer to the SP local save
    jmp     SaveSystemStack              ; save the hardware stack
_stackSaved

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
    jmp     CheckDone

ProcessMessage

    ; Put the tail pointer into r1
    move    b1,r1

    ; Save working variables
    push    a1                      ; Head pointer
    push    r0                      ; Pointer to queue descriptor
    push    r1                      ; Tail pointer
    push    m1                      ; Queue modulo

    ; Put the message ID into b
    move    #>MSG_ID,n1
    nop
    move    y:(r1+n1),b

    ; Verify that the message ID is valid by checking message range
    move    #>ENCODER_FIRST_MSG_ID,x0
    cmp     x0,b
    jlt     InvalidMessageID
    move    #>ENCODER_LAST_MSG_ID,x0
    cmp     x0,b
    jgt     InvalidMessageID

    ; Point r1 to the first argument of the message
    move    (r1)+
    move    (r1)+

    ; we don't use circular mode to access queue buffer with r1
    jsr     SysAllLinear	;stmp6661

    ; Clear the upper byte of the message ID
    move    #$00FFFF,x0
    and     x0,b

    ; Jump to the message handler
    move    #EncoderJumpTable,r2
    move    #$FFFF,m2
    move    b1,n2
    nop
    ; HACK, maybe not a good idea since this is a two-cycle instr, not one
    move    p:(r2+n2),r3
    nop
    jmp     (r3)

IdRecord
    ; Get the arguments
    ; Get the arguments. Depending upon EncoderSR state Pause or Record.
    move    x:EncoderSR,x0
    jset    #Play,x0,_recording_paused
    jsr     EncRecordMp3
    jmp     NextMessage

_recording_paused
    jsr     EncPause
    jmp     NextMessage

IdStop
    ; Get the arguments

    jsr     EncStop
    jmp     NextMessage

IdTimeMode
    ; Get the arguments

    jsr     EncTimeMode
    jmp     NextMessage

InvalidMessageID

    error

NextMessage
    ; Restore working variables
    pop     m1                      ; Queue modulo
    pop     r1                      ; Tail pointer
    pop     r0                      ; Pointer to buffer descriptor
    pop     a                       ; Head pointer

    ; Add the message length to the tail pointer
    move    y:(r1),n1
    nop
    move    (r1)+n1
    nop
    move    r1,b

CheckDone
    ; Check if head pointer equals the tail pointer
    cmp     a,b
    jne     ProcessMessage

    ; Update the buffer descriptor with the new tail pointer
    move    #>BD_TailPtr,n0
    nop
    move    b1,y:(r0+n0)


    jmp    EncProcess
enc_menuproc_return
      move y:<Const_ffffff,m1            ; Restore the modulo to linear  stmp6661
    ; restore stack
    move    #EncAdpcmOvlSave_sp,r5       ; pointer to the SP local save
    move    #_stackRestored,r6           ; setup the return address
    jmp     RestoreSystemStack           ; restore the hardware stack
_stackRestored

    move    y:MP3EncSignalReturn,a1

    ; All done
    rts
;///////////////////////////////////////////////////////////////////////////////

EncoderJumpTable
        dc      IdRecord
        dc      IdStop
        dc      IdTimeMode

; Note: It is important that the entries in the jump table be in the
; same order as the encoder message ID's which are defined in msgequ.inc

;///////////////////////////////////////////////////////////////////////////////
;   EncProcess
;///////////////////////////////////////////////////////////////////////////////
EncProcess
    move    x:EncModuleState,x0
    jset    #STATE_RECORDING,x0,EncStateRecording
    jset    #STATE_STOPPED,x0,EncStateStopped

    move    y:MP3EncSignalReturn,x0
    move    #>EVENT_INIT|EVENT_MESSAGE,a1
    or      x0,a
    move    a1,y:MP3EncSignalReturn
    jmp     enc_menuproc_return

;///////////////////////////////////////////////////////////////////////////////
;   EncStateStopped
;///////////////////////////////////////////////////////////////////////////////
EncStateStopped
    ; come back only if we get a message
    move    #>EVENT_MESSAGE,a1
    jmp     enc_menuproc_return

;///////////////////////////////////////////////////////////////////////////////
;   EncStateRecording
;///////////////////////////////////////////////////////////////////////////////
EncStateRecording

    btst    #0,y:FSilenceSR                                         ; check Silence detected??
    jcc     _ContinueCheckForSilence

    jsr     FCheckSilenceBlockCount                                 ; check whether end of album reached?
    tst     A
    jeq     _ContinueCheckForSilence

    jsr     SysAllLinear                                            ; end of album reached

    jsr     EncStop                                                 ; stop encoder

	if (@def('USE_PLAYLIST3'))
    clr     a                                                       ; reset fast key
	move    a0,y:Fg_CurrentSongFastkey
	move    a1,y:Fg_CurrentSongFastkey+1
	endif

    move    y:Fg_wEncCurrentDeviceNum,a2
    move    #EncFileNameString,a1
    bset    #22,a1
    jsr     FILEDelete                                              ; close the opened file

    jmp     enc_menuproc_return                                     ; return to the main menu

    ; Stop Recording
_ContinueCheckForSilence
    btst     #1,y:FSilenceSR                                        ; silence detected for the first time??
    jscs     CloseFileSilenceDetected                               ; close the current file and open the next

    ; If the battery level is below our voltage limit, stop recording.
    ; Prevents brownout, FAT corruption, and bad file generation that might hang the decoder later.
    jsr     FSysBatteryGetLevel     ; returns % of usable range in A
    push    a1
    jsr     GetEncVbatThreshholdAsPctOfUsableRange  ; returns % in x0
    move    x0,a
    tst     a
    pop     a
    jeq     _battery_ok ; checks if threshholding disabled (may occur in LiIon case)
_CompareVbatLevelToThreshhold
    nop
    cmp     x0,a
    jge     _battery_ok

    jsr     EncStop       ; Stop the MP3 encoder periodic ISR and clean up
    move    #>RECORDER_STOP_LOW_BATTERY,a ;Notify menus of record stop due to low batt
    jsr     PostEncoderStateChange
    jmp     enc_menuproc_return             ; Exit due to low battery.
_battery_ok
    ; Battery is ok, continue.

    ; Watch for errors coming from the isr.
    ; We could run out of disk space and need to stop.
    move    x:Mp3EncoderIsrSR,x0

    ;-----   DVRWARN                                                   testpoint
    ; If low space warning not indicated by isr, keep recording.
    jclr   #EncWarnLowSpace,x0,_keep_going

_warning_active
    ; Low Space Warning bit is ON                                      testpoint
    ; If we haven't done it yet, handle the low space warning.
    move    x:EncoderSR,y0
    jsclr   #EncWarningSent,y0,EncSendLowSpaceWarning
    move    x:Mp3EncoderIsrSR,x0           ; restore it

    ; Note: EncWarningSent bit gets reset to 0 in enc init.


_check_out_of_space
    ;-----
    jclr    #EncRanOutOfSpace,x0,_keep_going

    jsr     EncStop
    move    x:EncoderSR,x0				; Signal the menu why we stopped.
    bset    #EncRanOutOfSpace,x0
    move    x0,x:EncoderSR

    move    #>RECORDER_STOP_OUT_OF_SPACE,a
    jsr     PostEncoderStateChange
    jmp     enc_menuproc_return


_keep_going
    ; update time
    jsr     EncoderUpdateCurrentTime

    ; come back
    move    #>EVENT_MESSAGE,a1
    move    #>500,x0
    jsr     ModuleSetTimer
    move    a1,y:MP3EncSignalReturn
    jmp     enc_menuproc_return

;///////////////////////////////////////////////////////////////////////////////
;   EncRecordMp3
;///////////////////////////////////////////////////////////////////////////////
EncRecordMp3

    move    x:EncoderSR,x0
    jset    #Pause,x0,_recording_paused

    ; Init some vars.  These are system static.
    clr     a
    move    a,y:Fg_iEncTotalSeconds
    move    a,y:Fg_iEncCurrentSeconds

    ; load the encoder & fat write
    jsr     SysFlushCoderMemory
    jsr     SysLoadMp3Encoder
    jsr     SysLoadFATWrite
    jsr     LoadEncoderCommon

    ; make sure there's enough disk space
    jsr     EncCheckDiskSpace
    jcs     _not_enough_space

    ;----
    ; To find the clx available for recording, dec the FS value 'resvd clx count' times
    ; This gives correct 'max rec time available' on the display
    push    sr                          ; save post EncCheckDiskSpace SR
    clr     a
    move    b1,a0
    ; +1 more dec for another clx reserved by others
    do      #(ENC_MIN_FREE_CLUSTER_LIM+1),_adjust_for_resvd_clx
     dec    a
_adjust_for_resvd_clx
    move    a0,b1                       ; B1 is free clusters available for rec
    pop     sr                          ; restore post EncCheckDiskSpace SR
    ;----


    ; !!!! the following function requires the outputs of EncCheckDiskSpace
    ; calculate total possible record time
    jsr     CalcMp3MaxRecordTime
    move    a1,y:>Fg_iEncTotalSeconds

   ; Create the file
    jsr     EncCreateMp3File
    jcs     _file_create_failed

; This hack forces loading of Write portions of the media code for MMC player.
; This avoids re-entrancy in ResourceFileRead from Encoder ISR.
; The following block of code must be removed, when resource loading becomes reentrant
    move    y:MP3EncoderSinkBufTailPtr,r0
	clr     A
    move    A,x:(r0)
    move    y:<Const_000003,A1
    move    y:EncFileHandle,a2
    bset    #23,a0                      ; x memory
    move    y:MP3EncoderSinkBufModPtr,m0
    jsr     FileWrite
    jsr     FFlushCache   ; Force the writes to be committed to the disk, so that appropriate media components are loaded
    clr     A
    move    y:EncFileHandle,a2
    jsr     FileSeek                     ; rewind to beginning


    move    y:Fg_wEncBitRate,A
    clr     B                           #>0.001,x0
    move    y:Fg_wEncSamplingRate,y0
    mpy     x0,y0,B
    move    y:Fg_wEncNumberOfChannels,x0

    jsr     FInitMp3Encoder
    tst     A
    jne     _not_supported

_recording_paused
    ; Clear Pause bit
    bclr    #Pause,x:EncoderSR

    move    #>RECORDER_RESUME,a
    jsr     PostEncoderStateChange

    jsr     FResetMp3Encoder

    ; Init the isr
    jsr     EncMp3ModuleIsrInit

    ; Init the interrupt vectors
    move    #>AdcDriverIsr,x0
    move    #>AdcDriverIsr,x1
    move    #>EncMp3ModuleIsr,y0
    jsr     SysSetupEncVectors

    ; start the ADC
    move    #>MP3EncoderSourceBufDesc,r0
    jsr     AdcDriverInit
    jsr     AdcDriverEnablePort


    ; flag the isr to start recording
    clr     a
    bset    #Play,a1
    move    a1,x:EncoderCSR

    ; JLN Aug9 2004: Auto record test showed that MP3 enc at 44.1 and 48khz
    ; needed above 63.5MHz. 75MHz gave better results so use sample rate compare here.
    ; Note: min Vbat check is important for stability on core Freq raise to 75Mhz. (Default: enabled for 2.520RC1)
    ; We check for min Vbat in RecordVoiceFile before arriving here, and in this file during encode.
;    move    y:Fg_Wave_dwSamplesPerSec,A  ; could find MPEG ver of this variable.
;    move    #>44100,B
;    cmp     A,B
;    jle     _SetPeakSpeed   ; if B 44100 <= g_Wave_dwSamplesPerSec in A, raise fDCLK=75MHz

;    move    #>SPEED_MP3ENCODE,x1        ; Change to SPEED_MP3ENCODE
;    move    #>SPEED_CLIENT_MP3_ENC,y0	; speed client ID
;    jsr     SysSpeedIncrease
;    jmp     _skipPeakSpeed

_SetPeakSpeed
;   move    #>SPEED_MP3ENCODE,x1
    move    #>SPEED_PEAK,x1             ; Change to SPEED_PEAK improved record test in high datarate cases.
    move    #>SPEED_CLIENT_MP3_ENC,y0   ; speed client ID
    jsr     SysSpeedIncrease
_skipPeakSpeed

    ; Turn the Encoder Timer Module ON
    move    #>EncoderTimerNum,x0
    move    #_TimerEnabled_1,r0
    jmp     SysEnableTimer
_TimerEnabled_1

    ; TODO - add a timeout here
_WaitForRecord
    move    x:EncoderSR,x0
    jclr    #Play,x0,_WaitForRecord

    ; set the record start time
    jsr     SysGetCurrentTime
    move    a1,x:RecStartTimeHigh
    move    a0,x:RecStartTimeLow

    ; init the module state
    clr     a
    bset    #STATE_RECORDING,a1
    move    a1,x:EncModuleState

    move    #>RECORDER_START,a
    jsr     PostEncoderStateChange

_exit
    move    #>EVENT_INIT|EVENT_MESSAGE,a1
    move    a1,y:MP3EncSignalReturn

    rts


_already_recording
    clr     a
    bset    #Stop,a1
    bset    #EncAlreadyRecording,a1
    move    a1,x:EncoderSR
    ; TODO - what to do here???????
    error
    jmp     _exit

_not_enough_space
    jsr     SysUnLoadFATWrite
    clr     a
    bset    #Stop,a1
    bset    #EncNotEnoughSpace,a1
    move    a1,x:EncoderSR
    move    #>RECORDER_STOP_OUT_OF_SPACE,a
    jsr     PostEncoderStateChange
    jmp     _exit


_not_supported
    jsr     SysUnLoadFATWrite
    clr     a
    bset    #Stop,a1
    move    a1,x:EncoderSR
    move    #>RECORDER_STOP_NORMAL,a
    jsr     PostEncoderStateChange
    jmp     _exit


_file_create_failed
    jsr     SysUnLoadFATWrite
    ; ??????????????
    error

;-------------
; Function: GetVbatThreshholdAsPctOfUsableRange
; Register inputs: none. Reads DCDC mode field from revision register at x:HW_REVR.DCDCMODE
; Note: The Vbat threshhold values used below are defined in LCDexample's project.inc
; Outputs: x0 & a have Vbat % of usable range to use as threshhold for allowing mp3 enc to continue.
; issue: returned buck thresh when it should have given nand only thresh!!
GetEncVbatThreshholdAsPctOfUsableRange
  if (@def('DEVICE_3500'))
        ; if DCDC converter in buck mode, use buck mode encode stop threshhold (liion range)
        move    x:HW_REVR,x0
        clr     a
        move    #>HW_REVR_DCDC_MODE_SETMASK,a1 ;(HW_REVR_DCDCMODE_B2_BITPOS|HW_REVR_DCDCMODE_B1_BITPOS|HW_REVR_DCDCMODE_B0_BITPOS)
        and     x0,a                ; clears bits other than DCDC mode
        rep      #HW_REVR_DCDCMODE_B0_BITPOS ; shift DCDC mode field to low A1.
         lsr     a
        move    a1,x0               ; DCDC mode's three bit bitfield in low part of x0
         move    #>3,a              ; STMP35xx DCDC converter mode 3 is a buck mode
         cmp     x0,a
         jeq     _DcDcBuckMode
          move    #>1,a             ; mode 1 is a buck mode
          cmp     x0,a
          jeq     _DcDcBuckMode
           clr     a                ; mode 0 is the last buck mode, else boost mode.
           cmp     x0,a
           jeq     _DcDcBuckMode
           jmp     _notDcDcBuckMode
_DcDcBuckMode
        ; we're in buck mode so use the LiIon Vbat threshhold
        ;move    #>MP3_ENC_BUCKMODE_MIN_V_BAT_RANGE_PCT,x0 ; reenable this when LiIon player build verified.
        move    #>0,x0 ; buck mode lradc bat measurement gives junk value so no threshholding for buck yet (LiIon). cause: alkaline build.
        jmp     _compareBatLevelToThreshhold
_notDcDcBuckMode
    ; Our DCDC converter #1 is in boost mode. If MMC build, use Vbat threshhold for MMC case,
    ; else use nand only build Vbat threshhold
  if @def('MMC')
    move    #>MP3_ENC_MMC_MIN_V_BAT_RANGE_PCT,x0
    jmp     _compareBatLevelToThreshhold
  endif
    ; else nand only build.
    move    #>MP3_ENC_NANDONLY_MIN_V_BAT_RANGE_PCT,x0
    jmp     _compareBatLevelToThreshhold
  endif ; DEVICE_3500
;   This previous approach always compared to 10% of battery usable range. xPCM enc does this.
;_defaultVbatThreshhold
    move    #>BATT_SAFETY_MARGIN,x0 ;A low percentage of the functional voltage range.
_compareBatLevelToThreshhold
    move    x0,a                    ; return a copy of the % in both x0 and a.
    nop                             ; the caller then makes the comparison
    rts


;///////////////////////////////////////////////////////////////////////////////
;   EncPause
;///////////////////////////////////////////////////////////////////////////////
EncPause

    ; Flag the ISR to stop recording
    move    x:EncoderCSR,x0
    bset    #Stop,x0
    move    x0,x:EncoderCSR

_WaitForStop
    move    x:EncoderSR,x0
    jclr    #Stop,x0,_WaitForStop

    bset    #Pause,x:EncoderSR
    move    #>RECORDER_PAUSE,a
    jsr     PostEncoderStateChange

    ; Turn Timer Module Off
    move    #>EncoderTimerNum,x0
    move    #_TimerDisabled,r0
    jmp     SysDisableTimer
_TimerDisabled

    jsr     SysAllLinear
    jsr     FResetMp3Encoder

    move   #MP3EncoderSourceBuf,x0
    move   x0,y:MP3EncoderBufHeadPtr
    move   x0,y:MP3EncoderBufTailPtr

    ; Free DCLK for other speed api users
    move    #>SPEED_CLIENT_MP3_ENC,y0
    jsr     SysSpeedClockFree


_exit_encstop
    move    #>EVENT_INIT|EVENT_MESSAGE,a1
    move    a1,y:MP3EncSignalReturn
    rts



;///////////////////////////////////////////////////////////////////////////////
;   EncSendLowSpaceWarning (internal routine)        DVRWARN
;   Description: Sends a warning msg on the soft stack to the menus to indicate
;                that record space runs out in a few more seconds
;   Inputs:  None
;   Returns: None
;   Destroys: y0
;///////////////////////////////////////////////////////////////////////////////
EncSendLowSpaceWarning
    ; Send a warning msg to the menus to indicate low record space.

    move r7,r0

    move y:>Const_000002,y0
    push y0

    move #>MENU_MSG_ENC_LOW_SPACE,y0
    push y0

    jsr SysPostMessage

    pop y0
    pop y0

;    testpoint

    ; Prevents us from sending excess & redundant messages.
    ; Note: This warn 'msg sent' bit is reset to 0 elsewhere when record space becomes available.
    ; Use x:EncoderSR not x:EncoderIsrSR as this bool is part of a user-state flag.
    move    x:EncoderSR,y0
    bset    #EncWarningSent,y0
    move    y0,x:EncoderSR
    rts

;///////////////////////////////////////////////////////////////////////////////
;   Function: EncStop
;   Purpose:  Flag the ISR to stop encoding, Turn off timer,
;             unload the FAT write code, Set EncModuleState to stopped,
;             free the DCLK, & send encoder state change msg RECORDER_STOP_NORMAL.
;   Inputs:   No parameters but reads and writes the globals below.
;   Outputs:  No register returns. Registers not restored.
;
;   Global access: Reads x:EncoderSR, writes x:EncoderCSR, reads y:EncoderIsrState,
;                  writes x:EncModuleState, writes y:MP3EncSignalReturn
;///////////////////////////////////////////////////////////////////////////////
EncStop

    move    x:EncoderSR,x0
    jset    #Stop,x0,_exit_encstop

    ; Flag the ISR to stop recording
    move    x:EncoderCSR,x0
    bset    #Stop,x0
    move    x0,x:EncoderCSR

_WaitForStop
    move    x:EncoderSR,x0
    jclr    #Stop,x0,_WaitForStop

    ; Turn Timer Module Off
    move    #>EncoderTimerNum,x0
    move    #_TimerDisabled,r0
    jmp     SysDisableTimer
_TimerDisabled

    ; close the file
	if (@def('USE_PLAYLIST3'))
    jsr     SysAllLinear
    move    y:MP3EncFileHandle,a1
    jsr     FFgetFastHandle
	move    a0,y:Fg_CurrentSongFastkey
	move    a1,y:Fg_CurrentSongFastkey+1
	endif

    jsr     SysAllLinear
    clr     a
    move    y:MP3EncFileHandle,a2
    jsr     FILEClose
    jcs     _file_close_failed

;    ; flush drive 0. In case of flash drives, this frees all update blocks for next encoding.
;    clr     a           ; drive number. NOTE: since voicemenu RecordVoiceFile now calls DriveFlush(0), disable this one.
;    jsr     DriveFlush  ;

    ; Unload the FAT write code
    jsr     SysUnLoadFATWrite

    ; Change state to stopped
    clr     a
    bclr    #STATE_STOPPED,a1
    move    a1,x:EncModuleState  ; Note that this var is an enum, not a bitfield

    ;move    #>1,a                 ; for debug only
    ;move    a1,y:FVStepHalt   ; global causes halt at entry into VoltageStep interface

    ; Free DCLK for other speed api users
    move    #>SPEED_CLIENT_MP3_ENC,y0
    jsr     SysSpeedClockFree

    move    #>RECORDER_STOP_NORMAL,a
    jsr     PostEncoderStateChange

_exit_encstop
    move    #>EVENT_INIT|EVENT_MESSAGE,a1
    move    a1,y:MP3EncSignalReturn
    rts

_file_close_failed
    error


;///////////////////////////////////////////////////////////////////////////////
;   EncTimeMode
;///////////////////////////////////////////////////////////////////////////////
EncTimeMode
    move    #>EVENT_INIT|EVENT_MESSAGE,a1
    move    a1,y:MP3EncSignalReturn
    rts

;///////////////////////////////////////////////////////////////////////////////
;   EncCreateMp3File
;///////////////////////////////////////////////////////////////////////////////
EncCreateMp3File

_create_file
    ; create the file
    jsr     SysAllLinear
    move    y:Fg_wEncCurrentDeviceNum,a2                   ; device number
    move    #EncFileNameString,a1
    bset    #22,a1
    jsr     FILECreate
    jcs     _file_create_failed

    ; save the file handle
    move    a1,y:MP3EncFileHandle

    bclr    #HW_SR_C_BITPOS,sr
    rts

_file_create_failed
    ; delete it
    jsr     SysAllLinear
    move    y:Fg_wEncCurrentDeviceNum,a2                   ; device number
    move    #EncFileNameString,a1
    bset    #22,a1
    jsr     FILEDelete
    jcs     _failed_to_delete
    jmp     _create_file

_failed_header_write
    error

_failed_to_delete
    error


CloseFileSilenceDetected
    ; close the file
    jsr     SysAllLinear
    clr     a
    move    y:MP3EncFileHandle,a2
    jsr     FILEClose

    ; init some vars.  These are system static.
    clr     a
    move    a,y:Fg_iEncCurrentSeconds
    move    a1,y:EncoderIsrEntryCount
    move    a1,y:MP3EncoderSampleCountHigh
    move    a1,y:MP3EncoderSampleCountLow
    move    a1,y:MP3EncWriteFailCount
    move    a1,y:MP3EncBytesWrittenHigh
    move    a1,y:MP3EncBytesWrittenLow
    move    a1,y:MP3EncIsrFlags
    move    y:Fg_wEncBitRate,A
    clr     B                           #>0.001,x0
    move    y:Fg_wEncSamplingRate,y0
    mpy     x0,y0,B
    move    y:Fg_wEncNumberOfChannels,x0

    jsr     FInitMp3Encoder

    jsr     FResetMp3Encoder

    ; get the next file number
    move    #EncFileNameString,r0
  if @def('USE_PLAYLIST3')
    jsr     FGetHighestMP3Number
  else
    jsr     FGetMP3Number
  endif
    ; increment the file number
    move    y:<Const_000001,x0
    add     x0,A
    move    #EncFileNameString,r0

    ; generate the next file name
    jsr     FMP3_CreateFilename

    ; create the mp3 file
    jsr     EncCreateMp3File

    ; Post the message to menu to update the track name
    move    #MsgMenuSongTrackchange,r0
    move    #-1,m0
    jsr     SysPostMessage

    bclr    #1,y:FSilenceSR

    rts

;///////////////////////////////////////////////////////////////////////////////
; EncoderUpdateCurrentTime
;///////////////////////////////////////////////////////////////////////////////
EncoderUpdateCurrentTime
; Updates the current elapsed time of the song and the remaining time on disk
; Sends a message to the LCD to print them if they have changed.

_CalMp3EncodingCurrentTime
    move    y:Fg_wEncSamplingRate,x0

    move    #>44100,A
    cmp     x0,A		 #>0.026122,y0
    jeq     _ComputeInverseNumFrames

    move    #>48000,A
    cmp     x0,A		 #>0.024000,y0
    jeq     _ComputeInverseNumFrames
    ; Else 32000
    move    #>0.036,y0
_ComputeInverseNumFrames
	move	y:FMp3EncFrameCount,x0
	mpy	x0,y0,a
	move	#0,a0
_checkIfUpdateNeeded
        move    y:Fg_iEncCurrentSeconds,y0
        cmp     y0,a
        jne     _update
        rts
_update
        move    a1,y:Fg_iEncCurrentSeconds
        move    #MsgMenuSongTimeChange,r0
        move    #-1,m0
        jsr     SysPostMessage
        rts

;///////////////////////////////////////////////////////////////////////////////
; SysLoadMp3Encoder
;///////////////////////////////////////////////////////////////////////////////
SysLoadMp3Encoder
    ; Load the MP3 encoder into P memory
    move    #>RSRC_ENC_MP3P_CODE,x0       ; Resource number
    move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
    move    #>(F_lc_u_e_MP3ENC_P_buf-F_lc_u_b_MP3ENC_P_buf)*3,y1            ; Size of the overlay area
    move    #>F_lc_u_b_MP3ENC_P_buf,y0    ; Address of the overlay area
    move    #TARGET_MEM_P,a1                ; Overlay area is in P memory
    jsr     SysLoadResource

    ; Load the MP3 encoder into X memory
    move    #>RSRC_ENC_MP3X_CODE,x0       ; Resource number
    move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
    move    #>(F_lc_u_e_MP3ENC_X_buf-F_lc_u_b_MP3ENC_X_buf)*3,y1            ; Size of the overlay area
    move    #>F_lc_u_b_MP3ENC_X_buf,y0    ; Address of the overlay area

    move    #TARGET_MEM_X,a1                ; Overlay area is in X memory
    jsr     SysLoadResource

    ; Load the MP3 encoder into Y memory
    move    #>RSRC_ENC_MP3Y_CODE,x0       ; Resource number
    move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
    move    #>(F_lc_u_e_MP3ENC_Y_buf-F_lc_u_b_MP3ENC_Y_buf)*3,y1            ; Size of the overlay area
    move    #>F_lc_u_b_MP3ENC_Y_buf,y0    ; Address of the overlay area
    move    #TARGET_MEM_Y,a1                ; Overlay area is in Y memory
    jsr     SysLoadResource

    rts

;///////////////////////////////////////////////////////////////////////////////
;   CalcMp3MaxRecordTime
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
CalcMp3MaxRecordTime
	move	r0,x0				; save r0
	move	n0,x1				; save n0

	move	y:Fg_iRbrSetting,r0
	move	#Mp3EncClusterTimeScale,n0
        nop
	move	x:(r0+n0),y0
	move	x0,r0				; pop r0
	move	x1,n0				; pop n0
	move    b0,x1                           ; get bytespercluster ready
	mpy	y0,x1,a		b1,x0
	do	#4,_end_scale_adjust3
	asl	a
	asl	a
_end_scale_adjust3
	asl	a
	move	a1,x1

	mpy	x0,x1,a                         ; a = # of second / 10
	move	a1,x0				; integral part
	move	a0,y0				; decimal part
	move	#>5,x1
	mpy	x0,x1,a
	jclr	#23,y0,_MagNoChange
	rep	#5
	inc	a
	bclr	#23,y0
_MagNoChange
	move	a0,x0
	mpy	y0,x1,a
	add	x0,a				; a = num. of second
	rts

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
    ; error macro halts in DEBUG builds or resets the DSP in RETAIL builds.

PostEncoderStateChange
    move    y:<Const_ffffff,m0
    move    r7,r0
    move    y:<Const_000003,x0
    push    x0
    move    #>MENU_RECORDER_STATE_CHNG,x0
    push    x0
    push    a1
    jsr     SysPostMessage
    pop     a1
    pop     x0
    pop     x0
    rts


        endsec
