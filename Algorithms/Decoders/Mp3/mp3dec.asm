;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2003
;
; STMP MP3 Decoder         
;///////////////////////////////////////////////////////////////////////////////

        section DECAPP_Mp3Dec

        ;
        ; Includes
        ;

        include "project.inc"        
        include "sysequ.inc"
        include "hwequ.inc"
        include "mp3dec.inc"
        include "dacequ.inc"
        include "decoderequ.inc"
        include "mp3mem.xref"
        include "stack.xref"
        include "stackmem.xref"
        include "const.xref"
	    include "decodermem.xref"
        include "bufdesc.xref"
        include "sysmem.xref"
        include "sysmacro.asm"

        ; Externally visible routines in this file
        ;
        GLOBAL    Mp3DecReset
        GLOBAL    Mp3DecInit
        GLOBAL    Mp3DecProcess
        GLOBAL    MpegDecFree_SyncThresholdBase
        GLOBAL    MpegDecSet_SyncThresholdBase
        
        ;
        ; External References
        ;
        EXTERN    FHIO_LevelL_o,FHIO_LevelR_o
        EXTERN    FBass_Enhance_o,FBass_Freq_o,FTreble_Enhance_o,FTreble_Freq_o
        EXTERN    FMP3_Reset_asm,FMP3_Decode_asm
        EXTERN    Fgrancount_o
        EXTERN    FLeft_Meter_o
        EXTERN    FRight_Meter_o
        EXTERN    Fbitbuf_data_demand_o,FPCM_space_required_o,Fstatus_o,Fsampfreq_o
        EXTERN    Fbitrate_o
        EXTERN    g_BaseWasResetOnSyncLoss
        EXTERN    g_FileIsMp3         
        EXTERN    Mp3SampFreqTable,DacSampFreqTable
        EXTERN    Falgorithm_o
        EXTERN    CurrentSongBad
        EXTERN 	  g_wFilePos_StartTryingToSyncHigh,g_wFilePos_StartTryingToSyncLow
        EXTERN    g_Mp3TimerCount,FerrorLogBasePtr_o
        EXTERN    Mp3strictcomplianceFlag

        ;
        ; Code Start
        ;
		org     p,"DECAPP_Mp3Dec_P":


;*******************************************************************************
; Mp3DecReset  :: Reset Routine for MP3 Decoder : Called at startup
;*******************************************************************************
Mp3DecReset
        ; initialize Parthus decoder locations to proper reset values
        ; init the volume controls
        clr    a
        move   a,x:FHIO_LevelL_o             ; volume control Left
        move   a,x:FHIO_LevelR_o             ; volume control Right

        ; init other control locations
        move    a,x:Fgrancount_o             ; granule count
        move    a,x:FLeft_Meter_o            ; Left channel meter
        move    a,x:FRight_Meter_o           ; Right channel meter
        jsr     Mp3DecInit

        ; clear the "file is mp3" flag and the "Base Was Reset OnSyncLoss" flag
        clr     a
        move    a,y:g_FileIsMp3         ; must be reset for each song. If = 1, song is mp3.
        move    a1,y:g_BaseWasResetOnSyncLoss

        ; Mark byte position in file where decoder started looking for sync
        jsr     MpegDecSet_SyncThresholdBase   
      
        ; NOTE: This was set to the current filepos byte value and not to zero in 
        ; case we just rewinded over a border. We must count the bytes from _where_
        ; we start looking for sync, not from _0_.
        rts

;*******************************************************************************
; Mp3DecInit  :: Initialization Routine for MP3 Decoder : Called for each new song
;*******************************************************************************
Mp3DecInit
        ; Set the following flag to 1, if strict MPEG compliance is desired.
        ; Many encoders generate non-compliant streams, which may be decoded, if the
        ; following flag is cleared.
        clr     A
        move    A,x:Mp3strictcomplianceFlag
        jsr     Mp3InitSource           ; Setup the Source Buffer & Pointers       
        jsr     Mp3InitSink             ; Setup the Sink Buffer & Pointers
        jsr     FMP3_Reset_asm          ; Call Parthus MP3 decoder reset

        ; copy updated pointers
        move    x:Fbitbuf_readptr,x0
        move    x0,y:g_pMp3DataSrcBufTail       ; update the TAIL ptr
	    ; Note: we don't update the Head Ptr because it was init'd to half full
        rts

