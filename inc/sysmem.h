//;******************************************************************************
//; Copyright(C) SigmaTel, Inc. 2000-2003
//; File: sysmem.h
//; ST System Memory Externs
//;******************************************************************************

#ifndef SYSMEM_XREF_C
#define SYSMEM_XREF_C

// Variables in X
extern unsigned int _X g_wDecoderCSR;
extern unsigned int _X g_wDecoderSR;
extern unsigned int _X g_wDecoderCSR2;	// DECODE2EOF
extern unsigned int _X g_wEncoderSR;
extern unsigned int _X g_wEncoderCSR;
extern unsigned int _X g_wRecStartTimeHigh;
extern unsigned int _X g_wRecStartTimeLow;
extern unsigned int _X g_wSysError;
#ifdef TRACEBUF_EN
extern unsigned int _X g_wTraceBuffer; //Disabled by default.   
extern unsigned int _X g_wTracePointer;
#endif
extern unsigned int _X g_wUserScratchX[];
extern unsigned int _X g_wNextVoiceNumValue;
extern unsigned int _X g_wEncAdpcmSave_r7;
extern unsigned int _X g_wEncAdpcmSave_m7;
extern unsigned int _X g_wEncModuleState;
extern unsigned int _X g_wEncoderIsrSR;
extern unsigned int _X g_bAudibleNeedFileSize;		// 10/12/04 mmiu - Added for AA file navigation
extern unsigned int _X g_bAudiblePlayThrough;		// 11/1/04 mmiu - Added for file position play-through indicator
extern unsigned int _X g_wSongStartOffsetHigh;		// 11/1/04 mmiu - Added until confirmed we can use g_wSongByteTotalConsumedHigh/Low
extern unsigned int _X g_wSongStartOffsetLow;

// Variables in Y
extern int          _Y g_VolumeBias;
extern unsigned int _Y g_wCurrentRoutinePtr;
extern unsigned int _Y g_wSystemFileHandle;
extern unsigned int _Y g_wSongByteTotalHigh;
extern unsigned int _Y g_wSongByteTotalLow; 
extern unsigned int _Y g_wSongByteLengthHigh;
extern unsigned int _Y g_wSongByteLengthLow;
extern unsigned int _Y g_wInvSampleRate;
extern unsigned int _Y g_wCurrentSongBad;
extern unsigned int _Y g_iSongType;
extern _packed BYTE _Y g_CurrentSongName[];
extern unsigned int _Y g_wCurrentDirDevId;
extern unsigned int _Y g_wSongInvBitRatePtr;
extern unsigned int _Y g_wUserScratchXDescriptor;
extern unsigned int _Y g_wUserScratchYDescriptor;
extern _packed BYTE _Y g_EncFileNameString[];
extern unsigned int _Y g_wEncFileHandle;
extern unsigned int _Y g_wEncAdpcmOvlSave_sp;
extern unsigned int _Y g_wRootDirectory;
extern unsigned int _Y g_wUserScratchY[];
extern unsigned int _Y g_wPrevInvBitRatePtr;

#endif  // SYSMEM_XREF_C
