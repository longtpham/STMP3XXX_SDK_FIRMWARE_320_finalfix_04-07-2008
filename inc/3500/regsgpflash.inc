;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2001
;
; Filename: regsgpflash.inc
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
        if !(@def(regsgpflashinc))
regsgpflashinc            equ         1

;C insert
;#include "types.h"
;endc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;   GPFLASH STMP Registers 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
HW_GPFLASH_BASEADDR                 equ     $F0C0

;ASM Only
HW_GPFLASH_CSR0R                    equ     HW_GPFLASH_BASEADDR
HW_GPFLASH_CSR1R                    equ     (HW_GPFLASH_BASEADDR+1)
HW_GPFLASH_DMA_ADDR                 equ     (HW_GPFLASH_BASEADDR+2)
HW_GPFLASH_XFER_SIZER               equ     (HW_GPFLASH_BASEADDR+3)
HW_GPFLASH_TIMING1R                 equ     (HW_GPFLASH_BASEADDR+4)
HW_GPFLASH_TIMING2R                 equ     (HW_GPFLASH_BASEADDR+5)
HW_GPFLASH_TIMINGBUSYR              equ     (HW_GPFLASH_BASEADDR+6)
;endasm

;///////////////////////////////////////////////////////////////////////////////
;  GPFLASH Control Status Register (HW_GPFLASH_CSR0R) Bit Definitions
HW_GPFLASH_CSR0_KICK_BITPOS                     equ     (0)
HW_GPFLASH_CSR0_RW_BITPOS           	        equ     (1)
HW_GPFLASH_CSR0_INT_ENA_BITPOS      	        equ     (2)
HW_GPFLASH_CSR0_INT_STATUS_BITPOS   	        equ     (3)
HW_GPFLASH_CSR0_BUSY_INT_TYPE_BITPOS            equ     (4)
HW_GPFLASH_CSR0_BUSY_INT_ENA_BITPOS             equ     (8)
HW_GPFLASH_CSR0_BUSY_TIMEOUT_INT_ENA_BITPOS     equ     (9)
HW_GPFLASH_CSR0_BUSY_INT_STATUS_BITPOS          equ     (10)
HW_GPFLASH_CSR0_SOFT_RESET_BITPOS               equ     (11)
HW_GPFLASH_CSR0_CEB_CTRL_BITPOS     	        equ     (12)
HW_GPFLASH_CSR0_CLK_DISABLE_BITPOS              equ     (13)
HW_GPFLASH_CSR0_XFER_TYPE_BITPOS    	        equ     (16)
HW_GPFLASH_CSR0_EXT_BUSY_VAL_BITPOS             equ     (20)

HW_GPFLASH_CSR0_KICK_WIDTH                      equ     (1)
HW_GPFLASH_CSR0_RW_WIDTH           	        equ     (1)
HW_GPFLASH_CSR0_INT_ENA_WIDTH      	        equ     (1)
HW_GPFLASH_CSR0_INT_STATUS_WIDTH   	        equ     (1)
HW_GPFLASH_CSR0_BUSY_INT_TYPE_WIDTH             equ     (4)
HW_GPFLASH_CSR0_BUSY_INT_ENA_WIDTH              equ     (1)
HW_GPFLASH_CSR0_BUSY_TIMEOUT_INT_ENA_WIDTH      equ     (1)
HW_GPFLASH_CSR0_BUSY_INT_STATUS_WIDTH           equ     (1)
HW_GPFLASH_CSR0_SOFT_RESET_WIDTH                equ     (1)
HW_GPFLASH_CSR0_CEB_CTRL_WIDTH     	        equ     (1)
HW_GPFLASH_CSR0_CLK_DISABLE_WIDTH               equ     (1)
HW_GPFLASH_CSR0_XFER_TYPE_WIDTH    	        equ     (4)
HW_GPFLASH_CSR0_EXT_BUSY_VAL_WIDTH              equ     (1)

