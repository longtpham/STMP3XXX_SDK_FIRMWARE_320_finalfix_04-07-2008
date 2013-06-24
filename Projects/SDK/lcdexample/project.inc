;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2004
;
;  Filename: project.inc
;  Description: 
;///////////////////////////////////////////////////////////////////////////////

 if (!@def(_PROJECT_INC))
_PROJECT_INC                equ     1

 if @def('STMP_BUILD_PLAYER')
    include "hwequ.inc"
 else
    ;include "regscodec.inc"
 endif

;////////////////////////////////////////////////////////////////////////////////
; Uncomment next define to allow player boot if you have stfm1000 macro enabled (defaults ON) & 
; your board crystal is not able to output nominal voltage range. Used in player main.asm. STMP00012148
; Defining this will result in lower battery life (price of using a lower quality crystal). 
; Those that want to save battery life and have good crystals that meet our HW team's specs can 
; comment out this line to reduce crystal bias current and so battery current in player mode.  
SUPPORT_MARGINAL_XTALS equ 1

;///////////////////////////////////////////////////////////////////////////////
; BOOT SPEED CONFIG & ASSOCIATED BATTERY VOLTAGE CHECK ENABLE
;///////////////////////////////////////////////////////////////////////////////
; Users can enable one of ATTEMPT_FAST_BOOT or FASTEST boot or neither project define below by defining
; values as 0 or 1, else it defaults to the boot speed used in previous SDKs.
ATTEMPT_FAST_BOOT    equ 1
ATTEMPT_FASTEST_BOOT equ 0   
; Changing this define to 0 removes Vbat threshholding & unconditionally gives fast boot config above- 
; disabling this check is suggested for profiling if you want to try to tweak down threshholds by maybe 50mV.
; Also the tests should clear the fast boot config defines to link & boot at normal speed.
SPEED_BOOT_BATTERY_CHECK    equ 1

