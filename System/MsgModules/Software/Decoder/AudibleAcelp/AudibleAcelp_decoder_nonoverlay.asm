;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000/2001
;
; STMP3410 ISR-level Decoder Module (CANNOT be overlayed)
;///////////////////////////////////////////////////////////////////////////////
        section Acelp_DecoderNonOverlay
 

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
    include "AudibleAcelpdec.inc"
    include "const.xref"
    ;include "acelpdecodermem.xref"
    include "sysmisc.xref"
    ;include "acelpmem.xref"
    include "bufdesc.xref"
    include "mixer.xref"
    include "systimers.xref"
    list
    page    132,60,0,0,0


;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////
       GLOBAL  AcelpDecoderModuleTimerDisable
       GLOBAL  AcelpDecoderModuleIsr
       GLOBAL  AcelpGetInvBitRate    

        ; This functions should not be used. Placed to satisfy the linker. Reported to VAge.
       Global  Fprintf,Fexit


;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////
       EXTERN    AcelpDecoderFileHandle
       EXTERN    FAcelpDecProcess
       EXTERN    AcelpDecIsrSp
	   EXTERN    Acelp_BitRate
	   EXTERN    FUpdateSongByteTotal
	   EXTERN    g_bAudiblePlayThrough

       EXTERN    CurrentSongBad

;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;   Code
;///////////////////////////////////////////////////////////////////////////////
    org     p,"DECMODACELP_DecoderNonOverlay_P":

;*******************************************************************************
;  Function:
;             AcelpDecoderModuleTimerDisable
;  Description:
;              Disable ACELPModuleISR interrupt timer.
;  Parameters passed: 
;              None
;  Registers Trashed: 
;             x0,r0,A
;  Return Values:
;             None
;*******************************************************************************
AcelpDecoderModuleTimerDisable
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
;             AcelpDecoderModuleIsr
;  Description:
;              ACELP ISR Module
;  Parameters passed: 
;              None
;  Registers Trashed: 
;             None. Registers backed up on stack.
;  Return Values:
;             None
;*******************************************************************************
AcelpDecoderModuleIsr
        ; save off all registers
        move    r6,y:IntL1_SaveR6            ; r6 used for return mechanism
        move    #_DecoderSaveRegReturn,r6    ; setup the return address
        jmp     SaveRegs                     ; save the remaining registers
_DecoderSaveRegReturn
        jsr     SysAllLinear             ; ACELP Decoder always assumes linear modulo.    
        ; save off stack
        move    #_DecoderSaveStackReturn,r6  ; setup the return address
        move    #AcelpDecIsrSp,r5                 ; pointer to the SP local save
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
        ; Error Code in A, B contains bytes consumed.
        jsr     FAcelpDecProcess            ; Decode a sub-frame.
        tst    A                ; Check the return value.
        jeq    _incrementByteCount        
        bset    #EndOfSong,x:DecoderCSR        ; End of song reached or Error.
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
        jsr     AcelpDecoderModuleTimerDisable    ; turn off timer
_restore_stack
        ; Setup Timer0 Module for use by decoder    
        move    #>DECODER_TIMER_SYNC_COUNT,x1  ; Call every 0.5 ms, as often as possible.
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
        move    #AcelpDecIsrSp,r5                 ; pointer to the SP local save
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
;             AcelpGetInvBitRate
;  Description:
;              Uses the filehdr_bitrate information decoded from ACELP file header
;             to compute inverse bit rate.  
;             The output is compatible to MP3.
;             Called only at beginning of file playback.
;             Uses Direct division. 
;  Parameters passed: 
;              Uses filehdr_bitrate
;  Registers Trashed: 
;             x0,A
;  Return Values:
;             Writes y:SongInvBitRate variable.
;*******************************************************************************
;--------------------------------------------------
; Return value in A0

AcelpGetInvBitRate
    clr    A    y:Acelp_BitRate,x0       ; ###CHANGE
    move    y:<Const_001000,A0          
     and     #$FE,CCR                  ; clear any carry bit if present    
    rep    #$30
    div    x0,A
    add     x0,a 
    move    A0,y:SongInvBitRate
    rts       




;*******************************************************************************
;  Function:
;             DecoderByteCounter
;  Description:
;              Counts how far the song has advanced in terms of bytes.
;  Parameters passed: 
;              B contains bytes decoded
;  Registers Trashed: 
;             A,B
;  Return Values:
;             None
;*******************************************************************************
DecoderByteCounter
	jsr		FUpdateSongByteTotal
    move    y:SongByteTotalHigh,a1
    move    y:SongByteTotalLow,a0
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
	move	#1,a1
	move	a1,x:g_bAudiblePlayThrough
_continue_decoding_song
    rts
    


; Not supported. The library must not use these.
; Placed here to satisfy the linker.
Fprintf
        rts
        
Fexit   
        rts                

        endsec

    