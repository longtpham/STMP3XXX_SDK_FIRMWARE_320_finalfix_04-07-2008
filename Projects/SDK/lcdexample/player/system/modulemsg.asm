;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2001
;
;  File        : modulemsg.asm
;  Description : Message queue descriptor and module table declaration
;                for project
;///////////////////////////////////////////////////////////////////////////////

    section SYSPLAYER_ModuleMsg

;///////////////////////////////////////////////////////////////////////////////
;  Macros
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "sysmacro.asm"
    list

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "sysequ.inc"
    include "msgequ.inc"
    include "resource.inc"
    list

;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

    ;Variables in Y
    global  DecoderQueueDescriptor
    global  CaptureQueueDescriptor
    global  FDisplayQueueDescriptor
    global  MixerQueueDescriptor
    global  SystemQueueDescriptor
    global  Fglb_SoftTimerQueueDescriptor
    if (!@def('REMOVE_FM'))
    global  TunerQueueDescriptor
    endif
	if @def('USE_PLAYLIST3')
    global  MusicLibPlayQueueDescriptor
    global  MusicLibBrowseQueueDescriptor
    global  MusicLibPlayAllNextQueueDescriptor
    global  MusicLibPlayAllPrevQueueDescriptor
    global  MusicLibShuffleQueueDescriptor

    global  MusicLibVoiceQueueDescriptor
    global  MusicLibVoicePlayQueueDescriptor
	    if @def('MMC')
    global  MusicLibMergeQueueDescriptor
    	endif
	endif	; if @def('USE_PLAYLIST3')
	if @def('USE_PLAYLIST5')
	global  PL5PlaybackModuleQueueDescriptor
	global  PL5BrowseModuleQueueDescriptor
	endif   ; if @def('USE_PLAYLIST5')
    global  DecoderProcessPtr
    global  EncoderProcessPtr
    global  DecoderResourcePtr
	global  FDecoderResourcePtr
    global  GeqQueueDescriptor
    global  EncoderResourcePtr
    global  MenuResourcePtr
    global  FMenuResourcePtr

    global  CaptureTailPtr
    global  CaptureHeadPtr

    global  ModuleTable
    global  ModuleTableLed
    global  ModuleRouterTable

    ;menu globals
    global  MenuQueueDescriptor
    global  MenuTableMenuPtr
    global  MenuProcessEventRoutinePtr

    global  MenuTimerHigh
    global  MenuTimerLow
    global  MenuQueueModulo
    global  MenuTailPtr
    global  MenuHeadPtr
    global  MenuEventWaitMask
    global  MenuEventSignalMask

    ;Equates
    global  MODULE_NUM_DECODER
    global  MODULE_NUM_CAPTURE
    global  MODULE_NUM_LED
    global  MODULE_NUM_MIXER
    global  MODULE_NUM_SYSTEM
    if (!@def('REMOVE_FM'))
    global  MODULE_NUM_FMTUNER
    endif
    global  MODULE_NUM_MENU
    global  MODULE_NUM_GEQ
	if @def('USE_PLAYLIST3')
    global  MODULE_NUM_MUSICLIB_PLAY
    global  MODULE_NUM_MUSICLIB_PLAYALLNEXT
    global  MODULE_NUM_MUSICLIB_PLAYALLPREV
    global  MODULE_NUM_MUSICLIB_SHUFFLE
    global  MODULE_NUM_MUSICLIB_BROWSE
    global  MODULE_NUM_MUSICLIB_VOICE
	if @def('MMC')
    global  MODULE_NUM_MUSICLIB_MERGE
	endif   ; if @def('MMC')
	endif	; if @def('USE_PLAYLIST3')
	if @def('USE_PLAYLIST5')
	global  MODULE_NUM_PL5_PLAYBACK
	global  MODULE_NUM_PL5_BROWSE
	endif   ; if @def('USE_PLAYLIST5')
    global  MODULE_COUNT
    global  MODULE_ENTRY_SIZE
    global  MODULE_TABLE_MODULO
    global  ROUTER_TABLE_SIZE


;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////
    nolist
    include "bufdesc.xref"
    list

    extern  EmptyFunction
    extern  FDisplayModuleInit
    extern  FDisplayProcessQueue
    extern  MixerInit
    extern  MixerProcessQueue
    extern  FSystemModuleInit
    extern  FSystemModuleProcessQueue
    extern  InitUserTask
    extern  UserTaskKernel
    extern  FSoftTimerModuleProcessQueue
    extern  FSoftTimerModuleInit
    extern  GeqOverlayModule
    extern  GeqInit
