#include "hwequ.h"
#include "messages.h"
#include "exec.h"
#include "resource.h"
#include "project.h"

#include "..\tunerdriver.h"
#include "Philips5767.h"

#pragma optimize 1

union WriteRegisterBundle Philips5767Write;
union ReadRegisterBundle Philips5767Read;

struct ErrorBits g_wFMTunerError;

#define NOT_SEARCHING	0
#define HI_INJECTION	1
#define FM_STATION_LEVEL_ALLOWED 2
#define FM_FREQ_STEP	50

/* FM TUNER Constants that can be overruled by project.inc */
#if !defined(NUMBER_OF_PRESETS)
#define NUMBER_OF_PRESETS	10
#endif
#if !defined(FM_BAND_LIMIT_HIGH_FREQ)
#define FM_BAND_LIMIT_HIGH_FREQ	108000
#endif
#if !defined(FM_BAND_LIMIT_LOW_FREQ)
#define FM_BAND_LIMIT_LOW_FREQ	87500
#endif
#if !defined(FM_TUNER_GRID)
#define FM_TUNER_GRID	100
#endif
#define FM_SEARCH_HIGH_LIMIT (FM_BAND_LIMIT_HIGH_FREQ+FM_FREQ_STEP)
#define FM_SEARCH_LOW_LIMIT (FM_BAND_LIMIT_LOW_FREQ-FM_FREQ_STEP)

/* 
 The nominal IF count is 55.  If the tuner is tuned to 50KHz lower than the station frequency,
 the IF should be about 43. For the frequency 50 KHz above the station then the IF should be
 around 67.  The following two constants are set to cover the IF variation for 50KHz above and
 below the station's frequency.
*/
#if !defined(FM_TUNER_IF_COUNT_HIGH)
#define   FM_TUNER_IF_COUNT_HIGH	62
#endif
#if !defined(FM_TUNER_IF_COUNT_LOW)
#define FM_TUNER_IF_COUNT_LOW	49
#endif

#define INIT_PLL_LOW  0x43
#define INIT_PLL_HIGH  0x2b
	
/* FM Tuner Global variables */
WORD g_wCurrentFrequency=90500;
BOOL _X g_bTunedStereoStation;
BOOL _X g_bSearchEndAtBandLimit = TRUE;
WORD  g_wCurrentPreset;
WORD wBeginTunerSearchFreq;
/* Set up the preset stations */

typedef struct PresetStation {
	WORD Frequency;
	INT	Level;
	} FMTunerPreset;
FMTunerPreset  g_astPresetStations[NUMBER_OF_PRESETS];

/* FM Tuner Static variables */
BOOL bTunerSearchHitBandLimit;
WORD wFM_Tuner_Search_Freq;
WORD wTunedFrequency;
INT iFMTuner_Searching = NOT_SEARCHING; // 0 - idle, 1 searching up, -1 searching down
INT iFMTuner_Injection = HI_INJECTION; // 1 - High injection, 0 - low injection
INT iFMTuner_IF_Count;
INT iFMTuner_IFcount[2];
INT iFMTuner_Level[2];
INT iFMTuner_Output_Level;
INT iFM_Preset_Count;
INT iFMTuner_InterFreq[3];
INT iFMTuner_InterADC[3];
INT iFMTuner_Sensitivity = 5;	/* Tuner sensitivity, default to 5 of 16 levels.  */
INT iWait_IF_Settle;
BOOL bForcedMono=FALSE;
BOOL bForcedMute=FALSE;
/* end preset settings */

_reentrant INT Philips5767WriteRegister(void);
_reentrant INT Philips5767ReadRegister(void);
_reentrant INT Philips5767FrequencyToPLL(INT);
_reentrant INT Philips5767PllToFrequency(INT);
_reentrant void Philips5767Mute(BOOL);
_reentrant INT Philips5767TuneToFrequency(INT);
_reentrant INT Philips3WireWriteBytesFromWords(INT, WORD* );
_reentrant INT Philips3WireReadBytesIntoWords(INT iBytes, WORD* pArray);
_reentrant INT iSetFreqAndWriteToTuner(INT wFrequency );
_reentrant INT Philips5767ContinueSearching( void );
_reentrant INT Philips5767ContinueScanning( void );
_reentrant void Philips3WireClockDelay(void);
 
