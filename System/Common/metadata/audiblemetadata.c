/* $Header: Z:/projects/Audible/SigmaTel/AudibleOEM/STMP3XXX_SDK_PLAYER_FIRMWARE/System/common/metadata/rcs/audiblemetadata.c 1.12 2005/04/01 05:20:40Z mmiu Exp $ */
/* $Name: SigmaTel_v1.0_RC4 $ */
///////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002
//
// Filename: mp3metadata.c
// Description: Functions for extracting the mp3 meta-data.
//
// 9/28/04 mmiu
///////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "fstypes.h"
#include "fsapi.h"
#include "stringlib.h"
#include "hwequ.h"
#include "project.h"
#include "FileSystem.h"
#include "metadata.h"
#include "resource.h"
#include "metadata.h"
#include "menumanager.h"
#include "mp3metadata.h"
#include "audiblesongposition.h"
#include "AudibleTargetCheck/CheckTargeting.h"
#include "AudibleTargetCheck/MetaFillAudibleFileInfo.h"
#include "sysserialnumber.h"
#include <limits.h>
#include "audiblefillmetadata.h"
#include "AAMetaStrings.h"
#include "AudibleTargetCheck/AAActivationRecords.h"

#ifdef USE_PLAYLIST5
#include "playlist5.h"
#include "playerlib.h"
#endif

CAudibleAAInternal			metaAudFileData;
AudibleDRMArgs_t			AudibleDRMArgs;
CAudibleEncryptionStrings	encStr;

extern unsigned int _X	g_bAudibleCurrentChapterNum, g_bAudibleTotalChapterCount;
extern unsigned int _Y g_wSongCurrentMinutes, g_wSongCurrentSeconds;
extern	g_wBitRateFound;
extern	g_wBitRate;
extern	g_wSamplingRate;
extern	_X g_wTargetCheckOK;
extern	int	Acelp_SamplingRate;
extern	int	Acelp_BitRate;

// These routines can be used if using ASCII serial number

static void _reentrant nibbleToAscii(char *p, unsigned int num);
static unsigned int _reentrant getNibble(unsigned int nibbleNum, unsigned int value);
static void _reentrant SerialNumber2ASCII(int *serialNumber, char *dest);


static void _reentrant FS_SwizzleText(_packed char *pText);
static void _reentrant SwizzleData(int *pData, int numWords);
CAudibleDRMRecordData stm_ActivationData[8];
#define	ACTIVATION_DATA_SIZE		560				// 8 activation records of type CAudibleDRMRecordData
_packed char temp_stmActivationData[ACTIVATION_DATA_SIZE];

//
//	For testing only. If this is #defined, double-check that the correct
//	segments are listed in xcopy.dsc, Y_AppCluster.dsc, and metadata.loc.
//
//#define	ACTIVATION_TEST
#ifndef	ACTIVATION_TEST
unsigned char stm_DeviceID[AUDIBLE_PLAYER_ID_SIZE+1];
//Comment the next line to use the number value instead of ASCII for the Device ID.
#define ASCII_DEVICE_ID
#else
unsigned char stm_DeviceID[AUDIBLE_PLAYER_ID_SIZE+1] = {
                0x0011,     // length, "0002F5CA3DA20311"
                0x0030, 0x0030, 0x0030, 0x0032, 0x0046, 0x0035, 0x0043, 0x0041,
                0x0033, 0x0044, 0x0041, 0x0032, 0x0030, 0x0033, 0x0031, 0x0031,
                0x0000, 0x0000};
#endif	// ACTIVATION_TEST