;Tuner externs
 if (!@def('REMOVE_FM'))
    extern FTunerModuleInit
    extern FTunerModuleProcessQueue
 endif  ; if !@def('REMOVE_FM'))
	if @def('USE_PLAYLIST3')
    extern FMusicLibPlayModuleInit
    extern FMusicLibPlayModuleProcessQueue
    extern FMusicLibBrowseModuleInit
    extern FMusicLibBrowseModuleProcessQueue
    extern FMusicLibPlayAllNextModuleInit
    extern FMusicLibPlayAllNextModuleProcessQueue
    extern FMusicLibPlayAllPrevModuleInit
    extern FMusicLibPlayAllPrevModuleProcessQueue
    extern FMusicLibShuffleModuleInit
    extern FMusicLibShuffleModuleProcessQueue
    extern FMusicLibVoiceModuleInit
    extern FMusicLibVoiceModuleProcessQueue
    extern FMusicLibVoicePlayModuleInit
    extern FMusicLibVoicePlayModuleProcessQueue
	if @def('MMC')
    extern FMusicLibMergeModuleInit
    extern FMusicLibMergeModuleProcessQueue
	endif   ; if @def('MMC')
	endif	; if @def('USE_PLAYLIST3')
    if @def('USE_PLAYLIST5')
    extern FPL5PlaybackModuleInit
    extern FPL5PlaybackModuleProcessQueue
    extern FPL5BrowseModuleInit
    extern FPL5BrowseModuleProcessQueue
    endif   ; if @def('USE_PLAYLIST5')
;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////

    ; Message Queue Size
MSG_QUEUE_SIZE_DECODER      equ     32
MSG_QUEUE_MODULO_DECODER    equ     MSG_QUEUE_SIZE_DECODER-1

MSG_QUEUE_SIZE_CAPTURE      equ     32
MSG_QUEUE_MODULO_CAPTURE    equ     MSG_QUEUE_SIZE_CAPTURE-1

; Ken 2005/7/28 03:51
	if @def('USE_PLAYLIST3')
MSG_QUEUE_SIZE_DISPLAY          equ     144
	else
MSG_QUEUE_SIZE_DISPLAY          equ     160
	endif
; Ken 2005/7/28 03:51
MSG_QUEUE_MODULO_DISPLAY        equ     MSG_QUEUE_SIZE_DISPLAY-1

MSG_QUEUE_SIZE_MIXER        equ     32
MSG_QUEUE_MODULO_MIXER      equ     MSG_QUEUE_SIZE_MIXER-1

MSG_QUEUE_SIZE_SYSTEM       equ     32
MSG_QUEUE_MODULO_SYSTEM     equ     MSG_QUEUE_SIZE_SYSTEM-1

MSG_QUEUE_SIZE_MENU         equ     64
MSG_QUEUE_MODULO_MENU       equ     MSG_QUEUE_SIZE_MENU-1

MSG_QUEUE_SIZE_GEQ  	    equ     32
MSG_QUEUE_MODULO_GEQ        equ     MSG_QUEUE_SIZE_GEQ-1

    if !@def(MSG_QUEUE_SIZE_TUNER)
MSG_QUEUE_SIZE_TUNER equ 32
    endif

    if !@def(MSG_QUEUE_SIZE_SOFT_TIMERS)
MSG_QUEUE_SIZE_SOFT_TIMERS equ 32
    endif

	if @def('USE_PLAYLIST3')
MSG_QUEUE_SIZE_MUSICLIB_PLAY  	    equ     32
MSG_QUEUE_MODULO_MUSICLIB_PLAY         equ     MSG_QUEUE_SIZE_MUSICLIB_PLAY-1

MSG_QUEUE_SIZE_MUSICLIB_BROWSE  	    equ     32
MSG_QUEUE_MODULO_MUSICLIB_BROWSE         equ     MSG_QUEUE_SIZE_MUSICLIB_BROWSE-1
MSG_QUEUE_SIZE_MUSICLIB_PLAYALLNEXT  	    equ     16
MSG_QUEUE_MODULO_MUSICLIB_PLAYALLNEXT         equ     MSG_QUEUE_SIZE_MUSICLIB_PLAYALLNEXT-1
MSG_QUEUE_SIZE_MUSICLIB_PLAYALLPREV  	    equ     16
MSG_QUEUE_MODULO_MUSICLIB_PLAYALLPREV         equ     MSG_QUEUE_SIZE_MUSICLIB_PLAYALLPREV-1
MSG_QUEUE_SIZE_MUSICLIB_SHUFFLE  	    equ     16
MSG_QUEUE_MODULO_MUSICLIB_SHUFFLE         equ     MSG_QUEUE_SIZE_MUSICLIB_SHUFFLE-1
MSG_QUEUE_SIZE_MUSICLIB_VOICE  	    equ     32
MSG_QUEUE_MODULO_MUSICLIB_VOICE         equ     MSG_QUEUE_SIZE_MUSICLIB_VOICE-1
MSG_QUEUE_SIZE_MUSICLIB_VOICEPLAY  	    equ     32
MSG_QUEUE_MODULO_MUSICLIB_VOICEPLAY         equ     MSG_QUEUE_SIZE_MUSICLIB_VOICEPLAY-1
	if @def('MMC')
MSG_QUEUE_SIZE_MUSICLIB_MERGE  	    equ     32
MSG_QUEUE_MODULO_MUSICLIB_MERGE         equ     MSG_QUEUE_SIZE_MUSICLIB_MERGE-1
	endif   ; if @def('MMC')
	endif	; if @def('USE_PLAYLIST3')
	if @def('USE_PLAYLIST5')
MSG_QUEUE_SIZE_PL5_PLAYBACK  	    equ     32
MSG_QUEUE_MODULO_PL5_PLAYBACK       equ     MSG_QUEUE_SIZE_PL5_PLAYBACK-1
MSG_QUEUE_SIZE_PL5_BROWSE  	        equ     32
MSG_QUEUE_MODULO_PL5_BROWSE         equ     MSG_QUEUE_SIZE_PL5_BROWSE-1

	endif   ; if @def('USE_PLAYLIST5')
