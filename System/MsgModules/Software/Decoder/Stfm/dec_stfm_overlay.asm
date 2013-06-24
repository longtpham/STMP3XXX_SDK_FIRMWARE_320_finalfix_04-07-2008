;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000/2001
;
; STMP3 User-level Decoder Module (Can be overlayed)        
;///////////////////////////////////////////////////////////////////////////////
        opt     mex

        section DECMOD_Stfm


;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////
       GLOBAL  StfmDecoderModule
       GLOBAL  StfmDecoderModuleInit

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////
       EXTERN	DecoderCSR
       EXTERN	DecoderSR
       EXTERN	DacDriverIsr
       EXTERN	DecoderProcessPtr
       EXTERN	SysLoadResource
       EXTERN   SysSpeedIncrease
       EXTERN   SysSpeedClockFree
       EXTERN 	F_lc_u_b_STFM_P
       EXTERN 	F_lc_u_e_STFM_Dec_P
       EXTERN 	F_lc_u_b_STFM_X
       EXTERN 	F_lc_u_e_STFM_Dec_X
       EXTERN 	F_lc_u_b_STFM_Y
       EXTERN 	F_lc_u_e_STFM_Dec_Y
       EXTERN	SysSetupDecVectors
       EXTERN	TotalSongCount
       EXTERN	StfmDecoderSinkBufDesc
       EXTERN	g_StfmDecSinkBufDescriptor
       EXTERN	DecoderQueueDescriptor
       EXTERN	ModuleSetTimer
       EXTERN	SysCheckTime
       EXTERN	SysPostMessage
       EXTERN	MsgDecoderStateChange
       EXTERN	MsgMenuSongTimeChange
;       EXTERN	StfmDecoderFileHandle
       EXTERN	CurrentDirDevId
       EXTERN	DacDriverDisablePort
       EXTERN	DacDriverEnablePort
       EXTERN	DacDriverInit
       EXTERN   SysAllLinear
       EXTERN   DecoderTimerNum
       EXTERN   SysEnableTimer
       EXTERN   InitBufferEmpty
       EXTERN   MixerMasterFadeOut
       EXTERN   MixerMasterFadeIn
       EXTERN   FILEClose
       EXTERN   FILEOpen
       EXTERN   FileEOF
       EXTERN   FileRead
       EXTERN   InvSampleRate
       EXTERN   RewindMode
;       EXTERN   StfmSecondstoSkip
       EXTERN	SongCurrentMinutes
       EXTERN	SongCurrentSeconds
       EXTERN	SongRemainMinutes
       EXTERN	SongRemainSeconds
       EXTERN	SongTotalMinutes
       EXTERN	SongTotalSeconds
       EXTERN	SongByteTotalLow
       EXTERN	SongByteTotalHigh
       EXTERN	SongByteLengthHigh
       EXTERN	SongByteLengthLow
       EXTERN   DecSetSongPosZero
       EXTERN   g_pStfmDecSinkBufSize
       EXTERN   FillBufferY
       EXTERN   MsgGeqSetCoef
       EXTERN   MsgGeqParam
       EXTERN   Fg_CurrentSongName
       EXTERN   wave_header_size
        ; for parser
       EXTERN   MsgParserStopSong
       EXTERN   MsgParserNextSong
       EXTERN   MsgParserNextParam
       EXTERN   MsgParserPrevSong
       EXTERN   MsgParserPrevParam
       EXTERN   NEXT_SONG
       EXTERN   NEXT_SONG_PLAY_EOF
       EXTERN   NEXT_SONG_PLAY_BUTTON
       EXTERN   PREV_SONG
       EXTERN   PREV_SONG_PLAY
       EXTERN   PREV_SONG_RWND
       EXTERN   NEXT_SONG_FFWD

        ; for DAC
       EXTERN   DacData1L
       EXTERN   DacData1LPtr
       EXTERN   DacData1R
       EXTERN   DacData1RPtr
 
      
       EXTERN   GetFilledSpace
       EXTERN   SysFlushCoderMemory

        ; AB Mode References
       EXTERN   MsgDecoderABModeOff
