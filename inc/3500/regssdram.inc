;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Copyright(C) SigmaTel, Inc. 2002-2003
;;  File        : regssdram.inc
;;  Description : Mixed Signal IP Register definition
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

    if !(@def(regssdraminc))
regssdraminc            equ         1

;C insert
;#include "types.h"
;endc

HW_SDRAM_BASEADDR                       equ     $F900

;ASM Only
HW_SDRAM_CSR                            equ (HW_SDRAM_BASEADDR)     ; mask = $ffffff
HW_SDRAM_ADDR1                          equ (HW_SDRAM_BASEADDR+1)   ; mask = $ffffff
HW_SDRAM_ADDR2                          equ (HW_SDRAM_BASEADDR+2)   ; mask = $00000f
HW_SDRAM_SYSADDR                        equ (HW_SDRAM_BASEADDR+3)   ; mask = $00ffff
HW_SDRAM_SIZE                           equ (HW_SDRAM_BASEADDR+4)   ; mask = $03ffff
HW_SDRAM_TIMER1                         equ (HW_SDRAM_BASEADDR+5)   ; mask = $ffffff
HW_SDRAM_TIMER2                         equ (HW_SDRAM_BASEADDR+6)   ; mask = $0fffff
HW_SDRAM_BAR                            equ (HW_SDRAM_BASEADDR+7)   ; mask = $00ffff
HW_SDRAM_MR                             equ (HW_SDRAM_BASEADDR+8)   ; mask = $00ffff
HW_SDRAM_DBAR1                          equ (HW_SDRAM_BASEADDR+9)   ; mask = $ffffff
HW_SDRAM_DBAR2                          equ (HW_SDRAM_BASEADDR+10)  ; mask = $00000f
HW_SDRAM_DMR1                           equ (HW_SDRAM_BASEADDR+11)  ; mask = $ffffff
HW_SDRAM_DMR2                           equ (HW_SDRAM_BASEADDR+12)  ; mask = $00000f
HW_SDRAM_MODE                           equ (HW_SDRAM_BASEADDR+14)  ; mask = $001fff
HW_SDRAM_TYPE                           equ (HW_SDRAM_BASEADDR+15)  ; mask = $00000f
;endasm

;///////////////////////////////////////////////////////////////////////////////
;  SDRAM CSR (HW_SDRAM_CSR) Bit Definitions
HW_SDRAM_CSR_SDRAMEN_BITPOS             equ     0
HW_SDRAM_CSR_IE_BITPOS                  equ     1
HW_SDRAM_CSR_RNW_BITPOS                 equ     2
HW_SDRAM_CSR_KICK_BITPOS                equ     3
HW_SDRAM_CSR_LM_BITPOS                  equ     4
HW_SDRAM_CSR_ISTAT_BITPOS               equ     5
HW_SDRAM_CSR_PWDN_BITPOS                equ     6
HW_SDRAM_CSR_SBYTE_BITPOS               equ     8
HW_SDRAM_CSR_MEM_BITPOS                 equ     10
HW_SDRAM_CSR_BIGE_BITPOS                equ     12
HW_SDRAM_CSR_ASIZE_BITPOS               equ     13
HW_SDRAM_CSR_UKICK_BITPOS               equ     16
HW_SDRAM_CSR_DIV_BITPOS                 equ     17
HW_SDRAM_CSR_MULTI_BITPOS               equ     21
HW_SDRAM_CSR_SDRAM_BITPOS               equ     22
HW_SDRAM_CSR_SIGN_BITPOS                equ     23

HW_SDRAM_CSR_SDRAMEN_WIDTH              equ     1
HW_SDRAM_CSR_IE_WIDTH                   equ     1
HW_SDRAM_CSR_RNW_WIDTH                  equ     1
HW_SDRAM_CSR_KICK_WIDTH                 equ     1
HW_SDRAM_CSR_LM_WIDTH                   equ     1
HW_SDRAM_CSR_ISTAT_WIDTH                equ     1
HW_SDRAM_CSR_PWDN_WIDTH                 equ     1
HW_SDRAM_CSR_SBYTE_WIDTH                equ     2
HW_SDRAM_CSR_MEM_WIDTH                  equ     2
HW_SDRAM_CSR_BIGE_WIDTH                 equ     1
HW_SDRAM_CSR_ASIZE_WIDTH                equ     3
HW_SDRAM_CSR_UKICK_WIDTH                equ     1
HW_SDRAM_CSR_DIV_WIDTH                  equ     4
HW_SDRAM_CSR_MULTI_WIDTH                equ     1
HW_SDRAM_CSR_SDRAM_WIDTH                equ     1
HW_SDRAM_CSR_SIGN_WIDTH                 equ     1

