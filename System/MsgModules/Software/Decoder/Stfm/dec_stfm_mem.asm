;******************************************************************************
; Copyright(C) SigmaTel, Inc. 2000/2001
;
; STMP Decoder Module Memory
;******************************************************************************
    section DECSTFMMOD_DecStfmMem


;*******************************************************************************
;  Include files
;*******************************************************************************
        include "dacequ.inc"
        include "hwequ.inc"
        include "sysequ.inc"
        include "msgequ.inc"
        include "dec_stfmequ.inc"
        include "project.inc"


;*******************************************************************************
;  Extern definitions
;*******************************************************************************

       ; For i2s_dma_receive.c
       GLOBAL   Fg_I2SDMATargetMBuffer
       GLOBAL   g_I2SDMATargetMBuffer

       GLOBAL   g_pStfmDecSinkBufMod
       GLOBAL   g_StfmDataSrc_mbuf
       GLOBAL   g_StfmDecSink_mbuf
       GLOBAL   g_StfmDecSinkBufDescriptor,g_pStfmDecSinkBufSize
       GLOBAL   g_pStfmDecSinkBufHead,g_pStfmDecSinkBufTail
       GLOBAL   g_StfmDataSrcBufDescriptor,g_pStfmDataSrcBufMod,g_pStfmDataSrcBufSize
       GLOBAL   g_pStfmDataSrcBufHead,g_pStfmDataSrcBufTail
       GLOBAL   StfmDecoderSaveR7
       GLOBAL   StfmDecoderSaveM7
       GLOBAL   StfmDecoderSourceBufDesc
       GLOBAL   StfmDecoderSinkBufDesc
	   GLOBAL   StfmInitFlag
       GLOBAL	StfmDecoderPointer
       GLOBAL   Fg_I2SDMATargetMBufferSize   ; For i2s_dma_receive.c
       GLOBAL   g_I2SDMATargetMBufferSize   ; For i2s_dma_receive.c

        GLOBAL  StfmRxIsrSp
        GLOBAL  StfmDecSaveR6
        GLOBAL  StfmDecSavePC
        GLOBAL  StfmDecSaveSR
        GLOBAL  StfmDecSinkEmptySpace
        GLOBAL  StfmDecSinkFilledSpace
        GLOBAL  StfmDecSinkOverrunCount
        GLOBAL  StfmDecRunIsrSp
        GLOBAL  StfmDecoderRunIsrSp
        GLOBAL  StfmDecoderEventValue
        global  g_StfmDacMonitorDescriptor
        global  g_pStfmDacMonitorMod
        global  g_pStfmDacMonitorSize
        global  g_pStfmDacMonitorHead
        global  g_pStfmDacMonitorTail

;*******************************************************************************
;  Extern references
;*******************************************************************************
;*******************************************************************************
;  X memory
;*******************************************************************************
      org   x,"DECAPP_DecStfmMem_mod_X":
        buffer  M,g_StfmDataSrcBufSize
g_I2SDMATargetMBuffer
g_StfmDataSrc_mbuf
        ds     g_StfmDataSrcBufSize
        endbuf

;*******************************************************************************
;  Y memory
;*******************************************************************************
      org   y,"DECAPP_DecStfmMem_mod_Y":

        buffer  M,g_StfmDecSinkBufSize
g_StfmDecSink_mbuf
        ds     g_StfmDecSinkBufSize
        endbuf

      org   y,"DECAPP_DecStfmMem_Y":

;Warning: The g_StfmDataSrc_mbuf locations below must stay together in this order!
g_StfmDataSrcBufDescriptor  dc      g_StfmDataSrc_mbuf
g_pStfmDataSrcBufMod        dc      g_StfmDataSrcBufModulo
Fg_I2SDMATargetMBufferSize   ; For i2s_dma_receive.c
g_I2SDMATargetMBufferSize   ; For i2s_dma_receive.c
g_pStfmDataSrcBufSize       dc      g_StfmDataSrcBufSize
g_pStfmDataSrcBufHead       dc      g_StfmDataSrc_mbuf
g_pStfmDataSrcBufTail       dc      g_StfmDataSrc_mbuf

;Warning: The g_StfmDecSink_mbuf locations below must stay together in this order!
g_StfmDecSinkBufDescriptor  dc      g_StfmDecSink_mbuf
g_pStfmDecSinkBufMod        dc      g_StfmDecSinkBufModulo
g_pStfmDecSinkBufSize       dc      g_StfmDecSinkBufSize
g_pStfmDecSinkBufHead       dc      g_StfmDecSink_mbuf
g_pStfmDecSinkBufTail       dc      g_StfmDecSink_mbuf

;Warning: The descriptor locations below must stay together in this order!
g_StfmDacMonitorDescriptor  dc      g_StfmDataSrc_mbuf
g_pStfmDacMonitorMod        dc      g_StfmDataSrcBufModulo
g_pStfmDacMonitorSize       dc      g_StfmDataSrcBufSize
g_pStfmDacMonitorHead       dc      g_StfmDataSrc_mbuf
g_pStfmDacMonitorTail       dc      g_StfmDataSrc_mbuf

StfmDecoderEventValue   dc      0
StfmDecoderRunIsrSp     dc      0
StfmRxIsrSp             dc      0     ; decoder stores stack ptr here when saving off stack
StfmDecSaveR6           dc      0 
StfmDecSavePC           dc      0 
StfmDecSaveSR           dc      0 
StfmDecSinkEmptySpace   dc      0 
StfmDecSinkFilledSpace   dc      0 
StfmDecSinkOverrunCount  dc      0 

StfmDecoderSourceBufDesc dc      g_StfmDataSrcBufDescriptor
StfmDecoderSinkBufDesc   dc      g_StfmDecSinkBufDescriptor

StfmInitFlag			  dc	  0
StfmDecoderPointer       dc      0

        endsec

