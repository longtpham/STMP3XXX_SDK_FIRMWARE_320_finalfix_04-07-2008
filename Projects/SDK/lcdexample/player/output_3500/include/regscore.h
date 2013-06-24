#if !(defined(__REGS_STATUS_INC))
#define __REGS_STATUS_INC 1


/////////////////////////////////////////////////////////////////////////////////
//  OMR Register (HW_OMR) Bit Positions
#define HW_OMR_MA_BITPOS 0
#define HW_OMR_MB_BITPOS 1
#define HW_OMR_DE_BITPOS 2
#define HW_OMR_YE_BITPOS 3
#define HW_OMR_MC_BITPOS 4
#define HW_OMR_SD_BITPOS 6

#define HW_OMR_MA_SETMASK 1<<HW_OMR_MA_BITPOS
#define HW_OMR_MB_SETMASK 1<<HW_OMR_MB_BITPOS
#define HW_OMR_DE_SETMASK 1<<HW_OMR_DE_BITPOS
#define HW_OMR_YE_SETMASK 1<<HW_OMR_YE_BITPOS
#define HW_OMR_MC_SETMASK 1<<HW_OMR_MC_BITPOS
#define HW_OMR_SD_SETMASK 1<<HW_OMR_SD_BITPOS

#define HW_OMR_MA_CLRMASK ~(WORD)HW_OMR_MA_SETMASK
#define HW_OMR_MB_CLRMASK ~(WORD)HW_OMR_MB_SETMASK
#define HW_OMR_DE_CLRMASK ~(WORD)HW_OMR_DE_SETMASK
#define HW_OMR_YE_CLRMASK ~(WORD)HW_OMR_YE_SETMASK
#define HW_OMR_MC_CLRMASK ~(WORD)HW_OMR_MC_SETMASK
#define HW_OMR_SD_CLRMASK ~(WORD)HW_OMR_SD_SETMASK


/////////////////////////////////////////////////////////////////////////////////
//  Status Register (HW_SR) Bit Positions
#define HW_SR_C_BITPOS 0
#define HW_SR_O_BITPOS 1
#define HW_SR_Z_BITPOS 2
#define HW_SR_N_BITPOS 3
#define HW_SR_U_BITPOS 4
#define HW_SR_E_BITPOS 5
#define HW_SR_L_BITPOS 6
#define HW_SR_IM_BITPOS 8
#define HW_SR_IM0_BITPOS 8
#define HW_SR_IM1_BITPOS 9
#define HW_SR_SM_BITPOS 10
#define HW_SR_SM0_BITPOS 10
#define HW_SR_SM1_BITPOS 11
#define HW_SR_TM_BITPOS 13
#define HW_SR_DP_BITPOS 14
#define HW_SR_LOOP_BITPOS 15

#define HW_SR_C_SETMASK 1<<HW_SR_C_BITPOS
#define HW_SR_O_SETMASK 1<<HW_SR_O_BITPOS
#define HW_SR_Z_SETMASK 1<<HW_SR_Z_BITPOS
#define HW_SR_N_SETMASK 1<<HW_SR_N_BITPOS
#define HW_SR_U_SETMASK 1<<HW_SR_U_BITPOS
#define HW_SR_E_SETMASK 1<<HW_SR_E_BITPOS
#define HW_SR_L_SETMASK 1<<HW_SR_L_BITPOS
#define HW_SR_IM_SETMASK 3<<HW_SR_IM_BITPOS
#define HW_SR_IM0_SETMASK 1<<HW_SR_IM0_BITPOS
#define HW_SR_IM1_SETMASK 1<<HW_SR_IM1_BITPOS
#define HW_SR_IM_L0_SETMASK 0<<HW_SR_IM_BITPOS
#define HW_SR_IM_L1_SETMASK 1<<HW_SR_IM_BITPOS
#define HW_SR_IM_L2_SETMASK 2<<HW_SR_IM_BITPOS
#define HW_SR_IM_L3_SETMASK 3<<HW_SR_IM_BITPOS
#define HW_SR_SM_SETMASK 3<<HW_SR_SM_BITPOS
#define HW_SR_SM0_SETMASK 1<<HW_SR_SM0_BITPOS
#define HW_SR_SM1_SETMASK 1<<HW_SR_SM1_BITPOS
#define HW_SR_TM_SETMASK 1<<HW_SR_TM_BITPOS
#define HW_SR_DP_SETMASK 1<<HW_SR_DP_BITPOS
#define HW_SR_LOOP_SETMASK 1<<HW_SR_LOOP_BITPOS

#define HW_SR_C_CLRMASK ~(WORD)HW_SR_C_SETMASK
#define HW_SR_O_CLRMASK ~(WORD)HW_SR_O_SETMASK
#define HW_SR_Z_CLRMASK ~(WORD)HW_SR_Z_SETMASK
#define HW_SR_N_CLRMASK ~(WORD)HW_SR_N_SETMASK
#define HW_SR_U_CLRMASK ~(WORD)HW_SR_U_SETMASK
#define HW_SR_E_CLRMASK ~(WORD)HW_SR_E_SETMASK
#define HW_SR_L_CLRMASK ~(WORD)HW_SR_L_SETMASK
#define HW_SR_IM_CLRMASK (0x00FFFF)&(~(WORD)HW_SR_IM_SETMASK)
#define HW_SR_IM0_CLRMASK ~(WORD)HW_SR_IM0_SETMASK
#define HW_SR_IM1_CLRMASK ~(WORD)HW_SR_IM1_SETMASK
#define HW_SR_SM_CLRMASK ~(WORD)HW_SR_SM_SETMASK
#define HW_SR_SM0_CLRMASK ~(WORD)HW_SR_SM0_SETMASK
#define HW_SR_SM1_CLRMASK ~(WORD)HW_SR_SM1_SETMASK
#define HW_SR_TM_CLRMASK ~(WORD)HW_SR_TM_SETMASK
#define HW_SR_DP_CLRMASK ~(WORD)HW_SR_DP_SETMASK
#define HW_SR_LOOP_CLRMASK ~(WORD)HW_SR_LOOP_SETMASK

/////////////////////////////////////////////////////////////////////////////////
//  RAM/ROM Config Register Bit Positions
#define HW_RAM_ROM_CFG_ROM_IMAGE_EN_BITPOS 18
#define HW_RAM_ROM_CFG_ROM_CLK_EN_BITPOS 19
#define HW_RAM_ROM_CFG_PXRAM_CLK_EN_BITPOS 20
#define HW_RAM_ROM_CFG_PYRAM_CLK_EN_BITPOS 21
#endif

