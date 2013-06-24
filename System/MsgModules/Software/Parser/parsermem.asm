;******************************************************************************
; Copyright(C) SigmaTel, Inc. 2001
;
; STMP Parser Module Memory
;******************************************************************************
        section PARMOD_ParserMem


;*******************************************************************************
;  Include files
;*******************************************************************************

        include "dacequ.inc"
        include "hwequ.inc"
        include "sysequ.inc"
        include "msgequ.inc"
        include "mp3dec.inc"
        include "decoderequ.inc"
        include "encadpcmequ.inc"

;*******************************************************************************
;  Extern definitions
;*******************************************************************************
        GLOBAL   ParserCSR,Fg_wParserCSR
        GLOBAL   ParserVoiceCSR,Fg_wParserVoiceCSR
        GLOBAL   DecoderType,Fg_wDecoderType
        GLOBAL   ShuffleSeed,Fg_wShuffleSeed
        GLOBAL   Shuffle0to23,Fg_wShuffle0to23
        GLOBAL   Shuffle24to47,Fg_wShuffle24to47
        GLOBAL   TotalSongsPlayed,Fg_wTotalSongsPlayed
        GLOBAL   ParserFileHandle,Fg_wParserFileHandle
        GLOBAL   ParserLoopCount,Fg_wParserLoopCount
        GLOBAL   SongByteLengthHigh,Fg_wSongByteLengthHigh
        GLOBAL   SongByteLengthLow,Fg_wSongByteLengthLow
        GLOBAL   SongExtensionList,Fg_wSongExtensionList
        GLOBAL   VoiceExtensionList,Fg_wVoiceExtensionList
        GLOBAL   Dev0Count,Dev1Count,Dev2Count,Dev3Count
        GLOBAL   Fg_wDev0Count,Fg_wDev1Count,Fg_wDev2Count,Fg_wDev3Count
        GLOBAL   SongTitle,Fg_wSongTitle
        GLOBAL   SongArtist,Fg_wSongArtist
        GLOBAL   DirectorySongNumber,Fg_wDirectorySongNumber
        GLOBAL   DirectorySongCount,Fg_wDirectorySongCount
        GLOBAL   CurrentDirDevId,Fg_wCurrentDirDevId
        GLOBAL   CurrentSongName,Fg_wCurrentSongName
        GLOBAL   SongExtension,Fg_wSongExtension
        GLOBAL   Mp3Extension,Fg_wMp3Extension
        GLOBAL   WavExtension,Fg_wWavExtension
        GLOBAL   Fg_wEncADCGain,EncAdcGain
        GLOBAL   EncADCSRR
    if (!(@def('DEVICE_3400')||@def('DEVICE_3300')))
    ; WMA specific code
        GLOBAL  WmaExtension,Fg_wWmaExtension
    endif
        GLOBAL   InvSampleRate,Fg_wInvSampleRate
        GLOBAL   EncClusterTimeScale,Fg_wEncClusterTimeScale
        GLOBAL   EncInvSampleRateScale,Fg_wEncInvSampleRateScale

        GLOBAL   read_byte_state,write_byte_state,wav_byte_state     
        GLOBAL   Fg_wRead_byte_state,Fg_wWrite_byte_state,Fg_wWav_byte_state     

        GLOBAL   pSadpcmencode,en_pred,en_index
        GLOBAL   Fg_wpSadpcmencode,Fg_wEn_pred,Fg_wEn_index          
        
        GLOBAL   CurrentSongBad,Fg_wCurrentSongBad

        GLOBAL   error_flag,Fg_wError_flag
              
        GLOBAL   sample_cntH,sample_cntL,total_cntH,total_cntL      
        GLOBAL   Fg_wSample_cntH,Fg_wSample_cntL,Fg_wTotal_cntH,Fg_wTotal_cntL      

        GLOBAL   riff_ckidH,riff_ckidL,riff_chunksizeH,riff_chunksizeL,cktypeH,cktypeL         
        GLOBAL   Fg_wRiff_ckidH,Fg_wRiff_ckidL,Fg_wRiff_chunksizeH,Fg_wRiff_chunksizeL,Fg_wCktypeH,Fg_wCktypeL          
                 
        GLOBAL   wave_ckidH,wave_ckidL
        GLOBAL   Fg_wWave_ckidH,Fg_wWave_ckidL
              
        GLOBAL   wave_chunksize,wformattag,nchannels,nsamplespersec,navgbytespersec,nblockalign,EncADCSRR      
        GLOBAL   Fg_wWave_chunksize,Fg_wFormattag,Fg_wNchannels,Fg_wNsamplespersec,Fg_wNavgbytespersec,Fg_wNblockalign,Fg_wEncADCSRR      

        GLOBAL   Fg_wEncNAvgBytesPerSec,EncNAvgBytesPerSec
        GLOBAL   Fg_wEncNSamplesPerSec,EncNSamplesPerSec

        GLOBAL   wbitspersample,cbsize,wsamplesperblock 
        GLOBAL   Fg_wBitspersample,Fg_wCbsize,Fg_wSamplesperblock               

        GLOBAL   fact_ckidH,fact_ckidL,fact_chunksize,timeL,timeH            
        GLOBAL   Fg_wFact_ckidH,Fg_wFact_ckidL,Fg_wFact_chunksize,Fg_wTimeL,Fg_wTimeH            

        GLOBAL   data_ckidH,data_ckidL,data_chunksizeL,data_chunksizeH  
        GLOBAL   Fg_wData_ckidH,Fg_wData_ckidL,Fg_wData_chunksizeL,Fg_wData_chunksizeH  

        GLOBAL   wave_header_size,Fg_wWave_header_size
        GLOBAL   Byteoffset,Fg_wByteoffset

        GLOBAL   Fg_wList_ckidH,list_ckidH,Fg_wList_ckidL,list_ckidL,Fg_wList_chunksize,list_chunksize
        GLOBAL   Fg_wInfo_ckidH,info_ckidH,Fg_wInfo_ckidL,info_ckidL,Fg_wIsft_ckidH,isft_ckidH,Fg_wIsft_ckidL
        GLOBAL   isft_ckidL,Fg_wIsft_chunksize,isft_chunksize,Fg_wSig_ckidH,sig_ckidH,Fg_wSig_ckidM
        GLOBAL   sig_ckidM,Fg_wSig_ckidL,sig_ckidL

        GLOBAL    SecondstoSkip

