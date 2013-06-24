;******************************************************************************
; Copyright(C) SigmaTel, Inc. 2000/2001
;
; STMP DAC Driver Module
;******************************************************************************
; Module Files:
;   \common\dac\dacdrv.asm
;   \common\dac\dacmem.asm
;   \inc\dacequ.asm
;   \inc\dacdef.asm
;
;******************************************************************************

;*******************************************************************************
;  Include files
;*******************************************************************************
		include "hwequ.inc"
		include "dacequ.inc"
		include "stack.xref"
		include "stackmem.xref"
		include "sysequ.inc"
		include "dacmem.xref"
        include "dacmemy.xref"
	    include "irqequ.inc"
	    include "sysmacro.asm"
	    include "stack.xref"
	    include "stackmem.xref"
	    include "const.xref"
		include "sysmisc.xref"
    	include "bufdesc.xref"

        page    132,60,0,0,0

;*******************************************************************************
;  Extern definitions
;*******************************************************************************
        GLOBAL    DacDriverIsr

;*******************************************************************************
;  Extern references
;*******************************************************************************
    	EXTERN    DacBufHeadPtr
    	EXTERN    DacBufModPtr
    	EXTERN    DacBufTailPtr
    	EXTERN    DacUfBuffCount
    	EXTERN    DacUfErrCount
    	EXTERN    DacSinkBufDesc
        EXTERN	  ExecFgndModules
        EXTERN    DecoderCSR
        EXTERN    CopyBufSourceDesc
        EXTERN    EncoderSourceBufDesc
 if @def('TUNER_STFM1000') 
        EXTERN    Fg_EncodeStfm
        EXTERN    Fg_DACsrrCorrection
        EXTERN    g_pStfmDacMonitorHead
        EXTERN    g_StfmDacMonitorDescriptor
        EXTERN    g_pStfmDacMonitorSize
        EXTERN    g_pStfmDacMonitorMod
        EXTERN    g_pStfmDacMonitorTail
        EXTERN    g_I2SDMATargetMBufferSize
        EXTERN    RunStfmFilter
        EXTERN    StfmDecSaveSR
        EXTERN    StfmDecSavePC
        EXTERN    StfmDecoderRun
        EXTERN    DacIsrSaveIPR
  endif
;*******************************************************************************
;  Code start
;*******************************************************************************


;******************************************************************************
; Psuedo code for DacDriverIsr as of 10/27/2005
;
; if( sizeof DacBuf < 64 )
;   Repeat last 64 samples to avoid underflow interrupt
;   Increment DacUfErrCount
;   Exit Isr
; else if( Number of empty words in DacBuf > Number of samples avail in CopyBufSource )
;   TotalSamplestoProcess = Number of samples avail in CopyBufSource
;   Run ExecFgndModules
; else if( Number of empty words in DacBuf > 0 )
;   TotalSamplestoProcess = Number of empty words in DacBuf
;   Run ExecFgndModules
; else if( Number of empty words in DacBuf == 0 )
;   Exit Isr
;   
;******************************************************************************
;       DacDriverIsr
;
;       This is the ISR that is vectored in the vector table.  It must
;       return with an rti since it will be called at interrupt time.  It
;       will use the DacDriverProcess routine to complete its functionality.
;******************************************************************************
		org p,".dacdriver":
DacDriverIsr

;        trace   #>$BB0000

        ; Clear the DM bit (double-precision multiply mode, bit 6) of the Mode Register
        ; This will be returned to its previous state when the RTI occurs (since the SR
        ; will be popped back off System Stack Low (SSL).
        ; Accumulator behavior is unpredictable when the DM bit is set.
        ; Next cycle after clearing it should NOT be an ALU instruction.
        andi #$BF,mr

        if ("MIPS"=="2")
            bset    #GP1_20,x:GP1DataOut
        endif
        ; assumptions
        ; 1) we will filter all available source data or destination space, whichever is greater
        ; 2) we are processing well ahead of where the next block that
        ;    the DAC port will use
        ; 3) we will set the next word count interrupt to a constant to keep interrupt rates
        ;    constant
        ; 4) the location we are dumping data into is far ahead of where we
        ;    are programming the DAC port for because the sample output rate is
        ;    faster than the rate we can finish this ISR because of the prefiltering

        move    x0,x:DacIsrSavex0       ; save x0 state

        ; Check if spirous interrupt. If so exit ...
        move    x:HW_ICLSTATUS0R,x0
        jset    #HW_ICLSTATUS0R_SST0_BITPOS,x0,_resetDacCount    ;Check DAC Refill
        jset    #HW_ICLSTATUS0R_SST1_BITPOS,x0,_resetDacCount    ;Check DAC Underflow
        ; None of the above, exit interruption
        move    x:DacIsrSavex0,x0                            ; restore x0 state
        rti

