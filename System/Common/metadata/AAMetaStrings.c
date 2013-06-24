/******************************************************************************
*                                                                             *
*                (C) Copyright Audible Inc. (1997-2005)                       *
*                         All Rights Reserved                                 *
*    This material is confidential and shall remain as such. Any unauthorized *
*   use, distribution, reproduction or storage of this material or any part   *
*   thereof is strictly prohibited.                                           *
******************************************************************************/

/*---------------------------------------------------------------------------*\
	File:	AAMetaStrings.c
	
	Description:
		The routines in this module retrieve metadata strings from an AA
		file. These strings are mined individually instead of all at once
		for systems that are very tight on memory. Note that native file
		system calls are used instead of the normal Audible Input Stream.
\*---------------------------------------------------------------------------*/

#include "fstypes.h"
#include "fsapi.h"
#include "stringlib.h"				// SigmaTel-specific
#include "FileSystem.h"				// SigmaTel-specific
#include "AAMetaStrings.h"
#include "CoreEncryption.h"
#include "CircularBuffer.h"
#include "IAACodec.h"
#include "AAFileInternalCommon.h"
//#include <string.h>


#ifndef	MIN
	#define	MIN(x,y)	(((x) < (y)) ? (x) : (y))
#endif


#define	rewind(p)	Fseek(p, 0, SEEK_SET)



/******** DUPLICATED ROUTINES START ********/

/*
	Description:
		Actually, this isn't a duplicated routine. It's just my implementation
		of the standard fread which reads data into an unpacked array of
		chars.
*/

#define	kTempBufSize	60		// In bytes
static size_t _reentrant fread( void *pBuffer, size_t elemSize, size_t numElem, int fHandle )
{
	int		tempBuf[(kTempBufSize + 2) / 3];
	size_t	bytesRemaining = elemSize * numElem;
	int		bytesRead = 0;
	size_t	totalBytesRead = 0;

	while ( bytesRemaining ) {
		bytesRead = Fread( fHandle, tempBuf, (bytesRemaining > kTempBufSize) ? kTempBufSize : bytesRemaining, Y_MEMORY, -1 );
		unpack_data( (_packed BYTE _Y *) tempBuf, (BYTE _Y *) pBuffer, bytesRead );
		pBuffer = (int *) pBuffer + bytesRead;
		totalBytesRead += bytesRead;
		bytesRemaining -= bytesRead;
	}

	return totalBytesRead;
}

/******** DUPLICATED ROUTINES END ********/



#define	ReadULong(f,p)		ReadLong(f, (long *) p)
#define	ReadUShort(f,p)		ReadShort(f, (short *) p)


/*---------------------------------------------------------------------------*\
	Name:	ReadLong
	
	Description:
		Read a 32-bit value from the file and byte swap it for little-endian
		systems.
\*---------------------------------------------------------------------------*/

static int	SwizzleInt(int value)
{
	return ((value >> 16) & 0x0000FF) | (value & 0x00FF00) | ((value << 16) & 0xFF0000);
}

static size_t _reentrant ReadLong( int pInFile, long * pValue )
{
	size_t	n;
	int		temp[2];

	temp[0] = temp[1] = 0;
	n = Fread( pInFile, temp, 4, Y_MEMORY, -1 );
	*pValue = ((unsigned long) SwizzleInt(temp[0]) << 8) | temp[1];
	
	return n;
}



/*---------------------------------------------------------------------------*\
	Name:	ReadShort
	
	Description:
		Read a 16-bit value from the file and byte swap it for little-endian
		systems.
\*---------------------------------------------------------------------------*/

static size_t _reentrant ReadShort( int pInFile, short * pValue )
{
	size_t	n;
	int		temp;

	temp = 0;
	n = Fread( pInFile, &temp, 2, Y_MEMORY, -1 );
	*pValue = (SwizzleInt(temp) >> 8) & 0x00FFFF;
	
	return n;
}



/*---------------------------------------------------------------------------*\
	Name:	ReadString
	
	Description:
		This routine reads a string from the file for either the specified
		string length or buffer size, whichever is less. The string is then
		NUL-terminated. This is really just a buffer read since it doesn't
		try to identify NUL within the string being read from the file.
\*---------------------------------------------------------------------------*/