;///////////////////////////////////////////////////////////////////////////////
; MEDIA DEFINITIONS
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
; MNAND - Number of chips forming the internal physical device (i.e. # of NANDs)
    if @def('NAND1')
SM_INTERNAL_CHIPS           equ     1
    else
        if @def('NAND2')
SM_INTERNAL_CHIPS           equ     2
        else
            if @def('NAND3')
SM_INTERNAL_CHIPS           equ     3
            else
                if @def('NAND4')
SM_INTERNAL_CHIPS           equ     4
                else
SM_INTERNAL_CHIPS           equ     1
                endif
            endif
        endif
    endif

;///////////////////////////////////////////////////////////////////////////////
; If SMARTMEDIA_DETECT_ACTIVE_HIGH is defined:
;   SmartMedia will be detected as inserted if the GPIO pin reads 1 and removed if it reads 0.  
; If SMARTMEDIA_DETECT_ACTIVE_HIGH is not defined ** comment out next line **
;   SmartMedia will be detected as inserted if the GPIO pin reads 0 and removed if it reads 1.
;*** comment out if active high ****
;SMARTMEDIA_DETECT_ACTIVE_HIGH  equ     1

    if @def('SMEDIA')
NUM_REMOVABLE_MEDIA         equ     1
NUM_SM_EXTERNAL             equ     1
MMC_MAX_PHYSICAL_DEVICES    equ     0
SM_MAX_LOGICAL_DEVICES      equ     2
    else
        if @def('MMC')
NUM_REMOVABLE_MEDIA         equ     1
NUM_SM_EXTERNAL             equ     0
MMC_MAX_PHYSICAL_DEVICES    equ     1
SM_MAX_LOGICAL_DEVICES      equ     1
        else
NUM_REMOVABLE_MEDIA         equ     0
NUM_SM_EXTERNAL             equ     0
MMC_MAX_PHYSICAL_DEVICES    equ     0
SM_MAX_LOGICAL_DEVICES      equ     1
        endif
    endif

;///////////////////////////////////////////////////////////////////////////////
; Mass Storage Class definitions
;///////////////////////////////////////////////////////////////////////////////
MULTI_LUN_BUILD             equ     1   ; Set to 0 if Composite Device build is desired.    

;c insert
;////////////////////////////////////////////////////////////////////////////////
;//  SCSI
;#if (MULTI_LUN_BUILD==0)
;  #if (NUM_REMOVABLE_MEDIA == 1)
;    #define SCSI_NUM_TARGETS                        2
;    #define SCSI_DEVICE_NUM_LUNS_DEV_0              1
;    #define SCSI_DEVICE_NUM_LUNS_DEV_1              1
;  #else
;    #define SCSI_NUM_TARGETS                        1
;    #define SCSI_DEVICE_NUM_LUNS_DEV_0              1
;  #endif
;#else
;    #define SCSI_NUM_TARGETS                        1
;  #if (NUM_REMOVABLE_MEDIA == 1)
;    #define SCSI_DEVICE_NUM_LUNS_DEV_0              2
;  #else
;    #define SCSI_DEVICE_NUM_LUNS_DEV_0              1  
;  #endif
;#endif
;
;
;#define USBMSC_NUM_DEVICES                      SCSI_NUM_TARGETS
;
;endC

;//////////////////////////////////////////////////////////////////////////////
; Define number of logical medias and drives for three builds (MTP, USBMSC, and Player)
;//////////////////////////////////////////////////////////////////////////////
;c insert 
;#ifdef MMC
;#ifdef MTP_BUILD
;// --------------------
;// MTP and MMC
;// --------------------
;#define NUM_LOGICAL_MEDIA       2
;#define NUM_LOGICAL_DRIVES      8
;#else  // ifndef MTP_BUILD
;#ifdef STMP_BUILD_PLAYER
;// --------------------
;// Player and MMC
;// --------------------
;#else
;// --------------------
;// USBMSC and MMC
;// --------------------
;#define NUM_LOGICAL_MEDIA       3
;#define NUM_LOGICAL_DRIVES      8
;#endif // ifdef STMP_BUILD_PLAYER
;#endif // ifdef MTP_BUILD
;#else  // ifndef MMC
;#ifdef MTP_BUILD
;// --------------------
;// MTP and NAND only
;// --------------------
;#define NUM_LOGICAL_MEDIA       1
;#define NUM_LOGICAL_DRIVES      7
;#else  // ifndef MTP_BUILD
;#ifdef STMP_BUILD_PLAYER
;// --------------------
;// Player and NAND only
;// --------------------
;#else
;// --------------------
;// USBMSC and NAND only
;// --------------------
;#define NUM_LOGICAL_MEDIA       2
;#define NUM_LOGICAL_DRIVES      7
;#endif // ifdef STMP_BUILD_PLAYER
;#endif // ifdef MTP_BUILD
;#endif // ifdef MMC 
;endC 

;
; If we are in an MTP build then we don't use as many transfers buffers.
;
 if (@def('MTP_BUILD'))
MAX_USB_TRANSFERS_QUEUED    equ     16

;//!
;//! This varible holds the watchdog count for the store flush.
;//!
;/
;C insert
;#include <types.h>
;extern volatile INT g_StoreWatchDogCount;
;extern const INT g_StoreWatchDogTimeout;
;endc
 endif

;//////////////////////////////////////////////////////////////////////////////
; These are needed here for Mass Storage Class
; Needs to be cleaned up
;//////////////////////////////////////////////////////////////////////////////
 if (!@def('STMP_BUILD_PLAYER'))
SCRATCH_USER_Y_SIZE  equ   512
SCRATCH_USER_X_SIZE  equ   512

;c insert
;#define BROWNOUT_LEVEL1_DETECTED                0x000001
;endC

 endif


;///////////////////////////////////////////////////////////////////////////////
; SmartMedia/NAND defs
SM_MAX_PHYSICAL_DEVICES     equ     SM_INTERNAL_CHIPS+NUM_SM_EXTERNAL
SM_REMOVABLE_DEVICE_NUM     equ     SM_INTERNAL_CHIPS

;///////////////////////////////////////////////////////////////////////////////
; Sysloadresources defs
SM_MAX_RESOURCE_DEVICES     equ     SM_INTERNAL_CHIPS+NUM_SM_EXTERNAL

;///////////////////////////////////////////////////////////////////////////////
; MMC defs
MMC_MAX_PARTITIONS          equ     1
MMC_MAX_LOGICAL_DEVICES     equ     MMC_MAX_PHYSICAL_DEVICES*MMC_MAX_PARTITIONS

;///////////////////////////////////////////////////////////////////////////////
; SPI defs
SPI_MAX_DEVICES             equ     MMC_MAX_PHYSICAL_DEVICES

;///////////////////////////////////////////////////////////////////////////////
; Global media defs
MAX_PHYSICAL_DEVICES        equ     SM_MAX_PHYSICAL_DEVICES+MMC_MAX_PHYSICAL_DEVICES
MAX_LOGICAL_DEVICES         equ     SM_MAX_LOGICAL_DEVICES+MMC_MAX_LOGICAL_DEVICES

;///////////////////////////////////////////////////////////////////////////////
; DO NOT CHANGE THESE!!!
SM_MAX_PARTITIONS           equ     4
MAX_HANDLES                 equ     2
;///////////////////////////////////////////////////////////////////////////////


;///////////////////////////////////////////////////////////////////////////////
; Battery LRADC Values 
;///////////////////////////////////////////////////////////////////////////////
; brownout trip point in mV (moved by RS)
; BATT_SAFETY_MARGIN:  percentage value used by:
;   * SysSaveSettings to determine if enough power is left to attempt a settings save. 
;   * Recorder to conditionally prevent the start or continuation of 
;     audio recording to media.
BATT_SAFETY_MARGIN  equ     10

;; player resource drive refresh allowed only when battery usable % is above this value. 
;; 50% of usable range [0.9V to 1.5V] is around 1.2V This is the safe target alkaline voltage to do a refresh.
;; LIION will also require 50% which will work but that could be changed in the future. 
;RESOURCE_REFRESH_MIN_BATT_PCT equ 50

; stmp00005071 adds these for mp3 encode stability at low battery per battery type & mmc presence.

;// Vbat threshholds empirically tuned by profiling max bat current for 3 HW cases: 
;// 1 GigaByte MMC, NAND flash only, & LiIon Buck mode. See voicemenu pre-encode Vbat check.
 if (!@def('CLCD'))
MP3_ENC_MMC_MIN_V_BAT_MV equ 1100
MP3_ENC_NANDONLY_MIN_V_BAT_MV equ 1000
 else
MP3_ENC_MMC_MIN_V_BAT_MV equ 1250
MP3_ENC_NANDONLY_MIN_V_BAT_MV equ 1150
 endif
MP3_ENC_BUCKMODE_MIN_V_BAT_MV equ 3300

; These Vbat use-range-percentages are derived to correspond with target battery 
; voltages of 1.10V for MMC, 1.0V for nand only (alkaline or nimh), and 3.3V for any LiIon.
; See mp3 encoder overlay.
MP3_ENC_MMC_MIN_V_BAT_RANGE_PCT      equ 33
MP3_ENC_NANDONLY_MIN_V_BAT_RANGE_PCT equ 17
MP3_ENC_BUCKMODE_MIN_V_BAT_RANGE_PCT equ 25

;///////////////////////////////////////////////////////////////////////////////
; Voice recording filenames
; number of digits in filename Vxxx.wav
;///////////////////////////////////////////////////////////////////////////////
DIGITS_IN_VOICE_FILENAME   equ      3   

;///////////////////////////////////////////////////////////////////////////////
; Mic Bias: Set this enable to 1 to internally generate mic bias voltage. 
; Also below, must configure internal resistor value and pin to use for DC bias. 
; Can set to 0 to disable internally generated mic bias voltage at pin lradc1/2.
 if @def('DEVICE_3500')
ENABLE_INTERNALLY_GENERATED_MICBIAS equ 1 
; MIC BIAS circuit source selection: use Low Resolution ADC pin 2 or 1 to bias mic. 
; Per your layout: select LRADC 2 or 1 below 
;   (pin LRADC 2 for 35xx cpu card revB, LRADC 1 for 35xx cpu card revC, ref schematics, & demo player)
LRADC_NUM_FOR_MIC_BIAS equ 1 
; Sets internal mic bias R to value 2, 4, or 8 (in kOhms).
MICBIAS_CONFIGURED_RESISTOR_KOHMS equ 8

; Allows mic bias to settle before sampling. This delay currently unused. For rec btn when bias not yet ready.
REC_BTN_INTERNAL_MIC_BIAS_SETTLING_DELAY_MS equ 400 
; Option always keeps bias enabled after mainmenu init. This allows for start pop prevention if you use rec button from outside voice menu.
; This selection costs 1 mWatt but gains the desired low record button to sampling latency in the record-from-music-menu use-case.
KEEP_MIC_BIAS_ENABLED equ 0

 else
; STMP3410
ENABLE_INTERNALLY_GENERATED_MICBIAS equ 0
LRADC_NUM_FOR_MIC_BIAS equ 1 
MICBIAS_CONFIGURED_RESISTOR_KOHMS equ 2
 endif 

;///////////////////////////////////////////////////////////////////////////////
; Number of available soft timers
;///////////////////////////////////////////////////////////////////////////////
   if @def('SYNC_LYRICS')
SOFT_TIMERS  equ 10
   else
   if @def('JPEG_DECODER')
SOFT_TIMERS  equ 10
   else
SOFT_TIMERS  equ 9
   endif
   endif

;///////////////////////////////////////////////////////////////////////////////
;  sizes
;///////////////////////////////////////////////////////////////////////////////
   if @def('MMC')
   if @def('USE_PLAYLIST5')
MENU_STACK_SIZE equ    1500
   else
MENU_STACK_SIZE equ    1250
   endif ;if @def('USE_PLAYLIST5')
   else
   if @def('USE_PLAYLIST5')
MENU_STACK_SIZE equ    1500
   else
MENU_STACK_SIZE equ    1250
   endif ;if @def('USE_PLAYLIST5')
   endif ;if @def('MMC')

; NOTE: SDK2.6 USED 550 FOR NEXT LINE, BUT SDK3.0BETA USED 750. Should it be ifdef MTP 750 else 550 for other builds? TOVERIFY. MYALLOC
; 
STACK_L1_SIZE   equ     750
STACK_L2_SIZE   equ     100
STACK_L3_SIZE   equ     160

;
; If we are in MTP mode the overlay task stack can shrink.
; TODO shrink this stack for MTP, I will leave it at 500 until we verify that it 
; is ok with switching code.
;
    if @def('MTP_BUILD')
OVERLAY_MANAGER_STACK_SIZE      equ     300
    endif
    
;///////////////////////////////////////////////////////////////////////////////
; maximum number of nested funclets 
;///////////////////////////////////////////////////////////////////////////////
MAX_NESTED_FUNCLET   equ     6 

;///////////////////////////////////////////////////////////////////////////////
;    LCD DEFINITIONS
;///////////////////////////////////////////////////////////////////////////////

SPACE_CHAR      equ     $000020          
ZERO_CHAR       equ     $000030
COLON_CHAR      equ     $00003A
PERIOD_CHAR     equ     $00002E

 if (@def('S6B33B0A_LCD'))
LCD_X_SIZE      equ      128
LCD_Y_SIZE      equ      159
 endif

 if (@def('SED15XX_LCD'))
LCD_X_SIZE      equ     128
LCD_Y_SIZE      equ     64
 endif


;////////////////////////////////////////////////////////////////////////////////
;   Details on Customizing Contrast
;///////////////////////////////////////////////////////////////////////////////
;   Max supported LCD range is 0 - 3F; however due to hardware/voltage differences
;   the range of visibility is usually smaller than this.  It is important to 
;   calibrate the visible range, because the contrast setting is saved.
;   If the user shuts off the player while lcd is not visible, the player is useless
;   unless the ezact sequence is remembered.
;
;   To find out what range your player supports: 
;   change these equs to full range or comment out (full range is default)
;
;;;;;;;
; uncomment the line below to build code that will provide raw contrast value
; recommended calibration using player -- uncomment 
;;;;;;;
;CONTRAST_CALIBRATION    equ  1
;
;//////////////////////////
 if (@def('DEMO_HW'))
; this is the setting for ET301 demos; Next 2 line have not been updated for new Shingyi LCD (June6'05)
LCD_MAX_CONTRAST    equ $32 
LCD_MIN_CONTRAST    equ $1E
 else

 if (@def('S6B33B0A_LCD'))
LCD_MAX_CONTRAST	equ 210
LCD_MIN_CONTRAST	equ 160	
 endif

 if (@def('SED15XX_LCD'))
; Next 3 line comment was for OLD shingyi LCD: (default LCD before sdk2.6)
; Engineering board regs support range [17-37].
;   Engineering board DC/DC support range [24-46]. 
;   One default contrast range [24-42] works for both.
;
; The 3 sets of contrast ranges below are updated for SDK2.6 to support either 
; new or old ShingYih LCDs. "File" refers to \resource\shingyih\system_lcd_init_seq.src
;
; 3/10/05 - Use one of the 2 next contrast ranges depending on which
; ShingYih LCD you have.  See \resources\shingyi\readme.txt 
;
; Optimal for NEW LCD with NEW file (SDK2.6 default)  
; G098064-41 LCD module (present on engr board revH LCD card)

 if (@def('NEWSHINGYIH'))
LCD_MAX_CONTRAST equ 250
LCD_MIN_CONTRAST equ 0
 else
;-----
; Near optimal for OLD LCD with NEW file. 
; NOT optimal for the new lcd but pretty good. So u may use this if u want 1 build for both LCDs.
LCD_MAX_CONTRAST equ 250
LCD_MIN_CONTRAST equ 0

;=====
; Optimal for OLD ShingYih LCD with OLD file (as sdk2.521)
; Suggest going with default new init file & values above for your LCD instead of this historic ver.
;LCD_MAX_CONTRAST equ 42
;LCD_MIN_CONTRAST equ 24 

 endif
 endif

 endif
 
;////////////////////////////////////////////////////////////////////////////////
; The default value of the lcd contrast in % of range
;   the default value is used when no settings.dat is available
;////////////////////////////////////////////////////////////////////////////////

 if (@def('S6B33B0A_LCD'))
DEFAULT_CONTRAST    equ  50 ; 60% of range is default value
 endif

 if (@def('SED15XX_LCD'))
DEFAULT_CONTRAST    equ  50 ; % of range is default value (was 60%)
 endif


; percentage change per increment/decrement message (LCD_DEC_CONTRAST/LCD_INC_CONTRAST)
LCD_STEPSIZE_CONTRAST equ 10  ; make lower when doing calibration


;///////////////////////////////////////////////////////////////////////////////
; For FFWD and RWND
;///////////////////////////////////////////////////////////////////////////////
SECONDS_TO_SKIP             equ     1
SECONDS_TO_SKIP1            equ     3
SECONDS_TO_SKIP2            equ     6
PREV_SONG_THRESHOLD equ 5  ; number of seconds to cause reset to begin of song for PREV push
FIRST_TIME_BOUNDARY equ 15 ; number of 300 mSec periods before the numbers of seconds to skip increases
SECOND_TIME_BOUNDARY equ 30 ; number of 300 mSec periods before the numbers of seconds to skip increases

; For audible FFW/RWD
SECONDS_TO_SKIP_FOR_3X_RATE		equ 1
SECONDS_TO_SKIP_FOR_60X_RATE	equ 18
SECONDS_TO_SKIP_FOR_600X_RATE	equ 180
SECONDS_TO_SKIP_FOR_1800X_RATE	equ 540
LEVEL1_BOUNDARY equ 17 ; number of 300 mSec periods before the numbers of seconds to skip increases
LEVEL2_BOUNDARY equ 33 ; number of 300 mSec periods before the numbers of seconds to skip increases
LEVEL3_BOUNDARY equ 50 ; number of 300 mSec periods before the numbers of seconds to skip increases
; Stmp00010296 Ticket #71685:  for song shorter than the min. skip size
; if song_time < skip_size, bCurrentSongShort=>TRUE (ignore FWD/RWD commands)
; Short Song Time, songs too short to play.
SHORT_SONG_TIME		equ		SECONDS_TO_SKIP 	

;///////////////////////////////////////////////////////////////////////////////
; MP3 Sync Values
;///////////////////////////////////////////////////////////////////////////////
MP3_SYNC_THRESHOLD              equ     70000 ; # bytes to look for sync before marking it bad
MP3_SYNC_THRESHOLD1             equ     10000 ; # bytes to look for sync before slowing decoder isr frequency (to let menus run)
MP3_DECODERISR_FAST             equ     7500  ; once we have sync'd, the isr should be called this frequently
MP3_DECODERISR_SLOW             equ     50000 ; if decoder is having difficulty syncing, switch isr to be called less frequently


;///////////////////////////////////////////////////////////////////////////////
;// Multi-Stage Volume Control Definitions
;///////////////////////////////////////////////////////////////////////////////
MULTI_STAGE_VOLUME              equ     $1                  ;// Use Multi-Stage Volume

;// Master Volume definitions
MIX_MSTR_MAX_VOL                equ     $0
NUM_MSTR_ATT_STEPS              equ     ($1F-MIX_MSTR_MAX_VOL)

;// DAC-Mode definitions
MIX_DAC_NOM_VOL                 equ     $6                  ;// Adjusts 0dB point
MIX_DAC_MIN_VOL                 equ     $1F
; For minimum clipping: Set MIX_DAC_MAX_VOL below to match MIX_DAC_NOM_VOL value 2 lines above.
; For additional gain (with possible clipping): Set MIX_DAC_MAX_VOL in range below. 
;                                               Each integer below MIX_DAC_NOM_VOL provides 1.5 dB gain on Stmp34x0; 
;                                               Max gain possible: 8 step diff would provide +12dB gain.   
MIX_DAC_MAX_VOL                 equ     $0  ; range: [0 to MIX_DAC_NOM_VOL] (0 for loudest volume)

NUM_DAC_ATT_STEPS               equ     (MIX_DAC_MIN_VOL-MIX_DAC_NOM_VOL+1)
NUM_DAC_GAIN_STEPS              equ     (MIX_DAC_NOM_VOL-MIX_DAC_MAX_VOL)
NUM_DAC_MODE_VOL_STEPS          equ     (NUM_DAC_ATT_STEPS+NUM_MSTR_ATT_STEPS+NUM_DAC_GAIN_STEPS+1)

;// If there is no stored volume, set to 50% of DAC-Mode max; change if DAC Mode is not the default
;// STMP35xx has 2dB attenuation per step so default needs a few more steps above midpoint.
DEFAULT_VOLUME                  equ     ((NUM_DAC_MODE_VOL_STEPS/2)+6)         

;// Set maximum restored volume to 75% of DAC-Mode max; change if DAC Mode is not the default
MAX_RESTORED_VOLUME             equ     ((3*NUM_DAC_MODE_VOL_STEPS)/4)   


;// Line In definitions (used for Line-In 1)
MIX_LINE_NOM_VOL                equ     $8                  ;// 0dB point of the Line In
MIX_LINE_MIN_VOL                equ     $1F                 ;// Minimum volume of Line In
MIX_LINE_MAX_VOL                equ     $6                  ;// Maximum volume of Line In (can adjust extra gain)
NUM_LINE_ATT_STEPS              equ     (MIX_LINE_MIN_VOL-MIX_LINE_NOM_VOL+1)
NUM_LINE_GAIN_STEPS             equ     (MIX_LINE_NOM_VOL-MIX_LINE_MAX_VOL)
NUM_LINE_MODE_VOL_STEPS         equ     (NUM_LINE_ATT_STEPS+NUM_MSTR_ATT_STEPS+NUM_LINE_GAIN_STEPS+1)

;// Line In definitions (used for FM tuner with 144 pin package)
MIX_FM_NOM_VOL                equ     $8                  ;// 0dB point of the Line In
MIX_FM_MIN_VOL                equ     $1F                 ;// Minimum volume of Line In
MIX_FM_MAX_VOL                equ     $6                  ;// Maximum volume of Line In (can adjust extra gain)
NUM_FM_ATT_STEPS              equ     (MIX_FM_MIN_VOL-MIX_FM_NOM_VOL+1)
NUM_FM_GAIN_STEPS             equ     (MIX_FM_NOM_VOL-MIX_FM_MAX_VOL)
NUM_FM_MODE_VOL_STEPS         equ     (NUM_FM_ATT_STEPS+NUM_MSTR_ATT_STEPS+NUM_FM_GAIN_STEPS+1)

;///////////////////////////////////////////////////////////////////////////////
;// When enabled (1), periodically checks for one of several FM state machine invalid deadlock states & recovers. 
;// Define as 0 to disable FM-deadlock-state checking and recovery.
FM_WATCHDOG_ENABLE              equ     1

    if !@def('STMP_BUILD_PLAYER')
;//
;//! This varible holds the lcd display state for the mtp project.
;//
;/
;C insert
;#include <types.h>
;extern volatile WORD g_wActivityState;
;endc
    endif ; if !@def('STMP_BUILD_PLAYER')

;C insert
;void _reentrant Init5VSense(void);
;void _reentrant ServiceDCDC(void);
;endc

;//////////////////////////////////////////////////////////////////////////
;// JPEG Thumbnail Mode Setting
THUMBNAIL_X					equ			2		;// number of column in thumbnail mode
THUMBNAIL_Y					equ			2		;// number of row in  thumbnail mode
THUMBNAIL_X_OFFSET			equ			4		;// thumbnail boundary offset x
THUMBNAIL_Y_OFFSET			equ			4		;// thumbnail boundary offset y

   endif ; if (!@def(_PROJECT_INC))
