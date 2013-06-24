;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2005
;
;  File        : syserror.asm
;  Description : System Error Handler
;///////////////////////////////////////////////////////////////////////////////

    section SYSFUN_Error

    opt     mex

    page    255,255,0

;///////////////////////////////////////////////////////////////////////////////
;   Macros
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "hwequ.inc"
    include "resource.inc"
    list

;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

    global    SysErrorInit
    global    SysDefaultAllVectors
    global    FatalError
;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include   "const.xref"
    list

 if (@DEF('STMP_BUILD_PLAYER'))    
    extern    CurrentModule
 endif ;    
 if (!@DEF('STMP_BUILD_PLAYER'))
    extern    FSystemShutdown
 endif
;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////

LCD_CS_BitPos               equ  8      ; LCD Chip Select
LCD_RW_BitPos               equ  9      ; LCD Write/Read
LCD_DS_BitPos               equ  10     ; LCD Data Sync ('E') signal
LCD_RS_BitPos               equ  11     ; LCD RS bit

LCD_GPIO_DATA_INPUT_MASK    equ  $ffff00
LCD_GPIO_DATA_OUTPUT_MASK   equ  $0000ff
LCD_GPIO_ALL_ENABLE_MASK    equ  $000fff

;///////////////////////////////////////////////////////////////////////////////
;   X Memory
;///////////////////////////////////////////////////////////////////////////////

    org x,"SYSFUN_Error_X":

 if (@DEF('BETA'))

LcdInitSequence   ; From system_lcd_init_seq.src  (this is for new shingyih lcd june2005)
; dc     00002A
; dc     000002
; dc     00000D   ; size of file
 dc     0000AE   ; LCD panel off
 dc     0000A2   ; Set LCD Bias - POR default bias (see solomon controller data sheet)
 dc     0000A9   ; extended cmd (set bias ratio, set TC value, modify osc freq)
 dc     0000E2   ; bias ratio 1/9 or 1/7, (TC0,POR), osc freq ??? out of register range
 dc     000024   ; Internal Regulator Resistor ratio
 dc     00002F   ; Set power control reg (all on)
 dc     000081   ; Set contrast (ext command)
 dc     00002C   
 dc     0000C8   
 dc     0000A0   ; Set segment remap = 0
 dc     0000A6   ; normal display
 dc     000040   ; Display start line = 0
 dc     0000AF   ; Display on


HexImageArray
    dc  $AEC1FF,$C1BAB6,$BDFFFF,$FFBF80,$AE9DFF,$B9B6B6,$B6DDFF
    dc  $C9B6B6,$EBE7FF,$EF80ED,$B6D0FF,$CEB6B6,$B5C3FF,$CFB6B6
    dc  $8EFEFF,$FCFAF6,$B6C9FF,$C9B6B6,$B6F9FF,$E1D6B6,$EE81FF
    dc  $81EEEE,$B680FF,$C9B6B6,$BEC1FF,$DDBEBE,$BE80FF,$C1BEBE
    dc  $B680FF,$BEB6B6,$F680FF,$FEF6F6

 endif ; if (@DEF('BETA'))

;///////////////////////////////////////////////////////////////////////////////
;   Y Memory
;///////////////////////////////////////////////////////////////////////////////

    org y,"SYSFUN_Error_Y":

;///////////////////////////////////////////////////////////////////////////////
;   P Memory
;///////////////////////////////////////////////////////////////////////////////

    org p,"SYSFUN_Error_P":

  if (@DEF('STMP_BUILD_PLAYER'))   
;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysDefaultAllVectors
;
;   Type:           Function
;
;   Description:    This function prefills all 2 word interrupt vectors with: 
;                   'debug nop'   or      'jmp FatalError' which resets the DSP.
;                   For DEBUG     or      RETAIL builds.    
;
;   Inputs:         None
;
;   Outputs:        None
;                   
;   Notes:          Assumes vector table size of 64 two-word entries at P:0.
;<
;///////////////////////////////////////////////////////////////////////////////
FSysDefaultAllVectors
SysDefaultAllVectors
    if (@DEF('RETAIL'))
     move    #>$0AF080,x0          ; Opcode for JMP (no JSR since no return)
     move    #FatalError,x1        ; Address of FatalError ISR, resets DSP
    else ; BETA or DEBUG
     move    y:<Const_000200,x0    ; Opcode for DEBUG
     move    y:<Const_000000,x1    ; Opcode for NOP
    endif 

    move    #$0000,r0 ;Set only 63 vect now now since 64th vector COULD hold 'jsr SysCallFunclet' (defined in 2400_syscallfunction.asm)
    do      #63,_endVectInit ;old comment: should be #64 (was #40) to fill 128 ($80) vector words: 128/2 wordsPerVect= decimal 64 vectors.
        movem    x0,p:(r0)+
        movem    x1,p:(r0)+
