;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2001
;
; Filename: regstimer.inc
; Description: Register definitions for  Timers interface
;///////////////////////////////////////////////////////////////////////////////
; The following naming conventions are followed in this file.
; All registers are named using the format...
;     HW_<module>_<regname>
; where <module> is the module name which can be any of the following...
;     USB20
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
    if !(@def(regstimerinc))
regstimerinc            equ         1

;C insert
;#include "types.h"
;endc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;   TIMER STMP Registers 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
HW_TMR_BASEADDR             equ     ($F100)

HW_TMR0_BASEADDR            equ     HW_TMR_BASEADDR
HW_TMR1_BASEADDR            equ     HW_TMR_BASEADDR+$40
HW_TMR2_BASEADDR            equ     HW_TMR_BASEADDR+$80
HW_TMR3_BASEADDR            equ     HW_TMR_BASEADDR+$C0

HW_TIMER_NUMBER_0                       equ     0
HW_TIMER_NUMBER_1                       equ     1
HW_TIMER_NUMBER_2                       equ     2
HW_TIMER_NUMBER_3                       equ     3

HW_TMRCSR                   equ     0
HW_TMRCNTR                 equ     1

;ASM Only
;///////////////////////////////////////////////////////////////////////////////
;  Timer Control Status Registers 
;///////////////////////////////////////////////////////////////////////////////
HW_TMR0CSR                          equ    HW_TMR0_BASEADDR+HW_TMRCSR
HW_TMR0CNTR                         equ    HW_TMR0_BASEADDR+HW_TMRCNTR
HW_TMR1CSR                          equ    HW_TMR1_BASEADDR+HW_TMRCSR
HW_TMR1CNTR                         equ    HW_TMR1_BASEADDR+HW_TMRCNTR
HW_TMR2CSR                          equ    HW_TMR2_BASEADDR+HW_TMRCSR
HW_TMR2CNTR                         equ    HW_TMR2_BASEADDR+HW_TMRCNTR
HW_TMR3CSR                          equ    HW_TMR3_BASEADDR+HW_TMRCSR
HW_TMR3CNTR                         equ    HW_TMR3_BASEADDR+HW_TMRCNTR

;endasm

;///////////////////////////////////////////////////////////////////////////////
;  TIMER CSR (HW_TMR0CSR) Bit Definitions
HW_TMR0CSR_TIMER_ENABLE_BITPOS      equ    (0)
HW_TMR0CSR_TIMER_INT_EN_BITPOS      equ    (1)
HW_TMR0CSR_INVERT_BITPOS            equ    (2)
HW_TMR0CSR_TIMER_CONTROL_BITPOS     equ    (3)
HW_TMR0CSR_TIMER_STATUS_BITPOS      equ    (7)
HW_TMR0CSR_TIMER_MODE_BITPOS        equ    (8)
HW_TMR0CSR_CLKGT_BITPOS             equ    (23)

HW_TMR0CSR_TIMER_ENABLE_WIDTH       equ    (1)
HW_TMR0CSR_TIMER_INT_EN_WIDTH       equ    (1)
HW_TMR0CSR_INVERT_WIDTH             equ    (1)
HW_TMR0CSR_TIMER_CONTROL_WIDTH      equ    (3)
HW_TMR0CSR_TIMER_STATUS_WIDTH       equ    (1)
HW_TMR0CSR_TIMER_MODE_WIDTH         equ    (2)
HW_TMR0CSR_CLKGT_WIDTH              equ    (1)

