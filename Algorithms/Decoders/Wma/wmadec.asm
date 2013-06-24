;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000/2001
;
; STMP MP3 Decoder         
;///////////////////////////////////////////////////////////////////////////////

     section DECWMAMOD_WmaDec
        ;
        ; Includes
        ;
        
    include "sysequ.inc"
    include "hwequ.inc"
    include "wmadec.inc"
    include "dacequ.inc"
    include "decoderequ.inc"
    include "sysmacro.asm"
    include "wmamem.xref"
    include "wmalib.xref"
    include "wmadec.xref"
    include "stack.xref"
    include "stackmem.xref"
    include "const.xref"
    include "wmadecodermem.xref"
    include "wmamem.xref"
	include "dacmem.xref"
    include "bufdesc.xref"
    include "sysmem.xref"
    include "sysmisc.xref"
    include "sysresource.xref"
    include "const.xref"
    include "sysresources.inc"
    include "resource.inc"
	


    ; External Definitions
    ;
    GLOBAL    WmaDecInit
    GLOBAL    WmaDecProcess
	GLOBAL	WMAParseInfo
    GLOBAL   WmaInitSink
    GLOBAL FLoadAes_Enc_P,LoadAes_Enc_P
    GLOBAL LoadPkConst,FLoadPkConst
    GLOBAL FLoadJanusYConst,LoadJanusYConst
	GLOBAL WmaGetSongInfo

    if (@def('USE_PLAYLIST5'))
    extern bDRMLicExpired
    endif
    
    if @def('WMAAPI_NO_DRM')
    else
    GLOBAL WmaLoadLicenseInit
    GLOBAL WmaLoadDecoderCommon
    GLOBAL LoadJanusCommonInit 
    GLOBAL LoadWmaDecoderInit  
    GLOBAL LoadJanusConstants  
    GLOBAL FLoadJanusCommonInit
    GLOBAL FLoadJanusConstants
    GLOBAL SysLoadJanusCommon,FSysLoadJanusCommon 
    GLOBAL FLoadAes_Enc_P
    GLOBAL FLoadJanusSystem_Y,LoadJanusSystem_Y
    GLOBAL FLoadDevcertTemplate,LoadDevcertTemplate
    GLOBAL LoadDeviceCert,FLoadDeviceCert
    GLOBAL LoadJansuXMem,FLoadJansuXMem
    GLOBAL LoadAes_Enc_Y,FLoadAes_Enc_Y
    GLOBAL FLoadJanusSystem,LoadJanusSystem
    GLOBAL LoadJanusYData,FLoadJanusYData
    GLOBAL FLoadJanusWmaSupport,LoadJanusWmaSupport
	

    endif
    ;
    ; External References
    ;

    extern pTitle		  
    extern pAuthor       
    extern pCopyright    
    extern pDescription  
    extern pRating 
    extern fileContDesc_title_len    
    extern fileContDesc_title_len	     
    extern fileContDesc_author_len      
    extern fileContDesc_copyright_len   
    extern fileContDesc_description_len 
    extern fileContDesc_rating_len 
    extern    SysLoadResource
    extern    InitLongVar         
    extern    DacSetSamplingRate
    extern  F_lc_u_b_WmaLicenseInit
    extern  F_lc_u_e_WmaLicenseInit
    extern  F_lc_u_b_WMANonOvl
    extern  F_lc_u_e_WMANonOvl
    extern  SysLoadResource
        if @def('WMAAPI_NO_DRM')
    else
    if @def('JANUS')
    extern F_lc_e__xconstdrmconstants
    extern F_lc_b__xconstdrmconstants
    endif
    endif
    extern F_lc_u_e_DrmpdCommon_P
    extern F_lc_u_b_DrmpdCommon_P
    extern F_lc_u_e_WMA_CONST_X
    extern F_lc_u_b_WMA_CONST_X 
    extern F_lc_u_e_JanusCommon_P
    extern F_lc_u_b_JanusCommon_P
    extern F_lc_u_e_wmasupport_P
    extern F_lc_u_b_wmasupport_P
    extern F_lc_u_e_TABLE1_X
    extern F_lc_u_b_TABLE1_X
    extern F_lc_u_e_TABLE_X
    extern F_lc_u_b_TABLE_X
    extern F_lc_u_b_AES_ENC_P
    extern F_lc_u_e_AES_ENC_P
    extern F_lc_u_b_CONST_PKCRYPTO_Y
    extern F_lc_u_e_CONST_PKCRYPTO_Y

    if @def('WMAAPI_NO_DRM')
    else
	extern	  PmidInit	
	extern   CurrentDirDevId

    endif
    ;
    ; Code Start
    ;
    


       org      p,".wmadecinit":
