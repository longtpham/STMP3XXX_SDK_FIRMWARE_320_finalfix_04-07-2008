/**

*** Copyright (c) SigmaTel, Inc. Unpublished

***

*** SigmaTel, Inc.

*** Proprietary & Confidential

***

*** This source code and the algorithms implemented therein constitute

*** confidential information and may comprise trade secrets of SigmaTel, Inc.

*** or its associates, and any use thereof is subject to the terms and

*** conditions of the Confidential Disclosure Agreement pursuant to which this

*** source code was originally received.

**/


/*-------------------------------------------------------------------
        LV24000.c: Driver for Sanyo LV24000 tuner

V1.0	30-Dec-2004
		- Initial version

V1.1	03-Jan-2005
		- Add external clock support (compiler' switch USE_EXTCLK and USE_PWM0)

V1.2	04-Jan-2005
		- Add wrap frequency option by CheckRfLimit to wrap the frequency 
		  by manual step
		- Enable PWM0 when entering measurement mode. Disable when exits
V1.3A	31-Jan-2005
		- Remove USE_EXTCLK option (it required now). Discard timer 3 code.
		- Remove CountPulseCnt1-function
		- Remove Pulse3wNRW-function
		- Intergrate CountPulseCnt1 into CountPulse-function
		- Function IsFrequencyOk: remove 0Hz deviation comparing (not used in this project)
		  (Suggestion of Dennis W.)
		- Remove IR01_CNT_CTRL_REG, IR01_IRQ_MSK_REG, IR01_AUDIO_CTRL1_REG, IR01_AUDIO_CTRL2_REG from the shadow list
		- Don't turn on IR1_RCTL1_EN_MEAS-bit in GetFmFieldStrength (not necessary for LV24000-V6)
		  (Suggestion of Dennis W.)
		- Merge function SetOutputSelect into SetupChipMode
		- ScanSetFreq: remove PrecisionLevel QSSF_PRECISION_NONE, QSSF_PRECISION_LOW support

V1.3B	03-Feb-2005
		- Merge Set_StereoDecode_Freq-funtion in TunerDriverSetMonoStereoMode-function
		- Rearrange dwFreq in CountPulse-function
		- Start PWM0 in SetupChipMode (12MHz for RF, 32kHz for others). CountPulse function will reprogram PWM0 
		  to 12MHz when calibrating IF adn SD frequency
		- Add CB_SCANFREQ compiler option to disable the callback code during scanning (report current frequency)

V1.3C	04-Feb-2005
		- Move IF calibration to Power Up handling
		- Move Start PWM0 code back to CountPulse-function
		- Remove measure time parameter in FineTuneRfOsc-function (we always measure RF with 12MHz clock and 128-tab)
		- Remove byPrecisionLevel parameter in ScanSetFreq-function
		- Merge FineTuneRfOsc, CalcualteRfCap into ScanSetFreq-function
		- Remove u24MsrTimeUs parameter from function GetRfAtCapOsc

V1.3D	14-Feb-2005
		- Remove (with CB_SCANFREQ option) all callbacks in ScanFmRoutine
		- Optimise Callback function
		- Remove USE_FMSCAN option (always support)
		- Move g_wFMTunerError.m_bTUNER_BUSY handling to ScanStation
		- ScanSetFreq-function reviewed

V1.3E	16-Feb-2005
		- Remove compile options and their code:
			* PCB_REV_A: PCB revision A is no longer supported
			* USE_EXTCLK: external clock is always required
			* USE_PWM0: PWM0 is always required 
		- Remove not used code
		- Skip hardware limit checking on IF osc, SD osc, RF limit

V1.3F	21-Feb-2005
		- Discard RF frequency by setting/scanning frequency, only use display frequency:
			* Function ScanSetFreq: accept display frequency as input (was RF-frequency)
			* Remove ScanFmRoutine-function
			* Remove FindFmStation-function
			* Remove GetDisplayFrequency-function
			* Remove SetRfFrequency-function (merg into TunerDriverTuneToFrequency)
			* Change CheckRfLimit to CheckFreqLimit
			* Add IsFmStation-function
		- Remove CB_SCANFREQ compiler option
		- Remove IF_PHASE_IS_NEG compiler option
		- Change callback to StoreFmStation (no parameter), remove callback reason equations 
V1.4A	20-Apr-2005
		- Add "soft AFC" function (See V1.4A marking)
		- File FmTunerMenu.c: enable SysPostMessage(2,TUNER_GET_STATES) statement for AFC periodic checking

V1.4B	26-Apr-2005
		- Use scanning with FS function (was IF) to simplify code and achieve speed:
			* Add function ScanGetFs
			* Remove MeasureFrequency-function (no longer needed)
			* Use PWM0 only with 12MHz rate - no need to switch between 12MHz/32kHz
			* Change function prototype of PWM0Init(BOOL bHighClock) - No parameter required now
			* Remove Measuring IF in lock mode support in CountPulse-function
		- Reintroduce measuring with timer 3 code (SigmaTel's request):
			* Add compiler's option USE_PWM0.
			* Rename CountPulse-function to CountPulseWith12MHzClk
			* Add CountPulseWithTimer3 function
			* Add macro mCNTPULSE_FUNC to switch between the PWM0/TMR3 function, depends on USE_PWM0
		- Integrate PWM0Init function into CountPulseWith12MHzClk
		- Add code to support displaying during scanning:
			* Redefine SCANSTN_NEXT (bit 0=1), SCANSTN_PREV (bit 0=0), SCANSTN_AUTO (bit 1):scan parameter of 
			  ScanStation-function. New definition STN_USCAN, STN_DSCAN, STN_ASCAN in g_byStnFlag (see Sanyo24000.h)
			  to keep track of the in progress scanning action.
			* Add ScanStation variant for updating display: this function terminates when a station is found or the
			  scanning distance is reached.
			* Restart the scan action in TunerGetStatus, depends on scanning flags in g_byStnFlag
			* Add USE_SCANNING_DISPLAY compiler option to turn the displaying during scanning code on/off
		- Add macro mSCANSETFREQ_FUNC to split the ScanSetFreq function to ScanSetFreqPwm(1 parm.) and ScanSetFreqTmr (2 parm.)

V1.4C	02-May-2005
		- Clean up source code: remove not used code
			* Remove USE_AUDIO_MUTE option and its code
			* Remove USE_REGION option and its code
		- Optimizing for code space
			* Use fix values for registers to remove the register shadowing resource
			* Remove SetAFCMode-function. The LV24xxx operates in 2 modes:
				Measuring mode: Enable measurement, disable AFC, select something at output
				Normal mode: Disable measurement, enable AFC, nothing selected at output
			* Simplify SetUpChipMode/GetOutputSelect by redefining CO_xxx using IR1_MSRCS_MSS_xxx
			  (The CO_xxx is then also the mask to select the correct output in IR01_MSRC_SEL_REG register)
			* Change SetupChipMode-function into SetChipOutput: works with chip output. When something is set at 
			  chip output, the measure mode is enabled and AFC will be disabled. When return to normal mode, 
			  (invoke with CO_NONE as output), the measure mode is disabled and AFC is enabled
			* Add g_byChipOutput global to remove fuction GetOutputSelect
			* Remove bit manipulation function (DriveBit, DriveBitNeg, SetRegBits)
			* Remove shadowing stuff (g_Lv24ShadowList, g_byaShwRegValue and shadowing code)
			* Remove g_wCurFmFreq, use g_wCurrentFrequency instead
		- Add SortFmStations-function: sort stations from strong to weak after auto store
V1.4D	13-May-2005
		- SortFmStations: Set frequency to the 1st preset after auto-scanning
		- ScanStation: set g_wFMTunerError.m_bTUNER_BUSY to TRUE when entering this function
		  for the first time to fix Mute/Un-mute problem. Remove code to mute/un-mute the LV24xxx (leave it to system-sw)
		- Optimize setting RF-frequency by combining calculating the RF-OSC value and interpolating instead of 
		  only interpolating (reduce number of RF-measurements)

V1.4E	18-May-2005
		- Also support scanning with IF (compile switch USE_SCAN_IF, not defined is scanning with FS)
		- When USE_SCAN_IF and USE_PWM0 are defined, add code to switch PWM0 to 32kHz for measuring IF-frequency
		- Change TUNER_PROCESS_INTERVAL_ms (in TunerModule.c) from 100 to 10ms to speed up display time (pause between didplay)

V1.4F	18-May-2005
		- Remove not used resource/codes:
			U16 g_uOscResLo, g_uOscResHi
			U8  g_byCurOsc

V1.4G	01-Jun-2005
		- Support 24MHz external clock: 
			* Add function CountPulseWith24MHzClk (24MHz specific code)
			* Remove USE_PWM0, USE_TMR3 compiler switch
			* Add LV_MSRCLK compiler switch. Possible values for LV_MSRCLK are:
				CD_PWM0		0: Measure with PWM0 (switching between 12MHz and 32kHz)
				CD_TMR3		1: Measure with Timer 3
				CD_24MHz	2: Measure with 24MHz external clock
		- Rename CountPulseWith12MHzClk to CountPulseWithPwm0 (just for clarity)
		- Only use g_byIfBW-global variable when USE_SCAN_FS is defined
		- Define CO_IF_FREE to speed up the IF-calibration when using external clock (PWM0 or 24MHz)
		- Lower the IF_SWING threshold (by IsFmStation) from 70 kHz to 60 kHz.
		- Use iOscRes for approaching RF-frequency in LinTunDac-function to reduce number of iterations 
		  by interpolation RF-frequency
		- Fix bugs reported by SigmaTel (Dennis W.)
			* Stereo not correctly displayed: 
				+ Bug in ScanStation (with display code enabled, CompletionProc is invoked 
				  before restoring chip state -> interrupt is not armed!)
			* Scanning with Timer 3 code is not so good as PWM0: increase measure time from 4ms to 6ms (1s extra scan time)

-------------------------------------------------------------------*/

/*-------------------------------------------------------------------
        FM TUNER Constants that can be overruled by project.inc 
-------------------------------------------------------------------*/
#include "hwequ.h"
#include "messages.h"
#include "exec.h"
#include "resource.h"
#include "project.h"
#include "..\tunerdriver.h"
#include "Sanyo24000.h"

#ifdef FUNCLET
#pragma asm
    extern SysCallFunclet
#pragma endasm
#endif

/*-------------------------------------------------------------------
        Compiler's options
-------------------------------------------------------------------*/
// Scanning methode
#define USE_SCAN_IF				// Scanning with IF-curve. Use FS-curve if not defined

// Scanning's behaviour
#define USE_SCANNING_DISPLAY	// Update display during scanning if defined

// Region select. 
#define USA_REGION			// USA if defined
//#define EUROPE_REGION       // EUROPE if defined
//#define JAPAN_REGION        // Japan if defined
// LV24xxx specific options
#define LOW_SPEED_AGC			// Undef this for fast AGC device

// Left out for code space
//#define USE_HWRF_LIMIT		// Checking hardware RF limit

// --------- 
// Internal options - don't modify
// ---------
// Internal options/equations - don't modify
// ---------
#ifdef FM_TUNER_USE_PWM0
	#define mCNTPULSE_FUNC(t, p) CountPulseWith12MHzClk(p)
	#define mSCANSETFREQ_FUNC(f, t) ScanSetFreqPwm(f)
#else	//FM_TUNER_USE_PWM0
	#define USE_TMR3			// Use timer 3 if PWM0 not defined
	#define mCNTPULSE_FUNC(t, p) CountPulseWithTimer3(t, p)
	#define mSCANSETFREQ_FUNC(f, t) ScanSetFreqTmr(f, t)
#endif //FM_TUNER_USE_PWM0

#if CD_24MHz  // Not used
	#define mCNTPULSE_FUNC(t, p) CountPulseWith24MHzClk(p)
	#define mSCANSETFREQ_FUNC(f, t) ScanSetFreqPwm(f)
#endif //CD_24MHz

#ifndef USE_SCAN_IF			// Scanning with FS if USE_SCAN_IF not defined
	#define USE_SCAN_FS			
#endif //USE_SCAN_IF

#ifndef USE_SCANNING_DISPLAY
	#define NO_SCANNING_DISPLAY
#endif //USE_SCANNING_DISPLAY

#ifdef LOW_SPEED_AGC		// AGC delay for low speed AGC device
#ifdef FM_TUNER_USE_PWM0
	#define WAIT_AGC_TIME	DLT_8ms
#else //FM_TUNER_USE_PWM0
	#define WAIT_AGC_TIME	DLT_5ms
#endif //FM_TUNER_USE_PWM0

#if CD_24MHz // Not used
	#define WAIT_AGC_TIME	DLT_8ms
#endif //CD_24MHz
#endif //LOW_SPEED_AGC

/*-------------------------------------------------------------------
        FM TUNER Constants that can be overruled by project.inc 
-------------------------------------------------------------------*/
#pragma optimize 1

#ifndef NUMBER_OF_PRESETS
#define NUMBER_OF_PRESETS	10
#endif
#ifndef FM_BAND_LIMIT_HIGH_FREQ
//#ifdef (USA_REGION || EUROPE_REGION)  // // US or Europe region
#define FM_BAND_LIMIT_HIGH_FREQ	108000
//#else // Japan region
//#define FM_BAND_LIMIT_HIGH_FREQ	90000
//#endif
#endif
#ifndef FM_BAND_LIMIT_LOW_FREQ
//#ifdef (USA_REGION || EUROPE_REGION) // US or Europe region
#define FM_BAND_LIMIT_LOW_FREQ	87500
//#else // Japan region
//#define FM_BAND_LIMIT_LOW_FREQ	76000
//#endif
#endif
#ifndef FM_TUNER_GRID
#define FM_TUNER_GRID	100
#endif
#define FM_SEARCH_HIGH_LIMIT	(FM_BAND_LIMIT_HIGH_FREQ)
#define FM_SEARCH_LOW_LIMIT		(FM_BAND_LIMIT_LOW_FREQ)

/*-------------------------------------------------------------------
        FM Tuner Global variables
-------------------------------------------------------------------*/
struct ErrorBits g_wFMTunerError;
//#ifdef (USA_REGION || EUROPE_REGION)  // // US or Europe region
WORD g_wCurrentFrequency = 90500;
//#else // Japan region
//WORD g_wCurrentFrequency = 76000;
//#endif

BOOL g_bTunedStereoStation;

WORD g_wCurrentPreset;
BOOL g_bSearchEndAtBandLimit = TRUE;

typedef struct PresetStation 
{
	WORD Frequency;
	INT	 Level;
} FMTunerPreset;
FMTunerPreset g_astPresetStations[NUMBER_OF_PRESETS];	// Set up the preset stations

/*-------------------------------------------------------------------
        Macro
---------------------------------------------------------------------*/
// Convert Display Frequency to RF
#define DisplayFreqToRf(wDisp) (wDisp + (DEFAULT_IF_FREQ/1000))	// Negative IF phase: RF (in 1 kHz) = display + IF

// Convert RF to Display Frequency
#define RfToDisplayFreq(wRf) (wRf - (DEFAULT_IF_FREQ/1000));	// Negative IF phase: display = RF - IF

/*-------------------------------------------------------------------
        LV24xxx Default values
---------------------------------------------------------------------*/
// Below values can be adjusted to the desired level
#define	LV_DEFAULT_SEARCH_LEVEL		4	// Default search level (0...7)
#define LV_SCAN_DISPLAY_DISTANCE	7 /*3*/	// Update during scan in 100 kHz step (minimal is 1)
#define	LV_DEFAULT_SOFT_STEREO		3	// Default soft stereo level (0..7 - 0 is disabled)
#define	LV_DEFAULT_SOFT_MUTE		3	// Default soft mute level (0..7 - 0 is disabled)
#define	LV_DEFAULT_VOLUME			4	// Default volume level (0..15 - 15 is minimal volume)
#define	LV_DEFAULT_TONE_LVL			7	// Default tone level (0..15 - 15 is minimal tone)

