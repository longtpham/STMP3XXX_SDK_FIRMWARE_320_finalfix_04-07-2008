;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2001
;
; STMP3 FM Tuner Module
;///////////////////////////////////////////////////////////////////////////////

    opt     mex

    section FMTMOD_FMTuner

;///////////////////////////////////////////////////////////////////////////////
;   Includes and external referencse
;///////////////////////////////////////////////////////////////////////////////
    nolist
    
    include "sysmacro.asm"
    include "stack.xref"
    include "sysmem.xref"
    include "const.xref"
    include "sysequ.inc"
    include "msgequ.inc"
    include "hwequ.inc"
    include "resource.inc"
    include "project.inc"
    list
        page    132,60,0,0,0
;///////////////////////////////////////////////////////////////////////////////
;   Symbol Exports 
;///////////////////////////////////////////////////////////////////////////////
;Functions
    global  FmtProcessQueue
    global  FMTunerInit
    global  DecimalFrequency
    global  Fg_wDecimalFrequency
       
;Variable in P        

;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////


;///////////////////////////////////////////////////////////////////////////////
;  X memory
;///////////////////////////////////////////////////////////////////////////////


;///////////////////////////////////////////////////////////////////////////////
;  Y memory
;///////////////////////////////////////////////////////////////////////////////
     org   y,"FMTModule.y":

Fg_wDecimalFrequency
DecimalFrequency        dc      0
FmtSP                   ds      1

shift_reg2              dc      0
shift_reg               dc      0
frequency               dc      0

 if !@def(HW_FM_TUNER_DATA_ENABLE_R)
HW_FM_TUNER_DATA_ENABLE_R               equ HW_GP0ENR
 endif ;!@def('HW_FM_TUNER_DATA_ENABLE_R')
HW_FM_TUNER_DATA_DATA_OUT_R equ         HW_FM_TUNER_DATA_ENABLE_R+1
HW_FM_TUNER_DATA_DATA_IN_R equ          HW_FM_TUNER_DATA_ENABLE_R+2
HW_FM_TUNER_DATA_DATA_OUT_ENABLE_R equ  HW_FM_TUNER_DATA_ENABLE_R+3

 if !@def(HW_FM_TUNER_CLOCK_ENABLE_R)
HW_FM_TUNER_CLOCK_ENABLE_R               equ HW_GP0ENR
 endif ;!@def('HW_FM_TUNER_CLOCK_R')
HW_FM_TUNER_CLOCK_DATA_OUT_R equ         HW_FM_TUNER_CLOCK_ENABLE_R+1
HW_FM_TUNER_CLOCK_DATA_IN_R equ          HW_FM_TUNER_CLOCK_ENABLE_R+2
HW_FM_TUNER_CLOCK_DATA_OUT_ENABLE_R equ  HW_FM_TUNER_CLOCK_ENABLE_R+3

 if !@def(HW_FM_TUNER_WE_ENABLE_R)
HW_FM_TUNER_WE_ENABLE_R               equ HW_GP0ENR
 endif ;!@def('HW_FM_TUNER_WE_R')
HW_FM_TUNER_WE_DATA_OUT_R equ         HW_FM_TUNER_WE_ENABLE_R+1
HW_FM_TUNER_WE_DATA_IN_R equ          HW_FM_TUNER_WE_ENABLE_R+2
HW_FM_TUNER_WE_DATA_OUT_ENABLE_R equ  HW_FM_TUNER_WE_ENABLE_R+3


 if @def(HW_FM_TUNER_POWER_ENABLE_R)
HW_FM_TUNER_POWER_OUT_R         equ     HW_FM_TUNER_POWER_ENABLE_R+1
HW_FM_TUNER_POWER_IN_R          equ     HW_FM_TUNER_POWER_ENABLE_R+2
HW_FM_TUNER_POWER_OUT_ENABLE_R  equ     HW_FM_TUNER_POWER_ENABLE_R+3
 endif ;@def(HW_FM_TUNER_POWER_ENABLE_R)

 if !@def(FM_DATA_bitpos)
FM_DATA_bitpos          equ     12 
 endif 
 if !@def(FM_CLK_bitpos)
FM_CLK_bitpos           equ     13      
 endif 
 if !@def(FM_WE_bitpos)
