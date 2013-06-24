///////////////////////////////////////////////////////////////////////////////
//  Copyright(C) SigmaTel, Inc. 2000-2004
//  File        : checkremoveablemedia.c
// Description : Routine called from main loop to monitor removable media
///////////////////////////////////////////////////////////////////////////////


#include "types.h"
//#include "mediadetection.h"
#include "messages.h"
#include "exec.h"  
//#include "ddildl.h"
#include "mmcddi.h"
#include "mmchal.h"
#include "MMCHalFunctions.h"
//#include "regs3410.h" 
#include "lowresolutionadc.h"
#include "regsgpio.h"
#include "battery_config.h"

#define NUM_MMC_DETECT_WAITS    50

// This is the minimum battery voltage (in mV) at which the external device
// will operate. If battery voltage level is inferior, the external MMC
// will not be enumerated. 
// Battery type macro verified to be in scope during LIION player build option. 
// TODO: verify new LIION case to prevent MMC init if Vlion < threshhold. 
#if BATTERY_TYPE_LI_ION
#define MMC_VOLTAGE_THRESHOLD	3200        // LIION battery threshold in mV. 
#else
#define MMC_VOLTAGE_THRESHOLD	1100		// alkaline or NiMH battery threshold in mV. Was 1020mV, now 1100mV per current draw requirements.	
#endif

extern MMC_MEDIA_INFO MEDIA_DDI_MEM MmcMediaInfo;
extern WORD g_bWriteProtected;
extern MMC_PHYSICAL_BUS MmcPhysicalBus[];

BOOL g_bMMCPreviousTest;
WORD g_iCountDown = 1;
BOOL bMMCInitDone = FALSE;

_asmfunc MMCEnumerateWrapper(void*,int);
_asmfunc MMCDetectPresenceWrapper(void*);
_asmfunc MMCCheckWriteProtectWrapper(void*);

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          void _reentrant CheckRemovableMedia(void)
//
//   Type:          Function 
//
//   Description:   Checks if removeable media state has changed.
//                  If state has changed, post message to menus and clear media changed flag
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
//////////////////////////////////////////////////////////////////////////////////////////
void _reentrant CheckRemovableMedia(void)
{
  BOOL 	bMMCPresent;
  INT	iAdcBattReading;

  if ( (g_iCountDown-- == 0) || !bMMCInitDone)   // was  !iCountDown-- 
  {
      // Has the media changed?
//      bMMCPresent = MMCHalDetectPresence(MmcMediaInfo.pMMCPhysicalBus);

      bMMCPresent = MMCDetectPresenceWrapper(MmcMediaInfo.pMMCPhysicalBus);

      if ((bMMCPresent != g_bMMCPreviousTest) || !bMMCInitDone)
        {		       
		// Due to over current danger when resetting SD cards with capacities
		// larger than 256MBytes, we will not allow MMC detection if the battery
		// voltage is less than MMC_VOLTAGE_THRESHOLD. 
        #if 0
            #pragma asm
            WARN 'Note: can remove this lradc init call after verifying earlier call ok'
            #pragma endasm
            // Init the Low Resolution Analog to Digital Converter (LRADC) Channel: BATTery
            // TODO: remove this call when you have time to verify its functional w/o it since 
            //           we now call it from MiscProjectInit which is called from main. Aug3,2004.
            SysLowResolutionADCInit(BATT, OFF, VBATT_REF); // was originally in mainmenu.c but was sometimes too late so reading was 0V.
        #endif
		    // Get the current value
        	iAdcBattReading = SysLowResolutionAdcReadVolt(BATT,INSTANT_RESULT);         

			// If battery reading is 0, the ADC used to read battery is not
			// operational yet. So do nothing.
			if(iAdcBattReading == 0)
			{   //DebugBuildAssert(0);
			    return; // if this happens, MMC init does not occur this time due to low bat.
			}    
			
			// Battery voltage is inferior at threshold and the media is present
			// => Report Insufficient Power to operate external media
			if((iAdcBattReading < MMC_VOLTAGE_THRESHOLD) && (bMMCPresent))
			{
    			SysPostMessage(2,MENU_MSG_MEDIA_OVER_CUR);
			}
			else
			{   // DebugBuildAssert(0); //testpoint to verify that MMC init is invoked.
			
		        // DriveGetInfo(1, DriveInfoIsWriteProtected, &g_bWriteProtected);   
		        // Unfortunately, DriveGetInfo is not working.  We'll work around it for now
		        // by calling the HAL function directly.
		        // TT-TODO fix this hack that requires a direct call into the HAL layer.
		        g_bWriteProtected = MMCCheckWriteProtectWrapper(MmcMediaInfo.pMMCPhysicalBus);
		        SysPostMessage(2,MENU_MSG_MEDIA_CHANGED);
			}
      }

      g_bMMCPreviousTest = bMMCPresent;
      g_iCountDown = NUM_MMC_DETECT_WAITS; 
      bMMCInitDone = TRUE;     

  }
  
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          MMCInitDetectionPins
//
//   Type:          Function
//
//   Description:   Setup the GPIO pins used to interface with the MMC
//
//   Inputs:        None
//
//   Outputs:       none
//
//   Notes:         Essentially the same as MMCHalInit()
//<
////////////////////////////////////////////////////////////////////////////////
void MMCInitDetectionPins(void)
{
	// set the pointer to the Physical Bus.
    MmcMediaInfo.pMMCPhysicalBus = &(MmcPhysicalBus[0]);

    // Configure the SPI Port.  Relevant pins are as follows:
    // SPI_SCK    Pin 12
    // SPI_MISO   Pin 13
    // SPI_MOSI   Pin 14
    // SPI_SS (Slave Select). Pin 15
    HW_GP0PWR.I |= 0x00F000;        // Power Up SPI Pins
    HW_GP0ENR.I &= (WORD)(~0x00F000); // Configure SPI for functionality.

    // Power Up the Control Pins
    *(MmcPhysicalBus[0].wGpioBase + HW_GPB_GPPWR) |= MmcPhysicalBus[0].wGpioSetMask;
    *(MmcPhysicalBus[0].wGpioBase + HW_GPB_GPENR) |= MmcPhysicalBus[0].wGpioSetMask;
    *(MmcPhysicalBus[0].wWriteProtectGpioBase + HW_GPB_GPPWR) |= MmcPhysicalBus[0].wWriteProtectGpioSetMask;
    *(MmcPhysicalBus[0].wWriteProtectGpioBase + HW_GPB_GPENR) |= MmcPhysicalBus[0].wWriteProtectGpioSetMask;
    // Power up the Pin.
    *(MmcPhysicalBus[0].wMediaDetectGpioBase + HW_GPB_GPPWR) |= MmcPhysicalBus[0].wMediaDetectGpioSetMask;
    // Configure Pin as Input
    *(MmcPhysicalBus[0].wMediaDetectGpioBase + HW_GPB_GPDOER) &= MmcPhysicalBus[0].wMediaDetectGpioClearMask;
    // Enable it as GPIO
    *(MmcPhysicalBus[0].wMediaDetectGpioBase + HW_GPB_GPENR) |= MmcPhysicalBus[0].wMediaDetectGpioSetMask;
}