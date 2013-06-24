////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2004-2006
//
// File        : smvwave.c
// Description : smv wave
////////////////////////////////////////////////////////////////////////////////
#ifdef ADPCMTEST
#include <stdio.h>
#endif
#include "defs.h"
#include "wavedefs.h"
#include "bitstreamwave.h"


DWORD g_SmvWave_dwWavDuration;
WORD  g_SmvWave_wFormatTag;		
WORD  g_SmvWave_wChannels;		
DWORD g_SmvWave_dwSamplesPerSec;
DWORD g_SmvWave_dwAvgBytesPerSec;
WORD  g_SmvWave_wBlockAlign;	
WORD  g_SmvWave_wnSamplesPerBlock;
WORD  g_SmvWave_wBitsPerSample;
DWORD g_SmvWave_wMegicWord;
long int g_SmvWave_datachunksize;
long int g_SmvWave_riffsize = 0;
long int g_SmvWave_wavesize = 0;
long int g_SmvWave_datasize = 0;
long int g_SmvWave_data_offset = 0;
//WORD SmvTotalSamplesPerBlock;
int smv_riff_file_flag;

extern int _Y SMV_pred0;
extern int _Y SMV_index0;  
extern int _Y SMV_pred1;
extern int _Y SMV_index1;  

long int		SmvDataSize[5];		// Stores Nested Chunk sizes. Upto 5 levels allowed

WORD  g_SmvWave_wcbSize;

long int	smv_ckidlist[TOTAL_CKID] =
{
	0x46464952,    // RIFF_CKID
	0x45564157,	   // WAVE_CKID
	0x20746d66,	   // FMT_CKID
	0x74636166,	   // FACT_CKID
	0x61746164	   // DATA_CKID
};

long int smv_list_ckid = 0x5453494c; // LIST
long int smv_info_ckid = 0x4f464e49; // INFO
long int smv_isft_ckid = 0x54465349; // ISFT
short int smv_sigmatel[5] =
{
    0x4953,
    0x4d47,
    0x5441,
    0x4c45,
    0x0
};


WORD  g_SmvWave_wcbSize;

void Smv_Init_IMA_ADPCM_States(void);

void Smv_Init_IMA_ADPCM_States(void)
{
    SMV_pred0 = 0;
    SMV_index0 = 0;
    SMV_pred1 = 0;
    SMV_index1 = 0;
}    
