/*==================================================================================================

                                         MODULE DESCRIPTION

====================================================================================================
                               Perception Digital Confidential Proprietary
                               (c) Copyright PD 2004, All Rights Reserved

MODULE NAME: mf_sf_module_name.c   AUTHOR: Developer Name   CREATION DATE:DD/MM/YYYY

PRODUCT NAMES: All

GENERAL DESCRIPTION:

   General description of this grouping of functions.

Document Reference
----------------------------------------
Music Library Software Design Specification
Version : 01.01
Date : 2-12-2004


Problem Reporting Instructions
----------------------------------------
Problems or corrections to this document must be reported by mailing to
darwinso@perceptiondigital.com with subject format:
[MUSICLIB]<space><title>


Subsystem/Module
----------------------------------------
Firmware player part


Platforms Requiring This Change/Design
----------------------------------------
SigmaTel MP3 flash-based player


Initial Model(s) Requiring This Change/Design:
----------------------------------------
1)	Date : 2-12-2004		PD-800


Ground-Up Designs
----------------------------------------
The music library building engine is the outermost child function. Therefore
the calling tree is in the following structure.

called by UI
|
+---AddEntryToLibrary() {
|		build the music library
|	}
|
+---FlushLibraryToFlash() {
		flush the list to the flash
	}


Source location
----------------------------------------
All files are in directory:
	<Project_Directory>\system\msgmodules\software\MusicLib_Player\src\musiclib_build_function.c


Portability: All
Note:	1) For porting to other platform look for comment with " 24 bits dependent ",
		the code following may require changes.
		2) "#define _24_bit_word_" define 24bit word constants.
		3) Macro marked "_24_bit_word_" may require changes.

Sorting
-----------------------------------------
1) All sorting in capital letter ascii, all small letters are convert to capital on sorting.


Revision History:

                         Modification        Tracking
Author                       Date             Number           Description of Changes
---------------------    ------------        ----------        -------------------------------------
Developer Name            DD/MM/YYYY         PDaaaxxxxx        brief description of changes made


Notes:

This version use static ram as temp_name_buf[], compilable on tasking.

====================================================================================================
                                            DESCRIPTION
====================================================================================================

GLOBAL FUNCTIONS:
   MF_global_func_name()

called by UI, flush sorted and associate list to flash when no more song needed to be added
COMP_OPT void FlushLibraryToFlash(void) {
	for each group X in (artist,album,genre) {
		flush ram_song_group.sorted_list to flash_song_group.sorted_list flash;
		flush ram_X_group.sorted_list to flash_X_group.sorted_list;
		do a counting on ram_X_group.associated_list to check to number of child and song ID associated with that group;
		flush the counted result to flash_X_group.associated_list;
		flush num_of_song to flash_num_of_song;
		flush num_of_artist to flash_num_of_artist;
		flush num_of_album to flash_num_of_album;
		flush num_of_genre to flash_num_of_genre;
	}
}

Item: The global variable, int24 *g_wSongTitle, int24 *g_wSongArtist, int24
*g_wSongAlbum, int24 *g_wSongGenre, int24 *g_wSongYear, int24 *g_wSongPath
and int g_Unicode are grouped into one struct RAM_SONG_INFO_T, see section
5.4.1
New function is added to flush the music library to flash after all the
songs are added.

COMP_OPT void AddEntryToLibrary(RAM_SONG_INFO_T *song_info, int16 option) {
	num_of_song++;
	flush song_info.song_name to flash_song_name[Media_type][num_of_song].name;
	flush song_info.g_unicode to flash_song_name[Media_type][num_of_song].g_unicode;
	flush song_info.path_name to flash_path_name[num_of_song];
	func(sort the song name) {
		locate the start and end index in the ram_song_group.mark_table_index
		using binary search in the mark table, by comparing the new song name with
		ram_song_group.mark_table_string;

		load the song names from flash indicated by the start and end index to
		temp_name_buf in sorted order;

		start binary search in temp_name_buf to locate the exact position for
		putting the new song ID (ie, num_of_song) in;
		insert the new song ID (ie, num_of_song) into ram_song_group.sorted_list
		in proper location;

		insert the new track number to ram_song_group.associate_list[num_of_song];

		func(update the mark table) {
			increase the distance of specific mark entry;

			if the distance is larger than threshold, split the entries in the
			mark table into half;
		}
	}

	for each group X in (artist,album,genre) {
		locate the start and end index in the ram_X_group.mark_table_index using a
		binary search in the mark table, by comparing the new group X name with
		ram_X_group.mark_table_string;

		load the group X names from flash indicated by the start and end index to temp_name_buf;

		start binary search in ram_X_group.sorted_list to locate the exact
		position for putting the group ID in;

		if the group ID does not exist before {
			num_of_X++;

			insert the group ID (num_of_X) into ram_X_group.sorted_list in proper
			location if it does not exist before;

			flush song_info.X_name to flash_X_name[num_of_X];

			flush song_info.g_unicode to flash_X_name[num_of_song].g_unicode;

			func(update the mark table) {
				increase the distance of specific mark entry;

				if the distance is larger than threshold, split the entries in the
				mark table into half.
			}
		}
		for artist group, the associate list contains the artist ID;
		for album group, the associate list contains the album ID;
		for genre group, the associate list contains the genre ID

		insert the new or existing group ID into ram_X_group.associated_list[num_of_song];
	}
}




LOCAL  FUNCTIONS:
   mf_local_func_name()

====================================================================================================


TRACEABILITY MATRIX:
   None

==================================================================================================*/

/*==================================================================================================
                                           INCLUDE FILES
==================================================================================================*/
#ifdef WIN32
#define _PC_SIMULATION_
#endif

#ifndef _PC_SIMULATION_
#define _RUNNING_IN_EMBEDDED_
#endif

#ifdef	_RUNNING_IN_EMBEDDED_
#define COMP_OPT static _reentrant
#define COMP_OPT_G
#else
#define COMP_OPT
#endif

#pragma optimize 1
#include	"..\hdr\musiclib_hdr.h"
#include ".\musiclib_basic_function.c"

#define _USEFLASH_WRITE_	/* only the build functions need to use ML_MemWrite() */

#ifdef _USE_FS_SECTAB_
#include "..\..\..\..\inc\fserror.h"
#include "..\..\..\..\filesystem\fat32\h\extern.h"
#endif

#include ".\musiclib_flash_function.c"

/*==================================================================================================
								Conditional Compilation Directives
==================================================================================================*/
#ifdef _PC_SIMULATION_

#if 1
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

#else
#include "resource.h"
#include "menumanager.h"
#endif

/*==================================================================================================
                                         LOCAL CONSTANTS
==================================================================================================*/
#if 0
/* caution: there is no terminator in this case, it's up to the UI to handle end of string */
#define _ADD_SPACE_TO_NAME_		/* for demo only:pack space to end of song name string */
#else
#if 1
/*
disabled this if add tailing space for display effect => skip copy terminator,
because you lose performence if you search non-null-terminated-string for terminator,
but no error.
 */
#define _SAVENAMESHORTCUT_		/* optimise: quit copy on '\0' when short name encounter*/
#endif
#endif

#if 1
#define _SKIP_PERPOSITION_
#endif

#if 1
#define _SORTBY_CAPITAL_
#endif

#if 1
#define _USE_SMALLEND_	/* you _must_use_ this ! since the big endien code is not fullly implemented */
#endif


#define	LEN_OF_FLASH_NAME_BUF	(LEN_OF_NAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD)
#define	LEN_OF_MTSTRING			(LEN_OF_SUB_STRING_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD)
#define	AUDIBLE_STRUCT_SIZE		((NUM_OF_AUDIBLE_FILE/NUM_OF_ITEM_IN_ONE_WORD)+1)

/*==================================================================================================
                                         LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
                                         LOCAL ENUMS
==================================================================================================*/
#define	MT_NAMEID_COL			(0)
#define	MT_COUNT_COL			(1)

#define	ASSOC_COUNT_LIST		(0)
#define	ASSOC_CID_LIST			(1)

#define	TEMP_LIST_PREID_COL		(0)
#define	TEMP_LIST_THISID_COL	(1)
#define	TEMP_LIST_NXID_COL		(2)

//#define	INDEX_EOF	0xFFF

#define UINT24_STR_CMPTYPE		(0)
#define UINT24_VAL_CMPTYPE		(1)
#define UINT24_MARKTAB_CMPTYPE	(2)
#define UINT24_PATH_CMPTYPE		(3)
#define ITEMCMP_ITEM01_EQUAL	(0x00)
#define ITEMCMP_ITEM0_SMALL		(0x00FFFF)
#define ITEMCMP_ITEM0_LARGE		(0x01)

#define	NUM_OF_ID3GENRE			(0x50)	/* 80 */

#ifdef _USE_FS_SECTAB_
#define SECTOR_ADD_SUCCESS				0
#define SECTOR_ADD_ERROR_FSEEK			-1
#define SECTOR_ADD_ERROR_READ_MODE		-2
#define SECTOR_ADD_ERROR_FILE_SIZE		-3
#define SECTOR_ADD_ERROR_OUTPUT			-4
#define SECTOR_ADD_ERROR_INCOMPLETE		-5
#endif

#define ONTHEGO_SDCHK					0x2

/*==================================================================================================
                                     LOCAL STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/* struct to store the sorting information in ram during
construction of music library */
typedef struct {
/* mark table: used to enhance the searching / inserting performance by locating
the search range within two nodes appeared in the mark table*/
	uint24 mark_table_index[NUM_OF_MARK_TABLE_ENTRY][NUM_OF_MARK_TABLE_COL];
	uint24 mark_table_string[NUM_OF_MARK_TABLE_ENTRY][LEN_OF_SUB_STRING_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD];
//	uint24 mark_table_index[49][2];
//	uint24 mark_table_string[49][LEN_OF_SUB_STRING_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD];
} RAM_MARKTABLE_INFO_T;

/* struct to store the group information: artist, album and genre, in ram during
construction of music library */
typedef struct {
/* sorted list, it is the sorted link list with entry in each cell is the next
item¡¦s ID*/
	uint24 sorted_list[MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD];
	uint24 sorted_list_hd;
/* associate list, for song group, it is the track number associated with that song
ID For other group, it is the group ID associated with that song ID*/
	uint24 associate_list[MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD];
} RAM_GROUP_INFO_T;

#ifdef _NEWMUSIC_
typedef struct {
	/* the content of i th entry is the modified date of song with sorted_list[i]=songID */
	uint24 song_date[NUM_OF_SONG_IN_NEW_MUSIC];	/* song_date[] and sorted_list[] _must_be_following_each_other_, for flash_function.c */
	/* sorted list, it is the sorted list in decending order of modified date, with entry in each cell is the songID */
	uint24 sorted_list[NUM_OF_SONG_IN_NEW_MUSIC];	/* song_date[] and sorted_list[] _must_be_following_each_other_, for flash_function.c */
	uint24 num_of_id;			/* num_of_1X_song in_struct_position is hard-coded */
	/* num_of_X to remember each new music list size */
	uint24 num_of_1day_song;	/* num_of_1X_song in_struct_position is hard-coded */
	uint24 num_of_1week_song;	/* num_of_1X_song in_struct_position is hard-coded */
	uint24 num_of_1month_song;	/* num_of_1X_song in_struct_position is hard-coded */
} NEW_MUSIC_LIST_T;
#endif

#ifdef _AUDIBLE_
/* if you change this struct define also change #define AUDIBLE_STRUCT_SIZE		((NUM_OF_AUDIBLE_FILE/NUM_OF_ITEM_IN_ONE_WORD)+1) */
typedef struct {
	/* storage list, it is the list storing songID for audible */
	uint24 store_list[NUM_OF_AUDIBLE_FILE/NUM_OF_ITEM_IN_ONE_WORD];
	uint24 num_of_id;	/* num_of_id _must_ locate at end of store_list position, for flash_function access */
} AUDIBLE_LIST_T;
#endif

/*==================================================================================================
                                     LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
                                          LOCAL VARIABLES
==================================================================================================*/
/* RAM */
/* ram_artist_marktable = id0, ram_album_marktable = id1, ram_genre_marktable = id2, id3 reserved for song, ram_year_marktable = id4 */
RAM_MARKTABLE_INFO_T ram_marktable[MAX_NUM_OF_GROUP];
/* each group below has 2 lists: one to store sorted group ID link list (sorted_list),
another one is the child list (associate_list) */
/* ram_artist_marktable = id0, ram_album_marktable = id1, ram_genre_marktable = id2, id3 reserved for song, ram_year_marktable = id4 */
RAM_GROUP_INFO_T ram_group[MAX_NUM_OF_GROUP];
/* song group has 2 lists: one to store sorted song ID link list (sorted list),
another one is the track number associated with that song ID (associate_list) */

//uint24 dummyword[500]	/* 05/02/16 there is 1 browsing bug after mem optimize in cvs 011 version which need this dummy words to work correctly */
/* However version base_011_srs works fine with mem optimize code(don't need this dummyword ! */

/* temporary buffer to store the names for exact position search in the link list */
/* as temp buf for on_the_fly_list path */
uint24 temp_name_buf[LEN_OF_NAME_IN_BYTE];	/* use as general temp string buffer */
uint24 ram_song_track[MAX_NUM_OF_SONG];

uint24	MarkTableID[MAX_NUM_OF_GROUP];	/* include song_marktable */
uint24	temp_list_buf[DISTANCE_IN_MARK_TABLE][3];
uint24	insert_name[LEN_OF_NAME_IN_BYTE];	/* as temp buf for on_the_fly_list path */
uint24	group_dtype[MAX_NUM_OF_GROUP];
#ifdef _NEWMUSIC_
NEW_MUSIC_LIST_T ram_newmusic;
NEW_MUSIC_LIST_T ram_flash_newmusic;
NEW_MUSIC_LIST_T ram_sd_newmusic;
uint24 cutout_mm;	/* YYMMDD:12 bits - Year, 6 bits - Month, 6 - bits Date */
uint24 cutout_ww;	/* YYMMDD:12 bits - Year, 6 bits - Month, 6 - bits Date */
uint24 cutout_dd;	/* YYMMDD:12 bits - Year, 6 bits - Month, 6 - bits Date */
#endif

#ifdef _AUDIBLE_
AUDIBLE_LIST_T	ram_audible;
#endif

/* variables in _X */
FLASH_GROUP_NAME_T _X flash_group_name;
uint24 _X ram_song_year[MAX_NUM_OF_SONG];
uint24 _X flash_path_name[(LEN_OF_FILEPATH_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD)+2];	  //+2 for storing fast key/
uint24 _X temp_queue[(MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD)+1];	/* +1 for share the temp buf with audible struct */
uint24 _X temp_associated[NUM_OF_LIST_IN_CHILD_ARRAY][MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD];
uint24 _X artist_subassociated[NUM_OF_LIST_IN_CHILD_ARRAY][MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD];
uint24 _X artist_associated_count[MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD];
uint24 _X album_associated[NUM_OF_LIST_IN_CHILD_ARRAY][MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD];
uint24 _X temp_song_info_name[LEN_OF_FLASH_NAME_BUF];
uint24 _X OnTheGo_path[NUM_OF_SONG_IN_ON_THE_FLY][LEN_OF_FILEPATH_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD];

#ifdef _USE_FS_SECTAB_	/* define in flash_function.c 05/03/01 use fs access to store musiclib instead of direct sector access */
extern LONG _X l2_sector_table[LEVEL2_TABLE_SIZE];
extern LONG SectorSize;
extern uint24 num_long_per_sec;
#endif
extern ON_THE_FLY_LIST_T _X on_the_fly_list;
ON_THE_FLY_LIST_T on_the_fly_temp_list_Y;

#ifdef _PC_DEBUGPRINT_
uint24	w;		/* debug use */
uint24	y;		/* debug use */
#endif

extern BYTE _X genre_table[][LEN_OF_ID3V1_GENRE];
extern unsigned int g_rsrc_TimeDate_CodeBank;

/*==================================================================================================
                                          GLOBAL VARIABLES
==================================================================================================*/
extern int g_file_time;
extern BOOL g_SortByTracknum;
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
extern _reentrant void UpdateDateValues(int a, int b,int* iDateArray[]);
extern uint24 g_is_SD_inserted;

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
COMP_OPT uint24 ChkTermInWord(uint24 myword) {
	return (((myword & 0xFF0000) == 0x00) || ((myword & 0x00FF00) == 0x00) || ((myword & 0x0000FF) == 0x00));
}

COMP_OPT void ML_cpy_flash_group_name(FLASH_GROUP_NAME_T _X *dbuf, FLASH_GROUP_NAME_T *sbuf)
{
	uint24 i;

	for (i=0;i<LEN_OF_NAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD;i++) {
		dbuf->name[i] = sbuf->name[i];
	}
	dbuf->unicode = sbuf->unicode;
}

/*================================================================================================*/
COMP_OPT void IncWordVal(uint24 _X *uint24_p, uint24 pos, uint24 val) {
	macro_temp_var = pos/NUM_OF_ITEM_IN_ONE_WORD;
	if ((pos & 0x01) == 0x01) {
		uint24_p[macro_temp_var] = ((uint24_p[macro_temp_var] & 0xFFF000) | ((uint24_p[macro_temp_var] & 0x000FFF) + (val & 0x000FFF)));
	} else {
		uint24_p[macro_temp_var] = (((((uint24_p[macro_temp_var] & 0xFFF000)>>12) + (val & 0x000FFF))<<12) | (uint24_p[macro_temp_var] & 0x000FFF));
	}
}

