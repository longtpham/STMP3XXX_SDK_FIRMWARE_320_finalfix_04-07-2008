// Supported algorithms
#define 	WAVE_FORMAT_PCM 	    0x01
#define 	WAVE_FORMAT_ADPCM		0x02
#define		WAVE_FORMAT_IMA_ADPCM	0x11
#define		WAVE_FORMAT_G721_ADPCM	0x40

// Supported chunk types
#define		RIFF_CKID		0
#define		WAVE_CKID		1
#define		FMT_CKID		2
#define		FACT_CKID		3
#define		DATA_CKID		4
// Add more Ckids here

#define		UNKNOWN_CKID	5
#define		TOTAL_CKID		5


#define     ADPCM_SOURCE_BUF_SIZE    2048