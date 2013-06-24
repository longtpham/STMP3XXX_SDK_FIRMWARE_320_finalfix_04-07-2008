;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2001
;
; STMP3 File Parser Module for Voice Recordings
;///////////////////////////////////////////////////////////////////////////////

        opt     mex
        include "resource.inc"

        section PARVOIMOD_Parser


;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////
        GLOBAL  ParserVoiceProcessQueue
;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////
       EXTERN   ParserProcessPtr
       EXTERN   SysPostMessage
       EXTERN   SysSetDecoder
       EXTERN   PlayMessage
       EXTERN   MsgDecoderRw
       EXTERN   UserScratchY
       EXTERN   ParserCSR
       EXTERN   ParserVoiceCSR
       EXTERN   DecoderType
       EXTERN   ButtonPressBit
       EXTERN   ParserPlayBit
       EXTERN   ParserRwndBit
       EXTERN   CurrentSongNumber
       EXTERN   ParserFileHandle
       EXTERN   CurrentDirDevId
       EXTERN   CurrentSongName
       EXTERN   ParserLoopCount
       EXTERN   ParserVoiceCSR
       EXTERN	CurrentSongBad		; GDA_20010530

       EXTERN   Byteoffset
       EXTERN   DirectorySongCount
       EXTERN   DirectorySongNumber
       EXTERN   FILEClose
       EXTERN   FILEOpen
       EXTERN   FileRead
       EXTERN   SongByteLengthHigh
       EXTERN   SongByteLengthLow
       EXTERN   SongByteTotalHigh
       EXTERN   SongByteTotalLow
       EXTERN   VoiceExtensionList
       EXTERN   SongTitle
       EXTERN   SysAllLinear
       EXTERN   SysLoadResource
       EXTERN   SysCountSongs
       EXTERN    SysSpeedIncrease
       EXTERN    SysSpeedClockFree
       EXTERN   TotalSongCount
       EXTERN   InvSampleRate
       EXTERN   FindLastVoice
       EXTERN   FindNextVoice
       EXTERN   FindPrevVoice

       EXTERN   DecGetSongPos
       EXTERN   DecSetSongPosZero
       EXTERN   DecSetSongPosEnd
       EXTERN   DecClearSongTime
       EXTERN   DecGetSongTime        
       EXTERN   DecoderForceInit

        ; wav files
       EXTERN     cbsize
       EXTERN     cktypeH
       EXTERN     cktypeL
       EXTERN     data_ckidH
       EXTERN     data_ckidL
       EXTERN     error_flag
       EXTERN     fact_ckidH
       EXTERN     fact_ckidL
       EXTERN     nblockalign
       EXTERN     nchannels
       EXTERN     nsamplespersec
       EXTERN     read_byte_state
       EXTERN     riff_chunksizeH
       EXTERN     riff_chunksizeL
       EXTERN     riff_ckidH
       EXTERN     riff_ckidL
       EXTERN     sample_cntH
       EXTERN     sample_cntL
       EXTERN     total_cntH
       EXTERN     total_cntL
       EXTERN     wave_chunksize
       EXTERN     wave_ckidH
       EXTERN     wave_ckidL
       EXTERN     wbitspersample
       EXTERN     wformattag
       EXTERN     wsamplesperblock
       EXTERN     wave_header_size

       EXTERN     DecoderGetStatus
       EXTERN     MsgParserDeviceEnum
       EXTERN     StopMessage
       EXTERN     FileSeek
       

       EXTERN     ParserFfwdBit
       EXTERN     MsgDecoderFf
       EXTERN     SongInvBitRate
       

       EXTERN    F_lc_u_bP_directory
       EXTERN    F_lc_u_eP_directory
       EXTERN    F_lc_u_bP_sys_cluster

       include    "const.xref"
       include    "parser_voice.xref"
;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////

        nolist
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
        include "adpcmequ.inc"
        include "parserequ.inc"
        include "dacequ.inc"
        include "sysspeed.inc"
        list

        page    132,60,0,0,0

;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////


;///////////////////////////////////////////////////////////////////////////////
;  X memory
;///////////////////////////////////////////////////////////////////////////////
	org     x,"PARVOIMOD_Parser_X":

;///////////////////////////////////////////////////////////////////////////////
;  Y memory
;///////////////////////////////////////////////////////////////////////////////
	org     y,"PARVOIMOD_Parser_Y":

