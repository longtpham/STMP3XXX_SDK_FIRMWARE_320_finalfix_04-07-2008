;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2001
;
;  File        : msgs.asm
;  Description : Message structures used in player
;///////////////////////////////////////////////////////////////////////////////

    section     EXEC_Msgs

;///////////////////////////////////////////////////////////////////////////////
;   Macros
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////
                            
    nolist
    include "sysequ.inc"
    include "sysresources.inc"
    include "msgequ.inc"
    include "resource.inc"
    list                     

;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

    global    StopMessage

    global    PlayMessage
    global    PlayDirMsg

    global    MsgDecoderNextSong
    global    MsgDecoderPrevSong
    global    MsgDecoderFf 
    global    MsgDecoderRw 

    global    MsgDecoderStateChange
    global    MsgMenuSongTimeChange
    global    MsgDecoderABModeOff
    
    global    MsgBatteryChange
    global    MsgRecorderWriteData

    ; Geq Messages
    global    MsgGeqSetCoef    
    global    MsgGeqParam

    ; Parser Messages
    global    MsgParserStopSong
    global    MsgParserNextSong
    global    MsgParserNextParam
    global    MsgParserPrevSong
    global    MsgParserPrevParam
    global    MsgParserDeviceEnum
    global    NEXT_SONG
    global    NEXT_SONG_PLAY_EOF
    global    NEXT_SONG_PLAY_BUTTON
    global    PREV_SONG
    global    PREV_SONG_PLAY
    global    PREV_SONG_RWND
    global    ButtonPressBit
    global    ParserPlayBit
    global    ParserRwndBit
    global    MsgParserSetCurrentSong
    global    SetCurrentSongStringPtr
    global    MsgMenuSongTrackchange
  if @def('AUDIBLE')    
    global    MsgMenuSetSongPosition
    global    MsgMenuUpdateChapterInfo
    global    MsgMenuUsbConnected
  endif
    global    MsgUpdateLyricsAB
    global    NEXT_SONG_FFWD
    global    ParserFfwdBit
  
  
    global  LCD_SIZE_X      
    global  LCD_SIZE_Y      
    global  LCD_SIZE_ROW   
    global  LCD_POS_Y_LINE_0
    global  LCD_POS_Y_LINE_1
    global  LCD_POS_Y_LINE_2
    global  LCD_POS_Y_LINE_3
    global  LCD_POS_Y_LINE_4
    global  LCD_POS_Y_LINE_5
    global  LCD_POS_Y_LINE_6
    global  LCD_POS_Y_LINE_7

    global  LCD_FIRST_ROW_NUM
    global  LCD_FIRST_ROW_POS_Y
    global  LCD_DLINE_MENU_START_POS_Y
    
    global  LCD_DLINE_MENU_00_SONG_TIME_POS_Y
    global  LCD_DLINE_MENU_00_SONG_NUMBER_POS_Y
    global  LCD_DLINE_MENU_06_VOICE_TIME_CURRENT_POS_Y
    global  LCD_DLINE_MENU_TITLE_POS_Y
    
;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////



;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////

; LCD dimensions
LCD_SIZE_X              equ     128
LCD_SIZE_Y              equ     64
LCD_SIZE_ROW            equ     8

; Physical rows are numbered from 0 to LCD_SIZE_Y/LCD_SIZE_ROW
LCD_FIRST_ROW_NUM       equ     1
LCD_FIRST_ROW_POS_Y     equ     LCD_FIRST_ROW_NUM*LCD_SIZE_ROW ; skipping the first row

; LCD menu line positions
LCD_DLINE_ICON_0_POS_Y              equ     LCD_FIRST_ROW_POS_Y
LCD_DLINE_ICON_1_POS_Y              equ     LCD_DLINE_ICON_0_POS_Y+LCD_SIZE_ROW

; leave a blank row between the icons & typical menu information (lists may use different placement)
LCD_DLINE_MENU_START_POS_Y          equ     LCD_DLINE_ICON_1_POS_Y+(2*LCD_SIZE_ROW)

LCD_DLINE_MENU_00_SONG_NAME_POS_Y   equ     LCD_DLINE_MENU_START_POS_Y
LCD_DLINE_MENU_00_ARTIST_NAME_POS_Y equ     LCD_DLINE_MENU_00_SONG_NAME_POS_Y+LCD_SIZE_ROW
LCD_DLINE_MENU_00_SONG_TIME_POS_Y   equ     LCD_DLINE_MENU_00_ARTIST_NAME_POS_Y+LCD_SIZE_ROW
LCD_DLINE_MENU_00_SONG_NUMBER_POS_Y equ     LCD_DLINE_MENU_00_SONG_TIME_POS_Y+LCD_SIZE_ROW

LCD_DLINE_MENU_06_VOICE_NAME_POS_Y          equ     LCD_DLINE_MENU_START_POS_Y
LCD_DLINE_MENU_06_VOICE_TIME_CURRENT_POS_Y  equ     LCD_DLINE_MENU_06_VOICE_NAME_POS_Y+LCD_SIZE_ROW
LCD_DLINE_MENU_06_VOICE_TIME_TOTAL_POS_Y    equ     LCD_DLINE_MENU_06_VOICE_TIME_CURRENT_POS_Y+LCD_SIZE_ROW

