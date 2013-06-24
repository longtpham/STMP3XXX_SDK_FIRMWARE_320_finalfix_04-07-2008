#ifndef _FMTUNER_MENU_H
#define _FMTUNER_MENU_H

#include "..\..\..\..\..\..\System\MsgModules\Hardware\Tuner\tunerdriver.h"

#define DISABLE_EQ  0
#define ENABLE_EQ   1

extern WORD  g_wCurrentPreset;
extern WORD g_wCurrentFrequency;
extern BOOL _X g_bTunedStereoStation;
extern BOOL _X g_bSearchEndAtBandLimit;

typedef struct PresetStation {
	WORD Frequency;
	INT	Level;
	} FMTunerPreset;
extern FMTunerPreset  g_astPresetStations[];

extern void sw_stereocontrol_reset();
#endif




