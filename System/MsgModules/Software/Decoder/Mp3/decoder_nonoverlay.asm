;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2003
;
; STMP3410 ISR-level Decoder Module (CANNOT be overlayed)
;
; Filename: decoder_nonoverlay.asm (MPEG - Used with layers 1,2,3)
;
;///////////////////////////////////////////////////////////////////////////////
        section DECAPP_DecoderNonOverlay
 

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////
    nolist
    include "project.inc"
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
    include "systimers.xref"
    include "mp3dec.inc"
    include "const.xref"
    include "decodermem.xref"
    include "sysmisc.xref"
    include "mp3mem.xref"
    include "bufdesc.xref"
    include "mixer.xref"
    list
    page    132,60,0,0,0


;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////
       GLOBAL   DecoderModuleTimerDisable
       GLOBAL   Mp3DecoderModuleIsr
       GLOBAL   DecoderFileRead
       GLOBAL   DecoderReadStatus
       GLOBAL   Mp3GetInvBitRate
;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////
       EXTERN   Mp3DecoderFileHandle
       EXTERN   Mp3DecProcess,Fstatus_o
       EXTERN   Mp3DecIsrSp

       EXTERN   FILEClose
       EXTERN   FILEOpen
       EXTERN   FileEOF
       EXTERN   FileRead

       EXTERN   DecGetSongPos
       EXTERN   DecSetSongPosZero
       EXTERN   DecSetSongPosEnd
       EXTERN   DecClearSongTime
       EXTERN   CurrentSongBad
       EXTERN   Fbitbuf_data_demand_o,Fbitrate_o,Falgorithm_o
       extern   Mp3DecFileReadIsrSp
       extern   DecoderReadSaveR6
       extern   Mp3DecSavePC
       extern   Mp3DecSaveSR
       extern   g_Mp3TimerCount
       extern   Fg_wBitRateFound
       extern   SecondstoSkip
	   extern	Fg_wFrameLengthInBytes	; declared in "mp3metadata.c", SL
;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////
DECODER_READ_BUSY_BIT   equ 0           ;0 Decoder File Read function not busy
DECODER_READ_UPDATE_BIT equ 1           ;0 Decoder File Read function return void
DECODER_READ_ERROR_BIT  equ 2           ;0 Decoder File Read function successfull
DECODER_READ_ENABLE_BIT equ 3           ;0 Decoder File Read function not to be call

SAFETY_NET_WORDS        equ 200         ; Need for MP3 files ripped with the CRC bit set
                                        ; It looks like even if Parthus MP3 decoder
                                        ; reports the the Mp3DecoderSourceBuf being
                                        ; free we need this safety net of words.
                                        ; To be persued by Parthus ....
MAX_WORDS_TO_READ       equ 800         ; Number max of words to be read at once  
EOF_SAFETY_OFFSET_BYTES equ 551         ; MAX_MP3_FRAME_BYTES+1                 DECODE2EOF
; Above equates could be moved to mp3dec.inc
;///////////////////////////////////////////////////////////////////////////////
;   X Memory
;///////////////////////////////////////////////////////////////////////////////
        org     x,"DECAPP_DecoderNonOverlay_X":
DecoderReadStatus       dc  0
DecoderFileReadNBytes   dc  0

;///////////////////////////////////////////////////////////////////////////////
;   Y Memory
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;   Code
;///////////////////////////////////////////////////////////////////////////////
        org     p,"DECAPP_DecoderNonOverlay_P":

;*******************************************************************************
; DecoderModuleTimerDisable Routine
;*******************************************************************************
DecoderModuleTimerDisable
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
; DecoderFileRead Routine
;*******************************************************************************
DecoderFileRead
        ; CANNOT be overlayed, called by ISR.
        ;Here the Interruption level is dropped automatically to 0 (user level)
