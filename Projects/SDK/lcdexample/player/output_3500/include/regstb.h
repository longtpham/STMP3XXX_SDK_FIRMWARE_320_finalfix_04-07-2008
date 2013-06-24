/////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2003
// Filename: regstb.inc
// Description: Register definitions for Trace Buffer
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
#if !(defined(regstbinc))
#define regstbinc 1

#include "types.h"

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

//   Trace Buffer STMP Registers 
//   Last Updated 6.30.2003 D. Baker
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#define HW_TB_BASEADDR (0xF080)




/////////////////////////////////////////////////////////////////////////////////

//  Trace Buffer Configuration Register (HW_TB_CFG) Bit Definitions

#define HW_TB_CFG_CLK_ENABLE_BITPOS (0)
#define HW_TB_CFG_ENABLE_BITPOS (1)
#define HW_TB_CFG_DONE_BITPOS (3)
#define HW_TB_CFG_DMA_ASEL_BITPOS (4)
#define HW_TB_CFG_TRIG_EVENT_BITPOS (6)

#define HW_TB_CFG_CLK_ENABLE_WIDTH (1)
#define HW_TB_CFG_ENABLE_WIDTH (1)
#define HW_TB_CFG_RSVD1_WIDTH (1)
#define HW_TB_CFG_DONE_WIDTH (1)
#define HW_TB_CFG_DMA_ASEL_WIDTH (2)
#define HW_TB_CFG_TRIG_EVENT_WIDTH (1)
#define HW_TB_CFG_RSVD2_WIDTH (17)

#define HW_TB_CFG_CLK_ENABLE_SETMASK (((1<<HW_TB_CFG_CLK_ENABLE_WIDTH)-1)<<HW_TB_CFG_CLK_ENABLE_BITPOS) 
#define HW_TB_CFG_ENABLE_SETMASK (((1<<HW_TB_CFG_ENABLE_WIDTH)-1)<<HW_TB_CFG_ENABLE_BITPOS) 
#define HW_TB_CFG_DONE_SETMASK (((1<<HW_TB_CFG_DONE_WIDTH)-1)<<HW_TB_CFG_DONE_BITPOS) 
#define HW_TB_CFG_DMA_ASEL_SETMASK (((1<<HW_TB_CFG_DMA_ASEL_WIDTH)-1)<<HW_TB_CFG_DMA_ASEL_BITPOS) 
#define HW_TB_CFG_TRIG_EVENT_SETMASK (((1<<HW_TB_CFG_TRIG_EVENT_WIDTH)-1)<<HW_TB_CFG_TRIG_EVENT_BITPOS) 

#define HW_TB_CFG_CLK_ENABLE_CLRMASK (~(WORD)HW_TB_CFG_CLK_ENABLE_SETMASK)
#define HW_TB_CFG_ENABLE_CLRMASK (~(WORD)HW_TB_CFG_ENABLE_SETMASK)
#define HW_TB_CFG_DONE_CLRMASK (~(WORD)HW_TB_CFG_DONE_SETMASK)
#define HW_TB_CFG_DMA_ASEL_CLRMASK (~(WORD)HW_TB_CFG_DMA_ASEL_SETMASK)
#define HW_TB_CFG_TRIG_EVENT_CLRMASK (~(WORD)HW_TB_CFG_TRIG_EVENT_SETMASK)

typedef union               
{
    struct {
	 int CLK_ENABLE      : HW_TB_CFG_CLK_ENABLE_WIDTH;
	 int ENABLE 	     : HW_TB_CFG_ENABLE_WIDTH;
        int rsvd1           : HW_TB_CFG_RSVD1_WIDTH;
	 int DONE 	     : HW_TB_CFG_DONE_WIDTH;
	 int DMA_ASEL 	     : HW_TB_CFG_DMA_ASEL_WIDTH;
	 int TRIG_EVENT      : HW_TB_CFG_TRIG_EVENT_WIDTH;
        int rsvd2           : HW_TB_CFG_RSVD2_WIDTH;
    } B;
    int I;
    unsigned int U;
} tb_cfg_type;
#define HW_TB_CFG      (*(volatile tb_cfg_type _X*) (HW_TB_BASEADDR+0))    /* Trace Buffer Configuration Register */

/////////////////////////////////////////////////////////////////////////////////

