#ifndef MUSICLIB_GHDR_H
#define MUSICLIB_GHDR_H

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================

                                        General Description

====================================================================================================

                               Sigmatel Inc Confidential Proprietary
                               (c) Copyright Sigmatel Inc 2004, All Rights Reserved

HEADER NAME: mf_sf_filename.h   AUTHOR: Developer Name      CREATION DATE: DD/MM/YYYY

PRODUCT NAMES: All

GENERAL DESCRIPTION:

    General description of this grouping of functions.

Portability: All


Revision History:

                         Modification        Tracking
Author                       Date             Number           Description of Changes
---------------------    ------------        ----------        -------------------------------------
Developer Name            DD/MM/YYYY         PDaaaxxxxx        brief description of changes made


====================================================================================================
                                            DESCRIPTION
====================================================================================================

GLOBAL FUNCTIONS:
    MF_global_func_name()

TRACEABILITY MATRIX:
    None

==================================================================================================*/

/*==================================================================================================
								Conditional Compilation Directives
==================================================================================================*/
#ifdef WIN32
#define _PC_SIMULATION_
#else
#define _RUNNING_IN_EMBEDDED_
#endif	// WIN32

#if 1
#define _NEWMUSIC_	/* install new music list */
#endif

#if 1
#define _AUDIBLE_	/* install audible list */
#endif

#if 1
#define _ONTHEGO_	/* install on the go list */
#endif

#ifdef PL3_FB
#define _FOLDER_BROWSE_ // install folder browsing
#endif

#if 1
#define _SUPPORT_2000_SONGS_
#endif

/*==================================================================================================
                                           INCLUDE FILES
==================================================================================================*/
#ifdef _RUNNING_IN_EMBEDDED_
#define OEM_SEEK_CUR    SEEK_CUR
#define OEM_SEEK_SET    SEEK_SET
#define OEM_SEEK_END    SEEK_END
#else
#define _X
#define _Y
#define _packed

#define _asmfunc
#define _reentrant

#define OEM_SEEK_CUR    1
#define OEM_SEEK_SET    0
#define OEM_SEEK_END    2
#endif	// _RUNNING_IN_EMBEDDED_

#include "types.h"
#include "exec.h"
#include "messages.h"
#include "project.h"

/*==================================================================================================
                                             CONSTANTS
==================================================================================================*/
/* The bit mask for 2 bits Unicode Flag selection in "uint8 unicode".
Variable Represented		Bits used in "uint8 unicode"
Artistname				1:0
Albumname				3:2
Genrename				5:4
Songname				7:6
----------------------------------------------------------
    Value (2 bits)			Meanings
    0					RAW and All ASCII
    1					Uni-code
    2					Mixed, non-unicode

    3(Genre only)		Genre is ID3v1 spec=>number
*/
#define BITMASK_ARTIST	(0x03)
#define BITMASK_ALBUM	(0x0C)
#define BITMASK_GENRE	(0x30)
#define BITMASK_SONG	(0xC0)

#define BITCHK_ARTIST_ALLASCII	(0x0)
#define BITCHK_ALBUM_ALLASCII	(0x0)
#define BITCHK_GENRE_ALLASCII	(0x0)
#define BITCHK_SONG_ALLASCII	(0x0)

#define BITCHK_ARTIST_UNICODE	(0x01)
#define BITCHK_ALBUM_UNICODE	(0x04)
#define BITCHK_GENRE_UNICODE	(0x10)
#define BITCHK_SONG_UNICODE		(0x40)

#define BITCHK_ARTIST_MIXCODE	(0x02)
#define BITCHK_ALBUM_MIXCODE	(0x08)
#define BITCHK_GENRE_MIXCODE	(0x20)
#define BITCHK_SONG_MIXCODE		(0x80)

#define BITCHK_GENRE_ID3V1		(0x30)

#define UNKNOWN_YEAR_CODE		(1)	/* set a very large number */

