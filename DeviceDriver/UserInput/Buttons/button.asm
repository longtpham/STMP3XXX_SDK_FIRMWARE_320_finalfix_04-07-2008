;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2003
;
; File          : Button.asm
; Description   : 
;                 
;///////////////////////////////////////////////////////////////////////////////

    section BUTMOD_Button



;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////

;    nolist
    nolist
    include "sysmacro.asm"
    include "systrace.asm"
    include "sysequ.inc"
    include "hwequ.inc"
    include "sysresources.inc"
    include "msgequ.inc"
    include "buttons.inc"
    include "buttonsdef.inc"
    include "resource.inc"

    list

;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

; Functions  
    GLOBAL  ButtonGetTimeLast
    GLOBAL  ButtonInit
    GLOBAL  ButtonIsr
    GLOBAL  ButtonsOff
    GLOBAL  ButtonsOn
    GLOBAL  ButtonTrackTimeLastOff
    GLOBAL  ButtonTrackTimeLastOn
    GLOBAL  SysGetButton
    GLOBAL  SysCheckForButton
    GLOBAL  SysCheckForHoldButtonActivated    
    GLOBAL  SysButtonClearBuffer

; Variables in Y
    GLOBAL  Btl_Flags,FBtl_Flags
    GLOBAL  Btl_MessageNumber
    GLOBAL  Btl_MessageCount
    GLOBAL  Btl_ResourceByteCount
    GLOBAL  Btl_ResourceSize
    GLOBAL  ButtonBufferDescriptor,FButtonBufferDescriptor 
    GLOBAL  ButtonBufferModulo     
    GLOBAL  ButtonBufferTailPtr
    global  FLowResAdcBattResult
    global  FLowResAdcAuxResult    
    global  FLowResAdcAux2Result    
    global  FLowResAdcBattAvg
    global  FLowResAdcAuxAvg
    global  FLowResAdcAux2Avg
    global  FLowResAdcStatus
    global  ColRowBitsDebounced
    global  State
    
    global ButtonBufferHeadPtr
    global ButtonBufferTailPtr
; Variables in X    
    GLOBAL  ButtonIsrFlag

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "sysmem.xref"
    include "sysmisc.xref"
    include "stack.xref"
    include "stackmem.xref"
    include "sysirq.xref"
    include "systime.xref"
    include "sysresource.xref"
    include "bufdesc.xref"
    include "systimers.xref"
    include "const.xref"

    extern  Fsrand
    extern FSysLowResolutionADCInit
    extern FSysLowResolutionAdcReadVolt

    list
    
 if (@DEF('FUNCLET'))           

    extern  SysCallFunclet
    
 endif


;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////

STATE_NO_BUTTONS_PRESSED    equ     0
STATE_DEBOUNCING            equ     1
STATE_TEST_HOLD_RELEASE     equ     2
HoldButtonReportedBit       equ     2
HoldBit                     equ     23 ; bit position of hold modifier

DebounceTime                equ     10 ; measured in msec
HoldTime                    equ     300 ; measured in msec

; test values below used to simplify single stepping
;DebounceTime            equ     3
;HoldTime                equ     6

BUTTON_BUFFER_SIZE      equ     10
BUTTON_BUFFER_MODULO    equ     BUTTON_BUFFER_SIZE-1

BUTTON_ON               equ     0
BUTTON_TRACK_TIME_BIT   equ     1

;///////////////////////////////////////////////////////////////////////////////
;   X Memory
;///////////////////////////////////////////////////////////////////////////////
    org     x,"BUTMOD_Button_X":

ButtonIsrSp             dc      0
ButtonIsrFlag           dc      0
ButtonTrackTimeLow      dc      0
ButtonTrackTimeHigh     dc      0

;///////////////////////////////////////////////////////////////////////////////
;   Y Memory
;///////////////////////////////////////////////////////////////////////////////
    org     y,"BUTMOD_Button_mod_Y":
ButtonBuffer            dsm     BUTTON_BUFFER_SIZE

    org     y,"BUTMOD_Button_Y":


FButtonBufferDescriptor
ButtonBufferDescriptor  dc      ButtonBuffer
ButtonBufferModulo      dc      BUTTON_BUFFER_MODULO
                        dc      BUTTON_BUFFER_SIZE
ButtonBufferHeadPtr     dc      ButtonBuffer
ButtonBufferTailPtr     dc      ButtonBuffer

; data for ButtonTranslator
    if ("DBG"=="ON")
; debug only
WatchBuffer     ds      200
WatchBufferPtr  dc      WatchBuffer
    endif

; data for ButtonDriver Init
State                   dc      STATE_NO_BUTTONS_PRESSED
ColRowBitsPrevious      dc      $ffffff
ColRowBitsCurrent       dc      0
ColRowBitsDebounced     dc      0
PreviousTimeLow         dc      0
PreviousTimeHigh        dc      0

; data for ScanButtons
Sb_ButtonBits           dc  0

;Btl_PhysMap             ds      BTL_BUTTON_PHYS_MAP_SIZE
;Btl_EventMap            ds      BTL_BUTTON_EVENT_MAP_SIZE

Btl_ResourceByteCount   ds      1
Btl_ResourceSize        ds      1
Btl_MessageNumber       ds      1
Btl_MessageCount        ds      1

FBtl_Flags
Btl_Flags               dc      $000000 ; Bit 0 = Execute Translator
                                        ;  0 -> ButtonsOff
                                        ;  1 -> ButtonsOn
                                        ; Bit 1 = Keep track of time between buttons
                                        ;  0 -> Turn OFF
                                        ;  1 -> Turn ON
                                        ; Bit 2 = Keep track of Hold Button Status
                                        ;   0 -> Hold button "press hold" event not yet reported
                                        ;   1 -> Hold button "press hold" event already reported



