//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;; Copyright(C) SigmaTel, Inc. 2002-2003
//;; Filename    : regsswizzle.inc
//;; Description : Register definitions for Swizzle interface
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

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
#if !(defined(regsswizzleinc))
#define regsswizzleinc 1

#include "types.h"

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

//   SWIZZLE STMP3500 Registers 
//   Last Updated 7.11.2003 Dave Dyches
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;





#define HW_SWIZZLE_BASEADDR 0xF380



////  Swizzle CSR1 (HW_SWIZZLECSR1) Bit Definitions
#define HW_SWIZZLECSR1_EN_BITPOS (0)
#define HW_SWIZZLECSR1_LA_BITPOS (1)
#define HW_SWIZZLECSR1_LNR_BITPOS (2)
#define HW_SWIZZLECSR1_SIGN_BITPOS (3)
#define HW_SWIZZLECSR1_SHIFT_BITPOS (4)
#define HW_SWIZZLECSR1_MEM_BITPOS (8)
#define HW_SWIZZLECSR1_CLK_OFF_BITPOS (9)
#define HW_SWIZZLECSR1_NEWADD_BITPOS (10)
#define HW_SWIZZLECSR1_RSVD_BITPOS (11)

#define HW_SWIZZLECSR1_EN_WIDTH (1)
#define HW_SWIZZLECSR1_LA_WIDTH (1)
#define HW_SWIZZLECSR1_LNR_WIDTH (1)
#define HW_SWIZZLECSR1_SIGN_WIDTH (1)
#define HW_SWIZZLECSR1_SHIFT_WIDTH (4)
#define HW_SWIZZLECSR1_MEM_WIDTH (1)
#define HW_SWIZZLECSR1_CLK_OFF_WIDTH (1)
#define HW_SWIZZLECSR1_NEWADD_WIDTH (1)
#define HW_SWIZZLECSR1_RSVD_WIDTH (13)

#define HW_SWIZZLECSR1_EN_SETMASK (((1<HW_SWIZZLECSR1_EN_WIDTH)-1)<<HW_SWIZZLECSR1_EN_BITPOS)
#define HW_SWIZZLECSR1_LA_SETMASK (((1<HW_SWIZZLECSR1_LA_WIDTH)-1)<<HW_SWIZZLECSR1_LA_BITPOS)
#define HW_SWIZZLECSR1_LNR_SETMASK (((1<HW_SWIZZLECSR1_LNR_WIDTH)-1)<<HW_SWIZZLECSR1_LNR_BITPOS)
#define HW_SWIZZLECSR1_SIGN_SETMASK (((1<HW_SWIZZLECSR1_SIGN_WIDTH)-1)<<HW_SWIZZLECSR1_SIGN_BITPOS)
#define HW_SWIZZLECSR1_SHIFT_SETMASK (((1<HW_SWIZZLECSR1_SHIFT_WIDTH)-1)<<HW_SWIZZLECSR1_SHIFT_BITPOS)
#define HW_SWIZZLECSR1_MEM_SETMASK (((1<HW_SWIZZLECSR1_MEM_WIDTH)-1)<<HW_SWIZZLECSR1_MEM_BITPOS)
#define HW_SWIZZLECSR1_CLK_OFF_SETMASK (((1<HW_SWIZZLECSR1_CLK_OFF_WIDTH)-1)<<HW_SWIZZLECSR1_CLK_OFF_BITPOS)
#define HW_SWIZZLECSR1_NEWADD_SETMASK (((1<HW_SWIZZLECSR1_NEWADD_WIDTH)-1)<<HW_SWIZZLECSR1_NEWADD_BITPOS)
#define HW_SWIZZLECSR1_RSVD_SETMASK (((1<HW_SWIZZLECSR1_RSVD_WIDTH)-1)<<HW_SWIZZLECSR1_RSVD_BITPOS)

