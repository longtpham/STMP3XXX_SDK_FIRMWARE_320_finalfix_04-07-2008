////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2004-2006
//
// File        : smvwavedata.h
// Description : smvwavedata header file
////////////////////////////////////////////////////////////////////////////////
extern DWORD g_SmvWave_dwWavDuration;
extern WORD  g_SmvWave_wFormatTag;		
extern WORD  g_SmvWave_wChannels;		
extern DWORD g_SmvWave_dwSamplesPerSec;
extern DWORD g_SmvWave_dwAvgBytesPerSec;
extern WORD  g_SmvWave_wBlockAlign;	
extern DWORD g_SmvWave_wMegicWord;	
extern long int g_SmvWave_datachunksize;
extern long int g_SmvWave_riffsize;
extern long int g_SmvWave_wavesize;
extern long int g_SmvWave_datasize;
extern long int g_SmvWave_data_offset;
extern long int	smv_ckidlist[TOTAL_CKID];
extern long int SmvDataSize[5];		// Stores Nested Chunk sizes. Upto 5 levels allowed
//extern WORD SmvTotalSamplesPerBlock;
extern int smv_riff_file_flag;
extern WORD  g_SmvWave_wBitsPerSample;
extern WORD  g_SmvWave_wnSamplesPerBlock;

extern long int smv_list_ckid;
extern long int smv_info_ckid;
extern long int smv_isft_ckid;
extern short int smv_sigmatel[5];


extern WORD  g_SmvWave_wcbSize;

// Function Declarations
extern int Initialize_Wave_Header(int formattag, int num_channels, int SamplingRate,int NumberOfBits);
//void Write_Wave_Header(void);
//void Decode_SMVADPCM(int FormatTag);
//void Encode_ADPCM(int FormatTag, int numChannels);
extern void Smv_Init_IMA_ADPCM_States(void);
