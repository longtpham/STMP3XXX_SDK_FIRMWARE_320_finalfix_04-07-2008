;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2004
;
;  File        : bootmanager.asm
;  Description : BootManager main program and when built with EEPROM option
;                this serves as the EEPROM stored program that boots STMP
;                and includes USB boot (recovery mode).
;///////////////////////////////////////////////////////////////////////////////

    page    255,255,0

;*******************************************************************************
;  Include files
;*******************************************************************************

    nolist
    include "sysequ.inc"
    include "stack.inc"

    if (@def('DEVICE_3500'))
        include "regsusb20.inc"
        include "regsi2c.inc"
        include "regsgpio.inc"
        include "regsspare.inc"
        include "regscore.inc"
        include "regsgpflash.inc"
        include "regsusb20phy.inc"
        include "regsdcdc.inc"
        include "regsclkctrl.inc"
		include "regslradc.inc"
		include "regsicoll.inc"
		include "glueregs1.8V.inc"
    else
        include "hwequ.inc"
    endif

    list

;*******************************************************************************
;  Extern definitions
;*******************************************************************************

    global    BootManager
    global    ROM_SIZE
    global    LoadRoutinePtrs
    global    MainLoop
    global    Getkey
    global    DecryptWord
    global    UpdateCheckSum
    global    Booty
    if (@def('BM_DO_NOT_INCLUDE_VERSIONS'))
    else
        global    VERSION_MAJOR
        global    VERSION_MIDDLE
        global    VERSION_MINOR
    endif

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

    include "bootmanager.xref"
    if (@def('DEVICE_3500'))
        include "3500bootmanagermem.xref"
    else
        include "3410bootmanagermem.xref"
    endif

    extern      L1DataStack

    extern      b1p8Volts
    extern      SaveCommand ; debugging
  if (@def('USE_EEPROM'))
    extern      Fusb_dci_isr
  endif

;*******************************************************************************
;  Constants
;*******************************************************************************

; Component version number of the boot manager (booty booter).
; Note: bootmanager 3.110.011 is source code equal to the 3.101.103 release  NO LEADING ZEROS IN VERSION
    if (@def('BM_DO_NOT_INCLUDE_VERSIONS'))
    else
VERSION_MAJOR       equ     3
VERSION_MIDDLE      equ     200
VERSION_MINOR       equ     910
    endif

; Version of the data that the booty booter processes
BOOTY_VERSION       equ     $000200

    if (@def('DEVICE_3410'))

ROM_SIZE                equ     8*1024

    else
                                           ; 3500 build
ROM_SIZE                equ     12*1024

    endif

    if (@def('USE_EEPROM'))
DELAY_SETUP_PLAY_BUTTON equ     2457/3        ; $000333 = 100us @ 24.576MHz
TIMEOUT_RECOVERY_HIGH   equ     $000007     ; 5 seconds = 5*24,567,000 = 122,880,000 = 0x000007530000
TIMEOUT_RECOVERY_LOW    equ     $530000
TIMEOUT_RECOVERY        equ     $3E8000     ; 5s @ 24.576MHz, 30 cycle loop
                                        ;  5*24576000/30 = 4096000 = $3E8000
    endif


    if (@def('DEVICE_3500'))
HW_BOOT_MODE_BIT_3_GPIO equ     HW_GPIO_003     ; LCD.D3
    endif

; Number of loop iterations needed to wait for the PLL and DC-DC to settle
; Count = Delay * 24.576MHz / 3  (Delay is in seconds)
SETTLING_COUNT  equ     10000           ; 1.23 ms

;PLL Settings for Clock Speed Up
PLL_PDIV        equ     17              ; (17+33)*(24/20) = 60
PLL_DDIV        equ     0               ; 60 / (2^0) = 60 MHz

;Battery Thresholds for brownout and fast boot
;Event 1 - 1.2V for Fast Boot.  Event 2 - 0.9V for Battery BO
BATTERY_THRESHOLD       equ     $096069

;*******************************************************************************
;  X memory
;*******************************************************************************

;*******************************************************************************
;  Y memory
;*******************************************************************************

;*******************************************************************************
;  P memory
;*******************************************************************************

;*******************************************************************************
;   Boot loader
;
;*******************************************************************************
; The boot loader is able to load code into p, x, and/or y memory.  It is also
; capable of initializing a specified block of memory to a constant value.
; Each transfer is preceded by a header that defines the target memory, the
; base address, and the optional initialization constant if appropriate.
;
;
;   Word 0             Mc3 Mc2 Mc1 Mc0 x x x x a15 a14 a13 a12 a11 a10 a9 a8 a7 a6 a5 a4 a3 a2 a1 a0
;   Word 1             x   x   x   x   x x x x l15 l14 l13 l12 l11 l10 l9 l8 l7 l6 l5 l4 l3 l2 l1 l0
;   Word (optional)    D23-D0
;
;        Mc3 Mc2 Mc1 Mc0
;            (P) (X) (Y)
;         0   0   0   0         Unused
;         0   0   0   1         load into Y memory
;         0   0   1   0         load into X memory
;         0   0   1   1         load into L memory (XY memory)
;         0   1   0   0         load into P memory
;         0   1   0   1         Unused
;         0   1   1   0         Unused
;         0   1   1   1         Unused
;         1   0   0   0         Unused
;         1   0   0   1         init Y memory with data value
;         1   0   1   0         init X memory with data value
;         1   0   1   1         init L memory with data value
;         1   1   0   0         init P memory with data value
;         1   1   0   1         Unused
;         1   1   1   0         Unused
;         1   1   1   1         BootLoad complete, exit and execute loaded program

    org p,"EXEC_Vectors_P":

    jmp     Booty


  if (@def('USE_EEPROM'))
    org p,"USB_Vector_P":
    ; USB ARC
    jsr     Fusb_dci_isr
  endif

    org p,"VDDIO_BO_Vector_P":
    ; VDDIO Brownout
    jmp     VDDIOBrownout

    org p,"BOOTMANAGER_P":