HW_SDRAM_CSR_SDRAMEN_SETMASK            equ     (((1<<HW_SDRAM_CSR_SDRAMEN_WIDTH)-1)<<HW_SDRAM_CSR_SDRAMEN_BITPOS)
HW_SDRAM_CSR_IE_SETMASK                 equ     (((1<<HW_SDRAM_CSR_IE_WIDTH)-1)<<HW_SDRAM_CSR_IE_BITPOS)
HW_SDRAM_CSR_RNW_SETMASK                equ     (((1<<HW_SDRAM_CSR_RNW_WIDTH)-1)<<HW_SDRAM_CSR_RNW_BITPOS)
HW_SDRAM_CSR_KICK_SETMASK               equ     (((1<<HW_SDRAM_CSR_KICK_WIDTH)-1)<<HW_SDRAM_CSR_KICK_BITPOS)
HW_SDRAM_CSR_LM_SETMASK                 equ     (((1<<HW_SDRAM_CSR_LM_WIDTH)-1)<<HW_SDRAM_CSR_LM_BITPOS)
HW_SDRAM_CSR_ISTAT_SETMASK              equ     (((1<<HW_SDRAM_CSR_ISTAT_WIDTH)-1)<<HW_SDRAM_CSR_ISTAT_BITPOS)
HW_SDRAM_CSR_PWDN_SETMASK               equ     (((1<<HW_SDRAM_CSR_PWDN_WIDTH)-1)<<HW_SDRAM_CSR_PWDN_BITPOS)
HW_SDRAM_CSR_SBYTE_SETMASK              equ     (((1<<HW_SDRAM_CSR_SBYTE_WIDTH)-1)<<HW_SDRAM_CSR_SBYTE_BITPOS)
HW_SDRAM_CSR_MEM_SETMASK                equ     (((1<<HW_SDRAM_CSR_MEM_WIDTH)-1)<<HW_SDRAM_CSR_MEM_BITPOS)
HW_SDRAM_CSR_BIGE_SETMASK               equ     (((1<<HW_SDRAM_CSR_BIGE_WIDTH)-1)<<HW_SDRAM_CSR_BIGE_BITPOS)
HW_SDRAM_CSR_ASIZE_SETMASK              equ     (((1<<HW_SDRAM_CSR_ASIZE_WIDTH)-1)<<HW_SDRAM_CSR_ASIZE_BITPOS)
HW_SDRAM_CSR_UKICK_SETMASK              equ     (((1<<HW_SDRAM_CSR_UKICK_WIDTH)-1)<<HW_SDRAM_CSR_UKICK_BITPOS)
HW_SDRAM_CSR_DIV_SETMASK                equ     (((1<<HW_SDRAM_CSR_DIV_WIDTH)-1)<<HW_SDRAM_CSR_DIV_BITPOS)
HW_SDRAM_CSR_MULTI_SETMASK              equ     (((1<<HW_SDRAM_CSR_MULTI_WIDTH)-1)<<HW_SDRAM_CSR_MULTI_BITPOS)
HW_SDRAM_CSR_SDRAM_SETMASK              equ     (((1<<HW_SDRAM_CSR_SDRAM_WIDTH)-1)<<HW_SDRAM_CSR_SDRAM_BITPOS)
HW_SDRAM_CSR_SIGN_SETMASK               equ     (((1<<HW_SDRAM_CSR_SIGN_WIDTH)-1)<<HW_SDRAM_CSR_SIGN_BITPOS)

