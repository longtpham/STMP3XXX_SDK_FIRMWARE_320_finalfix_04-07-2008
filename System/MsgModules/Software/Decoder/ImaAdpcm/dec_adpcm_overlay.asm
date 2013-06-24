;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000/2001
;
; STMP3 User-level Decoder Module (Can be overlayed)        
;///////////////////////////////////////////////////////////////////////////////
        opt     mex

        section DECMOD_AdpcmIma


;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////
       GLOBAL  AdpcmDecoderModule
       GLOBAL  AdpcmDecoderModuleInit

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////
       EXTERN	DecAdpcmImaModuleTimerDisable
       EXTERN	DecAdpcmImaModuleIsr
       EXTERN	DecoderCSR
       EXTERN	DecoderSR
       EXTERN	DacDriverIsr
       EXTERN	AdpcmDecoderEventValue
       EXTERN	DecoderProcessPtr
       EXTERN	SysLoadResource
       EXTERN   SysSpeedIncrease
       EXTERN   SysSpeedClockFree
       EXTERN 	F_lc_u_b_ADPCM_P
       EXTERN 	F_lc_u_e_ADPCM_Dec_P
       EXTERN 	F_lc_u_b_ADPCM_X
       EXTERN 	F_lc_u_e_ADPCM_Dec_X
       EXTERN 	F_lc_u_b_ADPCM_Y
       EXTERN 	F_lc_u_e_ADPCM_Dec_Y
       EXTERN	SysSetupDecVectors
       EXTERN	TotalSongCount
       EXTERN	AdpcmDecoderSinkBufDesc
       EXTERN	g_AdpcmDecSinkBufDescriptor
       EXTERN	DecAdpcmProcess
       EXTERN	DecoderQueueDescriptor
       EXTERN	ModuleSetTimer
       EXTERN	SysCheckTime
       EXTERN	SysPostMessage
       EXTERN	MsgDecoderStateChange
       EXTERN	MsgMenuSongTimeChange
       EXTERN	AdpcmDecoderFileHandle
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
       EXTERN   AdpcmSecondstoSkip
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
       EXTERN   g_pAdpcmDecSinkBufSize
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
       EXTERN   AdpcmABPositionAHigh
       EXTERN   AdpcmABPositionALow
       EXTERN   AdpcmABPositionBHigh
       EXTERN   AdpcmABPositionBLow 
       EXTERN   PlayMessage
       EXTERN   DecGetSongPos


       extern F_lc_u_bX_app_cluster
       extern CurrentSongBad
       extern Fg_Wave_data_offset
       extern Decode_MS_ADPCM_Block
       extern Decode_IMA_ADPCM_Block
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
        include "dec_adpcmequ.inc"
        include "dec_adpcm_overlay.xref"
        include "wave.xref"
		include "adpcm.xref"
        
        
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
        org     p,"DECMOD_AdpcmIma_overlay_P":

        ; Resource number
        dc      RSRC_DEC_ADPCM_MOD_CODE

;*******************************************************************************
; AdpcmDecoderModuleInit
;  Init decoder variables
;  Init decoder isr & enable isr
;*******************************************************************************
AdpcmDecoderModuleInit

        ; save off stack
        move    #_stackSaved,r6              ; setup the return address
        move    #UserScratchY,r5             ; pointer to the SP local save
        jmp     SaveSystemStack              ; save the hardware stack
_stackSaved

        ; init the ModuleTable process pointer
        move    #AdpcmDecoderModule,x0
        move    x0,y:DecoderProcessPtr

        ; Load the ADPCM decoder into P memory
        move    #>RSRC_DEC_ADPCMP_CODE,x0       ; Resource number
        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
        move    #>(F_lc_u_e_ADPCM_Dec_P-F_lc_u_b_ADPCM_P)*3,y1 ; Size of the overlay area
        move    #>F_lc_u_b_ADPCM_P,y0              ; Address of the overlay area
        move    #TARGET_MEM_P,a1                ; Overlay area is in P memory
        jsr     SysLoadResource

        ; Load the ADPCM decoder into X memory
        move    #>RSRC_DEC_ADPCMX_CODE,x0       ; Resource number
        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
        move    #>(F_lc_u_e_ADPCM_Dec_X-F_lc_u_b_ADPCM_X)*3,y1 ; Size of the overlay area
        move    #>F_lc_u_b_ADPCM_X,y0                       ; Address of the overlay area
        move    #TARGET_MEM_X,a1                ; Overlay area is in X memory
        jsr     SysLoadResource

        ; Load the ADPCM decoder into Y memory
        move    #>RSRC_DEC_ADPCMY_CODE,x0       ; Resource number
        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
        move    #>(F_lc_u_e_ADPCM_Dec_Y-F_lc_u_b_ADPCM_Y)*3,y1 ; Size of the overlay area
        move    #>F_lc_u_b_ADPCM_Y,y0                       ; Address of the overlay area
        move    #TARGET_MEM_Y,a1                ; Overlay area is in Y memory
        jsr     SysLoadResource

        ; Clear the Sink Buffer
        jsr     DecoderClearSinkBuffer

        ; Initialize for ADPCM Decoder
        move    #g_AdpcmDecSinkBufDescriptor,x0
        move    x0,y:AdpcmDecoderSinkBufDesc      ; decoder sink buffer descriptor

        ; Init decoder status to stopped
        move    y:<Const_000000,x0
        bset    #Stop,x0
        move    x0,x:DecoderSR

        move    y:Fg_Wave_wChannels,A
        asr     A   #>TARGET_MEM_Y,B
        move    A,x0
	    move	#>g_AdpcmDecSinkBufDescriptor,A
	    jsr	EffectsInit

        ; Init the interrupt vectors
        move    #>DacDriverIsr,x0
        move    #>DacDriverIsr,x1
        move    #>DecAdpcmImaModuleIsr,y0
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
; AdpcmDecoderModule
;*******************************************************************************
AdpcmDecoderModule

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

_UpdateLcdMessage
_checkForSync
        move    x:DecoderCSR,x0              ; copy contents of status register
        jset    #FileReadError,x0,_FileReadError ; error reading the file
        jclr    #Play,x0,_LCDdone            ; only update if playing (save MIPS)
        jset    #SongInfo,x0,_getSongCurrentTime
        bset    #SongInfo,x0                 ; prevents constant updating of same song info
        move    x0,x:DecoderCSR              ; SongInfo bit is cleared by opening of new song file

        move    #>MsgGeqSetCoef,r0
        move    y:Fg_Wave_dwSamplesPerSec,x0
        move    x0,y:MsgGeqParam
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