Mp3InitSource
        ; setup Source Buffer Pointers
        move    y:g_Mp3DataSrcBufDescriptor,x0  ; BASE address
        move    x0,x:Fbitbuf_addr
        move    y:g_pMp3DataSrcBufMod,x0        ; buffer MODULO
        move    x0,x:Fbitbuf_mod
        move    y:g_pMp3DataSrcBufSize,x0       ; buffer SIZE
        move    x0,x:Fbitbuf_size
        move    y:g_pMp3DataSrcBufHead,x0       ; HEAD
        move    x0,x:Fbitbuf_writeptr
        move    y:g_pMp3DataSrcBufTail,x0       ; TAIL
        move    x0,x:Fbitbuf_readptr

        ; clear the bit buffer and pcm buffer for mp3 decode routines
        clr     a
        move    y:<Const_ffffff,m3
        move    #>g_Mp3DataSrc_mbuf,r3
        do      #g_Mp3DataSrcBufSize/2,_clear_bitbuffer
        move    a,x:(r3)+
        move    a,x:(r3)+
_clear_bitbuffer
        nop
        rts

Mp3InitSink
        ; init buffer head / tail to beginning
        move    #g_Mp3DecSinkBufDescriptor,r0      ; SINK buffer descriptor ptr
        move    #>$ffff,m0
        jsr     InitBufferEmpty

        ; init sink buffer to half full (with zeros)
        move    #g_Mp3DecSinkBufDescriptor,r0      ; SINK buffer descriptor ptr
        move    #>$ffff,m0
        clr     a
        move    a,y1                         ; Y1 = 0 = value to init to
        move    y:g_pMp3DecSinkBufSize,a
        move    #>(1152+10),x0
        sub     x0,a
        move    a1,n0
        jsr     FillBufferY

        ; setup Sink Buffer Pointers
        move    y:g_Mp3DecSinkBufDescriptor,x0
        move    x0,x:FPCM_addr               ; BASE address
        move    y:g_pMp3DecSinkBufMod,x0
        move    x0,x:FPCM_mod                ; buffer MODULO
        move    y:g_pMp3DecSinkBufSize,x0
        move    x0,x:FPCM_size               ; buffer SIZE
        move    y:g_pMp3DecSinkBufHead,x0
        move    x0,x:FPCM_writeptr           ; HEAD
        move    y:g_pMp3DecSinkBufTail,x0
        move    x0,x:FPCM_readptr            ; TAIL

        ; clear the bit buffer and pcm buffer for mp3 decode routines
        clr     a
        move    y:<Const_ffffff,m3
        move    #>g_Mp3DecSink_mbuf,r3
        do      #g_Mp3DecSinkBufSize/2,_clear_pcmbuffer
        move    a,y:(r3)+
        move    a,y:(r3)+
_clear_pcmbuffer
        nop
        rts        


;*******************************************************************************
; Mp3DecProcess  ::  Main Process Routine for MP3 Decoder (Wrapper)
;*******************************************************************************
Mp3DecProcess

_check_source
        ; Get filled space from source buffer
        move    y:Mp3DecoderSourceBufDesc,r0
        move    y:<Const_ffffff,m0
        jsr     GetFilledSpace                  ; Result in A
        ; check if enough data available for decoder
        move    x:Fbitbuf_data_demand_o,x0
        cmp     x0,a                         ; a must be able to hold x0 samples
        jge     _check_sink
        ; check if data_demand > input buffer size (poorly encoded song--reset if so)
        move    #>g_Mp3DataSrcBufSize,a
        cmp     x0,a
        jle     _exit_mp3
        jmp     _ResetMp3
