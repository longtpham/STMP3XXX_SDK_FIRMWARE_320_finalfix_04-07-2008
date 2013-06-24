    if !(@def(__REGS_STATUS_INC))
__REGS_STATUS_INC equ 1

;Asm only
HW_PXRAM_CFG                            equ     $00FFE8         ; PXRAM Memory Config register
HW_PYRAM_CFG                            equ     $00FFE9         ; PXRAM Memory Config register
HW_ETRX_CFG                             equ     $00FFEE         ; ETRX Config register
HW_ETRY_CFG                             equ     $00FFEF         ; ETRY Config register
HW_ETR_SHIFT                            equ     $00FFF0         ; ETR Shift register
HW_ETR_RESULT                           equ     $00FFF1         ; ETR Result register
HW_RAM_ROM_CFG                          equ     $00FFED         ; RAM/ROM Config register
;endasm

;///////////////////////////////////////////////////////////////////////////////
;  OMR Register (HW_OMR) Bit Positions
HW_OMR_MA_BITPOS                        equ     0
HW_OMR_MB_BITPOS                        equ     1
HW_OMR_DE_BITPOS                        equ     2
HW_OMR_YE_BITPOS                        equ     3
HW_OMR_MC_BITPOS                        equ     4
HW_OMR_SD_BITPOS                        equ     6

HW_OMR_MA_SETMASK                       equ     1<<HW_OMR_MA_BITPOS
HW_OMR_MB_SETMASK                       equ     1<<HW_OMR_MB_BITPOS
HW_OMR_DE_SETMASK                       equ     1<<HW_OMR_DE_BITPOS
HW_OMR_YE_SETMASK                       equ     1<<HW_OMR_YE_BITPOS
HW_OMR_MC_SETMASK                       equ     1<<HW_OMR_MC_BITPOS
HW_OMR_SD_SETMASK                       equ     1<<HW_OMR_SD_BITPOS

HW_OMR_MA_CLRMASK                       equ     ~HW_OMR_MA_SETMASK
HW_OMR_MB_CLRMASK                       equ     ~HW_OMR_MB_SETMASK
HW_OMR_DE_CLRMASK                       equ     ~HW_OMR_DE_SETMASK
HW_OMR_YE_CLRMASK                       equ     ~HW_OMR_YE_SETMASK
HW_OMR_MC_CLRMASK                       equ     ~HW_OMR_MC_SETMASK
HW_OMR_SD_CLRMASK                       equ     ~HW_OMR_SD_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  Status Register (HW_SR) Bit Positions
HW_SR_C_BITPOS                          equ     0
HW_SR_O_BITPOS                          equ     1
HW_SR_Z_BITPOS                          equ     2
HW_SR_N_BITPOS                          equ     3
HW_SR_U_BITPOS                          equ     4
HW_SR_E_BITPOS                          equ     5
HW_SR_L_BITPOS                          equ     6
HW_SR_IM_BITPOS                         equ     8
HW_SR_IM0_BITPOS                        equ     8
HW_SR_IM1_BITPOS                        equ     9
HW_SR_SM_BITPOS                         equ     10
HW_SR_SM0_BITPOS                        equ     10
HW_SR_SM1_BITPOS                        equ     11
HW_SR_TM_BITPOS                         equ     13
HW_SR_DP_BITPOS                         equ     14
HW_SR_LOOP_BITPOS                       equ     15

HW_SR_C_SETMASK                         equ     1<<HW_SR_C_BITPOS
HW_SR_O_SETMASK                         equ     1<<HW_SR_O_BITPOS
HW_SR_Z_SETMASK                         equ     1<<HW_SR_Z_BITPOS
HW_SR_N_SETMASK                         equ     1<<HW_SR_N_BITPOS
HW_SR_U_SETMASK                         equ     1<<HW_SR_U_BITPOS
HW_SR_E_SETMASK                         equ     1<<HW_SR_E_BITPOS
HW_SR_L_SETMASK                         equ     1<<HW_SR_L_BITPOS
HW_SR_IM_SETMASK                        equ     3<<HW_SR_IM_BITPOS
HW_SR_IM0_SETMASK                       equ     1<<HW_SR_IM0_BITPOS
HW_SR_IM1_SETMASK                       equ     1<<HW_SR_IM1_BITPOS
HW_SR_IM_L0_SETMASK                     equ     0<<HW_SR_IM_BITPOS
HW_SR_IM_L1_SETMASK                     equ     1<<HW_SR_IM_BITPOS
HW_SR_IM_L2_SETMASK                     equ     2<<HW_SR_IM_BITPOS
HW_SR_IM_L3_SETMASK                     equ     3<<HW_SR_IM_BITPOS
HW_SR_SM_SETMASK                        equ     3<<HW_SR_SM_BITPOS
HW_SR_SM0_SETMASK                       equ     1<<HW_SR_SM0_BITPOS
HW_SR_SM1_SETMASK                       equ     1<<HW_SR_SM1_BITPOS
HW_SR_TM_SETMASK                        equ     1<<HW_SR_TM_BITPOS
HW_SR_DP_SETMASK                        equ     1<<HW_SR_DP_BITPOS
HW_SR_LOOP_SETMASK                      equ     1<<HW_SR_LOOP_BITPOS

HW_SR_C_CLRMASK                         equ     ~HW_SR_C_SETMASK
HW_SR_O_CLRMASK                         equ     ~HW_SR_O_SETMASK
HW_SR_Z_CLRMASK                         equ     ~HW_SR_Z_SETMASK
HW_SR_N_CLRMASK                         equ     ~HW_SR_N_SETMASK
HW_SR_U_CLRMASK                         equ     ~HW_SR_U_SETMASK
HW_SR_E_CLRMASK                         equ     ~HW_SR_E_SETMASK
HW_SR_L_CLRMASK                         equ     ~HW_SR_L_SETMASK
HW_SR_IM_CLRMASK                        equ     ($00FFFF)&(~HW_SR_IM_SETMASK)
HW_SR_IM0_CLRMASK                       equ     ~HW_SR_IM0_SETMASK
HW_SR_IM1_CLRMASK                       equ     ~HW_SR_IM1_SETMASK
HW_SR_SM_CLRMASK                        equ     ~HW_SR_SM_SETMASK
HW_SR_SM0_CLRMASK                       equ     ~HW_SR_SM0_SETMASK
HW_SR_SM1_CLRMASK                       equ     ~HW_SR_SM1_SETMASK
HW_SR_TM_CLRMASK                        equ     ~HW_SR_TM_SETMASK
HW_SR_DP_CLRMASK                        equ     ~HW_SR_DP_SETMASK
HW_SR_LOOP_CLRMASK                      equ     ~HW_SR_LOOP_SETMASK

;///////////////////////////////////////////////////////////////////////////////
;  RAM/ROM Config Register Bit Positions
HW_RAM_ROM_CFG_ROM_IMAGE_EN_BITPOS      equ     18
HW_RAM_ROM_CFG_ROM_CLK_EN_BITPOS        equ     19
HW_RAM_ROM_CFG_PXRAM_CLK_EN_BITPOS      equ     20
HW_RAM_ROM_CFG_PYRAM_CLK_EN_BITPOS      equ     21
    endif

