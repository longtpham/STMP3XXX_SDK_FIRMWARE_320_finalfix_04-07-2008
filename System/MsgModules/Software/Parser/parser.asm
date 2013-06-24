;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2001
;
; STMP3 File Parser Module
;///////////////////////////////////////////////////////////////////////////////

        opt     mex

        section PARMOD_Parser
 

;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////
        GLOBAL   ParserInit
        GLOBAL   ParserProcessQueue

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////
        EXTERN    ParserProcessPtr
        EXTERN    SysPostMessage
        EXTERN    SysSetDecoder
        EXTERN    SysSetEncoder
        EXTERN    PlayMessage
        EXTERN    MsgDecoderFf
        EXTERN    MsgDecoderRw
        EXTERN    UserScratchY
        EXTERN    DecoderSR
        EXTERN    ParserCSR
        EXTERN    DecoderType
        EXTERN    ButtonPressBit
        EXTERN    ParserPlayBit
        EXTERN    ParserFfwdBit
        EXTERN    ParserRwndBit
        EXTERN    CurrentSongNumber
        EXTERN    Shuffle0to23
        EXTERN    Shuffle24to47
        EXTERN    ShuffleSeed
        EXTERN    TotalSongsPlayed
        EXTERN    SysTime
        EXTERN    ParserFileHandle
        EXTERN    CurrentDirDevId
        EXTERN    CurrentSongName
        EXTERN    CurrentSongBad
        EXTERN    Byteoffset
        EXTERN    Dev0Count
        EXTERN    Dev1Count
        EXTERN    Dev2Count
        EXTERN    Dev3Count
        EXTERN    DirectorySongCount
        EXTERN    DirectorySongNumber
        EXTERN    FILEClose
        EXTERN    FILEOpen
        EXTERN    FileRead
        EXTERN    InitBufferEmpty
        EXTERN    SongByteLengthHigh
        EXTERN    SongByteLengthLow
        EXTERN    SongByteTotalHigh
        EXTERN    SongByteTotalLow
        EXTERN    SongByteTotalConsumedHigh
        EXTERN    SongByteTotalConsumedLow
        EXTERN    SongExtensionList
        EXTERN    SongTitle
        EXTERN    SysAllLinear
        EXTERN    SysLoadResource
        EXTERN    SysCountSongs
        EXTERN    SysFindSong
        EXTERN    SysGetSpeed
        EXTERN    SysSpeedIncrease
        EXTERN    SysSpeedClockFree
        EXTERN    TotalSongCount
        EXTERN    MsgMenuSongTimeChange
        EXTERN    InvSampleRate
        EXTERN    SongInvBitRate
        EXTERN    DecGetSongPos
        EXTERN    DecGetSongTime
        EXTERN    DecSetSongPosZero
        EXTERN    DecSetSongPosEnd
        EXTERN    DecClearSongTime
	EXTERN    CP_SetByte
	EXTERN    SongArtist
        EXTERN    DecoderForceInit

        EXTERN    SongExtension
        EXTERN    Mp3Extension
        EXTERN    WavExtension
    if (!(@def('DEVICE_3400')||@def('DEVICE_3300')))
    ; WMA specific code
        EXTERN    WmaExtension
    endif
        EXTERN      cbsize
        EXTERN      cktypeH
        EXTERN      cktypeL
        EXTERN      data_ckidH
        EXTERN      data_ckidL
        EXTERN      error_flag
        EXTERN      fact_ckidH
        EXTERN      fact_ckidL
        EXTERN      nblockalign
        EXTERN      nchannels
        EXTERN      nsamplespersec
        EXTERN      read_byte_state
        EXTERN      riff_chunksizeH
        EXTERN      riff_chunksizeL
        EXTERN      riff_ckidH
        EXTERN      riff_ckidL
        EXTERN      sample_cntH
        EXTERN      sample_cntL
        EXTERN      total_cntH
        EXTERN      total_cntL
        EXTERN      wave_chunksize
        EXTERN      wave_ckidH
        EXTERN      wave_ckidL
        EXTERN      wbitspersample
        EXTERN      wformattag
        EXTERN      wsamplesperblock
        EXTERN      wave_header_size
        EXTERN      DecoderGetStatus
        EXTERN      MsgParserDeviceEnum
        EXTERN      StopMessage
        EXTERN      FileSeek

        EXTERN  F_lc_u_bP_directory
        EXTERN  F_lc_u_eP_directory


;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////

        nolist
    include "resource.inc"
    include "sysequ.inc"
    include "sysresources.inc"
    include "msgequ.inc"
    include "decoderequ.inc"
    include "mp3dec.inc"
    include "hwequ.inc"
    include "stack.xref"
    include "stackmem.xref"
    include "sysmacro.asm"
    include "systemdefs.inc"
    include "sysspeed.inc"
    include "adpcmequ.inc"
    include "parserequ.inc"
    include "dacequ.inc"
    include "project.inc"
    
    include "const.xref"
    include "parser.xref"
    include "decodermem.xref"
    include "sysmem.xref"


        page    132,60,0,0,0

;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////


;///////////////////////////////////////////////////////////////////////////////
;  X memory
;///////////////////////////////////////////////////////////////////////////////
        org x,"PARMOD_Parser_X":

;///////////////////////////////////////////////////////////////////////////////
;  Y memory
;///////////////////////////////////////////////////////////////////////////////
        org y,"PARMOD_Parser_Y":
	
;///////////////////////////////////////////////////////////////////////////////
;   Code
;///////////////////////////////////////////////////////////////////////////////
        org p,"PARMOD_Parser_overlay_P":

        ; Resource number
        dc      RSRC_PARMOD_CODE

;*******************************************************************************
; ParserInit
;*******************************************************************************

ParserInit
; init the ModuleTable process pointer
 move    #ParserProcessQueue,x0
 move    x0,y:ParserProcessPtr
 rts
        ; save off stack
        move    #_stackSaved,r6              ; setup the return address
        move    #UserScratchY,r5             ; pointer to the SP local save
        jmp     SaveSystemStack              ; save the hardware stack
_stackSaved
	move	y:UserScratchY,r5	     ; Save SP on stack
	push    r5

        ; init the ModuleTable process pointer
        move    #ParserProcessQueue,x0
        move    x0,y:ParserProcessPtr

;        jsr     InitParserVars               ; init device and song info to zeros
;        jsr     InitSongVars
        jsr     InitSongCount
        jcs     _exitInit                    ; no songs found  if carry is set
        jsr     InitSongInfo                 ; get info on first song

_exitInit
        ; restore stack
_restoreStack
	pop	r5                           ; Recover SP from stack
        move    r5,y:UserScratchY

        move    #UserScratchY,r5             ; pointer to the SP local save
        move    #_stackRestored,r6           ; setup the return address
        jmp     RestoreSystemStack           ; restore the hardware stack
_stackRestored
        rts


InitSongVars
        ; Init CurrentSongName to have DIR_SEPARATOR at beginning for absolute path
        move    #>DirectorySeparator,x0      ; "ROOT" directory
        move    x0,y:CurrentSongName

        ; Init ByteOffset to offset by "/" = 1 byte
        move    y:Const_000001,x0
        move    x0,y:Byteoffset
        rts

;*******************************************************************************
; ParserProcessQueue
; r0=Descriptor for this modules message queue (in y memory)
; m0=linear
;*******************************************************************************

ParserProcessQueue
       ; save off stack
        move    #_stackSaved,r6              ; setup the return address
        move    #UserScratchY,r5             ; pointer to the SP local save
        jmp     SaveSystemStack              ; save the hardware stack
_stackSaved
        move    y:UserScratchY,r5	         ; Save SP on stack
        push    r5

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

        ; Check  if the queue is empty
        jmp     _CheckDone

_ProcessMessage

        ; Put the tail pointer into r1
        move    b1,r1

        ; Save working variables
        push    a1                           ; Head pointer
        push    r0                           ; Pointer to queue descriptor
        push    r1                           ; Tail pointer
        push    m1                           ; Queue modulo

        ; Put the message ID into b
        move    #>MSG_ID,n1
        nop
        move    y:(r1+n1),b

        ; Verify that the message ID is valid by checking message range
        move    #>PARSER_FIRST_MSG_ID,x0
        cmp     x0,b
        jlt     _InvalidMessageID
        move    #>PARSER_LAST_MSG_ID,x0
        cmp     x0,b
        jgt     _InvalidMessageID

        ; Check for case of no songs to play
        move    y:TotalSongCount,a
        tst     a
        jeq     _DeviceEnumerate             ; recount songs
