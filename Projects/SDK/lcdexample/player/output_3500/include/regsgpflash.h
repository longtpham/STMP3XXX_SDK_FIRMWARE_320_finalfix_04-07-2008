/////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
// Filename: regsgpflash.inc
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
#if !(defined(regsgpflashinc))
#define regsgpflashinc 1

#include "types.h"

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//   GPFLASH STMP Registers 
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
#define HW_GPFLASH_BASEADDR 0xF0C0


/////////////////////////////////////////////////////////////////////////////////
//  GPFLASH Control Status Register (HW_GPFLASH_CSR0R) Bit Definitions
#define HW_GPFLASH_CSR0_KICK_BITPOS (0)
#define HW_GPFLASH_CSR0_RW_BITPOS (1)
#define HW_GPFLASH_CSR0_INT_ENA_BITPOS (2)
#define HW_GPFLASH_CSR0_INT_STATUS_BITPOS (3)
#define HW_GPFLASH_CSR0_BUSY_INT_TYPE_BITPOS (4)
#define HW_GPFLASH_CSR0_BUSY_INT_ENA_BITPOS (8)
#define HW_GPFLASH_CSR0_BUSY_TIMEOUT_INT_ENA_BITPOS (9)
#define HW_GPFLASH_CSR0_BUSY_INT_STATUS_BITPOS (10)
#define HW_GPFLASH_CSR0_SOFT_RESET_BITPOS (11)
#define HW_GPFLASH_CSR0_CEB_CTRL_BITPOS (12)
#define HW_GPFLASH_CSR0_CLK_DISABLE_BITPOS (13)
#define HW_GPFLASH_CSR0_XFER_TYPE_BITPOS (16)
#define HW_GPFLASH_CSR0_EXT_BUSY_VAL_BITPOS (20)

#define HW_GPFLASH_CSR0_KICK_WIDTH (1)
#define HW_GPFLASH_CSR0_RW_WIDTH (1)
#define HW_GPFLASH_CSR0_INT_ENA_WIDTH (1)
#define HW_GPFLASH_CSR0_INT_STATUS_WIDTH (1)
#define HW_GPFLASH_CSR0_BUSY_INT_TYPE_WIDTH (4)
#define HW_GPFLASH_CSR0_BUSY_INT_ENA_WIDTH (1)
#define HW_GPFLASH_CSR0_BUSY_TIMEOUT_INT_ENA_WIDTH (1)
#define HW_GPFLASH_CSR0_BUSY_INT_STATUS_WIDTH (1)
#define HW_GPFLASH_CSR0_SOFT_RESET_WIDTH (1)
#define HW_GPFLASH_CSR0_CEB_CTRL_WIDTH (1)
#define HW_GPFLASH_CSR0_CLK_DISABLE_WIDTH (1)
#define HW_GPFLASH_CSR0_XFER_TYPE_WIDTH (4)
#define HW_GPFLASH_CSR0_EXT_BUSY_VAL_WIDTH (1)