/*
// These test activation records are for "DRM Lite".
CAudibleDRMRecordData stm_ActivationData[8] =

{ {  0x01, 0x2D, 0x89, 0xD7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x54, 0x73, 0x74, 0x50, 0x6C, 0x61, 0x79, 0x65, 0x72, 0x49, 0x44, 0x20, 0x32, 0x30, 0x20, 0x42, 0x79, 0x74, 0x65, 0x00, 0x87, 0xF1, 0x99, 0xB4, 0xC7, 0xFE, 0xCA, 0xA6, 0x90, 0xAB, 0x43, 0x4B, 0x94, 0x8D, 0xB7, 0x72, 0x49, 0xEF, 0x0A, 0x71, 0x87, 0x9D, 0x3A, 0xDF, 0x14, 0xB4, 0xF6, 0x25, 0xC3, 0x4A, 0xCE, 0x90, 0x3F, 0x50, 0x79, 0x67, 0xD8, 0xD4, 0x2D, 0x24 },
  {  0x01, 0x11, 0x27, 0x24, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x54, 0x73, 0x74, 0x50, 0x6C, 0x61, 0x79, 0x65, 0x72, 0x49, 0x44, 0x20, 0x32, 0x30, 0x20, 0x42, 0x79, 0x74, 0x65, 0x00, 0x87, 0xF1, 0x99, 0xB4, 0xC7, 0xFE, 0xCA, 0xA6, 0x90, 0xAB, 0x43, 0x4B, 0x94, 0x8D, 0xB7, 0x72, 0x49, 0xEF, 0x0A, 0x71, 0x2F, 0xC8, 0xC0, 0x24, 0xBD, 0x2D, 0xC3, 0x0E, 0xBE, 0x45, 0xAB, 0xAB, 0x6C, 0xF5, 0x3E, 0xBD, 0x27, 0xF8, 0x5E, 0xB9 },
  {  0x01, 0x11, 0xF2, 0x22, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x54, 0x73, 0x74, 0x50, 0x6C, 0x61, 0x79, 0x65, 0x72, 0x49, 0x44, 0x20, 0x32, 0x30, 0x20, 0x42, 0x79, 0x74, 0x65, 0x00, 0x87, 0xF1, 0x99, 0xB4, 0xC7, 0xFE, 0xCA, 0xA6, 0x90, 0xAB, 0x43, 0x4B, 0x94, 0x8D, 0xB7, 0x72, 0x49, 0xEF, 0x0A, 0x71, 0xD9, 0x49, 0xD2, 0x86, 0x37, 0x27, 0x15, 0xF4, 0xB7, 0x46, 0x06, 0x44, 0x8F, 0xE7, 0x43, 0x56, 0x81, 0x67, 0x65, 0xF9 },
  {  0x01, 0x1F, 0xFC, 0x27, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x54, 0x73, 0x74, 0x50, 0x6C, 0x61, 0x79, 0x65, 0x72, 0x49, 0x44, 0x20, 0x32, 0x30, 0x20, 0x42, 0x79, 0x74, 0x65, 0x00, 0x87, 0xF1, 0x99, 0xB4, 0xC7, 0xFE, 0xCA, 0xA6, 0x90, 0xAB, 0x43, 0x4B, 0x94, 0x8D, 0xB7, 0x72, 0x49, 0xEF, 0x0A, 0x71, 0xC0, 0x09, 0xC8, 0xC5, 0xC5, 0x36, 0xCF, 0x45, 0xE2, 0xE8, 0x28, 0x0E, 0x79, 0xF5, 0xF0, 0x19, 0x80, 0x50, 0x6B, 0x43 },
  {  0x01, 0x30, 0x45, 0x0B, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x54, 0x73, 0x74, 0x50, 0x6C, 0x61, 0x79, 0x65, 0x72, 0x49, 0x44, 0x20, 0x32, 0x30, 0x20, 0x42, 0x79, 0x74, 0x65, 0x00, 0x87, 0xF1, 0x99, 0xB4, 0xC7, 0xFE, 0xCA, 0xA6, 0x90, 0xAB, 0x43, 0x4B, 0x94, 0x8D, 0xB7, 0x72, 0x49, 0xEF, 0x0A, 0x71, 0x7F, 0xB2, 0x06, 0x55, 0xC8, 0x6F, 0xCC, 0xDA, 0xD6, 0x5F, 0x0E, 0xE4, 0x8C, 0x3D, 0x17, 0x36, 0x91, 0xF6, 0xAC, 0x40 },
  {  0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x05, 0x00, 0x00, 0x00, 0x01, 0x54, 0x73, 0x74, 0x50, 0x6C, 0x61, 0x79, 0x65, 0x72, 0x49, 0x44, 0x20, 0x32, 0x30, 0x20, 0x42, 0x79, 0x74, 0x65, 0x00, 0x87, 0xF1, 0x99, 0xB4, 0xC7, 0xFE, 0xCA, 0xA6, 0x90, 0xAB, 0x43, 0x4B, 0x94, 0x8D, 0xB7, 0x72, 0x49, 0xEF, 0x0A, 0x71, 0xCA, 0x17, 0x89, 0x80, 0xFB, 0xCE, 0xBE, 0x4D, 0x26, 0xF3, 0x8F, 0x4D, 0xE9, 0x95, 0xEB, 0x05, 0x1F, 0xBE, 0xC1, 0x6B },
  {  0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x06, 0x00, 0x00, 0x00, 0x01, 0x54, 0x73, 0x74, 0x50, 0x6C, 0x61, 0x79, 0x65, 0x72, 0x49, 0x44, 0x20, 0x32, 0x30, 0x20, 0x42, 0x79, 0x74, 0x65, 0x00, 0x87, 0xF1, 0x99, 0xB4, 0xC7, 0xFE, 0xCA, 0xA6, 0x90, 0xAB, 0x43, 0x4B, 0x94, 0x8D, 0xB7, 0x72, 0x49, 0xEF, 0x0A, 0x71, 0xE7, 0x2F, 0x2C, 0x9F, 0x23, 0x60, 0x1C, 0xA2, 0x1C, 0x1C, 0x1A, 0x20, 0x32, 0x8C, 0x96, 0xE8, 0x60, 0x7A, 0x88, 0x86 },
  {  0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x07, 0x00, 0x00, 0x00, 0x01, 0x54, 0x73, 0x74, 0x50, 0x6C, 0x61, 0x79, 0x65, 0x72, 0x49, 0x44, 0x20, 0x32, 0x30, 0x20, 0x42, 0x79, 0x74, 0x65, 0x00, 0x87, 0xF1, 0x99, 0xB4, 0xC7, 0xFE, 0xCA, 0xA6, 0x90, 0xAB, 0x43, 0x4B, 0x94, 0x8D, 0xB7, 0x72, 0x49, 0xEF, 0x0A, 0x71, 0x2A, 0x35, 0xEF, 0x65, 0xA4, 0x6A, 0x9E, 0x07, 0xA5, 0x03, 0x73, 0x10, 0x2C, 0xA7, 0x05, 0x0C, 0x39, 0x57, 0x04, 0x8A }
};
*/