;///////////////////////////////////////////////////////////////////////////////
;   Code
;///////////////////////////////////////////////////////////////////////////////
	org     p,"PARVOIMOD_Parser_overlay_P":

        ; Resource number
        dc      RSRC_PARVOIMOD_CODE

;*******************************************************************************
; ParserInit
;*******************************************************************************

ParserInit
        ; save off stack
        move    #_stackSaved,r6              ; setup the return address
        move    #UserScratchY,r5             ; pointer to the SP local save
        jmp     SaveSystemStack              ; save the hardware stack
_stackSaved
	move	y:UserScratchY,r5	     ; Save SP on stack
	push	r5

        ; parser speed client changes to SPEED_MAX if we're not there or higher
        move    #>SPEED_MAX,x1          
        move    #>SPEED_CLIENT_PARSER,y0
        jsr     SysSpeedIncrease 

        ; init the ModuleTable process pointer
        move    #ParserVoiceProcessQueue,x0
        move    x0,y:ParserProcessPtr

        jsr     InitParserVars               ; init device and song info to zeros
        jsr     InitVoiceVars
        jsr     InitVoiceCount
        jcs     _exitInit                    ; no voice files found  IF carry is set
        jsr     InitVoiceInfo                ; get info on last file created

_exitInit
        ; Free DCLK for other speed clients
        move    #>SPEED_CLIENT_PARSER,y0
        jsr     SysSpeedClockFree

        ; restore stack
_restoreStack
	pop	r5                           ; Recover SP from stack
        move    r5,y:UserScratchY

        move    #UserScratchY,r5             ; pointer to the SP local save
        move    #_stackRestored,r6           ; setup the return address
        jmp     RestoreSystemStack           ; restore the hardware stack
_stackRestored
        rts


InitVoiceVars
        ; Init CurrentSongName to have "/VOICE/" at beginning for absolute path
        move    #CurrentSongName,r0
        move    y:Const_ffffff,m0
        move    #>("OV"++DirectorySeparator),x0
        move    x0,y:(r0)+                   ; "/VO"
        move    #>"ECI",x0
        move    x0,y:(r0)+                   ; "ICE"
        move    #>DirectorySeparator,x0
        move    x0,y:(r0)+                   ; "/"

        ; Init ByteOffset to offset by "/VOICE/" = 7 bytes
        move    #>7,x0
        move    x0,y:Byteoffset
 
        rts

;*******************************************************************************
; ParserVoiceProcessQueue
; r0=Descriptor for this modules message queue (in y memory)
; m0=linear
;*******************************************************************************

ParserVoiceProcessQueue

        ; save off stack
        move    #_stackSaved,r6              ; setup the return address
        move    #UserScratchY,r5             ; pointer to the SP local save
        jmp     SaveSystemStack              ; save the hardware stack
_stackSaved
        move    y:UserScratchY,r5	         ; Save SP on stack
        push    r5

        ; Increase DCLK speed if needed, for parser functions
        push    r0
        push    m0

        ; Parser speed client increases DCLK to SPEED_MAX if we're not higher.
        move    #>SPEED_MAX,x1          
        move    #>SPEED_CLIENT_PARSER,y0
        jsr     SysSpeedIncrease 

        pop     m0
        pop     r0

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

        ; Check  IF the queue is empty
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
;  testpoint                       ; previous VOICEHANG    MSG ID RANGE CHECK

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
        jne     _songsPresent

        jmp     _DeviceEnumerate             ; recount songs
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
; NextSong will have one parameter : 
; 0 = NextSong, 1 = NextSong + PLAY + EOF, 3 = NextSong + PLAY + Button Press
; For Voice:
;  0: Find Next file
;  1: Reset to beginning of current file and stop
;  3: Find Next file and PLAY
        ; Get the argument
        move    y:(r1)+,x0                   ; 0:NextSong , 1:NextSong + PLAY
        push    x0                           ; save off PLAY parameter

        jset    #ButtonPressBit,x0,_findNext
        jclr    #ParserPlayBit,x0,_findNext

_EOFCase
        ; Case 1: Reset to beginning of current file
        ; Originally, we just stop, but now we must check if looping through tracks is necessary.
        jsr     CheckRepeatMode              ; Returns A=0 if no repeat, A != 0 if get next song
        tst     a
        jne     _findNext                    ; Continue through voice files
        pop     x0                           ; balance the stack
        jsr     DecClearSongTime             ; clears total and current time
        jsr     ParserOpenVoice              ; opens voice file, gets file info
        jsr     ParserCloseVoice

        jmp     _exitNextSong

