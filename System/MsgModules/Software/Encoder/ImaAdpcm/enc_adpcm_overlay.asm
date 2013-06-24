;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2004
;
; STMP3 Encoder ADPCM Ima Module
;///////////////////////////////////////////////////////////////////////////////

    include "resource.inc"
    include "project.inc"
    opt     mex

    section ENCMOD_AdpcmIma



;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////

; states for EncModuleState
STATE_STOPPED           equ              0
STATE_RECORDING         equ              1
WRITE                   equ              2

;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

   GLOBAL   EncAdpcmImaProcessQueue
   GLOBAL   EncAdpcmImaInit


;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

   EXTERN  Fg_iEncTotalSeconds
   EXTERN  Fg_iEncCurrentSeconds
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
   EXTERN   EncoderIsrSR
   EXTERN   EncoderIsrState
   EXTERN   EncCheckDiskSpace
   EXTERN   CalcMaxRecordTime
   EXTERN   CalcHrsMinSec
   EXTERN   AdcDriverInit         
   EXTERN   AdcDriverEnablePort
   EXTERN   AdcDriverIsr   
   EXTERN   EncAdpcmImaModuleIsr    
   EXTERN   EncAdpcmImaModuleIsrInit
   EXTERN   FSysBatteryGetLevel
   EXTERN   FFadeoutPreviousFrames
   EXTERN   RecStartTimeHigh 
   EXTERN   RecStartTimeLow  
   EXTERN   FClearSourceBuffer
   EXTERN   EncSignalReturn

   EXTERN   EncFileNameString
   EXTERN   AdpcmEncFileHandle             
   EXTERN   Fg_wEncCurrentDeviceNum
    
   EXTERN   EncAdpcmOvlSave_sp             

   EXTERN   EncModuleState

   EXTERN   SysLoadFATWrite
   EXTERN   SysUnLoadFATWrite

   EXTERN   FILEOpen
   EXTERN   FILEClose
   EXTERN   FILEAppend
   EXTERN   FILECreate
   EXTERN   FILEDelete
   EXTERN   FileWrite

	if (@def('USE_PLAYLIST3'))
    EXTERN  FFgetFastHandle
	EXTERN  Fg_CurrentSongFastkey
	endif
   
    extern Fg_Wave_wChannels
    extern Fg_Wave_dwSamplesPerSec
    extern g_wEncFormatTag
    extern g_wEncNumberOfChannels
    extern g_wEncSamplingRate
    extern g_wEncPCMBits
    extern FInitialize_Wave_Header
    extern FWrite_Wave_Header
    extern Fg_Wave_datasize
    extern Fg_Wave_wFormatTag
    extern Encode_MS_ADPCM_Block
    extern Encode_IMA_ADPCM_Block
    extern Encode_PCM_Block
    extern AdpcmEncoderPointer
    extern Fg_Wave_dwAvgBytesPerSec
    extern Fg_Wave_wBlockAlign
 if @DEF('TUNER_STFM1000')
 	EXTERN Fg_ADCsource
	EXTERN Fg_EncodeStfm
 endif
    EXTERN   NextVoiceNumValue
    EXTERN   InvSampleRate
    EXTERN  FileSeek
    EXTERN  FInit_IMA_ADPCM_States
    EXTERN  FInit_MS_ADPCM_States
   EXTERN  F_lc_u_b_ADPCM_P
   EXTERN  F_lc_u_e_ADPCM_Enc_P
   EXTERN  F_lc_u_b_ADPCM_X
   EXTERN  F_lc_u_e_ADPCM_Enc_X
   EXTERN  F_lc_u_b_ADPCM_Y
   EXTERN  F_lc_u_e_ADPCM_Enc_Y
   EXTERN   EncoderBufHeadPtr
   EXTERN   EncoderBufTailPtr
   EXTERN   ModuleSetTimer
   extern   EncoderSourceBuf
   EXTERN   MsgMenuSongTimeChange
   EXTERN   SongCurrentMinutes
   EXTERN   SongCurrentSeconds
   EXTERN   MsgDecoderStateChange
   EXTERN   SysDelay
   EXTERN   ClearADCFilterStates
   EXTERN   Fg_ADCsource
   include "const.xref"
   include "enc_adpcm_overlay.xref"

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
    include "adpcmequ.inc"
    include "encadpcmequ.inc"
    include "sysmacro.asm"
    include "SystemDefs.inc"
    include "msgequ.inc"

    list

    page    132,60,0,0,0

