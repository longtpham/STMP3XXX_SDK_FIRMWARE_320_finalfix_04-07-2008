////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002
//
// Filename: metadata.h
// Description:
////////////////////////////////////////////////////////////////////////////////

#ifndef _METADATA_H
#define _METADATA_H

///////////////////////////////////////////////////////////////////////////////
// MetaData definitions
///////////////////////////////////////////////////////////////////////////////

//#define MMC_CMD59       0x40|59

///////////////////////////////////////////////////////////////////////////////
// Meta Data structure definitions
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// MetaData definitions

#define META_DATA_FOUND                     (WORD)(0)
#define META_DATA_NOT_FOUND                 (WORD)(0x100)
#define META_DATA_FILE_NOT_SUPPORTED        (WORD)(0x101)
#define META_DATA_FILEOPEN_ERROR            (WORD)(0x200)
#define META_DATA_FILEREAD_ERROR            (WORD)(0x201)
#define META_DATA_FILECLOSE_ERROR           (WORD)(0x202)
#define META_DATA_FILESEEK_ERROR            (WORD)(0x203)
#define META_DATA_FILEWRITE_ERROR           (WORD)(0x204)
#define META_DATA_SUCCESS			        (WORD)(0x205)

#define META_DATA_NOT_VBR_FORMAT            (WORD)(0x102)

// VBR
#define VBR_NOT_DETERMINED 0
#define VBR_TRUE 1

#ifndef TITLE_SIZE
//#define TITLE_SIZE 30
#ifndef USE_PLAYLIST3
#define TITLE_SIZE 256		//was 30 but LFNs require up to 256!
#else
#define TITLE_SIZE 80		//was 256 but music library only store up to 129!
#endif
#endif

#ifndef ARTIST_SIZE
#ifndef USE_PLAYLIST3
//#define ARTIST_SIZE 30
#define ARTIST_SIZE 100		// increase artist size for use with ID3V2 tags (Stmp00003110)
#else
#define ARTIST_SIZE 48		// increase artist size for use with ID3V2 tags (Stmp00003110)
#endif
#endif

#ifndef ALBUM_SIZE
#ifndef USE_PLAYLIST3
//#define ALBUM_SIZE 30
#define ALBUM_SIZE 50		// increase album size (not as much) for use with ID3V2 tags (Stmp00003110)
#else
#define ALBUM_SIZE 48		// increase album size (not as much) for use with ID3V2 tags (Stmp00003110)
#endif
#endif

#ifdef USE_PLAYLIST3
#ifndef GENRE_SIZE
#define GENRE_SIZE 20
#endif

#ifndef YEAR_SIZE
#define YEAR_SIZE 8
#endif

#ifndef TRACKNUM_SIZE
#define TRACKNUM_SIZE 5 // assume track number is smaller than 0xfff (12bit or 4095)
#endif

#define MAX_TRACK_NUMBER 0x000fff
#define UNKNOWN_TRACK_NUMBER MAX_TRACK_NUMBER
#endif

#define XING_TOC_SIZE	100
#if MTP_BUILD
// This size is used to build table for FF/RW for vbr songs,
// and as there is no such function in mtp mode, it is reduced to release y-memory.
#define VBR_TOC_SIZE    2
#else
#define VBR_TOC_SIZE	200
#endif

extern UCS3 g_wSongTitle[TITLE_SIZE];
extern UCS3 g_wSongArtist[ARTIST_SIZE];
extern UCS3 g_wSongAlbum[ALBUM_SIZE];
#ifdef USE_PLAYLIST3
extern UCS3 g_wSongGenre[GENRE_SIZE];
extern UCS3 g_wSongYear[YEAR_SIZE];
extern WORD g_wSongTrackNum;
#endif

extern WORD g_wVBRTOCTable[VBR_TOC_SIZE];

typedef struct {
    WORD        iTitleBufferLength;
	UCS3       *wTitle;        // Title of Song
    WORD        iArtistBufferLength;
    UCS3       *wArtist;
    WORD        iAlbumBufferLength;
    UCS3       *wAlbum;
#ifdef USE_PLAYLIST3
    WORD        iGenreBufferLength;
    UCS3       *wGenre;
    WORD        iYearBufferLength;
    UCS3       *wYear;
    WORD        wTrackNum;
#endif
    BYTE       btGenre;       // Genre of audio file -- if MP3, use the ID3v1 tag byte
	WORD   	   wSampRate;	  // Sample Rate of song (Hz)
	WORD       wNumChannels;
	WORD   	   wBitRate;	  // Bitrate of song (for MP3 VBR, this will be first bitrate found)
	LONG       iTime;		  // Length of song in seconds
	DWORD 	   dwStartPos;    // Position where audio starts. (if required by the decoder)
	DWORD 	   dwNumBytes;	  // Total Bytes in audio part of song
    DWORD      dwDataBytes;   // for wma; (total bytes - header bytes)
	DWORD 	   dwNumFrames;	  // See note below on frames
	BOOL  	   bVBR;		  // 0 = CBR or does not apply , 1 = VBR (**see question below**)
	INT	  	   iFileType;	  // 0 = unknown, 1 = MP3, 2 = WMA, 4 = AAC, 8 = IMA ADPCM, etc (use defines below)
	INT	  	   iResourceNum;  // Resource number of the decoder that needed for this file
} FILE_META_DATA;

// File Type Defines (use for iFileType) -- matches bit defines in sysequ.inc
#define UNKNOWN_TYPE   0
#define MP3_TYPE       1
#define WMA_TYPE       2
#define AAC_TYPE       4
#define IMA_ADPCM_TYPE 8
#define MS_ADPCM_TYPE  16
#define PCM_WAV_TYPE   32
#define ASF_TYPE       64
#define AUDIBLE_ACELP_TYPE 128
#define	AUDIBLE_MP3_TYPE   256		// 9/28/04 mmiu

#define SMV_ADPCM_TYPE 512


//////SRS WOW DEFS/////////////////////////////////////////////////////////////
// Sample rates
#ifdef WOW
#define SR_48KHZ        48000
#define SR_44KHZ        44100
#define SR_32KHZ        32000
#define SR_24KHZ        24000
#define SR_22KHZ        22050
#define SR_16KHZ        16000
#define SR_12KHZ        12000
#define SR_11KHZ        11025
#define SR_8KHZ          8000
#endif


///////////////////////////////////////////////////////////////////////////////
// MetaData prototypes
///////////////////////////////////////////////////////////////////////////////

RETCODE _reentrant GetFileMetaData(INT PackedPathNameAddress, INT btCurrentDevice, FILE_META_DATA *MetaData);
#ifdef USE_PLAYLIST3
INT _reentrant GetRelativePath(int highlight_index, int browse_type, INT* PackedPathNameAddress);
INT _reentrant RetrieveSongInfo(INT a, INT b, INT *p_path);
#endif

#endif // #ifndef _METADATA_H
