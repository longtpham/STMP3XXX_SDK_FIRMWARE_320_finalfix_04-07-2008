;******************************************************************************
; Copyright(C) SigmaTel, Inc. 2000/2001
;
; STMP DAC Driver Module
;******************************************************************************
; Module Files:
;   \common\dac\dacdrv.asm
;   \common\dac\dacmem.asm
;   \inc\dacequ.asm
;   \inc\dacdef.asm
;
;******************************************************************************

;*******************************************************************************
;  Include files
;*******************************************************************************
		include "resource.inc"
        include "sysresources.inc"
        include "sysequ.inc"
        include "hwequ.inc"
		include "dacequ.inc"
		include "stack.xref"
		include "stackmem.xref"
		include "sysequ.inc"
		include "dacmem.xref"
        include "dacmemy.xref"
	    include "irqequ.inc"
	    include "sysmacro.asm"
	    include "stack.xref"
	    include "stackmem.xref"
	    include "const.xref"
		include "sysmisc.xref"
    	include "bufdesc.xref"
		include "fgndmodules.inc"
		include "fgndmodules.xref"
        include "sysmem.xref"
        page    132,60,0,0,0

;*******************************************************************************
;  Extern definitions
;*******************************************************************************
    extern ZerodBPoint
  if @DEF('TUNER_STFM1000')
    extern EncoderBufHeadPtr
    extern EncoderBufModPtr
    extern Fg_EncodeStfm
  endif
    extern F_lc_u_e_FGndModules_P
    extern F_lc_u_b_FGndModules_P
    extern F_lc_u_e_srs_FGndModules_P
    extern F_lc_u_b_Srs_Effects_X
    extern F_lc_u_e_Srs_Effects_X

	extern SysLoadResource
    
;*******************************************************************************
;  Global Definitions
;*******************************************************************************

    global  CopytoEffectsBuffer
	global	EffectsInit
    global  LoadEffectsModules
        

;*******************************************************************************
;  Code start
;*******************************************************************************

        org p,".Effectsbuffer":
        
;;***********************************************************************
;	Function:
;		EffectsInit
;	C Function:
;		void EffectsInit(int BufDescrPtr,int MemoryTypeFlag)
;	Description:
;		Called from DecoderInit
;		Sets the Decoder descriptor and memory type.
;		Initializes the Copy Sink buffer
;  	Parameters passed: 
;		A : DecoderOutputBuffer Descriptor
;		B : Memory type flag : 0->X memory, 1->Y memory.
;		x0 : Mono output = 0, Stereo Output = 1
;		MP3 and WMA always duplicate mono samples, so set as 1 for these decoders.
;  	Return Values:
;		None
;	Registers Destroyed:	   
;		x1,y0.
;;***********************************************************************

FEffectsInit
EffectsInit

    push	m0
	push	r0
	push	n0
	push	m1
	push	r1
	push	n1

	; push all passing parameters. 
    push    A
    push    B
    push    x0
    
    ; load EffectsModules here.
    jsr    LoadEffectsModules
    
    ; pop all passing parameters.
    pop     x0
    pop     B
    pop     A
    
    move	y:<Const_ffffff,m0
	move	#>CopyModuleBase,r0
	move	#>EffSrcDescrOffset,n0
	move	m0,m1
	move	A,y:(r0+n0)
	move	#EffParameter1_Offset,n0
	nop
	move	B,y:(r0+n0)
	move	#EffParameter0_Offset,n0
	nop
	move	x0,y:(r0+n0)
	move	#>EffSinkDescrOffset,n0

	; Now clear the sink buffer
	move	#0,y0
    move	y:(r0+n0),r1				; r1 points to the Buffer desriptor
    move    #>BD_BufferSize,n1
	move	y:(r1),r0			; r0 points the the buffer
    move    y:(r1+n1),x1                ; get the buffer SIZE
	rep		x1
    move    y0,x:(r0)+

	pop		n1
	pop		r1
	pop		m1
	pop		n0
	pop		r0
	pop		m0
	rts
    
;;***********************************************************************
;	Function:
;		LoadEffectsModules
;	Description:
;		Load  Effects Modules
;  	Parameters passed: 
;		None
;  	Return Values:
;		None
;	Registers Destroyed:	   
;		Assume all other than defined by convention.
;;***********************************************************************

LoadEffectsModules

    ; Load the Effects Modules into P memory
    move    #>RSRC_EFFECTS_MODULES_P,x0                           	; Resource number
    move    #>RSRC_TYPE_DATA,x1                                     ; Resource type is data
    move    #>(F_lc_u_e_FGndModules_P-F_lc_u_b_FGndModules_P)*3,y1  ; Size of the overlay area
    move    #>F_lc_u_b_FGndModules_P,y0                             ; Address of the overlay area
    move    #TARGET_MEM_P,a1                                        ; Overlay area is in P memory
    jsr     SysLoadResource

 if (@DEF('WOW'))
    ; Load the SRS Effects Modules into P memory
    move    #>RSRC_SRS_EFFECTS_MODULES_P,x0                         ; Resource number
    move    #>RSRC_TYPE_DATA,x1                                     ; Resource type is data
    move    #>(F_lc_u_e_srs_FGndModules_P-F_lc_u_e_FGndModules_P)*3,y1  ; Size of the overlay area
    move    #>F_lc_u_e_FGndModules_P,y0                             ; Address of the overlay area
    move    #TARGET_MEM_P,a1                                        ; Overlay area is in P memory
    jsr     SysLoadResource

    ; Load the SRS effects module into X memory
    move    #>RSRC_SRS_EFFECTS_MODULES_X,x0                   		; Resource number
    move    #>RSRC_TYPE_DATA,x1                                 	; Resource type is data
    move    #>(F_lc_u_e_Srs_Effects_X-F_lc_u_b_Srs_Effects_X)*3,y1  ; Size of the overlay area
    move    #>F_lc_u_b_Srs_Effects_X,y0                           	; Address of the overlay area
    move    #TARGET_MEM_X,a1                                    	; Overlay area is in X memory
    
    jsr     SysLoadResource
 endif
 
    rts

		org p,".Copybuffer":