;*******************************************************************************
;  Function:
;             WmaDecInit
;  Description:
;	      Called by WMADecoderModuleInit. Runs through WMA Initialization
;	      Routines. Do not call this routine, during run-time
;	      All the called functions come from overlays.		
;  Parameters passed: 
;	      None
;  Registers Trashed: 
;             All
;  Return Values:
;             None
;*******************************************************************************
WmaDecInit
	; No source buffer. Source buffer is internal to WMA.
	move    y:WmaInitFlag,A
	tst	A	#0,A
	jne	_Continue


    jsr     WmaInitSink             	; Setup the Sink Buffer
	clr 	A				; Clear flags
	; The following flag will be used to verify that reconstructed data is available.
	move 	A,x:WMAReconstructedFlag
	; Initialize WMA and Kickstart the DAC.
	; r7 and m7 are unaltered
	jsr	WMADecoderInitSection
	tst	A
	jne	_exit				; Initialization failed.
	; Initialization successful. 
	; Store correct sampling frequency to DacSrr
	; Reuse the MP3SampFreqTable. Re-organized the WMA frequency table!
	; WMA returns index to the DAC Frequency table, which is identical to MP3 freq table!
	move	y:<Const_000001,y0
	move	y0,y:WmaInitFlag
_Continue


	clr		A							; Return clean.
_exit
	rts


WmaInitSink
        ; init buffer head / tail to beginning
        move    #g_WmaDecSinkBufDescriptor,r0      ; SINK buffer descriptor ptr
        move    #>$ffff,m0
        jsr     InitBufferEmpty

        ; init sink buffer to half full (with zeros)
        move    #g_WmaDecSinkBufDescriptor,r0   ; SINK buffer descriptor ptr
        move    #>$ffff,m0
        clr     a
        move    a,y1                         	; Y1 = 0 = value to init to
        move    y:g_pWmaDecSinkBufSize,a
        move    y:<Const_000800,x0		; 2048 samples .. approx half.
        sub     x0,a
        move    a1,n0
        jsr     FillBufferX			; Output buffer is in X memory.

        rts        

;	WMA code overlays. 
;	.pInit: Init Section
;	.pCommon  : All rates, including init portion
;	.pCore : All rates
;	.pHigh : High rate specific code.	
;	.pHighMid : Code common to high rate and mid rate.
;	.pMid : Mid rate specific code
;	.pLow : Low rate specific code
;	.pMidLow : Code common to Mid and Low rate.

;	Data overlay sections (ROM tables):
;	1. Huffman 16-O tables (mono and stereo)
;	2. Huffman 44-O tables (mono and stereo)
;	3. Huffman 44-Q tables (mono and stereo)

WMADecoderInitSection
	push 	m0
	push 	m1
	push 	m2
	push 	m3
	push 	m4
	push 	m5
	push 	m6
        jsr     SysAllLinear		; WMA always assumes linear addressing.

		; Validate that the file is WMA
	jsr		WMAParseInfo
	tst 	A
	jne	_exit			
;	Content description is available at this point.
;	printf("- WMA version  : %d\r\n",filehdr_version);
;	// translate our sample rate enum into something we can really work with
;	printf("- Sample Rate  : %s\r\n",SampleRateStrings[filehdr_sample_rate]);
;	
;	// show the rest of the audio stream information
;	printf("- Channels     : %d\r\n", filehdr_num_channels);
;	printf("- Bit Rate     : %d bps\r\n", filehdr_bitrate);
;	printf("- Duration     : %d ms\r\n", filehdr_duration);
;	printf("- DRM          : %s\r\n\r\n", filehdr_has_DRM ? "Yes" : "No");


        if @def('WMAAPI_NO_DRM')
    else
    if @def('JANUS')
    move y:filest_bHasJanusDRM,A
    tst A
    jne _come_here 
        ; clear the pcm buffer for Wma decode routines
        ; PCM buffer is in X memory.
	; bit buffer is cleared internally.
        clr     a
        move    y:<Const_ffffff,m3
        move    #>g_WmaDecSink_mbuf,r3
	move	#g_WmaDecSinkBufSize,x0
        do      x0,_clear_pcmbuffer
        move    a,x:(r3)+
        nop
