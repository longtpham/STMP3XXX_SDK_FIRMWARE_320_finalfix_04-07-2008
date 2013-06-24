/////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2006
//
// Filename: encoderproperties.h
// Description: Definitions used in manipulating Encoder Properties
/////////////////////////////////////////////////////////////////////////////////

extern _Y WORD g_wEncSamplingRate;
extern _Y WORD g_wEncBitRate;
extern _Y WORD g_wEncADCGain;
extern _Y WORD g_wEncADCSRR;
extern _Y WORD g_wEncInvSampleRateScale;
extern _Y WORD g_wEncNAvgBytesPerSec;
extern _Y WORD g_wEncClusterTimeScale;
extern _X WORD g_wEncoderSR;
extern _Y WORD g_wEncCurrentDeviceNum;
extern _Y WORD g_wEncFormatTag;
extern _Y WORD g_wEncNumberOfChannels;
extern _Y WORD g_wEncPCMBits;
extern _Y _packed BYTE g_EncFileNameString[];
extern _Y WORD g_wSamplingRateIndex;
extern _Y WORD g_wEncSamplingRateIdx;
extern _Y _fract g_wEncADCLChOffset;
extern _Y _fract g_wEncADCRChOffset;
extern _X _fract DCOffsetADC[2][7];

                             // struct passed as argument to EncSetProperties
typedef struct {
  int    device;
  _packed BYTE *pFilename;
  int    samplingRateInHz;
  int    bitRateInKbps;
  int    FormatTag; // Algorithm. 1 = PCMWAVE, 2 = MS ADPCM, 0x11 = IMA ADPCM
  int    EncNumberOfChannels;
  int    EncPCMBits;    // Number of bits for PCMWAVE (8,16,24), else ignored
  int    iSource;     // Note: Not implemeted, yet.
} EncProperties;

typedef struct {
  WORD   wNSamplesPerSec;
  WORD   wADCGain;
  WORD   wADCSRR;
  WORD   wInvSampleRateScale;
  WORD   wNAvgBytesPerSec;
  WORD   wClusterTimeScale;
} EncoderSampleRateParms;

RETCODE _reentrant EncSetProperties(EncProperties *pProps);

// Access functions (macros)

#define EncGetSampleRateInHz()  (g_wEncNSamplesPerSec)


// These properties are not yet implemented

#define EncGetDevice()          (0)
#define EncGetFilename()        ("")
#define EncGetEncodingMethod()  (0)


#define ERROR_ENCODER_INVALID_SAMPLE_RATE ((RETCODE)(-1))
#define ERROR_ENCODER_IS_ACTIVE           ((RETCODE)(-2))