LCD_DLINE_MENU_TITLE_POS_Y                  equ     LCD_FIRST_ROW_POS_Y+(1*LCD_SIZE_ROW)

; LCD Object postions
LCD_DOBJ_SONG_NAME_POS_X    equ     0
LCD_DOBJ_SONG_NAME_POS_Y    equ     LCD_DLINE_MENU_00_SONG_NAME_POS_Y
LCD_DOBJ_ARTIST_NAME_POS_X  equ     0
LCD_DOBJ_ARTIST_NAME_POS_Y  equ     LCD_DLINE_MENU_00_ARTIST_NAME_POS_Y
LCD_DOBJ_STATE_POS_X        equ     0
LCD_DOBJ_STATE_POS_Y        equ     LCD_DLINE_ICON_0_POS_Y
LCD_DOBJ_STATE_SIZE_X       equ     8
LCD_DOBJ_STATE_SIZE_Y       equ     8

;///////////////////////////////////////////////////////////////////////////////
;   Y Memory
;///////////////////////////////////////////////////////////////////////////////

      org y,"EXEC_Msgs_Y":

;///////////////////////////////////////////////////////////////////////////////
; Misc Messages
;///////////////////////////////////////////////////////////////////////////////

;--------------------------------------
; Decoder Stop Message
StopMessage         
                    dc      2
                    dc      DECODER_STOP
;--------------------------------------
MsgParserNextSong   
                    dc      3
                    dc      PARSER_NEXT_SONG
MsgParserNextParam  dc      NEXT_SONG

;--------------------------------------
MsgParserPrevSong   
                    dc      3
                    dc      PARSER_PREV_SONG
MsgParserPrevParam  dc      PREV_SONG

;--------------------------------------
MsgParserStopSong   
                    dc      2
                    dc      PARSER_STOP

;--------------------------------------
MsgParserDeviceEnum  
                    dc      2
                    dc      PARSER_DEVICE_ENUMERATE

;--------------------------------------
MsgParserSetCurrentSong
                    dc      3
                    dc      PARSER_SET_CURRENT_SONG
SetCurrentSongStringPtr
                    dc      0

;--------------------------------------
PlayMessage         
                    dc      2
                    dc      DECODER_PLAY

;--------------------------------------
        ;///////////////// T E M P O R A R Y ////////////////////////////
PlayDirMsg          
                    dc      5                     ; count
                    dc      DECODER_SET_DIR       ; opcode
                    dc      1                     ; device (card) number
                    dc      0                     ; song index number
                    dc      $00005C               ; '\' "root directory"
        ;///////////////// T E M P O R A R Y ////////////////////////////

;--------------------------------------
MsgDecoderNextSong  
                    dc      2
                    dc      DECODER_NEXT_SONG

;--------------------------------------
MsgDecoderPrevSong  
                    dc      2
                    dc      DECODER_PREV_SONG

;--------------------------------------
MsgDecoderFf        
                    dc      2
                    dc      DECODER_FFWD

;--------------------------------------
MsgDecoderRw        
                    dc      2
                    dc      DECODER_RWND


;--------------------------------------
MsgDecoderStateChange
                    dc      2
                    dc      MENU_DECODER_STATE_CHNG                                                

;--------------------------------------
MsgMenuSongTimeChange
                    dc      2
                    dc      MENU_SONG_TIME_CHNG

;--------------------------------------
MsgDecoderABModeOff
                    dc      2
                    dc      MENU_AB_MODE_OFF

;--------------------------------------
MsgBatteryChange
                    dc      2
                    dc      MENU_BATTERY_CHNG

;--------------------------------------
MsgRecorderWriteData
                    dc      2
                    dc      MENU_RECORDER_WRITE_DATA

;--------------------------------------                   
MsgGeqSetCoef
                    dc      3
                    dc      GEQ_SET_COEFFICIENTS
MsgGeqParam         dc      0

MsgMenuSongTrackchange
                    dc      2
                    dc      MENU_SONG_TRACK_CHNG

  if @def('AUDIBLE')
;----------------------------------------
MsgMenuSetSongPosition
                    dc      2
                    dc      MENU_SET_SONG_POSITION

;----------------------------------------
MsgMenuUpdateChapterInfo
                    dc      2
                    dc      MENU_UPDATE_CHAPTER_INFO
;----------------------------------------
MsgMenuUsbConnected
                    dc      2
                    dc      MENU_USB_CONNECTED
  endif

    if (@def('SYNC_LYRICS'))
;--------------------------------------
MsgUpdateLyricsAB
                    dc      2
                    dc      MENU_MSG_LYRICS_SYNC_AB
    endif

;///////////////////////////////////////////////////////////////////////////////
;   X Memory
;///////////////////////////////////////////////////////////////////////////////
    org x,"EXEC_Msgs_X":
    
    endsec