_clear_pcmbuffer
        nop
_come_here 
    endif
    endif       
    move y:filest_bHasDRM,A
	tst  A
	jeq  _Continue_NoLicense
	if @def('WMAAPI_NO_DRM')
	else
	; Get the serial number
	move y:CurrentDirDevId,A
	jsr	 PmidInit
    tst  A
	jne	_exit	            ; exit on error. else ID is now at x:Fg_pmid	
	jsr  WMAFilelicenseWrap
	tst  A
	endif
    if (@def('USE_PLAYLIST5'))	
	jne _DRMfail
	nop
    jmp _Continue_NoLicense
_DRMfail
    move    #>1,B0
    move    B0,y:FbDRMLicExpired
    jmp _exit
;  	jne	_exit
    else
  	jne	_exit	
	nop
    endif	    
    

_Continue_NoLicense
    move y:auobj_iWeightingMode,A
	tst A		y:<Const_000002,x0
	move y:auobj_fNoiseSub,A
	teq x0,A
	move A,x:mode_type			; 0 = High, 1 = Mid, 2 = Low

	; All the pointers have been set. Now load appropriate Huffmann tables.
	; Load both the stereo and mono tables.
	; Load mono tables first.
	move y:auobj_iEntropyMode,A
	tfr  A,B 	#>(RSRC_WMA_HUFF44QB-1),x0	; Entropy mode is 1,2,3. so make base 0.
	add  x0,A			
	move A,x0	 		     ; Resource number
        move #>RSRC_TYPE_DATA,x1             ; Resource type is data
	move #>(HuffTableSizeBase-1),y1
	add  y1,B
	move B,r0
	nop
	move 	y:(r0),y1			; Data size is in bytes	 
	move 	#>Fpg_rgiHuffDecTbl16smOb,y0	; Address of the overlay area. Same for all Now.
        move    #TARGET_MEM_Y,a1                ; Overlay area is in Y memory
        jsr     SysLoadResource

	; Prepare the code space now!
	; Common sections of core decoder loaded first.
	move    #>RSRC_WMA_CORE,x0
	move    #>RSRC_TYPE_DATA,x1             		; Resource type is data
        move    #>(F_lc_e__pCore-F_lc_b__pCore)*3,y1 ; Size of the overlay area
        move    #>F_lc_b__pCore,y0      ;  Address of the overlay area
        move    #TARGET_MEM_P,a1             		; Overlay area is in Y memory
        jsr     SysLoadResource

	
	; MidLow is loaded by default. This arrangement uses minimum space.
	move    #>RSRC_WMA_MIDLOW,x0
	move    #>RSRC_TYPE_DATA,x1             		; Resource type is data
        move    #>(F_lc_e__pMidLow-F_lc_b__pMidLow)*3,y1 ; Size of the overlay area
        move    #>F_lc_b__pMidLow,y0      ;  Address of the overlay area
        move    #TARGET_MEM_P,a1             		; Overlay area is in Y memory
        jsr     SysLoadResource
	 
	move x:mode_type,B
        tst  B 		y:<Const_000001,y1
	jeq  _case0
	cmp y1,B   y:<Const_000002,y1
	jeq  _case1
	cmp y1,B
	jeq  _case2
	; Decoder organized into two main overlay schemes
	;	1. Initialization
	;	2. Core Decoder
	;	; Core decoder is organized into following core Sections
	;	.pCommon  : All rates, including init portion
	;	.pCore : All rates
	;	.pHigh : High rate specific code.	
	;	.pHighMid : Code common to high rate and mid rate.
	;	.pMid : Mid rate specific code
	;	.pLow : Low rate specific code
	;	.pMidLow : Code common to Mid and Low rate.
	
