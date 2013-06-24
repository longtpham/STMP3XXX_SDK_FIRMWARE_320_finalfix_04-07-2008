/////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000
// System defs
/////////////////////////////////////////////////////////////////////////////////

#if (!defined(SYSEQU_INC))
#define SYSEQU_INC 1

#define SOFTTIMER_CONTINUOUS_EVENT 0


#define HOLD_BUTTON_REPORTED_BITMASK 4


// Decoder types
#define DECODER_TYPE_MP3 0
#define DECODER_TYPE_WMA 1
#define DECODER_TYPE_AAC 2
#define DECODER_TYPE_ADPCM_IMA 3
#define DECODER_TYPE_ADPCM_MS 4
#define DECODER_TYPE_PCM 5
#define DECODER_TYPE_STFM 6

// Encoder types
#define ENCODER_TYPE_ADPCM_IMA 0
#define ENCODER_TYPE_PCM 1
#define ENCODER_TYPE_MP3 2

// EncoderSR/EncoderCSR bit equates.  Also uses Stop & Play below.
#define EncNotEnoughSpace 0
#define EncRanOutOfSpace 1
#define EncRecordError 2
#define EncToldToStop 3
#define EncAlreadyRecording 4
// ENCODER_STOPPED       equ     6   ; As RecorderStateMachine.h
#define EncWarnLowSpace 7
#define EncWarningSent 8
// ENCODER_RECORDING     equ     12

// DecoderSR/DecoderCSR bit equates
//  This bit is set when the decoder sends a message to the parser and is 
#define DecoderIgnoreMessages 0   
//  waiting for a response.
#define VBRFlag 1
#define RepeatSong 2
#define RepeatAll 3
#define Random 4
#define Pause 5
#define Stop 6
#define Rwnd 7
#define Ffwd 8
#define TimeMode 9
// set when Fstatus_o = 1 for mp3 
#define DecSync 10    
#define Play 12
// starting with sdk2.105: changed to set when decoder done instead of when file reads done 
#define EndOfSong 13    
#define EndOfList 14
#define SongInfo 15
// set when a song file is currently opened by the decoder
#define FileIsOpen 16    
// used by parser
#define SkipBlockHeader 17    
#define ABMode_A 18
#define ABMode_B 19
#define ABQuiet 20
// set if bad file encountered
#define BadFile 21    
// mp3 decoder looking for sync
#define SyncWait 22    
// error reading file, duh
#define FileReadError 23    

// DecoderCSR2 bit equates
//Set when EOF reached, prevent freads beyond EOF 
#define EndOfFileReached 0  
// Set when need to allow LCD to update time
#define TimeNeedsUpdate 1  


// ParserVoiceCSR bits (specific to voice files)
// repeat playing current file
#define RepeatOneFile 0     
// continuously loop through files
#define RepeatAllFiles 1     
// start at current file, play all files, then stop
#define LoopOnce 2     

// DecAdpcmFlags bits (used by ADPCM decoders)
// Set to '1' when header unpacked, cleared during StopCurrentSong
#define WaveHeaderFound 0       

#if (defined(TRACEBUF_EN))
// Trace buffer words.  Not ENabled by default.
#define TRACE_BUFF_SIZE 256
#define TRACE_BUFF_MODULO TRACE_BUFF_SIZE-1 
#endif

// Scratch area stuff
#if defined(STMP_BUILD_PLAYER)
#define SCRATCH_USER_Y_SIZE 512
#define SCRATCH_USER_X_SIZE 256
#else 
#if defined(FAT32TEST)
#define SCRATCH_USER_Y_SIZE 683
#define SCRATCH_USER_X_SIZE 128
#else 
#define SCRATCH_USER_Y_SIZE 128
#define SCRATCH_USER_X_SIZE 128
#endif
#endif

#define SCRATCH_USER_X_SIZE_BYTES SCRATCH_USER_X_SIZE*3
#define SCRATCH_USER_X_MODULO SCRATCH_USER_X_SIZE-1
#define SCRATCH_USER_Y_SIZE_BYTES SCRATCH_USER_Y_SIZE*3
#define SCRATCH_USER_Y_MODULO SCRATCH_USER_Y_SIZE-1

// Boot ROM execute location
#define BootExecuteCommand 0x012B

#define BOOT_USB 0x000002
#define BOOT_SMARTMEDIA 0x000004
#define BOOT_COMPACTFLASH 0x000005
#define BOOT_I2C_SLAVE 0x000009
#define BOOT_I2C_MASTER 0x00000A
#define BOOT_SPI_SLAVE 0x00000B
#define BOOT_SPI_MASTER 0x00000C
#define BOOT_HOSTPORT 0x00000D
#define BOOT_BURNIN 0x00000F