HW_TMR0CSR_TIMER_ENABLE_SETMASK     equ    (((1<<HW_TMR0CSR_TIMER_ENABLE_WIDTH)-1)<<HW_TMR0CSR_TIMER_ENABLE_BITPOS)
HW_TMR0CSR_TIMER_INT_EN_SETMASK     equ    (((1<<HW_TMR0CSR_TIMER_INT_EN_WIDTH)-1)<<HW_TMR0CSR_TIMER_INT_EN_BITPOS)
HW_TMR0CSR_INVERT_SETMASK           equ    (((1<<HW_TMR0CSR_INVERT_WIDTH)-1)<<HW_TMR0CSR_INVERT_BITPOS)
HW_TMR0CSR_TIMER_CONTROL_SETMASK    equ    (((1<<HW_TMR0CSR_TIMER_CONTROL_WIDTH)-1)<<HW_TMR0CSR_TIMER_CONTROL_BITPOS)
HW_TMR0CSR_TIMER_STATUS_SETMASK     equ    (((1<<HW_TMR0CSR_TIMER_STATUS_WIDTH)-1)<<HW_TMR0CSR_TIMER_STATUS_BITPOS)
HW_TMR0CSR_TIMER_MODE_SETMASK       equ    (((1<<HW_TMR0CSR_TIMER_MODE_WIDTH)-1)<<HW_TMR0CSR_TIMER_MODE_BITPOS)
HW_TMR0CSR_CLKGT_SETMASK            equ    (((1<<HW_TMR0CSR_CLKGT_WIDTH)-1)<<HW_TMR0CSR_CLKGT_BITPOS)

HW_TMR0CSR_TIMER_ENABLE_CLRMASK     equ    (~HW_TMR0CSR_TIMER_ENABLE_SETMASK)
HW_TMR0CSR_TIMER_INT_EN_CLRMASK     equ    (~HW_TMR0CSR_TIMER_INT_EN_SETMASK)
HW_TMR0CSR_INVERT_CLRMASK           equ    (~HW_TMR0CSR_INVERT_SETMASK)
HW_TMR0CSR_TIMER_CONTROL_CLRMASK    equ    (~HW_TMR0CSR_TIMER_CONTROL_SETMASK)
HW_TMR0CSR_TIMER_STATUS_CLRMASK     equ    (~HW_TMR0CSR_TIMER_STATUS_SETMASK)
HW_TMR0CSR_TIMER_MODE_CLRMASK       equ    (~HW_TMR0CSR_TIMER_MODE_SETMASK)
HW_TMR0CSR_CLKGT_CLRMASK            equ    (~HW_TMR0CSR_CLKGT_SETMASK)

;///////////////////////////////////////////////////////////////////////////////
;  TIMER CSR (HW_TMR1CSR) Bit Definitions
HW_TMR1CSR_TIMER_ENABLE_BITPOS      equ    (0)
HW_TMR1CSR_TIMER_INT_EN_BITPOS      equ    (1)
HW_TMR1CSR_INVERT_BITPOS            equ    (2)
HW_TMR1CSR_TIMER_CONTROL_BITPOS     equ    (3)
HW_TMR1CSR_TIMER_STATUS_BITPOS      equ    (7)
HW_TMR1CSR_TIMER_MODE_BITPOS        equ    (8)
HW_TMR1CSR_CLKGT_BITPOS             equ    (23)

HW_TMR1CSR_TIMER_ENABLE_WIDTH       equ    (1)
HW_TMR1CSR_TIMER_INT_EN_WIDTH       equ    (1)
HW_TMR1CSR_INVERT_WIDTH             equ    (1)
HW_TMR1CSR_TIMER_CONTROL_WIDTH      equ    (3)
HW_TMR1CSR_TIMER_STATUS_WIDTH       equ    (1)
HW_TMR1CSR_TIMER_MODE_WIDTH         equ    (2)
HW_TMR1CSR_CLKGT_WIDTH              equ    (1)