_reentrant INT TunerDriverInit(void)
{
    INT i, iRetry;
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

    //Set the initial state    
    HW_3WIRE_CLOCK_DOR = FALSE;
    HW_3WIRE_DATA_DOR = TRUE;
    HW_3WIRE_WRITE_ENABLE_DOR = TRUE;
#ifdef HW_3WIRE_BUS_ENABLE_DOR 
    HW_3WIRE_BUS_ENABLE_DOR = TRUE;
#endif 
    
    //make all output
    HW_3WIRE_DATA_DOER = TRUE;
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
    Philips5767Write.Bytes.B1.I = 0;
    Philips5767Write.Bytes.B1.B.m_bMute      = 0;

    Philips5767Write.Bytes.B1.B.m_uPLL8_13   = INIT_PLL_HIGH;
    Philips5767Write.Bytes.B2.I=INIT_PLL_LOW;
    Philips5767Write.Bytes.B3.I=0xb0;
    Philips5767Write.Bytes.B3.B.m_uSearchStopLevel=1;
    Philips5767Write.Bytes.B4.I=0x11;
    Philips5767Write.Bytes.B4.B.m_bStandby=0;
    Philips5767Write.Bytes.B5.I=0;
#ifdef FM_TUNER_HCC 
    Philips5767Write.Bytes.B4.B.m_bHCC= (FM_TUNER_HCC ? 1 : 0);
#endif 
#ifdef FM_TUNER_SNC 
    Philips5767Write.Bytes.B4.B.m_bSNC= (FM_TUNER_SNC ? 1 : 0);
#endif 
#ifdef FM_TUNER_SOFT_MUTE 
    Philips5767Write.Bytes.B4.B.m_bSoftMute= (FM_TUNER_SOFT_MUTE ? 1 : 0);
#endif 
#ifdef FM_TUNER_XTAL 
    Philips5767Write.Bytes.B4.B.m_bXTAL= (FM_TUNER_XTAL ? 1 : 0);
#endif 
#ifdef FM_JAPAN_REGION 
    Philips5767Write.Bytes.B4.B.m_bBandLimits= 1;
#else
    Philips5767Write.Bytes.B4.B.m_bBandLimits= 0;
#endif    
#ifdef FM_TUNER_US_DEEMPHASIS 
    Philips5767Write.Bytes.B5.B.m_bDeemphasisTime= 1 ;
#else
    Philips5767Write.Bytes.B5.B.m_bDeemphasisTime= 0;
#endif 
#ifdef FM_TUNER_PLLREF 
    Philips5767Write.Bytes.B5.B.m_bPLLReference= (FM_TUNER_PLLREF ? 1 : 0);
#endif 
// initialize the g_wFMTunerError
    g_wFMTunerError.m_bTUNER_BUSY= FALSE;
	g_wFMTunerError.m_bSEARCHING_UP=FALSE;
	g_wFMTunerError.m_bSEARCHING_DOWN=FALSE;
	g_wFMTunerError.m_bSCANNING_STATION=FALSE;
	g_wFMTunerError.m_bBAND_LIMIT_HIGH_ERROR=FALSE;
	g_wFMTunerError.m_bBAND_LIMIT_LOW_ERROR=FALSE;
	g_wFMTunerError.m_bUNUSED_PRESET=FALSE;
	g_wFMTunerError.m_bPRESET_RANGE_ERROR=FALSE;
	g_wFMTunerError.m_bPOWERED_OFF=TRUE;
	// test if the tuner exist, write and read data while the tuner powered up
	iRetry = 3;
	g_wFMTunerError.m_bTUNER_NOT_FOUND=TRUE;
	while ((iRetry-- > 0) &&(g_wFMTunerError.m_bTUNER_NOT_FOUND==TRUE))
	{
    	Philips5767WriteRegister();
		for (i=0;i<1000;i++)
			Philips3WireClockDelay();
		Philips3WireReadBytesIntoWords(5,Philips5767Read.Array);
		if ((Philips5767Read.Bytes.B1.B.m_uPLL8_13 == INIT_PLL_HIGH) && (Philips5767Read.Bytes.B2.I==INIT_PLL_LOW))
			g_wFMTunerError.m_bTUNER_NOT_FOUND=FALSE;
	}
    Philips5767Write.Bytes.B4.B.m_bStandby=1;
    Philips5767WriteRegister();
    return TUNER_SUCCESS;
}

_reentrant INT TunerGetStatus( void )
{
    INT error=E_TUNER_BUSY;
#ifdef	HW_FMTUNER_READY_ENR
    if (HW_FMTUNER_READY_DIR)
    	error = Philips5767ReadRegister();
#else
    error = Philips3WireReadBytesIntoWords(1,Philips5767Read.Array);
    if((error == TUNER_SUCCESS)&& Philips5767Read.Bytes.B1.B.m_bReadyFlag)
    	error = Philips5767ReadRegister();
	else
		error = E_TUNER_BUSY;
#endif
	
	return error;
}

_reentrant void TunerTuningInit(void)
{
	g_wFMTunerError.m_bBAND_LIMIT_HIGH_ERROR = FALSE;
	g_wFMTunerError.m_bBAND_LIMIT_LOW_ERROR = FALSE;	// clear the searching error bits
	g_wFMTunerError.m_bTUNER_BUSY = TRUE;
	iFMTuner_Injection = 1; // use high injection
}
_reentrant INT TunerDriverTuneToFrequency(INT wFrequency)
{
    INT error=TUNER_SUCCESS;

	iFMTuner_Searching = 0;
	TunerTuningInit();
	wFM_Tuner_Search_Freq = wFrequency;	// set the current frequency
	iWait_IF_Settle = 2;	// any time there is a jump in frequency, let wait for IF count to settle
    return Philips5767TuneToFrequency(wFrequency);
}

