#include "stdlib.h"
#include <stdio.h>
#include "math.h"

/*  void xcall(_fract sd_audio_Lpr_i16, 
             _fract sd_audio_Lmr_i16,
             _fract sd_audio_LprB0_i16,
             _fract sd_audio_LprB1_i16,  
             _fract sd_audio_LprA1_i16,
             _fract sd_audio_LprA2_i16,

*/

extern    unsigned short sd_audio_LprGa_u16; 
extern    unsigned short sd_audio_LmrGa_u16; 
            
  _fract sd_audio_Left_i16;                 // left channel audio out
  _fract sd_audio_Right_i16;                // right channel audio out  

  _fract sd_audio_LprXz_i16;                // LPR x(n-1) stereo filter
  _fract sd_audio_LprXzz_i16;               // LPR x(n-2) stereo filter
  _fract sd_audio_LprYz_i16;                // LPR y(n-1) stereo filter
  _fract sd_audio_LprYzz_i16;               // LPR y(n-2) stereo filter
  _fract sd_audio_LmrXz_i16;                // LMR x(n-1) stereo filter
  _fract sd_audio_LmrXzz_i16;               // LMR x(n-2) stereo filter
  _fract sd_audio_LmrYz_i16;                // LMR y(n-1) stereo filter
  _fract sd_audio_LmrYzz_i16;               // LMR y(n-2) stereo filter
  _fract sd_audio_FilteredLpr_i16;          // LPR filter output
  _fract sd_audio_FilteredLmr_i16;          // LMR filter output

  _fract  sd_audio_Acc_i32;                  // mimics H/W accumulator
//  _fract temp1_reg;                         // mimics 16 bit register
//  _fract temp2_reg;                         // mimics 16 bit register
//  _fract temp3_reg;                         // mimics 16 bit register

                                                  // *** Programmable Coef's
  int sd_audio_pCoefForcedMono_u1;               // Forced Mono control bit
  int sd_audio_pCoefBypassBlend_u1;              // Forced bypass of stereo blend
  int sd_audio_pCoefBypassSoftmute_u1;           // Forced bypass of softmute