static int _reentrant ReadString( int pInFile, int strLength, char * strBuf, int strBufSize )
{
	size_t	n;
	
	n = fread( strBuf, 1, MIN( strLength, strBufSize - 1 ), pInFile );
	strBuf[n] = '\0';
	
	return n;
}



/*---------------------------------------------------------------------------*\
	Name:	ValidateAAFile
	
	Description:
		This is really the same as ValidateAAFileHeader(). It was copied
		here and ported for this specific module. Do we need to validate the
		file every time?
\*---------------------------------------------------------------------------*/

static AudErrorCode _reentrant ValidateAAFile( int pInFile )
{
	long	actualFileSize;
	long	fileSize;
	long	magicNumber;
	
	// Get file size here.
	actualFileSize = FSFileSize( pInFile );
	rewind( pInFile );

	// Read stored file size and Audible-specific magic number
	ReadLong( pInFile, &fileSize );
	ReadLong( pInFile, &magicNumber );
	if ( actualFileSize != fileSize )
		return AUD_AA_FILE_TRUNCATED;
	if ( magicNumber != 0x57907536 )
		return AUD_NOT_AUDIBLE_FILE;

	// Exit leaving file pointer directly after the magic number
	return AUD_NO_ERROR;
}



static AudErrorCode _reentrant FindSection( int pFile, long section, long *pSectionLength)
{
	int		i;
	long	totalSections;
	long	sectionNumber;
	long	sectionStart;
	long	junk;
	
	// Make sure we start after the validation info
	Fseek( pFile, 8, SEEK_SET );

	// Get the number of sections
	ReadLong( pFile, &totalSections );
	ReadLong( pFile, &junk); // skip info
		
	// Iteratate through all sections
	for ( i = 0; i < totalSections; i++ ) {
		// Read section number
		ReadLong( pFile, &sectionNumber );
		ReadLong( pFile, &sectionStart );
		ReadLong( pFile, pSectionLength );
		
		// If the section number is not for metadata strings, let's advance to the next section
		if ( sectionNumber == section ) {			
			// Set the file pointer to the start of the section
			Fseek( pFile, sectionStart, SEEK_SET );
			return AUD_NO_ERROR;
		}
	}
	
	// If we get through all sections, the section wasn't found.
	return 	AUD_AUDIO_END_OF_FILE_REACHED;
}



/*---------------------------------------------------------------------------*\
	Name:	FindName
	
	Description:
		Helper function to find the metadata string name and its value. Once
		the filepointer has been set to the Optional Header, this routine
		reads through that section to look for that value pair. For example,
		to look for the title, set the following parameters:
			pMetaName		"title"
			pTitleBuf		buffer for title
			
	Returns:
		0		If metadata string was found
		non-0	Otherwise
\*---------------------------------------------------------------------------*/

static int _reentrant FindName( int pInFile, char * pMetaStrBuf, size_t iMetaStrBufSize, int sectionLength, char * pMetaName )
{
	long	pairCount;
	int		i;
	size_t	n;
	int		totalBytesRead = 0;
	int		isValuePresent = 0;		// This initialization is necessary
	long	nameLength;
	long	valueLength;
	long	nextPos;
	char	nameStr[32];

	ReadLong( pInFile, &pairCount );
	for ( i = 0; i < pairCount; i++ ) {
		totalBytesRead += fread( &isValuePresent, 1, 1, pInFile );
		if ( isValuePresent )
			break;						// Invalid file
		totalBytesRead += ReadLong( pInFile, &nameLength );
		totalBytesRead += ReadLong( pInFile, &valueLength );
		nextPos = Ftell( pInFile ) + nameLength + valueLength;

		// Read the name
		n = ReadString( pInFile, nameLength, nameStr, iMetaStrBufSize );
		totalBytesRead += n;
		if ( _strcmp( nameStr, pMetaName ) ) {
			if ( totalBytesRead >= sectionLength )
				break;
			Fseek( pInFile, nextPos, SEEK_SET );
			continue;
		}
		
		// Now read the value and return
		ReadString( pInFile, valueLength, pMetaStrBuf, iMetaStrBufSize );

		return 0;
	}

	// Since we use the metadata string for temp storage, if we didn't find a string,
	// make sure we null it out.
	pMetaStrBuf[0] = '\0';

	return -1;				// Metadata string not found
}