; debug  ;//dbgmpg
        ; save off all registers

        ; Clear the DM bit (double-precision multiply mode, bit 6) of the Mode Register
        ; This will be returned to its previous state when the RTI occurs (since the SR
        ; will be popped back off System Stack Low (SSL).
        ; Accumulator behavior is unpredictable when the DM bit is set.
        ; Next cycle after clearing it should NOT be an ALU instruction.
        andi #$BF,mr

        move    r6,y:DecoderReadSaveR6          ; r6 used for return mechanism
        move    #_DecoderReadSaveRegReturn,r6   ; setup the return address
        jmp     SaveRegs                        ; save the remaining registers
_DecoderReadSaveRegReturn
        
        ; save off stack
        move    #_DecoderReadSaveStackReturn,r6 ; setup the return address
        move    #Mp3DecFileReadIsrSp,r5         ; pointer to the SP local save
        jmp     SaveSystemStack                 ; save the hardware stack
_DecoderReadSaveStackReturn
        jsr     SysAllLinear
        move    x:Fbitbuf_data_demand_o,a
        tst     a
        jgt     _checkForSpace
        jeq     _return
        error

_return
        move    x:DecoderReadStatus,x0
        bclr    #DECODER_READ_BUSY_BIT,x0
        bset    #DECODER_READ_UPDATE_BIT,x0
        bclr    #DECODER_READ_ERROR_BIT,x0

_RestoreSystem

        ;Raise Int level to 1 so we can update the BUSY flag without being
        ;interrupted by the Decoder Isr

        bset    #8,sr                       ; Raise interrupt level to 1
        nop                                 ; Those nops make sure that 
        nop                                 ; any interrupt pending is processed
        nop                                 ; DO NOT REMOVED, UNLESS YOU HAVE 
                                            ; A FREE DAY TO DEBUG CODE

        move    a1,x:DecoderFileReadNBytes
        move    x0,x:DecoderReadStatus

        ; restore stack
        move    #Mp3DecFileReadIsrSp,r5         ; pointer to the SP local save
        move    #_DecoderReadStackRestored,r6   ; setup the return address
        jmp     RestoreSystemStack              ; restore the hardware stack
_DecoderReadStackRestored

        move    #_DecoderReadRestoreRegReturn,r6 ; setup the return address
        jmp     RestoreRegs                     ; restore registers
_DecoderReadRestoreRegReturn
        move    y:DecoderReadSaveR6,r6          ; restore minimal reg set

        movec   y:Mp3DecSavePC,ssh              ;Force return 
        movec   y:Mp3DecSaveSR,ssl
        nop
        rti                            ;This needs to be a RTI to restore the SR

_checkForSpace

        ; Determine how much data needed
        move    y:Mp3DecoderSourceBufDesc,r4
        move    y:<Const_ffffff,m4
        jsr     GetEmptySpace               ; returns: a==number of words in buffer
                      ; The checks are valid only during syncing process.                          
                      move    x:Fstatus_o,B
                      move    y:<Const_000001,x0
                      cmp     x0,B          
                      jne     _ContinueNoSync
                      move    y:<Const_000003,x0
                      move    x:Falgorithm_o,B
                      cmp     x0,B
                      jne     _checkForFilledSpace
_ContinueNoSync                   
                      move    x:Fbitbuf_data_demand_o,x0
	                  move    #>SAFE_NUMBER,B
                      cmp     x0,B
                      ; Under some rare circumstances the data demand exceeds SAFE region. This happens
                      ; when the file is corrupt. Allow the overwrite to take place in such a case to allow
                      ; resync and playback. This will skip some frames, but allow syncing.
                      ; In such a case fill up the buffer equal to data demand.
                      jgt     _WithinBounds

                      ; This section is rarely hit.
                      ; AmtToRead = bitbuf_data_demand_o - FilledSpace;
                      ; AmtToRead = bitbuf_data_demand_o - Bitbufsize + Emptyspace
                      add     x0,A      #>g_Mp3DataSrcBufSize,x0
                      sub     x0,A      y:<Const_000001,x0
                      ; Free space reported will be one less. Correct for this.
                      add     x0,A                      
                      jmp     _checkForFilledSpace
                      
_WithinBounds
    	              move #>SAFE_NUMBER,x0    
                      sub     x0,A           x:Fbitbuf_data_demand_o,x0
                      add     x0,A
_checkForFilledSpace
                      tst     A
                      jgt     _checkEof
                      clr     a                            ; enough data in memory already
                      jmp     _return


                ;Simulate old code
                    ; Determine how much data needed
;                    move    y:Mp3DecoderSourceBufDesc,r0
;                    move    y:<Const_ffffff,m0
;                    jsr     GetFilledSpace               ; returns: a==number of words in buffer
;                    move    a,x0
;                    move    x:Fbitbuf_data_demand_o,a
;                    sub     x0,a
;                    tst     a
;                    jgt     _checkEof
;                    clr     a                            ; enough data in memory already
;                    jmp     _return
                ;Simulate old code                    




_checkEof
        ; See if EOF was already reached on a previous read.
        move    x:DecoderCSR2,x0            
        jclr    #EndOfFileReached,x0,_ProceedToCheckAvailable  ; See if fread needed. DECODE2EOF 
        ; EOF already reached so return now without reading file

        clr     a
        jmp     _return

_ProceedToSafetyNet
        ; Add safety net
;        move    #>SAFETY_NET_WORDS,x0
;        sub     x0,a
;        tst     a
;        jgt     _ProceedToCheckLimitRead

;        clr     a
;        jmp     _return
        
;_ProceedToCheckLimitRead
;        ;This checks if the words to read are greater than a constant
;        ;if so, it limits the number of words read at one time to this constant
;        ; Why? At least the 1sttrack of the playlist will have a few seconds 
;        ; missing when played if read too many words.
;        move    #>MAX_WORDS_TO_READ,x0
;        cmp     x0,a
;        jlt     _ProceedToCheckAvailable
;        
;        ; Limit the read to x0 words
;        move    x0,a1
                ;Simulate old code                     
_ProceedToCheckAvailable 

        push    a0                          ; save off bytes needed
        push    a1
        push    a2
        clr     a
        move    y:Mp3DecoderFileHandle,a2
        jsr     FileEOF                     ; returns bytes left in file in A1
        move    a1,x0                       ; X0 = bytes left in file
        pop     a2                          ; restore bytes needed in A
        pop     a1
        pop     a0
        cmp     x0,a                        ; If bytes needed < bytes left then 
                                            ; Read more data
        jle     _readMoreData

        clr     a                           ; if bytes needed greater than bytes 
                                            ; left => it is end of song
        move    x:Fbitbuf_data_demand_o,a1
        cmp     x0,a
        jgt     _EndOfSong
        
        move    x0,a1                       ; Read bytes left in file
        jmp     _readMoreData        

_EndOfSong
        clr  a
        ; Must set EndOfFileReached instead of EndOfSong as in previous versions. STMP1851.
        move    x:DecoderCSR2,x0
        bset    #EndOfFileReached,x0        
        move    x0,x:DecoderCSR2
        ; File read attempts will stop, decode continues. DECODE2EOF    
        ; testpoint      
        jmp     _return



                ;Simulate old code
;                    push    a0                           ; save off bytes needed
;                    push    a1
;                    push    a2
;                    clr     a
;                    move    y:Mp3DecoderFileHandle,a2
;                    jsr     FileEOF                      ; returns bytes left in file in A1
;                    move    a1,x0                        ; X0 = bytes left in file
;                    pop     a2                              ; restore bytes needed in A
;                    pop     a1
;                    pop     a0
;                    cmp     x0,a                            ; if bytes needed > bytes left then EndOfSong -> Exit
;                    jle     _readMoreData

;                    clr     a
;                    bset    #EndOfSong,x:DecoderCSR
;                    jmp     _return
                ;Simulate old code
            




_readMoreData

        move    y:<Const_000003,x0
        move    a,x1
        mpy     x0,x1,a                     ; get number of bytes (from words)
        asr     a                           ; fix for integer arithmetic
        move    a0,a                        ; A1 = num bytes to read
        move    y:Mp3DecoderFileHandle,a2   ; A2 = File handle
        move    #TARGET_MEM_X,a0            ; A0 = Memory space identifier
        move    y:g_pMp3DataSrcBufHead,r0   ; R0 = Buffer pointer to read into
        push    r0                          ; save for byte swap (MSB/LSB)
        push    x1                          ; x1 = number of words to transfer (save for byte swap)
        jsr     SysAllLinear                ; force all M registers to linear
        move    y:g_pMp3DataSrcBufMod,m0    ; M0 = Buffer modulo
        jsr     FileRead                    ; FileRead A1 num bytes into r0 buf
        jcc     _swapMsbLsb
        pop     x1                          ; balance the stack
        pop     r0                           

        move    x:DecoderReadStatus,x0
        bclr    #DECODER_READ_BUSY_BIT,x0
        bset    #DECODER_READ_UPDATE_BIT,x0
        bset    #DECODER_READ_ERROR_BIT,x0
        jmp     _RestoreSystem
        

_swapMsbLsb
        pop     x1                           ; needed for Do Loop Count
        pop     r0                           ; needed for MSB/LSB swap
        move    y:g_pMp3DataSrcBufMod,m0     ; M0 = Buffer modulo
        push    a                            ; must return with a = num bytes transferred
        move    y:<Const_000080,x0           ; multiply factor needed
        do      x1,_swap_msb_lsb
        move    x:(r0),y0                    ; y0 = B2:B1:B0 (bytes from flash)
        move    #>$ff,x1                     ; byte mask
        mpy     y0,x0,a                      ; a = shifted bytes
        and     x1,a                         ; A1 = MSB in LSB position ($00:00:B2)
        move    a1,y1                        ; Y1 = $00:00:B2
        move    a0,y0                        ; Y0 = $B1:B0:00
        mpy     y0,x0,a                      ; A1 = $00:00:B1 A0 = $B0:00:00
        move    a0,b                         ; b1 = $B0:00:00
        or      y1,b                         ; b1 = $B0:00:B2
        and     x1,a                         ; A1 = $00:00:B1
        move    a1,y1                        ; Y1 = $00:00:B1
        mpy     x0,y1,a                      ; A0 = $00:B1:00
        move    a0,y1                        ; Y1 = $00:B1:00
        or      y1,b                         ; b1 = $B0:B1:B2 :: MSB/LSB are swapped
        move    b1,y0
        move    y0,x:(r0)+                   ; save new value
_swap_msb_lsb
        ; update the head ptr
        move    r0,y:g_pMp3DataSrcBufHead    ; adjust the pointer
        pop     a                            ; must return with a = num bytes transferred
        jmp     _return


;*******************************************************************************
; DecoderByteCounter Routine
;*******************************************************************************
DecoderByteCounter
; Inputs: A1 = number of bytes read from mp3 file in current call
; Outputs: EndOfSong bit is set if done with current song
;          A = 0 if done with current song
        ; CANNOT be overlayed, called by ISR.
        ; 1.  Total Bytes = Current Bytes + Previous SubTotal
        ; A1 has current bytes in it.
        tst     a
        jeq     _proceed_to_eof_reached_check ; Skip the decoded sum if none to add
        ; We decoded some data this pass so:
_add_in_new_bytes
        move    a1,x0                         ; x0 = bytes decoded this pass
        clr     a
        move    a,x1                          ; prepare for double-precision add
        move    y:SongByteTotalHigh,a1        ; total bytes decoded in previous passes
        move    y:SongByteTotalLow,a0
        add     x,a                           ; A = new byte total (48 bits)
        move    a1,y:SongByteTotalHigh
        move    a0,y:SongByteTotalLow
        ; total bytes in A1:A0

       ; Warning: DISPLAY TIME should not be and is no longer based on 
       ; SongByteTotalLow directly. 
       ; The time calc routine requires a running total of bytes actually consumed
       ; by the decoder rather than total bytes read from file into Mp3DataSrcBuf. 
       ; Using Mp3 consumed bytes instead accounts for queuing and time delay 
       ; that SongByteTotalLow does not. Changing to running tot of bytes 
       ; consumed by decoder, the current time displayed is the time for 
       ; a byte that was just decoded rather than time for byte that we just 
       ; sent to the Mp3 data src buf which is prefilled and has not been decoded 
       ; yet. Resolution keeps a bytes consumed count in the ISR.  STMP00001793

_proceed_to_eof_reached_check

        ; DECODE2EOF: Note using SongBytesTotalLow here we were setting  
        ; EndOfSong bit prematurely (when file read finished). Decoding of final 
        ; src buf sz words remained undone (<=7500 bytes). Now we have two 
        ; different checks to determine when to stop the decoder at a later time. 
        ; Allows decode of additional frames at end of encoding. Note: One status 
        ; bit used to stop file reads at EOF and another bit to allow decoder to 
        ; finish consumption of remaining MPEG data after file reads complete.  
        ; STMP00001851

        ; 2. if freads didn't find EOF yet, don't consider stopping decoder.
        move    x:DecoderCSR2,x0
        jclr    #EndOfFileReached,x0,_continue_decoding_song

_done_with_song_fread
        ; 3. File reads done and we have at most: src buf sz encoded bytes remaining. 
        ; Don't stop decode by setting EndOfSong bit until one of the 2 checks say.
        ;   if consumed bytes > (fileSz - safetyEofOffset) then stop decoder.
        
        ; _do_consumption_check     Prevents consumption beyond encoding. 
        ; Load B1:B0 with the song length in bytes
        move    y:SongByteLengthHigh,b         ; high bytes of song length
        move    y:SongByteLengthLow,b0          ; low bytes of song length
        clr     a
;		move    #>EOF_SAFETY_OFFSET_BYTES,a0
        move    x:Fg_wFrameLengthInBytes,a0		; Updated in GetMp3FrameInfo(), SL
        sub     a,b                             ; b1:b0=(fileSz - safety offset)
        clr     a
        move    y:SongByteTotalConsumedHigh,a1  ; total bytes decoded in A1:A0
        move    y:SongByteTotalConsumedLow,a0        
        ; added begin, SL
        ; the consumed byte (file size) includes tag size, minus it for a more precious calculation
        ; i.e. consumed byte = consumed byte - tag size
        push    r0
        move    #>FMp3DecoderStartFrameFilePosition,r0
        move    y:(r0)+,x0
        move    y:(r0)+,x1
        sub     x,a
        pop		r0
        ; added end, SL
        ; cmp Decoded byte count to (filesz - safety offset)    All in bytes
        cmp     a,b                             
;   jgt     _continue_decoding_song ; debug line only. skips required src_buf_check below.
        jgt     _do_src_buf_check   ; jump & continue if decode limit in b > consumed

        bset    #EndOfSong,x:DecoderCSR 
        ; testpoint
        clr     a
        ; After EOF, Less than Safety offset bytes left to decode. Flag decode stop.
        jmp     _continue_decoding_song

_do_src_buf_check    
        ; 4. Redundant secondary decoder stop check inspects encoded src buffer vs 
        ; demand in the rare case that the consumption check above is in error.
        jsr     GetMpegSrcBufWords        ; Returns # src buf words in A1
        clr     b
        move    Fbitbuf_data_demand_o,b        
        cmp     b,a
        jgt     _continue_decoding_song   ; if src words in a > demand in b, continue decode
        ; jgt (>) required above to stop decoder. (not >=)

        bset    #EndOfSong,x:DecoderCSR    
        ; testpoint
        clr     a
        ; After EOF, last src buf data can not meet demand. Flag decoder stop.

_continue_decoding_song
        rts

;*******************************************************************************
; SubRoutine: GetMpegSrcBufWords Internal Routine
; Purpose:    Calls GetEmptySpace and subtracts emptyspace size from src buf size
; Inputs:     Global src buffer size variable. Calls GetEmptySpace uses globals.
; Output:     Returns current number of source buffer bytes in A1.    DECODE2EOF
;*******************************************************************************
GetMpegSrcBufWords
        clr     a
        jsr     GetEmptySpace                   ; empty word count in A1
        clr     b        
        move    #>g_Mp3DataSrcBufSize,b
        sub     a,b                             ; src buf words = sz - free
        clr     a
        move    b1,a                            ; to a1 & sign extends

        ; Returns in A1 the number of words currently in the encoded src buffer
        rts

       
;*******************************************************************************
; Mp3DecoderModuleIsr
;*******************************************************************************
Mp3DecoderModuleIsr
; debug ;//dbgmpg
       ;trace   #>$AA0000 ;Enable is TRACEBUF_EN else trace macro stubbed. systrace.asm


        ; Clear the DM bit (double-precision multiply mode, bit 6) of the Mode Register
        ; This will be returned to its previous state when the RTI occurs (since the SR
        ; will be popped back off System Stack Low (SSL).
        ; Accumulator behavior is unpredictable when the DM bit is set.
        ; Next cycle after clearing it should NOT be an ALU instruction.
        andi #$BF,mr
    
        move    x0,y:IntL1_SaveR6           ; save x0 state
        
        ; Check if spurious interrupt. If so, exit now!
        move    x:HW_ICLSTATUS0R,x0
        jset    #HW_ICLSTATUS0R_SST13_BITPOS,x0,_ContinueIsr
        ; None of the above, exit interruption
        move    y:IntL1_SaveR6,x0           ; restore x0 state
        rti

_ContinueIsr
        move    y:IntL1_SaveR6,x0           ; restore x0 state

        ; save off all registers
        move    r6,y:IntL1_SaveR6            ; r6 used for return mechanism
        move    #_DecoderSaveRegReturn,r6    ; setup the return address
        jmp     SaveRegs                     ; save the remaining registers
_DecoderSaveRegReturn

        ; save off stack
        move    #_DecoderSaveStackReturn,r6  ; setup the return address
        move    #Mp3DecIsrSp,r5                 ; pointer to the SP local save
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

        ; if (DecSync == 1) Enable the Digital to Analog Converter
        jclr    #DecSync,x0,_checkFileOK
        move    x:HW_DACCSR,x0
        jset    #HW_DACCSR_TXEN_BITPOS,x0,_checkFileOK
        bset    #HW_DACCSR_TXEN_BITPOS,x:HW_DACCSR

_checkFileOK
        move    y:CurrentSongBad,x0
        jclr    #0,x0,_checkForData
        bset    #EndOfSong,x:DecoderCSR     ; force to end of bad file
        ; testpoint

        bset    #EndOfFileReached,x:DecoderCSR2                     ; DECODE2EOF
        jmp     _IsrStopSequence

_checkForData
        move    y:Mp3DecoderSourceBufDesc,r4 ; TIME_FIX
        move    y:<Const_ffffff,m4
        jsr     GetEmptySpace
        push    a1                          ; save pre-decode words free result
        ; Determine if more data needed for decoder
        move    y:Mp3DecoderSourceBufDesc,r0
        move    y:<Const_ffffff,m0
        jsr     GetFilledSpace               ; returns: a==number of words in buffer
        move    x:Fbitbuf_data_demand_o,x0
        cmp     x0,a

        ; if supply < demand,  Skip the decoder call.
        jlt     _getMoreBytes 
_callDecoder
        jsr     Mp3DecProcess
        move    x:DecoderCSR,x0
        jset    #BadFile,x0,_IsrStopSequence

_getMoreBytes
        ;--------- Tally the song bytes consumed by decoder, summing free space decrease
        move    y:Mp3DecoderSourceBufDesc,r4 ; TIME_FIX
        move    y:<Const_ffffff,m4
        jsr     GetEmptySpace             
        push    a1   
        clr     a
        pop     a0                        ; Move post-decode free words result to A0 (final)

        clr     b       
        pop     b0                        ; pre-decode free words result in B0 (initial)
        ; Compare the pre and post decode free word counts.  
        cmp     b,a                       ; if( (finalFree - initialFree) > 0 ) 
                                          ;   Free increasing. Decoder consumed.
        jle     _none_consumed         
        ; Free space increased so tally consumed bytes
        sub     b,a                       ; Gives 'words free increase' in A
        move    y:<Const_000003,y0        ; Convert word count in a0 to bytes
        move    a0,x0
        mpy     y0,x0,a
        asr     a        
        ; We tally consumed bytes when buf free bytes decreases across an Mp3DecProcess.
        ; If more empty space now, then the decoder consumed bytes so tally them.
        move    y:SongByteTotalConsumedHigh,x1
        move    y:SongByteTotalConsumedLow,x0
        add     x,a
        move    a1,y:SongByteTotalConsumedHigh
        move    a0,y:SongByteTotalConsumedLow
_none_consumed  ; End Tally. Next, read more bytes into MP3 src buf. -----

        ; Determine if file read is already in progress
        move    x:DecoderReadStatus,x0
        jset    #DECODER_READ_BUSY_BIT,x0,_restore_stack
                
_ForceInt
        bset    #DECODER_READ_ENABLE_BIT,x0     ;Enables the file read decoder
        bset    #DECODER_READ_BUSY_BIT,x0       ;Sets the busy bit
        move    x0,x:DecoderReadStatus

        ; Determine if decoder file read has new information
        jclr    #DECODER_READ_UPDATE_BIT,x0,_restore_stack
        
        ; Process new information from decoder file read
        bclr    #DECODER_READ_UPDATE_BIT,x0
        move    x0,x:DecoderReadStatus
        
        clr     a
        move    x:DecoderFileReadNBytes,a1      ; TIME_FIX note _incrementByteCount
        
        jclr    #DECODER_READ_ERROR_BIT,x0,_incrementByteCount

        bclr    #DECODER_READ_ERROR_BIT,x0
        move    x0,x:DecoderReadStatus

        bset    #FileReadError,x:DecoderCSR ; failed to read data
        jmp     _IsrStopSequence

_incrementByteCount
        jsr     DecoderByteCounter           ; Expects number of bytes decoded in A1.
                                             ; EndOfSong bit is set if done with song
_checkIfTrackDone
        move    x:DecoderCSR,x0
        jclr    #EndOfSong,x0,_restore_stack   ; continue decoding song if bit not yet set
        ; Note _restore_stack conditional jump above. DECODE2EOF

        ; Stop Decoder since EndOfSong is indicated

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

        ; Check if DAC is not even on (never sync'd -- possible bad file)

        ; if (#EndOfSong && #SyncWait && !#DecSync)   #BadFile = TRUE;

        move    x:DecoderCSR,x0              ; copy contents of status register
        jclr    #EndOfSong,x0,_ok ; ok if EndOfSong not flagged yet
        jclr    #SyncWait,x0,_ok  ; ok if Sync was previously found (ReSyncWaiting)
        jset    #DecSync,x0,_ok   ; ok if DecSync is ON now
        bset    #BadFile,x:DecoderCSR
_ok

        ; Since done decoding per pause sequence, disable timer.
        ; User level will turn DAC off before advancing to next song
        bclr    #Play,x:DecoderCSR           ; clear to avoid "Pause" on next song
        jsr     DecoderModuleTimerDisable    ; turn off timer

_restore_stack

        ;///////////////// T E M P O R A R Y ////////////////////////////
        ; ***** THIS IS DONE IN CASE A BAD FILE IS ENCOUNTERED --
        ;  the Mp3 Decoder will stream through it faster, and get on
        ;  to the next song (hopefully a good one).
        ; Setup Timer0 Module for use by decoder
        jsr     Mp3GetInvBitRate             ; used for timer count and time. must be called each time in case VBR.
        move    x:DecoderCSR,y0              ; copy contents of status register
        move    #>DecoderTimerNum,x0         ; input to timer routine
        move    y:g_Mp3TimerCount,x1         ; mp3dec.asm fills this in
_progtimer
        move    #_TimerCountLoaded,r0
        jmp     SysLoadTimerCount
_TimerCountLoaded

        ; reset the isr for next time
        move    #>DecoderTimerNum,x0
        move    #_TimerReset,r0
        jmp     SysClearTimerIrq
_TimerReset

        ; restore stack
        move    #Mp3DecIsrSp,r5                 ; pointer to the SP local save
        move    #_stackRestored,r6           ; setup the return address
        jmp     RestoreSystemStack           ; restore the hardware stack
_stackRestored

        ; restore all registers
        move    #DecoderRestoreRegReturn,r6  ; setup the return address
        jmp     RestoreRegs                  ; restore registers
DecoderRestoreRegReturn
        move    y:IntL1_SaveR6,r6            ; restore minimal reg set

;        trace   #>$AAFFFF

        ; Determines if need to start Decoder File Read Function
        btst    #DECODER_READ_ENABLE_BIT,x:DecoderReadStatus
        jcc     _End
        
        ; Clears the enable bit
        bchg    #DECODER_READ_ENABLE_BIT,x:DecoderReadStatus
        
        movec   ssl,y:Mp3DecSaveSR     ;Saves return address and status register
        movec   ssh,y:Mp3DecSavePC     ;Call back address for DecoderFileRead()
        nop
        movec   #DecoderFileRead,ssh   ;Forces DecoderFileRead call after rti
        nop
_End
        rti                            ;Int level is dropped to 0 (user level)
                                       ;DecoderFileRead() operates at user level


;*******************************************************************************
; Name:        Mp3GetInvBitRate
; Type:        SubRoutine
;
; Description: Requires the decoder to be sync'd and x:Fbitrate_o to have the 
;              bitrate in it. Sets up the inverse of the bitrate used to create 
;              the song times, & number of bytes used for FFWD/RWND (based on bitrate).
;
; Inputs:      Global y:Fg_wBitRateFound
;
; Outputs:     May update global y:Mp3DecoderFfwdRwndValue
;              Updates    global y:SongInvBitRate
;              Clears condition code register's carry bit indicating SUCCESS.
;*******************************************************************************
Mp3GetInvBitRate
        ; first, calculate the FFWD/RWND amount
        move    y:Fg_wBitRateFound,x0       ; bitrate found by the parser - kbps
        move    #1000,y0                    ; to get bps
        mpy     x0,y0,a
        asr     a                           ; fix for sign bit
        move    a0,x0                       ; bitrate (bits per second)
        move    y:SecondstoSkip,y0        ; project specific value
        mpy     x0,y0,a                     ; bitrate * 3 seconds
        asr     a                           ; fix for sign bit
        asr     a              ; divide by 8 to get bytes from bits (do 3 ASR's)
        asr     a
        asr     a
        tst     a
        ; if(A==0) Fbitrate_o is 0, so don't update (don't want to be stuck at 0)
        jeq     _SkipSave                   
        move    a0,y:Mp3DecoderFfwdRwndValue
_SkipSave
        ; Computation of SongInvBitrate. 
        ; The following operation ensures *all* rates are taken care of.
        ; Equivalent to previous table based approach.
        move    y:<Const_001000,A               
        and     #$FE,CCR                  ; clear any carry bit if present      
        rep     #$18
        div     x0,A
        add     x0,a 
        move    A0,y:SongInvBitRate
        rts       
    

        endsec
