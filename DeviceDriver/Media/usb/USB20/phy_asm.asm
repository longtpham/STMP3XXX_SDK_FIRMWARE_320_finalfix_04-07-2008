; Filename: phy_asm.asm  copyright sigmatel, inc. 2005. 

      include "regsdcdc.inc"
      include "regsusb20.inc"
      include "regsusb20phy.inc"
      include "regsclkctrl.inc"
      include "regsgpio.inc"
    
    global FPHY_Startup

	extern 	   Fg_bLimitedVDDD

; Note these 3 bits are part of a 3 bit register bitfield called fab id at [15:13]. 
; When reading that field, all 3 bits must be compared to constants. 
SBX_BITPOS                                                      equ 15
HW_FABID_POINT16MICRON_PROCESS_BITPOS   equ 14
CBX_BITPOS                                                      equ 13
;----
    
    org p,".ptextphy_asm":  

FPHY_Startup:
    bset    #14,x:HW_GP0ENR
    bset    #14,x:HW_GP0DOER
    bset    #14,x:HW_GP0DOR
    bclr    #14,x:HW_GP0DOR
    move    #>$000810,x0
    move    x0,x:HW_DCDC_VDDIO
    move    #>$800000,x0
    move    x0,x:HW_USBPHYPWD           ; Force all PLL regs to default state

    jsr     FPHY_Resume

    ; These settings adjust the 45 and 1500 ohm resistors to their nominal values
    ; based on TA3 silicon
;    move    #>$f0f0f0,x0
;    move    x:HW_USBPHYTX,a1
;    and     x0,a1   #>$09090b,x0
;    or      x0,a1
;    move    a1,x:HW_USBPHYTX
    
    rts




FPHY_Resume:
      ;undo some items turned off in FPHY_Shutdown
      bclr  #HW_USBRX_FSCKSOURCESEL_BITPOS,x:HW_USBPHYRX
      ; Turns OFF the +5V current limit. This limits the inrush current.
      ; When using power supply source from USB, the inrush current most
      ; be limited to a value (I think 100mA, see USB specs).
      ; By now, if plugged to the USB, the inrush current must be over.
      bset    #HW_VDD5V_PWR_CHARGE_DISABLE_ILIMIT_BITPOS,x:HW_VDD5V_PWR_CHARGE

      ; WARNING !!!!
      ; This bit actually shuts down also the DC-DC Converter, therefore use
      ; only if you are sure that the part is operating from the linear
      ; regulators by sensing presence of 5 Volts.

      move    x:HW_USBCSR,x0
      jclr    #HW_USBCSR_VBUSSENSE_BITPOS,x0,_DoNotSetBrownout

	  ;If using the 5V to DCDC handoff, disable PWDN_ON_IOBRNOUT shutdown
  if (@def('DCDC_POWER_TRANSFER'))
      ;allow Init5VSense and ServiceDCDC to control PWDN_ON_IOBRNOUT bit
  else
      bset    #HW_VDD5V_PWR_CHARGE_PWDN_ON_IOBRNOUT_BITPOS,x:HW_VDD5V_PWR_CHARGE
  endif

_DoNotSetBrownout
      ; USB Pll is programmed to its default value of 60Mhz        
      move    #>$800000,x0
      move    x0,x:HW_USBPHYPWD           ; Power ON the PHY

      move    #>$000000,x0
      move    x0,x:HW_USBPHYPWD           ; Power ON the PHY

      ; From Steve Vu information
      bset    #HW_CCR_CKRST_BITPOS,x:HW_CCR      ; Enables writes to Clock Control Register (take out of reset)
      bset    #HW_CCR_PLLEN_BITPOS,x:HW_CCR      ; Enable (old 3410) PLL because the MUX 
                                          ; switching from old Pll to new (USB) PLL
                                          ; requires a clock during this phase (prevents glitches)

      ; instead of depending on a previous write of 0 to HW_CCR[DDIV], write it now since it divides either DCLK src.
      move    x:HW_CCR,a
      move    #HW_CCR_DDIV_CLRMASK,x0
      and     x0,a1                       ; clears DDIV field. Post DIV used in case of either DCLK src. divider is 2^0 aka 1.
      move    a1,x:HW_CCR                 ; "

      ;---- start conditional fDCLK reduction segment
      ;jmp     _stayFast         ; test tool only. Uncomment to stay at fDCLK=60|68Mhz for ALL devices.
      ; If Vddd limited device (3502 3501), CHANGE DCLK frequency TO 48MHz by setting the USB PHY PLL divider to 10.
      btst    #0,y:Fg_bLimitedVDDD
      jcc     _stayFast
      ; only reduce fDCLK if 3501 or 3502 for USB file transfer speed stability.

      move    x:HW_USBPHYRX,x0 ; PLLCKDIVCTL = 1001 in bits [19:16] gives a PLL divider of 10 (480/10=48MHz). flash timings change too.
      move    #HW_USBRX_PLLCKDIVCTL_CLRMASK,a       ; f0ffff
      and     x0,a    ; post: a1 has cleared field with other bits unchanged.
      move   #>HW_USBPHYRX_PLLDIV_BY_10,x0   ;Set divider field to 9 (for actual divider of 10) 480Mhz PLL/10=48MHz.
      or      x0,A1          ;  
      move    a1,x:HW_USBPHYRX  ; set new clock divider.
      jmp     _staySlow	;bypass the setting of DCLK to 68 Mhz
