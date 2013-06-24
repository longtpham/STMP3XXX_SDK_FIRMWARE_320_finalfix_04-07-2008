;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000
;
; FSTMP3 Wma DecoderSystem Memory         
;///////////////////////////////////////////////////////////////////////////////
;*******************************************************************************
;  Include files
;*******************************************************************************

    include "sysequ.inc"
    include "hwequ.inc"
    include "Wmadec.inc"
           
;*******************************************************************************

    section WmaWrapperMem

;*******************************************************************************
;  Extern definitions
;*******************************************************************************
        ; These need to remain GLOBAL in Motorola


;*******************************************************************************
;  Extern references
;*******************************************************************************

    include "Wmadec.xref"
    include "sysmisc.xref"
    include "sysmem.xref"


;*******************************************************************************
;  X memory
;*******************************************************************************

;*******************************************************************************
;  Y memory
;*******************************************************************************


; buffer descriptors

; processing routine descriptors

        endsec

;*******************************************************************************
;*******************************************************************************
;*******************************************************************************
;*******************************************************************************

    	section WmaDecoderMem

;*******************************************************************************
;  Extern definitions
;*******************************************************************************
        GLOBAL   g_pWmaDecSinkBufMod
        GLOBAL   g_WmaDecSink_mbuf
        GLOBAL   g_WmaDecSinkBufDescriptor,g_pWmaDecSinkBufSize
        GLOBAL   g_pWmaDecSinkBufHead,g_pWmaDecSinkBufTail
	GLOBAL	HuffTableSizeBase
	GLOBAL g_pLeft            
	GLOBAL Fg_pLeft
	GLOBAL mode_type          
	GLOBAL WMA_RECON_MAX_SAMPLES
    GLOBAL xdummy1,xdummy2,xdummy4,xdummy5

WMA_RECON_MAX_SAMPLES	equ			128

;*******************************************************************************
;  X memory
;*******************************************************************************
        
        
		org     x,"WmaDecoderMem_mod_X":

        buffer  M,g_WmaDecSinkBufSize         
g_WmaDecSink_mbuf       
        bsc     g_WmaDecSinkBufSize,0
        endbuf


	org 	x,"WmaDecoderMem_X":
Fg_pLeft
g_pLeft			ds			WMA_RECON_MAX_SAMPLES*2
mode_type       ds			1

	org 	x,".xdummy1":
xdummy1     ds   1
	org 	x,".xdummy2":
xdummy2     ds   1    
	org 	x,".xdummy3":
xdummy3     ds   1    
	org 	x,".xdummy4":
xdummy4     ds   1    
	org 	x,".xdummy5":
xdummy5     ds   1  
    
;*******************************************************************************
;  Y memory
;*******************************************************************************


		org     y,"WmaDecoderMem_Y":
; No Source Descriptors. Non streaming model adopted, to save memory.

;Warning: The DecSinkBuf locations below must stay together in this order!
g_WmaDecSinkBufDescriptor  dc      g_WmaDecSink_mbuf
g_pWmaDecSinkBufMod        dc      g_WmaDecSinkBufModulo
g_pWmaDecSinkBufSize       dc      g_WmaDecSinkBufSize
g_pWmaDecSinkBufHead       dc      g_WmaDecSink_mbuf ; DecSinkBufHeadPtr
g_pWmaDecSinkBufTail       dc      g_WmaDecSink_mbuf ; DecSinkBufTailPtr

HuffTableSizeBase    
			dc    	7044		; Huff44QB
			dc    	14547		; Huff44OB
			dc    	5160		; Huff16Ob	


; HuffTableSizeBase    
; 			dc    	4698		; Huff44QB
; 			dc    	9699		; Huff44OB
; 			dc    	3447		; Huff16Ob	

    endsec
