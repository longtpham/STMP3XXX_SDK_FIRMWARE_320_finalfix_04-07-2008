/////////////////////////////////////////////////////////////////////////////////
//  Copyright(C) SigmaTel, Inc. 2000-2004
//  Filename: project.inc
//  Description: 
/////////////////////////////////////////////////////////////////////////////////

#if (!defined(_PROJECT_INC))
#define _PROJECT_INC 1

#if defined(STMP_BUILD_PLAYER)
#include "hwequ.h"
#else 
//include "regscodec.inc"
#endif

//////////////////////////////////////////////////////////////////////////////////
// Uncomment next define to allow player boot if you have stfm1000 macro enabled (defaults ON) & 
// your board crystal is not able to output nominal voltage range. Used in player main.asm. STMP00012148
// Defining this will result in lower battery life (price of using a lower quality crystal). 
// Those that want to save battery life and have good crystals that meet our HW team's specs can 
// comment out this line to reduce crystal bias current and so battery current in player mode.  
#define SUPPORT_MARGINAL_XTALS 1

/////////////////////////////////////////////////////////////////////////////////
// BOOT SPEED CONFIG & ASSOCIATED BATTERY VOLTAGE CHECK ENABLE
/////////////////////////////////////////////////////////////////////////////////
// Users can enable one of ATTEMPT_FAST_BOOT or FASTEST boot or neither project define below by defining
// values as 0 or 1, else it defaults to the boot speed used in previous SDKs.
#define ATTEMPT_FAST_BOOT 1
#define ATTEMPT_FASTEST_BOOT 0   
// Changing this define to 0 removes Vbat threshholding & unconditionally gives fast boot config above- 
// disabling this check is suggested for profiling if you want to try to tweak down threshholds by maybe 50mV.
// Also the tests should clear the fast boot config defines to link & boot at normal speed.
#define SPEED_BOOT_BATTERY_CHECK 1

/////////////////////////////////////////////////////////////////////////////////
// MEDIA DEFINITIONS
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// MNAND - Number of chips forming the internal physical device (i.e. # of NANDs)
#if defined(NAND1)
#define SM_INTERNAL_CHIPS 1
#else 
#if defined(NAND2)
#define SM_INTERNAL_CHIPS 2
#else 
#if defined(NAND3)
#define SM_INTERNAL_CHIPS 3
#else 
#if defined(NAND4)
#define SM_INTERNAL_CHIPS 4
#else 
#define SM_INTERNAL_CHIPS 1
#endif
#endif
#endif
#endif

/////////////////////////////////////////////////////////////////////////////////
// If SMARTMEDIA_DETECT_ACTIVE_HIGH is defined:
//   SmartMedia will be detected as inserted if the GPIO pin reads 1 and removed if it reads 0.  
// If SMARTMEDIA_DETECT_ACTIVE_HIGH is not defined ** comment out next line **
//   SmartMedia will be detected as inserted if the GPIO pin reads 0 and removed if it reads 1.
//*** comment out if active high ****
//SMARTMEDIA_DETECT_ACTIVE_HIGH  equ     1

#if defined(SMEDIA)
#define NUM_REMOVABLE_MEDIA 1
#define NUM_SM_EXTERNAL 1
#define MMC_MAX_PHYSICAL_DEVICES 0
#define SM_MAX_LOGICAL_DEVICES 2
#else 
#if defined(MMC)
#define NUM_REMOVABLE_MEDIA 1
#define NUM_SM_EXTERNAL 0
#define MMC_MAX_PHYSICAL_DEVICES 1
#define SM_MAX_LOGICAL_DEVICES 1
#else 
#define NUM_REMOVABLE_MEDIA 0
#define NUM_SM_EXTERNAL 0
#define MMC_MAX_PHYSICAL_DEVICES 0
#define SM_MAX_LOGICAL_DEVICES 1
#endif
#endif

/////////////////////////////////////////////////////////////////////////////////
// Mass Storage Class definitions
/////////////////////////////////////////////////////////////////////////////////
// Set to 0 if Composite Device build is desired.    
#define MULTI_LUN_BUILD 1   

