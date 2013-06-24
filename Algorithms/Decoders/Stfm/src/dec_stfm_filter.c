#include <stdio.h>
#include "dec_stfm_filter.h"

//
//
//  File: dec_stfm_filter.c  
//

#define TRUE 1
#define FALSE 0

#define SIMPLE_FILTER 1    // Only used for initial testing
#ifndef SIMPLE_FILTER
/// Prototyptes:
void Init_IIRFiltCoeff(_fract sd_audio_LprB0_i16,
                      _fract sd_audio_LprB1_i16,  
                       _fract sd_audio_LprA1_i16,
                       _fract sd_audio_LprA2_i16,
                       
                       _fract sd_audio_LmrB0_i16,
                       _fract sd_audio_LmrB1_i16,  
                       _fract sd_audio_LmrA1_i16,
                       _fract sd_audio_LmrA2_i16);

void Update_IIRCoefs(void);
void Init_IIRFiltCoefs(void);
#endif
///////////////////////////////////////////////////////////////////////////////////
//        Function IIRFilter
///////////////////////////////////////////////////////////////////////////////////
//
//  Input:
//
//   _fract X                    input sample
//   _fract _circ *PtrXUpdate    pointer to newest X sample
//   _fract _circ *PtrYUpdate    pointer to newest Y output data
//   _fract _circ *PtrIIRCoeff   IIR Filter Coefficients B0, B1, B2, A1, A2
//
//  Output:
//   _fract                     filtered output samples
//
//Note: _fract data type is left justified 16-bit data sample
// _circ type data must be allocated globally
//
//      @44.1K samples/sec, the routine uses about 3.2MIPS
//
_inline _fract IIRFilter(_fract  X, 
                        _fract _circ _X *PtrXUpdate, 
                        _fract _circ _X *PtrYUpdate,  
                        _fract _circ _Y *PtrIIRCoeff)
{
    _fract Y = 0;
    int i = 0;
 
    *--PtrXUpdate = X;      // New input sample X(t) 
    for(i = 0; i < 5; i++)  Y += *PtrIIRCoeff++ * *PtrXUpdate++;  //IIR Filter using MAC instruction
    *PtrYUpdate-- = Y;      // Y(t) becomes Y(t-1) for next loop
   
    return(Y);      // Output value
}

///////////////////////////////////////////////////////////////////////////////////
typedef _circ _Y union
 {   
    struct {
        _fract  B0;              
        _fract  B1;             
        _fract  B2;             
        _fract  A1;             
        _fract  A2;             
    } T;
    _fract IIRFiltCoeff[5];   
 } filt_coeff;


// Filter coefficients Range = (-1.0, +1.0) ==> reduce coeff by and double the gain
// B0/2, B1/2, B2/2=B0/2, A1/2, A2/2
 
    filt_coeff LMR;
    filt_coeff LPR;
   
    _fract sd_audio_FilteredLmr_i16;

    _fract LMR_Data = 0;
    _fract LPR_Data = 0;
    
       
//    _fract sd_audio_FilteredLmr_i16 = 0;
//    _fract sd_audio_FilteredLpr_i16 = 0;
    
// Data to be filtered X(t),   X(t-1), X(t-2), Y(t-1), Y(t-2)
//                                       ^              ^
//                                       |              |
//                                  update w/Y      update w/X
//                                       |              |
//                                       v              v
// Then                X(t-1), X(t-2), Y(t-1), Y(t-2), X(t)
// Then                X(t-2), Y(t-1), Y(t-2), X(t),   X(t-1)
// Then                Y(t-1), Y(t-2), X(t),   X(t-1), X(t-2)
// Then                Y(t-2), X(t),   X(t-1), X(t-2), Y(t-1)
// Then                X(t),   X(t-1), X(t-2), Y(t-1), Y(t-2)
// Then repeat ... 
static _fract _circ _X LMR_XYData[5] = {0, 0, 0, 0, 0};        // LMR data to be filtered
static _fract _circ _X LPR_XYData[5] = {0, 0, 0, 0, 0};        // LRP data to be filtered

static _fract _circ _X *LMR_PtrXUpdate = &LMR_XYData[1];     // Address to store X(t) and start of XYData                            
static _fract _circ _X *LMR_PtrYUpdate = &LMR_XYData[2];     // Address to store Y which becomes Y(t-1) for next loop                            

