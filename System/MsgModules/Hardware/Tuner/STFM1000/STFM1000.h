////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2005
//
// Filename: STFM1000.h
// Description: 
////////////////////////////////////////////////////////////////////////////////

#ifndef _STFM_1000_H
#define _STFM_1000_H
//////////////////////////////////////////////////////////////////////////////
//!   Includes
//////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//! Definitions
////////////////////////////////////////////////////////////////////////////////

/**********************************************************************/

//! STFM1000 Register Addresses
// as of 2005/07/28
#define TUNE1_REG_ADDR                      0x00
#define SDNOMINAL_REG_ADDR                  0x04
#define PILOTTRACKING_REG_ADDR              0x08
#define STFM1000_UNUSED_REG_0C              0x0C
#define INITIALIZATION1_REG_ADDR            0x10
#define INITIALIZATION2_REG_ADDR            0x14
#define INITIALIZATION3_REG_ADDR            0x18
#define INITIALIZATION4_REG_ADDR            0x1C
#define INITIALIZATION5_REG_ADDR            0x20
#define INITIALIZATION6_REG_ADDR            0x24
#define REF_REG_ADDR                        0x28
#define LNA_REG_ADDR                        0x2C
#define MIXFILT_REG_ADDR                    0x30
#define CLK1_REG_ADDR                       0x34
#define CLK2_REG_ADDR                       0x38
#define ADC_REG_ADDR                        0x3C
#define STFM1000_UNUSED_REG_40              0x40
#define AGC_CONTROL1_REG_ADDR               0x44
#define AGC_CONTROL2_REG_ADDR               0x48
#define STFM1000_UNUSED_REG_4C              0x4C
#define STFM1000_UNUSED_REG_50              0x50
#define STFM1000_UNUSED_REG_54              0x54
#define STFM1000_UNUSED_REG_58              0x58
#define DATAPATH_REG_ADDR                   0x5C
#define RMS_REG_ADDR                        0x60
#define AGC_STAT_REG_ADDR                   0x64
#define SIGNALQUALITY_REG_ADDR              0x68
#define DCEST_REG_ADDR                      0x6C
#define RSSI_TONE_REG_ADDR                  0x70
#define PILOTCORRECTION_REG_ADDR            0x74
#define ATTENTION_REG_ADDR                  0x78
#define CL3_REG_ADDR                        0x7C
#define CHIPID_REG_ADDR                     0x80


//! STFM1000 Register Fields
// as of 2005/07/28

// ******* Generic Definitions ***********************
#define REGISTER_BYTES                      4

#define WHOLEREG_MASK                       0xFFFFFFFF
#define BYTE3_MASK                          0xFF000000
#define BYTE2_MASK                          0x00FF0000
#define BYTE1_MASK                          0x0000FF00
#define BYTE0_MASK                          0x000000FF
// ***************************************************


// #define TUNE1_REG_ADDR                      0x00
// ***************************************************
#define TUNE1_B2_MIX_REG_MASK               0x001C0000
#define TUNE1_CICOSR_REG_MASK               0x00007E00
#define TUNE1_PLL_DIV_REG_MASK              0x000001FF
// ***************************************************
#define TUNE1_STARTUP_REG              0
#define TUNE1_B2_MIX_REG               1             // IF = -100 kHz
// b010110100 -- For Ftune=87.5MHz, we want Ffe=875-1, thus Ndiv = 11, Incr = 4
#define TUNE1_PLL_DIV_REG              0xB4
#define TUNE1_CICOSR_REG               36

// #define SDNOMINAL_REG_ADDR                  0x04
// ***************************************************
#define SDNOMINAL_REG_MASK                  0xFFFFFFFF
// ***************************************************
// Based on tuning param's above
#define SDNOMINAL_REG                  476390083

// #define PILOTTRACKING_REG_ADDR              0x08
// ***************************************************
#define B2_PILOTTRACKING_ENABLE_REG_MASK    0x00008000
#define B2_PILOTLPF_TIMECONSTANT_REG_MASK   0x00000F00
#define B2_PFDSCALE_REG_MASK                0x000000F0
#define B2_PFDFILTER_SPEEDUP_REG_MASK       0x0000000F
// ***************************************************
 // start with pilottracking OFF
#define B2_PILOTTRACKING_ENABLE_REG    0
#define B2_PILOTLPF_TIMECONSTANT_REG   1
#define B2_PFDSCALE_REG                12
#define B2_PFDFILTER_SPEEDUP_REG       6


// #define STFM1000_UNUSED_REG_0C              0x0C
// ***************************************************
// ***************************************************

// #define INITIALIZATION1_REG_ADDR            0x10
// ***************************************************
#define SCALE_DEMOD_REG_MASK                0xE0000000
#define ATTENTION_DISABLE_REG_MASK          0x1F800000
#define BYPASS_DCFILT_REG_MASK              0x00700000
#define DCEST_EN_REG_MASK                   0x00080000
#define GAIN_MATCH_REG_MASK                 0x0007F000
#define IQSWAP_REG_MASK                     0x00000800
#define PHASE_MATCH_REG_MASK                0x000007F0
#define B2_BYPASS_FILT_REG_MASK             0x0000000C
#define DMD_BYPASS_REG_MASK                 0x00000003
// ***************************************************
#define SCALE_DEMOD_REG                1
#define ATTENTION_DISABLE_REG          0
#define BYPASS_DCFILT_REG              3
#define DCEST_EN_REG                   1
#define GAIN_MATCH_REG                 0
#define IQSWAP_REG                     0
#define PHASE_MATCH_REG                0
#define B2_BYPASS_FILT_REG             2
#define DMD_BYPASS_REG                 0