////////////////////////////////////////////////////////////////////////////////
//  SCSI
#if (MULTI_LUN_BUILD==0)
  #if (NUM_REMOVABLE_MEDIA == 1)
    #define SCSI_NUM_TARGETS                        2
    #define SCSI_DEVICE_NUM_LUNS_DEV_0              1
    #define SCSI_DEVICE_NUM_LUNS_DEV_1              1
  #else
    #define SCSI_NUM_TARGETS                        1
    #define SCSI_DEVICE_NUM_LUNS_DEV_0              1
  #endif
#else
    #define SCSI_NUM_TARGETS                        1
  #if (NUM_REMOVABLE_MEDIA == 1)
    #define SCSI_DEVICE_NUM_LUNS_DEV_0              2
  #else
    #define SCSI_DEVICE_NUM_LUNS_DEV_0              1  
  #endif
#endif


#define USBMSC_NUM_DEVICES                      SCSI_NUM_TARGETS


////////////////////////////////////////////////////////////////////////////////
// Define number of logical medias and drives for three builds (MTP, USBMSC, and Player)
////////////////////////////////////////////////////////////////////////////////
#ifdef MMC
#ifdef MTP_BUILD
// --------------------
// MTP and MMC
// --------------------
#define NUM_LOGICAL_MEDIA       2
#define NUM_LOGICAL_DRIVES      8
#else  // ifndef MTP_BUILD
#ifdef STMP_BUILD_PLAYER
// --------------------
// Player and MMC
// --------------------
#else
// --------------------
// USBMSC and MMC
// --------------------
#define NUM_LOGICAL_MEDIA       3
#define NUM_LOGICAL_DRIVES      8
#endif // ifdef STMP_BUILD_PLAYER
#endif // ifdef MTP_BUILD
#else  // ifndef MMC
#ifdef MTP_BUILD
// --------------------
// MTP and NAND only
// --------------------
#define NUM_LOGICAL_MEDIA       1
#define NUM_LOGICAL_DRIVES      7
#else  // ifndef MTP_BUILD
#ifdef STMP_BUILD_PLAYER
// --------------------
// Player and NAND only
// --------------------
#else
// --------------------
// USBMSC and NAND only
// --------------------
#define NUM_LOGICAL_MEDIA       2
#define NUM_LOGICAL_DRIVES      7
#endif // ifdef STMP_BUILD_PLAYER
#endif // ifdef MTP_BUILD
#endif // ifdef MMC 

// If we are in an MTP build then we don't use as many transfers buffers.
#if (defined(MTP_BUILD))
#define MAX_USB_TRANSFERS_QUEUED 16

////!
////! This varible holds the watchdog count for the store flush.
////!
///
#include <types.h>
extern volatile INT g_StoreWatchDogCount;
extern const INT g_StoreWatchDogTimeout;
#endif

////////////////////////////////////////////////////////////////////////////////
// These are needed here for Mass Storage Class
// Needs to be cleaned up
////////////////////////////////////////////////////////////////////////////////
#if (!defined(STMP_BUILD_PLAYER))
#define SCRATCH_USER_Y_SIZE 512
#define SCRATCH_USER_X_SIZE 512

#define BROWNOUT_LEVEL1_DETECTED                0x000001

#endif


/////////////////////////////////////////////////////////////////////////////////
// SmartMedia/NAND defs
#define SM_MAX_PHYSICAL_DEVICES SM_INTERNAL_CHIPS+NUM_SM_EXTERNAL
#define SM_REMOVABLE_DEVICE_NUM SM_INTERNAL_CHIPS

/////////////////////////////////////////////////////////////////////////////////
// Sysloadresources defs
#define SM_MAX_RESOURCE_DEVICES SM_INTERNAL_CHIPS+NUM_SM_EXTERNAL

/////////////////////////////////////////////////////////////////////////////////
// MMC defs
#define MMC_MAX_PARTITIONS 1
#define MMC_MAX_LOGICAL_DEVICES MMC_MAX_PHYSICAL_DEVICES*MMC_MAX_PARTITIONS

/////////////////////////////////////////////////////////////////////////////////
// SPI defs
#define SPI_MAX_DEVICES MMC_MAX_PHYSICAL_DEVICES

/////////////////////////////////////////////////////////////////////////////////
// Global media defs
#define MAX_PHYSICAL_DEVICES SM_MAX_PHYSICAL_DEVICES+MMC_MAX_PHYSICAL_DEVICES
#define MAX_LOGICAL_DEVICES SM_MAX_LOGICAL_DEVICES+MMC_MAX_LOGICAL_DEVICES

