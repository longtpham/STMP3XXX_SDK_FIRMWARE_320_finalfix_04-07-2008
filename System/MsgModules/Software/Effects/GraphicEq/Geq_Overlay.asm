;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2003
;
; STMP3xxx User-level Geq Effects Module (Can be overlayed)        
;///////////////////////////////////////////////////////////////////////////////
        opt     mex

        section Geq_Overlay


;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////
;        nolist
    include "sysequ.inc"
    include "msgequ.inc"
    include "resource.inc"
    include "geqequ.inc"
    
    if @def('SPECTRUM_ANAL')    
      include "effects.inc"
    endif
    
    include "sysmacro.asm"
    include "project.inc"   

    include "stack.xref"
    include "stackmem.xref"
    include "sysmem.xref"
    include "sysresources.inc"
    include "const.xref"
    include "sysmisc.xref"
    include "sysresource.xref"

    if @def('SPECTRUM_ANAL')
      include "spectralanal.xref"
    endif
        page    132,60,0,0,0



;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////
    Global GeqInit,GeqOverlayModule
;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////
    extern    UserScratchY
    extern x_filter_state_base_odd
    extern x_filter_state_base_even
    extern y_filter_state_base_odd
    extern y_filter_state_base_even
    extern currentdBgainbase
    extern TargetdBGainBase
    extern GeqQueueDescriptor
    extern GeqModuleBase
    extern ZerodBPoint
    extern CenterFreq
    extern FilterCoefs
    extern Geq_Quadrant  
    extern Geq_sn        
    extern Geq_cs        
    extern Geq_alpha_by_4
    extern Filter_1_by_2Q
    extern FSysUpdateVolume
	extern SysPostMessage

;///////////////////////////////////////////////////////////////////////////////
;  Macros
;///////////////////////////////////////////////////////////////////////////////


;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;   X Memory
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;   Y Memory
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;   Code
;///////////////////////////////////////////////////////////////////////////////
       org      p,"GeqOverlay_overlay_P":
      
        ; Resource number
        dc      RSRC_GEQOVERLAY_CODE

;;***********************************************************************
;   Function:         GeqInit
;   Description:      Initializes the Graphic equalizer filter states, sets 
;                     current gains and initializes the sink buffer.
;  Input Parameters:  r0 = GeqRoutineDescriptor Ptr
;  Outputs:           a = wait flag that Executive will store in GEQ ModuleTableEntry
;;***********************************************************************
GeqInit

    ; save off stack
    move    #>stackSaved,r6              ; setup the return address
    move    #>UserScratchY,r5             ; pointer to the SP local save
    jmp     SaveSystemStack              ; save the hardware stack
stackSaved
    clr     A
    move    A,y:g_VolumeBias
    jsr     SetdBTableEntriesEqual
    jsr     Reset_Filter_states
    
    if @def('SPECTRUM_ANAL')    
      jsr        SpectralAnalInit
    endif

_restoreStack
    move    #>UserScratchY,r5             ; pointer to the SP local save
    move    #>_stackRestored,r6           ; setup the return address
    jmp     RestoreSystemStack           ; restore the hardware stack
_stackRestored
    ; Wait to call the GEQ module's process function until a message arrives
    move    #>EVENT_MESSAGE,A
    rts
    
;-------------------------------------------------------------------------------
Reset_Filter_states
    move    #>x_filter_state_base_odd,r3
    move    #0,x1

    move    x1,x:(r3)+                          ; Initialize the x filter states
    move    x1,x:(r3)+                          ; Initialize the x filter states


    move    #>x_filter_state_base_even,r3
    nop
    move    x1,x:(r3)+                          ; Initialize the x filter states
    move    x1,x:(r3)+                          ; Initialize the x filter states

    move    #>y_filter_state_base_odd,r3
    rep     #2*NumberOfBands
    move    x1,x:(r3)+                          ; Initialize the x filter states

    move    #>y_filter_state_base_even,r3
    rep     #2*NumberOfBands
    move    x1,x:(r3)+                          ; Initialize the x filter states

    rts

;-------------------------------------------------------------------------------
SetdBTableEntriesEqual
    move    #currentdBgainbase,r5
    move    #TargetdBGainBase,r3
    do     #NumberOfBands,_EndFilterBanks
        move    y:(r3)+,A
        move    A,y:(r5)+
_EndFilterBanks
    rts



