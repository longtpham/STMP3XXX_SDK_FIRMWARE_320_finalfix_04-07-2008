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

This version use static ram as temp_vname_buf[], compilable on tasking.

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
INCLUDE FILES
==================================================================================================*/
#ifdef WIN32
#define _PC_SIMULATION_
#endif

#ifndef _PC_SIMULATION_
#define _RUNNING_IN_EMBEDDED_
#endif

#ifdef	_RUNNING_IN_EMBEDDED_
#define COMP_OPT _reentrant static
#define COMP_OPT_G
#else
#define COMP_OPT
#endif

#define VOICE_FUNCTION

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
#include "settimemenu.h"
#include "resource.h"
#include "menumanager.h"
#endif

/*==================================================================================================
LOCAL CONSTANTS
==================================================================================================*/
#define	LEN_OF_FLASH_VOICE_NAME_BUF	(LEN_OF_VOICE_NAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD)
#define	LEN_OF_MTSTRING			(LEN_OF_SUB_STRING_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD)

/*==================================================================================================
LOCAL MACROS
==================================================================================================*/
#if 0
/* distance in mark table */
#define DISTANCE_IN_MARK_TABLE (4) /* must be divisible by MAX_NUM_OF_SONG, Test use 4 */
#else
/* distance in mark table */
#define DISTANCE_IN_MARK_TABLE (40) /* must be divisible by MAX_NUM_OF_SONG */
#endif
/* number of entry in mark table */
#define NUM_OF_MARK_TABLE_ENTRY (2 * (MAX_NUM_OF_SONG/DISTANCE_IN_MARK_TABLE) - 1)	/* The
table entries once set will not be changed during the whole insert process, so the total
number of entries is doubled */
/* in mark table, one column is ID, another is distance */
#define NUM_OF_MARK_TABLE_COL (2)
/* length of string stored in mark table */
#define LEN_OF_SUB_STRING_IN_BYTE (30) /* must be an integer multiple of 3 */

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
	uint24 mark_tableID;
	uint24 dummy92[93];
} RAM_VOICE_MARKTABLE_INFO_T;

/* struct to store the group information: artist, album and genre, in ram during
construction of music library */
typedef struct {
	/* sorted list, it is the sorted link list with entry in each cell is the next
	item¡¦s ID*/
	uint24 sorted_list[MAX_NUM_OF_VOICE/NUM_OF_ITEM_IN_ONE_WORD];
	uint24 sorted_list_hd;
	uint24 num_of_voice;
	uint24 dummy9[10];
} RAM_VOICE_INFO_T;

/*==================================================================================================
LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
LOCAL VARIABLES
==================================================================================================*/
/* RAM */

uint24 vtemp_var;		/* counter used in macro functions */
uint24	temp_vlist_buf[DISTANCE_IN_MARK_TABLE][3];
uint24	insert_vname[LEN_OF_NAME_IN_BYTE];	/* as temp buf for on_the_fly_list path */
uint24 temp_vname_buf_Y[LEN_OF_NAME_IN_BYTE];	/* use as general temp string buffer */
FLASH_GROUP_NAME_T flash_group_vname_y;

/* variables in _X */
RAM_VOICE_MARKTABLE_INFO_T _X ram_voice_marktable;
RAM_VOICE_INFO_T _X ram_voice_info;
FLASH_GROUP_NAME_T _X flash_group_vname;
uint24 _X flash_path_vname_X[(LEN_OF_FILEPATH_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD)+2];
uint24 _X temp_vname_buf[LEN_OF_NAME_IN_BYTE];	/* use as general temp string buffer */
uint24 _X temp_vqueue[(MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD)+1];	/* +1 for share the temp buf with audible struct */

/*==================================================================================================
GLOBAL VARIABLES
==================================================================================================*/
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
extern _reentrant void RefreshDisplayTime(int *p_display_time);
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

