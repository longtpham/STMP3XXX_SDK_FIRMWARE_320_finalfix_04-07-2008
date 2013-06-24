//;///////////////////////////////////////////////////////////////////////////////
//; Copyright(C) SigmaTel, Inc. 2000-2004
//;
//; Filename: battery.h
//; Description: 
//;///////////////////////////////////////////////////////////////////////////////

#ifndef _BATTERY_H
#define _BATTERY_H


// Functions  
extern _reentrant void SysLRADCBrownoutInit(WORD wLowResADCBrownoutLevel);
extern _reentrant WORD SysBatteryGetLevel(void);
extern _reentrant WORD MvToLradcResult(WORD mVolts);
extern _reentrant void SysBatteryLRADCInit(void);
extern _reentrant void SysLRADCBrownoutIsrInit(void);

// player resource drive refresh allowed only when battery usable % is above this value. 
// 50% of usable range [0.9V to 1.5V] is around 1.2V This is the safe target alkaline voltage to do a refresh.
// LIION will also require 50% which will work but that could be changed in the future. 
#define RESOURCE_REFRESH_MIN_BATT_PCT 50


#endif // _BATTERY_H




