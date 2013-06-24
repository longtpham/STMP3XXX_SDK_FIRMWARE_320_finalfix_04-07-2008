;******************************************************************************
; Copyright(C) SigmaTel, Inc. 2000-2003
;
; STMP ADC Driver Module
;******************************************************************************
; Module Files:
;   \common\adc\adcdrv.asm
;   \common\adc\adcmemx.asm
;   \common\adc\adcmemy.asm
;   \inc\adcequ.asm
;
;******************************************************************************
    section AdcDrv
;*******************************************************************************
;  Include files
;*******************************************************************************

        page    132,60,0,0,0

    nolist
    include "resource.inc"
    include "sysresources.inc"
    include "project.inc"
    include "sysequ.inc"
    include "hwequ.inc"
    include "adcequ.inc"
    include "msgequ.inc"
    include "decoderequ.inc"
    include "encadpcmequ.inc"
    include "sysmacro.asm"
    include "systrace.asm"
    include "stack.xref"
    include "stackmem.xref"
    include "const.xref"
    list
    page    132,60,0,0,0

;*******************************************************************************
;  Extern definitions
;*******************************************************************************

    GLOBAL    AdcDriverInit
    GLOBAL    AdcDriverIsr
    GLOBAL    AdcDriverEnablePort,ClearADCFilterStates
    GLOBAL    AdcDriverDisablePort
    
;*******************************************************************************
;  Extern references
;*******************************************************************************

    EXTERN    AdcDrvSinkBufDescPtr
    EXTERN    AdcIsrSaver7
    EXTERN    AdcIsrSavex0
    EXTERN    AdcIsrEntryCount
    EXTERN    AdcOfErrCount
    EXTERN    AdcSinkOfCounter
    EXTERN    AdcSoftOfCount
    EXTERN    AdcIsrSampleCountHigh
    EXTERN    AdcIsrSampleCountLow
    EXTERN    GetFilledSpace
    EXTERN    GetEmptySpace
    EXTERN   SysAllLinear


    EXTERN    AdcSourceBufDesc
    EXTERN    AdcSinkBufDesc  
    EXTERN    AdcBufDescriptor   
    EXTERN    AdcBufAddrPtr
    EXTERN    AdcBufModPtr       
    EXTERN    AdcBufSizePtr      
    EXTERN    AdcBufHeadPtr      
    EXTERN    AdcBufTailPtr      
    EXTERN    AdcBufTailPtrTemp      
    EXTERN    AdcIsrSave_adiv
    EXTERN    Fg_wEncNumberOfChannels
    EXTERN    EncoderBufHeadPtrTemp
    EXTERN    AdcEncoderBufModPtr

    EXTERN    EncAdcGain
    EXTERN    EncADCSRR

    EXTERN    AdcFlt2Coeffs
    EXTERN    AdcFlt3Coeffs

    EXTERN    ADCProcessSampleCount

;    EXTERN    AdcFlt1StateVarsCh0
;    EXTERN    AdcFlt1StatePtrCh0
    EXTERN    AdcFlt2StateVarsCh0
    EXTERN    AdcFlt2StatePtrCh0
    EXTERN    AdcFlt3StateVarsCh0
    EXTERN    AdcFlt3StatePtrCh0

;    EXTERN    AdcFlt1StateVarsCh1    
;    EXTERN    AdcFlt1StatePtrCh1
    EXTERN    AdcFlt2StateVarsCh1
    EXTERN    AdcFlt2StatePtrCh1
    EXTERN    AdcFlt3StateVarsCh1
    EXTERN    AdcFlt3StatePtrCh1

	EXTERN	Fg_wEncSamplingRate

  ; stmp6964  Remove DC offset
    EXTERN    EncAdcLChOffset
    EXTERN    EncAdcRChOffset
  ;  EXTERN  MP3EncoderSourceBufDesc2

    comment ~
    ; TODO - Get rid of this stuff!!!! Used only for adpcm dev.
    EXTERN    CurrAdcIsrCount
    EXTERN    CurrAdcOfCount
    EXTERN    CurrAdcSinkEmptySpace
    ~
    
;*******************************************************************************
;  Code start
;*******************************************************************************

    org p,".pAdcDrv":
