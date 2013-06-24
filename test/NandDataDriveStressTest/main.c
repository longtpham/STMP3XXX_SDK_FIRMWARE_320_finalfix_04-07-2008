///////////////////////////////////////////////////////////////////////////////
//!
//! \file main.c
//! \brief main startup for the Nand Stress Test.
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
#include "persist_bits.h"
//#include "flashtiming.h"
#include "messages.h"
#include "displaydrv.h"
#include "kernel.h"
#include "ddildl.h"
#include "ddildl_internal.h"
#include "sysresourceapi.h"
#include "regsgpio.h"

_asmfunc void SysTimeReset(void);
_reentrant void EnableButtons(void);



////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////


#include "sysirq.h"
#include "ipr.h"

BOOL g_bLimitedVDDD = FALSE;
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          Main
//
//   Type:          Function
//
//   Description:   Main entry point
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void Project_Startup(void)
{

    int i;
    
    #pragma asm

        include "regsusb20phy.inc"
        include "regsclkctrl.inc"
        include "regsdcdc.inc"
        include "regsusb20.inc"

        extern  FTimeInit
        jsr     FTimeInit
    
      ; Speed Up the DCLK to 60 Mhz
      ; This will speed up USB connection
          bset    #HW_VDD5V_PWR_CHARGE_DISABLE_ILIMIT_BITPOS,x:HW_VDD5V_PWR_CHARGE
      
          ; USB Pll is programmed to its default value of 60Mhz        
      
          ; From Steve Vu information
          bset    #HW_CCR_CKRST_BITPOS,x:HW_CCR         ; Enables writes to Clock Control Register
          bset    #HW_CCR_PLLEN_BITPOS,x:HW_CCR         ; Enable (old 3410) PLL because the MUX 
                                                    ; switching from old Pll to new (USB) PLL
                                                    ; requires a clock during this phase (prevents glitches)
      
          move    #>$800000,x0
          move    x0,x:HW_USBPHY_BASEADDR               ; Force all PLL regs to default state
          move    #>$000000,x0
          move    x0,x:HW_USBPHY_BASEADDR               ; Power ON the PHY
  
        ; The 3500 datasheet says to CLEAR this bit to initiate a new PLL count
        ; Any writes to HW_USBPHYRX must have the PLLLOCKED bit set to avoid resetting the PLL lock sequence
        ; Correcting this error will definitely increase the loop time in _wait_loop below.
        bclr    #HW_USBRX_PLLLOCKED_BITPOS,x:HW_USBPHYRX ;Start PLL lock Sequence    
      
          clr     a
        move    #>1600,a0
     
_wait_loop
          ; We will wait a maximum of 200Us
          move    x:HW_USBPHYRX,x0
          jset    #HW_USBRX_PLLLOCKED_BITPOS,x0,_locked   ; Wait for PLL to lock    
          dec     a
          jne     _wait_loop
  
        ; ** NOTE ** There is currently no provision here for the case where the lock bit is not
        ; set within the above loop counter time. It just falls through to _locked
        ; Something should be done in the case of the bit not locking.
      
_locked
          bset    #HW_CCR_PLL_SOURCE_SEL_BITPOS,x:HW_CCR                 ; Select the new PLL (USB)
          bset    #HW_CCR_CKSRC_BITPOS,x:HW_CCR                      ; Route clock from Crystal to PLL
      
          bclr    #HW_CCR_PLLEN_BITPOS,x:HW_CCR                      ; Disable (old 3410) PLL now that we have
                                                              ; the new one locked
      
          bclr    #HW_USBCSR_CLKOFF_BITPOS,x:HW_USBCSR        ; Turns ON clock for USB Block
                                                              ; ARC + STMP USB INterface
    #pragma endasm



    SysPostMessage(3,LCD_SET_CONTRAST,30);
    EnableButtons();

    // Set the Resource Tag
    SysSetResourceTag(DRIVE_TAG_RESOURCE_BIN);

    // Init the media
    for(i = 0 ; i < g_wNumMedia ; i++)
    {
        if(MediaInit(i) != SUCCESS)
        {
            SystemHalt();
        }
    }        

    //turn interrupts on
    IPRSetIVLPriority(IVL_LINE_1,IVL_PRIORITY_1);

    // Unmask all interrupts
    SysUnMaskAllInterrupts(INTERRUPTS_UNMASK_ALL);

    // Discover the Media and its drives
    for(i = 0 ; i < g_wNumMedia ; i++)
    {    
        if(MediaDiscoverAllocation(i) != SUCCESS)
        {
            SystemHalt();
        }
    }        

    // Init All Drives
    for(i=0;i<g_wNumDrives;i++)
    {
        if(DriveInit(i) != SUCCESS)
        {
            SystemHalt();
        }
    }

    SysTimeReset();

}    


_reentrant void EnableButtons(void)
{
  // STOP Button:
  //   Col 0 = GP2, 0
  //   Row 0 = GP2, 1
  #pragma asm
  include "regsgpio.inc"
  ;Set up the ROW as an INPUT
  bset  #1,x:HW_GP2ENR           ; Enable it
  bset  #1,x:HW_GP2DIR           ; Set as input  
  bset  #1,x:HW_GP2PWR           ; Power up the pin

  bset  #19,x:HW_GP1ENR           ; Enable it
  bset  #19,x:HW_GP1DIR           ; Set as input  
  bset  #19,x:HW_GP1PWR           ; Power up the pin

  ; Set up the column as an OUTPUT
  bset  #0,x:HW_GP2DOR           ; Set as output
  bset  #0,x:HW_GP2ENR           ; Enable it
  bclr  #0,x:HW_GP2DOER          ; Disable it
  bset  #0,x:HW_GP2PWR           ; Power up the pin

  ; Set up the column as an OUTPUT
  bset  #23,x:HW_GP1DOR           ; Set as output
  bset  #23,x:HW_GP1ENR           ; Enable it
  bclr  #23,x:HW_GP1DOER          ; Disable it
  bset  #23,x:HW_GP1PWR           ; Power up the pin
                                     
 #pragma endasm
}