/*-------------------------------------------------------------------
        LV24xxx registers default values
---------------------------------------------------------------------*/
// 0x0102: Measure source select - Set AFC at low speed, high trigger level
#define IR01_MSRCSEL_DEFAULT	(IR1_AFCSPD_LO|IR1_AFCLVL_HI)

// 0x0108: Interrupt mask: IRQ active high, enable AFC/Stereo interrupt
#define IR01_IRQMSK_DEFAULT		(IR1_IRQM_EN_AFC|IR1_IRQM_EN_MS|IR1_IRQM_ACT_HIGH)

// 0x0202: Radio control 1 - Set all reserved bits to 1 according to spec.
//	We need 2 settings for this register:
//		- Setting for measurement: enable measurement bit=1, keep AFC reset and disabled
//		- Setting for normal operation: enable measurement bit=0, AFC enabled
#define IR01_RADIOCTRL1_DEFAULT	(IR1_RCTL1_NA_H_0|IR1_RCTL1_NA_H_1|IR1_RCTL1_NA_H_4)
#define IR01_RADIOCTRL1_MSR		(IR01_RADIOCTRL1_DEFAULT|IR1_RCTL1_EN_MEAS|IR1_RCTL1_RST_AFC)
#define IR01_RADIOCTRL1_NORMAL	(IR01_RADIOCTRL1_DEFAULT|IR1_RCTL1_EN_AFC)

// 0x0206: Radio control 2 - Turn VREF2/VREF on, IF PLL mute off, Turn on AGC speed for quick fieldstrength measuring
#define IR01_RADIOCTRL2_DEFAULT	(IR1_RCTL2_AGC_SPD|IR1_RCTL2_IF_PM_OFF)
#define IR01_RADIOCTRL2_PM_ON	(IR1_RCTL2_AGC_SPD|IR1_RCTL2_IF_PM_ON)

// 0x0207: Radio control 3 - mute tone, select FM source, Set AGC level for FM
#define IR01_RADIOCTRL3_AUDIO	(IR1_RCTL3_SE_FM|IR1_RCTL3_AGC_SETLVL|IR1_RCTL3_AUDIO_MUTE_L|IR1_RCTL3_VOL_SHIFT)
#define IR01_RADIOCTRL3_NOAUDIO (IR1_RCTL3_SE_FM|IR1_RCTL3_AGC_SETLVL)

// 0x0208: Stereo Control - Stereo, SD PLL mute off, set default CS value, auto slew rate on
#define IR1_DEF_CS_VAL			((U8)0x4<<4)	// IR01_STEREO_CTRL_REG: default channel separation value	
#define IR01_STEREOCTRL_DEFAULT	(IR1_DEF_CS_VAL|IR1_STCTL_AUTO_SLEWRATE|IR1V6_CANCEL_PILOT)

// 0x0209: Audio Control 1 - Set default tone and volume level
#define IR01_AUDIOCTRL1_DEFAULT	((U8)((LV_DEFAULT_TONE_LVL<<4)|(LV_DEFAULT_VOLUME<<0)))

// 0x020A: Audio Control 2 - treble/bass=center, no beep out, set de-emphasis according to region select
#ifdef USA_REGION  // US or Europe region
#define IR01_AUDIOCTRL2_DEFAULT	(IR1_BEEP_HIGH|IR1_ACTL2_DEEMP75)
#else // Set EUROPE or Japan
#define IR01_AUDIOCTRL2_DEFAULT	(IR1_BEEP_HIGH|IR1_ACTL2_DEEMP50)
#endif //USA_REGION

// 0x20B: Power and soft control - default value of soft stereo, soft mute
#define IR01_PWSCTRL_DEFAULT	((U8)((LV_DEFAULT_SOFT_STEREO<<5)|(LV_DEFAULT_SOFT_MUTE<<2)))
#define IR01_PWSCTRL_POWER_ON	(IR01_PWSCTRL_DEFAULT|IR1_PSCTL_PW_RAD)
#define IR01_PWSCTRL_POWER_OFF	(IR01_PWSCTRL_DEFAULT)

/*-------------------------------------------------------------------
        Local data
---------------------------------------------------------------------*/
typedef struct	// Register address-value structure (for default register settings)
{
	U16 wRegAddress;
	U8  byValue;
} I3W_REG_VALUES;

// ----- Default register value list of LV2400x
I3W_REG_VALUES g_Lv2400xDefault[] = 
{
	IR01_MSRC_SEL_REG,		IR01_MSRCSEL_DEFAULT,	// 0x0102 - Measure source select: nothing selected
	IR01_IRQ_MSK_REG,		IR01_IRQMSK_DEFAULT,	// 0x0108 - Interrupt mask: IRQ active high, enable AFC/Stereo interrupt (V1.4A)
	IR01_RADIO_CTRL1_REG,	IR01_RADIOCTRL1_NORMAL, // 0x0202 - Radio control 1 - Enable AFC as default - keep reserved bit high
	IR01_RADIO_CTRL3_REG,	IR01_RADIOCTRL3_AUDIO,	// 0x0207 - Radio Control 3: Un-mute audio, mute tone, select FM source, Set AGC level for FM
	IR01_AUDIO_CTRL1_REG,	IR01_AUDIOCTRL1_DEFAULT,// 0x0209 - Audio Control 1
	IR01_AUDIO_CTRL2_REG,	IR01_AUDIOCTRL2_DEFAULT,// 0x020A - Audio Control 2: treble/bass=center, no beep out, de-emphasis comform region
	IR01_RADIO_CTRL2_REG,	IR01_RADIOCTRL2_DEFAULT,// 0x0206 - Radio Control 2: VREF2/VREF on
	IR01_PW_SCTRL_REG,		IR01_PWSCTRL_POWER_ON,	// 0x020B - Power and soft control: turn on FM. Set default soft mute, soft stereo
};
#define LV2400X_DEFAULT_REG_LSIZE (sizeof(g_Lv2400xDefault)/sizeof(g_Lv2400xDefault[0]))

/*-------------------------------------------------------------------
        Sanyo global variable for internal usage
---------------------------------------------------------------------*/
U8 g_byStnFlag;				// Station flags
U8 g_byScanLevel = LV_DEFAULT_SEARCH_LEVEL;	// Search level
U8 g_byBlock;				// Current block selected
U8 g_byChipOutput;			// Current chip output
U8 g_byCurCap;				// CAP-value for current RF-frequency

#ifdef USE_SCAN_FS
U8 g_byIfBW;				// Bandwith setting (use during scan)
#endif //USE_SCAN_FS

#ifdef USE_SCANNING_DISPLAY
U24  g_wOrgFreq;
#endif	//USE_SCANNING_DISPLAY

typedef struct				// Structure for caching the frequency of a specific CAP-value
{
	U16 wCap;				// The CAP-value
	U24 u24Freq1;			// Frequency 1 in kHz (lower limit)
	U24 u24Freq2;			// Frequency 2 in kHz (upper limit)
} SSF_CACHE_FREQ;
SSF_CACHE_FREQ g_SsfCache;	// Global var. for caching frequency limit of a CAP-value

// LV2400x QSSF resources
#ifdef USE_HWRF_LIMIT
U24 g_wHwRfLow;				// The minimal hardware RF (in kHz)
U24 g_wHwRfHigh;			// The maximal hardware RF (in kHz)
#endif //USE_HWRF_LIMIT
U24 g_uQssCoefL;			// Coefficient low (at CAP=low, OSC=low)
U24 g_uQssCoefH;			// Coefficient high (at CAP=high, OSC=low)

// Buffer for caching field strength during scan
#ifdef USE_SCAN_IF
typedef struct
{
	U24 u24Freq;			// RF frequency
	U8  byFs;				// Measured FS by above RF
} FSFREQ_ST;
FSFREQ_ST g_FreqFsCache;
#endif //USE_SCAN_FS

// Buffer for caching IF frequency during scan
#ifdef USE_SCAN_IF
typedef struct
{
	U24 u24Freq;
	int iIfFreq;
} FMIFFREQ_ST;
FMIFFREQ_ST g_FreqIfCache;
#endif //USE_SCAN_IF

// Sanyo LV24000 prototypes
// Parser functions
_reentrant void WriteReg(U16 wRegAddress, U8 byValue);
_reentrant U8   ReadReg(U16 wRegAddress);
_reentrant void SelectRegBlock(U16 wRegAddress);

// Hardware IO functions
_reentrant void Sanyo3WireIoDelay(void);
_reentrant void IoWrite3W(U8 byLow, U8 byHigh);
_reentrant U8   IoRead3W(U8 byReg);
_reentrant void IoWriteByte3W(U8 byData);

// LV24000 specific functions
_reentrant void Set_IF_Freq(void);
_reentrant U8   SetChipOutput(U8 byOutput);
_reentrant U8   WriteIfOsc(U16 wIfOscValue);
_reentrant U8   WriteSdOsc(U16 wSdOscValue);
_reentrant U8   WriteRfOsc(U16 wRfOscValue);
_reentrant U8   WriteRfCap(U16 wCapValue);

_reentrant U8  GetFmFieldStrength(void);

#ifdef FM_TUNER_USE_PWM0
_reentrant U8 CountPulseWith12MHzClk(PU24 pu24Freq);
_reentrant U8 ScanSetFreqPwm(U24 wDisplayFreq);
#endif //FM_TUNER_USE_PWM0

#ifdef USE_TMR3
_reentrant U8 CountPulseWithTimer3(U24 u24TimeUs, PU24 pu24Freq);
_reentrant U8 ScanSetFreqTmr(U24 wDisplayFreq, U24 u24TimeUs);
#endif //USE_TMR3

#if CD_24MHz    // not used
_reentrant U8 CountPulseWith24MHzClk(PU24 pu24Freq);
_reentrant U8 ScanSetFreqPwm(U24 wDisplayFreq);
#endif //CD_24MHz

_reentrant int IsFrequencyOk(U24 u24CurFreq, U24 u24ExpFreq, U24 u24Precise);
_reentrant U8  LinTuneDac(U24 u24ExpFreq, U24 dwMeasureTimeUs, FPTR_WRITE_OSC fpWriteOsc, U16 wX1, U16 wX2, int iOscRes);
_reentrant int LvAbs(int i);
_reentrant U24 CalculateCoeff(U24 wFreq);
_reentrant U8  InitTuningRfCapOsc(void);
_reentrant U8  GetRfAtCapOsc(U16 u16CapValue, U16 u16OscValue, PU24 pu24Freq);

_reentrant U8  CheckFreqLimit(PU24 pu24CurFreq);

_reentrant U8  ScanStation(U8 byFlag);
_reentrant U8  IsFmStation(U24 u24CurFreq, int iDir);
#ifdef USE_SCAN_FS
_reentrant U8  ScanGetFs(U24 u24Freq);
#endif //USE_SCAN_FS

_reentrant void StoreFmStation(void);
_reentrant void StoreFmPreset(INT iLocation, WORD wFrequency, INT iLevel, WORD wNewCurPreset);
_reentrant void SortFmStations(void);
_reentrant void LvDelay(WORD dwDelayMs);
_reentrant void CompletionProc(void);

#ifdef NOT_USE_YET
_reentrant void Sanyo24000Mute(BOOL bMute);
#endif //NOT_USE_YET

static void do_nothing(void) {      // This function is here to make the dang FUNCLET, below, work right.
                                    // Without this here, the compiler switches back to ".ptext..." as soon
                                    // as it hits some code.  With this here, the "org p,"SYSFUNCLET_..._P"
                                    // stays in effect.
                                    // The function is declared "static" so that it's invisible outside this file.
  return;
}


#ifdef FUNCLET
    #pragma asm
        FTunerDriverInit:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_TUNER_DRIVER_INIT

        org p,"SYSFUNCLET_TUNER_DRIVER_INIT_P":
        dc      RSRC_FUNCLET_TUNER_DRIVER_INIT
    #pragma endasm
_reentrant INT TunerDriverInit_Funclet(void)
#else
_reentrant INT TunerDriverInit(void)
#endif
{
    INT i;
	//---------
	// SMTP35xx specific intialisations
	//---------
    //power up the required pins
#if (defined(DEVICE_3410)||defined(DEVICE_3310))
    HW_3WIRE_CLOCK_PWR = TRUE;
    HW_3WIRE_DATA_PWR  = TRUE;
    HW_3WIRE_WRITE_ENABLE_PWR = TRUE;
#ifdef HW_3WIRE_BUS_ENABLE_PWR 
    HW_3WIRE_BUS_ENABLE_PWR = TRUE;
#endif 
#endif

    //make them all gpio
    HW_3WIRE_CLOCK_ENR = TRUE;
    HW_3WIRE_DATA_ENR  = TRUE;
    HW_3WIRE_WRITE_ENABLE_ENR = TRUE;
#ifdef HW_3WIRE_BUS_ENABLE_ENR 
    HW_3WIRE_BUS_ENABLE_ENR = TRUE;
#endif 

	// Configure the GPIO for 3-wire bus of the LV24000
	// CLOCK = output (high)
	// WE (NR_W) = Output (high)
	// DATA = Input
    HW_3WIRE_CLOCK_DOR = FALSE;
    HW_3WIRE_DATA_DOR = TRUE;
    HW_3WIRE_WRITE_ENABLE_DOR = TRUE;
#ifdef HW_3WIRE_BUS_ENABLE_DOR 
    HW_3WIRE_BUS_ENABLE_DOR = TRUE;
#endif 
    
    //make all output, except DATA
    HW_3WIRE_DATA_DOER = FALSE;
    HW_3WIRE_CLOCK_DOER = TRUE;
    HW_3WIRE_WRITE_ENABLE_DOER = TRUE;
#ifdef HW_3WIRE_BUS_ENABLE_DOER 
    HW_3WIRE_BUS_ENABLE_DOER = TRUE;
#endif 

// If not using the hardware ready bit then uncomment the next line
#undef	HW_FMTUNER_READY_ENR

#ifdef	HW_FMTUNER_READY_ENR
// set-up the FM Tuner ready input from SWP1 connected to GP16
	HW_FMTUNER_READY_ENR  = TRUE;
	HW_FMTUNER_READY_DOER = FALSE;
	HW_FMTUNER_READY_PWR  = TRUE;
#endif

	*(PU16)&g_wFMTunerError = 0;				// Equivalent of above code
	//g_wFMTunerError.m_bPOWERED_OFF = TRUE;	// Will be set later

	//---------
	// Sanyo LV24xxx specific initialisations
	//---------
	// Software init: Global variables 
	g_byBlock = 0xFF;			// No valid block select at LV2400x yet
	g_byStnFlag = 0;			// Default scanning options
			
	// Flush frequency cache
#ifdef USE_SCAN_FS
	g_FreqFsCache.u24Freq = 0;
#endif //USE_SCAN_FS
#ifdef USE_SCAN_IF
	g_FreqIfCache.u24Freq = 0;
#endif //USE_SCAN_IF
	g_SsfCache.wCap = 0xFF;

	// LV24xxx Hardware init: Write the default values to the device
	for (i=0; i<LV2400X_DEFAULT_REG_LSIZE; i++)
		WriteReg(g_Lv2400xDefault[i].wRegAddress, g_Lv2400xDefault[i].byValue);

	// De-emphasis bit is already set by writing default value LV24xxx registers
	// Power down the tuner as this is the expected state from tuner module
	TunerDriverSetTunerStandby(TRUE);

    return TUNER_SUCCESS;
} // End TunerDriverInit
#ifdef FUNCLET
    #pragma asm
        org p,".ptextsanyo24000":
    #pragma endasm
#endif

