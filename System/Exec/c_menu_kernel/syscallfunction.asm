    include "sysmacro.asm"
    include "msgequ.inc"

    EXTERN ContextSwitch
    EXTERN MenuResourcePtr
    EXTERN EVENT_NONE


    Section SYSCALLFUNCTION

    GLOBAL SysCallFunction
    GLOBAL EnterFunction
    GLOBAL ReturnHandler
    GLOBAL SysPostMessageA

	org p,"MenuManager_p":


EnterFunction
;This function is the 'wrapper' function when calling into a bank.  It pops the parameters off the stack, and calls the function on the stack
;when that function finally returns, it updates the resource back to the bank that called us, pushes in _ReturnHandler to make sure the original
;calling function gets the return value its expecting.
    pop R0                  ;third param (ptr) (was in R4 of SysCallFunction, needs to be in R0 of called function)
    pop B1                  ;second param (int)(was in X0 of SysCallFunction, needs to be in B1 of called function)
    pop A1                  ;first param (int) (was in B1 of SysCallFunction, needs to be in A1 of called function)
    pop r6                  ;pop off the function we're calling
    nop
    jsr (r6)                ;call the function.
    pop b1                  ;get the resource of the function to return to
    move b1,y:MenuResourcePtr ;fixup the resource
    push A1                 ;push the return value on the stack
    move #ReturnHandler,r0 ;push our 'return handler'
    push r0
    move #>(EVENT_NONE|EVENT_REPEAT),a1
    jmp ContextSwitch       ;this should send us back to the exec!

ReturnHandler
    pop A1                  ;get the return value
    pop SSH
    nop
    rts                     ;this should return to the user task.

SysCallFunction
    ;int _asmfunc SysCallFunction(unsigned int RESOURCE,int (int,int,int*),  int, int, int *);
    ;from C,the resource is in A1
    ;       ptr to entry point is in R0
    ;       first param is in B1
    ;       second param is in X
    ;       ptr param in in R4
;This function sets up the module table to put the new resource number in, push the parameters onto the software stack, then return to
;the executive.
    push SSH                        ;push the calling function onto the software stack
    move y:MenuResourcePtr,a0
    push a0                         ;push the calling function resource number onto the software stack
    move A1,y:MenuResourcePtr       ;fixup the resource
    push R0                         ;push the function to call address onto the stack
    push B1                         ;push the first parameter   to the function (int)
    push X0                         ;push the second parameter  to the function (int)
    push R4                         ;push the third parameter   to the function (ptr)
    move #EnterFunction,r0
    push r0                         ;push EnterFunction onto the stack
    move #>(EVENT_NONE|EVENT_REPEAT),a1
    jmp ContextSwitch

    ENDSEC 