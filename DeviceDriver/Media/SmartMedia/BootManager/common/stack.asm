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

;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

    ;Functions 
    global  SaveSystemStack
    global  RestoreSystemStack
    
;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////

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
;>  Name:           SaveSystemStack
;
;   Type:           Function
;
;   Description:    Saves the system hardware stack to the data stack
;
;   Inputs:         r5 = Pointer to local save area for current hardware SP
;                   r6 = Pointer to return address
;                   r7 = Current data stack pointer
;
;   Outputs:        none
;                   
;   Notes:          none
;<
;///////////////////////////////////////////////////////////////////////////////
SaveSystemStack
    move    sp,a
    move    a,x:(r5)+                   ; save sp
    tst     a   #>1,x0    
    jeq     _sp_is_cleared
_save_system_stack
    movec   ssl,x:(r7)+
    movec   ssh,x:(r7)+
    sub     x0,a                        ; decrement stack count
    jne     _save_system_stack
_sp_is_cleared
    jmp     (r6)

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           RestoreSystemStack
;
;   Type:           Function
;
;   Description:    Restores the system hardware stack from the data stack.
;
;   Inputs:         r5 = Pointer to local save area for old hardware SP
;                   r6 = Pointer to return address
;                   r7 = Current data stack pointer
;
;   Outputs:        none
;                   
;   Notes:          none
;<
;///////////////////////////////////////////////////////////////////////////////
RestoreSystemStack
    move    x:(r5),a                    ; original, saved sp
    tst     a   #>1,x0                  ; use for counter
    jeq     _sp_is_restored             ; if sp==0, done
_restore_system_stack
    movec   x:-(r7),ssl
    movec   x:-(r7),ssh
    sub     x0,a                        ; decrement stack count
    jne     _restore_system_stack
_sp_is_restored
    jmp     (r6)

    endsec