_findNext

        jsr     ParserNextVoice              ; find next voice file and update file info
        pop     x0 ; restore stack balance & PLAY parameter (used below if we checkIfPlay)
        jcs     _exitNextSong                ; If we couldn't go to next song, we definitely can't play it. VOICEHANG1

_checkIfPlay
        jset    #ParserFfwdBit,x0,_checkFfwd ; Only bit #2 is set for forward.       
        jclr    #ParserPlayBit,x0,_exitNextSong

        jsr     DecoderForceInit             ; decoder must init
        move    #PlayMessage,r0              ; post msg to decoder to PLAY
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage        
        jmp     _exitNextSong

_checkFfwd
        jsr     DecoderForceInit             ; decoder must init
        move    #MsgDecoderFf,r0             ; post msg to decoder to FFWD
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

        ; HACK -- this is just so time will be shown to pass as you cross over
        move    #>$41893,x0                   ; Inv128Kbit = 4096/128000
        move    x0,y:SongInvBitRate

        move    x:DecoderType,x0
        jclr    #DECODER_TYPE_ADPCM_IMA,x0,_exitNextSong
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
        move    y:(r1)+,x0                   ; 0:PrevSong , 1:PrevSong + PLAY, 2:PrevSong + Rwnd
        push    x0                           ; save off PLAY parameter

        jsr     ChoosePrevVoice               ; update song selection number

        pop     x0                           ; restore PLAY parameter
        ; if sr.c set, we can't go to prev song, so don't try to play, just exit. VOICEHANG1
        jcs     _exitPrevSong      

        jclr    #ParserPlayBit,x0,_checkRwndBit
        jsr     DecoderForceInit             ; decoder must init
        move    #PlayMessage,r0              ; post msg to decoder to PLAY
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage
        jmp     _exitPrevSong                

_checkRwndBit
        jclr    #ParserRwndBit,x0,_exitPrevSong

        jsr     DecSetSongPosEnd             ; rewind from end of previous song
        move    #MsgDecoderRw,r0             ; post msg to decoder to RWND
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage

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
_exitPrevSong
        jmp     _NextMessage

;-------------------------RepeatFiles Function-------------------------
_Repeat
; The Voice directory will have several repeat modes.
; The Repeat message will have one parameter:
;   0 = Repeat Off
;   1 = Repeat Current Voice File
;   2 = Repeat All Voice Files, continuously
;   4 = Play all files once, beginning with current one
        ; Get the argument
        move    y:(r1)+,x0                   ; 0:RepeatOff , 1:RepeatAll , 2:RepeatOne , 4:LoopOnce
        jset    #RepeatOneFile,x0,_RepeatOne
        jset    #RepeatAllFiles,x0,_RepeatAll
        jset    #LoopOnce,x0,_LoopOnce
_RepeatOff
        bclr    #RepeatOneFile,x:ParserVoiceCSR
        bclr    #RepeatAllFiles,x:ParserVoiceCSR
        bclr    #LoopOnce,x:ParserVoiceCSR
        jmp     _NextMessage
_RepeatOne
        bset    #RepeatOneFile,x:ParserVoiceCSR
        bclr    #RepeatAllFiles,x:ParserVoiceCSR
        bclr    #LoopOnce,x:ParserVoiceCSR
        jmp     _NextMessage
_RepeatAll
        bclr    #RepeatOneFile,x:ParserVoiceCSR
        bset    #RepeatAllFiles,x:ParserVoiceCSR
        bclr    #LoopOnce,x:ParserVoiceCSR
        jmp     _NextMessage
_LoopOnce
        bclr    #RepeatOneFile,x:ParserVoiceCSR
        bclr    #RepeatAllFiles,x:ParserVoiceCSR
        bset    #LoopOnce,x:ParserVoiceCSR
        jmp     _NextMessage

;-------------------------Random Function-------------------------
_Random
; /*********** NOT ACTIVE FOR VOICE MODE ***********/
; Random will have one parameter : 0 = Random Off, 1 = Random On
        ; Get the argument (parameter)
        move    y:(r1)+,a                    ; 0:RandomOff , 1:RandomOn
        jmp     _NextMessage

;-------------------------Stop Message------------------------------
_Stop
; In Voice mode -- a Stop message will cause the current voice track to stop
; playing and will reset to the beginning of the last file created
        jsr     InitVoiceInfo
        jmp     _NextMessage

