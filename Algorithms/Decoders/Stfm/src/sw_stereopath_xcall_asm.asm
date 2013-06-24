;////////////////////////////////////////////////////////////////////////////////
;// Copyright(C) SigmaTel, Inc. 2005
;//
;// File :         sw_stereopath_xcall_asm.asm
;// Description:   This file contain assembly implementation for
;//				  IIR filter for stereo path.
;////////////////////////////////////////////////////////////////////////////////

;////////////////////////////////////////////////////////////////////////////
; Global definitions
;////////////////////////////////////////////////////////////////////////////
	extern y:FLPR_PtrXUpdate
	extern y:FLPR_PtrYUpdate
	extern y:FLMR_PtrXUpdate
	extern y:FLMR_PtrYUpdate
	extern x:FIIRFiltCoeff
	extern y:Fg_sd_audio_pCoefForcedMono_u1
	extern y:Fg_sd_audio_pCoefBypassBlend_u1
	extern y:Fg_sd_audio_pCoefBypassSoftmute_u1
	extern y:FgLPR_Mult_fract
	extern y:FgLMR_Mult_fract      

	
;///////////////////////////////////////////////////////////////////////////////////
; Local labels
;///////////////////////////////////////////////////////////////////////////////////
	org     x,".xdatasw_stereopath_xcall_asm":
;///////////////////////////////////////////////////////////////////////////////////
;xdata		ds		1
;///////////////////////////////////////////////////////////////////////////////////
	org     y,".ydatasw_stereopath_xcall_asm":
;///////////////////////////////////////////////////////////////////////////////////
;ydata		ds		2
;///////////////////////////////////////////////////////////////////////////////////

;/////////////////////////////////////////////////////////////////////////////////
;// Name:           sw_stereopath_xcall_asm
;//
;//  Type:           Function
;//  Description:    This function is assembly implementation of stereo path.
;//
;//  Inputs:         int * pDin_X --pointer to circular input buffer in _X memory.   r0, m0
;//                             Data is left justified in the word.
;//                             The data order is: LPR0, LMR0, LPR1, LMR1, LPR2, LMR2, ...
;//
;// 				 int * pDout_Y --pointer to circularoutput buffer in _Y memory. r4, m4
;//                             Data is left justified in the word.  
;//                             The data order is: L0, R0, L1, R1, L2, R2, ...
;//
;//  				int samples_per_block --number of samples per block, A1
;//                                         must be a multiple of 16
;//                  
;//
;//  Outputs:        r0, r4 point to next element in input and output buffers
;//                  
;//
;//  Notes:        m4 not preserved  
;/////////////////////////////////////////////////////////////////////////////////
   
   org     p,".ptextsw_stereopath_xcall_asm":

    
;Fsw_stereopath_xcall_asm:
StfmLowPassFilter:
   
 ; Optimized Filter
   
	move	r4,r1		            ;Output buffer in r1
    move    m4,m1
	
	move	#FIIRFiltCoeff,r3   ; pass the address into r3, not the value at the filter coef address
	
	move	y:FLPR_PtrXUpdate,r4
	move	y:FLPR_PtrYUpdate,r2
	move	y:FLMR_PtrXUpdate,r5
	move	y:FLMR_PtrYUpdate,r6
		
	move	y:FgLPR_Mult_fract,x1         ; gain factors
    move	y:FgLMR_Mult_fract,y1
    
    movec	#4,m4
	movec	m4,m3
	movec	m4,m2
	movec	m3,m6
	movec	m3,m5

	move	x:(r0)+,x0	            ;This is a part of Doloop, moved outside the Do loop

	do		A1,_DO_LOOP_END

;   LPR path filter

	move	x0,y:-(r4)				                                                            
	move	x:(r3)+,x0				y:(r4)+,y0
	mpy		x0,y0,A					x:(r3)+,x0				y:(r4)+,y0
	mac		x0,y0,A					x:(r3)+,x0				y:(r4)+,y0
	mac		x0,y0,A					x:(r3)+,x0				y:(r4)+,y0
	mac		x0,y0,A					x:(r3)+,x0				y:(r4)+,y0
	mac		x0,y0,A				    x:(r0)+,B1
	asl		A                       ;A1,y0        ; Stmp00011112 change to double the gain 
    move 	A1,y0								
	move	A1,y:-(r2)							
	mpy		x1,y0,A					B1,y:-(r5)		    ;A = LPR_Data = gLPR_Mult * IIRFilter_output

;   LMR path filter

	move	x:(r3)+,x0				y:(r5)+,y0
	mpy		x0,y0,B					x:(r3)+,x0				y:(r5)+,y0
	mac		x0,y0,B					x:(r3)+,x0				y:(r5)+,y0
	mac		x0,y0,B					x:(r3)+,x0				y:(r5)+,y0
	mac		x0,y0,B					x:(r3)+,x0				y:(r5)+,y0
	mac		x0,y0,B					A1,A						 			
	asl		B						;B1,x0         ;Stmp00011112 change to double the gain
    move    B1,x0									
    mpy		x0,y1,B					B1,y:-(r6)			; B = LMR_Data = gLMR_Mult * IIRFilter_output			  
    move	B1,B
    add		A,B	   					x:(r0)+,x0			;*pFilteredOutput++ = Left channel  = (LPR_Data + LMR_Data);    
    ;move	x:(r0)+,x0			;*pFilteredOutput++ = Left channel  = (LPR_Data + LMR_Data);    
   
    subl	B,A                   B1,y:(r1)+
    ;move	B1,y:(r1)+			;*pFilteredOutput++ = Right channel = (LPR_Data - LMR_Data);   											
    move	A1,y:(r1)+				

_DO_LOOP_END:

    move	x:(r0)-		            ;This will compensate for extra increment of r0 register
	move	r4,y:FLPR_PtrXUpdate
	move	r2,y:FLPR_PtrYUpdate
	move	r5,y:FLMR_PtrXUpdate
	move	r6,y:FLMR_PtrYUpdate
    move    r1,r4

   	movec	m7,m3
	movec	m7,m4
	movec	m7,m6
	movec	m7,m2
	movec	m7,m5

	rts							