_songsPresent

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

;-------------------------NextSong Function-------------------------
_NextSong
; NextSong has one input parameter at y:r1 with values indicating these commands:
;   0 = NextSong 
;   1 = NextSong + PLAY + EOF
;   2 = NextSong + Button Pressed (needed when repeat on & button pressed, force next)
;   3 = NextSong + PLAY + Button Pressed

        ; Get the argument
        move    y:(r1)+,x0                   ; 0:NextSong , 1:NextSong + PLAY
        push    x0                           ; save off PLAY parameter

        jsr     ChooseNextSong               ; update song selection number

        pop     x0                           ; restore PLAY parameter (used below)
        tst     a                            ;  if last song -- A != 0
        jeq     _checkIfPlay
        jset    #ButtonPressBit,x0,_checkIfPlay ; continue  if button pressed for next song
        jset    #ParserFfwdBit,x0,_checkFfwd
        jmp     _exitNextSong                ;  else, exit -- no play message (done with songs)
_checkIfPlay
        jclr    #ParserPlayBit,x0,_checkFfwd
        move    y:CurrentSongBad,x1
        jset    #0,x1,_exitNextSong          ; don't send play message if song bad

        jsr     DecoderForceInit             ; decoder must init
        move    #PlayMessage,r0              ; post msg to decoder to PLAY
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage
        jmp     _exitNextSong

_checkFfwd
        jclr    #ParserFfwdBit,x0,_exitNextSong
        jsr     DecoderForceInit             ; decoder must init
        move    #MsgDecoderFf,r0             ; post msg to decoder to FFWD
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        ; HACK -- this is just so time will be shown to pass as you cross over
        move    #>$41893,x0                   ; Inv128Kbit = 4096/128000
        move    x0,y:SongInvBitRate

        move    x:DecoderType,x0
        jclr    #DECODER_TYPE_ADPCM_IMA,x0,_exitPrevSong
        bset    #SkipBlockHeader,x:ParserCSR ; tells adpcm there is no block header to look for
        ; ADJUST FOR ADPCM BOUNDARY
        ; This is done in the FFWD state of dec_adpcm.
_exitNextSong
        jmp     _NextMessage

;-------------------------PrevSong Function-------------------------
_PrevSong
; PrevSong will have one parameter : 
; 0 = PrevSong, 1 = PrevSong + PLAY + EOF, 3 = PrevSong + PLAY + Button Press
        ; Get the argument
        move    y:(r1)+,x0                   ; 0:PrevSong , 1:PrevSong + PLAY, 2:PrevSong + Rwn        jset    #ParserFfwdBit,x0,_checkFfwd
        push    x0                           ; save off PLAY parameter

        jsr     ChoosePrevSong               ; update song selection number

        pop     x0                           ; restore PLAY parameter
        jclr    #ParserPlayBit,x0,_checkRwndBit
        jsr     DecoderForceInit             ; decoder must init
        move    #PlayMessage,r0              ; post msg to decoder to PLAY
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage
        jmp     _exitPrevSong

_checkRwndBit
        jclr    #ParserRwndBit,x0,_exitPrevSong
        jsr     DecSetSongPosEnd             ; rewind from end of previous song
        jsr     DecoderForceInit             ; decoder must init
        move    #MsgDecoderRw,r0             ; post msg to decoder to RWND
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        ; HACK -- this is just so time will be shown to pass as you cross over
        move    #>$41893,x0                   ; Inv128Kbit = 4096/128000
        move    x0,y:SongInvBitRate

        move    x:DecoderType,x0
        jclr    #DECODER_TYPE_ADPCM_IMA,x0,_exitPrevSong
        bset    #SkipBlockHeader,x:ParserCSR ; tells adpcm there is no block header to look for
        ; ADJUST FOR ADPCM BOUNDARY
        ; When rewinding to a previous ADPCM selection, you must adjust so that the 
        ; pointer is on a block alignment. We are assuming 512 byte blocks.
        move    #>$fffe00,x0                 ; assumes nblockalign = 512 ($200)
        clr     a
        move    y:SongByteTotalLow,a1           

        and     x0,a                         ; line up on a block boundary
        move    y:wave_header_size,x0
        add     x0,a                         ; offset by header (takes care of RIFF header)
        move    a1,x0
        clr     b
        move    y:SongByteLengthLow,b1
        cmp     x0,b
        jgt     _storeResult
        move    #>512,x0
        sub     x0,a
_storeResult
        move    a1,y:SongByteTotalLow

        ; ADJUST CONSUMED VAR FOR ADPCM BOUNDARY  (as above) 
        ; When rewinding to a previous ADPCM selection, you must adjust so that the 
        ; pointer is on a block alignment. We are assuming 512 byte blocks.
        move    #>$fffe00,x0                 ; assumes nblockalign = 512 ($200)
        clr     a
        move    y:SongByteTotalConsumedLow,a1           

        and     x0,a                         ; line up on a block boundary
        move    y:wave_header_size,x0
        add     x0,a                         ; offset by header (takes care of RIFF header)
        move    a1,x0
        clr     b
        move    y:SongByteLengthLow,b1
        cmp     x0,b
        jgt     _storeAdjustmentResult
        move    #>512,x0
        sub     x0,a
_storeAdjustmentResult
        move    a1,y:SongByteTotalConsumedLow
        
_exitPrevSong
        jmp     _NextMessage

;-------------------------RepeatSong Function-------------------------
_Repeat
; Repeat will have one parameter : 0 = Repeat Off, 1 = Repeat All, 2 = Repeat Song
        ; Get the argument
        move    y:(r1)+,a                    ; 0:RepeatOff , 1:RepeatAll , 2:RepeatSong
        tst     a
        jne     _checkAll
_RepeatOff
        bclr    #RepeatAll,x:ParserCSR
        bclr    #RepeatSong,x:ParserCSR
        jmp     _NextMessage
_checkAll
        jclr    #0,a1,_RepeatSong
_RepeatAll
        bset    #RepeatAll,x:ParserCSR
        bclr    #RepeatSong,x:ParserCSR
        jmp     _NextMessage
_RepeatSong
        bclr    #RepeatAll,x:ParserCSR
        bset    #RepeatSong,x:ParserCSR
        jmp     _NextMessage

;-------------------------Random Function-------------------------
_Random
; Random will have one parameter : 0 = Random Off, 1 = Random On
        ; Get the argument
        move    y:(r1)+,a                    ; 0:RandomOff , 1:RandomOn
        tst     a
        jne     _RandomOn
_RandomOff
        bclr    #Random,x:ParserCSR
        jmp     _NextMessage
_RandomOn
        bset    #Random,x:ParserCSR

        ; Create a new SEED for RANDOM
        clr     b
        move    y:ShuffleSeed,b1             ; current seed
        move    y:SysTime+1,x0               ; arbitrary number -- always changing
        add     x0,b                         ; add them
        move    b1,y:ShuffleSeed             ; store new seed

        ; Clear the song bit markers (these are used to track which songs have been played)
        clr     b
        move    b,y:Shuffle0to23             ; default : bits for songs 0 to 23 (actual:1-24)
        move    b,y:Shuffle24to47            ; default : bits for songs 0 to 23 (actual:1-24)

        ; Mark current song  if already playing (or paused)
        move    x:DecoderSR,x0
        jclr    #Stop,x0,_markCurrentSong
        jmp     _exitRandom
_markCurrentSong
        clr     a
        clr     b
        move    y:CurrentSongNumber,a0
        dec     a                            ; we want bit number (so subtract 1 from song number)
        move    a0,x0
        tst     a
        jne     _nonZero
        bset    #0,b1                        ; set the bit (Song Selected)
        jmp     _songSelected
_nonZero
        rep     x0
        asr     b                            ; shift until the bit is in location 0
        bset    #0,b1                        ;  else, set it for next time
        ; undo the rotate
        rep     x0
        asl     b
_songSelected
        move    #>23,a
        cmp     x0,a
        jge     _saveShuffle0to23
        move    b1,y:Shuffle24to47           ; save new bits values
        jmp     _setSongNumber
_saveShuffle0to23
        move    b1,y:Shuffle0to23            ; save new bits values
_setSongNumber
        clr     a
        inc     a
        move    a0,y:TotalSongsPlayed        ; tracks number of songs that played

_exitRandom
        jmp     _NextMessage
;-------------------------Stop Message------------------------------
_Stop
; ParserStop will reset to the first song
        jsr     InitSongInfo
        jsr     ParserOpenSong               ; gets title & artist
        jsr     ParserCloseSong              ; don't leave it open
        jmp     _NextMessage

