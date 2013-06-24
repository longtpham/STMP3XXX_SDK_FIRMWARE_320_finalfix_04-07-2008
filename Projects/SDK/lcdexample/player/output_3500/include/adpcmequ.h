//;***********************************************************************
//SigmaTel, Inc
//$Archive: /Audio/ADPCM/stmp3400/h/defs.h $
//$Revision: 4 $
//$Date: 8/07/00 6:33p $
//Description: 
//Notes:  Local Definitions
//***********************************************************************/

#define BLOCK_SIZE `512

#define BD_MessageCmd 8
#define BD_MessageMode 9
#define BD_SamplesSkip 10
#define BD_ReturnCode 11
#define BD_ReturnValue 12


#if ("BLD"=="SMNISR")
#define CmdIn 0x8000
#define SimIn 0x8001
#define SimOut 0x8002
#define wavoutput 0x8003
#endif


#define ADPCMBUFSIZE `2048


// IMA ADPCM
#define WAVE_FORMAT_DVI_ADPCM 0x0011  
#define WAVE_FORMAT_PCM 0x01
#define WAVE_FORMAT_ADPCM 0x02
#define WAVE_FORMAT_IMA_ADPCM 0x11
#define WAVE_FORMAT_G721_ADPCM 0x40


#define PLAY_CMD 0x1
#define STOP_CMD 0x2
#define REW_CMD 0x3
#define FFD_CMD 0x4
#define REC_CMD 0x5



#define STOPPED_STATE 0x0
#define PLAY_STATE 0x1
#define RECORD_STATE 0x2

#define RECORD_MODE 0x0
#define PLAYER_MODE 0x1




// Return Codes        
#define RETURN_OK 0x0
#define REW_SAMPLES 0x1
#define FWD_SAMPLES 0x2
#define ERROR_READING_WAVE_HEADER 0x3



// Error Values  
//NO_ERROR                   equ     $0
#define NO_ERRORS_FOUND 0x0
#define RIFF_UNPACK_ERROR 0x1
#define WAVE_UNPACK_ERROR 0x2
#define FMT_UNPACK_ERROR 0x3
#define UNKNOWN_CODEC_ERROR 0x4
#define FACT_UNPACK_ERROR 0x5
#define DATA_UNPACK_ERROR 0x6


//Sec2Min                      equ     $022222 ; 1/60
//Min2Sec                      equ     60