/////////////////////////////////////////////////////////////////////////////////
// DO NOT CHANGE THESE!!!
#define SM_MAX_PARTITIONS 4
#define MAX_HANDLES 2
/////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////
// Battery LRADC Values 
/////////////////////////////////////////////////////////////////////////////////
// brownout trip point in mV (moved by RS)
// BATT_SAFETY_MARGIN:  percentage value used by:
//   * SysSaveSettings to determine if enough power is left to attempt a settings save. 
//   * Recorder to conditionally prevent the start or continuation of 
//     audio recording to media.
#define BATT_SAFETY_MARGIN 10

//; player resource drive refresh allowed only when battery usable % is above this value. 
//; 50% of usable range [0.9V to 1.5V] is around 1.2V This is the safe target alkaline voltage to do a refresh.
//; LIION will also require 50% which will work but that could be changed in the future. 
//RESOURCE_REFRESH_MIN_BATT_PCT equ 50

// stmp00005071 adds these for mp3 encode stability at low battery per battery type & mmc presence.

//// Vbat threshholds empirically tuned by profiling max bat current for 3 HW cases: 
//// 1 GigaByte MMC, NAND flash only, & LiIon Buck mode. See voicemenu pre-encode Vbat check.
#if (!defined(CLCD))
#define MP3_ENC_MMC_MIN_V_BAT_MV 1100
#define MP3_ENC_NANDONLY_MIN_V_BAT_MV 1000
#else 
#define MP3_ENC_MMC_MIN_V_BAT_MV 1250
#define MP3_ENC_NANDONLY_MIN_V_BAT_MV 1150
#endif
#define MP3_ENC_BUCKMODE_MIN_V_BAT_MV 3300

// These Vbat use-range-percentages are derived to correspond with target battery 
// voltages of 1.10V for MMC, 1.0V for nand only (alkaline or nimh), and 3.3V for any LiIon.
// See mp3 encoder overlay.
#define MP3_ENC_MMC_MIN_V_BAT_RANGE_PCT 33
#define MP3_ENC_NANDONLY_MIN_V_BAT_RANGE_PCT 17
#define MP3_ENC_BUCKMODE_MIN_V_BAT_RANGE_PCT 25

/////////////////////////////////////////////////////////////////////////////////
// Voice recording filenames
// number of digits in filename Vxxx.wav
/////////////////////////////////////////////////////////////////////////////////
#define DIGITS_IN_VOICE_FILENAME 3   

/////////////////////////////////////////////////////////////////////////////////
// Mic Bias: Set this enable to 1 to internally generate mic bias voltage. 
// Also below, must configure internal resistor value and pin to use for DC bias. 
// Can set to 0 to disable internally generated mic bias voltage at pin lradc1/2.
#if defined(DEVICE_3500)
#define ENABLE_INTERNALLY_GENERATED_MICBIAS 1 
// MIC BIAS circuit source selection: use Low Resolution ADC pin 2 or 1 to bias mic. 
// Per your layout: select LRADC 2 or 1 below 
//   (pin LRADC 2 for 35xx cpu card revB, LRADC 1 for 35xx cpu card revC, ref schematics, & demo player)
#define LRADC_NUM_FOR_MIC_BIAS 1 
// Sets internal mic bias R to value 2, 4, or 8 (in kOhms).
#define MICBIAS_CONFIGURED_RESISTOR_KOHMS 8

// Allows mic bias to settle before sampling. This delay currently unused. For rec btn when bias not yet ready.
#define REC_BTN_INTERNAL_MIC_BIAS_SETTLING_DELAY_MS 400 
// Option always keeps bias enabled after mainmenu init. This allows for start pop prevention if you use rec button from outside voice menu.
// This selection costs 1 mWatt but gains the desired low record button to sampling latency in the record-from-music-menu use-case.
#define KEEP_MIC_BIAS_ENABLED 0

#else 
// STMP3410
#define ENABLE_INTERNALLY_GENERATED_MICBIAS 0
#define LRADC_NUM_FOR_MIC_BIAS 1 
#define MICBIAS_CONFIGURED_RESISTOR_KOHMS 2
#endif