;-------------------------Device Enumerate---------------------------
_DeviceEnumerate
; This message is sent when a device (card) has been removed from (or inserted into) the player.
; This will cause the current song ( if playing) to stop and the Parser will re-init
; the song count for a new count value.

       ; Increase DCLK speed for parser functions              
        push    r0
        push    m0

        ; Parser speed client increases DCLK to SPEED_MAX if we're not higher.
        move    #>SPEED_MAX,x1          
        move    #>SPEED_CLIENT_PARSER,y0
        jsr     SysSpeedIncrease 

        pop     m0
        pop     r0

        jsr     DecoderGetStatus             ; returns DecoderSR in x0
        jset    #Stop,x0,_enumerate

        ; Free DCLK for other speed clients
        move    #>SPEED_CLIENT_PARSER,y0
        jsr     SysSpeedClockFree
        
        ; STOP the decoder
        move    #StopMessage,r0
        move    y:<Const_ffffff,m0
  
        jsr     SysPostMessage
        ; tell Parser to enumerate (after stop is done)
        move    #>MsgParserDeviceEnum,r0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage
        jmp     _NextMessage
_enumerate
        jsr     ParserInit

        ; Free DCLK for other speed clients
        move    #>SPEED_CLIENT_PARSER,y0
        jsr     SysSpeedClockFree

        jmp     _NextMessage

;-------------------------Set Current Song---------------------------
_SetCurrentSong
        ; Get the argument
        move    y:(r1)+,a                    ; A = song number to set to
        move    y:<Const_000001,x0           ; use for inc dev number
        tst     a                            ; check if zero
        jne     _CheckUpperLimit
        move    x0,a                         ; set to song #1
_CheckUpperLimit
        move    y:TotalSongCount,x1          ; total number of songs
        cmp     x1,a                         ; make sure A <= TotalSongCount
        jle     _SearchForSong
        move    x0,a                         ; set to first song if parameter too large
_SearchForSong
        move    a,y:CurrentSongNumber        ; save it

        clr     b          #Dev0Count,r0     ; B = device number
        do      #MAX_LOGICAL_DEVICES,_FindTheSong
        move    y:(r0)+,x1                   ; get number of songs on this device
        cmp     x1,a                         ; is song on this device?
        jgt     _NextDevice
        enddo                                ; break out of the DO loop
        jmp     _FindTheSong
_NextDevice
        sub     x1,a                         ; subtract this device from A, try next
        inc     b
_FindTheSong
        move    a,y:DirectorySongNumber      ; out of the current directory
        move    x1,y:DirectorySongCount      ; number of songs on this device (in root)
        move    b0,y:CurrentDirDevId         ; current device number

        ; store off song info by open/close file
        jsr     ParserOpenSong     
        jsr     ParserCloseSong
        jmp     _NextMessage
;-------------------------Invalid Message---------------------------
_InvalidMessageID

        error

_NextMessage
        ; Restore working variables
        pop     m1                           ; Queue modulo
        pop     r1                           ; Tail pointer
        pop     r0                           ; Pointer to buffer descriptor
        pop     a                            ; Head pointer

        ; Add the message length to the tail pointer
        move    y:(r1),n1
        nop
        move    (r1)+n1
        nop
        move    r1,b

_CheckDone
        ; Check  if head pointer equals the tail pointer
        cmp     a,b
        jne     _ProcessMessage

        ; Update the buffer descriptor with the new tail pointer
        move    #>BD_TailPtr,n0
        nop
        move    b1,y:(r0+n0)

        ; restore stack
        pop     r5                           ; Recover SP from stack
        move    r5,y:UserScratchY
        move    #UserScratchY,r5             ; pointer to the SP local save
        move    #_stackRestored,r6           ; setup the return address
        jmp     RestoreSystemStack           ; restore the hardware stack
_stackRestored


        ; I want to be called again when I have a message
        move    #EVENT_MESSAGE,a1
        move   y:<Const_ffffff,m1

        rts

_JumpTable
        dc      _NextSong
        dc      _PrevSong
        dc      _Repeat
        dc      _Random
        dc      _Stop
        dc      _DeviceEnumerate
        dc      _SetCurrentSong
; Note: It is important that the entries in the jump table be in the
; same order as the Parser message ID's which are defined in msgequ.inc


;*******************************************************************************
; Parser Functions
; 
; 
;*******************************************************************************

InitParserVars
; Inits device and song info to zeros
        move    #ParserFileHandle,r0
        move    y:<Const_ffffff,m0
        clr     a
        do      #(CurrentDirDevId-ParserFileHandle)/2,_initVars
        move    a,y:(r0)+
        move    a,y:(r0)+
_initVars
        move    a,y:(r0)+
        rts

;-----------------------------------------------------------------
; InitSongCount
; Counts the number of songs across all devices to get a TotalSongCount.
; Returns:
;            Songs Found:
;                    Carry bit cleared
;                    Number of songs in y:TotalSongCount
;            No Songs Found:
;                    Carry bit set
;                    Zero in y:TotalSongCount
;-----------------------------------------------------------------
InitSongCount
        ; Setup arguments for calling SysLoadResource
        move    #>RSRC_DIRECTORY_P_CODE,x0      ; Resource number of the applett
        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
        move    #>F_lc_u_bP_directory,y0     ; Address of the application area
        move    #>(F_lc_u_eP_directory-F_lc_u_bP_directory)*3,y1 ; Size of the application area
        move    #TARGET_MEM_P,a1                ; Application area is in P memory
    
        ; Load the directory applett into the application area
        jsr     SysLoadResource

        clr     a         ;x0,y:CurrentSongName ; a2 = dev id = 0
        clr     b         a,y:TotalSongCount
        move    a,y:CurrentSongNumber
        move    a,y:CurrentDirDevId          ; start at 0
        move    a,y:Dev0Count
        move    a,y:Dev1Count
        move    a,y:Dev2Count
        move    a,y:Dev3Count

        jsr     DecClearSongTime

        move    y:<Const_800000,x1           ; flag for first dev id
        move    #Dev0Count,r1                ; to store number of songs in device 0
        push    x1
        push    r1

        do      #MAX_LOGICAL_DEVICES,_countAllSongs
            move    y:CurrentDirDevId,a2
            move    #>CurrentSongName,a1
            move    #TARGET_MEM_Y,x0
            or      x0,a                         ; A2 = dev# , A1 = memspace/dir path ptr , A0 = 0
            move    #SongExtensionList,r0        ; R0 = ptr to extension list in Y memory
            jsr     SysCountSongs
            move    a1,a
            tst     a
            jne     _CountFound                  ; error  if carry flag == 1
            pop     r1
            pop     x1
            move    a,y:(r1)+
            jmp     _incDevId
_CountFound
            ; update song count
            pop     r1
            move    a1,b                         ; b = num songs on current device
            move    a1,y:(r1)+                   ; store number of songs on device
            move    y:TotalSongCount,x0
            add     x0,b                         ; b = total songs including current device
            move    b,y:TotalSongCount
            pop     x1
            jclr    #23,x1,_incDevId
            move    y:CurrentDirDevId,x1
_incDevId
            push    x1
            push    r1
            move    y:CurrentDirDevId,b
            move    y:<Const_000001,x0
            add     x0,b
            move    b,y:CurrentDirDevId
_countAllSongs

        pop     r1
        pop     x1
        clr     a                            ; return A = 0  if songs found
        bclr    #HW_SR_C_BITPOS,sr           ; return with carry clear  if songs found
        jclr    #23,x1,_songsFound
        ; Print No Files Present for SongTitle
        move    #SongTitle,r0
        move    #$2A2020,x1                  ; "*  "
        move    x1,y:(r0)+
        move    #$206F4E,x1                  ; " oN"
        move    x1,y:(r0)+
        move    #$6C6946,x1                  ; "liF"
        move    x1,y:(r0)+
        move    #$207365,x1                  ; " se"
        move    x1,y:(r0)+
        move    #$657250,x1                  ; "erP"
        move    x1,y:(r0)+
        move    #$6E6573,x1                  ; "nes"
        move    x1,y:(r0)+
        move    #>$002A74,x1                 ; "*t"
        move    x1,y:(r0)+
        bset    #HW_SR_C_BITPOS,sr           ; return with carry set  if no songs
        move    y:<Const_000000,x1           ; there are no songs, set the default to dev 0
_songsFound
        move    x1,y:CurrentDirDevId         ; store the first device to have songs
        rts

