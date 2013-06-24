/////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename: EncoderProperties.c
// Description: Setup or retrieve encoder setup parameters.
//              NOTE: Currently supports only IMA-ADPCM encoding method
///////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "encadpcmequ.h"
#include "encoderProperties.h"
#include "sysequ.h"
#include "stringlib.h"
#include "recordsettingsmenu.h"

#ifdef FUNCLET
#pragma asm
    extern SysCallFunclet
#pragma endasm
#endif

_X _fract DCOffsetADC[2][7] = { {0,0,0,0,0,0,0}, {0,0,0,0,0,0,0} };       // DC Offsets for ADC  - stmp6964


// Have to update this if you add or remove a sampling rate
#define SAMPLE_RATE_PARMS_TABLE_CNT 7



static void do_nothing(void) {      // This function is here to make the dang FUNCLET, below, work right.
                                    // Without this here, the compiler switches back to ".ptext..." as soon
                                    // as it hits some code.  With this here, the "org p,"SYSFUNCLET_..._P"
                                    // stays in effect.
                                    // The function is declared "static" so that it's invisible outside this file.
  return;
}



/////////////////////////////////////////////////////////////////////////////////
//> Name: EncSetProperties
//
//  Type: Function
//  Description: Set the encoder properties
//  Inputs: Pointer to properties structure:
//          {
//              int  device;
//              char *pFilename;
//              int  samplingRateInHz;
//              int  encodingMethod;
//          }
//  Outputs: SUCCESS or error code
//  Notes: 
//<
/////////////////////////////////////////////////////////////////////////////////

#ifdef FUNCLET
    #pragma asm

        org p,".ptextencoderproperties":

        FEncSetProperties:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        CallFunclet RSRC_FUNCLET_ENCSETPROPERTIES

        org p,"SYSFUNCLET_EncSetProperties_P":
        dc     RSRC_FUNCLET_ENCSETPROPERTIES
    #pragma endasm

RETCODE _reentrant EncSetProperties_Funclet(EncProperties *pProps)
#else
RETCODE _reentrant EncSetProperties(EncProperties *pProps)
#endif
{
  int idx;
  extern _P EncoderSampleRateParms parmsTbl[];


  if (!(g_wEncoderSR & (1<<Stop))) {                        // If encoder is busy, don't change properties!
    return(ERROR_ENCODER_IS_ACTIVE);
  }


  for (idx=0; idx<SAMPLE_RATE_PARMS_TABLE_CNT; idx++) {

    if (parmsTbl[idx].wNSamplesPerSec == pProps->samplingRateInHz) {

      g_wEncFormatTag = pProps->FormatTag; // Algorithm to be used. 1 = PCMWAVE, 2 = MS ADPCM, 0x11 = IMA ADPCM
      g_wEncNumberOfChannels = pProps->EncNumberOfChannels;
      g_wEncPCMBits = pProps->EncPCMBits;    // Number of bits for PCMWAVE (8,16,24), else ignored
      g_wEncCurrentDeviceNum = pProps->device;
      g_wEncSamplingRate = parmsTbl[idx].wNSamplesPerSec;
      g_wEncBitRate =  pProps->bitRateInKbps;
      g_wEncADCGain = parmsTbl[idx].wADCGain;
      g_wEncADCSRR = parmsTbl[idx].wADCSRR;
      packed_strcpy(g_EncFileNameString,pProps->pFilename);
//      g_wSamplingRateIndex = DACSampleRate[idx];      // DAC Sample rate  
      g_wEncADCLChOffset = DCOffsetADC[0][idx];         // L-Chan DC Offset  stmp6964
      g_wEncADCRChOffset = DCOffsetADC[1][idx];         // R-Chan DC Offset  stmp6964 
      g_wEncSamplingRateIdx = idx;                      // used by ComputeDCOffset  stmp6964

      return(NOERROR);
    }
  }

  return(ERROR_ENCODER_INVALID_SAMPLE_RATE);
}



                                     // The table of Encoder Sample Rate parameters is created, below,
                                     // using assembly language.  If you try to use C (e.g. a "static"
                                     // array of structs, as it is treated in the code above), the compiler
                                     // will force it into a ".pdata..." section, instead of leaving it in
                                     // the funclet memory section.  This would keep it from being overlayed.
#pragma asm
  include "encadpcmequ.inc"

; include "dacequ.inc"