;*******************************************************************************
;  Extern references
;*******************************************************************************

;*******************************************************************************
;  X memory
;*******************************************************************************
        org x,"PARMOD_ParserMem_X":
Fg_wParserCSR
ParserCSR            dc      0

Fg_wParserVoiceCSR
ParserVoiceCSR       dc      0

Fg_wDecoderType
DecoderType          dc      0           ; parser stores the decoder type here
;*******************************************************************************
;  Y memory
;*******************************************************************************
          org     y,"PARMOD_ParserMem_Y":

  
;-----------------------------------------------------------------
; File System Locations
;-----------------------------------------------------------------
; below all init to zero ***********
Fg_wParserLoopCount
ParserLoopCount      dc      0          ; count of how many parser voice files have played

Fg_wParserFileHandle
ParserFileHandle     dc      0          ; the handle assigned by FileOpen

Fg_wSongByteLengthHigh
SongByteLengthHigh   dc      0          ; the number of bytes in the entire song (MSBs)

Fg_wSongByteLengthLow
SongByteLengthLow    dc      0          ; the number of bytes in the entire song (LSBs)

Fg_wCurrentSongName
CurrentSongName      ds      MaxDirCount+5  ; song name will be 8.3 = 12 bytes plus 1 for '0' to end, filled in by SysFindSong

Fg_wByteoffset
Byteoffset           dc      0          ; used by parser to tell SysFindSong the offset to song name

Fg_wDirectorySongNumber
DirectorySongNumber  dc      0          ; the song number in the directory or playlist (1 to N)

Fg_wDirectorySongCount
DirectorySongCount   dc      0          ; the number of songs in the current directory

Fg_wDev0Count
Dev0Count            dc      0          ; the number of songs in device 0

Fg_wDev1Count 
Dev1Count            dc      0          ; the number of songs in device 1

Fg_wDev2Count
Dev2Count            dc      0          ; the number of songs in device 2

Fg_wDev3Count
Dev3Count            dc      0          ; the number of songs in device 3
; Song Info (from TAG, header, etc)
; NOTE: ** SongArtist must be right before SongTitle. See DO loop in decoder_overlay.asm - search for _ID3Copy
Fg_wSongTitle                           ;this has changed to be Unicode!
SongTitle            ds      31         ; size = 10 to match ID3v1 (this can grow for ID3v2)

