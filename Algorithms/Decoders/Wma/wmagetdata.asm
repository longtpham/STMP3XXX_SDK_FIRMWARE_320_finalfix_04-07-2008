;;***********************************************************************
;SigmaTel, Inc
;$Archive: $
;$Revision:  $
;$Date: $
;Description: Call back function for FileRead.
;   Equivalent C code is available at the end of file.
;***********************************************************************/

    section WmaGetData

;********************************************************************************
;*	INCLUDES
;********************************************************************************
	include "sysmacro.asm"
	include "const.xref"	

;********************************************************************************
;*	DEFINES
;********************************************************************************
INBUFSIZE                       equ     96  ; INBUFSIZE*3 bytes.
INBUF2SIZE                       equ     6  ; INBUFSIZE*3 bytes.
WMA_MAX_DATA_REQUESTED			equ		128
STRINGSIZE						equ 	128

;********************************************************************************
;*	GLOBALs
;********************************************************************************
	global streaming_started
	global Fstreaming_started
	global last_offset
	global WMA_File_Offset
	global pTitle,pAuthor,pCopyright,pDescription,pRating
	global FpTitle,FpAuthor,FpCopyright,FpDescription,FpRating
    global WMAFileCBGetData,Gpbuffer,WMAFileCBGetData2
;********************************************************************************
;*	EXTERNs
;********************************************************************************

    extern SaveSystemStack,RestoreSystemStack
    extern FileSeek,FileRead,WmaDecoderFileHandle,WmaDecoderFileHandle2

;********************************************************************************
;*	 Y memory
;********************************************************************************

	 org y,".InBuf":
Finbuf							ds		INBUFSIZE
Finbuf2                         ds      INBUF2SIZE

	org y,"WMA_Y_mem":
        buffer  M,3
ByteState
								dc		0
								dc		$8000
								dc		$80
        endbuf
streaming_started				dc		0
BufRead							dc		0
read_offset						ds		1
read_reqd_flag					ds		1
Fstreaming_started
num_bytes_wanted				ds		1
totalRead						ds		1
wmadecsp	   					ds		1


	org x,".OtherInputData":
Gpbuffer                        ds      WMA_MAX_DATA_REQUESTED
    ; Other Global data.
	org x,".OtherInputData_ovl":
FpTitle
pTitle		 					ds 		STRINGSIZE
FpAuthor
pAuthor      					ds 		STRINGSIZE
FpCopyright
pCopyright   					ds 		STRINGSIZE
FpDescription
pDescription 					ds 		STRINGSIZE
FpRating
pRating      					ds 		STRINGSIZE


;********************************************************************************
;*	 L memory
;********************************************************************************
	org L,"WMA_LongVariables":
last_offset						dc		0
WMA_File_Offset					dc		0



;********************************************************************************
;*	 Code
;********************************************************************************

    org p,".pCommon":
;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  WMAFileCBGetData
;
;    Type:    Function
;           
;    Description:    
;   This routine reads only when required. Typical buffer size is 96 words, 
;   but any optimum size as required by the file system can be selected.
;   This is defined by INBUFSIZE. 
;   During initialization phase, we allow excess file reads and seeks, but no real-time
;   requirements exist.
;   Once streaming is started, the file reads are minimized. File seeks rarely occur.
;   This ensures that file-system is minimally loaded and the function performs optimally.
;	Data stored in g_pBuffer[128] in char format.
;
;    Inputs:        
;	    x1:x0 = offset
;	    A = num_bytes
;    Outputs:        
;       r0 : ptr to the buffer
;       A = num bytes read.
;    Notes:
;       All trashed registers are backed up and restored. The file-system trashes these.
;///////////////////////////////////////////////////////////////////////////////

WMAFileCBGetData
	push	r1
	push	r2
	push	r3
	push	r4
	push 	r5
	push	r6

	push	ssh

	push	n0
	push	n1
	push	n2
	push	n3
	push	n4
	push 	n5
	push	n6

	push 	x0
	push	x1
	push 	y0
	push	y1

	push	B0
	push 	B1
	push	B2
	push 	m0

	move	#>WMA_MAX_DATA_REQUESTED,y0
	cmp		y0,A		x,L:WMA_File_Offset
	tgt		y0,A
	move	A,y0				   				 	; y0 = nWanted


	clr	A
	move	A,y:totalRead
;	if (streaming_started == 1)
	move	y:streaming_started,A  				 	; minimize the FILE read calls during streaming.	
	tst	A			y0,y:num_bytes_wanted
	jeq	NoStreaming

