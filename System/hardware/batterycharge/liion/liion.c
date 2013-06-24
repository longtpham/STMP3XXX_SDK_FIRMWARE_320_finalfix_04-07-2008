////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2003
//
// Filename: liion.c
// Description: The lithium  ion specific implementation of the battery charging 
//              algorithms
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//   Includes and external references
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "project.h"
#include "gettime.h"
#include "batterycharge.h"
#include "lowresolutionadc.h"

#include "liion.h"
#include "battery_config.h"


////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

#ifndef STMP_BUILD_PLAYER
DWORD g_ChargeTimes=0;
#define MAX_CHARGE_TIMES 600*60*4
#endif

DWORD g_dwLiIonTimeOut=0;
WORD g_LiIonState = LIION_UNKNOWN;

WORD g_wVBatt_mV_Charging    =0;
WORD g_wVBatt_mV_NotCharging =0;
WORD g_iThreshholdcount = 0;
BOOL g_bTrickleChargingNow = FALSE; // used by batterycharge.c and nimh.c. Here just a placeholder.
////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

static void do_nothing(void) {      // This function is here to make the dang FUNCLET, below, work right.
                                    // Without this here, the compiler switches back to ".ptext..." as soon
                                    // as it hits some code.  With this here, the "org p,"SYSFUNCLET_..._P"
                                    // stays in effect.
                                    // The function is declared "static" so that it's invisible outside this file.
  return;
}
#ifdef MTP_BUILD                               
#ifdef FUNCLET
  #pragma asm
    extern SysCallFunclet
  #pragma endasm
#endif    
#endif    


/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          BatteryChargeImplementatonGetCurrentLimit
//
//   Type:          Function 
//
//   Description:   Should evaluate whatever parameters are required and return a current
//                  limit for the battery recharge circuitry.
//
//   Inputs:        none
//
//   Outputs:       current limit in milliamps
//
//   Notes:         
//<
//////////////////////////////////////////////////////////////////////////////////////////

#ifdef MTP_BUILD                               
#ifdef FUNCLET
    #pragma asm
        FBatteryChargeImplementatonGetCurrentLimit:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_BATTERYCHARGEIMPLEMENTATONGETCURRENTLIMIT

        org p,"SYSFUNCLET_BatteryChargeImplementatonGetCurrentLimit_P":
        dc      RSRC_FUNCLET_BATTERYCHARGEIMPLEMENTATONGETCURRENTLIMIT
    #pragma endasm
_reentrant WORD BatteryChargeImplementatonGetCurrentLimit_Funclet(void)
#else
_reentrant WORD BatteryChargeImplementatonGetCurrentLimit(void)
#endif    
#else	// Non-MTP Build
_reentrant WORD BatteryChargeImplementatonGetCurrentLimit(void)
#endif
{
    WORD wVBatt_mV;
    WORD wCurrentLimit = 0;

    switch (g_LiIonState)
    {
    case LIION_CHARGED:
        //nothing to do.
        break;
    case LIION_CONDITIONING:
#ifdef STMP_BUILD_PLAYER
        if(GetTime() > g_dwLiIonTimeOut)
#else
        if( g_ChargeTimes > MAX_CHARGE_TIMES )
#endif

        {//we've sat conditioning the battery for too long, something must be wrong
            BatteryChargeDisableCharging(TRUE);
        }
        else
        {
            if(g_wVBatt_mV_NotCharging > LIION_CONDITIONING_VOLTAGE_THRESHHOLD)
            {
                g_LiIonState = LIION_CHARGING;
            }
            else
            {
                wCurrentLimit = LIION_CONDITIONING_CURRENT_LIMIT;
            }
        }
        break;
    case LIION_CHARGING:
#ifdef STMP_BUILD_PLAYER
        if(GetTime() > g_dwLiIonTimeOut)
#else
        if( g_ChargeTimes > MAX_CHARGE_TIMES )
#endif
        {//we've sat too long charging without reaching our threshhold, something must be wrong.
            BatteryChargeDisableCharging(TRUE);
        }
        else
        {
            if(g_iThreshholdcount > LIION_CHARGING_DIFFERENTIAL_THRESHHOLD_COUNT)
            {
                g_LiIonState = LIION_WAITING_FOR_FINISH;
                g_dwLiIonTimeOut = GetTime() + LIION_TOPOFF_TIMEOUT;
            }
            wCurrentLimit = LIION_CHARGING_CURRENT_LIMIT;
        }
        break;
    case LIION_WAITING_FOR_FINISH:
#ifdef STMP_BUILD_PLAYER
        if(GetTime() > g_dwLiIonTimeOut)
#else
        if( g_ChargeTimes > MAX_CHARGE_TIMES )
#endif
        {
            g_LiIonState = LIION_CHARGED;
            BatteryChargeDisableCharging(FALSE);
        }
        else
        {
            wCurrentLimit = LIION_CHARGING_CURRENT_LIMIT;

        }
        break;
    }
    return wCurrentLimit;
}