_reentrant INT TunerDriverIncrementUp(void )
{
    INT error=TUNER_SUCCESS;

	iFMTuner_Searching = 0;
	TunerTuningInit();
	if (wFM_Tuner_Search_Freq < FM_BAND_LIMIT_HIGH_FREQ)
			error = Philips5767TuneToFrequency(wFM_Tuner_Search_Freq=(FM_TUNER_GRID * (INT) ((wFM_Tuner_Search_Freq+FM_TUNER_GRID)/FM_TUNER_GRID)));  // expected E_TUNER_BUSY
	else
	{
		iWait_IF_Settle = 2;	// any time there is a jump in frequency, let wait for IF count to settle
		error = Philips5767TuneToFrequency(wFM_Tuner_Search_Freq=FM_BAND_LIMIT_LOW_FREQ);  // expected E_TUNER_BUSY
	}
    return error;
}

_reentrant INT TunerDriverIncrementDown(void)
{
    INT error=TUNER_SUCCESS;
	iFMTuner_Searching = 0;
	TunerTuningInit();
	if (wFM_Tuner_Search_Freq > FM_BAND_LIMIT_LOW_FREQ)
		error = Philips5767TuneToFrequency(wFM_Tuner_Search_Freq=(FM_TUNER_GRID * (INT) ((wFM_Tuner_Search_Freq-FM_TUNER_GRID)/100)));  // expected E_TUNER_BUSY
	else
	{
		iWait_IF_Settle = 2;	// any time there is a jump in frequency, let wait for IF count to settle
		error = Philips5767TuneToFrequency(wFM_Tuner_Search_Freq=FM_BAND_LIMIT_HIGH_FREQ);  // expected E_TUNER_BUSY
	}
    return error;
}

_reentrant INT TunerDriverSearchUp(void)
{
    INT error=TUNER_SUCCESS;
	wBeginTunerSearchFreq=wFM_Tuner_Search_Freq;		// Remember the origin of the searching
	bTunerSearchHitBandLimit=FALSE;						// so we can stop searching after one round
// if frequency is at the top of the band
	if ((wFM_Tuner_Search_Freq >= FM_BAND_LIMIT_HIGH_FREQ)&&(g_bSearchEndAtBandLimit==FALSE))
	{
		iWait_IF_Settle = 2;	// any time there is a jump in frequency, let wait for IF count to settle
		wFM_Tuner_Search_Freq = FM_SEARCH_LOW_LIMIT-FM_FREQ_STEP;
	}

	if (iFMTuner_Searching == 0)	 // only search while not busy
	{
		if (wFM_Tuner_Search_Freq < FM_BAND_LIMIT_HIGH_FREQ)
		{
			iFMTuner_InterFreq[0] = iFMTuner_InterFreq[1] = iFMTuner_InterFreq[2] = 0;
			iFMTuner_Searching = 1;
			TunerTuningInit();
			g_wFMTunerError.m_bSEARCHING_UP = TRUE;
    		Philips5767Write.Bytes.B1.B.m_bMute      = 1;
    		Philips5767Write.Bytes.B3.B.m_bMonoStereo=1;
			error = Philips5767TuneToFrequency(wFM_Tuner_Search_Freq+=FM_FREQ_STEP);  // expected E_TUNER_BUSY
		}
		else
			error = Philips5767TuneToFrequency(wFM_Tuner_Search_Freq);  // expected E_TUNER_BUSY
	}
	else
		error = TUNER_SUCCESS;	// fake out the tuner search
    return error;
}

_reentrant INT TunerDriverSearchDown(void)
{
    INT error=TUNER_SUCCESS;
	wBeginTunerSearchFreq=wFM_Tuner_Search_Freq;		// Remember the origin of the searching
	bTunerSearchHitBandLimit=FALSE;						// so we can stop searching after one round
// if frequency is at the bottom of the band
	if ((wFM_Tuner_Search_Freq <= FM_BAND_LIMIT_LOW_FREQ)&&(g_bSearchEndAtBandLimit==FALSE))
	{
		iWait_IF_Settle = 2;	// any time there is a jump in frequency, let wait for IF count to settle
		wFM_Tuner_Search_Freq = FM_SEARCH_HIGH_LIMIT+FM_FREQ_STEP;
	}

	if (iFMTuner_Searching == 0)	 // only search while not busy
	{
		if (wFM_Tuner_Search_Freq > FM_BAND_LIMIT_LOW_FREQ)
		{
			iFMTuner_InterFreq[0] = iFMTuner_InterFreq[1] = iFMTuner_InterFreq[2] = 0;
			iFMTuner_Searching = -1;
			TunerTuningInit();
			g_wFMTunerError.m_bSEARCHING_DOWN = TRUE;
    		Philips5767Write.Bytes.B1.B.m_bMute      = 1;
    		Philips5767Write.Bytes.B3.B.m_bMonoStereo=1;
			error = Philips5767TuneToFrequency(wFM_Tuner_Search_Freq-=FM_FREQ_STEP); // expect E_TUNER_BUSY
		}
		else
			error = Philips5767TuneToFrequency(wFM_Tuner_Search_Freq);  // expected E_TUNER_BUSY
	}
	else
		error = TUNER_SUCCESS;	// fake out the tuner search
    return error;
}