;-------------------------Device Enumerate---------------------------
_DeviceEnumerate
; This message is sent when a device (card) has been removed from (or inserted into) the player.
; This will cause the current song ( IF playing) to stop and the Parser will re-init
; the song count for a new count value.

        jsr     DecoderGetStatus             ; returns DecoderSR in x0
        jset    #Stop,x0,_enumerate
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
        jsr ParserInit
        jmp _NextMessage

;-------------------------Set Current Song---------------------------
_SetCurrentSong
        jmp     _NextMessage
;-------------------------Invalid Message---------------------------
_InvalidMessageID

        debugOrContinue    ; If DEBUG build, execute debug instruction. VOICEHANG

        ; RETAIL build falls through after invalid msg Rx attempting to goto next message.
        ; This prevents RETAIL crash which previously occurred on invalid message ID Rx.

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
        ; Check  IF head pointer equals the tail pointer
        cmp     a,b
        jne     _ProcessMessage

        ; Update the buffer descriptor with the new tail pointer
        move    #>BD_TailPtr,n0
        nop
        move    b1,y:(r0+n0)

        ; Free DCLK for other speed clients
        move    #>SPEED_CLIENT_PARSER,y0
        jsr     SysSpeedClockFree

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
        nop
        rts

;-----------------------------------------------------------------
; InitVoiceCount
; Counts the number of VOICE files in disk0/VOICE/ directory.
; Returns:
;            Voice Files Found:
;                    Carry bit cleared
;                    Number of files in y:TotalSongCount
;            No Voice Files Found:
;                    Carry bit set
;                    Zero in y:TotalSongCount
;-----------------------------------------------------------------
InitVoiceCount
        ; Setup arguments for calling SysLoadResource
        move    #>RSRC_DIRECTORY_P_CODE,x0      ; Resource number of the applett
        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
        move    #>F_lc_u_bP_directory,y0     ; Address of the application area
        move    #>(F_lc_u_eP_directory-F_lc_u_bP_directory)*3,y1 ; Size of the application area
        move    #TARGET_MEM_P,a1                ; Application area is in P memory
    
        ; Load the directory applett into the application area
        jsr     SysLoadResource

        clr     a                            ; a2 = dev id = 0
        move    a,y:CurrentDirDevId          ; all voice files store on disk0
        move    y:CurrentDirDevId,a2
        move    #>CurrentSongName,a1          ; "/VOICE/"
        move    #TARGET_MEM_Y,x0
        or      x0,a                         ; A2 = dev# , A1 = memspace/dir path ptr , A0 = 0
        move    #VoiceExtensionList,r0       ; R0 = ptr to extension list in Y memory
        jsr     SysCountSongs                ; VOICEHANG1 note: TODO: change to not count if filename is unsupported.
        jcs     _noSongs                     ; error  IF carry flag == 1
        tst     a
        jne     _CountFound                  ; see  IF no songs found
_noSongs
        clr     a                            ;  IF error == count = 0
        move    a,y:TotalSongCount
        move    a,y:ParserLoopCount
        move    a,y:CurrentSongNumber
        ; Print No Files Present for SongTitle
        move    #>SongTitle,r0
        move    #$206F4E,x1                  ; " oN"
        move    x1,y:(r0)+
        move    #$6C6946,x1                  ; "liF"
        move    x1,y:(r0)+
        move    #$007365,x1                  ; " se"
        move    x1,y:(r0)+
        bset    #HW_SR_C_BITPOS,sr                 ; return with carry set  IF not songs found
        rts
_CountFound
        ; update song count
        move    a,y:TotalSongCount
        bclr    #HW_SR_C_BITPOS,sr                 ; return with carry clear  IF songs found
        rts

