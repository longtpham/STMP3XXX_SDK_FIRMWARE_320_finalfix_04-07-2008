;;***********************************************************************
;SigmaTel, Inc
;$Archive: $
;$Revision:  $
;$Date: $
;Description:  Bit stream parsing and I/O functions
; 		
;		
;***********************************************************************/
    section SMVADPCM_BitStream

;*********************************************************************************
;	Include Files
;
;*********************************************************************************
    include "const.xref"
    include "sysmacro.asm"
    include "filesystem.inc"
;*********************************************************************************
;	Globals
;
;*********************************************************************************
    global SmvGetByte, SmvGetWord, SmvGetDWord, SmvWriteByte, SmvWriteWord, SmvWriteDWord
    global SmvGetSWord
    global FSmv_Wave_Fill_Input_Bitstream_Buffer,FSmv_Wave_Flush_Encoded_Bitstream_Buffer
    global SmvGet_file_offset,SmvSeek_data_offset,SmvSeek_to_zero,SmvSkipData,smvwave_file_offset
;*********************************************************************************
;	Externs
;
;*********************************************************************************
	extern AdpcmEncFileHandle,AdpcmDecoderFileHandle
	extern FileRead,FileWrite,FileSeek,FFseek
    extern SysAllLinear
;*********************************************************************************
;	X memory
;
;*********************************************************************************
    org y,"SMV_Bitstream_Wave_Y":
TempArr         ds          2
smvwave_file_offset     ds          1
;*********************************************************************************
;	P memory
;
;*********************************************************************************
    org p,"SMV_ADPCM_Wave_BitStream_P":

;///////////////////////////////////////////////////////////////////////////////

; The following API functions are non-real time and can be invoked to
; write to the file, as the data is made available. Typically, these
; functions operate at the user or messaging level. These are used prior
; to real-time encoding or decoding.

;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  SmvWriteWord
;
;    Type:	Function
;
;       void SmvWriteWord
;
;    Description:	
;			Writes Word of Data to the bitstream buffer
;
;    Inputs:		A
;    Outputs:		
;			None
;    Notes:
;///////////////////////////////////////////////////////////////////////////////
SmvWriteWord
    move    A,y:TempArr
    move	y:AdpcmEncFileHandle,a2		
    move	#TempArr,r0                               ; Setup buffer pointer
    move	y:<Const_000002,a1                      ; Number bytes to write
    move	#$400000,a0                             ; Buffer memory space is in Y
    jsr     SysAllLinear
    jsr		FileWrite
    rts


;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  SmvWriteByte
;
;    Type:	Function
;
;       void SmvWriteByte
;
;    Description:	
;			Writes Byte of Data to the bitstream buffer
;
;    Inputs:		A
;    Outputs:		
;			None
;    Notes:
;///////////////////////////////////////////////////////////////////////////////

SmvWriteByte
    move    A,y:TempArr
    move	y:AdpcmEncFileHandle,a2		
    move	#TempArr,r0                               ; Setup buffer pointer
    move	y:<Const_000001,a1                      ; Number bytes to write
    move	#$400000,a0                             ; Buffer memory space is in Y
    jsr     SysAllLinear
    jsr		FileWrite
    rts


;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  SmvWriteDWord
;
;    Type:	Function
;
;       void SmvWriteDWord
;
;    Description:	
;			Writes 4 Bytes of Data to the bitstream buffer
;
;    Inputs:		A1:A0
;    Outputs:		
;			None
;    Notes:
;///////////////////////////////////////////////////////////////////////////////

SmvWriteDWord
    move    A0,y:TempArr
    move    A1,y:(TempArr+1)
    move	y:AdpcmEncFileHandle,a2		
    move	#TempArr,r0                               ; Setup buffer pointer
    move	y:<Const_000004,a1                      ; Number bytes to write
    move	#$400000,a0                             ; Buffer memory space is in Y
    jsr     SysAllLinear
    jsr		FileWrite
    rts



;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  SmvGetByte
;
;    Type:	Function
;
;       void SmvGetByte
;
;    Description:	
;		   Gets a Byte of Data from the file
;
;    Inputs:	
;    Outputs:		
;		   A contains the byte of data
;    Notes:
;///////////////////////////////////////////////////////////////////////////////

