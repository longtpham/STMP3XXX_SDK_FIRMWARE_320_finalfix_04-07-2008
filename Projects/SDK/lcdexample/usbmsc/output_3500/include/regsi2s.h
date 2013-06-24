/////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2003
// Filename: regsi2s.inc
// Description: Register definitions for I2S interface
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
#if !(defined(regsi2sinc))
#define regsi2sinc 1

#include "types.h"

///////////////////////////////////////////////////////////////////////////////////
////  I2S Registers (SAI)
///////////////////////////////////////////////////////////////////////////////////


#define HW_SAI_BASEADDR (0xFFF0)




#define HW_SAIRCSR_REN0_BITPOS (0)
#define HW_SAIRCSR_REN1_BITPOS (1)
#define HW_SAIRCSR_REN2_BITPOS (2)
#define HW_SAIRCSR_RMME_BITPOS (3)
#define HW_SAIRCSR_RSVD0_BITPOS (4)
#define HW_SAIRCSR_RWL_BITPOS (5)
#define HW_SAIRCSR_RDIR_BITPOS (7)
#define HW_SAIRCSR_RLRS_BITPOS (8)
#define HW_SAIRCSR_RCKP_BITPOS (9)
#define HW_SAIRCSR_RREL_BITPOS (10)
#define HW_SAIRCSR_RDWJ_BITPOS (11)
#define HW_SAIRCSR_RXIE_BITPOS (12)
#define HW_SAIRCSR_RSVD1_BITPOS (13)
#define HW_SAIRCSR_ROFL_BITPOS (14)
#define HW_SAIRCSR_RDR_BITPOS (15)
#define HW_SAIRCSR_ROFCL_BITPOS (16)
#define HW_SAIRCSR_RSVD2_BITPOS (17)


#define HW_SAIRCSR_REN0_WIDTH (1)
#define HW_SAIRCSR_REN1_WIDTH (1)
#define HW_SAIRCSR_REN2_WIDTH (1)
#define HW_SAIRCSR_RMME_WIDTH (1)
#define HW_SAIRCSR_RSVD0_WIDTH (1)
#define HW_SAIRCSR_RWL_WIDTH (2)
#define HW_SAIRCSR_RDIR_WIDTH (1)
#define HW_SAIRCSR_RLRS_WIDTH (1)
#define HW_SAIRCSR_RCKP_WIDTH (1)
#define HW_SAIRCSR_RREL_WIDTH (1)
#define HW_SAIRCSR_RDWJ_WIDTH (1)
#define HW_SAIRCSR_RXIE_WIDTH (1)
#define HW_SAIRCSR_RSVD1_WIDTH (1)
#define HW_SAIRCSR_ROFL_WIDTH (1)
#define HW_SAIRCSR_RDR_WIDTH (1)
#define HW_SAIRCSR_ROFCL_WIDTH (1)
#define HW_SAIRCSR_RSVD2_WIDTH (7)


