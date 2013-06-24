//;///////////////////////////////////////////////////////////////////////////////
//; Copyright(C) SigmaTel, Inc. 2000-2001
//;
//; Filename: parsermem.xref
//; Description: Global refs from parsermem.asm
//;///////////////////////////////////////////////////////////////////////////////

#ifndef PARESERMEM_XREF_C
#define PARESERMEM_XREF_C

extern unsigned int _X g_wParserCSR;
extern unsigned int _X g_wDecoderType;

extern unsigned int _Y g_wParserFileHandle;
extern unsigned int _Y g_wSongByteLengthHigh;
extern unsigned int _Y g_wSongByteLengthLow;
extern _packed BYTE _Y g_wCurrentSongName[];
extern unsigned int _Y g_wByteoffset;
extern unsigned int _Y g_wDirectorySongNumber;
extern unsigned int _Y g_wDirectorySongCount;
extern unsigned int _Y g_wDev0Count, g_wDev1Count, g_wDev2Count, g_wDev3Count;
extern unsigned int _Y g_wCurrentDirDevId;
extern unsigned int _Y g_wCurrentSongBad;
extern unsigned int _Y g_wSongExtension;
extern unsigned int _Y g_wSongExtensionList;
extern unsigned int _Y g_wMp3Extension;
extern unsigned int _Y g_wWavExtension;
extern unsigned int _Y g_wVoiceExtensionList;
extern unsigned int _Y g_wInvSampleRate;
extern unsigned int _Y g_wShuffleSeed;
extern unsigned int _Y g_wShuffle0to23;
extern unsigned int _Y g_wShuffle24to47;
extern unsigned int _Y g_wTotalSongsPlayed;
extern unsigned int _Y g_wpSadpcmencode,g_wEn_pred,g_wEn_index;          
extern unsigned int _Y g_wRead_byte_state,g_wWrite_byte_state,g_wWav_byte_state;     
extern unsigned int _Y g_wError_flag;
extern unsigned int _Y g_wRecorder_state;
extern unsigned int _Y g_wSample_cntH,g_wSample_cntL,g_wTotal_cntH,g_wTotal_cntL;      
extern unsigned int _Y g_wRiff_ckidH,g_wRiff_ckidL,g_wRiff_chunksizeH,g_wRiff_chunksizeL,g_wCktypeH,g_wCktypeL;          
extern unsigned int _Y g_wWave_ckidH,g_wWave_ckidL;
extern unsigned int _Y g_wWave_chunksize,g_wFormattag,g_wNchannels,g_wNsamplespersec,g_wNavgbytespersec,g_wNblockalign;
extern unsigned int _Y g_wEncInvSampleRateScale;
extern unsigned int _Y g_wBitspersample,g_wCbsize,g_wSamplesperblock;
extern unsigned int _Y g_wEncClusterTimeScale;
extern unsigned int _Y g_wFact_ckidH,g_wFact_ckidL,g_wFact_chunksize,g_wTimeL,g_wTimeH;
extern unsigned int _Y g_wData_ckidH,g_wData_ckidL,g_wData_chunksizeL,g_wData_chunksizeH;  
extern unsigned int _Y g_wWave_header_size;

#endif // ifndef PARESERMEM_XREF_C


