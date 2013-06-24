;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2001
;
; File : msadpcm_encode.asm
; Description : Encoder algorithm for MS ADPCM
;///////////////////////////////////////////////////////////////////////////////
    if @def('MS_ADPCM')
	section Encode_MS_ADPCM

;*********************************************************************************
;	Include Files
;
;*********************************************************************************
	include "const.xref"
    include "stack.xref"
    include "wave.xref"
    include "msadpcm.xref"
    include "sysmacro.asm"

;*********************************************************************************
;	Globals
;
;*********************************************************************************

    global Encode_MS_ADPCM_Block,FEncode_MS_ADPCM_Block

;*********************************************************************************
;	Externs
;
;*********************************************************************************

    extern WriteByteData, WriteWordData,EncByteState,Enc_Const_0000ff
    if @def('ADPCMTEST')
    extern FSampleBuf,FBitBuf
    endif
    extern iSamp2,iSamp1,iCoef2,iCoef1,iPredSample,iDelta     

;*********************************************************************************
;       Local Short Data Variables.
;		
;*********************************************************************************

MSADPCMSAMPLESTOEVALUATE    equ         16

	org y,".shortdata@Encode_MS_ADPCM",OVERLAY,NEAR:

nibblestate             ds              1
SamplestoDecode         ds              1
numchannels             ds              1
InpBufPtr               ds              1
InpBufPtrEnc            ds              1
iDeltaTemp              ds              1
Arraybase               ds              1
Arraybase2              ds              1
encodedbyte             ds              1
imin                    ds              1
MinError                ds              1
Error_Accum             ds              1
negval                  ds              1
predloc                 ds              1
TempConst_00000f        ds              1
TempConst_000007        ds              1
TempConst_fffff8        ds	            1
TempConst_007fff        ds              1
TempConst_ff8000        ds	            1

 	calls 'Fmain','Encode_MS_ADPCM'
;///////////////////////////////////////////////////////////////////////////////
;
;		P memory
;
;///////////////////////////////////////////////////////////////////////////////

	org p,"Encode_MSADPCM_Block_P":

;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  Encode_MS_ADPCM_Block
;
;    Type:	Function
;
;    Description:	Encodes MS ADPCM block and places compressed data.
;
;    Inputs:		
;			A = Number of Channels
;			r0 = Outputbuffer, m0 = Modulo.
;           r4 = X:Inputbuffer, m4 = modulo,
;
;    Outputs:		
;			None
;    Notes:
;			Registers trashed: Assume all except r7. R0 and R4 will be updated to point
;			to point next input/output locations.
;///////////////////////////////////////////////////////////////////////////////

FEncode_MS_ADPCM_Block
Encode_MS_ADPCM_Block
    if @def('ADPCMTEST')
    ; While Testing pass r0, r4 implicitly
    ; m0 and m4 for testing are by default linear
    move #FBitBuf,r0
    move y:<Const_000002,n4
    move #FSampleBuf,r4
    endif
    move #>$f,x0
    move x0,y:<TempConst_00000f
    move #>$ff,x0
    move x0,y:<Enc_Const_0000ff
    move #>$7,x0
    move x0,y:<TempConst_000007
    move #>$fffff8,x0
    move x0,y:<TempConst_fffff8
    move #>$7fff,x0
    move x0,y:<TempConst_007fff
    move #>$ff8000,x0
    move x0,y:<TempConst_ff8000
    move y:<Const_000002,n3
    move A,n4		
    move r4,y:<InpBufPtr
    move y:first_block_flag,B
    tst	B		A,y:<numchannels
    jeq _Continue
    move #iDeltaEnc,r3
    do  A,_EndSetDeltaEnc
        move y:(r4)+n4,A
        move y:(r4)-,x0
        sub	x0,A
        asr	A							; Avoid overflow on sub
        abs	A           y:<Const_000010,y0
        move A,x0
        mpy  x0,#9,A					; Set iDeltaEnc such that it gives Error Delta = 4
        ; Don't let iDelta be zero or very small to prevent Divide by Zero.
        cmp  y0,A
        tlt  y0,A
        move A,y:(r3)+
