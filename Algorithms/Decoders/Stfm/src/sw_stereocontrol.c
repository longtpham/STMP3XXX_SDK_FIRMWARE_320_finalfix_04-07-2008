////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2005
//
// File :         sw_stereocontrol.c
// Description:   This file contain implementation of function sw_stereocontrol
//                _xcall and sw_stereocontrol_reset.
//				 
////////////////////////////////////////////////////////////////////////////////

#include "sw_stereocontrol.h"
   
// **************************************************************************
// ************ Global variables ********************************************
// **************************************************************************

int _Y g_sd_audio_LprB0_i16 = DEFAULT_B0 * COEF_SCALE;      // -3dB cutoff = 17 kHz
int _Y g_sd_audio_LprB1_i16 = DEFAULT_B1 * COEF_SCALE;      // -3dB cutoff = 17 kHz
int _Y g_sd_audio_LprA1_i16 = DEFAULT_A1 * COEF_SCALE;      // -3dB cutoff = 17 kHz
int _Y g_sd_audio_LprA2_i16 = DEFAULT_A2 * COEF_SCALE;      // -3dB cutoff = 17 kHz

int _Y g_sd_audio_pCoefForcedMono_u1     = 0;
int _Y g_sd_audio_pCoefBypassBlend_u1    = 0;
int _Y g_sd_audio_pCoefBypassSoftmute_u1 = 0;
int _Y g_sd_audio_pCoefBypassBwCtl_u1    = 0;

_fract gLPR_Mult_fract = .99;
_fract gLMR_Mult_fract = .99;

_fract LPR_Data_fract = 0;
_fract LMR_Data_fract = 0;
_fract _circ _Y  LPR_XYData[5] = {0, 0, 0, 0, 0};       // LRP data to be filtered
_fract _circ _Y  LMR_XYData[5] = {0, 0, 0, 0, 0};       // LMR data to be filtered

_fract _circ _Y *_Y LPR_PtrXUpdate = &LPR_XYData[0];    // Address to store X(t) and start of XYData
_fract _circ _Y *_Y LPR_PtrYUpdate = &LPR_XYData[2];    // Address to store Y which becomes Y(t-1) for next loop

_fract _circ _Y *_Y LMR_PtrXUpdate = &LMR_XYData[0];    // Address to store X(t) and start of XYData
_fract _circ _Y *_Y LMR_PtrYUpdate = &LMR_XYData[2];    // Address to store Y which becomes Y(t-1) for next loop

// Semaphore to indicate that global coefficients in variables g_sd_audio_* is ready to be copied to IIRFiltCoeff[]
int IIRFiltCoeffSemaphore = 0;

/* IIRFilterCoeff array is in Q23 format */
int _circ _X IIRFiltCoeff[10] = {  // -3dB cutoff = 17 kHz
                                  DEFAULT_B0 * COEF_SCALE,  // LPR    
                                  DEFAULT_B1 * COEF_SCALE,  // LPR   
                                  DEFAULT_B2 * COEF_SCALE,  // LPR    
                                  DEFAULT_A1 * COEF_SCALE,  // LPR   
                                  DEFAULT_A2 * COEF_SCALE,  // LPR
                                  
                                  DEFAULT_B0 * COEF_SCALE,  // LMR   
                                  DEFAULT_B1 * COEF_SCALE,  // LMR    
                                  DEFAULT_B2 * COEF_SCALE,  // LMR     
                                  DEFAULT_A1 * COEF_SCALE,  // LMR   
                                  DEFAULT_A2 * COEF_SCALE   // LMR
                                }; 

int gCOEF_SCALE                  = COEF_SCALE;