;       EXTERN   StfmABPositionAHigh
;       EXTERN   StfmABPositionALow
;       EXTERN   StfmABPositionBHigh
;       EXTERN   StfmABPositionBLow 
       EXTERN   PlayMessage
       EXTERN   DecGetSongPos
       EXTERN g_I2SDMAReceiveIsr
       EXTERN StfmDecoderEventValue
       EXTERN FI2SDMAReceiveStart
       EXTERN FI2SDMAReceiveStop


       extern F_lc_u_bX_app_cluster
       extern CurrentSongBad
       extern Fg_Wave_data_offset
;       extern Decode_MS_ADPCM_Block
;       extern Decode_IMA_ADPCM_Block
       extern Decode_PCM_Block
       
       EXTERN   FileSeek
       EXTERN   wave_file_offset
       EXTERN	EffectsInit
       EXTERN	DacClearMemory

       include "stack.xref"
       include "stackmem.xref"
       include "const.xref"
       include "syscoder.xref"

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////
;        nolist
        include "resource.inc"
        include "sysequ.inc"
        include "hwequ.inc"
        include "sysspeed.inc"
        include "dacequ.inc"
        include "msgequ.inc"
        include "mixerequ.inc"
        include "sysresources.inc"
        include "systemdefs.inc"
        include "dec_stfmequ.inc"
        include "dec_stfm_overlay.xref"
        include "wave.xref"
		include "stfm.xref"
        
        
;        list
        page    132,60,0,0,0

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
        org     p,"DECMOD_Stfm_overlay_P":

        ; Resource number
        dc      RSRC_DEC_STFM_MOD_CODE

;*******************************************************************************
; StfmDecoderModuleInit
;  Init decoder variables
;  Init decoder isr & enable isr
;*******************************************************************************
StfmDecoderModuleInit

        ; save off stack
        move    #_stackSaved,r6              ; setup the return address
        move    #UserScratchY,r5             ; pointer to the SP local save
        jmp     SaveSystemStack              ; save the hardware stack
_stackSaved

        ; init the ModuleTable process pointer
        move    #StfmDecoderModule,x0
        move    x0,y:DecoderProcessPtr

        ; Load the STFM decoder into P memory
        move    #>RSRC_DEC_STFMP_CODE,x0       ; Resource number
        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
        move    #>(F_lc_u_e_STFM_Dec_P-F_lc_u_b_STFM_P)*3,y1 ; Size of the overlay area
        move    #>F_lc_u_b_STFM_P,y0              ; Address of the overlay area
        move    #TARGET_MEM_P,a1                ; Overlay area is in P memory
        jsr     SysLoadResource

        ; Load the STFM decoder into X memory
        move    #>RSRC_DEC_STFMX_CODE,x0       ; Resource number
        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
        move    #>(F_lc_u_e_STFM_Dec_X-F_lc_u_b_STFM_X)*3,y1 ; Size of the overlay area
        move    #>F_lc_u_b_STFM_X,y0                       ; Address of the overlay area
        move    #TARGET_MEM_X,a1                ; Overlay area is in X memory
        jsr     SysLoadResource

        ; Load the STFM decoder into Y memory
        move    #>RSRC_DEC_STFMY_CODE,x0       ; Resource number
        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
        move    #>(F_lc_u_e_STFM_Dec_Y-F_lc_u_b_STFM_Y)*3,y1 ; Size of the overlay area
        move    #>F_lc_u_b_STFM_Y,y0                       ; Address of the overlay area
        move    #TARGET_MEM_Y,a1                ; Overlay area is in Y memory
        jsr     SysLoadResource

        ; Clear the Sink Buffer
        jsr     DecoderClearSinkBuffer

        ; Initialize for STFM Decoder
        move    #g_StfmDecSinkBufDescriptor,x0
        move    x0,y:StfmDecoderSinkBufDesc      ; decoder sink buffer descriptor

        ; Init decoder status to stopped
        move    y:<Const_000000,x0
        bset    #Stop,x0
        move    x0,x:DecoderSR

        move    #>TARGET_MEM_Y,B
        move    y:<Const_000001,x0
	    move	#>g_StfmDecSinkBufDescriptor,A
        jsr	    EffectsInit     ; was   jsr	    CopyBufferInit

        ; Init the interrupt vectors
        move    #>DacDriverIsr,x0
        move    #>DacDriverIsr,x1
        move    #>g_I2SDMAReceiveIsr,y0

        jsr     SysSetupDecVectors

        ; restore stack
