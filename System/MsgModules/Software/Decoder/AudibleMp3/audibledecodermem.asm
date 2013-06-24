;******************************************************************************
; Copyright(C) SigmaTel, Inc. 2000-2003
;
; Filename: decodermem.asm
;
; Description: Audible MP3 Decoder Module Memory
;******************************************************************************
        section DECMOD_AudibleDecoderMem



;*******************************************************************************
;  Include files
;*******************************************************************************
        include "dacequ.inc"
        include "hwequ.inc"
        include "sysequ.inc"
        include "msgequ.inc"
        include "audiblemp3dec.inc"
        include "decoderequ.inc"

;*******************************************************************************
;  Extern definitions
;*******************************************************************************
       GLOBAL  AudibleMp3ABPositionAHigh
       GLOBAL  AudibleMp3ABPositionALow
       GLOBAL  AudibleMp3ABPositionAConsumedHigh
       GLOBAL  AudibleMp3ABPositionAConsumedLow
       GLOBAL  AudibleMp3ABPositionBHigh
       GLOBAL  AudibleMp3ABPositionBLow 
       GLOBAL  AudibleMp3DecIsrSp
;      The following was moved to work-around corruption of encoder data
;      GLOBAL  AudibleMp3DecoderEventValue 
       GLOBAL  AudibleMp3DecoderFfwdRwndValue
;      GLOBAL  AudibleMp3DecoderFileHandle
       GLOBAL  AudibleMp3DecoderIsrSaveM7
       GLOBAL  AudibleMp3DecoderIsrSaveR7
       GLOBAL  AudibleMp3DecoderSaveM7
       GLOBAL  AudibleMp3DecoderSaveR7
       GLOBAL  AudibleMp3DecoderSinkBufDesc
       GLOBAL  AudibleMp3DecoderSourceBufDesc
;*******************************************************************************
;  Extern references
;*******************************************************************************
        EXTERN    g_AudibleMp3DataSrcBufDescriptor,g_AudibleMp3DecSinkBufDescriptor
        EXTERN    DacDriverInit

;*******************************************************************************
;  X memory
;*******************************************************************************
        org     x,"DECAPP_AudibleDecoderMem_X":

AudibleMp3DecoderSaveR7     dc      0
AudibleMp3DecoderSaveM7     dc      0
AudibleMp3DecoderIsrSaveR7  dc      0
AudibleMp3DecoderIsrSaveM7  dc      0

;*******************************************************************************
;  Y memory
;*******************************************************************************
	org     y,"DECAPP_AudibleDecoderMem_Y":

AudibleMp3DecIsrSp             dc      0     ; decoder stores stack ptr here when saving off stack

AudibleMp3DecoderSourceBufDesc dc      g_AudibleMp3DataSrcBufDescriptor
AudibleMp3DecoderSinkBufDesc   dc      g_AudibleMp3DecSinkBufDescriptor

AudibleMp3DecoderFfwdRwndValue dc      FFWD_RWND_BYTE_SKIP ; number of bytes to skip for ffwd/rwnd

; Following variable moved to sysmem.asm to avoid its corruption 
;AudibleMp3DecoderEventValue    dc      EVENT_MESSAGE       ; message to return to executive
	
;-----------------------------------------------------------------
; File System Locations
;-----------------------------------------------------------------
;Mp3DecoderFileHandle    dc      0          ; the handle assigned by FileOpen

; AB Mode Byte Positions
AudibleMp3ABPositionAHigh      dc      0
AudibleMp3ABPositionALow       dc      0
AudibleMp3ABPositionAConsumedHigh dc   0
AudibleMp3ABPositionAConsumedLow  dc   0
AudibleMp3ABPositionBHigh      dc      0
AudibleMp3ABPositionBLow       dc      0

        endsec

