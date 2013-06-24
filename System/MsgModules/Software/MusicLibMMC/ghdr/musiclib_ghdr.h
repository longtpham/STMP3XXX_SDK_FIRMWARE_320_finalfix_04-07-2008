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
#endif

#if 1
#define _NEWMUSIC_	/* install new music list */
#endif

#if 1
#define _AUDIBLE_	/* install audible list */
#endif

#if 1
#define _ONTHEGO_	/* install on the go list */
#endif

#ifndef _PC_SIMULATION_
#if 1
#define _USE_FS_MUSICLIB_	/* 05/03/01 use fs access to store musiclib instead of direct sector access */

#if 1
#define _USE_FS_SECTAB_		/* use sector_table on build */
#endif

#if 0
#define _WRITEL2SEC_	/* debug print l2_sector_table[] in file */
#endif

#if 0
#define _USE_16MDB_		/* use 16M DB for development */
#endif

#endif
#endif

/*==================================================================================================
                                           INCLUDE FILES
==================================================================================================*/

#ifdef _PC_SIMULATION_
#define _X
#else
#include "types.h"
#include "exec.h"
#include "messages.h"
#include "project.h"
#endif

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

//#define UNKNOWN_YEAR_CODE		(9999)	/* set a very large number */
#define UNKNOWN_YEAR_CODE		(1)	/* set a very large number */

#define	INDEX_EOF	0xFFF

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
#define		ITEM_MVIDEO			18

/*
input parameter for void ML_AddEntryToLibrary(uint24 Media_type, RAM_SONG_INFO_T *song_info, int16 option);
option input.
*/
#define		ADD_OPTION_MUSIC	(0x00)
#define		ADD_OPTION_VOICE	(0x01)
#define		ADD_OPTION_AUDIBLE	(0x02)

/* Constant for key action */
#define		ACTION_OK				0		/* Press OK button */
#define		ACTION_BACK				1		/* Press BACK button */
#define		ACTION_UP				2		/* Press UP button */
#define		ACTION_DOWN				3		/* Press DOWN button */
#define		ACTION_PLAYALL			4		/* Play All function */
#define		ACTION_MENU				5		/* Back the browsing tree to root */

#define		NO_SD					0
#define		HAS_SD					1

#define		PLAY_NORMAL				0
#define		PLAY_SHUFFLE			1

#define		SHUFFLE_NEXT_SONG			0
#define		SHUFFLE_PREVIOUS_SONG			1

#define		ON_THE_GO_EXIST			0
#define		ON_THE_GO_FULL			1
#define		ON_THE_GO_FREE			2
#define		ON_THE_GO_DEL_PLAYING	3
#define		ON_THE_GO_DEL_SMALLER_ID	4

#define		REC_VOICE_TYPE			0
#define		REC_FMREC_TYPE			1
#define		REC_PHOTO_TYPE			2
#define		REC_MVIDEO_TYPE			3
#define		VOICE_FILE_ADD			0
#define		VOICE_FILE_DEL			1

#define		MAX_BROWSE_WINDOW_SIZE	(8)
#define		BROWSE_WINDOW_SIZE		(4)
#if 0
/* maximum number of song file, should not be larger than 2048 each for on-board
flash or external SD card, test use 20 songs */
#define MAX_NUM_OF_SONG (20)
#else
/* maximum number of song file, should not be larger than 2048 each for on-board
flash or external SD card */
#define MAX_NUM_OF_SONG (1000)
#endif

/* number of byte in one DSP word */
#define NUM_OF_BYTE_IN_ONE_WORD (3)
#if 0
/* length of name field: artist, album and genre, in byte */
#define LEN_OF_NAME_IN_BYTE (33) /* must be an integer multiple of 3 */
/* length of file pathname in byte, assuming directory and file in 8.3 format, there
are 10 level directory structure */
#define LEN_OF_FILEPATH_IN_BYTE (21) /* must be an integer multiple of 3 */
#else
#if 0
#define LEN_OF_NAME_IN_BYTE (81) /* must be an integer multiple of 3 */
#else
#define LEN_OF_NAME_IN_BYTE (120) /* must be an integer multiple of 3 */
#endif
/* length of file pathname in byte, assuming directory and file in 8.3 format, there
are 10 level directory structure */
#define LEN_OF_FILEPATH_IN_BYTE (129) /* must be an integer multiple of 3 */
#define LEN_OF_LONG_FILENAME_IN_BYTE (129) /* must be an integer multiple of 3 */
#endif

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
#define MAX_NUM_OF_MVIDEO	(MAX_NUM_OF_VOICE)	/* _must_be_equal_voice_num_ */
#define LEN_OF_FMREC_NAME_IN_BYTE (LEN_OF_NAME_IN_BYTE) /* must be an integer multiple of 3 */
#define LEN_OF_FMREC_FILEPATH_IN_BYTE (LEN_OF_FILEPATH_IN_BYTE) /* must be an integer multiple of 3 */

