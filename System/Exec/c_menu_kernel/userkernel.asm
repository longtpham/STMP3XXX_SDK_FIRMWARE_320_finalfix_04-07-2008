;*******************************************************************************
; Copyright (C) SigmaTel, Inc. 2001-2003    
; 
; Filename: userkernel.asm
;     
;*******************************************************************************

    section USERTASKKERNEL
    include "project.inc"
    include "hwequ.inc"
    include "msgequ.inc"
    include "irqequ.inc"
    include "sysmacro.asm"

    EXTERN SysPostMessage
    EXTERN MenuTailPtr
    EXTERN MenuHeadPtr
    EXTERN MenuEventSignalMask
    EXTERN SysCheckForButton

    GLOBAL ContextSwitch
    GLOBAL UserTaskKernel
    GLOBAL InitUserTask
    GLOBAL MenuIntervalHigh
    GLOBAL MenuIntervalLow
    GLOBAL pOldStack

;Variables in Y
    GLOBAL MenuWaitingMsg
	org y,"MenuManager_Stack_y":

    if !@def(MENU_STACK_SIZE)
MENU_STACK_SIZE equ 199
    endif ;!@def(MENU_STACK_SIZE)

UserTaskSoftwareStack:
    DUP MENU_STACK_SIZE
    dc $CCC0DE
    ENDM
EndUserTaskSoftwareStack
    dc $DEADBE


	org y,"SYSMOD_SystemApi_Y":
    pOldStack:          dc UserTaskSoftwareStack;

    MenuIntervalHigh:   dc 0
    MenuIntervalLow:    dc 0

MenuWaitingMsg
    dc 0

    extern FUserTask

;The user task kernel aka MenuManager_p section
	org p,"MenuManager_p":

;*******************************************************************************
; Description: Switches task contexts and checks for msgs and buttons pending.
; Inputs:      
; Outputs:     Conditionally sets y:MenuEventSignalMask EVENT_BUTTON_BITPOS EVENT_MESSAGE_BITPOS 
; Notes:       

UserTaskKernel:
    push    SSH
    move #_ReturnToExec,r1 
    push r1
    jmp ContextSwitch       ;this does not corrupt A! and it will switch to the user task
    ;nothing between the context switch above and _ReturnToExec gets executed!
_ReturnToExec
    ;by the time we get here, SysWaitOnEvent or SysCallFunction has loaded a1 with the proper wait mask.
    ;do NOT corrupt A between here and the RTS!
_CheckForMessagesPending
    move y:MenuTailPtr,x0
    move y:MenuHeadPtr,b
    cmp x0,b
    jeq _CheckForButtonsPending
    bset #>EVENT_MESSAGE_BITPOS,y:MenuEventSignalMask   ;make sure we know there's still messages left
_CheckForButtonsPending
    push    a1
    jsr     SysCheckForButton
    pop     a
    jcc     _NoButtonsPending
    bset #>EVENT_BUTTON_BITPOS,y:MenuEventSignalMask   ;make sure we know there's still messages left
_NoButtonsPending
    pop     SSH
    nop
    rts

;*******************************************************************************
; Description: This function swaps the soft stack around between the old soft stack and 
;              the current one. Needs to be improved before we move to multiple tasks.
; Inputs:      y:pOldStack    ,     r7 (soft stack pointer)
; Outputs:     
; Notes:       Blocks interrupts during stack swap.

SwapStack:
	;Mask off interrupts
    move    #>SYS_TIME_IVL_MASK,b1
    
    move    x:HW_IPR,x0
    clr b
    move    b1,x:HW_IPR                    ; disables decoder timer IVL

    ;swap the stacks.
    move y:pOldStack,r6 
    move r7,y:pOldStack
    move r6,r7

    ; assertion testpoint : r7 in expected range

    ;reenable interrupts
    move    x0,x:HW_IPR                    ; re-enables the IVL line of decoder
	rts

;*******************************************************************************
; Description: Infinite asm loop calls non-returning C function: UserTask()
; Inputs:      none
; Outputs:     none
; Notes:       This routine is not returned from. The SysWaitOnEvent(0,0,-1) 
;  at the end of UserTask() will essentially terminate the user task and wait for
;  the kernel to shut the player down.

UserTask
    jsr FUserTask
    jmp UserTask

;*******************************************************************************
; Description: This ModuleTableEntry Init function Loads UserTask's context.
; Inputs:      SSH HW stack register
; Outputs:     A is the event wait value to be written in ModuleTableEntry.wait
;              Specifies when to call this module next.
; Notes:   

InitUserTask:
    push    SSH           ;we decend the hardware stack, so save it off.  (This is ok because we swap stacks twice!)
    jsr SwapStack               ;swap to the user stack
    move #>-1,x0                ;put -1 into the stack (maybe for error checking?!)
    push x0
    move #UserTask,r6
    push r6
    push SR
    jsr SwapStack

    ; Wait on no event. None bit runs module's process func ASAP once since 
    ; ModuleTableEntry.EventSignal default has EVENT_NONE set also. 
    move    #>EVENT_NONE,A 
    pop     SSH
    nop
    rts


;*******************************************************************************
; Description: Save status reg of current context, swap the software stack to 
;              new context, pop & use new context's Status Reg & Program Counter.
; Inputs:      SR to save
; Outputs:     SR and PC changed to new context
; Notes:       

ContextSwitch
    push SR
    jsr SwapStack           ;swap's the stack
    pop SSL                 ;
    pop SSH                 ;gets the PC that was pushed onto the software stack
    nop                     ;pipeline!
    rti                     ;this completes the context switch. 
                            ; rti pops PC and SR from SSH and SSL

    endsec 
