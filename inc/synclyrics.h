////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2004-2006
//
// File        : synclyrics.h
// Description : sync lyrics header file
////////////////////////////////////////////////////////////////////////////////
#ifndef __SYNC_LYRICS_H
#define __SYNC_LYRICS_H

#include "types.h"
#include "project.h"
#include "messages.h"
#include "metadata.h"


// Menu message, MUST check with menus.h to avoid duplicate
// Change the menu message and timer value to match your project
//#define MENU_MSG_LYRICS_UPDATE		MENU_LAST_MSG_ID+8		// moved into msgequ.inc
#define TIMER_LYRICS_UPDATE			9
/////////////////////////////////////////////////////////////////

#define	SLT					('S'|('L'<<8)|('T'<<16))	// V2.2 frame identifiers
#define SYLT				('Y'|('L'<<8)|('T'<<16))	// V2.3 and V2.4 frame identifiers
#define MAX_LYRICS_STRING	100		// Max. number of characters per lyrics including terminate zero
#define LYRICS_ROW_BUF_SIZE	((LCD_X_SIZE/4)+1)

// ASF Header Extension Object
#define ASF_HEADER_EXTENSION_GUID_0		0xbf03b5
#define ASF_HEADER_EXTENSION_GUID_1		0xa92e5f
#define ASF_HEADER_EXTENSION_GUID_2		0x8e11cf
#define ASF_HEADER_EXTENSION_GUID_3		0xc000e3
#define ASF_HEADER_EXTENSION_GUID_4		0x53200c
#define ASF_HEADER_EXTENSION_GUID_5		0x000065

// ASF Reserved 1
#define ASF_RESERVED_1_GUID_0			0xd3d211
#define ASF_RESERVED_1_GUID_1			0xa9baab
#define ASF_RESERVED_1_GUID_2			0x8e11cf
#define ASF_RESERVED_1_GUID_3			0xc000e6
#define ASF_RESERVED_1_GUID_4			0x53200c
#define ASF_RESERVED_1_GUID_5			0x000065

// ASF Metadata Library Object
#define ASF_METADATA_LIBRARY_GUID_0		0x231c94
#define ASF_METADATA_LIBRARY_GUID_1		0x949844
#define ASF_METADATA_LIBRARY_GUID_2		0xa149d1
#define ASF_METADATA_LIBRARY_GUID_3		0x131d41
#define ASF_METADATA_LIBRARY_GUID_4		0x70454e
#define ASF_METADATA_LIBRARY_GUID_5		0x000054

// Data structure for lyrics library
typedef struct {
	WORD	iTitleBufferLength;
	UCS3	*wTitle;
	WORD	iArtistBufferLength;
	UCS3	*wArtist;
	WORD	iAlbumBufferLength;
	UCS3	*wAlbum;
	DWORD	dwNumBytes;
} LYRICS_FILE_META_DATA;

// Function Prototypes
RETCODE _reentrant Id3V2LyricsReadFrame(INT iHandle, WORD uFrameSize, INT *ptr);
RETCODE _reentrant Mp3MetaDataReadLyrics3(INT iHandle,INT iIgnored, INT *pMetaData);
RETCODE _reentrant Mp3MetaDataReadLRC(INT iIgnored1,INT iIgnored2, INT *pMetaData);
RETCODE _reentrant WMALyricsReadFrame(INT iHandle, WORD uFrameSize, INT *ptr);
RETCODE _reentrant LyricsInit(INT a, INT b, INT *c);
RETCODE _reentrant LyricsStart(INT a, INT b, INT *c);
RETCODE _reentrant LyricsPause(INT a, INT b, INT *c);
RETCODE _reentrant LyricsGetNext(INT a, INT b, INT *c);
RETCODE _reentrant LyricsStop(INT a, INT b, INT *c);
RETCODE _reentrant LyricsIsValid(void);
RETCODE _reentrant LyricsSkipping(INT a, INT b, INT *c);
RETCODE _reentrant LyricsGetNextLyricsTime(INT a, INT b, INT *pTime);
DWORD _reentrant LyricsGetFirstLyricsTime(void);

extern UCS3 _X * _X	g_pSongLyrics;

#endif