HW_GPFLASH_CSR0_KICK_SETMASK                    equ     (((1<<HW_GPFLASH_CSR0_KICK_WIDTH)-1)<<HW_GPFLASH_CSR0_KICK_BITPOS)
HW_GPFLASH_CSR0_RW_SETMASK           	        equ     (((1<<HW_GPFLASH_CSR0_RW_WIDTH)-1)<<HW_GPFLASH_CSR0_RW_BITPOS)
HW_GPFLASH_CSR0_INT_ENA_SETMASK      	        equ     (((1<<HW_GPFLASH_CSR0_INT_ENA_WIDTH)-1)<<HW_GPFLASH_CSR0_INT_ENA_BITPOS)
HW_GPFLASH_CSR0_INT_STATUS_SETMASK   	        equ     (((1<<HW_GPFLASH_CSR0_INT_STATUS_WIDTH)-1)<<HW_GPFLASH_CSR0_INT_STATUS_BITPOS)
HW_GPFLASH_CSR0_BUSY_INT_TYPE_SETMASK           equ     (((1<<HW_GPFLASH_CSR0_BUSY_INT_TYPE_WIDTH)-1)<<HW_GPFLASH_CSR0_BUSY_INT_TYPE_BITPOS)
HW_GPFLASH_CSR0_BUSY_INT_ENA_SETMASK            equ     (((1<<HW_GPFLASH_CSR0_BUSY_INT_ENA_WIDTH)-1)<<HW_GPFLASH_CSR0_BUSY_INT_ENA_BITPOS)
HW_GPFLASH_CSR0_BUSY_TIMEOUT_INT_ENA_SETMASK    equ     (((1<<HW_GPFLASH_CSR0_BUSY_TIMEOUT_INT_ENA_WIDTH)-1)<<HW_GPFLASH_CSR0_BUSY_TIMEOUT_INT_ENA_BITPOS)
HW_GPFLASH_CSR0_BUSY_INT_STATUS_SETMASK         equ     (((1<<HW_GPFLASH_CSR0_BUSY_INT_STATUS_WIDTH)-1)<<HW_GPFLASH_CSR0_BUSY_INT_STATUS_BITPOS)
HW_GPFLASH_CSR0_SOFT_RESET_SETMASK              equ     (((1<<HW_GPFLASH_CSR0_SOFT_RESET_WIDTH)-1)<<HW_GPFLASH_CSR0_SOFT_RESET_BITPOS)
HW_GPFLASH_CSR0_CEB_CTRL_SETMASK     	        equ     (((1<<HW_GPFLASH_CSR0_CEB_CTRL_WIDTH)-1)<<HW_GPFLASH_CSR0_CEB_CTRL_BITPOS)
HW_GPFLASH_CSR0_CLK_DISABLE_SETMASK             equ     (((1<<HW_GPFLASH_CSR0_CLK_DISABLE_WIDTH)-1)<<HW_GPFLASH_CSR0_CLK_DISABLE_BITPOS)
HW_GPFLASH_CSR0_XFER_TYPE_SETMASK    	        equ     (((1<<HW_GPFLASH_CSR0_XFER_TYPE_WIDTH)-1)<<HW_GPFLASH_CSR0_XFER_TYPE_BITPOS)
HW_GPFLASH_CSR0_EXT_BUSY_VAL_SETMASK            equ     (((1<<HW_GPFLASH_CSR0_EXT_BUSY_VAL_WIDTH)-1)<<HW_GPFLASH_CSR0_EXT_BUSY_VAL_BITPOS)

