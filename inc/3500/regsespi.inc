;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Copyright(C) SigmaTel, Inc. 2002-2003
;;; Filename    : regsespi.inc
;;; Description : Register definitions for ESPI interface
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

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
    if !(@def(regsespiinc))
regsespiinc            equ         1

;C insert
;#include "types.h"
;endc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;   ESPI STMP Registers 
;   Last Updated 7.1.2003 M. Henson
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

HW_ESPI_BASEADDR             equ     ($FF00)


;ASM Only 

HW_ESPI_CSR                  	equ     (HW_ESPI_BASEADDR+0)
HW_ESPI_CNFG			equ     (HW_ESPI_BASEADDR+1)
HW_ESPI_CLKCNTRL		equ     (HW_ESPI_BASEADDR+2)
HW_ESPI_PIODATA			equ     (HW_ESPI_BASEADDR+3)
HW_ESPI_ADDR			equ     (HW_ESPI_BASEADDR+4)
HW_ESPI_INDEX			equ     (HW_ESPI_BASEADDR+5)

;endasm

; /////////////////////////////////////////////
; //  ESPI Control/Status Register Bit Definitions
HW_ESPI_CSR_KICK_BITPOS        	equ     (0)
HW_ESPI_CSR_RSVD0_BITPOS 	equ     (1)
HW_ESPI_CSR_DONE_INT_BITPOS    	equ     (4)
HW_ESPI_CSR_PIOMATCH_INT_BITPOS	equ     (5)
HW_ESPI_CSR_RSVD1_BITPOS 	equ     (6)
HW_ESPI_CSR_DONE_INTEN_BITPOS 	equ     (8)
HW_ESPI_CSR_PIOMATCH_INTEN_BITPOS equ     (9)
HW_ESPI_CSR_RSVD2_BITPOS 	equ     (10)
HW_ESPI_CSR_DMAWAIT_BITPOS 	equ     (12)
HW_ESPI_CSR_RSVD3_BITPOS 	equ     (16)
HW_ESPI_CSR_MPSPSEL_BITPOS 	equ     (20)
HW_ESPI_CSR_RSVD4_BITPOS 	equ     (22)
HW_ESPI_CSR_SFTRST_BITPOS 	equ     (23)

HW_ESPI_CSR_KICK_WIDTH        	equ     (1)
HW_ESPI_CSR_RSVD0_WIDTH 	equ     (3)
HW_ESPI_CSR_DONE_INT_WIDTH    	equ     (1)
HW_ESPI_CSR_PIOMATCH_INT_WIDTH	equ     (1)
HW_ESPI_CSR_RSVD1_WIDTH 	equ     (2)
HW_ESPI_CSR_DONE_INTEN_WIDTH 	equ     (1)
HW_ESPI_CSR_PIOMATCH_INTEN_WIDTH equ    (1)
HW_ESPI_CSR_RSVD2_WIDTH 	equ     (2)
HW_ESPI_CSR_DMAWAIT_WIDTH 	equ     (4)
HW_ESPI_CSR_RSVD3_WIDTH 	equ     (4)
HW_ESPI_CSR_MPSPSEL_WIDTH 	equ     (2)
HW_ESPI_CSR_RSVD4_WIDTH 	equ     (1)
HW_ESPI_CSR_SFTRST_WIDTH 	equ     (1)