;///////////////////////////////////////////////////////////////////////////////
; Booty code
;///////////////////////////////////////////////////////////////////////////////
; This entry point is used when the BootManager is called by another function.
;   Typically called by USB MSC in Win98 because that OS does not have native
;   MSC support. a1 = SmBlockTag
;   BootManager must be located at p:$80
BootManager
        move    a1,p:TempBlockTagSave
        clr     b                       ; reset the stack pointer
        move    b,sp
        jsr     BootyInit
        move    p:TempBlockTagSave,a1
        move    a1,x:SmBlockTag         ; save the block tag
        jmp     LoadRoutinePtrs
TempBlockTagSave
        dc      0

; This entry point is used when the BootManager is loaded from ROM
Booty
        ; init the software stack
        move    #>L1DataStack,r7

        move    #-1,m7
        move    m7,m6
        move    m7,m5
        move    m7,m4
        move    m7,m3
        move    m7,m2
        move    m7,m1
        move    m7,m0

;;;;;;;;;;;;;;;;;;  Add Battery BO ;;;;;;;;;;;;;;;;;;;;;;;;;;;
	 ;Skip if connected to 5V
	 move    x:HW_USBCSR,x0
	 btst	#HW_USBCSR_VBUSSENSE_BITPOS,x0
 if (@def('USE_EEPROM'))
     jcc	_CheckBattBO

     ; If USB connected, we'll want to use the USB PLL which draws more
     ; current - which is OK because we're powered by the USB.
     bset    #HW_VDD5V_PWR_CHARGE_DISABLE_ILIMIT_BITPOS,x:HW_VDD5V_PWR_CHARGE

     ; USB Pll is programmed to its default value of 60Mhz
     ; From Steve Vu information
     bset    #HW_CCR_CKRST_BITPOS,x:HW_CCR      	; Enables writes to Clock Control Register
     bset    #HW_CCR_PLLEN_BITPOS,x:HW_CCR      	; Enable (old 3410) PLL because the MUX
                                            		; switching from old Pll to new (USB) PLL
                                            		; requires a clock during this phase (prevents glitches)

     ; if EEPROM, need to use USB PLL because recovery Mode requires USB.
     move    #>$800000,x0
     move    x0,x:HW_USBPHY_BASEADDR           	; Force all PLL regs to default state
     move    #>$000000,x0
     move    x0,x:HW_USBPHY_BASEADDR           	; Power ON the PHY

	; The 3500 datasheet says to CLEAR this bit to initiate a new PLL count
	; Any writes to HW_USBPHYRX must have the PLLLOCKED bit set to avoid resetting the PLL lock sequence
	; Correcting this error will definitely increase the loop time in _wait_loop below.
	bclr    #HW_USBRX_PLLLOCKED_BITPOS,x:HW_USBPHYRX ;Start PLL lock Sequence

    clr     a
	move	#>1600,a0

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
        bset    #HW_CCR_PLL_SOURCE_SEL_BITPOS,x:HW_CCR      ; Select the new PLL (USB)
        bset    #HW_CCR_CKSRC_BITPOS,x:HW_CCR               ; Route clock from Crystal to PLL

        bclr    #HW_CCR_PLLEN_BITPOS,x:HW_CCR               ; Disable (old 3410) PLL now that we have
                                                            ; the new one locked

        bclr    #HW_USBCSR_CLKOFF_BITPOS,x:HW_USBCSR        ; Turns ON clock for USB Block

        jmp     _SkipClockSpeedUp
 else   ; not defined as EEPROM.
    jcs	_SkipBattBO
 endif

_CheckBattBO
	 ;Init battery brownout and shutdown if too low
	 jsr InitBatteryBrownout

	 ; Test Battery Threshold1 to allow player to boot at low battery levels.
	 ; If greater than threshold, speed up the clock
	 btst    #HW_BATT_RESULT_GT_EVENT1_BITPOS,x:HW_BATT_RESULT
	 jcc     _SkipClockSpeedUp
;     jmp     _SkipClockSpeedUp
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

_SkipBattBO
  if (@def('DEVICE_3500'))
    ; Speed Up the DCLK to 60 Mhz
    ; This will speed up USB connection
;~~~~~~~~~~~~~~~~~~~~ CHANGING PLL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        bset    #HW_VDD5V_PWR_CHARGE_DISABLE_ILIMIT_BITPOS,x:HW_VDD5V_PWR_CHARGE

        ; USB Pll is programmed to its default value of 60Mhz

        ; From Steve Vu information
        bset    #HW_CCR_CKRST_BITPOS,x:HW_CCR      	; Enables writes to Clock Control Register
        bset    #HW_CCR_PLLEN_BITPOS,x:HW_CCR      	; Enable (old 3410) PLL because the MUX
                                            		; switching from old Pll to new (USB) PLL
                                            		; requires a clock during this phase (prevents glitches)

        ; otherwise using system clock to reduce power consumption.
		move #HW_CCR_PDIV_CLRMASK,x0
		move  x:HW_CCR,a
		and x0,a
		move #>$002000,x0
		or  x0,a
		move a1,x:HW_CCR

		rep #1000
          nop

		; ** NOTE ** There is currently no provision here for the case where the lock bit is not
		; set within the above loop counter time. It just falls through to _locked
		; Something should be done in the case of the bit not locking.