;;***********************************************************************
;   Function:
;           SetTargetGain
;   Description:
;           This routine shows how the target gains are set.
;  Parameters passed: 
;           y1 = Band No.
;           y0 = Gain in 0.5 dB units.
;   Return value:
;           None.
;;***********************************************************************
SetTargetGain
    ; Check the range of y0. y0 lies in [-28,28]
    move    #>-`28,A
    cmp     y0,A
    jle     _Continue
    rts
_Continue
    move    #>`28,A
    cmp     y0,A
    jge     _Continue1
    rts
_Continue1
    move    #TargetdBGainBase,r2
    move    y1,n2
    move    y:<Const_ffffff,m2
    move    #>ZerodBPoint,A
    add     y0,A
    move    A,y:(r2+n2)                             ; Set the value of the ptr

    rts


;*******************************************************************************
;  Function:          GeqOverlayModule
;  Description:       Geq interface module
;  Parameters passed: None
;  Registers Trashed: All
;  Return Values:     a1 is word specifying events to wait for before calling this
;                     module process function next.
;*******************************************************************************
GeqOverlayModule
        nop
        ; save off stack
        move    #_stackSaved,r6              ; setup the return address
        move    #UserScratchY,r5             ; pointer to the SP local save
        jmp     SaveSystemStack              ; save the hardware stack
_stackSaved

_ProcessSysMessages
        ; switch on system messages
        move    #_queueProcessed,r6
        jmp     GeqProcessQueue
_queueProcessed

        ; restore stack
        move    #>UserScratchY,r5             ; pointer to the SP local save
        move    #>_stackRestored,r6           ; setup the return address
        jmp     RestoreSystemStack           ; restore the hardware stack
_stackRestored
	move y:<Const_ffffff,m1
        ;// Want to update the system volume for the EQ auto gain feature
        if (@def(MULTI_STAGE_VOLUME))
            jsr     FSysUpdateVolume
        endif
        ; I want to be called again when I have a message
        move    #EVENT_MESSAGE,a1
        rts


;-------------------------------------------------
;*******************************************************************************
;  Function:
;             GeqProcessQueue
;  Description:
;         Deque messages passed to the Geq
;             r0=Descriptor for this modules message queue (in y memory)
;             m0=linear
;  Parameters passed: 
;         None
;  Registers Trashed: 
;             All
;  Return Values:
;             None
;*******************************************************************************
GeqProcessQueue
        push    r6                        ; needed for return

        move    #GeqQueueDescriptor,r0
        move    y:<Const_ffffff,m0

        ; Put the queue modulo into m1
        move    #>BD_BufferModulo,n0
        nop
        move    y:(r0+n0),m1

        ; Put the head pointer into a
        move    #BD_HdPtr,n0
        nop
        move    y:(r0+n0),a

        ; Put the tail pointer into b
        move    #BD_TailPtr,n0
        nop
        move    y:(r0+n0),b

        ; Check if the queue is empty
        jmp     _CheckDone

_ProcessMessage
        ; Put the tail pointer into r1
        move    b1,r1

        ; Save working variables
        push    a1                      ; Head pointer
        push    r0                      ; Pointer to queue descriptor
        push    r1                      ; Tail pointer
        push    m1                      ; Queue modulo

        ; Put the message ID into b
        move    #>MSG_ID,n1
        nop
        move    y:(r1+n1),b

        ; Verify that the message ID is valid by checking message range
        move    #>GEQ_FIRST_MSG_ID,x0
        cmp     x0,b
        jlt     _InvalidMessageID
        move    #>GEQ_LAST_MSG_ID,x0
        cmp     x0,b
        jgt     _InvalidMessageID

        ; Point r1 to the first argument of the message
        move    (r1)+
        move    (r1)+

        ; Clear the upper byte of the message ID
        move    #$00FFFF,x0
        and     x0,b

        ; Jump to the message handler
        move    #_JumpTable,r2
        move    y:<Const_ffffff,m2
        move    b1,n2
        nop
        movem   p:(r2+n2),r3
        nop
        jmp     (r3)

;------------------------- GeqSetEQ Function-------------------------
_GeqSetEQ
        ; Pass to Enable or disable the Eq.
        ; Length of message 2+1
        move    #GeqModuleBase,r2
        move    y:(r1)+,A               ; Pass 1 to enable the Eq, 0 to disable.
        move    A,y:(r2)
        jmp     _NextMessage

