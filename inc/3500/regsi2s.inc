;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2003
;
; Filename: regsi2s.inc
; Description: Register definitions for I2S interface
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
    if !(@def(regsi2sinc))
regsi2sinc            equ         1

;C insert
;#include "types.h"
;endc

;/////////////////////////////////////////////////////////////////////////////////
;//  I2S Registers (SAI)
;/////////////////////////////////////////////////////////////////////////////////


HW_SAI_BASEADDR     equ     ($FFF0)


;ASM Only
HW_SAIRCSR          equ     (HW_SAI_BASEADDR)
HW_SAIRX0R          equ     (HW_SAI_BASEADDR+1)
HW_SAIRX1R          equ     (HW_SAI_BASEADDR+2)
HW_SAIRX2R          equ     (HW_SAI_BASEADDR+3)
HW_SAITCSR          equ     (HW_SAI_BASEADDR+4)
HW_SAITX0R          equ     (HW_SAI_BASEADDR+5)
HW_SAITX1R          equ     (HW_SAI_BASEADDR+6)
HW_SAITX2R          equ     (HW_SAI_BASEADDR+7)
;endasm


HW_SAIRCSR_REN0_BITPOS          equ     (0)
HW_SAIRCSR_REN1_BITPOS          equ     (1)
HW_SAIRCSR_REN2_BITPOS          equ     (2)
HW_SAIRCSR_RMME_BITPOS          equ     (3)
HW_SAIRCSR_RSVD0_BITPOS         equ     (4)
HW_SAIRCSR_RWL_BITPOS           equ     (5)
HW_SAIRCSR_RDIR_BITPOS          equ     (7)
HW_SAIRCSR_RLRS_BITPOS          equ     (8)
HW_SAIRCSR_RCKP_BITPOS          equ     (9)
HW_SAIRCSR_RREL_BITPOS          equ     (10)
HW_SAIRCSR_RDWJ_BITPOS          equ     (11)
HW_SAIRCSR_RXIE_BITPOS          equ     (12)
HW_SAIRCSR_RSVD1_BITPOS         equ     (13)
HW_SAIRCSR_ROFL_BITPOS          equ     (14)
HW_SAIRCSR_RDR_BITPOS           equ     (15)
HW_SAIRCSR_ROFCL_BITPOS         equ     (16)
HW_SAIRCSR_RSVD2_BITPOS         equ     (17)


HW_SAIRCSR_REN0_WIDTH 	        equ 	(1)
HW_SAIRCSR_REN1_WIDTH           equ     (1)
HW_SAIRCSR_REN2_WIDTH           equ     (1)
HW_SAIRCSR_RMME_WIDTH           equ     (1)
HW_SAIRCSR_RSVD0_WIDTH          equ     (1)
HW_SAIRCSR_RWL_WIDTH            equ     (2)
HW_SAIRCSR_RDIR_WIDTH           equ     (1)
HW_SAIRCSR_RLRS_WIDTH           equ     (1)
HW_SAIRCSR_RCKP_WIDTH           equ     (1)
HW_SAIRCSR_RREL_WIDTH           equ     (1)
HW_SAIRCSR_RDWJ_WIDTH           equ     (1)
HW_SAIRCSR_RXIE_WIDTH           equ     (1)
HW_SAIRCSR_RSVD1_WIDTH          equ     (1)
HW_SAIRCSR_ROFL_WIDTH           equ     (1)
HW_SAIRCSR_RDR_WIDTH            equ     (1)
HW_SAIRCSR_ROFCL_WIDTH          equ     (1)
HW_SAIRCSR_RSVD2_WIDTH          equ     (7)