_check_sink
        ; Get empty space from sink buffer
        move    y:Mp3DecoderSinkBufDesc,r4
        move    y:<Const_ffffff,m4
        jsr     GetEmptySpace
        ; check if enough space available for decoder output
        move    x:FPCM_space_required_o,x0
        cmp     x0,a                         ; a must be able to hold x0 samples
        jlt     _exit_mp3
_source_and_sink_pass

_decode_mp3     
        ; setup Source Buffer Pointers
        move    y:g_Mp3DataSrcBufDescriptor,x0    ; BASE address
        move    x0,x:Fbitbuf_addr
        move    y:g_pMp3DataSrcBufMod,x0        ; buffer MODULO
        move    x0,x:Fbitbuf_mod
        move    y:g_pMp3DataSrcBufSize,x0       ; buffer SIZE
        move    x0,x:Fbitbuf_size
        move    y:g_pMp3DataSrcBufHead,x0       ; HEAD
        move    x0,x:Fbitbuf_writeptr
        move    y:g_pMp3DataSrcBufTail,x0       ; TAIL
        move    x0,x:Fbitbuf_readptr

        ; setup Sink Buffer Pointers
        move    y:g_Mp3DecSinkBufDescriptor,x0
        move    x0,x:FPCM_addr               ; BASE address
        move    y:g_pMp3DecSinkBufMod,x0
        move    x0,x:FPCM_mod                ; buffer MODULO
        move    y:g_pMp3DecSinkBufSize,x0
        move    x0,x:FPCM_size               ; buffer SIZE
        move    y:g_pMp3DecSinkBufHead,x0
        move    x0,x:FPCM_writeptr           ; HEAD
        move    y:g_pMp3DecSinkBufTail,x0
        move    x0,x:FPCM_readptr            ; TAIL

        ; save off stack
        move    #_Mp3SaveSysStackRtn,r6      ; setup the return address
        move    #L1Sp,r5                     ; pointer to the SP local save
        jmp     SaveSystemStack              ; save the hardware stack
_Mp3SaveSysStackRtn

        ; Assertion testpoint here caught: r7 < L1StackSpace. Stmp00002947/Stmp00003027.
	; Allow for a decoder settling interval between message sends to dec.

        ; for VBR-Check
        move    x:Fbitrate_o,a
        push    a

        move    r7,x:Mp3DecoderIsrSaveR7
        move    m7,x:Mp3DecoderIsrSaveM7

        ; call MP3 decoder
        jsr     FMP3_Decode_asm

        move    x:Mp3DecoderIsrSaveR7,r7
        move    x:Mp3DecoderIsrSaveM7,m7

        ; ******************************************************************
        ; SYNC CHECKING SECTION
        ; when Fstatus_o == 1,  set #DecSync in x:DecoderCSR
        ; when Fstatus_o == 0 , sync not yet found -- check for bad file
        ; when Fstatus_o == -1, error encountered -- reset and look for sync
        ; ******************************************************************
        move    x:Fstatus_o,a           ; MP3 decoder sets this when sync is found
        tst     a
        jgt     _Sync                   ; Equals '1' if sync'd
        pop     a                       ; balance the stack for VBR-CHECK. CCR N,Z bits unchanged

        jeq     _NoSyncYet_CheckFile    ; if 0: never synch'd. Check for bad file.

        ; Negative Fstatus_o: sync was found but IS lost. We'll _ResetMp3 but first: 
        ; If just now lost sync, set sync thresh base value to current filepos.
        move    y:g_BaseWasResetOnSyncLoss,a
        tst     a
        jne     _NoSyncYet_CheckFile         ; if flag set, skip the base change.
        ; Sync was just lost due to error.
        jsr     MpegDecSet_SyncThresholdBase ; Mark sync loss file position.
        bset    #0,y:g_BaseWasResetOnSyncLoss
        jmp     _ResetMp3               

