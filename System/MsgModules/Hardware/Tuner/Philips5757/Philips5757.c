#include "types.h"
#include "hwequ.h"
#include "messages.h"
#include "exec.h"
#include "resource.h"
#include "project.h"

#include "..\tunerdriver.h"
#include "Philips5757.h"

extern INT _X g_iTunerProcessIntervalMs;

Philips5757ShiftRegisterType Philips5757ShiftRegisterOut;
Philips5757ShiftRegisterType Philips5757ShiftRegisterIn;

_reentrant INT Philips5757WriteRegister(BOOL bSearch);
_reentrant INT Philips5757ReadRegister(void);
_reentrant INT Philips5757PllToFrequency(INT iPLL);
_reentrant INT Philips5757FrequencyToPLL(INT iFrequency);
_reentrant INT Philips5767ContinueSearching( void );
_reentrant INT Philips5767ContinueScanning( void );
_reentrant INT Philips5757Tuning(BOOL bSearch);
_reentrant void Philips3WireClockDelay(void);
_reentrant INT Philips3WireReadBits(WORD*);
int mydummy(void)
{
    return 0;
}

#pragma asm 
    org p,"uncodebankify":
#pragma endasm 

struct ErrorBits g_wFMTunerError;

/* FM TUNER Constants that can be overruled by project.inc or resouce.inc */
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
#define FM_FREQ_STEP 50

#define FM_SEARCH_HIGH_LIMIT (FM_BAND_LIMIT_HIGH_FREQ+FM_FREQ_STEP)
#define FM_SEARCH_LOW_LIMIT (FM_BAND_LIMIT_LOW_FREQ-FM_FREQ_STEP)

/* Define Tuner flags used in g_wFMTunerError */
#define TUNER_OK	0
#define TUNER_BUSY	0x000001
#define TUNER_BUSY_SEARCHING_UP	0x000003
#define	TUNER_BUSY_SEARCHING_DOWN	0x000005
#define	TUNER_BUSY_SCANNING_STATION	0x000011
#define	TUNER_BUSY_SCAN_STATION_SEARCHING	0x000013
#define	TUNER_POWERED_OFF	0x80000
#define BAND_LIMIT_HIGH_ERROR	0x000100
#define BAND_LIMIT_LOW_ERROR	0x000200
#define UNUSED_PRESET	0x000400
#define	PRESET_RANGE_ERROR	0x000800

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
//FMTunerPreset g_astPresetStations[NUMBER_OF_PRESETS];
FMTunerPreset  g_astPresetStations[NUMBER_OF_PRESETS];

/* FM Tuner Static variables */
BOOL bTunerSearchHitBandLimit;
WORD wFM_Tuner_Search_Freq;
INT iFM_Preset_Count;
BOOL bSetFreqBeforeSearch;