ReadBuf
;       if (offset > last_offset)
	move	L:last_offset,A
	sub	x,A			#0,y1 					 	; last_offset - offset
	neg	A			   							 	; offset - last_offset
	jlt	NoStreaming	   							 	
	move	A0,y0	   							 	
        clr     A   A1,y1                                   ; y1:y0 = read_offset.
	move	y:BufRead,A0
	sub	y,A	
	tst	A			#0,n1					 	; A = navailable.
	jlt	NoStreamingCheck						 	; Hit, during first streaming frame.

    ; If we hit here, we are within a narrow range of INPUTBUFSIZE
    move    A0,A
    move    y0,x0
    move    y:num_bytes_wanted,y0

; 	x0 = read_offset, A = navailable, y0 = nWanted.
    
	cmp		y0,A 
	tge		y0,A
	tst		A			A,y1 					 	; is available data zero? Skip GetData, if yes.
	jeq	_ContinueRead

; 	reqd = y1, x0 = Read_offset, n1 = index.
    move #>Finbuf,r4
	jsr	GetData
_ContinueRead
	move	y:num_bytes_wanted,A
	sub		y1,A						
	jeq		exitGetData

	push	A1										; reqd
	push	y1										; navailable

    move    #_stackSaved,r6          				; setup the return address
    move    #wmadecsp,r5             				; pointer to the SP local save
    jmp     SaveSystemStack          				; save the hardware stack
_stackSaved
;   ret = fread (&inbuf[0], INBUFSIZE, 1, g_fp);
        move    y:WmaDecoderFileHandle,a2
	move	#>INBUFSIZE*3,A1						; Always read data of size INBUFSIZE. Data passed is bytes
	move	y:<Const_400000,A0						; X memory
	move 	#Finbuf,r0
	jsr	FileRead
	move	#>INBUFSIZE*3,A1
	move	A1,y:BufRead
    move    #wmadecsp,r5             				; pointer to the SP local save
    move    #_stackRestored,r6       				; setup the return address
    jmp     RestoreSystemStack       				; restore the hardware stack
_stackRestored

	clr		B			L:last_offset,x
	move	y:BufRead,B0							; A1 contains bytes read.
	add		x,B			
	move	B,L:last_offset

	pop		n1		
	pop		y1
	move	y:BufRead,A									; A = BufRead
	cmp		y1,A		#0,x0
	tgt		y1,A
	move	A,y1
; 	reqd = y1, x0 = Read_offset, x1 = index.
    move    #>Finbuf,r4
	jsr		GetData
	
	jmp		exitGetData

NoStreamingCheck

	move  #0,y1
	neg   A		#>INBUFSIZE*3,y0
	sub   y,A
	jgt   NoStreaming


    move    #_stackSaved,r6          				; setup the return address
    move    #wmadecsp,r5             				; pointer to the SP local save
    jmp     SaveSystemStack          				; save the hardware stack
_stackSaved


;   ret = fread (&inbuf[0], INBUFSIZE, 1, g_fp);
        move    y:WmaDecoderFileHandle,a2
	move	#>INBUFSIZE*3,A1						; Always read data of size INBUFSIZE. Data passed is bytes
	move	y:<Const_400000,A0						; X memory
	move 	#Finbuf,r0
	jsr	FileRead
	
	
	move	#>INBUFSIZE*3,A1
	move	A1,y:BufRead
    move    #wmadecsp,r5             				; pointer to the SP local save
    move    #_stackRestored,r6       				; setup the return address
    jmp     RestoreSystemStack       				; restore the hardware stack
_stackRestored

	clr		B			L:last_offset,x
	move	y:BufRead,B0							; A1 contains bytes read.
	add		x,B			
	move	B,L:last_offset
	move   L:WMA_File_Offset,x
	jmp    ReadBuf



NoStreaming
; 	last_offset = offset;
	move 	y:<Const_000001,A
	move 	A,y:streaming_started
	move	x0,y0									; x0 destroyed in stack save, below

    move    #_stackSaved,r6 						; setup the return address
    move    #wmadecsp,r5    						; pointer to the SP local save
    jmp     SaveSystemStack 						; save the hardware stack
_stackSaved

;	push 	y0
;	push	x1
;; 	The file system does not support SEEK_SET. So seek to zero first
;	clr		A
;    move    y:WmaDecoderFileHandle,a2
;  	jsr		FileSeek
;	pop		A1
;	pop		A0
	clr		A
    move    x1,A1
    move    y0,A0
	move	A,L:last_offset
	
;	fseek(g_fp, offset, SEEK_SET);
        move    y:WmaDecoderFileHandle,a2
  	jsr		FileSeek
