////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2005
//
// Filename: STFM1000.c
// Description:
////////////////////////////////////////////////////////////////////////////////

#include "hwequ.h"
#include "messages.h"
#include "exec.h"
#include "resource.h"
#include "project.h"
#include "gettime.h"
#include "types.h"
#include "..\Algorithms\Decoders\Stfm\src\sw_stereocontrol.h"
#include "..\tunerdriver.h"
#include "stfm1000.h"
#include "hwequ.h"
#include "misc.h"
#include "regsgpio.h"
#include "display.h"

#ifdef PLAYER
#include "sysspeed.h"
#endif

#pragma optimize 1
#define STFM1000_ENABLE_MONITOR_SIGNAL_QUALITY 1
BOOL  g_STFM1000_CustomerForceMono = FALSE; //STMP00013255

#ifdef USE_PLAYLIST2
static DWORD GetDclkCount(void);
#endif

#ifdef USE_PLAYLIST3
DWORD GetDclkCount(void);
#endif


void SetDclkCount(DWORD dwCount);

struct ErrorBits g_wFMTunerError;

#ifdef CLCD_16BIT   
BOOL g_blIsTB2Fm = TRUE;
#endif

/* FM Tuner Global variables */
WORD g_wCurrentFrequency=96700;//90500;
BOOL _X g_bTunedStereoStation;          //
BOOL _X g_bSearchEndAtBandLimit = TRUE; //
WORD  g_wCurrentPreset;               //
volatile WORD g_wRawRSSIData;
extern int _X g_iTunerProcessIntervalMs; //
#ifndef MIN_FILTER
extern _reentrant void sw_stereocontrol_xcall(unsigned short sd_audio_Rssi_u16);
extern _reentrant void sw_stereocontrol_bwsweep(void);
extern sw_stereocontrol_t sw_stereocontrol;
extern int g_sd_audio_pCoefForcedMono_u1;
#else
extern int sd_audio_pCoefForcedMono_u1;
#endif
extern _reentrant INT SysGetSpeed(void);
#ifdef FUNCLET
    #pragma asm
        extern SysCallFunclet
        extern SysCallFunclet2
    #pragma endasm
#endif
FMTunerPreset   g_astPresetStations[NUMBER_OF_PRESETS];
#ifdef SD_USE_50KHZ_TUNING_GRID
FMTunerSearchStatus g_astSearchStations;
#else
FMTunerPreset g_astSearchStations;
#endif

eOptimizeChannelState s_eOptimizeChannelState = OPTIMIZE_CHANNEL_STATE_COMPLETE;
eMonitorSignalQualityState s_eMonitorSignalQualityState =  MONITOR_SIGNAL_QUALITY_STATE_COMPLETE;
/* FM Tuner Static variables */
WORD s_wCurrentFMFrequency;
WORD s_wBeginSearchFMFrequency;
INT  s_iFMTunerSearchStatus = NOT_SEARCHING; // 0 - idle, 1 searching up, -1 searching down
BOOL _X s_bTunerSearchHitBandLimit;
INT  s_iFMPresetCount;
volatile INT  s_iFMSignalStrengthRSSI;
#ifdef SD_USE_50KHZ_TUNING_GRID
volatile INT  s_iFMSignalStrengthMpxDc;
WORD s_wDisplayFMFrequency;
#endif
//INT  s_iFMTunerSensitivity = 20;	/* Tuner sensitivity, i.e.RSSI after decoded, the value is 0-70 */
WORD wCountForTriggerMonitorQuality = 0;   // reset it when find a station or Rssi ten read
BOOL bIsWeakSignal = FALSE;     // Reset to 0 whenever we change to a new station
WORD s_wTrackTableIndex = 0;
BOOL _X bIsFirstTimeEnterFMMode = TRUE;  //
BOOL bIsFirstTimeToNewStation = TRUE;
// // inserted these statics dec 4 2006
WORD wSd_ctl_Confidence_u16 = 0;
DWORD dwBias1Up;
DWORD dwBias1Down;
INT global_filtered_rssi = 0;  // DEBUG!!!
INT count_MonitorQuality = 0;  // DEBUG!!!

// tuning process data structures, including shadow registers
STFM1000Tune_t StfmTuneData;


// Register settings for powerup. Allocated in X to save Y and fit janus player
#ifdef PLAYER
STFM1000Tune_t _X StfmTuneDefault =
#else
STFM1000Tune_t _X StfmTuneDefault =
#endif
{
    // shadow images written to on-chip registers
    // initialized to TA2 values, and re-initialized in STFM1000RegValsReset if necessary
    0x00044F01, // TUNE1_REG
    0x1C5EBCF0, // Shadow reg for SDNOMINAL_REG
    0x000001B6, // Shadow reg for PILOTTRACKING_REG
    0x9fb80008, // Shadow reg for INITIALIZATION1_REG
    0xC506e444, // Shadow reg for INITIALIZATION2_REG  Var
    0x1402190B, // Shadow reg for INITIALIZATION3_REG
    0x525bf052, // Shadow reg for INITIALIZATION4_REG
    0x6800d106, // Shadow reg for INITIALIZATION5_REG
    0x00003ecb, // Shadow reg for INITIALIZATION6_REG
    0x0030222D, // ShadowRef reg for ANALOG1_REG
    0x05080009, // ShadowLna reg for ANALOG2_REG
    0x00007205, // ShadowMixFlt reg for ANALOG3_REG
    0x10002000, // ShadowClk1 reg for ANALOG4_REG
    0x7F000000, // ShadowClk2 reg for ANALOG4_REG
    0x00033282, // ShadowAdc reg for ANALOG5_REG
    0x002c8802, // Shadow reg for AGC_CONTROL1_REG
    0x00140050, // Shadow reg for AGC_CONTROL2_REG
    0x00014210, // Shadow reg for DATAPATH_REG
    // tuning parameters
    TUNE_RSSI_THRESH,
    TUNE_MPX_DC_THRESH,
    ADJACENT_CHAN_THRESH,
    PILOT_EST_THRESH,
    // parameters for monitoring AGC
    0,      // sd_ctl_AgcMonitorDisabled;
    AGC_MSEC_PER_CYCLE,
    0,      //sd_ctl_LnaDriving_u1
    LNA_TURN_OFF_THRESH,
    LNA_TURN_ON_THRESH,
    LNA_TURN_OFF_REG_REF,
    LNA_TURN_ON_REG_REF,
    // tuning data being maintained
    0,0,0,0,
    // data read from on-chip registers
    0,0,
    0   //VERSION_TB2
};



//! Tuning Tables (Precalculated)
// Since tune1 register only use bits[20:0] 21 bits, so we make it as WORD instead of DWORD to save memory
//#define  STFM1000_PLL_TABLE
#ifdef STFM1000_PLL_TABLE
WORD _X sdc_ctl_TableTune1_w[325] =
{
    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,

    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,

    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,

    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0
};  // tuning table for programming TUNE1_REG[23:0]

DWORD _X sdc_ctl_TableSdnom_dw[325] =
{
    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,

    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,

    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,

    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0
};  // tuning table for setting SDNOMINAL_REG[31:0]
#endif
// * Gear Shift Pilot Tracking on chip
// (tspec_v09y.pdf::SD_GearShiftPilotTracking)


#ifdef PLAYER
STFM1000GearShiftPilotTrackingTable_t
#else
_X STFM1000GearShiftPilotTrackingTable_t
#endif
STFM1000TrackTable[] =
#if 0
    {
     {10, 0x82A5},
     { 6, 0x8395},
     { 6, 0x8474},
     { 8, 0x8535},
     {20, 0x8632},
     {50, 0x8810},
     { 0, 0x0000},   // table stops processing at 0,0
     { 0, 0x0000},
     { 0, 0x0000}}; // leave room to tweak in more steps
#else
    // Updated table
    {
     {10, 0x81b6},
     { 6, 0x82A5},
     { 6, 0x8395},
     { 8, 0x8474},
     {20, 0x8535},
     {50, 0x8632},
     { 0, 0x8810},
     { 0, 0x0000},   // table stops processing at 0,0
     { 0, 0x0000},
     { 0, 0x0000}}; // leave room to tweak in more steps
#endif

// Note: Since 32 <= cicosr <= 45 , a lookup table for Factor/cicosr can be used
// This data is used by STFM1000Reset or STFM1000QuickTune() function dependingu upon
// the value of #define STFM1000_PLL_TABLE.
//#define OPTIMIZED_TUNE1_SDNOMINAL_CAL
#ifdef OPTIMIZED_TUNE1_SDNOMINAL_CAL
// If we save this table in X memory, something is messed up
const DWORD _Y FractCicosr[14] =   {  // All values are pre-scaled by 2048 for rounding
                                 (DWORD)100.0e3 * 4096 / 3.0 /32.0 / 228.0e3 * 134217728,
                                 (DWORD)100.0e3 * 4096 / 3.0 /33.0 / 228.0e3 * 134217728,
                                 (DWORD)100.0e3 * 4096 / 3.0 /34.0 / 228.0e3 * 134217728,
                                 (DWORD)100.0e3 * 4096 / 3.0 /35.0 / 228.0e3 * 134217728,
                                 (DWORD)100.0e3 * 4096 / 3.0 /36.0 / 228.0e3 * 134217728,
                                 (DWORD)100.0e3 * 4096 / 3.0 /37.0 / 228.0e3 * 134217728,
                                 (DWORD)100.0e3 * 4096 / 3.0 /38.0 / 228.0e3 * 134217728,
                                 (DWORD)100.0e3 * 4096 / 3.0 /39.0 / 228.0e3 * 134217728,
                                 (DWORD)100.0e3 * 4096 / 3.0 /40.0 / 228.0e3 * 134217728,
                                 (DWORD)100.0e3 * 4096 / 3.0 /41.0 / 228.0e3 * 134217728,
                                 (DWORD)100.0e3 * 4096 / 3.0 /42.0 / 228.0e3 * 134217728,
                                 (DWORD)100.0e3 * 4096 / 3.0 /43.0 / 228.0e3 * 134217728,
                                 (DWORD)100.0e3 * 4096 / 3.0 /44.0 / 228.0e3 * 134217728,
                                 (DWORD)100.0e3 * 4096 / 3.0 /45.0 / 228.0e3 * 134217728
                                };
const _fract OneFifteenth = 0.066666666666666666666667;
const _fract OneTwelfth   = 0.083333333333333333333333;

volatile DWORD FC;
#endif

//#define DEBUG_MONITOR_SIGNAL_QUALITY
#ifdef DEBUG_MONITOR_SIGNAL_QUALITY
#define STFM1000_MONITOR_SIGNAL_LOG_MAX   240
struct stLogWord {
	WORD wMonitorQualityCurrentState    :4;
	WORD wOptimizedChannelCurrentState  :4;
	WORD wIsPolitPreset                 :4;
	WORD wWeakSignal	                :4;
	WORD wFunctionIndex                 :8;
};

#if 0
typedef struct
{
    struct stLogWord stLogFirstWord;
    WORD      wRssiValue;
    WORD      wLnaRms;
    WORD      wLnaTh;
    WORD      wAgcOut;
}stSTFM1000MonitorQuality;
WORD s_wLnaRms = 0;
WORD s_wLnaTh = 0;
WORD s_wAgcOut= 0;
WORD s_wCurrentTime;
extern WORD g_wTunerMessage;

#else
typedef struct
{
    WORD      wRssiValue;
    WORD      wMpxAdc;
    BOOL      bIsStation;
    WORD      wModulo;
    WORD      wFreq;
}stSTFM1000MonitorQuality;
WORD s_wMpxAdc = 0;
WORD s_wIsSatation = 0;
WORD s_wModulo= 0;
WORD s_wCurrentTime;

#endif

#ifdef DEBUG_MONITOR_SIGNAL_QUALITY
WORD wMonitorQualityIndex = 0;
void LogDebugFunction(WORD wFunctionIndex);
#define STFM1000_MONITOR_SIGNAL_LOG_MAX   256
stSTFM1000MonitorQuality _X stSTFM100MonitorQualityLogArrary[STFM1000_MONITOR_SIGNAL_LOG_MAX];
WORD wTimeLogArray[STFM1000_MONITOR_SIGNAL_LOG_MAX];
WORD wLogIndex = 0;
WORD wLogTimeDelta =0;
#endif



#endif // endif DEBUG_MONITOR_SIGNAL_QUALITY