_reentrant INT TunerDriverInit(void)
{
    INT i, iRetry;
    //power up the required pins
#if defined(DEVICE_3410)||defined(DEVICE_3310)
    HW_3WIRE_CLOCK_PWR = TRUE;
    HW_3WIRE_DATA_PWR  = TRUE;
    HW_3WIRE_WRITE_ENABLE_PWR = TRUE;
#endif

    //make them all gpio
    HW_3WIRE_CLOCK_ENR = TRUE;
    HW_3WIRE_DATA_ENR  = TRUE;
    HW_3WIRE_WRITE_ENABLE_ENR = TRUE;
    HW_3WIRE_FM_POWER_DOWN_ENR = TRUE;
	HW_3WIRE_MONO_UNTUNED_ENR = TRUE;

    //make all output, except the MONO_UNTUNED
    HW_3WIRE_DATA_DOER = TRUE;
    HW_3WIRE_CLOCK_DOER = TRUE;
    HW_3WIRE_WRITE_ENABLE_DOER = TRUE;
    HW_3WIRE_FM_POWER_DOWN_DOER = TRUE;
	HW_3WIRE_MONO_UNTUNED_DOER = FALSE;   // input signal for MONO/STEREO# or UNTUNED/TUNED#
    // set tuner process interval to 100 msec
    g_iTunerProcessIntervalMs = 100;
    //Set the initial state    
    HW_3WIRE_CLOCK_DOR = FALSE;
    HW_3WIRE_DATA_DOR = TRUE;
    HW_3WIRE_WRITE_ENABLE_DOR = FALSE;
	HW_3WIRE_FM_POWER_DOWN_DOR = TRUE;	  // FM Tuner is powered down

    Philips5757ShiftRegisterOut.I = 0;
	Philips5757ShiftRegisterOut.B.m_uSearchLevel=2;	   /* searching level is > 30 uV */
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

    Philips5757ShiftRegisterOut.B.m_uPLL0_14 = Philips5757FrequencyToPLL(wFM_Tuner_Search_Freq=87500);
	bSetFreqBeforeSearch = FALSE;
// test for the present of the tuner
	HW_3WIRE_FM_POWER_DOWN_DOR = FALSE;	  // power up the FM Tuner
	g_wFMTunerError.m_bTUNER_NOT_FOUND=TRUE;
	iRetry = 3;
	while ((iRetry-- > 0) && (g_wFMTunerError.m_bTUNER_NOT_FOUND==TRUE))
	{
	    Philips5757WriteRegister(FALSE);
		for (i=0;i<100;i++)
	   		Philips3WireClockDelay();
		Philips3WireReadBits(&Philips5757ShiftRegisterIn.I);
		if((Philips5757ShiftRegisterIn.B.m_uPLL0_14 >= 6936)&&(Philips5757ShiftRegisterIn.B.m_uPLL0_14<=9656))
			g_wFMTunerError.m_bTUNER_NOT_FOUND=FALSE;
	}

	HW_3WIRE_FM_POWER_DOWN_DOR = TRUE;	  // FM Tuner is powered down
 
    return TUNER_SUCCESS;
}


_reentrant INT TunerGetStatus()
{
	return Philips5757ReadRegister();
}

_reentrant INT TunerDriverTuneToFrequency(INT wFrequency)
{
    INT error;
    wFM_Tuner_Search_Freq = wFrequency;
    return Philips5757Tuning(FALSE);
}

_reentrant INT TunerDriverIncrementUp(void )
{
    wFM_Tuner_Search_Freq+=FM_TUNER_GRID;
    if (wFM_Tuner_Search_Freq > FM_BAND_LIMIT_HIGH_FREQ)
		wFM_Tuner_Search_Freq = FM_BAND_LIMIT_LOW_FREQ;
    return Philips5757Tuning(FALSE);
}

_reentrant INT TunerDriverIncrementDown(void)
{
    wFM_Tuner_Search_Freq-=FM_TUNER_GRID;
   if (wFM_Tuner_Search_Freq < FM_BAND_LIMIT_LOW_FREQ)
		wFM_Tuner_Search_Freq = FM_BAND_LIMIT_HIGH_FREQ;
    return Philips5757Tuning(FALSE);
}

_reentrant INT TunerDriverSearchUp(void)
{
	g_wFMTunerError.m_bSEARCHING_UP = TRUE;
    wBeginTunerSearchFreq = wFM_Tuner_Search_Freq;

// if frequency is at the top of the band then seach from the bottom of the band when wrapping around
	if ((wFM_Tuner_Search_Freq >= FM_BAND_LIMIT_HIGH_FREQ)&&(g_bSearchEndAtBandLimit==FALSE))
		wFM_Tuner_Search_Freq = FM_SEARCH_LOW_LIMIT-FM_FREQ_STEP;

    wFM_Tuner_Search_Freq+=FM_FREQ_STEP;
    Philips5757ShiftRegisterOut.B.m_bSearchUp = TRUE;
	g_wFMTunerError.m_bBAND_LIMIT_HIGH_ERROR=FALSE;
	g_wFMTunerError.m_bBAND_LIMIT_LOW_ERROR=FALSE;
	bTunerSearchHitBandLimit = FALSE;
    bSetFreqBeforeSearch = FALSE;
    return Philips5757Tuning(TRUE);
}