;******************************************************************************
;       AdcDriverInit
;
;       Same as all other init routines.  Must set internal status to
;       conditions.
;       r0 : Contains pointer to the buffer descriptor
;******************************************************************************
AdcDriverInit
    move    r0,y:AdcSinkBufDesc
    move    (r0)+
    move    y:(r0),r0
    move    r0,y:AdcEncoderBufModPtr    
    
    clr     a
    move    a,y:AdcIsrEntryCount
    move    a,y:AdcOfErrCount
    move    a,y:AdcIsrSampleCountHigh
    move    a,y:AdcIsrSampleCountLow
    move    a,y:AdcSoftOfCount
    rts

;******************************************************************************
;       AdcDriverEnablePort
;
;       Arguments:
;
;******************************************************************************
AdcDriverEnablePort
    jsr     ClearADCFilterStates    



    if (@def('DEVICE_3400')||@def('DEVICE_3300'))
        move    y:Const_000000,x0                   ; Clear ADCCSR.
        move    x0,x:HW_ADCCSR

        bclr    #HW_MIXPWRDNR_PR0_BITPOS,x:HW_MIXPWRDNR ; Enable ADC and input MUX circuitry
    else
        move    y:Const_000000,x0                   ; Clear ADCCSR.  This enables the ADC clocks
        move    x0,x:HW_ADCCSR

        bclr    #HW_MIXPWRDNR_PR0_BITPOS,x:HW_MIXPWRDNR ; Enable ADC and input MUX circuitry
    endif


    ; 1. Program x:HW_ADCBAR (ADC Base Address Register)
    move    y:AdcDrvSinkBufDescPtr,r0 ; get sink buffer descriptor ptr
    move    #>BD_BufferBaseAddress,n0
    nop
    move    y:(r0+n0),x0            ; x0 = buffer base address
    move    x0,x:HW_ADCBAR          ; init ADC port base addr reg with the buffer base address

    ; 2. Program x:HW_ADCMR (ADC Modulo Register)
    move    #>BD_BufferModulo,n0    ; offset for the sink buffer modulo
    nop
    move    y:(r0+n0),x0            ; x0 = buffer modulo
    move    x0,x:HW_ADCMR           ; init ADC port modulo register with the buffer modulo

    ; 3. Program x:HW_ADCCPR (Adc Current Position Register)
    ;    CPR = offset from base address.
    move    #>BD_TailPtr,n0
    nop
    move    y:(r0+n0),a             ; a = tail ptr
    move    #>BD_BufferBaseAddress,n0
    nop
    move    y:(r0+n0),x0            ; x0 = base address
    sub     x0,a
    move    a,x:HW_ADCCPR           ; current position register

    ; 4. Program x:HW_ADCWCR (ADC Word Count Register)
    move    #>AdcBlockSize,x0
    move    x0,x:HW_ADCWCR          ; keep a constant interrupt rate

    ; 5. Program x:HW_ADCSRR (Sample Rate Register)
    
    if (@def('DEVICE_3400')||@def('DEVICE_3300'))
        bset    #HW_CMTR_ADCKL_BITPOS,x:HW_CMTR ; make the ADC clock slave to the ADIV bits
    else
        ; No such bit on 3410 device. DAC and ADC have 2 separate dividers        
    endif
    

    if (@def('DEVICE_3400')||@def('DEVICE_3300'))

        move    x:HW_CCR,x0             ; save the adiv bits
        move    #>HW_CCR_ADIV_SETMASK,a
        and     x0,a
        move    a1,x:AdcIsrSave_adiv

        bset    #HW_CCR_ADIV_B0_BITPOS,x0                   ;set the adiv bits
        bset    #HW_CCR_ADIV_B1_BITPOS,x0
        bset    #HW_CCR_ADIV_B2_BITPOS,x0
        move    x0,x:HW_CCR

    else
        move    x:HW_CCR,x0             ; save the adiv bits
        move    #>(HW_CCR_ADCDIV_SETMASK|HW_CCR_DACDIV_SETMASK),a
        and     x0,a
        move    a1,x:AdcIsrSave_adiv

        ;set ADCDIV to 0(for FAnalog = 6MHz) and DACDIV to 7(for FAnalog = 1MHz)
		;move    #>(HW_CCR_ADCDIV_SETMASK|HW_CCR_DACDIV_SETMASK),a
        ;or      x0,a
        ;move    a1,x:HW_CCR

		; Set DACDIV to 7(for FAnalog = 1MHz)
		move    #>(HW_CCR_DACDIV_SETMASK),a
        or      x0,a
        move    a1,x:HW_CCR

		; For Sampling Rates greater than 22050Hz FAnalog = 6MHz for better SNR #'s
		; and for sampling rates less than 22050Hz FAnalog is still maintained at 1MHz 
		; because with 6MHz and even with no scaling after analog to digital conversion, 
		; output signal is getting clipped. 
		clr a
		clr b
		move	y:Fg_wEncSamplingRate,a
		move	#>16000,b
		cmp	b,a
		jgt	FAnalog6MHz