_locked2
        bset    #HW_CCR_CKSRC_BITPOS,x:HW_CCR               ; Route clock from Crystal to PLL

  endif   ; end of STMP3500.
;--------------------- End PLL Settings ---------------------------------------

_SkipClockSpeedUp
    ; init all variables
    jsr     BootyInit

  if (@def('USE_EEPROM'))
    ; Check for Recovery Mode.
    jsr     CheckForRecoveryMode
    jcs     ChangeToBootUsb           ; go boot from USB
  endif


    ; Find out if we're loading the Player (STMPCode00Blocks) or the DCC
    ;   (STMPCode01Blocks) image.
STMP_CODE_00_BLOCKS     equ     $000000
STMP_CODE_01_BLOCKS     equ     $000001
        jsr     DetectUsb
        jcc     _boot_player
        move    #>STMP_CODE_01_BLOCKS,x0
        jmp     _set_block_tag
_boot_player
        move    #>STMP_CODE_00_BLOCKS,x0
_set_block_tag
        move    x0,x:SmBlockTag

LoadRoutinePtrs
		; Don't need to set FlashInit_1p8V as the 1.8V init routine,
		; because b1p8Volts flag is handled in the BootyInit routine
		; FlashInitPort1 will work for either NAND voltage, because
		; FlashInit_1p8V simply sets b1p8Volts, then calls FlashInitPort1

    if (@def('BOOT_SEQUENCE_ORG'))
        move    #>FlashInitPort1,x0              ; get init routine address
    else
        move    #>FlashInitPort1_Boot,x0         ; get init routine address
    endif
        move    x0,x:InitRoutine        ; save init routine address in memory
        move    #>FlashReceive1,x0              ; get receive routine address
        move    x0,x:ReceiveRoutine     ; save receive routine address in memory
        move    #>FlashExit1,x0              ; get exit routine address
        move    x0,x:ExitRoutine        ; save exit routine address in memory

    if (@def('BOOT_SEQUENCE_ORG'))
        move    #>FlashError1,x0              ; get error routine address
    else
        move    #>FlashError1_Boot,x0         ; get error routine address
    endif
        move    x0,x:ErrorRoutine       ; save error routine address in memory

RunInitRoutine
        move    x:InitRoutine,r6        ; r6=pointer to the port init routine
        nop
        jsr     (r6)                    ; call the port init routine

; init the software stack
        move    #>L1DataStack,r7

        move    #-1,m7
        move    m7,m6
        move    m7,m5
        move    m7,m4
        move    m7,m3
        move    m7,m2
        move    m7,m1
        move    m7,m0

Getkey
        jsr     GetCipherKey            ; get the cipher key... duh.

        jsr     GetCheckSumTarget       ; get the checksum target

        jsr     VerifyVersion           ; make sure the code version matches this

                                        ;   chip revision
MainLoop
        ; receive first word (opcode plus address)
        move    x:ReceiveRoutine,r6     ; r6=pointer to the receive port routine
        nop
        jsr     GetWord                 ; get the next word
        move    x0,y1                   ; y1=copy of original command word

        ;move    x0,x:SaveCommand        ; FOO: Debugging

_DontStop
        ; get the address to initialize
        move    #>$00ffff,a1            ; this could have been done with move x0,rx which would truncate the MSByte
        and     x0,a                    ; but this would not be compatible with future 24 bit address machines
        move    a1,x:InitPtr            ; save a copy in memory

        ; check if cmd opcode = $f (done)
        move    #>$8,y0
        mpy     y1,y0,a
        move    #>$0f,y0
        and     y0,a
        move    a1,a
        move    #>$f,x1
        cmp     x1,a
        jeq     Reboot

        ; get # of words to initialize
        push    y1
        jsr     GetWord                 ; get the next word
        pop     y1
        move    x0,x:InitCount          ; x0=number of words to initialize

        ; switch statement for command
        move    #>$8,y0
        mpy     y1,y0,a
        move    #>$0f,y0
        and     y0,a
        move    a1,a
        move     #>$1,y1
        cmp     y1,a
        jeq     LoadYmem
        move     #>$2,y1
        cmp     y1,a
        jeq     LoadXmem
        move     #>$3,y1
        cmp     y1,a
        jeq     LoadLmem
        move     #>$4,y1
        cmp     y1,a
        jeq     LoadPmem
        move     #>$9,y1
        cmp     y1,a
        jeq     InitYmem
        move     #>$a,y1
        cmp     y1,a
        jeq     InitXmem
        move     #>$b,y1
        cmp     y1,a
        jeq     InitLmem
        move     #>$c,y1
        cmp     y1,a
        jeq     InitPmem
        move     #>$f,y1
        cmp     y1,a
        jeq     Reboot
        ; Invalid command!
        bset    #BOOT_INFO_INVALID_COMMAND,x:BootInfo
        move    x:ErrorRoutine,r0
        nop
        jmp     (r0)

        ; Y memory code lode
