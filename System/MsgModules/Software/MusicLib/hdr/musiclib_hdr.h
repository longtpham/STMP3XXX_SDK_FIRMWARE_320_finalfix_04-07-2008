#ifndef MUSICLIB_HDR_H
#define MUSICLIB_HDR_H

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
#include "..\ghdr\musiclib_ghdr.h"

/*==================================================================================================
								Conditional Compilation Directives
==================================================================================================*/
/* To specify the music library is run in the player */
#define _ADD_ENTRY_TO_LIBRARY_ALOG_IN_PLAYER_

#define	_24_bit_word_

/*==================================================================================================
						Debug compile only Conditional Compilation Directives
==================================================================================================*/

/*==================================================================================================
                                             CONSTANTS
==================================================================================================*/
/* maximum number of artist */
#define MAX_NUM_OF_ARTIST (MAX_NUM_OF_SONG)
/* maximum number of album */
#define MAX_NUM_OF_ALBUM (MAX_NUM_OF_SONG)
/* maximum number of genre */
#define MAX_NUM_OF_GENRE (MAX_NUM_OF_SONG)
/* maximum number of year */
#define MAX_NUM_OF_YEAR (MAX_NUM_OF_SONG)
/* number of bit in one DSP word */
#define NUM_OF_BIT_IN_ONE_WORD (24)
/* number of bit for one item = log2(MAX_NUM_OF_SONG), corresponding to the absolute
number of song (2048) supported set above */
#define NUM_OF_BIT_FOR_ONE_ITEM (12)
/* number of item in one DSP word */
#define NUM_OF_ITEM_IN_ONE_WORD (NUM_OF_BIT_IN_ONE_WORD/NUM_OF_BIT_FOR_ONE_ITEM)
/* in the flash child array for one group, there will be two (2) information, one is
number of child for that group ID, another information is the child ID(s)*/
#define NUM_OF_LIST_IN_CHILD_ARRAY (2)

/* distance in mark table */
#define DISTANCE_IN_MARK_TABLE (100) /* must be divisible by MAX_NUM_OF_SONG */
/* number of entry in mark table */
#define NUM_OF_MARK_TABLE_ENTRY (2 * (MAX_NUM_OF_SONG/DISTANCE_IN_MARK_TABLE) - 1)	/* The
table entries once set will not be changed during the whole insert process, so the total
number of entries is doubled */
/* in mark table, one column is ID, another is distance */
#define NUM_OF_MARK_TABLE_COL (2)
/* length of string stored in mark table */
#define LEN_OF_SUB_STRING_IN_BYTE (24) /* must be an integer multiple of 3 */

/*==================================================================================================
                                               MACROS
==================================================================================================*/

/*==================================================================================================
                                               ENUMS
==================================================================================================*/

#define	TRUE	1
#define	FALSE	0

#define	EOSTR	0x00

#define BITSET_NAME_ALLASCII	(0x0)
#define BITSET_NAME_UNICODE		(0x01)
#define BITSET_NAME_MIXCODE		(0x02)

/* ram_artist_marktable = id0, ram_album_marktable = id1, ram_genre_marktable = id2, id3 reserved for song, ram_year_marktable = id4 */
#define MAX_NUM_OF_GROUP	0x5	/* include ram_song_marktable */

