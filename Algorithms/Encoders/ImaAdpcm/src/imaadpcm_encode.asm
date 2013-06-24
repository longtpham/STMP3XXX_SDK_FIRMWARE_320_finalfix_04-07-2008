;;***********************************************************************
;SigmaTel, Inc
;$Archive: $
;$Revision:  $
;$Date: $
;Description:  Encodes an IMAADPCM block of data
; 		
;		
;***********************************************************************/
    if @def('IMA_ADPCM')
	section Encode_IMA_ADPCM

;*********************************************************************************
;	Include Files
;
;*********************************************************************************
	include "const.xref"
    include "wave.xref"
    include "imaadpcm.xref"

;*********************************************************************************
;	Globals
;
;*********************************************************************************
    
     global Encode_IMA_ADPCM_Block,FEncode_IMA_ADPCM_Block

;*********************************************************************************
;	Externs
;
;*********************************************************************************
    
    extern WriteByteData,WriteWordData,EncByteState,Enc_Const_0000ff
    if @def('ADPCMTEST')
    extern FBitBuf,FSampleBuf
    endif
;*********************************************************************************
;       Local Short Data Variables.
;		
;*********************************************************************************



	org y,".shortdata@Encode_IMA_ADPCM",OVERLAY,NEAR:

Encoded                 ds          1
encodedbyte             ds          1
nibblestate             ds          1
byteval                 ds          1
numchannels             ds          1
TempConst_00000f        ds          1
TempConst_007fff        ds          1
TempConst_ff8000        ds	        1
ReadPtrChannel0         ds          1
InputSample             ds          1
 	calls 'Fmain','Encode_IMA_ADPCM'

;*********************************************************************************
;		P memory 
;
;*********************************************************************************

    org p,"Encode_IMA_ADPCM_Block_P":   

;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  Encode_IMA_ADPCM_Block
;
;    Type:	Function
;           Encode_IMA_ADPCM_Block(NumChannels,Outputbuf,InputBuf)
;    Description:	Encodes IMA ADPCM block and places compressed data.
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

FEncode_IMA_ADPCM_Block
Encode_IMA_ADPCM_Block
    if @def('ADPCMTEST')
    ; While Testing pass r0, r4 implicitly
    ; m0 and m4 for testing are by default linear
    move #FBitBuf,r0
    move #FSampleBuf,r4
    endif
    move #>$f,x0
    move x0,y:<TempConst_00000f
    move #>$ff,x0
    move x0,y:<Enc_Const_0000ff
    move #>$7fff,x0
    move x0,y:<TempConst_007fff
    move #>$ff8000,x0
    move x0,y:<TempConst_ff8000
    move y:<Const_000002,n5
    move A,n4		
    move A,y:<numchannels
    clr  A      #IMA_pred0,r3
    move A,y:<EncByteState
    do  y:<numchannels,_WriteBlockHeader
        move y:(r4)+,x0
        mpy  x0,#8,A
        move A,y:(r3)+
        jsr  WriteWordData
        move y:(r3)+,A
        jsr  WriteByteData
        ; Write the reserved word
        clr  A
        jsr  WriteByteData
_WriteBlockHeader

	clr  A		y:Fg_Wave_wnSamplesPerBlock,B
	move y:<Const_000001,x0
	sub	 x0,B	A,y:<nibblestate
    asr  B                                 ; Encode 8 samples at a time.
    asr  B
    asr  B
    do B,_EndEncodeBlock
        move #IMA_pred0,r5
        move r4,y:<ReadPtrChannel0
        do   y:<numchannels,_EndChannelLoop
            do   #8,_EndWordLoop
                move  y:(r4)+n4,x0
                mpy   x0,#8,A
                jsr   IMAADPCM_Encode_Sample
                move  y:<nibblestate,B
                move  y:<Const_000001,x0
                eor   x0,B      y:<Encoded,x0
                move  B,y:<nibblestate
                move  y:<encodedbyte,A
                move  x0,y:<encodedbyte
                jne   _Continue3
                mpy   x0,#20,B
                move  y:<TempConst_00000f,x0
                and   x0,A      B0,x0
                or    x0,A
                move  A1,A
                jsr   WriteByteData
