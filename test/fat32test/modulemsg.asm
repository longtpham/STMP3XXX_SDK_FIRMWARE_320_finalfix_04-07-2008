;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000
;
; STMP3 System Post Message
;///////////////////////////////////////////////////////////////////////////////

        section SYSPLAYER_ModuleMsg

;///////////////////////////////////////////////////////////////////////////////
;   Includes and external references
;///////////////////////////////////////////////////////////////////////////////
    nolist
        include "sysmacro.asm"
        include "sysequ.inc"
        include "msgequ.inc"
        include "resource.inc"
    
        include "bufdesc.xref"
    list

        EXTERN  EmptyFunction
    	extern  FDisplayModuleInit
    	extern  FDisplayProcessQueue

;///////////////////////////////////////////////////////////////////////////////
;   Symbol Exports
;///////////////////////////////////////////////////////////////////////////////
;Variables in Y
        GLOBAL  QueueDescriptorArray       
        GLOBAL  MODULE_COUNT
        GLOBAL  MODULE_ENTRY_SIZE
        GLOBAL  MODULE_TABLE_MODULO
        GLOBAL  MSG_QUEUE_COUNT
        GLOBAL  ModuleRouterTable
        GLOBAL  ROUTER_TABLE_SIZE
        GLOBAL  ModuleTableDisplay
    	GLOBAL  FModuleTableDisplay
        GLOBAL  FDisplayQueueDescriptor


;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////

MSG_QUEUE_COUNT             equ     1

MSG_QUEUE_SIZE_DISPLAY      equ     160
MSG_QUEUE_MODULO_DISPLAY    equ     MSG_QUEUE_SIZE_DISPLAY-1

MODULE_NUM_DISPLAY          equ     0

MODULE_COUNT                equ     1

; Size of a module table entry
MODULE_ENTRY_SIZE           equ     9

; Module table modulo
MODULE_TABLE_MODULO         equ     (MODULE_ENTRY_SIZE*MODULE_COUNT)-1

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

ModuleRouterTable
                        dc      MSG_TYPE_LCD|FDisplayQueueDescriptor
                        dc      MODULE_NUM_DISPLAY
EndModuleRouterTable

ROUTER_TABLE_SIZE       EQU     (EndModuleRouterTable-ModuleRouterTable)/2                        

; Array of buffer descriptors for the message queues
; It is important that these descriptors are in the same
; order as the message types as defined in msgequ.inc
QueueDescriptorArray

FDisplayQueueDescriptor dc      DisplayMsgQueue
                        dc      MSG_QUEUE_MODULO_DISPLAY
                        dc      MSG_QUEUE_SIZE_DISPLAY
                        dc      DisplayMsgQueue
                        dc      DisplayMsgQueue

    buffer  M,MODULE_ENTRY_SIZE*(MODULE_COUNT+1)

ModuleTable
                    
    ; Display module
FModuleTableDisplay
ModuleTableDisplay
    dc  EVENT_INIT|EVENT_NONE           ; Event signal
    dc  EVENT_INIT|EVENT_MESSAGE        ; Event wait
    dc  RSRC_DISPLAY_MODULE             ; Resource number of code
    dc  FDisplayQueueDescriptor         ; Message queue descriptor
    dc  FDisplayProcessQueue            ; Event processing routine
    dc  FDisplayModuleInit              ; Initialization routine
    dc  0                               ; Signal Timer High
    dc  0                               ; Signal Timer Low
    dc  EmptyFunction                   ; BackGround Processing routine
    
    endbuf

;///////////////////////////////////////////////////////////////////////////////
;   Code
;///////////////////////////////////////////////////////////////////////////////

    org     p,"syspostmessage.p":
        endsec