#ifdef FM_USA_REGION        // If in US region
BOOL s_bSearchFromTopBand = FALSE;
#endif
//DWORD swSteelyDan[100];
static void do_nothing(void) {      // This function is here to make the dang FUNCLET, below, work right.
                                    // Without this here, the compiler switches back to ".ptext..." as soon
                                    // as it hits some code.  With this here, the "org p,"SYSFUNCLET_..._P"
                                    // stays in effect.
                                    // The function is declared "static" so that it's invisible outside this file.
  return;
}
////////////////////////////////////////////////////////////////////////////////
//!
//! \brief      TunerInit
//! \fntype     Function
//!
//! Initializes Stfm static data.  This includes initializing the values in two
//! tables that convert a displayed frequency to the contents of the two major
//! tuning control registers TUNE1_REG and SDNOMINAL_REG  (see below).
//! initializes physical interface to tuner (ddi_i2c_Open(),  to verify that
//! i2c hardware is up and running, and that a STFM1000 is located at the
//! expected address.) initializes Audio Processing channel from tuner
//! (ddi_dri_Init()) (registers for alert from tuner; this callback is a
//! parameter in the DRI stream control block) allocates memory for the
//! TunerSettingsTable data structure. initializes tuner itself in power-down
//! mode.
//!
//! \param[in]  pTunerObject     Pointer to Tuner Object.
//!
//! \return     SUCCESS/ERROR.
//!
////////////////////////////////////////////////////////////////////////////////
_inline INT STFM1000TunerInit(void)
{
    DWORD  STFM1000_Version;
    INT RetCode = SUCCESS;

    RetCode = STFM1000ReadSubRegister(  // Get version of STFM1000 part
            CHIPID_REG_ADDR,
            MAJOR_REV_ID_REG_MASK | MINOR_REV_ID_REG_MASK,
            &STFM1000_Version);

    StfmTuneDefault.STFM1000_Version =  STFM1000_Version;   // Test for VERSION_TB2
    
#ifdef CLCD_16BIT   
    if( STFM1000_Version == VERSION_TA2 )
    {
        g_blIsTB2Fm = FALSE;
    }
    else
    {
        g_blIsTB2Fm = TRUE;
    }
#endif
    // Initialize tuning tables (temp, replaced with compile-time in code)
    STFM1000Reset();

    // Initialize default register values (temp, replaced with compile-time
    // in code)
    STFM1000RegValsReset();

    // Set to 100kHz for I2C
    STFM1000I2CInit(0, 0);

    return SUCCESS ;
}
////////////////////////////////////////////////////////////////////////////////
//!
//! \brief      Disables DRI transmission
//! \fntype     Function
//!
//! Function will perform the following steps:
//!     Stop AGC Monitor
//!     Disable 3600 DMA
//!     Disable DRI stream to transmit from chip
//!
//! \param[in]  None
//!
//! \return     SUCCESS/ERROR.
//!
////////////////////////////////////////////////////////////////////////////////
_inline INT STFM1000DriOff(void)
{
    INT RetCode = SUCCESS;

    // * Stops DRI transmission from tuner (tspec_v09u.pdf::DRI_off)
    RetCode = STFM1000WriteRegister(
        DATAPATH_REG_ADDR,
        DRI_EN_REG_MASK,
        &StfmTuneData.sd_ctl_ShadowDataPath_dw,
        0);
    if (SUCCESS != RetCode)
    {
        //SystemHalt();
        return RetCode;
    }

    return RetCode;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          TunerGetTunerDriverTuneToFrequency
//
//   Type:          Function
//
//   Description:   Tune to the frequency iFrequency
//
//   Inputs:        iFrequency
//
//   Outputs:       Error status
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
_reentrant INT TunerDriverTuneToFrequency(INT iFrequency)
{
    INT iTuningStatus=TUNER_SUCCESS;
    // clear search flag
	s_iFMTunerSearchStatus = 0;
	TunerTuningInit();
    //STFM1000DriOff();
	s_wCurrentFMFrequency = iFrequency;	// set the current frequency
#ifdef SD_USE_50KHZ_TUNING_GRID
    s_wDisplayFMFrequency = s_wCurrentFMFrequency;
#endif

    // DRI off
    STFM1000DriOff();
    // Tune to the frequency
    STFM1000TunerQuicksetStation(iFrequency);

    //Force tuner busy
    return E_TUNER_BUSY;
}
// OptmizeChannel() calls the following 3 procedures:
// * Set Channel Filter on chip  (tspec_v09x.pdf::SD_SetChannelFilter)
_inline INT STFM1000SetNearChannelOffset(WORD wOffsetCode)
{
    STFM1000Tune_t * pDrst = &StfmTuneData;

    // set "near channel" offset
    return STFM1000WriteRegister(
        INITIALIZATION3_REG_ADDR,
        B2_NEAR_CHAN_MIX_REG_MASK,
        &pDrst->sd_ctl_ShadowInitialization3_dw,
        wOffsetCode);
}

_inline INT STFM1000GetNearChannelOffset(void)
{
    STFM1000Tune_t * pDrst = &StfmTuneData;
    DWORD dwTemp;
    INT RetCode;
    //INT RetCode = SUCCESS;

    // * gets near channel amplitude
    RetCode = STFM1000ReadSubRegister(
        SIGNALQUALITY_REG_ADDR,
        NEAR_CHAN_AMPLITUDE_REG_MASK,
        &dwTemp);

    pDrst->sd_ctl_ShadowSignalQuality_w = (WORD)dwTemp ;

    return RetCode;

}
_inline INT STFM1000SetNearChannelFilter(WORD wBypassSetting)
{
    STFM1000Tune_t * pDrst = &StfmTuneData;

    // set filter settings
    return STFM1000WriteRegister(
        INITIALIZATION1_REG_ADDR,
        B2_BYPASS_FILT_REG_MASK,
        &pDrst->sd_ctl_ShadowInitialization1_dw,
        wBypassSetting);

}

_inline INT STFM1000GearShiftPtGear(DWORD dwBits)
{
    INT RetCode;
    // Shift gear
    RetCode = STFM1000WriteRegister(
        PILOTTRACKING_REG_ADDR,
        (BYTE1_MASK | BYTE0_MASK),
        &StfmTuneData.sd_ctl_ShadowPilotTracking_dw,
        dwBits);

    if (SUCCESS != RetCode)
    {
        //SystemHalt();
        return RetCode;
    }

    return RetCode;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          TunerTuningInit
//
//   Type:          Function
//
//   Description:   Init some of globals
//
//   Inputs:        none
//
//   Outputs:       Error status
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
_reentrant void TunerTuningInit(void)
{
	g_wFMTunerError.m_bBAND_LIMIT_HIGH_ERROR = FALSE;
	g_wFMTunerError.m_bBAND_LIMIT_LOW_ERROR = FALSE;	// clear the searching error bits
	g_wFMTunerError.m_bTUNER_BUSY = TRUE;
    g_iTunerProcessIntervalMs = 50;                     // Reset to 50ms
}



////////////////////////////////////////////////////////////////////////////////
//!
//! \brief      Enables DRI transmission
//! \fntype     Function
//!
//! Function will perform the following steps:
//!     Enable DRI stream to transmit from chip
//!     Enable 3600 DMA
//!     Start AGC Monitor
//!
//! \param[in]  None
//!
//! \return     SUCCESS/ERROR.
//!
////////////////////////////////////////////////////////////////////////////////
_inline INT STFM1000DriOn(void)
{
    INT RetCode = SUCCESS;
    STFM1000Tune_t * pDrst = &StfmTuneData;
    // * Starts DRI transmission from tuner (tspec_v09u.pdf::DRI_on)
    RetCode = STFM1000WriteRegister(
        DATAPATH_REG_ADDR,
        DRI_EN_REG_MASK,
        &pDrst->sd_ctl_ShadowDataPath_dw,
        1);
    if (SUCCESS != RetCode)
    {
        //SystemHalt();
        return RetCode;
    }
    return RetCode;
}




////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          TunerDriverInit
//
//   Type:          Function
//
//   Description:   Init FM tuner ardware to be ready for operation
//
//   Inputs:        none
//
//   Outputs:       Error status
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
/******************************************************************************
 * Group TunerDriverInit, STFM1000TunerInit, STFM1000Reset, STFM1000RegsValsReset
 * and I2CInit together as one funclet since these functions are called once
 * during TunerDriverInit
 ******************************************************************************/
#ifdef FUNCLET
    #pragma asm
        org p,".stfm1000_SYS":
        FTunerDriverInit:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet2 RSRC_FUNCLET_TUNER_DRIVER_INIT

        org p,"SYSFUNCLET_TUNER_DRIVER_INIT_P":
        dc      RSRC_FUNCLET_TUNER_DRIVER_INIT
    #pragma endasm
_reentrant INT TunerDriverInit_Funclet(void)
#else
_reentrant INT TunerDriverInit(void)
#endif
{
    INT iReturn;
// In the software (c filter)so we probably should call
#ifdef FM_TUNER_HCC
// Bypass_BWCtrl 0: HCC= 1 enbale/1: HCC disbale
#endif
#ifdef FM_TUNER_SNC
// BypassBlend 0: SNC=1 enable/1: SNC= 0 disable
#endif
#ifdef FM_TUNER_SOFT_MUTE
// BypassSoftMute: 0: Softmute=1 enable/1: softmute=0, disable softmute
#endif
#ifdef FM_JAPAN_REGION
// FIll the empty
#endif
#ifdef FM_USA_REGION
// FIll the empty
#endif

    g_wFMTunerError.m_bTUNER_BUSY= FALSE;
    g_wFMTunerError.m_bSEARCHING_UP=FALSE;
    g_wFMTunerError.m_bSEARCHING_DOWN=FALSE;
    g_wFMTunerError.m_bSCANNING_STATION=FALSE;
    g_wFMTunerError.m_bBAND_LIMIT_HIGH_ERROR=FALSE;
    g_wFMTunerError.m_bBAND_LIMIT_LOW_ERROR=FALSE;
    g_wFMTunerError.m_bUNUSED_PRESET=FALSE;
    g_wFMTunerError.m_bPRESET_RANGE_ERROR=FALSE;
    g_wFMTunerError.m_bPOWERED_OFF = TRUE;
    // test if the tuner exist, write and read data while the tuner powered up
    g_wFMTunerError.m_bTUNER_NOT_FOUND=TRUE;
    // Quick test and tuner
    // Check the error code for return

    iReturn = STFM1000TunerInit();

    if(iReturn != SUCCESS)
    {
        return iReturn;
    }

    // Turn-off tuner in case in the debug environment you didn't tuner off tuner properly
    iReturn = TunerDriverSetTunerStandby(TRUE);
    if(iReturn != SUCCESS)
    {
        return iReturn;
    }

    return TUNER_SUCCESS;

}


////////////////////////////////////////////////////////////////////////////////
//!
//! \brief      Reset STFM software
//! \fntype     Function
//!
//! Function will perform the following steps:
//!     Initialize TUNE1 & SDNOMIBNAL tables and
//!
//! \param[in]  None
//!
//! \return     None
//!
////////////////////////////////////////////////////////////////////////////////

_reentrant void STFM1000Reset(void)
{
#ifdef STFM1000_PLL_TABLE
    unsigned int ndiv;           // N Divider in PLL
    unsigned int incr;           // Increment in PLL
    unsigned int cicosr;         // CIC oversampling ratio
    DWORD sdnominal;      // value to serve pilot/interpolator loop in SD
    int temp;
    Fract_type FractTemp;
#endif
    // ptr to X in Y syntax is:    int_X * _Y ptrInYtoX; // _Y is default so not required
    STFM1000Tune_t _X * pStfmTuneDefault = &StfmTuneDefault;
    STFM1000Tune_t * pStfmTuneData = &StfmTuneData;

    // The following two tables form the tuning tables for programming
    // TUNE1_REG[14:0] and SDNOMINAL_REG[31:0].  These tables should be indexed
    // with the actual frequency which the Analog Front End must tune (fe_freq)
    // and NOT the frequency which the listener wants to tune to (tune_freq)
    // They are related as follows: fe_freq = tune_freq + IF
    // NOTE: These two tables should be built up and stored in memory.  The
    // following calculations will NOT actually be made in MCU code.
    // Memory required is as follows:  tune1_table = 325 x 32 bits
    //                                 sdnom_table = 325 x 32 bits
#ifdef STFM1000_PLL_TABLE
    for (i = FREQUENCY_100KHZ_MIN;
        i < FREQUENCY_100KHZ_MIN + FREQUENCY_100KHZ_RANGE; i++)
    {
#if 0
        ndiv = (DWORD)((i+14)/15) - 48;
        incr = i - (DWORD)(i/15)*15;
        cicosr = (DWORD)(i*2/3.0/16.0 + 0.5);
        sdnominal =
            (DWORD)(i*100.0e3/1.5/(DWORD)cicosr/2.0/2.0*2.0*8.0*256.0/228.0e3*65536);
#else
        FractTemp.I = 2048 * (i + 14) + 1;          // scale for rounding
        FractTemp.F = FractTemp.F * OneFifteenth;   // Multiply as _fract values
        ndiv = FractTemp.I >> 11;                   // unscale for rounding
        ndiv = ndiv - 48;

        FractTemp.I = (2048 * i) + 1;               // scale for rounding
        FractTemp.F = FractTemp.F * OneFifteenth;   // Multiply as _fract values
        temp = FractTemp.I >> 11;                   // unscale for rounding
        temp = temp * 15;
        incr = i - temp;

        FractTemp.I = 1024 * i;
        FractTemp.F = FractTemp.F * OneTwelfth;         // Multiply as _fract values
        cicosr = FractTemp.I;
        cicosr = cicosr + 1024;                         // Round to nearest 0.5
        cicosr = cicosr >> 11;                          // Unscale for rounding

        sdnominal = i * FractCicosr[cicosr - 32];   // Array is pre-scaled for rounding
        sdnominal = sdnominal >> 12;                // unscale for rounding
#endif

        temp = 0x00000000;                                    // clear
        temp = temp | ((cicosr<<9) & TUNE1_CICOSR_REG_MASK);  // bits[14:9] 0x00007E00
        temp = temp | ((ndiv<<4)   & TUNE1_PLL_DIV_REG_MASK); // bits[8:4]  0x000001F0
        temp = temp | ((incr)      & 0x0000000F); // bits[3:0]  0x0000000F
        sdc_ctl_TableTune1_w[i - FREQUENCY_100KHZ_MIN] = (WORD)(temp & 0x001FFFFF);
        sdc_ctl_TableSdnom_dw[i - FREQUENCY_100KHZ_MIN] = sdnominal;
    }
#ifdef TEST_BUILD_TABLE

    // Use the following BoxView command to dump sdc_ctl_TableSdnom_dw to file MyData in 2 columns
    // OUTPUT MyData -2
    // OUTPUT OFF       ## closes all files
    // OUTPUT           ## lists open files
    // close all        ## closes all files
    __asm(" move #$0103c0,x0");     // 324 words to file #1
    __asm(" move #$000c00,r0");     // read from address xc00
    __asm(" move #$004001,r1");     // read from space X


    SystemHalt();   // at address P:E5D3
    SystemHalt();
    SystemHalt();
#endif
#endif
    pStfmTuneDefault->sd_ctl_ShadowTune1_dw = 0;
    pStfmTuneDefault->sd_ctl_ShadowSdnominal_dw = 0;
    pStfmTuneDefault->sd_ctl_IfFreq_i = 0;
    pStfmTuneDefault->sd_ctl_FeFreq_w = 0;
    pStfmTuneDefault->sd_ctl_Tune1Data_dw = 0;
    pStfmTuneDefault->sd_ctl_SdnominalData_dw = 0;

    // Temporary Initialization of Threshold value(s)
    pStfmTuneData->sd_ctl_TuneRssiTh_w  = 28;
    pStfmTuneData->sd_ctl_TuneMpxDcTh_w = 300;
    pStfmTuneData->sd_ctl_AdjChanTh_w   = 100;
    pStfmTuneData->sd_ctl_PilotEstTh_w  = 28;
    pStfmTuneData->sd_ctl_AgcMonitorDisabled  = TRUE;

}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief      Reset STFM shadow registers (not h/w registers)
//! \fntype     Function
//!
//! Function will perform the following steps:
//!     Initialize the STFM shadow registers to known state
//!
//! \param[in]  None
//!
//! \return     None
//!
////////////////////////////////////////////////////////////////////////////////


_reentrant void STFM1000RegValsReset(void)
{
    // NOTE: This reset() section is meant to build a set of initial values to
    // be stored in a table in memory.
    // Initialization Table = 17 words  x 32 bits
    // Updated on 10/31/2005 according to Jon Hendrix. Moved to X in feb '06.
    STFM1000Tune_t _X * pStfmTuneDefault = &StfmTuneDefault;
    INT RetCode = SUCCESS;
    DWORD dwShadowInitialization6Reg;
    INT i;

#ifndef STFM1000_POWER_REDUCTION_VERSION    // Must be first since TB2 can overwrite values
    pStfmTuneDefault->sd_ctl_ShadowMixFilt_dw = 0x00007200; // MIXFILT_REG
    pStfmTuneDefault->sd_ctl_ShadowAdc_dw = 0x00033000;     // ADC_REG
#endif
    // dec 2006: These numbers are derived from the stfm1000 t-spec (ic designers spec). See stfm1000 designers for more info. g:\consumer audio\steely dan\  t specs  ta2a or tb2
if (VERSION_TB2 == StfmTuneDefault.STFM1000_Version)
{
    pStfmTuneDefault->sd_ctl_ShadowTune1_dw           = 0x00044F01; // TUNE1_REG
    pStfmTuneDefault->sd_ctl_ShadowSdnominal_dw       = 0x1C5EBCF0; // Shadow reg for SDNOMINAL_REG
    pStfmTuneDefault->sd_ctl_ShadowPilotTracking_dw   = 0x000001B6; // Shadow reg for PILOTTRACKING_REG
    pStfmTuneDefault->sd_ctl_ShadowInitialization1_dw = 0x9fb80008; // Shadow reg for INITIALIZATION1_REG
    pStfmTuneDefault->sd_ctl_ShadowInitialization2_dw = 0x8506e444; // Shadow reg for INITIALIZATION2_REG  Var
    pStfmTuneDefault->sd_ctl_ShadowInitialization3_dw = 0xB400190B;  //Crystal Lock Bypass: 0xB402190B Or Crystal Lock on: 0xB400190B // Shadow reg for INITIALIZATION3_REG
    pStfmTuneDefault->sd_ctl_ShadowInitialization4_dw = 0x525bf052; // Shadow reg for INITIALIZATION4_REG
    pStfmTuneDefault->sd_ctl_ShadowInitialization5_dw = 0x0808d106; // Shadow reg for INITIALIZATION5_REG
    pStfmTuneDefault->sd_ctl_ShadowInitialization6_dw = 0x000166cb;  //0x000366cb; // Shadow reg for INITIALIZATION6_REG
    pStfmTuneDefault->sd_ctl_ShadowRef_dw             = 0x00B8222D; // ShadowRef reg for ANALOG1_REG
    pStfmTuneDefault->sd_ctl_ShadowLna_dw             = 0x0D080009; // ShadowLna reg for ANALOG2_REG
    pStfmTuneDefault->sd_ctl_ShadowMixFilt_dw         = 0x00007205; // ShadowMixFlt reg for ANALOG3_REG
    pStfmTuneDefault->sd_ctl_ShadowClk1_dw            = 0x10002000; // ShadowClk1 reg for ANALOG4_REG
    pStfmTuneDefault->sd_ctl_ShadowClk2_dw            = 0x7F000000; // ShadowClk2 reg for ANALOG4_REG
    pStfmTuneDefault->sd_ctl_ShadowAdc_dw             = 0x001B3282; // ShadowAdc reg for ANALOG5_REG Var
    pStfmTuneDefault->sd_ctl_ShadowAgcControl1_dw     = 0x1BCB2202; // Shadow reg for AGC_CONTROL1_REG
    pStfmTuneDefault->sd_ctl_ShadowAgcControl2_dw     = 0x000000F0; // Shadow reg for AGC_CONTROL2_REG
    pStfmTuneDefault->sd_ctl_ShadowDataPath_dw        = 0x00010210; // Shadow reg for DATAPATH_REG
#ifdef FM_TUNER_US_DEEMPHASIS
    pStfmTuneDefault->sd_ctl_ShadowInitialization2_dw = 0x8506e444; // INITIALIZATION2_REG, clear bit[8], 75us de-emphasis,
#else
    pStfmTuneDefault->sd_ctl_ShadowInitialization2_dw = 0x8506e544; // set it to 50us de-emphasis
#endif

}   //VERSION_TB2

if (VERSION_TA2 == StfmTuneDefault.STFM1000_Version)
{
    pStfmTuneDefault->sd_ctl_ShadowTune1_dw           = 0x00044F01; // TUNE1_REG
    pStfmTuneDefault->sd_ctl_ShadowSdnominal_dw       = 0x1C5EBCF0; // Shadow reg for SDNOMINAL_REG
    pStfmTuneDefault->sd_ctl_ShadowPilotTracking_dw   = 0x000001B6; // Shadow reg for PILOTTRACKING_REG
    pStfmTuneDefault->sd_ctl_ShadowInitialization1_dw = 0x9fb80008; // Shadow reg for INITIALIZATION1_REG
//    pStfmTuneDefault->sd_ctl_ShadowInitialization2_dw = 0xC506e444; // Shadow reg for INITIALIZATION2_REG  Var
    pStfmTuneDefault->sd_ctl_ShadowInitialization3_dw = 0x1402190B; // Crypstal Lock Bypassed 0x1402190B; or Crystal Lock on 0x1400190B;  Shadow reg for INITIALIZATION3_REG
    pStfmTuneDefault->sd_ctl_ShadowInitialization4_dw = 0x525bf052; // Shadow reg for INITIALIZATION4_REG
    pStfmTuneDefault->sd_ctl_ShadowInitialization5_dw = 0x6800d106; // Shadow reg for INITIALIZATION5_REG
    pStfmTuneDefault->sd_ctl_ShadowInitialization6_dw = 0x00003ecb; // Shadow reg for INITIALIZATION6_REG
    pStfmTuneDefault->sd_ctl_ShadowRef_dw             = 0x0030222D; // ShadowRef reg for ANALOG1_REG
    pStfmTuneDefault->sd_ctl_ShadowLna_dw             = 0x05080009; // ShadowLna reg for ANALOG2_REG
//    pStfmTuneDefault->sd_ctl_ShadowMixFilt_dw         = 0x00007205; // ShadowMixFlt reg for ANALOG3_REG
    pStfmTuneDefault->sd_ctl_ShadowClk1_dw            = 0x10002000; // ShadowClk1 reg for ANALOG4_REG
    pStfmTuneDefault->sd_ctl_ShadowClk2_dw            = 0x7F000000; // ShadowClk2 reg for ANALOG4_REG
//    pStfmTuneDefault->sd_ctl_ShadowAdc_dw             = 0x00033282; // ShadowAdc reg for ANALOG5_REG Var
    pStfmTuneDefault->sd_ctl_ShadowAgcControl1_dw     = 0x002c8702; // Shadow reg for AGC_CONTROL1_REG
    pStfmTuneDefault->sd_ctl_ShadowAgcControl2_dw     = 0x00140050; // Shadow reg for AGC_CONTROL2_REG
    pStfmTuneDefault->sd_ctl_ShadowDataPath_dw        = 0x00014210; // Shadow reg for DATAPATH_REG

#ifdef FM_TUNER_US_DEEMPHASIS
    pStfmTuneDefault->sd_ctl_ShadowInitialization2_dw = 0xC506e444; // INITIALIZATION2_REG, clear bit[8], 75us de-emphasis,
#else
    pStfmTuneDefault->sd_ctl_ShadowInitialization2_dw = 0xC506e544; // set it to 50us de-emphasis
#endif
}   //VERSION_TA2

}   //STFM1000RegValsReset()

#ifdef FUNCLET
    #pragma asm
        org p,".ptextstfm1000":
    #pragma endasm
#endif




////////////////////////////////////////////////////////////////////////////////
//!
//! \brief      STFM tuner power-up
//! \fntype     Function
//!
//! Function will perform the following steps:
//!     Opens a timer device
//!     Sets tuner power on
//!     Initializes DRI interface from tuner (ddi_dri_PowerUp())
//!
//! \param[in]  pTunerObject     Pointer to Tuner Object.
//!
//! \return     SUCCESS/ERROR.
//!
////////////////////////////////////////////////////////////////////////////////
// not a funclet currently.
#if 0 //def FUNCLET
    #pragma asm
        org p,".stfm1000_SYS":
        FSTFM1000TunerPowerUp:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet2 RSRC_FUNCLET_STFM1000_POWER_UP

        org p,"SYSFUNCLET_STFM1000_POWER_UP_P":
        dc      RSRC_FUNCLET_STFM1000_POWER_UP
    #pragma endasm
_reentrant INT STFM1000TunerPowerUp_Funclet(void)
#else
_reentrant INT STFM1000TunerPowerUp(void)
#endif
{
    INT RetCode = SUCCESS;

    // set tuner power on
    RetCode = STFM1000RegsPowerUp1();
    if (SUCCESS != RetCode)
    {
        //SystemHalt();
        return RetCode;
    }
    RetCode = STFM1000RegsPowerUp2();
    if (SUCCESS != RetCode)
    {
        //SystemHalt();
        return RetCode;
    }


#if 0
    /* Run through bring-up sequence */
    RetCode = STFM1000BringUp();
    if (SUCCESS != RetCode)
    {
        //SystemHalt();
        return RetCode;
    }
#endif
    return RetCode;
}
////////////////////////////////////////////////////////////////////////////////
//!
//! \brief      STFM register initialization for power-up
//! \fntype     Function
//!
//! \param[in]  None
//!
//! \return     SUCCESS/ERROR.
//!
////////////////////////////////////////////////////////////////////////////////
#ifdef FUNCLET
    #pragma asm
        FSTFM1000RegsPowerUp1:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet2 RSRC_FUNCLET_STFM1000_REG_POWER_UP1

        org p,"SYSFUNCLET_STFM1000_REG_POWER_UP1_P":
        dc      RSRC_FUNCLET_STFM1000_REG_POWER_UP1
    #pragma endasm
_reentrant INT STFM1000RegsPowerUp1_Funclet(void)
#else
_reentrant INT STFM1000RegsPowerUp1(void)
#endif
{
    STFM1000Tune_t * pStfmTuneData = &StfmTuneData;
    STFM1000Tune_t _X * pStfmTuneDefault = &StfmTuneDefault; // moved to X feb '06
    INT RetCodes[40]; // Allows storing upto 29 register write results
    DWORD dwShadowTemp;
    int iIndex = 1;

    RetCodes[0] = SUCCESS;

    RetCodes[iIndex++] = STFM1000WriteRegister(
        REF_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowRef_dw,
        0x00200000);

    /* Wait at least 1 msec */
    STFM1000delay_ms(1);

    /* Write to all Steely Dan control registers across I2C link
       Start off by accepting all B2_x register writes,
              else some registers won't "accept" data */
    RetCodes[iIndex++] = STFM1000WriteRegister(
        DATAPATH_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowDataPath_dw,
        pStfmTuneDefault->sd_ctl_ShadowDataPath_dw);

    // Now we can write to all other registers
    RetCodes[iIndex++] = STFM1000WriteRegister(
        TUNE1_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowTune1_dw,
        pStfmTuneDefault->sd_ctl_ShadowTune1_dw);

    RetCodes[iIndex++] = STFM1000WriteRegister(
        SDNOMINAL_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowSdnominal_dw,
        pStfmTuneDefault->sd_ctl_ShadowSdnominal_dw);
    RetCodes[iIndex++] = STFM1000WriteRegister(
        PILOTTRACKING_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowPilotTracking_dw,
        pStfmTuneDefault->sd_ctl_ShadowPilotTracking_dw);
    RetCodes[iIndex++] = STFM1000WriteRegister(
        INITIALIZATION1_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowInitialization1_dw,
        pStfmTuneDefault->sd_ctl_ShadowInitialization1_dw);
    RetCodes[iIndex++] = STFM1000WriteRegister(
        INITIALIZATION2_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowInitialization2_dw,
        pStfmTuneDefault->sd_ctl_ShadowInitialization2_dw);
    RetCodes[iIndex++] = STFM1000WriteRegister(
        INITIALIZATION3_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowInitialization3_dw,
        pStfmTuneDefault->sd_ctl_ShadowInitialization3_dw);
    RetCodes[iIndex++] = STFM1000WriteRegister(
        INITIALIZATION4_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowInitialization4_dw,
        pStfmTuneDefault->sd_ctl_ShadowInitialization4_dw);
    RetCodes[iIndex++] = STFM1000WriteRegister(
        INITIALIZATION5_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowInitialization5_dw,
        pStfmTuneDefault->sd_ctl_ShadowInitialization5_dw);
    RetCodes[iIndex++] = STFM1000WriteRegister(
        INITIALIZATION6_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowInitialization6_dw,
        pStfmTuneDefault->sd_ctl_ShadowInitialization6_dw);
    RetCodes[iIndex++] = STFM1000WriteRegister(
        AGC_CONTROL1_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowAgcControl1_dw,
        pStfmTuneDefault->sd_ctl_ShadowAgcControl1_dw);
    RetCodes[iIndex++] = STFM1000WriteRegister(
        AGC_CONTROL2_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowAgcControl2_dw,
        pStfmTuneDefault->sd_ctl_ShadowAgcControl2_dw);
    // Did any register's initialization fail?
    for(  iIndex -= 1; iIndex > 0; --iIndex)
    {
        if (SUCCESS != RetCodes[iIndex])
        {
            //SystemHalt();
            break;
        }
    }
    return RetCodes[iIndex];

}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextstfm1000":
    #pragma endasm
#endif

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief      STFM register initialization for power-up
//! \fntype     Function
//!
//! \param[in]  None
//!
//! \return     SUCCESS/ERROR.
//!
////////////////////////////////////////////////////////////////////////////////
#ifdef FUNCLET
    #pragma asm
        FSTFM1000RegsPowerUp2:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet2 RSRC_FUNCLET_STFM1000_REG_POWER_UP2

        org p,"SYSFUNCLET_STFM1000_REG_POWER_UP2_P":
        dc      RSRC_FUNCLET_STFM1000_REG_POWER_UP2
    #pragma endasm
_reentrant INT STFM1000RegsPowerUp2_Funclet(void)
#else
_reentrant INT STFM1000RegsPowerUp2(void)
#endif
{
    STFM1000Tune_t * pStfmTuneData = &StfmTuneData;
    STFM1000Tune_t _X * pStfmTuneDefault = &StfmTuneDefault; // moved to X feb '06
    INT RetCodes[40]; // Allows storing upto 29 register write results
    DWORD dwShadowTemp;
    int iIndex = 1;

    RetCodes[0] = SUCCESS;

    RetCodes[iIndex++] = STFM1000WriteRegister(
        CLK1_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowClk1_dw,
        0x10000000);

    RetCodes[iIndex++] = STFM1000WriteRegister(
        CLK1_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowClk1_dw,
        0x20000000);

    RetCodes[iIndex++] = STFM1000WriteRegister(
        CLK1_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowClk1_dw,
        0x00000000);

    RetCodes[iIndex++] = STFM1000WriteRegister(
        CLK2_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowClk2_dw,
        pStfmTuneDefault->sd_ctl_ShadowClk2_dw);

    RetCodes[iIndex++] = STFM1000WriteRegister(
        REF_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowRef_dw,
        pStfmTuneDefault->sd_ctl_ShadowRef_dw);

    RetCodes[iIndex++] = STFM1000WriteRegister(
        CLK1_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowClk1_dw,
        0x30000000);

    RetCodes[iIndex++] = STFM1000WriteRegister(
        CLK1_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowClk1_dw,
        0x30002000);

    RetCodes[iIndex++] = STFM1000WriteRegister(
        CLK1_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowClk1_dw,
        pStfmTuneDefault->sd_ctl_ShadowClk1_dw);

    RetCodes[iIndex++] = STFM1000WriteRegister(
        LNA_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowLna_dw,
        pStfmTuneDefault->sd_ctl_ShadowLna_dw);

    RetCodes[iIndex++] = STFM1000WriteRegister(
        MIXFILT_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowMixFilt_dw,
        0x00008000);

    RetCodes[iIndex++] = STFM1000WriteRegister(
        MIXFILT_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowMixFilt_dw,
        0x00000000);

    RetCodes[iIndex++] = STFM1000WriteRegister(
        MIXFILT_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowMixFilt_dw,
        pStfmTuneDefault->sd_ctl_ShadowMixFilt_dw);    // 0x00007205

    RetCodes[iIndex++] = STFM1000WriteRegister(
        ADC_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowAdc_dw,
        pStfmTuneDefault->sd_ctl_ShadowAdc_dw);     // 0x00033282

    /* Following is a RO register, however writing to it does clear bits */
    RetCodes[iIndex++] = STFM1000WriteRegister(
        ATTENTION_REG_ADDR,
        WHOLEREG_MASK,
        &dwShadowTemp,
        0x0000003F);

    // Did any register's initialization fail?
    for(  iIndex -= 1; iIndex > 0; --iIndex)
    {
        if (SUCCESS != RetCodes[iIndex])
        {
            //SystemHalt();
            break;
        }
    }
    return RetCodes[iIndex];
}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextstfm1000":
    #pragma endasm
#endif

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          TunerDriverIncrementUp
//
//   Type:          Function
//
//   Description:   Change FM frequency up one FM_TUNER_GRID and tune to it
//
//   Inputs:        none
//
//   Outputs:       Error status
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
#ifdef FUNCLET
    #pragma asm
        org p,".stfm1000_SYS":
        FTunerDriverIncrementUp:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet2 RSRC_FUNCLET_TUNER_DRIVER_INC_UP

        org p,"SYSFUNCLET_TUNER_DRIVER_INC_UP_P":
        dc      RSRC_FUNCLET_TUNER_DRIVER_INC_UP
    #pragma endasm
_reentrant INT TunerDriverIncrementUp_Funclet(void)
#else
_reentrant INT TunerDriverIncrementUp(void)
#endif
{
	if (s_wCurrentFMFrequency < FM_BAND_LIMIT_HIGH_FREQ)
    {
#ifdef SD_USE_50KHZ_TUNING_GRID
        s_wCurrentFMFrequency = 50 * (INT) ((s_wDisplayFMFrequency+50)/50);
#else
#ifdef SD_USE_100KHZ_TUNING_GRID

        s_wCurrentFMFrequency = FM_TUNER_GRID * (INT) ((s_wCurrentFMFrequency+FM_TUNER_GRID)/FM_TUNER_GRID);
#else
        s_wCurrentFMFrequency = (s_wCurrentFMFrequency+FM_TUNER_GRID);
#endif
#endif
    }
    else
	{
        s_wCurrentFMFrequency = FM_BAND_LIMIT_LOW_FREQ;
	}
#ifdef SD_USE_50KHZ_TUNING_GRID
    s_wDisplayFMFrequency = s_wCurrentFMFrequency;
#endif
    return (TunerDriverTuneToFrequency(s_wCurrentFMFrequency));
}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextstfm1000":
    #pragma endasm
#endif


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          TunerDriverIncrementDown
//
//   Type:          Function
//
//   Description:   Change FM frequency down one FM_TUNER_GRID and tune to it
//
//   Inputs:        none
//
//   Outputs:       Error status
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
#ifdef FUNCLET
    #pragma asm
        org p,".stfm1000_SYS":
        FTunerDriverIncrementDown:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet2 RSRC_FUNCLET_TUNER_DRIVER_INC_DOWN

        org p,"SYSFUNCLET_TUNER_DRIVER_INC_DOWN_P":
        dc      RSRC_FUNCLET_TUNER_DRIVER_INC_DOWN
    #pragma endasm
_reentrant INT TunerDriverIncrementDown_Funclet(void)
#else
_reentrant INT TunerDriverIncrementDown(void)
#endif
{

	if (s_wCurrentFMFrequency > FM_BAND_LIMIT_LOW_FREQ)
    {
#ifdef SD_USE_50KHZ_TUNING_GRID
        s_wCurrentFMFrequency = 50 * (INT) ((s_wDisplayFMFrequency-50)/50);
#else
#ifdef SD_USE_100KHZ_TUNING_GRID

        s_wCurrentFMFrequency = FM_TUNER_GRID * (INT) ((s_wCurrentFMFrequency-FM_TUNER_GRID)/FM_TUNER_GRID);
#else
        s_wCurrentFMFrequency = (s_wCurrentFMFrequency-FM_TUNER_GRID);

#endif
#endif
	}
    else
	{
		s_wCurrentFMFrequency = FM_BAND_LIMIT_HIGH_FREQ;
	}
#ifdef SD_USE_50KHZ_TUNING_GRID
    s_wDisplayFMFrequency = s_wCurrentFMFrequency;
#endif
    return (TunerDriverTuneToFrequency(s_wCurrentFMFrequency));

}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextstfm1000":
    #pragma endasm
#endif

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          TunerDriverSetTunerStandby
//
//   Type:          Function
//
//   Description:   Set tuner as standby or power-on state
//
//   Inputs:        bStandby: TRUE - Standby/FALSE - power-on
//
//   Outputs:       return iTuningStatus
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////

#ifdef FUNCLET
    #pragma asm
        org p,".stfm1000_SYS":
        FTunerDriverSetTunerStandby:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet2 RSRC_FUNCLET_TUNER_DRIVER_SET_STANDBY

        org p,"SYSFUNCLET_TUNER_DRIVER_SET_STANDBY_P":
        dc      RSRC_FUNCLET_TUNER_DRIVER_SET_STANDBY
    #pragma endasm
_reentrant INT TunerDriverSetTunerStandby_Funclet(BOOL bStandby )
#else
_reentrant INT TunerDriverSetTunerStandby(BOOL bStandby )
#endif
{
    INT iReturn;
    // Reset this two parameters
    s_eOptimizeChannelState = OPTIMIZE_CHANNEL_STATE_COMPLETE;
    s_eMonitorSignalQualityState =  MONITOR_SIGNAL_QUALITY_STATE_COMPLETE;

    if (bStandby)
	{
        // DRI off
        iReturn = STFM1000DriOff();
        if( 0 !=  iReturn)
        {
            return iReturn;
        }
        // Take down
        iReturn = STFM1000TakeDown();
        if( 0 !=  iReturn)
        {
            return iReturn;
        }

        iReturn = STFM1000TunerLowPowerMode();
        if( 0 !=  iReturn)
        {
            return iReturn;
        }
        //STFM1000I2CClose();
        g_wFMTunerError.m_bPOWERED_OFF = TRUE;

	}
	else
	{
        // Update the init I2C
        iReturn = STFM1000I2CInit(0, 0);
        // Things realy bad
        if( 0 !=  iReturn)
        {
            //SystemHalt();
            return iReturn;
        }
        iReturn = STFM1000TunerPowerUp();
        if( 0 !=  iReturn)
        {
            return iReturn;
        }
        iReturn = STFM1000BringUp();
        if( 0 !=  iReturn)
        {
            return iReturn;
        }
        if (bIsFirstTimeEnterFMMode == TRUE)
        {
            iReturn = STFM1000FindRSSIThresHold();
            if( 0 !=  iReturn)
            {
                return iReturn;
            }
            bIsFirstTimeEnterFMMode = FALSE;
        }
        STFM1000DriOff();
        g_wFMTunerError.m_bPOWERED_OFF = FALSE;
        // Restore the frequency from settings.dat
        s_wCurrentFMFrequency = g_wCurrentFrequency;
#ifdef SD_USE_50KHZ_TUNING_GRID
        s_wDisplayFMFrequency = s_wCurrentFMFrequency;
#endif
	}
    return TUNER_SUCCESS;
}
_reentrant INT STFM1000TunerLowPowerMode(void)
{
    STFM1000Tune_t * pStfmTuneData = &StfmTuneData;
    INT RetCodes;
#if 0
    RetCodes = STFM1000WriteRegister(
        INITIALIZATION6_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowDataPath_dw,
        0x0000fecb);
    if (SUCCESS != RetCodes)
    {
        //SystemHalt();
        return RetCodes;
    }
#endif
    RetCodes = STFM1000WriteRegister(
        DATAPATH_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowDataPath_dw,
        0x00010210);
    if (SUCCESS != RetCodes)
    {
        //SystemHalt();
        return RetCodes;
    }
    // Set all the control registers to default
    RetCodes = STFM1000WriteRegister(
        TUNE1_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowTune1_dw,
        0x00000000);
    if (SUCCESS != RetCodes)
    {
        //SystemHalt();
            return RetCodes;

    }


    RetCodes = STFM1000WriteRegister(
        SDNOMINAL_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowSdnominal_dw,
        0x00000000);
    if (SUCCESS != RetCodes)
    {
        //SystemHalt();
        return RetCodes;

    }

    RetCodes = STFM1000WriteRegister(
        PILOTTRACKING_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowPilotTracking_dw,
        0x00000000);
    if (SUCCESS != RetCodes)
    {
        //SystemHalt();
        return RetCodes;

    }

    RetCodes = STFM1000WriteRegister(
        INITIALIZATION1_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowInitialization1_dw,
        0x00000000);
    if (SUCCESS != RetCodes)
    {
        //SystemHalt();
        return RetCodes;

    }

    RetCodes = STFM1000WriteRegister(
        INITIALIZATION2_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowInitialization2_dw,
        0x00000000);
    if (SUCCESS != RetCodes)
    {
        //SystemHalt();
        return RetCodes;

    }

    RetCodes = STFM1000WriteRegister(
        INITIALIZATION3_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowInitialization3_dw,
        0x00000000);
    if (SUCCESS != RetCodes)
    {
        //SystemHalt();
        return RetCodes;

    }

    RetCodes = STFM1000WriteRegister(
        INITIALIZATION4_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowInitialization4_dw,
        0x00000000);
    if (SUCCESS != RetCodes)
    {
        //SystemHalt();
        return RetCodes;

    }

    RetCodes = STFM1000WriteRegister(
        INITIALIZATION5_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowInitialization5_dw,
        0x00000000);
    if (SUCCESS != RetCodes)
    {
        //SystemHalt();
        return RetCodes;

    }

    if (VERSION_TB2 == StfmTuneDefault.STFM1000_Version) //Different power down state for TB2
    {
        RetCodes = STFM1000WriteRegister(
            INITIALIZATION6_REG_ADDR,
            WHOLEREG_MASK,
            &pStfmTuneData->sd_ctl_ShadowInitialization6_dw,
            0x00017E00);//0x00007E00);
    }
    else
    {
        RetCodes = STFM1000WriteRegister(
            INITIALIZATION6_REG_ADDR,
            WHOLEREG_MASK,
            &pStfmTuneData->sd_ctl_ShadowInitialization6_dw,
            0x0000FC00);// 0x0000C000);STMP00012489
    }
    if (SUCCESS != RetCodes)
    {
        //SystemHalt();
        return RetCodes;

    }



    RetCodes = STFM1000WriteRegister(
        REF_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowRef_dw,
        0x00000000);
    if (SUCCESS != RetCodes)
    {
        //SystemHalt();
        return RetCodes;

    }

    RetCodes = STFM1000WriteRegister(
        LNA_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowLna_dw,
        0x00000000);
    if (SUCCESS != RetCodes)
    {
        //SystemHalt();
        return RetCodes;

    }

    RetCodes = STFM1000WriteRegister(
        MIXFILT_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowMixFilt_dw,
        0x00000000);
    if (SUCCESS != RetCodes)
    {
        //SystemHalt();
        return RetCodes;

    }

    RetCodes = STFM1000WriteRegister(
        CLK1_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowClk1_dw,
        0x20000000);
    if (SUCCESS != RetCodes)
    {
        //SystemHalt();
        return RetCodes;        
        
    }
 
    RetCodes = STFM1000WriteRegister(
        CLK1_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowClk1_dw,
        0x00000003);  //0x03400000);   // Set bit 22, 24, and 25 to reduce the off-state current by 200uA. sdk2.610 release value was 0x0.
    if (SUCCESS != RetCodes)
    {
        //SystemHalt();
        return RetCodes;

    }

    RetCodes = STFM1000WriteRegister(
        CLK2_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowClk2_dw,
        0x00000000);
    if (SUCCESS != RetCodes)
    {
        //SystemHalt();
        return RetCodes;

    }

    RetCodes = STFM1000WriteRegister(
        ADC_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowAdc_dw,
        0x00000000);
    if (SUCCESS != RetCodes)
    {
        //SystemHalt();
        return RetCodes;

    }
    RetCodes = STFM1000WriteRegister(
        AGC_CONTROL1_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowAgcControl1_dw,
        0x00000000);
    if (SUCCESS != RetCodes)
    {
        //SystemHalt();
        return RetCodes;

    }

    RetCodes = STFM1000WriteRegister(
        AGC_CONTROL2_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowAgcControl2_dw,
        0x00000000);
    if (SUCCESS != RetCodes)
    {
        //SystemHalt();
        return RetCodes;

    }


#if 0

    RetCodes = STFM1000WriteRegister(
        REF_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowRef_dw,
        0x00000000);
    if (SUCCESS != RetCodes)
    {
        //SystemHalt();
          return RetCodes;

    }


    RetCodes = STFM1000WriteRegister(
        LNA_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowLna_dw,
        0x00000000);
    if (SUCCESS != RetCodes)
    {
        //SystemHalt();
        return RetCodes;

    }

    RetCodes = STFM1000WriteRegister(
        MIXFILT_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowMixFilt_dw,
        0x00000000);
    if (SUCCESS != RetCodes)
    {
        //SystemHalt();
            return RetCodes;

    }

    RetCodes = STFM1000WriteRegister(
        CLK1_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowClk1_dw,
        0x00000003);
    if (SUCCESS != RetCodes)
    {
        //SystemHalt();
        return RetCodes;

    }

    RetCodes = STFM1000WriteRegister(
        CLK2_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowClk2_dw,
        0x00000000);
    if (SUCCESS != RetCodes)
    {
        //SystemHalt();
        return RetCodes;

    }

    RetCodes = STFM1000WriteRegister(
        ADC_REG_ADDR,
        WHOLEREG_MASK,
        &pStfmTuneData->sd_ctl_ShadowAdc_dw,
        0x00000000);

    if (SUCCESS != RetCodes)
    {
        //SystemHalt();
        return RetCodes;

    }
#endif
    return RetCodes;

}
_reentrant INT STFM1000TakeDown(void)
{

    INT RetCode = SUCCESS;

    // Stop data processing
    RetCode = STFM1000DataPathOff();
    if (SUCCESS != RetCode)
    {
        //SystemHalt();
        return RetCode;
    }

    return RetCode;
}

// DP_OFF()


////////////////////////////////////////////////////////////////////////////////
//! \internal
//! \brief      This function is called by power-up sequence
//! \fntype     Function
//!
//! Function will perform the following steps:
//!     Tunr on datapath
//!     Initialize
//!     Initializes DRI interface from tuner (ddi_dri_PowerUp())
//!
//! \param[in]  pTunerObject     Pointer to Tuner Object.
//!
//! \return     SUCCESS/ERROR.
//!
////////////////////////////////////////////////////////////////////////////////
_reentrant INT STFM1000BringUp(void)
{

    INT RetCode = SUCCESS;

    // Turn on STFM registers
    RetCode = STFM1000DataPathOn();
    if (SUCCESS != RetCode)
    {
        //SystemHalt();
         return RetCode;
    }
    // Enable DRI stream
    //RetCode = STFM1000DriOn();
    if (SUCCESS != RetCode)
    {
        //SystemHalt();
        return RetCode;
    }
    return RetCode;
}
// DP_ON()
// ****************************************
//! STFM Data Path On
// ****************************************
// * Turn on data path on chip  (tspec_v09u.pdf::SD_DP_On)
_reentrant INT STFM1000DataPathOn(void)
{
    INT RetCode = SUCCESS;
    STFM1000Tune_t * pDrst = &StfmTuneData;


    RetCode = STFM1000WriteRegister(
        DATAPATH_REG_ADDR,
        DP_EN_REG_MASK,
        &pDrst->sd_ctl_ShadowDataPath_dw,
        1);
    if (SUCCESS != RetCode)
    {
        //SystemHalt();
        return RetCode;
    }

    STFM1000delay_ms(3); // wait 3 ms to settle

    RetCode = STFM1000WriteRegister(
        DATAPATH_REG_ADDR,
        DB_ACCEPT_REG_MASK,
        &pDrst->sd_ctl_ShadowDataPath_dw,
        1);
    if (SUCCESS != RetCode)
    {
        //SystemHalt();
          return RetCode;
    }

    RetCode = STFM1000WriteRegister(
        AGC_CONTROL1_REG_ADDR,
        B2_BYPASS_AGC_CTL_REG_MASK,
        &pDrst->sd_ctl_ShadowAgcControl1_dw,
        0);
    if (SUCCESS != RetCode)
    {
        //SystemHalt();
        return RetCode;
    }

    RetCode = STFM1000WriteRegister(
        DATAPATH_REG_ADDR,
        DB_ACCEPT_REG_MASK,
        &pDrst->sd_ctl_ShadowDataPath_dw,
        0);
    if (SUCCESS != RetCode)
    {
        //SystemHalt();
          return RetCode;
    }

    return RetCode;
}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextstfm1000":
    #pragma endasm
#endif

#ifdef FUNCLET
    #pragma asm
        FSTFM1000FindRSSIThresHold:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet2 RSRC_FUNCLET_STFM1000_FIND_RSSI_THRESHOLD

        org p,"SYSFUNCLET_STFM1000_FIND_RSSI_THRESHOLD_P":
        dc      RSRC_FUNCLET_STFM1000_FIND_RSSI_THRESHOLD
    #pragma endasm
_reentrant INT STFM1000FindRSSIThresHold_Funclet( void)
#else
_reentrant INT STFM1000FindRSSIThresHold(void)
#endif
{
    INT iReturn = TUNER_SUCCESS;

    INT iTempFreq = g_wCurrentFrequency;
    INT iTemp1 = 100;
    INT iTestFrequency;
    BOOL bIsAStation;
    // Scanning from 87.5MHz to 88.4 to find the lowest RSSI and use it as
    // noise level, then plus 10 as the default RSSI settings

    for (iTestFrequency = FM_BAND_LIMIT_LOW_FREQ; iTestFrequency < (FM_BAND_LIMIT_LOW_FREQ + 1000); iTestFrequency += FM_TUNER_GRID)
    {
        iReturn = STFM1000TunerQuicksetStation(iTestFrequency);
        if(iReturn != SUCCESS)
        {
            return iReturn;
        }
        // Wait 20 ms for tuner settle before checking the station
        STFM1000delay_ms(20);
        iReturn = STFM1000TunerQueryIsStation(&bIsAStation);
        if(iReturn != SUCCESS)
        {
            return iReturn;
        }

        if(iTemp1 > s_iFMSignalStrengthRSSI)
        {
            iTemp1 = s_iFMSignalStrengthRSSI;
        }
    }
    s_iFMSignalStrengthRSSI = iTemp1 + RSSI_THRESHOLD_OFFSET;
    if(s_iFMSignalStrengthRSSI > 25)
    {
        StfmTuneData.sd_ctl_TuneRssiTh_w = s_iFMSignalStrengthRSSI;
    }

    g_wCurrentFrequency = iTempFreq;
    return iReturn;
}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextstfm1000":
    #pragma endasm
#endif

// ****************************************
//! STFM Data Path Off
// ****************************************
#ifdef FUNCLET
    #pragma asm
        FSTFM1000DataPathOff:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet2 RSRC_FUNCLET_STFM1000_DATA_PATH_OFF

        org p,"SYSFUNCLET_STFM1000_DATA_PATH_OFF_P":
        dc      RSRC_FUNCLET_STFM1000_DATA_PATH_OFF
    #pragma endasm
_reentrant INT STFM1000DataPathOff_Funclet( void)
#else
_reentrant INT STFM1000DataPathOff(void)
#endif
{
    INT RetCode = SUCCESS;
    STFM1000Tune_t * pDrst = &StfmTuneData;

    // * Turns off Data Path on tuner (tspec_v09u.pdf::SD_DP_Off)

    RetCode = STFM1000WriteRegister(
        DATAPATH_REG_ADDR,
        DB_ACCEPT_REG_MASK,
        &pDrst->sd_ctl_ShadowDataPath_dw,
        1);
    if (SUCCESS != RetCode)
    {
        //SystemHalt();
         return RetCode;
    }

    RetCode = STFM1000WriteRegister(
        DATAPATH_REG_ADDR,
        DP_EN_REG_MASK,
        &pDrst->sd_ctl_ShadowDataPath_dw,
        0);
    if (SUCCESS != RetCode)
    {
        //SystemHalt();
        return RetCode;
    }

    RetCode = STFM1000WriteRegister(
        AGC_CONTROL1_REG_ADDR,
        B2_BYPASS_AGC_CTL_REG_MASK,
        &pDrst->sd_ctl_ShadowAgcControl1_dw,
        1);
    if (SUCCESS != RetCode)
    {
        //SystemHalt();
        return RetCode;
    }

    RetCode = STFM1000WriteRegister(
        PILOTTRACKING_REG_ADDR,
        B2_PILOTTRACKING_ENABLE_REG_MASK,
        &pDrst->sd_ctl_ShadowPilotTracking_dw,
        0);
    if (SUCCESS != RetCode)
    {
        //SystemHalt();
        return RetCode;
    }

    RetCode = STFM1000WriteRegister(
        DATAPATH_REG_ADDR,
        DB_ACCEPT_REG_MASK,
        &pDrst->sd_ctl_ShadowDataPath_dw,
        0);
    if (SUCCESS != RetCode)
    {
        //SystemHalt();
         return RetCode;
    }

    return RetCode;
}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextstfm1000":
    #pragma endasm
#endif

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          TunerDriverSetPresetStations
//
//   Type:          Function
//
//   Description:   Replace the lastest preset station with the current tuned station
//
//   Inputs:        iPresetStation
//
//   Outputs:       Error status
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
#ifdef FUNCLET
    #pragma asm
        org p,".stfm1000_SYS":
        FTunerDriverSetPresetStations:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet2 RSRC_FUNCLET_TUNER_DRIVER_REPLACE_PRESET

        org p,"SYSFUNCLET_TUNER_DRIVER_REPLACE_PRESET_P":
        dc      RSRC_FUNCLET_TUNER_DRIVER_REPLACE_PRESET
    #pragma endasm
_reentrant INT TunerDriverSetPresetStations_Funclet( INT iPresetStation)
#else
_reentrant INT TunerDriverSetPresetStations( INT iPresetStation)
#endif
{
	INT iTuningStatus = TUNER_SUCCESS;

	if ((iPresetStation-1<NUMBER_OF_PRESETS)&&(iPresetStation>0))
	{
		g_astPresetStations[iPresetStation-1].Frequency = g_wCurrentFrequency;
		g_astPresetStations[iPresetStation-1].Level = 1;
		g_wCurrentPreset = iPresetStation;
        SysPostMessage(2,MENU_TUNER_TUNED);
	}
	else
		g_wFMTunerError.m_bPRESET_RANGE_ERROR=TRUE;
	return iTuningStatus;
}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextstfm1000":
    #pragma endasm
#endif

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          TunerDriverErasePresetStations
//
//   Type:          Function
//
//   Description:   Erase a preset station
//
//   Inputs:        iPresetStation
//
//   Outputs:       Error status
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
#ifdef FUNCLET
    #pragma asm
        org p,".stfm1000_SYS":
        FTunerDriverErasePresetStations:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet2 RSRC_FUNCLET_TUNER_DRIVER_ERASE_PRESET

        org p,"SYSFUNCLET_TUNER_DRIVER_ERASE_PRESET_P":
        dc      RSRC_FUNCLET_TUNER_DRIVER_ERASE_PRESET
    #pragma endasm
_reentrant INT TunerDriverErasePresetStations_Funclet( INT iPresetStation)
#else
_reentrant INT TunerDriverErasePresetStations( INT iPresetStation)
#endif
{
	INT iTuningStatus = TUNER_SUCCESS;

	if ((iPresetStation-1<NUMBER_OF_PRESETS)&&(iPresetStation>0))
	{
		g_astPresetStations[iPresetStation-1].Frequency = 0;
		g_astPresetStations[iPresetStation-1].Level = 0;
		g_wCurrentPreset = 0;
        SysPostMessage(2,MENU_TUNER_TUNED);
	}
	else
		g_wFMTunerError.m_bPRESET_RANGE_ERROR=TRUE;
	return iTuningStatus;
}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextstfm1000":
    #pragma endasm
#endif

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          TunerDriverSetToPresetStations
//
//   Type:          Function
//
//   Description:   Tune to a preset station
//
//   Inputs:        iPresetStation: A preset station number
//
//   Outputs:       return iTuningStatus
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
#ifdef FUNCLET
    #pragma asm
        org p,".stfm1000_SYS":
        FTunerDriverSetToPresetStations:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet2 RSRC_FUNCLET_TUNER_DRIVER_TUNE_TO_PRESET

        org p,"SYSFUNCLET_TUNER_DRIVER_TUNE_TO_PRESET_P":
        dc      RSRC_FUNCLET_TUNER_DRIVER_TUNE_TO_PRESET
    #pragma endasm
_reentrant INT TunerDriverSetToPresetStations_Funclet(INT iPresetStation)
#else
_reentrant INT TunerDriverSetToPresetStations(INT iPresetStation)
#endif
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
}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextstfm1000":
    #pragma endasm
#endif

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          TunerDriverSearchDown
//
//   Type:          Function
//
//   Description:   Search down to find a station
//
//   Inputs:        none
//
//   Outputs:       Error status
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
#ifdef FUNCLET
    #pragma asm
        org p,".stfm1000_SYS":
        FTunerDriverSearchDown:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet2 RSRC_FUNCLET_TUNER_DRIVER_SEARCH_DOWN

        org p,"SYSFUNCLET_TUNER_DRIVER_SEARCH_DOWN_P":
        dc      RSRC_FUNCLET_TUNER_DRIVER_SEARCH_DOWN
    #pragma endasm
_reentrant INT TunerDriverSearchDown_Funclet(void)
#else
_reentrant INT TunerDriverSearchDown(void)
#endif
{
#ifdef SD_USE_50KHZ_TUNING_GRID
    // step the search base to 100KHz
	s_wCurrentFMFrequency = 100* (int) ((s_wCurrentFMFrequency)/100);
#endif

	// Remember the origin of the searching
    s_wBeginSearchFMFrequency = s_wCurrentFMFrequency;
    s_bTunerSearchHitBandLimit = FALSE;

    // fake out the tuner search
    if(s_iFMTunerSearchStatus != NOT_SEARCHING)
    {
        return TUNER_SUCCESS;
    }
    TunerTuningInit();
#ifdef FM_USA_REGION
    // Skip the first even station if we start from 108MHz -stmp9292
    if(s_wBeginSearchFMFrequency == FM_BAND_LIMIT_HIGH_FREQ)
    {
        s_bSearchFromTopBand = TRUE;
    }
#endif
    // if frequency is at the bottom of the band
	if ((s_wCurrentFMFrequency <= FM_BAND_LIMIT_LOW_FREQ)&&(g_bSearchEndAtBandLimit==FALSE))
	{
		s_wCurrentFMFrequency = FM_SEARCH_HIGH_LIMIT;
    	s_bTunerSearchHitBandLimit=TRUE;

	}

    g_wFMTunerError.m_bSEARCHING_UP = FALSE;
	g_wFMTunerError.m_bSEARCHING_DOWN = TRUE;
	s_iFMTunerSearchStatus = SEARCHING_DOWN;
     // Turn off DRI before change station
    STFM1000DriOff();

    //STFM1000DataPathOff();
	if (s_wCurrentFMFrequency > FM_BAND_LIMIT_LOW_FREQ)
	{
  		STFM1000QuickTune((s_wCurrentFMFrequency -= FM_FREQ_STEP));
	}
	else
    {
		STFM1000QuickTune(s_wCurrentFMFrequency);
    }

#ifdef SD_USE_50KHZ_TUNING_GRID
    s_wDisplayFMFrequency = s_wCurrentFMFrequency;
#endif
    return E_TUNER_BUSY;

}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextstfm1000":
    #pragma endasm
#endif




#ifdef DEBUG_MONITOR_SIGNAL_QUALITY
void LogDebugFunction(WORD wFunctionIndex)
{
#if 0

            wMonitorQualityIndex %= STFM1000_MONITOR_SIGNAL_LOG_MAX;
            stSTFM100MonitorQualityLogArrary[wMonitorQualityIndex].stLogFirstWord.wIsPolitPreset = StfmTuneData.sd_ctl_PilotPresent_b;
            stSTFM100MonitorQualityLogArrary[wMonitorQualityIndex].stLogFirstWord.wMonitorQualityCurrentState = s_eMonitorSignalQualityState;
            stSTFM100MonitorQualityLogArrary[wMonitorQualityIndex].stLogFirstWord.wOptimizedChannelCurrentState = s_eOptimizeChannelState;
            stSTFM100MonitorQualityLogArrary[wMonitorQualityIndex].stLogFirstWord.wFunctionIndex = wFunctionIndex;
            stSTFM100MonitorQualityLogArrary[wMonitorQualityIndex].stLogFirstWord.wWeakSignal = bIsWeakSignal;
            stSTFM100MonitorQualityLogArrary[wMonitorQualityIndex].wAgcOut = s_wAgcOut;
            stSTFM100MonitorQualityLogArrary[wMonitorQualityIndex].wLnaRms = s_wLnaRms;
            stSTFM100MonitorQualityLogArrary[wMonitorQualityIndex].wLnaTh = s_wTrackTableIndex;//s_wLnaTh;
            stSTFM100MonitorQualityLogArrary[wMonitorQualityIndex].wRssiValue = s_iFMSignalStrengthRSSI;
            wMonitorQualityIndex++;
#else

            wMonitorQualityIndex %= STFM1000_MONITOR_SIGNAL_LOG_MAX;

            stSTFM100MonitorQualityLogArrary[wMonitorQualityIndex].wRssiValue = s_iFMSignalStrengthRSSI;
            stSTFM100MonitorQualityLogArrary[wMonitorQualityIndex].wMpxAdc = s_wMpxAdc;
            stSTFM100MonitorQualityLogArrary[wMonitorQualityIndex].bIsStation = StfmTuneData.sd_ctl_ShadowTune1_dw;//s_wLnaTh;
            stSTFM100MonitorQualityLogArrary[wMonitorQualityIndex].wFreq = s_wCurrentFMFrequency;//s_wLnaTh;
            stSTFM100MonitorQualityLogArrary[wMonitorQualityIndex].wModulo = StfmTuneData.sd_ctl_ShadowSdnominal_dw;
            wMonitorQualityIndex++;
#endif

}
#endif


// ****************************************
//! OPTIMIZE CHANNEL
// ****************************************
#ifdef FUNCLET
    #pragma asm
        org p,".stfm1000_SYS":
        FSTFM1000OptimizeChannel:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet2 RSRC_FUNCLET_STFM1000_TUNER_OPTIMIZE_CHANNEL

        org p,"SYSFUNCLET_STFM1000_TUNER_OPTIMIZE_CHANNEL_P":
        dc      RSRC_FUNCLET_STFM1000_TUNER_OPTIMIZE_CHANNEL
    #pragma endasm
_reentrant INT STFM1000OptimizeChannel_Funclet(void)
#else
_reentrant INT STFM1000OptimizeChannel(void)
#endif
{
    INT iRetCode = SUCCESS;
    BOOL bPilotPresent = FALSE;
    STFM1000Tune_t * pDrst = &StfmTuneData;

    switch(s_eOptimizeChannelState)
    {
        case OPTIMIZE_CHANNEL_STATE_START:
            iRetCode = STFM1000OptimizeChannelStart();
            break;
        case OPTIMIZE_CHANNEL_STATE_PROCESSING_1:
            STFM1000GetNearChannelOffset();
            if ( pDrst->sd_ctl_ShadowSignalQuality_w < pDrst->sd_ctl_AdjChanTh_w)
            {
                iRetCode = STFM1000SetNearChannelOffset(5);

                g_iTunerProcessIntervalMs = 10;
                s_eOptimizeChannelState = OPTIMIZE_CHANNEL_STATE_PROCESSING_2;
                iRetCode = E_TUNER_BUSY;
            }
            else
            {
                iRetCode = STFM1000SetNearChannelFilter(0);
                iRetCode = STFM1000LookForPilot(&bPilotPresent);
                if(bPilotPresent)
                {
                    s_wTrackTableIndex = 0;

                    iRetCode = STFM1000GearShiftPtGear(STFM1000TrackTable[s_wTrackTableIndex].dw_bits);

                    g_iTunerProcessIntervalMs = STFM1000TrackTable[s_wTrackTableIndex].dw_msec_delay;
                    s_eOptimizeChannelState = OPTIMIZE_CHANNEL_STATE_PROCESSING_3;
                    iRetCode =  E_TUNER_BUSY;
                }
                else
                {
                    iRetCode = STFM1000OptimizeChannelEnd();
                }

            }
            break;
       case OPTIMIZE_CHANNEL_STATE_PROCESSING_2:
            STFM1000GetNearChannelOffset();

            if ( pDrst->sd_ctl_ShadowSignalQuality_w < pDrst->sd_ctl_AdjChanTh_w)
            {
                iRetCode = STFM1000SetNearChannelFilter(2);
            }
            else
            {
                iRetCode = STFM1000SetNearChannelFilter(0);
            }

            iRetCode = STFM1000LookForPilot(&bPilotPresent);
            if(bPilotPresent)
            {
                s_wTrackTableIndex = 0;
                iRetCode = STFM1000GearShiftPtGear(STFM1000TrackTable[s_wTrackTableIndex].dw_bits);

                g_iTunerProcessIntervalMs = STFM1000TrackTable[s_wTrackTableIndex].dw_msec_delay;
                s_eOptimizeChannelState = OPTIMIZE_CHANNEL_STATE_PROCESSING_3;
                iRetCode =  E_TUNER_BUSY;
            }
            else
            {
                iRetCode = STFM1000OptimizeChannelEnd();
            }
            break;
        case OPTIMIZE_CHANNEL_STATE_PROCESSING_3:
            s_wTrackTableIndex++;
            if ( /*(0 != STFM1000TrackTable[s_wTrackTableIndex].dw_msec_delay)
                 || */(0 != STFM1000TrackTable[s_wTrackTableIndex].dw_bits) )
            {
                iRetCode = STFM1000GearShiftPtGear(STFM1000TrackTable[s_wTrackTableIndex].dw_bits);

                if(STFM1000TrackTable[s_wTrackTableIndex].dw_msec_delay == 0)
                {
                   iRetCode = STFM1000OptimizeChannelEnd();
                }
                else
                {
                    g_iTunerProcessIntervalMs = STFM1000TrackTable[s_wTrackTableIndex].dw_msec_delay;
                    s_eOptimizeChannelState = OPTIMIZE_CHANNEL_STATE_PROCESSING_3;
                    iRetCode =  E_TUNER_BUSY;
                }
            }
            break;
        default:
            break;
     }
    return iRetCode;
}
_reentrant INT STFM1000OptimizeChannelStart(void)
{
    INT RetCode = SUCCESS;
    BOOL bPilotPresent = FALSE;

    RetCode = STFM1000WriteRegister(
        DATAPATH_REG_ADDR,
        DB_ACCEPT_REG_MASK,
        &StfmTuneData.sd_ctl_ShadowDataPath_dw,
        1);

    if (SUCCESS != RetCode)
    {
        //SystemHalt();
        return RetCode;
    }

    RetCode = STFM1000WriteRegister(
        PILOTTRACKING_REG_ADDR,
        WHOLEREG_MASK,
        &StfmTuneData.sd_ctl_ShadowPilotTracking_dw,
        0x81B6);

    if (SUCCESS != RetCode)
    {
        //SystemHalt();
        return RetCode;
    }

    // call SetChannelFilter
    STFM1000SetNearChannelOffset(1);
    g_iTunerProcessIntervalMs = 10;
    s_eOptimizeChannelState = OPTIMIZE_CHANNEL_STATE_PROCESSING_1;
    return E_TUNER_BUSY;
}
_reentrant INT STFM1000OptimizeChannelEnd(void)
{
    INT RetCode = SUCCESS;

    RetCode = STFM1000WriteRegister(
        DATAPATH_REG_ADDR,
        DB_ACCEPT_REG_MASK,
        &StfmTuneData.sd_ctl_ShadowDataPath_dw,
        0);

    if (SUCCESS != RetCode)
    {
        //SystemHalt();
        return RetCode;
    }
    s_eOptimizeChannelState = OPTIMIZE_CHANNEL_STATE_COMPLETE;
    RetCode = STFM1000DriOn();
    if (SUCCESS != RetCode)
    {
        //SystemHalt();
        return RetCode;
    }
    SysPostMessage(2,MENU_TUNER_TUNED);
    //g_wFMTunerError.m_bTUNER_BUSY = FALSE;

    return RetCode;
}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextstfm1000":
    #pragma endasm
#endif






////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          TunerDriverScanStations
//
//   Type:          Function
//
//   Description:   Scan thw whole Fm bandwidth to find  NUMBER_OF_PRESETS strongest
//                  stations as preset stations
//
//   Inputs:        none
//
//   Outputs:       Error status
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
#ifdef FUNCLET
    #pragma asm
        org p,".stfm1000_SYS":
        FTunerDriverScanStations:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet2 RSRC_FUNCLET_TUNER_DRIVER_SCAN_STATION

        org p,"SYSFUNCLET_TUNER_DRIVER_SCAN_STATION_P":
        dc      RSRC_FUNCLET_TUNER_DRIVER_SCAN_STATION
    #pragma endasm
_reentrant INT TunerDriverScanStations_Funclet(void)
#else
_reentrant INT TunerDriverScanStations(void)
#endif
{
	INT	i;

/* initialize the preset structure */
	for (i=0; i<NUMBER_OF_PRESETS; i++)
	{
		g_astPresetStations[i].Frequency = 0;
		g_astPresetStations[i].Level = 0;
	}
/* set the searching */
// initialize the g_wFMTunerError
    g_wFMTunerError.m_bTUNER_BUSY= TRUE;
	g_wFMTunerError.m_bSEARCHING_UP=FALSE;
	g_wFMTunerError.m_bSEARCHING_DOWN=FALSE;
	g_wFMTunerError.m_bSCANNING_STATION = TRUE;
	g_wFMTunerError.m_bBAND_LIMIT_HIGH_ERROR=FALSE;
	g_wFMTunerError.m_bBAND_LIMIT_LOW_ERROR=FALSE;
	g_wFMTunerError.m_bUNUSED_PRESET=FALSE;
	g_wFMTunerError.m_bPRESET_RANGE_ERROR=FALSE;
    // Avoid to handle any message
	g_wFMTunerError.m_bPOWERED_OFF = TRUE;
    s_iFMPresetCount = 0;
	g_wFMTunerError.m_bSCANNING_STATION = TRUE;
	s_wCurrentFMFrequency = FM_BAND_LIMIT_LOW_FREQ-FM_TUNER_GRID;
    //g_bSearchEndAtBandLimit = TRUE;
    /*******************************************************
     * Using TunerDriverSearchUp function
     * If find a station, update display
     * In the TunerGetStatus function
     * If it scanning, scanning the entire bandwidth
     * If find more than NUMBER_OF_PRESETS,
     * Update the weeker station with the stronger station
     * until search the entire bandwidth
     *******************************************************/
	return TunerDriverSearchUp();
}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextstfm1000":
    #pragma endasm
#endif


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          TunerDriverSetMonoStereoMode
//
//   Type:          Function
//
//   Description:   Set tuner stereo/mono mode
//
//   Inputs:        iMode: TUNER_MODE_STEREO or TUNER_MODE_MONO
//
//   Outputs:       return iTuningStatus
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
#ifdef FUNCLET
    #pragma asm
        org p,".stfm1000_SYS":
        FTunerDriverSetMonoStereoMode:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet2 RSRC_FUNCLET_TUNER_DRIVER_SET_MONO_STEREO_MODE

        org p,"SYSFUNCLET_TUNER_DRIVER_SET_MONO_STEREO_MODE_P":
        dc      RSRC_FUNCLET_TUNER_DRIVER_SET_MONO_STEREO_MODE
    #pragma endasm
_reentrant INT TunerDriverSetMonoStereoMode_Funclet(INT iMode )
#else
_reentrant INT TunerDriverSetMonoStereoMode(INT iMode )
#endif
{
    INT iTuningStatus=TUNER_SUCCESS;
    if(iMode == TUNER_MODE_STEREO)
	{
        /* enable "forced mono" in black box */
#ifndef MIN_FILTER
        g_sd_audio_pCoefForcedMono_u1 = 0;
#else
        sd_audio_pCoefForcedMono_u1 = 0;
#endif
        // Set stereo bit
        g_STFM1000_CustomerForceMono = FALSE;  // STMP00013255
	}
    else if (iMode == TUNER_MODE_MONO)
	{
        /* enable "forced mono" in black box */
#ifndef MIN_FILTER
        g_sd_audio_pCoefForcedMono_u1 = 1;
#else
        sd_audio_pCoefForcedMono_u1 = 1;
#endif
        // Set Mono bit
        g_STFM1000_CustomerForceMono = TRUE;  // STMP00013255
	}
    else
        iTuningStatus = E_TUNER_PARAMETER_IN_ERROR;
    return iTuningStatus;
}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextstfm1000":
    #pragma endasm
#endif

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          TunerDriverSetSensitivity
//
//   Type:          Function
//
//   Description:   Set tuner sensitivity, i.e.RSSI after decoded, the value is 0-70
//
//   Inputs:        iSensitivity: sensitivity level from 0-100
//
//   Outputs:       return iTuningStatus
//
//   Notes:         Func not in 3.120. Does nothing anyway. maybe remove it to save ram? any callers?
//<
////////////////////////////////////////////////////////////////////////////////
#if 0    //This function is moved into tunermodule.c
_reentrant INT TunerDriverSetSensitivity(INT iSensitivity )
{
    //s_iFMTunerSensitivity =  (iSensitivity*70)/100;
    //StfmTuneData.sd_ctl_TuneRssiTh_w = s_iFMTunerSensitivity;
    return TUNER_SUCCESS;
}
#endif

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          TunerDriverGetTunerstate
//
//   Type:          Function
//
//   Description:   Get tuner status such as frequency, signal strength, stereo/mono
//
//   Inputs:        none
//
//   Outputs:       return iTuningStatus
//
//   Notes:         This function is called every 100ms when staying in a station
//                  to pass the RSSI raw data to c-filter, and the monitor signal quality
//                  should be down every 1 second
//<
////////////////////////////////////////////////////////////////////////////////
extern WORD	_X g_wTunedFrequency;   //
extern BOOL	g_bStereoBroadcast;

_reentrant INT TunerDriverGetTunerstate( void )
{
    INT iTuningStatus = SUCCESS;
    BOOL bIsStation;
    BOOL bNeedDisplay = FALSE;

    // Frequency
#ifdef SD_USE_50KHZ_TUNING_GRID
    g_wCurrentFrequency = s_wDisplayFMFrequency;
#else
    g_wCurrentFrequency = s_wCurrentFMFrequency;
#endif

#ifdef DEBUG_MONITOR_SIGNAL_QUALITY
     wLogTimeDelta = SysGetCurrentTime() - wLogTimeDelta;
     wLogIndex %= STFM1000_MONITOR_SIGNAL_LOG_MAX;
     wTimeLogArray[wLogIndex] = wLogTimeDelta;
     wLogIndex++;
     wLogTimeDelta = SysGetCurrentTime();;
#endif
    // Update RSSI
    iTuningStatus = STFM1000TunerQueryIsStation(&bIsStation);
    if (SUCCESS != iTuningStatus)
    {
        //SystemHalt();
        return iTuningStatus;
    }
    // Mask the high 8 bit since the RSSI is in the lower 16-bit
    g_wRawRSSIData &= 0x0000FFFF;
#ifndef MIN_FILTER
    // Call the function to calc the c-filter coefficiencts
    sw_stereocontrol_xcall((unsigned short)g_wRawRSSIData);
    sw_stereocontrol_bwsweep();
    // Update this from a global in C-filter
    // Mono/stereo
	g_bTunedStereoStation = (!g_sd_audio_pCoefForcedMono_u1 );
#else
    g_bTunedStereoStation = (!sd_audio_pCoefForcedMono_u1);
#endif
    // If the channel optimized is not settled, don't need to monitor the signal quality
    if(s_eOptimizeChannelState != OPTIMIZE_CHANNEL_STATE_COMPLETE)
    {
        wCountForTriggerMonitorQuality = 0;
    }

    if(wCountForTriggerMonitorQuality++ >= 10)
    {
        wCountForTriggerMonitorQuality = 0;
        //We have a pending state, restart
        //if (s_eMonitorSignalQualityState != MONITOR_SIGNAL_QUALITY_STATE_COMPLETE)

        s_eMonitorSignalQualityState = MONITOR_SIGNAL_QUALITY_STATE_START;

        // Do we need to check if there is a station then do the signal quality monitor
        // If the frequency change, then  bIsWeakSignal = FALSE;
        //
        // We like to keep the return value, it could be E_TUNER_BUSY
#ifdef STFM1000_ENABLE_MONITOR_SIGNAL_QUALITY
        iTuningStatus = STFM1000TunerMonitorQuality();
#endif

    }
    // Don't disable the timer if the optimized channel and monitor signal quality are not done
    // since it would take longer than 100ms
    if((s_eMonitorSignalQualityState != MONITOR_SIGNAL_QUALITY_STATE_COMPLETE) ||
       (s_eOptimizeChannelState != OPTIMIZE_CHANNEL_STATE_COMPLETE))
    {
        iTuningStatus = E_TUNER_BUSY;
    }

    if( g_bStereoBroadcast != g_bTunedStereoStation )
    {
        g_bStereoBroadcast = g_bTunedStereoStation;
        bNeedDisplay = TRUE;
    }

    if (g_wTunedFrequency != FM_TUNER_GRID*((g_wCurrentFrequency+FM_TUNER_GRID/2)/FM_TUNER_GRID))
    {
        g_wTunedFrequency = FM_TUNER_GRID*((g_wCurrentFrequency+FM_TUNER_GRID/2)/FM_TUNER_GRID);
        bNeedDisplay = TRUE;
    }

    if( bNeedDisplay == TRUE )
    {
        SysPostMessage( 3, MENU_TUNER_REFRESH_DISPLAY, 1<<DISPLAY_FMTUNER_FREQUENCY );
    }

	return iTuningStatus;
}
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          bIsTunerModulePowered
//
//   Type:          Function
//
//   Description:   Check if tuning is on or off
//
//   Inputs:        none
//
//   Outputs:       TRUE: power-on/FALSE: power-off
//
//    Notes: HJ deleted this from 3.120. still here in 2.620
//<
////////////////////////////////////////////////////////////////////////////////
#if 0    // This function is moved into  tunermodule.c
_reentrant BOOL bIsTunerModulePowered(void)
{
    return ( !(g_wFMTunerError.m_bPOWERED_OFF));
}
#endif

////////////////////////////////////////////////////////////////////////////////
//

//>  Name:          STFM1000ContinueSearching
//
//   Type:          Function
//
//   Description:   Search until complete
//
//   Inputs:        none
//
//   Outputs:       Busy or SUCCESS
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
#ifdef FUNCLET
    #pragma asm
        FSTFM1000ContinueSearching:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet2 RSRC_FUNCLET_STFM1000_CONTINUE_SEARCHING

        org p,"SYSFUNCLET_STFM1000_CONTINUE_SEARCHING_P":
        dc      RSRC_FUNCLET_STFM1000_CONTINUE_SEARCHING
    #pragma endasm
_reentrant INT STFM1000ContinueSearching_Funclet(void)
#else
_reentrant INT STFM1000ContinueSearching( void )
#endif
{
#define RSSI_READ_COUNT 5
	INT iTuningStatus;
#ifdef SD_USE_200KHZ_TUNING_GRID
    WORD wFreqModulo = 0;
#endif
    INT iRssiTemp = 0;
    WORD wLoopCount = RSSI_READ_COUNT;
#ifdef SD_USE_50KHZ_TUNING_GRID
    INT iMpxAdcTemp = 0;
#endif
    BOOL bIsStation = FALSE;
    INT iTemp = 0;

	if (((s_wCurrentFMFrequency >=s_wBeginSearchFMFrequency)&&(s_bTunerSearchHitBandLimit==TRUE)&&(s_iFMTunerSearchStatus>0))||
		((s_wCurrentFMFrequency <=s_wBeginSearchFMFrequency)&&(s_bTunerSearchHitBandLimit==TRUE)&&(s_iFMTunerSearchStatus<0)))
	{

		iTuningStatus = STFM1000TunerSearchEnded(s_wBeginSearchFMFrequency);
	}
	else
	{
        STFM1000TunerQueryIsStation(&bIsStation);
#ifdef SD_USE_200KHZ_TUNING_GRID
        wFreqModulo = s_wCurrentFMFrequency % 200;
        if(bIsStation)
        {
            if(wFreqModulo != 0)
            {
                return(STFM1000TunerSearchEnded(s_wCurrentFMFrequency));
            }
        }
        iTuningStatus = STFM1000NextSearchFrequency();
#else

        iRssiTemp = s_iFMSignalStrengthRSSI;
#ifdef SD_USE_50KHZ_TUNING_GRID
        iMpxAdcTemp = s_iFMSignalStrengthMpxDc;
#endif
        if(bIsStation)
        {
            while((--wLoopCount > 0))
            {
                STFM1000TunerQueryIsStation(&bIsStation);
                iRssiTemp += s_iFMSignalStrengthRSSI;
#ifdef SD_USE_50KHZ_TUNING_GRID
                iMpxAdcTemp = (iMpxAdcTemp + s_iFMSignalStrengthMpxDc)/2;
#endif
            }
            iRssiTemp /= RSSI_READ_COUNT;
        }
        s_iFMSignalStrengthRSSI = iRssiTemp;
#ifdef SD_USE_50KHZ_TUNING_GRID
        s_iFMSignalStrengthMpxDc = iMpxAdcTemp;
#endif

        // First time do a search
        if(bIsFirstTimeToNewStation)
        {
            if(bIsStation)
            {
#ifdef SD_USE_50KHZ_TUNING_GRID
                if(s_wCurrentFMFrequency > g_astSearchStations.Frequency)
                {
                    iTemp = s_wCurrentFMFrequency - g_astSearchStations.Frequency;
                }
                else
                {
                    iTemp = g_astSearchStations.Frequency - s_wCurrentFMFrequency;
                }
                g_astSearchStations.Frequency = s_wCurrentFMFrequency;
                g_astSearchStations.Level = s_iFMSignalStrengthRSSI;

                g_astSearchStations.MpxAdc = s_iFMSignalStrengthMpxDc;

                if(iTemp != 100)
                {
                    bIsFirstTimeToNewStation = FALSE;
                }
#else
                bIsFirstTimeToNewStation = FALSE;
                g_astSearchStations.Frequency = s_wCurrentFMFrequency;
                g_astSearchStations.Level = s_iFMSignalStrengthRSSI;
#endif
            }
            iTuningStatus = STFM1000NextSearchFrequency();
        }
        else
        {
#ifdef SD_USE_50KHZ_TUNING_GRID
            if(bIsStation)
            {

                if((s_iFMSignalStrengthRSSI) > (g_astSearchStations.Level))
                {
                    iTuningStatus = STFM1000TunerSearchEnded(s_wCurrentFMFrequency);
                    g_astSearchStations.Frequency = s_wCurrentFMFrequency;
                    g_astSearchStations.Level = s_iFMSignalStrengthRSSI;
                    g_astSearchStations.MpxAdc = s_iFMSignalStrengthMpxDc;

                }
                // Back to the one previous found
                else
                {
                    iTuningStatus = STFM1000TunerSearchEnded(g_astSearchStations.Frequency);
                    s_iFMSignalStrengthRSSI = g_astSearchStations.Level;
                }
            }
            // Back to the one prevoius found
            else
            {
               iTuningStatus = STFM1000TunerSearchEnded(g_astSearchStations.Frequency);
               s_iFMSignalStrengthRSSI = g_astSearchStations.Level;
            }

            if(g_astSearchStations.MpxAdc > 300)
            {
                s_wDisplayFMFrequency = s_wCurrentFMFrequency + 50;
            }
            else if(g_astSearchStations.MpxAdc < -300)
            {
                s_wDisplayFMFrequency = s_wCurrentFMFrequency - 50;
            }
            else
            {
                s_wDisplayFMFrequency = s_wCurrentFMFrequency;
            }
#else
            if(bIsStation)
            {
                if((g_astSearchStations.Level > s_iFMSignalStrengthRSSI))
                {
                    iTuningStatus = STFM1000TunerSearchEnded(g_astSearchStations.Frequency);
                    s_iFMSignalStrengthRSSI = g_astSearchStations.Level;
                }
                else
                {
                    g_astSearchStations.Frequency = s_wCurrentFMFrequency;
                    g_astSearchStations.Level = s_iFMSignalStrengthRSSI;
                    iTuningStatus = STFM1000NextSearchFrequency();   // not sure if it's a station
                }
            }
            // Back to the one prevoius found
            else
            {
               iTuningStatus = STFM1000TunerSearchEnded(g_astSearchStations.Frequency);
               s_iFMSignalStrengthRSSI = g_astSearchStations.Level;
            }

#endif

         }
#endif
	}
	return iTuningStatus;
}

#ifdef FUNCLET
    #pragma asm
        org p,".ptextstfm1000":
    #pragma endasm
#endif

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          STFM1000NextSearchFrequency
//
//   Type:          Function
//
//   Description:   Tune to next frequency
//
//   Inputs:        None
//
//   Outputs:       Busy or SUCCESS
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
_reentrant INT STFM1000NextSearchFrequency( void )
{

	//if ((s_wCurrentFMFrequency > FM_BAND_LIMIT_LOW_FREQ) && (s_wCurrentFMFrequency < FM_BAND_LIMIT_HIGH_FREQ))
	if ((s_wCurrentFMFrequency >= FM_BAND_LIMIT_LOW_FREQ) && (s_wCurrentFMFrequency <= FM_BAND_LIMIT_HIGH_FREQ))
    {
        s_wCurrentFMFrequency+=FM_FREQ_STEP*s_iFMTunerSearchStatus;

		STFM1000TunerQuicksetStation(s_wCurrentFMFrequency);
    }
	else
	{

		if (g_bSearchEndAtBandLimit==TRUE)
		{
			if(s_iFMTunerSearchStatus > 0)
            {
				g_wFMTunerError.m_bBAND_LIMIT_HIGH_ERROR = TRUE;
            }
			else
            {
				g_wFMTunerError.m_bBAND_LIMIT_LOW_ERROR = TRUE;
            }
			STFM1000TunerSearchEnded(s_wCurrentFMFrequency);
		}
		else
		{
			s_bTunerSearchHitBandLimit=TRUE;
			if(s_iFMTunerSearchStatus>0)
            {
				//s_wCurrentFMFrequency = FM_BAND_LIMIT_LOW_FREQ + FM_FREQ_STEP;
				s_wCurrentFMFrequency = FM_BAND_LIMIT_LOW_FREQ;
            }
			else
            {
				//s_wCurrentFMFrequency = FM_BAND_LIMIT_HIGH_FREQ - FM_FREQ_STEP;
				s_wCurrentFMFrequency = FM_BAND_LIMIT_HIGH_FREQ;
            }

			STFM1000TunerQuicksetStation(s_wCurrentFMFrequency);
		}
	}

	return E_TUNER_BUSY;
}


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          STFM1000EndAutoScanning
//
//   Type:          Function
//
//   Description:   Scan complete
//
//   Inputs:        None
//
//   Outputs:       Busy or SUCCESS
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
_reentrant INT STFM1000EndAutoScanning( void )
{
	WORD wPresetFrequency;
	if ((wPresetFrequency = g_astPresetStations[0].Frequency)==0)
    {
		wPresetFrequency = FM_BAND_LIMIT_LOW_FREQ;
    }
	g_wFMTunerError.m_bSCANNING_STATION = FALSE;	 // end preset scanning
	g_wCurrentPreset = 1;
    // So can hanle other messages
	g_wFMTunerError.m_bPOWERED_OFF = FALSE;

	return TunerDriverTuneToFrequency(wPresetFrequency);
}
////////////////////////////////////////////////////////////////////////////////
//!
//! \brief      STFM tuner tuner lock sequence
//! \fntype     Function
//!
//! Function will perform the following steps:
//!     TBD when Generic Level implemented [get rssi into Settings Table
//!         (then NotifyApi())]
//!     get stereo status (then NotifyApi())
//!     SD_DP_On (Turn on data path on chip)
//!     DRI_On (Start DRI flow)
//!     Start DRI stream reception (ddi_dri_StartStream())
//!     unmutes tuner audio output
//!     enables tuner alert source (interrupt) - not relevant here since alert
//!          carried by data flow which is turning on.
//!     Optimize Station (details TBD from STFM1000 HW group following empirical
//!          research).
//!
//! \param[in]  pTunerObject     Pointer to Tuner Object.
//!
//! \return     SUCCESS/ERROR.
//!
////////////////////////////////////////////////////////////////////////////////
_reentrant INT STFM1000TunerLockStation(void)
{
    INT RetCode = SUCCESS;
    STFM1000Tune_t * pDrst = &StfmTuneData;
    // * get stereo status (then NotifyApi())
    //! Note on setting stereo status:  The stereo / mono status is determined
    //! in the filter function when it's processing the signal.  This variable
    //! must be set from within the filter.  TBD
#if 0
    // * Turn on data path on chip
    RetCode = STFM1000DataPathOn();
//    if (SUCCESS != RetCode)
//    {
//        SystemHalt();
//        return RetCode;
//    }
#endif
    // * Optimize Station
    RetCode = STFM1000OptimizeChannel();
//    if (SUCCESS != RetCode)
//    {
//        SystemHalt();
//        return RetCode;
//    }
#if 0
    // * Starts DRI transmission from tuner (tspec_v09u.pdf::DRI_on)
    RetCode = STFM1000DriOn();
    if (SUCCESS != RetCode)
    {
        //SystemHalt();
        return RetCode;
    }
#endif
    return E_TUNER_BUSY;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief      Tune to a  new frequency value
//! \fntype     Function
//!
//! Function will perform the following steps:
//!     Determine the front end frequency to tune to
//!     Reconfigure TUNE1 & SDMNOMINAL registers to tune to new frequency
//!
//! \param[in]  wTuneFreqIn     new tuning frequency to set (in KHz)
//!
//! \return     SUCCESS/ERROR
//!
////////////////////////////////////////////////////////////////////////////////
_reentrant INT STFM1000QuickTune(WORD wTuneFreqIn)
{
#define TCA   7240   // 72.4*100
#define TCB   7      // 0.07*100
#define CORNER_FREQUENCY 1034  // 7240/7
    WORD wTuneCap;
    INT RetCode;
    DWORD dwControlSelect;
    WORD wTuneFreq100kHzTemp;
#ifndef STFM1000_PLL_TABLE
    // Following variables used only to construct tables
    LONG ndiv;           // N Divider in PLL
    LONG incr;           // Increment in PLL
    LONG cicosr;         // CIC oversampling ratio
    LONG sdnominal;      // value to serve pilot/interpolator loop in SD
    LONG temp;        // used in tuning table construction
#ifdef OPTIMIZED_TUNE1_SDNOMINAL_CAL
    Fract_type FractTemp;
#endif
#endif

    STFM1000Tune_t * pDrst = &StfmTuneData;
    WORD wTuneFreq100kHz = wTuneFreqIn / 100;

    // We assume that mix_reg is as defined in the steely dan memory map (and
    // then right-justified)
    // NOTE:  This is the default value to which mix_reg was initialized.
    // If it later will change dynamically, read it from the SD register now.
    WORD mix_reg = TUNE1_B2_MIX_REG;
    //s_wCurrentFMFrequency = wTuneFreqIn;
       // reset the value
    wSd_ctl_Confidence_u16 = 3;

    wTuneFreq100kHzTemp = wTuneFreq100kHz;
    if (0 == mix_reg)
    // Need change sd_ctl_IfFreq_i back to INT if confirmed from Aarom
    //???????????????????????
    //??????????????????????
        pDrst->sd_ctl_IfFreq_i = -2;
    else if (1 == mix_reg)
        pDrst->sd_ctl_IfFreq_i = -1;
    else if (2 == mix_reg)
        pDrst->sd_ctl_IfFreq_i = 0;
    else if (3 == mix_reg)
        pDrst->sd_ctl_IfFreq_i = 1;
    else if (4 == mix_reg)
        pDrst->sd_ctl_IfFreq_i = 2;


    switch(wTuneFreq100kHz)
    {

#ifdef FM_JAPAN_REGION
// **************************************************
// Japan Band Alternate Tunings to avoid RF spurs
// **************************************************
    case 840:	      // change ZIF to 0Hz
        pDrst->sd_ctl_Tune1Data_dw = 0x84680;
        pDrst->sd_ctl_SdnominalData_dw = 0x1C11F704;
        break;

    case 885:		// change ZIF to +0kHz
        pDrst->sd_ctl_Tune1Data_dw = 0x84AB0;
        pDrst->sd_ctl_SdnominalData_dw = 0x1BF9B021;
        break;

    case 899:		// change ZIF to +100kHz
        pDrst->sd_ctl_Tune1Data_dw = 0xC4CC0;
        pDrst->sd_ctl_SdnominalData_dw = 0x1BB369A9;
        break;

// **************************************************
// End - Japan Band Alternate Tunings to avoid RF spurs
// **************************************************

#else  // must be US Band
// **************************************************
// US Band Alternate Tunings to avoid RF spurs
// **************************************************

    case 885:		// change ZIF to +0kHz
        pDrst->sd_ctl_Tune1Data_dw = 0x84AB0;
        pDrst->sd_ctl_SdnominalData_dw = 0x1BF9B021;
        break;

    case 899:		// change ZIF to +100kHz
        pDrst->sd_ctl_Tune1Data_dw = 0xC4CC0;
        pDrst->sd_ctl_SdnominalData_dw = 0x1BB369A9;
        break;

    case 900:		// change ZIF to 0kHz
        pDrst->sd_ctl_Tune1Data_dw = 0x84CC0;
        pDrst->sd_ctl_SdnominalData_dw = 0x1BB369A9;
        break;

    case 915:		// change ZIF to +0kHz
        pDrst->sd_ctl_Tune1Data_dw = 0x84CD0;
        pDrst->sd_ctl_SdnominalData_dw = 0x1C299A5B;
        break;

    case 930:		// change ZIF to +0kHz
        pDrst->sd_ctl_Tune1Data_dw = 0x84ee0;
        pDrst->sd_ctl_SdnominalData_dw = 0x1be3e6aa;
        break;

    case 945:		// change ZIF to +0kHz
        pDrst->sd_ctl_Tune1Data_dw = 0x84ef0;
        pDrst->sd_ctl_SdnominalData_dw = 0x1c570f8b;
        break;

    case 959:		// change ZIF to +100kHz
        pDrst->sd_ctl_Tune1Data_dw = 0xC5100;
        pDrst->sd_ctl_SdnominalData_dw = 0x1C11F704;
        break;

    case 960:	      // change ZIF to 0Hz
        pDrst->sd_ctl_Tune1Data_dw = 0x85100;
        pDrst->sd_ctl_SdnominalData_dw = 0x1C11F704;
        break;

    case 975:		// change ZIF to +0kHz
        pDrst->sd_ctl_Tune1Data_dw = 0x85310;
        pDrst->sd_ctl_SdnominalData_dw = 0x1bd03d57;
        break;

    case 990:		// change ZIF to +0kHz
        pDrst->sd_ctl_Tune1Data_dw = 0x85320;
        pDrst->sd_ctl_SdnominalData_dw = 0x1c3dc822;
        break;

    case 1005:		// change ZIF to +0kHz
        pDrst->sd_ctl_Tune1Data_dw = 0x85530;
        pDrst->sd_ctl_SdnominalData_dw = 0x1bfc93ff;
        break;

    case 1019:	     // change ZIF to +100kHz
        pDrst->sd_ctl_Tune1Data_dw = 0xC5740;
        pDrst->sd_ctl_SdnominalData_dw = 0x1BBE683C;
        break;

    case 1020:	     // change ZIF to +0kHz
        pDrst->sd_ctl_Tune1Data_dw = 0x85740;
        pDrst->sd_ctl_SdnominalData_dw = 0x1bbe683c;
        break;

    case 1035:		// change ZIF to +0kHz
        pDrst->sd_ctl_Tune1Data_dw = 0x85750;
        pDrst->sd_ctl_SdnominalData_dw = 0x1c26dab6;
        break;

    case 1050:		// change ZIF to +0kHz
        pDrst->sd_ctl_Tune1Data_dw = 0x85960;
        pDrst->sd_ctl_SdnominalData_dw = 0x1be922b4;
        break;

    case 1065:		// change ZIF to +0kHz
        pDrst->sd_ctl_Tune1Data_dw = 0x85970;
        pDrst->sd_ctl_SdnominalData_dw = 0x1c4f357c;
        break;

    case 1079:	// change ZIF to +100kHz
        pDrst->sd_ctl_Tune1Data_dw = 0xC5B80;
        pDrst->sd_ctl_SdnominalData_dw = 0x1C11F704;
        break;

    case 1080:	// change ZIF to 0Hz
        pDrst->sd_ctl_Tune1Data_dw = 0x85B80;
        pDrst->sd_ctl_SdnominalData_dw = 0x1C11F704;
        break;
// **************************************************
// End - US Band Alternate Tunings to avoid RF spurs
// **************************************************
#endif  // end #ifdef FM_JAPAN_REGION

    default:
#ifndef STFM1000_PLL_TABLE
        wTuneFreq100kHz = wTuneFreq100kHz + pDrst->sd_ctl_IfFreq_i;
#ifndef OPTIMIZED_TUNE1_SDNOMINAL_CAL
        // Don't try to optimize the calculation, the compile could
        // do something to give your incorrect results
        ndiv = (LONG)((wTuneFreq100kHz+14)/15) - 48;
        incr = (LONG)(wTuneFreq100kHz/15);
        incr = wTuneFreq100kHz - incr*15;
        cicosr = (LONG)(wTuneFreq100kHz*2/3.0/16.0 + 0.5);
        sdnominal =
            (LONG)(wTuneFreq100kHz*100.0e3/1.5/(LONG)cicosr/2.0/2.0*2.0*8.0*256.0/228.0e3*65536);
#else
        //      ndiv = (DWORD)((wTuneFreq100kHz + 14) / 15) - 48;
        FractTemp.I = 2048 * (wTuneFreq100kHz + 14) + 1;    // scale for rounding
        FractTemp.F = FractTemp.F * OneFifteenth;       // Multiply as _fract values
        ndiv = FractTemp.I >> 11;                       // unscale for rounding
        ndiv = ndiv - 48;

        //      incr = wTuneFreq100kHz - (DWORD)(wTuneFreq100kHz / 15) * 15;
        FractTemp.I = (2048 * wTuneFreq100kHz) + 1; // scale for rounding
        FractTemp.F = FractTemp.F * OneFifteenth;   // Multiply as _fract values
        temp = FractTemp.I >> 11;                   // unscale for rounding
        temp = temp * 15;
        incr = wTuneFreq100kHz - temp;

        //      cicosr = (DWORD)(wTuneFreq100kHz * 2 / 3.0 / 16.0 + 0.5);   // cicosr in range [32,45]
        FractTemp.I = 1024 * wTuneFreq100kHz;
        FractTemp.F = FractTemp.F * OneTwelfth;         // Multiply as _fract values
        cicosr = FractTemp.I;
        cicosr = cicosr + 1024;                         // Round to nearest 0.5
        cicosr = cicosr >> 11;                          // Unscale for rounding

        // sdnominal = (DWORD)(wTuneFreq100kHz * 100.0e3 / 1.5 /(DWORD)cicosr2 / 2.0 / 2.0 * 2.0 * 8.0 * 256.0 / 228.0e3 * 65536);
        sdnominal = wTuneFreq100kHz * FractCicosr[cicosr - 32]; // Array is pre-scaled for rounding
        sdnominal = sdnominal >> 12;                            // unscale for rounding
#endif
        temp = 0x00000000;                                    // clear
        temp = temp | ((cicosr<<9)  & TUNE1_CICOSR_REG_MASK);  // bits[14:9] 0x00007E00
        temp = temp | ((ndiv<<4)    & TUNE1_PLL_DIV_REG_MASK); // bits[8:4]  0x000001F0
        temp = temp | ((incr)       & 0x0000000F);              // bits[3:0]  0x0000000F
        pDrst->sd_ctl_Tune1Data_dw = ((temp) & 0x000000007FFF) | 0x40000;//(TableTune1_u32[FeFreq_u32] & 0x00007FFF); // Bits [14:0]
        pDrst->sd_ctl_SdnominalData_dw = sdnominal;


#else
        // Form table index based on what Front End must actually tune...
        pDrst->sd_ctl_FeFreq_w = wTuneFreq100kHz + pDrst->sd_ctl_IfFreq_i;
        // Freq of Front End = Freq user wants to tune + IF
        // where IF = f(mix_reg), as defined in memory map

        // Check that freq index is within range of tuning table
        if (FREQUENCY_100KHZ_MIN > pDrst->sd_ctl_FeFreq_w)
        {
            //SystemHalt();
            pDrst->sd_ctl_FeFreq_w = FREQUENCY_100KHZ_MIN;
        }
        if (FREQUENCY_100KHZ_MIN + FREQUENCY_100KHZ_RANGE <
                                    pDrst->sd_ctl_FeFreq_w)
        {
            //SystemHalt();
            pDrst->sd_ctl_FeFreq_w = FREQUENCY_100KHZ_MIN +
                                        FREQUENCY_100KHZ_RANGE;
        }

        // Just tune lower two bytes - assume MIX_REG already set in initialization
        pDrst->sd_ctl_Tune1Data_dw =
           ((DWORD)(sdc_ctl_TableTune1_w[pDrst->sd_ctl_FeFreq_w - FREQUENCY_100KHZ_MIN])
           & 0x00007FFF); // Bits [14:0]

        pDrst->sd_ctl_SdnominalData_dw =
        sdc_ctl_TableSdnom_dw[pDrst->sd_ctl_FeFreq_w - FREQUENCY_100KHZ_MIN];
        // Bits [31:0]
#endif

    }

// **** Fix for TB2 stopping off-station during seek
    if (VERSION_TA2 == StfmTuneDefault.STFM1000_Version)

        {

        RetCode = STFM1000WriteRegister(

            AGC_CONTROL1_REG_ADDR,

            B2_AGCREF_REG_MASK,

            &pDrst->sd_ctl_ShadowAgcControl1_dw,

            7);

         } // end if TA2

     else  // must be version TB2

        {

        RetCode = STFM1000WriteRegister(

            AGC_CONTROL1_REG_ADDR,

            0x00003F00,          // mask for B2_DB_WB_REF_REG

            &pDrst->sd_ctl_ShadowAgcControl1_dw,

            34);

        } // end version TB2

    // **** End of Fix for TB2 stopping off-station during seek

    // Write control data to Steely Dan
    RetCode = STFM1000WriteRegister(
        TUNE1_REG_ADDR,
        //TUNE1_B2_MIX_REG_MASK|TUNE1_CICOSR_REG_MASK | TUNE1_PLL_DIV_REG_MASK,
        WHOLEREG_MASK,
        &pDrst->sd_ctl_ShadowTune1_dw,
        pDrst->sd_ctl_Tune1Data_dw);

    RetCode |= STFM1000WriteRegister(
        SDNOMINAL_REG_ADDR,
        WHOLEREG_MASK,
        &pDrst->sd_ctl_ShadowSdnominal_dw,
        pDrst->sd_ctl_SdnominalData_dw);

// **** Following fix for seek-not-stopping on alternate tunings
    /* enable double-buffered writes */
    RetCode = STFM1000WriteRegister(
            DATAPATH_REG_ADDR,
            DB_ACCEPT_REG_MASK,
            &pDrst->sd_ctl_ShadowDataPath_dw,
            1);

    /* disable double-buffered writes */
    RetCode = STFM1000WriteRegister(
            DATAPATH_REG_ADDR,
            DB_ACCEPT_REG_MASK,
            &pDrst->sd_ctl_ShadowDataPath_dw,
            0);
// **** End of fix for seek-not-stopping on alternate tunings

    // If bit[3:0] == 0
    if((pDrst->sd_ctl_Tune1Data_dw & 0x00000000000F) == 0)
    {
        RetCode |= STFM1000WriteRegister(
            CLK1_REG_ADDR,
            ENABLE_TAPDELAYFIX_REG_MASK,
            &pDrst->sd_ctl_ShadowClk1_dw,
            0);
    }
    else
    {
        RetCode |= STFM1000WriteRegister(
            CLK1_REG_ADDR,
            ENABLE_TAPDELAYFIX_REG_MASK,
            &pDrst->sd_ctl_ShadowClk1_dw,
            1);
    }

    // We like to keep the tune cap always positive value
    if(wTuneFreq100kHzTemp > CORNER_FREQUENCY)
    {
        wTuneCap = (WORD)(((TCB* wTuneFreq100kHzTemp - TCA)/100) & 0x00001F);
    }
    else
    {
        wTuneCap = (WORD)(((TCA - TCB* wTuneFreq100kHzTemp)/100) & 0x00001F);
    }

    if(wTuneCap < 4)
    {
        wTuneCap = 4;
    }
    RetCode |= STFM1000WriteRegister(
            LNA_REG_ADDR,
            ANTENNA_TUNECAP_REG_MASK,
            &pDrst->sd_ctl_ShadowLna_dw,
            wTuneCap);
    // Add new update
    RetCode |= STFM1000WriteRegister(
            INITIALIZATION2_REG_ADDR,
            DRI_CLK_EN_REG_MASK,
            &pDrst->sd_ctl_ShadowInitialization2_dw,
            0x00000000);

    switch(wTuneFreq100kHzTemp)
    {
        case 778:
        case 779:
        case 780:
        case 781:
        case 782:
        case 838:
        case 839:
        case 840:
        case 841:
        case 842:
        case 898:
        case 899:
        case 900:
        case 901:
        case 902:
        case 958:
        case 959:
        case 960:
        case 961:
        case 962:
        case 1018:
        case 1019:
        case 1020:
        case 1021:
        case 1022:
        case 1078:
        case 1079:
        case 1080:
            dwControlSelect = 5;
            break;
        default:
        dwControlSelect = 4;
    }
    RetCode |= STFM1000WriteRegister(
        DATAPATH_REG_ADDR,
        DRI_CLK_DIV_REG_MASK,
        &pDrst->sd_ctl_ShadowDataPath_dw,
        dwControlSelect);

    RetCode |= STFM1000WriteRegister(
            INITIALIZATION2_REG_ADDR,
            DRI_CLK_EN_REG_MASK,
            &pDrst->sd_ctl_ShadowInitialization2_dw,
            0x00000001);
    return (RetCode);
}



////////////////////////////////////////////////////////////////////////////////
//!
//! \brief      Set tuner to a new frequency
//! \fntype     Function
//!
//! Function will perform the following steps:
//!     Calls QucikTune() function to set the new tuning frequency
//!
//! \param[in]  wNewFrequency   New frequency to set (in KHz)
//!
//! \return     SUCCESS.
//!
////////////////////////////////////////////////////////////////////////////////
//  STFM1000TunerQuicksetStation(

_reentrant INT STFM1000TunerQuicksetStation(WORD wNewFrequency)
{
    INT RetCode = SUCCESS;
    s_wCurrentFMFrequency = wNewFrequency;
#if 0
    // Turn off DRI before change station
    //STFM1000DriOff();
    /* Take Down procedure call */
    RetCode = STFM1000TakeDown();
    if (SUCCESS != RetCode)
    {
        //SystemHalt();
         return RetCode;
    }
#endif
    RetCode = STFM1000QuickTune(wNewFrequency);
    if (SUCCESS != RetCode)
    {
        //SystemHalt();
        return RetCode;
    }
#if 0
    /* BringUp procedure call */
    RetCode = STFM1000BringUp();
    if (SUCCESS != RetCode)
    {
        //SystemHalt();
        return RetCode;
    }
#endif
    return RetCode;
}
// ****************************************
//! LOOK FOR PILOT TONE ON CHIP
// ****************************************

// * Look for Pilot Tone on chip  (tspec_v09x.pdf::SD_LookForPilot)

/*
#ifdef FUNCLET
    #pragma asm
        org p,".stfm1000_SYS":
        FSTFM1000LookForPilot:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet2 RSRC_FUNCLET_STFM1000_LOOK_FOR_PILOT

        org p,"SYSFUNCLET_STFM1000_LOOK_FOR_PILOT_P":
        dc      RSRC_FUNCLET_STFM1000_LOOK_FOR_PILOT
    #pragma endasm
_reentrant INT STFM1000LookForPilot_Funclet(BOOL * bPilotPresent)
#else
*/
#if 0   // move it to second codebank
_reentrant INT STFM1000LookForPilot(BOOL * bPilotPresent)
//#endif
{
    INT iCount;
    DWORD dwPiloTest;
    DWORD dwPiloTestLowerLimit = 0x1E;  			//TB2  +30 (0x1E)
    DWORD dwPiloTestUpperLimit = 0x7F;  //TB2
    DWORD dwPiloTestMask = PILOTEST_REG_TB2_MASK;   //VERSION_TA2

    STFM1000Tune_t * pDrst = &StfmTuneData;
    INT RetCode = SUCCESS;

    if (VERSION_TA2 == StfmTuneDefault.STFM1000_Version)
    {
        dwPiloTestMask = PILOTEST_REG_TA2_MASK;
        // Range changed to [-75, -30] on 11/03/2006
        dwPiloTestLowerLimit = 0xE2;     // TA2  -30(0xE2)
        dwPiloTestUpperLimit = 0xB5;     // TA2  -75(0xB5)

        /* Let's check the pilot for 3 times, and
           declare pilot present if value is within range all 3 times, */
        for (iCount=3; iCount>0; --iCount)
        {
            // get pilot level

            RetCode = STFM1000ReadSubRegister(
                PILOTCORRECTION_REG_ADDR,
                dwPiloTestMask,
                &dwPiloTest);

            if (SUCCESS != RetCode)
            {
                 //SystemHalt();
                 return RetCode;
            }

            if (g_STFM1000_CustomerForceMono == TRUE)   // STMP00013255
            {
                dwPiloTest = 0;
            }

            // compare pilot level to threshold
            // (value is positive so "ABS()" not needed)
            // -75<= dwPilotTest<-30
            if ((dwPiloTestUpperLimit < dwPiloTest) && (dwPiloTest < dwPiloTestLowerLimit))
            {
                continue;
            }
            else
            {
                // turn off pilot tracking on chip
                RetCode = STFM1000WriteRegister(
                    PILOTTRACKING_REG_ADDR,
                    B2_PILOTTRACKING_ENABLE_REG_MASK,
                    &pDrst->sd_ctl_ShadowPilotTracking_dw,
                    0);

                if (SUCCESS != RetCode)
                {
                     return RetCode;
                }

                *bPilotPresent = FALSE; // result to the calling function
                g_sd_audio_pCoefForcedMono_u1 = TRUE;
                break;
            } // pilot level below threshold
        }
        /* Pilot check passed every time in the preceding loop ? */
        if (iCount <= 0)
        {
            *bPilotPresent = TRUE; // result to the calling function
        }

    }
    else //TB2
    {
        /* Let's check the pilot for 3 times, and
           declare pilot present if value is within range all 3 times, */
        for (iCount=3; iCount>0; --iCount)
        {
            // get pilot level

            RetCode = STFM1000ReadSubRegister(
                PILOTCORRECTION_REG_ADDR,
                dwPiloTestMask,
                &dwPiloTest);

            if (SUCCESS != RetCode)
            {
                 //SystemHalt();
                 return RetCode;
            }

            if (g_STFM1000_CustomerForceMono == TRUE)
            {
                dwPiloTest = 0;
            }

            // compare pilot level to threshold
            // (value is positive so "ABS()" not needed)
            // 30<=dwPilotTest<127
            if ((dwPiloTestLowerLimit < dwPiloTest) &&
                 (dwPiloTest < dwPiloTestUpperLimit))
            {
                continue;
            }
            else
            {
                 // turn off pilot tracking on chip
                RetCode = STFM1000WriteRegister(
                    PILOTTRACKING_REG_ADDR,
                    B2_PILOTTRACKING_ENABLE_REG_MASK,
                    &pDrst->sd_ctl_ShadowPilotTracking_dw,
                    0);

                if (SUCCESS != RetCode)
                {
                     //SystemHalt();
                     return RetCode;
                }

                *bPilotPresent = FALSE; // result to the calling function

                /* enable "forced mono" in black box */
                g_sd_audio_pCoefForcedMono_u1 = TRUE;
                break;

            } // if ((dwPiloTestLowerLimit < dwPiloTest) ...
        }
        /* Pilot check passed every time in the preceding loop ? */
        if (iCount <= 0)
        {
            *bPilotPresent = TRUE; // result to the calling function
        }
    }
    pDrst->sd_ctl_PilotPresent_b = *bPilotPresent;
    return RetCode;
}
#endif // move it to second codebank
#ifdef FUNCLET
    #pragma asm
        org p,".ptextstfm1000":
    #pragma endasm
#endif



#ifdef FUNCLET
    #pragma asm
        org p,".ptextstfm1000":
    #pragma endasm
#endif

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief      STFM tuner station query
//! \fntype     Function
//!
//! Function will perform the following steps:
//!             Wiats 10 msec for SD circuits to settle
//!             Checks RSSI & TONEDATA to decide if this is a valid station
//!
//! \param[in]  pTunerObject     Pointer to Tuner Object.
//!             pbIsStation      Fn updates TRUE if this is a valid station
//!                                         FLASE if not a valid station
//! \return     SUCCESS/ERROR.
//!
////////////////////////////////////////////////////////////////////////////////
_reentrant INT STFM1000TunerQueryIsStation(BOOL * pbIsStation)
{

    DWORD dwShadowToneReg;
    WORD wRssiMantissa, wRssiExponent;
    WORD wPrssi;
    INT  iRssiLog;
    INT  iMpxDc;
    DWORD dwRssiDecoded;
    DWORD dwTemp;
    STFM1000Tune_t * pDrst = &StfmTuneData;
    INT RetCode = SUCCESS;

#if 0
    // We don't need to do it since we relase control for 10ms before it
    // Wait 10.0 msec for SD circuits to settle
    STFM1000delay_ms(50);
#endif
    // Get Rssi register readings from STFM1000
    RetCode = STFM1000ReadRegisterToShadow(RSSI_TONE_REG_ADDR,
                                            &dwShadowToneReg);
    if (SUCCESS != RetCode)
    {
        //SystemHalt();
        return RetCode;
    }
    g_wRawRSSIData = (WORD)(dwShadowToneReg & 0x00000000FFFF);
    // parse out signal strength reading sub-registers
    // 32-bit Temp variable used below becos, actual variables are 16-bit
    // and the ReadShadowRegister API requires a integer pointer.
    STFM1000ReadShadowRegister(RSSI_DCEST_REG_MASK, &dwShadowToneReg,
            &dwTemp);
    pDrst->sd_ctl_ShadowRssiDcEst_w = dwTemp & 0xffff;

    STFM1000ReadShadowRegister(TONEDATA_REG_MASK, &dwShadowToneReg,
            &dwTemp);
    pDrst->sd_ctl_ShadowToneData_w = dwTemp & 0x000fff;

    // Decode floating point RssiDcEst
    wRssiMantissa = (pDrst->sd_ctl_ShadowRssiDcEst_w & 0xFFE0)>>5;// 11 msb
    wRssiExponent = (pDrst->sd_ctl_ShadowRssiDcEst_w & 0x001F);   // 5 lsb
    dwRssiDecoded = wRssiMantissa << wRssiExponent;

    // Convert Rssi to 10log(Rssi)
    for (wPrssi = 20; wPrssi >0; --wPrssi)
    {
        if (dwRssiDecoded >= (1 << wPrssi))
        {
            break;
        }
    }
    iRssiLog = (3*dwRssiDecoded >> wPrssi) + (3*wPrssi - 3);
    // clamp to positive
    if (iRssiLog < 0)
    {
        iRssiLog = 0;
    }
    // Compensate for errors in truncation/approximation by adding 1
    iRssiLog += 1;

    // Save the value for sending across to UI later
    pDrst->sd_ctl_ShadowRssiDcEstLog_w = (WORD)iRssiLog;
    s_iFMSignalStrengthRSSI = iRssiLog;

    // Determine absolute value of iMpxDc
    /* TONE_DATA_REG is 12 bit signed, sign extend if needed */
    if (pDrst->sd_ctl_ShadowToneData_w & 0x000800)
    {
        /* negative, so sign extend to 24 bits int */
        iMpxDc = pDrst->sd_ctl_ShadowToneData_w | 0xfff000;
    }
    else
    {
        iMpxDc = pDrst->sd_ctl_ShadowToneData_w;
    }
#ifdef SD_USE_50KHZ_TUNING_GRID
    s_iFMSignalStrengthMpxDc = iMpxDc;
#endif
    iMpxDc = (iMpxDc < 0) ? (0 - iMpxDc) : (iMpxDc);
#ifdef DEBUG_MONITOR_SIGNAL_QUALITY
    //s_wMpxAdc = iMpxDc;
#endif
    // Check if both values over their thresholds; if so, it's valid.
#ifdef SD_USE_50KHZ_TUNING_GRID
    if (iRssiLog > pDrst->sd_ctl_TuneRssiTh_w)
#else
    if ((iRssiLog > pDrst->sd_ctl_TuneRssiTh_w) &&
       (iMpxDc < pDrst->sd_ctl_TuneMpxDcTh_w))
#endif
    {
        *pbIsStation = TRUE;
    }
    else
    {
        *pbIsStation = FALSE;
    }

    return RetCode;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief      Read a 4-byte STFM register via I2C interface into shadow
//! \fntype     function
//!
//! Function will perform the following steps:
//!     - Sends out address byte over the I2C interface
//!     - Read back 4-byte register contents
//!     - Updates shadow
//!
//! \param[in]  wAddr     register address to read from
//!             pShadow     pointer to shadow register
//!
//! \return     SUCCESS/ERROR.
//!
////////////////////////////////////////////////////////////////////////////////
_reentrant INT STFM1000ReadRegisterToShadow(WORD wAddr, DWORD *pShadow)
{
    WORD wTriesCount = 0;
    INT  RetCode;
 // Now begin writing the address and relevant data bytes
    do{

       RetCode = STFM1000I2CReadRegister((wAddr-4),     // Register addr
                                          0x00000000,    // dummy write value
                                          pShadow // Data to set the register
                                         );
       if(0 != RetCode)
       {
           RetCode = STFM1000I2CInit(0,0);
       }
       else
       {
            break;
       }

    }while(++wTriesCount < MAX_I2C_READ_TRIES);

    STFM1000ReadDataReAlignment(pShadow);
    // Something really bad
    if(0 != RetCode)
    {
        // SystemHalt();
    }
    return RetCode;
}  // end STFM1000ReadRegisterToShadow()

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief      Extract a register field (based on mask) from shadow
//! \fntype     function
//!
//! Function will perform the following steps:
//!     - Extract from shadow the required bit-field
//!     - Put the data into value pointed by pData
//!
//! \param[in]  dwMask     Bit-positions to place the data
//!             pShadow     pointer to shadow register
//!             pData       Pointer to data (this fn will update)
//!
//! \return     SUCCESS/ERROR.
//!
////////////////////////////////////////////////////////////////////////////////
_reentrant void STFM1000ReadShadowRegister(DWORD dwMask, DWORD *pShadow,DWORD *pData)
{
    DWORD dwShift, dwMaskCopy, dwNewData = *pShadow;

    // Determine from mask the shift required to "place" the new data
    dwMaskCopy = dwMask;
    dwShift = 0;
    while ((dwMaskCopy & 0x00000001) == 0)  // Determine how many trailing zeros
    {
        dwMaskCopy = dwMaskCopy >> 1;
        dwShift++;
    }

    *pData = (dwNewData & dwMask) >> dwShift;
}  // end STFM1000ReadShadowRegister()


////////////////////////////////////////////////////////////////////////////////
//!
//! \brief      Read masked I2C sub-register into buffer
//! \fntype     function
//!
//! Function will perform the following steps:
//!     - Extract from shadow the required bit-field
//!     - Put the data into value pointed by pData
//!
//! \param[in]  wAddr     register address to read from
//!             dwMask     Bit-positions to extract the data
//!             pData       Pointer to data (this fn will update)
//!
//! \return     SUCCESS/ERROR.
//!
////////////////////////////////////////////////////////////////////////////////
_reentrant INT STFM1000ReadSubRegister(WORD wAddr, DWORD dwMask,
    DWORD *pData)
{
    DWORD dwTempShadow;
    INT RetCode = SUCCESS;
    // read 4-byte register into temporary buffer
    RetCode = STFM1000ReadRegisterToShadow(wAddr, &dwTempShadow);

    if (SUCCESS != RetCode)
    {
        //SystemHalt();
         return RetCode;
    }

    // get masked sub-register out of temporary buffer
    STFM1000ReadShadowRegister(dwMask, &dwTempShadow, pData);

    return RetCode;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief      Write an STFM register via I2C interface
//! \fntype     function
//!
//! Function will perform the following steps:
//!     - shift the data into correct bit-field based on mask
//!     - form new data by placing this shifted data with that from shadow
//!     - Write into STFM register and update the shadow.
//!     - DOES NOT update shadow on failure.
//!
//! \param[in]  wAddr     register address to write to
//!             dwMask     Bit-positions to place the data
//!             pShadow     pointer to shadow register
//!             dwdata     New data to write into STFM register
//!
//! \return     SUCCESS/ERROR.
//!
////////////////////////////////////////////////////////////////////////////////
_reentrant INT STFM1000WriteRegister(WORD wAddr, DWORD dwMask, DWORD *pdwShadow, DWORD dwData)
{
    DWORD dwShift, dwMaskCopy, dwPlacedData, dwDataNoChange, dwNewData;
    DWORD dwEventMask;
    WORD  wTriesCount = 0;
    INT RetCode = SUCCESS;

    // Determine from mask the shift required to "place" the new data
    dwMaskCopy = dwMask;
    dwShift = 0;
    while (0==(dwMaskCopy&0x00000001))  // Determine how many trailing zeros
    {
        dwMaskCopy = dwMaskCopy >> 1;
        dwShift++;
    }

    // shift data into place and mask it
    dwPlacedData = (dwData<< dwShift) & dwMask;

    // Apply anti-mask to shadow reg for data that we're NOT changing
    dwDataNoChange = *pdwShadow&(~dwMask);
    // data_no_change has zeros over mask position now

    // Form new control data - this data should represent the entire 32-bit
    // control word, however we will only transmit those bytes of the control
    // word to which the mask applies
    dwNewData = dwDataNoChange | dwPlacedData;

    STFM1000WriteDataReAlignment(&dwNewData);
    // Now begin writing the address and relevant data bytes
    do{

        RetCode = STFM1000I2CWriteRegister(wAddr,     // Register addr
                                           dwNewData // Data to set the register
                                           );
       if(0 != RetCode)
       {
           RetCode = STFM1000I2CInit(0,0);
       }
       else
       {
            break;
       }
    }while(++wTriesCount < MAX_I2C_WRITE_TRIES);

    // Something really bad
    if(0 != RetCode)
    {
        //SystemHalt();
    }

    if (RetCode == SUCCESS)
    {
        // Save the unlignment data, not alognment data
        *pdwShadow = dwDataNoChange | dwPlacedData;
    }
    else
    {
        //SystemHalt();
    }

    return RetCode;
}  // end STFM1000WriteRegister()

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          STFM1000WriteDataReAlignment
//
//   Type:          Function
//
//   Description:   The I2C sends the highest byte first which will go to the LSB
//                  [7:0] first, so we have to swap the data before write, but we not
//                  real care the highest 2-byte of the pdwNewData since the I2C driver
//                  will ignore them
//
//   Inputs:        pdwNewData
//
//   Outputs:       None
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
_reentrant void STFM1000WriteDataReAlignment(DWORD* pdwNewData)
{
    DWORD dwTemp = ((*pdwNewData)>>24) & (0x0000000000ff);   // bit[32:24] ->bit[7:0]
    dwTemp |=  (((*pdwNewData)>>8) & (0x00000000ff00));       // bit[23:16] ->bit[15:8]
    dwTemp |=  (((*pdwNewData)<<8) & (0x000000ff0000));       // bit[15:8] ->bit[23:16]
    dwTemp |=  (((*pdwNewData)<<24) & (0x0000ff000000));       // bit[7:0] ->bit[31:24]
    *pdwNewData = dwTemp;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          STFM1000ReadDataReAlignment
//
//   Type:          Function
//
//   Description:   The I2C receive the lowest byte of Steely Dan register first which will go to the MSB
//                  bit[31:24] so we have to swap the data after read, but we not
//                  real care the highest 2-byte of the pdwNewData
//
//   Inputs:        pdwNewData
//
//   Outputs:       None
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
_reentrant void STFM1000ReadDataReAlignment(DWORD* pdwNewData)
{
    DWORD dwTemp = ((*pdwNewData)>>24) & (0x0000000000ff);   // bit[31:24] ->bit[7:0]
    dwTemp |=  (((*pdwNewData)>>8) & (0x00000000ff00));       // bit[23:16] ->bit[15:8]
    dwTemp |=  (((*pdwNewData)<<8) & (0x000000ff0000));       // bit[15:8] ->bit[23:16]
    dwTemp |=  (((*pdwNewData)<<24) & (0x0000ff000000));       // bit[7:0] ->bit[31:24]
    *pdwNewData = dwTemp;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          STFM1000delay_ms
//
//   Type:          Function
//
//   Description:   Delay for wDelay ms
//
//   Inputs:        wDelay ms
//
//   Outputs:       None
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
_reentrant void STFM1000delay_ms(WORD wDelay)
{

    WORD wTargetTime;
    // Calculate the target time
    wTargetTime = SysGetCurrentTime() + wDelay;

    // Do the wait thang
    while(1)
    {
        if(SysGetCurrentTime()> wTargetTime )
        {
            break;
        }
    }

}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          STFM1000I2CWriteRegister
//
//   Type:          Function
//
//   Description:   Write 4 bytes to I2C device address / sub address
//
//
//
//   Inputs:    INT I2C_Sub_Addr        Register to write on STFM1000
//              DWORD DataOut          4 bytes of data
//
//   Outputs:     0 = No error
//               -1 = Timeout error
//               -2 = ACKF - no ACK received
//
//   Notes:
//
//           First write sends 1 byte of data, second write sends the remaining NumBytes.
//           Use NumBytes = ONE_BYTE for the "dummy write" needed to receive data
//
//
//  How to set the data you want:
//
//  If you are write 4-byte to a register, if the value you are going to set is
//  0x00010203. To be aware, the MSB is sent first in HW_I2CDAT register. For example,
//  the slave address for Steely Dan is 0xC0, and we like to set 0x00000004 register.
//  So the first word for the write command will be 0xC0 04 plus  1-byte data going to
//  set to the register, if you like to set the register as 0x00 01 02 03 of bits[0:31],
//  so have to set HW_I2CDAT register as 0xC0 04 00, then 0x01 02 03.
//
////////////////////////////////////////////////////////////////////////////////
_reentrant INT STFM1000I2CWriteRegister(WORD I2C_Sub_Addr, DWORD DataOut)
{
    DWORD_type MyData;
    i2cdata_type i2cdata;
    INT cnt;
    MyData.D = DataOut;

    for(cnt = 0; (0 != HW_I2CCSR.B.BUSY); cnt++)    // Delay time 1 cnt
    {
        if (TIMEOUT_COUNT <= cnt) return(TIMEOUT_ERROR);
    }

    i2cdata.B.BYTE0 = STFM1000_I2C_ADDR;
    i2cdata.B.BYTE1 = I2C_Sub_Addr;
    i2cdata.B.BYTE2 = MyData.U.W0;  // First byte to write (in LSB of word)
    HW_I2CDAT.U = i2cdata.U;

    HW_I2CCSR.B.TREQ = 1;       // Master transaction request - generate start condition
    HW_I2CCSR.B.TREQ = 0;       // and transmit Slave Device Write addres

    for(cnt = 0; (1 != HW_I2CCSR.B.TUFL); cnt++)    // Delay time aprox 0.184ms @100Khz I2c Clock and 36Mhz system clock
    {
        if (TIMEOUT_COUNT <= cnt) return(TIMEOUT_ERROR);
    }

    for(cnt = 0; (0 != HW_I2CCSR.B.ACKF); cnt++)
    {
        if (TIMEOUT_COUNT <= cnt) return(ACKF_ERROR);
    }

    HW_I2CCSR.B.WL = THREE_BYTES;   // Word Length 1, 2, or 3 bytes
    HW_I2CCSR.B.LWORD = 1;          // Send the stop command

    HW_I2CDAT.U = MyData.U.W1;      // Send last 3 bytes

    HW_I2CCSR.B.TUFLCL = 1;         // Clear the Receiver Overflow bit
    HW_I2CCSR.B.TUFLCL = 0;

    for(cnt = 0; (0 != HW_I2CCSR.B.ACKF); cnt++)
    {
        if (TIMEOUT_COUNT <= cnt) return(ACKF_ERROR);
    }
    return(0);

}
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          STFM1000I2CReadRegister
//
//   Type:          Function
//
//   Description:   Read a 4-byte register
//                  first word has first 3 byte
//                  second word has remaining byte
//                  Uses delay to wait for RDR.
//                  The delay time is aprox 0.370ms @100Khz I2c Clock and 36Mhz system clock
//                  This routind also calls I2C_Write_Master_Sync which uses delay to wait for TUFL.
//                  The delay time is aprox 0.184ms @100Khz I2c Clock and 36Mhz system clock
//                  ==> Total delay waiting for status bits is aprox 0.55ms
//
//
//   Inputs:    int I2C_Sub_Addr       STMP1000 register address that is 4 less than the values to be read
//              DWORD DataOut          4 bytes data for dummy write from "shadow register" @ I2C_Sub_Addr
//              DWORD * DataIn         4 bytes data read
//
//   Outputs:
//
//   Notes:
//           A "dummy" write is first made to setup the I2C_Sub_Addr. This value is 4 less than
//           the value of the STFM1000 register to be read. e.g. If I2C_Sub_Addr=0 then the register
//           at location 4 is read. This is because the STMP3500 must write 3 bytes on the initial write
//           and a minimum of 1 byte on the final write. Because the STFM1000 "auto increments" the
//           address, the address of the start of the read is 4. The user must insure tha the correct
//           data is written for the "dummy" write. It is recommended that a "shadow register" be
//           used that contains the correct data to be written.
//           Then the first read gets 3 bytes of data, second read sends the remaining 1 byte.
//
//           If last byte is zero then the 4 bytes are re-read.
//           This appears to be a defect in the STMP3500 SOC as an I2C analyzer looking at
//           the I2C bus shows the correct data is sent by the STFM1000 device
//           Rereading the data improves the error rate since
//           all observed errors were due to the last byte being zero.
//
////////////////////////////////////////////////////////////////////////////////

_reentrant INT STFM1000I2CReadRegister(WORD I2C_Sub_Addr, DWORD DataOut, DWORD * DataIn)
{
    DWORD_type MyData;
    INT cnt;
    INT iRtnCode;
    i2cdata_type i2cdata;
    INT nReads = 3;              // number of words within I2C transaction
    DWORD Timer = 0;
    INT JunkWord;

        //  Perform DummyWrite
        iRtnCode = STFM1000I2CWriteRegister(I2C_Sub_Addr, DataOut);
        if(0 != iRtnCode) return iRtnCode;

        for(cnt = 0; (0 != HW_I2CCSR.B.BUSY); cnt++)
        {
            if (TIMEOUT_COUNT <= cnt) return(TIMEOUT_ERROR);
        }

        for(cnt = 0; (0 != HW_I2CCSR.B.ACKF); cnt++)
        {
            if (TIMEOUT_COUNT <= cnt) return(ACKF_ERROR);
        }

// Need to add a delay after writing address and data because STOP/START sequence is too
// short otherwise and the STFM1000 device does not recognize the STOP/START, but sees data
// one clock too soon, and it then pulls the data line low to ACK, but this destroys the
// one that STMP3500 is trying to write. The one indicates that the command is a read.
// Then the STFM1000 is waiting to read data and the STMP3500 is also waiting for a read.
// This deadlock is removed if a wait is added between the read and write commands.
// Sometimes the delay is 3X or 4X longer, but his is OK in the I2C protocol.

///       HW_GP1DOR.B.B12 = 1;   // DEBUG ONLY
        SetDclkCount((DWORD) 0);   // Init the DCLK count register for time measurement
//        Timer = GetDclkCount(); // Assume DCLK = 1/f and f=24Mhz to generate a 42us delay
        while(Timer < 2000)    // min 42us, can be longer if an interrupt happens, but that is OK
        {
           Timer = GetDclkCount(); // Assume DCLK = 1/f and f=24Mhz to generate a 42us delay
        }
///        HW_GP1DOR.B.B12 = 0;   // DEBUG ONLY

        // Start read from address I2C_Sub_Addr + 4
        HW_I2CCSR.B.WL = THREE_BYTES;      // Word Length 24 bits or 3 bytes
        i2cdata.U = 0;
        i2cdata.B.BYTE0 = STFM1000_I2C_ADDR + READMODE;
        HW_I2CDAT.U = i2cdata.U;

    // Unlike write transactions, the WL bits only affect number of bytes read during the
    // last read (HW_I2CCSR.B.LWORD set).  All other reads are always 3 bytes at a time.

        HW_I2CCSR.B.TREQ = 1;       // Generate the Repeated Start
        HW_I2CCSR.B.TREQ = 0;
    // Nov 2006: GR
    // Hardware has a problem that sometimes zeros the last byte read during a transaction
    // that has HW_I2CCSR.B.LWORD set.  To avoid this problem, reading continues past the
    // bytes needed so that none of the bytes we keep can be affected by the hardware bug.
    while (nReads > 0)
    {
        // Wait for Receive Data Register bit to be set
        // Indicating that the word is ready

        for(cnt = 0; (1 != HW_I2CCSR.B.RDR); cnt++)
        {
            //SysWaitOnEvent(0,0,0);       // let kernel spin once
            if (TIMEOUT_COUNT <= cnt) return(TIMEOUT_ERROR);
        }

	  if (nReads == 2)
	      MyData.U.W0 = HW_I2CDAT.U;          // First word, keep first 3 bytes
	  if (nReads == 1)
	  {
		MyData.U.W1 = HW_I2CDAT.U;          // Second word, really need only LS byte
		HW_I2CCSR.B.LWORD = 1;          // Send the stop command  (only needed once in this function). Stop since this is a repeated start read.
		HW_I2CCSR.B.WL = ONE_BYTE;      // Word Length one byte
	  }
	  if (nReads == 0)
		JunkWord = HW_I2CDAT.U;	//Read the extra word, throw it away

        for(cnt = 0; (0 != HW_I2CCSR.B.ACKF); cnt++)
        {
            if (TIMEOUT_COUNT <= cnt) return(ACKF_ERROR);
        }

        HW_I2CCSR.B.ROFLCL = 1;         // clear the Reciever Overflow bit
        HW_I2CCSR.B.ROFLCL = 0;  
	  nReads--;
    }   // end nReads loop

    *DataIn = (MyData.D>>16) & 0x0000ffffffff;  // Put in same format as input
    
    return(0);
        
}
 
// MLP: 6/29/06
// These 2 DclkCount function are moved here from PlayList2_static.c
// The playlist2 code does not use them and STFM needs them.
#if (defined USE_PLAYLIST2) || (defined USE_PLAYLIST5)
static DWORD GetDclkCount(void)
{
    return((((DWORD)(HW_DCLKCNTU.I))<<24) | ((DWORD)(HW_DCLKCNTL.I)));
}
#endif
#ifdef USE_PLAYLIST3
DWORD GetDclkCount(void)
{
    return((((DWORD)(HW_DCLKCNTU.I))<<24) | ((DWORD)(HW_DCLKCNTL.I)));
}
#endif

void SetDclkCount(DWORD dwCount)
{
    // TODO:  protect this from interrupts
    HW_DCLKCNTU.I = (WORD)(dwCount >> 24);
    HW_DCLKCNTL.I = (WORD)(dwCount);
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          I2C_Init
//
//   Type:          Function
//
//   Description:   Initialize I2C hardware registers for use by STFM1000 I2C
//
//
//   Inputs:     mode   1 = Fast (400Kbps) 0 = Slow (100Kbps)
//               int ClockDiv   If ClockDiv is zero then the speed is computed according
//                              to SysGetSpeed() and the mode bit
//
//   Outputs:    -1 Unexpected register value upon reset
//				 -2 Unable to enable RIE
//				 -3 Time waitinf for BUSY to clear
//
//   Notes:      table of ClockDiv vs Processor Speed
//                  I2C Clock = DCLK /(ClockDiv * 1 + 3)     Fast Mode
//                  I2C Clock = DCLK /(ClockDiv * 4 + 12)    Slow Mode
//
//    !!! The STMP3500 data sheet uses factors *2 and *8 instead of *1 and *4 as above !!!
//
//        The measured clock frequencies are a little below the desired frequencies.
//        i.e. 100Khz clock was measured at 97Khx and 400Khz clock was measured at 370Khz.
//
//              Excessive error rate observed at 400Kbps.
//              It has been recommended that the values of the pull-up resistors need to be
//              changed to run in Fast mode.
//
//
//  SPEED_IDLE      equ     0
//  SPEED_MP3       equ     1
//  SPEED_ADPCM     equ     2
//  SPEED_MIXER     equ     3
//  SPEED_ADCBASE   equ     4
//  SPEED_MAX       equ     5
//  SPEED_WMA       equ     6
//  SPEED_MP3ENCODE	equ	    7
//  SPEED_PEAK      equ     8
//  SPEED_LAST      equ     SPEED_PEAK
////////////////////////////////////////////////////////////////////////////////
#ifdef FUNCLET
    #pragma asm
        org p,".stfm1000_SYS":
        FSTFM1000I2CInit:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet2 RSRC_FUNCLET_STFM1000_I2C_INIT

        org p,"SYSFUNCLET_STFM1000_I2C_INIT_P":
        dc      RSRC_FUNCLET_STFM1000_I2C_INIT
    #pragma endasm
_reentrant INT STFM1000I2CInit_Funclet(WORD mode, WORD ClockDiv)
#else
_reentrant INT STFM1000I2CInit(WORD mode, WORD ClockDiv)
#endif
{
    int Speed;
    int cnt;
    int iI2CWord3;
    int RtnCode = 0;
    int temp = 0;

    HW_I2CCSR.B.RIE = 0;		// Disable I2C IRQ

    HW_GP0ENR.B.B16 = 0;        // enable output
    HW_GP0ENR.B.B17 = 0;        // enable output

    HW_GP0PWR.B.B16 = 1;        // power on
    HW_GP0PWR.B.B17 = 1;        // power on

#if 1
    for(cnt = 0; (0 != HW_I2CCSR.B.BUSY); cnt++)    // Delay time 1 cnt
    {
        if (TIMEOUT_COUNT <= cnt)
        {
            HW_I2CCSR.I = 0;	// Reset I2C device
            HW_I2CCSR.B.TREQ = 1;
            HW_I2CCSR.B.TREQ = 0;
            HW_I2CCSR.B.I2C_EN = 1;
            HW_I2CCSR.B.ACKF = 0;
            break;  // no timeout, break anyway
        }
    }
#else
    for(cnt = 0; (0 != HW_I2CCSR.B.BUSY); cnt++)    // Delay time 1 cnt
    {
        if (TIMEOUT_COUNT <= cnt)   RtnCode = -3;     //return(TIMEOUT_ERROR);
    }
#endif

    for(cnt = 0; cnt < 100; cnt++) { _nop(); }  // Short delay

    HW_I2CCSR.I = 0;	// Reset I2C device

    iI2CWord3 = HW_I2CCSR.I;	// verify that reset worked

    if ( 0 != (RESET_HW_I2CCSR_MASK & iI2CWord3) )
    {
      	 RtnCode = -1;			// Continue anyway - test may not be that good
    }

    HW_I2CCSR.B.I2C_EN = 1;	 	// Enable I2C device

    if (0 == ClockDiv)         // lookup Clock Div based on System Clock Speed
    {
        Speed = SysGetSpeed();
        switch(Speed)
        {
            case SPEED_IDLE:      ClockDiv = I2C_IDLE_SPEED;          break;
            case SPEED_FM:        ClockDiv = I2C_FM_SPEED;            break;
            case SPEED_FM_EQ:     ClockDiv = I2C_FM_EQ_SPEED;         break;
            case SPEED_MP3:       ClockDiv = I2C_MP3_SPEED;           break;
            case SPEED_ADPCM:     ClockDiv = I2C_ADPCM_SPEED;         break;
            case SPEED_MIXER:     ClockDiv = I2C_MIXER_SPEED;         break;
            case SPEED_ADCBASE:   ClockDiv = I2C_ADCBASE_SPEED;       break;
            case SPEED_MAX:       ClockDiv = I2C_MAX_SPEED;           break;
            case SPEED_WMA:       ClockDiv = I2C_WMA_SPEED;           break;
            case SPEED_MP3ENCODE: ClockDiv = I2C_MP3ENCODE_SPEED;     break;
            case SPEED_PEAK:
            default: 			  ClockDiv = I2C_PEAK_SPEED;          break;
         }
    }

    //HW_I2CDIV.I = (ClockDiv<<1) & 0x0001FE;   // Clock Divider Register (e.g. 0x78) Write Only
    HW_I2CDIV.B.FACT = ClockDiv;                // Cannot be read, we need to set bit[8:1]

    HW_I2CCSR.B.MODE = mode;    // Operating Mode Bit  1=Fast

    if (1 == HW_I2CCSR.B.ROFL) 	// Clear Receiver Overflow
    {
    	temp = HW_I2CDAT.U;
    	HW_I2CCSR.B.ROFLCL = 1;
    	HW_I2CCSR.B.ROFLCL = 0;
//    	if ( 1 == HW_I2CCSR.B.ROFL) SystemHalt(); //debug only
    }

    if (1 == HW_I2CCSR.B.TUFL) 	// Clear Transmitter Undererflow
    {
    	HW_I2CDAT.U = temp;
    	HW_I2CCSR.B.TUFLCL = 1;
    	HW_I2CCSR.B.TUFLCL = 0;
//    	if (1 == HW_I2CCSR.B.TUFL) SystemHalt(); //debug only
    }

//  Only forISR version of code 	HW_I2CCSR.B.RIE = 1;		// Enable I2C IRQ

//  Only forISR version of code 	if( 0 == HW_I2CCSR.B.RIE) RtnCode = -2; //SystemHalt();	//Debug only

    return(RtnCode);
}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextstfm1000":
    #pragma endasm
#endif


_reentrant void STFM1000I2CClose(void)
{
    HW_I2CCSR.B.I2C_EN = 0;	 	// Disable I2C device
    HW_GP0PWR.B.B16 = 0;        // power down
    HW_GP0PWR.B.B17 = 0;        // power down

}
/*
#ifdef FUNCLET
    #pragma asm
        org p,".stfm1000_SYS":
        FSTFM1000SetLnaBias:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet2 RSRC_FUNCLET_STFM1000_SET_LNA_BIAS

        org p,"SYSFUNCLET_STFM1000_SET_LNA_BIAS_P":
        dc      RSRC_FUNCLET_STFM1000_SET_LNA_BIAS
    #pragma endasm
_reentrant INT STFM1000SetLnaBias_Funclet(INT iRssiLog)
#else
*/
#if 0  //move to second codebank
_reentrant INT STFM1000SetLnaBias(INT iRssiLog)
//#endif
{

    DWORD dwShadowAgcStatReg;
    DWORD dwBias1Temp;
    DWORD dwAgcOut;
    DWORD dwLnaRms,dwLnaAmp1ImproveDist;
    INT RetCode = SUCCESS;
    STFM1000Tune_t * pDrst = &StfmTuneData;


     /* read AGC_STAT register */
    RetCode = STFM1000ReadRegisterToShadow(AGC_STAT_REG_ADDR,
                                            &dwShadowAgcStatReg);
    if (SUCCESS != RetCode)
    {
        return RetCode;
    }
    /* extract AgcOut */
    STFM1000ReadShadowRegister(AGCOUT_STAT_REG_MASK, &dwShadowAgcStatReg,
            &dwAgcOut);
    // Bit[1] of ShadowLna
    dwBias1Up = (0 != (pDrst->sd_ctl_ShadowLna_dw & 0x00000002)) ? (1):(0);
    // Bit[0] of ShadowLna
    dwBias1Down = (0 != (pDrst->sd_ctl_ShadowLna_dw & 0x00000001)) ? (1):(0);
    /* extract LnaRms */
    STFM1000ReadShadowRegister(LNA_RMS_REG_MASK, &dwShadowAgcStatReg,
            &dwLnaRms);

    // Bit[23] of ShadowRef
    dwLnaAmp1ImproveDist = (0 != (pDrst->sd_ctl_ShadowRef_dw & 0x00800000)) ? (1):(0);  // bit 23

    if (31 == dwAgcOut)
    {

        if (VERSION_TA2 == StfmTuneDefault.STFM1000_Version)
            {
            RetCode = STFM1000WriteRegister(
                AGC_CONTROL1_REG_ADDR,
                B2_AGCREF_REG_MASK,
                &pDrst->sd_ctl_ShadowAgcControl1_dw,
                10);
            if (SUCCESS != RetCode)
                {
                //SystemHalt();
                return RetCode;
                }
            } // end if TA2
         else  // must be version TB2
            {
            RetCode = STFM1000WriteRegister(
                AGC_CONTROL1_REG_ADDR,
                0x00003F00,          // mask for B2_DB_WB_REF_REG
                &pDrst->sd_ctl_ShadowAgcControl1_dw,
                42);
            if (SUCCESS != RetCode)
                {
                //SystemHalt();
                return RetCode;
                }
            } // end version TB2

        if(18 >= iRssiLog)
        {
            if(dwLnaRms==0)
            {
                if(dwBias1Down)
                {
                    dwBias1Down = 0;
                }
                else
                {
                    dwBias1Up = 1;
                    dwLnaAmp1ImproveDist = 0;
                }
            }
        }
        if(28 <= iRssiLog)  // was 24
        {
            if(dwBias1Up)
            {
                dwBias1Up = 0;
                dwLnaAmp1ImproveDist = 1;
            }
            else
            {
                if (VERSION_TA2 == StfmTuneDefault.STFM1000_Version)
                    {
                    RetCode = STFM1000WriteRegister(
                        AGC_CONTROL1_REG_ADDR,
                        B2_AGCREF_REG_MASK,
                        &pDrst->sd_ctl_ShadowAgcControl1_dw,
                        7);
                    if (SUCCESS != RetCode)
                        {
                        //SystemHalt();
                        return RetCode;
                        }
                     } // end if TA2
                 else  // must be version TB2
                    {
                    RetCode = STFM1000WriteRegister(
                        AGC_CONTROL1_REG_ADDR,
                        0x00003F00,          // mask for B2_DB_WB_REF_REG
                        &pDrst->sd_ctl_ShadowAgcControl1_dw,
                        34);
                    if (SUCCESS != RetCode)
                        {
                        //SystemHalt();
                        return RetCode;
                        }
                    } // end version TB2

                dwBias1Down = 1;
            }

        }
    }
    else  // (31 != dwAgcOut)
    {
        if (VERSION_TA2 == StfmTuneDefault.STFM1000_Version)
            {
            RetCode = STFM1000WriteRegister(
                AGC_CONTROL1_REG_ADDR,
                B2_AGCREF_REG_MASK,
                &pDrst->sd_ctl_ShadowAgcControl1_dw,
                7);
            if (SUCCESS != RetCode)
                {
                //SystemHalt();
                return RetCode;
                }
             }  // end if TA2
         else  // must be version TB2
            {
            RetCode = STFM1000WriteRegister(
                AGC_CONTROL1_REG_ADDR,
                0x00003F00,          // mask for B2_DB_WB_REF_REG
                &pDrst->sd_ctl_ShadowAgcControl1_dw,
                34);
            if (SUCCESS != RetCode)
                {
                //SystemHalt();
                return RetCode;
                }
            } // end version TB2

        dwBias1Up = 0;
        dwBias1Down = 1;
        dwLnaAmp1ImproveDist = 1;
    }
    if (VERSION_TB2 == StfmTuneDefault.STFM1000_Version)
    {
        RetCode = STFM1000WriteRegister(
            REF_REG_ADDR,
            LNA_AMP1_IMPROVE_DISTORTION,
            &pDrst->sd_ctl_ShadowRef_dw,
            dwLnaAmp1ImproveDist);

        if (SUCCESS != RetCode)
        {
            return RetCode;
        }
    }
    dwBias1Temp = ((dwBias1Up<<1) | dwBias1Down) & (0x00000003);
    RetCode = STFM1000WriteRegister(
        LNA_REG_ADDR,
        (IBIAS1_UP_MASK | IBIAS1_DN_MASK),
        &pDrst->sd_ctl_ShadowLna_dw,
        dwBias1Temp);

    if (SUCCESS != RetCode)
    {
        return RetCode;
    }
    return RetCode;
}
#endif   //move to second codebank
#ifdef FUNCLET
    #pragma asm
        org p,".ptextstfm1000":
    #pragma endasm
#endif
// **************************************************************************
// ************ Global variables ********************************************
// **************************************************************************

extern int _Y g_sd_audio_LprB0_i16;      // -3dB cutoff = 17 kHz
extern int _Y g_sd_audio_LprB1_i16;      // -3dB cutoff = 17 kHz
extern int _Y g_sd_audio_LprA1_i16;      // -3dB cutoff = 17 kHz
extern int _Y g_sd_audio_LprA2_i16;      // -3dB cutoff = 17 kHz

extern int _Y g_sd_audio_pCoefForcedMono_u1;
extern int _Y g_sd_audio_pCoefBypassBlend_u1;
extern int _Y g_sd_audio_pCoefBypassSoftmute_u1;
extern int _Y g_sd_audio_pCoefBypassBwCtl_u1;

extern _fract gLPR_Mult_fract;
extern _fract gLMR_Mult_fract;

extern _fract LPR_Data_fract;
extern _fract LMR_Data_fract;
extern _fract _circ _Y  LPR_XYData[5];       // LRP data to be filtered
extern _fract _circ _Y  LMR_XYData[5];       // LMR data to be filtered

extern _fract _circ _Y *_Y LPR_PtrXUpdate;    // Address to store X(t) and start of XYData
extern _fract _circ _Y *_Y LPR_PtrYUpdate;    // Address to store Y which becomes Y(t-1) for next loop

extern _fract _circ _Y *_Y LMR_PtrXUpdate;    // Address to store X(t) and start of XYData
extern _fract _circ _Y *_Y LMR_PtrYUpdate;    // Address to store Y which becomes Y(t-1) for next loop

// Semaphore to indicate that global coefficients in variables g_sd_audio_* is ready to be copied to IIRFiltCoeff[]
extern int IIRFiltCoeffSemaphore;

/* IIRFilterCoeff array is in Q23 format */
extern  int _circ _X IIRFiltCoeff[10];

//extern  int gCOEF_SCALE; //change it to a Macro

extern  sw_stereocontrol_t sw_stereocontrol;

/////////////////////////////////////////////////////////////////////////////////
// Name:           sw_stereocontrol_bwsweep
//
//  Type:           Function
//  Description:    This function contain Signal quality indicator and
//                  week signal processingfunctionality.it updates above
//					IIR coefficients for stereo path.
//
//                  Semaphore IIRFiltCoeffSemaphore is set/cleared to indicate that coefficients
//                  are not valid/valid
//
//  Inputs:         none
//
//
//  Outputs:
//
//
//
//
//
//
//  Notes:
/////////////////////////////////////////////////////////////////////////////////
#if 0
_reentrant void sw_stereocontrol_bwsweep(void)
#else

//sd_audio_B0M_i16,sd_audio_B0B_i16,sd_audio_B0S_u16 ;
//sd_audio_B1M_i16,sd_audio_B1B_i16,sd_audio_B1S_u16 ;
//sd_audio_A1M_i16,sd_audio_A1B_i16,sd_audio_A1S_u16;
//sd_audio_A2M_i16,sd_audio_A2B_i16,sd_audio_A2S_u16;
struct _sd_audio_data{
   signed short sd_audio_M_i16;
   signed short sd_audio_B_i16;
   unsigned short sd_audio_S_u16;
};

#ifdef PLAYER
 struct _sd_audio_data const_sd_audio_data[8][4] = {
#else
 struct _sd_audio_data _X  const_sd_audio_data[8][4] = {
#endif
{{24740, -9152,  2},  {24730, -9142, 2},   {31646,-15695, 2},     {19382,-12183, 4}},    //if(sw_stereocontrol.temp2_reg > 18000)
{{24740, -9152, 2},   {24730,-9142, 2},    {31646, -15695, 2},    {25631, -13661, 2}},   //(sw_stereocontrol.temp2_reg > 16000)
{{18291, -4470, 2},   {18461, -4597, 2},   { 31646, -15695, 2},   {30815, -12481, 1}},  //(sw_stereocontrol.temp2_reg > 14000)
{{18291,-4470, 2},    {18461, -4597, 2},   { 31646, -15695, 2},   {14379, -2068, 1}},    //(sw_stereocontrol.temp2_reg > 12000)
{{28433,-4506,1},     {28462,-4584,1},     {31646,-15695,2},      {-181,5875,1}},        //(sw_stereocontrol.temp2_reg > 10000)
{{28433, -4506,1},    {28462, -4584,1},    {31646,-15695, 2},     {-14811,12511,1}},       //(sw_stereocontrol.temp2_reg > 8000)
{{22102,-2209, 1},    {22089,-2205, 1},    {31646,-15695, 2},     {-31231,18468, 1}},      // (sw_stereocontrol.temp2_reg > 6000)
{{22102, -2209,1},    {22089,-2205,1},     {31646, -15695, 2},    { -24664,11698, 2}}     //esle
};

_reentrant void sw_stereocontrol_bwsweep(void)
{
  // ****************** Bandwidth Sweep Algorithm ************************
  // *** Calculate 2nd order filter coefficients as function of desired BW
  // We do this by constructing piece-wise linear filter coef's as f(BW),
  // which is why we break the calc's into different BW regions below.
  // coef(BW) = S*(M*BW + B)
  // For more info, see sw_audio/ws_filter.m checked into CVS

  // #### TBD - shift frequency by2048 and then use a switch/case statement to simplify the code below
     unsigned long temp;
     int index= 0;
    if (sw_stereocontrol.sd_audio_pCoefBypassBwCtl_u1 == 0)      // if ==1, then we just go with default coef set
    {
        sw_stereocontrol.temp2_reg = (signed short)sw_stereocontrol.sd_audio_LprBw_u16;

        if(sw_stereocontrol.temp2_reg > 18000)        {index = 0;}
        else if(sw_stereocontrol.temp2_reg > 16000) {index = 1; }
        else if(sw_stereocontrol.temp2_reg > 14000) { index = 2;}
        else if(sw_stereocontrol.temp2_reg > 12000) {index = 3;}
        else if(sw_stereocontrol.temp2_reg > 10000) { index = 4;}
        else if(sw_stereocontrol.temp2_reg > 8000)   {index = 5;}
        else if(sw_stereocontrol.temp2_reg > 6000)   { index = 6;}
        else                                                           {index = 7;}
        sw_stereocontrol.sd_audio_B0M_i16 = const_sd_audio_data[index][0].sd_audio_M_i16;
        sw_stereocontrol.sd_audio_B0B_i16 =  const_sd_audio_data[index][0].sd_audio_B_i16;
        sw_stereocontrol.sd_audio_B0S_u16 = const_sd_audio_data[index][0].sd_audio_S_u16;

        sw_stereocontrol.sd_audio_B1M_i16 = const_sd_audio_data[index][1].sd_audio_M_i16;
        sw_stereocontrol.sd_audio_B1B_i16 =  const_sd_audio_data[index][1].sd_audio_B_i16;
        sw_stereocontrol.sd_audio_B1S_u16 = const_sd_audio_data[index][1].sd_audio_S_u16;

        sw_stereocontrol.sd_audio_A1M_i16 = const_sd_audio_data[index][2].sd_audio_M_i16;
        sw_stereocontrol.sd_audio_A1B_i16 =  const_sd_audio_data[index][2].sd_audio_B_i16;
        sw_stereocontrol.sd_audio_A1S_u16 = const_sd_audio_data[index][2].sd_audio_S_u16;

        sw_stereocontrol.sd_audio_A2M_i16 = const_sd_audio_data[index][3].sd_audio_M_i16;
        sw_stereocontrol.sd_audio_A2B_i16 =  const_sd_audio_data[index][3].sd_audio_B_i16;
        sw_stereocontrol.sd_audio_A2S_u16 = const_sd_audio_data[index][3].sd_audio_S_u16;

        // The piece-wise linear eq's are based on a scaled version (32768/22050) of BW
        // These coef's are scaled for use in an IIR equation as follows:
        // y[n] = 2*(b0/2*x[n] + b1*x[n-1] + b0/2*x[n-2] + -a1*y[n-1] + -a2/2*y[n-2])
        temp = (((long)sw_stereocontrol.temp2_reg << 1)*(long)24348);      // Note 32768/22050 <-> 2*(16384/22050) <-> 2*((16384/22050)*32768) >> 15
		sw_stereocontrol.sd_audio_AdjBw_u16 = temp  >> 15 ;
		temp =  (long)sw_stereocontrol.sd_audio_B0M_i16*(long)sw_stereocontrol.sd_audio_AdjBw_u16;
        sw_stereocontrol.temp3_reg = temp >> 15;                      	 // temp = mx
		temp = 	(long)sw_stereocontrol.sd_audio_B0S_u16*((long)sw_stereocontrol.temp3_reg +(long) sw_stereocontrol.sd_audio_B0B_i16);
        sw_stereocontrol.sd_audio_LprB0_i16 = temp >> 1;   				 // y = S*(mx + b)
        g_sd_audio_LprB0_i16 = (temp >> 1) * gCOEF_SCALE;   		 // y = S*(mx + b)
        temp = ((long)sw_stereocontrol.sd_audio_B1M_i16*(long)sw_stereocontrol.sd_audio_AdjBw_u16);
        sw_stereocontrol.temp3_reg = temp >> 15;                     	 // temp = mx
        temp = (long)sw_stereocontrol.sd_audio_B1S_u16*((long)sw_stereocontrol.temp3_reg +(long) sw_stereocontrol.sd_audio_B1B_i16);
        sw_stereocontrol.sd_audio_LprB1_i16 =temp ;       				 // y = S*(mx + b)
        g_sd_audio_LprB1_i16 =temp * gCOEF_SCALE ;       			 // y = S*(mx + b)
        temp = ((long)sw_stereocontrol.sd_audio_A1M_i16*(long)sw_stereocontrol.sd_audio_AdjBw_u16);
        sw_stereocontrol.temp3_reg = temp >> 15;                      	 // temp = mx
        temp = (long)sw_stereocontrol.sd_audio_A1S_u16*((long)sw_stereocontrol.temp3_reg + (long)sw_stereocontrol.sd_audio_A1B_i16);
        sw_stereocontrol.sd_audio_LprA1_i16 = -(temp);    				 // y = S*(mx + b) -- negative coef so IIR is all MACs
        g_sd_audio_LprA1_i16 = (-(temp)) * gCOEF_SCALE;    			 // y = S*(mx + b) -- negative coef so IIR is all MACs
        temp = ((long)sw_stereocontrol.sd_audio_A2M_i16*(long)sw_stereocontrol.sd_audio_AdjBw_u16);
        sw_stereocontrol.temp3_reg = temp >> 15;                      	 // temp = mx
        temp = (long)sw_stereocontrol.sd_audio_A2S_u16*((long)sw_stereocontrol.temp3_reg + (long)sw_stereocontrol.sd_audio_A2B_i16);
        sw_stereocontrol.sd_audio_LprA2_i16 = -(temp >> 1); 			 // y = S*(mx + b) -- negative coef so IIR is all MACs
        g_sd_audio_LprA2_i16 = (-(temp >> 1 )) * gCOEF_SCALE; 		 // y = S*(mx + b) -- negative coef so IIR is all MACs
        // *** end LPR channel -- LPR coefficients now ready for Stereo Path next time

    }  // end if (sd_audio_pCoef_BypassBwCtl == 0)
    else
    {
        sw_stereocontrol.sd_audio_LprB0_i16 = DEFAULT_B0;   // -3dB cutoff = 17 kHz
        sw_stereocontrol.sd_audio_LprB1_i16 = DEFAULT_B1;   // -3dB cutoff = 17 kHz
        sw_stereocontrol.sd_audio_LprA1_i16 = DEFAULT_A1;   // -3dB cutoff = 17 kHz
        sw_stereocontrol.sd_audio_LprA2_i16 = DEFAULT_A2;   // -3dB cutoff = 17 kHz

        g_sd_audio_LprB0_i16 = DEFAULT_B0 * gCOEF_SCALE;   // -3dB cutoff = 17 kHz
        g_sd_audio_LprB1_i16 = DEFAULT_B1 * gCOEF_SCALE;   // -3dB cutoff = 17 kHz
        g_sd_audio_LprA1_i16 = DEFAULT_A1 * gCOEF_SCALE;   // -3dB cutoff = 17 kHz
        g_sd_audio_LprA2_i16 = DEFAULT_A2 * gCOEF_SCALE;   // -3dB cutoff = 17 kHz
    }


     // Stereo Matrix part previously it was in stereopath.c file
     // All values sacled by 0.5 to pre-adjust for factor of 2 in "matrix" computations
     //gLPR_Mult_fract  = 1.0;
     //gLMR_Mult_fract  = 1.0;

    gLPR_Mult_fract  = _int2fract((sw_stereocontrol.sd_audio_LprGa_u16 * gCOEF_SCALE));   // SoftMute applied to LPR
    gLMR_Mult_fract  = _int2fract((sw_stereocontrol.sd_audio_LmrGa_u16 * gCOEF_SCALE));   // SoftMute applied to LMR

	if (g_sd_audio_pCoefForcedMono_u1)
    {
        gLMR_Mult_fract = 0.0;                      // Forced Mono
    }
    if (g_sd_audio_pCoefBypassBlend_u1)
    {
        gLMR_Mult_fract =  0.99;                    // No Blend
    }

    if (g_sd_audio_pCoefBypassSoftmute_u1)
    {
        gLPR_Mult_fract =  0.99;
    }
    //*****************************************************************
   //  * Stmp00011112 We like to increase the gain by 1.6 time which can be achieved by
  //   * 0.8 * 2
  //   * The 2-times change is in sw_stereopath_xcall_asm.asm
   //  *****************************************************************
    gLMR_Mult_fract *=  0.8;
    gLPR_Mult_fract *=  0.8;
   //****************************************************************
    IIRFiltCoeffSemaphore = 1;  // Filter coefficients are now valid!
}

#endif
#pragma asm
     org p,".pSTFM1000_NO_OVERLAY_P":
#pragma endasm

//Move from funclet
_reentrant INT STFM1000SetLnaBias(INT iRssiLog)
{

    DWORD dwShadowAgcStatReg;
    DWORD dwBias1Temp;
    DWORD dwAgcOut;
    DWORD dwLnaRms,dwLnaAmp1ImproveDist;
    INT RetCode = SUCCESS;
    STFM1000Tune_t * pDrst = &StfmTuneData;


     /* read AGC_STAT register */
    RetCode = STFM1000ReadRegisterToShadow(AGC_STAT_REG_ADDR,
                                            &dwShadowAgcStatReg);
    if (SUCCESS != RetCode)
    {
        return RetCode;
    }
    /* extract AgcOut */
    STFM1000ReadShadowRegister(AGCOUT_STAT_REG_MASK, &dwShadowAgcStatReg,
            &dwAgcOut);
    // Bit[1] of ShadowLna
    dwBias1Up = (0 != (pDrst->sd_ctl_ShadowLna_dw & 0x00000002)) ? (1):(0);
    // Bit[0] of ShadowLna
    dwBias1Down = (0 != (pDrst->sd_ctl_ShadowLna_dw & 0x00000001)) ? (1):(0);
    /* extract LnaRms */
    STFM1000ReadShadowRegister(LNA_RMS_REG_MASK, &dwShadowAgcStatReg,
            &dwLnaRms);

    // Bit[23] of ShadowRef
    dwLnaAmp1ImproveDist = (0 != (pDrst->sd_ctl_ShadowRef_dw & 0x00800000)) ? (1):(0);  // bit 23

    if (31 == dwAgcOut)
    {

        if (VERSION_TA2 == StfmTuneDefault.STFM1000_Version)
            {
            RetCode = STFM1000WriteRegister(
                AGC_CONTROL1_REG_ADDR,
                B2_AGCREF_REG_MASK,
                &pDrst->sd_ctl_ShadowAgcControl1_dw,
                10);
            if (SUCCESS != RetCode)
                {
                //SystemHalt();
                return RetCode;
                }
            } // end if TA2
         else  // must be version TB2
            {
            RetCode = STFM1000WriteRegister(
                AGC_CONTROL1_REG_ADDR,
                0x00003F00,          // mask for B2_DB_WB_REF_REG
                &pDrst->sd_ctl_ShadowAgcControl1_dw,
                42);
            if (SUCCESS != RetCode)
                {
                //SystemHalt();
                return RetCode;
                }
            } // end version TB2

        if(18 >= iRssiLog)
        {
            if(dwLnaRms==0)
            {
                if(dwBias1Down)
                {
                    dwBias1Down = 0;
                }
                else
                {
                    dwBias1Up = 1;
                    dwLnaAmp1ImproveDist = 0;
                }
            }
        }
        if(28 <= iRssiLog)  // was 24
        {
            if(dwBias1Up)
            {
                dwBias1Up = 0;
                dwLnaAmp1ImproveDist = 1;
            }
            else
            {
                if (VERSION_TA2 == StfmTuneDefault.STFM1000_Version)
                    {
                    RetCode = STFM1000WriteRegister(
                        AGC_CONTROL1_REG_ADDR,
                        B2_AGCREF_REG_MASK,
                        &pDrst->sd_ctl_ShadowAgcControl1_dw,
                        7);
                    if (SUCCESS != RetCode)
                        {
                        //SystemHalt();
                        return RetCode;
                        }
                     } // end if TA2
                 else  // must be version TB2
                    {
                    RetCode = STFM1000WriteRegister(
                        AGC_CONTROL1_REG_ADDR,
                        0x00003F00,          // mask for B2_DB_WB_REF_REG
                        &pDrst->sd_ctl_ShadowAgcControl1_dw,
                        34);
                    if (SUCCESS != RetCode)
                        {
                        //SystemHalt();
                        return RetCode;
                        }
                    } // end version TB2

                dwBias1Down = 1;
            }

        }
    }
    else  // (31 != dwAgcOut)
    {
        if (VERSION_TA2 == StfmTuneDefault.STFM1000_Version)
            {
            RetCode = STFM1000WriteRegister(
                AGC_CONTROL1_REG_ADDR,
                B2_AGCREF_REG_MASK,
                &pDrst->sd_ctl_ShadowAgcControl1_dw,
                7);
            if (SUCCESS != RetCode)
                {
                //SystemHalt();
                return RetCode;
                }
             }  // end if TA2
         else  // must be version TB2
            {
            RetCode = STFM1000WriteRegister(
                AGC_CONTROL1_REG_ADDR,
                0x00003F00,          // mask for B2_DB_WB_REF_REG
                &pDrst->sd_ctl_ShadowAgcControl1_dw,
                34);
            if (SUCCESS != RetCode)
                {
                //SystemHalt();
                return RetCode;
                }
            } // end version TB2

        dwBias1Up = 0;
        dwBias1Down = 1;
        dwLnaAmp1ImproveDist = 1;
    }
    if (VERSION_TB2 == StfmTuneDefault.STFM1000_Version)
    {
        RetCode = STFM1000WriteRegister(
            REF_REG_ADDR,
            LNA_AMP1_IMPROVE_DISTORTION,
            &pDrst->sd_ctl_ShadowRef_dw,
            dwLnaAmp1ImproveDist);

        if (SUCCESS != RetCode)
        {
            return RetCode;
        }
    }
    dwBias1Temp = ((dwBias1Up<<1) | dwBias1Down) & (0x00000003);
    RetCode = STFM1000WriteRegister(
        LNA_REG_ADDR,
        (IBIAS1_UP_MASK | IBIAS1_DN_MASK),
        &pDrst->sd_ctl_ShadowLna_dw,
        dwBias1Temp);

    if (SUCCESS != RetCode)
    {
        return RetCode;
    }
    return RetCode;
}
//move from funclet
//Move from funclet
_reentrant INT STFM1000LookForPilot(BOOL * bPilotPresent)
{
    INT iCount;
    DWORD dwPiloTest;
    DWORD dwPiloTestLowerLimit = 0x1E;  			//TB2  +30 (0x1E)
    DWORD dwPiloTestUpperLimit = 0x7F;  //TB2
    DWORD dwPiloTestMask = PILOTEST_REG_TB2_MASK;   //VERSION_TA2

    STFM1000Tune_t * pDrst = &StfmTuneData;
    INT RetCode = SUCCESS;

    if (VERSION_TA2 == StfmTuneDefault.STFM1000_Version)
    {
        dwPiloTestMask = PILOTEST_REG_TA2_MASK;
        // Range changed to [-75, -30] on 11/03/2006
        dwPiloTestLowerLimit = 0xE2;     // TA2  -30(0xE2)
        dwPiloTestUpperLimit = 0xB5;     // TA2  -75(0xB5)

        /* Let's check the pilot for 3 times, and
           declare pilot present if value is within range all 3 times, */
        for (iCount=3; iCount>0; --iCount)
        {
            // get pilot level

            RetCode = STFM1000ReadSubRegister(
                PILOTCORRECTION_REG_ADDR,
                dwPiloTestMask,
                &dwPiloTest);

            if (SUCCESS != RetCode)
            {
                 //SystemHalt();
                 return RetCode;
            }

            if (g_STFM1000_CustomerForceMono == TRUE)   // STMP00013255
            {
                dwPiloTest = 0;
            }

            // compare pilot level to threshold
            // (value is positive so "ABS()" not needed)
            // -75<= dwPilotTest<-30
            if ((dwPiloTestUpperLimit < dwPiloTest) && (dwPiloTest < dwPiloTestLowerLimit))
            {
                continue;
            }
            else
            {
                // turn off pilot tracking on chip
                RetCode = STFM1000WriteRegister(
                    PILOTTRACKING_REG_ADDR,
                    B2_PILOTTRACKING_ENABLE_REG_MASK,
                    &pDrst->sd_ctl_ShadowPilotTracking_dw,
                    0);

                if (SUCCESS != RetCode)
                {
                     return RetCode;
                }

                *bPilotPresent = FALSE; // result to the calling function
                g_sd_audio_pCoefForcedMono_u1 = TRUE;
                break;
            } // pilot level below threshold
        }
        /* Pilot check passed every time in the preceding loop ? */
        if (iCount <= 0)
        {
            *bPilotPresent = TRUE; // result to the calling function
        }

    }
    else //TB2
    {
        /* Let's check the pilot for 3 times, and
           declare pilot present if value is within range all 3 times, */
        for (iCount=3; iCount>0; --iCount)
        {
            // get pilot level

            RetCode = STFM1000ReadSubRegister(
                PILOTCORRECTION_REG_ADDR,
                dwPiloTestMask,
                &dwPiloTest);

            if (SUCCESS != RetCode)
            {
                 //SystemHalt();
                 return RetCode;
            }

            if (g_STFM1000_CustomerForceMono == TRUE)
            {
                dwPiloTest = 0;
            }

            // compare pilot level to threshold
            // (value is positive so "ABS()" not needed)
            // 30<=dwPilotTest<127
            if ((dwPiloTestLowerLimit < dwPiloTest) &&
                 (dwPiloTest < dwPiloTestUpperLimit))
            {
                continue;
            }
            else
            {
                 // turn off pilot tracking on chip
                RetCode = STFM1000WriteRegister(
                    PILOTTRACKING_REG_ADDR,
                    B2_PILOTTRACKING_ENABLE_REG_MASK,
                    &pDrst->sd_ctl_ShadowPilotTracking_dw,
                    0);

                if (SUCCESS != RetCode)
                {
                     //SystemHalt();
                     return RetCode;
                }

                *bPilotPresent = FALSE; // result to the calling function

                /* enable "forced mono" in black box */
                g_sd_audio_pCoefForcedMono_u1 = TRUE;
                break;

            } // if ((dwPiloTestLowerLimit < dwPiloTest) ...
        }
        /* Pilot check passed every time in the preceding loop ? */
        if (iCount <= 0)
        {
            *bPilotPresent = TRUE; // result to the calling function
        }
    }
    pDrst->sd_ctl_PilotPresent_b = *bPilotPresent;
    return RetCode;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          TunerGetStatus
//
//   Type:          Function
//
//   Description:   Check if tuning is done
//
//   Inputs:        none
//
//   Outputs:       Error status
//
//   Notes:         This function is called whenever the tuner is return E_TUNER_BUSY
//                  which is used to release the control back to the system when waiting
//                  for the settling period for tuning, optimizing channel, or monitoring
//                  the signal quality
//<
////////////////////////////////////////////////////////////////////////////////
_reentrant INT TunerGetStatus( void )
{
    // The timer could be E_TUNER_BUSY by tuning or optimizing channel and monitoring
    // the signal quality. In the last two cases, the tuner should be not in busy state
    // In those cases, if a message comes, we should ignor timer and handle the message
    // The tuning step:
    // 1. Tune to a frequency
    // 2. Optimize the channel after tuned, when complete, send a message to menu information
    //    the completion
    // 3. Minitor the signal quality every second
    INT iStatus;

    if(g_wFMTunerError.m_bTUNER_BUSY == TRUE)
    {
        iStatus = STFM1000TuningSearchingScanning();
        return(iStatus);
    }
    else
    {
        // We are in the middle of channel optimization, so discard the signal montor quality
        // since we only monitor the signal after the station is settled
        if(s_eOptimizeChannelState != OPTIMIZE_CHANNEL_STATE_COMPLETE)
        {
            // If we just received a message to start monitoring the signal quality
            // so discard the prevoius operation, start over
            if(s_eOptimizeChannelState == OPTIMIZE_CHANNEL_STATE_START)
            {
                return TUNER_SUCCESS;

            }
            else
            {
                return STFM1000OptimizeChannel();
            }
        }
        else
        {
            // warning: HJ made this change to use #else but DW concerned maybe we need #if case to run.
#ifdef STFM1000_ENABLE_MONITOR_SIGNAL_QUALITY
            // If we just received a message to start monitoring the signal quality
            // so discard the prevoius operation, start over
            if(s_eMonitorSignalQualityState == MONITOR_SIGNAL_QUALITY_STATE_START)
            {
                return TUNER_SUCCESS;
            }
            else
            {
                return (STFM1000TunerMonitorQuality());
            }
#else
                return TUNER_SUCCESS;
#endif
        }
    }
}

// New function Nov 2006
// Input: Input param is a pointer to prelim pilot check result. Retcode returned also for err status.
_reentrant INT STFM1000PrelimPilotCheck(BOOL * bPrelimPilot)
{
    INT iCount;
    DWORD dwPiloTest;
    DWORD dwPiloTestLowerLimit = 0x19;  			//TB2  +25(0x19)
    DWORD dwPiloTestUpperLimit = 0x7F;  			//TB2  +127(0x7F)
    DWORD dwPiloTestMask = PILOTEST_REG_TB2_MASK;   	//VERSION_TB2

    STFM1000Tune_t * pDrst = &StfmTuneData;
    INT RetCode = SUCCESS;

    if (VERSION_TA2 == StfmTuneDefault.STFM1000_Version)
    {
        dwPiloTestMask = PILOTEST_REG_TA2_MASK;
        dwPiloTestLowerLimit = 0x19;     // TA2  +25(0x19)
        dwPiloTestUpperLimit = 0xE7;     // TA2  -25(0xE7)
    }  // end if TA2

        /* Let's check the pilot 5 times, and declare "pilot possible"
	     if value falls outside range even once */
        for (iCount=5; iCount>0; --iCount)
        {
            // get pilot level
            RetCode = STFM1000ReadSubRegister(
                PILOTCORRECTION_REG_ADDR,
                dwPiloTestMask,
                &dwPiloTest);

            if (SUCCESS != RetCode)
            {
                 //SystemHalt();
                 return RetCode;
            }
            // compare pilot level to threshold
            // (value is positive so "ABS()" not needed)
            // -25<=dwPilotTest<+25  for TA2   ---   0<=dwPilotTest<+25  for TB2
            if ((dwPiloTest < dwPiloTestLowerLimit) ||
               ((dwPiloTestUpperLimit < dwPiloTest) && (dwPiloTest <= 0xFF)))
            {
                continue;
            }
            else
            {
                *bPrelimPilot = TRUE; // result to the calling function
                break;
            } // if ((dwPiloTest < dwPiloTestLowerLimit) ...
        }
        /* Pilot check is "small" every time in the preceding loop? */
        if (iCount <= 0)
        {
            *bPrelimPilot = FALSE; // result to the calling function
        }



    return RetCode;
}
_reentrant INT STFM1000TunerMonitorQuality(void)
{

    DWORD dwShadowToneReg;
    WORD wRssiMantissa, wRssiExponent;
    WORD wPrssi;
    INT iRssiLog;
    DWORD dwRssiDecoded;
    DWORD dwTemp;
    DWORD dwShadowAgcStatReg;
    DWORD dwAgcOut, dwLnaRms, dwLnaTh;
    BOOL bWriteFlag;
    STFM1000Tune_t * pDrst = &StfmTuneData;
    INT iRetCode = SUCCESS;

    switch(s_eMonitorSignalQualityState)
    {
        case MONITOR_SIGNAL_QUALITY_STATE_START:
            iRetCode = STFM1000TunerMonitorQualityStateStart();
            break;
        case MONITOR_SIGNAL_QUALITY_STATE_PROCESSING_1:

            STFM1000GetNearChannelOffset();
            if ( pDrst->sd_ctl_ShadowSignalQuality_w < pDrst->sd_ctl_AdjChanTh_w)
            {
                iRetCode = STFM1000SetNearChannelOffset(5);

                g_iTunerProcessIntervalMs = 10;
                s_eMonitorSignalQualityState = MONITOR_SIGNAL_QUALITY_STATE_PROCESSING_2;
                iRetCode = E_TUNER_BUSY;
            }
            else
            {
                iRetCode = STFM1000SetNearChannelFilter(0);
                if ((TRUE == pDrst->sd_ctl_PilotPresent_b) && (TRUE == bIsWeakSignal))
                {
                    s_wTrackTableIndex = 0;
                    iRetCode = STFM1000GearShiftPtGear(STFM1000TrackTable[s_wTrackTableIndex].dw_bits);

                    g_iTunerProcessIntervalMs = STFM1000TrackTable[s_wTrackTableIndex].dw_msec_delay;
                    s_eMonitorSignalQualityState = MONITOR_SIGNAL_QUALITY_STATE_PROCESSING_3;
                    iRetCode =  E_TUNER_BUSY;
                }
                else
                {
                    iRetCode = STFM1000TunerMonitorQualityStateFinal();
                }
            }
            break;
       case MONITOR_SIGNAL_QUALITY_STATE_PROCESSING_2:
            STFM1000GetNearChannelOffset();

            if ( pDrst->sd_ctl_ShadowSignalQuality_w < pDrst->sd_ctl_AdjChanTh_w)
            {
                iRetCode = STFM1000SetNearChannelFilter(2);

                if ((TRUE == pDrst->sd_ctl_PilotPresent_b) && (TRUE == bIsWeakSignal))
                {
                    s_wTrackTableIndex = 0;
                    iRetCode = STFM1000GearShiftPtGear(STFM1000TrackTable[s_wTrackTableIndex].dw_bits);

                    g_iTunerProcessIntervalMs = STFM1000TrackTable[s_wTrackTableIndex].dw_msec_delay;
                    s_eMonitorSignalQualityState = MONITOR_SIGNAL_QUALITY_STATE_PROCESSING_3;
                    iRetCode =  E_TUNER_BUSY;
                }
                else
                {
                    iRetCode = STFM1000TunerMonitorQualityStateFinal();
                }
            }
            else
            {
                iRetCode = STFM1000SetNearChannelFilter(0);
                if ((TRUE == pDrst->sd_ctl_PilotPresent_b) && (TRUE == bIsWeakSignal))
                {
                    s_wTrackTableIndex = 0;
                    iRetCode = STFM1000GearShiftPtGear(STFM1000TrackTable[s_wTrackTableIndex].dw_bits);

                    g_iTunerProcessIntervalMs = STFM1000TrackTable[s_wTrackTableIndex].dw_msec_delay;
                    s_eMonitorSignalQualityState = MONITOR_SIGNAL_QUALITY_STATE_PROCESSING_3;
                    iRetCode =  E_TUNER_BUSY;
                }
                else
                {
                    iRetCode = STFM1000TunerMonitorQualityStateFinal();
                }
            }
            break;
        case MONITOR_SIGNAL_QUALITY_STATE_PROCESSING_3:
            s_wTrackTableIndex++;
            if (/* (0 != STFM1000TrackTable[s_wTrackTableIndex].dw_msec_delay)
                 ||*/ (0 != STFM1000TrackTable[s_wTrackTableIndex].dw_bits) )
            {
                iRetCode = STFM1000GearShiftPtGear(STFM1000TrackTable[s_wTrackTableIndex].dw_bits);
                if(STFM1000TrackTable[s_wTrackTableIndex].dw_msec_delay == 0)
                {
                    /* configure "forced mono" in black box */
                    g_sd_audio_pCoefForcedMono_u1 = 0;
                    bIsWeakSignal = FALSE;

                    iRetCode = STFM1000TunerMonitorQualityStateFinal();
                }
                else
                {
                    g_iTunerProcessIntervalMs = STFM1000TrackTable[s_wTrackTableIndex].dw_msec_delay;
                    s_eMonitorSignalQualityState = MONITOR_SIGNAL_QUALITY_STATE_PROCESSING_3;
                    iRetCode =  E_TUNER_BUSY;
                }
            }
            break;
        default:
            break;
     }
    return iRetCode;

}
//
_reentrant INT STFM1000TunerMonitorQualityStateStart(void)
{
    DWORD dwShadowToneReg;
    //WORD wRssiMantissa, wRssiExponent;
    //WORD wPrssi;
    //INT iRssiLog;
    //DWORD dwRssiDecoded;
    DWORD dwTemp;
    DWORD dwShadowAgcStatReg;
    DWORD dwAgcOut, dwLnaRms, dwLnaTh;
    BOOL bWriteFlag;
    DWORD dwTempInit0 = 0;
    DWORD dwTempLna = 9;
    STFM1000Tune_t * pDrst = &StfmTuneData;
    INT iRetCode = SUCCESS;
    BOOL bPilotPresent = FALSE;
    BOOL bPrelimPilotCheck = FALSE;

    // **** Following section gives an occasional opportunity to the system
    // to recover stereo after confidence has been exhausted.
    // This helps the system eventually recover from brief multipath,
    // which might otherwise decrease confidence and forever force mono.
    count_MonitorQuality+=1;
    if (count_MonitorQuality>300)    // value indicates #sec to "wait" before trying again
        {
	  count_MonitorQuality = 0;   // reset counter
        wSd_ctl_Confidence_u16 = 3; // reset confidence
        }
     //****

    /* enable buffered writes */
    iRetCode = STFM1000WriteRegister(
            DATAPATH_REG_ADDR,
            DB_ACCEPT_REG_MASK,
            &pDrst->sd_ctl_ShadowDataPath_dw,
            1);
    if (SUCCESS != iRetCode)
    {
        //SystemHalt();
        return iRetCode;
    }

    /* Get Rssi register readings from STFM1000 */
    iRetCode = STFM1000ReadRegisterToShadow(RSSI_TONE_REG_ADDR,
                                            &dwShadowToneReg);
    if (SUCCESS != iRetCode)
    {
        //SystemHalt();
        return iRetCode;
    }
//******************** No longer using direct-read RSSI_DCEST,
//******************** rather we're using attack-decay-filtered RSSI
#if 0
    STFM1000ReadShadowRegister(RSSI_DCEST_REG_MASK, &dwShadowToneReg,
            &dwTemp);
    pDrst->sd_ctl_ShadowRssiDcEst_w = dwTemp & 0xffff;

    /* decode RSSI */
    wRssiMantissa = (pDrst->sd_ctl_ShadowRssiDcEst_w & 0xFFE0)>>5;// 11 msb
    wRssiExponent = (pDrst->sd_ctl_ShadowRssiDcEst_w & 0x001F);   // 5 lsb
    dwRssiDecoded = wRssiMantissa << wRssiExponent;

    /* Convert Rssi to 10log(Rssi) */
    for (wPrssi = 20; wPrssi >0; --wPrssi)
    {
        if (dwRssiDecoded >= (1 << wPrssi))
        {
            break;
        }
    }

    iRssiLog = (3*dwRssiDecoded >> wPrssi) + (3*wPrssi - 3);

    /* clamp to positive */
    if (iRssiLog < 0)
    {
        iRssiLog = 0;
    }

    /* Compensate for errors in truncation/approximation by adding 1 */
    iRssiLog += 1;
#endif
//********************

    // Save the value for sending across to UI later
    //pDrst->sd_ctl_ShadowRssiDcEstLog_w = (WORD)iRssiLog;
    //s_iFMSignalStrengthRSSI = iRssiLog;
    pDrst->sd_ctl_ShadowRssiDcEstLog_w = (WORD)global_filtered_rssi;
    s_iFMSignalStrengthRSSI = global_filtered_rssi;

    //if(iRssiLog < (sw_stereocontrol.sd_audio_pCoefLmrGaTh_u16 - 20))
    if(global_filtered_rssi < (sw_stereocontrol.sd_audio_pCoefLmrGaTh_u16 - 20))
    {
       if(TRUE == pDrst->sd_ctl_PilotPresent_b)
       {
            dwTempInit0 = 1;
       }
        /* configure filter for narrow band */
        iRetCode = STFM1000WriteRegister(
            INITIALIZATION1_REG_ADDR,
            B2_BYPASS_FILT_REG_MASK,
            &pDrst->sd_ctl_ShadowInitialization1_dw,
            dwTempInit0);

        if (SUCCESS != iRetCode)
        {
            //SystemHalt();
            return iRetCode;
        }

        /* Turn off pilot tracking */
        iRetCode = STFM1000WriteRegister(
            PILOTTRACKING_REG_ADDR,
            B2_PILOTTRACKING_ENABLE_REG_MASK,
            &pDrst->sd_ctl_ShadowPilotTracking_dw,
            0);

        if (SUCCESS != iRetCode)
        {
            //SystemHalt();
            return iRetCode;
        }

        /* enable "forced mono" in black box */
        g_sd_audio_pCoefForcedMono_u1 = 1;
        bIsWeakSignal = TRUE;
        pDrst->bPrevPilotPresent = 0;

        //STFM1000SetLnaBias(iRssiLog);
        STFM1000SetLnaBias(global_filtered_rssi);
    }
    else   // else, strong signal side
    {
        if (VERSION_TA2 == StfmTuneDefault.STFM1000_Version)
            {
            iRetCode = STFM1000WriteRegister(
                AGC_CONTROL1_REG_ADDR,
                B2_AGCREF_REG_MASK,
                &pDrst->sd_ctl_ShadowAgcControl1_dw,
                7);
            if (SUCCESS != iRetCode)
                {
                //SystemHalt();
                return iRetCode;
                }
             } // end if TA2
         else  // must be version TB2
            {
            iRetCode = STFM1000WriteRegister(
                AGC_CONTROL1_REG_ADDR,
                0x00003F00,          // mask for B2_DB_WB_REF_REG
                &pDrst->sd_ctl_ShadowAgcControl1_dw,
                34);
            if (SUCCESS != iRetCode)
                {
                //SystemHalt();
                return iRetCode;
                }
            } // end version TB2


        //if (iRssiLog > (sw_stereocontrol.sd_audio_pCoefLmrGaTh_u16 - 17))
        if (global_filtered_rssi > (sw_stereocontrol.sd_audio_pCoefLmrGaTh_u16 - 17))
        {
            iRetCode = STFM1000WriteRegister(
                LNA_REG_ADDR,
                0x0000000F,
                &pDrst->sd_ctl_ShadowLna_dw,
                0x00000009);

            if (SUCCESS != iRetCode)
            {
                //SystemHalt();
                return iRetCode;
            }

            // STFM1000SetNearChannelOffset(1); // call SetChannelFilter
            STFM1000SetNearChannelFilter(2);

            iRetCode = STFM1000PrelimPilotCheck(&bPrelimPilotCheck);
            if (SUCCESS != iRetCode)
            {
                 return iRetCode;
            }

            if (bPrelimPilotCheck)
            {

                 iRetCode = STFM1000LookForPilot(&bPilotPresent);
                 if (SUCCESS != iRetCode)
                 {
                      return iRetCode;
                 }

                 if (!bPilotPresent)
                 {
                    if(wSd_ctl_Confidence_u16 > 0)
                    {
                        s_eOptimizeChannelState = OPTIMIZE_CHANNEL_STATE_START;

                        while(E_TUNER_BUSY == STFM1000OptimizeChannel())
                        {
                        }

                        iRetCode = STFM1000LookForPilot(&bPilotPresent);
                        if (SUCCESS != iRetCode)
                        {
                             return iRetCode;
                        }
                        if (bPilotPresent)
                        {
				    if(wSd_ctl_Confidence_u16 > 0)  // only allow stereo if "confident"
				        {
                                g_sd_audio_pCoefForcedMono_u1 = 0;
				        }
                        }  // end if (bPilotPresent)
                        else
                        {
                            wSd_ctl_Confidence_u16--;
                            // turn off pilot tracking on chip
                            iRetCode = STFM1000WriteRegister(
                                PILOTTRACKING_REG_ADDR,
                                B2_PILOTTRACKING_ENABLE_REG_MASK,
                                &pDrst->sd_ctl_ShadowPilotTracking_dw,
                                0);
                            if (SUCCESS != iRetCode)
                            {
                                 return iRetCode;
                            }

                            /* enable "forced mono" in black box */
                            g_sd_audio_pCoefForcedMono_u1 = 1;
                        }
                    }
                    else  // if (wSd_ctl_Confidence_u16 > 0)
                    {
                        // turn off pilot tracking on chip
                        iRetCode = STFM1000WriteRegister(
                            PILOTTRACKING_REG_ADDR,
                            B2_PILOTTRACKING_ENABLE_REG_MASK,
                            &pDrst->sd_ctl_ShadowPilotTracking_dw,
                            0);
                        if (SUCCESS != iRetCode)
                        {
                             return iRetCode;
                        }

                        /* enable "forced mono" in black box */
                        g_sd_audio_pCoefForcedMono_u1 = 1;
                    }
                }
                else // if (!bPilotPresent)
                {
			  if (wSd_ctl_Confidence_u16 > 0)  // only allow stereo if "confident"
			      {
                        // if pilot just discovered OR signal was weak and now is strong,
			      // then gear-shift
                        if ((!pDrst->bPrevPilotPresent) || (bIsWeakSignal))
                            {
                            s_eOptimizeChannelState = OPTIMIZE_CHANNEL_STATE_START;

                            while(E_TUNER_BUSY == STFM1000OptimizeChannel())
                            {
                            }
                            g_sd_audio_pCoefForcedMono_u1 = 0;
                            }  // end if ((!pDrst->bPrevPilotPresent) || (bIsWeakSignal))

                        // be sure pilot tracking is enabled and in "low gear"

                        iRetCode = STFM1000WriteRegister(

                            PILOTTRACKING_REG_ADDR,

                            0x0000ffff,

                            &pDrst->sd_ctl_ShadowPilotTracking_dw,

                            0x8810);

                        if (SUCCESS != iRetCode)

                        {

                             return iRetCode;

                        }



                        // allow stereo

                        g_sd_audio_pCoefForcedMono_u1 = 0;
				}  // end if (wSd_ctl_Confidence_u16 > 0)
                }  // end if (!bPilotPresent)
            }
            else  // if (bPrelimPilotCheck)
            {
                // turn off pilot tracking on chip
                iRetCode = STFM1000WriteRegister(
                    PILOTTRACKING_REG_ADDR,
                    B2_PILOTTRACKING_ENABLE_REG_MASK,
                    &pDrst->sd_ctl_ShadowPilotTracking_dw,
                    0);
                if (SUCCESS != iRetCode)
                {
                     return iRetCode;
                }

                /* enable "forced mono" in black box */
                g_sd_audio_pCoefForcedMono_u1 = 1;
            }

            g_iTunerProcessIntervalMs = 10;
            s_eMonitorSignalQualityState = MONITOR_SIGNAL_QUALITY_STATE_PROCESSING_1;
            iRetCode = E_TUNER_BUSY;
            bIsWeakSignal = FALSE;
            pDrst->bPrevPilotPresent = bPilotPresent;   // update for use next time
        }

        else   // if (iRssiLog > (sw_stereocontrol.sd_audio_pCoefLmrGaTh_u16 - 17))
        {

            iRetCode = STFM1000LookForPilot(&bPilotPresent);
            if (SUCCESS != iRetCode)
            {
                 return iRetCode;
            }

            if (!bPilotPresent)
            {
                // turn off pilot tracking on chip
                iRetCode = STFM1000WriteRegister(
                    PILOTTRACKING_REG_ADDR,
                    B2_PILOTTRACKING_ENABLE_REG_MASK,
                    &pDrst->sd_ctl_ShadowPilotTracking_dw,
                    0);
                if (SUCCESS != iRetCode)
                {
                     return iRetCode;
                }

                /* enable "forced mono" in black box */
                g_sd_audio_pCoefForcedMono_u1 = 1;
                bIsWeakSignal = FALSE;
                pDrst->bPrevPilotPresent = bPilotPresent;   // update for use next time

            }  // end if (!bPilotPresent)
        } // end else if not (iRssiLog > (sw_stereocontrol.sd_audio_pCoefLmrGaTh_u16 - 17))


    }  // end "strong signal side"
    iRetCode = STFM1000TunerMonitorQualityStateFinal();
    return iRetCode;
}
_reentrant INT STFM1000TunerMonitorQualityStateFinal(void)
{
    DWORD dwTemp;
    DWORD dwShadowAgcStatReg;
    DWORD dwAgcOut, dwLnaRms, dwLnaTh;
    BOOL bWriteFlag;
    STFM1000Tune_t * pDrst = &StfmTuneData;
    INT iRetCode = SUCCESS;

    if (VERSION_TA2 == StfmTuneDefault.STFM1000_Version)
    {
        /* read AGC_STAT register */
        iRetCode = STFM1000ReadRegisterToShadow(AGC_STAT_REG_ADDR,
                                                &dwShadowAgcStatReg);

        if (SUCCESS != iRetCode)
        {
            //SystemHalt();
             return iRetCode;
        }

        /* extract AgcOut */
        STFM1000ReadShadowRegister(AGCOUT_STAT_REG_MASK, &dwShadowAgcStatReg,
                &dwAgcOut);

        /* extract LnaRms */
        STFM1000ReadShadowRegister(LNA_RMS_REG_MASK, &dwShadowAgcStatReg,
                &dwLnaRms);

        if (VERSION_TB2 == StfmTuneDefault.STFM1000_Version)
        {
             dwLnaTh = 12;
        }   //VERSION_TB2
        else
        {
            /* read AGC_CONTROL1 register */
            iRetCode = STFM1000ReadRegisterToShadow(AGC_CONTROL1_REG_ADDR,
                    &pDrst->sd_ctl_ShadowAgcControl1_dw);

            if (SUCCESS != iRetCode)
            {
                //SystemHalt();
                return iRetCode;
            }

            /* extract LNATH */
            STFM1000ReadShadowRegister(B2_LNATH_REG_MASK,
                    &pDrst->sd_ctl_ShadowAgcControl1_dw, &dwLnaTh);
        }

        /* Reset Write Flag */
        bWriteFlag = FALSE;
        dwTemp = 0;

        if ( (dwLnaRms > dwLnaTh) && (dwAgcOut <= 1) )
        {
            dwTemp = 1;
            /* Set Write Flag */
            bWriteFlag = TRUE;
        }
        else
        {
            if (dwAgcOut > 15)
            {
                dwTemp = 0;
                /* Set Write Flag */
                bWriteFlag = TRUE;
            }
        }

        /* Is Write Flag Set? */
        if (bWriteFlag == TRUE)
        {
            iRetCode = STFM1000WriteRegister(
                LNA_REG_ADDR,
                USEATTEN_MASK,
                &pDrst->sd_ctl_ShadowLna_dw,
                dwTemp);

            if (SUCCESS != iRetCode)
            {
                //SystemHalt();
                 return iRetCode;
            }
        }
    }   //VERSION_TA2
    /* disable buffered writes */
    iRetCode = STFM1000WriteRegister(
        DATAPATH_REG_ADDR,
        DB_ACCEPT_REG_MASK,
        &pDrst->sd_ctl_ShadowDataPath_dw,
        0);
    if (SUCCESS != iRetCode)
    {
        //SystemHalt();
        return iRetCode;
    }
    s_eMonitorSignalQualityState = MONITOR_SIGNAL_QUALITY_STATE_COMPLETE;
    return iRetCode;

}
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          STFM1000ContinueScanning
//
//   Type:          Function
//
//   Description:   Scanning until complete
//
//   Inputs:        none
//
//   Outputs:       Busy or SUCCESS
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
_reentrant INT STFM1000ContinueScanning( void )
{
	INT iTuningStatus;

	if ((s_iFMSignalStrengthRSSI > StfmTuneData.sd_ctl_TuneRssiTh_w ) &&
        (s_wCurrentFMFrequency >= FM_BAND_LIMIT_LOW_FREQ) &&
        (s_wCurrentFMFrequency <= FM_BAND_LIMIT_HIGH_FREQ)) // test for valid station, IF count in the range and Level is OK
	{
		SysPostMessage(2,MENU_TUNER_TUNED);
		if (s_iFMPresetCount < NUMBER_OF_PRESETS)
		{
#ifdef SD_USE_50KHZ_TUNING_GRID
            g_astPresetStations[s_iFMPresetCount].Frequency = s_wDisplayFMFrequency;
#else
#ifdef SD_USE_100KHZ_TUNING_GRID
            g_astPresetStations[s_iFMPresetCount].Frequency = ((s_wCurrentFMFrequency+FM_TUNER_GRID/2)/FM_TUNER_GRID)*FM_TUNER_GRID;

#else
            g_astPresetStations[s_iFMPresetCount].Frequency = s_wCurrentFMFrequency;
#endif
#endif
            g_astPresetStations[s_iFMPresetCount++].Level = s_iFMSignalStrengthRSSI;
            g_wCurrentPreset = s_iFMPresetCount;
		}
		else // if the preset stations are full, replace the weaker stations with a stronger one
		{
			INT iCount1, iCount2;
            FMTunerPreset stPresetTemp;
            // Bubble sort, the last station is always the weakest one
            for (iCount1 = 0; iCount1<NUMBER_OF_PRESETS; iCount1++)
            for (iCount2 = 0; iCount2<(NUMBER_OF_PRESETS - iCount1- 1); iCount2++)
            if (g_astPresetStations[iCount2].Level < g_astPresetStations[iCount2+1].Level)
            {
                stPresetTemp = g_astPresetStations[iCount2+1];
                g_astPresetStations[iCount2+1] = g_astPresetStations[iCount2];
                g_astPresetStations[iCount2] = stPresetTemp;
            }

            // Replace the last preset station
			if (g_astPresetStations[NUMBER_OF_PRESETS-1].Level < s_iFMSignalStrengthRSSI) // replaced the weaker one
			{
#ifdef SD_USE_50KHZ_TUNING_GRID
                    g_astPresetStations[NUMBER_OF_PRESETS-1].Frequency = s_wDisplayFMFrequency;
#else
#ifdef SD_USE_100KHZ_TUNING_GRID

                    g_astPresetStations[NUMBER_OF_PRESETS-1].Frequency = ((s_wCurrentFMFrequency+FM_TUNER_GRID/2)/FM_TUNER_GRID)*FM_TUNER_GRID;
#else
                    g_astPresetStations[NUMBER_OF_PRESETS-1].Frequency = s_wCurrentFMFrequency;
#endif
#endif
                    g_astPresetStations[NUMBER_OF_PRESETS-1].Level = s_iFMSignalStrengthRSSI;
			}

		}
		if (s_wCurrentFMFrequency  < FM_BAND_LIMIT_HIGH_FREQ) // scan more station if not at the end of the FM band
        {
			iTuningStatus = TunerDriverSearchUp();
		}
		else	// stop preset scanning
		{
			iTuningStatus = STFM1000EndAutoScanning();
		}
	}
    // Arrived at tuned station that is not upto par
	else
    {
		if (s_wCurrentFMFrequency  < FM_BAND_LIMIT_HIGH_FREQ) // scan more station if not at the end of the FM band
		{
            if (s_wCurrentFMFrequency < FM_BAND_LIMIT_LOW_FREQ)
                s_wCurrentFMFrequency = FM_BAND_LIMIT_LOW_FREQ; // Put the tuner in the scanning range
			iTuningStatus = TunerDriverSearchUp();
		}
		else	// stop preset scanning
		{
			iTuningStatus = STFM1000EndAutoScanning();
		}
	}

	return iTuningStatus;
}
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          STFM1000TunerSearchEnded
//
//   Type:          Function
//
//   Description:   Search complete
//
//   Inputs:        wSearchFrequency
//
//   Outputs:       Busy or SUCCESS
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
#ifdef FUNCLET
    #pragma asm
        org p,".stfm1000_SYS":
        FSTFM1000TunerSearchEnded:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet2 RSRC_FUNCLET_STFM1000_TUNER_SEARCH_ENDED

        org p,"SYSFUNCLET_STFM1000_TUNER_SEARCH_ENDED_P":
        dc      RSRC_FUNCLET_STFM1000_TUNER_SEARCH_ENDED
    #pragma endasm
_reentrant INT STFM1000TunerSearchEnded_Funclet(WORD wSearchFrequency )
#else
_reentrant INT STFM1000TunerSearchEnded(WORD wSearchFrequency )
#endif
{
#ifdef FM_USA_REGION
    WORD wFreqModulo = 0;
#endif
	g_wFMTunerError.m_bSEARCHING_UP = FALSE;
	g_wFMTunerError.m_bSEARCHING_DOWN = FALSE;
	s_iFMTunerSearchStatus = 0;	    // Let's call it a good station
#ifdef FM_USA_REGION
    // Skip the first even station we find if we search from 108MHz --stmp9292
    if(s_bSearchFromTopBand)
    {
        wFreqModulo = wSearchFrequency % 200;
        if(wFreqModulo == 0)
        {
            wSearchFrequency -= FM_FREQ_STEP;
        }
    }
    s_bSearchFromTopBand = FALSE;
#endif
    STFM1000TunerQuicksetStation(wSearchFrequency);
    bIsFirstTimeToNewStation = TRUE;

    return E_TUNER_BUSY;
}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextstfm1000":
    #pragma endasm
#endif
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          TunerDriverSearchUp
//
//   Type:          Function
//
//   Description:   Search up to find a station
//
//   Inputs:        none
//
//   Outputs:       Error status
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
_reentrant INT TunerDriverSearchUp(void)
{
#ifdef SD_USE_50KHZ_TUNING_GRID
    // step the search base to 100KHz
    s_wCurrentFMFrequency = 100 * (int) (s_wCurrentFMFrequency/100);
#endif
    // Remember the origin of the searching
    s_wBeginSearchFMFrequency = s_wCurrentFMFrequency;
    s_bTunerSearchHitBandLimit = FALSE;
    // fake out the tuner search
    if(s_iFMTunerSearchStatus != NOT_SEARCHING)
    {
        return TUNER_SUCCESS;
    }
#ifdef DEBUG_MONITOR_SIGNAL_QUALITY
    LogDebugFunction(0x50);
#endif

    TunerTuningInit();
#ifdef FM_USA_REGION
    // Skip the first even station we find if we search from 108MHz - -stmp9292

    if(s_wBeginSearchFMFrequency == FM_BAND_LIMIT_HIGH_FREQ)
    {
        s_bSearchFromTopBand = TRUE;
    }
#endif
    // If frequency is at the top of the band
    if ((s_wCurrentFMFrequency >= FM_BAND_LIMIT_HIGH_FREQ) && (g_bSearchEndAtBandLimit==FALSE))
    {
        s_wCurrentFMFrequency = FM_SEARCH_LOW_LIMIT;
        s_bTunerSearchHitBandLimit=TRUE;
    }

    g_wFMTunerError.m_bSEARCHING_UP = TRUE;
    g_wFMTunerError.m_bSEARCHING_DOWN = FALSE;
    s_iFMTunerSearchStatus = SEARCHING_UP;
    // Turn off DRI before change station
    STFM1000DriOff();

    if (s_wCurrentFMFrequency < FM_BAND_LIMIT_HIGH_FREQ)
    {
        STFM1000QuickTune((s_wCurrentFMFrequency += FM_FREQ_STEP));
    }
    else
    {
        STFM1000QuickTune(s_wCurrentFMFrequency);
    }
#ifdef SD_USE_50KHZ_TUNING_GRID
    s_wDisplayFMFrequency = s_wCurrentFMFrequency;
#endif
#ifdef DEBUG_MONITOR_SIGNAL_QUALITY
    LogDebugFunction(0x51);
#endif

    return E_TUNER_BUSY;
}
_reentrant INT STFM1000TuningSearchingScanning(void)
{
    INT iTuningStatus = TUNER_SUCCESS;
    BOOL bIsStation;
#ifdef SD_USE_50KHZ_TUNING_GRID
    g_wCurrentFrequency = s_wDisplayFMFrequency;
#else
    g_wCurrentFrequency = s_wCurrentFMFrequency;
#endif
    // if the tuner is success then test to see if scanning is in progress
    if (s_iFMTunerSearchStatus != NOT_SEARCHING)
    {	// stop searching after one round through the band limit
        iTuningStatus = STFM1000ContinueSearching();
    }
    else
    {
        if (g_wFMTunerError.m_bSCANNING_STATION==TRUE)
        {
            iTuningStatus = STFM1000ContinueScanning();
        }
        // if not tuning/searching/scanning	send out the message that tuning is done
        else
        {
            // Reset this flag when we change frequency
            bIsWeakSignal = FALSE;
            s_eOptimizeChannelState = OPTIMIZE_CHANNEL_STATE_START;
            iTuningStatus = STFM1000TunerLockStation();
            g_wFMTunerError.m_bTUNER_BUSY=FALSE;
            //g_bSearchEndAtBandLimit = FALSE;
        }
    }

    return iTuningStatus;
}
// inported from sw_stereocontroll.c for mem map savings.

//input:
_reentrant void sw_stereocontrol_xcall(unsigned short sd_audio_Rssi_u16)
{
    unsigned long  temp;

    IIRFiltCoeffSemaphore = 0;  // Filter coefficients are not yet valid - they are being recomputed!

    sw_stereocontrol.sd_audio_RssiMant_u16 = (sd_audio_Rssi_u16 & 0xFFE0) >> 5;  // 11 msb's
    sw_stereocontrol.sd_audio_RssiExp_u16 = sd_audio_Rssi_u16 & 0x001F;        // 5 lsb's
    sw_stereocontrol.sd_audio_RssiDecoded_u24 = sw_stereocontrol.sd_audio_RssiMant_u16 << sw_stereocontrol.sd_audio_RssiExp_u16;
    temp = sw_stereocontrol.sd_audio_RssiDecoded_u24;		//asign value of sd_audio_RssiDecoded_u24 to temp

    // *** Convert RSSI to 10*Log10(RSSI)
    // This is easily accomplished in DSP code using the CLZ instruction
    // rather than using all these comparisons.
    // The basic idea is this:
    // if x >= 2^P
    //   f(x) = 3*x>>P + (3*P-3)
    // Approx. is valid over the range of sd_audio_RssiDecoded_u24 in [0, 2^21]
    // ***

    temp = temp & 0xFFFFF;
    sw_stereocontrol.sd_audio_Prssi_u16 =0;
    while(temp >>= 1)								//right shift temp to find value of sd_audio_Prssi_u16
        sw_stereocontrol.sd_audio_Prssi_u16++;

    sw_stereocontrol.sd_audio_RssiLog_i16 = (3*sw_stereocontrol.sd_audio_RssiDecoded_u24 >> sw_stereocontrol.sd_audio_Prssi_u16) + (3*sw_stereocontrol.sd_audio_Prssi_u16-3);

    if (sw_stereocontrol.sd_audio_RssiLog_i16 < 0)  // Clamp to positive
        sw_stereocontrol.sd_audio_RssiLog_i16 = 0;

    // Compensate for errors in truncation/approximation by adding 1
    sw_stereocontrol.sd_audio_RssiLog_i16 = sw_stereocontrol.sd_audio_RssiLog_i16+1;

    // Scale up so we have some room for precision
    sw_stereocontrol.sd_audio_ScaledTrueRssi_u16 = sw_stereocontrol.sd_audio_RssiLog_i16 << 8;
    // ************
    // *** end RSSI
    // ************


    // **************************************************************************
    // *** Weak Signal Processing ***********************************************
    // **************************************************************************
    // *** This block operates each time the system can read RSSI_DCEST register
    // *** from Steely Dan.

    // *** Filter RSSI via attack/decay structure
    // Changed on 15NOV2006 the direction of the comparison to "attack" when RSSI "falls"
    if (sw_stereocontrol.sd_audio_ScaledTrueRssi_u16 <= sw_stereocontrol.sd_audio_PrevFilteredRssi_u16)
        sw_stereocontrol.sd_audio_Acc_u32 =(long)((long)sw_stereocontrol.sd_audio_pCoefRssiAttack_u16*(long)sw_stereocontrol.sd_audio_PrevFilteredRssi_u16
                                + ((long)65535-(long)sw_stereocontrol.sd_audio_pCoefRssiAttack_u16)*(long)sw_stereocontrol.sd_audio_ScaledTrueRssi_u16);
    else
        sw_stereocontrol.sd_audio_Acc_u32 = ((long)sw_stereocontrol.sd_audio_pCoefRssiDecay_u16*(long)sw_stereocontrol.sd_audio_PrevFilteredRssi_u16)
                                + (((long)65535-(long)sw_stereocontrol.sd_audio_pCoefRssiDecay_u16)*(long)sw_stereocontrol.sd_audio_ScaledTrueRssi_u16);

    sw_stereocontrol.sd_audio_FilteredRssi_u16 = sw_stereocontrol.sd_audio_Acc_u32 >> 16;
    sw_stereocontrol.sd_audio_PrevFilteredRssi_u16 = sw_stereocontrol.sd_audio_FilteredRssi_u16;


    global_filtered_rssi = sw_stereocontrol.sd_audio_FilteredRssi_u16>>8;  // DEBUG!!!


    // *** Form control variable
    // Generically speaking, ctl = f(RSSI) = filtered RSSI  (simplest way for now!)
    sw_stereocontrol.sd_audio_ControlLpr_i16 = sw_stereocontrol.sd_audio_FilteredRssi_u16;
    sw_stereocontrol.sd_audio_ControlLmr_i16 = sw_stereocontrol.sd_audio_FilteredRssi_u16;



    // *** Define LPR_BW = f(control LPR)
    // Assume that 5 kHz and 17 kHz are limits of LPR_BW control
    if (sw_stereocontrol.sd_audio_ControlLpr_i16 <= sw_stereocontrol.sd_audio_pCoefLprBwThLo_u16)
        sw_stereocontrol.sd_audio_LprBw_u16 = 5000;     // lower limit is 5 kHz
    else if  (sw_stereocontrol.sd_audio_ControlLpr_i16 >= sw_stereocontrol.sd_audio_pCoefLprBwThHi_u16)
        sw_stereocontrol.sd_audio_LprBw_u16 = 17000;    // upper limit is 17 kHz
    else
        sw_stereocontrol.sd_audio_LprBw_u16 = (long)17000 - (((long)sw_stereocontrol.sd_audio_pCoefLprBwSlSc_u16*((long)sw_stereocontrol.sd_audio_pCoefLprBwThHi_u16
                               - (long)sw_stereocontrol.sd_audio_ControlLpr_i16)) >> (long)sw_stereocontrol.sd_audio_pCoefLprBwSlSh_u16);

    // *** Define LMR_Gain = f(control LMR)
    // Assume that Blending occurs across 20 dB range of control LMR
    // For sake of listenability, approximate antilog blending curve
    // To simplify antilog approx, scale control LMR back into "RSSI in dB range" [0,60]
    sw_stereocontrol.sd_audio_ScaledControlLmr_u16 = sw_stereocontrol.sd_audio_ControlLmr_i16 >> 8;
    sw_stereocontrol.temp1_reg = sw_stereocontrol.sd_audio_pCoefLmrGaTh_u16 - sw_stereocontrol.sd_audio_ScaledControlLmr_u16;  // how far below blend threshold are we?
    if (sw_stereocontrol.temp1_reg < 0)             // We're not below threshold, so no blending needed
        sw_stereocontrol.temp1_reg = 0;
    sw_stereocontrol.temp2_reg = 20 - sw_stereocontrol.temp1_reg;    // Blend range = 20 dB
    if (sw_stereocontrol.temp2_reg < 0)
        sw_stereocontrol.temp2_reg = 0;               // if beyond that range, then clamp to 0

    // We want stereo separation (n dB) to rolloff linearly over the 20 dB wide blend region.
    // this necessitates a particular rolloff for the blend parameter, which is not obvious.
    // See sw_audio/log_approx.m for calculation of this rolloff, implemented below...
    // Note that stereo_separation (in dB) = 20*log10((1+a)/(1-a)), where a = blend scaler
    // appropriately scaled for 2^15.  This relationship sits at the heart of why this curve is needed.
    if (sw_stereocontrol.temp2_reg >= 15)
        sw_stereocontrol.temp3_reg = 264 * sw_stereocontrol.temp2_reg + 27487;
    else if (sw_stereocontrol.temp2_reg >= 10)
        sw_stereocontrol.temp3_reg = 650 * sw_stereocontrol.temp2_reg + 21692;
    else if (sw_stereocontrol.temp2_reg >= 5)
        sw_stereocontrol.temp3_reg = 1903 * sw_stereocontrol.temp2_reg + 9166;
    else
    sw_stereocontrol.temp3_reg = 3736 * sw_stereocontrol.temp2_reg;

    sw_stereocontrol.sd_audio_LmrGa_u16 = sw_stereocontrol.temp3_reg;

    if (sw_stereocontrol.sd_audio_LmrGa_u16 > 32767)
    sw_stereocontrol.sd_audio_LmrGa_u16 = 32767;   // Clamp to '1'


    // *** Define LPR_Gain = f(control LPR)
    // Assume that SoftMuting occurs across 20 dB range of control LPR
  // For sake of listenability, approximate antilog softmute curve
  // To simplify antilog approx, scale control LPR back into "RSSI in dB range" [0,60]

#if 0
  sw_stereocontrol.sd_audio_ScaledControlLpr_u16 = sw_stereocontrol.sd_audio_ControlLpr_i16 >> 8;
#else
  if((dwBias1Up ==0) && (dwBias1Down==0))  // IBIAS bits jerk RSSI around, so we compensate for that here
  {
    sw_stereocontrol.sd_audio_ScaledControlLpr_u16 = (sw_stereocontrol.sd_audio_ControlLpr_i16 >> 8)-6;
  }
  else if(dwBias1Up==1)
  {
    sw_stereocontrol.sd_audio_ScaledControlLpr_u16 = (sw_stereocontrol.sd_audio_ControlLpr_i16 >> 8)-12;
  }
  else
  {
    sw_stereocontrol.sd_audio_ScaledControlLpr_u16 = sw_stereocontrol.sd_audio_ControlLpr_i16 >> 8;
  }
#endif

  sw_stereocontrol.temp1_reg = sw_stereocontrol.sd_audio_pCoefLprGaTh_u16 - sw_stereocontrol.sd_audio_ScaledControlLpr_u16;  // how far below softmute threshold are we?
  if (sw_stereocontrol.temp1_reg < 0)             // We're not below threshold, so no softmute needed
    sw_stereocontrol.temp1_reg = 0;
  sw_stereocontrol.temp2_reg = 20 - sw_stereocontrol.temp1_reg;    // SoftMute range = 20 dB
  if (sw_stereocontrol.temp2_reg < 0)
    sw_stereocontrol.temp2_reg = 0;               // if beyond that range, then clamp to 0
  // Form 100*10^((temp2_reg-20)/20) approximation (antilog) over range [0,20] dB
  // approximation in range [0,100], but we only want to softmute down to -20 dB, no further
  if (sw_stereocontrol.temp2_reg > 16)
    sw_stereocontrol.temp3_reg = 9 * sw_stereocontrol.temp2_reg - 80;
  else if (sw_stereocontrol.temp2_reg > 12)
    sw_stereocontrol.temp3_reg = 6 * sw_stereocontrol.temp2_reg - 33;
  else if (sw_stereocontrol.temp2_reg > 8)
    sw_stereocontrol.temp3_reg = 4 * sw_stereocontrol.temp2_reg - 8;
  else
    sw_stereocontrol.temp3_reg = 2 * sw_stereocontrol.temp2_reg + 9;

  sw_stereocontrol.sd_audio_LprGa_u16 = 328 * sw_stereocontrol.temp3_reg;   // close to 32767*(1/100)

  if (sw_stereocontrol.sd_audio_LprGa_u16 > 32767)
    sw_stereocontrol.sd_audio_LprGa_u16 = 32767;   // Clamp to '1'

  if (sw_stereocontrol.sd_audio_LprGa_u16 < 3277)
    sw_stereocontrol.sd_audio_LprGa_u16 = 3277;   	 // Clamp to 0.1*32767 = -20 dB min gain

}
#pragma asm
        org p,".ptextstfm1000":
#pragma endasm
