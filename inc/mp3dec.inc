;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000
;
; STMP MP3 Wrapper
;///////////////////////////////////////////////////////////////////////////////

    IF (!@def(mp3decxref))
mp3decxref   equ     1

g_Mp3DecSinkBufBlockSize equ  $480             ; block size to process MP3 audio
g_Mp3DecSinkBufSize      equ  $900             ; The size of the MP3 decode buffer
g_Mp3DecSinkBufModulo    equ  g_Mp3DecSinkBufSize-1 ; The modulo of the MP3 decode buffer

g_Mp3DataSrcBufBlockSize equ  $40
;g_Mp3DataSrcBufSize      equ  $1E5
    if (@def('DEVICE_3400')||@def('DEVICE_3300'))
g_Mp3DataSrcBufSize      equ  530           ; Need at least 4 frames if running at 30Mhz
    else
g_Mp3DataSrcBufSize      equ  2500          ; Need at least 4 frames if running at 30Mhz
    endif
    
g_Mp3DataSrcBufModulo    equ  g_Mp3DataSrcBufSize-1

SAFE_NUMBER             equ   $265         

    ENDIF  ; IF (!@def(mp3decxref))

