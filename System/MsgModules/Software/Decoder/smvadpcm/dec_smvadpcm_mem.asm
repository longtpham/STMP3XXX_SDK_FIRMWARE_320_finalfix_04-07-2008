;******************************************************************************
; Copyright(C) SigmaTel, Inc. 2000/2001
;
; STMP Decoder Module Memory
;******************************************************************************
    section DECSMVADPCMMOD_DecSmvAdpcmMem


;*******************************************************************************
;  Include files
;*******************************************************************************
        include "dacequ.inc"
        include "hwequ.inc"
        include "sysequ.inc"
        include "msgequ.inc"
        include "dec_smvadpcmequ.inc"
        include "encadpcmequ.inc"
        include "project.inc"


;*******************************************************************************
;  Extern definitions
;*******************************************************************************
       GLOBAL   g_pSmvAdpcmDecSinkBufMod
       GLOBAL   g_SmvAdpcmDataSrc_buf
       GLOBAL   g_SmvAdpcmDecSink_mbuf
       GLOBAL   g_SmvAdpcmDecSinkBufDescriptor,g_pSmvAdpcmDecSinkBufSize
       GLOBAL   g_pSmvAdpcmDecSinkBufHead,g_pSmvAdpcmDecSinkBufTail
       GLOBAL   g_SmvAdpcmDataSrcBufDescriptor,g_pSmvAdpcmDataSrcBufMod,g_pSmvAdpcmDataSrcBufSize
       GLOBAL   g_pSmvAdpcmDataSrcBufHead,g_pSmvAdpcmDataSrcBufTail
       GLOBAL   DecSmvAdpcmCmd
       GLOBAL   DecSmvAdpcmReturnCode
       GLOBAL   DecSmvAdpcmReturnValueHigh
       GLOBAL   DecSmvAdpcmReturnValueLow
       GLOBAL   DecSmvAdpcmSamplesSkip
       GLOBAL   SmvAdpcmDecoderSaveR7
       GLOBAL   SmvAdpcmDecoderSaveM7
       GLOBAL   SmvAdpcmDecoderSourceBufDesc
       GLOBAL   SmvAdpcmDecoderSinkBufDesc
       GLOBAL   SmvAdpcmDecIsrSp