FAnalog1MHz
		clr a
		; Set ADCDIV to 7(for FAnalog = 1MHz)
		move    #>(HW_CCR_ADCDIV_SETMASK),a
        or		x0,a
        move    a1,x:HW_CCR
		jmp FAnalogComplete

FAnalog6MHz
		clr a
		; Set ADCDIV to 0(for FAnalog = 6MHz)
		move    #>(HW_CCR_ADCDIV_SETMASK),a
		not		a
        and     x0,a
        move    a1,x:HW_CCR
FAnalogComplete

    endif
                                        ; set the sample rate
    move    y:EncADCSRR,x0
    move    x0,x:HW_ADCSRR
    
    ; enable the port
    clr     a
    move    a,x:HW_ADCCSR           ; clear out any old stuff
    bset    #HW_ADCCSR_TXEN_BITPOS,a1
    bset    #HW_ADCCSR_TXIEN_BITPOS,a1
    move    a1,x:HW_ADCCSR

    ; Enable Low Pass Filter after Sigma Delta for better ADC SNR 
	clr		a
	move    x:HW_ADCCSR,x0             
    move    #>(HW_ADCCSR_LPFMODE_SETMASK),a
    or      x0,a
    move    a1,x:HW_ADCCSR

    rts



ClearADCFilterStates
    clr     a
    move    y:Const_ffffff,m0
    move    y:Const_ffffff,m4
     
;    move    #AdcFlt1StateVarsCh0,r0
;    move    r0,x:AdcFlt1StatePtrCh0
;    move    #AdcFlt1StateVarsCh1,r4
;    move    r4,x:AdcFlt1StatePtrCh1
;    do      #AdcFlt1NTaps,_flt1_loop
;        move    a,x:(r0)+
;        move    a,x:(r4)+
;_flt1_loop

    move    #AdcFlt2StateVarsCh0,r0
    move    r0,x:AdcFlt2StatePtrCh0
    move    #AdcFlt2StateVarsCh1,r4
    move    r4,x:AdcFlt2StatePtrCh1

    do      #AdcFlt2NTaps,_flt2_loop
;        move    a,x:(r0)+
        move    a,y:(r0)+
;        move    a,x:(r4)+
        move    a,y:(r4)+
_flt2_loop

    move    #AdcFlt3StateVarsCh0,r0
    move    r0,x:AdcFlt3StatePtrCh0
    move    #AdcFlt3StateVarsCh1,r4
    move    r4,x:AdcFlt3StatePtrCh1

    do      #AdcFlt3NTaps,_flt3_loop
        move    a,x:(r0)+
        move    a,x:(r4)+
        
_flt3_loop
    rts







;******************************************************************************
;       AdcDriverDisablePort
;
;******************************************************************************
AdcDriverDisablePort
    bclr    #HW_ADCCSR_TXEN_BITPOS,x:HW_ADCCSR
    
    ; restore the ADIV bits
    clr     a           x:HW_CCR,x0

    if (@def('DEVICE_3400')||@def('DEVICE_3300'))
        move    #>(HW_CCR_ADIV_CLRMASK),a

                                                        ; Power savings:
        bset    #HW_MIXPWRDNR_PR0_BITPOS,x:HW_MIXPWRDNR  ;   Disable ADC and input MUX circuitry
    else
        move    #>(HW_CCR_ADCDIV_CLRMASK&HW_CCR_DACDIV_CLRMASK),a

                                                        ; Power savings:
        bset    #HW_MIXPWRDNR_PR0_BITPOS,x:HW_MIXPWRDNR ;   Disable ADC and input MUX circuitry
        bset    #HW_ADCCSR_CLKGT_BITPOS,x:HW_ADCCSR     ;   Disable ADC clocks
    endif        


    and     x0,a        x:AdcIsrSave_adiv,x0
    or      x0,a
    move    a1,x:HW_CCR

        
    rts





