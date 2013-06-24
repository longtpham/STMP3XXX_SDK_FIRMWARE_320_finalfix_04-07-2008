;******************************************************************************
; Copyright(C) SigmaTel, Inc. 2000/2001
;
; STMP Encoder MP3 Module Memory
;******************************************************************************

    section ENCMP3MOD_EncMp3Mem


;*******************************************************************************
;  Include files
;*******************************************************************************

    ;include "adcequ.inc" 
    include "hwequ.inc"
    include "sysequ.inc"
    include "msgequ.inc"
    include "encmp3equ.inc"
    include "mp3_adcequ.inc"

;*******************************************************************************
;  Extern definitions
;*******************************************************************************

        extern MP3EncoderSourceBufDesc
    GLOBAL  FMp3_Inbuf
	GLOBAL	MP3EncoderSinkBuf

	GLOBAL	Mp3DataOffset
	GLOBAL	PreviousMp3Buffer
	GLOBAL	Mp3EncClusterTimeScale

	GLOBAL	MP3EncIsrSp
	GLOBAL	MP3EncSignalReturn
	GLOBAL	MP3EncoderSampleCountHigh
	GLOBAL	MP3EncoderSampleCountLow
	GLOBAL	MP3EncBytesWrittenHigh
	GLOBAL	MP3EncBytesWrittenLow
	GLOBAL	MP3EncWriteFailCount
       ;	GLOBAL	MP3EncFileHandle
	GLOBAL	MP3EncIsrFlags

;*******************************************************************************
;  Extern references
;*******************************************************************************


     org     x,"ENCAPP_EncMp3Mem_X":

Mp3DataOffset		dc	0
PreviousMp3Buffer	dc	0

Mp3EncClusterTimeScale	dc	0
			dc	3419278
			dc	2761724
			dc	2301437
			dc	1961880
			dc	1726077
			dc	1375571
			dc	1147042
			dc	980940
			dc	860969
			dc	687785
			dc	572606
			dc	490470
			dc	429969
			dc	343892
			dc	0


;*******************************************************************************
;  Y memory
;*******************************************************************************
     org     y,"ENCAPP_EncMp3Mem_Y":

MP3EncIsrSp                    dc      0

MP3EncSignalReturn             dc      0

MP3EncoderSampleCountHigh      dc      0
MP3EncoderSampleCountLow       dc      0
MP3EncBytesWrittenHigh         dc      0
MP3EncBytesWrittenLow          dc      0

MP3EncWriteFailCount           dc      0

;MP3EncFileHandle               dc      0
MP3EncIsrFlags                 dc      0
EncMP3Temp                dc      $0
EncMP3Temp1               dc      $0

	
    endsec




