;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2001
;
;  File        : syscoder.asm
;  Description : Misc System Routines for Encoders & Decoders
;///////////////////////////////////////////////////////////////////////////////

    page    255,255,0

    section SYSFUN_SysCoder

;///////////////////////////////////////////////////////////////////////////////
;   Macros
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "sysmacro.asm"
    include "systrace.asm"
    list

    OPT OP
;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "hwequ.inc"
    include "encadpcmequ.inc"
    include "msgequ.inc"
    include "resource.inc"
    include "sysresources.inc"
    list

;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////
    
    ;Functions  
    global  EncoderGetStatus
    global  DecoderGetStatus
    global  EncGetRecordTime
    global  DecGetSongPos
    global  Mp3DecGetSongPos
    global  DecGetSongNumber
    global  DecSetSongPos
    global  DecSetSongPosZero
    global  DecSetSongPosEnd
    global  DecClearSongTime
    global  DecGetSongTime
    global  DecGetCurrentTime
;    global  SysSetPlaybackMode
    global  SysSetPlaySet
    global  SysGetPlaySet
    global  SysGetPlaybackMode
    global  SysFlushCoderMemory
    global  ParserGetFileNamePtr
    global  SysSetDecoder
    global  SysSetDecoderResource
    global  SysSetEncoder
    global  DecoderForceInit
    global  EncoderForceInit
    global  SysStopDecoder
    global  SysStopEncoder

    global  SongSecondsPerByte
    global  SongCurrentMinutes
    global  SongCurrentSeconds
    global  SongTotalMinutes
    global  SongTotalSeconds
    global  SongRemainMinutes 
    global  SongRemainSeconds
    global  CurrentSongNumber
    global  TotalSongCount

    global  Fg_frSongSecondsPerByte
    global  Fg_wSongCurrentMinutes   
    global  Fg_wSongCurrentSeconds
    global  Fg_wSongTotalMinutes     
    global  Fg_wSongTotalSeconds     
    global  Fg_wSongRemainMinutes    
    global  Fg_wSongRemainSeconds    
    global  Fg_wCurrentSongNumber
    global  Fg_wTotalSongCount

        
;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "const.xref"
    list

    extern    DecoderSR
    extern    EncoderSR
    extern    RecStartTimeHigh 
    extern    RecStartTimeLow  
    extern    SysGetCurrentTime
    extern    SongByteLengthHigh
    extern    SongByteLengthLow
    extern    SongByteTotalHigh
    extern    SongByteTotalLow
    ; TIME_FIX   Add the 2 decoder song byte consumed vars
    extern    SongByteTotalConsumedHigh
    extern    SongByteTotalConsumedLow
    extern    SongInvBitRatePtr
    extern    CurrentDirDevId
    extern    SignalModule

    extern    F_lc_u_bX_app_cluster
    extern    F_lc_u_eX_app_cluster

    extern    F_lc_u_bY_app_cluster
    extern    F_lc_u_eY_app_cluster

    extern    F_lc_u_eP_app_cluster
    extern    F_lc_u_bP_app_cluster

  if (!(@def('DEVICE_3400')||@def('DEVICE_3300')))
    extern    F_lc_u_e_WMA_LY
  endif
    
    extern  DecoderResourcePtr
    extern  EncoderResourcePtr
    extern  MODULE_NUM_DECODER
    extern  MODULE_NUM_CAPTURE
    extern  SysPostMessage
    extern  SysLoadResource
    extern  DecoderQueueDescriptor
    extern  CaptureQueueDescriptor
    extern  DecoderProcessPtr
    extern  EncoderProcessPtr

 if (@DEF('FUNCLET'))           
    extern    SysCallFunclet
 endif

;///////////////////////////////////////////////////////////////////////////////
;   X Memory
;///////////////////////////////////////////////////////////////////////////////
        
    org     x,"SYSFUN_SysCoder_X":

;///////////////////////////////////////////////////////////////////////////////
;   Y Memory
;///////////////////////////////////////////////////////////////////////////////

    org     y,"SYSFUN_SysCoder_Y":
Fg_wSongCurrentMinutes
SongCurrentMinutes   dc      0