_reentrant INT TunerDriverSearchDown(void)
{
	g_wFMTunerError.m_bSEARCHING_DOWN = TRUE;
    wBeginTunerSearchFreq = wFM_Tuner_Search_Freq;
// if frequency is at the bottom of the band, seach from the top of the band when wrapping around
	if ((wFM_Tuner_Search_Freq <= FM_BAND_LIMIT_LOW_FREQ)&&(g_bSearchEndAtBandLimit==FALSE))
		wFM_Tuner_Search_Freq = FM_SEARCH_HIGH_LIMIT+FM_FREQ_STEP;

    wFM_Tuner_Search_Freq-=FM_FREQ_STEP;
    Philips5757ShiftRegisterOut.B.m_bSearchUp = FALSE;
	g_wFMTunerError.m_bBAND_LIMIT_HIGH_ERROR=FALSE;
	g_wFMTunerError.m_bBAND_LIMIT_LOW_ERROR=FALSE;
	bTunerSearchHitBandLimit = FALSE;
    bSetFreqBeforeSearch = FALSE;
    return Philips5757Tuning(TRUE);
}

_reentrant INT TunerDriverSetMonoStereoMode(INT wMode )
{
    INT error=TUNER_SUCCESS;

    if(wMode == TUNER_MODE_STEREO)
        Philips5757ShiftRegisterOut.B.m_bMonoStereo=0;
    else if (wMode == TUNER_MODE_MONO)
        Philips5757ShiftRegisterOut.B.m_bMonoStereo=1;
    else
        error = E_TUNER_PARAMETER_IN_ERROR;

    if(!error)
        error = Philips5757WriteRegister(FALSE);

    return error;
}