;-------------------------------------------------
ParserLastVoice
; For VOICE files :: resets to the latest voice file created
; Calls FindLastVoice function -- which will return the file name
; of the last voice file created.
; Returns with SR.carry set on error.

        ; Setup arguments for calling SysLoadResource
        move    #>RSRC_DIRECTORY_P_CODE,x0      ; Resource number of the applett
        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
        move    #>F_lc_u_bP_directory,y0     ; Address of the application area
        move    #>(F_lc_u_eP_directory-F_lc_u_bP_directory)*3,y1 ; Size of the application area
        move    #TARGET_MEM_P,a1                ; Application area is in P memory
    
        ; Load the directory applett into the application area
        jsr     SysLoadResource

        ; Call FindLastVoice
        ; Input Parameters:
        ; A2 = Device Number
        ; A1 = Complex ptr to dir path
        ; B1 = Complex ptr to file name buffer
        ; X1 = Byte offset within file name buffer
        ; Output:
        ;  IF successful: Carry clear, song name filled in
        ;  IF unsuccessful: Carry Set, A1 = error code

        clr     a      #TARGET_MEM_Y,x0      ; memory space identifier (Y RAM)
        move    y:Byteoffset,x1              ; X1 = byte offset (7)
        move    y:CurrentDirDevId,a2         ; card number for current directory
        move    #CurrentSongName,a1          ; base address of directory path
        or      x0,a                         ; A2 = dev#, A1=memspace/dir path, A0 = 0

        clr     b
        move    #CurrentSongName,b1          ; base address of song name (use ByteOffset to skip DIR)
        or      x0,b                         ; B2 = index, B1 = memspace/song ptr, B0=0
        jsr     FindLastVoice ; returns song name in y:CurrentSongName offset by ByteOffset. VOICEHANG1 review called subr.
        jcs     _exitParserLastVoice        ; Exit with carry bit set. no files were found.

_nameFound
        ; update song number
        move    y:TotalSongCount,x0          ; set to last file
        move    x0,y:DirectorySongNumber
        move    x0,y:CurrentSongNumber

_exitParserLastVoice
        nop
        
        rts

;-------------------------------------------------
ParserNextVoice
; For VOICE files :: finds the next voice file after the current
; Calls FindNextVoice function -- which will return the file name
; of the next voice file.
; Returns with Status Register carry bit set if error.
        
        ; Setup arguments for calling SysLoadResource
        move    #>RSRC_DIRECTORY_P_CODE,x0      ; Resource number of the applett
        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
        move    #>F_lc_u_bP_directory,y0     ; Address of the application area
        move    #>(F_lc_u_eP_directory-F_lc_u_bP_directory)*3,y1 ; Size of the application area
        move    #TARGET_MEM_P,a1                ; Application area is in P memory
    
        ; Load the directory applett into the application area
        jsr     SysLoadResource

        ; Call FindNextVoice
        ; Input Parameters:
        ; A2 = Device Number
        ; A1 = Complex ptr to dir path
        ; B1 = Complex ptr to file name buffer
        ; X1 = Byte offset within file name buffer
        ; Output:
        ;  IF successful: Carry clear, song name filled in
        ;  IF unsuccessful: Carry Set, A1 = error code

        clr     a      #TARGET_MEM_Y,x0      ; memory space identifier (Y RAM)
        move    y:Byteoffset,x1              ; X1 = byte offset (7)
        move    y:CurrentDirDevId,a2         ; card number for current directory
        move    #CurrentSongName,a1          ; base address of directory path
        or      x0,a                         ; A2 = dev#, A1=memspace/dir path, A0 = 0

        clr     b
        move    #CurrentSongName,b1 ; base address of song name (use ByteOffset to skip DIR)
        or      x0,b                ; B2 = index, B1 = memspace/song ptr, B0=0
;    testpoint 

        jsr     FindNextVoice 
        ; Returned song name in y:CurrentSongName offset by ByteOffset  VOICEHANG1
        ; When SR.C set, song name is not found or is invalid, so we can't go to next song.

        ;debugOrContinue  debug instruction if DEBUG build, else does nothing.

        ;Check for ERROR: song name not found.
        jcs     _exitParserNextVoice   

        ; Issue: NO ERROR RETURNED BY FindNextVoice WHEN UNSUPPORTED WAV FILENAME NEXT, See resulting counter inc below.
_nameFound

        ; update song number
        move    y:DirectorySongNumber,a      ; which voice file is current
        move    y:<Const_000001,x1
        add     x1,a                         ; increment to next
        move    y:TotalSongCount,x0          ; total number of voice files
        cmp     x0,a
        jle     _storeNewNumber
        move    x1,a                         ; reset to first file for wraparound
_storeNewNumber
        move    a,y:DirectorySongNumber      ; inc count in this dir
        move    a,y:CurrentSongNumber        ; inc overall count

        jsr     ParserOpenVoice              ; opens voice file, gets file info
        jsr     ParserCloseVoice

_exitParserNextVoice
        nop
        
        rts