FM_WE_bitpos            equ     14
 endif 



;///////////////////////////////////////////////////////////////////////////////
;   Code
;///////////////////////////////////////////////////////////////////////////////

        org p,"FMTMOD_Fmt_overlay_p":

        ; Resource header
    dc      RSRC_FMTMOD_CODE         ; Resource number

  if @def('FMTUNER')   ; skip the whole file if not enabled in makefile
;*******************************************************************************
;               Values for bits F.14 to F.0
;
;       bit #    (kHz)
FMValue  ;=============
;bit14            -
bit13   dc      102400
bit12   dc      51200
bit11   dc      25600
bit10   dc      12800
bit9    dc      6400
bit8    dc      3200
bit7    dc      1600
bit6    dc      800
bit5    dc      400
bit4    dc      200
bit3    dc      100
bit2    dc      50
bit1    dc      25
bit0    dc      12
;*******************************************************************************


;///////////////////////////////////////////////////////////////////////////////
;> Name: FMTunerInit
;
;  Type: Function
;  Description: Initialize the FM Tuner Module
;  Inputs: 
;  Outputs: 
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////
FMTunerInit
; This is the initial value of the shift register
;     2138 - frequency
;    3     - sensitivity
;   0      - stereo
    move    #$022000,y0
    move    y0,y:shift_reg
    move    #$002000,y0
    move    y0,y:frequency
    jsr     TranslateFrequency

    ; Initialize the GPIO pins 
    bset    #FM_DATA_bitpos,x:HW_FM_TUNER_DATA_ENABLE_R   ;DATA pin enable
    bset    #FM_CLK_bitpos,x:HW_FM_TUNER_CLOCK_ENABLE_R   ;CLOCK pin enable
    bset    #FM_WE_bitpos,x:HW_FM_TUNER_WE_ENABLE_R       ;WE pin enable

    bclr    #FM_DATA_bitpos,x:HW_FM_TUNER_DATA_DATA_OUT_R   ;DATA pin set data to 0
    bclr    #FM_CLK_bitpos,x:HW_FM_TUNER_CLOCK_DATA_OUT_R   ;CLOCK pin set to 0
    bclr    #FM_WE_bitpos,x:HW_FM_TUNER_CLOCK_DATA_OUT_R    ;WE seto to 0

    bset    #FM_DATA_bitpos,x:HW_FM_TUNER_DATA_DATA_OUT_ENABLE_R   ;DATA pin output enable
    bset    #FM_CLK_bitpos,x:HW_FM_TUNER_CLOCK_DATA_OUT_ENABLE_R   ;CLOCK pin output enable
    bset    #FM_WE_bitpos,x:HW_FM_TUNER_WE_DATA_OUT_ENABLE_R       ;WE pin output enable


 if @def(HW_FM_TUNER_POWER_ENABLE_R)
    ;if we have the power enable circuitry, lets enable the bits, but turn off the power.
    bset    #FM_POWER_ENABLE_bitpos,x:HW_FM_TUNER_POWER_ENABLE_R        ;set the power enable pin to GPIO mode
    jsr     FmtPowerOff
    bset    #FM_POWER_ENABLE_bitpos,x:HW_FM_TUNER_POWER_OUT_ENABLE_R    ;enable the output
 endif

    rts
       
;///////////////////////////////////////////////////////////////////////////////
;> Name: FmtProcessQueue
;
;  Type: Function
;  Description: Process the FM Tuner Module message queue.  This is the entry point
;               from the Executive.
;  Inputs:
;       r0=Descriptor for this modules message queue (in y memory)
;       m0=linear
;  Outputs: 
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////
FmtProcessQueue
    push    m1

    ; Save the system stack
    move    #_StackSaved,r6              ; setup the return address
    move    #FmtSP,r5                    ; pointer to the SP local save
    jmp     SaveSystemStack              ; save the hardware stack
_StackSaved

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
    move    #>TUNER_FIRST_MSG_ID,x0
    cmp     x0,b
    jlt     _InvalidMessageID
    move    #>TUNER_LAST_MSG_ID,x0
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
    

_FmtTuneFrequency
    move    y:(r1)+,a1              ; frequency
    jsr     FmtTuneFrequency
    jmp     _NextMessage

