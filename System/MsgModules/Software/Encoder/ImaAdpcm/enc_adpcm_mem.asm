;******************************************************************************
; Copyright(C) SigmaTel, Inc. 2000-2003
;
; STMP Encoder ADPCM Module Memory
;******************************************************************************

    section ENCADPCMMOD_EncAdpcmMem


;*******************************************************************************
;  Include files
;*******************************************************************************

    include "adcequ.inc"
    include "hwequ.inc"
    include "sysequ.inc"
    include "msgequ.inc"
    include "encadpcmequ.inc"

;*******************************************************************************
;  Extern definitions
;*******************************************************************************

   GLOBAL   EncIsrSp
    
   GLOBAL   EncIsrFlags
   GLOBAL   FEncoderSinkBuf,EncoderSinkBuf
   GLOBAL   EncoderSourceBufDesc 
   GLOBAL   EncoderBufAddrPtr    
   GLOBAL   EncoderBufModPtr     
   GLOBAL   EncoderBufSizePtr    
   GLOBAL   EncoderBufHeadPtr    
   GLOBAL   EncoderBufTailPtr    
   
   GLOBAL   EncoderSampleCountHigh    
   GLOBAL   EncoderSampleCountLow    
   GLOBAL   EncBytesWrittenHigh
   GLOBAL   EncBytesWrittenLow
;   GLOBAL   AdpcmEncFileHandle 
   GLOBAL   EncSignalReturn
   GLOBAL   AdpcmEncoderPointer
;   GLOBAL   FileHandle             
   GLOBAL   FEncoderSourceBuf,EncoderSourceBuf  
   GLOBAL   EncWriteFailCount        
    
   GLOBAL   pEncIndexTable,pEncStepSizeTable,encoded

    
    if ("ENCPCM"=="TRUE")
       GLOBAL   MonoTo16BitMonoShiftTable
       GLOBAL   MonoTo16BitMonoMaskTable
    endif   ; if ("ENCPCM"=="TRUE")
    
    if ("ENCT1"=="TRUE")
       GLOBAL   StopIt
       GLOBAL   StopCount
       GLOBAL   WriteStartTime     
       GLOBAL   ShortestWriteTime  
       GLOBAL   LongestWriteTime   
       GLOBAL   WriteTimeLogBuffer
       GLOBAL   WriteTimeLogBufferPtr    
       GLOBAL   AdpcmTimeLogBuffer
       GLOBAL   AdpcmTimeLogBufferPtr    
       GLOBAL   IsrTimeLogBuffer
       GLOBAL   IsrTimeLogBufferPtr    
       GLOBAL   IsrStartTime      
       GLOBAL   ShortestIsrTime   
       GLOBAL   LongestIsrTime    
       GLOBAL   AdpcmStartTime    
       GLOBAL   ShortestAdpcmTime 
       GLOBAL   LongestAdpcmTime  
    endif   ; if ("ENCT1"=="TRUE")


        
    GLOBAL  pEncoderSinkBufPtr0
    GLOBAL  EncoderSinkBuf0
  if 0
    GLOBAL  WriteIndex
    GLOBAL  ProcessIndex
    GLOBAL  EncoderWriteStatus0
  else
    GLOBAL  WriteSinkPointer
    GLOBAL  ProcessSinkPointer
  endif
    
    
        
;*******************************************************************************
;  Extern references
;*******************************************************************************
    
   EXTERN   AdcDriverInit
    
;*******************************************************************************
;  X memory
;*******************************************************************************
     org     x,"ENCAPP_EncAdpcmMem_mod_X":

; DecoderCSR:
; 23,22,21,20, 19,18,17,16, SongInfo(15),EndOfList(14),EndOfSong(13),Play(12),
; 11,DecSync(10),TimeMode(9),Ffwd(8), Rwnd(7),Stop(6),Pause(5),Random(4), RepeatAll(3),RepeatSong(2),PlayDir(1),PlayList(0)
;   Stop  -> set by User level when a STOP is necessary. User level then polls the Play bit to see when ISR has stopped.
;            Stop is cleared by user level when a DECODER_PLAY message is received.
;   Pause -> toggled by User level when DECODER_PLAY messages are received.
;            Pause is cleared by any STOP message or by the opening of a new Playlist or play Directory.
;   Play  -> set by ISR when it begins to play a song.
;            Play is cleared by the ISR when a Stop/Pause is issued.
;   Assumption: Only the decoder knows the Play/Pause differences.