;-------------------------------------------------
ParserPrevVoice
; For VOICE files :: finds the prev voice file from the current voice file.
; Calls FindPrevVoice function -- which will return the file name
; of the previous voice file.
; This routine returns with Status Register carry bit set if error.

        ; Setup arguments for calling SysLoadResource
        move    #>RSRC_DIRECTORY_P_CODE,x0      ; Resource number of the applett
        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
        move    #>F_lc_u_bP_directory,y0     ; Address of the application area
        move    #>(F_lc_u_eP_directory-F_lc_u_bP_directory)*3,y1 ; Size of the application area
        move    #TARGET_MEM_P,a1                ; Application area is in P memory
    
        ; Load the directory applett into the application area
        jsr     SysLoadResource

        ; Call FindPrevVoice
        ; Input Parameters:
        ; A2 = Device Number
        ; A1 = Complex ptr to dir path
        ; B1 = Complex ptr to file name buffer
        ; X1 = Byte offset within file name buffer
        ; Output:
        ;  IF successful: Carry clear, song name filled in
        ;  IF unsuccessful: Carry Set, A1 = error code

        clr     a      #TARGET_MEM_Y,x0      ; memory space identifier (Y RAM)
        move    y:Byteoffset,x1              ; X1 = byte offset (7)
        move    y:CurrentDirDevId,a2         ; card number for current directory
        move    #CurrentSongName,a1          ; base address of directory path
        or      x0,a                         ; A2 = dev#, A1=memspace/dir path, A0 = 0

        clr     b
        move    #CurrentSongName,b1          ; base address of song name (use ByteOffset to skip DIR)
        or      x0,b                         ; B2 = index, B1 = memspace/song ptr, B0=0
        jsr     FindPrevVoice                ; returns song name in y:CurrentSongName offset by ByteOffset
        ; If song name not found or is invalid, we can't go to prev song. 
        ; SR.carry is set on return.
        ;debugOrContinue  ; check sr.c error, song name not found. VOICEHANG1
        jcs     _exit        

_nameFound
        jsr     ParserOpenVoice              ; opens voice file, gets file info
        jsr     ParserCloseVoice
_exit
        nop
        
        rts

;-------------------------------------------------
InitVoiceInfo
; Reset to last file created
; Init the info for that file
        jsr     InitVoiceVars
        jsr     ParserCountSongs             ; returns value in y:DirectorySongCount
        jsr     ParserLastVoice              ; finds last voice file created
        jsr     DecClearSongTime             ; clears total and current time
        jsr     ParserOpenVoice              ; opens voice file, gets file info
        jsr     ParserCloseVoice
        rts

;-----------------------------------------------------------------
; ParserCountSongs
; Inputs: y:CurrentDirDevId has device id
;         y:CurrentSongName has the current directory to play from
; This function will count the songs in the current directory.
;-----------------------------------------------------------------
ParserCountSongs
        ; Setup arguments for calling SysLoadResource
        move    #>RSRC_DIRECTORY_P_CODE,x0      ; Resource number of the applett
        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
        move    #>F_lc_u_bP_directory,y0     ; Address of the application area
        move    #>(F_lc_u_eP_directory-F_lc_u_bP_directory)*3,y1 ; Size of the application area
        move    #TARGET_MEM_P,a1                ; Application area is in P memory
    
        ; Load the directory applett into the application area
        jsr     SysLoadResource

        ;-------------------------------------------------------------
        ; Count the Songs in the Directory (SysCountSongs)
        ;-------------------------------------------------------------
        ; Inputs:   A2 = Device number  A1 = Complex ptr to directory path
        ;           R0 = Pointer to an extension list (in y memory)
        ; Output:
        ;           A1 = File Count  IF successful (error code  IF not)
        ;-------------------------------------------------------------
        clr     a
        move    y:CurrentDirDevId,a2
        move    #>CurrentSongName,a1
        move    #TARGET_MEM_Y,x0
        or      x0,a                    ; A2 = dev# , A1 = memspace/dir path ptr , A0 = 0
        move    #VoiceExtensionList,r0  ; R0 = ptr to extension list in Y memory
        jsr     SysCountSongs
        jcs     _noSongs                     ; error  IF carry flag == 1
        tst     a
        jne     _CountFound                  ; see  IF no songs found
_noSongs
        clr     a                            ;  IF error == count = 0
_CountFound
        move    a1,y:DirectorySongCount ; total number of songs in directory
        rts