HW_ESPI_CSR_KICK_SETMASK      		equ     (((1<<HW_ESPI_CSR_KICK_WIDTH)-1)<<HW_ESPI_CSR_KICK_BITPOS)
HW_ESPI_CSR_RSVD0_SETMASK      		equ     (((1<<HW_ESPI_CSR_RSVD0_WIDTH)-1)<<HW_ESPI_CSR_RSVD0_BITPOS)
HW_ESPI_CSR_DONE_INT_SETMASK   		equ     (((1<<HW_ESPI_CSR_DONE_INT_WIDTH)-1)<<HW_ESPI_CSR_DONE_INT_BITPOS)
HW_ESPI_CSR_PIOMATCH_INT_SETMASK 	equ     (((1<<HW_ESPI_CSR_PIOMATCH_INT_WIDTH)-1)<<HW_ESPI_CSR_PIOMATCH_INT_BITPOS)
HW_ESPI_CSR_RSVD1_SETMASK      		equ     (((1<<HW_ESPI_CSR_RSVD1_WIDTH)-1)<<HW_ESPI_CSR_RSVD1_BITPOS)
HW_ESPI_CSR_DONE_INTEN_SETMASK 		equ     (((1<<HW_ESPI_CSR_DONE_INTEN_WIDTH)-1)<<HW_ESPI_CSR_DONE_INTEN_BITPOS)
HW_ESPI_CSR_PIOMATCH_INTEN_SETMASK      equ     (((1<<HW_ESPI_CSR_PIOMATCH_INTEN_WIDTH)-1)<<HW_ESPI_CSR_PIOMATCH_INTEN_BITPOS)
HW_ESPI_CSR_RSVD2_SETMASK      		equ     (((1<<HW_ESPI_CSR_RSVD2_WIDTH)-1)<<HW_ESPI_CSR_RSVD2_BITPOS)
HW_ESPI_CSR_DMAWAIT_SETMASK      	equ     (((1<<HW_ESPI_CSR_DMAWAIT_WIDTH)-1)<<HW_ESPI_CSR_DMAWAIT_BITPOS)
HW_ESPI_CSR_RSVD3_SETMASK      		equ     (((1<<HW_ESPI_CSR_RSVD3_WIDTH)-1)<<HW_ESPI_CSR_RSVD3_BITPOS)
HW_ESPI_CSR_MPSPSEL_SETMASK      	equ     (((1<<HW_ESPI_CSR_MPSPSEL_WIDTH)-1)<<HW_ESPI_CSR_MPSPSEL_BITPOS)
HW_ESPI_CSR_RSVD4_SETMASK      		equ     (((1<<HW_ESPI_CSR_RSVD4_WIDTH)-1)<<HW_ESPI_CSR_RSVD4_BITPOS)
HW_ESPI_CSR_SFTRST_SETMASK      	equ     (((1<<HW_ESPI_CSR_SFTRST_WIDTH)-1)<<HW_ESPI_CSR_SFTRST_BITPOS)

HW_ESPI_CSR_KICK_CLRMASK	  	equ     (~HW_ESPI_CSR_KICK_SETMASK)
HW_ESPI_CSR_DONE_INT_CLRMASK   		equ     (~HW_ESPI_CSR_DONE_INT_SETMASK)
HW_ESPI_CSR_PIOMATCH_INT_CLRMASK 	equ     (~HW_ESPI_CSR_PIOMATCH_INT_SETMASK)
HW_ESPI_CSR_DONE_INTEN_CLRMASK 		equ     (~HW_ESPI_CSR_DONE_INTEN_SETMASK)
HW_ESPI_CSR_PIOMATCH_INTEN_CLRMASK      equ     (~HW_ESPI_CSR_PIOMATCH_INTEN_SETMASK)
HW_ESPI_CSR_DMAWAIT_CLRMASK      	equ     (~HW_ESPI_CSR_DMAWAIT_SETMASK)
HW_ESPI_CSR_MPSPSEL_CLRMASK      	equ     (~HW_ESPI_CSR_MPSPSEL_SETMASK)
HW_ESPI_CSR_SFTRST_CLRMASK      	equ     (~HW_ESPI_CSR_SFTRST_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;	int KICK             	: HW_ESPI_CSR_KICK_WIDTH;	 
;	int RSVD0             	: HW_ESPI_CSR_RSVD0_WIDTH;
;	int DONE_INT           	: HW_ESPI_CSR_DONE_INT_WIDTH;
;	int PIOMATCH_INT       	: HW_ESPI_CSR_PIOMATCH_INT_WIDTH;
;	int RSVD1             	: HW_ESPI_CSR_RSVD1_WIDTH;
;	int DONE_INTEN         	: HW_ESPI_CSR_DONE_INTEN_WIDTH;
;	int PIOMATCH_INTEN     	: HW_ESPI_CSR_PIOMATCH_INTEN_WIDTH;
;	int RSVD2             	: HW_ESPI_CSR_RSVD2_WIDTH;
;	int DMAWAIT            	: HW_ESPI_CSR_DMAWAIT_WIDTH;
;	int RSVD3             	: HW_ESPI_CSR_RSVD3_WIDTH;
;	int MPSPSEL            	: HW_ESPI_CSR_MPSPSEL_WIDTH;
;	int RSVD4             	: HW_ESPI_CSR_RSVD4_WIDTH;
;	int SFTRST             	: HW_ESPI_CSR_SFTRST_WIDTH;
;    } B;
;
;    int I;
;    unsigned int U;

;} espi_csr_type;
;#define HW_ESPI_CSR      (*(volatile espi_csr_type _X*) (HW_ESPI_BASEADDR+0))    /* ESPI Control / Status Register */
;endc

