;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2001
;
;  File        : sysfunclet.asm
;  Description : Funclet kernel.
;///////////////////////////////////////////////////////////////////////////////

    page    255,255,0

    section SYSFUN_SysFunclet

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
    include "sysresources.inc"
    include "sysequ.inc"
    include "project.inc"
    list

;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////
    
    ;Functions  
    global  SysCallFunclet

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "const.xref"
    list
    
    extern  SaveRegs
    extern  SysLoadResource
    extern  RestoreRegs
    extern  F_lc_u_bP_space_funclet
    extern  F_lc_u_eP_space_funclet 
    extern  GetIrqL
    extern  NUMBER_REG_SAVED

;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////

   if (!@def(MAX_NESTED_FUNCLET))
MAX_NESTED_FUNCLET          equ     6       ;Maximum number of nested funclets
   endif
MAX_HARDWARE_STACK_LEVEL    equ     11      ;SysCallFunclet generates an error if
                                            ;the stack hardware level is greater or equal
                                            ;this value when called.

    ;Defines the starting address and size of the funclet space
FUNCLET_ADDRESS             equ     F_lc_u_bP_space_funclet
FUNCLET_SIZE                equ     (F_lc_u_eP_space_funclet-F_lc_u_bP_space_funclet)*3

;///////////////////////////////////////////////////////////////////////////////
;   X Memory
;///////////////////////////////////////////////////////////////////////////////
        
;///////////////////////////////////////////////////////////////////////////////
;   Y Memory
;///////////////////////////////////////////////////////////////////////////////

    org     y,"SYSFUN_SysFunclet_Y":
  
NestedFuncletLevel  dc      0                       ;Keeps track of the nested level
FuncletStack        ds      (MAX_NESTED_FUNCLET*2)  ;Size of Funclet Stack. SysCallFunclet saves into the stack
                                                    ;the funclet resource number and the return address
ptrFuncletStack     dc      FuncletStack-1          ;Pointer to the funclet stack. The pointer is pre incremented 
                                                    ;during a push.                                                    
;///////////////////////////////////////////////////////////////////////////////
;   P Memory
;///////////////////////////////////////////////////////////////////////////////

    org     p,"SYSFUN_SysFunclet_P":

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysCallFunclet
;
;   Type:           Function
;
;   Description:    Loads an applet in applet space and calls the applet loaded.
;
;   Inputs:         
;                   Resource Number for the applet to load in the software stack.
;
;   Outputs:        none
;                   
;   Notes:          
;                   r6 can not be used to pass a parameter to the funclet
;<
;///////////////////////////////////////////////////////////////////////////////
SysCallFunclet
    push     r6
    movec    ssh,y:(r7)+                    ;Get function return address from hardware stack
                                            ;and save it locally. This reduces the amount 
                                            ;of hardware stack consumed by the funclet mechanism
                                            
    move    #>_RetAdd1,r6                   ;Save all processor registers
    jmp     SaveRegs

_RetAdd1
    move    sp,a                            ;Check hardware stack level 
    move    #>MAX_HARDWARE_STACK_LEVEL,x0                              
    cmp     x0,a
    jlt     _StackLevelOK
    
    error

_StackLevelOK
                                            ;Check if call under interruption    
    jsr     GetIrqL 
    ; Returns SR in x0 with all bits cleared except 9:8 which is interrupt mask aka min IPL. 
    ; Interrupt Priority Level mask in x0 is min allowed interrupt level. Require IPL 0 or 3.
    move    #>$000300,a                     
    cmp     x0,a                            ; if( SR.MinInterruptLevel == IPL_ALLOW_IRQ_L3 ) OK 
    jeq     _ValidIrqLevel
    move    x0,a
    tst     a                               ; else if( SR.MinInterruptLevel == IPL_ALLOW_IRQ_ANY ) OK
    jeq     _ValidIrqLevel
                                            ; else     
    error                                   ;  Interrupt min level was not 0 or 3, halt if DEBUG, else reset DSP
    
