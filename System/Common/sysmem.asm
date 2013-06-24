;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2003
;
;  File        : sysmem.asm
;  Description : System memory declarations
;  !!!!!!!! IMPORTANT !!!!!!!! if change memory location or add/remove variables
;  !!!!!!!!!!!!!!!!!!!!!!!!!!!    update sysmem.h and sysmem.xref

    section SYSFUN_SysMem

;///////////////////////////////////////////////////////////////////////////////
;   Macros
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "sysequ.inc"
    include "systemdefs.inc"
    include "msgequ.inc"
    include "encadpcmequ.inc"
    include "project.inc"
    list

;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

    ;Functions
    ;       None

    ;Variables in X        
    global  DecoderCSR,Fg_wDecoderCSR
    global  DecoderSR,Fg_wDecoderSR
    global  DecoderCSR2,Fg_wDecoderCSR2    ; DECODE2EOF
    global  EncoderSR,Fg_wEncoderSR
    global  EncoderCSR,Fg_wEncoderCSR
    global  Fg_wDecAdpcmFlags,DecAdpcmFlags
    global  RecStartTimeHigh,Fg_wRecStartTimeHigh 
    global  RecStartTimeLow,Fg_wRecStartTimeLow  
    global  SysError,Fg_wSysError
    global  UserScratchX,Fg_wUserScratchX
    global  NextVoiceNumValue,Fg_wNextVoiceNumValue
  if (@def('TRACEBUF_EN'))
    global  TraceBuffer,Fg_wTraceBuffer        ; Not enabled by default 
    global  TracePointer,Fg_wTracePointer       
  endif
    global  EncAdpcmSave_r7,Fg_wEncAdpcmSave_r7
    global  EncAdpcmSave_m7,Fg_wEncAdpcmSave_m7   
    global  EncModuleState,Fg_wEncModuleState

    ;Variables in Y        
    global  Fg_VolumeBias,g_VolumeBias 
	global	Fg_wBitRate,g_wBitRate
	global	Fg_wSamplingRate,g_wSamplingRate
	global	Fg_wTargetCheckOK,g_wTargetCheckOK
	global	g_bAudibleNeedFileSize,Fg_bAudibleNeedFileSize
	global	g_bAudiblePlayThrough,Fg_bAudiblePlayThrough
	global	g_bAudibleConsumedBytesHigh,Fg_bAudibleConsumedBytesHigh
	global	g_bAudibleConsumedBytesLow,Fg_bAudibleConsumedBytesLow
	global	g_bAudibleCurrentChapterNum,Fg_bAudibleCurrentChapterNum
	global	g_bAudibleTotalChapterCount,Fg_bAudibleTotalChapterCount
	global	SongStartOffsetHigh,Fg_wSongStartOffsetHigh
	global	SongStartOffsetLow,Fg_wSongStartOffsetLow
    global  SongByteTotalHigh,Fg_wSongByteTotalHigh
    global  SongByteTotalLow,Fg_wSongByteTotalLow 
    ; TIME_FIX
    global  SongByteTotalConsumedHigh,Fg_wSongByteTotalConsumedHigh
    global  SongByteTotalConsumedLow,Fg_wSongByteTotalConsumedLow 
    global  SongByteLengthHigh,Fg_wSongByteLengthHigh
    global  SongByteLengthLow,Fg_wSongByteLengthLow 
    global  InvSampleRate,Fg_wInvSampleRate
    global  CurrentSongBad,Fg_wCurrentSongBad
	global  Fg_iSongType
    global  CurrentDirDevId,Fg_wCurrentDirDevId
    global  SongInvBitRatePtr,Fg_wSongInvBitRatePtr
    global  SystemFileHandle,Fg_wSystemFileHandle
    global  CurrentRoutinePtr,Fg_wCurrentRoutinePtr
    global  UserScratchYDescriptor,Fg_wUserScratchYDescriptor
    global  UserScratchY,Fg_wUserScratchY
    global  UserScratchXDescriptor,Fg_wUserScratchXDescriptor
    global  EncFileNameString,Fg_EncFileNameString
    global  EncFileHandle,Fg_wEncFileHandle,MP3EncFileHandle,FileHandle,AdpcmEncFileHandle
    global  AdpcmDecoderFileHandle,Mp3DecoderFileHandle,AudibleMp3DecoderFileHandle,WmaDecoderFileHandle,FWmaDecoderFileHandle,WmaDecoderFileHandle2,FWmaDecode
    global  EncAdpcmOvlSave_sp,Fg_wEncAdpcmOvlSave_sp
    global  RootDirectory,Fg_wRootDirectory
    global  SecondstoSkip,Fg_wSecondsToSkip,Fg_wAdpcmSecondstoSkip,AdpcmSecondstoSkip
    global  g_Mp3TimerCount,g_AudibleMp3TimerCount    
    global  PrevInvBitRatePtr,Fg_wPrevInvBitRatePtr
    ; NEW_TIME - For VBR
    global  AccumulatedMSecs,Fg_wAccumulatedMSecs
    global  AccumulatedSecs,Fg_wAccumulatedSecs
    global  DeltaTimeStampMsec,Fg_wDeltaTimeStampMsec 
    global  VBRSignature,Fg_wVBRSignature
    GLOBAL  FMp3DecoderStartFrameFilePosition, FAudibleMp3DecoderStartFrameFilePosition

    ; This variable was originally from decodermem.asm 
    GLOBAL  AudibleMp3DecoderEventValue, Mp3DecoderEventValue, AdpcmDecoderEventValue, WmaDecoderEventValue
	
    GLOBAL Fg_wEncCurrentDeviceNum,EncCurrentDeviceNum    
    GLOBAL Fg_wEncADCGain,EncAdcGain
    GLOBAL Fg_wEncFormatTag,g_wEncFormatTag			
    GLOBAL Fg_wEncNumberOfChannels,g_wEncNumberOfChannels	
    GLOBAL Fg_wEncSamplingRate,g_wEncSamplingRate		
    GLOBAL Fg_wEncPCMBits,g_wEncPCMBits          
    GLOBAL Fg_wEncADCSRR,EncADCSRR              
    GLOBAL Fg_wEncADCGain,EncAdcGain
    GLOBAL Fg_wEncBitRate,g_wEncBitRate  
    GLOBAL FEncoderWritesWaiting,EncoderWritesWaiting
    GLOBAL  gEncoderWriteStatus 
    global  PCMEncSavePC
    global  PCMEncSaveSR
    global  EncoderFileWriteIsrSp

    GLOBAL  Fg_iEncTotalSeconds
    GLOBAL  Fg_iEncCurrentSeconds
    global  Fg_wEncoderMode,g_wEncoderMode
	global  JanusDRMLicExpired,FJanusDRMLicExpired,FLicenseDeleted,LicenseDeleted

    if @def('USE_PLAYLIST5') 	
	global bDRMLicExpired,FbDRMLicExpired
	endif  ;@def('USE_PLAYLIST5')

    ; stmp6964
    GLOBAL Fg_wADCLChOffset,ADCLChOffset
    GLOBAL Fg_wADCRChOffset,ADCRChOffset
    GLOBAL Fg_wEncADCLChOffset,EncAdcLChOffset
    GLOBAL Fg_wEncADCRChOffset,EncAdcRChOffset
    GLOBAL Fg_wDACGain,DacGain
    GLOBAL Fg_wSamplingRateIndex,g_wSamplingRateIndex	
    GLOBAL Fg_wEncSamplingRateIdx,g_wEncSamplingRateIdx	
      
    if (!(@def('DEVICE_3400')||@def('DEVICE_3300')))
        ; add wma
    global  SongInvBitRate      ; ** TEMP** until wma updated like mp3 
    endif   
