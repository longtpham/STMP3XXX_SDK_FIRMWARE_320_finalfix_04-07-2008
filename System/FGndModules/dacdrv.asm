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
		include "fgndmodules.xref"

        page    132,60,0,0,0

;*******************************************************************************
;  Extern definitions
;*******************************************************************************
        GLOBAL    DacDriverEnablePort
        GLOBAL    DacDriverInit
        GLOBAL    DacDriverProcess
        GLOBAL    DacDriverDisablePort
		GLOBAL	  DacSetSamplingRate

;*******************************************************************************
;  Extern references
;*******************************************************************************
    	EXTERN    DacBufHeadPtr
    	EXTERN    DacBufModPtr
    	EXTERN    DacBufTailPtr
    	EXTERN    DacUfBuffCount
    	EXTERN    DacUfErrCount
    	EXTERN    DacSinkBufDesc

;*******************************************************************************
;  Code start
;*******************************************************************************

        org p,".dacdriverprocess":
;******************************************************************************
;       DacDriverProcess
;
;       For this device driver, this is the routine that processes audio
;       from the source buffer to the destination buffer.  It can not use
;       an rti to return because it must be general callable, not from the
;       DAC ISR vector.  For this driver, this is the DacPrefilter routine.
;******************************************************************************
DacDriverProcess
	move	y:DacSamplestoProcess,A
    asr     a                           ; we process left, then right, half at a time
    move    a,x1                        ; save the number of iterations in x1

	; Save the buffer pointers on the stack
	push	r3
	push	r4

    ; stage 1 of the DAC prefilt
    move    #>DacCoef1,r0
    move    #>DacCoef1Taps-1,m0
    move    y:<Const_000002,n0

    move    y:DacData1LPtr,r5        	; filter memory for left channel
    move    #DacData1Size-1,m5			; Set DAC buffer modulo.

    do      x1,_loop1                   ; x1 is # of loop iterations
        move    x:(r3)+n3,x0            ; get newest sample from source buffer
        move    x0,y:(r5)               ; update local x(n) buffer with newest sample

        ;       perform 1:2 even filter
        clr     a               x:(r0)+n0,x0    y:(r5)+,y0
        rep     #DacCoef1Taps/2
        mac     x0,y0,a         x:(r0)+n0,x0    y:(r5)+,y0      ; r5 should be back where it started for 1:2 odd filter
        macr    x0,y0,a
        move                    a,x:(r4)+n4                     ; r0 should be advanced by one for 1:2 odd filter

        ;       perform 1:2 odd filter
        clr     a               x:(r0)+n0,x0    y:(r5)+,y0
        rep     #DacCoef1Taps/2-1
        mac     x0,y0,a         x:(r0)+n0,x0    y:(r5)+,y0      ; r5 should be backed up by one for next storage by input data
        macr    x0,y0,a
        move                    a,x:(r4)+n4                     ; r0 should be back to the base for next 1:2 even filter
_loop1

    move    r5,y:DacData1LPtr        ; update pointer for next round
    move    y:DacData1RPtr,r5        ; filter memory for right channel

	pop		r4
	pop		r3
    move    (r4)+                       ; advance to right channel
    move    (r3)+                       ; advance to right channel

    do      x1,_loop2                   ; x1 is # of loop iterations
        move    x:(r3)+n3,x0            ; get newest sample from source buffer
        move    x0,y:(r5)               ; update local x(n) buffer with newest sample

        ;       perform 1:2 even filter
        clr     a               x:(r0)+n0,x0    y:(r5)+,y0
        rep     #DacCoef1Taps/2
        mac     x0,y0,a         x:(r0)+n0,x0    y:(r5)+,y0      ; r5 should be back where it started for 1:2 odd filter
        macr    x0,y0,a
        move                    a,x:(r4)+n4                     ; r0 should be advanced by one for 1:2 odd filter

        ;       perform 1:2 odd filter
        clr     a               x:(r0)+n0,x0    y:(r5)+,y0
        rep     #DacCoef1Taps/2-1
        mac     x0,y0,a         x:(r0)+n0,x0    y:(r5)+,y0      ; r5 should be backed up by one for next storage by input data
        macr    x0,y0,a
        move                    a,x:(r4)+n4                     ; r0 should be back to the base for next 1:2 even filter