;///////////////////////////////////////////////////////////////////////////////
;  X memory
;///////////////////////////////////////////////////////////////////////////////


;///////////////////////////////////////////////////////////////////////////////
;  Y memory
;///////////////////////////////////////////////////////////////////////////////

    ; Message Queues
    org     y,"SYSPLAYER_MsgQueue_decoder_Y":
DecoderMsgQueue         dsm     MSG_QUEUE_SIZE_DECODER

    org     y,"SYSPLAYER_MsgQueue_encoder_Y":
CaptureMsgQueue         dsm     MSG_QUEUE_SIZE_CAPTURE

    org     y,"SYSPLAYER_MsgQueue_display_Y":
DisplayMsgQueue             dsm     MSG_QUEUE_SIZE_DISPLAY

    org     y,"SYSPLAYER_MsgQueue_mixer_Y":
MixerMsgQueue           dsm     MSG_QUEUE_SIZE_MIXER

    org     y,"SYSPLAYER_MsgQueue_system_Y":
SystemMsgQueue          dsm     MSG_QUEUE_SIZE_SYSTEM

    org     y,"SYSPLAYER_MsgQueue_menu_Y":
MenuMsgQueue            dsm     MSG_QUEUE_SIZE_MENU

    org     y,"SYSPLAYER_MsgQueue_geq_Y":
GeqMsgQueue            dsm     MSG_QUEUE_SIZE_GEQ

    org     y,"SYSPLAYER_MsgQueue_FMTuner_Y":
TunerMsgQueue       dsm     MSG_QUEUE_SIZE_TUNER

    org     y,"SYSPLAYER_MsgQueue_SoftTimer_Y":
SoftTimerMsgQueue       dsm     MSG_QUEUE_SIZE_SOFT_TIMERS
	if @def('USE_PLAYLIST3')
    org     y,"SYSPLAYER_MsgQueue_MusicLib_Browse_Y":
MusicLibBrowseMsgQueue       dsm     MSG_QUEUE_SIZE_MUSICLIB_BROWSE

    org     y,"SYSPLAYER_MsgQueue_MusicLib_Play_Y":
MusicLibPlayMsgQueue       dsm     MSG_QUEUE_SIZE_MUSICLIB_PLAY

    org     y,"SYSPLAYER_MsgQueue_MusicLib_PlayAllNext_Y":
MusicLibPlayAllNextMsgQueue       dsm     MSG_QUEUE_SIZE_MUSICLIB_PLAYALLNEXT

    org     y,"SYSPLAYER_MsgQueue_MusicLib_PlayAllPrev_Y":
MusicLibPlayAllPrevMsgQueue       dsm     MSG_QUEUE_SIZE_MUSICLIB_PLAYALLPREV

    org     y,"SYSPLAYER_MsgQueue_MusicLib_Shuffle_Y":
MusicLibShuffleMsgQueue       dsm     MSG_QUEUE_SIZE_MUSICLIB_SHUFFLE

    org     y,"SYSPLAYER_MsgQueue_MusicLib_Voice_Y":
MusicLibVoiceMsgQueue       dsm     MSG_QUEUE_SIZE_MUSICLIB_VOICE
	if @def('MMC')
    org     y,"SYSPLAYER_MsgQueue_MusicLib_Merge_Y":
MusicLibMergeMsgQueue       dsm     MSG_QUEUE_SIZE_MUSICLIB_MERGE
	endif
    org     y,"SYSPLAYER_MsgQueue_MusicLib_VoicePlay_Y":
MusicLibVoicePlayMsgQueue       dsm     MSG_QUEUE_SIZE_MUSICLIB_VOICEPLAY
	endif	; if @def('USE_PLAYLIST3')
    if @def('USE_PLAYLIST5')
    org     y,"SYSPLAYER_MsgQueue_Playlist5_Playback_Y":
PL5PlaybackMsgQueue       dsm     MSG_QUEUE_SIZE_PL5_PLAYBACK
    org     y,"SYSPLAYER_MsgQueue_Playlist5_Browse_Y":
PL5BrowseMsgQueue       dsm     MSG_QUEUE_SIZE_PL5_BROWSE
    endif   ; if @def('USE_PLAYLIST5')
;----------------------------------------------------


;----------------------------------------------------
    org     y,"SYSPLAYER_ModuleMsg_Y":