/*---------------------------------------------------------------------------*\
	Name:	AAGetMetaString
	
	Description:
		This routine is a generic "get metadata string" routine. Here's
		what it does:
			* validate the file as an AA file
			* read the number of sections
			* for each section, look for the Optional Header
			* if the Optional Header is found, look for the specified
				metadata string

		This API may be used if you know (or want to know) the names of the
		metadata name pairs. If you don't know or care, use the convenience
		APIs. (e.g. AAMetaGetTitle())
\*---------------------------------------------------------------------------*/

AudErrorCode _reentrant AAGetMetaString( int pInFile, char * pMetaStrBuf, size_t iMetaStrBufSize, char * pMetaName )
{
	int				i;
	long			sectionLength;
	AudErrorCode	err;
	
	if ( pMetaStrBuf ) {
		// Validate that we're looking at an Audible file
		if ( (err = ValidateAAFile( pInFile )) != AUD_NO_ERROR )
			return err;
			
		// Skip some data for now
		Fseek( pInFile, 8, SEEK_SET );

		err = FindSection( pInFile, AUD_FILE_OPTIONAL_HEADER, &sectionLength );
		if (err)
			return err;

		// Let's look for the metadata string now
		if ( !FindName( pInFile, pMetaStrBuf, iMetaStrBufSize, sectionLength, pMetaName ) )
			return AUD_NO_ERROR;
	}

	// If we get through all sections, the metadata string wasn't found.
	return -1;
}



/*---------------------------------------------------------------------------*\
	The following APIs retrieve individual pieces of metadata. Strings are
	filled in with code instead of string constants to avoid generating
	.yconst* segments.
\*---------------------------------------------------------------------------*/

AudErrorCode _reentrant AAGetMetaTitle( int pInFile, int iTitleBufSize, char * pTitleBuf )
{
	char	titleStr[12];
	char	*p = titleStr;

	*p++ = 's';
	*p++ = 'h';
	*p++ = 'o';
	*p++ = 'r';
	*p++ = 't';
	*p++ = '_';
	*p++ = 't';
	*p++ = 'i';
	*p++ = 't';
	*p++ = 'l';
	*p++ = 'e';
	*p   = '\0';

	return AAGetMetaString( pInFile, pTitleBuf, iTitleBufSize, titleStr );
}

AudErrorCode _reentrant AAGetMetaAuthor( int pInFile, int iAuthorBufSize, char * pAuthorBuf )
{
	char			authorStr[7];
	char			*p = authorStr;
	char			*pBuf = (char *) pAuthorBuf;
	AudErrorCode	err;

	*p++ = 'a';
	*p++ = 'u';
	*p++ = 't';
	*p++ = 'h';
	*p++ = 'o';
	*p++ = 'r';
	*p   = '\0';

	err = AAGetMetaString( pInFile, pAuthorBuf, iAuthorBufSize, authorStr );
	if ( pBuf && (pBuf[0] == '\0') ) {
		pBuf[0] = 'N';
		pBuf[1] = 'o';
		pBuf[2] = ' ';
		_strcpy( &pBuf[3], authorStr );
	}

	return err;
}

AudErrorCode _reentrant AAGetMetaProductID( int pInFile, int iProdIDBufSize, char * pProdIDBuf )
{
	char	prodIDStr[11];
	char	*p = prodIDStr;

	*p++ = 'p';
	*p++ = 'r';
	*p++ = 'o';
	*p++ = 'd';
	*p++ = 'u';
	*p++ = 'c';
	*p++ = 't';
	*p++ = '_';
	*p++ = 'i';
	*p++ = 'd';
	*p   = '\0';

	return AAGetMetaString( pInFile, pProdIDBuf, iProdIDBufSize, prodIDStr );
}

AudErrorCode _reentrant AAGetMetaHeaderKey( int pInFile, int iHeaderKeyBufSize, char * pHeaderKeyBuf )
{
	char	keyStr[10];
	char	*p = keyStr;

	*p++ = 'H';
	*p++ = 'e';
	*p++ = 'a';
	*p++ = 'd';
	*p++ = 'e';
	*p++ = 'r';
	*p++ = 'K';
	*p++ = 'e';
	*p++ = 'y';
	*p   = '\0';

	return AAGetMetaString( pInFile, pHeaderKeyBuf, iHeaderKeyBufSize, keyStr );
}

