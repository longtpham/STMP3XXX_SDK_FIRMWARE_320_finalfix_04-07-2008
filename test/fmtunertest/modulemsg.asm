;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000
;
; STMP3 System Post Message
;///////////////////////////////////////////////////////////////////////////////

        section SYSPLAYER_ModuleMsg

;///////////////////////////////////////////////////////////////////////////////
;   Includes and external references
;///////////////////////////////////////////////////////////////////////////////
        include "msgequ.inc"
        include "resource.inc"

;///////////////////////////////////////////////////////////////////////////////
;   Symbol Exports
;///////////////////////////////////////////////////////////////////////////////
;Variables in Y
        GLOBAL  Fglb_TunerQueueDescriptor
        GLOBAL  FDisplayQueueDescriptor
	GLOBAL  MixerQueueDescriptor

        GLOBAL  MODULE_COUNT
        GLOBAL  MODULE_ENTRY_SIZE
        GLOBAL  MODULE_TABLE_MODULO
        GLOBAL  ModuleRouterTable
        GLOBAL  ROUTER_TABLE_SIZE
        GLOBAL  ModuleTable
        
	GLOBAL  MODULE_NUM_FMTUNER
	GLOBAL  FTunerModuleEntry
	GLOBAL  ModuleTableDisplay
    	GLOBAL  FModuleTableDisplay

	GLOBAL  FMenuTailPtr
	GLOBAL  FMenuHeadPtr
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

;///////////////////////////////////////////////////////////////////////////////
;   Externs
;///////////////////////////////////////////////////////////////////////////////
    	extern  FDisplayModuleInit
    	extern  FDisplayProcessQueue
	extern  FTunerModuleProcessQueue
	extern  FTunerModuleInit
	extern  MixerProcessQueue
	extern  MixerInit
 	extern UserTaskKernel
	extern InitUserTask
;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////

MSG_QUEUE_SIZE_DISPLAY      equ     160
MSG_QUEUE_MODULO_DISPLAY    equ     MSG_QUEUE_SIZE_DISPLAY-1

MSG_QUEUE_SIZE_FMTUNER      equ     160
MSG_QUEUE_MODULO_FMTUNER    equ     MSG_QUEUE_SIZE_FMTUNER-1

MSG_QUEUE_SIZE_MIXER        equ     32
MSG_QUEUE_MODULO_MIXER      equ     MSG_QUEUE_SIZE_MIXER-1

MSG_QUEUE_SIZE_MENU         equ     80
MSG_QUEUE_MODULO_MENU       equ     MSG_QUEUE_SIZE_MENU-1

;///////////////////////////////////////////////////////////////////////////////
;  X memory
;///////////////////////////////////////////////////////////////////////////////

        org     x,"syspostmessage.x":

;///////////////////////////////////////////////////////////////////////////////
;  Y memory
;///////////////////////////////////////////////////////////////////////////////
                        
    org     y,"syspostmessage.y":
; Message Queues
DisplayMsgQueue         dsm     MSG_QUEUE_SIZE_DISPLAY
FMTunerMsgQueue         dsm     MSG_QUEUE_SIZE_FMTUNER
MixerMsgQueue           dsm     MSG_QUEUE_SIZE_MIXER
MenuMsgQueue            dsm     MSG_QUEUE_SIZE_MENU

ModuleRouterTable
                        dc      MSG_TYPE_LCD|FDisplayQueueDescriptor
                        dc      MODULE_NUM_DISPLAY
                        dc      MSG_TYPE_TUNER|Fglb_TunerQueueDescriptor
                        dc      MODULE_NUM_FMTUNER
			dc      MSG_TYPE_MIXER|MixerQueueDescriptor
                        dc      MODULE_NUM_MIXER
                        dc      MSG_TYPE_MENU|MenuQueueDescriptor
                        dc      MODULE_NUM_MENU

EndModuleRouterTable
ROUTER_TABLE_SIZE       EQU     (EndModuleRouterTable-ModuleRouterTable)/2

; Array of buffer descriptors for the message queues
; It is important that these descriptors are in the same
; order as the message types as defined in msgequ.inc
FDisplayQueueDescriptor dc      DisplayMsgQueue
                        dc      MSG_QUEUE_MODULO_DISPLAY
                        dc      MSG_QUEUE_SIZE_DISPLAY
                        dc      DisplayMsgQueue
                        dc      DisplayMsgQueue

Fglb_TunerQueueDescriptor      dc      FMTunerMsgQueue
                        dc      MSG_QUEUE_MODULO_FMTUNER
                        dc      MSG_QUEUE_SIZE_FMTUNER
                        dc      FMTunerMsgQueue
                        dc      FMTunerMsgQueue

MixerQueueDescriptor    dc      MixerMsgQueue
                        dc      MSG_QUEUE_MODULO_MIXER
                        dc      MSG_QUEUE_SIZE_MIXER
                        dc      MixerMsgQueue
                        dc      MixerMsgQueue

MenuQueueDescriptor     dc      MenuMsgQueue
                        dc      MSG_QUEUE_MODULO_MENU
                        dc      MSG_QUEUE_SIZE_MENU
FMenuHeadPtr
MenuHeadPtr             dc      MenuMsgQueue
FMenuTailPtr
MenuTailPtr             dc      MenuMsgQueue

MODULE_NUM_DISPLAY      equ     0
MODULE_NUM_FMTUNER      equ     1
MODULE_NUM_MIXER        equ     2
MODULE_NUM_MENU		equ	3
	
MODULE_COUNT            equ     4

; Size of a module table entry
MODULE_ENTRY_SIZE       equ     9

; Module table modulo
MODULE_TABLE_MODULO     equ     (MODULE_ENTRY_SIZE*MODULE_COUNT)-1


ModuleTable
                    
    ; Display module
FModuleTableDisplay
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

FTunerModuleEntry
    dc  EVENT_INIT                      ; Event signal
    dc  EVENT_INIT|EVENT_MESSAGE        ; Event wait
    dc  RSRC_TUNER_MODULE               ; Resource number of code
    dc  Fglb_TunerQueueDescriptor       ; Message queue descriptor
    dc  FTunerModuleProcessQueue        ; Event processing routine
    dc  FTunerModuleInit                ; Initialization routine
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

    ; Menu module
ModuleTableMenus
MenuTableMenuPtr
MenuEventSignalMask
    dc  EVENT_INIT|EVENT_NONE               ; Event signal
MenuEventWaitMask
    dc  EVENT_INIT|EVENT_MESSAGE|EVENT_NONE ; Event wait
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

;///////////////////////////////////////////////////////////////////////////////
;   Code
;///////////////////////////////////////////////////////////////////////////////

    org     p,"syspostmessage.p":


EmptyFunction
    rts
        endsec