;       GLOBAL   SmvAdpcmDecoderEventValue
       GLOBAL   SmvAdpcmDecoderTimerCount
       GLOBAL   SmvAdpcmABPositionAHigh
       GLOBAL   SmvAdpcmABPositionALow
       GLOBAL   SmvAdpcmABPositionBHigh
       GLOBAL   SmvAdpcmABPositionBLow 
	   GLOBAL   SmvAdpcmInitFlag
       GLOBAL	SmvAdpcmDecoderPointer
       GLOBAL   Smv_Last_block_flag
       GLOBAL   SmvSamplestoDecode
       GLOBAL   smvinputbufsize 	
       GLOBAL    DecSmvAdpcmTemp,DecSmvAdpcmTemp1
       GLOBAL  	pSsmvadpcmdecode,smv_de_pred,smv_de_step,smv_de_index,pSsmvadpcmdecodeEnd
       GLOBAL  	pSmvDecIndexTable,pSmvDecStepSizeTable
    	      
       GLOBAL   smv_player_state

        GLOBAL   Smv_Dec_read_byte_state,Smv_Dec_wav_byte_state     
        GLOBAL   Fg_wSmv_DecRead_byte_state,Fg_wSmv_DecWav_byte_state     

        GLOBAL   Smv_Dec_error_flag,Fg_wSmv_DecError_flag
              
        GLOBAL   Smv_Dec_sample_cntH,Smv_Dec_sample_cntL,Smv_Dec_total_cntH,Smv_Dec_total_cntL      
        GLOBAL   Fg_wSmv_Dec_sample_cntH,Fg_wSmv_Dec_sample_cntL,Fg_wSmv_Dec_total_cntH,Fg_wSmv_Dec_total_cntL      

        GLOBAL   Smv_Dec_riff_ckidH,Smv_Dec_riff_ckidL,Smv_Dec_riff_chunksizeH,Smv_Dec_riff_chunksizeL,Smv_Dec_cktypeH,Smv_Dec_cktypeL         
        GLOBAL   Fg_wSmv_Dec_riff_ckidH,Fg_wSmv_Dec_riff_ckidL,Fg_wSmv_Dec_riff_chunksizeH,Fg_wSmv_Dec_riff_chunksizeL,Fg_wSmv_Dec_CktypeH,Fg_wSmv_Dec_CktypeL          
        GLOBAL   Smv_Dec_wave_ckidH,Smv_Dec_wave_ckidL
        GLOBAL   Fg_wSmv_Dec_wave_ckidH,Fg_wSmv_Dec_wave_ckidL
              
        GLOBAL   Smv_Dec_wave_chunksize,Smv_Dec_wformattag,Smv_Dec_nchannels,Smv_Dec_nsamplespersec,Smv_Dec_navgbytespersec,Smv_Dec_nblockalign
        GLOBAL   Fg_wSmv_Dec_wave_chunksize,Fg_Smv_Dec_wformattag,Fg_wSmv_Dec_Nchannels,Fg_wSmv_DecNsamplespersec,Fg_wSmv_Dec_Navgbytespersec,Fg_wSmv_Dec_Nblockalign

        GLOBAL   Smv_Dec_wbitspersample,Smv_Dec_cbsize,Smv_Dec_wsamplesperblock 
        GLOBAL   Fg_Smv_Dec_wbitspersample,Fg_wSmv_Dec_Cbsize,Fg_Smv_Dec_wsamplesperblock               

        GLOBAL   Smv_Dec_fact_ckidH,Smv_Dec_fact_ckidL
        GLOBAL   Fg_wSmv_Dec_fact_ckidH,Fg_wSmv_Dec_fact_ckidL

        GLOBAL   Smv_Dec_data_ckidH,Smv_Dec_data_ckidL
        GLOBAL   Fg_wSmv_Dec_data_ckidH,Fg_wSmv_Dec_data_ckidL

        GLOBAL   smv_wave_header_size,Fg_wSmv_Wave_header_size
        GLOBAL   Fg_wSmv_Byteoffset,Smv_Byteoffset 
        GLOBAL   Fg_wSmvAdpcmSecondstoSkip,SmvAdpcmSecondstoSkip,SmvRewindMode      	      
;*******************************************************************************
;  Extern references
;*******************************************************************************
;*******************************************************************************
;  X memory
;*******************************************************************************
      org   x,"DECAPP_DecSmvAdpcmMem_mod_X":

; Decoder source buffer need not be modulo.
; No descriptors needed as we no longer stream data in.
g_SmvAdpcmDataSrc_buf       ds              g_SmvAdpcmDataSrcBufSize         

      org   x,"DECAPP_DecSmvAdpcmMem_X":

SmvAdpcmDecoderSaveR7   dc      0
SmvAdpcmDecoderSaveM7   dc      0

; ADPCM Tables
; 1. Index Table
; 2. Stepsize table
; and X memory variable(s)

pSmvDecIndexTable
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
pSmvDecStepSizeTable
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
;  IMPORTANT NOTE ON DECAPP_DecSmvAdpcmMem_mod_Y:
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
;   modulu buffer of 4096 (=$1000) words, using "g_SmvAdpcmDecSinkBufSize_1", immediately 
;   followed by another buffer of 2560 (=$A000) words (using "g_SmvAdpcmDecSinkBufSize_2"),
;   to make a total of 6656.  The linker is happy with this, and so is the application.
;
;   It's not the way it's supposed to be done, but the tool isn't working right.
;
;   See Also: dec_smvadpcmequ.inc
;
;///////////////////////////////////////////////////////////////////////////////

      org   y,"DECAPP_DecSmvAdpcmMem_mod_Y":

        buffer  M,g_SmvAdpcmDecSinkBufSize_1