;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////

                               ;  RIFF header size (bytes), not counting the first 8 bytes
                               ;  This gets added to the number of bytes of data written
                               ;  to get the RIFF chunk size written to the file
RIFF_HDR_SIZE   equ 90



;///////////////////////////////////////////////////////////////////////////////
;   Code
;///////////////////////////////////////////////////////////////////////////////
      org     p,"ENCMOD_AdpcmIma_overlay_P":

    ; Resource number
    dc      RSRC_ENC_ADPCM_MOD_CODE

;*******************************************************************************
; Function     EncAdpcmImaInit
; Description: Writes the Encoder Process function pointer and inits the encoder
;              module state to stopped. Requests wait for next encoder msg.
; Inputs:      none
; Outputs:     A has word return value to be stored in ModuleTableEntry.wait
; Notes:          
;*******************************************************************************
EncAdpcmImaInit

    ; init the ModuleTable process pointer
    move    #EncAdpcmImaProcessQueue,x0
    move    x0,y:EncoderProcessPtr

    ; init the module state
    clr     a
    bset    #STATE_STOPPED,a1
    move    a1,x:EncModuleState
    
    
    ; wait to call this module back until a message arrives
    move    #>EVENT_MESSAGE,A
    
    rts

;*******************************************************************************
; Function:  EncAdpcmImaProcessQueue
; Inputs:    r0 = Descriptor for this modules message queue (in y memory)
;            m0 = set to -1 for linear addressing mode
;
; Outputs:   a1 word wait value specifies events to wait for before calling back
;            this module's process function.
; Notes:        
;*******************************************************************************

EncAdpcmImaProcessQueue

    ; save off stack
    move    #_stackSaved,r6              ; setup the return address
    move    #EncAdpcmOvlSave_sp,r5      ; pointer to the SP local save
    jmp     SaveSystemStack              ; save the hardware stack
_stackSaved
    push    m1
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
    ; Get the arguments. Depending upon EncoderSR state Pause or Record.
    move    x:EncoderSR,x0
    jset    #Play,x0,_recording_paused
    jsr     EncRecord
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
    pop     m1
    ; restore stack
    move    #EncAdpcmOvlSave_sp,r5       ; pointer to the SP local save
    move    #_stackRestored,r6           ; setup the return address
    jmp     RestoreSystemStack           ; restore the hardware stack
_stackRestored

    move    y:EncSignalReturn,a1

    ; All done
    rts

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
    
    move    y:EncSignalReturn,x0
    move    #>EVENT_INIT|EVENT_MESSAGE,a1
    or      x0,a
    move    a1,y:EncSignalReturn
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
	; Stmp00004393
    ; If the battery level is below our voltage limit, stop recording.  
    ; Prevent FAT corruption and bad file generation that might hang the decoder later.
    jsr     FSysBatteryGetLevel ; returns % of useful battery Voltage range in reg A
    move    #>BATT_SAFETY_MARGIN,x0 ;A low percentage of the functional voltage range. 
    cmp     x0,a
    jge     _battery_ok
    
    jsr     EncStop       ; Stop the xPCM encoder periodic ISR and clean up
    move    #>RECORDER_STOP_LOW_BATTERY,a ;Notify menus of record stop due to low batt
    jsr     PostEncoderStateChange
    jmp     enc_menuproc_return             ; Exit due to low battery.
_battery_ok    
        ; Battery is ok, continue. 
 
    ; Watch for errors coming from the isr
    ; could run out of disk space and need to stop
    move    x:EncoderIsrSR,x0
    jclr    #EncRanOutOfSpace,x0,_keep_going

    jsr     EncStop
    move    x:EncoderSR,x0				; Signal the menu why we stopped.
    bset    #EncRanOutOfSpace,x0
    move    x0,x:EncoderSR

    ; Send a StateChanged message to menu to wake it up
    ; Using MENU_DECODER_STATE_CHNG, instead of MENU_ENCODER_STATE_CHNG, just to save space
    move    #>RECORDER_STOP_OUT_OF_SPACE,a 
  	jsr     PostEncoderStateChange

    jmp     enc_menuproc_return


_keep_going
    ; update time 
    jsr     EncoderUpdateCurrentTime
    
    ; come back
    move    #>EVENT_MESSAGE,a1
    move    #>250,x0
    jsr     ModuleSetTimer
    move    a1,y:EncSignalReturn
    jmp     enc_menuproc_return
    
