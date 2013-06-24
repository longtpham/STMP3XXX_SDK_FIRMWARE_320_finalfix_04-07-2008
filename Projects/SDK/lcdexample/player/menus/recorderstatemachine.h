#ifndef _RECORDER_STATE_MACHINE_H
#define _RECORDER_STATE_MACHINE_H

// types of encoders
#define ENCODER_TYPE_ADPCM_IMA  0
#define ENCODER_TYPE_PCM        1
#define ENCODER_TYPE_MP3        2

//These are in the EncoderCSR
#define ENCODER_NO_SPACE            1<<0
#define ENCODER_RAN_OUT_OF_SPACE    1<<1
#define ENCODER_RECORD_ERROR        1<<2
#define ENCODER_TOLD_TO_STOP        1<<3
#define ENCODER_ALREADY_RECORDING   1<<4
#define ENCODER_PAUSED              1<<5
#define ENCODER_STOPPED             1<<6
#define ENCODER_WARN_LOW_SPACE      1<<7   // Only in EncoderIsrSr, menus may read it.
#define ENCODER_WARNING_SENT        1<<8
#define ENCODER_RECORDING           1<<12

#define ENCODERCURRENTMICROPHONEVOLUME 	HW_MIXMICINVR_GN_ZERO_SETMASK 

#define MIC_LEFT_SOURCE		    		HW_MIXRECSELR_SL_MIC_SETMASK
#define MIC_RIGHT_SOURCE		   		HW_MIXRECSELR_SR_MIC_SETMASK

#define LINE1_LEFT_SOURCE		    	HW_MIXRECSELR_SL_LINEIN1_SETMASK
#define LINE1_RIGHT_SOURCE		   		HW_MIXRECSELR_SR_LINEIN1_SETMASK

//define FM as line-in 1 for 100 pin package, FM or line-in2 for 144 pin package
#ifdef FMTUNER_ON_LINE2_IN
	#define FM_LEFT_SOURCE		    	HW_MIXRECSELR_SL_LINEIN2_SETMASK
	#define FM_RIGHT_SOURCE		   		HW_MIXRECSELR_SR_LINEIN2_SETMASK
#else
	#define FM_LEFT_SOURCE		    	HW_MIXRECSELR_SL_LINEIN1_SETMASK
	#define FM_RIGHT_SOURCE		   		HW_MIXRECSELR_SR_LINEIN1_SETMASK
#endif

// This default gain from the ADC input amp is maxed at +22.5 dB
// STMP35xx family: mic recording gets max gain at this pre-ADC gain stage
#define MAX_ADC_GAIN_SETMASK            (HW_MIXADCGAINR_GL_22P5_SETMASK|HW_MIXADCGAINR_GR_22P5_SETMASK)
#define CURRENTADCGAIN             		MAX_ADC_GAIN_SETMASK

#define	DIGITALGAINFORMP3RECORD	        0x80 // 3420/3560 GAIN

extern int g_iEncTotalSeconds;
extern int g_iEncCurrentSeconds;

extern volatile int g_bEncoderLowSpace; 

_reentrant int InitRecorderStateMachine(int a, int b, int *c);
_reentrant int HandleRecorderStateMachine(int a, int b, int *c);
_reentrant int ExitRecorderStateMachine(int a, int b, int *c);

#endif