// #define INITIALIZATION2_REG_ADDR            0x14
// ***************************************************
#define DRI_CLK_EN_REG_MASK                 0x80000000
#define DISBALE_IQMATCH_REG_MASK            0x20000000
#define DISABLE_GAINMATCH_REG_MASK          0x10000000
#define VIEW_TONEDATA_REG_MASK              0x08000000
#define PILOTCORRECTION_SMOOTHING_REG_MASK  0x04000000
#define PILOTEST_MIXOFFSET_REG_MASK         0x02000000
#define CCT_ENABLE_REG_MASK                 0x01000000
#define TIMECONSTANT_REG_MASK               0x000C0000
#define RDS_MIXOFFSET_REG_MASK              0x00020000
#define PILOTOFFSET_LMR_REG_MASK            0x0001E000
#define PILOTPEAKOFFSET_REG_MASK            0x00001E00
#define DEEMPH_50_75B_REG_MASK              0x00000100
#define K2_REG_MASK                         0x000000F0
#define K1_REG_MASK                         0x0000000F
// ***************************************************
#define DRI_CLK_EN_REG                 1
#define DISBALE_IQMATCH_REG            0
#define DISABLE_GAINMATCH_REG          0
#define VIEW_TONEDATA_REG              0
#define PILOTCORRECTION_SMOOTHING_REG  1              // Turn smoothing ON
#define PILOTEST_MIXOFFSET_REG         0
#define CCT_ENABLE_REG                 1
#define TIMECONSTANT_REG               1
#define RDS_MIXOFFSET_REG              1
#define PILOTOFFSET_LMR_REG            7
#define PILOTPEAKOFFSET_REG            2
#define DEEMPH_50_75B_REG              0              // 75 us
#define K2_REG                         4
#define K1_REG                         4

// #define INITIALIZATION3_REG_ADDR            0x18
// ***************************************************
#define DRI_TEST_MODE_REG_MASK              0xE0000000
#define B2_NEAR_CHAN_MIX_REG_MASK           0x1C000000
#define PILOTCORRECTION_LIMIT_REG_MASK      0x03FC0000
#define BYPASS_XTALLOCK_REG_MASK            0x00020000
#define SEL_441B_48_REG_MASK                0x00010000
#define PFD_DECRATE_REG_MASK                0x0000FFC0
#define BYPASS_COMPEN_REG_MASK              0x00000020
#define B2_I2PQ2_SCALE_REG_MASK             0x0000001F
// ***************************************************
#define DRI_TEST_MODE_REG              0              // Normal mode
// monitor the strength of the received channel
#define B2_NEAR_CHAN_MIX_REG           7
#define PILOTCORRECTION_LIMIT_REG      0
// start in pilot-lock mode, then switch later to xtal-lock once Fs is stable
#define BYPASS_XTALLOCK_REG            1
#define SEL_441B_48_REG                0
#define PFD_DECRATE_REG                100
#define BYPASS_COMPEN_REG              0
#define B2_I2PQ2_SCALE_REG             13

// #define INITIALIZATION4_REG_ADDR            0x1C
// ***************************************************
#define SAMPLE_SKIP_REG_MASK                0x7E000000
#define POSITION_0_255_REG_MASK             0x01FFFFFF
// ***************************************************
#define SAMPLE_SKIP_REG                41             // For 44.1 kHz
// For 44.1 kHz, (int)(91.93875*65536)
#define POSITION_0_255_REG             6025298

// #define INITIALIZATION5_REG_ADDR            0x20
// ***************************************************
#define FLAG_RDS_REG_MASK                   0xE0000000
#define NUM_SLOTS_REG_MASK                  0x1C000000
#define RDS_SLOT_POS_REG_MASK               0x0001C000
#define RSSI_SLOT_POS_REG_MASK              0x00003800
#define LMR_SLOT_POS_REG_MASK               0x00000700
#define LPR_SLOT_POS_REG_MASK               0x000000E0
#define DRI_QUIET_REG_MASK                  0x0000001F
// ***************************************************
#define FLAG_RDS_REG                   3
#define NUM_SLOTS_REG                  4
#define RDS_SLOT_POS_REG               3
#define RSSI_SLOT_POS_REG              2
#define LMR_SLOT_POS_REG               1
#define LPR_SLOT_POS_REG               0
#define DRI_QUIET_REG                  6    // Set to minimum acceptable

// #define INITIALIZATION6_REG_ADDR            0x24
// ***************************************************
#define PAD_I2C18_OEB_REG_MASK              0x00060000
#define DRI_CLKOUT_SEL_REG_MASK             0x00010000
#define PAD_PD10KTST_OEB_REG_MASK           0x0000C000
#define PAD_DRI_OEMAXB_REG_MASK             0x00002000
#define PAD_DRI_OEB_REG_MASK                0x00001C00
#define RXADC_LEVEL_REG_MASK                0x000003FF
#define CHIPID_MASK                         0x0000FF00  // Reset values: TA2: 0x0000C000 ; TB2: 0x00007C00
// ***************************************************
#define PAD_I2C18_OEB_REG              0
#define DRI_CLKOUT_SEL_REG             0
#define PAD_PD10KTST_OEB_REG           0
#define PAD_DRI_OEMAXB_REG             0
#define PAD_DRI_OEB_REG                0
#define RXADC_LEVEL_REG                581

// #define REF_REG_ADDR                        0x28
// ***************************************************
#define REF_PWRUP_MASK                      0x00200000
#define REG2V_PWRUP_MASK                    0x00100000
#define SELFBIASOFF_MASK                    0x00040000
#define IBIAS_ADJUST_MASK                   0x00030000
#define TESTMODE_VBGVAG_MASK                0x00008000
#define MIX_VAG_MASK                        0x00007000
#define LOWPOWER_MASK                       0x00000800
#define ADC_VAG_MASK                        0x00000700
#define LNA_VAG_MASK                        0x000000F0
#define ADJUST_MASK                         0x0000000F
// ***************************************************
#define REF_PWRUP                      0
#define REG2V_PWRUP                    0
#define SELFBIASOFF                    0
#define IBIAS_ADJUST                   0
#define TESTMODE_VBGVAG                0
#define MIX_VAG                        0
#define LOWPOWER                       0
#define ADC_VAG                        0
#define LNA_VAG                        0
#define ADJUST                         0

