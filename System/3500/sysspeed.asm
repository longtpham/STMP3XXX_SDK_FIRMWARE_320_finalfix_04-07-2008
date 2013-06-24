;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2004
;
;  File        : sysspeed.asm
;  Description : System Speed Routines. Includes public and private functions.
;///////////////////////////////////////////////////////////////////////////////

    section SYSFUN_SysSpeed

    opt     mex

;///////////////////////////////////////////////////////////////////////////////
;   Macros
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "sysmacro.asm"
    list

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "hwequ.inc"
    include "sysspeed.inc"
    include "resource.inc"
    include "project.inc"
    list

;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////
    global    SysSpeedResetAPI              ; Currently used only at system startup
    global   FSysSpeedSysStartUp            ; Once at system startup
    global    SysSpeedSysStartUp            
    global   FSysSpeedBoot                  ; Once at system startup
    global    SysSpeedBoot
    global   FSysSpeedSysShutDown           ; Once at system shutdown
    global    SysSpeedSysShutDown           
    global   FSysGetSpeed                   ; Public Interface (C)
    global    SysGetSpeed                   ; Public Interface (asm)
    global   FSysSpeedIncrease              ; Public Interface 
    global    SysSpeedIncrease              ; Public Interface 
    global   FSysSpeedClockFree             ; Public Interface 
    global    SysSpeedClockFree             ; Public Interface 
    global   FSysSpeedGetClockUserFlag      ; Only for the Executive speed client
    global    SysSpeedGetClockUserFlag      

    global   FSysSpeedForce                 ; SemiPrivate. Only used by test utilities.
    global    SysSpeedForce                 ; SemiPrivate. Only used by test utilities.
    global    ProgramPll                    ; Stub function
    
;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "const.xref"
    list

    if (@DEF('FUNCLET'))           
       extern    SysCallFunclet
    endif

	extern 	   Fg_bLimitedVDDD
    if !(@DEF('FORMATTER_TEST'))
	extern     FSteppingVoltageSet
    endif
	
    if (SPEED_BOOT_BATTERY_CHECK)	
    extern      FSysLowResolutionAdcReadVolt
    endif
;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////
; NOTE: Speed Index equates are in sysspeed.inc. 
; Default header file in mp3basecc\inc , project specific files are in 
; Timer Count Value = (Interval * Speed) / 2   (Interval is in microseconds)
CLOCK_INTERVAL  equ     1000            ; Increment system clock every 1 ms
BUTTON_INTERVAL equ     40000           ; Check buttons every 40 ms

; Number of loop iterations needed to wait for the PLL and DC-DC to settle
; Count = Delay * 24.576MHz / 3  (Delay is in seconds)
SETTLING_COUNT  equ     10000           ; 1.23 ms

; Core voltage (2.5 volt DC-DC) channel (S2C) field in the DCDC Status register
;S2C           equ   4                   ; Starts at bit 4           
;DCDC_STS_S2C_MASK equ   $F<<S2C         ; Four bits wide

SPEED_FIRST      equ     23              ; bit indicator if changing speed/voltage first


; speed table offsets
PDIV_OFFSET     equ     0
DDIV_OFFSET     equ     1
VDDD_OFFSET     equ     2
BO_OFFSET       equ     3
TIMER1_OFFSET   equ     4
TIMER2_OFFSET   equ     5
GF_OFFSET       equ     6

; Number of words in each entry of the speed table
SPEED_ENTRY_SIZE equ    7

 if !(@DEF(HW_DCDC_VDDD_VLVL_BITPOS))
HW_DCDC_VDDD_VLVL_BITPOS    equ    0
 endif 

; Note that 3 was the optimal value as seen for VDDIO and VDDD on a scope.
; This works in conjunction with a delay per iteration after VDD_OK bit. See SteppingVoltageSet
VOLTAGE_BITS_PER_STEP   equ 3


;///////////////////////////////////////////////////////////////////////////////
;  X memory
;///////////////////////////////////////////////////////////////////////////////

    org x,"SYSFUN_SysSpeed_X":
                
;///////////////////////////////////////////////////////////////////////////////
;  Y memory
;///////////////////////////////////////////////////////////////////////////////

    org y,"SYSFUN_SysSpeed_Y":

; Note: Do not change SpeedIndex value here from SPEED_UNITIALIZED or startup will fail 
;       with updated SysSetSpeed.         
SpeedIndex
        dc      SPEED_UNINITIALIZED  ; Current Index saved here. High for HW requirement. 
OldSpeedIndex
        dc      SPEED_MP3
;---------------------------------------------------
; SysSpeedIncrease() and SysSpeedClockFree() use this client bit flag.
; Each bit below is a binary semaphore set by a corresponding clock API user.
; Generally, new clock adjusting modules must insert a bit ID in sysspeed.inc 
; and use the SysSpeedIncrease(MY_BIT) and SysSpeedClockFree(MY_BIT) API passing 
; in their ID from sysspeed.inc for above SysSpeed calls to manipulate the 
; corresponding client bit flag.
clockUserFlag  dc      0                        ; Private to this module

; Table saves requested client speed indeces, 
; written by SysSpeedIncrease(), read by SysSpeedClockFree() for restoration.
SavedClientSpeedIndeces ds SPEED_CLIENT_HIGHEST_USER+1
;---------------------------------------------------
;g_wBootSpeedIndex  dc      255 ;define during profiling only. Records which boot speed used. Comment out if not profiling.
                

;///////////////////////////////////////////////////////////////////////////////
;   P Memory
;///////////////////////////////////////////////////////////////////////////////

    org p,"SYSFUN_SysSpeed_P":
        

;///////////////////////////////////////////////////////////////////////////////
;>  Name:           SysGetSpeed   or    int currentSpeedIndex SysGetSpeed();
;
;   Type:           Function
;
;   Description:    Asm or C version returns current value of y:SpeedIndex in 
;                   X1 or A1 respectively. (Registers A2 and A0 cleared in C ver)
;
;   Inputs:         None
;
;   Outputs:        x1 = Current Speed Index; C version also returns this in A1.
;                   Speed index values in increasing order:
;                   SPEED_IDLE = Used during idle operation (no audio decoding)  
;                   SPEED_MP3  = Used during MP3 decode operation 
;                   SPEED_MAX  = Used during browsing, shutdown, etc -- when not playing
;                   SPEED_WMA  = Used during Windows Media Audio decode operation
;                   SPEED_MP3ENCODE = Used during mp3 encode from line-in, mic, or FM.
;
;   Notes:          See also SysSpeed.inc for Speed API.
;<
;///////////////////////////////////////////////////////////////////////////////
FSysGetSpeed
    move    y:SpeedIndex,a              ; current Speed Index
