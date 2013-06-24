;******************************************************************************
; Copyright(C) SigmaTel, Inc. 2000
;
; Routines to process the module table
;******************************************************************************

        section SYSFUN_Module

        opt     mex
        opt     op

;*******************************************************************************
;  Include files
;*******************************************************************************

        nolist
        include "sysresources.inc"
        include "msgequ.inc"
        include "sysequ.inc"
 if (@DEF('FUNCLET'))           
    include "resource.inc"
 endif
        list

        page    132,60,0,0,0


;*******************************************************************************
;  Extern definitions
;*******************************************************************************

    global    InitModules
    global    ExecuteModules
    global    SignalModule
    global    ModuleSetTimer
    global    CurrentModule

    global    Module_EventSignal      
    global    Module_EventWait        
    global    Module_ResourceNumber   
    global    Module_QueueDescriptor  
    global    Module_ProcessEvent     
    global    Module_Init             

    global    FCallModule_debug
;*******************************************************************************
;  Extern references
;*******************************************************************************

    include "systime.xref"
    include "const.xref"

    extern  ModuleTable
    extern  SysLoadResource

    extern  MODULE_COUNT
    extern  MODULE_ENTRY_SIZE

 if (@DEF('FUNCLET'))           
    extern  SysCallFunclet
 endif


;*******************************************************************************
;   Equates
;*******************************************************************************

; Offsets within a module table entry
Module_EventSignal      equ     0
Module_EventWait        equ     1
Module_ResourceNumber   equ     2
Module_QueueDescriptor  equ     3
Module_ProcessEvent     equ     4
Module_Init             equ     5
Module_Timer_High       equ     6
Module_Timer_Low        equ     7
Module_Background       equ     8


;*******************************************************************************
;  Macros
;*******************************************************************************

    include "sysmacro.asm"

;*******************************************************************************
;  X memory
;*******************************************************************************
      org     x,"SYSFUN_Module_X":


;*******************************************************************************
;  Y memory
;*******************************************************************************
      org     y,"SYSFUN_Module_Y":

;ModuleNumberShadow      dc      0
;ModuleWaitShadow        dc      0
;ModuleSignalShadow      dc      0

;*******************************************************************************
;  P memory
;*******************************************************************************
    org p,"CurrentModule_p":
CurrentModule:
        dc  0


      org     p,"SYSFUN_Module_P":

;*******************************************************************************
; InitModule
;   Entry:
;       r1 = ptr to ModuleTableEntry
;*******************************************************************************
InitModule
    ; Save variables
    push    m1
    push    r1

    ; Check if the module needs to be loaded
    move    #Module_ResourceNumber,n1
    clr     a
    move    y:(r1+n1),x0        ; Resource number of this module
    cmp     x0,a                ; If resource number is zero  
    jeq     _CallModule         ; No need to load the module
    
    ; Setup arguments for calling SysLoadResource
    move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
    move    #>MAX_MODULE_SIZE,y1            ; Size of the overlay area
    move    #>F_lc_u_bP_Module_overlay,y0   ; Address of the overlay area
    move    #TARGET_MEM_P,a1                ; Overlay area is in P memory

    ; Load the module into the overlay area
    move    x0,p:CurrentModule              ;put the new module into the receptical.  
    jsr     SysLoadResource

_CallModule
    ; Get the pointer to the module table entry
    pop     r1
    push    r1

    ; Call the module's initialization routine
    move    #Module_Init,n1
    nop
    move	r1,r0	;  Pointer is passed on r0 for to C routine
    move    y:(r1+n1),r2
    nop
    jsr     (r2)

    

    ; Restore variables
    pop     r1
    pop     m1
    move    #Module_EventWait,n1
    nop     
    move    x0,y:(r1)           ;write in the new flags to the wait mask 
    rts
    

;*******************************************************************************
; SignalModule
;
; Input Parameters:
;   X1: Module Number
;   X0: Event to be signaled
;*******************************************************************************