_reentrant INT TunerGetStatus( void )
{
	U8 byStat; //V1.4A
    	    
#ifdef USE_SCANNING_DISPLAY
	// Resume the scanning action if needed
	if (g_byStnFlag & STN_SCAN_IN_PROGRESS)
	{
		ScanStation(g_byStnFlag);
		return(E_TUNER_BUSY);
	}
#endif	//USE_SCANNING_DISPLAY
    byStat = 0; // V1.4A

   	// Update current stereo state - watch out for reentrant - we may not access the LV24000 when it's busy!
	// Stereo broadcast when stereo state bit is 1
	if (g_wFMTunerError.m_bTUNER_BUSY)
	{
		g_bTunedStereoStation = FALSE;	// Just say mono - this is always the case when the LV24000 is busy
	}
	else
	{
		// Check data line state (V1.4A)
		//HW_3WIRE_DATA_DOER = FALSE; 			// Change DATA PIN to input mode
		//HW_3WIRE_WRITE_ENABLE_DOR = FALSE; 	// nRW line low (reading from device) - Already the default state
		if (HW_3WIRE_DATA_DIR)	// Data-pin high: interrupt occurred
		{
			byStat = ReadReg(IR01_IRQ_ID_REG);

			// // Determine stereo state if stereo interrupt
			if (byStat & IR1_IRQID_FS_MS)
				g_bTunedStereoStation = ( (ReadReg(IR01_RADIO_STAT_REG) & IR1_RSTAT_STEREO) != 0 );

			if (byStat & IR1_IRQID_AFC)
			{
				WriteReg(IR01_RADIO_CTRL3_REG, IR01_RADIOCTRL3_NOAUDIO);
				ReadReg(IR01_CTRL_STAT_REG);
				TunerDriverTuneToFrequency(g_wCurrentFrequency);
				WriteReg(IR01_RADIO_CTRL3_REG, IR01_RADIOCTRL3_AUDIO);
			}
			WriteReg(IR01_IRQ_OUT_REG, 0);	// Select interrupt on DATA-line
		}
		// End V1.4A
	}

	// Done
	// V1.4A Update current frequency
	if ( (g_byStnFlag & STN_FREQ_CHG) || (byStat & IR1_IRQID_FS_MS) ) //V1.4A
	{
		g_byStnFlag &= ~STN_FREQ_CHG;
		SysPostMessage(2,MENU_TUNER_TUNED);	// V1.4A
	}
	// End V1.4A

	//SysPostMessage(2,MENU_TUNER_TUNED);	// V1.3F
	return TUNER_SUCCESS;

} // End TunerGetStatus

_reentrant INT TunerDriverTuneToFrequency(INT wFrequency)
{
	if (g_wFMTunerError.m_bTUNER_BUSY)
		return TUNER_SUCCESS;
	g_wFMTunerError.m_bTUNER_BUSY = TRUE;

	// Set frequency handling
   	// Check limit
	if ( CheckFreqLimit((PU24)&wFrequency) == LVLS_NO_ERROR )
	{
		// No valid station by a frequency set
		g_byStnFlag &= (~STN_VALID);
		g_byStnFlag |= STN_FREQ_CHG;	// Frequency changed when this invoked
		
		// Select correct oscillator output and enable measuring mode
		SetChipOutput(CO_RF_OSC);
        
        // Set the frequency
		mSCANSETFREQ_FUNC((U24)wFrequency, LV_MSR_TIME_32ms);
		
		// Return to chip idle state 
		SetChipOutput(CO_NONE);

   	}

   	g_wCurrentFrequency = wFrequency;
	CompletionProc();	// V1.4A
	return TUNER_SUCCESS;
} // End TunerDriverTuneToFrequency


_reentrant INT TunerDriverIncrementUp(void )
{
	return(TunerDriverTuneToFrequency(g_wCurrentFrequency+FM_TUNER_GRID));
} // End TunerDriverIncrementUp

_reentrant INT TunerDriverIncrementDown(void)
{
	return(TunerDriverTuneToFrequency(g_wCurrentFrequency-FM_TUNER_GRID));
} // End TunerDriverIncrementDown

_reentrant INT TunerDriverSearchUp(void)
{
    // Perform the scan
	ScanStation(SCANSTN_NEXT);
	return TUNER_SUCCESS;
} // End TunerDriverSearchUp

_reentrant INT TunerDriverSearchDown(void)
{
    // Perform the scan
	ScanStation(SCANSTN_PREV);
	return TUNER_SUCCESS;
} // End TunerDriverSearchDown

_reentrant INT TunerDriverScanStations(void)
{

	INT	i;
	WORD PresetFrequency;

	if (g_wFMTunerError.m_bTUNER_BUSY)
		return TUNER_SUCCESS;

	// Clear current presets
	for (i=0; i<NUMBER_OF_PRESETS; i++)
	{
		StoreFmPreset(i, 0, 0, 0);
	}
	// Perform the scan
    ScanStation(SCANSTN_AUTO|SCANSTN_NEXT);

#ifdef USE_SCANNING_DISPLAY
	return E_TUNER_BUSY;
#endif //USE_SCANNING_DISPLAY

#ifdef NO_SCANNING_DISPLAY
	return TUNER_SUCCESS;
#endif //NO_SCANNING_DISPLAY
}  // End TunerDriverScanStations


_reentrant INT TunerDriverSetToPresetStations(INT iPresetStation)
{
	WORD wPresetFreq;
	if (iPresetStation >  NUMBER_OF_PRESETS)
	{
		iPresetStation = NUMBER_OF_PRESETS;
		g_wFMTunerError.m_bPRESET_RANGE_ERROR = TRUE;
	}
	else
	{
		g_wFMTunerError.m_bPRESET_RANGE_ERROR = FALSE;
	}
	wPresetFreq = g_astPresetStations[iPresetStation-1].Frequency;
	if ((wPresetFreq < FM_BAND_LIMIT_LOW_FREQ) || (wPresetFreq > FM_BAND_LIMIT_HIGH_FREQ))
	{
		wPresetFreq = FM_BAND_LIMIT_LOW_FREQ;
		g_wFMTunerError.m_bUNUSED_PRESET = TRUE;
	}
	else
	{
		g_wFMTunerError.m_bUNUSED_PRESET = FALSE;
	}
	g_wCurrentPreset = iPresetStation;
	return 	TunerDriverTuneToFrequency(wPresetFreq);
} // End TunerDriverSetToPresetStations
#ifdef FUNCLET
    #pragma asm
        FTunerDriverSetMonoStereoMode:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_TUNER_SET_MONO_STEREO

        org p,"SYSFUNCLET_TUNER_SET_MONO_STEREO_P":
        dc      RSRC_FUNCLET_TUNER_SET_MONO_STEREO
    #pragma endasm
_reentrant INT TunerDriverSetMonoStereoMode_Funclet(INT wMode)
#else
_reentrant INT TunerDriverSetMonoStereoMode(INT wMode)
#endif
{
	// Set stereo mode 
	if (wMode == TUNER_MODE_STEREO)
	{
	    // Set the stereo decoder clock 
		// Set_StereoDecode_Freq();
		// Turn on the stereo PLL mute measure the stereo decoder clock
		WriteReg(IR01_STEREO_CTRL_REG, IR01_STEREOCTRL_DEFAULT|IR1_STCTL_SD_PM);

		// Select correct oscillator output and enable measuring mode
		SetChipOutput(CO_SD_OSC);
		
		// Do the tuning - interpolate between 50-150
		LinTuneDac(DEFAULT_SD_FREQ, LV_MSR_TIME_100ms, WriteSdOsc, 50, 150, 0);

		// Tuning done - turn off the PLL mute and enable stereo
		WriteReg(IR01_STEREO_CTRL_REG, IR01_STEREOCTRL_DEFAULT);
		SetChipOutput(CO_NONE);
	} 
	else //if (wMode == TUNER_MODE_MONO)
	{
		// Set mono mode
		WriteReg(IR01_STEREO_CTRL_REG, IR01_STEREOCTRL_DEFAULT|IR1_STCTL_STEREO_L);
	} 

	return(TUNER_SUCCESS);
} // End TunerDriverSetMonoStereoMode
#ifdef FUNCLET
    #pragma asm
        org p,".ptextsanyo24000":
    #pragma endasm
#endif


_reentrant INT TunerDriverSetSensitivity(INT wSensitivity )
{
	// LV24000 has 7 field strength level. Each level is 10 dBuV
	// So just convert range 0...100 to 0...7, wrapping at 70 dBuV
#if 1
	g_byScanLevel = (U8)(wSensitivity/10);
	if (g_byScanLevel > 7)
	{
		g_byScanLevel = 7;
	}
#endif
	return TUNER_SUCCESS;
} // End TunerDriverSetSensitivity

_reentrant INT TunerDriverSetTunerStandby(BOOL bStandby )
{
	// Standby: TRUE -> power down the LV24000
	//			FALSE-> power up the LV24000

	// Drive the tuner power bit of LV24000 
	if (bStandby)	// Standby -> power down the LV24xxx
	{
		WriteReg(IR01_PW_SCTRL_REG, IR01_PWSCTRL_POWER_OFF);
	}
	else	// Not standby: power up and init LV24xxx
	{
		WriteReg(IR01_PW_SCTRL_REG, IR01_PWSCTRL_POWER_ON);
		InitTuningRfCapOsc();
		Set_IF_Freq();
		TunerDriverSetMonoStereoMode(TUNER_MODE_STEREO); 
	}
	g_wFMTunerError.m_bPOWERED_OFF = bStandby; 

	return TUNER_SUCCESS;
} // End TunerDriverSetTunerStandby

_reentrant INT TunerDriverGetTunerstate(void)
{
	return TunerGetStatus();
} // End TunerDriverGetTunerstate

_reentrant INT TunerDriverSetPresetStations(INT iPresetStation)
{

	if ((iPresetStation-1<NUMBER_OF_PRESETS)&&(iPresetStation>0))
	{
		StoreFmPreset(iPresetStation-1, g_wCurrentFrequency, 1, iPresetStation);
        SysPostMessage(2,MENU_TUNER_TUNED);
	}
	else
		g_wFMTunerError.m_bPRESET_RANGE_ERROR=TRUE;
	return TUNER_SUCCESS;
} // End TunerDriverSetPresetStations

_reentrant INT TunerDriverErasePresetStations(INT iPresetStation)
{

	if ((iPresetStation-1<NUMBER_OF_PRESETS)&&(iPresetStation>0))
	{
		StoreFmPreset(iPresetStation-1, 0, 0, 0);
        SysPostMessage(2,MENU_TUNER_TUNED);
	}
	else
		g_wFMTunerError.m_bPRESET_RANGE_ERROR=TRUE;

	return TUNER_SUCCESS;
} // End TunerDriverErasePresetStations

_reentrant BOOL bIsTunerModulePowered(void)
{
	return(!g_wFMTunerError.m_bPOWERED_OFF);
} // End bIsTunerModulePowered

// * --------------------------------------------------------------------------------- *
// *               Digital Parser for LV2400x                                          *
// * --------------------------------------------------------------------------------- *
_reentrant void WriteReg(U16 wRegAddress, U8 byValue)
{
	// Select the block of the register
	SelectRegBlock(wRegAddress);
	// Write data to register 
 	IoWrite3W(byValue, LSB(wRegAddress)); // low byte of wRegAddress is Register offset
} // End WriteReg

/**

*** Copyright (c) SigmaTel, Inc. Unpublished

***

*** SigmaTel, Inc.

*** Proprietary & Confidential

***

*** This source code and the algorithms implemented therein constitute

*** confidential information and may comprise trade secrets of SigmaTel, Inc.

*** or its associates, and any use thereof is subject to the terms and

*** conditions of the Confidential Disclosure Agreement pursuant to which this

*** source code was originally received.

**/

/* ************************************************************************************************
 *
 *  Function:   ReadReg
 *
 *  Purpose:    Read a register from the chip
 *  Input:		wRegAddress: complete address (with block/offset) of the register to be read
 *  Output:     The 8 bit read data 
 *  Comments:   The block select register (BLK_SEL_REG) won't be restored after the read
 *
 * ************************************************************************************************
 * Copyright (c) 2004. All rights reserved.
 * ************************************************************************************************ */
_reentrant U8 ReadReg(U16 wRegAddress)
{
	// Select the block of the register
	SelectRegBlock(wRegAddress);

	// Read data at the register offset
	return(IoRead3W(LSB(wRegAddress)));

} // End ReadReg

_reentrant void SelectRegBlock(U16 wRegAddress)
{
	U8 byBlock;

	// High byte of wRegAddress is block number
	//byBlock = MSB(wRegAddress);	// Compiler error!!!
	if (wRegAddress & 0x100)
		byBlock = 1;
	else
		byBlock = 2;

	// Skip selecting the block when it's already selected
	if (g_byBlock != byBlock)
	{
		g_byBlock = byBlock;				// Save block number for next time
 		IoWrite3W(byBlock, BLK_SEL_REG);	// Write the block number to BLK_SEL register
		byBlock = MSB(wRegAddress);
	}
} // End SelectRegBlock

// * --------------------------------------------------------------------------------- *
// *               Low level I/O routines for Sanyo-LV2400x                            *
// * --------------------------------------------------------------------------------- *
#pragma optimize R
_reentrant void Sanyo3WireIoDelay(void)
{
	_nop();
	_nop();
	_nop();
} // End Sanyo3WireIoDelay
#pragma endoptimize

/**

*** Copyright (c) SigmaTel, Inc. Unpublished

***

*** SigmaTel, Inc.

*** Proprietary & Confidential

***

*** This source code and the algorithms implemented therein constitute

*** confidential information and may comprise trade secrets of SigmaTel, Inc.

*** or its associates, and any use thereof is subject to the terms and

*** conditions of the Confidential Disclosure Agreement pursuant to which this

*** source code was originally received.

**/


/* ************************************************************************************************
 *
 *  Function:   IoWrite3W
 *
 *  Purpose:    Write 16 bit data to 3W-device
 *  Input:		byLow: low byte of the data
 *				byHigh: high byte of the data
 *  Output:     Nothing
 *  Global:		None.
 *  Comments:   
 *		- The LSB of the low byte will be written first
 *		- This function uses DGT4-protocol (CLOCK is default high)
 *
 * ************************************************************************************************
 * Copyright (c) 2004. All rights reserved.
 * ************************************************************************************************ */
_reentrant void IoWrite3W(U8 byLow, U8 byHigh)
{
	// Prepare the 3-wire bus for writing
	HW_3WIRE_WRITE_ENABLE_DOR = TRUE; 		// nRW line high for writing
	HW_3WIRE_DATA_DOER = TRUE; 				// Program DATA PIN to output

	// Perfrom the 3W-write
	IoWriteByte3W(byLow);	// Write low byte
	IoWriteByte3W(byHigh);	// Write high byte

 	// Let the device latch data
	HW_3WIRE_DATA_DOER = FALSE; 			// Change DATA PIN to input mode
	HW_3WIRE_WRITE_ENABLE_DOR = FALSE; 		// nRW line low for latching data
} // End IoWrite3W

/**

*** Copyright (c) SigmaTel, Inc. Unpublished

***

*** SigmaTel, Inc.

*** Proprietary & Confidential

***

*** This source code and the algorithms implemented therein constitute

*** confidential information and may comprise trade secrets of SigmaTel, Inc.

*** or its associates, and any use thereof is subject to the terms and

*** conditions of the Confidential Disclosure Agreement pursuant to which this

*** source code was originally received.

**/


/* ************************************************************************************************
 *
 *  Function:   IoRead3W
 *
 *  Purpose:    Read 8 bit data from 3W-device
 *  Input:		byReg: 8 bit data to be sent before the read (usually the register address for the read)
 *  Output:     The read data byte
 *  Comments:   
 *		- The LSB will be read first
 *		- This function uses DGT4-protocol (CLOCK is default high)
 *
 * ************************************************************************************************
 * Copyright (c) 2004. All rights reserved.
 * ************************************************************************************************ */
