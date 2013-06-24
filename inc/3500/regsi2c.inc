;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2003
;
; Filename: regsI2C.inc
; Description: Register definitions for GPFLASH interface
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
    if !(@def(regsi2cinc))
regsi2cinc            equ         1

;C insert
;#include "types.h"
;endc

;///////////////////////////////////////////////////////////////////////////////
;//   I2C STMP Registers
;///////////////////////////////////////////////////////////////////////////////

HW_I2C_BASEADDR                         equ     ($FFE5)

;ASM Only
HW_I2CDIV                               equ     (HW_I2C_BASEADDR)
HW_I2CDAT                               equ     (HW_I2C_BASEADDR+1)
HW_I2CCSR                               equ     (HW_I2C_BASEADDR+2)
;endasm

;///////////////////////////////////////////////////////////////////////////////
;//  I2C Clock Divider Register (HW_I2CDIV) Bit Definitions

HW_I2CDIV_FACT_BITPOS                   equ     (1)

HW_I2CDIV_FACT_SETMASK                  equ     ($FF<<HW_I2CDIV_FACT_BITPOS)

HW_I2CDIV_FACT_CLRMASK                  equ     (~HW_I2CDIV_FACT_SETMASK)

;C insert
;typedef union               /* I2C Clock Divider Register */
;{
;    struct {
;        int                :1; 
;        unsigned FACT      :8;
;    } B;
;    int I;
;    unsigned U;
;} i2cdivr_type;
;#define HW_I2CDIV (*(volatile i2cdivr_type _X*) (HW_I2C_BASEADDR))       /* I2C Divfact Registers        */
;endc


;///////////////////////////////////////////////////////////////////////////////
;//  I2C Data Register (HW_I2CDAT) Bit Definitions

HW_I2CDAT_DATA_BITPOS                   equ     (0)

HW_I2CDAT_DATA_SETMASK                  equ     ($FFFFFF)

HW_I2CDAT_DATA_CLRMASK                  equ     (~HW_I2CDAT_DATA_SETMASK)

;C insert
;typedef union               /* I2C Data Register */
;{
;    struct {
;         unsigned DATA :24; 
;    } B;
;    int I;
;    unsigned U;
;} i2cdatr_type;
;#define HW_I2CDAT (*(volatile i2cdatr_type _X*) (HW_I2C_BASEADDR+1)) /* I2C Data Registers (I2CDAT)      */
;endc


;///////////////////////////////////////////////////////////////////////////////
;//  I2C Control Status Register (HW_I2CCSR) Bit Definitions
HW_I2CCSR_I2C_EN_BITPOS                 equ     (0)
HW_I2CCSR_RIE_BITPOS                    equ     (1)
HW_I2CCSR_BUSY_BITPOS                   equ     (2)
HW_I2CCSR_ARB_LOST_BITPOS               equ     (3)
HW_I2CCSR_TIE_BITPOS                    equ     (4)
HW_I2CCSR_MODE_BITPOS                   equ     (5)
HW_I2CCSR_RDR_BITPOS                    equ     (6)
HW_I2CCSR_TDE_BITPOS                    equ     (7)
HW_I2CCSR_RWN_BITPOS                    equ     (8)
HW_I2CCSR_WL_BITPOS                     equ     (9)
HW_I2CCSR_WL0_BITPOS                    equ     (9)
HW_I2CCSR_WL1_BITPOS                    equ     (10)
HW_I2CCSR_TREQ_BITPOS                   equ     (11)
HW_I2CCSR_ROFL_BITPOS                   equ     (12)
HW_I2CCSR_TUFL_BITPOS                   equ     (13)
HW_I2CCSR_ACKF_BITPOS                   equ     (14)
HW_I2CCSR_B_CNT_BITPOS                  equ     (15)
HW_I2CCSR_B_CNT0_BITPOS                 equ     (15)
HW_I2CCSR_B_CNT1_BITPOS                 equ     (16)
HW_I2CCSR_L_WORD_BITPOS                 equ     (17)
HW_I2CCSR_SUBA_BITPOS                   equ     (18)
HW_I2CCSR_ROFLCL_BITPOS                 equ     (19)
HW_I2CCSR_TUFLCL_BITPOS                 equ     (20)
HW_I2CCSR_ONEBYTE_BITPOS                equ     (21)