HW_SDRAM_CSR_SDRAMEN_CLRMASK            equ     ~HW_SDRAM_CSR_SDRAMEN_SETMASK
HW_SDRAM_CSR_IE_CLRMASK                 equ     ~HW_SDRAM_CSR_IE_SETMASK
HW_SDRAM_CSR_RNW_CLRMASK                equ     ~HW_SDRAM_CSR_RNW_SETMASK
HW_SDRAM_CSR_KICK_CLRMASK               equ     ~HW_SDRAM_CSR_KICK_SETMASK
HW_SDRAM_CSR_LM_CLRMASK                 equ     ~HW_SDRAM_CSR_LM_SETMASK
HW_SDRAM_CSR_ISTAT_CLRMASK              equ     ~HW_SDRAM_CSR_ISTAT_SETMASK
HW_SDRAM_CSR_PWDN_CLRMASK               equ     ~HW_SDRAM_CSR_PWDN_SETMASK
HW_SDRAM_CSR_SBYTE_CLRMASK              equ     ~HW_SDRAM_CSR_SBYTE_SETMASK
HW_SDRAM_CSR_MEM_CLRMASK                equ     ~HW_SDRAM_CSR_MEM_SETMASK
HW_SDRAM_CSR_BIGE_CLRMASK               equ     ~HW_SDRAM_CSR_BIGE_SETMASK
HW_SDRAM_CSR_ASIZE_CLRMASK              equ     ~HW_SDRAM_CSR_ASIZE_SETMASK
HW_SDRAM_CSR_UKICK_CLRMASK              equ     ~HW_SDRAM_CSR_UKICK_SETMASK
HW_SDRAM_CSR_DIV_CLRMASK                equ     ~HW_SDRAM_CSR_DIV_SETMASK
HW_SDRAM_CSR_MULTI_CLRMASK              equ     ~HW_SDRAM_CSR_MULTI_SETMASK
HW_SDRAM_CSR_SDRAM_CLRMASK              equ     ~HW_SDRAM_CSR_SDRAM_SETMASK
HW_SDRAM_CSR_SIGN_CLRMASK               equ     ~HW_SDRAM_CSR_SIGN_SETMASK

;C insert
;typedef union               
;{
;    struct {
;        int SDRAMEN                     :1;
;        int IE                          :1;
;        int RNW                         :1;
;        int KICK                        :1;
;        int LM                          :1;
;        int ISTAT                       :1;
;        int PWDN                        :1;
;        int RSVD                        :1;
;        int SBYTE                       :2;
;        int MEM                         :2;
;        int BIGE                        :1;
;        int ASIZE                       :3;
;        int UKICK                       :1;
;        int DIV                         :4;
;        int MULTI                       :1;
;        int SDRAM                       :1;
;        int SIGN                        :1;
;    } B;
;    int I;
;} sdramcsr_type;

;#define HW_SDRAM_CSR (*(volatile sdramcsr_type _X*) (HW_SDRAM_BASEADDR))        
;#define HW_SDRAM_ADDR1 (*(volatile unsigned int _X*) (HW_SDRAM_BASEADDR+1))
;#define HW_SDRAM_ADDR2 (*(volatile unsigned int _X*) (HW_SDRAM_BASEADDR+2))
;#define HW_SDRAM_SYSADDR (*(volatile unsigned int _X*) (HW_SDRAM_BASEADDR+3))
;#define HW_SDRAM_SIZE (*(volatile unsigned int _X*) (HW_SDRAM_BASEADDR+4))
;#define HW_SDRAM_BAR (*(volatile unsigned int _X*) (HW_SDRAM_BASEADDR+7))
;#define HW_SDRAM_MR (*(volatile unsigned int _X*) (HW_SDRAM_BASEADDR+8))
;#define HW_SDRAM_DBAR1 (*(volatile unsigned int _X*) (HW_SDRAM_BASEADDR+9))
;#define HW_SDRAM_DBAR2 (*(volatile unsigned int _X*) (HW_SDRAM_BASEADDR+10))
;#define HW_SDRAM_DMR1 (*(volatile unsigned int _X*) (HW_SDRAM_BASEADDR+11))
;#define HW_SDRAM_DMR2 (*(volatile unsigned int _X*) (HW_SDRAM_BASEADDR+12))
;endc

;///////////////////////////////////////////////////////////////////////////////
;  SDRAM Start Address Low Register (HW_SDRAM_ADDR1) Bit Definitions
HW_SDRAM_ADDR1_XA_BITPOS                 equ     0

HW_SDRAM_ADDR1_XA_SETMASK                equ     $FFFFFF<<HW_SDRAM_ADDR1_XA_BITPOS

HW_SDRAM_ADDR1_XA_CLRMASK                equ     ~HW_SDRAM_ADDR1_XA_SETMASK

;///////////////////////////////////////////////////////////////////////////////
;  SDRAM Start Address High Register (HW_SDRAM_ADDR2) Bit Definitions
HW_SDRAM_ADDR2_XA_BITPOS                 equ     0

HW_SDRAM_ADDR2_XA_SETMASK                equ     $1F<<HW_SDRAM_ADDR2_XA_BITPOS

