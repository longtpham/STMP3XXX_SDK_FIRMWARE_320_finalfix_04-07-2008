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
#include "battery.h"
#include "lowresolutionadc.h"
#include "battery_config.h"

#ifdef STMP_BUILD_PLAYER
#include "regsusb20phy.h"
#include "menumanager.h"
#else
#include "kernel.h"
#include "misc.h"
#endif

#pragma optimize 1
////////////////////////////////////////////////////////////////////////////////
//   Things to parameterize
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//   Function prototypes
////////////////////////////////////////////////////////////////////////////////

_reentrant WORD BatteryChargeStateMachineInitialize(WORD wCounter);
_reentrant WORD BatteryChargeStateMachineWaitFor5V(WORD wCounter);
_reentrant WORD BatteryChargeStateMachineNormal(WORD wCounter);
_reentrant WORD BatteryChargeStateMachineBroken(WORD wCounter);

////////////////////////////////////////////////////////////////////////////////
//   External variables
////////////////////////////////////////////////////////////////////////////////
extern BOOL g_bBatteryChargeEnabled;
extern BOOL g_bBatteryChargePermanentlyDisabled;

#ifndef STMP_BUILD_PLAYER
extern BOOL g_bChargerLinearEnabled;
#endif

INT g_iBatteryChargeCounter=0;

////////////////////////////////////////////////////////////////////////////////
//   Local variables and data types 
////////////////////////////////////////////////////////////////////////////////
//used to calculate the current limit bitfield
// First element is mA charger current for this control bit, 
// Second element is bit set mask for this current control current bit.
_P BATT_CURRENT_BITS[5][2]= {
    {200,0x10},
    {100,0x08},
    { 50,0x04},
    { 25,0x02},
    { 10,0x01}
};

#define HW_VDD5V_PWR_CHARGE_BATT_CURRENT_100MA	0x08
#define HW_VDD5V_PWR_CHARGE_BATT_CURRENT_200MA	0x10

struct FunctionPointerStruct
{
    _reentrant WORD (*pFunction)(WORD);
};

#define BATTERY_CHARGE_UNINITIALIZED    0
#define BATTERY_CHARGE_WAIT_FOR_5V      1
#define BATTERY_CHARGE_NORMAL           2
#define BATTERY_CHARGE_BROKEN_BATTERY   3


_P struct FunctionPointerStruct g_BatteryChargeStateMachine[]=
    {
        {BatteryChargeStateMachineInitialize},
        {BatteryChargeStateMachineWaitFor5V},
        {BatteryChargeStateMachineNormal},
        {BatteryChargeStateMachineBroken},
    };


WORD g_wBatteryChargeState = BATTERY_CHARGE_UNINITIALIZED;
WORD g_wBatteryChargeCounter = 0;

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
//   Outputs:       none 
//
//   Notes:         Current does nothing as the state variables are initialized by the 
//                  compiler without needing code.
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant WORD BatteryChargeStateMachineBroken(WORD wCounter)
{
    return BATTERY_CHARGE_BROKEN_BATTERY;
}