#define HW_GPFLASH_CSR0_KICK_SETMASK (((1<<HW_GPFLASH_CSR0_KICK_WIDTH)-1)<<HW_GPFLASH_CSR0_KICK_BITPOS)
#define HW_GPFLASH_CSR0_RW_SETMASK (((1<<HW_GPFLASH_CSR0_RW_WIDTH)-1)<<HW_GPFLASH_CSR0_RW_BITPOS)
#define HW_GPFLASH_CSR0_INT_ENA_SETMASK (((1<<HW_GPFLASH_CSR0_INT_ENA_WIDTH)-1)<<HW_GPFLASH_CSR0_INT_ENA_BITPOS)
#define HW_GPFLASH_CSR0_INT_STATUS_SETMASK (((1<<HW_GPFLASH_CSR0_INT_STATUS_WIDTH)-1)<<HW_GPFLASH_CSR0_INT_STATUS_BITPOS)
#define HW_GPFLASH_CSR0_BUSY_INT_TYPE_SETMASK (((1<<HW_GPFLASH_CSR0_BUSY_INT_TYPE_WIDTH)-1)<<HW_GPFLASH_CSR0_BUSY_INT_TYPE_BITPOS)
#define HW_GPFLASH_CSR0_BUSY_INT_ENA_SETMASK (((1<<HW_GPFLASH_CSR0_BUSY_INT_ENA_WIDTH)-1)<<HW_GPFLASH_CSR0_BUSY_INT_ENA_BITPOS)
#define HW_GPFLASH_CSR0_BUSY_TIMEOUT_INT_ENA_SETMASK (((1<<HW_GPFLASH_CSR0_BUSY_TIMEOUT_INT_ENA_WIDTH)-1)<<HW_GPFLASH_CSR0_BUSY_TIMEOUT_INT_ENA_BITPOS)
#define HW_GPFLASH_CSR0_BUSY_INT_STATUS_SETMASK (((1<<HW_GPFLASH_CSR0_BUSY_INT_STATUS_WIDTH)-1)<<HW_GPFLASH_CSR0_BUSY_INT_STATUS_BITPOS)
#define HW_GPFLASH_CSR0_SOFT_RESET_SETMASK (((1<<HW_GPFLASH_CSR0_SOFT_RESET_WIDTH)-1)<<HW_GPFLASH_CSR0_SOFT_RESET_BITPOS)
#define HW_GPFLASH_CSR0_CEB_CTRL_SETMASK (((1<<HW_GPFLASH_CSR0_CEB_CTRL_WIDTH)-1)<<HW_GPFLASH_CSR0_CEB_CTRL_BITPOS)
#define HW_GPFLASH_CSR0_CLK_DISABLE_SETMASK (((1<<HW_GPFLASH_CSR0_CLK_DISABLE_WIDTH)-1)<<HW_GPFLASH_CSR0_CLK_DISABLE_BITPOS)
#define HW_GPFLASH_CSR0_XFER_TYPE_SETMASK (((1<<HW_GPFLASH_CSR0_XFER_TYPE_WIDTH)-1)<<HW_GPFLASH_CSR0_XFER_TYPE_BITPOS)
#define HW_GPFLASH_CSR0_EXT_BUSY_VAL_SETMASK (((1<<HW_GPFLASH_CSR0_EXT_BUSY_VAL_WIDTH)-1)<<HW_GPFLASH_CSR0_EXT_BUSY_VAL_BITPOS)

#define HW_GPFLASH_CSR0_KICK_CLRMASK (~(WORD)HW_GPFLASH_CSR0_KICK_SETMASK)
#define HW_GPFLASH_CSR0_RW_CLRMASK (~(WORD)HW_GPFLASH_CSR0_RW_SETMASK)
#define HW_GPFLASH_CSR0_INT_ENA_CLRMASK (~(WORD)HW_GPFLASH_CSR0_INT_ENA_SETMASK)
#define HW_GPFLASH_CSR0_INT_STATUS_CLRMASK (~(WORD)HW_GPFLASH_CSR0_INT_STATUS_SETMASK)
#define HW_GPFLASH_CSR0_BUSY_INT_TYPE_CLRMASK (~(WORD)HW_GPFLASH_CSR0_BUSY_INT_TYPE_SETMASK)
#define HW_GPFLASH_CSR0_BUSY_INT_ENA_CLRMASK (~(WORD)HW_GPFLASH_CSR0_BUSY_INT_ENA_SETMASK)
#define HW_GPFLASH_CSR0_BUSY_TIMEOUT_INT_ENA_CLRMASK (~(WORD)HW_GPFLASH_CSR0_BUSY_TIMEOUT_INT_ENA_SETMASK)
#define HW_GPFLASH_CSR0_BUSY_INT_STATUS_CLRMASK (~(WORD)HW_GPFLASH_CSR0_BUSY_INT_STATUS_SETMASK)
#define HW_GPFLASH_CSR0_SOFT_RESET_CLRMASK (~(WORD)HW_GPFLASH_CSR0_SOFT_RESET_SETMASK)
#define HW_GPFLASH_CSR0_CEB_CTRL_CLRMASK (~(WORD)HW_GPFLASH_CSR0_CEB_CTRL_SETMASK)
#define HW_GPFLASH_CSR0_CLK_DISABLE_CLRMASK (~(WORD)HW_GPFLASH_CSR0_CLK_DISABLE_SETMASK)
#define HW_GPFLASH_CSR0_XFER_TYPE_CLRMASK (~(WORD)HW_GPFLASH_CSR0_XFER_TYPE_SETMASK)
#define HW_GPFLASH_CSR0_EXT_BUSY_VAL_CLRMASK (~(WORD)HW_GPFLASH_CSR0_EXT_BUSY_VAL_SETMASK)