_reentrant INT TunerDriverSetSensitivity(INT wSensitivity )
{
    INT error=TUNER_SUCCESS;
    //convert the sensitivity passed into to the correct bits
    if(wSensitivity >100)
    {
        wSensitivity = 3;
    } 
    else if(wSensitivity > 30)
    {
        wSensitivity = 2;
    } 
    else if(wSensitivity > 10)
    {
        wSensitivity = 1;
    }
    else if(wSensitivity > 5)
    {
        wSensitivity = 0;
    }
    else
    {
        error = E_TUNER_PARAMETER_IN_ERROR;
    }
    if(!error)
    {
        //assign those bits to the command register
        Philips5757ShiftRegisterOut.B.m_uSearchLevel=wSensitivity;

    }

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

_reentrant INT Philips3WireReadBits(WORD* Bits)
{
    int i;
    int iErrorCount=0;
    INT error = E_TUNER_BUSY;
    WORD data=0;
    //set the data to be input
    HW_3WIRE_DATA_DOER = FALSE;
    Philips3WireClockDelay();
    //drive the 5757 to input mode
    HW_3WIRE_WRITE_ENABLE_DOR = FALSE;
    Philips3WireClockDelay();

//    __asm("	debug\n nop\n nop");

    if(!HW_3WIRE_DATA_DIR)
    {//if the input has gone low, that means we've tuned.
        error = TUNER_SUCCESS;

	    for(i=0;i<24;i++)
    	{
        	//toggle clock
        	Philips3WireClockDelay();
        	HW_3WIRE_CLOCK_DOR = TRUE;
        	Philips3WireClockDelay();
        	HW_3WIRE_CLOCK_DOR = FALSE;
        	Philips3WireClockDelay();

        	//get data
        	data = data <<1;
        	if(HW_3WIRE_DATA_DIR)
            	data |= 1;
        	else
            	data &=0xfffffe;
    	}
    	*Bits = data;
    }

    HW_3WIRE_WRITE_ENABLE_DOR = FALSE;
    return error;
}

_reentrant INT Philips3WireWriteBits(BOOL bSearch, WORD data)
{
    int i;
    int iErrorCount=0;
    INT error = TUNER_SUCCESS;

    HW_3WIRE_DATA_DOER = TRUE;
    HW_3WIRE_CLOCK_DOR = FALSE;
    HW_3WIRE_WRITE_ENABLE_DOR = TRUE;
    Philips3WireClockDelay();
// set the searching bit
    if(bSearch)
        HW_3WIRE_DATA_DOR = TRUE;
    else
        HW_3WIRE_DATA_DOR = FALSE;

    Philips3WireClockDelay();
    HW_3WIRE_CLOCK_DOR = TRUE;
    Philips3WireClockDelay();
   	HW_3WIRE_CLOCK_DOR = FALSE;

    //now the 24 bits
    for(i=0;i<24;i++)
    {
        Philips3WireClockDelay();
        if(data&0x800000)
            HW_3WIRE_DATA_DOR = TRUE;
        else
            HW_3WIRE_DATA_DOR = FALSE;
        data = (data<<1);
        Philips3WireClockDelay();
        HW_3WIRE_CLOCK_DOR = TRUE;
        Philips3WireClockDelay();
        HW_3WIRE_CLOCK_DOR = FALSE;
    }

	Philips3WireClockDelay();
    HW_3WIRE_WRITE_ENABLE_DOR = FALSE;
    HW_3WIRE_DATA_DOER = FALSE;
    HW_3WIRE_DATA_DOR = FALSE;
    for(i=0;i<20;i++)
    {
      Philips3WireClockDelay();
    }
    return error;
}
// take the PLL divider and returns the frequency in KHz
_reentrant INT Philips5757PllToFrequency(INT iPLL)
{
    INT iFrequency;
    
	if (iPLL != 0)
	{
		iFrequency =(INT) (((LONG) iPLL * 125L)/10l);
		iFrequency -= FMIF_FREQUENCY;
	}
	else   // search failure, set the frequency to beyond the limits 
	{
			iFrequency = 0;
	}
    return iFrequency;
}

// Take the tuning frequency in KHz, add the IF and return the PLL divider
_reentrant INT Philips5757FrequencyToPLL(INT iFrequency)
{
    INT iPLL=0;
	if (iFrequency > FM_SEARCH_HIGH_LIMIT)
		iFrequency = FM_SEARCH_HIGH_LIMIT;
	if (iFrequency < FM_SEARCH_LOW_LIMIT)
		iFrequency = FM_SEARCH_LOW_LIMIT;
	iFrequency +=FMIF_FREQUENCY;
	iPLL = (INT) (((LONG) iFrequency * 10l)/125L);
    return iPLL;
}


_reentrant INT Philips5757ReadRegister(void)
{
    INT error = TUNER_SUCCESS;
    int i;
    int PLL;
 
    if ((HW_3WIRE_DATA_DIR==TRUE)&& (g_wFMTunerError.m_bTUNER_BUSY == TRUE)) 
    	return E_TUNER_BUSY;

    error = Philips3WireReadBits(&Philips5757ShiftRegisterIn.I);

    if(error == TUNER_SUCCESS)
    {
        //update the global variables
        g_wCurrentFrequency=Philips5757PllToFrequency(Philips5757ShiftRegisterIn.B.m_uPLL0_14);
		if ((g_wFMTunerError.m_bSEARCHING_UP)||(g_wFMTunerError.m_bSEARCHING_DOWN))
			error = Philips5767ContinueSearching();
		else
		{
			g_wFMTunerError.m_bTUNER_BUSY = FALSE;
			wFM_Tuner_Search_Freq = ((g_wCurrentFrequency+FM_TUNER_GRID/2)/FM_TUNER_GRID)*FM_TUNER_GRID;// round to tuning grid
			SysPostMessage(2,MENU_TUNER_TUNED);
			TunerDriverGetTunerstate();
		}
		if ((g_wFMTunerError.m_bSCANNING_STATION) && (g_wFMTunerError.m_bTUNER_BUSY == FALSE))
			error = Philips5767ContinueScanning();
    }
    return error;
}

_reentrant INT Philips5757WriteRegister(BOOL bSearch)
{
    INT error = TUNER_SUCCESS;
 
    error = Philips3WireWriteBits(bSearch,Philips5757ShiftRegisterOut.I);
    return error;
}

_reentrant INT TunerDriverSetTunerStandby(BOOL bPowerDown)
{
    INT error = TUNER_SUCCESS;

	if (bPowerDown)
	{
		HW_3WIRE_FM_POWER_DOWN_DOR = TRUE;	// power down the FM Tuner
		g_wFMTunerError.m_bPOWERED_OFF = TRUE;
	}
	else
	{
		HW_3WIRE_FM_POWER_DOWN_DOR = FALSE;	// power up the FM Tuner
		g_wFMTunerError.m_bPOWERED_OFF = FALSE;
	}
	return error;
}

_reentrant BOOL bIsTunerModulePowered(void)
{
	return g_wFMTunerError.m_bPOWERED_OFF? FALSE: TRUE;
}

_reentrant INT TunerDriverGetTunerstate( void )
{
	g_bTunedStereoStation = HW_3WIRE_MONO_UNTUNED_DIR ? FALSE:TRUE;
	
	return TUNER_SUCCESS;
}

_reentrant INT TunerDriverScanStations( void )
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
	g_wFMTunerError.m_bSEARCHING_UP=TRUE;
	g_wFMTunerError.m_bSEARCHING_DOWN=FALSE;
	g_wFMTunerError.m_bSCANNING_STATION = TRUE;
	g_wFMTunerError.m_bBAND_LIMIT_HIGH_ERROR=FALSE;
	g_wFMTunerError.m_bBAND_LIMIT_LOW_ERROR=FALSE;
	g_wFMTunerError.m_bUNUSED_PRESET=FALSE;
	g_wFMTunerError.m_bPRESET_RANGE_ERROR=FALSE;
	/* move the tuner to lower band limit */
	wFM_Tuner_Search_Freq = FM_BAND_LIMIT_LOW_FREQ-FM_FREQ_STEP;
    Philips5757ShiftRegisterOut.B.m_bSearchUp = TRUE;
	iFM_Preset_Count = 0;
	bSetFreqBeforeSearch = TRUE;
	return  Philips5757Tuning(FALSE);  // Set the tuner frequency first, then search for stations	  
}
_reentrant INT TunerDriverSetToPresetStations( INT iPresetStation)
{
	INT error = TUNER_SUCCESS;

	g_wFMTunerError.m_bSEARCHING_UP=FALSE;
	g_wFMTunerError.m_bSEARCHING_DOWN=FALSE;
	if ((iPresetStation-1<NUMBER_OF_PRESETS)&&(iPresetStation>0))
	{
		g_wCurrentPreset = iPresetStation;
		g_wFMTunerError.m_bUNUSED_PRESET=FALSE;
		g_wFMTunerError.m_bPRESET_RANGE_ERROR=FALSE;
		wFM_Tuner_Search_Freq = g_astPresetStations[iPresetStation-1].Frequency;
		if ((wFM_Tuner_Search_Freq <= FM_BAND_LIMIT_LOW_FREQ) || (wFM_Tuner_Search_Freq >= FM_BAND_LIMIT_HIGH_FREQ))
		{
			g_wFMTunerError.m_bUNUSED_PRESET=TRUE;
			wFM_Tuner_Search_Freq = FM_BAND_LIMIT_LOW_FREQ;
		}
		error = Philips5757Tuning(FALSE);	  // tune to preset frequency
	}
	else
	{
		g_wFMTunerError.m_bPRESET_RANGE_ERROR=TRUE;
		g_wFMTunerError.m_bUNUSED_PRESET=FALSE;
	}
	return error;
}