_reentrant INT TunerDriverScanStations(void)
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
	g_wFMTunerError.m_bPOWERED_OFF=TRUE;

	g_wFMTunerError.m_bSCANNING_STATION = TRUE;
	wFM_Tuner_Search_Freq = FM_BAND_LIMIT_LOW_FREQ-FM_TUNER_GRID;
	iFM_Preset_Count = 0;
	iWait_IF_Settle = 3;	// any time there is a jump in frequency, let wait for IF count to settle
	return TunerDriverSearchUp();
}

_reentrant INT TunerDriverSetToPresetStations(INT iPresetStation)
{
	WORD wPresetFreq;
	if (iPresetStation >  NUMBER_OF_PRESETS)
	{
		iPresetStation = NUMBER_OF_PRESETS;
		g_wFMTunerError.m_bPRESET_RANGE_ERROR = TRUE;
	}
	else
		g_wFMTunerError.m_bPRESET_RANGE_ERROR = FALSE;
	wPresetFreq = g_astPresetStations[iPresetStation-1].Frequency;
	if ((wPresetFreq < FM_BAND_LIMIT_LOW_FREQ) || (wPresetFreq > FM_BAND_LIMIT_HIGH_FREQ))
	{
		wPresetFreq = FM_BAND_LIMIT_LOW_FREQ;
		g_wFMTunerError.m_bUNUSED_PRESET = TRUE;
	}
	else
		g_wFMTunerError.m_bUNUSED_PRESET = FALSE;
	g_wCurrentPreset = iPresetStation;
	return 	TunerDriverTuneToFrequency(wPresetFreq);
}

_reentrant INT TunerDriverSetMonoStereoMode(INT wMode )
{
    INT error=TUNER_SUCCESS;

    if(wMode == TUNER_MODE_STEREO)
	{
        bForcedMono = FALSE;
        Philips5767Write.Bytes.B3.B.m_bMonoStereo=1;
	}
    else if (wMode == TUNER_MODE_MONO)
	{
        bForcedMono = TRUE;
        Philips5767Write.Bytes.B3.B.m_bMonoStereo=0;
	}
    else
        error = E_TUNER_PARAMETER_IN_ERROR;

    if(!error)
        error = Philips5767WriteRegister();

    return error;
}

_reentrant INT TunerDriverSetSensitivity(INT wSensitivity )
{
    INT error=TUNER_SUCCESS;
    //convert the sensitivity passed into to the correct bits
	// let the sensitivy range = 0 - 100.
	// 5767 has 16 levels ADC, each level = 6.25
	//
    {
        //assign those bits to the command register
        if ((wSensitivity<=100) && (wSensitivity>=0))
        	iFMTuner_Sensitivity =  (wSensitivity*100)/625;		// this is used in manual searching
		wSensitivity  = 1;		  // lowest auto search level is 5
		if (iFMTuner_Sensitivity >= 7)
			wSensitivity = 2;
		if (iFMTuner_Sensitivity >= 10)
			wSensitivity = 3;
        Philips5767Write.Bytes.B3.B.m_uSearchStopLevel=wSensitivity;
// sensitivity only applied to search command, wait for next searching to execute the level change
    }

    return error;
}

_reentrant INT TunerDriverSetTunerStandby(BOOL bStandby )
{
    if (bStandby)
	{
    	Philips5767Write.Bytes.B4.B.m_bStandby=1;
		g_wFMTunerError.m_bPOWERED_OFF = TRUE;
	}
	else
	{
    	Philips5767Write.Bytes.B4.B.m_bStandby=0;
		g_wFMTunerError.m_bPOWERED_OFF = FALSE;
	}

    Philips5767WriteRegister();	 // update the tuner
    return (bStandby ? TUNER_SUCCESS : E_TUNER_BUSY);
}

_reentrant INT TunerDriverGetTunerstate( void )
{
    INT error;

    Philips3WireWriteBytesFromWords(1,Philips5767Write.Array);
    error = Philips3WireReadBytesIntoWords(5,Philips5767Read.Array);
    g_wCurrentFrequency=Philips5767PllToFrequency(Philips5767Read.Bytes.B2.B.m_uPLL0_7|(((INT)Philips5767Read.Bytes.B1.B.m_uPLL8_13)<<8));
	g_wCurrentFrequency = ((g_wCurrentFrequency+FM_TUNER_GRID/2)/FM_TUNER_GRID)*FM_TUNER_GRID;	// round the frequency
	iFMTuner_IF_Count = Philips5767Read.Bytes.B3.B.m_uIFCounter;
	iFMTuner_Output_Level = Philips5767Read.Bytes.B4.B.m_uADCLevel;
	g_bTunedStereoStation = Philips5767Read.Bytes.B3.B.m_bStereo;
	return error;
}

