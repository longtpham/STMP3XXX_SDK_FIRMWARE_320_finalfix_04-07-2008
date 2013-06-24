;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000
;
; STMP ADPCM DECODER EQUATE FILE
;///////////////////////////////////////////////////////////////////////////////

    IF (!@def(dec_smvadpcmequ1))
dec_smvadpcmequ1   equ     1

;///////////////////////////////////////////////////////////////////////////////
;  IMPORTANT NOTE ON SmvAdpcmDecSinkBuf:
;
;   The ADPCM Sink Buffer is a Modulo buffer.  Its current size is 6656 (=$1A00) words.
;   This must be aligned on an address that is a multiple of (currently) $2000.
;   An address which works is Y:$2000.
;
;   The Tasking Linker/Locator is getting confused.  If the buffer size is > 4096, the 
;   link fails to fit.  It appears that the locator is trying to move the buffer to 
;   y:$4000, but it's hard to tell.
;
;   The y:$2000 address should work, so we fake out the linker/locator by declaring a
;   modulu buffer of 4096 (=$1000) words, using "g_SmvAdpcmDecSinkBufSize_1", immediately 
;   followed by another buffer of 2560 (=$A000) words (using "g_SmvAdpcmDecSinkBufSize_2"), 
;   to make a total of 6656.  The linker is happy with this, and so is the application.
;
;   It's not the way it's supposed to be done, but the tool isn't working right.
;
;   See Also: dec_smvadpcm_mem.asm
;
;///////////////////////////////////////////////////////////////////////////////

g_SmvAdpcmDecSinkBufBlockSize equ  512;$1D0;$AE;192;512                     ; block size to process decoder audio
g_SmvAdpcmDecSinkBufSize      equ  $880;$838;$828;$828;$900;6656                ; The size of the decoder sink buffer
g_SmvAdpcmDecSinkBufSize_1      equ  $880;$838;$828;$828;$900;`4096                ; The 1st 4096 words of the SinkBufSize
;g_SmvAdpcmDecSinkBufSize_2      equ  (g_SmvAdpcmDecSinkBufSize-g_SmvAdpcmDecSinkBufSize_1) ; The rest of the SinkBufSize
g_SmvAdpcmDecSinkBufModulo    equ  g_SmvAdpcmDecSinkBufSize-1 ; The modulo of the decoder sink buffer


g_SmvAdpcmDataSrcBufBlockSize equ  $40
g_SmvAdpcmDataSrcBufSize      equ  `512
g_SmvAdpcmDataSrcBufModulo    equ  g_SmvAdpcmDataSrcBufSize-1

BLOCK_SIZE             equ    `512

WAVE_FORMAT_DVI_ADPCM  equ    $0011 ; IMA ADPCM
WAVE_FORMAT_PCM        equ    $1    ; MS PCM
WAVE_FORMAT_ADPCM      equ    $2    ; MS ADPCM
WAVE_FORMAT_IMA_ADPCM		equ	 	$11 ;IMA  ADPCM

PAUSE_CMD              equ    $0
PLAY_CMD               equ    $1
STOP_CMD               equ    $2
REW_CMD                equ    $3
FFD_CMD                equ    $4
REC_CMD                equ    $5

STOPPED_STATE          equ    $0
PLAY_STATE             equ    $1
RECORD_STATE           equ    $2

RECORD_MODE            equ    $0
PLAYER_MODE            equ    $1

; Return Codes        
RETURN_OK              equ    $0
REW_SAMPLES            equ    $1
FWD_SAMPLES            equ    $2
ERROR_READING_WAVE_HEADER  equ    $3

; Error Values  
NO_ERRORS              equ    $0
RIFF_UNPACK_ERROR      equ    $1
WAVE_UNPACK_ERROR      equ    $2
FMT_UNPACK_ERROR       equ    $3
UNKNOWN_CODEC_ERROR    equ    $4
FACT_UNPACK_ERROR      equ    $5
DATA_UNPACK_ERROR      equ    $6





ID3_TAG                      equ     $544147
ID3_TAG_DYSLEXIC             equ     $474154

DECODER_TIMER_SYNC_COUNT     equ     96000
DECODER_TIMER_NOSYNC_COUNT   equ     10000
; These values obtained by trial-and-error with pure tones
DecoderTimerCount32kbps      equ     $40000
DecoderTimerCount40kbps      equ     $40000
DecoderTimerCount48kbps      equ     $35000
DecoderTimerCount56kbps      equ     $33000
DecoderTimerCount64kbps      equ     $25000
DecoderTimerCount80kbps      equ     $23000
DecoderTimerCount96kbps      equ     $20000
DecoderTimerCount112kbps     equ     $18000
DecoderTimerCount128kbps     equ     $17000
DecoderTimerCount160kbps     equ     $16000
DecoderTimerCount192kbps     equ     $14000
DecoderTimerCount224kbps     equ     $13000
DecoderTimerCount256kbps     equ     $10000
DecoderTimerCount320kbps     equ     $09500

Sec2Min                      equ     $022222 ; 1/60
Min2Sec                      equ     60

DECODER_VOICE_BYTE_THRESHOLD equ     $B800  ; about 5 seconds
DECODER_SONG_BYTE_THRESHOLD  equ     80000  ; use to determine if PrevSong means previous or reset current
DECODER_SONG_SEC_THRESHOLD   equ     5      ; if > 5 seconds elapsed, reset to current song on a PrevSong push


ASL16                        equ     65536
ASL15                        equ     32768
ASL14                        equ     16384

READMODE                     equ     $000001 ; replace later when filesystem equate file is done

SEC                          equ     3       ; number of seconds to skip for FFWD/RWND push
FFWD_RWND_BYTE_SKIP          equ     (@CVI(3*(@FLR((SEC*128000/8)/3))))  ; num bytes to jump over for ffwd/rwnd button push
                                             ; 1600 = (128kbps)*(1/10s)*(1byte/8bits)+2 
                                             ; **** must be divisible by 3 to go from bytes to words


    ENDIF  ; IF (!@def(dec_smvadpcmequ1))

