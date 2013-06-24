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
#include "project.h"
#include "sysserialnumber.h"
#include "regsicoll.h"
#include "regscodec.h"
#include "regsdcdc.h"
#include "regsusb20.h"
#include "regsrevision.h"
#include "regsclkctrl.h"
#include "regslradc.h"
#include "regsgpio.h"
#include "icoll.h"
#include "ipr.h"
#include "misc.h"
#include "kernel.h"
#include "batterycharge.h"
#include "battery_config.h"

////////////////////////////////////////////////////////////////////////////////
// Externs
////////////////////////////////////////////////////////////////////////////////
extern void SysBatteryGetLevel(void);
extern WORD g_wVBatt_mV_NotCharging;

//wAdcBattResolution is used for battery monitoring.  It was moved here to eliminate need 
//for LowResolutionAdcInit.c.
extern WORD    wAdcBattResolution;

////////////////////////////////////////////////////////////////////////////////
// Defs
////////////////////////////////////////////////////////////////////////////////
//Assumes HW_BATT_CTRL.I = 0x020200.  REF 5120MV
#define BOOST_NLEV_BASE_STEP  46
#define BOOST_LOW_BATT_SHUTDOWN_THRESHOLD 50
#define BUCK_LOW_BATT_SHUTDOWN_THRESHOLD 160

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
void _long_interrupt(HW_IVECVDD5VDISC/2) WallPowerDisconnected_ISR(void);
void _long_interrupt(HW_IVECVDD5VCONN/2) WallPowerConnected_ISR(void);
void _long_interrupt(ICOLL_SOURCE_BROWNOUT) SysBrownout_ISR(void);
void _reentrant ServiceDCDC(void);
////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
_X  BOOL g_bServiceDCDC = FALSE;
_X  INT g_iBattLevel = 0;

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
RETCODE MiscProjectInit(void)
{
	//Initialize Battery.  Hard code instead of using SysBatteryLRADCInit(); to save
	//space.  Use the same batt control for both boost and buck.  Setting
	//DIV2 will create a path to GND which allow the player to detect a 'no battery'
	//condition. LRADC Settings: Batt Range - 5.12V-0V; REF_VAL-10, DIV2-1
	HW_BATT_CTRL.I = 0x020200;
	
	//Initialize LRADC globals
	wAdcBattResolution = RES_REF_5120MV;

    // Generically enable serial numbers for all media
    SerialNumberEnableInit();
    
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
//   Description:   
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
//   Description:   
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
//>  Name:          EnterNonReentrantSection
//
//   Type:          Function
//
//   Description:   Stub to satisfy linker, not actually needed.
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void EnterNonReentrantSection(void)
{
    return;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          LeaveNonReentrantSection
//
//   Type:          Function
//
//   Description:   Stub to satisfy linker, not actually needed.
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void LeaveNonReentrantSection(void)
{
    return;
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
void _reentrant Init5VSense(void)
{
//Only add code if using 5V to DCDC Power Transfer
#ifdef DCDC_POWER_TRANSFER
	/* setup 5V Detect Interrupts */
	// Since we are in USBMSC, the 5V connect INT is not needed	
   IcollInstallIsrVector(WallPowerDisconnected_ISR,ICOLL_SOURCE_VDD_5V_DISCONNECTED);
   IcollInstallIsrVector(SysBrownout_ISR,ICOLL_SOURCE_BROWNOUT);

   IcollSetPriority(ICOLL_SOURCE_VDD_5V_DISCONNECTED,ICOLL_PRIORITY_000);
   IcollSetSteering(ICOLL_SOURCE_VDD_5V_DISCONNECTED,ICOLL_STEERING_IVL3);    
   IcollEnable(ICOLL_SOURCE_VDD_5V_DISCONNECTED);

   HW_IPR.B.L3P = 3;  //Must be set to highest priority
   
	// Enable the IRQB interrupt & set level
	HW_IPR.B.IRQBP = 1;
	HW_IPR.B.IRQBT = 0;

	// Turn On Interrupts
	HW_USBCSR.B.VBUSDISCXIE = 1;
	HW_USBCSR.B.VBUSCXIE = 1;
	HW_ICLENABLE0R.B.SEN20 = 1;
	HW_ICLENABLE0R.B.SEN23 = 1;

	//*******TEMP Power/LRADC Settings.  Can be Moved Later *********//
	//DCDC settings, only required if using the 5V-DCDC power hand off on disconnects.
	
	//With the 5V to DCDC power transfer, there is no VDDIO protection.  Need to add it.
	HW_DCDC_VDDIO.B.BROWNOUT_ENABLE = 1;

#ifdef BATTERY_TYPE_LI_ION
		//buck mode
		HW_DCDC1_CTRL0.B.NLEV = 0x12;
		HW_DCDC2_CTRL0.B.NLEV = 0x09;
        HW_MIXTBR.I = 0x010040;         //clock glitch stmp10060 clear HW_MIXTBR.XBGC bit was 0x012040;
		HW_DCDC_TBR.I = 0x009000;
		HW_BATT_THRSH.B.THRESHOLD_EVENT0 = BUCK_LOW_BATT_SHUTDOWN_THRESHOLD;
#else
 		//boost mode
		HW_DCDC1_CTRL0.B.NLEV = 0x01;
		HW_DCDC2_CTRL0.B.NLEV = 0x01;
		HW_MIXTBR.I = 0x010040;           //clock glitch stmp10060 clear HW_MIXTBR.XBGC bit was 0x012040;
		HW_DCDC_TBR.I = 0x009000;	
		HW_BATT_THRSH.B.THRESHOLD_EVENT0 = BOOST_LOW_BATT_SHUTDOWN_THRESHOLD;
#endif

	//reset R/C Values to correct state
    HW_DCDC1_CTRL1.B.R = 5;
	HW_DCDC1_CTRL1.B.C = 4;
	HW_DCDC2_CTRL1.B.R = 5;
	HW_DCDC2_CTRL1.B.C = 4;

    //  low power mode with Ring OSC not counting
	// RINGOSC1/2 are used to read the dc-dc duty cycle
    HW_SPEED.I &= (HW_SPEED_OSC1_START_COUNT_CLRMASK & HW_SPEED_OSC1_PWRUP_CLRMASK & HW_SPEED_OSC2_PWRUP_CLRMASK & HW_SPEED_OSC2_START_COUNT_CLRMASK);
    SysWait(1);

	//The DCDC Converters serviced to ensure the proper control state.
	if(HW_VDD5V_PWR_CHARGE.B.VDD5V_PRESENT)
	{		
   		//service the DCDC Converters
   		g_bServiceDCDC = TRUE;
    	ServiceDCDC();

	}
	else
	{	//If 5V was connected during the ROM, the PWDN_ON_IOBRNOUT bit will be set.  This bit 
		//forces the internal linear regulators to stay on.  If 5V is removed with PWDN_ON_IOBRNOUT 
		//bit set, the player will shutdown in Boost or 2 Channel Buck.  In Single channel buck, 
		//the external VDDIO regulator will actually supply power to VDDD.  If the PWDN_ON_IOBRNOUT 
		//bit is cleared before the DCDC Converter is initialized, the player will crash.  Need to 
		//service the DCDC converter before clearing PWDN_ON_IOBRNOUT to maintain power in 1 channel buck.
		if(HW_VDD5V_PWR_CHARGE.B.PWDN_ON_IOBRNOUT)
		{
   			//service the DCDC Converters
   			g_bServiceDCDC = TRUE;
			ServiceDCDC();

			HW_VDD5V_PWR_CHARGE.B.PWDN_ON_IOBRNOUT=0;

			//reset R/C Values to correct state
    		HW_DCDC1_CTRL1.B.R = 5;
    		HW_DCDC1_CTRL1.B.C = 4;
    		HW_DCDC2_CTRL1.B.R = 5;
    		HW_DCDC2_CTRL1.B.C = 4;
		}
	}
#endif 	  //DCDC_POWER_TRANSFER
}

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

  INT iBattLevel=0;
  INT iNLevel=0;

  if(g_bServiceDCDC)	//service DCDC converter if flag is set
  {	        
		if((HW_VDD5V_PWR_CHARGE.B.VDD5V_PRESENT) || (HW_VDD5V_PWR_CHARGE.B.PWDN_ON_IOBRNOUT))
		{
//Build option to service DCDC based on DCDC Mode (boost or buck)
#ifdef BATTERY_TYPE_LI_ION
    		// Open DCDC control loop and set it up to be close to desired target for disconnect.
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
            	SysWait(1);
            	HW_DCDC1_CTRL0.B.NLEV = 0x12;                
            	// Ring OSC1 should be around 0x78 at this point
				g_bServiceDCDC = FALSE;	   //control loop in a good state; stop service
				HW_VDD5V_PWR_CHARGE.B.PWDN_ON_IOBRNOUT=0;  //Clear PWDN_ON_IOBRNOUT bit
			}
        	else
        	{   // 2 Channel buck mode
		    	// Ring OSC2 should be between 0x20 and 0x30 after the loop runs a few times
            	if((HW_SPEED.B.RINGOSC2 < 0x27) || (HW_SPEED.B.RINGOSC2 > 0x30))
            	{  
               		// Control loop is in a bad state
                	HW_DCDC2_CTRL1.B.FFOR = 1;
                	SysWait(1);
                	HW_DCDC2_CTRL1.B.FFOR = 0;
            	}
				else
				{
					g_bServiceDCDC = FALSE;	   //control loop in a good state; stop service		
					HW_VDD5V_PWR_CHARGE.B.PWDN_ON_IOBRNOUT=0;  //Clear PWDN_ON_IOBRNOUT bit
					HW_DCDC2_CTRL0.B.NLEV = 0x1F;  //Now change NLEV to Buck State
				}

            	// Control loop is in a bad state
            	HW_DCDC1_CTRL0.B.NLEV = 0x1A;
            	SysWait(1);
            	HW_DCDC1_CTRL0.B.NLEV = 0x12;                
            	// Ring OSC1 should be around 0x78 at this point
        	}
#else //Boost Player
    		// Open DCDC control loop and set it up to be close to desired target for disconnect.
    		HW_DCDC1_CTRL1.B.R = 9;
	    	HW_DCDC1_CTRL1.B.C = 0;
    		HW_DCDC2_CTRL1.B.R = 9;
	    	HW_DCDC2_CTRL1.B.C = 0;

        	// boost mode
  			//Check the battery level and updated if it has changed.  Required for Boost Mode.	
  			iBattLevel=HW_BATT_RESULT.B.DATA_OUT;

            // alter NLEV and return back to step control loop
			//scale NLEV based on battery voltage.  if >1.44 NLEV=06, 1.44V-0.9V NLEV=06 to NLEV=31 
			iNLevel = 31-(iBattLevel-BOOST_NLEV_BASE_STEP);   //assumes HW_BATT_CTRL.I = 0x020200 (steps 46-72)
			if (iNLevel < 6)
				iNLevel = 6;
			else if (iNLevel > 31)
				iNLevel = 31;

            HW_DCDC1_CTRL0.B.NLEV = iNLevel; 
            
			//Control Loop in an open state, clear PWDN_ON_IOBRNOUT bit
            HW_VDD5V_PWR_CHARGE.B.PWDN_ON_IOBRNOUT=0;                       
			
			// disable battery brownout for case with 5V present and battery removed
            // ** currently disabled in usbmsc in SDK ** add code if change SDK
        	
#endif
		}
		else  //5V is not connected,  close DCDC control loop
		{
//Build option to service DCDC based on DCDC Mode (boost or buck)
#ifdef BATTERY_TYPE_LI_ION
			//close DCDC Control loop
    		HW_DCDC1_CTRL1.B.R = 5;
    		HW_DCDC1_CTRL1.B.C = 4;
    		HW_DCDC2_CTRL1.B.R = 5;
    		HW_DCDC2_CTRL1.B.C = 4;
#else
			//close DCDC Control loop
    		HW_DCDC1_CTRL1.B.R = 3;
    		HW_DCDC1_CTRL1.B.C = 4;
    		HW_DCDC2_CTRL1.B.R = 3;
    		HW_DCDC2_CTRL1.B.C = 4;
#endif
		}
  }	//end g_bServiceDCDC
#endif  //DCDC_POWER_TRANSFER


}  //end Check5V


void _long_interrupt(HW_IVECVDD5VDISC/2) WallPowerDisconnected_ISR(void)
{
	int iRCDelay=10000;  //delay for buck mode R/C
	int iVDDDRampDelay=2000;  //delay for VDDD ramp


    #pragma asm
    ; Clear the DM bit (double-precision multiply mode, bit 6) of the Mode Register
    ; This will be returned to its previous state when the RTI occurs (since the SR
    ; will be popped back off System Stack Low (SSL).
    ; Accumulator behavior is unpredictable when the DM bit is set.
    ; Next cycle after clearing it should NOT be an ALU instruction.
    andi #$BF,mr
    nop
    #pragma endasm

	//May have been in suspend mode, speed up clock.
    HW_CCR.B.DDIV_MSB = 0;
    HW_CCR.B.DDIV = 0;

	//May have been in suspend mode.  Turn on DCDC converters and return to correct speed.
    HW_VDD5V_PWR_CHARGE.B.DCANA_LP = FALSE;
	HW_DCDC_TBR.B.DCDC2_STOPCLK = FALSE;  //Turn on DCDC#2
    HW_DCDC_TBR.B.DCDC1_STOPCLK = FALSE;  //Turn on DCDC#1

#ifdef BATTERY_TYPE_LI_ION
	//Disconnect routine to close DCDC Control loop on Buck Mode Players
    HW_DCDC1_CTRL1.B.C = 4;
    HW_DCDC2_CTRL1.B.C = 4;

	//Wait about 1ms before returning the 'R' back to its battery setting.
	while(iRCDelay)
		iRCDelay--;

    HW_DCDC1_CTRL1.B.R = 3;
    HW_DCDC2_CTRL1.B.R = 3;
#else	
	//Change NLEV back if boost mode
	if ((HW_REVR.B.DCDCMODE == 0x7) || (HW_REVR.B.DCDCMODE == 0x5))
		HW_DCDC1_CTRL0.B.NLEV = 0x01;

	//Set R/C values for DCDC mode
    HW_DCDC1_CTRL1.B.R = 5;
    HW_DCDC1_CTRL1.B.C = 4;
    HW_DCDC2_CTRL1.B.R = 5;
    HW_DCDC2_CTRL1.B.C = 4;

#endif
	//shutdow USB 
	SystemShutdown();

	g_bServiceDCDC = TRUE;
		
	//Check for Low Battery
	if(HW_BATT_RESULT.B.LT_EVENT0)
	{ //if battery is low, poweroff
		SystemPowerOff();
	}
	else
	{//reset to player

		//Before reseting, ramp VDDD to proper level
		HW_DCDC_VDDD.B.VOLTAGE_LEVEL = 0x16;

		//wait for VDDD to stabalize before reseting
		while(iVDDDRampDelay)
		{
			iVDDDRampDelay--;
		}
		//Reset the player	
 		SystemReset();
	}

}

void _long_interrupt(ICOLL_SOURCE_BROWNOUT) SysBrownout_ISR(void)
{
    #pragma asm
    ; Clear the DM bit (double-precision multiply mode, bit 6) of the Mode Register
    ; This will be returned to its previous state when the RTI occurs (since the SR
    ; will be popped back off System Stack Low (SSL).
    ; Accumulator behavior is unpredictable when the DM bit is set.
    ; Next cycle after clearing it should NOT be an ALU instruction.
    andi #$BF,mr
    nop
    #pragma endasm

	//Ensure it actually was a VDDIO BO
	if(HW_DCDC_VDDIO.B.BROWNOUT_STATUS)
	{
		//shutdown USB systems 1st 
		SystemShutdown();
		//now power off
		SystemPowerOff();
	}
}