LoadYmem
        move    x:InitPtr,r2
        move    x:InitCount,y0
        move    #$ffff,m2
        do      y0,_LoadYmemLoop
        push    r2
        jsr     GetWord                 ; x0=data to initialize memory with
        pop     r2
        nop
        move    x0,y:(r2)+              ; init memory with value
_LoadYmemLoop
        jmp     MainLoop

        ; X memory code lode
LoadXmem
        move    x:InitPtr,r2
        move    x:InitCount,y0
        move    #$ffff,m2
        do      y0,_LoadXmemLoop
        push    r2
        jsr     GetWord                 ; x0=data to initialize memory with
        pop     r2
        nop
        move    x0,x:(r2)+              ; init memory with value
_LoadXmemLoop
        jmp     MainLoop


        ; L memory code lode
LoadLmem
        move    x:InitPtr,r2
        move    x:InitCount,y0
        move    #$ffff,m2
        do      y0,_LoadLmemLoop
        push    r2
        jsr     GetWord                 ; x0=data to initialize memory with
        move    x0,x:LmemSaveHigh
        jsr     GetWord                 ; x0=data to initialize memory with
        pop     r2
        clr     a
        move    x:LmemSaveHigh,a1
        move    x0,a0                    ; a10=data to initialize memory with
        move    a10,L:(r2)+              ; init memory with value
_LoadLmemLoop
        jmp     MainLoop


        ; P memory code lode
LoadPmem
        move    x:InitPtr,r2
        move    x:InitCount,y0
        move    #$ffff,m2
        do      y0,_LoadPmemLoop
        push    r2
        jsr     GetWord                 ; x0=data to initialize memory with
        pop     r2
        nop
        movem   x0,p:(r2)+          ; init memory with value
_LoadPmemLoop
        jmp     MainLoop

        ; Y memory initialization
InitYmem
        jsr     GetWord                 ; x0=data to initialize memory with
        move    x:InitPtr,r2
        move    x:InitCount,y0
        move    #$ffff,m2
        do      y0,_InitYmemLoop
        move    x0,y:(r2)+          ; init memory with value
        nop
_InitYmemLoop
        jmp     MainLoop

        ; X memory initialization
InitXmem
        jsr     GetWord                 ; x0=data to initialize memory with
        move    x:InitPtr,r2
        move    x:InitCount,y0
        move    #$ffff,m2
        do      y0,_InitXmemLoop
        move    x0,x:(r2)+          ; init memory with value
        nop
_InitXmemLoop
        jmp     MainLoop

        ; L memory initialization
InitLmem
        jsr     GetWord                 ; x0=data to initialize memory with
        move    x0,x:LmemSaveHigh
        jsr     GetWord                 ; x0=data to initialize memory with
        clr     a
        move    x:LmemSaveHigh,a1
        move    x0,a0               ; a10=data to init L memory with
        move    x:InitPtr,r2
        move    x:InitCount,y0
        move    #$ffff,m2
        do      y0,_InitLmemLoop
        move    a10,L:(r2)+          ; init memory with value
        nop
_InitLmemLoop
        jmp     MainLoop

        ; P memory initialization
InitPmem
        jsr     GetWord                 ; x0=data to initialize memory with
        move    x:InitPtr,r2
        move    x:InitCount,y0
        move    #$ffff,m2
        do      y0,_InitPmemLoop
        movem   x0,p:(r2)+          ; init memory with value
        nop
_InitPmemLoop
        jmp     MainLoop

;///////////////////////////////////////////////////////////////////////////////
; Reboot
;///////////////////////////////////////////////////////////////////////////////
Reboot

        jsr     ValidateCheckSum
        move    x:ExitRoutine,r5        ; r5=pointer to the exit routine for port
        nop
        jsr     (r5)                    ; call the exit routine, on return, port will be disabled

	; Get off the high-speed PLL before switching to player/usbmsc
	if (@def('DEVICE_3500'))
		;Clear Battery settings
   		move    #>$000c00,x0
   		move    x0,x:HW_BATT_CTRL
		;clear Battery BO
		;set to clear IRQ event in HW_BATT_RESULT
    	bset    #HW_BATT_RESULT_IRQ_EVENT0_BITPOS,x:HW_BATT_RESULT
		bclr    #HW_IPR_IRQB_BITPOS,x:HW_IPR

    	bset	#HW_CCR_CKRST_BITPOS,x:HW_CCR			; Enable writes to the HW_CCR register
		bset	#HW_CCR_PLLEN_BITPOS,x:HW_CCR			; Enable the system PLL
		bclr    #HW_CCR_PLL_SOURCE_SEL_BITPOS,x:HW_CCR		; Select the system PLL as the clock source

    	move    #>$800000,x0
    	move    x0,x:HW_USBPHY_BASEADDR           		; Force all high-speed PLL regs to default state

;        ; Turn off I2C clocks per procedure in STMP00002736 HW defect.
;          This procedure can't be used since it makes usbmsc eeprom boot fail
;            if player is running and we attach usb.
;            We might simply need more code to enable i2c if this disable is used.
;        bset    #HW_I2CCSR_TREQ_BITPOS,x:HW_I2CCSR
;        bclr    #HW_I2CCSR_TREQ_BITPOS,x:HW_I2CCSR
;        bset    #HW_I2CCSR_I2C_EN_BITPOS,x:HW_I2CCSR
;        bclr    #HW_I2CCSR_ACKF_BITPOS,x:HW_I2CCSR
	endif


