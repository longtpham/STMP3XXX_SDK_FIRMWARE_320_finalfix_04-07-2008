/*==================================================================================================

MODULE DESCRIPTION

====================================================================================================
Perception Digital Confidential Proprietary
(c) Copyright PD 2004, All Rights Reserved

MODULE NAME: mf_sf_module_name.c   AUTHOR: Developer Name   CREATION DATE:DD/MM/YYYY

PRODUCT NAMES: All

GENERAL DESCRIPTION:

General description of this grouping of functions.


Source location
----------------------------------------
All files are in directory:
<Project_Directory>\system\msgmodules\software\MusicLib_Player\src\musiclib_flashaccess.c


Portability: All


Revision History:

Modification        Tracking
Author                       Date             Number           Description of Changes
---------------------    ------------        ----------        -------------------------------------
Developer Name            DD/MM/YYYY         PDaaaxxxxx        brief description of changes made


Notes:

====================================================================================================
DESCRIPTION
====================================================================================================

GLOBAL FUNCTIONS:
MF_global_func_name()




LOCAL  FUNCTIONS:
mf_local_func_name()

====================================================================================================


TRACEABILITY MATRIX:
None

==================================================================================================*/

/*==================================================================================================
Conditional Compilation Directives
==================================================================================================*/
#if 1
#define _OPTIMIZE_SKIP_ERROR_CHECK_
#endif

/*==================================================================================================
INCLUDE FILES
==================================================================================================*/
#include	"..\hdr\musiclib_flash_function.h"
#include "resource.h"

#ifdef _PC_SIMULATION_

#if 0
#define _PC_DEBUGPRINT_

#include	<stdio.h>
#include	<stdlib.h>
#endif

#if 1
#define	_ASSERT_
#endif

#ifdef	_ASSERT_
#include	<assert.h>
#endif

#define _X
typedef uint24	INT;

#else	/* !(_PC_SIMULATION_) */
#ifdef _USE_FS_MUSICLIB_
// Siukoon 2005-02-28
#include "fstypes.h"
#include "filesystem.h"
#else
#include	"ddildl.h"
#endif
#endif	/* !(_PC_SIMULATION_) */

// for calling SysWaitOnEvent, Astley 5/8/2005 12:10
#include "menumanager.h"
/*==================================================================================================
LOCAL CONSTANTS
==================================================================================================*/
#define FLASH_DRIVEID				(0)

/* these parameters are for absolute sector access only */
#define DATABASE_ADR				(204800)	/* starting # of sector, each sector 512Bytes */
//#define DATABASE_SIZE				(4096)		/* our DB for 1000 songs is 1MB (ie. for min. 2048 is good enough) */
#define DATABASE_SIZE				(8192)		/* single DB for 1000 songs is 2MB */
#define FLASH_DATABASE_ADR			(DATABASE_ADR)
#define SD_DATABASE_ADR				(FLASH_DATABASE_ADR + DATABASE_SIZE)
/* these parameters are for absolute sector access only */

#define FIRST_SEC_IN_USE			(0x0)

#define EOSEC	(0xFFFFFF)	/* default return value for error case */

#ifdef _PC_SIMULATION_
#define READ_TYPE_NORMAL			(0)	/* for error free compile only, no use */
#define WRITE_TYPE_RANDOM			(0)	/* for error free compile only, no use */
#endif

#if 0
#define FL_LEN_OF_NAME_IN_BYTE		(33)
#define FL_LEN_OF_UNICODE_IN_BYTE	(1)
#define FL_LEN_OF_FILEPATH_IN_BYTE	(21)
#else
#define FL_LEN_OF_NAME_IN_BYTE		(123)
#define FL_LEN_OF_UNICODE_IN_BYTE	(1)
#define FL_LEN_OF_FILEPATH_IN_BYTE	(168)
#define FL_LEN_OF_FASTKEY_IN_BYTE	(2)
#define FL_LEN_OF_LONG_FILENAME_IN_BYTE	(168)
#endif

/*================================================================================================*/
/*	Debug check to see the defined values _must_be_equal_ to these, to be operational !
#define NUM_OF_WORD_PER_SEC		170

#define ARTIST_NAME_SEC_SIZE		250
#define ALBUM_NAME_SEC_SIZE			250
#define GENRE_NAME_SEC_SIZE			250
#define SONG_NAME_SEC_SIZE			250

#define PATH_NAME_SEC_SIZE			334
#define SONG_ARRAY_SEC_SIZE			3
#define SONG_YEAR_SEC_SIZE			3
#define ARTIST_SORT_SEC_SIZE		3
#define ARTIST_ASSOC_SEC_SIZE		6
#define ARTIST_SUBASSOC_SEC_SIZE	6

#define ALBUM_SORT_SEC_SIZE			3
#define ALBUM_ASSOC_SEC_SIZE		6

#define GENRE_SORT_SEC_SIZE			3
#define GENRE_ASSOC_SEC_SIZE		6

#define NEW_MUSIC_SEC_SIZE			1
#define ON_THE_FLY_SEC_SIZE			1
#define M3U_MUSIC_SEC_SIZE			1


#define NAME_ITEM_PER_SEC			4
#define PATH_ITEM_PER_SEC			3
#define SONG_ARRAY_ITEM_PER_SEC		340
#define SONG_YEAR_ITEM_PER_SEC		340
#define SORT_ITEM_PER_SEC			340
#define ASSOC_ITEM_PER_SEC			340
#define SUBASSOC_ITEM_PER_SEC		340
#define LIST_ITEM_PER_SEC			170
*/


#define NAME_STRUCT_SIZE			((FL_LEN_OF_NAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD)+FL_LEN_OF_UNICODE_IN_BYTE)
/* simplified for min. code size */
/*
#define ARTIST_NAME_STRUCT_SIZE		NAME_STRUCT_SIZE
#define ALBUM_NAME_STRUCT_SIZE		NAME_STRUCT_SIZE
#define GENRE_NAME_STRUCT_SIZE		NAME_STRUCT_SIZE
#define SONG_NAME_STRUCT_SIZE		NAME_STRUCT_SIZE
*/
#define PATH_NAME_STRUCT_SIZE		((FL_LEN_OF_FILEPATH_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD)+FL_LEN_OF_FASTKEY_IN_BYTE)
#define LONG_FILENAME_STRUCT_SIZE	(FL_LEN_OF_LONG_FILENAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD)
#define SONG_YEAR_STRUCT_SIZE		(1)	/* 1 word */
#define NEW_MUSIC_STRUCT_SIZE		(NUM_OF_SONG_IN_NEW_MUSIC+NUM_OF_SONG_IN_NEW_MUSIC+4)	/* 4*uint24 */
#define ON_THE_FLY_STRUCT_SIZE		(NUM_OF_SONG_IN_ON_THE_FLY+1)

/* 	****the following _must_ be in this order (for *_START_SEC define in sequences)****	*/
#define NAME_SEC_SIZE				(MAX_NUM_OF_SONG/(NUM_OF_WORD_PER_SEC/NAME_STRUCT_SIZE))
/* simplified for min. code size */
/*
#define ARTIST_NAME_SEC_SIZE		(MAX_NUM_OF_ARTIST/(NUM_OF_WORD_PER_SEC/ARTIST_NAME_STRUCT_SIZE))
#define ALBUM_NAME_SEC_SIZE			(MAX_NUM_OF_ALBUM/(NUM_OF_WORD_PER_SEC/ALBUM_NAME_STRUCT_SIZE))
#define GENRE_NAME_SEC_SIZE			(MAX_NUM_OF_GENRE/(NUM_OF_WORD_PER_SEC/GENRE_NAME_STRUCT_SIZE))
#define SONG_NAME_SEC_SIZE			(MAX_NUM_OF_SONG/(NUM_OF_WORD_PER_SEC/SONG_NAME_STRUCT_SIZE))
*/
/* the following not divisible => +1 sector at the end */
#define PATH_NAME_SEC_SIZE			((MAX_NUM_OF_SONG)/(NUM_OF_WORD_PER_SEC/PATH_NAME_STRUCT_SIZE)+1)
#define LONG_FILENAME_SEC_SIZE		((MAX_NUM_OF_SONG)/(NUM_OF_WORD_PER_SEC/LONG_FILENAME_STRUCT_SIZE)+1)
#define SONG_ARRAY_SEC_SIZE			(((MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD)/NUM_OF_WORD_PER_SEC)+1)
#define SONG_YEAR_SEC_SIZE			((MAX_NUM_OF_SONG/NUM_OF_WORD_PER_SEC)+1)
#define ARTIST_SORT_SEC_SIZE		(((MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD)/NUM_OF_WORD_PER_SEC)+1)
#define ARTIST_ASSOC_SEC_SIZE		(((NUM_OF_LIST_IN_CHILD_ARRAY*(MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD))/NUM_OF_WORD_PER_SEC)+1)
#define ARTIST_SUBASSOC_SEC_SIZE	(((NUM_OF_LIST_IN_CHILD_ARRAY*(MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD))/NUM_OF_WORD_PER_SEC)+1)

#define ALBUM_SORT_SEC_SIZE			(((MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD)/NUM_OF_WORD_PER_SEC)+1)
#define ALBUM_ASSOC_SEC_SIZE		(((NUM_OF_LIST_IN_CHILD_ARRAY*(MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD))/NUM_OF_WORD_PER_SEC)+1)

#define GENRE_SORT_SEC_SIZE			(((MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD)/NUM_OF_WORD_PER_SEC)+1)
#define GENRE_ASSOC_SEC_SIZE		(((NUM_OF_LIST_IN_CHILD_ARRAY*(MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD))/NUM_OF_WORD_PER_SEC)+1)

#define YEAR_SORT_SEC_SIZE			(((MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD)/NUM_OF_WORD_PER_SEC)+1)
#define YEAR_ASSOC_SEC_SIZE			(((NUM_OF_LIST_IN_CHILD_ARRAY*(MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD))/NUM_OF_WORD_PER_SEC)+1)

#define NEW_MUSIC_SEC_SIZE			((NEW_MUSIC_STRUCT_SIZE/NUM_OF_WORD_PER_SEC)+1)
#define ON_THE_FLY_SEC_SIZE			((NUM_OF_SONG_IN_ON_THE_FLY/NUM_OF_WORD_PER_SEC)+1)
#define ON_THE_FLY_PATH_SEC_SIZE	(NUM_OF_SONG_IN_ON_THE_FLY/(NUM_OF_WORD_PER_SEC/PATH_NAME_STRUCT_SIZE)+1)
#define M3U_MUSIC_SEC_SIZE			(((NUM_OF_LIST_IN_NEW_MUSIC*NUM_OF_SONG_IN_NEW_MUSIC)/NUM_OF_WORD_PER_SEC)+1)

#define SONG_INFO_SEC_SIZE			(((MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD)/NUM_OF_WORD_PER_SEC)+1)
#define AUDIBLE_SEC_SIZE			(((NUM_OF_AUDIBLE_FILE/NUM_OF_ITEM_IN_ONE_WORD)/NUM_OF_WORD_PER_SEC)+1)
#define AUDIBLE_NAME_SEC_SIZE		(NUM_OF_AUDIBLE_FILE/(NUM_OF_WORD_PER_SEC/NAME_STRUCT_SIZE))
#define AUDIBLE_PATH_SEC_SIZE		(NUM_OF_AUDIBLE_FILE/(NUM_OF_WORD_PER_SEC/PATH_NAME_STRUCT_SIZE)+1)
#define AUDIBLE_LONG_FILENAME_SEC_SIZE	(NUM_OF_AUDIBLE_FILE/(NUM_OF_WORD_PER_SEC/LONG_FILENAME_STRUCT_SIZE)+1)

/* the following not divisible => +1 sector at the end */
#define VOICE_NAME_SEC_SIZE			(NAME_SEC_SIZE)
#define VOICE_PATH_SEC_SIZE			(PATH_NAME_STRUCT_SIZE)
#define VOICE_MARKTABLE_SEC_SIZE	((ML_VOICE_MARKTABLE_STRUCT_SIZE/NUM_OF_WORD_PER_SEC)+1)
#define VOICE_INFO_SEC_SIZE			((ML_VOICE_INFO_STRUCT_SIZE/NUM_OF_WORD_PER_SEC)+1)
#define VOICE_ARRAY_SEC_SIZE		(((MAX_NUM_OF_VOICE/NUM_OF_ITEM_IN_ONE_WORD)/NUM_OF_WORD_PER_SEC)+1)

/* the following not divisible => +1 sector at the end */
#define FMREC_NAME_SEC_SIZE			(NAME_SEC_SIZE)
#define FMREC_PATH_SEC_SIZE			(PATH_NAME_STRUCT_SIZE)
#define FMREC_MARKTABLE_SEC_SIZE	((ML_FMREC_MARKTABLE_STRUCT_SIZE/NUM_OF_WORD_PER_SEC)+1)
#define FMREC_INFO_SEC_SIZE			((ML_FMREC_INFO_STRUCT_SIZE/NUM_OF_WORD_PER_SEC)+1)
#define FMREC_ARRAY_SEC_SIZE		(((MAX_NUM_OF_FMREC/NUM_OF_ITEM_IN_ONE_WORD)/NUM_OF_WORD_PER_SEC)+1)

/* the following not divisible => +1 sector at the end */
#define PHOTO_NAME_SEC_SIZE			(NAME_SEC_SIZE)
#define PHOTO_PATH_SEC_SIZE			(PATH_NAME_STRUCT_SIZE)
#define PHOTO_MARKTABLE_SEC_SIZE	((ML_PHOTO_MARKTABLE_STRUCT_SIZE/NUM_OF_WORD_PER_SEC)+1)
#define PHOTO_INFO_SEC_SIZE			((ML_PHOTO_INFO_STRUCT_SIZE/NUM_OF_WORD_PER_SEC)+1)
#define PHOTO_ARRAY_SEC_SIZE		(((MAX_NUM_OF_PHOTO/NUM_OF_ITEM_IN_ONE_WORD)/NUM_OF_WORD_PER_SEC)+1)

/* the following not divisible => +1 sector at the end */
#define MVIDEO_NAME_SEC_SIZE		(NAME_SEC_SIZE)
#define MVIDEO_PATH_SEC_SIZE		(PATH_NAME_STRUCT_SIZE)
#define MVIDEO_MARKTABLE_SEC_SIZE	((ML_MVIDEO_MARKTABLE_STRUCT_SIZE/NUM_OF_WORD_PER_SEC)+1)
#define MVIDEO_INFO_SEC_SIZE		((ML_MVIDEO_INFO_STRUCT_SIZE/NUM_OF_WORD_PER_SEC)+1)
#define MVIDEO_ARRAY_SEC_SIZE		(((MAX_NUM_OF_MVIDEO/NUM_OF_ITEM_IN_ONE_WORD)/NUM_OF_WORD_PER_SEC)+1)

#define SONG_MERGE_SEC_SIZE			(((2*(MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD))/NUM_OF_WORD_PER_SEC)+1)
#define TRACKNUM_ARRAY_SEC_SIZE     SONG_ARRAY_SEC_SIZE
/* simplified for min. code size */
/*
#define SONG_ARTIST_SEC_SIZE		(((MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD)/NUM_OF_WORD_PER_SEC)+1)
#define SONG_ALBUM_SEC_SIZE			(((MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD)/NUM_OF_WORD_PER_SEC)+1)
#define SONG_GENRE_SEC_SIZE			(((MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD)/NUM_OF_WORD_PER_SEC)+1)
#define SONG_YEAR_SEC_SIZE			(((MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD)/NUM_OF_WORD_PER_SEC)+1)
*/
/*================================================================================================*/
/* _do_not_change_the_order_ of following define */
#define ARTIST_NAME_START_SEC		FIRST_SEC_IN_USE
#define ALBUM_NAME_START_SEC		(ARTIST_NAME_START_SEC + NAME_SEC_SIZE)
#define GENRE_NAME_START_SEC		(ALBUM_NAME_START_SEC + NAME_SEC_SIZE)
#define SONG_NAME_START_SEC			(GENRE_NAME_START_SEC + NAME_SEC_SIZE)
#define PATH_NAME_START_SEC			(SONG_NAME_START_SEC + NAME_SEC_SIZE)

/*
#define ALBUM_NAME_START_SEC		(ARTIST_NAME_START_SEC + ARTIST_NAME_SEC_SIZE)
#define GENRE_NAME_START_SEC		(ALBUM_NAME_START_SEC + ALBUM_NAME_SEC_SIZE)
#define SONG_NAME_START_SEC			(GENRE_NAME_START_SEC + GENRE_NAME_SEC_SIZE)
#define PATH_NAME_START_SEC			(SONG_NAME_START_SEC + SONG_NAME_SEC_SIZE)
*/
#define SONG_ARRAY_START_SEC		(PATH_NAME_START_SEC + PATH_NAME_SEC_SIZE)
/* the last 5 words of last sector of flash_song_array stores the num_of_* variables */
#define FLASH_NUM_OF_START_SEC		(SONG_ARRAY_START_SEC + SONG_ARRAY_SEC_SIZE - 1)
#define SONG_YEAR_START_SEC			(SONG_ARRAY_START_SEC + SONG_ARRAY_SEC_SIZE)
#define ARTIST_SORT_START_SEC		(SONG_YEAR_START_SEC + SONG_YEAR_SEC_SIZE)
#define ARTIST_ASSOC_START_SEC		(ARTIST_SORT_START_SEC + ARTIST_SORT_SEC_SIZE)
#define ARTIST_SUBASSOC_START_SEC 	(ARTIST_ASSOC_START_SEC + ARTIST_ASSOC_SEC_SIZE)

#define ALBUM_SORT_START_SEC		(ARTIST_SUBASSOC_START_SEC + ARTIST_SUBASSOC_SEC_SIZE)
#define ALBUM_ASSOC_START_SEC		(ALBUM_SORT_START_SEC + ALBUM_SORT_SEC_SIZE)

#define GENRE_SORT_START_SEC		(ALBUM_ASSOC_START_SEC + ALBUM_ASSOC_SEC_SIZE)
#define GENRE_ASSOC_START_SEC		(GENRE_SORT_START_SEC + GENRE_SORT_SEC_SIZE)

#define YEAR_SORT_START_SEC			(GENRE_ASSOC_START_SEC + GENRE_ASSOC_SEC_SIZE)
#define YEAR_ASSOC_START_SEC		(YEAR_SORT_START_SEC + YEAR_SORT_SEC_SIZE)

#define NEW_MUSIC_START_SEC			(YEAR_ASSOC_START_SEC + YEAR_ASSOC_SEC_SIZE)
#define ON_THE_FLY_START_SEC		(NEW_MUSIC_START_SEC + NEW_MUSIC_SEC_SIZE)
#define ON_THE_FLY_PATH_START_SEC	(ON_THE_FLY_START_SEC + ON_THE_FLY_SEC_SIZE)
#define M3U_MUSIC_START_SEC			(ON_THE_FLY_PATH_START_SEC + ON_THE_FLY_PATH_SEC_SIZE)

/*
#define SONG_ALBUM_START_SEC		(SONG_ARTIST_START_SEC + SONG_ARTIST_SEC_SIZE)
#define SONG_GENRE_START_SEC		(SONG_ALBUM_START_SEC + SONG_ALBUM_SEC_SIZE)
#define SONG_YEAR_START_SEC			(SONG_GENRE_START_SEC + SONG_GENRE_SEC_SIZE)
*/
#define SONG_INFO_ARTIST_START_SEC	(M3U_MUSIC_START_SEC + M3U_MUSIC_SEC_SIZE)
#define SONG_INFO_ALBUM_START_SEC	(SONG_INFO_ARTIST_START_SEC + SONG_INFO_SEC_SIZE)
#define SONG_INFO_GENRE_START_SEC	(SONG_INFO_ALBUM_START_SEC + SONG_INFO_SEC_SIZE)
#define SONG_INFO_YEAR_START_SEC	(SONG_INFO_GENRE_START_SEC + SONG_INFO_SEC_SIZE)

#define AUDIBLE_START_SEC			(SONG_INFO_YEAR_START_SEC + SONG_INFO_SEC_SIZE)
#define AUDIBLE_NUM_OF_START_SEC	(AUDIBLE_START_SEC + AUDIBLE_SEC_SIZE - 1)
#define AUDIBLE_NAME_START_SEC		(AUDIBLE_START_SEC + AUDIBLE_SEC_SIZE)
#define AUDIBLE_PATH_START_SEC		(AUDIBLE_NAME_START_SEC + AUDIBLE_NAME_SEC_SIZE)

#define VOICE_NAME_START_SEC		(AUDIBLE_PATH_START_SEC + AUDIBLE_PATH_SEC_SIZE)
#define VOICE_PATH_START_SEC		(VOICE_NAME_START_SEC + VOICE_NAME_SEC_SIZE)
#define VOICE_MARKTABLE_START_SEC	(VOICE_PATH_START_SEC + VOICE_PATH_SEC_SIZE)
#define VOICE_INFO_START_SEC		(VOICE_MARKTABLE_START_SEC + VOICE_MARKTABLE_SEC_SIZE)
#define VOICE_ARRAY_START_SEC		(VOICE_INFO_START_SEC + VOICE_INFO_SEC_SIZE)
#define VOICE_NUM_OF_START_SEC		(VOICE_ARRAY_START_SEC + VOICE_ARRAY_SEC_SIZE - 1)

#define FMREC_NAME_START_SEC		(VOICE_ARRAY_START_SEC + VOICE_ARRAY_SEC_SIZE)
#define FMREC_PATH_START_SEC		(FMREC_NAME_START_SEC + FMREC_NAME_SEC_SIZE)
#define FMREC_MARKTABLE_START_SEC	(FMREC_PATH_START_SEC + FMREC_PATH_SEC_SIZE)
#define FMREC_INFO_START_SEC		(FMREC_MARKTABLE_START_SEC + VOICE_MARKTABLE_SEC_SIZE)
#define FMREC_ARRAY_START_SEC		(FMREC_INFO_START_SEC + VOICE_INFO_SEC_SIZE)
#define FMREC_NUM_OF_START_SEC		(FMREC_ARRAY_START_SEC + FMREC_ARRAY_SEC_SIZE - 1)