g_SmvAdpcmDecSink_mbuf
        ds     g_SmvAdpcmDecSinkBufSize_1
        endbuf
;g_SmvAdpcmDecSink_extra    ds   g_SmvAdpcmDecSinkBufSize_2

      org   y,"DECAPP_DecSmvAdpcmMem_Y":

;Warning: The g_SmvAdpcmDataSrc_mbuf locations below must stay together in this order!
g_SmvAdpcmDataSrcBufDescriptor  dc      g_SmvAdpcmDataSrc_buf
g_pSmvAdpcmDataSrcBufMod        dc      g_SmvAdpcmDataSrcBufModulo
g_pSmvAdpcmDataSrcBufSize       dc      g_SmvAdpcmDataSrcBufSize
g_pSmvAdpcmDataSrcBufHead       dc      g_SmvAdpcmDataSrc_buf
g_pSmvAdpcmDataSrcBufTail       dc      g_SmvAdpcmDataSrc_buf

;Warning: The g_SmvAdpcmDecSink_mbuf locations below must stay together in this order!
g_SmvAdpcmDecSinkBufDescriptor  dc      g_SmvAdpcmDecSink_mbuf
g_pSmvAdpcmDecSinkBufMod        dc      g_SmvAdpcmDecSinkBufModulo
g_pSmvAdpcmDecSinkBufSize       dc      g_SmvAdpcmDecSinkBufSize
g_pSmvAdpcmDecSinkBufHead       dc      g_SmvAdpcmDecSink_mbuf
g_pSmvAdpcmDecSinkBufTail       dc      g_SmvAdpcmDecSink_mbuf

DecSmvAdpcmCmd              dc      $0
DecSmvAdpcmSamplesSkip      dc      $10              ; 3.0 seconds (Samples to skip in 20.4 sec format)
DecSmvAdpcmReturnCode       dc      $0
DecSmvAdpcmReturnValueHigh  dc      $0        
DecSmvAdpcmReturnValueLow   dc      $0

SmvAdpcmDecIsrSp             dc      0     ; decoder stores stack ptr here when saving off stack

SmvAdpcmDecoderSourceBufDesc dc      g_SmvAdpcmDataSrcBufDescriptor
SmvAdpcmDecoderSinkBufDesc   dc      g_SmvAdpcmDecSinkBufDescriptor

;SmvAdpcmDecoderEventValue    dc      EVENT_MESSAGE       ; message to return to executive
SmvAdpcmDecoderTimerCount    dc      DecoderTimerCount128kbps ; timer isr count value
; AB Mode Byte Positions
SmvAdpcmABPositionAHigh      dc      0
SmvAdpcmABPositionALow       dc      0
SmvAdpcmABPositionBHigh      dc      0
SmvAdpcmABPositionBLow       dc      0
Smv_Last_block_flag		  	  dc	  0
SmvSamplestoDecode 		  dc	  0
smvinputbufsize 			  dc	  0
SmvAdpcmInitFlag			  dc	  0
SmvAdpcmDecoderPointer       dc      0
SmvRewindMode                dc      0       

;-----------------------------------------------------------------
; File System Locations
;-----------------------------------------------------------------
; below all init to zero ***********
;SmvAdpcmDecoderFileHandle    dc      0          ; the handle assigned by FileOpen
; above all init to zero ***********

;-----------------------------------------------------------------
; ADPCM Variables
;-----------------------------------------------------------------
DecSmvAdpcmTemp            DC        $0
DecSmvAdpcmTemp1           DC        $0

pSsmvadpcmdecode
smv_de_pred                 DC        $0
smv_de_step                 DC        $700

pSsmvadpcmdecodeEnd
smv_de_index                DC        $0