/*==================================================================================================*/


/*==================================================================================================
                                               MACROS
==================================================================================================*/

/*==================================================================================================
                                               ENUMS
==================================================================================================*/
#define NUM_OF_MEDIA				(2)
//#define NUM_OF_MEDIA				(1)	/* testing */
#define MEDIA_TYPE_FLASH			(0)
#define MEDIA_TYPE_SD				(1)
/*==================================================================================================
                                     STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
#ifdef _PC_SIMULATION_
typedef char	int8;
typedef short	int16;
typedef int	int24;
typedef int	int32;

typedef int	intx;

typedef unsigned char	uint8;
typedef unsigned short	uint16;
typedef unsigned int	uint24;
typedef unsigned long	uint32;
typedef unsigned int	INT;
typedef unsigned long	LONG;

typedef char	BYTE;

#else

typedef char	int8;
typedef short	int16;
typedef int	int24;
typedef long	int32;

typedef int	intx;

typedef unsigned char	uint8;
typedef unsigned short	uint16;
typedef unsigned int	uint24;
typedef unsigned long	uint32;
#endif

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
COMP_OPT_REN void ML_AddEntryToLibrary(RAM_SONG_INFO_T *song_info, int16 option)
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
typedef struct {
	uint24 artist_name[LEN_OF_NAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD];
	uint24 album_name[LEN_OF_NAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD];
	uint24 genre_name[LEN_OF_NAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD];
	uint24 song_name[LEN_OF_NAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD];
	uint24 path_name[LEN_OF_FILEPATH_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD];
    uint32 g_songFastKey; 
	uint24 long_filename[LEN_OF_LONG_FILENAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD];
	uint24 year;
	uint24 track_number;
	uint8 unicode;
} RAM_SONG_INFO_T;

/* struct to store the group name: artist, album and genre, in the flash */
typedef struct {
	uint24 name[LEN_OF_NAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD];
	uint8 unicode;
} FLASH_GROUP_NAME_T;

/*==================================================================================================
                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
extern uint24 IsPlayOnTheGo;
extern FLASH_GROUP_NAME_T	browse_item_name[];
extern uint24				merge_id_list_flash[];
extern uint24				merge_id_list_sd[];
extern INT _X				g_iBrowseWindowSize;

/*==================================================================================================
                                        FUNCTION PROTOTYPES
==================================================================================================*/
#ifdef _WILLIAM_TEST_
uint24 ML_BrowseMenu( uint24 item_type, uint24 action, uint24 highlight, uint24 hasSD, FLASH_GROUP_NAME_T *item_list, uint24 *num_item, uint24 *total_num_item );		// fail return 0; success return 1
uint24 ML_DownList( uint24 step, FLASH_GROUP_NAME_T *item_list, uint24 *num_item );			// fail return 0; success return 1
uint24 ML_UpList( uint24 step, FLASH_GROUP_NAME_T *item_list, uint24 *num_item );				// fail return 0; success return 1
uint24 ML_StartPlaySong( uint24 highlight, RAM_SONG_INFO_T *songStruct, uint24 *songIndex, uint24 *totalSong );
uint24 ML_StopPlaySong( FLASH_GROUP_NAME_T *item_list, uint24 *num_item );
uint24 ML_PlayPreviousSong( RAM_SONG_INFO_T *songStruct, uint24 *songIndex );
uint24 ML_PlayNextSong( RAM_SONG_INFO_T *songStruct, uint24 *songIndex );
uint24 ML_StartPlayAllSong( uint24 hasSD, RAM_SONG_INFO_T *songStruct, uint24 *songIndex, uint24 *totalSong );
uint24 ML_PlayAllNextSong( RAM_SONG_INFO_T *songStruct, uint24 *songIndex );
uint24 ML_PlayAllPreviousSong( RAM_SONG_INFO_T *songStruct, uint24 *songIndex );
uint24 ML_StopPlayAll( FLASH_GROUP_NAME_T *item_list, uint24 *num_item );
uint24 ML_RetrieveSong( uint24 highlight, RAM_SONG_INFO_T *songStruct );
void ML_setPlayMode( uint24 mode );
#endif