Fg_wSongCurrentSeconds     
SongCurrentSeconds   dc      0

Fg_frSongSecondsPerByte   
SongSecondsPerByte   dc      0

Fg_wSongTotalMinutes      
SongTotalMinutes     dc      0

Fg_wSongTotalSeconds     
SongTotalSeconds     dc      0

Fg_wSongRemainMinutes    
SongRemainMinutes    dc      0

Fg_wSongRemainSeconds    
SongRemainSeconds    dc      0

Fg_wCurrentSongNumber
CurrentSongNumber    dc      0

Fg_wTotalSongCount
TotalSongCount       dc      0          ; gets loaded with total number of songs across all cards in root dirs
         
;///////////////////////////////////////////////////////////////////////////////
;   P Memory
;///////////////////////////////////////////////////////////////////////////////

    org     p,"SYSFUN_SysCoder_P":

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           DecoderGetStatus
;
;   Type:           Function
;
;   Description:    Get the Decoder status
;
;   Inputs:         none
;
;   Outputs:        x0 = DecoderSR
;                   
;   Notes:          DecoderSR bit equates.  The bit field that is set indicates
;                   the state of the decoder.  Bit fields are listed below.
;                    
;                   Bit 5  Pause  
;                   Bit 6  Stop   
;                   Bit 12 Play               
;                   
;<
;///////////////////////////////////////////////////////////////////////////////
DecoderGetStatus
    move    x:DecoderSR,x0
    move    x0,a
    rts

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           EncoderGetStatus
;
;   Type:           Function
;
;   Description:    Get the Encoder status
;
;   Inputs:         none
;
;   Outputs:        x0 = EncoderCSR
;                   a1 = EncoderCSR
;   Notes:          EncoderSR bit equates.  The bit field that is set indicates
;                   the state of the encoder.  Bit fields are listed below.
;                                                             
;                   Bit 0  EncNotEnoughSpace                                                                       
;                   Bit 1  EncRanOutOfSpace                                                                        
;                   Bit 2  EncRecordError                                                                          
;                   Bit 3  EncToldToStop                                                                           
;                   Bit 4  EncAlreadyRecording                                                                  
;                   Bit 6  Stop                                                                                 
;                   Bit 12 Rec
;
;<
;///////////////////////////////////////////////////////////////////////////////
EncoderGetStatus
    move    x:EncoderSR,x0
    move    x0,a
    rts

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           EncGetRecordTime
;
;   Type:           Function
;
;   Description:    Get the time that the Encoder started recording
;
;   Inputs:         none
;
;   Outputs:        x0 = Low word of the time that recording started
;                   x1 = High word of the time that recording started
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
EncGetRecordTime
    ; TODO:  have the encoder save the current time when it starts and avoid
    ;   this calculation.
    jsr     SysGetCurrentTime
    move    x:RecStartTimeHigh,x1
    move    x:RecStartTimeLow,x0
    sub     x,a
    rts
    
;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           DecGetSongPos
;
;   Type:           Function
;
;   Description:    Returns the byte count of the current position in the song
;                   being decoded.
;
;   Inputs:         none
;
;   Outputs:        A1 = SongByteTotalHigh
;                   A0 = SongByteTotalLow
;                   
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
DecGetSongPos
    clr     a
    move    y:SongByteTotalHigh,a1
    move    y:SongByteTotalLow,a0
;    move    y:SongByteTotalConsumedHigh,a1
;    move    y:SongByteTotalConsumedLow,a0
    ; TIME_FIX   Future accuracy enhancement to use consumed byte count instead  
    ; of SongByteTotalLow|High. USE OLD byte count until all decoders updated!
    ; AB mode for adpcm, mp3, wma use this routine. Consumed byte count version  
    ; is more precise but we must use the SongByteTotalLow|High values until ALL 
    ; supported decoders calculate the new variable. Future version will use 
    ; decoder consumed bytes instead of adjusted bytes read from file.
    rts