#define	INDEX_EOF	0xFFF
#ifdef _FOLDER_BROWSE_
#define INDEX_ROOT  0xffe
#define UNKNOWN_RECORD  0xfff
#endif  // _FOLDER_BROWSE_

/* Constant for item_type */
#define		ITEM_ARTIST			0
#define		ITEM_ALBUM			1
#define		ITEM_GENRE			2
#define		ITEM_TRACK			3
#define		ITEM_YEAR			4
#define		ITEM_SONG_INFO_ARTIST	5
#define		ITEM_SONG_INFO_ALBUM	6
#define		ITEM_SONG_INFO_GENRE	7
#define		ITEM_SONG_INFO_YEAR		9
#ifdef _NEWMUSIC_
#define		ITEM_1DAY			10
#define		ITEM_1WEEK			11
#define		ITEM_1MONTH			12
#endif
#ifdef _AUDIBLE_
#define		ITEM_AUDIBLE		13
#endif
#define		ITEM_ON_THE_GO		14

#define		ITEM_VOICE			15
#define		ITEM_FMREC			16
#define		ITEM_PHOTO			17
#ifdef _FOLDER_BROWSE_
#define		ITEM_INTERNAL		18
#define		ITEM_EXTERNAL       19
#endif  // _FOLDER_BROWSE_
#define     ITEM_UNKNOWN        0xff

/*
input parameter for void ML_AddEntryToLibrary(uint24 Media_type, RAM_SONG_INFO_T *song_info, int16 option);
option input.
*/
#define		ADD_OPTION_MUSIC	(0x00)
#define		ADD_OPTION_VOICE	(0x01)
#define		ADD_OPTION_AUDIBLE	(0x02)
#ifdef _FOLDER_BROWSE_
#define     ADD_OPTION_DIR      (0x03)
#endif  // _FOLDER_BROWSE_

/* Constant for key action */
#define		ACTION_OK				0		/* Press OK button */
#define		ACTION_BACK				1		/* Press BACK button */
#define		ACTION_UP				2		/* Press UP button */
#define		ACTION_DOWN				3		/* Press DOWN button */
#define		ACTION_PLAYALL			4		/* Play All function */
#define		ACTION_MENU				5		/* Back the browsing tree to root */

#define     PLAYMODE_CURRENT_PLAYMODE_BITPOS    0
#define     PLAYMODE_LAST_PLAYMODE_BITPOS       1
#define     PLAYMODE_REPEAT_BITPOS              2
#define     PLAYMODE_MEDIA_BITPOS               3

#define		NO_SD					0
#define		HAS_SD					1

#define		PLAY_NORMAL				0
#define		PLAY_SHUFFLE			1

#define     PLAY_REPEAT_OFF         0
#define     PLAY_REPEAT_ON          1

#define     PLAY_SELECT_FLASH       0
#define     PLAY_SELECT_SD          1

#define		SHUFFLE_NEXT_SONG			0
#define		SHUFFLE_PREVIOUS_SONG       1

#define		ON_THE_GO_EXIST			0
#define		ON_THE_GO_FULL			1
#define		ON_THE_GO_FREE			2
#define		ON_THE_GO_DEL_PLAYING	3
#define		ON_THE_GO_DEL_SMALLER_ID    4

#define		REC_VOICE_TYPE			0
#define		REC_FMREC_TYPE			1
#define		REC_PHOTO_TYPE			2
#define		VOICE_FILE_ADD			0
#define		VOICE_FILE_DEL			1

#define		MAX_BROWSE_WINDOW_SIZE	(8)
#define		BROWSE_WINDOW_SIZE		(4)
/* maximum number of song file, should not be larger than 2048 each for on-board
flash or external SD card */
#ifdef _SUPPORT_2000_SONGS_
#define MAX_NUM_OF_SONG (2000)
#else
#define MAX_NUM_OF_SONG (1000)
#endif // _SUPPORT_2000_SONGS_

