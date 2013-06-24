;******************************************************************************
; Copyright(C) SigmaTel, Inc. 2000
;
; STMP DAC Driver Module XMEM
;******************************************************************************
        
;*******************************************************************************
;  Include files
;*******************************************************************************
        include "dacequ.inc"

        
;*******************************************************************************
;  Extern definitions
;*******************************************************************************
        GLOBAL    DacBuf
        GLOBAL    DacIsrSavex0
        GLOBAL    DacIsrSaver7
 if @def('TUNER_STFM1000')
        GLOBAL    RunStfmFilter  ; Trigger the STFM filter to run
        GLOBAL    DacIsrSaveIPR
 endif


;*******************************************************************************
;  Extern references
;*******************************************************************************

    
;*******************************************************************************
;  X memory
;*******************************************************************************
        org     x,"Dacmem_Mod_X":
        
        buffer  M,DacBufSize
DacBuf      
        bsc     DacBufSize,0                      ; The DAC port buffer
        endbuf

       	org     x,"DacMem_X":

DacIsrSavex0    dc      0               ; needed because x0 used before register save in isr
DacIsrSaver7    dc      0               ; needed because DacDrvProcess corrupts r7
 if @def('TUNER_STFM1000')
RunStfmFilter   dc      0               ; Trigger the STFM filter to run
DacIsrSaveIPR   dc      0               ; Trigger the STFM filter to run
 endif



;*******************************************************************************
;  Y memory
;*******************************************************************************
; !!!!!!!!!!!!! All DAC ymem goes in dacmemy.asm
