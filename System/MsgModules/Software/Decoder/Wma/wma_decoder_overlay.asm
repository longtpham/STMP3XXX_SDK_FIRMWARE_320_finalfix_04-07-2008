;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2005
;
; STMP3xxx User-level Windows Media Audio (WMA) Decoder Module (Can be overlayed)
;///////////////////////////////////////////////////////////////////////////////
        opt     mex

        section Wma_DecoderOverlay


;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////
;        nolist
    include "project.inc"
    include "sysequ.inc"
    include "sysspeed.inc"
    include "hwequ.inc"
    include "dacequ.inc"
    include "msgequ.inc"
    include "decoderequ.inc"
    include "mixerequ.inc"
    include "resource.inc"
    include "sysmacro.asm"
    include "decoder_overlay.xref"
    include "stack.xref"
    include "stackmem.xref"
    include "sysmem.xref"
    include "wmamem.xref"
    include "Wmadec.inc"
    include "wmalib.xref"
    include "sysresources.inc"
    include "const.xref"
    include "wmadecodermem.xref"
    include "dacmem.xref"
    include "dacmemy.xref"
    include "dacdrv.xref"
    include "sysmisc.xref"
    include "sysirq.xref"
    include "bufdesc.xref"
    include "sysresource.xref"
    include "decodermem.xref"
    include "mixer.xref"
    include "wmadec.xref"
    include "systimers.xref"
    include "syscoder.xref"

        page    132,60,0,0,0



;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////
        GLOBAL  WmaDecoderModule
        GLOBAL  WmaDecoderModuleInit
        Global	CurrentModule

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////
        EXTERN   WmaDecInit,WmaDecProcess
	EXTERN	 EffectsInit
        EXTERN   Fg_CurrentSongName,WmaDecoderFileHandle,WmaDecoderFileHandle2
    	EXTERN   DecoderQueueDescriptor
    	EXTERN Fg_CurrentSongFastkey
        EXTERN   CurrentDirDevId
        EXTERN   WmaDecoderModuleTimerDisable
        EXTERN   WmaDecoderModuleIsr
        EXTERN   SysEnableTimer
        EXTERN   TotalSongCount

        EXTERN   SysLoadResource
        EXTERN    FILEClose
        EXTERN    FILEOpen
        EXTERN    FileEOF
        EXTERN    FileRead
		EXTERN    FFSFastOpen

        EXTERN    ModuleSetTimer

        EXTERN    MsgDecoderStateChange
        EXTERN    MsgMenuSongTimeChange
        EXTERN    MsgGeqSetCoef
        EXTERN    MsgGeqParam
        EXTERN    auobj_iSamplingRate

        EXTERN    MsgGeqSetCoef
        EXTERN    MsgGeqParam
        EXTERN    auobj_iSamplingRate

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
;        EXTERN    MsgDecoderABModeOff
        EXTERN    MsgParserDeviceEnum
        EXTERN    PlayMessage
		EXTERN    WmaGetSongInfo
        EXTERN    HasJanusDRMLICFail
        EXTERN    JanusDRMLicExpired

        EXTERN    DecoderProcessPtr
        EXTERN    GetInvBitRate
	    EXTERN 	  WMAParseInfo
        EXTERN    DacData1L,DacData1R
        EXTERN    SecondstoSkip
        EXTERN    filest_hdrst_nAvgBytesPerSec,filest_hdrst_cbHeader
        extern    filest_hdrst_nSamplesPerSec
	    EXTERN    FileSeek
	     EXTERN    SysSpeedIncrease
	     EXTERN    SysSpeedClockFree
	    extern    UserScratchY
    	extern    DacClearMemory
	    extern    SysDelay
        extern    Fg_wBitRateFound
        extern F_lc_u_b_DrmpdCommon_P
        extern F_lc_u_e_DrmpdCommon_P
        extern F_lc_u_e_TABLE_X
        extern F_lc_u_b_TABLE_X
        extern F_lc_u_e_WMA_CONST_X
        extern F_lc_u_b_WMA_CONST_X
        extern F_lc_u_e_JANUSMEM_X
        extern F_lc_u_b_JANUSMEM_X
	if (@def('USE_PLAYLIST3'))
		extern F_lc_u_e_MusicLibBrowse_P
		extern F_lc_u_b_MusicLibBrowse_P
	endif


	if (@def('SYNC_LYRICS'))
    EXTERN    MsgUpdateLyricsAB
    endif

    if (@def('JANUS')&&@def('CLCD')&&@def('BACKLIGHT')&&!@def('BATTERY_TYPE_LI_ION'))
        extern  FbWMALowBattery ; musicmenu would set or clear this variable depends on the battery level.
    endif
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
       org      p,"DECWMAMOD_DecoderOverlay_overlay_P":

        ; Resource number
        dc      RSRC_WMADEC_CODE

;*******************************************************************************
;  Function:          WmaDecoderModuleInit
;  Description:       Decoder Initialization and interface module
;  Parameters passed: None
;  Registers Trashed: All
;  Return Values:     A contains word to place in ModuleTableEntry.wait that specifies
;                     what events should cause the exec to call WmaDecoderModule.
;*******************************************************************************
WmaDecoderModuleInit

        ; save off stack
        move    #>stackSaved,r6              ; setup the return address
        move    #>UserScratchY,r5             ; pointer to the SP local save
        jmp     SaveSystemStack              ; save the hardware stack
