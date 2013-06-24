;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000/2001
;
; STMP3400 ISR-level Decoder Module (CANNOT be overlayed)
;///////////////////////////////////////////////////////////////////////////////
        section DECAPP_DecStfmNonOverlay


;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////
       ; For i2s_dma_receive.c
       GLOBAL   g_I2SDMAReceiveIsr
       GLOBAL   StfmDecoderRun

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////
        ; System Functions
       EXTERN   SysAllLinear
       EXTERN   SysClearTimerIrq
       EXTERN   SysDisableTimer
       EXTERN   GetFilledSpace
       EXTERN   GetEmptySpace

        ; Decoder Module
       EXTERN   DecoderCSR
       EXTERN   DecoderSR
       EXTERN   StfmRxIsrSp
       EXTERN   DecoderTimerNum


        EXTERN  Fg_Wave_data_offset
       EXTERN   SongByteLengthHigh
       EXTERN   SongByteLengthLow


        ; Parser Module
       EXTERN   CurrentSongBad          ; GDA_20010530

        ; File System
       EXTERN   FILEClose
       EXTERN   FILEOpen
       EXTERN   FileEOF
       EXTERN   FileRead

        ; STFM variables
        extern StfmDecSaveR6
        extern StfmDecSavePC
        extern StfmDecSaveSR

        extern StfmDecSinkEmptySpace
        extern Fg_I2SDMAReceiveClearInt 
        extern g_pStfmDataSrcBufMod
        extern StfmDecSinkFilledSpace
        extern StfmDecSinkOverrunCount
        extern g_pStfmDataSrcBufHead
        extern StfmDecoderRunIsrSp
        extern g_StfmDataSrcBufDescriptor
        extern StfmLowPassFilter
        extern g_pStfmDataSrcBufTail
        extern Fg_sd_audio_LprB1_i16
        extern Fg_sd_audio_LprB0_i16
        extern Fg_sd_audio_LprA1_i16
        extern Fg_sd_audio_LprA2_i16
        extern FIIRFiltCoeff
        extern FIIRFiltCoeffSemaphore
                
;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////
        nolist
        include "sysequ.inc"
        include "hwequ.inc"
        include "dacequ.inc"
        include "msgequ.inc"
;        include "stfmequ.inc"
        include "dec_stfmequ.inc"
        include "irqequ.inc"
        include "stack.xref"
        include "stackmem.xref"
        include "const.xref"
		include "wave.xref"
		include "stfm.xref"
        list
        page    132,60,0,0,0

;///////////////////////////////////////////////////////////////////////////////
;  Macros
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "sysmacro.asm"
    include "systrace.asm"
    list


;///////////////////////////////////////////////////////////////////////////////
;   Code
;///////////////////////////////////////////////////////////////////////////////
        org     p,"DECAPP_DecStfmNonOverlay_P":

;--------------------------------------------------