#define HW_SAIRCSR_REN0_SETMASK (((1<HW_SAIRCSR_REN0_WIDTH)-1)<<HW_SAIRCSR_REN0_BITPOS)
#define HW_SAIRCSR_REN1_SETMASK (((1<HW_SAIRCSR_REN1_WIDTH)-1)<<HW_SAIRCSR_REN1_BITPOS)
#define HW_SAIRCSR_REN2_SETMASK (((1<HW_SAIRCSR_REN2_WIDTH)-1)<<HW_SAIRCSR_REN2_BITPOS)
#define HW_SAIRCSR_RMME_SETMASK (((1<HW_SAIRCSR_RMME_WIDTH)-1)<<HW_SAIRCSR_RMME_BITPOS)
#define HW_SAIRCSR_RSVD0_SETMASK (((1<HW_SAIRCSR_RSVD0_WIDTH)-1)<<HW_SAIRCSR_RSVD0_BITPOS)
#define HW_SAIRCSR_RWL_SETMASK (((1<HW_SAIRCSR_RWL_WIDTH)-1)<<HW_SAIRCSR_RWL_BITPOS)
#define HW_SAIRCSR_RDIR_SETMASK (((1<HW_SAIRCSR_RDIR_WIDTH)-1)<<HW_SAIRCSR_RDIR_BITPOS)
#define HW_SAIRCSR_RLRS_SETMASK (((1<HW_SAIRCSR_RLRS_WIDTH)-1)<<HW_SAIRCSR_RLRS_BITPOS)
#define HW_SAIRCSR_RCKP_SETMASK (((1<HW_SAIRCSR_RCKP_WIDTH)-1)<<HW_SAIRCSR_RCKP_BITPOS)
#define HW_SAIRCSR_RREL_SETMASK (((1<HW_SAIRCSR_RREL_WIDTH)-1)<<HW_SAIRCSR_RREL_BITPOS)
#define HW_SAIRCSR_RDWJ_SETMASK (((1<HW_SAIRCSR_RDWJ_WIDTH)-1)<<HW_SAIRCSR_RDWJ_BITPOS)
#define HW_SAIRCSR_RXIE_SETMASK (((1<HW_SAIRCSR_RXIE_WIDTH)-1)<<HW_SAIRCSR_RXIE_BITPOS)
#define HW_SAIRCSR_RSVD1_SETMASK (((1<HW_SAIRCSR_RSVD1_WIDTH)-1)<<HW_SAIRCSR_RSVD1_BITPOS)
#define HW_SAIRCSR_ROFL_SETMASK (((1<HW_SAIRCSR_ROFL_WIDTH)-1)<<HW_SAIRCSR_ROFL_BITPOS)
#define HW_SAIRCSR_RDR_SETMASK (((1<HW_SAIRCSR_RDR_WIDTH)-1)<<HW_SAIRCSR_RDR_BITPOS)
#define HW_SAIRCSR_ROFCL_SETMASK (((1<HW_SAIRCSR_ROFCL_WIDTH)-1)<<HW_SAIRCSR_ROFCL_BITPOS)
#define HW_SAIRCSR_RSVD2_SETMASK (((1<HW_SAIRCSR_RSVD2_WIDTH)-1)<<HW_SAIRCSR_RSVD2_BITPOS)


#define HW_SAIRCSR_REN0_CLRMASK (~(WORD)HW_SAIRCSR_REN0_SETMASK)
#define HW_SAIRCSR_REN1_CLRMASK (~(WORD)HW_SAIRCSR_REN1_SETMASK)
#define HW_SAIRCSR_REN2_CLRMASK (~(WORD)HW_SAIRCSR_REN2_SETMASK)
#define HW_SAIRCSR_RMME_CLRMASK (~(WORD)HW_SAIRCSR_RMME_SETMASK)
#define HW_SAIRCSR_RSVD0_CLRMASK (~(WORD)HW_SAIRCSR_RSVD0_SETMASK)
#define HW_SAIRCSR_RWL_CLRMASK (~(WORD)HW_SAIRCSR_RWL_SETMASK)
#define HW_SAIRCSR_RDIR_CLRMASK (~(WORD)HW_SAIRCSR_RDIR_SETMASK)
#define HW_SAIRCSR_RLRS_CLRMASK (~(WORD)HW_SAIRCSR_RLRS_SETMASK)
#define HW_SAIRCSR_RCKP_CLRMASK (~(WORD)HW_SAIRCSR_RCKP_SETMASK)
#define HW_SAIRCSR_RREL_CLRMASK (~(WORD)HW_SAIRCSR_RREL_SETMASK)
#define HW_SAIRCSR_RDWJ_CLRMASK (~(WORD)HW_SAIRCSR_RDWJ_SETMASK)
#define HW_SAIRCSR_RXIE_CLRMASK (~(WORD)HW_SAIRCSR_RXIE_SETMASK)
#define HW_SAIRCSR_RSVD1_CLRMASK (~(WORD)HW_SAIRCSR_RSVD1_SETMASK)
#define HW_SAIRCSR_ROFL_CLRMASK (~(WORD)HW_SAIRCSR_ROFL_SETMASK)
#define HW_SAIRCSR_RDR_CLRMASK (~(WORD)HW_SAIRCSR_RDR_SETMASK)
#define HW_SAIRCSR_ROFCL_CLRMASK (~(WORD)HW_SAIRCSR_ROFCL_SETMASK)
#define HW_SAIRCSR_RSVD2_CLRMASK (~(WORD)HW_SAIRCSR_RSVD2_SETMASK)