AudErrorCode _reentrant AAGetMetaHeaderSeed( int pInFile, int iHeaderSeedBufSize, char * pHeaderSeedBuf )
{
	char	seedStr[11];
	char	*p = seedStr;

	*p++ = 'H';
	*p++ = 'e';
	*p++ = 'a';
	*p++ = 'd';
	*p++ = 'e';
	*p++ = 'r';
	*p++ = 'S';
	*p++ = 'e';
	*p++ = 'e';
	*p++ = 'd';
	*p   = '\0';

	return AAGetMetaString( pInFile, pHeaderSeedBuf, iHeaderSeedBufSize, seedStr );
}

AudErrorCode _reentrant AAGetMetaEncBlocks( int pInFile, int iEncBlocksBufSize, char * pEncBlocksBuf )
{
	char	encBlocksStr[16];
	char	*p = encBlocksStr;

	*p++ = 'E';
	*p++ = 'n';
	*p++ = 'c';
	*p++ = 'r';
	*p++ = 'y';
	*p++ = 'p';
	*p++ = 't';
	*p++ = 'e';
	*p++ = 'd';
	*p++ = 'B';
	*p++ = 'l';
	*p++ = 'o';
	*p++ = 'c';
	*p++ = 'k';
	*p++ = 's';
	*p   = '\0';

	return AAGetMetaString( pInFile, pEncBlocksBuf, iEncBlocksBufSize, encBlocksStr );
}

/*
AudErrorCode _reentrant AAGetMetaCPUType( int pInFile, int iCPUTypeBufSize, char *pCPUTypeBuf )
{
	char	CPUTypeStr[8];
	char	*p = CPUTypeStr;

	*p++ = 'C';
	*p++ = 'P';
	*p++ = 'U';
	*p++ = 'T';
	*p++ = 'y';
	*p++ = 'p';
	*p++ = 'e';
	*p   = '\0';

	return AAGetMetaString( pInFile, pCPUTypeBuf, iCPUTypeBufSize, CPUTypeStr );
}
*/


AudErrorCode _reentrant AAGetMetaCodecID( int pFile, int iUnused, unsigned short * pCodecID )
{
	AudErrorCode err;
	unsigned long junk;
	unsigned short shortJunk;

	err = FindSection(pFile, AUD_FILE_TOC_SEC, (long *) &junk);
	if (err)
		return err;

	// get codec id first
	Fseek( pFile, 6*4, SEEK_CUR );		// Skip 6 longs of data (24 bytes)
/*
	ReadULong( pFile, &junk);          // Read count of audio chapters.
    ReadLong( pFile,  (long *) &junk); // 1. Block Count 4 bytes        
	ReadLong( pFile,  (long *) &junk); // 2. Unused 8 bytes        
	ReadLong( pFile,  (long *) &junk);
	ReadLong( pFile,  (long *) &junk); // 3. Audio Size of chapter   4 bytes        
	ReadLong( pFile,  (long *) &junk); // 4. Time lenght of chapter  4 bytes
*/
    ReadUShort(pFile,  &shortJunk);    // 5. Codec ID of chapter     2 bytes
	*pCodecID = shortJunk;

	return AUD_NO_ERROR;
}



static	const	CAudibleFileCodecData	_P	CodecInfo[] = {
	// codecID, averageBitRate, codecFrameSize, codecFramePCMSamples, pcmSamplingRate, pcmBitperSample
	{	-1L,						-1L,	-1L,	-1L,	-1L,	-1L	},		// Default
	{	AUDIBLE_CODEC_G723,			-1L,	-1L,	240,	8000,	16	},
	{	AUDIBLE_CODEC_ACELP8_5,		8444,	19,		144,	8000,	16	},
	{	AUDIBLE_CODEC_ACELP16,		16000,	20,		160,	16000,	16	},
	{	AUDIBLE_CODEC_MP332_MONO,	32000,	104,	576,	22050,	16	},
	{	AUDIBLE_CODEC_HAC16,		16000,	-1,		1024,	16000,	16	}
};

