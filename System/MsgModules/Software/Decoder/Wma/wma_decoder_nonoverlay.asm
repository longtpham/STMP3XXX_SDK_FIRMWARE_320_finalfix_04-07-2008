;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000/2001
;
; STMP3410 ISR-level Decoder Module (CANNOT be overlayed)
;///////////////////////////////////////////////////////////////////////////////
        section Wma_DecoderNonOverlay
 

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////
    nolist
    include "irqequ.inc"
    include "sysequ.inc"
    include "hwequ.inc"
    include "dacequ.inc"
    include "msgequ.inc"
    include "sysmacro.asm"
    include "systrace.asm"
    include "decoderequ.inc"
    include "stack.xref"
    include "stackmem.xref"
    include "Wmadec.inc"
    include "const.xref"
	include "wmadecodermem.xref"
	include "sysmisc.xref"
    include "wmamem.xref"
    include "wmalib.xref"
	include "bufdesc.xref"
	include "mixer.xref"
	include "systimers.xref"
    list
    page    132,60,0,0,0


;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////
       GLOBAL  WmaDecoderModuleTimerDisable
       GLOBAL  WmaDecoderModuleIsr
       GLOBAL  GetInvBitRate	

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////
       EXTERN    WmaDecoderFileHandle
       EXTERN    WmaDecProcess
       EXTERN    WmaDecIsrSp

       EXTERN    FILEClose
       EXTERN    FILEOpen
       EXTERN    FileEOF
       EXTERN    FileRead

       EXTERN    CurrentSongBad
       EXTERN	 WMA_File_Offset	
;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;   X Memory
;///////////////////////////////////////////////////////////////////////////////
       	org     x,"DECWMAMOD_DecoderNonOverlay_X":

;///////////////////////////////////////////////////////////////////////////////
;   Y Memory
;///////////////////////////////////////////////////////////////////////////////
	org     y,"DECWMAMOD_DecoderNonOverlay_Y":

;///////////////////////////////////////////////////////////////////////////////
;   Code
;///////////////////////////////////////////////////////////////////////////////
	org     p,"DECMODWMA_DecoderNonOverlay_P":

;*******************************************************************************
;  Function:
;             WmaDecoderModuleTimerDisable
;  Description:
;		      Disable WMAModuleISR interrupt timer.
;  Parameters passed: 
;		      None
;  Registers Trashed: 
;             x0,r0,A
;  Return Values:
;             None
;*******************************************************************************
WmaDecoderModuleTimerDisable
        ; CANNOT be overlayed, called by ISR.

        ; Hardware Workaround -- Disable IVL at HW_IPR
        clr     a
        move    #>DECODER_IVL_MASK,a1
        move    x:HW_IPR,x0
        push    x0
        and     x0,a
        move    a1,x:HW_IPR                     ; disables decoder timer IVL

        ; Disable Timer0 Module for use by decoder
        move    #>DecoderTimerNum,x0
        move    #_TimerDisabled,r0
        jmp     SysDisableTimer
_TimerDisabled

        ; Hardware Workaround -- Restore IVL at HW_IPR
        pop     x0
        move    x0,x:HW_IPR                      ; re-enables the IVL line of decoder

        rts

;*******************************************************************************
;  Function:
;             DecoderByteCounter
;  Description:
;		      Counts how far the song has advanced in terms of bytes.
;  Parameters passed: 
;		      Uses WMA_File_Offset, to find the last accessed location in the file
;  Registers Trashed: 
;             A,B
;  Return Values:
;             None
;*******************************************************************************
DecoderByteCounter

 	move	L:WMA_File_Offset,A	          ; Get the file offset	
 	move    a1,y:SongByteTotalHigh
    move    a0,y:SongByteTotalLow
    ; total bytes in A1:A0
    ; 2 - Load B1:B0 with the song length in bytes
    clr     b
    move    y:SongByteLengthHigh,b1       ; high bytes of song length
    move    y:SongByteLengthLow,b0        ; low bytes of song length

    ; 3 - CMP total bytes expected to total bytes so far
    cmp     a,b
    jgt     _continue_decoding_song
_done_with_song
    bset    #EndOfSong,x:DecoderCSR
_continue_decoding_song
    rts
	