;wav_byte_state          DC        $0
smv_player_state            DC        $0


    
;-----------------------------------------------------------------
; ADPCM Variables
;-----------------------------------------------------------------
Fg_wSmv_Byteoffset
Smv_Byteoffset           dc      0          ; used by parser to tell SysFindSong the offset to song name

Fg_wSmv_DecRead_byte_state
Smv_Dec_read_byte_state                 DC          $0

Fg_wSmv_DecWav_byte_state
Smv_Dec_wav_byte_state                  DC          $0

Fg_wSmv_DecError_flag
Smv_Dec_error_flag                      DC          $0


;smv_player_state                    DC          $0
Fg_wRecorder_state
recorder_state                  DC          $0


Fg_wSmv_Dec_sample_cntH
Smv_Dec_sample_cntH                     DC          $0

Fg_wSmv_Dec_sample_cntL
Smv_Dec_sample_cntL                     DC          $0

Fg_wSmv_Dec_total_cntH
Smv_Dec_total_cntH                      DC          $0

Fg_wSmv_Dec_total_cntL
Smv_Dec_total_cntL                      DC          $0


; WAVE TABLE HEADER STORAGE SPACE
; RIFF_HEADER
Fg_wSmv_Dec_riff_ckidH
Smv_Dec_riff_ckidH              DCB       'FIR'

Fg_wSmv_Dec_riff_ckidL
Smv_Dec_riff_ckidL              DCB       'F'
; Chunksize can exceed 16 Mb, so provision more space.
Fg_wSmv_Dec_riff_chunksizeH
Smv_Dec_riff_chunksizeH         DC        $0

Fg_wSmv_Dec_riff_chunksizeL
Smv_Dec_riff_chunksizeL         DC        $0

Fg_wSmv_Dec_CktypeH
Smv_Dec_cktypeH                 DCB       'VAW'

Fg_wSmv_Dec_CktypeL
Smv_Dec_cktypeL                 DCB       'E'

; WAVE_FORMAT;
Fg_wSmv_Dec_wave_ckidH
Smv_Dec_wave_ckidH              DCB       'tmf'

Fg_wSmv_Dec_wave_ckidL
Smv_Dec_wave_ckidL              DCB       ' '
; Chunks are small for wave header
Fg_wSmv_Dec_wave_chunksize
Smv_Dec_wave_chunksize                  DC          $14

Fg_Smv_Dec_wformattag
Smv_Dec_wformattag                      DC          $0011

Fg_wSmv_Dec_Nchannels
Smv_Dec_nchannels                       DC          $1

Fg_wSmv_DecNsamplespersec
Smv_Dec_nsamplespersec                  DC          DEFAULT_ADPCM_NSAMPLESPERSEC

Fg_wSmv_Dec_Navgbytespersec
Smv_Dec_navgbytespersec                 DC          DEFAULT_ADPCM_NAVGBYTESPERSEC

Fg_wSmv_Dec_Nblockalign
Smv_Dec_nblockalign                     DC          `512

Fg_Smv_Dec_wbitspersample
Smv_Dec_wbitspersample                  DC          $4

Fg_wSmv_Dec_Cbsize
Smv_Dec_cbsize                          DC          $2

Fg_Smv_Dec_wsamplesperblock
Smv_Dec_wsamplesperblock                DC          ADPCM_BLOCK_SIZE*2-8+1

; FACT_HEADER;
Fg_wSmv_Dec_fact_ckidH
Smv_Dec_fact_ckidH              DCB       'caf'

Fg_wSmv_Dec_fact_ckidL
Smv_Dec_fact_ckidL              DCB       't'

;DATA_HEADER;
Fg_wDec_Data_ckidH
Smv_Dec_data_ckidH              DCB       'tad'

Fg_wDec_Data_ckidL
Smv_Dec_data_ckidL              DCB       'a'

; BYTES IN HEADER
Fg_wSmv_Wave_header_size
smv_wave_header_size        DC        $0


        endsec