stackSaved
        ; init the ModuleTable process pointer
        move    #WmaDecoderModule,x0
        move    x0,y:DecoderProcessPtr

        ; Load the Wma decoder into P memory
	; Common sections of core decoder loaded first.
	move    #>RSRC_DRMPD_COMMON,x0
	move    #>RSRC_TYPE_DATA,x1             		    ;Resource type is data
        move    #>(F_lc_u_e_DrmpdCommon_P-F_lc_u_b_DrmpdCommon_P)*3,y1 ;Size of the overlay area
        move    #>F_lc_u_b_DrmpdCommon_P,y0      ;  Address of the overlay area
        move    #TARGET_MEM_P,a1             		; Overlay area is in Y memory
        jsr     SysLoadResource


	move    #>RSRC_WMA_COMMON,x0
	move    #>RSRC_TYPE_DATA,x1             		; Resource type is data
        move    #>(F_lc_u_e_WMANonOvl-F_lc_u_b_WMANonOvl)*3,y1 ; Size of the overlay area
        move    #>F_lc_u_b_WMANonOvl,y0      ;  Address of the overlay area
        move    #TARGET_MEM_P,a1             		; Overlay area is in Y memory
        jsr     SysLoadResource

	move    #>RSRC_WMA_INIT,x0
	move    #>RSRC_TYPE_DATA,x1             		; Resource type is data
        move    #>(F_lc_e__pInit-F_lc_b__pInit)*3,y1 ; Size of the overlay area
        move    #>F_lc_b__pInit,y0      ;  Address of the overlay area
        move    #TARGET_MEM_P,a1             		; Overlay area is in Y memory
        jsr     SysLoadResource


        move    #>RSRC_WMA_TABLES,x0
        move    #>RSRC_TYPE_DATA,x1             		; Resource type is data
        move    #>(F_lc_u_e_TABLE_X-F_lc_u_b_TABLE_X)*3,y1 ; Size of the overlay area
        move    #>F_lc_u_b_TABLE_X,y0      ;  Address of the overlay area
        move    #TARGET_MEM_X,a1             		; Overlay area is in Y memory
        jsr     SysLoadResource


        move    #>RSRC_WMA_CONSTANTS,x0
        move    #>RSRC_TYPE_DATA,x1             		; Resource type is data
        move    #>(F_lc_u_e_WMA_CONST_X-F_lc_u_b_WMA_CONST_X)*3,y1 ; Size of the overlay area
        move    #>F_lc_u_b_WMA_CONST_X,y0      ;  Address of the overlay area
        move    #TARGET_MEM_X,a1             		; Overlay area is in Y memory
        jsr     SysLoadResource

        ; Load the Wma decoder into X memory.
        move    #>RSRC_WMA_DATA_X1,x0             ; Resource number
        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
        move    #>(F_lc_u_e_WMA_X1-F_lc_u_b_WMA_X1)*3,y1  ; Size of the overlay area
        move    #>F_lc_u_b_WMA_X1,y0              ; Address of the overlay area
        move    #TARGET_MEM_X,a1                ; Overlay area is in X memory
        jsr     SysLoadResource


        ; Load the Janus into X memory
        move    #>RSRC_JANUS_X,x0                                        ; Resource number
        move    #>RSRC_TYPE_DATA,x1                                         ; Resource type is data
        move    #>(F_lc_u_e_JANUSMEM_X-F_lc_u_b_JANUSMEM_X)*3,y1            ; Size of the overlay area
        move    #>F_lc_u_b_JANUSMEM_X,y0                                    ; Address of the overlay area
        move    #TARGET_MEM_X,a1                                            ; Overlay area is in X memory
        jsr     SysLoadResource

        ; Load the Janus Data into y memory
        move    #>RSRC_JANUSY_DATA,x0
        move    #>RSRC_TYPE_DATA,x1             		; Resource type is data
        move    #>(F_lc_u_e_DATA_JANUS_Y-F_lc_u_b_DATA_JANUS_Y)*3,y1 ; Size of the overlay area
        move    #>F_lc_u_b_DATA_JANUS_Y,y0      ;  Address of the overlay area
        move    #TARGET_MEM_Y,a1             		; Overlay area is in X memory
        jsr     SysLoadResource


        ; Load the Wma decoder into Y memory
        move    #>RSRC_WMA_DATA_Y,x0             ; Resource number
        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
        move    #>(F_lc_u_e_WMA_Y-F_lc_u_b_WMA_Y)*3,y1  ; Size of the overlay area
        move    #>F_lc_u_b_WMA_Y,y0      ;  Address of the overlay area
        move    #TARGET_MEM_Y,a1                ; Overlay area is in Y memory
        jsr     SysLoadResource

        ; Load the Wma decoder into L memory
	; X part of L
         move    #>RSRC_WMA_DATA_L_X,x0             ; Resource number
         move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
         move    #>(F_lc_u_e_WMA_LX-F_lc_u_b_WMA_LX)*3,y1  ; Size of the overlay area
         move    #>F_lc_u_b_WMA_LX,y0      ;  Address of the overlay area
         move    #TARGET_MEM_X,a1                ; Overlay area is in Y memory
         jsr     SysLoadResource

        ; Y part of L
         move    #>RSRC_WMA_DATA_L_Y,x0             ; Resource number
         move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
         move    #>(F_lc_u_e_WMA_LY-F_lc_u_b_WMA_LY)*3,y1  ; Size of the overlay area
         move    #>F_lc_u_b_WMA_LY,y0      ;  Address of the overlay area
         move    #TARGET_MEM_Y,a1                ; Overlay area is in Y memory
         jsr     SysLoadResource

        ; clear the pcm buffer for Wma decode routines
        ; PCM buffer is in X memory.
	; bit buffer is cleared internally.
        clr     a
        move    y:<Const_ffffff,m3
        move    #>g_WmaDecSink_mbuf,r3
	move	#g_WmaDecSinkBufSize,x0
        do      x0,_clear_pcmbuffer
        move    a,x:(r3)+
        nop
_clear_pcmbuffer

        ; Initialize Wma Decoder.
        move    #g_WmaDecSinkBufDescriptor,x0
        move    x0,y:WmaDecoderSinkBufDesc      ; decoder sink buffer descriptor

        ; Init decoder status to stopped
        move    y:<Const_000000,x0
        bset    #Stop,x0
        move    x0,x:DecoderSR

        move	#>g_WmaDecSinkBufDescriptor,A
		move	#>TARGET_MEM_X,B
		move	y:<Const_000001,x0							; Always Stereo output
    	; EffectsInit should be called before calling any DAC initialization to ensure the processing chain is loaded.
        jsr		EffectsInit

        ; Init the interrupt vectors
        move    #>DacDriverIsr,x0
        move    #>DacDriverIsr,x1
        move    #>WmaDecoderModuleIsr,y0
        jsr     SysSetupDecVectors

        ; restore stack
_restoreStack
        move    #>UserScratchY,r5             ; pointer to the SP local save
        move    #>_stackRestored,r6           ; setup the return address
        jmp     RestoreSystemStack           ; restore the hardware stack
_stackRestored

_exitInit
        ;ignore messages until the Decoder is reset
        bset    #DecoderIgnoreMessages,x:DecoderSR

        ; Wait to call WmaDecoderModule until we get a message event
        move    #>EVENT_MESSAGE,A

        rts

;*******************************************************************************
;  Function:          WmaDecoderModule
;  Description:       Decoder interface module
;  Parameters passed: None
;  Registers Trashed: All
;  Return Values:     A1 contains word specifying what events to wait on.
;*******************************************************************************
WmaDecoderModule
	nop
        ; save off stack
        move    #_stackSaved,r6              ; setup the return address
        move    #UserScratchY,r5             ; pointer to the SP local save
        jmp     SaveSystemStack              ; save the hardware stack
_stackSaved

_ProcessSysMessages
        ; switch on system messages
        move    #_queueProcessed,r6
        jmp     DecoderProcessQueue
_queueProcessed

_UpdateLcdMessage
        move    x:DecoderCSR,x0              ; copy contents of status register
        jset    #FileReadError,x0,_FileReadError ; error reading the file
        jset    #EndOfSong,x0,_continueChecks
        jclr    #Play,x0,_LCDdone            ; only update if playing (save MIPS)
_continueChecks
        jset    #SongInfo,x0,_getSongCurrentTime
        bset    #SongInfo,x0                 ; prevents constant updating of same song info
        move    x0,x:DecoderCSR              ; SongInfo bit is cleared by opening of new song file
        move    #>MsgGeqSetCoef,r0
        move    y:auobj_iSamplingRate,x0
        move    x0,y:MsgGeqParam
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

