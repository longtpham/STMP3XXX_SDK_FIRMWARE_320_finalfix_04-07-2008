;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2006
;
;  File        : syshwinit.asm PLAYER (STMP35xx Device Assumed)
;  Description : Initializes hardware. Routines: PowerInit & AnalogInit
;///////////////////////////////////////////////////////////////////////////////

    section     SYSHWINT_main
;///////////////////////////////////////////////////////////////////////////////
;  Macros
    nolist
    include "sysmacro.asm"
    list

;///////////////////////////////////////////////////////////////////////////////
;   Includes
    include "hwequ.inc"
    include "resource.inc"
    include "NANDHal.inc"
    include "regssystem.inc"

;Next line feature added but disabled by default
;HW_FABID_POINT16MICRON_PROCESS_BITPOS equ 14

;///////////////////////////////////////////////////////////////////////////////
;   Shared Definitions
    global PowerInit
    global AnalogInit

;///////////////////////////////////////////////////////////////////////////////
;   External References
    extern MixerMasterVol
    extern FPowerUpHeadPhones
    extern b1p8Volts
    extern Fg_bLimitedVDDD
    extern Fg_wSTMPDeviceId

 if (@DEF('FUNCLET'))           
    extern    SysCallFunclet
 endif
    
    include "const.xref"
;///////////////////////////////////////////////////////////////////////////////
;   Equates
; Number of loop iterations needed to wait for DC-DC to settle
DCDC_SETTLING_COUNT  equ     10000

;///////////////////////////////////////////////////////////////////////////////
;   P Memory
    org    p,"SYSHWINIT_main_P":

;///////////////////////////////////////////////////////////////////////////////
;>  Name:           PowerInit
;   Type:           Function
;   Description:    Initializes the DCDC converter output
;   Inputs:         none
;   Outputs:        none
;   Notes:          Battery Brownout is enabled in SysLRADCBrownOutInit()
;                      called from mainmenu.c
;<
;///////////////////////////////////////////////////////////////////////////////
PowerInit
   ;******** NOTE MIXTBR:BIT 9 DISABLES ALL POWERDOWN - MUST CLEAR BEFORE SHUTTING DOWN **********
   move    #>$010240,x0         ; bit 16 powers down right ADC , mic must use left ADC data  - clock glitch //stmp10060 clear HW_MIXTBR.XBGC bit was #>$012240
   move    x0,x:HW_MIXTBR       ; bit 13 switches crystal to band gap bias current 
                                ; bit 9 disables fast falling edge reset on pswitch pin    **** NOT HERE because DEBUG disabled ** add to RETAIL only build
                                ; bit 6 default to decrease pll charge pump current

   ; Note: setting the PLL_ZERO_VT bit 1 above caused initial PLL speedup (short clocks) 
   ; & associated illegal instruction irq so don't set it in the line above. 
   ; Set was intended to allow higher PLL freq for cold/SlowSilicon parts at low VDD. 
   ; Future can try to set that bit #1 when running from crystal, then resetting it here would probably be harmless. 

   ; need to clear Auto-Restart in case it has been set in USBMSC
   ; uncomment the next two lines if using AUTO_RESTART in USBMSC
   ; care must be taken if using RTC
   ;bclr   #HW_DCDC_PERSIST_AUTO_RESTART_BITPOS,x:HW_DCDC_PERSIST 
   ;bset   #HW_DCDC_PERSIST_UPDATE_BITPOS,x:HW_DCDC_PERSIST       ;0-1 Transistion Updates master register bits

   ;;;;;;;;;;;;;;;;;;;;;;;;;;;; DCDC Settings for STMP35xx ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ; Values may need to be updated after Matrix Lot
   ;   

   move    #>$000051,x0                     ;Need to turn on Battery with offset before using DCDC1_BAT_ADJ
   move    x0,x:HW_BATT_CTRL                ;or DCDC1_ADJ_TN in HW_DCDC_TBR 

   ; Check the DCDC Mode and set the DCDC TBR Reg
   btst    #>HW_REVR_DCDCMODE_BUCK_MASK_POS,x:HW_REVR
                                      ; What mode is dcdc1 in?
   jcs     _boostmode

   ; If in Buck Mode,
   move    #>$008010,x0               ; Buck Mode
   move    x0,x:HW_DCDC_TBR        ; Run DCDC at higher frequency to reduce the instantaneous peak inductor current

   ;Change DCDC_CTRL0 Registers to the correct value for Buck
   move    #>$120817,x0                     
   move    x0,x:HW_DCDC1_CTRL0
   move    #>$1F0817,x0
   move    x0,x:HW_DCDC2_CTRL0
   ;Change DCDC_CTRL1 Registers to the correct value for Buck
   move    #>$000034,x0               
   move    x0,x:HW_DCDC1_CTRL1
   move    x0,x:HW_DCDC2_CTRL1

   jmp     _dcdc_complete