/*==================================================================================================
FUNCTION: MusicLib Message functions

DESCRIPTION:
   Description of this specific function.

ARGUMENTS PASSED:
   parm_name1    - parm_name1 description
   parm_name2    - parm_name2 description

RETURN VALUE:
   return parameter description

PRE-CONDITIONS:
   None

POST-CONDITIONS:
   None

IMPORTANT NOTES:
   None

==================================================================================================*/
void ML_InitLibraryParameter(void);
void ML_AddEntryToLibrary(uint24 Media_type, RAM_SONG_INFO_T *song_info, int16 option);
void ML_FlushLibraryToFlash(uint24 Media_type);
#ifdef _NEWMUSIC_
//void ML_UpdateNewMusic(uint24 Media_type);
void ML_UpdateNewMusic(void);
#endif
INT ML_Buildl2SecTable(void);
INT ML_BuildSecTableFile(void);

//void ML_LoadOnTheGo(uint24 HasSD);	/* call at playlist2init.c to load the list */
//void ML_LoadOnTheGo(void);	/* call at playlist2init.c to load the list */
void ML_LoadOnTheGo(uint24 IsColdBoot);
void ML_SaveOnTheGo(void);	/* call at shutdownmenu.c to save the list */
void ML_ChkOnTheGo(uint24 Media_type, uint24 _X * temp_path, uint24 newID);
void ML_ChOnTheGo(uint24 Media_type, uint24 value);
void ML_UpdateOnTheGo(void);

void ML_InitVoiceParameter(void);
void ML_LoadVoiceParameter(uint24 Rec_type);
void ML_AddEntryToVoice(uint24 Rec_type, RAM_SONG_INFO_T *song_info, uint24 action);
void ML_FlushVoiceToFlash(uint24 Rec_type);

void ML_MergeLibraryTables(void);

/*
#ifndef _PC_SIMULATION_
int _reentrant MusicLibBrowseModuleInit(struct ModuleEntry *pModuleTableEntry);
int _reentrant MusicLibBrowseModuleProcessQueue(union WaitMask Signals,struct MessageQueueDescriptor * pQueue);

int _reentrant MusicLibBuildModuleInit(struct ModuleEntry *pModuleTableEntry);
int _reentrant MusicLibBuildModuleProcessQueue(union WaitMask Signals,struct MessageQueueDescriptor * pQueue);
#endif
*/
/*================================================================================================*/

#ifdef _USE_FS_MUSICLIB_
// Siukoon 2005-02-28
#define MUSICLIB_STDRIVE_NUMBER	    (0)
//#define MUSICLIB_FILESIZE_IN_BYTE   (1048576)
//#define MUSICLIB_FILESIZE_IN_BYTE   (1048576*2)
#ifdef _USE_16MDB_
//#define MUSICLIB_FILESIZE_IN_BYTE   ((2097152*2)*4)
#define MUSICLIB_FILESIZE_IN_BYTE   (16777216)
#else
#define MUSICLIB_FILESIZE_IN_BYTE   (2097152*2)
#endif
#define MUSICLIB_FLASHPART_FILESIZE_IN_BYTE   (MUSICLIB_FILESIZE_IN_BYTE/2)
#define WORD_PER_SECTOR             (171)
#define BYTE_PER_SECTOR             (512)
#define MUSICLIB_FILESIZE_IN_SECTOR (MUSICLIB_FILESIZE_IN_BYTE/BYTE_PER_SECTOR)
#define MUSICLIB_FLASHPART_FILESIZE_IN_SECTOR (MUSICLIB_FLASHPART_FILESIZE_IN_BYTE/BYTE_PER_SECTOR)

#define LB_BYTE_PER_SECTOR          (2048)
#define LB_MUSICLIB_FILESIZE_IN_SECTOR (MUSICLIB_FILESIZE_IN_BYTE/LB_BYTE_PER_SECTOR)	//2048
#define LB_MUSICLIB_FLASHPART_FILESIZE_IN_SECTOR (MUSICLIB_FLASHPART_FILESIZE_IN_BYTE/LB_BYTE_PER_SECTOR)	//1024