/////////////////////////////////////////////////////////////////////////////////
// Number of available soft timers
/////////////////////////////////////////////////////////////////////////////////
#if defined(SYNC_LYRICS)
#define SOFT_TIMERS 10
#else 
#if defined(JPEG_DECODER)
#define SOFT_TIMERS 10
#else 
#define SOFT_TIMERS 9
#endif
#endif

/////////////////////////////////////////////////////////////////////////////////
//  sizes
/////////////////////////////////////////////////////////////////////////////////
#if defined(MMC)
#if defined(USE_PLAYLIST5)
#define MENU_STACK_SIZE 1500
#else 
#define MENU_STACK_SIZE 1250
#endif //if @def('USE_PLAYLIST5')
#else 
#if defined(USE_PLAYLIST5)
#define MENU_STACK_SIZE 1500
#else 
#define MENU_STACK_SIZE 1250
#endif //if @def('USE_PLAYLIST5')
#endif //if @def('MMC')

// NOTE: SDK2.6 USED 550 FOR NEXT LINE, BUT SDK3.0BETA USED 750. Should it be ifdef MTP 750 else 550 for other builds? TOVERIFY. MYALLOC
// 
#define STACK_L1_SIZE 750
#define STACK_L2_SIZE 100
#define STACK_L3_SIZE 160

// If we are in MTP mode the overlay task stack can shrink.
// TODO shrink this stack for MTP, I will leave it at 500 until we verify that it 
// is ok with switching code.
#if defined(MTP_BUILD)
#define OVERLAY_MANAGER_STACK_SIZE 300
#endif

/////////////////////////////////////////////////////////////////////////////////
// maximum number of nested funclets 
/////////////////////////////////////////////////////////////////////////////////
#define MAX_NESTED_FUNCLET 6 

/////////////////////////////////////////////////////////////////////////////////
//    LCD DEFINITIONS
/////////////////////////////////////////////////////////////////////////////////

#define SPACE_CHAR 0x000020          
#define ZERO_CHAR 0x000030
#define COLON_CHAR 0x00003A
#define PERIOD_CHAR 0x00002E

#if (defined(S6B33B0A_LCD))
#define LCD_X_SIZE 128
#define LCD_Y_SIZE 159
#endif

#if (defined(SED15XX_LCD))
#define LCD_X_SIZE 128
#define LCD_Y_SIZE 64
#endif


//////////////////////////////////////////////////////////////////////////////////
//   Details on Customizing Contrast
/////////////////////////////////////////////////////////////////////////////////
//   Max supported LCD range is 0 - 3F; however due to hardware/voltage differences
//   the range of visibility is usually smaller than this.  It is important to 
//   calibrate the visible range, because the contrast setting is saved.
//   If the user shuts off the player while lcd is not visible, the player is useless
//   unless the ezact sequence is remembered.
//   To find out what range your player supports: 
//   change these equs to full range or comment out (full range is default)
//;;;;;;
// uncomment the line below to build code that will provide raw contrast value
// recommended calibration using player -- uncomment 
//;;;;;;
//CONTRAST_CALIBRATION    equ  1
////////////////////////////
#if (defined(DEMO_HW))
// this is the setting for ET301 demos; Next 2 line have not been updated for new Shingyi LCD (June6'05)
#define LCD_MAX_CONTRAST 0x32 
#define LCD_MIN_CONTRAST 0x1E
#else 

#if (defined(S6B33B0A_LCD))
#define LCD_MAX_CONTRAST 210
#define LCD_MIN_CONTRAST 160	
#endif

#if (defined(SED15XX_LCD))
// Next 3 line comment was for OLD shingyi LCD: (default LCD before sdk2.6)
// Engineering board regs support range [17-37].
//   Engineering board DC/DC support range [24-46]. 
//   One default contrast range [24-42] works for both.
// The 3 sets of contrast ranges below are updated for SDK2.6 to support either 
// new or old ShingYih LCDs. "File" refers to \resource\shingyih\system_lcd_init_seq.src
// 3/10/05 - Use one of the 2 next contrast ranges depending on which
// ShingYih LCD you have.  See \resources\shingyi\readme.txt 
// Optimal for NEW LCD with NEW file (SDK2.6 default)  
// G098064-41 LCD module (present on engr board revH LCD card)