_boostmode
   move    #>$008000,x0               ; Boost Mode (Default)
   move    x0,x:HW_DCDC_TBR           ; bit 15 high  => use band gap bias current
                                      ; Allow SysSpeed to set TBR based on core voltage setting

   ;Change DCDC_CTRL0 Registers to the correct value for Boost
   move    #>$010817,x0                     
   move    x0,x:HW_DCDC1_CTRL0
   move    x0,x:HW_DCDC2_CTRL0
   ;Change DCDC_CTRL1 Registers to the correct value for Boost
   move    #>$000054,x0               
   move    x0,x:HW_DCDC1_CTRL1
   move    x0,x:HW_DCDC2_CTRL1

_dcdc_complete
   ;---------------------------------------------------------------------------------------------
;Next 10 lines, remove semicolon from column 1 to enable feature: feature added but commented out by default per TZ, MW, MF, JLN mtg March 2006.
;   ; if 0.16um process bit set, 
;   ; (this is the 0.16 micron process bit that identifies 35xx Bz versus 35xx Az.)
;   btst #HW_FABID_POINT16MICRON_PROCESS_BITPOS,x:$FA82            
;   jcc    _notPoint16   ; jumps if not set
;
;   move    #>$04F358,x0 ; .16 bit is set so Use low nibble as 8 to make 0.16um chip HP amp output level behave like 0.18um chip, 
;                        ; 0x8 low nibble is 0.75dB volume reduction versus 0xB.
;   jmp     _point16     
;                        ; else use low nibble as B as in previous SDKs. 
;_notPoint16
   ; REF CTRL Updated 10.17.03 after audio testing. Settings based on 1.37V VDDD. 
   move    #>$04F35B,x0            ; bit 15 powers down selfbias circuit. Before Jun18 04, value was $04F35D. $04f35B value before Mar 15 '06. 
_point16

   move    x0,x:HW_REF_CTRL        ; Set LWREF to lower voltage by 20%
   ; ADCREF=1.22V, VAG=0.68, DACVBGVAL_is_low_nibble=0x9==1001 gives 0.92V (1.15V dsheet-20%=0.92V as tested value for SB6 0dB sine THD pass at 8 steps from max volume)
   ; (DACVBGVAL was 0xB=1.20V -20%=0.96V in SDK3.110 & 2.610) (much earlier it was 1.0V) (20% less than datasheet values since LWREF bit 18 is set)
   ; Low nibble change from B to 8 (3 steps less= 0.75dB volume reduction) makes R&S audio test THD pass at 8 volume steps from peak & gives a higher Vpeak amplitude passing THD spec at 
   ; 8 button presses from max volume on SB6. Low nibble=9 (2 decrement from B) was all that was needed to make TB6 pass THD test.
   ; Test used 0dB sine wma and mp3. Headphone amp designer MF says we can just spec the THD at -3dB from full scale like most products do (slight spec change in dsheet v1.11) & leave SW at B, or 
   ; make the -0.75dB volume reduction change described above (8 value) but that causes all volume settings to be shifted down by 0.75dB which some customers may not want so
   ; MF recommendation (chosen) is the slight spec change & no SW change and the test procedure would set volume to 9 steps from max instead of 8 when doing the THD test for any chip.
   ; JLN recommendation for audiophiles seeking 1.25dB extra dynamic range (higher undistored vpeak at 8 steps from max volume)is to use 8 for low nibble iff 0.16um process, else use B low nibble as before for 0.18um process ICs. Keeps outputs most similar.
   ;---------------------------------------------------------------------------------------------
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;   Set Voltage ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;Before changing voltages, turn-off Brown-out to prevent false shutdowns
   bclr    #HW_DCDC_VDDIO_BROWNOUT_ENABLE_BITPOS,x:HW_DCDC_VDDIO                ;Disable VDDIO Brownout
   bclr    #HW_DCDC_VDDD_BROWNOUT_ENABLE_BITPOS,x:HW_DCDC_VDDD              ;Disable VDDD Brownout

   move    #>$00080F,x0          ;PLAYER VDDIO
   move    x0,x:HW_DCDC_VDDIO    ;set IO rail to 3.01V, brownout - 2.56V 

    ; Delay while the Voltage settles
    clr     a
    move    #>DCDC_SETTLING_COUNT,a0
_SettlingDelay  ; Each loop is 3 clocks
    dec     a
    jne     _SettlingDelay