_resetDacCount
        move    #>DacBlockSize,x0       ; get the Dac buffer block size
        move    x0,x:HW_DACWCR          ; update the Dac port word count register
        move    x:DacIsrSavex0,x0

_saveRegisters
        move    r7,y:IntL3_SaveR7       ; save minimal reg set to local
        move    m7,y:IntL3_SaveM7
        move    r6,y:IntL3_SaveR6
        move    #>L3DataStack,r7        ; setup the L2 data stack
        move    y:<Const_ffffff,m7
        move    #_dacSaveRegReturn,r6   ; setup the return address
        jmp     SaveRegs                ; save the remaining registers
_dacSaveRegReturn
        move    #_dacSaveSysStackRtn,r6 ; setup the return address
        move    #L3Sp,r5                ; pointer to the SP local save
        jmp     SaveSystemStack         ; save the hardware stack
_dacSaveSysStackRtn
        move    r7,x:DacIsrSaver7       ; needed because r7 is used below
        ; update tail ptr
        clr     b
        move    y:DacBufDescriptor,b1   ; buffer descriptor holds base address
        move    x:HW_DACCPR,x1
        add     x1,b
        move    b1,y:DacBufTailPtr      ; update Dac Sink Buffer tail ptr

        ; check if underflow occurred
        move    x:HW_DACCSR,x0
        jclr    #HW_DACCSR_TXEXC_BITPOS,x0,_noUnderflow
        bclr    #HW_DACCSR_TXEXC_BITPOS,x0
        move    x0,x:HW_DACCSR          ; clear the exception for next time
_Underflow
        ; for underflow, load the previous stereo sample from the "tail" and then update headptr
        move    y:DacBufTailPtr,r0
        move    y:DacBufModPtr,m0
        move    #>DacBlockSize,x0
        move    (r0)-                   ; back up the ptr to last stereo pair sent out the dac
        move    (r0)-
        move    x:(r0)+,y0              ; first stereo sample to repeat
        move    x:(r0)+,y1              ; second stereo sample to repeat
        do      x0,_endRepeatSample
        move    y0,x:(r0)+
        move    y1,x:(r0)+
_endRepeatSample

        move    r0,y:DacBufHeadPtr      ; adjust the new headptr
        move    y:<Const_ffffff,m0
        ; update underflow counter
        move    y:DacUfErrCount,a
        move    y:<Const_000001,x0
        add     x0,a
        move    a1,y:DacUfErrCount      ; update the underflow error count
;;        bclr #HW_GP_B12_BITPOS,x:HW_GP1DOR  ;;; DEBUG ONLY 0
;;        bset #HW_GP_B12_BITPOS,x:HW_GP1DOR  ;;; DEBUG ONLY 1 
;;        bclr #HW_GP_B12_BITPOS,x:HW_GP1DOR  ;;; DEBUG ONLY 0
;; Debug code to slow down DAC sample rate if underrun occurs, this is very fine tuning
;; There is corresponding code in I2s_dma_receive.c to speed up DAC sample rate if I2S data overrun occurs
;; This code only handles the underrun condition


 if @def('TUNER_STFM1000') 
;;;        move    x:HW_DACSRR,a       ; get current DAC sample rate
;;;        move    y:Fg_DACsrrCorrection,x0  ;Get correction  (corresponding speedup on overrun in i2s_dma_receive.c)
;;;        add     x0,a                ; add to DAC sample rate to slow it down 
;;;        move    a1,x:HW_DACSRR      ; update DAC sample rate into DacSrr
 endif
        