;******************************************************************************
;       AdcDriverIsr
;
;       This is the ISR that is vectored in the vector table.  It must
;       return with an rti since it will be called at interrupt time.  It
;       will use the AdcDriverProcess routine to complete its functionality.
;
;		The job of the ADC ISR is to take new samples from the ADC Source Buffer
;		(which were put there by the ADC DMA), filter-decimate them (decimate by 
;		8), and leave the results in the ADC Sink Buffer (aka the Encoder Source 
;		Buffer).  The data in the Adc Source Buffer is interleaved stereo.  The
;		output buffer may be either inlerleaved stereo or mono (the other channel
;		is discarded).
;
;		The ADC ISR gets called when either the ADC Word Count goes to zero or
;		when there is an ADC Overrun.  An ADC Overrun occurs one sample period
;		after the Word Count goes to zero, if the Word Count has not been reset
;		in time (e.g. when interrupts are blocked)  Note the the "sample period"
;		in this situation is for the 8x oversampled rate, so it can be rather short.
;		In any case, an Overrun interrupt (TXEXC - Transmit exception) is always
;		coupled with a regular "done" interrupt (TXI), where the Word Count has 
;		gone to zero.
;
;		The ADC Source Buffer and ADC Sink Buffers are ring (modulo) buffers - 
;		they wrap around.  The ADC hardware automatically wraps around for the ADC 
;		Source buffer.  Both buffers have "head" and "tail" pointers.  The "head"
;		points at the next samples to be added to the buffer.  The "tail" points
;		at the next sample to be taken from the buffer (for processing).
;
;		Even if the Word Count goes to zero, the ADC continues to sample and DMA
;		data into the ADC Source Buffer.  The Word Count "wraps" around, and the
;		Current Position Register (HW_ADCCPR) continues to advance (modulo the 
;		buffer size).  So, if interrupts are blocked, an Overrun may be declared
;		but no data is actually lost (unless interrupts are blocked long enough
;		for the Current Position to catch up with the "tail" pointer).
;
;		The scheme of the ADC ISR is *not* to handle a fixed set of samples on
;		each interrupt.  Instead, the ISR processes as much data as it can.  How
;		much that is depends on how many unprocessed samples there are in the
;		ADC Source Buffer and on how much empty space is available in the ADC
;		Sink Buffer (Encoder Source Buffer).  You may have 64 input samples available
;		for processing but the Encoder may not have enough space to accept all of
;		the results.  So, although the ISR nominally processes AdcBlockSize samples
;		(typically, 64) on each pass, the actual number may vary from one time
;		to the next.
;
;		Likewise, the Word Count Register is not always reset to the same number.