_getSongCurrentTime
        jsr     DecoderUpdateCurrentTime     ; updates the display of the elapsed/remaining song time
_LCDdone

_checkABModeBoundaries
        move    x:DecoderCSR,x0
        jset    #Pause,x0,_CheckIfTimeMustUpdate       ; must check Pause bit in case "just Paused" now
        jclr    #ABMode_A,x0,_CheckIfTimeMustUpdate
        jsr     CheckABBoundaries
        jcs     _handleCheckABBoundariesError

_CheckIfTimeMustUpdate
        ; Check if we need to allow LCD to update time display.
        ; This is to prevent EndOfSong before display update.
        move    x:DecoderCSR2,x0
        jset    #TimeNeedsUpdate,x0,_exitWmaDecoderModule
_CheckIfEndOfList
        move    x:DecoderCSR,x0
        jset    #EndOfList,x0,_exitWmaDecoderModule
_CheckIfEndOfSong
         move   x:DecoderCSR,x0
         jclr   #EndOfSong,x0,_exitWmaDecoderModule
		; debug
         move    y:WmaDecoderSinkBufDesc,r0
         move    y:<Const_ffffff,m0
         jsr     GetFilledSpace              ; returns: a==number of words in buffer
		 move    #>$200,y0
		 cmp 	 y0,a
		 jgt     _exitWmaDecoderModule

		 ; Check space available in output buffer
		 ; if more than 512 samples available, go to _exitWmaDecoder
        ;-------------------------------------------------------------
        ; Done With Song
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
        move    #>SPEED_CLIENT_WMA_DEC,y0
        jsr     SysSpeedClockFree

        ;ignore messages until the Decoder is reset
        bset    #DecoderIgnoreMessages,x:DecoderSR

        move    #>EVENT_MESSAGE,x0
        move    x0,y:WmaDecoderEventValue        ; allow the parser to call it next (if songs are left)


_exitWmaDecoderModule
        ; restore stack
        move    #>UserScratchY,r5             ; pointer to the SP local save
        move    #>_stackRestored,r6           ; setup the return address
        jmp     RestoreSystemStack           ; restore the hardware stack
_stackRestored

        move     y:WmaDecoderEventValue,a1   ; return value for executive (EVENT_NONE or EVENT_MESSAGE)
        move     y:<Const_ffffff,m1          ; make sure that m1 is -1!
        rts

_FileReadError
        bclr    #FileReadError,x:DecoderCSR
        bset    #FileReadError,x:DecoderSR
        ; stop the DAC & close the file
        jsr     StopCurrentSong
_handleStopCurrentSongError
        jsr     DecoderCloseSong
_handleCloseSongError
_handleCheckABBoundariesError
        ; tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        ; WMA speed client frees DCLK for other speed clients
        move    #>SPEED_CLIENT_WMA_DEC,y0
        jsr     SysSpeedClockFree

        move    #>EVENT_MESSAGE,x0
        move    x0,y:WmaDecoderEventValue
        jmp     _exitWmaDecoderModule

DecoderSetEventTimer
; if we want to return on the module timer OR message,
; this routine sets WmaDecoderEventValue up.
        move    #>EVENT_MESSAGE,a1
        move    #>500,x0
        jsr     ModuleSetTimer
        move    a1,y:WmaDecoderEventValue
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
;        bclr    #SyncWait,x:DecoderCSR
        bset    #Stop,x:DecoderCSR

_pollPlaybitStop
        move    x:DecoderCSR,x0
        jset    #Play,x0,_pollPlaybitStop ; wait for isr to enter "Stop" state
        ; isr will turn off Timer Module before returning

        ; Fade the Volume to Mute
        move    #>msFadeOut,x0
        jsr     MixerMasterFadeOut      ; fade volume down to mute
_muted

        move	#>g_WmaDecSinkBufDescriptor,A
		move	#>TARGET_MEM_X,B
		move	y:<Const_000001,x0							; Always Stereo output
    	; EffectsInit should be called before calling any DAC initialization to ensure the processing chain is loaded.
        jsr		EffectsInit

        jsr     DacDriverDisablePort

        ; clear dac memory
		jsr		DacClearMemory
        ; Set the status register
        bset    #Stop,x:DecoderSR
        bclr    #Pause,x:DecoderSR
        bclr    #Play,x:DecoderSR

        and     #>$FE,ccr
        rts

;-------------------------------------------------
DecoderUpdateLCD
        jsr     DecoderUpdateCurrentTime     ; 0:00 / ----
        rts
;-------------------------------------------------
;DecoderUpdateTotalTime
; Updates the total time of the current song.
; Sends a message to the LCD to print it.
;        move    y:<Const_ffffff,m4
;        move    #SongByteLengthHigh,r4
;        move    #SongByteLengthLow,r5
;        move    m4,m5
;        jsr     GetSongTime
;        move    x1,y:SongTotalMinutes
;        move    x0,y:SongTotalSeconds

;        rts

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
        bclr    #TimeNeedsUpdate,x:DecoderCSR2 ; allow EndOfSong to go into effect (if end is reached)
        rts
_update
        bset    #TimeNeedsUpdate,x:DecoderCSR2 ; prevent EndOfSong check prior to a necessary LCD time update
        move    x1,y:SongCurrentMinutes
        move    x0,y:SongCurrentSeconds
        move    #MsgMenuSongTimeChange,r0
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

; ***** Updated method for calculating WMA current play time *****
; This method will take the normalized (by 512) SongSecondsPerByte and multiply it
; by the (current byte position minus header byte length) to get the song time.
; IF the SongSecondsPerByte is ZERO, it will use the old method to calculate the time.
        move    y:SongSecondsPerByte,a
        tst     a
        jeq     _UseInvBitRate               ; the old method
_UseSecondsPerByte
        ; New Method
        move    y:SongSecondsPerByte,y0         ; (bytes/seconds)
        move    L:filest_hdrst_cbHeader,x    ; length of wma object header
        move    y:(r4),A1                    ; high byte count (current location in file)
        move    y:(r5),A0                    ; low byte count (current location in file)
        sub     x,A     #0,x0                ; A = current location - header length
        tlt     x0,A                         ; Fix if negative result (set it to 0)
        move    A0,x0
        move    A1,x1
        jmp     _Multiply

_UseInvBitRate
        ; Old Method
        move    y:SongInvBitRate,y0          ; Current inv bit rate multiplier
        move    y:(r4),A1                    ; high byte count
        move    y:(r5),A0                    ; low byte count
        move    L:filest_hdrst_cbHeader,x
        sub     x,A     #0,x0
        tlt     x0,A
        move    A0,x0
        move    A1,x1