;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "bufdesc.xref"
    include "sysmisc.xref"
    list

;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////
        
;///////////////////////////////////////////////////////////////////////////////
;   X Memory
;///////////////////////////////////////////////////////////////////////////////
        
    org     x,"SYSFUN_SysMem_X":

Fg_wDecoderCSR
DecoderCSR          dc      ($000000)|(1<<Stop)

Fg_wDecoderSR
DecoderSR           dc      ($000000)|(1<<Stop)

; DECODE2EOF
Fg_wDecoderCSR2
DecoderCSR2         dc      ($000000)

Fg_wEncoderSR
EncoderSR           dc      ($000000)|(1<<Stop)

Fg_wDecAdpcmFlags
DecAdpcmFlags       dc      0

Fg_wRecStartTimeHigh
RecStartTimeHigh    dc      0

Fg_wRecStartTimeLow
RecStartTimeLow     dc      0

Fg_wSysError
SysError            dc      0              ; store system error here before jmp to SystemError

  if @def('AUDIBLE')
; Added for AA file navigation, specifically to handle
; crossing song boundaries during Ffwd or Rew.
Fg_bAudibleNeedFileSize
g_bAudibleNeedFileSize			dc		0		; 0=Don't need to read file size
												; 1=Read file size and set vars for FFwd
												; 2=Read file size and set vars for Rew
