;;***********************************************************************
;SigmaTel, Inc
;$Archive: $
;$Revision: 3 $
;$Date:  $
;Description: 
;Notes:    This is the main file. 
;        
;***********************************************************************/

;   A spectral analyzer could be FFT based or band pass filter based:
;   FFT/DFT based analyser would be costly to implement, as audible acoustic frequencies 
;   span around 10 octaves. To procure bin value of 40 Hz at 48,000 Hz sampling freq
;    requires FFT length approx. 1024. This also means a buffer of this size.

;   Implementing using band pass filters, as a 15 band spectral analyser 
;    would be costly to implement likewise, 
;   approx MIPs would be 15*10*48000 ~ 7.5 MIPs over single channel.

;  We avoid both the problems by
;  1. Computing one band at a time, for both the channels.
;  2. Obviously, this requires the filter response to settle, because filters can't assume history
;  3. We define the settling time equivalent to 2*SamplingFreq/fc so for 40 Hz, this would be 2*48000/40 ~ 2400 samples.
;  4. The spectral peaks will decay gradually with a factor of say 0.7, but ramp up immediately, 
;      the above approach will be a fairly good representation of the spectral data.
;  5. Checking for peak commences after half of the SAInterval has been processed. 
;  6. This allows 2 updates of entire spectrum over a period of 1 second at 44.1 Khz
;  7. Decay factor of 3 dB per call is considered. This is about 6 dB per second.


    Section    SpectralAnalMain

;*******************************************************************************
;  Include files
;*******************************************************************************
        
    include "const.xref"
    include "spectralanal.xref"
    include "effects.inc"
    include "sysmacro.asm"

;*******************************************************************************
;  Extern definitions
;*******************************************************************************
        Global  SpectralAnalProcess
;*******************************************************************************
;  Extern references
;*******************************************************************************
        extern DacSamplestoProcess
        extern SACenterFreq
        extern SAFilter_1_by_2Q

;*******************************************************************************
;  Code start
;*******************************************************************************
    org p,"SpectralAnal":

;;***********************************************************************
;    Function:
;            SpectralAnalProcess
;    Description:
;            
;            
;  Parameters passed: 
;            r3 = Input buffer pointer
;            r4 = Output buffer pointer
;            m3 = Input buffer modulo
;            m4 = Output buffer modulo.
;  Return Values
;           
;
;;***********************************************************************
SpectralAnalProcess
    move    y:CurBandSampleCount,x0
    move    y:HalfBandSampleCount,B
    cmp     x0,B        y:TotalBandSampleCount,B
    jgt     _Continue
    move    y:<Const_000001,y0
    cmp     x0,B        y0,y:EvalThisbandFlag
    jgt     _Continue

    ; Completed evaluation of this band, so go to next band
    ; Update the spectra first.
    move    y:PeakSampleValue_ch0,A
    move    #>SpectralValArray_ch0,r6
    jsr     UpdateSpectra

    move    y:PeakSampleValue_ch1,A
    move    #>SpectralValArray_ch1,r6
    jsr     UpdateSpectra

    move    y:<Const_000001,y0
    move    y:SpectralAnalCurBandCnt,B
    add     y0,B        #>SANumberOfBands,y0
    cmp     y0,B        #0,y0
    teq     y0,B
    move    B,y:SpectralAnalCurBandCnt
    move    y0,y:EvalThisbandFlag                        ; Initialize the evaluation flag.
    move    y0,y:CurBandSampleCount                        ; Clear the counter

    ; Clear filters on channel 0
    move    #SA_y_filter_state_ch0,r2
    move    #SA_x_filter_state_ch0,r6
    move    y0,x:(r2)+
    move    y0,x:(r2)+
    move    y0,x:(r6)+
    move    y0,x:(r6)+

    ; Clear filters on channel 1
    move    #SA_y_filter_state_ch1,r2
    move    #SA_x_filter_state_ch1,r6
    move    y0,x:(r2)+
    move    y0,x:(r2)+
    move    y0,x:(r6)+
    move    y0,x:(r6)+

    ; Clear peak values.
    move    y0,y:PeakSampleValue_ch0
    move    y0,y:PeakSampleValue_ch1


    move    #>SAEvalInterval,y0
    move    #>3,x0
    add     y0,B        B,y0
    mpy     x0,y0,B     B,r6
    asr     B           #>SAFilterCoefs,x0
    move    B0,B
    add     x0,B        y:(r6),A
    asr     A           A,y:TotalBandSampleCount
    move    B,y:SpectralAnalCurFilter
    move    A,y:HalfBandSampleCount

