;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000/2001
;
; STMP3 User-level Decoder Module (Can be overlayed)        
;///////////////////////////////////////////////////////////////////////////////
        opt     mex

        section Acelp_DecoderOverlay


;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////
;        nolist
    include "sysequ.inc"
    include "sysspeed.inc"
    include "hwequ.inc"
    include "dacequ.inc"
    include "msgequ.inc"
    include "decoderequ.inc"
    include "mixerequ.inc"
    include "sysmacro.asm"    

    include "stack.xref"
    include "stackmem.xref"
    include "sysmem.xref"
    include "AudibleAcelpdec.inc"
    include "sysresources.inc"
    include "const.xref"
    include "dacmem.xref"
    include "dacmemy.xref"
    include "dacdrv.xref"
    include "sysmisc.xref"
    include "sysirq.xref"
    include "bufdesc.xref"
    include "sysresource.xref"
    include "decodermem.xref"
    include "mixer.xref"
    include "systimers.xref"
    include "syscoder.xref"
    include "resource.inc"

                                              

;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////
        GLOBAL  AcelpDecoderModule
        GLOBAL  AcelpDecoderModuleInit
        Global  CurrentModule

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////
        EXTERN   EffectsInit
        EXTERN   Fg_CurrentSongName,AcelpDecoderFileHandle
        EXTERN   DecoderQueueDescriptor
        EXTERN   CurrentDirDevId
        EXTERN   AcelpDecoderModuleTimerDisable
        EXTERN   AcelpDecoderModuleIsr
        EXTERN   SysEnableTimer
        EXTERN   TotalSongCount
        EXTERN   SysSpeedClockFree
        EXTERN   SysSpeedIncrease
        EXTERN   FAcelp_BitRate
        EXTERN   SysPostMessage
        ;EXTERN    FILEClose
        ;EXTERN    FILEOpen
        ;EXTERN    FileEOF
        ;EXTERN    FileRead

        EXTERN    ModuleSetTimer

        EXTERN    MsgDecoderStateChange
        EXTERN    MsgMenuSongTimeChange
        EXTERN    MsgGeqSetCoef
        EXTERN    MsgGeqParam
		EXTERN	  MsgMenuSetSongPosition
		EXTERN	  MsgMenuUpdateChapterInfo

        EXTERN    MsgGeqSetCoef
        EXTERN    MsgGeqParam
        ; for parser
        EXTERN    TotalSongsPlayed
        EXTERN    MsgParserStopSong
        EXTERN    MsgParserNextSong
        EXTERN    MsgParserNextParam
        EXTERN    MsgParserPrevSong
        EXTERN    MsgParserPrevParam
        EXTERN    NEXT_SONG
        EXTERN    NEXT_SONG_PLAY_EOF
        EXTERN    NEXT_SONG_PLAY_BUTTON
        EXTERN    PREV_SONG
        EXTERN    PREV_SONG_PLAY
        EXTERN    PREV_SONG_RWND

        EXTERN    DecSetSongPosZero
        EXTERN    DecGetSongPos        
        EXTERN    SysSetupDecVectors
        EXTERN    NEXT_SONG_FFWD

        ; AB Mode References
        EXTERN    MsgParserDeviceEnum
        EXTERN    PlayMessage

        EXTERN    DecoderProcessPtr
        EXTERN    AcelpGetInvBitRate
        EXTERN    DacData1L,DacData1R
        EXTERN    SecondstoSkip

        EXTERN    Acelp_SamplingRate
        EXTERN    AcelpSampFreqTable
        EXTERN    Acelp_HeaderSize
        GLOBAL    FAcelpGetAuthorPtr,FAcelpGetTitlePtr


        EXTERN    FileSeek
        extern    UserScratchY
        extern    DacClearMemory

        extern    F_lc_u_e_ACELP_P,F_lc_u_b_ACELP_P        
        extern    F_lc_u_e_ACELP_X,F_lc_u_b_ACELP_X
        extern    F_lc_u_e_ACELP_Y,F_lc_u_b_ACELP_Y        


		extern AcelpDecoderSinkBufDesc
		extern g_AcelpDecSinkBufDescriptor
		extern AcelpDecoderFfwdRwndValue
		extern FAcelp_Reset
		extern AcelpDecoderEventValue
		extern SysLoadResource
		extern FAudible_AcelpOpenFile
		extern FAudible_AcelpCloseFile
		extern FAudible_AcelpFileSeek
		extern FAudible_GoToNextChapter
		extern FAudible_GoToPrevChapter
		extern FAudible_AcelpFilePos
		extern FAudible_AcelpGetFileSize
		extern FAudible_AcelpUpdateChapterNumber
		extern AcelpABPositionBHigh
		extern AcelpABPositionBLow
		extern AcelpABPositionAHigh
		extern AcelpABPositionALow
		extern Fg_bAudibleNeedFileSize

        
;///////////////////////////////////////////////////////////////////////////////
;  Macros
;///////////////////////////////////////////////////////////////////////////////


;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;   X Memory
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;   Y Memory
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;   Code
;///////////////////////////////////////////////////////////////////////////////
       org      p,"DECACELPMOD_DecoderOverlay_overlay_P":
        ; Resource number
        dc      RSRC_AUDIBLE_ACELPDEC_CODE

;*******************************************************************************
;  Function:
;             AcelpDecoderModuleInit
;  Description:
;          Decoder Initialization and interface module
;  Parameters passed: 
;          None
;  Registers Trashed: 
;             All
;  Return Values:
;             None
;*******************************************************************************
AcelpDecoderModuleInit

    ; save off stack
    move    #>stackSaved,r6              ; setup the return address
    move    #>UserScratchY,r5             ; pointer to the SP local save
    jmp     SaveSystemStack              ; save the hardware stack
stackSaved
        ; init the ModuleTable process pointer
    move    #AcelpDecoderModule,x0
    move    x0,y:DecoderProcessPtr

    
	;TBD load any decoder app here?

    ; Initialize Acelp Decoder.
    move    #g_AcelpDecSinkBufDescriptor,x0
    move    x0,y:AcelpDecoderSinkBufDesc      ; decoder sink buffer descriptor

    ; Init decoder status to stopped
    move    y:<Const_000000,x0
    bset    #Stop,x0
    move    x0,x:DecoderSR

    ; EffectsInit should be called before calling any DAC initialization to ensure the processing chain is loaded.
	jsr		EffectsInit

    ; Init the interrupt vectors
    move    #>DacDriverIsr,x0
    move    #>DacDriverIsr,x1
    move    #>AcelpDecoderModuleIsr,y0
    jsr     SysSetupDecVectors

    ; restore stack
_restoreStack
    move    #>UserScratchY,r5             ; pointer to the SP local save
    move    #>_stackRestored,r6           ; setup the return address
    jmp     RestoreSystemStack           ; restore the hardware stack
_stackRestored

_exitInit
    ; come back if we get a message
    move    #>EVENT_MESSAGE,x0

    rts

;*******************************************************************************
;  Function:
;             AcelpDecoderModule
;  Description:
;          Decoder interface module
;  Parameters passed: 
;          None
;  Registers Trashed: 
;             All
;  Return Values:
;             None
;*******************************************************************************
AcelpDecoderModule
    nop
    ; save off stack
    move    #_stackSaved,r6              ; setup the return address
    move    #UserScratchY,r5             ; pointer to the SP local save
    jmp     SaveSystemStack              ; save the hardware stack
