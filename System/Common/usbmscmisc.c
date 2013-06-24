////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002-2003
//
// Filename:    usbmscmisc.c
// Description: Various system functions that need a better home
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//   Includes and external references
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "project.h"
#include "kernel.h"
#include "sysirq.h"
#include "icoll.h"
#include "filesystem.h"
#include "usb_api.h"
#include "usbmsc.h"
#include "usbmscrequests.h"
#include "scsi.h"
#include "mediadetection.h"
#include "bootmanager.h"
#include "misc.h"
#include "regsgpio.h"
#include "regscore.h"
#include "regsrtc.h"
#include "regsemc.h"
#include "gettime.h"
#include "regscodec.h"
#include "regslradc.h"
#include "regsclkctrl.h"
#include "regsdcdc.h"
#include "regstimer.h"
#include "regsswizzle.h"
#include "regssdram.h"
#include "regsrevision.h"
#include "regstb.h"

/****************************************************************************
 * Add code to turn off FM Tuner 
 ****************************************************************************/
#ifdef TUNER_5767  // If Philips5767 FM Tuner
#define PHILIPS_3WIRE_DELAY 25    // Thi & Tlow = 1usec.

// Changed HW_3WIRE_CLOCK_xxx from GP0/B18 to GP1/B9.  This change 
// supports the pin connections found in the STMP 3410/35xx TEA5767
// FM Tuner daughtercard Rev A and the STMP35xx EVK.
#ifndef HW_3WIRE_CLOCK_ENR
#define HW_3WIRE_CLOCK_ENR  HW_GP1ENR.B.B9
#define HW_3WIRE_CLOCK_DOER HW_GP1DOER.B.B9
#define HW_3WIRE_CLOCK_PWR  HW_GP1PWR.B.B9
#define HW_3WIRE_CLOCK_DOR  HW_GP1DOR.B.B9
#endif

#ifndef HW_3WIRE_DATA_ENR
#define HW_3WIRE_DATA_ENR  HW_GP0ENR.B.B19
#define HW_3WIRE_DATA_DOER HW_GP0DOER.B.B19
#define HW_3WIRE_DATA_DIR  HW_GP0DIR.B.B19
#define HW_3WIRE_DATA_PWR  HW_GP0PWR.B.B19
#define HW_3WIRE_DATA_DOR  HW_GP0DOR.B.B19
#endif


#ifndef HW_3WIRE_BUS_ENABLE_ENR
#define HW_3WIRE_BUS_ENABLE_ENR  HW_GP1ENR.B.B15
#define HW_3WIRE_BUS_ENABLE_DOER HW_GP1DOER.B.B15
#define HW_3WIRE_BUS_ENABLE_PWR  HW_GP1PWR.B.B15
#define HW_3WIRE_BUS_ENABLE_DOR  HW_GP1DOR.B.B15
#endif

#ifndef HW_3WIRE_WRITE_ENABLE_ENR
#define HW_3WIRE_WRITE_ENABLE_ENR  HW_GP2ENR.B.B3
#define HW_3WIRE_WRITE_ENABLE_DOER HW_GP2DOER.B.B3
#define HW_3WIRE_WRITE_ENABLE_PWR  HW_GP2PWR.B.B3
#define HW_3WIRE_WRITE_ENABLE_DOR  HW_GP2DOR.B.B3
#endif

#define HW_FMTUNER_READY_ENR  HW_GP0ENR.B.B16
#define HW_FMTUNER_READY_DOER HW_GP0DOER.B.B16
#define HW_FMTUNER_READY_PWR  HW_GP0PWR.B.B16
#define HW_FMTUNER_READY_DIR  HW_GP0DIR.B.B16
#endif // end of TUNER_5767

#ifdef TUNER_5757  // if Philips5757 FM Tuner
#define PHILIPS_3WIRE_DELAY 41  // Thi & Tlow = 1.67usec.

#ifndef HW_3WIRE_CLOCK_ENR
#define HW_3WIRE_CLOCK_ENR  HW_GP0ENR.B.B18
#define HW_3WIRE_CLOCK_DOER HW_GP0DOER.B.B18
#define HW_3WIRE_CLOCK_PWR  HW_GP0PWR.B.B18
#define HW_3WIRE_CLOCK_DOR  HW_GP0DOR.B.B18
#endif