/* number of byte in one DSP word */
#define NUM_OF_BYTE_IN_ONE_WORD (3)
#define LEN_OF_NAME_IN_BYTE (120) /* must be an integer multiple of 3 */
/* length of file pathname in byte, assuming directory and file in 8.3 format, there
are 10 level directory structure */
#define LEN_OF_FILEPATH_IN_BYTE (129) /* must be an integer multiple of 3 */
#define LEN_OF_LONG_FILENAME_IN_BYTE (129) /* must be an integer multiple of 3 */

/* number of list in new music, 1-day, 1-week, 1-month */
#define NUM_OF_LIST_IN_NEW_MUSIC (3)
/* number of songs in each new music list */
#define NUM_OF_SONG_IN_NEW_MUSIC (40)
#define NUM_OF_SONG_IN_NEW_MUSIC_DAY (20)
#define NUM_OF_SONG_IN_NEW_MUSIC_WEEK (30)
#define NUM_OF_SONG_IN_NEW_MUSIC_MONTH (40)
/* number of songs in the on-the-fly list */
#define NUM_OF_SONG_IN_ON_THE_FLY (30)
/* number of files audible list */
#define NUM_OF_AUDIBLE_FILE (250)

#define MAX_NUM_OF_VOICE (1000)
#define LEN_OF_VOICE_NAME_IN_BYTE (LEN_OF_NAME_IN_BYTE) /* must be an integer multiple of 3 */
#define LEN_OF_VOICE_FILEPATH_IN_BYTE (LEN_OF_FILEPATH_IN_BYTE) /* must be an integer multiple of 3 */

#define MAX_NUM_OF_FMREC (MAX_NUM_OF_VOICE)	/* _must_be_equal_voice_num_ */
#define MAX_NUM_OF_PHOTO (MAX_NUM_OF_VOICE)	/* _must_be_equal_voice_num_ */
#ifdef _FOLDER_BROWSE_
#define MAX_NUM_OF_DIR   (1000)
#else
#define MAX_NUM_OF_DIR   (1)
#endif  // _FOLDER_BROWSE_
#define LEN_OF_FMREC_NAME_IN_BYTE (LEN_OF_NAME_IN_BYTE) /* must be an integer multiple of 3 */
#define LEN_OF_FMREC_FILEPATH_IN_BYTE (LEN_OF_FILEPATH_IN_BYTE) /* must be an integer multiple of 3 */

#ifndef _MAX_DIR_DEPTH
#define _MAX_DIR_DEPTH  8   // referred to "playlist3internal.h"
#endif  // _MAX_DIR_DEPTH

/*==================================================================================================*/


/*==================================================================================================
                                               MACROS
==================================================================================================*/

/*==================================================================================================
                                               ENUMS
==================================================================================================*/
#define NUM_OF_MEDIA				(2)
#define MEDIA_TYPE_FLASH			(0)
#define MEDIA_TYPE_SD				(1)
/*==================================================================================================
                                     STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

typedef char	int8;
typedef short	int16;
typedef int     int24;
typedef long    int32;

typedef int	intx;

typedef unsigned char	uint8;
typedef unsigned short	uint16;
typedef unsigned int	uint24;
typedef unsigned long	uint32;

/*
Any missing information in the name fields will be filled with ¡§Unknown¡¨ in Unicode format.
artist_name[] = ¡§Unknown¡¨; unicode = 0x01;
album_name[] = ¡§Unknown¡¨; unicode = 0x04;
genre_name[] = ¡§Unknown¡¨; unicode = 0x08;
song_name[] = ¡§Unknown¡¨; unicode = 0x0F;
*/
/*
path_name[] _must_have_data_:
path_name[] = (Max. 120 Characters);
year range:
year = 0x000000-0xFFFFFF;
DOCUMENT CONTROL NUMBER : Version : 01.01
Unknown track number:
track_number = 0x7FFFFF;
unicode refer to above #define BITMASK_*
*/
/*
Interface of UI and Music Library
1) If file was created in ID3V1 format genre, UI will convert it to Unicode string.

2) Music Library expected to reserve 128x7 words RAM for UI mapping table.

3) UI to Music Library variable passing length definition:
	All ASCII Characters:
		Max. Number of Bytes Stored = 120 Bytes
		Max. Number of Characters Stored/Display = 40 Characters
	Unicode Characters:
		Max. Number of Bytes Stored = 120 Bytes
		Max. Number of Characters Stored/Display = 40 Characters

4) UI input data to Music Library in two formats.
	Formats:	1) All ASCII (24 bits data)
				2) Unicode + Mix. Non -Unicode (16 bits data)

5) UI calling function:
COMP_OPT_REN void ML_AddEntryToLibrary(RAM_SONG_INFO_T *song_info, int16 option);
	int16 option¡¨ definition:
		option	= 0 - song_info struct contains a mp3/wma format file.
				= 1 - song_info struct contains a voice format file.
				= 2 - song_info struct contains a audible format file.

	i) Only mp3/wma file (option = 0, 2) files will be sorted,
	ii) Voice and audible(option = 1) file will be store in a voice_list and audible_list separately.

6) Modification Date:
	uint24 g_file_time:
		YYMMDD:	12 bits - Year, 6 bits - Month, 6 - bits Date
*/