_restoreStack
        move    #UserScratchY,r5             ; pointer to the SP local save
        move    #_stackRestored,r6           ; setup the return address
        jmp     RestoreSystemStack           ; restore the hardware stack
_stackRestored

        ; come back if we get a message
        move    #>EVENT_MESSAGE,A
        
        rts

;*******************************************************************************
; StfmDecoderModule
;*******************************************************************************
StfmDecoderModule

        ; save off stack
        move    #_stackSaved,r6              ; setup the return address
        move    #UserScratchY,r5             ; pointer to the SP local save
        jmp     SaveSystemStack              ; save the hardware stack
_stackSaved

_ProcessSysMessages
        ; switch on system messages
        move    #DecoderQueueDescriptor,r0
        move    #-1,m0
        move    #_queueProcessed,r6
        jmp     DecoderProcessQueue
_queueProcessed

        ; Allow PSM/PlayerLib to call it next (if songs left)
        move    #>EVENT_MESSAGE,x0
        move    x0,y:StfmDecoderEventValue 

_exitStfmDecoderModule
        ; restore stack
        move    #UserScratchY,r5             ; pointer to the SP local save
        move    #_stackRestored,r6           ; setup the return address
        jmp     RestoreSystemStack           ; restore the hardware stack
_stackRestored

        ; Return value for executive (EVENT_NONE or EVENT_MESSAGE)
        move    y:StfmDecoderEventValue,a1 
        move    y:<Const_ffffff,m1
        rts

;*******************************************************************************
; Decoder Module Functions
;*******************************************************************************
;DecoderSetEventTimer
; If we want to return on the module timer OR message,
; this routine sets StfmDecoderEventValue up.
;        move    #>EVENT_MESSAGE,a1
;        move    #>500,x0
;        jsr     ModuleSetTimer
;        move    a1,y:StfmDecoderEventValue
;        rts

;*******************************************************************************
; DecoderProcessQueue
; r0=Descriptor for this modules message queue (in y memory)
; m0=linear
;*******************************************************************************
DecoderProcessQueue
        push    r6                        ; needed for return

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
;        move    x:DecoderSR,x0
;        jset    #DecoderIgnoreMessages,x0,_NextMessage

_ContinueProcessing
        ; Point r1 to the first argument of the message
        move    (r1)+
        move    (r1)+

        ; we don't use circular mode to access queue buffer with r1
        jsr     SysAllLinear		;stmp6661

        ; Clear the upper byte of the message ID
        move    #$00FFFF,x0
        and     x0,b

        ; Jump to the message handler
        move    #_JumpTable,r2
        move    #$FFFF,m2
        move    b1,n2
        nop
        ; HACK, maybe not a good idea since this is a two-cycle instr, not one
        movem   p:(r2+n2),r3
        nop
        jmp     (r3)
;-------------------------_ResetDecoder Function-------------------------
_ResetDecoder
;        bclr    #DecoderIgnoreMessages,x:DecoderSR
        ;NEED TO ADD MORE "RESET" stuff so that DECODER_RESET really does do what its supposed to
        jmp     _NextMessage


;-------------------------SetDir Function-------------------------
_Ignore
_SetDir
        ; No Longer Functional
        ; Not currently supporting browsing -- we just play through all songs
        ; ----------------------------------------------
        ; THIS MESSAGE IS NOT SUPPORTED!!!!!!!!!!!!!!!!!
        ; ----------------------------------------------
        jmp     _NextMessage

