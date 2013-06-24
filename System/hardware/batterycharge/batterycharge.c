////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2003
//
// Filename: batterycharge.c
// Description: Contains the core 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//   Includes and external references
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "batterycharge.h"
#include "regsdcdc.h"
#include "regsusb20phy.h"


////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
//The charging starts out disabled, but not permanently
BOOL g_bBatteryChargeEnabled =FALSE;

//NOTE:  maybe this PERMANENT thing should be stored in stickybits?
BOOL g_bBatteryChargePermanentlyDisabled=FALSE;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

#ifndef STMP_BUILD_PLAYER
/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          BatteryChargeInitialize
//
//   Type:          Function 
//
//   Description:   Initialize the battery charge component.
//
//   Inputs:        none
//
//   Outputs:       SUCCESS or not 
//
//   Notes:         Current does nothing as the state variables are initialized by the 
//                  compiler without needing code.
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant WORD BatteryChargeInitialize(void)
{
    //currently, nothing to do.  These variables are initialized by the compiler.
    //g_bBatteryChargeEnabled =FALSE;
    //g_bBatteryChargePermanentlyDisabled=FALSE;
    
    HW_USBPHYPWD.B.PWDIBIAS = FALSE;//required to get it to operate

    return BatteryChargeImplementationInitialize();
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          BatteryChargeIsTrickleCharging
//
//   Type:          Function 
//
//   Description:   Returns TRUE if the battery is charging in trickle mode.
//
//   Inputs:        none 
//                  
//   Outputs:       TRUE or FALSE
//
//   Notes:         Should only be used as a display mechanism.   It may lag 
//                  (by several hundred milliseconds) the true state of the charging circuit.
//<
//////////////////////////////////////////////////////////////////////////////////////////
extern BOOL g_bTrickleChargingNow;
_reentrant BOOL BatteryChargeIsTrickleCharging(void)
{
    BOOL bIsTrickleCharging = FALSE;
    if((!HW_VDD5V_PWR_CHARGE.B.PWD) && HW_VDD5V_PWR_CHARGE.B.VDD5V_PRESENT && g_bTrickleChargingNow)
        bIsTrickleCharging = TRUE;
    return bIsTrickleCharging;
}

#endif

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          BatteryChargeStopCharging
//
//   Type:          Function 
//
//   Description:   Turns off battery charge.
//
//   Inputs:        BOOL bPermanent--"permanently" disable recharge (until power off) 
//                  
//
//   Outputs:       none 
//
//   Notes:         
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant void BatteryChargeStopCharging(void)
{
    //Power off the charging
    HW_VDD5V_PWR_CHARGE.B.PWD = TRUE;
    HW_VDD5V_PWR_CHARGE.B.BATT_CURRENT = 0;
#ifndef STMP_BUILD_PLAYER
	g_bTrickleChargingNow = FALSE;
#endif
}
 
/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          BatteryChargeDisableCharging
//
//   Type:          Function 
//
//   Description:   Disables the battery charge circuit (maybe "permanently).
//
//   Inputs:        BOOL bPermanent--"permanently" disable recharge (until power off) 
//                  
//
//   Outputs:       none 
//
//   Notes:         
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant void BatteryChargeDisableCharging(BOOL bPermanent)
{
    BatteryChargeStopCharging();

    if(bPermanent)//once this is set, there's no turning back
        g_bBatteryChargePermanentlyDisabled = TRUE;

    g_bBatteryChargeEnabled = FALSE;

}


/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          BatteryChargeEnableCharging
//
//   Type:          Function 
//
//   Description:   Enables battery charging.
//
//   Inputs:        none 
//                  
//   Outputs:       none 
//
//   Notes:         
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant void BatteryChargeEnableCharging(void)
{

    g_bBatteryChargeEnabled = TRUE;

}


/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          BatteryChargeIsCharging
//
//   Type:          Function 
//
//   Description:   Returns true if the battery charging hardware is enabled, and the 
//                  current limit is greater than 0.
//
//   Inputs:        none 
//                  
//   Outputs:       none 
//
//   Notes:         Should only be used as a display mechanism.   It may lag 
//                  (by several hundred milliseconds) the true state of the charging circuit.
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant BOOL BatteryChargeIsCharging(void)
{
    BOOL bIsCharging = FALSE;
    if((!HW_VDD5V_PWR_CHARGE.B.PWD) && HW_VDD5V_PWR_CHARGE.B.VDD5V_PRESENT)
        bIsCharging = TRUE;
    return bIsCharging;
}