_getSongTotalTime
        ; updates the display of the elapsed/remaining song time
;        jsr     DecoderUpdateTotalTime       
_getSongCurrentTime
        ; updates the display of the elapsed/remaining song time
        jsr     DecoderUpdateCurrentTime     
_LCDdone

_checkABModeBoundaries
        move    x:DecoderCSR,x0
        ; must check Pause bit in case "just Paused" now
        jset    #Pause,x0,_CheckIfEndOfList   
        jsset   #ABMode_A,x0,CheckABBoundaries            ; if ABMode on, check the boundaries

_CheckIfEndOfList
        move    x:DecoderCSR,x0
        jset    #EndOfList,x0,_exitAdpcmDecoderModule
_CheckIfEndOfSong
         move   x:DecoderCSR,x0
         jclr   #EndOfSong,x0,_exitAdpcmDecoderModule

         move    y:AdpcmDecoderSinkBufDesc,r0
         move    y:<Const_ffffff,m0
         jsr     GetFilledSpace              ; returns: a==number of words in buffer
		 move    #>$A00,y0
		 cmp 	 y0,a
		 jgt     _exitAdpcmDecoderModule

        ;-------------------------------------------------------------
        ; Done With Song 
        ;-------------------------------------------------------------
_AdvanceToNextSong
        bclr    #EndOfSong,x:DecoderCSR

        jsr     StopCurrentSong              ; includes FadeOut & disable DAC
        jsr     DecoderCloseSong

        ; tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0
        move    #-1,m0
        jsr     SysPostMessage
        move    #MsgMenuSongTimeChange,r0
        move    #-1,m0
        jsr     SysPostMessage

        ; tell the parser to advance to next song and Play
        move    #>NEXT_SONG_PLAY_EOF,x0      ; EOF && PLAY
        move    x0,y:MsgParserNextParam
        move    #>MsgParserNextSong,r0
        move    #-1,m0
        jsr     SysPostMessage

        ; Free the DCLK to go to IDLE or to be used by other speed client
        move    #>SPEED_CLIENT_ADPCM_DEC,y0
        jsr     SysSpeedClockFree

        ; Ignore messages until the Decoder is reset.
        bset    #DecoderIgnoreMessages,x:DecoderSR

        ; Allow PSM/PlayerLib to call it next (if songs left)
        move    #>EVENT_MESSAGE,x0
        move    x0,y:AdpcmDecoderEventValue 

_exitAdpcmDecoderModule
        ; restore stack
        move    #UserScratchY,r5             ; pointer to the SP local save
        move    #_stackRestored,r6           ; setup the return address
        jmp     RestoreSystemStack           ; restore the hardware stack
_stackRestored

        ; Return value for executive (EVENT_NONE or EVENT_MESSAGE)
        move    y:AdpcmDecoderEventValue,a1 
        move    y:<Const_ffffff,m1
        rts

_FileReadError
        ; Stop the DAC & close the file
        jsr     StopCurrentSong
        jsr     DecoderCloseSong
        bclr    #FileReadError,x:DecoderCSR
        bset    #FileReadError,x:DecoderSR 

        ; Tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        ; Free the DCLK to be used by other speed clients or to IDLE
        move    #>SPEED_CLIENT_ADPCM_DEC,y0
        jsr     SysSpeedClockFree

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AdpcmDecoderEventValue
        jmp     _exitAdpcmDecoderModule

DecoderSetEventTimer
; If we want to return on the module timer OR message,
; this routine sets AdpcmDecoderEventValue up.
        move    #>EVENT_MESSAGE,a1
        move    #>500,x0
        jsr     ModuleSetTimer
        move    a1,y:AdpcmDecoderEventValue
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
        ; Now clear the song timings
        clr     a
        move    a,y:SongByteTotalHigh  ; high bytes
        move    a,y:SongByteTotalLow   ; low bytes

        ; Fade the Volume to Mute
        move    #>msFadeOut,x0
        jsr     MixerMasterFadeOut      ; fade volume down to mute
_muted

        move    y:Fg_Wave_wChannels,A
        asr     A   #>TARGET_MEM_Y,B
        move    A,x0
	    move	#>g_AdpcmDecSinkBufDescriptor,A
	    ; EffectsInit should be called before calling any DAC initialization to ensure the processing chain is loaded.
        jsr	EffectsInit

        jsr     DacDriverDisablePort

        ; clear dac memory
	jsr	DacClearMemory

        ; Set the status register
        bset    #Stop,x:DecoderSR
        bclr    #Pause,x:DecoderSR
        bclr    #Play,x:DecoderSR
        rts

;-------------------------------------------------
;DecoderUpdateTotalTime
; Updates the total time of the current song.
; Sends a message to the LCD to print it.
;        move    #-1,m4
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
        rts
_update
        move    x1,y:SongCurrentMinutes
        move    x0,y:SongCurrentSeconds
        move    #MsgMenuSongTimeChange,r0
        move    #-1,m0
        jsr     SysPostMessage
_getSongRemainTime
        jsr     GetSongRemainTime
_postMessageToLcd
        rts

;-------------------------------------------------
GetSongTime
; Inputs:  R4 = HighByte count ptr , M4 = linear
;          R5 =  LowByte count ptr , M5 = linear
;          Global y:InvSampleRate, actually a scaled InvByteRate, includes a 2^x factor
; Output:  X1 = Song Minutes
;          X0 = Song Seconds
;
; You can call this to get total song length or for current song position.
; This will only work when the decoder has synced up already.
        move    y:InvSampleRate,y0           ; scaled inverse BYTE RATE
        move    y:(r4),x1                    ; high byte count
        move    y:(r5),x0                    ; low byte count

        clr     b
        clr     a                            
        
        ; Dual Precision Multiply version not required as of apr 2, 02 ver. --------------------
        ; (Low byte count * inv rate + high byte count * inv rate)  gives totSeconds
        ; Warning: All interrupt subroutines must stop Dual Precision mode and restore mr on exit
        ;          ALU results are indeterminate while in DP mode except for DP ops.
;        jclr    #23,x0,_positive_ok          ; if sign bit cleared, no work needed
;        move    x0,a1
;        asr     a                            ; forced positive interp of LBC & lose LSB (ok)
;        move    a1,x0                        ; 