/*================================================================================================*/
#ifdef _PC_DEBUGPRINT_
COMP_OPT void ML_vlist_get_val(uint24 *uint24_p, uint24 pos, uint24 *val) {
	*val = uint24_p[(vtemp_var = pos/NUM_OF_ITEM_IN_ONE_WORD)];
	if ((pos & 0x01) == 0x01) {
		*val = (*val & 0x000FFF);
	} else {
		*val = ((*val & 0xFFF000)>>12);
	}
}
#endif
/*================================================================================================*/
COMP_OPT void ML_vlist_put_val_X(uint24 _X *uint24_p, uint24 pos, uint24 val) {
	vtemp_var = pos/NUM_OF_ITEM_IN_ONE_WORD;
	if ((pos & 0x01) == 0x01) {
		uint24_p[vtemp_var] = ((uint24_p[vtemp_var] & 0xFFF000) | (val & 0x000FFF));
	} else {
		uint24_p[vtemp_var] = (((val & 0x000FFF)<<12) | (uint24_p[vtemp_var] & 0x000FFF));
	}
}

/*==================================================================================================
FUNCTION: FlushVoiceToFlash()

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
void ML_FlushVoiceToFlash(uint24 Rec_type) {
	uint24	i;
	uint24	j;
	uint24	k;

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
#if 0
	for (i=0;i<(MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD);i++) {
		temp_vqueue[i] = 0x00;
	}
#endif
	j = ram_voice_info.sorted_list_hd;
	for (i=0;i<ram_voice_info.num_of_voice;i++) {
		ML_vlist_put_val_X(temp_vqueue, i, j);
		ML_list_get_val_X(ram_voice_info.sorted_list, j, &k);
		j = k;
	}
	temp_vqueue[(MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD)] = ram_voice_info.num_of_voice;
	//__asm("debug");
	if (Rec_type == REC_VOICE_TYPE) {
		ML_MemWrite(MEDIA_TYPE_FLASH, VOICE_ARRAY_DTYPE, ram_voice_info.num_of_voice, temp_vqueue, ML_LIST_STRUCT_SIZE);
		ML_MemWrite(MEDIA_TYPE_FLASH, VOICE_INFO_DTYPE, ram_voice_info.num_of_voice, (uint24 _X *) &ram_voice_info, ML_VOICE_INFO_STRUCT_SIZE);
		ML_MemWrite(MEDIA_TYPE_FLASH, VOICE_MARKTABLE_DTYPE, ram_voice_marktable.mark_tableID, (uint24 _X *) &ram_voice_marktable, ML_VOICE_MARKTABLE_STRUCT_SIZE);
	} else if (Rec_type == REC_FMREC_TYPE) {
		ML_MemWrite(MEDIA_TYPE_FLASH, FMREC_ARRAY_DTYPE, ram_voice_info.num_of_voice, temp_vqueue, ML_LIST_STRUCT_SIZE);
		ML_MemWrite(MEDIA_TYPE_FLASH, FMREC_INFO_DTYPE, ram_voice_info.num_of_voice, (uint24 _X *) &ram_voice_info, ML_VOICE_INFO_STRUCT_SIZE);
		ML_MemWrite(MEDIA_TYPE_FLASH, FMREC_MARKTABLE_DTYPE, ram_voice_marktable.mark_tableID, (uint24 _X *) &ram_voice_marktable, ML_VOICE_MARKTABLE_STRUCT_SIZE);
	} else if (Rec_type == REC_PHOTO_TYPE) {
		ML_MemWrite(MEDIA_TYPE_FLASH, PHOTO_ARRAY_DTYPE, ram_voice_info.num_of_voice, temp_vqueue, ML_LIST_STRUCT_SIZE);
		ML_MemWrite(MEDIA_TYPE_FLASH, PHOTO_INFO_DTYPE, ram_voice_info.num_of_voice, (uint24 _X *) &ram_voice_info, ML_VOICE_INFO_STRUCT_SIZE);
		ML_MemWrite(MEDIA_TYPE_FLASH, PHOTO_MARKTABLE_DTYPE, ram_voice_marktable.mark_tableID, (uint24 _X *) &ram_voice_marktable, ML_VOICE_MARKTABLE_STRUCT_SIZE);
	} else {
		ML_MemWrite(MEDIA_TYPE_FLASH, MVIDEO_ARRAY_DTYPE, ram_voice_info.num_of_voice, temp_vqueue, ML_LIST_STRUCT_SIZE);
		ML_MemWrite(MEDIA_TYPE_FLASH, MVIDEO_INFO_DTYPE, ram_voice_info.num_of_voice, (uint24 _X *) &ram_voice_info, ML_VOICE_INFO_STRUCT_SIZE);
		ML_MemWrite(MEDIA_TYPE_FLASH, MVIDEO_MARKTABLE_DTYPE, ram_voice_marktable.mark_tableID, (uint24 _X *) &ram_voice_marktable, ML_VOICE_MARKTABLE_STRUCT_SIZE);
	}
}

/*================================================================================================*/
COMP_OPT uint24 ItemCmp(uint24 CmpType, uint24 _X *item0, uint24 *item1) {
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
FUNCTION: AddEntryToVoice()

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
void ML_AddEntryToVoice(uint24 Rec_type, RAM_SONG_INFO_T *song_info, uint24 action) {
	uint24	mt_lbID;
	uint24	mt_ubID;
	uint24	mt_midID;

	uint24	buf_lbID;
	uint24	buf_ubID;
	uint24	buf_midID;

	uint24	pathtype;
	uint24	nametype = 0;

	uint24 i;
	uint24 j;
	/*
	uint24 k;
	*/
	uint24 temp_name_bufID;
	uint24 temp_previousID;
	uint24 temp_midID;

	uint24 name_found;
	uint24 current_voiceID;
	uint24 *p_fastkey;

#ifdef _PC_DEBUGPRINT_
	printf("\nEnter AddToEntryLibrary().\n");
#endif
	/*
	flush song_info.song_name to flash_song_name[MEDIA_TYPE_FLASH][num_of_song].name;
	flush song_info.unicode to flash_song_name[MEDIA_TYPE_FLASH][num_of_song].unicode;
	flush song_info.path_name to flash_path_vname_X[num_of_song];
	flush song_info.year to flash_year[num_of_song];
	num_of_song++;
	func(sort the song name) {
	locate the start and end index in the ram_song_group.mark_table_index
	using binary search in the mark table, by comparing the new song name with
	ram_song_group.mark_table_string;

	load the song names from flash indicated by the start and end index to
	temp_vname_buf in sorted order;

	start binary search in temp_vname_buf to locate the exact position for
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
	*/

	if (action == VOICE_FILE_ADD) {
		current_voiceID = ram_voice_info.num_of_voice;
		for (i=0;i<(LEN_OF_FILEPATH_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD);i++) {
			flash_path_vname_X[i] = song_info->path_name[i];
			if (ChkTermInWord(flash_path_vname_X[i])) {
				if (((flash_path_vname_X[i] & 0x00FF00) == 0x00)
					|| ((flash_path_vname_X[i] & 0xFF0000) == 0x00)) {
						flash_path_vname_X[i] = flash_path_vname_X[i] & 0x00FFFF;
						i++;
				}
				for (j=i;j<(LEN_OF_FILEPATH_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD);j++) {
					flash_path_vname_X[j] = 0x00;
				}
				break;
			}
		}
		switch (Rec_type) {
			case REC_VOICE_TYPE:
				pathtype = VOICE_PATH_DTYPE;
				nametype = VOICE_NAME_DTYPE;
				break;
			case REC_FMREC_TYPE:
				pathtype = FMREC_PATH_DTYPE;
				nametype = FMREC_NAME_DTYPE;
				break;
			case REC_PHOTO_TYPE:
				pathtype = PHOTO_PATH_DTYPE;
				nametype = PHOTO_NAME_DTYPE;
				break;
			case REC_MVIDEO_TYPE:
				pathtype = MVIDEO_PATH_DTYPE;
				nametype = MVIDEO_NAME_DTYPE;
				break;
			default:
				return;
				//break;
		}
		p_fastkey=(uint24 *)&song_info->g_songFastKey;
		flash_path_vname_X[(LEN_OF_FILEPATH_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD)+0]=*p_fastkey;p_fastkey++;
		flash_path_vname_X[(LEN_OF_FILEPATH_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD)+1]=*p_fastkey;

		if (ML_MemWrite(MEDIA_TYPE_FLASH, pathtype, current_voiceID, flash_path_vname_X, ML_PATH_NAME_STRUCT_SIZE) != SUCCESS) {
			/* flash access error */
		}

		for (i=0;i<LEN_OF_FLASH_VOICE_NAME_BUF;i++) {
			flash_group_vname.name[i] = song_info->song_name[i];
		}
		flash_group_vname.unicode = (song_info->unicode & 0xc0) >> 6;	// according to the Unicode Flag selection of "playlist3init.c", SL

		if (ML_extract_string_Voice(flash_group_vname.unicode, insert_vname, flash_group_vname.name, LEN_OF_NAME_IN_BYTE) == 0x00) {
			/* "Unknown Song":unicode=ALL_ASCII */
			flash_group_vname.name[0] = 0x6E5528;
			flash_group_vname.name[1] = 0x6F6E6B;
			flash_group_vname.name[2] = 0x206E77;
			flash_group_vname.name[3] = 0x6E6F53;
			flash_group_vname.name[4] = 0x002967;
			flash_group_vname.unicode = BITSET_NAME_ALLASCII;
			ML_extract_string_Voice(flash_group_vname.unicode, insert_vname, flash_group_vname.name, LEN_OF_NAME_IN_BYTE);
		}
#if 0
		/* store the exact sorting string to flash for debug only, _PRINTSONGID_, debug sorting */
		if ((song_info->unicode & BITMASK_SONG) == BITCHK_SONG_ALLASCII) {
			j = 0;
			for (i=0;i<LEN_OF_FLASH_NAME_BUF;i++) {
				flash_group_vname.name[i] = (insert_name[j] & 0xFF) | ((insert_name[++j] & 0xFF)<<8) | ((insert_name[++j] & 0xFF)<<16);
				j++;
			}
		}
#endif
		if (ML_MemWrite(MEDIA_TYPE_FLASH, nametype, current_voiceID, (uint24 _X *)&flash_group_vname, ML_SONG_NAME_STRUCT_SIZE) != SUCCESS) {
			/* flash access error */
		}
	}
#ifdef _PC_DEBUGPRINT_
	printf("\ninsert_vname=");
	for (w=0;w<LEN_OF_NAME_IN_BYTE;w++) {
		printf("[%X]", insert_vname[w]);
	}
#endif

	name_found = FALSE;
	mt_lbID = 0;
	if (ram_voice_marktable.mark_tableID > 0x01) {
		mt_ubID = ram_voice_marktable.mark_tableID;	/* ram_voice_marktable.mark_tableID = # of entry-1 */
		mt_midID = mt_ubID>>1;
		/* table size = 0 to (ram_voice_marktable.mark_tableID-1) */
		do {
			i = ItemCmp(UINT24_MARKTAB_CMPTYPE, ram_voice_marktable.mark_table_string[mt_midID],
				insert_vname);
			if (i == ITEMCMP_ITEM0_SMALL) {
				mt_lbID = mt_midID+1;
			} else if (i == ITEMCMP_ITEM0_LARGE) {
				mt_ubID = mt_midID;
			} else {
				i = LEN_OF_MTSTRING;
			}
			/* first 30 char equal */
			if (i == LEN_OF_MTSTRING) {
				ML_MemRead_X(MEDIA_TYPE_FLASH, nametype, ram_voice_marktable.mark_table_index[mt_midID][MT_NAMEID_COL], (uint24 _X *) &flash_group_vname, ML_SONG_NAME_STRUCT_SIZE);
				ML_extract_string_Voice(flash_group_vname.unicode,
					temp_vname_buf_Y,
					flash_group_vname.name,
					LEN_OF_NAME_IN_BYTE);
				for (i=0;i<LEN_OF_NAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD;i++) {
					temp_vname_buf[i] = temp_vname_buf_Y[i];
				}
				if ((i = ItemCmp(UINT24_STR_CMPTYPE, temp_vname_buf, insert_vname)) == ITEMCMP_ITEM0_SMALL) {
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

	/* copy2 temp_vname_buf */
	/* build index table for marktable search */
	if (mt_midID > 0) {
		ML_list_get_val_X(ram_voice_info.sorted_list,
			ram_voice_marktable.mark_table_index[mt_midID-1][MT_NAMEID_COL], &j);
		temp_previousID = j;
		/* find the first previous index */
		for (i=0;i<(ram_voice_marktable.mark_table_index[mt_midID-1][MT_COUNT_COL]-2);i++) {
			ML_list_get_val_X(ram_voice_info.sorted_list, j, &temp_previousID);
			j = temp_previousID;
		}
	} else {
		temp_previousID = INDEX_EOF;	/* so the first previous index is invalid */
	}
	i = ram_voice_marktable.mark_table_index[mt_midID][MT_NAMEID_COL];

	for (temp_name_bufID=0;temp_name_bufID<ram_voice_marktable.mark_table_index[mt_midID][MT_COUNT_COL];
		temp_name_bufID++) {
			if (i == INDEX_EOF) break;
			temp_vlist_buf[temp_name_bufID][TEMP_LIST_PREID_COL] = temp_previousID;
			temp_vlist_buf[temp_name_bufID][TEMP_LIST_THISID_COL] = i;
			ML_list_get_val_X(ram_voice_info.sorted_list, i, &temp_vlist_buf[temp_name_bufID][TEMP_LIST_NXID_COL]);

			temp_previousID = temp_vlist_buf[temp_name_bufID][TEMP_LIST_THISID_COL];
			i = temp_vlist_buf[temp_name_bufID][TEMP_LIST_NXID_COL];
	}	/* temp_name_bufID-1 = last entry */

#ifdef _PC_DEBUGPRINT_
	printf("\ntemp_name_bufID=%d:temp_vlist_buf=", temp_name_bufID);
	//for (w=0;w<DISTANCE_IN_MARK_TABLE;w++) {
	for (w=0;w<temp_name_bufID;w++) {
		if (w == temp_name_bufID) { printf("\n"); }
		printf("\n[%X:", temp_vlist_buf[w][TEMP_LIST_PREID_COL]);
		printf("%X:", temp_vlist_buf[w][TEMP_LIST_THISID_COL]);
		printf("%X]", temp_vlist_buf[w][TEMP_LIST_NXID_COL]);
	}
#endif
	name_found = FALSE;
	/* bsearch */
	buf_lbID = 0;	/* lower bound, point of match at the end */
	if (temp_name_bufID > 0x00) {
		buf_ubID = temp_name_bufID;
		buf_midID = temp_name_bufID>>1;
		/* table size = 0 to (temp_name_bufID-1) */
		do {
			ML_MemRead_X(MEDIA_TYPE_FLASH, nametype, temp_vlist_buf[buf_midID][TEMP_LIST_THISID_COL], (uint24 _X *) &flash_group_vname, ML_SONG_NAME_STRUCT_SIZE);
			ML_extract_string_Voice(flash_group_vname.unicode,
				temp_vname_buf_Y, flash_group_vname.name, LEN_OF_NAME_IN_BYTE);
			for (i=0;i<LEN_OF_NAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD;i++) {
				temp_vname_buf[i] = temp_vname_buf_Y[i];
			}
			i = ItemCmp(UINT24_STR_CMPTYPE, temp_vname_buf, insert_vname);
			if (i == ITEMCMP_ITEM0_SMALL) {
				buf_lbID = buf_midID+1;
			} else if (i == ITEMCMP_ITEM0_LARGE) {
				buf_ubID = buf_midID;
			} else {
				name_found = TRUE;
				buf_midID++;	/* point to the nx id */
				break;
			}
			buf_midID = (buf_ubID + buf_lbID)>>1;
		} while (buf_lbID < buf_ubID);
	} else {
		buf_midID = 0x00;
	}

	/* update the sorted_list, mark_table-add entry only */
#ifdef _PC_DEBUGPRINT_
	printf("\nbuf_midID=%X:insert pos=%X", buf_midID, buf_midID);
#endif
	if (buf_midID == 0) {
		/* Input is the 1st one, update mark_table-add entry only */
		ram_voice_marktable.mark_table_index[mt_lbID][MT_NAMEID_COL] = ram_voice_info.num_of_voice;
		for (i=0;i<LEN_OF_MTSTRING;i++) {
			ram_voice_marktable.mark_table_string[mt_midID][i] = insert_vname[i];
		}
	}
	if (buf_midID < temp_name_bufID ) {
		ML_vlist_put_val_X(ram_voice_info.sorted_list, ram_voice_info.num_of_voice, temp_vlist_buf[buf_midID][TEMP_LIST_THISID_COL]);
		if (temp_vlist_buf[buf_midID][TEMP_LIST_PREID_COL] == INDEX_EOF) {
			ram_voice_info.sorted_list_hd = ram_voice_info.num_of_voice;
		} else {
			ML_vlist_put_val_X(ram_voice_info.sorted_list, temp_vlist_buf[buf_midID][TEMP_LIST_PREID_COL], ram_voice_info.num_of_voice);
		}
	} else {
		/* Input is the last one */
		if (temp_name_bufID > 0) {
			ML_vlist_put_val_X(ram_voice_info.sorted_list, ram_voice_info.num_of_voice, temp_vlist_buf[buf_midID-1][TEMP_LIST_NXID_COL]);
			ML_vlist_put_val_X(ram_voice_info.sorted_list, temp_vlist_buf[buf_midID-1][TEMP_LIST_THISID_COL], ram_voice_info.num_of_voice);
		} else {
			/* 1st name: songID=0 */
			ML_vlist_put_val_X(ram_voice_info.sorted_list, ram_voice_info.num_of_voice, (uint24) INDEX_EOF);
			ram_voice_info.sorted_list_hd = ram_voice_info.num_of_voice;
			ram_voice_marktable.mark_tableID = 1;
		}
	}
	/* update mark_table-add entry only */
	ram_voice_marktable.mark_table_index[mt_midID][MT_COUNT_COL]++;

#ifdef _PC_DEBUGPRINT_
	printf("ram_group.sorted_list_hd=[%d]\n", ram_voice_info.sorted_list_hd);
	printf("ram_group[%d].sorted_list=", gp_count);
	//for (w=0;w<MAX_NUM_OF_SONG;w++) {
	for (w=0;w<ram_voice_info.num_of_voice;w++) {
		ML_vlist_get_val(ram_voice_info.sorted_list, w, &y);
		printf("[%d]", y);
	}
#if 0
	printf("\nflash_group_vname.name=", ram_voice_info.num_of_voice);
	ML_MemRead_X(MEDIA_TYPE_FLASH, nametype, ram_voice_info.num_of_voice, (uint24 *) &flash_group_vname, ML_SONG_NAME_STRUCT_SIZE);
	for (w=0;w<LEN_OF_FLASH_VOICE_NAME_BUF;w++) {
		printf("[%X]", flash_group_vname.name[w]);
	}
	printf("\nflash_group_vname.unicode=%X", ram_voice_info.num_of_voice, flash_group_vname.unicode);
#endif
#endif

	/* update mark_table-split if needed */
	if (ram_voice_marktable.mark_table_index[mt_midID][MT_COUNT_COL] > DISTANCE_IN_MARK_TABLE) {
		/* split position index 1/2 of ram_voice_marktable.mark_table_index[mt_midID][MT_COUNT_COL] */
#ifdef _ASSERT_
		assert(ram_voice_marktable.mark_tableID < NUM_OF_MARK_TABLE_ENTRY);
#endif
		/* move entries down */
		for (j=ram_voice_marktable.mark_tableID;j>(mt_midID+1);j--) {
			for (i=0;i<LEN_OF_MTSTRING;i++) {
				ram_voice_marktable.mark_table_string[j][i]
				= ram_voice_marktable.mark_table_string[j-1][i];
				/*
				#ifdef _SAVENAMESHORTCUT_
				//at here the effect is too small...
				if (ram_voice_marktable.mark_table_string[j][i] == EOSTR) break;
				#endif
				*/
			}
			ram_voice_marktable.mark_table_index[j][MT_NAMEID_COL]
			= ram_voice_marktable.mark_table_index[j-1][MT_NAMEID_COL];
			ram_voice_marktable.mark_table_index[j][MT_COUNT_COL]
			= ram_voice_marktable.mark_table_index[j-1][MT_COUNT_COL];
		}
		/*
		#ifdef _ASSERT_
		assert(temp_name_bufID == DISTANCE_IN_MARK_TABLE);
		#endif
		*/
		temp_midID = mt_midID+1;
		j = temp_vlist_buf[DISTANCE_IN_MARK_TABLE/2][TEMP_LIST_THISID_COL];
		ML_MemRead_X(MEDIA_TYPE_FLASH, nametype, j, (uint24 _X *) &flash_group_vname, ML_SONG_NAME_STRUCT_SIZE);
		ML_extract_string_Voice(flash_group_vname.unicode,
			temp_vname_buf_Y,
			flash_group_vname.name,
			LEN_OF_MTSTRING);
		for (i=0;i<LEN_OF_MTSTRING;i++) {
			ram_voice_marktable.mark_table_string[temp_midID][i] = temp_vname_buf_Y[i];
		}
		ram_voice_marktable.mark_table_index[temp_midID][MT_NAMEID_COL] = j;
		if (buf_midID > (DISTANCE_IN_MARK_TABLE/2)) {
			ram_voice_marktable.mark_table_index[temp_midID][MT_COUNT_COL]
			= (DISTANCE_IN_MARK_TABLE/2) + 1;
			ram_voice_marktable.mark_table_index[mt_midID][MT_COUNT_COL]
			= (DISTANCE_IN_MARK_TABLE/2);
		} else {
			ram_voice_marktable.mark_table_index[temp_midID][MT_COUNT_COL]
			= (DISTANCE_IN_MARK_TABLE/2);
			ram_voice_marktable.mark_table_index[mt_midID][MT_COUNT_COL]
			= (DISTANCE_IN_MARK_TABLE/2) + 1;
		}
		ram_voice_marktable.mark_tableID++;
#ifdef _PC_DEBUGPRINT_
		printf("\nvMarkTableID new add=%d", ram_voice_marktable.mark_tableID);
#endif
	}

	ram_voice_info.num_of_voice++;

#ifdef _PC_DEBUGPRINT_
	printf("\nUpdated MrakTableID=%d, ram_voice_marktable.mark_table_index=", ram_voice_marktable.mark_tableID);
	for (w=0;w<ram_voice_marktable.mark_tableID;w++) {
		printf("\n%X", ram_voice_marktable.mark_table_index[w][MT_NAMEID_COL]);
		printf("C=%XStr=", ram_voice_marktable.mark_table_index[w][MT_COUNT_COL]);
		for (y=0;y<LEN_OF_MTSTRING;y++) {
			printf("%X:", ram_voice_marktable.mark_table_string[w][y]);
		}
	}
	printf("\nnum_of_voice=%d", ram_voice_info.num_of_voice);
	printf("ram_voice_marktable.mark_tableID=%d", ram_voice_marktable.mark_tableID);

	printf("\nSorted ram_name(compared string)=");
	w = ram_voice_info.sorted_list_hd;
	while (TRUE) {
		if (w == INDEX_EOF) break;
		printf("\nID=%X", w);

		ML_MemRead_X(MEDIA_TYPE_FLASH, nametype, w, (uint24 *) &flash_group_vname, ML_SONG_NAME_STRUCT_SIZE);
		ML_extract_string_Voice(flash_group_vname.unicode,
			temp_vname_buf,
			flash_group_vname.name,
			LEN_OF_NAME_IN_BYTE);
		printf("[");
		for (y=0;y<LEN_OF_NAME_IN_BYTE;y++) {
			//			printf("[%X]", temp_vname_buf[y]);
			printf("%c", temp_vname_buf[y]);
			if (temp_vname_buf[y] == 0) break;
		}
		printf("]");
		ML_vlist_get_val(ram_voice_info.sorted_list, w, &y);
		w = y;
	}
#endif

#ifdef _PC_DEBUGPRINT_
	printf("\nAddToEntryLibrary() return.\n");
#endif
}

/*================================================================================================*/
/*==================================================================================================
FUNCTION: ML_MusicLibVoiceInit()

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
void ML_InitVoiceParameter(void) {
	uint24 i;

	ram_voice_marktable.mark_tableID = 0;
	ram_voice_info.num_of_voice = 0;

	temp_vlist_buf[0][TEMP_LIST_PREID_COL] = INDEX_EOF;
	temp_vlist_buf[0][TEMP_LIST_THISID_COL] = INDEX_EOF;
	temp_vlist_buf[0][TEMP_LIST_NXID_COL] = INDEX_EOF;

	ram_voice_info.sorted_list_hd = INDEX_EOF;
	for (i=0;i<NUM_OF_MARK_TABLE_ENTRY;i++) {
		ram_voice_marktable.mark_table_string[i][0] = EOSTR; /* '\0' */
		ram_voice_marktable.mark_table_index[i][MT_NAMEID_COL] = INDEX_EOF;
		ram_voice_marktable.mark_table_index[i][MT_COUNT_COL] = 0;
	}
}

void ML_LoadVoiceParameter(uint24 Rec_type) {

	temp_vlist_buf[0][TEMP_LIST_PREID_COL] = INDEX_EOF;
	temp_vlist_buf[0][TEMP_LIST_THISID_COL] = INDEX_EOF;
	temp_vlist_buf[0][TEMP_LIST_NXID_COL] = INDEX_EOF;

	if (Rec_type == REC_VOICE_TYPE) {
		ML_MemRead_X(MEDIA_TYPE_FLASH, VOICE_INFO_DTYPE, ram_voice_info.num_of_voice, (uint24 _X *) &ram_voice_info, ML_VOICE_INFO_STRUCT_SIZE);
		ML_MemRead_X(MEDIA_TYPE_FLASH, VOICE_MARKTABLE_DTYPE, ram_voice_marktable.mark_tableID, (uint24 _X *) &ram_voice_marktable, ML_VOICE_MARKTABLE_STRUCT_SIZE);
	} else if (Rec_type == REC_FMREC_TYPE) {
		ML_MemRead_X(MEDIA_TYPE_FLASH, FMREC_INFO_DTYPE, ram_voice_info.num_of_voice, (uint24 _X *) &ram_voice_info, ML_VOICE_INFO_STRUCT_SIZE);
		ML_MemRead_X(MEDIA_TYPE_FLASH, FMREC_MARKTABLE_DTYPE, ram_voice_marktable.mark_tableID, (uint24 _X *) &ram_voice_marktable, ML_VOICE_MARKTABLE_STRUCT_SIZE);
	} else if (Rec_type == REC_PHOTO_TYPE) {
		ML_MemRead_X(MEDIA_TYPE_FLASH, PHOTO_INFO_DTYPE, ram_voice_info.num_of_voice, (uint24 _X *) &ram_voice_info, ML_VOICE_INFO_STRUCT_SIZE);
		ML_MemRead_X(MEDIA_TYPE_FLASH, PHOTO_MARKTABLE_DTYPE, ram_voice_marktable.mark_tableID, (uint24 _X *) &ram_voice_marktable, ML_VOICE_MARKTABLE_STRUCT_SIZE);
	} else {
		ML_MemRead_X(MEDIA_TYPE_FLASH, MVIDEO_INFO_DTYPE, ram_voice_info.num_of_voice, (uint24 _X *) &ram_voice_info, ML_VOICE_INFO_STRUCT_SIZE);
		ML_MemRead_X(MEDIA_TYPE_FLASH, MVIDEO_MARKTABLE_DTYPE, ram_voice_marktable.mark_tableID, (uint24 _X *) &ram_voice_marktable, ML_VOICE_MARKTABLE_STRUCT_SIZE);
	}
}

/*================================================================================================*/