_case0
	; The codec is Highrate
	move    #>RSRC_WMA_HIGH,x0
	move    #>RSRC_TYPE_DATA,x1             		; Resource type is data
        move    #>(F_lc_e__pHigh-F_lc_b__pHigh)*3,y1 		; Size of the overlay area
        move    #>F_lc_b__pHigh,y0     				;  Address of the overlay area
        move    #TARGET_MEM_P,a1       				; Overlay area is in Y memory
        jsr     SysLoadResource

	move    #>RSRC_WMA_HIGHMID,x0
	move    #>RSRC_TYPE_DATA,x1             		; Resource type is data
        move    #>(F_lc_e__pHighMid-F_lc_b__pHighMid)*3,y1 	; Size of the overlay area
        move    #>F_lc_b__pHighMid,y0     			;  Address of the overlay area
        move    #TARGET_MEM_P,a1          			; Overlay area is in Y memory
        jsr     SysLoadResource
	jmp	_endLoadResource

_case1
	; The codec is MidRate
	move    #>RSRC_WMA_MID,x0
	move    #>RSRC_TYPE_DATA,x1             		; Resource type is data
        move    #>(F_lc_e__pMid-F_lc_b__pMid)*3,y1 		; Size of the overlay area
        move    #>F_lc_b__pMid,y0      				;  Address of the overlay area
        move    #TARGET_MEM_P,a1             			; Overlay area is in Y memory
        jsr     SysLoadResource

	move    #>RSRC_WMA_HIGHMID,x0
	move    #>RSRC_TYPE_DATA,x1             		; Resource type is data
        move    #>(F_lc_e__pHighMid-F_lc_b__pHighMid)*3,y1 	; Size of the overlay area
        move    #>F_lc_b__pHighMid,y0      			;  Address of the overlay area
        move    #TARGET_MEM_P,a1             			; Overlay area is in Y memory
        jsr     SysLoadResource					
	jmp	_endLoadResource

_case2
	; The codec is Lowrate
	move    #>RSRC_WMA_LOW,x0
	move    #>RSRC_TYPE_DATA,x1             		; Resource type is data
        move    #>(F_lc_e__pLow-F_lc_b__pLow)*3,y1 		; Size of the overlay area
        move    #>F_lc_b__pLow,y0      				;  Address of the overlay area
        move    #TARGET_MEM_P,a1             			; Overlay area is in Y memory
        jsr     SysLoadResource

_endLoadResource
    if @def('WMAAPI_NO_DRM')
    else
    if @def('JANUS')
    move y:filest_bHasJanusDRM,A
    tst A
    jeq _exithere
    jsr  LoadJanusCommonInit       ; load janus common code decode process
    clr     a
    move    #>auobj_piPrevOutput,r3
	move	#$AAB,x0
    do      x0,_clear_PrevOutput
    move    a,y:(r3)+
_clear_PrevOutput
    move    a,y:(r3)    
    endif
    endif
_exithere
    clr	A						; Return Clean.
_exit
	pop 	m6
	pop 	m5
	pop 	m4
	pop 	m3
	pop 	m2
	pop 	m1
	pop 	m0
        rts


		org     p,"DECWMAMOD_WMADec_P":
;*******************************************************************************
;  Function:
;             WmaDecProcess
;  Description:
;	      Called by WMADecoderModule. 
;	      Handles I/O	
;  WMA does not have same output frame size. Just take whatever data
;  the decoder provides and reconstruct, when enough space is available.
;  Sometimes no data maybe returned.
;  This requires that WMADecProcessISR, must operate every 0.5 ms, to 
;  continuously poll, whether there is space available in the output
;  buffer. If there is enough space, reconstruct the output data.
;  The output reconstruction is staggered as and when space is available,
;  reducing the size of the spillover buffer.
;  This scheme also allows the player to run the clock for average MIPs,
;  Worst case demand computed by locating worst possible scenarios at 192Kbps, 48 Khz, stereo.
;  Average MIPs demand is the average MIPs at 192 Kbps, 48 Khz, stereo.
;  Peak MIPS Demand:
;	STMP3400 : 51+(DRM=12)=63 : 
;	STMP3410 : 43+(DRM=12)=55 : RLC Tables are not packed	
;  Average MIPS Demand:
;	STMP3400 : 30+(DRM=5)=35
;	STMP3410 : 27+(DRM=5)=32
;
;  Parameters passed: 
;	      None
;  Registers Trashed: 
;             All
;  Return Values:
;             None
;*******************************************************************************
WmaDecProcess


	; We are running the Core Decoder.