#define HW_SWIZZLECSR1_EN_CLRMASK (~(WORD)HW_SWIZZLECSR1_EN_SETMASK)
#define HW_SWIZZLECSR1_LA_CLRMASK (~(WORD)HW_SWIZZLECSR1_LA_SETMASK)
#define HW_SWIZZLECSR1_LNR_CLRMASK (~(WORD)HW_SWIZZLECSR1_LNR_SETMASK)
#define HW_SWIZZLECSR1_SIGN_CLRMASK (~(WORD)HW_SWIZZLECSR1_SIGN_SETMASK)
#define HW_SWIZZLECSR1_SHIFT_CLRMASK (~(WORD)HW_SWIZZLECSR1_SHIFT_SETMASK)
#define HW_SWIZZLECSR1_MEM_CLRMASK (~(WORD)HW_SWIZZLECSR1_MEM_SETMASK)
#define HW_SWIZZLECSR1_CLK_OFF_CLRMASK (~(WORD)HW_SWIZZLECSR1_CLK_OFF_SETMASK)
#define HW_SWIZZLECSR1_NEWADD_CLRMASK (~(WORD)HW_SWIZZLECSR1_NEWADD_SETMASK)
#define HW_SWIZZLECSR1_RSVD_CLRMASK (~(WORD)HW_SWIZZLECSR1_RSVD_SETMASK)

////////////////////////////////////////////////////////////////////////////////
//  Bit Manipulation Unit Registers
////////////////////////////////////////////////////////////////////////////////
typedef union
{
    struct
    {
    int EN      :1;     /* Swizzle Enable                           */
    int LA      :1;     /* Left Align Data                          */
    int LNR     :1;     /* Left Barrel Shift                        */
    int SIGN    :1;     /* Sign Extend Data                         */
    unsigned SHIFT :4;          /* Number of positions to shift (0 to 23)   */
    int MEM     :1;     /* Manipulate in Memory (not in registers)  */
    int CLK_OFF :1;     /* Gate the Clock, Power Off                */
    int NEWADD  :1;     /* Place the data into a new location       */     
    } B;
    int I;
    unsigned U;
} swizzlecsr1_type;
////////////////////////////////////////////////////////////////////////////////
#define HW_SWIZZLECS1R (*(volatile swizzlecsr1_type _X*) (HW_SWIZZLE_BASEADDR))       /* Swizzle Control & Status Register 1 */

////  Swizzle CSR2 (HW_SWIZZLECSR2) Bit Definitions
#define HW_SWIZZLECSR2_KICK_BITPOS (0)
#define HW_SWIZZLECSR2_SASEL_BITPOS (1)
#define HW_SWIZZLECSR2_DESASEL_BITPOS (3)
#define HW_SWIZZLECSR2_BIGE_BITPOS (5)
#define HW_SWIZZLECSR2_BITREV_BITPOS (6)
#define HW_SWIZZLECSR2_PLSB_BITPOS (7)
#define HW_SWIZZLECSR2_PISB_OFF_BITPOS (8)
#define HW_SWIZZLECSR2_PMSB_BITPOS (9)
#define HW_SWIZZLECSR2_P16L_BITPOS (10)
#define HW_SWIZZLECSR2_P16I_BITPOS (11)
#define HW_SWIZZLECSR2_BS_EN_BITPOS (12)
#define HW_SWIZZLECSR2_SBYTEDEST_BITPOS (13)
#define HW_SWIZZLECSR2_UNKICK_BITPOS (15)
#define HW_SWIZZLECSR2_RSVD_BITPOS (16)

#define HW_SWIZZLECSR2_KICK_WIDTH (1)
#define HW_SWIZZLECSR2_SASEL_WIDTH (2)
#define HW_SWIZZLECSR2_DESASEL_WIDTH (2)
#define HW_SWIZZLECSR2_BIGE_WIDTH (1)
#define HW_SWIZZLECSR2_BITREV_WIDTH (1)
#define HW_SWIZZLECSR2_PLSB_WIDTH (1)
#define HW_SWIZZLECSR2_PMSB_WIDTH (1)
#define HW_SWIZZLECSR2_P16L_WIDTH (1)
#define HW_SWIZZLECSR2_P16I_WIDTH (1)
#define HW_SWIZZLECSR2_BS_EN_WIDTH (1)
#define HW_SWIZZLECSR2_SBYTEDEST_WIDTH (2)
#define HW_SWIZZLECSR2_UNKICK_WIDTH (1)
#define HW_SWIZZLECSR2_RSVD_WIDTH (8)

