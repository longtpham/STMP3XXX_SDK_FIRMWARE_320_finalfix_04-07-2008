;******************************************************************************
; Copyright(C) SigmaTel, Inc. 2000/2001
;
; STMP DAC Driver Module
;******************************************************************************
; Module Files:
;
;******************************************************************************

;*******************************************************************************
;  Include files
;*******************************************************************************
    	include "fgndmodules.inc"

        page    132,60,0,0,0

;*******************************************************************************
;  Extern definitions
;*******************************************************************************

;*******************************************************************************
;  Global definitions
;*******************************************************************************
		global EffectsDescriptor
;*******************************************************************************
;  X memory
;*******************************************************************************

		org	x,"FgndMemoryX":
        buffer  M,FgndWorkBufSize
FgndWorkBuffer
        bsc     FgndWorkBufSize,0
        endbuf


;*******************************************************************************
;  X memory
;*******************************************************************************
		org	y,"FgndMemoryY":
; These must stay in this order:
EffectsDescriptor		      dc      FgndWorkBuffer
FgndWorkBufModPtr             dc      FgndWorkBufSize-1
FgndWorkBufSizePtr            dc      FgndWorkBufSize
FgndWorkBufHeadPtr            dc      FgndWorkBuffer                ; FgndWorkBuf Head Ptr
FgndWorkBufTailPtr            dc      FgndWorkBuffer                ; FgndWorkBufTailPtr
