//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;  Copyright(C) SigmaTel, Inc. 2002-2003
//;  File        : regssdram.inc
//;  Description : Mixed Signal IP Register definition
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// The following naming conventions are followed in this file.
// All registers are named using the format...
//     HW_<module>_<regname>
// where <module> is the module name which can be any of the following...
//     SYSTEM
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

#if !(defined(regssdraminc))
#define regssdraminc 1

#include "types.h"

#define HW_SDRAM_BASEADDR 0xF900


/////////////////////////////////////////////////////////////////////////////////
//  SDRAM CSR (HW_SDRAM_CSR) Bit Definitions
#define HW_SDRAM_CSR_SDRAMEN_BITPOS 0
#define HW_SDRAM_CSR_IE_BITPOS 1
#define HW_SDRAM_CSR_RNW_BITPOS 2
#define HW_SDRAM_CSR_KICK_BITPOS 3
#define HW_SDRAM_CSR_LM_BITPOS 4
#define HW_SDRAM_CSR_ISTAT_BITPOS 5
#define HW_SDRAM_CSR_PWDN_BITPOS 6
#define HW_SDRAM_CSR_SBYTE_BITPOS 8
#define HW_SDRAM_CSR_MEM_BITPOS 10
#define HW_SDRAM_CSR_BIGE_BITPOS 12
#define HW_SDRAM_CSR_ASIZE_BITPOS 13
#define HW_SDRAM_CSR_UKICK_BITPOS 16
#define HW_SDRAM_CSR_DIV_BITPOS 17
#define HW_SDRAM_CSR_MULTI_BITPOS 21
#define HW_SDRAM_CSR_SDRAM_BITPOS 22
#define HW_SDRAM_CSR_SIGN_BITPOS 23

#define HW_SDRAM_CSR_SDRAMEN_WIDTH 1
#define HW_SDRAM_CSR_IE_WIDTH 1
#define HW_SDRAM_CSR_RNW_WIDTH 1
#define HW_SDRAM_CSR_KICK_WIDTH 1
#define HW_SDRAM_CSR_LM_WIDTH 1
#define HW_SDRAM_CSR_ISTAT_WIDTH 1
#define HW_SDRAM_CSR_PWDN_WIDTH 1
#define HW_SDRAM_CSR_SBYTE_WIDTH 2
#define HW_SDRAM_CSR_MEM_WIDTH 2
#define HW_SDRAM_CSR_BIGE_WIDTH 1
#define HW_SDRAM_CSR_ASIZE_WIDTH 3
#define HW_SDRAM_CSR_UKICK_WIDTH 1
#define HW_SDRAM_CSR_DIV_WIDTH 4
#define HW_SDRAM_CSR_MULTI_WIDTH 1
#define HW_SDRAM_CSR_SDRAM_WIDTH 1
#define HW_SDRAM_CSR_SIGN_WIDTH 1