/* struct to store the song information passed from UI, this struct will be placed in
ghdr\musiclib_ghdr.h and included in UI files*/
typedef struct _ram_song_info {
	uint24 artist_name[LEN_OF_NAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD];
	uint24 album_name[LEN_OF_NAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD];
	uint24 genre_name[LEN_OF_NAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD];
	uint24 song_name[LEN_OF_NAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD];
	uint24 path_name[LEN_OF_FILEPATH_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD];
    uint32 g_songFastKey;
	uint24 dir_name[LEN_OF_LONG_FILENAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD];
	uint24 year;
	uint24 track_number;
	uint8 unicode;
} RAM_SONG_INFO_T;

/* struct to store the group name: artist, album and genre, in the flash */
typedef struct _flash_group_name {
	uint24 name[LEN_OF_NAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD];
	uint8 unicode;
} FLASH_GROUP_NAME_T;

// struct to store directories information passed from UI
#ifdef _FOLDER_BROWSE_
typedef struct _ml_DirInfo {
	uint24  u8Unicode : 8;
	uint24  u12DirDepth : 12;
	uint24  u4Added : 4;		
	INT     iDirRecord;
	DWORD   dwFastKey;
	uint24  u24PathName[LEN_OF_FILEPATH_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD];
} ML_DIRINFO_T;
#endif  // _FOLDER_BROWSE_

/*==================================================================================================
                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
extern uint24   IsPlayOnTheGo;
extern FLASH_GROUP_NAME_T   browse_item_name[];
extern uint24   merge_id_list_flash[];
extern uint24   merge_id_list_sd[];
extern INT _X   g_iBrowseWindowSize;
#ifdef _FOLDER_BROWSE_
extern ML_DIRINFO_T g_tDirInfoBuffer[_MAX_DIR_DEPTH];
extern uint24       g_u24DirName[LEN_OF_NAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD];
extern uint24       g_u24BrowseNumOfDirInDir[];
#endif  // _FOLDER_BROWSE_
extern INT _X    *sec_temp_buf_X;
extern uint24   playMode;   /* bit 0 = current play mode; bit 1 = last play mode ; bit 2 = repeat on/off(0=off/1=on) */

/*==================================================================================================
                                        FUNCTION PROTOTYPES
==================================================================================================*/

///////////////////////////////////////////////////////////////////////
//! \brief
//!
//! \fntype Function
//!
//! Called by UI, to initialize the parameters before insert any item.
//! Call only once before inserting items. Call once for each media.
//!
//! \param[in]  none
//!
//! \return
//!
///////////////////////////////////////////////////////////////////////
void ML_InitLibraryParameter(void);

