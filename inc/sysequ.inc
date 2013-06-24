;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000
;
; System defs
;///////////////////////////////////////////////////////////////////////////////

    IF (!@def(SYSEQU_INC))
SYSEQU_INC equ 1

SOFTTIMER_CONTINUOUS_EVENT equ  0


HOLD_BUTTON_REPORTED_BITMASK  equ  4


; Decoder types
DECODER_TYPE_MP3        equ     0
DECODER_TYPE_WMA        equ     1
DECODER_TYPE_AAC        equ     2
DECODER_TYPE_ADPCM_IMA  equ     3
DECODER_TYPE_ADPCM_MS   equ     4
DECODER_TYPE_PCM        equ     5
DECODER_TYPE_STFM       equ     6

; Encoder types
ENCODER_TYPE_ADPCM_IMA  equ     0
ENCODER_TYPE_PCM        equ     1
ENCODER_TYPE_MP3        equ     2

; EncoderSR/EncoderCSR bit equates.  Also uses Stop & Play below.
EncNotEnoughSpace       equ     0
EncRanOutOfSpace        equ     1
EncRecordError          equ     2
EncToldToStop           equ     3
EncAlreadyRecording  equ     4
; ENCODER_STOPPED       equ     6   ; As RecorderStateMachine.h
EncWarnLowSpace         equ     7
EncWarningSent          equ     8
; ENCODER_RECORDING     equ     12

; DecoderSR/DecoderCSR bit equates
DecoderIgnoreMessages  equ     0   ;  This bit is set when the decoder sends a message to the parser and is 
                                    ;  waiting for a response.
VBRFlag              equ     1
RepeatSong           equ     2
RepeatAll            equ     3
Random               equ     4
Pause                equ     5
Stop                 equ     6
Rwnd                 equ     7
Ffwd                 equ     8
TimeMode             equ     9
DecSync              equ     10    ; set when Fstatus_o = 1 for mp3 
Play                 equ     12
EndOfSong            equ     13    ; starting with sdk2.105: changed to set when decoder done instead of when file reads done 
EndOfList            equ     14
SongInfo             equ     15
FileIsOpen           equ     16    ; set when a song file is currently opened by the decoder
SkipBlockHeader      equ     17    ; used by parser
ABMode_A             equ     18
ABMode_B             equ     19
ABQuiet              equ     20
BadFile              equ     21    ; set if bad file encountered
SyncWait             equ     22    ; mp3 decoder looking for sync
FileReadError        equ     23    ; error reading file, duh

; DecoderCSR2 bit equates
EndOfFileReached	 equ	 0  ;Set when EOF reached, prevent freads beyond EOF 
TimeNeedsUpdate      equ     1  ; Set when need to allow LCD to update time


; ParserVoiceCSR bits (specific to voice files)
RepeatOneFile        equ     0     ; repeat playing current file
RepeatAllFiles       equ     1     ; continuously loop through files
LoopOnce             equ     2     ; start at current file, play all files, then stop

; DecAdpcmFlags bits (used by ADPCM decoders)
WaveHeaderFound      equ     0       ; Set to '1' when header unpacked, cleared during StopCurrentSong

  if (@def('TRACEBUF_EN'))
; Trace buffer words.  Not ENabled by default.
TRACE_BUFF_SIZE         equ     256
TRACE_BUFF_MODULO       equ     TRACE_BUFF_SIZE-1 
  endif
    
; Scratch area stuff
 if @def('STMP_BUILD_PLAYER')
SCRATCH_USER_Y_SIZE 		equ 512
SCRATCH_USER_X_SIZE 		equ 256
 else
   if @def('FAT32TEST')
SCRATCH_USER_Y_SIZE 		equ 683
SCRATCH_USER_X_SIZE 		equ 128
   else
SCRATCH_USER_Y_SIZE 		equ 128
SCRATCH_USER_X_SIZE 		equ 128
   endif
 endif

SCRATCH_USER_X_SIZE_BYTES   equ     SCRATCH_USER_X_SIZE*3
SCRATCH_USER_X_MODULO       equ     SCRATCH_USER_X_SIZE-1
SCRATCH_USER_Y_SIZE_BYTES   equ     SCRATCH_USER_Y_SIZE*3
SCRATCH_USER_Y_MODULO       equ     SCRATCH_USER_Y_SIZE-1

; Boot ROM execute location
BootExecuteCommand      equ     $012B

BOOT_USB            equ     $000002
BOOT_SMARTMEDIA     equ     $000004
BOOT_COMPACTFLASH   equ     $000005
BOOT_I2C_SLAVE      equ     $000009
BOOT_I2C_MASTER     equ     $00000A
BOOT_SPI_SLAVE      equ     $00000B
BOOT_SPI_MASTER     equ     $00000C
BOOT_HOSTPORT       equ     $00000D
BOOT_BURNIN         equ     $00000F