Fg_wSongArtist
SongArtist           ds      31         ; size = 10 to match ID3v1 (this can grow for ID3v2)

Fg_wCurrentDirDevId
CurrentDirDevId      dc      0          ; device id of the directory (card #)

; GDA_20010530
Fg_wCurrentSongBad
CurrentSongBad       dc      0          ; Non-zero if current song was found to be corrupted, zero-length, etc.
                                        ;  If set, code should avoid trying to read the file.   
; above all init to zero ***********

; the extension list must be grouped together
Fg_wSongExtension
SongExtension        dc      0          ; store current song extension here

    if (@def('DEVICE_3400')||@def('DEVICE_3300'))
Fg_wSongExtensionList
SongExtensionList    dc      11         ; number of extensions
    else
    ; WMA code inclusion
Fg_wSongExtensionList
SongExtensionList    dc      19         ; number of extensions
    endif

Fg_wMp3Extension
Mp3Extension         dc      $33706D    ; '3' , 'p' , 'm'  "mp3"
                     dc      $33504D    ; "MP3"
                     dc      $33704D    ; "Mp3"
Fg_wWavExtension
WavExtension         dc      $564157    ; 'V' , 'A' , 'W'  "WAV"
                     dc      $766177    ; 'v' , 'a' , 'w'  "wav"
                     dc      $564177    ; VAw
                     dc      $566177    ; Vaw
                     dc      $566157    ; VaW
                     dc      $764157    ; vAW
                     dc      $764177    ; vAw
                     dc      $766157    ; vaW
    if (!(@def('DEVICE_3400')||@def('DEVICE_3300')))
Fg_wWmaExtension
WmaExtension         dc      $616D77    ; 'a' , 'm' , 'w'  "wma"  wma
                     dc      $416d77    ; wmA
                     dc      $614d77    ; wMa
                     dc      $414d77    ; wMA
                     dc      $616d57    ; Wma
                     dc      $416d57    ; WmA
                     dc      $614d57    ; WMa
                     dc      $414d57    ; WMA
    endif

Fg_wVoiceExtensionList
VoiceExtensionList   dc      1

Fg_wVoiceExtension
VoiceExtension       dc      $564157    ; "VAW"

Fg_wInvSampleRate
InvSampleRate        dc      0          ; inverse of the adpcm sample rate
;-----------------------------------------------------------------
; Shuffle Algorithm Memory Space
;-----------------------------------------------------------------
Fg_wShuffleSeed
ShuffleSeed          dc      15123      ; init to some value, save each time shutdown occurs

Fg_wShuffle0to23
Shuffle0to23         dc      0          ; keeps track of which songs played (0-23)

Fg_wShuffle24to47
Shuffle24to47        dc      0          ; keeps track of which songs played (24-47

Fg_wTotalSongsPlayed
TotalSongsPlayed     dc      0          ; used by Shuffle to know when complete

;-----------------------------------------------------------------
; ADPCM Variables
;-----------------------------------------------------------------

Fg_wEncADCSRR
EncADCSRR                       DC          DEFAULT_ADPCM_ADCSRR

Fg_wpSadpcmencode
pSadpcmencode
Fg_wEn_pred
en_pred                         DC          $0

Fg_wEn_index
en_index                        DC          $0

;pSadpcmdecode
;de_pred                         DC          $0
;de_step                         DC          $700

;pSadpcmdecodeEnd
;de_index                        DC          $0

Fg_wWrite_byte_state
write_byte_state                DC          $0

Fg_wRead_byte_state
read_byte_state                 DC          $0

Fg_wWav_byte_state
wav_byte_state                  DC          $0

Fg_wError_flag
error_flag                      DC          $0


;player_state                    DC          $0
Fg_wRecorder_state
recorder_state                  DC          $0


Fg_wSample_cntH
sample_cntH                     DC          $0

Fg_wSample_cntL
sample_cntL                     DC          $0

Fg_wTotal_cntH
total_cntH                      DC          $0

Fg_wTotal_cntL
total_cntL                      DC          $0


; WAVE TABLE HEADER STORAGE SPACE
; RIFF_HEADER
Fg_wRiff_ckidH
riff_ckidH              DCB       'FIR'

Fg_wRiff_ckidL
riff_ckidL              DCB       'F'
; Chunksize can exceed 16 Mb, so provision more space.
Fg_wRiff_chunksizeH
riff_chunksizeH         DC        $0

Fg_wRiff_chunksizeL
riff_chunksizeL         DC        $0

Fg_wCktypeH
cktypeH                 DCB       'VAW'

Fg_wCktypeL
cktypeL                 DCB       'E'

; WAVE_FORMAT;
Fg_wWave_ckidH
wave_ckidH              DCB       'tmf'

Fg_wWave_ckidL
wave_ckidL              DCB       ' '
; Chunks are small for wave header
Fg_wWave_chunksize
wave_chunksize                  DC          $14

Fg_wFormattag
wformattag                      DC          $0011

Fg_wNchannels
nchannels                       DC          $1



; WARNING: This variable is shared with the Decoder, so it may get overwritten.
; EncNsamplespersec, below, is the persistent copy.  nsamplespersec gets
; set from EncNSamplesPerSec when setting up the encoder.
Fg_wNsamplespersec
nsamplespersec                  DC          DEFAULT_ADPCM_NSAMPLESPERSEC

Fg_wEncNSamplesPerSec
EncNSamplesPerSec               DC          DEFAULT_ADPCM_NSAMPLESPERSEC


Fg_wEncADCGain
EncAdcGain                      DC          DEFAULT_ADPCM_ADCGAIN



; EncInvSampleRateScale = 4096/nsamplespersec * 2^23
Fg_wEncInvSampleRateScale
EncInvSampleRateScale           DC          DEFAULT_ADPCM_INVSAMPLERATESCALE


; WARNING: This variable is shared with the Decoder, so it may get overwritten.
; EncNAvgBytesPerSec, below, is the persistent copy.  navgbytespersec gets
; set from EncNAvgBytesPerSec when setting up the encoder.
Fg_wNavgbytespersec
navgbytespersec                 DC          DEFAULT_ADPCM_NAVGBYTESPERSEC

Fg_wEncNAvgBytesPerSec
EncNAvgBytesPerSec              DC          DEFAULT_ADPCM_NAVGBYTESPERSEC 



Fg_wNblockalign
nblockalign                     DC          `512

Fg_wBitspersample
wbitspersample                  DC          $4

Fg_wCbsize
cbsize                          DC          $2

Fg_wSamplesperblock
wsamplesperblock                DC          ADPCM_BLOCK_SIZE*2-8+1
;EncClusterTimeScale             DC         (wsamplesperblock/nblockalign/nsamplespersec)*(2^32)
Fg_wEncClusterTimeScale
EncClusterTimeScale             DC          DEFAULT_ADPCM_CLUSTERTIMESCALE

; FACT_HEADER;
Fg_wFact_ckidH
fact_ckidH              DCB       'caf'

Fg_wFact_ckidL
fact_ckidL              DCB       't'
; Chunks are small for wave header
Fg_wFact_chunksize
fact_chunksize          DC        $4


Fg_wTimeL
timeL                   DC        $0

Fg_wTimeH
timeH                   DC        $0


;DATA_HEADER;
Fg_wData_ckidH
data_ckidH              DCB       'tad'

Fg_wData_ckidL
data_ckidL              DCB       'a'
; Chunksize can exceed 16 Mb, so provision more space.
Fg_wData_chunksizeL
data_chunksizeL         DC        $0

Fg_wData_chunksizeH
data_chunksizeH         DC        $0

; BYTES IN HEADER
Fg_wWave_header_size
wave_header_size        DC        $0

; LIST INFO header
Fg_wList_ckidH
list_ckidH              DCB       'SIL'

Fg_wList_ckidL
list_ckidL              DCB       'T'

Fg_wList_chunksize
list_chunksize          DC        $16

; INFO header
Fg_wInfo_ckidH
info_ckidH              DCB       'FNI'

Fg_wInfo_ckidL
info_ckidL              DCB       'O'

; INFO info
Fg_wIsft_ckidH
isft_ckidH              DCB       'FSI'

Fg_wIsft_ckidL
isft_ckidL              DCB       'T'

Fg_wIsft_chunksize
isft_chunksize         DC        $0a

; SIGMATEL heading
Fg_wSig_ckidH
sig_ckidH              DCB       'GIS'

Fg_wSig_ckidM
sig_ckidM              DCB       'TAM'

Fg_wSig_ckidL
sig_ckidL              DC        $004C45


; Skip value. Maintain constant skip for different bit rates
SecondstoSkip          DC        3
        endsec