SignalModule
        ; Calculate the offset into the module table
        move    #>MODULE_ENTRY_SIZE,y0
        mpy     x1,y0,a                 ; Offset = module number * entry size
        asr     a                       ; Offset is in a0

        ; Point to the selected entry in the module table
        move    #>ModuleTable,r1
        move    a0,n1
        nop

        ; EventSignal |= Event
        clr     a
        move    y:(r1+n1),a1
        or      x0,a
        move    a1,y:(r1+n1)
        
        ; Set adress mode back to linear

        rts

;*******************************************************************************
; ExecuteModules
; returns A non-zero if any modules have run
;*******************************************************************************
InitModules
ExecuteModules
		push SSH
        ; Start at the beginning of the module table
        move    #ModuleTable,r1

        ; For each entry in the module table
		clr a
        clr b
		move #MODULE_COUNT,a0
		push a0
TopOfExecuteModulesLoop
        ; Check if this module has any events to process
        move    #Module_EventWait,n1
        nop
        move    y:(r1+n1),x0
        move    #Module_EventSignal,n1
        nop
        move    y:(r1+n1),a

        jclr    #EVENT_INIT_BITPOS,a1,_check_timer
        
        bset    #EVENT_NONE_BITPOS,x0          ; force the module to run as well
        push    a2
        push    a1
        push    a0
        push    x0
        bset    #0,b1
        push    b1
        jsr     InitModule
        pop     b1
        pop     x0
        pop     a0
        pop     a1
        pop     a2

_check_timer
        jsset   #EVENT_TIMER_BITPOS,x0,ModuleCheckTimer

        btst    #EVENT_BACKGROUND_BITPOS,x0
        jcc     Proceed

        ;Check for Background Events
        move    #Module_Background,n1
        nop
        move    y:(r1+n1),r6
        push    x0
        push    r1
        push    b1
        jsr     (r6)
        pop     b1
        pop     r1
        pop     x0

Proceed
        and     x0,a                    ; Event = EventSignal & EventWait
        jeq     NextModule              ; If Event == 0 call next module

        ; reset the signal
        move    #Module_EventSignal,n1
        move    #>EVENT_NONE,x0
        move    x0,y:(r1+n1)

        ; Save variables
        push    m1
        push    r1
        push    a0
        push    a1
        push    a2
        push    b1
        ; Check if the module is already loaded
        move    #Module_ResourceNumber,n1
        clr     a
        move    y:(r1+n1),x0        ; Resource number of this module
        cmp     x0,a              ; If resource number is zero  
        jeq     CallModule        ; No need to load the module
        movem   p:CurrentModule,b   ; Resource number of currently loaded module
        cmp     x0,b
        jeq     CallModule        ; No need to load the module

        ; Setup arguments for calling SysLoadResource
        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
        move    #>MAX_MODULE_SIZE,y1            ; Size of the overlay area
        move    #>F_lc_u_bP_Module_overlay,y0            ; Address of the overlay area
        move    #TARGET_MEM_P,a1                ; Overlay area is in P memory

        move    x0,p:CurrentModule              ;put the new module into the receptical.  
        ; Load the module into the overlay area
        jsr     SysLoadResource

CallModule
        ; Restore variables
        pop     b1
        pop     a2
        pop     a1
        pop     a0
        pop     r1

        ; Pass the module a pointer to the it's message queue descriptor
        move    #Module_QueueDescriptor,n1
        move    y:<Const_ffffff,m0
        move    y:(r1+n1),r0

        ; Save variables
        push    r1

        ; Call the module's event processing routine
        ;    A1 = Event(s) that the module needs to process
        ;    R0 = Pointer to the module's message queue descriptor
        ;    M0 = Linear addressing
        move    #Module_ProcessEvent,n1
        nop

        move    y:(r1+n1),r2
        move  y:<Const_ffffff,m1

FCallModule_debug
        bset    #0,b1
        push    b1
        jsr     (r2)
        pop     b1
        ; The module returns the events it wants to wait for in A1

        ; Restore variables
        pop     r1
        pop     m1

        ; Save the EventWait that was returned by the module
        move    #Module_EventWait,n1
        nop
        move    a1,y:(r1+n1)
        bclr    #>EVENT_REPEAT_BITPOS,y:(r1+n1)            ;REPEAT isn't something we want saved in the table.
        
