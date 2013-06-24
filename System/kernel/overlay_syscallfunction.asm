;///////////////////////////////////////////////////////////////////////////////
;//
;// Filename: overlay_syscallfunction.asm
;// Description: This files holds the implementation of the SysCallFunction.
;//
;//
;// Copyright (c) SigmaTel, Inc. Unpublished
;//
;// SigmaTel, Inc.
;// Proprietary  Confidential
;//
;// This source code and the algorithms implemented therein constitute
;// confidential information and may comprise trade secrets of SigmaTel, Inc.
;// or its associates, and any use thereof is subject to the terms and
;// conditions of the Confidential Disclosure Agreement pursuant to which this
;// source code was originally received.
;//
;///////////////////////////////////////////////////////////////////////////////
    include "sysmacro.asm"
    include "kernel.inc"
    include "const.xref"

    GLOBAL  SysCallFunction
    GLOBAL  exSysCallFunction_v
    GLOBAL  exSysCallFunction_i
    GLOBAL  exSysCallFunction_ii
    GLOBAL  exSysCallFunction_iii
    GLOBAL  v_SysCallFunction_v
    GLOBAL  i_SysCallFunction_i
    GLOBAL  SysCallFunction_i
    GLOBAL  i_SysCallFunction_iipp
    GLOBAL  i_SysCallFunction_iiip

    EXTERN Fg_RegionTable
    EXTERN Kernel_GetRegion
    EXTERN Fg_pCurrentTask
    EXTERN Kernel_ContextSwitch
    EXTERN FKernel_SetTimeout
    EXTERN Overlay_IsTaskLoaded
    EXTERN SaveRegs
    EXTERN RestoreRegs
    EXTERN SysLoadResource
	EXTERN FOverlayManager
    
TARGET_MEM_P        equ     $100000
RSRC_TYPE_DATA                  equ     $000002    

   if @DEF('FUNCLET')
	org  p,".ShortAddressForFunclet1",near:$80
CallShortAddressToSaveOneWordForFunclet1
	jmp		SysCallFunclet

    org p,".ptextkernel_syscallfunclet":
    
SysCallFunclet2
SysCallFunclet
; if 1
    push    r0                      ;; Save r0 so I can use it
     move    ssh,r0     ;;Get addr of ResourceNumber and also removes JSR return address from the stack as desired
     move    p:(r0),n6              ;; gets ResourceNumber into n6 input param required by guts segment.
    pop     r0                      ;; Restore r0
    move    #>FFuncletEntryPoint,r6 ;; Address of funclet overlay region in module space
    jmp     SysCallFuntionEntrypointforFunclets
; else (original code)
    ;move    #>FFuncletEntryPoint,r6
    ;jmp     SysCallFuntionEntrypointforFunclets
; endif    
   org p,".ptextFuncletEntryPoint":
    dc  1
FFuncletEntryPoint
    
   endif;@DEF('FUNCLET')

    org p,".ptextkernel_syscallfunction":
   
SysCallFunction
exSysCallFunction_v
exSysCallFunction_i
exSysCallFunction_ii
exSysCallFunction_iii
v_SysCallFunction_v
i_SysCallFunction_i
SysCallFunction_i
i_SysCallFunction_iipp
i_SysCallFunction_iiip
SysCallFunction_L
    ;r0 is the resource
    ;r4 has the address of the function required
    ;the rest are parameters. (<x>6 and <x>7 are excluded)
    move    r0,n6           ;move the resource to n6
    move    r1,r0           ;reorder the pointers to match how they need to be 
    move    r2,r1           ;  when calling the native function
    move    r6,r2
    move    r4,r6           ;move the function pointer to r6
    move    r5,r4  

SysCallFuntionEntrypointforFunclets
    ; preserve a   
    push    a2
    push    a1              ;save off A
    push    a0

    ; if the resource to call into is zero then there is nothing to load.
    ; otherwise, check to see if a non-system task is calling into a resource.
    move    n6,a 
    tst     a
    jne     _check_for_calling_from_task
    
    ; the resource to call is zero, so just call it and get out.
    pop     a0
    pop     a1              ;restore A
    pop     a2
    push    ssh
    jsr     (r6)            ;call the function
    pop     ssh
    nop
    rts 
    
_check_for_calling_from_task
    ; if calling from a task (i.e., not from the kernel) then overlay.
    ; otherwise, already in the kernel, may not need an overlay.
    move    y:Fg_pCurrentTask,a
    tst     a
    jne     _SysCallFunctionGuts    