/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          BatteryChargeStateMachineInitialize
//
//   Type:          State Function 
//
//   Description:   Initialize the battery charge state machine and components.
//
//   Inputs:        wCounter--ignored
//
//   Outputs:       next state in the state machine:  BATTERY_CHARGE_WAIT_FOR_5V
//
//   Notes:         
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant WORD BatteryChargeStateMachineInitialize(WORD wCounter)
{

#ifdef STMP_BUILD_PLAYER
    HW_USBPHYPWD.B.PWDIBIAS = FALSE;//required to get it to operate
    BatteryChargeImplementationInitialize();
#else
    if( g_bChargerLinearEnabled == FALSE )
        return BATTERY_CHARGE_BROKEN_BATTERY; // prevents charging attempt & LCD charging indicator.
    if( BatteryChargeInitialize() != SUCCESS )
        return BATTERY_CHARGE_BROKEN_BATTERY; // Next state allows no charging attempts until reboot.
#endif

    BatteryChargeEnableCharging();
    return BATTERY_CHARGE_WAIT_FOR_5V;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          BatteryChargeStateMachineWaitFor5V
//
//   Type:          State Function 
//
//   Description:   Stays in this state until 5v is applied, or the 'permanently disabled' bit is 
//                  set
//
//   Inputs:        wCounter--ignored
//
//   Outputs:       next state in the state machine:  
//                      BATTERY_CHARGE_WAIT_FOR_5V
//                      BATTERY_CHARGE_NORMAL
//                      BATTERY_CHARGE_BROKEN_BATTERY
//
//   Notes:         
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant WORD BatteryChargeStateMachineWaitFor5V(WORD wCounter)
{
    //Set the current to zero so that we can accurately measure the voltage
    HW_VDD5V_PWR_CHARGE.B.BATT_CURRENT = 0;
    if(g_bBatteryChargePermanentlyDisabled)
    {
        return BATTERY_CHARGE_BROKEN_BATTERY;
    } 
    else if(HW_VDD5V_PWR_CHARGE.B.VDD5V_PRESENT)
        return BATTERY_CHARGE_NORMAL;
    //else
    return BATTERY_CHARGE_WAIT_FOR_5V;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          BatteryChargeStateMachineNormal
//
//   Type:          State Function 
//
//   Description:   Call the appropriate functions (BatteryChargeImplementationSample and 
//                  BatteryChargeImplementationGetCurrentLimit) 
//
//   Inputs:        wCounter--ignored
//
//   Outputs:       next state in the state machine:  
//                      BATTERY_CHARGE_WAIT_FOR_5V
//                      BATTERY_CHARGE_NORMAL
//
//   Notes:         
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant WORD BatteryChargeStateMachineNormal(WORD wCounter)
{
    WORD wCurrentLimit;
    WORD wBitfield;
    WORD wBatteryCurrentReading;
    int i;
    WORD wReturn;

    if(wCounter>POWER_OFF_FREQUENCY)
    {
        //this will go to the state that will turn off the charging until
        //5v is inserted.
        wReturn = BATTERY_CHARGE_WAIT_FOR_5V;
    }
    else 
    {

        BatteryChargeImplementationSample(wCounter);//wCounter will be zero (false) for the first time
                                                     //this will let us sample once when the charging is off
        wCurrentLimit = BatteryChargeImplementatonGetCurrentLimit();    
        wBitfield = 0;
        if(wCurrentLimit && g_bBatteryChargeEnabled)
        {

     		//Grab the actual current setting.
			wBatteryCurrentReading = HW_VDD5V_PWR_CHARGE.B.BATT_CURRENT;

        	//Determine which bits to set to reach desired battery charge current.         
            for(i=0;i<5;i++)
            {
                if (wCurrentLimit >= BATT_CURRENT_BITS[i][0])
                {
                    wCurrentLimit -= BATT_CURRENT_BITS[i][0];
                    wBitfield |= BATT_CURRENT_BITS[i][1];
                }
            }

			//Update current bitfield if required.
			if(wBatteryCurrentReading != wBitfield)
			{
				//step up the charging to prevent large current spikes on USB.
				//step to 100mA, 200mA, and then step to final current setting.
				if(wBitfield > HW_VDD5V_PWR_CHARGE_BATT_CURRENT_100MA)
				{
		   			HW_VDD5V_PWR_CHARGE.B.BATT_CURRENT=HW_VDD5V_PWR_CHARGE_BATT_CURRENT_100MA;  //set to 100mA
			   		HW_VDD5V_PWR_CHARGE.B.PWD = FALSE;
#ifdef STMP_BUILD_PLAYER
			   		SysWaitOnEvent(0,0,1);	 //wait to settle	
#else
					SysWait(1);
#endif
	        	}

				//step to 200mA
				if(wBitfield > HW_VDD5V_PWR_CHARGE_BATT_CURRENT_200MA)
				{
		   			HW_VDD5V_PWR_CHARGE.B.BATT_CURRENT=HW_VDD5V_PWR_CHARGE_BATT_CURRENT_200MA;  //set to 200mA
			   		HW_VDD5V_PWR_CHARGE.B.PWD = FALSE;
#ifdef STMP_BUILD_PLAYER
			   		SysWaitOnEvent(0,0,1);	 //wait to settle	
#else
					SysWait(1);
#endif
	        	}
         
            	HW_VDD5V_PWR_CHARGE.B.PWD = FALSE;
            	HW_VDD5V_PWR_CHARGE.B.BATT_CURRENT=wBitfield;
			}
        }
        wReturn = BATTERY_CHARGE_NORMAL;
    }

    return wReturn;
}


/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          BatteryChargeStateMachine
//
//   Type:          State Function 
//
//   Description:   Entry Point for menus.  Callable by SysCallFunction(). Responsible for
//					ensuring proper battery charging functions are called.
//
//					Call the appropriate functions (BatteryChargeImplementationSample and 
//                  BatteryChargeImplementationGetCurrentLimit) 
//
//   Inputs:        wCounter--ignored
//
//   Outputs:       next state in the state machine:  
//                      BATTERY_CHARGE_WAIT_FOR_5V
//                      BATTERY_CHARGE_NORMAL
//
//   Notes:         
//<
//////////////////////////////////////////////////////////////////////////////////////////
#ifdef STMP_BUILD_PLAYER
_reentrant int BatteryChargeStateMachine(int a, int b, int *c)
#else
extern DWORD g_ChargeTimes;
_reentrant void BatteryChargeStateMachine(void)
#endif
{
    WORD wNewState;
 
    wNewState = g_BatteryChargeStateMachine[g_wBatteryChargeState].pFunction(g_wBatteryChargeCounter);

    if(wNewState == g_wBatteryChargeState)
        g_wBatteryChargeCounter++;
    else
        g_wBatteryChargeCounter=0;

    g_wBatteryChargeState = wNewState;

#ifdef STMP_BUILD_PLAYER
	return wNewState;
#else
    g_ChargeTimes++;
#endif

}