_Multiply
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
;	      Deque messages passed to the Decoder
;             r0=Descriptor for this modules message queue (in y memory)
;             m0=linear
;  Parameters passed:
;	      None
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

        ;We always want to process DECODER_RESET
        move    #>DECODER_RESET,x0
        cmp     x0,b
        jeq     _ContinueProcessing

        ;If we're supposed to be ignoring messages, lets skip to the next message
        move    x:DecoderSR,x0
        jset    #DecoderIgnoreMessages,x0,_NextMessage

_ContinueProcessing
        ; Check for case of no songs to play
        move    y:TotalSongCount,a
        tst     a
        jne     _songsPresent
        jsr     DecABModeDisable        ; reset any AB mode settings
        move    #>EVENT_MESSAGE,x0      ; no songs avail -- only call on button push
        move    x0,y:WmaDecoderEventValue
        jmp     _NextMessage            ; ignore messages - no songs!

_songsPresent
        ; Point r1 to the first argument of the message
        move    (r1)+
        move    (r1)+

       ; we don't use circular mode to access queue buffer with r1
        jsr     SysAllLinear	;stmp6661

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

;-------------------------SetPlaylist Function-------------------------
_ResetDecoder
        bclr    #DecoderIgnoreMessages,x:DecoderSR
	jsr     DecABModeDisable             ; reset any AB mode settings
        ;NEED TO ADD MORE "RESET" stuff so that DECODER_RESET really does do what its supposed to
        jmp     _NextMessage

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

		jsr		SysAllLinear			; Always linear
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
        move    #>msFadeOut,x0
        jsr     MixerMasterFadeOut      ; fade volume down to mute
_muted

        move	#>g_WmaDecSinkBufDescriptor,A
		move	#>TARGET_MEM_X,B
		move	y:<Const_000001,x0							; Always Stereo output
    	; EffectsInit should be called before calling any DAC initialization to ensure the processing chain is loaded.
        jsr		EffectsInit

        jsr     DacDriverDisablePort    ; turn off the DAC

        ; WMA speed client frees DCLK
        ; free DCLK for other speed clients
        move    #>SPEED_CLIENT_WMA_DEC,y0
        jsr     SysSpeedClockFree

        ; tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        move    #>EVENT_MESSAGE,x0
        move    x0,y:WmaDecoderEventValue

        jmp     _NextMessage

_beginPlay

        ;jsr     WmaGetSongInfo
        move    x:DecoderCSR,x0
        bclr    #Pause,x0
        bclr    #SongInfo,x0            ; necessary to notify decoder module to update song info (new song)
        move    x0,x:DecoderCSR

        move    r7,x:WmaDecoderSaveR7   ; temp -- later need to strip r7 out of the routines
        move    m7,x:WmaDecoderSaveM7   ; temp -- later need to strip m7 out of the routines
        jsr     WmaDecoderOpenSong
        jcs     _handleOpenSongError
        ;----------
    ;DS :- In order to reduce the time delay between songs we set the clock to 75Mhz (peak) instead of the wma frequency.
    ; After WmaDecInit is called the clock peak request is freed.
        ;move    #>SPEED_WMA,x1          ; historic comment: Change to SPEED_MAX was changed to speed wma then speed peak
        if (@def('JANUS')&&@def('CLCD')&&@def('BACKLIGHT')&&!@def('BATTERY_TYPE_LI_ION'))
        jsr     WMALowBatteryCheck
        endif

        move    #>SPEED_PEAK,x1
        move    #>SPEED_CLIENT_WMA_DEC,y0
        jsr     SysSpeedIncrease

        jsr     WmaDecInit              ; init the wma decoder
          tst	A
          jne	_StopPlay

        move    y:<Const_ffffff,m3
        move    #>g_WmaDecSink_mbuf,r3      ; clear the output buf
        move	#g_WmaDecSinkBufSize,x0
        do      x0,_clear_pcmbuffer
          move    a,x:(r3)+
          nop
_clear_pcmbuffer

        ; Free DCLK for other speed clients
        move    #>SPEED_CLIENT_WMA_DEC,y0
        jsr     SysSpeedClockFree
        ;----------

        move	#>g_WmaDecSinkBufDescriptor,A
		move	#>TARGET_MEM_X,B
		move	y:<Const_000001,x0							; Always Stereo output
    	; EffectsInit should be called before calling any DAC initialization to ensure the processing chain is loaded.
        jsr		EffectsInit

        ; Init dac and decoder for new song
        jsr     DacDriverInit
        jsr     DacDriverEnablePort   ; set up the DAC control registers, enable the port

        move	y:Ffilehdr_sample_rate,A
	    jsr		DacSetSamplingRate

        ;jsr     WmaGetSongInfo

		jsr	GetInvBitRate		; Get the Inverse of Bit Rate.

        jsr     DecoderUpdateLCD        ; song time and number

        move    y:SongByteTotalHigh,a  ; high bytes
        move    y:SongByteTotalLow,a0   ; low bytes
	tst	A
	jeq	_ContinueSkipRandomCheck
	; Else seek to the location
        jsr     WMAFileSeek
_ContinueSkipRandomCheck


        ; Check for the high MIPs low rate file.
        move x:mode_type,B
        move y:<Const_000002,y0
        cmp  y0,B y:filehdr_num_channels,B
        jne  _ContinueCheck1
        ; Check number of channels
        cmp  y0,B y:filest_hdrst_nSamplesPerSec,B
        jne  _ContinueCheck1
        ; Check sampling rate
        move #>32000,y0
        cmp  y0,B
        jeq  _Speed60Mhz

_ContinueCheck1

        move y:filest_bHasJanusDRM,A
        tst  A
        jne  _Speed60Mhz
        move #>160000,x0
        move y:filest_bHasDRM,A
    	tst  A      #>96000,B
        teq  x0,B
        move y:filehdr_bitrate,A
        cmp  B,A
        jge  _Speed60Mhz

_Speed48Mhz
        move    #>SPEED_MAX,x1          ; Change to SPEED_MAX
        move    #>SPEED_CLIENT_WMA_DEC,y0
        jsr     SysSpeedIncrease
        jmp     _SetDacInterrupt

_Speed60Mhz
        ; WMA speed client increases DCLK if we're not at desired or higher
        move    #>SPEED_WMA,x1          ; Change to SPEED_WMA
        move    #>SPEED_CLIENT_WMA_DEC,y0
        jsr     SysSpeedIncrease

_SetDacInterrupt

        ; Enable the DAC interrupt
        bset    #HW_DACCSR_TXIEN_BITPOS,x:HW_DACCSR


        move    x:WmaDecoderSaveM7,m7
        move    x:WmaDecoderSaveR7,r7



        ; Turn Timer Module ON
        move    #>DecoderTimerNum,x0
        move    #_TimerEnabled_2,r0
        jmp     SysEnableTimer
