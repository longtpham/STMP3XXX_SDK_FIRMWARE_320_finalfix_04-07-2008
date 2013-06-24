;SDK2400 SysCallFunction
    include "sysmacro.asm"
    include "const.xref"

    include "sysequ.inc"
    include "sysresources.inc"

    OPT OP

    extern  Fg_RegionTable
    extern  SysLoadResource
    extern  SaveRegs
    extern  RestoreRegs

    global FFuncletEntryPoint
	global FFuncletEntryPoint2
    global  SysCallFunction2400
	global TempSaveSR1,TempSaveSR2

    org y,".ydataSDK2400_syscallfunction":

Fg_iSysCallFunctionCounter  dc  0
Fg_iSysCallFunctionTmpRegionPointer ds 1
Fg_iSysCallFunctionTmpOldResource ds 1
Fg_iSysCallFunctionTmpRegionSize ds 1
TempSaveSR1	dc	0
TempSaveSR2	dc	0
	org  p,".ShortAddressForFunclet1",near:$80
CallShortAddressToSaveOneWordForFunclet1
	jmp		SysCallFunclet
	org  p,".ShortAddressForFunclet2",near:$82
CallShortAddressToSaveOneWordForFunclet2
	jmp		SysCallFunclet2
    org p,".ptextSDK2400_syscallfunction":

	if (@DEF('STMP_BUILD_PLAYER'))
SysCallFunclet2
;if 1
    push    r0                      ;; Save r0 so I can use it
     move    ssh,r0     ;;Get addr of ResourceNumber and also removes JSR return address from the stack as desired
     move    p:(r0),n6              ;; gets ResourceNumber into n6 input param required by guts segment.
    pop     r0                      ;; Restore r0
    move    #>FFuncletEntryPoint2,r6 ;; Address of funclet overlay region in module space
    jmp     FSysCallFunctionGuts
;else (original approach had no short intermediate jump)
    ;move    #>FFuncletEntryPoint2,r6
    ;jmp     FSysCallFunctionGuts
;endif
	else
SysCallFunclet2
	endif ;The next statement should *always* be SysCallfunclet
SysCallFunclet
;if 1
    push    r0                      ;; Save r0 so I can use it
     move    ssh,r0     ;;Get addr of ResourceNumber and also removes JSR return address from the stack as desired
     move    p:(r0),n6              ;; gets ResourceNumber into n6 input param required by guts segment.
    pop     r0                      ;; Restore r0
    move    #>FFuncletEntryPoint,r6 ;; Address of funclet overlay region in module space
    jmp     FSysCallFunctionGuts
;else (original approach had no short intermediate jump)
    ;move    #>FFuncletEntryPoint,r6
    ;jmp     FSysCallFunctionGuts
;endif

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;  Calling convention for SysCallFunction2400():
;
;   SysCallFunction2400((WORD *) iRscNum, (WORD*)(<args>) pFun, <args>)
;
;       R0  - Resource Number (cast to a WORD pointer, to be passed through R0)
;       R4  - Entry point of function to be called
;
;       A,B,X0,Y0, etc. are the same as if you'd called pFun(), directly, 
;         except pointer arguments start with R5, instead of R0.
;         SysCallFunction2400 rearranges the pointer arguments to the right
;         locations (i.e. R5 -> R0) before calling pFun().
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
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
SysCallFunction_iiip
SysCallFunction2400
    move    r0,n6           ;move the resource to n6
    move    r1,r0           ;reorder the pointers to match how they need to be when calling the native function
    move    r2,r1
    move    r6,r2
    move    r4,r6           ;move the function pointer to r6
    move    r5,r4 

SysCallModuleInit
SysCallModule	 
    jsr     SysAllLinear
    push    a2
    push    a1              ;save off A
    push    a0
    move    n6,a            ;see if we need to load anything
    tst     a
    pop     a0
    pop     a1              ;restore A
    pop     a2
    jne     FSysCallFunctionGuts    ;if the resource number is non-zero, we need to load
    push    SSH             ;else, call the function directly (and short circuit all of the code below)
    jsr     (r6)
    pop     SSH
    nop
    rts 
    