;_positive_ok
;        ori     #40,mr                       ; Enter Dual Precision mode for 2 word multiply
;        mac     x0,y0,a                      ; Low Byte Count * inverse rate -> A
;        mac     x1,y0,a        a0,x0         ; A += high byte count*inv rate. 
                                              ; Collect DP1 from a0 per 56k Fam Manual pg 3.17
                                              
;        move    a,b                          ; Collect DP3_DP2 from A 
        ; Analyze lower 2 words of result  DP1_DP0
;        move    b0,a1                        ; Place DP1 in a1
;        move    x0,a0                        ; Concatenate DP0 after DP1 in A = A1:A0 = DP1_DP0
;        andi    #$bf,mr                      ; Exit Dual Precision mode
;        nop
;        nop                                  ; non ALU op needed due to pipeline delay
;        asr     a                            ; converts frac to int        
;        -------------------------------------

_lowByteMath
        ; Low byte count * scaled inv byte rate gives total low byte word equiv seconds
        ; test sign bit before multiply
        jclr    #23,x0,_positive_ok          ; if lbc sign bit cleared, no work needed
        clr     a                            ;  else -- shift right, mpy, shift left (the LSB is lost but is insignificant)
        move    x0,a1                        ; 24bit Low Byte Count to a1
        asr     a                            ; shift Low Byte Count right (lose LSB -- negligible)
        move    a1,x0                        ; friendlier low byte count
        mpy     x0,y0,a                      ; lbc (int) * upScaledAdjustedSecPerByte (frac). result is A1 int : A0 frac
        asl     a                            ; shift left to undo above shift right 
        jmp     _do_high_word
_positive_ok
        mpy     x0,y0,a                      ; no adjustment necessary
_do_high_word

        ; A1:A0 has low_word_scaled_whole_seconds:fractional_seconds. Unscaled below with asr sequence.
        ; Single asr shift for Post-IntByInt-mpy not needed to get true result since this is int*frac         

        ; Note on above multiply and a max handled result size of 23 bits: 
        ; Low word byte count max is inverse 8kHz which allows for max low byte count of $FFFFFE.
        ; ($FFFFFE bytes/(4000 decimal bytes/sec))/(60sec/min) = 69.9 minutes file display time. 
        ; Note that reducing the scaling factor would increase this number by powers of 2, but
        ; this would cost a reduction of precision for the higher more often used sample rates. 
        ; Therefore this max time calc is deemed large enough for the max display time of a single 8kHz file.
        ; Note: Inclusion of the high byte count below now greatly extends the display time maximum.

        ; high byte count calculation
        
        move    a1,b1                        ; save low word secs. set up for MAC, treats as 24bit unsigned
        clr     a
        move    b1,a0                        ; "shift right 24 bits", low word seconds in its place
        mac     x1,y0,a                      ; Calc high word secs and accum with low for tot secs

        ; A = TotSeconds * scaling factor

        ; Undo the 2^9 normalization (built into inv rate constant)
        do      #4,_end_do_it
        asr     a
        asr     a
_end_do_it
        asr     a                            ; A1:A0 = total seconds

        ; Now we have A1:A0 = total int seconds.  
        ; See 'mpy of Frac & Int' section in Moto doc: Fractional & Integer Math using the 56k  
        ; 1  Fractional seconds already thrown out
        ; 2. A1 is expected to always be 0. If A1 was > 0, it would  
        ;    represent nearly 17 million seconds, which will not fit on the media, nor 
        ;    does a song or recording exist of that length.
        ; 3. So we only care about A0 seconds which ranges to max positive word size $7fffff

        move    a0,x0                        ; x0 = total number of seconds
        move    #>Sec2Min,y0                 ; (1/60) (min/sec) $022222
        mpy     x0,y0,a                      ; Makes a0 have int whole minutes
        clr     b            a1,x1           ; OUTPUT # 1 : x1 = number of whole minutes

        ; 1st check for 60-second boundary
        move    #>Min2Sec,a                  ; "60"
        cmp     x0,a                         ; compare totSeconds to 60
        jne     _finish
        clr     a                            ; then, inc minutes, set seconds = 0, and exit
        move    a,x0
        move    x1,a0
        inc     a
        move    a0,x1
        jmp     _exit

_finish
        move    #>Min2Sec,y0                 ; 60 sec/min
        mpy     x1,y0,a                      ; totMin * 60
        asr     a                            ; post integer mpy adjustment
        move    a0,y0                        ; y0 = number of seconds in multiples of 60
        move    x0,a                         ; total seconds in a
        sub     y0,a                         ; a-y0=totSec-totWholeMinInSecs, A= secs remaining after minutes
        move    a1,x0                        ; OUTPUT # 2 : x0 = number of seconds

        ; 2nd check for 60-second boundary
        move    #>Min2Sec,a                  ; "60"
        cmp     x0,a                         ; if num seconds = 60
        jne     _exit
        clr     a                            ; then, inc minutes, set seconds = 0
        move    a,x0
        move    x1,a0                        ; tot minutes incremented
        inc     a
        move    a0,x1                        ; OUTPUT # 1 : x1 = number of whole minutes
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
        move    #>1,x0                       ; borrow 1 minute to fix negative seconds
        sub     x0,a
        move    #>60,x0
        add     x0,b                         ; adjust the seconds remaining
_done
        move    a1,y:SongRemainMinutes       ; store result (minutes)
        move    b1,y:SongRemainSeconds       ; store result (seconds)
        rts

;-------------------------------------------------

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
        move    x:DecoderSR,x0
        jset    #DecoderIgnoreMessages,x0,_NextMessage

_ContinueProcessing
        ; Check for case of no songs to play
        move    y:TotalSongCount,a
        tst     a
        jne     _songsPresent
        jsr     DecABModeDisable        ; reset any AB mode settings
        move    #>EVENT_MESSAGE,x0      ; no songs avail -- only call on button push
        move    x0,y:AdpcmDecoderEventValue
        jmp     _NextMessage            ; ignore messages - no songs!

_songsPresent
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
        bclr    #DecoderIgnoreMessages,x:DecoderSR