// #define LNA_REG_ADDR                        0x2C
// ***************************************************
#define RMS_SHIFT_RIGHT_MASK                0x08000000
#define PWRUP_RMSDETECTOR_MASK              0x04000000
#define PWRUP_REG_MASK                      0x02000000
#define PWRUP_LNA_MASK                      0x01000000
#define SEL_IBIAS_FROM_CLKXTAL_MASK         0x00800000
#define USEATTEN_MASK                       0x00600000
#define ANTENNA_TUNECAP_REG_MASK            0x001F0000
#define USE_TEST_MASK                       0x00008000
#define LNA_CURRENT_UP30_MASK               0x00004000
#define LNA_CURRENT_DN50_MASK               0x00002000
#define USE_LOW_REG_MASK                    0x00000100
#define REG_ADJUST_MASK                     0x000000F0
#define VCASN2_UP_MASK                      0x00000008
#define VCASN2_DN_MASK                      0x00000004
#define VCASN1_UP_MASK                      0x00000002
#define VCASN1_DN_MASK                      0x00000001
#define IBIAS2_UP_MASK                      0x00000008
#define IBIAS2_DN_MASK                      0x00000004
#define IBIAS1_UP_MASK                      0x00000002
#define IBIAS1_DN_MASK                      0x00000001
// ***************************************************
#define RMS_SHIFT_RIGHT                0
#define PWRUP_RMSDETECTOR              0
#define PWRUP_REG                      0
#define PWRUP_LNA                      0
#define SEL_IBIAS_FROM_CLKXTAL         0
#define USEATTEN                       0
#define ANTENNA_TUNECAP_REG            0
#define USE_TEST                       0
#define LNA_CURRENT_UP30               0
#define LNA_CURRENT_DN50               0
#define USE_LOW_REG                    0
#define REG_ADJUST                     0
#define VCASN2_UP                      0
#define VCASN2_DN                      0
#define VCASN1_UP                      0
#define VCASN1_DN                      0
#define IBIAS2_UP                      0
#define IBIAS2_DN                      0
#define IBIAS1_UP                      0
#define IBIAS1_DN                      0


// #define MIXFILT_REG_ADDR                    0x30
// ***************************************************
#define RESET_MIX_MASK                      0x00008000
#define PWRUP_DIG_CLK_MASK                  0x00004000
#define PWRUP_ADC_CLK_MASK                  0x00002000
#define PWRUP_MASK                          0x00001000
#define DELAY_DIG_CLK1_MASK                 0x00000800
#define DELAY_DIG_CLK0_MASK                 0x00000400
#define INVERT_DIG_CLK_MASK                 0x00000200
#define MOVE_ADC_SAMPLE_BACK_MASK           0x00000100
#define INVERT_PHASE_MASK                   0x00000080
#define EXTEND_NOV_MASK                     0x00000040
#define DISABLE_INPUT_SWAP_MASK             0x00000020
#define DISABLE_GAIN_SWAP_MASK              0x00000010
#define CURRENT_UP30_MASK                   0x00000008
#define CURRENT_DOWN50_MASK                 0x00000004
#define VAG_CURRENT_UP30_MASK               0x00000002
#define VAG_CURRENT_DOWN50_MASK             0x00000001
// ***************************************************
#define RESET_MIX                      0
#define PWRUP_DIG_CLK                  0
#define PWRUP_ADC_CLK                  0
#define PWRUP                          0
#define DELAY_DIG_CLK1                 0
#define DELAY_DIG_CLK0                 0
#define INVERT_DIG_CLK                 0
#define MOVE_ADC_SAMPLE_BACK           0
#define INVERT_PHASE                   0
#define EXTEND_NOV                     0
#define DISABLE_INPUT_SWAP             0
#define DISABLE_GAIN_SWAP              0
#define CURRENT_UP30                   0
#define CURRENT_DOWN50                 0
#define VAG_CURRENT_UP30               0
#define VAG_CURRENT_DOWN50             0

// #define CLK1_REG_ADDR                       0x34
// ***************************************************
#define HWFM_RESET_MASK                     0x20000000
#define CLK1_PWRUP_MASK                     0x10000000
#define ENABLE_REG_MASK                     0x08000000
#define XTALAMP_SAVEPOWER_BIT1_MASK         0x02000000
#define XTALAMP_SAVEPOWER_BIT0_MASK         0x01000000
#define CLKOUT_SAVEPOWER_BIT1_MASK          0x00800000
#define CLKOUT_SAVEPOWER_BIT0_MASK          0x00400000
#define ENABLE_TESTVCOVOLTAGE_MASK          0x00200000
#define ENABLE_LSBBOOST_MASK                0x00100000
#define ENABLE_TESTPTATIDAC_MASK            0x00080000
#define ENABLE_TEST_DELAY_DAC_MASK          0x00040000
#define SEL_CLK_PTATIDAC_DELAY_MASK         0x00020000
#define SEL_CLK_SC_IS_FREF24_MASK           0x00010000
#define TESTOUTQ_ISDELAYOUT_PFDB_MASK       0x00008000
#define DISABLE_CLKMIXFILT_MASK             0x00004000
#define ENABLE_VCO_MASK                     0x00002000
#define PTAT_SHORT_RESISTOR_MASK            0x00001000
#define PTAT_TEMPCO_DN_MASK                 0x00000800
#define PTAT_TEMPCO_UP_MASK                 0x00000400
#define PTAT_PCASC_UP_MASK                  0x00000200
#define PTAT_PCASC_DN_MASK                  0x00000100
#define DELAY_SCALEUP_MASK                  0x00000080
#define DELAY_SCALEDN_MASK                  0x00000040
#define ENABLE_TAPDELAYFIX_REG_MASK         0x00000020
#define ENABLE_DELAYMISMATCH_TAU_REG1_MASK  0x00000010
#define XTAL_LOAD_MASK                      0x0000000F
// ***************************************************
#define HWFM_RESET                     0
#define CLK1_PWRUP                     0
#define ENABLE_REG                     0
#define XTALAMP_SAVEPOWER_BIT1         0
#define XTALAMP_SAVEPOWER_BIT0         0
#define CLKOUT_SAVEPOWER_BIT1          0
#define CLKOUT_SAVEPOWER_BIT0          0
#define ENABLE_TESTVCOVOLTAGE          0
#define ENABLE_LSBBOOST                0
#define ENABLE_TESTPTATIDAC            0
#define ENABLE_TEST_DELAY_DAC          0
#define SEL_CLK_PTATIDAC_DELAY         0
#define SEL_CLK_SC_IS_FREF24           0
#define TESTOUTQ_ISDELAYOUT_PFDB       0
#define DISABLE_CLKMIXFILT             0
#define ENABLE_VCO                     0
#define PTAT_SHORT_RESISTOR            0
#define PTAT_TEMPCO_DN                 0
#define PTAT_TEMPCO_UP                 0
#define PTAT_PCASC_UP                  0
#define PTAT_PCASC_DN                  0
#define DELAY_SCALEUP                  0
#define DELAY_SCALEDN                  0
#define ENABLE_TAPDELAYFIX_REG         0
#define ENABLE_DELAYMISMATCH_TAU_REG1  0
#define XTAL_LOAD                      0

