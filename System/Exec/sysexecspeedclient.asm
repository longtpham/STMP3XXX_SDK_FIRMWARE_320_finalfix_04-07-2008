;///////////////////////////////////////////////////////////////////////////////
;
;  File        : SysExecSpeedClient.asm
;  Description : Conditionally Increase or free the shared clock speed and 
;                notify menus on btn event for the Executive
;
;///////////////////////////////////////////////////////////////////////////////

    section     SYSFUN_SysExecSpeedClient

    opt     mex

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "sysequ.inc"
    include "sysspeed.inc"
    include "hwequ.inc"
    include "buttons.inc"    
    include "msgequ.inc"    
    include "systimers.inc"  
    list

    page    132,60,0,0,0
    

;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////
    global    SetExecState
    global    ExecSpeedAndBtnCheck
    global    FDisableExecSpeedClient ;optional disable call used by FM menu
    global    DisableExecSpeedClient
    global    FReEnableExecSpeedClient ;Only used to undo DisableExecSpeedClient 
    global    ReEnableExecSpeedClient

    nolist
    include "button.xref"
    list

    extern    SysSpeedResetAPI
    extern    SysSpeedIncrease
    extern    SysSpeedClockFree
    extern    SysSpeedGetClockUserFlag
    extern    SysCheckTime
    extern    SignalModule
    extern    MODULE_NUM_MENU

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////
    nolist
    include "systime.xref"
    list
;///////////////////////////////////////////////////////////////////////////////
;   Y Memory
;///////////////////////////////////////////////////////////////////////////////    
      org     y,"SYSFUN_SysExecSpeedClient_Y":
    
g_iIdleTargetTimeHigh   dc  0       ; Units: msec
g_iIdleTargetTimeLow    dc  0
BTN_ACTIVE_TIME_MS      equ 10000   ; On transition from button Q empty to !empty, 
                                    ; don't request IDLE speed until interval passes
ExecStateBits           dc  0
BUTTON_PENDING           equ    0      ; In ExecStateBits
IDLE_TIMER_ACTIVE        equ    1
NON_EXEC_SPEED_CLIENTS   equ    2
EXEC_SPEED_CLIENT_DISABLED equ  3

;*******************************************************************************
;  P memory
;*******************************************************************************
      org     p,"SYSFUN_SysExecSpeedClient_P":
      
;///////////////////////////////////////////////////////////////////////////////
;   Name:           SetExecState
;
;   Type:           Internal Function
;
;   Description:    Prepare bitflags in ExecStateBits 
;                   BUTTON_PENDING and NON_EXEC_SPEED_CLIENTS remaining indicator
;
;   Inputs:         No register inputs. Uses Global y:ExecStateBits
;
;   Outputs:        None. Updates y:ExecStateBits
;
;   Notes:         
;///////////////////////////////////////////////////////////////////////////////
SetExecState
    ; Prepare bitflags in ExecStateBits for call to ExecSpeedAndBtnCheck
    jsr     SysCheckForButton
    jcc     _NoBtn
    move    y:ExecStateBits,a1
    bset    #BUTTON_PENDING,a1
    jmp     _BtnIndicatorReady
_NoBtn
    move    y:ExecStateBits,a1
    bclr    #BUTTON_PENDING,a1
_BtnIndicatorReady    
    move    a1,y:ExecStateBits

    clr     a
    jsr     SysSpeedGetClockUserFlag  ; Indicate any non-Executive speed clients
    tst     a
    jeq     _NoClients
    move    y:ExecStateBits,a1    
    bset    #NON_EXEC_SPEED_CLIENTS,a1
    jmp     _ClientsIndicatorReady
_NoClients
    move    y:ExecStateBits,a1   
    bclr    #NON_EXEC_SPEED_CLIENTS,a1
_ClientsIndicatorReady
    move    a1,y:ExecStateBits

    rts

