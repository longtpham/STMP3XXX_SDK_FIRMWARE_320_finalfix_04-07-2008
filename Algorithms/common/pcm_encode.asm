;;***********************************************************************
;SigmaTel, Inc
;$Archive: $
;$Revision:  $
;$Date: $
;Description:  Decodes an MSADPCM block of data
; 		
;		
;***********************************************************************/
    if @def('WINDOWS_PCM')
	section Encode_PCM

;*********************************************************************************
;	Include Files
;
;*********************************************************************************
	include "const.xref"
    include "wave.xref"
    include "sysmacro.asm"
;*********************************************************************************
;	Globals
;
;*********************************************************************************
    
     global Encode_PCM_Block,FEncode_PCM_Block

;*********************************************************************************
;	Externs
;
;*********************************************************************************
    
    if @def('ADPCMTEST')
    extern FSampleBuf,FBitBuf
    endif
    extern WriteByteData, WriteWordData
    extern EncByteState,Enc_Const_0000ff
    extern WriteWord24Data
 


	org y,".shortdata@Encode_PCM",OVERLAY,NEAR:
 	calls 'Fmain','Encode_PCM'


;*********************************************************************************
;		P memory 
;
;*********************************************************************************


    org p,"Encode_PCM_Block_P":   


;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  Encode_PCM_Block
;
;    Type:	Function
;
;    Description:	Encode PCM Block and places compressed data.
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

FEncode_PCM_Block
Encode_PCM_Block
    if @def('ADPCMTEST')
    ; While Testing pass r0, r4 implicitly
    ; m0 and m4 for testing are by default linear
    move #FBitBuf,r0
    move #FSampleBuf,r4
    endif

    ; Initialize data for Bitstream functions
    clr  B      #>$ff,x0
    move B,y:<EncByteState
    move x0,y:<Enc_Const_0000ff

    ; Find actual number of samples to copy
    move    A,y1
    move    y:Fg_Wave_wnSamplesPerBlock,A
    rep     y1
    asl     A       y:<Const_000008,x0
    asr     A       y:Fg_Wave_wBitsPerSample,B ; Restore the extra left shift

    ; Find number of bits per Sample and handle each of these separately
    cmp  x0,B       y:<Const_000010,x0
    jeq  _Encode8BitPCM

    cmp  x0,B       y:<Const_000018,x0
    jeq  _Encode16BitPCM

    cmp  x0,B
    jeq  _Encode24BitPCM

    ; Else Just return. Above formats are enough.
    rts    

    ; Who would want to use 8 bit PCM? It's here anyway
_Encode8BitPCM
    
    do A,_EndEncode8Block
        move y:(r4)+,x0
        move y:<Const_000080,y0
        mpy x0,#16,A
        add y0,A
        jsr WriteByteData
_EndEncode8Block
        rts

    
    
_Encode16BitPCM
    do A,_EndEncode16Block
        move y:(r4)+,x0
        mpy x0,#8,A
        jsr WriteWordData
_EndEncode16Block
        rts

_Encode24BitPCM
    do A,_EndEncode24Block
        move y:(r4)+,A
        jsr WriteWord24Data
_EndEncode24Block
        rts

        
    endsec
            

    endif