typedef union
{
    struct {
        int REN0        :HW_SAIRCSR_REN0_WIDTH;        // Receiver 0 enable
        int REN1        :HW_SAIRCSR_REN1_WIDTH;        // Receiver 1 enable
        int REN2        :HW_SAIRCSR_REN2_WIDTH;        // Receiver 2 enable
        int RMME        :HW_SAIRCSR_RMME_WIDTH;        // Receiver master mode enable
        int RSVD0       :HW_SAIRCSR_RSVD0_WIDTH;       // Reserved
        unsigned RWL    :HW_SAIRCSR_RWL_WIDTH;         // Receiver word length control
        int RDIR        :HW_SAIRCSR_RDIR_WIDTH;        // Receiver data shift direction 
        int RLRS        :HW_SAIRCSR_RLRS_WIDTH;        // Receiver left right select
        int RCKP        :HW_SAIRCSR_RCKP_WIDTH;        // Receiver clock polarity
        int RREL        :HW_SAIRCSR_RREL_WIDTH;        // Receiver relative timing
        int RDWJ        :HW_SAIRCSR_RDWJ_WIDTH;        // Receiver data word justification
        int RXIE        :HW_SAIRCSR_RXIE_WIDTH;        // Receiver interrupt enable
        int RSVD1       :HW_SAIRCSR_RSVD1_WIDTH;       // Reserved
        int ROFL        :HW_SAIRCSR_ROFL_WIDTH;        // Receiver data overflow
        int RDR         :HW_SAIRCSR_RDR_WIDTH;         // Receiver data ready flag
        int ROFCL       :HW_SAIRCSR_ROFCL_WIDTH;       // Receiver data overflow clear
        unsigned RSVD2  :HW_SAIRCSR_RSVD2_WIDTH;       // Reserved
    } B;
    int I;
    unsigned U;
} saircsr_type;

#define HW_SAIRCSR (*(volatile saircsr_type _X*) (HW_SAI_BASEADDR))       /* I2S Receive CSR         */

typedef union
{
    struct {
        unsigned SAI :24;
    } B;
    int I;
    unsigned U;
} saixr_type;

#define HW_SAIRX0R (*(volatile saixr_type _X*)  (HW_SAI_BASEADDR+1))       /* I2S Received data reg 0 */
#define HW_SAIRX1R (*(volatile saixr_type _X*)  (HW_SAI_BASEADDR+2))       /* I2S Received data reg 1 */
#define HW_SAIRX2R (*(volatile saixr_type _X*)  (HW_SAI_BASEADDR+3))       /* I2S Received data reg 2 */


#define HW_SAITCSR_TEN0_BITPOS (0)
#define HW_SAITCSR_TEN1_BITPOS (1)
#define HW_SAITCSR_TEN2_BITPOS (2)
#define HW_SAITCSR_TMME_BITPOS (3)
#define HW_SAITCSR_RSVD0_BITPOS (4)
#define HW_SAITCSR_TWL_BITPOS (5)
#define HW_SAITCSR_TDIR_BITPOS (7)
#define HW_SAITCSR_TLRS_BITPOS (8)
#define HW_SAITCSR_TCKP_BITPOS (9)
#define HW_SAITCSR_TREL_BITPOS (10)
#define HW_SAITCSR_TDWE_BITPOS (11)
#define HW_SAITCSR_TXIE_BITPOS (12)
#define HW_SAITCSR_RSVD1_BITPOS (13)
#define HW_SAITCSR_TUFL_BITPOS (14)
#define HW_SAITCSR_TDE_BITPOS (15)
#define HW_SAITCSR_TUFCL_BITPOS (16)
#define HW_SAITCSR_RSVD2_BITPOS (17)


