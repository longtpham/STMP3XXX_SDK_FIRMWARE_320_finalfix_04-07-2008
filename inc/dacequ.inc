    IF (!@def(dacequ))
dacequ equ 1

        page 255,255


FlashBufSize          equ     441             ; The size of the Flash buffer 
FlashBufModulo        equ     FlashBufSize-1  ; The modulo of the MP3 decode buffer


DacBlockSize          equ     $40             ; The size of the block we process at each interrupt
 if ("BLD"!="SIMDAC")
DacBufSize            equ     $200            ; Use $41 for Sim only : Temporary : The buffer size dedicated to the DAC port
 else
DacBufSize            equ     $100            ; Use $41 for Sim only : Temporary : The buffer size dedicated to the DAC port
 endif
DacBufModulo          equ     DacBufSize-1    ; The modulo of the DAC buffer 

SimOutL               equ     $8000
SimOutR               equ     $8001

DacCoef1Taps          equ     35
DacData1Size          equ     DacCoef1Taps/2+1
;DacOutputBufferSize  equ     DacData1Size*8


;; DAC 
;; Source frq -> Target 
;; 48.000 kHz -> 48kHz = (((8*48/48    ) - 1) * $010000) =   =>  DACSRR = $070000 
;; 44.100 kHz -> 48kHz = (((8*48/44.1  ) - 1) * $010000) =   =>  DACSRR = $07b51e 
;; 32.000 kHz -> 48kHz = (((8*48/32    ) - 1) * $010000) =   =>  DACSRR = $0b0000 
;; 24.000 kHz -> 48kHz = (((8*48/24    ) - 1) * $010000) =   =>  DACSRR = $0f0000 
;; 22.050 kHz -> 48kHz = (((8*48/22.05 ) - 1) * $010000) =   =>  DACSRR = $106a3b 
;; 16.000 kHz -> 48kHz = (((8*48/16    ) - 1) * $010000) =   =>  DACSRR = $170000 
;; 12.000 kHz -> 48kHz = (((8*48/12    ) - 1) * $010000) =   =>  DACSRR = $1f0000 
;; 11.025 kHz -> 48kHz = (((8*48/11.025) - 1) * $010000) =   =>  DACSRR = $21d476 
;;  8.000 kHz -> 48kHz = (((8*48/8     ) - 1) * $010000) =   =>  DACSRR = $2f0000 

    if (@def('DEVICE_3500'))
; setting for 24MHz crystal, xtal/4=6MHz
DAC48kHz              equ     $06d000         ; load into DACSRR
DAC44_1kHz            equ     $0780df
DAC32kHz              equ     $0ab800
DAC24kHz              equ     $0ea000
DAC22_05kHz           equ     $1001be
DAC16kHz              equ     $167000
DAC12kHz              equ     $1e4000
DAC11_025kHz          equ     $21037c
DAC8kHz               equ     $2de000
    else   ; 3410 values to use in conjunction with setting bit 11 of x:FA00
DAC48kHz              equ     $045555         ; load into DACSRR
DAC44_1kHz            equ     $04CE14
DAC32kHz              equ     $070000
DAC24kHz              equ     $09AAAB
DAC22_05kHz           equ     $0A9C28
DAC16kHz              equ     $0F0000
DAC12kHz              equ     $145555
DAC11_025kHz          equ     $16384F
DAC8kHz               equ     $1F0000
    endif

    ENDIF