/*================================================================================================*/
COMP_OPT void ML_list_get_val(uint24 *uint24_p, uint24 pos, uint24 *val) {
	*val = uint24_p[(macro_temp_var = pos/NUM_OF_ITEM_IN_ONE_WORD)];
	if ((pos & 0x01) == 0x01) {
		*val = (*val & 0x000FFF);
	} else {
		*val = ((*val & 0xFFF000)>>12);
	}
}

/*================================================================================================*/
COMP_OPT void ML_list_put_val_X(uint24 _X *uint24_p, uint24 pos, uint24 val) {
	macro_temp_var = pos/NUM_OF_ITEM_IN_ONE_WORD;
	if ((pos & 0x01) == 0x01) {
		uint24_p[macro_temp_var] = ((uint24_p[macro_temp_var] & 0xFFF000) | (val & 0x000FFF));
	} else {
		uint24_p[macro_temp_var] = (((val & 0x000FFF)<<12) | (uint24_p[macro_temp_var] & 0x000FFF));
	}
}

/*================================================================================================*/
COMP_OPT void ML_list_put_val(uint24 *uint24_p, uint24 pos, uint24 val) {
	macro_temp_var = pos/NUM_OF_ITEM_IN_ONE_WORD;
	if ((pos & 0x01) == 0x01) {
		uint24_p[macro_temp_var] = ((uint24_p[macro_temp_var] & 0xFFF000) | (val & 0x000FFF));
	} else {
		uint24_p[macro_temp_var] = (((val & 0x000FFF)<<12) | (uint24_p[macro_temp_var] & 0x000FFF));
	}
}

#ifdef _NEWMUSIC_
/*================================================================================================*/
COMP_OPT void GetCutoutTime(void) {
	int rtc_yy;
	int rtc_mm;
	int rtc_dd;

#ifdef _PC_SIMULATION_
	rtc_yy = 2005;
	rtc_mm = 3;
	rtc_dd = 20;
	// get rtc time
#else
	int24* iDateArray[3];

	iDateArray[0] = &rtc_mm;
	iDateArray[1] = &rtc_dd;
	iDateArray[2] = &rtc_yy;

	SysCallFunction(g_rsrc_TimeDate_CodeBank,UpdateDateValues,0,0,(int*) &iDateArray);
	rtc_yy = 2000+rtc_yy;
#endif

	if (rtc_dd > 1) {
		cutout_dd = ((rtc_yy & 0x000FFF) << 12) | ((rtc_mm & 0x00003F) << 6) | ((rtc_dd-1) & 0x00003F);
	} else {
		if (rtc_mm == 0x1) {
			/* sort as month 13 */
			cutout_dd = (((rtc_yy-1) & 0x000FFF) << 12) | ((0xC) << 6);
		} else {
			cutout_dd = ((rtc_yy & 0x000FFF) << 12) | (((rtc_mm-1) & 0x00003F) << 6);
		}
		cutout_dd |= (0x1F & 0x00003F);
	}

	if (rtc_dd < 0x7) {
		if (rtc_mm == 0x1) {
			/* sort as month 13 */
			cutout_ww = (((rtc_yy-1) & 0x000FFF) << 12) | ((0xC) << 6);
		} else {
			cutout_ww = ((rtc_yy & 0x000FFF) << 12) | (((rtc_mm-1) & 0x00003F) << 6);
		}
		cutout_ww |= ((0x18 + rtc_dd) & 0x00003F);	/* 0x1F - 0x7 => 0x18 */
	} else {
		cutout_ww = ((rtc_yy & 0x000FFF) << 12) | ((rtc_mm & 0x00003F) << 6) | ((rtc_dd - 0x7) & 0x00003F);
	}

	if (rtc_mm == 0x1) {
		/* sort as month 13 */
		cutout_mm = (((rtc_yy-1) & 0x000FFF) << 12) | ((0xC) << 6) | (rtc_dd & 0x00003F);
	} else {
		cutout_mm = ((rtc_yy & 0x000FFF) << 12) | (((rtc_mm-1) & 0x00003F) << 6) | (rtc_dd & 0x00003F);
	}
}
#endif

