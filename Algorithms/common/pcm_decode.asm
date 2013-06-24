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
	section Decode_PCM

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
    
     global Decode_PCM_Block,FDecode_PCM_Block

;*********************************************************************************
;	Externs
;
;*********************************************************************************
    
    extern ReadByteData,ReadSWordData,DecByteState,Dec_Const_0000ff
    extern ReadS24WordData

    if @def('ADPCMTEST')
    extern FSampleBuf,FBitBuf
    endif

	org y,".shortdata@Decode_PCM",OVERLAY,NEAR:
 	calls 'Fmain','Decode_PCM'

;*********************************************************************************
;		P memory 
;
;*********************************************************************************

    org p,"Decode_PCM_Block_P":   

;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  Decode_PCM_Block
;
;    Type:	Function
;
;       void Decode_PCM_Block(int *Output, int *input, int SamplestoDecode)
;
;    Description:	Decodes a PCM Block
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

FDecode_PCM_Block
Decode_PCM_Block
    if @def('ADPCMTEST')
    ; While Testing pass r0, r4 implicitly
    ; m0 and m4 for testing are by default linear
    move #FBitBuf,r0
    move #FSampleBuf,r4
    endif

    ; Initialize data for Bitstream functions
    clr  B      #>$ff,x0
    move B,y:<DecByteState
    move x0,y:<Dec_Const_0000ff

    ; Find actual number of samples to copy
    move    y:Fg_Wave_wChannels,y1
    rep     y1
    asl     A       y:<Const_000008,x0
    asr     A       y:Fg_Wave_wBitsPerSample,B ; Restore the extra left shift

    ; Find number of bits per Sample and handle each of these separately
    cmp  x0,B       y:<Const_000010,x0
    jeq  _Decode8BitPCM

    cmp  x0,B       y:<Const_000018,x0
    jeq  _Decode16BitPCM

    cmp  x0,B
    jeq  _Decode24BitPCM

    ; Else Just return. Above formats are enough.
    rts    

    ; Who would ever want to use 8 bit PCM? It's here anyway
_Decode8BitPCM
    do A,_EndDecode8Block
        jsr ReadByteData
        move A1,x0
        move y:<Const_000080,A
        sub  x0,A       
        move A,x0
        mpy  x0,#8,A
        move A0,y:(r4)+ 
_EndDecode8Block
        rts

_Decode16BitPCM
    do A,_EndDecode16Block
        jsr ReadSWordData
        move A1,x0
        mpy  x0,#16,A
        move A0,y:(r4)+    
_EndDecode16Block
        rts

_Decode24BitPCM
    do A,_EndDecode24Block
        jsr ReadS24WordData
        move A1,y:(r4)+    
_EndDecode24Block
        rts
    

        
    endsec
            

    endif