_TimerEnabled_2
        ; A "tick" appears before every stop->play or pause->play transition.
        ; I have checked all the buffers from output->sink and cleared any history associated with these
        ; Workaround: Delay the fade in by 30 ms.
        move    #>30,x0
        jsr     SysDelay

        move    #>msFadeIn,x0
        jsr     MixerMasterFadeIn            ; fade volume in to previous level

        ; tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage
        jsr     DecoderSetEventTimer         ; set up DecoderEventValue for module timer
    if (@def('USE_PLAYLIST3'))
	; reload browse_app for browsing modules
        move    #>RSRC_MUSICLIB_BROWSE_APP,x0                      ; Resource number
        move    #>RSRC_TYPE_DATA,x1                        ; Resource type is data
        move    #>(F_lc_u_e_MusicLibBrowse_P-F_lc_u_b_MusicLibBrowse_P)*3,y1 ; overlay area sz
        move    #>F_lc_u_b_MusicLibBrowse_P,y0                      ; Address of the overlay area
        move    #TARGET_MEM_P,a1                           ; Overlay area is in P memory
        jsr     SysLoadResource
	endif
        jmp     _NextMessage

_StopPlay
        clr a
        move y:HasJanusDRMLICFail,A
        tst a
        jeq _continue
        move y:<Const_000001,x0
        move x0,y:JanusDRMLicExpired
    if @def('USE_PLAYLIST5')
		bset    #0,y:CurrentSongBad
    endif  ;if @def('USE_PLAYLIST5')

_continue
        ; Free DCLK for other speed clients
        move    #>SPEED_CLIENT_WMA_DEC,y0
        jsr     SysSpeedClockFree



        jsr     StopCurrentSong              ; includes FadeOut & disable DAC
        jsr     DecoderCloseSong

        ; tell the PlayerStateMachine  to advance to next song and Play
        move    #>NEXT_SONG_PLAY_EOF,x0      ; EOF && PLAY
        move    x0,y:MsgParserNextParam
        move    #>MsgParserNextSong,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        ; Free DCLK for other speed clients
        move    #>SPEED_CLIENT_WMA_DEC,y0
        jsr     SysSpeedClockFree

        ;ignore messages until the Decoder is reset
        bset    #DecoderIgnoreMessages,x:DecoderSR

        move    #>EVENT_MESSAGE,x0
        move    x0,y:WmaDecoderEventValue        ; allow the parser to call it next (if songs are left)
    if (@def('USE_PLAYLIST3'))
	; reload browse_app for browsing modules
	    move    #>RSRC_MUSICLIB_BROWSE_APP,x0                      ; Resource number
        move    #>RSRC_TYPE_DATA,x1                        ; Resource type is data
        move    #>(F_lc_u_e_MusicLibBrowse_P-F_lc_u_b_MusicLibBrowse_P)*3,y1 ; overlay area sz
        move    #>F_lc_u_b_MusicLibBrowse_P,y0                      ; Address of the overlay area
        move    #TARGET_MEM_P,a1                           ; Overlay area is in P memory
        jsr     SysLoadResource
	endif
        jmp     _NextMessage




;-------------------------Stop Function-------------------------
_Stop
        jsr     DecABModeDisable             ; reset any AB mode settings
        jsr     StopCurrentSong
        jsr     DecoderCloseSong
        jsr     DecSetSongPosZero
        jsr     DecClearSongTime

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
        move    #>SPEED_CLIENT_WMA_DEC,y0
        jsr     SysSpeedClockFree

        move    #>EVENT_MESSAGE,x0
        move    x0,y:WmaDecoderEventValue

        jmp     _NextMessage

;-------------------------Ffwd Function-------------------------
_Ffwd
        bset #ABQuiet,x:DecoderCSR

        ; Open File If Necessary
        move    x:DecoderCSR,x0
        jset    #FileIsOpen,x0,_FileOpenedForFfwd
        clr     a
        move    y:WmaDecoderFileHandle,a2
        jsr     WmaDecoderOpenSong         ; open the file
	jcs     _handleOpenSongError
        ; tell the menu module that the status has changed.
        ; this is necessary to update song name when crossing a boundary
        move    #>MsgDecoderStateChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage
_FileOpenedForFfwd

        ; Very bad, if the ISR also wants to mess with WMA variables
        ; Shut down the ISR timer for timer being.
        move    #>DecoderTimerNum,x0
        move    #_TimerDisabled_1,r0
        jmp     SysDisableTimer
_TimerDisabled_1

        move    #>SPEED_WMA,x1          ; Change to SPEED_MAX
        move    #>SPEED_CLIENT_WMA_DEC,y0
        jsr     SysSpeedIncrease


        jsr     WmaGetSongInfo
	tst	A
	jne	_goToNextSong

        ; Free DCLK for other speed clients
        move    #>SPEED_CLIENT_WMA_DEC,y0
        jsr     SysSpeedClockFree


    move	#>g_WmaDecSinkBufDescriptor,A
	move	#>TARGET_MEM_X,B
	move	y:<Const_000001,x0							; Always Stereo output
    ; EffectsInit should be called before calling any DAC initialization to ensure the processing chain is loaded.
	jsr	EffectsInit

        ; Great. Now, I have exclusive access.
        jsr     DacDriverInit
        jsr     DacDriverEnablePort   ; set up the DAC control registers, enable the port


        move	y:Ffilehdr_sample_rate,A
	    jsr		DacSetSamplingRate



	;jsr     WmaGetSongInfo
        ; FileSeek forward

        ; check if too close to beginning of file
        ;

        ; GET THE BITRATE
        clr     a
        move    y:Fg_wBitRateFound,a0
        ; CONVERT TO "BYTE"-RATE
        asr     a
        asr     a
        asr     a
        move    a0,y0
        ; MPY BY NUMBER OF SECONDS TO SKIP
        clr     a	y:SecondstoSkip,x0
;        move    y:filest_hdrst_nAvgBytesPerSec,y0
        mpy     x0,y0,B         y:SongByteTotalHigh,a1
        asr	B   y:SongByteTotalLow,a0
        add     A,B
        clr     A
        move    y:SongByteLengthHigh,A
        move    y:SongByteLengthLow,A0
        cmp     A,B                     ; if num bytes to skip < num bytes left, ffwd
        jlt     _Cont_Fwd
_goToNextSong


               ; Free DCLK for other speed clients
        move    #>SPEED_CLIENT_WMA_DEC,y0
        jsr     SysSpeedClockFree

        move    #>SPEED_MP3,x1          ; Change to SPEED_MAX
        move    #>SPEED_CLIENT_WMA_DEC,y0
        jsr     SysSpeedIncrease


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

        ; When moving to next song via _Ffwd, Free DCLK for other speed clients
        move    #>SPEED_CLIENT_WMA_DEC,y0
        jsr     SysSpeedClockFree

        ; tell the PlayerStateMachine  to advance to next song and Play
        move    #>NEXT_SONG_FFWD,x0 ; Button && PLAY
        move    x0,y:MsgParserNextParam
        move    #>MsgParserNextSong,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        ;ignore messages until the Decoder is reset
        bset    #DecoderIgnoreMessages,x:DecoderSR

        move    #>EVENT_MESSAGE,x0
        move    x0,y:WmaDecoderEventValue
        jmp     _NextMessage