;; end debug code        
        jmp     _exitDacIsr

_noUnderflow
        ; Get number of samples left in Sink Buffer (DacBuf)
        move    y:DacSinkBufDesc,r0
        move    y:<Const_ffffff,m0
        jsr     GetFilledSpace          ; returns: a==number of words in buffer


;        jmp     _getProcessCount    ;; NEW Remove code that looks for pending underflow condition 
        
;;; BEGIN Remove code that looks for pending underflow condition       stfm1000ta2 has just as many underflows with or without this code HJ found. stfm1000tb2 had none (SDK3.120).
        
; comment ~
        ; see if approaching underflow
        move    #>DacBlockSize,x0       ; get the Dac buffer block size
        cmp     x0,a
        jge     _getProcessCount        ; if >=, no underflow in sight

_loadZeros                              ; else, avoid underflow by writing zeros
        ; not enough words in DacBuf, so load with DacBlockSize*2 zeros
;;        bclr #HW_GP_B12_BITPOS,x:HW_GP1DOR  ;;; DEBUG ONLY
;;;         bset #HW_GP_B12_BITPOS,x:HW_GP1DOR  ;;; DEBUG ONLY
;;;         bclr #HW_GP_B12_BITPOS,x:HW_GP1DOR  ;;; DEBUG ONLY
;;;         bset #HW_GP_B12_BITPOS,x:HW_GP1DOR  ;;; DEBUG ONLY
;;;         bclr #HW_GP_B12_BITPOS,x:HW_GP1DOR  ;;; DEBUG ONLY
;;;         bset #HW_GP_B12_BITPOS,x:HW_GP1DOR  ;;; DEBUG ONLY       
;;        bclr #HW_GP_B12_BITPOS,x:HW_GP1DOR  ;;; DEBUG ONLY

;       jmp     _getProcessCount        ;; debug - skip stuffing the buffer w/fake data

;; Debug code to slow down DAC sample rate if underrun occurs, this is very fine tuning
;; There is corresponding code in I2s_dma_receive.c to speed up DAC sample rate if I2S data overrun occurs
;; This code only handles the underrun condition

;;;        move    x:HW_DACSRR,a       ; get current DAC sample rate
;;;        move    y:<Const_000001,x0  ; get +1
;;;        add     x0,a                ; add +1 to DAC sample rate to slow it down 
;;;        move    a1,x:HW_DACSRR      ; update DAC sample rate into DacSrr
        
;; end debug code

        move    y:DacBufHeadPtr,r0      ; load zeros beginning at the head
        move    y:DacBufModPtr,m0

; if @DEF('TUNER_STFM1000')
; ; Use previous values if STFM1000 build to reduce noise - pops/clicks effect
; 
;        move    (r0)-                   ; back up the ptr to last stereo pair sent out the dac
;        move    (r0)-
;        move    x:(r0)+,y0              ; first stereo sample to repeat
;        move    x:(r0)+,y1              ; second stereo sample to repeat
;        do      x0,_endLoadZeros
;        move    y0,x:(r0)+
;        move    y1,x:(r0)+ 
; else
        clr     a
        do      x0,_endLoadZeros        ; x0 = DacBlockSize, loop fills DacBlockSize*2
        move    a,x:(r0)+
        move    a,x:(r0)+
; endif
 
_endLoadZeros
        move    r0,y:DacBufHeadPtr      ; update the head ptr, leave tail as it is
        move    y:<Const_ffffff,m0               ; return it to linear

        ; update underflow counter
        move    x:DecoderCSR,x0
        jclr    #Play,x0,_exitDacIsr   ; don't update the count if not playing (i.e. FadeOut)
        move    y:DacUfBuffCount,a
        move    y:<Const_000001,x0
        add     x0,a
        move    a1,y:DacUfBuffCount     ; update the number of underflow silence buffers inserted
        jmp     _exitDacIsr