HW_TMR1CSR_TIMER_ENABLE_SETMASK     equ    (((1<<HW_TMR1CSR_TIMER_ENABLE_WIDTH)-1)<<HW_TMR1CSR_TIMER_ENABLE_BITPOS)
HW_TMR1CSR_TIMER_INT_EN_SETMASK     equ    (((1<<HW_TMR1CSR_TIMER_INT_EN_WIDTH)-1)<<HW_TMR1CSR_TIMER_INT_EN_BITPOS)
HW_TMR1CSR_INVERT_SETMASK           equ    (((1<<HW_TMR1CSR_INVERT_WIDTH)-1)<<HW_TMR1CSR_INVERT_BITPOS)
HW_TMR1CSR_TIMER_CONTROL_SETMASK    equ    (((1<<HW_TMR1CSR_TIMER_CONTROL_WIDTH)-1)<<HW_TMR1CSR_TIMER_CONTROL_BITPOS)
HW_TMR1CSR_TIMER_STATUS_SETMASK     equ    (((1<<HW_TMR1CSR_TIMER_STATUS_WIDTH)-1)<<HW_TMR1CSR_TIMER_STATUS_BITPOS)
HW_TMR1CSR_TIMER_MODE_SETMASK       equ    (((1<<HW_TMR1CSR_TIMER_MODE_WIDTH)-1)<<HW_TMR1CSR_TIMER_MODE_BITPOS)
HW_TMR1CSR_CLKGT_SETMASK            equ    (((1<<HW_TMR1CSR_CLKGT_WIDTH)-1)<<HW_TMR1CSR_CLKGT_BITPOS)

HW_TMR1CSR_TIMER_ENABLE_CLRMASK     equ    (~HW_TMR1CSR_TIMER_ENABLE_SETMASK)
HW_TMR1CSR_TIMER_INT_EN_CLRMASK     equ    (~HW_TMR1CSR_TIMER_INT_EN_SETMASK)
HW_TMR1CSR_INVERT_CLRMASK           equ    (~HW_TMR1CSR_INVERT_SETMASK)
HW_TMR1CSR_TIMER_CONTROL_CLRMASK    equ    (~HW_TMR1CSR_TIMER_CONTROL_SETMASK)
HW_TMR1CSR_TIMER_STATUS_CLRMASK     equ    (~HW_TMR1CSR_TIMER_STATUS_SETMASK)
HW_TMR1CSR_TIMER_MODE_CLRMASK       equ    (~HW_TMR1CSR_TIMER_MODE_SETMASK)
HW_TMR1CSR_CLKGT_CLRMASK            equ    (~HW_TMR1CSR_CLKGT_SETMASK)

;///////////////////////////////////////////////////////////////////////////////
;  TIMER CSR (HW_TMR2CSR) Bit Definitions
HW_TMR2CSR_TIMER_ENABLE_BITPOS      equ    (0)
HW_TMR2CSR_TIMER_INT_EN_BITPOS      equ    (1)
HW_TMR2CSR_INVERT_BITPOS            equ    (2)
HW_TMR2CSR_TIMER_CONTROL_BITPOS     equ    (3)
HW_TMR2CSR_TIMER_STATUS_BITPOS      equ    (7)
HW_TMR2CSR_TIMER_MODE_BITPOS        equ    (8)
HW_TMR2CSR_CLKGT_BITPOS             equ    (23)

HW_TMR2CSR_TIMER_ENABLE_WIDTH       equ    (1)
HW_TMR2CSR_TIMER_INT_EN_WIDTH       equ    (1)
HW_TMR2CSR_INVERT_WIDTH             equ    (1)
HW_TMR2CSR_TIMER_CONTROL_WIDTH      equ    (3)
HW_TMR2CSR_TIMER_STATUS_WIDTH       equ    (1)
HW_TMR2CSR_TIMER_MODE_WIDTH         equ    (2)
HW_TMR2CSR_CLKGT_WIDTH              equ    (1)

