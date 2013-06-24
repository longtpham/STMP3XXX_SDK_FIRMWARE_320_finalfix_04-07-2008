;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2003
; Filename: decoder_overlay.asm  (MPEG Audio - used with layers 1,2,3)
; STMP3 User-level Decoder Module (Can be overlayed)        
;///////////////////////////////////////////////////////////////////////////////
        opt     mex

        section Audible_DECMOD_DecoderOverlay


;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////
;        nolist
    include "project.inc"
    include "resource.inc"
    include "sysequ.inc"
    include "sysspeed.inc"
    include "hwequ.inc"
    include "dacequ.inc"
    include "msgequ.inc"
    include "decoderequ.inc"
    include "mixerequ.inc"
    include "stack.xref"
    include "stackmem.xref"
    include "audiblemp3dec.inc"
    include "sysresources.inc"
    include "systemdefs.inc"
    include "const.xref"
    include "audibledecodermem.xref"
    include "audiblemp3mem.xref"
    include "dacmem.xref"
    include "dacmemy.xref"
    include "dacdrv.xref"
    include "sysmisc.xref"
    include "bufdesc.xref"
    include "sysresource.xref"
    include "audibledecodermem.xref"
    include "mixer.xref"
    include "systime.xref"
    include "syscoder.xref"
    include "systimers.xref"
    include "audible_decoder_overlay.xref"


        page    132,60,0,0,0



;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////
        GLOBAL  AudibleMp3DecoderModule
        GLOBAL  AudibleMp3DecoderModuleInit

;///////////////////////////////////////////////////////////////////////////////
;   External References
;/////////////////////////////////////////////////////////////////////////////// 
        EXTERN   SysLoadResource
        EXTERN   SysFlushCoderMemory
        EXTERN   AudibleMp3DecReset,AudibleMp3DecInit,AudibleMp3DecProcess
        EXTERN   AudibleMpegDecFree_SyncThresholdBase,AudibleMpegDecSet_SyncThresholdBase
        EXTERN   Fg_CurrentSongName,AudibleMp3DecoderFileHandle
        EXTERN   CurrentDirDevId
        EXTERN   AudibleDecoderModuleTimerDisable
        EXTERN   AudibleMp3DecoderModuleIsr
        EXTERN   SysEnableTimer
        EXTERN   TotalSongCount
        EXTERN   AudibleDecoderReadStatus
        EXTERN   Fbitrate_o

        EXTERN    FAudible_MP3Reset
        EXTERN    FAudible_MP3FileOpen
        EXTERN    FAudible_MP3FileClose
        EXTERN    FAudible_MP3FileSeek
        EXTERN    FAudible_MP3FilePos
        EXTERN    FAudible_MP3NextChapter
        EXTERN    FAudible_MP3PrevChapter
        EXTERN    FAudible_MP3GetFileSize
		EXTERN	  FAudible_MP3UpdateChapterNumber
		EXTERN	  Fg_bAudibleNeedFileSize

        EXTERN    ModuleSetTimer

        EXTERN    MsgDecoderStateChange
        EXTERN    MsgMenuSongTimeChange
		EXTERN	  MsgMenuSetSongPosition
		EXTERN	  MsgMenuUpdateChapterInfo
        EXTERN    FBass_Enhance_o,FBass_Freq_o,FTreble_Enhance_o,FTreble_Freq_o

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
        EXTERN    Mp3DecGetSongPos        
        EXTERN    SysSetupDecVectors
        EXTERN    NEXT_SONG_FFWD

        ; AB Mode References
        EXTERN    MsgDecoderABModeOff
        EXTERN    DecoderProcessPtr
        EXTERN    MsgParserDeviceEnum
        EXTERN    PlayMessage

        EXTERN    MsgGeqSetCoef
        EXTERN    MsgGeqParam
        EXTERN    Fsampfreq_o
		extern   Fg_wBitRateFound		

;	EXTERN    RSRC_DECMOD_CODE
;	EXTERN    RSRC_MP3Y_CODE
;	EXTERN    RSRC_MP3X_CODE
;	EXTERN    RSRC_MP3P_CODE

;	EXTERN    MEM_P_ORG_SYS

    if (@def('DEVICE_3400')||@def('DEVICE_3300'))
        EXTERN  F_lc_u_bP_sys_cluster
        EXTERN  F_lc_u_bP_app_cluster
	    EXTERN  F_lc_u_eP_app_cluster
        EXTERN  F_lc_u_bX_sys_cluster
        EXTERN  F_lc_u_eY_phys
        EXTERN  F_lc_u_bY_app_cluster
    else
    ; WMA Build
        EXTERN  F_lc_u_bP_app_cluster
        EXTERN F_lc_u_bY_app_cluster;
        EXTERN F_lc_u_e_MP3_P_buf;
        EXTERN F_lc_u_b_MP3_P_buf;
        EXTERN F_lc_u_e_MP3_X_buf;
        EXTERN F_lc_u_b_MP3_X_buf;
        EXTERN F_lc_u_e_MP3_Y_buf;
        EXTERN F_lc_u_b_MP3_Y_buf;
    endif
    EXTERN    F_lc_u_bX_app_cluster;
    ;EXTERN    FileSeek
    EXTERN    SysSpeedIncrease
    EXTERN    SysSpeedClockFree
    EXTERN    EffectsInit	
    EXTERN    DacClearMemory
    EXTERN    AudibleMp3GetInvBitRate
    EXTERN    FAudibleMp3DecoderStartFrameFilePosition
    
    EXTERN    AccumulatedMSecs
    EXTERN    AccumulatedSecs
    EXTERN    DeltaTimeStampMsec
    
    EXTERN    VBRSignature
    EXTERN    Fg_wVBRTOCTable
    EXTERN    Fg_VBRGlobalData
    EXTERN    Rdiv_uiuiui
;///////////////////////////////////////////////////////////////////////////////
;  Macros
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "sysmacro.asm"
    include "systrace.asm"
    list

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
       org      p,"Audible_DECMOD_DecoderOverlay_overlay_P":
      
        ; Resource number
        dc      RSRC_AUDIBLE_DECMOD_CODE

;*******************************************************************************
; Name:        AudibleMp3DecoderModuleInit
; Description: Init decoder variables. Init decoder ISR & enable DAC ISR.
; Inputs:      
; Outputs:     A = EVENT_MESSAGE to wait and call this module's process 
;                  function when msg is received.
; Notes:       
;*******************************************************************************
AudibleMp3DecoderModuleInit
        ; save off stack
        move    #_stackSaved,r6              ; setup the return address
        move    #UserScratchY,r5             ; pointer to the SP local save
        jmp     SaveSystemStack              ; save the hardware stack
_stackSaved

        ; flush decoder memory
;        jsr     SysFlushCoderMemory

        ; init the ModuleTable process pointer
        move    #AudibleMp3DecoderModule,x0
        move    x0,y:DecoderProcessPtr

;    if (@def('DEVICE_3400')||@def('DEVICE_3300'))
;        ; Load the MP3 decoder into P memory
;        move    #>RSRC_AUDIBLE_MP3P_CODE,x0             ; Resource number
;        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
;        move    #>(F_lc_u_eP_app_cluster-F_lc_u_bP_app_cluster)*3,y1 ;ov sz
;        move    #>F_lc_u_bP_app_cluster,y0      ; Address of the overlay area
;        move    #TARGET_MEM_P,a1                ; Overlay area is in P memory
;        jsr     SysLoadResource
;
;        ; Load the MP3 decoder into X memory
;        move    #>RSRC_AUDIBLE_MP3X_CODE,x0             ; Resource number
;        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
;        move    #>(F_lc_u_bX_sys_cluster-F_lc_u_bX_app_cluster)*3,y1 ;overlay sz 
;        move    #>F_lc_u_bX_app_cluster,y0      ; Address of the overlay area
;        move    #TARGET_MEM_X,a1                ; Overlay area is in X memory
;        jsr     SysLoadResource
;
;        ; Load the MP3 decoder into Y memory
;        move    #>RSRC_AUDIBLE_MP3Y_CODE,x0             ; Resource number
;        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
;        move    #>(F_lc_u_eY_phys-F_lc_u_bY_app_cluster)*3,y1 ; overlay area sz
;        move    #>F_lc_u_bY_app_cluster,y0      ;  Address of the overlay area
;        move    #TARGET_MEM_Y,a1                ; Overlay area is in Y memory
;        jsr     SysLoadResource
;
;    else
;        ; Load the MP3 decoder into P memory
;        move    #>RSRC_AUDIBLE_MP3P_CODE,x0             ; Resource number
;        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
;        move    #>(F_lc_u_e_MP3_P_buf-F_lc_u_b_MP3_P_buf)*3,y1 ; overlay area sz
;        move    #>F_lc_u_b_MP3_P_buf,y0      ; Address of the overlay area
;        move    #TARGET_MEM_P,a1                ; Overlay area is in P memory
;        jsr     SysLoadResource
;
;        ; Load the MP3 decoder into X memory
;        move    #>RSRC_AUDIBLE_MP3X_CODE,x0             ; Resource number
;        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data                  
;        move    #>(F_lc_u_e_MP3_X_buf-F_lc_u_b_MP3_X_buf)*3,y1 ; overlay area sz
;        move    #>F_lc_u_b_MP3_X_buf,y0      ; Address of overlay area
;        move    #TARGET_MEM_X,a1                ; Overlay area is in X memory
;        jsr     SysLoadResource
;
;        ; Load the MP3 decoder into Y memory
;        move    #>RSRC_AUDIBLE_MP3Y_CODE,x0             ; Resource number
;        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
;        move    #>(F_lc_u_e_MP3_Y_buf-F_lc_u_b_MP3_Y_buf)*3,y1 ; overlay area sz
;        ;Stmp00003952
;	;move    #>F_lc_u_bY_app_cluster,y0      ;  Address of the overlay area
;	move    #>F_lc_u_b_MP3_Y_buf,y0      ;  Address of the overlay area
;        move    #TARGET_MEM_Y,a1                ; Overlay area is in Y memory
;        jsr     SysLoadResource
;
;    endif   ; if (@def('DEVICE_3400')||@def('DEVICE_3300'))

;		jsr     LoadAudibleMP3Decoder

        ; EffectsInit should be called before calling any DAC initialization to ensure the processing chain is loaded.
        jsr     EffectsInit

        ; Init the interrupt vectors
        move    #>DacDriverIsr,x0
        move    #>DacDriverIsr,x1
        move    #>AudibleMp3DecoderModuleIsr,y0
        jsr     SysSetupDecVectors

        ; restore stack