#pragma optimize R
_reentrant void Philips3WireClockDelay(void)
{
    int i;
   for(i=0;i<PHILIPS_3WIRE_DELAY;i++)
   {
     _nop(); // TODO: For portability: convert this to macro defined in a single file like types.h
   };
}
#pragma endoptimize

_reentrant INT Philips3WireReadByte(void)
{
    int i;
    int data=0;

    for(i=0;i<8;i++)
    {
        Philips3WireClockDelay();
        HW_3WIRE_CLOCK_DOR = TRUE;
        Philips3WireClockDelay();
        data = data <<1;
        if(HW_3WIRE_DATA_DIR)
            data |= 1;
        else
            data &=0x0000fe;
        Philips3WireClockDelay();
        HW_3WIRE_CLOCK_DOR = FALSE;
    }
        Philips3WireClockDelay();
        Philips3WireClockDelay();
    
    return data;
}

_reentrant void Philips3WireWriteByte(INT data)
{
    int i;

    for(i=0;i<8;i++)
    {
        if(data&0x80)
            HW_3WIRE_DATA_DOR = TRUE;
        else
            HW_3WIRE_DATA_DOR = FALSE;
        data = (data<<1)&0xfe;
        Philips3WireClockDelay();
        HW_3WIRE_CLOCK_DOR = TRUE;
        Philips3WireClockDelay();
        HW_3WIRE_CLOCK_DOR = FALSE;
    }
        Philips3WireClockDelay();
        Philips3WireClockDelay();
}


_reentrant INT Philips3WireReadBytesIntoWords(INT iBytes, WORD* pArray)
{
    int i;
    int iErrorCount=0;
    INT error = TUNER_SUCCESS;

    //set the data to be input
    HW_3WIRE_DATA_DOER = FALSE;
    //drive the 5767 to input mode
        Philips3WireClockDelay();
    HW_3WIRE_WRITE_ENABLE_DOR = FALSE;
        Philips3WireClockDelay();
    HW_3WIRE_WRITE_ENABLE_DOR = TRUE;
        Philips3WireClockDelay();
    HW_3WIRE_WRITE_ENABLE_DOR = FALSE;
    for(i=0;i<iBytes;i++)
    {
        pArray[i] = Philips3WireReadByte();
        if(pArray[i]==0 || pArray[i]==0xff)
        {
            iErrorCount++;
        }
    }
    HW_3WIRE_WRITE_ENABLE_DOR = TRUE;
    if(iErrorCount == iBytes)
    {
        error = E_TUNER_NO_COMMUNICATION;
    }
    return error;
}

_reentrant INT Philips3WireWriteBytesFromWords(INT iBytes, WORD* pArray)
{
    int i;
    int iErrorCount=0;
    INT error = TUNER_SUCCESS;

    //set the data to be output
    HW_3WIRE_DATA_DOER = TRUE;
    //drive the 5767 to input mode
        Philips3WireClockDelay();
    HW_3WIRE_WRITE_ENABLE_DOR = TRUE;
        Philips3WireClockDelay();
    HW_3WIRE_WRITE_ENABLE_DOR = FALSE;
        Philips3WireClockDelay();
    HW_3WIRE_WRITE_ENABLE_DOR = TRUE;
        Philips3WireClockDelay();

    for(i=0;i<iBytes;i++)
    {
        Philips3WireWriteByte(pArray[i]);
    }

    HW_3WIRE_WRITE_ENABLE_DOR = TRUE;
    return error;
}

_reentrant INT Philips5767PllToFrequency(INT iPLL)
{
    if(!Philips5767Write.Bytes.B3.B.m_bHiLoInjection)
		return (INT) (((DWORD) iPLL * (DWORD) 8192)/(DWORD) 1000)	+ 225; 
	else
		return (INT) (((DWORD) iPLL * (DWORD) 8192)/(DWORD) 1000)	- 225;
}

_reentrant INT Philips5767FrequencyToPLL(INT iFrequency)
{
    INT iPll=0;
    INT iFif=225;
    if(!Philips5767Write.Bytes.B3.B.m_bHiLoInjection)
        iFif=-iFif;
//  reduce calculations and round up the number
      iPll = (((DWORD)iFrequency+(DWORD)iFif)*(DWORD)1000+(DWORD)4096)/(DWORD)8192;
    return iPll;
}