HW_I2CCSR_I2C_EN_SETMASK                equ     (1<<HW_I2CCSR_I2C_EN_BITPOS)
HW_I2CCSR_RIE_SETMASK                   equ     (1<<HW_I2CCSR_RIE_BITPOS)
HW_I2CCSR_BUSY_SETMASK                  equ     (1<<HW_I2CCSR_BUSY_BITPOS)
HW_I2CCSR_ARB_LOST_SETMASK              equ     (1<<HW_I2CCSR_ARB_LOST_BITPOS)
HW_I2CCSR_TIE_SETMASK                   equ     (1<<HW_I2CCSR_TIE_BITPOS)
HW_I2CCSR_MODE_SETMASK                  equ     (1<<HW_I2CCSR_MODE_BITPOS)
HW_I2CCSR_RDR_SETMASK                   equ     (1<<HW_I2CCSR_RDR_BITPOS)
HW_I2CCSR_TDE_SETMASK                   equ     (1<<HW_I2CCSR_TDE_BITPOS)
HW_I2CCSR_RWN_SETMASK                   equ     (1<<HW_I2CCSR_RWN_BITPOS)
HW_I2CCSR_WL_SETMASK                    equ     (3<<HW_I2CCSR_WL_BITPOS)
HW_I2CCSR_WL0_SETMASK                   equ     (1<<HW_I2CCSR_WL0_BITPOS)
HW_I2CCSR_WL1_SETMASK                   equ     (1<<HW_I2CCSR_WL1_BITPOS)
HW_I2CCSR_TREQ_SETMASK                  equ     (1<<HW_I2CCSR_TREQ_BITPOS)
HW_I2CCSR_ROFL_SETMASK                  equ     (1<<HW_I2CCSR_ROFL_BITPOS)
HW_I2CCSR_TUFL_SETMASK                  equ     (1<<HW_I2CCSR_TUFL_BITPOS)
HW_I2CCSR_ACKF_SETMASK                  equ     (1<<HW_I2CCSR_ACKF_BITPOS)
HW_I2CCSR_B_CNT_SETMASK                 equ     (3<<HW_I2CCSR_B_CNT_BITPOS)
HW_I2CCSR_B_CNT0_SETMASK                equ     (1<<HW_I2CCSR_B_CNT0_BITPOS)
HW_I2CCSR_B_CNT1_SETMASK                equ     (1<<HW_I2CCSR_B_CNT1_BITPOS)
HW_I2CCSR_L_WORD_SETMASK                equ     (1<<HW_I2CCSR_L_WORD_BITPOS)
HW_I2CCSR_SUBA_SETMASK                  equ     (1<<HW_I2CCSR_SUBA_BITPOS)
HW_I2CCSR_ROFLCL_SETMASK                equ     (1<<HW_I2CCSR_ROFLCL_BITPOS)
HW_I2CCSR_TUFLCL_SETMASK                equ     (1<<HW_I2CCSR_TUFLCL_BITPOS)
HW_I2CCSR_ONEBYTE_SETMASK               equ     (1<<HW_I2CCSR_ONEBYTE_BITPOS)

