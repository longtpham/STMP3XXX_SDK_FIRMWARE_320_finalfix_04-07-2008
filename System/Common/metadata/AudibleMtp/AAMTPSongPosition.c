/* $Header: Z:/projects/Audible/SigmaTel/AudibleOEM/STMP3XXX_SDK_PLAYER_FIRMWARE/System/common/metadata/rcs/audiblesongposition.c 1.3 2005/02/09 08:55:02Z mmiu Exp $ */
/* $Name: SigmaTel_v1.0_RC3 SigmaTel_v1.0_RC2 SigmaTel_v1.0_RC1 $ */
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
#include "sysmem.h"
#include "fserror.h"
#include "AAFileAccess.h"
#include "AAMTPSongPosition.h"

#define POSITION_FILE_EXTENTION	0x534F50   // SOP
//static			UCS3 tempStr[TITLE_SIZE];
//static	_packed BYTE LongFileName[MAX_FILENAME_LENGTH/2];
//static	_packed BYTE pcFilePathName[MAX_FILENAME_LENGTH/2];

static void _reentrant AudibleGetSongPositionFilePathName(INT PackedPathNameAddress, _packed BYTE *filePathName, INT btCurrentDevice);
static int	SwizzleInt(int value);
static BOOL MyReadULong( int hPosFile, unsigned long *value );
static BOOL MyWriteULong( int hPosFile, unsigned long value );
static unsigned long CalculateChecksum( tFilePosition *pFilePosition );

extern unsigned int _X	g_bAudibleConsumedBytesLow, g_bAudibleConsumedBytesHigh;

tFilePosition filePosition;

/*
RETCODE _reentrant AAMTPSetFilePosition(INT PackedPathNameAddress, INT btCurrentDevice) {
    INT fOut; 
	tFilePosition filePosition;
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
	AudibleGetSongPositionFilePathName(PackedPathNameAddress, pcFilePathName, btCurrentDevice);

	// mmiu - For SanDisk Sansa e100 series, always get file position from internal Flash
	btCurrentDevice = 0;
    if ( FSGetByte( pcFilePathName, 0, Y_MEMORY ) != '/' )
		PutByte( pcFilePathName, 'a', 0, Y_MEMORY );

	// Load FAT Write code to application space
	SysLoadFATWrite();   

	// Open file for write 
	if( (fOut = FSFileOpen( pcFilePathName, WRITE_PLUS, btCurrentDevice)) < 0 ) {
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

	FSFileClose(fOut);
	FlushCache();
    SysUnLoadFATWrite();

	return META_DATA_SUCCESS;
}
*/

AudErrorCode _reentrant AAMTPGetFilePosition(INT PackedPathNameAddress, INT btCurrentDevice, unsigned long * pPlaybackPos, int * pPlayedThrough ) {
    INT fIn; 
	//tFilePosition filePosition;

	// Get song position file path and name
//	AudibleGetSongPositionFilePathName(PackedPathNameAddress, pcFilePathName, btCurrentDevice);

	// mmiu - For SanDisk Sansa e100 series, always get file position from internal Flash
	btCurrentDevice = 0;

//    if ( FSGetByte( pcFilePathName, 0, Y_MEMORY ) != '/' )
//		PutByte( pcFilePathName, 'a', 0, Y_MEMORY );

	// Set defaults
	if ( pPlaybackPos )
		*pPlaybackPos = 0;
	if ( pPlayedThrough )
		*pPlayedThrough = 0;

	// Open file for read
	if( (fIn = FSFileOpen( (_packed BYTE *)PackedPathNameAddress, READ, btCurrentDevice)) < 0 ) {
		if(fIn == FILE_NOT_FOUND) {
			return AUD_NO_ERROR;
		} else {
			return AUD_FILE_OPEN_FAIL;
		}
    }

	// Read current audible file position 
	if (	!MyReadULong( fIn, &filePosition.version )	||
			!MyReadULong( fIn, &filePosition.checksum )	||
			!MyReadULong( fIn, &filePosition.filePos )	||
			!MyReadULong( fIn, &filePosition.playThroughFlag ) ) {
		FSFileClose(fIn);
		return AUD_FILE_READ_FAIL;
	}

	// Check checksum. If checksum bad, eat the error and report success. Start at beginning of file.
	if ( CalculateChecksum( &filePosition ) != filePosition.checksum ) {
        FSFileClose( fIn );
		return AUD_NO_ERROR;
	}

	// Set the values
	if ( pPlaybackPos )
		*pPlaybackPos = filePosition.filePos;
	if ( pPlayedThrough )
		*pPlayedThrough = filePosition.playThroughFlag;

	FSFileClose(fIn);

	return AUD_NO_ERROR;
}

static void _reentrant AudibleGetSongPositionFilePathName(INT PackedPathNameAddress, _packed BYTE *filePathName, INT btCurrentDevice) {
/*
    WORD wExtension[2], iStrLen;

	// Null terminate 
	filePathName[0] = 0;
	packed_strcat(filePathName,(_packed BYTE*)PackedPathNameAddress);
	
	// Get LFN.
	if( SysCallFunction(RSRC_PLAYLIST_CODEBANK, Playlist_GetLFN, TYPE_FILE, (INT)g_CurrentTrack.pFileEntry,
						(INT*)tempStr) == PLAYLIST_SUCCESS ) {
		iStrLen= packed_strlen((_packed BYTE*)filePathName);

   
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
*/	
}



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
