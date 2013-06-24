;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2001-2003
;
; File : sysgpioa.asm from bootgpio.asm
; Description : GPIO routines
;///////////////////////////////////////////////////////////////////////////////

    section     SECTION_BootGpio

;///////////////////////////////////////////////////////////////////////////////
;  Macros
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;  Includes
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "hwequ.inc"
    list
        
;///////////////////////////////////////////////////////////////////////////////
;  External Definitions
;///////////////////////////////////////////////////////////////////////////////

;    GLOBAL  SysGpioPadPowerUp
    GLOBAL  SysGpioEnableAsInput_as
    GLOBAL  SysGpioEnableAsOutput_as
    GLOBAL  SysGpioDisable_as
    GLOBAL  SysGpioGetInputState_as
    GLOBAL  SysGpioSetOutputState_as
    
;///////////////////////////////////////////////////////////////////////////////
;  External References
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;  Equates
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;  X Memory
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;  Y Memory
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;  P Memory
;///////////////////////////////////////////////////////////////////////////////

    org p:

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysGpioPadPowerUp
;
;   Type:           Function
;
;   Description:    
;
;   Inputs:         x0 = GPIO number
;
;   Outputs:        sr.Carry = 0 Success
;                              1 Fail
;                   x0 = GPIO number
;
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
    comment ~
SysGpioPadPowerUp
    
    nop
    ; get the GPB base address and bit masks
    jsr     GpioGetGpbInfo
    jcs     _exit
    
    ; safety
    move    #>$FFFF,m0
    
    ; power up the pad
    move    #HW_GPB_POWER_UP_OFFSET,n0     ; get offset to power up reg
    clr     a                                   ; setup a
    move    x:(r0+n0),a1                        ; get the power up register
    or      y1,a                                ; mask in the bit
    move    a1,x:(r0+n0)                        ; set the hardware bit
    bclr    #HW_SR_CARRY_BITPOS,sr              ; success
    
_exit
    rts
    ~
;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysGpioEnableAsInput
;
;   Type:           Function
;
;   Description:    
;
;   Inputs:         x0 = GPIO number
;
;   Outputs:        sr.Carry = 0 Success
;                              1 Fail
;                   x0 = GPIO number
;
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
SysGpioEnableAsInput_as

    ; account for C call
    move    a1,x0
    
    ; get the GPB base address and bit masks
    jsr     GpioGetGpbInfo
    jcc     _continue
    move    #1,a
    rts
    
_continue
    ; safety
    move    #>$FFFF,m0
    
    ; make sure the output enable bit is cleared
    move    #HW_GPB_GPDOER,n0  ; get offset to output en reg
    clr     a                                   ; setup a
    move    x:(r0+n0),a1                        ; get the output en register
    and     y0,a                                ; mask the bit
    move    a1,x:(r0+n0)                        ; clear the hardware bit
    bclr    #HW_SR_C_BITPOS,sr              ; success

    ; enable the pin as a GPIO
    move    #HW_GPB_GPENR,n0       ; get offset to enable reg
    clr     a                                   ; setup a
    move    x:(r0+n0),a1                        ; get the enable register
    or      y1,a                                ; mask in the bit
    move    a1,x:(r0+n0)                        ; set the hardware bit
    bclr    #HW_SR_C_BITPOS,sr              ; success
    
_exit
    clr     a
    rts

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysGpioEnableAsOutput
;
;   Type:           Function
;
;   Description:    
;
;   Inputs:         x0 = GPIO number
;
;   Outputs:        sr.Carry = 0 Success
;                              1 Fail
;                   x0 = GPIO number
;
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
SysGpioEnableAsOutput_as

    ; account for C call
    move    a1,x0
    
    ; get the GPB base address and bit masks
    jsr     GpioGetGpbInfo
    jcc     _continue
    move    #1,a
    rts
    
_continue
    ; safety
    move    #>$FFFF,m0
    
    ; set the output enable bit
    move    #HW_GPB_GPDOER,n0  ; get offset to output en reg
    clr     a                                   ; setup a
    move    x:(r0+n0),a1                        ; get the output en register
    or      y1,a                                ; set the bit
    move    a1,x:(r0+n0)                        ; clear the hardware bit
    bclr    #HW_SR_C_BITPOS,sr              ; success

    ; enable the pin as a GPIO
    move    #HW_GPB_GPENR,n0       ; get offset to enable reg
    clr     a                                   ; setup a
    move    x:(r0+n0),a1                        ; get the enable register
    or      y1,a                                ; mask in the bit
    move    a1,x:(r0+n0)                        ; set the hardware bit
    bclr    #HW_SR_C_BITPOS,sr              ; success
    
_exit
    clr     a
    rts

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysGpioDisable
;
;   Type:           Function
;
;   Description:    
;
;   Inputs:         x0 = GPIO number
;
;   Outputs:        sr.Carry = 0 Success
;                              1 Fail
;                   x0 = GPIO number
;
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
SysGpioDisable_as

    ; account for C call
    move    a1,x0

    ; get the GPB base address and bit masks
    jsr     GpioGetGpbInfo
    jcc     _continue
    clr     a
    move    #1,a1
    rts
    