sw_stereocontrol_t sw_stereocontrol = {
                DEFAULT_RSSI_ATTACK,    
                DEFAULT_RSSI_DELAY,      
                DEFAULT_LPR_BW_TH_LO,     
                DEFAULT_LPR_BW_TH_HI,      
                DEFAULT_LPR_GA_TH,           
                DEFAULT_LMR_GA_TH,          
                DEFAULT_LPR_BW_SL_SC,     
                DEFAULT_LPR_BW_SL_SH,
                0,
                DEFAULT_LPR_GA,  
                DEFAULT_LMR_GA,          
                DEFAULT_B0,               
                DEFAULT_B1,              
                DEFAULT_B2,          
                DEFAULT_A1,               
                DEFAULT_A2,
                0, 0, 0, 0,  
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0 
                };

#if 0
/////////////////////////////////////////////////////////////////////////////////
// Name:           Update_IIRFiltCoeff
//
//  Type:           Function
//  Description:    This function copies the IIR coefficients from global variabls g_sd_audio_*
//                  to IIRFiltCoeff if semaphore IIRFiltCoeffSemaphore is set. The
//                  semaphore is then cleared.
//
//  Inputs:         g_sd_audio_*  global variables for filter coefficients
//
//  Outputs:        IIRFiltCoeff[]
//                  IRFiltCieffSemaphore cleared            
//
//  Notes:          Do not make a funclet
//                  LPR and LMR filter coefficients are the same
//                  This routine needs to be called right before sw_stereopath_xcall_asm
//               
/////////////////////////////////////////////////////////////////////////////////
void Update_IIRFiltCoeff()
{    
/*
    if (IIRFiltCoeffSemaphore)
    {   
        IIRFiltCoeff[0] = g_sd_audio_LprB0_i16;
        IIRFiltCoeff[1] = g_sd_audio_LprB1_i16;
        IIRFiltCoeff[2] = g_sd_audio_LprB0_i16;
        IIRFiltCoeff[3] = g_sd_audio_LprA1_i16;
        IIRFiltCoeff[4] = g_sd_audio_LprA2_i16;
        IIRFiltCoeff[5] = g_sd_audio_LprB0_i16;
        IIRFiltCoeff[6] = g_sd_audio_LprB1_i16;
        IIRFiltCoeff[7] = g_sd_audio_LprB0_i16;
        IIRFiltCoeff[8] = g_sd_audio_LprA1_i16;
        IIRFiltCoeff[9] = g_sd_audio_LprA2_i16; 
            
        IIRFiltCoeffSemaphore = 0;    
    }
*/
}
#endif

#if 0
#ifdef REAL_I2S_DATA
void StfmLowPassFilter(int iNumWords, volatile _fract _X * pInputSample, volatile _fract * pFilteredOutput)
#else
void StfmLowPassFilter(int iNumWords, volatile _fract _Y * pInputSample, volatile _fract * pFilteredOutput)
#endif
{
/////////////////////////////////////////////////////////////////////////////////  
//
//   Name:   StfmLowPassFilter  - Simple version of IIR filter used for testing until ASM version is validated      
//
//  Input: 
//
//    _fract * pInputSample  Array of X samples of stereo data ( LPR, LMR ) 
//         
//   int num_samples         Number of stereo samples
//
//  Output: 
//
//  _fract * pFilteredOutput Array of filtered Y stereo data (L, R)
//
//  Notes: Assumes no overflow is possible since all data is _fract type
//         Input values are scaled by dividing by 2 to correct for times 2
//         factor when doing matrix calculations
// 
// **************************************************************************
// *** Stereo Processing ****************************************************
// **************************************************************************
// *** This block operates at Fs = 44.1kHz
// ********
// *** LPR path filter (2nd order IIR)
/////////////////////////////////////////////////////////////////////////////////  

        LPR_Data_fract = (*pInputSample++) >> 1;
        LMR_Data_fract = (*pInputSample++) >> 1;
             
        *pFilteredOutput++ = (LPR_Data_fract + LMR_Data_fract);       // Left channel   
        *pFilteredOutput++ = (LPR_Data_fract - LMR_Data_fract);       // Right channel
}  


#endif