; /////////////////////////////////////////////
; //  ESPI Configuration Register 1 Bit Definitions
HW_ESPI_CNFG_ESPI_EN_BITPOS        	equ     (0)
HW_ESPI_CNFG_READ_RAM_BITPOS        	equ     (1)
HW_ESPI_CNFG_SS_BITPOS        		equ     (2)
HW_ESPI_CNFG_MSB_FIRST_BITPOS     	equ     (3)
HW_ESPI_CNFG_XFER_SIZE_BITPOS        	equ     (4)
HW_ESPI_CNFG_SPI_MODE_BITPOS        	equ     (20)

HW_ESPI_CNFG_ESPI_EN_WIDTH        	equ     (1)
HW_ESPI_CNFG_READ_RAM_WIDTH        	equ     (1)
HW_ESPI_CNFG_SS_WIDTH        		equ     (1)
HW_ESPI_CNFG_MSB_FIRST_WIDTH     	equ     (1)
HW_ESPI_CNFG_XFER_SIZE_WIDTH        	equ     (16)
HW_ESPI_CNFG_SPI_MODE_WIDTH        	equ     (4)

HW_ESPI_CNFG_ESPI_EN_SETMASK        	equ     (((1<<HW_ESPI_CNFG_ESPI_EN_WIDTH)-1)<<HW_ESPI_CNFG_ESPI_EN_BITPOS)
HW_ESPI_CNFG_READ_RAM_SETMASK        	equ     (((1<<HW_ESPI_CNFG_READ_RAM_WIDTH)-1)<<HW_ESPI_CNFG_READ_RAM_BITPOS)
HW_ESPI_CNFG_SS_SETMASK        		equ     (((1<<HW_ESPI_CNFG_SS_WIDTH)-1)<<HW_ESPI_CNFG_SS_BITPOS)
HW_ESPI_CNFG_MSB_FIRST_SETMASK     	equ     (((1<<HW_ESPI_CNFG_MSB_FIRST_WIDTH)-1)<<HW_ESPI_CNFG_MSB_FIRST_BITPOS)
HW_ESPI_CNFG_XFER_SIZE_SETMASK        	equ     (((1<<HW_ESPI_CNFG_XFER_SIZE_WIDTH)-1)<<HW_ESPI_CNFG_XFER_SIZE_BITPOS)
HW_ESPI_CNFG_SPI_MODE_SETMASK        	equ     (((1<<HW_ESPI_CNFG_SPI_MODE_WIDTH)-1)<<HW_ESPI_CNFG_SPI_MODE_BITPOS)

HW_ESPI_CNFG_ESPI_EN_CLRMASK        	equ    	(~HW_ESPI_CNFG_ESPI_EN_SETMASK) 
HW_ESPI_CNFG_READ_RAM_CLRMASK        	equ     (~HW_ESPI_CNFG_READ_RAM_SETMASK)
HW_ESPI_CNFG_SS_CLRMASK        		equ     (~HW_ESPI_CNFG_SS_SETMASK)
HW_ESPI_CNFG_MSB_FIRST_CLRMASK     	equ     (~HW_ESPI_CNFG_MSB_FIRST_SETMASK)
HW_ESPI_CNFG_XFER_SIZE_CLRMASK        	equ     (~HW_ESPI_CNFG_XFER_SIZE_SETMASK)
HW_ESPI_CNFG_SPI_MODE_CLRMASK        	equ     (~HW_ESPI_CNFG_SPI_MODE_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;	int ESPI_EN             	: HW_ESPI_CNFG_ESPI_EN_WIDTH;
;	int READ_RAM            	: HW_ESPI_CNFG_READ_RAM_WIDTH;
;	int SS		            	: HW_ESPI_CNFG_SS_WIDTH;
;	int MSB_FIRST            	: HW_ESPI_CNFG_MSB_FIRST_WIDTH;
;	int XFER_SIZE            	: HW_ESPI_CNFG_XFER_SIZE_WIDTH;
;	int SPI_MODE            	: HW_ESPI_CNFG_SPI_MODE_WIDTH;
;    } B;
;
;    int I;
;    unsigned int U;

;} espi_cnfg_type;
;#define HW_ESPI_CNFG      (*(volatile espi_cnfg_type _X*) (HW_ESPI_BASEADDR+1))    /* ESPI Config Register 1 */
;endc

; /////////////////////////////////////////////
; //  ESPI Clock Control Register Bit Definitions
HW_ESPI_CLKCNTRL_CLKPHA_BITPOS        	equ     (0)
HW_ESPI_CLKCNTRL_CLKPOL_BITPOS        	equ     (1)
HW_ESPI_CLKCNTRL_RSRVD0_BITPOS        	equ     (2)
HW_ESPI_CLKCNTRL_CLKDIV_BITPOS        	equ     (4)
HW_ESPI_CLKCNTRL_RSRVD1_BITPOS        	equ     (12)