; I2S DMA completion ISR
g_I2SDMAReceiveIsr

        ; Clear the DM bit (double-precision multiply mode, bit 6) of the Mode Register
        ; This will be returned to its previous state when the RTI occurs (since the SR
        ; will be popped back off System Stack Low (SSL).
        ; Accumulator behavior is unpredictable when the DM bit is set.
        ; Next cycle after clearing it should NOT be an ALU instruction.
        andi #$BF,mr

    move    x0,y:IntL1_SaveR6           ; save x0 state

_CheckSpurious
    ; Check if spirous interrupt. If so exit ...
    move    x:HW_ICLSTATUS1R,x0
    jset    #HW_ICLSTATUS1R_SST31_BITPOS,x0,_ContinueIsr

    ; None of the above, exit interruption
    move    y:IntL1_SaveR6,x0           ; restore x0 state
    rti

_ContinueIsr
;    bset    #HW_GP_B15_BITPOS,x:HW_GP1DOR
    move    y:IntL1_SaveR6,x0           ; restore x0 state
    
    ; save off all registers
    move    r6,y:IntL1_SaveR6            ; r6 used for return mechanism
    move    #_StfmDecSaveRegReturn,r6    ; setup the return address
    jmp     SaveRegs                     ; save the remaining registers
_StfmDecSaveRegReturn

    ; save off stack
    move    #_StfmDecSaveStackReturn,r6  ; setup the return address
    move    #StfmRxIsrSp,r5                 ; pointer to the SP local save
    jmp     SaveSystemStack              ; save the hardware stack
_StfmDecSaveStackReturn


        jsr     SysAllLinear

        ; FIXME!!! This should be handled here in assembly to assure that we
        ; have a new I2S buffer and not an I2S overflow condition
        jsr     Fg_I2SDMAReceiveClearInt 

    
;----------------------- EXIT
_ExitIsr
    ; restore stack
    move    #StfmRxIsrSp,r5                 ; pointer to the SP local save
    move    #_stackRestored,r6           ; setup the return address
    jmp     RestoreSystemStack           ; restore the hardware stack
_stackRestored

    ; restore all registers
    move    #_StfmDecRestoreRegReturn,r6  ; setup the return address
    jmp     RestoreRegs                  ; restore registers
_StfmDecRestoreRegReturn
    move    y:IntL1_SaveR6,r6            ; restore minimal reg set

;    debug
    nop
    
    rti                     ;Int level is dropped to 0 (user level)

;--------------------- Post ISR processing (aka "bottom half")
StfmDecoderRun
        ;Here the Interruption level is dropped automatically to 0 (user level)
        ; Clear the DM bit (double-precision multiply mode, bit 6) of the Mode Register
        ; This will be returned to its previous state when the RTI occurs (since the SR
        ; will be popped back off System Stack Low (SSL).
        ; Accumulator behavior is unpredictable when the DM bit is set.
        ; Next cycle after clearing it should NOT be an ALU instruction.        
    andi #$BF,mr
    
;    debug
    nop
    nop
    nop

;        debug
    ; save off all registers
    move    r6,y:StfmDecSaveR6         ; r6 used for return mechanism
    move    #_StfmSaveRegReturn,r6  ; setup the return address
    jmp     SaveRegs                        ; save the remaining registers
_StfmSaveRegReturn

    ; save off stack
    move    #_StfmSaveStackReturn,r6 ; setup the return address
    move    #StfmDecoderRunIsrSp,r5       ; pointer to the SP local save
    jmp     SaveSystemStack                 ; save the hardware stack
_StfmSaveStackReturn

        ;bset    #HW_GP_B16_BITPOS,x:HW_GP0DOR
        ; Look at the I2S current position register to find the new
        ; source buffer head
        move    y:g_StfmDataSrcBufDescriptor,b1
        move    x:HW_CDSYNCCPR,x0
        add     x0,b
        move    b1,y:g_pStfmDataSrcBufHead

        move    #g_StfmDataSrcBufDescriptor,r0
        move    y:<Const_ffffff,m0
        clr     a
        jsr     GetFilledSpace          ; returns: a==number of words in buffer
        asr     a       ; Number of stereo samples 

        ; Save the number of words to give to the filter in B
        move    a,b

        ; Get empty space from sink buffer
		jsr		SysAllLinear
        move    #g_StfmDecSinkBufDescriptor,r4
        jsr     GetEmptySpace

        move    a,y:StfmDecSinkEmptySpace
        asr     a   ; Number of stereo samples

        ; Check if we're overrunning the sink buffer
        move    a,x0
        cmp     x0,b ; GetEmptySpace return space available in A
        jlt     _EnoughOutputSpace
        
        ; Record the buffer overrun
        move    y:StfmDecSinkOverrunCount,a
        move    y:<Const_000001,x0
        add     x0,a
        move    a1,y:StfmDecSinkOverrunCount

_EnoughOutputSpace
; !!!!!!!!!!!!!! STFM FILTER FUNCTION CALL - SEE TASKING DOCS FOR CALLING CONVENTIONS
        push    b
        move    b,a     ; Setup size of buffer to process
        move    y:g_pStfmDataSrcBufMod,m0   ; Setup modulo
        move    y:g_pStfmDataSrcBufTail,r0  ; Setup source buffer pointer
        move    y:g_pStfmDecSinkBufMod,m4  ; Setup current output pointer within modulo buffer
        move    y:g_pStfmDecSinkBufHead,r4  ; Setup current output pointer within modulo buffer

        move    y:<Const_000000,a0
        tst     a
        jeq     _SkipFilterCall
        ; Call the filter function to process the input buffer
 if @def('MIN_FILTER')
        jsr     FStfmLowPassFilter ; This function does not save registers because it overwrites I2S input buffers
 else
        ;jsr     FUpdate_IIRFiltCoeff ; this version in sw_stereocontrol.c will clobber reg A.  
        jsr     UpdateIIRFiltCoeffs ; using the asm version will leave A, m0, r0, and r4 as is.  
        jsr     StfmLowPassFilter ; This function does not save registers because it overwrites I2S input buffers
 endif
_SkipFilterCall
; !!!!!!!!!!!!!! END OF FILTER FUNCTION CALL

;        bclr    #HW_GP_B17_BITPOS,x:HW_GP0DOR    ; make it low 

        pop     b   ; Pop back out the # samples filtered

        ; Update the source buffer tail
        move    r0,y:g_pStfmDataSrcBufTail

        ; Update the sink buffer head
        move    r4,y:g_pStfmDecSinkBufHead

        move    #g_StfmDecSinkBufDescriptor,r0
        move    y:<Const_ffffff,m0
        jsr     GetFilledSpace          ; returns: a==number of words in buffer
        move    a,y1
        move    a,y:StfmDecSinkFilledSpace

_ContinueStuff
    ;bclr #HW_GP_B16_BITPOS,x:HW_GP0DOR    ; make it low 

;-------------------- EXIT
_ExitIsr
    ;Raise Int level to 1 so we can update the BUSY flag without being
    ;interrupted by the Decoder Isr

    bset    #8,sr                       ; Raise interrupt level to 1
    nop                                 ; Those nops make sure that 
    nop                                 ; any interrupt pending is processed
    nop                                 ; DO NOT REMOVE, UNLESS YOU HAVE 
                                        ; A FREE DAY TO DEBUG CODE                                                    
                              
;    bclr    #HW_GP_B15_BITPOS,x:HW_GP1DOR    ; make it low 

    ; restore stack
    move    #StfmDecoderRunIsrSp,r5         ; pointer to the SP local save
    move    #_StfmStackRestored,r6   ; setup the return address
    jmp     RestoreSystemStack              ; restore the hardware stack
_StfmStackRestored

    move    #_StfmRestoreRegReturn,r6 ; setup the return address
    jmp     RestoreRegs                     ; restore registers
_StfmRestoreRegReturn
    move    y:StfmDecSaveR6,r6          ; restore minimal reg set
    movec   y:StfmDecSavePC,ssh              ;Force return 
    movec   y:StfmDecSaveSR,ssl
;    debug
    nop
    rti                            ;This needs to be a RTI to restore the SR

 if !(@def('MIN_FILTER'))
; Function will update the IIR coefficients if IIRFiltCoeffSemaphore is set.
; Asm version was written so register A would be saved.  
UpdateIIRFiltCoeffs
    move    y:FIIRFiltCoeffSemaphore,b
    tst     b
    jeq     _FiltSemaphoreCleared

    move    y:Fg_sd_audio_LprB0_i16,y0
    move    y0,x:FIIRFiltCoeff
    move    y:Fg_sd_audio_LprB1_i16,y1
    move    y1,x:FIIRFiltCoeff+1
    move    y0,x:FIIRFiltCoeff+2
    move    y:Fg_sd_audio_LprA1_i16,b
    move    b,x:FIIRFiltCoeff+3
    move    y:Fg_sd_audio_LprA2_i16,x1
    move    x1,x:FIIRFiltCoeff+4
    move    y0,x:FIIRFiltCoeff+5
    move    y1,x:FIIRFiltCoeff+6
    move    y0,x:FIIRFiltCoeff+7
    move    b,x:FIIRFiltCoeff+8
    move    x1,x:FIIRFiltCoeff+9

    move    #0,y1
    move    y1,y:FIIRFiltCoeffSemaphore

_FiltSemaphoreCleared
    rts
 endif
    endsec
