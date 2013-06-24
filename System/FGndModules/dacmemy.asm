;******************************************************************************
; Copyright(C) SigmaTel, Inc. 2000
;
; STMP DAC Driver Module YMEM
;******************************************************************************
        
;*******************************************************************************
;  Include files
;*******************************************************************************
		include "hwequ.inc"
		include "dacequ.inc"
		include "stack.xref"
		include "stackmem.xref"
		include "sysequ.inc"
	    include "irqequ.inc"
	    include "sysmacro.asm"
	    include "stack.xref"
	    include "stackmem.xref"
	    include "const.xref"
		include "sysmisc.xref"
    	include "bufdesc.xref"
        
;*******************************************************************************
;  Extern definitions
;*******************************************************************************
        GLOBAL    DacData1L
        GLOBAL    DacData1R
        GLOBAL    DacData1LPtr
        GLOBAL    DacData1RPtr
        GLOBAL    DacBufDescriptor
        GLOBAL    DacBufHeadPtr
        GLOBAL    DacBufTailPtr
        GLOBAL    DacBufModPtr
        GLOBAL    DacUfBuffCount
        GLOBAL    DacUfErrCount
		GLOBAL	  DacSamplestoProcess
		GLOBAL	  TotalSamplestoProcess
		GLOBAL	  FgndLoopCnt
		GLOBAL	  DacSampFreqTable	
 if @def('TUNER_STFM1000')
        global    Fg_EncodeStfm
 endif


        EXTERN    DacBuf
;*******************************************************************************
;  X memory
;*******************************************************************************
; !!!!!!!!!!!!! All DAC xmem goes in dacmem.asm

;*******************************************************************************
;  Y memory
;*******************************************************************************
	org     y,"DacMem_ModL_Y":

        buffer  M,DacData1Size
DacData1L
        bsc     DacData1Size,0
        endbuf

	org     y,"DacMem_ModR_Y":
        buffer  M,DacData1Size
DacData1R
        bsc     DacData1Size,0
        endbuf

	org     y,"DacMem_Y":

        
DacData1LPtr             dc      DacData1L                     
DacData1RPtr             dc      DacData1R                     


;-----------
; These must stay in this order:
DacBufDescriptor         dc      DacBuf
DacBufModPtr             dc      DacBufModulo
DacBufSizePtr            dc      DacBufSize
DacBufHeadPtr            dc      DacBuf                ; DacBuf Head Ptr
DacBufTailPtr            dc      DacBuf                ; DacBufTailPtr
;-----------

DacUfErrCount            dc      0                     ; Counter for DAC underflow exceptions
DacUfBuffCount           dc      0                     ; Counter for number of silence buffers inserted (for underflows)
DacSamplestoProcess		 dc		 0					   ; To process not more than 64 samples.	
TotalSamplestoProcess	 dc		 0
FgndLoopCnt				 dc		 0

 if @def('TUNER_STFM1000')
Fg_EncodeStfm                   dc  0
 endif

; This must stay in this order. 

DacSampFreqTable        
						dc      DAC8kHz					; 8000
                        dc      DAC11_025kHz			; 11025
                        dc      DAC12kHz				; 12000
                        dc      DAC16kHz				; 16000
                        dc      DAC22_05kHz				; 22050
                        dc      DAC24kHz 				; 24000
                        dc      DAC32kHz				; 32000
                        dc      DAC44_1kHz   			; 44100
                        dc      DAC48kHz				; 48000