SysGetSpeed
    move    y:SpeedIndex,x1             ; current Speed Index
    rts


;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysSetSpeed         int SysSetSpeed(int DesiredSpeedIndex);
;
;   Type:           Private Function. Used only by speed management API internals.
;
;   Description:    Programs the PLL and VDDD to the indexed values 
;                   of the speed table to provide the MIPS necessary for various
;                   system operational modes.
;
;   Inputs:         Desired speed index in x1; or from C: a1 is desired speed index
;
;                   Speed index values ** MUST BE ORDERED FROM LOWEST TO HIGHEST FREQ **:
;                   SPEED_IDLE = Used during idle operation (no audio decoding)  
;                   SPEED_MP3  = Used during MP3 decode operation 
;                   SPEED_MAX  = Used during browsing, shutdown, etc -- when not playing
;                   SPEED_WMA  = Used during WMA decode operation 
;                   SPEED_MP3ENCODE = Used during mp3 line-in, mic, or FM record.
;
;   Outputs:        Register A1 returns y:SpeedIndex that was active on function entry 
;                   
;   Notes: *** Routine should ONLY be used by the API wrapper at 'EOF & sysspeed.inc'*** 
;       All callers that were using SysSetSpeed must convert to use that API instead.
;
;       STMP3410 Reset values are : xtal 24.576MHz / Vddd = 1.77
;       when increasing frequency, the voltage should be increased first
;       when decreasing frequency, the frequency should be lowered before voltage
;
;       To minimize code complexity: IDLE speed index MUST be initially called
;               after reset so the same code path will be taken for the reset 
;               condition (freq lowered first). If a faster initial speed is
;               required change order at _CheckReset.
;
;       when adjusting voltage the brownout voltage should be considered:
;               lower before the core voltage is lowered
;               raise core voltage and settle before setting brownout
;
;       dc-dc settling time is about 1V per millisecond dV/dt, assuming SigmaTel recommended
;               configuration (C=64 and dcdc_clk = 1.5MHz) 
;
;       DDIV takes 256 dclocks to go into effect, do not enable pll until
;               DDIV is in effect because Vddd is set for final clock speed
;               and some pll values can cause core instability (too high freq)
;
;       OldSpeedIndex stores previous Speed Index
;               HOWEVER during the function msb is set to indicate speed 
;               changing first
;
;       All timing loops assume running off crystal with no dividers (DDIV = 0) 
;
;       Static Register Usage:
;       x0 = PDIV
;       x1 = DDIV  (after input value is used to find table entry)
;       r2 =  beginning of speed table entry
;       r3 =  y:OldSpeedIndex
;<
;///////////////////////////////////////////////////////////////////////////////
FSysSetSpeed                    ; File Private entry point.
FSysSpeedForce                  ; C entry point for test utilities only.
    move    a1,x1