_ValidIrqLevel    
    clr     a                               ;Check funclet max nested level 
    move    y:NestedFuncletLevel,a0         ;and increment nested level
    inc     a   
    move    #>MAX_NESTED_FUNCLET,x0   
    move    a0,a
    cmp     x0,a
    jle     _ValidLevel
    
    error
    
_ValidLevel
    ;Save funclet resource number in funclet stack
    ;To be used for return in case funclet nested
    move    y:<Const_ffffff,m0
    move    y:ptrFuncletStack,r0            ;Get Funclet Stack pointer into r0
    move    a1,y:NestedFuncletLevel         ;Saved level of nesting
    lua     (r0)+,r0                        ;Stack is pre incremented prior a push

    move    #>(NUMBER_REG_SAVED+1),n7       ;Get the return address from software stack
    move    y:<Const_ffffff,m1
    lua     (r7)-n7,r1
    nop
    move    y:(r1),a0
    
    move    #>(NUMBER_REG_SAVED+2),n7
    move    a0,y:(r0)+                      ;Push return address into Funclet stack

    lua     (r7)-n7,r1                      ;Get funclet resource number from software stack
    nop
    move    y:(r1),x0

    move    x0,y:(r0)                       ;Push funclet resource number into Funclet Stack
    move    r0,y:ptrFuncletStack
    
    ;Check for funclet already loaded
    clr     a
    movem   p:FUNCLET_ADDRESS,a1
    cmp     x0,a
    jeq     _FuncletLoadedSuccess    

    ;Load funclet in funclet space
                                            ;x0 is funclet resource number
    move    #>RSRC_TYPE_DATA,x1             ;Resource type is data
    move    #>FUNCLET_SIZE,y1               ;Size of the funclet area
    move    #>FUNCLET_ADDRESS,y0            ;Address of the funclet area
    move    #TARGET_MEM_P,a1                ;Funclet area is in P memory
    jsr     SysLoadResource
    jcc     _FuncletLoadedSuccess
    
    error
    
_FuncletLoadedSuccess
    ;Restore the registers (arguments to funclet)
    move    #>_RetAdd2,r6
    jmp     RestoreRegs
    
_RetAdd2
    pop     r6                              ;Pushes the funclet resource number and return address
    pop     r6                              ;from the software stack to adjust stack pointer

    ; Caller function DO-loops must be disabled.
    push    la                              ; Save the loop address
    move    y:Const_ffffff,r6               ; Set it to the end of memory
    move    r6,la

    ;Call the funclet
    move    #>(FUNCLET_ADDRESS+1),r6
    nop

    jsr     (r6)
    
    
    ;Returning from a funclet executes from here

    ;we save the SR in software stack in case used to return a value

    push    SR
    
    move    #>_RetAdd3,r6                   ;Save all processor registers
    jmp     SaveRegs

_RetAdd3

    ;Check if nested funclet and adjust Funclet Nested Counter
    clr     a
    move    y:NestedFuncletLevel,a0
    dec     a
    move    a0,y:NestedFuncletLevel
    
    push    sr                              ;Save Nested funclet result
    
    ;Unstack funclet stack
    move    y:<Const_ffffff,m0
    move    y:ptrFuncletStack,r0
    nop
    move    y:-(r0),ssh                     ;Restores return address
    move    y:-(r0),x0                      ;Restores return funclet
    move    r0,y:ptrFuncletStack
    
    pop     sr
    jeq     _FuncletReturnLoadedSuccess

    ;Funclet was nested, load return funclet in funclet space
    move    #>RSRC_TYPE_DATA,x1             ;Resource type is data
    move    #>FUNCLET_SIZE,y1               ;Size of the funclet area
    move    #>FUNCLET_ADDRESS,y0            ;Address of the funclet area
    move    #TARGET_MEM_P,a1                ;Funclet area is in P memory
    jsr     SysLoadResource
    jcc     _FuncletReturnLoadedSuccess
    
    error
    
_FuncletReturnLoadedSuccess
    ;Restore all registers
    move    #>_RetAdd4,r6                   ;Save all processor registers
    jmp     RestoreRegs

_RetAdd4
    pop     sr
    pop     la
    
    rts    
    endsec