;-------------------------------------------------
ResetSongNumber
; For SONG files  :: resets to the first song on first disk having songs (unless in shuffle mode)
        ; Resets needed for Shuffle-mode
        clr     a
        move    a,y:TotalSongsPlayed
        move    a,y:Shuffle0to23
        move    a,y:Shuffle24to47

        ; may want to grab original offset into directory here,  if it isn't '0'
        move    y:<Const_000001,x0
        move    x0,y:CurrentSongNumber       ; out of all songs across devices
        move    x0,y:DirectorySongNumber     ; out of the current directory

        clr     b          #Dev0Count,r0     ; B = device number
        do      #MAX_LOGICAL_DEVICES,_getFirstDev
        move    y:(r0)+,a                    ; get number of songs on this device
        tst     a                            ; check  if > 0
        jeq     _noSongs
        enddo                                ; break out of the DO loop
        jmp     _getFirstDev
_noSongs
        inc     b
_getFirstDev
        move    a,y:DirectorySongCount       ; number of songs on this device (in root)
        move    b0,y:CurrentDirDevId         ; current device number
_exitResetSongNumber
        rts

;-------------------------------------------------
InitSongInfo
        jsr     InitSongVars
        jsr     ResetSongNumber              ; sets up for first song on first device with songs

_continueInit
        jsr     ParserCountSongs             ; returns value in y:DirectorySongCount
        jsr     DecClearSongTime             ; clears total and current time
        jsr     ParserOpenSong               ; opens song, gets song info
        jsr     ParserCloseSong
        rts

;-----------------------------------------------------------------
; ParserCountSongs
; Inputs: y:CurrentDirDevId has device id
;;;;;         y:CurrentDirPath has the current directory to play from
;         y:CurrentSongName has the current directory to play from
; This function will count the songs in the current directory.
;-----------------------------------------------------------------
ParserCountSongs
        ;-------------------------------------------------------------
        ; Count the Songs in the Directory (SysCountSongs)
        ;-------------------------------------------------------------
        ; Inputs:   A2 = Device number  A1 = Complex ptr to directory path
        ;           R0 = Pointer to an extension list (in y memory)
        ; Output:
        ;           A1 = File Count  if successful (error code  if not)
        ;-------------------------------------------------------------
        ; Setup arguments for calling SysLoadResource
        move    #>RSRC_DIRECTORY_P_CODE,x0      ; Resource number of the applett
        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data

        move    #>F_lc_u_bP_directory,y0     ; Address of the application area
        move    #>(F_lc_u_eP_directory-F_lc_u_bP_directory)*3,y1 ; Size of the application area
        move    #TARGET_MEM_P,a1                ; Application area is in P memory
    
        ; Load the directory applett into the application area
        jsr     SysLoadResource

        clr     a
        move    y:CurrentDirDevId,a2
        move    #>CurrentSongName,a1
        move    #TARGET_MEM_Y,x0
        or      x0,a                    ; A2 = dev# , A1 = memspace/dir path ptr , A0 = 0
        move    #SongExtensionList,r0   ; R0 = ptr to extension list in Y memory
        jsr     SysCountSongs
        jcc     _CountFound
        clr     a
_CountFound
        move    a1,y:DirectorySongCount ; total number of songs in directory
        rts

;-----------------------------------------------------------------
; ParserOpenSong
;///////////////// T E M P O R A R Y ////////////////////////////
; Currently, ParserOpenSong will return Title & Artist for ID3v1
; It will return the file name  if not ID3v1.
; ID3v2 is NOT yet supported.
; Currently, Fs and bitrate come from Mp3 decoder when it syncs.
;///////////////// T E M P O R A R Y ////////////////////////////
;-----------------------------------------------------------------
ParserOpenSong

        clr     a
        move    a,y:CurrentSongBad

        ;-------------------------------------------------------------
        ; Find the Song (SysFindSong)
        ;-------------------------------------------------------------
        ; Inputs:   A2 = Device number  A1 = Complex ptr to directory path
        ;           B2 = Song number    B1 = Complex ptr to file name buffer
        ;           X1 = Byte offset within file name buffer
        ;           R0 = Pointer to an extension list (in y memory)
        ; Output:
        ;           File name buffer contains the song name.
        ;           X1:X0 = total number of bytes for the song
        ;-------------------------------------------------------------
        ; Setup arguments for calling SysLoadResource
        move    #>RSRC_DIRECTORY_P_CODE,x0      ; Resource number of the applett
        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
        move    #>F_lc_u_bP_directory,y0     ; Address of the application area
        move    #>(F_lc_u_eP_directory-F_lc_u_bP_directory)*3,y1 ; Size of the application area
        move    #TARGET_MEM_P,a1                ; Application area is in P memory
    
        ; Load the directory applett into the application area
        jsr     SysLoadResource

        clr     a      #TARGET_MEM_Y,x0      ; memory space identifier (Y RAM)
        move    y:Byteoffset,x1              ; X1 = byte offset (0)
        move    y:CurrentDirDevId,a2         ; card number for current directory
        move    #CurrentSongName,a1          ; base address of directory path
        or      x0,a                         ; A2 = dev#, A1=memspace/dir path, A0 = 0

        clr     b      #SongExtensionList,r0 ; list of song extensions to look for
        move    y:DirectorySongNumber,b2     ; number of song to find in this directory
        move    #CurrentSongName,b1          ; base address of song name (use ByteOffset to skip DIR)
        or      x0,b                         ; B2 = index, B1 = memspace/song ptr, B0=0
        jsr     SysFindSong                  ; returns song name in y:CurrentSongName
        jcs     _exit
        ; Store the byte size of the song
        move    x1,y:SongByteLengthHigh      ; total bytes of song (MSBs)
        move    x0,y:SongByteLengthLow       ; total bytes of song (LSBs)

        jsr     SetDecoder                   ; ensure the correct decoder is used.
        push    x0                           ; x0 has decoder bit set

        ;-------------------------------------------------------------
        ; Open the Song (FILEOpen)
        ;-------------------------------------------------------------
        ; Inputs:   A2 = Device number  A1 = Complex ptr to file name buffer
        ;           A0 = Open attributes
        ; Output:
        ;           A1 = File handle ( if successful)
        ;           A1 = Error code  ( if failed)
        ;-------------------------------------------------------------
        jsr     SysAllLinear
        clr     a      #TARGET_MEM_Y,x0      ; memory space identifier (Y RAM)
        move    y:CurrentDirDevId,a2         ; card number for current directory
        move    #CurrentSongName,a1          ; base address of song name (absolute path)
        or      x0,a                         ; A2 = dev#, A1=memspace/file name buffer, A0=0
        move    #>READMODE,a0                ; A0 = file open attribute (Read Only)
        jsr     FILEOpen
        jcc     _okay2
        pop     x0
        jmp     _exit
_okay2

        move    a1,y:ParserFileHandle        ; store the handle for the file
        pop     x0                           ; x0 has decoder bit set
        jsset   #DECODER_TYPE_MP3,x0,FindMp3Info
        jsset   #DECODER_TYPE_ADPCM_IMA,x0,FindAdpcmInfo
    if (!(@def('DEVICE_3400')||@def('DEVICE_3300')))
        jsset   #DECODER_TYPE_WMA,x0,FindWMAInfo
	endif
        ;-------------------------------------------------------------
        ; Seek to Zero (init) the Song (FileSeek)
        ;-------------------------------------------------------------
        ; Inputs:   A2 = Handle  A1/A0 = relative offset
        ;-------------------------------------------------------------
        jsr     SysAllLinear
        clr     a                            ; offset of 0
        move    y:ParserFileHandle,a2
        jsr     FileSeek
        jcs     _exit
        jsr     DecSetSongPosZero            ; reset to beginning of song
_exit
        rts


FindMp3Info
; This routine will look for ID3v1 TAG at end of MP3 file.

        ;-------------------------------------------------------------
        ; Seek to TAG (last 128 Bytes)
        ;-------------------------------------------------------------
        ; Inputs:   A2 = Handle  A1/A0 = relative offset
        ;-------------------------------------------------------------
        jsr     SysAllLinear
        clr     a      y:<Const_000080,x0
        move    a,x1
        move    y:SongByteLengthHigh,a1
        move    y:SongByteLengthLow,a0       ; total bytes of song (LSBs)
        sub     x,a
        push    a1
        push    a0                           ; save for later
        move    y:ParserFileHandle,a2
        jsr     FileSeek
        
        jcs     _fileSystemError

        ;-------------------------------------------------------------
        ; Read from the Song (FileRead)
        ;-------------------------------------------------------------
        ; Inputs:   A2 = File handle  A1 = Number of bytes to read in bits 0-15
        ;           A0 = Memory space identifier
        ;           R0 = Buffer pointer to read into
        ;           M0 = Buffer modulo
        ; Output:
        ;           A1 = Number of bytes read ( if successful)
        ;           A1 = Error code  ( if failed)
        ;-------------------------------------------------------------
        clr     a
        move    y:ParserFileHandle,a2
        move    y:<Const_000080,a1
        move    #TARGET_MEM_X,a0             ; A0 = Memory space identifier
        move    #UserScratchY,r0             ; R0 = Buffer pointer to read into
        jsr     SysAllLinear                 ; force all M registers to linear
        move    #SCRATCH_USER_Y_MODULO,m0    ; M0 = Buffer modulo
        jsr     FileRead                     ; Fill the buffer
        jcc     _storeInfo
