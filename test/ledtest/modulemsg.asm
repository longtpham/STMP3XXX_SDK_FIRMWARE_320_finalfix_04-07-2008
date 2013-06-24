;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2001
;
; Filename:     modulemsg.asm
; Description:  STMP3 Module Post Message
;///////////////////////////////////////////////////////////////////////////////

        section SYSPLAYER_ModuleMsg

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////
    nolist
    include "sysmacro.asm"
    include "resource.inc"
    include "sysequ.inc"
    list
    include "msgequ.inc"

;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

;Variables in Y
    GLOBAL  QueueDescriptorArray       
    GLOBAL  LedQueueDescriptor
    GLOBAL  ModuleTable
    GLOBAL  MSG_QUEUE_COUNT
    GLOBAL  MSG_QUEUE_SIZE_LED
    GLOBAL  MSG_QUEUE_MODULO_LED
    GLOBAL  ModuleRouterTable
    GLOBAL  ROUTER_TABLE_SIZE
    GLOBAL  MODULE_TABLE_MODULO
    GLOBAL  MODULE_COUNT
    GLOBAL  MODULE_ENTRY_SIZE

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "bufdesc.xref"
    list

    EXTERN  EmptyFunction
    EXTERN  LedProcessQueue         ; Event processing routine
    EXTERN  LedInit                 ; Initialization routine

;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////

MSG_QUEUE_COUNT             equ     1
MSG_QUEUE_SIZE_LED          equ     40
MSG_QUEUE_MODULO_LED        equ     MSG_QUEUE_SIZE_LED-1


;!!!!!!!!!!!!!!!!!!!!!!! WARNING !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; Module table
; !!!!!!!!!!!! These must match the module order in the ModuleTable below
MODULE_NUM_LED      equ     0
MODULE_COUNT        equ     1

; Size of a module table entry
MODULE_ENTRY_SIZE   equ     9

;///////////////////////////////////////////////////////////////////////////////
;  X memory
;///////////////////////////////////////////////////////////////////////////////

        org     x,"syspostmessage.x":

;///////////////////////////////////////////////////////////////////////////////
;  Y memory
;///////////////////////////////////////////////////////////////////////////////

        org     y,"syspostmessage.y":

ModuleRouterTable
                        dc      MSG_TYPE_LED|LedQueueDescriptor
                        dc      MODULE_NUM_LED
EndModuleRouterTable

ROUTER_TABLE_SIZE       EQU     (EndModuleRouterTable-ModuleRouterTable)/2                        

MODULE_TABLE_MODULO     equ     (MODULE_ENTRY_SIZE*MODULE_COUNT)-1
; Message Queues
LedMsgQueue             dsm     MSG_QUEUE_SIZE_LED


; Array of buffer descriptors for the message queues
; It is important that these descriptors are in the same
; order as the message types as defined in msgequ.inc
QueueDescriptorArray

LedQueueDescriptor      dc      LedMsgQueue
                        dc      MSG_QUEUE_MODULO_LED
                        dc      MSG_QUEUE_SIZE_LED
                        dc      LedMsgQueue
                        dc      LedMsgQueue

    buffer  M,MODULE_ENTRY_SIZE*(MODULE_COUNT+1)

ModuleTable

    ; LED module
ModuleTableLed
    dc  EVENT_INIT|EVENT_NONE   ; Event signal
    dc  EVENT_INIT|EVENT_MESSAGE  ; Event wait
    dc  0                       ; Resource number of code
    dc  LedQueueDescriptor      ; Message queue descriptor
    dc  LedProcessQueue         ; Event processing routine
    dc  LedInit                 ; Initialization routine
    dc  0                       ; Signal Timer High
SignalTimerLow    dc  0                       ; Signal Timer Low
    dc  EmptyFunction           ; BackGround Processing routine
    endbuf

;///////////////////////////////////////////////////////////////////////////////
;   Code
;///////////////////////////////////////////////////////////////////////////////

    org     p,"syspostmessage.p":

        endsec