_reentrant U8 IoRead3W(U8 byReg)
{
	INT iBitCnt;
	U8 byData;

	// Prepare 3-wire bus for writing
	HW_3WIRE_WRITE_ENABLE_DOR = TRUE; 		// nRW line high for writing
	HW_3WIRE_DATA_DOER = TRUE; 				// Program DATA PIN to output

	// Write the register offset
	IoWriteByte3W(byReg);

	// Prepare 3-wire bus for reading
	HW_3WIRE_DATA_DOER = FALSE; 			// Change DATA PIN to input mode
	HW_3WIRE_WRITE_ENABLE_DOR = FALSE; 		// nRW line low for latching data

	// Read the 8 bits data (LSB is read first)
   	byData = 0;
	for (iBitCnt=0; iBitCnt<8; iBitCnt++)
	{
		HW_3WIRE_CLOCK_DOR = FALSE;			// Clock low -> The chip shifts its data out
		Sanyo3WireIoDelay();
		byData>>=1;							// Make room for next bit (the first shift is dummy!)
		if (HW_3WIRE_DATA_DIR)				// Fetch the value of the DATA pin and patch into bit position
			byData |= 0x80;
		HW_3WIRE_CLOCK_DOR = TRUE;			// Clock high
		Sanyo3WireIoDelay();
	}

	// Return the read data
	return(byData);
} // End IoRead3W

/**

*** Copyright (c) SigmaTel, Inc. Unpublished

***

*** SigmaTel, Inc.

*** Proprietary & Confidential

***

*** This source code and the algorithms implemented therein constitute

*** confidential information and may comprise trade secrets of SigmaTel, Inc.

*** or its associates, and any use thereof is subject to the terms and

*** conditions of the Confidential Disclosure Agreement pursuant to which this

*** source code was originally received.

**/

/* ************************************************************************************************
 *
 *  Function:   IoWriteByte3W
 *
 *  Purpose:    Generate clock to send 8 bit data to 3W-device
 *  Input:		byData: 8 bit data to be sent
 *  Output:     Nothing
 *  Comments:   - This function is meant for internal usage (to be called by IoWrite3W or IoRead3W)
 *
 * ************************************************************************************************
 * Copyright (c) 2004. All rights reserved.
 * ************************************************************************************************ */
_reentrant void IoWriteByte3W(U8 byData)
{
	// Write 1 byte to the 3-wire device (LSB is written first)
	// Caller must prepare the bus for write mode before invoke this function
	INT iBitCnt;  
	// Number of data bit
    iBitCnt = 8;

	// Write the data to the chip: LSB first
	while (iBitCnt)
	{
		HW_3WIRE_CLOCK_DOR = FALSE;		// Clock low
		Sanyo3WireIoDelay();
		iBitCnt--;						// Update bit count, place here to use this as clock delay
		if (byData & 1)
		{
			HW_3WIRE_DATA_DOR = TRUE;	// Drive data pin high
		}
		else 
		{
			HW_3WIRE_DATA_DOR = FALSE;	// Drive data pin low
		}
		HW_3WIRE_CLOCK_DOR = TRUE;		// Clock high
		Sanyo3WireIoDelay();
		byData >>=1;					// Next bit
	}
} // End IoWriteByte3W
/**
*** Copyright (c) SigmaTel, Inc. Unpublished
***	 
*** SigmaTel, Inc. 
*** Proprietary & Confidential	
***	
*** This source code and the algorithms implemented therein constitute 
*** confidential information and may comprise trade secrets of SigmaTel, Inc. 
*** or its associates, and any use thereof is subject to the terms and 
*** conditions of the Confidential Disclosure Agreement pursuant to which this 
*** source code was originally received.
**/  

/* ************************************************************************************************
 *
 *  Function:   Set_IF_Freq
 *  Purpose:    Set the IF frequency to default IF (DEFAULT_IF_FREQ)
 *  Input:		Nothing
 *  Output:     Status as defined in LvErr.h
 *  Comments:	None
 *
 * ************************************************************************************************
 * Copyright (c) 2004. All rights reserved.
 * ************************************************************************************************ */
_reentrant void Set_IF_Freq(void)
{
	// turn on demodulator PLL mute to set the IF
	WriteReg(IR01_RADIO_CTRL2_REG, IR01_RADIOCTRL2_PM_ON);

	// Select correct oscillator output and enable measuring mode
	SetChipOutput(CO_IF_FREE);

	// Do the tuning 
	LinTuneDac(DEFAULT_IF_FREQ, LV_MSR_TIME_32ms, WriteIfOsc, 50, 150, 0);

	// Tuning done - restore status
	WriteReg(IR01_RADIO_CTRL2_REG, IR01_RADIOCTRL2_DEFAULT);	// Turn off the IF-PLL mute for normal operation mode
	SetChipOutput(CO_NONE);   

} // End Set_IF_Freq
/**

*** Copyright (c) SigmaTel, Inc. Unpublished

***

*** SigmaTel, Inc.

*** Proprietary & Confidential

***

*** This source code and the algorithms implemented therein constitute

*** confidential information and may comprise trade secrets of SigmaTel, Inc.

*** or its associates, and any use thereof is subject to the terms and

*** conditions of the Confidential Disclosure Agreement pursuant to which this

*** source code was originally received.

**/	

/* ************************************************************************************************
 *
 *  Function:   SetChipOutput
 *
 *  Authors:    Hung van Le
 *  Purpose:    Setup chip mode for measurement or normal mode
 *  Input:      
 *		byOutput: desired output (See CO_xxx definitions in Sanyo24000.h)
 *  Output:     The original output select 
 *  Comments:   Use this routine before measuring chip's frequencies. It manages the measurement mode 
 *				and the AFC state of the hardware
 *
 * ************************************************************************************************
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
_reentrant U8 SetChipOutput(U8 byOutput)
{
	U8  byCurOutput;

	// Setup the chip in correct mode
	if (byOutput == CO_NONE)	// Normal operation mode
	{
		// This enables the AFC and disables the chip measuremnt 
		WriteReg(IR01_RADIO_CTRL1_REG, IR01_RADIOCTRL1_NORMAL);
	}
	else
	{
		// Some output is needed - setup the chip in measuring mode
		WriteReg(IR01_RADIO_CTRL1_REG, IR01_RADIOCTRL1_MSR); // Enable measuring, disable AFC
	}

	// Select the desired oscillator output to measure
	// Determine current chip output
	byCurOutput = g_byChipOutput;

	// Do nothing if no change
	if ( byCurOutput != byOutput )
	{
		g_byChipOutput = byOutput;
		if (byOutput==CO_IF_FREE)	// Convert the software defined value to hardware value
			byOutput = CO_IF_OSC;	// IF-free and IF-lock is same output of LV2400x (ie IF out)
		WriteReg(IR01_MSRC_SEL_REG, (IR01_MSRCSEL_DEFAULT|byOutput));
	}

	return(byCurOutput);
} // End SetChipOutput

   

/**

*** Copyright (c) SigmaTel, Inc. Unpublished

***

*** SigmaTel, Inc.

*** Proprietary & Confidential

***

*** This source code and the algorithms implemented therein constitute

*** confidential information and may comprise trade secrets of SigmaTel, Inc.

*** or its associates, and any use thereof is subject to the terms and

*** conditions of the Confidential Disclosure Agreement pursuant to which this

*** source code was originally received.

**/

/* ************************************************************************************************
 *
 *  Function:   Virtualize register access to the hardware
 *			WriteIfOsc (adjust the DAC of IF PLL )
 *			WriteSdOsc (adjust the DAC of Stereo decoder clock)
 *			WriteRfOsc (adjust the DAC of RF varicap)
 *  Authors:    Hung van Le
 *  Purpose:    These functions provide access to registers of the device which are used for setting frequencies
 *  Comments:   None
 *
 * ************************************************************************************************
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
_reentrant U8 WriteIfOsc(U16 wIfOscValue) 
{
	U16 wTmp;

	// Positive IF osc (ie IF frequency increases when the osc value increases)
	// V1.3E skipped
	//if (wIfOscValue & 0xFF00)
	//	return(LVLS_IFOSC_OUTOFLIMIT_ERR);
	WriteReg(IR01_IF_OSC_REG, (U8)wIfOscValue);

	// Program IF Center Frequency Oscillator and IF bandwidth according to the value of IF_OSC
	WriteReg(IR01_IFCEN_OSC_REG, (U8)wIfOscValue);	// IF center has the same value as IF DAC

	// IF BW is 65% of IF DAC value
	wTmp = wIfOscValue*65;
	wTmp /= 100;

#ifdef USE_SCAN_FS
	// Also save the band width setting (used during scanning)
	g_byIfBW = (U8)wTmp;
#endif //USE_SCAN_FS
	WriteReg(IR01_IF_BW_REG, (U8)wTmp);	

	return(LVLS_NO_ERROR);
} // End WriteIfOsc

_reentrant U8 WriteSdOsc(U16 wSdOscValue)
{
	// Positive Stereo decoder clock (ie SD frequency increases when the osc value increases)
	// V1.3E skipped
	//if (wSdOscValue & 0xFF00)
	//	return(LVLS_STEREOSC_OUTOFLIMIT_ERR);
    WriteReg(IR01_SD_OSC_REG, (U8)wSdOscValue);
	return(LVLS_NO_ERROR);
} // End WriteSdOsc

_reentrant U8 WriteRfOsc(U16 wRfOscValue)
{
	// Positive FM osc (ie FM frequency increases when the osc value increases)
	if (wRfOscValue & 0xFF00)
		return(LVLS_RFOSC_OUTOFLIMIT_ERR);

	WriteReg(IR01_FM_OSC_REG, (U8)wRfOscValue);
	return(LVLS_NO_ERROR);
} // End WriteRfOsc

/**

*** Copyright (c) SigmaTel, Inc. Unpublished

***

*** SigmaTel, Inc.

*** Proprietary & Confidential

***

*** This source code and the algorithms implemented therein constitute

*** confidential information and may comprise trade secrets of SigmaTel, Inc.

*** or its associates, and any use thereof is subject to the terms and

*** conditions of the Confidential Disclosure Agreement pursuant to which this

*** source code was originally received.

**/  

/* ************************************************************************************************
 *
 *  Function:   WriteRfCap
 *  Authors:    Hung van Le
 *  Purpose:    Virtualized register access to FM-capacitor bank switch
 *  Input:		
 *			wCapValue: the logical CAP value (will be converted to hardware value)
 *  Output:     Status as defined in LvErr.h
 *  Comments:
 *		Write the CAP bank select - using software logical value to deal with 7.5 bits FM CAP
 *			Logical value	Physical value
 *			0...63			255...193
 *			64..191			127...0
 *		Following conversions are used:
 *			- Hardware to software conversion: 
 *				if hw<128 
 *					sw=~(hw+64) 
 *				else 
 *					sw=~hw
 *			- Software to hardware conversion: 
 *				if sw<64 
 *					hw=~sw 
 *				else 
 *					hw=~sw-64
 *
 * ************************************************************************************************
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
_reentrant U8 WriteRfCap(U16 wCapValue)
{
	// Make sure value is valid
	if ( wCapValue > QSS_MAX_RFCAP ) // Value exceeds limit
		return(LVLS_RFCAP_OUTOFLIMIT_ERR);
	// Remember current CAP-value for reusing it by next frequency setting
    g_byCurCap = wCapValue;

	// Convert the software value to hardware value before writing it
	if (wCapValue < 64)
		wCapValue = 255 - wCapValue;	
	else
		wCapValue = 255 - wCapValue - 64;

	// Write value to the chip
	WriteReg(IR01_FM_CAP_REG, (U8)wCapValue);
	return(LVLS_NO_ERROR);
} // End WriteRfCap

/**

*** Copyright (c) SigmaTel, Inc. Unpublished

***

*** SigmaTel, Inc.

*** Proprietary & Confidential

***

*** This source code and the algorithms implemented therein constitute

*** confidential information and may comprise trade secrets of SigmaTel, Inc.

*** or its associates, and any use thereof is subject to the terms and

*** conditions of the Confidential Disclosure Agreement pursuant to which this

*** source code was originally received.

**/

/* ************************************************************************************************
 *
 *  Function:   CountPulseWith12MHzClk
 *
 *  Authors:    Hung van Le
 *  Purpose:    Counting the pulse (Measure frequencies) of the currently selected chip's output.
 *				Measuring interval is made with external clock
 *  Input:      
 *				u24TimeUs: time to measure in us
 *				pu24Freq: buffer to receive the measured frequency (unit depends on chip output)
 *  Output:     Status as defined in LvErr.h
 *  Comments:   
 *		The code is fixed for 12MHz external clock (counter swapping enabled) and 32kHz clock (counter swapping disabled)
 *		Counter 2 usage (with swapping)
 *			RF (100 MHz): IR1_CTAB_128, 128 (measuring time 0.66ms - 127ppm)
 *			RF (100 MHz): IR1_CTAB_512, 512 (measuring time 2.62ms - 32ppm)
 *
 *			IF (45  kHz): IR1_CTAB_128, 128 (overflow !)
 *			IF (100 kHz): IR1_CTAB_128, 128 (measuring time 2.56ms - 33ppm)
 *			
 *			IF (45  kHz): IR1_CTAB_32, 32 (measuring time 1.42ms - 59ppm)
 *			IF (100 kHz): IR1_CTAB_32, 32 (measuring time 0.64ms - 130ppm)
 *			
 *			SD (38  kHz): IR1_CTAB_32, 32 (measuring time 1.68ms - 49ppm)
 *			
 *			RDS(57  kHz): IR1_CTAB_32, 32 (measuring time 1.12ms - 74ppm)
 *			
 *		Because of short measuring time, this method is not suitable for measuring IF/SD/RDS PLL in lock mode.
 *		In free running mode (calibration mode), this method can be used.
 *
 *		RF is not PLL, so this method always works
 *		We use following:
 *			RF 128	(the count in swapping mode is 128*IMR01_CNT2_PRESCALER = 256)
 *			IF/SD in free running mode: 32 ( the count in swapping mode is 32*IMR01_CNT2_PRESCALER = 64)
 *
 * ************************************************************************************************
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
#ifdef FM_TUNER_USE_PWM0
_reentrant U8 CountPulseWith12MHzClk(PU24 pu24Freq)
{
#define EXT_CLK_12MHZ	((U24)12*MHz)	// Clock rate for RF/SD/IF-free(generated with PWM0)
#define EXT_CLK_32KHZ	((U24)32*KHz)	// Clock rate for IF-lock (generated with PWM0)
#define EXT_CLK_FOR_RF	((DWORD)(EXT_CLK_12MHZ/1000)*IMR01_FM_DIVIDER)	// External clock for RF (1 kHz unit for RF)

	U8   byResult;
	U16  wPulseCnt1, wPulseCnt2;
	U8   byCntCtrl;
	DWORD dwFreq, dwTimeOut;
	int  iOrgCcr, iOrgPwmCsr;

	// Return value 0 means no pulses counted (chip removed?)
	//*pu24Freq = 0;
	dwFreq = EXT_CLK_FOR_RF;	// avoid compiler's warning
	byResult = LVLS_NO_ERROR;

	// Use 12MHz PMW0 clock for all measurements
	if ( g_byChipOutput == CO_RF_OSC)
	{
		byCntCtrl = IR1_CTAB_128|IR1_CCTL_CNT_SEL;	// Use counter 2, set tab value, enable counter swapping
		wPulseCnt2 = 128*IMR01_CNT2_PRESCALER;		// Pulses for counter 2
		//dwFreq = EXT_CLK_FOR_RF;
	}
#ifdef USE_SCAN_IF
	else if ( g_byChipOutput == CO_IF_OSC)
	{
		// IF: (usede by scanning & free running): no swap counter, tab=512 (measure 32ms by 32kHz clock)
		byCntCtrl = IR1_CTAB_512|IR1_CCTL_CNT_SEL|IR1_CCTL_SWP_CNT_L;	
		wPulseCnt2 = (U16)512 *IMR01_CNT2_PRESCALER;	// Pulses for counter 2
		dwFreq = EXT_CLK_32KHZ;
	}
#endif //USE_SCAN_IF
	else
	{
		// Other frequencies (Stereo decoder, IF free running): swap counter, tab=32
		byCntCtrl = IR1_CTAB_32|IR1_CCTL_CNT_SEL;	// Use counter 2, set tab value, enable counter swapping
		wPulseCnt2 = 32 *IMR01_CNT2_PRESCALER;		// Pulses for counter 2
		dwFreq = EXT_CLK_12MHZ;
	}
		
    // Set up PWM0 of SMTP35xx for highest PWM output frequency, which is equal to the crystal 
    // clock/2.  We'll end up with a 50% duty cycle PWM output.
    // 1. PWM0 does not use the predivider, so reference to clock for this PWM refers 
    //    to the crystal clock. 
    // 2. Set the period to 2 clock cycles.
    // 3. Set the cycle delay for outputting the active state to 0 for no delay.
    // 4. Set the cycle delay for outputting the inactive state to 1 for a single 
    //    cycle delay.

	iOrgCcr = HW_CCR.I;
	iOrgPwmCsr = HW_PWM_CSR.I;

	// Enable analog and crystal clocks
    HW_CCR.I |= 0x400009;			

#ifdef USE_SCAN_FS
	// Program clock rate (12MHz clock)
	//HW_PWM_CH0AR.B.ACTIVE = 0;		// Number of clock cycles to count before resetting ACTIVE flip flop
	//HW_PWM_CH0AR.B.INACTIVE = 1;		// Number of clock cycles to count before resetting INACTIVE flip flop
	HW_PWM_CH0AR.I = ((U24)1<<12) | ((U24)0<<0);	// Equivalent of above code
	/*
	HW_PWM_CH0BR.I = 0; // clear all
	HW_PWM_CH0BR.I |= ((U24)3<<14);		// Active state = high (bit[15:14]=11b)
	HW_PWM_CH0BR.I |= ((U24)2<<12);		// Inactive state = low (bit[13:12]=10b)
	HW_PWM_CH0BR.I |= 1;				// Sets period to 2 (crystal) clock cycles
	*/
	// Equivalent of above code
	HW_PWM_CH0BR.I = ((U24)3<<14) | ((U24)2<<12) | 1;	// 12MHz clock