;test for nikhil's sim
        bclr    #0,omr
        jmp     $0
;--end test
        jmp *



;///////////////////////////////////////////////////////////////////////////////
;   DetectUsb
;
;	Return Carry = 1 if USB is detected, Carry = 0 otherwise
;///////////////////////////////////////////////////////////////////////////////
DetectUsb

    if (@def('DEVICE_3410'))

; Note - this method used by Bill Gordon to fix the USB debounce problem.  For
; more information see STMP3856, STMP2723, and STMP2726.
;    WARN '******* Warning - USB Debounce Change - Check changes.  ********'
        if (1)
          push  a2
          push  a1
          push  a0

          clr     a
          do      #24,_usb_samp ; Insert NOP's on next line to reduce the "sample rate"
            ; This pin detects the USB as either plugged in or not.
             btst    #HW_SPARER_USB_PLUGIN_BITPOS,x:HW_SPARER
             nop     ; Pipeline padding ??? May not be necessary.
             jcc     _sample0
             rol     a  ;  A "1" was detected. Count it (rotate C bit into A1 bit 0)
        _sample0:
             nop        ;  Padding for the "do" instruction (pipeline effects)
        _usb_samp:
          move   a1,a0 ;  Move counted 1's to A0.  btst only checks bits 0 - 23
          btst   #19,a ;  C = 1 for 20 or more 1's, C = 0 otherwise.

          pop   a0
          pop   a1
          pop   a2

        else
          btst    #HW_SPARER_USB_PLUGIN_BITPOS,x:HW_SPARER
        endif

    else

        if (@DEF('SIM_USB20'))

            bset    #SR_CARRY,sr

        else

			; Check for +5V connected, and from what source
			; If +5V not present, USB not connected
			; If +5V present, USB or Charger connected
            move    x:HW_USBCSR,x0
			btst	#HW_USBCSR_VBUSSENSE_BITPOS,x0
            jcc		DetectUsbExit

		; +5V present, so arbitrate USB vs Charger
		; if 5V is present, turn on PWDN_ON_IOBRNOUT
		bset    #HW_VDD5V_PWR_CHARGE_PWDN_ON_IOBRNOUT_BITPOS,x:HW_VDD5V_PWR_CHARGE

        	; Enable USB plug in detect circuitry
        	bset    #HW_USBCSR_PLUGGEDIN_EN_BITPOS,x:HW_USBCSR

			; Datasheet states to wait at least 1ms before relying on the
			; USBCSR_PLUGGEDIN bit. At 60MHz, this is 60,000 cycles.
			; nop instruction = 2 cycles, 3750*8*2 = 60,000
        	rep #3750
            	nop
        	rep #3750    ; note: toverify4   sdk2.6 had only the first rep loop and commented out the others but we likely need them for the full 1ms delay. See comment above. Also we run at 68mHz so we need more looping still.
            	nop
        	rep #3750
            	nop
        	rep #3750
            	nop
        	rep #3750
            	nop
        	rep #3750
            	nop
        	rep #3750
            	nop
        	rep #3750
            	nop

        	; Check USB plugged-in detection
        	move    x:HW_USBCSR,x0

        if (!@def('USE_EEPROM'))
        	; Disable USB plug in detect circuitry
        	bclr    #HW_USBCSR_PLUGGEDIN_EN_BITPOS,x:HW_USBCSR
        endif

        	; Reflect the USBCSR_PLUGGEDIN bit on the Carry bit
        	btst    #HW_USBCSR_PLUGGEDIN_BITPOS,x0

DetectUsbExit

			; Return C=1 if USB plugged in, C=0 if no 5V or 5V alternate source

        endif

    endif
    rts

;///////////////////////////////////////////////////////////////////////////////
;   InitBatteryBrownout
;
;///////////////////////////////////////////////////////////////////////////////
InitBatteryBrownout
	;Set Battery CTRL Bits
   move    #>$000056,x0
   move    x0,x:HW_BATT_CTRL

   ;wait for battery to settle before setting IRQ's
   rep #3750
   nop

   ;Set the thresholds for fast boot and brownout.
   move    #>BATTERY_THRESHOLD,x0
   move    x0,x:HW_BATT_THRSH

   ;Set the polarity
   bclr    #22,x:HW_BATT_CTRL

   ;Clear the IRQ0 Flag (set to clear)
   bset    #HW_BATT_RESULT_IRQ_EVENT0_BITPOS,x:HW_BATT_RESULT

   ;Turn on IRQ
   bset    #20,x:HW_BATT_CTRL

   ;Init Brownout interrupt
   ; init the vector addresses
   move    #>$bf080,x0             ; opcode for JSR
   move    #HW_IVECIRQB,r1
   move    #>SysBrownOutIsr,n1
   move    x0,p:(r1)+
   move    n1,p:(r1)

   bset    #HW_IPR_IRQB_BITPOS,x:HW_IPR

   ; Init the SR bits
   move    sr,x0
   bclr    #HW_SR_IM0_BITPOS,x0
   bclr    #HW_SR_IM1_BITPOS,x0
   move    x0,sr

   rts