///////////////////////////////////////////////////////////////////////
//! \brief
//!
//! \fntype Function
//!
//! Called by UI, the AddEntryToLibrary is the music library building
//! algorithms for the flash-type memory and SD memory by sorting the song information
//! by, but not limited to, album, artist, genre, year and track. Called once for each song,
//! the song information is recorded in music library.
//!
//! \param[in]  uint24 Media_type - Possible values: MEDIA_TYPE_FLASH(=0); MEDIA_TYPE_SD(=1)
//! \param[in]  RAM_SONG_INFO_T *song_info - Structure to store the song information passed from UI, this structure is defined in ghdr\musiclib_ghdr.h and included in UI files)
//! \param[in]  int16 option - option = 0 -- song_info struct contains a mp3, wma or wav format file.
//!
//! \return
//!
///////////////////////////////////////////////////////////////////////
void ML_AddEntryToLibrary(uint24 Media_type, RAM_SONG_INFO_T *song_info, int16 option);

///////////////////////////////////////////////////////////////////////
//! \brief
//!
//! \fntype Function
//!
//! \param[in]
//!
//! \return
//!
///////////////////////////////////////////////////////////////////////
#ifdef _FOLDER_BROWSE_
_reentrant void ML_AddDirEntryToLibrary(uint24 u24MediaType, ML_DIRINFO_T *ptDirInfo, int16 i16Option);
#endif  // _FOLDER_BROWSE_

///////////////////////////////////////////////////////////////////////
//! \brief
//!
//! \fntype Function
//!
//! Called by UI, flush sorted and associate list to flash when no
//! more song needed to be added for that media. This function finalizes the building of
//! music library for that particular media.
//!
//! \param[in]  uint24 Media_type - Possible values: MEDIA_TYPE_FLASH(=0); MEDIA_TYPE_SD(=1)
//!
//! \return
//!
//! \note   Bulid the library first (call ML_AddEntryToLibrary) before calling this
//!         function.
///////////////////////////////////////////////////////////////////////
_reentrant INT ML_FlushLibraryToFlash(INT Media_type, INT b, INT *c);

///////////////////////////////////////////////////////////////////////
//! \brief
//!
//! \fntype Function
//!
//! \param[in]
//!
//! \return
//!
///////////////////////////////////////////////////////////////////////
#ifdef _NEWMUSIC_
void ML_UpdateNewMusic(void);
#endif

///////////////////////////////////////////////////////////////////////
//! \brief
//!
//! \fntype Function
//!
//! Called by UI, to initialize the file access mechanism for all music
//! library operation if the music.lib file and the music.sec file exist (call
//! ML_BuildSecTableFile() if these files do not exist).
//!
//! \param[in]  none
//!
//! \return
//!
//! \note   Either ML_Buildl2SecTable(void) or ML_BuildSecTableFile(void)
//!         must be called before calling any other music library functions.
///////////////////////////////////////////////////////////////////////
_reentrant INT ML_Buildl2SecTable(INT a, INT b, INT *c);

///////////////////////////////////////////////////////////////////////
//! \brief
//!
//! \fntype Function
//!
//! Called by UI, to initialize the file access mechanism for all music
//! library operation.
//! Two system files (music.lib and music.sec) is created.
//! Call this only if intended to generate / refresh these files, or if the music.lib file and the
//! music.sec file do not exist.
//!
//! \param[in]  none
//!
//! \return
//!
//! \note   Either ML_Buildl2SecTable(void) or ML_BuildSecTableFile(void)
//!         must be called before calling any other music library functions.
///////////////////////////////////////////////////////////////////////
_reentrant INT ML_BuildSecTableFile(INT a, INT b, INT *c);

///////////////////////////////////////////////////////////////////////
//! \brief
//!
//! \fntype Function
//!
//! Preload the list, prepare for renew.
//! IsColdBoot=TRUE: loads the list content and the path name to the on_the_fly_list
//! structure in RAM.
//! IsColdBoot=FALSE: updates the path name only, to the on_the_fly_list structure
//! in RAM.
//!
//! \param[in]  uint24 IsColdBoot - Possible values: TRUE or FALSE
//!
//! \return
//!
//! \note   Use IsColdBoot=TRUE only once when the machine boot-up, other
//!         time when you need to renew the list, use IsColdBoot=FALSE instead.
///////////////////////////////////////////////////////////////////////
void ML_LoadOnTheGo(uint24 IsColdBoot);