HW_SDRAM_ADDR2_XA_CLRMASK                equ     ~HW_SDRAM_ADDR2_XA_SETMASK

;///////////////////////////////////////////////////////////////////////////////
;  System Start Address Register (HW_SDRAM_SYSADDR) Bit Definitions
HW_SDRAM_SYSADDR_XA_BITPOS                 equ     0

HW_SDRAM_SYSADDR_XA_SETMASK                equ     $FFFF<<HW_SDRAM_SYSADDR_XA_BITPOS

HW_SDRAM_SYSADDR_XA_CLRMASK                equ     ~HW_SDRAM_SYSADDR_XA_SETMASK

;///////////////////////////////////////////////////////////////////////////////
;  Number of Bytes to be transfered Register (HW_SDRAM_SIZE) Bit Definitions
HW_SDRAM_SIZE_XA_BITPOS                 equ     0

HW_SDRAM_SIZE_XA_SETMASK                equ     $3FFFF<<HW_SDRAM_SIZE_XA_BITPOS

HW_SDRAM_SIZE_XA_CLRMASK                equ     ~HW_SDRAM_SIZE_XA_SETMASK

;///////////////////////////////////////////////////////////////////////////////
;  SDRAM Timer1 Register (HW_SDRAM_TIMER1) Bit Definitions
HW_SDRAM_TIMER1_INIT_BITPOS             equ     0
HW_SDRAM_TIMER1_TRP_BITPOS              equ     16
HW_SDRAM_TIMER1_TRFC_BITPOS             equ     20

HW_SDRAM_TIMER1_INIT_WIDTH              equ     16
HW_SDRAM_TIMER1_TRP_WIDTH               equ     4
HW_SDRAM_TIMER1_TRFC_WIDTH              equ     4

HW_SDRAM_TIMER1_INIT_SETMASK            equ     (((1<<HW_SDRAM_TIMER1_INIT_WIDTH)-1)<<HW_SDRAM_TIMER1_INIT_BITPOS)
HW_SDRAM_TIMER1_TRP_SETMASK             equ     (((1<<HW_SDRAM_TIMER1_TRP_WIDTH)-1)<<HW_SDRAM_TIMER1_TRP_BITPOS)
HW_SDRAM_TIMER1_TRFC_SETMASK            equ     (((1<<HW_SDRAM_TIMER1_TRFC_WIDTH)-1)<<HW_SDRAM_TIMER1_TRFC_BITPOS)

HW_SDRAM_TIMER1_INIT_CLRMASK            equ     ~HW_SDRAM_TIMER1_INIT_SETMASK
HW_SDRAM_TIMER1_TRP_CLRMASK             equ     ~HW_SDRAM_TIMER1_TRP_SETMASK
HW_SDRAM_TIMER1_TRFC_CLRMASK            equ     ~HW_SDRAM_TIMER1_TRFC_SETMASK

;C insert
;typedef union               
;{
;    struct {
;        int INIT                :16;
;        int TRP                 :4;
;        int TRFC                :4;
;    } B;
;    int I;
;} sdramtimer1_type;

;#define HW_SDRAM_TIMER1 (*(volatile sdramtimer1_type _X*) (HW_SDRAM_BASEADDR+5))
;endc

;///////////////////////////////////////////////////////////////////////////////
;  SDRAM Timer2 Register (HW_SDRAM_TIMER2) Bit Definitions
HW_SDRAM_TIMER2_TXSR_BITPOS             equ     0
HW_SDRAM_TIMER2_TREF_BITPOS             equ     4
HW_SDRAM_TIMER2_TRCD_BITPOS             equ     16

HW_SDRAM_TIMER2_TXSR_WIDTH              equ     4
HW_SDRAM_TIMER2_TREF_WIDTH              equ     12
HW_SDRAM_TIMER2_TRCD_WIDTH              equ     4

HW_SDRAM_TIMER2_TXSR_SETMASK            equ     (((1<<HW_SDRAM_TIMER2_TXSR_WIDTH)-1)<<HW_SDRAM_TIMER2_TXSR_BITPOS)
HW_SDRAM_TIMER2_TREF_SETMASK            equ     (((1<<HW_SDRAM_TIMER2_TREF_WIDTH)-1)<<HW_SDRAM_TIMER2_TREF_BITPOS)
HW_SDRAM_TIMER2_TRCD_SETMASK            equ     (((1<<HW_SDRAM_TIMER2_TRCD_WIDTH)-1)<<HW_SDRAM_TIMER2_TRCD_BITPOS)

