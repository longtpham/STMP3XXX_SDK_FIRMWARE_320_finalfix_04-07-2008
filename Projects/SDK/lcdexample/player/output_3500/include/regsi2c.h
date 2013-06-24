/////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2003
// Filename: regsI2C.inc
// Description: Register definitions for GPFLASH interface
/////////////////////////////////////////////////////////////////////////////////
// The following naming conventions are followed in this file.
// All registers are named using the format...
//     HW_<module>_<regname>
// where <module> is the module name which can be any of the following...
//     USB20
// (Note that when there is more than one copy of a particular module, the
// module name includes a number starting from 0 for the first instance of
// that module)
// <regname> is the specific register within that module
// We also define the following...
//     HW_<module>_<regname>_BITPOS
// which defines the starting bit (i.e. LSB) of a multi bit field
//     HW_<module>_<regname>_SETMASK
// which does something else, and
//     HW_<module>_<regname>_CLRMASK
// which does something else.
// Other rules
//     All caps
//     Numeric identifiers start at 0
#if !(defined(regsi2cinc))
#define regsi2cinc 1

#include "types.h"

/////////////////////////////////////////////////////////////////////////////////
////   I2C STMP Registers
/////////////////////////////////////////////////////////////////////////////////

#define HW_I2C_BASEADDR (0xFFE5)


/////////////////////////////////////////////////////////////////////////////////
////  I2C Clock Divider Register (HW_I2CDIV) Bit Definitions

#define HW_I2CDIV_FACT_BITPOS (1)

#define HW_I2CDIV_FACT_SETMASK (0xFF<<HW_I2CDIV_FACT_BITPOS)

#define HW_I2CDIV_FACT_CLRMASK (~(WORD)HW_I2CDIV_FACT_SETMASK)

typedef union               /* I2C Clock Divider Register */
{
    struct {
        int                :1; 
        unsigned FACT      :8;
    } B;
    int I;
    unsigned U;
} i2cdivr_type;
#define HW_I2CDIV (*(volatile i2cdivr_type _X*) (HW_I2C_BASEADDR))       /* I2C Divfact Registers        */


/////////////////////////////////////////////////////////////////////////////////
////  I2C Data Register (HW_I2CDAT) Bit Definitions

#define HW_I2CDAT_DATA_BITPOS (0)

#define HW_I2CDAT_DATA_SETMASK (0xFFFFFF)

#define HW_I2CDAT_DATA_CLRMASK (~(WORD)HW_I2CDAT_DATA_SETMASK)

typedef union               /* I2C Data Register */
{
    struct {
         unsigned DATA :24; 
    } B;
    int I;
    unsigned U;
} i2cdatr_type;
#define HW_I2CDAT (*(volatile i2cdatr_type _X*) (HW_I2C_BASEADDR+1)) /* I2C Data Registers (I2CDAT)      */


/////////////////////////////////////////////////////////////////////////////////
////  I2C Control Status Register (HW_I2CCSR) Bit Definitions
#define HW_I2CCSR_I2C_EN_BITPOS (0)
#define HW_I2CCSR_RIE_BITPOS (1)
#define HW_I2CCSR_BUSY_BITPOS (2)
#define HW_I2CCSR_ARB_LOST_BITPOS (3)
#define HW_I2CCSR_TIE_BITPOS (4)
#define HW_I2CCSR_MODE_BITPOS (5)
#define HW_I2CCSR_RDR_BITPOS (6)
#define HW_I2CCSR_TDE_BITPOS (7)
#define HW_I2CCSR_RWN_BITPOS (8)
#define HW_I2CCSR_WL_BITPOS (9)
#define HW_I2CCSR_WL0_BITPOS (9)
#define HW_I2CCSR_WL1_BITPOS (10)
#define HW_I2CCSR_TREQ_BITPOS (11)
#define HW_I2CCSR_ROFL_BITPOS (12)
#define HW_I2CCSR_TUFL_BITPOS (13)
#define HW_I2CCSR_ACKF_BITPOS (14)
#define HW_I2CCSR_B_CNT_BITPOS (15)
#define HW_I2CCSR_B_CNT0_BITPOS (15)
#define HW_I2CCSR_B_CNT1_BITPOS (16)
#define HW_I2CCSR_L_WORD_BITPOS (17)
#define HW_I2CCSR_SUBA_BITPOS (18)
#define HW_I2CCSR_ROFLCL_BITPOS (19)
#define HW_I2CCSR_TUFLCL_BITPOS (20)
#define HW_I2CCSR_ONEBYTE_BITPOS (21)