;*******************************************************************************
;  Function:
;             WmaDecoderModuleIsr
;  Description:
;		      WMA ISR Module
;  Parameters passed: 
;		      None
;  Registers Trashed: 
;             None. Registers backed up on stack.
;  Return Values:
;             None
;*******************************************************************************
WmaDecoderModuleIsr
        ; Clear the DM bit (double-precision multiply mode, bit 6) of the Mode Register
        ; This will be returned to its previous state when the RTI occurs (since the SR
        ; will be popped back off System Stack Low (SSL).
        ; Accumulator behavior is unpredictable when the DM bit is set.
        ; Next cycle after clearing it should NOT be an ALU instruction.
        andi #$BF,mr

        ; save off all registers
        move    r6,y:IntL1_SaveR6            ; r6 used for return mechanism
        move    #_DecoderSaveRegReturn,r6    ; setup the return address
        jmp     SaveRegs                     ; save the remaining registers
_DecoderSaveRegReturn
        jsr     SysAllLinear		     ; WMA Decoder always assumes linear modulo.	
        ; save off stack
        move    #_DecoderSaveStackReturn,r6  ; setup the return address
        move    #WmaDecIsrSp,r5                 ; pointer to the SP local save
        jmp     SaveSystemStack              ; save the hardware stack
_DecoderSaveStackReturn

_DecoderStateMachine
        move    x:DecoderCSR,x0
        jset    #Stop,x0,_IsrStopSequence
        jset    #Pause,x0,_IsrPauseSequence

_IsrPlaySequence
        bset    #Play,x:DecoderCSR

        ; Set the status register
        bclr    #Stop,x:DecoderSR
        bclr    #Pause,x:DecoderSR
        bset    #Play,x:DecoderSR

        ; enable the dac. 
        move    x:HW_DACCSR,x0
        jset    #HW_DACCSR_TXEN_BITPOS,x0,_checkFileOK
        bset    #HW_DACCSR_TXEN_BITPOS,x:HW_DACCSR

_checkFileOK
        move    y:CurrentSongBad,x0
        jclr    #0,x0,_checkForData
        bset    #EndOfSong,x:DecoderCSR     ; force to end of bad file
        jmp     _IsrStopSequence

_checkForData
_callDecoder
	if	@def('PASSTHRU')
        jsr     PassThruProcess		     	; Play back a tone to test rest of the system.
	else
        jsr     WmaDecProcess			; Decode a sub-frame.
	tst	A				; Check the return value.
	jeq	_incrementByteCount		
        bset    #EndOfSong,x:DecoderCSR		; End of song reached or Error.
	endif
_incrementByteCount
        jsr     DecoderByteCounter           	
_checkIfTrackDone
        move    x:DecoderCSR,x0
        jclr    #EndOfSong,x0,_restore_stack   ; continue decoding song if bit not yet set

_IsrStopSequence
        ; Set the status register
        bset    #Stop,x:DecoderSR
        bclr    #Pause,x:DecoderSR
        bclr    #Play,x:DecoderSR
        jmp     _decoderTimerOff
_IsrPauseSequence
        ; Set the status register
        bset    #Pause,x:DecoderSR
        bclr    #Play,x:DecoderSR
_decoderTimerOff
        move    x:DecoderCSR,x0              ; copy contents of status register
        jclr    #EndOfSong,x0,_ok
_ok
        ; if done decoding song, turn timer off
        ; user level will turn dac off before advancing to next song
        bclr    #Play,x:DecoderCSR           ; clear to avoid "Pause" on next song
        jsr     WmaDecoderModuleTimerDisable    ; turn off timer
_restore_stack
        ; Setup Timer0 Module for use by decoder	
	    move	#>DecoderTimerCountWMADec,x1  ; Call every 0.5 ms, as often as possible.
        move    #>DecoderTimerNum,x0         ; input to timer routine
        move    #_TimerCountLoaded,r0

        jmp     SysLoadTimerCount
_TimerCountLoaded
        ; reset the isr for next time
        move    #>DecoderTimerNum,x0
        move    #_TimerReset,r0
        jmp     SysClearTimerIrq
_TimerReset
        ; restore stack
        move    #WmaDecIsrSp,r5                 ; pointer to the SP local save
        move    #_stackRestored,r6           ; setup the return address
        jmp     RestoreSystemStack           ; restore the hardware stack
_stackRestored

        ; restore all registers
        move    #DecoderRestoreRegReturn,r6  ; setup the return address
        jmp     RestoreRegs                  ; restore registers
DecoderRestoreRegReturn
        move    y:IntL1_SaveR6,r6            ; restore minimal reg set
        rti

;*******************************************************************************
;  Function:
;             GetInvBitRate
;  Description:
;		      Uses the filehdr_bitrate information decoded from WMA file header
;             to compute inverse bit rate.  
;             The output is compatible to MP3.
;             Called only at beginning of file playback.
;             Uses Direct division. 
;  Parameters passed: 
;		      Uses filehdr_bitrate
;  Registers Trashed: 
;             x0,A
;  Return Values:
;             Writes y:SongInvBitRate variable.
;*******************************************************************************
;--------------------------------------------------
; Return value in A0

GetInvBitRate
	clr	A    y:filehdr_bitrate,x0
	move	y:<Const_001000,A0	  	
 	and 	#$FE,CCR    	          ; clear any carry bit if present	
	rep	#$30
	div	x0,A
	add 	x0,a 
	move    A0,y:SongInvBitRate
    rts       


        endsec

    