#define HW_SWIZZLECSR2_KICK_SETMASK (((1<HW_SWIZZLECSR2_KICK_WIDTH)-1)<<HW_SWIZZLECSR2_KICK_BITPOS)
#define HW_SWIZZLECSR2_SASEL_SETMASK (((1<HW_SWIZZLECSR2_SASEL_WIDTH)-1)<<HW_SWIZZLECSR2_SASEL_BITPOS)
#define HW_SWIZZLECSR2_DESASEL_SETMASK (((1<HW_SWIZZLECSR2_DESASEL_WIDTH)-1)<<HW_SWIZZLECSR2_DESASEL_BITPOS)
#define HW_SWIZZLECSR2_BIGE_SETMASK (((1<HW_SWIZZLECSR2_BIGE_WIDTH)-1)<<HW_SWIZZLECSR2_BIGE_BITPOS)
#define HW_SWIZZLECSR2_BITREV_SETMASK (((1<HW_SWIZZLECSR2_BITREV_WIDTH)-1)<<HW_SWIZZLECSR2_BITREV_BITPOS)
#define HW_SWIZZLECSR2_PLSB_SETMASK (((1<HW_SWIZZLECSR2_PLSB_WIDTH)-1)<<HW_SWIZZLECSR2_PLSB_BITPOS)
#define HW_SWIZZLECSR2_PMSB_SETMASK (((1<HW_SWIZZLECSR2_PMSB_WIDTH)-1)<<HW_SWIZZLECSR2_PMSB_BITPOS)
#define HW_SWIZZLECSR2_P16L_SETMASK (((1<HW_SWIZZLECSR2_P16L_WIDTH)-1)<<HW_SWIZZLECSR2_P16L_BITPOS)
#define HW_SWIZZLECSR2_P16I_SETMASK (((1<HW_SWIZZLECSR2_P16I_WIDTH)-1)<<HW_SWIZZLECSR2_P16I_BITPOS)
#define HW_SWIZZLECSR2_BS_EN_SETMASK (((1<HW_SWIZZLECSR2_BS_EN_WIDTH)-1)<<HW_SWIZZLECSR2_BS_EN_BITPOS)
#define HW_SWIZZLECSR2_SBYTEDEST_SETMASK (((1<HW_SWIZZLECSR2_SBYTEDEST_WIDTH)-1)<<HW_SWIZZLECSR2_SBYTEDEST_BITPOS)
#define HW_SWIZZLECSR2_UNKICK_SETMASK (((1<HW_SWIZZLECSR2_UNKICK_WIDTH)-1)<<HW_SWIZZLECSR2_UNKICK_BITPOS)
#define HW_SWIZZLECSR2_RSVD_SETMASK (((1<HW_SWIZZLECSR2_RSVD_WIDTH)-1)<<HW_SWIZZLECSR2_RSVD_BITPOS)

#define HW_SWIZZLECSR2_KICK_CLRMASK (~(WORD)HW_SWIZZLECSR2_KICK_SETMASK)
#define HW_SWIZZLECSR2_SASEL_CLRMASK (~(WORD)HW_SWIZZLECSR2_SASEL_SETMASK)
#define HW_SWIZZLECSR2_DESASEL_CLRMASK (~(WORD)HW_SWIZZLECSR2_DESASEL_SETMASK)
#define HW_SWIZZLECSR2_BIGE_CLRMASK (~(WORD)HW_SWIZZLECSR2_BIGE_SETMASK)
#define HW_SWIZZLECSR2_BITREV_CLRMASK (~(WORD)HW_SWIZZLECSR2_BITREV_SETMASK)
#define HW_SWIZZLECSR2_PLSB_CLRMASK (~(WORD)HW_SWIZZLECSR2_PLSB_SETMASK)
#define HW_SWIZZLECSR2_PMSB_CLRMASK (~(WORD)HW_SWIZZLECSR2_PMSB_SETMASK)
#define HW_SWIZZLECSR2_P16L_CLRMASK (~(WORD)HW_SWIZZLECSR2_P16L_SETMASK)
#define HW_SWIZZLECSR2_P16I_CLRMASK (~(WORD)HW_SWIZZLECSR2_P16I_SETMASK)
#define HW_SWIZZLECSR2_BS_EN_CLRMASK (~(WORD)HW_SWIZZLECSR2_BS_EN_SETMASK)
#define HW_SWIZZLECSR2_SBYTEDEST_CLRMASK (~(WORD)HW_SWIZZLECSR2_SBYTEDEST_SETMASK)
#define HW_SWIZZLECSR2_UNKICK_CLRMASK (~(WORD)HW_SWIZZLECSR2_UNKICK_SETMASK)
#define HW_SWIZZLECSR2_RSVD_CLRMASK (~(WORD)HW_SWIZZLECSR2_RSVD_SETMASK)