_fileSystemError
	bset	#0,y:CurrentSongBad              ; handle a bad file read
        pop     a0
        pop     a1                           ; balance the stack
        move    #SongTitle,r4                ; song title stored here
	jmp	_noID3v1Tag
_storeInfo
        move    #UserScratchY,r0             ; R0 = Buffer pointer with info
        move    y:<Const_ffffff,m0                   ; linear addressing
        move    #>ID3_TAG_DYSLEXIC,x0
        clr     a     #SongTitle,r4          ; used  if ID3v1Tag exists
        pop     a0                           ; a0 = SongByteLengthLow adjusted by 128
        pop     a1                           ; a1 = SongByteLengthHigh adjusted by 128
        move    x:(r0)+,b                    ;  if there is a TAG, then b="GAT"
        cmp     x0,b
        jne     _noID3v1Tag
_ID3v1Tag
        move    a0,y:SongByteLengthLow
        move    a1,y:SongByteLengthHigh
        do      #20,_ID3Copy
        move    x:(r0)+,x0
        move    x0,y:(r4)+                   ; copy song TITLE and ARTIST to proper locations
_ID3Copy
        ;zero out the last byte in both the song name and artist, just in case the tag isn't 0 terminated
        clr     a
        move    #SongTitle,a0               ;set up the complex pointer
        bset    #TARGET_MEM_Y_BITPOS,a0     ;set the memory space
        move    #>29,r1                     ;lets zero out SongTitle[29]
        jsr     CP_SetByte
;        clr     a                          ;we're assuming a1 stays zero, so we don't need to do this
        move    #SongArtist,a0              ;set up the complex pointer
        bset    #TARGET_MEM_Y_BITPOS,a0     ;set the memory space
;        move    #>29,r1                     ;we're assuming r1 stays 29, so we comment this out
        jsr     CP_SetByte              
        jmp     _doneSongInfo           
_noID3v1Tag
        ; store the file name as the song name
        move     #CurrentSongName,r0
        do       #5,_fileNameCopy
        move     y:(r0)+,x0
        move     x0,y:(r4)+
_fileNameCopy
        clr      a
        do       #15,_clearOutRemainder
        move     a,y:(r4)+
        nop
_clearOutRemainder
_doneSongInfo
;; at this point, the SongTitle and SongArtist are stored into memory (dyslexic format)
;; and are available for printing to the screen.

        move     #0,x0                       ; return with x0 cleared!!!
        rts

FindAdpcmInfo
; This routine will parse the WAV file and get the appropriate variables.
        ;-------------------------------------------------------------
        ; Get first block of data from WAV file
        ;-------------------------------------------------------------
        ; Inputs:   A2 = Handle  A1/A0 = relative offset
        ;-------------------------------------------------------------
        jsr     SysAllLinear
        clr     a
        move    y:ParserFileHandle,a2
        jsr     FileSeek
        jcs     _fileSystemError

        ;-------------------------------------------------------------
        ; Read from the Song (FileRead)
        ;-------------------------------------------------------------
        ; Inputs:   A2 = File handle  A1 = Number of bytes to read in bits 0-15
        ;           A0 = Memory space identifier
        ;           R0 = Buffer pointer to read into
        ;           M0 = Buffer modulo
        ; Output:
        ;           A1 = Number of bytes read ( if successful)
        ;           A1 = Error code  ( if failed)
        ;-------------------------------------------------------------
        clr     a
        move    y:ParserFileHandle,a2
        move    #>128,a1
        move    #TARGET_MEM_X,a0             ; A0 = Memory space identifier
        move    #UserScratchY,r0             ; R0 = Buffer pointer to read into
        jsr     SysAllLinear                 ; force all M registers to linear
        move    #SCRATCH_USER_Y_MODULO,m0    ; M0 = Buffer modulo
        jsr     FileRead                     ; Fill the buffer
        jcc     _storeInfo
_fileSystemError
        or      #>$01,ccr        
        jmp     _exit
_storeInfo

        move    #UserScratchY,r0             ; R0 = Buffer pointer to read into
        jsr     ParserUnpackWaveHeader

        jsr     DecSetDacSrr                 ; set the dac sample rate
        jsr     GetInvSampleRate             ; load 1/sample rate into y:InvSampleRate
_storeFileName
        ; store the file name as the song name
        clr     a     #SongTitle,r4          ; used  if ID3v1Tag exists
        move     #CurrentSongName,r0
        do       #5,_fileNameCopy
        move     y:(r0)+,x0
        move     x0,y:(r4)+
_fileNameCopy
        clr      a
        do       #15,_clearOutRemainder
        move     a,y:(r4)+
        nop
        and      #>$FE,ccr
_clearOutRemainder
_exit
        move     #0,x0                       ; return with x0 cleared!!!
        rts

    if (!(@def('DEVICE_3400')||@def('DEVICE_3300')))

FindWMAInfo
		; Need to make sure that the right overlays are always available,
		; while calling this, otherwise the code will behave unpredicatably.
		jsr		DecoderForceInit
		; Just post a message to WMADecoder to Parse the header information.
        move    r7,r0
        move    y:<Const_000002,x0
        push    x0
        move    #>DECODER_GET_SONG_INFO,x0
        push    x0
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage
        pop     x0
        pop     x0
        move     #0,x0                       ; return with x0 cleared!!!
        rts
	endif
;;***********************************************************************
;  Function:
;      ParserUnpackWaveHeader   ; formerly UnPack_wave_header
;  Description:
;       Unpack the wave header
;  Parameters passed:
;  Return values:Places error value in the errorflag.
;  Note:
;;***********************************************************************
ParserUnpackWaveHeader
; UnPack_wave_header
  push   r0                     ; save starting point
  clr    a
  move   a,y:total_cntL
  move   a,y:total_cntH
  move   a,y:sample_cntL
  move   a,y:sample_cntH
  move   a,y:read_byte_state
  move   a,y:error_flag
  move  A,y:error_flag
  move  #>1,n1
  move  #>2,n2
  move  #>3,n3
  
  ; UnPack "RIFF" and validate for error
  move  n3,y1
  jsr   Wave_unpack
  move  y:riff_ckidH,A
  cmp    x1,A    #>RIFF_UNPACK_ERROR,y0
  jne    _Error

  move  n1,y1
  jsr   Wave_unpack
  move  y:riff_ckidL,A
  cmp    x1,A    #>RIFF_UNPACK_ERROR,y0
  jne    _Error

  ; Find value of chunksize
  move  n3,y1
  jsr   Wave_unpack
  move  x1,y:riff_chunksizeL

  move  n1,y1
  jsr   Wave_unpack
  move  x1,y:riff_chunksizeH

  ; unpack WAVE and check for error
  move  n3,y1
  jsr   Wave_unpack
  move  y:cktypeH,A
  cmp    x1,A    #>WAVE_UNPACK_ERROR,y0
  jne    _Error

  move  n1,y1
  jsr   Wave_unpack
  move  y:cktypeL,A
  cmp    x1,A    #>WAVE_UNPACK_ERROR,y0
  jne    _Error

  ; unpack "fmt " and check for error
  move  n3,y1
  jsr   Wave_unpack
  move  y:wave_ckidH,A
  cmp    x1,A    #>FMT_UNPACK_ERROR,y0
  jne    _Error

  move  n1,y1
  jsr   Wave_unpack
  move  y:wave_ckidL,A
  cmp    x1,A    #>FMT_UNPACK_ERROR,y0
  jne    _Error

  ; Pack Wave chunk size
  ; Find value of chunksize
  move  n3,y1
  jsr   Wave_unpack
  move  x1,y:wave_chunksize

  move  n1,y1
  jsr   Wave_unpack
  ; The return value of unpack is 0, but is not validated.

  ; Get wFormattag
  move  n2,y1
  jsr   Wave_unpack
  move  x1,y:wformattag                ; defines the type of WAVE file

  move  n2,y1
  jsr   Wave_unpack
  ; UnPack nchannels
  move  x1,y:nchannels                 ; 1 for mono, 2 for stereo (not supported in our code yet)

  move  n3,y1
  jsr   Wave_unpack
  ; UnPack nSamplespersec
  move  x1,y:nsamplespersec            ; frequency of the sample rate of the wave file

  move  n1,y1
  jsr   Wave_unpack
  ; This value should be zero

  ; UnPack navgbytespersec, not useful here as no streaming data involved
  move  n3,y1
  jsr   Wave_unpack
  move  n1,y1
  jsr   Wave_unpack

  ; UnPack nblockalign
  move  n2,y1
  jsr   Wave_unpack
  move  x1,y:nblockalign               ; the block alignment (in bytes) of the data

  ; UnPack wbitspersample
  move  n2,y1
  jsr   Wave_unpack
  move  x1,y:wbitspersample            ; number of bits per channel

  move  y:wformattag,x1

