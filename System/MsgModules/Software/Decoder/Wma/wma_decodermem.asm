;******************************************************************************
; Copyright(C) SigmaTel, Inc. 2000/2001
;
; STMP Decoder Module Memory
;******************************************************************************

        section Wma_DecoderMem



;*******************************************************************************
;  Include files
;*******************************************************************************
        include "dacequ.inc"
        include "hwequ.inc"
        include "sysequ.inc"
        include "msgequ.inc"
        include "Wmadec.inc"
        include "decoderequ.inc"
;*******************************************************************************
;  Extern definitions
;*******************************************************************************
       GLOBAL  WmaABPositionAHigh
       GLOBAL  WmaABPositionALow
       GLOBAL  WmaABPositionBHigh
       GLOBAL  WmaABPositionBLow 
       GLOBAL  WmaDecIsrSp
;       GLOBAL  WmaDecoderEventValue
       GLOBAL  WmaDecoderFfwdRwndValue
;       GLOBAL  WmaDecoderFileHandle
       GLOBAL  WmaDecoderIsrSaveM7
       GLOBAL  WmaDecoderIsrSaveR7
       GLOBAL  WmaDecoderSaveM7
       GLOBAL  WmaDecoderSaveR7
       GLOBAL  WMAReconstructedFlag
       GLOBAL  WmaDecoderSinkBufDesc
       GLOBAL  WmaInitFlag
       GLOBAL   HasJanusDRMLICFail
;*******************************************************************************
;  Extern references
;*******************************************************************************
       EXTERN g_WmaDecSinkBufDescriptor
;*******************************************************************************
;  X memory
;*******************************************************************************
        org     x,"DECWMAMOD_DecoderMem_X":

WmaDecoderSaveR7     	dc      	0
WmaDecoderSaveM7     	dc      	0
WmaDecoderIsrSaveR7  	dc      	0
WmaDecoderIsrSaveM7  	dc      	0
WMAReconstructedFlag 	dc		0


;*******************************************************************************
;  Y memory
;*******************************************************************************
	org     y,"DECWMAMOD_DecoderMem_Y":

WmaInitFlag		dc	0
WmaDecIsrSp             dc      0     ; decoder stores stack ptr here when saving off stack
    if @def('USE_PLAYLIST5')       
FHasJanusDRMLICFail 
    endif   ;if @def('USE_PLAYLIST5')       
HasJanusDRMLICFail     dc   0 ;indicate that license is faild for this track. 



WmaDecoderSinkBufDesc   dc      g_WmaDecSinkBufDescriptor

WmaDecoderFfwdRwndValue dc      FFWD_RWND_BYTE_SKIP ; number of bytes to skip for ffwd/rwnd
;WmaDecoderEventValue    dc      EVENT_MESSAGE       ; message to return to executive
;-----------------------------------------------------------------
; Song Information. Used to know the position of song in terms of time
;-----------------------------------------------------------------
; Inverting Bitrates: (1/bitrate)*(8bits/bytes)*2^11 where 2^11 is to normalize and must be undone at end

;-----------------------------------------------------------------
; File System Locations
;-----------------------------------------------------------------
;WmaDecoderFileHandle    dc      0          ; the handle assigned by FileOpen

; AB Mode Byte Positions
WmaABPositionAHigh      dc      0
WmaABPositionALow       dc      0
WmaABPositionBHigh      dc      0
WmaABPositionBLow       dc      0


    
        endsec