_Continue3  
                nop
_EndWordLoop
                move y:<ReadPtrChannel0,r4 
                move (r5)+n5            ; Advance to next channel
                move (r4)+
_EndChannelLoop
        move y:<Const_000004,A
        rep  y:<numchannels
        asl  A      y:<ReadPtrChannel0,r4
        move A,n4
        nop
        move (r4)+n4
        move y:<numchannels,n4
_EndEncodeBlock
    rts     
        


;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  IMAADPCM_Encode_Sample
;
;    Type:	Function
;
;       int IMAADPCM_Encode_Sample(int16 sample)
;
;    Description:	Encodes an ADPCM Sample
;
;    Inputs:		
;			A = Sample to be encoded
;           r3 -> Points to index value
;           r5 -> ptr to pSadpcmencode Structure block.
;    Outputs:		
;			A = Compressed nibble
;    Notes:
;			Registers trashed: Assume all except r7. R0 and R4 will be updated to point
;			to point next input/output locations.
;///////////////////////////////////////////////////////////////////////////////
IMAADPCM_Encode_Sample
    move    A,y:<InputSample
    move    y:(r5)+,y0                              ; y0 = pred
    move    y:(r5)-,x0                              ; x0 = index
    move    #>IMAStepSizeTable,B
    add     x0,B    #0,A0
    clr     B       B,r3                            ; r3 points to step

    sub     y0,A    y:<Const_000008,y1              ; A = diff = input - pred, y1 = encoded = 0
    tlt     y1,B
    abs     A       B,y:<Encoded
    move    x:(r3),B                                ; B = stepsize
    cmp     B,A                                     ; ? (diff >= stepsize), 
    jmi     _Next_Quant_step1
    sub     B,A                                     ; diff = diff - step_size
    bset    #2,y:<Encoded                           ; encoded = encoded | 4
_Next_Quant_step1
    lsr     B                                       ; step = step >> 1
    cmp     B,A                                     ; ? (diff >= stepsize), 
    jmi     _Next_Quant_step2
    sub     B,A                                     ; diff = diff - step_size
    bset    #1,y:<Encoded                               ; encoded = encoded | 2
_Next_Quant_step2
    lsr     B                                       ; step = step >> 1
    cmp     B,A                                     ; ? (diff >= stepsize), 
    jmi     _Quant_over
    sub     B,A                                     ; diff = diff - step_size
    bset    #0,y:<Encoded                               ; encoded = encoded | 1

_Quant_over:    
    lsr     B
    sub     A,B     y:<InputSample,y0                   ; Get the input in y0
    jset    #3,y:<Encoded,_Negative_slope               ; ? encoded & 8 then -ve slope
    add     y0,B    y:<TempConst_007fff,y1
    jmp     _Store_predicted_val
_Negative_slope
    sub     y0,B    y:<TempConst_007fff,y1
    neg     B
_Store_predicted_val:
    ; Saturate the predicted value
    cmp    y1,B         y:<TempConst_ff8000,x1
    tgt    y1,B
    cmp    x1,B         y:<Encoded,A
    tlt    x1,B
    move    B,y:(r5)+                               ; Store the predicted value.


    ; Pack the encoded value here
    ; Get stepsize
    move    #>IMAIndexTable,y0
    add     y0,A        
    move    A,r2                                    ; Set r2 = &indextable[encoded]
    move    y:(r5),B                                ; B = index value       
    clr     A           x:(r2),x0                   ; x0 = indextable[encoded]
    add     x0,B        #>`88,x0                    ; Get the new index, index = index + indextable[encoded]
    tlt     A,B
    cmp     x0,B        y:<Encoded,A
    tgt     x0,B
    move    B,y:(r5)-
    ; return value is in A
    rts

    endsec
    
    endif