_Cont_Fwd
        move    B0,y:SongByteTotalLow
        move    B1,y:SongByteTotalHigh
	jsr	GetInvBitRate		; Get the Inverse of Bit Rate.
        jsr     DecoderUpdateLCD        ; song time and number
        ; Now start the ISR timer again.
        move    #>DecoderTimerNum,x0
        move    #_TimerEnabled_10,r0
        jmp     SysEnableTimer
_TimerEnabled_10

        ; post message to update the time
        move    #MsgMenuSongTimeChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        move    #>EVENT_MESSAGE,x0
        move    x0,y:WmaDecoderEventValue

        jmp     _NextMessage

	;-------------------------Rwnd Function-------------------------
_Rwnd

        ; check if too close to beginning of file
        ;
        ; GET THE BITRATE
        clr     a
        move    y:Fg_wBitRateFound,a0
        ; CONVERT TO "BYTE"-RATE
        asr     a
        asr     a
        asr     a
        move    a0,y0
        ; MPY BY NUMBER OF SECONDS TO SKIP
        clr     a	y:SecondstoSkip,x0
;        move    y:filest_hdrst_nAvgBytesPerSec,y0
        mpy     x0,y0,B         y:SongByteTotalHigh,a1
	asr	B               y:SongByteTotalLow,a0
        sub     B,A
	move    A1,y:SongByteTotalHigh
	move    A0,y:SongByteTotalLow
	jgt	_rewind
_goToPrevSong
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

        ; When moving to next song via _Rwnd, Free DCLK for other speed clients
        move    #>SPEED_CLIENT_WMA_DEC,y0
        jsr     SysSpeedClockFree

;        ; tell the parser to backup a song and send RWND message when done
        move    #>PREV_SONG_RWND,x0          ; get previous song then rewind from end
        move    x0,y:MsgParserPrevParam
        move    #>MsgParserPrevSong,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        ;ignore messages until the Decoder is reset
        bset    #DecoderIgnoreMessages,x:DecoderSR

        move    #>EVENT_MESSAGE,x0
        move    x0,y:WmaDecoderEventValue
        jmp     _NextMessage

_rewind

        jsr     SysAllLinear
        ; Open File If Necessary
        move    x:DecoderCSR,x0
        jset    #FileIsOpen,x0,_FileOpenedForRwnd
        jsr     WmaDecoderOpenSong
	jcs     _handleOpenSongError
        ; tell the menu module that the status has changed.
        ; this is necessary to update song name when crossing a boundary
        move    #>MsgDecoderStateChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage
_FileOpenedForRwnd

        ; Very bad, if the ISR also wants to mess with WMA variables
        ; Shut down the ISR timer for timer being.
        move    #>DecoderTimerNum,x0
        move    #_TimerDisabled_2,r0
        jmp     SysDisableTimer
_TimerDisabled_2

        move    #>SPEED_WMA,x1          ; Change to SPEED_MAX
        move    #>SPEED_CLIENT_WMA_DEC,y0
        jsr     SysSpeedIncrease



        jsr     WmaGetSongInfo
	tst	A
	jne	_goToPrevSongonError

        ; Free DCLK for other speed clients
        move    #>SPEED_CLIENT_WMA_DEC,y0
        jsr     SysSpeedClockFree


	move	#>g_WmaDecSinkBufDescriptor,A
	move	#>TARGET_MEM_X,B
	move	y:<Const_000001,x0							; Always Stereo output
	; EffectsInit should be called before calling any DAC initialization to ensure the processing chain is loaded.
    jsr	EffectsInit

        jsr     DacDriverInit
        jsr     DacDriverEnablePort   ; set up the DAC control registers, enable the port

        move	y:Ffilehdr_sample_rate,A
	    jsr		DacSetSamplingRate


;	jsr     WmaGetSongInfo

_ContRewind
	jsr	GetInvBitRate		; Get the Inverse of Bit Rate.
        jsr     DecoderUpdateLCD        ; song time and number
        ; Now start the ISR timer again.
        move    #>DecoderTimerNum,x0
        move    #_TimerEnabled_11,r0
        jmp     SysEnableTimer
_TimerEnabled_11

        ; post message to update the time
        move    #MsgMenuSongTimeChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        move    #>EVENT_MESSAGE,x0
        move    x0,y:WmaDecoderEventValue

        jmp     _NextMessage

_handleOpenSongError
_handleFileSeekError
_goToPrevSongonError

             ; Free DCLK for other speed clients
        move    #>SPEED_CLIENT_WMA_DEC,y0
        jsr     SysSpeedClockFree

      move    #>SPEED_MP3,x1          ; Change to SPEED_MAX   TOVERIFY SAYS SPEED_MAX (48) BUT REALLY SETS 36MHZ. WHICH IS DESIRED?
      move    #>SPEED_CLIENT_WMA_DEC,y0
      jsr     SysSpeedIncrease

        ; Convince the parser that we need to go to previous song.
        clr     B
        move    B,y:SongCurrentMinutes
        move    B,y:SongCurrentSeconds

        ; Configure CSR
        bclr    #Ffwd,x:DecoderCSR
        bclr    #Rwnd,x:DecoderCSR
        jsr     DecABModeDisable             ; reset any AB mode settings

        jsr     StopCurrentSong
        jsr     DecoderCloseSong

        ; Free DCLK for other speed clients
        move    #>SPEED_CLIENT_WMA_DEC,y0
        jsr     SysSpeedClockFree

;        ; tell the PlayerStateMachine to backup a song and send RWND message when done
        move    #>PREV_SONG_RWND,x0          ; get previous song then rewind from end
        move    x0,y:MsgParserPrevParam
        move    #>MsgParserPrevSong,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        ;ignore messages until the Decoder is reset
        bset    #DecoderIgnoreMessages,x:DecoderSR

        move    #>EVENT_MESSAGE,x0
        move    x0,y:WmaDecoderEventValue
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

        ; Free DCLK for other speed clients
        move    #>SPEED_CLIENT_WMA_DEC,y0
        jsr     SysSpeedClockFree

        ; tell the PlayerStateMachine to advance to next song
        move    #>NEXT_SONG,x0          ; Button && NoPlay
        move    x0,y:MsgParserNextParam
        move    #>MsgParserNextSong,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage
        ; tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        ;ignore messages until the Decoder is reset
        bset    #DecoderIgnoreMessages,x:DecoderSR

        move    #>EVENT_MESSAGE,x0
        move    x0,y:WmaDecoderEventValue
        jmp     _NextMessage
