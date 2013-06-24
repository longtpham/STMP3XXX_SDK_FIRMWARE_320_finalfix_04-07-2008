;;  SigmaTel Inc
;;  $Archive: $
;;  $Revision: 1.1.1.1.4.1 $
;;  $Date: 2004/12/30 07:12:24 $
;;  Description: Reentrant.asm
;; ----------------------------------------------------------------------------*/
;
		Section  StmpReentrantAPI

;****************************************************************************************************************
;  Include files
;***************************************************************************************************************
    include "sysmacro.asm"
    include "const.xref"

; Use relative paths only

;***********************************************************************************************************************
;	Global definitions
;***************************************************************************************************************
    global  FEnterNonReentrantSection,EnterNonReentrantSection
    global  FLeaveNonReentrantSection,LeaveNonReentrantSection

    GLOBAL   EncodingFileWriteFlag

;;***********************************************************************************************************************
;;	    Extern  definitions
;;***************************************************************************************************************
       extern    SetIrqL
       extern    GetIrqL

;;***********************************************************************************************************************
;;	    Extern  definitions
;;***************************************************************************************************************
;
IPL_LEVEL_1         equ     $0100
IPL_LEVEL_2         equ     $0200
IPL_LEVEL_3         equ     $0300

;;**************************************************************************************************************
;;	Code Start
;;**************************************************************************************************************
    org y,"StmpReentrant_y":
FS_IRQ_Level_Buffer    bsc  16,$111111
FS_Reentrancy_Nesting_Level         dc  0
EncodingFileWriteFlag   dc      0   ; Flag to indicate we're Encoding.


        org p,"StmpReentrant_P":
FEnterNonReentrantSection
EnterNonReentrantSection
	movec   ssh,y:(r7)+
	push	x0
	push	y0
	push	r0
	push	n0
	push	a2
	push	a1
	push	a0
;   tt 1/15/04 - changes for voice record.  Want to make the file system interruptable since
;               we want the Encoder ISR to run in the background during a FileWrite.  Thus,
;               this will be disabled until SDK3.xxx when pre-emptive kernel will make this
;               necessary.
    btst    #0,y:EncodingFileWriteFlag
    jcs     _EndofEntry

    jsr     GetIrqL
    push    x0
 
;;;;;    move    #>IPL_LEVEL_1,x0            ;Sets new IRQ Level to 1
    move    #>IPL_LEVEL_3,x0            ; Disable all interrupts in order to save index correctly
    jsr     SetIrqL
    move    #FS_IRQ_Level_Buffer,n0
    move    y:FS_Reentrancy_Nesting_Level,r0
    pop     x0
    move    x0,y:(r0+n0)     ;Saves IRQ Level
    move    (r0)+
    move    r0,y:FS_Reentrancy_Nesting_Level

    move    #>IPL_LEVEL_1,x0            ;Sets new IRQ Level to 1
    jsr     SetIrqL
_EndofEntry
	nop
	pop		a0
	pop		a1
	pop		a2
	pop		n0
	pop		r0
	pop		y0
	pop		x0
    movec y:-(r7),ssh
    nop
    rts

FLeaveNonReentrantSection
LeaveNonReentrantSection
	movec   ssh,y:(r7)+
	push	x0
	push	y0
	push	r0
	push	n0
	push	a2
	push	a1
	push	a0
;   tt 1/15/04 - changes for voice record.  Want to make the file system interruptable since
;               we want the Encoder ISR to run in the background during a FileWrite.  Thus,
;               this will be disabled until SDK3.xxx when pre-emptive kernel will make this
;               necessary.
    btst    #0,y:EncodingFileWriteFlag
    jcs     _EndofLeave

    move    #>IPL_LEVEL_3,x0            ;Sets new IRQ Level to 3
    jsr     SetIrqL

    move    #FS_IRQ_Level_Buffer,n0
    move    y:FS_Reentrancy_Nesting_Level,r0
    nop
    move    (r0)-
    NOP
    move    y:(r0+n0),x0     ;Restores IRQ Level
    move    r0,y:FS_Reentrancy_Nesting_Level
    jsr     SetIrqL

_EndofLeave
	nop
	pop		a0
	pop		a1
	pop		a2
	pop		n0
	pop		r0
	pop		y0
	pop		x0
    movec y:-(r7),ssh
    nop
    rts

    endsec