ModuleRouterTable
                        dc      MSG_TYPE_DECODER|DecoderQueueDescriptor
                        dc      MODULE_NUM_DECODER
                        dc      MSG_TYPE_ENCODER|CaptureQueueDescriptor
                        dc      MODULE_NUM_CAPTURE
                        dc      MSG_TYPE_PARSER|MenuQueueDescriptor
                        dc      MODULE_NUM_MENU
                        dc      MSG_TYPE_LCD|FDisplayQueueDescriptor
                        dc      MODULE_NUM_DISPLAY
                        dc      MSG_TYPE_MIXER|MixerQueueDescriptor
                        dc      MODULE_NUM_MIXER
                        dc      MSG_TYPE_SYSTEM|SystemQueueDescriptor
                        dc      MODULE_NUM_SYSTEM
                        dc      MSG_TYPE_MENU|MenuQueueDescriptor
                        dc      MODULE_NUM_MENU
                        dc      MSG_TYPE_SOFT_TIMER|Fglb_SoftTimerQueueDescriptor
                        dc      MODULE_NUM_SOFT_TIMER
    if (!@def('REMOVE_FM'))
                        dc      MSG_TYPE_TUNER|TunerQueueDescriptor
                        dc      MODULE_NUM_FMTUNER
    endif
                        dc      MSG_TYPE_GEQ|GeqQueueDescriptor
                        dc      MODULE_NUM_GEQ
	if @def('USE_PLAYLIST3')
                        dc      MSG_TYPE_MUSICLIB_BROWSE|MusicLibBrowseQueueDescriptor
                        dc      MODULE_NUM_MUSICLIB_BROWSE
                        dc      MSG_TYPE_MUSICLIB_PLAY|MusicLibPlayQueueDescriptor
                        dc      MODULE_NUM_MUSICLIB_PLAY
                        dc      MSG_TYPE_MUSICLIB_PLAYALLNEXT|MusicLibPlayAllNextQueueDescriptor
                        dc      MODULE_NUM_MUSICLIB_PLAYALLNEXT
                        dc      MSG_TYPE_MUSICLIB_PLAYALLPREV|MusicLibPlayAllPrevQueueDescriptor
                        dc      MODULE_NUM_MUSICLIB_PLAYALLPREV
                        dc      MSG_TYPE_MUSICLIB_SHUFFLE|MusicLibShuffleQueueDescriptor
                        dc      MODULE_NUM_MUSICLIB_SHUFFLE
                        dc      MSG_TYPE_MUSICLIB_VOICE|MusicLibVoiceQueueDescriptor
                        dc      MODULE_NUM_MUSICLIB_VOICE
                        dc      MSG_TYPE_MUSICLIB_VOICEPLAY|MusicLibVoicePlayQueueDescriptor
                        dc      MODULE_NUM_MUSICLIB_VOICEPLAY
	if @def('MMC')
                        dc      MSG_TYPE_MUSICLIB_MERGE|MusicLibMergeQueueDescriptor
                        dc      MODULE_NUM_MUSICLIB_MERGE
	endif   ; if @def('MMC')
	endif	; if @def('USE_PLAYLIST3')
	if @def('USE_PLAYLIST5')
                        dc      MSG_TYPE_PL5_PLAYBACK|PL5PlaybackModuleQueueDescriptor
                        dc      MODULE_NUM_PL5_PLAYBACK
                        dc      MSG_TYPE_PL5_BROWSE|PL5BrowseModuleQueueDescriptor
                        dc      MODULE_NUM_PL5_BROWSE
	endif   ; if @def('USE_PLAYLIST5')
EndModuleRouterTable
ROUTER_TABLE_SIZE       EQU     (EndModuleRouterTable-ModuleRouterTable)/2

;----------------------------------------------------

    ; Array of buffer descriptors for the message queues
    ; It is important that these descriptors are in the same
    ; order as the message types as defined in msgequ.inc
    ; After each queue descriptor you must define a constant specifying the first
    ; command supported for the module using this queue.

DecoderQueueDescriptor  dc      DecoderMsgQueue             ; buffer base address
                        dc      MSG_QUEUE_MODULO_DECODER    ; buffer modulo
                        dc      MSG_QUEUE_SIZE_DECODER      ; buffer queue size
                        dc      DecoderMsgQueue             ; buffer head ptr
                        dc      DecoderMsgQueue             ; buffer tail ptr

CaptureQueueDescriptor  dc      CaptureMsgQueue
                        dc      MSG_QUEUE_MODULO_CAPTURE
                        dc      MSG_QUEUE_SIZE_CAPTURE
CaptureHeadPtr		dc      CaptureMsgQueue
CaptureTailPtr		dc      CaptureMsgQueue

FDisplayQueueDescriptor      dc      DisplayMsgQueue
                        dc      MSG_QUEUE_MODULO_DISPLAY
                        dc      MSG_QUEUE_SIZE_DISPLAY
                        dc      DisplayMsgQueue
                        dc      DisplayMsgQueue

MixerQueueDescriptor    dc      MixerMsgQueue
                        dc      MSG_QUEUE_MODULO_MIXER
                        dc      MSG_QUEUE_SIZE_MIXER
                        dc      MixerMsgQueue
                        dc      MixerMsgQueue

SystemQueueDescriptor   dc      SystemMsgQueue
                        dc      MSG_QUEUE_MODULO_SYSTEM
                        dc      MSG_QUEUE_SIZE_SYSTEM
                        dc      SystemMsgQueue
                        dc      SystemMsgQueue

MenuQueueDescriptor     dc      MenuMsgQueue
                        dc      MSG_QUEUE_MODULO_MENU
                        dc      MSG_QUEUE_SIZE_MENU
MenuHeadPtr             dc      MenuMsgQueue
MenuTailPtr             dc      MenuMsgQueue

GeqQueueDescriptor      dc      GeqMsgQueue
                        dc      MSG_QUEUE_MODULO_GEQ
                        dc      MSG_QUEUE_SIZE_GEQ
                        dc      GeqMsgQueue
                        dc      GeqMsgQueue
    if (!@def('REMOVE_FM'))
