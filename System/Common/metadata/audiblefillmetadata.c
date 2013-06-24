/*----------------------------------------------------------------------------*\
	File:	audiblefillmetadata.c

	Description:
		The main routine here is AudibleFillMetadata(). This routine is used
		to fill in the SigmaTel FILE_META_DATA data structure from Audible's
		CAudibleFileInfoEmbedded data structure. We also select the decoder
		here instead of the top level routine.

		AudibleFillMetadata() is intended for use as a code bank.
\*----------------------------------------------------------------------------*/

#include "AudibleFillMetadata.h"
#include "metadata.h"
#include "resource.h"
#include "AAMetadataOverlay.h"
#include "AudCStandFunctions.h"


extern unsigned int _X	g_bAudibleCurrentChapterNum, g_bAudibleTotalChapterCount;
extern unsigned int _Y g_wSongCurrentMinutes, g_wSongCurrentSeconds;
extern	WORD	g_wBitRateFound;
extern	g_wBitRate;
extern	g_wSamplingRate;



RETCODE _reentrant AudibleFillMetadata( INT MetaDataArg, INT fileInfoArg, INT * pUnused )
{
	unsigned int				currentTimeSeconds;
	FILE_META_DATA				*MetaData = (FILE_META_DATA *) MetaDataArg;
	CAudibleFileInfoEmbedded	*fileInfo = (CAudibleFileInfoEmbedded *) fileInfoArg;
	int							pInFile;

	// Now that we have metadata, we can set file type and resource number
	switch ( fileInfo->codec.codecID ) {
		case AUDIBLE_CODEC_MP332_MONO:				// Format 4
		    MetaData->iFileType = (INT)AUDIBLE_MP3_TYPE;
			MetaData->iResourceNum = RSRC_AUDIBLE_DECMOD_CODE;
			break;

		case AUDIBLE_CODEC_ACELP8_5:				// Format 2
		case AUDIBLE_CODEC_ACELP16:					// Format 3
		    MetaData->iFileType = (INT)AUDIBLE_ACELP_TYPE;
		    MetaData->iResourceNum = RSRC_AUDIBLE_ACELPDEC_CODE;
			break;

		default:
			return META_DATA_NOT_FOUND;
	}

	// Don't worry about filling in the title and author (artist) strings here
	// anymore. It's handled in GetAudibleMetaData().

	MetaData->bVBR			= FALSE;
	MetaData->wSampRate		= fileInfo->codec.pcmSamplingRate;
	MetaData->wBitRate		= (fileInfo->codec.averageBitRate + 999) / 1000;		// wBitRate is kbps - round up to nearest 1000
	MetaData->iTime			= fileInfo->audio.totalAudioTimeMS / 1000;
	MetaData->dwDataBytes	= fileInfo->audio.totalAudioSizeBytes;
	MetaData->dwNumBytes	= fileInfo->audio.totalAudioSizeBytes;
	MetaData->dwNumFrames	= 0;

	g_bAudibleTotalChapterCount = fileInfo->audio.audioChapterCount;
	g_wBitRateFound = MetaData->wBitRate;
	g_wSamplingRate = fileInfo->codec.pcmSamplingRate;
	g_wBitRate = fileInfo->codec.averageBitRate;
	
	// Get current chapter number using song position 
	if(MetaData->dwStartPos > MetaData->dwDataBytes) {
		MetaData->dwStartPos = 0L;
		g_bAudibleCurrentChapterNum = 1;			// If startPos out of range, start over.
	} else {
		for(g_bAudibleCurrentChapterNum = 0; g_bAudibleCurrentChapterNum < fileInfo->audio.audioChapterCount; g_bAudibleCurrentChapterNum++) {
			if(MetaData->dwStartPos < fileInfo->audio.pAudioChaptersOffsets[g_bAudibleCurrentChapterNum])
				break;
		}
	}

	// Update current seconds and minutes using song position
	currentTimeSeconds = (MetaData->dwStartPos*8)/fileInfo->codec.averageBitRate;
	g_wSongCurrentMinutes = currentTimeSeconds/60;
	g_wSongCurrentSeconds = currentTimeSeconds%60;

	return META_DATA_FOUND;
}