#endif //USE_SCAN_FS

#ifdef USE_SCAN_IF
	// Program PWM0 at 12MHz when clock swapping is enabled. Otherwise 32kHz
	if (byCntCtrl & IR1_CCTL_SWP_CNT_L) // if counter is not swapped
	{
		// 32KHz clock
		HW_PWM_CH0AR.I = ((U24)749<<12) | ((U24)349<<0);
		HW_PWM_CH0BR.I = ((U24)3<<14) | ((U24)2<<12) | 749;
	}
	else
	{
		// 12MHz clock
		HW_PWM_CH0AR.I = ((U24)1<<12) | ((U24)0<<0);
		HW_PWM_CH0BR.I = ((U24)3<<14) | ((U24)2<<12) | 1;	
	}
#endif //USE_SCAN_IF

	// Enable PWM0
    HW_PWM_CSR.B.MSTR_EN = 1;		// Enable PWM channel operation
    HW_PWM_CSR.B.PWM0_EN = 1;		// Enable PWM0 channel operation
	// --- End PWM0 setup ------

	// Program swap counter bit, tab bit, select counter 2
	WriteReg(IR01_CNT_CTRL_REG, (byCntCtrl|IR1_CCTL_CNT1_CLR)); // Drive Clear counter bit of high
	WriteReg(IR01_CNT_CTRL_REG, byCntCtrl);						// Drive Clear counter bit of low (counter 1 is reset)

	// Enable counter to start counting
	WriteReg(IR01_CNT_CTRL_REG, byCntCtrl|IR1_CCTL_CNT_EN);

	
	// Wait counting done by polling interrupt status register for counter 2 interrupt
	dwTimeOut = SysGetCurrentTime() + 40;	// Timeout after 40ms
	do
	{
		// Query counter 2 interrupt flag
		if (ReadReg(IR01_IRQ_ID_REG) & IR1_IRQID_CNT2)	
			break;


		
		// Check timeout
		if (SysGetCurrentTime() > dwTimeOut)
			byResult = LVLS_POLLING_CNT_TIMEOUT;
	} while (byResult==LVLS_NO_ERROR);
		
	// Counting done - Disable the counter (also clear IR1_IRQID_CNT2 bit)
	WriteReg(IR01_CNT_CTRL_REG, byCntCtrl);

	// Restore PWM settings
	HW_PWM_CSR.I = iOrgPwmCsr;
	HW_CCR.I = iOrgCcr;
	
	// Calculate and save the measured frequency to simulate the (frequency) read-register


	if (byResult == LVLS_NO_ERROR)


	{
		// Read counter1 of LV2400x
		wPulseCnt1 = ReadReg(IR01_CNT_H_REG);	// High byte
		wPulseCnt1 <<=8;
		wPulseCnt1 |= ReadReg(IR01_CNT_L_REG);	// Patch low byte





		// Formula to calculate frequency from pulse counts
		//	if NoSwapping
		//		f = (N1 * fext * DividerFactor)/N2
		//  if Swapping
		//		f = (N2 * fext * DividerFactor)/N1
		//	(N2 is value of counter 2 = TabValue * CNT2_div_factor)
		// When we're here, dwFreq = f_ext * DividerFactor
#ifdef USE_SCAN_IF
		if (byCntCtrl & IR1_CCTL_SWP_CNT_L) // if counter is not swapped
		{
			// Swap the 2 count values
			iOrgCcr = wPulseCnt1;
			wPulseCnt1 = wPulseCnt2;
			wPulseCnt2 = (U16)iOrgCcr;
		}
#endif //USE_SCAN_IF
		dwFreq *= (DWORD)wPulseCnt2;
		*pu24Freq = (U24)(dwFreq/wPulseCnt1);
	}

	return(byResult);
} // End CountPulseWith12MHzClk
#endif //FM_TUNER_USE_PWM0

/**

*** Copyright (c) SigmaTel, Inc. Unpublished

***

*** SigmaTel, Inc.

*** Proprietary & Confidential

***

*** This source code and the algorithms implemented therein constitute

*** confidential information and may comprise trade secrets of SigmaTel, Inc.

*** or its associates, and any use thereof is subject to the terms and

*** conditions of the Confidential Disclosure Agreement pursuant to which this

*** source code was originally received.

**/
/* ************************************************************************************************
 *
 *  Function:   CountPulseWithTimer3
 *
 *  Authors:    Hung van Le
 *  Purpose:    Counting the pulse (Measure frequencies) of the currently 
 *		selected chip's output. Software is responsible for measuring interval
 *  Input:      
 *		u24TimeUs: time to measure in us
 *		pwFreq: buffer to receive the measured frequency (unit depends on chip output)
 *  Output:     Status as defined in LvErr.h
 *  Comments:   
 *		- This function programs the LV2400x for counting pulse (reset counter...) with counter 1 and uses 
 *		  timer 3 polling to do the time critical part (pulse NR_W low to enabling the LV2400x's counter).
 *		  With timer 3, the extra overhead time can be determined sothat the accurate frequency can be calculated.
 *		- This function is not required when the external clock is used for frequency measurements
 *
 * ************************************************************************************************
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
#ifdef USE_TMR3
_reentrant U8 CountPulseWithTimer3(U24 u24TimeUs, PU24 pu24Freq)
{
	// SMTP35xx timer 3 specific constances
#define _SMTP35XX_TMR3_CLK_MHZ	24		// Timer 3 clock frequency in MHz
#define _SMTP35XX_TMR3_DIV		64		// Timer 3 clock divider mode 3 - Should be 64 !!!
	
	U48 dwFreq;
	int iOrgTmr3Ctrl;		// Org. timer 3 control value 
	int iOrgHwCcr;			// Org. clock control register value
	U24 u24TmrCnt;
	U8	byResult;
	//WORD wOrgSysIrqLevel;	// Needed for storing the system interrupt level

	// Return value 0 means no pulses counted
	//*pu24Freq = 0;
	byResult = LVLS_NO_ERROR;

	// Reset counter
	WriteReg(IR01_CNT_CTRL_REG, (IR1_CCTL_SWP_CNT_L|IR1_CCTL_CNT1_CLR)); // Drive Clear counter bit of high - don't swap te counters
	WriteReg(IR01_CNT_CTRL_REG, IR1_CCTL_SWP_CNT_L);					// Drive Clear counter bit of low (counter 1 is reset)
		
	// -------------
	// Start LV2400x counter and keep the NRW line low during specified measuring time (timed by timer 3 of SMTP35xx)
	// -------------
	// Save state of timer 3 - Reading this register also clears the timer 3 status bit
	iOrgTmr3Ctrl = HW_TMR3CSR.I;
	iOrgHwCcr = HW_CCR.I;

	// Prepare SMTP35xx-timer 3 for writing
	HW_CCR.B.CKRST = 1;		// Clock Reset - must be 1 before writing to CCR
	HW_CCR.B.CKSRC = 1;		// Digital clock source = crystal
	HW_CCR.B.ACKEN = 1;		// Analog clock enable (must be 1 for using RTC, Alarm, PWM, Timers, DAC or ADC
	HW_CCR.B.XTLEN = 1;		// XTLEN must be 1 for using the crystal as timer's clock 
	HW_TMR3CSR.B.CLKGT = 0;	// Clock gate - must be 0 before writing to TMR3 control register
	HW_TMR3CSR.I = 0;		// Clear all bits of TMR3 control register

	// Program timer 3:
	HW_TMR3CSR.B.TIMER_MODE = 3;		// timer mode = 0:DSP_clk/2, 1:24MHz/4=6MHz, 2:24MHz/16=1.5MHz, 3:24MHz/64=375 kHz
	//HW_TMR3CSR.B.TIMER_CONTROL = 0;	// timer control = 000b: internal clock, downcount, no output - Already done
	//HW_TMR3CSR.B.TIMER_ENABLE = 0;	// timer enable = 0: don't enable the timer yet - Already done

	// Calculate the initial count for timer 3 (N = f*t where f in MHz, t in us. (f=24MHz/divider)
	u24TmrCnt = (u24TimeUs * _SMTP35XX_TMR3_CLK_MHZ) / _SMTP35XX_TMR3_DIV;

	// Program the initial count into timer 3
	HW_TMR3CNTR.B.COUNT = u24TmrCnt;

	// Start the 2 counters
	//wOrgSysIrqLevel = SysMaskAllInterrupts();	// No interrupt for the next 2 statement
	WriteReg(IR01_CNT_CTRL_REG, IR1_CCTL_SWP_CNT_L|IR1_CCTL_CNT_EN); // Enable counter of LV2400x
	HW_TMR3CSR.B.TIMER_ENABLE = 1;				// Start timer 3 of SMTP35xx
	//SysUnMaskAllInterrupts(wOrgSysIrqLevel);	// Restore interrupt

	// polling the timer status
	dwFreq = SysGetCurrentTime() + 150;			// Temporary use dwFreq as time out count
	while (HW_TMR3CSR.B.TIMER_STATUS != 1)
	{
		if (SysGetCurrentTime() > dwFreq)
		{
			byResult = LVLS_POLLING_CNT_TIMEOUT;
			break;
		}
	}

	// Timer expired. Stop the 2 counters
	//wOrgSysIrqLevel = SysMaskAllInterrupts();	// No interrupt for the next 2 statement
	HW_3WIRE_WRITE_ENABLE_DOR = TRUE; 			// nRW line high to let LV2400x stop its counter
	HW_TMR3CSR.B.TIMER_ENABLE = 0;				// Stop timer 3
	//SysUnMaskAllInterrupts(wOrgSysIrqLevel);	// Restore interrupt

	// Change DATA-pin to output: place here to avoid software overhead between stopping the 2 counters
	HW_3WIRE_DATA_DOER = TRUE; 					// Program DATA PIN to output
	WriteReg(IR01_CNT_CTRL_REG, IR1_CCTL_SWP_CNT_L); // Disable counter of LV2400x

	// Calculate the total count = initial count + extra count
	u24TmrCnt += (u24TmrCnt - (U24)HW_TMR3CNTR.B.COUNT);

	// Convert the count to time (in us) t = N/f with f in MHz, t is in us
	// This is the actual time which was used 
//	u24TimeUs = (u24TmrCnt *_SMTP35XX_TMR3_DIV)/_SMTP35XX_TMR3_CLK_MHZ;
	u24TimeUs = (u24TmrCnt *(_SMTP35XX_TMR3_DIV/2))/_SMTP35XX_TMR3_CLK_MHZ;

	// Restore the state of timer 3
	HW_CCR.I = iOrgHwCcr;
	HW_TMR3CSR.I= iOrgTmr3Ctrl;
	// ----------
	// End Pulse NRW and timer 3 handling
	// ----------

	// Read pulse count from counter 1 of LV2400x
	if (byResult == LVLS_NO_ERROR)
	{
		dwFreq = ReadReg(IR01_CNT_H_REG);	// High byte
		dwFreq <<=8;
		dwFreq |= ReadReg(IR01_CNT_L_REG);	// Patch low byte
		
		// Convert pulse count to frequency: Use formula f = n * d / t
		//		where:	f: frequency in Hz
		//				n: pulse count
		//				d: divider factor
		//				t: measured time in us
		if ( g_byChipOutput == CO_RF_OSC)
			dwFreq *= (IMR01_FM_DIVIDER * 1000);	// 1 KHz unit for RF
		else
			dwFreq *= 1000000; // 1 Hz unit for other freq.
		*pu24Freq = (U24)(dwFreq/u24TimeUs);
	}
	else
	{
		*pu24Freq = 0;
	}
	return(byResult);
} // End CountPulseWithTimer3
#endif //USE_TMR3
/**

*** Copyright (c) SigmaTel, Inc. Unpublished

***

*** SigmaTel, Inc.

*** Proprietary & Confidential

***

*** This source code and the algorithms implemented therein constitute

*** confidential information and may comprise trade secrets of SigmaTel, Inc.

*** or its associates, and any use thereof is subject to the terms and

*** conditions of the Confidential Disclosure Agreement pursuant to which this

*** source code was originally received.

**/
/* ************************************************************************************************
 *
 *  Function:   CountPulseWith24MHzClk
 *
 *  Authors:    Hung van Le
 *  Purpose:    Counting the pulse (Measure frequencies) of the currently selected chip's output.
 *				Measuring interval is made with external clock
 *  Input:      
 *				pu24Freq: buffer to receive the measured frequency (unit depends on chip output)
 *  Output:     Status as defined in LvErr.h
 *  Comments:   
 *		The code is fixed for 24MHz external clock
 *		Counter 2 usage
 *			RF (100 MHz): IR1_CTAB_128, swap (measuring time 0.66ms - 64ppm)
 *			RF (100 MHz): IR1_CTAB_32,  swap (measuring time 0.16ms - 254ppm)
 *
 *			IF-free(100 kHz): IR1_CTAB_32, swap (measuring time 0.64ms - 65ppm)
 *			IF-lock(100 kHz): IR1_CTAB_32768, no swap (measuring time 2.73ms - average 5000ppm)
 *			
 *			SD-free(38  kHz): IR1_CTAB_32, swap (measuring time 1.68ms - 25ppm)
 *			
 *		We use following:
 *			RF 128	(the count in swapping mode is 128*IMR01_CNT2_PRESCALER = 256)
 *			IF/SD in free running mode: 32 ( the count in swapping mode is 32*IMR01_CNT2_PRESCALER = 64)
 *			IF-lock: 5 loop with 32768-tab, no swap. Deviation is 5000ppm, but for IF lock, 
 *					 deviation up to 2% is allowable.
 *
 * ************************************************************************************************
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
#if CD_24MHz  // not used
_reentrant U8 CountPulseWith24MHzClk(PU24 pu24Freq)
{
#define EXT_CLK_24MHZ	(24000)			// External clock rate in kHz
#define EXT_CLK_FOR_RF	(EXT_CLK_24MHZ*IMR01_FM_DIVIDER)	// External clock for RF (1 kHz unit for RF)
	U8   byResult;
	U16  wPulseCnt1, wPulseCnt2;
	U8   byCntCtrl;
	DWORD dwFreq, dwTimeOut;
#ifdef USE_SCAN_IF
	int  i, iMsrLoop;
#else
	#define iMsrLoop 1
#endif //USE_SCAN_IF

	// Return value 0 means no pulses counted (chip removed?)
	//*pu24Freq = 0;
	dwFreq = EXT_CLK_FOR_RF;	// avoid compiler's warning
	byResult = LVLS_NO_ERROR;
#ifdef USE_SCAN_IF
	iMsrLoop = 1;
#endif //USE_SCAN_IF

	// Determine counter 2 tab setting
	if ( g_byChipOutput == CO_RF_OSC)
	{
		byCntCtrl = IR1_CTAB_128|IR1_CCTL_CNT_SEL;	// Use counter 2, set tab value, enable counter swapping
		wPulseCnt2 = 128*IMR01_CNT2_PRESCALER;		// Pulses for counter 2
		//dwFreq = EXT_CLK_FOR_RF;
	}
#ifdef USE_SCAN_IF
	else if ( g_byChipOutput == CO_IF_OSC)
	{
		// IF: (used by scanning): no swap counter, tab=32768 (measure 2.73ms by 24MHz clock)
		byCntCtrl = IR1_CTAB_32768|IR1_CCTL_CNT_SEL|IR1_CCTL_SWP_CNT_L;	

		// Lower the count and frequency with factor 512 to avoid overflow by calculation
		wPulseCnt2 = (U16)(32768/512)*IMR01_CNT2_PRESCALER;	// Pulses for counter 2
		dwFreq = ((DWORD)EXT_CLK_24MHZ*1000)/512;
		iMsrLoop = 5;		// Number of measuring loops to be done
	}
#endif //USE_SCAN_IF
	else
	{
		// Other frequencies (Stereo decoder, IF free running): swap counter, tab=32
		byCntCtrl = IR1_CTAB_32|IR1_CCTL_CNT_SEL;	// Use counter 2, set tab value, enable counter swapping
		wPulseCnt2 = 32 * IMR01_CNT2_PRESCALER;		// Pulses for counter 2
		dwFreq = (DWORD)EXT_CLK_24MHZ*1000;
	}
		
	// Program swap counter bit, tab bit, select counter 2
	WriteReg(IR01_CNT_CTRL_REG, (byCntCtrl|IR1_CCTL_CNT1_CLR)); // Drive Clear counter bit of high
	WriteReg(IR01_CNT_CTRL_REG, byCntCtrl);						// Drive Clear counter bit of low (counter 1 is reset)

	// Perform measuring
#ifdef USE_SCAN_IF
	for (i=0; (i<iMsrLoop) && (byResult==LVLS_NO_ERROR); i++)
#endif //USE_SCAN_IF
	{
		// Enable counter to start counting
		WriteReg(IR01_CNT_CTRL_REG, byCntCtrl|IR1_CCTL_CNT_EN);
		
		// Wait counting done by polling interrupt status register for counter 2 interrupt
		dwTimeOut = SysGetCurrentTime() + 15;	// Timeout after 15ms
		do
		{
			// Query counter 2 interrupt flag
			if (ReadReg(IR01_IRQ_ID_REG) & IR1_IRQID_CNT2)	
				break;
		
			// Check timeout
			if (SysGetCurrentTime() > dwTimeOut)
				byResult = LVLS_POLLING_CNT_TIMEOUT;
		} while (byResult==LVLS_NO_ERROR);
		
		// Counting done - Disable the counter (also clear IR1_IRQID_CNT2 bit)
		WriteReg(IR01_CNT_CTRL_REG, byCntCtrl);
	}

	if (byResult == LVLS_NO_ERROR)
	{
		// Read pulses counted from counter1 of LV2400x
		wPulseCnt1 = ReadReg(IR01_CNT_H_REG);	// High byte
		wPulseCnt1 <<=8;
		wPulseCnt1 |= ReadReg(IR01_CNT_L_REG);	// Patch low byte
	
		// Convert the pulses to frequency
		// Formula to calculate frequency from pulse counts
		//	if NoSwapping
		//		f = (N1 * fext * DividerFactor)/N2
		//  if Swapping
		//		f = (N2 * fext * DividerFactor)/N1
		//	(N2 is value of counter 2 = TabValue * CNT2_div_factor)
		// When we're here, dwFreq = f_ext * DividerFactor
#ifdef USE_SCAN_IF
		if (byCntCtrl & IR1_CCTL_SWP_CNT_L) // if counter is not swapped
		{
			// Swap the 2 count values
			i = wPulseCnt1;
			wPulseCnt1 = wPulseCnt2;
			wPulseCnt2 = (U16)i;
		}
#endif //USE_SCAN_IF
				dwFreq *= (DWORD)wPulseCnt2;
		*pu24Freq = (U24)((dwFreq/wPulseCnt1)/iMsrLoop);
	}
	return(byResult);
} // End CountPulseWith24MHzClk
#endif //CD_24MHz

/**

*** Copyright (c) SigmaTel, Inc. Unpublished

***

*** SigmaTel, Inc.

*** Proprietary & Confidential

***

*** This source code and the algorithms implemented therein constitute

*** confidential information and may comprise trade secrets of SigmaTel, Inc.

*** or its associates, and any use thereof is subject to the terms and

*** conditions of the Confidential Disclosure Agreement pursuant to which this

*** source code was originally received.
**/
_reentrant U8 GetFmFieldStrength(void)
{
	U8 byRegValue;
	U8 byFs;
		
	// Read Radio status register to determine field strength
	byRegValue = ReadReg(IR01_RADIO_STAT_REG);

	// Extract FieldStrength
	byRegValue &= IR1_RSTAT_FS; // extract field strength bits
	byRegValue ^= IR1_RSTAT_FS; // Invert field strength level (0 becomes 1)
	
	byFs = 0; // field strength to be returned
	while (byRegValue != 0)
	{
		if ( byRegValue & 1 )
			byFs++;
		byRegValue>>=1;
	}
	
	return(byFs);
} // End GetFmFieldStrength