///////////////////////////////////////////////////////////////////////
//! \brief
//!
//! \fntype Function
//!
//! Save the list to flash memory.
//!
//! \param[in]
//!
//! \return
//!
//! \note   It is recommended to save the list as soon as possible after it was
//!         changed by the user.
///////////////////////////////////////////////////////////////////////
void ML_SaveOnTheGo(void);	/* call at shutdownmenu.c to save the list */

///////////////////////////////////////////////////////////////////////
//! \brief
//!
//! \fntype Function
//! When the Music Lib is rebuilding, the path of each song is
//! compared with the path stored in the on_the_fly_list structure, to decide if it was in the
//! list before, if so the ID is replaced by the new songID, and the song is keep in the list.
//! Otherwise the song is deleted.
//!
//! \param[in]  uint24 Media_type - Possible values: MEDIA_TYPE_FLASH(=0); MEDIA_TYPE_SD(=1)
//! \param[in]  uint24 _X * temp_path - Input value, the path of song to be checked.
//! \param[in]  uint24 newID - Input value, the new songID of the song.
//!
//! \return
//!
///////////////////////////////////////////////////////////////////////
void ML_ChkOnTheGo(uint24 Media_type, uint24 _X * temp_path, uint24 newID);

///////////////////////////////////////////////////////////////////////
//! \brief
//!
//! \fntype Function
//!
//! Set a flag for all the songs of Media_type to be keep or delete
//! in the ML_UpdateOnTheGo().
//!
//! \param[in]  uint24 Media_type - Possible values: MEDIA_TYPE_FLASH(=0); MEDIA_TYPE_SD(=1)
//! \param[in]  uint24 value - Possible values: TRUE (song will be keep in the list); INDEX_EOF (song will be deleted)
//!
//! \return
//!
///////////////////////////////////////////////////////////////////////
void ML_ChOnTheGo(uint24 Media_type, uint24 value);

///////////////////////////////////////////////////////////////////////
//! \brief
//!
//! \fntype Function
//!
//! Before the list can be browse, call this function to finalize the list.
//!
//! \param[in]  none
//!
//! \return
//!
///////////////////////////////////////////////////////////////////////
void ML_UpdateOnTheGo(void);

///////////////////////////////////////////////////////////////////////
//! \brief
//!
//! \fntype Function
//!
//! Called by UI, to initialize the parameters before insert any item.
//! Call only once before inserting items. Call once for each media.
//!
//! \param[in]  none
//!
//! \return
//!
///////////////////////////////////////////////////////////////////////
void ML_InitVoiceParameter(void);

///////////////////////////////////////////////////////////////////////
//! \brief
//!
//! \fntype Function
//!
//! \param[in]
//!
//! \return
//!
///////////////////////////////////////////////////////////////////////
void ML_LoadVoiceParameter(uint24 Rec_type);

///////////////////////////////////////////////////////////////////////
//! \brief
//!
//! \fntype Function
//!
//! Called by UI, the ML_AddEntryToVoice is the voice library
//! building algorithms for the flash-type memory by sorting the voice files by file names.
//! Called once for each voice file, the information is recorded in music library.
//!
//! \param[in]  uint24 Rec_type - Possible values: REC_VOICE_TYPE(=0); REC_FMREC_TYPE(=1)
//! \param[in]  RAM_SONG_INFO_T *song_info - struct to store the song information passed from UI, this struct will be placed in ghdr\musiclib_ghdr.h and included in UI files
//! \param[in]  uint24 action - option = 0 // always set zero
//!
//! \return
//!
///////////////////////////////////////////////////////////////////////
void ML_AddEntryToVoice(uint24 Rec_type, RAM_SONG_INFO_T *song_info, uint24 action);