_restoreStack
        move    #UserScratchY,r5             ; pointer to the SP local save
        move    #_stackRestored,r6           ; setup the return address
        jmp     RestoreSystemStack           ; restore the hardware stack
_stackRestored

_exitInit

        ;ignore messages until the Decoder is reset
        bset    #DecoderIgnoreMessages,x:DecoderSR

        ; wait to call the decoder module's process routine until we get a message
        move    #>EVENT_MESSAGE,A

        rts

;*******************************************************************************
; AudibleMp3DecoderModule
;*******************************************************************************
AudibleMp3DecoderModule
        ; save off stack
        move    #_stackSaved,r6              ; setup the return address
        move    #UserScratchY,r5             ; pointer to the SP local save
        jmp     SaveSystemStack              ; save the hardware stack
_stackSaved

_InitEventValue
        move    #>EVENT_MESSAGE,a1
        move    #>500,x0
        jsr     ModuleSetTimer
        move    a1,y:AudibleMp3DecoderEventValue

_ProcessSysMessages
        ; switch on system messages
        move    #_queueProcessed,r6
        jmp     DecoderProcessQueue
_queueProcessed

_UpdateLcdMessage
_checkForSync
        move    x:DecoderCSR,x0              ; copy contents of status register
        jset    #FileReadError,x0,_FileReadError 
        ; if decoder is not in sync yet, don't update current time
        jclr    #DecSync,x0,_LCDdone 
        jclr    #Play,x0,_LCDdone            ; only update if playing (save MIPS)
        jset    #SongInfo,x0,_getSongCurrentTime
        ; prevents constant updating of same song info
        bset    #SongInfo,x0      
        move    x0,x:DecoderCSR ;SongInfo bit is cleared by opening new song file

        ; Sync found - Playing, store PlayTime Start.
        ; NEW_TIME
    if (1)                          ; New VBR Additions    
        jsr     SysGetCurrentTime           ; function returns time in ms in a
        move    a0,y:DeltaTimeStampMsec     ; Store Play time origin - when pressed Play & Sync found.
    endif

        ; If synced, update the GeQ for sample rate.
        move    #>MsgGeqSetCoef,r0
        move    x:Fsampfreq_o,x0
        move    x0,y:MsgGeqParam
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

_getSongTotalTime
        ; NEW_TIME - VBR time
_getSongCurrentTime
; check to see if non-vbr file.  If so, use old method.
; Note - we still would like to process U (Unknown) VBR files with VBR method
;        but FFWD and RWND won't work properly.
        move    y:VBRSignature,a
        move    y:<Const_000000,x0          ; See if non-VBR
        cmp     x0,a
        jeq     _GetSongTimeNotVBR          ; non-VBR file
        jsr     UpdateVBRTime               ; Update VBR time.
        jmp     _GetSongTimeDone            ; See if an update is needed.        
        
_GetSongTimeNotVBR
        jsr     DecoderUpdateCurrentTime
        
_GetSongTimeDone
_LCDdone

_checkABModeBoundaries
        move    x:DecoderCSR,x0
;****************************************************
; Next line is the reason FFWD through AB doesn't work!!
;****************************************************
        ; must check Pause bit in case "just Paused" now
        jset    #Pause,x0,_CheckIfEndOfList               
; FSERROR:
        jclr    #ABMode_A,x0,_CheckIfEndOfList
        jsr     CheckABBoundaries
        jcs     _handleCheckABBoundariesError
;        jsset   #ABMode_A,x0,CheckABBoundaries  ;if ABMode on, check boundaries

_CheckIfEndOfList
        move    x:DecoderCSR,x0
        jset    #EndOfList,x0,_exitMp3DecoderModule
_CheckIfEndOfSong
         move   x:DecoderCSR,x0
         jclr   #EndOfSong,x0,_exitMp3DecoderModule

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

        ; Tell the PSM/PlayerLib to advance to next song and Play
        move    #>NEXT_SONG_PLAY_EOF,x0      ; EOF && PLAY
        move    x0,y:MsgParserNextParam
        move    #>MsgParserNextSong,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage
        
        move    #>SPEED_CLIENT_AUDIBLE_MP3,y0
        jsr     SysSpeedClockFree             ;Free DCLK for other speed clients

        ; Ignore messages until the Decoder receives a reset message
        bset    #DecoderIgnoreMessages,x:DecoderSR

        ; Allow the PSM/PlayerLib to call it next (if songs are left)
        move    #>EVENT_MESSAGE,x0
        move    x0,y:AudibleMp3DecoderEventValue    

_exitMp3DecoderModule
        ; restore stack
        move    #UserScratchY,r5             ; pointer to the SP local save
        move    #_stackRestored,r6           ; setup the return address
        jmp     RestoreSystemStack           ; restore the hardware stack
_stackRestored

        ; Return value for executive (EVENT_NONE or EVENT_MESSAGE)
        move     y:AudibleMp3DecoderEventValue,a1   
        move     y:<Const_ffffff,m1          ; make sure that m1 is -1!

        rts

        ;-----------------------------------------------------------------------

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

        ; Free DCLK for other speed clients
        move    #>SPEED_CLIENT_AUDIBLE_MP3,y0       ; speed client ID
        jsr     SysSpeedClockFree

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AudibleMp3DecoderEventValue
        jmp     _exitMp3DecoderModule
        ;-----------------------------------------------------------------------
        
DecoderSetEventTimer
; If we want to return on the module timer OR a message,
; this routine sets AudibleMp3DecoderEventValue up.
        move    #>EVENT_MESSAGE,a1
        move    #>500,x0
        jsr     ModuleSetTimer
        move    a1,y:AudibleMp3DecoderEventValue
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
        bclr    #SyncWait,x:DecoderCSR
        bset    #Stop,x:DecoderCSR

_pollPlaybitStop
        move    x:DecoderCSR,x0
        jset    #Play,x0,_pollPlaybitStop ; wait for isr to enter "Stop" state
        ; isr will turn off Timer Module before returning

        ; Fade the Volume to Mute
        move    #>msFadeOut,x0
        jsr     MixerMasterFadeOut      ; fade volume down to mute
_muted
        bclr    #BadFile,x:DecoderCSR   ; Allow next good song to play normally

        ; EffectsInit should be called before calling any DAC initialization to ensure the processing chain is loaded.
        jsr     EffectsInit
        
        jsr     DacDriverDisablePort

        ; clear dac memory
		jsr		DacClearMemory
        ; Set the status register
        bset    #Stop,x:DecoderSR
        bclr    #Pause,x:DecoderSR
        bclr    #Play,x:DecoderSR
        bclr    #VBRFlag,x:DecoderSR

        ;  $418937   ;1/8000*8*2^9
        move    #>$418937,x0        ; Corresponds to 8 kbps.
        move    x0,y:SongInvBitRate

        ; force success. Returns with CCR Carry bit clear indicating SUCCESS
        and     #>$FE,ccr
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
; Sends a message to the LCD to print them if they have changed. TIME_FIX
; Use precise song bytes consumed count instead of SongByteTotalLow fread count.
; NEW_TIME - Use a timer to keep track of time.  Increment seconds when necessary.
; Hook added for VBR files with no header information = 'U' instead of V or X.
	if (0)									; mmiu - Don't worry about this for now. Audible content not VBR.
        move    #>$000055,x0                 ; See if VBR but unknown 'U'
        move    y:VBRSignature,a
        cmp     x0,a
        jeq     _NotVBR                     ; VBR - but corrupt data.
        move    y:<Const_000000,x0          ; See if non-VBR
        cmp     x0,a
        jeq     _NotVBR                     ; non-VBR file
        jsr     CalculateCurrentTimeVBR    ; updates the display of the elapsed/remaining song time
        move    y:AccumulatedSecs,a0        ; Load Accumulated Seconds.
        jsr     SecondsToMinSecs            ; Convert total Seconds to Seconds & Minutes.
        jmp     _checkIfUpdateNeeded        ; See if an update is needed.
        
_NotVBR        
    endif
        move    #SongByteTotalConsumedHigh,r4
        move    #SongByteTotalConsumedLow,r5
        move    y:<Const_ffffff,m4
        move    m4,m5
        jsr     GetSongTime
_checkIfUpdateNeeded
        ; _update time display if calculated minutes != current display minutes
        move    y:SongCurrentMinutes,a
        cmp     x1,a
        jne     _update

        ; _update time display if calculated seconds != current display seconds
        move    y:SongCurrentSeconds,a
        cmp     x0,a
        jne     _update                      
        jmp     _ExitDecoderUpdateCurrentTime ; No change, don't _update display

_update
         move    x1,y:SongCurrentMinutes
         move    x0,y:SongCurrentSeconds

         move    #MsgMenuSongTimeChange,r0
         move    y:<Const_ffffff,m0
         jsr     SysPostMessage

		 jsr	 FAudible_MP3UpdateChapterNumber

		 ; tell the menu module to update chapter info
         move    #MsgMenuUpdateChapterInfo,r0
         move    y:<Const_ffffff,m0
         jsr     SysPostMessage

_getSongRemainTime
         jsr     GetSongRemainTime

_ExitDecoderUpdateCurrentTime
        rts

;-------------------------------------------------
GetSongTime
; Inputs:  R4 = HighByte count ptr , M4 = linear
;          R5 =  LowByte count ptr , M5 = linear
; Output:  X1 = Song Minutes
;          X0 = Song Seconds
;
; You can call this to get total song length or for current song position.
; Currently, this code is set up to work with the MP3 decoder only.
; This will only work when the decoder has synced up already.
        push    r0
        move    y:SongInvBitRate,y0       ; Current inv bit rate multiplier
        move    y:(r4),x1                    ; high byte count
        move    y:(r5),x0                    ; low byte count

        clr     A
        move    x0,A0
        move    x1,A1
        tst     A
        jeq     _Continue_Zero
        move    #>FAudibleMp3DecoderStartFrameFilePosition,r0
		nop									; mmiu - Added to get rid of compiler warning
        move    y:(r0)+,x0
        move    y:(r0)+,x1
        sub     x,A
        move    A0,x0
        move    A1,x1
_Continue_Zero
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

    if (1)
        move    a0,y:AccumulatedSecs        ; Update Accumulated Seconds.
        jsr     SecondsToMinSecs            ; Convert total Seconds to Seconds & Minutes.

        ; At this point, we have A1:A0 = total integer seconds
        pop     r0

        rts
    endif        