;	    jsr     DecABModeDisable             ; reset any AB mode settings
        ; _ResetDecoder may execute often so try not to be a MSG sender here.

        ;NEED TO ADD MORE "RESET" stuff so that DECODER_RESET really does do what its supposed to
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
        bclr  #ABQuiet,x:DecoderCSR
        push    r1
        ; Configure CSR
        move    x:DecoderCSR,x0
        bclr    #Stop,x0
        bclr    #Ffwd,x0
        bclr    #Rwnd,x0
        bclr    #EndOfList,x0
        move    x0,x:DecoderCSR

        ; Check if RWD -- in which case we Skip the Block Header
;        move    x:ParserCSR,x0
;        jclr    #SkipBlockHeader,x0,_checkPause
;        jsr     Reset_internal_states
;        jsr     AdpcmInitBuffers             ; init the buffers, reset head/tail ptrs
_checkPause
        move    x:DecoderCSR,x0
        jclr    #Pause,x0,_checkPlayBit ; check if Paused
_Paused
        bclr    #Pause,x0
        move    x0,x:DecoderCSR         ; turn off "pause" mode

        move    #>SPEED_ADPCM,x1        ; Increase to SPEED_ADPCM if needed
        move    #>SPEED_CLIENT_ADPCM_DEC,y0  ; Speed Client ID
        jsr     SysSpeedIncrease

        jsr     DecoderFileSeek

        ; Turn Timer Module ON
        move    #>DecoderTimerNum,x0
        move    #_TimerEnabled_1,r0
        jmp     SysEnableTimer
_TimerEnabled_1
        ; wait for dac to turn on
_pollDac
        move    x:HW_DACCSR,x0
        jclr    #HW_DACCSR_TXEN_BITPOS,x0,_pollDac

        ; Fade in the Volume from Mute
        move    #>msFadeIn,x0
        jsr     MixerMasterFadeIn            ; fade volume in to previous level

        ; tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0
        move    #-1,m0
        jsr     SysPostMessage

        jsr     DecoderSetEventTimer         ; set up AdpcmDecoderEventValue for module timer

        pop     r1
        jmp     _NextMessage

_checkPlayBit
        jclr    #Play,x0,_beginPlay     ; if playing, switch to Pause
        bset    #Pause,x:DecoderCSR     ; msg to isr to "Pause"

        ; We need to be absolutely sure that the timer module will always be 
        ; enabled before this polling mechanism!
_pollPlaybit
        move    x:DecoderCSR,x0
        jset    #Play,x0,_pollPlaybit   ; wait for isr to enter "Pause" state

        ; Fade the Volume to Mute
        move    #>msFadeOut,x0
        jsr     MixerMasterFadeOut      ; fade volume down to mute
_muted

        move    y:Fg_Wave_wChannels,A
        asr     A   #>TARGET_MEM_Y,B
        move    A,x0
	    move	#>g_AdpcmDecSinkBufDescriptor,A
	    ; EffectsInit should be called before calling any DAC initialization to ensure the processing chain is loaded.
        jsr	EffectsInit

        jsr     DacDriverDisablePort    ; turn off the DAC

        ; Free the DCLK to be used by other speed clients or to IDLE
        move    #>SPEED_CLIENT_ADPCM_DEC,y0
        jsr     SysSpeedClockFree

        ; tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0
        move    #-1,m0
        jsr     SysPostMessage

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AdpcmDecoderEventValue

        pop     r1
        jmp     _NextMessage

_beginPlay
        ; necessary to notify decoder module to update song info (new song)
        bclr    #SongInfo,x:DecoderCSR  
        jsr     DecoderOpenSong

        jsr     DecAdpcmInit
		tst		A
		jeq		_ContinueUpdate


_StopPlay
        jsr     StopCurrentSong              ; includes FadeOut & disable DAC
        jsr     DecoderCloseSong

        ; tell the PlayerStateMachine  to advance to next song and Play
        move    #>NEXT_SONG_PLAY_EOF,x0      ; EOF && PLAY
        move    x0,y:MsgParserNextParam
        move    #>MsgParserNextSong,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        ;ignore messages until the Decoder is reset
        bset    #DecoderIgnoreMessages,x:DecoderSR

		; Initialization failed, so return cleanly
        pop     r1
        jmp     _NextMessage

_ContinueUpdate
        
        jsr     DecoderFileSeek

        jsr     DecoderUpdateCurrentTime        ; song time and number

        move    y:Fg_Wave_wChannels,A
        asr     A   #>TARGET_MEM_Y,B
        move    A,x0
	    move	#>g_AdpcmDecSinkBufDescriptor,A
	    ; EffectsInit should be called before calling any DAC initialization to ensure the processing chain is loaded.
        jsr	EffectsInit

        ; Init dac and decoder for new song
        jsr     DacDriverInit

        ; Enable the DAC interrupt
        bset    #HW_DACCSR_TXIEN_BITPOS,x:HW_DACCSR

         
        move    #>SPEED_ADPCM,x1    ; Change to SPEED_ADPCM. Increases if needed
        move    #>SPEED_CLIENT_ADPCM_DEC,y0
        jsr     SysSpeedIncrease

        ; Turn Timer Module ON
        move    #>DecoderTimerNum,x0
        move    #_TimerEnabled_2,r0
        jmp     SysEnableTimer
_TimerEnabled_2

        ; poll for dac on
_pollDac_2
        move    x:HW_DACCSR,x0
        jclr    #HW_DACCSR_TXEN_BITPOS,x0,_pollDac_2

        ; Fade in the Volume from Mute
        move    #>msFadeIn,x0
        jsr     MixerMasterFadeIn            ; fade volume in to previous level

        ; tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0
        move    #-1,m0
        jsr     SysPostMessage

        jsr     DecoderSetEventTimer         ; set up AdpcmDecoderEventValue for module timer

        pop     r1
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
        move    #-1,m0
        jsr     SysPostMessage

        ; tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0
        move    #-1,m0
        jsr     SysPostMessage
        move    #MsgMenuSongTimeChange,r0
        move    #-1,m0
        jsr     SysPostMessage

        ; Free the DCLK to be used by other speed clients or to IDLE
        move    #>SPEED_CLIENT_ADPCM_DEC,y0
        jsr     SysSpeedClockFree

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AdpcmDecoderEventValue

        jmp     _NextMessage