static void _reentrant FillCodecInfoFromInternalData( unsigned long audioSizeBytes, unsigned long  audioTimeSeconds, CAudibleFileCodecData *pCodecInfo )
{
	int i;

	for ( i = 1; i < (int) (sizeof( CodecInfo ) / sizeof( CodecInfo[0] )); i++ ) {
		if ( CodecInfo[i].codecID == pCodecInfo->codecID ) {
			*pCodecInfo = CodecInfo[i];
			if ( pCodecInfo->codecID == AUDIBLE_CODEC_G723 )
				pCodecInfo->averageBitRate = (audioSizeBytes * 8 / audioTimeSeconds ) * 100 / 99;  // 100 / 99 - adjustment since our time stamp table has seconds of 990 ms

			return;
		}
	}

	// Default
	*pCodecInfo = CodecInfo[0];
	pCodecInfo->averageBitRate = (audioSizeBytes * 8 / audioTimeSeconds);
}



AudErrorCode _reentrant AAGetMetaCodec( int pFile, int iUnused, CAudibleFileCodecData * pCodecInfo )
{
	AudErrorCode	err;

	// Do a little error checking
	if ( (pFile < 0) || !pCodecInfo )
		return AUD_INVALID_INPUT_PARAM;

	err = AAGetMetaCodecID( pFile, 0, (unsigned short *) &pCodecInfo->codecID );
	if ( err != AUD_NO_ERROR )
		return err;

	FillCodecInfoFromInternalData( 0, 1, pCodecInfo );

	return AUD_NO_ERROR;
}



AudErrorCode _reentrant AAGetMetaAudio( int pFile, int iUnused, CAudibleFileAudioData * pAudioInfo )
{
	AudErrorCode err;
	CAudibleFileCodecData codec;
	short shortJunk;
	long junk;
	CAudibleSpecificHeader * pSpecificHead = NULL; // used only for sizeofs
	unsigned long timeSec;
//	CAudibleFileAudioData * pAudioInfo = (CAudibleFileAudioData *) audioInfo;

	// Do a little error checking
	if ( (pFile < 0) || !pAudioInfo )
		return AUD_INVALID_INPUT_PARAM;

	err = AAGetMetaCodec( pFile, 0, &codec );
	if (err) return err;

	// get audio information
	err = FindSection(pFile, AUD_FILE_AUDIBLE_SPEC_HEADER, &junk);
	if (err) return err;
    ReadUShort( pFile, &shortJunk);
	junk = sizeof( pSpecificHead->szTitleID ) + sizeof( pSpecificHead->digitalSignature );
	Fseek( pFile, junk, SEEK_CUR );
	
    ReadULong( pFile, &pAudioInfo->totalAudioSizeBytes);
	timeSec = pAudioInfo->totalAudioSizeBytes * 8 / codec.averageBitRate;
	pAudioInfo->totalAudioTimeMS  = timeSec * 1000 + ( ( ( (pAudioInfo->totalAudioSizeBytes * 8) % codec.averageBitRate) * 1000 ) / codec.averageBitRate );

	// get chapter count
	err = FindSection(pFile, AUD_FILE_TOC_SEC, &junk);
	ReadULong( pFile, &pAudioInfo->audioChapterCount  ); // Read count of audio chapters.
    
	// get partial information
	err = FindSection(pFile, AUD_FILE_COMMON_HEADER, &junk);
	if (err) return err;
	Fseek( pFile, 8, SEEK_CUR );	// Skip version number (2 bytes), min version number (2), and magic number (4)
/*
    ReadShort( pFile, &shortJunk ); // version number 
    ReadShort( pFile, &shortJunk ); // min version number
    ReadLong ( pFile, &junk);		// magic number
*/
    ReadLong( pFile, &junk);		// is partial file
	if (junk == 1) {
		pAudioInfo->IsPartialFile = TRUE;
		ReadLong( pFile, (long *) &pAudioInfo->origFileAudioSizeByte ); // size of original
		ReadLong( pFile, (long *) &pAudioInfo->segmentStartByte ); // segment start
		ReadLong( pFile, (long *) &pAudioInfo->segmentEndByte );   // segment end
	} else { // not a partial file
		pAudioInfo->IsPartialFile = FALSE;
		pAudioInfo->origFileAudioSizeByte  = pAudioInfo->totalAudioSizeBytes;
        pAudioInfo->segmentStartByte       = 0;
        pAudioInfo->segmentEndByte         = pAudioInfo->totalAudioSizeBytes;
	}

	return err;
}