FSysCallFunctionGuts
    push    SSH
    jsr     SysAllLinear
    move    r6,n7
    push    r6
    move    #>_saveregs1,r6
    jmp     SaveRegs
_saveregs1
    move    n7,r6
    jsr FPushCodeBank
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	 push x0
	 push sr
	 bset #8,sr
	OPT   NOOPNOP
	 nop
	 nop
	 nop
	OPT   OPNOP
	 pop x0
	 move x0,y:TempSaveSR1
	 pop  x0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    move    y1,y:Fg_iSysCallFunctionTmpRegionSize
    move    r6,y:Fg_iSysCallFunctionTmpRegionPointer
    move    n6,y:Fg_iSysCallFunctionTmpOldResource
    move    #>_restoreregs1,r6
    jmp     RestoreRegs
_restoreregs1
    move    y:Fg_iSysCallFunctionTmpRegionSize,n6
    pop     r6                              ; get the function pointer
    push    n6                              ;save off the size
    move    y:Fg_iSysCallFunctionTmpOldResource,n6
    move    y:Fg_iSysCallFunctionTmpRegionPointer,n7
    push    n6                              ; save the old resource
    push    n7                              ; save the region we were working on
                                            ; must set the LA to be an address that we'll never hit--so that we don't
                                            ; automatically do an interation of the DO loop when the wrong code is loaded
    push    la                              ; Save the loop address 
    move    m7,la                           ; m7 is guaranteed to be 0xffffffff

;;;;;;;;;;;;;;;;;;;;;;;;;;;
	move	y:TempSaveSR1,sr
	nop
	nop
	nop
	nop
;;;;;;;;;;;;;;;;;;;;;;;;;;;

    jsr (r6)                                ;call our function

    pop la
    pop n7      ;pop out the previous region information
    pop n6      ;pop out the old resource number
    pop r6
    push    sr
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	 push x0
	 push sr
	 bset #8,sr
	OPT   NOOPNOP
	 nop
	 nop
	 nop
	OPT   OPNOP
	 pop x0
	 move x0,y:TempSaveSR2
	 pop  x0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    move    r6,y:Fg_iSysCallFunctionTmpRegionSize
    move    #>_saveregs2,r6
    jmp     SaveRegs
_saveregs2
    move    n7,r6
    move    y:Fg_iSysCallFunctionTmpRegionSize,y1
;;;;;;;;;;;;;;;;;;;;;;;;;;;
	move	y:TempSaveSR2,sr
	nop
	nop
	nop
	nop
;;;;;;;;;;;;;;;;;;;;;;;;;;;

    jsr FPopCodeBank
    move    #>_restoreregs2,r6
    jmp     RestoreRegs
_restoreregs2
    pop     sr
    pop SSH
    nop
    rts

;input:
;r6 contains the new function pointer
;n6 contains the new resource number
;output:
;n6 old resource number
;r6 old region descriptor
;y1 size in bytes of region
FPushCodeBank
    move    r6,y0
    move    #Fg_RegionTable,r6
    nop