;-------------------------Ffwd Function-------------------------
_Ffwd
        bset    #ABQuiet,x:DecoderCSR
        bset    #Ffwd,x:DecoderCSR
        bclr    #Rwnd,x:DecoderCSR

        ; Open File (if necessary)
        move    x:DecoderCSR,x0
        jset    #FileIsOpen,x0,_FileOpenedForFfwd
        jsr     DecoderOpenSong
        jsr     DecAdpcmInit
        ; tell the menu module that the status has changed.
        ; this is necessary to update song name when crossing a boundary
        move    #>MsgDecoderStateChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage
_FileOpenedForFfwd

_Fast_forward
        move    y:AdpcmSecondstoSkip,x0
        move    y:Fg_Wave_dwAvgBytesPerSec,y0
        mpy     x0,y0,A    
        asr	A  
	move    y:SongByteTotalHigh,b  ; high bytes
        move    y:SongByteTotalLow,b0   ; low bytes
        add     b,a
        clr     b
        move	y:SongByteLengthHigh,B
        move	y:SongByteLengthLow,B0
        cmp     b,a                     ; if num bytes to skip < num bytes left, ffwd
        jlt     _bytesLeftPassed
_resetToStart
        ; End of File reached -- need next song
        jsr     DecABModeDisable             ; reset any AB mode settings
        jsr     StopCurrentSong
        jsr     DecoderCloseSong
        jsr     DecoderClearSinkBuffer             ; init the buffers, reset head/tail ptrs

        ; tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0
        move    #-1,m0
        jsr     SysPostMessage

        ; When moving to next song via _Ffwd, Free DCLK for other speed clients  
        move    #>SPEED_CLIENT_ADPCM_DEC,y0
        jsr     SysSpeedClockFree


        ; tell the parser to find next song and send FFWD message when done
        move    #>NEXT_SONG_FFWD,x0          ; get next song then ffwd
        move    x0,y:MsgParserNextParam
        move    #>MsgParserNextSong,r0
        move    #-1,m0
        jsr     SysPostMessage

        ;ignore messages until the Decoder is reset
        bset    #DecoderIgnoreMessages,x:DecoderSR

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AdpcmDecoderEventValue
        jmp     _NextMessage

_bytesLeftPassed
        move    a0,y:SongByteTotalLow   ; update count
        move    a1,y:SongByteTotalHigh
        jsr     DecoderUpdateCurrentTime        ; song time and number

_SeekFwd
        
        jsr     DecoderSeek

        jsr     DecoderClearSinkBuffer             ; init the buffers, reset head/tail ptrs
        ; post message to update the time
        move    #MsgMenuSongTimeChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AdpcmDecoderEventValue

        jmp     _NextMessage

;-------------------------Rwnd Function-------------------------
_Rwnd
        bclr    #Ffwd,x:DecoderCSR
        bset    #Rwnd,x:DecoderCSR
        ; Make sure the file is open
        move    x:DecoderCSR,x0
        jset    #FileIsOpen,x0,_FileOpenedForRwnd
        jsr     DecoderOpenSong     
        jsr     DecAdpcmInit
        ; tell the menu module that the status has changed.
        ; this is necessary to update song name when crossing a boundary
        move    #>MsgDecoderStateChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage
_FileOpenedForRwnd
		move	y:RewindMode,A				 ; Other decoders ignore this.
        tst     A       #0,x0
        jeq     _ContinueRwnd
        ; Else seek to end prior to rewinding.
		move	y:SongByteLengthLow,A
        move    A,y:SongByteTotalLow
		move	y:SongByteLengthHigh,A
        move    A,y:SongByteTotalHigh
        move    x0,y:RewindMode
_ContinueRwnd
        ; check if too close to beginning of file
        move    y:AdpcmSecondstoSkip,x0
        move    y:Fg_Wave_dwAvgBytesPerSec,y0
        mpy     x0,y0,A    
		asr		A
        move    y:SongByteTotalHigh,B
        move    y:SongByteTotalLow,b0
        sub     a,b
        jgt     _rewind
_goToPrevSong
        jsr     SysAllLinear
        jsr     DecABModeDisable             ; reset any AB mode settings
        jsr     StopCurrentSong
        jsr     DecoderCloseSong
        jsr     DecoderClearSinkBuffer             ; init the buffers, reset head/tail ptrs

        ; tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0
        move    #-1,m0
        jsr     SysPostMessage

        ; When moving to next song via _Rwnd, Free DCLK for other speed clients  
        move    #>SPEED_CLIENT_ADPCM_DEC,y0
        jsr     SysSpeedClockFree

        ; tell the parser to backup a song and send RWND message when done
        move    #>PREV_SONG_RWND,x0          ; get previous song then rewind from end
        move    x0,y:MsgParserPrevParam
        move    #>MsgParserPrevSong,r0
        move    #-1,m0
        jsr     SysPostMessage

        ;ignore messages until the Decoder is reset
        bset    #DecoderIgnoreMessages,x:DecoderSR

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AdpcmDecoderEventValue
        jmp     _NextMessage

_rewind
        move    b0,y:SongByteTotalLow
        move    b1,y:SongByteTotalHigh
        jsr     DecoderUpdateCurrentTime
	jsr     DecoderSeek
        jsr     DecoderClearSinkBuffer

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AdpcmDecoderEventValue
        jmp     _NextMessage

;-------------------------NextSong Function-------------------------
_NextSong
        ; Configure CSR
        bclr    #Ffwd,x:DecoderCSR
        bclr    #Rwnd,x:DecoderCSR
        jsr     DecABModeDisable        ; reset any AB mode settings

        move    x:DecoderCSR,x0
        jset    #Play,x0,_nextStop
        jset    #Pause,x0,_nextStop
        jsr     DecoderCloseSong

        ; When moving to Next Song, Free DCLK for other speed clients  
        move    #>SPEED_CLIENT_ADPCM_DEC,y0
        jsr     SysSpeedClockFree

        ; Tell the PSM/PlayerLib to advance to next song
        move    #>NEXT_SONG,x0          ; Button && NoPlay
        move    x0,y:MsgParserNextParam
        move    #>MsgParserNextSong,r0
        move    #-1,m0
        jsr     SysPostMessage
       ; tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        ;ignore any messages until we're reset
        bset    #DecoderIgnoreMessages,x:DecoderSR

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AdpcmDecoderEventValue
        jmp     _NextMessage