#define HW_SDRAM_CSR_SDRAMEN_SETMASK (((1<<HW_SDRAM_CSR_SDRAMEN_WIDTH)-1)<<HW_SDRAM_CSR_SDRAMEN_BITPOS)
#define HW_SDRAM_CSR_IE_SETMASK (((1<<HW_SDRAM_CSR_IE_WIDTH)-1)<<HW_SDRAM_CSR_IE_BITPOS)
#define HW_SDRAM_CSR_RNW_SETMASK (((1<<HW_SDRAM_CSR_RNW_WIDTH)-1)<<HW_SDRAM_CSR_RNW_BITPOS)
#define HW_SDRAM_CSR_KICK_SETMASK (((1<<HW_SDRAM_CSR_KICK_WIDTH)-1)<<HW_SDRAM_CSR_KICK_BITPOS)
#define HW_SDRAM_CSR_LM_SETMASK (((1<<HW_SDRAM_CSR_LM_WIDTH)-1)<<HW_SDRAM_CSR_LM_BITPOS)
#define HW_SDRAM_CSR_ISTAT_SETMASK (((1<<HW_SDRAM_CSR_ISTAT_WIDTH)-1)<<HW_SDRAM_CSR_ISTAT_BITPOS)
#define HW_SDRAM_CSR_PWDN_SETMASK (((1<<HW_SDRAM_CSR_PWDN_WIDTH)-1)<<HW_SDRAM_CSR_PWDN_BITPOS)
#define HW_SDRAM_CSR_SBYTE_SETMASK (((1<<HW_SDRAM_CSR_SBYTE_WIDTH)-1)<<HW_SDRAM_CSR_SBYTE_BITPOS)
#define HW_SDRAM_CSR_MEM_SETMASK (((1<<HW_SDRAM_CSR_MEM_WIDTH)-1)<<HW_SDRAM_CSR_MEM_BITPOS)
#define HW_SDRAM_CSR_BIGE_SETMASK (((1<<HW_SDRAM_CSR_BIGE_WIDTH)-1)<<HW_SDRAM_CSR_BIGE_BITPOS)
#define HW_SDRAM_CSR_ASIZE_SETMASK (((1<<HW_SDRAM_CSR_ASIZE_WIDTH)-1)<<HW_SDRAM_CSR_ASIZE_BITPOS)
#define HW_SDRAM_CSR_UKICK_SETMASK (((1<<HW_SDRAM_CSR_UKICK_WIDTH)-1)<<HW_SDRAM_CSR_UKICK_BITPOS)
#define HW_SDRAM_CSR_DIV_SETMASK (((1<<HW_SDRAM_CSR_DIV_WIDTH)-1)<<HW_SDRAM_CSR_DIV_BITPOS)
#define HW_SDRAM_CSR_MULTI_SETMASK (((1<<HW_SDRAM_CSR_MULTI_WIDTH)-1)<<HW_SDRAM_CSR_MULTI_BITPOS)
#define HW_SDRAM_CSR_SDRAM_SETMASK (((1<<HW_SDRAM_CSR_SDRAM_WIDTH)-1)<<HW_SDRAM_CSR_SDRAM_BITPOS)
#define HW_SDRAM_CSR_SIGN_SETMASK (((1<<HW_SDRAM_CSR_SIGN_WIDTH)-1)<<HW_SDRAM_CSR_SIGN_BITPOS)

#define HW_SDRAM_CSR_SDRAMEN_CLRMASK ~(WORD)HW_SDRAM_CSR_SDRAMEN_SETMASK
#define HW_SDRAM_CSR_IE_CLRMASK ~(WORD)HW_SDRAM_CSR_IE_SETMASK
#define HW_SDRAM_CSR_RNW_CLRMASK ~(WORD)HW_SDRAM_CSR_RNW_SETMASK
#define HW_SDRAM_CSR_KICK_CLRMASK ~(WORD)HW_SDRAM_CSR_KICK_SETMASK
#define HW_SDRAM_CSR_LM_CLRMASK ~(WORD)HW_SDRAM_CSR_LM_SETMASK
#define HW_SDRAM_CSR_ISTAT_CLRMASK ~(WORD)HW_SDRAM_CSR_ISTAT_SETMASK
#define HW_SDRAM_CSR_PWDN_CLRMASK ~(WORD)HW_SDRAM_CSR_PWDN_SETMASK
#define HW_SDRAM_CSR_SBYTE_CLRMASK ~(WORD)HW_SDRAM_CSR_SBYTE_SETMASK
#define HW_SDRAM_CSR_MEM_CLRMASK ~(WORD)HW_SDRAM_CSR_MEM_SETMASK
#define HW_SDRAM_CSR_BIGE_CLRMASK ~(WORD)HW_SDRAM_CSR_BIGE_SETMASK
#define HW_SDRAM_CSR_ASIZE_CLRMASK ~(WORD)HW_SDRAM_CSR_ASIZE_SETMASK
#define HW_SDRAM_CSR_UKICK_CLRMASK ~(WORD)HW_SDRAM_CSR_UKICK_SETMASK
#define HW_SDRAM_CSR_DIV_CLRMASK ~(WORD)HW_SDRAM_CSR_DIV_SETMASK
#define HW_SDRAM_CSR_MULTI_CLRMASK ~(WORD)HW_SDRAM_CSR_MULTI_SETMASK
#define HW_SDRAM_CSR_SDRAM_CLRMASK ~(WORD)HW_SDRAM_CSR_SDRAM_SETMASK
#define HW_SDRAM_CSR_SIGN_CLRMASK ~(WORD)HW_SDRAM_CSR_SIGN_SETMASK