#define ARTIST_MERGE_START_SEC		(FMREC_ARRAY_START_SEC + FMREC_ARRAY_SEC_SIZE)
#define ALBUM_MERGE_START_SEC		(ARTIST_MERGE_START_SEC + SONG_MERGE_SEC_SIZE)
#define GENRE_MERGE_START_SEC		(ALBUM_MERGE_START_SEC + SONG_MERGE_SEC_SIZE)
#define SONG_MERGE_START_SEC		(GENRE_MERGE_START_SEC + SONG_MERGE_SEC_SIZE)
#define YEAR_MERGE_START_SEC		(SONG_MERGE_START_SEC + SONG_MERGE_SEC_SIZE)

#define PHOTO_NAME_START_SEC		(YEAR_MERGE_START_SEC + SONG_MERGE_SEC_SIZE)
#define PHOTO_PATH_START_SEC		(PHOTO_NAME_START_SEC + PHOTO_NAME_SEC_SIZE)
#define PHOTO_MARKTABLE_START_SEC	(PHOTO_PATH_START_SEC + PHOTO_PATH_SEC_SIZE)
#define PHOTO_INFO_START_SEC		(PHOTO_MARKTABLE_START_SEC + VOICE_MARKTABLE_SEC_SIZE)
#define PHOTO_ARRAY_START_SEC		(PHOTO_INFO_START_SEC + VOICE_INFO_SEC_SIZE)
#define PHOTO_NUM_OF_START_SEC		(PHOTO_ARRAY_START_SEC + PHOTO_ARRAY_SEC_SIZE - 1)

#define MVIDEO_NAME_START_SEC		(PHOTO_ARRAY_START_SEC + PHOTO_ARRAY_SEC_SIZE)
#define MVIDEO_PATH_START_SEC		(MVIDEO_NAME_START_SEC + MVIDEO_NAME_SEC_SIZE)
#define MVIDEO_MARKTABLE_START_SEC	(MVIDEO_PATH_START_SEC + MVIDEO_PATH_SEC_SIZE)
#define MVIDEO_INFO_START_SEC		(MVIDEO_MARKTABLE_START_SEC + VOICE_MARKTABLE_SEC_SIZE)
#define MVIDEO_ARRAY_START_SEC		(MVIDEO_INFO_START_SEC + VOICE_INFO_SEC_SIZE)
#define MVIDEO_NUM_OF_START_SEC		(MVIDEO_ARRAY_START_SEC + MVIDEO_ARRAY_SEC_SIZE - 1)

// Note: should be located here, otherwise will corrupt other sorting field
#define LONG_FILENAME_START_SEC		(MVIDEO_ARRAY_START_SEC + MVIDEO_ARRAY_SEC_SIZE)
#define AUDIBLE_LONG_FILENAME_START_SEC	(LONG_FILENAME_START_SEC + LONG_FILENAME_SEC_SIZE)
#define TRACKNUM_ARRAY_START_SEC    (AUDIBLE_LONG_FILENAME_START_SEC+AUDIBLE_LONG_FILENAME_SEC_SIZE)

/*================================================================================================*/
#define NAME_ITEM_PER_SEC			(NUM_OF_WORD_PER_SEC/NAME_STRUCT_SIZE)
/* simplified for min. code size */
//#define NAME_ITEM_PER_SEC			(NUM_OF_WORD_PER_SEC/SONG_NAME_STRUCT_SIZE)
#define ARTIST_NAME_ITEM_PER_SEC	(NUM_OF_WORD_PER_SEC/ARTIST_NAME_STRUCT_SIZE)
#define ALBUM_NAME_ITEM_PER_SEC		(NUM_OF_WORD_PER_SEC/ALBUM_NAME_STRUCT_SIZE)
#define GENRE_NAME_ITEM_PER_SEC		(NUM_OF_WORD_PER_SEC/GENRE_NAME_STRUCT_SIZE)
#define SONG_NAME_ITEM_PER_SEC		(NUM_OF_WORD_PER_SEC/SONG_NAME_STRUCT_SIZE)

#define PATH_NAME_ITEM_PER_SEC		(NUM_OF_WORD_PER_SEC/PATH_NAME_STRUCT_SIZE)
#define LONG_FILENAME_ITEM_PER_SEC	(NUM_OF_WORD_PER_SEC/LONG_FILENAME_STRUCT_SIZE)
/* simplified for min. code size */
//#define SONG_ARRAY_ITEM_PER_SEC		(NUM_OF_WORD_PER_SEC*NUM_OF_ITEM_IN_ONE_WORD)
#define SONG_YEAR_ITEM_PER_SEC		(NUM_OF_WORD_PER_SEC)
#define INDEX_ITEM_PER_SEC			(NUM_OF_WORD_PER_SEC*NUM_OF_ITEM_IN_ONE_WORD)
/* simplified for min. code size */
/*
#define SORT_ITEM_PER_SEC			(NUM_OF_WORD_PER_SEC*NUM_OF_ITEM_IN_ONE_WORD)
#define ASSOC_ITEM_PER_SEC			(NUM_OF_WORD_PER_SEC*NUM_OF_ITEM_IN_ONE_WORD)
#define SUBASSOC_ITEM_PER_SEC		(NUM_OF_WORD_PER_SEC*NUM_OF_ITEM_IN_ONE_WORD)
#define AUDIBLE_ITEM_PER_SEC		(NUM_OF_WORD_PER_SEC*NUM_OF_ITEM_IN_ONE_WORD)
*/
#define LIST_ITEM_PER_SEC			(NUM_OF_WORD_PER_SEC)

/*================================================================================================*/
/* address according to array id */
/* use for access the value of variable, address of the physical storage */
#define ADDR_FLASH_NUM_OF_SONG			(169)
#define ADDR_FLASH_NUM_OF_ARTIST		(168)
#define ADDR_FLASH_NUM_OF_ALBUM			(167)
#define ADDR_FLASH_NUM_OF_GENRE			(166)
#define ADDR_FLASH_NUM_OF_YEAR			(165)
#define ADDR_FLASH_NUM_OF_VOICE			((MAX_NUM_OF_VOICE/NUM_OF_ITEM_IN_ONE_WORD)-(((MAX_NUM_OF_VOICE/NUM_OF_ITEM_IN_ONE_WORD)/NUM_OF_WORD_PER_SEC)*NUM_OF_WORD_PER_SEC))
#define ADDR_FLASH_NUM_OF_FMREC			((MAX_NUM_OF_FMREC/NUM_OF_ITEM_IN_ONE_WORD)-(((MAX_NUM_OF_FMREC/NUM_OF_ITEM_IN_ONE_WORD)/NUM_OF_WORD_PER_SEC)*NUM_OF_WORD_PER_SEC))
#define ADDR_FLASH_NUM_OF_PHOTO			((MAX_NUM_OF_PHOTO/NUM_OF_ITEM_IN_ONE_WORD)-(((MAX_NUM_OF_PHOTO/NUM_OF_ITEM_IN_ONE_WORD)/NUM_OF_WORD_PER_SEC)*NUM_OF_WORD_PER_SEC))
#define ADDR_FLASH_NUM_OF_MVIDEO		((MAX_NUM_OF_MVIDEO/NUM_OF_ITEM_IN_ONE_WORD)-(((MAX_NUM_OF_MVIDEO/NUM_OF_ITEM_IN_ONE_WORD)/NUM_OF_WORD_PER_SEC)*NUM_OF_WORD_PER_SEC))

#define ADDR_FLASH_NUM_OF_NEW_1DAY		(NUM_OF_SONG_IN_NEW_MUSIC+NUM_OF_SONG_IN_NEW_MUSIC+1)
#define ADDR_FLASH_NUM_OF_NEW_1WEEK		(NUM_OF_SONG_IN_NEW_MUSIC+NUM_OF_SONG_IN_NEW_MUSIC+2)
#define ADDR_FLASH_NUM_OF_NEW_1MONTH	(NUM_OF_SONG_IN_NEW_MUSIC+NUM_OF_SONG_IN_NEW_MUSIC+3)
#define ADDR_NUM_OF_AUDIBLE				((NUM_OF_AUDIBLE_FILE/NUM_OF_ITEM_IN_ONE_WORD)-(((NUM_OF_AUDIBLE_FILE/NUM_OF_ITEM_IN_ONE_WORD)/NUM_OF_WORD_PER_SEC)*NUM_OF_WORD_PER_SEC))

/*================================================================================================*/

/*==================================================================================================
LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
LOCAL ENUMS
==================================================================================================*/
//#define	FL_LEN_OF_FLASH_NAME_BUF	(FL_LEN_OF_NAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD)

/*==================================================================================================
LOCAL STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
LOCAL VARIABLES
==================================================================================================*/
#ifdef _PC_SIMULATION_
uint24 mem_sec_artist_name[NUM_OF_MEDIA][NAME_SEC_SIZE][NUM_OF_READWORD_PER_SEC];
uint24 mem_sec_album_name[NUM_OF_MEDIA][NAME_SEC_SIZE][NUM_OF_READWORD_PER_SEC];
uint24 mem_sec_genre_name[NUM_OF_MEDIA][NAME_SEC_SIZE][NUM_OF_READWORD_PER_SEC];
uint24 mem_sec_song_name[NUM_OF_MEDIA][NAME_SEC_SIZE][NUM_OF_READWORD_PER_SEC];
/*
uint24 mem_sec_artist_name[NUM_OF_MEDIA][ARTIST_NAME_SEC_SIZE][NUM_OF_READWORD_PER_SEC];
uint24 mem_sec_album_name[NUM_OF_MEDIA][ALBUM_NAME_SEC_SIZE][NUM_OF_READWORD_PER_SEC];
uint24 mem_sec_genre_name[NUM_OF_MEDIA][GENRE_NAME_SEC_SIZE][NUM_OF_READWORD_PER_SEC];
uint24 mem_sec_song_name[NUM_OF_MEDIA][SONG_NAME_SEC_SIZE][NUM_OF_READWORD_PER_SEC];
*/
uint24 mem_sec_song_array[NUM_OF_MEDIA][SONG_ARRAY_SEC_SIZE][NUM_OF_READWORD_PER_SEC];
uint24 mem_sec_song_path[NUM_OF_MEDIA][PATH_NAME_SEC_SIZE][NUM_OF_READWORD_PER_SEC];
uint24 mem_sec_song_year[NUM_OF_MEDIA][SONG_YEAR_SEC_SIZE][NUM_OF_READWORD_PER_SEC];
uint24 mem_sec_artist_sort[NUM_OF_MEDIA][ARTIST_SORT_SEC_SIZE][NUM_OF_READWORD_PER_SEC];
uint24 mem_sec_artist_assoc[NUM_OF_MEDIA][ARTIST_ASSOC_SEC_SIZE][NUM_OF_READWORD_PER_SEC];
uint24 mem_sec_artist_subassoc[NUM_OF_MEDIA][ARTIST_ASSOC_SEC_SIZE][NUM_OF_READWORD_PER_SEC];
uint24 mem_sec_album_sort[NUM_OF_MEDIA][ALBUM_SORT_SEC_SIZE][NUM_OF_READWORD_PER_SEC];
uint24 mem_sec_album_assoc[NUM_OF_MEDIA][ALBUM_ASSOC_SEC_SIZE][NUM_OF_READWORD_PER_SEC];
uint24 mem_sec_genre_sort[NUM_OF_MEDIA][GENRE_SORT_SEC_SIZE][NUM_OF_READWORD_PER_SEC];
uint24 mem_sec_genre_assoc[NUM_OF_MEDIA][GENRE_ASSOC_SEC_SIZE][NUM_OF_READWORD_PER_SEC];
uint24 mem_sec_year_sort[NUM_OF_MEDIA][YEAR_SORT_SEC_SIZE][NUM_OF_READWORD_PER_SEC];
uint24 mem_sec_year_assoc[NUM_OF_MEDIA][YEAR_ASSOC_SEC_SIZE][NUM_OF_READWORD_PER_SEC];
uint24 mem_sec_new_music[NUM_OF_MEDIA][NEW_MUSIC_SEC_SIZE][NUM_OF_READWORD_PER_SEC];

uint24 mem_sec_song_info_artist[NUM_OF_MEDIA][SONG_INFO_SEC_SIZE][NUM_OF_READWORD_PER_SEC];
uint24 mem_sec_song_info_album[NUM_OF_MEDIA][SONG_INFO_SEC_SIZE][NUM_OF_READWORD_PER_SEC];
uint24 mem_sec_song_info_genre[NUM_OF_MEDIA][SONG_INFO_SEC_SIZE][NUM_OF_READWORD_PER_SEC];
uint24 mem_sec_song_info_year[NUM_OF_MEDIA][SONG_INFO_SEC_SIZE][NUM_OF_READWORD_PER_SEC];
uint24 mem_sec_audible[NUM_OF_MEDIA][AUDIBLE_SEC_SIZE][NUM_OF_READWORD_PER_SEC];
uint24 mem_sec_audible_name[NUM_OF_MEDIA][AUDIBLE_NAME_SEC_SIZE][NUM_OF_READWORD_PER_SEC];
uint24 mem_sec_audible_path[NUM_OF_MEDIA][AUDIBLE_PATH_SEC_SIZE][NUM_OF_READWORD_PER_SEC];
uint24 mem_sec_song_merge[NUM_OF_MEDIA][GENRE_ASSOC_SEC_SIZE][NUM_OF_READWORD_PER_SEC];
#endif

#ifdef _PC_SIMULATION_
COMP_OPT_G uint24 _X sec_temp_buf_X[NUM_OF_READWORD_PER_SEC];
#else
extern uint24 _X sec_temp_buf_X[NUM_OF_READWORD_PER_SEC];
#endif

#ifdef _USE_FS_SECTAB_
extern LONG _X l2_sector_table[LEVEL2_TABLE_SIZE];
extern LONG SectorSize;
extern uint24 num_long_per_sec;
#endif

extern unsigned int g_rsrc_Flash_Error;
/*==================================================================================================
GLOBAL VARIABLES
==================================================================================================*/
extern uint24 g_is_SD_inserted;
#ifdef _USE_FS_MUSICLIB_
// Siukoon 2005-02-28
extern int g_iFileHandle;
extern _reentrant LONG Fread(INT HandleNumber,INT *Buffer,LONG NumBytesToRead,INT Source_Memory,INT MOdulo);
extern _reentrant INT Fseek(INT HandleNumber,LONG NumBytesToSeek,INT SeekPosition);
extern _reentrant LONG Fwrite(INT HandleNumber,INT  *Buffer,LONG NumBytesToWrite,INT Source_Memory,INT modulo);
#endif

/*==================================================================================================
LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
FUNCTION: mf_local_func_name()

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

/*==================================================================================================
GLOBAL FUNCTIONS
==================================================================================================*/
/*==================================================================================================
FUNCTION: MF_global_func_name()

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
#ifdef _PC_SIMULATION_
/*==================================================================================================
FUNCTION: PC simulation utility functions

DESCRIPTION:
drive read/write.

ARGUMENTS PASSED:


RETURN VALUE:


PRE-CONDITIONS:
None

POST-CONDITIONS:
None

IMPORTANT NOTES:
call before using any media as musiclib

==================================================================================================*/
COMP_OPT int setup_mask_lvl(int a) {
	return (0);
}