;///////////////////////////////////////////////////////////////////////////////
;   EncRecord
;///////////////////////////////////////////////////////////////////////////////
EncRecord

    move    x:EncoderSR,x0
    jset    #Pause,x0,_recording_paused

    ; Init some vars.  These are system static.
    clr     a
    move    a,y:Fg_iEncTotalSeconds
    move    a,y:Fg_iEncCurrentSeconds

    ; load the encoder & fat write 
 if @DEF('TUNER_STFM1000')
    clr	a
    move	y:Fg_ADCsource,a0		; if encoding from Steely Dan, do not flush the App memory spaces
    dec		a
    jeq     _EncodeFM
 endif
    jsr     SysFlushCoderMemory
_EncodeFM
    jsr     SysLoadAdpcmImaEncoder
    jsr     SysLoadFATWrite
    jsr     LoadEncoderCommon

    
    ; Setup the initial wave header
    move	y:g_wEncFormatTag,A
	move	y:g_wEncNumberOfChannels,B
	move	y:g_wEncSamplingRate,x0
    move    y:g_wEncPCMBits,y0
    
    jsr     SysAllLinear
    jsr     FInitialize_Wave_Header
	tst		A
	jne		_not_supported

    ; Find the multiplier to compute Samples per second, etc.
    jsr     GetInvBitRate

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
    jsr     CalcMaxRecordTime
    move    a1,y:>Fg_iEncTotalSeconds

    ; Create the file
    jsr     EncCreateFile
    jcs     _file_create_failed
    
    jsr     SysAllLinear
    jsr     FWrite_Wave_Header   
    ; The following is a hack which forces data cache to be flushed to the disk, 
    ; thereby preventing media components from being loaded from ISR.
    jsr     FFlushCache ;Force writes to be committed to disk, so appropriate media components load
    
    jsr     SetEncoderType
    
_recording_paused
    ; Clear Pause bit
    bclr    #Pause,x:EncoderSR
    
    move    #>RECORDER_RESUME,a
    jsr     PostEncoderStateChange
    
    
    ; Init the isr
    jsr     EncAdpcmImaModuleIsrInit
    
    ; Init the interrupt vectors
    move    #>AdcDriverIsr,x0
    move    #>AdcDriverIsr,x1
    move    #>EncAdpcmImaModuleIsr,y0
    jsr     SysSetupEncVectors
    
    
    ; start the ADC
 if @def('TUNER_STFM1000')
    clr	a
    move	y:Fg_ADCsource,a0		; if encoding from Steely Dan, ADC is not running
    dec		a
    jne     _InitADCdriver
	move    y:Fg_EncodeStfm,a0
	bset	#0,a0
	move    a0,y:Fg_EncodeStfm
    jmp     _byPassADCdriver
 endif
_InitADCdriver
    move    #EncoderSourceBufDesc,r0
    jsr     AdcDriverInit
    jsr     AdcDriverEnablePort
_byPassADCdriver    
    ; flag the isr to start recording
    clr     a
    bset    #Play,a1
    move    a1,x:EncoderCSR

    ; JLN Aug9 2004: Auto record test showed that PCM and ADPCM 44.1 and 48khz mono 
    ; may need fDCLK above 60MHz. 75MHz gave better results so use sample rate compare here.
    ; Byte rate comparison would raise some low sample rates that don't need it. TZ codesign.
    ; Note: Sigmatel only uses 44.1 and 48khz xPCM enc during TEST. These samplerates are not 
    ; offered in player settings menu.
    ; testpoint
;    move    y:Fg_Wave_dwSamplesPerSec,A
;    move    #>44100,B
;    cmp     A,B
;    jle     _Speed75Mhz   ; if B 44100 <= g_Wave_dwSamplesPerSec in A, raise fDCLK=75MHz

    move    y:Fg_Wave_dwAvgBytesPerSec,a
    move    y:g_wEncFormatTag,B
    move    #>WAVE_FORMAT_PCM,x0
    cmp     x0,B
    jeq     _SetPCMEncoder
    ; Set speeds for ADPCM encoders
;    move    #>45000,b
    move    #>8000,b      ; This was changed for 2.4xx due to adc overflows
    cmp     b,a
   ;jmp _Speed75Mhz      ; Test tool only: Always setting 75MHz here fixed record TEST.
    jgt     _Speed60Mhz
;    move    #>40000,b
    move    #>5000,b      ; This was changed for 2.4xx due to adc overflows
    cmp     b,a
    jgt     _Speed48Mhz
    jmp     _SpeedNormal
    