typedef union               
{
    struct {
        unsigned int KICK                 :1;
        unsigned int RW                   :1;
        unsigned int INT_XFERDONE_ENA     :1;
        unsigned int INT_XFERDONE_STATUS  :1;
        unsigned int BUSY_INT_TYPE        :2;
        unsigned int BUSY_TIMEOUT_EDGE    :2;
        unsigned int BUSY_INT_ENA         :1;
        unsigned int BUSY_TIMEOUT_INT_ENA :1;
	 unsigned int BUSY_INT_STATUS	  :1;
        unsigned int SOFT_RESET           :1;
        unsigned int CEB_CTRL             	:1;
        unsigned int CLK_DISABLE          	:1;
	 unsigned int BUSY_TIMEOUT_INT_STAT 	:1;
	 unsigned int 				:1;
        unsigned int XFER_TYPE            :2;
        unsigned int SPARE                :2;
	 unsigned int EXT_BUSY_VAL		:1;
	 unsigned int 				:3;
    } B;
    int I;
} gpflashcsr0_type;
#define HW_GPFLASH_CSR0R     (*(volatile gpflashcsr0_type _X*) (HW_GPFLASH_BASEADDR))    /* GPFLASH Control 0 / Status Register */

/////////////////////////////////////////////////////////////////////////////////
//  GPFLASH Control Status Register (HW_GPFLASH_CSR1R) Bit Definitions
#define HW_GPFLASH_CSR1_MEM_BITPOS (0)
#define HW_GPFLASH_CSR1_START_BYTE_BITPOS (4)
#define HW_GPFLASH_CSR1_CEB_BITPOS (8)
#define HW_GPFLASH_CSR1_WP0_BITPOS (12)
#define HW_GPFLASH_CSR1_WP1_BITPOS (13)
#define HW_GPFLASH_CSR1_MODE16_BITPOS (16)
#define HW_GPFLASH_CSR1_MUX_BITPOS (17)
#define HW_GPFLASH_CSR1_SECTOR_ERASE_CTL_BITPOS (18)
#define HW_GPFLASH_CSR1_WP_CTL_BITPOS (19)

#define HW_GPFLASH_CSR1_MEM_WIDTH (2)
#define HW_GPFLASH_CSR1_START_BYTE_WIDTH (2)
#define HW_GPFLASH_CSR1_CEB_WIDTH (2)
#define HW_GPFLASH_CSR1_WP0_WIDTH (1)
#define HW_GPFLASH_CSR1_WP1_WIDTH (1)
#define HW_GPFLASH_CSR1_MODE16_WIDTH (1)
#define HW_GPFLASH_CSR1_MUX_WIDTH (1)
#define HW_GPFLASH_CSR1_SECTOR_ERASE_CTL_WIDTH (1)
#define HW_GPFLASH_CSR1_WP_CTL_WIDTH (1)