_FmtTuneManualUp
    jsr     FmtTuneManualUp
    jmp     _NextMessage

_FmtTuneManualDown
    jsr     FmtTuneManualDown
    jmp     _NextMessage

_FmtTuneSearchUp
    jsr     FmtTuneSearchUp
    jmp     _NextMessage

_FmtTuneSearchDown
    jsr     FmtTuneSearchDown
    jmp     _NextMessage

_FmtMono
    jsr     FmtMono
    jmp     _NextMessage

_FmtStereo
    jsr     FmtStereo
    jmp     _NextMessage

_FmtReset
    jsr     FmtReset
    jmp     _NextMessage

_FmtPowerOn
    jsr     FmtPowerOn
    jmp     _NextMessage

_FmtPowerOff
    jsr     FmtPowerOff
    jmp     _NextMessage
    
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

    ; Restore the system stack
    move    #FmtSP,r5                    ; pointer to the SP local save
    move    #_Leave,r6                   ; setup the return address
    jmp     RestoreSystemStack           ; restore the hardware stack
_Leave
    move    #EVENT_MESSAGE,a1
    pop     m1

    rts

_JumpTable
    dc      _FmtTuneFrequency
    dc      _FmtTuneManualUp
    dc      _FmtTuneManualDown
    dc      _FmtTuneSearchUp
    dc      _FmtTuneSearchDown
    dc      _FmtMono
    dc      _FmtStereo
    dc      _FmtReset
    dc      _FmtPowerOn
    dc      _FmtPowerOff

; Note: It is important that the entries in the jump table be in the
; same order as the FM Tuner message ID's which are defined in msgequ.inc


;///////////////////////////////////////////////////////////////////////////////
;> Name: FmtCheckFrequency
;
;  Type: Function
;  Description: This message will cause the FM Tuner to increase its VCO by 50 kHz 
;               
;  Inputs:     y:DecimalFrequency
;  Outputs:    carry bit set if in band, clear if out of band
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////
FmtCheckFrequency
    move #>87500,x0
    move y:DecimalFrequency,a
    cmp x0,a
    jlt  _OutOfBand
    move #>108000,x0
    cmp x0,a
    jgt _OutOfBand
    or  #>$01,ccr
    jmp _finished
_OutOfBand:
    and #>$FE,ccr
_finished:
    rts
;///////////////////////////////////////////////////////////////////////////////
;> Name: FmtTuneFrequency
;
;  Type: Function
;  Description: This message will cause the FM Tuner to increase its VCO by 50 kHz 
;               
;  Inputs:     a1 = frequency
;  Outputs:    y:DecimalFrequency
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////
FmtTuneFrequency

    bclr    #0,y:shift_reg2             ; turn search off
    move    a1,y:DecimalFrequency       ; set up decimal frequency to be translated
    jsr     TranslateFrequencyBack      ; y:frequency is now set up
    
    jsr     SetFrequency                        ; b1=frequency
    jsr     Write_Shift_Reg             ; write into the shift register
    jsr     Read_Shift_Reg
    rts

;///////////////////////////////////////////////////////////////////////////////
;> Name: FmtTuneManualUp
;
;  Type: Function
;  Description: This message will cause the FM Tuner to increase its VCO by 50 kHz 
;               
;  Inputs:  
;  Outputs:    y:DecimalFrequency
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////
FmtTuneManualUp

    bclr    #0,y:shift_reg2             ; turn search off
    
    move    y:frequency,b1
    move    #>$4,y0                     ; set the step value (4=50kHz, 2=25kHz, 1=12.5kHz)
    add     y0,b                                ; add step value to current freq.
    move    b1,y:frequency      
    
    jsr     SetFrequency                        ; b1=frequency
    jsr     Write_Shift_Reg             ; write into the shift register
    jsr     Read_Shift_Reg
    jsr     FmtCheckFrequency
    jcs     _exit
    move    #>87500,a1
    jsr     FmtTuneFrequency
_exit
    rts