Fg_wTargetCheckOK
g_wTargetCheckOK				dc		0

; Added for file position play-through indicator. Also added
; separate song start offset until we can confirm that
; SongByteTotalConsumedHigh/Low can be used.
Fg_bAudiblePlayThrough
g_bAudiblePlayThrough			dc		0		; 0=not played through
												; 1=played through
												; All other values reserved

; Specifies current chapter # in a audible book
Fg_bAudibleCurrentChapterNum
g_bAudibleCurrentChapterNum		dc		0	

; Specifies total # of chapters in a audible book
Fg_bAudibleTotalChapterCount
g_bAudibleTotalChapterCount		dc		0	

; This variable keeps track total decoder consumed bytes 
g_bAudibleConsumedBytesHigh
Fg_bAudibleConsumedBytesHigh	dc		0
g_bAudibleConsumedBytesLow
Fg_bAudibleConsumedBytesLow		dc		0

Fg_wSongStartOffsetHigh
SongStartOffsetHigh				dc		0
Fg_wSongStartOffsetLow
SongStartOffsetLow				dc		0
  endif
	
    org     x,"SYSFUN_SysMem_2_X":

    buffer  M,SCRATCH_USER_X_SIZE
Fg_wUserScratchX
UserScratchX
    bsc     SCRATCH_USER_X_SIZE,$dec0de
    endbuf

  if (@def('TRACEBUF_EN'))
  ; Note: TraceBuffer NOT allocated by default. 
  ; To enable: Uncomment define TRACEBUF_EN in player.mk 
  ; Used by systrace.asm's trace macro which writes a word arg to this circ buffer     
    org     x,"SYSFUN_SysMem_3_X":   

    buffer  M,TRACE_BUFF_SIZE              
Fg_wTraceBuffer
TraceBuffer
    bsc     TRACE_BUFF_SIZE,$dec0de
    endbuf
  endif 

    org     x,"SYSFUN_SysMem_4_X":

  if (@def('TRACEBUF_EN'))
Fg_wTracePointer
TracePointer            dc TraceBuffer
  endif

Fg_wNextVoiceNumValue
NextVoiceNumValue       dc      0
Fg_wEncAdpcmSave_r7
EncAdpcmSave_r7         dc      $0      ; Akshay's code was written
Fg_wEncAdpcmSave_m7
EncAdpcmSave_m7         dc      $0      ;  before we implemented the
                                        ;  stack in r7.

Fg_wEncModuleState
EncModuleState          dc      0
Fg_wEncoderCSR
EncoderCSR              dc      0



;///////////////////////////////////////////////////////////////////////////////
;   Y Memory
;///////////////////////////////////////////////////////////////////////////////
        
    org     y,"SYSFUN_SysMem_Y":

; This variable was moved from decodermem.asm since it was conflicting with 
; Encoder memory usage
WmaDecoderEventValue
AdpcmDecoderEventValue
Mp3DecoderEventValue 
AudibleMp3DecoderEventValue dc      EVENT_MESSAGE       ; message to return to executive


    if (!@def(SECONDS_TO_SKIP))
; this is due to the many builds that include sysmem but not decoderequ.inc !!!!!
SECONDS_TO_SKIP                 equ     1     ; ff/rw skip
    endif
 
Fg_wAdpcmSecondstoSkip
AdpcmSecondstoSkip        dc      SECONDS_TO_SKIP

Fg_wSecondsToSkip
SecondstoSkip           dc      SECONDS_TO_SKIP
   
Fg_VolumeBias
g_VolumeBias            dc      0

g_AudibleMp3TimerCount
g_Mp3TimerCount         dc      7500              ; timer count value is loaded here

