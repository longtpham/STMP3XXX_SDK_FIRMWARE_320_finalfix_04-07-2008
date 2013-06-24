////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2004
//
// Filename:    nimh.c
// Description: The Nickel Metal Hydride specific implementation of init, 
//              get current limit, sample and helper function(s). 
//              If enabled, samples GPIO input for NiMH battery detection. 
//              Implementation of charger state machine functions.
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//   Includes and external references
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "project.h"
#include "gettime.h"
#include "batterycharge.h"
#include "lowresolutionadc.h"

#include "nimh.h"
#include "battery_config.h"
#include "regsgpio.h"

// nimh internal function
_reentrant INT CompareBatteryCasePolarityToConfigured(WORD wNimhBatteryCaseLogicLevel);
 
////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

DWORD g_dwNimhTimeOut=0;
WORD g_wNimhState = NIMH_UNKNOWN;

WORD g_wVBatt_mV_Charging    =0;
WORD g_wVBatt_mV_NotCharging =0;

WORD g_iThreshholdcount = 0;
BOOL g_bTrickleChargingNow = FALSE; // set below, read by istricklecharging api func

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

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
//   Outputs:       current limit in milliAmps
//
//   Notes:         
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant WORD BatteryChargeImplementatonGetCurrentLimit(void)
{
    WORD wVBatt_mV;
    WORD wCurrentLimit = 0;
    WORD wNimhTrickleChargeEnable = NIMH_TRICKLE_CHARGE_ENABLE;

    switch (g_wNimhState)
    {
    case NIMH_CHARGED: // so apply a trickle charge current to stay full.
        // 21mA is 0.03C prescribed trickle charge for STMP3 products. 25mA gives 0.035C.
        if( wNimhTrickleChargeEnable )
        {   wCurrentLimit = NIMH_TRICKLE_CHARGE_CURRENT;
            g_bTrickleChargingNow = TRUE; 
        }
        else //all charging is complete. 
            wCurrentLimit = 0;
        // Display batt level as full. 
        break;
        
    case NIMH_CONDITIONING:
#if 0  // state not implemented for equilibrium charge rate algo.
        if(GetTime() > g_dwNimhTimeOut)
        {//we've sat conditioning the battery for too long, something must be wrong
            BatteryChargeDisableCharging(TRUE);
        }
        else
        {   if(g_wVBatt_mV_NotCharging > NIMH_CONDITIONING_VOLTAGE_THRESHHOLD)
            {
                g_nimhState = NIMH_CHARGING;
            }
            else
            {   wCurrentLimit = NIMH_CONDITIONING_CURRENT_LIMIT;
            }
        }
#endif        
        break;

    case NIMH_CHARGING:
        if(GetTime() > g_dwNimhTimeOut)
        {   // equilibrium charge's fixed interval has completed. 
            g_wNimhState = NIMH_CHARGED;
            if( wNimhTrickleChargeEnable )
                wCurrentLimit = NIMH_TRICKLE_CHARGE_CURRENT;
            else 
            {   wCurrentLimit = 0;
                BatteryChargeDisableCharging(TRUE);                
            }
        }
        else // no quit timeout yet. Safety check for max voltage exceeded.
        {   if( g_wVBatt_mV_Charging > NIMH_VOLTAGE_THRESHHOLD_MV ) 
            {   g_wNimhState = NIMH_CHARGED;
                wCurrentLimit = 0;
                BatteryChargeDisableCharging(TRUE);   
            }
            else // state remains NIMH_CHARGING at this current level.
                wCurrentLimit = NIMH_EQUILIBRIUM_CHARGING_CURRENT_LIMIT;
        }    
        break;
        
    case NIMH_WAITING_FOR_FINISH:
#if 0 // state not implemented for equilibrium rate charging mode       
        if(GetTime() > g_dwNimhTimeOut)
        {
            g_wNimhState = NIMH_CHARGED; 
            // Display Vbatt level as full. 
            if( wNimhTrickleChargeEnable == 0)
                BatteryChargeDisableCharging(FALSE); 
            // else  Trickle charge occurs next. 
        }
        else
        {
            wCurrentLimit = NIMH_EQUILIBRIUM_CHARGING_CURRENT_LIMIT;
        }
#endif
        break;
    }
    return wCurrentLimit;
}


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
//   Outputs:       SUCCESS or if nimh case ground checked may return NIMH_POLARITY_NOT_DETECTED
//
//   Notes:         May set the g_nimhState to NIMH_CHARGING
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant WORD BatteryChargeImplementationInitialize(void)
{
    WORD wVBatt_mV; // enable below is static and can be set at run time via emulator.
    static WORD g_wBatteryCasePolarityDetectEnable = BATTERY_CASE_POLARITY_DETECT_ENABLE;
    WORD wConfiguredBattCaseGpBit = CONFIGURED_BATT_CASE_GP_BIT;
    
    BatteryChargeImplementationSample(FALSE); // instant adc result: batt voltage
    wVBatt_mV = g_wVBatt_mV_NotCharging;

    if( g_wBatteryCasePolarityDetectEnable ) // setup & check input pin next
    { 	// Enables pin to be a GPIO pin.
        CONFIGURED_BATT_CASE_GP_NUM_ENR.I |= wConfiguredBattCaseGpBit;
        
        // Set GPIO n bit m as input pin for batt case polarity check.
        CONFIGURED_BATT_CASE_GP_NUM_DOER.I &= (~(wConfiguredBattCaseGpBit)); 
        // See values in battery_config.h

        if(CompareBatteryCasePolarityToConfigured(NIMH_BATTERY_CASE_LOGIC_LEVEL) != SUCCESS)
    	{	BatteryChargeDisableCharging(TRUE); // disable of charging for this bootup.	
    		return ((WORD)NIMH_POLARITY_NOT_DETECTED); //don't charge   
    	}	
	}
    
    g_wNimhState = NIMH_CHARGING; //  next state

    g_dwNimhTimeOut = GetTime() + NIMH_EQUILIBRIUM_CHARGING_TIMEOUT_MS;
    return SUCCESS;
}