/*==================================================================================================
FUNCTION: FlushLibraryToFlash()

DESCRIPTION:
Description of this specific function.

Called by UI, flush sorted and associate list to flash when no more song needed to be added

ARGUMENTS PASSED:
None

RETURN VALUE:
COMP_OPT

PRE-CONDITIONS:
Bulid the library first (call AddEntryToLibrary) before calling this function.

POST-CONDITIONS:
None

IMPORTANT NOTES:
None

==================================================================================================*/
void ML_FlushLibraryToFlash(uint24 Media_type) {
	uint24	i;
	uint24	j;
	uint24	k;
	uint24	m;
	uint24 gp_count;
	uint24 listnxpos;
	uint24 listsize;

	uint24	temp_w;
	uint24	temp_w2;
	uint24	*read_p_Y;
    uint24 iAlbum_ID;
    uint24 l;
    uint24 n;

#ifdef _PC_DEBUGPRINT_
	printf("\nEnter ML_FlushLibraryToFlash().\n");
#endif

	/*
	flush ram_song_group.sorted_list to flash_song_array flash;
	flush num_of_song to flash_num_of_song;
	for each group X in (album,genre, year) {
	flush ram_X_group.sorted_list to flash_X_group.sorted_list;
	do a counting on ram_X_group.associated_list to check to number of child and song ID associated with that group;
	flush the counted result to flash_X_group.associated_list;
	flush num_of_album to flash_num_of_album;
	flush num_of_genre to flash_num_of_genre;
	}
	*/

	/* code for artist, album, genre, song, year, track */
	for (gp_count=0;gp_count<MAX_NUM_OF_GROUP;gp_count++) {
//		if (gp_count != TRACK_SORTLOOP) {	/* skip the track slot, for it's not stored at this moment */
#if 0
			for (i=0;i<(MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD);i++) {
				temp_queue[i] = 0x00;
			}
#endif
			j = ram_group[gp_count].sorted_list_hd;
			for (i=0;i<num_of_item[gp_count];i++) {
				ML_list_put_val_X(temp_queue, i, j);
				ML_list_get_val(ram_group[gp_count].sorted_list, j, &k);
				j = k;
			}
			switch (gp_count) {
				case ARTIST_SORTLOOP:
					ML_MemWrite(Media_type, ARTIST_SORT_DTYPE, num_of_item[SONG_SORTLOOP], temp_queue, ML_LIST_STRUCT_SIZE);
					break;
				case ALBUM_SORTLOOP:
					ML_MemWrite(Media_type, ALBUM_SORT_DTYPE, num_of_item[SONG_SORTLOOP], temp_queue, ML_LIST_STRUCT_SIZE);
					break;
				case GENRE_SORTLOOP:
					ML_MemWrite(Media_type, GENRE_SORT_DTYPE, num_of_item[SONG_SORTLOOP], temp_queue, ML_LIST_STRUCT_SIZE);
					break;
				case SONG_SORTLOOP:
					ML_MemWrite(Media_type, SONG_ARRAY_DTYPE, num_of_item[SONG_SORTLOOP], temp_queue, ML_LIST_STRUCT_SIZE);
					ML_MemWrite(Media_type, NUM_OF_SONG_DTYPE, num_of_item[SONG_SORTLOOP], temp_queue, ML_INDEX_STRUCT_SIZE);	/* num_of_song, temp_queue is dummy */
					break;
				case YEAR_SORTLOOP:
					ML_MemWrite(Media_type, YEAR_SORT_DTYPE, num_of_item[SONG_SORTLOOP], temp_queue, ML_LIST_STRUCT_SIZE);
					break;
			}
// 		}
		if (gp_count != SONG_SORTLOOP) {
#if 0
			for (i=0;i<(MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD);i++) {
				temp_queue[i] = 0x00;
				temp_associated[ASSOC_COUNT_LIST][i] = 0x00;
				temp_associated[ASSOC_CID_LIST][i] = 0x00;
				artist_subassociated[ASSOC_CID_LIST][i] = 0x00;
				artist_subassociated[ASSOC_CID_LIST][i] = 0x00;
			}
#endif
			/* optimized, (condition:MAX(num_of_item[gp_count]) _must_be_divisiable by NUM_OF_ITEM_IN_ONE_WORD) caution for memory overwrite */
			for (i=0;i<((num_of_item[gp_count]+1)/NUM_OF_ITEM_IN_ONE_WORD);i++) {
				temp_associated[ASSOC_COUNT_LIST][i] = 0x000000;
			}
			for (i=0;i<num_of_item[SONG_SORTLOOP];i++) {
				ML_list_get_val(ram_group[gp_count].associate_list, i, &j);
				IncWordVal(temp_associated[ASSOC_COUNT_LIST], j, 0x01);
			}
			ML_list_get_val_X(temp_associated[ASSOC_COUNT_LIST], 0, &j);
			for (i=1;i<num_of_item[gp_count];i++) {
				IncWordVal(temp_associated[ASSOC_COUNT_LIST], i, j);
				ML_list_get_val_X(temp_associated[ASSOC_COUNT_LIST], i, &j);
			}

			ML_list_put_val_X(temp_queue, 0, 0x0);
			j = 0;
			for (i=1;i<num_of_item[gp_count];i++) {
				ML_list_get_val_X(temp_associated[ASSOC_COUNT_LIST], j, &k);
				ML_list_put_val_X(temp_queue, i, k);
				j++;
			}
			/* temp_queue=>head position for insert element */

			j = ram_group[SONG_SORTLOOP].sorted_list_hd;
			for (i=0;i<num_of_item[SONG_SORTLOOP];i++) {
				/*
				temp_associated[ASSOC_CID_LIST][temp_queue[ram_group[gp_count].associate_list[j]]] = j;
				*/
				ML_list_get_val(ram_group[gp_count].associate_list, j, &k);
				ML_list_get_val_X(temp_queue, k, &temp_w);
				ML_list_put_val_X(temp_associated[ASSOC_CID_LIST], temp_w, j);
				IncWordVal(temp_queue, k, 0x01);
				ML_list_get_val(ram_group[SONG_SORTLOOP].sorted_list, j, &k);
				j = k;
			}

// Album associated_list inherits the sequence in SONG sort_list which is sorted by song name.
// Re-sort Album associated_list by track number here.

    if (g_SortByTracknum && (gp_count == ALBUM_SORTLOOP))
    {
        ML_list_put_val_X(temp_queue, 0x00, 0x00);

	    for (iAlbum_ID=1; iAlbum_ID<num_of_item[ALBUM_SORTLOOP]; iAlbum_ID++) {
		    ML_list_get_val_X(temp_associated[ASSOC_COUNT_LIST], (iAlbum_ID-1), &temp_w);
		    ML_list_put_val_X(temp_queue, iAlbum_ID, temp_w);
	    }

	    for (iAlbum_ID=0; iAlbum_ID<num_of_item[ALBUM_SORTLOOP]; iAlbum_ID++) {
	        ML_list_get_val_X(temp_queue, iAlbum_ID, &temp_w);                          // temp_w = start_offset
	        ML_list_get_val_X(temp_associated[ASSOC_COUNT_LIST], iAlbum_ID, &temp_w2);  // temp_w2 = end_offset

            for(i=temp_w2-1; i>temp_w; i--) {
                for(j=temp_w;j<i;j++) {
                    ML_list_get_val_X(temp_associated[ASSOC_CID_LIST], j, &k);
                    ML_list_get_val(ram_group[SONG_SORTLOOP].associate_list, k, &m);    // get track number of song k
                    ML_list_get_val_X(temp_associated[ASSOC_CID_LIST], j+1, &l);
                    ML_list_get_val(ram_group[SONG_SORTLOOP].associate_list, l, &n);    // get track number of song l

                    if (m>n) {
                        ML_list_put_val_X(temp_associated[ASSOC_CID_LIST], j, l);
                        ML_list_put_val_X(temp_associated[ASSOC_CID_LIST], j+1, k);
                    }
                }
            }
	    }
    }

			switch (gp_count) {
				case ARTIST_SORTLOOP:
					/*
					// use this for artist->song display mode
					ML_MemWrite(Media_type, ARTIST_ASSOC_COUNT_DTYPE, num_of_item[SONG_SORTLOOP], &temp_associated, ML_LIST_STRUCT_SIZE);
					*/
					for (i=0;i<(MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD);i++) {
						artist_associated_count[i] = temp_associated[ASSOC_COUNT_LIST][i];
					}
					break;
				case ALBUM_SORTLOOP:
					ML_MemWrite(Media_type, ALBUM_ASSOC_COUNT_DTYPE, num_of_item[SONG_SORTLOOP], (uint24 _X *)&temp_associated, ML_LIST_STRUCT_SIZE);
					/* prepare for artist tree building */
					for (i=0;i<(MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD);i++) {
						album_associated[ASSOC_COUNT_LIST][i] = temp_associated[ASSOC_COUNT_LIST][i];
						album_associated[ASSOC_CID_LIST][i] = temp_associated[ASSOC_CID_LIST][i];
					}
//					if (g_SortByTracknum) {
//					    for (i=0;i<(MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD);i++) {
//                            album_associated[ASSOC_CID_LIST][i] = temp_associated[ASSOC_CID_LIST][i];
//					    }
//				    }
					break;
				case GENRE_SORTLOOP:
					ML_MemWrite(Media_type, GENRE_ASSOC_COUNT_DTYPE, num_of_item[SONG_SORTLOOP], (uint24 _X *)&temp_associated, ML_LIST_STRUCT_SIZE);
					break;
				case YEAR_SORTLOOP:
					/* for year */
					ML_MemWrite(Media_type, YEAR_ASSOC_COUNT_DTYPE, num_of_item[SONG_SORTLOOP], (uint24 _X *)&temp_associated, ML_LIST_STRUCT_SIZE);
					ML_MemWrite(Media_type, SONG_YEAR_DTYPE, num_of_item[SONG_SORTLOOP], ram_song_year, ML_VALUE_STRUCT_SIZE);
					break;
			}
		}	/* if (gp_count != SONG_SORTLOOP) */
	}	/* for (gp_count=0;gp_count<MAX_NUM_OF_GROUP;gp_count++) */

#ifdef _PC_DEBUGPRINT_
	printf("\nnum_of_track=%d", num_of_item[5]);
	printf("\nSorted ram_song_track(compared string)=");
	i =ram_group[5].sorted_list_hd;
	while (i != INDEX_EOF) {
		printf("\nID=%d\t", i);
		printf("[%d]", ram_song_track[i]);
		ML_list_get_val(ram_group[5].sorted_list, i, &j);
		i = j;
	}

	printf("\ncount:");
	for (i=0;i<num_of_item[5];i++) {
		ML_list_get_val_X(temp_associated[ASSOC_COUNT_LIST], i, &j);
		printf("[%d]", j);
	}
	printf("\nsongID:");
	for (i=0;i<num_of_item[SONG_SORTLOOP];i++) {
		ML_list_get_val_X(temp_associated[ASSOC_CID_LIST], i, &j);
		printf("[%d]", j);
	}
#endif

    if (g_SortByTracknum) {
	    for (i=0;i<num_of_item[SONG_SORTLOOP];i++) {
	        ML_list_get_val(ram_group[SONG_SORTLOOP].associate_list, i, &j);
		    ML_list_put_val_X(temp_queue, i, j);
	    }
	    ML_MemWrite(Media_type, TRACKNUM_ARRAY_DTYPE, num_of_item[SONG_SORTLOOP], temp_queue, ML_LIST_STRUCT_SIZE);
    }

	/*
	for group artist {
	flush ram_artist_group.sorted_list to flash_artist_group.sorted_list;
	do a counting on ram_artist_group.associated_list to check the number of child and song ID associated with artist;
	base on the song ID, do a counting on ram_album_group.associated_list to select album ID and ram_album_group.sequence_list to sort the album ID.
	base on the song ID, do a counting on ram_song_group.associated_list to select the track ID, and sort them ascending order.
	flush the counted/sorted result to flash_artist_group.associated_list and flash_artist_group.sub_associated_list;
	flush num_of_artist to flash_num_of_artist;
	}
	*/
	/* code for artist */
	/*
	*** Caution:
	you _must_have_ :
	1) the flash_track_associated_list content in the temp_associated array _and_
	2) the flash_album_associated_list[ASSOC_COUNT_LIST] content
	in the album_associated array album_associated[ASSOC_COUNT_LIST] location _and_
	3) the flash_artist_associated_list[ASSOC_COUNT_LIST] content
	in the artist_associated_count array
	to start building artist tree.
	*/
	/* build sorted songs according to track group by album */
#if 0
    if (g_SortByTracknum == FALSE)
    {
	ML_list_put_val_X(temp_queue, 0x00, 0x00);
	for (i=1;i<num_of_item[ALBUM_SORTLOOP];i++) {	/* i = albumID */
		ML_list_get_val_X(album_associated[ASSOC_COUNT_LIST], (i-1), &temp_w2);
		ML_list_put_val_X(temp_queue, i, temp_w2);
	}
	/* temp_queue=>head position for insert element */

	m = ram_group[TRACK_SORTLOOP].sorted_list_hd;	/* sorted trackID */
	for (i=0;i<num_of_item[TRACK_SORTLOOP];i++) {	/* i = trackID */
		ML_list_get_val_X(temp_associated[ASSOC_COUNT_LIST], m, &listsize);
#ifdef _ASSERT_
		assert(listsize < MAX_NUM_OF_SONG);
#endif
		if (m > 0) {
			ML_list_get_val_X(temp_associated[ASSOC_COUNT_LIST], (m-1), &temp_w2);	/* temp_w2 = pos for start of songID group by track, keep it */
			listsize -= temp_w2;
		} else {
			temp_w2 = 0;	/* temp_w2 = pos for start of songID group by track, keep it */
		}
		do {
			ML_list_get_val_X(temp_associated[ASSOC_CID_LIST], temp_w2, &temp_w);	/* temp_w = songID, keep it for reference */

			ML_list_get_val(ram_group[ALBUM_SORTLOOP].associate_list, temp_w, &j);	/* j = albumID */
			ML_list_get_val_X(temp_queue, j, &k);	/* k = pos for start of songID group by album, keep j */

			ML_list_put_val_X(album_associated[ASSOC_CID_LIST], k, temp_w);	/* k = pos for songID */
			IncWordVal(temp_queue, j, 0x01);	/* nx write pos */
			listsize--;
			//if ((--listsize) > 0) {
			temp_w2++;
			//}
		} while (listsize > 0);
		ML_list_get_val(ram_group[TRACK_SORTLOOP].sorted_list, m, &j);	/* sorted trackID */
		m = j;
	}

#ifdef _PC_DEBUGPRINT_
	printf("\nsort track by album:");
	/*
	printf("\nnum_of_track=%d", num_of_item[5]);
	printf("\nSorted ram_song_track(compared string)=");
	i =ram_group[5].sorted_list_hd;
	while (i != INDEX_EOF) {
	printf("\nID=%d\t", i);
	printf("[%d]", ram_song_track[i]);
	ML_list_get_val(ram_group[5].sorted_list, i, j);
	i = j;
	}
	*/
	printf("\ncount:");
	for (i=0;i<num_of_item[2];i++) {
		ML_list_get_val_X(album_associated[ASSOC_COUNT_LIST], i, &j);
		printf("[%d]", j);
	}
	printf("\nsongID:");
	for (i=0;i<num_of_item[SONG_SORTLOOP];i++) {
		ML_list_get_val_X(album_associated[ASSOC_CID_LIST], i, &j);
		printf("[%d]", j);
	}
#endif
    }
#endif
	/* build sorted songID group by artist */
	/*
	for (i=0;i<(MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD);i++) {
	temp_associated[ASSOC_COUNT_LIST][i] = artist_associated_count[i];
	}
	*/
#if 1
	ML_list_put_val_X(temp_queue, 0x00, 0x00);
	for (i=1;i<num_of_item[ARTIST_SORTLOOP];i++) {	/* i = artistD */
		ML_list_get_val_X(artist_associated_count, (i-1), &temp_w2);
		ML_list_put_val_X(temp_queue, i, temp_w2);
	}
	/* temp_queue=>head position for insert element */

	m = ram_group[ALBUM_SORTLOOP].sorted_list_hd;	/* sorted albumID */
	for (i=0;i<num_of_item[ALBUM_SORTLOOP];i++) {	/* i = albumID */
		ML_list_get_val_X(album_associated[ASSOC_COUNT_LIST], m, &listsize);
#ifdef _ASSERT_
		assert(listsize < MAX_NUM_OF_SONG);
#endif
		if (m > 0) {
			ML_list_get_val_X(album_associated[ASSOC_COUNT_LIST], (m-1), &temp_w2);	/* temp_w2 = pos for start of songID group by album, keep it */
			listsize -= temp_w2;
		} else {
			temp_w2 = 0;	/* temp_w2 = pos for start of songID group by track, keep it */
		}
		do {
			ML_list_get_val_X(album_associated[ASSOC_CID_LIST], temp_w2, &temp_w);	/* temp_w = songID, keep it for reference */

			ML_list_get_val(ram_group[ARTIST_SORTLOOP].associate_list, temp_w, &j);	/* j = artistID */
			ML_list_get_val_X(temp_queue, j, &k);	/* k = pos for start of songID group by artist, keep j */

			ML_list_put_val_X(temp_associated[ASSOC_CID_LIST], k, m);	/* k = pos for albumID */
			ML_list_put_val_X(artist_subassociated[ASSOC_CID_LIST], k, temp_w);	/* k = pos for songID */
			IncWordVal(temp_queue, j, 0x01);	/* nx write pos */
			listsize--;
			//if ((--listsize) > 0) {
			temp_w2++;
			//}
		} while (listsize > 0);
		ML_list_get_val(ram_group[ALBUM_SORTLOOP].sorted_list, m, &j);	/* sorted albumID */
		m = j;
	}
#else
	ML_list_put_val_X(temp_queue, 0x00, 0x00);
	for (i=1;i<num_of_item[ARTIST_SORTLOOP];i++) {	/* i = artistD */
		ML_list_get_val_X(artist_associated_count, (i-1), &temp_w2);
		ML_list_put_val_X(temp_queue, i, temp_w2);
	}
	/* temp_queue=>head position for insert element */

	for (i=0;i<num_of_item[SONG_SORTLOOP];i++) {
		ML_list_get_val_X(album_associated[ASSOC_CID_LIST], i, &temp_w);	/* temp_w = songID, keep it for reference */

		ML_list_get_val(ram_group[ARTIST_SORTLOOP].associate_list, temp_w, &j);	/* j = artistID */
		ML_list_get_val_X(temp_queue, j, &k);	/* k = pos for start of songID group by artist, keep j */

		ML_list_put_val_X(artist_subassociated[ASSOC_CID_LIST], k, temp_w);	/* k = pos for songID */
		ML_list_get_val(ram_group[ALBUM_SORTLOOP].associate_list, temp_w, &temp_w2);	/* j = artistID */
		ML_list_put_val_X(temp_associated[ASSOC_CID_LIST], k, temp_w2);	/* k = pos for songID, also put in albumID */
		IncWordVal(temp_queue, j, 0x01);	/* nx write pos */
	}
#endif

#ifdef _PC_DEBUGPRINT_
	printf("\ntemp result sort albumID, trackID:");
	/*
	printf("\nnum_of_track=%d", num_of_item[5]);
	printf("\nSorted ram_song_track(compared string)=");
	i =ram_group[5].sorted_list_hd;
	while (i != INDEX_EOF) {
	printf("\nID=%d\t", i);
	printf("[%d]", ram_song_track[i]);
	ML_list_get_val(ram_group[5].sorted_list, i, j);
	i = j;
	}
	*/
	printf("\ncount album:");
	for (i=0;i<num_of_item[SONG_SORTLOOP];i++) {
		ML_list_get_val_X(artist_associated_count, i, &j);
		printf("[%d]", j);
	}
	printf("\nassoc albumID:");
	for (i=0;i<num_of_item[SONG_SORTLOOP];i++) {
		ML_list_get_val_X(temp_associated[ASSOC_CID_LIST], i, &j);
		printf("[%d]", j);
	}
	/*
	printf("\ncount song:");
	for (i=0;i<num_of_item[SONG_SORTLOOP];i++) {
	ML_list_get_val_X(artist_subassociated[ASSOC_COUNT_LIST], i, &j);
	printf("[%d]", j);
	}
	*/
	printf("\nassoc songID:");
	for (i=0;i<num_of_item[SONG_SORTLOOP];i++) {
		ML_list_get_val_X(artist_subassociated[ASSOC_CID_LIST], i, &j);
		printf("[%d]", j);
	}
#endif

	for (i=0;i<(MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD);i++) {
		temp_associated[ASSOC_COUNT_LIST][i] = 0x00;
		artist_subassociated[ASSOC_COUNT_LIST][i] = 0x00;
	}
	ML_list_put_val_X(temp_queue, 0x00, 0x00);
	for (i=1;i<num_of_item[ALBUM_SORTLOOP];i++) {	/* i = albumID */
		ML_list_get_val_X(artist_associated_count, (i-1), &temp_w2);
		ML_list_put_val_X(temp_queue, i, temp_w2);
	}
	/* temp_queue=>head position for insert element */

	temp_w2 = 0x00;	/* pos for accumulated # of album */
	temp_w = 0x00;	/* pos for albumID */
	ML_list_get_val_X(temp_associated[ASSOC_CID_LIST], 0x00, &j);	/* j = previous albumID */
	ML_list_put_val_X(artist_subassociated[ASSOC_COUNT_LIST], 0x00, 0x01);
	ML_list_put_val_X(temp_associated[ASSOC_COUNT_LIST], 0x00, 0x01);
	ML_list_get_val_X(artist_associated_count, 0x00, &listnxpos);
	for (i=1;i<num_of_item[SONG_SORTLOOP];i++) {
		if (i == listnxpos) {
			temp_w2++;
			//temp_w = listnxpos;
			temp_w++;
			ML_list_get_val_X(temp_associated[ASSOC_CID_LIST], listnxpos, &j);	/* j = previous albumID */
			ML_list_put_val_X(temp_associated[ASSOC_CID_LIST], temp_w, j);
			ML_list_put_val_X(artist_subassociated[ASSOC_COUNT_LIST], temp_w, 0x01);
			ML_list_put_val_X(temp_associated[ASSOC_COUNT_LIST], temp_w2, 0x01);
			ML_list_get_val_X(artist_associated_count, temp_w2, &listnxpos);
		} else {
			ML_list_get_val_X(temp_associated[ASSOC_CID_LIST], i, &k);	/* k = albumID */
			if (j != k) {
				IncWordVal(temp_associated[ASSOC_COUNT_LIST], temp_w2, 0x01);
				temp_w++;
				ML_list_put_val_X(temp_associated[ASSOC_CID_LIST], temp_w, k);
				j = k;
			}
			IncWordVal(artist_subassociated[ASSOC_COUNT_LIST], temp_w, 0x01);
		}
	}

	ML_list_get_val_X(temp_associated[ASSOC_COUNT_LIST], 0x00, &j);
	for (i=1;i<num_of_item[ARTIST_SORTLOOP];i++) {
		IncWordVal(temp_associated[ASSOC_COUNT_LIST], i, j);
		ML_list_get_val_X(temp_associated[ASSOC_COUNT_LIST], i, &j);
	}

	ML_list_get_val_X(artist_subassociated[ASSOC_COUNT_LIST], 0x00, &j);
	for (i=1;i<num_of_item[SONG_SORTLOOP];i++) {
		IncWordVal(artist_subassociated[ASSOC_COUNT_LIST], i, j);
		ML_list_get_val_X(artist_subassociated[ASSOC_COUNT_LIST], i, &j);
	}

	if (ML_MemWrite(Media_type, ARTIST_ASSOC_COUNT_DTYPE, num_of_item[SONG_SORTLOOP], (uint24 _X *)&temp_associated, ML_LIST_STRUCT_SIZE) != SUCCESS) {
		/* flash access error */
	}
	if (ML_MemWrite(Media_type, ARTIST_SUBASSOC_COUNT_DTYPE, num_of_item[SONG_SORTLOOP], (uint24 _X *)&artist_subassociated, ML_LIST_STRUCT_SIZE) != SUCCESS) {
		/* flash access error */
	}

#ifdef _PC_DEBUGPRINT_
	printf("\nsort artist->albumID->trackID:");
	/*
	printf("\nnum_of_track=%d", num_of_item[5]);
	printf("\nSorted ram_song_track(compared string)=");
	i =ram_group[5].sorted_list_hd;
	while (i != INDEX_EOF) {
	printf("\nID=%d\t", i);
	printf("[%d]", ram_song_track[i]);
	ML_list_get_val(ram_group[5].sorted_list, i, j);
	i = j;
	}
	*/
	printf("\ncount album:\t");
	for (i=0;i<num_of_item[SONG_SORTLOOP];i++) {
		ML_list_get_val_X(temp_associated[ASSOC_COUNT_LIST], i, &j);
		printf("[%d]", j);
	}
	printf("\nassoc albumID:\t");
	for (i=0;i<num_of_item[SONG_SORTLOOP];i++) {
		ML_list_get_val_X(temp_associated[ASSOC_CID_LIST], i, &j);
		printf("[%d]", j);
	}
	printf("\ncount song:\t");
	for (i=0;i<num_of_item[SONG_SORTLOOP];i++) {
		ML_list_get_val(artist_subassociated[ASSOC_COUNT_LIST], i, &j);
		printf("[%d]", j);
	}
	printf("\nassoc songID:\t");
	for (i=0;i<num_of_item[SONG_SORTLOOP];i++) {
		ML_list_get_val(artist_subassociated[ASSOC_CID_LIST], i, &j);
		printf("[%d]", j);
	}
#endif

	/* write song_info_X */
	for (i=0;i<(MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD);i++) {
		temp_queue[i] = ram_group[ARTIST_SORTLOOP].associate_list[i];
	}
	if (ML_MemWrite(Media_type, SONG_INFO_ARTIST_DTYPE, num_of_item[SONG_SORTLOOP], temp_queue, ML_LIST_STRUCT_SIZE) != SUCCESS) {
		/* flash access error */
	}
	for (i=0;i<(MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD);i++) {
		temp_queue[i] = ram_group[ALBUM_SORTLOOP].associate_list[i];
	}
	if (ML_MemWrite(Media_type, SONG_INFO_ALBUM_DTYPE, num_of_item[SONG_SORTLOOP], temp_queue, ML_LIST_STRUCT_SIZE) != SUCCESS) {
		/* flash access error */
	}
	for (i=0;i<(MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD);i++) {
		temp_queue[i] = ram_group[GENRE_SORTLOOP].associate_list[i];
	}
	if (ML_MemWrite(Media_type, SONG_INFO_GENRE_DTYPE, num_of_item[SONG_SORTLOOP], temp_queue, ML_LIST_STRUCT_SIZE) != SUCCESS) {
		/* flash access error */
	}
	for (i=0;i<(MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD);i++) {
		temp_queue[i] = ram_group[YEAR_SORTLOOP].associate_list[i];
	}
	if (ML_MemWrite(Media_type, SONG_INFO_YEAR_DTYPE, num_of_item[SONG_SORTLOOP], temp_queue, ML_LIST_STRUCT_SIZE) != SUCCESS) {
		/* flash access error */
	}

#ifdef _NEWMUSIC_
	/* new music */
	read_p_Y = (uint24 *) &ram_newmusic;
	for (i=0;i<NEW_MUSIC_STRUCT_SIZE;i++) {
		temp_queue[i] = read_p_Y[i];
	}
	if (ML_MemWrite(Media_type, NEW_MUSIC_DTYPE, NEW_MUSIC_STRUCT_SIZE, temp_queue, NEW_MUSIC_STRUCT_SIZE) != SUCCESS) {
		/* flash access error */
	}
#endif

#ifdef _AUDIBLE_
	read_p_Y = (uint24 *) &ram_audible;
	for (i=0;i<AUDIBLE_STRUCT_SIZE;i++) {
		temp_queue[i] = read_p_Y[i];	/* chk temp_queue len */
	}
	if (ML_MemWrite(Media_type, AUDIBLE_DTYPE, AUDIBLE_STRUCT_SIZE, temp_queue, AUDIBLE_STRUCT_SIZE) != SUCCESS) {
		/* flash access error */
	}
#endif

#if 0	/* 05/02/19 for cvs012 code no sd init */
	if (Media_type == MEDIA_TYPE_FLASH) {
		/* init SD DB, in case SD is empty. */
		for (i=0;i<MAX_NUM_OF_GROUP;i++) {
			num_of_item[i] = 0x00;
		}
		for (i=0;i<(MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD);i++) {
			temp_queue[i] = 0x000000;
		}
		if (ML_MemWrite(MEDIA_TYPE_SD, SONG_ARRAY_DTYPE, num_of_item[SONG_SORTLOOP], temp_queue, ML_LIST_STRUCT_SIZE) != SUCCESS) {
			/* flash access error */
		}
		if (ML_MemWrite(MEDIA_TYPE_SD, NUM_OF_SONG_DTYPE, num_of_item[SONG_SORTLOOP], temp_queue, ML_INDEX_STRUCT_SIZE) != SUCCESS) {	/* num_of_song, temp_queue is dummy */
			/* flash access error */
		}
	}
#endif	/* 05/02/19 for cvs012 code no sd init */
   FlushAndCloseCache();
}

/*================================================================================================*/
COMP_OPT uint24 ItemCmp(uint24 CmpType, uint24 *item0, uint24 *item1) {
	uint24 i;
	uint24 cmpsize;

	switch(CmpType) {
		case	UINT24_STR_CMPTYPE:
			cmpsize = LEN_OF_NAME_IN_BYTE;
			break;
		case	UINT24_VAL_CMPTYPE:
			cmpsize = 0x01;
			break;
		case	UINT24_MARKTAB_CMPTYPE:
			cmpsize = (LEN_OF_SUB_STRING_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD);
			break;
		case	UINT24_PATH_CMPTYPE:
			cmpsize = (LEN_OF_FILEPATH_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD);
			break;
		default	:
			return (0x02);	/* error, not handled */
			//break;
	}
	for (i=0;i<cmpsize;i++) {
		if ((item0[i] < item1[i]) || ((item0[i] == EOSTR) && (item1[i] != EOSTR))) {
			return (ITEMCMP_ITEM0_SMALL);
		} else if ((item0[i] > item1[i]) || ((item1[i] == EOSTR) && (item0[i] != EOSTR))) {
			return (ITEMCMP_ITEM0_LARGE);
		}
		if ((item1[i] == EOSTR) && (item0[i] == EOSTR)) {
			return (ITEMCMP_ITEM01_EQUAL);
		}
	}
	return (ITEMCMP_ITEM01_EQUAL);
}