#define HW_I2CCSR_I2C_EN_SETMASK (1<<HW_I2CCSR_I2C_EN_BITPOS)
#define HW_I2CCSR_RIE_SETMASK (1<<HW_I2CCSR_RIE_BITPOS)
#define HW_I2CCSR_BUSY_SETMASK (1<<HW_I2CCSR_BUSY_BITPOS)
#define HW_I2CCSR_ARB_LOST_SETMASK (1<<HW_I2CCSR_ARB_LOST_BITPOS)
#define HW_I2CCSR_TIE_SETMASK (1<<HW_I2CCSR_TIE_BITPOS)
#define HW_I2CCSR_MODE_SETMASK (1<<HW_I2CCSR_MODE_BITPOS)
#define HW_I2CCSR_RDR_SETMASK (1<<HW_I2CCSR_RDR_BITPOS)
#define HW_I2CCSR_TDE_SETMASK (1<<HW_I2CCSR_TDE_BITPOS)
#define HW_I2CCSR_RWN_SETMASK (1<<HW_I2CCSR_RWN_BITPOS)
#define HW_I2CCSR_WL_SETMASK (3<<HW_I2CCSR_WL_BITPOS)
#define HW_I2CCSR_WL0_SETMASK (1<<HW_I2CCSR_WL0_BITPOS)
#define HW_I2CCSR_WL1_SETMASK (1<<HW_I2CCSR_WL1_BITPOS)
#define HW_I2CCSR_TREQ_SETMASK (1<<HW_I2CCSR_TREQ_BITPOS)
#define HW_I2CCSR_ROFL_SETMASK (1<<HW_I2CCSR_ROFL_BITPOS)
#define HW_I2CCSR_TUFL_SETMASK (1<<HW_I2CCSR_TUFL_BITPOS)
#define HW_I2CCSR_ACKF_SETMASK (1<<HW_I2CCSR_ACKF_BITPOS)
#define HW_I2CCSR_B_CNT_SETMASK (3<<HW_I2CCSR_B_CNT_BITPOS)
#define HW_I2CCSR_B_CNT0_SETMASK (1<<HW_I2CCSR_B_CNT0_BITPOS)
#define HW_I2CCSR_B_CNT1_SETMASK (1<<HW_I2CCSR_B_CNT1_BITPOS)
#define HW_I2CCSR_L_WORD_SETMASK (1<<HW_I2CCSR_L_WORD_BITPOS)
#define HW_I2CCSR_SUBA_SETMASK (1<<HW_I2CCSR_SUBA_BITPOS)
#define HW_I2CCSR_ROFLCL_SETMASK (1<<HW_I2CCSR_ROFLCL_BITPOS)
#define HW_I2CCSR_TUFLCL_SETMASK (1<<HW_I2CCSR_TUFLCL_BITPOS)
#define HW_I2CCSR_ONEBYTE_SETMASK (1<<HW_I2CCSR_ONEBYTE_BITPOS)