Btl_Rows
    dc BUTTON_ROWS
Btl_RowEntries
;Bd_Rows_Ptr
    if (BUTTON_ROWS>0)
    dupf num,0,(BUTTON_ROWS-1)
        dc BUTTON_GPIO_ENABLE_ROW_\num
        dc BUTTON_GPIO_SENSE_ROW_\num
        dc BUTTON_BITPOS_ROW_\num
    endm
    endif ;    if (BUTTON_ROWS>0)

Btl_Columns
    dc BUTTON_COLUMNS
Btl_ColumnEntries
;Bd_Columns_Ptr
    if (BUTTON_COLUMNS>0)
    dupf ColumnNumber,0,(BUTTON_COLUMNS-1)
        dc BUTTON_GPIO_ENABLE_COLUMN_\ColumnNumber
        dc BUTTON_GPIO_DATA_OUT_COLUMN_\ColumnNumber
        dc BUTTON_GPIO_OUTPUT_ENABLE_COLUMN_\ColumnNumber
        dc BUTTON_BITPOS_COLUMN_\ColumnNumber
    endm
    endif ;    if (BUTTON_COLUMNS>0)
    
Btl_NonScanButtons
    dc BUTTON_NON_SCAN_BUTTONS
Btl_NonScanEntries
;Bd_NsButton_Ptr
    if (BUTTON_NON_SCAN_BUTTONS>0)
    dupf num,0,(BUTTON_NON_SCAN_BUTTONS-1)
        dc BUTTON_GPIO_ENABLE_NON_SCAN_\num
        dc BUTTON_GPIO_SENSE_NON_SCAN_\num
        dc BUTTON_BITPOS_NON_SCAN_\num
    endm
    endif   ; if (BUTTON_NON_SCAN_BUTTONS>0)

Btl_ScanButtonEvents
;Bd_ButtonNumbers_Ptr
    dupf column,0,(BUTTON_COLUMNS-1)
        dupf row,0,(BUTTON_ROWS-1)
            dc BUTTON_EVENT_COL\column\ROW\\row 
        endm
    endm

Btl_NonScanEvents
;Bd_NsButtonNumbers_Ptr
    dupf nonscan,0,(BUTTON_NON_SCAN_BUTTONS-1)
        dc BUTTON_EVENT_NONSCAN\nonscan
    endm    


        if (@def('BUTTON_LRADC_BUTTONS'))               ; LRADC "buttons" are optional

            Btl_LRADCLevels:
                dc  BUTTON_LRADC_BUTTONS
            Btl_LRADCEntries:
                if (BUTTON_LRADC_BUTTONS>0)
                    dupf num,0,(BUTTON_LRADC_BUTTONS-1)
                        dc  BUTTON_LRADC_LEVEL_\num
                    endm
                endif   ; if (BUTTON_LRADC_BUTTONS>0)

            Btl_LRADCEvents:
                dupf num,0,(BUTTON_LRADC_BUTTONS-1)
                    dc  BUTTON_EVENT_LRADC_BUTTON\num
                endm

        endif   ; if (@def('BUTTON_LRADC_BUTTONS'))


Btl_NumEvents
    dc BUTTON_MAPPED_EVENTS
Btl_EventMap
    dupf num,0,(BUTTON_MAPPED_EVENTS-1)
        dc BUTTON_EVENT_\num
    endm  



; The variables here that are global should be allocated in 
; System\3xxx\LowResolutionAdc.c and externed here
; 
; Low resolution ADC variables
FLowResAdcStatus        dc  0       ; Bit 0 Execute Low Resolution ADC BATT reading
                                    ; 0 -> Low Resolution ADC BATT OFF
                                    ; 1 -> Low Resolution ADC BATT ON
                                    ; Bit 1 Execute Low Resolution ADC AUX reading
                                    ; 0 -> Low Resolution ADC AUX OFF
                                    ; 1 -> Low Resolution ADC AUX ON
				    ; Bit 2 Execute Low Resolution ADC AUX2 reading (3500)
                                    ; 0 -> Low Resolution ADC AUX2 OFF
                                    ; 1 -> Low Resolution ADC AUX2 ON

; The result and average variables should be declared in the LowResolutionAdc.c files
; And externed here.
LowResAdcBattVar
FLowResAdcBattResult    dc  0                                    
FLowResAdcBattAvg       dc  0
LowResAdcBattAccu       dc  0
LowResAdcBattAvgCnt     dc  LOW_RES_ADC_AVG_CNT

LowResAdcAuxVar
FLowResAdcAuxResult     dc  0                                    
FLowResAdcAuxAvg        dc  0
LowResAdcAuxAccu        dc  0
LowResAdcAuxAvgCnt      dc  LOW_RES_ADC_AVG_CNT

; This set of variables is only applicable to the 3500
LowResAdcAux2Var
FLowResAdcAux2Result    dc  0                                    
FLowResAdcAux2Avg       dc  0
LowResAdcAux2Accu       dc  0
LowResAdcAux2AvgCnt     dc  LOW_RES_ADC_AVG_CNT


IndexLowResAdcResult    equ FLowResAdcBattResult-LowResAdcBattVar
IndexLowResAdcAvg       equ FLowResAdcBattAvg-LowResAdcBattVar  
IndexLowResAdcAccu      equ LowResAdcBattAccu-LowResAdcBattVar
IndexLowResAdcAvgCnt    equ LowResAdcBattAvgCnt-LowResAdcBattVar

LOW_RES_ADC_AVG_SHIFTER equ 8
LOW_RES_ADC_AVG_CNT     equ 1<<LOW_RES_ADC_AVG_SHIFTER