HW_SAIRCSR_REN0_SETMASK         equ     (((1<HW_SAIRCSR_REN0_WIDTH)-1)<<HW_SAIRCSR_REN0_BITPOS)
HW_SAIRCSR_REN1_SETMASK         equ     (((1<HW_SAIRCSR_REN1_WIDTH)-1)<<HW_SAIRCSR_REN1_BITPOS)
HW_SAIRCSR_REN2_SETMASK         equ     (((1<HW_SAIRCSR_REN2_WIDTH)-1)<<HW_SAIRCSR_REN2_BITPOS)
HW_SAIRCSR_RMME_SETMASK         equ     (((1<HW_SAIRCSR_RMME_WIDTH)-1)<<HW_SAIRCSR_RMME_BITPOS)
HW_SAIRCSR_RSVD0_SETMASK        equ     (((1<HW_SAIRCSR_RSVD0_WIDTH)-1)<<HW_SAIRCSR_RSVD0_BITPOS)
HW_SAIRCSR_RWL_SETMASK          equ     (((1<HW_SAIRCSR_RWL_WIDTH)-1)<<HW_SAIRCSR_RWL_BITPOS)
HW_SAIRCSR_RDIR_SETMASK         equ     (((1<HW_SAIRCSR_RDIR_WIDTH)-1)<<HW_SAIRCSR_RDIR_BITPOS)
HW_SAIRCSR_RLRS_SETMASK         equ     (((1<HW_SAIRCSR_RLRS_WIDTH)-1)<<HW_SAIRCSR_RLRS_BITPOS)
HW_SAIRCSR_RCKP_SETMASK         equ     (((1<HW_SAIRCSR_RCKP_WIDTH)-1)<<HW_SAIRCSR_RCKP_BITPOS)
HW_SAIRCSR_RREL_SETMASK         equ     (((1<HW_SAIRCSR_RREL_WIDTH)-1)<<HW_SAIRCSR_RREL_BITPOS)
HW_SAIRCSR_RDWJ_SETMASK         equ     (((1<HW_SAIRCSR_RDWJ_WIDTH)-1)<<HW_SAIRCSR_RDWJ_BITPOS)
HW_SAIRCSR_RXIE_SETMASK         equ     (((1<HW_SAIRCSR_RXIE_WIDTH)-1)<<HW_SAIRCSR_RXIE_BITPOS)
HW_SAIRCSR_RSVD1_SETMASK        equ     (((1<HW_SAIRCSR_RSVD1_WIDTH)-1)<<HW_SAIRCSR_RSVD1_BITPOS)
HW_SAIRCSR_ROFL_SETMASK         equ     (((1<HW_SAIRCSR_ROFL_WIDTH)-1)<<HW_SAIRCSR_ROFL_BITPOS)
HW_SAIRCSR_RDR_SETMASK          equ     (((1<HW_SAIRCSR_RDR_WIDTH)-1)<<HW_SAIRCSR_RDR_BITPOS)
HW_SAIRCSR_ROFCL_SETMASK        equ     (((1<HW_SAIRCSR_ROFCL_WIDTH)-1)<<HW_SAIRCSR_ROFCL_BITPOS)
HW_SAIRCSR_RSVD2_SETMASK        equ     (((1<HW_SAIRCSR_RSVD2_WIDTH)-1)<<HW_SAIRCSR_RSVD2_BITPOS)


HW_SAIRCSR_REN0_CLRMASK         equ     (~HW_SAIRCSR_REN0_SETMASK)
HW_SAIRCSR_REN1_CLRMASK         equ     (~HW_SAIRCSR_REN1_SETMASK)
HW_SAIRCSR_REN2_CLRMASK         equ     (~HW_SAIRCSR_REN2_SETMASK)
HW_SAIRCSR_RMME_CLRMASK         equ     (~HW_SAIRCSR_RMME_SETMASK)
HW_SAIRCSR_RSVD0_CLRMASK        equ     (~HW_SAIRCSR_RSVD0_SETMASK)
HW_SAIRCSR_RWL_CLRMASK          equ     (~HW_SAIRCSR_RWL_SETMASK)
HW_SAIRCSR_RDIR_CLRMASK         equ     (~HW_SAIRCSR_RDIR_SETMASK)
HW_SAIRCSR_RLRS_CLRMASK         equ     (~HW_SAIRCSR_RLRS_SETMASK)
HW_SAIRCSR_RCKP_CLRMASK         equ     (~HW_SAIRCSR_RCKP_SETMASK)
HW_SAIRCSR_RREL_CLRMASK         equ     (~HW_SAIRCSR_RREL_SETMASK)
HW_SAIRCSR_RDWJ_CLRMASK         equ     (~HW_SAIRCSR_RDWJ_SETMASK)
HW_SAIRCSR_RXIE_CLRMASK         equ     (~HW_SAIRCSR_RXIE_SETMASK)
HW_SAIRCSR_RSVD1_CLRMASK        equ     (~HW_SAIRCSR_RSVD1_SETMASK)
HW_SAIRCSR_ROFL_CLRMASK         equ     (~HW_SAIRCSR_ROFL_SETMASK)
HW_SAIRCSR_RDR_CLRMASK          equ     (~HW_SAIRCSR_RDR_SETMASK)
HW_SAIRCSR_ROFCL_CLRMASK        equ     (~HW_SAIRCSR_ROFCL_SETMASK)
HW_SAIRCSR_RSVD2_CLRMASK        equ     (~HW_SAIRCSR_RSVD2_SETMASK)