;-------------------------------------------------
SecondsToMinSecs
; Inputs:  A1:A0 = total Integer Seconds.
;       
; Output:  X1 = Song Minutes
;          X0 = Song Seconds
;
; This converts the total integer seconds in A to Seconds/Minutes.

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
; Name:        DecoderProcessQueue
;
; Description: Processes msg by executing corresponding msg handling code segment.
;              _JumpTable is used to find msg handler sub function. (ordered)
;              Message handling sub functions exit to _NextMessage, 
;              others arrive there eventually to remove one msg from the queue.  
;
; Inputs:      r0 = Descriptor for this modules message queue (in y memory)
;              m0 = -1 for linear addressing
;              r6 = return address
;
; Outputs:     _NextMessage eventually pops r6 return address and jumps to it.
;
; Notes:       See _JumpTable label matching.
;*******************************************************************************
DecoderProcessQueue
        push    r6                        ; needed for return

        ; Check if waiting in sync mode
        move    x:DecoderCSR,x0
        jset    #SyncWait,x0,_WaitForSync   ; See play msg handler
_SyncCheckReturn
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
; debug ; testpoint: see b1 for msg ID  ;//dbgmpg
        ; Verify that the message ID is valid by checking message range
        move    #>DECODER_FIRST_MSG_ID,x0
        cmp     x0,b
        jlt     _InvalidMessageID
        move    #>DECODER_LAST_MSG_ID,x0
        cmp     x0,b
        jgt     _InvalidMessageID
        ; Message ID is in valid range

        ;We always want to process DECODER_RESET
        move    #>DECODER_RESET,x0
        cmp     x0,b
        jeq     _ContinueProcessing

        ;If we're supposed to be ignoring messages, lets skip to the next message
        move    x:DecoderSR,x0
;    btst #DecoderIgnoreMessages,x0       ;//dbgmpg
;    debugcs  ; WE'RE IGNORING THIS MESSAGE
        jset    #DecoderIgnoreMessages,x0,_NextMessage

_ContinueProcessing
        ; Check for case of no songs to play
        move    y:TotalSongCount,a
        tst     a
        jne     _songsPresent
        jsr     DecABModeDisable        ; reset any AB mode settings
        move    #>EVENT_MESSAGE,x0      ; no songs avail -- only call on button push
        move    x0,y:AudibleMp3DecoderEventValue
        jmp     _NextMessage            ; ignore messages - no songs!

_songsPresent
        ; Point r1 to the first argument of the message
        move    (r1)+
        move    (r1)+

        ; Clear the upper byte of the message ID
        move    #$00FFFF,x0
        and     x0,b

        ; Put it to linear before processing any messages
        jsr     SysAllLinear

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
        
;-------------------------dirPlaylist Function-------------------------
_dirPlaylist
        ; Get the arguments
        pop     r1
        nop
        move    y:(r1)+,a2              ; device (card) number
        move    y:(r1)+,a1              ; memory space and ptr to path

        ; store the playlist information
        ;move    a2,y:CurrentPListDevId  ; store the directory device id
        ;move    a1,y:CurrentPListPathPtr ; store memory space and ptr to path

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
        ; Clear the bad song flag before beginning to Play
        clr     a
        move    a,y:CurrentSongBad

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
        jset    #SyncWait,x0,_GoToPause
        jclr    #Play,x0,_beginPlay     ; if playing, switch to Pause else _beginPlay
_GoToPause
        bset    #Pause,x:DecoderCSR     ; msg to isr to "Pause"
        bclr    #SyncWait,x:DecoderCSR
        ; we need to be absolutely sure that the timer module will always be 
        ; enabled before this polling mechanism!
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

        ; EffectsInit should be called before calling any DAC initialization to ensure the processing chain is loaded.
        jsr     EffectsInit
        
        jsr     DacDriverDisablePort    ; turn off the DAC

        ;========================================================
        ;========================================================
        ; ADJUST the file pointers for the PAUSE state
        ; Added 20FEB03-JG
        ; Determine how much data LEFT OVER in SOURCE buffer
        ; The source buffer is cleared out when the PLAY message is received, therefore, we
        ; need to "rewind" the file pointer by the amount of data left unused in the input
        ; buffer.
        move    y:AudibleMp3DecoderSourceBufDesc,r0
        move    y:<Const_ffffff,m0
        jsr     GetFilledSpace              ; returns: a==number of words in buffer
        move    y:<Const_000003,x0
        move    a,x1
        mpy     x0,x1,a                     ; get number of bytes (from words)
        asr     a                           ; fix for integer arithmetic
        move    a0,x0                       ; x0 = num bytes left in buffer
        move    y:<Const_000000,x1          ; clear x1 for 48-bit subtract

        ; Calculate Song Byte Position
        clr     a
        move    y:SongByteTotalLow,a0       ; get byte position in file
        move    y:SongByteTotalHigh,a1      ;   before the adjustment
        sub     x,a                         ; a = adjusted byte position
        jge     _GreaterThanZero
        clr     a                           ; to prevent negative offset into file
_GreaterThanZero
        move    a0,y:SongByteTotalLow       ; store new "adjusted" value
        move    a1,y:SongByteTotalHigh

; 2/8/2005 mmiu - Remove code to seek. Not necessary to seek here.
;        ; Adjust the file pointer in the file using FileSeek adjustments
;        ;seek to the start of the file.
;        jsr     SysAllLinear
;        clr     a                            ; offset of 0
;        ;move    y:AudibleMp3DecoderFileHandle,a2
;        ;jsr     FileSeek                     ; rewind to beginning
;
;        ; 10/5/2004 mmiu - Redirect general FileSeek with FAudible_MP3FileSeek
;        ;jsr     FAudible_MP3FileSeek         ; rewind to beginning
;
;        ; add offset to the start of the audio data.
;        ; use the Start Position - found from MP3MetaData scan.
;        move    #>FAudibleMp3DecoderStartFrameFilePosition,r0
;        nop
;        move    y:(r0)+,x0
;        move    y:(r0)+,x1
;        ; Add the current offset in the file.
;        ;seek to the start of the data.
;        clr     a
;        move    y:SongByteTotalLow,a0
;        move    y:SongByteTotalHigh,a1
;        add     x,a
;        tst     a
;        jeq     _Done
;        ;move    y:AudibleMp3DecoderFileHandle,a2
;        ;jsr     FileSeek                    ; seek to the new position in the file
;
;        ; 10/5/2004 mmiu - Redirect general FileSeek with FAudible_MP3FileSeek
;        ;jsr     FAudible_MP3FileSeek
_Done
        ;========================================================
        ;========================================================

        ; On pause, Free DCLK for other speed clients
        move    #>SPEED_CLIENT_AUDIBLE_MP3,y0     ; speed client ID
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
        move    x0,y:AudibleMp3DecoderEventValue
        jmp     _NextMessage

_beginPlay
        move    x:DecoderCSR,x0
        bclr    #Pause,x0
        ; Necessary to notify decoder module to update song info (new song)
        bclr    #SongInfo,x0            
        ; Inform decoder module that sync has not been found yet
        bclr    #DecSync,x0             
        move    x0,x:DecoderCSR
        ; Init g_Mp3DataSrc_mbuf to empty (reset Head/Tail)
        ;jsr     InitInputBuffer         

; 10/8/2004 mmiu - Looks like old code. Causes crash now that we load decoder later.
;        move    r7,x:AudibleMp3DecoderSaveR7   ; temp -- later need to strip r7 out of the routines
;        move    m7,x:AudibleMp3DecoderSaveM7   ; temp -- later need to strip m7 out of the routines

        ; Increase speed to SPEED_MAX. Allows faster sync times.
        ; After syncing, speed is dropped to SPEED_MP3 for bitrates < 256 kbps.
        move    #>SPEED_MAX,x1     
        move    #>SPEED_CLIENT_AUDIBLE_MP3,y0  ; Speed client identifier
        jsr     SysSpeedIncrease                ; Manages clock speed clients

		; TEST! TEST! TEST!
        ; Load the MP3 decoder into P memory
        ;move    #>RSRC_AUDIBLE_MP3P_CODE,x0             ; Resource number
        ;move    #>RSRC_TYPE_DATA,x1                     ; Resource type is data
        ;move    #>(F_lc_u_e_MP3_P_buf-F_lc_u_b_MP3_P_buf)*3,y1 ; overlay area sz
        ;move    #>F_lc_u_b_MP3_P_buf,y0                 ; Address of the overlay area
        ;move    #TARGET_MEM_P,a1                        ; Overlay area is in P memory
        ;jsr     SysLoadResource
		; TEST! TEST! TEST!

        jsr     DecoderOpenSong                 ; WARNING: includes FileSeek(0)
        jcc     _OpenSuccess

        ; testpoint   Can insert optional halt here to catch DecoderOpenSong errors.

; 10/8/2004 mmiu - Looks like old code. Causes crash now that we load decoder later.
;        move    x:AudibleMp3DecoderSaveM7,m7 ; Restore r7,m7 on DecoderOpenSong error. 
;        move    x:AudibleMp3DecoderSaveR7,r7

        ; DecoderOpenSong Error: Free DCLK for other speed clients
        move    #>SPEED_CLIENT_AUDIBLE_MP3,y0
        jsr     SysSpeedClockFree   

        jmp     _Stop
_OpenSuccess

        ; FileSeek to SongByte position (for Bookmarks) and handle REWIND over boundary
;_BookMarkCode ; Block temporarily removed since it caused unpause issue and 
               ; bookmark feature not ready yet. 
               ; Will be reincarnated when scheduled. For now, MP3 bookmarks not supported.

_UpdateTime
        ; Init g_Mp3DataSrc_mbuf to empty (reset Head/Tail)
        jsr     InitInputBuffer         
        clr     A
        ;move    y:AudibleMp3DecoderFileHandle,a2
        ;jsr     FileSeek       
        ; 10/5/2004 mmiu - Redirect general FileSeek with FAudible_MP3FileSeek
        ;jsr     FAudible_MP3FileSeek

        move    y:SongByteTotalHigh,A
        move    y:SongByteTotalLow,A0