HW_GPFLASH_CSR0_KICK_CLRMASK                    equ     (~HW_GPFLASH_CSR0_KICK_SETMASK)
HW_GPFLASH_CSR0_RW_CLRMASK           	        equ     (~HW_GPFLASH_CSR0_RW_SETMASK)
HW_GPFLASH_CSR0_INT_ENA_CLRMASK      	        equ     (~HW_GPFLASH_CSR0_INT_ENA_SETMASK)
HW_GPFLASH_CSR0_INT_STATUS_CLRMASK   	        equ     (~HW_GPFLASH_CSR0_INT_STATUS_SETMASK)
HW_GPFLASH_CSR0_BUSY_INT_TYPE_CLRMASK           equ     (~HW_GPFLASH_CSR0_BUSY_INT_TYPE_SETMASK)
HW_GPFLASH_CSR0_BUSY_INT_ENA_CLRMASK            equ     (~HW_GPFLASH_CSR0_BUSY_INT_ENA_SETMASK)
HW_GPFLASH_CSR0_BUSY_TIMEOUT_INT_ENA_CLRMASK    equ     (~HW_GPFLASH_CSR0_BUSY_TIMEOUT_INT_ENA_SETMASK)
HW_GPFLASH_CSR0_BUSY_INT_STATUS_CLRMASK         equ     (~HW_GPFLASH_CSR0_BUSY_INT_STATUS_SETMASK)
HW_GPFLASH_CSR0_SOFT_RESET_CLRMASK              equ     (~HW_GPFLASH_CSR0_SOFT_RESET_SETMASK)
HW_GPFLASH_CSR0_CEB_CTRL_CLRMASK     	        equ     (~HW_GPFLASH_CSR0_CEB_CTRL_SETMASK)
HW_GPFLASH_CSR0_CLK_DISABLE_CLRMASK             equ     (~HW_GPFLASH_CSR0_CLK_DISABLE_SETMASK)
HW_GPFLASH_CSR0_XFER_TYPE_CLRMASK    	        equ     (~HW_GPFLASH_CSR0_XFER_TYPE_SETMASK)
HW_GPFLASH_CSR0_EXT_BUSY_VAL_CLRMASK            equ     (~HW_GPFLASH_CSR0_EXT_BUSY_VAL_SETMASK)

;Asm Only
HW_GPFLASH_CSR0_READ                equ     1<<HW_GPFLASH_CSR0_RW_BITPOS
HW_GPFLASH_CSR0_WRITE               equ     0
HW_GPFLASH_CSR0_CLEAR_INT           equ     1<<HW_GPFLASH_CSR0_INT_STATUS_BITPOS
HW_GPFLASH_CSR0_CEB_CTRL_RETURN_TO_HIGH equ 1<<HW_GPFLASH_CSR0_CEB_CTRL_BITPOS
HW_GPFLASH_CSR0_CMD_XFER            equ     0
HW_GPFLASH_CSR0_ADDR_XFER           equ     1<<HW_GPFLASH_CSR0_XFER_TYPE_BITPOS
HW_GPFLASH_CSR0_DATA_XFER           equ     2<<HW_GPFLASH_CSR0_XFER_TYPE_BITPOS
HW_GPFLASH_CLOCK_ENABLE             equ     $0
HW_GPFLASH_CLOCK_DISABLE            equ     $1<<HW_GPFLASH_CSR0_CLK_DISABLE_BITPOS
;endasm

;C insert
;typedef union               
;{
;    struct {
;        unsigned int KICK                 :1;
;        unsigned int RW                   :1;
;        unsigned int INT_XFERDONE_ENA     :1;
;        unsigned int INT_XFERDONE_STATUS  :1;
;        unsigned int BUSY_INT_TYPE        :2;
;        unsigned int BUSY_TIMEOUT_EDGE    :2;
;        unsigned int BUSY_INT_ENA         :1;
;        unsigned int BUSY_TIMEOUT_INT_ENA :1;
;	 unsigned int BUSY_INT_STATUS	  :1;
;        unsigned int SOFT_RESET           :1;
;        unsigned int CEB_CTRL             	:1;
;        unsigned int CLK_DISABLE          	:1;
;	 unsigned int BUSY_TIMEOUT_INT_STAT 	:1;
;	 unsigned int 				:1;
;        unsigned int XFER_TYPE            :2;
;        unsigned int SPARE                :2;
;	 unsigned int EXT_BUSY_VAL		:1;
;	 unsigned int 				:3;
;    } B;
;    int I;
;} gpflashcsr0_type;

;#define HW_GPFLASH_CSR0R     (*(volatile gpflashcsr0_type _X*) (HW_GPFLASH_BASEADDR))    /* GPFLASH Control 0 / Status Register */
;endc