#ifndef HW_3WIRE_DATA_ENR
#define HW_3WIRE_DATA_ENR  HW_GP0ENR.B.B19
#define HW_3WIRE_DATA_DOER HW_GP0DOER.B.B19
#define HW_3WIRE_DATA_DIR  HW_GP0DIR.B.B19
#define HW_3WIRE_DATA_PWR  HW_GP0PWR.B.B19
#define HW_3WIRE_DATA_DOR  HW_GP0DOR.B.B19
#endif
/* Note: FM_WE is on GP51 for SDK */
#ifndef HW_3WIRE_WRITE_ENABLE_ENR
#define HW_3WIRE_WRITE_ENABLE_ENR  HW_GP2ENR.B.B3
#define HW_3WIRE_WRITE_ENABLE_DOER HW_GP2DOER.B.B3
#define HW_3WIRE_WRITE_ENABLE_PWR  HW_GP2PWR.B.B3
#define HW_3WIRE_WRITE_ENABLE_DOR  HW_GP2DOR.B.B3
#endif

/* FM Tuner mono/stereo when BUS-CLOCK is low and NOT Tuned/Tuned when BUS-CLOCK is high */
#ifndef HW_3WIRE_MONO_UNTUNED_ENR
#define HW_3WIRE_MONO_UNTUNED_ENR  HW_GP1ENR.B.B15
#define HW_3WIRE_MONO_UNTUNED_DOER HW_GP1DOER.B.B15
#define HW_3WIRE_MONO_UNTUNED_PWR  HW_GP1PWR.B.B15
#define HW_3WIRE_MONO_UNTUNED_DIR  HW_GP1DIR.B.B15
#endif

/*  FM Tuner's power switch control bit */	 
#ifndef HW_3WIRE_FM_POWER_DOWN_ENR
#define HW_3WIRE_FM_POWER_DOWN_ENR  HW_GP0ENR.B.B16
#define HW_3WIRE_FM_POWER_DOWN_DOER HW_GP0DOER.B.B16
#define HW_3WIRE_FM_POWER_DOWN_PWR  HW_GP0PWR.B.B16
#define HW_3WIRE_FM_POWER_DOWN_DOR  HW_GP0DOR.B.B16
#define HW_3WIRE_FM_POWER_DOWN_DIR  HW_GP0DIR.B.B16
#endif
#endif  // end of #ifdef TUNER_5757

#if 0
void STFM1000TunerSafeStandbyMode(void);
void STFM1000I2CInit(void);
INT STFM1000WriteRegister(WORD I2C_Sub_Addr, DWORD DataOut);
#endif
/****************************************************************************/

#if defined(DEVICE_3500)
#include "regsgpflash.h"
#endif

    #pragma asm
    nolist
    include "sysmacro.asm"
    list 
    #pragma endasm
    
////////////////////////////////////////////////////////////////////////////////
// Symbol Exports
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Externs
////////////////////////////////////////////////////////////////////////////////
extern BOOL g_bLimitedVDDD;

////////////////////////////////////////////////////////////////////////////////
// defs
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
int g_iHighPwrPeriphInitMode = STRUCTURE_INIT_ONLY; // external var