_nextStop
         jsr     StopCurrentSong
         jsr     DecoderCloseSong

        ; tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        ; Free DCLK for other speed clients
        move    #>SPEED_CLIENT_WMA_DEC,y0
        jsr     SysSpeedClockFree

        ; tell the PlayerStateMachine to advance to next song and Play
        move    #>NEXT_SONG_PLAY_BUTTON,x0 ; Button && PLAY
        move    x0,y:MsgParserNextParam
        move    #>MsgParserNextSong,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        ;ignore messages until the Decoder is reset
        bset    #DecoderIgnoreMessages,x:DecoderSR

        move    #>EVENT_MESSAGE,x0
        move    x0,y:WmaDecoderEventValue
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

        ; Free DCLK for other speed clients
        move    #>SPEED_CLIENT_WMA_DEC,y0
        jsr     SysSpeedClockFree

        ; tell the PlayerStateMachine to advance to next song and Play
        move    #>PREV_SONG,x0          ; PrevSong && Stop
        move    x0,y:MsgParserPrevParam
        move    #>MsgParserPrevSong,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage
        ; tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        ;ignore messages until the Decoder is reset
        bset    #DecoderIgnoreMessages,x:DecoderSR

        move    #>EVENT_MESSAGE,x0
        move    x0,y:WmaDecoderEventValue
        jmp     _NextMessage

_prevStop
        ; song was playing
        jsr     StopCurrentSong
        jsr     DecoderCloseSong

        ; tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        ; Free DCLK for other speed clients
        move    #>SPEED_CLIENT_WMA_DEC,y0
        jsr     SysSpeedClockFree

        ; tell the PlayerStateMachine to advance to next song and Play
        move    #>PREV_SONG_PLAY,x0     ; PrevSong && Play
        move    x0,y:MsgParserPrevParam
        move    #>MsgParserPrevSong,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        ;ignore messages until the Decoder is reset
        bset    #DecoderIgnoreMessages,x:DecoderSR

        move    #>EVENT_MESSAGE,x0
        move    x0,y:WmaDecoderEventValue
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
        move    a1,y:WmaABPositionAHigh    ; save high byte
        move    a0,y:WmaABPositionALow     ; save low byte
        jmp     _exitABMode
_checkB
        jset    #ABMode_B,x0,_disableABMode
        bset    #ABMode_B,x:DecoderCSR  ; second msg is for "B" location
        jsr     DecGetSongPos           ; returns byte position in A1:A0
        move    a1,y:WmaABPositionBHigh    ; save high byte
        move    a0,y:WmaABPositionBLow     ; save low byte

        ; make sure A < B
        clr     b
        move    y:WmaABPositionAHigh,b1    ; B1 = AHigh
        move    y:WmaABPositionALow,b0     ; B0 = ALow
        cmp     a,b                     ; compare Bmode to Amode
        jlt     _exitABMode             ; Bmode > Amode
_disableABMode
        jsr     DecABModeDisable        ; clear bits, reset byte positions to 0.

_exitABMode
        move    x:DecoderCSR,x0
        jclr    #Stop,x0,_next
        move    #>EVENT_MESSAGE,x0
        move    x0,y:WmaDecoderEventValue
_next
        jmp     _NextMessage

	;-------------------------SetEQ Function----------------------------
_SetEQ
	; No Eq settings for WMA.
        jmp     _NextMessage

_DisplaySongInfo

		push	r1
		push	m1

		jsr		WmaDecoderOpenSong

        ; This is going to do lots of things, but we are interested
        ; in song information only.
        jsr    WMAParseInfo
		jsr    WmaGetSongInfo
		jsr    GetInvBitRate		; Get the Inverse of Bit Rate.
        jsr    DecoderUpdateLCD        ; song time and number

		jsr		DecoderCloseSong

		pop		m1
		pop		r1

        move    #>EVENT_MESSAGE,x0				; Don't wait on timer events.
        move    x0,y:WmaDecoderEventValue

        jmp    _NextMessage


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
        jmp     (r6)                      ; return to WmaDecoderModule routine

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
		dc		_SetEQ
		dc      _DisplaySongInfo

; Note: It is important that the entries in the jump table be in the
; same order as the Decoder message ID's which are defined in msgequ.inc

;*******************************************************************************
; DECODER-SPECIFIC FILE FUNCTIONS
; These functions are used by the decoder to perform file system actions.
;*******************************************************************************
;-----------------------------------------------------------------
; WmaDecoderOpenSong
;
;-----------------------------------------------------------------
WmaDecoderOpenSong

        ; if the song is already open then just exit
        move    x:DecoderCSR,x0
        jclr    #FileIsOpen,x0,_notYetOpen
        and     #>$FE,ccr
        rts
_notYetOpen

        ;-------------------------------------------------------------
        ; Open the Song (FILEOpen)
        ;-------------------------------------------------------------
        ; Inputs:   A2 = Device number  A1 = Complex ptr to file name buffer
        ;           A0 = Open attributes
        ; Output:
        ;           A1 = File handle (if successful)
        ;           A1 = Error code  (if failed)
        ;-------------------------------------------------------------
        jsr     SysAllLinear
        clr     a      #TARGET_MEM_Y,x0      ; memory space identifier (Y RAM)
        move    a,y:CurrentSongBad           ; need to mark current song not bad

  if @def('USE_PLAYLIST3')
;        move    y:CurrentDirDevId,a2         ; card number for current directory
;        move    #Fg_CurrentSongName,a1          ; base address of song name
;        or      x0,a                         ; A2 = dev#, A1=memspace/file name buffer, A0=0
;        move    #>READMODE,a0                ; A0 = file open attribute (Read Only)
		move    #Fg_CurrentSongFastkey,r0
		move    y:(r0)+,A0
		move    y:(r0),A1
        move    #>READMODE,b                ; A0 = file open attribute (Read Only)
		;iHandle = FastOpen(g_CurrentSongFastkey,(_packed char *)"r");
		jsr FFSFastOpen

        ;jsr     FILEOpen
  else  ;@def('USE_PLAYLIST3')
   if (@DEF('USE_PLAYLIST5'))
  	    move    #Fg_CurrentFastKey,r0
	    move    y:(r0)+,A0
	    move    y:(r0),A1
        move    #>READMODE,b                ; A0 = file open attribute (Read Only)
        jsr FFSFastOpen				;;iHandle = FastOpen(g_CurrentSongFastkey,(_packed char *)"r");
   else    ;(@DEF('USE_PLAYLIST5'))
        move    y:CurrentDirDevId,a2         ; card number for current directory
        move    #Fg_CurrentSongName,a1          ; base address of song name
        or      x0,a                         ; A2 = dev#, A1=memspace/file name buffer, A0=0
        move    #>READMODE,a0                ; A0 = file open attribute (Read Only)
        jsr     FILEOpen
   endif   ;(@DEF('USE_PLAYLIST5'))
  endif  ;@def('USE_PLAYLIST3')

        jcs     _exit