#define HW_GPFLASH_CSR1_MEM_SETMASK (((1<<HW_GPFLASH_CSR1_MEM_WIDTH)-1)<<HW_GPFLASH_CSR1_MEM_BITPOS)
#define HW_GPFLASH_CSR1_START_BYTE_SETMASK (((1<<HW_GPFLASH_CSR1_START_BYTE_WIDTH)-1)<<HW_GPFLASH_CSR1_START_BYTE_BITPOS)
#define HW_GPFLASH_CSR1_CEB_SETMASK (((1<<HW_GPFLASH_CSR1_CEB_WIDTH)-1)<<HW_GPFLASH_CSR1_CEB_BITPOS)
#define HW_GPFLASH_CSR1_WP0_SETMASK (((1<<HW_GPFLASH_CSR1_WP0_WIDTH)-1)<<HW_GPFLASH_CSR1_WP0_BITPOS)
#define HW_GPFLASH_CSR1_WP1_SETMASK (((1<<HW_GPFLASH_CSR1_WP1_WIDTH)-1)<<HW_GPFLASH_CSR1_WP1_BITPOS)
#define HW_GPFLASH_CSR1_MODE16_SETMASK (((1<<HW_GPFLASH_CSR1_MODE16_WIDTH)-1)<<HW_GPFLASH_CSR1_MODE16_BITPOS)
#define HW_GPFLASH_CSR1_MUX_SETMASK (((1<<HW_GPFLASH_CSR1_MUX_WIDTH)-1)<<HW_GPFLASH_CSR1_MUX_BITPOS)
#define HW_GPFLASH_CSR1_SECTOR_ERASE_CTL_SETMASK (((1<<HW_GPFLASH_CSR1_SECTOR_ERASE_CTL_WIDTH)-1)<<HW_GPFLASH_CSR1_SECTOR_ERASE_CTL_BITPOS)
#define HW_GPFLASH_CSR1_WP_CTL_SETMASK (((1<<HW_GPFLASH_CSR1_WP_CTL_WIDTH)-1)<<HW_GPFLASH_CSR1_WP_CTL_BITPOS)

#define HW_GPFLASH_CSR1_MEM_CLRMASK (~(WORD)HW_GPFLASH_CSR1_MEM_SETMASK)
#define HW_GPFLASH_CSR1_START_BYTE_CLRMASK (~(WORD)HW_GPFLASH_CSR1_START_BYTE_SETMASK)
#define HW_GPFLASH_CSR1_CEB_CLRMASK (~(WORD)HW_GPFLASH_CSR1_CEB_SETMASK)
#define HW_GPFLASH_CSR1_WP0_CLRMASK (~(WORD)HW_GPFLASH_CSR1_WP0_SETMASK)
#define HW_GPFLASH_CSR1_WP1_CLRMASK (~(WORD)HW_GPFLASH_CSR1_WP1_SETMASK)
#define HW_GPFLASH_CSR1_MODE16_CLRMASK (~(WORD)HW_GPFLASH_CSR1_MODE16_SETMASK)
#define HW_GPFLASH_CSR1_MUX_CLRMASK (~(WORD)HW_GPFLASH_CSR1_MUX_SETMASK)
#define HW_GPFLASH_CSR1_SECTOR_ERASE_CTL_CLRMASK (~(WORD)HW_GPFLASH_CSR1_SECTOR_ERASE_CTL_SETMASK)
#define HW_GPFLASH_CSR1_WP_CTL_CLRMASK (~(WORD)HW_GPFLASH_CSR1_WP_CTL_SETMASK)


typedef union               
{
    struct {
        int MEM                  :2;
        int RSVD0                :2;
        int START_BYTE           :2;
        int RSVD1                :2;
        int CEB                  :2;
        int RSVD2                :2;
        int WP0                  :1;
        int WP1                  :1;
        int RSVD3                :2;
        int MODE16               :1;
        int MUX                  :1;
        int SECTOR_ERASE_CTL     :1;
        int WP_CTL               :1;
        int RSVD4                :4;
    } B;
    int I;
} gpflashcsr1_type;
#define HW_GPFLASH_CSR1R     (*(volatile gpflashcsr1_type _X*) (HW_GPFLASH_BASEADDR+1))    /* GPFLASH Control 1 / Status Register */

/////////////////////////////////////////////////////////////////////////////////
// GPFLASH DMA ADDRESS register (HW_GPFLASH_DMA_ADDR) Bit Definitions
#define HW_GPFLASH_DMA_ADD_SETMASK (0x00FFFF)

#define HW_GPFLASH_DMA_ADD_CLRMASK (~(WORD)HW_GPFLASH_DMA_ADD_SETMASK)

typedef union               
{
    struct {
        int ADD            :16;
        int RSVD           :8;
    } B;
    int I;
} gpflashdmaadd_type;
#define HW_GPFLASH_DMA_ADDR      (*(volatile gpflashdmaadd_type _X*) (HW_GPFLASH_BASEADDR+2))    

