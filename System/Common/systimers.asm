;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2001
;
;  File        : systimers.asm
;  Description : STMP System Timers
;///////////////////////////////////////////////////////////////////////////////

    section SYSFUN_SysTimers
    page    132,60,0,0,0

;///////////////////////////////////////////////////////////////////////////////
;   Macros
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "sysmacro.asm"
    list

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "hwequ.inc"
    include "decoderequ.inc"
    include "buttons.inc"
    list
    
;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

    global  SysResetAllTimers
    global  SysEnableTimer
    global  SysDisableTimer
    global  SysClearTimerIrq
    global  SysLoadTimerCount
    global  DecoderTimerNum
    global  EncoderTimerNum
    global  ButtonTimerNum
    global  SysTimerNum

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "const.xref"
    list

;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////
        
; To ADD a new timer:
; 1. EQUate a timer number below with a name to be used by calling functions.
; 2. Set the TimerNControlInit to the value that it should be init'd to.
; 3. Set the TimerNCountInit to the count value to init to.
; 4. Call the functions below as needed, with the proper inputs.

DecoderTimerNum   equ      0
EncoderTimerNum   equ      0
ButtonTimerNum    equ      1
SysTimerNum       equ      2

Timer0ControlInit equ      $000002
Timer1ControlInit equ      $000002
Timer2ControlInit equ      $000002

Timer0CountInit   equ      DECODER_TIMER_SYNC_COUNT
Timer1CountInit   equ      BUTTON_TIMER_COUNT
Timer2CountInit   equ      $004E20

;///////////////////////////////////////////////////////////////////////////////
;   X Memory
;///////////////////////////////////////////////////////////////////////////////
        
    org     x,"SYSFUN_SysTimers_X":

;///////////////////////////////////////////////////////////////////////////////
;   Y Memory
;///////////////////////////////////////////////////////////////////////////////

    org     y,"SYSFUN_SysTimers_Y":

;///////////////////////////////////////////////////////////////////////////////
;   P Memory
;///////////////////////////////////////////////////////////////////////////////

    org     p,"SYSFUN_SysTimers_P":

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysResetAllTimers
;
;   Type:           Function
;
;   Description:    Configures all 3 timer control and count registers to
;                   default values.
;
;   Inputs:         none
;
;   Outputs:        none
;                   
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
SysResetAllTimers

    ; init the Timer Control Registers
    move    #>Timer0ControlInit,x0
    move    x0,x:HW_TMR0CR
    move    #>Timer1ControlInit,x0
    move    x0,x:HW_TMR1CR
    move    #>Timer2ControlInit,x0
    move    x0,x:HW_TMR2CR

    ; init the Timer Count Registers
    move    #>Timer0CountInit,x0
    move    x0,x:HW_TMR0CNTR
    move    #>Timer1CountInit,x0
    move    x0,x:HW_TMR1CNTR
    move    #>Timer2CountInit,x0
    move    x0,x:HW_TMR2CNTR
    rts

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysEnableTimer
;
;   Type:           Function
;
;   Description:    Enables the timer number specified by setting the enable bit.
;
;   Inputs:         x0 = Timer Number
;                   r0 = Return Address
;
;   Outputs:        none
;                   
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
SysEnableTimer
    move    x0,a
    tst     a
    jne     _one
_zero
    bset    #HW_TMRCR_TE_BITPOS,x:HW_TMR0CR
    jmp     (r0)
_one
    move    y:<Const_000001,x0
    cmp     x0,a
    jne     _two
    bset    #HW_TMRCR_TE_BITPOS,x:HW_TMR1CR
    jmp     (r0)
_two
    move    y:<Const_000002,x0
    cmp     x0,a
    jne     _error
    bset    #HW_TMRCR_TE_BITPOS,x:HW_TMR2CR
    jmp     (r0)
_error
    error

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysDisableTimer
;
;   Type:           Function
;
;   Description:    Disables the timer number specified by clearing the enable bit.
;
;   Inputs:         x0 = Timer Number
;                   r0 = Return Address
;
;   Outputs:        none
;                   
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
SysDisableTimer
    move    x0,a
    tst     a
    jne     _one
_zero
    bclr    #HW_TMRCR_TE_BITPOS,x:HW_TMR0CR
    jmp     (r0)
_one
    move    y:<Const_000001,x0
    cmp     x0,a
    jne     _two
    bclr    #HW_TMRCR_TE_BITPOS,x:HW_TMR1CR
    jmp     (r0)
_two
    move    y:<Const_000002,x0
    cmp     x0,a
    jne     _error
    bclr    #HW_TMRCR_TE_BITPOS,x:HW_TMR2CR
    jmp     (r0)
_error
    error

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysClearTimerIrq
;
;   Type:           Function
;
;   Description:    Clears the interrupt by reading the control register for the timer number specified.
;
;   Inputs:         x0 = Timer Number
;                   r0 = Return Address
;
;   Outputs:        none
;                   
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
SysClearTimerIrq
    move    x0,a
    tst     a
    jne     _one
_zero
    move    x:HW_TMR0CR,x0
    jmp     (r0)
_one
    move    y:<Const_000001,x0
    cmp     x0,a
    jne     _two
    move    x:HW_TMR1CR,x0
    jmp     (r0)
_two
    move    y:<Const_000002,x0
    cmp     x0,a
    jne     _error
    move    x:HW_TMR2CR,x0
    jmp     (r0)
_error
    error

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysLoadTimerCount
;
;   Type:           Function
;
;   Description:    Loads the value in x1 into the timer count register for the timer specified in x0.
;
;   Inputs:         x0 = Timer Number
;                   x1 = Timer Count Value
;                   r0 = Return Address
;
;   Outputs:        none
;                   
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
SysLoadTimerCount
    move    x0,a
    tst     a
    jne     _one
_zero
    move    x1,x:HW_TMR0CNTR
    jmp     (r0)
_one
    move    y:<Const_000001,x0
    cmp     x0,a
    jne     _two
    move    x1,x:HW_TMR1CNTR
    jmp     (r0)
_two
    move    y:<Const_000002,x0
    cmp     x0,a
    jne     _error
    move    x1,x:HW_TMR2CNTR
    jmp     (r0)
_error
    error

    endsec