_SetPCMEncoder    
    move    y:Fg_Wave_wChannels,y0
    move    y:Fg_Wave_dwSamplesPerSec,A
    rep     y0
    asl     A
    asr     A
    move    y:Fg_Wave_dwAvgBytesPerSec,B
    asr     B
    add     B,A
    move    #>64000,b    ; This was changed for sdk 2.4xx due to adc overflows
;    move    #>150000,b
    cmp     b,a
    jge     _Speed60Mhz  ; This was changed for sdk 2.4xx due to adc overflows
    jmp     _Speed48Mhz  ; This was changed for sdk 2.4xx due to adc overflows
;    jgt     _Speed60Mhz
;    move    #>120000,b
;    cmp     b,a
;    jgt     _Speed48Mhz
    
_SpeedNormal
    move    #>SPEED_ADCBASE,x1              ; To SPEED_ADCBASE (36MHz) but adc base vddd is met.
    move    #>SPEED_CLIENT_ADPCM_ENC,y0     ; Speed client ID
    jsr     SysSpeedIncrease
    jmp	    _TimerModule

_Speed48Mhz    
    move    #>SPEED_MAX,x1                  ; To SPEED_MAX if we're not higher
    move    #>SPEED_CLIENT_ADPCM_ENC,y0     ; Speed client ID
    jsr     SysSpeedIncrease
    jmp	    _TimerModule

_Speed60Mhz    
    move    #>SPEED_WMA,x1                  ; To SPEED_WMA if we're not higher
    move    #>SPEED_CLIENT_ADPCM_ENC,y0     ; Speed client ID
    jsr     SysSpeedIncrease
;    jmp     _TimerModule
;
;_Speed75Mhz    
;    move    #>SPEED_PEAK,x1                 ; To SPEED_PEAK if we're not higher
;    move    #>SPEED_CLIENT_ADPCM_ENC,y0     ; Speed client ID
;    jsr     SysSpeedIncrease

_TimerModule    
    
    move   #>100,x0
    jsr    SysDelay
    jsr    SysAllLinear
    jsr    FClearSourceBuffer 
    move   #EncoderSourceBuf,x0
    move   x0,y:EncoderBufHeadPtr
    move   x0,y:EncoderBufTailPtr

    jsr     ClearADCFilterStates

      
    ; Turn the Encoder Timer Module ON
    move    #>EncoderTimerNum,x0
    move    #_TimerEnabled_1,r0
    jmp     SysEnableTimer
_TimerEnabled_1

    ; TODO - add a timeout here
_WaitForRecord
    move    x:EncoderCSR,x0
    jset    #Play,x0,_WaitForRecord
    
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
    move    a1,y:EncSignalReturn
    
    rts
    
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
    jmp     _exit



_file_create_failed
    jsr     SysUnLoadFATWrite
    ; ??????????????
    error
    error
        
_failed
    error    
        

;///////////////////////////////////////////////////////////////////////////////
;   EncStop
;///////////////////////////////////////////////////////////////////////////////
EncStop
    push    ssh
    nop
    
    bclr    #Pause,x:EncoderSR
    
    move    x:EncoderSR,x0
    jset    #Stop,x0,_exit_encstop
_exec_encstop
    ; Flag the ISR to stop recording
    move    x:EncoderCSR,x0
    bset    #Stop,x0
    move    x0,x:EncoderCSR

_WaitForStop
    move    x:EncoderCSR,x0
    jset    #Stop,x0,_WaitForStop

    ; Turn Timer Module Off
    move    #>EncoderTimerNum,x0
    move    #_TimerDisabled,r0
    jmp     SysDisableTimer
_TimerDisabled

    move    y:Fg_wEncCurrentDeviceNum,a
    move    #>EncFileNameString,r4
    move    #AdpcmEncFileHandle,r0
    jsr     SysAllLinear
;    jsr     FFadeoutPreviousFrames
    
    move   #EncoderSourceBuf,x0
    move   x0,y:EncoderBufHeadPtr
    move   x0,y:EncoderBufTailPtr
    
    
    ; Rewind the file & update the RIFF header
    jsr     EncUpdateRiffHeader  

    ; Unload the FAT write code
    jsr     SysUnLoadFATWrite

    ; Change state to stopped
    clr     a
    bclr    #STATE_STOPPED,a1
    move    a1,x:EncModuleState  ; Note that this var is an enum, not a bitfield
    
    ; Free DCLK for other speed api users
    move    #>SPEED_CLIENT_ADPCM_ENC,y0
    jsr     SysSpeedClockFree