_stackSaved

_DecoderCheckErrors
    ; TBD :: Do we want to have decoder report errors here?

_InitEventValue
    move    #>EVENT_MESSAGE,a1
    move    #>500,x0
    jsr     ModuleSetTimer
    move    a1,y:AcelpDecoderEventValue
_ProcessSysMessages
    ; switch on system messages
    move    #_queueProcessed,r6
    jmp     DecoderProcessQueue
_queueProcessed

_UpdateLcdMessage
    move    x:DecoderCSR,x0              ; copy contents of status register
    jset    #FileReadError,x0,_FileReadError ; error reading the file
    jclr    #Play,x0,_LCDdone            ; only update if playing (save MIPS)
    jset    #SongInfo,x0,_getSongCurrentTime
    bset    #SongInfo,x0                 ; prevents constant updating of same song info
    move    x0,x:DecoderCSR              ; SongInfo bit is cleared by opening of new song file
    move    #>MsgGeqSetCoef,r0
    move    y:Acelp_SamplingRate,x0      ; ###CHANGE
    move    x0,y:MsgGeqParam
    move    y:<Const_ffffff,m0
    jsr     SysPostMessage

_getSongTotalTime
    jsr     DecoderUpdateTotalTime       ; updates the display of the elapsed/remaining song time
_getSongCurrentTime
    jsr     DecoderUpdateCurrentTime     ; updates the display of the elapsed/remaining song time
_LCDdone

_checkABModeBoundaries
    move    x:DecoderCSR,x0
    jset    #Pause,x0,_CheckIfEndOfList               ; must check Pause bit in case "just Paused" now
    jsset   #ABMode_A,x0,CheckABBoundaries            ; if ABMode on, check the boundaries

_CheckIfEndOfList
    move    x:DecoderCSR,x0
    jset    #EndOfList,x0,_exitAcelpDecoderModule
_CheckIfEndOfSong
    move   x:DecoderCSR,x0
    jclr   #EndOfSong,x0,_exitAcelpDecoderModule

    ;-------------------------------------------------------------
    ; Done With Song  ::: TBD ::: Add PlayList and Directory features
    ;-------------------------------------------------------------
_AdvanceToNextSong
    bclr    #EndOfSong,x:DecoderCSR
    ; tell the menu module that the status has changed.
    move    #>MsgDecoderStateChange,r0
    move    y:<Const_ffffff,m0
    jsr     SysPostMessage
    move    #MsgMenuSongTimeChange,r0
    move    y:<Const_ffffff,m0
    jsr     SysPostMessage

    jsr     StopCurrentSong              ; includes FadeOut & disable DAC
    jsr     DecoderCloseSong

    ; tell the parser to advance to next song and Play
    move    #>NEXT_SONG_PLAY_EOF,x0      ; EOF && PLAY
    move    x0,y:MsgParserNextParam
    move    #>MsgParserNextSong,r0
    move    y:<Const_ffffff,m0
    jsr     SysPostMessage

    ; Free DCLK for other speed clients
    move    #>SPEED_CLIENT_AUDIBLE_ACELP,y0        
    jsr     SysSpeedClockFree  

    move    #>EVENT_MESSAGE,x0
    move    x0,y:AcelpDecoderEventValue        ; allow the parser to call it next (if songs are left)

_exitAcelpDecoderModule
    ; restore stack
    move    #>UserScratchY,r5             ; pointer to the SP local save
    move    #>_stackRestored,r6           ; setup the return address
    jmp     RestoreSystemStack           ; restore the hardware stack
_stackRestored

    move     y:AcelpDecoderEventValue,a1   ; return value for executive (EVENT_NONE or EVENT_MESSAGE)
    move     y:<Const_ffffff,m1          ; make sure that m1 is -1!
    rts

_FileReadError
    ; stop the DAC & close the file
    jsr     StopCurrentSong
    jsr     DecoderCloseSong
    bclr    #FileReadError,x:DecoderCSR
    bset    #FileReadError,x:DecoderSR 

    ; tell the menu module that the status has changed.
    move    #>MsgDecoderStateChange,r0
    move    y:<Const_ffffff,m0
    jsr     SysPostMessage

    ; Free DCLK for other speed clients
    move    #>SPEED_CLIENT_AUDIBLE_ACELP,y0        
    jsr     SysSpeedClockFree  

    move    #>EVENT_MESSAGE,x0
    move    x0,y:AcelpDecoderEventValue
    jmp     _exitAcelpDecoderModule

DecoderSetEventTimer
    ; if we want to return on the module timer OR message,
    ; this routine sets AcelpDecoderEventValue up.
    move    #>EVENT_MESSAGE,a1
    move    #>500,x0
    jsr     ModuleSetTimer
    move    a1,y:AcelpDecoderEventValue
    rts
;*******************************************************************************
; Decoder Module Functions
;*******************************************************************************
;-------------------------------------------------
StopCurrentSong
    ; Configure CSR
    bclr    #Pause,x:DecoderCSR
    bclr    #Ffwd,x:DecoderCSR
    bclr    #Rwnd,x:DecoderCSR
    bset    #Stop,x:DecoderCSR

_pollPlaybitStop
    move    x:DecoderCSR,x0
    jset    #Play,x0,_pollPlaybitStop ; wait for isr to enter "Stop" state
    ; isr will turn off Timer Module before returning

    ; Fade the Volume to Mute
    move    x:HW_MIXMASTERVR,x0
    jset    #HW_MIXVOLUMER_MUTE_BITPOS,x0,_muted
    move    #>msFadeOut,x0
    jsr     MixerMasterFadeOut      ; fade volume down to mute
_muted

    jsr     DacDriverDisablePort

    ; clear dac memory
    jsr        DacClearMemory
    ; Set the status register
    bset    #Stop,x:DecoderSR
    bclr    #Pause,x:DecoderSR
    bclr    #Play,x:DecoderSR

    rts

;-------------------------------------------------
DecoderUpdateLCD
    jsr     DecoderUpdateTotalTime       ; ---- / 0:00
    jsr     DecoderUpdateCurrentTime     ; 0:00 / ----
    rts
;-------------------------------------------------
DecoderUpdateTotalTime
; Updates the total time of the current song.
; Sends a message to the LCD to print it.
    move    y:<Const_ffffff,m4
    move    #SongByteLengthHigh,r4
    move    #SongByteLengthLow,r5
    move    m4,m5
    jsr     GetSongTime
    move    x1,y:SongTotalMinutes
    move    x0,y:SongTotalSeconds

    rts

;-------------------------------------------------
DecoderUpdateCurrentTime
; Updates the current elapsed time of the song and the remaining time in song
; Sends a message to the LCD to print them if they have changed.
    move    #SongByteTotalHigh,r4
    move    #SongByteTotalLow,r5
    jsr     GetSongTime
_checkIfUpdateNeeded
    move    y:SongCurrentMinutes,a
    cmp     x1,a
    jne     _update
    move    y:SongCurrentSeconds,a
    cmp     x0,a
    jne     _update                      ; if no change, do not update lcd
    rts
