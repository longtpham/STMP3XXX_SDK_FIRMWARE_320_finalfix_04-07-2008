;******************************************************************************
; Copyright(C) SigmaTel, Inc. 2000/2001
;
; STMP Decoder Module Memory
;******************************************************************************
    section DECADPCMMOD_DecAdpcmMem


;*******************************************************************************
;  Include files
;*******************************************************************************
        include "dacequ.inc"
        include "hwequ.inc"
        include "sysequ.inc"
        include "msgequ.inc"
        include "dec_adpcmequ.inc"
        include "encadpcmequ.inc"
        include "project.inc"


;*******************************************************************************
;  Extern definitions
;*******************************************************************************
       GLOBAL   g_pAdpcmDecSinkBufMod
       GLOBAL   g_AdpcmDataSrc_buf
       GLOBAL   g_AdpcmDecSink_mbuf
       GLOBAL   g_AdpcmDecSinkBufDescriptor,g_pAdpcmDecSinkBufSize
       GLOBAL   g_pAdpcmDecSinkBufHead,g_pAdpcmDecSinkBufTail
       GLOBAL   g_AdpcmDataSrcBufDescriptor,g_pAdpcmDataSrcBufMod,g_pAdpcmDataSrcBufSize
       GLOBAL   g_pAdpcmDataSrcBufHead,g_pAdpcmDataSrcBufTail
       GLOBAL   DecAdpcmCmd
       GLOBAL   DecAdpcmReturnCode
       GLOBAL   DecAdpcmReturnValueHigh
       GLOBAL   DecAdpcmReturnValueLow
       GLOBAL   DecAdpcmSamplesSkip
       GLOBAL   AdpcmDecoderSaveR7
       GLOBAL   AdpcmDecoderSaveM7
       GLOBAL   AdpcmDecoderSourceBufDesc
       GLOBAL   AdpcmDecoderSinkBufDesc
       GLOBAL   AdpcmDecIsrSp
;       GLOBAL   AdpcmDecoderEventValue
       GLOBAL   AdpcmDecoderTimerCount
       GLOBAL   AdpcmABPositionAHigh
       GLOBAL   AdpcmABPositionALow
       GLOBAL   AdpcmABPositionBHigh
       GLOBAL   AdpcmABPositionBLow 
	   GLOBAL   AdpcmInitFlag
       GLOBAL	 AdpcmDecoderPointer
       GLOBAL   Last_block_flag
       GLOBAL   SamplestoDecode
       GLOBAL   inputbufsize 	
       GLOBAL    DecAdpcmTemp,DecAdpcmTemp1
       GLOBAL  	pSadpcmdecode,de_pred,de_step,de_index,pSadpcmdecodeEnd
       GLOBAL  	pDecIndexTable,pDecStepSizeTable
    	      
       GLOBAL   player_state

        GLOBAL   Dec_read_byte_state,Dec_wav_byte_state     
        GLOBAL   Fg_wDecRead_byte_state,Fg_wDecWav_byte_state     

        GLOBAL   Dec_error_flag,Fg_wDecError_flag
              
        GLOBAL   Dec_sample_cntH,Dec_sample_cntL,Dec_total_cntH,Dec_total_cntL      
        GLOBAL   Fg_wDec_sample_cntH,Fg_wDec_sample_cntL,Fg_wDec_total_cntH,Fg_wDec_total_cntL      

        GLOBAL   Dec_riff_ckidH,Dec_riff_ckidL,Dec_riff_chunksizeH,Dec_riff_chunksizeL,Dec_cktypeH,Dec_cktypeL         
        GLOBAL   Fg_wDec_riff_ckidH,Fg_wDec_riff_ckidL,Fg_wDec_riff_chunksizeH,Fg_wDec_riff_chunksizeL,Fg_wDec_CktypeH,Fg_wDec_CktypeL          
        GLOBAL   Dec_wave_ckidH,Dec_wave_ckidL
        GLOBAL   Fg_wDec_wave_ckidH,Fg_wDec_wave_ckidL
              
        GLOBAL   Dec_wave_chunksize,Dec_wformattag,Dec_nchannels,Dec_nsamplespersec,Dec_navgbytespersec,Dec_nblockalign
        GLOBAL   Fg_wDec_wave_chunksize,Fg_Dec_wformattag,Fg_wDec_Nchannels,Fg_wDecNsamplespersec,Fg_wDec_Navgbytespersec,Fg_wDec_Nblockalign

        GLOBAL   Dec_wbitspersample,Dec_cbsize,Dec_wsamplesperblock 
        GLOBAL   Fg_Dec_wbitspersample,Fg_wDec_Cbsize,Fg_Dec_wsamplesperblock               

        GLOBAL   Dec_fact_ckidH,Dec_fact_ckidL
        GLOBAL   Fg_wDec_fact_ckidH,Fg_wDec_fact_ckidL

        GLOBAL   Dec_data_ckidH,Dec_data_ckidL
        GLOBAL   Fg_wDec_data_ckidH,Fg_wDec_data_ckidL

        GLOBAL   wave_header_size,Fg_wWave_header_size
        GLOBAL   Fg_wByteoffset,Byteoffset 
        GLOBAL   Fg_wAdpcmSecondstoSkip,AdpcmSecondstoSkip,RewindMode      	      