;Allow SysSpeed to Change VDDD Voltage.  Leave VDDD at reset value. 
; TODO: INSPECT FOR WHERE g_bLimited VDDD parts get their VDDD set. 

_TurnOnBO
   ;Turn On BO when Ready
   ; Enable Brownout for VDDIO & maybe for VDDD 
   bset    #HW_DCDC_TBR_PWRUP_VDDIO_BRNOUT_BITPOS,x:HW_DCDC_TBR 
   bset    #HW_DCDC_VDDIO_BROWNOUT_ENABLE_BITPOS,x:HW_DCDC_VDDIO
     ;If 3502 device disable VDDD BO (required), else enable it.  
     move       y:Fg_wSTMPDeviceId,a
     move       #>$3502,y1
     cmp        y1,a
     jeq        _TurnOffBO
   bset     #HW_DCDC_VDDD_BROWNOUT_ENABLE_BITPOS,x:HW_DCDC_VDDD
   jmp      _EndSetBO    
_TurnOffBO
     bclr    #HW_DCDC_VDDD_BROWNOUT_ENABLE_BITPOS,x:HW_DCDC_VDDD    ;Disable VDDD Brownout
_EndSetBO


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
       
;;;;;;;;;;;;;;;;;;;;;;;;;;;; GPIO Setup ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;Check for 1.8V NAND mode
HW_BOOT_MODE_BIT_3_GPIO equ HW_GPIO_003
HW_BOOT_MODE_BIT_2_GPIO equ HW_GPIO_002
HW_BOOT_MODE_BIT_1_GPIO equ HW_GPIO_001

    ; Before powering up the GPIO, determine from mode bit 3 if this is a 1.8V
    ; NAND (0) or 3.3V NAND (1)
    ;If GP3 = 0, GP2 = 0, and GP1 = 1, then 1.8V NAND boot mode 
    bset    #HW_BOOT_MODE_BIT_3_GPIO,x:HW_GP0PWR    ; Power up the pin
    bset    #HW_BOOT_MODE_BIT_2_GPIO,x:HW_GP0PWR    ; Power up the pin
    bset    #HW_BOOT_MODE_BIT_1_GPIO,x:HW_GP0PWR    ; Power up the pin

    bclr    #HW_BOOT_MODE_BIT_3_GPIO,x:HW_GP0DOER   ; Configure as an input
    bclr    #HW_BOOT_MODE_BIT_2_GPIO,x:HW_GP0DOER   ; Configure as an input
    bclr    #HW_BOOT_MODE_BIT_1_GPIO,x:HW_GP0DOER   ; Configure as an input

    bset    #HW_BOOT_MODE_BIT_3_GPIO,x:HW_GP0ENR    ; Enable the pin as a GPIO
    bset    #HW_BOOT_MODE_BIT_2_GPIO,x:HW_GP0ENR    ; Enable the pin as a GPIO
    bset    #HW_BOOT_MODE_BIT_1_GPIO,x:HW_GP0ENR    ; Enable the pin as a GPIO

    btst    #HW_BOOT_MODE_BIT_3_GPIO,x:HW_GP0DIR    ; Get the state of the pin
    jcs     NANDIs3_3V                              ; If 1, then NOT 1.8V NAND
    btst    #HW_BOOT_MODE_BIT_2_GPIO,x:HW_GP0DIR    ; Get the state of the pin
    jcs     NANDIs3_3V                              ; If 1, then NOT 1.8V NAND
    btst    #HW_BOOT_MODE_BIT_1_GPIO,x:HW_GP0DIR    ; Get the state of the pin
    jcc     NANDIs3_3V                              ; If 0, then NOT 1.8V NAND

NANDIs1_8V

CF_8_BIT_1p8VOLTS       equ     $3f    
    ; Setup proper voltage on Flash before turning the drivers on.
    ; Enables all GPFLASH lines EXCEPT the extra 8 data lines as 1.8V
    ; Extra 8 data lines for 16-bit flash are setup in PowerUpFlashPads
    move    #>CF_8_BIT_1p8VOLTS,a
    move    a,x:HW_SYSTEM_1_8V_PAD_EN

    ;Set the 1.8V flag (clear by default)
    move    #>1,a
    move    a,NANDHALmem:b1p8Volts
    jmp     NANDCheckExit
NANDIs3_3V
    move    #>0,a
    move    a,NANDHALmem:b1p8Volts