;-----------------------------------------------------------------
; ParserOpenVoice
;-----------------------------------------------------------------
ParserOpenVoice
; ASSUMES file name has been found and absolute path is stored in y:CurrentSongName

        jsr     SetVoiceDecoder              ; ensure the ADPCM decoder is used.

	clr	a			     ; GDA_20010530
	move	a,y:CurrentSongBad

        ;-------------------------------------------------------------
        ; Open the Song (FILEOpen)
        ;-------------------------------------------------------------
        ; Inputs:   A2 = Device number  A1 = Complex ptr to file name buffer
        ;           A0 = Open attributes
        ; Output:
        ;           A1 = File handle ( IF successful)
        ;           A1 = Error code  ( IF failed)
        ;-------------------------------------------------------------
        jsr     SysAllLinear
        clr     a      #TARGET_MEM_Y,x0      ; memory space identifier (Y RAM)
        move    y:CurrentDirDevId,a2         ; card number for current directory
        move    #CurrentSongName,a1          ; base address of song name (absolute path)
        or      x0,a                         ; A2 = dev#, A1=memspace/file name buffer, A0=0
        move    #>READMODE,a0                ; A0 = file open attribute (Read Only)
        jsr     FILEOpen              
        jcc     _fileOpened
        error    ; ERROR - file did not open.  
        ; Reset system or debug halt for RETAIL or DEBUG build respectively.        
        ; Future ver should handle this error and continue instead of quitting.
        ; We might set y:CurrentSongBad which prevents decode attempts and 
        ; 1) Since FILEOpen failed, don't call FILEClose. (expectation)   OR
        ; 2) When FILEOpen failed, is FILEClose still needed & will it behave?

_fileOpened
        move    a1,y:ParserFileHandle        ; store the handle for the file

        jsr     FindAdpcmInfo

        ;-------------------------------------------------------------
        ; Seek to Zero (init) the Song (FileSeek)
        ;-------------------------------------------------------------
        ; Inputs:   A2 = Handle  A1/A0 = relative offset
        ;-------------------------------------------------------------
        jsr     SysAllLinear
        clr     a                            ; offset of 0
        move    y:ParserFileHandle,a2
        jsr     FileSeek

        jsr     DecSetSongPosZero            ; reset to beginning of song
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

        ;-------------------------------------------------------------
        ; Read from the Song (FileRead)
        ;-------------------------------------------------------------
        ; Inputs:   A2 = File handle  A1 = Number of bytes to read in bits 0-15
        ;           A0 = Memory space identifier
        ;           R0 = Buffer pointer to read into
        ;           M0 = Buffer modulo
        ; Output:
        ;           A1 = Number of bytes read ( IF successful)
        ;           A1 = Error code  ( IF failed)
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
; GDA_20010530        error                  ;***temporary***check for errors*****
	bset	#0,y:CurrentSongBad
	jmp	_storeFileName

_storeInfo

        move    #UserScratchY,r0             ; R0 = Buffer pointer to read into
        jsr     ParserUnpackWaveHeader

        jsr     DecSetDacSrr                 ; set the dac sample rate
        jsr     GetInvSampleRate             ; load 1/sample rate into y:InvSampleRate
_storeFileName
        ; store the file name as the song name
        clr     a     #SongTitle,r4          ; used  IF ID3v1Tag exists
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
        move     #0,x0                       ; return with x0 cleared!!!
        move     y:<Const_ffffff,m0
        rts

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

  ;  IF not yet "DATA", check for "FACT"
  move  y:fact_ckidH,A
  cmp    x1,A
  jeq    _Fact_section

;===========================================================
  ;  IF not "DATA" nor "FACT", skip UNKNOWN chunk
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
; SetVoiceDecoder
; This function sets up for the ADPCM decoder to be loaded.
;-----------------------------------------------------------------
SetVoiceDecoder
        move    y:Const_000000,x0       ; use x0 for Decoder Type
        bset    #DECODER_TYPE_ADPCM_IMA,x0
        move    x0,x:DecoderType
        push    x0
        jsr     SysSetDecoder
        pop     x0                      ; preserve which decoder is in
        rts

;-----------------------------------------------------------------
; ParserCloseVoice
; This function will use the ParserFileHandle to close the file.
;-----------------------------------------------------------------
ParserCloseVoice
        ;-------------------------------------------------------------
        ; Close the Song (FILEClose)
        ;-------------------------------------------------------------
        ; Inputs:   A2 = File handle
        ; Output:
        ;           A1 = File handle ( IF successful)
        ;           A1 = Error code  ( IF failed)
        ;-------------------------------------------------------------
        jsr     SysAllLinear
        clr     a
        move    y:ParserFileHandle,a2        ; pass the file handle to the function
        jsr     FILEClose
        jcc     _fileClosed
        error                                ; ERROR - file did not close (TBD -- ADD CODE!!)