_reentrant INT Philips5767ReadRegister(void)
{
    INT error = TUNER_SUCCESS;
    int iPLL;
 
    error = Philips3WireReadBytesIntoWords(5,Philips5767Read.Array);

    if(error == TUNER_SUCCESS)
    {
        // the tuner takes longer to get the IF counter done right,  so wait to get the right count
        if (iWait_IF_Settle>0)
		{
			--iWait_IF_Settle;
			Philips3WireWriteBytesFromWords(2,Philips5767Write.Array);
			error =  E_TUNER_BUSY;
		}
        else
        {
        	if(Philips5767Read.Bytes.B1.B.m_bReadyFlag)
        	{
        //update the global variables
            	Philips5767Write.Bytes.B1.B.m_uPLL8_13 =Philips5767Read.Bytes.B1.B.m_uPLL8_13;
            	Philips5767Write.Bytes.B2.B.m_uPLL0_7  =Philips5767Read.Bytes.B2.B.m_uPLL0_7;

            	g_wCurrentFrequency=Philips5767PllToFrequency(Philips5767Read.Bytes.B2.B.m_uPLL0_7|(((INT)Philips5767Read.Bytes.B1.B.m_uPLL8_13)<<8));
// pass the tuning information
				iFMTuner_IF_Count = Philips5767Read.Bytes.B3.B.m_uIFCounter;
				iFMTuner_Output_Level = Philips5767Read.Bytes.B4.B.m_uADCLevel;
				g_bTunedStereoStation = Philips5767Read.Bytes.B3.B.m_bStereo;

// if the tuner is success then test to see if scanning is in progress
				if (iFMTuner_Searching != NOT_SEARCHING)
				{	// stop searching after one round through the band limit
					error = Philips5767ContinueSearching();
				}	
				else	// if not searching	send out the message that tuning is done
				{
					wTunedFrequency = g_wCurrentFrequency;
					if (g_wFMTunerError.m_bSCANNING_STATION==TRUE)
						error = Philips5767ContinueScanning();
					else
					{
            			g_wFMTunerError.m_bTUNER_BUSY=FALSE;	// clear tuner busy bit
            			SysPostMessage(2,MENU_TUNER_TUNED);
					}
				}
			} // end of tuner_success
        	else
        	{
            	error = E_TUNER_BUSY;
        	}
		}
    }
    return error;
}

_reentrant INT Philips5767WriteRegister(void)
{
    INT error = TUNER_SUCCESS;
 
    Philips3WireWriteBytesFromWords(5,Philips5767Write.Array);
    Philips5767Write.Bytes.B1.B.m_bSearchMode=0;
    return error;
}

_reentrant BOOL bIsTunerModulePowered(void)
{
	return 	 Philips5767Write.Bytes.B4.B.m_bStandby ? FALSE : TRUE;
}

_reentrant void Philips5767Mute(BOOL bMute)
{
    if (bMute)
	{
		bForcedMute=TRUE;
    	Philips5767Write.Bytes.B1.B.m_bMute=1;
	}
	else
	{
		bForcedMute=FALSE;
    	Philips5767Write.Bytes.B1.B.m_bMute=0;
	}
}


_reentrant INT iSetFreqAndWriteToTuner(INT wFrequency )
{
    INT iPLL;
    
    iPLL = Philips5767FrequencyToPLL(wFrequency);
    Philips5767Write.Bytes.B2.B.m_uPLL0_7 = iPLL&0xff;
    Philips5767Write.Bytes.B1.B.m_uPLL8_13 = (iPLL>>8)&0x3f;
    return Philips5767WriteRegister();
}

_reentrant INT Philips5767TuneToFrequency(INT wFrequency)
{
    INT error=TUNER_SUCCESS;

// IF counter needs more time if there is a big frequency jump
	if ((wFrequency > FM_BAND_LIMIT_HIGH_FREQ) || (wFrequency < FM_BAND_LIMIT_LOW_FREQ))
		iWait_IF_Settle = 3;
//  Positive frequency use High injection. Otherwise use low injection
	if (wFrequency > 0)
		Philips5767Write.Bytes.B3.B.m_bHiLoInjection = 1;
	else
	{
		Philips5767Write.Bytes.B3.B.m_bHiLoInjection = 0;
		wFrequency = -wFrequency;
	}
    error = iSetFreqAndWriteToTuner(wFrequency );
    return E_TUNER_BUSY; // force the tuner busy
}

_reentrant INT TunerSearchEnded( WORD SearchFrequency )
{
    if (bForcedMute == FALSE)
    	Philips5767Write.Bytes.B1.B.m_bMute = 0;
    if (bForcedMono == FALSE)
    	Philips5767Write.Bytes.B3.B.m_bMonoStereo = 0;
	g_wFMTunerError.m_bSEARCHING_UP = FALSE;
	g_wFMTunerError.m_bSEARCHING_DOWN = FALSE;
	iFMTuner_Searching = 0;	// Let's call it a good station
	return Philips5767TuneToFrequency(SearchFrequency); // error should be Tuner busy
}

