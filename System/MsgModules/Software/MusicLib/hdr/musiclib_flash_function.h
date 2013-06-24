#ifndef MUSICLIB_FLASH_FUNCTION_H
#define MUSICLIB_FLASH_FUNCTION_H

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
                                           INCLUDE FILES
==================================================================================================*/
#include "..\hdr\musiclib_hdr.h"

#include "filesystem.h"

/*==================================================================================================
                                             CONSTANTS
==================================================================================================*/
/* Use for all code access only, _do_not_use_for_buffer_malloc_, because read sector function always return 512bytes */
#define NUM_OF_BYTE_PER_SEC	(510)	/* divisible by NUM_OF_BYTE_IN_ONE_WORD */
#define NUM_OF_WORD_PER_SEC			(NUM_OF_BYTE_PER_SEC/NUM_OF_BYTE_IN_ONE_WORD)

/* select data type to R/W to */
#define ARTIST_NAME_DTYPE		0x0
#define ALBUM_NAME_DTYPE		0x1
#define GENRE_NAME_DTYPE		0x2
#define SONG_NAME_DTYPE			0x3

#define PATH_NAME_DTYPE			0x4
#define SONG_ARRAY_DTYPE		0x5
#define SONG_YEAR_DTYPE			0x6
#define ARTIST_SORT_DTYPE		0x7
#define ARTIST_ASSOC_COUNT_DTYPE		0x8 // ARTIST_ASSOC_COUNT_DTYPE + ITEM_ARTIST * 5 => 8 + 0 * 5
#define ARTIST_ASSOC_CID_DTYPE		0x9
#define ARTIST_SUBASSOC_COUNT_DTYPE 	0xA
#define ARTIST_SUBASSOC_CID_DTYPE 	0xB

#define ALBUM_SORT_DTYPE		0xC
#define ALBUM_ASSOC_COUNT_DTYPE		0xD // ARTIST_ASSOC_COUNT_DTYPE + ITEM_ALBUM * 5 => 8 + 1 * 5
#define ALBUM_ASSOC_CID_DTYPE		0xE

#define GENRE_SORT_DTYPE		0x11
#define GENRE_ASSOC_COUNT_DTYPE		0x12    // ARTIST_ASSOC_COUNT_DTYPE + ITEM_GENRE * 5 => 8 + 2 * 5
#define GENRE_ASSOC_CID_DTYPE		0x13

#define YEAR_SORT_DTYPE			0x1B
#define YEAR_ASSOC_COUNT_DTYPE		0x1C    // ARTIST_ASSOC_COUNT_DTYPE + ITEM_YEAR * 5 => 8 + 4 * 5
#define YEAR_ASSOC_CID_DTYPE		0x1D
#define NEW_MUSIC_DTYPE			0x30	/* for build call only */
#define ON_THE_FLY_DTYPE		0x31
#define M3U_MUSIC_DTYPE			0x32
#define NEW_1DAY_DTYPE			0x33
#define NEW_1WEEK_DTYPE			0x34
#define NEW_1MONTH_DTYPE		0x35
#define	AUDIBLE_DTYPE			0x36
#define AUDIBLE_NAME_DTYPE		0x37
#define AUDIBLE_PATH_DTYPE		0x38
#define ON_THE_FLY_PATH_DTYPE	0x39

#define SONG_INFO_ARTIST_DTYPE	0x40
#define SONG_INFO_ALBUM_DTYPE	0x41
#define SONG_INFO_GENRE_DTYPE	0x42
#define SONG_INFO_YEAR_DTYPE	0x44

#define VOICE_NAME_DTYPE		0x45
#define VOICE_PATH_DTYPE		0x46
#define VOICE_MARKTABLE_DTYPE	0x47
#define VOICE_INFO_DTYPE		0x48
#define VOICE_ARRAY_DTYPE		0x49
#define NUM_OF_VOICE_DTYPE		0x4A

#define FMREC_NAME_DTYPE		0x4B
#define FMREC_PATH_DTYPE		0x4C
#define FMREC_MARKTABLE_DTYPE	0x4D
#define FMREC_INFO_DTYPE		0x4E
#define FMREC_ARRAY_DTYPE		0x4F
#define NUM_OF_FMREC_DTYPE		0x50

#define PHOTO_NAME_DTYPE		0x51
#define PHOTO_PATH_DTYPE		0x52
#define PHOTO_MARKTABLE_DTYPE	0x53
#define PHOTO_INFO_DTYPE		0x54
#define PHOTO_ARRAY_DTYPE		0x55
#define NUM_OF_PHOTO_DTYPE		0x56

