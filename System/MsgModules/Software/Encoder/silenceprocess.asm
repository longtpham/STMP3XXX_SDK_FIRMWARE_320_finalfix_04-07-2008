;;***********************************************************************
;SigmaTel, Inc
;$Archive:  $
;$Revision: 2 $
;$Date: 1/24/04 12:14p $
;Description:  silenceprocess.asm
;Notes:	This is the main file. 		
; TBD - Move DCOffset code into a separate file
;***********************************************************************/
    Section	SilenceProcess
;************************************************************************
;  Include files
;************************************************************************
; Use relative paths only
    include "sysmacro.asm"
	include "const.xref"
    include "sysequ.inc"

;************************************************************************
;	Global Variables
;************************************************************************
    global FindEnergy,SilenceProcess
    global FindDCOffset

;************************************************************************
;	Extern Variables
;************************************************************************

    extern  Fg_wEncNumberOfChannels
    extern  FSilenceSR
    extern  FDetectSilence
    extern  GetEmptySpace
    extern  g_wEncoderMode
  
;************************************************************************
;  Code start
;************************************************************************
	org p,"SilenceProcess":
;////////////////////////////////////////////////////////////////////////
;   Name:           FindDCOffset
;   Type:           Function
;   Description:    Finds DC bias over a block of Samples
;   Inputs:         A : Ptr to input buffer
;                   B : No. Of Samples to be processed
;                   x0: NumChannels
;                   y0: Modulo value for source buffer
;   Output:         DC bias over a block of Samples
;   Return Value:   DC bias over a block of Samples
;   Registers Trashed : r0,n0,A,B
;   Notes:          
;////////////////////////////////////////////////////////////////////////
FindDCOffset 
    move A,r0
    move x0,n0
    move y0,m0
    clr  A
    move y:(r0)+n0,x0      
    rep  B
    add x0,A         y:(r0)+n0,x0
    
;;;    move y:(r0)-n0,x0   ;; backup r0 by 2
;;;    move y:(r0)-n0,x0
;;;    move #0,x0          ;; zero out buffer
;;;    rep B
;;;    move x0,y:(r0)-n0
  
    move #>$ffff,m0
    rts

;////////////////////////////////////////////////////////////////////////
;   Name:           FindEnergy
;   Type:           Function
;   Description:    Finds energy over a block of Samples
;   Inputs:         A : Ptr to input buffer
;                   B : No. Of Samples to be processed
;                   x0: NumChannels
;                   y0: Modulo value for source buffer
;   Output:         energy over a block of Samples
;   Return Value:   energy over a block of Samples
;   Registers Trashed : r0,n0
;   Notes:          
;////////////////////////////////////////////////////////////////////////
FindEnergy
    move A,r0
    move x0,n0
    move y0,m0
    clr  A
    move y:(r0)+n0,x0
    rep  B
        mac x0,x0,A         y:(r0)+n0,x0
    move #>$ffff,m0
       
    rts
   
   
;////////////////////////////////////////////////////////////////////////
;   Name:           SilenceProcess
;   Type:           Function
;   Description:    Determines Silence and also handles pointers correctly to skip
;   Inputs:         r0 : Ptr to Source Descriptor
;   Output:         Sets SilenceSR
;   Return Value:   1 to Skip Encoding, 0 to continue to encode
;   Registers Trashed : Assume all
;   Notes:          
;////////////////////////////////////////////////////////////////////////
SilenceProcess
    ; album mode or song mode?
    move    y:g_wEncoderMode,A
    tst     A
    jeq     _ProcessSamples

    move    y:Fg_wEncNumberOfChannels,A
    push    r0
    ; silence detection block
    jsr     FDetectSilence
    pop     r0
    tst     A
    jeq     _AudioDetected
    bset    #0,y:FSilenceSR                             ; silence detected for the first time?
    jcs     _DropSamples
    bset    #1,y:FSilenceSR                             ; file close flag
    jne     _ExitIsr

_AudioDetected
    bclr    #0,y:FSilenceSR                             ; audio detected
    btst    #1,y:FSilenceSR                             ; check whether the new file is available.
    jcc    _ProcessSamples

_DropSamples
   ; this is to prevent the encoder input buffer from overflowing
    move    r0,r4
    push    r4
    move    #$ffff,m4
    jsr     GetEmptySpace                               ; returns Empty space count in A
    pop     r4
	move	y:<Const_000080,B                           ; compare with 128 samples
    move    y:Fg_wEncNumberOfChannels,x0
    rep     x0
    asl     B
    asr     B
    move    B,x0
    cmp     x0,A
    jgt     _ExitIsr                                    ; exit if enough space is available
    ; Else Drop samples = 128 * NumChannels
    move    #BD_BufferModulo,n4
    move    x0,n0
    move    y:(r4+n4),m0
    move    #BD_TailPtr,n4
    nop
    move    y:(r4+n4),r0
    nop
    move    (r0)+n0
    move    r0,y:(r4+n4)
    move    y:<Const_ffffff,m0
_ExitIsr                       
    move   y:<Const_000001,A
    rts

_ProcessSamples
    clr     A
    rts   

    endsec 