;///////////////////////////////////////////////////////////////////////////////
;   SysBrownOutIsr
;
;///////////////////////////////////////////////////////////////////////////////
SysBrownOutIsr
		; Prepare chip for shutdown
    	bset	#HW_CCR_CKRST_BITPOS,x:HW_CCR			; Enable writes to the HW_CCR register
		bset	#HW_CCR_PLLEN_BITPOS,x:HW_CCR			; Enable the system PLL
		bclr    #HW_CCR_PLL_SOURCE_SEL_BITPOS,x:HW_CCR		; Select the system PLL as the clock source

    	move    #>$800000,x0
    	move    x0,x:HW_USBPHY_BASEADDR           		; Force all high-speed PLL regs to default state
		; turn off the DCDC converter
    	bset    #HW_CCR_PWDN_BITPOS,x:HW_CCR    ; If using the DCDC converter then
	                                        ; the part will turn off.  If not
	                                        ; using the DCDC converter then
	                                        ; the part will just kinda stop
	                                        ; working :)
_loop  jmp _loop							; must stop here


	rti

;///////////////////////////////////////////////////////////////////////////////
;   DelayLongBReg
;
;///////////////////////////////////////////////////////////////////////////////
DelayLongBReg
    dec b
    jne DelayLongBReg
    rts

;///////////////////////////////////////////////////////////////////////////////
;   BootyInit
;///////////////////////////////////////////////////////////////////////////////
BootyInit
    ; clear out all the variables
    move    #BOOT_RSRV_SIZE,x1
    move    #BOOT_RSRV_START,r0
    move    #>$ffff,m0
    move    #>$000000,x0
    do      x1,_end_init
        move    x0,x:(r0)+
        nop
_end_init
    nop

    if (@def('DEVICE_3500'))
        ; Setup GP Flash for 3500
        bset    #HW_GPFLASH_CSR1_MUX_BITPOS,x:HW_GPFLASH_CSR1R
    endif


    bset    #HW_DCDC_VDDIO_BROWNOUT_ENABLE_BITPOS,x:HW_DCDC_VDDIO

    move    #>BOOTY_VERSION,x0
    move    x0,x:BootyVersion
    clr     a
    move    a,x:BootInfo
    move    #-1,m3                  ; make m3 linear, used in bset addressing

    ; Setup some timeout variables
    move    #>TIMEOUT_SLOW_HIGH,x0
    move    x0,x:TimeOutInitHigh
    move    #>TIMEOUT_SLOW_LOW,x0
    move    x0,x:TimeOutInitLow

    if (@def('DEVICE_3500'))
	; Before powering up the GPIO, determine from mode bit 3 if this is a 1.8V
	; NAND (0) or 3.3V NAND (1)
	; ** Note that other boot modes in the 3500 also use bit 3 = 0, so if this becomes
	; an issue, then more arbitration needs to be done here
	; Using this simple method to minimize memory usage

	; Power up mode pin 3, enable it as an input, and check it.
	bset	#HW_BOOT_MODE_BIT_3_GPIO,x:HW_GP0PWR	; Power up the pin
	bclr	#HW_BOOT_MODE_BIT_3_GPIO,x:HW_GP0DOER	; Configure as an input
	bset	#HW_BOOT_MODE_BIT_3_GPIO,x:HW_GP0ENR	; Enable the pin as a GPIO
	nop												; Necessary?
	btst	#HW_BOOT_MODE_BIT_3_GPIO,x:HW_GP0DIR	; Get the state of the pin
	nop												; Necessary?
	jcs     NANDIs3_3V								; If 1, then 3.3V NAND boot

NANDIs1_8V

CF_8_BIT_1p8VOLTS       equ     $3f
	; Setup proper voltage on Flash before turning the drivers on.
	; Enables all GPFLASH lines EXCEPT the extra 8 data lines as 1.8V
	move    #>CF_8_BIT_1p8VOLTS,a
	move    a,x:HW_GLUE_1_8V_PAD_EN

	;Set the 1.8V flag (clear by default)
	move	#>1,a
	move	a,p:b1p8Volts

NANDIs3_3V

	endif

    ; Enable HOLD switch GPIO before powering up. If it is not set-up as a GPIO, it
	; will drive low.  This will cause excessive current if the HOLD switch is on .
	bset #HW_GP_B11_BITPOS,x:HW_GP1ENR

    ; Power Up All Pads
    ; Don't power up GP0 because this will power the data and sclock pins of
    ; the MMC - Sometimes Renesas will brownout and get into Reset/Power/Reset
    ; loop.
;    move    a1,x:HW_GP0PWR
;    move    a1,x:HW_GP1PWR  ;Stmp 9048 : Commented out this line and added the following 2 lines for TB5 USB compliance.
;    move    #0,a1
	move    #>$2370ff,a1
	move	a1,x:HW_GP1PWR
;; GP52 can be MMC_WP(I). If power up it is CF_IORDn(O), there will be conflict and draw excessive current
;    move    #-1,a1
    move    #$ffffef,a1
    move    a1,x:HW_GP2PWR

    rts

;///////////////////////////////////////////////////////////////////////////////
;   GetCipherKey
;///////////////////////////////////////////////////////////////////////////////
GetCipherKey
    ; cipher key is always the first word
    move    x:ReceiveRoutine,r6     ; r6=pointer to the receive port routine
    nop
    jsr     (r6)                    ; call the receive routine, on return x0=received word
    move    x0,x:CipherKey          ; save it, duh.
    rts