///////////////////////////////////////////////////////////////////////////////
typedef union
{
    struct
    {
    int KICK    :1;         /* Start transfer                      */
    unsigned SASEL :2;              /* Source memory Select                */
    unsigned DESASEL :2;                    /* Destination memory Select           */
    int BIGE        :1;             /* Big Endian Enable                   */
    int BITREV      :1;     /* Bit reverse the data                */
    int PLSB        :1;             /* Pass Least Significant Byte         */
    int PISB        :1;     /* Pass Intermediate Byte              */
    int PMSB        :1;     /* Pass Most Significant Byte          */
    int P16L        :1;     /* Pass Least Significant word         */
    int P16I        :1;     /* Pass Intermediate significant word  */
    int BS_EN       :1;     /* Barrel Shift Enable                 */
    unsigned SBYTE  :2;                     /* Start byte                          */
    int UNKICK      :1;     /* Halt transfer                       */  
    } B;
    unsigned int I;
    unsigned int U;
} swizzlecsr2_type;
///////////////////////////////////////////////////////////////////////////////
#define HW_SWIZZLECS2R (*(volatile swizzlecsr2_type _X*) (HW_SWIZZLE_BASEADDR+1))     /* Swizzle Control & Status Register 2 */

////  Swizzle SIZER (HW_SWIZZLESIZER) Bit Definitions
#define HW_SWIZZLESIZER_SIZE_BITPOS (0)
#define HW_SWIZZLESIZER_NEW_SHIFT_BITPOS (16)
#define HW_SWIZZLESIZER_RSVD_BITPOS (21)

#define HW_SWIZZLESIZER_SIZE_WIDTH (16)
#define HW_SWIZZLESIZER_NEW_SHIFT_WIDTH (5)
#define HW_SWIZZLESIZER_RSVD_WIDTH (3)

#define HW_SWIZZLESIZER_SIZE_SETMASK (((1<HW_SWIZZLESIZER_SIZE_WIDTH)-1)<<HW_SWIZZLESIZER_SIZE_BITPOS)
#define HW_SWIZZLESIZER_NEW_SHIFT_SETMASK (((1<HW_SWIZZLESIZER_NEW_SHIFT_WIDTH)-1)<<HW_SWIZZLESIZER_NEW_SHIFT_BITPOS)
#define HW_SWIZZLESIZER_RSVD_SETMASK (((1<HW_SWIZZLESIZER_RSVD_WIDTH)-1)<<HW_SWIZZLESIZER_RSVD_BITPOS)

#define HW_SWIZZLESIZER_SIZE_CLRMASK (~(WORD)HW_SWIZZLESIZER_SIZE_SETMASK)
#define HW_SWIZZLESIZER_NEW_SHIFT_CLRMASK (~(WORD)HW_SWIZZLESIZER_NEW_SHIFT_SETMASK)
#define HW_SWIZZLESIZER_RSVD_CLRMASK (~(WORD)HW_SWIZZLESIZER_RSVD_SETMASK)

///////////////////////////////////////////////////////////////////////////////
typedef union
{
    struct
    {
    unsigned SIZE      :16;        /* Number of memory words to manipulate */
    unsigned NEW_SHIFT :5;         /* Source memory Select                 */
    } B;
    int I;
    unsigned U;
} swizzlesizer_type;
///////////////////////////////////////////////////////////////////////////////
#define HW_SWIZZLESIZER (*(volatile swizzlesizer_type _X*) (HW_SWIZZLE_BASEADDR+2))        /* Swizzle Transfer Size Register */
#define HW_SWIZZLESOURCER (*(volatile unsigned int _X*) (HW_SWIZZLE_BASEADDR+3))      /* Swizzle Source Address Register */
#define HW_SWIZZLEDATA1R (*(volatile unsigned int _X*) (HW_SWIZZLE_BASEADDR+4))       /* Swizzle Data1 Register */
#define HW_SWIZZLEDATA2R (*(volatile unsigned int _X*) (HW_SWIZZLE_BASEADDR+5))       /* Swizzle Data2 Register */
#define HW_SWIZZLEDESTADDRR (*(volatile unsigned int _X*) (HW_SWIZZLE_BASEADDR+6))    /* Swizzle Destination Address Register */
#define HW_SWIZZLEBIGENDIANR (*(volatile unsigned int _X*) (HW_SWIZZLE_BASEADDR+7))   /* Swizzle Big Endian Register */
#define HW_SWIZZLEBITREVR (*(volatile unsigned int _X*) (HW_SWIZZLE_BASEADDR+8))      /* Swizzle BITREV Register */
#define HW_SWIZZLEPASSLSBR (*(volatile unsigned int _X*) (HW_SWIZZLE_BASEADDR+9))     /* Swizzle Pass Least Significant Byte Register */
#define HW_SWIZZLEPASSISBR (*(volatile unsigned int _X*) (HW_SWIZZLE_BASEADDR+10))    /* Swizzle Pass Intermediate Byte Register */
#define HW_SWIZZLEPASSMSBR (*(volatile unsigned int _X*) (HW_SWIZZLE_BASEADDR+11))    /* Swizzle Pass Most Significant Byte Register */
#define HW_SWIZZLEPASSLSWR (*(volatile unsigned int _X*) (HW_SWIZZLE_BASEADDR+12))    /* Swizzle Pass Least Significant Word Register */
#define HW_SWIZZLEPASSISWR (*(volatile unsigned int _X*) (HW_SWIZZLE_BASEADDR+13))    /* Swizzle Pass Intermediate Significant Word Register */
#define HW_SWIZZLEPASSMSWR (*(volatile unsigned int _X*) (HW_SWIZZLE_BASEADDR+14))    /* Swizzle Pass Most Significant Word Register */
#define HW_SWIZZLEBARRELR (*(volatile unsigned int _X*) (HW_SWIZZLE_BASEADDR+15))    /* Swizzle Barrel Shift Register */
#define HW_SWIZZLEDIV3LR (*(volatile unsigned int _X*) (HW_SWIZZLE_BASEADDR+16))    /* Swizzle Divide By 3 Lower Register */