/*==================================================================================================
FUNCTION: AddEntryToLibrary()

DESCRIPTION:
Description of this specific function.

Called by UI, the AddEntryToLibrary is the music library building algorithms for
the flash-type memory by sorting the memory by, but not limited to, album,
artist, genre and track.

ARGUMENTS PASSED:
*song_info		-RAM_SONG_INFO_T pointer
The global variable, int24 *g_wSongTitle, int24 *g_wSongArtist, int24
*g_wSongAlbum, int24 *g_wSongGenre, int24 *g_wSongYear, int24 *g_wSongPath
and int g_Unicode are grouped into one struct RAM_SONG_INFO_T,

option			-int16 option

RETURN VALUE:
COMP_OPT

PRE-CONDITIONS:
None

POST-CONDITIONS:
None

IMPORTANT NOTES:
None

==================================================================================================*/
void ML_AddEntryToLibrary(uint24 Media_type, RAM_SONG_INFO_T *song_info, int16 option) {
	uint24	mt_lbID;
	uint24	mt_ubID;
	uint24	mt_midID;
	/*
	uint24	temp_midID;
	*/
	uint24	buf_lbID;
	uint24	buf_ubID;
	uint24	buf_midID;

	uint24 i;
	uint24 j;
	/*
	uint24 k;
	*/
	uint24 temp_name_bufID;
	uint24 temp_previousID;
	uint24 temp_midID;

	uint24 gp_count;
	uint24 name_unicode;
	uint24 name_found;
	uint24 current_songID;
	uint24 *p_fastkey;

#ifdef _PC_DEBUGPRINT_
	printf("\nEnter AddToEntryLibrary().\n");
#endif
	/*
	flush song_info.song_name to flash_song_name[Media_type][num_of_song].name;
	flush song_info.unicode to flash_song_name[Media_type][num_of_song].unicode;
	flush song_info.path_name to flash_path_name[num_of_song];
	flush song_info.year to flash_year[num_of_song];
	num_of_song++;
	func(sort the song name) {
	locate the start and end index in the ram_song_group.mark_table_index
	using binary search in the mark table, by comparing the new song name with
	ram_song_group.mark_table_string;

	load the song names from flash indicated by the start and end index to
	temp_name_buf in sorted order;

	start binary search in temp_name_buf to locate the exact position for
	putting the new song ID (ie, num_of_song) in;
	insert the new song ID (ie, num_of_song) into ram_song_group.sorted_list
	in proper location;

	insert the new track number to ram_song_group.associate_list[num_of_song];

	func(update the mark table) {
	increase the distance of specific mark entry;

	if the distance is larger than threshold, split the entries in the
	mark table into half;
	}
	}
	for each group X in (artist,album,genre,year,track) {
	locate the start and end index in the ram_X_group.mark_table_index using a
	binary search in the mark table, by comparing the new group X name with
	ram_X_group.mark_table_string;

	load the group X names from flash indicated by the start and end index to temp_name_buf;

	start binary search in ram_X_group.sorted_list to locate the exact
	position for putting the group ID in;

	if the group ID does not exist before {
	num_of_X++;

	insert the group ID (num_of_X) into ram_X_group.sorted_list in proper
	location if it does not exist before;

	if group=album {
	update the ram_album_group.sequence_list(num_of_X)=sort_count for proper ranking;
	}

	flush song_info.X_name to flash_X_name[num_of_X];

	flush song_info.g_unicode to flash_X_name[num_of_song].g_unicode;

	func(update the mark table) {
	increase the distance of specific mark entry;

	if the distance is larger than threshold, split the entries in the
	mark table into half.
	}
	}
	for artist group, the associate list contains the artist ID;
	for album group, the associate list contains the album ID;
	for genre group, the associate list contains the genre ID

	insert the new or existing group ID into ram_X_group.associated_list[num_of_song];
	}
	*/
	InitandUseCache();
	if (option == ADD_OPTION_MUSIC) {
		if (num_of_item[SONG_SORTLOOP]>= MAX_NUM_OF_SONG) return;
		current_songID = num_of_item[SONG_SORTLOOP];

		for (i=0;i<(LEN_OF_FILEPATH_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD);i++) {
			flash_path_name[i] = song_info->path_name[i];
			if (ChkTermInWord(flash_path_name[i])) {
				if ((flash_path_name[i] & 0x0000FF) == 0x00) {
					flash_path_name[i] = flash_path_name[i] & 0x000000;
				} else if ((flash_path_name[i] & 0x00FF00) == 0x00) {
					flash_path_name[i] = flash_path_name[i] & 0x0000FF;
				} else {
					flash_path_name[i] = flash_path_name[i] & 0x00FFFF;
				}
				i++;
				for (j=i;j<(LEN_OF_FILEPATH_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD);j++) {
					flash_path_name[j] = 0x00;
				}
				break;
			}
			/*
			#ifdef _SAVENAMESHORTCUT_
			if (ChkTermInWord(flash_path_name[i])) break;
			#endif
			*/
		}
		p_fastkey=(uint24 *)&song_info->g_songFastKey;
		flash_path_name[(LEN_OF_FILEPATH_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD)+0]=*p_fastkey;p_fastkey++;
		flash_path_name[(LEN_OF_FILEPATH_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD)+1]=*p_fastkey;
		if (ML_MemWrite(Media_type, PATH_NAME_DTYPE, current_songID, flash_path_name, ML_PATH_NAME_STRUCT_SIZE) != SUCCESS) {
			/* flash access error */
		}

		ML_ChkOnTheGo(Media_type, flash_path_name, current_songID);

		// Save Long filename
		for (i=0;i<(LEN_OF_LONG_FILENAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD);i++) {
			flash_path_name[i] = song_info->long_filename[i];
			if (flash_path_name[i] == 0x00) {
				i++;
				for (j=i;j<(LEN_OF_LONG_FILENAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD);j++) {
					flash_path_name[j] = 0x00;
				}
				break;
			}
		}
		// TODO: database overflow if 2MB flash DB is used
//		if (ML_MemWrite(Media_type, LONG_FILENAME_DTYPE, current_songID, flash_path_name, ML_LONG_FILENAME_STRUCT_SIZE) != SUCCESS) {
//			/* flash access error */
//		}

		for (gp_count=0;gp_count<MAX_NUM_OF_GROUP;gp_count++) {
#ifdef _PC_DEBUGPRINT_
			printf("\nInsert group_name[%d]:", gp_count);
#endif
			name_unicode = 0x0;
			switch (gp_count) {
			case ARTIST_SORTLOOP:
				if ((song_info->unicode & BITMASK_ARTIST) == BITCHK_ARTIST_ALLASCII) {
					name_unicode = BITSET_NAME_ALLASCII;
				} else if ((song_info->unicode & BITMASK_ARTIST) == BITCHK_ARTIST_UNICODE) {
					name_unicode = BITSET_NAME_UNICODE;
				} else {
					name_unicode = BITSET_NAME_MIXCODE;
				}
				for (i=0;i<LEN_OF_FLASH_NAME_BUF;i++) {
					temp_song_info_name[i] = song_info->artist_name[i];
				}
				break;
			case ALBUM_SORTLOOP:
				if ((song_info->unicode & BITMASK_ALBUM) == BITCHK_ALBUM_ALLASCII) {
					name_unicode = BITSET_NAME_ALLASCII;
				} else if ((song_info->unicode & BITMASK_ALBUM) == BITCHK_ALBUM_UNICODE) {
					name_unicode = BITSET_NAME_UNICODE;
				} else {
					name_unicode = BITSET_NAME_MIXCODE;
				}
				for (i=0;i<LEN_OF_FLASH_NAME_BUF;i++) {
					temp_song_info_name[i] = song_info->album_name[i];
				}
				break;
			case GENRE_SORTLOOP:
				if ((song_info->unicode & BITMASK_GENRE) == BITCHK_GENRE_ALLASCII) {
					name_unicode = BITSET_NAME_ALLASCII;
				} else if ((song_info->unicode & BITMASK_GENRE) == BITCHK_GENRE_UNICODE) {
					name_unicode = BITSET_NAME_UNICODE;
				} else {
					name_unicode = BITSET_NAME_MIXCODE;
				}
				if ((song_info->unicode & BITMASK_GENRE) == BITCHK_GENRE_ID3V1) {
					name_unicode = BITSET_NAME_ALLASCII;
					if (song_info->genre_name[0] < NUM_OF_ID3GENRE) {
						j = 0x00;
						for (i=0;i<LEN_OF_ID3V1_GENRE;i++) {
							temp_song_info_name[i] = genre_table[song_info->genre_name[0]][j++];
							temp_song_info_name[i] |= (genre_table[song_info->genre_name[0]][j++]<<8);
							temp_song_info_name[i] |= (genre_table[song_info->genre_name[0]][j++]<<16);
							/*
							for (i=0;i<(LEN_OF_ID3V1_GENRE/NUM_OF_BYTE_IN_ONE_WORD);i++) {
							temp_song_info_name[i] = genre_table[song_info->genre_name[0]][i];
							*/
						}
					} else {
						temp_song_info_name[0] = 0x00;
					}
				} else {
					if (((song_info->genre_name[0] & 0x0000FF) == 0x28)
						&& ((song_info->genre_name[1] & 0x0000FF) == 0x29)
						&& ((song_info->genre_name[1] & 0x00FF00) == 0x00)) {
							/* genre is format: "(NN)" */
							temp_song_info_name[0] = (song_info->genre_name[0] & 0x00FF00)>>8;
							if ((temp_song_info_name[0] > 0x2F) && (temp_song_info_name[0] < 0x3A)) {
								temp_song_info_name[0] -= 0x30;
							}
							temp_song_info_name[1] = (song_info->genre_name[0] & 0xFF0000)>>16;
							if ((temp_song_info_name[1] > 0x2F) && (temp_song_info_name[1] < 0x3A)) {
								temp_song_info_name[1] -= 0x30;
							}
							if ((temp_midID = (temp_song_info_name[0]*10) + temp_song_info_name[1]) < NUM_OF_ID3GENRE) {
								j = 0x00;
								for (i=0;i<LEN_OF_ID3V1_GENRE;i++) {
									temp_song_info_name[i] = genre_table[temp_midID][j++];
									temp_song_info_name[i] |= (genre_table[temp_midID][j++]<<8);
									temp_song_info_name[i] |= (genre_table[temp_midID][j++]<<16);
								}
							} else {
								temp_song_info_name[0] = 0x00;
							}
					} else {
						for (i=0;i<LEN_OF_FLASH_NAME_BUF;i++) {
							temp_song_info_name[i] = song_info->genre_name[i];
						}
					}
				}
				break;
			case SONG_SORTLOOP:
#ifdef _ADD_SPACE_TO_NAME_
				/* add tailing sp for display effect => skip copy terminator */
				for (i=0;i<LEN_OF_FLASH_NAME_BUF;i++) {
					flash_group_name.name[i] = 0x202020;
				}
#endif
				for (i=0;i<LEN_OF_FLASH_NAME_BUF;i++) {
#ifdef _ADD_SPACE_TO_NAME_
					/* add tailing sp for display effect => skip copy terminator */
					if ((song_info->unicode & BITMASK_SONG) == BITCHK_SONG_ALLASCII) {
						if (song_info->song_name[i] == 0x0) {
							break;
						} else if ((song_info->song_name[i] & 0x0000FF) == 0x0) {
							break;
						} else if ((song_info->song_name[i] & 0x00FF00) == 0x0) {
							flash_group_name.name[i] = song_info->song_name[i];
							flash_group_name.name[i] |= 0x202000;
							break;
						} else if ((song_info->song_name[i] & 0xFF0000) == 0x0) {
							flash_group_name.name[i] = song_info->song_name[i];
							flash_group_name.name[i] |= 0x200000;
							break;
						} else {
							flash_group_name.name[i] = song_info->song_name[i];
						}
					} else {
						if (song_info->song_name[i] == EOSTR) {
							break;
						} else {
							flash_group_name.name[i] = song_info->song_name[i];
						}
					}
#else	/* _ADD_SPACE_TO_NAME_ */
					flash_group_name.name[i] = song_info->song_name[i];
#endif	/* _ADD_SPACE_TO_NAME_ */
				}

				if ((song_info->unicode & BITMASK_SONG) == BITCHK_SONG_ALLASCII) {
					flash_group_name.unicode = BITSET_NAME_ALLASCII;
				} else {
					/*
					if (((song_info->unicode & BITMASK_SONG) == BITCHK_SONG_UNICODE)
					|| ((song_info->unicode & BITMASK_SONG) == BITCHK_SONG_MIXCODE))
					*/
					if ((song_info->unicode & BITMASK_SONG) == BITCHK_SONG_UNICODE) {
						flash_group_name.unicode = BITSET_NAME_UNICODE;
					} else {
						flash_group_name.unicode = BITSET_NAME_MIXCODE;
						/*
						assert((song_info->unicode & BITMASK_SONG) == BITCHK_SONG_MIXCODE);
						*/
					}
				}
				if (ML_extract_string(flash_group_name.unicode, insert_name, flash_group_name.name, LEN_OF_NAME_IN_BYTE) == 0x00) {
					/* "Unknown Song":unicode=ALL_ASCII */
					flash_group_name.name[0] = 0x6E5528;
					flash_group_name.name[1] = 0x6F6E6B;
					flash_group_name.name[2] = 0x206E77;
					flash_group_name.name[3] = 0x6E6F53;
					flash_group_name.name[4] = 0x002967;
					flash_group_name.unicode = BITSET_NAME_ALLASCII;
					ML_extract_string(flash_group_name.unicode, insert_name, flash_group_name.name, LEN_OF_NAME_IN_BYTE);
				}
#if 0
				/* store the exact sorting string to flash for debug only, _PRINTSONGID_, debug sorting */
				if ((song_info->unicode & BITMASK_SONG) == BITCHK_SONG_ALLASCII) {
					j = 0;
					for (i=0;i<LEN_OF_FLASH_NAME_BUF;i++) {
						flash_group_name.name[i] = (insert_name[j] & 0xFF) | ((insert_name[++j] & 0xFF)<<8) | ((insert_name[++j] & 0xFF)<<16);
						j++;
					}
				}
#endif
				if (ML_MemWrite(Media_type, SONG_NAME_DTYPE, num_of_item[gp_count], (uint24 _X *)&flash_group_name, ML_SONG_NAME_STRUCT_SIZE) != SUCCESS) {
					/* flash access error */
				}

#ifdef _PC_DEBUGPRINT_
				printf("\ninsert_name=");
				for (w=0;w<LEN_OF_NAME_IN_BYTE;w++) {
					printf("[%X]", insert_name[w]);
				}
#endif
				break;
			case YEAR_SORTLOOP:
				if ((song_info->year == 0x00) || (song_info->year > 0x7DA)) {
#if 1
					insert_name[0] = UNKNOWN_YEAR_CODE;
#else
#ifdef _NEWMUSIC_
					insert_name[0] = ((g_file_time & 0xFFF000) >> 12);
#else
					insert_name[0] = 0x7D4;		//2004
#endif
#endif
				} else {
					insert_name[0] = song_info->year;
				}
				break;
#if 0
			case TRACK_SORTLOOP:
				insert_name[0] = song_info->track_number;
				break;
#endif
			}

			if ((gp_count == ARTIST_SORTLOOP) || (gp_count == ALBUM_SORTLOOP) ||(gp_count == GENRE_SORTLOOP)){
				if (ML_extract_string(name_unicode, insert_name, temp_song_info_name, LEN_OF_NAME_IN_BYTE) == 0x00) {
					switch (gp_count) {
					case ARTIST_SORTLOOP:
						/* "Unknown Artist":unicode=ALL_ASCII */
						temp_song_info_name[0] = 0x6E5528;
						temp_song_info_name[1] = 0x6F6E6B;
						temp_song_info_name[2] = 0x206E77;
						temp_song_info_name[3] = 0x747241;
						temp_song_info_name[4] = 0x747369;
						temp_song_info_name[5] = 0x000029;
						break;
					case ALBUM_SORTLOOP:
						/* "Unknown Album":unicode=ALL_ASCII */
						temp_song_info_name[0] = 0x6E5528;
						temp_song_info_name[1] = 0x6F6E6B;
						temp_song_info_name[2] = 0x206E77;
						temp_song_info_name[3] = 0x626C41;
						temp_song_info_name[4] = 0x296D75;
						temp_song_info_name[5] = 0x000000;
						break;
					case GENRE_SORTLOOP:
						/* "Unknown Genre":unicode=ALL_ASCII */
						temp_song_info_name[0] = 0x6E5528;
						temp_song_info_name[1] = 0x6F6E6B;
						temp_song_info_name[2] = 0x206E77;
						temp_song_info_name[3] = 0x6E6547;
						temp_song_info_name[4] = 0x296572;
						temp_song_info_name[5] = 0x000000;
						break;
						/*
						default:
						// "Unknown Titled":unicode=ALL_ASCII
						temp_song_info_name[0] = 0x6B6E55;
						temp_song_info_name[1] = 0x776F6E;
						temp_song_info_name[2] = 0x54206E;
						temp_song_info_name[3] = 0x6C7469;
						temp_song_info_name[4] = 0x006465;
						*/
					}
					name_unicode = BITSET_NAME_ALLASCII;
					ML_extract_string(name_unicode, insert_name, temp_song_info_name, LEN_OF_NAME_IN_BYTE);
				}
			}
#ifdef _PC_DEBUGPRINT_
			printf("\ninsert_name[%d]=", gp_count);
			for (w=0;w<LEN_OF_NAME_IN_BYTE;w++) {
				printf("[%X]", insert_name[w]);
			}
#endif

			name_found = FALSE;
			mt_lbID = 0;
			if (MarkTableID[gp_count] > 0x01) {
				mt_ubID = MarkTableID[gp_count];	/* MarkTableID = # of entry-1 */
				mt_midID = mt_ubID>>1;
				/* table size = 0 to (MarkTableID-1) */
				do {
// 					if ((gp_count == YEAR_SORTLOOP) || (gp_count == TRACK_SORTLOOP)) {
                    if (gp_count == YEAR_SORTLOOP) {
						/* case for year */
						i = ItemCmp(UINT24_VAL_CMPTYPE, ram_marktable[gp_count].mark_table_string[mt_midID],
							insert_name);
					} else {
						i = ItemCmp(UINT24_MARKTAB_CMPTYPE, ram_marktable[gp_count].mark_table_string[mt_midID],
							insert_name);
					}
					if (i == ITEMCMP_ITEM0_SMALL) {
						mt_lbID = mt_midID+1;
					} else if (i == ITEMCMP_ITEM0_LARGE) {
						mt_ubID = mt_midID;
					} else {
//						if ((gp_count == YEAR_SORTLOOP) || (gp_count == TRACK_SORTLOOP)) {
                        if (gp_count == YEAR_SORTLOOP) {
							/* case for year */
							name_found = TRUE;
							break;
						} else {
							i = LEN_OF_MTSTRING;
						}
					}
					/* first 30 char equal */
					if (i == LEN_OF_MTSTRING) {
#ifdef _ASSERT_
						/* case for year, year search should _not_ reach here */
// 						assert((gp_count != YEAR_SORTLOOP) && (gp_count != TRACK_SORTLOOP));
						assert(gp_count != YEAR_SORTLOOP);
#endif
						ML_MemRead(Media_type, group_dtype[gp_count], ram_marktable[gp_count].mark_table_index[mt_midID][MT_NAMEID_COL], (uint24 *) &flash_group_name_y, ML_SONG_NAME_STRUCT_SIZE);
						ML_cpy_flash_group_name((FLASH_GROUP_NAME_T _X *) &flash_group_name,(FLASH_GROUP_NAME_T *) &flash_group_name_y);
						ML_extract_string(flash_group_name.unicode,
							temp_name_buf,
							flash_group_name.name,
							LEN_OF_NAME_IN_BYTE);
						if ((i = ItemCmp(UINT24_STR_CMPTYPE, temp_name_buf, insert_name)) == ITEMCMP_ITEM0_SMALL) {
							mt_lbID = mt_midID+1;
						} else if (i == ITEMCMP_ITEM0_LARGE) {
							mt_ubID = mt_midID;
						} else {
							name_found = TRUE;
							break;
						}
					}
					mt_midID = (mt_ubID + mt_lbID)>>1;
				} while (mt_lbID < mt_ubID);
				if ((name_found != TRUE) && (mt_midID > 0x00)) mt_midID--;
			} else {
				mt_midID = 0x00;
			}
#ifdef _PC_DEBUGPRINT_
			printf("\nmt_midID=%d", mt_midID);
#endif

			if ((name_found != TRUE) || (gp_count == SONG_SORTLOOP)) {
				/* copy2 temp_name_buf */
				/* build index table for marktable search */
				if (mt_midID > 0) {
					ML_list_get_val(ram_group[gp_count].sorted_list,
						ram_marktable[gp_count].mark_table_index[mt_midID-1][MT_NAMEID_COL], &j);
					temp_previousID = j;
					/* find the first previous index */
					for (i=0;i<(ram_marktable[gp_count].mark_table_index[mt_midID-1][MT_COUNT_COL]-2);i++) {
						ML_list_get_val(ram_group[gp_count].sorted_list, j, &temp_previousID);
						j = temp_previousID;
					}
				} else {
					temp_previousID = INDEX_EOF;	/* so the first previous index is invalid */
				}
				i = ram_marktable[gp_count].mark_table_index[mt_midID][MT_NAMEID_COL];

				for (temp_name_bufID=0;temp_name_bufID<ram_marktable[gp_count].mark_table_index[mt_midID][MT_COUNT_COL];
					temp_name_bufID++) {
						if (i == INDEX_EOF) break;
						temp_list_buf[temp_name_bufID][TEMP_LIST_PREID_COL] = temp_previousID;
						temp_list_buf[temp_name_bufID][TEMP_LIST_THISID_COL] = i;
						ML_list_get_val(ram_group[gp_count].sorted_list, i, &temp_list_buf[temp_name_bufID][TEMP_LIST_NXID_COL]);

						temp_previousID = temp_list_buf[temp_name_bufID][TEMP_LIST_THISID_COL];
						i = temp_list_buf[temp_name_bufID][TEMP_LIST_NXID_COL];
				}	/* temp_name_bufID-1 = last entry */

#ifdef _PC_DEBUGPRINT_
				printf("\ntemp_name_bufID=%d:temp_list_buf=", temp_name_bufID);
				//for (w=0;w<DISTANCE_IN_MARK_TABLE;w++) {
				for (w=0;w<temp_name_bufID;w++) {
					if (w == temp_name_bufID) { printf("\n"); }
					printf("\n[%X:", temp_list_buf[w][TEMP_LIST_PREID_COL]);
					printf("%X:", temp_list_buf[w][TEMP_LIST_THISID_COL]);
					printf("%X]", temp_list_buf[w][TEMP_LIST_NXID_COL]);
				}
#endif

				if (gp_count == SONG_SORTLOOP) {
					name_found = FALSE;
				}
				/* bsearch */
				buf_lbID = 0;	/* lower bound, point of match at the end */
				if (temp_name_bufID > 0x00) {
					buf_ubID = temp_name_bufID;
					buf_midID = temp_name_bufID>>1;
					/* table size = 0 to (temp_name_bufID-1) */
					do {
						if (gp_count == YEAR_SORTLOOP) {
							/* case for year */
							j = ram_song_year[temp_list_buf[buf_midID][TEMP_LIST_THISID_COL]];
							/*					i = ItemCmp(UINT24_VAL_CMPTYPE,
							&ram_song_year[temp_list_buf[buf_midID][TEMP_LIST_THISID_COL]],
							insert_name);
							*/
							i = ItemCmp(UINT24_VAL_CMPTYPE,
								&j,
								insert_name);
#if 0
						} else if (gp_count == TRACK_SORTLOOP) {
							/* case for track */
							j = ram_song_track[temp_list_buf[buf_midID][TEMP_LIST_THISID_COL]];
							/*
							i = ItemCmp(UINT24_VAL_CMPTYPE,
							&ram_song_track[temp_list_buf[buf_midID][TEMP_LIST_THISID_COL]],
							insert_name);
							*/
							i = ItemCmp(UINT24_VAL_CMPTYPE,
								&j,
								insert_name);
#endif
						} else {
							ML_MemRead(Media_type, group_dtype[gp_count], temp_list_buf[buf_midID][TEMP_LIST_THISID_COL], (uint24 *) &flash_group_name_y, ML_SONG_NAME_STRUCT_SIZE);
							ML_cpy_flash_group_name((FLASH_GROUP_NAME_T _X *) &flash_group_name,(FLASH_GROUP_NAME_T *) &flash_group_name_y);
							ML_extract_string(flash_group_name.unicode,
								temp_name_buf, flash_group_name.name, LEN_OF_NAME_IN_BYTE);
							i = ItemCmp(UINT24_STR_CMPTYPE, temp_name_buf, insert_name);
						}
						if (i == ITEMCMP_ITEM0_SMALL) {
							buf_lbID = buf_midID+1;
						} else if (i == ITEMCMP_ITEM0_LARGE) {
							buf_ubID = buf_midID;
						} else {
							name_found = TRUE;
							if (gp_count == SONG_SORTLOOP) {
								buf_midID++;	/* point to the nx id */
							}
							break;
						}
						buf_midID = (buf_ubID + buf_lbID)>>1;
					} while (buf_lbID < buf_ubID);
				} else {
					buf_midID = 0x00;
				}
				if ((name_found != TRUE) || (gp_count == SONG_SORTLOOP)) {
					if ((gp_count == ARTIST_SORTLOOP) || (gp_count == ALBUM_SORTLOOP) || (gp_count == GENRE_SORTLOOP)) {
						for (i=0;i<LEN_OF_FLASH_NAME_BUF;i++) {
							flash_group_name.name[i] = temp_song_info_name[i];
						}
					}
					if (gp_count == YEAR_SORTLOOP) {
						/* case for year */
						ram_song_year[num_of_item[gp_count]] = insert_name[0];
#if 0
					} else if (gp_count == TRACK_SORTLOOP) {
						/* case for track */
						ram_song_track[num_of_item[gp_count]] = insert_name[0];
#endif
					} else if (gp_count != SONG_SORTLOOP) {
						flash_group_name.unicode = name_unicode;
						if (ML_MemWrite(Media_type, group_dtype[gp_count], num_of_item[gp_count], (uint24 _X *)&flash_group_name, ML_SONG_NAME_STRUCT_SIZE) != SUCCESS) {
							/* flash access error */
						}
					}

					/* update the sorted_list, mark_table-add entry only */
#ifdef _PC_DEBUGPRINT_
					printf("\nbuf_midID=%X:insert pos=%X", buf_midID, buf_midID);
#endif
					if (buf_midID == 0) {
						/* Input is the 1st one, update mark_table-add entry only */
						ram_marktable[gp_count].mark_table_index[mt_lbID][MT_NAMEID_COL] = num_of_item[gp_count];
//  						if ((gp_count == YEAR_SORTLOOP) || (gp_count == TRACK_SORTLOOP)) {
						if (gp_count == YEAR_SORTLOOP) {
							/* case for year */
							ram_marktable[gp_count].mark_table_string[mt_lbID][0] = insert_name[0];
						} else {
							for (i=0;i<LEN_OF_MTSTRING;i++) {
								ram_marktable[gp_count].mark_table_string[mt_midID][i] = insert_name[i];
							}
						}
					}
					if (buf_midID < temp_name_bufID ) {
						ML_list_put_val(ram_group[gp_count].sorted_list, num_of_item[gp_count], temp_list_buf[buf_midID][TEMP_LIST_THISID_COL]);
						if (temp_list_buf[buf_midID][TEMP_LIST_PREID_COL] == INDEX_EOF) {
							ram_group[gp_count].sorted_list_hd = num_of_item[gp_count];
						} else {
							ML_list_put_val(ram_group[gp_count].sorted_list, temp_list_buf[buf_midID][TEMP_LIST_PREID_COL], num_of_item[gp_count]);
						}
					} else {
						/* Input is the last one */
						if (temp_name_bufID > 0) {
							ML_list_put_val(ram_group[gp_count].sorted_list, num_of_item[gp_count], temp_list_buf[buf_midID-1][TEMP_LIST_NXID_COL]);
							ML_list_put_val(ram_group[gp_count].sorted_list, temp_list_buf[buf_midID-1][TEMP_LIST_THISID_COL], num_of_item[gp_count]);
						} else {
							/* 1st name: songID=0 */
							ML_list_put_val(ram_group[gp_count].sorted_list, num_of_item[gp_count], (uint24) INDEX_EOF);
							ram_group[gp_count].sorted_list_hd = num_of_item[gp_count];
							MarkTableID[gp_count] = 1;
						}
					}
					/* update mark_table-add entry only */
					ram_marktable[gp_count].mark_table_index[mt_midID][MT_COUNT_COL]++;

#ifdef _PC_DEBUGPRINT_
					printf("ram_group.sorted_list_hd=[%d]\n", ram_group[gp_count].sorted_list_hd);
					printf("ram_group[%d].sorted_list=", gp_count);
					//for (w=0;w<MAX_NUM_OF_SONG;w++) {
					for (w=0;w<num_of_item[gp_count];w++) {
						ML_list_get_val(ram_group[gp_count].sorted_list, w, &y);
						printf("[%d]", y);
					}
#if 0
					printf("\nflash_group_name.name=", num_of_item[gp_count]);
					ML_MemRead(Media_type, SONG_NAME_DTYPE, num_of_item[gp_count], (uint24 *) &flash_group_name_y, ML_SONG_NAME_STRUCT_SIZE);
					ML_cpy_flash_group_name((FLASH_GROUP_NAME_T _X *) &flash_group_name,(FLASH_GROUP_NAME_T *) &flash_group_name_y);
					for (w=0;w<LEN_OF_FLASH_NAME_BUF;w++) {
						printf("[%X]", flash_group_name.name[w]);
					}
					printf("\nflash_group_name.unicode=%X", num_of_item[gp_count], flash_group_name.unicode);
#endif
#endif

					/* update mark_table-split if needed */
					if (ram_marktable[gp_count].mark_table_index[mt_midID][MT_COUNT_COL] > DISTANCE_IN_MARK_TABLE) {
						/* split position index 1/2 of ram_marktable[gp_count].mark_table_index[mt_midID][MT_COUNT_COL] */
#ifdef _ASSERT_
						assert(MarkTableID[gp_count] < NUM_OF_MARK_TABLE_ENTRY);
#endif
						/* move entries down */
						for (j=MarkTableID[gp_count];j>(mt_midID+1);j--) {
// 							if ((gp_count == YEAR_SORTLOOP) || (gp_count == TRACK_SORTLOOP)) {
							if (gp_count == YEAR_SORTLOOP) {
								/* case for year */
								ram_marktable[gp_count].mark_table_string[j][0]
								= ram_marktable[gp_count].mark_table_string[j-1][0];
							} else {
								for (i=0;i<LEN_OF_MTSTRING;i++) {
									ram_marktable[gp_count].mark_table_string[j][i]
									= ram_marktable[gp_count].mark_table_string[j-1][i];
									/*
									#ifdef _SAVENAMESHORTCUT_
									//at here the effect is too small...
									if (ram_marktable[gp_count].mark_table_string[j][i] == EOSTR) break;
									#endif
									*/
								}
							}
							ram_marktable[gp_count].mark_table_index[j][MT_NAMEID_COL]
							= ram_marktable[gp_count].mark_table_index[j-1][MT_NAMEID_COL];
							ram_marktable[gp_count].mark_table_index[j][MT_COUNT_COL]
							= ram_marktable[gp_count].mark_table_index[j-1][MT_COUNT_COL];
						}
						/*
						#ifdef _ASSERT_
						assert(temp_name_bufID == DISTANCE_IN_MARK_TABLE);
						#endif
						*/
						temp_midID = mt_midID+1;
						j = temp_list_buf[DISTANCE_IN_MARK_TABLE/2][TEMP_LIST_THISID_COL];
//  						if ((gp_count == YEAR_SORTLOOP) || (gp_count == TRACK_SORTLOOP)) {
						if (gp_count == YEAR_SORTLOOP) { 
							/* case for year */
							ram_marktable[gp_count].mark_table_string[temp_midID][0] = insert_name[0];
						} else {
							ML_MemRead(Media_type, group_dtype[gp_count], j, (uint24 *) &flash_group_name_y, ML_SONG_NAME_STRUCT_SIZE);
							ML_cpy_flash_group_name((FLASH_GROUP_NAME_T _X *) &flash_group_name,(FLASH_GROUP_NAME_T *) &flash_group_name_y);
							ML_extract_string(flash_group_name.unicode,
								ram_marktable[gp_count].mark_table_string[temp_midID],
								flash_group_name.name,
								LEN_OF_MTSTRING);
						}
						ram_marktable[gp_count].mark_table_index[temp_midID][MT_NAMEID_COL] = j;
						if (buf_midID > (DISTANCE_IN_MARK_TABLE/2)) {
							ram_marktable[gp_count].mark_table_index[temp_midID][MT_COUNT_COL]
							= (DISTANCE_IN_MARK_TABLE/2) + 1;
							ram_marktable[gp_count].mark_table_index[mt_midID][MT_COUNT_COL]
							= (DISTANCE_IN_MARK_TABLE/2);
						} else {
							ram_marktable[gp_count].mark_table_index[temp_midID][MT_COUNT_COL]
							= (DISTANCE_IN_MARK_TABLE/2);
							ram_marktable[gp_count].mark_table_index[mt_midID][MT_COUNT_COL]
							= (DISTANCE_IN_MARK_TABLE/2) + 1;
						}
						MarkTableID[gp_count]++;
#ifdef _PC_DEBUGPRINT_
						printf("\nMarkTableID[gp_count] new add=%d", MarkTableID[gp_count]);
#endif
					}

					if (gp_count == SONG_SORTLOOP) {
						ML_list_put_val(ram_group[gp_count].associate_list, num_of_item[gp_count], song_info->track_number);
					} else {
						ML_list_put_val(ram_group[gp_count].associate_list, current_songID, num_of_item[gp_count]);
					}
					num_of_item[gp_count]++;

#ifdef _PC_DEBUGPRINT_
					printf("\nUpdated MrakTableID[%d]=%d, ram_marktable[gp_count].mark_table_index=", gp_count, MarkTableID[gp_count]);
					for (w=0;w<MarkTableID[gp_count];w++) {
						printf("\n%X", ram_marktable[gp_count].mark_table_index[w][MT_NAMEID_COL]);
						printf("C=%XStr=", ram_marktable[gp_count].mark_table_index[w][MT_COUNT_COL]);
						for (y=0;y<LEN_OF_MTSTRING;y++) {
							printf("%X:", ram_marktable[gp_count].mark_table_string[w][y]);
						}
					}
					printf("\nnum_of_item[gp_count]=%d", num_of_item[gp_count]);
					printf("MarkTableID[gp_count]=%d", MarkTableID[gp_count]);

					printf("\nSorted ram_name(compared string)=");
					w = ram_group[gp_count].sorted_list_hd;
					while (TRUE) {
						if (w == INDEX_EOF) break;
						printf("\nID=%X", w);

						if (gp_count == YEAR_SORTLOOP) {
							printf("[%d]", ram_song_year[w]);
#if 0
						} else if (gp_count == TRACK_SORTLOOP) {
							printf("[%d]", ram_song_track[w]);
#endif
						} else {
							ML_MemRead(Media_type, group_dtype[gp_count], w, (uint24 *) &flash_group_name_y, ML_SONG_NAME_STRUCT_SIZE);
							ML_cpy_flash_group_name((FLASH_GROUP_NAME_T _X *) &flash_group_name,(FLASH_GROUP_NAME_T *) &flash_group_name_y);
							ML_extract_string(flash_group_name.unicode,
								temp_name_buf,
								flash_group_name.name,
								LEN_OF_NAME_IN_BYTE);
							printf("[");
							for (y=0;y<LEN_OF_NAME_IN_BYTE;y++) {
								//			printf("[%X]", temp_name_buf[y]);
								printf("%c", temp_name_buf[y]);
								if (temp_name_buf[y] == 0) break;
							}
							printf("]");
						}
						ML_list_get_val(ram_group[gp_count].sorted_list, w, &y);
						w = y;
					}
#endif
				} else {
					/* song sorting _should_not_ enter here */
					/* (name_found != TRUE) && marktable strings != insert_name */
					ML_list_put_val(ram_group[gp_count].associate_list, current_songID,
						temp_list_buf[buf_midID][TEMP_LIST_THISID_COL]);
				}
			} else {
				/* song sorting _should_not_ enter here */
				/* marktable string == insert_name */
				ML_list_put_val(ram_group[gp_count].associate_list, current_songID,
					ram_marktable[gp_count].mark_table_index[mt_midID][MT_NAMEID_COL]);
			}
		}

#ifdef _NEWMUSIC_
		/* install new music list */
		insert_name[0] = g_file_time;	/* YYMMDD:12 bits - Year, 6 bits - Month, 6 - bits Date */
		/* for Jan */
		if ((insert_name[0] & 0x000FC0) == 0x40) {
			/* sort as month 13 */
			insert_name[0] &= 0x00003F;
			insert_name[0] |= ((g_file_time & 0xFFF000) - 0x1000) | (0x340);
		}

		if (cutout_mm < insert_name[0]) {
			if (ram_newmusic.num_of_id == 0x00) {
				i = 0x00;
			} else {
				for (i=0;i<ram_newmusic.num_of_id;i++) {
					if (ram_newmusic.song_date[i] < insert_name[0]) {
						if (ram_newmusic.num_of_id < NUM_OF_SONG_IN_NEW_MUSIC) {
							j = ram_newmusic.num_of_id;
						} else {
							j = NUM_OF_SONG_IN_NEW_MUSIC-1;
						}
						do {
							ram_newmusic.song_date[j] = ram_newmusic.song_date[j-1];
							ram_newmusic.sorted_list[j] = ram_newmusic.sorted_list[j-1];
							j--;
						} while (j>i);
						break;
					}
				}
			}
			if (i < NUM_OF_SONG_IN_NEW_MUSIC) {
				ram_newmusic.song_date[i] = insert_name[0];
				ram_newmusic.sorted_list[i] = current_songID;
				ram_newmusic.num_of_id++;
			}
		}
#endif

	} else if (option == ADD_OPTION_AUDIBLE) {
#ifdef _AUDIBLE_
		for (i=0;i<LEN_OF_FILEPATH_IN_BYTE;i++) {
			flash_path_name[i] = song_info->path_name[i];
#ifdef _SAVENAMESHORTCUT_
			if (ChkTermInWord(flash_path_name[i])) break;
#endif
		}
		if (ML_MemWrite(Media_type, AUDIBLE_PATH_DTYPE, ram_audible.num_of_id, flash_path_name, ML_PATH_NAME_STRUCT_SIZE) != SUCCESS) {
			/* flash access error */
		}

		// Save Audible Long filename
		for (i=0;i<(LEN_OF_LONG_FILENAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD);i++) {
			flash_path_name[i] = song_info->long_filename[i];
			if (flash_path_name[i] == 0x00) {
				i++;
				for (j=i;j<(LEN_OF_LONG_FILENAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD);j++) {
					flash_path_name[j] = 0x00;
				}
				break;
			}
		}
		// TODO: database overflow if 2MB flash DB is used
//		if (ML_MemWrite(Media_type, AUDIBLE_LONG_FILENAME_DTYPE, ram_audible.num_of_id, flash_path_name, ML_LONG_FILENAME_STRUCT_SIZE) != SUCCESS) {
//			/* flash access error */
//		}

		for (i=0;i<LEN_OF_FLASH_NAME_BUF;i++) {
			flash_group_name.name[i] = song_info->song_name[i];
		}
		/* always unicode */
		flash_group_name.unicode = BITSET_NAME_UNICODE;
		if (ML_MemWrite(Media_type, AUDIBLE_NAME_DTYPE, ram_audible.num_of_id, (uint24 _X *)&flash_group_name, ML_SONG_NAME_STRUCT_SIZE) != SUCCESS) {
			/* flash access error */
		}

		if (Media_type == MEDIA_TYPE_FLASH) {
			ML_list_put_val(ram_audible.store_list, ram_audible.num_of_id, ram_audible.num_of_id);
		} else {
			ML_list_put_val(ram_audible.store_list, ram_audible.num_of_id, (MAX_NUM_OF_SONG+ram_audible.num_of_id));
		}
		ram_audible.num_of_id++;
#else
#ifdef _ASSERT_
		assert(0);
#endif
#endif
	} else if (option == ADD_OPTION_VOICE) {
#ifdef _ASSERT_
		/* not implemented */
		assert(0);
#endif
	}	/*   if ((option == ADD_OPTION_MUSIC) || (option == ADD_OPTION_AUDIBLE)) */

#ifdef _PC_DEBUGPRINT_
	printf("\nAddToEntryLibrary() return.\n");
#endif

}

