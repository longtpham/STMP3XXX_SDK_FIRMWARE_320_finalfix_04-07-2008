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

#ifdef _PC_SIMULATION_
#define COMP_OPT
#define _reentrant
#else
#include "filesystem.h"
#endif

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
#define ARTIST_ASSOC_COUNT_DTYPE		0x8
#define ARTIST_ASSOC_CID_DTYPE		0x9
#define ARTIST_SUBASSOC_COUNT_DTYPE 	0xA
#define ARTIST_SUBASSOC_CID_DTYPE 	0xB

#define ALBUM_SORT_DTYPE		0xC
#define ALBUM_ASSOC_COUNT_DTYPE		0xD
#define ALBUM_ASSOC_CID_DTYPE		0xE

#define GENRE_SORT_DTYPE		0x11
#define GENRE_ASSOC_COUNT_DTYPE		0x12
#define GENRE_ASSOC_CID_DTYPE		0x13
/*
#define GENRE_SORT_DTYPE		0xF
#define GENRE_ASSOC_COUNT_DTYPE		0x10
#define GENRE_ASSOC_CID_DTYPE		0x11
*/

#define YEAR_SORT_DTYPE			0x1B
#define YEAR_ASSOC_COUNT_DTYPE		0x1C
#define YEAR_ASSOC_CID_DTYPE		0x1D
/*
#define YEAR_SORT_DTYPE			0x12
#define YEAR_ASSOC_COUNT_DTYPE		0x13
#define YEAR_ASSOC_CID_DTYPE		0x14
*/
#ifdef _USEFLASH_WRITE_
#define NEW_MUSIC_DTYPE			0x30	/* for build call only */
#endif
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
//#define SONG_INFO_YEAR_DTYPE	0x43

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

#define MVIDEO_NAME_DTYPE		0x57
#define MVIDEO_PATH_DTYPE		0x58
#define MVIDEO_MARKTABLE_DTYPE	0x59
#define MVIDEO_INFO_DTYPE		0x5a
#define MVIDEO_ARRAY_DTYPE		0x5b
#define NUM_OF_MVIDEO_DTYPE		0x5c

// Note: defined according to "musiclib_flash_function.c"
#define LONG_FILENAME_DTYPE			0x5d
#define AUDIBLE_LONG_FILENAME_DTYPE	0x5e

#define ARTIST_MERGE_DTYPE	0x5f
#define ALBUM_MERGE_DTYPE	0x60
#define GENRE_MERGE_DTYPE	0x61
#define SONG_MERGE_DTYPE	0x62
#define YEAR_MERGE_DTYPE	0x63
#define TRACKNUM_ARRAY_DTYPE     0x64

//#define NUM_OF_SONG_DTYPE		0x21
#define NUM_OF_SONG_DTYPE		0x25
#define NUM_OF_ARTIST_DTYPE		0x22
#define NUM_OF_ALBUM_DTYPE		0x23
#define NUM_OF_GENRE_DTYPE		0x24
#define NUM_OF_YEAR_DTYPE		0x26

#define NUM_OF_NEW_1DAY_DTYPE	0x27
#define NUM_OF_NEW_1WEEK_DTYPE	0x28
#define NUM_OF_NEW_1MONTH_DTYPE	0x29
#define NUM_OF_AUDIBLE_DTYPE	0x2A

/*
** Use for all buffer malloc,
Read function return 512 Bytes, read buffer _must_be_ 171Words or larger
*/
//#define NUM_OF_READBYTE_PER_SEC		(512)	/* the DataDriveWriteSector() will write about 10 random bytes to buffer position>512 bytes, to prevent memory data overwritten, define > 512 */
#define NUM_OF_READBYTE_PER_SEC		(540)		/* the DataDriveWriteSector() will write about 10 random bytes to buffer position>512 bytes, to prevent memory data overwritten, define > 512 */
//#define NUM_OF_READWORD_PER_SEC		((NUM_OF_READBYTE_PER_SEC/NUM_OF_BYTE_IN_ONE_WORD)+1)	/* not divisible, +1 */
#define LB_NUM_OF_READBYTE_PER_SEC	(540*4)		/* the DataDriveWriteSector() will write about 10 random bytes to buffer position>512 bytes, to prevent memory data overwritten, define > 512 */
#define NUM_OF_READWORD_PER_SEC		(LB_NUM_OF_READBYTE_PER_SEC/NUM_OF_BYTE_IN_ONE_WORD)