;*******************************************************************************
;  Extern references
;*******************************************************************************
;*******************************************************************************
;  X memory
;*******************************************************************************
      org   x,"DECAPP_DecAdpcmMem_mod_X":

; Decoder source buffer need not be modulo.
; No descriptors needed as we no longer stream data in.
g_AdpcmDataSrc_buf       ds              g_AdpcmDataSrcBufSize         

      org   x,"DECAPP_DecAdpcmMem_X":

AdpcmDecoderSaveR7   dc      0
AdpcmDecoderSaveM7   dc      0

; ADPCM Tables
; 1. Index Table
; 2. Stepsize table
; and X memory variable(s)

pDecIndexTable
        		DC        -1
        		DC        -1
        		DC        -1
        		DC        -1
        		DC        2
        		DC        4
        		DC        6
        		DC        8
        		DC        -1
        		DC        -1
        		DC        -1
        		DC        -1
        		DC        2
        		DC        4
        		DC        6
                        DC        8

    ; All step sizes are aligned to MSB.
pDecStepSizeTable
                        DC        $700 
                        DC        $800 
                        DC        $900 
                        DC        $a00 
                        DC        $b00 
                        DC        $c00 
                        DC        $d00 
                        DC        $e00 
                        DC        $1000 
                        DC        $1100 
                        DC        $1300 
                        DC        $1500 
                        DC        $1700 
                        DC        $1900 
                        DC        $1c00 
                        DC        $1f00 
                        DC        $2200 
                        DC        $2500 
                        DC        $2900 
                        DC        $2d00 
                        DC        $3200 
                        DC        $3700 
                        DC        $3c00 
                        DC        $4200 
                        DC        $4900 
                        DC        $5000 
                        DC        $5800 
                        DC        $6100 
                        DC        $6b00 
                        DC        $7600 
                        DC        $8200 
                        DC        $8f00 
                        DC        $9d00 
                        DC        $ad00 
                        DC        $be00 
                        DC        $d100 
                        DC        $e600 
                        DC        $fd00 
                        DC        $11700 
                        DC        $13300 
                        DC        $15100 
                        DC        $17300 
                        DC        $19800 
                        DC        $1c100 
                        DC        $1ee00 
                        DC        $22000 
                        DC        $25600 
                        DC        $29200 
                        DC        $2d400 
                        DC        $31c00 
                        DC        $36c00 
                        DC        $3c300 
                        DC        $42400 
                        DC        $48e00 
                        DC        $50200 
                        DC        $58300 
                        DC        $61000 
                        DC        $6ab00 
                        DC        $75600 
                        DC        $81200 
                        DC        $8e000 
                        DC        $9c300 
                        DC        $abd00 
                        DC        $bd000 
                        DC        $cff00 
                        DC        $e4c00 
                        DC        $fba00 
                        DC        $114c00 
                        DC        $130700 
                        DC        $14ee00 
                        DC        $170600 
                        DC        $195400 
                        DC        $1bdc00 
                        DC        $1ea500 
                        DC        $21b600 
                        DC        $251500 
                        DC        $28ca00 
                        DC        $2cdf00 
                        DC        $315b00 
                        DC        $364b00 
                        DC        $3bb900 
                        DC        $41b200 
                        DC        $484400 
                        DC        $4f7e00 
                        DC        $577100 
                        DC        $602f00 
                        DC        $69ce00 
                        DC        $746200 
                        DC        $7fff00 