#define HW_SAITCSR_TEN0_WIDTH (1)
#define HW_SAITCSR_TEN1_WIDTH (1)
#define HW_SAITCSR_TEN2_WIDTH (1)
#define HW_SAITCSR_TMME_WIDTH (1)
#define HW_SAITCSR_RSVD0_WIDTH (1)
#define HW_SAITCSR_TWL_WIDTH (2)
#define HW_SAITCSR_TDIR_WIDTH (1)
#define HW_SAITCSR_TLRS_WIDTH (1)
#define HW_SAITCSR_TCKP_WIDTH (1)
#define HW_SAITCSR_TREL_WIDTH (1)
#define HW_SAITCSR_TDWE_WIDTH (1)
#define HW_SAITCSR_TXIE_WIDTH (1)
#define HW_SAITCSR_RSVD1_WIDTH (1)
#define HW_SAITCSR_TUFL_WIDTH (1)
#define HW_SAITCSR_TDE_WIDTH (1)
#define HW_SAITCSR_TUFCL_WIDTH (1)
#define HW_SAITCSR_RSVD2_WIDTH (7)


#define HW_SAITCSR_TEN0_SETMASK (((1<HW_SAITCSR_TEN0_WIDTH)-1)<<HW_SAITCSR_TEN0_BITPOS)
#define HW_SAITCSR_TEN1_SETMASK (((1<HW_SAITCSR_TEN1_WIDTH)-1)<<HW_SAITCSR_TEN1_BITPOS)
#define HW_SAITCSR_TEN2_SETMASK (((1<HW_SAITCSR_TEN2_WIDTH)-1)<<HW_SAITCSR_TEN2_BITPOS)
#define HW_SAITCSR_TMME_SETMASK (((1<HW_SAITCSR_TMME_WIDTH)-1)<<HW_SAITCSR_TMME_BITPOS)
#define HW_SAITCSR_RSVD0_SETMASK (((1<HW_SAITCSR_RSVD0_WIDTH)-1)<<HW_SAITCSR_RSVD0_BITPOS)
#define HW_SAITCSR_TWL_SETMASK (((1<HW_SAITCSR_TWL_WIDTH)-1)<<HW_SAITCSR_TWL_BITPOS)
#define HW_SAITCSR_TDIR_SETMASK (((1<HW_SAITCSR_TDIR_WIDTH)-1)<<HW_SAITCSR_TDIR_BITPOS)
#define HW_SAITCSR_TLRS_SETMASK (((1<HW_SAITCSR_TLRS_WIDTH)-1)<<HW_SAITCSR_TLRS_BITPOS)
#define HW_SAITCSR_TCKP_SETMASK (((1<HW_SAITCSR_TCKP_WIDTH)-1)<<HW_SAITCSR_TCKP_BITPOS)
#define HW_SAITCSR_TREL_SETMASK (((1<HW_SAITCSR_TREL_WIDTH)-1)<<HW_SAITCSR_TREL_BITPOS)
#define HW_SAITCSR_TDWE_SETMASK (((1<HW_SAITCSR_TDWE_WIDTH)-1)<<HW_SAITCSR_TDWE_BITPOS)
#define HW_SAITCSR_TXIE_SETMASK (((1<HW_SAITCSR_TXIE_WIDTH)-1)<<HW_SAITCSR_TXIE_BITPOS)
#define HW_SAITCSR_RSVD1_SETMASK (((1<HW_SAITCSR_RSVD1_WIDTH)-1)<<HW_SAITCSR_RSVD1_BITPOS)
#define HW_SAITCSR_TUFL_SETMASK (((1<HW_SAITCSR_TUFL_WIDTH)-1)<<HW_SAITCSR_TUFL_BITPOS)
#define HW_SAITCSR_TDE_SETMASK (((1<HW_SAITCSR_TDE_WIDTH)-1)<<HW_SAITCSR_TDE_BITPOS)
#define HW_SAITCSR_TUFCL_SETMASK (((1<HW_SAITCSR_TUFCL_WIDTH)-1)<<HW_SAITCSR_TUFCL_BITPOS)
#define HW_SAITCSR_RSVD2_SETMASK (((1<HW_SAITCSR_RSVD2_WIDTH)-1)<<HW_SAITCSR_RSVD2_BITPOS)


