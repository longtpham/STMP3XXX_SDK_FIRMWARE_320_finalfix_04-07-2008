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
//#include "AAMetaStrings.h"
#include "AAMTPAccess.h"
#include "CoreEncryption.h"
#include "CircularBuffer.h"
#include "IAACodec.h"
#include "AAFileInternalCommon.h"
#include "resource.h"
//#include "metadata.h"
#include "menumanager.h"
#include "AAMTPSongPosition.h"
//#include <string.h>


#ifndef	MIN
	#define	MIN(x,y)	(((x) < (y)) ? (x) : (y))
#endif


#define	rewind(p)	Fseek(p, 0, SEEK_SET)



/*
	MTP Property Access Routines
*/



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

void _reentrant Pack4Bytes( void _Y * pSource, void _Y * pDestination, WORD wDestinationStart )
{
    int i;
    int N;
    
    //Pack 4 bytes into a buffer
    for(i=0; i<=4; i++)
	{
	    N = GetByteFromArrayInY(pSource, i);
		PutByteInArrayInY(pDestination, wDestinationStart, N);
        wDestinationStart++;
    }
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

static AudErrorCode _reentrant AAGetMetaString( int pInFile, char * pMetaStrBuf, size_t iMetaStrBufSize, char * pMetaName )
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

AudErrorCode _reentrant AAMTPGetTitle( int PackedPathNameAddress, int btCurrentDevice, char * pTitleBuf )
{
	char			titleStr[12];
	char			*p = titleStr;
	int				fp;
	AudErrorCode	err;

	// Do a little error checking
	if ( pTitleBuf == NULL )
		return AUD_INVALID_INPUT_PARAM;

	fp = FSFileOpen( (_packed BYTE *) PackedPathNameAddress, READ, btCurrentDevice );
	if ( fp < 0 )
		return -1;

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

	// Assume max string length of 256 chars.
	err = AAGetMetaString( fp, pTitleBuf, 256, titleStr );
	Fclose( fp );

	return err;
}

AudErrorCode _reentrant AAMTPGetAuthor( int PackedPathNameAddress, int btCurrentDevice, char * pAuthorBuf )
{
	char			authorStr[7];
	char			*p = authorStr;
	char			*pBuf = (char *) pAuthorBuf;
	int				fp;
	AudErrorCode	err;

	// Do a little error checking
	if ( pAuthorBuf == NULL )
		return AUD_INVALID_INPUT_PARAM;

	fp = FSFileOpen( (_packed BYTE *) PackedPathNameAddress, READ, btCurrentDevice );
	if ( fp < 0 )
		return -1;

	*p++ = 'a';
	*p++ = 'u';
	*p++ = 't';
	*p++ = 'h';
	*p++ = 'o';
	*p++ = 'r';
	*p   = '\0';

	// Assume max string length of 256 chars.
	err = AAGetMetaString( fp, pAuthorBuf, 256, authorStr );
	Fclose( fp );
	if ( pBuf && (pBuf[0] == '\0') ) {
		pBuf[0] = 'N';
		pBuf[1] = 'o';
		pBuf[2] = ' ';
		_strcpy( &pBuf[3], authorStr );
	}

	return err;
}

AudErrorCode _reentrant AAMTPGetProductID( int PackedPathNameAddress, int btCurrentDevice, char * pProdIDBuf )
{
	char			prodIDStr[11];
	char			*p = prodIDStr;
	int				fp;
	AudErrorCode	err;

	// Do a little error checking
	if ( pProdIDBuf == NULL )
		return AUD_INVALID_INPUT_PARAM;

	fp = FSFileOpen( (_packed BYTE *) PackedPathNameAddress, READ, btCurrentDevice );
	if ( fp < 0 )
		return -1;

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

	// Assume max string length of 256 chars.
	err = AAGetMetaString( fp, pProdIDBuf, 256, prodIDStr );
	Fclose( fp );

	return err;
}



/*
	Description:
		This routine returns the duration of the specified title in seconds.
*/

AudErrorCode _reentrant AAMTPGetDuration( int PackedPathNameAddress, int btCurrentDevice, int * pDuration )
{
	AudErrorCode				err;
	long						junk;
	CAudibleSpecificHeader *	pSpecificHead = NULL; // used only for sizeofs
	int							fp;

	// Do a little error checking
	if ( !pDuration )
		return AUD_INVALID_INPUT_PARAM;

	fp = FSFileOpen( (_packed BYTE *) PackedPathNameAddress, READ, btCurrentDevice );
	if ( fp < 0 )
		return -1;

	err = FindSection( fp, AUD_FILE_AUDIBLE_SPEC_HEADER, &junk );
	if ( err ) {
		Fclose( fp );
		return err;
	}

	// Skip a bunch of stuff
	junk =		2											// short - Version of hash table
			+	sizeof( pSpecificHead->szTitleID )			// product ID
			+	sizeof( pSpecificHead->digitalSignature )	// digital signature
			+	4;											// total size of audio in bytes
	Fseek( fp, junk, SEEK_CUR );

	// Read the duration in seconds	
	ReadULong( fp, pDuration );

	Fclose( fp );

	return err;
}



/*
	Description:
		This routine returns the codec ID of the specified title. The current
		codec IDs found in AA files are
			ID									Format
			--									------
			0x0001 AUDIBLE_CODEC_G723			Format 1
			0xC010 AUDIBLE_CODEC_ACELP8_5		Format 2
			0xC00C AUDIBLE_CODEC_ACELP16		Format 3
			0xC00D AUDIBLE_CODEC_MP332_MONO		Format 4

		These codec IDs are translated into the MTP property equivalents:
			ID		Format
			--		------
			0		Non-Audible Format
			1		Format 1
			2		Format 2
			3		Format 3
			4		Format 4
*/

AudErrorCode _reentrant AAMTPGetCodecID( int PackedPathNameAddress, int btCurrentDevice, unsigned short * pCodecID )
{
	AudErrorCode	err;
	unsigned long	junk;
	int				fp;
	unsigned short	id;

	fp = FSFileOpen( (_packed BYTE *) PackedPathNameAddress, READ, btCurrentDevice );
	if ( fp < 0 )
		return -1;

	err = FindSection( fp, AUD_FILE_TOC_SEC, (long *) &junk );
	if ( err ) {
		Fclose( fp );
		return err;
	}

	// get codec id first
	Fseek( fp, 6*4, SEEK_CUR );		// Skip 6 longs of data (24 bytes)
    ReadUShort( fp, &id );

	Fclose( fp );

	// Translate from Audible ID to MTP ID
	switch ( id ) {
		case AUDIBLE_CODEC_G723:		*pCodecID = 1;	break;
		case AUDIBLE_CODEC_ACELP8_5:	*pCodecID = 2;	break;
		case AUDIBLE_CODEC_ACELP16:		*pCodecID = 3;	break;
		case AUDIBLE_CODEC_MP332_MONO:	*pCodecID = 4;	break;
		default:						*pCodecID = 0;	break;
	}

	return AUD_NO_ERROR;
}



/*
	Description:
		This routine returns the AudibleTitleSegment MTP property.
*/

AudErrorCode _reentrant AAMTPGetTitleSegment( int PackedPathNameAddress, int btCurrentDevice, UINT128 * pTitleSegment )
{
	long    junk;
	int     fp;  
	TitleSegmentData_t TempTitleSegmentData;
    
    AudErrorCode				err;
	CAudibleSpecificHeader *	pSpecificHead = NULL; // used only for sizeofs
                
	TempTitleSegmentData.IsPartialFile = 0;
    
    fp = FSFileOpen( (_packed BYTE *) PackedPathNameAddress, READ, btCurrentDevice );
	if ( fp < 0 )
		return -1;

	// get partial information
	err = FindSection( fp, AUD_FILE_COMMON_HEADER, &junk );
	if ( err ) {
		Fclose( fp );
		return err;
	}
	Fseek( fp, 8, SEEK_CUR );	// Skip version number (2 bytes), min version number (2), and magic number (4)

    ReadULong( fp, &TempTitleSegmentData.IsPartialFile );

	if ( TempTitleSegmentData.IsPartialFile == 1 ) {
		ReadULong( fp, &TempTitleSegmentData.origFileAudioSizeByte );
		ReadULong( fp, &TempTitleSegmentData.segmentStartByte );
		ReadULong( fp, &TempTitleSegmentData.segmentEndByte );
	}
	else {
		// Not a partial file. Look for total audio size so that we can populate the title segment data structure
		err = FindSection( fp, AUD_FILE_AUDIBLE_SPEC_HEADER, &junk );
		if ( err ) {
			Fclose( fp );
			return err;
		}

		// Skip a bunch of stuff
		junk =		2											// short - Version of hash table
				+	sizeof( pSpecificHead->szTitleID )			// product ID
				+	sizeof( pSpecificHead->digitalSignature );	// digital signature
		Fseek( fp, junk, SEEK_CUR );
		
		// Read the total audio size in bytes
	    ReadULong( fp, &junk );

		TempTitleSegmentData.origFileAudioSizeByte = junk;
        TempTitleSegmentData.segmentStartByte      = 0;
        TempTitleSegmentData.segmentEndByte        = junk;
	}
    //Pack the titlesegment for return
    Pack4Bytes((void _Y *)&TempTitleSegmentData.IsPartialFile,(void _Y *)pTitleSegment, 0);
    Pack4Bytes((void _Y *)&TempTitleSegmentData.origFileAudioSizeByte,(void _Y *)pTitleSegment, 4);
    Pack4Bytes((void _Y *)&TempTitleSegmentData.segmentStartByte,(void _Y *)pTitleSegment, 8);
    Pack4Bytes((void _Y *)&TempTitleSegmentData.segmentEndByte,(void _Y *)pTitleSegment, 12);
      
	Fclose( fp );

	return AUD_NO_ERROR;
}



/*
	Description:
		This routine retrieves the playback position for the specified file.
*/

AudErrorCode _reentrant AAMTPGetPlaybackPosition( int PackedPathNameAddress, int btCurrentDevice, unsigned long * pPlaybackPos )
{
    return AAMTPGetFilePosition( PackedPathNameAddress, btCurrentDevice, pPlaybackPos, NULL );
}



/*
	Description:
		This routine retrieves the played-through indicator flag for the
		specified title.
*/

AudErrorCode _reentrant AAMTPGetPlayedThrough( int PackedPathNameAddress, int btCurrentDevice, unsigned int * pPlayedThrough )
{
    return AAMTPGetFilePosition( PackedPathNameAddress, btCurrentDevice, NULL, pPlayedThrough );
}
