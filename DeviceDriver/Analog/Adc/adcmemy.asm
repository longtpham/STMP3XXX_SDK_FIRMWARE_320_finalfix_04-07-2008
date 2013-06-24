;******************************************************************************
; Copyright(C) SigmaTel, Inc. 2000
;
; STMP ADC Driver Module YMEM
;******************************************************************************
    section AdcMemY        
;*******************************************************************************
;  Include files
;*******************************************************************************
    include "adcequ.inc"
    include "hwequ.inc"
    include "sysequ.inc"
        
;*******************************************************************************
;  Extern definitions
;*******************************************************************************
    EXTERN  EncoderSourceBufDesc
    EXTERN  AdcBuf

;*******************************************************************************
;  GLOBAL definitions
;*******************************************************************************

    
    GLOBAL    AdcDrvSinkBufDescPtr
    GLOBAL    AdcIsrEntryCount
    GLOBAL    AdcOfErrCount
    GLOBAL    AdcSinkOfCounter
    GLOBAL    AdcSoftOfCount
    GLOBAL    AdcIsrSampleCountHigh
    GLOBAL    AdcIsrSampleCountLow
    GLOBAL    AdcSourceBufDesc
    GLOBAL    AdcSinkBufDesc  
    GLOBAL    AdcBufDescriptor   
    GLOBAL    AdcBufAddrPtr
    GLOBAL    AdcBufModPtr       
    GLOBAL    AdcBufSizePtr      
    GLOBAL    AdcBufHeadPtr      
    GLOBAL    AdcBufTailPtr  
    GLOBAL    AdcBufTailPtrTemp  
    GLOBAL    AdcFlt1Coeffs    
    GLOBAL    AdcFlt2Coeffs    
    GLOBAL    AdcFlt3Coeffs    
    GLOBAL    EncoderBufHeadPtrTemp
    GLOBAL    AdcEncoderBufModPtr
    GLOBAL    AdcFlt2StateVarsCh0
    GLOBAL    AdcFlt2StateVarsCh1
    

;*******************************************************************************
;  X memory
;*******************************************************************************
; !!!!!!!!!!!!! All DAC xmem goes in dacmem.asm

;*******************************************************************************
;  Y memory
;*******************************************************************************
    org y,"AdcmemY":

        buffer  M,AdcFlt3NTaps
AdcFlt3Coeffs
        dc  0.002484491
        dc  0.007461870
        dc  0.010270138
        dc  0.005247340
        dc  -0.00484901
        dc  -0.00912268
        dc  -0.00271584
        dc  0.004532082
        dc  0.001424069
        dc  -0.00685632
        dc  -0.00608846
        dc  0.003258106
        dc  0.005191196
        dc  -0.00488004
        dc  -0.01005750
        dc  0.000135682
        dc  0.009190375
        dc  -0.00011608
        dc  -0.01350802
        dc  -0.00637524
        dc  0.011668052
        dc  0.008380299
        dc  -0.01437994
        dc  -0.01709538
        dc  0.010075082
        dc  0.021609684
        dc  -0.00922267
        dc  -0.03374890
        dc  -0.00102152
        dc  0.043391962
        dc  0.013050154
        dc  -0.06446916
        dc  -0.04375332
        dc  0.137410615
        dc  0.329146241
        dc  0.329146241
        dc  0.137410615
        dc  -0.04375332
        dc  -0.06446916
        dc  0.013050154
        dc  0.043391962
        dc  -0.00102152
        dc  -0.03374890
        dc  -0.00922267
        dc  0.021609684
        dc  0.010075082
        dc  -0.01709538
        dc  -0.01437994
        dc  0.008380299
        dc  0.011668052
        dc  -0.00637524
        dc  -0.01350802
        dc  -0.00011608
        dc  0.009190375
        dc  0.000135682
        dc  -0.01005750
        dc  -0.00488004
        dc  0.005191196
        dc  0.003258106
        dc  -0.00608846
        dc  -0.00685632
        dc  0.001424069
        dc  0.004532082
        dc  -0.00271584
        dc  -0.00912268
        dc  -0.00484901
        dc  0.005247340
        dc  0.010270138
        dc  0.007461870
        dc  0.002484491

        endbuf
    


; Moved to y. It saves 1 MIPs.
                                                  ; Decimation filter state variables (W's)
        buffer  M,AdcFlt2NTaps
AdcFlt2StateVarsCh0
        bsc     AdcFlt2NTaps,0
        endbuf

        buffer  M,AdcFlt2NTaps
AdcFlt2StateVarsCh1
        bsc     AdcFlt2NTaps,0
        endbuf

    
AdcSourceBufDesc         dc     AdcBufDescriptor
AdcSinkBufDesc           dc     EncoderSourceBufDesc   ; This is changed by the algorithm to point to its own source buffer

;-----------
; These must stay in this order:
AdcBufDescriptor         
AdcBufAddrPtr            dc      AdcBuf
AdcBufModPtr             dc      AdcBufModulo
AdcBufSizePtr            dc      AdcBufSize
AdcBufHeadPtr            dc      AdcBuf                ; AdcBuf Head Ptr
AdcBufTailPtr            dc      AdcBuf                ; AdcBufTailPtr
;-----------
AdcBufTailPtrTemp        dc      AdcBuf 
EncoderBufHeadPtrTemp    dc      0
AdcEncoderBufModPtr      dc      0

AdcDrvSinkBufDescPtr     dc      AdcBufDescriptor

AdcOfErrCount            dc      0                     ; Counter for ADC overflow exceptions
AdcOfBuffCount           dc      0                     ; Counter for number of silence buffers inserted (for overflows)
AdcSinkOfCounter         dc      0                     ; Counter for number of times we get to the ADC isr
                                                       ;   and there's no room in the sink buffer.
AdcSoftOfCount           dc      0                     ; Counter for source full, sink full condition (soft overflow)

AdcIsrEntryCount        dc      0                   ; Keeps track of # of entries to ISR

AdcIsrSampleCountHigh   dc      0
AdcIsrSampleCountLow    dc      0


    

    endsec