_NoSyncYet_CheckFile
        ; Decoder status is 0 indicating that decoder has not sync'd yet. 

        ; Allow us to quickly skip a file without MPEG headers.
        ; THIS CODE doesn't check for extremely large ID3V2 headers- should be added later.
        ; IF g_FileIsMp3 is zero, we need to do more checks

        ; FIRST: Check if the file ever had sync'd. 
        ; If so, say it's a good file & no further checking.
        move    y:g_FileIsMp3,a         ; Set IF sync found (stays set for song duration)
        tst     a                       ; if ZERO- we never found mp3 sync for song
        jgt     _SyncNotFound           ; Always jumps if we ever had sync for the file.

        ; SECOND: If (we've been trying for MP3_SYNC_THRESHOLD1 bytes) (Def in 2 project.inc files).
        ;         This check is to lower the frequency of the decoder isr if we're not syncing -
        ;         otherwise buttons and other messages get blocked by an overfrequent decoder isr.
        move    y:SongByteTotalHigh,B
        move    y:SongByteTotalLow,b0
        move    y:g_wFilePos_StartTryingToSyncHigh,A
        move    y:g_wFilePos_StartTryingToSyncLow,a0
        sub     a,b
        ; B1:B0=(fread bytepos - file byte start which can be reset during song) 
        move    #MP3_SYNC_THRESHOLD1,x0 ; value defined in project.inc
        cmp     x0,b
        jlt     _SyncNotFound
        ; Decoder has not synchronized within threshold 1

        ; THIRD: If ((we have been trying for MP3_SYNC_THRESHOLD bytes) && 
        ;            (still no sync found)) give up and mark song bad  
        ;        Else keep trying to sync
        move    #MP3_DECODERISR_SLOW,x0
        move    x0,y:g_Mp3TimerCount
        move    #MP3_SYNC_THRESHOLD,x0  
        cmp     x0,b
        jlt     _SyncNotFound

        ; Set the current song as bad. Forces skip to next song.
        bset    #0,y:CurrentSongBad
        move    #MP3_DECODERISR_SLOW,x0
        move    x0,y:g_Mp3TimerCount
        jmp     _SyncNotFound

_ResetMp3
        ; If (CRC Error not reported by decoder) continue and reset decoder
        move    x:FerrorLogBasePtr_o,r0
        move    #ERROR_CRC_WAS_WRONG,x0
        move    x:(r0),a
        cmp     x0,a
        jne     _ContinueReset
        ; CRC was wrong

        ; If ((encoding's 'CRC was wrong' was reported by decoder) && 
        ;    (failed to sync for MP3_SYNC_THRESHOLD bytes)) mark song bad
        move    y:SongByteTotalHigh,B
        move    y:SongByteTotalLow,b0
        move    y:g_wFilePos_StartTryingToSyncHigh,A
        move    y:g_wFilePos_StartTryingToSyncLow,A0
        sub     a,b
        move    #MP3_SYNC_THRESHOLD,x0  
        cmp     x0,b
        jlt     _ContinueReset ; If (before sync thresh) don't mark song as bad

        ; (CRC is bad in the file) AND (dec can't sync after MP3_SYNC_THRESHOLD bytes)
        bset    #0,y:CurrentSongBad        ; Mark song bad.
        move    #MP3_DECODERISR_SLOW,x0
        move    x0,y:g_Mp3TimerCount
        jmp     _SyncNotFound

_ContinueReset
        ;  Call Parthus MP3 decoder reset since sync was lost.
        jsr     Mp3InitSource           ; Reset Source Buffer and Pointers
        jsr     FMP3_Reset_asm          ; Call Parthus MP3 decoder reset
        move    x:Fbitbuf_readptr,x0    ; Copy updated pointer from Reset routine ...
        move    x0,y:g_pMp3DataSrcBufTail ; ... to our Tail Pointer
	    ; Note: we don't update the Head Ptr because it was init'd to half full
        bclr    #DecSync,x:DecoderCSR        ; no longer sync'd
        jmp     _SyncNotFound

_Sync
        move    #MP3_DECODERISR_FAST,x0
        move    x0,y:g_Mp3TimerCount
        move    x:DecoderCSR,x0
        jset    #DecSync,x0,_CheckIfVBR
        bset    #DecSync,x:DecoderCSR       ; when sync is found, set bit and get samp rate
        bset    #0,y:g_FileIsMp3            ; This stays set for duration of song.
        clr     a
        move    a1,y:g_BaseWasResetOnSyncLoss ; Stays clear until sync lost

        ; Store correct sampling frequency to DacSrr
        move    #Mp3SampFreqTable,r0        ; table of sampling frequency values
        move    #DacSampFreqTable,r4        ; table of values to write to DacSrr
        move    x:Fsampfreq_o,a             ; sampling frequency returned by decoder (found in Mp3 header)
        move    x:(r0)+,x0    y:(r4)+,y0    ; get first value

        do      #9,_EndFsLoop               ; Loop through sampling frequencies to find match
        cmp     x0,a
        jne     _GetNextValue
        enddo
        jmp     _EndFsLoop
_GetNextValue
        move    x:(r0)+,x0    y:(r4)+,y0    ; get next value
_EndFsLoop
        move    y0,x:HW_DACSRR              ; store samp freq to Dac

_CheckIfVBR
        ; VBR-CHECK : check if bitrate has changed from previous
        move    x:Fbitrate_o,x0             ; New bitrate found in current frame
        clr     a
        pop     a                           ; Previous value of Fbitrate_o
        tst     a                           ; Fbitrate_o resets to 0
        jeq     _SyncNotFound
        cmp     x0,a                        ; If current bitrate = previous bitrate
        jeq     _SyncNotFound
        bset    #VBRFlag,x:DecoderSR        ; Set flag if bitrates different

_SyncNotFound

        ; restore stack 
        move    #L1Sp,r5                     ; pointer to the SP local save
        move    #_Mp3RestoreSysStackRtn,r6   ; setup the return address
        jmp     RestoreSystemStack           ; restore the hardware stack
_Mp3RestoreSysStackRtn

        ; copy updated pointers
        move    x:Fbitbuf_readptr,x0
        move    x0,y:g_pMp3DataSrcBufTail    ; update the TAIL ptr
        move    x:FPCM_writeptr,x0
        move    x0,y:g_pMp3DecSinkBufHead    ; update the HEAD ptr
_exit_mp3
        nop
        rts


;*******************************************************************************
; Function: MpegDecSet_SyncThresholdBase
;
; Usage:    if ( mpeg sync attempts start ) call this 
;
; Purpose:  Sets global y:g_wFilePos_StartTryingToSync High:Low to fp at sync start 
;           attempt time so valid encodings won't be marked bad, but invalid ones are.
;
; I/O:      No register inputs or outputs. Preserves registers.
;
MpegDecSet_SyncThresholdBase
    ; Reset StartTryingToSync file pos to the current value of songByteTotal
    push    b1
    push    b0
    move    y:SongByteTotalHigh,b1
    move    y:SongByteTotalLow,b0
    ; Mark byte position in file where we start looking for sync
    move    b1,y:g_wFilePos_StartTryingToSyncHigh 
    move    b0,g_wFilePos_StartTryingToSyncLow
    pop     b0
    pop     b1
    rts

;*******************************************************************************
; Function: MpegDecFree_SyncThresholdBase
;
; Usage:    if ( mpeg sync attempts stop ) call this 
;
; Purpose:  Sets global y:g_wFilePos_StartTryingToSync High:Low to max positive so 
;           good song won't be marked bad
;
; I/O:      No register inputs or outputs. Preserves registers.
;

MpegDecFree_SyncThresholdBase 
    push    b1
    push    b0
    move    #$7FFFFF,b1   
    move    #$FFFFFE,b0      
    move    b1,y:g_wFilePos_StartTryingToSyncHigh
    move    b0,y:g_wFilePos_StartTryingToSyncLow
    pop     b0
    pop     b1
    rts	



        endsec