#if (defined(NEWSHINGYIH))
#define LCD_MAX_CONTRAST 250
#define LCD_MIN_CONTRAST 0
#else 
//-----
// Near optimal for OLD LCD with NEW file. 
// NOT optimal for the new lcd but pretty good. So u may use this if u want 1 build for both LCDs.
#define LCD_MAX_CONTRAST 250
#define LCD_MIN_CONTRAST 0

//=====
// Optimal for OLD ShingYih LCD with OLD file (as sdk2.521)
// Suggest going with default new init file & values above for your LCD instead of this historic ver.
//LCD_MAX_CONTRAST equ 42
//LCD_MIN_CONTRAST equ 24 

#endif
#endif

#endif

//////////////////////////////////////////////////////////////////////////////////
// The default value of the lcd contrast in % of range
//   the default value is used when no settings.dat is available
//////////////////////////////////////////////////////////////////////////////////

#if (defined(S6B33B0A_LCD))
// 60% of range is default value
#define DEFAULT_CONTRAST 50 
#endif

#if (defined(SED15XX_LCD))
// % of range is default value (was 60%)
#define DEFAULT_CONTRAST 50 
#endif


// percentage change per increment/decrement message (LCD_DEC_CONTRAST/LCD_INC_CONTRAST)
// make lower when doing calibration
#define LCD_STEPSIZE_CONTRAST 10  


/////////////////////////////////////////////////////////////////////////////////
// For FFWD and RWND
/////////////////////////////////////////////////////////////////////////////////
#define SECONDS_TO_SKIP 1
#define SECONDS_TO_SKIP1 3
#define SECONDS_TO_SKIP2 6
// number of seconds to cause reset to begin of song for PREV push
#define PREV_SONG_THRESHOLD 5  
// number of 300 mSec periods before the numbers of seconds to skip increases
#define FIRST_TIME_BOUNDARY 15 
// number of 300 mSec periods before the numbers of seconds to skip increases
#define SECOND_TIME_BOUNDARY 30 

// For audible FFW/RWD
#define SECONDS_TO_SKIP_FOR_3X_RATE 1
#define SECONDS_TO_SKIP_FOR_60X_RATE 18
#define SECONDS_TO_SKIP_FOR_600X_RATE 180
#define SECONDS_TO_SKIP_FOR_1800X_RATE 540
// number of 300 mSec periods before the numbers of seconds to skip increases
#define LEVEL1_BOUNDARY 17 
// number of 300 mSec periods before the numbers of seconds to skip increases
#define LEVEL2_BOUNDARY 33 
// number of 300 mSec periods before the numbers of seconds to skip increases
#define LEVEL3_BOUNDARY 50 
// Stmp00010296 Ticket #71685:  for song shorter than the min. skip size
// if song_time < skip_size, bCurrentSongShort=>TRUE (ignore FWD/RWD commands)
// Short Song Time, songs too short to play.
#define SHORT_SONG_TIME SECONDS_TO_SKIP 	

/////////////////////////////////////////////////////////////////////////////////
// MP3 Sync Values
/////////////////////////////////////////////////////////////////////////////////
// # bytes to look for sync before marking it bad
#define MP3_SYNC_THRESHOLD 70000 
// # bytes to look for sync before slowing decoder isr frequency (to let menus run)
#define MP3_SYNC_THRESHOLD1 10000 
// once we have sync'd, the isr should be called this frequently
#define MP3_DECODERISR_FAST 7500  
// if decoder is having difficulty syncing, switch isr to be called less frequently
#define MP3_DECODERISR_SLOW 50000 


/////////////////////////////////////////////////////////////////////////////////
//// Multi-Stage Volume Control Definitions
/////////////////////////////////////////////////////////////////////////////////
//// Use Multi-Stage Volume
#define MULTI_STAGE_VOLUME 0x1                  

//// Master Volume definitions
#define MIX_MSTR_MAX_VOL 0x0
#define NUM_MSTR_ATT_STEPS (0x1F-MIX_MSTR_MAX_VOL)