// #define CLK2_REG_ADDR                       0x38
// ***************************************************
#define PLL_PHASE_SCALE_MAX_REG3_MASK       0x70000000
#define PLL_PHASE_TWEAK_REG2_MASK           0x0C000000
#define PLL_FREQ_TWEAK_REG2_MASK            0x03000000
#define PLL_LF_OVERRIDE_REG24_MASK          0x00FFFFFF
// ***************************************************
#define PLL_PHASE_SCALE_MAX_REG3       0
#define PLL_PHASE_TWEAK_REG2           0
#define PLL_FREQ_TWEAK_REG2            0
#define PLL_LF_OVERRIDE_REG24          0

// #define ADC_REG_ADDR                        0x3C
// ***************************************************
#define VREF_TM_MUX_ENABLE_MASK             0x00040000
#define PWRUPQ_MASK                         0x00020000
#define PWRUPI_MASK                         0x00010000
#define TESTMODE_ADCREF_MASK                0x00008000
#define ADC_VREF_ADJ_MASK                   0x00007000
#define ADC_EXTEND_NOV_MASK                 0x00000800
#define EXTEND_DELAY_MASK                   0x00000400
#define CURRENT_DOWN50_VREF_MASK            0x00000200
#define CURRENT_UP30_VREF_MASK              0x00000100
#define CURRENT_DOWN50_VAG_MASK             0x00000080
#define CURRENT_UP30_VAG_MASK               0x00000040
#define CURRENT_DOWN50_OTA2_MASK            0x00000020
#define CURRENT_UP30_OTA2_MASK              0x00000010
#define CURRENT_DOWN50_OTA1_MASK            0x00000008
#define CURRENT_UP30_OTA1_MASK              0x00000004
#define CURRENT_DOWN50_ALL_MASK             0x00000002
#define CURRENT_UP30_ALL_MASK               0x00000001
// ***************************************************
#define VREF_TM_MUX_ENABLE             0
#define PWRUPQ                         0
#define PWRUPI                         0
#define TESTMODE_ADCREF                0
#define ADC_VREF_ADJ                   0
#define ADC_EXTEND_NOV                 0
#define EXTEND_DELAY                   0
#define CURRENT_DOWN50_VREF            0
#define CURRENT_UP30_VREF              0
#define CURRENT_DOWN50_VAG             0
#define CURRENT_UP30_VAG               0
#define CURRENT_DOWN50_OTA2            0
#define CURRENT_UP30_OTA2              0
#define CURRENT_DOWN50_OTA1            0
#define CURRENT_UP30_OTA1              0
#define CURRENT_DOWN50_ALL             0
#define CURRENT_UP30_ALL               0


// #define STFM1000_UNUSED_REG_40              0x40
// ***************************************************
// ***************************************************

// #define AGC_CONTROL1_REG_ADDR               0x44
// ***************************************************
#define B2_SEL_LNA_OR_WB_REG_MASK           0x00200000
#define B2_LNATH_REG_MASK                   0x001F0000
#define B2_BYPASS_REF_CTL_REG_MASK          0x00008000
#define B2_BYPASS_AGC_CTL_REG_MASK          0x00004000
#define B2_AGCREF_REG_MASK                  0x00003F00
#define B2_LNA_GAIN_REG_MASK                0x000000F8
#define B2_AGCGAIN_REG_MASK                 0x00000007
// ***************************************************
#define B2_SEL_LNA_OR_WB_REG           0
#define B2_LNATH_REG                   12
#define B2_BYPASS_REF_CTL_REG          0
#define B2_BYPASS_AGC_CTL_REG          1    // Start out with AGC bypassed
#define B2_AGCREF_REG                  30
#define B2_LNA_GAIN_REG                0    // Start with minimum gain
#define B2_AGCGAIN_REG                 2


//#define AGC_CONTROL2_REG_ADDR               0x48
// ***************************************************
#define DECREFCTL_REG_MASK                  0x0FFF0000
#define B2_DECAGC_REG_MASK                  0x00000FFF
// ***************************************************
#define DECREFCTL_REG                  20
// 80 runs the AGC at 800k/80 = 10kHz
#define B2_DECAGC_REG                  80


// #define STFM1000_UNUSED_REG_4C              0x4C
// ***************************************************
// ***************************************************

// #define STFM1000_UNUSED_REG_50              0x50
// ***************************************************
// ***************************************************

// #define STFM1000_UNUSED_REG_54              0x54
// ***************************************************
// ***************************************************

// #define STFM1000_UNUSED_REG_58              0x58
// ***************************************************
// ***************************************************

// #define DATAPATH_REG_ADDR                   0x5C
// ***************************************************
#define DP_EN_REG_MASK                      0x01000000
#define DB_ACCEPT_REG_MASK                  0x00010000
#define TST_CLK_OUT_REG_MASK                0x00004000
#define I2C_CLK_FREQ_REG_MASK               0x00000E00
#define DP_CLK_SEL_REG_MASK                 0x00000100
#define INTERP_DATAPATH_CLKGATE_REG_MASK    0x00000080
#define DRI_CLK_DIV_REG_MASK                0x0000007C
#define DRI_CLK_SEL_REG_MASK                0x00000002
#define DRI_EN_REG_MASK                     0x00000001
// ***************************************************
#define DP_EN_REG                      0
// Start off accepting all B2_x writes
#define DB_ACCEPT_REG                  1
#define TST_CLK_OUT_REG                0
#define I2C_CLK_FREQ_REG               0
#define DP_CLK_SEL_REG                 0
#define INTERP_DATAPATH_CLKGATE_REG    0
// Want 4 since 24MHz/6MHz = 4
#define DRI_CLK_DIV_REG                4
#define DRI_CLK_SEL_REG                0
#define DRI_EN_REG                     0


// #define RMS_REG_ADDR                        0x60
// ***************************************************
#define RMS_I_REG_MASK                      0x7FFF0000
#define RMS_Q_REG_MASK                      0x00007FFF
// ***************************************************

