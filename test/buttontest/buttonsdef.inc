;//////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2001
;
; File          :   ButtonsDef.inc
; Description   :   This file contains the button physical map and event map
;                   descriptor.
;                   
;//////////////////////////////////////////////////////////////////////////////
    include "project.inc"
    include "sysmacro.asm"

    IF (!@def(BUTTONSDEFS_INC))
BUTTONSDEFS_INC equ 1

    ;*****************************************************************************************
    ;* Scan Buttons (Matrix) Description
    ;*****************************************************************************************

    ;Scan Button Row Descriptor 
    BUTTON_ROWS:            equ     3               ;Numer of rows in matrix          
    DEFINE_ROW 0,GP2,1
    DEFINE_ROW 1,GP1,19
    DEFINE_ROW 2,GP2,2

    ;Scan Button Column Descriptor 
    BUTTON_COLUMNS:         equ     4               ;Number of columns in matrix
    DEFINE_COLUMN 0,GP2,0
    DEFINE_COLUMN 1,GP1,23
    DEFINE_COLUMN 2,GP1,22
    DEFINE_COLUMN 3,GP1,18

 
    ;Attaches a unique number to each scan button
    ;Unused buttons are set to $000000
    DEFINE_SCAN_BUTTON_EVENT COL0,ROW0,$000002,STOP_BUTTON
    DEFINE_SCAN_BUTTON_EVENT COL0,ROW1,$000010,PREVIOUS_BUTTON
    DEFINE_SCAN_BUTTON_EVENT COL0,ROW2,$000080,MENU_BUTTON
    DEFINE_SCAN_BUTTON_EVENT COL1,ROW0,$000008,RECORD_BUTTON
    DEFINE_SCAN_BUTTON_EVENT COL1,ROW1,$000020,NEXT_BUTTON
    DEFINE_SCAN_BUTTON_EVENT COL1,ROW2,$000100,VOLUME_DOWN_BUTTON
    DEFINE_SCAN_BUTTON_EVENT COL2,ROW0,$001000,EQ_BUTTON
    DEFINE_SCAN_BUTTON_EVENT COL2,ROW1,$000400,ERASE_BUTTON
    DEFINE_SCAN_BUTTON_EVENT COL2,ROW2,$000200,VOLUME_UP_BUTTON
    DEFINE_SCAN_BUTTON_EVENT COL3,ROW0,$002000,SHUFFLE_BUTTON
    DEFINE_SCAN_BUTTON_EVENT COL3,ROW1,$000800,AB_BUTTON
    DEFINE_SCAN_BUTTON_EVENT COL3,ROW2,$000000,UNUSED_BUTTON      ;Unused button



    ;*****************************************************************************************
    ;* NonScan Buttons Description
    ;*****************************************************************************************

    ;Non Scan Button Descriptor 
    BUTTON_NON_SCAN_BUTTONS: equ    2               ;Number of non scan buttons
    DEFINE_NON_SCAN_BUTTON 0,GP1,10
    DEFINE_NON_SCAN_BUTTON 1,GP1,11

    ;Attaches a unique number to each non scan button
    DEFINE_NON_SCAN_BUTTON_EVENT 0,$000004,PLAY_BUTTON
    DEFINE_NON_SCAN_BUTTON_EVENT 1,$000040,HOLD_BUTTON



    ;*****************************************************************************************
    ;* LRADC "Buttons" Description
    ;*****************************************************************************************
    ; UNCOMMENT THE NEXT LINE TO ENABLE REMOTE BUTTON PROCESSING -- UPDATE THE TOTAL # BUTTON
	if (@def('REMOTE1')||@def('REMOTE2'))
		define  BUTTON_LRADC_BUTTONS  '6'       ; Number of LRADC "buttons" (voltage levels)
	endif
    ; The LRADC button definitions below assume that when no button is pressed, the AUX
    ; input goes HIGH, above the limit voltage of the last button listed.

    ; To set a minimum voltage, below which we will treat it as "no button pressed", 
    ; add a dummy button at the beginning of the table and in the LRADC_BUTTON_EVENT entries below.
    ; Set the event's second argument to zero, to indicate no button pressed.

    DEFINE_LRADC_BUTTON     0,438                   ; Upper limit voltage (mV) for button 0
    DEFINE_LRADC_BUTTON     1,902                   ; Upper limit voltage (mV) for button 1
    DEFINE_LRADC_BUTTON     2,1417                  ; Upper limit voltage (mV) for button 2
    DEFINE_LRADC_BUTTON     3,1959                  ; etc.  - MUST BE INCREASING!
    DEFINE_LRADC_BUTTON     4,2397
    DEFINE_LRADC_BUTTON     5,2964
    ; Levels above this will be treated as "no button pressed".  To remove the upper limit, just
    ; set the last button's upper limit above what the LRADC can read (e.g. >3.3V for VDDIO).

	if (@def('REMOTE2'))	; Map remote control key in the separate keys as key pad
		DEFINE_LRADC_BUTTON_EVENT      0,UNUSED_BUTTON,REMOTE_UNUSED_BUTTON             ; Unused (Headphone plug-in detect)
		DEFINE_LRADC_BUTTON_EVENT      1,27,REMOTE_PLAY_BUTTON					; Play (same code as PLAY_BUTTON)
		DEFINE_LRADC_BUTTON_EVENT      3,28,REMOTE_RWD_BUTTON					; RWD
		DEFINE_LRADC_BUTTON_EVENT      2,29,REMOTE_FF_BUTTON					; FF
		DEFINE_LRADC_BUTTON_EVENT      4,30,REMOTE_VOLUME_UP_BUTTON				; V+
		DEFINE_LRADC_BUTTON_EVENT      5,31,REMOTE_VOLUME_DOWN_BUTTON			; V-
	else					; Map remote control key in the same key as keys pad, or
							; no remote control buttons
		DEFINE_LRADC_BUTTON_EVENT      0,UNUSED_BUTTON,REMOTE_UNUSED_BUTTON             ; Unused (Headphone plug-in detect)
		DEFINE_LRADC_BUTTON_EVENT      1,PLAY_BUTTON,REMOTE_PLAY_BUTTON                 ; Play (same code as PLAY_BUTTON)
		DEFINE_LRADC_BUTTON_EVENT      2,NEXT_BUTTON,REMOTE_FF_BUTTON                   ; FF
		DEFINE_LRADC_BUTTON_EVENT      3,PREVIOUS_BUTTON,REMOTE_RWD_BUTTON              ; RWD
		DEFINE_LRADC_BUTTON_EVENT      4,VOLUME_UP_BUTTON,REMOTE_VOLUME_UP_BUTTON       ; V+
		DEFINE_LRADC_BUTTON_EVENT      5,VOLUME_DOWN_BUTTON,REMOTE_VOLUME_DOWN_BUTTON   ; V-
	endif
	

    ;*****************************************************************************************
    ;* Hold Button Description
    ;*****************************************************************************************
    HOLD_BUTTON_EVENT:      equ     BUTTON_EVENT_NONSCAN1



	if (@def('REMOTE2'))
		BUTTON_MAPPED_EVENTS:   equ     39              ;Number of mapped events with remote control
	else
		BUTTON_MAPPED_EVENTS:   equ     29              ;Number of mapped events no remote control
	endif

	;Menu
		DEFINE_MAPPED_EVENT  0,PRESS_RELEASE,MENU_BUTTON
		DEFINE_MAPPED_EVENT  14,PRESS_HOLD,MENU_BUTTON
	;vol -
		DEFINE_MAPPED_EVENT  1,PRESS_RELEASE,VOLUME_DOWN_BUTTON
		DEFINE_MAPPED_EVENT  15,PRESS_HOLD,VOLUME_DOWN_BUTTON
	;vol +
		DEFINE_MAPPED_EVENT  2,PRESS_RELEASE,VOLUME_UP_BUTTON
		DEFINE_MAPPED_EVENT  16,PRESS_HOLD,VOLUME_UP_BUTTON
	;unused button
		DEFINE_MAPPED_EVENT  3,PRESS_RELEASE,UNUSED_BUTTON
		DEFINE_MAPPED_EVENT  17,PRESS_HOLD,UNUSED_BUTTON	
	;previous
		DEFINE_MAPPED_EVENT  4,PRESS_RELEASE,PREVIOUS_BUTTON
		DEFINE_MAPPED_EVENT  18,PRESS_HOLD,PREVIOUS_BUTTON
	;next
		DEFINE_MAPPED_EVENT  5,PRESS_RELEASE,NEXT_BUTTON
		DEFINE_MAPPED_EVENT  19,PRESS_HOLD,NEXT_BUTTON
	;Erase
		DEFINE_MAPPED_EVENT  6,PRESS_RELEASE,ERASE_BUTTON
		DEFINE_MAPPED_EVENT  20,PRESS_HOLD,ERASE_BUTTON
	;AB
		DEFINE_MAPPED_EVENT  7,PRESS_RELEASE,AB_BUTTON
		DEFINE_MAPPED_EVENT  21,PRESS_HOLD,AB_BUTTON
	;stop
		DEFINE_MAPPED_EVENT  8,PRESS_RELEASE,STOP_BUTTON
		DEFINE_MAPPED_EVENT  22,PRESS_HOLD,STOP_BUTTON
	;record
		DEFINE_MAPPED_EVENT  9,PRESS_RELEASE,RECORD_BUTTON
		DEFINE_MAPPED_EVENT  23,PRESS_HOLD,RECORD_BUTTON
	;EQ
		DEFINE_MAPPED_EVENT  10,PRESS_RELEASE,EQ_BUTTON
		DEFINE_MAPPED_EVENT  24,PRESS_HOLD,EQ_BUTTON
	;play mode
		DEFINE_MAPPED_EVENT  11,PRESS_RELEASE,SHUFFLE_BUTTON
		DEFINE_MAPPED_EVENT  25,PRESS_HOLD,SHUFFLE_BUTTON
	;play
		DEFINE_MAPPED_EVENT  12,PRESS_RELEASE,PLAY_BUTTON
		DEFINE_MAPPED_EVENT  26,PRESS_HOLD,PLAY_BUTTON
	;hold
		DEFINE_MAPPED_EVENT  13,PRESS_RELEASE,HOLD_BUTTON
		DEFINE_MAPPED_EVENT  27,PRESS_HOLD,HOLD_BUTTON

	if (@def('REMOTE2')) ; Map separately
		;example REMOTE BUTTONS
		;remote play
			DEFINE_MAPPED_EVENT  28,PRESS_RELEASE,REMOTE_PLAY_BUTTON
			DEFINE_MAPPED_EVENT  33,PRESS_HOLD,REMOTE_PLAY_BUTTON
		;remote next
			DEFINE_MAPPED_EVENT  29,PRESS_RELEASE,REMOTE_FF_BUTTON
			DEFINE_MAPPED_EVENT  34,PRESS_HOLD,REMOTE_FF_BUTTON
		;remote previous
			DEFINE_MAPPED_EVENT  30,PRESS_RELEASE,REMOTE_RWD_BUTTON
			DEFINE_MAPPED_EVENT  35,PRESS_HOLD,REMOTE_RWD_BUTTON

		;remote vol +
			DEFINE_MAPPED_EVENT  31,PRESS_RELEASE,REMOTE_VOLUME_UP_BUTTON
			DEFINE_MAPPED_EVENT  36,PRESS_HOLD,REMOTE_VOLUME_UP_BUTTON
		;remote vol -
			DEFINE_MAPPED_EVENT  32,PRESS_RELEASE,REMOTE_VOLUME_DOWN_BUTTON
			DEFINE_MAPPED_EVENT  37,PRESS_HOLD,REMOTE_VOLUME_DOWN_BUTTON
	
		;P_H Vol+ P_R Vol-    
    			DEFINE_2BUTTON_MAPPED_EVENT	 38,PRESS_HOLD,15,16
	else
		;P_H Vol+ P_R Vol-    
    			DEFINE_2BUTTON_MAPPED_EVENT	 28,PRESS_HOLD,15,16
	endif
 
 endif ;(!@def(BUTTONSDEF_INC))