_topofloop
    move    y:(r6)+,a
    tst     a   a,r3
    ccerror eq              ;if A is null, that means the function pointer isn't in any region at all
    move    y:(r3)+,x0      ;get the current resource
    move    y:(r3)+,a       ;get beginning
    move    y:(r3)-,b       ;get end (and back up by one)
    cmp     y0,a            ;compare the function address to the bottom of the region
    jgt     _topofloop      ;loop to top if its not in a region
    cmp     y0,b            ;compare the function address to the top of the region
    jlt     _topofloop      ;loop to top if its not in the region
    jeq     _topofloop      ;loop to top if its not in the region

    move    y:(r3)-         ;(back up by one)
    ;if we make it here, then the pointer is within the current table, 
    ;       a contains the lowest address of the region
    ;       b contains the highest address of the region
    ;       x0 contains the old resource loaded in the region
    ;       r3 contains a pointer to the region table
    ;       n6 contains the new resource number to load
    ;       y0 contains the function pointer (don't need)
;   Sysload resource
;   Inputs:         x0 = Resource Number
;                   x1 = Resource Type
;                   y0 = Target Pointer
;                   y1 = Target available space (in Bytes)
;                   a1 = Space target << 16
    push    x0              ;save off the current/old resource in the region
    push    r3              ;save off the pointer to the region entry

    sub     a,b             ;get the length
    move    y:<Const_000003,y0
    move    b1,y1
    mpy     y0,y1,b
    asr     b
    move    b0,y1

    move    a1,y0           ;put the lowest address in the region into y0
    move    n6,a            ;put the resource number into a for comparison to the old in x0

    cmp     x0,a            ;compare the old resource to the new one needed
    move    a1,x0           ;put the resource into x0
    jeq     _noNeedToLoad   ;don't load if we don't need to
    push    y1
    push    x0              ;save off the new resource number
    push    ssh
    jsr     SysCallFunctionCommonLoad
    pop     ssh
    pop     x0              ;pop the new resource number into x0
    pop     y1
_noNeedToLoad
    ;increment the SysCallFunction pointer
;    clr     a
;    move    y:Fg_iSysCallFunctionCounter,a0
;    inc     a
;    move    a0,y:Fg_iSysCallFunctionCounter
    
    pop     r6              ;pop off the region
    move    x0,y:(r6)   ;put the newly loaded resource number into the region table 
    clr     a
	move	#>3,n6
    move    y:(r6+n6),a0
    inc     a
    move    a0,y:(r6+n6) ;increment the SysCallFunctionCounter for this region table 
    pop     n6              ; pop the old resource number
_exit
    rts




;inputs:
;n6 has the resource number to pop to
;r6 has the pointer to the region descriptor
;outputs:
;none

FPopCodeBank
    ;decrement the SysCallFunction pointer
;    clr     a
;    move    y:Fg_iSysCallFunctionCounter,a0
;    dec     a
;    move    a0,y:Fg_iSysCallFunctionCounter
;    tst     a
   	push    n6
    clr     a
	move	#>3,n6
    move    y:(r6+n6),a0
    dec     a
    move    a0,y:(r6+n6)
	pop     n6
    tst     a
    jeq     _noNeedToLoad       ;if the counter is zero, lets not reload


    move    y:(r6)+,a           ;current resource number
    move    y:(r6)-,y0          ;lower memory bound
    move    n6,x0               ;
    cmp     x0,a
    jeq     _noNeedToLoad       ;if the resource is already loaded, lets not reload
    move    x0,y:(r6)
;   Sysload resource
;   Inputs:         x0 = Resource Number
;                   x1 = Resource Type
;                   y0 = Target Pointer
;                   y1 = Target available space (in Bytes)
;                   a1 = Space target << 16

    ;a is already set
    ;r0 is already set
;    move    y:<Const_7fffff,y1      ;max size = fffffff

    push    ssh
    jsr     SysCallFunctionCommonLoad
    pop     ssh
    nop
_noNeedToLoad
    rts

SysCallFunctionCommonLoad
    clr     b                       ;set the nested resource to the RS_ROOT_HANDLE
    move    #>TARGET_MEM_P,a1       ;set up the memory space
    move    #>RSRC_TYPE_DATA,x1     ;set up the type
	move    y:<Const_7fffff,y1      ;max size = fffffff
    push    ssh                     ;call the function
	push    x0
    jsr     SysLoadResource
	pop     x0
    pop     ssh
    ccerror cs
    rts


    org p,".ptextFuncletEntryPoint":
    dc  1
FFuncletEntryPoint
    org p,".ptextFuncletEntryPoint2":
    dc  1
FFuncletEntryPoint2