;
;       NOTE: For testing purposes, there are some "if (@def..." conditions
;       below (PRESCALE and NOFILTS).  These can be used to study the effects
;       of certain parts of the algorithm.  If PRESCALE is defined, the scaling
;       factor will be applied before the filter-decimators, instead of after.
;       This may cause numerical overflow in the filters.  If NOFILTS is defined,
;       the filter stages will be disabled;  decimation will still take place,
;       but aliasing will occur.
;******************************************************************************
AdcDriverIsr


        ; Clear the DM bit (double-precision multiply mode, bit 6) of the Mode Register
        ; This will be returned to its previous state when the RTI occurs (since the SR
        ; will be popped back off System Stack Low (SSL).
        ; Accumulator behavior is unpredictable when the DM bit is set.
        ; Next cycle after clearing it should NOT be an ALU instruction.
        andi #$BF,mr

    push    x0          ; Save this
    
    ; Check if spurious interrupt. If so exit ...
    move    x:HW_ICLSTATUS0R,x0
    nop
    jset    #HW_ICLSTATUS0R_SST2_BITPOS,x0,realADCInt       ;Check ADC Refill
    jset    #HW_ICLSTATUS0R_SST3_BITPOS,x0,realADCInt       ;Check ADC Overflow

    ; None of the above, exit interruption

    pop     x0          ; Restore x0
    rti




realADCInt
    pop     x0          ; put back the old value



;///////////////////////////////////////////////////////////////////////////////
;  Save state
;///////////////////////////////////////////////////////////////////////////////

saveRegisters
    move    r7,y:IntL3_SaveR7       ; save minimal reg set to local
    move    m7,y:IntL3_SaveM7
    move    r6,y:IntL3_SaveR6
    move    #>L3DataStack,r7        ; setup the L2 data stack
    move    y:<Const_ffffff,m7
    move    #adcSaveRegReturn,r6   ; setup the return address
    jmp     SaveRegs                ; save the remaining registers
adcSaveRegReturn
    move    #adcSaveSysStackRtn,r6 ; setup the return address
    move    #L3Sp,r5                ; pointer to the SP local save
    jmp     SaveSystemStack         ; save the hardware stack
adcSaveSysStackRtn
    move    r7,x:AdcIsrSaver7       ; needed because r7 is used below

    jsr     SysAllLinear    



;///////////////////////////////////////////////////////////////////////////////
;  keep track of entries to this isr
;///////////////////////////////////////////////////////////////////////////////

    clr     a
    move    y:AdcIsrEntryCount,a0
    inc     a
    move    a0,y:AdcIsrEntryCount



;///////////////////////////////////////////////////////////////////////////////
;  Determine how much unprocessed data is in the Adc Source Buf
;
;   oldFilledSpace = GetFilledSpace(AdcBuf)             // What size was it?
;   AdcSourceheadPtr = AdcSourceBaseAddr + HW_ADCCPR    // Update Head Ptr
;   newFilledSpace = GetFilledSpace(AdcBuf)             // What size, now?
;   if(newFilledSpace < oldFilledSpace) {
;       HardwareTailOverrun
;   }
;
;///////////////////////////////////////////////////////////////////////////////

    move    y:AdcSourceBufDesc,r0   ; oldFilledSpace = GetFilledSpace(AdcBuf)    // What size was it?
    move    #$ffff,m0
    jsr     GetFilledSpace
    move    a,b                     ; (b = oldFilledSpace)

                                    
    move    y:AdcBufAddrPtr,a       ; AdcSourceheadPtr = AdcSourceBaseAddr + 
    move    x:HW_ADCCPR,x1          ;                    HW_ADCCPR     // Update Head Ptr
    add     x1,a
    move    a1,y:AdcBufHeadPtr

    move    y:AdcSourceBufDesc,r0
    move    #$ffff,m0
    jsr     GetFilledSpace          ; (a = newFilledSpace)

    cmp     b,a                     ; if(newFilledSpace < oldFilledSpace) {
    jge     adc_no_tail_overrun     ;   Tail Overrun (the head caught up with the tail)
    jsr	    adc_tail_overrun        ; }

adc_no_tail_overrun



;///////////////////////////////////////////////////////////////////////////////
; see how much empty space the sink buffer has
;  
;   AdcSinkBufSpace = GetEmptySpace(AdcSinkBuf)
;   //if (AdcSinkBufSpace <= 0) {
;   //    ADC Sink Buffer Overrun
;   //}

;///////////////////////////////////////////////////////////////////////////////


    move    y:AdcSinkBufDesc,r4
    move    #$ffff,m4
    jsr     GetEmptySpace           ; a = AdcSinkBufSpace



;///////////////////////////////////////////////////////////////////////////////
;  if (mono)
;     AdcSinkBufNumSamples = AdcSinkBufSpace
;  else
;     AdcSinkBufNumSamples = AdcSinkBufSpace/2
;///////////////////////////////////////////////////////////////////////////////

    move    y:Fg_wEncNumberOfChannels,x0
    rep     x0
    asr     A
    asl     A                       


;///////////////////////////////////////////////////////////////////////////////
;  get amount of data waiting in source buffer
;
;   AdcSrcBufSpace = GetFilledSpace(AdcBuf)
;   AdcSrcBufNumSamples = AdcSrcBufSpace / (8 * 2)   // decimate by 8, stereo input
;   NumSamplesToProcess = MIN(AdcSrcBufNumSamples, AdcSinkBufNumSamples)
;
;///////////////////////////////////////////////////////////////////////////////

    push    a1                      ; temp save AdcSinkBufNumSamples

    move    y:AdcSourceBufDesc,r0   ; AdcSrcBufSpace = GetFilledSpace(AdcBuf)
    move    #$ffff,m0
    jsr     GetFilledSpace

    rep     #4                      ; AdcSrcBufNumSamples = 
    asr     a                       ;       AdcSrcBufSpace / (8 * 2) // decimate by 8, stereo input


    pop     x0                      ; NumSamplesToProcess = MIN(AdcSrcBufNumSamples, AdcSinkBufNumSamples)
    cmp     x0,a
    jge     use_sink_count
    move    a1,x0
use_sink_count
    tfr     x0,A




;///////////////////////////////////////////////////////////////////////////////
;  if (NumSamples <= 0)
;       exit
;///////////////////////////////////////////////////////////////////////////////

    tst     a 
    jle     reset_word_count





;///////////////////////////////////////////////////////////////////////////////
;  Filter-decimate the data from the Adc Src Buf to the Adc Sink Buf
;///////////////////////////////////////////////////////////////////////////////

    move    y:EncAdcGain,x1  ;  get scaling factor (sample-rate dependent)
                             

    move    #AdcFlt2NTaps-1,m1
    move    m1,m5

    move    #AdcFlt3NTaps-1,m2
    move    m2,m6

    move    x0,x:ADCProcessSampleCount

    move    x:AdcFlt2StatePtrCh0,r1            ; Use R1 and R5 for the second filter    
    move    x:AdcFlt3StatePtrCh0,r2            ; Use R2 and R6 for the third filter    


    move    y:AdcBufTailPtr,r3
    move    r3,y:AdcBufTailPtrTemp      

    move    y:AdcSinkBufDesc,r3
    move    #>BD_HdPtr,n3
    nop
    move    y:(r3+n3),r3
    move    r3,y:EncoderBufHeadPtrTemp  

    move y:EncAdcLChOffset,y1   ; stmp6964 Remove DC Offset
     
    jsr     ProcessSingleChannel

    move    r1,x:AdcFlt2StatePtrCh0
    move    r2,x:AdcFlt3StatePtrCh0
    
    move    y:AdcBufTailPtrTemp,r0      
    move    y:AdcBufTailPtr,r3    
    nop      
    move    (r3)+
    move    r0,y:AdcBufTailPtr
    move    r3,y:AdcBufTailPtrTemp      

    move    y:EncoderBufHeadPtrTemp,r0
    move    y:AdcSinkBufDesc,r3
    move    #>BD_HdPtr,n3
    nop
    move    (r3)+n3
    nop
    move    y:(r3),y0
    move    r0,y:(r3)
    move    y0,r3
    nop
    move    (r3)+
    move    r3,y:EncoderBufHeadPtrTemp  

;; Start - Save in descriptor used by DAC  
;    move    #>MP3EncoderSourceBufDesc2,r3
;    nop
;    move    (r3)+n3     ; point to HdPtr
;    nop
;    move    r0,y:(r3)    
;    move     y:EncoderBufHeadPtrTemp,r3 
;; End - Save in descriptor used by DAC    
 
 
    move    y:Fg_wEncNumberOfChannels,A
    move    y:<Const_000001,x0
    cmp     x0,A
    jeq     _MonoProcessing

    
    
    move    x:AdcFlt2StatePtrCh1,r1            ; Use R4 and R5 for the second filter    
    move    x:AdcFlt3StatePtrCh1,r2            ; Use R2 and R6 for the third filter    

    move y:EncAdcRChOffset,y1   ; stmp6964 Remove DC Offset
      
    jsr     ProcessSingleChannel

    move    r1,x:AdcFlt2StatePtrCh1
    move    r2,x:AdcFlt3StatePtrCh1

_MonoProcessing
    move    y:<Const_ffffff,m1
    move    y:<Const_ffffff,m2
    move    y:<Const_ffffff,m5
    move    y:<Const_ffffff,m6


;///////////////////////////////////////////////////////////////////////////////
;  keep track of total samples
;
;   dwAdccIsrSampleCount += ADCProcessSampleCount
;
;///////////////////////////////////////////////////////////////////////////////

    clr     a
    move    x:ADCProcessSampleCount,a0
    move    y:AdcIsrSampleCountHigh,x1
    move    y:AdcIsrSampleCountLow,x0
    add     x,a
    move    a1,y:AdcIsrSampleCountHigh
    move    a0,y:AdcIsrSampleCountLow




;///////////////////////////////////////////////////////////////////////////////
;
;  reset_word_count
;
;  Determine the space available in the ADC Src buffer.
;  First, update the Head pointer (the Current Position may have changed while 
;  we were busy).
;  Then determine the number of samples to take.
;
;   AdcSrcHeadPtr = AdcSrcBaseAssr + HW_ADCCPR  // Update Head Ptr
;
;	AdcEmptySpace = GetEmptySpace(AdcBuf)
;
;   if (AdcEmptySpace >= 2*AdcBlockSize)
;       newWordCount = AdcBlockSize
;   else if (AdcEmptySpace > AdcBlockSize)
;       newWordCount = AdcEmptySpace - AdcBlockSize
;   else
;        Software Overrun     // This will jump back to reset_word_count
;
;   HW_ADCWCR = newWordCount
;
;///////////////////////////////////////////////////////////////////////////////

reset_word_count


    move    y:AdcBufAddrPtr,a       ; AdcSourceHeadPtr = AdcSourceBaseAddr + 
    move    x:HW_ADCCPR,x1          ;                    HW_ADCCPR      // Update Head Ptr
    add     x1,a
    move    a1,y:AdcBufHeadPtr

    move    y:AdcSourceBufDesc,r4   ; AdcEmptySpace = GetEmptySpace(AdcBuf)
    move    #$ffff,m4
    jsr     GetEmptySpace



    move    #>AdcBlockSize,b        ;   if (AdcEmptySpace >= 2*AdcBlockSize)
    asl     b
    cmp     a,b
    jgt     less_than_2_blocks_available
        move    #>AdcBlockSize,a    ;       newWordCount = AdcBlockSize
        jmp     new_word_count_ready

less_than_2_blocks_available    
    asr     b                       ;   else if (AdcEmptySpace > AdcBlockSize)
    cmp     a,b
    jge     adc_software_overrun
    asr   a                         ;       newWordCount = AdcEmptySpace/2
                                    ;   else
                                    ;        Software Overrun  // This will jump back to reset_word_count

new_word_count_ready
    if (@def('DEVICE_3500'))
    move    x:HW_ADCWCR,x0
    sub     x0,a
    move    a,x:HW_ADCWCR
    else
    move    a,x:HW_ADCWCR           ; HW_ADCWCR = newWordCount
    endif





;///////////////////////////////////////////////////////////////////////////////
;  Common exit for ISR
;  Clear interrupt bits.
;  Restore regs.
;///////////////////////////////////////////////////////////////////////////////


exitAdcIsr
    move    x:HW_ADCCSR,x0           ; Clear TXI and TXEXC bits
    bclr    #HW_ADCCSR_TXEXC_BITPOS,x0
    bclr    #HW_ADCCSR_TXI_BITPOS,x0
    move    x0,x:HW_ADCCSR

    move    x:AdcIsrSaver7,r7
    move    #$ffff,m7
restoreRegisters
    move    #L3Sp,r5                ; pointer to the SP local save
    move    #adcRestoreSysStackRtn,r6 ; setup the return address
    jmp     RestoreSystemStack      ; restore the hardware stack
adcRestoreSysStackRtn
    move    #adcRestoreRegReturn,r6 ; setup the return address
    jmp     RestoreRegs             ; restore registers
adcRestoreRegReturn
    move    y:IntL3_SaveR6,r6       ; restore minimal reg set
    move    y:IntL3_SaveM7,m7
    move    y:IntL3_SaveR7,r7
    rti





;///////////////////////////////////////////////////////////////////////////////
;  Handle softare-detected ADC source buffer overrun
;  (Note: Hardware-detected overrun (TXEXC) is ignored.)
;///////////////////////////////////////////////////////////////////////////////

adc_software_overrun

    ; inc the software overflow counter
    clr     a
    move    y:AdcSoftOfCount,a0
    inc     a
    move    a0,y:AdcSoftOfCount
    
    ; set the buffer to empty
    jsr     AdcFlushOneSrcBlk
    jmp     reset_word_count

    




;///////////////////////////////////////////////////////////////////////////////
;   AdcFilledSpace = GetFilledSpace(AdcBuf)
;   if (AdcFilledSpace < AdcBlockSize)      // This shouldn't normally be true if we need to flush
;       AdcTailPtr = AdcHeadPtr
;   else {
;       AdcTailPtr += AdcBlockSize
;       if (AdcTailPtr >= (AdcBufBaseAddr + AdcBufSize))
;           AdcTailPtr -= AdcBufSize
;   }
;
;///////////////////////////////////////////////////////////////////////////////

AdcFlushOneSrcBlk

    move    y:AdcSourceBufDesc,r0       ; AdcCurrentFilledSpace = GetFilledSpace(AdcBuf)
    move    #$ffff,m0
    jsr     GetFilledSpace

    move    #>AdcBlockSize,x0           ;   if (AdcFilledSpace < AdcBlockSize)
    cmp     x0,a
    jgt     more_than_a_block_filled
    move    y:AdcBufHeadPtr,a           ;       AdcTailPtr = AdcHeadPtr
    move    #>$fffffe,x0                ; (Make sure it's even (channel zero))
    and     x0,a
    move    a,y:AdcBufTailPtr
    rts

more_than_a_block_filled                ;   else {
        move    y:AdcBufTailPtr,a       ;       AdcTailPtr += AdcBlockSize
        add     x0,a

        move    y:AdcBufAddrPtr,b       ;       if (AdcTailPtr > (AdcBufBaseAddr + AdcBufSize))
        move    y:AdcBufSizePtr,x1
        add     x1,b   

        cmp     b,a
        jlt     _ok
            sub     x1,a                ;           AdcTailPtr -= AdcBufSize
_ok

    move    a,y:AdcBufTailPtr           ;   }
    rts


;///////////////////////////////////////////////////////////////////////////////
;  adc_tail_overrun
;
;   The DMA has made the Head Ptr has caught up with (and possibly passed) the 
;   Tail Ptr.
;
;   Add 
;   
;///////////////////////////////////////////////////////////////////////////////

adc_tail_overrun

    move    y:AdcBufHeadPtr,a
    move    #>AdcBlockSize,x0
    add     x0,a

        move    y:AdcBufAddrPtr,b       ;       if (AdcTailPtr > (AdcBufBaseAddr + AdcBufSize))
        move    y:AdcBufSizePtr,x1
        add     x1,b

        cmp     b,a
        jlt     _ok
            sub     x1,a                ;           AdcTailPtr -= AdcBufSize
                                    
_ok                                     ;   }
                        
    move    #>$fffffe,x0                ;   (make sure it's even)
    and     x0,a 

    move    a,y:AdcBufTailPtr
    rts