static _fract _circ _X *LPR_PtrXUpdate = &LPR_XYData[1];     // Address to store X(t) and start of XYData                            
static _fract _circ _X *LPR_PtrYUpdate = &LPR_XYData[2];     // Address to store Y which becomes Y(t-1) for next loop                            

// Flags for blending data
int sd_audio_pCoefForcedMono_u1 = 0;
int sd_audio_pCoefBypassBlend_u1 = 0;
int sd_audio_pCoefBypassSoftmute_u1 = 0;

// **************************************************************************
// **************************************************************************
// Init_IIRFiltCoeff
//             
//  Initialize global filter coefficients
//
// Input:  sd_audio_LprB0_i16       
//         sd_audio_LprB1_i16       
//         sd_audio_LprA1_i16       
//         sd_audio_LprA2_i16      (via sw_stereocontrol) 
//
//         _fract sd_audio_LmrB0_i16
//         _fract sd_audio_LmrB1_i16  
//         _fract sd_audio_LmrA1_i16
//         _fract sd_audio_LmrA2_i16
//                    
//         _fract LprGa_u16
//         _fract LmrGa_u16
//         int sd_audio_pCoefForcedMono_u1
//         int sd_audio_pCoefBypassBlend_u1
//         int sd_audio_pCoefBypassSoftmute_u1
//
//  Output: LPR and LMR IIR filter coefficient arrays
//
//    _fract gLMR_Mult;       // Result of Blending, etc. 
//    _fract gLPR_Mult;       // Result of Blending, etc. 
// 
// **************************************************************************
// **************************************************************************
 
    _fract gLMR_Mult;       // Result of Blending, etc. 
    _fract gLPR_Mult;       // Result of Blending, etc. 

#ifndef SIMPLE_FILTER  

void Update_IIRFiltCoeff(_fract sd_audio_LprB0_i16,
                        _fract sd_audio_LprB1_i16,  
                        _fract sd_audio_LprA1_i16,
                        _fract sd_audio_LprA2_i16,
                       
                        _fract sd_audio_LmrB0_i16,
                        _fract sd_audio_LmrB1_i16,  
                        _fract sd_audio_LmrA1_i16,
                        _fract sd_audio_LmrA2_i16,
                        
                        _fract LprGa_u16,
                        _fract LmrGa_u16,
                        int sd_audio_pCoefForcedMono_u1,
                        int sd_audio_pCoefBypassBlend_u1,
                        int sd_audio_pCoefBypassSoftmute_u1)
{   
    
    sd_audio_LprGa_u16 = LprGa_u16>>1;
    sd_audio_LmrGa_u16 = LmrGa_u16>>12;
                   
    LPR.T.B0 = sd_audio_LprB0_i16;
    LPR.T.B1 = sd_audio_LprB1_i16;            
    LPR.T.B2 = sd_audio_LprB0_i16;              
    LPR.T.A1 = sd_audio_LprA1_i16;              
    LPR.T.A2 = sd_audio_LprA2_i16; 
    
    LMR.T.B0 = sd_audio_LmrB0_i16;
    LMR.T.B1 = sd_audio_LmrB1_i16;            
    LMR.T.B2 = sd_audio_LmrB0_i16;              
    LMR.T.A1 = sd_audio_LmrA1_i16;              
    LMR.T.A2 = sd_audio_LmrA2_i16;  
  
        if (sd_audio_pCoefForcedMono_u1) 
        {
            gLMR_Mult = 0.0;                      // Forced Mono
        }
        else if (sd_audio_pCoefBypassBlend_u1) 
        {
            gLMR_Mult = 1.0;                    // No Blend
        }
        else 
        {
            gLMR_Mult = sd_audio_LmrGa_u16;     // Blend
        }

        if (sd_audio_pCoefBypassSoftmute_u1)
        {
            gLPR_Mult = 1.0;    
        }
        else
        {
            gLPR_Mult  = sd_audio_LprGa_u16;   // SoftMute applied to LPR
            gLMR_Mult *= sd_audio_LmrGa_u16;   // SoftMute applied to LMR
        }            
}
///////////////////////////////////////////////////////////////////////////////////