_decode_Wma     
	move	y:<Const_000001,A
	; Set the decoder to stream data. 
	move 	A,y:streaming_started
	move x:WMAReconstructedFlag,A
	tst A
	jne _ReconstructData
	; r7 and m7 are unaltered
    jsr  WMAFileDecodeData		; Call the core decoder.
	move #>cWMA_NoErr,x0
	cmp  x0,A	y:<Const_000001,x0
	jne  _PrepareToReturn			; Error. Top level module must check return values!
	move x0,x:WMAReconstructedFlag
	; Fall Through!
_ReconstructData
_ReconLoop
        move    #g_WmaDecSinkBufDescriptor,r4      ; SINK buffer descriptor ptr
        move    #>$ffff,m4
	jsr 	GetEmptySpace		
	move 	y:<Const_000100,x0		; Reconstruct in sets of 256 samples
	cmp  	x0,A		#0,A
	jlt  	_PrepareToReturn		; No more space in the output, exit for time being

	move 	#Fg_pLeft,r0			; Left channel
 	move 	#0,r4					; Right channel. Set this to null for interleaved samples
 	move 	#>WMA_RECON_MAX_SAMPLES,A		; Get samples in sets of 128 in each channel
 	jsr		WMAFileGetPCM
;	 Returns number of samples decoded in A.
	tst  A		
	jeq  _EndRecon			; Reconstruct as much as possible.
	move y:filehdr_num_channels,x0
	rep   x0
	asl  A		x0,B		
	asr  A		y:<Const_000001,y0	; Restore the extra left shift.
	; Copy Output samples from g_pLeft buffer to the output buffer.
	cmp 	y0,B
        move    y:g_pWmaDecSinkBufMod,m0
        move    y:g_pWmaDecSinkBufHead,r0
	move	#g_pLeft,r4
	move	y:<Const_000080,y0
	jeq	_MonoChannel
	move	x:(r4)+,x0		; Get the first sample in x0
	do   A,_EndCopySamplesStereo
		mpy  x0,y0,A	x:(r4)+,x0	; Output is aligned to 16 bit. Make it 24.
		move A0,x:(r0)+   
_EndCopySamplesStereo
	jmp  _EndCopySamples
_MonoChannel
	move	x:(r4)+,x0		; Get the first sample in x0
	do   A,_EndCopySamplesMono
		mpy  x0,y0,A	x:(r4)+,x0
		move A0,x:(r0)+			; Duplicate mono data in both channels.
		move A0,x:(r0)+   
_EndCopySamplesMono
_EndCopySamples
	move  r0,y:g_pWmaDecSinkBufHead
	move 	y:<Const_ffffff,m0		; Restore the modulo to linear
	jmp  _ReconLoop
_EndRecon
	clr	A
	move 	A,x:WMAReconstructedFlag	; Returned zero sample reconstruction
	move 	y:<Const_ffffff,m0		; Restore the modulo to linear
	jmp	_decode_Wma			; Completed reconstruction, now decode more data

_PrepareToReturn
	move 	y:<Const_ffffff,m0		; Restore the modulo to linear
_exit
        nop
        rts



WMAParseInfo
    clr	A
	move 	A,y:streaming_started
    ; Clear meta data.
    move A,x:pTitle		    
    move A,x:pAuthor          
    move A,x:pCopyright       
    move A,x:pDescription     
    move A,x:pRating          
    ; Limit the size of string data which can be filled.    
    move y:<Const_000080,A
    move A,y:fileContDesc_title_len    
    move A,y:fileContDesc_title_len	     
    move A,y:fileContDesc_author_len      
    move A,y:fileContDesc_copyright_len   
    move A,y:fileContDesc_description_len 
    move A,y:fileContDesc_rating_len      

    jsr     SysAllLinear		; WMA always assumes linear addressing.
	jsr	InitLongVar		; Initialize long variables.
    jsr	WMAFileIsWMA
	tst 	A
	jne	_exit			; Exits if file is not WMA
	; Initialize the decoder
    jsr     WMAFileDecodeInit
	tst 	A
    jne	_exit			
    ; get header information
    jsr	WMAFileDecodeInfo
	tst 	A
	jne	_exit			
    jsr     WMAFileContentDesc
	tst 	A
	jne	_exit			
	clr		A
_exit
	rts
    