TunerQueueDescriptor    dc      TunerMsgQueue
                        dc      MSG_QUEUE_SIZE_TUNER-1
                        dc      MSG_QUEUE_SIZE_TUNER
                        dc      TunerMsgQueue
                        dc      TunerMsgQueue
    endif
Fglb_SoftTimerQueueDescriptor dc     SoftTimerMsgQueue
                        dc      MSG_QUEUE_SIZE_SOFT_TIMERS-1
                        dc      MSG_QUEUE_SIZE_SOFT_TIMERS
                        dc      SoftTimerMsgQueue
                        dc      SoftTimerMsgQueue
	if @def('USE_PLAYLIST3')
MusicLibBrowseQueueDescriptor    dc      MusicLibBrowseMsgQueue
                        dc      MSG_QUEUE_SIZE_MUSICLIB_BROWSE-1
                        dc      MSG_QUEUE_SIZE_MUSICLIB_BROWSE
                        dc      MusicLibBrowseMsgQueue
                        dc      MusicLibBrowseMsgQueue

MusicLibPlayQueueDescriptor    dc      MusicLibPlayMsgQueue
                        dc      MSG_QUEUE_SIZE_MUSICLIB_PLAY-1
                        dc      MSG_QUEUE_SIZE_MUSICLIB_PLAY
                        dc      MusicLibPlayMsgQueue
                        dc      MusicLibPlayMsgQueue

MusicLibPlayAllNextQueueDescriptor    dc      MusicLibPlayAllNextMsgQueue
                        dc      MSG_QUEUE_SIZE_MUSICLIB_PLAYALLNEXT-1
                        dc      MSG_QUEUE_SIZE_MUSICLIB_PLAYALLNEXT
                        dc      MusicLibPlayAllNextMsgQueue
                        dc      MusicLibPlayAllNextMsgQueue

MusicLibPlayAllPrevQueueDescriptor    dc      MusicLibPlayAllPrevMsgQueue
                        dc      MSG_QUEUE_SIZE_MUSICLIB_PLAYALLPREV-1
                        dc      MSG_QUEUE_SIZE_MUSICLIB_PLAYALLPREV
                        dc      MusicLibPlayAllPrevMsgQueue
                        dc      MusicLibPlayAllPrevMsgQueue

MusicLibShuffleQueueDescriptor    dc      MusicLibShuffleMsgQueue
                        dc      MSG_QUEUE_SIZE_MUSICLIB_SHUFFLE-1
                        dc      MSG_QUEUE_SIZE_MUSICLIB_SHUFFLE
                        dc      MusicLibShuffleMsgQueue
                        dc      MusicLibShuffleMsgQueue

MusicLibVoiceQueueDescriptor    dc      MusicLibVoiceMsgQueue
                        dc      MSG_QUEUE_SIZE_MUSICLIB_VOICE-1
                        dc      MSG_QUEUE_SIZE_MUSICLIB_VOICE
                        dc      MusicLibVoiceMsgQueue
                        dc      MusicLibVoiceMsgQueue

MusicLibVoicePlayQueueDescriptor    dc      MusicLibVoicePlayMsgQueue
                        dc      MSG_QUEUE_SIZE_MUSICLIB_VOICEPLAY-1
                        dc      MSG_QUEUE_SIZE_MUSICLIB_VOICEPLAY
                        dc      MusicLibVoicePlayMsgQueue
                        dc      MusicLibVoicePlayMsgQueue

	if @def('MMC')
MusicLibMergeQueueDescriptor    dc      MusicLibMergeMsgQueue
                        dc      MSG_QUEUE_SIZE_MUSICLIB_MERGE-1
                        dc      MSG_QUEUE_SIZE_MUSICLIB_MERGE
                        dc      MusicLibMergeMsgQueue
                        dc      MusicLibMergeMsgQueue
	endif   ; if @def('MMC')
	endif	; if @def('USE_PLAYLIST3')
    if @def('USE_PLAYLIST5')
PL5PlaybackModuleQueueDescriptor      dc      PL5PlaybackMsgQueue
                        dc      MSG_QUEUE_SIZE_PL5_PLAYBACK-1
                        dc      MSG_QUEUE_SIZE_PL5_PLAYBACK
                        dc      PL5PlaybackMsgQueue
                        dc      PL5PlaybackMsgQueue

PL5BrowseModuleQueueDescriptor        dc      PL5BrowseMsgQueue
                        dc      MSG_QUEUE_SIZE_PL5_BROWSE-1
                        dc      MSG_QUEUE_SIZE_PL5_BROWSE
                        dc      PL5BrowseMsgQueue
                        dc      PL5BrowseMsgQueue

    endif   ; if @def('USE_PLAYLIST5')
    ;!!!!!!!!!!!!!!!!!!!!!!! WARNING !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    ; Module table
    ; !!!!!!!!!!!! These must match the module order in the ModuleTable below
MODULE_NUM_DECODER      equ     0
MODULE_NUM_CAPTURE      equ     1
MODULE_NUM_DISPLAY      equ     2
MODULE_NUM_MIXER        equ     3
MODULE_NUM_SYSTEM       equ     4
MODULE_NUM_MENU         equ     5
MODULE_NUM_SOFT_TIMER   equ     6
    if (!@def('REMOVE_FM'))