int24 DataDriveReadSector(uint32 dwSectorNumber, uint24 wLogDriveNumber, INT iType, uint24 *pSectorData) {
	uint24 i;
	uint24 j;
	uint24 Media_type;

	if ((dwSectorNumber >= SD_DATABASE_ADR) && (dwSectorNumber < (SD_DATABASE_ADR + DATABASE_SIZE))) {
		dwSectorNumber -= SD_DATABASE_ADR;
		Media_type = MEDIA_TYPE_SD;
	} else if ((dwSectorNumber >= FLASH_DATABASE_ADR) && (dwSectorNumber < (FLASH_DATABASE_ADR + DATABASE_SIZE))) {
		dwSectorNumber -= FLASH_DATABASE_ADR;
		Media_type = MEDIA_TYPE_FLASH;
	} else {
		return(!SUCCESS);
	}

	if (dwSectorNumber >= SONG_MERGE_START_SEC) {
		j = dwSectorNumber-SONG_MERGE_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			pSectorData[i] = mem_sec_song_merge[Media_type][j][i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= AUDIBLE_PATH_START_SEC) {
		j = dwSectorNumber-AUDIBLE_PATH_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			pSectorData[i] = mem_sec_audible_path[Media_type][j][i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= AUDIBLE_NAME_START_SEC) {
		j = dwSectorNumber-AUDIBLE_NAME_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			pSectorData[i] = mem_sec_audible_name[Media_type][j][i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= AUDIBLE_START_SEC) {
		j = dwSectorNumber-AUDIBLE_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			pSectorData[i] = mem_sec_audible[Media_type][j][i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= SONG_INFO_YEAR_START_SEC) {
		j = dwSectorNumber-SONG_INFO_YEAR_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			pSectorData[i] = mem_sec_song_info_year[Media_type][j][i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= SONG_INFO_GENRE_START_SEC) {
		j = dwSectorNumber-SONG_INFO_GENRE_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			pSectorData[i] = mem_sec_song_info_genre[Media_type][j][i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= SONG_INFO_ALBUM_START_SEC) {
		j = dwSectorNumber-SONG_INFO_ALBUM_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			pSectorData[i] = mem_sec_song_info_album[Media_type][j][i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= SONG_INFO_ARTIST_START_SEC) {
		j = dwSectorNumber-SONG_INFO_ARTIST_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			pSectorData[i] = mem_sec_song_info_artist[Media_type][j][i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= NEW_MUSIC_START_SEC) {
		j = dwSectorNumber-NEW_MUSIC_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			pSectorData[i] = mem_sec_new_music[Media_type][j][i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= YEAR_ASSOC_START_SEC) {
		j = dwSectorNumber-YEAR_ASSOC_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			pSectorData[i] = mem_sec_year_assoc[Media_type][j][i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= YEAR_SORT_START_SEC) {
		j = dwSectorNumber-YEAR_SORT_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			pSectorData[i] = mem_sec_year_sort[Media_type][j][i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= GENRE_ASSOC_START_SEC) {
		j = dwSectorNumber-GENRE_ASSOC_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			pSectorData[i] = mem_sec_genre_assoc[Media_type][j][i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= GENRE_SORT_START_SEC) {
		j = dwSectorNumber-GENRE_SORT_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			pSectorData[i] = mem_sec_genre_sort[Media_type][j][i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= ALBUM_ASSOC_START_SEC) {
		j = dwSectorNumber-ALBUM_ASSOC_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			pSectorData[i] = mem_sec_album_assoc[Media_type][j][i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= ALBUM_SORT_START_SEC) {
		j = dwSectorNumber-ALBUM_SORT_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			pSectorData[i] = mem_sec_album_sort[Media_type][j][i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= ARTIST_SUBASSOC_START_SEC) {
		j = dwSectorNumber-ARTIST_SUBASSOC_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			pSectorData[i] = mem_sec_artist_subassoc[Media_type][j][i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= ARTIST_ASSOC_START_SEC) {
		j = dwSectorNumber-ARTIST_ASSOC_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			pSectorData[i] = mem_sec_artist_assoc[Media_type][j][i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= ARTIST_SORT_START_SEC) {
		j = dwSectorNumber-ARTIST_SORT_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			pSectorData[i] = mem_sec_artist_sort[Media_type][j][i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= SONG_YEAR_START_SEC) {
		j = dwSectorNumber-SONG_YEAR_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			pSectorData[i] = mem_sec_song_year[Media_type][j][i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= SONG_ARRAY_START_SEC) {
		j = dwSectorNumber-SONG_ARRAY_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			pSectorData[i] = mem_sec_song_array[Media_type][j][i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= PATH_NAME_START_SEC) {
		j = dwSectorNumber-PATH_NAME_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			pSectorData[i] = mem_sec_song_path[Media_type][j][i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= SONG_NAME_START_SEC) {
		j = dwSectorNumber-SONG_NAME_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			pSectorData[i] = mem_sec_song_name[Media_type][j][i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= GENRE_NAME_START_SEC) {
		j = dwSectorNumber-GENRE_NAME_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			pSectorData[i] = mem_sec_genre_name[Media_type][j][i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= ALBUM_NAME_START_SEC) {
		j = dwSectorNumber-ALBUM_NAME_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			pSectorData[i] = mem_sec_album_name[Media_type][j][i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= ARTIST_NAME_START_SEC) {
		j = dwSectorNumber-ARTIST_NAME_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			pSectorData[i] = mem_sec_artist_name[Media_type][j][i];
		}
		return (SUCCESS);
	}
	return (!SUCCESS);
}

int24 DataDriveWriteSector(uint32 dwSectorNumber, uint24 wLogDriveNumber, INT iType, uint24 *pSectorData) {
	uint24 i;
	uint24 j;
	uint24 Media_type;

	if ((dwSectorNumber >= SD_DATABASE_ADR) && (dwSectorNumber < (SD_DATABASE_ADR + DATABASE_SIZE))) {
		dwSectorNumber -= SD_DATABASE_ADR;
		Media_type = MEDIA_TYPE_SD;
	} else if ((dwSectorNumber >= FLASH_DATABASE_ADR) && (dwSectorNumber < (FLASH_DATABASE_ADR + DATABASE_SIZE))) {
		dwSectorNumber -= FLASH_DATABASE_ADR;
		Media_type = MEDIA_TYPE_FLASH;
	} else {
		return(!SUCCESS);
	}

	if (dwSectorNumber >= SONG_MERGE_START_SEC) {
		j = dwSectorNumber-SONG_MERGE_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			mem_sec_song_merge[Media_type][j][i] = pSectorData[i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= AUDIBLE_PATH_START_SEC) {
		j = dwSectorNumber-AUDIBLE_PATH_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			mem_sec_audible_path[Media_type][j][i] = pSectorData[i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= AUDIBLE_NAME_START_SEC) {
		j = dwSectorNumber-AUDIBLE_NAME_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			mem_sec_audible_name[Media_type][j][i] = pSectorData[i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= AUDIBLE_START_SEC) {
		j = dwSectorNumber-AUDIBLE_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			mem_sec_audible[Media_type][j][i] = pSectorData[i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= SONG_INFO_YEAR_START_SEC) {
		j = dwSectorNumber-SONG_INFO_YEAR_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			mem_sec_song_info_year[Media_type][j][i] = pSectorData[i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= SONG_INFO_GENRE_START_SEC) {
		j = dwSectorNumber-SONG_INFO_GENRE_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			mem_sec_song_info_genre[Media_type][j][i] = pSectorData[i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= SONG_INFO_ALBUM_START_SEC) {
		j = dwSectorNumber-SONG_INFO_ALBUM_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			mem_sec_song_info_album[Media_type][j][i] = pSectorData[i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= SONG_INFO_ARTIST_START_SEC) {
		j = dwSectorNumber-SONG_INFO_ARTIST_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			mem_sec_song_info_artist[Media_type][j][i] = pSectorData[i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= NEW_MUSIC_START_SEC) {
		j = dwSectorNumber-NEW_MUSIC_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			mem_sec_new_music[Media_type][j][i] = pSectorData[i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= YEAR_ASSOC_START_SEC) {
		j = dwSectorNumber-YEAR_ASSOC_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			mem_sec_year_assoc[Media_type][j][i] = pSectorData[i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= YEAR_SORT_START_SEC) {
		j = dwSectorNumber-YEAR_SORT_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			mem_sec_year_sort[Media_type][j][i] = pSectorData[i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= GENRE_ASSOC_START_SEC) {
		j = dwSectorNumber-GENRE_ASSOC_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			mem_sec_genre_assoc[Media_type][j][i] = pSectorData[i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= GENRE_SORT_START_SEC) {
		j = dwSectorNumber-GENRE_SORT_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			mem_sec_genre_sort[Media_type][j][i] = pSectorData[i];
		}
		return (SUCCESS);
	}

	if (dwSectorNumber >= ALBUM_ASSOC_START_SEC) {
		j = dwSectorNumber-ALBUM_ASSOC_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			mem_sec_album_assoc[Media_type][j][i] = pSectorData[i];
		}
		return (SUCCESS);
	}

	if (dwSectorNumber >= ALBUM_SORT_START_SEC) {
		j = dwSectorNumber-ALBUM_SORT_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			mem_sec_album_sort[Media_type][j][i] = pSectorData[i];
		}
		return (SUCCESS);
	}

	if (dwSectorNumber >= ARTIST_SUBASSOC_START_SEC) {
		j = dwSectorNumber-ARTIST_SUBASSOC_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			mem_sec_artist_subassoc[Media_type][j][i] = pSectorData[i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= ARTIST_ASSOC_START_SEC) {
		j = dwSectorNumber-ARTIST_ASSOC_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			mem_sec_artist_assoc[Media_type][j][i] = pSectorData[i];
		}
		return (SUCCESS);
	}
	if (dwSectorNumber >= ARTIST_SORT_START_SEC) {
		j = dwSectorNumber-ARTIST_SORT_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			mem_sec_artist_sort[Media_type][j][i] = pSectorData[i];
		}
		return (SUCCESS);
	}

	if (dwSectorNumber >= SONG_YEAR_START_SEC) {
		j = dwSectorNumber-SONG_YEAR_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			mem_sec_song_year[Media_type][j][i] = pSectorData[i];
		}
		return (SUCCESS);
	}

	if (dwSectorNumber >= SONG_ARRAY_START_SEC) {
		j = dwSectorNumber-SONG_ARRAY_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			mem_sec_song_array[Media_type][j][i] = pSectorData[i];
		}
		return (SUCCESS);
	}

	if (dwSectorNumber >= PATH_NAME_START_SEC) {
		j = dwSectorNumber-PATH_NAME_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			mem_sec_song_path[Media_type][j][i] = pSectorData[i];
		}
		return (SUCCESS);
	}

	if (dwSectorNumber >= SONG_NAME_START_SEC) {
		j = dwSectorNumber-SONG_NAME_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			mem_sec_song_name[Media_type][j][i] = pSectorData[i];
		}
		return (SUCCESS);
	}

	if (dwSectorNumber >= GENRE_NAME_START_SEC) {
		j = dwSectorNumber-GENRE_NAME_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			mem_sec_genre_name[Media_type][j][i] = pSectorData[i];
		}
		return (SUCCESS);
	}

	if (dwSectorNumber >= ALBUM_NAME_START_SEC) {
		j = dwSectorNumber-ALBUM_NAME_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			mem_sec_album_name[Media_type][j][i] = pSectorData[i];
		}
		return (SUCCESS);
	}

	if (dwSectorNumber >= ARTIST_NAME_START_SEC) {
		j = dwSectorNumber-ARTIST_NAME_START_SEC;
		for (i=0;i<NUM_OF_WORD_PER_SEC;i++) {
			mem_sec_artist_name[Media_type][j][i] = pSectorData[i];
		}
		return (SUCCESS);
	}

	return(!SUCCESS);
}
/*================================================================================================*/
#else
RETCODE _reentrant DataDriveWriteSector(WORD wDriveNumber, 
                                        DWORD dwSectorNumber,
                                        SECTOR_BUFFER * Buffer, 
                                        INT iType);
RETCODE _reentrant DataDriveReadSector(DWORD dwSectorNumber,
                            WORD wDriveNumber,
                            INT iType,
                            SECTOR_BUFFER * Buffer);

#ifdef _USEFLASH_WRITE_
// //* CACHE FUNCTIONALITY *///////////////
///////////////////////////////////////////
///////////////////////////////////////////
typedef struct {
	INT CacheValid;
    INT DeviceNum;
	LONG SectorNumber;
	INT WriteAttribute;
	INT Mode;
	INT CacheCounter;
	INT _X *CacheMem;
} ML_tCACHEDESCR;


// X space allocated for cached sector buffers.
//defines for 2K sector caches
#define ML_MAXCACHES     (9)
#define ML_CACHEBUFSIZE  (720) //Ideally is should be 705 (4 sectors of 528 bytes each i.e. 512 + 16 redundant bytes) but its 728 to match MB-CMA sector buffer size

#define CACHE_READCOUNTER         200 //keep a higher count here so chance of flushing a "write" sector are reduced so you will mainly use the read-only sectors in case of a cache miss
#define CACHE_WRITECOUNTER        100

#ifdef MERGE_FUNCTION
#define ML_CacheDesc Merge_ML_CacheDesc
#define ML_CacheBuffer Merge_ML_CacheBuffer
#define InitandUseCache Merge_InitandUseCache
#define FlushAndCloseCache Merge_FlushAndCloseCache
#define IncrementCacheCounters Merge_IncrementCacheCounters
#define DataDriveReadSectorCache Merge_DataDriveReadSectorCache
#define DataDriveWriteSectorCache Merge_DataDriveWriteSectorCache

#else

#ifdef VOICE_FUNCTION
#define ML_CacheDesc Voice_ML_CacheDesc
#define ML_CacheBuffer Voice_ML_CacheBuffer
#define InitandUseCache Voice_InitandUseCache
#define FlushAndCloseCache Voice_FlushAndCloseCache
#define IncrementCacheCounters Voice_IncrementCacheCounters
#define DataDriveReadSectorCache Voice_DataDriveReadSectorCache
#define DataDriveWriteSectorCache Voice_DataDriveWriteSectorCache

#else

//this will be for the build function
#define ML_CacheDesc Build_ML_CacheDesc
#define ML_CacheBuffer Build_ML_CacheBuffer
#define InitandUseCache Build_InitandUseCache
#define FlushAndCloseCache Build_FlushAndCloseCache
#define IncrementCacheCounters Build_IncrementCacheCounters
#define DataDriveReadSectorCache Build_DataDriveReadSectorCache
#define DataDriveWriteSectorCache Build_DataDriveWriteSectorCache
#endif
#endif



ML_tCACHEDESCR _X ML_CacheDesc[ML_MAXCACHES];
int _X ML_CacheBuffer[ML_CACHEBUFSIZE*ML_MAXCACHES];
extern int ML_UseCache;

void InitandUseCache(void)
{
if(!ML_UseCache)
{
    int i,j;
    // Setup Cache
    for(i=0; i < ML_MAXCACHES;i++)
    {   
        ML_CacheDesc[i].CacheValid = 0;
        ML_CacheDesc[i].WriteAttribute = 0;
		ML_CacheDesc[i].SectorNumber = 0;
        
    }
    j = 0;
    for(i=0;i < ML_MAXCACHES;i++)
    {
        ML_CacheDesc[i].CacheMem = (INT _X *) &ML_CacheBuffer[j];
        j += ML_CACHEBUFSIZE;
    }
	ML_UseCache=1;
}
}

INT FlushAndCloseCache(void)
{
  RETCODE rslt;
  int ix;

  if(ML_UseCache)
 {
	  for (ix = 0; ix < ML_MAXCACHES; ix++)
	  {
	    if ((1 == ML_CacheDesc[ix].CacheValid) && (1 == ML_CacheDesc[ix].WriteAttribute))
	    {
	      rslt = DataDriveWriteSector(ML_CacheDesc[ix].DeviceNum,ML_CacheDesc[ix].SectorNumber, (SECTOR_BUFFER*)ML_CacheDesc[ix].CacheMem,WRITE_TYPE_RANDOM);
	      
	      ML_CacheDesc[ix].CacheValid = 0;
		  ML_CacheDesc[ix].WriteAttribute=0;
		  ML_CacheDesc[ix].SectorNumber = 0;
	      if (SUCCESS != rslt)
	      {
	        ML_UseCache=0;
	        return MEDIAWRITE_FAILED;
	      }
	    }
	  	ML_CacheDesc[ix].CacheValid = 0; //even if its not a write cache , disable it.
	  }
}
   ML_UseCache=0;
   return SUCCESS; 
}

void IncrementCacheCounters(void)
{
  INT i;
  for (i = 0; i< ML_MAXCACHES;i++)
  {
    ML_CacheDesc[i].CacheCounter=ML_CacheDesc[i].CacheCounter+1;
  }
}


RETCODE DataDriveReadSectorCache(DWORD dwSectorNumber,
									   WORD wDriveNumber,
									   INT iType,
									   SECTOR_BUFFER * Buffer)
{
if(ML_UseCache)
{
	  INT i=0,j=0,selection,Counter;
	  // First Search if the buffer is available in the cache
	  for (i = 0; i < ML_MAXCACHES; i++)
	  {
	    if (ML_CacheDesc[i].CacheValid)
	    {
	      if((ML_CacheDesc[i].SectorNumber == dwSectorNumber )  && (ML_CacheDesc[i].DeviceNum == wDriveNumber))
	      {
	                ML_CacheDesc[i].Mode = WRITE_TYPE_RANDOM;
	                if (ML_CacheDesc[i].CacheCounter > CACHE_READCOUNTER) //ensure that the sector was not a write sector already before bumping up the counter (read counter >write)
	                	ML_CacheDesc[i].CacheCounter = CACHE_READCOUNTER;
	                for(j=0;j<ML_CACHEBUFSIZE;j++)
					    Buffer[j] = ML_CacheDesc[i].CacheMem[j];
	                return SUCCESS;
	      }
	    }
	  }
	  // Cache Miss, so must read.
	  // Now find the Least recently used Buffer
	  selection = 0;
	  Counter = 0;
	  for (i = 0; i < ML_MAXCACHES; i++)
	  {
	    if (ML_CacheDesc[i].CacheValid)
	    {
	      if (ML_CacheDesc[i].CacheCounter > Counter)
	      {
	        selection = i;
	        Counter = ML_CacheDesc[i].CacheCounter;
	      }
	    }
	    else
	    {
	      selection = i;
	      ML_CacheDesc[selection].CacheValid = 1;
	      break;
	    }
	  }
	  // Flush the sector to the disk, if write attribute was set.
	  if (ML_CacheDesc[selection].WriteAttribute == 1)
	  {
	    if(DataDriveWriteSector(wDriveNumber, ML_CacheDesc[selection].SectorNumber, (SECTOR_BUFFER*) ML_CacheDesc[selection].CacheMem,iType) != SUCCESS)
	      return ERROR;
	  }
	   if(	DataDriveReadSector(dwSectorNumber,wDriveNumber, iType, (SECTOR_BUFFER*) ML_CacheDesc[selection].CacheMem) != SUCCESS)
	     return ERROR;
      
        for(j=0;j<ML_CACHEBUFSIZE;j++)
		    Buffer[j] = ML_CacheDesc[selection].CacheMem[j];

	  ML_CacheDesc[selection].DeviceNum = wDriveNumber;
	  ML_CacheDesc[selection].WriteAttribute = 0;
	  ML_CacheDesc[selection].Mode = WRITE_TYPE_RANDOM;
	  ML_CacheDesc[selection].SectorNumber = dwSectorNumber;
	  IncrementCacheCounters();
	  ML_CacheDesc[selection].CacheCounter = CACHE_READCOUNTER;
	  return SUCCESS;
}
else
{ //cache not enabled , call directly
	if(DataDriveReadSector(dwSectorNumber,wDriveNumber, iType, Buffer)!= SUCCESS)
	return ERROR;
}
	return SUCCESS;
}

RETCODE DataDriveWriteSectorCache(WORD wDriveNumber,
                                        DWORD dwSectorNumber,
                                        SECTOR_BUFFER * Buffer,
                                        INT iType)
{
if(ML_UseCache)
{

	  INT i,j,Counter,selection;
	  // First Search if the buffer is available in the cache
	  for (i = 0; i < ML_MAXCACHES; i++)
	  {
	    if (ML_CacheDesc[i].CacheValid)
	    {
	      if ((ML_CacheDesc[i].SectorNumber == dwSectorNumber) && (ML_CacheDesc[i].DeviceNum == wDriveNumber))
	        break;
	    }
	  }

	  if (i == ML_MAXCACHES)
	  {
	    // Cache Miss, so must read.
	    // Now find the Least recently used Buffer
	    selection = 0;
	    Counter = 0;
	    for (i = 0; i < ML_MAXCACHES; i++)
	    {
	      if (ML_CacheDesc[i].CacheValid)
	      {
	        if (ML_CacheDesc[i].CacheCounter > Counter)
	        {
	          selection = i;
	          Counter = ML_CacheDesc[i].CacheCounter;
	        }
	      }
	      else
	      {
	        ML_CacheDesc[i].CacheValid = 1;
	        selection = i;
	        break;
	      }
	    }
	    // Flush the sector to the disk, if write attribute was set.
	    if (ML_CacheDesc[selection].WriteAttribute == 1)
	    {
	      if(DataDriveWriteSector(wDriveNumber, ML_CacheDesc[selection].SectorNumber,(SECTOR_BUFFER*) ML_CacheDesc[selection].CacheMem,iType) != SUCCESS)
	      {
	        return ERROR;
	      }
	    }
        ML_CacheDesc[selection].SectorNumber = dwSectorNumber;
	    ML_CacheDesc[selection].Mode = WRITE_TYPE_RANDOM;
	    i = selection;
	  }

	    for(j=0;j<ML_CACHEBUFSIZE;j++)
		    ML_CacheDesc[i].CacheMem[j] = Buffer[j];

	    ML_CacheDesc[i].WriteAttribute = 1;
	    ML_CacheDesc[i].CacheCounter = CACHE_WRITECOUNTER;
	    ML_CacheDesc[i].DeviceNum = wDriveNumber;
	    IncrementCacheCounters();
	    return SUCCESS;
}
else
{
	if(DataDriveWriteSector(wDriveNumber,dwSectorNumber,Buffer, iType) != SUCCESS)
	   return ERROR;
}
    return SUCCESS;
}

// //* End CACHE FUNCTIONALITY *///////////////
///////////////////////////////////////////
///////////////////////////////////////////
#else

#define DataDriveReadSectorCache DataDriveReadSector //for non-write non-cache operations

#endif //_USEFLASH_WRITE_ for cache

// William 20050129
// input the interrupt level to mask (for decoder it is 1), output the
// original interrupt level for you to restore
COMP_OPT int setup_mask_lvl(int a)
{
#pragma asm
	move   sr,b1
		rep   #8
		asr   b
		move   #>$3,x0
		and      x0,b
		move   b1,x1   ;backup the original mask

		and      x0,a
		rep   #8
		asl   a
		move   sr,b
		move   #>$fffcff,x0
		and      x0,b
		add   a,b
		move   b1,sr   ;setup the new mask
		move   x1,a
		tst   a
		rts
#pragma endasm
}
// end
#endif	/* _PC_SIMULATION_ */

/*==================================================================================================
FUNCTION: Local utility functions

DESCRIPTION:
Description of this specific function.

ARGUMENTS PASSED:
uint24 index = input ID of item,
sec_id = return sector ID for item,
item_id = return item address in sector

RETURN VALUE:
Return (sec_id=item_id=0xFFFFFF) for undefine item

PRE-CONDITIONS:
None

POST-CONDITIONS:
None

IMPORTANT NOTES:
call before using any media as musiclib

==================================================================================================*/
COMP_OPT void cal_offset(LONG index, uint24 *lb_off) {
	uint24 lb_offset;
#ifdef _USE_16MDB_
	*lb_off = 0;
#else
	if (SectorSize == LB_BYTE_PER_SECTOR) {
		lb_offset = index-((index/4)*4);
		if ((lb_offset) == 1) {
			*lb_off = 171;
		} else if (lb_offset == 2) {
			*lb_off = 342;
		} else if (lb_offset == 3) {
			*lb_off = 512;
		} else {
			*lb_off = 0;
		}
	} else {
		*lb_off = 0;
	}
#endif
}

COMP_OPT void cal_sec(uint24 item_per_sec, uint24 start_sec, uint24 index, uint32 *sec_id, uint24 *in_sec_id) {
	*sec_id = (index/item_per_sec);
	*in_sec_id = index - ((*sec_id)*item_per_sec);
	*sec_id += start_sec;
}

#ifdef _USE_FS_SECTAB_
/*================================================================================================*/
#ifdef USE_FUNC_GET_MAP_SEC
COMP_OPT void get_map_sec_Browse(uint24 Media_type, uint24 dbsec_id, LONG *sec_id) {
#else
#ifdef VOICE_FUNCTION
_reentrant void get_map_sec_Voice(uint24 Media_type, uint24 dbsec_id, LONG *sec_id) {
#else
_reentrant void get_map_sec(uint24 Media_type, uint24 dbsec_id, LONG *sec_id) {
#endif
#endif
	uint24 start_sec;
	uint32 temp_sec;
	uint24 temp_in_sec;
	LONG _X *temp_long_p;

#ifndef _OPTIMIZE_SKIP_ERROR_CHECK_
	if (index >= SEC_TABLE_SIZE_IN_LONG) {
		*sec_id = EOSEC;
		*in_sec_id = EOSEC;
		return;
	}
#endif
	if (Media_type == MEDIA_TYPE_FLASH) {
		start_sec = 0x00;
	} else {
#ifdef _USE_16MDB_
		start_sec = NUM_OF_LEVEL2_FLASHPART_SEC;
#else
		if (SectorSize == BYTE_PER_SECTOR) {
			start_sec = NUM_OF_LEVEL2_FLASHPART_SEC;
		} else {
			start_sec = NUM_OF_LEVEL2_FLASHPART_2048SEC;
		}
#endif
	}
#if 0
#ifdef VOICE_FUNCTION
	get_item_sec_Voice(num_long_per_sec, start_sec, dbsec_id, &temp_sec, &temp_in_sec);
#else
	get_item_sec(num_long_per_sec, start_sec, dbsec_id, &temp_sec, &temp_in_sec);
#endif
#else
#ifdef _USE_16MDB_
	cal_sec(NUM_OF_LONG_PER_512SEC, start_sec, dbsec_id, &temp_sec, &temp_in_sec);
#else
	cal_sec(num_long_per_sec, start_sec, dbsec_id, &temp_sec, &temp_in_sec);
#endif
	/*
	temp_sec = (dbsec_id/NUM_OF_LONG_PER_SEC);
	temp_in_sec = dbsec_id - (temp_sec*NUM_OF_LONG_PER_SEC);
	temp_sec += start_sec;
	*/
#endif
	/*
	if ((temp_sec < 0x00) || (temp_sec >= 50)) {
	_asm("debug");
	}
	*/
	if (DataDriveReadSectorCache(l2_sector_table[temp_sec], FLASH_DRIVEID, (INT)READ_TYPE_NORMAL, sec_temp_buf_X) != SUCCESS) {
		/* error */
	}
	temp_long_p = (LONG _X *) sec_temp_buf_X;
	*sec_id = temp_long_p[temp_in_sec];
	//*sec_id = ((LONG _X *) sec_temp_buf_X)[temp_in_sec*LONG_SIZE_IN_BYTE];

	if (*sec_id < MediaTable[0].FIRSTDataSector) {	/* FAT16 -> 0x221 for 512M : FAT32 -> 0x7EA for 512M : 0xFB6 for 1G */
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,g_rsrc_Flash_Error);
		SysWaitOnEvent(0,0,100);
		_asm("debug");
	}
}
#endif

#if 1	/* replace with 3 generic sec extract function */
#ifdef VOICE_FUNCTION
COMP_OPT void get_item_sec_Voice(uint24 Media_type, uint24 item_per_sec, uint24 start_sec, uint24 index, uint32 *sec_id, uint24 *in_sec_id, uint24 *lb_off) {
#else
COMP_OPT void get_item_sec(uint24 Media_type, uint24 item_per_sec, uint24 start_sec, uint24 index, uint32 *sec_id, uint24 *in_sec_id, uint24 *lb_off) {
#endif
	LONG map_sec_id;

#ifndef _OPTIMIZE_SKIP_ERROR_CHECK_
	if (index >= MAX_NUM_OF_SONG) {
		*sec_id = EOSEC;
		*in_sec_id = EOSEC;
		return;
	}
#endif

	cal_sec(item_per_sec, start_sec, index, sec_id, in_sec_id);
	/*
	*sec_id = (index/item_per_sec);
	*in_sec_id = index - ((*sec_id)*item_per_sec);
	*sec_id += start_sec;
	*/
	cal_offset(*sec_id, lb_off);

#ifdef _USE_FS_SECTAB_
#ifdef USE_FUNC_GET_MAP_SEC
	get_map_sec_Browse(Media_type, *sec_id, &map_sec_id);
#else
#ifdef VOICE_FUNCTION
	get_map_sec_Voice(Media_type, *sec_id, &map_sec_id);
#else
	get_map_sec(Media_type, *sec_id, &map_sec_id);
#endif
#endif
	*sec_id = (uint32) map_sec_id;
#endif
}

/*
COMP_OPT void get_flash_num_of_sec(uint24 index, uint32 *sec_id, uint24 *in_sec_addr) {
*sec_id = FLASH_NUM_OF_START_SEC;
if (index == NUM_OF_SONG_DTYPE) {
*in_sec_addr = ADDR_FLASH_NUM_OF_SONG;
} else if (index == NUM_OF_ARTIST_DTYPE) {
*in_sec_addr = ADDR_FLASH_NUM_OF_ARTIST;
} else if (index == NUM_OF_ALBUM_DTYPE) {
*in_sec_addr = ADDR_FLASH_NUM_OF_ALBUM;
} else if (index == NUM_OF_GENRE_DTYPE) {
*in_sec_addr = ADDR_FLASH_NUM_OF_GENRE;
} else if (index == NUM_OF_YEAR_DTYPE) {
*in_sec_addr = ADDR_FLASH_NUM_OF_YEAR;
} else {
*sec_id = EOSEC;
*in_sec_addr = EOSEC;
}
}
*/
#else		/* replace with 3 generic sec extract function */
COMP_OPT void get_artist_name_sec(uint24 index, uint32 *sec_id, uint24 *in_sec_id) {
#ifndef _OPTIMIZE_SKIP_ERROR_CHECK_
	if (index >= MAX_NUM_OF_ARTIST) {
		*sec_id = EOSEC;
		*in_sec_id = EOSEC;
		return;
	}
#endif
	*sec_id = (index/ARTIST_NAME_ITEM_PER_SEC);
	*in_sec_id = index - ((*sec_id)*ARTIST_NAME_ITEM_PER_SEC);
	*sec_id += ARTIST_NAME_START_SEC;
}

COMP_OPT void get_album_name_sec(uint24 index, uint32 *sec_id, uint24 *in_sec_id) {
#ifndef _OPTIMIZE_SKIP_ERROR_CHECK_
	if (index >= MAX_NUM_OF_ALBUM) {
		*sec_id = EOSEC;
		*in_sec_id = EOSEC;
		return;
	}
#endif
	*sec_id = (index/ALBUM_NAME_ITEM_PER_SEC);
	*in_sec_id = index - ((*sec_id)*ALBUM_NAME_ITEM_PER_SEC);
	*sec_id += ALBUM_NAME_START_SEC;
}

COMP_OPT void get_genre_name_sec(uint24 index, uint32 *sec_id, uint24 *in_sec_id) {
#ifndef _OPTIMIZE_SKIP_ERROR_CHECK_
	if (index >= MAX_NUM_OF_GENRE) {
		*sec_id = EOSEC;
		*in_sec_id = EOSEC;
		return;
	}
#endif
	*sec_id = (index/GENRE_NAME_ITEM_PER_SEC);
	*in_sec_id = index - ((*sec_id)*GENRE_NAME_ITEM_PER_SEC);
	*sec_id += GENRE_NAME_START_SEC;
}

COMP_OPT void get_song_name_sec(uint24 index, uint32 *sec_id, uint24 *in_sec_id) {
#ifndef _OPTIMIZE_SKIP_ERROR_CHECK_
	if (index >= MAX_NUM_OF_SONG) {
		*sec_id = EOSEC;
		*in_sec_id = EOSEC;
		return;
	}
#endif
	*sec_id = (index/SONG_NAME_ITEM_PER_SEC);
	*in_sec_id = index - ((*sec_id)*SONG_NAME_ITEM_PER_SEC);
	*sec_id += SONG_NAME_START_SEC;
}

COMP_OPT void get_path_name_sec(uint24 index, uint32 *sec_id, uint24 *in_sec_id) {
#ifndef _OPTIMIZE_SKIP_ERROR_CHECK_
	if (index >= MAX_NUM_OF_SONG) {
		*sec_id = EOSEC;
		*in_sec_id = EOSEC;
		return;
	}
#endif
	*sec_id = (index/PATH_NAME_ITEM_PER_SEC);
	*in_sec_id = index - ((*sec_id)*PATH_NAME_ITEM_PER_SEC);
	*sec_id += PATH_NAME_START_SEC;
}

COMP_OPT void get_song_array_sec(uint24 index, uint32 *sec_id, uint24 *in_sec_id) {
#ifndef _OPTIMIZE_SKIP_ERROR_CHECK_
	if (index >= MAX_NUM_OF_SONG) {
		*sec_id = EOSEC;
		*in_sec_id = EOSEC;
		return;
	}
#endif
	*sec_id = (index/SONG_ARRAY_ITEM_PER_SEC);
	*in_sec_id = index - ((*sec_id)*SONG_ARRAY_ITEM_PER_SEC);
	*sec_id += SONG_ARRAY_START_SEC;
}

COMP_OPT void get_song_year_sec(uint24 index, uint32 *sec_id, uint24 *in_sec_id) {
#ifndef _OPTIMIZE_SKIP_ERROR_CHECK_
	if (index >= MAX_NUM_OF_SONG) {
		*sec_id = EOSEC;
		*in_sec_id = EOSEC;
		return;
	}
#endif
	*sec_id = (index/SONG_YEAR_ITEM_PER_SEC);
	*in_sec_id = index - ((*sec_id)*SONG_YEAR_ITEM_PER_SEC);
	*sec_id += SONG_YEAR_START_SEC;
}

COMP_OPT void get_artist_sort_sec(uint24 index, uint32 *sec_id, uint24 *in_sec_id) {
#ifndef _OPTIMIZE_SKIP_ERROR_CHECK_
	if (index >= MAX_NUM_OF_ARTIST) {
		*sec_id = EOSEC;
		*in_sec_id = EOSEC;
		return;
	}
#endif
	*sec_id = (index/SORT_ITEM_PER_SEC);
	*in_sec_id = index - ((*sec_id)*SORT_ITEM_PER_SEC);
	*sec_id += ARTIST_SORT_START_SEC;
}

COMP_OPT void get_artist_assoc_count_sec(uint24 index, uint32 *sec_id, uint24 *in_sec_id) {
#ifndef _OPTIMIZE_SKIP_ERROR_CHECK_
	if (index >= MAX_NUM_OF_ARTIST) {
		*sec_id = EOSEC;
		*in_sec_id = EOSEC;
		return;
	}
#endif
	*sec_id = (index/ASSOC_ITEM_PER_SEC);
	*in_sec_id = index - ((*sec_id)*ASSOC_ITEM_PER_SEC);
	*sec_id += ARTIST_ASSOC_START_SEC;
}

COMP_OPT void get_artist_subassoc_count_sec(uint24 index, uint32 *sec_id, uint24 *in_sec_id) {
#ifndef _OPTIMIZE_SKIP_ERROR_CHECK_
	if (index >= MAX_NUM_OF_ARTIST) {
		*sec_id = EOSEC;
		*in_sec_id = EOSEC;
		return;
	}
#endif
	*sec_id = (index/SUBASSOC_ITEM_PER_SEC);
	*in_sec_id = index - ((*sec_id)*SUBASSOC_ITEM_PER_SEC);
	*sec_id += ARTIST_SUBASSOC_START_SEC;
}

COMP_OPT void get_album_sort_sec(uint24 index, uint32 *sec_id, uint24 *in_sec_id) {
#ifndef _OPTIMIZE_SKIP_ERROR_CHECK_
	if (index >= MAX_NUM_OF_ALBUM) {
		*sec_id = EOSEC;
		*in_sec_id = EOSEC;
		return;
	}
#endif
	*sec_id = (index/SORT_ITEM_PER_SEC);
	*in_sec_id = index - ((*sec_id)*SORT_ITEM_PER_SEC);
	*sec_id += ALBUM_SORT_START_SEC;
}

COMP_OPT void get_album_assoc_count_sec(uint24 index, uint32 *sec_id, uint24 *in_sec_id) {
#ifndef _OPTIMIZE_SKIP_ERROR_CHECK_
	if (index >= MAX_NUM_OF_ALBUM) {
		*sec_id = EOSEC;
		*in_sec_id = EOSEC;
		return;
	}
#endif
	*sec_id = (index/ASSOC_ITEM_PER_SEC);
	*in_sec_id = index - ((*sec_id)*ASSOC_ITEM_PER_SEC);
	*sec_id += ALBUM_ASSOC_START_SEC;
}

COMP_OPT void get_genre_sort_sec(uint24 index, uint32 *sec_id, uint24 *in_sec_id) {
#ifndef _OPTIMIZE_SKIP_ERROR_CHECK_
	if (index >= MAX_NUM_OF_GENRE) {
		*sec_id = EOSEC;
		*in_sec_id = EOSEC;
		return;
	}
#endif
	*sec_id = (index/SORT_ITEM_PER_SEC);
	*in_sec_id = index - ((*sec_id)*SORT_ITEM_PER_SEC);
	*sec_id += GENRE_SORT_START_SEC;
}

COMP_OPT void get_genre_assoc_count_sec(uint24 index, uint32 *sec_id, uint24 *in_sec_id) {
#ifndef _OPTIMIZE_SKIP_ERROR_CHECK_
	if (index >= MAX_NUM_OF_GENRE) {
		*sec_id = EOSEC;
		*in_sec_id = EOSEC;
		return;
	}
#endif
	*sec_id = (index/ASSOC_ITEM_PER_SEC);
	*in_sec_id = index - ((*sec_id)*ASSOC_ITEM_PER_SEC);
	*sec_id += GENRE_ASSOC_START_SEC;
}
COMP_OPT void get_year_sort_sec(uint24 index, uint32 *sec_id, uint24 *in_sec_id) {
#ifndef _OPTIMIZE_SKIP_ERROR_CHECK_
	if (index >= MAX_NUM_OF_YEAR) {
		*sec_id = EOSEC;
		*in_sec_id = EOSEC;
		return;
	}
#endif
	*sec_id = (index/SORT_ITEM_PER_SEC);
	*in_sec_id = index - ((*sec_id)*SORT_ITEM_PER_SEC);
	*sec_id += YEAR_SORT_START_SEC;
}

COMP_OPT void get_year_assoc_count_sec(uint24 index, uint32 *sec_id, uint24 *in_sec_id) {
#ifndef _OPTIMIZE_SKIP_ERROR_CHECK_
	if (index >= MAX_NUM_OF_YEAR) {
		*sec_id = EOSEC;
		*in_sec_id = EOSEC;
		return;
	}
#endif
	*sec_id = (index/ASSOC_ITEM_PER_SEC);
	*in_sec_id = index - ((*sec_id)*ASSOC_ITEM_PER_SEC);
	*sec_id += YEAR_ASSOC_START_SEC;
}

COMP_OPT void get_artist_assoc_cid_sec(uint24 index, uint32 *sec_id, uint24 *in_sec_id) {
#ifndef _OPTIMIZE_SKIP_ERROR_CHECK_
	if (index >= MAX_NUM_OF_ARTIST) {
		*sec_id = EOSEC;
		*in_sec_id = EOSEC;
		return;
	}
#endif
	*sec_id = ((index+MAX_NUM_OF_SONG)/ASSOC_ITEM_PER_SEC);
	*in_sec_id = (index+MAX_NUM_OF_SONG) - ((*sec_id)*ASSOC_ITEM_PER_SEC);
	*sec_id += ARTIST_ASSOC_START_SEC;
}

COMP_OPT void get_artist_subassoc_cid_sec(uint24 index, uint32 *sec_id, uint24 *in_sec_id) {
#ifndef _OPTIMIZE_SKIP_ERROR_CHECK_
	if (index >= MAX_NUM_OF_ARTIST) {
		*sec_id = EOSEC;
		*in_sec_id = EOSEC;
		return;
	}
#endif
	*sec_id = ((index+MAX_NUM_OF_SONG)/ASSOC_ITEM_PER_SEC);
	*in_sec_id = (index+MAX_NUM_OF_SONG) - ((*sec_id)*ASSOC_ITEM_PER_SEC);
	*sec_id += ARTIST_SUBASSOC_START_SEC;
}

/* index=pos from *_ASSOC_COUNT_DTYPR read */
COMP_OPT void get_album_assoc_cid_sec(uint24 index, uint32 *sec_id, uint24 *in_sec_id) {
#ifndef _OPTIMIZE_SKIP_ERROR_CHECK_
	if (index >= MAX_NUM_OF_ALBUM) {
		*sec_id = EOSEC;
		*in_sec_id = EOSEC;
		return;
	}
#endif
	*sec_id = ((index+MAX_NUM_OF_SONG)/ASSOC_ITEM_PER_SEC);
	*in_sec_id = (index+MAX_NUM_OF_SONG) - ((*sec_id)*ASSOC_ITEM_PER_SEC);
	*sec_id += ALBUM_ASSOC_START_SEC;
}

COMP_OPT void get_genre_assoc_cid_sec(uint24 index, uint32 *sec_id, uint24 *in_sec_id) {
#ifndef _OPTIMIZE_SKIP_ERROR_CHECK_
	if (index >= MAX_NUM_OF_GENRE) {
		*sec_id = EOSEC;
		*in_sec_id = EOSEC;
		return;
	}
#endif
	*sec_id = ((index+MAX_NUM_OF_SONG)/ASSOC_ITEM_PER_SEC);
	*in_sec_id = (index+MAX_NUM_OF_SONG) - ((*sec_id)*ASSOC_ITEM_PER_SEC);
	*sec_id += GENRE_ASSOC_START_SEC;
}


COMP_OPT void get_year_assoc_cid_sec(uint24 index, uint32 *sec_id, uint24 *in_sec_id) {
#ifndef _OPTIMIZE_SKIP_ERROR_CHECK_
	if (index >= MAX_NUM_OF_YEAR) {
		*sec_id = EOSEC;
		*in_sec_id = EOSEC;
		return;
	}
#endif
	*sec_id = ((index+MAX_NUM_OF_SONG)/ASSOC_ITEM_PER_SEC);
	*in_sec_id = (index+MAX_NUM_OF_SONG) - ((*sec_id)*ASSOC_ITEM_PER_SEC);
	*sec_id += YEAR_ASSOC_START_SEC;
}
#endif		/* replace with 3 generic sec extract function */

/*
COMP_OPT void get_new_music_sec(uint24 index, uint32 *sec_id, uint24 *in_sec_id) {
#ifdef _ASSERT_
assert(0);
#endif
}

COMP_OPT void get_on_the_fly_sec(uint24 index, uint32 *sec_id, uint24 *in_sec_id) {
#ifdef _ASSERT_
assert(0);
#endif
}

COMP_OPT void get_m3u_music_sec(uint24 index, uint32 *sec_id, uint24 *in_sec_id) {
#ifdef _ASSERT_
assert(0);
#endif
}
*/

/*================================================================================================*/

#ifdef _PC_SIMULATION_
COMP_OPT void ML_MediaDetail(void) {

#if 1
	printf("\nMAX_NUM_OF_SONG=%d", MAX_NUM_OF_SONG);
	printf("\nFlash memory simulator sector alocation:");
	printf("\nNAME_STRUCT_SIZE=%d", NAME_STRUCT_SIZE);
	/*
	printf("\nARTIST_NAME_STRUCT_SIZE=%d", ARTIST_NAME_STRUCT_SIZE);
	printf("\nALBUM_NAME_STRUCT_SIZE=%d", ALBUM_NAME_STRUCT_SIZE);
	printf("\nGENRE_NAME_STRUCT_SIZE=%d", GENRE_NAME_STRUCT_SIZE);
	printf("\nSONG_NAME_STRUCT_SIZE=%d", SONG_NAME_STRUCT_SIZE);
	*/
	printf("\nPATH_NAME_STRUCT_SIZE=%d", PATH_NAME_STRUCT_SIZE);
	printf("\nNEW_MUSIC_STRUCT_SIZE=%d", NEW_MUSIC_STRUCT_SIZE);

	printf("\nNAME_SEC_SIZE=%d", NAME_SEC_SIZE);
	/*
	printf("\nARTIST_NAME_SEC_SIZE=%d", ARTIST_NAME_SEC_SIZE);
	printf("\nALBUM_NAME_SEC_SIZE=%d", ALBUM_NAME_SEC_SIZE);
	printf("\nGENRE_NAME_SEC_SIZE=%d", GENRE_NAME_SEC_SIZE);
	printf("\nSONG_NAME_SEC_SIZE=%d", SONG_NAME_SEC_SIZE);
	*/
	printf("\nPATH_NAME_SEC_SIZE=%d", PATH_NAME_SEC_SIZE);
	printf("\nSONG_ARRAY_SEC_SIZE=%d", SONG_ARRAY_SEC_SIZE);
	printf("\nSONG_YEAR_SEC_SIZE=%d", SONG_YEAR_SEC_SIZE);
	printf("\nARTIST_SORT_SEC_SIZE=%d", ARTIST_SORT_SEC_SIZE);
	printf("\nARTIST_ASSOC_SEC_SIZE=%d", ARTIST_ASSOC_SEC_SIZE);
	printf("\nARTIST_SUBASSOC_SEC_SIZE=%d", ARTIST_SUBASSOC_SEC_SIZE);

	printf("\nALBUM_SORT_SEC_SIZE=%d", ALBUM_SORT_SEC_SIZE);
	printf("\nALBUM_ASSOC_SEC_SIZE=%d", ALBUM_ASSOC_SEC_SIZE);

	printf("\nGENRE_SORT_SEC_SIZE=%d", GENRE_SORT_SEC_SIZE);
	printf("\nGENRE_ASSOC_SEC_SIZE=%d", GENRE_ASSOC_SEC_SIZE);

	printf("\nYEAR_SORT_SEC_SIZE=%d", YEAR_SORT_SEC_SIZE);
	printf("\nYEAR_ASSOC_SEC_SIZE=%d", YEAR_ASSOC_SEC_SIZE);

	printf("\nNEW_MUSIC_SEC_SIZE=%d",NEW_MUSIC_SEC_SIZE);
	printf("\nON_THE_FLY_SEC_SIZE=%d",ON_THE_FLY_SEC_SIZE);
	printf("\nM3U_MUSIC_SEC_SIZE=%d",M3U_MUSIC_SEC_SIZE);
	printf("\nAUDIBLE_SEC_SIZE=%d",AUDIBLE_SEC_SIZE);
	printf("\nAUDIBLE_NAME_SEC_SIZE=%d",AUDIBLE_NAME_SEC_SIZE);
	printf("\nAUDIBLE_PATH_SEC_SIZE=%d",AUDIBLE_PATH_SEC_SIZE);

	printf("\nARTIST_NAME_START_SEC=%d",ARTIST_NAME_START_SEC);
	printf("\nALBUM_NAME_START_SEC=%d",ALBUM_NAME_START_SEC);
	printf("\nGENRE_NAME_START_SEC=%d",GENRE_NAME_START_SEC);
	printf("\nSONG_NAME_START_SEC=%d", SONG_NAME_START_SEC);

	printf("\nPATH_NAME_START_SEC=%d", PATH_NAME_START_SEC);
	printf("\nSONG_ARRAY_START_SEC=%d", SONG_ARRAY_START_SEC);
	printf("\nSONG_YEAR_START_SEC=%d", SONG_YEAR_START_SEC);
	printf("\nARTIST_SORT_START_SEC=%d",ARTIST_SORT_START_SEC);
	printf("\nARTIST_ASSOC_START_SEC=%d",ARTIST_ASSOC_START_SEC);
	printf("\nARTIST_SUBASSOC_START_SEC =%d",ARTIST_SUBASSOC_START_SEC);

	printf("\nALBUM_SORT_START_SEC=%d",ALBUM_SORT_START_SEC);
	printf("\nALBUM_ASSOC_START_SEC=%d",ALBUM_ASSOC_START_SEC);

	printf("\nGENRE_SORT_START_SEC=%d",GENRE_SORT_START_SEC);
	printf("\nGENRE_ASSOC_START_SEC=%d",GENRE_ASSOC_START_SEC);

	printf("\nYEAR_SORT_START_SEC=%d",YEAR_SORT_START_SEC);
	printf("\nYEAR_ASSOC_START_SEC=%d",YEAR_ASSOC_START_SEC);

	printf("\nNEW_MUSIC_START_SEC=%d",NEW_MUSIC_START_SEC);
	printf("\nON_THE_FLY_START_SEC=%d",ON_THE_FLY_START_SEC);
	printf("\nM3U_MUSIC_START_SEC=%d",M3U_MUSIC_START_SEC);
	printf("\nAUDIBLE_START_SEC=%d",AUDIBLE_START_SEC);
	printf("\nAUDIBLE_NAME_START_SEC=%d",AUDIBLE_NAME_START_SEC);
	printf("\nAUDIBLE_PATH_START_SEC=%d",AUDIBLE_PATH_START_SEC);

	printf("\nVOICE_NAME_START_SEC=%d",VOICE_NAME_START_SEC);
	printf("\nVOICE_PATH_START_SEC=%d",VOICE_PATH_START_SEC);
	printf("\nVOICE_MARKTABLE_START_SEC=%d",VOICE_MARKTABLE_START_SEC);
	printf("\nVOICE_INFO_START_SEC=%d",VOICE_INFO_START_SEC);
	printf("\nVOICE_ARRAY_START_SEC=%d",VOICE_ARRAY_START_SEC);
	printf("\nVOICE_NUM_OF_START_SEC=%d",VOICE_NUM_OF_START_SEC);

	printf("\nFMREC_NAME_START_SEC=%d",FMREC_NAME_START_SEC);
	printf("\nFMREC_PATH_START_SEC=%d",FMREC_PATH_START_SEC);
	printf("\nFMREC_MARKTABLE_START_SEC=%d",FMREC_MARKTABLE_START_SEC);
	printf("\nFMREC_INFO_START_SEC=%d",FMREC_INFO_START_SEC);
	printf("\nFMREC_ARRAY_START_SEC=%d",FMREC_ARRAY_START_SEC);
	printf("\nFMREC_NUM_OF_START_SEC=%d",FMREC_NUM_OF_START_SEC);

	printf("\nVOICE_NAME_ITEM_PER_SEC=%d",	NAME_ITEM_PER_SEC);
	printf("\nVOICE_PATH_ITEM_PER_SEC=%d",	PATH_NAME_ITEM_PER_SEC);

	printf("\nFMREC_NAME_ITEM_PER_SEC=%d",	NAME_ITEM_PER_SEC);
	printf("\nFMREC_PATH_ITEM_PER_SEC=%d",	PATH_NAME_ITEM_PER_SEC);

	printf("\nADDR_FLASH_NUM_OF_VOICE=%d",ADDR_FLASH_NUM_OF_VOICE);
	printf("\nADDR_FLASH_NUM_OF_FMREC=%d",ADDR_FLASH_NUM_OF_FMREC);

	printf("\nNAME_ITEM_PER_SEC=%d",NAME_ITEM_PER_SEC);
	/*
	printf("\nARTIST_NAME_ITEM_PER_SEC=%d",ARTIST_NAME_ITEM_PER_SEC);
	printf("\nALBUM_NAME_ITEM_PER_SEC=%d",ALBUM_NAME_ITEM_PER_SEC);
	printf("\nGENRE_NAME_ITEM_PER_SEC=%d",GENRE_NAME_ITEM_PER_SEC);
	printf("\nSONG_NAME_ITEM_PER_SEC=%d", SONG_NAME_ITEM_PER_SEC);
	*/
	printf("\nPATH_NAME_ITEM_PER_SEC=%d", PATH_NAME_ITEM_PER_SEC);
	//	printf("\nSONG_ARRAY_ITEM_PER_SEC=%d", SONG_ARRAY_ITEM_PER_SEC);
	printf("\nSONG_YEAR_ITEM_PER_SEC=%d",SONG_YEAR_ITEM_PER_SEC);
	/*
	printf("\nSORT_ITEM_PER_SEC=%d",SORT_ITEM_PER_SEC);
	printf("\nASSOC_ITEM_PER_SEC=%d",ASSOC_ITEM_PER_SEC);
	printf("\nSUBASSOC_ITEM_PER_SEC=%d",SUBASSOC_ITEM_PER_SEC);
	*/
	printf("\nLIST_ITEM_PER_SEC=%d",LIST_ITEM_PER_SEC);
#endif

#if 1
	printf("\nNAME_STRUCT_SIZE=0x%X", NAME_STRUCT_SIZE);
	printf("\nNAME_STRUCT_SIZE=0x%X", NAME_STRUCT_SIZE);
	/*
	printf("\nARTIST_NAME_STRUCT_SIZE=0x%X", ARTIST_NAME_STRUCT_SIZE);
	printf("\nALBUM_NAME_STRUCT_SIZE=0x%X", ALBUM_NAME_STRUCT_SIZE);
	printf("\nGENRE_NAME_STRUCT_SIZE=0x%X", GENRE_NAME_STRUCT_SIZE);
	printf("\nSONG_NAME_STRUCT_SIZE=0x%X", SONG_NAME_STRUCT_SIZE);
	*/
	printf("\nPATH_NAME_STRUCT_SIZE=0x%X", PATH_NAME_STRUCT_SIZE);
	printf("\nNEW_MUSIC_STRUCT_SIZE=0x%X", NEW_MUSIC_STRUCT_SIZE);

	/*
	printf("\nARTIST_NAME_SEC_SIZE=0x%X", ARTIST_NAME_SEC_SIZE);
	printf("\nALBUM_NAME_SEC_SIZE=0x%X", ALBUM_NAME_SEC_SIZE);
	printf("\nGENRE_NAME_SEC_SIZE=0x%X", GENRE_NAME_SEC_SIZE);
	printf("\nSONG_NAME_SEC_SIZE=0x%X", SONG_NAME_SEC_SIZE);
	*/
	printf("\nPATH_NAME_SEC_SIZE=0x%X", PATH_NAME_SEC_SIZE);
	printf("\nSONG_ARRAY_SEC_SIZE=0x%X", SONG_ARRAY_SEC_SIZE);
	printf("\nSONG_YEAR_SEC_SIZE=0x%X", SONG_YEAR_SEC_SIZE);
	printf("\nARTIST_SORT_SEC_SIZE=0x%X", ARTIST_SORT_SEC_SIZE);
	printf("\nARTIST_ASSOC_SEC_SIZE=0x%X", ARTIST_ASSOC_SEC_SIZE);
	printf("\nARTIST_SUBASSOC_SEC_SIZE=0x%X", ARTIST_SUBASSOC_SEC_SIZE);

	printf("\nALBUM_SORT_SEC_SIZE=0x%X", ALBUM_SORT_SEC_SIZE);
	printf("\nALBUM_ASSOC_SEC_SIZE=0x%X", ALBUM_ASSOC_SEC_SIZE);

	printf("\nGENRE_SORT_SEC_SIZE=0x%X", GENRE_SORT_SEC_SIZE);
	printf("\nGENRE_ASSOC_SEC_SIZE=0x%X", GENRE_ASSOC_SEC_SIZE);

	printf("\nYEAR_SORT_SEC_SIZE=0x%X", YEAR_SORT_SEC_SIZE);
	printf("\nYEAR_ASSOC_SEC_SIZE=0x%X", YEAR_ASSOC_SEC_SIZE);

	printf("\nNEW_MUSIC_SEC_SIZE=0x%X",NEW_MUSIC_SEC_SIZE);
	printf("\nON_THE_FLY_SEC_SIZE=0x%X",ON_THE_FLY_SEC_SIZE);
	printf("\nM3U_MUSIC_SEC_SIZE=0x%X",M3U_MUSIC_SEC_SIZE);
	printf("\nSONG_INFO_SEC_SIZE=0x%X",SONG_INFO_SEC_SIZE);
	printf("\nAUDIBLE_SEC_SIZE=0x%X",AUDIBLE_SEC_SIZE);
	printf("\nAUDIBLE_NAME_SEC_SIZE=0x%X",AUDIBLE_NAME_SEC_SIZE);
	printf("\nAUDIBLE_PATH_SEC_SIZE=0x%X",AUDIBLE_PATH_SEC_SIZE);

	printf("\nARTIST_NAME_START_SEC=0x%X",ARTIST_NAME_START_SEC);
	printf("\nALBUM_NAME_START_SEC=0x%X",ALBUM_NAME_START_SEC);
	printf("\nGENRE_NAME_START_SEC=0x%X",GENRE_NAME_START_SEC);
	printf("\nSONG_NAME_START_SEC=0x%X", SONG_NAME_START_SEC);

	printf("\nPATH_NAME_START_SEC=0x%X", PATH_NAME_START_SEC);
	printf("\nSONG_ARRAY_START_SEC=0x%X", SONG_ARRAY_START_SEC);
	printf("\nSONG_YEAR_START_SEC=0x%X", SONG_YEAR_START_SEC);
	printf("\nARTIST_SORT_START_SEC=0x%X",ARTIST_SORT_START_SEC);
	printf("\nARTIST_ASSOC_START_SEC=0x%X",ARTIST_ASSOC_START_SEC);
	printf("\nARTIST_SUBASSOC_START_SEC =0x%X",ARTIST_SUBASSOC_START_SEC);

	printf("\nALBUM_SORT_START_SEC=0x%X",ALBUM_SORT_START_SEC);
	printf("\nALBUM_ASSOC_START_SEC=0x%X",ALBUM_ASSOC_START_SEC);

	printf("\nGENRE_SORT_START_SEC=0x%X",GENRE_SORT_START_SEC);
	printf("\nGENRE_ASSOC_START_SEC=0x%X",GENRE_ASSOC_START_SEC);

	printf("\nYEAR_SORT_START_SEC=0x%X",YEAR_SORT_START_SEC);
	printf("\nYEAR_ASSOC_START_SEC=0x%X",YEAR_ASSOC_START_SEC);

	printf("\nNEW_MUSIC_START_SEC=0x%X",NEW_MUSIC_START_SEC);
	printf("\nON_THE_FLY_START_SEC=0x%X",ON_THE_FLY_START_SEC);
	printf("\nM3U_MUSIC_START_SEC=0x%X",M3U_MUSIC_START_SEC);
	printf("\nAUDIBLE_START_SEC=0x%X",AUDIBLE_START_SEC);
	printf("\nAUDIBLE_NAME_START_SEC=0x%X",AUDIBLE_NAME_START_SEC);
	printf("\nAUDIBLE_PATH_START_SEC=0x%X",AUDIBLE_PATH_START_SEC);

	printf("\nSONG_INFO_ARTIST_START_SEC=0x%X",SONG_INFO_ARTIST_START_SEC);
	printf("\nSONG_INFO_ALBUM_START_SEC=0x%X",SONG_INFO_ALBUM_START_SEC);
	printf("\nSONG_INFO_GENRE_START_SEC=0x%X",SONG_INFO_GENRE_START_SEC);
	printf("\nSONG_INFO_YEAR_START_SEC=0x%X",SONG_INFO_YEAR_START_SEC);

	printf("\nVOICE_NAME_START_SEC=0x%X",VOICE_NAME_START_SEC);
	printf("\nVOICE_PATH_START_SEC=0x%X",VOICE_PATH_START_SEC);
	printf("\nVOICE_MARKTABLE_START_SEC=0x%X",VOICE_MARKTABLE_START_SEC);
	printf("\nVOICE_INFO_START_SEC=0x%X",VOICE_INFO_START_SEC);
	printf("\nVOICE_ARRAY_START_SEC=0x%X",VOICE_ARRAY_START_SEC);
	printf("\nVOICE_NUM_OF_START_SEC=0x%X",VOICE_NUM_OF_START_SEC);

	printf("\nFMREC_NAME_START_SEC=0x%X",FMREC_NAME_START_SEC);
	printf("\nFMREC_PATH_START_SEC=0x%X",FMREC_PATH_START_SEC);
	printf("\nFMREC_MARKTABLE_START_SEC=0x%X",FMREC_MARKTABLE_START_SEC);
	printf("\nFMREC_INFO_START_SEC=0x%X",FMREC_INFO_START_SEC);
	printf("\nFMREC_ARRAY_START_SEC=0x%X",FMREC_ARRAY_START_SEC);
	printf("\nFMREC_NUM_OF_START_SEC=0x%X",FMREC_NUM_OF_START_SEC);

	printf("\nARTIST_MERGE_START_SEC=0x%X",ARTIST_MERGE_START_SEC);
	printf("\nALBUM_MERGE_START_SEC=0x%X",ALBUM_MERGE_START_SEC);
	printf("\nGENRE_MERGE_START_SEC=0x%X",GENRE_MERGE_START_SEC);
	printf("\nSONG_MERGE_START_SEC=0x%X",SONG_MERGE_START_SEC);
	printf("\nYEAR_MERGE_START_SEC=0x%X",YEAR_MERGE_START_SEC);

	printf("\nMUSICLIB.LIB Offset:");
	printf("\nARTIST_NAME_START_SEC=0x%X",ARTIST_NAME_START_SEC*512);
	printf("\nALBUM_NAME_START_SEC=0x%X",ALBUM_NAME_START_SEC*512);
	printf("\nGENRE_NAME_START_SEC=0x%X",GENRE_NAME_START_SEC*512);
	printf("\nSONG_NAME_START_SEC=0x%X", SONG_NAME_START_SEC*512);

	printf("\nPATH_NAME_START_SEC=0x%X", PATH_NAME_START_SEC*512);
	printf("\nSONG_ARRAY_START_SEC=0x%X", SONG_ARRAY_START_SEC*512);
	printf("\nSONG_YEAR_START_SEC=0x%X", SONG_YEAR_START_SEC*512);
	printf("\nARTIST_SORT_START_SEC=0x%X",ARTIST_SORT_START_SEC*512);
	printf("\nARTIST_ASSOC_START_SEC=0x%X",ARTIST_ASSOC_START_SEC*512);
	printf("\nARTIST_SUBASSOC_START_SEC =0x%X",ARTIST_SUBASSOC_START_SEC*512);

	printf("\nALBUM_SORT_START_SEC=0x%X",ALBUM_SORT_START_SEC*512);
	printf("\nALBUM_ASSOC_START_SEC=0x%X",ALBUM_ASSOC_START_SEC*512);

	printf("\nGENRE_SORT_START_SEC=0x%X",GENRE_SORT_START_SEC*512);
	printf("\nGENRE_ASSOC_START_SEC=0x%X",GENRE_ASSOC_START_SEC*512);

	printf("\nYEAR_SORT_START_SEC=0x%X",YEAR_SORT_START_SEC*512);
	printf("\nYEAR_ASSOC_START_SEC=0x%X",YEAR_ASSOC_START_SEC*512);

	printf("\nNEW_MUSIC_START_SEC=0x%X",NEW_MUSIC_START_SEC*512);
	printf("\nON_THE_FLY_START_SEC=0x%X",ON_THE_FLY_START_SEC*512);
	printf("\nM3U_MUSIC_START_SEC=0x%X",M3U_MUSIC_START_SEC*512);
	printf("\nAUDIBLE_START_SEC=0x%X",AUDIBLE_START_SEC*512);
	printf("\nAUDIBLE_NAME_START_SEC=0x%X",AUDIBLE_NAME_START_SEC*512);
	printf("\nAUDIBLE_PATH_START_SEC=0x%X",AUDIBLE_PATH_START_SEC*512);

	printf("\nSONG_INFO_ARTIST_START_SEC=0x%X",SONG_INFO_ARTIST_START_SEC*512);
	printf("\nSONG_INFO_ALBUM_START_SEC=0x%X",SONG_INFO_ALBUM_START_SEC*512);
	printf("\nSONG_INFO_GENRE_START_SEC=0x%X",SONG_INFO_GENRE_START_SEC*512);
	printf("\nSONG_INFO_YEAR_START_SEC=0x%X",SONG_INFO_YEAR_START_SEC*512);

	printf("\nVOICE_NAME_START_SEC=0x%X",VOICE_NAME_START_SEC*512);
	printf("\nVOICE_PATH_START_SEC=0x%X",VOICE_PATH_START_SEC*512);
	printf("\nVOICE_MARKTABLE_START_SEC=0x%X",VOICE_MARKTABLE_START_SEC*512);
	printf("\nVOICE_INFO_START_SEC=0x%X",VOICE_INFO_START_SEC*512);
	printf("\nVOICE_ARRAY_START_SEC=0x%X",VOICE_ARRAY_START_SEC*512);
	printf("\nVOICE_NUM_OF_START_SEC=0x%X",VOICE_NUM_OF_START_SEC*512);

	printf("\nFMREC_NAME_START_SEC=0x%X",FMREC_NAME_START_SEC*512);
	printf("\nFMREC_PATH_START_SEC=0x%X",FMREC_PATH_START_SEC*512);
	printf("\nFMREC_MARKTABLE_START_SEC=0x%X",FMREC_MARKTABLE_START_SEC*512);
	printf("\nFMREC_INFO_START_SEC=0x%X",FMREC_INFO_START_SEC*512);
	printf("\nFMREC_ARRAY_START_SEC=0x%X",FMREC_ARRAY_START_SEC*512);
	printf("\nFMREC_NUM_OF_START_SEC=0x%X",FMREC_NUM_OF_START_SEC*512);


	/*
	printf("\nARTIST_NAME_ITEM_PER_SEC=0x%X",ARTIST_NAME_ITEM_PER_SEC);
	printf("\nALBUM_NAME_ITEM_PER_SEC=0x%X",ALBUM_NAME_ITEM_PER_SEC);
	printf("\nGENRE_NAME_ITEM_PER_SEC=0x%X",GENRE_NAME_ITEM_PER_SEC);
	printf("\nSONG_NAME_ITEM_PER_SEC=0x%X", SONG_NAME_ITEM_PER_SEC);
	*/
	printf("\nPATH_NAME_ITEM_PER_SEC=0x%X", PATH_NAME_ITEM_PER_SEC);
	//	printf("\nSONG_ARRAY_ITEM_PER_SEC=0x%X", SONG_ARRAY_ITEM_PER_SEC);
	printf("\nSONG_YEAR_ITEM_PER_SEC=0x%X",SONG_YEAR_ITEM_PER_SEC);
	/*
	printf("\nSORT_ITEM_PER_SEC=0x%X",SORT_ITEM_PER_SEC);
	printf("\nASSOC_ITEM_PER_SEC=0x%X",ASSOC_ITEM_PER_SEC);
	printf("\nSUBASSOC_ITEM_PER_SEC=0x%X",SUBASSOC_ITEM_PER_SEC);
	*/
	printf("\nLIST_ITEM_PER_SEC=0x%X",LIST_ITEM_PER_SEC);
#endif
}
/*================================================================================================*/
#endif	/* _PC_SIMULATION_ */

#ifdef _USEFLASH_WRITE_
/* use at build function only */
/*==================================================================================================
FUNCTION: ML_MemWrite()

DESCRIPTION:
Description of this specific function.

ARGUMENTS PASSED:
uint24 media_type, uint24 item_type, uint24 item_id, uint24 *dbuf_p

RETURN VALUE:
dbuf_p is pointing to 1 set of struct data of item_type
(eg. to FLASH_GROUP_NAME_T, not only the name string, but name+unicode).


PRE-CONDITIONS:
None

POST-CONDITIONS:
None

IMPORTANT NOTES:
None

==================================================================================================*/
static uint24 ML_MemWrite(uint24 media_type, uint24 item_type, uint24 item_id, uint24 _X *dbuf_p, uint24 cpylen) {
	uint24 i;
	uint24 j;
	uint24 nsec = 0;
	uint24 start_sec = 0;
	uint24 struct_size = 0;
	uint24 lb_offset;

#ifndef _USE_FS_MUSICLIB_
	uint24 addr_offset;
#endif
	uint24 in_sec_id;
	uint24 ret_code;

	//uint32 dwSector;
	LONG dwSector;
	uint24 _X *writebuf_p;
	int interrupt_bk;

	interrupt_bk = setup_mask_lvl(1);

	if (media_type >= NUM_OF_MEDIA) return (!SUCCESS);
#ifndef _USE_FS_MUSICLIB_
	ret_code = (!SUCCESS);
	switch(media_type) {
		case	MEDIA_TYPE_FLASH:
			addr_offset = FLASH_DATABASE_ADR;
			break;
		case	MEDIA_TYPE_SD:
			addr_offset = SD_DATABASE_ADR;
			break;
		default :
			return(!SUCCESS);
	}
#endif

	switch(item_type) {
#ifndef VOICE_FUNCTION
#ifndef MERGE_FUNCTION
		case ARTIST_NAME_DTYPE:
		case ALBUM_NAME_DTYPE:
		case GENRE_NAME_DTYPE:
		case SONG_NAME_DTYPE:
		case PATH_NAME_DTYPE:
		case LONG_FILENAME_DTYPE:
		case AUDIBLE_NAME_DTYPE:
		case AUDIBLE_PATH_DTYPE:
		case AUDIBLE_LONG_FILENAME_DTYPE:
		case ON_THE_FLY_PATH_DTYPE:
			nsec = NAME_ITEM_PER_SEC;
			struct_size = NAME_STRUCT_SIZE;
			if (item_type == ARTIST_NAME_DTYPE) {
				if (item_id < MAX_NUM_OF_ARTIST) {
					start_sec = ARTIST_NAME_START_SEC;
				} else {
					break;
				}
			} else if (item_type == ALBUM_NAME_DTYPE) {
				if (item_id < MAX_NUM_OF_ALBUM) {
					start_sec = ALBUM_NAME_START_SEC;
				} else {
					break;
				}
			} else if (item_type == GENRE_NAME_DTYPE) {
				if (item_id < MAX_NUM_OF_GENRE) {
					start_sec = GENRE_NAME_START_SEC;
				} else {
					break;
				}
			} else if (item_type == SONG_NAME_DTYPE) {
				if (item_id < MAX_NUM_OF_SONG) {
					start_sec = SONG_NAME_START_SEC;
				} else {
					break;
				}
			} else if (item_type == PATH_NAME_DTYPE) {
				if (item_id < MAX_NUM_OF_SONG) {
					nsec = PATH_NAME_ITEM_PER_SEC;
					start_sec = PATH_NAME_START_SEC;
					struct_size = PATH_NAME_STRUCT_SIZE;
				} else {
					break;
				}
			} else if (item_type == LONG_FILENAME_DTYPE) {
				if (item_id < MAX_NUM_OF_SONG) {
					nsec = LONG_FILENAME_ITEM_PER_SEC;
					start_sec = LONG_FILENAME_START_SEC;
					struct_size = LONG_FILENAME_STRUCT_SIZE;
				} else {
					break;
				}
			} else if (item_type == AUDIBLE_NAME_DTYPE) {
				if (item_id < NUM_OF_AUDIBLE_FILE) {
					start_sec = AUDIBLE_NAME_START_SEC;
				} else {
					break;
				}
			} else if (item_type == AUDIBLE_PATH_DTYPE) {
				if (item_id < NUM_OF_AUDIBLE_FILE) {
					nsec = PATH_NAME_ITEM_PER_SEC;
					start_sec = AUDIBLE_PATH_START_SEC;
					struct_size = PATH_NAME_STRUCT_SIZE;
				} else {
					break;
				}
			} else if (item_type == AUDIBLE_LONG_FILENAME_DTYPE) {
				if (item_id < NUM_OF_AUDIBLE_FILE) {
					nsec = LONG_FILENAME_ITEM_PER_SEC;
					start_sec = AUDIBLE_LONG_FILENAME_START_SEC;
					struct_size = LONG_FILENAME_STRUCT_SIZE;
				} else {
					break;
				}
			} else if (item_type == ON_THE_FLY_PATH_DTYPE) {
				if (item_id < NUM_OF_SONG_IN_ON_THE_FLY) {
					nsec = PATH_NAME_ITEM_PER_SEC;
					start_sec = ON_THE_FLY_PATH_START_SEC;
					struct_size = PATH_NAME_STRUCT_SIZE;
				} else {
					break;
				}
			}
			get_item_sec(media_type, nsec, start_sec, item_id, (uint32*) &dwSector, &in_sec_id, &lb_offset);
			writebuf_p = sec_temp_buf_X + (struct_size*in_sec_id) + lb_offset;
#ifdef	_ASSERT_
			assert((struct_size*in_sec_id)<NUM_OF_WORD_PER_SEC);
#endif
#ifdef _USE_FS_SECTAB_
			if ((ret_code = DataDriveReadSectorCache(dwSector, FLASH_DRIVEID, (INT)READ_TYPE_NORMAL, sec_temp_buf_X)) != SUCCESS) {
				/* error */
				break;
			}
#else
#ifdef _USE_FS_MUSICLIB_
			ret_code = Fseek(g_iFileHandle, dwSector*BYTE_PER_SECTOR, SEEK_SET);
			ret_code = Fread(g_iFileHandle, (INT* _X) sec_temp_buf_X , BYTE_PER_SECTOR, X_MEMORY, -1);
#else
			dwSector += addr_offset;
			if ((ret_code = DataDriveReadSectorCache(dwSector, FLASH_DRIVEID, (INT)READ_TYPE_NORMAL, sec_temp_buf_X)) != SUCCESS) {
				/* error */
				break;
			}
#endif
#endif
			for (i=0;i<cpylen;i++) {
				writebuf_p[i] = dbuf_p[i];
			}
#ifdef _USE_FS_SECTAB_
			ret_code = DataDriveWriteSectorCache(FLASH_DRIVEID, dwSector, sec_temp_buf_X, (INT)WRITE_TYPE_RANDOM);
#else
#ifdef _USE_FS_MUSICLIB_
			ret_code = Fseek(g_iFileHandle, dwSector*BYTE_PER_SECTOR, SEEK_SET);
			ret_code = Fwrite(g_iFileHandle, (INT* _X) sec_temp_buf_X , BYTE_PER_SECTOR, X_MEMORY, -1);
#else
			//SysLoadFATWrite();
			ret_code = DataDriveWriteSectorCache(FLASH_DRIVEID, dwSector, sec_temp_buf_X, (INT)WRITE_TYPE_RANDOM);
			//SysUnLoadFATWrite();
#endif
#endif
			break;
		case SONG_ARRAY_DTYPE:
		case ARTIST_SORT_DTYPE:
		case ARTIST_ASSOC_COUNT_DTYPE:
		case ARTIST_ASSOC_CID_DTYPE:
		case ARTIST_SUBASSOC_COUNT_DTYPE:
		case ARTIST_SUBASSOC_CID_DTYPE:
		case ALBUM_SORT_DTYPE:
		case ALBUM_ASSOC_COUNT_DTYPE:
		case ALBUM_ASSOC_CID_DTYPE:
		case GENRE_SORT_DTYPE:
		case GENRE_ASSOC_COUNT_DTYPE:
		case GENRE_ASSOC_CID_DTYPE:
		case YEAR_SORT_DTYPE:
		case YEAR_ASSOC_COUNT_DTYPE:
		case YEAR_ASSOC_CID_DTYPE:
		case SONG_YEAR_DTYPE:
		case SONG_INFO_ARTIST_DTYPE:
		case SONG_INFO_ALBUM_DTYPE:
		case SONG_INFO_GENRE_DTYPE:
		case SONG_INFO_YEAR_DTYPE:
		case AUDIBLE_DTYPE:
		case TRACKNUM_ARRAY_DTYPE:
			if (item_type == SONG_ARRAY_DTYPE) {
				/* item_id = # of song to store */
				nsec = SONG_ARRAY_SEC_SIZE;
				/* loop 3 times */
				start_sec = SONG_ARRAY_START_SEC;
			} else if (item_type == ARTIST_SORT_DTYPE) {
				/* item_id = # of song to store */
				nsec = ARTIST_SORT_SEC_SIZE;
				/* loop 3 times */
				start_sec = ARTIST_SORT_START_SEC;
			} else if ((item_type == ARTIST_ASSOC_COUNT_DTYPE) || (item_type == ARTIST_ASSOC_CID_DTYPE)) {
				/* item_id = # of song to store */
				nsec = ARTIST_ASSOC_SEC_SIZE;
				/* loop 6 times */
				start_sec = ARTIST_ASSOC_START_SEC;
			} else if ((item_type == ARTIST_SUBASSOC_COUNT_DTYPE) || (item_type == ARTIST_SUBASSOC_CID_DTYPE)) {
				/* item_id = # of song to store */
				nsec = ARTIST_SUBASSOC_SEC_SIZE;
				/* loop 6 times */
				start_sec = ARTIST_SUBASSOC_START_SEC;
			} else if (item_type == ALBUM_SORT_DTYPE) {
				/* item_id = # of song to store */
				nsec = ALBUM_SORT_SEC_SIZE;
				/* loop 3 times */
				start_sec = ALBUM_SORT_START_SEC;
			} else if ((item_type == ALBUM_ASSOC_COUNT_DTYPE) || (item_type == ALBUM_ASSOC_CID_DTYPE)) {
				/* item_id = # of song to store */
				nsec = ALBUM_ASSOC_SEC_SIZE;
				/* loop 6 times */
				start_sec = ALBUM_ASSOC_START_SEC;
			} else if (item_type == GENRE_SORT_DTYPE) {
				/* item_id = # of song to store */
				nsec = GENRE_SORT_SEC_SIZE;
				/* loop 3 times */
				start_sec = GENRE_SORT_START_SEC;
			} else if ((item_type == GENRE_ASSOC_COUNT_DTYPE) || (item_type == GENRE_ASSOC_CID_DTYPE)) {
				/* item_id = # of song to store */
				nsec = GENRE_ASSOC_SEC_SIZE;
				/* loop 6 times */
				start_sec = GENRE_ASSOC_START_SEC;
			} else if (item_type == YEAR_SORT_DTYPE) {
				/* item_id = # of song to store */
				nsec =YEAR_SORT_SEC_SIZE;
				/* loop 3 times */
				start_sec = YEAR_SORT_START_SEC;
			} else if ((item_type == YEAR_ASSOC_COUNT_DTYPE) || (item_type == YEAR_ASSOC_CID_DTYPE)) {
				/* item_id = # of song to store */
				nsec = YEAR_ASSOC_SEC_SIZE;
				/* loop 6 times */
				start_sec = YEAR_ASSOC_START_SEC;
			} else if (item_type == SONG_YEAR_DTYPE) {
				nsec = SONG_YEAR_SEC_SIZE;
				/* loop 6 times */
				start_sec = SONG_YEAR_START_SEC;
			} else if (item_type == SONG_INFO_ARTIST_DTYPE) {
				/* item_id = # of song to store */
				nsec = SONG_INFO_SEC_SIZE;
				/* loop 3 times */
				start_sec = SONG_INFO_ARTIST_START_SEC;
			} else if (item_type == SONG_INFO_ALBUM_DTYPE) {
				/* item_id = # of song to store */
				nsec = SONG_INFO_SEC_SIZE;
				/* loop 3 times */
				start_sec = SONG_INFO_ALBUM_START_SEC;
			} else if (item_type == SONG_INFO_GENRE_DTYPE) {
				/* item_id = # of song to store */
				nsec = SONG_INFO_SEC_SIZE;
				/* loop 3 times */
				start_sec = SONG_INFO_GENRE_START_SEC;
			} else if (item_type == SONG_INFO_YEAR_DTYPE) {
				/* item_id = # of song to store */
				nsec = SONG_INFO_SEC_SIZE;
				/* loop 3 times */
				start_sec = SONG_INFO_YEAR_START_SEC;
			} else if (item_type == AUDIBLE_DTYPE) {
				/* item_id = # of song to store */
				nsec = AUDIBLE_SEC_SIZE;
				/* loop 3 times */
				start_sec = AUDIBLE_START_SEC;
			}
			 else if (item_type == TRACKNUM_ARRAY_DTYPE) {
			    nsec = TRACKNUM_ARRAY_SEC_SIZE;
			    start_sec = TRACKNUM_ARRAY_START_SEC;
			}
#else	/* MERGE_FUNCTION */
		case ARTIST_MERGE_DTYPE:
		case ALBUM_MERGE_DTYPE:
		case GENRE_MERGE_DTYPE:
		case SONG_MERGE_DTYPE:
		case YEAR_MERGE_DTYPE:
			if (item_type == ARTIST_MERGE_DTYPE) {
				/* item_id = # of song to store */
				nsec = SONG_MERGE_SEC_SIZE;
				/* loop 6 times */
				start_sec = ARTIST_MERGE_START_SEC;
			} else if (item_type == ALBUM_MERGE_DTYPE) {
				/* item_id = # of song to store */
				nsec = SONG_MERGE_SEC_SIZE;
				/* loop 6 times */
				start_sec = ALBUM_MERGE_START_SEC;
			} else if (item_type == GENRE_MERGE_DTYPE) {
				/* item_id = # of song to store */
				nsec = SONG_MERGE_SEC_SIZE;
				/* loop 6 times */
				start_sec = GENRE_MERGE_START_SEC;
			} else if (item_type == SONG_MERGE_DTYPE) {
				/* item_id = # of song to store */
				nsec = SONG_MERGE_SEC_SIZE;
				/* loop 6 times */
				start_sec = SONG_MERGE_START_SEC;
			} else if (item_type == YEAR_MERGE_DTYPE) {
				/* item_id = # of song to store */
				nsec = SONG_MERGE_SEC_SIZE;
				/* loop 6 times */
				start_sec = YEAR_MERGE_START_SEC;
			}
#endif	/* MERGE_FUNCTION */
			for (i=0;i<nsec;i++) {
#ifdef _USE_FS_MUSICLIB_
				dwSector = (start_sec + i);
#ifdef _USE_FS_SECTAB_
				cal_offset(dwSector, &lb_offset);
#ifdef USE_FUNC_GET_MAP_SEC
				get_map_sec_Browse(media_type, dwSector, &dwSector);
#else
				get_map_sec(media_type, dwSector, &dwSector);
#endif
#endif
#else
				dwSector = addr_offset + (start_sec + i);
#endif
#ifndef _USE_16MDB_
#ifdef _USE_FS_SECTAB_
				ret_code = DataDriveReadSectorCache(dwSector, FLASH_DRIVEID, (INT)READ_TYPE_NORMAL, sec_temp_buf_X);
#endif
#endif
				writebuf_p = dbuf_p + (NUM_OF_WORD_PER_SEC*i);
				/* use writebuf_p for parameter in DataDriveWriteSectorCache will write random bytes to the sector boundary in flash */
				for (j=0;j<NUM_OF_WORD_PER_SEC;j++) {
					sec_temp_buf_X[lb_offset+j] = writebuf_p[j];
				}
#ifdef _USE_FS_SECTAB_
				ret_code = DataDriveWriteSectorCache(FLASH_DRIVEID, dwSector, sec_temp_buf_X, (INT)WRITE_TYPE_RANDOM);
#else
#ifdef _USE_FS_MUSICLIB_
				ret_code = Fseek(g_iFileHandle, dwSector*BYTE_PER_SECTOR, SEEK_SET);
				ret_code = Fwrite(g_iFileHandle, (INT* _X) sec_temp_buf_X , BYTE_PER_SECTOR, X_MEMORY, -1);
#else
				//SysLoadFATWrite();
				//ret_code = DataDriveWriteSectorCache(dwSector, FLASH_DRIVEID, (INT)WRITE_TYPE_RANDOM, writebuf_p);
				ret_code = DataDriveWriteSectorCache(FLASH_DRIVEID, dwSector, sec_temp_buf_X, (INT)WRITE_TYPE_RANDOM);
				//SysUnLoadFATWrite();
#endif
#endif
			}
			break;
#ifndef MERGE_FUNCTION
		case NEW_MUSIC_DTYPE:
			//case NEW_1MONTH_DTYPE:
			//case NEW_1WEEK_DTYPE:
			//case NEW_1DAY_DTYPE:
		case ON_THE_FLY_DTYPE:
			if (item_type == NEW_MUSIC_DTYPE) {
				/* item_id is not used */
				dwSector = NEW_MUSIC_START_SEC;
				struct_size = (NEW_MUSIC_STRUCT_SIZE*2);
			} else {
				/* item_id is not used */
				dwSector = ON_THE_FLY_START_SEC;
				struct_size = ON_THE_FLY_STRUCT_SIZE;
			}
#ifdef _USE_FS_SECTAB_
			cal_offset(dwSector, &lb_offset);
#ifdef USE_FUNC_GET_MAP_SEC
			get_map_sec_Browse(media_type, dwSector, &dwSector);
#else
			get_map_sec(media_type, dwSector, &dwSector);
#endif
#endif
#ifndef _USE_FS_MUSICLIB_
			dwSector += addr_offset;
#endif
#ifndef _USE_16MDB_
#ifdef _USE_FS_SECTAB_
			ret_code = DataDriveReadSectorCache(dwSector, FLASH_DRIVEID, (INT)READ_TYPE_NORMAL, sec_temp_buf_X);
#endif
#endif
			/* use writebuf_p for parameter in DataDriveWriteSectorCache will write random bytes to the sector boundary in flash */
			for (j=0;j<struct_size;j++) {
				sec_temp_buf_X[lb_offset+j] = dbuf_p[j];
			}
#ifdef _USE_FS_SECTAB_
			ret_code = DataDriveWriteSectorCache(FLASH_DRIVEID, dwSector, sec_temp_buf_X, (INT)WRITE_TYPE_RANDOM);
#else
#ifdef _USE_FS_MUSICLIB_
			ret_code = Fseek(g_iFileHandle, dwSector*BYTE_PER_SECTOR, SEEK_SET);
			ret_code = Fwrite(g_iFileHandle, (INT* _X) sec_temp_buf_X , BYTE_PER_SECTOR, X_MEMORY, -1);
#else
			//SysLoadFATWrite();
			ret_code = DataDriveWriteSectorCache(FLASH_DRIVEID, dwSector, sec_temp_buf_X, (INT)WRITE_TYPE_RANDOM);
			//SysUnLoadFATWrite();
#endif
#endif
			break;
		case M3U_MUSIC_DTYPE:
#ifdef _ASSERT_
			assert(0);
#endif

			break;
		case NUM_OF_SONG_DTYPE:
		case NUM_OF_ARTIST_DTYPE:
		case NUM_OF_ALBUM_DTYPE:
		case NUM_OF_GENRE_DTYPE:
		case NUM_OF_YEAR_DTYPE:
			/* item_id is not used, dbuf_p is not used */
			dwSector = FLASH_NUM_OF_START_SEC;
#ifdef _USE_FS_SECTAB_
			cal_offset(dwSector, &lb_offset);
#ifdef USE_FUNC_GET_MAP_SEC
			get_map_sec_Browse(media_type, dwSector, &dwSector);
#else
			get_map_sec(media_type, dwSector, &dwSector);
#endif
			if ((ret_code = DataDriveReadSectorCache(dwSector, FLASH_DRIVEID, (INT)READ_TYPE_NORMAL, sec_temp_buf_X)) != SUCCESS) {
				/* error */
				break;
			}
#else
#ifdef _USE_FS_MUSICLIB_
			ret_code = Fseek(g_iFileHandle, dwSector*BYTE_PER_SECTOR, SEEK_SET);
			ret_code = Fread(g_iFileHandle, (INT* _X) sec_temp_buf_X , BYTE_PER_SECTOR, X_MEMORY, -1);
#else
			dwSector += addr_offset;
			if ((ret_code = DataDriveReadSectorCache(dwSector, FLASH_DRIVEID, (INT)READ_TYPE_NORMAL, sec_temp_buf_X)) != SUCCESS) {
				/* error */
				break;
			}
#endif
#endif
			sec_temp_buf_X[lb_offset+ADDR_FLASH_NUM_OF_SONG] = num_of_item[SONG_SORTLOOP];
//			sec_temp_buf_X[ADDR_FLASH_NUM_OF_SONG] = num_of_song;
			sec_temp_buf_X[lb_offset+ADDR_FLASH_NUM_OF_ARTIST] = num_of_item[ARTIST_SORTLOOP];
			sec_temp_buf_X[lb_offset+ADDR_FLASH_NUM_OF_ALBUM] = num_of_item[ALBUM_SORTLOOP];
			sec_temp_buf_X[lb_offset+ADDR_FLASH_NUM_OF_GENRE] = num_of_item[GENRE_SORTLOOP];
			sec_temp_buf_X[lb_offset+ADDR_FLASH_NUM_OF_YEAR] = num_of_item[YEAR_SORTLOOP];
			/*
			sec_temp_buf_X[ADDR_FLASH_NUM_OF_ARTIST] = num_of_artist;
			sec_temp_buf_X[ADDR_FLASH_NUM_OF_ALBUM] = num_of_album;
			sec_temp_buf_X[ADDR_FLASH_NUM_OF_GENRE] = num_of_genre;
			sec_temp_buf_X[ADDR_FLASH_NUM_OF_YEAR] = num_of_year;
			*/
#ifdef _USE_FS_SECTAB_
			ret_code = DataDriveWriteSectorCache(FLASH_DRIVEID, dwSector, sec_temp_buf_X, (INT)WRITE_TYPE_RANDOM);
#else
#ifdef _USE_FS_MUSICLIB_
			ret_code = Fseek(g_iFileHandle, dwSector*BYTE_PER_SECTOR, SEEK_SET);
			ret_code = Fwrite(g_iFileHandle, (INT* _X) sec_temp_buf_X , BYTE_PER_SECTOR, X_MEMORY, -1);
#else
			//SysLoadFATWrite();
			ret_code = DataDriveWriteSectorCache(FLASH_DRIVEID, dwSector, , sec_temp_buf_X(INT)WRITE_TYPE_RANDOM);
			//SysUnLoadFATWrite();
#endif
#endif
			setup_mask_lvl( interrupt_bk );
			break;
#endif	/* MERGE_FUNCTION */
#else	/* VOICE_FUNCTION */
		case VOICE_NAME_DTYPE:
		case VOICE_PATH_DTYPE:
		case FMREC_NAME_DTYPE:
		case FMREC_PATH_DTYPE:
		case PHOTO_NAME_DTYPE:
		case PHOTO_PATH_DTYPE:
		case MVIDEO_NAME_DTYPE:
		case MVIDEO_PATH_DTYPE:
			if (item_type == VOICE_NAME_DTYPE) {
				if (item_id < MAX_NUM_OF_VOICE) {
					nsec = NAME_ITEM_PER_SEC;
					start_sec = VOICE_NAME_START_SEC;
					struct_size = NAME_STRUCT_SIZE;
				} else {
					break;
				}
			} else if (item_type == VOICE_PATH_DTYPE) {
				if (item_id < MAX_NUM_OF_VOICE) {
					nsec = PATH_NAME_ITEM_PER_SEC;
					start_sec = VOICE_PATH_START_SEC;
					struct_size = PATH_NAME_STRUCT_SIZE;
				} else {
					break;
				}
			} else if (item_type == FMREC_NAME_DTYPE) {
				if (item_id < MAX_NUM_OF_FMREC) {
					nsec = NAME_ITEM_PER_SEC;
					start_sec = FMREC_NAME_START_SEC;
					struct_size = NAME_STRUCT_SIZE;
				} else {
					break;
				}
			} else if (item_type == FMREC_PATH_DTYPE) {
				if (item_id < MAX_NUM_OF_FMREC) {
					nsec = PATH_NAME_ITEM_PER_SEC;
					start_sec = FMREC_PATH_START_SEC;
					struct_size = PATH_NAME_STRUCT_SIZE;
				} else {
					break;
				}
			} else if (item_type == PHOTO_NAME_DTYPE) {
				if (item_id < MAX_NUM_OF_PHOTO) {
					nsec = NAME_ITEM_PER_SEC;
					start_sec = PHOTO_NAME_START_SEC;
					struct_size = NAME_STRUCT_SIZE;
				} else {
					break;
				}
			} else if (item_type == PHOTO_PATH_DTYPE) {
				if (item_id < MAX_NUM_OF_PHOTO) {
					nsec = PATH_NAME_ITEM_PER_SEC;
					start_sec = PHOTO_PATH_START_SEC;
					struct_size = PATH_NAME_STRUCT_SIZE;
				} else {
					break;
				}
			} else if (item_type == MVIDEO_NAME_DTYPE) {
				if (item_id < MAX_NUM_OF_MVIDEO) {
					nsec = NAME_ITEM_PER_SEC;
					start_sec = MVIDEO_NAME_START_SEC;
					struct_size = NAME_STRUCT_SIZE;
				} else {
					break;
				}
			} else if (item_type == MVIDEO_PATH_DTYPE) {
				if (item_id < MAX_NUM_OF_MVIDEO) {
					nsec = PATH_NAME_ITEM_PER_SEC;
					start_sec = MVIDEO_PATH_START_SEC;
					struct_size = PATH_NAME_STRUCT_SIZE;
				} else {
					break;
				}
			}

			get_item_sec_Voice(media_type, nsec, start_sec, item_id, (uint32*) &dwSector, &in_sec_id, &lb_offset);
			writebuf_p = sec_temp_buf_X + (struct_size*in_sec_id) + lb_offset;
#ifdef	_ASSERT_
			assert((struct_size*in_sec_id)<NUM_OF_WORD_PER_SEC);
#endif
#ifdef _USE_FS_SECTAB_
			if ((ret_code = DataDriveReadSectorCache(dwSector, FLASH_DRIVEID, (INT)READ_TYPE_NORMAL, sec_temp_buf_X)) != SUCCESS) {
				/* error */
				break;
			}
#else
#ifdef _USE_FS_MUSICLIB_
			ret_code = Fseek(g_iFileHandle, dwSector*BYTE_PER_SECTOR, SEEK_SET);
			ret_code = Fread(g_iFileHandle, (INT* _X) sec_temp_buf_X , BYTE_PER_SECTOR, X_MEMORY, -1);
#else
			dwSector += addr_offset;
			if ((ret_code = DataDriveReadSectorCache(dwSector, FLASH_DRIVEID, (INT)READ_TYPE_NORMAL, sec_temp_buf_X)) != SUCCESS) {
				/* error */
				break;
			}
#endif
#endif
			for (i=0;i<cpylen;i++) {
				writebuf_p[i] = dbuf_p[i];
			}
#ifdef _USE_FS_SECTAB_
			ret_code = DataDriveWriteSectorCache(FLASH_DRIVEID, dwSector, sec_temp_buf_X, (INT)WRITE_TYPE_RANDOM);
#else
#ifdef _USE_FS_MUSICLIB_
			ret_code = Fseek(g_iFileHandle, dwSector*BYTE_PER_SECTOR, SEEK_SET);
			ret_code = Fwrite(g_iFileHandle, (INT* _X) sec_temp_buf_X , BYTE_PER_SECTOR, X_MEMORY, -1);
#else
			//SysLoadFATWrite();
			ret_code = DataDriveWriteSectorCache(FLASH_DRIVEID, dwSector, sec_temp_buf_X, (INT)WRITE_TYPE_RANDOM);
			//SysUnLoadFATWrite();
#endif
#endif
			break;
		case VOICE_MARKTABLE_DTYPE:
		case VOICE_INFO_DTYPE:
		case VOICE_ARRAY_DTYPE:
		case FMREC_MARKTABLE_DTYPE:
		case FMREC_INFO_DTYPE:
		case FMREC_ARRAY_DTYPE:
		case PHOTO_MARKTABLE_DTYPE:
		case PHOTO_INFO_DTYPE:
		case PHOTO_ARRAY_DTYPE:
		case MVIDEO_MARKTABLE_DTYPE:
		case MVIDEO_INFO_DTYPE:
		case MVIDEO_ARRAY_DTYPE:
			if (item_type == VOICE_MARKTABLE_DTYPE) {
				/* item_id = # of song to store */
				nsec = VOICE_MARKTABLE_SEC_SIZE;
				start_sec = VOICE_MARKTABLE_START_SEC;
			} else if (item_type == VOICE_INFO_DTYPE) {
				/* item_id = # of song to store */
				nsec = VOICE_INFO_SEC_SIZE;
				start_sec = VOICE_INFO_START_SEC;
			} else if (item_type == VOICE_ARRAY_DTYPE) {
				/* item_id = # of song to store */
				nsec = VOICE_ARRAY_SEC_SIZE;
				/* loop 3 times */
				start_sec = VOICE_ARRAY_START_SEC;
			} else if (item_type == FMREC_MARKTABLE_DTYPE) {
				/* item_id = # of song to store */
				nsec = VOICE_MARKTABLE_SEC_SIZE;
				start_sec = FMREC_MARKTABLE_START_SEC;
			} else if (item_type == FMREC_INFO_DTYPE) {
				/* item_id = # of song to store */
				nsec = VOICE_INFO_SEC_SIZE;
				start_sec = FMREC_INFO_START_SEC;
			} else if (item_type == FMREC_ARRAY_DTYPE) {
				/* item_id = # of song to store */
				nsec = FMREC_ARRAY_SEC_SIZE;
				/* loop 3 times */
				start_sec = FMREC_ARRAY_START_SEC;
			} else if (item_type == PHOTO_MARKTABLE_DTYPE) {
				/* item_id = # of photo to store */
				nsec = VOICE_MARKTABLE_SEC_SIZE;
				start_sec = PHOTO_MARKTABLE_START_SEC;
			} else if (item_type == PHOTO_INFO_DTYPE) {
				/* item_id = # of photo to store */
				nsec = VOICE_INFO_SEC_SIZE;
				start_sec = PHOTO_INFO_START_SEC;
			} else if (item_type == PHOTO_ARRAY_DTYPE) {
				/* item_id = # of photo to store */
				nsec = PHOTO_ARRAY_SEC_SIZE;
				/* loop 3 times */
				start_sec = PHOTO_ARRAY_START_SEC;
			} else if (item_type == MVIDEO_MARKTABLE_DTYPE) {
				/* item_id = # of movie to store */
				nsec = VOICE_MARKTABLE_SEC_SIZE;
				start_sec = MVIDEO_MARKTABLE_START_SEC;
			} else if (item_type == MVIDEO_INFO_DTYPE) {
				/* item_id = # of movie to store */
				nsec = VOICE_INFO_SEC_SIZE;
				start_sec = MVIDEO_INFO_START_SEC;
			} else if (item_type == MVIDEO_ARRAY_DTYPE) {
				/* item_id = # of movie to store */
				nsec = MVIDEO_ARRAY_SEC_SIZE;
				/* loop 3 times */
				start_sec = MVIDEO_ARRAY_START_SEC;
			}

			for (i=0;i<nsec;i++) {
#ifdef _USE_FS_MUSICLIB_
				dwSector = (start_sec + i);
				cal_offset(dwSector, &lb_offset);
#ifdef _USE_FS_SECTAB_
				get_map_sec_Voice(media_type, dwSector, &dwSector);
#endif
#else
				dwSector = addr_offset + (start_sec + i);
#endif
#ifndef _USE_16MDB_
#ifdef _USE_FS_SECTAB_
				ret_code = DataDriveReadSectorCache(dwSector, FLASH_DRIVEID, (INT)READ_TYPE_NORMAL, sec_temp_buf_X);
#endif
#endif
				writebuf_p = dbuf_p + (NUM_OF_WORD_PER_SEC*i);
				/* use writebuf_p for parameter in DataDriveWriteSectorCache will write random bytes to the sector boundary in flash */
				for (j=0;j<NUM_OF_WORD_PER_SEC;j++) {
					sec_temp_buf_X[lb_offset+j] = writebuf_p[j];
				}
#ifdef _USE_FS_SECTAB_
				ret_code = DataDriveWriteSectorCache(FLASH_DRIVEID, dwSector, sec_temp_buf_X, (INT)WRITE_TYPE_RANDOM);
#else
#ifdef _USE_FS_MUSICLIB_
				ret_code = Fseek(g_iFileHandle, dwSector*BYTE_PER_SECTOR, SEEK_SET);
				ret_code = Fwrite(g_iFileHandle, (INT* _X) sec_temp_buf_X , BYTE_PER_SECTOR, X_MEMORY, -1);
#else
				//SysLoadFATWrite();
//				ret_code = DataDriveWriteSectorCache(dwSector, FLASH_DRIVEID, (INT)WRITE_TYPE_RANDOM, writebuf_p);
				ret_code = DataDriveWriteSectorCache(FLASH_DRIVEID, dwSector, sec_temp_buf_X, (INT)WRITE_TYPE_RANDOM);
				//SysUnLoadFATWrite();
#endif
#endif
			}
			break;
#endif	/* VOICE_FUNCTION */
#ifndef _USE_FS_MUSICLIB_
		default:
#ifdef _ASSERT_
			assert(0);
#endif
			ret_code = (!SUCCESS);
			break;
#endif
	}
	setup_mask_lvl( interrupt_bk );
#ifdef _USE_FS_MUSICLIB_
	//	if (ret_code > 0) {
	return (SUCCESS);
	//	} else {
	//		return (!SUCCESS);
	//	}
#else
	return (ret_code);
#endif
}

/*================================================================================================*/
#endif	/* _USEFLASH_WRITE_ */

/*==================================================================================================
FUNCTION: ML_MemRead()

DESCRIPTION:
Description of this specific function.

ARGUMENTS PASSED:
uint24 media_type, uint24 item_type, uint24 item_id, uint24 *dbuf_p

RETURN VALUE:
dbuf_p is pointing to 1 set of struct data of item_type
(eg. to FLASH_GROUP_NAME_T, not only the name string, but name+unicode).

In case *_ASSOC_COUNT_DTYPE, dbuf_p points to a memory of uint24[2],
uint24[0]=size:uint24[1]=pos_relative_to_list_head

PRE-CONDITIONS:
None

POST-CONDITIONS:
None

IMPORTANT NOTES:
None

==================================================================================================*/
#ifdef BROWSE_PLAY_MODULE
#ifdef MERGE_MODULE
_reentrant uint24 ML_MemRead_Merge(uint24 media_type, uint24 item_type, uint24 item_id, uint24 *dbuf_p, uint24 cpylen) {
#else
_reentrant uint24 ML_MemRead_Browse(uint24 media_type, uint24 item_type, uint24 item_id, uint24 *dbuf_p, uint24 cpylen) {
#endif
#else
#ifdef VOICE_FUNCTION
COMP_OPT uint24 ML_MemRead_X(uint24 media_type, uint24 item_type, uint24 item_id, uint24 _X *dbuf_p, uint24 cpylen) {
#else
static uint24 ML_MemRead(uint24 media_type, uint24 item_type, uint24 item_id, uint24 *dbuf_p, uint24 cpylen) {
#endif
#endif
	uint24 i = 0;
	uint24 k;
	uint24 spsec = 0;
	uint24 start_sec = 0;
	uint24 idlvl_offset;
	uint24 struct_size;
	uint24 addr_offset;
	uint24 in_sec_id;
	uint24 ret_code;
	uint24 lb_offset;

	//uint32 dwSector;
	LONG dwSector;
	uint24 _X *readbuf_p;
	int interrupt_bk;

	interrupt_bk = setup_mask_lvl(1);

	if (media_type >= NUM_OF_MEDIA) return (!SUCCESS);

#ifndef _USE_FS_MUSICLIB_
	ret_code = (!SUCCESS);
	switch(media_type) {
		case	MEDIA_TYPE_FLASH:
			addr_offset = FLASH_DATABASE_ADR;
			break;
		case	MEDIA_TYPE_SD:
			addr_offset = SD_DATABASE_ADR;
			break;
		default :
			return(!SUCCESS);
	}
#endif
	switch(item_type) {
#ifndef MERGE_MODULE
		case ARTIST_NAME_DTYPE:
		case ALBUM_NAME_DTYPE:
		case GENRE_NAME_DTYPE:
		case SONG_NAME_DTYPE:
		case PATH_NAME_DTYPE:
		case LONG_FILENAME_DTYPE:
		case AUDIBLE_NAME_DTYPE:
		case AUDIBLE_PATH_DTYPE:
		case AUDIBLE_LONG_FILENAME_DTYPE:
		case ON_THE_FLY_PATH_DTYPE:
		case VOICE_NAME_DTYPE:
		case VOICE_PATH_DTYPE:
		case FMREC_NAME_DTYPE:
		case FMREC_PATH_DTYPE:
		case PHOTO_NAME_DTYPE:
		case PHOTO_PATH_DTYPE:
		case MVIDEO_NAME_DTYPE:
		case MVIDEO_PATH_DTYPE:
#if 1
			//if (( item_type == PATH_NAME_DTYPE ) || ( item_type == AUDIBLE_PATH_DTYPE )) {
			if (( item_type == PATH_NAME_DTYPE )  || ( item_type == AUDIBLE_PATH_DTYPE )
				|| ( item_type == ON_THE_FLY_PATH_DTYPE ) || ( item_type == VOICE_PATH_DTYPE )
				|| ( item_type == FMREC_PATH_DTYPE ) || ( item_type == PHOTO_PATH_DTYPE) || ( item_type == MVIDEO_PATH_DTYPE)) {
				spsec = PATH_NAME_ITEM_PER_SEC;
				struct_size = PATH_NAME_STRUCT_SIZE;
			} else if ((item_type == LONG_FILENAME_DTYPE) || (item_type == AUDIBLE_LONG_FILENAME_DTYPE)) {
				spsec = LONG_FILENAME_ITEM_PER_SEC;
				struct_size = LONG_FILENAME_STRUCT_SIZE;
			} else {
				struct_size = NAME_STRUCT_SIZE;
				spsec = NAME_ITEM_PER_SEC;
			}
			switch (item_type) {
				case VOICE_NAME_DTYPE:
					start_sec = VOICE_NAME_START_SEC;
					break;
				case VOICE_PATH_DTYPE:
					start_sec = VOICE_PATH_START_SEC;
					break;
				case FMREC_NAME_DTYPE:
					start_sec = FMREC_NAME_START_SEC;
					break;
				case FMREC_PATH_DTYPE:
					start_sec = FMREC_PATH_START_SEC;
					break;
				case PHOTO_NAME_DTYPE:
					start_sec = PHOTO_NAME_START_SEC;
					break;
				case PHOTO_PATH_DTYPE:
					start_sec = PHOTO_PATH_START_SEC;
					break;
				case MVIDEO_NAME_DTYPE:
					start_sec = MVIDEO_NAME_START_SEC;
					break;
				case MVIDEO_PATH_DTYPE:
					start_sec = MVIDEO_PATH_START_SEC;
					break;
				case AUDIBLE_NAME_DTYPE:
					start_sec = AUDIBLE_NAME_START_SEC;
					break;
				case AUDIBLE_PATH_DTYPE:
					start_sec = AUDIBLE_PATH_START_SEC;
					break;
				case ON_THE_FLY_PATH_DTYPE:
					start_sec = ON_THE_FLY_PATH_START_SEC;
					break;
				case LONG_FILENAME_DTYPE:
					start_sec = LONG_FILENAME_START_SEC;
					break;
				case AUDIBLE_LONG_FILENAME_DTYPE:
					start_sec = AUDIBLE_LONG_FILENAME_START_SEC;
					break;
				default:
					start_sec = ARTIST_NAME_START_SEC + item_type * NAME_SEC_SIZE;
					//break;
			}
#else
			struct_size = NAME_STRUCT_SIZE;
			spsec = NAME_ITEM_PER_SEC;
			switch(item_type) {
				case ARTIST_NAME_DTYPE:
					//spsec = ARTIST_NAME_ITEM_PER_SEC;
					start_sec = ARTIST_NAME_START_SEC;
					//struct_size = ARTIST_NAME_STRUCT_SIZE;
					break;
				case ALBUM_NAME_DTYPE:
					//spsec = ALBUM_NAME_ITEM_PER_SEC;
					start_sec = ALBUM_NAME_START_SEC;
					//struct_size = ALBUM_NAME_STRUCT_SIZE;
					break;
				case GENRE_NAME_DTYPE:
					//spsec = GENRE_NAME_ITEM_PER_SEC;
					start_sec = GENRE_NAME_START_SEC;
					//struct_size = GENRE_NAME_STRUCT_SIZE;
					break;
				case SONG_NAME_DTYPE:
					//spsec = SONG_NAME_ITEM_PER_SEC;
					start_sec = SONG_NAME_START_SEC;
					//struct_size = SONG_NAME_STRUCT_SIZE;
					break;
				case PATH_NAME_DTYPE:
					spsec = PATH_NAME_ITEM_PER_SEC;
					start_sec = PATH_NAME_START_SEC;
					struct_size = PATH_NAME_STRUCT_SIZE;
					break;
				case AUDIBLE_NAME_DTYPE:
					start_sec = AUDIBLE_NAME_START_SEC;
					break;
				case AUDIBLE_PATH_DTYPE:
					spsec = PATH_NAME_ITEM_PER_SEC;
					start_sec = AUDIBLE_PATH_START_SEC;
					struct_size = PATH_NAME_STRUCT_SIZE;
					break;
				case ON_THE_FLY_PATH_DTYPE:
					spsec = PATH_NAME_ITEM_PER_SEC;
					start_sec = ON_THE_FLY_PATH_START_SEC;
					struct_size = PATH_NAME_STRUCT_SIZE;
					break;
				case VOICE_NAME_DTYPE:
					start_sec = VOICE_NAME_START_SEC;
					break;
				case VOICE_PATH_DTYPE:
					spsec = PATH_NAME_ITEM_PER_SEC;
					start_sec = VOICE_PATH_START_SEC;
					struct_size = PATH_NAME_STRUCT_SIZE;
					break;
				case FMREC_NAME_DTYPE:
					start_sec = FMREC_NAME_START_SEC;
					break;
				case FMREC_PATH_DTYPE:
					spsec = PATH_NAME_ITEM_PER_SEC;
					start_sec = FMREC_PATH_START_SEC;
					struct_size = PATH_NAME_STRUCT_SIZE;
					break;
				case PHOTO_NAME_DTYPE:
					start_sec = PHOTO_NAME_START_SEC;
					break;
				case PHOTO_PATH_DTYPE:
					spsec = PATH_NAME_ITEM_PER_SEC;
					start_sec = PHOTO_PATH_START_SEC;
					struct_size = PATH_NAME_STRUCT_SIZE;
					break;
				case MVIDEO_NAME_DTYPE:
					start_sec = MVIDEO_NAME_START_SEC;
					break;
				case MVIDEO_PATH_DTYPE:
					spsec = PATH_NAME_ITEM_PER_SEC;
					start_sec = MVIDEO_PATH_START_SEC;
					struct_size = PATH_NAME_STRUCT_SIZE;
					break;
				/*
				default:
					// error
					break;
				*/
			}
#endif
#ifndef VOICE_FUNCTION
			get_item_sec(media_type, spsec, start_sec, item_id, (uint32*) &dwSector, &in_sec_id, &lb_offset);
#else
			get_item_sec_Voice(media_type, spsec, start_sec, item_id, (uint32*) &dwSector, &in_sec_id, &lb_offset);
#endif
			readbuf_p = sec_temp_buf_X + (struct_size*in_sec_id) + lb_offset;
#ifdef	_ASSERT_
			assert((struct_size*in_sec_id)<NUM_OF_WORD_PER_SEC);
#endif
#ifdef _USE_FS_SECTAB_
			//dwSector = 0x7D000;
			if ((ret_code = DataDriveReadSectorCache(dwSector, FLASH_DRIVEID, (INT)READ_TYPE_NORMAL, sec_temp_buf_X)) != SUCCESS) {
				/* error */
				break;
			}
#else
#ifdef _USE_FS_MUSICLIB_
			ret_code = Fseek(g_iFileHandle, dwSector*BYTE_PER_SECTOR, SEEK_SET);
			ret_code = Fread(g_iFileHandle, (INT* _X) sec_temp_buf_X , BYTE_PER_SECTOR, X_MEMORY, -1);
#else
			dwSector += addr_offset;
			if ((ret_code = DataDriveReadSectorCache(dwSector, FLASH_DRIVEID, (INT)READ_TYPE_NORMAL, sec_temp_buf_X)) != SUCCESS) {
				/* error */
				break;
			}
#endif
#endif
			for (i=0;i<cpylen;i++) {
				dbuf_p[i] = readbuf_p[i];
			}
			ret_code = SUCCESS;
			break;
#ifndef VOICE_FUNCTION
		case ON_THE_FLY_DTYPE:
#ifdef _USEFLASH_WRITE_
			/* use at build function only */
		case NEW_MUSIC_DTYPE:
			if (item_type == NEW_MUSIC_DTYPE) {
				/* item_id is not used */
				dwSector = NEW_MUSIC_START_SEC;
				struct_size = NEW_MUSIC_STRUCT_SIZE;
			} else
				/* use at build function only */
#endif
			{
				/* item_id is not used */
				dwSector = ON_THE_FLY_START_SEC;
				struct_size = ON_THE_FLY_STRUCT_SIZE;
			}
#ifdef _USE_FS_SECTAB_
			cal_offset(dwSector, &lb_offset);
#ifdef USE_FUNC_GET_MAP_SEC
			get_map_sec_Browse(media_type, dwSector, &dwSector);
#else
			get_map_sec(media_type, dwSector, &dwSector);
#endif
			if ((ret_code = DataDriveReadSectorCache(dwSector, FLASH_DRIVEID, (INT)READ_TYPE_NORMAL, sec_temp_buf_X)) != SUCCESS) {
				/* error */
				break;
			}
#else
#ifdef _USE_FS_MUSICLIB_
			ret_code = Fseek(g_iFileHandle, dwSector*BYTE_PER_SECTOR, SEEK_SET);
			ret_code = Fread(g_iFileHandle, (INT* _X) sec_temp_buf_X , BYTE_PER_SECTOR, X_MEMORY, -1);
#else
			dwSector += addr_offset;
			if ((ret_code = DataDriveReadSectorCache(dwSector, FLASH_DRIVEID, (INT)READ_TYPE_NORMAL, sec_temp_buf_X)) != SUCCESS) {
				/* error */
				break;
			}
#endif
#endif
			for (i=0;i<struct_size;i++) {
				dbuf_p[i] = sec_temp_buf_X[lb_offset+i];
			}
			break;
		case SONG_YEAR_DTYPE:
		case NEW_1MONTH_DTYPE:
		case NEW_1WEEK_DTYPE:
		case NEW_1DAY_DTYPE:
		case NUM_OF_NEW_1MONTH_DTYPE:
		case NUM_OF_NEW_1WEEK_DTYPE:
		case NUM_OF_NEW_1DAY_DTYPE:
			if (item_type == SONG_YEAR_DTYPE) {
				get_item_sec(media_type, SONG_YEAR_ITEM_PER_SEC, SONG_YEAR_START_SEC, item_id, (uint32*) &dwSector, &in_sec_id, &lb_offset);
			} else {
				if ((item_type >= NUM_OF_NEW_1DAY_DTYPE) && (item_type <= NUM_OF_NEW_1MONTH_DTYPE)) {
#if 1
					in_sec_id = (NEW_MUSIC_STRUCT_SIZE+ADDR_FLASH_NUM_OF_NEW_1DAY+(item_type-NUM_OF_NEW_1DAY_DTYPE));
#else
					if (item_type == NUM_OF_NEW_1MONTH_DTYPE) {
						in_sec_id = (NEW_MUSIC_STRUCT_SIZE+ADDR_FLASH_NUM_OF_NEW_1MONTH);
					} else if (item_type == NUM_OF_NEW_1WEEK_DTYPE) {
						in_sec_id = (NEW_MUSIC_STRUCT_SIZE+ADDR_FLASH_NUM_OF_NEW_1WEEK);
					} else if (item_type == NUM_OF_NEW_1DAY_DTYPE) {
						in_sec_id = (NEW_MUSIC_STRUCT_SIZE+ADDR_FLASH_NUM_OF_NEW_1DAY);
					}
#endif
				} else {
					in_sec_id = (NEW_MUSIC_STRUCT_SIZE+item_id+NUM_OF_SONG_IN_NEW_MUSIC);	/* merged_newmusic is at 44word offset */
				}
#ifdef _USE_FS_SECTAB_
				cal_offset(NEW_MUSIC_START_SEC, &lb_offset);
#ifdef USE_FUNC_GET_MAP_SEC
				get_map_sec_Browse(MEDIA_TYPE_FLASH, NEW_MUSIC_START_SEC, &dwSector);
#else
				get_map_sec(MEDIA_TYPE_FLASH, NEW_MUSIC_START_SEC, &dwSector);
#endif
#else
				dwSector = NEW_MUSIC_START_SEC;
				cal_offset(dwSector, &lb_offset);
				addr_offset = FLASH_DATABASE_ADR;	/* locate at flash DB area */
#endif
			}
#ifdef _USE_FS_SECTAB_
			if ((ret_code = DataDriveReadSectorCache(dwSector, FLASH_DRIVEID, (INT)READ_TYPE_NORMAL, sec_temp_buf_X)) != SUCCESS) {
				/* error */
				break;
			}
#else
#ifdef _USE_FS_MUSICLIB_
			ret_code = Fseek(g_iFileHandle, dwSector*BYTE_PER_SECTOR, SEEK_SET);
			ret_code = Fread(g_iFileHandle, (INT* _X) sec_temp_buf_X , BYTE_PER_SECTOR, X_MEMORY, -1);
#else
			dwSector += addr_offset;
			if ((ret_code = DataDriveReadSectorCache(dwSector, FLASH_DRIVEID, (INT)READ_TYPE_NORMAL, sec_temp_buf_X)) != SUCCESS) {
				/* error */
				break;
			}
#endif
#endif
			*dbuf_p = sec_temp_buf_X[in_sec_id + lb_offset];
			break;
		case M3U_MUSIC_DTYPE:
#ifdef _ASSERT_
			assert(0);
#endif
			break;
#endif	/* VOICE_FUNCTION */
#endif	/* MERGE_MODULE */
		case NUM_OF_SONG_DTYPE:
		case NUM_OF_ARTIST_DTYPE:
		case NUM_OF_ALBUM_DTYPE:
		case NUM_OF_GENRE_DTYPE:
		case NUM_OF_YEAR_DTYPE:
		case NUM_OF_VOICE_DTYPE:
		case NUM_OF_PHOTO_DTYPE:
		case NUM_OF_MVIDEO_DTYPE:
			/*
			#ifdef VOICE_FUNCTION
			*dbuf_p = 4;
			__asm("debug");
			break;
			#endif
			*/
		case NUM_OF_FMREC_DTYPE:
		case AUDIBLE_DTYPE:
			if (item_type == NUM_OF_VOICE_DTYPE) {
				/* item_id is not used */
				dwSector = VOICE_NUM_OF_START_SEC;
			} else if (item_type == NUM_OF_FMREC_DTYPE) {
				/* item_id is not used */
				dwSector = FMREC_NUM_OF_START_SEC;
			} else if (item_type == NUM_OF_PHOTO_DTYPE) {
				/* item_id is not used */
				dwSector = PHOTO_NUM_OF_START_SEC;
			} else if (item_type == NUM_OF_MVIDEO_DTYPE) {
				/* item_id is not used */
				dwSector = MVIDEO_NUM_OF_START_SEC;
			} else if (item_type == AUDIBLE_DTYPE) {
				/* item_id is not used */
				dwSector = AUDIBLE_NUM_OF_START_SEC;
				media_type = MEDIA_TYPE_FLASH;
#ifndef _USE_FS_SECTAB_
#ifndef _USE_FS_MUSICLIB_
				addr_offset = FLASH_DATABASE_ADR;
#endif
#endif
			} else {
				/* item_id is not used */
				dwSector = FLASH_NUM_OF_START_SEC;
			}
#ifdef _USE_FS_SECTAB_
			cal_offset(dwSector, &lb_offset);
#ifdef USE_FUNC_GET_MAP_SEC
			get_map_sec_Browse(media_type, dwSector, &dwSector);
#else
#ifdef VOICE_FUNCTION
			get_map_sec_Voice(media_type, dwSector, &dwSector);
#else
			get_map_sec(media_type, dwSector, &dwSector);
#endif
#endif
			if ((ret_code = DataDriveReadSectorCache(dwSector, FLASH_DRIVEID, (INT)READ_TYPE_NORMAL, sec_temp_buf_X)) != SUCCESS) {
				/* error */
				break;
			}
#else
#ifdef _USE_FS_MUSICLIB_
			ret_code = Fseek(g_iFileHandle, dwSector*BYTE_PER_SECTOR, SEEK_SET);
			ret_code = Fread(g_iFileHandle, (INT* _X) sec_temp_buf_X , BYTE_PER_SECTOR, X_MEMORY, -1);
#else
			dwSector += addr_offset;
			if ((ret_code = DataDriveReadSectorCache(dwSector, FLASH_DRIVEID, (INT)READ_TYPE_NORMAL, sec_temp_buf_X)) != SUCCESS) {
				/* error */
				break;
			}
#endif
#endif
#if 1
			switch (item_type) {
				case NUM_OF_SONG_DTYPE:
					*dbuf_p = sec_temp_buf_X[lb_offset+ADDR_FLASH_NUM_OF_SONG];
					break;
				case NUM_OF_YEAR_DTYPE:
					*dbuf_p = sec_temp_buf_X[lb_offset+ADDR_FLASH_NUM_OF_YEAR];
					break;
				case NUM_OF_VOICE_DTYPE:
					*dbuf_p = sec_temp_buf_X[lb_offset+ADDR_FLASH_NUM_OF_VOICE];
					break;
				case NUM_OF_FMREC_DTYPE:
					*dbuf_p = sec_temp_buf_X[lb_offset+ADDR_FLASH_NUM_OF_FMREC];
					break;
				case NUM_OF_PHOTO_DTYPE:
					*dbuf_p = sec_temp_buf_X[lb_offset+ADDR_FLASH_NUM_OF_PHOTO];
					break;
				case NUM_OF_MVIDEO_DTYPE:
					*dbuf_p = sec_temp_buf_X[lb_offset+ADDR_FLASH_NUM_OF_MVIDEO];
					break;
				case AUDIBLE_DTYPE:
					i = sec_temp_buf_X[lb_offset+ADDR_NUM_OF_AUDIBLE];
					break;
				default:
					*dbuf_p = sec_temp_buf_X[lb_offset+(ADDR_FLASH_NUM_OF_ARTIST-item_type+NUM_OF_ARTIST_DTYPE)];
					break;
			}
#else
			if (item_type == NUM_OF_SONG_DTYPE) {
				*dbuf_p = sec_temp_buf_X[ADDR_FLASH_NUM_OF_SONG];
			} else if (item_type == NUM_OF_ARTIST_DTYPE) {
				*dbuf_p = sec_temp_buf_X[ADDR_FLASH_NUM_OF_ARTIST];
			} else if (item_type == NUM_OF_ALBUM_DTYPE) {
				*dbuf_p = sec_temp_buf_X[ADDR_FLASH_NUM_OF_ALBUM];
			} else if (item_type == NUM_OF_GENRE_DTYPE) {
				*dbuf_p = sec_temp_buf_X[ADDR_FLASH_NUM_OF_GENRE];
			} else if (item_type == NUM_OF_YEAR_DTYPE) {
				*dbuf_p = sec_temp_buf_X[ADDR_FLASH_NUM_OF_YEAR];
			/*
			} else {
				// error
				break;
			*/
			}
#endif
			if (item_type != AUDIBLE_DTYPE) break;
			/* code for AUDIBLE_DTYPE only */
			start_sec = AUDIBLE_NUM_OF_START_SEC;
			if (item_id < i) {
#ifdef _USE_FS_SECTAB_
				media_type = MEDIA_TYPE_FLASH;
#endif
#ifndef _USE_FS_MUSICLIB_
				media_type = FLASH_DATABASE_ADR;
				addr_offset = FLASH_DATABASE_ADR;
#endif
			} else {
				item_id = item_id - i;
#ifdef _USE_FS_SECTAB_
				media_type = MEDIA_TYPE_SD;
#endif
#ifndef _USE_FS_MUSICLIB_
				media_type = SD_DATABASE_ADR;
				addr_offset = SD_DATABASE_ADR;
#endif
			}
			/* code for AUDIBLE_DTYPE continue down */
		case SONG_ARRAY_DTYPE:
		case ARTIST_SORT_DTYPE:
		case ARTIST_ASSOC_COUNT_DTYPE:
		case ARTIST_ASSOC_CID_DTYPE:
		case ARTIST_SUBASSOC_COUNT_DTYPE:
		case ARTIST_SUBASSOC_CID_DTYPE:
		case ALBUM_SORT_DTYPE:
		case ALBUM_ASSOC_COUNT_DTYPE:
		case ALBUM_ASSOC_CID_DTYPE:
		case GENRE_SORT_DTYPE:
		case GENRE_ASSOC_COUNT_DTYPE:
		case GENRE_ASSOC_CID_DTYPE:
		case YEAR_SORT_DTYPE:
		case YEAR_ASSOC_COUNT_DTYPE:
		case YEAR_ASSOC_CID_DTYPE:
		case SONG_INFO_ARTIST_DTYPE:
		case SONG_INFO_ALBUM_DTYPE:
		case SONG_INFO_GENRE_DTYPE:
		case SONG_INFO_YEAR_DTYPE:
		case VOICE_ARRAY_DTYPE:
		case FMREC_ARRAY_DTYPE:
		case PHOTO_ARRAY_DTYPE:
		case MVIDEO_ARRAY_DTYPE:
		case TRACKNUM_ARRAY_DTYPE:
#ifdef MERGE_MODULE
		case ARTIST_MERGE_DTYPE:
		case ALBUM_MERGE_DTYPE:
		case GENRE_MERGE_DTYPE:
		case SONG_MERGE_DTYPE:
		case YEAR_MERGE_DTYPE:
#endif	/* MERGE_MODULE */
			idlvl_offset = 0x0;
			spsec = INDEX_ITEM_PER_SEC;
			switch(item_type) {
				case SONG_ARRAY_DTYPE:
					//spsec = SONG_ARRAY_ITEM_PER_SEC;
					start_sec = SONG_ARRAY_START_SEC;
					break;
				case ARTIST_SORT_DTYPE:
					//spsec = SORT_ITEM_PER_SEC;
					start_sec = ARTIST_SORT_START_SEC;
					break;
				case ARTIST_ASSOC_COUNT_DTYPE:
					//spsec = ASSOC_ITEM_PER_SEC;
					start_sec = ARTIST_ASSOC_START_SEC;
					break;
				case ARTIST_SUBASSOC_COUNT_DTYPE:
					//spsec = SUBASSOC_ITEM_PER_SEC;
					start_sec = ARTIST_SUBASSOC_START_SEC;
					break;
				case ALBUM_SORT_DTYPE:
					//spsec = SORT_ITEM_PER_SEC;
					start_sec = ALBUM_SORT_START_SEC;
					break;
				case ALBUM_ASSOC_COUNT_DTYPE:
					//spsec = ASSOC_ITEM_PER_SEC;
					start_sec = ALBUM_ASSOC_START_SEC;
					break;
				case GENRE_SORT_DTYPE:
					//spsec = SORT_ITEM_PER_SEC;
					start_sec = GENRE_SORT_START_SEC;
					break;
				case GENRE_ASSOC_COUNT_DTYPE:
					//spsec = ASSOC_ITEM_PER_SEC;
					start_sec = GENRE_ASSOC_START_SEC;
					break;
				case YEAR_SORT_DTYPE:
					//spsec = SORT_ITEM_PER_SEC;
					start_sec = YEAR_SORT_START_SEC;
					break;
				case YEAR_ASSOC_COUNT_DTYPE:
					//spsec = ASSOC_ITEM_PER_SEC;
					start_sec = YEAR_ASSOC_START_SEC;
					break;

				case ARTIST_ASSOC_CID_DTYPE:
					//spsec = ASSOC_ITEM_PER_SEC;
					start_sec = ARTIST_ASSOC_START_SEC;
					idlvl_offset = MAX_NUM_OF_SONG;
					break;
				case ARTIST_SUBASSOC_CID_DTYPE:
					//spsec = SUBASSOC_ITEM_PER_SEC;
					start_sec = ARTIST_SUBASSOC_START_SEC;
					idlvl_offset = MAX_NUM_OF_SONG;
					break;
				case ALBUM_ASSOC_CID_DTYPE:
					//spsec = ASSOC_ITEM_PER_SEC;
					start_sec = ALBUM_ASSOC_START_SEC;
					idlvl_offset = MAX_NUM_OF_SONG;
					break;
				case GENRE_ASSOC_CID_DTYPE:
					//spsec = ASSOC_ITEM_PER_SEC;
					start_sec = GENRE_ASSOC_START_SEC;
					idlvl_offset = MAX_NUM_OF_SONG;
					break;
				case YEAR_ASSOC_CID_DTYPE:
					//spsec = ASSOC_ITEM_PER_SEC;
					start_sec = YEAR_ASSOC_START_SEC;
					idlvl_offset = MAX_NUM_OF_SONG;
					break;
				case SONG_INFO_ARTIST_DTYPE:
					//spsec = SONG_INFO_ARTIST_ITEM_PER_SEC;
					start_sec = SONG_INFO_ARTIST_START_SEC;
					break;
				case SONG_INFO_ALBUM_DTYPE:
					//spsec = SONG_INFO_ALBUM_ITEM_PER_SEC;
					start_sec = SONG_INFO_ALBUM_START_SEC;
					break;
				case SONG_INFO_GENRE_DTYPE:
					//spsec = SONG_INFO_GENRE_ITEM_PER_SEC;
					start_sec = SONG_INFO_GENRE_START_SEC;
					break;
				case SONG_INFO_YEAR_DTYPE:
					//spsec = SONG_INFO_YEAR_ITEM_PER_SEC;
					start_sec = SONG_INFO_YEAR_START_SEC;
					break;
				case VOICE_ARRAY_DTYPE:
					//spsec = VOICE_ARRAY_ITEM_PER_SEC;
					start_sec = VOICE_ARRAY_START_SEC;
					break;
				case FMREC_ARRAY_DTYPE:
					//spsec = FMREC_ARRAY_ITEM_PER_SEC;
					start_sec = FMREC_ARRAY_START_SEC;
					break;
				case PHOTO_ARRAY_DTYPE:
					//spsec = PHOTO_ARRAY_ITEM_PER_SEC;
					start_sec = PHOTO_ARRAY_START_SEC;
					break;
				case MVIDEO_ARRAY_DTYPE:
					//spsec = MVIDEO_ARRAY_ITEM_PER_SEC;
					start_sec = MVIDEO_ARRAY_START_SEC;
					break;
                case TRACKNUM_ARRAY_DTYPE:
                    start_sec = TRACKNUM_ARRAY_START_SEC;
                    break;
#ifdef MERGE_MODULE
				case ARTIST_MERGE_DTYPE:
					start_sec = ARTIST_MERGE_START_SEC;
					break;
				case ALBUM_MERGE_DTYPE:
					start_sec = ALBUM_MERGE_START_SEC;
					break;
				case GENRE_MERGE_DTYPE:
					start_sec = GENRE_MERGE_START_SEC;
					break;
				case SONG_MERGE_DTYPE:
					start_sec = SONG_MERGE_START_SEC;
					break;
				case YEAR_MERGE_DTYPE:
					start_sec = YEAR_MERGE_START_SEC;
					break;
#endif	/* MERGE_MODULE */
					// _do_not_add_ default:
			};
#ifdef VOICE_FUNCTION
			get_item_sec_Voice(media_type, spsec, start_sec, (item_id+idlvl_offset), (uint32*) &dwSector, &in_sec_id, &lb_offset);
#else
			get_item_sec(media_type, spsec, start_sec, (item_id+idlvl_offset), (uint32*) &dwSector, &in_sec_id, &lb_offset);
#endif
#ifdef _USE_FS_SECTAB_
			if ((ret_code = DataDriveReadSectorCache(dwSector, FLASH_DRIVEID, (INT)READ_TYPE_NORMAL, sec_temp_buf_X)) != SUCCESS) {
				/* error */
				break;
			}
#else
#ifdef _USE_FS_MUSICLIB_
			ret_code = Fseek(g_iFileHandle, dwSector*BYTE_PER_SECTOR, SEEK_SET);
			ret_code = Fread(g_iFileHandle, (INT* _X) sec_temp_buf_X , BYTE_PER_SECTOR, X_MEMORY, -1);
#else
			dwSector += addr_offset;
			if ((ret_code = DataDriveReadSectorCache(dwSector, FLASH_DRIVEID, (INT)READ_TYPE_NORMAL, sec_temp_buf_X)) != SUCCESS) {
				/* error */
				break;
			}
#endif
#endif
#ifdef BROWSE_PLAY_MODULE
			ML_list_get_val_X_Browse((uint24 _X *)&sec_temp_buf_X[lb_offset], in_sec_id, dbuf_p);
#else
#ifdef VOICE_FUNCTION
			ML_list_get_val_X((uint24 _X *)&sec_temp_buf_X[lb_offset], in_sec_id, &i);
			*dbuf_p = i;
#else
			ML_list_get_val_X((uint24 _X *)&sec_temp_buf_X[lb_offset], in_sec_id, dbuf_p);
#endif
#endif
			break;
			/* code for AUDIBLE_DTYPE end */
#ifndef VOICE_FUNCTION
#ifndef MERGE_MODULE
		case NUM_OF_AUDIBLE_DTYPE:
			*dbuf_p = 0;
			for (i=0;i<NUM_OF_MEDIA;i++) {
#ifdef _USE_FS_SECTAB_
				cal_offset(AUDIBLE_NUM_OF_START_SEC, &lb_offset);
#ifdef USE_FUNC_GET_MAP_SEC
				get_map_sec_Browse((MEDIA_TYPE_FLASH+i), AUDIBLE_NUM_OF_START_SEC, &dwSector);
#else
				get_map_sec((MEDIA_TYPE_FLASH+i), AUDIBLE_NUM_OF_START_SEC, &dwSector);
#endif
				if ((ret_code = DataDriveReadSectorCache(dwSector, FLASH_DRIVEID, (INT)READ_TYPE_NORMAL, sec_temp_buf_X)) != SUCCESS) {
					/* error */
					break;
				}
#else
#ifdef _USE_FS_MUSICLIB_
				if (i ==1) {
					/* error case, FS for SD not implemented */
					_asm("debug");
					ret_code = Fseek(g_iFileHandle, ((AUDIBLE_NUM_OF_START_SEC+SD_DATABASE_ADR)*BYTE_PER_SECTOR), SEEK_SET);
					ret_code = Fread(g_iFileHandle, (INT* _X) sec_temp_buf_X , BYTE_PER_SECTOR, X_MEMORY, -1);
				} else {
					ret_code = Fseek(g_iFileHandle, dwSector*BYTE_PER_SECTOR, SEEK_SET);
					ret_code = Fread(g_iFileHandle, (INT* _X) sec_temp_buf_X , BYTE_PER_SECTOR, X_MEMORY, -1);
				}
#else
				if ((ret_code = DataDriveReadSectorCache((AUDIBLE_NUM_OF_START_SEC+(FLASH_DATABASE_ADR+(i*DATABASE_SIZE))), FLASH_DRIVEID, (INT)READ_TYPE_NORMAL, sec_temp_buf_X)) != SUCCESS) {
					/* error */
					break;
				}
#endif
#endif
				*dbuf_p = *dbuf_p + sec_temp_buf_X[lb_offset+ADDR_NUM_OF_AUDIBLE];

				if (g_is_SD_inserted != HAS_SD) break;
			}
			break;
#endif	/* MERGE_MODULE */
#else	/* VOICE_FUNCTION */
		case VOICE_MARKTABLE_DTYPE:
		case VOICE_INFO_DTYPE:
		case FMREC_MARKTABLE_DTYPE:
		case FMREC_INFO_DTYPE:
		case PHOTO_MARKTABLE_DTYPE:
		case PHOTO_INFO_DTYPE:
		case MVIDEO_MARKTABLE_DTYPE:
		case MVIDEO_INFO_DTYPE:
			if (item_type == VOICE_MARKTABLE_DTYPE) {
				/* item_id = # of song to store */
				spsec = VOICE_MARKTABLE_SEC_SIZE;
				start_sec = VOICE_MARKTABLE_START_SEC;
			} else if (item_type == VOICE_INFO_DTYPE) {
				/* item_id = # of song to store */
				spsec = VOICE_INFO_SEC_SIZE;
				start_sec = VOICE_INFO_START_SEC;
			} else if (item_type == FMREC_MARKTABLE_DTYPE) {
				/* item_id = # of song to store */
				spsec = VOICE_MARKTABLE_SEC_SIZE;
				start_sec = FMREC_MARKTABLE_START_SEC;
			} else if (item_type == FMREC_INFO_DTYPE) {
				/* item_id = # of song to store */
				spsec = VOICE_INFO_SEC_SIZE;
				start_sec = FMREC_INFO_START_SEC;
			} else if (item_type == PHOTO_MARKTABLE_DTYPE) {
				/* item_id = # of photo to store */
				spsec = VOICE_MARKTABLE_SEC_SIZE;
				start_sec = PHOTO_MARKTABLE_START_SEC;
			} else if (item_type == PHOTO_INFO_DTYPE) {
				/* item_id = # of photo to store */
				spsec = VOICE_INFO_SEC_SIZE;
				start_sec = PHOTO_INFO_START_SEC;
			} else if (item_type == MVIDEO_MARKTABLE_DTYPE) {
				/* item_id = # of movie to store */
				spsec = VOICE_MARKTABLE_SEC_SIZE;
				start_sec = MVIDEO_MARKTABLE_START_SEC;
			} else if (item_type == MVIDEO_INFO_DTYPE) {
				/* item_id = # of movie to store */
				spsec = VOICE_INFO_SEC_SIZE;
				start_sec = MVIDEO_INFO_START_SEC;
			}
			for (i=0;i<spsec;i++) {
#ifdef _USE_FS_MUSICLIB_
				dwSector = (start_sec + i);
				cal_offset(dwSector, &lb_offset);
#ifdef _USE_FS_SECTAB_
				get_map_sec_Voice(media_type, dwSector, &dwSector);
				if ((ret_code = DataDriveReadSectorCache(dwSector, FLASH_DRIVEID, (INT)READ_TYPE_NORMAL, sec_temp_buf_X)) != SUCCESS) {
					/* error */
					break;
				}
#else
				ret_code = Fseek(g_iFileHandle, dwSector*BYTE_PER_SECTOR, SEEK_SET);
				ret_code = Fread(g_iFileHandle, (INT* _X) sec_temp_buf_X , BYTE_PER_SECTOR, X_MEMORY, -1);
#endif
#else
				dwSector = addr_offset + (start_sec + i);
				if ((ret_code = DataDriveReadSectorCache(dwSector, FLASH_DRIVEID, (INT)READ_TYPE_NORMAL, sec_temp_buf_X)) != SUCCESS) {
					/* error */
					break;
				}
#endif
				readbuf_p = dbuf_p + (NUM_OF_WORD_PER_SEC*i);
				struct_size = NUM_OF_WORD_PER_SEC;
				if (((i == (VOICE_MARKTABLE_SEC_SIZE-1) ) && (item_type == VOICE_MARKTABLE_DTYPE)) ||
					((i == (VOICE_MARKTABLE_SEC_SIZE-1) ) && (item_type == FMREC_MARKTABLE_DTYPE)) ||
					((i == (VOICE_MARKTABLE_SEC_SIZE-1) ) && (item_type == PHOTO_MARKTABLE_DTYPE)) ||
					((i == (VOICE_MARKTABLE_SEC_SIZE-1) ) && (item_type == MVIDEO_MARKTABLE_DTYPE))) {
						struct_size = 79;
				} else if (((i == (VOICE_INFO_SEC_SIZE-1) ) && (item_type == VOICE_INFO_DTYPE)) ||
					((i == (VOICE_INFO_SEC_SIZE-1) ) && (item_type == FMREC_INFO_DTYPE)) ||
					((i == (VOICE_INFO_SEC_SIZE-1) ) && (item_type == PHOTO_INFO_DTYPE)) ||
					((i == (VOICE_INFO_SEC_SIZE-1) ) && (item_type == MVIDEO_INFO_DTYPE))) {
						struct_size = 162;
				}
				for (k=0;k<struct_size;k++) {
					readbuf_p[k] = sec_temp_buf_X[lb_offset+k];
				}
			}
			break;
#endif	/* VOICE_FUNCTION */
		/*
		default:
#ifdef _ASSERT_
			assert(0);
#endif
			ret_code = (!SUCCESS);
			break;
		*/
	}
	setup_mask_lvl( interrupt_bk );
#ifdef _USE_FS_MUSICLIB_
//	if (ret_code > 0) {
	return (SUCCESS);
//	} else {
//		return (!SUCCESS);
//	}
#else
	return (ret_code);
#endif
}

/*================================================================================================*/