;///////////////////////////////////////////////////////////////////////////////
; Name:        Mp3DecGetSongPos
; Type:        Function
; Description: Returns the byte count of the current position in the song
;              being decoded.
;
; Inputs:      none
; Outputs:     Returns in A1:A0 SongByteTotalConsumedHigh and Low
; Note:        Corrects low bitrate issue with MP3 A-B function.
;///////////////////////////////////////////////////////////////////////////////
Mp3DecGetSongPos
    clr     a
    move    y:SongByteTotalConsumedHigh,a1
    move    y:SongByteTotalConsumedLow,a0
    rts

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           DecSetSongPos
;
;   Type:           Function
;
;   Description:    Sets the decoder file position to a specific location.
;                   being decoded.
;                   Expects it to be paused.
;
;   Inputs:         A1 = upper word of 48-bit address
;                   A0 = lower word of 48-bit address
;
;   Outputs:        A = 0 if no error
;                   A1 = error code if unsuccessful
;                   
;   Notes:          Not operational for MPEG decoder yet.
;<
;///////////////////////////////////////////////////////////////////////////////
DecSetSongPos
    ; File must be paused or stopped
    move    x:DecoderSR,x0
    jclr    #Play,x0,_CheckValue
    move    #>ERROR_DECODER_PLAYING,a
    rts
_CheckValue
    clr     b
    move    y:SongByteLengthHigh,b1
    move    y:SongByteLengthLow,b0
    cmp     a,b
    jgt     _SetPosition
    move    #>ERROR_DECODER_SETPOS,a
    rts
_SetPosition
    move    a1,y:SongByteTotalHigh
    move    a0,y:SongByteTotalLow
    clr     a
    rts


;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           DecSetSongPosZero
;
;   Type:           Function
;
;   Description:    Resets the song file pointer to the beginning of the file.
;
;   Inputs:         none
;
;   Outputs:        Updates SongByteTotalHigh & SongByteTotalHigh
;                   
;   Notes:          _asmfunc void DecSetSongPosZero(void)
;<
;///////////////////////////////////////////////////////////////////////////////
DecSetSongPosZero
    clr     a
    move    a,y:SongByteTotalHigh
    move    a,y:SongByteTotalLow
    ; TIME_FIX  Clear precise byte pos vars also.
    move    a,y:SongByteTotalConsumedHigh
    move    a,y:SongByteTotalConsumedLow
    rts

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           DecSetSongPosEnd
;
;   Type:           Function
;
;   Description:    Resets the song file pointer to the end of the file.
;
;   Inputs:         none
;
;   Outputs:        Updates SongByteTotalHigh & SongByteTotalLow
;                   
;   Notes:          _asmfunc void DecSetSongPosEnd(void)
;<
;///////////////////////////////////////////////////////////////////////////////
DecSetSongPosEnd
    move    y:SongByteLengthHigh,x0
    move    x0,y:SongByteTotalHigh
    move    x0,y:SongByteTotalConsumedHigh
    
    move    y:SongByteLengthLow,x0
    move    x0,y:SongByteTotalLow
    move    x0,y:SongByteTotalConsumedLow
    ; TIME_FIX  Sets consumed byte position vars to last byte also
    rts

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           DecClearSongTime
;
;   Type:           Function
;
;   Description:    Clear the song time
;
;   Inputs:         none
;
;   Outputs:        Clears SongTotalMinute/Seconds, SongCurrentMinutes/Seconds,
;                   and SongRemainMinutes/Seconds
;                   
;   Notes:          _asmfunc void DecClearSongTime(void)
;<
;///////////////////////////////////////////////////////////////////////////////
DecClearSongTime
    clr     a
;    move    a,y:SongTotalMinutes
;    move    a,y:SongTotalSeconds
    move    a,y:SongCurrentMinutes
    move    a,y:SongCurrentSeconds
    move    a,y:SongRemainMinutes
    move    a,y:SongRemainSeconds
    rts
        