NANDCheckExit

    move    y:<Const_ffffff,x0      ;Power Up GPIO pads
    move    x0,x:HW_GP0PWR
    move    x0,x:HW_GP1PWR
  if @def('MMC')
    ;; don't turn on GP52 power because default is CF_IORDn. 
    ;; This pin is also MMC protect and it may draw excessive current
    move    #$ffffef,a
    move    a,x:HW_GP2PWR
  else
    move    x0,x:HW_GP2PWR
  endif
    rts

 if (@DEF('FUNCLET'))
    ;If using funclet, switch counter back to original here for the next function
    org    p,"SYSHWINIT_main_P":
    
 endif

;///////////////////////////////////////////////////////////////////////////////
;>  Name:           AnalogInit
;   Type:           Function
;   Description:    Initializes the analog sub system
;   Inputs:         none
;   Outputs:        none
;   Notes:          none
;<
;///////////////////////////////////////////////////////////////////////////////
AnalogInit
 if (@DEF('FUNCLET'))           
    ;If using funclet, function will be executed via funclet kernel
    CallFunclet RSRC_FUNCLET_ANALOGINIT

    org p,"SYSFUNCLET_AnalogInit_P":
    
    dc      RSRC_FUNCLET_ANALOGINIT     ; Funclet Resource number
 endif

    ; power down unused peripherals
    bset    #HW_ADCCSR_CLKGT_BITPOS,x:HW_ADCCSR                         ; Power down ADC
 if (@DEF('FM_TUNER_USE_PWM0')) 
   bset    #HW_TMR3CR_CG_BITPOS,x:HW_TMR3CR                             ; Power down TIMER3
 endif
    bset    #HW_SDRAM_CSR_PWDN_BITPOS,x:HW_SDRAM_CSR                    ; Power down SDRAM
    bset    #HW_SWIZZLECSR1_CLK_OFF_BITPOS,x:HW_SWIZZLECS1R             ; Power down SWIZZLE
    bclr    #HW_RAM_ROM_CFG_ROM_CLK_EN_BITPOS,x:HW_RAM_ROM_CFG          ; Power down PROM

    move    #>$000101,x0          ; Reduce current draw by changing from default ($000000)
    move    x0,x:HW_MIXRECSELR    ; GLUE Analog RecSelect register


;///// stop here if USBMSC is being built

 if (@DEF('PLAYER'))   ;add player specific init's  

    ;ensure USB is powered down
    bclr    #HW_CCR_PLL_SOURCE_SEL_BITPOS,x:HW_CCR      ; clear bit to select system PLL
    move    #>$800000,x0
    move    x0,x:HW_USBPHY_BASEADDR           ; Force all PLL regs to default state

    bset    #HW_USBCSR_CLKOFF_BITPOS,x:HW_USBCSR                        ; Power down USB for all devices

  ; No power down available: GLUE, MIRAGE_GLUE, RTC, I2C, SPI, SAI, ICOLL
  ;3500 clock settings
  ;Use xtal/4 - 6MHz
  bclr    #HW_CCR_ADIV1_B0_BITPOS,x:HW_CCR
  bclr    #HW_CCR_ADIV1_B1_BITPOS,x:HW_CCR
  bclr    #HW_CCR_ADIV1_B2_BITPOS,x:HW_CCR
  bclr    #HW_CCR_ADIV_B0_BITPOS,x:HW_CCR
  bclr    #HW_CCR_ADIV_B1_BITPOS,x:HW_CCR
  bclr    #HW_CCR_ADIV_B2_BITPOS,x:HW_CCR

  ;turn-on RTC
  bclr    #HW_RTC_CSR_SFTRST_BITPOS,x:HW_RTC_CSR

    move    y:<Const_000000,x0
    move    x0,x:HW_MIXMASTERVR         ; muted, init default saved level 
    bset    #HW_MIXMASTERVR_MUTE_BITPOS,x:MixerMasterVol  ;Always make sure that the volume starts muted


    move    #>$008048,x0                ; Mic mute=1, +20db=1, vol=01000
    move    x0,x:HW_MIXMICINVR          ; GLUE Analog MicIn volume register

    move    #>$008808,x0                ; LineIn mute=1, lvol=01000, rvol=01000
    move    x0,x:HW_MIXLINE1INVR        ; GLUE Analog LineIn volume register

    move    #>$008808,x0                ; FMIn mute=1, lvol=01000, rvol=01000
    move    x0,x:HW_MIXLINE2INVR        ; GLUE Analog FMIn volume register

    ;bclr #HW_GP_B16_BITPOS,x:HW_GP0DOR
    jsr     FPowerUpHeadPhones
    ;bclr #HW_GP_B16_BITPOS,x:HW_GP0DOR

 endif 


    rts


 if (@DEF('FUNCLET'))
    ;If using funclet, switch counter back to original here for the next function
    org    p,"SYSHWINIT_main_P":
    
 endif
     endsec





