;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000/2001
;
; Decoder Module       
;///////////////////////////////////////////////////////////////////////////////

    IF (!@def(DECODEREQU_INC))
DECODEREQU_INC equ     1

ID3_TAG                      equ     $544147
ID3_TAG_DYSLEXIC             equ     $474154

DECODER_TIMER_SYNC_COUNT     equ     7500    ; for power savings
DecoderTimerCountWMADec	     equ     $01d4c  ; 0.25 ms timer at 60 Mhz clock.		
;DECODER_TIMER_SYNC_COUNT     equ     96000
DECODER_TIMER_NOSYNC_COUNT   equ     3500
; These values obtained by trial-and-error with pure tones
;DecoderTimerCount32kbps      equ     $40000
;DecoderTimerCount40kbps      equ     $40000
;DecoderTimerCount48kbps      equ     $35000
;DecoderTimerCount56kbps      equ     $33000
;DecoderTimerCount64kbps      equ     $25000
;DecoderTimerCount80kbps      equ     $23000
;DecoderTimerCount96kbps      equ     $20000
;DecoderTimerCount112kbps     equ     $18000
;DecoderTimerCount128kbps     equ     $17000
;DecoderTimerCount160kbps     equ     $16000
;DecoderTimerCount192kbps     equ     $14000
;DecoderTimerCount224kbps     equ     $13000
;DecoderTimerCount256kbps     equ     $10000
;DecoderTimerCount320kbps     equ     $09500

Sec2Min                      equ     $022222 ; 1/60
Min2Sec                      equ     60

DECODER_VOICE_BYTE_THRESHOLD equ     $B800  ; about 5 seconds
DECODER_SONG_BYTE_THRESHOLD  equ     80000  ; use to determine if PrevSong means previous or reset current
DECODER_SONG_SEC_THRESHOLD   equ     5      ; if > 5 seconds elapsed, reset to current song on a PrevSong push

; Default MP3 Sync Values -- customizable via project.inc
    if (!@def(MP3_SYNC_THRESHOLD))
MP3_SYNC_THRESHOLD              equ     70000 ; # bytes to look for sync before marking it bad
    endif

    if (!@def(MP3_SYNC_THRESHOLD1))
MP3_SYNC_THRESHOLD1             equ     1000  ; # bytes to look for sync before slowing decoder isr frequency (to let menus run)
    endif

    if (!@def(MP3_DECODERISR_FAST))
MP3_DECODERISR_FAST             equ     7500  ; once we have sync'd, the isr should be called this frequently
    endif 

    if (!@def(MP3_DECODERISR_SLOW))
MP3_DECODERISR_SLOW             equ     50000 ; if decoder is having difficulty syncing, switch isr to be called less frequently
    endif

    if (!@def(MP3_DECODERISR_SLOW))
MP3_DECODERISR_SLOW             equ     50000 ; if decoder is having difficulty syncing, switch isr to be called less frequently
    endif

    if (!@def(SECONDS_TO_SKIP))
SECONDS_TO_SKIP                 equ     1     ; ff/rw skip
    endif

; CRC errror code $304080 is used by Parthus lib source code Oct 21,02. 
; Note that CRC error in Parthus "MP3 Code Release Document" (11/07/00) 
; is $301080 which is not used by lib or here.
ERROR_CRC_WAS_WRONG equ $304080


ASL16                        equ     65536
ASL15                        equ     32768
ASL14                        equ     16384

READMODE                     equ     $000001 ; replace later when filesystem equate file is done

FFWD_RWND_BYTE_SKIP          equ     (@CVI(3*(@FLR((SECONDS_TO_SKIP*128000/8)/3))))  ; num bytes to jump over for ffwd/rwnd button push
                                             ; 1600 = (128kbps)*(1/10s)*(1byte/8bits)+2 
                                             ; **** must be divisible by 3 to go from bytes to words
    ENDIF ;@DEF(DECODEREQU_INC)