// #define AGC_STAT_REG_ADDR                   0x64
// ***************************************************
#define AGCOUT_STAT_REG_MASK                0x1F000000
#define AGCREF_STAT_REG_MASK                0x003F0000
#define LNA_RMS_REG_MASK                    0x00001F00
#define WB_RMS_REG_MASK                     0x000000FF
// ***************************************************

// #define SIGNALQUALITY_REG_ADDR              0x68
// ***************************************************
#define SRC_NOMPOS_REG_MASK                 0x0000FF00
#define NEAR_CHAN_AMP_READY_REG_MASK        0x00000080
#define NEAR_CHAN_AMPLITUDE_REG_MASK        0x0000007F
// ***************************************************

// #define DCEST_REG_ADDR                      0x6C
// ***************************************************
#define DCEST_I_REG_MASK                    0x7FFF0000
#define DCEST_Q_REG_MASK                    0x00007FFF
// ***************************************************

// #define RSSI_TONE_REG_ADDR                  0x70
// ***************************************************
#define TONEDATA_REG_MASK                   0x0FFF0000
#define RSSI_DCEST_REG_MASK                 0x0000FFFF
// ***************************************************

// #define PILOTCORRECTION_REG_ADDR            0x74
// ***************************************************
#define PILOTEST_REG_MASK                   0xFF000000
#define PILOTEST_REG_TA2_MASK               0xFF000000
#define PILOTEST_REG_TB2_MASK               0xFE000000
#define PILOTCORRECTION_REG_MASK            0x00FFFFFF
// ***************************************************

// #define ATTENTION_REG_ADDR                  0x78
// ***************************************************
#define ATTENTION_REG_MASK                  0x0000003F
#define MIXFILT_10PERCENT_REG_MASK          0x00000020
#define DRI_OVERRUN_REG_MASK                0x00000010
#define PILOT_LIMIT_REG_MASK                0x00000008
#define ZIF_OUTCLIP_REG_MASK                0x00000004
#define WBRMS_CLIP_REG_MASK                 0x00000002
#define DEMOD_OUTCLIP_REG_MASK              0x00000001
// ***************************************************

// #define CL3_REG_ADDR                        0x7C
// ***************************************************
#define PLL_DAC_OVERFLOW_REG2_MASK          0x06000000
#define PLL_MODE_REG2_MASK                  0x01800000
#define PLL_SC_FILTEROUT_REG23_MASK         0x007FFFFF
// ***************************************************

// #define CHIPID_REG_ADDR                     0x80
// ***************************************************
#define MAJOR_REV_ID_REG_MASK               0x000000F0
#define MINOR_REV_ID_REG_MASK               0x0000000F
#define VERSION_TA1                         0x00000001
#define VERSION_TA2                         0x00000002  
#define VERSION_TB1                         0x00000011
#define VERSION_TB2                         0x00000012
// ***************************************************


////////////////////////////////////////////////////////////////////////////////
//! GENERIC Interface Section
////////////////////////////////////////////////////////////////////////////////



//! Generic Definitions

//! temp for when settings functions available from generic level
#define SETTINGS_IN_TABLE 20

#define SettingTableSize (sizeof(ddi_tuner_TunerSettingsTable_t)\
+(sizeof(ddi_tuner_TunerSettingsEntry_t)*(SETTINGS_IN_TABLE-1)))


//!  Mute duration parameter DDI_TUNER_SOUND_ON_USEC
#define DDI_TUNER_SOUND_ON_USEC (50 * 1000)

//!  Mute duration parameter DDI_TUNER_SOUND_OFF_USEC
#define DDI_TUNER_SOUND_OFF_USEC (20 * 1000)




////////////////////////////////////////////////////////////////////////////////
//! TUNING Interface Section
////////////////////////////////////////////////////////////////////////////////



//!  Tuning Structure
typedef struct _STFM1000Tune_
{
    //! Initial 18 registers copied from default data structure

    // shadow images written to on-chip registers
    // Shadow reg for TUNE1_REG
    DWORD sd_ctl_ShadowTune1_dw;
    // Shadow reg for SDNOMINAL_REG
    DWORD sd_ctl_ShadowSdnominal_dw;
    // Shadow reg for PILOTTRACKING_REG
    DWORD sd_ctl_ShadowPilotTracking_dw;
    // Shadow reg for INITIALIZATION1_REG
    DWORD sd_ctl_ShadowInitialization1_dw;
    // Shadow reg for INITIALIZATION2_REG
    DWORD sd_ctl_ShadowInitialization2_dw;
    // Shadow reg for INITIALIZATION3_REG
    DWORD sd_ctl_ShadowInitialization3_dw;
    // Shadow reg for INITIALIZATION4_REG
    DWORD sd_ctl_ShadowInitialization4_dw;
    // Shadow reg for INITIALIZATION5_REG
    DWORD sd_ctl_ShadowInitialization5_dw;
    // Shadow reg for INITIALIZATION6_REG
    DWORD sd_ctl_ShadowInitialization6_dw;
    // Shadow reg for ANALOG1_REG
    DWORD sd_ctl_ShadowRef_dw;
    // Shadow reg for ANALOG2_REG
    DWORD sd_ctl_ShadowLna_dw;
    // Shadow reg for ANALOG3_REG
    DWORD sd_ctl_ShadowMixFilt_dw;
    // Shadow reg for ANALOG4_REG
    DWORD sd_ctl_ShadowClk1_dw;
    // Shadow reg for ANALOG4_REG
    DWORD sd_ctl_ShadowClk2_dw;
    // Shadow reg for ANALOG5_REG
    DWORD sd_ctl_ShadowAdc_dw;
    // Shadow reg for AGC_CONTROL1_REG
    DWORD sd_ctl_ShadowAgcControl1_dw;
    // Shadow reg for AGC_CONTROL2_REG
    DWORD sd_ctl_ShadowAgcControl2_dw;
    // Shadow reg for DATAPATH_REG
    DWORD sd_ctl_ShadowDataPath_dw;

    // tuning parameters
    // user-set RSSI threshold for good station (SETID_SEARCH_RSSI_THRESH)
    WORD sd_ctl_TuneRssiTh_w;
    // driver-set Multiplex threshold for good station
    WORD sd_ctl_TuneMpxDcTh_w;
    // driver-set Adjacent-channel interference threshold for Bypass Filter
    // -setting
    WORD sd_ctl_AdjChanTh_w;
    // driver-set Pilot Level threshold to allow stereo decoding (else force
    // -mono)
    WORD sd_ctl_PilotEstTh_w;

    // parameters for monitoring AGC
    // driver-set switch to prevent AGC loop from running while DRI active
    WORD sd_ctl_AgcMonitorDisabled;
    // Approximate number of milliseconds between AGC monitor cycles
    WORD sd_ctl_agc_msec_per_cycle;
    // Current state of AGC on chip set by monitor routine
    BOOL sd_ctl_LnaDriving_u1;
    // driver-set AGC monitor threshold of LNA RMS-output to turn off LNA
    WORD sd_ctl_pCoefLnaTurnOffTh_w;
    // driver-set AGC monitor threshold of LNA RMS-output to turn on LNA
    WORD sd_ctl_pCoefLnaTurnOnTh_w;
    // Put this in AGC_CONTROL1_REG_ADDR:B2_AGCREF_REG_MASK when turn off LNA
    WORD sd_ctl_pRegAgcRefLnaOff_w;
    // Put this in AGC_CONTROL1_REG_ADDR:B2_AGCREF_REG_MASK when turn on LNA
    WORD sd_ctl_pRegAgcRefLnaOn_w;

    // tuning data being maintained
    // IF offset to tuning table (in 100 kHz units)
    INT  sd_ctl_IfFreq_i;
    // Freq which Analog Front End should be tuned to
    WORD sd_ctl_FeFreq_w;
    // calculated tuning data to be sent to SD
    DWORD sd_ctl_Tune1Data_dw;
    // sdnominal data to be sent to SD
    DWORD sd_ctl_SdnominalData_dw;

    // data read from on-chip registers
    WORD sd_ctl_ShadowRssiDcEst_w; // current RSSI reading
    WORD sd_ctl_ShadowRssiDcEstLog_w; // above value converted to log 
    WORD sd_ctl_ShadowToneData_w;  // current Multiplex reading
    // Added - francis: To accomodate SD_SetChannelFilter sequence
    WORD sd_ctl_ShadowSignalQuality_w; // cur Signal Quality reg reading
    // Pilot Present flag
    BOOL sd_ctl_PilotPresent_b;
    BOOL bPrevPilotPresent;
    DWORD STFM1000_Version;
 
}STFM1000Tune_t;

