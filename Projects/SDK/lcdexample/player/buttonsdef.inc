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
    BUTTON_ROWS:            equ     1               ;Numer of rows in matrix          
    DEFINE_ROW 0,GP2,2		;GPIO50
     
    ;Scan Button Column Descriptor 
    BUTTON_COLUMNS:         equ     4               ;Number of columns in matrix
    DEFINE_COLUMN 0,GP2,0	;GPIO48
    DEFINE_COLUMN 1,GP1,23	;GPIO47
    DEFINE_COLUMN 2,GP1,22	;GPIO46
    DEFINE_COLUMN 3,GP2,1	;GPIO49

     ;Attaches a unique number to each scan button
    ;Unused buttons are set to $000000
    DEFINE_SCAN_BUTTON_EVENT COL0,ROW0,$000002,MENU_BUTTON
    ;DEFINE_SCAN_BUTTON_EVENT COL0,ROW1,$000010,PREVIOUS_BUTTON
    ;DEFINE_SCAN_BUTTON_EVENT COL0,ROW2,$000080,MENU_BUTTON
    DEFINE_SCAN_BUTTON_EVENT COL1,ROW0,$000008,RV_BUTTON
    ;DEFINE_SCAN_BUTTON_EVENT COL1,ROW1,$000020,NEXT_BUTTON
    ;DEFINE_SCAN_BUTTON_EVENT COL1,ROW2,$000100,VOLUME_DOWN_BUTTON
    DEFINE_SCAN_BUTTON_EVENT COL2,ROW0,$001000,NEXT_BUTTON
    ;DEFINE_SCAN_BUTTON_EVENT COL2,ROW1,$000400,ERASE_BUTTON
    ;DEFINE_SCAN_BUTTON_EVENT COL2,ROW2,$000200,VOLUME_UP_BUTTON
    DEFINE_SCAN_BUTTON_EVENT COL3,ROW0,$002000,PREVIOUS_BUTTON
    ;DEFINE_SCAN_BUTTON_EVENT COL3,ROW1,$000800,AB_BUTTON
    ;DEFINE_SCAN_BUTTON_EVENT COL3,ROW2,$000000,UNUSED_BUTTON      ;Unused button



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
    ;* Hold Button Description
    ;*****************************************************************************************
    HOLD_BUTTON_EVENT:      equ     BUTTON_EVENT_NONSCAN1

	
	BUTTON_MAPPED_EVENTS:   equ     14              ;Number of mapped events
	
;previous
    DEFINE_MAPPED_EVENT  0,PRESS_RELEASE,PREVIOUS_BUTTON
    DEFINE_MAPPED_EVENT  1,PRESS_HOLD,PREVIOUS_BUTTON
;next
    DEFINE_MAPPED_EVENT  2,PRESS_RELEASE,NEXT_BUTTON
    DEFINE_MAPPED_EVENT  3,PRESS_HOLD,NEXT_BUTTON
;Menu
    DEFINE_MAPPED_EVENT 4,PRESS_RELEASE,MENU_BUTTON
    DEFINE_MAPPED_EVENT 5,PRESS_HOLD,MENU_BUTTON
;RV
    DEFINE_MAPPED_EVENT 6,PRESS_RELEASE,RV_BUTTON
    DEFINE_MAPPED_EVENT 7,PRESS_HOLD,RV_BUTTON
    
;play
    DEFINE_MAPPED_EVENT  8,PRESS_RELEASE,PLAY_BUTTON
    DEFINE_MAPPED_EVENT 9,PRESS_HOLD,PLAY_BUTTON
;hold
    DEFINE_MAPPED_EVENT  10,PRESS_RELEASE,HOLD_BUTTON
    DEFINE_MAPPED_EVENT  11,PRESS_HOLD,HOLD_BUTTON
    
  DEFINE_2BUTTON_MAPPED_EVENT	 12,PRESS_HOLD,1,7
   DEFINE_2BUTTON_MAPPED_EVENT	 13,PRESS_HOLD,3,7

    

	
    endif ;(!@def(BUTTONSDEF_INC))