;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           DecGetSongTime
;
;   Type:           Function
;
;   Description:    Get the current song time
;
;   Inputs:         none
;
;   Outputs:        a1 = Song Minutes
;                   a0 = Song Seconds
;                   x1 = Song Total Minutes
;                   x0 = Song Total Seconds
;
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
DecGetSongTime
    clr     a
    move    y:SongCurrentMinutes,a1
    move    y:SongCurrentSeconds,a0
    move    y:SongTotalMinutes,x1
    move    y:SongTotalSeconds,x0
    rts

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           DecGetCurrentTime
;
;   Type:           Function
;
;   Description:    Get the current song time only
;
;   Inputs:         none
;
;   Outputs:        a1 = Song Seconds
;                   b1 = Song Minutes
;
;   Notes:          _asmfunc SONGTIME DecGetCurrentTime(void);
;<
;///////////////////////////////////////////////////////////////////////////////
DecGetCurrentTime
    clr     a
    move    y:SongCurrentMinutes,b
    move    y:SongCurrentSeconds,a1
    rts


;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysFlushCoderMemory
;
;   Type:           Function
;
;   Description:    Clears out the System Application Space.  This function
;                   is typically used to clear the X, Y, and P memory in
;                   application space just prior to loading an overlay into
;                   the region.  This will eliminate the potential effect of  
;                   residues in memory from the previously resident code.
;
;   Inputs:         none
;
;   Outputs:        none
;                   
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
SysFlushCoderMemory
    move    y:<Const_ffffff,m0          ; linear

    ; clear x memory
    ; non-WMA : x app is 0x0000 to beginning of system
    ; WMA:      x app is 0x0000 to beginning of LTABLES (b_WMA_LX)
    move    #>(F_lc_u_eX_app_cluster-F_lc_u_bX_app_cluster),x1 ; num words to fill
    move    #0,r0                       ; start address
    clr     a
    rep     x1                          ;we can use the rep, decoders aren't playing
        move    a,x:(r0)+

    ; clear y memory
    move    #>(F_lc_u_eY_app_cluster-F_lc_u_bY_app_cluster),x1  ; num words to fill
    move    #F_lc_u_bY_app_cluster,r0                           ; start address
    rep     x1                                                  ;we can use the rep, decoders aren't playing
        move    a,y:(r0)+

   ; clear p memory
    move    #>(F_lc_u_eP_app_cluster-F_lc_u_bP_app_cluster),x1 ; num words to fill
    move    #>F_lc_u_bP_app_cluster,r0  ; start address
    rep     x1                          ;we can use the rep, decoders aren't playing
        move    a,p:(r0)+

    nop
    
    rts


;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysSetDecoder
;
;   Type:           Function
;
;   Description:    Loads new decoder to be executed in the decoder module table.
;
;   Inputs:         
;                   x0 = Decoder flag
;
;   Outputs:        none
;                   
;   Notes:          
;                   Decoders supported are:
;                       DECODER_TYPE_MP3
;                       DECODER_TYPE_ADPCM_IMA
;                       DECODER_TYPE_WMA -- for 3410 and up
;<
;///////////////////////////////////////////////////////////////////////////////
SysSetDecoder

    if (@DEF('FUNCLET'))           
   
       ;If using funclet, function will be executed via funclet kernel
       CallFunclet RSRC_FUNCLET_SYSSETDECODER
   
       org p,"SYSFUNCLET_SysSetDecoder_P":
       
       dc      RSRC_FUNCLET_SYSSETDECODER           ; Funclet Resource number
   
    endif

    bclr    #HW_SR_C_BITPOS,sr


  if (!@def('REMOVE_FM'))
    move    #>RSRC_DEC_STFM_MOD_CODE,y0     ; HACK!!!!
  else
    move    #>RSRC_DEC_ADPCM_MOD_CODE,y0
  endif
    
    bset    #HW_SR_C_BITPOS,sr
;    rts

;    jsset   #DECODER_TYPE_MP3,x0,_type_mp3
;    jsset   #DECODER_TYPE_ADPCM_IMA,x0,_type_adpcm_ima
;    jsset   #DECODER_TYPE_STFM,x0,_type_stfm

;    if (@def('DEVICE_3400')||@def('DEVICE_3300'))
;        ; not enough room for wma
;    else
;        ; wma supported
;        jsset   #DECODER_TYPE_WMA,x0,_type_wma
;    endif

;    jcc     _type_error
    move    y0,y:DecoderResourcePtr
    rts