#define ARTIST_MERGE_DTYPE  0x5d
#define ALBUM_MERGE_DTYPE   0x5e
#define GENRE_MERGE_DTYPE   0x5f
#define SONG_MERGE_DTYPE    0x60
#define YEAR_MERGE_DTYPE    0x61
#define TRACKNUM_ARRAY_DTYPE    0x63

#define NUM_OF_SONG_DTYPE		0x25
#define NUM_OF_ARTIST_DTYPE		0x22
#define NUM_OF_ALBUM_DTYPE		0x23
#define NUM_OF_GENRE_DTYPE		0x24
#define NUM_OF_YEAR_DTYPE		0x26

#define NUM_OF_NEW_1DAY_DTYPE	0x27
#define NUM_OF_NEW_1WEEK_DTYPE	0x28
#define NUM_OF_NEW_1MONTH_DTYPE	0x29
#define NUM_OF_AUDIBLE_DTYPE	0x2A

#ifdef _FOLDER_BROWSE_
#define DIR_NAME_DTYPE                              0x64
#define DIR_PATH_DTYPE                              0x65
#define DIR_ARRAY_DTYPE                             0x66
#define PARENT_DIR_OF_DIR_ASSOC_COUNT_DTYPE         0x62    // ARTIST_ASSOC_COUNT_DTYPE + ITEM_INTERNAL * 5 => 8 + 18 * 5
#define PARENT_DIR_OF_DIR_TOTAL_ASSOC_COUNT_DTYPE   0x67
#define NUM_OF_DIR_DTYPE	                        0x68
#define PARENT_DIR_OF_DIR_START_DTYPE	            0x69
#define PARENT_DIR_OF_FILE_START_DTYPE	            0x6a
#define DIR_LIST_FILE_CONTENT_CID_DTYPE             0x6b
#endif  // _FOLDER_BROWSE_

#define PARENT_DIR_OF_FILE_ASSOC_LIST_DTYPE         0x6c
#define PARENT_DIR_OF_DIR_ASSOC_LIST_DTYPE          0x6d

/*
 ** Use for all buffer malloc,
 Read function return 512 Bytes, read buffer _must_be_ 171Words or larger
 */
#define NUM_OF_READBYTE_PER_SEC		(540)		/* the DataDriveWriteSector() will write about 10 random bytes to buffer position>512 bytes, to prevent memory data overwritten, define > 512 */
#define LB_NUM_OF_READBYTE_PER_SEC	(540*4)		/* the DataDriveWriteSector() will write about 10 random bytes to buffer position>512 bytes, to prevent memory data overwritten, define > 512 */
#define NUM_OF_READWORD_PER_SEC		(LB_NUM_OF_READBYTE_PER_SEC/NUM_OF_BYTE_IN_ONE_WORD)

/* locate at the highest 200 sectors of 1M */
/* locate at the highest 200 sectors of 2M */
/* locate at the highest 200 sectors(2048B/sec) of 4M */
#ifdef _SUPPORT_2000_SONGS_
#define HASH_DB_START	0x3CE0	/* for sdmd.c _must_use_ MEDIA_TYPE_SD(ie. 1),  SD_DATABASE_ADR + HASH_AREA1 => 0x1000 + 0xce0 */
#define HASH_AREA1	0x1CE0	/* for sdmd.c _must_use_ MEDIA_TYPE_SD(ie. 1) */
#define HASH_AREA2	0x1D44	/* for sdmd.c _must_use_ MEDIA_TYPE_SD(ie. 1) */
#else
#define HASH_DB_START	0x1CE0	/* for sdmd.c _must_use_ MEDIA_TYPE_SD(ie. 1),  SD_DATABASE_ADR + HASH_AREA1 => 0x1000 + 0xce0 */
#define HASH_AREA1	0xCE0	/* for sdmd.c _must_use_ MEDIA_TYPE_SD(ie. 1) */
#define HASH_AREA2	0xD44	/* for sdmd.c _must_use_ MEDIA_TYPE_SD(ie. 1) */
#endif  // _SUPPORT_2000_SONGS_

/*==================================================================================================
                                               MACROS
==================================================================================================*/

/*==================================================================================================
                                               ENUMS
==================================================================================================*/

/*==================================================================================================
                                     STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
                                        FUNCTION PROTOTYPES
==================================================================================================*/

/*================================================================================================*/
#ifdef __cplusplus
}
#endif

#endif  /* MUSICLIB_FLASH_FUNCTION_H */