_InsideKernel
    ; already in the kernel.  
    ; determine if the target function needs to be paged in.
    pop     a0
    pop     a1                          ;restore A
    pop     a2
    push    r6                          ;push the function to call
    push    n6                          ;push the resource to call

    ; if the target resource to be called is the same as the default resource
    ; coded into the g_RegionTable for the specified region then there is no need
    ; to page in because the code is already present.  otherwise, the target
    ; code must be loaded into the overlay region before it can be called.
    jsr     SysCallFunctionSaveRegs     ;destroys no registers
    move    r6,r0                       ;r0 is function to call
    jsr     Kernel_GetRegion            ;find the region (returns rgn ptr in r6, rgn ix in a1)
    move    r7,r6                       ;copy new fn ptr from r7 to r6 ???
    move    #>-32,n6                    ;force immediate long address mode, -32 to n6
    move    y:(r0),a                    ;get the old resource (r0 is old fn ptr)
    move    y:(r6+n6),x0                ;get the new resource (r6 is new fn ptr ???, n6 is -32) 

    ; compare the old resource (in a) to the new resource (in x0), and if they are 
    ; the same then the target function can be called immediately.
    ; otherwise, it must be paged in.
    cmp     x0,a
    jne     _page_in_from_inside_kernel

    ; call the target function
    jsr     SysCallFunctionRestoreRegs  ;restore the regs
    push    SSH                         ;push the return address on the stack
    push    la                          ;push the loop address
    move    y:<Const_000000,la          ;put 0 into the loop address
    jsr     (r6)                        ;call the function
    pop     la                          ;get the old loop address
    pop     SSH                         ;pop off the return address
    move    y:(r7)-                     ;pop off the region pointer and resource
    move    y:(r7)-  
    rts

_page_in_from_inside_kernel
    ; page in the target function's overlay.

    move    a,y:(r6+n6)                 ;save the old resource on the stack (overwriting the new one!?)
    jsr     SysCallFunctionCommonLoad   ;load up the new region

    ; call the target function
    jsr     SysCallFunctionRestoreRegs  ;restore the regs
    push    SSH                         ;push the return address on the stack
    push    la                          ;push the loop address
    move    y:<Const_000000,la          ;put 0 into the loop address
    jsr     (r6)                        ;call the function
    pop     la                          ;get the old loop address
    pop     SSH                         ;pop off the return address

    ; determine if there is a need to restore the previous contents of the 
    ; overlaid memory.  if the default resource indicated in g_ResourceTable
    ; for the specified region was zero then there isn't any need to restore it;
    ; nothing was there to begin with.  just leave whatever was paged in in place.

    ; extract the old/default resource id from the stack.
    push    sr                          ;save the current status register
    jsr     SysCallFunctionSaveRegs     ;destroys no registers
    move    #>-34,n7                    ;find the old region resource
    nop     
    move    y:(r7+n7),r0
    jsr     Kernel_GetRegion            ; pull region's reource number out
    move    #>-33,n7
    nop
    move    y:(r7+n7),x0
    ; do not execute the next instruction *unless* the original resource is to be reloaded.
    ; in that case, do it *right* before calling SysCallFunctionCommonLoad.
    ; otherwise, the old resource number (possibly zero) is put back into the
    ; g_ResourceTable, forcing another load on the next SysCallFunction.
    ; thus, only half the potential speed up is realized.
    ;move    x0,y:(r0)                   ; put the old rsrc id back into g_ResourceTable.
    move    x0,a                        ;old resource id is in x0
            
    ; if the old/default resource id is zero then bypass the code reload operation.
    tst     a
    jeq     _bypass_the_reload          ; if it was zero dont reload the region.
    move    x0,y:(r0)                   ; put the old rsrc id back into g_ResourceTable.
    jsr     SysCallFunctionCommonLoad   ; otherwise page the code in.

_bypass_the_reload:
    jsr     SysCallFunctionRestoreRegs  ;restore the regs
    pop     sr                          ;pop off the SR returned by the function
    move    y:(r7)- ;pop off the region pointer and resource saved on the stack
    move    y:(r7)-  
    rts 
        
_SysCallFunctionGuts
    pop     a0
    pop     a1          ;restore A and balance the stack.
    pop     a2
    ;at this point:     r6 has the pointer to call, n6 the resource to call, and all other registers are as they should be when 
    ;                   the function is called

    push    r7                      ;make room for the old index
    push    r7                      ;and the old resource
    push    r6                      ;save off the function pointer
    jsr     SysCallFunctionSaveRegs ;destroys no registers
