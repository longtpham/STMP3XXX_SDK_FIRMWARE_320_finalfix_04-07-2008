;******************************************************************************
; Copyright(C) SigmaTel, Inc. 2000-2003
;
; Executive routines to process the module table
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
    
    extern  SysCallModule
    extern  SysCallModuleInit
    extern  Fg_ModuleRegion

 if (@DEF('FUNCLET'))           
    extern  SysCallFunclet
 endif


;*******************************************************************************
;   Equates
;*******************************************************************************

; Word offsets within each ModuleTableEntry in the ModuleTable
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
; Private Function: InitModule
; Description: Calls C or asm Module_Init func ptr in the ModuleTableEntry.
;
; Inputs:   r1 = ptr to ModuleTableEntry
;
; Outputs:  Writes ModuleTableEntry.EventWait (wait to call module's process 
;           function next until the event requested by this module init routine's 
;           return value occurs)
;
; Notes:    Private routine called by ExecuteModules only.
;*******************************************************************************
InitModule
    ; Save variables
    push    m1
    push    r1

    ; Check if the module needs to be loaded
    move    #Module_ResourceNumber,n1
	nop
    move    y:(r1+n1),n6        ; Resource number of this module
	nop
    ; Call the module's initialization routine
    move    #Module_Init,n1
    nop
    move    y:(r1+n1),r6 ;Module_Init func ptr for this r1 ModuleTableEntry

    move	r1,r0 ;Copy pModuleTableEntry to r0 for C module init calls. (added at SDK 2.313.006)
    ; After the above r0 set was added, and all modules were made to return proper event 
    ; callback values, no module was allowed to run constantly as some had previously. 
    ; This allowed the main loop's DSP wait for irq instruction to execute which saved
    ; 5% of battery power while increasing the noise floor during full scale audio 
    ; by 5dB on the STMP3410. (meets 74dB spec in all cases)

   ; Call the module table init function for this module entry
   jsr     SysCallModuleInit
    ; Here, a1 must always have module's wait mask, for storage below into ModuleTableEntry. 
    ; Verified that all module init routines now return wait mask in a1.     

    ; Restore variables
    pop     r1
    pop     m1
    move    #Module_EventWait,n1
    nop     
    move    a1,y:(r1+n1) ;write in the new flags to the ModuleTableEntry's wait mask 
     ;[Defect fixed: Previous ver mistakenly wrote to Module_EventSignal since "+n1" was missing.
     ;This could have caused premature or late calls to module process functions.]
    rts
    

;*******************************************************************************
; API Function: SignalModule
; Description:  Set EventSignal bits given into ModuleTableEntry.EventSignal flag
;
; Input Parameters:  X1: Module Number
;                    X0: Event to be signaled for this module number to process
;
; Outputs: Sets bits in the ModuleTableEntry.EventSignal
;
; Notes:   ORs the given input parameter event flag bits into the given module 
;          number's ModuleTableEntry.EventSignal existing event flag. 
;*******************************************************************************

SignalModule
        ; Calculate the offset into the module table
        move    #>MODULE_ENTRY_SIZE,y0
        mpy     x1,y0,a                 ; Offset = module number * entry size
        asr     a                       ; ModuleTableEntry Offset in a0 after shift

        ; Point to the selected entry in the module table
        move    #>ModuleTable,r1
        move    a0,n1
        nop

        ; ModuleTableEntry.EventSignal |= Event Param
        clr     a
        move    y:(r1+n1),a1            ; A1 = ModuleTableEntry.EventSignal
        or      x0,a                    ; A |= (events to be signalled param)
        move    a1,y:(r1+n1)            ; ModuleTableEntry.EventSignal = A1
        
        ; Set address mode back to linear. (mode unchanged)

        rts