_update
    move    x1,y:SongCurrentMinutes
    move    x0,y:SongCurrentSeconds

    move    #MsgMenuSongTimeChange,r0
    move    y:<Const_ffffff,m0
    jsr     SysPostMessage
	
	jsr		FAudible_AcelpUpdateChapterNumber
	; tell the menu module to update chapter info
	move    #MsgMenuUpdateChapterInfo,r0
    move    y:<Const_ffffff,m0
    jsr     SysPostMessage
_getSongRemainTime
    jsr     GetSongRemainTime
_postMessageToLcd
    rts

;-------------------------------------------------
GetSongTime
; Inputs:  R4 = HighByte count ptr , M4 = linear
;          R5 =  LowByte count ptr , M5 = linear
; Output:  X1 = Song Minutes
;          X0 = Song Seconds
;
; You can call this to get total song length or for current song position.

    move    y:SongInvBitRate,y0          ; Current inv bit rate multiplier
    move    y:(r4),A                    ; high byte count
    move    y:(r5),A0                    ; low byte count

    move    #0,x1    
    move    y:Acelp_HeaderSize,x0
    sub     x,A
    move    A,x1
    move    A0,x0
    

    ; First, do the low byte count
    ; test sign bit before multiply
    jclr    #23,x0,_positive             ; if sign bit cleared, no work needed
    clr     a                            ;  else -- shift right, mpy, shift left (the LSB is lost but is insignificant)
    move    x0,a1
    asr     a                            ; shift right (lose LSB -- no big deal)
    move    a1,x0
    mpy     x0,y0,a
    asl     a                            ; shift left to undo adjustment
    jmp     _continue
_positive
    mpy     x0,y0,a                      ; no adjustment necessary
_continue

    ; Now, do the high byte count
    move    a1,b1                        ; set up for MAC
    clr     a
    move    b1,a0                        ; "shift right 24 bits"
    mac     x1,y0,a

    ; Undo the normalization (built into InvBitRate)
    do      #4,_end_do
    asr     a
    asr     a
_end_do
    asr     a                            ; A1:A0 = total seconds

    ; At this point, we have A1:A0 = total integer seconds
    ; 1. throw out the fractional seconds (already done)
    ; 2. A1 should always be 0, if A1 had only a $000001 in it, that would
    ;    represent nearly 17 million seconds, which will not fit on the media, nor 
    ;    does a song exist of that length!!!
    ; 3. Thus, we only care about A0 seconds!

    move    a0,x0                        ; x0 = total number of seconds
    move    #>Sec2Min,y0                 ; (1/60) (min/sec)
    mpy     x0,y0,a
    clr     b            a1,x1           ; OUTPUT # 1 : x1 = number of minutes

    ; 1st check for 60-second boundary
    move    #>Min2Sec,a                  ; "60"
    cmp     x0,a                         ; if num seconds = 60
    jne     _finish
    clr     a                            ; then, inc minutes, set seconds = 0
    move    a,x0
    move    x1,a0
    inc     a
    move    a0,x1
    jmp     _exit

_finish
    move    #>Min2Sec,y0                 ; 60 sec/min
    mpy     x1,y0,a
    asr     a
    move    a0,y0                        ; y0 = number of seconds in multiples of 60
    move    x0,a
    sub     y0,a                         ; A = number of seconds remaining after minutes
    move    a1,x0                        ; OUTPUT # 2 : x0 = number of seconds

    ; 2nd check for 60-second boundary
    ; (return to this later -- 1st check is redundant)
    move    #>Min2Sec,a                  ; "60"
    cmp     x0,a                         ; if num seconds = 60
    jne     _exit
    clr     a                            ; then, inc minutes, set seconds = 0
    move    a,x0
    move    x1,a0
    inc     a
    move    a0,x1

_exit
    rts

;-------------------------------------------------
GetSongRemainTime
    move    y:SongTotalMinutes,a
    move    y:SongCurrentMinutes,x0
    sub     x0,a                         ; A = minutes left (if borrow not necessary)
    move    y:SongTotalSeconds,b
    move    y:SongCurrentSeconds,x0
    sub     x0,b                         ; B = seconds left (if non-negative)
    jge     _done
    move    y:<Const_000001,x0                       ; borrow 1 minute to fix negative seconds
    sub     x0,a
    move    #>60,x0
    add     x0,b                         ; adjust the seconds remaining
_done
    move    a1,y:SongRemainMinutes       ; store result (minutes)
    move    b1,y:SongRemainSeconds       ; store result (seconds)
    rts

;-------------------------------------------------
;*******************************************************************************
;  Function:
;             DecoderProcessQueue
;  Description:
;          Deque messages passed to the Decoder
;             r0=Descriptor for this modules message queue (in y memory)
;             m0=linear
;  Parameters passed: 
;          None
;  Registers Trashed: 
;             All
;  Return Values:
;             None
;*******************************************************************************
DecoderProcessQueue
        push    r6                        ; needed for return

        move    #DecoderQueueDescriptor,r0
        move    y:<Const_ffffff,m0

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
        push    a1                      ; Head pointer
        push    r0                      ; Pointer to queue descriptor
        push    r1                      ; Tail pointer
        push    m1                      ; Queue modulo

        ; Put the message ID into b
        move    #>MSG_ID,n1
        nop
        move    y:(r1+n1),b

        ; Verify that the message ID is valid by checking message range
        move    #>DECODER_FIRST_MSG_ID,x0
        cmp     x0,b
        jlt     _InvalidMessageID
        move    #>DECODER_LAST_MSG_ID,x0
        cmp     x0,b
        jgt     _InvalidMessageID

        ; Check for case of no songs to play
        move    y:TotalSongCount,a
        tst     a
        jne     _songsPresent
        jsr     DecABModeDisable        ; reset any AB mode settings
        move    #>EVENT_MESSAGE,x0      ; no songs avail -- only call on button push
        move    x0,y:AcelpDecoderEventValue
        jmp     _NextMessage            ; ignore messages - no songs!

_songsPresent
        ; Point r1 to the first argument of the message
        move    (r1)+
        move    (r1)+

        ; Clear the upper byte of the message ID
        move    #$00FFFF,x0
        and     x0,b

        ; Jump to the message handler
        move    #_JumpTable,r2
        move    y:<Const_ffffff,m2
        move    b1,n2
        nop
        ; HACK, maybe not a good idea since this is a two-cycle instr, not one
        movem   p:(r2+n2),r3
        nop
        jmp     (r3)
        
;-------------------------ResetDecoder Function-------------------------
_ResetDecoder
        bclr    #DecoderIgnoreMessages,x:DecoderSR
        ; _ResetDecoder may execute often so try not to be a MSG sender here.
        
        ;NEED TO ADD MORE "RESET" stuff so DECODER_RESET really does do what its supposed to
        jmp     _NextMessage
        

;-------------------------SetPlaylist Function-------------------------

_dirPlaylist
        ; Get the arguments
        pop     r1
        nop
        move    y:(r1)+,a2              ; device (card) number
        move    y:(r1)+,a1              ; memory space and ptr to path

        ; store the playlist information
        ;move    a2,y:CurrentPListDevId  ; store the directory device id
        ;move    a1,y:CurrentPListPathPtr ; store the memory space and ptr to path

        ; jsr   FileOpen
        ; get the Kth song from playlist file--get ptr and store or store path
        ; close the playlist file
        ; update K for next playlist open

        jmp     _NextMessage

