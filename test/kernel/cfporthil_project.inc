;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2003
;
;  Filename: cfporthil_project.inc
;  Description: 
;///////////////////////////////////////////////////////////////////////////////

    if (!@def(_CFPORTHIL_PROJECT_INC))
_CFPORTHIL_PROJECT_INC    equ     1

    include "regsgpio.inc"


;///////////////////////////////////////////////////////////////////////////////
; PORTHIL GPIO DEFINITIONS
;///////////////////////////////////////////////////////////////////////////////

; Input/Output Equates
CF_GPIO_IO_DATA_REG         equ    HW_GP1DOR
CF_GPIO_IO_DIR_REG          equ    HW_GP1DOER
CF_GPIO_IO_DATA_SETMASK     equ    $FF
CF_GPIO_IO_DATA_CLEARMASK   equ    $FFFF00

; NAND DATA INPUT/OUTPUTS (D0..D7) --> 8 pins to setup (8BIT NAND)
CF_GPIO_DATAPINS_DIR_REG    equ    HW_GP1DOER
CF_GPIO_DATAPINS_ENBL_REG   equ    HW_GP1ENR
CF_GPIO_DATAPINS_OUT_WORD   equ    $FF
CF_GPIO_DATAPINS_IN_WORD    equ    $FFFF00
CF_GPIO_DATAPINS_ENABLE     equ    $FF
CF_GPIO_DATAPINS_DISABLE    equ    $FFFF00

; NAND DATA INPUT/OUTPUTS (D8..D15) --> 8 extra pins to setup (16BIT NAND)
CF_GPIO_DATAPINS16_DIR_REG    equ    HW_GP3DOER
CF_GPIO_DATAPINS16_ENBL_REG   equ    HW_GP3ENR
CF_GPIO_DATAPINS16_OUT_WORD   equ    $FF
CF_GPIO_DATAPINS16_IN_WORD    equ    $FFFF00
CF_GPIO_DATAPINS16_ENABLE     equ    $FF
CF_GPIO_DATAPINS16_DISABLE    equ    $FFFF00

; NAND ADDRESS LATCH ENABLE Equates
CF_GPIO_ALE_DATA_REG        equ    HW_GP1DOR
CF_GPIO_ALE_DIR_REG         equ    HW_GP1DOER
CF_GPIO_ALE_ENBL_REG        equ    HW_GP1ENR
CF_PORT_ALE_MASK            equ    $20000
CF_GPIO_ALE_BITPOS          equ    17

; NAND COMMAND LATCH ENABLE Equates
CF_GPIO_CLE_DATA_REG        equ    HW_GP1DOR
CF_GPIO_CLE_DIR_REG         equ    HW_GP1DOER
CF_GPIO_CLE_ENBL_REG        equ    HW_GP1ENR
CF_PORT_CLE_MASK            equ    $10000
CF_GPIO_CLE_BITPOS          equ    16

; NAND READ ENABLE Equates
CF_GPIO_REN_DATA_REG        equ    HW_GP2DOR
CF_GPIO_REN_DIR_REG         equ    HW_GP2DOER
CF_GPIO_REN_ENBL_REG        equ    HW_GP2ENR
CF_GPIO_REN_BITPOS          equ    5

; NAND WRITE ENABLE Equates
CF_GPIO_WEN_DATA_REG        equ    HW_GP2DOR
CF_GPIO_WEN_DIR_REG         equ    HW_GP2DOER
CF_GPIO_WEN_ENBL_REG        equ    HW_GP2ENR
CF_GPIO_WEN_BITPOS          equ    6

; NAND CHIP ENABLE 0 (device number 0) Equates
CF_GPIO_CE0_BITPOS          equ    14
CF_GPIO_CE0_DATA_REG        equ    HW_GP1DOR
CF_GPIO_CE0_DIR_REG         equ    HW_GP1DOER
CF_GPIO_CE0_ENBL_REG        equ    HW_GP1ENR

; NAND CHIP ENABLE 1 (device number 1) Equates
CF_GPIO_CE1_BITPOS          equ    21
CF_GPIO_CE1_DATA_REG        equ    HW_GP1DOR
CF_GPIO_CE1_DIR_REG         equ    HW_GP1DOER
CF_GPIO_CE1_ENBL_REG        equ    HW_GP1ENR

; NAND CHIP ENABLE 2 (device number 2) Equates
CF_GPIO_CE2_BITPOS          equ    13
CF_GPIO_CE2_DATA_REG        equ    HW_GP1DOR
CF_GPIO_CE2_DIR_REG         equ    HW_GP1DOER
CF_GPIO_CE2_ENBL_REG        equ    HW_GP1ENR

; NAND CHIP ENABLE 3 (device number 3) Equates
CF_GPIO_CE3_BITPOS          equ    12
CF_GPIO_CE3_DATA_REG        equ    HW_GP1DOR
CF_GPIO_CE3_DIR_REG         equ    HW_GP1DOER
CF_GPIO_CE3_ENBL_REG        equ    HW_GP1ENR

; NAND READY/BUSY OUTPUT Equates
CF_GPIO_READY_BITPOS        equ      8
CF_GPIO_READY_DIR_REG       equ      HW_GP2DOER
CF_GPIO_READY_ENBL_REG      equ      HW_GP2ENR
CF_GPIO_READY_DIN_REG       equ      HW_GP2DIR

; NAND WRITE PROTECT Equates
CF_GPIO_WP0_BITPOS          equ    7
CF_GPIO_WP0_DATA_REG        equ    HW_GP2DOR
CF_GPIO_WP0_DIR_REG         equ    HW_GP2DOER
CF_GPIO_WP0_ENBL_REG        equ    HW_GP2ENR

    endif ; if (!@def(CFPORTHIL_PROJECT_INC))