;///////////////////////////////////////////////////////////////////////////////
;   GetWord
; Entry:
;   r6 = Receive routine ptr
; Exit:
;   r6 = Receive routine ptr
;   x0 = PlainText
;///////////////////////////////////////////////////////////////////////////////
GetWord
    jsr     (r6)                    ; call the receive routine, on return x0=received word
    jsr     DecryptWord             ; decrypt it... duh!
    jsr     UpdateCheckSum          ; add it to the running checksum
    ; update the word count
    clr     b
    move    x:WordCount,b0
    inc     b
    move    b0,x:WordCount
    rts

;///////////////////////////////////////////////////////////////////////////////
;   DecryptWord
; Entry:
;   x0 = CipherText
; Exit:
;   x0 = PlainText
;///////////////////////////////////////////////////////////////////////////////
DecryptWord
    move    x:CipherKey,b
    tst     b
    jne     _key_is_one
    ; key is zero
    bclr    #HW_SR_C_BITPOS,sr
    jmp     _shift_it
_key_is_one
    clr     b
    bset    #HW_SR_C_BITPOS,sr
_shift_it
    move    x0,b
    rol     b
    move    b1,x0
    btst    #HW_SR_C_BITPOS,sr
    jcs     _new_key_is_one
    clr     b
    move    b,x:CipherKey
    rts
_new_key_is_one
    move    #>$000001,b
    move    b1,x:CipherKey
    rts

;///////////////////////////////////////////////////////////////////////////////
;   UpdateCheckSum
; Entry:
;   x0 = data
; Exit:
;   x0 = data
;///////////////////////////////////////////////////////////////////////////////
UpdateCheckSum
    clr     b
    move    x:CheckSum,b1
    add     x0,b
    move    b1,x:CheckSum
    rts

;///////////////////////////////////////////////////////////////////////////////
;   GetCheckSumTarget
;///////////////////////////////////////////////////////////////////////////////
GetCheckSumTarget
    ; checksum target is always the second word
    move    x:ReceiveRoutine,r6     ; r6=pointer to the receive port routine
    nop
    jsr     (r6)                    ; call the receive routine, on return x0=received word
    jsr     DecryptWord             ; decrypt it
    move    x0,x:CheckSumTarget     ; save it, duh.
    move    #>$000000,x0            ; clear out the running checksum
    move    x0,x:CheckSum
    rts

;///////////////////////////////////////////////////////////////////////////////
;   ValidateCheckSum
;///////////////////////////////////////////////////////////////////////////////
ValidateCheckSum
    move    x:CheckSum,x0
    move    x:CheckSumTarget,a
    cmp     x0,a
    jne     _uh_oh
    rts
_uh_oh
    ; the checksum is bad!
    move    x:BootInfo,x0
    bset    #BOOT_INFO_CHECKSUM,x0
    move    x0,x:BootInfo
    move    x:ErrorRoutine,r6
    nop
    jmp     (r6)

;///////////////////////////////////////////////////////////////////////////////
;   VerifyVersion
;///////////////////////////////////////////////////////////////////////////////
VerifyVersion
    ; version is always the third word
    move    x:ReceiveRoutine,r6     ; r6=pointer to the receive port routine
    nop
    jsr     GetWord
    clr     a
    move    x:BootyVersion,a1
    cmp     x0,a
    jne     _uh_oh
    rts
_uh_oh
    ; well, this just won't do!
;    move    #BootInfo,r3           ; m3 is linear from BootyInit
;    nop
;    bset    #BOOT_INFO_VERSION,x:(r3)
    bset    #BOOT_INFO_VERSION,x:BootInfo
    ; move on
    move    x:ErrorRoutine,r6      ; r6=pointer to the errir routine
    nop
    jmp     (r6)


;///////////////////////////////////////////////////////////////////////////////
;   SetupTimeOut
;  The timer decrements a 48-bit acc value that is initialized as follows:
;       a2 = 0
;       a1 = number of ticks
;       a0 = 0
;  Therefore, a tick is equal to 2^24 dec operations on a.  Assuming a one-cycle
;    timeout loop gives:
;       (1/24.576Mhz)*(2^24) = 682.7ms per tick
;    or
;       (1/48Mhz)*(2^24) = 349.5ms per tick
;  5 minute timeout => 5*60=300s
;  23 instructions for 24.576MHz timeout loop
;   1. 300s/682.7ms=439.4 ticks for a one-cycle timeout loop, or
;     439.4/23=19.1 ticks for a 23-cycle loop.
;   2. 300s/349.5ms=858.4 ticks for a one-cycle timeout loop, or
;     858.4/????=???? ticks for a ?-cycle loop.
;///////////////////////////////////////////////////////////////////////////////
SetupTimeOut
    move    x:TimeOutInitHigh,x1
    move    x1,x:TimeOutHigh
    move    x:TimeOutInitLow,x1
    move    x1,x:TimeOutLow
    rts

;///////////////////////////////////////////////////////////////////////////////
;   CheckTimeOut
;///////////////////////////////////////////////////////////////////////////////
CheckTimeOut
    clr     b                       ; 1
    move    x:TimeOutHigh,b1        ; 2
    move    x:TimeOutLow,b0         ; 2
    dec     b                       ; 1
    jeq     _timed_out              ; 2
    move    b1,x:TimeOutHigh        ; 2
    move    b0,x:TimeOutLow         ; 2
    bclr    #0,sr                    ; 1
    rts                             ; 1