;*******************************************************************************
;  Y memory
;*******************************************************************************

;///////////////////////////////////////////////////////////////////////////////
;  IMPORTANT NOTE ON DECAPP_DecAdpcmMem_mod_Y:
;
;   The ADPCM Sink Buffer is a Modulo buffer.  Its current size is 6656 (=$1A00) words.
;   This must be aligned on an address that is a multiple of (currently) $2000.
;   An address which works is Y:$2000.
;
;   The Tasking Linker/Locator is getting confused.  If the buffer size (in the "buffer"
;   directive, below) is > 4096, the link fails to fit.  It appears that the locator is
;   trying to move the buffer to y:$4000, but it's hard to tell.
;
;   The y:$2000 address should work, so we fake out the linker/locator by declaring a
;   modulu buffer of 4096 (=$1000) words, using "g_AdpcmDecSinkBufSize_1", immediately 
;   followed by another buffer of 2560 (=$A000) words (using "g_AdpcmDecSinkBufSize_2"), 
;   to make a total of 6656.  The linker is happy with this, and so is the application.
;
;   It's not the way it's supposed to be done, but the tool isn't working right.
;
;   See Also: dec_adpcmequ.inc
;
;///////////////////////////////////////////////////////////////////////////////

      org   y,"DECAPP_DecAdpcmMem_mod_Y":

        buffer  M,g_AdpcmDecSinkBufSize_1         
g_AdpcmDecSink_mbuf
        ds     g_AdpcmDecSinkBufSize_1
        endbuf
g_AdpcmDecSink_extra    ds   g_AdpcmDecSinkBufSize_2

      org   y,"DECAPP_DecAdpcmMem_Y":

;Warning: The g_AdpcmDataSrc_mbuf locations below must stay together in this order!
g_AdpcmDataSrcBufDescriptor  dc      g_AdpcmDataSrc_buf
g_pAdpcmDataSrcBufMod        dc      g_AdpcmDataSrcBufModulo
g_pAdpcmDataSrcBufSize       dc      g_AdpcmDataSrcBufSize
g_pAdpcmDataSrcBufHead       dc      g_AdpcmDataSrc_buf
g_pAdpcmDataSrcBufTail       dc      g_AdpcmDataSrc_buf

;Warning: The g_AdpcmDecSink_mbuf locations below must stay together in this order!
g_AdpcmDecSinkBufDescriptor  dc      g_AdpcmDecSink_mbuf
g_pAdpcmDecSinkBufMod        dc      g_AdpcmDecSinkBufModulo
g_pAdpcmDecSinkBufSize       dc      g_AdpcmDecSinkBufSize
g_pAdpcmDecSinkBufHead       dc      g_AdpcmDecSink_mbuf
g_pAdpcmDecSinkBufTail       dc      g_AdpcmDecSink_mbuf

DecAdpcmCmd              dc      $0
DecAdpcmSamplesSkip      dc      $10              ; 3.0 seconds (Samples to skip in 20.4 sec format)
DecAdpcmReturnCode       dc      $0
DecAdpcmReturnValueHigh  dc      $0        
DecAdpcmReturnValueLow   dc      $0

AdpcmDecIsrSp             dc      0     ; decoder stores stack ptr here when saving off stack

AdpcmDecoderSourceBufDesc dc      g_AdpcmDataSrcBufDescriptor
AdpcmDecoderSinkBufDesc   dc      g_AdpcmDecSinkBufDescriptor

;AdpcmDecoderEventValue    dc      EVENT_MESSAGE       ; message to return to executive
AdpcmDecoderTimerCount    dc      DecoderTimerCount128kbps ; timer isr count value
; AB Mode Byte Positions
AdpcmABPositionAHigh      dc      0
AdpcmABPositionALow       dc      0
AdpcmABPositionBHigh      dc      0
AdpcmABPositionBLow       dc      0
Last_block_flag		  	  dc	  0
SamplestoDecode 		  dc	  0
inputbufsize 			  dc	  0
AdpcmInitFlag			  dc	  0
AdpcmDecoderPointer       dc      0
RewindMode                dc      0       