;///////////////////////////////////////////////////////////////////////////////
;  GPFLASH Control Status Register (HW_GPFLASH_CSR1R) Bit Definitions
HW_GPFLASH_CSR1_MEM_BITPOS                  equ     (0)
HW_GPFLASH_CSR1_START_BYTE_BITPOS           equ     (4)
HW_GPFLASH_CSR1_CEB_BITPOS                  equ     (8)
HW_GPFLASH_CSR1_WP0_BITPOS                  equ     (12)
HW_GPFLASH_CSR1_WP1_BITPOS                  equ     (13)
HW_GPFLASH_CSR1_MODE16_BITPOS               equ     (16)
HW_GPFLASH_CSR1_MUX_BITPOS                  equ     (17)
HW_GPFLASH_CSR1_SECTOR_ERASE_CTL_BITPOS     equ     (18)
HW_GPFLASH_CSR1_WP_CTL_BITPOS               equ     (19)

HW_GPFLASH_CSR1_MEM_WIDTH                  equ     (2)
HW_GPFLASH_CSR1_START_BYTE_WIDTH           equ     (2)
HW_GPFLASH_CSR1_CEB_WIDTH                  equ     (2)
HW_GPFLASH_CSR1_WP0_WIDTH                  equ     (1)
HW_GPFLASH_CSR1_WP1_WIDTH                  equ     (1)
HW_GPFLASH_CSR1_MODE16_WIDTH               equ     (1)
HW_GPFLASH_CSR1_MUX_WIDTH                  equ     (1)
HW_GPFLASH_CSR1_SECTOR_ERASE_CTL_WIDTH     equ     (1)
HW_GPFLASH_CSR1_WP_CTL_WIDTH               equ     (1)

HW_GPFLASH_CSR1_MEM_SETMASK                  equ   (((1<<HW_GPFLASH_CSR1_MEM_WIDTH)-1)<<HW_GPFLASH_CSR1_MEM_BITPOS)
HW_GPFLASH_CSR1_START_BYTE_SETMASK           equ   (((1<<HW_GPFLASH_CSR1_START_BYTE_WIDTH)-1)<<HW_GPFLASH_CSR1_START_BYTE_BITPOS)
HW_GPFLASH_CSR1_CEB_SETMASK                  equ   (((1<<HW_GPFLASH_CSR1_CEB_WIDTH)-1)<<HW_GPFLASH_CSR1_CEB_BITPOS)
HW_GPFLASH_CSR1_WP0_SETMASK                  equ   (((1<<HW_GPFLASH_CSR1_WP0_WIDTH)-1)<<HW_GPFLASH_CSR1_WP0_BITPOS)
HW_GPFLASH_CSR1_WP1_SETMASK                  equ   (((1<<HW_GPFLASH_CSR1_WP1_WIDTH)-1)<<HW_GPFLASH_CSR1_WP1_BITPOS)
HW_GPFLASH_CSR1_MODE16_SETMASK               equ   (((1<<HW_GPFLASH_CSR1_MODE16_WIDTH)-1)<<HW_GPFLASH_CSR1_MODE16_BITPOS)
HW_GPFLASH_CSR1_MUX_SETMASK                  equ   (((1<<HW_GPFLASH_CSR1_MUX_WIDTH)-1)<<HW_GPFLASH_CSR1_MUX_BITPOS)
HW_GPFLASH_CSR1_SECTOR_ERASE_CTL_SETMASK     equ   (((1<<HW_GPFLASH_CSR1_SECTOR_ERASE_CTL_WIDTH)-1)<<HW_GPFLASH_CSR1_SECTOR_ERASE_CTL_BITPOS)
HW_GPFLASH_CSR1_WP_CTL_SETMASK               equ   (((1<<HW_GPFLASH_CSR1_WP_CTL_WIDTH)-1)<<HW_GPFLASH_CSR1_WP_CTL_BITPOS)