;///////////////////////////////////////////////////////////////////////////////
;> Name: FmtTuneManualDown
;
;  Type: Function
;  Description: This message will cause the FM Tuner to decrease its VCO by 50 kHz.
;  Inputs:     
;  Outputs:    y:DecimalFrequency
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////
FmtTuneManualDown

    bclr    #0,y:shift_reg2             ; turn search off
    
    move    y:frequency,b1
    move    #>$4,y0                     ; set the step value (4=50kHz, 2=25kHz, 1=12.5kHz)
    sub     y0,b                                ; subtract the step value from current freq.
    move    b1,y:frequency      
            
    jsr     SetFrequency                        ; b1=frequency
    jsr     Write_Shift_Reg             ; write into the shift register
    jsr     Read_Shift_Reg
    jsr     FmtCheckFrequency
    jcs     _exit
    move    #>108000,a1
    jsr     FmtTuneFrequency
_exit
    rts
            
;///////////////////////////////////////////////////////////////////////////////
;> Name: FmtTuneSearchUp
;
;  Type: Function
;  Description: This message will cause the FM TUNER to scan the higher 
;               frequencies until it locks onto a strong signal
;  Inputs:
;  Outputs:     y:DecimalFrequency
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////
FmtTuneSearchUp
    bset    #0,y:shift_reg2                ; set the search pin
    move    #$800000,a1
    move    a1,y:shift_reg                 ; choose the UP direction

    jsr     Write_Shift_Reg                ; Write the shift register
    jsr     Read_Shift_Reg                 ; Read the shift register
    jsr     FmtCheckFrequency
    jcs     _exit
    move    #>87500,a1
    jsr     FmtTuneFrequency
_exit
    rts
;///////////////////////////////////////////////////////////////////////////////
;> Name: FmtTuneSearchDown
;
;  Type: Function
;  Description: This message will cause the FM TUNER to scan the lower 
;               frequencies until it locks onto a strong signal
;  Inputs:
;  Outputs:       y:DecimalFrequency
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////
FmtTuneSearchDown

    bset    #0,y:shift_reg2                 ; set the search pin
    move    #$000000,a1
    move    a1,y:shift_reg                  ; choose the DOWN direction

    jsr     Write_Shift_Reg                 ; Write the shift register
    jsr     Read_Shift_Reg                  ; Read the shift register
    move    y:DecimalFrequency,b
    jsr     FmtCheckFrequency
    jcs     _exit
    move    #>108000,a1
    jsr     FmtTuneFrequency
_exit
    rts
;///////////////////////////////////////////////////////////////////////////////
;> Name: FmtMono
;
;  Type: Function
;  Description: 
;  Inputs:
;  Outputs:    y:DecimalFrequency
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////
FmtMono
    bclr    #0,y:shift_reg2                 ; make sure it doesn't start a search
    bset    #22,y:shift_reg                 ; choose  MONO 
    jsr     Write_Shift_Reg
    jsr     Read_Shift_Reg
    jsr     Read_Shift_Reg

    rts
;///////////////////////////////////////////////////////////////////////////////
;> Name: FmtStereo
;
;  Type: Function
;  Description: 
;  Inputs:
;  Outputs:   y:DecimalFrequency
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////
FmtStereo
    bclr    #0,y:shift_reg2                ; make sure it doesn't start a search
    bclr    #22,y:shift_reg                ; choose STEREO
    jsr     Write_Shift_Reg

    rts

;///////////////////////////////////////////////////////////////////////////////
;> Name: FmtReset
;
;  Type: Function
;  Description: 
;  Inputs:
;  Outputs:   y:DecimalFrequency
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////
FmtReset
    move    #>$1e00,a0
    move    a0,y:shift_reg
    jsr     Write_Shift_Reg

    rts

;///////////////////////////////////////////////////////////////////////////////
;> Name: FmtPowerOn
;
;  Type: Function
;  Description: 
;  Inputs:
;  Outputs:   y:DecimalFrequency
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////
FmtPowerOn
 if @def(HW_FM_TUNER_POWER_ENABLE_R)
    bclr    #FM_POWER_ENABLE_bitpos,x:HW_FM_TUNER_POWER_OUT_R          ;set it to high (power ON)
 endif 
    rts