NextModule
        jclr    #>EVENT_REPEAT_BITPOS,a1,_DontRepeat
        jmp     TopOfExecuteModulesLoop
_DontRepeat
        ; Go to the next entry in the module table
        move    #>MODULE_ENTRY_SIZE,n1
        nop
        move    (r1)+n1
Loop
		clr a
		pop a0
		dec a
		push a0
		tst a
		jne TopOfExecuteModulesLoop
		pop a0
        ; Set adress mode back to linear
        move    b1,a                    ;move the 'has anybody been called' variable to a
		pop SSH
        move    y:<Const_ffffff,m1
        rts

;*******************************************************************************
;  ModuleSetTimer
;
;  Entry:
;   x0 = time in milliseconds
;   a1 = Module_EventWait 
;  Exit:
;   a1 = Module_EventWait with EVENT_TIMER bit set
;*******************************************************************************
ModuleSetTimer

 if (@DEF('FUNCLET'))           

    ;If using funclet, function will be executed via funclet kernel
    CallFunclet RSRC_FUNCLET_MODULESETTIMER

    org p,"SYSFUNCLET_ModuleSetTimer_P":
    
    dc      RSRC_FUNCLET_MODULESETTIMER           ; Funclet Resource number

 endif

    ; Get current time and add the wait count to it
    push    a1                                  ; save the Module_EventWait
    jsr     SysGetCurrentTime                   ; get the current time
    clr     b                                   ; put the low count in b
    move    a0,b
    add     x0,b                                ; add the time to wait
    jcc     _no_wrap
    move    y:<Const_000001,x0                              ; rollover in low count so
    add     x0,a                                ;   increment high count
_no_wrap
    move    b1,a0                               ; put the count back together
    
    ; search the ModuleTable for the current module
    ; TODO - we could get around this search by having the module init pass in
    ;   the pointer to the modules table entry & have each module save it away,
    ;   then pass it into this function.
    move    #Module_ResourceNumber,n0           ; current module resource num ptr
    movem   p:CurrentModule,b   ; Resource number of currently loaded module
    move    #ModuleTable,r0                     ; index into the module table
    move    y:<Const_ffffff,m0
    do      #MODULE_COUNT,_end_search
        move    y:(r0+n0),x1                    ; get the resource number
        cmp     x1,b                            ; is this the one?
        jne     _keep_looking
        enddo
        jmp     _stop_looking
_keep_looking
        move    #MODULE_ENTRY_SIZE,n0           ; point to the next table entry
        nop
        move    (r0)+n0
        move    #Module_ResourceNumber,n0       ; setup for 
_stop_looking
        nop
_end_search
    ; TODO - no error checking.  Problem if module not found.
    move    #Module_Timer_High,n0               ; write the high time value
    nop
    move    a1,y:(r0+n0)
    move    #Module_Timer_Low,n0                ; write the high time value
    nop
    move    a0,y:(r0+n0)                        ; write the low time value
    pop     a1                                  ; get the Module_EventWait
    bset    #EVENT_TIMER_BITPOS,a1              ; set the timer wait signal
    rts

 if (@DEF('FUNCLET'))

    ;If using funclet, switch counter back to original here
    org    p,"SYSFUN_Module_P":
    
 endif
                                   
;*******************************************************************************
;   ModuleCheckTimer
;
;  Entry:
;   a1 = Module_EventSignal 
;   r1 = ptr to current module in ModuleTable
;   n1 = ValueX
;  Exit:
;   a1 = Module_EventWait with EVENT_TIMER bit set
;   r1 = ptr to current module in ModuleTable
;   n1 = ValueX
;*******************************************************************************
ModuleCheckTimer

    push    n1
    push    a1
    
    jsr     SysGetCurrentTime           ; get the current time
    
    ; get the module target time
    move    #Module_Timer_High,n1
    clr     b
    move    y:(r1+n1),b1
    move    #Module_Timer_Low,n1
    nop
    move    y:(r1+n1),b0
    
    ; compare current time and target time
    cmp     a,b
    pop     a1
    pop     n1
    jle     _it_is_time
    rts
_it_is_time
    bset    #EVENT_TIMER_BITPOS,a1          ; signal the module
    rts

    endsec


