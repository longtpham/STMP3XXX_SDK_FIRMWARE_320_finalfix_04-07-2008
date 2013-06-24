    section Mp3EncMem

	GLOBAL	MP3EncoderSourceBufDesc
    GLOBAL  MP3EncoderSourceBuf
	GLOBAL	MP3EncoderBufAddrPtr
	GLOBAL	MP3EncoderBufModPtr
	GLOBAL	MP3EncoderBufSizePtr
	GLOBAL	MP3EncoderBufHeadPtr
	GLOBAL	MP3EncoderBufTailPtr

	GLOBAL	MP3EncoderSinkBufDesc
	GLOBAL	MP3EncoderSinkBufAddrPtr
	GLOBAL	MP3EncoderSinkBufModPtr
	GLOBAL	MP3EncoderSinkBufSizePtr
	GLOBAL	MP3EncoderSinkBufHeadPtr
	GLOBAL	MP3EncoderSinkBufTailPtr

	GLOBAL	FMP3EncoderSourceBufDesc
	GLOBAL	FMP3EncoderBufAddrPtr
	GLOBAL	FMP3EncoderBufModPtr
	GLOBAL	FMP3EncoderBufSizePtr
	GLOBAL	FMP3EncoderBufHeadPtr
	GLOBAL	FMP3EncoderBufTailPtr
	GLOBAL	FMP3EncoderSinkBufDesc
	GLOBAL	FMP3EncoderSinkBufAddrPtr
	GLOBAL	FMP3EncoderSinkBufModPtr
	GLOBAL	FMP3EncoderSinkBufSizePtr
	GLOBAL	FMP3EncoderSinkBufHeadPtr
	GLOBAL	FMP3EncoderSinkBufTailPtr
    
    GLOBAL FMp3EncFrameCount
    GLOBAL FMp3Enc_BitRate     
    GLOBAL Mp3Enc_BitRate      
    GLOBAL FMp3Enc_SamplingRate
    GLOBAL Mp3Enc_SamplingRate 
    GLOBAL FMp3Enc_NumChannels 
    GLOBAL Mp3Enc_NumChannels  




    include "encmp3equ.inc"

;*******************************************************************************
;  X memory
;*******************************************************************************
    org     x,"ENCAPP_EncMp3Mem_mod_X":

    buffer  M,MP3ENCODE_SINK_BUF_SIZE
MP3EncoderSinkBuf
    bsc     MP3ENCODE_SINK_BUF_SIZE,$0         ; The MP3 output buffer
    endbuf

;*******************************************************************************
;  Y memory
;*******************************************************************************
     org     y,"ENCAPP_EncMp3Mem_mod_Y":

        buffer  M,MP3ENCODE_SOURCE_BUF_SIZE
FMp3_Inbuf
MP3EncoderSourceBuf
        bsc     MP3ENCODE_SOURCE_BUF_SIZE,0         ; The MP3 input buffer
        endbuf




     org     y,"ENCAPP_EncMp3Mem_Y":

FMp3EncFrameCount              dc      0

FMp3Enc_BitRate                
Mp3Enc_BitRate                 dc      128
FMp3Enc_SamplingRate           
Mp3Enc_SamplingRate            dc      44
FMp3Enc_NumChannels            
Mp3Enc_NumChannels             dc      2

MP3EncoderSourceBufDesc
FMP3EncoderSourceBufDesc
FMP3EncoderBufAddrPtr
MP3EncoderBufAddrPtr           dc      MP3EncoderSourceBuf
FMP3EncoderBufModPtr
MP3EncoderBufModPtr            dc      MP3ENCODE_SOURCE_BUF_MODULO
FMP3EncoderBufSizePtr
MP3EncoderBufSizePtr           dc      MP3ENCODE_SOURCE_BUF_SIZE
FMP3EncoderBufHeadPtr
MP3EncoderBufHeadPtr           dc      MP3EncoderSourceBuf
FMP3EncoderBufTailPtr
MP3EncoderBufTailPtr           dc      MP3EncoderSourceBuf

FMP3EncoderSinkBufDesc
MP3EncoderSinkBufDesc
FMP3EncoderSinkBufAddrPtr
MP3EncoderSinkBufAddrPtr       dc      MP3EncoderSinkBuf
FMP3EncoderSinkBufModPtr
MP3EncoderSinkBufModPtr        dc      MP3ENCODE_SINK_BUF_MODULO
FMP3EncoderSinkBufSizePtr
MP3EncoderSinkBufSizePtr       dc      MP3ENCODE_SINK_BUF_SIZE
FMP3EncoderSinkBufHeadPtr
MP3EncoderSinkBufHeadPtr       dc      MP3EncoderSinkBuf
FMP3EncoderSinkBufTailPtr
MP3EncoderSinkBufTailPtr       dc      MP3EncoderSinkBuf

    endsec