_reentrant INT NextSearchFrequency( void )
{
	INT error;

	if ((wFM_Tuner_Search_Freq >= FM_SEARCH_LOW_LIMIT) && (wFM_Tuner_Search_Freq <= FM_SEARCH_HIGH_LIMIT))
		error = Philips5767TuneToFrequency(wFM_Tuner_Search_Freq+=FM_FREQ_STEP*iFMTuner_Searching); // expected E_TUNER_BUSY
	else
	{
		if (g_bSearchEndAtBandLimit==TRUE)
		{
			if(iFMTuner_Searching > 0)
				g_wFMTunerError.m_bBAND_LIMIT_HIGH_ERROR = TRUE;
			else
				g_wFMTunerError.m_bBAND_LIMIT_LOW_ERROR = TRUE;
			error = TunerSearchEnded(wFM_Tuner_Search_Freq); // error should be Tuner busy
		}
		else
		{
			bTunerSearchHitBandLimit=TRUE;
			if(iFMTuner_Searching>0)
				wFM_Tuner_Search_Freq=FM_BAND_LIMIT_LOW_FREQ-FM_FREQ_STEP;
			else
				wFM_Tuner_Search_Freq=FM_BAND_LIMIT_HIGH_FREQ+FM_FREQ_STEP;
			iFMTuner_InterFreq[0] = iFMTuner_InterFreq[1] = iFMTuner_InterFreq[2] = 0;
			iFMTuner_Injection = 1; // use high injection
			error = Philips5767TuneToFrequency(wFM_Tuner_Search_Freq); // error should be Tuner busy
		}
	}	/* end if((g_wFM_Tuner_Search_Fre)...., else { ... } */
	return error;
}

_reentrant INT Philips5767ContinueSearching( void )
{
	INT error;
    int i;
	
	if (((wFM_Tuner_Search_Freq >=wBeginTunerSearchFreq)&&(bTunerSearchHitBandLimit==TRUE)&&(iFMTuner_Searching>0))||
		((wFM_Tuner_Search_Freq <=wBeginTunerSearchFreq)&&(bTunerSearchHitBandLimit==TRUE)&&(iFMTuner_Searching<0)))
	{
		error = TunerSearchEnded(wBeginTunerSearchFreq);
	}
	else
	{
		iFMTuner_IFcount[iFMTuner_Injection] = iFMTuner_IF_Count;
		iFMTuner_Level[iFMTuner_Injection] = iFMTuner_Output_Level;
		if (iFMTuner_Injection == HI_INJECTION)
		{
			iFMTuner_InterFreq[1-iFMTuner_Searching] = iFMTuner_InterFreq[1];
			iFMTuner_InterFreq[1] = iFMTuner_InterFreq[1+iFMTuner_Searching];
			iFMTuner_InterFreq[1+iFMTuner_Searching] = iFMTuner_IF_Count;
			iFMTuner_InterADC[1] = iFMTuner_InterADC[1+iFMTuner_Searching];
			iFMTuner_InterADC[1+iFMTuner_Searching] = iFMTuner_Output_Level;
			if ((iFMTuner_InterFreq[0] > 0) && (iFMTuner_InterFreq[0] <= 50) && (iFMTuner_InterFreq[1] >= 52) && 
				(iFMTuner_InterFreq[1] <= 58) && (iFMTuner_InterFreq[2] >= 60)) // for a valid station, IF count must be valid
			{
				iFMTuner_Level[iFMTuner_Injection] = iFMTuner_InterADC[1];
				iFMTuner_Injection = 0; // Check this station on the low side
				error = Philips5767TuneToFrequency(-wFM_Tuner_Search_Freq+FM_FREQ_STEP*iFMTuner_Searching); // expect E_TUNER_BUSY 
			}
			else
			{
				error = NextSearchFrequency();
			}
		}
		else // low injection to double check the station
		{
			if ((iFMTuner_IF_Count >= FM_TUNER_IF_COUNT_LOW) && (iFMTuner_IF_Count <= FM_TUNER_IF_COUNT_HIGH) 
					&& (iFMTuner_Output_Level >= iFMTuner_Sensitivity)) // for a valid station, IF count must be valid
			{	// Everything is OK
				iFMTuner_Injection = HI_INJECTION;
				if (((iFMTuner_Level[1] - iFMTuner_Level[0]) <= FM_STATION_LEVEL_ALLOWED) && ((iFMTuner_Level[0] - iFMTuner_Level[1]) <= FM_STATION_LEVEL_ALLOWED))	// Levels on both injections are consistant then it can be a good station
				{
    				error = TunerSearchEnded(wFM_Tuner_Search_Freq-FM_FREQ_STEP*iFMTuner_Searching);
				}
				else // try other frequencies, the too much change in the level between the low and high injection
				{
					error = NextSearchFrequency();
				}
			}
			else
			{	// IF count or ADC level is not right
				iFMTuner_Injection = HI_INJECTION;
				error = NextSearchFrequency();
			}	/* end 	if ((iFMTuner_IF_Count >= FM_TUNER_IF_COUNT_LOW) ..., else { ... } */
		}	/* end if (iFMTuner_Injection == HI_INJECTION) ..., else { ... } */
	}	/* end if (iFMTuner_Searching != NOT_SEARCHING) { ... } */
	return error;
}