AudErrorCode _reentrant AAGetMetaCommon( int pFile, int iUnused, CAudibleCommonHeader * pCommonHead )
{
	AudErrorCode			err;
	long					sectionLength;
	size_t					n;
//	CAudibleCommonHeader	*pCommonHead = (CAudibleCommonHeader *) commonHead;

	// Do a little error checking
	if ( (pFile < 0) || !pCommonHead )
		return AUD_INVALID_INPUT_PARAM;

	// Go to the Audible common header. If not found, exit.
	err = FindSection(pFile, AUD_FILE_COMMON_HEADER, (long *) &sectionLength);
	if (err)
		return err;

	// Check size		
	if ( sectionLength != 24 )
		return AUD_AA_FILE_BAD_FORMAT;

	// We definitely need the version number to enable decrypt of player and group IDs.
	n  = ReadShort( pFile, &pCommonHead->versionNumber );
	n += ReadShort( pFile, &pCommonHead->minVersionNumber );
/*
	ReadLong ( pFile, &pCommonHead->magicNumber      ) &&
	//------------ Read information if this is partial file from larger one.   
	ReadLong( pFile, &pCommonHead->IsPartialFile )     &&
	ReadLong( pFile, &pCommonHead->sizeOfOriginalAudio ) &&
	ReadLong( pFile, &pCommonHead->segmentStart )      &&
	ReadLong( pFile, &pCommonHead->segmentEnd );       
	//----------------  
	if ( !readRes )
	{ return AUD_FILE_READ_FAIL;
	}
	if ( pCommonHead->magicNumber != 0x57907536 || pCommonHead->minVersionNumber != 0 )
	{ return AUD_AA_FILE_BAD_FORMAT;
	}
	//----------- If version more than 1 - means some new future format
	if ( pCommonHead->versionNumber > 1 || pCommonHead->versionNumber < 0 )
	{ return AUD_AA_FILE_UNSOPPORTED_FORMAT;
	}
	return AUD_NO_ERROR;
	}
*/

	return AUD_NO_ERROR;
}



AudErrorCode _reentrant AAGetMetaAudibleSpecificHeader( int pFile, int iUnused, CAudibleSpecificHeader * pSpecificHead )
{
	AudErrorCode	err;
	long			sectionLength;
	size_t			n;
//	CAudibleSpecificHeader	*pSpecificHead = (CAudibleSpecificHeader *) specificHead;

	// Do a little error checking
	if ( (pFile < 0) || !pSpecificHead )
		return AUD_INVALID_INPUT_PARAM;

	// Go to the Audible specific header. If not found, exit.
	err = FindSection(pFile, AUD_FILE_AUDIBLE_SPEC_HEADER, (long *) &sectionLength);
	if (err)
		return err;

	// At this point, we're at the Audible specific header.
	n =  ReadUShort( pFile, &pSpecificHead->hashTableVersion );
	n += fread( pSpecificHead->szTitleID, 1, sizeof( pSpecificHead->szTitleID ), pFile );
	n += fread( pSpecificHead->digitalSignature, 1, sizeof( pSpecificHead->digitalSignature ), pFile );
	n += ReadULong( pFile, &pSpecificHead->audioSizeBytes );
	n += ReadULong( pFile, &pSpecificHead->audioTimeSeconds );
	n += ReadULong( pFile, &pSpecificHead->hashBlockSize );
	n += ReadULong( pFile, &pSpecificHead->unused );

	if ( n != 18 + sizeof(pSpecificHead->szTitleID) + sizeof(pSpecificHead->digitalSignature) )
		return AUD_FILE_READ_FAIL;

	return AUD_NO_ERROR;
}



