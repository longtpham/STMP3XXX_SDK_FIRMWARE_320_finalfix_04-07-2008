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
    BUTTON_ROWS:            equ     3               ;Numer of rows in matrix          
    DEFINE_ROW 0,GP2,1
    DEFINE_ROW 1,GP1,19
    DEFINE_ROW 2,GP2,2

    ;Scan Button Column Descriptor 
    BUTTON_COLUMNS:         equ     3               ;Number of columns in matrix
    DEFINE_COLUMN 0,GP2,0
    DEFINE_COLUMN 1,GP1,23
    DEFINE_COLUMN 2,GP1,22
    DEFINE_COLUMN 3,GP1,18

    ;Attaches a unique number to each scan button
    ;Unused buttons are set to $000000
    DEFINE_SCAN_BUTTON_EVENT COL0,ROW0,$000020,DOWN_BUTTON      ;(S10)    (STOP)
    DEFINE_SCAN_BUTTON_EVENT COL0,ROW1,$000040,SEEK_DOWN_BUTTON        ;(SW7)    (EQ)
    DEFINE_SCAN_BUTTON_EVENT COL0,ROW2,$008000,UNUSED_BUTTON       ;(S4 )    (REC)
    DEFINE_SCAN_BUTTON_EVENT COL1,ROW0,$000100,UP_BUTTON      ;(S11)    (PGDN)
    DEFINE_SCAN_BUTTON_EVENT COL1,ROW1,$000200,SEEKUP_BUTTON        ;(S8)     (RW)
    DEFINE_SCAN_BUTTON_EVENT COL1,ROW2,$010000,VOLDN_BUTTON     ;(S5 )    (VOL-)
    DEFINE_SCAN_BUTTON_EVENT COL2,ROW0,$000001,PGUP_BUTTON      ;(S12)    (PAGEUP)
    DEFINE_SCAN_BUTTON_EVENT COL2,ROW1,$000002,FF_BUTTON        ;(S9)     (FF)
    DEFINE_SCAN_BUTTON_EVENT COL2,ROW2,$004000,VOLUP_BUTTON     ;(S6 )    (VOL+)



    ;*****************************************************************************************
    ;* NonScan Buttons Description
    ;*****************************************************************************************

    ;Non Scan Button Descriptor 
    BUTTON_NON_SCAN_BUTTONS: equ    2               ;Number of non scan buttons
    DEFINE_NON_SCAN_BUTTON 0,GP1,10
    DEFINE_NON_SCAN_BUTTON 1,GP1,11

    ;Attaches a unique number to each non scan button
    DEFINE_NON_SCAN_BUTTON_EVENT 0,$000004,PLAY_BUTTON
    DEFINE_NON_SCAN_BUTTON_EVENT 1,$002000,HOLD_BUTTON


    ;*****************************************************************************************
    ;* Hold Button Description
    ;*****************************************************************************************
    HOLD_BUTTON_EVENT:      equ     HOLD_BUTTON



    BUTTON_MAPPED_EVENTS:   equ     14              ;Number of mapped events
;page down
    DEFINE_MAPPED_EVENT  0,PRESS_RELEASE,PLAY_BUTTON
;    DEFINE_MAPPED_EVENT  1,PRESS_HOLD,PLAY_BUTTON
    DEFINE_MAPPED_EVENT  1,PRESS_RELEASE,VOLUP_BUTTON
;vox
    DEFINE_MAPPED_EVENT  2,PRESS_RELEASE,UNUSED_BUTTON
;    DEFINE_MAPPED_EVENT  3,PRESS_HOLD,UNUSED_BUTTON
	DEFINE_MAPPED_EVENT  3,PRESS_RELEASE,VOLDN_BUTTON
;FF
    DEFINE_MAPPED_EVENT  4,PRESS_RELEASE,DOWN_BUTTON
    DEFINE_MAPPED_EVENT  5,PRESS_HOLD,DOWN_BUTTON
;RW
    DEFINE_MAPPED_EVENT  6,PRESS_RELEASE,SEEK_DOWN_BUTTON
    DEFINE_MAPPED_EVENT  7,PRESS_HOLD,SEEK_DOWN_BUTTON
;repeat
    DEFINE_MAPPED_EVENT  8,PRESS_RELEASE,SEEK_DOWN_BUTTON
    DEFINE_MAPPED_EVENT  9,PRESS_HOLD,UNUSED_BUTTON
;mode
    DEFINE_MAPPED_EVENT 10,PRESS_RELEASE,UP_BUTTON
    DEFINE_MAPPED_EVENT 11,PRESS_HOLD,UP_BUTTON
;pg_up
    DEFINE_MAPPED_EVENT 12,PRESS_RELEASE,SEEKUP_BUTTON
    DEFINE_MAPPED_EVENT 13,PRESS_HOLD,SEEKUP_BUTTON


    endif ;(!@def(BUTTONSDEF_INC))