_nextStop
         jsr     StopCurrentSong
         jsr     DecoderCloseSong

        ; tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0
        move    #-1,m0
        jsr     SysPostMessage

        ; When moving to Next Song, Free DCLK for other speed clients  
        move    #>SPEED_CLIENT_ADPCM_DEC,y0
        jsr     SysSpeedClockFree

        ; Tell the PSM/PlayerLib to advance to next song and Play
        move    #>NEXT_SONG_PLAY_BUTTON,x0 ; Button && PLAY
        move    x0,y:MsgParserNextParam
        move    #>MsgParserNextSong,r0
        move    #-1,m0
        jsr     SysPostMessage

        ;ignore any messages until we're reset
        bset    #DecoderIgnoreMessages,x:DecoderSR


        move    #>EVENT_MESSAGE,x0
        move    x0,y:AdpcmDecoderEventValue
        jmp     _NextMessage

;-------------------------PrevSong Function-------------------------
_PrevSong
        ; Configure CSR
        bclr    #Ffwd,x:DecoderCSR
        bclr    #Rwnd,x:DecoderCSR
        jsr     DecABModeDisable             ; reset any AB mode settings

        ; When moving to Prev Song, Free DCLK for other speed clients  
        move    #>SPEED_CLIENT_ADPCM_DEC,y0
        jsr     SysSpeedClockFree
        move    x:DecoderCSR,x0
        jset    #Play,x0,_prevStop
        jset    #Pause,x0,_prevStop

        ; Song is stopped
        jsr     DecoderCloseSong

        ; Tell the PSM/PlayerLib to advance to next song and Play
        move    #>PREV_SONG,x0          ; PrevSong && Stop
        move    x0,y:MsgParserPrevParam
        move    #>MsgParserPrevSong,r0
        move    #-1,m0
        jsr     SysPostMessage
        move    #>MsgDecoderStateChange,r0 ; JEFFRO
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        ;ignore any messages until we're reset
        bset    #DecoderIgnoreMessages,x:DecoderSR

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AdpcmDecoderEventValue
        jmp     _NextMessage

_prevStop
        ; song was playing
        jsr     StopCurrentSong
        jsr     DecoderCloseSong

        ; tell the menu module that the status has changed.
        move    #>MsgDecoderStateChange,r0
        move    #-1,m0
        jsr     SysPostMessage

        ; Tell the PSM/PlayerLib to advance to next song and Play
        move    #>PREV_SONG_PLAY,x0     ; PrevSong && Play
        move    x0,y:MsgParserPrevParam
        move    #>MsgParserPrevSong,r0
        move    #-1,m0
        jsr     SysPostMessage

        ;ignore any messages until we're reset
        bset    #DecoderIgnoreMessages,x:DecoderSR

        move    #>EVENT_MESSAGE,x0
        move    x0,y:AdpcmDecoderEventValue

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
        jsr     DecSetPosBoundary       ; sets the position to the adpcm boundary
        move    a1,y:AdpcmABPositionAHigh    ; save high byte
        move    a0,y:AdpcmABPositionALow     ; save low byte
        jmp     _NextMessage
_checkB
        jset    #ABMode_B,x0,_disableABMode
        bset    #ABMode_B,x:DecoderCSR  ; second msg is for "B" location
        jsr     DecGetSongPos           ; returns byte position in A1:A0
;        jsr     DecSetPosBoundary       ; sets the position to the adpcm boundary
        move    a1,y:AdpcmABPositionBHigh    ; save high byte
        move    a0,y:AdpcmABPositionBLow     ; save low byte

        ; make sure A < B
        clr     b
        move    y:AdpcmABPositionAHigh,b1    ; B1 = AHigh
        move    y:AdpcmABPositionALow,b0     ; B0 = ALow
        cmp     a,b                     ; compare Bmode to Amode
        jlt     _NextMessage            ; Bmode > Amode
_disableABMode
        jsr     DecABModeDisable        ; clear bits, reset byte positions to 0.
        jmp     _NextMessage

;-------------------------SetEQ Function----------------------------
_SetEQ
; *** EQ is not functional for ADPCM ... yet.

;        ; Get the arguments
;        move    y:(r1)+,x0              ; Bass Corner Frequency
;        move    x0,x:FBass_Freq_o       ; Parthus mp3 decoder location
;        move    y:(r1)+,x0              ; Bass Enhancement
;        move    x0,x:FBass_Enhance_o    ; Parthus mp3 decoder location
;        move    y:(r1)+,x0              ; Treble Corner Frequency
;        move    x0,x:FTreble_Freq_o     ; Parthus mp3 decoder location
;        move    y:(r1)+,x0              ; Treble Enhancement
;        move    x0,x:FTreble_Enhance_o  ; Parthus mp3 decoder location
;        move    y:(r1)+,x0              ; Tone Attenuation
;        move    x0,x:
        ; we don't need circular mode to access queue buffer with r1
;        push    m1			;stmp6661
;        move    y:<Const_ffffff,m1	;stmp6661
_GetSongInfo
		; Not used. Only WMA uses it.
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
        jmp     (r6)                      ; return to AdpcmDecoderModule routine

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
	    dc	    _GetSongInfo

; Note: It is important that the entries in the jump table be in the
; same order as the Decoder message ID's which are defined in msgequ.inc

;*******************************************************************************
; DECODER-SPECIFIC FILE FUNCTIONS
; These functions are used by the decoder to perform file system actions.
;*******************************************************************************
DecAdpcmInit
        move    y:AdpcmInitFlag,A
        tst     A   #0,A
        jne     _Exit
        move    A,y:AdpcmInitFlag

	jsr     SysAllLinear
        ; Make sure we start at the right place
        clr     a                            ; offset of 0
        move    A,y:wave_file_offset
        move    y:AdpcmDecoderFileHandle,a2
        jsr     FileSeek

        jsr     SysAllLinear
        jsr     Fwaveparser
		tst		A
		jeq		_Continue
        ; Try to compute song information anyway.
		jsr     GetInvBitRate		; Get the Inverse of Bit Rate.
        move    y:<Const_ffffff,A
		bset  	#0,y:CurrentSongBad            ; handle a bad file read
		jmp		_Exit
_Continue
		jsr		SetDecoderType
		tst		A
		jne		_Exit
        jsr     DecoderClearSinkBuffer
        jsr     Init_ADPCM_States

        ; Header won't be big.
		; Now find the song Length
		move	y:Fg_Wave_datachunksize,A
		move	A,y:SongByteLengthLow
		move	y:(Fg_Wave_datachunksize+1),A
		move	A,y:SongByteLengthHigh

		jsr     GetInvBitRate		; Get the Inverse of Bit Rate.

        jsr     SetDacSRR