HW_I2CCSR_I2C_EN_CLRMASK                equ     (~HW_I2CCSR_I2C_EN_SETMASK)
HW_I2CCSR_RIE_CLRMASK                   equ     (~HW_I2CCSR_RIE_SETMASK)
HW_I2CCSR_BUSY_CLRMASK                  equ     (~HW_I2CCSR_BUSY_SETMASK)
HW_I2CCSR_ARB_LOST_CLRMASK              equ     (~HW_I2CCSR_ARB_LOST_SETMASK)
HW_I2CCSR_TIE_CLRMASK                   equ     (~HW_I2CCSR_TIE_SETMASK)
HW_I2CCSR_MODE_CLRMASK                  equ     (~HW_I2CCSR_MODE_SETMASK)
HW_I2CCSR_RDR_CLRMASK                   equ     (~HW_I2CCSR_RDR_SETMASK)
HW_I2CCSR_TDE_CLRMASK                   equ     (~HW_I2CCSR_TDE_SETMASK)
HW_I2CCSR_RWN_CLRMASK                   equ     (~HW_I2CCSR_RWN_SETMASK)
HW_I2CCSR_WL_CLRMASK                    equ     (~HW_I2CCSR_WL_SETMASK)
HW_I2CCSR_WL0_CLRMASK                   equ     (~HW_I2CCSR_WL0_SETMASK)
HW_I2CCSR_WL1_CLRMASK                   equ     (~HW_I2CCSR_WL1_SETMASK)
HW_I2CCSR_TREQ_CLRMASK                  equ     (~HW_I2CCSR_TREQ_SETMASK)
HW_I2CCSR_ROFL_CLRMASK                  equ     (~HW_I2CCSR_ROFL_SETMASK)
HW_I2CCSR_TUFL_CLRMASK                  equ     (~HW_I2CCSR_TUFL_SETMASK)
HW_I2CCSR_ACKF_CLRMASK                  equ     (~HW_I2CCSR_ACKF_SETMASK)
HW_I2CCSR_B_CNT_CLRMASK                 equ     (~HW_I2CCSR_B_CNT_SETMASK)
HW_I2CCSR_B_CNT0_CLRMASK                equ     (~HW_I2CCSR_B_CNT0_SETMASK)
HW_I2CCSR_B_CNT1_CLRMASK                equ     (~HW_I2CCSR_B_CNT1_SETMASK)
HW_I2CCSR_L_WORD_CLRMASK                equ     (~HW_I2CCSR_L_WORD_SETMASK)
HW_I2CCSR_SUBA_CLRMASK                  equ     (~HW_I2CCSR_SUBA_SETMASK)
HW_I2CCSR_ROFLCL_CLRMASK                equ     (~HW_I2CCSR_ROFLCL_SETMASK)
HW_I2CCSR_TUFLCL_CLRMASK                equ     (~HW_I2CCSR_TUFLCL_SETMASK)
HW_I2CCSR_ONEBYTE_CLRMASK               equ     (~HW_I2CCSR_ONEBYTE_SETMASK)

;C insert
;typedef union               /* I2C Control Register         */
;{
;    struct {
;        int I2C_EN      :1; /* Peripheral Enable            */
;        int RIE         :1; /* Receiver Interrupt Enable        */
;        int BUSY        :1; /* I2C Bus Busy             */
;        int ARBLOST     :1; /* Aritration lost          */
;        int TIE         :1; /* Transmitter Interrupt Enable     */
;        int MODE        :1; /* Operating Mode Bit           */
;        int RDR         :1; /* Receiver Data Ready          */
;       int TDE         :1; /* Transmitter Data Empty       */
;       int RWN         :1; /* Read/Not Write           */
;       unsigned WL     :2; /* Word Length              */
;        int TREQ        :1; /* DSP Transmit Request         */
;        int ROFL        :1; /* Receiver Overflow            */
;        int TUFL        :1; /* Transmitter Underflow        */
;        int ACKF        :1; /* Acknowledge Failure          */
;        unsigned BCNT   :2; /* Byte Count               */
;        int LWORD       :1; /* Last Word                */
;        int SUBA        :1; /* Sub Address              */
;        int ROFLCL      :1; /* Receiver Overflow Clear      */
;        int TUFLCL      :1; /* Transmitter Underflow Clear      */
;        int ONEBYTE     :1; /* Special One Data Byte Transmission Mode */
;    } B;
;    int I;
;    unsigned U;
;} i2ccsr_type;
;#define HW_I2CCSR (*(volatile i2ccsr_type _X*) (HW_I2C_BASEADDR+2)) /* I2C Control/Status Register (I2CCSR) */
;endc

    endif