MODULE_NUM_FMTUNER      equ     7
MODULE_NUM_GEQ	        equ     8
	if @def('USE_PLAYLIST3')
MODULE_NUM_MUSICLIB_PLAY     equ     9
MODULE_NUM_MUSICLIB_BROWSE    equ     10
MODULE_NUM_MUSICLIB_PLAYALLNEXT     equ     11
MODULE_NUM_MUSICLIB_PLAYALLPREV     equ     12
MODULE_NUM_MUSICLIB_SHUFFLE     equ    13
MODULE_NUM_MUSICLIB_VOICE    equ     14
MODULE_NUM_MUSICLIB_VOICEPLAY    equ     15
	if @def('MMC')
MODULE_NUM_MUSICLIB_MERGE     equ     16
MODULE_COUNT            equ     17
	else    ; if @def('MMC')
MODULE_COUNT            equ     16
	endif   ; if @def('MMC')
	else	; if @def('USE_PLAYLIST3')
    if @def('USE_PLAYLIST5')
MODULE_NUM_PL5_PLAYBACK    equ     9
MODULE_NUM_PL5_BROWSE      equ     10
MODULE_COUNT            equ     11
    else
MODULE_COUNT            equ     9
    endif   ; if @def('USE_PLAYLIST5')
	endif	; if @def('USE_PLAYLIST3')
    else
MODULE_NUM_GEQ	        equ     7
    	if @def('USE_PLAYLIST3')
MODULE_NUM_MUSICLIB_PLAY     equ     8
MODULE_NUM_MUSICLIB_BROWSE    equ     9
MODULE_NUM_MUSICLIB_PLAYALLNEXT     equ     10
MODULE_NUM_MUSICLIB_PLAYALLPREV     equ     11
MODULE_NUM_MUSICLIB_SHUFFLE     equ    12
MODULE_NUM_MUSICLIB_VOICE    equ     13
MODULE_NUM_MUSICLIB_VOICEPLAY    equ     14
	    if @def('MMC')
MODULE_NUM_MUSICLIB_MERGE     equ     15
MODULE_COUNT            equ     16
	    else    ; if @def('MMC')
MODULE_COUNT            equ     15
	    endif   ; if @def('MMC')
	    else	; if @def('USE_PLAYLIST3')
    if @def('USE_PLAYLIST5')
MODULE_NUM_PL5_PLAYBACK    equ     8
MODULE_NUM_PL5_BROWSE      equ     9
MODULE_COUNT            equ     10
    else
MODULE_COUNT            equ     8
    endif   ; if @def('USE_PLAYLIST5')
	    endif	; if @def('USE_PLAYLIST3')
    endif   ; if (!@def('REMOVE_FM'))

    ; Size of a module table entry
MODULE_ENTRY_SIZE       equ     9

        org     y,"SYSPLAYER_ModuleTable_Y":

ModuleTable

ModuleTableDecoder
    ; Decoder module
    dc  0                               ; Event signal
    dc  EVENT_INIT                      ; Event wait
FDecoderResourcePtr
DecoderResourcePtr
    dc  RSRC_DECMOD_CODE                ; Resource number of code
    dc  DecoderQueueDescriptor          ; Message queue descriptor
DecoderProcessPtr
    dc  0                               ; Event processing routine - populated by init routine
    dc  F_lc_u_bP_Module_overlay+1
    dc  0                               ; Signal Timer High
    dc  0                               ; Signal Timer Low
    dc  EmptyFunction                   ; BackGround Processing routine

    ; Capture module
ModuleTableEncoder
    dc  0                               ; Event signal
    dc  EVENT_INIT                      ; Event wait
EncoderResourcePtr
    dc  RSRC_ENC_ADPCM_MOD_CODE         ; Resource number of code
    dc  CaptureQueueDescriptor          ; Message queue descriptor
EncoderProcessPtr
    dc  0                               ; Event processing routine - populated by init routine
    dc  F_lc_u_bP_Module_overlay+1
    dc  0                               ; Signal Timer High
    dc  0                               ; Signal Timer Low
    dc  EmptyFunction                   ; BackGround Processing routine

    ; Display module
ModuleTableDisplay
    dc  EVENT_INIT|EVENT_NONE           ; Event signal
    dc  EVENT_INIT|EVENT_MESSAGE        ; Event wait
    dc  RSRC_DISPLAY_MODULE             ; Resource number of code
    dc  FDisplayQueueDescriptor          ; Message queue descriptor
    dc  FDisplayProcessQueue            ; Event processing routine
    dc  FDisplayModuleInit              ; Initialization routine
    dc  0                               ; Signal Timer High
    dc  0                               ; Signal Timer Low
    dc  EmptyFunction                   ; BackGround Processing routine

    ; Mixer module
ModuleTableMixer
    dc  EVENT_INIT|EVENT_NONE           ; Event signal
    dc  EVENT_INIT|EVENT_MESSAGE        ; Event wait
    dc  RSRC_MIXMOD_CODE                ; Resource number of code
    dc  MixerQueueDescriptor            ; Message queue descriptor
    dc  MixerProcessQueue               ; Event processing routine
    dc  MixerInit                       ; Initialization routine
    dc  0                               ; Signal Timer High
    dc  0                               ; Signal Timer Low
    dc  EmptyFunction                   ; BackGround Processing routine

    ; System module