Fg_wCurrentRoutinePtr
CurrentRoutinePtr       dc      $ffffff

Fg_wSystemFileHandle
SystemFileHandle        dc      1

FAdpcmDecoderFileHandle
FMp3DecoderFileHandle
AudibleMp3DecoderFileHandle
Mp3DecoderFileHandle;    dc      0          ; the handle assigned by FileOpen //;added
FWmaDecoderFileHandle
WmaDecoderFileHandle;    dc      0          ;the handle assigned by FileOpen //;added
AdpcmDecoderFileHandle    dc      0          ; the handle assigned by FileOpen//;added

WmaDecoderFileHandle2;    dc      0          ;the handle assigned by FileOpen //;added
FWmaDecoderFileHandle2    dc      0          ;the handle assigned by FileOpen //;added

  if @def('AUDIBLE')
; Try keeping global bitrate and sampling rate vars
Fg_wBitRate
g_wBitRate						dc		0

Fg_wSamplingRate
g_wSamplingRate					dc		0
  endif

; This counts song bytes read from mp3 file into our Mp3DecSrcBuf
Fg_wSongByteTotalHigh
SongByteTotalHigh       dc      0       ; high word of total bytes decoded (fread) this song
Fg_wSongByteTotalLow
SongByteTotalLow        dc      0       ; low word of total bytes decoded (fread) this song

; Counts bytes sent to MP3 decoder. TIME_FIX Resolves low bitrate encoding issues. 
; Needed for current-time calculations which are displayed and determines song end.
Fg_wSongByteTotalConsumedHigh
SongByteTotalConsumedHigh       dc      0  ; high word of total bytes decoded this song.
Fg_wSongByteTotalConsumedLow
SongByteTotalConsumedLow        dc      0  ; low word of total bytes decoded this song

Fg_wSongByteLengthHigh
SongByteLengthHigh      dc      0       ; the number of bytes in the entire song (MSBs)
Fg_wSongByteLengthLow
SongByteLengthLow       dc      0       ; the number of bytes in the entire song (LSBs)
Fg_wInvSampleRate
InvSampleRate        dc      0          ; inverse of the adpcm sample rate
; GDA_20010530
Fg_wCurrentSongBad
CurrentSongBad       dc      0          ; Non-zero if current song was found to be corrupted, zero-length, etc.
                                        ;  If set, code should avoid trying to read the file.
Fg_iSongType		dc		0			; For curreny song type								
										   
Fg_wSongInvBitRatePtr
SongInvBitRatePtr       dc      0       ; ptr to inv bitrate location when song is opened

FJanusDRMLicExpired
JanusDRMLicExpired      dc      0

FLicenseDeleted
LicenseDeleted      dc      0

    if @def('USE_PLAYLIST5') 
FbDRMLicExpired
bDRMLicExpired      dc      0
    endif   ; @def('USE_PLAYLIST5')

    if (!(@def('DEVICE_3400')||@def('DEVICE_3300')))
        ; add wma -- stores inverse of bitrate here for wma
SongInvBitRate          dc      0       ; ** TEMP** until wma updated like mp3 
    endif   

Fg_wPrevInvBitRatePtr
PrevInvBitRatePtr       dc      0       ; ptr to save the previous value of the SongInvBitRatePtr


