/*==================================================================================================

                                         MODULE DESCRIPTION

===================================================================================================
                               Perception Digital Confidential Proprietary
                               (c) Copyright PD 2004, All Rights Reserved

MODULE NAME: musiclib_browse_function.c   AUTHOR: Developer Name   CREATION DATE:DD/MM/YYYY

PRODUCT NAMES: All

GENERAL DESCRIPTION:

   General description of this grouping of functions.

Document Reference
----------------------------------------
Music Library Software Design Specification
Version : 01.01
Date : 21-12-2004


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
+---ML_BrowseMenu() {
|		return a list of items for display
|	}
|
+---ML_DownList() {
|		return a list of items for scroll down
|	}
|
+---ML_UpList() {
|		return a list of items for scroll up
|	}

Source location
----------------------------------------
All files are in directory:
	<Project_Directory>\system\msgmodules\software\MusicLib_Player\src\musiclib_browse_function.c


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


====================================================================================================
                                            DESCRIPTION
====================================================================================================


==================================================================================================*/

/*==================================================================================================
                                           INCLUDE FILES
==================================================================================================*/
#ifndef _PC_SIMULATION_
#define _RUNNING_IN_EMBEDDED_
#endif

#ifdef	_RUNNING_IN_EMBEDDED_
#define COMP_OPT _reentrant static
#define COMP_OPT_G extern
#else
#define COMP_OPT
#define _reentrant
#endif

#pragma optimize 1
#define BROWSE_PLAY_MODULE

#include "..\hdr\musiclib_hdr.h"
#include "musiclib_basic_function.c"

#define USE_FUNC_GET_MAP_SEC
#include "..\..\..\..\filesystem\fat32\h\extern.h"
#include "musiclib_flash_function.c"

/*==================================================================================================
                                         LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
                                         LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
                                         LOCAL ENUMS
==================================================================================================*/

/*==================================================================================================
                                     LOCAL STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
                                     LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
                                          LOCAL VARIABLES
==================================================================================================*/

/* store the merged list of item from flash memory and SD memory */
uint24              	flash_name_list_size;
uint24 volatile     	flash_window_pos;			/* starting position of the sorted list in flash */
uint24              	sd_name_list_size;
uint24 volatile		sd_window_pos;				/* starting position of the sorted list in SD card */
uint24		 	window_merge_index;
uint24              	total_merge_num_item;	/* if at song level total_merge_num_item=(num. of merged song), at other level = 0 */
uint24			parent_window_pos[4];			// store the parents' flash_window_pos and sd_window_pos
//uint24			parent_merge_item_size[2];		// store the total number of merged item of parents

int24			songIndexInMedia[2];

/* current state */
uint24              	current_state;
/* browsePath[4] save the parent level ID of browsing, eg1. now at artist->album->song level => browsePath[0];browsePath[1]=artistIDs : browsePath[2];browsePath[3]=albumIDs */
/* eg2. now at album->song level => browsePath[0];browsePath[1]=albumIDs */
uint24              	browsePath[4]; 
/* has insert SD card */
uint24              	hasInsertSD;
/* store current folder path of play song list */
uint24              	songPath[5];
uint24              	play_flash_name_list_size;
uint24              	play_sd_name_list_size;

// store play song attributes
uint24              	org_songPath[5];
uint24              	org_play_flash_name_list_size;
uint24              	org_play_sd_name_list_size;
uint24			org_currentPlayAllLevel;

/* store the last requested item */
uint24			last_request_item;

uint24 _X           	tmp_name_array[4];
uint24 _X 		sec_temp_buf_X[NUM_OF_READWORD_PER_SEC];
FLASH_GROUP_NAME_T	name[2];

/// variables for play module 
uint24 volatile				play_flash_window_pos;		/* starting position of the sorted list in flash for play song */
uint24 volatile				play_sd_window_pos;			/* starting position of the sorted list in flash for play song in SD card */
uint24					play_total_merge_num_item;
uint24					play_window_merge_index;
//uint24					currentSongID;				// the song id highlighted when start play song
//uint24					currentSongID_SD;			// the song id highlighted when start play song