;; Source frq -> Target
;; 48.000 kHz -> 48kHz = (((8*48/48    ) - 1) * $010000) =   =>  DACSRR = $070000
;; 44.100 kHz -> 48kHz = (((8*48/44.1  ) - 1) * $010000) =   =>  DACSRR = $07b51e
;; 32.000 kHz -> 48kHz = (((8*48/32    ) - 1) * $010000) =   =>  DACSRR = $0b0000
;; 24.000 kHz -> 48kHz = (((8*48/24    ) - 1) * $010000) =   =>  DACSRR = $0f0000
;; 22.050 kHz -> 48kHz = (((8*48/22.05 ) - 1) * $010000) =   =>  DACSRR = $106a3b
;; 16.000 kHz -> 48kHz = (((8*48/16    ) - 1) * $010000) =   =>  DACSRR = $170000
;; 12.000 kHz -> 48kHz = (((8*48/12    ) - 1) * $010000) =   =>  DACSRR = $1f0000
;; 11.025 kHz -> 48kHz = (((8*48/11.025) - 1) * $010000) =   =>  DACSRR = $21d476
;;  8.000 kHz -> 48kHz = (((8*48/8     ) - 1) * $010000) =   =>  DACSRR = $2f0000

    
; Only includes sample rates correspoingint to ADC, and in same order
; Does not include 12kHz or 24kHz
; values a little different from above to match values in file: dacequ.inc
;    dc  $2de000     ;;  8.000 kHz
;    dc  $21037c     ;; 11.025 kHzz
;    dc  $167000     ;; 16.000 kHz
;    dc  $1001be     ;; 22.050 kHz
;    dc  $0ab800     ;; 32.000 kHz
;    dc  $0780df     ;; 44.100 kHz
;    dc  $06d000     ;; 48.000 kHz


;FDACSampleRate 
;    dc  DAC8kHz            ;;  8.000 kHz    ??? value incorrect 
;    dc  DAC11_025kHz       ;; 11.025 kHzz   ??? value incorrect  
;    dc  DAC16kHz            ;; 16.000 kHz 
;    dc  DAC22_05kHz       ;; 22.050 kHz   
;    dc  DAC32kHz           ;; 32.000 kHz   
;    dc  DAC44_1kHz         ;; 44.100 kHz   
;    dc  DAC48kHz           ;; 48.000 kHz     

  ; Encoder sampling rate parameter tables, stored in P memory, in the loadable funclet section

FparmsTbl    
  ; 8000 Hz
  dc  ADPCM_NSAMPLESPERSEC_8000
  dc  ADPCM_ADCGAIN_8000
  dc  ADPCM_ADCSRR_8000
  dc  ADPCM_INVSAMPLERATESCALE_8000
  dc  ADPCM_NAVGBYTESPERSEC_8000
  dc  ADPCM_CLUSTERTIMESCALE_8000

  ; 11025 Hz
  dc  ADPCM_NSAMPLESPERSEC_11025
  dc  ADPCM_ADCGAIN_11025
  dc  ADPCM_ADCSRR_11025
  dc  ADPCM_INVSAMPLERATESCALE_11025
  dc  ADPCM_NAVGBYTESPERSEC_11025
  dc  ADPCM_CLUSTERTIMESCALE_11025

  ; 16000 Hz
  dc  ADPCM_NSAMPLESPERSEC_16000
  dc  ADPCM_ADCGAIN_16000
  dc  ADPCM_ADCSRR_16000
  dc  ADPCM_INVSAMPLERATESCALE_16000
  dc  ADPCM_NAVGBYTESPERSEC_16000
  dc  ADPCM_CLUSTERTIMESCALE_16000

  ; 22050 Hz
  dc  ADPCM_NSAMPLESPERSEC_22050
  dc  ADPCM_ADCGAIN_22050
  dc  ADPCM_ADCSRR_22050
  dc  ADPCM_INVSAMPLERATESCALE_22050
  dc  ADPCM_NAVGBYTESPERSEC_22050
  dc  ADPCM_CLUSTERTIMESCALE_22050

  ; 32000 Hz
  dc  ADPCM_NSAMPLESPERSEC_32000
  dc  ADPCM_ADCGAIN_32000
  dc  ADPCM_ADCSRR_32000
  dc  ADPCM_INVSAMPLERATESCALE_32000
  dc  ADPCM_NAVGBYTESPERSEC_32000
  dc  ADPCM_CLUSTERTIMESCALE_32000
 
   ; 44100 Hz
   dc  ADPCM_NSAMPLESPERSEC_44100
  dc  ADPCM_ADCGAIN_44100
   dc  ADPCM_ADCSRR_44100
   dc  ADPCM_INVSAMPLERATESCALE_44100
   dc  ADPCM_NAVGBYTESPERSEC_44100
   dc  ADPCM_CLUSTERTIMESCALE_44100
  
   ; 48000 Hz
   dc  ADPCM_NSAMPLESPERSEC_48000
  dc  ADPCM_ADCGAIN_48000
   dc  ADPCM_ADCSRR_48000
   dc  ADPCM_INVSAMPLERATESCALE_48000
   dc  ADPCM_NAVGBYTESPERSEC_48000
   dc  ADPCM_CLUSTERTIMESCALE_48000

#pragma endasm
  

#ifdef FUNCLET
    #pragma asm
        org p,".ptextencoderproperties":
    #pragma endasm
#endif


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