#define HW_SAITCSR_TEN0_CLRMASK (~(WORD)HW_SAITCSR_TEN0_SETMASK)
#define HW_SAITCSR_TEN1_CLRMASK (~(WORD)HW_SAITCSR_TEN1_SETMASK)
#define HW_SAITCSR_TEN2_CLRMASK (~(WORD)HW_SAITCSR_TEN2_SETMASK)
#define HW_SAITCSR_TMME_CLRMASK (~(WORD)HW_SAITCSR_TMME_SETMASK)
#define HW_SAITCSR_RSVD0_CLRMASK (~(WORD)HW_SAITCSR_RSVD0_SETMASK)
#define HW_SAITCSR_TWL_CLRMASK (~(WORD)HW_SAITCSR_TWL_SETMASK)
#define HW_SAITCSR_TDIR_CLRMASK (~(WORD)HW_SAITCSR_TDIR_SETMASK)
#define HW_SAITCSR_TLRS_CLRMASK (~(WORD)HW_SAITCSR_TLRS_SETMASK)
#define HW_SAITCSR_TCKP_CLRMASK (~(WORD)HW_SAITCSR_TCKP_SETMASK)
#define HW_SAITCSR_TREL_CLRMASK (~(WORD)HW_SAITCSR_TREL_SETMASK)
#define HW_SAITCSR_TDWE_CLRMASK (~(WORD)HW_SAITCSR_TDWE_SETMASK)
#define HW_SAITCSR_TXIE_CLRMASK (~(WORD)HW_SAITCSR_TXIE_SETMASK)
#define HW_SAITCSR_RSVD1_CLRMASK (~(WORD)HW_SAITCSR_RSVD1_SETMASK)
#define HW_SAITCSR_TUFL_CLRMASK (~(WORD)HW_SAITCSR_TUFL_SETMASK)
#define HW_SAITCSR_TDE_CLRMASK (~(WORD)HW_SAITCSR_TDE_SETMASK)
#define HW_SAITCSR_TUFCL_CLRMASK (~(WORD)HW_SAITCSR_TUFCL_SETMASK)
#define HW_SAITCSR_RSVD2_CLRMASK (~(WORD)HW_SAITCSR_RSVD2_SETMASK)


typedef union
{
    struct {
        int TEN0       :HW_SAITCSR_TEN0_WIDTH;         // Transmitter 0 enable
        int TEN1       :HW_SAITCSR_TEN1_WIDTH;         // Transmitter 1 enable
        int TEN2       :HW_SAITCSR_TEN2_WIDTH;         // Transmitter 2 enable
        int TMME       :HW_SAITCSR_TMME_WIDTH;         // Transmitter master mode enable
        int RSVD0      :HW_SAITCSR_RSVD0_WIDTH;        // Reserved
        unsigned TWL   :HW_SAITCSR_TWL_WIDTH;          // Transmitter word length control
        int TDIR       :HW_SAITCSR_TDIR_WIDTH;         // Transmitter data shift direction 
        int TLRS       :HW_SAITCSR_TLRS_WIDTH;         // Transmitter left right select
        int TCKP       :HW_SAITCSR_TCKP_WIDTH;         // Transmitter clock polarity
        int TREL       :HW_SAITCSR_TREL_WIDTH;         // Transmitter relative timing
        int TDWE       :HW_SAITCSR_TDWE_WIDTH;         // Transmitter data word justification
        int TXIE       :HW_SAITCSR_TXIE_WIDTH;         // Transmitter interrupt enable
        int RSVD1      :HW_SAITCSR_RSVD1_WIDTH;        // Reserved
        int TUFL       :HW_SAITCSR_TUFL_WIDTH;         // Transmitter data overflow
        int TDE        :HW_SAITCSR_TDE_WIDTH;          // Transmitter data ready flag
        int TUFCL      :HW_SAITCSR_TUFCL_WIDTH;        // Transmitter data overflow clear
        unsigned RSVD2 :HW_SAITCSR_RSVD2_WIDTH;        // Reserved
    } B;
    int I;
    unsigned U;
} saitcsr_type;

#define HW_SAITCSR (*(volatile saitcsr_type _X*) (HW_SAI_BASEADDR+5)) /* I2S Transmit CSR      */

#define HW_SAITX0R (*(volatile saixr_type _X*)  (HW_SAI_BASEADDR+6))       /* I2S Transmit data reg 0 */
#define HW_SAITX1R (*(volatile saixr_type _X*)  (HW_SAI_BASEADDR+7))       /* I2S Transmit data reg 1 */
#define HW_SAITX2R (*(volatile saixr_type _X*)  (HW_SAI_BASEADDR+8))       /* I2S Transmit data reg 2 */

#endif