_reentrant INT EndAutoScanning( void )
{
	WORD PresetFrequency;
	if ((PresetFrequency = g_astPresetStations[0].Frequency)==0)
		PresetFrequency = FM_BAND_LIMIT_LOW_FREQ;
	g_wFMTunerError.m_bSCANNING_STATION = FALSE;	 // end preset scanning
	g_wCurrentPreset=1;
	return TunerDriverTuneToFrequency(PresetFrequency);									
}
_reentrant INT Philips5767ContinueScanning( void )
{
	INT i;
	INT error;
	INT iLowestLevel;
	INT iLowestPreset;

	if ((iFMTuner_IF_Count >= FM_TUNER_IF_COUNT_LOW) && (iFMTuner_IF_Count <= FM_TUNER_IF_COUNT_HIGH) && 
			(iFMTuner_Output_Level >= iFMTuner_Sensitivity ) && (wTunedFrequency >= FM_BAND_LIMIT_LOW_FREQ) && (wTunedFrequency <= FM_BAND_LIMIT_HIGH_FREQ)) // test for valid station, IF count in the range and Level is OK
	{
		SysPostMessage(2,MENU_TUNER_TUNED);
		if (iFM_Preset_Count < NUMBER_OF_PRESETS)
		{
			g_astPresetStations[iFM_Preset_Count].Frequency = ((wTunedFrequency+FM_TUNER_GRID/2)/FM_TUNER_GRID)*FM_TUNER_GRID;
			g_astPresetStations[iFM_Preset_Count++].Level = iFMTuner_Output_Level;
			g_wCurrentPreset = iFM_Preset_Count;
		}
		else // if the preset stations are full, replace the weaker stations with a stronger one
		{
			iLowestLevel = 16;
			iLowestPreset = 0;
			for (i= 0; i< NUMBER_OF_PRESETS; i++)
				if (g_astPresetStations[i].Level<iLowestLevel)
				{
					iLowestLevel = g_astPresetStations[i].Level;
					iLowestPreset = i;
				}
				if (iLowestLevel < iFMTuner_Output_Level) // replaced the weaker one
				{
					g_astPresetStations[g_wCurrentPreset=iLowestPreset].Frequency = ((wTunedFrequency+FM_TUNER_GRID/2)/FM_TUNER_GRID)*FM_TUNER_GRID;
					g_astPresetStations[iLowestPreset].Level = iFMTuner_Output_Level;
				}
		}
		if (wTunedFrequency  < FM_BAND_LIMIT_HIGH_FREQ) // scan more station if not at the end of the FM band
        {
			error = TunerDriverSearchUp();
		}
		else	// stop preset scanning
		{
			error = EndAutoScanning();
		}
	}
	else // Arrived at tuned station that is not upto par
	{
		if (wTunedFrequency  < FM_BAND_LIMIT_HIGH_FREQ) // scan more station if not at the end of the FM band
		{
            if (wTunedFrequency < FM_BAND_LIMIT_LOW_FREQ)
                wFM_Tuner_Search_Freq = FM_BAND_LIMIT_LOW_FREQ; // Put the tuner in the scanning range
			error = TunerDriverSearchUp();
		}
		else	// stop preset scanning
		{
			error = EndAutoScanning();
		}
	}

	return error;
}

_reentrant INT TunerDriverSetPresetStations( INT iPresetStation)
{
	INT error = TUNER_SUCCESS;

	if ((iPresetStation-1<NUMBER_OF_PRESETS)&&(iPresetStation>0))
	{
		g_astPresetStations[iPresetStation-1].Frequency = g_wCurrentFrequency;
		g_astPresetStations[iPresetStation-1].Level = 1;
		g_wCurrentPreset = iPresetStation;
        SysPostMessage(2,MENU_TUNER_TUNED);
	}
	else
		g_wFMTunerError.m_bPRESET_RANGE_ERROR=TRUE;
	return error;
}
_reentrant INT TunerDriverErasePresetStations( INT iPresetStation)
{
	INT error = TUNER_SUCCESS;

	if ((iPresetStation-1<NUMBER_OF_PRESETS)&&(iPresetStation>0))
	{
		g_astPresetStations[iPresetStation-1].Frequency = 0;
		g_astPresetStations[iPresetStation-1].Level = 0;
		g_wCurrentPreset = 0;
        SysPostMessage(2,MENU_TUNER_TUNED);
	}
	else
		g_wFMTunerError.m_bPRESET_RANGE_ERROR=TRUE;
	return error;
}


