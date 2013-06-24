;;***********************************************************************
;SigmaTel, Inc
;$Archive: $
;$Revision:  $
;$Date: $
;Description:  Decodes an IMA ADPCM block of data
; 		
;		
;***********************************************************************/
    if @def('IMA_ADPCM')
	section Decode_IMA_ADPCM

;*********************************************************************************
;	Include Files
;
;*********************************************************************************
	include "const.xref"
    include "wave.xref"
    include "imaadpcm.xref"
    include "sysmacro.asm"

;*********************************************************************************
;	Globals
;
;*********************************************************************************
    
     global Decode_IMA_ADPCM_Block,FDecode_IMA_ADPCM_Block

;*********************************************************************************
;	Externs
;
;*********************************************************************************
    
    extern ReadByteData,ReadSWordData,DecByteState,Dec_Const_0000ff
    if @def('ADPCMTEST')
    extern FBitBuf,FSampleBuf
    endif
;*********************************************************************************
;       Local Short Data Variables.
;		
;*********************************************************************************



	org y,".shortdata@Decode_IMA_ADPCM",OVERLAY,NEAR:

nibblestate             ds          1
byteval                 ds          1
nibble                  ds          1
SamplestoDecode         ds          1
numchannels             ds          1
TempConst_00000f        ds          1
TempConst_007fff        ds          1
TempConst_ff8000        ds	        1
OutputbufChannel0       ds          1


 	calls 'Fmain','Decode_IMA_ADPCM'


;*********************************************************************************
;		P memory 
;
;*********************************************************************************

    
    org p,"Decode_IMA_ADPCM_Block_P":   

;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  Decode_IMA_ADPCM_Block
;
;    Type:	Function
;
;       void Decode_IMA_ADPCM_Block(int *Output, int *input, int SamplestoDecode)
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

FDecode_IMA_ADPCM_Block
Decode_IMA_ADPCM_Block
    if @def('ADPCMTEST')
    ; While Testing pass r0, r4 implicitly
    ; m0 and m4 for testing are by default linear
    move #FBitBuf,r0
    move #FSampleBuf,r4
    endif
    ; Because one sample is available in the header, we need to decode one less.
    move y:<Const_000001,y0
    sub  y0,A       y:Fg_Wave_wChannels,y1
    move A,y:<SamplestoDecode
    clr  A  y1,y:<numchannels
    move y1,n4
    move A,y:<DecByteState
    move A,y:<nibblestate
    move y:<Const_000002,n3

    move #>$f,x0
    move x0,y:<TempConst_00000f
    move #>$7fff,x0
    move x0,y:<TempConst_007fff
    move #>$ff8000,x0
    move x0,y:<TempConst_ff8000
    move #>$ff,x0
    move x0,y:<Dec_Const_0000ff

    move #IMA_pred0,r3
    do  y:<numchannels,_EndGetBlock
        jsr ReadSWordData
        move A,x0       A,y:(r3)+                         ; Save the first sample as the predicted value.
        mpy  x0,#16,A
        move A0,y:(r4)+                         ; First sample is outputed
        jsr  ReadByteData
        move A1,y:(r3)+                          ; Store step size
        jsr  ReadByteData                       ; Dummy Read to skip the Reserved byte.
_EndGetBlock

    move y:<SamplestoDecode,A
    asr  A
    asr  A
    asr  A                  
    do A,_EndDecodeBlock
        move #IMA_index0,r3
        move r4,y:<OutputbufChannel0
        do   y:<numchannels,_EndChannelLoop
            do #8,_EndWord
                move    y:<nibblestate,B
                move    y:<Const_000001,y0 ; Avoid overflows
                move    y:<byteval,x0
                mpy  x0,#4,A
                eor  y0,B   
                move B,y:<nibblestate
                ; Check the results of xor
                jeq  _LowerNibble
                jsr  ReadByteData
                move A1,y:<byteval
_LowerNibble
                move y:<TempConst_00000f,y1
                and y1,A    
                jsr IMAADPCM_Decode_Sample       ; decode first the lower nibble
                move A,x0
                mpy  x0,#16,A
                move A0,y:(r4)+n4
_EndWord
        move y:<OutputbufChannel0,r4
        move (r3)+n3
        move (r4)+
_EndChannelLoop        
        move y:<Const_000004,A
        rep  y:<numchannels
        asl  A      y:<OutputbufChannel0,r4
        move A,n4
        nop
        move (r4)+n4
        move y:<numchannels,n4
_EndDecodeBlock        
    rts    



;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  IMAADPCM_Decode_Sample
;
;    Type:	Function
;
;       int IMAADPCM_Decode_Sample(unsigned int nibble)
;
;    Description:	Decodes an ADPCM Sample
;
;    Inputs:		
;			A = Nibble to decode
;           r3 -> Points to index value
;    Outputs:		
;			A = Sample Decoded
;    Notes:
;			Registers trashed: Assume all except r7. R0 and R4 will be updated to point
;			to point next input/output locations.
;///////////////////////////////////////////////////////////////////////////////
FIMAADPCM_Decode_Sample
IMAADPCM_Decode_Sample

;	temp = stepsizeTable[sDecode->index];
    move  y:(r3)-,y0
    move  #>IMAStepSizeTable,B
    add   y0,B      A1,y:<nibble
    move  B,r2
    move  y:<nibble,y1
    clr   A     x:(r2),B                 ; B = temp or current step
    btst  #2,y1                    ;Is (input & 4) ?
    ; If bit set then, diff = sDecode->step
    tcs   B,A
    lsr    B                       ; sDecode->step >> 1
    jclr  #1,y1,_Quant_step2        ; sDecode->step >> 1
    ; If bit set then, diff += (sDecode->step>>1)
    add    B,A
_Quant_step2
    lsr    B                       ; sDecode->step >> 2
    jclr  #0,y1,_Quant_step3        ; sDecode->step >> 2
    ; If bit set then, diff += (sDecode->step>>2)
    add    B,A
_Quant_step3
    lsr    B                       ; sDecode->step >> 3
    add    A,B  y:(r3),y0         ; diff += (sDecode->step>>3), Get the previous predicted sample.
    jclr  #3,y1,_Quant_over         ;Is input negative?
    neg    B                       ; Yes, negative, so negate the difference.
_Quant_over
    add   y0,B          y:<TempConst_007fff,y1             ; New predicted sample = prev + diff
    cmp    y1,B         y:<TempConst_ff8000,x1
    tgt    y1,B
    cmp    x1,B         #>IMAIndexTable,y0
    tlt    x1,B
    move   y:<nibble,A
    add     y0,A            B,y:(r3)+               ; New predicted sample = prev + diff
    move    A,r2                                    ; Set r2 = &indextable[encoded]
    move    y:(r3),B                                ; B = index value       
    clr     A           x:(r2),x0                   ; x0 = indextable[encoded]
    add     x0,B        #>`88,x0                    ; Get the new index, index = index + indextable[encoded]
    tlt     A,B
    cmp     x0,B        
    tgt     x0,B
    move    B,y:(r3)-
    move    y:(r3)+,A
    ; return value is in A
    rts
        
    endsec
            

    endif