;-------------------------SetDir Function-------------------------
_SetDir
        ; No Longer Functional
        ; Not currently supporting browsing -- we just play through all songs
        ; ----------------------------------------------
        ; THIS MESSAGE IS NOT SUPPORTED!!!!!!!!!!!!!!!!!
        ; ----------------------------------------------
        jmp     _NextMessage

;-------------------------Play/Pause Function-------------------------
_Play   ; Toggles between Play and Pause

        jsr        SysAllLinear            ; Always linear
        move    #MsgMenuSongTimeChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        bclr  #ABQuiet,x:DecoderCSR
        ; Configure CSR
        move    x:DecoderCSR,x0
        bclr    #Stop,x0
        bclr    #Ffwd,x0
        bclr    #Rwnd,x0
        bclr    #EndOfList,x0
        move    x0,x:DecoderCSR

_checkPlayBit
;        jset    #SyncWait,x0,_GoToPause
        jclr    #Play,x0,_beginPlay     ; if playing, switch to Pause
_GoToPause
        bset    #Pause,x:DecoderCSR     ; msg to isr to "Pause"
;        bclr    #SyncWait,x:DecoderCSR
        ; we need to be absolutely sure that the timer module will always be enabled before this polling mechanism!
_pollPlaybit
        move    x:DecoderCSR,x0
        jset    #Play,x0,_pollPlaybit   ; wait for isr to enter "Pause" state

_pollPausebit
        move    x:DecoderSR,x0
        jclr    #Pause,x0,_pollPausebit

        ; Fade the Volume to Mute
        move    x:HW_MIXMASTERVR,x0
        jset    #HW_MIXVOLUMER_MUTE_BITPOS,x0,_muted
        move    #>msFadeOut,x0
        jsr     MixerMasterFadeOut      ; fade volume down to mute
_muted

        jsr     DacDriverDisablePort    ; turn off the DAC

		; Free DCLK for other speed clients
		move    #>SPEED_CLIENT_AUDIBLE_ACELP,y0        
		jsr     SysSpeedClockFree  

		; 2/10/2005 mmiu - Added handle saving file position on Pause
		; tell the menu module to update file psition in *.POS file
        ;move    #MsgMenuSetSongPosition,r0
        ;move    y:<Const_ffffff,m0
        ;jsr     SysPostMessage

        ; tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AcelpDecoderEventValue

        jmp     _NextMessage

_beginPlay
		jsr     SysAllLinear
        move    x:DecoderCSR,x0
        bclr    #Pause,x0
        bclr    #SongInfo,x0            ; necessary to notify decoder module to update song info (new song)

		move    x0,x:DecoderCSR
		
		; To avoid audible parsing and file seek latencies bump up the clock all the way upto MAX 
;        move    #>SPEED_MAX,x1          ; Change to SPEED_MP3
        move    #>SPEED_PEAK,x1          ; Change to SPEED_MP3
        move    #>SPEED_CLIENT_AUDIBLE_ACELP,y0
        jsr     SysSpeedIncrease

        jsr     AcelpDecoderOpenSong
		jcs		_StopPlay				; if error opening file, stop

	    ; EffectsInit should be called before calling any DAC initialization to ensure the processing chain is loaded.
        move    #>g_AcelpDecSinkBufDescriptor,A
        move    #>TARGET_MEM_Y,B
        move    y:<Const_000000,x0                            ; Always mono output
        jsr        EffectsInit
        
        ; Init dac and decoder for new song
        jsr     DacDriverInit
        jsr     DacDriverEnablePort   ; set up the DAC control registers, enable the port

        jsr     FAcelp_Reset
        tst     A
        jne     _StopPlay

        clr     A            ; Corresponds to the sampling rate index 0 = 8000 Hz
        move    y:Acelp_SamplingRate,x0
        move    #AcelpSampFreqTable,r0
        do  #9,_SamplingRateSearch            
        move x:(r0)+,B
        cmp x0,B    
        jne _ContinueSRsearch            
        enddo
        jmp _SetSR
_ContinueSRsearch
        inc A
_SamplingRateSearch                                
        nop            
_SetSR        
        move    A0,A
    	jsr		DacSetSamplingRate
        
        jsr    AcelpGetInvBitRate        ; Get the Inverse of Bit Rate.

        ;jsr     DecoderUpdateLCD        ; song time and number

        move    y:SongByteTotalHigh,A
        move    y:SongByteTotalLow,A0
        jsr     FAudible_AcelpFileSeek

;_ContinueSkipRandomCheck
		; Depending on sampling rate set audible acelp clock 
        move    #>SPEED_CLIENT_AUDIBLE_ACELP,y0
        jsr     SysSpeedClockFree

		move y:Acelp_SamplingRate,A
        move #>8000,B
        cmp  B,A
        jeq  _SpeedSR8KHz
_SpeedSR16KHz
	    move    #>SPEED_AUDIBLE_ACELP_SR16KHz,x1
		jmp		_SpeedSelected
_SpeedSR8KHz
		move    #>SPEED_AUDIBLE_ACELP_SR8KHz,x1
_SpeedSelected        
		move    #>SPEED_CLIENT_AUDIBLE_ACELP,y0
        jsr     SysSpeedIncrease
        
        ; Enable the DAC interrupt
        bset    #HW_DACCSR_TXIEN_BITPOS,x:HW_DACCSR

        ; Turn Timer Module ON
        move    #>DecoderTimerNum,x0
        move    #_TimerEnabled_2,r0
        jmp     SysEnableTimer
_TimerEnabled_2
        move    #>msFadeIn,x0
        jsr     MixerMasterFadeIn            ; fade volume in to previous level

        ; tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage
        jsr     DecoderSetEventTimer         ; set up DecoderEventValue for module timer
        jmp     _NextMessage
        
_StopPlay
        jsr     StopCurrentSong              ; includes FadeOut & disable DAC
        jsr     DecoderCloseSong

        ; tell the parser to advance to next song and Play
        move    #>NEXT_SONG_PLAY_EOF,x0      ; EOF && PLAY
        move    x0,y:MsgParserNextParam
        move    #>MsgParserNextSong,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        ; Free DCLK for other speed clients
        move    #>SPEED_CLIENT_AUDIBLE_ACELP,y0
        jsr     SysSpeedClockFree

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AcelpDecoderEventValue        ; allow the parser to call it next (if songs are left)

        jmp     _NextMessage
        

;-------------------------Stop Function-------------------------
_Stop
        jsr     DecABModeDisable             ; reset any AB mode settings
        jsr     StopCurrentSong
        jsr     DecoderCloseSong
		;jsr     DecClearSongTime

        ; tell the parser module to reset to first song
        move    #>MsgParserStopSong,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        ; tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage
        move    #MsgMenuSongTimeChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        ; Free DCLK for other speed clients
        move    #>SPEED_CLIENT_AUDIBLE_ACELP,y0        
        jsr     SysSpeedClockFree  

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AcelpDecoderEventValue

        jmp     _NextMessage