typedef union               
{
    struct {
        int SDRAMEN                     :1;
        int IE                          :1;
        int RNW                         :1;
        int KICK                        :1;
        int LM                          :1;
        int ISTAT                       :1;
        int PWDN                        :1;
        int RSVD                        :1;
        int SBYTE                       :2;
        int MEM                         :2;
        int BIGE                        :1;
        int ASIZE                       :3;
        int UKICK                       :1;
        int DIV                         :4;
        int MULTI                       :1;
        int SDRAM                       :1;
        int SIGN                        :1;
    } B;
    int I;
} sdramcsr_type;
#define HW_SDRAM_CSR (*(volatile sdramcsr_type _X*) (HW_SDRAM_BASEADDR))        
#define HW_SDRAM_ADDR1 (*(volatile unsigned int _X*) (HW_SDRAM_BASEADDR+1))
#define HW_SDRAM_ADDR2 (*(volatile unsigned int _X*) (HW_SDRAM_BASEADDR+2))
#define HW_SDRAM_SYSADDR (*(volatile unsigned int _X*) (HW_SDRAM_BASEADDR+3))
#define HW_SDRAM_SIZE (*(volatile unsigned int _X*) (HW_SDRAM_BASEADDR+4))
#define HW_SDRAM_BAR (*(volatile unsigned int _X*) (HW_SDRAM_BASEADDR+7))
#define HW_SDRAM_MR (*(volatile unsigned int _X*) (HW_SDRAM_BASEADDR+8))
#define HW_SDRAM_DBAR1 (*(volatile unsigned int _X*) (HW_SDRAM_BASEADDR+9))
#define HW_SDRAM_DBAR2 (*(volatile unsigned int _X*) (HW_SDRAM_BASEADDR+10))
#define HW_SDRAM_DMR1 (*(volatile unsigned int _X*) (HW_SDRAM_BASEADDR+11))
#define HW_SDRAM_DMR2 (*(volatile unsigned int _X*) (HW_SDRAM_BASEADDR+12))

/////////////////////////////////////////////////////////////////////////////////
//  SDRAM Start Address Low Register (HW_SDRAM_ADDR1) Bit Definitions
#define HW_SDRAM_ADDR1_XA_BITPOS 0

#define HW_SDRAM_ADDR1_XA_SETMASK 0xFFFFFF<<HW_SDRAM_ADDR1_XA_BITPOS

#define HW_SDRAM_ADDR1_XA_CLRMASK ~(WORD)HW_SDRAM_ADDR1_XA_SETMASK

/////////////////////////////////////////////////////////////////////////////////
//  SDRAM Start Address High Register (HW_SDRAM_ADDR2) Bit Definitions
#define HW_SDRAM_ADDR2_XA_BITPOS 0

#define HW_SDRAM_ADDR2_XA_SETMASK 0x1F<<HW_SDRAM_ADDR2_XA_BITPOS

#define HW_SDRAM_ADDR2_XA_CLRMASK ~(WORD)HW_SDRAM_ADDR2_XA_SETMASK

/////////////////////////////////////////////////////////////////////////////////
//  System Start Address Register (HW_SDRAM_SYSADDR) Bit Definitions
#define HW_SDRAM_SYSADDR_XA_BITPOS 0

#define HW_SDRAM_SYSADDR_XA_SETMASK 0xFFFF<<HW_SDRAM_SYSADDR_XA_BITPOS

#define HW_SDRAM_SYSADDR_XA_CLRMASK ~(WORD)HW_SDRAM_SYSADDR_XA_SETMASK