ModuleTableSystem
    dc  EVENT_INIT|EVENT_NONE           ; Event signal
    dc  EVENT_INIT|EVENT_MESSAGE|EVENT_NONE ; Event wait
        dc  RSRC_SYSMOD_CODE            ; Resource number of code
    dc  SystemQueueDescriptor           ; Message queue descriptor
    dc  FSystemModuleProcessQueue       ; Event processing routine
    dc  FSystemModuleInit               ; Initialization routine
    dc  0                               ; Signal Timer High
    dc  0                               ; Signal Timer Low
    dc  EmptyFunction                   ; BackGround Processing routine

    ; Menu module
ModuleTableMenus
MenuTableMenuPtr
MenuEventSignalMask
    dc  EVENT_INIT|EVENT_NONE               ; Event signal
MenuEventWaitMask
    dc  EVENT_INIT|EVENT_MESSAGE|EVENT_NONE ; Event wait
FMenuResourcePtr
MenuResourcePtr
    dc  RSRC_MAIN_MENU_CODE_BANK            ; Resource number of code
    dc  MenuQueueDescriptor                 ; Message queue descriptor
MenuProcessEventRoutinePtr
    dc  UserTaskKernel                      ; Event processing routine
MenuInit
    dc  InitUserTask                        ; Initialization routine
MenuTimerHigh
    dc  0                                   ; Signal Timer High
MenuTimerLow
    dc  0                                   ; Signal Timer Low
    dc  EmptyFunction                       ; BackGround Processing routine

Fglb_uSoftTimerModuleEntry
    dc  EVENT_INIT                      ; Event signal
    dc  EVENT_INIT|EVENT_MESSAGE                   ; Event wait
    dc  RSRC_SOFT_TIMER_MODULE_CODE     ; Resource number of code
    dc  Fglb_SoftTimerQueueDescriptor   ; Message queue descriptor
    dc  FSoftTimerModuleProcessQueue    ; Event processing routine
    dc  FSoftTimerModuleInit            ; Initialization routine
    dc  0                               ; Signal Timer High
    dc  0                               ; Signal Timer Low
    dc  EmptyFunction                   ; BackGround Processing routine


 if (!@def('REMOVE_FM'))
TunerModuleEntry
    dc  EVENT_INIT                      ; Event signal
    dc  EVENT_INIT|EVENT_MESSAGE        ; Event wait
    dc  RSRC_TUNER_MODULE               ; Resource number of code
    dc  TunerQueueDescriptor            ; Message queue descriptor

    dc  FTunerModuleProcessQueue        ; Event processing routine
    dc  FTunerModuleInit                ; Initialization routine

    dc  0                               ; Signal Timer High
    dc  0                               ; Signal Timer Low
    dc  EmptyFunction                   ; BackGround Processing routine
 endif

ModuleTableGeq
GeqTableGeqPtr
    dc  EVENT_INIT|EVENT_NONE           ; Event signal
    dc  EVENT_INIT|EVENT_MESSAGE	; Event wait
GeqResourcePtr
    dc  RSRC_GEQOVERLAY_CODE            ; Resource number of code
    dc  GeqQueueDescriptor              ; Message queue descriptor
GeqProcessEventRoutinePtr
    dc  GeqOverlayModule                ; Event processing routine
    dc  GeqInit                         ; Initialization routine
    dc  0                               ; Signal Timer High
    dc  0                               ; Signal Timer Low
    dc  EmptyFunction	             	; BackGround Processing routine
	if @def('USE_PLAYLIST3')
MusicLibPlayModuleEntry
    dc  EVENT_INIT|EVENT_NONE           ; Event signal
    dc  EVENT_INIT|EVENT_MESSAGE        ; Event wait
    dc  RSRC_MUSICLIB_PLAY_MODULE           ; Resource number of code
    dc  MusicLibPlayQueueDescriptor         ; Message queue descriptor
    dc  FMusicLibPlayModuleProcessQueue     ; Event processing routine
    dc  FMusicLibPlayModuleInit             ; Initialization routine
    dc  0                               ; Signal Timer High
    dc  0                               ; Signal Timer Low
    dc  EmptyFunction                   ; BackGround Processing routine

MusicLibBrowseModuleEntry
    dc  EVENT_INIT|EVENT_NONE           ; Event signal
    dc  EVENT_INIT|EVENT_MESSAGE        ; Event wait
    dc  RSRC_MUSICLIB_BROWSE_MODULE           ; Resource number of code
    dc  MusicLibBrowseQueueDescriptor         ; Message queue descriptor
    dc  FMusicLibBrowseModuleProcessQueue     ; Event processing routine
    dc  FMusicLibBrowseModuleInit             ; Initialization routine
    dc  0                               ; Signal Timer High
    dc  0                               ; Signal Timer Low
    dc  EmptyFunction                   ; BackGround Processing routine

MusicLibPlayAllNextModuleEntry
    dc  EVENT_INIT|EVENT_NONE           ; Event signal
    dc  EVENT_INIT|EVENT_MESSAGE        ; Event wait
    dc  RSRC_MUSICLIB_PLAYALLNEXT_MODULE           ; Resource number of code
    dc  MusicLibPlayAllNextQueueDescriptor         ; Message queue descriptor
    dc  FMusicLibPlayAllNextModuleProcessQueue     ; Event processing routine
    dc  FMusicLibPlayAllNextModuleInit             ; Initialization routine
    dc  0                               ; Signal Timer High
    dc  0                               ; Signal Timer Low
    dc  EmptyFunction                   ; BackGround Processing routine