static AudErrorCode _reentrant AAGetMetaIDList( int pFile, CAudibleGroupOrPlayer_IDList * pIDList, int listID )
{
	int				i;
	long			temp;
	AudErrorCode	err;
	long			sectionLength;

	// Do a little error checking
	if ( (pFile < 0) || !pIDList )
		return AUD_INVALID_INPUT_PARAM;

	// Go to the Audible specific header. If not found, exit.
	err = FindSection(pFile, listID, (long *) &sectionLength);
	if (err)
		return err;

	// At this point, we're at the player ID list header.
	if ( ReadLong( pFile, &temp ) != 4 )
		return AUD_FILE_READ_FAIL;

	pIDList->idCount = temp;

	//---------------- Size of section should be equal to 4 + pIDList->idCount * 4
	if ( sectionLength != 4 + pIDList->idCount * 4 )
		return AUD_AA_FILE_BAD_FORMAT;

	pIDList->pIDsArray = pIDList->idList;
	if ( pIDList->idCount >= sizeof( pIDList->idList ) / sizeof( pIDList->idList[0] ) )
		pIDList->idCount = sizeof( pIDList->idList ) / sizeof( pIDList->idList[0] );

/*
	//--------- If count is less than fits to embedded array - then we leave it inside, otherwise dynamically allocate  
	if ( pIDList->idCount < sizeof( pIDList->idList ) / sizeof( pIDList->idList[0] ) )
	{
		pIDList->pIDsArray = pIDList->idList;
	}
	else
	{
		pIDList->pIDsArray = (unsigned long *)Audible_malloc( pIDList->idCount );
		if ( pIDList->pIDsArray == NULL )
		{
			return AUD_MEM_ALLOC_FAILED;
		}
	}
*/

	//---------- Now read values one by one
	for ( i = 0; i < pIDList->idCount; i++ ) { 
		if ( ReadULong( pFile, (unsigned long *) &temp ) != 4 )
			return AUD_FILE_READ_FAIL;
		pIDList->pIDsArray[i] = temp;
	}

	return AUD_NO_ERROR;
}



AudErrorCode _reentrant AAGetMetaPlayerIDList( int pFile, int iUnused, CAudibleGroupOrPlayer_IDList * pIDList )
{
	return AAGetMetaIDList( pFile, pIDList, AUD_FILE_PLAYERID_SEC );
}



AudErrorCode _reentrant AAGetMetaGroupIDList( int pFile, int iUnused, CAudibleGroupOrPlayer_IDList * pIDList )
{
	return AAGetMetaIDList( pFile, pIDList, AUD_FILE_GROUPID_SEC );
}



/*---------------------------------------------------------------------------*\
	Name:	AAGetMetaTOC

	Description:
		This routine gets the chapter count and offsets from the TOC header.
		Not all data is read. The resulting data is enough for metadata
		purposes but not enough for opening an AA file via
		AudibleAAOpenFile().
\*---------------------------------------------------------------------------*/

AudErrorCode _reentrant AAGetMetaTOC( int pFile, int iUnused, CAudibleFileAudioData *pAudioInfo )
{
	AudErrorCode		err;
	long				sectionLength;
	int					i;
	unsigned int		totalBlockCount;
	unsigned long		audioSizeSum;

	// Do a little error checking
	if ( (pFile < 0) || !pAudioInfo )
		return AUD_INVALID_INPUT_PARAM;

	// Go to the Audible specific header. If not found, exit.
	err = FindSection( pFile, AUD_FILE_TOC_SEC, (long *) &sectionLength );
	if ( err )
		return err;

	// At this point, we're right at the beginning of the TOC
	//---- AA file consist of multiple sections, each section - multiple blocks.
	if ( ReadULong( pFile, &pAudioInfo->audioChapterCount ) != 4 )
		return AUD_FILE_READ_FAIL;

	//---- Check for valid chapter count
	if ( pAudioInfo->audioChapterCount > AUDIO_CHAPTERS_MAX_COUNT )
		return AUD_AA_FILE_BAD_FORMAT;

	//----------- Good, now iterate through all sections.
	audioSizeSum = 0;
	for ( i = 0; i < pAudioInfo->audioChapterCount; i++ ) {
		long	entriesCount;
		long	audioSize;

		pAudioInfo->pAudioChaptersOffsets[i] = audioSizeSum;

		Fseek(      pFile, 12, SEEK_CUR );
		ReadLong(   pFile, &audioSize );
		audioSizeSum += audioSize;
		Fseek( pFile, 6, SEEK_CUR );
		ReadLong(   pFile, &entriesCount );

		//--------------------- Now we skip time stamp table, as we know the bitrate.
		Fseek( pFile, entriesCount * 8, SEEK_CUR );
	}

	return AUD_NO_ERROR;
}