WmaGetSongInfo
	move    y:WmaInitFlag,A
	tst	A	#0,A
	jne	_Continue

    clr	A
	move 	A,y:streaming_started
    ; Clear meta data.
    move A,x:pTitle		    
    move A,x:pAuthor          
    move A,x:pCopyright       
    move A,x:pDescription     
    move A,x:pRating          
    ; Limit the size of string data which can be filled.    
    move y:<Const_000080,A
    move A,y:fileContDesc_title_len    
    move A,y:fileContDesc_title_len	     
    move A,y:fileContDesc_author_len      
    move A,y:fileContDesc_copyright_len   
    move A,y:fileContDesc_description_len 
    move A,y:fileContDesc_rating_len 

    jsr     SysAllLinear		; WMA always assumes linear addressing.
	jsr	InitLongVar		; Initialize long variables.
    jsr	WMAFileIsWMA
	tst 	A
	jne	_exit			; Exits if file is not WMA
    jsr	WMAFileDecodeInfo
	tst 	A
	jne	_exit
_Continue
	clr a				

_exit
	rts
    

    
	if @def('WMAAPI_NO_DRM')
	else
	org p,".pWmaWrap":
WmaLoadLicenseInit
    move    #>RSRC_WMA_LICENSEINIT,x0                                        ; Resource number
    move    #>RSRC_TYPE_DATA,x1                                         ; Resource type is data
    move    #>(F_lc_u_e_WmaLicenseInit-F_lc_u_b_WmaLicenseInit)*3,y1            ; Size of the overlay area
    move    #>F_lc_u_b_WmaLicenseInit,y0                                    ; Address of the overlay area
    move    #TARGET_MEM_P,a1                                            ; Overlay area is in P memory
    jsr     SysLoadResource
    rts
    endif

	if @def('WMAAPI_NO_DRM')
	else
	org p,".pWmaWrap":
WmaLoadDecoderCommon
    move    #>RSRC_WMA_COMMON,x0
	move    #>RSRC_TYPE_DATA,x1             		; Resource type is data
    move    #>(F_lc_u_e_WMANonOvl-F_lc_u_b_WMANonOvl)*3,y1 ; Size of the overlay area
    move    #>F_lc_u_b_WMANonOvl,y0      ;  Address of the overlay area
    move    #TARGET_MEM_P,a1             		; Overlay area is in Y memory
    jsr     SysLoadResource

	move    #>RSRC_WMA_INIT,x0
    move    #>RSRC_TYPE_DATA,x1             		; Resource type is data
    move    #>(F_lc_e__pInit-F_lc_b__pInit)*3,y1 ; Size of the overlay area
    move    #>F_lc_b__pInit,y0      ;  Address of the overlay area
    move    #TARGET_MEM_P,a1             		; Overlay area is in Y memory
    jsr     SysLoadResource
    rts
    endif    
    
	if @def('WMAAPI_NO_DRM')
	else
	org p,".pWmaWrap":
LoadWmaDecoderInit
    ; Load the Wma decoder into P memory
	; Common sections of core decoder loaded first.
	move    #>RSRC_DRMPD_COMMON,x0
	move    #>RSRC_TYPE_DATA,x1             		; Resource type is data
        move    #>(F_lc_u_e_DrmpdCommon_P-F_lc_u_b_DrmpdCommon_P)*3,y1 ; Size of the overlay area
        move    #>F_lc_u_b_DrmpdCommon_P,y0      ;  Address of the overlay area
        move    #TARGET_MEM_P,a1             		; Overlay area is in Y memory
        jsr     SysLoadResource
        

        move    #>RSRC_WMA_TABLES,x0
        move    #>RSRC_TYPE_DATA,x1             		; Resource type is data
        move    #>(F_lc_u_e_TABLE_X-F_lc_u_b_TABLE_X)*3,y1 ; Size of the overlay area
        move    #>F_lc_u_b_TABLE_X,y0      ;  Address of the overlay area
        move    #TARGET_MEM_X,a1             		; Overlay area is in Y memory
        jsr     SysLoadResource

        move    #>RSRC_WMA_CONSTANTS,x0
        move    #>RSRC_TYPE_DATA,x1             		; Resource type is data
        move    #>(F_lc_u_e_WMA_CONST_X-F_lc_u_b_WMA_CONST_X)*3,y1 ; Size of the overlay area
        move    #>F_lc_u_b_WMA_CONST_X,y0      ;  Address of the overlay area
        move    #TARGET_MEM_X,a1             		; Overlay area is in Y memory
        jsr     SysLoadResource
        
        
        rts
       endif

    
	if @def('WMAAPI_NO_DRM')
	else
	if @def('JANUS')
	org p,".pWmaWrap":
