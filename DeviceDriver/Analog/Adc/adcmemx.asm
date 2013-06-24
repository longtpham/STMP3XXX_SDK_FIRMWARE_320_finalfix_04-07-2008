;******************************************************************************
; Copyright(C) SigmaTel, Inc. 2000
;
; STMP DAC Driver Module XMEM
;******************************************************************************
    Section Adcmemx        
;*******************************************************************************
;  Include files
;*******************************************************************************
    include "adcequ.inc"
    include "hwequ.inc"
    include "sysequ.inc"
        
;*******************************************************************************
;  Extern definitions
;*******************************************************************************

    GLOBAL    AdcBuf
    GLOBAL    AdcIsrSaver7
    GLOBAL    AdcIsrSavex0

    GLOBAL    AdcIsrSave_adiv


    GLOBAL    AdcFlt2Coeffs


    GLOBAL    ADCProcessSampleCount

    EXTERN    AdcFlt2StateVarsCh0
    GLOBAL    AdcFlt2StatePtrCh0
    GLOBAL    AdcFlt3StateVarsCh0
    GLOBAL    AdcFlt3StatePtrCh0


    EXTERN    AdcFlt2StateVarsCh1
    GLOBAL    AdcFlt2StatePtrCh1
    GLOBAL    AdcFlt3StateVarsCh1
    GLOBAL    AdcFlt3StatePtrCh1

    
;*******************************************************************************
;  Extern references
;*******************************************************************************
    
;*******************************************************************************
;  X memory
;*******************************************************************************
        org x,"AdcmemX":
        
        buffer  M,AdcBufSize
AdcBuf      
        bsc     AdcBufSize,0                      ; The ADC port buffer
        endbuf


        buffer  M,AdcFlt2NTaps
AdcFlt2Coeffs
        dc  -0.001590613676
        dc  -0.005212482843
        dc  -0.010058785069
        dc  -0.011967768315
        dc  -0.004030238672
        dc  0.020350794452
        dc  0.062784618006
        dc  0.116494918510
        dc  0.167050459567
        dc  0.197856631120
        dc  0.197856631120
        dc  0.167050459567
        dc  0.116494918510
        dc  0.062784618006
        dc  0.020350794452
        dc  -0.004030238672
        dc  -0.011967768315
        dc  -0.010058785069
        dc  -0.005212482843
        dc  -0.001590613676
        endbuf


                                                  ; Decimation filter state variables (W's)
        buffer  M,AdcFlt3NTaps
AdcFlt3StateVarsCh0
        bsc     AdcFlt3NTaps,0
        endbuf

        buffer  M,AdcFlt3NTaps
AdcFlt3StateVarsCh1
        bsc     AdcFlt3NTaps,0
        endbuf


AdcFlt2StatePtrCh0   dc  AdcFlt2StateVarsCh0
AdcFlt2StatePtrCh1   dc  AdcFlt2StateVarsCh1

AdcFlt3StatePtrCh0   dc  AdcFlt3StateVarsCh0
AdcFlt3StatePtrCh1   dc  AdcFlt3StateVarsCh1



ADCProcessSampleCount dc  0

AdcIsrSavex0    dc      0               ; needed because x0 used before register save in isr
AdcIsrSaver7    dc      0               ; needed because AdcDrvProcess corrupts r7

AdcIsrSave_adiv:    dc      9               ; clckr register adiv bits


    endsec