/**

*** Copyright (c) SigmaTel, Inc. Unpublished

***

*** SigmaTel, Inc.

*** Proprietary & Confidential

***

*** This source code and the algorithms implemented therein constitute

*** confidential information and may comprise trade secrets of SigmaTel, Inc.

*** or its associates, and any use thereof is subject to the terms and

*** conditions of the Confidential Disclosure Agreement pursuant to which this

*** source code was originally received.

**/
/* ************************************************************************************************
 *
 *  Function:   IsFrequencyOk
 *
 *  Authors:    Hung van Le
 *  Purpose:    Verify if u24CurFreq is in range of u24ExpFreq +/- margin
 *  Input:
 *			u24CurFreq: current frequency
 *			u24ExpFreq: the expected frequency
 *			u24Precise: precise of the compare (max=1000000, 1000000 is exact comparing)
 *  Output:
 *			0 : u24CurFreq within the margin
 *			-1: u24CurFreq too low (i.e. u24CurFreq < (u24ExpFreq - margin)
 *			+1: u24CurFreq too high (i.e. u24CurFreq > (u24ExpFreq + margin)
 *  Comments:   margin = (1000000 * DividerFactor) / u24Precise
 *		Divider factor is dependant on current output select of the chip
 *
 * ************************************************************************************************
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
_reentrant int IsFrequencyOk(U24 u24CurFreq, U24 u24ExpFreq, U24 u24Precise)
{
	U24 wMargin;
	U24 dwFactor;

   	if ( g_byChipOutput == CO_RF_OSC )
		dwFactor = 1000 * IMR01_FM_DIVIDER;	// 1 KHz unit for RF
	else
		dwFactor = 100000;	// 1 Hz unit for other frequencies
	wMargin = (U24)(dwFactor / u24Precise);

	if ( u24CurFreq < (u24ExpFreq - wMargin) )
		return(-1);
	else if ( u24CurFreq > (u24ExpFreq + wMargin) )
		return(1);
	else
		return(0);
} // End IsFrequencyOk



/**

*** Copyright (c) SigmaTel, Inc. Unpublished

***

*** SigmaTel, Inc.

*** Proprietary & Confidential

***

*** This source code and the algorithms implemented therein constitute

*** confidential information and may comprise trade secrets of SigmaTel, Inc.

*** or its associates, and any use thereof is subject to the terms and

*** conditions of the Confidential Disclosure Agreement pursuant to which this

*** source code was originally received.

**/
/*-------------------------------------------------------------------
        Algorithm routines
-------------------------------------------------------------------*/