HW_TMR2CSR_TIMER_ENABLE_SETMASK     equ    (((1<<HW_TMR2CSR_TIMER_ENABLE_WIDTH)-1)<<HW_TMR2CSR_TIMER_ENABLE_BITPOS)
HW_TMR2CSR_TIMER_INT_EN_SETMASK     equ    (((1<<HW_TMR2CSR_TIMER_INT_EN_WIDTH)-1)<<HW_TMR2CSR_TIMER_INT_EN_BITPOS)
HW_TMR2CSR_INVERT_SETMASK           equ    (((1<<HW_TMR2CSR_INVERT_WIDTH)-1)<<HW_TMR2CSR_INVERT_BITPOS)
HW_TMR2CSR_TIMER_CONTROL_SETMASK    equ    (((1<<HW_TMR2CSR_TIMER_CONTROL_WIDTH)-1)<<HW_TMR2CSR_TIMER_CONTROL_BITPOS)
HW_TMR2CSR_TIMER_STATUS_SETMASK     equ    (((1<<HW_TMR2CSR_TIMER_STATUS_WIDTH)-1)<<HW_TMR2CSR_TIMER_STATUS_BITPOS)
HW_TMR2CSR_TIMER_MODE_SETMASK       equ    (((1<<HW_TMR2CSR_TIMER_MODE_WIDTH)-1)<<HW_TMR2CSR_TIMER_MODE_BITPOS)
HW_TMR2CSR_CLKGT_SETMASK            equ    (((1<<HW_TMR2CSR_CLKGT_WIDTH)-1)<<HW_TMR2CSR_CLKGT_BITPOS)

HW_TMR2CSR_TIMER_ENABLE_CLRMASK     equ    (~HW_TMR2CSR_TIMER_ENABLE_SETMASK)
HW_TMR2CSR_TIMER_INT_EN_CLRMASK     equ    (~HW_TMR2CSR_TIMER_INT_EN_SETMASK)
HW_TMR2CSR_INVERT_CLRMASK           equ    (~HW_TMR2CSR_INVERT_SETMASK)
HW_TMR2CSR_TIMER_CONTROL_CLRMASK    equ    (~HW_TMR2CSR_TIMER_CONTROL_SETMASK)
HW_TMR2CSR_TIMER_STATUS_CLRMASK     equ    (~HW_TMR2CSR_TIMER_STATUS_SETMASK)
HW_TMR2CSR_TIMER_MODE_CLRMASK       equ    (~HW_TMR2CSR_TIMER_MODE_SETMASK)
HW_TMR2CSR_CLKGT_CLRMASK            equ    (~HW_TMR2CSR_CLKGT_SETMASK)

;///////////////////////////////////////////////////////////////////////////////
;  TIMER CSR (HW_TMR3CSR) Bit Definitions
HW_TMR3CSR_TIMER_ENABLE_BITPOS      equ    (0)
HW_TMR3CSR_TIMER_INT_EN_BITPOS      equ    (1)
HW_TMR3CSR_INVERT_BITPOS            equ    (2)
HW_TMR3CSR_TIMER_CONTROL_BITPOS     equ    (3)
HW_TMR3CSR_TIMER_STATUS_BITPOS      equ    (7)
HW_TMR3CSR_TIMER_MODE_BITPOS        equ    (8)
HW_TMR3CSR_CLKGT_BITPOS             equ    (23)

HW_TMR3CSR_TIMER_ENABLE_WIDTH       equ    (1)
HW_TMR3CSR_TIMER_INT_EN_WIDTH       equ    (1)
HW_TMR3CSR_INVERT_WIDTH             equ    (1)
HW_TMR3CSR_TIMER_CONTROL_WIDTH      equ    (3)
HW_TMR3CSR_TIMER_STATUS_WIDTH       equ    (1)
HW_TMR3CSR_TIMER_MODE_WIDTH         equ    (2)
HW_TMR3CSR_CLKGT_WIDTH              equ    (1)