;///////////////////////////////////////////////////////////////////////////////
; >
; Name:     _reentrant void DisableExecSpeedClient(void);
; Type:     API function. C or asm callable
; Purpose:  Optional disable call initially used by FM menu. 
;           ReEnableExecSpeedClient should be used also.
; Inputs:   None
; Output:   None
; Globals:  Sets bit y:ExecStateBits.EXEC_SPEED_CLIENT_DISABLED
; <
;///////////////////////////////////////////////////////////////////////////////
FDisableExecSpeedClient
DisableExecSpeedClient
    bset    #EXEC_SPEED_CLIENT_DISABLED,y:ExecStateBits
    ; Clear Exec's bin semaphore via API for return to IDLE clock speed
    move    #>SPEED_CLIENT_EXECUTIVE,y0
    jsr     SysSpeedClockFree    
    rts
    
;///////////////////////////////////////////////////////////////////////////////
; >
; Name:     _reentrant void ReEnableExecSpeedClient(void);
; Type:     API function. C or asm callable
; Purpose:  Only needed when undoing the DisableExecSpeedClient call.
; Inputs:   None
; Output:   None
; Globals:  Clears bit y:ExecStateBits.EXEC_SPEED_CLIENT_DISABLED
; <
;///////////////////////////////////////////////////////////////////////////////
FReEnableExecSpeedClient
ReEnableExecSpeedClient
    bclr    #EXEC_SPEED_CLIENT_DISABLED,y:ExecStateBits
    rts

    
;///////////////////////////////////////////////////////////////////////////////
; Name:    ExecSpeedAndBtnCheck
;
; Purpose: Conditionally free DCLOCK & notify menus on btn event for Executive
;
; Input:   y:ExecStateBits BUTTON_PENDING & NON_EXEC_SPEED_CLIENTS are initialized
;          g_iIdleTargetTimeHigh/Low 
;
; Outputs: Updates flags in y:ExecStateBits, signals menu module if button pending
;          g_iIdleTargetTimeHigh/Low read and may be written
;
; Notes:   To be used only by the Executive process.
;
;    // Called from Exec loop. Needs periodic execution, button event detection. 
;    // C/psuedo conceptual model: Asm hand compilation is derived from this logic.
;
;    // Set buttonPendingFlag and nonExecClients flag
;
;    if ( (nonExecClientCount != 0) || buttonPendingFlag )
;        idleTimerActive = FALSE;				
;
;    if( (idleTimerActive==FALSE)&& ((nonExecClients==0) && (buttonPendingFlag==0)) )
;    {	idleTimerActive = TRUE;
;	    IdleTargetTime = t+dt;  } // all in ms
;
;    if( buttonPendingFlag )
;    {  // Assumes SysSetSpeed returns only after a safe settling time expires.
;       // Assumption allows use case: play button increase to Exec speed, WMA  
;       //                             decode increases DCLK again quickly.
;       // If assumption is ever seen to fail, 
;       //    Alt: if still idle 1 sec after button, increase.
;       if( y:ExecStateBits.EXEC_SPEED_CLIENT_DISABLED == 0 )
;       {   
;           #pragma asm
;           ; Does not change speed if requested = current
;           jsr     SysSpeedIncrease(SPEED_IDLE+1,SPEED_CLIENT_EXECUTIVE) 
;           #pragma endasm   
;       }
;       #pragma asm    
;    _signal_menus
;        ; signal the menu module that button event occurred
;        move    #>MODULE_NUM_MENU,x1
;        move    #>EVENT_BUTTON,x0
;        jsr     SignalModule
;        #pragma endasm   
;    }
;    if( idleTimerActive )
;        if (target_time_reached) 
;        {   IdleTimerActive = FALSE;
;            SysSpeedClockFree(SPEED_CLIENT_EXECUTIVE);
;        }
;    return;
;///////////////////////////////////////////////////////////////////////////////
ExecSpeedAndBtnCheck
;    if (y:ExecStateBits.NON_EXEC_SPEED_CLIENTS ||y:ExecStateBits.BUTTON_PENDING)
;        idleTimerActive = FALSE;	

    move    y:ExecStateBits,a
    btst    #NON_EXEC_SPEED_CLIENTS,a1
    jcs     _IdleTimerNotActive
    btst    #BUTTON_PENDING,a1
    jcs     _IdleTimerNotActive
    jmp     _IdleTimerIndicated