/*================================================================================================*/
#ifdef _USE_FS_SECTAB_
/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:			GetSectorAddress
//
//   Type:
//
//   Description:
//
//   Inputs:		HandleNumber
//
//   Outputs:		Error Code
//
//   Notes:			From the sample code 05/02/28 musicmenu.c
//<
//////////////////////////////////////////////////////////////////////////////////////////
//INT	GetSectorAddress(INT HandleNumber, uint24 is_writefile)
INT	GetSectorAddress(INT HandleNumber, uint24 is_writefile, LONG *SecSize)
{
	extern _reentrant INT Fseek(INT HandleNumber,LONG NumBytesToSeek,INT SeekPosition);
#ifdef _WRITEL2SEC_
	_packed char	FilePath[] = "/:a.2LCES\0\0\0";
#endif
	INT				iFileHandle = -1;
	WORD			wNewLine[4];
	uint24 			current_longnum;
	LONG			sec_num;
	uint24			current_data_sec_num;
	uint24			i;

	INT RetValue=0;
	INT _X *buf;
	LONG FileSize,RemainBytesToRead;
	INT Device;
	LONG BuffOffset=0,BytesToCopy;
	LONG RemainBytesInSector,BytesPerSector;


	//	Seek to the Beginning of the File
	if (Fseek(HandleNumber, 0, SEEK_SET) != FS_SUCCESS)
		return SECTOR_ADD_ERROR_FSEEK;


	Device = Handle[HandleNumber].Device;
	BytesPerSector=MediaTable[Device].BytesPerSector;
	*SecSize = BytesPerSector;
	if (BytesPerSector == BYTE_PER_SECTOR) {
		num_long_per_sec = NUM_OF_LONG_PER_512SEC;
	} else {
		num_long_per_sec = NUM_OF_LONG_PER_2048SEC;
	}
	//	Check whether the File is opened in Read Mode or not
	if((Handle[HandleNumber].Mode & READ_MODE)!=READ_MODE )
	{
		Handle[HandleNumber].ErrorCode = INVALID_MODE;
		return SECTOR_ADD_ERROR_READ_MODE;
	}


	//	Return Error if the Filesize is 0 Byte
	RemainBytesToRead = GetFileSize(HandleNumber);
	if (RemainBytesToRead==0)
	{
		Handle[HandleNumber].ErrorCode = FS_EOF;
		return SECTOR_ADD_ERROR_FILE_SIZE;
	}

	if (is_writefile) {
		//	Open a File to Save the Output
		//SysLoadFATWrite();
		//iFileHandle = Fopen(FilePath, (_packed char *)"w");
		//iFileHandle = FSFileOpen((_packed BYTE *) g_strMusicLib_SecTable_FilePath, WRITE, MUSICLIB_STDRIVE_NUMBER);
		iFileHandle = Fopen((_packed char *) g_strMusicLib_SecTable_FilePath, (_packed char *)"w");
		if (iFileHandle < 0)
		{
			//SysUnLoadFATWrite();
			return SECTOR_ADD_ERROR_OUTPUT;
		}
	}
#ifdef _WRITEL2SEC_
	else {
		iFileHandle = Fopen(FilePath, (_packed char *)"w");
	}
#endif
	wNewLine[0] = 0x00;
	wNewLine[1] = 0x00;
	wNewLine[2] = 0x00;
	wNewLine[3] = 0x00;
	current_data_sec_num = 0x00;
	current_longnum = 0x00;
	sec_num = 0x00;

	RemainBytesInSector = BytesPerSector - Handle[HandleNumber].BytePosInSector;
	while(RemainBytesToRead >0)
	{
		if ((RemainBytesInSector != 0) && (RemainBytesToRead > RemainBytesInSector))
		{
			BytesToCopy	= RemainBytesInSector;
			RemainBytesInSector = 0;
		}
		else
		{
			BytesToCopy = RemainBytesToRead;
			if(BytesToCopy > BytesPerSector)
			{
				BytesToCopy = BytesPerSector;
			}
		}

		if ((RetValue = UpdateHandleOffsets(HandleNumber)))
		{
			Handle[HandleNumber].ErrorCode = RetValue;
			if (is_writefile) {
				Fclose(iFileHandle);
				//SysUnLoadFATWrite();
			}
			return SECTOR_ADD_ERROR_INCOMPLETE;
		}

		RemainBytesToRead -= BytesToCopy;
		Handle[HandleNumber].CurrentOffset += BytesToCopy;
		Handle[HandleNumber].BytePosInSector += BytesToCopy;

		BuffOffset+=BytesToCopy;

		if (is_writefile) {
			//FSFileWrite(LONG_SIZE_IN_BYTE, iFileHandle, Y_MEMORY, -1, (WORD *)&Handle[HandleNumber].CurrentSector);
			//FSFileWrite(10, iFileHandle, Y_MEMORY, -1, (WORD *)wNewLine);
			Fwrite(iFileHandle, (INT *)&Handle[HandleNumber].CurrentSector, LONG_SIZE_IN_BYTE, Y_MEMORY, -1);
			//Fwrite(iFileHandle, (INT *)wNewLine, 10, Y_MEMORY, -1);
#ifdef _USE_16MDB_
			if ((++current_longnum) >= NUM_OF_LONG_PER_512SEC) {
#else
			current_data_sec_num++;
			current_longnum++;
			if (((current_longnum >= NUM_OF_LONG_PER_512SEC) && (BytesPerSector == BYTE_PER_SECTOR))
				|| ((current_longnum >= NUM_OF_LONG_PER_2048SEC) && (BytesPerSector == LB_BYTE_PER_SECTOR))) {
#endif
					Fwrite(iFileHandle, (INT *)wNewLine, 2, Y_MEMORY, -1);
					wNewLine[0]++;
					current_longnum = 0x00;
#ifdef _USE_16MDB_
					Fwrite(iFileHandle, (INT *)sec_temp_buf_X, 512, X_MEMORY, -1);
					Fwrite(iFileHandle, (INT *)sec_temp_buf_X, 512, X_MEMORY, -1);
					Fwrite(iFileHandle, (INT *)sec_temp_buf_X, 512, X_MEMORY, -1);
#endif
			}
#ifndef _USE_16MDB_
			if ((current_data_sec_num <= HASH_DB_START) && (BytesPerSector == LB_BYTE_PER_SECTOR)) {
				for(i=0;i<3;i++) {
					Fwrite(iFileHandle, (INT *)&Handle[HandleNumber].CurrentSector, LONG_SIZE_IN_BYTE, Y_MEMORY, -1);
					//Fwrite(iFileHandle, (INT *)&current_longnum, 6, Y_MEMORY, -1);
					current_data_sec_num++;
					current_longnum++;
					if (((current_longnum >= NUM_OF_LONG_PER_512SEC) && (BytesPerSector == BYTE_PER_SECTOR))
						|| ((current_longnum >= NUM_OF_LONG_PER_2048SEC) && (BytesPerSector == LB_BYTE_PER_SECTOR))) {
							//if (current_longnum >= 341) {
							Fwrite(iFileHandle, (INT *)wNewLine, 2, Y_MEMORY, -1);
							wNewLine[0]++;
							current_longnum = 0x00;
					}
				}
			}
#endif
#ifdef _USE_16MDB_
			if ((++sec_num) == SEC_TABLE_FLASHPART_SIZE_IN_LONG) {
				Fwrite(iFileHandle, (INT *)temp_queue, BYTE_FILL_INTERDB_GAP, Y_MEMORY, -1);
#else
			if ((((++sec_num) == SEC_TABLE_FLASHPART_SIZE_IN_LONG) && (BytesPerSector == BYTE_PER_SECTOR))
				|| ((current_data_sec_num == (DATABASE_SIZE/2)) && (BytesPerSector == LB_BYTE_PER_SECTOR))) {
					if (BytesPerSector == BYTE_PER_SECTOR) {
						Fwrite(iFileHandle, (INT *)temp_queue, BYTE_FILL_INTERDB_GAP, Y_MEMORY, -1);
					} else {
						Fwrite(iFileHandle, (INT *)temp_queue, BYTE_FILL_INTERDB_GAP_2048, Y_MEMORY, -1);
					}
#endif
					sec_num = 0x00;
					wNewLine[0] = 0x00;;
					current_longnum = 0x00;
#ifdef _USE_16MDB_
					Fwrite(iFileHandle, (INT *)sec_temp_buf_X, 512, X_MEMORY, -1);
					Fwrite(iFileHandle, (INT *)sec_temp_buf_X, 512, X_MEMORY, -1);
					Fwrite(iFileHandle, (INT *)sec_temp_buf_X, 512, X_MEMORY, -1);
#endif
			}
			//SysWaitOnEvent(0, 0, 0);
		} else {
#ifdef _WRITEL2SEC_
			Fwrite(iFileHandle, (INT *)&Handle[HandleNumber].CurrentSector, LONG_SIZE_IN_BYTE, Y_MEMORY, -1);
			Fwrite(iFileHandle, (INT *)wNewLine, 10, Y_MEMORY, -1);
#endif
			l2_sector_table[current_longnum++] = Handle[HandleNumber].CurrentSector;

#ifdef _USE_16MDB_
			if (current_longnum > NUM_OF_LEVEL2_SEC) {
#else
			if (((current_longnum > NUM_OF_LEVEL2_SEC) && (BytesPerSector == BYTE_PER_SECTOR))
				|| ((current_longnum > NUM_OF_LEVEL2_2048SEC) && (BytesPerSector == LB_BYTE_PER_SECTOR))) {
#endif
					// error case
#if 1
					return SECTOR_ADD_ERROR_INCOMPLETE;
#else
					_asm("debug");
#endif
			}
		}
	}


	if (is_writefile) {
#ifndef _USE_16MDB_
		if (BytesPerSector == LB_BYTE_PER_SECTOR) {
			Fwrite(iFileHandle, (INT *)temp_queue, 2556, Y_MEMORY, -1);
		}
#endif
		Fclose(iFileHandle);
		//SysUnLoadFATWrite();
	}
#ifdef _WRITEL2SEC_
	else {
		Fclose(iFileHandle);
	}
#endif
	return SECTOR_ADD_SUCCESS;
}

// ram sec_table only
INT ML_Buildl2SecTable(void) {
	INT				iFileHandle = -1;
	int				ret_code;

	//iFileHandle = Fopen(g_strMusicLib_SecTable_FilePath, (_packed char *)"r");
	iFileHandle = FSFileOpen(g_strMusicLib_SecTable_FilePath,READ,MUSICLIB_STDRIVE_NUMBER);
	if (iFileHandle < 0) {
		/* file access error */
#ifndef _PC_SIMULATION_
		_asm("debug");
#endif
		return SECTOR_ADD_ERROR_INCOMPLETE;
	}
#ifdef _USE_16MDB_
	ret_code = GetSectorAddress(iFileHandle, FALSE, &SectorSize);
#else
	ret_code = GetSectorAddress(iFileHandle, FALSE, &SectorSize);
	if ((SectorSize != BYTE_PER_SECTOR) && (SectorSize != LB_BYTE_PER_SECTOR)) {
		/* sector size unknown, can't continue. */
		_asm("debug");
	}
#endif
	Fclose(iFileHandle);

	return ret_code;
}


INT ML_BuildSecTableFile(void) {
	// Siukoon 2005-03-01
#if 0
	//g_iFileHandle = Fopen(g_strMusicLib_FilePath, (_packed char *)"w");
	g_iFileHandle = FSFileOpen(g_strMusicLib_FilePath,WRITE,MUSICLIB_STDRIVE_NUMBER);
#else
	//g_iFileHandle = Fopen(g_strMusicLib_FilePath, (_packed char *)"r");
	g_iFileHandle = FSFileOpen(g_strMusicLib_FilePath,READ,MUSICLIB_STDRIVE_NUMBER);
#endif
	if (g_iFileHandle < 0) {
		/* file access error */
#ifndef _PC_SIMULATION_
		_asm("debug");
#endif
		return SECTOR_ADD_ERROR_INCOMPLETE;
	} else {
		/* file open success */
#ifdef _USE_FS_SECTAB_
#ifdef _USE_16MDB_
		GetSectorAddress(g_iFileHandle, TRUE, &SectorSize);
#else
		GetSectorAddress(g_iFileHandle, TRUE, &SectorSize);
		if ((SectorSize != BYTE_PER_SECTOR) && (SectorSize != LB_BYTE_PER_SECTOR)) {
			/* sector size unknown, can't continue. */
			_asm("debug");
		}
#endif
		Fclose(g_iFileHandle);
		g_iFileHandle = -1;

		ML_Buildl2SecTable();
#endif
	}
	return SECTOR_ADD_SUCCESS;
}

#endif /* _USE_FS_SECTAB_ */

/*==================================================================================================
FUNCTION: ML_MusicLibInit()

DESCRIPTION:
Description of this specific function.

Called by UI, to initialize the parameters before insert any item.

ARGUMENTS PASSED:
void

RETURN VALUE:
void

PRE-CONDITIONS:
Call only once before insert items.

POST-CONDITIONS:
None

IMPORTANT NOTES:
None

==================================================================================================*/
void ML_InitLibraryParameter(void) {
	uint24 i;
	uint24 j;
	//uint24 k;

	for (i=0;i<MAX_NUM_OF_GROUP;i++) {
		MarkTableID[i] = 0;
		num_of_item[i] = 0;
	}

	group_dtype[0] = ARTIST_NAME_DTYPE;
	group_dtype[1] = ALBUM_NAME_DTYPE;
	group_dtype[2] = GENRE_NAME_DTYPE;
	group_dtype[3] = SONG_NAME_DTYPE;

	temp_list_buf[0][TEMP_LIST_PREID_COL] = INDEX_EOF;
	temp_list_buf[0][TEMP_LIST_THISID_COL] = INDEX_EOF;
	temp_list_buf[0][TEMP_LIST_NXID_COL] = INDEX_EOF;

	for (j=0;j<MAX_NUM_OF_GROUP;j++) {
		ram_group[j].sorted_list_hd = INDEX_EOF;
		for (i=0;i<NUM_OF_MARK_TABLE_ENTRY;i++) {
			ram_marktable[j].mark_table_string[i][0] = EOSTR; /* '\0' */
			ram_marktable[j].mark_table_index[i][MT_NAMEID_COL] = INDEX_EOF;
			ram_marktable[j].mark_table_index[i][MT_COUNT_COL] = 0;
		}
	}

#ifdef _NEWMUSIC_
	ram_newmusic.num_of_id = 0x00;
	ram_newmusic.num_of_1day_song = 0x00;
	ram_newmusic.num_of_1week_song = 0x00;
	ram_newmusic.num_of_1month_song = 0x00;
	for (i=0;i<NUM_OF_SONG_IN_NEW_MUSIC;i++) {
		ram_newmusic.song_date[i] = 0x00;
		ram_newmusic.sorted_list[i] = 0x00;
	}
	GetCutoutTime();
#endif

#ifdef _AUDIBLE_
	ram_audible.num_of_id = 0x00;
#endif

#ifdef _PC_SIMULATION_
	ML_MediaDetail();
#endif
}

#ifdef _NEWMUSIC_
/*================================================================================================*/
#if 1
void ML_UpdateNewMusic(void) {
	uint24 i;
	uint24 j;
	uint24 k;
	uint24 temp_time;
	uint24 temp_id;
	uint24 *newmusic_p_Y;
	GetCutoutTime();
#if 0
#ifdef _USE_FS_MUSICLIB_
	// Siukoon 2005-03-01
	g_iFileHandle = FSFileOpen(g_strMusicLib_FilePath,WRITE,MUSICLIB_STDRIVE_NUMBER);
	if (g_iFileHandle < 0) {
		/* file access error */
#ifndef _PC_SIMULATION_
		_asm("debug");
#endif
	} else {
		/* file open success */
	}
#endif
#endif

	newmusic_p_Y = (uint24 *) &ram_flash_newmusic;
	if (ML_MemRead(MEDIA_TYPE_FLASH, NEW_MUSIC_DTYPE, NEW_MUSIC_STRUCT_SIZE, newmusic_p_Y, NEW_MUSIC_STRUCT_SIZE) != SUCCESS) {
		/* flash access error */
		return;
	}
	if (g_is_SD_inserted == HAS_SD) {
		newmusic_p_Y = (uint24 *) &ram_sd_newmusic;
		if (ML_MemRead(MEDIA_TYPE_SD, NEW_MUSIC_DTYPE, NEW_MUSIC_STRUCT_SIZE, newmusic_p_Y, NEW_MUSIC_STRUCT_SIZE) != SUCCESS) {
			/* flash access error */
			return;
		}
	} else {
		ram_sd_newmusic.num_of_id = 0x00;
	}

	ram_newmusic.num_of_1day_song = 0x00;
	ram_newmusic.num_of_1week_song = 0x00;
	ram_newmusic.num_of_1month_song = 0x00;

	/* you _must_read_ ram_newmusic.num_of_id first */
	ram_newmusic.num_of_id = ram_flash_newmusic.num_of_id + ram_sd_newmusic.num_of_id;
	if (ram_newmusic.num_of_id > NUM_OF_SONG_IN_NEW_MUSIC) {
		ram_newmusic.num_of_id = NUM_OF_SONG_IN_NEW_MUSIC;
	}
	i = 0;
	j = 0;
	k = 0;

	//i<ram_newmusic.num_of_id
	do {
		if (g_is_SD_inserted == HAS_SD) {
			if (ram_flash_newmusic.song_date[i] < ram_sd_newmusic.song_date[j]) {
				temp_time = ram_sd_newmusic.song_date[j];
				temp_id = ram_sd_newmusic.sorted_list[j++]+MAX_NUM_OF_SONG;
			} else {
				temp_time = ram_flash_newmusic.song_date[i];
				temp_id = ram_flash_newmusic.sorted_list[i++];
			}
		} else {
			temp_time = ram_flash_newmusic.song_date[i];
			temp_id = ram_flash_newmusic.sorted_list[i++];
		}
		ram_newmusic.song_date[k] = temp_time;
		ram_newmusic.sorted_list[k++] = temp_id;
		if (temp_time > cutout_dd) {
			if (ram_newmusic.num_of_1day_song < NUM_OF_SONG_IN_NEW_MUSIC_DAY) {
				ram_newmusic.num_of_1day_song++;
			}
		}
		if (temp_time > cutout_ww) {
			if (ram_newmusic.num_of_1week_song < NUM_OF_SONG_IN_NEW_MUSIC_WEEK) {
				ram_newmusic.num_of_1week_song++;
			}
		}
		if (temp_time > cutout_mm) {
			if (ram_newmusic.num_of_1month_song < NUM_OF_SONG_IN_NEW_MUSIC_MONTH) {
				ram_newmusic.num_of_1month_song++;
			}
		}
	} while (k < NUM_OF_SONG_IN_NEW_MUSIC);
	newmusic_p_Y = (uint24 *) &ram_flash_newmusic;
	for (i=0;i<NEW_MUSIC_STRUCT_SIZE;i++) {	/* merged_newmusic is at 44word offset */
		temp_queue[i] = newmusic_p_Y[i];	/* temp_queue is 1000word, newmusic struct is 44 word 44*2=88word */
	}
	j = 0;
	newmusic_p_Y = (uint24 *) &ram_newmusic;
	for (i=NEW_MUSIC_STRUCT_SIZE;i<(NEW_MUSIC_STRUCT_SIZE*2);i++) {	/* merged_newmusic is at 44word offset */
		temp_queue[i] = newmusic_p_Y[j++];	/* temp_queue is 1000word, newmusic struct is 44 word 44*2=88word */
	}
	if (ML_MemWrite(MEDIA_TYPE_FLASH, NEW_MUSIC_DTYPE, NEW_MUSIC_STRUCT_SIZE, temp_queue, NEW_MUSIC_STRUCT_SIZE) != SUCCESS) {
		/* flash access error */
	}
}

#else
/* not in use */
/* This version only update the list of 'Media_type' */
void ML_UpdateNewMusic(uint24 Media_type) {
	uint24 i;
	uint24 *newmusic_p_Y;

	GetCutoutTime();
	newmusic_p_Y = (uint24 *) &ram_newmusic;
	if (ML_MemRead(Media_type, NEW_MUSIC_DTYPE, NEW_MUSIC_STRUCT_SIZE, newmusic_p_Y, NEW_MUSIC_STRUCT_SIZE) != SUCCESS) {
		/* flash access error */
		return;
	}

	ram_newmusic.num_of_1day_song = 0x00;
	ram_newmusic.num_of_1week_song = 0x00;
	ram_newmusic.num_of_1month_song = 0x00;

	/* you _must_read_ ram_newmusic.num_of_id first */
	for (i=0;i<ram_newmusic.num_of_id;i++) {
		if (ram_newmusic.song_date[i] > cutout_dd) {
			ram_newmusic.num_of_1day_song++;
		}
		if (ram_newmusic.song_date[i] > cutout_ww) {
			ram_newmusic.num_of_1week_song++;
		}
		if (ram_newmusic.song_date[i] > cutout_mm) {
			ram_newmusic.num_of_1month_song++;
		}
	}
	/* new music */
	for (i=0;i<NEW_MUSIC_STRUCT_SIZE;i++) {
		temp_queue[i] = newmusic_p_Y[i];
	}
	if (ML_MemWrite(Media_type, NEW_MUSIC_DTYPE, NEW_MUSIC_STRUCT_SIZE, temp_queue, NEW_MUSIC_STRUCT_SIZE) != SUCCESS) {
		/* flash access error */
	}
}
/* This version only update the list of 'Media_type' */
#endif
#endif	/* _NEWMUSIC_ */

#ifdef _ONTHEGO_
void ML_ChkOnTheGo(uint24 Media_type, uint24 _X * temp_path, uint24 newID) {
	uint24 i;
	uint24 j;
	uint24 k;
	BOOL eq;

	for (i=0;i<on_the_fly_temp_list_Y.num_of_id;i++) {
		eq = TRUE;
		if ((on_the_fly_list.store_list[i] == INDEX_EOF)
			&& (((on_the_fly_temp_list_Y.store_list[i] >= MAX_NUM_OF_SONG) && (Media_type != MEDIA_TYPE_FLASH))
			|| ((on_the_fly_temp_list_Y.store_list[i] < MAX_NUM_OF_SONG) && (Media_type == MEDIA_TYPE_FLASH)))) {

				for (k=0;k<(LEN_OF_FILEPATH_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD);k++) {
					if (temp_path[k] != OnTheGo_path[i][k]) {
						eq = FALSE;
						//					break;
					}
				}
				if (eq) {
					if (Media_type == MEDIA_TYPE_FLASH) {
						on_the_fly_temp_list_Y.store_list[i] = newID;
						on_the_fly_list.store_list[i] = TRUE;
					} else {
						on_the_fly_temp_list_Y.store_list[i] = newID+MAX_NUM_OF_SONG;
						on_the_fly_list.store_list[i] = ONTHEGO_SDCHK;
					}
					return;
				}
		}
	}
}

void ML_UpdateOnTheGo(void) {
	uint24 i;
	uint24 r;
	/*
	uint24 _X *on_the_fly_p_X;
	uint24 *on_the_fly_p_Y;
	*/
	i = 0x0;
	r = 0x0;
	on_the_fly_list.num_of_id = 0x0;
	while (i < on_the_fly_temp_list_Y.num_of_id) {
		while (on_the_fly_list.store_list[i] == INDEX_EOF) {
			i++;
			if (i >= on_the_fly_temp_list_Y.num_of_id) { return; }
		}
		on_the_fly_list.store_list[r] = on_the_fly_temp_list_Y.store_list[i];
		/*
		for (k=0;k<(LEN_OF_FILEPATH_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD);k++) {
		OnTheGo_path[r][k] = OnTheGo_path[i][k];
		}
		*/
		r++;
		i++;
		on_the_fly_list.num_of_id++;
	}
	on_the_fly_temp_list_Y.num_of_id = 0x0;
	/*
	on_the_fly_p_Y = (uint24 *)&on_the_fly_temp_list_Y;
	on_the_fly_p_X = (uint24 _X *)&on_the_fly_list;
	for (i=0;i<ON_THE_FLY_STRUCT_SIZE;i++) {
	on_the_fly_p_X[i] = on_the_fly_p_Y[i];
	}
	*/
}

void ML_ChOnTheGo(uint24 Media_type, uint24 value) {
	uint24 i;

	for (i=0;i<on_the_fly_temp_list_Y.num_of_id;i++) {
		if (((on_the_fly_temp_list_Y.store_list[i] < MAX_NUM_OF_SONG) && (Media_type == MEDIA_TYPE_FLASH)) || ((on_the_fly_temp_list_Y.store_list[i] >= MAX_NUM_OF_SONG) && (Media_type != MEDIA_TYPE_FLASH))) {
			on_the_fly_list.store_list[i] = value;
		}
	}
}

//void ML_LoadOnTheGo(uint24 HasSD) {
void ML_LoadOnTheGo(uint24 IsColdBoot) {
	uint24 i;
	uint24 j;
	uint24 k;
	uint24 id;
	uint24 Media_type;
	uint24 _X *on_the_fly_p_X;
	uint24 *on_the_fly_p_Y;

	on_the_fly_p_Y = (uint24 *)&on_the_fly_temp_list_Y;
	on_the_fly_p_X = (uint24 _X *)&on_the_fly_list;
	if (IsColdBoot) {
		ML_MemRead(MEDIA_TYPE_FLASH, ON_THE_FLY_DTYPE, ON_THE_FLY_STRUCT_SIZE, on_the_fly_p_Y, ON_THE_FLY_STRUCT_SIZE);

		for (i=0;i<ON_THE_FLY_STRUCT_SIZE;i++) {
			on_the_fly_p_X[i] = on_the_fly_p_Y[i];
		}
		for (i=0;i<on_the_fly_temp_list_Y.num_of_id;i++) {
			ML_MemRead(MEDIA_TYPE_FLASH, ON_THE_FLY_PATH_DTYPE, i, temp_name_buf, ML_PATH_NAME_STRUCT_SIZE);
			for (j=0;j<(LEN_OF_FILEPATH_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD);j++) {
				OnTheGo_path[i][j] = temp_name_buf[j];
				/*
				if (ChkTermInWord(OnTheGo_path[i][j])) {
				if (((OnTheGo_path[i][j] & 0x00FF00) == 0x00)
				|| ((OnTheGo_path[i][j] & 0xFF0000) == 0x00)) {
				OnTheGo_path[i][j] = OnTheGo_path[i][j] & 0x00FFFF;
				j++;
				}
				for (k=j;k<(LEN_OF_FILEPATH_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD);k++) {
				OnTheGo_path[i][k] = 0x00;
				}
				break;
				}
				*/
			}
		}
	} else {
		for (i=0;i<ON_THE_FLY_STRUCT_SIZE;i++) {
			on_the_fly_p_Y[i] = on_the_fly_p_X[i];
		}
		for (i=0;i<on_the_fly_temp_list_Y.num_of_id;i++) {
			id = on_the_fly_temp_list_Y.store_list[i];
			if (id >= MAX_NUM_OF_SONG) {
				id -= MAX_NUM_OF_SONG;
				Media_type = MEDIA_TYPE_SD;
			} else {
				Media_type = MEDIA_TYPE_FLASH;
			}
			ML_MemRead(Media_type, PATH_NAME_DTYPE, id, temp_name_buf, ML_PATH_NAME_STRUCT_SIZE);
			for (j=0;j<(LEN_OF_FILEPATH_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD);j++) {
				OnTheGo_path[i][j] = temp_name_buf[j];
				/*
				if (ChkTermInWord(OnTheGo_path[i][j])) {
				if (((OnTheGo_path[i][j] & 0x00FF00) == 0x00)
				|| ((OnTheGo_path[i][j] & 0xFF0000) == 0x00)) {
				OnTheGo_path[i][j] = OnTheGo_path[i][j] & 0x00FFFF;
				j++;
				}
				for (k=j;k<(LEN_OF_FILEPATH_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD);k++) {
				OnTheGo_path[i][k] = 0x00;
				}
				break;
				}
				*/
			}
		}
	}

	/*
	for (i=i;i<NUM_OF_SONG_IN_ON_THE_FLY;i++) {
	OnTheGo_path[i][0] = 0x0;
	}
	*/
}

// todo  ADD A FUNCTION HEADER TO EVERY FUNCTION THAT DOES NOT HAVE ONE!
// 
void ML_SaveOnTheGo(void) {
	uint24 _X *on_the_fly_p_X;
	uint24 i;
	uint24 j;
	uint24 k;
	uint24 id;
	uint24 Media_type;

	for (i=0;i<on_the_fly_list.num_of_id;i++) {
		id = on_the_fly_list.store_list[i];
		if (id >= MAX_NUM_OF_SONG) {
			id -= MAX_NUM_OF_SONG;
			Media_type = MEDIA_TYPE_SD;
		} else {
			Media_type = MEDIA_TYPE_FLASH;
		}
		ML_MemRead(Media_type, PATH_NAME_DTYPE, id, insert_name, ML_PATH_NAME_STRUCT_SIZE);

		for (j=0;j<(LEN_OF_FILEPATH_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD);j++) {
			OnTheGo_path[i][j] = insert_name[j];
			if (ChkTermInWord(OnTheGo_path[i][j])) {
				if ((OnTheGo_path[i][j] & 0x0000FF) == 0x00) {
					OnTheGo_path[i][j] = OnTheGo_path[i][j] & 0x000000;
				} else if ((OnTheGo_path[i][j] & 0x00FF00) == 0x00) {
					OnTheGo_path[i][j] = OnTheGo_path[i][j] & 0x0000FF;
				} else {
					OnTheGo_path[i][j] = OnTheGo_path[i][j] & 0x00FFFF;
				}
				j++;
				for (k=j;k<(LEN_OF_FILEPATH_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD);k++) {
					OnTheGo_path[i][k] = 0x00;
				}
				break;
			}
		}
		ML_MemWrite(MEDIA_TYPE_FLASH, ON_THE_FLY_PATH_DTYPE, i, OnTheGo_path[i], ML_PATH_NAME_STRUCT_SIZE);
	}
	on_the_fly_p_X = (uint24 _X *)&on_the_fly_list;
	ML_MemWrite(MEDIA_TYPE_FLASH, ON_THE_FLY_DTYPE, ON_THE_FLY_STRUCT_SIZE, on_the_fly_p_X, ON_THE_FLY_STRUCT_SIZE);
}
#endif	/* #ifdef _ONTHEGO_ */

/*================================================================================================*/

#ifdef _PC_SIMULATION_
extern uint24 ML_GetItemName( uint24 id, uint24 memory_type, uint24 item_type, FLASH_GROUP_NAME_T *itemStruct );

extern uint24 ML_GetPathName( uint24 id, uint24 memory_type, uint24 item_type, uint24 *pathname );

/* id = song ID */
extern uint24 ML_GetSongYear( uint24 id, uint24 memory_type, uint24 *year );

extern uint24 ML_GetLvlOneItemSortListSize( uint24 memory_type, uint24 item_type, uint24 *size );

/* sortedListIndex = { 0..num_item_in_sorted_list-1 }, ID = output the item ID */
//extern uint24 ML_GetLvlOneItemID( uint24 memory_type, uint24 item_type, uint24 sortedListIndex, uint24 *ID );

/* get the number of album in sorted list under particular Artist */
extern uint24 ML_GetFlashArtistAlbumSortListSize( uint24 memory_type, uint24 artistID, uint24 *size );

/* get the album ID under particular Artist */
/* sortedListIndex = { 0..num_item_in_sorted_list-1 }, ID = output the item ID */
//extern uint24 ML_GetFlashArtistAlbumID( uint24 memory_type, uint24 artistID, uint24 sortedListIndex, uint24 *ID );

/* get the number of song in sorted list under particular Artist and Album */
//extern uint24 ML_GetFlashArtistAlbumSongSortListSize( uint24 memory_type, uint24 artistID, uint24 albumID, uint24 *size );

/* get the song ID under particular Artist and Album*/
/* sortedListIndex = { 0..num_item_in_sorted_list-1 }, ID = output the item ID */
//extern uint24 ML_GetFlashArtistAlbumSongID( uint24 memory_type, uint24 artistID, uint24 albumID, uint24 sortedListIndex, uint24 *ID );

/* get the number of song in sorted list under particular Album, Genre and Song Year */
//extern uint24 ML_GetLvlTwoSongSortListSize( uint24 memory_type, uint24 id, uint24 item_type, uint24 *size );

/* get the song ID under particular Album, Genre and Song Year */
/* sortedListIndex = { 0..num_item_in_sorted_list-1 }, ID = output the item ID */
//extern uint24 ML_GetLvlTwoSongID( uint24 memory_type, uint24 lvl_id, uint24 item_type, uint24 sortedListIndex, uint24 *ID );

// Get total number of song under particular Artist
extern uint24 _reentrant GetArtistSongSize( uint24 memory_type, uint24 artistID, uint24 *size );

// Get song ID by index under particular Artist
// sortedListIndex {0..total_num_song under Artist-1}
//extern uint24 _reentrant GetArtistSongID( uint24 memory_type, uint24 artistID, uint24 sortedListIndex, uint24 *id );

/* song_info->song_name, song_info->path_name, song_info->track_number are _not_filled_ */
//extern uint24 _reentrant ML_GetMLSongInfo(uint24 memory_type, uint24 song_id, RAM_SONG_INFO_T *song_info);
extern uint24 _reentrant ML_ConstructSongStruct(uint24 song_id, uint24 memory_type, RAM_SONG_INFO_T *song_info);

/*==================================================================================================
FUNCTION: MLT_songbrowse()

DESCRIPTION:
Description of this specific function.

Called by test UI, to browse sorted song tree.

ARGUMENTS PASSED:

RETURN VALUE:

PRE-CONDITIONS:

POST-CONDITIONS:

IMPORTANT NOTES:
None

==================================================================================================*/
COMP_OPT void MLT_songbrowse(uint24 Media_type) {
	uint24 i;
	uint24 j;
	uint24 k;
	uint24 r;
	//uint24 s;
	//uint24 t;
	uint24 u;
	uint24 m;
	uint24 n;

	//RAM_SONG_INFO_T song_info;
	FLASH_GROUP_NAME_T nameitem;
	uint24 pathname[LEN_OF_FILEPATH_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD];
	//uint24 sorted_list[MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD];
	//uint24 size;

	printf("MarkTableID[3]=%d", MarkTableID[3]);

	for (i=0;i<MarkTableID[3];i++) {
		printf("\nram_song_marktable.mark_table_index[%d]=", i);
		printf("%X", ram_marktable[SONG_SORTLOOP].mark_table_index[i][MT_NAMEID_COL]);
		printf("Ct=%X\tSubStr=", ram_marktable[SONG_SORTLOOP].mark_table_index[i][MT_COUNT_COL]);
		for (j=0;j<LEN_OF_MTSTRING;j++) {
			printf("%X:", ram_marktable[SONG_SORTLOOP].mark_table_string[i][j]);
		}
	}

	ML_MemRead(Media_type, NUM_OF_SONG_DTYPE, NUM_OF_SONG_DTYPE, (uint24 *) &num_of_item_y[SONG_SORTLOOP], ML_VALUE_STRUCT_SIZE);
	printf("\nnum_of_song=%d", num_of_item_y[SONG_SORTLOOP]);
	printf("\nSorted ram_song_name(compared string)=");
	for (i=0;i<num_of_item_y[SONG_SORTLOOP];i++) {
		ML_MemRead(Media_type, SONG_ARRAY_DTYPE, i, (uint24 *) &j, ML_INDEX_STRUCT_SIZE);
		printf("\nID=%d\t", j);
		ML_MemRead(Media_type, SONG_NAME_DTYPE, j, (uint24 *) &flash_group_name, ML_SONG_NAME_STRUCT_SIZE);
		ML_cpy_flash_group_name((FLASH_GROUP_NAME_T *) &flash_group_name_y, (FLASH_GROUP_NAME_T _X *) &flash_group_name);
		ML_extract_string(flash_group_name_y.unicode,
			temp_name_buf,
			flash_group_name_y.name,
			LEN_OF_NAME_IN_BYTE);
		printf("[");
		for (k=0;k<LEN_OF_NAME_IN_BYTE;k++) {
			printf("%c", temp_name_buf[k]);
#ifdef _SAVENAMESHORTCUT_
			if (temp_name_buf[k] == 0) break;
#endif
		}
		printf("]");
	}
	ML_MemRead(Media_type, NUM_OF_YEAR_DTYPE, NUM_OF_YEAR_DTYPE, (uint24 *) &num_of_item_y[4], ML_VALUE_STRUCT_SIZE);
	printf("\nnum_of_year=%d", num_of_item_y[4]);
	printf("\nSorted ram_song_year(compared string)=");
	for (i=0;i<num_of_item[4];i++) {
		ML_MemRead(Media_type, YEAR_SORT_DTYPE, i, (uint24 *) &j, ML_INDEX_STRUCT_SIZE);
		printf("\nID=%d\t", j);
		ML_MemRead(Media_type, SONG_YEAR_DTYPE, j, (uint24 *) &flash_group_name, ML_SONG_NAME_STRUCT_SIZE);
		printf("[%d]", flash_group_name.name[0]);
	}
	printf("\nTest browse interface functions:");
	ML_GetLvlOneItemSortListSize(0, ITEM_TRACK, &j);
	printf("\nML_GetLvlOneItemSortListSize(0, ITEM_TRACK, j)=%d", j);
	for (i=0;i<j;i++) {
		ML_GetLvlOneItemID(0, ITEM_TRACK, i, &k);
		printf("\nML_GetLvlOneItemID(0, ITEM_TRACK, i, k)=%d", k);
		printf("\nID=%d\t", k);
		ML_GetItemName(k, 0, ITEM_TRACK, &nameitem);
		printf("ML_GetItemName()=");
		for (k=0;k<LEN_OF_FLASH_NAME_BUF;k++) {
			printf("[%X]", nameitem.name[k]);
#ifdef _SAVENAMESHORTCUT_
			if (nameitem.name[k] == 0) break;
#endif
		}
		ML_GetPathName( i, 0, ITEM_TRACK, pathname);
		printf("\npathname[]=");
		for (k=0;k<LEN_OF_FILEPATH_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD;k++) {
			printf("[%X]", pathname[k]);
#ifdef _SAVENAMESHORTCUT_
			if (pathname[k] == 0) break;
#endif
		}
	}

	/* artist */
	ML_GetLvlOneItemSortListSize(0, ITEM_ARTIST, &j);
	printf("\nML_GetLvlOneItemSortListSize(0, ITEM_ARTIST, j)=%d", j);
	for (i=0;i<j;i++) {
		ML_GetFlashArtistAlbumSortListSize(0, i, &k);
		printf("\nML_GetFlashArtistAlbumSortListSize(0, i, &k)=%d", k);
		printf("\nOrderID=%d\t", i);
		ML_GetItemName(i, 0, ITEM_ARTIST, &nameitem);
		printf("ML_GetItemName()=");
		for (r=0;r<LEN_OF_FLASH_NAME_BUF;r++) {
			printf("[%X]", nameitem.name[r]);
#ifdef _SAVENAMESHORTCUT_
			if (nameitem.name[r] == 0) break;
#endif
		}
#if 0
		/* sub list */
		for (r=0;r<k;r++) {
			ML_GetFlashArtistAlbumID(0, i, r, &t);
			printf("\n\tML_GetFlashArtistAlbumID(0, i, r, &t)=%d", t);
			ML_GetFlashArtistAlbumSongSortListSize(0, i, t, &n);
			printf("\n\tML_GetFlashArtistAlbumSongSortListSize(0, i, t, &n)=%d", n);
			for (m=0;m<n;m++) {
				ML_GetFlashArtistAlbumSongID(0, i, t, m, &s);
				printf("\n\t\tML_GetFlashArtistAlbumSongID(0, i, t, m, &s)=%d", s);
				printf("\n\t\tSubOrderID=%d\t", m);
				ML_GetItemName(s, 0, ITEM_TRACK, &nameitem);
				printf("ML_GetItemName(ITEM_TRACK)=");
				for (u=0;u<LEN_OF_FLASH_NAME_BUF;u++) {
					printf("[%X]", nameitem.name[u]);
#ifdef _SAVENAMESHORTCUT_
					if (nameitem.name[u] == 0) break;
#endif
				}
			}
		}
#endif
	}

	/* album */
	ML_GetLvlOneItemSortListSize(0, ITEM_ALBUM, &j);
	printf("\nML_GetLvlOneItemSortListSize(0, ITEM_ALBUM, j)=%d", j);
	for (i=0;i<j;i++) {
		ML_GetLvlOneItemID(0, ITEM_ALBUM, i, &k);
		printf("\nML_GetLvlOneItemID(0, ITEM_ALBUM, i, k)=%d", k);
		printf("\nOrderID=%d\t", i);
		ML_GetItemName(k, 0, ITEM_ALBUM, &nameitem);
		printf("ML_GetItemName()=");
		for (u=0;u<LEN_OF_FLASH_NAME_BUF;u++) {
#if 0
			printf("[%X]", nameitem.name[u]);
#else
			printf("%c", (nameitem.name[u] & 0x0000FF));
			printf("%c", ((nameitem.name[u] & 0xFF00)>>8));
			printf("%c", ((nameitem.name[u] & 0xFF0000)>>16));
#endif
#ifdef _SAVENAMESHORTCUT_
			if (nameitem.name[u] == 0) break;
#endif
		}
		ML_GetLvlTwoSongSortListSize( 0, k, ITEM_ALBUM, &n);
		printf("\nML_GetLvlTwoSongSortListSize( 0, k, ITEM_ALBUM, &n)=%d", n);
		for (m=0;m<n;m++) {
			ML_GetLvlTwoSongID( 0, k, ITEM_ALBUM, m, &r);
			printf("\nML_GetLvlTwoSongID( 0, k, ITEM_ALBUM, m, &r)=%d", r);
			ML_GetItemName(r, 0, ITEM_TRACK, &nameitem);
			printf("ML_GetItemName()=");
			for (u=0;u<LEN_OF_FLASH_NAME_BUF;u++) {
				printf("[%X]", nameitem.name[u]);
#ifdef _SAVENAMESHORTCUT_
				if (nameitem.name[u] == 0) break;
#endif
			}
		}
	}

	/* genre */
	ML_GetLvlOneItemSortListSize(0, ITEM_GENRE, &j);
	printf("\nML_GetLvlOneItemSortListSize(0, ITEM_GENRE, j)=%d", j);
	for (i=0;i<j;i++) {
		ML_GetLvlOneItemID(0, ITEM_GENRE, i, &k);
		printf("\nML_GetLvlOneItemID(0, ITEM_GENRE, i, k)=%d", k);
		printf("\nOrderID=%d\t", i);
		ML_GetItemName(i, 0, ITEM_GENRE, &nameitem);
		printf("ML_GetItemName()=");
		for (k=0;k<LEN_OF_FLASH_NAME_BUF;k++) {
			printf("[%X]", nameitem.name[k]);
#ifdef _SAVENAMESHORTCUT_
			if (nameitem.name[k] == 0) break;
#endif
		}
	}

	//ML_ConstructSongStruct(10, 0, &song_info);
	ML_GetLvlOneItemSortListSize(0, ITEM_1MONTH, &r);

	for (i=0;i<r;i++) {
		ML_GetLvlOneItemID(0, ITEM_1MONTH, i, &m );
		printf("m=%d", m);
	}
}

/*================================================================================================*/
#else
/*==================================================================================================
FUNCTION: MusicLibBuildModuleInit()

DESCRIPTION:
MusicLib Module Init.

ARGUMENTS PASSED:
struct ModuleEntry *pModuleTableEntry

RETURN VALUE:
int

PRE-CONDITIONS:
None

POST-CONDITIONS:
None

IMPORTANT NOTES:
None

==================================================================================================*/
int _reentrant MusicLibBuildModuleInit(struct ModuleEntry *pModuleTableEntry) {
	int i;
	union WaitMask returnMask;

	ML_InitLibraryParameter();

	//Tell ModuleTableEntry.wait to call this module's process function next when it has a msg
	returnMask.I=0x000000 | 2; // EVENT_MESSAGE
	return returnMask.I;
}

/*================================================================================================*/

/*==================================================================================================
FUNCTION: MusicLibBuildModuleProcessQueue()

DESCRIPTION:
MusicLib build Module ProcessQueue, UI interface for all musiclib function.

ARGUMENTS PASSED:
union WaitMask Signals,struct MessageQueueDescriptor * pQueue

RETURN VALUE:
int

PRE-CONDITIONS:
None

POST-CONDITIONS:
None

IMPORTANT NOTES:
None

==================================================================================================*/
int _reentrant MusicLibBuildModuleProcessQueue(union WaitMask Signals,struct MessageQueueDescriptor * pQueue)
{
#if 0
	union WaitMask returnMask;
	struct CMessage  Msg;

	RAM_SONG_INFO_T *build_parm2;
	int16 build_parm1;

	returnMask.I=0x000000 | 2;//Initialize it to signal us if we have a message

	//handle incoming messages
	if(Signals.B.m_bMessage) {
		while(GetMessage(pQueue,&Msg)) {
			switch(Msg.m_uMsg[0])
			{
			case MUSICLIB_BUILD_ADD_ENTRY:
				build_parm1 = Msg.m_uMsg[1];
				build_parm2 = (RAM_SONG_INFO_T*)Msg.m_uMsg[2];

				ML_AddEntryToLibrary(build_parm1, build_parm2,0);
				ML_FlushLibraryToFlash(build_parm1);
				break;
				/*
				case MUSICLIB_FLUSH_MEM:
				ML_FlushLibraryToFlash(build_parm1);
				break;
				*/
			default:
				break;
			}
		}
	}
	return returnMask.I;
#else
	return (0x000000 | 2);
#endif
}
/*================================================================================================*/
#endif /* _PC_SIMULATION_ */