/* Constant for UI State */
#define  STATE_MAIN_MENU    0xA8  /* Main Menu */
#define  STATE_ARTIST_LVL_1 0x00  /* Show Aritst Name */
#define  STATE_ARTIST_LVL_2 0x01  /* Show Album Name for particular Artist */
#define  STATE_ARTIST_LVL_3 0x02  /* Show Song Name for particular Artist */
#define  STATE_ALBUM_LVL_1  0x04  /* Show Ablum Name */
#define  STATE_ALBUM_LVL_2  0x05  /* Show Song Name for particular Album */
#define  STATE_GENRE_LVL_1  0x08  /* Show Genre Name */
#define  STATE_GENRE_LVL_2  0x09  /* Show Song Name for particular Genre */
#define  STATE_TRACK_LVL_1  0x0C  /* Show Song Name */
#define  STATE_YEAR_LVL_1   0x10  /* Show Year */
#define  STATE_YEAR_LVL_2   0x11  /* Show Song Name for paritcular Year */
#define	 STATE_1DAY	        0x28  /* Show Song in New Music -- 1 Day */
#define	 STATE_1WEEK	    0x2C  /* Show Song in New Music -- 1 Week */
#define	 STATE_1MONTH	    0x30  /* Show Song in New Music -- 1 Month */
#define	 STATE_AUDIBLE	    0x34  /* Show Song in Spocken Word */
#define	 STATE_ONTHEGO	    0x38  /* Show Song in On The Go */
#define	 STATE_VOICE	    0x3C  /* Show Song in Voice */
#define	 STATE_FMREC	    0x40  /* Show Song in FM Rec */
#define  STATE_PHOTO		0x44  /* Show Picture in Photo */
#ifdef _FOLDER_BROWSE_
// The definition of internal / external state depends on _MAX_DIR_DEPTH
// STATE_INTERNAL_LVL_1 = ITEM_INTERNAL << 8
// STATE_EXTERNAL_LVL_1 = ITEM_EXTERNAL << 8
#define  STATE_INTERNAL_LVL_1   0x90  /* Show dir / song in internal media */
#define  STATE_INTERNAL_LVL_2   0x91  /* Show dir / song in internal media */
#define  STATE_INTERNAL_LVL_3	0x92  /* Show dir / song in internal media */
#define  STATE_INTERNAL_LVL_4	0x93  /* Show dir / song in internal media */
#define  STATE_INTERNAL_LVL_5	0x94  /* Show dir / song in internal media */
#define  STATE_INTERNAL_LVL_6	0x95  /* Show dir / song in internal media */
#define  STATE_INTERNAL_LVL_7   0x96  /* Show dir / song in internal media */
#define  STATE_INTERNAL_LVL_8	0x97  /* Show dir / song in internal media */
#define  STATE_EXTERNAL_LVL_1   0x98  /* Show dir / song in external media */
#define  STATE_EXTERNAL_LVL_2   0x99  /* Show dir / song in external media */
#define  STATE_EXTERNAL_LVL_3   0x9a  /* Show dir / song in external media */
#define  STATE_EXTERNAL_LVL_4   0x9b  /* Show dir / song in external media */
#define  STATE_EXTERNAL_LVL_5   0x9c  /* Show dir / song in external media */
#define  STATE_EXTERNAL_LVL_6   0x9d  /* Show dir / song in external media */
#define  STATE_EXTERNAL_LVL_7   0x9e  /* Show dir / song in external media */
#define  STATE_EXTERNAL_LVL_8   0x9f  /* Show dir / song in external media */
#endif  // _FOLDER_BROWSE_
#define  STATE_PLAY_SONG    0xAB  /* Play Song */
#define  STATE_PLAY_ALL     0xAC  /* Play All Song */

#define ML_SONG_NAME_STRUCT_SIZE		(LEN_OF_NAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD + 1)
#define ML_PATH_NAME_STRUCT_SIZE		((LEN_OF_FILEPATH_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD)+2)
#define ML_LIST_STRUCT_SIZE				(MAX_NUM_OF_SONG)	/* this is no checked now */
#define ML_INDEX_STRUCT_SIZE			(1)	/* this is no checked now */
#define ML_VALUE_STRUCT_SIZE			(1)	/* this is no checked now */
#ifdef _FOLDER_BROWSE_
#define ML_DIR_LIST_STRUCT_SIZE         (MAX_NUM_OF_DIR)
#endif  // _FOLDER_BROWSE_
#define	AUDIBLE_STRUCT_SIZE		        ((NUM_OF_AUDIBLE_FILE/NUM_OF_ITEM_IN_ONE_WORD)+1)

#define ML_VOICE_MARKTABLE_STRUCT_SIZE	((NUM_OF_MARK_TABLE_ENTRY*NUM_OF_MARK_TABLE_COL)+(NUM_OF_MARK_TABLE_ENTRY*(LEN_OF_SUB_STRING_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD))+1)	// 949 // +1 for num_of info /* this size refer to musiclib_voice_function.c */
#define ML_VOICE_INFO_STRUCT_SIZE		((MAX_NUM_OF_VOICE/NUM_OF_ITEM_IN_ONE_WORD)+2)	// 502 /* this size refer to musiclib_voice_function.c */

#define ARTIST_SORTLOOP			(0)	/* this is the loop id of gp_count variable, _do_not_change_it_ */
#define ALBUM_SORTLOOP			(1)	/* this is the loop id of gp_count variable, _do_not_change_it_ */
#define GENRE_SORTLOOP			(2)	/* this is the loop id of gp_count variable, _do_not_change_it_ */
#define SONG_SORTLOOP			(3)	/* this is the loop id of gp_count variable, _do_not_change_it_ */
#define YEAR_SORTLOOP			(4)	/* this is the loop id of gp_count variable, _do_not_change_it_ */
#define TRACK_SORTLOOP			(5)	/* this is the loop id of gp_count variable, _do_not_change_it_ */
#ifdef _FOLDER_BROWSE_
#define DIR_SORTLOOP            (6)
#endif  // _FOLDER_BROWSE_

#define VOID_INDEX		0x00FFFFL
#define UNKNOWN_INDEX   0xffffff