;	ret = fread (&inbuf[0], INBUFSIZE, 1, g_fp);
        move    y:WmaDecoderFileHandle,a2
	move	#>INBUFSIZE*3,A1	  					; Always read data of size INBUFSIZE. Data passed is bytes!
	move	y:<Const_400000,A0	  					; X memory
	move 	#Finbuf,r0
	jsr		FileRead
	move	#>INBUFSIZE*3,A1
	move	A1,y:BufRead

    move    #wmadecsp,r5          					; pointer to the SP local save
    move    #_stackRestored,r6    					; setup the return address
    jmp     RestoreSystemStack    					; restore the hardware stack
_stackRestored
; 	reqd = y1, x0 = Read_offset, n1 = index.
	move	y:BufRead,A
	move	y:num_bytes_wanted,y1
	cmp		y1,A		
	tgt		y1,A
	move	A,y1
	move	#0,n1
	move	#0,x0
    move    #>Finbuf,r4
	jsr	GetData

exitGetData
_exit
	move	y:totalRead,A
	move	#>Gpbuffer,r0
	pop		m0
	pop		B2
	pop		B1
	pop		B0

	pop		y1
	pop		y0
	pop		x1
	pop		x0

	pop		n6
	pop		n5
	pop		n4
	pop		n3
	pop		n2
	pop		n1
	pop		n0

	pop		ssh
	pop		r6
	pop		r5
	pop		r4
	pop		r3
	pop		r2
	pop		r1

	rts												; Return value in A.


;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  GetData
;
;    Type:    Function
;           
;    Description:    
;       The routine converts the input data into character format 
;       24bit packed word to 8 bit character conversion.
;
;    Inputs:        
;       y1 = Reqd.
;       n1 = index
;       x0 = read_offset 
;    Outputs:        
;    Notes:
;       Preserve y1.
;///////////////////////////////////////////////////////////////////////////////

GetData
	move	y:totalRead,A
	add	y1,A
	move	A,y:totalRead
	push	y1										; push this up the stack.
	move	y1,y0
	move	#>$2aaaab,y1				
	mpy		x0,y1,A			y:<Const_7fffff,y1		; Convert byte offset to word.
	asr		A				A,n4
	move	A0,A
; 	byte_offset = (get_L(Acc0)>>1 & 0x7fffff);
; 	Word offset is now saved in x0
; 	Byte offset is available in A1
	and   	y1,A			y:<Const_200000,y1
	clr		B				y:<Const_000001,x1
	move	#0,A2
	cmp		y1,A			
	tgt		x1,B		
	move	y:<Const_400000,y1
	cmp		y1,A			y:<Const_000002,x1
	tgt		x1,B		
	move	#ByteState,y1
	add		y1,B			(r4)+n4					; r4 = &inbuf[read_offset]
	move	y:<Const_000002,m0
	move	B,r0
	move	#Gpbuffer,r1
 	move	y:(r4)+,x0 								; Get the first word
	move	(r1)+n1									; Index it correctly.
	move	#>$ff,x1
	do	y0,_LoopEnd1
		move	y:(r0)+,y0							; Get byte state
		mpy		x0,y0,A								; Locate byte.
		cmp		y1,B	
		teq		x0,A								; Is this byte 0? Yes! Transfer the word.
		and		x1,A		r0,B	
		cmp		y1,B		A1,x:(r1)+
		jne		_LoopEnd
 		move	y:(r4)+,x0
_LoopEnd
		move	r0,B
_LoopEnd1
	move	y:<Const_ffffff,m0
	pop		y1
	rts



;///////////////////////////////////////////////////////////////////////////////
;
;>   Name:  WMAFileCBGetData2
;
;    Type:    Function
;           
;    Description:    
;   This routine reads only when required. Typical buffer size is 6 words, 
;   but any optimum size as required by the file system can be selected.
;   This is defined by INBUF2SIZE. 
;   During initialization phase, we allow excess file reads and seeks, but no real-time
;   requirements exist.
;   Once streaming is started, the file reads are minimized. File seeks rarely occur.
;   This ensures that file-system is minimally loaded and the function performs optimally.
;	Data stored in g_pBuffer[128] in char format.
;
;    Inputs:        
;	    x1:x0 = offset
;	    A = num_bytes
;    Outputs:        
;       r0 : ptr to the buffer
;       A = num bytes read.
;    Notes:
;       All trashed registers are backed up and restored. The file-system trashes these.
;///////////////////////////////////////////////////////////////////////////////

WMAFileCBGetData2
	push	r1
	push	r2
	push	r3
	push	r4
	push 	r5
	push	r6

	push	ssh

	push	n0
	push	n1
	push	n2
	push	n3
	push	n4
	push 	n5
	push	n6

	push 	x0
	push	x1
	push 	y0
	push	y1

	push	B0
	push 	B1
	push	B2
	push 	m0

	clr	A   y0,y:num_bytes_wanted
	move	A,y:totalRead

	move	x0,y0									; x0 destroyed in stack save, below

    move    #_stackSaved,r6 						; setup the return address
    move    #wmadecsp,r5    						; pointer to the SP local save
    jmp     SaveSystemStack 						; save the hardware stack