HW_GPFLASH_CSR1_MEM_CLRMASK                  equ (~HW_GPFLASH_CSR1_MEM_SETMASK)
HW_GPFLASH_CSR1_START_BYTE_CLRMASK           equ (~HW_GPFLASH_CSR1_START_BYTE_SETMASK)
HW_GPFLASH_CSR1_CEB_CLRMASK                  equ (~HW_GPFLASH_CSR1_CEB_SETMASK)
HW_GPFLASH_CSR1_WP0_CLRMASK                  equ (~HW_GPFLASH_CSR1_WP0_SETMASK)
HW_GPFLASH_CSR1_WP1_CLRMASK                  equ (~HW_GPFLASH_CSR1_WP1_SETMASK)
HW_GPFLASH_CSR1_MODE16_CLRMASK               equ (~HW_GPFLASH_CSR1_MODE16_SETMASK)
HW_GPFLASH_CSR1_MUX_CLRMASK                  equ (~HW_GPFLASH_CSR1_MUX_SETMASK)
HW_GPFLASH_CSR1_SECTOR_ERASE_CTL_CLRMASK     equ (~HW_GPFLASH_CSR1_SECTOR_ERASE_CTL_SETMASK)
HW_GPFLASH_CSR1_WP_CTL_CLRMASK               equ (~HW_GPFLASH_CSR1_WP_CTL_SETMASK)

;ASM Only
HW_GPFLASH_USE_X_MEMORY             equ     $0
HW_GPFLASH_USE_Y_MEMORY             equ     $1
HW_GPFLASH_USE_P_MEMORY             equ     $2

HW_GPFLASH_8BIT_ACCESS              equ     $0
HW_GPFLASH_16BIT_ACCESS             equ     $1<<HW_GPFLASH_CSR1_MODE16_BITPOS

HW_GPFLASH_START_BYTE_LSB           equ     $0
HW_GPFLASH_START_BYTE_ISB           equ     $1<<HW_GPFLASH_CSR1_START_BYTE_BITPOS
HW_GPFLASH_START_BYTE_MSB           equ     $2<<HW_GPFLASH_CSR1_START_BYTE_BITPOS

;  Use the following mask to clear all bits but the WPn ones
;  This will typically be used to protect the WPn bits from change when changing the rest of CSR1 
HW_GPFLASH_WPn_MASK                 equ     ($1<<HW_GPFLASH_CSR1_WP0_BITPOS)+($1<<HW_GPFLASH_CSR1_WP1_BITPOS)+($1<<HW_GPFLASH_CSR1_WP_CTL_BITPOS)


HW_GPFLASH_MUX_EMC_ACCESS           equ     $0
HW_GPFLASH_MUX_GPFLASH_ACCESS       equ     $1<<HW_GPFLASH_CSR1_MUX_BITPOS
;endasm

;C insert
;typedef union               
;{
;    struct {
;        int MEM                  :2;
;        int RSVD0                :2;
;        int START_BYTE           :2;
;        int RSVD1                :2;
;        int CEB                  :2;
;        int RSVD2                :2;
;        int WP0                  :1;
;        int WP1                  :1;
;        int RSVD3                :2;
;        int MODE16               :1;
;        int MUX                  :1;
;        int SECTOR_ERASE_CTL     :1;
;        int WP_CTL               :1;
;        int RSVD4                :4;
;    } B;
;    int I;
;} gpflashcsr1_type;

;#define HW_GPFLASH_CSR1R     (*(volatile gpflashcsr1_type _X*) (HW_GPFLASH_BASEADDR+1))    /* GPFLASH Control 1 / Status Register */
;endc

;///////////////////////////////////////////////////////////////////////////////
; GPFLASH DMA ADDRESS register (HW_GPFLASH_DMA_ADDR) Bit Definitions
HW_GPFLASH_DMA_ADD_SETMASK            equ  ($00FFFF)

HW_GPFLASH_DMA_ADD_CLRMASK            equ  (~HW_GPFLASH_DMA_ADD_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int ADD            :16;
;        int RSVD           :8;
;    } B;
;    int I;
;} gpflashdmaadd_type;

;#define HW_GPFLASH_DMA_ADDR      (*(volatile gpflashdmaadd_type _X*) (HW_GPFLASH_BASEADDR+2))    
;endc