HW_TMR3CSR_TIMER_ENABLE_SETMASK     equ    (((1<<HW_TMR3CSR_TIMER_ENABLE_WIDTH)-1)<<HW_TMR3CSR_TIMER_ENABLE_BITPOS)
HW_TMR3CSR_TIMER_INT_EN_SETMASK     equ    (((1<<HW_TMR3CSR_TIMER_INT_EN_WIDTH)-1)<<HW_TMR3CSR_TIMER_INT_EN_BITPOS)
HW_TMR3CSR_INVERT_SETMASK           equ    (((1<<HW_TMR3CSR_INVERT_WIDTH)-1)<<HW_TMR3CSR_INVERT_BITPOS)
HW_TMR3CSR_TIMER_CONTROL_SETMASK    equ    (((1<<HW_TMR3CSR_TIMER_CONTROL_WIDTH)-1)<<HW_TMR3CSR_TIMER_CONTROL_BITPOS)
HW_TMR3CSR_TIMER_STATUS_SETMASK     equ    (((1<<HW_TMR3CSR_TIMER_STATUS_WIDTH)-1)<<HW_TMR3CSR_TIMER_STATUS_BITPOS)
HW_TMR3CSR_TIMER_MODE_SETMASK       equ    (((1<<HW_TMR3CSR_TIMER_MODE_WIDTH)-1)<<HW_TMR3CSR_TIMER_MODE_BITPOS)
HW_TMR3CSR_CLKGT_SETMASK            equ    (((1<<HW_TMR3CSR_CLKGT_WIDTH)-1)<<HW_TMR3CSR_CLKGT_BITPOS)

HW_TMR3CSR_TIMER_ENABLE_CLRMASK     equ    (~HW_TMR3CSR_TIMER_ENABLE_SETMASK)
HW_TMR3CSR_TIMER_INT_EN_CLRMASK     equ    (~HW_TMR3CSR_TIMER_INT_EN_SETMASK)
HW_TMR3CSR_INVERT_CLRMASK           equ    (~HW_TMR3CSR_INVERT_SETMASK)
HW_TMR3CSR_TIMER_CONTROL_CLRMASK    equ    (~HW_TMR3CSR_TIMER_CONTROL_SETMASK)
HW_TMR3CSR_TIMER_STATUS_CLRMASK     equ    (~HW_TMR3CSR_TIMER_STATUS_SETMASK)
HW_TMR3CSR_TIMER_MODE_CLRMASK       equ    (~HW_TMR3CSR_TIMER_MODE_SETMASK)
HW_TMR3CSR_CLKGT_CLRMASK            equ    (~HW_TMR3CSR_CLKGT_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;       int TIMER_ENABLE              :1;
;       int TIMER_INT_EN              :1;
;       int INVERT                    :1;
;       int TIMER_CONTROL             :3;
;       int RSVD0                     :1;
;       int TIMER_STATUS              :1;
;       int TIMER_MODE                :2;
;       int RSVD1                     :13;
;       int CLKGT                     :1;
;    } B;
;    int I;
;} timercsr_type;

;#define HW_TMR0CSR        (*(volatile timercsr_type _X*) (HW_TMR0_BASEADDR+HW_TMRCSR))  /* Timer0 Control Status Register */
;#define HW_TMR1CSR        (*(volatile timercsr_type _X*) (HW_TMR1_BASEADDR+HW_TMRCSR))  /* Timer1 Control Status Register */
;#define HW_TMR2CSR        (*(volatile timercsr_type _X*) (HW_TMR2_BASEADDR+HW_TMRCSR))  /* Timer2 Control Status Register */
;#define HW_TMR3CSR        (*(volatile timercsr_type _X*) (HW_TMR3_BASEADDR+HW_TMRCSR))  /* Timer3 Control Status Register */
;endc