HW_SDRAM_TIMER2_TXSR_CLRMASK            equ     ~HW_SDRAM_TIMER2_TXSR_SETMASK
HW_SDRAM_TIMER2_TREF_CLRMASK            equ     ~HW_SDRAM_TIMER2_TREF_SETMASK
HW_SDRAM_TIMER2_TRCD_CLRMASK            equ     ~HW_SDRAM_TIMER2_TRCD_SETMASK

;C insert
;typedef union               
;{
;    struct {
;        int TXSR                :4;
;        int TREF                :12;
;        int TRCD                :4;
;        int RSVD                :4; 
;    } B;
;    int I;
;} sdramtimer2_type;

;#define HW_SDRAM_TIMER2 (*(volatile sdramtimer2_type _X*) (HW_SDRAM_BASEADDR+6))
;endc

;///////////////////////////////////////////////////////////////////////////////
;  System Modulo Base Address Register (HW_SDRAM_BAR) Bit Definitions
HW_SDRAM_BAR_XA_BITPOS                 equ     0

HW_SDRAM_BAR_XA_SETMASK                equ     $FFFF<<HW_SDRAM_BAR_XA_BITPOS

HW_SDRAM_BAR_XA_CLRMASK                equ     ~HW_SDRAM_BAR_XA_SETMASK

;///////////////////////////////////////////////////////////////////////////////
;  System Modulo Register (HW_SDRAM_MR) Bit Definitions
HW_SDRAM_MR_XA_BITPOS                 equ     0

HW_SDRAM_MR_XA_SETMASK                equ     $FFFF<<HW_SDRAM_MR_XA_BITPOS

HW_SDRAM_MR_XA_CLRMASK                equ     ~HW_SDRAM_MR_XA_SETMASK

;///////////////////////////////////////////////////////////////////////////////
;  SDRAM Mode Register (HW_SDRAM_MODE) Bit Definitions
HW_SDRAM_MODE_XA_BITPOS                 equ     0

HW_SDRAM_MODE_XA_WIDTH                  equ     14

HW_SDRAM_MODE_XA_SETMASK                equ     (((1<<HW_SDRAM_MODE_XA_WIDTH)-1)<<HW_SDRAM_MODE_XA_BITPOS)

HW_SDRAM_MODE_XA_CLRMASK                equ     ~HW_SDRAM_MODE_XA_SETMASK

;C insert
;typedef union               
;{
;    struct {
;        int VALUE               :14;
;        int RSVD                :10; 
;    } B;
;    int I;
;} sdrammode_type;

;#define HW_SDRAM_MODE (*(volatile sdrammode_type _X*) (HW_SDRAM_BASEADDR+14))
;endc

;///////////////////////////////////////////////////////////////////////////////
;  SDRAM Type Register (HW_SDRAM_TYPE) Bit Definitions
HW_SDRAM_TYPE_COLWIDTH_BITPOS                 equ     0
HW_SDRAM_TYPE_ROWWIDTH_BITPOS                 equ     4

HW_SDRAM_TYPE_COLWIDTH_WIDTH                  equ     4
HW_SDRAM_TYPE_ROWWIDTH_WIDTH                  equ     4

HW_SDRAM_TYPE_COLWIDTH_SETMASK                equ     (((1<<HW_SDRAM_TYPE_COLWIDTH_WIDTH)-1)<<HW_SDRAM_TYPE_COLWIDTH_BITPOS)
HW_SDRAM_TYPE_ROWWIDTH_SETMASK                equ     (((1<<HW_SDRAM_TYPE_ROWWIDTH_WIDTH)-1)<<HW_SDRAM_TYPE_ROWWIDTH_BITPOS)

HW_SDRAM_TYPE_COLWIDTH_CLRMASK                equ     (~HW_SDRAM_TYPE_COLWIDTH_SETMASK)
HW_SDRAM_TYPE_ROWWIDTH_CLRMASK                equ     (~HW_SDRAM_TYPE_ROWWIDTH_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int COLWIDTH               :4;
;        int ROWWIDTH               :4; 
;    } B;
;    int I;
;} sdramtype_type;

;#define HW_SDRAM_TYPE (*(volatile sdramtype_type _X*) (HW_SDRAM_BASEADDR+14))
;endc

  endif