SmvGetByte
    move    y:smvwave_file_offset,A0
    move    y:(smvwave_file_offset+1),A1
	inc		A
    move    A0,y:smvwave_file_offset

	clr		B		A1,y:(smvwave_file_offset+1)
	move	B,y:TempArr
    move	y:AdpcmDecoderFileHandle,a2		
    move	#TempArr,r0                               ; Setup buffer pointer
    move	y:<Const_000001,a1                      ; Number bytes to write
    move	#$400000,a0                             ; Buffer memory space is in Y
    jsr     SysAllLinear
    jsr		FileRead
    move    y:TempArr,A
    rts

;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  SmvGetWord
;
;    Type:	Function
;
;       void SmvGetWord
;
;    Description:	
;		   Gets an unsigned Word of Data from the file
;
;    Inputs:	
;    Outputs:		
;		   A
;    Notes:
;///////////////////////////////////////////////////////////////////////////////

SmvGetWord
    move    y:smvwave_file_offset,A0
    move    y:(smvwave_file_offset+1),A1
	inc		A
	inc		A
    move    A0,y:smvwave_file_offset
	clr		B    A1,y:(smvwave_file_offset+1)

	move	B,y:TempArr
    move	y:AdpcmDecoderFileHandle,a2		
    move	#TempArr,r0                               ; Setup buffer pointer
    move	y:<Const_000002,a1                      ; Number bytes to write
    move	#$400000,a0                             ; Buffer memory space is in Y
    jsr     SysAllLinear
    jsr		FileRead
    move    y:TempArr,A
    rts

;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  SmvGetSWord
;
;    Type:	Function
;
;       void SmvGetSWord
;
;    Description:	
;		   Gets a signed Word of Data from the file
;
;    Inputs:	
;    Outputs:		
;		   A
;    Notes:
;///////////////////////////////////////////////////////////////////////////////

SmvGetSWord
    move    y:smvwave_file_offset,A0
    move    y:(smvwave_file_offset+1),A1
	inc		A
	inc		A
    move    A0,y:smvwave_file_offset
	clr		B    A1,y:(smvwave_file_offset+1)
	move	B,y:TempArr

    move	y:AdpcmDecoderFileHandle,a2		
    move	#TempArr,r0                               ; Setup buffer pointer
    move	y:<Const_000002,a1                      ; Number bytes to write
    move	#$400000,a0                             ; Buffer memory space is in Y
    jsr		FileRead
    move    y:<Const_008000,x0
    move    y:TempArr,B
    and     x0,B    B,A
    asl     B
    sub     B,A
    rts


;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  SmvGetDWord
;
;    Type:	Function
;
;       void SmvGetDWord
;
;    Description:	
;		   Gets an unsigned DWord (4 bytes) of Data from the file
;
;    Inputs:	
;    Outputs:		
;		   A
;    Notes:
;///////////////////////////////////////////////////////////////////////////////

SmvGetDWord
    move    y:smvwave_file_offset,A0
    move    y:(smvwave_file_offset+1),A1
	inc		A
	inc		A
	inc		A
	inc		A
    move    A0,y:smvwave_file_offset
	clr		B    A1,y:(smvwave_file_offset+1)
	move	B,y:(TempArr+1)

    move	y:AdpcmDecoderFileHandle,a2		
    move	#TempArr,r0                               ; Setup buffer pointer
    move	y:<Const_000004,a1                      ; Number bytes to write
    move	#$400000,a0                             ; Buffer memory space is in Y
    jsr     SysAllLinear
    jsr		FileRead
    move    y:(TempArr+1),A
    move    y:TempArr,A0
    rts


;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  Smv_Wave_Fill_Input_Bitstream_Buffer
;
;    Type:	Function
;
;       int Smv_Wave_Fill_Input_Bitstream_Buffer(int size)
;
;    Description:	
;       Fills the input bitstream buffer
;
;    Inputs:       A contains the size in bytes
;    Outputs:	   r0: Ptr to X memory location	
;		   A contains the size of data actually read in
;    Notes:
;///////////////////////////////////////////////////////////////////////////////

