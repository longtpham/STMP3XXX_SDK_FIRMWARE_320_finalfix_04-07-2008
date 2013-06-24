#ifndef __BATTERYCHARGE_H
#define __BATTERYCHARGE_H

//entry point for menus. function called via SysCallFunction()
#ifdef STMP_BUILD_PLAYER
_reentrant int BatteryChargeStateMachine(int a, int b, int *c);
#else
_reentrant void BatteryChargeStateMachine(void);
#endif

_reentrant WORD BatteryChargeInitialize(void);
_reentrant void BatteryChargeDisableCharging(BOOL);
_reentrant void BatteryChargeEnableCharging(void);
_reentrant BOOL BatteryChargeIsCharging(void);
_reentrant BOOL BatteryChargeIsTrickleCharging(void);

_reentrant WORD BatteryChargeImplementationInitialize(void);
_reentrant void BatteryChargeImplementationSample(BOOL);
_reentrant WORD BatteryChargeImplementatonGetCurrentLimit(void);


#endif 