;*******************************************************************************
; Function:    ExecuteModules
;
; Description: Called by the Exec main loop program to execute all  
;              ready modules in a cooperative manner. Initializes modules as needed.
;
; Inputs:      Reads and writes the global ModuleTable Entries.  
;
; Returns:     Register A as non-zero if any modules have run
;
; Notes:       Alternate name: InitModules 
;              In this cooperative system, each module specifies when it desires
;              to run next on each iteration.
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
        move    y:(r1+n1),x0            ; ModuleTableEntry.EventWait flags in x0
        
        move    #Module_EventSignal,n1
        nop
        move    y:(r1+n1),a            ; ModuleTableEntry.EventSignal flags in A

        ; if (ModuleTableEntry.EventSignal.initBit) InitModule()
        jclr    #EVENT_INIT_BITPOS,a1,_check_timer 
        
        ; Set wait for none bit in our copy of CurrentModuleEntry.EventWait
        ; Forces module's process function to run at least once after InitModule.  
        bset    #EVENT_NONE_BITPOS,x0 
        
        push    a2
        push    a1
        push    a0
        push    x0
        bset    #0,b1
        push    b1
        jsr     InitModule ;  Writes ModuleTableEntry.EventWait 
        pop     b1         ; (Run next on x0 events desired by module's init func) 
        pop     x0
        pop     a0
        pop     a1
        pop     a2

_check_timer  
        ; if(ModuleTableEntry.EventWait.timerBit) Check module's wake-up timer
        jsset   #EVENT_TIMER_BITPOS,x0,ModuleCheckTimer 
        ; On exit from ModuleCheckTimer: 
        ;   a1 = Module's EventSignal with EVENT_TIMER bit set if it is wake-up time.
        ;   r1 = ptr to current ModuleTableEntry
        ;   x0   still has module's EventWait flags (unchanged)

        ;-----------------------------------------------  
        btst    #EVENT_BACKGROUND_BITPOS,x0
        jcc     Proceed

        ;Check for Background Events
        move    #Module_Background,n1
        nop
        move    y:(r1+n1),r6
        push    x0
        push    r1
        push    b1
        jsr     (r6)    ; May be a stub function that just returns.
        pop     b1
        pop     r1
        pop     x0
        ;-----------------------------------------------
Proceed                  ; Module's Event = module's EventWait & EventSignal
        and     x0,a       ; Only handle events module waited on AND that were signalled. 
        jeq     NextModule ; If (EventWait&EventSignal == 0) process Next ModuleTableEntry.
                           ; else{ set the ModuleTableEntry.EventSignal to EVENT_NONE and CallModule }
        move    #Module_EventSignal,n1
        move    #>EVENT_NONE,x0
        move    x0,y:(r1+n1)

        ; Save variables
        push    m1
;        push    r1
;        push    a0
;        push    a1
;        push    a2
;        push    b1
        ; Check if the module is already loaded

        ; Setup arguments for calling SysLoadResource
;        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
;        move    #>MAX_MODULE_SIZE,y1            ; Size of the overlay area
;        move    #>F_lc_u_bP_Module_overlay,y0            ; Address of the overlay area
;        move    #TARGET_MEM_P,a1                ; Overlay area is in P memory

        ;move    x0,p:CurrentModule              ;put the new module into the receptical.  
;		move    x0,y:(r6)        
        ; Load the module into the overlay area
;        jsr     SysLoadResource


CallModule
        ; Restore variables
;        pop     b1
;        pop     a2
;        pop     a1
;        pop     a0
;        pop     r1

        ; Pass the module a pointer to it's message queue descriptor
        move    #Module_QueueDescriptor,n1
        move    y:<Const_ffffff,m0
        move    y:(r1+n1),r0

        ; Save CurrentModuleEntry ptr
        push    r1

        ; Call the module's event processing routine
        ;    A1 = Event(s) that the module needs to process now
        ;    R0 = Pointer to the module's message queue descriptor
        ;    M0 = Linear addressing = -1
        move    #Module_ProcessEvent,n1
        nop
        move    y:(r1+n1),r6        ; ptr to module's event processing function
        move  y:<Const_ffffff,m6

        move    #Module_ResourceNumber,n1
        nop
        move    y:(r1+n1),n6        ; Resource number of this module

FCallModule_debug
        bset    #0,b1
        push    b1
;        jsr     (r2)
		jsr    SysCallModule
        pop     b1
        ;Module's event processing function returns events it wants to wait for in A1

        ; Restore variables
        pop     r1                  ; CurrentModuleEntry ptr
        pop     m1

        ;Save the EventWait value returned by the module, into its ModuleTableEntry.EventWait field
        move    #Module_EventWait,n1
        nop
        move    a1,y:(r1+n1)
        
        ;Ensure the REPEAT bit is inactive. ModuleTableEntry.EventWait.repeat=0 
        bclr    #>EVENT_REPEAT_BITPOS,y:(r1+n1)            
        
NextModule
        jclr    #>EVENT_REPEAT_BITPOS,a1,_DontRepeat
        jmp     TopOfExecuteModulesLoop
_DontRepeat
        ; ***** Go to the next ModuleTableEntry r1 in the ModuleTable *****
        move    #>MODULE_ENTRY_SIZE,n1
        nop
        move    (r1)+n1
Loop
		clr a
		pop a0                     ;Decrement MODULE_COUNT for this round robin cycle
		dec a
		push a0
		tst a
		jne TopOfExecuteModulesLoop
		pop a0
        ; No Modules left to execute. Return to main program aka super loop.
        move    b1,a               ;move the 'has any module been called' variable to A
	pop SSH
        move    y:<Const_ffffff,m1 ;Set the r1 adressing mode back to linear
        rts


;*******************************************************************************
; API Function: ModuleSetTimer
; Description:  Used by module process functions to set module's next runtime.
;
; Inputs:       x0 = time in milliseconds
;               a1 = Module_EventWait 
;
; Outputs:      a1 = Module_EventWait with EVENT_TIMER bit set
;
; Notes:       
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
;    movem   p:CurrentModule,b   ; Resource number of currently loaded module

    move    #Fg_ModuleRegion,r6
    nop
    move    y:(r6),b
   
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
;  Private Function: ModuleCheckTimer
;
;  Inputs:   a1 = Module_EventSignal 
;            r1 = ptr to current ModuleTableEntry in ModuleTable
;            n1 = ValueX
;
;  Outputs:  a1 = Module_EventSignal with EVENT_TIMER bit set if it's time to run
;            r1 = ptr to current ModuleTableEntry in ModuleTable
;            n1 = ValueX
;
;  Notes:    preserves x0
;*******************************************************************************
ModuleCheckTimer

    push    n1
    push    a1
    
    jsr     SysGetCurrentTime           ; get the current time. preserves x0
    
    ;Get the two word module target time from the ModuleTableEntry.Module_Timer in B
    move    #Module_Timer_High,n1
    clr     b
    move    y:(r1+n1),b1
    move    #Module_Timer_Low,n1
    nop
    move    y:(r1+n1),b0
    
    ; Compare current time in A to module's target run time in B
    cmp     a,b
    pop     a1                              ; module's previous event signal flag
    pop     n1
    jle     _it_is_time
    rts
_it_is_time
    bset    #EVENT_TIMER_BITPOS,a1 ;signal the module (event signal value with timer bit set)
    rts

    endsec