;///////////////////////////////////////////////////////////////////////////////
; GPFLASH XFER SIZE register (HW_GPFLASH_XFER_SIZE) Bit Definitions
HW_GPFLASH_XFER_SIZER_SETMASK          equ  ($001FFF)

HW_GPFLASH_XFER_SIZER_CLRMASK          equ  (~HW_GPFLASH_XFER_SIZER_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        unsigned NUM_BYTES          :13;
;        int RSVD          :11;
;    } B;
;    int I;
;} gpflashxfersizer_type;

;#define HW_GPFLASH_XFER_SIZER      (*(volatile gpflashxfersizer_type _X*) (HW_GPFLASH_BASEADDR+3))    
;endc

;///////////////////////////////////////////////////////////////////////////////
; GPFLASH TIMING 1 register (HW_GPFLASH_TIMING1) Bit Definitions
HW_GPFLASH_TSU_BITPOS                equ  (0)
HW_GPFLASH_THOLD_BITPOS              equ  (8)

HW_GPFLASH_TSU_WIDTH                 equ  (5)
HW_GPFLASH_THOLD_WIDTH               equ  (5)

HW_GPFLASH_TSU_SETMASK               equ  (((1<<HW_GPFLASH_TSU_WIDTH)-1)<<HW_GPFLASH_TSU_BITPOS)
HW_GPFLASH_THOLD_SETMASK             equ  (((1<<HW_GPFLASH_THOLD_WIDTH)-1)<<HW_GPFLASH_THOLD_BITPOS)

HW_GPFLASH_TSU_CLRMASK               equ (~HW_GPFLASH_TSU_SETMASK)
HW_GPFLASH_THOLD_CLRMASK             equ (~HW_GPFLASH_THOLD_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int TSU          :5;
;        int RSVD0          :3;       
;        int THOLD           :5;
;        int RSVD1          :11;
;    } B;
;    int I;
;} gpflashtiming1_type;

;#define HW_GPFLASH_TIMING1      (*(volatile gpflashtiming1_type _X*) (HW_GPFLASH_BASEADDR+4))    
;endc

;///////////////////////////////////////////////////////////////////////////////
; GPFLASH TIMING 2 register (HW_GPFLASH_TIMING2) Bit Definitions
HW_GPFLASH_TDS_BITPOS                 equ  (0)
HW_GPFLASH_TDH_BITPOS                 equ  (8)

HW_GPFLASH_TDS_WIDTH                  equ  (6)
HW_GPFLASH_TDH_WIDTH                  equ  (6)

HW_GPFLASH_TDS_SETMASK                equ  (((1<<HW_GPFLASH_TDS_WIDTH)-1)<<HW_GPFLASH_TDS_BITPOS)
HW_GPFLASH_TDH_SETMASK                equ  (((1<<HW_GPFLASH_TDH_WIDTH)-1)<<HW_GPFLASH_TDH_BITPOS)

HW_GPFLASH_TDS_CLRMASK                equ (~HW_GPFLASH_TDS_SETMASK)
HW_GPFLASH_TDH_CLRMASK                equ (~HW_GPFLASH_TDH_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int TDS           :6;
;        int RSVD0          :2;       
;        int TDH           :6;
;        int RSVD1          :10;
;    } B;
;    int I;
;} gpflashtiming2_type;

;#define HW_GPFLASH_TIMING2      (*(volatile gpflashtiming2_type _X*) (HW_GPFLASH_BASEADDR+5))    
;endc

;///////////////////////////////////////////////////////////////////////////////
; GPFLASH TIMING 2 register (HW_GPFLASH_TIMINGBUSY) Bit Definitions
HW_GPFLASH_TWTO_SETMASK                equ  ($01FFFF)

HW_GPFLASH_TWTO_CLRMASK                equ (~HW_GPFLASH_TWTO_SETMASK)


;C insert
;typedef union               
;{
;    struct {
;        int TWTO           :18;
;        int RSVD           :6;       
;    } B;
;    int I;
;} gpflashtimingbusy_type;

;#define HW_GPFLASH_TIMINGBUSY      (*(volatile gpflashtimingbusy _X*) (HW_GPFLASH_BASEADDR+6))    
;endc
   endif