Fg_wCurrentDirDevId
CurrentDirDevId      dc      0          ; device id of the directory (card #)

Fg_wAccumulatedMSecs      
AccumulatedMSecs        dc      0          ; NEW_TIME - VBR Play Start Time.

Fg_wAccumulatedSecs      
AccumulatedSecs         dc      0          ; NEW_TIME - VBR Playing Time.

Fg_wDeltaTimeStampMsec      
DeltaTimeStampMsec      dc      0           ; VBR TIME - keep track of the last time stamp (msecs).

Fg_wVBRSignature
VBRSignature            dc      0           ;NEW_TIME - VBR flag - non-zero if VBR.

Fg_wEncoderMode
g_wEncoderMode          dc      0

FMp3DecoderStartFrameFilePosition 
FAudibleMp3DecoderStartFrameFilePosition ds 2


;///////////////////////////////////////////////////////////////////////////////
; Scratch area stuff

Fg_wUserScratchXDescriptor
UserScratchXDescriptor  dc      UserScratchX
                        dc      SCRATCH_USER_X_MODULO
                        dc      SCRATCH_USER_X_SIZE
                        dc      UserScratchX
                        dc      UserScratchX
Fg_wUserScratchYDescriptor
UserScratchYDescriptor  dc      UserScratchY
                        dc      SCRATCH_USER_Y_MODULO
                        dc      SCRATCH_USER_Y_SIZE
                        dc      UserScratchY
                        dc      UserScratchY

; Static encoder variables
; file name to encode to is 10 words (30 bytes) long.
Fg_EncFileNameString
EncFileNameString       dc      0,0,0,0,0,0,0,0,0,0
                        

Fg_wEncFileHandle
FAdpcmEncFileHandle;added
AdpcmEncFileHandle
FileHandle
MP3EncFileHandle;               dc      0
EncFileHandle           dc      0

Fg_wEncAdpcmOvlSave_sp
EncAdpcmOvlSave_sp      dc      $0                                                    

Fg_wRootDirectory
RootDirectory           dc      DirectorySeparator

    org     y,"SYSFUN_SysMem_mod_Y":
    buffer  M,SCRATCH_USER_Y_SIZE
Fg_wUserScratchY
UserScratchY
    bsc     SCRATCH_USER_Y_SIZE,$dec0de
    endbuf

     org     y,"ENCAPP_EncMem_Static":

Fg_wEncCurrentDeviceNum
EncCurrentDeviceNum     dc      0

Fg_wEncADCGain
EncAdcGain                      DC          DEFAULT_ADPCM_ADCGAIN

Fg_wEncADCSRR
EncADCSRR                       DC          DEFAULT_ADPCM_ADCSRR

; The Menu will set these values, so that the Encoder knows what to do.
Fg_wEncFormatTag
g_wEncFormatTag			        dc		   $2		; FormatTag = $11 for IMA ADPCM, 2 = MSADPCM,1=PCM
Fg_wEncNumberOfChannels
g_wEncNumberOfChannels	                dc		   1
Fg_wEncSamplingRate
g_wEncSamplingRate		        dc		   16000
Fg_wEncPCMBits
g_wEncPCMBits                           dc                 16        ; Number of bits for PCM recording
Fg_wEncBitRate
g_wEncBitRate                           dc                 128000

FEncoderWritesWaiting
EncoderWritesWaiting        dc      0   ; Count of buffers waiting to be written. 
gEncoderWriteStatus         dc      0   ; WriteToDisk semaphore.
EncoderFileWriteIsrSp:      dc      0

PCMEncSavePC:               ds      1
PCMEncSaveSR:               ds      1

Fg_iEncTotalSeconds    dc  0
Fg_iEncCurrentSeconds  dc  0


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; stmp6964

Fg_wSamplingRateIndex           
g_wSamplingRateIndex            ds  0

Fg_wDACGain
DacGain                         DC          1

Fg_wEncSamplingRateIdx       
g_wEncSamplingRateIdx           DC 0


Fg_wEncADCLChOffset             ; Left Channel (1)
EncAdcLChOffset                 DC 0

Fg_wEncADCRChOffset             ; Right Channel (1)
EncAdcRChOffset                 DC 0

;Fg_wADCLChOffset                ; Left Channel
;AdcLChOffset                    DC          500,1333,11000,6941,0,5100,0
;AdcLChOffset                    DC          500,1333,3000,5941,0,4100,0
;AdcLChOffset                    DC          0,0,0,0,0,0,0
;AdcLChOffset                    DC          1000,1333,4000,4555,-2424,3000,14000
;AdcLChOffset                    DC          1000,1333,4000,12000,12400,10000,14000

;Fg_wADCRChOffset                ; Right Channel
;AdcRChOffset                    DC          9000,7600,12250,7250,0,3600,0
;AdcRChOffset                    DC          9000,7600,12250,7250,0,3600,0
;AdcRChOffset                    DC          0,0,0,0,0,0,0
;AdcRChOffset                    DC         9000,8600,10250,17500,20000,18500,14000
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;///////////////////////////////////////////////////////////////////////////////
;   P Memory
;///////////////////////////////////////////////////////////////////////////////

    endsec