_EndSetDeltaEnc
_Continue
	clr A		y:iDeltaEnc,x0
	move A,y:first_block_flag
    move A,y:<EncByteState
	move y:<InpBufPtr,r4
    move #iDelta,r3
    move r3,y:<Arraybase2
   	move y:iDeltaEnc,x0
	do y:<numchannels,_EndFindPredictor
    ; 	iDelta[0] = iDeltaEnc[0];
    ; 	iDelta[1] = iDeltaEnc[1];
    	move x0,y:<iDeltaTemp
        ; r3 points to iDelta
		jsr	Find_Predictor
		move A,n1
		move #Fg_Wave_iCoef2Array,r1
		jsr	 WriteByteData
;		iCoef2[ch] = iCoef2Array[coefindex[ch]];
        move #>$6,n3
		move y:(r1+n1),A
        move (r3)-n3                                ; Set it to point to iCoef1
        move y:<Const_000002,n3
		move #Fg_Wave_iCoef1Array,r1
		move A,y:(r3)+n3
;		iCoef1[ch] = iCoef1Array[coefindex[ch]];
		move y:(r1+n1),A
		move A,y:(r3)+n3
		; Prepare for the next channel.
		move y:<InpBufPtr,r4
        move #(iDelta+1),r3
   	    move y:(iDeltaEnc+1),x0
		move (r4)+
_EndFindPredictor


;	for(ch=0; ch < num_channels; ch++)
;	{
;		WriteWordData(iDeltaEnc[ch]);
;	}
	move y:iDeltaEnc,A
	do y:<numchannels,_EndSaveIDelta
		jsr WriteWordData
		move y:(iDeltaEnc+1),A
_EndSaveIDelta

;	for(ch=0; ch < num_channels; ch++)
;	{
;		iSamp1[ch] = InputBuf[num_channels + ch];
;		WriteWordData(iSamp1[ch]);
;	}
    move y:<InpBufPtr,r4
    move #iSamp1,r3
    move (r4)+n4
    move y:(r4)+,x0
    mpy x0,#8,A
    do y:<numchannels,_EndSaveiSamp1
        move A,y:(r3)+
        jsr WriteWordData
        move y:(r4)+,x0
        mpy x0,#8,A
_EndSaveiSamp1
		

;	for(ch=0; ch < num_channels; ch++)
;	{
;		iSamp2[ch] = InputBuf[ch];
;		WriteWordData(iSamp2[ch]);
;	}
    move y:<InpBufPtr,r4
    move #iSamp2,r3
    move r3,y:<Arraybase
    move y:(r4)+,x0
    mpy x0,#8,A
    do y:<numchannels,_EndSaveiSamp2
		move A,y:(r3)+
		jsr WriteWordData
		move y:(r4)+,x0
		mpy x0,#8,A
_EndSaveiSamp2

;	for(ch=0; ch < num_channels; ch++)
;	{
;		iPredSamp[ch] = (iSamp1[ch]*iCoef1[ch] + iSamp2[ch]*iCoef2[ch])>>8;
;	}
    ; The following loop is duplicated in both Decoder and Encoder.
    ; Duplicated, because encoder and decoder overlays will be different.
    move y:<Arraybase,r3
    do   y:<numchannels,_EndPredChannelLoop
        move  y:(r3)+n3,x0                  ; x0 = iSamp2
        move  y:(r3)+n3,x1                  ; x1 = iSamp1
        move  y:(r3)+n3,y0                  ; y0 = iCoef2
        mpy x0,y0,A       y:(r3)+n3,y0      ; y0 = iCoef1
        mac x1,y0,A
        asr A      
        move A0,x0         
        mpy  x0,#8,A                        ; Account for default leftshift.
        move A,y:(r3)+n3       ; Store the predicted value
        move y:<Arraybase,r3
        nop
        move (r3)+
_EndPredChannelLoop