/* locate at the highest 200 sectors of 1M */
//#define HASH_AREA1	0x738	/* for sdmd.c */
//#define HASH_AREA2	0x79C	/* for sdmd.c */
/* locate at the highest 200 sectors of 2M */
#ifdef _USE_16MDB_
#define HASH_AREA1	0xF38	/* for sdmd.c */
#define HASH_AREA2	0xF9C	/* for sdmd.c */
#else
/* locate at the highest 200 sectors(2048B/sec) of 4M */
#define HASH_DB_START	0x1CE0	/* for sdmd.c _must_use_ MEDIA_TYPE_SD(ie. 1) */
#define HASH_AREA1	0xCE0	/* for sdmd.c _must_use_ MEDIA_TYPE_SD(ie. 1) */
#define HASH_AREA2	0xD44	/* for sdmd.c _must_use_ MEDIA_TYPE_SD(ie. 1) */
#endif

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
#ifdef _PC_SIMULATION_
COMP_OPT void ML_MediaDetail(void);
#endif

#ifdef _USEFLASH_WRITE_
/* use at build function only */
/*==================================================================================================
FUNCTION: ML_MemWirte()

DESCRIPTION:
   Write to flash

ARGUMENTS PASSED:
 uint24 media_type,
 uint24 item_type,
 uint24 item_id,
 uint24 *dbuf_p			-- dbuf_p is pointing to 1 set of struct data of item_type

RETURN VALUE:
   return parameter description

PRE-CONDITIONS:
   dbuf_p is pointing to 1 set of struct data of item_type
(eg. to FLASH_GROUP_NAME_T, not only the name string, but name+unicode).

POST-CONDITIONS:
   None

IMPORTANT NOTES:
   None

==================================================================================================*/
static uint24 ML_MemWrite(uint24 media_type, uint24 item_type, uint24 item_id, uint24 _X *dbuf_p, uint24 cpylen);
#endif

/*==================================================================================================
FUNCTION: ML_MemRead()

DESCRIPTION:
   Read From flash

ARGUMENTS PASSED:
 uint24 media_type,
 uint24 item_type,
 uint24 item_id,
 uint24 *dbuf_p			-- dbuf_p is pointing to 1 set of struct data of item_type

RETURN VALUE:
   return parameter description

PRE-CONDITIONS:
   dbuf_p is pointing to 1 set of struct data of item_type
(eg. to FLASH_GROUP_NAME_T, not only the name string, but name+unicode).

POST-CONDITIONS:
   None

IMPORTANT NOTES:
   None

==================================================================================================*/
#ifdef BROWSE_PLAY_MODULE
_reentrant uint24 ML_MemRead_Browse(uint24 media_type, uint24 item_type, uint24 item_id, uint24 *dbuf_p, uint24 cpylen);
#else
#ifdef VOICE_FUNCTION
COMP_OPT uint24 ML_MemRead_X(uint24 media_type, uint24 item_type, uint24 item_id, uint24 _X *dbuf_p, uint24 cpylen);
#else
static uint24 ML_MemRead(uint24 media_type, uint24 item_type, uint24 item_id, uint24 *dbuf_p, uint24 cpylen);
#endif
#endif

/*================================================================================================*/
#ifdef __cplusplus
}
#endif

#endif  /* MUSICLIB_FLASH_FUNCTION_H */