FLoadJanusCommonInit
LoadJanusCommonInit
    move    #>RSRC_JANUS_COMMON,x0                                        ; Resource number
    move    #>RSRC_TYPE_DATA,x1                                         ; Resource type is data
    move    #>(F_lc_u_e_JanusCommon_P-F_lc_u_b_JanusCommon_P)*3,y1            ; Size of the overlay area
    move    #>F_lc_u_b_JanusCommon_P,y0                                    ; Address of the overlay area
    move    #TARGET_MEM_P,a1                                            ; Overlay area is in P memory
    jsr     SysLoadResource
    
    move    #>RSRC_JANUS_TABLES,x0
    move    #>RSRC_TYPE_DATA,x1             		; Resource type is data
    move    #>(F_lc_u_e_TABLE1_X-F_lc_u_b_TABLE1_X)*3,y1 ; Size of the overlay area
    move    #>F_lc_u_b_TABLE1_X,y0      ;  Address of the overlay area
    move    #TARGET_MEM_X,a1             		; Overlay area is in X memory
    jsr     SysLoadResource
    rts
    
FLoadJanusYData
LoadJanusYData
    move    #>RSRC_JANUSY_DATA,x0
    move    #>RSRC_TYPE_DATA,x1             		; Resource type is data
    move    #>(F_lc_u_e_DATA_JANUS_Y-F_lc_u_b_DATA_JANUS_Y)*3,y1 ; Size of the overlay area
    move    #>F_lc_u_b_DATA_JANUS_Y,y0      ;  Address of the overlay area
    move    #TARGET_MEM_Y,a1             		; Overlay area is in X memory
    jsr     SysLoadResource
    rts
    
FLoadJanusSystem
LoadJanusSystem
    ; Load the janus section into P memory
    move    #>RSRC_JANUSP_CODE,x0             ; Resource number
    move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
    move    #>(F_lc_u_e_janus_p-F_lc_u_b_janus_p)*3,y1 ;overlay size
    move    #>F_lc_u_b_janus_p,y0      ; Address of the overlay area
    move    #TARGET_MEM_P,a1                ; Overlay area is in P memory
    jsr     SysLoadResource

    ; Load the janus section into X memory
    move    #>RSRC_JANUSX_CODE,x0             ; Resource number
    move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
    move    #>(F_lc_u_e_janus_X_buf-F_lc_u_b_janus_X_buf)*3,y1 ;overlay size 
    move    #>F_lc_u_b_janus_X_buf,y0      ; Address of the overlay area
    move    #TARGET_MEM_X,a1                ; Overlay area is in X memory
    jsr     SysLoadResource

    rts

FLoadJanusWmaSupport
LoadJanusWmaSupport
    move    #>RSRC_JANUS_WMASUPPORT,x0                                        ; Resource number
    move    #>RSRC_TYPE_DATA,x1                                         ; Resource type is data
    move    #>(F_lc_u_e_wmasupport_P-F_lc_u_b_wmasupport_P)*3,y1            ; Size of the overlay area
    move    #>F_lc_u_b_wmasupport_P,y0                                    ; Address of the overlay area
    move    #TARGET_MEM_P,a1                                            ; Overlay area is in P memory
    jsr     SysLoadResource
    rts
    
    endif
    endif
    
    
	if @def('WMAAPI_NO_DRM')
	else
    if @def('JANUS')
	org p,".pWmaWrap":
FSysLoadJanusCommon
SysLoadJanusCommon
    move    #>RSRC_JANUS_COMMON,x0                                        ; Resource number
    move    #>RSRC_TYPE_DATA,x1                                         ; Resource type is data
    move    #>(F_lc_u_e_JanusCommon_P-F_lc_u_b_JanusCommon_P)*3,y1            ; Size of the overlay area
    move    #>F_lc_u_b_JanusCommon_P,y0                                    ; Address of the overlay area
    move    #TARGET_MEM_P,a1                                            ; Overlay area is in P memory
    jsr     SysLoadResource
    rts
    endif
    endif
    
	if @def('WMAAPI_NO_DRM')
	else
    if @def('JANUS')
	org p,".pWmaWrap":
