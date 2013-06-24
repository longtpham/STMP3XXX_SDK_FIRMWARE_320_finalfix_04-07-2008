;******************************************************************************
; Copyright(C) SigmaTel, Inc. 2000-2003
;
; Filename: decodermem.asm
;
; Description: MP3 Decoder Module Memory
;******************************************************************************
        section DECMOD_DecoderMem



;*******************************************************************************
;  Include files
;*******************************************************************************
        include "dacequ.inc"
        include "hwequ.inc"
        include "sysequ.inc"
        include "msgequ.inc"
        include "mp3dec.inc"
        include "decoderequ.inc"

;*******************************************************************************
;  Extern definitions
;*******************************************************************************
       GLOBAL  Mp3ABPositionAHigh
       GLOBAL  Mp3ABPositionALow
       GLOBAL  Mp3ABPositionAConsumedHigh
       GLOBAL  Mp3ABPositionAConsumedLow
       GLOBAL  Mp3ABPositionBHigh
       GLOBAL  Mp3ABPositionBLow 
       GLOBAL  Mp3DecIsrSp
;      The following was moved to work-around corruption of encoder data
;      GLOBAL  Mp3DecoderEventValue 
       GLOBAL  Mp3DecoderFfwdRwndValue
;      GLOBAL  Mp3DecoderFileHandle
       GLOBAL  Mp3DecoderIsrSaveM7
       GLOBAL  Mp3DecoderIsrSaveR7
       GLOBAL  Mp3DecoderSaveM7
       GLOBAL  Mp3DecoderSaveR7
       GLOBAL  Mp3DecoderSinkBufDesc
       GLOBAL  Mp3DecoderSourceBufDesc
;*******************************************************************************
;  Extern references
;*******************************************************************************
        EXTERN    g_Mp3DataSrcBufDescriptor,g_Mp3DecSinkBufDescriptor
        EXTERN    DacDriverInit

;*******************************************************************************
;  X memory
;*******************************************************************************
        org     x,"DECAPP_DecoderMem_X":

Mp3DecoderSaveR7     dc      0
Mp3DecoderSaveM7     dc      0
Mp3DecoderIsrSaveR7  dc      0
Mp3DecoderIsrSaveM7  dc      0

;*******************************************************************************
;  Y memory
;*******************************************************************************
	org     y,"DECAPP_DecoderMem_Y":

Mp3DecIsrSp             dc      0     ; decoder stores stack ptr here when saving off stack

Mp3DecoderSourceBufDesc dc      g_Mp3DataSrcBufDescriptor
Mp3DecoderSinkBufDesc   dc      g_Mp3DecSinkBufDescriptor

Mp3DecoderFfwdRwndValue dc      FFWD_RWND_BYTE_SKIP ; number of bytes to skip for ffwd/rwnd

; Following variable moved to sysmem.asm to avoid its corruption 
;Mp3DecoderEventValue    dc      EVENT_MESSAGE       ; message to return to executive
	
;-----------------------------------------------------------------
; File System Locations
;-----------------------------------------------------------------
;Mp3DecoderFileHandle    dc      0          ; the handle assigned by FileOpen

; AB Mode Byte Positions
Mp3ABPositionAHigh      dc      0
Mp3ABPositionALow       dc      0
Mp3ABPositionAConsumedHigh dc   0
Mp3ABPositionAConsumedLow  dc   0
Mp3ABPositionBHigh      dc      0
Mp3ABPositionBLow       dc      0

        endsec