#ifndef _PC_SIMULATION_
#define MUSICLIB_FAT_ERROR_NOT_ENOUGH_MEDIA_SPACE	1
#define MUSICLIB_FAT_ERROR_CREATE_FILE			2
#define MUSICLIB_FAT_ERROR_DELETE_FILE			3
#define MUSICLIB_FAT_ERROR_OPEN_FILE			4
#endif

/////////////////////

#define LONG_SIZE_IN_BYTE				(6)

//#define SEC_TABLE_SIZE_IN_LONG			(4096)
//#define SEC_TABLE_SIZE_IN_LONG			(MUSICLIB_FILESIZE_IN_SECTOR)	//8192
#define SEC_TABLE_SIZE_IN_LONG			(8192)
#define SEC_TABLE_SIZE_IN_BYTE			(SEC_TABLE_SIZE_IN_LONG*LONG_SIZE_IN_BYTE)	//49152

#define SEC_TABLE_FLASHPART_SIZE_IN_LONG	(SEC_TABLE_SIZE_IN_LONG/2)
#define SEC_TABLE_FLASHPART_SIZE_IN_BYTE	(SEC_TABLE_SIZE_IN_BYTE/2)

#define NUM_OF_LONG_PER_512SEC				(85)
//#define NUM_OF_LEVEL2_SEC				(50)
//#define NUM_OF_LEVEL2_FLASHPART_SEC		(25)
#define NUM_OF_LEVEL2_SEC				(98)	/* 49x2 */
#define NUM_OF_LEVEL2_FLASHPART_SEC		(49)	/* 4096/85 */
//#define LEVEL2_SEC_TABLE_WORDSIZE		(NUM_OF_LEVEL2_SEC*2)
#define LEVEL2_TABLE_SIZE				(NUM_OF_LEVEL2_SEC*2)
#define SEC_TABLE_FLASH_START_SEC		(0x00)
#define SEC_TABLE_SD_START_SEC			(NUM_OF_LEVEL2_FLASHPART_SEC)

#ifdef _USE_16MDB_
#define SEC_TABLE_FILESIZE_IN_BYTE		((BYTE_PER_SECTOR*NUM_OF_LEVEL2_SEC)*4)
#else
#define SEC_TABLE_FILESIZE_IN_BYTE		(BYTE_PER_SECTOR*NUM_OF_LEVEL2_SEC)
#endif
#define BYTE_FILL_INTERDB_GAP			(((NUM_OF_LEVEL2_FLASHPART_SEC*NUM_OF_LONG_PER_512SEC)-SEC_TABLE_FLASHPART_SIZE_IN_LONG)*LONG_SIZE_IN_BYTE+2)


#define LB_SEC_TABLE_SIZE_IN_LONG			(LB_MUSICLIB_FILESIZE_IN_SECTOR)	//2048
#define LB_SEC_TABLE_SIZE_IN_BYTE			(LB_SEC_TABLE_SIZE_IN_LONG*LONG_SIZE_IN_BYTE)	//12288

#define LB_SEC_TABLE_FLASHPART_SIZE_IN_LONG	(LB_SEC_TABLE_SIZE_IN_LONG/2)	//1024
#define LB_SEC_TABLE_FLASHPART_SIZE_IN_BYTE	(LB_SEC_TABLE_SIZE_IN_BYTE/2)	//6144

#define NUM_OF_LONG_PER_2048SEC			(341)
#define NUM_OF_LEVEL2_2048SEC				(25)
#define NUM_OF_LEVEL2_FLASHPART_2048SEC		(13)	/* 4096/341=>13 */

//#define BYTE_FILL_INTERDB_GAP_2048			(((NUM_OF_LEVEL2_FLASHPART_2048SEC*NUM_OF_LONG_PER_2048SEC)-LB_SEC_TABLE_FLASHPART_SIZE_IN_LONG)*LONG_SIZE_IN_BYTE+8)	//2016+8=2024
#define BYTE_FILL_INTERDB_GAP_2048			(((341-4)*6)+2)	//2024
#endif

#ifdef __cplusplus
}
#endif

#endif  /* MUSICLIB_GHDR_H */