_endVectInit

    rts
 endif
 ;     STMP_BUILD_PLAYER

 if (@DEF('STMP_BUILD_PLAYER')) 
;///////////////////////////////////////////////////////////////////////////////
;>  Name:           SysErrorInit
;   Type:           Function
;   Description:    Initializes all "error" interrupt vectors to be handled by 
;                   the trapping software for RETAIL and BETA builds. 
;                   For DEBUG builds, debug instructions at "error" vectors.
;                   If BETA build, this function initializes the display code 
;                   for the error handler. Supports only the LCD hardware.  
;   Inputs:         None
;   Outputs:        None
;   Notes:          DEBUG and RETAIL build versions verified for SDK3410_2.200.
;                   BETA builds are not recommended currently.
;<
;///////////////////////////////////////////////////////////////////////////////   
SysErrorInit
    ; Init the LCD controller
    if (@DEF('BETA'))
       jsr     SysErrLcdInit
    endif ; if (@DEF('BETA'))

    if (@DEF('BETA')||@DEF('RETAIL'))
       move    #>$0BF080,x0             ; Opcode for JSR
    else
	if (@DEF('TRACEBUFFER'))
	  move #>$0BF080,x0             ; Opcode for JSR
	else
       move    y:<Const_000200,x0       ; Opcode for DEBUG
	endif
    endif ; if (@DEF('BETA')||@DEF('RETAIL'))
    ;----------------
    ; Setup the hardware reset vector
    move    #HW_IVECRESET,r1            ; Point to reset vector
    if (@DEF('BETA'))
       move    #HardwareResetIsr,x1     ; Address of ISR
    else
       if (@DEF('RETAIL'))
          move    #FatalError,x1        ; Address of ISR
       else ; DEBUG
	  if (@DEF('TRACEBUFFER'))
	    move #DisableTraceBufferAndStop,x1
	  else
          move    y:<Const_000000,x1    ; Opcode for NOP
	  endif
       endif ; if (@DEF('RETAIL'))
    endif ; if (@DEF('BETA'))
    movem    x0,p:(r1)+
    movem    x1,p:(r1)
    ;----------------
    ; Setup the stack error vector
    move    #HW_IVECSTERR,r1            ; Point to stack error vector
    if (@DEF('BETA'))
       move    #StackErrorIsr,x1        ; Address of ISR
    else
       if (@DEF('RETAIL'))
          move    #FatalError,x1        ; Address of ISR
       else ; DEBUG
	if (@DEF('TRACEBUFFER'))
	  move #DisableTraceBufferAndStop,x1
	else
          move    y:<Const_000000,x1    ; Opcode for NOP
	 endif
       endif ; (@DEF('RETAIL'))
    endif ; if (@DEF('BETA'))
    movem    x0,p:(r1)+
    movem    x1,p:(r1)
    ;----------------
    ; Setup the trace vector
    move    #HW_IVECTRAC,r1             ; Point to trace vector
    if (@DEF('BETA'))
       move    #TraceIsr,x1             ; Address of ISR
    else
       if (@DEF('RETAIL'))
          move    #FatalError,x1        ; Address of ISR
       else ; DEBUG
	if (@DEF('TRACEBUFFER'))
	  move #DisableTraceBufferAndStop,x1
	else
          move    y:<Const_000000,x1    ; Opcode for NOP
	endif
       endif ; if (@DEF('RETAIL'))
    endif ; if (@DEF('BETA'))
    movem    x0,p:(r1)+
    movem    x1,p:(r1)
    ;----------------
    ; Setup the software interrupt vector
    move    #HW_IVECSWI,r1              ; Point to SWI vector
    if (@DEF('BETA'))
       move    #SoftwareInterruptIsr,x1 ; Address of ISR
    else
       if (@DEF('RETAIL'))
          move    #FatalError,x1        ; Address of ISR
       else ; DEBUG
          move    y:<Const_000000,x1    ; Opcode for NOP
       endif ; if (@DEF('RETAIL'))
    endif ; if (@DEF('BETA'))
    movem    x0,p:(r1)+
    movem    x1,p:(r1)
    ;----------------
    ; Setup the illegal instruction vector
    move    #HW_IVECILI,r1              ; Point to illegal instruction vector
    if (@DEF('BETA'))
       move    #IllegalInstructionIsr,x1 ; Address of ISR
    else
       if (@DEF('RETAIL'))
          move    #FatalError,x1        ; Address of ISR
       else
	if (@DEF('TRACEBUFFER'))
	  move #DisableTraceBufferAndStop,x1
	else
          move    y:<Const_000000,x1    ; Opcode for NOP
	endif
       endif ; if (@DEF('RETAIL'))
    endif ; if (@DEF('BETA'))
    movem    x0,p:(r1)+
    movem    x1,p:(r1)
    ;----------------
    ; Setup the fatal error vector
    move    #HW_IVECERROR,r1            ; Point to error vector
    if (@DEF('BETA'))
       move    #>$0AF080,x0             ; Opcode for JMP (no JSR since no return)
       move    #ErrorIsr,x1             ; Address of ISR
    else ; DEBUG or RETAIL
       if (@DEF('RETAIL'))
          move    #>$0AF080,x0          ; Opcode for JMP (no JSR since no return)
          move    #FatalError,x1        ; Address of ISR
       else ; DEBUG
	if (@DEF('TRACEBUFFER'))
	  move #>$0BF080,x0
	  move #DisableTraceBufferAndStop,x1
	else	
          move    y:<Const_000200,x0    ; Opcode for DEBUG
          move    y:<Const_000000,x1    ; Opcode for NOP
	endif
       endif ; if (@DEF('RETAIL'))
    endif ; if (@DEF('BETA'))
    movem    x0,p:(r1)+
    movem    x1,p:(r1)

    rts
 endif 
      ; (@DEF('STMP_BUILD_PLAYER'))    

