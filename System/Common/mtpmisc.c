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
#include "kernel.h"
#include "usb_api.h"
#include "misc.h"
#include "regsgpio.h"
#include "regsrtc.h"
#include "gettime.h"
#include "regscodec.h"
#include "regslradc.h"
#include "regsclkctrl.h"
#include "regstimer.h"

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

int g_iHighPwrPeriphInitMode = STRUCTURE_INIT_ONLY; 

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
    //TODO put this back.
//    IcollShutDown();
    
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
static RETCODE _reentrant SysGpioDisableAll(void)
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
static RETCODE _reentrant SysPadPowerDownAll(void)
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
static RETCODE _reentrant SysPllShutdown(void)
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
///PJJRETCODE GoToSleep(void)
///PJJ{
///PJJ    #pragma asm
///PJJ        wait
///PJJ    #pragma endasm
///PJJ    
///PJJ    return SUCCESS;
///PJJ}

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
///PJJDWORD GetDclkCount(void)
///PJJ{
///PJJ    return((((DWORD)(HW_DCLKCNTU.I))<<24) | ((DWORD)(HW_DCLKCNTL.I)));
///PJJ}

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
///PJJvoid SetDclkCount(DWORD dwCount)
///PJJ{
///PJJ    // TODO:  protect this from interrupts
///PJJ    HW_DCLKCNTU.I = (WORD)(dwCount >> 24);
///PJJ    HW_DCLKCNTL.I = (WORD)(dwCount);
///PJJ}

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
///RETCODE SmartMediaSetPortTiming(WORD wTiming)
///{
///    // TODO:  validate the input
///    
///    // Set the timing
///    HW_FLSMTMR1R.I = wTiming;
///    
///    return SUCCESS;
///}