;C insert
;typedef union
;{
;    struct {
;        int REN0        :HW_SAIRCSR_REN0_WIDTH;        // Receiver 0 enable
;        int REN1        :HW_SAIRCSR_REN1_WIDTH;        // Receiver 1 enable
;        int REN2        :HW_SAIRCSR_REN2_WIDTH;        // Receiver 2 enable
;        int RMME        :HW_SAIRCSR_RMME_WIDTH;        // Receiver master mode enable
;        int RSVD0       :HW_SAIRCSR_RSVD0_WIDTH;       // Reserved
;        unsigned RWL    :HW_SAIRCSR_RWL_WIDTH;         // Receiver word length control
;        int RDIR        :HW_SAIRCSR_RDIR_WIDTH;        // Receiver data shift direction 
;        int RLRS        :HW_SAIRCSR_RLRS_WIDTH;        // Receiver left right select
;        int RCKP        :HW_SAIRCSR_RCKP_WIDTH;        // Receiver clock polarity
;        int RREL        :HW_SAIRCSR_RREL_WIDTH;        // Receiver relative timing
;        int RDWJ        :HW_SAIRCSR_RDWJ_WIDTH;        // Receiver data word justification
;        int RXIE        :HW_SAIRCSR_RXIE_WIDTH;        // Receiver interrupt enable
;        int RSVD1       :HW_SAIRCSR_RSVD1_WIDTH;       // Reserved
;        int ROFL        :HW_SAIRCSR_ROFL_WIDTH;        // Receiver data overflow
;        int RDR         :HW_SAIRCSR_RDR_WIDTH;         // Receiver data ready flag
;        int ROFCL       :HW_SAIRCSR_ROFCL_WIDTH;       // Receiver data overflow clear
;        unsigned RSVD2  :HW_SAIRCSR_RSVD2_WIDTH;       // Reserved
;    } B;
;    int I;
;    unsigned U;
;} saircsr_type;
;
;#define HW_SAIRCSR (*(volatile saircsr_type _X*) (HW_SAI_BASEADDR))       /* I2S Receive CSR         */
;
;typedef union
;{
;    struct {
;        unsigned SAI :24;
;    } B;
;    int I;
;    unsigned U;
;} saixr_type;
;
;#define HW_SAIRX0R (*(volatile saixr_type _X*)  (HW_SAI_BASEADDR+1))       /* I2S Received data reg 0 */
;#define HW_SAIRX1R (*(volatile saixr_type _X*)  (HW_SAI_BASEADDR+2))       /* I2S Received data reg 1 */
;#define HW_SAIRX2R (*(volatile saixr_type _X*)  (HW_SAI_BASEADDR+3))       /* I2S Received data reg 2 */
;endc


HW_SAITCSR_TEN0_BITPOS          equ     (0)
HW_SAITCSR_TEN1_BITPOS          equ     (1)
HW_SAITCSR_TEN2_BITPOS          equ     (2)
HW_SAITCSR_TMME_BITPOS          equ     (3)
HW_SAITCSR_RSVD0_BITPOS         equ     (4)
HW_SAITCSR_TWL_BITPOS           equ     (5)
HW_SAITCSR_TDIR_BITPOS          equ     (7)
HW_SAITCSR_TLRS_BITPOS          equ     (8)
HW_SAITCSR_TCKP_BITPOS          equ     (9)
HW_SAITCSR_TREL_BITPOS          equ     (10)
HW_SAITCSR_TDWE_BITPOS          equ     (11)
HW_SAITCSR_TXIE_BITPOS          equ     (12)
HW_SAITCSR_RSVD1_BITPOS         equ     (13)
HW_SAITCSR_TUFL_BITPOS          equ     (14)
HW_SAITCSR_TDE_BITPOS           equ     (15)
HW_SAITCSR_TUFCL_BITPOS         equ     (16)
HW_SAITCSR_RSVD2_BITPOS         equ     (17)