;-------------------------Ffwd Function-------------------------
_Ffwd
        bset #ABQuiet,x:DecoderCSR
        ; Open File If Necessary
        move    x:DecoderCSR,x0
        jset    #FileIsOpen,x0,_FileOpenedForFfwd

		; mmiu - If file is not open, do not allow user to enter the AA file and ffwd through content.
		; It's more important to preserve file position.
;		jmp		_Stop
_goToNextSong
        ; Configure CSR
        bclr    #Ffwd,x:DecoderCSR
        bclr    #Rwnd,x:DecoderCSR
        jsr     DecABModeDisable             ; reset any AB mode settings

        ; tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        ; Free DCLK for other speed clients
        move    #>SPEED_CLIENT_AUDIBLE_ACELP,y0        
        jsr     SysSpeedClockFree  

        ; Tell the PlayerStateMachine to advance to next song and Play
        move    #>NEXT_SONG_FFWD,x0 ; Button && PLAY
        move    x0,y:MsgParserNextParam
        move    #>MsgParserNextSong,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        ;ignore any messages until we're reset
        bset    #DecoderIgnoreMessages,x:DecoderSR

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AcelpDecoderEventValue
        jmp     _NextMessage


        clr     a
        move    y:AcelpDecoderFileHandle,a2
		move	#>1,x0                       ; Set for FF
		move	x0,x:Fg_bAudibleNeedFileSize
        jsr     AcelpDecoderOpenSong         ; open the file
        ; tell the menu module that the status has changed.
        ; this is necessary to update song name when crossing a boundary
        move    #>MsgDecoderStateChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage
_FileOpenedForFfwd
        
        ; Very bad, if the ISR also wants to mess with ACELP variables
        ; Shut down the ISR timer for timer being.
        move    #>DecoderTimerNum,x0
        move    #_TimerDisabled_1,r0
        jmp     SysDisableTimer
_TimerDisabled_1

        ; Great. Now, I have exclusive access.
		jsr     SysAllLinear
        ; EffectsInit should be called before calling any DAC initialization to ensure the processing chain is loaded.
        move    #>g_AcelpDecSinkBufDescriptor,A
        move    #>TARGET_MEM_Y,B
        move    y:<Const_000000,x0                            ; Always mono output
        jsr        EffectsInit

        jsr     DacDriverInit
        jsr     DacDriverEnablePort   ; set up the DAC control registers, enable the port
        jsr     FAcelp_Reset
        tst     A
        jne     _Stop
        jsr     AcelpGetSongInfo

        ; FileSeek forward
        jsr     DecoderComputeSeek
        move    y:SongByteTotalHigh,a1
        move    y:SongByteTotalLow,a0
        add     A,B
        move    B0,y:SongByteTotalLow
        move    B1,y:SongByteTotalHigh

		; Get bytes corresponding to (total song length(seconds) - 10sec)
		move    #>10,x0
        move    y:FAcelp_BitRate,A
        move    A,y0
        mpy     x0,y0,B         
        asr     B               
        asr     B               
        asr     B               
        asr     B          
		clr     A
        move    y:SongByteLengthHigh,A1
        move    y:SongByteLengthLow,A0
		sub		B,A							; A --> Bytes corresponding to ( SongByteLength(in bytes) - 10seconds(in bytes) )
        move    y:SongByteTotalHigh,B1
        move    y:SongByteTotalLow,B0		; B --> SongByteTotal 
		cmp     A,B							; if num bytes to skip < num bytes left, ffwd
        jlt     _Cont_Fwd
;_goToNextSong
        ; Configure CSR
;        bclr    #Ffwd,x:DecoderCSR
;       bclr    #Rwnd,x:DecoderCSR
;        jsr     DecABModeDisable             ; reset any AB mode settings

;        jsr     StopCurrentSong
;        jsr     DecoderCloseSong

;        ; tell the menu module that the status has changed.
;        move    #>MsgDecoderStateChange,r0
;        move    y:<Const_ffffff,m0
;        jsr     SysPostMessage

;        ; tell the parser to advance to next song and Play
;       move    #>NEXT_SONG_FFWD,x0 ; Button && PLAY
;       move    x0,y:MsgParserNextParam
;       move    #>MsgParserNextSong,r0
;       move    y:<Const_ffffff,m0
;        jsr     SysPostMessage

;        move    #>EVENT_MESSAGE,x0
;        move    x0,y:AcelpDecoderEventValue
		; if SongByteTotal >= (SongByteLength(in bytes) - 10seconds(in bytes))
		move    A0,y:SongByteTotalLow
        move    A1,y:SongByteTotalHigh
        jsr     AcelpGetInvBitRate				; Get the Inverse of Bit Rate.
        jsr     DecoderUpdateLCD				; song time and number
        jmp     _NextMessage

_Cont_Fwd
        jsr     AcelpGetInvBitRate      ; Get the Inverse of Bit Rate.
        jsr     DecoderUpdateLCD        ; song time and number

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AcelpDecoderEventValue

        jmp     _NextMessage

    ;-------------------------Rwnd Function-------------------------
_Rwnd
        move    x:DecoderCSR,x0
        jset    #FileIsOpen,x0,_RwndCheckFilePos

		; mmiu - If file is not open, do not allow user to enter the AA file and ffwd through content.
		; It's more important to preserve file position.
_goToPrevSong
        ; Configure CSR
        bclr    #Ffwd,x:DecoderCSR
        bclr    #Rwnd,x:DecoderCSR
        jsr     DecABModeDisable             ; reset any AB mode settings

        ; tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        ; Free DCLK for other speed clients
        move    #>SPEED_CLIENT_AUDIBLE_ACELP,y0        
        jsr     SysSpeedClockFree  

        ; Tell the PlayerStateMachine to go to previous song and Play
        move    #>PREV_SONG_RWND,x0 ; Button && PLAY
        move    x0,y:MsgParserPrevParam
        move    #>MsgParserPrevSong,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        ;ignore any messages until we're reset
        bset    #DecoderIgnoreMessages,x:DecoderSR

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AcelpDecoderEventValue
        jmp     _NextMessage

_RwndCheckFilePos
        jsr     DecoderComputeSeek
        ; check if too close to beginning of file
        move    y:SongByteTotalHigh,a
        move    y:SongByteTotalLow,a0
        sub     B,A           b0,y:AcelpDecoderFfwdRwndValue
		move    A1,y:SongByteTotalHigh
		move    A0,y:SongByteTotalLow
		jgt	_rewind
;_goToPrevSong
;        ; Configure CSR
;        bclr    #Ffwd,x:DecoderCSR
;        bclr    #Rwnd,x:DecoderCSR
;        jsr     DecABModeDisable             ; reset any AB mode settings

;        jsr     StopCurrentSong
;        jsr     DecoderCloseSong

;        ; tell the menu module that the status has changed.
;        move    #>MsgDecoderStateChange,r0
;        move    y:<Const_ffffff,m0
;        jsr     SysPostMessage
;;
;;        ; tell the parser to backup a song and send RWND message when done
;        move    #>PREV_SONG_RWND,x0          ; get previous song then rewind from end
;        move    x0,y:MsgParserPrevParam
;        move    #>MsgParserPrevSong,r0
;        move    y:<Const_ffffff,m0
;        jsr     SysPostMessage