FLoadJanusConstants
LoadJanusConstants
    move    #>RSRC_JANUS_CONSTANTS,x0
    move    #>RSRC_TYPE_DATA,x1             		; Resource type is data
    move    #>(F_lc_e__xconstdrmconstants-F_lc_b__xconstdrmconstants)*3,y1 ; Size of the overlay area
    move    #>F_lc_b__xconstdrmconstants,y0      ;  Address of the overlay area
    move    #TARGET_MEM_X,a1             		; Overlay area is in X memory
    jsr     SysLoadResource
    rts

FLoadDevcertTemplate
LoadDevcertTemplate
    move    #>RSRC_DEVCERT_TEMPLATE,x0
    move    #>RSRC_TYPE_DATA,x1             		; Resource type is data
    move    #>(F_lc_e__xdevecerttemplate-F_lc_b__xdevecerttemplate)*3,y1 ; Size of the overlay area
    move    #>F_lc_b__xdevecerttemplate,y0      ;  Address of the overlay area
    move    #TARGET_MEM_X,a1             		; Overlay area is in X memory
    jsr     SysLoadResource
    rts
    
FLoadDeviceCert
LoadDeviceCert
    move    #>RSRC_DEVCERT,x0
    move    #>RSRC_TYPE_DATA,x1             		; Resource type is data
    move    #>(F_lc_e__xdevcert-F_lc_b__xdevcert)*3,y1 ; Size of the overlay area
    move    #>F_lc_b__xdevcert,y0      ;  Address of the overlay area
    move    #TARGET_MEM_X,a1             		; Overlay area is in X memory
    jsr     SysLoadResource
    rts


FLoadJansuXMem
LoadJansuXMem
    ; Load the Janus into X memory
    move    #>RSRC_JANUS_X,x0                                        ; Resource number
    move    #>RSRC_TYPE_DATA,x1                                         ; Resource type is data
    move    #>(F_lc_u_e_JANUSMEM_X-F_lc_u_b_JANUSMEM_X)*3,y1            ; Size of the overlay area
    move    #>F_lc_u_b_JANUSMEM_X,y0                                    ; Address of the overlay area
    move    #TARGET_MEM_X,a1                                            ; Overlay area is in X memory
    jsr     SysLoadResource
    rts
    
    endif
    endif

	org p,".pWmaWrap":
FLoadAes_Enc_P
LoadAes_Enc_P
    move    #>RSRC_AES_ENC,x0
    move    #>RSRC_TYPE_DATA,x1             		; Resource type is data
    move    #>(F_lc_u_e_AES_ENC_P-F_lc_u_b_AES_ENC_P)*3,y1 ; Size of the overlay area
    move    #>F_lc_u_b_AES_ENC_P,y0      ;  Address of the overlay area
    move    #TARGET_MEM_P,a1             		; Overlay area is in X memory
    jsr     SysLoadResource
    rts
    
    
	org p,".pWmaWrap":
FLoadPkConst
LoadPkConst
    move    #>RSRC_CONST_PKCRYPTO,x0
    move    #>RSRC_TYPE_DATA,x1             		; Resource type is data
    move    #>(F_lc_u_e_CONST_PKCRYPTO_Y-F_lc_u_b_CONST_PKCRYPTO_Y)*3,y1 ; Size of the overlay area
    move    #>F_lc_u_b_CONST_PKCRYPTO_Y,y0      ;  Address of the overlay area
    move    #TARGET_MEM_Y,a1             		; Overlay area is in X memory
    jsr     SysLoadResource
    rts
    
FLoadJanusYConst
LoadJanusYConst
    move    #>RSRC_CONST_Y,x0
    move    #>RSRC_TYPE_DATA,x1             		; Resource type is data
    move    #>(F_lc_u_e_CONST_Y-F_lc_u_b_CONST_Y)*3,y1 ; Size of the overlay area
    move    #>F_lc_u_b_CONST_Y,y0      ;  Address of the overlay area
    move    #TARGET_MEM_Y,a1             		; Overlay area is in X memory
    jsr     SysLoadResource
    rts
    



;	if @def('WMAAPI_NO_DRM')
;	else
	org p,".pWmaWrapdummy":
Loaddummy
    rts
;    endif

        endsec




