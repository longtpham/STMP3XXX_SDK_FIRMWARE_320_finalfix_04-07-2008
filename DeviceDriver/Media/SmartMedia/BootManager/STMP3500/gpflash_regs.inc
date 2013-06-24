;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2001
;
; Filename: gpflash_regs.inc
; Description: Register definitions for GPFLASH interface
;///////////////////////////////////////////////////////////////////////////////




HW_GPFLASH_BASEADDR                 equ     $F0C0


;///////////////////////////////////////////////////////////////////////////////

HW_GPFLASH_CSR0R                                equ     HW_GPFLASH_BASEADDR
HW_GPFLASH_CSR0_KICK_BITPOS                     equ     0
HW_GPFLASH_CSR0_RW_BITPOS           	        equ     1
HW_GPFLASH_CSR0_INT_ENA_BITPOS      	        equ     2
HW_GPFLASH_CSR0_INT_STATUS_BITPOS   	        equ     3
HW_GPFLASH_CSR0_BUSY_INT_TYPE_BITPOS            equ     4
HW_GPFLASH_CSR0_BUSY_INT_ENA_BITPOS             equ     8
HW_GPFLASH_CSR0_BUSY_TIMEOUT_INT_ENA_BITPOS     equ     9
HW_GPFLASH_CSR0_BUSY_INT_STATUS_BITPOS          equ     10
HW_GPFLASH_CSR0_SOFT_RESET_BITPOS               equ     11
HW_GPFLASH_CSR0_CEB_CTRL_BITPOS     	        equ     12
HW_GPFLASH_CSR0_CLK_DISABLE_BITPOS              equ     13
HW_GPFLASH_CSR0_XFER_TYPE_BITPOS    	        equ     16
HW_GPFLASH_CSR0_EXT_BUSY_VAL_BITPOS             equ     20


HW_GPFLASH_CSR0_READ                equ     1<<HW_GPFLASH_CSR0_RW_BITPOS
HW_GPFLASH_CSR0_WRITE               equ     0

HW_GPFLASH_CSR0_INT_ENA             equ     1<<HW_GPFLASH_CSR0_INT_ENA_BITPOS

HW_GPFLASH_CSR0_CLEAR_INT           equ     1<<HW_GPFLASH_CSR0_INT_STATUS_BITPOS

HW_GPFLASH_CSR0_CEB_CTRL_RETURN_TO_HIGH equ 1<<HW_GPFLASH_CSR0_CEB_CTRL_BITPOS

HW_GPFLASH_CSR0_CMD_XFER            equ     0
HW_GPFLASH_CSR0_ADDR_XFER           equ     1<<HW_GPFLASH_CSR0_XFER_TYPE_BITPOS
HW_GPFLASH_CSR0_DATA_XFER           equ     2<<HW_GPFLASH_CSR0_XFER_TYPE_BITPOS

HW_GPFLASH_CLOCK_ENABLE             equ     $0
HW_GPFLASH_CLOCK_DISABLE            equ     $1<<HW_GPFLASH_CSR0_CLK_DISABLE_BITPOS



;///////////////////////////////////////////////////////////////////////////////
HW_GPFLASH_CSR1R                            equ     (HW_GPFLASH_BASEADDR+1)

HW_GPFLASH_CSR1_START_BYTE_BITPOS           equ     4
HW_GPFLASH_CSR1_CEB_BITPOS                  equ     8
HW_GPFLASH_CSR1_WP0_BITPOS                  equ     12
HW_GPFLASH_CSR1_WP1_BITPOS                  equ     13
HW_GPFLASH_CSR1_MODE16_BITPOS               equ     16
HW_GPFLASH_CSR1_MUX_BITPOS                  equ     17
HW_GPFLASH_CSR1_SECTOR_ERASE_CTL_BITPOS     equ     18
HW_GPFLASH_CSR1_WP_CTL_BITPOS               equ     19


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


;///////////////////////////////////////////////////////////////////////////////
HW_GPFLASH_DMA_ADDR      equ     (HW_GPFLASH_BASEADDR+2)
; DMA Start Addr - Bits 0:15


;///////////////////////////////////////////////////////////////////////////////
HW_GPFLASH_XFER_SIZER    equ     (HW_GPFLASH_BASEADDR+3)
; Size - Bits 0:12


;///////////////////////////////////////////////////////////////////////////////
HW_GPFLASH_TIMING1R      equ     (HW_GPFLASH_BASEADDR+4)


;///////////////////////////////////////////////////////////////////////////////
HW_GPFLASH_TIMING2R      equ     (HW_GPFLASH_BASEADDR+5)


;///////////////////////////////////////////////////////////////////////////////
HW_GPFLASH_BUSYR        equ     (HW_GPFLASH_BASEADDR+6)
       


;///////////////////////////////////////////////////////////////////////////////
;///////////////////////////////////////////////////////////////////////////////
;///////////////////////////////////////////////////////////////////////////////