;-----------------------------------------------------------------
; File System Locations
;-----------------------------------------------------------------
; below all init to zero ***********
;AdpcmDecoderFileHandle    dc      0          ; the handle assigned by FileOpen
; above all init to zero ***********

;-----------------------------------------------------------------
; ADPCM Variables
;-----------------------------------------------------------------
DecAdpcmTemp            DC        $0
DecAdpcmTemp1           DC        $0

pSadpcmdecode
de_pred                 DC        $0
de_step                 DC        $700

pSadpcmdecodeEnd
de_index                DC        $0

;wav_byte_state          DC        $0
player_state            DC        $0


    
;-----------------------------------------------------------------
; ADPCM Variables
;-----------------------------------------------------------------
Fg_wByteoffset
Byteoffset           dc      0          ; used by parser to tell SysFindSong the offset to song name

Fg_wDecRead_byte_state
Dec_read_byte_state                 DC          $0

Fg_wDecWav_byte_state
Dec_wav_byte_state                  DC          $0

Fg_wDecError_flag
Dec_error_flag                      DC          $0


;player_state                    DC          $0
Fg_wRecorder_state
recorder_state                  DC          $0


Fg_wDec_sample_cntH
Dec_sample_cntH                     DC          $0

Fg_wDec_sample_cntL
Dec_sample_cntL                     DC          $0

Fg_wDec_total_cntH
Dec_total_cntH                      DC          $0

Fg_wDec_total_cntL
Dec_total_cntL                      DC          $0


; WAVE TABLE HEADER STORAGE SPACE
; RIFF_HEADER
Fg_wDec_riff_ckidH
Dec_riff_ckidH              DCB       'FIR'

Fg_wDec_riff_ckidL
Dec_riff_ckidL              DCB       'F'
; Chunksize can exceed 16 Mb, so provision more space.
Fg_wDec_riff_chunksizeH
Dec_riff_chunksizeH         DC        $0

Fg_wDec_riff_chunksizeL
Dec_riff_chunksizeL         DC        $0

Fg_wDec_CktypeH
Dec_cktypeH                 DCB       'VAW'

Fg_wDec_CktypeL
Dec_cktypeL                 DCB       'E'

; WAVE_FORMAT;
Fg_wDec_wave_ckidH
Dec_wave_ckidH              DCB       'tmf'

Fg_wDec_wave_ckidL
Dec_wave_ckidL              DCB       ' '
; Chunks are small for wave header
Fg_wDec_wave_chunksize
Dec_wave_chunksize                  DC          $14

Fg_Dec_wformattag
Dec_wformattag                      DC          $0011

Fg_wDec_Nchannels
Dec_nchannels                       DC          $1

Fg_wDecNsamplespersec
Dec_nsamplespersec                  DC          DEFAULT_ADPCM_NSAMPLESPERSEC

Fg_wDec_Navgbytespersec
Dec_navgbytespersec                 DC          DEFAULT_ADPCM_NAVGBYTESPERSEC

Fg_wDec_Nblockalign
Dec_nblockalign                     DC          `512

Fg_Dec_wbitspersample
Dec_wbitspersample                  DC          $4

Fg_wDec_Cbsize
Dec_cbsize                          DC          $2

Fg_Dec_wsamplesperblock
Dec_wsamplesperblock                DC          ADPCM_BLOCK_SIZE*2-8+1

; FACT_HEADER;
Fg_wDec_fact_ckidH
Dec_fact_ckidH              DCB       'caf'

Fg_wDec_fact_ckidL
Dec_fact_ckidL              DCB       't'

;DATA_HEADER;
Fg_wDec_Data_ckidH
Dec_data_ckidH              DCB       'tad'

Fg_wDec_Data_ckidL
Dec_data_ckidL              DCB       'a'

; BYTES IN HEADER
Fg_wWave_header_size
wave_header_size        DC        $0


        endsec