_Exit
        rts


;-----------------------------------------------------------------
; DecoderFileSeek
; Input: A1:A0 has the value to seek (0 will seek to beginning)
;-----------------------------------------------------------------
DecoderFileSeek
        jsr     SysAllLinear
        ; Always seek to zero first.
        clr     a
        move    y:AdpcmDecoderFileHandle,a2
        jsr     FileSeek
        
        jsr     DecoderSeek

         ; Atlast seek.
        jsr     SysAllLinear
        move    y:AdpcmDecoderFileHandle,a2
        jsr     FileSeek
        rts

;-----------------------------------------------------------------
; DecoderSeek
;-----------------------------------------------------------------

DecoderSeek

        move    y:SongByteTotalHigh,a  ; high bytes
        move    y:SongByteTotalLow,a0   ; low bytes
        move    y:(Fg_Wave_data_offset+1),b
        move    y:Fg_Wave_data_offset,b0
        sub     b,a        #0,x0
        tlt     x0,a                     ; We have reached the beginning of the file.
        jlt     _Continue


        move    y:Fg_Wave_wBlockAlign,y0
        ; Compute blocks to skip
        and     #$FE,ccr      
        rep     #25
        div     y0,A
        move    A0,x0
        ; Convert blocks to bytes
        mpy     x0,y0,A
        asr     A                           ; Account for default left shift.
_Continue
        ; Now adjust the samples remaining.
	move	y:SongByteLengthHigh,B
	move	y:SongByteLengthLow,B0
       	sub     A,B
	move	B0,y:Fg_Wave_datachunksize
        move	B1,y:(Fg_Wave_datachunksize+1)

        ; Now add the Data chunk offset.
        move    y:(Fg_Wave_data_offset+1),b
        move    y:Fg_Wave_data_offset,b0
        add     B,A


        rts

;---------------------------------------------------------------------------------------------


;-----------------------------------------------------------------
; DecoderOpenSong
;
;-----------------------------------------------------------------
DecoderOpenSong
        ; if the song is already open then just exit
        move    x:DecoderCSR,x0
        jclr    #FileIsOpen,x0,_notYetOpen
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

	if (@DEF('USE_PLAYLIST3'))
;        move    y:CurrentDirDevId,a2         ; card number for current directory
;        move    #Fg_CurrentSongName,a1          ; base address of song name
;        or      x0,a                         ; A2 = dev#, A1=memspace/file name buffer, A0=0
;        move    #>READMODE,a0 
;;        move    #>READMODE,a0                ; A0 = file open attribute (Read Only)
		move    #Fg_CurrentSongFastkey,r0
		move    y:(r0)+,A0
		move    y:(r0),A1
        move    #>READMODE,b                ; A0 = file open attribute (Read Only)
		;iHandle = FastOpen(g_CurrentSongFastkey,(_packed char *)"r");
		jsr FFSFastOpen
                       ; A0 = file open attribute (Read Only)
        ;jsr     FILEOpen
	else  ;(@DEF('USE_PLAYLIST3'))
	  if (@DEF('USE_PLAYLIST5'))
  		move    #Fg_CurrentFastKey,r0 
		move    y:(r0)+,A0
		move    y:(r0),A1
        move    #>READMODE,b                ; A0 = file open attribute (Read Only)
        jsr FFSFastOpen				;;iHandle = FastOpen(g_CurrentSongFastkey,(_packed char *)"r");
	  else   ;(@DEF('USE_PLAYLIST5'))

        move    y:CurrentDirDevId,a2         ; card number for current directory
        move    #Fg_CurrentSongName,a1          ; base address of song name
        or      x0,a                         ; A2 = dev#, A1=memspace/file name buffer, A0=0
        move    #>READMODE,a0                ; A0 = file open attribute (Read Only)
        jsr     FILEOpen
	  endif  ;(@DEF('USE_PLAYLIST5'))
	endif ;(@DEF('USE_PLAYLIST3'))

        jcc     _fileOpened
        error                                ; ERROR - file did not open (TBD -- ADD CODE!!)
_fileOpened
        move    a1,y:AdpcmDecoderFileHandle       ; store the handle for the file
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
        move    A,y:AdpcmInitFlag
        move    y:AdpcmDecoderFileHandle,a2
        jsr     FileSeek
        rts


;-----------------------------------------------------------------
; DecoderCloseSong
; This function will use the AdpcmDecoderFileHandle to close the file.
;-----------------------------------------------------------------
DecoderCloseSong
        
        clr     a                           
        move    A,y:AdpcmInitFlag
        move    x:DecoderCSR,x0
        jclr    #FileIsOpen,x0,_exit ; don't call FileClose if song file isn't open
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
        move    y:AdpcmDecoderFileHandle,a2       ; pass the file handle to the function
        jsr     FILEClose
        jcc     _fileClosed
        ; FILEClose returned an err code in A1. Some codes are non-critical. 
        ; consider checking A1 here for any critical FILEClose errors & handling.
        ;error              Iff DEBUG build: break. Iff RETAIL build: reset DSP        
_fileClosed
        move    x:DecoderCSR,x0
        bclr    #FileIsOpen,x0
        move    x0,x:DecoderCSR ;prevent attempts to close file multiple times w/o opening
_exit
        ;*** TBD : clear song parameters?
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
        move    x0,y:AdpcmABPositionAHigh
        move    x0,y:AdpcmABPositionALow
        move    x0,y:AdpcmABPositionBHigh
        move    x0,y:AdpcmABPositionBLow

        ; post msg to menu to remove AB icon
        move    #>MsgDecoderABModeOff,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

;        bclr #SkipBlockHeader,x:ParserCSR

        rts

DecGetABModeBPos
; Returns: B1 = AdpcmABPositionBHigh , B0 = AdpcmABPositionBLow
        clr     b
        move    y:AdpcmABPositionBHigh,b1
        move    y:AdpcmABPositionBLow,b0
        rts

DecSetSongPosA
        move    x:DecoderCSR,x0
        jset    #ABQuiet,x0,_skip
        jsr     StopCurrentSong
        bclr    #Stop,x:DecoderCSR

_skip
        ; Seek to Zero in File
        jsr     SysAllLinear
        clr     a
        move    y:AdpcmDecoderFileHandle,a2
        jsr     FileSeek
        jcc     _atZero
        error