_timed_out
    move    x:BootInfo,b0
    bset    #BOOT_INFO_TIMEOUT,b0
    move    b0,x:BootInfo
    bset    #0,sr
    rts


  if (@def('USE_EEPROM'))

;///////////////////////////////////////////////////////////////////////////////
; CheckForRecoveryMode
;  Recovery Mode is entered if the following occurs:
;     Boot mode pins are set for SmartMedia, power up with USB connected, and
;     the Play/Power button is pressed for 5 seconds
;///////////////////////////////////////////////////////////////////////////////

CheckForRecoveryMode
    ; TODO: the addition of play/pswitch recovery modes and no time to do this
    ;   cleanly has made this function a piece of unmitigated crud.

_PlayRecoveryMode
    jsr     SetupPlayButton
    move    #DetectPlayButton,r5

    jsr     ClearDclkCountRegister

_play_hold_loop
    ; bail if Play is not pressed or if USB gets disconnected
    jsr     DetectUsb
    jcc     _not_emergency_mode
    jsr     (r5)
    jcc     _not_emergency_mode
    ; check for timeout
    jsr     ReadDclkCountRegister
    move    #>TIMEOUT_RECOVERY_LOW,y0
    move    #>TIMEOUT_RECOVERY_HIGH,y1
    sub     y,a
    jlt     _play_hold_loop

    ; emergency mode!
    bset    #HW_SR_C_BITPOS,sr
    jmp     _Done

_not_emergency_mode
    bclr    #HW_SR_C_BITPOS,sr

_Done
    rts

;///////////////////////////////////////////////////////////////////////////////
;  ClearDclkCountRegister
;///////////////////////////////////////////////////////////////////////////////
ClearDclkCountRegister
    clr     a
    move    a,x:$ffea
    move    a,x:$ffeb
    rts

;///////////////////////////////////////////////////////////////////////////////
;  ReadDclkCountregister
;///////////////////////////////////////////////////////////////////////////////
ReadDclkCountRegister
    clr     a
    move    x:$ffeb,a1                  ; get the upper count
    move    x:$ffea,a0                  ; get the lower count
    move    x:$ffeb,b                   ; get the upper count again
    move    a1,x0                       ; make sure it's the same
    cmp     x0,b
    jne     ReadDclkCountRegister
    rts

;///////////////////////////////////////////////////////////////////////////////
;  DetectPswitch
;///////////////////////////////////////////////////////////////////////////////
DetectPswitch
    move    x:HW_SPARER,x0
    btst    #HW_SPARER_PSWITCH_BITPOS,x0
    rts

;///////////////////////////////////////////////////////////////////////////////
;  SetupPlayButton
;///////////////////////////////////////////////////////////////////////////////
SetupPlayButton
    ;  The Play/Power pin is pin 14, GPIO1 bit 10
    bset    #HW_GP_B10_BITPOS,x:HW_GP1ENR
    clr     b
    move    #>DELAY_SETUP_PLAY_BUTTON,b0               ; 100us @ 24.576MHz
_wait
    dec     b
    jne     _wait
    rts

;///////////////////////////////////////////////////////////////////////////////
;  DisablePlayButton
;///////////////////////////////////////////////////////////////////////////////
;DisablePlayButton
;    ;  The Play/Power pin is pin 14, GPIO1 bit 10
;_PlayRecoveryMode
;    bclr    #HW_GP_B10_BITPOS,x:HW_GP1ENR;

;_not_play_recovery
;    nop
;    rts

;///////////////////////////////////////////////////////////////////////////////
;  DetectPlayButton
;///////////////////////////////////////////////////////////////////////////////
DetectPlayButton
    move    x:HW_GP1DIR,x0
    btst    #HW_GP_B10_BITPOS,x0
    rts

  endif     ; if EEPROM

;///////////////////////////////////////////////////////////////////////////////
;   ChangeToBootUsb
;///////////////////////////////////////////////////////////////////////////////
ChangeToBootUsb
    move    x:BootInfo,x0
    move    #>$800000,a
    and     x0,a
    move    a1,x:BootInfo
    move    #0,sp                       ; clear the hardware stack
    move    #>UsbInitPort,x0            ; get init routine address
    move    x0,x:InitRoutine            ; save init routine address in memory
    move    #>UsbReceive,x0             ; get receive routine address
    move    x0,x:ReceiveRoutine         ; save receive routine address in memory
    move    #>UsbExit,x0                ; get exit routine address
    move    x0,x:ExitRoutine            ; save exit routine address in memory
    move    #>UsbError,x0               ; get error routine address
    move    x0,x:ErrorRoutine           ; save error routine address in memory
    jmp     RunInitRoutine              ; do it!

;///////////////////////////////////////////////////////////////////////////////
;   VDDIO Brownout Detect
;   Since the WP line is always high, we need to force it low here.
;///////////////////////////////////////////////////////////////////////////////
VDDIOBrownout

    ; Disable Write Protect - Renesas has RES# tied to WP#
    bset    #HW_GP_B0_BITPOS,x:HW_GP2ENR
    bclr    #HW_GP_B0_BITPOS,x:HW_GP2DOR
    bset    #HW_GP_B0_BITPOS,x:HW_GP2DOER

    clr     b
    move    #>WAIT_10_MS,b0
    asl     b
    asl     b
    asl     b                       ; delay for 80ms
    jsr     DelayLongBReg

    jmp     Booty