/*
// These test activation records are for DSA.
CAudibleDRMRecordData stm_ActivationData[8] =

{ {  0x01, 0x11, 0x27, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x54, 0x73, 0x74, 0x50, 0x6C, 0x61, 0x79, 0x65, 0x72, 0x49, 0x44, 0x20, 0x32, 0x30, 0x20, 0x42, 0x79, 0x74, 0x65, 0x00, 0x65, 0xC4, 0xD2, 0xB0, 0x2F, 0xF2, 0x56, 0x3F, 0x86, 0xC8, 0x51, 0x4C, 0x00, 0x6E, 0x85, 0x8A, 0xE7, 0xF0, 0x92, 0x42, 0x5F, 0x23, 0x8E, 0xBD, 0x68, 0x04, 0xC7, 0x8A, 0x52, 0xEA, 0x2A, 0x8C, 0xC6, 0xFF, 0x8A, 0x16, 0xD3, 0x93, 0x7A, 0xF9 },
  {  0x01, 0x11, 0xF2, 0x22, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x54, 0x73, 0x74, 0x50, 0x6C, 0x61, 0x79, 0x65, 0x72, 0x49, 0x44, 0x20, 0x32, 0x30, 0x20, 0x42, 0x79, 0x74, 0x65, 0x00, 0xB1, 0x2A, 0x2A, 0xAB, 0x0E, 0x98, 0x63, 0xAF, 0x92, 0x6A, 0x8A, 0xF1, 0xFF, 0x68, 0xF2, 0xD7, 0xAB, 0xAC, 0x69, 0xF9, 0xAE, 0x2E, 0xB6, 0x8F, 0x1A, 0x75, 0xD4, 0x2D, 0x01, 0x26, 0x5A, 0x18, 0xE7, 0x44, 0xAC, 0xE5, 0x45, 0xC5, 0x43, 0x7B },
  {  0x01, 0x1F, 0xFC, 0x27, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x54, 0x73, 0x74, 0x50, 0x6C, 0x61, 0x79, 0x65, 0x72, 0x49, 0x44, 0x20, 0x32, 0x30, 0x20, 0x42, 0x79, 0x74, 0x65, 0x00, 0x07, 0x9F, 0x8E, 0x7B, 0xDE, 0x39, 0x14, 0x11, 0x9C, 0x74, 0x5B, 0x2C, 0xA2, 0xBD, 0xF5, 0x95, 0xF8, 0x47, 0xB3, 0xB7, 0xEE, 0xB3, 0x43, 0x99, 0xEA, 0x6E, 0x3D, 0x89, 0x96, 0x9D, 0x03, 0x8E, 0x43, 0x34, 0x48, 0x24, 0xD6, 0xF3, 0xC9, 0xF9 },
  {  0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x54, 0x73, 0x74, 0x50, 0x6C, 0x61, 0x79, 0x65, 0x72, 0x49, 0x44, 0x20, 0x32, 0x30, 0x20, 0x42, 0x79, 0x74, 0x65, 0x00, 0xE0, 0x1C, 0x65, 0xA4, 0xBF, 0x1B, 0xEE, 0x88, 0x66, 0xCA, 0x96, 0xAC, 0x19, 0x53, 0x4B, 0xAD, 0x3A, 0xBB, 0x80, 0x86, 0x47, 0x4C, 0x8C, 0xFE, 0xB7, 0x9D, 0x20, 0x1C, 0x81, 0x69, 0x71, 0xE0, 0x58, 0x77, 0x2A, 0xCB, 0x52, 0x05, 0x2F, 0x35 },
  {  0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x54, 0x73, 0x74, 0x50, 0x6C, 0x61, 0x79, 0x65, 0x72, 0x49, 0x44, 0x20, 0x32, 0x30, 0x20, 0x42, 0x79, 0x74, 0x65, 0x00, 0x06, 0x1D, 0xA2, 0x7B, 0x30, 0xD5, 0x23, 0x5A, 0x9A, 0x0C, 0x19, 0xFE, 0x12, 0xCF, 0xEC, 0x65, 0x11, 0x2E, 0x1F, 0xE9, 0xCF, 0xB1, 0x54, 0x11, 0x64, 0xC2, 0xDD, 0x77, 0xE9, 0x16, 0x9B, 0xF7, 0x4E, 0x45, 0xD0, 0x37, 0x56, 0x8E, 0x8D, 0x31 },
  {  0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x05, 0x00, 0x00, 0x00, 0x01, 0x54, 0x73, 0x74, 0x50, 0x6C, 0x61, 0x79, 0x65, 0x72, 0x49, 0x44, 0x20, 0x32, 0x30, 0x20, 0x42, 0x79, 0x74, 0x65, 0x00, 0x2C, 0x35, 0x7C, 0xA9, 0x59, 0x0F, 0x9A, 0xDE, 0x56, 0xC2, 0x76, 0xD2, 0x2B, 0x45, 0x6F, 0x04, 0xAA, 0x69, 0x94, 0xB4, 0xB7, 0xC4, 0xD3, 0xC3, 0x87, 0x87, 0x08, 0xC4, 0x1D, 0xBB, 0x30, 0x96, 0x58, 0xF9, 0xD9, 0x1D, 0x49, 0xAE, 0xEA, 0x41 },
  {  0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x06, 0x00, 0x00, 0x00, 0x01, 0x54, 0x73, 0x74, 0x50, 0x6C, 0x61, 0x79, 0x65, 0x72, 0x49, 0x44, 0x20, 0x32, 0x30, 0x20, 0x42, 0x79, 0x74, 0x65, 0x00, 0xD8, 0x89, 0xED, 0x6A, 0x11, 0x78, 0x13, 0x06, 0x2B, 0xE4, 0xE0, 0x86, 0xC8, 0xFD, 0xD6, 0xB4, 0x86, 0xEF, 0x97, 0x9F, 0x80, 0xBF, 0x6F, 0x98, 0x23, 0xF8, 0xF7, 0x6C, 0xDE, 0x11, 0x7E, 0xDA, 0x04, 0xDD, 0x43, 0xC3, 0x1B, 0x1E, 0x94, 0xD7 },
  {  0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x07, 0x00, 0x00, 0x00, 0x01, 0x54, 0x73, 0x74, 0x50, 0x6C, 0x61, 0x79, 0x65, 0x72, 0x49, 0x44, 0x20, 0x32, 0x30, 0x20, 0x42, 0x79, 0x74, 0x65, 0x00, 0xF9, 0xF0, 0xC5, 0xBC, 0x96, 0xE0, 0xCF, 0x25, 0x54, 0xEE, 0x7B, 0x15, 0xDA, 0x2B, 0x13, 0xA4, 0x0B, 0x83, 0xE8, 0x8C, 0x0A, 0xDC, 0x03, 0xE0, 0x85, 0x2C, 0x56, 0x28, 0x4E, 0xC9, 0x46, 0x2E, 0x51, 0xA3, 0x81, 0xF7, 0x30, 0xA4, 0x76, 0xB8 }
};
*/