HW_ESPI_CLKCNTRL_CLKPHA_WIDTH        	equ     (1)
HW_ESPI_CLKCNTRL_CLKPOL_WIDTH        	equ     (1)
HW_ESPI_CLKCNTRL_RSRVD0_WIDTH        	equ     (2)
HW_ESPI_CLKCNTRL_CLKDIV_WIDTH        	equ     (8)
HW_ESPI_CLKCNTRL_RSRVD1_WIDTH        	equ     (12)

HW_ESPI_CLKCNTRL_CLKPHA_SETMASK        	equ     (((1<<HW_ESPI_CLKCNTRL_CLKPHA_WIDTH)-1)<<HW_ESPI_CLKCNTRL_CLKPHA_BITPOS)
HW_ESPI_CLKCNTRL_CLKPOL_SETMASK        	equ     (((1<<HW_ESPI_CLKCNTRL_CLKPOL_WIDTH)-1)<<HW_ESPI_CLKCNTRL_CLKPOL_BITPOS)
HW_ESPI_CLKCNTRL_RSRVD0_SETMASK        	equ     (((1<<HW_ESPI_CLKCNTRL_RSRVD0_WIDTH)-1)<<HW_ESPI_CLKCNTRL_RSRVD0_BITPOS)
HW_ESPI_CLKCNTRL_CLKDIV_SETMASK        	equ     (((1<<HW_ESPI_CLKCNTRL_CLKDIV_WIDTH)-1)<<HW_ESPI_CLKCNTRL_CLKDIV_BITPOS)
HW_ESPI_CLKCNTRL_RSRVD1_SETMASK        	equ     (((1<<HW_ESPI_CLKCNTRL_RSRVD1_WIDTH)-1)<<HW_ESPI_CLKCNTRL_RSRVD1_BITPOS)

HW_ESPI_CLKCNTRL_CLKPHA_CLRMASK        	equ     (~HW_ESPI_CLKCNTRL_CLKPHA_SETMASK)
HW_ESPI_CLKCNTRL_CLKPOL_CLRMASK        	equ     (~HW_ESPI_CLKCNTRL_CLKPOL_SETMASK)
HW_ESPI_CLKCNTRL_RSRVD0_CLRMASK        	equ     (~HW_ESPI_CLKCNTRL_RSRVD0_SETMASK)
HW_ESPI_CLKCNTRL_CLKDIV_CLRMASK        	equ     (~HW_ESPI_CLKCNTRL_CLKDIV_SETMASK)
HW_ESPI_CLKCNTRL_RSRVD1_CLRMASK        	equ     (~HW_ESPI_CLKCNTRL_RSRVD1_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;	int CLKPHA             	: HW_ESPI_CLKCNTRL_CLKPHA_WIDTH;
;	int CLKPOL             	: HW_ESPI_CLKCNTRL_CLKPOL_WIDTH;
;	int RSRVD0             	: HW_ESPI_CLKCNTRL_RSRVD0_WIDTH;
;	int CLKDIV             	: HW_ESPI_CLKCNTRL_CLKDIV_BITPOS;
;	int RSRVD1             	: HW_ESPI_CLKCNTRL_RSRVD1_WIDTH;
;    } B;
;
;    int I;
;    unsigned int U;

;} espi_clkcntrl_type;
;#define HW_ESPI_CLKCNTRL      (*(volatile espi_clkcntrl_type _X*) (HW_ESPI_BASEADDR+2))    /* ESPI Clock Control Register */
;endc

; /////////////////////////////////////////////
; //  ESPI PIO Data Register Bit Definitions
HW_ESPI_PIODATA_MATCH_BITPOS        	equ     (0)
HW_ESPI_PIODATA_IN_BITPOS        	equ     (8)
HW_ESPI_PIODATA_OUT_BITPOS        	equ     (16)

HW_ESPI_PIODATA_MATCH_WIDTH        	equ     (8)
HW_ESPI_PIODATA_IN_WIDTH        	equ     (8)
HW_ESPI_PIODATA_OUT_WIDTH        	equ     (8)