#ifdef MTP_BUILD                               
#ifdef FUNCLET
    #pragma asm
        ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
        org p,".ptextliion":
   #pragma endasm
#endif
#endif


/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          BatteryChargeImplementationInitialize
//
//   Type:          Function 
//
//   Description:   Initialize whatever state machine variables are required.  
//
//   Inputs:        none
//
//   Outputs:       current limit in milliamps
//
//   Notes:         
//<
//////////////////////////////////////////////////////////////////////////////////////////
#ifdef MTP_BUILD                               
#ifdef FUNCLET
    #pragma asm
        FBatteryChargeImplementationInitialize:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_BATTERYCHARGEIMPLEMENTATIONINITIALIZE

        org p,"SYSFUNCLET_BatteryChargeImplementationInitialize_P":
        dc      RSRC_FUNCLET_BATTERYCHARGEIMPLEMENTATIONINITIALIZE
    #pragma endasm
_reentrant WORD BatteryChargeImplementationInitialize_Funclet(void)
#else
_reentrant WORD BatteryChargeImplementationInitialize(void)
#endif    
#else	// Non-MTP Build
_reentrant WORD BatteryChargeImplementationInitialize(void)
#endif
{
    WORD wVBatt_mV;
    
    BatteryChargeImplementationSample(FALSE);
    wVBatt_mV = g_wVBatt_mV_NotCharging;
    if(wVBatt_mV > LIION_CONDITIONING_VOLTAGE_THRESHHOLD)
    {//Are we above the 'conditioning' voltage?
        g_LiIonState = LIION_CHARGING;
    } 
    else
    {   //lets condition.
        g_LiIonState = LIION_CONDITIONING;
    }
#if (LIION_BATTERY_VOLTAGE == LIION_BATTERY_VOLTAGE_4_1)
    HW_VDD5V_PWR_CHARGE.B.LI_TYPE=1
#endif 
    g_dwLiIonTimeOut = GetTime() + LIION_CHARGING_TIMEOUT;

#ifndef STMP_BUILD_PLAYER
    g_ChargeTimes=0;
#endif

    return SUCCESS;
}

#ifdef MTP_BUILD                               
#ifdef FUNCLET
    #pragma asm
        ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
        org p,".ptextliion":
   #pragma endasm
#endif
#endif


/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          BatteryChargeImplementationSample
//
//   Type:          Function 
//
//   Description:   Samples the required bits   
//
//   Inputs:        none
//
//   Outputs:       current limit in milliamps
//
//   Notes:         
//<
/////////////////////////////////////////////////////////////////////////////////////////
#ifdef MTP_BUILD                               
#ifdef FUNCLET
    #pragma asm
        FBatteryChargeImplementationSample:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_BATTERYCHARGEIMPLEMENTATIONSAMPLE

        org p,"SYSFUNCLET_BatteryChargeImplementationSample_P":
        dc      RSRC_FUNCLET_BATTERYCHARGEIMPLEMENTATIONSAMPLE
    #pragma endasm
_reentrant void BatteryChargeImplementationSample_Funclet(BOOL bCharging)
#else
_reentrant void BatteryChargeImplementationSample(BOOL bCharging)
#endif    
#else	// Non-MTP Build
_reentrant void BatteryChargeImplementationSample(BOOL bCharging)
#endif
{
    WORD mVBatt_mV=SysLowResolutionAdcReadVolt(BATT,FALSE);
    if(bCharging)
        g_wVBatt_mV_Charging    = mVBatt_mV;
    else
    {
        g_wVBatt_mV_NotCharging = mVBatt_mV;
        if((g_wVBatt_mV_Charging - g_wVBatt_mV_NotCharging) <= LIION_CHARGING_DIFFERENTIAL_THRESHHOLD)
            g_iThreshholdcount++;
        else
            g_iThreshholdcount = 0;
    }

    
}

#ifdef MTP_BUILD                               
#ifdef FUNCLET
    #pragma asm
        ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
        org p,".ptextliion":
   #pragma endasm
#endif
#endif