//  Trace Buffer Base Address Register (HW_TB_BAR) Bit Definitions

#define HW_TB_BAR_ADDRESS_BITPOS (0)

#define HW_TB_BAR_ADDRESS_WIDTH (16)        
#define HW_TB_BAR_RSVD_WIDTH (8)

#define HW_TB_BAR_ADDRESS_SETMASK (((1<<HW_TB_BAR_ADDRESS_WIDTH)-1)<<HW_TB_BAR_ADDRESS_BITPOS) 

#define HW_TB_BAR_ADDRESS_CLRMASK (~(WORD)HW_TB_BAR_ADDRESS_SETMASK)

typedef union               
{
    struct {
	 int ADDRESS      : HW_TB_BAR_ADDRESS_WIDTH;
        int reserved     : HW_TB_BAR_RSVD_WIDTH;
    } B;
    int I;
    unsigned int U;
} tb_bar_type;
#define HW_TB_BAR      (*(volatile tb_bar_type _X*) (HW_TB_BASEADDR+1))    /* Trace Buffer Base Address Register */

/////////////////////////////////////////////////////////////////////////////////

//  Trace Buffer Modulo Register (HW_TB_MOD) Bit Definitions

#define HW_TB_MOD_MODULUS_BITPOS (0)

#define HW_TB_MOD_MODULUS_WIDTH (14)        
#define HW_TB_MOD_RSVD_WIDTH (10)

#define HW_TB_MOD_MODULUS_SETMASK (((1<<HW_TB_MOD_MODULUS_WIDTH)-1)<<HW_TB_MOD_MODULUS_BITPOS) 

#define HW_TB_MOD_MODULUS_CLRMASK (~(WORD)HW_TB_MOD_MODULUS_SETMASK)

typedef union               
{
    struct {
	 int MODULUS      : HW_TB_MOD_MODULUS_WIDTH;
        int reserved        : HW_TB_MOD_RSVD_WIDTH;
    } B;
    int I;
    unsigned int U;
} tb_mod_type;
#define HW_TB_MOD      (*(volatile tb_mod_type _X*) (HW_TB_BASEADDR+2))    /* Trace Buffer Modulus Register */

/////////////////////////////////////////////////////////////////////////////////

//  Trace Buffer Current Index Register (HW_TB_CIR) Bit Definitions

#define HW_TB_CIR_INDEX_BITPOS (0)

#define HW_TB_CIR_INDEX_WIDTH (14)        
#define HW_TB_CIR_RSVD_WIDTH (10)

#define HW_TB_CIR_INDEX_SETMASK (((1<<HW_TB_CIR_INDEX_WIDTH)-1)<<HW_TB_CIR_INDEX_BITPOS) 

#define HW_TB_CIR_INDEX_CLRMASK (~(WORD)HW_TB_CIR_INDEX_SETMASK)

typedef union               
{
    struct {
	 int INDEX        : HW_TB_CIR_INDEX_WIDTH;
        int reserved     : HW_TB_CIR_RSVD_WIDTH;
    } B;
    int I;
    unsigned int U;
} tb_cir_type;
#define HW_TB_CIR      (*(volatile tb_cir_type _X*) (HW_TB_BASEADDR+3))    /* Trace Buffer Current Index Register */

/////////////////////////////////////////////////////////////////////////////////

//  Trace Buffer One Byte Code Register (HW_TB_OBC) Bit Definitions

#define HW_TB_OBC_CODE_BITPOS (0)

#define HW_TB_OBC_CODE_WIDTH (8)        
#define HW_TB_OBC_RSVD_WIDTH (16)

#define HW_TB_OBC_CODE_SETMASK (((1<<HW_TB_OBC_CODE_WIDTH)-1)<<HW_TB_OBC_CODE_BITPOS) 

#define HW_TB_OBC_CODE_CLRMASK (~(WORD)HW_TB_OBC_CODE_SETMASK)

typedef union               
{
    struct {
	 int CODE        : HW_TB_OBC_CODE_WIDTH;
        int reserved    : HW_TB_OBC_RSVD_WIDTH;
    } B;
    int I;
    unsigned int U;
} tb_obc_type;
#define HW_TB_OBC      (*(volatile tb_obc_type _X*) (HW_TB_BASEADDR+4))    /* Trace Buffer one byte code Register */

