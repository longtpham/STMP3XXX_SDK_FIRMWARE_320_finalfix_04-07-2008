/* $Header: Z:/projects/Audible/SigmaTel/AudibleOEM/STMP3XXX_SDK_PLAYER_FIRMWARE/System/common/metadata/rcs/audiblesongposition.c 1.4 2005/03/22 21:49:43Z mmiu Exp $ */
/* $Name: SigmaTel_v1.0_RC4 $ */
///////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002
//
// Filename: mp3metadata.c
// Description: Functions for extracting the mp3 meta-data.
//
///////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "platform.h"
#include "stringlib.h"
#include "hwequ.h"
#include "project.h"
#include "FileSystem.h"
#include "metadata.h"
#include "resource.h"
#include "menumanager.h"
#include "fstypes.h"
#include "fsapi.h"
#ifdef   USE_PLAYLIST2
#include "playlist2.h"
#include "playlist2internal.h"
#else
#ifdef   USE_PLAYLIST3
#include "playlist3.h"
#include "playlist3internal.h"
#endif
#endif
#ifdef USE_PLAYLIST5
#include "playlist5.h"
#include "playerlib.h"
#endif
#include "sysmem.h"
#include "audiblesongposition.h"
#include "fserror.h"

#define POSITION_FILE_EXTENTION	0x534F50   // SOP
		UCS3 tempStr[TITLE_SIZE];
_packed BYTE LongFileName[MAX_FILENAME_LENGTH/2];
_packed BYTE pcFilePathName[MAX_FILENAME_LENGTH/2];

void AudibleGetSongPositionFilePathName(INT PackedPathNameAddress, _packed BYTE *filePathName, INT btCurrentDevice);
static int	SwizzleInt(int value);
static BOOL MyReadULong( int hPosFile, unsigned long *value );
static BOOL MyWriteULong( int hPosFile, unsigned long value );
static unsigned long CalculateChecksum( tFilePosition *pFilePosition );

extern unsigned int _X	g_bAudibleConsumedBytesLow, g_bAudibleConsumedBytesHigh;
extern unsigned int _Y Mp3DecoderStartFrameFilePosition;