_atZero
; Sets the song position to AB position "A"
        clr     a
        move    y:AdpcmABPositionAHigh,a1
        move    y:AdpcmABPositionALow,a0
        tst     a
        jeq     _AZero                    ; if A was at very beginning, then we must process block header
;        bset    #SkipBlockHeader,x:ParserCSR
_AZero
        move    a1,y:SongByteTotalHigh
        move    a0,y:SongByteTotalLow
        jsr     SysAllLinear
        move    y:AdpcmDecoderFileHandle,a2
        jsr     FileSeek
        jcc     _atALocation
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

DecSetPosBoundary
; Sets the position to the adpcm boundary
; Input: A1:A0 has song positions
; Output: A1:A0 has a0 fixed on the song boundary
        ; When rewinding to a previous ADPCM selection, you must adjust so that the 
        ; pointer is on a block alignment. We are assuming 512 byte blocks.
        move    #>$fffe00,x0                 ; assumes Dec_nblockalign = 512 ($200)
        clr     b
        move    a0,b1                        ; b1 = low value of song bytes
        and     x0,b                         ; line up on a block boundary
        move    y:wave_header_size,x0
        add     x0,b                         ; offset by header (takes care of RIFF header)

        move    y:SongByteLengthLow,x0       ; x0 = total bytes in file (LOW)
        cmp     x0,b
        jlt     _storeResult
        move    #>512,x0                     ; make smaller
        sub     x0,b
_storeResult
        move    b1,a0
        rts


DecoderClearSinkBuffer
        ; init buffer head / tail to beginning
        move    #g_AdpcmDecSinkBufDescriptor,r0      ; SINK buffer descriptor ptr
        move    #>$ffff,m0
        jsr     InitBufferEmpty
        ; clear the sink buffer for adpcm decode routines
        clr     a
        move    y:<Const_ffffff,m3
        move    #>g_AdpcmDecSink_mbuf,r3
        move    #>g_AdpcmDecSinkBufSize/2,x0
        do      x0,_clear_pcmbuffer
        move    a,y:(r3)+
        move    a,y:(r3)+
_clear_pcmbuffer
        nop
        rts

;---------------------------------------------------------------------------------------------


Init_ADPCM_States
        ; First initialize the common portions
        ; Next call the corresponding state functions.
        ; This is not the last block
        jsr     SysAllLinear
        clr     A
        move    A,y:Last_block_flag
        ; By default decode Samples per block
        move    y:Fg_Wave_wnSamplesPerBlock,A
        move    A,y:SamplestoDecode
        ; By default input buffer should be block align in size.
        move    y:Fg_Wave_wBlockAlign,A
        move    A,y:inputbufsize 	
        if @def('IMA_ADPCM')
        jsr     FInit_IMA_ADPCM_States
        endif        
        if @def('MS_ADPCM')
        jsr     FInit_MS_ADPCM_States
        endif        

;*******************************************************************************
;  Function:
;             GetInvBitRate
;  Description:
;             The output is compatible to MP3.
;             Called only at beginning of file playback.
;             Uses Direct division. 
;  Parameters passed: 
;		      Uses navgbytespersec
;  Registers Trashed: 
;             x0,A,B
;  Return Values:
;             Writes y:InvSampleRate variable.
;*******************************************************************************
;--------------------------------------------------
; Return value in A0

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

; Picked up from the Parser.
SetDacSRR
        move    y:Fg_Wave_dwSamplesPerSec,x0
        move    #44100,a
        cmp     x0,a
        jne     _sr48k
        move    #DAC44_1kHz,y0
        jmp     _loadDACSRR
_sr48k
        move    #48000,a
        cmp     x0,a
        jne     _sr32k
        move    #DAC48kHz,y0
        jmp     _loadDACSRR
_sr32k
        move    #32000,a
        cmp     x0,a
        jne     _sr24k
        move    #DAC32kHz,y0
        jmp     _loadDACSRR
_sr24k
        move    #24000,a
        cmp     x0,a
        jne     _sr22050
        move    #DAC24kHz,y0
        jmp     _loadDACSRR
_sr22050
        move    #22050,a
        cmp     x0,a
        jne     _sr16k
        move    #DAC22_05kHz,y0
        jmp     _loadDACSRR
_sr16k
        move    #16000,a
        cmp     x0,a
        jne     _sr12k
        move    #DAC16kHz,y0
        jmp     _loadDACSRR
_sr12k
        move    #12000,a
        cmp     x0,a
        jne     _sr11025
        move    #DAC12kHz,y0
        jmp     _loadDACSRR
_sr11025
        move    #11025,a
        cmp     x0,a
        jne     _sr8k
        move    #DAC11_025kHz,y0
        jmp     _loadDACSRR
_sr8k
        move    #8000,a
        cmp     x0,a
        jne     _ExitDacSRR
        move    #DAC8kHz,y0
        jmp     _loadDACSRR

_loadDACSRR
        move    y0,x:HW_DACSRR          ; load current song sample rate into DacSrr
        clr     A
_ExitDacSRR
        rts



;--------------------------------------------------------------------------
SetDecoderType
		move	y:Fg_Wave_wFormatTag,B
        if  @def('MS_ADPCM')
		move	#>WAVE_FORMAT_ADPCM,x0
		cmp		x0,B	#Decode_MS_ADPCM_Block,r6
		jeq		_EndDecodeBlock
        endif

		if  @def('IMA_ADPCM')
		move	#>WAVE_FORMAT_IMA_ADPCM,x0
		cmp		x0,B		#Decode_IMA_ADPCM_Block,r6
		jeq		_EndDecodeBlock
        endif

        if  @def('G721_ADPCM')
		move	#>WAVE_FORMAT_G721_ADPCM,x0
		cmp		x0,B		#Decode_G721_ADPCM_Block,r6
		jeq		_EndDecodeBlock
        endif		

        if  @def('WINDOWS_PCM')
		move	#>WAVE_FORMAT_PCM,x0
		cmp		x0,B		#Decode_PCM_Block,r6
		jeq		_EndDecodeBlock
        endif		
		
		; If it hits this, it is an unknown algorithm. Disable decoding.
		move	y:<Const_000001,A
		rts

_EndDecodeBlock
		clr		A    r6,y:AdpcmDecoderPointer
		rts
        endsec


