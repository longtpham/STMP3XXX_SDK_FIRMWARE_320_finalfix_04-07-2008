/*;;***********************************************************************
;SigmaTel, Inc
;$Archive: $
;$Revision:  $
;$Date: $
;Description: 
;***********************************************************************/

#ifdef ADPCMTEST
#include <stdio.h>
#endif
#include "defs.h"
#include "wavedefs.h"
#include "bitstreamwave.h"


DWORD g_Wave_dwWavDuration;
WORD  g_Wave_wFormatTag;		
WORD  g_Wave_wChannels;		
DWORD g_Wave_dwSamplesPerSec;
DWORD g_Wave_dwAvgBytesPerSec;
WORD  g_Wave_wBlockAlign;	
WORD  g_Wave_wnSamplesPerBlock;
WORD  g_Wave_wBitsPerSample;
DWORD g_Wave_wMegicWord;
long int g_Wave_datachunksize;
long int g_Wave_riffsize = 0;
long int g_Wave_wavesize = 0;
long int g_Wave_datasize = 0;
long int g_Wave_data_offset = 0;
WORD TotalSamplesPerBlock;
int riff_file_flag;
#ifdef IMA_ADPCM
extern int _Y IMA_pred0;
extern int _Y IMA_index0;  
extern int _Y IMA_pred1;
extern int _Y IMA_index1;  
#endif

long int		DataSize[5];		// Stores Nested Chunk sizes. Upto 5 levels allowed
#if defined(MS_ADPCM)|defined(IMA_ADPCM)
WORD  g_Wave_wcbSize;
#endif
#ifdef MS_ADPCM
WORD  g_Wave_wnNumCoef;
short  g_Wave_wCoef1[10];		// Provision for 10 coefficient sets. Signed data.
short  g_Wave_wCoef2[10];
extern int _Y first_block_flag;
// Keep these constants separate, from above buffer
short int g_Wave_iCoef1Array[7] =
{
	256,
	512,
	0,
	192,
	240,
	460,
	392
};
short int g_Wave_iCoef2Array[7] = 
{ 
	0,
	-256,
	0,
	64,
	0,
	-208,
	-232
};

#endif

long int	ckidlist[TOTAL_CKID] =
{
	0x46464952,    // RIFF_CKID
	0x45564157,	   // WAVE_CKID
	0x20746d66,	   // FMT_CKID
	0x74636166,	   // FACT_CKID
	0x61746164	   // DATA_CKID
};

long int list_ckid = 0x5453494c; // LIST
long int info_ckid = 0x4f464e49; // INFO
long int isft_ckid = 0x54465349; // ISFT
short int sigmatel[5] =
{
    0x4953,
    0x4d47,
    0x5441,
    0x4c45,
    0x0
};


#ifdef IMA_ADPCM
WORD  g_Wave_wcbSize;
#endif





#ifdef IMA_ADPCM
void Init_IMA_ADPCM_States(void);
#endif

#ifdef IMA_ADPCM
void Init_IMA_ADPCM_States(void)
{
    IMA_pred0 = 0;
    IMA_index0 = 0;
    IMA_pred1 = 0;
    IMA_index1 = 0;
}    
#endif

#ifdef MS_ADPCM
void Init_MS_ADPCM_States(void);
#endif


#ifdef MS_ADPCM
void Init_MS_ADPCM_States(void)
{
    first_block_flag=1;
}    
#endif