// Test IDs
//unsigned char stm_DeviceID[AUDIBLE_PLAYER_ID_SIZE+1] = " TstPlayerID 20 Byte";
/* SerialNumber[] */


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          GetAudibleMetaData
//
//   Type:          Function
//
//   Description:   Finds the meta-data within a .AA file.
//
//   Inputs:        Ptr to pathname, current device id, ptr to meta-data struct.
//
//   Outputs:       RETCODE
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant GetAudibleMetaData(INT PackedPathNameAddress, INT btCurrentDevice, FILE_META_DATA * MetaData)
{
	unsigned int currentTimeSeconds, temp;
	WORD chipSerialNum[(NUM_RAW_SERIAL_NUMBER_BYTES/3) + 1];
	WORD i, j;
	INT fIn;
	CAudibleFileInfoEmbedded	fileInfo;
    RETCODE						rtn = META_DATA_NOT_FOUND;

	// When we get metadata, always reset the targeting check OK flag.
	g_wTargetCheckOK = FALSE;

	// Get Song Position
	SysCallFunction(RSRC_AUDIBLE_SONG_POSITION_P, AudibleGetFilePosition, (INT)PackedPathNameAddress, btCurrentDevice, (void*)MetaData);

	// Open the AA file
#ifdef USE_PLAYLIST5
    g_CurrentFastKey = (g_PL5_PL_queue.pos + g_PL5_Playback.currentplay)->file.lFastKey;
	fIn = FSFastOpen(g_CurrentFastKey,READ);
#else
	fIn = FSFileOpen( (_packed BYTE *) PackedPathNameAddress, READ, btCurrentDevice );
#endif  // #ifdef USE_PLAYLIST5
	if ( fIn < 0 )
		return META_DATA_NOT_FOUND;

	// First we check retrieval of metadata.
	_memset( &metaAudFileData, '\0', sizeof( metaAudFileData ) );
	_memset( &fileInfo, '\0', sizeof( fileInfo ) );
    _memset( &encStr, '\0', sizeof( encStr ) );


	// Use new code bank APIs to retrieve metadata. No longer necessary to open
	// AA file via Audible SDK file open routines.
	SysCallFunction( RSRC_AUDIBLE_METASTRINGS_P, AAGetMetaTitle, fIn, MetaData->iTitleBufferLength, (INT *) MetaData->wTitle );
	SysCallFunction( RSRC_AUDIBLE_METASTRINGS_P, AAGetMetaAuthor, fIn, MetaData->iArtistBufferLength, (INT *) MetaData->wArtist );
	SysCallFunction( RSRC_AUDIBLE_METASTRINGS_P, AAGetMetaCodec, fIn, 0, (INT *) &fileInfo.codec );
	SysCallFunction( RSRC_AUDIBLE_METASTRINGS_P, AAGetMetaAudio, fIn, 0, (INT *) &fileInfo.audio );
	SysCallFunction( RSRC_AUDIBLE_METASTRINGS_P, AAGetMetaCommon, fIn, 0, (INT *) &metaAudFileData.audCommonHeader );
	SysCallFunction( RSRC_AUDIBLE_METASTRINGS_P, AAGetMetaAudibleSpecificHeader, fIn, 0, (INT *) &metaAudFileData.audSpecificHeader );
	SysCallFunction( RSRC_AUDIBLE_METASTRINGS_P, AAGetMetaPlayerIDList, fIn, 0, (INT *) &metaAudFileData.audPlayerIDList );
	SysCallFunction( RSRC_AUDIBLE_METASTRINGS_P, AAGetMetaGroupIDList, fIn, 0, (INT *) &metaAudFileData.audGroupIDList );
	SysCallFunction( RSRC_AUDIBLE_METASTRINGS_P, AAGetMetaHeaderKey, fIn, sizeof(encStr.sz_EncryptionKey), (INT *) &encStr.sz_EncryptionKey );
	SysCallFunction( RSRC_AUDIBLE_METASTRINGS_P, AAGetMetaHeaderSeed, fIn, sizeof(encStr.sz_EncryptionSeed), (INT *) &encStr.sz_EncryptionSeed );
	SysCallFunction( RSRC_AUDIBLE_METASTRINGS_P, AAGetMetaEncBlocks, fIn, sizeof(encStr.sz_EncryptedBlocks), (INT *) &encStr.sz_EncryptedBlocks );
	SysCallFunction( RSRC_AUDIBLE_METASTRINGS_P, AAGetMetaTOC, fIn, 0, (INT *) &fileInfo.audio );

	// Do AA header decrypt
	SysCallFunction( RSRC_AUDIBLE_FILLFILEINFO_P, FillAudibleFileInfo, (INT) &metaAudFileData, (INT) 0, (INT *) &encStr );

	// We're done with the file now at this point
	FSFileClose( fIn );

//	Fclose( metaInputStream.m_pInputFile );

	// Fill in the SigmaTel metadata data structure from the metadata we just retrieved above.
	// Also select the decoder as appropriate.
	rtn = SysCallFunction( RSRC_AUDIBLE_FILLMETADATA_P, AudibleFillMetadata, (INT) MetaData, (INT) &fileInfo, (INT *) PackedPathNameAddress );
	if ( rtn != META_DATA_FOUND )
		return rtn;

#ifndef	ACTIVATION_TEST
	// Get laser fuze chip ID
	ChipGetSerialNumber(chipSerialNum);

	// Re-Order serial # words
	temp = chipSerialNum[0];
	chipSerialNum[0] = chipSerialNum[2];
	chipSerialNum[2] = temp;
#ifdef ASCII_DEVICE_ID
	// Unpack device ID and convert to ASCII
	_memset(stm_DeviceID, 0, sizeof(stm_DeviceID));
	SerialNumber2ASCII(chipSerialNum, &stm_DeviceID[1]);
#else
  	// Unpack device ID without converting to ASCII
	_memset(stm_DeviceID, 0, sizeof(stm_DeviceID));
	SwizzleData((int *) chipSerialNum, 3);
	unpack_data((_packed BYTE *) chipSerialNum, stm_DeviceID, 9);
#endif  //ASCII_DEVICE_ID
#endif	// ACTIVATION_TEST

/*
	// Open activation file
	// This file will always reside in internal flash media
	FS_SwizzleText((_packed char *)activationFile);

	if( (fIn = FSFileOpen((_packed BYTE *) activationFile,READ, 0)) < 0 ) {
		_memset(temp_stmActivationData, 0, (ACTIVATION_DATA_SIZE+2)/3);
    } else {
		// Read all eight activation records from activation file
		Fread( fIn, (INT *)temp_stmActivationData, ACTIVATION_DATA_SIZE, Y_MEMORY, -1 );
	}
	FSFileClose(fIn);
*/

	// Read the activation records into a local data structure
	if ( SysCallFunction( RSRC_AUDIBLE_ACTIVATION_P, AAGetActivationRecords, 0, 0, (INT *) temp_stmActivationData ) < 0 )
		return rtn;

	// Unpack activation data
	unpack_data((_packed BYTE *) temp_stmActivationData, (BYTE *)stm_ActivationData, ACTIVATION_DATA_SIZE);

	// Now check file targeting. All the data is already in memory at this point.
	AudibleDRMArgs.pRecords = stm_ActivationData;
	AudibleDRMArgs.RecordCount = sizeof( stm_ActivationData ) / sizeof( stm_ActivationData[0] );
	if ( SysCallFunction( RSRC_AUDIBLE_TARGET_CHECK_P, AACheckTargeting, (INT) &metaAudFileData, (INT) &AudibleDRMArgs, (INT *) &stm_DeviceID[1] ) == AUD_NO_ERROR )
		g_wTargetCheckOK = TRUE;

    return rtn;
}