/////////////////////////////////////////////////////////////////////////////////

//  Trace Buffer Trigger Command Register (HW_TB_TCS) Bit Definitions

#define HW_TB_TCS_TRG_STYLE_BITPOS (0)
#define HW_TB_TCS_CAP_CLASS_BITPOS (1)
#define HW_TB_TCS_TRG_CLASS_BITPOS (3)
#define HW_TB_TCS_FREEZE_BITPOS (5)

#define HW_TB_TCS_TRG_STYLE_WIDTH (1)        
#define HW_TB_TCS_CAP_CLASS_WIDTH (2)        
#define HW_TB_TCS_TRG_CLASS_WIDTH (2)        
#define HW_TB_TCS_FREEZE_WIDTH (1)        
#define HW_TB_TCS_RSVD_WIDTH (18)

#define HW_TB_TCS_TRG_STYLE_SETMASK (((1<<HW_TB_TCS_TRG_STYLE_WIDTH)-1)<<HW_TB_TCS_TRG_STYLE_BITPOS) 
#define HW_TB_TCS_CAP_CLASS_SETMASK (((1<<HW_TB_TCS_CAP_CLASS_WIDTH)-1)<<HW_TB_TCS_CAP_CLASS_BITPOS) 
#define HW_TB_TCS_TRG_CLASS_SETMASK (((1<<HW_TB_TCS_TRG_CLASS_WIDTH)-1)<<HW_TB_TCS_TRG_CLASS_BITPOS) 
#define HW_TB_TCS_FREEZE_SETMASK (((1<<HW_TB_TCS_FREEZE_WIDTH)-1)<<HW_TB_TCS_FREEZE_BITPOS) 

#define HW_TB_TCS_TRG_STYLE_CLRMASK (~(WORD)HW_TB_TCS_TRG_STYLE_SETMASK)
#define HW_TB_TCS_CAP_CLASS_CLRMASK (~(WORD)HW_TB_TCS_CAP_CLASS_SETMASK)
#define HW_TB_TCS_TRG_CLASS_CLRMASK (~(WORD)HW_TB_TCS_TRG_CLASS_SETMASK)
#define HW_TB_TCS_FREEZE_CLRMASK (~(WORD)HW_TB_TCS_FREEZE_SETMASK)

typedef union               
{
    struct {
	 int TRG_STYLE       : HW_TB_TCS_TRG_STYLE_WIDTH;
	 int CAP_CLASS       : HW_TB_TCS_CAP_CLASS_WIDTH;
	 int TRG_CLASS       : HW_TB_TCS_TRG_CLASS_WIDTH;
	 int FREEZE          : HW_TB_TCS_FREEZE_WIDTH;
        int reserved        : HW_TB_TCS_RSVD_WIDTH;
    } B;
    int I;
    unsigned int U;
} tb_tcs_type;
#define HW_TB_TCS      (*(volatile tb_tcs_type _X*) (HW_TB_BASEADDR+16))    /* Trace Buffer Trigger Command Register */

/////////////////////////////////////////////////////////////////////////////////

//  Trace Buffer Trigger Value Register (HW_TB_TVR) Bit Definitions

#define HW_TB_TVR_MATCH_ADDR_BITPOS (0)

#define HW_TB_TVR_MATCH_ADDR_WIDTH (16)        
#define HW_TB_TVR_RSVD_WIDTH (8)

#define HW_TB_TVR_MATCH_ADDR_SETMASK (((1<<HW_TB_TVR_MATCH_ADDR_WIDTH)-1)<<HW_TB_TVR_MATCH_ADDR_BITPOS) 

#define HW_TB_TVR_MATCH_ADDR_CLRMASK (~(WORD)HW_TB_TVR_MATCH_ADDR_SETMASK)

typedef union               
{
    struct {
	 int MATCH_ADDR      : HW_TB_TVR_MATCH_ADDR_WIDTH;
        int reserved        : HW_TB_TVR_RSVD_WIDTH;
    } B;
    int I;
    unsigned int U;
} tb_tvr_type;
#define HW_TB_TVR      (*(volatile tb_tvr_type _X*) (HW_TB_BASEADDR+24))    /* Trace Buffer Trigger Value Register */



#endif

