;******************************************************************************
;       AdcDriverProcess
;
;******************************************************************************
AdcDriverProcess
    rts




;******************************************************************************
;     ProcessSingleChannel

 
    ;    Passed:
    ;    x:ADCProcessSampleCount
    ;    r1 = x:AdcFlt2StatePtr
    ;    r2 = x:AdcFlt3StatePtr
    ;    y:AdcBufTailPtrTemp
    ;    EncoderBufHeadPtrTemp  
    ;    All modulo regs
    ;    Return Values : r0, r1,r2, y:AdcBufTailPtrTemp,EncoderBufHeadPtrTemp 

;******************************************************************************

ProcessSingleChannel
    push   ssh
    move    x:ADCProcessSampleCount,x0



    move    y:EncoderBufHeadPtrTemp,r0
    move    y:AdcEncoderBufModPtr,m0
    move    y:Fg_wEncNumberOfChannels,n0

    move    y:AdcBufTailPtrTemp,r4
    move    y:AdcBufModPtr,m4
    move    #2,n4                       ; Work on alternate Channels

            move    #>AdcFlt2Coeffs,r5
        move    #>AdcFlt3Coeffs,r6

    do      x0,end_filter_loop
        do #2,end_second_filter
            ; Get 4 samples
            move    x:(r4)+n4,A
            move    x:(r4)+n4,A     A,y:(r1)+
            move    x:(r4)+n4,A     A,y:(r1)+
            move    x:(r4)+n4,A     A,y:(r1)+
            move    A,y:(r1)+       

            ; Now, filter scaled samples in state vars
            clr     a   x:(r5)+,x0  y:(r1)+,y0
            rep     #AdcFlt2NTaps-1
                mac x0,y0,a     x:(r5)+,x0      y:(r1)+,y0  
            macr    x0,y0,a     
            move    a,x:(r2)+
end_second_filter
        clr     a   x:(r2)+,x0  y:(r6)+,y0
        rep     #AdcFlt3NTaps-1
            mac x0,y0,a     x:(r2)+,x0  y:(r6)+,y0
        macr    x0,y0,a     
; This version scales at the end of the filtering
        move    a,x0
        mpy     x0,x1,a
        move    a0,a
        sub     y1,a                        ; stmp6964  Remove DC offset (scaled)
        move    a,y:(r0)+n0                 ; put one of eight into the sink buf
end_filter_loop    
    move    r0,y:EncoderBufHeadPtrTemp      ; Save off sink ptr
    move    r4,y:AdcBufTailPtrTemp          ; Save off source ptr
    
    move y:<Const_ffffff,m0
    move y:<Const_ffffff,m4

    pop  ssh
    nop
    rts


    endsec