/////////////////////////////////////////////////////////////////////////////////
//  Number of Bytes to be transfered Register (HW_SDRAM_SIZE) Bit Definitions
#define HW_SDRAM_SIZE_XA_BITPOS 0

#define HW_SDRAM_SIZE_XA_SETMASK 0x3FFFF<<HW_SDRAM_SIZE_XA_BITPOS

#define HW_SDRAM_SIZE_XA_CLRMASK ~(WORD)HW_SDRAM_SIZE_XA_SETMASK

/////////////////////////////////////////////////////////////////////////////////
//  SDRAM Timer1 Register (HW_SDRAM_TIMER1) Bit Definitions
#define HW_SDRAM_TIMER1_INIT_BITPOS 0
#define HW_SDRAM_TIMER1_TRP_BITPOS 16
#define HW_SDRAM_TIMER1_TRFC_BITPOS 20

#define HW_SDRAM_TIMER1_INIT_WIDTH 16
#define HW_SDRAM_TIMER1_TRP_WIDTH 4
#define HW_SDRAM_TIMER1_TRFC_WIDTH 4

#define HW_SDRAM_TIMER1_INIT_SETMASK (((1<<HW_SDRAM_TIMER1_INIT_WIDTH)-1)<<HW_SDRAM_TIMER1_INIT_BITPOS)
#define HW_SDRAM_TIMER1_TRP_SETMASK (((1<<HW_SDRAM_TIMER1_TRP_WIDTH)-1)<<HW_SDRAM_TIMER1_TRP_BITPOS)
#define HW_SDRAM_TIMER1_TRFC_SETMASK (((1<<HW_SDRAM_TIMER1_TRFC_WIDTH)-1)<<HW_SDRAM_TIMER1_TRFC_BITPOS)

#define HW_SDRAM_TIMER1_INIT_CLRMASK ~(WORD)HW_SDRAM_TIMER1_INIT_SETMASK
#define HW_SDRAM_TIMER1_TRP_CLRMASK ~(WORD)HW_SDRAM_TIMER1_TRP_SETMASK
#define HW_SDRAM_TIMER1_TRFC_CLRMASK ~(WORD)HW_SDRAM_TIMER1_TRFC_SETMASK

typedef union               
{
    struct {
        int INIT                :16;
        int TRP                 :4;
        int TRFC                :4;
    } B;
    int I;
} sdramtimer1_type;
#define HW_SDRAM_TIMER1 (*(volatile sdramtimer1_type _X*) (HW_SDRAM_BASEADDR+5))

/////////////////////////////////////////////////////////////////////////////////
//  SDRAM Timer2 Register (HW_SDRAM_TIMER2) Bit Definitions
#define HW_SDRAM_TIMER2_TXSR_BITPOS 0
#define HW_SDRAM_TIMER2_TREF_BITPOS 4
#define HW_SDRAM_TIMER2_TRCD_BITPOS 16

#define HW_SDRAM_TIMER2_TXSR_WIDTH 4
#define HW_SDRAM_TIMER2_TREF_WIDTH 12
#define HW_SDRAM_TIMER2_TRCD_WIDTH 4

#define HW_SDRAM_TIMER2_TXSR_SETMASK (((1<<HW_SDRAM_TIMER2_TXSR_WIDTH)-1)<<HW_SDRAM_TIMER2_TXSR_BITPOS)
#define HW_SDRAM_TIMER2_TREF_SETMASK (((1<<HW_SDRAM_TIMER2_TREF_WIDTH)-1)<<HW_SDRAM_TIMER2_TREF_BITPOS)
#define HW_SDRAM_TIMER2_TRCD_SETMASK (((1<<HW_SDRAM_TIMER2_TRCD_WIDTH)-1)<<HW_SDRAM_TIMER2_TRCD_BITPOS)