;Move the new resource into the right place in the table
    push    n6                      ;save off the required resource number
    move    r6,r0                   ;get the target function pointer
    jsr     Kernel_GetRegion        ;find which region it is trying to call to.  (region ptr comes out in r0, index in a1)
    move    y:Fg_pCurrentTask,r6    ;get the pointer to the current TaskEntry
    move    #>TASK_REGION_LIST_OFFSET,n6    ;index into it to the TASK_REGION_LIST member
    move    a1,n4                   ;put the region index into n4
    lua     (r6)+n6,r4              ;update r4 to point to the beginning of the TASK_REGION_LIST member
    pop     b                       ;get the new resource number
    move    #>34,n6                 ;offset the stack by -18 (to hit the old index)
    move    r7,r6
    move    y:(r4+n4),x0            ;get the old resource
    lua     (r6)-n6,r6  
    move    b1,y:(r4+n4)            ;store the new resouce, update r6 to point to the old address
    move    a,y:(r6)+               ;store the old index
    move    x0,y:(r6)               ;store the old resource
    
    move    y:Fg_pCurrentTask,r0        ;see if we need 
    jsr     Overlay_IsTaskLoaded
    tst     a
    jne     _callfunction
    
    ;jsr     FKernel_SetTimeout
    ;jsr     Kernel_ContextSwitch   

	jsr		FOverlayManager

_callfunction    
    jsr     SysCallFunctionRestoreRegs    
    pop     r6                     
    push    ssh
    push    la                              ; Save the loop address 
    move    m7,la                           ; m7 is guaranteed to be 0xffffffff (la must be set to ffff so we don't trigger a do loop)
    jsr     (r6)
    pop la                                  ;restore the loop address register
    pop     ssh
_returnfromfunction   
    push    sr
    jsr     SysCallFunctionSaveRegs ;save off the return registers             
    move    #>34,n6                 ;get the offset from the current location in the stack to the stored values
    move    r7,r6                   ;move the current stack pointer into r6
    move    y:Fg_pCurrentTask,r0    ;get a current task pointer
    lua     (r6)-n6,r6              ;make r6 point to the index on the stack
    move    #>TASK_REGION_LIST_OFFSET,n0
    move    y:(r6)+,n4              ;get the old index
    lua     (r0)+n0,r4
    move    y:(r6),x0               ;get the old resource number
    move    x0,y:(r4+n4)
    jsr     Overlay_IsTaskLoaded
    tst     a
    jne     _nofixupneeded
    
    ;jsr     FKernel_SetTimeout
    ;jsr     Kernel_ContextSwitch    ;context switch

	jsr		FOverlayManager

_nofixupneeded
    jsr     SysCallFunctionRestoreRegs
    pop     sr
    move    y:(r7)-
    move    y:(r7)-
    rts

SysCallFunctionSaveRegs
    move    r6,n7
    move    #>_saveregs,r6
    jmp     SaveRegs
_saveregs
    move    n7,r6    
    rts    

SysCallFunctionRestoreRegs
    move    r6,n7
    move    #>_restoreregs,r6
    jmp     RestoreRegs
_restoreregs
    move    n7,r6    
    rts    

SysCallFunctionCommonLoad
    move    x0,y:(r0)+      ;save the new resource in the region table
    move    y:(r0)+,a       ;get the start address
    move    y:(r0)+,b

    sub     a,b                 ;get the length
    move    y:<Const_000003,y0
    move    b1,y1
    mpy     y0,y1,b
    asr     b
    move    b0,y1               ;y1 now holds the length in bytes
    move    a,y0                ;y0 now holds the target pointer
    clr     b                       ;set the nested resource to the RS_ROOT_HANDLE
    move    #>TARGET_MEM_P,a1       ;set up the memory space
    move    #>RSRC_TYPE_DATA,x1     ;set up the type
    push    ssh                     ;call the function
    jsr     SysLoadResource
    pop     ssh
    ccerror cs
    rts
	if (@DEF('MTP_BUILD'))
 global SysAllLinear
SysAllLinear
    move    y:<Const_ffffff,m0
    move    y:<Const_ffffff,m1
    move    y:<Const_ffffff,m2
    move    y:<Const_ffffff,m3
    move    y:<Const_ffffff,m4
    move    y:<Const_ffffff,m5
    move    y:<Const_ffffff,m6
    move    y:<Const_ffffff,m7
    rts
    endif