_stackSaved

	clr     A
    move    x1,A1
    move    y0,A0
	
;	fseek(g_fp, offset, SEEK_SET);
    move    y:WmaDecoderFileHandle2,a2
  	jsr		FileSeek
;	ret = fread (&inbuf[0], INBUFSIZE, 1, g_fp);
    move    y:WmaDecoderFileHandle2,a2
	move	#>INBUF2SIZE*3,A1	  					; Always read data of size INBUFSIZE. Data passed is bytes!
	move	y:<Const_400000,A0	  					; X memory
	move 	#Finbuf2,r0
	jsr		FileRead

    move    #wmadecsp,r5          					; pointer to the SP local save
    move    #_stackRestored,r6    					; setup the return address
    jmp     RestoreSystemStack    					; restore the hardware stack
_stackRestored
; 	reqd = y1, x0 = Read_offset, n1 = index.
	move	y:BufRead,A
	move	y:num_bytes_wanted,y1
	cmp		y1,A		
	tgt		y1,A
	move	A,y1
	move	#0,n1
	move	#0,x0
    move    #>Finbuf2,r4
	jsr	GetData

_exit
	move	y:totalRead,A
	move	#>Gpbuffer,r0
	pop		m0
	pop		B2
	pop		B1
	pop		B0

	pop		y1
	pop		y0
	pop		x1
	pop		x0

	pop		n6
	pop		n5
	pop		n4
	pop		n3
	pop		n2
	pop		n1
	pop		n0

	pop		ssh
	pop		r6
	pop		r5
	pop		r4
	pop		r3
	pop		r2
	pop		r1

	rts												; Return value in A.
    

    endsec





; EQUIVALENT C CODE
; tWMA_U32 WMAFileCBGetData(tHWMAFileState *state, tWMA_U32 offset,
; 						   tWMA_U32 num_bytes,  int **ppData)
; {
;     tWMA_U32 ret;
; 	int i;
; 	int read_offset = 0;
; 	int reqd;
; 	int navailable;
; 	int total_read = 0;
; 
; 
;     tWMA_U32 nWanted = num_bytes <= (tWMA_U32) MAX_BUFSIZE ? num_bytes : (tWMA_U32) MAX_BUFSIZE;
;     
; 	data_func_call++;
; 	if(num_bytes != nWanted)
;     {
;         fprintf(stderr, "** WMAFileCBGetData: Requested too much (%lu).\n",
;                 num_bytes);
;     }
; 
; 	if (streaming_started == 1)
; 	{
; 		if (offset > last_offset)
; 		{
; 			read_offset = offset - last_offset;
; 
; 			if (last_offset == 0x3a704)	
; 					last_offset=0x3a704;
; 			navailable = BufRead - read_offset;
; 			if (navailable < 0)			// Likely the first time.
; 			{
;               // This condition will occur very rarely.
; 				fseek(g_fp, offset, SEEK_SET);
; 				last_offset = offset;
; 				BufRead = fread (&inbuf[0], 1, INBUFSIZE, g_fp);
; 				if (nWanted > BufRead)
; 					nWanted = BufRead;
; 				total_read = nWanted;
; 				GetData(0,nWanted,0);
; 			}
; 			else
; 			{
; 				if (navailable >= nWanted)
; 					reqd = nWanted;
; 				else
; 					reqd = navailable;
; 				if (reqd !=0)
; 					GetData(read_offset,reqd,0);
; 				total_read = reqd;
; 				if (reqd !=nWanted)
; 				{
; 					reqd = nWanted - navailable;
; 					last_offset += BufRead;
; 					BufRead = fread (&inbuf[0], 1, INBUFSIZE, g_fp);
; 					if (reqd > BufRead)
; 						reqd = BufRead;
; 					total_read += reqd;
; 					GetData(0,reqd,navailable);
; 				}
; 			}
; 		}
; // The following condition not reqd to be coded. Validated in prototype.
; 		else
; 		{
; 			printf("\n Streaming Error!");
; 		}
; 	}
; 	else
; 	{
; 		fseek(g_fp, offset, SEEK_SET);
; 		last_offset = offset;
; 	    BufRead = fread (&inbuf[0], 1, INBUFSIZE, g_fp);
; 		if (nWanted > BufRead)
; 			nWanted = BufRead;
; 		total_read = nWanted;
; 		GetData(0,nWanted,0);
; 	}
; 	
;     g_cbBuffer = total_read;
; 
;     *ppData = g_pBuffer;
; 
; 
; //    fprintf(stderr, "++ WMAFileCBGetData: %lu bytes from %lu.\n", ret, offset);
; 
;     return total_read;
; }