/* ************************************************************************************************
 *
 *  Function:   LinTuneDac
 *
 *  Authors:    Hung van Le
 *  Purpose:    Generic routine to tune all OSC registers of the chip (using lineair interpolating)
 *  Input:      
 *			 u24ExpFreq: expected frequency
 *			 dwMeasureTimeUs: measure time in us (also used for determining the measuring deviation)
 *			 fpWriteOsc: pointer to Write_xxx_Osc routine
 *			 wX1, wX2: start X value - see comment
 *			 iOscRes: Osc resolution in 10Hz- This is only meant for RF-frequency (performance gain)
 *  Output:     Status as defined in LvErr.h
 *  Comments:
 *		Algorithm:
 *			x1 = point1, measure f1
 *			x2 = point2;
 *			Loop
 *				write x2
 *				measure f2
 *				xe = interpolate(x1,f1,x2,f2,fexp)
 *				fe = measure
 *				check fe against fexp, exit if OK
 *				Move X2,f2 to X1, f1; xe,fe to X2,f2
 *			goto loop
 *
 *		Input wX1, wX2:
 *			use wX1, wX2 for point 1,2
 *
 * ************************************************************************************************
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
_reentrant U8 LinTuneDac(U24 u24ExpFreq, U24 dwMeasureTimeUs, FPTR_WRITE_OSC fpWriteOsc, U16 wX1, U16 wX2, int iOscRes)
{
	U8   byResult;
	U24  wY1, wY2;
	U16  wOscValue;
	int  iStep, iCompRes;
	BOOL bDone;

	// Init var.
	bDone = FALSE;
	//iStep = 0; // Avoid compiler's complain
	iCompRes = 0;

	// ----- Draw a line to approach the desired point --------
	// Determine point1
	fpWriteOsc(wX1);				// Set X1
	byResult = mCNTPULSE_FUNC(dwMeasureTimeUs, &wY1);	// Get Y1
	if (byResult != LVLS_NO_ERROR)
		goto return_LinTuneDac;
	if ( IsFrequencyOk(wY1, u24ExpFreq, dwMeasureTimeUs) == 0 )
		bDone = TRUE;		// frequency within margin
	
	// Determine wX2 when iOscRes is specified and u24ExpFreq is not approached.
	// Using of iOscRes is special: it's only meant for RF-frequency
	// So wY1, wY2 are assumed to be RF-frequency in kHz when iOscRes is not 0.
	// iOscRes, when not 0, specifies the RF-OSC resolution in 10Hz
	if ( (!bDone) && ( iOscRes !=0) )
	{
		iStep = ((int)u24ExpFreq - (int)wY1)*100;
		iStep /= iOscRes;
		wX2 = (int)wX1 + iStep;
	}
	
	while (!bDone)
	{
		byResult = fpWriteOsc(wX2);			// Set X2
		if (byResult != LVLS_NO_ERROR)
			goto return_LinTuneDac;
		byResult = mCNTPULSE_FUNC(dwMeasureTimeUs, &wY2);	// Get Y2
		if (byResult != LVLS_NO_ERROR)
			goto return_LinTuneDac;
		
		// Check if we have correct frequency
		if ( (iCompRes = IsFrequencyOk(wY2, u24ExpFreq, dwMeasureTimeUs)) == 0 )
		{
			bDone = TRUE;		// frequency within margin
		}
		else
		{
			// Interpolate the 2 points for the OscStep
			// Draw a line from (X1,Y1) to (X2,Y2) to calculate the osc step to the u24ExpFreq
			//					       x2 - x1
			//			step = (y - y1)---------
			//					       y2 - y1
			{
				int iDeltaY;
				int sA,sB;

				iDeltaY = (int)wY2-(int)wY1;
				if (iDeltaY != 0 )
				{
					sA = (int)u24ExpFreq - (int)wY1;
					sB = sA * ((int)wX2 - (int)wX1);
					sB += (iDeltaY/2);
					iStep = sB/iDeltaY;
				}
				else
				{
					iStep = 0;
				}
			}

			// Calculate new X2
			wOscValue = wX1 + iStep; 

			// Move X2 to X1 when the X2 is nearer the expected frequency
			if ( (LvAbs((int)wY2-(int)u24ExpFreq)) < (LvAbs((int)wY1-(int)u24ExpFreq)) )
			{
				wX1 = wX2;
				wY1 = wY2;
			}
			
			// Calculate the new X2
			wX2 = wOscValue;

			// If istep is 0, no more interpolating possible 
			if (iStep == 0)
			{
				byResult = LVLS_TUNE_OSC_ERR;
				break; 
			}
		}
	}

return_LinTuneDac:
	if (!bDone)
	{
		if (iCompRes <0)
			return(LVLS_TUNEOSC_LOWFREQ_ERR);
		else if (iCompRes > 0)
			return(LVLS_TUNEOSC_HIGHFREQ_ERR);
	}
	return(byResult);
} // End LinTuneDac 


_reentrant int LvAbs(int i)
{
	if ( i>0)
		return(i);
	else 
		return(-i);
} // End LvAbs

_reentrant U24 CalculateCoeff(U24 wFreq) 
{ 
	// Coefficient calculation: Coeff = Constance/f^2 
	// The contance is fixed at (2^32-1) * 2^15 (for range 65 MHz - 130MHz)
	// The calculation is 
	//		  2^32-1
	//		---------- * 2^15
	//		f in 1 kHz
	//		------------------
	//			f in 1 kHz

#define _C1			((DWORD)0xFFFFFFFF)		// 2^32-1
#define _C2_SHIFT	15
#define _C2			(1<<_C2_SHIFT)			// 2^15
	DWORD dwTmp;

	// Can not calculate when freq is 0
	if (wFreq == 0)
		return(0);

	dwTmp = _C1/wFreq;
	//dwTmp *= _C2;
	dwTmp <<= _C2_SHIFT;

	return((U24)(dwTmp/wFreq));
//	return( ((_C1/wFreq) * _C2)/wFreq );
} // End CalculateCoeff

_reentrant U8 InitTuningRfCapOsc(void)
{
	U24 u24Temp;

	// Select correct oscillator output and enable measuring mode
	SetChipOutput(CO_RF_OSC);

	// Determine the min/max RF frequency (hardware RF-limit)
#ifdef USE_HWRF_LIMIT
	// Set CAP/OSC at minimal value and measure the the minimal hardware RF
	GetRfAtCapOsc(QSS_MIN_RFCAP, QSS_MIN_RFOSC, &g_wHwRfLow);

	// Set CAP/OSC at maximal value and measure the maximal hardware RF
	GetRfAtCapOsc(QSS_MAX_RFCAP, QSS_MAX_RFOSC, &g_wHwRfHigh);

	// Init QSSF-data
	// Determine the 2 points for CAP-low line and the OSC-low resolution
	GetRfAtCapOsc(QSS_MAX_RFCAP, QSS_MIN_RFOSC, &u24Temp);
	g_uQssCoefL = CalculateCoeff(g_wHwRfLow);	// Point1_0: Cap=min, Osc=min
	g_uQssCoefH = CalculateCoeff(u24Temp);		// Point2_0: Cap=max, Osc=min


#else // USE_HWRF_LIMIT

	// Set CAP/OSC at minimal CAP value and determine point1_0
	GetRfAtCapOsc(QSS_MIN_RFCAP, QSS_MIN_RFOSC, &u24Temp);
	g_uQssCoefL = CalculateCoeff(u24Temp);						// Point1_0: Cap=min, Osc=min


	// Set CAP/OSC at max. CAP value and determine point2_0
	GetRfAtCapOsc(QSS_MAX_RFCAP, QSS_MIN_RFOSC, &u24Temp);
	g_uQssCoefH = CalculateCoeff(u24Temp);						// Point2_0: Cap=max, Osc=min

#endif //USE_HWRF_LIMIT

	// Return to idle state
	SetChipOutput(CO_NONE);

#ifdef USE_HWRF_LIMIT
	// Error check
	if ( g_wHwRfHigh <= g_wHwRfLow)
		return(LVLS_INV_RFLIMIT_ERR);
#endif //USE_HWRF_LIMIT
	return(LVLS_NO_ERROR);
} // End InitTuningRfCapOsc


_reentrant U8 GetRfAtCapOsc(U16 u16CapValue, U16 u16OscValue, PU24 pu24Freq)

{
	WriteRfCap(u16CapValue);
	WriteRfOsc(u16OscValue);
	return(mCNTPULSE_FUNC(LV_MSR_TIME_32ms, pu24Freq)); // Measure the RF
} // End GetRfAtCapOsc

#ifdef USE_TMR3
_reentrant U8 ScanSetFreqTmr(U24 u24DisplayFreq, U24 u24TimeUs)
#endif //USE_TMR3
#ifdef FM_TUNER_USE_PWM0
_reentrant U8 ScanSetFreqPwm(U24 u24DisplayFreq)
#endif //FM_TUNER_USE_PWM0
#if CD_24MHz   // not used
_reentrant U8 ScanSetFreqPwm(U24 u24DisplayFreq)
#endif //CD_24MHz
{
	U24   u24TuneFreq;
	U24   wCoeff;
	int   i, iOscRes;
	U16   wCapValue, wOscValue;
	BOOL  bValidCap;

	DWORD c1, c2;
	U8    byResult;

	// Convert display frequency to RF-frequency
	u24TuneFreq = DisplayFreqToRf(u24DisplayFreq);

	// Try to use the current CAP again
	bValidCap = FALSE;
	wCapValue = g_byCurCap;
	if (g_SsfCache.wCap == wCapValue)
	{
		// Re-use this CAP if the wanted frequency is within the frequency range of this CAP
		if ( (u24TuneFreq > g_SsfCache.u24Freq1) && (u24TuneFreq < g_SsfCache.u24Freq2) )
			bValidCap = TRUE;
	}

	// Calculate new CAP value if we don't have a valid CAP yet
	if (!bValidCap)
	{
		// calculate Coeff of desired frequency
		wCoeff = CalculateCoeff(u24TuneFreq);

		// Derive CAP from QSS coefficient
		i = (int)g_uQssCoefL - (int)g_uQssCoefH;	// i is delta of the 2 QssCoefs
		if (i == 0)
			return(LVLS_CAPOSC_CALC_ERR);

		// Interpolate the CAP value using the 2 coefficients determined by InitTuningRfCapOsc
		wCapValue = ( (U16)(((((int)wCoeff-(int)g_uQssCoefL)*((int)QSS_MIN_RFCAP-(int)QSS_MAX_RFCAP) + (i/2))/i) + (int)QSS_MIN_RFCAP) );
	}

	// Verify and adjust the CAP (if necessary) by calculating the desired OSC-value
	for (i=4; i!=0; i--)
	{
		// Determine f1, f2 (the frequency limit (in kHz) of current CAP)
		if (g_SsfCache.wCap != wCapValue)
        {
			// Dirty cache. Validate the cache with measured data
			//GetRfAtCapOsc(wCapValue, QSS_MIN_RFOSC, &g_SsfCache.u24Freq1);
			WriteRfCap(wCapValue);
			WriteRfOsc(QSS_MIN_RFOSC);
			mCNTPULSE_FUNC(u24TimeUs, &g_SsfCache.u24Freq1); // Measure the RF
			if (u24TuneFreq >= g_SsfCache.u24Freq1)
			{
				//GetRfAtCapOsc(wCapValue, QSS_MAX_RFOSC, &g_SsfCache.u24Freq2);
				WriteRfOsc(QSS_MAX_RFOSC);
				mCNTPULSE_FUNC(u24TimeUs, &g_SsfCache.u24Freq2); // Measure the RF


				g_SsfCache.wCap = wCapValue;
			}
		}
		
		// Verify the CAP/OSC data
		bValidCap = TRUE;			// Assume we have correct CAP-value
		if (u24TuneFreq < g_SsfCache.u24Freq1)
		{
			// Wanted frequency is too low, decrease the CAP-value and recalculate
			wCapValue--;
			bValidCap = FALSE;
		}
		else if (u24TuneFreq > g_SsfCache.u24Freq2)
		{
			// Wanted frequency is too high, increase the CAP-value and recalculate
			wCapValue++;
			bValidCap = FALSE;
		}
		
		// Calculate the OSC-value if valid CAP
		if (bValidCap)
		{
			// Calculate OSC value
			//f1: RF at osc1, f2: RF at osc2, D_osc=osc2-osc1
			//f: desired RF
			//a1=f1+f	
			//a2=f-f1	
			//b1=f1+f2	
			//b2=f2-f1
		
			//c1=a1/b2	-> ie (f1+f)/(f2-f1)
			//c2=b1/a2	-> ie (f1+f2)/(f-f1)
			//d1=c1*f2
			//d2=d1/c2
			//d3=d2*1000
			//d4=d3/f
			//d5=d4^2
			//d6=d5/10
		
			//e1=d6*D_osc
			//e2=e1/10^5
			//if (d4<0) e2=-e2
			//Osc = Osc1+e2

			// f1, f2 are in kHz
			c1 = (g_SsfCache.u24Freq1+u24TuneFreq)/(g_SsfCache.u24Freq2-g_SsfCache.u24Freq1);
			c2 = (g_SsfCache.u24Freq1+g_SsfCache.u24Freq2)/(u24TuneFreq-g_SsfCache.u24Freq1);
			c1 *= g_SsfCache.u24Freq2;
			c1 /= c2;
			c1 *= 1000;
			c1 /= u24TuneFreq;
			c1 *= c1;
			c1 /= 10;
			c1 *= (QSS_MAX_RFOSC-QSS_MIN_RFOSC);
			c1 /= 100000;
			wOscValue = QSS_MIN_RFOSC + (WORD)c1; 
			
			// Calculate the RF-OSC resolution and pass it to the LinTuneDac to reduce the measuring loop
			iOscRes = (g_SsfCache.u24Freq2-g_SsfCache.u24Freq1)*100;	// iOscRes is in 10Hz-unit
			iOscRes /= (QSS_MAX_RFOSC-QSS_MIN_RFOSC);

			// Validate the CAP/OSC value with interpolating through measurements. The interpolating should take only 1 to 3
			// measurements to finish the job (The calculated values have RF deviation <40kHz)
#ifdef USE_TMR3
			byResult = LinTuneDac(u24TuneFreq, u24TimeUs, WriteRfOsc, wOscValue, QSS_MAX_RFOSC,iOscRes);
#endif //USE_TMR3
#ifdef FM_TUNER_USE_PWM0
			byResult = LinTuneDac(u24TuneFreq, LV_MSR_TIME_32ms, WriteRfOsc, wOscValue, QSS_MAX_RFOSC,iOscRes);
#endif //FM_TUNER_USE_PWM0
#if CD_24MHz   // not used
			byResult = LinTuneDac(u24TuneFreq, LV_MSR_TIME_16ms, WriteRfOsc, wOscValue, QSS_MAX_RFOSC, iOscRes);
#endif //CD_PWM0

			// Code below is just for exception
			if (byResult==LVLS_NO_ERROR)
				return(LVLS_NO_ERROR);
			if (byResult == LVLS_TUNEOSC_LOWFREQ_ERR)
				wCapValue++;
			else if (byResult == LVLS_TUNEOSC_HIGHFREQ_ERR)
				wCapValue--;
		} //EndIf bValidCap...
	} // EndFor i...

	//if (i==0)
		return(LVLS_CAPOSC_CALC_ERR);

	//return(LVLS_NO_ERROR);
} // End ScanSetFreqTmr/ScanSetFreqPwm

/**

*** Copyright (c) SigmaTel, Inc. Unpublished

***

*** SigmaTel, Inc.

*** Proprietary & Confidential

***

*** This source code and the algorithms implemented therein constitute

*** confidential information and may comprise trade secrets of SigmaTel, Inc.

*** or its associates, and any use thereof is subject to the terms and

*** conditions of the Confidential Disclosure Agreement pursuant to which this

*** source code was originally received.

**/
/* ************************************************************************************************
 *
 *  Function:   CheckFreqLimit
 *
 *  Authors:    Hung van Le
 *  Purpose:    Check input frequency against the FM_BAND_LIMIT_LOW_FREQ, FM_BAND_LIMIT_HIGH_FREQ limit
 *				Wrap the input frequency if it exceeds limit
 *				The frequency will be checked against the hardware limit (stored in m_dwHwRfLow/m_dwHwRfHigh)
 *  Input:      
 *			pu24CurFreq: pointer to frequency to be checked (in 1 kHz unit)
 *			wBandLow, wBandHigh: the band limit in 1 kHz
 *			bWrapFreq: wrap frequency (high to low, low to high) if TRUE
 *  Output:     Status as defined in LvErr.h
 *  Global:		g_wHwRfLow: the lower limit of hardware RF frequency
 *				g_wHwRfHigh: the upper limit of hardware RF frequency
 *  Comments:	Scheme
 *			*pu24CurFreq	bWrapFreq	New freq		Status
 *		a)	<wBandLow		true		wBandHigh		No error
 *		c)	>wBandHigh		true		wBandLow		No error
 *		e)	Other cases		X 			No change		No error
 *		Extra check:
 *			*pu24CurFreq	New freq   	Status
 *			<g_wHwRfLow		g_wHwRfLow	Error: Unreachable frequency
 *			>g_wHwRfHigh	g_wHwRfHigh	Error: Unreachable frequency
 *			Other cases		No change	No error (within hardware limit)
 * ************************************************************************************************
 * Copyright (c) 2002-2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
_reentrant U8 CheckFreqLimit(PU24 pu24CurFreq)
{
	U8 byResult;

	byResult = LVLS_NO_ERROR;
	if (*pu24CurFreq < FM_BAND_LIMIT_LOW_FREQ) // exceed lower limit
	{
		// Wrap low to high - no error
		*pu24CurFreq = FM_BAND_LIMIT_HIGH_FREQ;
	}
	else if (*pu24CurFreq > FM_BAND_LIMIT_HIGH_FREQ) // exceed upper limit
	{
		// Wrap high to low - no error
		*pu24CurFreq = FM_BAND_LIMIT_LOW_FREQ;
	}

#ifdef USE_HWRF_LIMIT
	// Check against hardware limit
	if ( (*pu24CurFreq<g_wHwRfLow) || (*pu24CurFreq>g_wHwRfHigh) )
	{
		(*pu24CurFreq<g_wHwRfLow)?(*pu24CurFreq=g_wHwRfLow):(*pu24CurFreq=g_wHwRfHigh);
		byResult = (LVLS_UNREACHABLE_FREQ_ERR);
	}
#endif //USE_HWRF_LIMIT
	return(byResult);
} // End CheckFreqLimit

/**

*** Copyright (c) SigmaTel, Inc. Unpublished

***

*** SigmaTel, Inc.

*** Proprietary & Confidential

***

*** This source code and the algorithms implemented therein constitute

*** confidential information and may comprise trade secrets of SigmaTel, Inc.

*** or its associates, and any use thereof is subject to the terms and

*** conditions of the Confidential Disclosure Agreement pursuant to which this

*** source code was originally received.

**/
/* ************************************************************************************************
 *
 *  Function:   ScanStation
 *
 *  Authors:    Hung van Le
 *  Purpose:    Search for FM station
 *  Input:      byFlag: flag to control the scan:
 *			SCANSTN_NEXT: scan direction
 *				      1 => scan up (increase frequency).
 *				      0 => scan down (decrease frequency)
 *			SCANSTN_AUTO: 1=> Auto scan: scan whole band in specified direction with fieldstrength in g_byScanLevel
 *						     Return found station via StoreFmStation
 *			              0=> Search for next/previous station with fieldstrength in g_byScanLevel
 *  Output:     Status as defined in LvErr.h
 *  Global:	g_byStnFlag:
 *			STN_VALID (W): clear when the scan starts, after the scan it reflects the valid station
 *		g_wDisplayBandLow (RO): start frequency for the scan when wBeginFreq is 0
 *		g_wDisplayBandHigh (RO): end frequency for the scan when wEndFreq is 0
 *		g_byScanLevel: field strength level to accept/reject a station (must be set before enter this function)
 *			
 *  Comments:   The scan can be aborted when the StoreFmStation function reports any error (no supported now)
 *
 * ************************************************************************************************
 * Copyright (c) 2004. Semiconductor Ideas to the Market (ItoM) B.V. All rights reserved.
 * ************************************************************************************************ */
#ifdef USE_SCANNING_DISPLAY
_reentrant U8 ScanStation(U8 byFlag)
{
#define SCFM_FROMSTATION_STEP	((U24)200)	// 200 kHz - Step size to step away from a station
	U8   byResult;
	BOOL bDone;
	int  iDir, iStepFreq;
	int  iDisplayCnt;

	// Determine scan direction
	if (byFlag & SCANSTN_NEXT)
		iDir = 1;		// Scan up
	else
		iDir = -1;		// Scan down
	
	// Initialize the scan limits according to scan mode
	iStepFreq = 0;

	if ( (g_byStnFlag & STN_SCAN_IN_PROGRESS) == 0 )	// Scan is start for the 1st time
	{
		if (byFlag & SCANSTN_AUTO)	// Auto search 
		{
			g_wCurrentFrequency = FM_BAND_LIMIT_LOW_FREQ; 	// Always scan up by auto scan
			//g_byStnFlag &= (~STN_VALID);			// No valid station
			g_byStnFlag = (STN_ASCAN|STN_USCAN);	// Clear valid station flag, mark auto scan and its direction
		}
		else	// Search
		{
			CheckFreqLimit(&g_wCurrentFrequency);	// Make sure we have a valid current frequency
			g_byStnFlag |= byFlag;			// Save scan direction
		}
		g_wFMTunerError.m_bTUNER_BUSY = TRUE;          
		g_wFMTunerError.m_bSCANNING_STATION = TRUE;
		g_wFMTunerError.m_bSEARCHING_UP	 = TRUE;
		g_bTunedStereoStation = FALSE;			// Just say mono by scanning
		g_wOrgFreq = g_wCurrentFrequency;		// This is our start frequency
		//@@@ WriteReg(IR01_RADIO_CTRL3_REG, IR01_RADIOCTRL3_NOAUDIO); // Mute audio

		SetChipOutput(CO_RF_OSC);  
	}
	else // Scanning is already started
	{
		if (byFlag & SCANSTN_AUTO)	// Auto search 
			iStepFreq = FM_TUNER_GRID;
	}
		
	// Step away from current station
	if (g_byStnFlag & STN_VALID)			// Valid station
	{
		g_byStnFlag &= (~STN_VALID);
	}
   	iStepFreq = SCFM_FROMSTATION_STEP;	

	// Perform scan
	bDone = FALSE;
	iDisplayCnt = 0;
	while (!bDone)
	{
		// Calculate the new frequency
		g_wCurrentFrequency += (iStepFreq*iDir);

		// Check frequency limit
		byResult = CheckFreqLimit(&g_wCurrentFrequency);

		// Check end condition (when we reach the start frequency again)
		if (byResult == LVLS_NO_ERROR)
		{
			if ( (iStepFreq != 0) && (g_wCurrentFrequency == g_wOrgFreq) )
			{
				byResult = LVLS_NO_STATION_FOUND_ERR;
				bDone = TRUE;
			}
			else
			{
				// Check if we have a station on current frequency
				byResult = IsFmStation(g_wCurrentFrequency, iDir);
			}
		}

		// Check for valid station
		if (byResult == LVLS_NO_ERROR)
		{
			// Set the found station
			mSCANSETFREQ_FUNC(g_wCurrentFrequency, LV_MSR_TIME_32ms);	

			// Save the found station if auto scan mode
			if (byFlag & SCANSTN_AUTO)				// Auto scan mode
			{
				StoreFmStation();					// Save station
				byResult = LVLS_SCAN_UPDATE_ERR;	// Pause scan loop with frequency update error
			}
			bDone = TRUE;	// Exit the scan loop
		}
		else if (byResult == LVLS_NOT_STATION_ERR)
		{
			// Station not found - try next grid
			iStepFreq = FM_TUNER_GRID;

			// Update display if necessary
			iDisplayCnt++;
			if (iDisplayCnt > LV_SCAN_DISPLAY_DISTANCE)
			{
				byResult = LVLS_SCAN_UPDATE_ERR;
				bDone = TRUE;
			}
		}
		else 
		{
			// Other errors, terminate
			bDone = TRUE;		// Done when end of band reached
		}
	} // EndWhile
		
	// Determine state for next time	
	if (byResult == LVLS_SCAN_UPDATE_ERR)
	{
		// Update frequency - no completion
		SysPostMessage(2, MENU_TUNER_TUNED);	
	}
	else // Scanning completed
	{
		if (byFlag & SCANSTN_AUTO)				// Auto scan mode
			SortFmStations();
		
        SetChipOutput(CO_NONE);					// Normal chip mode
		//@@@ WriteReg(IR01_RADIO_CTRL3_REG, IR01_RADIOCTRL3_AUDIO);	// Unmute audio

		// Finishing scanning to system module         
		g_wFMTunerError.m_bSCANNING_STATION = FALSE;
		g_wFMTunerError.m_bSEARCHING_UP	 = FALSE;
		g_byStnFlag &= ~(STN_SCAN_IN_PROGRESS);					// Clear scanning in progress mark
        //g_byStnFlag |= STN_VALID;
		//g_wFMTunerError.m_bTUNER_BUSY = FALSE;				// Done in CompletionProc
		CompletionProc();	// V1.4A
	}

    return(byResult);
} // End ScanStation
#endif //USE_SCANNING_DISPLAY

