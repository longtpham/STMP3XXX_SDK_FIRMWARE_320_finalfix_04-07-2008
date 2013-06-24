;//////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2001
;
; File          :   ButtonsDef.inc
; Description   :   This file contains the button physical map and event map
;                   descriptor.
;                   
;//////////////////////////////////////////////////////////////////////////////
    include "sysmacro.asm"

    IF (!@def(BUTTONSDEFS_INC))
BUTTONSDEFS_INC equ 1   

    ;*****************************************************************************************
    ;* Scan Buttons (Matrix) Description
    ;*****************************************************************************************

    ;Scan Button Row Descriptor 
    BUTTON_ROWS:            equ     3               ;Number of rows in matrix          
    DEFINE_ROW 0,GP2,1				    ;Macro calls from sysmacro.asm
    DEFINE_ROW 1,GP1,19
    DEFINE_ROW 2,GP2,2

    ;Scan Button Column Descriptor 
    BUTTON_COLUMNS:         equ     4               ;Number of columns in matrix
    DEFINE_COLUMN 0,GP2,0			    ;macro calls from sysmacro.asm
    DEFINE_COLUMN 1,GP1,23
    DEFINE_COLUMN 2,GP1,22
    DEFINE_COLUMN 3,GP1,18

    ;Attaches a unique number to each scan button
    ;Unused buttons are set to $000000
    DEFINE_SCAN_BUTTON_EVENT COL0,ROW0,$000002,STOP_BUTTON     ;argh macros
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
    DEFINE_SCAN_BUTTON_EVENT COL3,ROW2,$010000,UNUSED_BUTTON      ;Unused button
  


    ;*****************************************************************************************
    ;* NonScan Buttons Description
    ;*****************************************************************************************

    ;Non Scan Button Descriptor 		    ;play and hold
    BUTTON_NON_SCAN_BUTTONS: equ    2               ;Number of non scan buttons
    DEFINE_NON_SCAN_BUTTON 0,GP1,10
    DEFINE_NON_SCAN_BUTTON 1,GP1,11

    ;Attaches a unique number to each non scan button
    DEFINE_NON_SCAN_BUTTON_EVENT 0,$000004,PLAY_BUTTON
    DEFINE_NON_SCAN_BUTTON_EVENT 1,$000040,HOLD_BUTTON


    ;*****************************************************************************************
    ;* Hold Button Description
    ;*****************************************************************************************
    HOLD_BUTTON_EVENT:      equ     BUTTON_EVENT_NONSCAN1      ;special hold event



    BUTTON_MAPPED_EVENTS:   equ     25              ;Number of mapped events
;UNUSED_BUTTON
    DEFINE_MAPPED_EVENT  3,PRESS_RELEASE,UNUSED_BUTTON
    DEFINE_MAPPED_EVENT  15,PRESS_HOLD,UNUSED_BUTTON
;previous
    DEFINE_MAPPED_EVENT  4,PRESS_RELEASE,PREVIOUS_BUTTON
    DEFINE_MAPPED_EVENT  16,PRESS_HOLD,PREVIOUS_BUTTON
;next
    DEFINE_MAPPED_EVENT  5,PRESS_RELEASE,NEXT_BUTTON
    DEFINE_MAPPED_EVENT  17,PRESS_HOLD,NEXT_BUTTON
;play
    DEFINE_MAPPED_EVENT  27,PRESS_RELEASE,PLAY_BUTTON
    DEFINE_MAPPED_EVENT  28,PRESS_HOLD,PLAY_BUTTON
;play mode
    DEFINE_MAPPED_EVENT  11,PRESS_RELEASE,SHUFFLE_BUTTON
    DEFINE_MAPPED_EVENT  23,PRESS_HOLD,SHUFFLE_BUTTON
;stop
    DEFINE_MAPPED_EVENT  8,PRESS_RELEASE,STOP_BUTTON
    DEFINE_MAPPED_EVENT  20,PRESS_HOLD,STOP_BUTTON
;hold
    DEFINE_MAPPED_EVENT  25,PRESS_RELEASE,HOLD_BUTTON
    DEFINE_MAPPED_EVENT  26,PRESS_HOLD,HOLD_BUTTON
;record
    DEFINE_MAPPED_EVENT 9,PRESS_RELEASE,RECORD_BUTTON
    DEFINE_MAPPED_EVENT 21,PRESS_HOLD,RECORD_BUTTON
;vol +
    DEFINE_MAPPED_EVENT 2,PRESS_RELEASE,VOLUME_UP_BUTTON
    DEFINE_MAPPED_EVENT 14,PRESS_HOLD,VOLUME_UP_BUTTON
;vol -
    DEFINE_MAPPED_EVENT 1,PRESS_RELEASE,VOLUME_DOWN_BUTTON
    DEFINE_MAPPED_EVENT 13,PRESS_HOLD,VOLUME_DOWN_BUTTON
;AB
    DEFINE_MAPPED_EVENT 7,PRESS_RELEASE,AB_BUTTON
    DEFINE_MAPPED_EVENT 19,PRESS_HOLD,AB_BUTTON
;Erase
    DEFINE_MAPPED_EVENT 6,PRESS_RELEASE,ERASE_BUTTON
    DEFINE_MAPPED_EVENT 18,PRESS_HOLD,ERASE_BUTTON
;EQ
    DEFINE_MAPPED_EVENT 10,PRESS_RELEASE,EQ_BUTTON
    DEFINE_MAPPED_EVENT 22,PRESS_HOLD,EQ_BUTTON
;Menu
    DEFINE_MAPPED_EVENT 0,PRESS_RELEASE,MENU_BUTTON
    DEFINE_MAPPED_EVENT 12,PRESS_HOLD,MENU_BUTTON
;P_H Vol+ P_R Vol-    
    DEFINE_2BUTTON_MAPPED_EVENT	 24,PRESS_HOLD,13,14


    endif ;(!@def(BUTTONSDEF_INC))