;        move    y:AudibleMp3DecoderFileHandle,a2
;        jsr     FileSeek       
        ; 10/5/2004 mmiu - Redirect general FileSeek with FAudible_MP3FileSeek
        jsr     FAudible_MP3FileSeek
        jsr     DecoderUpdateCurrentTime     ; 0:00 / ----

   		; EffectsInit should be called before calling any DAC initialization to ensure the processing chain is loaded.
        move	#>g_AudibleMp3DecSinkBufDescriptor,A
		move	#>TARGET_MEM_Y,B
		move	y:<Const_000001,x0							; Always Stereo output
		jsr		EffectsInit
        jsr     AudibleMp3DecInit

        ; Init dac and decoder for new song
        jsr     DacDriverInit
        jsr     DacDriverEnablePort   ; set up the DAC control registers, enable the port
        ; Enable the DAC interrupt
        bset    #HW_DACCSR_TXIEN_BITPOS,x:HW_DACCSR
		
; 10/8/2004 mmiu - Looks like old code. Causes crash now that we load decoder later.
;        move    x:AudibleMp3DecoderSaveM7,m7
        ; Assertion testpoint for r7 and AudibleMp3DecoderSaveR7 in valid range        
;        move    x:AudibleMp3DecoderSaveR7,r7

        move    #>DecoderTimerNum,x0            ; Turn Timer Module ON
        move    #_TimerEnabled_2,r0
        jmp     SysEnableTimer
_TimerEnabled_2

        ; tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage
        jsr     DecoderSetEventTimer         ; set up DecoderEventValue for module timer

        ; poll for dac on or BadFile (in case bad file encountered)
_WaitForSync
        ; Get DAC state. If DAC on, do vol fade in (since DecSync was achieved).
        move    x:HW_DACCSR,x0
        jset    #HW_DACCSR_TXEN_BITPOS,x0,_FadeIn 
        ; DAC was not ON
    
        move    x:DecoderCSR,x0         ; check for bad file
        jset    #BadFile,x0,_BadFileDetected

        move    #>EVENT_NONE,x0
        move    x0,y:AudibleMp3DecoderEventValue  ; need to be called every time

        move    x:DecoderCSR,x0
        jset    #SyncWait,x0,_SyncCheckReturn
        bset    #SyncWait,x:DecoderCSR  ; signifies we are waiting for sync
        jmp     _NextMessage

_BadFileDetected
        ; #BadFile was set
        bclr #BadFile,x:DecoderCSR      ; reset this bit
        jmp _ExitPlay                   ; conditionally takes one of two exit paths
        ;----------------------


_FadeIn
        bclr    #BadFile,x:DecoderCSR   ; reset this bit

        ; Set clock to audible mp3 rate 
        move    #>SPEED_CLIENT_AUDIBLE_MP3,y0
        jsr     SysSpeedClockFree        

        move    #>SPEED_AUDIBLE_MP3,x1     
		move    #>SPEED_CLIENT_AUDIBLE_MP3,y0   ; Speed client identifier
        jsr     SysSpeedIncrease				; Manages clock speed clients

        ; Fade in the Volume from Mute
        move    #>msFadeIn,x0
        jsr     MixerMasterFadeIn            ; fade volume in to previous level
        
_ExitPlay

        ; Note: Some songs sync immediately, thus SyncWait will not be
        ;       set, so we need to jump to NextMessage and pop the data stack
        move    x:DecoderCSR,x0
        jclr    #SyncWait,x0,_NextMessage
        bclr    #SyncWait,x:DecoderCSR  ; we are done waiting for sync
        jmp     _SyncCheckReturn

;-------------------------Stop Function-------------------------
_Stop
        jsr     DecABModeDisable             ; reset any AB mode settings
        jsr     StopCurrentSong
        jsr     DecoderCloseSong
        ;jsr     DecSetSongPosZero
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
	
        ; On stop, mp3 decoder frees the DCLK speed for other speed clients
        move    #>SPEED_CLIENT_AUDIBLE_MP3,y0     ; Speed client identifier
        jsr     SysSpeedClockFree

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AudibleMp3DecoderEventValue

        jmp     _NextMessage

;-------------------------Ffwd Function-------------------------
_Ffwd
		bset #ABQuiet,x:DecoderCSR
        ; Open File If Necessary
        move    x:DecoderCSR,x0
        jset    #FileIsOpen,x0,_FileOpenedForFfwd

		; mmiu - If file is not open, do not allow user to enter the AA file and ffwd through content.
		; It's more important to preserve file position.
_goToNextSong
        ; Configure CSR
        bclr    #Ffwd,x:DecoderCSR
        bclr    #Rwnd,x:DecoderCSR
        jsr     DecABModeDisable             ; reset any AB mode settings

		; tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        ; When going to next song via ffwd, mp3 decoder frees the DCLK speed for other speed clients
        move    #>SPEED_CLIENT_AUDIBLE_MP3,y0     ; Speed client identifier
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
        move    x0,y:AudibleMp3DecoderEventValue
        jmp     _NextMessage


;		move	#>1,x0									; set for FFwd
;		move	x0,x:Fg_bAudibleNeedFileSize
;       jsr     DecoderOpenSong
;		jcs     _handleOpenSongError
;		; tell the menu module that the status has changed.
;        ; this is necessary to update song name when crossing a boundary
;        move    #>MsgDecoderStateChange,r0
;        move    y:<Const_ffffff,m0
;        jsr     SysPostMessage

_FileOpenedForFfwd
        move    #>DecoderTimerNum,x0
        move    #_TimerDisabled_1,r0
        jmp     SysDisableTimer
_TimerDisabled_1	
        jsr     InitInputBuffer							; set to empty
        ; EffectsInit should be called before calling any DAC initialization to ensure the processing chain is loaded.
		move	#>g_AudibleMp3DecSinkBufDescriptor,A
		move	#>TARGET_MEM_Y,B
		move	y:<Const_000001,x0						; Always Stereo output
        jsr		EffectsInit

        jsr     AudibleMp3DecInit						; init the buffers, reset head/tail ptrs, reset MP3 decoder
        jsr     DacDriverInit
        jsr     AudibleMpegDecFree_SyncThresholdBase

		; FileSeek forward
        jsr     AudibleMp3GetInvBitRate					; needed for songtime calculation
		clr     a
        move    y:AudibleMp3DecoderFfwdRwndValue,a0
        clr     b 
        move    y:AudibleMp3DecoderFfwdRwndValue,b0 
        jsr     DecoderSeek								; FSeeks & Increments SongByteTotal & SongByteTotalConsumed if ok
		
		; Get bytes corresponding to (total song length(seconds) - 10sec)
		move    #>1000,x0
		move    y:Fg_wBitRateFound,y0					; g_wBitRateFound --> Kbps
		mpy		x0,y0,A
		asr		A										; A --> bitrate in bps
		move    A0,y0
		move    #>10,x0
        mpy     x0,y0,B         
        asr     B               
        asr     B               
        asr     B               
        asr     B          
		clr     A
        move    y:SongByteLengthHigh,A1
        move    y:SongByteLengthLow,A0
		sub		B,A										; A --> Bytes corresponding to ( SongByteLength(in bytes) - 10seconds(in bytes) )
        move    y:SongByteTotalHigh,B1
        move    y:SongByteTotalLow,B0					; B --> SongByteTotal 
		cmp     A,B										; if num bytes to skip < num bytes left, ffwd
        jlt     _bytesLeftPassed
_stopFastforwarding
		; if SongByteTotal >= (SongByteLength(in bytes) - 10seconds(in bytes))
		move    A0,y:SongByteTotalLow
        move    A1,y:SongByteTotalHigh
		move    A0,y:SongByteTotalConsumedLow
        move    A1,y:SongByteTotalConsumedHigh
	    
		jsr     FAudible_MP3FileSeek
        jsr     DecoderUpdateCurrentTime				; 0:00 / ----
		
		jmp     _NextMessage

_bytesLeftPassed
		move    B0,y:SongByteTotalConsumedLow
        move    B1,y:SongByteTotalConsumedHigh
        jsr     AudibleMpegDecSet_SyncThresholdBase		;reset base for sync attempt count
        jsr     DecoderUpdateCurrentTime				; 0:00 / ----
	    
		move    #>EVENT_MESSAGE,x0
        move    x0,y:AudibleMp3DecoderEventValue

        jmp     _NextMessage

;-------------------------Rwnd Function-------------------------
_Rwnd
        ; Open File If Necessary
        move    x:DecoderCSR,x0
        jset    #FileIsOpen,x0,_RwndCheckFilePos

		; mmiu - If file is not open, do not allow user to enter the AA file and rewind through content.
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

        ; When going to next song via rwnd, mp3 decoder frees the DCLK speed for other speed clients
        move    #>SPEED_CLIENT_AUDIBLE_MP3,y0     ; Speed client identifier
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
        move    x0,y:AudibleMp3DecoderEventValue
        jmp     _NextMessage


_RwndCheckFilePos
        ; check if too close to beginning of file, if so rwnd into previous song
        clr     a
        move    y:SongByteTotalHigh,a1
        move    y:SongByteTotalLow,a0
        ; TIME_FIX  Continue to use SongByteTotalHigh|Low as this doesn't 
        ; require high precision and only determines when to jump to prev song.
        clr     b
        move    y:AudibleMp3DecoderFfwdRwndValue,b0
        cmp     a,b
        jlt     _rewind
_stopRewinding
        clr     A
		move    A1,y:SongByteTotalHigh
		move    A0,y:SongByteTotalLow
		move    A0,y:SongByteTotalConsumedLow
        move    A1,y:SongByteTotalConsumedHigh
        jsr     FAudible_MP3FileSeek
        jsr     DecoderUpdateCurrentTime     
        jmp     _NextMessage
_rewind
        ; Open File If Necessary
        move    x:DecoderCSR,x0
        jset    #FileIsOpen,x0,_FileOpenedForRwnd
		; mmiu - If file is not open, do not allow user to enter the AA file and ffwd through content.
		; It's more important to preserve file position.
		jmp		_handleOpenSongError

		move	#>2,x0										; set for Rewind
		move	x0,x:Fg_bAudibleNeedFileSize
        jsr     DecoderOpenSong
		jcs     _handleOpenSongError
        ; tell the menu module that the status has changed.
        ; this is necessary to update song name when crossing a boundary
        move    #>MsgDecoderStateChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage
_FileOpenedForRwnd
        jsr     InitInputBuffer								; set to empty
        jsr     AudibleMp3GetInvBitRate						; needed for songtime calculation
        jsr     DecoderUpdateCurrentTime					; 0:00 / ----

        ; EffectsInit should be called before calling any DAC initialization to ensure the processing chain is loaded.
		move	#>g_AudibleMp3DecSinkBufDescriptor,A
		move	#>TARGET_MEM_Y,B
		move	y:<Const_000001,x0							; Always Stereo output
        jsr		EffectsInit
        
        jsr     AudibleMp3DecInit							;init buffers, reset head/tail ptrs, reset MP3 decoder

        clr     a
        move    a,x1
        move    a,y1                    ; Do parallel ops for consumed counter in y and b
        move    y:SongByteTotalHigh,a1  ; high bytes
        move    y:SongByteTotalLow,a0   ; low bytes
        move    y:SongByteTotalConsumedHigh,b1
        move    y:SongByteTotalConsumedLow,b0 

        move    y:AudibleMp3DecoderFfwdRwndValue,x0
        move    y:AudibleMp3DecoderFfwdRwndValue,y0
        sub     x,a                     ; a = location in song to go to (can't FSEEK negative num)
        sub     y,b
        ; TIME_FIX  Inserted subtract and update for SongByteTotalConsumedHigh:Low
        ; as above. This will keep the displayed current time correct on Rewind. 

        ; A is >=0, but floor B to prevent negative SongByteTotalConsumed
        tst     B
        jge     _ConsumedFloorDone
        clr     B
_ConsumedFloorDone

        push    b1  ;Save results of 2 subtractions. SongByteTotalConsumedHigh:Low in B
        push    b0
        push    a1  ; A1:A0 = Future SongByteTotalHigh:Low to seek to
        push    a0

        jsr     AudibleMpegDecFree_SyncThresholdBase ; Free it: sync attempts postponed

        ;; File Seek to byte zero first, for forward seek afterwards.

        ; Restore calculated desired rewind locations for next seek fwd
        clr     a               ; Clear A2 and B2
        clr     b
        pop     a0
        pop     a1     ; A1:A0 = Future SongByteTotalHigh:Low to seek to
        pop     b0
        pop     b1     ; B1:B0 = Future SongByteTotalConsumedHigh:Low to seek to
        move    a1,y:SongByteTotalHigh  ; high bytes
        move    a0,y:SongByteTotalLow   ; low bytes
        move    b1,y:SongByteTotalConsumedHigh
        move    b0,y:SongByteTotalConsumedLow

; FSERROR:        
        ; If seek to zero errored
 ;       jcs     _handleFileSeekError ;Note: prev SongByte count values preserved on seek err

        ; FileSeek to correct file byte offsets in A1:A0 and B1:B0
        ; Also sets global SongByteTotal & SongByteTotalConsumed  
                
;       ; Rewind success. 

        jsr     AudibleMpegDecSet_SyncThresholdBase ; reset base for sync attempt count

        ; Post message to update the current time display
        move    #MsgMenuSongTimeChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AudibleMp3DecoderEventValue

        jmp     _NextMessage

;-------------------------NextSong Function-------------------------
_NextSong
        ; Configure CSR
        bclr    #SyncWait,x:DecoderCSR
        bclr    #Ffwd,x:DecoderCSR
        bclr    #Rwnd,x:DecoderCSR
        jsr     DecABModeDisable             ; reset any AB mode settings
        move    x:DecoderCSR,x0
        jset    #Play,x0,_nextStop
        jset    #Pause,x0,_nextStop
        jsr     DecoderCloseSong

        ; Free DCLK for other speed clients
        move    #>SPEED_CLIENT_AUDIBLE_MP3,y0
        jsr     SysSpeedClockFree

        ; tell the PlayerStateMachine to advance to next song due to button press
        move    #>NEXT_SONG,x0          ; Button && NoPlay
        move    x0,y:MsgParserNextParam
        move    #>MsgParserNextSong,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage
        ; tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0 ; JEFFRO
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        ;ignore any messages until we're reset
        bset    #DecoderIgnoreMessages,x:DecoderSR

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AudibleMp3DecoderEventValue
        jmp     _NextMessage
_nextStop
         jsr     StopCurrentSong
         jsr     DecoderCloseSong

        ; tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        ; Free DCLK for other speed clients
        move    #>SPEED_CLIENT_AUDIBLE_MP3,y0
        jsr     SysSpeedClockFree

        ; tell the parser to advance to next song and Play
        move    #>NEXT_SONG_PLAY_BUTTON,x0 ; Button && PLAY
        move    x0,y:MsgParserNextParam
        move    #>MsgParserNextSong,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        ;ignore any messages until we're reset
        bset    #DecoderIgnoreMessages,x:DecoderSR

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AudibleMp3DecoderEventValue
        jmp     _NextMessage

;-------------------------PrevSong Function-------------------------
_PrevSong

        ; Configure CSR
        bclr    #SyncWait,x:DecoderCSR
        bclr    #Ffwd,x:DecoderCSR
        bclr    #Rwnd,x:DecoderCSR
        jsr     DecABModeDisable             ; reset any AB mode settings

        move    x:DecoderCSR,x0
        jset    #Play,x0,_prevStop
        jset    #Pause,x0,_prevStop

        ; Song is stopped
        jsr     DecoderCloseSong

        ; Free DCLK for other speed clients
        move    #>SPEED_CLIENT_AUDIBLE_MP3,y0
        jsr     SysSpeedClockFree

        ; Tell the PlayerStateMachine/PlayerLib to go back to previous song
        move    #>PREV_SONG,x0          ; PrevSong && Stop
        move    x0,y:MsgParserPrevParam
        move    #>MsgParserPrevSong,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage
        ; tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0 ; JEFFRO
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        ;ignore any messages until we're reset
        bset    #DecoderIgnoreMessages,x:DecoderSR

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AudibleMp3DecoderEventValue
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
        move    #>SPEED_CLIENT_AUDIBLE_MP3,y0
        jsr     SysSpeedClockFree

        ; tell the PlayerStateMachine to advance to next song and Play
        move    #>PREV_SONG_PLAY,x0     ; PrevSong && Play
        move    x0,y:MsgParserPrevParam
        move    #>MsgParserPrevSong,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        ;ignore any messages until we're reset
        bset    #DecoderIgnoreMessages,x:DecoderSR

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AudibleMp3DecoderEventValue
        jmp     _NextMessage

;-------------------------TimeMode Function-------------------------
_TimeMode       ; switches between elapsed and remaining song time
        move    x:DecoderCSR,x0
        bchg    #TimeMode,x0
        move    x0,x:DecoderCSR
        jmp     _NextMessage

;-------------------------ABMode Function---------------------------
_ABMode
        move    x:DecoderCSR2,x0
        jset    #EndOfFileReached,x0,_disableABMode ; if mp3 freads done,quitAB
        move    x:DecoderCSR,x0        
_checkA
        jset    #ABMode_A,x0,_checkB
        bset    #ABMode_A,x:DecoderCSR  ; first msg is for "A" location
        jsr     Mp3DecGetSongPos           ; returns consumed byte position in A1:A0
        move    a1,y:AudibleMp3ABPositionAHigh ; save consumed high byte //FOR LOOPING BACK TO "A", SEEK TO CONSUMED BYTES AS OPPOSED TO READ BYTES, SO THAT YOU DON'T LOSE DATA
        move    a0,y:AudibleMp3ABPositionALow  ; save consumed low byte
        move    a1,y:AudibleMp3ABPositionAConsumedHigh ; ditto
        move    a0,y:AudibleMp3ABPositionAConsumedLow

        jmp     _exitABMode
_checkB
        jset    #ABMode_B,x0,_disableABMode
        bset    #ABMode_B,x:DecoderCSR  ; second msg is for "B" location
        jsr     Mp3DecGetSongPos           ; returns byte position in A1:A0
        move    a1,y:AudibleMp3ABPositionBHigh    ; save high byte. consumed version
        move    a0,y:AudibleMp3ABPositionBLow     ; save low byte

        ; make sure AconsumedPos < Bpos  (both are consumption positions)
        clr     b
;        move    y:AudibleMp3ABPositionAHigh,b1    ; B1 = PosAHigh
;        move    y:AudibleMp3ABPositionALow,b0     ; B0 = PosALow
        move    y:AudibleMp3ABPositionAConsumedHigh,b1    ; B1 = PosAHigh consumed ver
        move    y:AudibleMp3ABPositionAConsumedLow,b0     ; B0 = PosALow  consumed ver
        cmp     a,b                     ; Bpos vs Apos, both consumed ver
        jge     _disableABMode          ; Disable if Bpos < Apos, else continue
        
        ; ------ if B position is too near end, disable AB Mode         
        move    y:SongByteLengthHigh,b          ; clears b2
        move    y:SongByteLengthLow,b0

        ; bitrate/8 gives 1 sec of bytes
        ; 1 sec at 8/16/32/64/128kbps is 1000/2000/4000/8000/16000 bytes
        clr     a                          
        move    x:Fbitrate_o,a1
        ; 3 right shifts: bits to bytes, then mpy by p seconds for byte offset
        rep     #3                  
        lsr     a
        move    a1,x1               ; bytes per second
        move    y:<Const_000003,y1  ; <<< times p seconds gives p seconds of bytes (EOF offset)
        mpy     x1,y1,a
        asr     a                   ; frac to int 56k adjustment
        ; Byte count result for p seconds of MP3 is in A1:A0. 
        ; Actually just in A0 at max case 324000/8*pSeconds.
        
        ; Note: Alt ver of above would use constant byte count but must handle worst case 
        ; of highest encoding rate at p seconds but reduces Bpos limit excessively.
;    testpoint
        ; Bytes in p seconds (A1:A0) subtracted from SongByteLength offset 
        sub     a,b        
        tst     b
        jle     _disableABMode                      ; Require fileLength >offset
        ; Now B1:B0 has valid (fileLength-n000) bytes 
        ;                                (n000 bytes as p seconds of offset)

        ;Finally: if( SongByte[Sub]Total < (fileLength-n000) )
        ;             allow Bpos, else DecABModeDisable since Bpos near EOF.
        move    y:SongByteTotalHigh,a               ; clears A2
        move    y:SongByteTotalLow,a0
        cmp     b,a
        jlt     _exitABMode                         ; jump & continue
        
        ; ------ alt: if B position is too near end, disable AB Mode         
;       clr     b
;    	move    y:SongRemainMinutes,b1    
;    	tst     b
;    	jne     _exitABMode ;jump and allow this Bpos if not last minute of song
;       clr     b
;    	move    y:SongRemainSeconds,b1
;       clr     a
;    	move    #>4,a           ; << EOS seconds offset, allowed Bpos limit
;    	cmp     a,b
;    	jgt     _exitABMode    ; if remaining seconds > const, allow this Bpos

_disableABMode
        jsr     DecABModeDisable        ; clear bits, reset byte positions to 0.

_exitABMode
        move    x:DecoderCSR,x0
        jclr    #Stop,x0,_next
        move    #>EVENT_MESSAGE,x0
        move    x0,y:AudibleMp3DecoderEventValue
_next
        jmp     _NextMessage

;-------------------------SetEQ Function----------------------------
_SetEQ
        ; Get the arguments
        move    y:(r1)+,x0              ; Bass Corner Frequency
        move    x0,x:FBass_Freq_o       ; Parthus mp3 decoder location
        move    y:(r1)+,x0              ; Bass Enhancement
        move    x0,x:FBass_Enhance_o    ; Parthus mp3 decoder location
        move    y:(r1)+,x0              ; Treble Corner Frequency
        move    x0,x:FTreble_Freq_o     ; Parthus mp3 decoder location
        move    y:(r1)+,x0              ; Treble Enhancement
        move    x0,x:FTreble_Enhance_o  ; Parthus mp3 decoder location
        move    y:(r1)+,x0              ; Tone Attenuation
;        move    x0,x:

        move    x:DecoderCSR,x0
        jclr    #Stop,x0,_exitSetEQ
        move    #>EVENT_MESSAGE,x0
        move    x0,y:AudibleMp3DecoderEventValue
_exitSetEQ
		; Does not do anything for MP3
_GetSongInfo
        jmp     _NextMessage

;-----------------------------------------------------------------------------------
_NextChapter
        bset    #ABQuiet,x:DecoderCSR
        jsr     DecABModeDisable        ; reset any AB mode settings
        ; Open File If Necessary
        move    x:DecoderCSR,x0
        jset    #FileIsOpen,x0,_FileOpenedForNextChapter
        clr     a
        jsr     DecoderOpenSong         ; open the file
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

        ; EffectsInit should be called before calling any DAC initialization to ensure the processing chain is loaded.
        jsr     EffectsInit
        
        ; Great. Now, I have exclusive access.
		jsr     SysAllLinear
        jsr     DacDriverInit
        jsr     DacDriverEnablePort   ; set up the DAC control registers, enable the port
        jsr     FAudible_MP3Reset
        tst     A
        jne     _Stop

        ; Increase speed to SPEED_MAX. Allows faster file seeks.
        move    #>SPEED_CLIENT_AUDIBLE_MP3,y0
        jsr     SysSpeedClockFree        
        move    #>SPEED_MAX,x1     
        move    #>SPEED_CLIENT_AUDIBLE_MP3,y0   ; Speed client identifier
        jsr     SysSpeedIncrease                ; Manages clock speed clients

        ; Navigate to next chapter
        jsr     FAudible_MP3NextChapter
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
        move    x0,y:AudibleMp3DecoderEventValue
        jmp     _NextMessage

_Cont_NextChapter
;        jsr     AcelpGetInvBitRate        ; Get the Inverse of Bit Rate.
;        jsr     DecoderUpdateLCD        ; song time and number

        ; mmiu - Get the current file offset and save it off. This is necessary
		; to continue playback at the new position.
        jsr     FAudible_MP3FilePos
        move    A,y:SongByteTotalHigh
        move    A0,y:SongByteTotalLow
        move    A,y:SongByteTotalConsumedHigh
        move    A0,y:SongByteTotalConsumedLow

;        move    #>SPEED_CLIENT_AUDIBLE_MP3,y0
;        jsr     SysSpeedClockFree             ;Free DCLK for other speed clients

        ; tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage
        jsr     DecoderSetEventTimer         ; set up DecoderEventValue for module timer

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AudibleMp3DecoderEventValue

		jsr	 FAudible_MP3UpdateChapterNumber
		; tell the menu module to update chapter info
        move    #MsgMenuUpdateChapterInfo,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        jmp     _NextMessage

;-----------------------------------------------------------------------------------
_PrevChapter
        bset    #ABQuiet,x:DecoderCSR
        jsr     DecABModeDisable        ; reset any AB mode settings
        ; Open File If Necessary
        move    x:DecoderCSR,x0
        jset    #FileIsOpen,x0,_FileOpenedForPrevChapter
        clr     a
        jsr     DecoderOpenSong         ; open the file
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

        ; EffectsInit should be called before calling any DAC initialization to ensure the processing chain is loaded.
        jsr     EffectsInit
        
        ; Great. Now, I have exclusive access.
		jsr     SysAllLinear
        jsr     DacDriverInit
        jsr     DacDriverEnablePort   ; set up the DAC control registers, enable the port
        jsr     FAudible_MP3Reset
        tst     A
        jne     _Stop

        ; Increase speed to SPEED_MAX. Allows faster file seeks.
        move    #>SPEED_CLIENT_AUDIBLE_MP3,y0
        jsr     SysSpeedClockFree        
        move    #>SPEED_MAX,x1     
        move    #>SPEED_CLIENT_AUDIBLE_MP3,y0   ; Speed client identifier
        jsr     SysSpeedIncrease                ; Manages clock speed clients

        ; Navigate to previous chapter
        jsr     FAudible_MP3PrevChapter
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

	    ; tell the parser to go to previous song and Play
	    move    #>NEXT_SONG_PLAY_EOF,x0      ; EOF && PLAY
	    move    x0,y:MsgParserNextParam
	    move    #>MsgParserPrevSong,r0
	    move    y:<Const_ffffff,m0
	    jsr     SysPostMessage

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AudibleMp3DecoderEventValue
        jmp     _NextMessage

_Cont_PrevChapter
;        jsr     AcelpGetInvBitRate        ; Get the Inverse of Bit Rate.
;        jsr     DecoderUpdateLCD        ; song time and number

        ; mmiu - Get the current file offset and save it off. This is necessary
		; to continue playback at the new position.
        jsr     FAudible_MP3FilePos
        move    A,y:SongByteTotalHigh
        move    A0,y:SongByteTotalLow
        move    A,y:SongByteTotalConsumedHigh
        move    A0,y:SongByteTotalConsumedLow

        ; tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage
        jsr     DecoderSetEventTimer         ; set up DecoderEventValue for module timer

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AudibleMp3DecoderEventValue

		jsr		FAudible_MP3UpdateChapterNumber
		; tell the menu module to update chapter info
        move    #MsgMenuUpdateChapterInfo,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        jmp     _NextMessage


_handleStopCurrentSongError
_handleOpenSongError
_handleCloseSongError
_handleEofError
_handleFileSeekError
        ; just stop the decoder
        jmp     _Stop
        
;-------------------------Invalid Message---------------------------
_InvalidMessageID
        error       ; debug halt for DEBUG build, resets part if RETAIL build

;-------------------------------------------------------------------
_NextMessage 
        ; We must arrive here with stack balanced, to pop correct return address.
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
        ; Msg queue empty. 
        ; Update the buffer descriptor with the new tail pointer
        move    #>BD_TailPtr,n0
        nop
        move    b1,y:(r0+n0)
        pop     r6                        ; return address. 
        nop
        jmp     (r6)                      ; return to Mp3DecoderModule routine


; Stmp3000 popped bad r6. Play invalid song followed by valid song: mpeg frame sync attempted on 
; invalid mp3 extension file.
; Results in DecoderForceInit (which sends decoder EVENT_INIT msg so kernel reinits this decoder), 
; followed by play message being sent to decoder by playerlib/PSM.
; In that case the following error sometimes occurs per MPEG decoder ISR sync interval variation: 
; Before jump to return address, r7 pointed 4 stack locations higher than desired value so pop (an r7 macro)
; above returned bad value ($1f) and the jmp r6 above then hit a debug in the vector table. So dec_overlay
; pushed 4 without having a chance to pop 4, causing stack unbalance. This is resolved by updates
; at _WaitForSync label and when a song is closed. Stmp3000
;===============================================================================

;-------------------------------------------------------------------------------

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
        dc      _SetEQ
		dc		_GetSongInfo
		dc		_NextChapter
		dc		_PrevChapter

; Note: It is important that the entries in the jump table be in the
; same order as the Decoder message ID's which are defined in msgequ.inc

;*******************************************************************************
; DECODER-SPECIFIC FILE FUNCTIONS
; These functions are used by the decoder to perform file system actions.
;*******************************************************************************
;-------------------------------------------------------------------------------
; Name    DecoderFileSeek
;
; Type:   MP3 Decoder Overlay Private Function
;
; Input:  A1:A0 has number of bytes to increment global SongByteTotalHigh:Low 
;               and to FileSeek (0 seeks to beginning of file)
;         B1:B0 has number of bytes to increment global SongByteTotalConsumedHigh:Low
;
; Output: CCR.carry set on error including FileSeek failure or negative parameter
;
; Note:   To DecoderFileSeek backwards, caller must first DecoderFileSeek to zero.
;-------------------------------------------------------------------------------
DecoderFileSeek
        tst     A
        jge     _IsNextParamValid       ; Disallow negative seeks
        or      #>$01,ccr               ; carry bit indicates error
        jmp     _exit
_IsNextParamValid
        tst     B
        jge     _SeekIt                 ; Disallow negative seeks
        or      #>$01,ccr               ; carry bit indicates error
        jmp     _exit
_SeekIt
        ; FileSeek forward
        push    a1
        push    a0
        push    b1
        push    b0
        ; Special Case - 0 will reset the pointer to the beginning of the file.
        tst     a
        jne     _AddOffset
        ; Reset the file pointer
        clr     a        
        ;move    y:AudibleMp3DecoderFileHandle,a2
        ;jsr     FileSeek    
        ; 10/5/2004 mmiu - Redirect general FileSeek with FAudible_MP3FileSeek
        jsr     FAudible_MP3FileSeek
        
        ;seek to the start of the audio data.
        ;load the Start Position - found from MP3MetaData scan.
        move    #>FAudibleMp3DecoderStartFrameFilePosition,r0
        nop
        move    y:(r0)+,a0
        move    y:(r0)+,a1
_AddOffset        
        ; Add the offset.  
        ;move    y:AudibleMp3DecoderFileHandle,a2
        ;jsr     FileSeek                
        ; 10/5/2004 mmiu - Redirect general FileSeek with FAudible_MP3FileSeek
        jsr     FAudible_MP3FileSeek
; FSERROR:
        move    y:Const_000000,a2
        pop     b0
        pop     b1
        pop     a0
        pop     a1                      ; a1:a0 has seek offset value
        jcs     _exit                   ; bail if the FileSeek call failed

        jsr     DecoderSeek

_exit
        rts

DecoderSeek

        tst     a
        jne     _addBytes
_setToZero
        ; DecSetSongPosZero clears SongByteTotal and SongByteTotalConsumed globals
        jsr     DecSetSongPosZero 
        jmp     _updateScreen
_addBytes
        move    y:SongByteTotalHigh,x1
        move    y:SongByteTotalLow,x0
        add     x,a                     ; a has byte value (prior to next Read)
        move    a1,y:SongByteTotalHigh
        move    a0,y:SongByteTotalLow
        ; TIME_FIX  Also do a seek byte add and update for consumed bytes as above.
        nop                             ; Don't remove. Disasm showed mangling without.
        nop
        move    y:SongByteTotalConsumedHigh,x1
        move    y:SongByteTotalConsumedLow,x0
        add     x,b                     ; b has byte value (prior to next Read)
        move    b1,y:SongByteTotalConsumedHigh
        move    b0,y:SongByteTotalConsumedLow

		; mmiu - Do the actual seek. A = new seek pos.
;        jsr     FAudible_MP3FileSeek

_updateScreen
        jsr     DecoderUpdateCurrentTime
        and     #>$FE,ccr                   ; clear carry bit indicating SUCCESS
        rts

;-------------------------------------------------------------------------------
InitInputBuffer
        ; Init the input buffer to empty (set buffer head/tail to beginning)
        move    #g_AudibleMp3DataSrcBufDescriptor,r0     ; buffer descriptor ptr
        move    y:<Const_ffffff,m0
        jsr     InitBufferEmpty

		; 10/4/2004 mmiu - Call C function to reset buffer as well. Keeps C
		; pointers in sync with asm.
		jsr     FAudible_MP3Reset
_exit
        rts

;-------------------------------------------------------------------------------
; LoadAudibleMP3Decoder
;
; This routine loads the Audible+MP3 decoder into memory. Actually, the MP3
; algorithm is not part of that decoder. The Audible+MP3 routines allow access
; to the encoded audio within an Audible Audio file.
;-------------------------------------------------------------------------------
LoadAudibleMP3Decoder
        ; Load the MP3 decoder into P memory
        move    #>RSRC_AUDIBLE_MP3P_CODE,x0             ; Resource number
        move    #>RSRC_TYPE_DATA,x1                     ; Resource type is data
        move    #>(F_lc_u_e_MP3_P_buf-F_lc_u_b_MP3_P_buf)*3,y1 ; overlay area sz
        move    #>F_lc_u_b_MP3_P_buf,y0                 ; Address of the overlay area
        move    #TARGET_MEM_P,a1                        ; Overlay area is in P memory
        jsr     SysLoadResource

        ; Load the MP3 decoder into X memory
        move    #>RSRC_AUDIBLE_MP3X_CODE,x0             ; Resource number
        move    #>RSRC_TYPE_DATA,x1                     ; Resource type is data                  
        move    #>(F_lc_u_e_MP3_X_buf-F_lc_u_b_MP3_X_buf)*3,y1 ; overlay area sz
        move    #>F_lc_u_b_MP3_X_buf,y0                 ; Address of overlay area
        move    #TARGET_MEM_X,a1                        ; Overlay area is in X memory
        jsr     SysLoadResource

        ; Load the MP3 decoder into Y memory
        move    #>RSRC_AUDIBLE_MP3Y_CODE,x0             ; Resource number
        move    #>RSRC_TYPE_DATA,x1                     ; Resource type is data
        move    #>(F_lc_u_e_MP3_Y_buf-F_lc_u_b_MP3_Y_buf)*3,y1 ; overlay area sz
        move    #>F_lc_u_b_MP3_Y_buf,y0                 ;  Address of the overlay area
        move    #TARGET_MEM_Y,a1                        ; Overlay area is in Y memory
        jsr     SysLoadResource


        ; The default value of 48,000 bytes for rewind is excessively large for 
        ; small low bit rate songs. Default val is used to skip, until sync found.
        move    y:SongByteLengthHigh,a
        move    y:SongByteLengthLow,a0
        asr     A
        asr     A       #0,B
        asr     A       y:AudibleMp3DecoderFfwdRwndValue,B0
        cmp     b,a
        tgt     b,a
        move    A0,y:AudibleMp3DecoderFfwdRwndValue    

        ; Do not Init SongByteTotalConsumed counts to 0 here as it is done once
        ; at declaration time and reclearing here impedes FF/RW boundary cross
        ; handling.

        ; Clear the bit buffer and pcm buffer for mp3 decode routines
        clr     a
        move    y:<Const_ffffff,m3
        move    #>g_AudibleMp3DecSink_mbuf,r3
        do      #g_AudibleMp3DecSinkBufSize,_clear_pcmbuffer
         move    a,y:(r3)+
         nop
_clear_pcmbuffer

        move    #>g_AudibleMp3DataSrc_mbuf,r3
        do      #g_AudibleMp3DataSrcBufSize,_clear_bitbuffer
         move    a,x:(r3)+
         nop
_clear_bitbuffer

        ; Initialize for Mp3 Decoder
        move    #g_AudibleMp3DataSrcBufDescriptor,x0
        move    x0,y:AudibleMp3DecoderSourceBufDesc  ; decoder source buffer descriptor
        move    #g_AudibleMp3DecSinkBufDescriptor,x0
        move    x0,y:AudibleMp3DecoderSinkBufDesc    ; decoder sink buffer descriptor

        ; Init the Parthus Mp3 Decoder
        jsr     AudibleMp3DecReset

        ; Init decoder status to stopped & non-VBR
        move    y:<Const_000000,x0
        move    x0,x:AudibleDecoderReadStatus
        bset    #Stop,x0
        move    x0,x:DecoderSR

        move    x:DecoderCSR2,x0           ; Reclear EOF reached bit. DECODE2EOF
        bclr    #EndOfFileReached,x0
        move    x0,x:DecoderCSR2

        rts

;-----------------------------------------------------------------
; DecoderOpenSong
;
; Carry clear = open good
; Carry set   = open bad
;-----------------------------------------------------------------
DecoderOpenSong
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
        clr     a                            ; memory space identifier (Y RAM)
        move    a,y:CurrentSongBad           ; need to mark current song not bad 
		jsr     LoadAudibleMP3Decoder

		; mmiu - Redirect FileOpen call to an Audible-specific file open...
        move    #Fg_CurrentSongName,r0       ; FAudible_MP3FileOpen expects filename in r0
        jsr     FAudible_MP3FileOpen         ; Do the real file open
		tst		A                            ; Were we successful?
		jeq		_fileOpenOK                  ; If 0, yes, open was successful
		or		#>$01,ccr                    ; Set error flag
		jmp		_exit

_fileOpenOK
        move    a1,y:AudibleMp3DecoderFileHandle       ; store the handle for the file
        move    x:DecoderCSR,x0
        bset    #FileIsOpen,x0
        move    x0,x:DecoderCSR              ; keep track of an open song

        ; mmiu - Store the total length of the audio
        jsr     FAudible_MP3GetFileSize
        move	A,y:SongByteLengthHigh       ; Song length in bytes
        move	A0,y:SongByteLengthLow

        and     #>$FE,ccr                    ; Clear error flag

; 10/5/2004 mmiu - Skip seek completely. Jus' rip it out.
;        jmp     _exit                        ; mmiu - Skip the file seek
;
;        ;-------------------------------------------------------------
;        ; Seek to Zero (init) the Song (FileSeek)
;        ;-------------------------------------------------------------
;        ; Inputs:   A2 = Handle  A1/A0 = relative offset
;        ;-------------------------------------------------------------
;        jsr     SysAllLinear
;        clr     a                            ; offset of 0
;        move    y:AudibleMp3DecoderFileHandle,a2
;        jsr     FileSeek
;        ;seek to the start of the data.
;        move    #>FAudibleMp3DecoderStartFrameFilePosition,r0
;        move    y:AudibleMp3DecoderFileHandle,a2
;        move    y:(r0)+,a0
;        move    y:(r0)+,a1
;        jsr     FileSeek
;        jcs     _exit

_exit
        rts

;-----------------------------------------------------------------
; DecoderCloseSong
; This function will use the AudibleMp3DecoderFileHandle to close the file.
;
; Returns:
; Carry Set   = File close error
; Carry Clear = File close OK
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
        ;clr     a
        ;move    y:AudibleMp3DecoderFileHandle,a2       ; pass the file handle to the function
        ;jsr     FILEClose

        ; mmiu - Redirect FileClose to Audible_MP3FileClose.
		;   RETCODE Audible_MP3FileClose(void);
		jsr     FAudible_MP3FileClose
		tst		A							; Check return value
		jeq		_fileCloseOK				; If 0, file close succeeded
		or		#>$01,ccr					; Otherwise, error
		jmp		_fileClosed

_fileCloseOK
		and     #>$FE,ccr					; File close succeeded

        ; if FILEClose has error we still need to fall through 
_fileClosed
        move    x:DecoderCSR,x0
        bclr    #FileIsOpen,x0
        move    x0,x:DecoderCSR              ; prevents attempts to close a file multiple times w/out opening

		; tell the menu module to update file psition in *.POS file
        move    #MsgMenuSetSongPosition,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage
	
_exit
        ;*** TBD : clear song parameters?
        rts
_notOpen
        and     #>$FE,ccr
        jmp     _exit
        
;--------------------------------------------------
CheckABBoundaries
        move    x:DecoderCSR,x0
        jset    #EndOfSong,x0,_turnOffABMode ; Shutdown AB if EOS hit 
_isBModeSet
        jset    #ABMode_B,x0,_checkBoundaryB
        ; Note: Now #EndOfFileReached is freads done, #EndOfSong means decoder done.
        jmp     _exitCheckAB            ; jump & continue
_turnOffABMode        
        ; end of song reached before "B" -- turn off AB Mode
        jsr     DecABModeDisable
;    testpoint
        jmp     _exitCheckAB
_checkBoundaryB
        jsr     Mp3DecGetSongPos        ; returns current song position in A1:A0
        jsr     DecGetABModeBPos        ; returns AB Mode "B" position in B1:B0
        cmp     a,b
        jgt     _exitCheckAB            ; Not at position "B" yet.
        jsr     DecSetSongPosA          ; At "Bpos", so set to position "Apos"
;    testpoint

        rts

_exitCheckAB
        and    #>$FE,ccr
        rts

;--------------------------------------------------
DecABModeDisable
; Disables the AB Mode by clearing the bits and resetting byte positions to 0.
; Sends message to MENU to turn off AB icons.
        bclr    #ABMode_A,x:DecoderCSR  ; disable AB Mode
        bclr    #ABMode_B,x:DecoderCSR
        move    y:<Const_000000,x0      ; reset memory locations
        move    x0,y:AudibleMp3ABPositionAHigh
        move    x0,y:AudibleMp3ABPositionALow
        move    x0,y:AudibleMp3ABPositionAConsumedHigh
        move    x0,y:AudibleMp3ABPositionAConsumedLow
        move    x0,y:AudibleMp3ABPositionBHigh
        move    x0,y:AudibleMp3ABPositionBLow

        ; post msg to menu to remove AB icon
        move    #>MsgDecoderABModeOff,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage
        rts

DecGetABModeBPos
; Returns: B1 = AudibleMp3ABPositionBHigh , B0 = AudibleMp3ABPositionBLow
        clr     b
        move    y:AudibleMp3ABPositionBHigh,b1
        move    y:AudibleMp3ABPositionBLow,b0
        rts

; Internal subroutine: DecSetSongPosA
; Outputs: Returns with carry bit set if error in: Stopping song, file seek to 0, 
;          file seek to "A"
DecSetSongPosA
        move    x:DecoderCSR,x0
        jset    #ABQuiet,x0,_skip
        jsr     StopCurrentSong
; FSERROR:
        jcs     _exit
        bclr    #Stop,x:DecoderCSR
_skip

        ; Seek to Zero in File
        jsr     SysAllLinear
        clr     a
        ;move    y:AudibleMp3DecoderFileHandle,a2
        ;jsr     FileSeek
        ; 10/5/2004 mmiu - Redirect general FileSeek with FAudible_MP3FileSeek
        jsr     FAudible_MP3FileSeek

        move    #>FAudibleMp3DecoderStartFrameFilePosition,r0
		nop
;        move    y:AudibleMp3DecoderFileHandle,a2
        move    y:(r0)+,a0
        move    y:(r0)+,a1

;        jsr     FileSeek
        ; 10/5/2004 mmiu - Redirect general FileSeek with FAudible_MP3FileSeek
        jsr     FAudible_MP3FileSeek
        jcs     _exit
        
;_atZero
; Sets the song position to AB position "A"
;    testpoint

        clr     a
        move    y:AudibleMp3ABPositionAHigh,a1
        move    y:AudibleMp3ABPositionALow,a0
        move    a1,y:SongByteTotalHigh
        move    a0,y:SongByteTotalLow
        ; TIME_FIX  Also update bytes consumed
        clr     b
        move    y:AudibleMp3ABPositionAConsumedHigh,b1
        move    y:AudibleMp3ABPositionAConsumedLow,b0
        move    b1,y:SongByteTotalConsumedHigh  ; reset to saved A consumed pos
        move    b0,y:SongByteTotalConsumedLow

;        move    y:AudibleMp3DecoderFileHandle,a2
;        jsr     FileSeek                    ; Seeks to A1:A0 in A2 filehandle
        ; 10/5/2004 mmiu - Redirect general FileSeek with FAudible_MP3FileSeek
        jsr     FAudible_MP3FileSeek
; FSERROR:
        jcs     _exit
        
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


;*******************************************************************************
; UpdateVBRTime.
; This function updates the time if we're a VBR file.
;*******************************************************************************
UpdateVBRTime:
    if (1)
        ; Updates the display of the elapsed/remaining song time
;       wCurrentTick = (WORD)SysGetCurrentTime();
        jsr     SysGetCurrentTime
        ; return current time in a.
        move    y:AccumulatedMSecs,y1
;       wTemp = wCurrentTick - g_wDeltaTimeStampMsec;
        move    y:DeltaTimeStampMsec,x1
        move    a0,b
        sub     x1,b
;       g_wAccumulatedMSecs += wTemp;
        add     y1,b
        move    b1,y1
;       if (g_wAccumulatedMSecs > 1000) 
        tfr     y1,b
        move    #1000,x1
        move    #0,b2
        cmp     x1,b
        jle     _EndUVBRTTest
;       {
;           g_wAccumulatedSecs++;
        move    y:AccumulatedSecs,b
        move    #>1,y0
        add     y0,b
        move    b1,y:AccumulatedSecs
;           g_wAccumulatedMSecs -= 1000;
        tfr     y1,b
        sub     x1,b
        move    b1,y1
;       }

_EndUVBRTTest
; Store away DeltaTimeStampMsec and Accumulated milliseconds.
        move    a0,y:DeltaTimeStampMsec
        move    y1,y:AccumulatedMSecs
        
; Load A with Current Seconds
        move    y:AccumulatedSecs,a0
        jsr     SecondsToMinSecs            ; Convert total Seconds to Seconds & Minutes.
        ; Return results as X1 = Song Minutes, X0 = Song Seconds.
        
        ; Don't let the Current Time exceed the Total Time
        move    y:SongTotalMinutes,a1
        cmp     x1,a
        jgt     _CheckForChange1                ; if total not exceeded, do not update lcd  
        move    y:SongTotalSeconds,a1
        cmp     x0,a
        jlt     _UVBRT_no_update                ; if total not exceeded, do not update lcd               

_CheckForChange1        
        ; See if the time has changed since the last time we displayed current time.
        move    y:SongCurrentMinutes,a
        cmp     x1,a
        jne     _UVBRTupdate
        move    y:SongCurrentSeconds,a
        cmp     x0,a
        jne     _UVBRTupdate                      ; if no change, do not update lcd       
        jeq     _UVBRT_no_update                  ; if no change, skip over update
        
_UVBRTupdate
        move    x1,y:SongCurrentMinutes
        move    x0,y:SongCurrentSeconds
        move    #MsgMenuSongTimeChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage   
        
_UVBRT_no_update             
        rts
    endif        

;*******************************************************************************
; CalculateCurrentTimeVBR
; This function calculates where we should place the file pointer when doing a
; FFWD or RWND with a VBR file.
;*******************************************************************************
;-------------------------------------------------------------------------------
; Name    CalculateCurrentTimeVBR
;
; Type:   MP3 Decoder Overlay Private Function
;
; Description: Given a consumed byte count, interpolate where we are in
;         time using the VBR lookup table - for use when playing VBR files. 
;
; Input:  none
;
; Output: ??
;
; Note:   Set the CurrentBytePointer.
;         We can have up to 200 entries.
;         Changes can be applied to CalculateCurrentTime() in mp3metadata.c then
;         the list file can be copied here.
;------------------------------------------------------------------------------- 
                            
    if (1)                            
CalculateCurrentTimeVBR:
;        debug
        move    #3,n7
        nop
        move    (r7)+n7
        move    #0,n5           ; BYTE  i=0;
        move    #0,x1
        move    #-3,n7
        nop
        move    x1,y:(r7+n7)    
;           wCurrentSecondsx128=0;       
        move    y:SongByteTotalConsumedHigh,a0
        move    #0,a1
        move    a0,a
        move    y:SongByteTotalConsumedLow,y0
        move    #0,y1
        add     y,a
        move    a0,x0
        move    a1,x1           
;           dwCurrentBytePointer = ((DWORD)g_wSongByteTotalConsumedHigh<<24) + g_wSongByteTotalConsumedLow;
        move    #0,y0
        move    #-2,n7
        move    (r7)+
        move    y1,y:(r7+n7)
        move    (r7)-
        move    y0,y:(r7+n7)    
;           dwAccumulatedBytes=0;
        move    #Fg_wVBRTOCTable,r4
        jmp     CCT_VBR_LOOP_TEST
;           dwAccumulatedBytes += g_wVBRTOCTable[i++];
CCT_VBR_WHILE_TOP:   
        move    n5,n4
        nop
        lua     (r4)+n4,r6
        nop
        move    y:(r6),y0
        move    #0,y1
        move    #-2,n7
        move    (r7)+
        move    y:(r7+n7),b
        move    (r7)-
        move    y:(r7+n7),b0
        add     y,b
        move    (r7)+
        move    b1,y:(r7+n7)
        move    (r7)-
        move    b0,y:(r7+n7)
        move    n5,a
        move    #>1,y1
        add     y1,a
        move    a1,n5
;           wCurrentSecondsx128 += g_VBRGlobalData.VBRDurPerBin;
        move    x:Fg_VBRGlobalData+2,y0
        move    #-3,n7
        nop
        move    y:(r7+n7),b
        add     y0,b
        move    b1,y:(r7+n7)
        
CCT_VBR_LOOP_TEST:              ; while loop test here.
        move    #-2,n7
        move    (r7)+
        move    y:(r7+n7),b
        move    (r7)-
        move    y:(r7+n7),b0
        tfr     x1,a
        move    x0,a0
        move    #0,b2
        move    #0,a2
        cmp     b,a
        jlt     CCT_VBR_LOOP_DONE
        move    x:Fg_VBRGlobalData,a
        move    n5,b
        move    #0,a2
        cmp     a,b
        jlt     CCT_VBR_WHILE_TOP
        ; wTemp = g_wVBRTOCTable[i-1];
        
CCT_VBR_LOOP_DONE:   
        move    #Fg_wVBRTOCTable-1,r5
        nop
        lua     (r5)+n5,r6
        nop
        move    y:(r6),b
;wRoundError = (wTemp >> 1);
        move    b,y0
        lsr     b
;Subtraction = (WORD)(dwAccumulatedBytes - dwCurrentBytePointer);
        move    #-2,n7
        move    (r7)+
        move    y:(r7+n7),a
        move    (r7)-
        move    y:(r7+n7),a0
        sub     x,a
        move    a0,x0
        move    a1,x1
;Subtraction = ((g_VBRGlobalData.VBRDurPerBin * Subtraction) + wRoundError) / wTemp;
        move    x:Fg_VBRGlobalData+2,y1
        move    b1,b0
        asl     b
        mac     y1,x0,b
        asr     b
        move    b0,a
        tfr     y0,b
        jsr     Rdiv_uiuiui
;wCurrentSecondsx128 -= Subtraction;
        move    #-3,n7
        nop
        move    y:(r7+n7),b
        move    a0,a
        sub     a,b
        move    b1,y0
;g_wAccumulatedSecs = wCurrentSecondsx128/128;
        mpy     y0,#7,b
        move    b1,y:AccumulatedSecs
 ;} finish up!!
        move    (r7)+n7
        rts        
    endif
        endsec