HW_SAITCSR_TEN0_WIDTH 	        equ 	(1)
HW_SAITCSR_TEN1_WIDTH           equ     (1)
HW_SAITCSR_TEN2_WIDTH           equ     (1)
HW_SAITCSR_TMME_WIDTH           equ     (1)
HW_SAITCSR_RSVD0_WIDTH          equ     (1)
HW_SAITCSR_TWL_WIDTH            equ     (2)
HW_SAITCSR_TDIR_WIDTH           equ     (1)
HW_SAITCSR_TLRS_WIDTH           equ     (1)
HW_SAITCSR_TCKP_WIDTH           equ     (1)
HW_SAITCSR_TREL_WIDTH           equ     (1)
HW_SAITCSR_TDWE_WIDTH           equ     (1)
HW_SAITCSR_TXIE_WIDTH           equ     (1)
HW_SAITCSR_RSVD1_WIDTH          equ     (1)
HW_SAITCSR_TUFL_WIDTH           equ     (1)
HW_SAITCSR_TDE_WIDTH            equ     (1)
HW_SAITCSR_TUFCL_WIDTH          equ     (1)
HW_SAITCSR_RSVD2_WIDTH          equ     (7)


HW_SAITCSR_TEN0_SETMASK         equ     (((1<HW_SAITCSR_TEN0_WIDTH)-1)<<HW_SAITCSR_TEN0_BITPOS)
HW_SAITCSR_TEN1_SETMASK         equ     (((1<HW_SAITCSR_TEN1_WIDTH)-1)<<HW_SAITCSR_TEN1_BITPOS)
HW_SAITCSR_TEN2_SETMASK         equ     (((1<HW_SAITCSR_TEN2_WIDTH)-1)<<HW_SAITCSR_TEN2_BITPOS)
HW_SAITCSR_TMME_SETMASK         equ     (((1<HW_SAITCSR_TMME_WIDTH)-1)<<HW_SAITCSR_TMME_BITPOS)
HW_SAITCSR_RSVD0_SETMASK        equ     (((1<HW_SAITCSR_RSVD0_WIDTH)-1)<<HW_SAITCSR_RSVD0_BITPOS)
HW_SAITCSR_TWL_SETMASK          equ     (((1<HW_SAITCSR_TWL_WIDTH)-1)<<HW_SAITCSR_TWL_BITPOS)
HW_SAITCSR_TDIR_SETMASK         equ     (((1<HW_SAITCSR_TDIR_WIDTH)-1)<<HW_SAITCSR_TDIR_BITPOS)
HW_SAITCSR_TLRS_SETMASK         equ     (((1<HW_SAITCSR_TLRS_WIDTH)-1)<<HW_SAITCSR_TLRS_BITPOS)
HW_SAITCSR_TCKP_SETMASK         equ     (((1<HW_SAITCSR_TCKP_WIDTH)-1)<<HW_SAITCSR_TCKP_BITPOS)
HW_SAITCSR_TREL_SETMASK         equ     (((1<HW_SAITCSR_TREL_WIDTH)-1)<<HW_SAITCSR_TREL_BITPOS)
HW_SAITCSR_TDWE_SETMASK         equ     (((1<HW_SAITCSR_TDWE_WIDTH)-1)<<HW_SAITCSR_TDWE_BITPOS)
HW_SAITCSR_TXIE_SETMASK         equ     (((1<HW_SAITCSR_TXIE_WIDTH)-1)<<HW_SAITCSR_TXIE_BITPOS)
HW_SAITCSR_RSVD1_SETMASK        equ     (((1<HW_SAITCSR_RSVD1_WIDTH)-1)<<HW_SAITCSR_RSVD1_BITPOS)
HW_SAITCSR_TUFL_SETMASK         equ     (((1<HW_SAITCSR_TUFL_WIDTH)-1)<<HW_SAITCSR_TUFL_BITPOS)
HW_SAITCSR_TDE_SETMASK          equ     (((1<HW_SAITCSR_TDE_WIDTH)-1)<<HW_SAITCSR_TDE_BITPOS)
HW_SAITCSR_TUFCL_SETMASK        equ     (((1<HW_SAITCSR_TUFCL_WIDTH)-1)<<HW_SAITCSR_TUFCL_BITPOS)
HW_SAITCSR_RSVD2_SETMASK        equ     (((1<HW_SAITCSR_RSVD2_WIDTH)-1)<<HW_SAITCSR_RSVD2_BITPOS)


