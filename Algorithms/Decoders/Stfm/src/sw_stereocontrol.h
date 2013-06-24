////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2005
//
// File :         sw_stereocontrol.h
// Description:   This function contain declarations of functions and
//				  sw_stereocontrol structure.
////////////////////////////////////////////////////////////////////////////////

extern void Update_IIRFiltCoeff(void);
extern void sw_stereocontrol_reset(void);
extern void sw_stereopath_xcall(int _X *pDai, int _X *pDa0, int n);


extern int g_sd_audio_LprB0_i16;
extern int g_sd_audio_LprB1_i16;
extern int g_sd_audio_LprA1_i16;
extern int g_sd_audio_LprA2_i16;

#define DEFAULT_RSSI_ATTACK     24108
#define DEFAULT_RSSI_DELAY      59299 
#define DEFAULT_LPR_BW_TH_LO     5120
#define DEFAULT_LPR_BW_TH_HI     7680
#define DEFAULT_LPR_GA_TH          20
#define DEFAULT_LMR_GA_TH          50
#define DEFAULT_LPR_BW_SL_SC     4800
#define DEFAULT_LPR_BW_SL_SH       10

#define DEFAULT_LPR_GA          32767
#define DEFAULT_LMR_GA          32767

// Default IIR filter coefficients for -3dB cutoff @ 17 kHz
#define DEFAULT_B0               9403 
#define DEFAULT_B1              18812 
#define DEFAULT_B2         DEFAULT_B0 
#define DEFAULT_A1             -16079 
#define DEFAULT_A2              -5563

#define COEF_SCALE                256           /* << 8 same as * 256 */
        
        


_reentrant void sw_stereocontrol_xcall(unsigned short sd_audio_Rssi_u16);
_reentrant void sw_stereocontrol_bwsweep(void);
 
extern int  _Y g_sd_audio_pCoefForcedMono_u1;               // Forced Mono control bit
extern int  _Y g_sd_audio_pCoefBypassBlend_u1;              // Forced bypass of stereo blend
extern int  _Y g_sd_audio_pCoefBypassSoftmute_u1;           // Forced bypass of softmute

typedef struct sw_stereocontrol_str
{                                                  // *** Programmable Coef's
  unsigned short sd_audio_pCoefRssiAttack_u16;    // prog coef RSSI attack
  unsigned short sd_audio_pCoefRssiDecay_u16;     // prog coef RSSI decay

  unsigned short sd_audio_pCoefLprBwThLo_u16;     // prog coef Low Th LPR BW
  unsigned short sd_audio_pCoefLprBwThHi_u16;     // prog coef High Th LPR BW
 
  unsigned short sd_audio_pCoefLprGaTh_u16;       // prog coef Th LPR Gain (SoftMute)
  unsigned short sd_audio_pCoefLmrGaTh_u16;       // prog coef Th LMR Gain (Blend)

  unsigned short sd_audio_pCoefLprBwSlSc_u16;     // prog coef Slope scale LPR BW
  unsigned short sd_audio_pCoefLprBwSlSh_u16;     // prog coef Slope shift LPR BW
//  unsigned short sd_audio_pCoefLprGaSlSc_u16;     // prog coef Slope scale LPR Gain
//  unsigned short sd_audio_pCoefLprGaSlSh_u16;     // prog coef Slope shift LPR Gain

  char sd_audio_pCoefBypassBwCtl_u1;              // prog coef Slope shift LPR Gain
                                                  // *** end of Programmable Coef's

  unsigned short sd_audio_LprGa_u16;              // LPR Gain (SoftMute) desired
  unsigned short sd_audio_LmrGa_u16;              // LMR Gain (Blend) desired

  signed short sd_audio_LprB0_i16;                // LPR,LMR stereo filter coef
  signed short sd_audio_LprB1_i16;                // LPR,LMR stereo filter coef  
  signed short sd_audio_LprA1_i16;                // LPR,LMR stereo filter coef
  signed short sd_audio_LprA2_i16;                // LPR,LMR stereo filter coef

  unsigned long sd_audio_RssiDecoded_u24;         // integer decoded RSSI
  signed short sd_audio_RssiLog_i16;              // 10log10(decoded RSSI)
  unsigned short sd_audio_RssiMant_u16;           // mantissa of float-coded RSSI
  unsigned short sd_audio_RssiExp_u16;            // exponent of float-coded RSSI
  unsigned short sd_audio_Prssi_u16;              // power of 2 for RSSI

  unsigned short sd_audio_ScaledTrueRssi_u16;     // scaled version for precision
  unsigned short sd_audio_FilteredRssi_u16;       // Attack/Decay filtered RSSI
  unsigned short sd_audio_PrevFilteredRssi_u16;   // previous version of above
//  unsigned short sd_audio_DecRssi_u16;            // Decimation modulo counter
  
  signed short sd_audio_ControlLpr_i16;           // master control for LPR
  signed short sd_audio_ControlLmr_i16;           // master control for LMR
  unsigned short sd_audio_LprBw_u16;              // LPR Bandwidth desired
  unsigned short sd_audio_ScaledControlLmr_u16;   // Scaled down version Ctl LMR
  unsigned short sd_audio_ScaledControlLpr_u16;   // Scaled down version Ctl LPR

  signed short sd_audio_B0B_i16;                  // BW ctl B0 coef y-intercept
  unsigned short sd_audio_B0S_u16;                // BW ctl B0 coef scale
  signed short sd_audio_B0M_i16;                  // BW ctl B0 coef slope
  signed short sd_audio_B1B_i16;                  // BW ctl B1 coef y-intercept
  unsigned short sd_audio_B1S_u16;                // BW ctl B1 coef scale
  signed short sd_audio_B1M_i16;                  // BW ctl B1 coef slope
  signed short sd_audio_A1B_i16;                  // BW ctl A1 coef y-intercept
  unsigned short sd_audio_A1S_u16;                // BW ctl A1 coef scale
  signed short sd_audio_A1M_i16;                  // BW ctl A1 coef slope
  signed short sd_audio_A2B_i16;                  // BW ctl A2 coef y-intercept
  unsigned short sd_audio_A2S_u16;                // BW ctl A2 coef scale
  signed short sd_audio_A2M_i16;                  // BW ctl A2 coef slope
  unsigned short sd_audio_AdjBw_u16;              // Desired Filter BW scaled into range

  unsigned long  sd_audio_Acc_u32;                // mimics H/W accumulator
  signed short temp1_reg;                         // mimics 16 bit register
  signed short temp2_reg;                         // mimics 16 bit register
  signed short temp3_reg;                         // mimics 16 bit register

}sw_stereocontrol_t;

extern int gCOEF_SCALE;

extern sw_stereocontrol_t sw_stereocontrol;
