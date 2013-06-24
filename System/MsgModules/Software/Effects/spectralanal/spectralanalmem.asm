;;***********************************************************************
;SigmaTel, Inc
;$Archive:  $
;$Revision: 3 $
;$Date:  $
;Description: 
;Notes:    Spectral Anal Filter coefficients, dBTables. etc.
;        
;***********************************************************************/
    Section    SpectrumAnalMem

;*******************************************************************************
;  Include files
;*******************************************************************************

    include "effects.inc"
    include "Geqequ.inc"

;*******************************************************************************
;  Extern definitions
;*******************************************************************************
        global SAFilterCoefs
        global SA_y_filter_state_ch0
        global SA_y_filter_state_ch1
        global SA_x_filter_state_ch0
        global SA_x_filter_state_ch1
        global SAEvalInterval
        global CurBandSampleCount        
        global HalfBandSampleCount        
        global TotalBandSampleCount    
        global EvalThisbandFlag        
        global SpectralAnalCurBandCnt
        global SpectralAnalCurFilter    
        global PeakSampleValue_ch0
        global PeakSampleValue_ch1        
        global SpectralAnalDecay        
        global FSpectralValArray_ch0,SpectralValArray_ch0
        global FSpectralValArray_ch1,SpectralValArray_ch1
        global SACenterFreq
        global SAFilter_1_by_2Q
;*******************************************************************************
;  Y memory
;*******************************************************************************


    org y,"SpectralAnalCoefmem":


; The filter coefficients for each of the bandpass filters. These values are obtained
; using filtdes.c. Sampling Frequency = 48000 Hz. 
; Use 15 band 2/3 octave filter settings, on ISO frequencies, with constant Q = 2.4.
;----------------------------------------------
;  Type          Center freq       Q        Gain
;----------------------------------------------
; bandpass         25              2.4       1
; bandpass         40              2.4       1
; bandpass         63              2.4       1
; bandpass         100             2.4       1
; bandpass         160             2.4       1
; bandpass         250             2.4       1
; bandpass         400             2.4       1
; bandpass         630             2.4       1
; bandpass         1000            2.4       1
; bandpass         1600            2.4       1
; bandpass         2500            2.4       1
; bandpass         4000            2.4       1
; bandpass         6300            2.4       1
; bandpass         10000           2.4       1
; bandpass         16000           2.4       1


SAFilterCoefs
                DC        0.49931872
                DC        0.00034065
                DC        0.99931335

                DC        0.49891034
                DC        0.00054482
                DC        0.99889660

                DC        0.49828491
                DC        0.00085755
                DC        0.99825096

                DC        0.49728042
                DC        0.00135979
                DC        0.99719495

                DC        0.49565592
                DC        0.00217203
                DC        0.99543750

                DC        0.49322969
                DC        0.00338516
                DC        0.99269783

                DC        0.48921427
                DC        0.00539286
                DC        0.98785853

                DC        0.48312840
                DC        0.00843579
                DC        0.97978729

                DC        0.47352692
                DC        0.01323654
                DC        0.96519828

                DC        0.45848337
                DC        0.02075832
                DC        0.93753821

                DC        0.43723652
                DC        0.03138175
                DC        0.88749748

                DC        0.40566039
                DC        0.04716981
                DC        0.78432488

                DC        0.36731482
                DC        0.06634258
                DC        0.58873397

                DC        0.33247688
                DC        0.08376155
                DC        0.21546087

                DC        0.34715471
                DC        0.07642265
                DC        -0.42357734


; Spectrum evalutation commences after half of the following samples are processed
; This allows filters to settle down.
; Filter settling time ~ 4*SamplingFreq/CenterFreq
; One Spectral evaluation is done every 20704 samples.
; At 44.1 Khz this allows us two spectral updates per second.
 
SAEvalInterval
                dc        7680
                dc        4800
                dc        3048
                dc        1920
                dc        1200
                dc        768
                dc        480
                dc        304
                dc        192
                dc        120
                dc        76
                dc        48
                dc        32
                dc        20
                dc        16
    
    org x,"SAFilterStateMem":

SA_y_filter_state_ch0        DSM            2
SA_x_filter_state_ch0        DSM            2
SA_y_filter_state_ch1        DSM            2
SA_x_filter_state_ch1        DSM            2

SACenterFreq                dc              25    
                            dc              40    
                            dc              63    
                            dc              100   
                            dc              160   
                            dc              250   
                            dc              400   
                            dc              630   
                            dc              1000  
                            dc              1600  
                            dc              2500  
                            dc              4000  
                            dc              6300  
                            dc              10000 
                            dc              16000 

SAFilter_1_by_2Q            dc              0.208333333



        org    y,"SAMem":

HalfBandSampleCount      dc           0
TotalBandSampleCount     dc           0
EvalThisbandFlag         dc           0
SpectralAnalCurFilter    dc           SAFilterCoefs
SpectralAnalDecay        dc           3                    ; 3 dB decrement per call
PeakSampleValue_ch0      dc           0                
PeakSampleValue_ch1      dc           0                

CurBandSampleCount       dc           0
SpectralAnalCurBandCnt   dc           0


        org    y,"SAMem":
FSpectralValArray_ch0
SpectralValArray_ch0     ds           SANumberOfBands        
FSpectralValArray_ch1
SpectralValArray_ch1     ds           SANumberOfBands        

;*******************************************************************************
;  X memory
;*******************************************************************************


    EndSec