;///////////////////////////////////////////////////////////////////////////////



    if (@DEF('TRACEBUFFER'))

    ; This code will turn off the 3410 Trace Buffer (so that the OnCE debugger
    ; doesn't fill the buffer up with its memory scans) and then stop at a
    ; DEBUG.

    ; Change points of crashes (e.g. Stack overflow interrupt vector) to jump
    ; to DisableTraceBufferAndStop, to turn off Trace Buffer before stopping
    ; the processor.


    global  DisableTraceBufferAndStop
DisableTraceBufferAndStop

  IF (@DEF('DEVICE_3410')||@DEF('DEVICE_3310'))
    move    x:$f080,y0
    bclr    #0,y0
    move    y0,x:$f080

    nop        ; Probably unnecessary delay...
    nop
    nop

  ENDIF ; IF (@DEF('DEVICE_3410')||@DEF('DEVICE_3310'))

    debug
    nop
    nop

    rti

    endif ; if (@DEF('TRACEBUFFER'))
	

;///////////////////////////////////////////////////////////////////////////////
;>  Name:           HardwareResetIsr
;   Type:           Interrupt Vector
;   Description:    Interrupt vector for hardware reset
;   Inputs:         none
;   Outputs:        none
;   Notes:          none
;<
;///////////////////////////////////////////////////////////////////////////////
 if (@DEF('BETA'))

HardwareResetIsr

    ; Clear the DM bit (double-precision multiply mode, bit 6) of the Mode Register
    ; This will be returned to its previous state when the RTI occurs (since the SR
    ; will be popped back off System Stack Low (SSL).
    ; Accumulator behavior is unpredictable when the DM bit is set.
    ; Next cycle after clearing it should NOT be an ALU instruction.
    andi #$BF,mr

    ; The error code is 0000
    move    y:<Const_000000,b0

    jmp     FatalError

 endif  ; if (@DEF('BETA'))


;///////////////////////////////////////////////////////////////////////////////
;>  Name:           StackErrorIsr
;   Type:           Interrupt Vector
;   Description:    Hardware vector for stack error
;   Inputs:         none
;   Outputs:        none
;   Notes:          none
;<
;///////////////////////////////////////////////////////////////////////////////
 if (@DEF('BETA'))
StackErrorIsr
    ; Clear the DM bit (double-precision multiply mode, bit 6) of the Mode Register
    ; This will be returned to its previous state when the RTI occurs (since the SR
    ; will be popped back off System Stack Low (SSL).
    ; Accumulator behavior is unpredictable when the DM bit is set.
    ; Next cycle after clearing it should NOT be an ALU instruction.
    andi #$BF,mr

    ; The error code is 0002
    move    y:<Const_000002,b0

    jmp     FatalError

 endif ; if (@DEF('BETA'))

;///////////////////////////////////////////////////////////////////////////////
;>  Name:           TraceIsr
;   Type:           Interrupt Vector
;   Description:    Hardware vector for trace
;   Inputs:         none
;   Outputs:        none
;   Notes:          none
;<
;///////////////////////////////////////////////////////////////////////////////
 if (@DEF('BETA'))
TraceIsr
    ; Clear the DM bit (double-precision multiply mode, bit 6) of the Mode Register
    ; This will be returned to its previous state when the RTI occurs (since the SR
    ; will be popped back off System Stack Low (SSL).
    ; Accumulator behavior is unpredictable when the DM bit is set.
    ; Next cycle after clearing it should NOT be an ALU instruction.
    andi #$BF,mr

    ; The error code is 0004
    move    y:<Const_000004,b0

    jmp     FatalError

 endif ; if (@DEF('BETA'))

;///////////////////////////////////////////////////////////////////////////////
;>  Name:           SoftwareInterruptIsr
;   Type:           Interrupt Vector
;   Description:    Hardware vector for software interrupt
;   Inputs:         none
;   Outputs:        none
;   Notes:          none
;<
;///////////////////////////////////////////////////////////////////////////////
 if (@DEF('BETA'))
SoftwareInterruptIsr
    ; Clear the DM bit (double-precision multiply mode, bit 6) of the Mode Register
    ; This will be returned to its previous state when the RTI occurs (since the SR
    ; will be popped back off System Stack Low (SSL).
    ; Accumulator behavior is unpredictable when the DM bit is set.
    ; Next cycle after clearing it should NOT be an ALU instruction.
    andi #$BF,mr

    ; The error code is 0006
    move    #>$0006,b0

    jmp     FatalError

 endif ; if (@DEF('BETA'))


;///////////////////////////////////////////////////////////////////////////////
;>  Name:           IllegalInstructionIsr
;   Type:           Interrupt Vector
;   Description:    Hardware vector for illegal instruction interrupt
;   Inputs:         none
;   Outputs:        none
;   Notes:          none
;<
;///////////////////////////////////////////////////////////////////////////////
 if (@DEF('BETA'))
IllegalInstructionIsr
    ; Clear the DM bit (double-precision multiply mode, bit 6) of the Mode Register
    ; This will be returned to its previous state when the RTI occurs (since the SR
    ; will be popped back off System Stack Low (SSL).
    ; Accumulator behavior is unpredictable when the DM bit is set.
    ; Next cycle after clearing it should NOT be an ALU instruction.
    andi #$BF,mr

    ; The error code is 0038
    move    #>$0038,b0

    jmp     FatalError

 endif ; if (@DEF('BETA'))

;///////////////////////////////////////////////////////////////////////////////
;>  Name:           ErrorIsr
;   Type:           Interrupt Vector
;   Description:    Hardware vector for error
;   Inputs:         none
;   Outputs:        none
;   Notes:          none
;<
;///////////////////////////////////////////////////////////////////////////////
 if (@DEF('BETA'))
ErrorIsr
    ; Clear the DM bit (double-precision multiply mode, bit 6) of the Mode Register
    ; This will be returned to its previous state when the RTI occurs (since the SR
    ; will be popped back off System Stack Low (SSL).
    ; Accumulator behavior is unpredictable when the DM bit is set.
    ; Next cycle after clearing it should NOT be an ALU instruction.
    andi #$BF,mr

    ; Get the return address off the stack
    movec   ssh,b0                      ; pop

    ; Backup to the address of the error
    dec     b

    jmp     FatalError_nospclear

 endif ; if (@DEF('BETA'))


;///////////////////////////////////////////////////////////////////////////////
;>  Name:           FatalError
;   Type:           Interrupt Service Routine
;   Description:    Reset the SOC iff this is a RETAIL or BETA build. Clear sp.
;   Inputs:         b0 = 16-bit error code used in BETA build only
;   Outputs:        
;   Notes:          DEBUG build version does nothing and returns. 
;<
;///////////////////////////////////////////////////////////////////////////////
FatalError
 if (@DEF('BETA'))
    ; Init the Stack Pointer
    move    y:<Const_000000,sp
 endif ; if (@DEF('BETA'))

;///////////////////////////////////////////////////////////////////////////////
;>  Name:           FatalError_nospclear
;   Type:           Interrupt Service Routine
;   Description:    Reset the SOC iff this is a RETAIL or BETA build. sp unchanged.
;   Inputs:         b0 = 16-bit error code used in BETA build only
;   Outputs:        
;   Notes:          DEBUG build version does nothing and returns.
;<
;///////////////////////////////////////////////////////////////////////////////
FatalError_nospclear
 if (@DEF('BETA')||@DEF('RETAIL'))
    ; Mute the headphones
    bset    #HW_MIXVOLUMER_MUTE_BITPOS,x:HW_MIXMASTERVR 
 endif 
 if (@DEF('BETA'))
    ; Change the display to reverse video
    move    #>$A7,x0
    jsr     SysErrSendCmd

    ; Goto row 0 (actually page 0)
    move    #>$B0,x0
    jsr     SysErrSendCmd

    ; Goto column 0 (if display is not upside down then column 34)
    move    y:<Const_000010,x0
    jsr     SysErrSendCmd
    move    y:<Const_000000,x0
    jsr     SysErrSendCmd

    ; Save the error code
    move    b0,r1

    ; Display the software version
    move    #>VERSION_MINOR,b0
    jsr     DisplayHexValue

    ; Restore the error code
    move    r1,b0

    ; Display the error code
    jsr     DisplayHexValue

    ; Restore the error code again
    move    r1,a

    ; If the error address (error code) is in a module
    move    #CurrentModule,y1
    cmp     y1,a
    jlt     _Wait

    ; Get the resource number of the current module
    move    y1,r1
    nop
    movem   p:(r1),b0

    ; Display the resource number
    jsr     DisplayHexValue

_Wait
    ; Delay for 3 seconds so the error can be read
    move    #>3000,x1
    do      x1,_Delay
        move    y:<Const_008000,x0
        rep     x0
            nop
        nop
_Delay

    ; Make sure that the GPIO pin for the play button is correct
    bset    #HW_GP_B10_BITPOS,x:HW_GP1ENR
    bclr    #HW_GP_B10_BITPOS,x:HW_GP1DOER

    ; Wait until the play button is pressed
_CheckPlay
    btst    #HW_GP_B10_BITPOS,x:HW_GP1DIR
    jcc     _CheckPlay

 endif ; if (@DEF('BETA'))

 if (@DEF('BETA')||@DEF('RETAIL'))

    ; Disable interrupts in the core. 0 is the 3410/35xx reset value.
    move    y:<Const_000000,x0
    move    x0,x:HW_IPR 

    ; NOTE:  we should disable all peripherals here manually
 if (@DEF('STMP_BUILD_PLAYER'))
    ; Turn off the DAC. 0 is the 3410/35xx reset value.
    move    x0,x:HW_DACCSR
 endif

    ; Tristate the LCD outputs so correct boot mode will be detected. GPIO 0 
    ; Enable Reg is cleared to perform assigned function (LCD) rather than GPIO.
    move    x0,x:HW_GP0ENR

    ; Clear out the interrupt collector to reset values.
    move    #HW_ICOLL_BASEADDR,r0
    move    y:<Const_ffffff,m0
 if (@DEF('DEVICE_3410'))
    do      #(HW_ICLOBSVZ1R-HW_ICOLL_BASEADDR),_reset_icoll ; 3410 ver verified
 else
    do      #(HW_ICLPRIOR4R-HW_ICOLL_BASEADDR),_reset_icoll ; 35xx ver has 1 more reg
 endif
        move    x0,x:(r0)+
        nop
_reset_icoll

    ; Delay a few ms letting the boot lines discharge.
    ; Use nops since interrupts are disabled.  
    ; At 36Mhz, delays 5.5 ms. (At 60 MHz, 3.33 ms. At 10 MHz, 20 ms.)
    move    #>200000,x0
    rep     x0
        nop

    ; SysRTCShutdown routine may be needed [update: not for 35xx] to prevent RTC corruption.
    ; Note that DCDC convertor shutdown can cause RTC corruption if this is not
    ; called first, but we are resetting here, so the DCDC convertor is not shutdown 
    ; and therefore the internal Real Time Clock may be ok without this call.  
    ; SysRTCShutdown not needed for stmp35xx & not defined for 35xx SDKs so commented. 

 if (!@DEF('STMP_BUILD_PLAYER'))
    ; if not player (usbmsc and mtp have this) Shutdown usb, gpio, pad power, and PLL
    jsr FSystemShutdown ; With this call & RCR set below, I've verified we re-enumerate MTP RETAIL app ok. 
 endif

    ; STMP35xx datasheet ver 1.07 RCR:SRST says we should use the normal DCDC voltage change 
    ; algorithm to restore the DCDC state to its default state before issuing this soft reset, 
    ; else may lead to unstable DCDC converter operation. 
    ; But we have low p space & can't call funclet from isr, so risk it as before & don't set vddd.
    ;   
    ;WARNING: this routine uses funclets and we're in an ISR so can't use it unless drop to user mode. 
    ;jsr SysSpeedSysStartUp ; sets vddd=default 1.37V & sets fDCLK=10MHz 
    ;Above line may be needed for stable reset since it steps down to avoid vddd undershoot brownout with 1 large step

    ; Force a "digital wide" reset condition via reset control register write

    ;Reset 35xx/3410 with bits [7:4]=1011= 0xD hex 
    move    #>$0F07D0,x0 ;HW_RCR bit values 0x0F07D0 verified STMP3410 9/25/02. 35xx verified   
    move    x0,x:HW_RCR ; Reset Control Register
    jmp     *           ; jump to this line until the processor reset takes affect
 endif ; if (@DEF('BETA')||@DEF('RETAIL'))
    rts                 ; only reached in DEBUG case if this were called in a DEBUG build (should not be). 



;///////////////////////////////////////////////////////////////////////////////
;>  Name:           DisplayHexValue
;   Type:           Function
;   Description:    ???
;   Inputs:         b0 = 16-bit value
;   Outputs:        ???
;   Notes:          ???
;<
;///////////////////////////////////////////////////////////////////////////////
 if (@DEF('BETA'))
DisplayHexValue
    ; Position the value at the top of B0
    rep     #8
        asl     b

    ; Make sure the image pointer is linear
    move    y:<Const_ffffff,m0

    ; Setup a mask for selecting a lower byte
    move    #>$FF,y1

    ; There are four nibbles in a value
    do      #4,_ValueLoop

        ; Clear previous nibble
        move    y:<Const_000000,b1

        ; Shift the value up a nibble into B1
        rep     #4
            asl     b

        ; Get the most significant nibble
        move    b1,a

        ; Get the hex image to be displayed for this nibble
        ; R0 = HexImageArray + (nibble * 2)
        move    #HexImageArray,x0
        lsl     a
        add     x0,a
        move    a1,r0

        ; There are two words in an image
        do      #2,_ImageLoop

            ; Get a word of the image
            move    x:(r0)+,a1

            ; There are three bytes in a word
            do      #3,_WordLoop

                ; Save the word
                move    a1,y0

                ; Put the lower byte into X0
                and     y1,a
                move    a1,x0

                ; Display the byte on the LCD
                jsr     SysErrSendData

                ; Shift the word down a byte
                mpy     y0,#8,a

                nop
                nop
_WordLoop
            nop
            nop

_ImageLoop
        nop
        nop

_ValueLoop
    nop
    nop

    ; Display a space
    do      #6,_SpaceLoop
        move    #>$FF,x0
        jsr     SysErrSendData
        nop
_SpaceLoop
    nop

    rts

 endif ; if (@DEF('BETA'))

;///////////////////////////////////////////////////////////////////////////////
;>  Name:           SysErrLcdInit
;   Type:           Function
;   Description:    Sets up an LCD Write Command, LCD Write Data, and LCD Read Data
;                   using the GPIO block and manually forcing pins for data signals
;                   including the CS,RS,RW & 'E' (LCD Data Sync) signals.
;   Inputs:         ???
;   Outputs:        ???
;   NOtes:          This routine accomplishes the following:
;                   1. Enables GPIO pins to be GPIO functions with GPIO0 Enable Reg
;                   2. Sets the initial value of the 'E' data sync to be 0
;                   3. Sets up signal pins (DS,CS,RS,RW) to be outputs
;                   4. Sends an LCD initialization string
;                   5. Sends an LCD Write data command to display an ASCII character
;                   6. Sets up an LCD Read Data transaction manually
;                   7. Loops back to step 4 continuously
;<
;///////////////////////////////////////////////////////////////////////////////
 if (@DEF('BETA'))
SysErrLcdInit
    ; We must first enable the GPIO pins we want to use for input/output to LCD
    move    #>LCD_GPIO_ALL_ENABLE_MASK,x1
    ; Set the bits in the GPIO 0 Enable reg
    move    x:HW_GP0ENR,a               ; Get the current contents of the GPIO enable reg
    or      x1,a                        ; Set the bits that are of concern using mask in x1
    move    a1,x:HW_GP0ENR              ; Send new value out to the enable register

    ; Set initial value of 'E' (LCD Data Sync) to be zero when output enabled
    bclr    #LCD_DS_BitPos,x:HW_GP0DOR

    ; Enable the LCD Control Signal data output bits (CS,A0/RS,DS,E) to be output & present using Data Out Enable Reg
    ; Set up the GPIO bits for data output enable register to enable for output (set to 1)
    move    #>LCD_GPIO_ALL_ENABLE_MASK,x1
    move    x:HW_GP0DOER,a              ; Get the current contents of the GPIO Data Out Enable Reg
    or      x1,a                        ; Set the bits that are of concern using mask in x1
    move    a1,x:HW_GP0DOER             ; Send new value out to the Data Out Enable Reg to force pins

    ; cycle through the init sequence
    move    x:LcdInitSequence,x0
    move    #>LcdInitSequence+1,r0
    move    y:<Const_ffffff,m0
    do      x0,_end_lcd_init_seq
    move    x:(r0)+,x0
    jsr     SysErrSendCmd
    nop
_end_lcd_init_seq
    nop

    rts

 endif ; if (@DEF('BETA'))


;///////////////////////////////////////////////////////////////////////////////
;>  Name:           SysErrSendCmd
;   Type:           Function
;   Description:    Sends a command byte to the LCD
;   Inputs:         x0 = hex byte command value to send to LCD
;   Outputs:        none
;   Notes:          none
;<
;///////////////////////////////////////////////////////////////////////////////
 if (@DEF('BETA'))
SysErrSendCmd
    move    #>LCD_GPIO_DATA_INPUT_MASK,x1
    ; Set the bits in the GPIO Data Out Reg
    move    x:HW_GP0DOR,a               ; Get the current contents of the GPIO Data Out Reg
    and     x1,a                        ; Clear out the D0-D7 data bits
    or      x0,a                        ; Set the bits that are of concern using mask in x1
    move    a1,x:HW_GP0DOR              ; Send new value out to the Data Out Reg

    ; Now set up control bits for write command (RS,CS,RW = 0)
    bclr    #LCD_RS_BitPos,x:HW_GP0DOR
    bclr    #LCD_RW_BitPos,x:HW_GP0DOR
    nop                                 ; Delay some cycles for RW to settle
    nop

    ; Set up the 'E' Data Sync pulse to sync data
    bset    #LCD_DS_BitPos,x:HW_GP0DOR ; Set 'E' to 1 (high)
    bclr    #LCD_CS_BitPos,x:HW_GP0DOR ; Clear the CS bit
    ; Calculated at 48 MHz clocking
    ; At 48 MHz, T=20.833 nanosec/cycle.  'E' pulse high width (tEWHW) is 60 nsec minimum.
    ; Therefore, 60/20.833 = 2.88, so we need 3 clock cycles, creating a pulse width of 62.499 nsec.
    nop
    nop
    nop
    ; Set CS bit, Clear DS bit for pulse and cycle completion
    bset    #LCD_CS_BitPos,x:HW_GP0DOR ; Set the CS bit
    bclr    #LCD_DS_BitPos,x:HW_GP0DOR ; Clear 'E' for Data Sync
    ; For Enable Cycle Time (t cyc6 - a complete cycle time) is 300 nsec minimum.
    ; Therefore, (300 - 63)/20.833 = 11.38, so we need 12 clock cycles to complete LCD transfer cycle.
;        nop     ; 1
;        nop
;        nop
;        nop
;        nop     ; 5
;        nop
;        nop
;        nop
;        nop
;        nop     ; 10
;        nop
;        nop

    rep #3          ;(4 + 2) cycles
        nop         ;2 cycles each

    rts                                 ; END LcdDrvSendCmd

 endif ; if (@DEF('BETA'))


;///////////////////////////////////////////////////////////////////////////////
;>  Name:           SysErrSendData
;   Type:           Function
;   Description:    Sends a data byte to the LCD
;   Inputs:         x0 = hex byte data value to send to LCD
;   Outputs:        none
;   Notes:          none
;<
;///////////////////////////////////////////////////////////////////////////////
 if (@DEF('BETA'))
SysErrSendData
    move    #>LCD_GPIO_DATA_OUTPUT_MASK,a1
    and     x0,a                        ;do this to make sure x1 bit(23-16) are zero
    move    a1,x1
    move    #>LCD_GPIO_DATA_INPUT_MASK,x0
    ; Set the bits in the GPIO Data Out Reg, HW_GP0DOER should already be set to output from previous cmd write
    ; So bits should immediately set to this value using HW_GP0DOR reg
    move    x:HW_GP0DOR,a              ; Get the current contents of the GPIO Data Out Reg
    and     x0,a                        ; Clear out the D0-D7 data bits
    or      x1,a                        ; Set the bits that are of concern using mask in x1
    move    a1,x:HW_GP0DOR             ; Send new value out to the Data Out Reg

    ; Now set up control bits for write command (CS=0,RW=0,RS=1)
    bset    #LCD_RS_BitPos,x:HW_GP0DOR
    bclr    #LCD_RW_BitPos,x:HW_GP0DOR
    nop                                 ; Delay some cycles for RW to settle
    nop

    ; Set up the 'E' Data Sync signal to sync data
    bset    #LCD_DS_BitPos,x:HW_GP0DOR ; Set 'E' to 1 (high)
    bclr    #LCD_CS_BitPos,x:HW_GP0DOR

    ; Set up the 'E' Data Sync pulse to sync data
    bset    #LCD_DS_BitPos,x:HW_GP0DOR ; Set 'E' to 1 (high)
    bclr    #LCD_CS_BitPos,x:HW_GP0DOR ; Clear the CS bit
    ; Calculated at 48 MHz clocking
    ; At 48 MHz, T=20.833 nanosec/cycle.  'E' pulse high width (tEWHW) is 60 nsec minimum.
    ; Therefore, 60/20.833 = 2.88, so we need 3 clock cycles, creating a pulse width of 62.499 nsec.
    nop
    nop
    nop
    ; Set CS bit, Clear DS bit for pulse and cycle completion
    bset    #LCD_CS_BitPos,x:HW_GP0DOR ; Set the CS bit
    bclr    #LCD_DS_BitPos,x:HW_GP0DOR ; Clear 'E' for Data Sync
    ; For Enable Cycle Time (t cyc6 - a complete cycle time) is 300 nsec minimum.
    ; Therefore, (300 - 63)/20.833 = 11.38, so we need 12 clock cycles to complete LCD transfer cycle.
;        nop     ; 1
;        nop
;        nop
;        nop
;        nop     ; 5
;        nop
;        nop
;        nop
;        nop
;        nop     ; 10
;        nop
;        nop
    
    rep #3          ;(4 + 2) cycles
        nop         ;2 cycles each
        
    rts                                 ; END LcdDrvSendData

 endif ; if (@DEF('BETA'))

        endsec