_stayFast
      ;---- end conditional fDCLK reduction segment

      ; Set the fDCLK to 68.57 Mhz (480/7) (|60Mhz if use _BY_8 mask defined in regsusb20phy.inc) in order the increase efficiency on USB transfers	
      move    x:HW_USBPHYRX,x0 
      move    #HW_USBRX_PLLCKDIVCTL_CLRMASK,a       ; f0ffff
      and     x0,a    ; post: a1 has cleared field with other bits unchanged.
      move   #>HW_USBPHYRX_PLLDIV_BY_7,x0     ; 480 MHz / this named number = fDCLK for usb mode applications (non 3502 SOC).
      or     x0,A1
      move    a1,x:HW_USBPHYRX  ; set new clock divider.
_staySlow
      bclr    #HW_USBRX_PLLLOCKED_BITPOS,x:HW_USBPHYRX ;Start PLL lock Sequence    

      clr     a
      move    #>1600,a0

_wait_loop
      ; We will wait a maximum of 200Us
      move    x:HW_USBPHYRX,x0
      jset    #HW_USBRX_PLLLOCKED_BITPOS,x0,_locked   ; Wait for PLL to lock    
      dec     a
      jne     _wait_loop

_locked

      bset    #HW_CCR_PLL_SOURCE_SEL_BITPOS,x:HW_CCR                 ; Select the new PLL (USB)
      bset    #HW_CCR_CKSRC_BITPOS,x:HW_CCR                      ; Route clock from Crystal to PLL

      bclr    #HW_CCR_PLLEN_BITPOS,x:HW_CCR                      ; Disable (old 3410) PLL now that we have
                                                          ; the new one locked

      bclr    #HW_USBCSR_CLKOFF_BITPOS,x:HW_USBCSR        ; Turns ON clock for USB Block
                                                          ; ARC + STMP USB INterface
      
      bclr   #HW_USBRX_FSCKSOURCESEL_BITPOS,x:HW_USBPHYRX      ;make sure the rx stuff is running on the 2.0 phy clock now

      bclr #14,x:HW_GP0DOR

      ; ---- 
      ; this is the 0.16 micron process bit that identifies 35xx Bx.
      btst #HW_FABID_POINT16MICRON_PROCESS_BITPOS,x:$FA82            ; alt to above 2 lines
      jcc    _notPoint16   
          ; Without this adjustment, the J/K levels could cause a USB high-speed compliance failure.  
          ; This issue has no impact on slower USB full-speed devices. 
          move    #>$fff0d0,x0      ; set mask to clear the TXcals
          move    x:HW_USBPHYTX,a1
          and     x0,a1   #>$07070b,x0      ; TXCALs are set to 7 instead of 8
         jmp     _PhyResumeAlmostDone
_notPoint16
          ; original values for 0.18 micron process. 
          ; These settings adjust the 45 and 1500 ohm resistors to their nominal values
          ; based on TA3 silicon
          move    #>$f0f0f0,x0
          move    x:HW_USBPHYTX,a1
          and     x0,a1   #>$08080b,x0
_PhyResumeAlmostDone
          or      x0,a1
          move    a1,x:HW_USBPHYTX
      ; ---
      ; this is the 0.16 micron process bit that identifies 35xx Bx.
      btst   #HW_FABID_POINT16MICRON_PROCESS_BITPOS,x:$FA82         ; alt to above 2 lines
      jcc    _notPoint16_L2   
          ; The 35xx Bx's increased jitter could cause USB high-speed compliance failures.  This issue has no impact on USB full-speed devices.       
          ; Next several inserted lines from apps engr MS are the fix. Modify the PHY PLL - PLLCPNSEL=4, PLLCPDBLIP=0, PLLV3ISEL = 1      
          ; This passed usb compliance testing and the eye diagram test on 35xx TBx.
          move    #>$f0f0f0,x0        ; Put register mask in x0      
          move    x:HW_USBPHYPLL,a1   ; Load a1 with the value in the USBPHYPLL register
          and     x0,a1               ; Apply the mask in x0 to a1
          move a1,y:(r7)+             ; Push a1 onto the stack.

          move    x:$FA82,a           ; contains fab id as bits [15:13]. saves 24 bit value in 24 bit register a1.
          jset    #SBX_BITPOS,a1,_nonCBXregValue   ; If this bit is set it can't be a CBx die
          jclr    #CBX_BITPOS,a1,_nonCBXregValue   ; If this bit is clear it can't be a CBx die

            move    #>$201,x0           ; This is a CBx part so put $201 into x0
            jmp     _WritePLLReg        ; Skip over the non CBX code.
_nonCBXregValue
          move    #>$401,x0           ; This is not a CBx part so put $401 into x0
_WritePLLReg
          move y:-(r7),a1             ; Get a1 from the stack
          or      x0,a1               ; Or in our settings
          move    a1,x:HW_USBPHYPLL   ; Save the regsiter back to HW_USBPHYPLL


_notPoint16_L2

      clr a  

      rts
      