;~
;;; END Remove code that looks for pending underflow condition  

_getProcessCount

        move    y:CopyBufSourceDesc,r0	; Always check the empty space for copy buffer.
										; All other buffers are always assumed to be full.
        move    y:<Const_ffffff,m0
        jsr     GetFilledSpace          ; returns: a==number of words in buffer
        move    a,y1                    ; y1=samples available in source buffer
                                        ; y0=total size of source buffer

_chooseBufSpace
        move    y:DacSinkBufDesc,r4
        move    y:<Const_ffffff,m4
        jsr     GetEmptySpace           ; returns empty space count in A
        asr     a                       ; sink gets 2x source buffer samples due to oversample
        cmp     y1,a                    ; a - y1 = sink - source
        tgt    y1,a                    ; use source space
		; Else use Sink Space	
        move    y:<Const_000000,a0      ; clear the low portion of A so we only test a1=0
        tst     a
        jeq     _exitDacIsr

_KickStartForegroundModules
	asr 	A	    		; Make sure to process only even number of samples.
        asr     A                       ; Make sure to process even number of samples on each channel
        move    y:<Const_000000,a0      ; clear the low portion of A so we only test a1=0
	asl 	A
        asl     A
        move	A,y:TotalSamplestoProcess

  if @DEF('TUNER_STFM1000')
      ; get the room in the encode buffer
        move    y:Fg_EncodeStfm,b0
        bclr    #1,b0
		bclr    #2,b0
        jclr    #0,b0,_notStfmEncode
        bset    #2,b0				   ; set saved IPR flag
       ; save the IPR to block all level 1 and level 0 interrupt from coming in
        move    x:HW_IPR,a1
        move    a1,x:DacIsrSaveIPR
        move    #>$303f,x0    ; 0 nibble disables i2s (benign).   first 3 disables line2
		and     x0,a1
        move    a1,x:HW_IPR
       ; lower interrupt priority to 1 to allow other level 2 to come in
        bclr    #8,sr
		nop
        move    #>EncoderSourceBufDesc,r4
        move    y:<Const_ffffff,m4
        jsr     GetEmptySpace          ; returns: a==number of words free in buffer
        asr     a
        ; see if the there is enough room
        move    y:TotalSamplestoProcess,x0       ; get number of available samples
        cmp     x0,a
        jlt     _notStfmEncode
        bset    #1,b0                  ; enough room to copy the stfm audio for encoding
_notStfmEncode
        move    b0,y:Fg_EncodeStfm
  endif

        jsr     ExecFgndModules     	

  if @DEF('TUNER_STFM1000')
        ; echeck if IPR was saved
        move    y:Fg_EncodeStfm,b0
		jclr    #2,b0,_exitDacIsr
        ; raise the priority level back to 2
        bset    #>8,sr
        ; restore the IPR
        move    x:DacIsrSaveIPR,a1
        move    a1,x:HW_IPR
		; interrupt level is restored
  endif

_exitDacIsr
  if @def('TUNER_STFM1000')
_checkI2SDMA
        ; First check that the I2S DMA is enabled
        move    x:HW_CDSYNCCSR,x0
        jset    #HW_CDSYNCCSR_RESET_BITPOS,x0,_I2SDMADisabled

        ; Now check the word count
        clr     a
        move    x:HW_CDSYNCWCR,a
        tst     a
        jne     _I2SAlreadyRunning
_SyncWithI2S
        ; The word count was zero, so we're just starting up
        move    y:<Const_000000,x0
        move    x0,x:HW_CDSYNCCPR   ; Clear the CPR
        
        ; Reset the DAC monitor buffer to be zero
        move    #g_StfmDacMonitorDescriptor,r0
        move    y:<Const_ffffff,m0
        jsr     InitBufferEmpty

        jmp     _I2SStarted