_reentrant INT Philips5767ContinueSearching( void )
{
	INT error = TUNER_SUCCESS;
	if (g_wFMTunerError.m_bSEARCHING_UP)
	{
		if (g_wCurrentFrequency == 0)
			g_wCurrentFrequency = FM_SEARCH_HIGH_LIMIT;
		if ((g_wCurrentFrequency >= wBeginTunerSearchFreq) && (bTunerSearchHitBandLimit) && (!g_bSearchEndAtBandLimit))
		{
			wFM_Tuner_Search_Freq =  wBeginTunerSearchFreq;
			g_wFMTunerError.m_bSEARCHING_UP = FALSE;
			error = Philips5757Tuning(FALSE);
		}
		else
			if ((g_wCurrentFrequency > FM_BAND_LIMIT_HIGH_FREQ))
			{
				if (g_bSearchEndAtBandLimit)
				{	/* stop searching */
					g_wCurrentFrequency = wFM_Tuner_Search_Freq = FM_BAND_LIMIT_HIGH_FREQ;
					g_wFMTunerError.m_bBAND_LIMIT_HIGH_ERROR=TRUE;
					g_wFMTunerError.m_bSEARCHING_UP = FALSE;
					error = Philips5757Tuning(FALSE);
				}
				else
				{	/* wrap the search from the other end */
					bTunerSearchHitBandLimit = TRUE;
					g_wCurrentFrequency = wFM_Tuner_Search_Freq = FM_BAND_LIMIT_LOW_FREQ;
					SysPostMessage(2,MENU_TUNER_TUNED);
					bSetFreqBeforeSearch = TRUE;
					error = Philips5757Tuning(FALSE);
				}
			}
			else
			{
 				// check to see if the search ends at the same frequency, otherwise the search is done
				if (((g_wCurrentFrequency/FM_TUNER_GRID)*FM_TUNER_GRID)>((wFM_Tuner_Search_Freq/FM_TUNER_GRID)*FM_TUNER_GRID))
				{
					g_wFMTunerError.m_bSEARCHING_UP = FALSE;
					g_wFMTunerError.m_bTUNER_BUSY = FALSE;
					wFM_Tuner_Search_Freq = ((g_wCurrentFrequency+FM_TUNER_GRID/2)/FM_TUNER_GRID)*FM_TUNER_GRID;// round to tuning grid
					error = Philips5757Tuning(FALSE);
				}
				else
				{	// same frequency, step 1/2 grid and search again if is not set tuner to frequency before searching
					if (bSetFreqBeforeSearch)
					{
						bSetFreqBeforeSearch = FALSE;
						error = Philips5757Tuning(TRUE);
					}
					else
					{
						wFM_Tuner_Search_Freq += FM_TUNER_GRID/2;
						bSetFreqBeforeSearch = TRUE;
						error = Philips5757Tuning(FALSE);
					}
				}
			}
	}
	if (g_wFMTunerError.m_bSEARCHING_DOWN)
	{
		if (g_wCurrentFrequency == 0)
			g_wCurrentFrequency = FM_SEARCH_LOW_LIMIT;
		if ((g_wCurrentFrequency <= wBeginTunerSearchFreq) && (bTunerSearchHitBandLimit) && (!g_bSearchEndAtBandLimit))
		{
			wFM_Tuner_Search_Freq =  wBeginTunerSearchFreq;
			g_wFMTunerError.m_bSEARCHING_DOWN = FALSE;
			error = Philips5757Tuning(FALSE);
		}
		else
			if ((g_wCurrentFrequency < FM_BAND_LIMIT_LOW_FREQ))
			{
				if (g_bSearchEndAtBandLimit)
				{	/* stop searching */
					g_wCurrentFrequency = wFM_Tuner_Search_Freq = FM_BAND_LIMIT_LOW_FREQ;
					g_wFMTunerError.m_bBAND_LIMIT_LOW_ERROR=TRUE;
					g_wFMTunerError.m_bSEARCHING_DOWN = FALSE;
					error = Philips5757Tuning(FALSE);
				}
				else
				{	/* wrap the search from the other end */
					bTunerSearchHitBandLimit = TRUE;
					g_wCurrentFrequency = wFM_Tuner_Search_Freq = FM_BAND_LIMIT_HIGH_FREQ;
					SysPostMessage(2,MENU_TUNER_TUNED);
					bSetFreqBeforeSearch = TRUE;
					error = Philips5757Tuning(FALSE);
				}
			}
			else
			{
				// Check if the search ended in the same frequency
				if (((g_wCurrentFrequency/FM_TUNER_GRID)*FM_TUNER_GRID)<((wFM_Tuner_Search_Freq/FM_TUNER_GRID)*FM_TUNER_GRID))
				{
					g_wFMTunerError.m_bSEARCHING_DOWN = FALSE;
					g_wFMTunerError.m_bTUNER_BUSY = FALSE;
					wFM_Tuner_Search_Freq = ((g_wCurrentFrequency+FM_TUNER_GRID/2)/FM_TUNER_GRID)*FM_TUNER_GRID;// round to tuning grid
					error = Philips5757Tuning(FALSE);
				}
				else
				{	// step 1/2 grid and search again
					if (bSetFreqBeforeSearch)
					{
						bSetFreqBeforeSearch = FALSE;
						error = Philips5757Tuning(TRUE);
					}
					else
					{
						wFM_Tuner_Search_Freq -= FM_TUNER_GRID/2;
						bSetFreqBeforeSearch = TRUE;
						error = Philips5757Tuning(FALSE);
					}
				}
			}
	}
	return error;
}

