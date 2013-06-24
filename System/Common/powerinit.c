///////////////////////////////////////////////////////////////////////////////
//!
//! \file powerinit.c
//! \brief This file handles initializing the power state for the device.
//!
//!
//! Copyright (c) SigmaTel, Inc. Unpublished
//!
//! SigmaTel, Inc.
//! Proprietary  Confidential
//!
//! This source code and the algorithms implemented therein constitute
//! confidential information and may comprise trade secrets of SigmaTel, Inc.
//! or its associates, and any use thereof is subject to the terms and
//! conditions of the Confidential Disclosure Agreement pursuant to which this
//! source code was originally received.
//!
///////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "regsclkctrl.h"
#include "regsdcdc.h"

////////////////////////////////////////////////////////////////////////////////
// Externs
////////////////////////////////////////////////////////////////////////////////
extern BOOL g_bLimitedVDDD;

///////////////////////////////////////////////////////////////////////////////
//!
//!   PowerInit
//!
//!   This function handles the startup power settings for the device.
//!
//!   \param void
//!
//!   \retval SUCCESS - Always successful.
//!
//!   Notes:         this is the mtp ver. usbmscmisc.c has a ver & player's
//!                  ver in syshwinit.asm is also used.
//!
///////////////////////////////////////////////////////////////////////////////
RETCODE PowerInit(void)
{
#if defined(DEVICE_3410) || defined(DEVICE_3310)
    // bit 16 powers down right ADC , mic must use left ADC data 
    // bit 13 switches crystal to band gap bias current 
    // bit 9 disables fast falling edge reset on pswitch pin    **** NOT HERE because DEBUG disabled ** add to RETAIL only build
    HW_MIXTBR.I = (INT)0x010040;		   // stmp10060 clock glitch {was (INT)0x012040; in SDK3.101} 
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
    // MTP is to have same vddd for all 35xx family members but vddd limited ones. Keeps usb current in usb spec.
    if(g_bLimitedVDDD) // eg 3502/01 device
    {   //HW_DCDCTBR.I = (INT)0x008000;				// Not nedded
        //HW_DCDC_VDDD.B.VOLTAGE_LEVEL = 0x1A;		//0x1A is 1.85V; history: 0x1F = 2.02V
        // Vddd level is already set in chip capability for vddd limited cases. 
        // Vddd BO is disabled in usb mode.
    }
    else // all other 35xx family members
    {
        HW_DCDC_VDDD.B.BROWNOUT_LEVEL = 0x0b;       // Brownout set to 1.37V. VDDD BO is disabled in MSC so this is moot.
        
        // VDDD was previously set to 1.50V. fDCLK=60MHz assumed. 
        // 1.54V is 0x0f. Max unstable voltage per results from 35xx matrix lot in a few temperature edge cases. 
        // 1.63V is 0x13. VDDD safest is 1.63V. Use 1.60V for less USB current.
        HW_DCDC_VDDD.B.VOLTAGE_LEVEL = 0x12;        // VDDD set to 1.60V (0x12).  Usb current checked by JLN. 
        //STMP00006310 soln: new logic prevents this from overwriting vddd level in limited vddd case.
    }   
    HW_DCDC_VDDIO.I = 0x0810;                       // VDDIO set to 3.07V

    HW_CCR.B.XTLEN = 0;                             // Turn OFF Analog clocks
    HW_CCR.B.ACKEN = 0;                             // To reduce current
//    PowerDownFMTuners(); // Turn off FM Tuner to reduce current. Probably useful for mtp also but check codesize growth.

// This is not needed with the DCDC-usb5v Hand-off code.  Here in case a customer 
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

///////////////////////////////////////////////////////////////////////////////
//!
//!   PllInit
//!
//!   This function handles the PLL initialization.
//!
//!   \param void
//!
//!   \retval SUCCESS - Always successful.
//!
//!   Notes: Only used in the 3410 build
//!
///////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////
//!
//! ProgramPll
//!
//! Set the Clock Control Register for Phase Locked Loop mode, delay >1ms, 
//! switch DCLK to PLL.
//! 
//! \param iPdiv    Value for HW_CCR PDIV field
//!
//! \retval         SUCCESS
//!
//! Notes:          Only used in the 3410 build.
//!                 
///////////////////////////////////////////////////////////////////////////////
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