_type_mp3
    move    #>RSRC_DECMOD_CODE,y0
    bset    #HW_SR_C_BITPOS,sr
    rts
_type_adpcm_ima
    move    #>RSRC_DEC_ADPCM_MOD_CODE,y0
    bset    #HW_SR_C_BITPOS,sr
    rts
    
    if (!@def('REMOVE_FM'))
_type_stfm
    move    #>RSRC_DEC_STFM_MOD_CODE,y0
    bset    #HW_SR_C_BITPOS,sr
    rts
    endif
_type_wma
    if (@def('DEVICE_3400')||@def('DEVICE_3300'))
        ; not enough room for wma
    else
        ; wma supported
        move    #>RSRC_WMADEC_CODE,y0		
        bset    #HW_SR_C_BITPOS,sr
        rts
    endif
_type_aac
_type_error
    error

    if (@DEF('FUNCLET'))
   
       ;If using funclet, switch counter back to original here
       org    p,"SYSFUN_SysCoder_P":                             
       
    endif


;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           DecoderForceInit
;
;   Type:           Function
;
;   Description:    Initializes the current decoder
;
;   Inputs:         none
;
;   Outputs:        none
;                   
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
DecoderForceInit
    ; if we're starting to play then the decoder needs to init
    move    #>MODULE_NUM_DECODER,x1
    move    #>EVENT_INIT,x0
    jsr     SignalModule
    rts    

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysSetDecoderResource
;
;   Type:           Function
;
;   Description:    Loads new decoder to be executed in the decoder module table.
;
;   Inputs:         
;                   a = Decoder resource number
;
;   Outputs:        none
;                   
;   Notes:          **Similar to SysSetDecoder, but loads resource directly from Acc. a.
;                   Decoders supported are:
;                       DECODER_TYPE_MP3
;                       DECODER_TYPE_ADPCM_IMA
;                       DECODER_TYPE_WMA -- for 3410 and up
;<
;///////////////////////////////////////////////////////////////////////////////
SysSetDecoderResource
    move    a1,y:DecoderResourcePtr
    rts
    
;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysSetEncoder
;
;   Type:           Function
;
;   Description:    Loads new encoder to be executed in the encoder module table.
;
;   Inputs:         
;                   x0 = Encoder flag
;
;   Outputs:        none
;                   
;   Notes:          
;                   Encoders supported are:
;                       ENCODER_TYPE_ADPCM_IMA
;<
;///////////////////////////////////////////////////////////////////////////////
SysSetEncoder

    if (@DEF('FUNCLET'))           
   
       ;If using funclet, function will be executed via funclet kernel
       CallFunclet RSRC_FUNCLET_SYSSETENCODER
   
       org p,"SYSFUNCLET_SysSetEncoder_P":
       
       dc      RSRC_FUNCLET_SYSSETENCODER           ; Funclet Resource number
   
    endif

    bclr    #HW_SR_C_BITPOS,sr
    jsset   #ENCODER_TYPE_ADPCM_IMA,x0,_type_adpcm_ima
    jsset   #ENCODER_TYPE_MP3,x0,_type_mp3
    jcc     _type_error
    move    y0,y:EncoderResourcePtr
    rts

_type_adpcm_ima
    move    #>RSRC_ENC_ADPCM_MOD_CODE,y0
    bset    #HW_SR_C_BITPOS,sr
    rts

_type_mp3
    if (@def('MP3_ENCODE'))
    move    #>RSRC_ENC_MP3_MOD_CODE,y0
    bset    #HW_SR_C_BITPOS,sr
    rts
    endif

_type_pcm
_type_error
    error

    if (@DEF('FUNCLET'))
   
       ;If using funclet, switch counter back to original here
       org    p,"SYSFUN_SysCoder_P":                             
       
    endif
    

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           EncoderForceInit
;
;   Type:           Function
;
;   Description:    Initializes the current encoder
;
;   Inputs:         none
;
;   Outputs:        none
;                   
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
EncoderForceInit
    ; if we're starting to record then the encoder needs to init
    move    #>MODULE_NUM_CAPTURE,x1
    move    #>EVENT_INIT,x0
    jsr     SignalModule
    rts    
    