/* Check if build environment already defined following */
#ifndef TUNE_RSSI_THRESH
#define TUNE_RSSI_THRESH        (20)    // First Guess from Hardware Team
#endif

#ifndef TUNE_MPX_DC_THRESH
#define TUNE_MPX_DC_THRESH      (300)   // First Guess from Hardware Team
#endif

#ifndef ADJACENT_CHAN_THRESH
#define ADJACENT_CHAN_THRESH    (100)   // First Guess from Hardware Team
#endif

#ifndef PILOT_EST_THRESH
#define PILOT_EST_THRESH        (17)    // TBD from SD hardware folks
#endif

// AGC control initial values

#ifndef AGC_MSEC_PER_CYCLE
#define AGC_MSEC_PER_CYCLE 20
#endif

#ifndef LNA_TURN_OFF_THRESH
#define LNA_TURN_OFF_THRESH 8
#endif

#ifndef LNA_TURN_ON_THRESH
#define LNA_TURN_ON_THRESH 15
#endif

#ifndef LNA_TURN_OFF_REG_REF
#define LNA_TURN_OFF_REG_REF 27
#endif

#ifndef LNA_TURN_ON_REG_REF
#define LNA_TURN_ON_REG_REF 8
#endif

#ifndef LNA_AMP1_IMPROVE_DISTORTION
#define LNA_AMP1_IMPROVE_DISTORTION         0x00800000
#endif 

// convert between display values and register values

#define RSSI_DISPLAY_TO_REG(displayval) ((60*displayval)/100)
#define RSSI_REG_TO_DISPLAY(regval) ((100*regval)/60)


#define FREQUENCY_100KHZ_MIN   758
#define FREQUENCY_100KHZ_RANGE 325




////////////////////////////////////////////////////////////////////////////////
//! I2C Interface Section
////////////////////////////////////////////////////////////////////////////////

//! I2C Interface buffer

#define STFM1000_I2C_ADDR 0xC0

#define MAX_REGISTERS_I2C 1
#define MAX_I2C_WRITE_TRIES 6
#define MAX_I2C_READ_TRIES  6

/**************************************************** 
 * I2C data and clock pin
 * GPIO-16: I2C_SCL
 * GPIO-17: I2C_SDA
 ****************************************************/
#define HW_I2C_CLOCK_ENR    HW_GP0ENR.B.B16    //  0 - Assigned function, 1 - GPIO pin
#define HW_I2C_CLOCK_PWR    HW_GP0PWR.B.B16    //  0 - Power down,        1 - Power on
#define HW_I2C_CLOCK_DOER   HW_GP0DOER.B.B16   //  0 - Input,             1 - Output
#define HW_I2C_CLOCK_DIR    HW_GP0DIR.B.B16    //  Input data pin if configured as input
#define HW_I2C_CLOCK_DOR    HW_GP0DOR.B.B16    //  Output dtat pint if configured as output
#define HW_I2C_DATA_ENR	    HW_GP0ENR.B.B17
#define HW_I2C_DATA_PWR	    HW_GP0PWR.B.B17
#define HW_I2C_DATA_DOER    HW_GP0DOER.B.B17
#define HW_I2C_DATA_DIR     HW_GP0DIR.B.B17
#define HW_I2C_DATA_DOR     HW_GP0DOR.B.B17

#if 0
// speed in MHz * 10 /4 - 3 for 50Khz I2C clock
#define I2C_IDLE_SPEED      102/4 -3 
#define I2C_FM_SPEED        240/4 -3 
#define I2C_FM_EQ_SPEED     300/4 -3 
#define I2C_MP3_SPEED       360/4 -3 
#define I2C_ADPCM_SPEED     360/4 -3
#define I2C_MIXER_SPEED IDLE_SPEED  
#define I2C_ADCBASE_SPEED   360/4 -3 
#define I2C_MAX_SPEED       480/4 -3 
#define I2C_WMA_SPEED       600/4 -3  
#define I2C_MP3ENCODE_SPEED 636/4 -3  
#define I2C_PEAK_SPEED      756/4 -3 
#endif