RETCODE _reentrant AudibleSetFilePosition(INT PackedPathNameAddress, INT btCurrentDevice) {
    INT fOut;
	tFilePosition filePosition;
    INT iFileAttribute;
	INT *p;

	unsigned long tempSongByteLength = ((unsigned long) g_wSongByteLengthHigh << 24) | g_wSongByteLengthLow;

	filePosition.version = 1;
    filePosition.checksum = 0;
    filePosition.filePos = ((unsigned long) g_bAudibleConsumedBytesHigh << 24) | g_bAudibleConsumedBytesLow;

	// If current song position is very close to end of file, set file position to begining of the file
	// 1000 bytes correspond to 0.5 seconds for 8Kpbs
	if(((long)tempSongByteLength - (long)filePosition.filePos) < 1000) {
		filePosition.filePos = 0;
	}

    filePosition.playThroughFlag = g_bAudiblePlayThrough;

	// Get song position file path and name
#ifndef USE_PLAYLIST5
	AudibleGetSongPositionFilePathName(PackedPathNameAddress, pcFilePathName, btCurrentDevice);
#endif
/*
	// mmiu - For SanDisk Sansa e100 series, always get file position from internal Flash
	btCurrentDevice = 0;
    if ( FSGetByte( pcFilePathName, 0, Y_MEMORY ) != '/' )
		PutByte( pcFilePathName, 'a', 0, Y_MEMORY );
*/

	// Load FAT Write code to application space
	SysLoadFATWrite();

	// Open file for write
#ifdef USE_PLAYLIST5
	g_CurrentFastKey = (g_PL5_PL_queue.pos + g_PL5_Playback.currentplay)->file.lFastKey;
	if( (fOut = FSFastOpen(g_CurrentFastKey,WRITE_PLUS) )< 0 ) {
#else
    if( (fOut = FSFileOpen( pcFilePathName, WRITE_PLUS, btCurrentDevice)) < 0 ){
#endif
		SysUnLoadFATWrite();
        return META_DATA_FILEOPEN_ERROR;
    }

	// Calculate checksum
	filePosition.checksum = CalculateChecksum( &filePosition );

	// Write current audible file position
	if (	!MyWriteULong( fOut, filePosition.version )		||
			!MyWriteULong( fOut, filePosition.checksum )	||
			!MyWriteULong( fOut, filePosition.filePos )		||
			!MyWriteULong( fOut, filePosition.playThroughFlag ) ) {
    	FSFileClose(fOut);
		SysUnLoadFATWrite();
		return META_DATA_FILEWRITE_ERROR;
	}

    //Set the hidden and system attribute bits.
    iFileAttribute = filegetattrib (pcFilePathName);
    filesetattrib (fOut, iFileAttribute | ATTR_HIDDEN );

    FSFileClose(fOut);
	FlushCache();
    SysUnLoadFATWrite();

	return META_DATA_SUCCESS;
}

RETCODE _reentrant AudibleGetFilePosition(INT PackedPathNameAddress, INT btCurrentDevice, FILE_META_DATA *MetaData) {
    INT fIn;
	tFilePosition filePosition;

#ifndef USE_PLAYLIST5
	// Get song position file path and name
	AudibleGetSongPositionFilePathName(PackedPathNameAddress, pcFilePathName, btCurrentDevice);
#endif

/*
	// mmiu - For SanDisk Sansa e100 series, always get file position from internal Flash
	btCurrentDevice = 0;
    if ( FSGetByte( pcFilePathName, 0, Y_MEMORY ) != '/' )
		PutByte( pcFilePathName, 'a', 0, Y_MEMORY );
*/

	// Set defaults
	MetaData->dwStartPos				= 0;
	g_bAudiblePlayThrough				= 0;
	Mp3DecoderStartFrameFilePosition	= 0;

	// Open file for read
#ifdef USE_PLAYLIST5
	g_CurrentFastKey = (g_PL5_PL_queue.pos + g_PL5_Playback.currentplay)->file.lFastKey;
	if( (fIn = FSFastOpen(g_CurrentFastKey,READ)) < 0 ) {
#else
    if( (fIn = FSFileOpen( pcFilePathName, READ, btCurrentDevice)) < 0 ) {
#endif
		if(fIn == FILE_NOT_FOUND) {
			return META_DATA_SUCCESS;
		} else {
			return META_DATA_FILEOPEN_ERROR;
		}
    }

	// Read current audible file position
	if (	!MyReadULong( fIn, &filePosition.version )	||
			!MyReadULong( fIn, &filePosition.checksum )	||
			!MyReadULong( fIn, &filePosition.filePos )	||
			!MyReadULong( fIn, &filePosition.playThroughFlag ) ) {
		FSFileClose(fIn);
		return META_DATA_FILEREAD_ERROR;
	}

	// Check checksum. If checksum bad, eat the error and report success. Start at beginning of file.
	if ( CalculateChecksum( &filePosition ) != filePosition.checksum ) {
		FSFileClose( fIn );
		return META_DATA_SUCCESS;
	}

	MetaData->dwStartPos				= filePosition.filePos;
    g_bAudiblePlayThrough				= filePosition.playThroughFlag;
	Mp3DecoderStartFrameFilePosition	= 0;

	FSFileClose(fIn);

	return META_DATA_SUCCESS;
}
#ifdef USE_PLAYLIST5

#else
void AudibleGetSongPositionFilePathName(INT PackedPathNameAddress, _packed BYTE *filePathName, INT btCurrentDevice) {
    WORD wExtension[2], iStrLen;

	// Null terminate
	filePathName[0] = 0;
	packed_strcat(filePathName,(_packed BYTE*)PackedPathNameAddress);

	// Get LFN from CMA
	if( SysCallFunction(RSRC_PLAYLIST_CODEBANK, Playlist_GetLFN, TYPE_FILE, (INT)g_CurrentTrack.pFileEntry,
						(INT*)tempStr) == PLAYLIST_SUCCESS ) {
		iStrLen= packed_strlen((_packed BYTE*)filePathName);

		// Find the last '/'
		while(iStrLen--) {
			// Start at the end, go backward until the char is '/'
			if(packed_get(filePathName,iStrLen)=='/')
				break;
		}

		// Append LFN to PackedPathNameAddress string
		iStrLen++;
		packed_set(filePathName, iStrLen, 0);
		// CMA returns unpacked LFN string, pack this string
		pack_string(tempStr, LongFileName);
		packed_strcat(filePathName, (_packed BYTE*)LongFileName);
	}
	iStrLen= packed_strlen((_packed BYTE*)filePathName);

	// Find the '.'
	while(iStrLen--) {
		// Start at the end, go backward until the char is '.'
		if(packed_get(pcFilePathName,iStrLen)=='.')
			break;
	}

	// Append POS to PackedPathNameAddress string
	iStrLen++;
	packed_set(filePathName, iStrLen, 0);
	wExtension[0] = POSITION_FILE_EXTENTION;
	wExtension[1] = 0;
	packed_strcat(filePathName,(_packed BYTE*)wExtension);
}
#endif


//------------------------------------------------------------------------------
//	Utility Functions
//------------------------------------------------------------------------------
static int	SwizzleInt(int value)
{
	return ((value >> 16) & 0x0000FF) | (value & 0x00FF00) | ((value << 16) & 0xFF0000);
}

/*
	Description:
		This routine reads a 32-bit long from the specified file.
*/
static BOOL MyReadULong( int hPosFile, unsigned long *value )
{
	int	temp[2];

	temp[0] = temp[1] = 0;
	if ( Fread( hPosFile, temp, 4, Y_MEMORY, -1 ) == 4 ) {
		*value = ((unsigned long) SwizzleInt(temp[0]) << 8) | temp[1];

		return TRUE;
	}

	return FALSE;
}

/*
	Description:
		This routine writes a 32-bit long to the specified file.
*/
static BOOL MyWriteULong( int hPosFile, unsigned long value )
{
	int	temp[2];

	temp[0] = SwizzleInt( value >> 8 );
	temp[1] = value & 0xFF;
	if ( Fwrite( hPosFile, temp, 4, Y_MEMORY, -1 ) == 4 )
		return TRUE;

	return FALSE;
}

static unsigned long _reentrant CalculateLongChecksum( unsigned long value )
{
	int				i;
	unsigned long	checksum = 0;

	for ( i = 0; i < 4; i++ ) {
		checksum += value & 0xFF;
		value >>= 8;
	}

	return checksum;
}

/*
	Description:
		The checksum is calculated and stored in the file position structure.
*/
static unsigned long CalculateChecksum( tFilePosition *pFilePosition )
{
	unsigned long	checksum = 0;

	checksum += CalculateLongChecksum( pFilePosition->filePos );
	checksum += CalculateLongChecksum( pFilePosition->playThroughFlag );

	return ~checksum;
}