_Continue
    move    y:DacSamplestoProcess,A
    asr     A           y:HalfBandSampleCount,x0            ; Half samples per channel
    cmp     x0,A        y:CurBandSampleCount,y0
    ; Do not ever process more than HalfBandSampleCount
    tgt     x0,A
    add     y0,A        y:SpectralAnalCurFilter,r5
    move    A,y:CurBandSampleCount                        ; Update the counter

    ; Next Set the filter state modulo buffers. 
    move    y:<Const_000001,m2
    move    m2,m6                                    

    ; Process the channel0
    move    #SA_y_filter_state_ch0,r2
    move    #SA_x_filter_state_ch0,r6
    push    r3
    jsr     ApplyBiquadandEvaluate                        ; Apply Biquad
    move    y:EvalThisbandFlag,B
    tst     B        y:PeakSampleValue_ch0,x0
    teq     x0,A                                        ; If equal, then save the value.
    move    A,y:PeakSampleValue_ch0                            ; Save the peak Sample value.


    ; Process the channel1
    pop     r3
    move    #SA_y_filter_state_ch1,r2
    move    #SA_x_filter_state_ch1,r6
    move    (r3)+
    jsr     ApplyBiquadandEvaluate                        ; Apply Biquad
    move    y:EvalThisbandFlag,B
    tst     B        y:PeakSampleValue_ch1,x0
    teq     x0,A                                        ; If equal, then save the value.
    move    A,y:PeakSampleValue_ch1                            ; Save the peak Sample value.


    ; Revert back the modified modulo counters.
    move    y:<Const_ffffff,m2
    move    m2,m6
    
    rts
        

;;***********************************************************************
;    Function:
;            ApplyBiquadandEvaluate
;    Description:
;            
;            
;  Parameters passed: 
;            r3 = Input buffer pointer
;            r4 = Output buffer pointer
;            m2 = 1
;            m6 = 1
;  Return Values: A contains half of the peak value. 
;        So scale it correctly while computing dB values
;           
;
;;***********************************************************************

ApplyBiquadandEvaluate
    move    y:DacSamplestoProcess,A
    asr        A       
;;  move    A,x0        y:PeakSampleValue_ch0,A  ; old code on this line. Use alt line below instead.
    move    A,x0        y:<Const_000000,A ; new code single line change said to solve defect 6830 which was that silence on one chan not displayed. 
    ori        #$08,MR                                        ; To shift left automatically.
    do    x0,_EndBlock
        move    y:(r5)+,y0     x:(r2)+,x0                     ;y0 = a2    x0 = y[n-2]
        mpy     -x0,y0,B       y:(r5)+,y0       x:(r3)+n3,x1  ;y0 = b0    x0 = x[n]
        mac     x1,y0,B        x:(r6),x0                      ;x0 = x[n-2]
        mac     -x0,y0,B       y:(r5)-,y0       x:(r2)+,x0    ;y0 = a1,     x0 = y[n-1]
        macr    x0,y0,B        x1,x:(r6)+                     ;x:(r6) = x[n-1]
        cmpm    A,B            B,x:(r2)+        y:(r5)-,y0    ;y0 = filterResp, x:(r2)=y[n-2], dummy to decrement r5
        tgt     B,A                                           
_EndBlock        
    andi    #$F7,MR                                           ; Restore the shifts.
    rts

    
;;***********************************************************************
;    Function:
;            UpdateSpectra
;    Description:
;            
;     CurPeak = Decay * CurPeak
;     if ( | MeasuredPeak | > CurPeak)
;         CurPeak = | MeasuredPeak |
;   First the peak power is found in dB
;   Then it is placed in range 0 to 64, by  
;     Computed = 64 + dBPower
;    if (Computed < 0) Computed is 0.
;     Takes 63 cycles.

;   Displayed value is 0 -> -64 dB or less
;   Displayed value is 32 -> -32 dB 
;   Displayed 

; value is 64 -> 0 dB
;  Parameters passed: 
;       A = peak value
;       r6 = SpectralValueArray
;  Return Values: A contains half of the peak value. 
;        So scale it correctly while computing dB values
;           
;
;;***********************************************************************


UpdateSpectra
    abs     A      #0,r5
    jeq     _NoNorm
    move    y:<Const_000018,r5            ; Always use positive values        
    rep     #$18
    norm    r5,A    
    nop    
    move    (r5)+                                ; Correct for 6 dB offset
_NoNorm
    move    y:<Const_000018,B
    move    r5,x0
    sub     x0,B        #>$3,y0                
    move    B,x0
    mpy     x0,y0,B     A,y1
    move    B0,y0                                ; 6 dB per bit
    ; Now find the fractional portion (as dB). Granularity 1 dB.
    mpy     y1,#19,A
    move    #0,A0                                ; We are interested in only first 4 bits.
    move    #>14,B
    sub     A,B
    jle     _Continue
    asr     B        y0,A                        ; Find fractional dB approx. Correct to 0.5 dB
    add     B,A
    move    A,y0
_Continue
    ; y0  Contains peak power in dB value, should be read negative
    ; Convert it to a form which is convenient to display.
    move    y:<Const_000040,A                   ; On scale 0 to 64.    
    sub     y0,A    #0,y0
    tlt     y0,A                                ; Do not let the value become negative
    move    y:SpectralAnalCurBandCnt,n6
    move    y:SpectralAnalDecay,x0              ; Around 3dB decrement everytime
    move    y:(r6+n6),B
    sub     x0,B        #0,x0
    tlt     x0,B                                ; Do not let displayed value become negative.
    cmp     A,B
    tlt     A,B    
    move    B,y:(r6+n6)                            
    rts

    Endsec