#define LEN_OF_ID3V1_GENRE		(0xF)	/* max len of genre in table, _may_not_have_terminator_ */

#define NumOfLevels 4

#define		ON_THE_GO_TEST			0
#define		ON_THE_GO_ADD			1
#define		ON_THE_GO_DEL			2
#define		ON_THE_GO_DEL_ALL		3

#ifdef _FOLDER_BROWSE_
#define ML_MAX_LEVEL    _MAX_DIR_DEPTH
#else
#define ML_MAX_LEVEL    3
#endif  // _FOLDER_BROWSE_

/*==================================================================================================
                                     STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/* struct to store the sorting information in ram during
construction of music library */
typedef struct _ram_marktable_info {
    /* mark table: used to enhance the searching / inserting performance by locating
    the search range within two nodes appeared in the mark table*/
    uint24 mark_table_index[NUM_OF_MARK_TABLE_ENTRY][NUM_OF_MARK_TABLE_COL];
    uint24 mark_table_string[NUM_OF_MARK_TABLE_ENTRY][LEN_OF_SUB_STRING_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD];
} RAM_MARKTABLE_INFO_T;


/* if you change this struct define also change #define ON_THE_FLY_STRUCT_SIZE		(NUM_OF_SONG_IN_ON_THE_FLY+1) */
typedef struct _on_the_fly_list {
	/* storage list, it is the list storing songID for NUM_OF_SONG_IN_ON_THE_FLY */
	uint24 store_list[NUM_OF_SONG_IN_ON_THE_FLY];
	uint24 num_of_id;	/* num_of_id _must_ locate at end of store_list position, for flash_function access */
} ON_THE_FLY_LIST_T;

/* struct to store the group information: artist,album and genre,in ram during
construction of music library */
typedef struct _ram_group_info {
    /* sorted list,it is the sorted link list with entry in each cell is the next
    item¡¦s ID*/
    uint24 sorted_list[MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD];
    uint24 sorted_list_hd;
    /* associate list,for song group,it is the track number associated with that song
    ID For other group,it is the group ID associated with that song ID*/
    uint24 associate_list[MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD];
} RAM_GROUP_INFO_T;

#ifdef _FOLDER_BROWSE_
typedef struct _ml_DirAssocList {
    /* sorted list,it is the sorted link list with entry in each cell is the next
    item¡¦s ID*/
    uint24  u24SortedList[MAX_NUM_OF_DIR/NUM_OF_ITEM_IN_ONE_WORD];
    uint24  u24SortedListHd;
    /* associate list,for song group,it is the track number associated with that song
    ID For other group,it is the group ID associated with that song ID*/
    uint24  u24AssociateList[MAX_NUM_OF_DIR/NUM_OF_ITEM_IN_ONE_WORD];
} ml_DirAssocList_t;
#endif  // _FOLDER_BROWSE_

/*==================================================================================================
                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
/* RAM */
extern uint24 _X    num_of_item[MAX_NUM_OF_GROUP];
extern unsigned int g_rsrc_TimeDate_CodeBank;
extern INT  g_file_time;
extern uint24   g_is_SD_inserted;
extern RAM_SONG_INFO_T  song_info;
extern unsigned int g_rsrc_Flash_Error;
extern _packed BYTE g_strMusicLib_FilePath[];
extern _packed BYTE g_strMusicLib_SecTable_FilePath[];
extern int  g_iFileHandle;
extern BOOL g_SortByTracknum;   // set to FALSE for maybe 1-2 seconds faster player boot. TRUE to show album songs in album order rather than alpha sorted in album gui.
extern BYTE _X  genre_table[][LEN_OF_ID3V1_GENRE];
extern RAM_MARKTABLE_INFO_T _X  ram_marktable[MAX_NUM_OF_GROUP];
extern RAM_GROUP_INFO_T         ram_group[MAX_NUM_OF_GROUP];
extern uint24 _X                ram_song_year[MAX_NUM_OF_SONG];
extern RAM_GROUP_INFO_T _X      g_tParentDirOfFileAssocList;
extern RAM_MARKTABLE_INFO_T _X  g_tDirMarktable;

/*================================================================================================*/
/*==================================================================================================
                                        FUNCTION PROTOTYPES
==================================================================================================*/
/*==================================================================================================
FUNCTION: utility macro functions for:
		1) check string terminator
		2) list array operation

DESCRIPTION:
		1) check string terminator
		2) list array operation

ARGUMENTS PASSED:

RETURN VALUE:

PRE-CONDITIONS:
   None

POST-CONDITIONS:
   None

IMPORTANT NOTES:
   None

==================================================================================================*/
/*================================================================================================*/
#ifdef __cplusplus
}
#endif
#endif  /* MUSICLIB_HDR_H */