////  Swizzle DIV3UR (HW_SWIZZLEDIV3UR) Bit Definitions
#define HW_SWIZZLEDIV3UR_DIV3_UPPER_BITPOS (0)
#define HW_SWIZZLEDIV3UR_RSVD0_BITPOS (8)
#define HW_SWIZZLEDIV3UR_REMAINDER_BITPOS (20)
#define HW_SWIZZLEDIV3UR_RSVD1_BITPOS (22)

#define HW_SWIZZLEDIV3UR_DIV3_UPPER_WIDTH (8)
#define HW_SWIZZLEDIV3UR_RSVD0_WIDTH (12)
#define HW_SWIZZLEDIV3UR_REMAINDER_WIDTH (2)
#define HW_SWIZZLEDIV3UR_RSVD1_WIDTH (2)

#define HW_SWIZZLEDIV3UR_DIV3_UPPER_SETMASK (((1<HW_SWIZZLEDIV3UR_DIV3_UPPER_WIDTH)-1)<<HW_SWIZZLEDIV3UR_DIV3_UPPER_BITPOS)
#define HW_SWIZZLEDIV3UR_RSVD0_SETMASK (((1<HW_SWIZZLEDIV3UR_RSVD0_WIDTH)-1)<<HW_SWIZZLEDIV3UR_RSVD0_BITPOS)
#define HW_SWIZZLEDIV3UR_REMAINDER_SETMASK (((1<HW_SWIZZLEDIV3UR_REMAINDER_WIDTH)-1)<<HW_SWIZZLEDIV3UR_REMAINDER_BITPOS)
#define HW_SWIZZLEDIV3UR_RSVD1_SETMASK (((1<HW_SWIZZLEDIV3UR_RSVD1_WIDTH)-1)<<HW_SWIZZLEDIV3UR_RSVD1_BITPOS)

#define HW_SWIZZLEDIV3UR_DIV3_UPPER_CLRMASK (~(WORD)HW_SWIZZLEDIV3UR_DIV3_UPPER_SETMASK)
#define HW_SWIZZLEDIV3UR_RSVD0_CLRMASK (~(WORD)HW_SWIZZLEDIV3UR_RSVD0_SETMASK)
#define HW_SWIZZLEDIV3UR_REMAINDER_CLRMASK (~(WORD)HW_SWIZZLEDIV3UR_REMAINDER_SETMASK)
#define HW_SWIZZLEDIV3UR_RSVD1_CLRMASK (~(WORD)HW_SWIZZLEDIV3UR_RSVD1_SETMASK)

///////////////////////////////////////////////////////////////////////////////
typedef union
{
    struct
    {
    unsigned DIV3_UPPER    :8;     /* Number of memory words to manipulate */
    unsigned RSVD0         :12;         /* Source memory Select                 */
    unsigned REMAINDER     :2;         /* Source memory Select                 */
    } B;
    int I;
    unsigned U;
} swizzlediv3ur_type;
///////////////////////////////////////////////////////////////////////////////
#define HW_SWIZZLEDIV3UR (*(volatile swizzlediv3ur_type _X*) (HW_SWIZZLE_BASEADDR+17))    /* Swizzle Divide By 3 Upper Register */

#endif