;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           StopDecoder
;
;   Type:           Function
;
;   Description:    Stops the current decoder
;
;   Inputs:         none
;
;   Outputs:        none
;                   
;   Notes:          none
;<
;///////////////////////////////////////////////////////////////////////////////
SysStopDecoder
    
    if (@DEF('FUNCLET'))           
   
       ;If using funclet, function will be executed via funclet kernel
       CallFunclet RSRC_FUNCLET_SYSSTOPDECODER
   
       org p,"SYSFUNCLET_SysStopDecoder_P":
       
       dc      RSRC_FUNCLET_SYSSTOPDECODER           ; Funclet Resource number
   
    endif

    jsr     DecoderGetStatus
    jset    #Stop,x0,_stopped

    ;post the stop message to the decoder.
    move    r7,r0
    move    #>2,y0
    push    y0
    move    #>DECODER_STOP,y0
    push    y0
    move    y:<Const_ffffff,m0
    jsr     SysPostMessage
    pop     y0
    pop     y0

	;No direct SysLoadResource as it will mess up the kernel

    ;Load and Execute decoder via syscallmodule (which is same as syscallfunction2400)
    move    #>EVENT_MESSAGE,a1
    move    #DecoderQueueDescriptor,r0 
    move    y:<Const_ffffff,m0
    move    y:DecoderProcessPtr,r6          ;function to call
	move    y:DecoderResourcePtr,n6         ; Module resource number (decoder overlay code)
    jsr     SysCallModule

    ;Wait for the decoder to stop
_WaitStop
    jsr     DecoderGetStatus
    jset    #Stop,x0,_stopped
    jmp     _WaitStop

_stopped
    rts
    
    if (@DEF('FUNCLET'))
   
       ;If using funclet, switch counter back to original here
       org    p,"SYSFUN_SysCoder_P":                             
       
    endif

    
;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           StopEncoder
;
;   Type:           Function
;
;   Description:    Stops the current encoder
;
;   Inputs:         none
;
;   Outputs:        none
;                   
;   Notes:          none
;<
;///////////////////////////////////////////////////////////////////////////////
SysStopEncoder
    if (@DEF('FUNCLET'))           
   
       ;If using funclet, function will be executed via funclet kernel
       CallFunclet RSRC_FUNCLET_SYSSTOPENCODER
   
       org p,"SYSFUNCLET_SysStopEncoder_P":
       
       dc      RSRC_FUNCLET_SYSSTOPENCODER           ; Funclet Resource number
   
    endif

    jsr     EncoderGetStatus
    jset    #Pause,x0,_Encorder_Start_again
    jset    #Stop,x0,_stopped
_Stop_Encoder_Begin
    ;post the stop message to the encoder.
    move    r7,r0
    move    #>2,y0
    push    y0
    move    #>ENCODER_STOP,y0
    push    y0
    move    y:<Const_ffffff,m0
    jsr     SysPostMessage
    pop     y0
    pop     y0

	;No direct SysLoadResource as it will mess up the kernel

    ;Load and Execute encoder via syscallmodule (which is same as syscallfunction2400)
    move    #>EVENT_MESSAGE,a1
    move    #CaptureQueueDescriptor,r0 
    move    y:<Const_ffffff,m0
    move    y:EncoderProcessPtr,r6
	move    y:EncoderResourcePtr,n6         ; Module resource number
    jsr     SysCallModule

    ;Wait for the encoder to stop
_WaitStop
    jsr     EncoderGetStatus
    jset    #Stop,x0,_stopped
    jmp     _WaitStop

_stopped
    rts

_Encorder_Start_again
    ;post the stop message to the encoder.
    move    r7,r0
    move    #>2,y0
    push    y0
    move    #>ENCODER_RECORD,y0
    push    y0
    move    y:<Const_ffffff,m0
    jsr     SysPostMessage
    pop     y0
    pop     y0
    jmp     _Stop_Encoder_Begin

    if (@DEF('FUNCLET'))
   
       ;If using funclet, switch counter back to original here
       org    p,"SYSFUN_SysCoder_P":                             
       
    endif

    endsec


