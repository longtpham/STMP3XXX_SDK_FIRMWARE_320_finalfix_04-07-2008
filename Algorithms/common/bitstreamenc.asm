;;***********************************************************************
;SigmaTel, Inc
;$Archive: $
;$Revision:  $
;$Date: $
;Description:  Bit stream parsing functions for Encoder buffers
; 		
;		
;***********************************************************************/
    section ADPCM_BitStream_Encoder

;*********************************************************************************
;	Include Files
;
;*********************************************************************************
    include "const.xref"
    include "sysmacro.asm"
;*********************************************************************************
;   Globals
;*********************************************************************************
    global WriteByteData, WriteWordData
    global EncByteState,Enc_Const_0000ff
    if @def('WINDOWS_PCM')
    global WriteWord24Data
    endif
;*********************************************************************************
;	Externs
;
;*********************************************************************************

;*********************************************************************************
;	P memory
;
;*********************************************************************************
	org y,".shortdata@ADPCM_BitStream_Encoder",OVERLAY,NEAR:

Enc_Const_0000ff        ds          1
EncByteState            ds          1

 	calls 'Encode_IMA_ADPCM','ADPCM_BitStream_Encoder'
 	calls 'Encode_MS_ADPCM','ADPCM_BitStream_Encoder'
 	calls 'Encode_PCM','ADPCM_BitStream_Encoder'

    org p,"ADPCM_BitStream_Encoder_P":

;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  WriteByteData
;
;    Type:	Function
;
;       int WriteByteData()
;
;    Description:	Writes Byte of data to the bit stream buffer.
;			Maintains internal state to track the bytes of data.
;
;    Inputs:		A1
;    Outputs:		None
;			
;    Notes:
;			Registers trashed : x0,y0,y1,A. 
;			r0 is updated if required.
;///////////////////////////////////////////////////////////////////////////////

WriteByteData
    move   y:<Enc_Const_0000ff,x0
    and    x0,A
    move   A1,y0
    move  y:<EncByteState,A
    tst   A        y:<Const_000001,y1
    jeq   _Bytezero
    cmp   y1,A  y:<Const_000002,y1
    jeq   _Byteone
    mpy   y0,#8,A
    move  A0,y0
    move  x:(r0),A  
    or    y0,A      #0,y1
    move  A1,x:(r0)+
    move  y1,y:<EncByteState
    rts        

         
       
_Bytezero
    move  y1,y:<EncByteState
    move  y0,x:(r0)                             ; Go to Next word.
    rts

        
_Byteone        
    move  x:(r0),x0
    mpy   y0,#16,A
    move  A0,A
    or    x0,A      y1,y:<EncByteState
    move  A1,x:(r0)
    rts        


;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  WriteWordData
;
;    Type:	Function
;
;       int WriteWordData()
;
;    Description:	Writes Word of data to the bit stream buffer.
;			Maintains internal state to track the bytes of data.
;
;    Inputs:		A
;    Outputs:		None
;			
;    Notes:
;			Registers trashed : x0,y0,y1,A. 
;			r0 is updated if required.
;///////////////////////////////////////////////////////////////////////////////
WriteWordData
    move   A,y0
    mpy    y0,#8,B      
    jsr    WriteByteData
    move   B1,A
    jsr    WriteByteData
    rts

;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  WriteWord24Data
;
;    Type:	Function
;
;       int WriteWord24Data()
;
;    Description:	Writes Word of data to the bit stream buffer.
;			Maintains internal state to track the bytes of data.
;
;    Inputs:		A
;    Outputs:		None
;			
;    Notes:
;			Registers trashed : x0,y0,y1,A. 
;			r0 is updated if required.
;///////////////////////////////////////////////////////////////////////////////
    if @def('WINDOWS_PCM')
WriteWord24Data
    move   A,y0
    mpy    y0,#8,B      
    jsr    WriteByteData
    move   B1,A
    jsr    WriteByteData
    move   B1,y0
    mpy    y0,#8,A
    jsr    WriteByteData
    rts
    endif

    endsec
