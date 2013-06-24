////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002
//
// Filename: miscproject.c
// Description: Misc Project-specific
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//   Includes and external references
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "hwequ.h"
#include "project.h"
#include "sysserialnumber.h"
#include "lowresolutionadc.h"
#include "battery.h"
#include "battery_config.h"
#include "menus.h"
#include "exec.h"
#include "batterycharge.h"
#include "messages.h"
#include "misc.h"


////////////////////////////////////////////////////////////////////////////////
// Externs
////////////////////////////////////////////////////////////////////////////////
extern void SysDelay (unsigned int wait_time);

#ifdef FUNCLET
#pragma asm
    extern SysCallFunclet
#pragma endasm
#endif
////////////////////////////////////////////////////////////////////////////////
// Defs
////////////////////////////////////////////////////////////////////////////////
#define TIMER_BATTERY_CHARGER_TIMEOUT_MS  5000	//5-Sec

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
void _long_interrupt(HW_IVECVDD5VDISC/2) WallPowerDisconnected_ISR(void);
void _long_interrupt(HW_IVECVDD5VCONN/2) WallPowerConnected_ISR(void);
void _reentrant ServiceDCDC(void);
void _reentrant UpdateBatteryCharging(void);
void _reentrant UpdateDCDCDutyCycle(void);

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
_X  BOOL g_bServiceDCDC = FALSE;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          MiscProjectInit
//
//   Type:          Function
//
//   Description:   
//
//   Inputs:        none
//
//   Outputs:       RETCODE
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant MiscProjectInit(void)
{ 
    // Init the Low Resolution Analog to Digital Converter (LRADC) Channel: BATTery
    SysLowResolutionADCInit(BATT, OFF, VBATT_REF);

	// Allow Power Transfer code to handle Brownout Inits if using a 5V Wall Adaptor
#ifndef DCDC_POWER_TRANSFER
    // Setup Battery brownout detection -- shuts down device if batteries too low
    SysLRADCBrownoutInit((WORD)(BATT_BROWNOUT));
#endif

    // Generically enable serial numbers for all media
    SerialNumberEnableInit();
    
    // Initialize the chip serial number
    ChipSerialNumberInit();

    // Initialize the internal media serial number
    InternalMediaSerialNumberInit();
   
    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          SerialNumberEnableInit
//
//   Type:          Function
//
//   Description:   This function provides a mechanism for initializing any
//                  project-specific mechanisms that may be used to enable/disable
//                  serial numbers.  The SerialNumbersEnabled function is also
//                  provided.
//
//   Inputs:        none
//
//   Outputs:       RETCODE
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant SerialNumberEnableInit(void)
{
    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          SerialNumbersEnabled
//
//   Type:          Function
//
//   Description:   This function gives the option for the system to determine
//                  at run time if serial numbers are available (possibly by
//                  sensing a GPIO).
//
//   Inputs:        none
//
//   Outputs:       BOOL        TRUE if serial numbers enabled
//                              FALSE if serial numbers disabled
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
BOOL _reentrant SerialNumbersEnabled(void)
{
    #ifdef WMAAPI_NO_DRM
        return FALSE;
    #else
        return TRUE;
    #endif    
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          UpdateBatteryCharging
//
//   Type:          Function
//
//   Description:   This function turns battery charging on/off timer and statemachine.
//
//   Inputs:        none
//
//   Outputs:       none        
//                              
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant UpdateBatteryCharging(void)
{
#ifdef BATTERY_CHARGE
	//check for 5V and turn on/off charging
	if(HW_VDD5V_PWR_CHARGE.B.VDD5V_PRESENT)
	{ 
		//5V is connected so turn on charging and timer
		BatteryChargeEnableCharging();
	   	SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_BATTERY_CHARGER,
	   		0,TIMER_BATTERY_CHARGER_TIMEOUT_MS,MENU_BATTERY_CHARGER_TIMEOUT);
	}
	else
	{
		//5V is removed so disable charging and timer
		BatteryChargeDisableCharging(FALSE);
		SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_BATTERY_CHARGER);
	}
#endif
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          Init5VSense
//
//   Type:          Function
//
//   Description:   Initialize the 5V detection interrupts and variables
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////

#ifdef FUNCLET
    #pragma asm
        FInit5VSense:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_INIT5VSENSE

        org p,"SYSFUNCLET_INIT5VSENSE_P":
        dc      RSRC_FUNCLET_INIT5VSENSE
    #pragma endasm
void _reentrant Init5VSense_Funclet(void)
#else
void _reentrant Init5VSense(void)
#endif
{
//Only add code if using 5V to DCDC Power Transfer
#ifdef DCDC_POWER_TRANSFER
	/* setup 5V Detect Interrupts */
	VECTOR(HW_IVECVDD5VDISC, (INT) WallPowerDisconnected_ISR);
	VECTOR(HW_IVECVDD5VCONN, (INT) WallPowerConnected_ISR);
	
	// Turn On Interrupts
	HW_USBCSR.B.VBUSDISCXIE = 1;
	HW_USBCSR.B.VBUSCXIE = 1;
	HW_ICLENABLE0R.B.SEN20 = 1;
	HW_ICLENABLE0R.B.SEN23 = 1;

    //  low power mode with Ring OSC not counting.  Set this up once so it 
	//  doesn't have to run every time ServiceDCDC() is called. 
    HW_SPEED.I &= (HW_SPEED_OSC1_START_COUNT_CLRMASK & HW_SPEED_OSC1_PWRUP_CLRMASK & HW_SPEED_OSC2_PWRUP_CLRMASK & HW_SPEED_OSC2_START_COUNT_CLRMASK);

    SysDelay(1);

	//Call the DCDC service routine 
	g_bServiceDCDC=TRUE;
    ServiceDCDC();


#endif	//DCDC_POWER_TRANSFER
}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextmiscproject":
    #pragma endasm
#endif

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          ServiceDCDC
//
//   Type:          Function
//
//   Description:   Service the DCDC converter to prepare for disconnect
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant ServiceDCDC(void)
{	  
//Only add code if using 5V to DCDC Power Transfer
#ifdef DCDC_POWER_TRANSFER

  // *************** BATTERY NOTE when connected to 5V *******************
  // 1 - Should add periodic battery check and update NLEV if change is noticed
  // 2 - When no battery is present, the pin will be floating creating a false value.
  //     To fix set the Divide by 2 bit in the LRADC.  This will provide a path to GND.
  //     Change configuration of SysBatteryGetLevel() to HW_BATT_CTRL - 0x020200
  // 3 - Disable battery brownout for case of 5V present and battery removed
  // **********************************************************************

  if(g_bServiceDCDC)	//only service DCDC converter if flag is set
  {	
	//If we are here, there has been a 5V event (inserted or removed).  Update the
	//battery charging status.
	UpdateBatteryCharging();

  	UpdateDCDCDutyCycle();
  }

#endif	 //DCDC_POWER_TRANSFER
}  //end ServiceDCDC

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          UpdateDCDCDutyCycle
//
//   Type:          Function
//
//   Description:   Update the DCDC Duty Cycle for open loop so the player will
//					remained powered on 5V disconnect
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
#ifdef FUNCLET
    #pragma asm
        FUpdateDCDCDutyCycle:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_UPDATEDCDCDUTYCYCLE

        org p,"SYSFUNCLET_UPDATEDCDCDUTYCYCLE_P":
        dc      RSRC_FUNCLET_UPDATEDCDCDUTYCYCLE
    #pragma endasm
void _reentrant UpdateDCDCDutyCycle_Funclet(void)
#else
void _reentrant UpdateDCDCDutyCycle(void)
#endif
{
  //INT iBattLevelPercent=0;
  INT iNLevel=0;

	// If 5V is present, open DCDC Control Loop and setup to be close to desired 
	//state when unplug to prevent shutdown.
	if((HW_VDD5V_PWR_CHARGE.B.VDD5V_PRESENT) || (HW_VDD5V_PWR_CHARGE.B.PWDN_ON_IOBRNOUT))
	{
//Build option to service DCDC based on battery type.  LIION - BUCK, else - BOOST Player
#ifdef  BATTERY_TYPE_LI_ION 		
		//Before placing DCDC converter in Open Loop, make sure DCDC2 NLEV is starting from
		//the reset setting.
		if(HW_DCDC2_CTRL0.B.NLEV != 0x09)
		{
			HW_DCDC2_CTRL0.B.NLEV = 0x09;
			SysDelay(5);
		}

    	// setup DC/DC control loop to be close to desired state when unplug to prevent shutdown.
    	HW_DCDC1_CTRL1.B.R = 9;
	    HW_DCDC1_CTRL1.B.C = 0;
    	HW_DCDC2_CTRL1.B.R = 9;
	    HW_DCDC2_CTRL1.B.C = 0;
	    		
        //Check for Single Channel Buck.  DCDCMODE=1 for 100-pin package.  DCDCMODE=3 if 144-pin package
		//is forced to single channel buck
        if ((HW_REVR.B.DCDCMODE == 0x1)||(HW_REVR.B.DCDCMODE == 0x3))
        {   
            // Control loop is in a bad state
            HW_DCDC1_CTRL0.B.NLEV = 0x1A;
            SysDelay(1);
            HW_DCDC1_CTRL0.B.NLEV = 0x12;                
            // Ring OSC1 should be around 0x78 at this point
			
			//It is possible in single channel buck to get here without 5V.  This would 
			//occur if PWDN_ON_IOBRNOUT is set without 5V applied.  If so, ServiceDCDC()
			//needs to be called again to close the DCDC control loop.	Only clear flag if
			//5V is present.  This will happen the second time UpdateDCDCDutyCycle() is called.
			if(HW_VDD5V_PWR_CHARGE.B.PWDN_ON_IOBRNOUT)
			{
				HW_VDD5V_PWR_CHARGE.B.PWDN_ON_IOBRNOUT=0;  //Clear PWDN_ON_IOBRNOUT bit
				HW_MIXTBR.B.PSRN = 1;   //Need set PSRN bit to disable fast falling edge reset
			}
			else
			{	//If we are here, this is the second time through.  Time to clear the service 
				//flag
				g_bServiceDCDC = FALSE;	   
			}
		}
		else
		{	//2 Channel buck mode.
			// Ring OSC2 should be between 0x20 and 0x30 after the loop runs a few times
            if((HW_SPEED.B.RINGOSC2 < 0x27) || (HW_SPEED.B.RINGOSC2 > 0x30))
            {  
               // Control loop is in a bad state
                HW_DCDC2_CTRL1.B.FFOR = 1;
                SysDelay(1);
                HW_DCDC2_CTRL1.B.FFOR = 0;
            }
			else
			{
				g_bServiceDCDC = FALSE;	   //control loop in a good state; stop service
				HW_DCDC2_CTRL0.B.NLEV = 0x1F;  //Now change NLEV to Buck State for DCDC2
				HW_VDD5V_PWR_CHARGE.B.PWDN_ON_IOBRNOUT=0;  //Clear PWDN_ON_IOBRNOUT bit
				HW_MIXTBR.B.PSRN = 1;   //Need set PSRN bit to disable fast falling edge reset
			}
            // Control loop is in a bad state
            HW_DCDC1_CTRL0.B.NLEV = 0x1A;
            SysDelay(1);
            HW_DCDC1_CTRL0.B.NLEV = 0x12;                
            // Ring OSC1 should be around 0x78 at this point
        }
#else //Boost Mode
	// If 5V is present, open DCDC Control Loop and setup to be close to desired 
	//state when unplug to prevent shutdown.
    	HW_DCDC1_CTRL1.B.R = 9;
	    HW_DCDC1_CTRL1.B.C = 0;
    	HW_DCDC2_CTRL1.B.R = 9;
	    HW_DCDC2_CTRL1.B.C = 0;

        if ((HW_REVR.B.DCDCMODE == 0x7) || (HW_REVR.B.DCDCMODE == 0x5))
        {   // boost mode
            // alter NLEV and return back to step control loop
			//iBattLevelPercent=SysBatteryGetLevel(); //returns current % of bat's usable range

			//Assumes .9V to 1.6V range  
			//scale NLEV based on battery voltage(0.9V-1.37V;0%-75%).  if >1.44 NLEV=06, 1.44V-0.9V NLEV=06 to NLEV=31  
			iNLevel = 31-(SysBatteryGetLevel()/3); //was iBattLevelPercent instead of direct call.
			if (iNLevel < 6)
				iNLevel = 6;
			else if (iNLevel > 31)
				iNLevel = 31;

            HW_DCDC1_CTRL0.B.NLEV = iNLevel;                  
			HW_VDD5V_PWR_CHARGE.B.PWDN_ON_IOBRNOUT=0;  //Clear PWDN_ON_IOBRNOUT bit
			HW_MIXTBR.B.PSRN = 1;   //Need set PSRN bit to disable fast falling edge reset 
			g_bServiceDCDC=FALSE;
        }
#endif
  	}	
  	else //5V is not connected.  Configure DCDC Converters for battery use. Close DCDC Control Loop.
  	{
#ifdef  BATTERY_TYPE_LI_ION
    HW_DCDC1_CTRL1.B.R = 3;
    HW_DCDC1_CTRL1.B.C = 4;
    HW_DCDC2_CTRL1.B.R = 3;
    HW_DCDC2_CTRL1.B.C = 4;
#else //Boost Mode
	//Change NLEV back if boost mode
	if ((HW_REVR.B.DCDCMODE == 0x7) || (HW_REVR.B.DCDCMODE == 0x5))
		HW_DCDC1_CTRL0.B.NLEV = 0x01; 

    HW_DCDC1_CTRL1.B.R = 5;
    HW_DCDC1_CTRL1.B.C = 4;
    HW_DCDC2_CTRL1.B.R = 5;
    HW_DCDC2_CTRL1.B.C = 4;
#endif

	g_bServiceDCDC = FALSE;	   // stop service

    //turn on battery brownout
    SysLRADCBrownoutInit((WORD)(BATT_BROWNOUT));
  	}// end if(HW_VDD5V_PWR_CHARGE.B.VDD5V_PRESENT)
}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextmiscproject":
    #pragma endasm
#endif

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          WallPowerDisconnected_ISR
//
//   Type:          ISR
//
//   Description:   Close DCDC Control loop as power is transfered to DCDC converters
//					
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void _long_interrupt(HW_IVECVDD5VDISC/2) WallPowerDisconnected_ISR(void)
{
#ifdef DCDC_POWER_TRANSFER
    int iWaitCount=10000;  

    #pragma asm
    ; Clear the DM bit (double-precision multiply mode, bit 6) of the Mode Register
    ; This will be returned to its previous state when the RTI occurs (since the SR
    ; will be popped back off System Stack Low (SSL).
    ; Accumulator behavior is unpredictable when the DM bit is set.
    ; Next cycle after clearing it should NOT be an ALU instruction.
    andi #$BF,mr
    nop
    #pragma endasm

#ifdef  BATTERY_TYPE_LI_ION 
    HW_DCDC1_CTRL1.B.C = 4;
    HW_DCDC2_CTRL1.B.C = 4;

	//Wait about 1ms before returning the 'R' back to its battery setting.
	while(iWaitCount)
		iWaitCount--;

    HW_DCDC1_CTRL1.B.R = 3;
    HW_DCDC2_CTRL1.B.R = 3;
#else //Boost Mode 
	//Change NLEV back if boost mode
	if ((HW_REVR.B.DCDCMODE == 0x7) || (HW_REVR.B.DCDCMODE == 0x5))
		HW_DCDC1_CTRL0.B.NLEV = 0x01;  

    HW_DCDC1_CTRL1.B.R = 5;
    HW_DCDC1_CTRL1.B.C = 4;
    HW_DCDC2_CTRL1.B.R = 5;
    HW_DCDC2_CTRL1.B.C = 4;
#endif
			
	//Clear the 5V Connect/Disconnect IRQ
	HW_USBCSR.B.VBUSDISCXIRQ = 1;
	HW_USBCSR.B.VBUSCXIRQ = 1;

	g_bServiceDCDC = TRUE;
#endif
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          WallPowerConnected_ISR
//
//   Type:          ISR
//
//   Description:   Open DCDC Control loop and set Service DCDC Flag
//					
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void _long_interrupt(HW_IVECVDD5VCONN/2) WallPowerConnected_ISR(void)
{    
#ifdef DCDC_POWER_TRANSFER
    int iWaitCount=1000;   //Wait for 5V to settle. 100us at 60MHz

    #pragma asm
    ; Clear the DM bit (double-precision multiply mode, bit 6) of the Mode Register
    ; This will be returned to its previous state when the RTI occurs (since the SR
    ; will be popped back off System Stack Low (SSL).
    ; Accumulator behavior is unpredictable when the DM bit is set.
    ; Next cycle after clearing it should NOT be an ALU instruction.
    andi #$BF,mr
    nop
    #pragma endasm

	//Set PWDN_ON_IOBRNOUT bit.  It will be cleared after DCDC duty 
	//cycle has been configured. PSRN must be cleared for PWDN_ON_IO_BRNOUT
	//to work properly.  
	HW_MIXTBR.B.PSRN = 0; 
	HW_VDD5V_PWR_CHARGE.B.PWDN_ON_IOBRNOUT=1;  

	//Open DCDC control loop
  	HW_DCDC1_CTRL1.B.R = 9;
    HW_DCDC1_CTRL1.B.C = 0;
   	HW_DCDC2_CTRL1.B.R = 9;
    HW_DCDC2_CTRL1.B.C = 0;

	//Clear the 5V Connect/Disconnect IRQ
	HW_USBCSR.B.VBUSDISCXIRQ = 1;
	HW_USBCSR.B.VBUSCXIRQ = 1;

	//Set flag to open DCDC Control loop
	g_bServiceDCDC = TRUE;
	
	//Wait for 5V to settle. 100us at 60MHz
	while(iWaitCount)
		iWaitCount--;
#endif
}