;///////////////////////////////////////////////////////////////////////////////
;  TIMER CNTR register (HW_TMR0CNTR) Bit Definitions
HW_TMR0CNTR_COUNT_BITPOS           equ    0
HW_TMR0CNTR_COUNT_WIDTH            equ    24
HW_TMR0CNTR_COUNT_SETMASK          equ    (((1<<HW_TMR0CNTR_COUNT_WIDTH)-1)<<HW_TMR0CNTR_COUNT_BITPOS)
HW_TMR0CNTR_COUNT_CLRMASK          equ    (~HW_TMR0CNTR_COUNT_SETMASK)

;///////////////////////////////////////////////////////////////////////////////
;  TIMER CNTR register (HW_TMR1CNTR) Bit Definitions
HW_TMR1CNTR_COUNT_BITPOS           equ    0
HW_TMR1CNTR_COUNT_WIDTH            equ    24
HW_TMR1CNTR_COUNT_SETMASK          equ    (((1<<HW_TMR1CNTR_COUNT_WIDTH)-1)<<HW_TMR1CNTR_COUNT_BITPOS)
HW_TMR1CNTR_COUNT_CLRMASK          equ    (~HW_TMR1CNTR_COUNT_SETMASK)

;///////////////////////////////////////////////////////////////////////////////
;  TIMER CNTR register (HW_TMR2CNTR) Bit Definitions
HW_TMR2CNTR_COUNT_BITPOS           equ    0
HW_TMR2CNTR_COUNT_WIDTH            equ    24
HW_TMR2CNTR_COUNT_SETMASK          equ    (((1<<HW_TMR2CNTR_COUNT_WIDTH)-1)<<HW_TMR2CNTR_COUNT_BITPOS)
HW_TMR2CNTR_COUNT_CLRMASK          equ    (~HW_TMR2CNTR_COUNT_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;       int COUNT                    :24;
;    } B;
;    int I;
;} tmrcntr_type;
;#define HW_TMR0CNTR        (*(volatile tmrcntr_type _X*) (HW_TMR0_BASEADDR+HW_TMRCNTR))  /* Timer0 Count Register */
;#define HW_TMR1CNTR        (*(volatile tmrcntr_type _X*) (HW_TMR1_BASEADDR+HW_TMRCNTR))  /* Timer1 Count Register */
;#define HW_TMR2CNTR        (*(volatile tmrcntr_type _X*) (HW_TMR2_BASEADDR+HW_TMRCNTR))  /* Timer2 Count Register */
;#define HW_TMR3CNTR        (*(volatile tmrcntr_type _X*) (HW_TMR3_BASEADDR+HW_TMRCNTR))  /* Timer3 Count Register */
;endc


 ;*********************  REGISTER ALIAS DEFINES TO MATCH LEGACY CODE *******************************
 ;
 ; The following defines were added to match regs3410.inc definition to build SDK2XXX code without needing 
 ; to update the actual files. Only the defines needed to build SDK2.400 were added.   
HW_TIMER_BASEADDR                       equ     $F100

HW_TIMER0_BASEADDR                      equ     HW_TIMER_BASEADDR
HW_TIMER1_BASEADDR                      equ     HW_TIMER0_BASEADDR+$40
HW_TIMER2_BASEADDR                      equ     HW_TIMER1_BASEADDR+$40
HW_TIMER3_BASEADDR                      equ     HW_TIMER2_BASEADDR+$40

HW_TMR0CR                 				equ		HW_TMR0_BASEADDR
HW_TMR1CR                               equ		HW_TMR1_BASEADDR
HW_TMR2CR                               equ		HW_TMR2_BASEADDR
HW_TMR3CR                               equ     HW_TIMER3_BASEADDR

HW_TMRCR_TE_BITPOS                      equ     0   ; Timer enable
HW_TMR3CR_CG_BITPOS                     equ     23  ; Timer clock gating control
HW_TMR3CR_CG_SETMASK                    equ     1<<HW_TMR3CR_CG_BITPOS
HW_TMR3CR_CG_CLRMASK                    equ     ~HW_TMR3CR_CG_SETMASK
   endif