#ifdef NO_SCANNING_DISPLAY
_reentrant U8 ScanStation(U8 byFlag)
{
#define SCFM_FROMSTATION_STEP	((U24)200)	// 200 kHz - Step size to step away from a station
	U8   byResult;
	BOOL bDone;
	int  iDir, iStepFreq;
	U24  wOrgFreq;

	// SigmaTel Specific handlings
	if (g_wFMTunerError.m_bTUNER_BUSY)
		return LVLS_CHIP_BUSY_ERR;
	g_wFMTunerError.m_bTUNER_BUSY = TRUE;	// Claim the device

	// Determine scan direction
	if (byFlag & SCANSTN_NEXT)
		iDir = 1;		// Scan up
	else
		iDir = -1;		// Scan down

	// Initialize the scan limits according to scan mode
	iStepFreq = 0;
	//wOrgFreq = 0;	// To avoid compiler warning (uninitialized var.)
	if (byFlag & SCANSTN_AUTO)	// Auto search 
	{
		g_wCurrentFrequency = FM_BAND_LIMIT_LOW_FREQ; 	// Always scan up by auto scan
		g_byStnFlag &= (~STN_VALID);			// No valid station
	}
	else // Search next mode
	{
		// Make sure we have a valid current frequency
		CheckFreqLimit(&g_wCurrentFrequency);

		// Step away from current station
		if (g_byStnFlag & STN_VALID)			// Valid station
		{
			g_byStnFlag &= (~STN_VALID);
		}
		iStepFreq = SCFM_FROMSTATION_STEP;	
        
	}
	wOrgFreq = g_wCurrentFrequency; // This is our start frequency
		
#ifdef USE_SCAN_IF
	// Set narrow band to reduce field strength hits
	//WriteReg(IR01_IF_BW_REG, ((g_byIfBW/10)*4));	// 40%
#endif //USE_SCAN_IF

	// Select correct oscillator output and enable measuring mode
	SetChipOutput(CO_RF_OSC);

	// Perform scan
	bDone = FALSE;
	while (!bDone)
	{
		// Calculate the new frequency
		g_wCurrentFrequency += (iStepFreq*iDir);

		// Check frequency limit
		byResult = CheckFreqLimit(&g_wCurrentFrequency);

		// Check end condition (when we reach the start frequency again)
		if (byResult == LVLS_NO_ERROR)
		{
			if ( (iStepFreq != 0) && (g_wCurrentFrequency == wOrgFreq) )
			{
				byResult = LVLS_NO_STATION_FOUND_ERR;
				bDone = TRUE;
			}
			else
			{
				// Check if we have a station on current frequency
				byResult = IsFmStation(g_wCurrentFrequency, iDir);
			}
		}

		// Check for valid station
		if (byResult == LVLS_NO_ERROR)
		{
			// Set the found station
			mSCANSETFREQ_FUNC(g_wCurrentFrequency, LV_MSR_TIME_32ms);

			// Save the found station if auto scan mode
			if (byFlag & SCANSTN_AUTO)	// Auto search 
			{
				iStepFreq = SCFM_FROMSTATION_STEP;
				StoreFmStation();
			}
			else	// Normal scan mode
			{
				// Just scan once
				bDone = TRUE;	
			}
		}
		else if (byResult == LVLS_NOT_STATION_ERR)
		{
			// Station not found - try next grid
			iStepFreq = FM_TUNER_GRID;
		}
		else 
		{
			// Other errors, terminate
			bDone = TRUE;		// Done when end of band reached
		}
	} // EndWhile
		
	if (byFlag & SCANSTN_AUTO)			// Auto scan mode
		SortFmStations();
	// Restore chip config
#ifdef USE_SCAN_IF
	//WriteReg(IR01_IF_BW_REG, g_byIfBW);		// Restore bandwidth
#endif //USE_SCAN_IF
	SetChipOutput(CO_NONE);					// Normal chip mode

	// SigmaTel specific handling
	CompletionProc();	// V1.4A
	return(byResult);
} // End ScanStation
#endif //NO_SCANNING_DISPLAY

#ifdef USE_SCAN_IF
_reentrant U8 IsFmStation(U24 u24CurFreq, int iDir)
{
#define SCFM_IF_DISTANCE	(FM_TUNER_GRID/2)	// 50 kHz IF distance (1 kHz-unit)
#define SCFM_IF_SWING		((int)60000)		// 60 kHz IF swing (1 Hz-unit)

	int  i, aiIfFreq[2];
	U8   byMsrFs;
	U24  au24Freq[2];
	
	// Init var.
	//g_byStnFlag &= (~STN_VALID);			// No valid station now

	// Quick check for correct level
	//if (g_byScanLevel > 0)	//V1.4D removed: scan level is always >0
	{
		mSCANSETFREQ_FUNC(u24CurFreq, LV_MSR_TIME_6ms);
		byMsrFs=GetFmFieldStrength();
		if (byMsrFs == (g_byScanLevel-1))
		{
#ifdef LOW_SPEED_AGC
			LvDelay(WAIT_AGC_TIME);
#endif //LOW_SPEED_AGC
			byMsrFs=GetFmFieldStrength();
		}
		if (byMsrFs < (g_byScanLevel-0))
			return(LVLS_NOT_STATION_ERR);
	}
	
	// Calculate the 2 FM frequency points for station check
	au24Freq[0] = (u24CurFreq - SCFM_IF_DISTANCE);
	au24Freq[1]= (u24CurFreq + SCFM_IF_DISTANCE);

	// See if we can get the IF from the cache
	for (i=0; i<2; i++)
	{
		if (au24Freq[i] == g_FreqIfCache.u24Freq)
			aiIfFreq[i] = g_FreqIfCache.iIfFreq;
		else
			aiIfFreq[i] = 0;
	}

	// Perform measuring if IF is unknown yet
	for (i=0; i<2; i++)
	{
		if (aiIfFreq[i] == 0)
		{
			mSCANSETFREQ_FUNC(au24Freq[i], LV_MSR_TIME_16ms);
			
			// Measure IF frequency
			byMsrFs = SetChipOutput(CO_IF_OSC);
			mCNTPULSE_FUNC(LV_MSR_TIME_16ms, (PU24)&aiIfFreq[i]);
			SetChipOutput(byMsrFs);	// restore org. chip output
		}
	}

	// Caching frequency (scan direction dependant)
	if (iDir > 0)	// Scan up
		i = 1;
	else
		i = 0;
	g_FreqIfCache.u24Freq = au24Freq[i];
	g_FreqIfCache.iIfFreq = aiIfFreq[i];
	
	// Check IF swing
	if ( (aiIfFreq[1] - aiIfFreq[0]) >= SCFM_IF_SWING)
	{
		// IF swing good - valid station
		g_byStnFlag |= (STN_VALID);	// Valid station
		return(LVLS_NO_ERROR);
	}
		
	return(LVLS_NOT_STATION_ERR);
} // End IsFmStation
#endif //USE_SCAN_IF

#ifdef USE_SCAN_FS
_reentrant U8 IsFmStation(U24 u24CurFreq, int iDir)
{
#define SCFM_FS_CHK_DISTANCE		(100 * KHz)		// FS check distance
	U8  byFs0, byFs1;
	U8  byResult;
	U24 u24Freq1;

	// Quick check for correct level

	//if (g_byScanLevel > 0)	//V1.4D removed: scan level is always >0
	{

		mSCANSETFREQ_FUNC(u24CurFreq, LV_MSR_TIME_4ms);
#ifdef LOW_SPEED_AGC
		LvDelay(WAIT_AGC_TIME);
#endif //LOW_SPEED_AGC
		if (GetFmFieldStrength() < (g_byScanLevel-0))
			return(LVLS_NOT_STATION_ERR);
	}

	// Assume not a valid station
	byResult = LVLS_NOT_STATION_ERR;
	u24Freq1 = u24CurFreq - SCFM_FS_CHK_DISTANCE; 

	// Set IF bandwidth to measure the FS-curve
	//WriteReg(IR01_IF_BW_REG, (g_byIfBW/6));	// 16%
	//WriteReg(IR01_IF_BW_REG, ((g_byIfBW/10)*3));	// 30%
	WriteReg(IR01_IF_BW_REG, ((g_byIfBW/10)*4));	// 40%

	// Fetch FS1 by upscan
	byFs0 = 0xFF;
	byFs1 = 0xFF;
	if (iDir>0)	// Upscan: fetch FS1 from cache
	{
		if ( u24Freq1 == g_FreqFsCache.u24Freq )
			byFs1 = g_FreqFsCache.byFs;
	}
	else	// Down scan: fetch FS0 from cache
	{
		if ( u24CurFreq == g_FreqFsCache.u24Freq )
			byFs0 = g_FreqFsCache.byFs;
	}

	// Check f+0 for both scan up/scan down.
	if (byFs0 == (U8)0xFF)
		byFs0 = ScanGetFs(u24CurFreq);

	// Check for sufficient FS
	if (byFs0 >= g_byScanLevel)
	{
		// Field strength OK - check f-1(grid)  

		if (byFs1 == (U8)0xFF) 
			byFs1 = ScanGetFs(u24Freq1);


		if (byFs0 > (byFs1 + 1) )
        {
			g_byStnFlag |= (STN_VALID);	// Valid station
			byResult = LVLS_NO_ERROR;
		}
	}

	// Save measuring result for next scan step
	// Up scan: save f+0
	// Down scan: save f-1
	if (iDir>0)
	{
		g_FreqFsCache.u24Freq = u24CurFreq;
		g_FreqFsCache.byFs = byFs0;
	}
	else
	{
		g_FreqFsCache.u24Freq = u24Freq1;
		g_FreqFsCache.byFs = byFs1;
	}

	// Restore IF bandwidth
	WriteReg(IR01_IF_BW_REG, g_byIfBW);

	return(byResult);
} // End IsFmStation
#endif //USE_SCAN_FS

#ifdef USE_SCAN_FS
_reentrant U8 ScanGetFs(U24 u24Freq)
{
	// Set RF frequency and measure the FS
	mSCANSETFREQ_FUNC(u24Freq, LV_MSR_TIME_16ms);

	// Measure FS
#ifdef LOW_SPEED_AGC

	LvDelay(WAIT_AGC_TIME);
#endif //LOW_SPEED_AGC

	return GetFmFieldStrength();
} // End ScanGetFs
#endif //USE_SCAN_FS

#ifdef FUNCLET
    #pragma asm
        FStoreFmStation:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_TUNER_STORE_FM_STATION

        org p,"SYSFUNCLET_TUNER_STORE_FM_STATION_P":
        dc      RSRC_FUNCLET_TUNER_STORE_FM_STATION
    #pragma endasm
_reentrant void StoreFmStation_Funclet(void)
#else
_reentrant void StoreFmStation(void)
#endif
{

	// Store current FM frequency in a preset, remove the weakest one if no more storage available
	INT iCurLevel, iLowestPreset;
	INT i, iLowestLevel;
	
	//iCurLevel = ScanGetFs(g_wCurrentFrequency);
	iCurLevel = GetFmFieldStrength();
	iLowestPreset = NUMBER_OF_PRESETS;
	if (g_wCurrentPreset < NUMBER_OF_PRESETS)
	{
		// Save the station
		iLowestPreset = g_wCurrentPreset;
		g_wCurrentPreset++;	// Storage for next time
	}
	else
	{
		// No more storage place - replace the weaker stations with a stronger one
		iLowestLevel = 7;
		
		// Search for the one with the weakst level
		for (i= 0; i < NUMBER_OF_PRESETS; i++)
		{
			if (g_astPresetStations[i].Level< iLowestLevel)
			{
				iLowestLevel = g_astPresetStations[i].Level;
				iLowestPreset = i;
			}
		}
		if (iLowestLevel >= iCurLevel)
			iLowestPreset = NUMBER_OF_PRESETS; // No save due to weak level of the new station
	}
	
	// Save the new station if OK
	if (iLowestPreset < NUMBER_OF_PRESETS)
	{
		StoreFmPreset(iLowestPreset, g_wCurrentFrequency, iCurLevel, g_wCurrentPreset);
		//g_astPresetStations[iLowestPreset].Frequency = g_wCurrentFrequency;
		//g_astPresetStations[iLowestPreset].Level = iCurLevel;
	}

} // End StoreFmStation
#ifdef FUNCLET
    #pragma asm
        org p,".ptextsanyo24000":
    #pragma endasm
#endif

_reentrant void StoreFmPreset(INT iLocation, WORD wFrequency, INT iLevel, WORD wNewCurPreset)
{
	g_astPresetStations[iLocation].Frequency = wFrequency;
	g_astPresetStations[iLocation].Level = iLevel;
	g_wCurrentPreset = wNewCurPreset;
} // End StoreFmPreset

_reentrant void SortFmStations()
{
	// Sort the found stations from strongest to weakest
	BOOL bSwap;
	INT  iLevel;
	WORD wFrequency;
	INT  i;
	
	do
	{
		bSwap=FALSE;
		for (i=0; i < (NUMBER_OF_PRESETS-1); i++)
		{
			if (g_astPresetStations[i].Level < g_astPresetStations[i+1].Level)
			{
				iLevel = g_astPresetStations[i].Level;
				wFrequency = g_astPresetStations[i].Frequency;
				StoreFmPreset(i, g_astPresetStations[i+1].Frequency, g_astPresetStations[i+1].Level, 1);
				StoreFmPreset(i+1, wFrequency, iLevel, 1);
				bSwap = TRUE;
			}
		}
	} while (bSwap);

	if (g_astPresetStations[0].Frequency != 0)
	{
		g_wCurrentPreset = 1;
		g_wCurrentFrequency = g_astPresetStations[0].Frequency;
		mSCANSETFREQ_FUNC(g_wCurrentFrequency, LV_MSR_TIME_32ms);
	}
} // End SortFmStations

_reentrant void LvDelay(WORD wDelayMs)
{
	DWORD dwTimeout;

	dwTimeout = SysGetCurrentTime() + wDelayMs;
	while (SysGetCurrentTime() < dwTimeout);
} // End LvDelay


// V1.4A
_reentrant void CompletionProc()
{
	// Update stereo state  
	WriteReg(IR01_IRQ_OUT_REG, 0);			// Select interrupt on DATA-line
	SysPostMessage(2, MENU_TUNER_TUNED);	
	g_wFMTunerError.m_bTUNER_BUSY = FALSE;	// The chip is ready when this function is invoked

} // End CompletionProc