_IdleTimerNotActive
    bclr    #IDLE_TIMER_ACTIVE,a1
_IdleTimerIndicated
    move    a1,y:ExecStateBits      

;    if( (idleTimerActive==FALSE)&& ((nonExecClientCount==0) && (buttonPendingFlag==0)) )
;		idleTimerActive = TRUE;
;		IdleTargetTime = t+dt_ms;

    btst    #IDLE_TIMER_ACTIVE,a1
    jcs     _NoSetTargetTime
    btst    #NON_EXEC_SPEED_CLIENTS,a1
    jcs     _NoSetTargetTime
    btst    #BUTTON_PENDING,a1
    jcs     _NoSetTargetTime    
;       Set Up Target Time
;		idleTimerActive = TRUE;
;		IdleTargetTime = t+dt   in ms
    bset    #IDLE_TIMER_ACTIVE,a1
    move    a1,y:ExecStateBits

    jsr     SysGetCurrentTime; save time (ms) of this btn Q empty to !empty event
    clr     B
    move    #>BTN_ACTIVE_TIME_MS,b0  
    add     B,A                     ; save time marking end of high speed mode.
    move    a1,y:g_iIdleTargetTimeHigh
    move    a0,y:g_iIdleTargetTimeLow
_NoSetTargetTime

;   if ( buttonPending )
;   {    if( y:ExecStateBits.EXEC_SPEED_CLIENT_DISABLED == 0 )
;        {  SysSpeedIncrease(MP3_SPEED,SPEED_CLIENT_EXECUTIVE);
;        }
;        SignalModule BUTTON_PENDING   
;   }
    move    y:ExecStateBits,a1
    btst    #BUTTON_PENDING,a1
    jcc     _NoButtonsPending

    btst    #EXEC_SPEED_CLIENT_DISABLED,a1
    jcs     _signal_menus
    ; Conditionally increase DCLK frequency for quick button response and hang avoidance.
    ; Falls back to IDLE only when all SysSpeed API managed binary semaphores are free. 
    ; Note that SysSpeedIncrease returns immediately if requested==current.
;  testpoint
    ; Exec's SpeedIndex must always be IDLE or IDLE+1, else speed reductions would
    ; fail while the Exec speed client holds the clock high.
    move    #>SPEED_IDLE+1,x1 
    move    #>SPEED_CLIENT_EXECUTIVE,y0    
    jsr     SysSpeedIncrease

_signal_menus              ; signal the menu module that a button event occurred
    move    #>MODULE_NUM_MENU,x1
    move    #>EVENT_BUTTON,x0
    jsr     SignalModule
_NoButtonsPending

;    if( idleTimerActive )
;        if (target time reached) 
;        {   IdleTimerActive=FALSE
;            SysSpeedClockFree(SPEED_CLIENT_EXECUTIVE);   }
    move    y:ExecStateBits,a1
    btst    #IDLE_TIMER_ACTIVE,a1
    jcc     _EndTimerCheck

     move    y:g_iIdleTargetTimeHigh,x1 ;system uptime at which to move to idle (ms)
     move    y:g_iIdleTargetTimeLow,x0             
     jsr     SysCheckTime                ; Is it time to request idle yet?
     jclr    #HW_SR_C_BITPOS,sr,_EndTimerCheck

      ; We've reached target uptime (ms) for the Executive to request idle
      move    y:ExecStateBits,a1
      bclr    #IDLE_TIMER_ACTIVE,a1
      move    a1,y:ExecStateBits    

;  testpoint
      ; Clear Exec's bin semaphore via API for return to IDLE clock speed
      move    #>SPEED_CLIENT_EXECUTIVE,y0
      jsr     SysSpeedClockFree

_EndTimerCheck
    rts


    endsec
    