//// DAC-Mode definitions
//// Adjusts 0dB point
#define MIX_DAC_NOM_VOL 0x6                  
#define MIX_DAC_MIN_VOL 0x1F
// For minimum clipping: Set MIX_DAC_MAX_VOL below to match MIX_DAC_NOM_VOL value 2 lines above.
// For additional gain (with possible clipping): Set MIX_DAC_MAX_VOL in range below. 
//                                               Each integer below MIX_DAC_NOM_VOL provides 1.5 dB gain on Stmp34x0; 
//                                               Max gain possible: 8 step diff would provide +12dB gain.   
// range: [0 to MIX_DAC_NOM_VOL] (0 for loudest volume)
#define MIX_DAC_MAX_VOL 0x0  

#define NUM_DAC_ATT_STEPS (MIX_DAC_MIN_VOL-MIX_DAC_NOM_VOL+1)
#define NUM_DAC_GAIN_STEPS (MIX_DAC_NOM_VOL-MIX_DAC_MAX_VOL)
#define NUM_DAC_MODE_VOL_STEPS (NUM_DAC_ATT_STEPS+NUM_MSTR_ATT_STEPS+NUM_DAC_GAIN_STEPS+1)

//// If there is no stored volume, set to 50% of DAC-Mode max; change if DAC Mode is not the default
//// STMP35xx has 2dB attenuation per step so default needs a few more steps above midpoint.
#define DEFAULT_VOLUME ((NUM_DAC_MODE_VOL_STEPS/2)+6)         

//// Set maximum restored volume to 75% of DAC-Mode max; change if DAC Mode is not the default
#define MAX_RESTORED_VOLUME ((3*NUM_DAC_MODE_VOL_STEPS)/4)   


//// Line In definitions (used for Line-In 1)
//// 0dB point of the Line In
#define MIX_LINE_NOM_VOL 0x8                  
//// Minimum volume of Line In
#define MIX_LINE_MIN_VOL 0x1F                 
//// Maximum volume of Line In (can adjust extra gain)
#define MIX_LINE_MAX_VOL 0x6                  
#define NUM_LINE_ATT_STEPS (MIX_LINE_MIN_VOL-MIX_LINE_NOM_VOL+1)
#define NUM_LINE_GAIN_STEPS (MIX_LINE_NOM_VOL-MIX_LINE_MAX_VOL)
#define NUM_LINE_MODE_VOL_STEPS (NUM_LINE_ATT_STEPS+NUM_MSTR_ATT_STEPS+NUM_LINE_GAIN_STEPS+1)

//// Line In definitions (used for FM tuner with 144 pin package)
//// 0dB point of the Line In
#define MIX_FM_NOM_VOL 0x8                  
//// Minimum volume of Line In
#define MIX_FM_MIN_VOL 0x1F                 
//// Maximum volume of Line In (can adjust extra gain)
#define MIX_FM_MAX_VOL 0x6                  
#define NUM_FM_ATT_STEPS (MIX_FM_MIN_VOL-MIX_FM_NOM_VOL+1)
#define NUM_FM_GAIN_STEPS (MIX_FM_NOM_VOL-MIX_FM_MAX_VOL)
#define NUM_FM_MODE_VOL_STEPS (NUM_FM_ATT_STEPS+NUM_MSTR_ATT_STEPS+NUM_FM_GAIN_STEPS+1)

/////////////////////////////////////////////////////////////////////////////////
//// When enabled (1), periodically checks for one of several FM state machine invalid deadlock states & recovers. 
//// Define as 0 to disable FM-deadlock-state checking and recovery.
#define FM_WATCHDOG_ENABLE 1

#if !defined(STMP_BUILD_PLAYER)
////
////! This varible holds the lcd display state for the mtp project.
////
///
#include <types.h>
extern volatile WORD g_wActivityState;
#endif // if !@def('STMP_BUILD_PLAYER')

void _reentrant Init5VSense(void);
void _reentrant ServiceDCDC(void);

////////////////////////////////////////////////////////////////////////////
//// JPEG Thumbnail Mode Setting
//// number of column in thumbnail mode
#define THUMBNAIL_X 2		
//// number of row in  thumbnail mode
#define THUMBNAIL_Y 2		
//// thumbnail boundary offset x
#define THUMBNAIL_X_OFFSET 4		
//// thumbnail boundary offset y
#define THUMBNAIL_Y_OFFSET 4		

#endif // if (!@def(_PROJECT_INC))