HW_SAITCSR_TEN0_CLRMASK         equ     (~HW_SAITCSR_TEN0_SETMASK)
HW_SAITCSR_TEN1_CLRMASK         equ     (~HW_SAITCSR_TEN1_SETMASK)
HW_SAITCSR_TEN2_CLRMASK         equ     (~HW_SAITCSR_TEN2_SETMASK)
HW_SAITCSR_TMME_CLRMASK         equ     (~HW_SAITCSR_TMME_SETMASK)
HW_SAITCSR_RSVD0_CLRMASK        equ     (~HW_SAITCSR_RSVD0_SETMASK)
HW_SAITCSR_TWL_CLRMASK          equ     (~HW_SAITCSR_TWL_SETMASK)
HW_SAITCSR_TDIR_CLRMASK         equ     (~HW_SAITCSR_TDIR_SETMASK)
HW_SAITCSR_TLRS_CLRMASK         equ     (~HW_SAITCSR_TLRS_SETMASK)
HW_SAITCSR_TCKP_CLRMASK         equ     (~HW_SAITCSR_TCKP_SETMASK)
HW_SAITCSR_TREL_CLRMASK         equ     (~HW_SAITCSR_TREL_SETMASK)
HW_SAITCSR_TDWE_CLRMASK         equ     (~HW_SAITCSR_TDWE_SETMASK)
HW_SAITCSR_TXIE_CLRMASK         equ     (~HW_SAITCSR_TXIE_SETMASK)
HW_SAITCSR_RSVD1_CLRMASK        equ     (~HW_SAITCSR_RSVD1_SETMASK)
HW_SAITCSR_TUFL_CLRMASK         equ     (~HW_SAITCSR_TUFL_SETMASK)
HW_SAITCSR_TDE_CLRMASK          equ     (~HW_SAITCSR_TDE_SETMASK)
HW_SAITCSR_TUFCL_CLRMASK        equ     (~HW_SAITCSR_TUFCL_SETMASK)
HW_SAITCSR_RSVD2_CLRMASK        equ     (~HW_SAITCSR_RSVD2_SETMASK)


;C insert
;typedef union
;{
;    struct {
;        int TEN0       :HW_SAITCSR_TEN0_WIDTH;         // Transmitter 0 enable
;        int TEN1       :HW_SAITCSR_TEN1_WIDTH;         // Transmitter 1 enable
;        int TEN2       :HW_SAITCSR_TEN2_WIDTH;         // Transmitter 2 enable
;        int TMME       :HW_SAITCSR_TMME_WIDTH;         // Transmitter master mode enable
;        int RSVD0      :HW_SAITCSR_RSVD0_WIDTH;        // Reserved
;        unsigned TWL   :HW_SAITCSR_TWL_WIDTH;          // Transmitter word length control
;        int TDIR       :HW_SAITCSR_TDIR_WIDTH;         // Transmitter data shift direction 
;        int TLRS       :HW_SAITCSR_TLRS_WIDTH;         // Transmitter left right select
;        int TCKP       :HW_SAITCSR_TCKP_WIDTH;         // Transmitter clock polarity
;        int TREL       :HW_SAITCSR_TREL_WIDTH;         // Transmitter relative timing
;        int TDWE       :HW_SAITCSR_TDWE_WIDTH;         // Transmitter data word justification
;        int TXIE       :HW_SAITCSR_TXIE_WIDTH;         // Transmitter interrupt enable
;        int RSVD1      :HW_SAITCSR_RSVD1_WIDTH;        // Reserved
;        int TUFL       :HW_SAITCSR_TUFL_WIDTH;         // Transmitter data overflow
;        int TDE        :HW_SAITCSR_TDE_WIDTH;          // Transmitter data ready flag
;        int TUFCL      :HW_SAITCSR_TUFCL_WIDTH;        // Transmitter data overflow clear
;        unsigned RSVD2 :HW_SAITCSR_RSVD2_WIDTH;        // Reserved
;    } B;
;    int I;
;    unsigned U;
;} saitcsr_type;
;
;#define HW_SAITCSR (*(volatile saitcsr_type _X*) (HW_SAI_BASEADDR+5)) /* I2S Transmit CSR      */
;
;#define HW_SAITX0R (*(volatile saixr_type _X*)  (HW_SAI_BASEADDR+6))       /* I2S Transmit data reg 0 */
;#define HW_SAITX1R (*(volatile saixr_type _X*)  (HW_SAI_BASEADDR+7))       /* I2S Transmit data reg 1 */
;#define HW_SAITX2R (*(volatile saixr_type _X*)  (HW_SAI_BASEADDR+8))       /* I2S Transmit data reg 2 */
;endc

    endif