// compute FACT = ((DCLK/I2CCLOCK) - 22)/8 for 20Khz I2C clock
#define I2C_IDLE_SPEED      (INT)((102*5 -22)/8) 
//#define I2C_FM_SPEED        (INT)((180*5 -22)/8) 
#define I2C_FM_SPEED        (INT)((240*5 -22)/8)    // stmp10415
#define I2C_FM_EQ_SPEED        (INT)((300*5 -22)/8)   
#define I2C_MP3_SPEED       (INT)((360*5 -22)/8) 
#define I2C_ADPCM_SPEED     (INT)((360*5 -22)/8)
#define I2C_MIXER_SPEED     IDLE_SPEED  
#define I2C_ADCBASE_SPEED   (INT)((360*5 -22)/8) 
#define I2C_MAX_SPEED       (INT)((480*5 -22)/8) 
#define I2C_WMA_SPEED       (INT)((600*5 -22)/8)  
#define I2C_MP3ENCODE_SPEED (INT)((636*5 -22)/8)  
#define I2C_PEAK_SPEED      (INT)((756*5 -22)/8)  

 
#define I2C_USBMSC_SPEED    686/4 - 3		// For USBMSC: fDCLK is fixed at 68.57 Mhz



#define PWR_ON_HW_I2CCSR_STATE 0x200000
#define RESET_HW_I2CCSR_STATE  0x010000 //0x008000 @ power on, or 0x010000 otherwise
#define RESET_HW_I2CCSR_MASK   0xFE7FFF


/****************************************************
 * I2C
 ****************************************************/
////////////////////////////////////////////////////////////////////////////////
//  Macros
////////////////////////////////////////////////////////////////////////////////
#define TIMEOUT_COUNT  8192
#define TIMEOUT_ERROR  -1
#define ACKF_ERROR     -2 
#define READMODE        1    
#define WRITEMODE       0 
#define ONE_BYTE        0   // WL values 8 bit
#define TWO_BYTES       2   // WL values 16 bit
#define THREE_BYTES     1   // WL values 24 bit
////////////////////////////////////////////////////////////////////////////////
//  Data type Definitions
////////////////////////////////////////////////////////////////////////////////
typedef union               /* I2C Data Register */
{
    struct {
        unsigned int BYTE2 :8;  // LSB
        unsigned int BYTE1 :8;
        unsigned int BYTE0 :8;  // MSB 
    } B;
     
    int I;
    unsigned U;
} i2cdata_type;

typedef union                
{
    DWORD D;
    struct {
        unsigned int W1;    // LSB
        unsigned int W0;    // MSB 
    } U;
 } DWORD_type;
 
////////////////////////////////////////////////////////////////////////////////
//  Prototypes
////////////////////////////////////////////////////////////////////////////////
_reentrant INT STFM1000I2CInit(WORD mode, WORD ClockDiv);
_reentrant INT STFM1000I2CWriteRegister(WORD I2C_Sub_Addr, DWORD DataOut);
_reentrant INT STFM1000I2CReadRegister(WORD I2C_Sub_Addr, DWORD DataOut,DWORD * DataIn);
_reentrant void STFM1000I2CClose(void);
_reentrant INT  STFM1000SetLnaBias(INT iRssiLog);

//! Tuning Function Prototypes
//!

_reentrant void STFM1000Reset(void);

_reentrant void STFM1000RegValsReset(void);

_reentrant INT STFM1000RegsPowerUp1(void);
_reentrant INT STFM1000RegsPowerUp2(void);

_reentrant INT STFM1000RegsPowerDown(void);

///////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
_reentrant INT STFM1000QuickTune(WORD wTuneFreqIn);


// * Optimize Channel on chip  (tspec_v09x.pdf::SD_Optimize_Channel)
_reentrant INT STFM1000OptimizeChannel(void);

// OptmizeChannel() calls the following 3 procedures:

// * Set Channel Filter on chip  (tspec_v09x.pdf::SD_SetChannelFilter)
_reentrant INT STFM1000SetChannelFilter(void);

_reentrant INT STFM1000TunerMonitorQuality(void);

// * Look for Pilot Tone on chip  (tspec_v09x.pdf::SD_LookForPilot)
_reentrant INT STFM1000LookForPilot(BOOL * bPilotPresent);

// * OPptimizeChannel on chip  (tspec_v09x.pdf::SD_GearShiftPilotTracking)
_reentrant INT STFM1000GearShiftPilotTracking(void);

_reentrant INT STFM1000TunerSafeStandbyMode(void);

_reentrant void TunerTuningInit(void);

//! Tuner Specific Function Prototypes
//
// Note:  Any of these primitives can block for a significant period of time.
// In that case, the primitive must spend its sleeping time waiting on the
// main message queue for an Abort message, which will cause it immediately
// to return a status of ERROR_DDI_TUNER_UNBLOCKED_BY_ABORT

_inline    INT STFM1000TunerInit(void); //_inline

_reentrant INT STFM1000TunerUninit(void);

_reentrant INT STFM1000TunerPowerUp(void);

_reentrant INT STFM1000TunerPowerDown(void);

_reentrant INT STFM1000TunerUnlockStation(void);

_reentrant INT STFM1000TunerQuicksetStation(WORD wNewFrequency);

_reentrant INT STFM1000TunerLockStation(void);

_reentrant INT STFM1000TunerQueryIsStation(BOOL * pIsStation);
_reentrant INT STFM1000TunerMonitorQualityStateStart(void);
_reentrant INT STFM1000TunerMonitorQualityStateFinal(void);
_reentrant INT STFM1000OptimizeChannelStart(void);
_reentrant INT STFM1000OptimizeChannelEnd(void);
_inline    INT STFM1000SetNearChannelOffset(WORD wOffsetCode);      //_inline
_inline    INT STFM1000GetNearChannelOffset(void);                  //_inline
_inline    INT STFM1000SetNearChannelFilter(WORD wBypassSetting);   //_inline
_inline    INT STFM1000GearShiftPtGear(DWORD dwBits);               //_inline

/* Private functions */

_inline    INT STFM1000DriOff(void);
_inline    INT STFM1000DriOn(void);                                 //_inline
_reentrant INT STFM1000DataPathOff(void);
_reentrant INT STFM1000DataPathOn(void);
_reentrant INT STFM1000TakeDown(void);
_reentrant INT STFM1000BringUp(void);
_reentrant INT STFM1000TunerLowPowerMode(void);
   