FEncoderSinkBuf
EncoderSinkBuf      ds          ADPCM_SINK_BUF_SIZE
; tt - 1/6/04 changes for multi-threaded record.

    DUPF Num,0,(NUM_ADPCM_BUFS-1)
        FEncoderSinkBuf\Num:
        EncoderSinkBuf\Num:   ds  ADPCM_SINK_BUF_SIZE
    ENDM
    



;*******************************************************************************
;  Y memory
;*******************************************************************************




     org     y,"ENCAPP_EncAdpcmMem_mod_Y":

        buffer  M,ADPCM_SOURCE_BUF_SIZE
FEncoderSourceBuf 
EncoderSourceBuf      
        bsc     ADPCM_SOURCE_BUF_SIZE,0         ; The ADPCM input buffer
        endbuf

     org     y,"ENCAPP_EncAdpcmMem_Y":


EncoderSourceBufDesc    
EncoderBufAddrPtr           dc      EncoderSourceBuf
EncoderBufModPtr            dc      ADPCM_SOURCE_BUF_MODULO
EncoderBufSizePtr           dc      ADPCM_SOURCE_BUF_SIZE
EncoderBufHeadPtr           dc      EncoderSourceBuf
EncoderBufTailPtr           dc      EncoderSourceBuf

 if 0
WriteIndex                  dc      0   ; Index to SinkBuffer array when writing to file.
ProcessIndex                dc      0   ; Index to SinkBuffer array when processing file.
 else
WriteSinkPointer            dc      pEncoderSinkBufPtr0   ; Pointer to the current Sink Buffer to be written.
ProcessSinkPointer          dc      pEncoderSinkBufPtr0   ; Pointer to the current Process Buffer to be written.
 endif

    DUPF Num,0,(NUM_ADPCM_BUFS-1)
        FpEncoderSinkBufPtr\Num:
        pEncoderSinkBufPtr\Num:   dc  EncoderSinkBuf\Num
    ENDM
    
 if 0    
    DUPF Num,0,(NUM_ADPCM_BUFS-1)
        FEncoderWriteStatus\Num:
        EncoderWriteStatus\Num: dc  0  
    ENDM 
 endif

EncIsrSp                    dc      0

EncSignalReturn             dc      0

EncoderSampleCountHigh      dc      0
EncoderSampleCountLow       dc      0   
EncBytesWrittenHigh         dc      0
EncBytesWrittenLow          dc      0

EncWriteFailCount           dc      0

;AdpcmEncFileHandle
;FileHandle                  dc      0
EncIsrFlags                 dc      0
EncAdpcmTemp                dc      $0
EncAdpcmTemp1               dc      $0
AdpcmEncoderPointer			dc		0
    

    if ("ENCPCM"=="TRUE")
    buffer  M,2
MonoTo16BitMonoShiftTable
        dc      $008000     ; >> 8 bits
        dc      $000080     ; << 8 bits
    endbuf

    buffer  M,4
MonoTo16BitMonoMaskTable
        dc      $00FFFF
        dc      $FF0000
        dc      $0000FF
        dc      $FFFF00
    endbuf
    endif   ; if ("ENCPCM"=="TRUE")
    
    if ("ENCT1"=="TRUE")
        buffer  M,512
WriteTimeLogBuffer      
        bsc     512,0         
        endbuf
WriteTimeLogBufferPtr   dc      WriteTimeLogBuffer

        buffer  M,512
AdpcmTimeLogBuffer      
        bsc     512,0         ; The ADPCM input buffer
        endbuf
AdpcmTimeLogBufferPtr   dc      AdpcmTimeLogBuffer

        buffer  M,512
IsrTimeLogBuffer      
        bsc     512,0         ; The ADPCM input buffer
        endbuf
IsrTimeLogBufferPtr     dc      IsrTimeLogBuffer

StopIt                  dc      0
StopCount               dc      0
WriteStartTime          dc      0
ShortestWriteTime       dc      0
LongestWriteTime        dc      0
IsrStartTime            dc      0
ShortestIsrTime         dc      0
LongestIsrTime          dc      0
AdpcmStartTime          dc      0
ShortestAdpcmTime       dc      0
LongestAdpcmTime        dc      0
    endif   ; if ("ENCT1"=="TRUE")


    
    endsec