SysSetSpeed                     ; File private entry point.
SysSpeedForce                   ; Asm entry point for test utilities only.
    if (@DEF('FUNCLET'))           
   
       ;If using funclet, function will be executed via funclet kernel
       CallFunclet RSRC_FUNCLET_SYSSETSPEED
   
       org p,"SYSFUNCLET_SysSetSpeed_P":
       
       dc      RSRC_FUNCLET_SYSSETSPEED           ; Funclet Resource number
   
    endif
    ; If limited VDDD part.
	move	#>Const_000000,y1
	move	y:Fg_bLimitedVDDD,b
	cmp		y1,b
	jeq		_ValidateSpeedIndex ; jumps to skip ceil if vddd not limited. 
	                            ;(stability for future 3502 3501 device which may be qual'd at 60MHz)

    ; This is a vddd limited part, so ceil the speed index. 
    move    #>SPEED_WMA,a  ;Warning: if SPEED_WMA is changed above 60. Must always have 60 here. 
    cmp     x1,a                
    jge     _ValidateSpeedIndex
    move    a1,x1              ; If requested index>60Mhz, ceil requested speed index to 60MHz since vddd limited part

_ValidateSpeedIndex
    ; Verify that the speed index (x1) is valid
    move    #>SPEED_LAST,a
    cmp     x1,a                
    jge     _CurrentVsRequested
    
    ; Invalid speed index value
    error
    nop

_CurrentVsRequested 
    move    y:SpeedIndex,y1	                      ; save current one on the stack
    push    y1
    ; If requested speed index equals current speed index, exit now.  
    ; Prevents system instability in case of excessive routine calls.
	clr     b    
    move    y:SpeedIndex,b1
    move    x1,a
    cmp     a,b
    jeq     _Exit

_WhichFirst
    ; Update old & new speed Index vars
    move    b1,y:OldSpeedIndex
    move    x1,y:SpeedIndex             ; used by SysGetSpeed

    ;Voltage or speed first ???
    ; if old freq (b1) >= new freq (x1), lower freq first (SPEED_FIRST)
    ; otherwise raise voltage before raising frequency
    cmp     x1,b
    jle     _TableEntry
    bset    #SPEED_FIRST,y:OldSpeedIndex

_TableEntry       
    ; Calculate the offset into the speed table
    move    #>SPEED_ENTRY_SIZE,y0
    mpy     x1,y0,a                     ; Offset = speed index * entry size
    asr     a                       
    move    a0,a                        ; Offset is in A

    ; Point to the selected entry in the speed table
    move    #>SpeedTable,y0
    add     y0,a
    move    a,r1                        ; used by auto incrementing functions
    move    a,r2                        ; always points to beginning of entry
    move    y:<Const_ffffff,m1

    move    x:HW_CCR,b

    ; Get the new divider values from the table
    movem   p:(r1)+,x0                  ; Get PDIV value from table
    movem   p:(r1)+,x1                  ; Get DDIV value from table

_CheckReset
    ; Check if the clock logic block is still in reset
    ; If not in reset state then check the speed
    ; Otherwise set speed first *** see NOTES: *** 
    jset    #HW_CCR_CKRST_BITPOS,b,_PostReset        
     
    ; Always set the speed first if coming out of reset *** see NOTES: ***
    bset    #SPEED_FIRST,y:OldSpeedIndex
    move    #OldSpeedIndex,r3
    jmp     _SetSpeed

_PostReset
    ; if new freq <= old freq, lower freq first
    ; otherwise raise voltage before raising frequency
    move    #OldSpeedIndex,r3
    nop
    nop
    jclr    #SPEED_FIRST,y:(r3),_SetVoltage

    
_CheckSpeed
    ; If already set to the requested speed then check voltage
    move    x0,a                    
    or      x1,a                        ; A = PDIV | DDIV from table
    move    #>HW_CCR_PDIV_SETMASK|HW_CCR_DDIV_SETMASK,y0
    and     y0,b                        ; B = PDIV | DDIV from register
    cmp     a,b
    jeq     _SetVoltage

_SetSpeed
    ; if new freq <= old freq, lower freq first
    ; otherwise raise voltage before raising frequency
    jset    #SPEED_FIRST,y:(r3),_SetPll
        

;~~~~~~~~~~~~~~~~~~~~ CHANGING VOLTAGE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
_SetVoltage
    ; Set the DC-DC converter to the new core voltage (Vddd)
    ; DCDC converter needs to be set before the PLL change, some speeds requiring higher voltages
    ; to function properly

    ; if setting speed first, assume voltage is being lowered so change brownout voltage first
    ; else raising voltage so raise Vddd first and allow to settle before setting new brownout
    jset    #SPEED_FIRST,y:(r3),_SetBOVolts

_SetVdddVolts
	move    #>VDDD_OFFSET,n2             ; do this here for pipeline efficiency

	; For parts with minimum VDDD settings (3501, 3502), skip this step.
	; ASSUMING that the higher static VDDD setting is appropriate for any speed setting 
	; resulting from the rest of this subroutine.
	move	#>Const_000000,y1
	move	y:Fg_bLimitedVDDD,a
	cmp		y1,a
	jeq		_SVContinue1
	jmp		_SVSkip1
_SVContinue1

    ;------------------
;old ver of VDDD set.
     ;move    #>HW_DCDC_VDDD_VOLTAGE_LEVEL_CLRMASK,y1
     ;move    x:HW_DCDC_VDDD,a
     ;and     y1,a                        ; Erase the previous VDDD voltage
     ;
     ;movem   p:(r2+n2),y0                ; Get new core voltage from table
     ;or      y0,a                        ; OR in the new VDDD value
     ;move    a,x:HW_DCDC_VDDD            ; Set the new VDD voltage

;else newer V set approach with stepping. New ver also sets HW_DCDCTBR as old ver used to here.
    ;------------------
    ; call the new SteppingVoltageSet c function to set voltage
    ;           SteppingVoltageSet(int iCurrentVoltageCode, int iDesiredVoltageCode, int iStepSize, 
    ;                                          int iVoltageType) ;
    ; When called from asm, the register sequence with these params is: a,b,x0,y0

    ; Value in A is not used later so don't save & restore it.
    push    b2
    push    b1
    push    b0
    push    x0
    push    y0
    push    x1
    
    move    x:HW_DCDC_VDDD,a ; original voltage in sub bitfield of this word
    move    #>HW_DCDC_VDDD_VOLTAGE_LEVEL_SETMASK,y1    
    and     y1,a             ; original voltage masked into a.    
    movem   p:(r2+n2),b      ; new desired VDDD core voltage from the table. n2=2
    move    #>VOLTAGE_BITS_PER_STEP,x0           ; iStepSize of 3 at ~30mV each gives 90mV per step.
    move    #>1,y0           ; iVoltageType 1 is VOLTAGE_TYPE_VDDD \

    if !(@DEF('FORMATTER_TEST'))
    jsr     FSteppingVoltageSet
    endif

    tst     a
    ccerror ne  ; DEBUG build halts on error code return Not Equal to zero. See A1 for code. 
                ;; RETAIL build resets the processor.     
    pop     x1
    pop     y0
    pop     x0
    pop     b0
    pop     b1
    pop     b2
;endif
    ;----------------           
    ; HW_DCDCTBR set subroutine was moved from here and imported under SteppingVoltageSet  
    ; interface since the TBR's DCDC1 duty cycle & VDD5V_ACTIVE fields must be set 
    ; per VDDD voltage change at time of change, according to SOC designer. 

_SVSkip1
    ; if speed first then already did brownout
    jset    #SPEED_FIRST,y:(r3),_VoltageDone

    ; otherwise changing Vddd first so allow to settle before changing brownout
    ; Timing loop uses crystal clock and no post-divider
    clr     a
    move    #>SETTLING_COUNT,a0
_SettlingDelay1  ; Each loop is 3 clocks (122 ns per loop if fDCLK=24.xMHz)
    dec     a
    jne     _SettlingDelay1                        

_SetBOVolts
        ; Adjust the Brownout Voltage
        move    #>BO_OFFSET,n2

	; For parts with minimum VDDD settings (3501), skip this step.
	; ASSUMING that the higher static VDDD setting is appropriate for any speed setting 
	; resulting from the rest of this subroutine.
	move	#>Const_000000,y1
	move	y:Fg_bLimitedVDDD,a
	cmp		y1,a
	jeq		_SVContinue2
	jmp		_SVSkip2
_SVContinue2


        move    x:HW_DCDC_VDDD,a
        move    #>HW_DCDC_VDDD_BROWNOUT_LEVEL_CLRMASK,y1  ; clear mask for Vddd brownout bits
        and     y1,a                        ; Erase the previous Brownout voltage

        movem   p:(r2+n2),y0                ; Get new brownout voltage from table                                              
        or      y0,a                        ; OR in the Brownout value

        move    a,x:HW_DCDC_VDDD            ; Set the new Brownout voltages

_SVSkip2
     ; if speed first then doing brownout first so adjust Vddd now
     jset    #SPEED_FIRST,y:(r3),_SetVdddVolts


_VoltageDone
    ; if speed set first then we're done -- otherwise adjust pll
    jset    #SPEED_FIRST,y:(r3),_Continue
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


;~~~~~~~~~~~~~~~~~~~~ CHANGING PLL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;;;;;
;;; requires x0 = PDIV
;;;          x1 = DDIV
;;;;;

_SetPll
    ; Set the PLL frequency:
    ; 1 - switch clock sources to xtal
    ; 2 - enable the analog clocks; pll block
    ;   - DDIV set to zero (no divider)
    ;   - PDIV set to indexed speed table divider
    ; 3 - wait pll settling delay
    ; 4 - DDIV set to indexed speed table divider
    ; 5 - wait for DDIV to take effect (256 clks)
    ; 6 - switch clock source to pll

    ; Switch the clock source to the crystal
    bclr    #HW_CCR_CKSRC_BITPOS,x:HW_CCR
    nop
    nop
        
    ; Read the current contents of the Clock Control register
    move    x:HW_CCR,b

    ; Turn on all the enables
    move    #>HW_CCR_ACKEN_SETMASK|HW_CCR_XTLEN_SETMASK|HW_CCR_PLLEN_SETMASK|HW_CCR_LTC_SETMASK|HW_CCR_CKRST_SETMASK,y0
    or      y0,b

    ; Clear the PDIV and the DDIV fields
    move    #>~(HW_CCR_PDIV_SETMASK|HW_CCR_DDIV_SETMASK),y0
    and     y0,b
    
    ; Set the new PDIV value
    or      x0,b

    ; Set DAC Clock.  These go with the values in mp3mem.asm and dacequ.inc for sample rate adjustments
	;If 3500 -- use default values from AnalogInit()

    ; Write the new contents to the Clock Control register
    move    b,x:HW_CCR
    nop
    nop


    ; Delay while the PLL settle
    clr     a
    move    #>SETTLING_COUNT,a0
_SettlingDelay  ; Each loop is 3 clocks (122 ns per loop)
    dec     a
    jne     _SettlingDelay                        

    ; Set the post-divider to the new DDIV value
    or      x1,b
    move    b,x:HW_CCR

    ; Wait  Clocks for DDIV change to complete
    do    #128,_DDIVSettle
    nop
    nop
_DDIVSettle

    ; Switch the clock source from the crystal to the PLL
    bset    #HW_CCR_CKSRC_BITPOS,b
    move    b,x:HW_CCR

   ; if new speed <= old speed:
   ; speed is being set first, so now do voltage
    jset    #SPEED_FIRST,y:(r3),_SetVoltage
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

_Continue
    ; fix table pointer so can autoincrement for the last 3
    clr     a
    move    r2,a1
    move    #>TIMER1_OFFSET,x0
    add     x0,a
    move    a1,r1
    nop
    nop
    ; Adjust the system clock timer count (timer 1) for the new clock speed
    movem   p:(r1)+,y0                  ; Get the timer count from the table
    move    y0,x:HW_TMR1CNTR

    ; Adjust the button timer count (timer 2) for the new clock speed
    movem   p:(r1)+,y0                  ; Get the timer count from the table
    move    y0,x:HW_TMR2CNTR
      

_GPSpeed
    ; Adjust GPFlash port access time. This pulls a word from last entry in SpeedTable _GPSPEED
    ; JLN Note: the 2 instructions below could be changed to get the best timing 
    ; for this flash but the nandtables may only have it for an assumed fDCLK of 68MHz.
    movem   p:(r1)+,y0
    move    y0,x:HW_GPFLASH_TIMING2R       

_Exit
    pop a                           ;move the old speed into the return value
    rts
    
SpeedTable
;!!!!! ** MUST BE ORDERED FROM LOWEST TO HIGHEST FREQ ** !!!!!    
        ; SPEED_IDLE
        dc      IDLE_PDIV<<HW_CCR_PDIV_B0_BITPOS        ; PLL divider
        dc      IDLE_DDIV<<HW_CCR_DDIV_B0_BITPOS        ; Post divider
        dc      IDLE_VOLTAGE<<HW_DCDC_VDDD_VLVL_BITPOS  ; VDD voltage
        dc      IDLE_BOVOLTAGE
        dc      IDLE_SPEED*BUTTON_INTERVAL/2            ; Timer 1 count
        dc      IDLE_SPEED*CLOCK_INTERVAL/2             ; Timer 2 count
        dc      IDLE_GPSPEED                            ; CompactFlash speed        
		; SPEED_AUDIBLE_MP3
        dc      AUDIBLE_MP3_PDIV<<HW_CCR_PDIV_B0_BITPOS        ; PLL divider
        dc      AUDIBLE_MP3_DDIV<<HW_CCR_DDIV_B0_BITPOS        ; Post divider
        dc      AUDIBLE_MP3_VOLTAGE<<HW_DCDC_VDDD_VLVL_BITPOS  ; VDD voltage
        dc      AUDIBLE_MP3_BOVOLTAGE
        dc      AUDIBLE_MP3_SPEED*BUTTON_INTERVAL/2            ; Timer 1 count
        dc      AUDIBLE_MP3_SPEED*CLOCK_INTERVAL/2             ; Timer 2 count
        dc      AUDIBLE_MP3_GPSPEED                            ; CompactFlash speed 
		; SPEED_AUDIBLE_ACELP_SR8KHz
        dc      AUDIBLE_ACELP_SR8KHz_PDIV<<HW_CCR_PDIV_B0_BITPOS        ; PLL divider
        dc      AUDIBLE_ACELP_SR8KHz_DDIV<<HW_CCR_DDIV_B0_BITPOS        ; Post divider
        dc      AUDIBLE_ACELP_SR8KHz_VOLTAGE<<HW_DCDC_VDDD_VLVL_BITPOS  ; VDD voltage
        dc      AUDIBLE_ACELP_SR8KHz_BOVOLTAGE
        dc      AUDIBLE_ACELP_SR8KHz_SPEED*BUTTON_INTERVAL/2            ; Timer 1 count
        dc      AUDIBLE_ACELP_SR8KHz_SPEED*CLOCK_INTERVAL/2             ; Timer 2 count
        dc      AUDIBLE_ACELP_SR8KHz_GPSPEED                            ; CompactFlash speed
        ; SPEED_FM  
        dc      FM_PDIV<<HW_CCR_PDIV_B0_BITPOS         ; PLL divider
        dc      FM_DDIV<<HW_CCR_DDIV_B0_BITPOS         ; Post divider
        dc      FM_VOLTAGE<<HW_DCDC_VDDD_VLVL_BITPOS   ; Core voltage
        dc      FM_BOVOLTAGE
        dc      FM_SPEED*BUTTON_INTERVAL/2             ; Timer 1 count
        dc      FM_SPEED*CLOCK_INTERVAL/2              ; Timer 2 count
        dc      FM_GPSPEED                             ; CompactFlash speed
        ; SPEED_FM_EQ  
        dc      FM_EQ_PDIV<<HW_CCR_PDIV_B0_BITPOS      ; PLL divider
        dc      FM_EQ_DDIV<<HW_CCR_DDIV_B0_BITPOS      ; Post divider
        dc      FM_EQ_VOLTAGE<<HW_DCDC_VDDD_VLVL_BITPOS   ; Core voltage
        dc      FM_EQ_BOVOLTAGE
        dc      FM_EQ_SPEED*BUTTON_INTERVAL/2          ; Timer 1 count
        dc      FM_EQ_SPEED*CLOCK_INTERVAL/2           ; Timer 2 count
        dc      FM_EQ_GPSPEED                             ; CompactFlash speed
        ; SPEED_AUDIBLE_ACELP_SR16KHz
        dc      AUDIBLE_ACELP_SR16KHz_PDIV<<HW_CCR_PDIV_B0_BITPOS        ; PLL divider
        dc      AUDIBLE_ACELP_SR16KHz_DDIV<<HW_CCR_DDIV_B0_BITPOS        ; Post divider
        dc      AUDIBLE_ACELP_SR16KHz_VOLTAGE<<HW_DCDC_VDDD_VLVL_BITPOS  ; VDD voltage
        dc      AUDIBLE_ACELP_SR16KHz_BOVOLTAGE
        dc      AUDIBLE_ACELP_SR16KHz_SPEED*BUTTON_INTERVAL/2            ; Timer 1 count
        dc      AUDIBLE_ACELP_SR16KHz_SPEED*CLOCK_INTERVAL/2             ; Timer 2 count
        dc      AUDIBLE_ACELP_SR16KHz_GPSPEED                            ; CompactFlash speed        
        ; SPEED_MP3
        dc      MP3_PDIV<<HW_CCR_PDIV_B0_BITPOS         ; PLL divider
        dc      MP3_DDIV<<HW_CCR_DDIV_B0_BITPOS         ; Post divider
        dc      MP3_VOLTAGE<<HW_DCDC_VDDD_VLVL_BITPOS   ; Core voltage
        dc      MP3_BOVOLTAGE
        dc      MP3_SPEED*BUTTON_INTERVAL/2             ; Timer 1 count
        dc      MP3_SPEED*CLOCK_INTERVAL/2              ; Timer 2 count
        dc      MP3_GPSPEED                             ; CompactFlash speed
        ; SPEED_ADPCM
        dc      ADPCM_PDIV<<HW_CCR_PDIV_B0_BITPOS         ; PLL divider
        dc      ADPCM_DDIV<<HW_CCR_DDIV_B0_BITPOS         ; Post divider
        dc      ADPCM_VOLTAGE<<HW_DCDC_VDDD_VLVL_BITPOS   ; Core voltage
        dc      ADPCM_BOVOLTAGE
        dc      ADPCM_SPEED*BUTTON_INTERVAL/2             ; Timer 1 count
        dc      ADPCM_SPEED*CLOCK_INTERVAL/2              ; Timer 2 count
        dc      ADPCM_GPSPEED                             ; CompactFlash speed
        ; SPEED_MIXER
        dc      MIXER_PDIV<<HW_CCR_PDIV_B0_BITPOS         ; As in the idle case's speed
        dc      MIXER_DDIV<<HW_CCR_DDIV_B0_BITPOS
        dc      MIXER_VOLTAGE<<HW_DCDC_VDDD_VLVL_BITPOS   ; Core voltage
        dc      MIXER_BOVOLTAGE
        dc      MIXER_SPEED*BUTTON_INTERVAL/2             ; Timer 1 count
        dc      MIXER_SPEED*CLOCK_INTERVAL/2              ; Timer 2 count
        dc      MIXER_GPSPEED                             ; CompactFlash speed
        ; SPEED_ADCBASE
        dc      ADCBASE_PDIV<<HW_CCR_PDIV_B0_BITPOS       ; PLL divider
        dc      ADCBASE_DDIV<<HW_CCR_DDIV_B0_BITPOS       ; Post divider
        dc      ADCBASE_VOLTAGE<<HW_DCDC_VDDD_VLVL_BITPOS ; Core Voltage
        dc      ADCBASE_BOVOLTAGE
        dc      ADCBASE_SPEED*BUTTON_INTERVAL/2           ; Timer 1 count
        dc      ADCBASE_SPEED*CLOCK_INTERVAL/2            ; Timer 2 count
        dc      ADCBASE_GPSPEED
        ; SPEED_MAX
        dc      MAX_PDIV<<HW_CCR_PDIV_B0_BITPOS         ; PLL divider
        dc      MAX_DDIV<<HW_CCR_DDIV_B0_BITPOS         ; Post divider
        dc      MAX_VOLTAGE<<HW_DCDC_VDDD_VLVL_BITPOS   ; Core voltage
        dc      MAX_BOVOLTAGE
        dc      MAX_SPEED*BUTTON_INTERVAL/2             ; Timer 1 count
        dc      MAX_SPEED*CLOCK_INTERVAL/2              ; Timer 2 count
        dc      MAX_GPSPEED                             ; CompactFlash speed
        ; SPEED_WMA
        dc      WMA_PDIV<<HW_CCR_PDIV_B0_BITPOS         ; PLL divider
        dc      WMA_DDIV<<HW_CCR_DDIV_B0_BITPOS         ; Post divider
        dc      WMA_VOLTAGE<<HW_DCDC_VDDD_VLVL_BITPOS   ; Core voltage
        dc      WMA_BOVOLTAGE
        dc      WMA_SPEED*BUTTON_INTERVAL/2             ; Timer 1 count
        dc      WMA_SPEED*CLOCK_INTERVAL/2              ; Timer 2 count
        dc      WMA_GPSPEED                             ; CompactFlash speed
        ; SPEED_MP3ENCODE
        dc      MP3ENCODE_PDIV<<HW_CCR_PDIV_B0_BITPOS         ; PLL divider
        dc      MP3ENCODE_DDIV<<HW_CCR_DDIV_B0_BITPOS         ; Post divider
        dc      MP3ENCODE_VOLTAGE<<HW_DCDC_VDDD_VLVL_BITPOS   ; Core voltage
        dc      MP3ENCODE_BOVOLTAGE
        dc      MP3ENCODE_SPEED*BUTTON_INTERVAL/2             ; Timer 1 count
        dc      MP3ENCODE_SPEED*CLOCK_INTERVAL/2              ; Timer 2 count
		dc      MP3ENCODE_GPSPEED                             ; Compact Flash speed
        ; SPEED_PEAK
        dc      PEAK_PDIV<<HW_CCR_PDIV_B0_BITPOS         ; PLL divider
        dc      PEAK_DDIV<<HW_CCR_DDIV_B0_BITPOS         ; Post divider
        dc      PEAK_VOLTAGE<<HW_DCDC_VDDD_VLVL_BITPOS   ; Core voltage
        dc      PEAK_BOVOLTAGE
        dc      PEAK_SPEED*BUTTON_INTERVAL/2             ; Timer 1 count
        dc      PEAK_SPEED*CLOCK_INTERVAL/2              ; Timer 2 count
        dc      PEAK_GPSPEED                             ; Compact Flash speed       


    if (@DEF('FUNCLET'))
    
       ;If using funclet, switch counter back to original here
       org    p,"SYSFUN_SysSpeed_P":                             
   
    endif


;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           ProgramPll
;
;   Type:           Function
;
;   Description:    Dummy routine to make SystemRoutineListPtr happy in sysmem.asm.
;                   SysSetSpeed replaces ProgramPll in the atlm project.
;
;   Inputs:         ???
;
;   Outputs:        ???
;                   
;   Notes:          ???
;<
;///////////////////////////////////////////////////////////////////////////////
ProgramPll
    rts

;///////////////////////////////////////////////////////////////////////////////
;>  Name:           SysSpeedResetAPI
;   Type:           API Function
;   Description:    Reset internal API data structures. For use in emergencies.                   
;   Inputs:         None
;   Outputs:        None
;   Notes:          C or Asm callable.
;<
;///////////////////////////////////////////////////////////////////////////////
FSysSpeedResetAPI 
SysSpeedResetAPI 
    clr     B
    move    b1,y:clockUserFlag
    
    move    #>$00deaf,b1  ; fill table with this invalid speed index values
    move    #>SavedClientSpeedIndeces,r0
    rep     #SPEED_CLIENT_HIGHEST_USER+1
     move   b1,y:(r0)+
    nop 
    rts

;///////////////////////////////////////////////////////////////////////////////
;>  Name:           SysSpeedSysStartUp
;   Type:           API Function
;   Description:    Must be called once at system startup. 
;   Inputs:         None
;   Outputs:        None
;   Notes:          C or Asm callable.
;<
;///////////////////////////////////////////////////////////////////////////////
FSysSpeedSysStartUp ; C entry point
SysSpeedSysStartUp
    move    #>SPEED_IDLE,x1
    jsr     SysSetSpeed
    rts    

;///////////////////////////////////////////////////////////////////////////////
;>  Name:           SysSpeedBoot
;   Type:           API Function
;   Description:    Should only be called one time at system startup. 
;   Inputs:         No register inputs
;     Users can enable one of ATTEMPT_FAST_BOOT or FASTEST boot project.inc defines by 
;     defining values as 0 instead of 1, else default to boot speed used in previous SDKs.
;   Outputs:        Returns previous speed in A
;                   Writes core speed & voltage registers
;   Notes:          C or Asm callable. 
;<
;///////////////////////////////////////////////////////////////////////////////
VBAT_LIION_THRESH_MV        equ 3200
VBAT_NON_LIION_THRESH_MV    equ 1200

FSysSpeedBoot  ; C entry point
SysSpeedBoot   
 if (SPEED_BOOT_BATTERY_CHECK)
    ;// Low Resolution ADC battery chan is already init by MiscProjectInit()
    ;// Get the current Vbat value  
    move    #>Const_000000,a    ; BATT param is 0
    move    #>Const_000000,b    ; bAverage bool param is INSTANT_RESULT = FALSE = 0
    jsr     FSysLowResolutionAdcReadVolt ;//params: (BATT, INSTANT_RESULT) 
    ; LRADC Vbat result in A.

    move    #>SPEED_MAX,x1        ; default: Change to safe player proven speed. Slower than other boot speeds.
 
  if @def('BATTERY_TYPE_LI_ION')
    ;WARN '** SysSpeedBoot using BATTERY_TYPE_LI_ION threshholds **'
    move    #>VBAT_LIION_THRESH_MV,b
    ; comment out this jmp when the LIION battery read is verified as reading correct voltages. done.
    ;jmp     _setTheBootSpeed
  else
    ;WARN '** SysSpeedBoot using alkaline threshholds **'
    move    #>VBAT_NON_LIION_THRESH_MV,b
  endif ; batt type determines threshhold to use
    cmp     A,B                   ; compare Vbat to battery type's min boot voltage for this speed.
    jlt     _trySpeedBoot         ; if B thresh < A Vbat, jump to _trySpeedBoot

    ; Vbat below thresh so keep safer default player proven speed. (slower)
        ; debug ; A,B register testpoint to see lradc api value accuracy with DC supply.
        ; nop
    move    #>SPEED_MAX,x1 
    jmp     _setTheBootSpeed
 else  ; SPEED_BOOT_BATTERY_CHECK
    move    #>SPEED_MAX,x1        ; default: Sets safe player proven speed. Slower than other boot speeds.
    ; Note that in this mode w/o Vbat check, we unconditionally honor the ATTEMPT_FAST_BOOT config values.
 endif ; SPEED_BOOT_BATTERY_CHECK -----------


_trySpeedBoot
	; For parts with minimum VDDD settings (3501, 3502), accelerate startup a little less than other parts. 
	; (60MHz (SPEED_WMA) may be highest guaranteed by future 3502 qualification)
	move	#>Const_000000,y1
	move	y:Fg_bLimitedVDDD,a
	cmp		y1,a
	jeq		_SSBNonLimited
 if (ATTEMPT_FAST_BOOT)
    move    #>SPEED_WMA,x1       ; for vddd limited parts, ceil
 endif
 if (ATTEMPT_FASTEST_BOOT)
    move    #>SPEED_WMA,x1       ; for vddd limited parts, ceil
 endif 
	jmp     _setTheBootSpeed     ; set to 60MHz since this is the limited vddd part's max rated speed. eg 3501 or 3502
_SSBNonLimited

 if (ATTEMPT_FAST_BOOT)
    move    #>SPEED_MP3ENCODE,x1  ; Change to fastest player proven speed w/ time in the field.
 endif
 if (ATTEMPT_FASTEST_BOOT)
 WARN 'Note: ATTEMPT_FASTEST_BOOT is enabled'
    move    #>SPEED_PEAK,x1       ; DSP's peak speed. boot failure Vbat thresh is higher in high 
                                  ; fDCLK, so configure SPEED_BOOT_BATTERY_CHECK in project.inc
 endif

_setTheBootSpeed
    ; input param in x1
    ;move    x1,y:g_wBootSpeedIndex ; record the selected boot speed here during profiling only.
    jsr     SysSetSpeed           ; return previous speed in A.

    rts    

    
;///////////////////////////////////////////////////////////////////////////////
;>  Name:           SysSpeedSysShutDown
;   Type:           API Function
;   Description:    Should only be called one time at system shutdown. 
;   Inputs:         None
;   Outputs:        None
;   Notes:          C or Asm callable.
;<
;///////////////////////////////////////////////////////////////////////////////
FSysSpeedSysShutDown  ; C entry point
SysSpeedSysShutDown
    move    #>SPEED_MAX,x1               ; Change to SPEED_MAX
    jsr     SysSetSpeed
    rts    
    
;///////////////////////////////////////////////////////////////////////////////
; > Name:       retCode SysSpeedIncrease(int SpeedIndex, int clockUserId)
;
;   Type:       API function
;
;   Description: Sets one of multibit binary semaphore speed client bits and moves  
;               the system to requested CLK freq if faster than current speed. 
;               If client ID is valid, sets client active bit & saves requested 
;               speed in a table enabling SysSpeedClockFree() to conditionally 
;               restore it later.
;
;   Inputs:     C:   SpeedIndex and clockUserId parameter described below.
;                    Module global y:clockUserFlag and y:SavedClientSpeedIndeces
;
;               ASM: 
;               x1 = SpeedIndex value desired such as: 
;                                    SPEED_IDLE, SPEED_MP3, SPEED_MAX, SPEED_WMA
;               y0= System Speed API client identifier constant from sysspeed.inc 
;               y:clockUserFlag module global 
;
;   Outputs:    y:clockUserFlag module global: requesting client's bit set.
;               y:SavedClientSpeedIndeces module global: table entry written with 
;                                                  client's requested SpeedIndex
;
;               Returns resulting speed index in register A1
;
;   Note:       Function helps prevent unwanted speed reductions. C or asm callable.
;               Each caller must have a speed client bit reserved in sysspeed.inc.
;               Must be used with SysSpeedClockFree(). 
;               In all cases, requested speed is still saved and used 
;               until SysSpeedClockFree() is called.
;               Unit & system asm tests on clockUserFlag & master SpeedIndex passed.
; <
;///////////////////////////////////////////////////////////////////////////////
FSysSpeedIncrease
	move    a1,x1                       ; SpeedIndex requested
	move    b1,y0                       ; Speed Client ID

SysSpeedIncrease
;  testpoint1
    ;int retCode = -1; default
    move    y:Const_ffffff,y1	        ; return code in y1 defaults to -1

    ; if  SPEED_CLIENT_HIGHEST_USER (b1) >= client id param (a1), ok, else return error.
    move    #>SPEED_CLIENT_HIGHEST_USER,B
    move    y0,A            ; client id parameter in A1
    cmp     A,B
    jge     _IdMaxPassed
    jmp     _NoFreqChange   ; return with error. Undefined client id parameter.

_IdMaxPassed
    ; API client ID min check
    clr     B
    cmp     A,B     ; if 0 (in B) <= client id param (a1), ok, else return error
    jle     _IdMinPassed
    jmp     _NoFreqChange   ; return with error

_IdMinPassed ; Speed Client ID valid (y0)

    ; Speed Index (x1) verification:
    push    a1                          ; save valid client id param
    push    x1                          ; save desired speed index
    jsr     SysGetSpeed                 
    ; Current SpeedIndex in x1 after SysGetSpeed
    pop     B                           ; Desired speed index in B1
    pop     x0                          ; valid client id param in x0

    move    y:Const_000000,y0 ;y0 will be non zero if desired speed <= current

    move    x1,A                        ; Current SpeedIndex from SysGetSpeed   
    cmp     A,B
    ;if desired (b1) < current (a1), don't increase speed, indicated as y0==2
    jlt     _DesiredBelowCurrent 
    ;if desired (b1) > current (a1), speed us. Indicated as: y0==0
    jgt     _SpeedCheckDone      
    ; desired (b1) == current (a1), don't increase speed, indicate as y0==1
    move    y:Const_000001,y0      
    jmp     _SpeedCheckDone

_DesiredBelowCurrent
    move    y:Const_000002,y0

_SpeedCheckDone
    push    b1            ; Save Desired SpeedIndex. Arg to pass to SysSetSpeed.

    ; Client id (x0) is valid so set that bit in the private multi binary semaphore
    move    y:Const_000001,b
    clr     a
    cmp     x0,a
    jeq     _ShiftsDone         ; Never allow 'rep 0' instruction

    rep     x0            ; client bit id param in x0. Convert bit id to bitmask
     lsl     b1

_ShiftsDone
    move    b1,y1
    ; y1 has mask with only the requesting client's bit set, to set y:clockUserFlag
    pop     x1          ; Restore Desired SpeedIndex arg for call to SysSetSpeed
;  testpoint2
    move    y:clockUserFlag,B
    OR      y1,B           ; Set client's speed client's active bit as requested
    move    b1,y:clockUserFlag

    ; Desired SpeedIndex is in x1. Save copy to allow possible restore later.    <<<Verified SpeedIndex save.
    move    #>SavedClientSpeedIndeces,r0    ; base addr
    move    x0,n0 ; client ID param= table offset to save this client's SpeedIndex
    nop                                     ; Setup pipeline. KEEP.
    move    x1,y:(r0+n0)                    ; Save client's SpeedIndex

    ; If we're already at or above desired speed (indicated by y0) skip the speed call   
    move    y0,a
    tst     a
    jne     _AlreadyThere    ; if y0 != 0, desired speed <= actual so just exit.
    
    jsr     SysSetSpeed                     ; Input Param: SpeedIndex in x1
    ; DCLK speed increased.
    
_AlreadyThere
_NoFreqChange 
    ; Return in A1 the current active SpeedIndex setting
    jsr     SysGetSpeed
    move    x1,A

    rts


;///////////////////////////////////////////////////////////////////////////////
; > Name:       retCode SysSpeedClockFree(int clockUserId)
;
;   Type:       API function
;
;   Description: Clears a clock user's active bit and sets the system DCLK to 
;               lowest active client's saved SpeedIndex or SPEED_IDLE 
;               iff all clock API user bits are inactive. 
;               Used with SysSpeedIncrease().
;
;   Inputs:     C:   clockUserId parameter is a constant from sysspeed.inc. 
;                    Module global y:clockUserFlag and y:SavedClientSpeedIndeces
;
;               ASM: y0= clockUserId constant from sysspeed.inc. 
;                    Module globals y:clockUserFlag and y:SavedClientSpeedIndeces
; 
;   Outputs:    Returns in a1=  Current SpeedIndex from SysGetSpeed
;               y:clockUserFlag module global: client's bit is cleared.
;               y:SavedClientSpeedIndeces module global: read 
;
;   Note:       Function prevents unwanted speed reductions. C or asm callable.
;               Each caller must have a speed client bit reserved in sysspeed.inc.
;               Unit & system tests on clockUserFlag & speed manipulation passed.
; <
;///////////////////////////////////////////////////////////////////////////////
FSysSpeedClockFree
    move    a1,y0

SysSpeedClockFree
;  testpoint1
    ;int retCode = -1; default
    move    y:Const_ffffff,y1	        ; return code in y1 defaults to -1

    ; if  SPEED_CLIENT_HIGHEST_USER (b1) >= client id param (a1), ok, else return error.
    move    #>SPEED_CLIENT_HIGHEST_USER,B
    move    y0,A                        ; client id parameter in A1
    cmp     A,B
    jge     _IdMaxPassed
    jmp     _ExitClockFree    ; return with error. Undefined client id parameter.

_IdMaxPassed
    ; API client ID min check
    clr     B
    cmp     A,B     ; if 0 (in B) <= client id param (a1), ok, else return error
    jle     _IdMinPassed
    jmp     _ExitClockFree    ; return with error. Undefined client id parameter.

_IdMinPassed
    move    a1,x0                     
    
    ; Client id bit (x0) is valid so clear that bit in the private multi binary-semaphore
    move    y:Const_000001,b
    clr     a
    cmp     x0,a
    jeq     _ShiftingDone      ; avoids 'rep 0'
    
    rep     x0                 ; client id param (x0). Convert bit id to bitmask
     lsl     b1

_ShiftingDone
    ; b1 has mask with only the requesting client's bit set. Invert it for a bit clear.
    neg     B               
    move    b1,b0
    dec     B  ; neg op gives 2's comp which is '1's  comp+1'. Sub 1 to get one's comp.
    move    b0,y1
    ; Example after above: xx00010 bitpos becomes xx11101 mask in b1. 

;  testpoint2
    ; use y1 value as mask for AND op to clear the single requested bit in y:clockUserFlag.
    move    y:clockUserFlag,A
    AND     y1,A                    ; Clear client's binary sem bit as requested              
    move    a1,y:clockUserFlag

    ; If no clients, set speed to IDLE and return.
    move    a1,B
    tst	    B
    jne     _ClientActive ;if no clock API user bits set, Reduce DCLK freq to IDLE. 
    ; else slow the system to IDLE
	move    #>SPEED_IDLE,x1	            ; Set DCLK speed back to idle.
	jsr     SysSetSpeed
    move    y:Const_000000,y1           ; Success
    jmp     _ExitClockFree              ; Exit at idle speed
_ClientActive

    ;-------------------
    ; Reduce speed to highest remaining active client's saved SpeedIndex

    ; Find highest active client's SpeedIndex in the SavedClientSpeedIndeces table.
    ; Offset into SpeedIndeces table==bitpos of clockUserFlag's highest active client bit
    move    #>SavedClientSpeedIndeces,r0   
    move    #>SPEED_CLIENT_HIGHEST_USER,n0  ; All of this must remain 0 based. 
    ; Start at highest speedclient bitpos, use first found to be active 

_SearchForHighestBitSet                     ; loop start
    move    #>1,A
    move    n0,y0                           ; next lower bitpos in y0

    move    #0,B                            ; y0 mirrors n0
    cmp     y0,B
    jeq     _NoRep                          ;avoid 'rep 0' since LC maxes
    rep     y0
     asl     A         
_NoRep

    ; a1 has 2^n0. Bitmask for associated bitpos. n0=clientId (0 based)
    move    a1,x1
    move    y:clockUserFlag,B   
    and     x1,B                             ; (2^n0 mask & clockUserFlag) in B  
    cmp     A,B    
    ; First found active client bitpos? If so use bit id in n0 as table offset.
    jeq     _DoneSearchingClientTable 

    move    n0,a                            ; if(bitpos==0) exit without a find
    tst     a
    jeq     _ExitClockFree     

    move    n0,b0                           ; Decrement n0 which is bitposition
    dec     B          
    move    b0,n0 ; This and 2 instr above can be replaced with another way to decrement n0

    jmp     _SearchForHighestBitSet ; again

_DoneSearchingClientTable   ; Only here if we found an active clock client.
    move    y:(r0+n0),x1    ; Restore the Speed of this highest remaining client

;  testpoint3
    ; Check for and trap on invalid saved SpeedIndex (x1).
    move    #>SPEED_LAST,a
    cmp     x1,a                
    jge     _ValidCheckPassed
    ; Invalid speed index value
    error  ; RETAIL build could skip the SysSetSpeed call on invalid speed index 
           ; by using debugOrContinue macro followed by jmp _ExitClockFree.
    nop
_ValidCheckPassed
    
    jsr     SysSetSpeed     ; Input Param: requested SpeedIndex in x1
    ; DCLK speed reduced to highest remaining client's SpeedIndex
    ;-------------------
    
_ExitClockFree
    ; Return in A1 the current SpeedIndex setting
    jsr     SysGetSpeed
    move    x1,A
    rts

;///////////////////////////////////////////////////////////////////////////////
; Name:    SysSpeedGetClockUserFlag 
; Type:    Final client helper function 
; Purpose: Used only by the Executive. Indicates if other clock API clients remain.
; Input:   Reads global y:clockUserFlag
; Output:  a1 returns 0 if there are zero non-Executive speed clients remaining. 
;          a1 returns non-0 if non-Exec speed clients remain active.
; Example: Exec caller tests return for 0:  tst a    jeq or jne
;///////////////////////////////////////////////////////////////////////////////
FSysSpeedGetClockUserFlag    
SysSpeedGetClockUserFlag    
    move    y:clockUserFlag,a
    move    #>$FFFFFF,y0
    bclr    #>SPEED_CLIENT_EXECUTIVE,y0
    and     y0,a    
    rts

    endsec
        