MusicLibPlayAllPrevModuleEntry
    dc  EVENT_INIT|EVENT_NONE           ; Event signal
    dc  EVENT_INIT|EVENT_MESSAGE        ; Event wait
    dc  RSRC_MUSICLIB_PLAYALLPREV_MODULE           ; Resource number of code
    dc  MusicLibPlayAllPrevQueueDescriptor         ; Message queue descriptor
    dc  FMusicLibPlayAllPrevModuleProcessQueue     ; Event processing routine
    dc  FMusicLibPlayAllPrevModuleInit             ; Initialization routine
    dc  0                               ; Signal Timer High
    dc  0                               ; Signal Timer Low
    dc  EmptyFunction                   ; BackGround Processing routine

MusicLibShuffleModuleEntry
    dc  EVENT_INIT|EVENT_NONE           ; Event signal
    dc  EVENT_INIT|EVENT_MESSAGE        ; Event wait
    dc  RSRC_MUSICLIB_SHUFFLE_MODULE           ; Resource number of code
    dc  MusicLibShuffleQueueDescriptor         ; Message queue descriptor
    dc  FMusicLibShuffleModuleProcessQueue     ; Event processing routine
    dc  FMusicLibShuffleModuleInit             ; Initialization routine
    dc  0                               ; Signal Timer High
    dc  0                               ; Signal Timer Low
    dc  EmptyFunction                   ; BackGround Processing routine

MusicLibVoiceModuleEntry
    dc  EVENT_INIT|EVENT_NONE           ; Event signal
    dc  EVENT_INIT|EVENT_MESSAGE        ; Event wait
    dc  RSRC_MUSICLIB_VOICE_MODULE           ; Resource number of code
    dc  MusicLibVoiceQueueDescriptor         ; Message queue descriptor
    dc  FMusicLibVoiceModuleProcessQueue     ; Event processing routine
    dc  FMusicLibVoiceModuleInit             ; Initialization routine
    dc  0                               ; Signal Timer High
    dc  0                               ; Signal Timer Low
    dc  EmptyFunction                   ; BackGround Processing routine

MusicLibVoicePlayModuleEntry
    dc  EVENT_INIT|EVENT_NONE           ; Event signal
    dc  EVENT_INIT|EVENT_MESSAGE        ; Event wait
    dc  RSRC_MUSICLIB_VOICEPLAY_MODULE           ; Resource number of code
    dc  MusicLibVoicePlayQueueDescriptor         ; Message queue descriptor
    dc  FMusicLibVoicePlayModuleProcessQueue     ; Event processing routine
    dc  FMusicLibVoicePlayModuleInit             ; Initialization routine
    dc  0                               ; Signal Timer High
    dc  0                               ; Signal Timer Low
    dc  EmptyFunction                   ; BackGround Processing routine
	if @def('MMC')
MusicLibMergeModuleEntry
    dc  EVENT_INIT|EVENT_NONE           ; Event signal
    dc  EVENT_INIT|EVENT_MESSAGE        ; Event wait
    dc  RSRC_MUSICLIB_MERGE_MODULE           ; Resource number of code
    dc  MusicLibMergeQueueDescriptor         ; Message queue descriptor
    dc  FMusicLibMergeModuleProcessQueue     ; Event processing routine
    dc  FMusicLibMergeModuleInit             ; Initialization routine
    dc  0                               ; Signal Timer High
    dc  0                               ; Signal Timer Low
    dc  EmptyFunction                   ; BackGround Processing routine
	endif	; if @def('MMC')
	endif	; if @def('USE_PLAYLIST3')
    if @def('USE_PLAYLIST5')
PL5PlaybackModuleEntry
    dc  EVENT_INIT                      ; Event signal
    dc  EVENT_INIT|EVENT_MESSAGE        ; Event wait
    dc  RSRC_PLAYLIST5_PLAYBACK_MODULE  ; Resource number of code
    dc  PL5PlaybackModuleQueueDescriptor; Message queue descriptor
    dc  FPL5PlaybackModuleProcessQueue  ; Event processing routine
    dc  FPL5PlaybackModuleInit          ; Initialization routine
    dc  0                               ; Signal Timer High
    dc  0                               ; Signal Timer Low
    dc  EmptyFunction                   ; BackGround Processing routine

PL5BrowseModuleEntry
    dc  EVENT_INIT                      ; Event signal
    dc  EVENT_INIT|EVENT_MESSAGE        ; Event wait
    dc  RSRC_PLAYLIST5_BROWSE_MODULE    ; Resource number of code
    dc  PL5BrowseModuleQueueDescriptor  ; Message queue descriptor
    dc  FPL5BrowseModuleProcessQueue    ; Event processing routine
    dc  FPL5BrowseModuleInit            ; Initialization routine
    dc  0                               ; Signal Timer High
    dc  0                               ; Signal Timer Low
    dc  EmptyFunction                   ; BackGround Processing routine
    endif   ; if @def('USE_PLAYLIST5')
    endsec