; Numerical constants
;PI              equ     3.141592654

; Buffer Descriptor offsets
BD_BufferBaseAddress    equ   0
BD_BufferModulo         equ   1
BD_BufferSize           equ   2
BD_HdPtr                equ   3
BD_TailPtr              equ   4

; Size of a buffer descriptor
BUFF_DESC_SIZE          equ   5

; Mp3 Song Table Descriptor Offsets
MST_ELEMENT_COUNT       equ   7        ; number of elements for each song
MST_CARD_NUMBER         equ   0
MST_ADDR_LOW            equ   1
MST_ADDR_HIGH           equ   2
MST_BYTES_LOW           equ   3
MST_BYTES_HIGH          equ   4
MST_SAMPLE_RATE         equ   5
MST_SONG_TYPE           equ   6

; Data Format Descriptor offsets
DF_NumChannels          equ   0
DF_SampleRate           equ   1
DF_BitsPerSample        equ   2


; Timer Descriptor Offset
TD_TimerNumber          equ   0
TD_ControlValue         equ   1
TD_CountValue           equ   2
TD_ControlRegister      equ   3
TD_CountRegister        equ   4


; File IO defines
FileInPort              equ   $8000
FileOutPort             equ   $8001
OnceInPort              equ   $8000
OnceOutPort             equ   $8001
SmFakeInPort            equ   $8002
SmFakeOutPort           equ   $8003
UsbFakeInPort           equ   $8004
UsbFakeOutPort          equ   $8005

; Target memory stuff
TARGET_MEM_MASK     equ     $F00000
TARGET_MEM_X        equ     $800000
TARGET_MEM_Y        equ     $400000
TARGET_MEM_L        equ     $200000
TARGET_MEM_P        equ     $100000

TARGET_MEM_X_BITPOS equ     23
TARGET_MEM_Y_BITPOS equ     22
TARGET_MEM_L_BITPOS equ     21
TARGET_MEM_P_BITPOS equ     20


; File System Stuff
MaxDirCount         equ     13         ; assume 3 deep max with 8.3 format (38 bytes inc. /'s)

; Error Codes
;	Value		Error
;	--------------------------------------------
;	$000000		Reserved - no error
;	$000001		SmartMedia Bad Programming Error
;	$000002		SmartMedia Read Timeout
;	$000003		USB PLL Lock Timeout
;       $000010         GetNumSongs returned an error
;       $000011         fopen failed to open file number (in x:TrackNum)
;       $000012         fread failed when attempting to read page
;       $000013         fread failed to read correct number of bytes
NO_ERROR                    equ     $000000
ERROR_PLL_LOCK_TIMEOUT      equ     $000001

ERROR_SM_BADP               equ     $000100
ERROR_SM_TIMEOUT            equ     $000101
ERROR_SM_INVALID_BLOCK      equ     $000102
ERROR_SM_INVALID_DATA       equ     $000103
ERROR_SM_ECC                equ     $000104
ERROR_SM_BLOCK_ERASE_FAILED equ     $000105
ERROR_SM_CARD_NOT_FOUND     equ     $000106
ERROR_SM_BUSY_TIMEOUT       equ     $000107
ERROR_SM_WRITE_FAILED       equ     $000108
ERROR_SM_DEVICE_ID          equ     $000109

ERROR_NUM_SONGS             equ     $000200
ERROR_FOPEN_FILENUM         equ     $000201
ERROR_FREAD_PAGE            equ     $000202
ERROR_FREAD_BYTE_COUNT      equ     $000203
ERROR_CREATING_VOICE_DIR    equ     $000204 

ERROR_USB_DISCONNECTED      equ     $000300

ERROR_DECODER_PLAYING       equ     $000400
ERROR_DECODER_SETPOS        equ     $000401             ; Attempted to set position past the end of file

; PLL defs
; According to Mike May, the PLL takes about 200us to stabalize, worst-case.
;   PLL lock delay is $000000001388 = 5000. We're always using the xtal at
;   24.576MHz, so this delay is 5000/(24.576MHz) = 203.5us.  We'll do this 5000
;   times for a total of 1.017s.
PLL_LOCK_DELAY_HIGH     equ   $000000
PLL_LOCK_DELAY_LOW      equ   $001388
PLL_TIMEOUT             equ   $001388

; Generic wait defs
WAIT_10_MS      equ     245760/2

;Those equates are used in the DCC/USBMSC
;Low battery detection bit
LOW_BATTERY_DETECTED_BITPOS	equ 0

    ENDIF   ; IF (!@def(SYSEQU_INC))