HW_ESPI_PIODATA_MATCH_SETMASK        	equ     (((1<<HW_ESPI_PIODATA_MATCH_WIDTH)-1)<<HW_ESPI_PIODATA_MATCH_BITPOS)
HW_ESPI_PIODATA_IN_SETMASK        	equ     (((1<<HW_ESPI_PIODATA_IN_WIDTH)-1)<<HW_ESPI_PIODATA_IN_BITPOS)
HW_ESPI_PIODATA_OUT_SETMASK        	equ     (((1<<HW_ESPI_PIODATA_OUT_WIDTH)-1)<<HW_ESPI_PIODATA_OUT_BITPOS)

HW_ESPI_PIODATA_MATCH_CLRMASK        	equ     (~HW_ESPI_PIODATA_MATCH_SETMASK)
HW_ESPI_PIODATA_IN_CLRMASK        	equ     (~HW_ESPI_PIODATA_IN_SETMASK)
HW_ESPI_PIODATA_OUT_CLRMASK        	equ     (~HW_ESPI_PIODATA_OUT_SETMASK)
;C insert
;typedef union               
;{
;    struct {
;	int MATCH             	: HW_ESPI_PIODATA_MATCH_WIDTH;
;	int IN             	: HW_ESPI_PIODATA_IN_WIDTH;
;	int OUT             	: HW_ESPI_PIODATA_OUT_WIDTH;
;    } B;
;
;    int I;
;    unsigned int U;

;} espi_piodata_type;
;#define HW_ESPI_PIODATA      (*(volatile espi_piodata_type _X*) (HW_ESPI_BASEADDR+3))    /* ESPI Clock Control Register */
;endc

; /////////////////////////////////////////////
; //  ESPI DMA Address Register Bit Definitions
HW_ESPI_ADDR_ADDR_BITPOS        	equ     (0)
HW_ESPI_ADDR_ADDR_WIDTH        		equ     (16)
HW_ESPI_ADDR_ADDR_SETMASK        	equ     (((1<<HW_ESPI_ADDR_ADDR_WIDTH)-1)<<HW_ESPI_ADDR_ADDR_BITPOS)
HW_ESPI_ADDR_ADDR_CLRMASK        	equ     (~HW_ESPI_ADDR_ADDR_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;	int ADDR             	: HW_ESPI_ADDR_ADDR_WIDTH;
;    } B;
;
;    int I;
;    unsigned int U;

;} espi_addr_type;
;#define HW_ESPI_ADDR      (*(volatile espi_addr_type _X*) (HW_ESPI_BASEADDR+4))    /* ESPI DMA Address Register */
;endc

; /////////////////////////////////////////////
; //  ESPI DMA Index Register Bit Definitions
HW_ESPI_INDEX_BYTE_BITPOS        	equ     (0)
HW_ESPI_INDEX_RSVD0_BITPOS        	equ     (2)
HW_ESPI_INDEX_MEM_SPACE_BITPOS        	equ     (4)

HW_ESPI_INDEX_BYTE_WIDTH        	equ     (2)
HW_ESPI_INDEX_RSVD0_WIDTH        	equ     (2)
HW_ESPI_INDEX_MEM_SPACE_WIDTH        	equ     (2)

HW_ESPI_INDEX_BYTE_SETMASK        	equ     (((1<<HW_ESPI_INDEX_BYTE_WIDTH)-1)<<HW_ESPI_INDEX_BYTE_BITPOS)
HW_ESPI_INDEX_RSVD0_SETMASK        	equ     (((1<<HW_ESPI_INDEX_RSVD0_WIDTH)-1)<<HW_ESPI_INDEX_RSVD0_BITPOS)
HW_ESPI_INDEX_MEM_SPACE_SETMASK        	equ     (((1<<HW_ESPI_INDEX_MEM_SPACE_WIDTH)-1)<<HW_ESPI_INDEX_MEM_SPACE_BITPOS)

HW_ESPI_INDEX_BYTE_CLRMASK        	equ     (~HW_ESPI_INDEX_BYTE_SETMASK)
HW_ESPI_INDEX_RSVD0_CLRMASK        	equ     (~HW_ESPI_INDEX_RSVD0_SETMASK)
HW_ESPI_INDEX_MEM_SPACE_CLRMASK        	equ     (~HW_ESPI_INDEX_MEM_SPACE_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;	int OFFSET             	: HW_ESPI_INDEX_BYTE_WIDTH;
;	int RSVD0             	: HW_ESPI_INDEX_RSVD0_WIDTH;
;	int MEM_SPACE          	: HW_ESPI_INDEX_MEM_SPACE_WIDTH;
;    } B;
;
;    int I;
;    unsigned int U;

;} espi_index_type;
;#define HW_ESPI_INDEX      (*(volatile espi_index_type _X*) (HW_ESPI_BASEADDR+5))    /* ESPI DMA Index Register */
;endc

    endif

