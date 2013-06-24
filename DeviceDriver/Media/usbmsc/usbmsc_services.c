
#include "kernel.h"
#include "usb_api.h"
#include "usbmsc.h"
#include "gettime.h"
#include "sectorarrayhandler.h"

#ifdef DEVICE_3500
#include "regsdcdc.h"
#include "regscodec.h"
#include "regsemc.h"
#include "regsgpflash.h"
#include "regsgpio.h"
#include "batterycharge.h"
#include "regsrtc.h"
#include "persist_bits.h"
#include "misc.h"
#include "regsrevision.h"
#include "regslradc.h"
#endif 

extern _reentrant void HALDisplayZStateEnableStatic(void);
extern _reentrant void HALDisplayZStateDisableStatic(void);

extern BOOL g_bLimitedVDDD;
extern BOOL g_bUsbConfigured;


void _reentrant UsbMscBulkOutService(BOOL bSetup, BYTE btDirection, _packed BYTE _USB_MEM *pBuffer, WORD wLength,WORD wPrivateData)
{
    USBMSC_DEVICE *pDev;
    pDev= (void*)(wPrivateData&0x00ffff);
    pDev->wTransferSize = wLength;
    if(wPrivateData & SECTOR_HANDLER)
    {

        long int lCurrentTime = GetTime();
        if(lCurrentTime - g_lUSBMSCLastBulkOutTime> 1000)
        {
            g_lUSBMSCFirstBulkOutTime = lCurrentTime;
            g_lUSBMSCBulkOutBytes = 0;
        }
        g_lUSBMSCLastBulkOutTime = lCurrentTime;
        g_lUSBMSCBulkOutBytes += wLength;

        WriteSectorArrayHandlerFillSector(&pDev->SectorArray);
    }
    else
    {
        ISRReleaseObject(&pDev->TransferObject);
    }
}

void _reentrant UsbMscBulkInService(BOOL bSetup, BYTE btDirection, _packed BYTE _USB_MEM *pBuffer, WORD wLength,WORD wPrivateData)
{
    USBMSC_DEVICE *pDev;
    pDev= (void*)(wPrivateData&0x00ffff);
    pDev->wTransferSize = wLength;
    if(wPrivateData & SECTOR_HANDLER)
    {
        long int lCurrentTime = GetTime();
        if(lCurrentTime - g_lUSBMSCLastBulkInTime> 1000)
        {
            g_lUSBMSCFirstBulkInTime = lCurrentTime;
            g_lUSBMSCBulkInBytes = 0;
        }
        g_lUSBMSCLastBulkInTime = lCurrentTime;
        g_lUSBMSCBulkInBytes += wLength;

        ReadSectorArrayHandlerFreeSector(&pDev->SectorArray);
    }
    else
    {
        ISRReleaseObject(&pDev->TransferObject);
    }
}


void _reentrant UsbMscResetService(BOOL bSetup, BYTE btDirection, BYTE _USB_MEM * pbtBuffer, WORD wLength,WORD wPrivateData)
{
    //Need to release the object we might be waiting on.
    int i;
    for(i=0;i<g_wUsbMscNumDevices;i++)
    {
        USBMSC_DEVICE *pDev = UsbMscDevice;
        if(pDev->TransferObject.m_pOwner==ISR_OWNER)
            ISRReleaseObject(&pDev->TransferObject);
        pDev++;
    }
#ifdef BATTERY_CHARGE
    BatteryChargeEnableCharging();
#endif
}

#ifdef DEVICE_3500

//Suspend/Resume is only supported on the 3500
void _reentrant UsbMscResumeService(BOOL bSetup, BYTE btDirection, BYTE _USB_MEM * pbtBuffer, WORD wLength,WORD wPrivateData)
{ 
    HW_BATT_CTRL.B.PWD = 0;					//Turn on LRADC

    if( g_bLimitedVDDD ){   //  can't call ChipUpdateCapability() here since its an isr.
        HW_DCDC_VDDD.B.VOLTAGE_LEVEL = 0x1A;    //Set core voltage back to normal per device type.0x1A is 1.85V. Note TSMC should get 1.95 & chartered at 1.85V.  
        //HW_DCDC_VDDD.B.BROWNOUT_ENABLE = 0;     // Disable the brownout [if not 3502/01 device] (its already disabled by suspend function)
    }
    else{
        HW_DCDC_VDDD.B.VOLTAGE_LEVEL = 0x13;    //Set core voltage back to normal per device type.0x13 is 1.63V  
        //TODO: *** Should wait before turning on BO to prevent false BO's.  VDDD BO is not currently enabled in USBMSC. [so don't enable vddd BO here]
        // HW_DCDC_VDDD.B.BROWNOUT_ENABLE = 1;     // Re-enable the brownout
    }

    HW_REF_CTRL.B.LOW_PWR=0;                // Turn back on all the power bits
    HW_REF_CTRL.B.BIASC=0;                  // drops bias currents
    HW_REF_CTRL.B.PWRDWNS = FALSE;          // Turn down the self bias circuit
    HW_DCDC_TBR.B.DCDC_ANA_BGR_BIAS = FALSE; // Switch over to the Vbg bias voltage
    HW_DCDC_TBR.B.DCDC1_STOPCLK = FALSE;     // Turn off DCDC#1
    HW_DCDC_TBR.B.DCDC2_STOPCLK = FALSE;     // Turn off DCDC#2
    HW_VDD5V_PWR_CHARGE.B.DCANA_LP = FALSE; // Turn off some unused circuitry in the dcdc converter

    HW_FLCR2.B.CLKOFF = 0;                // Turns on clock to flash module
    HW_GPFLASH_CSR0R.B.CLK_DISABLE = 0;    

#ifdef BATTERY_CHARGE
    BatteryChargeEnableCharging();
#endif 

    HALDisplayZStateDisableStatic();

}