;///////////////////////////////////////////////////////////////////////////////
;> Name: FmtPowerOff
;
;  Type: Function
;  Description: 
;  Inputs:
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////
FmtPowerOff
 if @def(HW_FM_TUNER_POWER_ENABLE_R)
    bset   #FM_POWER_ENABLE_bitpos,x:HW_FM_TUNER_POWER_OUT_R          ;set it to low (power off)
 endif 
    rts

;*************************************************************************
;                       Read the shift register into a1
;*************************************************************************

Read_Shift_Reg
    bclr    #FM_DATA_bitpos,x:HW_FM_TUNER_DATA_DATA_OUT_ENABLE_R        ;set DATA to input 

check_data_again
    btst    #FM_DATA_bitpos,x:HW_FM_TUNER_DATA_DATA_IN_R                ;wait until it goes low...
    jcc     _data_is_low
    jmp     check_data_again
_data_is_low

    rep #2000                           ; data available after search ready MSB is LOW 
    nop                                 ; tda = 14 usec
    nop 
_end_delay      

    clr a                               ; clear accumulator
    
    ; read the register
    do      #24,_end_do
        rep     #1000
        nop
        nop
        bset    #FM_CLK_bitpos,x:HW_FM_TUNER_CLOCK_DATA_OUT_R      ; set clock high
        rep     #100
        nop
        nop
        bclr    #FM_CLK_bitpos,x:HW_FM_TUNER_CLOCK_DATA_OUT_R      ; set clock low
        btst    #FM_DATA_bitpos,x:HW_FM_TUNER_DATA_DATA_IN_R       ; read the bit coming in and put it in C
        rol     a1                              ; rotate data one bit to the left moving
                                                ;  the carry bit to the 24 bit of a1
_end_do

    move    a1,y:shift_reg
    bset    #FM_DATA_bitpos,x:HW_FM_TUNER_DATA_DATA_OUT_ENABLE_R  ; Set DATA to output
    jsr     ReadFrequency
    jsr     TranslateFrequency
    rts
;*************************************************************************
;                       Write a1 in to the shift register 
;*************************************************************************
Write_Shift_Reg
    bset    #FM_DATA_bitpos,x:HW_FM_TUNER_DATA_DATA_OUT_ENABLE_R        ; set data high
    bset    #FM_WE_bitpos,x:HW_FM_TUNER_WE_DATA_OUT_R                   ; set write enable high
    bclr    #FM_CLK_bitpos,x:HW_FM_TUNER_CLOCK_DATA_OUT_R               ; set clock low
    move    y:shift_reg,a1
    move    y:shift_reg2,a2

    ;this section required to write the 25th bit located in a2
    btst    #0,a2
    jcs     _set2
    bclr    #FM_DATA_bitpos,x:HW_FM_TUNER_DATA_DATA_OUT_R
    rep     #50
    nop
    bset    #FM_CLK_bitpos,x:HW_FM_TUNER_CLOCK_DATA_OUT_R  ; set clock high
    rep     #50
    nop
    bclr    #FM_CLK_bitpos,x:HW_FM_TUNER_CLOCK_DATA_OUT_R  ; reset clock
    jmp     _jump2    
_set2
    bset    #FM_DATA_bitpos,x:HW_FM_TUNER_DATA_DATA_OUT_R
    rep     #50
    nop
    bset    #FM_CLK_bitpos,x:HW_FM_TUNER_CLOCK_DATA_OUT_R  ; set clock high
    rep     #50
    nop
    bclr    #FM_CLK_bitpos,x:HW_FM_TUNER_CLOCK_DATA_OUT_R  ; reset clock
_jump2

    do      #24,_end_do
        btst    #23,a1
        jcs     _set
        bclr    #FM_DATA_bitpos,x:HW_FM_TUNER_DATA_DATA_OUT_R
        rep     #50
        nop
        bset    #FM_CLK_bitpos,x:HW_FM_TUNER_CLOCK_DATA_OUT_R      ; set clock high
        rep     #50
        nop
        bclr    #FM_CLK_bitpos,x:HW_FM_TUNER_CLOCK_DATA_OUT_R      ; reset clock
     
        jmp     _jump    