///////////////////////////////////////////////////////////////////////
//! \brief
//!
//! \fntype Function
//!
//! Called by UI, flush sorted and associate list to flash when no
//! more voice needed to be added for that media. This function finalizes the voice building
//! of music library for that particular media.
//!
//! \param[in]  uint24 Rec_type - Possible values: REC_VOICE_TYPE(=0); REC_FMREC_TYPE(=1)
//!
//! \return
//!
//! \note   Bulid the library first (call ML_AddEntryToVoice) before calling this
//!         function.
///////////////////////////////////////////////////////////////////////
void ML_FlushVoiceToFlash(uint24 Rec_type);

///////////////////////////////////////////////////////////////////////
//! \brief
//!
//! \fntype Function
//!
//! Called by UI, the merge the music library tables album,
//! artist, genre, song and year.
//!
//! \param[in]  none
//!
//! \return
//!
///////////////////////////////////////////////////////////////////////
void ML_MergeLibraryTables(void);

///////////////////////////////////////////////////////////////////////
//! \brief
//!
//! \fntype Function
//!
//! Called by UI, the merge the music library tables album,
//! artist, genre, song and year.
//!
//! \param[in]  none
//!
//! \return
//!
///////////////////////////////////////////////////////////////////////
INT _reentrant PathFormationPartial(_packed BYTE* dst, _packed BYTE* src, int iDepth);
INT _reentrant PathFormation(_packed BYTE* dst, _packed BYTE* src, int iDepth);
_reentrant void TrimUnicodeString(UCS3* pString);
_reentrant void TrimPackedString(UCS3* pString);

///////////////////////////////////////////////////////////////////////
//! \brief
//!
//! \fntype Function
//!
//! \param[in]
//!
//! \return
//!
///////////////////////////////////////////////////////////////////////
_reentrant void ML_FlushSortedListToFlash(uint24 u24MediaType);

/*================================================================================================*/

// Siukoon 2005-02-28
#define MUSICLIB_STDRIVE_NUMBER	    (0)
#ifdef _SUPPORT_2000_SONGS_
#define MUSICLIB_FILESIZE_IN_BYTE   (4194304L*2) // (SINGLE_DATABASE_SIZE*BYTE_PER_SECTOR)*2
#else
#define MUSICLIB_FILESIZE_IN_BYTE   (2097152L*2) // (SINGLE_DATABASE_SIZE*BYTE_PER_SECTOR)*2
#endif  // _SUPPORT_2000_SONGS_
#define MUSICLIB_FLASHPART_FILESIZE_IN_BYTE   (MUSICLIB_FILESIZE_IN_BYTE/2)
#define WORD_PER_SECTOR             (171)
#define BYTE_PER_SECTOR             (512)
#define MUSICLIB_FILESIZE_IN_SECTOR (MUSICLIB_FILESIZE_IN_BYTE/BYTE_PER_SECTOR)
#define MUSICLIB_FLASHPART_FILESIZE_IN_SECTOR (MUSICLIB_FLASHPART_FILESIZE_IN_BYTE/BYTE_PER_SECTOR)

#define LB_BYTE_PER_SECTOR          (2048)
#define LB_MUSICLIB_FILESIZE_IN_SECTOR (MUSICLIB_FILESIZE_IN_BYTE/LB_BYTE_PER_SECTOR)
#define LB_MUSICLIB_FLASHPART_FILESIZE_IN_SECTOR (MUSICLIB_FLASHPART_FILESIZE_IN_BYTE/LB_BYTE_PER_SECTOR)

#define MUSICLIB_FAT_ERROR_NOT_ENOUGH_MEDIA_SPACE	1
#define MUSICLIB_FAT_ERROR_CREATE_FILE			2
#define MUSICLIB_FAT_ERROR_DELETE_FILE			3
#define MUSICLIB_FAT_ERROR_OPEN_FILE			4

/////////////////////

#define LONG_SIZE_IN_BYTE				(6)