;  move  #>WAVE_FORMAT_PCM,A
;  cmp    x1,A
;  jeq    _Unpack_fmt_over

  move  #>WAVE_FORMAT_DVI_ADPCM,A
  cmp    x1,A
  jeq    _Unpack_DVI_ADPCM
  
  move  #>WAVE_FORMAT_ADPCM,A
  cmp    x1,A
  jeq    _Unpack_MS_ADPCM
  move  #>UNKNOWN_CODEC_ERROR,y0  
  jmp    _Error

_Unpack_MS_ADPCM
_Unpack_DVI_ADPCM
  ; unpack cbsize
  move  n2,y1
  jsr   Wave_unpack
  move  x1,y:cbsize                    ; size in bytes of the extra information in the WAVE format header

  ; UnPack wsamplesperblock
  move  n2,y1
  jsr   Wave_unpack
  move  x1,y:wsamplesperblock          ; number of samples per channel per block

_Unpack_fmt_over
_Look_for_Data

  ; Try to locate data section
  move  n3,y1
  jsr   Wave_unpack
  move  y:data_ckidH,A
  cmp    x1,A
  jeq    _Data_section

  ;  if not yet "DATA", check for "FACT"
  move  y:fact_ckidH,A
  cmp    x1,A
  jeq    _Fact_section

;===========================================================
  ;  if not "DATA" nor "FACT", skip UNKNOWN chunk
  ; 
  move  n1,y1
  jsr   Wave_unpack                    ; high portion of tag

  ; get chunksize
  clr   b
  move  n3,y1
  jsr   Wave_unpack
  move  x1,b0                          ; low bytes of chunksize
  move  n1,y1
  jsr   Wave_unpack
  move  x1,b1                          ; high byte of chunksize

_loop_on_unknown
  move  n1,y1
  jsr   Wave_unpack
  dec   b
  jne   _loop_on_unknown
  jmp   _Look_for_Data
;===========================================================

_Fact_section
  move  n1,y1
  jsr   Wave_unpack
  move  y:fact_ckidL,A
  cmp    x1,A    #>FACT_UNPACK_ERROR,y0  
  jne    _Error

  ; Skip fact chunksize.
  move  n3,y1
  jsr   Wave_unpack
  move  n1,y1
  jsr   Wave_unpack

  ; Find sample_cnt.
  move  n3,y1
  jsr   Wave_unpack
  move  x1,y:sample_cntL

  move  n1,y1
  jsr   Wave_unpack
  move  x1,y:sample_cntH

  ; Now expect another chunk, should be data
  jmp   _Look_for_Data

_Data_section
  move  n1,y1
  jsr   Wave_unpack
  move  y:data_ckidL,A
  cmp    x1,A    #>DATA_UNPACK_ERROR,y0  
  jne    _Error

  ; Unpack data size
  move  n3,y1
  jsr   Wave_unpack
  move  x1,y:total_cntL
  move  n1,y1
  jsr   Wave_unpack
  move  x1,y:total_cntH

  ; Calculate byte size of entire wave header
  clr     a
  move    r0,a1
  pop     x0                           ; starting point (r0)
  sub     x0,a                         ; a = # words
  move    a1,x0
  move    #>3,x1                       ; 3 bytes/word
  mpy     x0,x1,a                      ; calculate bytes
  asr     a                            ; sign bit adjust
  move    a0,a                         
  move    y:read_byte_state,x0         ; num bytes extra
  add     x0,a
  move    a1,y:wave_header_size        ; num bytes in header

  ; Update SongByteLength to be total_cnt + wave_header (TBD:: change this to just total_cnt?)
  move    a1,x0
  clr     a
  move    y:total_cntH,a1
  move    y:total_cntL,a0
  move    #0,x1
  add     x,a                          ; song byte length -- header plus data
  move    a1,y:SongByteLengthHigh      ; total bytes of song (MSBs)
  move    a0,y:SongByteLengthLow       ; total bytes of song (LSBs)
  rts

_Error  
  pop   r0                             ; balance the stack
  move  y0,y:error_flag
  bset  #0,y:CurrentSongBad            ; handle a bad file read
  rts

;;***********************************************************************
;  Function:
;      Wave_unpack
;  Description:
;      UnPack wave data
;  Parameters passed: 
;      y1 = number of bytes to unpack
;  Return value:
;      x1 = value to unpack
;;***********************************************************************
Wave_unpack
  jsr    ByteRead
  move  A1,x1
  move  n1,A
  cmp   y1,A
  jeq   _Wav_unpack_end
  
  jsr    ByteRead
  move  A1,x0
  move  #>$80,y0
  mpy    x0,y0,A
  move  A0,A1
  or    x1,A
  move  A1,x1

  move  n2,A
  cmp    y1,A
  jeq    _Wav_unpack_end

  jsr    ByteRead
  move  A1,x0
  move  #>$8000,y0
  mpy    x0,y0,A
  move  A0,A1
  or    x1,A
  move  A1,x1

_Wav_unpack_end
  rts

;;***********************************************************************
;  Function:
;      ByteRead
;  Description:
;      Reads a byte at a time
;    Parameters passed: 
;       r0: Pointer to Input buffer
;
;  Registers Trashed
;      x0,y0,A
;  Return Values:
;      one Byte in A1
;;***********************************************************************
ByteRead
  clr    A    y:read_byte_state,y0
  cmp    y0,A  #>1,x0
  jeq    _Third_Word
  add   x0,A
  cmp    y0,A  
  jeq    _Second_Word

  ; MSB
_First_Word
  clr    A  #>$80,y0
  move  A,y:read_byte_state
  move  x:(r0)+,x0
  mpy    x0,y0,A    #>$ff,x0
  and    x0,A
  rts

_Second_Word
  add   x0,A
  move  A,y:read_byte_state
  move  x:(r0),x0
  move  #>$8000,y0
  mpy    x0,y0,A    #>$ff,x0
  and    x0,A
  rts

  ; LSB
_Third_Word    
  ; Third Word
  add   x0,A  #>$ff,x0
  move  A,y:read_byte_state
  move  x:(r0),A     
  and    x0,A
  rts


;-----------------------------------------------------------------
; SetDecoder
; This function will set the proper decoder depending on the file type
; For now we're just going to base it on file extension.  We'll need to add
; code to parse .wav files 
;-----------------------------------------------------------------
SetDecoder
        clr     a
        move    y:SongExtension,a1           ; A1 = song extension
        move    #>0,x0                       ; use x0 for Decoder Type

_checkMp3Extension
        ; See  if the extension from the FCB matches any Mp3 extensions in the list
        move    #>Mp3Extension,r4
        do      #3,_Mp3ExtensionLoop
        move    y:(r4)+,y0
        cmp     y0,a
        jne     _NoMatchMp3

        ; Found a matching extension
        bset    #DECODER_TYPE_MP3,x0
        move    x0,x:DecoderType
        enddo                           ; Break out of the extension loop
        jmp     _SongFound              ; A song has been found!

_NoMatchMp3
        nop
        nop
_Mp3ExtensionLoop
        nop

; **** Need to expand this for PCM/ADPCM etc. *****
_checkWavExtension
        ; See  if the extension from the FCB matches any Mp3 extensions in the list
        move    #>WavExtension,r4
        do      #8,_WavExtensionLoop
        move    y:(r4)+,y0
        cmp     y0,a
        jne     _NoMatchWav

        ; TBD ::: ADD HERE code to open the file, parse through for ADPCM/PCM/etc.

        ; Found a matching extension
        bset    #DECODER_TYPE_ADPCM_IMA,x0
        move    x0,x:DecoderType
        enddo                           ; Break out of the extension loop
        jmp     _SongFound              ; A song has been found!