_set
        bset    #FM_DATA_bitpos,x:HW_FM_TUNER_DATA_DATA_OUT_R
        rep     #50
        nop
        bset    #FM_CLK_bitpos,x:HW_FM_TUNER_CLOCK_DATA_OUT_R               ; set clock high
        rep     #50                            
        nop
        bclr    #FM_CLK_bitpos,x:HW_FM_TUNER_CLOCK_DATA_OUT_R               ; reset clock
_jump   
        rol     a1
_end_do 
    rep     #1000                                                           ; allow interferences to fade out
    nop
    nop
    bclr    #FM_WE_bitpos,x:HW_FM_TUNER_WE_DATA_OUT_R                       ; set write enable low

    rep     #1000                                       ; allow interferences to fade out
    nop
    nop
    bset    #FM_DATA_bitpos,x:HW_FM_TUNER_DATA_DATA_OUT_R
_wait
    jsr     ReadFrequency
    jsr     TranslateFrequency
    rts
;*************************************************************************
;                       SetFrequency
;
;  This subroutine sets the tuning frequency inside the shift register
;       y:frequency  -->   y:shift_reg   
;*************************************************************************
SetFrequency
    clr     a
    move    y:shift_reg,y0
    move    #$FF8000,a
    and     y0,a
    move    y:frequency,y0
    or      y0,a                                ; apply mask
    move    a1,y:shift_reg      
    rts

;*************************************************************************
;                      ReadFrequency
;
;  This macro reads the tuning frequency inside the shift register
;  and inserts it into y:frequency
;        y:shift_reg  -->  y:frequency
;*************************************************************************
ReadFrequency   

    move    y:shift_reg,b1
    move    #$007FFF,y0
    and     y0,b
    move    b1,y:frequency
    rts
 
;///////////////////////////////////////////////////////////////////////////////
;> Name: TranslateFrequency
;
;  Type: Function
;  Description: translate frequency to decimal form 
;  Inputs:    y:frequency
;  Outputs:   y:DecimalFrequency
;  Notes:     y:frequency  --> H[] --> b1 & y:DecimalFrequency
;<
;///////////////////////////////////////////////////////////////////////////////
TranslateFrequency
    clr     b
    clr     a
    move    y:frequency,a1
    move    #>25,y0             ; set up 25 khz
    do      #14,_end_do         ; check all 15 frequency bits
        jclr    #1,a,_no_add    ; check the 25 bit if it's high or low
        add     y0,b            ; if high add the increment
_no_add
        asr     a               ; shift right to check the next bit
        move    b1,x0           ; save off the total frequency
        
        move    y0,b0           ; set up increment to be multiplied
        asl     b                       ; multiply the increment by 2
        move    b0,y0           ; put increment back into y0
        
        move    x0,b1           ; put back the total frequency

_end_do

    move    #10700,x0
    sub     x0,b
    move    b1,y:DecimalFrequency
   
    rts
;///////////////////////////////////////////////////////////////////////////////
;> Name: TranslateFrequencyBack
;
;  Type: Function
;  Description: translate decimal frequency to the FM Tuner Form 
;  Inputs:    y:DecimalFrequency
;  Outputs:   y:frequency
;  Notes:     y:DecimalFrequency  --> H[] -->  y:frequency
;<
;/////////////////////////////////////////////////////////////////////////////// 
TranslateFrequencyBack
    ; add the offset due to analog components, may change in the future
    clr     b
    move    b,y:frequency
    move    y:DecimalFrequency,b1
    move    #10700,x0
    add     x0,b

   
    move    #FMValue,r0
    move    #-1,m0
    
    do      #13,_do_loop
        move    p:(r0)+,a1                     ; a = FM Value
        cmp     a,b
        jcs     _no                            ; Decimal Freq > FM Value?

        sub     a,b                            ; DecimalFrequency - FM Value
    
        clr     a                              ;
        move    y:frequency,a
        bset    #0,a1                          ; set bit 0
        lsl     a                              ; shift the register
        move    a1,y:frequency
        clr     a
        
        jmp     _done
_no
        clr     a                              ;
        move    y:frequency,a
        
        lsl     a                              ; shift the register
        move    a1,y:frequency
        clr     a

        nop
_done     

    nop
_do_loop
    nop
    rts

   else  ;; keeping module table the same, but just ignore FM tuner calls
FMTunerInit
FmtProcessQueue
   rts
   endif

   endsec