FSmv_Wave_Fill_Input_Bitstream_Buffer
Smv_Wave_Fill_Input_Bitstream_Buffer
    move  A,x0
    move	y:AdpcmDecoderFileHandle,a2		
    move	x0,a1                                   ; Number bytes to write
    move	#$800000,a0                             ; Buffer memory space is X
    jsr     SysAllLinear
    jsr		FileRead
    move    A1,A                                    ; Returned data size
    rts


;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  Smv_Wave_Flush_Encoded_Bitstream_Buffer
;
;    Type:	Function
;
;       void Wave_Flush_Output_Bitstream_Buffer(void)
;
;    Description:	
;       Flushes the encoded bitstream buffer
;
;    Inputs:    A contains number of Bytes to be written.
;				r0: Ptr to X memory buffer
;    Outputs:	None	
;    Notes:
;///////////////////////////////////////////////////////////////////////////////

FSmv_Wave_Flush_Encoded_Bitstream_Buffer
Smv_Wave_Flush_Encoded_Bitstream_Buffer
    move	y:AdpcmEncFileHandle,a2		
    move	#$800000,a0                             ; Buffer memory space is in X
    jsr     SysAllLinear
    jsr		FileWrite
    rts


; The following two functions will seek to the data chunk for raw decode.
;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  SmvGet_file_offset
;
;    Type:	Function
;
;           int SmvGet_file_offset
;
;    Description:	
;       Gets the current offset of the file being decoded
;
;    Inputs:    None
;    Outputs:	
;    Notes: Will return 0x7fffff, if exceeds 0x7fffff
;///////////////////////////////////////////////////////////////////////////////
SmvGet_file_offset
	clr	 A
    move y:smvwave_file_offset,A0
    move y:(smvwave_file_offset+1),A1
    rts


;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  SmvSeek_data_offset
;
;    Type:	Function
;
;           void SmvSeek_data_offset(int offset)
;
;    Description:	
;           Used to locate the data chunk after parsing.
;
;    Inputs:    A contains the offset.
;    Outputs:	None
;    Notes: Data chunk should be located early on.
;///////////////////////////////////////////////////////////////////////////////
SmvSeek_data_offset
    push    A1
	push	A0
	move	A0,y:smvwave_file_offset
    ; First seek to the beginning of the file.
    clr A	A1,y:(smvwave_file_offset+1)
    move    y:AdpcmDecoderFileHandle,a2
    jsr     SysAllLinear
    jsr     FileSeek
    jcc     _AtFileBegin
    error
_AtFileBegin
	pop		A0
	pop		A1
    move    y:AdpcmDecoderFileHandle,a2
    jsr     SysAllLinear
    jsr     FileSeek
    jcc     _Done
    error
_Done
    rts


;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  SmvSeek_to_zero
;
;    Type:	Function
;
;           void SmvSeek_to_zero(void)
;
;    Description:	
;           Used to seek to the beginning to rewrite the header
;
;    Inputs:    None
;    Outputs:	None
;    Notes:     Used by the encoder
;///////////////////////////////////////////////////////////////////////////////
SmvSeek_to_zero
    jsr     SysAllLinear
    clr A
    move    y:AdpcmEncFileHandle,a2
    jsr     FileSeek
    jcc     _AtFileBegin
    error
_AtFileBegin
    rts


;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  SmvSkipData
;
;    Type:	Function
;
;           void SmvSkipData(long amt)
;
;    Description:	
;           Used to skip unknown chunks
;
;    Inputs:    A1:A0: amount to skip
;    Outputs:	None
;    Notes:     Used by the decoder to skip unknown chunks
;///////////////////////////////////////////////////////////////////////////////

SmvSkipData
    move    y:(smvwave_file_offset+1),B
    move    y:smvwave_file_offset,B0
    add     A,B
    move    B0,y:smvwave_file_offset
    move    B1,y:(smvwave_file_offset+1)
    move	y:AdpcmDecoderFileHandle,B
    move    #>SEEK_CUR,x0
    jsr     SysAllLinear
    jsr     FFseek                      ; Call FAT32 API directly.
	; Don't check whether we skip to end of file.. which we will do for the last chunk.
_Done
    rts

	endsec
