#ifndef __TUNERDRIVER_H
#define __TUNERDRIVER_H


#include "types.h"

#define TUNER_MODE_STEREO   1
#define TUNER_MODE_MONO     0

#define E_TUNER_FUNCTION_NOT_SUPPORTED  -1
#define E_TUNER_PARAMETER_IN_ERROR      -2
#define E_TUNER_NO_COMMUNICATION        -3
#define E_TUNER_BUSY                    -4
#define TUNER_SUCCESS                   0
#define E_TUNER_POWERED_DOWN			-8

extern struct ModuleEntry *g_pTunerModuleEntry;

_reentrant INT TunerGetStatus(void);
_reentrant INT TunerDriverInit(void);
_reentrant INT TunerDriverTuneToFrequency(INT);
_reentrant INT TunerDriverIncrementUp(void );
_reentrant INT TunerDriverIncrementDown(void );
_reentrant INT TunerDriverSearchUp(void );
_reentrant INT TunerDriverSearchDown(void);
_reentrant INT TunerDriverSetMonoStereoMode(INT);
_reentrant INT TunerDriverSetSensitivity(INT);
_reentrant BOOL bIsTunerModulePowered(void);
_reentrant INT TunerDriverSetTunerStandby(BOOL);
_reentrant INT TunerDriverGetTunerstate( void );
_reentrant INT TunerDriverScanStations( void );
_reentrant INT TunerDriverSetToPresetStations( INT );
_reentrant INT TunerDriverSetPresetStations( INT );
_reentrant INT TunerDriverErasePresetStations( INT );


struct ErrorBits {
	int	m_bTUNER_BUSY:1;
	int	m_bSEARCHING_UP:1;
	int	m_bSEARCHING_DOWN:1;
	int	:1;
	int m_bSCANNING_STATION:1;
	int :3;
	int	m_bBAND_LIMIT_HIGH_ERROR:1;
	int m_bBAND_LIMIT_LOW_ERROR:1;
	int m_bUNUSED_PRESET:1;
	int m_bPRESET_RANGE_ERROR:1;
	int :1;
	int m_bTUNER_NOT_FOUND:1;
	int m_bPOWERED_OFF:1;
	};
extern struct ErrorBits g_wFMTunerError;

#endif