;;***********************************************************************
;	Function:
;		CopytoEffectsBuffer
;	Description:
;		Copies from source memory type X/Y to a Work buffer
;  	Parameters passed: 
;		r0: ptr to module control block entry
;		r3,m3,n3 : Source
;		r4,m4,n4 : Sink
;  	Return Values:
;		None
;	Registers Destroyed:	   
;		Assume all other than defined by convention.
;;***********************************************************************
CopytoEffectsBuffer
  if @DEF('TUNER_STFM1000')
	push	r2
        push    m2
        push    b0
        ; check available space in the encoder source buffer is done in DACIsr
        move    y:Fg_EncodeStfm,b0		; load encoding STFM flags
	jclr	#1,b0,_noEncMemCopy
        move    y:EncoderBufHeadPtr,r2
        move    y:EncoderBufModPtr,m2
_noEncMemCopy
  endif
	; The copy buffer manipulates the Workbuffer source ptr to be same as work buffer sink ptr.
        move	#>EffSinkDescrOffset,n0
	move	#>BD_TailPtr,n1
	move	y:(r0+n0),r1					; Pointer to Sink Descriptor
	nop
	move	r4,y:(r1+n1)					; Sink Buffer ptr


    ; Autoscaling for the Graphic Equalizer
    ; Find the scaling.
    move    y:<Const_7fffff,x1
    ; Scale only if Geq is enabled
    move    y:GeqModuleBase,A
    tst     A       y:g_VolumeBias,A
    jeq     _Continue
    ; Make sure that the scaling is positive
    tst     A       A,x0
    jlt     _Continue
    move    #ZerodBPoint,A
    sub     x0,A
    move    A,r1
    move    #>$1fffff,A
    move    x:(r1),x0
    add     x0,A
    asl     A
    asl     A
    move    A,x1

_Continue
	move	#EffParameter1_Offset,n0
	move	#>TARGET_MEM_Y,x0
	move	y:(r0+n0),A
	cmp		x0,A		#EffParameter0_Offset,n0
	jeq		_YMemoryCopy

	; Decoder output buffer is in X memory(default) 
_XMemoryCopy
	move	y:(r0+n0),A
	tst		A		y:DacSamplestoProcess,A
	jne		_StereoXCopy
	asr		A
	do	A1,_EndXCopyMono
		move	x:(r3)+,x0
  if @DEF('TUNER_STFM1000')
        jclr    #1,b0,_notEncStfmMX
        move    x0,y:(r2)+
        move    x0,y:(r2)+
_notEncStfmMX
  endif
        mpy     x1,x0,A
		move	A,x:(r4)+
		move	A,x:(r4)+					; Just duplicate the sample.
_EndXCopyMono
  if @DEF('TUNER_STFM1000')
	jmp     _ExitCopyBuffer
  else
	rts
  endif

_StereoXCopy
	do	A1,_EndXCopyStereo
		move	x:(r3)+,x0
  if @DEF('TUNER_STFM1000')
        jclr    #1,b0,_notEncStfmSX
        move    x0,y:(r2)+
_notEncStfmSX
  endif
        mpy     x1,x0,A
		move	A,x:(r4)+
_EndXCopyStereo
  if @DEF('TUNER_STFM1000')
	jmp     _ExitCopyBuffer
  else
	rts
  endif

	; Decoder output buffer is in Y memory
_YMemoryCopy
	move	y:(r0+n0),A
	tst		A		y:DacSamplestoProcess,A
	jne		_StereoYCopy
	asr		A
	do	A1,_EndYCopyMono
		move	y:(r3)+,x0
  if @DEF('TUNER_STFM1000')
        jclr    #1,b0,_notEncStfMY
        move    x0,y:(r2)+
        move    x0,y:(r2)+
_notEncStfMY
  endif
        mpy     x1,x0,A
		move	A,x:(r4)+
		move	A,x:(r4)+					; Just duplicate the sample.
_EndYCopyMono
  if @DEF('TUNER_STFM1000')
	jmp     _ExitCopyBuffer
  else
	rts
  endif
_StereoYCopy
	do	A1,_EndYCopyStereo
		move	y:(r3)+,x0
  if @DEF('TUNER_STFM1000')
        jclr    #1,b0,_notEncStfmSY
        move    x0,y:(r2)+
_notEncStfmSY
  endif
        mpy     x1,x0,A
		move	A,x:(r4)+
_EndYCopyStereo
  if @DEF('TUNER_STFM1000')
_ExitCopyBuffer
	jclr	#1,b0,_noEncMemCopyBuf
	bclr    #1,b0
        move    b0,y:Fg_EncodeStfm
        move    r2,y:EncoderBufHeadPtr
_noEncMemCopyBuf
	pop     b0
	pop     m2
        pop     r2
  endif
	rts