_exit_encstop
    
    move    #>EVENT_INIT|EVENT_MESSAGE,a1
    move    a1,y:EncSignalReturn
    pop     ssh
    nop
    rts

        

;///////////////////////////////////////////////////////////////////////////////
;   EncPause
;///////////////////////////////////////////////////////////////////////////////
EncPause

    bset    #Pause,x:EncoderSR

    ; Flag the ISR to stop recording
    move    x:EncoderCSR,x0
    bset    #Stop,x0
    move    x0,x:EncoderCSR

_WaitForStop
    move    x:EncoderCSR,x0
    jset    #Stop,x0,_WaitForStop
    
    move    #>RECORDER_PAUSE,a
    jsr     PostEncoderStateChange

    ; Turn Timer Module Off
    move    #>EncoderTimerNum,x0
    move    #_TimerDisabled,r0
    jmp     SysDisableTimer
_TimerDisabled

    move    y:Fg_wEncCurrentDeviceNum,a
    move    #>EncFileNameString,r4
    move    #AdpcmEncFileHandle,r0
    jsr     SysAllLinear
    jsr     FFadeoutPreviousFrames
    
    move   #EncoderSourceBuf,x0
    move   x0,y:EncoderBufHeadPtr
    move   x0,y:EncoderBufTailPtr
    
    ; Free DCLK for other speed api users
    move    #>SPEED_CLIENT_ADPCM_ENC,y0
    jsr     SysSpeedClockFree


_exit_encstop
    move    #>EVENT_INIT|EVENT_MESSAGE,a1
    move    a1,y:EncSignalReturn
    rts



        
;///////////////////////////////////////////////////////////////////////////////
;   EncTimeMode
;///////////////////////////////////////////////////////////////////////////////
EncTimeMode
    move    #>EVENT_INIT|EVENT_MESSAGE,a1
    move    a1,y:EncSignalReturn
    rts

;///////////////////////////////////////////////////////////////////////////////
;   EncCreateFile
;///////////////////////////////////////////////////////////////////////////////
EncCreateFile
    
_create_file
    ; create the file
    jsr     SysAllLinear
    move    y:Fg_wEncCurrentDeviceNum,a2                   ; device number
    move    #EncFileNameString,a1
    bset    #22,a1
    jsr     FILECreate
    jcs     _file_create_failed

    ; save the file handle
    move    a1,y:AdpcmEncFileHandle
    
    ; increment the voice number
    ; TODO - watch for overflow here
    clr     a
    move    x:NextVoiceNumValue,a0
    inc     a
    move    a0,x:NextVoiceNumValue
    
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
        
;///////////////////////////////////////////////////////////////////////////////
;   EncCreateFileName
;///////////////////////////////////////////////////////////////////////////////
EncUpdateRiffHeader
    push    ssh
    nop
    ; close the file
    jsr     SysAllLinear
    clr     a
    move    y:AdpcmEncFileHandle,a2
    jsr     FILEClose ;oflowed the HW sys stack due to # of subr calls. stmp3347
    jcs     _file_close_failed
    
    ; open it so we can write to the beginning
    jsr     SysAllLinear

    move    y:Fg_wEncCurrentDeviceNum,a2
    move    #>EncFileNameString,a1
    bset    #22,a1
    move    #>WRITE,a0
    jsr     FILEOpen
    jcs     _file_open_failed             
    
    move    a1,y:AdpcmEncFileHandle

	if (@def('USE_PLAYLIST3'))
    jsr     SysAllLinear
    move    y:AdpcmEncFileHandle,a1
    jsr     FFgetFastHandle
	move    a0,y:Fg_CurrentSongFastkey
	move    a1,y:Fg_CurrentSongFastkey+1
	endif

	; Update the Wave Header
    jsr     SysAllLinear
	jsr     FWrite_Wave_Header
    
    ; close the file
    jsr     SysAllLinear
    clr     a
    move    y:AdpcmEncFileHandle,a2
    jsr     FILEClose
    jcs     _file_close_failed_2

_exit
    bclr    #HW_SR_C_BITPOS,sr
    pop     ssh
    nop
    rts

_file_close_failed
    error
_file_open_failed
    error
_file_close_failed_2
    error
    