_NoMatchWav
        nop
        nop
_WavExtensionLoop
        nop

    if (!(@def('DEVICE_3400')||@def('DEVICE_3300')))
    ; WMA specific code
_checkWmaExtension
        ; See  if the extension from the FCB matches any Wma extensions in the list
        move    #>WmaExtension,r4
        do      #8,_WmaExtensionLoop
        move    y:(r4)+,y0
        cmp     y0,a
        jne     _NoMatchWma

        ; Found a matching extension
        bset    #DECODER_TYPE_WMA,x0
        move    x0,x:DecoderType
        enddo                           ; Break out of the extension loop
        jmp     _SongFound              ; A song has been found!

_NoMatchWma
        nop
        nop
_WmaExtensionLoop
        nop
    endif
; ---- add new loops here for AAC, etc as decoders become available

_SongFound
        push    x0
        jsr     SysSetDecoder
        pop     x0                      ; preserve which decoder is in
        rts

;-----------------------------------------------------------------
; ParserCloseSong
; This function will use the ParserFileHandle to close the file.
;-----------------------------------------------------------------
ParserCloseSong
        ;-------------------------------------------------------------
        ; Close the Song (FILEClose)
        ;-------------------------------------------------------------
        ; Inputs:   A2 = File handle
        ; Output:
        ;           A1 = File handle ( if successful)
        ;           A1 = Error code  ( if failed)
        ;-------------------------------------------------------------
        jsr     SysAllLinear
        clr     a
        move    y:ParserFileHandle,a2        ; pass the file handle to the function
        jsr     FILEClose
        jcc     _fileClosed
;        error                                ; ERROR - file did not close (TBD -- ADD CODE!!)
_fileClosed
_exit
        ;*** TBD : clear song parameters?
        rts

;-----------------------------------------------------------------
DecShuffle
; Chooses the next song by calling the RandomNumGenerator routine which will produce
; a random number [0..TotalSongCount-1]. Checks  if this song has been played yet which
; is determined  if the bit corresponding to the song number is set.
_callRandFunction
        move    y:TotalSongCount,y1          ; use as scaling factor
        jsr     RandomNumGenerator           ; returns rand number in x0 (0 to TotalSongCount-1)
        clr     b
        move    y:Shuffle0to23,b1            ; default : bits for songs 0 to 23 (actual:1-24)
        move    #>23,a
        cmp     x0,a
        jge     _accLoaded
        move    y:Shuffle24to47,b1           ; bits for songs 24 to 47 (actual:25-48)
_accLoaded

        ; check  if song played yet
        move    x0,a
        tst     a                            ; must make sure we don't "rep #0"
        jne     _nonZero
        jset    #0,b1,_callRandFunction      ;  if set, get a different number
        bset    #0,b1                        ; set the bit (Song Selected)
        jmp     _songSelected

_nonZero
        rep     x0
        asr     b                            ; shift until the bit is in location 0
        jset    #0,b1,_callRandFunction      ;  if set, get a different number (song already played)
        bset    #0,b1                        ;  else, set it for next time
        ; undo the rotate
        rep     x0
        asl     b

_songSelected
        move    #>23,a
        cmp     x0,a
        jge     _saveShuffle0to23
        move    b1,y:Shuffle24to47           ; save new bits values
        jmp     _setSongNumber
_saveShuffle0to23
        move    b1,y:Shuffle0to23            ; save new bits values
_setSongNumber
        clr     a
        move    x0,a0
        inc     a                            ; song number is 1 to N (not 0 to N-1)
        move    a0,y:CurrentSongNumber       ; overall song number across all devices
        move    y:TotalSongsPlayed,a0        ; keep track of songs played
        inc     a
        move    a0,y:TotalSongsPlayed

        ; get correct device number and correct directory song number
        move    y:CurrentSongNumber,x0
        jsr     GetDirectoryPointers

        rts

GetDirectoryPointers
; Input:   x0 = CurrentSongNumber (across all devices)
; Outputs: y:DirectorySongNumber is updated
;          y:CurrentDirDevId is updated

        move    #Dev0Count,r0                ; point to song counts on dev 0
        move    y:<Const_ffffff,m0                    ; linear
        clr     a                            ; A will hold the device number
        clr     b                            ; B will hold the song count
_findDevice
        move    y:(r0)+,y0                   ; y0 = song count on R0 device
        push    b1                           ; store prev song count
        add     y0,b                         ; B = song count on this dir + prev dir's
        cmp     x0,b
        jge     _foundDevice                 ;  if song count >= current song number, done
        inc     a                            ;  else, goto next device and try again
        pop     x1                           ; balance the stack
        jclr    #MAX_LOGICAL_DEVICES,a0,_continue
        error                                ; shouldn't get here --  else, endless loop problem
_continue
        jmp     _findDevice
_foundDevice
        move    a0,y:CurrentDirDevId         ; store device number
        pop     x1                           ; previous song count
        move    x0,b                         ; B = current song number
        sub     x1,b                         ; B = current song number - prev song count = dir song number
        move    b1,y:DirectorySongNumber     ; store it

        rts


;-----------------------------------------------------------------
RandomNumGenerator
; Input:  Y1 contains the scaling factor
;         Y:ShuffleSeed contains the seed
; Output: X0 contains random number (Song number - 1)
_beginRand
        move    y:ShuffleSeed,x0             ; X0 = seed
        move    #$544a,x1                    ; X1 = sum value
        move    #$3a0d,y0                    ; Y0 = multiplier
        mpy     y0,x0,a                      ; A0 = multiplier * seed
        asr     a                            ; undo the lack of sign bit in B0
        move    a0,a1                        ;  A = A0
        add     x1,a                         ;  A = (mult * seed) + sum
        move    a1,x0                        ; X0 = result (unscaled)
        mpy     x0,y1,a                      ; random number 1-64 (scaled)
        tst     a                            ; make sure it is positive value
        jge     _positive
        add     y1,a                         ; make it positive
_positive
        move    x0,y:ShuffleSeed             ; store new seed
        move    a1,x0                        ; return random number in x0
        rts

;-------------------------------------------------
ChoosePrevSong
; Output: CurrentSongNumber is updated
;  if > 5 seconds have played, resets to beginning of current song.
;  if <= 5 seconds have played, resets to previously played song.
;  if RepeatSong, should go to previous or current (see above) song and repeat that one.
;  if Random, choose the previous song from the Randomized list of songs.

        jsr     DecGetSongTime               ; Returns: A1 = Song Minutes Elapsed, A0 = Song Seconds Elapsed

_checkMinutes
        move    a0,x0                        ; save off seconds
        move    y:<Const_000000,a0           ; clear out low bits
        tst     a                            ;  if minutes > 0, reset to beginning of song
        jgt     _stayOnCurrentSong
        move    x0,a1                        ; a1 = seconds elapsed
        move    #>DECODER_SONG_SEC_THRESHOLD,x0
        cmp     x0,a
        jle     _gotoPreviousSong            ;  if seconds <= threshold, go to previous song

_stayOnCurrentSong
        jsr     DecSetSongPosZero
        rts

_gotoPreviousSong
        ; check  if need to switch devices
        move    x:ParserCSR,x0
        jset    #Random,x0,_prevRandomSong
        move    y:DirectorySongNumber,a      ; decrement directory song number
        move    y:CurrentSongNumber,b        ; decrement overall song number
        move    y:<Const_000001,x1
        sub     x1,a
        sub     x1,b
        tst     a     b,y:CurrentSongNumber  ; be sure not at song "0" (start at 1)
        jle     _wraparound
_noWraparound
        move    a,y:DirectorySongNumber
        jsr     ParserOpenSong
        jmp     _exitChoosePrevSong
_wraparound
        ; find previous device with songs
        ; ( if on first device, wraparound to last device)
        clr     a
        move    #Dev0Count,r0                ; point to song counts on dev 0
        move    y:<Const_ffffff,m0                    ; linear
_loop
        move    y:CurrentDirDevId,a0         ; offset
        dec     a
        tst     a
        jge     _aOk
        clr     a
        move    #>(MAX_LOGICAL_DEVICES-1),a0          ; set to last device
        move    y:TotalSongCount,x0
        move    x0,y:CurrentSongNumber       ; set to last song
_aOk
        move    a0,n0
        move    a0,y:CurrentDirDevId         ; update it