//! I2C Interface Function Prototypes

_reentrant INT STFM1000WriteRegister(WORD wAddr, DWORD dwMask,
    DWORD *pShadow, DWORD dwData);

_reentrant INT STFM1000ReadRegisterToShadow(WORD wAddr, DWORD *pShadow);

_reentrant void STFM1000ReadShadowRegister(DWORD dwMask, DWORD *pShadow,
    DWORD *pData);

_reentrant INT STFM1000ReadSubRegister(WORD wAddr, DWORD dwMask,
    DWORD *pData);

_reentrant void STFM1000delay_ms(WORD wDelay);		  

_reentrant INT STFM1000ContinueScanning( void );
_reentrant INT STFM1000ContinueSearching( void );
_reentrant INT STFM1000TunerSearchEnded( WORD wSearchFrequency );
_reentrant INT STFM1000NextSearchFrequency( void );
_reentrant INT STFM1000EndAutoScanning( void );
_reentrant void STFM1000WriteDataReAlignment(DWORD* pdwNewData);
_reentrant void STFM1000ReadDataReAlignment(DWORD* pdwNewData);
_reentrant INT STFM1000TuningSearchingScanning(void);
_reentrant INT STFM1000FindRSSIThresHold(void);
_reentrant int STFM1000TestPresence(void); // i2c write error detects non presense of stfm1000
_reentrant INT I2CWriteTunerRegister(WORD I2C_Sub_Addr, DWORD DataOut);
_reentrant I2CReset(WORD mode, WORD ClockDiv);


#define UCHAR unsigned char
typedef union                // Required since casting between int and _fract does not work as expected
{
    long _fract FF;
    DWORD D;
    _fract F;
    unsigned int I;
}Fract_type;

#define NOT_SEARCHING	    0
#define SEARCHING_UP        1
#define SEARCHING_DOWN     -1

#ifdef SD_USE_200KHZ_TUNING_GRID
#define FM_FREQ_STEP	200
#else
#define FM_FREQ_STEP	100
#endif

#define RSSI_THRESHOLD_OFFSET   7
/* FM TUNER Constants that can be overruled by project.inc */


#if !defined(NUMBER_OF_PRESETS)
#define NUMBER_OF_PRESETS	10
#endif

#if 0
#if !defined(FM_BAND_LIMIT_HIGH_FREQ)
 #ifdef FM_JAPAN_REGION
  #ifdef SD_USE_200KHZ_TUNING_GRID 
  // stmp10809 solved by changing this from 89000 to 89900 (correct japan high freq limit)
  #define	FM_BAND_LIMIT_HIGH_FREQ	89900
  #else
  #define	FM_BAND_LIMIT_HIGH_FREQ	90000 
  #endif
 #else // non japan fm band
  #ifdef SD_USE_200KHZ_TUNING_GRID 
  #define	FM_BAND_LIMIT_HIGH_FREQ	107900
  #else
  #define	FM_BAND_LIMIT_HIGH_FREQ	108000 
  #endif
 #endif // non japan fm band 
#endif // FM band limit high freq

#if !defined(FM_BAND_LIMIT_LOW_FREQ)
#ifdef FM_JAPAN_REGION
#define	FM_BAND_LIMIT_LOW_FREQ	76000
#else
#define	FM_BAND_LIMIT_LOW_FREQ	87500
#endif
#endif
#endif

#if !defined(FM_BAND_LIMIT_HIGH_FREQ)
    #ifdef  FM_USA_REGION               //USA
                #define FM_BAND_LIMIT_HIGH_FREQ    107900 
                #define FM_BAND_LIMIT_LOW_FREQ     87500
    #else             //  !(USA)
        #ifdef  FM_EUROPE_REGION      //Europe and China
                #define FM_BAND_LIMIT_HIGH_FREQ    108000 
                #define FM_BAND_LIMIT_LOW_FREQ      87500
        #else         //  !(EUROPE)
            #ifdef FM_JAPAN_REGION    //Japan
                #define FM_BAND_LIMIT_HIGH_FREQ	    90000
                #define FM_BAND_LIMIT_LOW_FREQ	    76000
            #endif   // end (JAPAN)
        #endif      // end (EUROPE)
    #endif          // end (USA)
#endif

#if !defined(FM_TUNER_GRID)
#ifdef SD_USE_200KHZ_TUNING_GRID
#define FM_TUNER_GRID	200
#else
#define FM_TUNER_GRID	100
#endif
#endif

#define FM_SEARCH_HIGH_LIMIT (FM_BAND_LIMIT_HIGH_FREQ+FM_FREQ_STEP)
#define FM_SEARCH_LOW_LIMIT (FM_BAND_LIMIT_LOW_FREQ-FM_FREQ_STEP)
#define STFM1000_TA2A_CHIP
#define STFM1000_POWER_REDUCTION_VERSION

/* Set up the preset stations */  
typedef struct PresetStation
{
	WORD Frequency;
	INT	 Level;
} FMTunerPreset;

typedef struct TunerSearchStatus
{
	WORD Frequency;
	INT	 Level;
    INT  MpxAdc;
} FMTunerSearchStatus;

typedef enum
{
    MONITOR_SIGNAL_QUALITY_STATE_START = 0,
    MONITOR_SIGNAL_QUALITY_STATE_PROCESSING_1, 
    MONITOR_SIGNAL_QUALITY_STATE_PROCESSING_2, 
    MONITOR_SIGNAL_QUALITY_STATE_PROCESSING_3,    
    MONITOR_SIGNAL_QUALITY_STATE_COMPLETE
}eMonitorSignalQualityState;

typedef enum
{
    OPTIMIZE_CHANNEL_STATE_START = 0,
    OPTIMIZE_CHANNEL_STATE_PROCESSING_1, 
    OPTIMIZE_CHANNEL_STATE_PROCESSING_2, 
    OPTIMIZE_CHANNEL_STATE_PROCESSING_3,
    OPTIMIZE_CHANNEL_STATE_COMPLETE      
}eOptimizeChannelState;

typedef struct _STFM1000GearShiftPilotTrackingTable
{
    DWORD dw_msec_delay;
    DWORD dw_bits;
} STFM1000GearShiftPilotTrackingTable_t;

#endif //_STFM_1000_H