// Numerical constants
//PI              equ     3.141592654

// Buffer Descriptor offsets
#define BD_BufferBaseAddress 0
#define BD_BufferModulo 1
#define BD_BufferSize 2
#define BD_HdPtr 3
#define BD_TailPtr 4

// Size of a buffer descriptor
#define BUFF_DESC_SIZE 5

// Mp3 Song Table Descriptor Offsets
// number of elements for each song
#define MST_ELEMENT_COUNT 7        
#define MST_CARD_NUMBER 0
#define MST_ADDR_LOW 1
#define MST_ADDR_HIGH 2
#define MST_BYTES_LOW 3
#define MST_BYTES_HIGH 4
#define MST_SAMPLE_RATE 5
#define MST_SONG_TYPE 6

// Data Format Descriptor offsets
#define DF_NumChannels 0
#define DF_SampleRate 1
#define DF_BitsPerSample 2


// Timer Descriptor Offset
#define TD_TimerNumber 0
#define TD_ControlValue 1
#define TD_CountValue 2
#define TD_ControlRegister 3
#define TD_CountRegister 4


// File IO defines
#define FileInPort 0x8000
#define FileOutPort 0x8001
#define OnceInPort 0x8000
#define OnceOutPort 0x8001
#define SmFakeInPort 0x8002
#define SmFakeOutPort 0x8003
#define UsbFakeInPort 0x8004
#define UsbFakeOutPort 0x8005

// Target memory stuff
#define TARGET_MEM_MASK 0xF00000
#define TARGET_MEM_X 0x800000
#define TARGET_MEM_Y 0x400000
#define TARGET_MEM_L 0x200000
#define TARGET_MEM_P 0x100000

#define TARGET_MEM_X_BITPOS 23
#define TARGET_MEM_Y_BITPOS 22
#define TARGET_MEM_L_BITPOS 21
#define TARGET_MEM_P_BITPOS 20


// File System Stuff
// assume 3 deep max with 8.3 format (38 bytes inc. /'s)
#define MaxDirCount 13         

// Error Codes
//	Value		Error
//	--------------------------------------------
//	$000000		Reserved - no error
//	$000001		SmartMedia Bad Programming Error
//	$000002		SmartMedia Read Timeout
//	$000003		USB PLL Lock Timeout
//       $000010         GetNumSongs returned an error
//       $000011         fopen failed to open file number (in x:TrackNum)
//       $000012         fread failed when attempting to read page
//       $000013         fread failed to read correct number of bytes
#define NO_ERROR 0x000000
#define ERROR_PLL_LOCK_TIMEOUT 0x000001

#define ERROR_SM_BADP 0x000100
#define ERROR_SM_TIMEOUT 0x000101
#define ERROR_SM_INVALID_BLOCK 0x000102
#define ERROR_SM_INVALID_DATA 0x000103
#define ERROR_SM_ECC 0x000104
#define ERROR_SM_BLOCK_ERASE_FAILED 0x000105
#define ERROR_SM_CARD_NOT_FOUND 0x000106
#define ERROR_SM_BUSY_TIMEOUT 0x000107
#define ERROR_SM_WRITE_FAILED 0x000108
#define ERROR_SM_DEVICE_ID 0x000109

#define ERROR_NUM_SONGS 0x000200
#define ERROR_FOPEN_FILENUM 0x000201
#define ERROR_FREAD_PAGE 0x000202
#define ERROR_FREAD_BYTE_COUNT 0x000203
#define ERROR_CREATING_VOICE_DIR 0x000204 

#define ERROR_USB_DISCONNECTED 0x000300

#define ERROR_DECODER_PLAYING 0x000400
// Attempted to set position past the end of file
#define ERROR_DECODER_SETPOS 0x000401             

// PLL defs
// According to Mike May, the PLL takes about 200us to stabalize, worst-case.
//   PLL lock delay is $000000001388 = 5000. We're always using the xtal at
//   24.576MHz, so this delay is 5000/(24.576MHz) = 203.5us.  We'll do this 5000
//   times for a total of 1.017s.
#define PLL_LOCK_DELAY_HIGH 0x000000
#define PLL_LOCK_DELAY_LOW 0x001388
#define PLL_TIMEOUT 0x001388

// Generic wait defs
#define WAIT_10_MS 245760/2

//Those equates are used in the DCC/USBMSC
//Low battery detection bit
#define LOW_BATTERY_DETECTED_BITPOS 0

#endif // IF (!@def(SYSEQU_INC))