_I2SAlreadyRunning

        ; Read the current position register to set the new head value
        move    y:g_StfmDacMonitorDescriptor,a
        move    x:HW_CDSYNCCPR,x0
        add     x0,a
        move    a,y:g_pStfmDacMonitorHead
        
        ; Now get the filled space in the DMA buffer
        move    #g_StfmDacMonitorDescriptor,r0
        move    y:<Const_ffffff,m0
        jsr     GetFilledSpace          ; returns: a==number of words in buffer
        move    a,x0

        move    y:g_pStfmDacMonitorSize,a
        asr     a       ; Buffer size / 2
        cmp     x0,a    ; compare words available with buffer size / 2
        jgt     _SkipFilterRun ; If words available is not up to half the buffer size, then skip the filter run
    
        ; Now update the buffer tail
        move    y:g_pStfmDacMonitorMod,m0   ; Setup modulo
        move    y:g_pStfmDacMonitorTail,r0
        move    a,n0
        nop
        nop
        move    (r0)+n0
        move    r0,y:g_pStfmDacMonitorTail

        ; Flag the filter to run
        move    y:<Const_ffffff,x0
        move    x0,x:RunStfmFilter

_SkipFilterRun
_I2SWordCountOK
_I2SStarted
        ; Keep the CDSYNC word count up
        move    y:g_I2SDMATargetMBufferSize,a
        asr     a
        move    x:HW_CDSYNCWCR,x0
        cmp     x0,a
        jlt     _SkipRefill
        move    a,x:HW_CDSYNCWCR
_SkipRefill
_I2SDMADisabled
  endif
 
        ; testpoint                      assert DacIsrSaveR7 meets expected stack addr range

        move    x:DacIsrSaver7,r7
        move    y:<Const_ffffff,m7
_restoreRegisters
        move    #L3Sp,r5                ; pointer to the SP local save
        move    #_dacRestoreSysStackRtn,r6 ; setup the return address
        jmp     RestoreSystemStack      ; restore the hardware stack
_dacRestoreSysStackRtn
		; If we have consumed too many MIPs, an exception will have occurred.
		; Clear it, for it will otherwise give rise to series of exceptions.
        move    x:HW_DACCSR,x0
        jclr    #HW_DACCSR_TXEXC_BITPOS,x0,_noForegroundUnderflow
        bclr    #HW_DACCSR_TXEXC_BITPOS,x0
        move    x0,x:HW_DACCSR          ; clear the exception for next time
        move    y:DacUfErrCount,a
        move    y:<Const_000001,x0
        add     x0,a
        move    a1,y:DacUfErrCount      ; update the underflow error count
_noForegroundUnderflow

  if @def('TUNER_STFM1000')
		clr     a
        ; check if the stfm filters are ready
        move    x:RunStfmFilter,a1
        tst     a
        jne     _runStfmFilter
  endif

        move    #_dacRestoreRegReturn,r6 ; setup the return address
        jmp     RestoreRegs             ; restore registers
_dacRestoreRegReturn

        ; testpoint                      assert IntL3_SaveR7 is in valid stack addr range

        move    y:IntL3_SaveR6,r6       ; restore minimal reg set
        move    y:IntL3_SaveM7,m7
        move    y:IntL3_SaveR7,r7

        if ("MIPS"=="2")
            bclr    #GP1_20,x:GP1DataOut
        endif

        nop
        rti

  if @def('TUNER_STFM1000')
        ; run the filter now
_runStfmFilter
        move    y:<Const_000000,x0
        move    x0,x:RunStfmFilter

        move    #_dacRestoreRegs,r6 ; setup the return address
        jmp     RestoreRegs             ; restore registers
_dacRestoreRegs

        ; testpoint                      assert IntL3_SaveR7 is in valid stack addr range

        move    y:IntL3_SaveR6,r6       ; restore minimal reg set
        move    y:IntL3_SaveM7,m7
        move    y:IntL3_SaveR7,r7

        movec   ssl,y:StfmDecSaveSR     ;Saves return address and status register
        movec   ssh,y:StfmDecSavePC     ;Call back address to use from StfmDecoderRun()
        nop
        movec   #>StfmDecoderRun,ssh   ;Forces StfmDecoderRun call after rti                          
        ;bclr    #8,ssl               ; interrupt level 2
        bset    #8,ssl
        bset    #9,ssl
        nop
        nop
        rti
  endif