_GeqSetEQLegacy
        ; If legacy eq is used, always disable the Eq.
        ; Length of message 2
        move    #GeqModuleBase,r2
        clr     A                       ; Disable the Graphic Equalizer 
        move    A,y:(r2)
        jmp     _NextMessage

_GeqSetAllBandGains
        ; Length of message 2+5
        clr     A
        move    A,y:g_VolumeBias
        move    #0,r2
        do      #NumberOfBands,_EndSetBandGain
            move    y:(r1)+,y0
            tfr     y0,A    y:g_VolumeBias,x1
            cmp     x1,A
            tlt     x1,A
            move    A,y:g_VolumeBias
            move    r2,y1
            move    (r2)+
            push    r1
            push    r2
            jsr     SetTargetGain
            pop     r2
            pop     r1
            nop
_EndSetBandGain
        jmp     _NextMessage

_GeqSetGain
        ; Length of message 2+2
        move    y:(r1)+,y1              ; Band No.
        move    y:(r1)+,y0              ; Band Gain

        tfr     y0,A    y:g_VolumeBias,x1
        cmp     x1,A
        tlt     x1,A
        move    A,y:g_VolumeBias

        push    r1
        jsr     SetTargetGain
               
        pop     r1
        jmp     _NextMessage
_GeqSetParam
        ; Length of message 2+10
        move    #Filter_1_by_2Q,r4
        do      #NumberOfBands,_EndSetFilterQ
            move    y:(r1)+,A
            move    A,x:(r4)+
_EndSetFilterQ        
        ; Update the center frequencies.
        move    #CenterFreq,r2
        do      #NumberOfBands,_EndSetCenterFreq
            move y:(r1)+,A
            move A,x:(r2)+
_EndSetCenterFreq        
        jmp     _NextMessage
_GeqSetCoefs
        ; Length of message 2+1
        move    y:(r1)+,A
        move    #Filter_1_by_2Q,r4
        move    #FilterCoefs,r3
        move    #CenterFreq,r2
        do      #NumberOfBands,_EndUpdateFilter
            push    A                       ; BackUpSamplingRate
            move    x:(r2)+,B
            move    x:(r4)+,x0
            ; A = samprate, B = fc, x0 = 1/2Q
            ; r3 = ptr to current filter
            jsr     FindBandPassCoefs
            pop     A
_EndUpdateFilter                
    if @def('SPECTRUM_ANAL')
        push    A                       ; BackUpSamplingRate
    endif    
        
        jsr     Reset_Filter_states
    
    if @def('SPECTRUM_ANAL')    
        pop     A
; Also set the spectrum analyzer coefficients, according to sampling frequencies.
        move    #SAFilter_1_by_2Q,r4
        move    #SAFilterCoefs,r3
        move    #SACenterFreq,r2
        do      #NumberOfBands,_EndUpdateSAFilter
            push    A                       ; BackUpSamplingRate
            move    x:(r2)+,B               ; Increment the Center frequencies.
            move    x:(r4),x0               ; Same Q for all Spectral Analyzer coefficients.
            ; A = samprate, B = fc, x0 = 1/2Q
            ; r3 = ptr to current filter
            jsr     FindBandPassCoefs
            pop     A
_EndUpdateSAFilter        
        jsr     SpectralAnalInit
    endif
        
        jmp     _NextMessage

_GeqGetSettings
        move    #TargetdBGainBase,r2
        move    y:<Const_ffffff,m2
        move    y:<Const_ffffff,m0
        move    r7,r0
        move    #>NumberOfBands+2,x0
        push    x0
        move    #>MENU_MSG_EQ_SETTINGS,x0
        push    x0
        
        move    #>ZerodBPoint,y0
        do   #NumberOfBands,_EndDoEqSave
            move    y:(r2)+,A
            sub     y0,A
            push    A                               ; Return value is in steps of 0.5 dB
_EndDoEqSave

        jsr     SysPostMessage

        pop     x0                                  ; Message length
        pop     x0                                  ; Id        
        do   #NumberOfBands,_EndDoEqStack
            pop  x0                                 ; Content.
            nop
_EndDoEqStack

        jmp     _NextMessage



    ;-------------------------Invalid Message---------------------------
_InvalidMessageID

        error

_NextMessage
        ; Restore working variables
        pop     m1                      ; Queue modulo
        pop     r1                      ; Tail pointer
        pop     r0                      ; Pointer to buffer descriptor
        pop     a                       ; Head pointer

        ; Add the message length to the tail pointer
        move    y:(r1),n1
        nop
        move    (r1)+n1
        nop
        move    r1,b