// Function: SuspendService()
// Inputs:   
// Outputs:  Resets the system on a chip if recharge or MMC build
// Notes: MMC builds now request 500mA initialy, with auto reduction to 100mA on suspend which resets. 
//        If high current is not granted we wait for suspend state so we can 
//        reset without MMC or battery charge features, and still operate. 
//
void _reentrant UsbMscSuspendService(BOOL bSetup, BYTE btDirection, BYTE _USB_MEM * pbtBuffer, WORD wLength,WORD wPrivateData)
{
    USHORT uStatus;
    BOOL   bLowPowerUsbStickyBitReadResult;

    // The reason for the second clause in following if statement
    // is to allow our device to pass WHQL selective suspend test.
    // WHQL selective suspend test will suspend device when it is
    // in Addressed state.  This is an unlikely scenario in the
    // real world.
	if ( ( usb_get_current_limit() > 100 ) && 
         ( FALSE == g_bUsbConfigured ) )
	{
        usb_device_get_status(USB_STATUS,&uStatus);
        if(uStatus == USB_STATE_ADDRESSED)
        {//If we're being suspended in the addressed state, that means we've likely asked for more current than allowed by the host
            //we're going to set a sticky bit (which we'll use when we reset to determine if we've been shutdown for too much 
            //current requested), and then reset the part.
            // 8/19/04 - add one bit so that we have a counter - we want to try 500mA twice before failing.            
            if( ReadStickyBit((volatile _X WORD *) &HW_RTC_PERSISTENT1, HW_RTC_PERSISTANT1_LOW_POWER2_USB_BITPOS, 
                                 &bLowPowerUsbStickyBitReadResult) != SUCCESS )
            {
                DebugBuildAssert(FALSE); // halts only in DEBUG builds.
            }
            // if 2nd try flag is set, we've failed the 2nd time, now drop down into 100mA mode.
            if (bLowPowerUsbStickyBitReadResult)
            {
                // Clear 2nd try flag
                if ( ClearStickyBit((volatile _X WORD *)&HW_RTC_PERSISTENT1, 
                            HW_RTC_PERSISTANT1_LOW_POWER2_USB_BITPOS)  != SUCCESS)
                {    
                    DebugBuildAssert(FALSE); // halts only in DEBUG build, vanishes in RETAIL build.   
                }
                // Set the 100mA flag.
                if( SetStickyBit((volatile _X WORD *)&HW_RTC_PERSISTENT1, HW_RTC_PERSISTANT1_LOW_POWER_USB_BITPOS) != SUCCESS )
                {
                    DebugBuildAssert(0); // halts only if DEBUG build. 
                }
            } else
            {
                //set the 2nd try sticky bit.
                if( SetStickyBit((volatile _X WORD *)&HW_RTC_PERSISTENT1, HW_RTC_PERSISTANT1_LOW_POWER2_USB_BITPOS) != SUCCESS )
                {
                    DebugBuildAssert(0); // halts only if DEBUG build. 
                }
            }
	
            usb_device_shutdown();
            for(uStatus =0;uStatus<10000;uStatus++)
                _nop();
                            
            //reset the part
            SystemReset();
        }
        else
        {  //DebugBuildAssert(FALSE);   //!
        }	
    }
    //#endif

	//Turn off LRADC 
    HW_BATT_CTRL.B.PWD = 1;

#ifdef DCDC_POWER_TRANSFER
    //If using hand-off code, leave the DCDC circuitry on.
    HW_VDD5V_PWR_CHARGE.B.DCANA_LP = FALSE;  
    HW_DCDC_TBR.B.DCDC2_STOPCLK = FALSE;      
    HW_DCDC_TBR.B.DCDC1_STOPCLK = FALSE;  
#else
    //If not using hand-off code, turn off circuitry.
    HW_VDD5V_PWR_CHARGE.B.DCANA_LP = TRUE;  //Turn off some unused circuitry in the dcdc converter
    HW_DCDC_TBR.B.DCDC2_STOPCLK = TRUE;  //Turn off DCDC#2
    HW_DCDC_TBR.B.DCDC1_STOPCLK = TRUE;  //Turn off DCDC#1
#endif 


    HW_DCDC_TBR.B.DCDC_ANA_BGR_BIAS = TRUE;  //Switch over to the Vbg bias voltage
    HW_REF_CTRL.B.PWRDWNS = TRUE;       // Turn down the self bias circuit
    HW_REF_CTRL.B.BIASC=1;              // drops bias currents
    HW_REF_CTRL.B.LOW_PWR=1;            // and some more.

    HW_FLCR2.B.CLKOFF = 1;                // Turns Off clock to flash module
    HW_GPFLASH_CSR0R.B.CLK_DISABLE = 1;  // Turns OFF GPFLASH

    HW_DCDC_VDDD.B.BROWNOUT_ENABLE = 0;       // Disable the brownout
    HW_DCDC_VDDD.B.VOLTAGE_LEVEL=0x0a;  //Set core to 1.34 volts

#ifdef BATTERY_CHARGE
    BatteryChargeDisableCharging(FALSE);
#endif 

    HALDisplayZStateEnableStatic();
    
}

#endif 