;-------------------------Play/Pause Function-------------------------
_Play   ; Toggles between Play and Pause
        push    r1

        ; Init dac and decoder for new song
        jsr     DecStfmInit
        jsr     DacDriverInit

        jsr     FI2SDMAReceiveStart

        bset    #HW_DACCSR_TXEN_BITPOS,x:HW_DACCSR
        bset    #HW_DACCSR_TXIEN_BITPOS,x:HW_DACCSR
        ; DCLK is set earlier in the FMTunerMenu.c
        ;move    #>SPEED_FM,x1	       ; Change speed to 18MHz to reduce FM power consumption
        ;move    #>SPEED_CLIENT_STFM_DEC,y0  ; Speed Client ID
        ;jsr     SysSpeedIncrease

        ; Fade in the Volume from Mute
        move    #>msFadeIn,x0
        jsr     MixerMasterFadeIn            ; fade volume in to previous level

        ; tell the menu module that the status has changed.
;        move    #>MsgDecoderStateChange,r0
;        move    #-1,m0
;        jsr     SysPostMessage

        clr     a
        bset    #Play,a1
        move    a1,x:DecoderCSR
        move    a1,x:DecoderSR

        pop     r1
        jmp     _NextMessage

;-------------------------Stop Function-------------------------
_Stop
        ; Fade in the Volume from Mute
        move    #>msFadeOut,x0
        jsr     MixerMasterFadeOut           ; fade volume in to previous level

        jsr     FI2SDMAReceiveStop

        bclr    #HW_DACCSR_TXEN_BITPOS,x:HW_DACCSR

        ; Free the DCLK to be used by other speed clients or to IDLE
        ; DCLK will be freed in the FMTunermenu.c
        ;move    #>SPEED_CLIENT_STFM_DEC,y0
        ;jsr     SysSpeedClockFree

        clr     a
        bset    #Stop,a1
        move    a1,x:DecoderCSR
        move    a1,x:DecoderSR

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
        jmp     (r6)                      ; return to StfmDecoderModule routine

_JumpTable
        dc      _ResetDecoder
        dc      _Ignore ; SetDir
        dc      _Play
        dc      _Stop
        dc      _Ignore ; Ffwd
        dc      _Ignore ; Rwnd
        dc      _Ignore ; NextSong
        dc      _Ignore ; PrevSong
        dc      _Ignore ; TimeMode
        dc      _Ignore ; ABMode
        dc      _Ignore ; SetEQ
	    dc	    _Ignore ; GetSongInfo

; Note: It is important that the entries in the jump table be in the
; same order as the Decoder message ID's which are defined in msgequ.inc

;*******************************************************************************
; DECODER-SPECIFIC FILE FUNCTIONS
; These functions are used by the decoder to perform file system actions.
;*******************************************************************************
DecStfmInit
        jsr     SysAllLinear
        jsr     DecoderClearSinkBuffer
        jsr     SetDacSRRnICR
_Exit
        rts

;---------------------------------------------------------------------------------------------

DecoderClearSinkBuffer
        ; init buffer head / tail to beginning
        move    #g_StfmDecSinkBufDescriptor,r0      ; SINK buffer descriptor ptr
        move    #>$ffff,m0
        jsr     InitBufferEmpty
        ; clear the sink buffer for stfm decode routines
        clr     a
        move    y:<Const_ffffff,m3
        move    #>g_StfmDecSink_mbuf,r3
        move    #>g_StfmDecSinkBufSize/2,x0
        do      x0,_clear_pcmbuffer
        move    a,y:(r3)+
        move    a,y:(r3)+
_clear_pcmbuffer
        nop
        rts

;---------------------------------------------------------------------------------------------
; Only 44.1KHz needed for STFM
SetDacSRRnICR
        move    #DAC44_1kHz,y0
;        move    #DAC24kHz,y0
        move    y0,x:HW_DACSRR          ; load current song sample rate into DacSrr
;;;        move    y:<Const_000040,y0      ;64 = 0x40
;;;         move    y0,x:HW_DACICR         ; DAC Interrupt Control Register, NonZero to avoid (rare) DAC underruns
       
        rts

        endsec