////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          SystemShutdown
//
//   Type:          Function
//
//   Description:   
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant SystemShutdown(void)
{
    // Globally mask all interrupts
    SysMaskAllInterrupts();
    
    // Kill USB
    usb_device_shutdown();
    
    // Whack the ICOLL
    IcollShutDown();
    
    // Turn off all GPIO
    SysGpioDisableAll();
    
    // Turn off all Pad Power
    SysPadPowerDownAll();
    
    // Turn off the PLL
    SysPllShutdown();


    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          SysGpioDisableAll
//
//   Type:          Function
//
//   Description:   
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant SysGpioDisableAll(void)
{
    HW_GP0ENR.I = 0;
    HW_GP1ENR.I = 0;
    HW_GP2ENR.I = 0;
    HW_GP3ENR.I = 0;

    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          SysPadPowerDownAll
//
//   Type:          Function
//
//   Description:   
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant SysPadPowerDownAll(void)
{
    // TODO:  implement this     
    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          SysPllShutdown
//
//   Type:          Function
//
//   Description:   
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant SysPllShutdown(void)
{
    HW_CCR.B.CKSRC = 0;
    HW_CCR.B.PLLEN = 0;
    
    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          SystemReset
//
//   Type:          Function
//
//   Description:   Causes a STMP system on a chip reset by writing the reset control register.
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant SystemReset(void)
{
    #ifdef DEVICE_3400
        HW_RCR.I = 0x0000DE;
    #else
        HW_RCR.I = 0x0F07D0;
    #endif

    // Should never get here
//    SystemHalt();
    while(1) ;  // SystemHalt() not desirable here in RETAIL builds since it jumps to shutdown vector
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          SystemPowerOff
//
//   Type:          Function
//
//   Description:   
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant SystemPowerOff(void)
{
    //PLLEN must be cleared before shutting down
    HW_CCR.B.PLLEN = 0;
    // Set the power down bit & good-bye
    HW_CCR.B.PWDN = 1;

    // Should never get here
    SystemHalt();
}


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          DelayMs
//
//   Type:          Function
//
//   Description:   Spins for specified interval
//
//   Inputs:        WORD                Delay interval in ms
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant SysDelayMs(WORD wDelay)
{
    DWORD dwTargetTime;
    
    // Calculate the target time
    dwTargetTime = GetTime() + (DWORD)wDelay;
    
    // Do the wait thang
    while(1)
    {
        if(dwTargetTime <= GetTime())
            break;
    }
}

    //This commented out code was used as a kludge to detect the failure of the RTC startup and reboot if
    //necessary.
//void _reentrant SysDelayMs(WORD wDelay)
//{
//   INT  iCounter=0xfffff;
//    DWORD dwTargetTime;
    
    // Calculate the target time
//    dwTargetTime = GetTime() + (DWORD)wDelay;
    
    // Do the wait thang
//    while(iCounter--)
//    {
//        if(dwTargetTime <= GetTime())
//            break;
//    }
//    if(iCounter<=0)
//        HW_CCR.B.PWDN=TRUE;
//}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          GoToSleep
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
RETCODE GoToSleep(void)
{
    #pragma asm
        wait
    #pragma endasm
    
    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          PowerInit
//
//   Type:          Function
//
//   Description:   System power initialization
//
//   Inputs:        none
//
//   Outputs:       RETCODE
//
//   Notes:         This is the msc version. There is an mtp powerinit.c & a player 
//                  ver in syshwinit.asm
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE PowerInit(void)
{
#if defined(DEVICE_3410) || defined(DEVICE_3310)
    // bit 16 powers down right ADC , mic must use left ADC data 
    // bit 13 switches crystal to band gap bias current 
    // bit 9 disables fast falling edge reset on pswitch pin    **** NOT HERE because DEBUG disabled ** add to RETAIL only build
    HW_MIXTBR.I = (INT)0x010040;						   // stmp10060 clock glitch
    HW_DCDC_VDDIO.I = (INT)0x0046B; // set IO rail to 2.96V, brownout enabled (3.27V)
    // Conservative Settings for <= 48MHz, Set VddD to 1.56V, brownout to 1.45V, brownout enabled
    HW_DCDC_VDDD.I = (INT)0x000443;
    HW_DCDC_VDDA.I = (INT)0x008;          //  Set Analog Rail to 1.5V
    //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //;;;;;; It is ill advised to change these registers from default values
    //; based on simulations of dc-dc converter w/ 0.9 <= battery <= 1.6 
    //; DCDC#1 R=16384 PLVD=458752 (default) PLVU=229376 (default) NLVL=-393217 
    //; NLVL adjusted so that NLVL - 4*R >= --475137
      
    //; sdk2.105 change: Bit 16:12 (PLVU) in HW_DCDC1_CTRL0 needs to be set low 
    //;  this setting will act to limit the inductor current which will help dc-dc behavior 
    //;  under heavy load conditions.  This setting is only used by the dc-dc#1 in 
    //;  mode5 or 7, so all players can use this setting and dc-dc#1 will ignore it in other modes. 
    HW_DCDC1_CTRL0.I = (INT)0x0E0E40;      // DCDC1 Control Register A
    // based on simulations of dc-dc converter w/ 0.9 <= battery <= 1.
    // Mode7 DCDC#1 C=64,  Feed Forward = 0 (default) 
    HW_DCDC1_CTRL1.I = (INT)0x40;         // DCDC1 Control Register B
    // bit 17,16 high => dc-dc #2 clock slowed down since unused in Mode7 and 3
    // bit 11 high  => use band gap bias current
    // ** bit 7 high ** boost mode only
    // bit 4 high => dcdc#1 clk = 1.5MHz 
    // bit 3 low => duty cycle adjustment = 2 based on dcdc sims w/  0.9<= battery <= 1.6 
    HW_DCDCTBR.I = (INT)0x030890;
    // REF CTRL Updated 6.11.02 by MHenson to increase Vag to give more power output.
    // bit 15 powers down selfbias circuit 
    // other settings based on matrix lot with 0.6V peak output in mixer bypass mode
    HW_REF_CTRL.I = (INT)0x0483BD;
    HW_RAM_ROM_CFG.I = (INT)0x040800;       // Change PXRAM clock tune 
    HW_GP0PWR.I = (INT)0xFFFFFF;            // we need to power up the pads
    HW_GP1PWR.I = (INT)0xFFFFFF;
    HW_GP2PWR.I = (INT)0xFFFFFF;
    // Analog Initialization/////Init analog control regs//////////////////////////////
    HW_LRADC_CTRL.I &= (unsigned int)HW_LRADC_CTRL_BATPWD_SETMASK;  // Power on the Battery, AUX LRADC
    HW_ADCCSR.I |= (unsigned int)HW_ADCCSR_CLKGT_SETMASK;             // Power down ADC
    //HW_TMR3CR.I = HW_TMRCR_CG_SETMASK;                         // Power down TIMER3
    HW_TMR3CR.I = 0x800000;                                    // Power down TIMER3
    //HW_SDRAM_CSR.I |= HW_SDRAM_CSR_PWDN_SETMASK;              //Power down SDRAM
    HW_SDRAM_CSR.I |= (WORD)(1<<8);                           //Power down SDRAM
    HW_SWIZZLECS1R.I |= HW_SWIZZLECSR1_CLK_OFF_SETMASK;       //Power down SWIZZLE
    HW_RAM_ROM_CFG.I &= HW_RAM_ROM_CFG_PXRAM_CLK_EN_CLRMASK;  //Power down PXRAM ETR
    HW_RAM_ROM_CFG.I &= HW_RAM_ROM_CFG_PYRAM_CLK_EN_CLRMASK;  //Power down PYRAM ETR
    HW_RAM_ROM_CFG.I &= HW_RAM_ROM_CFG_ROM_CLK_EN_CLRMASK;    //Power down PROM
 
    HW_TB_CSR &= (WORD)HW_TB_CSR_TBEN_CLRMASK;                //Power down TRACE
    HW_MIXRECSELR.I = 0x000101;                                 // Reduce current draw
    #if 0 // When player migrates to SDK3.000, these values will need to be used.  really??
    HW_CCR.I &= HW_CCR_DACDIV_CLRMASK;
    HW_CCR.I |= (WORD)(1<<HW_CCR_DACDIV_BITPOS);              // Divide DAC clock by 1.5
    HW_MIXPWRDNR.I = 0x000a00;                                // Power Down Mixer (Use DAC Bypass),
    HW_MIXMASTERVR = HW_MIXMASTERVR_MUTE_SETMASK;             // Always make sure that the volume starts muted
    HW_MIXMICINVR = 0x008048;                                 // Mic mute=1, +20db=1, vol=01000
    HW_MIXLINE1INVR = 0x008808;                               // LineIn mute=1, lvol=01000, rvol=01000
    HW_MIXLINE2INVR = 0x008808;                               // FMIn mute=1, lvol=01000, rvol=01000
    #endif
#else // STMP3500 family assumed.
    // usbmsc is to have same vddd for all 35xx family members but vddd limited ones. Keeps usb current in usb spec.
    if(g_bLimitedVDDD) // eg 3502/01 device
    {   //HW_DCDCTBR.I = (INT)0x008000;                     // Not nedded
        //HW_DCDC_VDDD.B.VOLTAGE_LEVEL = 0x1A;      //0x1A is 1.85V; history: 0x1F = 2.02V
        // Vddd level is already set in chip capability for vddd limited cases. 
        // Vddd BO is disabled in usb mode.
    }
    else // all other 35xx family members
    {   HW_DCDC_VDDD.B.BROWNOUT_LEVEL = 0x0b;       // Brownout set to 1.37V. VDDD BO is disabled in MSC so this is moot.
        
        // VDDD was previously set to 1.50V. fDCLK=60MHz assumed. 
        // 1.54V is 0x0f. Max unstable voltage per results from 35xx matrix lot in a few temperature edge cases. 
        // 1.63V is 0x13. VDDD safest is 1.63V. Use 1.60V for less USB current.
        HW_DCDC_VDDD.B.VOLTAGE_LEVEL = 0x12;        // VDDD set to 1.60V (0x12).  Usb current checked by JLN. 
        //STMP00006310 soln: new logic prevents this from overwriting vddd level in limited vddd case.
    }   
    HW_DCDC_VDDIO.I = 0x0810;                       // VDDIO set to 3.07V

    HW_CCR.B.XTLEN = 0;                             // Turn OFF Analog clocks
    HW_CCR.B.ACKEN = 0;                             // To reduce current
    PowerDownFMTuners();                            // Turn off FM Tuner to reduce current

// This is not needed with the DCDC-usb5v Hand-off code. Here in case a customer 
// wants to use the hardware Auto-Restart instead of the hand-off. 
// This code is added to allow the player to automatially come up when the USB cable is removed and the device is restarted.
// bit 9 Auto restart will invoke pswitch when the 5V source is removed
// NOTES:
// 1 - If enabled VDDIO will not go to GND.  May Create MMC hangs or other peripheral problems.
// 2 - Must disable Auto-Restart in player
 
    //HW_DCDC_PERSIST.B.SLEEP_XTAL_ENABLE = 1;
    //HW_DCDC_PERSIST.B.AUTO_RESTART = 1;
    //HW_DCDC_PERSIST.B.UPDATE = 1;
#endif

    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          PllInit
//
//   Type:          Function
//
//   Description:   PLL initialization
//
//   Inputs:        none
//
//   Outputs:       RETCODE
//
//   Notes:         Only used in the 3410 build
//<
////////////////////////////////////////////////////////////////////////////////
#if defined(DEVICE_3410) || defined(DEVICE_3310)
RETCODE PllInit(void)
{

    // If we have a 3410 

    if(ProgramPll(0x006000) != SUCCESS)
    {
        SystemHalt();
    }
    
    
  
    return SUCCESS;
    
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          ProgramPll
//
//   Type:          Function
//
//   Description:   Set the Clock Control Register for Phase Locked Loop mode, 
//                  delay >1ms, switch DCLK to PLL.
//
//   Inputs:        INT iPdiv          Value for HW_CCR PDIV field
//
//   Outputs:       RETCODE            SUCCESS
//
//   Notes:         Only used in the 3410 build
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE ProgramPll(INT iPdiv)
{
    HW_MIXTBR.B.PCPCD = 1;  // aw jitter -- charge pump current decrease

    // Get it started
    HW_CCR.I = iPdiv|HW_CCR_CKRST_SETMASK|HW_CCR_PLLEN_SETMASK|HW_CCR_ACKEN_SETMASK;

    // Start the lock timer counter
    HW_CCR.B.LTC = 1;
    
    // IC designers state 1ms delay required here. At 24.576MHz, 1 ms is 24576 osc cycles.
    // Only need to wait for 12.288k but what the heck. ??
    // Asm loop below delays for at least 30000 oscillator cycles.
    // TODO:This asm loop might be replaced with a C function but cycle counts are so precise.
#pragma asm
    clr     a
    move    #>$001388,a0    ; 5000 decimal iterations
_wait_loop
    dec     a               ; 2   oscillator cycles
    jne     _wait_loop      ; 4+x oscillator cycles so >= 6 cycles per loop
                            ; 6 cyclesPerLoop * 5000 loops = 30000 cycles
#pragma endasm
    
    // We're locked so change the digital clock source to the PLL
    HW_CCR.B.CKSRC = 1;
    
    return SUCCESS;
}
#endif

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          GetDclkCount
//
//   Type:          Function
//
//   Description:   Duh
//
//   Inputs:        none          
//
//   Outputs:       DWORD               DCLK count
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
DWORD GetDclkCount(void)
{
    return((((DWORD)(HW_DCLKCNTU.I))<<24) | ((DWORD)(HW_DCLKCNTL.I)));
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          SetDclkCount
//
//   Type:          Function
//
//   Description:   Duh
//
//   Inputs:        DWORD               DCLK count          
//
//   Outputs:       none               
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void SetDclkCount(DWORD dwCount)
{
    // TODO:  protect this from interrupts
    HW_DCLKCNTU.I = (WORD)(dwCount >> 24);
    HW_DCLKCNTL.I = (WORD)(dwCount);
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          SmartMediaSetPortTiming
//
//   Type:          Function
//
//   Description:   Duh
//
//   Inputs:        WORD               SmartMedia port timing          
//
//   Outputs:       RETCODE            SUCCESS   
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE SmartMediaSetPortTiming(WORD wTiming)
{
    // TODO:  validate the input
    
    // Set the timing
    HW_FLSMTMR1R.I = wTiming;
    
    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          CompactFlashSetPortTiming
//
//   Type:          Function
//
//   Description:   Duh
//
//   Inputs:        WORD               Compact Flash port timing          
//
//   Outputs:       RETCODE            SUCCESS   
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE CompactFlashSetPortTiming(WORD wTiming)
{

    // Set the timing
    HW_FLCFTMR1R.I = wTiming;
    
    return SUCCESS;
}



/****************************************************************************
 * Add code to turn off FM Tuner 
 ****************************************************************************/
void _reentrant PowerDownFMTuners(void)
{
// if TEA5757 tuner
#ifdef TUNER_5757
//  enable the Power control GPIO and turn off the FET
	HW_3WIRE_FM_POWER_DOWN_PWR = TRUE;
	HW_3WIRE_FM_POWER_DOWN_ENR = TRUE;
    HW_3WIRE_FM_POWER_DOWN_DOER = TRUE;
    HW_3WIRE_FM_POWER_DOWN_DOR = TRUE;	

	HW_3WIRE_CLOCK_PWR = TRUE;
	HW_3WIRE_DATA_PWR = TRUE;
	HW_3WIRE_WRITE_ENABLE_PWR = TRUE;
    HW_3WIRE_MONO_UNTUNED_PWR = TRUE;

	HW_3WIRE_CLOCK_ENR = TRUE;
	HW_3WIRE_DATA_ENR = TRUE;
	HW_3WIRE_WRITE_ENABLE_ENR = TRUE;
    HW_3WIRE_MONO_UNTUNED_ENR = TRUE;

	HW_3WIRE_CLOCK_DOER = TRUE;
	HW_3WIRE_DATA_DOER = FALSE;
	HW_3WIRE_WRITE_ENABLE_DOER = TRUE;
    HW_3WIRE_MONO_UNTUNED_DOER = FALSE;

	HW_3WIRE_CLOCK_DOR = FALSE;
	HW_3WIRE_WRITE_ENABLE_DOR = FALSE;
#endif
#ifdef TUNER_5767
    int i;
// if TEA5767 tuner
// power up the GPIO and send the serial command to the tuner to shutdown
    HW_3WIRE_CLOCK_PWR = TRUE;
    HW_3WIRE_DATA_PWR  = TRUE;
    HW_3WIRE_WRITE_ENABLE_PWR = TRUE;
#ifdef HW_3WIRE_BUS_ENABLE_PWR 
    HW_3WIRE_BUS_ENABLE_PWR = TRUE;
#endif 
  

    //make them all gpio
    HW_3WIRE_CLOCK_ENR = TRUE;
    HW_3WIRE_DATA_ENR  = TRUE;
    HW_3WIRE_WRITE_ENABLE_ENR = TRUE;
#ifdef HW_3WIRE_BUS_ENABLE_ENR 
    HW_3WIRE_BUS_ENABLE_ENR = TRUE;
#endif 

    //Set the initial state    
    HW_3WIRE_CLOCK_DOR = FALSE;
    HW_3WIRE_DATA_DOR = TRUE;
    HW_3WIRE_WRITE_ENABLE_DOR = TRUE;
#ifdef HW_3WIRE_BUS_ENABLE_DOR 
    HW_3WIRE_BUS_ENABLE_DOR = TRUE;
#endif 
    
    //make all output
    HW_3WIRE_DATA_DOER = TRUE;
    HW_3WIRE_CLOCK_DOER = TRUE;
    HW_3WIRE_WRITE_ENABLE_DOER = TRUE;
#ifdef HW_3WIRE_BUS_ENABLE_DOER 
    HW_3WIRE_BUS_ENABLE_DOER = TRUE;
#endif 
     
    //drive the 5767 to input mode
    Philips5767_3WireClockDelay();
    HW_3WIRE_WRITE_ENABLE_DOR = TRUE;
    Philips5767_3WireClockDelay();
    HW_3WIRE_WRITE_ENABLE_DOR = FALSE;
    Philips5767_3WireClockDelay();
    HW_3WIRE_WRITE_ENABLE_DOR = TRUE;
    Philips5767_3WireClockDelay();
    
// write serial data to TEA5767 to put it in the standby mode
	Philips5767_3WireWriteByte(0x80);	// Mute, search mode, the tuner and PLL high byte
	Philips5767_3WireWriteByte(0x00);	// PLL low byte
	Philips5767_3WireWriteByte(0x00);	// SUD, SSLx, HLSI, MS, ML, MR, SWP1
	Philips5767_3WireWriteByte(0x51);	// SWP2, STANDBY, BL, XTAL, SMUTE, HCC, SNC, SI
	Philips5767_3WireWriteByte(0x00);	// PLLREF, DTC, 0, 0, 0, 0, 0, 0
    HW_3WIRE_WRITE_ENABLE_DOR = TRUE;

#endif
   // STFM1000I2CInit();
   // STFM1000TunerSafeStandbyMode();

}

/****************************************************************************
 *
 *
 *
  ****************************************************************************/ 
#ifdef TUNER_5767
void _reentrant Philips5767_3WireWriteByte(INT data)
{

    int iDataShift;
    
    //_asm ( " debug");

    for(iDataShift=0;iDataShift<8;iDataShift++)
    {
        if(data&0x80)
            HW_3WIRE_DATA_DOR = TRUE;
        else
            HW_3WIRE_DATA_DOR = FALSE;
        data = (data<<1)&0xfe;
        Philips5767_3WireClockDelay();
        HW_3WIRE_CLOCK_DOR = TRUE;
        Philips5767_3WireClockDelay();
        HW_3WIRE_CLOCK_DOR = FALSE;
    } // end of for loop
    Philips5767_3WireClockDelay();
    Philips5767_3WireClockDelay();
}


void _reentrant Philips5767_3WireClockDelay(void)
{
   int i;
   for(i=0;i<PHILIPS_3WIRE_DELAY;i++)
   {
       _asm(" nop");
   };
} 
#endif

#if 0
#define TIMEOUT_COUNT  0x4000

#define HW_I2C_BASEADDR (0xFFE5)
typedef union               /* I2C Clock Divider Register */
{
    struct {
        int                :1; 
        unsigned FACT      :8;
    } B;
  
    unsigned U;
} i2cdivr_type;
#define HW_I2CDIV (*(volatile i2cdivr_type _X*) (HW_I2C_BASEADDR))       /* I2C Divfact Registers        */

typedef union               /* I2C Data Register */
{
    struct {
        unsigned int BYTE0 :8;
        unsigned int BYTE1 :8;
        unsigned int BYTE2 :8;
    } B;
     
    int I;
    unsigned U;
} i2cdatr_type;
#define HW_I2CDAT (*(volatile i2cdatr_type _X*) (HW_I2C_BASEADDR+1)) /* I2C Data Registers (I2CDAT)      */

typedef union               /* I2C Control Register         */
{
    struct {
        int I2C_EN      :1; /* Peripheral Enable            */
        int RIE         :1; /* Receiver Interrupt Enable        */
        int BUSY        :1; /* I2C Bus Busy             */
        int ARBLOST     :1; /* Aritration lost          */
        int TIE         :1; /* Transmitter Interrupt Enable     */
        int MODE        :1; /* Operating Mode Bit           */
        int RDR         :1; /* Receiver Data Ready          */
       int TDE         :1; /* Transmitter Data Empty       */
       int RWN         :1; /* Read/Not Write           */
       unsigned WL     :2; /* Word Length              */
        int TREQ        :1; /* DSP Transmit Request         */
        int ROFL        :1; /* Receiver Overflow            */
        int TUFL        :1; /* Transmitter Underflow        */
        int ACKF        :1; /* Acknowledge Failure          */
        unsigned BCNT   :2; /* Byte Count               */
        int LWORD       :1; /* Last Word                */
        int SUBA        :1; /* Sub Address              */
        int ROFLCL      :1; /* Receiver Overflow Clear      */
        int TUFLCL      :1; /* Transmitter Underflow Clear      */
        int ONEBYTE     :1; /* Special One Data Byte Transmission Mode */
    } B;
    int I;
    unsigned U;
} i2ccsr_type;
#define HW_I2CCSR (*(volatile i2ccsr_type _X*) (HW_I2C_BASEADDR+2)) /* I2C Control/Status Register (I2CCSR) */


typedef union                
{
    DWORD D;
    struct {
        unsigned int W1;    // LSB
        unsigned int W0;    // MSB 
    } U;
 } DWORD_type;

typedef union               /* I2C Data Register */
{
    struct {
        unsigned int BYTE2 :8;  // LSB
        unsigned int BYTE1 :8;
        unsigned int BYTE0 :8;  // MSB 
    } B;
     
    int I;
    unsigned U;
} i2cdata_type;



void STFM1000TunerSafeStandbyMode(void)
{
        
    STFM1000WriteRegister(DATAPATH_REG_ADDR,    //0x5C,         // DATAPATH_REG_ADDR,
                          0x10020100);  // 0x00010210);
    
    STFM1000WriteRegister(REF_REG_ADDR, //0x28,         // REF_REG_ADDR,
                          0x00000000);

    STFM1000WriteRegister(LNA_REG_ADDR, //0x2C,         // LNA_REG_ADDR,
                          0x00000000);
   
    STFM1000WriteRegister(MIXFILT_REG_ADDR, //0x30,         // MIXFILT_REG_ADDR,
                          0x00000000);
    STFM1000WriteRegister(CLK1_REG_ADDR,    //0x34,         // CLK1_REG_ADDR,
                          0x00000000);
    STFM1000WriteRegister(CLK2_REG_ADDR,    //0x38,         // CLK2_REG_ADDR,
                          0x00000000);

    STFM1000WriteRegister(ADC_REG_ADDR, //0x3C,         // ADC_REG_ADDR,
                          0x00000000);
    return;
}
void STFM1000I2CInit(void)
{
    i2cdata_type i2ccsr;       // local copy of HW_I2CCSR
    int speed;

    HW_GP0ENR.B.B16 = 0;        // enable output
    HW_GP0ENR.B.B17 = 0;        // enable output

    HW_GP0PWR.B.B16 = 1;        // power on
    HW_GP0PWR.B.B17 = 1;        // power on
     
    HW_I2CCSR.I = 0x000001;     // clear all bits except Peripheral Enable 
 
    HW_I2CDIV.U = 0x9C;     // Clock Divider Register (e.g. 0x78) Write Only 
                            // Cannot be read                      
    HW_I2CCSR.B.MODE = 0;    // Operating Mode Bit  1=Fast
  
    return;
}

INT STFM1000WriteRegister(WORD I2C_Sub_Addr, DWORD DataOut)
{        
    DWORD_type MyData;
    i2cdata_type i2cdata;       
    INT cnt;
    MyData.D = DataOut;
    for(cnt = 0; (0 != HW_I2CCSR.B.BUSY); cnt++) 
    {    
        //SysWaitOnEvent(0,0,0);       // let kernel spin once
        if (TIMEOUT_COUNT <= cnt) return(-1);
    } 

    i2cdata.B.BYTE0 = 0xC0;     
    i2cdata.B.BYTE1 = I2C_Sub_Addr;  
    i2cdata.B.BYTE2 = MyData.U.W0;  // First byte to write (in LSB of word) 
    HW_I2CDAT.U = i2cdata.U; 
       
    HW_I2CCSR.B.TREQ = 1;       // Master transaction request - generate start condition    
    HW_I2CCSR.B.TREQ = 0;       // and transmit Slave Device Write addres
    
    for(cnt = 0; (1 != HW_I2CCSR.B.TUFL); cnt++)                                  
    {    
        //SysWaitOnEvent(0,0,0);       // let kernel spin once
        if (TIMEOUT_COUNT <= cnt) return(-1);
    }
    
    if(HW_I2CCSR.B.ACKF) return(-2); 
    
    HW_I2CCSR.B.WL = 1;   // Word Length 1, 2, or 3 bytes
    HW_I2CCSR.B.LWORD = 1;          // Send the stop command 

    HW_I2CDAT.U = MyData.U.W1;         // Send last 3 bytes
      
    HW_I2CCSR.B.TUFLCL = 1;         // Clear the Receiver Overflow bit   
    HW_I2CCSR.B.TUFLCL = 0;      
     
    if(HW_I2CCSR.B.ACKF)  return(-2); 
 
    return(0);
}

#endif
