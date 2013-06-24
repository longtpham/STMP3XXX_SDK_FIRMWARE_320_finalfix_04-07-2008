;******************************************************************************
; Copyright(C) SigmaTel, Inc. 2000/2001
;
; STMP Decoder Module Memory
;******************************************************************************

        section Acelp_DecoderMem



;*******************************************************************************
;  Include files
;*******************************************************************************
        include "dacequ.inc"
        include "hwequ.inc"
        include "sysequ.inc"
        include "msgequ.inc"
        include "AudibleAcelpdec.inc"
        include "decoderequ.inc"
;*******************************************************************************
;  Extern definitions
;*******************************************************************************
       GLOBAL  AcelpABPositionAHigh
       GLOBAL  AcelpABPositionALow
       GLOBAL  AcelpABPositionBHigh
       GLOBAL  AcelpABPositionBLow 
       GLOBAL  AcelpDecIsrSp
       GLOBAL  AcelpDecoderEventValue
       GLOBAL  AcelpDecoderFfwdRwndValue
       GLOBAL  AcelpDecoderFileHandle
       GLOBAL  AcelpDecoderSinkBufDesc
       GLOBAL  Acelp_SamplingRate
       GLOBAL  Acelp_BitRate
       GLOBAL  FAcelp_SamplingRate
       GLOBAL  FAcelp_BitRate
       GLOBAL  Acelp_HeaderSize,FAcelp_HeaderSize
       GLOBAL   CurrentSongName,Fg_wCurrentSongName
        GLOBAL   TotalSongsPlayed,Fg_wTotalSongsPlayed
        GLOBAL   SongTitle,Fg_wSongTitle
        GLOBAL   SongArtist,Fg_wSongArtist
        


;*******************************************************************************
;  Extern references
;*******************************************************************************
       EXTERN g_AcelpDecSinkBufDescriptor
;*******************************************************************************
;  X memory
;*******************************************************************************

;*******************************************************************************
;  Y memory
;*******************************************************************************
    org     y,"DECACELPMOD_DecoderMem_Y":

AcelpDecIsrSp             dc      0     ; decoder stores stack ptr here when saving off stack

AcelpDecoderSinkBufDesc   dc      g_AcelpDecSinkBufDescriptor

AcelpDecoderFfwdRwndValue dc      FFWD_RWND_BYTE_SKIP ; number of bytes to skip for ffwd/rwnd
AcelpDecoderEventValue    dc      EVENT_MESSAGE       ; message to return to executive
;-----------------------------------------------------------------
; Song Information. Used to know the position of song in terms of time
;-----------------------------------------------------------------
; Inverting Bitrates: (1/bitrate)*(8bits/bytes)*2^11 where 2^11 is to normalize and must be undone at end

;-----------------------------------------------------------------
; File System Locations
;-----------------------------------------------------------------
AcelpDecoderFileHandle    dc      0          ; the handle assigned by FileOpen

; AB Mode Byte Positions
AcelpABPositionAHigh      dc      0
AcelpABPositionALow       dc      0
AcelpABPositionBHigh      dc      0
AcelpABPositionBLow       dc      0


Fg_wTotalSongsPlayed
TotalSongsPlayed     dc      0          ; used by Shuffle to know when complete


; ###CHANGE
FAcelp_SamplingRate
Acelp_SamplingRate        dc      8000
FAcelp_BitRate
Acelp_BitRate             dc      16000

FAcelp_HeaderSize
Acelp_HeaderSize          dc      0
        endsec

