;;***********************************************************************
;SigmaTel, Inc
;$Archive: $
;$Revision:  $
;$Date: $
;Description:  Decodes an MSADPCM block of data
; 		
;		
;***********************************************************************/
    if @def('MS_ADPCM')
	section Decode_MS_ADPCM

;*********************************************************************************
;	Include Files
;
;*********************************************************************************
	include "const.xref"
    include "wave.xref"
    include "msadpcm.xref"
    include "sysmacro.asm"
;*********************************************************************************
;	Globals
;
;*********************************************************************************
    
     global Decode_MS_ADPCM_Block

;*********************************************************************************
;	Externs
;
;*********************************************************************************
    
    extern ReadByteData,ReadSWordData,DecByteState,Dec_Const_0000ff
    extern iSamp2,iSamp1,iCoef2,iCoef1,iPredSample,iDelta     
    if @def('ADPCMTEST')
    extern FSampleBuf,FBitBuf
    endif

;*********************************************************************************
;       Local Short Data Variables.
;		
;*********************************************************************************

	org y,".shortdata@Decode_MS_ADPCM",OVERLAY,NEAR:

nibblestate             ds          1
byteval                 ds          1
SamplestoDecode         ds          1
numchannels             ds          1
Arraybase		        ds	        1
Arraybase2              ds          1
TempConst_00000f        ds          1
TempConst_007fff        ds          1
TempConst_ff8000        ds	        1

 	calls 'Fmain','Decode_MS_ADPCM'



;*********************************************************************************
;		P memory 
;
;*********************************************************************************

    org p,"Decode_MS_ADPCM_Block_P":   

;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  Decode_MS_ADPCM_Block
;
;    Type:	Function
;
;       void Decode_MS_ADPCM_Block(int *Output, int *input, int SamplestoDecode)
;
;    Description:	Decodes an ADPCM Block
;
;    Inputs:		
;			A = Number of Samples to Decode
;			r0 = Inputbuffer, m0 = Modulo. 
;           r4 = Outputbuffer, m4 = modulo. 
;    Outputs:		
;			None
;    Notes:
;			Registers trashed: Assume all except r7. R0 and R4 will be updated to point
;			to point next input/output locations.
;///////////////////////////////////////////////////////////////////////////////

FDecode_MS_ADPCM_Block
Decode_MS_ADPCM_Block
    if @def('ADPCMTEST')
    ; While Testing pass r0, r4 implicitly
    ; m0 and m4 for testing are by default linear
    move #FBitBuf,r0
    move y:<Const_000002,n4
    move #FSampleBuf,r4
    endif

    move y:<Const_000002,y0
    sub  y0,A       y:Fg_Wave_wChannels,y1
    move A,y:<SamplestoDecode
    clr  A  y1,y:<numchannels
    move A,y:<DecByteState
    move A,y:<nibblestate
    move y:<Const_000002,n3

    move #>$f,x0
    move x0,y:<TempConst_00000f
    move #>$ff,x0
    move x0,y:<Dec_Const_0000ff
    move #>$7fff,x0
    move x0,y:<TempConst_007fff
    move #>$ff8000,x0
    move x0,y:<TempConst_ff8000
;	for (i = 0; i < gwChannels; i++)
;	{
;		bPredictor[i]=ReadByteData();
;		iCoef1[i] = wCoef1[bPredictor[i]];
;		iCoef2[i] = wCoef2[bPredictor[i]];
;	}

    move #>Fg_Wave_wCoef2,r1
    move #>Fg_Wave_wCoef1,r2
    move #iCoef2,r3
    do  y:<numchannels,_EndGetPredictor
        jsr ReadByteData
        move A1,n1                   ; A contains bPredictor[ch]
        move A1,n2
        move y:(r1+n1),A            ; Get wCoef2[bPredictor[ch]] in A
        move A,y:(r3)+n3            ; Save iCoef2[ch], increment to iCoef1
        move y:(r2+n2),A            ; Get wCoef1[bPredictor[ch]] in A
        move A,y:(r3)-              ; Save iCoef1[ch], decrement to iCoef2[1]
_EndGetPredictor   
        
;	for (i = 0; i < gwChannels; i++)
;		iDelta[i]=ReadSWordData();
    move #>iDelta,r1
    do  y:<numchannels,_EndGetDelta
        jsr ReadSWordData
        move A1,y:(r1)+
_EndGetDelta    
   
;	for (i = 0; i < gwChannels; i++)
;		iSamp1[i]=ReadSWordData();
    move #>iSamp1,r1
    do  y:<numchannels,_EndGetSamp1
        jsr ReadSWordData
        move A1,y:(r1)+
_EndGetSamp1
    
;	for (i = 0; i < gwChannels; i++)
;		iSamp2[i]=ReadSWordData();
    move #>iSamp2,r1
    move r1,y:<Arraybase
    do  y:<numchannels,_EndGetSamp2
        jsr ReadSWordData
        move A1,y:(r1)+
_EndGetSamp2
    
;	for (i = 0; i < gwChannels; i++)
;	{
;		OutputBuf[curptr] = iSamp2[i];
;		curptr++;
;	}

; Write the first two samples to the output buffer.
    move #>iSamp2,r1
    do  y:<numchannels,_EndWriteSamp2
        move y:(r1)+,x0
        mpy  x0,#16,A
        move A0,y:(r4)+              
_EndWriteSamp2

    move #>iSamp1,r1
    do  y:<numchannels,_EndWriteSamp1
        move y:(r1)+,x0
        mpy  x0,#16,A
        move A0,y:(r4)+
_EndWriteSamp1

    move #iDelta,r3
    move r3,y:<Arraybase2
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

    do y:<SamplestoDecode,_EndDecodeBlock
        move y:<Arraybase2,r3
        do   y:<numchannels,_EndChannelLoop
            move    y:<nibblestate,B
            move    y:<Const_000001,y0 ; Avoid overflows
            eor  y0,B   y:<byteval,A
            move B,y:<nibblestate
            move y:<TempConst_00000f,y1
            ; Check the results of xor
            jeq  _LowerNibble
            jsr  ReadByteData
            move A1,x0
            move x0,y:<byteval
            mpy x0,#4,A
_LowerNibble
            and y1,A    y:<Const_000008,y1
    ;			val -= (val & 0x8)<<1;
            and  y1,A   A1,B
            move A1,A
            asl  A
            sub  A,B    B,A                     ; A contains the index for adaption table
            ; r3 contains a pointer to iDelta
            jsr  MSADPCM_Decode_Sample
            move A,y:(r4)+
            move (r3)+                          ; Point the arraybase to the next channel
_EndChannelLoop        
            nop  
            nop            
_EndDecodeBlock        
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
;			A = Sample Decoded
;    Notes:
;           Internal function. Assumes r3 points to iDelta.
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
    asr     B	
    asr     B	
    asr     B	
    asr     B	
    asr     B	
    asr     B	
    asr     B	
    asr     B	       (r3)+n3                                 ; r3 incremented to iCoef1
    move    B0,B

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
