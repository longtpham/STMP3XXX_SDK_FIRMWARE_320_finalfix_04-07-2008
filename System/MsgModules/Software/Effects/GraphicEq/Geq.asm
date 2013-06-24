;;***********************************************************************
;SigmaTel, Inc
;$Archive: $
;$Revision: 3 $
;$Date:  $
;Description: 
;Notes:	This is the main file. 
;		
;***********************************************************************/

	Section	GeqMain

;*******************************************************************************
;  Include files
;*******************************************************************************
		include "geqequ.inc"
		include "stack.xref"
		include "stackmem.xref"
		include "sysequ.inc"
	    include "sysmacro.asm"
	    include "const.xref"
		include "sysmisc.xref"
    	include "bufdesc.xref"

;*******************************************************************************
;  Extern definitions
;*******************************************************************************
		Global	GeqInit
		Global  GeqProcess
;*******************************************************************************
;  Extern references
;*******************************************************************************
        if ("BLD"=="SMNISR")
		Extern	GeqSimIn,GeqSimOut
		Extern	GeqSimInInit,GeqSimOutInit
		endif 

		Extern	FilterCoefs
		Extern	dBTable,ZerodBPoint
		Extern    currentdBgainbase,currentgainbase,TargetdBGainBase	
		Extern    x_filter_state_base_odd,x_filter_state_base_even
		Extern	y_filter_state_base_odd,y_filter_state_base_even 	
        Extern  DacSamplestoProcess       



;*******************************************************************************
;  Code start
;*******************************************************************************
	org p,"GraphicEq":


;;***********************************************************************
;	Function:
;			GeqProcess
;	Description:
;			
;			
;  Parameters passed: 
;			r3 = Input buffer pointer
;			r4 = Output buffer pointer
;			m3 = Input buffer modulo
;			m4 = Output buffer modulo.
;  Return Values
;		   
;
;;***********************************************************************
GeqProcess

	jsr		UpdateDeltaGains

	move	y:<Const_000002,n2						; To jump over a filter	
	; Next Set the filter state modulo buffers. 
	move	y:<Const_000001,m2
	move	m2,m6									
	move	m6,m0

 	move	#FilterCoefs,r5
	move	#NumberOfBands*3-1,m5
	move	#currentgainbase,r1
	move	#NumberOfBands-1,m1

	; Process the even channel
	move	#y_filter_state_base_even,r2
	move	#x_filter_state_base_even,r6
	push	r3
	push	r4
	jsr		ApplyEqualizer							; Apply equalizer for even channel

	; Now process the odd channel.
	move	#y_filter_state_base_odd,r2
	move	#x_filter_state_base_odd,r6
	pop		r4
	pop		r3
	move	(r4)+									
	move	(r3)+
	jsr		ApplyEqualizer						   	; Apply equalizer for even channel
	move	(r3)-
	move	(r4)-

	; Revert back the modified modulo counters.
	move	y:<Const_ffffff,m0
	move 	m0,m1
	move	m1,m2
	move	m1,m5
	move	m5,m6

	rts
		
;;***********************************************************************
;	Function:
;			ApplyEqualizer
;	Description:
;			
;			
;  Parameters passed: 
;			r3 = Input buffer pointer
;			r4 = Output buffer pointer
;			m2 = 1
;			m4 = 1
;			n2 = 1
;  Return Values
;		   
;
;;***********************************************************************

ApplyEqualizer
	; r0 serves as the y filter state ptr base
 	move	r2,r0												; Save y state ptr
	move	y:DacSamplestoProcess,A
	asr		A	   #>NumberOfBands,y1							; Half number per channel	
 	clr		A	   A,x0
	move	#scalingfactor,x1								   	; Feedforward path gain
	ori		#$08,MR												; To shift left automatically.
 	do	x0,_EndBlock
		move	y:(r5)+,y0 		x:(r2)+,x0						;y0 = a2	x0 = y[n-2]
 		do	   y1,_EndFilterBanks
 			mpy		-x0,y0,B   	y:(r5)+,y0		x:(r3),x0		;y0 = b0	x0 = x[n]
 			mac		x0,y0,B	   	x:(r6),x0						;x0 = x[n-2]
 			mac		-x0,y0,B   	y:(r5)+,y0		x:(r2)+,x0		;y0 = a1, 	x0 = y[n-1]
 			macr 	x0,y0,B	   	y:(r1)+,x0						;y0 = Gain for scaling
 			move	B,y0	   	B,x:(r2)+n2						;y0 = filterResp, x:(r2)=y[n-2]
 			mac		x0,y0,A	   	y:(r5)+,y0 		x:(r2)+,x0 		;y0 = a2	x0 = y[n-2]
_EndFilterBanks		
 		move	x:(r3)+n3,x0	y:(r5)-,y0						; x0 = x[n], Correct for r5
 		mac		x1,x0,A			x0,x:(r6)+		 y:(r0)+,y0		; x:(r6) = x[n-1], dummy to incr r0
		asl		A				r0,r2
 		clr		A				A,x:(r4)+n4		 				; Atlast, got the result in A
_EndBlock														; Reset default shift to zero.
	andi	#$F7,MR
	rts


;;***********************************************************************
;	Function:
;			UpdateDeltaGains
;	Description:
;		The Gains are stored as entries to dBTable.
;		Everycall to this function compares the entries in currentdBtable	
;		with entries in the TargetdBTable. If these are equal, then the 
;		no changes are undertaken and value is copied to currentgainTable,
;		which gives decimal value of gain.
;			If entries are unequal, the currentdBTable entry is incremented
;		or decremented. This ensures that gain changes take place 
;		linearly in terms of dB.
;  Parameters passed: 
;
;  Return Values
;		   
;
;;***********************************************************************
UpdateDeltaGains
 	move	#currentgainbase,r2				; y:(r2) currentgainbaseptr
 	move	#currentdBgainbase,r5				; x:(r5) currentgaindBptr
 	move	#TargetdBGainBase,r6				; y:(r2) targetgaindBptr
	do	   #NumberOfBands,_EndFilterBanks
		move	y:(r5),x1						; x1 = currentgaindBvalue + base
		move	y:(r6)+,A						; A = targetgaindBvalue + base
		cmp		x1,A			x1,r1			; Is Target_gain_in_db == Current_gain_in_dB?
		jeq		_Continue
		jlt		_Continue1
		move	(r1)+							; Update only one gain
		jmp		_EndUpdate
_Continue1
		move	(r1)-
_EndUpdate
		move	x:(r1),y0						; y0 = current gain
		move	y0,y:(r2)+						; ygain:(band_no)+ = currentgain
		move	r1,y:(r5)+						; ydbptr:(band_no)+ = currentgaindBvalue 
		enddo
		jmp		_EndUpdate1
_Continue						
		move	x:(r1),y0						; y0 = current gain
		move	y0,y:(r2)+						; ygain:(band_no)+ = currentgain
		move	r1,y:(r5)+						; ydbptr:(band_no)+ = currentgaindBvalue 
_EndFilterBanks
		nop
_EndUpdate1
	rts



	
	Endsec