#ifdef _SUPPORT_2000_SONGS_
#define SEC_TABLE_SIZE_IN_LONG			(16384) // DATABASE_SIZE
#else
#define SEC_TABLE_SIZE_IN_LONG			(8192)  // DATABASE_SIZE
#endif // _SUPPORT_2000_SONGS_
#define SEC_TABLE_SIZE_IN_BYTE			(SEC_TABLE_SIZE_IN_LONG*LONG_SIZE_IN_BYTE)

#define SEC_TABLE_FLASHPART_SIZE_IN_LONG	(SEC_TABLE_SIZE_IN_LONG/2)
#define SEC_TABLE_FLASHPART_SIZE_IN_BYTE	(SEC_TABLE_SIZE_IN_BYTE/2)

#define NUM_OF_LONG_PER_512SEC          (85)    // 512/6=>85
#ifdef _SUPPORT_2000_SONGS_
#define NUM_OF_LEVEL2_SEC				(194)   /* 97x2 */
#define NUM_OF_LEVEL2_FLASHPART_SEC		(97)    /* 8192/85 */
#else
#define NUM_OF_LEVEL2_SEC				(98)	/* 49x2 */
#define NUM_OF_LEVEL2_FLASHPART_SEC		(49)	/* 4096/85 */
#endif // _SUPPORT_2000_SONGS_
#define LEVEL2_TABLE_SIZE				(NUM_OF_LEVEL2_SEC*2)
#define SEC_TABLE_FLASH_START_SEC		(0x00)
#define SEC_TABLE_SD_START_SEC			(NUM_OF_LEVEL2_FLASHPART_SEC)

#define SEC_TABLE_FILESIZE_IN_BYTE		(BYTE_PER_SECTOR*NUM_OF_LEVEL2_SEC)
#ifdef _SUPPORT_2000_SONGS_
#define BYTE_FILL_INTERDB_GAP			(320)   // 512-((8192-(85*96))*6)
#else
#define BYTE_FILL_INTERDB_GAP			(((NUM_OF_LEVEL2_FLASHPART_SEC*NUM_OF_LONG_PER_512SEC)-SEC_TABLE_FLASHPART_SIZE_IN_LONG)*LONG_SIZE_IN_BYTE+2)
#endif  // _SUPPORT_2000_SONGS_

#define LB_SEC_TABLE_SIZE_IN_LONG			(LB_MUSICLIB_FILESIZE_IN_SECTOR)
#define LB_SEC_TABLE_SIZE_IN_BYTE			(LB_SEC_TABLE_SIZE_IN_LONG*LONG_SIZE_IN_BYTE)

#define LB_SEC_TABLE_FLASHPART_SIZE_IN_LONG	(LB_SEC_TABLE_SIZE_IN_LONG/2)
#define LB_SEC_TABLE_FLASHPART_SIZE_IN_BYTE	(LB_SEC_TABLE_SIZE_IN_BYTE/2)

#define NUM_OF_LONG_PER_2048SEC			    (341)   // 2048/6=>341
#ifdef _SUPPORT_2000_SONGS_
#define NUM_OF_LEVEL2_2048SEC				(49)    // 16384/341=>49
#define NUM_OF_LEVEL2_FLASHPART_2048SEC		(25)    /* 8192/341=>25 */
#else
#define NUM_OF_LEVEL2_2048SEC				(25)    // 8192/341=>25
#define NUM_OF_LEVEL2_FLASHPART_2048SEC		(13)	/* 4096/341=>13 */
#endif // _SUPPORT_2000_SONGS_

#ifdef _SUPPORT_2000_SONGS_
#define BYTE_FILL_INTERDB_GAP_2048			(2000)  // 2048-((8192-(341*24))*6)
#define BYTE_FILL_INTERDB_2ND_GAP_2048      (2532)  // 99328-96796
#else
#define BYTE_FILL_INTERDB_GAP_2048			(((341-4)*6)+2) //2024
#define BYTE_FILL_INTERDB_2ND_GAP_2048      (2556)  // 50176-47620
#endif // _SUPPORT_2000_SONGS_

#ifdef __cplusplus
}
#endif

#endif  /* MUSICLIB_GHDR_H */