;        move    #>EVENT_MESSAGE,x0
;        move    x0,y:AcelpDecoderEventValue

		 clr	 A           
		 move    A1,y:SongByteTotalHigh
		 move    A0,y:SongByteTotalLow

		 jsr     AcelpGetInvBitRate        ; Get the Inverse of Bit Rate.
         jsr     DecoderUpdateLCD        ; song time and number
         jmp     _NextMessage

_rewind

        jsr     SysAllLinear
        move    x:DecoderCSR,x0
        jset    #FileIsOpen,x0,_FileOpenedForRwnd

		move	#>2,x0                       ; set for Rewind
		move	x0,x:Fg_bAudibleNeedFileSize
        jsr     AcelpDecoderOpenSong         ; open the file
        ; tell the menu module that the status has changed.
        ; this is necessary to update song name when crossing a boundary
        move    #>MsgDecoderStateChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage
_FileOpenedForRwnd

        ; Very bad, if the ISR also wants to mess with ACELP variables
        ; Shut down the ISR timer for timer being.
        move    #>DecoderTimerNum,x0
        move    #_TimerDisabled_2,r0
        jmp     SysDisableTimer
_TimerDisabled_2
		jsr     SysAllLinear
        
        ; EffectsInit should be called before calling any DAC initialization to ensure the processing chain is loaded.
        move    #>g_AcelpDecSinkBufDescriptor,A
        move    #>TARGET_MEM_Y,B
        move    y:<Const_000000,x0                            ; Always mono output
        jsr        EffectsInit

        jsr     DacDriverInit
        jsr     DacDriverEnablePort   ; set up the DAC control registers, enable the port
        jsr    FAcelp_Reset
        tst        A
        jne        _Stop
        jsr     AcelpGetSongInfo

        clr     a
        move    a,x1
        move    y:SongByteTotalHigh,a1  ; high bytes
        move    y:SongByteTotalLow,a0   ; low bytes
        move    y:AcelpDecoderFfwdRwndValue,x0
        sub     x,a
        move    A0,y:SongByteTotalLow
        move    A1,y:SongByteTotalHigh
        
        jsr    AcelpGetInvBitRate        ; Get the Inverse of Bit Rate.
        jsr     DecoderUpdateLCD        ; song time and number

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AcelpDecoderEventValue

        jmp     _NextMessage

    ;-------------------------NextSong Function-------------------------
_NextSong
        ; Configure CSR
        bclr    #Ffwd,x:DecoderCSR
        bclr    #Rwnd,x:DecoderCSR
        jsr     DecABModeDisable             ; reset any AB mode settings
        move    x:DecoderCSR,x0
        jset    #Play,x0,_nextStop
        jset    #Pause,x0,_nextStop
        jsr     DecoderCloseSong

        ; tell the parser to advance to next song
        move    #>NEXT_SONG,x0          ; Button && NoPlay
        move    x0,y:MsgParserNextParam
        move    #>MsgParserNextSong,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AcelpDecoderEventValue
        jmp     _NextMessage
_nextStop
         jsr     StopCurrentSong
         jsr     DecoderCloseSong

        ; tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        ; tell the parser to advance to next song and Play
        move    #>NEXT_SONG_PLAY_BUTTON,x0 ; Button && PLAY
        move    x0,y:MsgParserNextParam
        move    #>MsgParserNextSong,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AcelpDecoderEventValue
        jmp     _NextMessage

    ;-------------------------PrevSong Function-------------------------
_PrevSong

        ; Configure CSR
        bclr    #Ffwd,x:DecoderCSR
        bclr    #Rwnd,x:DecoderCSR
        jsr     DecABModeDisable             ; reset any AB mode settings

        move    x:DecoderCSR,x0
        jset    #Play,x0,_prevStop
        jset    #Pause,x0,_prevStop

        ; Song is stopped
        jsr     DecoderCloseSong

        ; tell the parser to advance to next song and Play
        move    #>PREV_SONG,x0          ; PrevSong && Stop
        move    x0,y:MsgParserPrevParam
        move    #>MsgParserPrevSong,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AcelpDecoderEventValue
        jmp     _NextMessage

_prevStop
        ; song was playing
        jsr     StopCurrentSong
        jsr     DecoderCloseSong

        ; tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        ; tell the parser to advance to next song and Play
        move    #>PREV_SONG_PLAY,x0     ; PrevSong && Play
        move    x0,y:MsgParserPrevParam
        move    #>MsgParserPrevSong,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AcelpDecoderEventValue
        jmp     _NextMessage

    ;-------------------------TimeMode Function-------------------------
_TimeMode       ; switches between elapsed and remaining song time
        move    x:DecoderCSR,x0
        bchg    #TimeMode,x0
        move    x0,x:DecoderCSR
        jmp     _NextMessage

    ;-------------------------ABMode Function---------------------------
_ABMode
        move    x:DecoderCSR,x0
_checkA
        jset    #ABMode_A,x0,_checkB
        bset    #ABMode_A,x:DecoderCSR  ; first msg is for "A" location
        jsr     DecGetSongPos           ; returns byte position in A1:A0
        move    a1,y:AcelpABPositionAHigh    ; save high byte
        move    a0,y:AcelpABPositionALow     ; save low byte
        jmp     _exitABMode
_checkB
        jset    #ABMode_B,x0,_disableABMode
        bset    #ABMode_B,x:DecoderCSR  ; second msg is for "B" location
        jsr     DecGetSongPos           ; returns byte position in A1:A0
        move    a1,y:AcelpABPositionBHigh    ; save high byte
        move    a0,y:AcelpABPositionBLow     ; save low byte

        ; make sure A < B
        clr     b
        move    y:AcelpABPositionAHigh,b1    ; B1 = AHigh
        move    y:AcelpABPositionALow,b0     ; B0 = ALow
        cmp     a,b                     ; compare Bmode to Amode
        jlt     _exitABMode             ; Bmode > Amode
_disableABMode
        jsr     DecABModeDisable        ; clear bits, reset byte positions to 0.

_exitABMode
        move    x:DecoderCSR,x0
        jclr    #Stop,x0,_next
        move    #>EVENT_MESSAGE,x0
        move    x0,y:AcelpDecoderEventValue
_next
        jmp     _NextMessage

    ;-------------------------SetEQ Function----------------------------
_SetEQ
    ; No Eq settings for ACELP.
        jmp     _NextMessage

_DisplaySongInfo

        push    r1
        push    m1

        jsr     AcelpDecoderOpenSong    

        ; This is going to do lots of things, but we are interested 
        ; in song information only.
		jsr     SysAllLinear
        jsr     FAcelp_Reset           ; Initializes the Decoder to get decoder information. 
        jsr    AcelpGetSongInfo
        jsr    AcelpGetInvBitRate      ; Get the Inverse of Bit Rate.
        jsr    DecoderUpdateLCD        ; song time and number

        jsr    DecoderCloseSong    

        pop    m1
        pop    r1

        move    #>EVENT_MESSAGE,x0                ; Don't wait on timer events.
        move    x0,y:AcelpDecoderEventValue

        jmp    _NextMessage

_NextChapter
        bset #ABQuiet,x:DecoderCSR
        jsr     DecABModeDisable             ; reset any AB mode settings
        ; Open File If Necessary
        move    x:DecoderCSR,x0
        jset    #FileIsOpen,x0,_FileOpenedForNextChapter
        clr     a
        move    y:AcelpDecoderFileHandle,a2
        jsr     AcelpDecoderOpenSong         ; open the file
        ; tell the menu module that the status has changed.
        ; this is necessary to update song name when crossing a boundary
        move    #>MsgDecoderStateChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage
