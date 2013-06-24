extern DWORD g_Wave_dwWavDuration;
extern WORD  g_Wave_wFormatTag;		
extern WORD  g_Wave_wChannels;		
extern DWORD g_Wave_dwSamplesPerSec;
extern DWORD g_Wave_dwAvgBytesPerSec;
extern WORD  g_Wave_wBlockAlign;	
extern DWORD g_Wave_wMegicWord;	
extern long int g_Wave_datachunksize;
extern long int g_Wave_riffsize;
extern long int g_Wave_wavesize;
extern long int g_Wave_datasize;
extern long int g_Wave_data_offset;
extern long int	ckidlist[TOTAL_CKID];
extern long int DataSize[5];		// Stores Nested Chunk sizes. Upto 5 levels allowed
extern WORD TotalSamplesPerBlock;
extern int riff_file_flag;
extern WORD  g_Wave_wBitsPerSample;
extern WORD  g_Wave_wnSamplesPerBlock;
#ifdef MS_ADPCM
extern WORD  g_Wave_wcbSize;
extern WORD  g_Wave_wnNumCoef;
extern short  g_Wave_wCoef1[10];		// Provision for 10 coefficient sets. Signed data.
extern short  g_Wave_wCoef2[10];
// Keep these constants separate, from above buffer
extern short g_Wave_iCoef1Array[7];
extern short g_Wave_iCoef2Array[7]; 
#endif

extern long int list_ckid;
extern long int info_ckid;
extern long int isft_ckid;
extern short int sigmatel[5];


#ifdef IMA_ADPCM
extern WORD  g_Wave_wcbSize;
#endif

// Function Declarations
extern int Initialize_Wave_Header(int formattag, int num_channels, int SamplingRate,int NumberOfBits);
void Write_Wave_Header(void);
void Decode_ADPCM(int FormatTag);
void Encode_ADPCM(int FormatTag, int numChannels);
extern void Init_IMA_ADPCM_States(void);
