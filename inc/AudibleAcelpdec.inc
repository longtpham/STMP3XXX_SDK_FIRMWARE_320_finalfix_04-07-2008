;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000
;
; STMP ACELP  Wrapper
;///////////////////////////////////////////////////////////////////////////////

    IF (!@def(acelpdecxref))
acelpdecxref   equ     1

g_AcelpDecSinkBufSize      equ          2048            ; The size of the Acelp decode buffer
g_AcelpDecSinkBufModulo    equ          g_AcelpDecSinkBufSize-1 ; The modulo of the Acelp decode buffer

g_AcelpDataSrcBufSize      equ          $20
g_AcelpDataSrcBufModulo    equ          g_AcelpDataSrcBufSize-1


STREAM_8K5                 equ         10    ; 10 Words= 19 bytes/Frame, 1 byte left for next Fr  

STREAM_6K5                 equ         8     ; 8  Words= 14+1/2 bytes/Frame                       

STREAM_5K0                 equ         10    ; 10 Words = 20 bytes/Frame 14+1/2 bytes/Frame       

STREAM_16K0                equ         10    ; 10 Words = 20 bytes/Frame                          

STREAM_MAX                 equ         0   ; 10 Words= 19 bytes/Frame, 1 byte left for next Fr  

L_FRAME_MAX                equ         40

NB_PRM_MAX                 equ         2

ACELPNET_5K0               equ         1
ACELPNET_6K5               equ         2
ACELPNET_8K5               equ         3
ACELPNET_16K0              equ         4


; =======> 5.0 kbps 

NB_PRM_5K0                 equ         20
L_FRAME_5K0                equ         240


; =======> 6.5 kbps 



RATE_6K5                   equ         1
NB_PRM_6K5                 equ         21
L_FRAME_6K5                equ         144


; =======> 8.5  kbps 

RATE_8K5                   equ         0
NB_PRM_8K5                 equ         21
L_FRAME_8K5                equ         144


; =======> 16 kbps 
                           
NB_PRM_16K0                equ         22      
L_FRAME_16K0               equ         160     



    ENDIF  ; IF (!@def(Acelpdecxref))

