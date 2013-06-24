///////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000

// System defs
///////////////////////////////////////////////////////////////////////////////

#define STOP_ENCODER_IDLE 2

#define SILENCE_DETECTED  1

#define AUDIO_DETECTED    0

#define THRESHOLD_ENERGY  64000

#define SILENCE_PERIOD  3                          // Silence Period = 3 Seconds

#define IDLE_SILENCE_PERIOD 300						//  5 minutes before it goes idle

extern int _Y Mp3Enc_NumChannels;

extern int _Y MP3EncoderBufHeadPtr;

extern int _Y MP3EncoderBufSizePtr;

extern int _Y MP3EncoderBufAddrPtr;

extern int _asmfunc FindEnergy(int BlockSize,int Ptr, int NumChannels,int modulo);

int CheckSilenceBlockCount(void);

