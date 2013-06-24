;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Copyright(C) SigmaTel, Inc. 2002-2003
;;  File        : regs_system.inc
;;  Description : SYSTEM IP Register definition
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; The following naming conventions are followed in this file.
; All registers are named using the format...
;     HW_<module>_<regname>
; where <module> is the module name which can be any of the following...
;     SYSTEM
; (Note that when there is more than one copy of a particular module, the
; module name includes a number starting from 0 for the first instance of
; that module)
;
; <regname> is the specific register within that module
;
; We also define the following...
;
;     HW_<module>_<regname>_BITPOS
; which defines the starting bit (i.e. LSB) of a multi bit field
;
;     HW_<module>_<regname>_SETMASK
; which does something else, and
;
;     HW_<module>_<regname>_CLRMASK
; which does something else.
;
; Other rules
;     All caps
;     Numeric identifiers start at 0

    if !(@def(regs_systeminc))
regs_systeminc            equ         1

;C insert
;#include "types.h"
;endc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;   SYSTEM STMP Registers 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
HW_SYSTEM_BASEADDR                       equ     ($FA00)

;///////////////////////////////////////////////////////////////////////////////
;  SYSTEM 1.8V Pad Enable Registers
;///////////////////////////////////////////////////////////////////////////////

;ASM Only
HW_SYSTEM_1_8V_PAD_EN                            equ     (HW_SYSTEM_BASEADDR+48)   ;;// $FA30
;endasm

;///////////////////////////////////////////////////////////////////////////////
;  SYSTEM  (HW_SYSTEM_1_8V_PAD_EN) Bit Definitions
HW_SYSTEM_1_8V_PAD_EN_BITPOS             equ     (0)
HW_SYSTEM_1_8V_KEEP_EN_BITPOS            equ     (8)

HW_SYSTEM_1_8V_PAD_EN_WIDTH              equ     (8)
HW_SYSTEM_1_8V_KEEP_EN_WIDTH             equ     (1)

HW_SYSTEM_1_8V_PAD_EN_SETMASK            equ     (((1<<HW_SYSTEM_1_8V_PAD_EN_WIDTH)-1)<<HW_SYSTEM_1_8V_PAD_EN_BITPOS)
HW_SYSTEM_1_8V_KEEP_EN_SETMASK           equ     (((1<<HW_SYSTEM_1_8V_KEEP_EN_WIDTH)-1)<<HW_SYSTEM_1_8V_KEEP_EN_BITPOS)

HW_SYSTEM_1_8V_PAD_EN_CLRMASK            equ     (~HW_SYSTEM_1_8V_PAD_EN_SETMASK)
HW_SYSTEM_1_8V_KEEP_EN_CLRMASK           equ     (~HW_SYSTEM_1_8V_KEEP_EN_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int P1_8V_PAD_EN	: HW_SYSTEM_1_8V_PAD_EN_WIDTH;
;        int P1_8V_KEEP_EN	: HW_SYSTEM_1_8V_KEEP_EN_WIDTH;
;    } B;
;    int I;
;    unsigned int U;
;} system_1_8v_en_type;

;#define HW_SYSTEM_1_8V_PAD_EN      (*(volatile system_1_8v_en_type _X*) (HW_SYSTEM_BASEADDR+48))    /* SYSTEM 1.8V Pad Enable */
;endc

    endif