;        jcc     _fileOpened
;        error                                ; ERROR - file did not open (TBD -- ADD CODE!!)
;_fileOpened
        move    a1,y:WmaDecoderFileHandle       ; store the handle for the file

        clr     a      #TARGET_MEM_Y,x0      ; memory space identifier (Y RAM)
     ;   move    a,y:CurrentSongBad           ; need to mark current song not bad
      ;  move    y:CurrentDirDevId,a2         ; card number for current directory
       ; move    #Fg_CurrentSongName,a1          ; base address of song name
        ;or      x0,a                         ; A2 = dev#, A1=memspace/file name buffer, A0=0
        ;move    #>READMODE,a0                ; A0 = file open attribute (Read Only)
  if @def('USE_PLAYLIST3')
		move    #Fg_CurrentSongFastkey,r0
		move    y:(r0)+,A0
		move    y:(r0),A1
        move    #>READMODE,b                ; A0 = file open attribute (Read Only)
		;iHandle = FastOpen(g_CurrentSongFastkey,(_packed char *)"r");
		jsr FFSFastOpen
  else
        move    a,y:CurrentSongBad           ; need to mark current song not bad
        move    y:CurrentDirDevId,a2         ; card number for current directory
        move    #Fg_CurrentSongName,a1          ; base address of song name
        or      x0,a                         ; A2 = dev#, A1=memspace/file name buffer, A0=0
        move    #>READMODE,a0                ; A0 = file open attribute (Read Only)
        jsr     FILEOpen
  endif

        jcs     _exit
        move   a1,y:WmaDecoderFileHandle2    ; Duplicate Handle
        move    x:DecoderCSR,x0
        bset    #FileIsOpen,x0
        move    x0,x:DecoderCSR              ; keep track of an open song

        ;-------------------------------------------------------------
        ; Seek to Zero (init) the Song (FileSeek)
        ;-------------------------------------------------------------
        ; Inputs:   A2 = Handle  A1/A0 = relative offset
        ;-------------------------------------------------------------
        jsr     SysAllLinear
        clr     a                            ; offset of 0
        move    y:WmaDecoderFileHandle,a2
        jsr     FileSeek
        jcs     _exit

        ; Make sure to run the initialization routine.
        clr     A
        move    A,y:WmaInitFlag
        move    A,y:JanusDRMLicExpired
        move    A,y:HasJanusDRMLICFail
        and     #>$FE,ccr
_exit
        rts


;-----------------------------------------------------------------
; DecoderCloseSong
; This function will use the WmaDecoderFileHandle to close the file.
;-----------------------------------------------------------------
DecoderCloseSong
        move    x:DecoderCSR,x0
        jclr    #FileIsOpen,x0,_notOpen         ; do not call FileClose if song file is not open
        ;-------------------------------------------------------------
        ; Close the Song (FILEClose)
        ;-------------------------------------------------------------
        ; Inputs:   A2 = File handle
        ; Output:
        ;           A1 = File handle (if successful)
        ;           A1 = Error code  (if failed)
        ;-------------------------------------------------------------
        jsr     SysAllLinear
        clr     a
        move    y:WmaDecoderFileHandle,a2       ; pass the file handle to the function
        jsr     FILEClose
        clr     a
        move    y:WmaDecoderFileHandle2,a2       ; pass the file handle to the function
        jsr     FILEClose
;_fileClosed
        move    x:DecoderCSR,x0
        bclr    #FileIsOpen,x0
        move    x0,x:DecoderCSR              ; prevents attempts to close a file multiple times w/out opening
_exit
        ;*** TBD : clear song parameters?
        rts
_notOpen
        and     #>$FE,ccr
        jmp     _exit


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
	if (@def('SYNC_LYRICS'))
		jcs		_exitCheckAB_err
        move    #>MsgUpdateLyricsAB,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage
	else
        rts
	endif
_exitCheckAB
        and    #>$FE,ccr
_exitCheckAB_err
        rts

;--------------------------------------------------
DecABModeDisable
; Disables the AB Mode by clearing the bits and resetting byte positions to 0.
; Sends message to MENU to turn off AB icons.
        bclr    #ABMode_A,x:DecoderCSR  ; disable AB Mode
        bclr    #ABMode_B,x:DecoderCSR
        move    y:<Const_000000,x0      ; reset memory locations
        move    x0,y:WmaABPositionAHigh
        move    x0,y:WmaABPositionALow
        move    x0,y:WmaABPositionBHigh
        move    x0,y:WmaABPositionBLow

        ; post msg to menu to remove AB icon
;        move    #>MsgDecoderABModeOff,r0
;        move    y:<Const_ffffff,m0
;        jsr     SysPostMessage
        rts

DecGetABModeBPos
; Returns: B1 = WmaABPositionBHigh , B0 = WmaABPositionBLow
        clr     b
        move    y:WmaABPositionBHigh,b1
        move    y:WmaABPositionBLow,b0
        rts

DecSetSongPosA
        move    x:DecoderCSR,x0
        jset    #ABQuiet,x0,_skip
        jsr     StopCurrentSong
        jcs     _exit
        bclr    #Stop,x:DecoderCSR
_skip

        ; Seek to Zero in File
        jsr     SysAllLinear
        clr     a
        move    y:WmaDecoderFileHandle,a2
        jsr     FileSeek
        jcs     _exit
;        jcc     _atZero
;        error
;_atZero
; Sets the song position to AB position "A"
        clr     a
        move    y:WmaABPositionAHigh,a1
        move    y:WmaABPositionALow,a0
        move    a1,y:SongByteTotalHigh
        move    a0,y:SongByteTotalLow
        move    y:WmaDecoderFileHandle,a2
        jsr     FileSeek
        jcs     _exit
;        jcc     _atALocation
;        error
;_atALocation
        jsr     DecoderUpdateCurrentTime

        move    x:DecoderCSR,x0
        jset    #ABQuiet,x0,_exit          ; do NOT post PLAY msg if FFWDing

        ; post message to self (decoder) to Play
        move    #>PlayMessage,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage
        and     #>$FE,ccr
_exit
        rts

; Temporary. Till we get the parser to wade through the headers.


SongInfoCpy
    move y:<Const_000002,n0     ; WMA Characters are located in every alternate word
    do  #10,_EndCpy
        move x:(r0)+n0,A
        move x:(r0)+n0,y0
        mpy  #16,y0,B
        move B0,y0
        or  y0,A        x:(r0)+n0,y0
        mpy  #8,y0,B
        move B0,y0
        or  y0,A
        move A1,y:(r4)+
_EndCpy
        rts


        endsec


Fsdmi_get_pmid
		rts

        if (@def('JANUS')&&@def('CLCD')&&@def('BACKLIGHT')&&!@def('BATTERY_TYPE_LI_ION'))
WMALowBatteryCheck
        move    y:FbWMALowBattery,A
        tst     A
        jeq     SkipWMALowBatt  ; if not low battery, do nothing
        ; Turn off the lcd backlight directly
        ; Take care that if the LCD driver is changed, e.g. not PWM2 for the backlight,
        ; Following also has to be changed accordingly.
        ; These 2 lines are copied from the function HALDisplayBacklightOff(void) in the lst file.
        move    #$2000,b1
        move    b1,x:$FA36
SkipWMALowBatt
        rts
        endif