_CheckDone
        ; Check if head pointer equals the tail pointer
        cmp     a,b
        jne     _ProcessMessage

        ; Update the buffer descriptor with the new tail pointer
        move    #>BD_TailPtr,n0
        nop
        move    b1,y:(r0+n0)

        pop     r6                        ; return address
        nop
        jmp     (r6)                      ; return to GeqModule routine

; Note: It is important that the entries in the jump table be in the
; same order as the Geq message ID's which are defined in msgequ.inc
_JumpTable

        dc      _GeqSetEQ
        dc      _GeqSetEQLegacy
        dc      _GeqSetAllBandGains
        dc      _GeqSetGain
        dc      _GeqSetCoefs        
        dc      _GeqSetParam
        dc      _GeqGetSettings




;-------------------------------------------------
;*******************************************************************************
;  Function:
;             FindBandPassCoefs
;  Description:
;     Computes filter coefficients based on provided specs.    
;  Parameters passed: 
;     r3 contains pointer to current filter.
;     A = SampleRate, B = Fc, x0 = 1/(2*Q)
;  Registers Trashed: 
;             All
;  Return Values:
;     r3 is incremented to point to the next filter.
;*******************************************************************************
    ; We wont support Q less than 0.5 or 1/(2*Q) must be less than 1    
    ; Passed fc,Sample Rate, 1/2*Q
    ; A = SampleRate, B = Fc, x0 = 1/(2*Q)
FindBandPassCoefs    
    push x0
    asr A       #0,x0
    cmp A,B     A,y0
    ; Clear the filter values.
    move x0,y:(r3)+
    move x0,y:(r3)+
    move x0,y:(r3)+
    ; Incorrect specification of frequencies
    jge _AllCoefZeroes
    move (r3)-
    move (r3)-
    move (r3)-
    and #$fe,ccr
    rep #24
        div y0,B
    add y0,B
    ; Result in A0

    ; Find the Geq_Quadrant to which this belongs to.
    clr  B      B0,x0
    move B,x:Geq_Quadrant
    tfr  x0,B   y:<Const_7fffff,A  
    sub  x0,A   y:<Const_400000,y0
    cmp  y0,A
    jgt  _Geq_Quadrant_I
    tfr  A,B    y0,x:Geq_Quadrant          ; Put non-zero value for Geq_Quadrant 
_Geq_Quadrant_I
    move B,y0
    mpy  y0,#14,A
    push A0
    push A1
    clr  B       #SineTable,y0
    add  y0,A       A0,B1
    lsr  B      A,r0
    move y:<Const_7fffff,y0
    sub  y0,B       B,x0
    move p:(r0)+,y1
    neg  B          
    move p:(r0),y0
    ; x*A[n+1] + (1-x)*A[n]
    mpy  x0,y0,A    B,x0
    macr  x0,y1,A
    move  A,x:Geq_sn

    pop  y0             
    clr  B      #CosTable,A
    pop  B1
    sub  y0,A       
    lsr  B      A,r0
    move y:<Const_7fffff,y0
    sub  y0,B             B,x0
    move p:(r0)-,y1
    neg  B
    move p:(r0),y0
    ; x*A[n+1] + (1-x)*A[n]
    mpy  x0,y0,A    B,x0
    macr  x0,y1,A
    move  A,x:Geq_cs

;   Geq_alpha = Geq_sn/(2*Q);
    pop  x0
    move x:Geq_sn,y0
    mpy  x0,y0,A
    asr  A          y:<Const_400000,x0
    add  x0,A       A,B
    asr  B          A,x0                ; x0 now contains (1 + Geq_alpha)/2
    move B,x:Geq_alpha_by_4


    move x:Geq_alpha_by_4,y0
    move y:<Const_200000,B
    sub  y0,B                          ; B will always be positive 

; a2/2 = (1/4 - Geq_alpha/4)/(1/2 + Geq_alpha/2)
    and #$fe,ccr
    rep #$18
    div x0,B
    add x0,B
    move B0,y:(r3)+


; Find b0/2 = (Geq_alpha/4)/(1/2 + Geq_alpha/2)
    move x:Geq_alpha_by_4,B
    and #$fe,ccr
    rep #$18
    div x0,B
    add x0,B
    move B0,y:(r3)+



    move x:Geq_cs,B