;///////////////////////////////////////////////////////////////////////////////
; EncoderUpdateCurrentTime
;///////////////////////////////////////////////////////////////////////////////
EncoderUpdateCurrentTime
; Updates the current elapsed time of the song and the remaining time on disk
; Sends a message to the LCD to print them if they have changed.

		move	y:Fg_Wave_datasize,x0
		move	y:Fg_Wave_datasize+1,x1
        jsr     GetSongTime
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
; GetSongTime
;   Entry:
;     x0 = num samples low
;     x1 = num samples high
;     y1 = inverse sample rate scale factor
;   Exit
;     a1 = seconds
; 
;  Notes:
;///////////////////////////////////////////////////////////////////////////////
GetSongTime
    clr     A       #0,y0
    ; Keep life simple use dual multiply.
    ori     #$40,mr
    move    y:InvSampleRate,y1          ; Pipeline Delay and load SP
    mac     x0,y1,a
    mac     x1,y1,a         a0,x0       ; Discard the lower 24 bits.
    andi    #$bf,mr
    ; a1:a0 contains the result        
    ; Because our inverse contains a built in factor.. correct for it.
    rep     #9                          ; Pipeline Delay
    asr     A
    move    a0,a
    rts
;///////////////////////////////////////////////////////////////////////////////
; SysLoadAdpcmImaEncoder
;///////////////////////////////////////////////////////////////////////////////
SysLoadAdpcmImaEncoder
    ; Load the ADPCM encoder into P memory
    move    #>RSRC_ENC_ADPCMP_CODE,x0       ; Resource number
    move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
    move    #>(F_lc_u_e_ADPCM_Enc_P-F_lc_u_b_ADPCM_P)*3,y1            ; Size of the overlay area
    move    #>F_lc_u_b_ADPCM_P,y0    ; Address of the overlay area
    move    #TARGET_MEM_P,a1                ; Overlay area is in P memory
    jsr     SysLoadResource

    ; Load the ADPCM encoder into X memory
    move    #>RSRC_ENC_ADPCMX_CODE,x0       ; Resource number
    move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
    move    #>(F_lc_u_e_ADPCM_Enc_X-F_lc_u_b_ADPCM_X)*3,y1            ; Size of the overlay area
    move    #>F_lc_u_b_ADPCM_X,y0    ; Address of the overlay area

    move    #TARGET_MEM_X,a1                ; Overlay area is in X memory
    jsr     SysLoadResource

    ; Load the ADPCM encoder into Y memory
    move    #>RSRC_ENC_ADPCMY_CODE,x0       ; Resource number
    move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
    move    #>(F_lc_u_e_ADPCM_Enc_Y-F_lc_u_b_ADPCM_Y)*3,y1            ; Size of the overlay area
    move    #>F_lc_u_b_ADPCM_Y,y0    ; Address of the overlay area
    move    #TARGET_MEM_Y,a1                ; Overlay area is in Y memory
    jsr     SysLoadResource
    
    rts
           
SetEncoderType
		move	y:Fg_Wave_wFormatTag,B
        if  @def('MS_ADPCM')
		move	#>WAVE_FORMAT_ADPCM,x0
		cmp		x0,B	#Encode_MS_ADPCM_Block,r6
		jeq		_EndEncodeBlock
        endif

		if  @def('IMA_ADPCM')
		move	#>WAVE_FORMAT_IMA_ADPCM,x0
		cmp		x0,B		#Encode_IMA_ADPCM_Block,r6
		jeq		_EndEncodeBlock
        endif

        if  @def('G721_ADPCM')
		move	#>WAVE_FORMAT_G721_ADPCM,x0
		cmp		x0,B		#Encode_G721_ADPCM_Block,r6
		jeq		_EndEncodeBlock
        endif		

        if  @def('WINDOWS_PCM')
		move	#>WAVE_FORMAT_PCM,x0
		cmp		x0,B		#Encode_PCM_Block,r6
		jeq		_EndEncodeBlock
        endif		
		
		move	y:<Const_000001,A
		rts

_EndEncodeBlock
        clr     A    r6,y:AdpcmEncoderPointer
		rts

; Inputs:  
; Outputs: 
; Notes:   
GetInvBitRate
    move    y:Fg_Wave_dwAvgBytesPerSec,B
    ; Convert to bits per second.
    asl     B
    asl     B       #0,A
    asl     B       y:<Const_001000,A0	  	
	move    B,x0
 	and 	#$FE,CCR    	          ; clear any carry bit if present	
	rep	#$30
	div	x0,A
	add 	x0,a 
	move    A0,y:InvSampleRate      ; Retain for compatability.Rename to inv bit rate
    rts       

; Inputs:  a1 msg parameter new encoder state
; Outputs: 
; Notes:   
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

