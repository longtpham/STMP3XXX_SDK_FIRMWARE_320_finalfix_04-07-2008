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
extern void _reentrant ServiceDCDC(void);
extern void _asmfunc SysBrownout_ISR(void);
extern WORD g_wVBatt_mV_NotCharging;

#ifdef FUNCLET
#pragma asm
    extern SysCallFunclet
#pragma endasm
#endif

//wAdcBattResolution is used for battery monitoring.  It was moved here to eliminate need 
//for LowResolutionAdcInit.c.
extern WORD    wAdcBattResolution;

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
//
// See notes below for why this is not declared as an ISR.
//
void WallPowerDisconnected_ISR(void);

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
_X  BOOL g_bServiceDCDC = FALSE;
_X  INT g_iBattLevel = 0;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
static void donothing(void){
}

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
#ifdef FUNCLET
    #pragma asm
        FMiscProjectInit:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_MISC_PROJECT_INIT

        org p,"RSRC_FUNCLET_MISC_PROJECT_INIT_P":
        dc      RSRC_FUNCLET_MISC_PROJECT_INIT
    #pragma endasm
RETCODE _reentrant MiscProjectInit_Funclet(void)
#else
RETCODE _reentrant MiscProjectInit(void)
#endif
{
    //Initialize Battery.  Hard code instead of using SysBatteryLRADCInit(); to save
    //space.  Use the same batt control for both boost and buck.  Setting
    //DIV2 will create a path to GND which allow the player to detect a 'no battery'
    //condition. LRADC Settings: Batt Range - 5.12V-0V; REF_VAL-10, DIV2-1
    HW_BATT_CTRL.I = 0x020200;
    
    //Initialize LRADC globals
    wAdcBattResolution = RES_REF_5120MV;

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
    
    //Only add code if using 5V to DCDC Power Transfer
#ifdef DCDC_POWER_TRANSFER
    /* setup 5V Detect Interrupts */
    // Since we are in USBMSC, the 5V connect INT is not needed 
    IcollInstallIsrVector(WallPowerDisconnected_ISR,
                          ICOLL_SOURCE_VDD_5V_DISCONNECTED);
    IcollInstallIsrVector(SysBrownout_ISR,ICOLL_SOURCE_BROWNOUT);

    IcollSetPriority(ICOLL_SOURCE_VDD_5V_DISCONNECTED,ICOLL_PRIORITY_000);
    IcollSetSteering(ICOLL_SOURCE_VDD_5V_DISCONNECTED,ICOLL_STEERING_IVL3);    
    IcollEnable(ICOLL_SOURCE_VDD_5V_DISCONNECTED);
    
#endif // ifdef DCDC_POWER_TRANSFER
    
    return SUCCESS;
}
#ifdef FUNCLET
    #pragma asm
        ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
        org p,".ptextmiscproject":
   #pragma endasm
#endif
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

        org p,"RSRC_FUNCLET_INIT5VSENSE_P":
        dc      RSRC_FUNCLET_INIT5VSENSE
    #pragma endasm
void _reentrant Init5VSense_Funclet(void)
#else
void _reentrant Init5VSense(void)
#endif
{
//Only add code if using 5V to DCDC Power Transfer
#ifdef DCDC_POWER_TRANSFER
    
    HW_IPR.B.L3P = 3;  //Must be set to highest priority
   
    // Enable the IRQB interrupt & set level
    HW_IPR.B.IRQBP = 1;
    HW_IPR.B.IRQBT = 0;

    // Turn On Interrupts
    HW_USBCSR.B.VBUSDISCXIE = 1;
//    HW_USBCSR.B.VBUSCXIE = 1;
//    HW_ICLENABLE0R.B.SEN20 = 1;
//    HW_ICLENABLE0R.B.SEN23 = 1;

    //*******TEMP Power/LRADC Settings.  Can be Moved Later *********//
    //DCDC settings, only required if using the 5V-DCDC power hand off on disconnects.
    
    //With the 5V to DCDC power transfer, there is no VDDIO protection.  Need to add it.
    HW_DCDC_VDDIO.B.BROWNOUT_ENABLE = 1;

#ifdef BATTERY_TYPE_LI_ION
        //buck mode
        HW_DCDC1_CTRL0.B.NLEV = 0x12;
        HW_DCDC2_CTRL0.B.NLEV = 0x09;
        HW_MIXTBR.I = 0x010040;         //stmp10060 clear HW_MIXTBR.XBGC bit was 0x012040;
        HW_DCDC_TBR.I = 0x009000;
        HW_BATT_THRSH.B.THRESHOLD_EVENT0 = BUCK_LOW_BATT_SHUTDOWN_THRESHOLD;
#else
        //boost mode
        HW_DCDC1_CTRL0.B.NLEV = 0x01;
        HW_DCDC2_CTRL0.B.NLEV = 0x01;
        HW_MIXTBR.I = 0x010040;     //stmp10060 clear HW_MIXTBR.XBGC bit was 0x012040;
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
    {   //If 5V was connected during the ROM, the PWDN_ON_IOBRNOUT bit will be set.  This bit 
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
#endif    //DCDC_POWER_TRANSFER
}

#ifdef FUNCLET
    #pragma asm
        org p,".ptextmiscproject":
    #pragma endasm
#endif

//
// TODO: Remove this hack which is replacing the floating point calls from 
// TODO: NANDMediaAllocate.  These should never be called in mtp mode.
//
#pragma asm
    global Rfpcif12
    global Rfpcif12
    global Rfpmulf2
    global Rfpcfi21
    global Rfpmacpf2
Rfpcif12:
Rfpmulf2:
Rfpcfi21:
Rfpmacpf2:
    rts
#pragma endasm


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          WallPowerDisconnected_ISR
//
//   Type:          Function
//
//   Description:   Handle disconnect from USB bus.  While this function should
//      technically be defined as an _interrupt function it is never actually 
//      returned from so we save all the code to save/restore of all of the 
//      registers.
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void WallPowerDisconnected_ISR(void)
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