;   IMPORTANT: The following defines must match the enums (LowResADCChannel,
;   LowResADCHalfPwr, and LowResADCRef) in lowresolutionadc.h.
LRADC_AUX       equ 1
LRADC_OFF       equ 0

LRADC_REF_VDDIO equ HW_LRADC_CTRL_AUXADC_SETMASK

LOW_RES_ADC_BATT        equ 0	;Bit position in FlowResAdcStatus
LOW_RES_ADC_AUX         equ 1
LOW_RES_ADC_AUX2	equ 2

                                    
;///////////////////////////////////////////////////////////////////////////////
;   P Memory
;///////////////////////////////////////////////////////////////////////////////
    org     p,"BUTMOD_Button_P":

;///////////////////////////////////////////////////////////////////////////////
;> Name:            ButtonInit
; Type:             Function
; Description:      Initializes button tables. The initialization parameters
;                   are read from the resources RSRC_BUTTON_PHYS_MAP and
;                   BTL_BUTTON_PHYS_MAP_SIZE.
; Inputs:           None.
; Outputs:          None.
;
; Notes:
;<         
;///////////////////////////////////////////////////////////////////////////////
ButtonInit

 if (@DEF('FUNCLET'))           

    ;If using funclet, function will be executed via funclet kernel
    CallFunclet    RSRC_FUNCLET_BUTTONINIT

    org p,"SYSFUNCLET_ButtonInit_P":
    
    dc      RSRC_FUNCLET_BUTTONINIT           ; Funclet Resource number

 endif

    ; Init the state machine
    jsr     ResetButtonState

    ; Enable all column outputs & drive them low
    jsr     SysAllLinear                            

    move    y:Btl_Columns,a
    move    #Btl_ColumnEntries,r0

    ;Check if any scan button declared 
    tst     a
    jeq     _end_row_enable
    
    do      a1,_end_column_enable
        move    y:(r0)+,r1             ; get the enable register address
        move    y:(r0)+,r2             ; get the data out register address
        move    y:(r0)+,r3             ; get the output enable register address
        move    y:(r0)+,x0             ; get the bit pos
        move    x:(r2),a               ; get the data out register
        or      x0,a                   ; set the bit
        move    a1,x:(r2)
        move    x:(r1),a               ; get the enable register
        or      x0,a                   ; set the bit
        move    a1,x:(r1)
        move    x0,a                   ; setup for bit clear
        not     a
        move    x:(r3),x0              ; get the output enable register
        and     x0,a                   ; clear the bit
        move    a1,x:(r3)
_end_column_enable

    clr     a

    ; Enable all row inputs
    move    y:Btl_Rows,a1
    move    #Btl_RowEntries,r0
    
    ;Check if any scan button declared 
    tst     a
    jeq     _end_row_enable

    do      a1,_end_row_enable
        move    y:(r0)+,r1             ; get the enable register address
        move    y:(r0)+,r2             ; get the data out register address
        move    y:(r0)+,a              ; get the bit pos
        move    x:(r1),x0              ; get the enable register
        or      x0,a                   ; set the bit
        move    a1,x:(r1)
_end_row_enable

    ; Enable all non-scan inputs
    clr     a
    move    y:Btl_NonScanButtons,a1
    move    #Btl_NonScanEntries,r0

    ;Check if any non scan buttons declared 
    tst     a
    jeq     _end_nsb_enable

    do      a1,_end_nsb_enable
        move    y:(r0)+,r1             ; get the enable register address
        move    y:(r0)+,r2             ; get the data out register address
        move    y:(r0)+,a              ; get the bit pos
        move    x:(r1),x0              ; get the enable register
        or      x0,a                   ; set the bit
        move    a1,x:(r1)
_end_nsb_enable


        if (@def('BUTTON_LRADC_BUTTONS'))
            if (BUTTON_LRADC_BUTTONS>0)

                move #>LRADC_AUX,a                  ; Enable sampling on the AUX channel of the LRADC
                move #>LRADC_OFF,b
                move #>LRADC_REF_VDDIO,x0
                jsr FSysLowResolutionADCInit
            endif
        endif


    ; clear the flags
    clr     a
    move    a,y:Btl_Flags
    
    ; turn on the button timer
    move    #_come_back_here,r0
    move    #>BUTTON_TIMER_NUM,x0
    jmp     SysEnableTimer
_come_back_here

    rts

 if (@DEF('FUNCLET'))

    ;If using funclet, switch counter back to original here
    org    p,"BUTMOD_Button_P":

 endif

;///////////////////////////////////////////////////////////////////////////////
;> Name:            ResetButtonState
; Type:             Function
; Description:      Resets all button state variables
; Inputs:           None.
; Outputs:          None.
;
; Notes:         
;<         
;///////////////////////////////////////////////////////////////////////////////
ResetButtonState
    clr     a
    move    a,y:State
    move    a,y:ColRowBitsCurrent
    move    a,y:ColRowBitsDebounced
    move    a,y:PreviousTimeLow
    move    a,y:PreviousTimeHigh
    move    y:<Const_ffffff,a
    move    a,y:ColRowBitsPrevious
    move    #>STATE_NO_BUTTONS_PRESSED,a
    move    a,y:State
    rts

;///////////////////////////////////////////////////////////////////////////////
;> Name:            ButtonsOn
; Type:             Function
; Description:      Enables button scanning
; Inputs:           None.
; Outputs:          None.
;
; Notes:         
;<         
;///////////////////////////////////////////////////////////////////////////////
ButtonsOn
    bset    #BUTTON_ON,y:Btl_Flags
    rts
    
;///////////////////////////////////////////////////////////////////////////////
;> Name:            ButtonsOff
; Type:             Function
; Description:      Disables button scanning
; Inputs:           None.
; Outputs:          None.
;
; Notes:         
;<         
;///////////////////////////////////////////////////////////////////////////////
ButtonsOff
    bclr    #BUTTON_ON,y:Btl_Flags
    rts