#define HW_SDRAM_TIMER2_TXSR_CLRMASK ~(WORD)HW_SDRAM_TIMER2_TXSR_SETMASK
#define HW_SDRAM_TIMER2_TREF_CLRMASK ~(WORD)HW_SDRAM_TIMER2_TREF_SETMASK
#define HW_SDRAM_TIMER2_TRCD_CLRMASK ~(WORD)HW_SDRAM_TIMER2_TRCD_SETMASK

typedef union               
{
    struct {
        int TXSR                :4;
        int TREF                :12;
        int TRCD                :4;
        int RSVD                :4; 
    } B;
    int I;
} sdramtimer2_type;
#define HW_SDRAM_TIMER2 (*(volatile sdramtimer2_type _X*) (HW_SDRAM_BASEADDR+6))

/////////////////////////////////////////////////////////////////////////////////
//  System Modulo Base Address Register (HW_SDRAM_BAR) Bit Definitions
#define HW_SDRAM_BAR_XA_BITPOS 0

#define HW_SDRAM_BAR_XA_SETMASK 0xFFFF<<HW_SDRAM_BAR_XA_BITPOS

#define HW_SDRAM_BAR_XA_CLRMASK ~(WORD)HW_SDRAM_BAR_XA_SETMASK

/////////////////////////////////////////////////////////////////////////////////
//  System Modulo Register (HW_SDRAM_MR) Bit Definitions
#define HW_SDRAM_MR_XA_BITPOS 0

#define HW_SDRAM_MR_XA_SETMASK 0xFFFF<<HW_SDRAM_MR_XA_BITPOS

#define HW_SDRAM_MR_XA_CLRMASK ~(WORD)HW_SDRAM_MR_XA_SETMASK

/////////////////////////////////////////////////////////////////////////////////
//  SDRAM Mode Register (HW_SDRAM_MODE) Bit Definitions
#define HW_SDRAM_MODE_XA_BITPOS 0

#define HW_SDRAM_MODE_XA_WIDTH 14

#define HW_SDRAM_MODE_XA_SETMASK (((1<<HW_SDRAM_MODE_XA_WIDTH)-1)<<HW_SDRAM_MODE_XA_BITPOS)

#define HW_SDRAM_MODE_XA_CLRMASK ~(WORD)HW_SDRAM_MODE_XA_SETMASK

typedef union               
{
    struct {
        int VALUE               :14;
        int RSVD                :10; 
    } B;
    int I;
} sdrammode_type;
#define HW_SDRAM_MODE (*(volatile sdrammode_type _X*) (HW_SDRAM_BASEADDR+14))

/////////////////////////////////////////////////////////////////////////////////
//  SDRAM Type Register (HW_SDRAM_TYPE) Bit Definitions
#define HW_SDRAM_TYPE_COLWIDTH_BITPOS 0
#define HW_SDRAM_TYPE_ROWWIDTH_BITPOS 4

#define HW_SDRAM_TYPE_COLWIDTH_WIDTH 4
#define HW_SDRAM_TYPE_ROWWIDTH_WIDTH 4

#define HW_SDRAM_TYPE_COLWIDTH_SETMASK (((1<<HW_SDRAM_TYPE_COLWIDTH_WIDTH)-1)<<HW_SDRAM_TYPE_COLWIDTH_BITPOS)
#define HW_SDRAM_TYPE_ROWWIDTH_SETMASK (((1<<HW_SDRAM_TYPE_ROWWIDTH_WIDTH)-1)<<HW_SDRAM_TYPE_ROWWIDTH_BITPOS)

#define HW_SDRAM_TYPE_COLWIDTH_CLRMASK (~(WORD)HW_SDRAM_TYPE_COLWIDTH_SETMASK)
#define HW_SDRAM_TYPE_ROWWIDTH_CLRMASK (~(WORD)HW_SDRAM_TYPE_ROWWIDTH_SETMASK)

typedef union               
{
    struct {
        int COLWIDTH               :4;
        int ROWWIDTH               :4; 
    } B;
    int I;
} sdramtype_type;
#define HW_SDRAM_TYPE (*(volatile sdramtype_type _X*) (HW_SDRAM_BASEADDR+14))

#endif






