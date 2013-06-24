;;***********************************************************************
;SigmaTel, Inc
;$Archive: $
;$Revision:  $
;$Date: $
;Description:  Bit stream parsing functions for Decoder buffers
; 		
;		
;***********************************************************************/

    section SMVADPCM_BitStream_Decoder

;*********************************************************************************
;	Include Files
;
;*********************************************************************************
    include "const.xref"
    include "sysmacro.asm"
;*********************************************************************************
;	External Definitions
;*********************************************************************************

    global SMVReadByteData,SMVReadSWordData
    global SMVDecByteState,SMVDec_Const_0000ff
    if @def('WINDOWS_PCM')
    global SMVReadS24WordData
    endif

;*********************************************************************************
;	External References
;*********************************************************************************

	org y,".shortdata@SMVADPCM_BitStream_Decoder",OVERLAY,NEAR:

SMVDec_Const_0000ff     ds          1
SMVDecByteState         ds          1

 	calls 'Decode_IMA_ADPCM','SMVADPCM_BitStream_Decoder'
 	calls 'Decode_MS_ADPCM','SMVADPCM_BitStream_Decoder'
 	calls 'Decode_PCM','SMVADPCM_BitStream_Decoder'

;*********************************************************************************
;	P memory
;*********************************************************************************
    org p,"SMVADPCM_BitStream_Decoder_P":

;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  SMVReadByteData
;
;    Type:	Function
;
;       int SMVReadByteData()
;
;    Description:	Reads Byte of data from input stream buffer.
;			Maintains internal state to track the bytes of data.
;
;    Inputs:		Implicit
;    Outputs:		
;			A1: Contains the extracted byte
;    Notes:
;			Registers trashed : x0,y0,A. 
;			r0 is updated if required.
;///////////////////////////////////////////////////////////////////////////////

SMVReadByteData
    move  y:<SMVDec_Const_0000ff,y0
    move  y:<SMVDecByteState,A
    tst   A     y:<Const_000001,x0
    jeq   _Bytezero
    cmp   x0,A  y:<Const_000002,x0
    jeq   _ByteOne
    move  x0,y:<SMVDecByteState
    move  x:(r0)+,x0
    mpy   x0,#16,A
    and   y0,A  #0,y0
    move  y0,y:<SMVDecByteState
    rts        
         
        
_Bytezero
    move  x:(r0),A
    and   y0,A      x0,y:<SMVDecByteState
    rts

        
_ByteOne        
    move  x0,y:<SMVDecByteState
    move  x:(r0),x0
    mpy   x0,#8,A
    and   y0,A  
    rts        


;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  SMVReadSWordData
;
;    Type:	Function
;
;       int SMVReadSWordData()
;
;    Description: Reads a signed word of data from input stream buffer.
;			Maintains internal state to track the bytes of data.
;
;    Inputs:		Implicit
;    Outputs:		
;			A: Contains the extracted signed word
;    Notes:
;			Registers trashed : x0,y0,A. 
;			r0 is updated if required.
;///////////////////////////////////////////////////////////////////////////////
SMVReadSWordData
    jsr  SMVReadByteData
    move A1,B
    jsr  SMVReadByteData
    move A1,x0
    mpy  x0,#16,A
    move A0,x0
    or   x0,B       y:<Const_008000,x0
    and  x0,B       B,A
    asl  B
    sub  B,A
    rts

;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  SMVReadS24WordData
;
;    Type:	Function
;
;       int SMVReadS24WordData()
;
;    Description: Reads a signed word of data from input stream buffer.
;			Maintains internal state to track the bytes of data.
;
;    Inputs:		Implicit
;    Outputs:		
;			A: Contains the extracted signed word
;    Notes:
;			Registers trashed : x0,y0,A. 
;			r0 is updated if required.
;///////////////////////////////////////////////////////////////////////////////
    if @def('WINDOWS_PCM')
SMVReadS24WordData
    jsr  SMVReadByteData
    move A1,B
    jsr  SMVReadByteData
    move A1,x0
    mpy  x0,#16,A
    move A0,x0
    or   x0,B       
    jsr  SMVReadByteData
    move A1,x0
    mpy  x0,#8,A
    move A0,x0
    or   x0,B       
    move B1,A           
    rts
    endif


    endsec