_FileOpenedForNextChapter
        
        ; Very bad, if the ISR also wants to mess with ACELP variables
        ; Shut down the ISR timer for timer being.
        move    #>DecoderTimerNum,x0
        move    #_TimerDisabled_3,r0
        jmp     SysDisableTimer
_TimerDisabled_3

        ; Great. Now, I have exclusive access.
		jsr     SysAllLinear
        ; EffectsInit should be called before calling any DAC initialization to ensure the processing chain is loaded.
        move    #>g_AcelpDecSinkBufDescriptor,A
        move    #>TARGET_MEM_Y,B
        move    y:<Const_000000,x0                            ; Always mono output
        jsr     EffectsInit

        jsr     DacDriverInit
        jsr     DacDriverEnablePort   ; set up the DAC control registers, enable the port
        jsr     FAcelp_Reset
        tst     A
        jne     _Stop
        jsr     AcelpGetSongInfo

        jsr		FAudible_GoToNextChapter
		tst		A
        jeq     _Cont_NextChapter

        ; Configure CSR
        bclr    #Ffwd,x:DecoderCSR
        bclr    #Rwnd,x:DecoderCSR
        jsr     DecABModeDisable             ; reset any AB mode settings

        jsr     StopCurrentSong
        jsr     DecoderCloseSong

        ; tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

	    ; tell the parser to advance to next song and Play
	    move    #>NEXT_SONG_PLAY_EOF,x0      ; EOF && PLAY
	    move    x0,y:MsgParserNextParam
	    move    #>MsgParserNextSong,r0
	    move    y:<Const_ffffff,m0
	    jsr     SysPostMessage

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AcelpDecoderEventValue
        jmp     _NextMessage

_Cont_NextChapter
        jsr     AcelpGetInvBitRate        ; Get the Inverse of Bit Rate.
        jsr     DecoderUpdateLCD        ; song time and number
		jsr 	FAudible_AcelpFilePos
        move    A,y:SongByteTotalHigh
        move    A0,y:SongByteTotalLow

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AcelpDecoderEventValue

		jsr		FAudible_AcelpUpdateChapterNumber
  	    ; tell the menu module to update chapter info
        move    #MsgMenuUpdateChapterInfo,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        jmp     _NextMessage
;-----------------------------------------------------------------------------------
_PrevChapter
        bset #ABQuiet,x:DecoderCSR
        jsr     DecABModeDisable             ; reset any AB mode settings
        ; Open File If Necessary
        move    x:DecoderCSR,x0
        jset    #FileIsOpen,x0,_FileOpenedForPrevChapter
        clr     a
        move    y:AcelpDecoderFileHandle,a2
        jsr     AcelpDecoderOpenSong         ; open the file
        ; tell the menu module that the status has changed.
        ; this is necessary to update song name when crossing a boundary
        move    #>MsgDecoderStateChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage
_FileOpenedForPrevChapter
        
        ; Very bad, if the ISR also wants to mess with ACELP variables
        ; Shut down the ISR timer for timer being.
        move    #>DecoderTimerNum,x0
        move    #_TimerDisabled_4,r0
        jmp     SysDisableTimer
_TimerDisabled_4

        ; Great. Now, I have exclusive access.
		jsr     SysAllLinear
        
        ; EffectsInit should be called before calling any DAC initialization to ensure the processing chain is loaded.
        move    #>g_AcelpDecSinkBufDescriptor,A
        move    #>TARGET_MEM_Y,B
        move    y:<Const_000000,x0                            ; Always mono output
        jsr     EffectsInit

        jsr     DacDriverInit
        jsr     DacDriverEnablePort   ; set up the DAC control registers, enable the port
        jsr     FAcelp_Reset
        tst     A
        jne     _Stop
        jsr     AcelpGetSongInfo

        jsr		FAudible_GoToPrevChapter
		tst		A
        jeq     _Cont_PrevChapter

        ; Configure CSR
        bclr    #Ffwd,x:DecoderCSR
        bclr    #Rwnd,x:DecoderCSR
        jsr     DecABModeDisable             ; reset any AB mode settings

        jsr     StopCurrentSong
        jsr     DecoderCloseSong

        ; tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

	    ; tell the parser to advance to next song and Play
	    move    #>PREV_SONG_PLAY,x0      ; EOF && PLAY
	    move    x0,y:MsgParserNextParam
	    move    #>MsgParserNextSong,r0
	    move    y:<Const_ffffff,m0
	    jsr     SysPostMessage

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AcelpDecoderEventValue
        jmp     _NextMessage

_Cont_PrevChapter
        jsr     AcelpGetInvBitRate        ; Get the Inverse of Bit Rate.
        jsr     DecoderUpdateLCD        ; song time and number
        jsr     FAudible_AcelpFilePos
        move    A,y:SongByteTotalHigh
        move    A0,y:SongByteTotalLow

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AcelpDecoderEventValue

		jsr		FAudible_AcelpUpdateChapterNumber
		; tell the menu module to update chapter info
        move    #MsgMenuUpdateChapterInfo,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage


        jmp     _NextMessage
		
    ;-------------------------Invalid Message---------------------------

_InvalidMessageID

        error

_NextMessage
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

_CheckDone
        ; Check if head pointer equals the tail pointer
        cmp     a,b
        jne     _ProcessMessage

        ; Update the buffer descriptor with the new tail pointer
        move    #>BD_TailPtr,n0
        nop
        move    b1,y:(r0+n0)

        pop     r6                        ; return address
        nop
        jmp     (r6)                      ; return to AcelpDecoderModule routine

_JumpTable
        dc      _ResetDecoder
        dc      _SetDir
        dc      _Play
        dc      _Stop
        dc      _Ffwd
        dc      _Rwnd
        dc      _NextSong
        dc      _PrevSong
        dc      _TimeMode
        dc      _ABMode
        dc        _SetEQ
        dc      _DisplaySongInfo
		dc		_NextChapter
		dc		_PrevChapter

; Note: It is important that the entries in the jump table be in the
; same order as the Decoder message ID's which are defined in msgequ.inc

;*******************************************************************************
; DECODER-SPECIFIC FILE FUNCTIONS
; These functions are used by the decoder to perform file system actions.
;*******************************************************************************
LoadAudibleAcelpDecoderApp
        ; Load the Audible+Acelpdecoder into P memory
        move    #>RSRC_AUDIBLE_ACELP_CODE_P,x0             ; Resource number
        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
        move    #>(F_lc_u_e_ACELP_P-F_lc_u_b_ACELP_P)*3,y1 ; overlay area sz
        move    #>F_lc_u_b_ACELP_P,y0      ; Address of the overlay area
        move    #TARGET_MEM_P,a1                ; Overlay area is in P memory
        jsr     SysLoadResource

        ; Load the Audible+Acelp decoder into X memory
        move    #>RSRC_AUDIBLE_ACELP_DATA_X,x0             ; Resource number
        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data                  
        move    #>(F_lc_u_e_ACELP_X-F_lc_u_b_ACELP_X)*3,y1 ; overlay area sz
        move    #>F_lc_u_b_ACELP_X,y0      ; Address of overlay area
        move    #TARGET_MEM_X,a1                ; Overlay area is in X memory
        jsr     SysLoadResource

        ; Load the MP3 decoder into Y memory
        move    #>RSRC_AUDIBLE_ACELP_DATA_Y,x0             ; Resource number
        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
        move    #>(F_lc_u_e_ACELP_Y-F_lc_u_b_ACELP_Y)*3,y1 ; overlay area sz
		move    #>F_lc_u_b_ACELP_Y,y0      ;  Address of the overlay area
        move    #TARGET_MEM_Y,a1                ; Overlay area is in Y memory
        jsr     SysLoadResource

	rts