; 	iDelta[0] = iDeltaEnc[0];
; 	iDelta[1] = iDeltaEnc[1];
	move y:iDeltaEnc,x0
	move x0,y:iDelta
	move y:(iDeltaEnc+1),x0
	move x0,y:(iDelta+1)


; 	nibblestate = 0;
; 	for (j = 2; j < wnSamplesPerBlock; j++)
; 	{
; 		for (ch = 0; ch < num_channels; ch++)
; 		{

	clr  A		y:Fg_Wave_wnSamplesPerBlock,B
	move y:<Const_000002,x0
	sub	 x0,B	A,y:<nibblestate
	tfr  x0,A 	y:<InpBufPtr,x0
	rep  y:<numchannels
	asl	 A
	asr  A						; Restore the extra shift.
	add  x0,A               y:<Arraybase2,r3
	move A,r4
	
    do B,_EndEncodeBlock
        do   y:<numchannels,_EndChannelLoop
; Input data is in 24 bit. The algorithm works on 16 bit data.
; 			iError = InputBuf[num_channels*j+ch] - iPredSamp[ch];
            move y:(r4)+,x0
            move y:(r3)-n3
            mpy x0,#8,A
            move #0,A0			
            clr  B          y:(r3)+n3,x0
            sub  x0,A       #1,x0
            tlt  x0,B                               ; Set negative flag if less than 0
            abs  A          B,y:<negval
            clr  A          y:(r3),y0   A,x0                        ; Get iDelta.
            move x0,A0
; 			iErrorDelta = iError/iDelta[ch];
            rep  #25
            div y0,A        
;           TBD: Divide using a series of compare operations to minimize MIPs

; 			if (iErrorDelta < -8)
; 				iErrorDelta = -8;
; 			else if (iErrorDelta > 7)
; 				iErrorDelta = 7;
            tst  B          A0,A
            jeq  _Continue1
            neg   A         y:<TempConst_fffff8,x0
            cmp   x0,A
            tlt   x0,A
            jmp  _Continue2
_Continue1
            move  y:<TempConst_000007,x0
            cmp   x0,A
            tgt   x0,A
_Continue2
; 			if (nibblestate == 0)
; 			{
; 				encodedbyte = iErrorDelta<<4;
; 				nibblestate = 1;
; 			}
; 			else
; 			{
; 				encodedbyte |= (iErrorDelta & 0xf);
; 				WriteByteData(encodedbyte);
; 				nibblestate = 0;
; 			}
            move  y:<nibblestate,B
            move  y:<Const_000001,x0
            eor   x0,B      y:<Const_000008,x0
            move  B,y:<nibblestate
            move  y:<encodedbyte,x0
            move  A,y:<encodedbyte
            jne   _Continue3
            mpy   x0,#20,B
            move  y:<TempConst_00000f,x0
            and   x0,A      B0,x0
            or    x0,A
            move  A1,A
            jsr   WriteByteData
_Continue3
            move  y:<encodedbyte,B
            tfr   B,A       y:<TempConst_00000f,x0
            and   x0,A
            move  A1,A
            ; r3 points to iDelta
            ; A contains the index for adaption table            
            ; B Errordelta is now signed            
            jsr  MSADPCM_Decode_Sample
            move y:<Arraybase2,r3
            nop
            move (r3)+                                          ; Advance to next channel
_EndChannelLoop
        move y:<Arraybase2,r3
        nop
_EndEncodeBlock
; 	// Save the current delta, to re-use for next block.
; 	iDeltaEnc[0] = iDelta[0];
; 	iDeltaEnc[1] = iDelta[1];
	move y:iDelta,x0
	move x0,y:iDeltaEnc
	move y:(iDelta+1),x0
	move x0,y:(iDeltaEnc+1)
    rts
    
         
;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  Find_Predictor
;
;    Type:	Function
;
;    Description: Searches for a suitable predictor. We do not do a full blown search
;       instead restrict the search to SAMPLESTOEVALUATE (say 16) samples. 
;       The performance loss is not severe, compared to an optimal search, which would
;       be excessively MIPs intensive.
;       Additional, we utilize the delta from the previous block.
;       Optimization criterion : Mean square Error.
;    Inputs:		
;           r3: Ptr to iDelta
;    Outputs:		
;			A: index for minimum error
;    Notes:
;			Registers trashed: Assume all except r7. R0 and R4 will be updated to point
;			to point next input/output locations.
;///////////////////////////////////////////////////////////////////////////////

Find_Predictor
; 	int MinError = 0x7fffffff; // Max int size.
    move y:<Const_7fffff,x0
    clr A       x0,y:<MinError
    move A,r6
; 	int imin = 0;
    move A,y:<imin
    move r4,y:<InpBufPtrEnc
    move #Fg_Wave_iCoef1Array,r2
    move #Fg_Wave_iCoef2Array,r5
;   Evaluate each coefficient pair for given channels.
    do  #7,_EndSearch
        move y:<iDeltaTemp,x0
        move x0,y:(r3)-n3
        move (r3)-n3                ; Point to iCoef1
        move y:(r2)+,A
        move A,y:(r3)-n3            
        move y:(r5)+,A      
        move A,y:(r3)+n3
        move (r3)+n3
; 		Error = MSADPCM_Encode_Sample_Set(SAMPLESTOEVALUATE, ch, num_channels, iCoef1Array[i], iCoef2Array[i],iDelta);
        ; r3 points to iPredSample
        jsr  MSADPCM_Encode_Sample_Set

; 		if (Error < MinError)
; 		{
; 			MinError = Error;
; 			imin = i;
; 		}
        move y:<MinError,B
        cmp  A,B        r6,x0
        tgt  A,B
        move B,y:<MinError
        move y:<imin,A
        tgt  x0,A
        move (r6)+
        move A,y:<imin
_EndSearch
    rts
        

;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  MSADPCM_Encode_Sample_Set
;
;    Type:	Function
;
;    Description: 
;           Encodes first few samples and computes Mean Square Error.
;    Inputs:		
;           r3: Ptr to iDelta
;    Outputs:		
;			A: Sum of Squared Error.
;    Notes:
;			Registers trashed: Assume all except r7. R0 and R4 will be updated to point
;			to point next input/output locations.
;///////////////////////////////////////////////////////////////////////////////

MSADPCM_Encode_Sample_Set
    move y:<InpBufPtrEnc,r4
    move r3,y:<predloc
    move (r3)-n3                    ; iCoef1
;	int iPredSamp = (iSamp1*iCoef1 + iSamp2*iCoef2)>>8;
    move y:(r3)-n3,y0       
    move y:(r4)+n4,x0
    mpy  x0,#8,A
    move A,x0               y:(r3)-n3,y1
    mpy  x0,y1,B            y:(r4)+n4,x1
    mpy  x1,#8,A
    move A,x1               A,y:(r3)-n3                         ; Write iSamp1
    mac  x1,y0,B            x0,y:(r3)                           ; Write iSamp2
    asr  B                  y:<predloc,r3                       ; Prevent overflow
    move B0,x0
    mpy  x0,#8,A
    clr  B          A,y:(r3)+n3
    move B,y:<Error_Accum
    do #MSADPCMSAMPLESTOEVALUATE,_EndEncodeBlock
;		iError = InputBuf[num_channels*j+ch] - iPredSamp;
;       Input data is 24 bit, but rest of algo works on 16 bit data.
        move y:(r4)+n4,x0       
        move y:(r3)-n3,y0                ; Dummy to decrement r3, so that it points to iPredSamp
        mpy x0,#8,A
        move #0,A0			
        clr  B          y:(r3)+n3,x0
        sub  x0,A       #1,x0
        tlt  x0,B                               ; Set negative flag if less than 0
        abs  A          B,y:<negval 
        move y:(r3),y0      A,x0                   ; Get iDelta.
;		ErrorAccum += iError*iError;
        mpy  x0,x0,B        
        asl  B              #0,A
        asl  B              y:<Error_Accum,x1
        asl  B
        asl  B
        add  x1,B           x0,A0
        move B,y:<Error_Accum
; 			iErrorDelta = iError/iDelta[ch];
        rep  #25
        div y0,A        
;           TBD: Divide using a series of compare operations to minimize MIPs

;		if (iErrorDelta < -8)
;			iErrorDelta = -8;
;		else if (iErrorDelta > 7)
;			iErrorDelta = 7;
        move y:<negval,B
        tst  B          A0,A
        jeq  _Continue1
        neg   A         y:<TempConst_fffff8,x0
        cmp   x0,A
        tlt   x0,A
        jmp  _Continue
_Continue1
        move  y:<TempConst_000007,x0
        cmp   x0,A
        tgt   x0,A
_Continue
        tfr   A,B       y:<TempConst_00000f,x0
        and   x0,A
        move  A1,A
        ; r3 points to iDelta
        ; A contains the index for adaption table            
        ; B Errordelta is now signed            
        jsr  MSADPCM_Decode_Sample
_EndEncodeBlock
    move y:<Error_Accum,A
    rts



;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  MSADPCM_Decode_Sample
;
;    Type:	Function
;           int MSADPCM_Decode_Sample(unsigned int ErrorDelta, int ErrorDelta)
;
;       
;    Description:	Decodes an MS ADPCM Sample
;
;    Inputs:		
;			A = ErrorDelta in unsigned form
;           B = Error Delta in signed form
;    Outputs:		
;			None
;    Notes:
;           Internal function. Assumes r3 points to iDelta.
;           This code is duplicated from Decoder, because it will be located in different overlays.
;///////////////////////////////////////////////////////////////////////////////

FMSADPCM_Decode_Sample
MSADPCM_Decode_Sample
; 	iNewSample = *iPredSample + (*iDelta)*iErrorDelta;
    move   B,x0         y:(r3)-n3,y0            ; Get iDelta
    mpy    x0,y0,B      y:(r3)-n3,x0            ; Get iPredSample
    asr    B            y:<TempConst_007fff,y1
    move   B0,B
    add    x0,B         y:<TempConst_ff8000,x1
; 	if (iNewSample > 0x7fff)
; 		iNewSample = 0x7fff;
; 	else if (iNewSample < -32768)
; 		iNewSample = -32768;
    cmp    y1,B         #>AdaptionTable,r1
    tgt    y1,B
    cmp    x1,B         A,n1
    tlt    x1,B
; 	*iPredSample = ((*iSamp1)*iCoef1 + (*iSamp2)*iCoef2)>>8;
    move    B,x1        y:(r3)-n3,y1                ; y1 = iCoef1
    mpy     x1,y1,B     y:(r3)-n3,y1                ; A = samp1*coef1  y1 = coef2
    move    y:(r3),x0                               ; x0 = iSamp2 <= iSamp1
; 	*iSamp1 = iNewSample;
    mac     x0,y1,B     y:(r1+n1),x0                         ; Get the adaption table entry

    asr     B           x1,y:(r3)+n3                ; Account for default leftshift. Update iSamp1. No need to update iSamp2.
    move    B0,y1                                   ;  Right shift by 8.
    mpy     y1,#8,B
    move    (r3)+n3                                 ; r3 incremented to iCoef1

; 	(*iDelta) = (*iDelta) * AdaptionTable[iErrorDelta&0xf] >> 8;
    mpy     x0,y0,A      (r3)+n3                    ; r3 incremented to iPredictedsamp
    asr     A           y:<Const_000010,y0
    move    A0,x0       
    mpy     x0,#8,A                              ; right shift by 8   
; 	if ((*iDelta) <16) 
; 		(*iDelta) = 16;
    cmp     y0,A        B,y:(r3)+n3              ; Write predicted Sample
    tlt     y0,A
    move    A,y:(r3)                            ; Save delta

    mpy     x1,#16,A
    move    A0,A                                ; Shift it to be 24 bit.
    rts


    endsec
    endif
