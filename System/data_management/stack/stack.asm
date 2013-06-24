;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2001
;
;  File        : stack.asm
;  Description : Stack routines
;///////////////////////////////////////////////////////////////////////////////

    section SYSFUN_Stack

;///////////////////////////////////////////////////////////////////////////////
;   Macros
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "sysmacro.asm"
    list

;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

    ;Functions 
    global  SaveRegs
    global  RestoreRegs
    global  SaveSystemStack
    global  RestoreSystemStack
    
    global  NUMBER_REG_SAVED

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "const.xref"
    list

;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////

NUMBER_REG_SAVED    equ EndSaveRegs-SaveRegs
        
;///////////////////////////////////////////////////////////////////////////////
;   X Memory
;///////////////////////////////////////////////////////////////////////////////
        
;///////////////////////////////////////////////////////////////////////////////
;   Y Memory
;///////////////////////////////////////////////////////////////////////////////
        
;///////////////////////////////////////////////////////////////////////////////
;   P Memory
;///////////////////////////////////////////////////////////////////////////////

    org     p,"SYSFUN_Stack_P":

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SaveRegs
;
;   Type:           Function
;
;   Description:    Saves all ALU and address registers to the data stack,
;                   except for r6, r7, and m7.
;
;   Inputs:         r6 = Return address
;                   The address at r6 is used as the destination to jmp back
;                   to after saving registers is complete.
;
;   Outputs:        none
;                   
;   Notes:          Does not save r6, r7, or m7
;<
;///////////////////////////////////////////////////////////////////////////////
SaveRegs
    push    a2
    push    a1
    push    a0
    push    b2
    push    b1
    push    b0
    push    x1
    push    x0
    push    y1
    push    y0
    push    r0
    push    r1
    push    r2
    push    r3
    push    r4
    push    r5
    push    m0
    push    m1
    push    m2
    push    m3
    push    m4
    push    m5
    push    m6
    push    n0
    push    n1
    push    n2
    push    n3
    push    n4
    push    n5
    push    n6
    push    n7
EndSaveRegs    
    jmp     (r6)

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           RestoreRegs
;
;   Type:           Function
;
;   Description:    Restores all ALU and address registers from the data stack,
;                   except for r6, r7, and m7.
;
;   Inputs:         r6 = Return address
;                   The address at r6 is used as the destination to jmp back
;                   to after restoring registers is complete.
;
;   Outputs:        none
;                   
;   Notes:          Does not restore r6, r7, or m7
;<
;///////////////////////////////////////////////////////////////////////////////
RestoreRegs
    pop     n7
    pop     n6
    pop     n5
    pop     n4
    pop     n3
    pop     n2
    pop     n1
    pop     n0
    pop     m6
    pop     m5
    pop     m4
    pop     m3
    pop     m2
    pop     m1
    pop     m0
    pop     r5
    pop     r4
    pop     r3
    pop     r2
    pop     r1
    pop     r0
    pop     y0
    pop     y1
    pop     x0
    pop     x1
    pop     b0
    pop     b1
    pop     b2
    pop     a0
    pop     a1
    pop     a2
    jmp     (r6)

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SaveSystemStack
;
;   Type:           Function
;
;   Description:    Saves the system hardware stack to the data stack
;
;   Inputs:         
;                   r6 = Pointer to return address
;                   
;
;   Outputs:        none
;                   
;   Notes:          none
;<
;///////////////////////////////////////////////////////////////////////////////
SaveSystemStack

    move    sp,a
    move    a,n7
    tst     a    y:<Const_000001,x0
    jeq     _sp_is_cleared

_save_system_stack
    movec   ssl,y:(r7)+
    movec   ssh,y:(r7)+
    sub     x0,a                        ; decrement stack count
    jne     _save_system_stack
_sp_is_cleared
    push    n7
  if (@DEF('DEBUG'))
    push    r7
  endif
    jmp     (r6)

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           RestoreSystemStack
;
;   Type:           Function
;
;   Description:    Restores the system hardware stack from the data stack.
;
;   Inputs:         
;                   r6 = Pointer to return address
;                   
;
;   Outputs:        none
;                   
;   Notes:          none
;<
;///////////////////////////////////////////////////////////////////////////////
RestoreSystemStack
  if (@DEF('DEBUG'))
    pop     x0
    move    r7,a
    cmp     x0,a
    ccerror ne
  endif
    pop     a; original, saved sp
    tst     a    y:<Const_000001,x0                 ; use for counter
    jeq     _sp_is_restored             ; if sp==0, done
_restore_system_stack
    pop     ssh
    pop     ssl
    sub     x0,a                        ; decrement stack count
    jne     _restore_system_stack
_sp_is_restored
    jmp     (r6)

    endsec