_continue    ; safety
    move    #>$FFFF,m0
    
    ; Disable the pin as a GPIO
    move    #HW_GPB_GPENR,n0       ; get offset to enable reg
    clr     a                                   ; setup a
    move    x:(r0+n0),a1                        ; get the enable register
    and     y0,a                                ; mask the bit
    move    a1,x:(r0+n0)                        ; clear the hardware bit
    bclr    #HW_SR_C_BITPOS,sr              ; success
    clr     a
    
_exit
    rts

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysGpioGetInputState
;
;   Type:           Function
;
;   Description:    
;
;   Inputs:         x0 = GPIO number
;
;   Outputs:        sr.Carry = 0 Success
;                              1 Fail
;                   sr.Zero  = 0  Bit not set
;                              1  Bit set
;                   x0 = GPIO number
;
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
SysGpioGetInputState_as

    ; account for C call
    move    a1,x0

    ; get the GPB base address and bit masks
    jsr     GpioGetGpbInfo
    jcc     _continue
    clr     a
    move    #1,a1
    rts                               
    
_continue
    ; safety
    move    #>$FFFF,m0
    
    ; Sense the pin state
    move    #HW_GPB_GPDIR,n0            ; get offset to data in reg
    clr     a                           ; setup a
    move    x:(r0+n0),a1                ; get the data in register
    and     y1,a                        ; isolate the bit
    bclr    #HW_SR_C_BITPOS,sr          ; success
    tst     a
    jeq     _exit
    clr     a
    move    #1,a1

_exit
    rts

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysGpioSetOutputState
;
;   Type:           Function
;
;   Description:    
;
;   Inputs:         x0 = GPIO number
;
;   Outputs:        sr.Carry = 0 Success
;                              1 Fail
;                   sr.Zero  = 0  Bit not set
;                              1  Bit set
;                   x0 = GPIO number
;
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
SysGpioSetOutputState_as

    ; account for C call
    move    a1,x0
    move    b1,r5                       ; should never be anything other than
                                        ;  0 or 1.

    ; get the GPB base address and bit masks
    jsr     GpioGetGpbInfo
    jcc     _continue
    clr     a
    move    #1,a1
    rts                               
    
_continue
    move    r5,b                        ; restore the bit state
    ; safety
    move    #>$FFFF,m0
    
    ; Set the pin state
    move    #HW_GPB_GPDOR,n0            ; get offset to data out reg
    clr     a                           ; setup a
    move    x:(r0+n0),a1                ; get the enable register
    tst     b                           ; check for set or clear
    jeq     _clear
    or      y1,a                        ; mask in the bit
    jmp     _done
_clear
    and     y0,a                        ; clear the bit
_done
    move    a1,x:(r0+n0)                ; set the hardware bit
    bclr    #HW_SR_C_BITPOS,sr          ; success
    clr     a
_exit
    rts

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           GpioGetGpbInfo
;
;   Type:           Function
;
;   Description:    
;
;   Inputs:         x0 = GPIO number
;
;   Outputs:        sr.Carry = 0 Success
;                              1 Fail
;                   r0 = GPB base address
;                   y0 = clear mask
;                   y1 = set mask
;                   x0 = GPIO number
;
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
GpioGetGpbInfo

    ; modulo-24 on the GPIO number to determine the GPB block & bit number
    move    #>$0AAAAB,x1                    ; 1/24 in 24Q0
    mpy     x0,x1,a
    asr     a                           ; a1 = GPB
    move    a1,y0                       ; y0 = GPB
    clr     a               #>24,y1
    mpy     y0,y1,b         x0,a1
    asr     b                           ; b0 = 24*GPB
    move    b0,y1                       ; y1 = 24*GPB
    sub     y1,a                        ; a = GPIONumber-(24*GPB) = bit number
    move    a1,x1                       ; x1 = bit number
    
    ; now get the GPB base address
    move    #>HW_GPB0_BLOCKNUM,b
    cmp     y0,b            #>HW_GPB0_BASEADDR,r0
    jne     _try_1
    jmp     _find_masks
_try_1    
    move    #>HW_GPB1_BLOCKNUM,b
    cmp     y0,b            #>HW_GPB1_BASEADDR,r0
    jne     _try_2
    jmp     _find_masks
_try_2    
    move    #>HW_GPB2_BLOCKNUM,b
    cmp     y0,b            #>HW_GPB2_BASEADDR,r0
    jne     _try_3
    jmp     _find_masks
_try_3    

    ; TODO:  need the ifdef(3410) for the following stuff
;    move    #>HW_GPB3_BLOCKNUM,b
;    cmp     y0,b            #>HW_GPB3_BASEADDR,r0
;    jne     _uh_oh
;    jmp     _find_masks
;_uh_oh
;    bset    #HW_SR_CARRY_BITPOS,sr              ; failed

    error            ; resets for RETAIL build, halt for debug build 

    ; now find the clear & set masks
_find_masks    
    clr     b
    clr     a
    move    x1,a0
    move    #>$000001,b0
_create_set_mask_loop
    dec     a
    jlt     _done_set
    asl     b
    jmp     _create_set_mask_loop
_done_set    
    move    b0,y1           ; y1 = set mask
    move    b0,b1
    not     b
    move    b1,y0           ; y0 = clear mask
    
_exit
    bclr    #HW_SR_C_BITPOS,sr              ; success
    rts

    endsec