_fileClosed
_exit
        rts

;-----------------------------------------------------------------
ChoosePrevVoice
; Output: CurrentSongNumber is updated
;  IF > 5 seconds have played, resets to beginning of current file.
;  IF <= 5 seconds have played, resets to previously played file.

        jsr     DecGetSongTime               ; Returns: A1 = Song Minutes Elapsed, A0 = Song Seconds Elapsed

_checkMinutes
        move    a0,x0                        ; save off seconds
        move    y:<Const_000000,a0           ; clear out low bits
        tst     a                            ;  IF minutes > 0, reset to beginning of file
        jgt     _stayOnCurrentVoice
        move    x0,a1                        ; a1 = seconds elapsed
        move    #>DECODER_SONG_SEC_THRESHOLD,x0
        cmp     x0,a
        jle     _gotoPreviousVoice           ;  IF seconds <= threshold, go to previous file


_stayOnCurrentVoice
        jsr     DecSetSongPosZero
        rts
        ;----------
_gotoPreviousVoice
        ; check  IF need to switch devices
        move    y:DirectorySongNumber,a      ; decrement directory song number
        ;Save y:CurrentSongNumber & y:DirectorySongNumber for restore if needed. VOICEHANG error handling added.
        push    a1 
        move    y:CurrentSongNumber,b        ; decrement overall song number
        push    b1
        move    y:<Const_000001,x1
        sub     x1,a
        sub     x1,b
        tst     a     b,y:CurrentSongNumber  ; be sure not at song "0" (start at 1)
        jle     _wraparound
        
_noWraparound
        move    a,y:DirectorySongNumber
        jsr     ParserPrevVoice
        jcc     _noErr

        ; Error on ParserPrevVoice call, restore current song and dir song number
        pop     b1
        move    b1,y:CurrentSongNumber
        pop     a1                          ; Stack balanced
        move    a1,y:DirectorySongNumber
        jmp     _noErrFinished
_noErr        
        pop     b1   
        pop     a1                          ; Stack balanced  
_noErrFinished
        jsr     ParserOpenVoice
        jmp     _exitChoosePrevVoice

_wraparound
        ; save y:CurrentSongNumber & y:DirectorySongNumber for restore if needed. 
        push    x0
        move    y:CurrentSongNumber,x0
        push    x0
        move    y:DirectorySongNumber,x0
        push    x0

        ; find last song
        ; set number to last        
        move    y:TotalSongCount,x0
        move    x0,y:CurrentSongNumber       ; set to last song
        move    x0,y:DirectorySongNumber     ; set to last song in directory
        jsr     ParserLastVoice
        jcc     _noRestore

        ; Restore these to handle errored ParserLastVoice call
        pop     x0
        move    x0,y:DirectorySongNumber
        pop     x0
        move    x0,y:CurrentSongNumber
        pop     x0
        
        pop     b1
        pop     a1                           ; Stack balanced
        jmp     _restoreDone

_noRestore
        pop     x0                           
        pop     x0
        pop     x0
        pop     b1
        pop     a1                           ; Stack balanced

_restoreDone        
        jsr     ParserOpenVoice
        ;----------      
_exitChoosePrevVoice
        jsr     ParserCloseVoice 
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

CheckRepeatMode
; Checks if it is necessary to continue playing tracks.
; Returns A = 0 if done, A != 0 if continue playing
        move    x:ParserVoiceCSR,x0
        jset    #RepeatOneFile,x0,_RepeatOne
        jset    #RepeatAllFiles,x0,_RepeatAll
        jset    #LoopOnce,x0,_LoopOnce
_NoRepeat
        clr     a
        rts
_RepeatOne
        jsr     DecoderForceInit             ; decoder must init
        move    #PlayMessage,r0              ; post msg to decoder to PLAY
        move    y:<Const_ffffff,m0
        jsr     SysPostMessage
        clr     a
        rts
_RepeatAll
        move    #>1,a                       ; return A != 0 to continue playing
        rts
_LoopOnce
        move    #>1,x0
        move    y:ParserLoopCount,a
        add     x0,a
        move    a,y:ParserLoopCount
        move    y:TotalSongCount,x0
        cmp     x0,a
        jge     _done
        move    #>1,a
        rts
_done
        jsr     ParserNextVoice             ; load up the next song for display
        clr     a                           ;    but don't play it.
        move    a,y:ParserLoopCount
        rts

        endsec