_loop2

    move    r5,y:DacData1RPtr        ; update pointer for next round
    ; update head and tail pointers
    move    (r3)-                       ; adjust r3 back one sample to point to left channel
    move    (r4)-                       ; adjust r4 back one sample to point to left channel
	; r3 and r4 are correctly set now.
	move	y:<Const_ffffff,m0
	move	m0,m5
    rts

        org p,".dacdriver":

;******************************************************************************
;       DacDriverEnablePort
;
;       This routine is used to enable the DAC module.
;       It is also responsible for setting the sample rate, programming
;       the DAC base address, DAC modulus, DAC current position, and the
;       number of samples available.
;
;       Arguments:
;
;******************************************************************************
DacDriverEnablePort
        ; 1. Program x:HW_DACBAR (DAC Base Address Register)
        move    y:DacBufDescriptor,x0
        move    x0,x:HW_DACBAR          ; init DAC port base addr reg with the buffer base address

        ; 2. Program x:HW_DACMR (DAC Modulo Register)
        move    y:DacBufModPtr,x0
        move    x0,x:HW_DACMR           ; init DAC port modulo register with the buffer modulo

        ; 3. Program x:HW_DACCPR (DAC Current Position Register)
        ;    CPR = offset from base address.
        move    y:DacBufTailPtr,a       ; a = tail ptr
        move    y:DacBufDescriptor,x0   ; x0 = base address
        sub     x0,a                    ; a = offset from base
        move    a,x:HW_DACCPR           ; program the current position register

        ; 4. Program x:HW_DACWCR (DAC Word Count Register)
        move    #>DacBlockSize,x0
        move    x0,x:HW_DACWCR          ; keep a constant interrupt rate

        ; 5. 
		; Don't program the DAC SRR. The DAC SRR should be set according to the sampling freq
		; as read from the file and not arbitrarily.


        rts

;******************************************************************************
;       DacDriverDisablePort
;
;       This routine is used to clear the HW_DACCSR_TXEN_BITPOS bit in the HW_DACCSR register.
;******************************************************************************
DacDriverDisablePort
        move    x:HW_DACCSR,x0
        bclr    #HW_DACCSR_TXEN_BITPOS,x0
        move    x0,x:HW_DACCSR
        rts


;******************************************************************************
;       DacDriverDisablePort
;
;       This routine sets the DAC Sampling Rate Register
; 		A = Sampling Rate Index 
;******************************************************************************
DacSetSamplingRate
	move	A,n4
    move    #DacSampFreqTable,r4        ; table of values to write to DacSrr
	nop
	; Get the DAC register value.
   	move 	y:(r4+n4),A    ; get first value
    move    A,x:HW_DACSRR              ; store samp freq to Dac
	rts

;******************************************************************************
;       DacDriverInit
;
;       Same as all other init routines.  Must set internal status to
;       conditions.
;******************************************************************************
DacDriverInit

    ; initialize the DAC sink buffer to completely full
    move    y:DacSinkBufDesc,r0
    move    y:<Const_ffffff,m0
    jsr     InitBufferFull
    ; adjust back one sample for stereo
    move    y:DacBufHeadPtr,r0
    bclr    #0,r0
    move    r0,y:DacBufHeadPtr

    ; fill the buffer with zeros
    clr     a
    move    y:<Const_ffffff,m3
    move    #>DacBuf,r3
    do      #DacBufSize,_clear_dacbuffer
        move    a,x:(r3)+
        nop
_clear_dacbuffer

    nop
    jsr     DacDriverEnablePort   ; set up the DAC control registers, enable the port
    rts

;******************************************************************************
;       DacClearMemory
;
;******************************************************************************
DacClearMemory
        clr     a       #DacData1L,r0
        move    #DacData1R,r1
        move    y:<Const_ffffff,m0
        move    y:<Const_ffffff,m1
        move    r0,y:DacData1LPtr
        move    r1,y:DacData1RPtr
        do      #DacData1Size,_clearDacMemory
        move    a,y:(r0)+
        move    a,y:(r1)+
_clearDacMemory
		rts



