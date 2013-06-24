;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2003
;
; STMP3 MP3 Decoder System Memory         
; Filename: mp3mem.asm
;///////////////////////////////////////////////////////////////////////////////
         
;*******************************************************************************
;  Include files
;*******************************************************************************

	include "sysequ.inc"
    include "hwequ.inc"
    include "mp3dec.inc"
           

;*******************************************************************************
;*******************************************************************************
;*******************************************************************************
;*******************************************************************************

       section Mp3WrapperMem

;*******************************************************************************
;  Extern definitions
;*******************************************************************************
        ; These need to remain GLOBAL in Motorola
        GLOBAL   Fbitbuf_addr,Fbitbuf_mod,Fbitbuf_size,Fbitbuf_writeptr 
        GLOBAL   Fbitbuf_readptr  
        GLOBAL   FPCM_addr,FPCM_mod,FPCM_size,FPCM_writeptr,FPCM_readptr

;*******************************************************************************
;  Extern references
;*******************************************************************************

    include "mp3dec.xref"
    include "sysmisc.xref"
    include "sysmem.xref"


;*******************************************************************************
;  X memory
;*******************************************************************************
		org     x,"DECAPP_Mp3WrapperMem_X":

Fbitbuf_size                    dc                0
Fbitbuf_mod                     dc                0
Fbitbuf_addr                    dc                0
Fbitbuf_readptr                 dc                0
Fbitbuf_writeptr                dc                0

FPCM_size                       dc                0
FPCM_mod                        dc                0
FPCM_addr                       dc                0
FPCM_readptr                    dc                0
FPCM_writeptr                   dc                0

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

    section Mp3DecoderMem

;*******************************************************************************
;  Extern definitions
;*******************************************************************************
        GLOBAL   g_pMp3DecSinkBufMod
        GLOBAL   g_Mp3DataSrc_mbuf
        GLOBAL   g_Mp3DecSink_mbuf
        GLOBAL   g_Mp3DecSinkBufDescriptor,g_pMp3DecSinkBufSize
        GLOBAL   g_pMp3DecSinkBufHead,g_pMp3DecSinkBufTail
        GLOBAL   g_Mp3DataSrcBufDescriptor,g_pMp3DataSrcBufMod,g_pMp3DataSrcBufSize
        GLOBAL   g_pMp3DataSrcBufHead,g_pMp3DataSrcBufTail
        GLOBAL   Mp3SampFreqTable
        GLOBAL   g_FileIsMp3
        GLOBAL   g_BaseWasResetOnSyncLoss
        GLOBAL   g_wFilePos_StartTryingToSyncHigh
        GLOBAL   g_wFilePos_StartTryingToSyncLow
     if ("BLD"=="PCMDMP")
        GLOBAL   PCM_counter
     endif

;*******************************************************************************
;  X memory
;*******************************************************************************
        
		org     x,"DECAPP_Mp3DecoderMem_X":
        
        buffer  M,g_Mp3DataSrcBufSize         
g_Mp3DataSrc_mbuf
        bsc     g_Mp3DataSrcBufSize,0
        endbuf


		org     x,"DECAPP_Mp3DecoderMem_2_X":

        ; Mp3 Sampling Frequency Table (used by Mp3DecProcess to find sampling frequency)
        ; These must be in same order as DacSampFreqTable in Y memory)
Mp3SampFreqTable        dc      8000
                        dc      11025
                        dc      12000
                        dc      16000
                        dc      22050
                        dc      24000
                        dc      32000
                        dc      44100
                        dc      48000


;*******************************************************************************
;  Y memory
;*******************************************************************************
        
		org     y,"DECAPP_Mp3DecoderMem_mod_Y":

        buffer  M,g_Mp3DecSinkBufSize         
g_Mp3DecSink_mbuf       
        bsc     g_Mp3DecSinkBufSize,0
        endbuf

		org     y,"DECAPP_Mp3DecoderMem_Y":

;Warning: The DataSrcBuf locations below must stay together in this order!
g_Mp3DataSrcBufDescriptor  dc      g_Mp3DataSrc_mbuf
g_pMp3DataSrcBufMod        dc      g_Mp3DataSrcBufModulo
g_pMp3DataSrcBufSize       dc      g_Mp3DataSrcBufSize
g_pMp3DataSrcBufHead       dc      g_Mp3DataSrc_mbuf  ; DataSrcBufHeadPtr
g_pMp3DataSrcBufTail       dc      g_Mp3DataSrc_mbuf  ; DataSrcBufTailPtr

;Warning: The DecSinkBuf locations below must stay together in this order!
g_Mp3DecSinkBufDescriptor  dc      g_Mp3DecSink_mbuf
g_pMp3DecSinkBufMod        dc      g_Mp3DecSinkBufModulo
g_pMp3DecSinkBufSize       dc      g_Mp3DecSinkBufSize
g_pMp3DecSinkBufHead       dc      g_Mp3DecSink_mbuf ; DecSinkBufHeadPtr
g_pMp3DecSinkBufTail       dc      g_Mp3DecSink_mbuf ; DecSinkBufTailPtr

g_FileIsMp3                dc      0  ; if 0, MP3 sync not found for current song. If 1, it was found at some point.
g_BaseWasResetOnSyncLoss   dc      0  ; Becomes 1 when decoder sync is lost.
g_wFilePos_StartTryingToSyncLow  dc 0 ; Only set to file read byte position when dec starts looking for sync. 
g_wFilePos_StartTryingToSyncHigh dc 0 ; Byte position is reset if sync attempts are stopped & restarted as with FF seek.
                                     
    endsec