#define HW_I2CCSR_I2C_EN_CLRMASK (~(WORD)HW_I2CCSR_I2C_EN_SETMASK)
#define HW_I2CCSR_RIE_CLRMASK (~(WORD)HW_I2CCSR_RIE_SETMASK)
#define HW_I2CCSR_BUSY_CLRMASK (~(WORD)HW_I2CCSR_BUSY_SETMASK)
#define HW_I2CCSR_ARB_LOST_CLRMASK (~(WORD)HW_I2CCSR_ARB_LOST_SETMASK)
#define HW_I2CCSR_TIE_CLRMASK (~(WORD)HW_I2CCSR_TIE_SETMASK)
#define HW_I2CCSR_MODE_CLRMASK (~(WORD)HW_I2CCSR_MODE_SETMASK)
#define HW_I2CCSR_RDR_CLRMASK (~(WORD)HW_I2CCSR_RDR_SETMASK)
#define HW_I2CCSR_TDE_CLRMASK (~(WORD)HW_I2CCSR_TDE_SETMASK)
#define HW_I2CCSR_RWN_CLRMASK (~(WORD)HW_I2CCSR_RWN_SETMASK)
#define HW_I2CCSR_WL_CLRMASK (~(WORD)HW_I2CCSR_WL_SETMASK)
#define HW_I2CCSR_WL0_CLRMASK (~(WORD)HW_I2CCSR_WL0_SETMASK)
#define HW_I2CCSR_WL1_CLRMASK (~(WORD)HW_I2CCSR_WL1_SETMASK)
#define HW_I2CCSR_TREQ_CLRMASK (~(WORD)HW_I2CCSR_TREQ_SETMASK)
#define HW_I2CCSR_ROFL_CLRMASK (~(WORD)HW_I2CCSR_ROFL_SETMASK)
#define HW_I2CCSR_TUFL_CLRMASK (~(WORD)HW_I2CCSR_TUFL_SETMASK)
#define HW_I2CCSR_ACKF_CLRMASK (~(WORD)HW_I2CCSR_ACKF_SETMASK)
#define HW_I2CCSR_B_CNT_CLRMASK (~(WORD)HW_I2CCSR_B_CNT_SETMASK)
#define HW_I2CCSR_B_CNT0_CLRMASK (~(WORD)HW_I2CCSR_B_CNT0_SETMASK)
#define HW_I2CCSR_B_CNT1_CLRMASK (~(WORD)HW_I2CCSR_B_CNT1_SETMASK)
#define HW_I2CCSR_L_WORD_CLRMASK (~(WORD)HW_I2CCSR_L_WORD_SETMASK)
#define HW_I2CCSR_SUBA_CLRMASK (~(WORD)HW_I2CCSR_SUBA_SETMASK)
#define HW_I2CCSR_ROFLCL_CLRMASK (~(WORD)HW_I2CCSR_ROFLCL_SETMASK)
#define HW_I2CCSR_TUFLCL_CLRMASK (~(WORD)HW_I2CCSR_TUFLCL_SETMASK)
#define HW_I2CCSR_ONEBYTE_CLRMASK (~(WORD)HW_I2CCSR_ONEBYTE_SETMASK)

typedef union               /* I2C Control Register         */
{
    struct {
        int I2C_EN      :1; /* Peripheral Enable            */
        int RIE         :1; /* Receiver Interrupt Enable        */
        int BUSY        :1; /* I2C Bus Busy             */
        int ARBLOST     :1; /* Aritration lost          */
        int TIE         :1; /* Transmitter Interrupt Enable     */
        int MODE        :1; /* Operating Mode Bit           */
        int RDR         :1; /* Receiver Data Ready          */
       int TDE         :1; /* Transmitter Data Empty       */
       int RWN         :1; /* Read/Not Write           */
       unsigned WL     :2; /* Word Length              */
        int TREQ        :1; /* DSP Transmit Request         */
        int ROFL        :1; /* Receiver Overflow            */
        int TUFL        :1; /* Transmitter Underflow        */
        int ACKF        :1; /* Acknowledge Failure          */
        unsigned BCNT   :2; /* Byte Count               */
        int LWORD       :1; /* Last Word                */
        int SUBA        :1; /* Sub Address              */
        int ROFLCL      :1; /* Receiver Overflow Clear      */
        int TUFLCL      :1; /* Transmitter Underflow Clear      */
        int ONEBYTE     :1; /* Special One Data Byte Transmission Mode */
    } B;
    int I;
    unsigned U;
} i2ccsr_type;
#define HW_I2CCSR (*(volatile i2ccsr_type _X*) (HW_I2C_BASEADDR+2)) /* I2C Control/Status Register (I2CCSR) */

#endif