; -a1/2 = (Geq_cs/2)/(1/2 + Geq_alpha/2)
    asr B
    and #$fe,ccr
    rep #$18
    div x0,B
    add x0,B    x:Geq_Quadrant,A
    tst A       B0,B
    jeq _ContinueGeq_Quadrant_I
    neg B
_ContinueGeq_Quadrant_I
    move B,y:(r3)+
    rts

_AllCoefZeroes
    pop x0
    rts

;   b0 = Geq_alpha;
;   b1 = 0;
;   b2 = -Geq_alpha;
;   a0 = 1 + Geq_alpha;
;   a1 = -2*Geq_cs;
;   a2 = 1 - Geq_alpha;
    


;;***********************************************************************
;    Function:
;            SpectralAnalInit
;    Description:
;            Initializes the Spectrum analyser filter states and other variables.
;  Parameters passed: 
;            
;;***********************************************************************
    if @def('SPECTRUM_ANAL')
SpectralAnalInit
    move    #>SA_x_filter_state_ch0,r3
    move    #0,x1

    move    x1,x:(r3)+                            ; Initialize the x filter states
    move    x1,x:(r3)+                            ; Initialize the x filter states

    move    #>SA_x_filter_state_ch1,r3
    move    #0,x1

    move    x1,x:(r3)+                            ; Initialize the x filter states
    move    x1,x:(r3)+                            ; Initialize the x filter states


    move    #>SA_y_filter_state_ch0,r3
    nop
    move    x1,x:(r3)+                            ; Initialize the y filter states
    move    x1,x:(r3)+                            ; Initialize the y filter states

    move    #>SA_y_filter_state_ch1,r3
    nop
    move    x1,x:(r3)+                            ; Initialize the y filter states
    move    x1,x:(r3)+                            ; Initialize the y filter states

    move     x1,y:CurBandSampleCount        
    move     x1,y:EvalThisbandFlag        

    move     #>SAFilterCoefs,x1
    move     x1,y:SpectralAnalCurFilter    
    move     y:SAEvalInterval,A
    asr        A         A,y:TotalBandSampleCount    
    move     A,y:HalfBandSampleCount        

    move    #0,x1

    move    #>SpectralValArray_ch0,r3                ; Initialize the spectral value array.
    rep     #SANumberOfBands
    move    x1,y:(r3)+                            
    move    #>SpectralValArray_ch1,r3                ; Initialize the spectral value array.
    rep     #SANumberOfBands
    move    x1,y:(r3)+                            
    move     x1,y:SpectralAnalCurBandCnt
    move     x1,y:PeakSampleValue_ch0            
    move     x1,y:PeakSampleValue_ch1            
    move     #>3,x1    
    move     x1,y:SpectralAnalDecay        
    rts
    
    endif
;-------------------------------------------------
;*******************************************************************************
;  Table:
;     Sine/Cosine Computations
;  Description:
;     For 0 to pi/2
;*******************************************************************************

        dc $0
