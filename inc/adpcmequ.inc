;;***********************************************************************
;SigmaTel, Inc
;$Archive: /Audio/ADPCM/stmp3400/h/defs.h $
;$Revision: 4 $
;$Date: 8/07/00 6:33p $
;Description: 
;Notes:  Local Definitions
;***********************************************************************/

BLOCK_SIZE                 equ     `512

BD_MessageCmd              equ     8
BD_MessageMode             equ     9
BD_SamplesSkip             equ     10
BD_ReturnCode              equ     11
BD_ReturnValue             equ     12


    if ("BLD"=="SMNISR")
CmdIn                      equ     $8000
SimIn                      equ     $8001
SimOut                     equ     $8002
wavoutput                  equ     $8003
  endif


ADPCMBUFSIZE               equ     `2048


WAVE_FORMAT_DVI_ADPCM      equ     $0011  ; IMA ADPCM
WAVE_FORMAT_PCM           	equ		$01
WAVE_FORMAT_ADPCM			equ	 	$02
WAVE_FORMAT_IMA_ADPCM		equ	 	$11
WAVE_FORMAT_G721_ADPCM		equ	 	$40


PLAY_CMD                   equ     $1
STOP_CMD                   equ     $2
REW_CMD                    equ     $3
FFD_CMD                    equ     $4
REC_CMD                    equ     $5



STOPPED_STATE              equ     $0
PLAY_STATE                 equ     $1
RECORD_STATE               equ     $2

RECORD_MODE                equ     $0
PLAYER_MODE                equ     $1




; Return Codes        
RETURN_OK                  equ     $0
REW_SAMPLES                equ     $1
FWD_SAMPLES                equ     $2
ERROR_READING_WAVE_HEADER  equ     $3



; Error Values  
;NO_ERROR                   equ     $0
NO_ERRORS_FOUND            equ     $0
RIFF_UNPACK_ERROR          equ     $1
WAVE_UNPACK_ERROR          equ     $2
FMT_UNPACK_ERROR           equ     $3
UNKNOWN_CODEC_ERROR        equ     $4
FACT_UNPACK_ERROR          equ     $5
DATA_UNPACK_ERROR          equ     $6


;Sec2Min                      equ     $022222 ; 1/60
;Min2Sec                      equ     60