/////////////////////////////////////////////////////////////////////////////////
// GPFLASH XFER SIZE register (HW_GPFLASH_XFER_SIZE) Bit Definitions
#define HW_GPFLASH_XFER_SIZER_SETMASK (0x001FFF)

#define HW_GPFLASH_XFER_SIZER_CLRMASK (~(WORD)HW_GPFLASH_XFER_SIZER_SETMASK)

typedef union               
{
    struct {
        unsigned NUM_BYTES          :13;
        int RSVD          :11;
    } B;
    int I;
} gpflashxfersizer_type;
#define HW_GPFLASH_XFER_SIZER      (*(volatile gpflashxfersizer_type _X*) (HW_GPFLASH_BASEADDR+3))    

/////////////////////////////////////////////////////////////////////////////////
// GPFLASH TIMING 1 register (HW_GPFLASH_TIMING1) Bit Definitions
#define HW_GPFLASH_TSU_BITPOS (0)
#define HW_GPFLASH_THOLD_BITPOS (8)

#define HW_GPFLASH_TSU_WIDTH (5)
#define HW_GPFLASH_THOLD_WIDTH (5)

#define HW_GPFLASH_TSU_SETMASK (((1<<HW_GPFLASH_TSU_WIDTH)-1)<<HW_GPFLASH_TSU_BITPOS)
#define HW_GPFLASH_THOLD_SETMASK (((1<<HW_GPFLASH_THOLD_WIDTH)-1)<<HW_GPFLASH_THOLD_BITPOS)

#define HW_GPFLASH_TSU_CLRMASK (~(WORD)HW_GPFLASH_TSU_SETMASK)
#define HW_GPFLASH_THOLD_CLRMASK (~(WORD)HW_GPFLASH_THOLD_SETMASK)

typedef union               
{
    struct {
        int TSU          :5;
        int RSVD0          :3;       
        int THOLD           :5;
        int RSVD1          :11;
    } B;
    int I;
} gpflashtiming1_type;
#define HW_GPFLASH_TIMING1      (*(volatile gpflashtiming1_type _X*) (HW_GPFLASH_BASEADDR+4))    

/////////////////////////////////////////////////////////////////////////////////
// GPFLASH TIMING 2 register (HW_GPFLASH_TIMING2) Bit Definitions
#define HW_GPFLASH_TDS_BITPOS (0)
#define HW_GPFLASH_TDH_BITPOS (8)

#define HW_GPFLASH_TDS_WIDTH (6)
#define HW_GPFLASH_TDH_WIDTH (6)

#define HW_GPFLASH_TDS_SETMASK (((1<<HW_GPFLASH_TDS_WIDTH)-1)<<HW_GPFLASH_TDS_BITPOS)
#define HW_GPFLASH_TDH_SETMASK (((1<<HW_GPFLASH_TDH_WIDTH)-1)<<HW_GPFLASH_TDH_BITPOS)

#define HW_GPFLASH_TDS_CLRMASK (~(WORD)HW_GPFLASH_TDS_SETMASK)
#define HW_GPFLASH_TDH_CLRMASK (~(WORD)HW_GPFLASH_TDH_SETMASK)

typedef union               
{
    struct {
        int TDS           :6;
        int RSVD0          :2;       
        int TDH           :6;
        int RSVD1          :10;
    } B;
    int I;
} gpflashtiming2_type;
#define HW_GPFLASH_TIMING2      (*(volatile gpflashtiming2_type _X*) (HW_GPFLASH_BASEADDR+5))    

/////////////////////////////////////////////////////////////////////////////////
// GPFLASH TIMING 2 register (HW_GPFLASH_TIMINGBUSY) Bit Definitions
#define HW_GPFLASH_TWTO_SETMASK (0x01FFFF)

#define HW_GPFLASH_TWTO_CLRMASK (~(WORD)HW_GPFLASH_TWTO_SETMASK)


typedef union               
{
    struct {
        int TWTO           :18;
        int RSVD           :6;       
    } B;
    int I;
} gpflashtimingbusy_type;
#define HW_GPFLASH_TIMINGBUSY      (*(volatile gpflashtimingbusy _X*) (HW_GPFLASH_BASEADDR+6))    
#endif