/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          BatteryChargeImplementationSample
//
//   Type:          Function 
//
//   Description:   ADC sample the required bits for batt voltage & can sample other control values.  
//
//   Inputs:        BOOL bCharging  Pass in TRUE if we are charging during this call, else pass FALSE.
//
//   Outputs:       none
//
//   Notes:         
//<
/////////////////////////////////////////////////////////////////////////////////////////
_reentrant void BatteryChargeImplementationSample(BOOL bCharging)
{
    WORD wBatt_mV = SysLowResolutionAdcReadVolt(BATT,FALSE);
    if(bCharging)
        g_wVBatt_mV_Charging    = wBatt_mV;
    else
    {
        g_wVBatt_mV_NotCharging = wBatt_mV;
//        if((g_wVBatt_mV_Charging - g_wVBatt_mV_NotCharging) <= NIMH_CHARGING_DIFFERENTIAL_THRESHHOLD)
//            g_iThreshholdcount++; // num batt samples below threshhold
//        else
//            g_iThreshholdcount = 0;
    }
  
}


/////////////////////////////////////////////////////////////////////////////////////////
// Function: CompareBatteryCasePolarityToConfigured()
// Input:    WORD wNimhBatteryCaseLogicLevel
//
// Output:   If GPIO sampled input pin logic level at nimh neg batt terminal case equals
//           configured NIMH battery case logic level, then function returns SUCCESS.
//           else returns ERROR.
//
// Notes:    private function
//
_reentrant INT CompareBatteryCasePolarityToConfigured(WORD wNimhBatteryCaseLogicLevel)
{
    //return SUCCESS; // for a simple stub version only.

    // First macro below is sampled battery case logic level from General Purpose Input pin. 
    // HW_GPnDIR.B.Bm  gives  GPIO n Data In Register.Bitfield.Bitnumber's input value. 
    // Ext Pullup R & transistor must be present for AAA's opposite logic & level shift. 
    // NiMH AA contact is (+), opposite polarity from AAA exposed cell casing (-).  

    if( CONFIGURED_GPIO_INPUT_BIT_FOR_BATTERY_CASE_POLARITY == wNimhBatteryCaseLogicLevel )
        return SUCCESS;   
    else
        return ERROR;
}

