////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002
//
// Filename: main.c
// Description: Main file for USB MSC firmware
////////////////////////////////////////////////////////////////////////////////

#if defined(DEVICE_3500)
#include "regsgpflash.h"
#endif

BOOL g_bLimitedVDDD = FALSE;
////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////


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
    #pragma asm

    if (@def('DEVICE_3500'))
        include "regsusb20phy.inc"
        include "regsclkctrl.inc"
        include "regsdcdc.inc"
        include "regsusb20.inc"
    endif

        extern  FTimeInit
        jsr     FTimeInit
    
    if (@def('DEVICE_3500'))

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
    endif

    #pragma endasm


}    