;///////////////////////////////////////////////////////////////////////////////
;> Name:            ButtonTrackTimeLastOn
; Type:             Function
; Description:      Enables tracking time between 2 button strokes
; Inputs:           None.
; Outputs:          None.
;
; Notes:         
;<         
;///////////////////////////////////////////////////////////////////////////////
ButtonTrackTimeLastOn
    bset    #BUTTON_TRACK_TIME_BIT,y:Btl_Flags

    ;Update Track Time Counters
    jsr     SysGetCurrentTime
    move    a1,x:ButtonTrackTimeHigh
    move    a0,x:ButtonTrackTimeLow
    rts

;///////////////////////////////////////////////////////////////////////////////
;> Name:            ButtonTrackTimeLastOff
; Type:             Function
; Description:      Disables tracking time between 2 button strokes
; Inputs:           None.
; Outputs:          None.
;
; Notes:         
;<         
;///////////////////////////////////////////////////////////////////////////////
ButtonTrackTimeLastOff
    bclr    #BUTTON_TRACK_TIME_BIT,y:Btl_Flags
    rts

;///////////////////////////////////////////////////////////////////////////////
;> Name:            ButtonIsr
; Type:             Function
; Description:      Detects if a button is pressed, type (Press Release vs. press
;                   hold), and queues the event in the button buffer.
;                   Reads the 2 Low Resolution ADC Channels if device is not 3400 or 3300.
; Inputs:           None.
; Outputs:          None.
;
; Notes:         
;<         
;///////////////////////////////////////////////////////////////////////////////
ButtonIsr

    ; Clear the DM bit (double-precision multiply mode, bit 6) of the Mode Register
    ; This will be returned to its previous state when the RTI occurs (since the SR
    ; will be popped back off System Stack Low (SSL).
    ; Accumulator behavior is unpredictable when the DM bit is set.
    ; Next cycle after clearing it should NOT be an ALU instruction.
    andi #$BF,mr
    
    move    x0,y:IntL2_SaveR6           ; save x0 

    ; Check if spirous interrupt. If so exit ...
    move    x:HW_ICLSTATUS0R,x0
    jset    #HW_ICLSTATUS0R_SST14_BITPOS,x0,_ContinueIsr

    ; None of the above, exit interruption
    move    y:IntL2_SaveR6,x0           ; Restore x0
    rti

_ContinueIsr
    move    y:IntL2_SaveR6,x0           ; Restore x0
    move    r7,y:IntL2_SaveR7          ; save minimal reg set to local
    move    m7,y:IntL2_SaveM7
    move    r6,y:IntL2_SaveR6
    move    #>L2DataStack,r7           ; setup the L2 data stack
    move    y:<Const_ffffff,m7
    move    #ButtonSaveRegReturn,r6    ; setup the return address
    jmp     SaveRegs                   ; save the remaining registers
ButtonSaveRegReturn
    move    #ButtonSaveSysStackRtn,r6  ; setup the return address
    move    #L2Sp,r5                   ; pointer to the SP local save
    jmp     SaveSystemStack            ; save the hardware stack
ButtonSaveSysStackRtn

        ; BATT Low Resolution ADC on?
        move    y:FLowResAdcStatus,x0
        jclr    #LOW_RES_ADC_BATT,x0,_CheckAuxOn
        move    #>LOW_RES_ADC_BATT,x0    
        move    #>LOW_RES_ADC_BATT_READ_MASK,y0
        move    #>LowResAdcBattVar,r1
        jsr     LowResAdcRead
    
_CheckAuxOn
        ; AUX Low Resolution ADC on?
        move    y:FLowResAdcStatus,x0
        jclr    #LOW_RES_ADC_AUX,x0,_CheckButtonsOn
        move    #>LOW_RES_ADC_AUX,x0    
        move    #>LOW_RES_ADC_AUX_READ_MASK,y0
        move    #>LowResAdcAuxVar,r1
        jsr     LowResAdcRead

;    if (@def('DEVICE_3500'))
;_CheckAux2On
;       ; AUX Low Resolution ADC on?
;        move    y:FLowResAdc2Status,x0
;        jclr    #LOW_RES_ADC_AUX2,x0,_CheckButtonsOn
;        move    #>LOW_RES_ADC_AUX2,x0    
;        move    #>LOW_RES_ADC_AUX2_READ_MASK,y0
;        move    #>LowResAdcAux2Var,r1
;        jsr     LowResAdcRead
;
;	else        
;   endif
    
_CheckButtonsOn
    ; buttons on?
    move    y:Btl_Flags,x0
    jclr    #BUTTON_ON,x0,_bisr_exit

    ; Get a button
    jsr     GetButton

    ; n1 = BUTTON_xx or BUTTON_NOT_FOUND
    jclr    #15,n1,_bisr_exit          ; if no button then exit

    ;Update Track Time Counters
    jsr     SysGetCurrentTime
    move    a1,x:ButtonTrackTimeHigh
    move    a0,x:ButtonTrackTimeLow

    bclr    #15,n1                     ; clear the found bit

    ; see if there's enough room in the ButtonBuffer
    move    #ButtonBufferDescriptor,r4
    move    y:<Const_ffffff,m4
    jsr     GetEmptySpace
    tst     a
;    ccerror eq ; Assertion testpoint: HALTS IF BUTTON Q OVERFLOW OCCURRED.
;    alternate halt ver:
    jeq     _bisr_exit                 ; if no room then we lose it
    
    ; there's room so put it in!
    move    y:ButtonBufferHeadPtr,r0
    move    y:ButtonBufferModulo,m0
    nop
    move    n1,y:(r0)+                 ; store the button
    move    r0,y:ButtonBufferHeadPtr   ; update the head ptr

    ; Seed the Random Number Generator (RNG)
    ; Record the Dclock count low register at button press time for RNG
    ; Button press times are random and this 24 counter wraps over twice/sec @48MHz     
    push    A1
    move    x:HW_CLK_CNT_L,A1 
    jsr     Fsrand              ; re-seed now with 24 bit random value.
    pop     A1          
    ;end RNG re-seeding
    
_bisr_exit

    ; reset the isr for next time
    move    #>BUTTON_TIMER_NUM,x0
    move    #_TimerReset,r0
    jmp     SysClearTimerIrq
_TimerReset

    move    #L2Sp,r5                   ; pointer to the SP local save
    move    #ButtonRestoreSysStackRtn,r6 ; setup the return address
    jmp     RestoreSystemStack         ; restore the hardware stack
ButtonRestoreSysStackRtn
    move    #ButtonRestoreRegReturn,r6 ; setup the return address
    jmp     RestoreRegs                ; restore registers
ButtonRestoreRegReturn

    ; Assertion testpoint for IntL2_SaveR7 integrity

    move    y:IntL2_SaveR6,r6          ; restore minimal reg set
    move    y:IntL2_SaveM7,m7
    move    y:IntL2_SaveR7,r7

    rti
    
;///////////////////////////////////////////////////////////////////////////////
;> Name:            GetButton
; Type:             Function
; Description:      Debounces the button detected by ScanButton. Determines if
;                   the action is type press release or press and hold. Selects
;                   the event corresponding to the button action.
;                   
; Inputs:           None.
; Outputs:          
;                   n1 = Button Event.
;                   n1 = 0 , No button event to report.
;
; Notes:            To avoid useless information, the HOLD button will be reported
;                   only after a changemet of its state.
;                   Press and Hold for the HOLD button is not reported.
;<         
;///////////////////////////////////////////////////////////////////////////////
GetButton

    ; Scan the buttons
    jsr     ScanButtons

    ; Run the button state machine
    ; jump to the appropriate state based upon state variable
    move    #StateJumpTable,r2
    move    y:<Const_ffffff,m2
    move    y:State,n2
    nop
    movem   p:(r2+n2),r3
    nop
    jmp     (r3)

NoButtonsPressed
    ; assumes x0=ColRowBitsCurrent
    move    x0,a
    tst     a
    jeq     ReturnNoButtonEvent        ; if no activity on buttons, then jump to exit
    push    x0                         ; save a copy of ColRowBitsCurrent
    jsr     SysGetCurrentTime          ; call returns time in ms in a
    move    a1,y:PreviousTimeHigh      ; save the current time to compare against later
    move    a0,y:PreviousTimeLow
    pop     x0                         ; get back the ColRowBitsCurrent value
    move    x0,y:ColRowBitsPrevious    ; save current scanned val for compare against later
    move    #>STATE_DEBOUNCING,x0
    move    x0,y:State
    jmp     ReturnNoButtonEvent

Debouncing
    ; assumes x0=ColRowBitsCurrent
    move    y:ColRowBitsPrevious,a     ; If (ColRowBitsCurrent==ColRowBitsPrevious)
    cmp     x0,a    y:PreviousTimeHigh,b
    jne     ButtonChanged

    ; the button hasn't changed
    ; let's see if it hasn't changed for 10ms
    move    y:PreviousTimeLow,b0
    move    #>DebounceTime,x1
    push    x0
    jsr     SysIsElapsedTime
    pop     x0

    ; test if debounce time has elapsed, if not exit with no button event returned
    jlt     ReturnNoButtonEvent

    ; the RowCol held on for DebounceTime, so button is debounced!
    move    x0,y:ColRowBitsDebounced    ; ColRowBitsDebounced=ColRowBitsCurrent
    move    #>STATE_TEST_HOLD_RELEASE,x0
    move    x0,y:State                  ; State=TestHoldRelease
    jsr     SysGetCurrentTime           ; function returns time in ms in a
    move    a1,y:PreviousTimeHigh       ; save the current time to compare against later
    move    a0,y:PreviousTimeLow
    jmp     ReturnNoButtonEvent

ButtonChanged
    ; the button changed in less than 10ms, so start over
    jsr     ResetButtonState            ; reset all internal states
    jmp     ReturnNoButtonEvent

TestHoldRelease
    ; assumes x0=ColRowBitsCurrent
    move    y:ColRowBitsDebounced,a
    cmp     x0,a    y:PreviousTimeHigh,b ; if (ColRowBitsCurrent==ColRowBitsDebounced)
    jne     ButtonChanged1

    ; user is still pressing the same button, let's see if it was held for "hold" time
    move    y:PreviousTimeLow,b0
    jsr     SysGetCurrentTime          ; function returns time in ms in a
    sub     b,a                        ; a=CurrentTime-PreviousTime in msec
    move    #>HoldTime,x1

    move    a0,b
    cmp     x1,b

    ; test to see if hold time has elapsed, if not exit with no button event returned
    jlt     ReturnNoButtonEvent

    ; button is a "hold"
    jsr     SysGetCurrentTime          ; function returns time in ms in a
    move    a1,y:PreviousTimeHigh      ; save the current time to compare against later
    move    a0,y:PreviousTimeLow
    ; leave State as TestHoldRelease to detect more holds/releases
    ; of this button, no need to debounce again
    move    y:ColRowBitsDebounced,x0   ; Get debounced value to construct return value
    bset    #HoldBit,x0                ; set hold modifer in return value mapper, x0
    jmp     ReturnButtonEvent          ; jmp to return a button event value

ButtonChanged1

    ; ColRowBitsCurrent!=ColRowBitsDebounced
    ; This is either a single button release, ColRowBitsCurrent==0
    ; or a multiple button combo had at least
    ; one button released, ColRowBitsCurrent!=ColRowBitsDebounced
    ; Remember, we did debounce before we got here.
    move    y:ColRowBitsDebounced,x0   ; Get debounced value to construct return value
    bclr    #HoldBit,x0                ; clear hold modifer in return value mapper, x0
    push    x0                         ; save a copy of the return value mapper
    jsr     ResetButtonState           ; Call init code;
    pop     x0                         ; restore the return value mapper
    jmp     ReturnButtonEvent          ; jmp to return a button event value

ReturnNoButtonEvent
    ; no input was detected, so return no button event.
    move    y:<Const_000000,n1
    rts

ReturnButtonEvent
    ;x0 = button event

    ;-------------------------------- Hold Button Management -----------------------------------------------------------
    ; The "Hold Button Event" is always reported if the HoldButtonReportedBit is cleared, otherwise it is reported only if:
    ;       1 - Event is "press and release". The HoldButtonReportedBit is then cleared again.
    ;Check Hold Button supported 

  if (@DEF(HOLD_BUTTON_EVENT))      ;If Hold Button Supported

    ;Hold Button is supported, chek if detected             
    move    #>HOLD_BUTTON_EVENT,y0  
    move    x0,a
    and     y0,a
    jeq     _FindEvent                 ;Hold Button not detected, proceed ...
    
    ;Hold Button detected.
    ;Restore Hold Bit and copy new event (all button bit clear but the Hold) to x0
    bset    #HoldBit,a1
    and     x0,a
    move    a1,x0
    
    ;If Hold Button already reported and button event is "press and hold"
    ;return a No Event.
    btst    #HoldButtonReportedBit,y:Btl_Flags
    jcc     _HoldEvent
    
    btst    #HoldBit,x0
    jcc     _ReportHoldEvent
    
    jmp     ReturnNoButtonEvent
    
    ;Hold Button has been already reported and event is "press and release"
    ;We need to report the evnt and clear the "Hold Button already reported" bit
    ;so the next time the Hold Button Event is reported again
_ReportHoldEvent
    bclr    #HoldButtonReportedBit,y:Btl_Flags
    jmp     _FindEvent                                          

    ; Determine the return event
_HoldEvent
    bset    #HoldButtonReportedBit,y:Btl_Flags

  endif ;(@DEF(HOLD_BUTTON_EVENT))


_FindEvent    
    jsr     FindEvent

    rts

StateJumpTable
    dc      NoButtonsPressed
    dc      Debouncing
    dc      TestHoldRelease


;///////////////////////////////////////////////////////////////////////////////
;> Name:            ScanButtons
; Type:             Function
; Description:      Scan all matrix and non matrix buttons. 
;                   
; Inputs:           None.
; Outputs:          
;                   x0 = Button Pressed.
;                      = 0 if no buttons pressed
;
; Notes:            If hold button supported and pressed, the hold button number
;                   is returned in place of the actual button.
;<         
;///////////////////////////////////////////////////////////////////////////////
ScanButtons

    ; Scan Loop
    move    y:<Const_000000,y0          ; reset the bits
    move    y:Btl_Columns,x0            ; get the num cols
    move    #>Btl_ColumnEntries,r5      ; get the address of the first col descriptor
    move    #>Btl_ScanButtonEvents,r4   ; get ptr to the button numbers

    jsr     SysAllLinear
    ;-- Prevent 'do 0' 0xffff loop when 0 buttons are type scanned. Stmp00002469
    move    x0,a                        ; Btl_Columns zero check
    tst     a
    jeq     _scan_loop_end              ; Read non-scan buttons if none to scan.
    ;--
    
    do      x0,_scan_loop_end       ; For each column:
        ; enable the column
        move    (r5)+                  ; skip the enable register address
        move    (r5)+                  ; skip the data out register address
        move    y:(r5)+,r3             ; get the output enable register address
        move    y:(r5)+,x1             ; get the bit pos
        move    x:(r3),a               ; get the output enable register
        or      x1,a                   ; set the bit
        move    a1,x:(r3)
        ; now scan the rows
        move    #>Btl_RowEntries,r0     ; get the address of the firt col descriptor
        move    y:Btl_Rows,x0           ; get the num rows
        do      x0,_row_read_loop_end   ; read all the rows
            clr     a                   ; pause
            ; TODO:  abstract this count!  Value determined by trial-error.
            move    y:<Const_000080,a0
_row_pause
            dec     a
            jne     _row_pause
            move    (r0)+              ; skip the enable register address
            move    y:(r0)+,r2         ; get the data in register address
            move    y:(r0)+,a          ; get the bit pos
            move    y:(r4)+,y1         ; get the button number
            move    x:(r2),x0          ; get the data in register
            and     x0,a               ; isolate the bit to sense
            move    y:<Const_000000,a2 ; get around neg problem
            tst     a
            jeq     _row_read_next     ; if not pressed check next one
            move    y1,a               ; setup for setting the button number
            or      y0,a               ; set the bit
            move    a,y0               ; save it for next time
_row_read_next
            nop
            nop
_row_read_loop_end
        ; disable the column
        move    x1,a                   ; prep for bit clear
        not     a
        move    x:(r3),x1              ; get the output enable reg
        and     x1,a                   ; clear the bit
        move    a1,x:(r3)
_scan_loop_end

    ;
    ; Read the non-scan buttons
    ;
    move    y:Btl_NonScanButtons,a
    tst     a
    jeq     _lradc_check
    move    a,x0
    move    #>Btl_NonScanEntries,r0
    move    #>Btl_NonScanEvents,r4      ; get ptr to the button numbers
    do      x0,_nsb_read_loop_end       ; read all the rows
        clr     a                       ; pause
        move    y:<Const_000080,a0
_nsb_pause
        dec     a
        jne     _nsb_pause
        move    (r0)+                  ; skip the enable register address
        move    y:(r0)+,r2             ; get the data in register address
        move    y:(r0)+,a              ; get the bit pos
        move    y:(r4)+,y1             ; get the button number
        move    x:(r2),x0              ; get the data in register
        and     x0,a                   ; isolate the bit to sense
        move    y:<Const_000000,a2     ; get around neg problem
        tst     a
        jeq     _nsb_read_next         ; if not pressed check next one
        move    y1,a                   ; setup for setting the button number
        or      y0,a                   ; set the bit
        move    a,y0                   ; save it for next time
_nsb_read_next
        nop
        nop
_nsb_read_loop_end


_lradc_check

        if (@def('BUTTON_LRADC_BUTTONS'))
            if (BUTTON_LRADC_BUTTONS>0)

                ; get LRADC AUX value
                push    y0
                move    #>LRADC_AUX,a
                jsr     FSysLowResolutionAdcReadVolt         ; returns LRADC value, in mV
                pop     y0

                ; compare to button levels
                move    a,x1                                ; LRADC level
                move    y:Btl_LRADCLevels,a                 ; Number of LRADC "buttons"
                tst     a
                jeq     _sb_exit

                move    a,x0
                move    #Btl_LRADCEntries,r0
                move    #Btl_LRADCEvents,r4
                do  x0,_lradc_loop_end
                    move    y:(r4)+,y1                      ; The button number (bit pattern)
                    move    y:(r0)+,a                       ; Upper limit for this button
                    cmp     x1,a                            ; if (LRADC < button's upper limit)
                    jlt     _next_level

                    move    y1,a                            ;   OR in this button's bits
                    or      y0,a
                    move    a,y0
                    enddo
                    jmp     _sb_exit
_next_level
                    nop
                    nop
_lradc_loop_end

            endif
        endif

_sb_exit

    ; If Hold is supported then, if Hold is detected, mask all other buttons
  if (@DEF(HOLD_BUTTON_EVENT))          ;If HOLD Button supported
    move    #>HOLD_BUTTON_EVENT,x0  
    move    y0,a
    and     x0,a                       ; isolate the Hold button
    jeq     _sb_really_exit

    move    a1,y0                      ;keep sending hold
  endif ;(@DEF(HOLD_BUTTON_EVENT))

_sb_really_exit
    move    y0,x0                      
    rts

;///////////////////////////////////////////////////////////////////////////////
;> Name:            FindEvent
; Type:             Function
; Description:      Scan all matrix and non matrix buttons. 
;                   
; Inputs:           
;                   x0 = Button Number.
; Outputs:          
;                   n1.Bit15 = 1 Event found
;                              0 Event not found
;                   n1 Bit0..Bit14 = Event Number.
;
; Notes:            If hold button supported and pressed, the hold button number
;                   is returned in place of the actual button.
;<         
;///////////////////////////////////////////////////////////////////////////////
FindEvent
    move    y:Btl_NumEvents,x1          ; get number of events
    move    #Btl_EventMap,r0            ; point to first entry
    move    y:<Const_ffffff,m0
    clr     b   y:<Const_000001,y0     ; init the counter
_entry_search_loop
    move    y:(r0)+,a
    cmp     x0,a                       ; do we have a winner?
    jeq     _found_event
    add     y0,b                       ; increment the counter
    cmp     x1,b                       ; done?
    jne     _entry_search_loop
    move    y:<Const_000000,n1         ; no events to match the button combo
    rts
_found_event
    move    b,n1                       ; get the event number
    bset    #15,n1                     ; flag event found

    rts


;///////////////////////////////////////////////////////////////////////////////
;> Name:            SysGetButton
; Type:             Function
; Description:      Retrieves next button event to process from button buffer
;                   
; Inputs:           None.
; Outputs:          
;                   CARRY = 1 => No button events to process
;                   CARRY = 0 => At least one button event to process
;                   x0 = Button Event Number
;
; Notes:            
;<         
;///////////////////////////////////////////////////////////////////////////////
SysGetButton
    ; any buttons to process?
    move    #ButtonBufferDescriptor,r0
    move    y:<Const_ffffff,m0
    jsr     GetFilledSpace
    tst     a
    jeq     _no_button_events
  
    ; we have a button...
    move    y:ButtonBufferTailPtr,r0
    move    y:ButtonBufferModulo,m0
    nop
    move    y:(r0)+,x0
    move    r0,y:ButtonBufferTailPtr
    bset    #HW_SR_C_BITPOS,sr
    rts

_no_button_events
    bclr    #HW_SR_C_BITPOS,sr
    rts


;///////////////////////////////////////////////////////////////////////////////
;> Name:            SysButtonClearBuffer
; Type:             Function
; Description:      Clears all pending button events from button buffer
;                   
; Inputs:           None.
; Outputs:          None.
; Notes:            
;<         
;///////////////////////////////////////////////////////////////////////////////
SysButtonClearBuffer
    jsr SysGetButton
    jcs SysButtonClearBuffer
    rts
    
;///////////////////////////////////////////////////////////////////////////////
;> Name:            SysCheckForButton
; Type:             Function
; Description:      Checks for any pending button events in button buffer
;                   
; Inputs:           None.
; Outputs:          
;                   CARRY = 1 => No button events to process
;                   CARRY = 0 => At least one button event to process
;
; Notes:            This function is the same as SysGetButton(), but does not 
;                   retrieve the event from the button buffer.
;<         
;///////////////////////////////////////////////////////////////////////////////
SysCheckForButton
    ; any buttons to process?
    move    #ButtonBufferDescriptor,r0
    move    y:<Const_ffffff,m0
    jsr     GetFilledSpace
    tst     a
    jeq     _no_button_events
    bset    #HW_SR_C_BITPOS,sr
    rts
_no_button_events
    bclr    #HW_SR_C_BITPOS,sr
    rts

;///////////////////////////////////////////////////////////////////////////////
;> Name:            SysCheckForHoldButtonActivated
; Type:             Function
; Description:      Checks for HOLD button status
;                   
; Inputs:           None.
; Outputs:          
;                   CARRY = 1 => Hold button is activated.
;                   CARRY = 0 => Hold button is not activated.
;
; Notes:         
;<         
;///////////////////////////////////////////////////////////////////////////////
SysCheckForHoldButtonActivated
    btst    #HoldButtonReportedBit,y:Btl_Flags
    rts
    
;///////////////////////////////////////////////////////////////////////////////
;> Name:            ButtonGetTimeLast
; Type:             Function
; Description:      Calculates the time (ms) from last button activation to
;                   current.
;                   
; Inputs:           None.
; Outputs:          
;                   a0 = Time (ms) least significant word
;                   a1 = Time (ms) most significant word
;
; Notes:            If the BUTTON_TRACK_TIME_BIT is cleared, this function
;                   returns 0. See ButtonTrackTimeLastOn and ButtonTrackTimeLastOff
;                   functions.
;<         
;///////////////////////////////////////////////////////////////////////////////
ButtonGetTimeLast
    btst    #BUTTON_TRACK_TIME_BIT,y:Btl_Flags
    jcs     _DoTheMath

    clr     a
    rts

_DoTheMath
    jsr     SysGetCurrentTime

    move    x:ButtonTrackTimeHigh,b1
    move    x:ButtonTrackTimeLow,b0
    sub     b,a
    rts
    
;///////////////////////////////////////////////////////////////////////////////
;> Name:            LowResAdcRead
; Type:             Function
; Description:      Reads the low resolution ADC specified in x0 and does an
;                   average of LOW_RES_ADC_AVG_CNT samples
;                   
; Inputs:           x0 = Low Resolution ADC (BATT or AUX)
;                   y0 = Read Mask
;                   r1 = Pointer to ADC Var Table
; Outputs:          None         
;
; Notes:            
;<         
;///////////////////////////////////////////////////////////////////////////////
LowResAdcRead
    move    y:<Const_ffffff,m1    

    if (@def('DEVICE_3500'))
    ; Reads Batt and save result
    clr     b                           ; Check if BATT ADC
    move    #>LOW_RES_ADC_BATT,b1
    cmp     x0,b
    jeq     _3500_BAT_Read           	; BATT ADC

    clr     b                           ; Check if AUX1 or 2
    move    #>LOW_RES_ADC_AUX,b1
    cmp     x0,b
    jeq     _3500_AUX_Read           	; AUX

_3500_AUX2_Read
    jmp    _3500_Read_Done				

_3500_AUX_Read
	clr		a
	move	x:HW_LRADC1_RESULT,a1
	and		y0,a1

    rep #8								; Justify the result
        lsr     a1
	jmp 	_3500_Read_Done

_3500_BAT_Read
    clr     a
    move    x:HW_BATT_RESULT,a1
    and     y0,a1                       ; Clear all unused bits

    rep #8								; Justify the result
        lsr     a1
	jmp 	_3500_Read_Done

_3500_Read_Done


	else  ;3410

    ; Is DAC analog clock turned ON
    move    x:HW_CCR,x1
    jclr    #HW_CCR_ACKEN_BITPOS,x1,_TurnOnDacClock
    
    ; Reads ADC and save result
    clr     a
    move    x:HW_LRADC_RES,a1


    and     y0,a1                       ; Clear all unsused bits
    
    clr     b                           ; Check if BATT ADC
    move    #>LOW_RES_ADC_BATT,b1
    cmp     x0,b
    jeq     _ResultFormatted           ; BATT ADC, no need to format the results

                        ; For AUX ADC, need to shift right result 8 times
    rep #8
        lsr     a1
	endif   ;ifdef (DEVICE_3500)
        

_ResultFormatted
    ; Save away the result
    move    #>IndexLowResAdcResult,n1
    nop
    move    a1,y:(r1+n1)
    
    ; Accumulate result and save it
    move    #>IndexLowResAdcAccu,n1
    nop
    move    y:(r1+n1),x0
    add     x0,a
    move    a1,y:(r1+n1)
    
    ; Is it time to average ?
    move    #>IndexLowResAdcAvgCnt,n1
    clr     b
    move    y:(r1+n1),b0
    dec     b
    jeq     _AverageResult

    ; Save New Average Counter
    move    b0,y:(r1+n1)
    jmp     _Done    


_AverageResult    
    ; Average the result and save it away
    move    #>IndexLowResAdcAvg,n1

    rep #LOW_RES_ADC_AVG_SHIFTER
        lsr     a1

    move    a1,y:(r1+n1)
    
    jmp    _ResetAverageVars


_TurnOnDacClock        
    ; Turns ON Adc Clock
    move    #>(HW_CCR_ACKEN_SETMASK|HW_CCR_CKRST_SETMASK),a1
    or      x1,a1
    move    a1,x:HW_CCR
    
_ResetAverageVars
    ; Resets average counter
    move    #>IndexLowResAdcAvgCnt,n1
    move    #>LOW_RES_ADC_AVG_CNT,x1
    move    x1,y:(r1+n1)

    ; Resets ADC Accu
    move    #>IndexLowResAdcAccu,n1
    move    y:<Const_000000,x1
    move    x1,y:(r1+n1)
    
_Done
    rts                

    endsec


