;******************************************************************************
; Copyright(C) SigmaTel, Inc. 2000/2001
;
; STMP Foreground Module Execution
;
;******************************************************************************
; Module Files:
;
;******************************************************************************

;*******************************************************************************
;  Include files
;*******************************************************************************
		include "hwequ.inc"
		include "dacequ.inc"
		include "stack.xref"
		include "stackmem.xref"
		include "sysequ.inc"
	    include "sysmacro.asm"
	    include "stack.xref"
	    include "stackmem.xref"
	    include "const.xref"
		include "fgndmodules.inc"
		include "fgndmodules.xref"

        page    132,60,0,0,0

;*******************************************************************************
;  Extern definitions
;*******************************************************************************
    extern FgndLoopCnt
    extern SysAllLinear
    extern TotalSamplestoProcess
    extern DacSamplestoProcess
;*******************************************************************************
;  Extern references
;*******************************************************************************
    global FDisablePluginModule
    global FEnablePluginModule

;*******************************************************************************
;  Code start
;*******************************************************************************
	org p,".FgndExec":
;;***********************************************************************
;	Function:
;		SetIOPtrs
;	Description:
;	   	Saves r3 and r4.
;  	Parameters passed: 
;		r0: Pointer to current module table entry.
;  	Return Values:
;		r3,m3,n3,r4,m4,n4: Pointers,modulo,index register settings for source 
;			and sink buffers
;	Registers Destroyed:	   
;		r1,m1,n1
;;***********************************************************************
	; Passed : Ptr to Module Control Block Entry
	; Return values : r3,m3: Source buffer settings
	; r4,m4 : Sink buffer settings.
GetIOPtrs
	move	#>EffBufIdenticalOffset,n0
	nop
	move	y:(r0+n0),A						; Get buffers identical flag	
	move	#>EffSrcDescrOffset,n0
	move	y:<Const_ffffff,m1
	move	y:(r0+n0),r1					; Pointer to source Descriptor
    move    #>BD_TailPtr,n1
	move	#>EffSinkDescrOffset,n0
	move    y:(r1+n1),r3					; Source buffer tail ptr
	move	#>BD_BufferModulo,n1
	move	y:<Const_000002,n3				; Stereo is interleaved
	move    y:(r1+n1),m3					; Source buffer modulo
	tst		A	y:(r0+n0),r1  				; Pointer to Sink Descriptor
	jne		_BufsIdentical
	move	y:(r1+n1),m4					; Sink Buffer modulo
	move	#>BD_HdPtr,n1
	move	y:<Const_000002,n4
	move	y:(r1+n1),r4					; Sink Buffer ptr
	rts

	; Buffers are identical. So set the Source tail ptr as Sink head ptr
_BufsIdentical
	move	r3,r4
	move	n3,n4
	move	m3,m4
	rts	

;;***********************************************************************
;	Function:
;		SetIOPtrs
;	Description:
;	   	Saves r3 and r4.
;  	Parameters passed: 
;		r0: Pointer to current module table entry.
;  	Return Values:
;		r0 : Pointer to next module table entry.
;	Registers Destroyed:	   
;		r1,m1,n1
;;***********************************************************************
SetIOPtrs
	move	#>EffBufIdenticalOffset,n0
	move	y:<Const_ffffff,m1
	move	y:(r0+n0),A						; Get buffers identical flag	
	tst		A	#>EffSrcDescrOffset,n0
	jne		_PrepareForNextModule			; Buffers identical? Then don't touch anything.
	move	y:(r0+n0),r1					; Pointer to source Descriptor
    move    #>BD_TailPtr,n1
	move	#>EffSinkDescrOffset,n0
	move    r3,y:(r1+n1)					; Source buffer tail ptr
	move	#>BD_HdPtr,n1
	move	y:(r0+n0),r1					; Pointer to Sink Descriptor
	nop
	move	r4,y:(r1+n1)					; Sink Buffer ptr
_PrepareForNextModule
	move	#FGndModuleEntrySize,n0
	nop
	move	(r0)+n0							; Set ptr to next Module Table Entry
	rts


	
;;***********************************************************************
;	Function:
;		ExecFgndModules
;	Description:
;		Main Foreground module execution loop.
;  	Parameters passed: 
;		None
;  	Return Values:
;		None
;	Registers Destroyed:	   
;		All.
;;***********************************************************************
ExecFgndModules
	jsr		SysAllLinear
	; The following loop ensures that not more than 64 samples are processed at a time
	; During startup, to ensure that DAC buffer is completely full, more samples may be
	; required. But don't process them all, else there could be DAC underflows.
    ; Do not process more than 128 samples at a time.
    ; In wbich case, momentary MIPs demand becomes double.
    ; The MIPs demand of all the Foreground modules should not exceed half of the total 
    ; Processor MIPs. 
	move y:<Const_000002,A
	move A,y:FgndLoopCnt
_ExecOuterLoop
	move    y:FgndLoopCnt,B
    move    y:<Const_000001,x0
    sub     x0,B    y:TotalSamplestoProcess,A
    jeq     _EndExecOuterLoop
    move    B,y:FgndLoopCnt
	tst		A		y:<Const_000040,B
	jle		_EndExecOuterLoop
	sub		B,A			A,x0
	tlt		x0,B
	move	B,y:DacSamplestoProcess
	move	A,y:TotalSamplestoProcess
			
	move	#ModuleControlBlockBase,r0
	do		#NUM_FGND_MODULES,_EndFGndLoop
		jsr	GetIOPtrs
		move y:(r0),A					; Check Module Enable Flag.
		tst	A
		jne	_Continue1
		; If equal to zero, then module is bypassed
		; Just copy data from input to output.										
		move	y:DacSamplestoProcess,x1
		do	x1,_EndBypass
			move	x:(r3)+,x0			; Bypass always assumes source as X
			move	x0,x:(r4)+			; Do not bypass the copy Module. Ever.
_EndBypass
		jmp	_Continue2
_Continue1
		move #>EffRoutine_Offset,n0
		push r0							; Save pointer to the current MCB entry
		move y:(r0+n0),r1
		nop
		jsr	(r1)						; Call the module
		pop	r0
_Continue2
		jsr	SetIOPtrs					; Also sets the ptr to next module block in r0
_EndFGndLoop
		jmp	_ExecOuterLoop
_EndExecOuterLoop
	rts
  if @def('WOW')
	org p,".WowFgndFncs":
;;***********************************************************************
;	Function:
;		EnablePluginModule
;	Description:
;		Sets Module Enable Flag in the foreground module table
;  	Parameters passed: 
;		r0: Base address of Module entry
;  	Return Values:
;		None
;	Registers Destroyed:	   
;		A1
;;***********************************************************************
FEnablePluginModule
        move y:<Const_000001,A1
        move A1,y:(r0)					; Enable Module Enable Flag.
		rts

;;***********************************************************************
;	Function:
;		DisablePluginModule
;	Description:
;		Clears Module Enable Flag in the foreground module table
;  	Parameters passed: 
;		r0: Base address of Module entry
;  	Return Values:
;		None
;	Registers Destroyed:	   
;		A1
;;***********************************************************************
FDisablePluginModule
        move y:<Const_000000,A1
        move A1,y:(r0)					; Disable Module Enable Flag.
		rts
  endif