_continue
        move    y:(r0+n0),a                  ; update pointer
        tst     a
        jeq     _loop                        ; try again  if no songs
        move    a,y:DirectorySongCount       ; new device count
        move    a,y:DirectorySongNumber      ; set to last song in directory
        jsr     ParserOpenSong
        jmp     _exitChoosePrevSong
_prevRandomSong
        ; use the random buffer to select next song
        ; check  if all songs have played
        move    y:TotalSongsPlayed,x0
        move    y:TotalSongCount,a
        cmp     x0,a                         ;  if TotalSongCount != TotalSongsPlayed
        jne     _moreRandomSongs             ; then there are more songs to play
        clr     a                            ;  else -- all have played, reset
        move    a,y:Shuffle0to23             ; clear for next time
        move    a,y:Shuffle24to47            ; clear for next time
        move    a,y:TotalSongsPlayed
_moreRandomSongs
        jsr     DecShuffle                   ; randomly select next song
        jsr     ParserOpenSong
_exitChoosePrevSong
        jsr     ParserCloseSong 
        rts

;-------------------------------------------------
ChooseNextSong
; Input:  x0 = ParserNextSongParameter
; Output: CurrentSongNumber is updated
;          if done with songs, return with acc. a = 1 and reset CurrentSongNumber to 1.
;          if more songs, return 0 in acc. a and CurrentSongNumber is updated to next song.
;          if RepeatSong -- should go to next song and repeat on that one.

        ; check  if RepeatSong && EOF occurred
        jset    #ButtonPressBit,x0,_continueChooseNextSong ;  if bit 1 = 1, a button was pressed, goto next song
        move    x:ParserCSR,x0
        jclr    #RepeatSong,x0,_continueChooseNextSong     ;  else, EOF was reached, so check if RepeatSong is set
        move    y:CurrentSongBad,a                         ; = 0 if song ok, 1 if not ok
        tst     a
        jeq     _exitChooseNextSong                        ;  song is not bad, so keep playing it
        jmp     _rtsChooseNextSong                         ; if Song Bad -- do not clear it by opening it
_continueChooseNextSong
        move    x:ParserCSR,x0
        jset    #Random,x0,_nextRandomSong
        ; check  if last song of last device
        move    y:CurrentSongNumber,a        ; overall song number
        move    y:TotalSongCount,x0          ; overall song count
        cmp     x0,a
        jlt     _checkDirCount
_checkRepeatAll
        move    x:ParserCSR,x0
        jclr    #RepeatAll,x0,_doneWithSongs
_RepeatAll
        jsr     InitSongInfo
        clr     a
        rts
_checkDirCount
        move    y:DirectorySongNumber,a      ; directory song number
        move    y:DirectorySongCount,x0      ; directory song count
        cmp     x0,a
        jlt     _messageSongsRemain
_getNextDevice
        clr     a
        move    #Dev0Count,r0                ; point to song counts on dev 0
        move    y:<Const_ffffff,m0                    ; linear
_loop
        move    y:CurrentDirDevId,a0         ; offset
        inc     a
        move    a0,n0
        move    a0,y:CurrentDirDevId         ; update it
        jclr    #MAX_LOGICAL_DEVICES,a0,_continue
        jmp     _doneWithSongs               ; prevent endless loop error
_continue
        move    y:(r0+n0),a                  ; update pointer
        tst     a
        jeq     _loop                        ; try again  if no songs
        move    a,y:DirectorySongCount       ; new device count
        clr     a
        jmp     _messageSongsRemain
_nextRandomSong
        ; use the random buffer to select next song
        ; check  if all songs have played
        move    y:TotalSongsPlayed,x0
        move    y:TotalSongCount,a
        cmp     x0,a                         ;  if TotalSongCount != TotalSongsPlayed
        jne     _moreRandomSongs             ; then there are more songs to play
        clr     a                            ;  else -- all have played, reset
        move    a,y:Shuffle0to23             ; clear for next time
        move    a,y:Shuffle24to47            ; clear for next time
        move    a,y:TotalSongsPlayed
        move    x:ParserCSR,x0
        jclr    #RepeatAll,x0,_doneWithSongs
_moreRandomSongs
        jsr     DecShuffle                   ; randomly select next song
        jmp     _exitChooseNextSong
_doneWithSongs
        jsr     InitSongInfo
        jsr     ParserOpenSong
        jsr     ParserCloseSong
        move    y:<Const_000001,a
        rts                                  ; acc. A = 1 (done with songs)
_messageSongsRemain
        move    y:<Const_000001,x1
        add     x1,a
        move    a,y:DirectorySongNumber      ; inc count in this dir
        move    y:CurrentSongNumber,a
        add     x1,a
        move    a,y:CurrentSongNumber        ; inc overall count
_exitChooseNextSong
        jsr     ParserOpenSong               ; get the song info
        jsr     ParserCloseSong
_rtsChooseNextSong
        clr     a                            ; acc. A = 0 (play more songs)
        rts

;-------------------------------------------------

GetInvSampleRate
; Requires the adpcm decoder to have found the nsamplespersec value.
; TBD :: Sets up the timer count value for the DecoderModuleIsr -- this is based on
; the bitrate.
; Also, sets up the inverse of the bitrate used to create the song times.

        move    y:nsamplespersec,a

        move    #>sr48k,x0
        cmp     x0,a
        jne     _test44_1
        move    #>Inv48k,x1                     ; 1/48000 -- use as multiplier
        jmp     _sampleRateFound
_test44_1
        move    #>sr44_1k,x0
        cmp     x0,a
        jne     _test_32k
        move    #>Inv44_1k,x1
        jmp     _sampleRateFound
_test_32k
        move    #>sr32k,x0
        cmp     x0,a
        jne     _test_24k
        move    #>Inv32k,x1
        jmp     _sampleRateFound
_test_24k
        move    #>sr24k,x0
        cmp     x0,a
        jne     _test_22_05k
        move    #>Inv24k,x1
        jmp     _sampleRateFound
_test_22_05k
        move    #>sr22_05k,x0
        cmp     x0,a
        jne     _test_16k
        move    #>Inv22_05k,x1
        jmp     _sampleRateFound
_test_16k
        move    #>sr16k,x0
        cmp     x0,a
        jne     _test_12k
        move    #>Inv16k,x1
        jmp     _sampleRateFound
_test_12k
        move    #>sr12k,x0
        cmp     x0,a
        jne     _test_11_025k
        move    #>Inv12k,x1
        jmp     _sampleRateFound
_test_11_025k
        move    #>sr11_025k,x0
        cmp     x0,a
        jne     _test_8k
        move    #>Inv11_025k,x1
        jmp     _sampleRateFound
_test_8k
        move    #>sr8k,x0
        cmp     x0,a
        jne     _error
        move    #>Inv8k,x1
        jmp     _sampleRateFound
_error
        error
_sampleRateFound
        move    x1,y:InvSampleRate
        rts

DecSetDacSrr
        move    y:nsamplespersec,x0
        move    #44100,a
        cmp     x0,a
        jne     _sr48k
        move    #DAC44_1kHz,y0
        jmp     _loadDACSRR
_sr48k
        move    #48000,a
        cmp     x0,a
        jne     _sr32k
        move    #DAC48kHz,y0
        jmp     _loadDACSRR
_sr32k
        move    #32000,a
        cmp     x0,a
        jne     _sr24k
        move    #DAC32kHz,y0
        jmp     _loadDACSRR
_sr24k
        move    #24000,a
        cmp     x0,a
        jne     _sr22050
        move    #DAC24kHz,y0
        jmp     _loadDACSRR
_sr22050
        move    #22050,a
        cmp     x0,a
        jne     _sr16k
        move    #DAC22_05kHz,y0
        jmp     _loadDACSRR
_sr16k
        move    #16000,a
        cmp     x0,a
        jne     _sr12k
        move    #DAC16kHz,y0
        jmp     _loadDACSRR
_sr12k
        move    #12000,a
        cmp     x0,a
        jne     _sr11025
        move    #DAC12kHz,y0
        jmp     _loadDACSRR
_sr11025
        move    #11025,a
        cmp     x0,a
        jne     _sr8k
        move    #DAC11_025kHz,y0
        jmp     _loadDACSRR
_sr8k
        move    #8000,a
        cmp     x0,a
        jne     _error
        move    #DAC8kHz,y0
        jmp     _loadDACSRR
_error 
        error
_loadDACSRR
        move    y0,x:HW_DACSRR          ; load current song sample rate into DacSrr
        rts

        endsec