;-----------------------------------------------------------------
; AcelpDecoderOpenSong
;
;-----------------------------------------------------------------
AcelpDecoderOpenSong

        ; if the song is already open then just exit
        move    x:DecoderCSR,x0
        jclr    #FileIsOpen,x0,_notYetOpen
        rts
_notYetOpen
        jsr     SysAllLinear
        clr     a
        move    a,y:CurrentSongBad           ; need to mark current song not bad 
		jsr		LoadAudibleAcelpDecoderApp
        move    #Fg_CurrentSongName,r0          ; address of song name
		jsr		FAudible_AcelpOpenFile
		tst		A
		jne		_ErrorOpeningFile

        move    x:DecoderCSR,x0
        bset    #FileIsOpen,x0
        move    x0,x:DecoderCSR              ; keep track of an open song

_SuccessOpeningFile
        jsr     FAudible_AcelpGetFileSize
        move	A,y:SongByteLengthHigh       ; Song length in bytes
        move	A0,y:SongByteLengthLow

        and     #>$FE,ccr
		rts

_ErrorOpeningFile
		or      #$01,ccr
		rts


;-----------------------------------------------------------------
; DecoderCloseSong
; This function will use the AcelpDecoderFileHandle to close the file.
;-----------------------------------------------------------------
DecoderCloseSong
        move    x:DecoderCSR,x0
        jclr    #FileIsOpen,x0,_exit         ; do not call FileClose if song file is not open
        ;-------------------------------------------------------------
        ; Close the Song (FILEClose)
        ;-------------------------------------------------------------
        ; Inputs:   A2 = File handle
        ; Output:
        ;           A1 = File handle (if successful)
        ;           A1 = Error code  (if failed)
        ;-------------------------------------------------------------
		jsr		FAudible_AcelpCloseFile
		tst		A
		jeq		_SuccessClosingFile
		error
_SuccessClosingFile
        move    x:DecoderCSR,x0
        bclr    #FileIsOpen,x0
        move    x0,x:DecoderCSR              ; prevents attempts to close a file multiple times w/out opening

;_checkIfCompleteTrackDecoded
;        move    x:DecoderCSR,x0
;        jclr    #EndOfSong,x0,_partialTrackDecoded   
;		; If complete track is decoded then set song position to the begining of the file
;		clr		A
;		;move	A0,x:g_bAudibleConsumedBytesLow
;		;move	A1,x:g_bAudibleConsumedBytesHigh
;		move	A0,y:SongByteTotalLow
;		move	A1,y:SongByteTotalHigh
;_partialTrackDecoded   
		; tell the menu module to update file psition in *.POS file
        move    #MsgMenuSetSongPosition,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

_exit
        and     #>$FE,ccr
        rts


;--------------------------------------------------
CheckABBoundaries
        move    x:DecoderCSR,x0
_isBModeSet
        jset    #ABMode_B,x0,_checkBoundaryB
        jclr    #EndOfSong,x0,_exitCheckAB
_turnOffABMode
        ; end of song reached before "B" pressed -- turn off AB Mode
        jsr     DecABModeDisable
        jmp     _exitCheckAB
_checkBoundaryB
        jsr     DecGetSongPos           ; returns current song position in A1:A0
        jsr     DecGetABModeBPos        ; returns AB Mode "B" position in B1:B0
        cmp     a,b
        jgt     _exitCheckAB            ; not at position "B" yet
        jsr     DecSetSongPosA          ; at "B", so set to position "A"
_exitCheckAB
        rts

;--------------------------------------------------
DecABModeDisable
; Disables the AB Mode by clearing the bits and resetting byte positions to 0.
; Sends message to MENU to turn off AB icons.
        bclr    #ABMode_A,x:DecoderCSR  ; disable AB Mode
        bclr    #ABMode_B,x:DecoderCSR
        move    y:<Const_000000,x0      ; reset memory locations
        move    x0,y:AcelpABPositionAHigh
        move    x0,y:AcelpABPositionALow
        move    x0,y:AcelpABPositionBHigh
        move    x0,y:AcelpABPositionBLow

        ; post msg to menu to remove AB icon
;        move    #>MsgDecoderABModeOff,r0
;        move    y:<Const_ffffff,m0
;        jsr     SysPostMessage
        rts

DecGetABModeBPos
; Returns: B1 = AcelpABPositionBHigh , B0 = AcelpABPositionBLow
        clr     b
        move    y:AcelpABPositionBHigh,b1
        move    y:AcelpABPositionBLow,b0
        rts

DecSetSongPosA
        move    x:DecoderCSR,x0
        jset    #ABQuiet,x0,_skip
        jsr     StopCurrentSong
        bclr    #Stop,x:DecoderCSR
_skip

;        ; Seek to Zero in File
;        jsr     SysAllLinear
;        clr     a
;        jsr     FAudible_AcelpFileSeek
;        tst     a
;        jeq     _atZero
;        error
_atZero
; Sets the song position to AB position "A"
        clr     a
        move    y:AcelpABPositionAHigh,a1
        move    y:AcelpABPositionALow,a0
        move    a1,y:SongByteTotalHigh
        move    a0,y:SongByteTotalLow
		jsr		FAudible_AcelpFileSeek
		tst		a
		jeq		_atALocation
		error
_atALocation
        jsr     DecoderUpdateCurrentTime

        move    x:DecoderCSR,x0
        jset    #ABQuiet,x0,_exit          ; do NOT post PLAY msg if FFWDing

        ; post message to self (decoder) to Play
        move    #>PlayMessage,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

_exit
        rts



      ; Decoder Specific Functions
;     This section contains decoder specific functions. 
;     In future, only this section needs to be added for
;     a new decoder, when we unify the handling of all 
;     the decoders

AcelpGetSongInfo
    rts   


SongInfoCpy
    move y:<Const_000002,n0     ; WMA Characters are located in every alternate word
    do  #10,_EndCpy
        move y:(r0)+n0,A
        move y:(r0)+n0,y0
        mpy  #16,y0,B
        move B0,y0
        or  y0,A        y:(r0)+n0,y0
        mpy  #8,y0,B
        move B0,y0
        or  y0,A
        move A1,y:(r4)+
_EndCpy
        rts



DecoderComputeSeek
        move    y:SecondstoSkip,x0
        move    y:FAcelp_BitRate,A
        move    A,y0
        mpy     x0,y0,B         
        asr        B               
        asr        B               
        asr        B               
        asr        B               
        rts

FAcelpGetAuthorPtr
FAcelpGetTitlePtr
	rts
        
        endsec