SineTable
        dc $000000
        dc $00c910
        dc $01921d
        dc $025b27
        dc $03242b
        dc $03ed27
        dc $04b619
        dc $057f00
        dc $0647d9
        dc $0710a3
        dc $07d95c
        dc $08a201
        dc $096a90
        dc $0a3309
        dc $0afb68
        dc $0bc3ac
        dc $0c8bd3
        dc $0d53dc
        dc $0e1bc3
        dc $0ee387
        dc $0fab27
        dc $1072a0
        dc $1139f1
        dc $120117
        dc $12c810
        dc $138edc
        dc $145577
        dc $151be0
        dc $15e214
        dc $16a813
        dc $176dda
        dc $183367
        dc $18f8b8
        dc $19bdcc
        dc $1a82a0
        dc $1b4733
        dc $1c0b82
        dc $1ccf8d
        dc $1d9350
        dc $1e56ca
        dc $1f19f9
        dc $1fdcdc
        dc $209f70
        dc $2161b4
        dc $2223a5
        dc $22e542
        dc $23a688
        dc $246777
        dc $25280c
        dc $25e846
        dc $26a822
        dc $27679e
        dc $2826b9
        dc $28e571
        dc $29a3c5
        dc $2a61b1
        dc $2b1f35
        dc $2bdc4e
        dc $2c98fc
        dc $2d553b
        dc $2e110a
        dc $2ecc68
        dc $2f8752
        dc $3041c7
        dc $30fbc5
        dc $31b54a
        dc $326e55
        dc $3326e3
        dc $33def3
        dc $349682
        dc $354d90
        dc $36041b
        dc $36ba20
        dc $376f9e
        dc $382494
        dc $38d8ff
        dc $398cdd
        dc $3a402e
        dc $3af2ef
        dc $3ba51e
        dc $3c56ba
        dc $3d07c2
        dc $3db833
        dc $3e680b
        dc $3f174a
        dc $3fc5ed
        dc $4073f2
        dc $412159
        dc $41ce1e
        dc $427a42
        dc $4325c1
        dc $43d09b
        dc $447acd
        dc $452457
        dc $45cd36
        dc $467568
        dc $471ced
        dc $47c3c2
        dc $4869e6
        dc $490f58
        dc $49b415
        dc $4a581d
        dc $4afb6d
        dc $4b9e04
        dc $4c3fe0
        dc $4ce100
        dc $4d8163
        dc $4e2106
        dc $4ebfe9
        dc $4f5e09
        dc $4ffb65
        dc $5097fc
        dc $5133cd
        dc $51ced4
        dc $526912
        dc $530285
        dc $539b2b
        dc $543302
        dc $54ca0a
        dc $556041
        dc $55f5a5
        dc $568a35
        dc $571def
        dc $57b0d2
        dc $5842dd
        dc $58d40f
        dc $596465
        dc $59f3de
        dc $5a827a
        dc $5b1036
        dc $5b9d11
        dc $5c290b
        dc $5cb421
        dc $5d3e52
        dc $5dc79d
        dc $5e5001
        dc $5ed77d
        dc $5f5e0e
        dc $5fe3b4
        dc $60686d
        dc $60ec38
        dc $616f14
        dc $61f100
        dc $6271fa
        dc $62f202
        dc $637115
        dc $63ef33
        dc $646c5a
        dc $64e889
        dc $6563c0
        dc $65ddfc
        dc $66573d
        dc $66cf81
        dc $6746c8
        dc $67bd10
        dc $683258
        dc $68a69f
        dc $6919e3
        dc $698c24
        dc $69fd61
        dc $6a6d99
        dc $6adcc9
        dc $6b4af2
        dc $6bb813
        dc $6c2429
        dc $6c8f35
        dc $6cf935
        dc $6d6228
        dc $6dca0d
        dc $6e30e3
        dc $6e96aa
        dc $6efb5f
        dc $6f5f03
        dc $6fc194
        dc $702311
        dc $708379
        dc $70e2cc
        dc $714108
        dc $719e2d
        dc $71fa39
        dc $72552d
        dc $72af06
        dc $7307c4
        dc $735f66
        dc $73b5ec
        dc $740b54
        dc $745f9e
        dc $74b2c9
        dc $7504d3
        dc $7555bd
        dc $75a586
        dc $75f42c
        dc $7641af
        dc $768e0f
        dc $76d94a
        dc $77235f
        dc $776c4f
        dc $77b418
        dc $77faba
        dc $784033
        dc $788484
        dc $78c7ac
        dc $7909a9
        dc $794a7c
        dc $798a24
        dc $79c89f
        dc $7a05ef
        dc $7a4211
        dc $7a7d05
        dc $7ab6cc
        dc $7aef63
        dc $7b26cb
        dc $7b5d04
        dc $7b920c
        dc $7bc5e3
        dc $7bf888
        dc $7c29fc
        dc $7c5a3d
        dc $7c894c
        dc $7cb727
        dc $7ce3cf
        dc $7d0f42
        dc $7d3981
        dc $7d628b
        dc $7d8a5f
        dc $7db0fe
        dc $7dd667
        dc $7dfa99
        dc $7e1d94
        dc $7e3f58
        dc $7e5fe5
        dc $7e7f39
        dc $7e9d56
        dc $7eba3a
        dc $7ed5e6
        dc $7ef058
        dc $7f0992
        dc $7f2192
        dc $7f3858
        dc $7f4de4
        dc $7f6237
        dc $7f754e
        dc $7f872c
        dc $7f97cf
        dc $7fa737
        dc $7fb564
        dc $7fc256
        dc $7fce0c
        dc $7fd888
        dc $7fe1c7
        dc $7fe9cc
        dc $7ff094
        dc $7ff622
        dc $7ffa73
        dc $7ffd88
        dc $7fff62
CosTable
        dc $7fffff
        dc $7fffff


        endsec


        