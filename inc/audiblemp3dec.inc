;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000
;
; STMP Audible MP3 Wrapper
;///////////////////////////////////////////////////////////////////////////////

    IF (!@def(audiblemp3decxref))
audiblemp3decxref   equ     1

g_AudibleMp3DecSinkBufBlockSize equ  $480             ; block size to process MP3 audio
g_AudibleMp3DecSinkBufSize      equ  $900             ; The size of the MP3 decode buffer
g_AudibleMp3DecSinkBufModulo    equ  g_AudibleMp3DecSinkBufSize-1 ; The modulo of the MP3 decode buffer

g_AudibleMp3DataSrcBufBlockSize equ  $40
;g_AudibleMp3DataSrcBufSize      equ  $1E5
    if (@def('DEVICE_3400')||@def('DEVICE_3300'))
g_AudibleMp3DataSrcBufSize      equ  530           ; Need at least 4 frames if running at 30Mhz
    else
g_AudibleMp3DataSrcBufSize      equ  2500          ; Need at least 4 frames if running at 30Mhz
    endif
    
g_AudibleMp3DataSrcBufModulo    equ  g_AudibleMp3DataSrcBufSize-1

AUDIBLE_MP3_SAFE_NUMBER             equ   $265         

    ENDIF  ; IF (!@def(audiblemp3decxref))