void Init_IIRFiltCoefs()
{
    int i;

// Initialize sample storage    
    for( i=0; i<5; i++)                     
    {
        LPR_XYData[i] = 0;
        LMR_XYData[i] = 0;
    }

// *** Programmable initial setup
    sd_audio_pCoefForcedMono_u1 = FALSE;
    sd_audio_pCoefBypassBlend_u1 = FALSE;
    sd_audio_pCoefBypassSoftmute_u1 = FALSE;
    
// initialize filter coefficients        
    Update_IIRFiltCoeff(0.286956787,    // LPR B0/2 = 0.5 *18806/32768 
                        0.574096679,    // LPR B1/2 = 18812/32768 
                        0.490692138,    // LPR A1/2 = 16079/32768
                        0.169754028,    // LPR A2/2 = 0.5 * 11125/32768};
                         
                        0.286956787,    // LMR B0/2 = 0.5 *18806/32768 
                        0.574096679,    // LMR B1/2 = 18812/32768 
                        0.490692138,    // LMR A1/2 = 16079/32768
                        0.169754028,    // LMR A2/2 = 0.5 * 11125/32768};
                        1.0,            // sd_audio_LprGa_u16 
                        1.0,            // sd_audio_LmrGa_u16
                        sd_audio_pCoefForcedMono_u1,
                        sd_audio_pCoefBypassBlend_u1,
                        sd_audio_pCoefBypassSoftmute_u1);
}
#endif

// **************************************************************************
// **************************************************************************
// This program performs the main datapath function on the FM multiplex 
// components L+R and L-R.  It produces audio out Left and Right.
// It's role is to filter the signal based on input parameters from
// sw_stereocontrol.
//
// Output: sd_audio_Left_i16  (to audio stream DAC processing)
//         sd_audio_Right_i16 (to audio stream DAC processing)
//
// Version: 35xx simplified version
// **************************************************************************
// **************************************************************************

// !!!!!!! FIXME !!!!!!!!!
// The pInputSample pointer should really be in X memory, but for testing
// purposes, we have a static buffer of data in Y memory that we use
#ifdef REAL_I2S_DATA
void StfmLowPassFilter(int iNumWords, volatile _fract _X * pInputSample, volatile _fract * pFilteredOutput)
#else
void StfmLowPassFilter(int iNumWords, volatile _fract _Y * pInputSample, volatile _fract * pFilteredOutput)
#endif
{
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
//         Filter coefficients are scaled by dividing by 2 to insure they
//         ar within _fract values (-1.0, +1.0)
// 
// **************************************************************************
// *** Stereo Processing ****************************************************
// **************************************************************************
// *** This block operates at Fs = 44.1kHz
// ********
// *** LPR path filter (2nd order IIR)
  
    register int i;
    iNumWords = iNumWords; // This loop interates on stereo samples
     
    for (i = 0; i < iNumWords; i++)   
    {

#ifdef SIMPLE_FILTER
        LPR_Data = (*pInputSample++)>>1;
        LMR_Data = (*pInputSample++)>>1;
             
    #define FOUR_LINES_OF_MATH 
    #ifdef FOUR_LINES_OF_MATH
        *pFilteredOutput++ = (LPR_Data + LMR_Data);       // Left channel   
        *pFilteredOutput++ = (LPR_Data - LMR_Data);       // Right channel
    #endif
    
    //#define DIRECT_COPY
    #ifdef DIRECT_COPY
        *pFilteredOutput++ = LPR_Data;       // Left channel   
        *pFilteredOutput++ = LMR_Data;       // Right channel
    #endif    
#else
          
 // changing order of the calls to IIRFilter improveds asm code ??? Why ???
 // Blend coefficients gLxR_Mult are updated with LxR.IIRFiltCoefs
    
        LMR_Data = gLPR_Mult * IIRFilter(*pInputSample++, LMR_PtrXUpdate, 
                                            LMR_PtrYUpdate, LMR.IIRFiltCoeff);
                                        
        LPR_Data = gLMR_Mult * IIRFilter(*pInputSample++, LPR_PtrXUpdate, 
                                            LPR_PtrYUpdate, LPR.IIRFiltCoeff);
        
        *pFilteredOutput++ = (LPR_Data + LMR_Data);         // Matrix Left channel   
        *pFilteredOutput++ = (LPR_Data - LMR_Data);         // Matrix Right channel 
#endif
    }  
}