static void _reentrant nibbleToAscii(char *p, unsigned int num) {
	if ( num > 9 )
		num += 'A'-'0'-10;	// Convert to hex A-F
	*p = num + '0';
}

static unsigned int _reentrant getNibble(unsigned int nibbleNum, unsigned int value) {
	WORD shiftValue = 24-(nibbleNum*4);
	return (value&(0xF<< shiftValue))>>shiftValue;
}

//
// This routine converts from a binary serial number to a hex string. For
// example, 0x123456, 0x789ABC, 0xDEF012 becomes "123456789ABCDEF0120000".
//
static void _reentrant SerialNumber2ASCII(int *serialNumber, char *dest)
{
	int	i;
    int j;

	// Get 0 to 3 ASCII bytes
	for(i=0; i<4; i++) {
		nibbleToAscii(&dest[i], getNibble((i+1+2), serialNumber[0]));
	}
	// Get 4 to 15 ASCII bytes
	for(i=1; i<3; i++) {
		for(j=0; j<6; j++) {
			nibbleToAscii(&dest[(i*6)+j-2], getNibble((j+1), serialNumber[i]));
		}
	}
	// Get 16 to 19 ASCII bytes; Add Null characters.
	for(i=0; i<4; i++) {
		dest[16+i] = 0;
	}
}


static void _reentrant FS_SwizzleText(_packed char *pText) {
	int bDone=0;
    unsigned int *pTmp=(unsigned int*)pText;
    unsigned int tmp;

	while(!bDone) {
        tmp = *pTmp;
        tmp = ((tmp & 0x0000ff)<<16) | ((tmp & 0x00ff00)) | ((tmp & 0xff0000)>>16);
        *pTmp++=tmp;
        if(!(tmp&0x0000ff) || !(tmp&0x00ff00)  || !(tmp&0xff0000))
            bDone = 1;
    }
}

static void _reentrant SwizzleData(int *pData, int numWords) {
    unsigned int tmp;

	while ( numWords-- ) {
		tmp = *pData;
        tmp = ((tmp & 0x0000ff)<<16) | ((tmp & 0x00ff00)) | ((tmp & 0xff0000)>>16);
		*pData++ = tmp;
	}
}