/* current play all tree level */
uint24					playAllLevel;
uint24					isPlayAll;

/* last play song level */
uint24					lastPlaySongLevel;
uint24					currentPlayAllLevel;

/* play mode */
uint24	playMode;			/* bit 0 = current play mode; bit 1 = last play mode ; bit 2 = repeat on/off(0=off/1=on), not use now, bit 3 = select flash/SD */
uint24	lastSongID;

// variables for flash access
uint24 num_of_song_y;
uint24 num_of_item_y[MAX_NUM_OF_GROUP];

BOOL g_current_play_all;

/* for shuffle */
uint24 num_row[NumOfLevels], num_col[NumOfLevels];
uint24 g_tmp_offset[NumOfLevels], tmp_TOC[NumOfLevels];
uint24 Excess[22];
uint24 Excess_list_size;
uint24 shuffleOrder;

#ifdef _USE_FS_SECTAB_	/* define in flash_function.c 05/03/01 use fs access to store musiclib instead of direct sector access */
LONG _X l2_sector_table[LEVEL2_TABLE_SIZE];
LONG SectorSize;
uint24 num_long_per_sec;
#endif

ON_THE_FLY_LIST_T _X on_the_fly_list;
uint24 OnTheGoThisID;
uint24 OnTheGoPlayID;
uint24 OnTheGoPlayMedia;
uint24 IsPlayOnTheGo;

uint24 _reentrant ML_GetData( uint24 *array, uint24 pos ) {
	if ( (pos & 1) != 0 ) {
		return (array[(pos>>1)]/4096);
	}
	else {
		return (array[(pos>>1)] & 0xFFF);
	}	
}

void _reentrant ML_charIndexToWordIndex( uint24 unicode, uint24 charIndex, uint24 *wordIndex, uint24 *relativeByteIndex ) {
	if (unicode == BITSET_NAME_ALLASCII) {
		*wordIndex = charIndex/NUM_OF_BYTE_IN_ONE_WORD;
		*relativeByteIndex = charIndex - (*wordIndex*NUM_OF_BYTE_IN_ONE_WORD);			
	}
	else {
		*wordIndex = charIndex;
		*relativeByteIndex = 0;
	}
}

void _reentrant ML_strcpy( uint24 *dest, uint24 *src ) {
	uint24 i;

//	if ( !dest || !src )
//		return 0;

	for ( i=0;i<LEN_OF_NAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD;i++ ) {
		dest[i] = src[i];
		if ( src[i] == 0 )
			return;
	}

//	return 1;
}

void _reentrant ML_strcpy_Y2X( uint24 _X *dest, uint24 *src, uint24 len ) {
	uint24 i;

//	for ( i=0;i<LEN_OF_NAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD;i++ ) {
	for ( i=0;i<len;i++ ) {
		dest[i] = src[i];
		if ( src[i] == 0 )
			return;
	}

//	return 1;
}

void _reentrant ML_selectItemInfoByState( uint24 isSongBrowse, uint24 state, uint24 memory_type, uint24 *item_type, uint24 *id1, uint24 *id2 ) {
//	uint24 index1;
//	uint24 index2;
	uint24 temp;
	uint24 *listPt;

	*id1 = *id2 = 0xFFFFFF;

	*item_type = state>>2;
	
	temp = state & 3;
	if ( temp ) {		
//		index1 = (memory_type<<1) + 1 + isSongBrowse;

		listPt = ( !isSongBrowse ) ? browsePath : songPath;
//		*id1 = (ML_GetData( listPt, (index1-1))<<12) | ML_GetData( listPt, index1 );
		*id1 = listPt[ memory_type ];

		if ( temp == 2 ) {
//			index2 = (memory_type<<1) + temp + 3 + isSongBrowse;								
//			*id2 = (ML_GetData( listPt, (index2-1))<<12) | ML_GetData( listPt, index2 );
			*id2 = listPt[ memory_type + 2 ];
		}
	}
}