_reentrant INT Philips5767ContinueScanning( void )
{
	INT i;
	INT error;
	INT iLowestLevel;
	INT iLowestPreset;

	if ((g_wCurrentFrequency >= FM_BAND_LIMIT_LOW_FREQ) && (g_wCurrentFrequency <= FM_BAND_LIMIT_HIGH_FREQ)) // test for valid station, IF count in the range and Level is OK
	{
		SysPostMessage(2,MENU_TUNER_TUNED);
		if (iFM_Preset_Count < NUMBER_OF_PRESETS)
		{
			g_astPresetStations[iFM_Preset_Count].Frequency = ((g_wCurrentFrequency+FM_TUNER_GRID/2)/FM_TUNER_GRID)*FM_TUNER_GRID;
			g_astPresetStations[iFM_Preset_Count++].Level = 1;
			g_wCurrentPreset = iFM_Preset_Count;
		}
		else // if the preset stations are full, Stop searching
		{
			g_wFMTunerError.m_bSCANNING_STATION = FALSE;	 // end preset scanning
			g_wCurrentPreset=1;
			error = TunerDriverTuneToFrequency(g_astPresetStations[0].Frequency);
		}
		if (g_wCurrentFrequency  < FM_BAND_LIMIT_HIGH_FREQ) // scan more station if not at the end of the FM band
        {
			error = TunerDriverSearchUp();
		}
		else	// stop preset scanning
		{
			g_wFMTunerError.m_bSCANNING_STATION = FALSE;	 // end preset scanning
			g_wCurrentPreset=1;
			error = TunerDriverTuneToFrequency(g_astPresetStations[0].Frequency);
		}
	}
	else // Arrived at tuned station that is not upto par
	{
		if ((g_wCurrentFrequency  < FM_BAND_LIMIT_HIGH_FREQ)) // scan more station if not at the end of the FM band
		{
            if (g_wCurrentFrequency < FM_BAND_LIMIT_LOW_FREQ)
                wFM_Tuner_Search_Freq = FM_BAND_LIMIT_LOW_FREQ; // Put the tuner in the scanning range
			error = TunerDriverSearchUp();
		}
		else	// stop preset scanning
		{
			g_wFMTunerError.m_bSCANNING_STATION = FALSE;	 // end preset scanning
			g_wCurrentPreset=1;
			error = TunerDriverTuneToFrequency(g_astPresetStations[0].Frequency);
		}
	}

	return error;
}

// set the tuner frequency and start tuning
_reentrant INT Philips5757Tuning(BOOL bSearch)
{
	INT error;
	g_wFMTunerError.m_bTUNER_BUSY = TRUE;
    Philips5757ShiftRegisterOut.B.m_uPLL0_14 = Philips5757FrequencyToPLL(wFM_Tuner_Search_Freq);
    error = Philips5757WriteRegister(bSearch);
	return E_TUNER_BUSY;
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
