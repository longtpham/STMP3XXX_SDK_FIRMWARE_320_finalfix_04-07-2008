/*==================================================================================================

MODULE DESCRIPTION

====================================================================================================
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
#include	"..\hdr\musiclib_flash_function.h"

/*==================================================================================================
LOCAL CONSTANTS
==================================================================================================*/
#define ML_SONG_NAME_STRUCT_SIZE		(LEN_OF_NAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD + 1)
#define ML_PATH_NAME_STRUCT_SIZE		((LEN_OF_FILEPATH_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD)+2)

/*==================================================================================================
LOCAL MACROS
==================================================================================================*/
#define isAtLevel1( state ) ( ( (state & 0x03) == 0 ) ? 1 : 0 )
#define isUnderArtist( state )  ( (state <= 0x02) ? 1 : 0 )
#define getLastPlayMode( value )	((value&2)>>1)
#define setLastPlayMode( value )	( value = (((value&1)<<1) | (value&1)) )
#define setCurrentPlayMode( value, mode )	( value = ((value&2) | (mode&1)) )
#define getCurrentPlayMode( value )	(value&1)

/*==================================================================================================
LOCAL ENUMS
==================================================================================================*/

/*==================================================================================================
LOCAL STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

// Flash Access Interfaces
//COMP_OPT uint24 LOCAL_GetLvlOneItem_info( uint24 memory_type, uint24 item_type, uint24 sortedListIndex, uint24 *info );
extern uint24 _reentrant LOCAL_GetLvlOneItem_info( uint24 memory_type, uint24 item_type, uint24 sortedListIndex, uint24 *info );
COMP_OPT uint24 ML_GetLvlOneItemSortListSize( uint24 memory_type, uint24 item_type, uint24 *size );
//COMP_OPT uint24 LOCAL_GetFlashAssociatedList_info( uint24 memory_type, uint24 lvl1_id, uint24 albumID, uint24 item_type, uint24 sortedListIndex, uint24 *info );
extern uint24 _reentrant LOCAL_GetFlashAssociatedList_info( uint24 memory_type, uint24 lvl1_id, uint24 albumID, uint24 item_type, uint24 sortedListIndex, uint24 *info );
//COMP_OPT uint24 ML_GetLvlTwoSongSortListSize( uint24 memory_type, uint24 id, uint24 item_type, uint24 *size );
//COMP_OPT uint24 ML_GetFlashArtistAlbumSongSortListSize( uint24 memory_type, uint24 artistID, uint24 albumID, uint24 *size );
//COMP_OPT uint24 ML_GetLvlOneItemID( uint24 memory_type, uint24 item_type, uint24 sortedListIndex, uint24 *ID );
//COMP_OPT uint24 ML_GetLvlTwoSongID( uint24 memory_type, uint24 lvl_id, uint24 item_type, uint24 sortedListIndex, uint24 *ID );
//COMP_OPT uint24 ML_GetFlashArtistAlbumSongID( uint24 memory_type, uint24 artistID, uint24 albumID, uint24 sortedListIndex, uint24 *ID );
COMP_OPT uint24 ML_GetItemName( uint24 id, uint24 memory_type, uint24 item_type, FLASH_GROUP_NAME_T *itemStruct );
COMP_OPT uint24 ML_GetSongYear( uint24 id, uint24 memory_type, uint24 *year );
COMP_OPT void ML_GetTracknums( uint24 isSongBrowse, uint24 state, uint24 pos1, uint24 pos2, uint24 *tracknum1, uint24 *tracknum2, uint24 *id1, uint24 *id2);
COMP_OPT uint24 ML_GetItemTracknum ( uint24 id, uint24 memory_type, uint24 *tracknum );
COMP_OPT uint24 ML_GetItemTracknumByPosition( uint24 isSongBrowse, uint24 state, uint24 memory_type, uint24 pos, uint24 *tracknum, uint24 *id );
COMP_OPT uint24 ML_GetItemNameByPosition( uint24 isSongBrowse, uint24 state, uint24 memory_type, uint24 pos, FLASH_GROUP_NAME_T *name, uint24 *id );
//COMP_OPT uint24 ML_GetData( uint24 *array, uint24 pos );
extern uint24 _reentrant ML_GetData( uint24 *array, uint24 pos );
//COMP_OPT void ML_selectItemInfoByState( uint24 isSongBrowse, uint24 state, uint24 memory_type, uint24 *item_type, uint24 *id1, uint24 *id2 );
extern void _reentrant ML_selectItemInfoByState( uint24 isSongBrowse, uint24 state, uint24 memory_type, uint24 *item_type, uint24 *id1, uint24 *id2 );
COMP_OPT void ML_GetListAndID( uint24 isSongBrowse, uint24 state, uint24 memory_type, uint24 pos, uint24 *size, uint24 *id );
COMP_OPT uint24 ML_IsAtSongLevel( uint24 state );
COMP_OPT void ML_GetNameByState( uint24 id, uint24 state, uint24 memory_type, FLASH_GROUP_NAME_T *name );
COMP_OPT uint24 ML_updateListPointer( uint24 state, FLASH_GROUP_NAME_T *name1, FLASH_GROUP_NAME_T *name2, uint24 *pointer1, uint24 *pointer2 );
//COMP_OPT void ML_strcpy_Y2X( uint24 _X *dest, uint24 *src, uint24 len );
extern void _reentrant ML_strcpy_Y2X( uint24 _X *dest, uint24 *src, uint24 len );
COMP_OPT int8 ML_strcmp( FLASH_GROUP_NAME_T *src, FLASH_GROUP_NAME_T *dest );
//COMP_OPT void ML_charIndexToWordIndex( uint24 unicode, uint24 charIndex, uint24 *wordIndex, uint24 *relativeByteIndex );
extern void _reentrant ML_charIndexToWordIndex( uint24 unicode, uint24 charIndex, uint24 *wordIndex, uint24 *relativeByteIndex );
COMP_OPT void ML_GetNames( uint24 isSongBrowse, uint24 state, uint24 pos1, uint24 pos2, uint24 *id1, uint24 *id2 );
extern void _reentrant ML_strcpy( uint24 *dest, uint24 *src );

#ifdef BROWSE_AND_PLAY
COMP_OPT uint24 ML_GetIDFromCurrentWindow( uint24 highlight, uint24 *id, uint24 *id2 );
#endif

#ifdef PREVIOUS_SONG
COMP_OPT void ML_movePointerBackward( uint24 isSongBrowse, uint24 state, uint24 *flash_pos, uint24 flash_size, uint24 *sd_pos, uint24 sd_size );
#endif

#ifdef PLAY_PREV
COMP_OPT void ML_findLastItemPointer( uint24 isSongBrowse, uint24 state, uint24 *pos1, uint24 *id1, uint24 *pos2, uint24 *id2 );
#endif

#ifdef PLAY_MODULE
COMP_OPT uint24 ML_ConstructSongStruct(uint24 song_id, uint24 memory_type, uint24 isAudible, RAM_SONG_INFO_T *song_info, uint24 isVoice);
//COMP_OPT uint24 ML_ConstructAudibleStruct(uint24 aid, uint24 memory_type, RAM_SONG_INFO_T *song_info);
COMP_OPT uint24 ML_GetSongStructByPosition( uint24 state, uint24 pos, uint24 pos2, RAM_SONG_INFO_T *songStruct );
COMP_OPT uint24 ML_GetPathName( uint24 id, uint24 memory_type, uint24 item_type, uint24 *pathname );
COMP_OPT uint24 ML_GetLongFilename( uint24 id, uint24 memory_type, uint24 item_type, uint24 *long_filename );

#ifdef SHUFFLE
COMP_OPT void ML_AddToSortedList(uint24* list_ptr, uint24 list_size, uint24 entry);
COMP_OPT void ML_ShuffleInit(uint24 TOC);
COMP_OPT void	ML_startShuffle( uint24 *index );
COMP_OPT uint24 ML_InverseShuffle(uint24 song_num);

#ifdef SHUFFLE_NEXT
COMP_OPT uint24	ML_getNextShuffleSong( uint24 direction, uint24 playLevel, RAM_SONG_INFO_T *songStruct, uint24 *songIndex );
COMP_OPT uint24 ML_Shuffle(uint24 shuffle_order);
#endif

#endif	// SHUFFLE

#ifdef SHUFFLE_UTILITY
COMP_OPT uint24 ML_SearchSongID( uint24 memory_type, uint24 state, uint24 songID, uint24 *index );
COMP_OPT uint24 ML_GetSongInfoByID( uint24 memory_type, uint24 state, uint24 *index, uint24 *lastIndex );
#endif

#ifdef SHUFFLE_UTILITY2
COMP_OPT void	ML_ShuffleToNormal( uint24 *state, uint24 songID );
COMP_OPT void	ML_BuildSongPathFromSongID( uint24 songID );
//COMP_OPT uint24 GetItemInfoBySongID(uint24 memory_type, uint24 item_type, uint24 song_id, uint24 *pos);
#endif

COMP_OPT void	ML_updateSize( uint24 currentLvl );
#ifdef PLAY_ALL
COMP_OPT void	ML_buildSongPath( uint24 *state, uint24 startSongPathIndex );
#endif

#ifdef PLAY_NEXT
COMP_OPT uint24 ML_PlayNextSong( RAM_SONG_INFO_T *songStruct, uint24 *songIndex );
#endif

#ifdef PLAY_NEXT_AND_PREV
COMP_OPT void ML_updateSongPath( FLASH_GROUP_NAME_T *name, uint24 startIndex, uint24 *id1, uint24 *pos1, uint24 *id2, uint24 *pos2 );
COMP_OPT void ML_getWindowPosAndSize( uint24 currentLvl, uint24 treeLvl, uint24 *pos1, uint24 *pos2 );
#endif

#ifdef PLAY_ALL_START
COMP_OPT uint24 LOCAL_GetArtistSong_info( uint24 memory_type, uint24 artistID, uint24 sortedListIndex, uint24 *info );
COMP_OPT uint24 GetArtistSongSize( uint24 memory_type, uint24 artistID, uint24 *size );
//COMP_OPT uint24 GetArtistSongID( uint24 memory_type, uint24 artistID, uint24 sortedListIndex, uint24 *id );
#endif

#endif

extern unsigned long ReadRTC(void);

void _reentrant srand( WORD seed );
_reentrant WORD rand(void);
/*
COMP_OPT void debug_now(int a, int b, int *pt1, int *pt2)
{
__asm("debug");
}
*/
/*==================================================================================================
LOCAL VARIABLES
==================================================================================================*/
extern uint24               g_is_SD_inserted;
/* store the merged list of item from flash memory and SD memory */
extern uint24              	flash_name_list_size;
extern uint24 volatile     	flash_window_pos;			/* starting position of the sorted list in flash */
extern uint24              	sd_name_list_size;
extern uint24 volatile		sd_window_pos;				/* starting position of the sorted list in SD card */
extern uint24		 	window_merge_index;
extern uint24              	total_merge_num_item;
extern uint24			parent_window_pos[4];			// store the parents' flash_window_pos and sd_window_pos
extern uint24			parent_merge_item_size[2];		// store the total number of merged item of parents

extern int24			songIndexInMedia[2];

/* current state */
extern uint24              	current_state;
extern uint24              	browsePath[4];
/* has insert SD card */
extern uint24              	hasInsertSD;
/* store current folder path of play song list */
extern uint24              	songPath[5];
extern uint24              	play_flash_name_list_size;
extern uint24              	play_sd_name_list_size;
// store play song attributes
extern uint24              	org_songPath[5];
extern uint24              	org_play_flash_name_list_size;
extern uint24              	org_play_sd_name_list_size;
extern uint24			org_currentPlayAllLevel;
/* store the last requested item */
extern uint24			last_request_item;
//uint24 _X           	tmp_name_array[LEN_OF_NAME_IN_BYTE];
extern uint24 _X           	tmp_name_array[4];
//uint24 _X           	tmp_name_array2[LEN_OF_NAME_IN_BYTE];
//uint24 _X 		sec_temp_buf[NUM_OF_READWORD_PER_SEC];
extern uint24 _X 		sec_temp_buf_X[NUM_OF_READWORD_PER_SEC];
extern FLASH_GROUP_NAME_T	name[2];
// variables for play module
extern uint24 volatile				play_flash_window_pos;		/* starting position of the sorted list in flash for play song */
extern uint24 volatile				play_sd_window_pos;			/* starting position of the sorted list in flash for play song in SD card */
extern uint24					play_total_merge_num_item;
extern uint24					play_window_merge_index;
//extern uint24					currentSongID;				// the song id highlighted when start play song
//extern uint24					currentSongID_SD;			// the song id highlighted when start play song

/* current play all tree level */
extern uint24					playAllLevel;
extern uint24					isPlayAll;

/* last play song level */
extern uint24					lastPlaySongLevel;
extern uint24					currentPlayAllLevel;

/* play mode */
extern uint24	playMode;			/* bit 0 = current play mode; bit 1 = last play mode ; bit 2 = repeat on/off(0=off/1=on) */
extern uint24	lastSongID;

// variables for flash access
extern uint24 num_of_song_y;
extern uint24 num_of_item_y[MAX_NUM_OF_GROUP];

extern BOOL g_current_play_all;

/* for shuffle */
extern uint24 num_row[NumOfLevels], num_col[NumOfLevels];
extern uint24 g_tmp_offset[NumOfLevels], tmp_TOC[NumOfLevels];
extern uint24 Excess[22];
extern uint24 Excess_list_size;
extern uint24 shuffleOrder;
extern ON_THE_FLY_LIST_T _X on_the_fly_list;
extern uint24 OnTheGoThisID;
extern uint24 OnTheGoPlayID;
extern uint24 OnTheGoPlayMedia;
extern BOOL g_SortByTracknum;
/*==================================================================================================
GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
LOCAL FUNCTIONS
==================================================================================================*/
COMP_OPT void ML_GetListAndID( uint24 isSongBrowse, uint24 state, uint24 memory_type, uint24 pos, uint24 *size, uint24 *id ) {
	uint24 item_type;
	uint24 currentID1;
	uint24 currentID2;
	uint24 id1, id2;

	currentID1 = 0xFFFFFF;
	currentID2 = 0xFFFFFF;
	ML_selectItemInfoByState( isSongBrowse, state, memory_type, &item_type, &currentID1, &currentID2 );

	id1 = currentID1 & 0xFFF;
	id2 = currentID2 & 0xFFF;

	*size = 0;
	switch( state & 3 ) {
		case 0:
			ML_GetLvlOneItemSortListSize( memory_type, item_type, size );
//			if ( !ML_GetLvlOneItemID( memory_type, item_type, pos, id ) )
			if (!LOCAL_GetLvlOneItem_info(memory_type, item_type, pos, id))
				*id = 0xFFF;
			break;
		case 1:
			if ( id1 != 0xFFF ) {
//				ML_GetLvlTwoSongSortListSize( memory_type, id1, item_type, size );
				LOCAL_GetFlashAssociatedList_info(memory_type, id1, VOID_INDEX, item_type, VOID_INDEX, size);
			}
//			if ( !ML_GetLvlTwoSongID( memory_type, id1, item_type, pos, id ) )
			if (!LOCAL_GetFlashAssociatedList_info(memory_type, id1, VOID_INDEX, item_type, pos, id))
				*id = 0xFFF;
			break;
		default:
			if ( id1 != 0xFFF && id2 != 0xFFF ) {
//				ML_GetFlashArtistAlbumSongSortListSize( memory_type, id1, id2, size );
				LOCAL_GetFlashAssociatedList_info(memory_type, id1, id2, ITEM_ARTIST, VOID_INDEX, size);
			}
//			if ( !ML_GetFlashArtistAlbumSongID( memory_type, id1, id2, pos, id ) )
			if (!LOCAL_GetFlashAssociatedList_info(memory_type, id1, id2, ITEM_ARTIST, pos, id))
				*id = 0xFFF;
			break;

	}
}

/*================================================================================================*/
/* num_of_X, sorted ID */
//COMP_OPT uint24 LOCAL_GetLvlOneItem_info( uint24 memory_type, uint24 item_type, uint24 sortedListIndex, uint24 *info ) {
//	uint24 dtype;
//	uint24 sdtype;
//	uint24 itype;
//	uint24 temp;
//	uint24 i;
//
////	*info = 0x0;			/* browse request */
//	if (memory_type >= NUM_OF_MEDIA) return (FALSE);
//	dtype = NUM_OF_SONG_DTYPE;
//	itype = SONG_SORTLOOP;
//
//	// Modified by William
//	if ( item_type <= 4 && item_type != ITEM_TRACK ) {
//		dtype = item_type + NUM_OF_ARTIST_DTYPE;
//		itype = item_type;
//		sdtype = item_type * 5 + ARTIST_SORT_DTYPE;
//	}
//	else if ( item_type == ITEM_TRACK || (item_type >=ITEM_SONG_INFO_ARTIST && item_type <=ITEM_SONG_INFO_YEAR) ) {
//		sdtype = ( item_type == ITEM_TRACK ) ? SONG_ARRAY_DTYPE : (item_type - ITEM_SONG_INFO_ARTIST + SONG_INFO_ARTIST_DTYPE);
//	}
//	else {
//		if (memory_type != MEDIA_TYPE_FLASH) {
//			if (sortedListIndex == VOID_INDEX) {
//				return (FALSE);
//			} else {
//				*info = 0x00;	/* list is merged */
//				return (TRUE);
//			}
//		}
//		if (item_type == ITEM_ON_THE_GO) {
//			if (sortedListIndex == VOID_INDEX) {
//				*info = on_the_fly_list.num_of_id;
//			} else if (sortedListIndex < on_the_fly_list.num_of_id) {
//				*info = on_the_fly_list.store_list[sortedListIndex];
//			} else {
//				return (FALSE);
//			}
//			return (TRUE);
//		} else if (item_type == ITEM_VOICE) {
//			dtype = NUM_OF_VOICE_DTYPE;
//			sdtype = VOICE_ARRAY_DTYPE;
//		} else if (item_type == ITEM_FMREC) {
//			dtype = NUM_OF_FMREC_DTYPE;
//			sdtype = FMREC_ARRAY_DTYPE;
//		} else if (item_type == ITEM_PHOTO) {
//			dtype = NUM_OF_PHOTO_DTYPE;
//			sdtype = PHOTO_ARRAY_DTYPE;
//		} else if (item_type == ITEM_MVIDEO) {
//			dtype = NUM_OF_MVIDEO_DTYPE;
//			sdtype = MVIDEO_ARRAY_DTYPE;
//		} else {
//			temp = item_type - ITEM_1DAY;
//			dtype = temp + NUM_OF_NEW_1DAY_DTYPE;
//			sdtype = temp + NEW_1DAY_DTYPE;
//		}
//	}
//
//#ifdef BROWSE_PLAY_MODULE
//	if (ML_MemRead_Browse(memory_type, dtype, dtype, (uint24 *) &num_of_item_y[itype], ML_VALUE_STRUCT_SIZE) != SUCCESS) {
//#else
//	if (ML_MemRead(memory_type, dtype, dtype, (uint24 *) &num_of_item_y[itype], ML_VALUE_STRUCT_SIZE) != SUCCESS) {
//#endif
//		/* flash access error */
//		return (FALSE);
//	}
//	if (sortedListIndex == VOID_INDEX) {
//		*info = num_of_item_y[itype];
//		return (TRUE);
//	}
//
//	if (sortedListIndex < num_of_item_y[itype]) {
//#ifdef BROWSE_PLAY_MODULE
//		if (ML_MemRead_Browse(memory_type, sdtype, sortedListIndex, info, ML_INDEX_STRUCT_SIZE) != SUCCESS) {
//#else
//		if (ML_MemRead(memory_type, sdtype, sortedListIndex, info, ML_INDEX_STRUCT_SIZE) != SUCCESS) {
//#endif
//			/* flash access error */
//			return (FALSE);
//		}
//	} else {
//		return (FALSE);
//	}
//
//	return (TRUE);
//}

COMP_OPT uint24 ML_GetLvlOneItemSortListSize( uint24 memory_type, uint24 item_type, uint24 *size ) {
	return (LOCAL_GetLvlOneItem_info(memory_type, item_type, VOID_INDEX, size));
}

/* Associated size, Associated ID */
//COMP_OPT uint24 LOCAL_GetFlashAssociatedList_info( uint24 memory_type, uint24 lvl1_id, uint24 albumID, uint24 item_type, uint24 sortedListIndex, uint24 *info ) {
//	uint24 i;
//	uint24 j;
//	uint24 k;
//	uint24 pos;
//	uint24 dtype;
//	uint24 idtype;
//
//	if (memory_type >= NUM_OF_MEDIA) return (FALSE);
//	dtype = ARTIST_ASSOC_COUNT_DTYPE + item_type * 5;
//	idtype = dtype + 1;
//	if (LOCAL_GetLvlOneItem_info(memory_type, item_type, VOID_INDEX, &i) == FALSE) {
//		return (FALSE);
//	}
//	if (lvl1_id >= i) return (FALSE);
//
//	pos = 0;
//	if (lvl1_id > 0) {
//#ifdef BROWSE_PLAY_MODULE
//		if (ML_MemRead_Browse(memory_type, dtype, (lvl1_id-1), &pos, ML_INDEX_STRUCT_SIZE) != SUCCESS) {
//#else
//		if (ML_MemRead(memory_type, dtype, (lvl1_id-1), &pos, ML_INDEX_STRUCT_SIZE) != SUCCESS) {
//#endif
//			/* flash access error */
//			return (FALSE);
//		}
//	}
//#ifdef BROWSE_PLAY_MODULE
//	if (ML_MemRead_Browse(memory_type, dtype, lvl1_id, &k, ML_INDEX_STRUCT_SIZE) != SUCCESS) {
//#else
//	if (ML_MemRead(memory_type, dtype, lvl1_id, &k, ML_INDEX_STRUCT_SIZE) != SUCCESS) {
//#endif
//		/* flash access error */
//		return (FALSE);
//	}
//	k -= pos;
//	if (albumID == VOID_INDEX) {
//		if (sortedListIndex == VOID_INDEX) {
//			*info = k;
//			return (TRUE);
//		}
//		if (sortedListIndex >= k) return (FALSE);
//		pos += sortedListIndex;
//#ifdef BROWSE_PLAY_MODULE
//		if (ML_MemRead_Browse(memory_type, idtype, pos, info, ML_INDEX_STRUCT_SIZE) != SUCCESS) {
//#else
//		if (ML_MemRead(memory_type, idtype, pos, info, ML_INDEX_STRUCT_SIZE) != SUCCESS) {
//#endif
//			/* flash access error */
//			return (FALSE);
//		}
//		return (TRUE);
//	} else {
//		if (LOCAL_GetLvlOneItem_info(memory_type, ITEM_ALBUM, VOID_INDEX, &i) == FALSE) {
//			return (FALSE);
//		}
//		if (albumID >= i) return (FALSE);
//	}
//
//	for (i=0;i<k;i++) {
//#ifdef BROWSE_PLAY_MODULE
//		if (ML_MemRead_Browse(memory_type, idtype, pos, &j, ML_INDEX_STRUCT_SIZE) != SUCCESS) {
//#else
//		if (ML_MemRead(memory_type, idtype, pos, &j, ML_INDEX_STRUCT_SIZE) != SUCCESS) {
//#endif
//			/* flash access error */
//			return (FALSE);
//		}
//		if (j == albumID) {
//			break;
//		}
//		pos++;
//	}
//	if (i == k) return (FALSE);
//
//	i = 0;
//	if (pos > 0) {
//#ifdef BROWSE_PLAY_MODULE
//		if (ML_MemRead_Browse(memory_type, ARTIST_SUBASSOC_COUNT_DTYPE, (pos-1), &i, ML_INDEX_STRUCT_SIZE) != SUCCESS) {
//#else
//		if (ML_MemRead(memory_type, ARTIST_SUBASSOC_COUNT_DTYPE, (pos-1), &i, ML_INDEX_STRUCT_SIZE) != SUCCESS) {
//#endif
//			/* flash access error */
//			return (FALSE);
//		}
//	}
//#ifdef BROWSE_PLAY_MODULE
//	if (ML_MemRead_Browse(memory_type, ARTIST_SUBASSOC_COUNT_DTYPE, pos, &k, ML_INDEX_STRUCT_SIZE) != SUCCESS) {
//#else
//	if (ML_MemRead(memory_type, ARTIST_SUBASSOC_COUNT_DTYPE, pos, &k, ML_INDEX_STRUCT_SIZE) != SUCCESS) {
//#endif
//		/* flash access error */
//		return (FALSE);
//	}
//	k -= i;
//	if (sortedListIndex == VOID_INDEX) {
//		*info = k;
//		return (TRUE);
//	}
//	if (sortedListIndex >= k) return (FALSE);
//	i += sortedListIndex;
//#ifdef BROWSE_PLAY_MODULE
//	if (ML_MemRead_Browse(memory_type, ARTIST_SUBASSOC_CID_DTYPE, i, info, ML_INDEX_STRUCT_SIZE) != SUCCESS) {
//#else
//	if (ML_MemRead(memory_type, ARTIST_SUBASSOC_CID_DTYPE, i, info, ML_INDEX_STRUCT_SIZE) != SUCCESS) {
//#endif
//		/* flash access error */
//		return (FALSE);
//	}
//	return (TRUE);
//}

COMP_OPT void ML_GetNames( uint24 isSongBrowse, uint24 state, uint24 pos1, uint24 pos2, uint24 *id1, uint24 *id2 ) {
	ML_GetItemNameByPosition( isSongBrowse, state, MEDIA_TYPE_FLASH, pos1, &name[0], id1 );
	ML_GetItemNameByPosition( isSongBrowse, state, MEDIA_TYPE_SD, pos2, &name[1], id2 );
}

COMP_OPT uint24 ML_GetItemNameByPosition( uint24 isSongBrowse, uint24 state, uint24 memory_type, uint24 pos, FLASH_GROUP_NAME_T *name, uint24 *id ) {

	uint24 *flashsize;
	uint24 *sdsize;
	uint24 size;

	if ( isSongBrowse ) {
		flashsize = &play_flash_name_list_size;
		sdsize = &play_sd_name_list_size;
	}
	else {
		flashsize = &flash_name_list_size;
		sdsize = &sd_name_list_size;
	}

	if ( (memory_type == MEDIA_TYPE_FLASH && (*flashsize == 0 || pos >= *flashsize) ) ||
		(memory_type == MEDIA_TYPE_SD && (*sdsize == 0 || pos >= *sdsize) ) ) {
			name->name[0] = 0;
			*id = 0xFFF;
	}
	else {
//		ML_GetIDFromSortedList( isSongBrowse, state, memory_type, pos, id );
		ML_GetListAndID( isSongBrowse, state, memory_type, pos, &size, id );
		if (( state >= STATE_1DAY && state <= STATE_1MONTH ) || (state == STATE_ONTHEGO))
			state = STATE_TRACK_LVL_1;
		ML_GetNameByState( *id, state, memory_type, name );
	}

	return 1;
}

COMP_OPT void ML_GetTracknums( uint24 isSongBrowse, uint24 state, uint24 pos1, uint24 pos2, uint24 *tracknum1, uint24 *tracknum2, uint24 *id1, uint24 *id2) {
	ML_GetItemTracknumByPosition( isSongBrowse, state, MEDIA_TYPE_FLASH, pos1, tracknum1, id1 );
	ML_GetItemTracknumByPosition( isSongBrowse, state, MEDIA_TYPE_SD, pos2, tracknum2, id2 );
}

COMP_OPT uint24 ML_GetItemTracknumByPosition( uint24 isSongBrowse, uint24 state, uint24 memory_type, uint24 pos, uint24 *tracknum, uint24 *id ) {

	uint24 *flashsize;
	uint24 *sdsize;
	uint24 size;

	if ( isSongBrowse ) {
		flashsize = &play_flash_name_list_size;
		sdsize = &play_sd_name_list_size;
	}
	else {
		flashsize = &flash_name_list_size;
		sdsize = &sd_name_list_size;
	}

	if ( (memory_type == MEDIA_TYPE_FLASH && (*flashsize == 0 || pos >= *flashsize) ) ||
		(memory_type == MEDIA_TYPE_SD && (*sdsize == 0 || pos >= *sdsize) ) ) {
			*tracknum = *id = 0xFFF;
	}
	else {
		ML_GetListAndID( isSongBrowse, state, memory_type, pos, &size, id );
		ML_GetItemTracknum( *id, memory_type, tracknum );
	}

	return 1;
}

COMP_OPT uint24 ML_GetItemTracknum ( uint24 id, uint24 memory_type, uint24 *tracknum ) {

	*tracknum = 0xfff;

	if (LOCAL_GetLvlOneItem_info(memory_type, ITEM_TRACK, VOID_INDEX, (uint24 *) &num_of_item_y[SONG_SORTLOOP]) == FALSE) {
		return (FALSE);
	}

	if (id >= num_of_item_y[SONG_SORTLOOP]) return (FALSE);

#ifdef BROWSE_PLAY_MODULE
	return (!ML_MemRead_Browse(memory_type, TRACKNUM_ARRAY_DTYPE, id, tracknum, ML_VALUE_STRUCT_SIZE));
#else
	return (!ML_MemRead(memory_type, TRACKNUM_ARRAY_DTYPE, id, tracknum, ML_VALUE_STRUCT_SIZE));
#endif
}

COMP_OPT uint24 ML_IsAtSongLevel( uint24 state ) {
	return ( (!isAtLevel1( state ) && state != STATE_ARTIST_LVL_2) || state == STATE_TRACK_LVL_1 || (state >= STATE_1DAY && state <= STATE_MVIDEO) );
}

/* Get Name depending on the current state */
COMP_OPT void ML_GetNameByState( uint24 id, uint24 state, uint24 memory_type, FLASH_GROUP_NAME_T *name ) {
	uint24 year = 0;
	uint24 item_type;

	name->name[0] = 0;

	if (( isAtLevel1( state ) ) || (state == STATE_VOICE) || (state == STATE_FMREC) || (state==STATE_PHOTO) || (state==STATE_MVIDEO))
		item_type = state>>2;
	else if ( ML_IsAtSongLevel( state ) )
		item_type = ITEM_TRACK;
	else
		item_type = ITEM_ALBUM;

	if ( state != STATE_YEAR_LVL_1 )
		ML_GetItemName( id, memory_type, item_type, name );
	else {
#ifdef BROWSE_MODULE
		ML_GetSongYear( id, memory_type, &name->name[0] );
		name->name[1] = 0;
		name->unicode = 1;
#endif
	}
}

/*==================================================================================================
FUNCTION: ML_GetItemName()

DESCRIPTION:
get ArtistName, AlbumName, GenreName, SongName.

ARGUMENTS PASSED:
uint24 id, uint24 memory_type, uint24 item_type, FLASH_GROUP_NAME_T *itemStruct

RETURN VALUE:
uint24

PRE-CONDITIONS:
None

POST-CONDITIONS:
None

IMPORTANT NOTES:
None

==================================================================================================*/
COMP_OPT uint24 ML_GetItemName( uint24 id, uint24 memory_type, uint24 item_type, FLASH_GROUP_NAME_T *itemStruct ) {
	uint24 ndtype;
	uint24 itype;
#ifdef _PRINTSONGID_
	uint24 *tgtname_p;
	uint24 i;
	uint24 j;
#endif

	itemStruct->name[0] = 0x0;	/* browse request */
	if (id >= MAX_NUM_OF_SONG) {
		id -= MAX_NUM_OF_SONG;
		memory_type = MEDIA_TYPE_SD;
	}
#ifdef _AUDIBLE_
	if (item_type == ITEM_AUDIBLE) {
		itype = SONG_SORTLOOP;
		ndtype = AUDIBLE_NAME_DTYPE;
	} else
#endif
	{
		itype = item_type;
		ndtype = item_type;
	}
	if ((item_type == ITEM_VOICE) || (item_type == ITEM_FMREC) || (item_type == ITEM_PHOTO) || (item_type == ITEM_MVIDEO)) {
		itype = SONG_SORTLOOP;
		if (item_type == ITEM_VOICE) {
			ndtype = VOICE_NAME_DTYPE;
		} else if(item_type == ITEM_FMREC) {
			ndtype = FMREC_NAME_DTYPE;
		} else if(item_type == ITEM_PHOTO) {
			ndtype = PHOTO_NAME_DTYPE;
		} else {
			ndtype = MVIDEO_NAME_DTYPE;
		}
	}
	if ( memory_type != MEDIA_TYPE_SD || item_type != ITEM_AUDIBLE ) {
		if (LOCAL_GetLvlOneItem_info(memory_type, item_type, VOID_INDEX, (uint24 *) &num_of_item_y[itype]) == FALSE) {
			return (FALSE);
		}
		if (id >= num_of_item_y[itype]) return (FALSE);
	}
//	ML_MemRead(memory_type, ARTIST_SORT_DTYPE, id, (uint24 *) &i, ML_LIST_STRUCT_SIZE);	/* input id=sorted_order */
//	ML_MemRead(memory_type, ARTIST_NAME_DTYPE, i, (uint24 *) itemStruct, ML_SONG_NAME_STRUCT_SIZE);	/* input id=sorted_order */
#ifdef BROWSE_PLAY_MODULE
	if (ML_MemRead_Browse(memory_type, ndtype, id, (uint24 *) itemStruct, ML_SONG_NAME_STRUCT_SIZE) != SUCCESS) {	/* input id=input_sqeuence_ID */
#else
	if (ML_MemRead(memory_type, ndtype, id, (uint24 *) itemStruct, ML_SONG_NAME_STRUCT_SIZE) != SUCCESS) {	/* input id=input_sqeuence_ID */
#endif
		/* flash access error */
		return (FALSE);
	}
#ifdef _PRINTSONGID_
#ifdef _PRINTSONGID_UNICODE_
	/* name format: "XXXNAME": XXX=3 digit id # : NAME=original name */
	if (itemStruct->unicode != 0x0) {
		/* unicode & mix */
	} else
#endif	/* _PRINTSONGID_UNICODE_ */
	{
		/* name format: "XXXNAME": XXX=3 digit id # : NAME=original name */
		tgtname_p = (uint24 *) itemStruct;
		for (i=((LEN_OF_NAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD)-1);i>0;i--) {
			tgtname_p[i] = tgtname_p[i-1];
		}
		i = id/100;
		/* memory_type is display instead of 3 digits */
		/* name format: "YXXNAME": if flash then Y=0:if SD then Y=1 : XX=2 digit id # : NAME=original name */
		tgtname_p[0] = memory_type+0x30;
		j = (id - (i*100))/10;
		tgtname_p[0] = tgtname_p[0] | (((j+0x30)<<8) & 0x00FF00);
		i = (id - (i*100) - (j*10));
		tgtname_p[0] = tgtname_p[0] | (((i+0x30)<<16) & 0xFF0000);
	}
#endif
	return (TRUE);
}

/*==================================================================================================
FUNCTION: ML_GetSongYear()

DESCRIPTION:
get song year.

ARGUMENTS PASSED:
uint24 id, uint24 memory_type, uint24 *year

RETURN VALUE:
uint24

PRE-CONDITIONS:
None

POST-CONDITIONS:
None

IMPORTANT NOTES:
None

==================================================================================================*/
COMP_OPT uint24 ML_GetSongYear( uint24 id, uint24 memory_type, uint24 *year ) {
	*year = 0x00;
	if (LOCAL_GetLvlOneItem_info(memory_type, ITEM_TRACK, VOID_INDEX, (uint24 *) &num_of_item_y[SONG_SORTLOOP]) == FALSE) {
		return (FALSE);
	}
	if (id >= num_of_item_y[SONG_SORTLOOP]) return (FALSE);
	/* because #define SUCCESS 0 */
#ifdef BROWSE_PLAY_MODULE
	return (!ML_MemRead_Browse(memory_type, SONG_YEAR_DTYPE, id, year, ML_VALUE_STRUCT_SIZE));
#else
	return (!ML_MemRead(memory_type, SONG_YEAR_DTYPE, id, year, ML_VALUE_STRUCT_SIZE));
#endif
}

COMP_OPT uint24 ML_updateListPointer( uint24 state, FLASH_GROUP_NAME_T *name1, FLASH_GROUP_NAME_T *name2, uint24 *pointer1, uint24 *pointer2 ) {
	int24 tmp;
	uint24 ret;

	ret = 0;
	if ( name2->name[0] == 0 ) {
		*pointer1 = *pointer1 + 1;
		ret = 1;
	}
	else if ( name1->name[0]==0 ) {
		*pointer2 = *pointer2 + 1;
		ret = 2;
	}
	else
	{
		tmp = ML_strcmp( name1, name2 );
		if ( tmp <= 0 ) {
			*pointer1 = *pointer1 + 1;
			ret = 1;
		}
		if ( tmp > 0 || ((tmp-0)|ML_IsAtSongLevel( state )) == 0 ) {
			*pointer2 = *pointer2 + 1;
			ret += 2;
		}
	}

	return ret;
}

COMP_OPT int8 ML_strcmp( FLASH_GROUP_NAME_T *src, FLASH_GROUP_NAME_T *dest ) {
	uint24 totalWordSize = LEN_OF_NAME_IN_BYTE;
//	uint24 srcName[LEN_OF_NAME_IN_BYTE];
//	uint24 destName[LEN_OF_NAME_IN_BYTE];
	uint24 i, j;
	uint24 startStringIndex, startStringIndex2;
	uint24 wordIndex, relativeByteIndex;
	uint24 char1, char2;
	uint24 unicode1, unicode2;

	unicode1 = src->unicode;
	unicode2 = dest->unicode;

	ML_strcpy_Y2X( tmp_name_array, src->name, 4 );
#ifdef BROWSE_PLAY_MODULE
	startStringIndex = ML_strhead_pos_BROWSE(unicode1, tmp_name_array);
#else
	startStringIndex = ML_strhead_pos(unicode1, tmp_name_array);
#endif
	ML_strcpy_Y2X( tmp_name_array, dest->name, 4 );
#ifdef BROWSE_PLAY_MODULE
	startStringIndex2 = ML_strhead_pos_BROWSE(unicode2, tmp_name_array);
#else
	startStringIndex2 = ML_strhead_pos(unicode2, tmp_name_array);
#endif

	/*
	ML_extract_string( src->unicode, srcName, (uint24 _X *)tmp_name_array, totalWordSize );
	ML_strcpy_Y2X( tmp_name_array, dest->name );
	ML_extract_string( dest->unicode, destName, (uint24 _X*)tmp_name_array, totalWordSize );
	*/
	for ( i=0;i<totalWordSize;i++ ) {
		ML_charIndexToWordIndex( unicode1, (i+startStringIndex), &wordIndex, &relativeByteIndex );
		tmp_name_array[0] = src->name[wordIndex];
#ifdef BROWSE_PLAY_MODULE
		char1 = ML_extract_byte_BROWSE(unicode1, tmp_name_array, relativeByteIndex);
#else
		char1 = ML_extract_byte(unicode1, tmp_name_array, relativeByteIndex);
#endif

		ML_charIndexToWordIndex( unicode2, (i+startStringIndex2), &wordIndex, &relativeByteIndex );
		tmp_name_array[0] = dest->name[wordIndex];
#ifdef BROWSE_PLAY_MODULE
		char2 = ML_extract_byte_BROWSE(unicode2, tmp_name_array, relativeByteIndex);
#else
		char2 = ML_extract_byte(unicode2, tmp_name_array, relativeByteIndex);
#endif

		if ( (char1 | char2) == 0 )
			return 0;
		else if ( char1 < char2 )
			return -1;
		else if ( char1 > char2 )
			return 1;

		/*
		if ( (srcName[i] | destName[i]) == 0 )
		return 0;
		else if ( srcName[i] < destName[i] )
		return -1;
		else if ( srcName[i] > destName[i] )
		return 1;
		*/
	}

	return 0;
}

/*================================================================================================*/
/* Utilities functions */
#ifdef BROWSE_AND_PLAY
/* Get the ID of highlight item in current window */
COMP_OPT uint24 ML_GetIDFromCurrentWindow( uint24 highlight, uint24 *id, uint24 *id2 ) {
	/*
	if ( (merge_id_list_flash[highlight] == 0xFFFFFF && merge_id_list_sd[highlight] == 0xFFFFFF) ||
	( (merge_id_list_flash[highlight] & 0xFFF) == 0xFFF && (merge_id_list_sd[highlight] & 0xFFF) == 0xFFF ) ||
	highlight > g_iBrowseWindowSize ) {
	*id = 0xFFFFFF;
	*id2 = 0xFFFFFF;
	return 0;
	}
	*/
	*id = merge_id_list_flash[highlight];
	*id2 = merge_id_list_sd[highlight];

	return 1;
}
#endif

// Darwin
#ifdef PREVIOUS_SONG
COMP_OPT void ML_movePointerBackward( uint24 isSongBrowse, uint24 state, uint24 *flash_pos, uint24 flash_size, uint24 *sd_pos, uint24 sd_size ) {
	int24 temp;
	uint24 id1, id2;
    uint24 tracknum1, tracknum2;

    if (flash_size <= 0 || *flash_pos <=0) {
        *sd_pos = *sd_pos - 1;
        return;
    }

    if (sd_size <= 0 || *sd_pos <= 0) {
        *flash_pos = *flash_pos - 1;
        return;
    }

    tracknum1 = tracknum2 = id1 = id2 = 0;
	if (g_SortByTracknum && ((state==STATE_ARTIST_LVL_3) || (state==STATE_ALBUM_LVL_2)))
        ML_GetTracknums( isSongBrowse, state, (*flash_pos-1), *sd_pos, &tracknum1, &tracknum2, &id1, &id2);

    if ((id1 != 0xfff) && (id2 != 0xfff) && (tracknum1 == tracknum2)) {
        ML_GetNames( isSongBrowse, state, (*flash_pos-1), *sd_pos, &id1, &id2 );
        if ( ML_IsAtSongLevel( state ) ) {
            if (ML_strcmp(&name[0], &name[1]) == 0) {
                *flash_pos = *flash_pos - 1;
                return;
            }
        }
    }

    if (g_SortByTracknum && ((state==STATE_ARTIST_LVL_3) || (state==STATE_ALBUM_LVL_2))) {
        ML_GetItemTracknumByPosition(isSongBrowse, state, MEDIA_TYPE_SD, (*sd_pos-1), &tracknum2, &id2);

        if (tracknum1 > tracknum2) {
            *flash_pos = *flash_pos - 1;
            return;
        } else if (tracknum1 < tracknum2) {
		    *sd_pos = *sd_pos - 1;
		    return;
		}

	    ML_GetNames( isSongBrowse, state, (*flash_pos-1), (*sd_pos-1), &id1, &id2 );
	}
	else
	    ML_GetItemNameByPosition( isSongBrowse, state, MEDIA_TYPE_SD, (*sd_pos-1), &name[1], &id2 );

   	temp = ML_strcmp( &name[0], &name[1] );
   	switch( temp ) {
   		case -1:
        	*sd_pos = *sd_pos - 1;
   			break;
        case 0:
   			if ( !ML_IsAtSongLevel( state ) )
        		*flash_pos = *flash_pos - 1;
   			*sd_pos = *sd_pos - 1;
        	break;
        default:
	        *flash_pos = *flash_pos - 1;
	        break;
    }
}
#endif

#ifdef PLAY_PREV
COMP_OPT void ML_findLastItemPointer( uint24 isSongBrowse, uint24 state, uint24 *pos1, uint24 *id1, uint24 *pos2, uint24 *id2 ) {
	uint24 temp, temp2;
	uint24 ret;
	uint24 *flash_size, *sd_size;
    uint24 tracknum1, tracknum2;

	if ( isSongBrowse == 1 ) {
		*pos1 = ( play_flash_name_list_size > 0 ) ? play_flash_name_list_size - 1 : 0;
		*pos2 = ( play_sd_name_list_size > 0 ) ? play_sd_name_list_size - 1 : 0;
		flash_size = &play_flash_name_list_size;
		sd_size = &play_sd_name_list_size;
	}
	else {
		*pos1 = ( flash_name_list_size > 0 ) ? flash_name_list_size - 1 : 0;
		*pos2 = ( sd_name_list_size > 0 ) ? sd_name_list_size - 1 : 0;
		flash_size = &flash_name_list_size;
		sd_size = &sd_name_list_size;
	}

	while (g_SortByTracknum && ((state==STATE_ARTIST_LVL_3) || (state==STATE_ALBUM_LVL_2))) {
	    ML_GetTracknums( isSongBrowse, state, *pos1, *pos2, &tracknum1, &tracknum2, id1, id2);

        if (tracknum1 < tracknum2) {
			if ( *sd_size > 0 ) {
			    *pos1 = *pos1 + 1;
				name[0].name[0] = 0;
			}
        }
        else if (tracknum1 > tracknum2) {
            if ( *flash_size > 0 ) {
				*pos2 = *pos2 + 1;
				name[1].name[0] = 0;
			}
        }
        else
            break;

        return;
    }

	ML_GetNames( isSongBrowse, state, *pos1, *pos2, id1, id2 );
//	ML_GetItemNameByPosition( 1, state, MEDIA_TYPE_FLASH, *pos1, &name[0], id1 );
//	ML_GetItemNameByPosition( 1, state, MEDIA_TYPE_SD, *pos2, &name[1], id2 );

	// update the pointers to make sure they point to the last item of merge list
	temp = *pos1;
	temp2 = *pos2;
	ret = ML_updateListPointer( state, &name[0], &name[1], pos1, pos2 );
//	if ( temp != *pos1 && temp2 != *pos2 ) {

	switch ( ret ) {
		case 3:
			*pos2 = temp2;
			if ( !ML_IsAtSongLevel( state ) )
				*pos1 = temp;
			break;

		case 1:
//			if ( play_sd_name_list_size > 0 )
			if ( *sd_size > 0 )
				name[0].name[0] = 0;
			else
				*pos1 = temp;
			break;

		default:
//			if ( play_flash_name_list_size > 0 )
			if ( *flash_size > 0 )
				name[1].name[0] = 0;
			else
				*pos2 = temp2;
			break;
	}
}
#endif

#ifdef PLAY_MODULE
COMP_OPT uint24 ML_GetSongStructByPosition( uint24 state, uint24 pos, uint24 pos2, RAM_SONG_INFO_T *songStruct ) {
	uint24 id;
	uint24 id2;
	uint24 temp;
	uint24 isAudible;

	uint24 funcID;
	uint24 memory_type;
    uint24 tracknum1, tracknum2;
    BOOL skip = FALSE;

    if (g_SortByTracknum && ((state==STATE_ARTIST_LVL_3) || (state==STATE_ALBUM_LVL_2))) {
        ML_GetTracknums (1, state, pos, pos2, &tracknum1, &tracknum2, &id, &id2);

        if (tracknum1 < tracknum2) {
            funcID = id;
            memory_type = MEDIA_TYPE_FLASH;
            skip = TRUE;
        }
        else if (tracknum1 > tracknum2) {
		    funcID = id2;
		    memory_type = MEDIA_TYPE_SD;
            skip = TRUE;
        }
    }

    if (skip == FALSE)
    {
	ML_GetNames( 1, state, pos, pos2, &id, &id2 );
	funcID = id;
	memory_type = MEDIA_TYPE_FLASH;
	temp = ML_updateListPointer( state, &name[0], &name[1], &pos, &pos2 );
	if ( temp == 2 )
	{
		funcID = id2;
		memory_type = MEDIA_TYPE_SD;
	}
	}
	if ( funcID != 0xFFF ) {
//		isAudible = state / STATE_AUDIBLE;
//		ML_ConstructSongStruct( funcID, memory_type, songStruct );
		if (state == STATE_VOICE) {
			ML_ConstructSongStruct( funcID, memory_type, (state == STATE_AUDIBLE), songStruct, 1 );
		} else if (state == STATE_FMREC) {
			ML_ConstructSongStruct( funcID, memory_type, (state == STATE_AUDIBLE), songStruct, 2 );
		} else if (state == STATE_PHOTO) {
			ML_ConstructSongStruct( funcID, memory_type, (state == STATE_AUDIBLE), songStruct, 3 );
		} else if (state == STATE_MVIDEO) {
			ML_ConstructSongStruct( funcID, memory_type, (state == STATE_AUDIBLE), songStruct, 4 );
		} else {
			ML_ConstructSongStruct( funcID, memory_type, (state == STATE_AUDIBLE), songStruct, 0 );
		}
	}
	return 1;
}

/*================================================================================================*/
/* Functions for play module */

/* add for extract song_info from musiclib, 05/02/14 */
/* song_info->track_number _not_filled_ */
/* isVoice = [0,1,2] */
COMP_OPT uint24 ML_ConstructSongStruct(uint24 song_id, uint24 memory_type, uint24 isAudible, RAM_SONG_INFO_T *song_info, uint24 isVoice) {
	uint24 i;
	uint24 item_id;
	uint24 item_type;
	FLASH_GROUP_NAME_T item_name;
	uint24 temp;
	uint24 *namePts[4];

//	uint24 song_id;

//	song_id = 2;

	if (isVoice>0) {
		if (isVoice == 1) {
			item_type = ITEM_VOICE;
		} else if (isVoice == 2) {
			item_type = ITEM_FMREC;
		} else if (isVoice == 3) {
			item_type = ITEM_PHOTO;
		} else {
			item_type = ITEM_MVIDEO;
		}
		song_info->artist_name[0] = 0x00;
		song_info->album_name[0] = 0x00;
		song_info->genre_name[0] = 0x00;

		song_info->unicode = 0x00;
		if (ML_GetItemName(song_id, memory_type, item_type, &item_name) == FALSE) {
			return (FALSE);
		}
		for (i=0;i<(LEN_OF_NAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD);i++) {
			song_info->song_name[i] = item_name.name[i];
		}
		if (item_name.unicode == BITSET_NAME_ALLASCII) {
			song_info->unicode |= BITCHK_SONG_ALLASCII;
		} else if (item_name.unicode == BITSET_NAME_UNICODE) {
			song_info->unicode |= BITCHK_SONG_UNICODE;
		} else {
			song_info->unicode |= BITCHK_SONG_MIXCODE;
		}
		/* path */
	} else {
		namePts[0] = song_info->artist_name;
		namePts[1] = song_info->album_name;
		namePts[2] = song_info->genre_name;
		namePts[3] = song_info->song_name;

		if ( !isAudible && song_id >= MAX_NUM_OF_SONG) {
			song_id -= MAX_NUM_OF_SONG;
			memory_type = MEDIA_TYPE_SD;
		}
		song_info->unicode = 0x00;
		for ( i=0;i<5;i++ ) {
			if ( (!isAudible) && (i != 3) ) {
				if (LOCAL_GetLvlOneItem_info(memory_type, (i+5), song_id, &item_id) == FALSE) {
					return (FALSE);
				}
			}
			else
				item_id = song_id;

			if ( i != 4 ) {
				if ( isAudible && i < 3 ) {
					name[0].name[0] = 0;
					name[0].unicode = 1;
				}
				else {
//					item_type = ( !isAudible ) ? i : ITEM_AUDIBLE;
					item_type = ( isAudible ) ? ITEM_AUDIBLE : i;
//					if (ML_GetItemName(item_id, memory_type, i, &name[0]) == FALSE) {
					if (ML_GetItemName(item_id, memory_type, item_type, &name[0]) == FALSE) {
						return (FALSE);
					}
				}

				ML_strcpy( namePts[i], name[0].name );
				temp = i<<1;
				song_info->unicode |= (name[0].unicode<<temp);
			}
			else if ( !isAudible ) {
				if (ML_GetSongYear(item_id, memory_type, &song_info->year ) == FALSE) {
					return (FALSE);
				}
			}
		}
		/* path */
		item_type = ( !isAudible ) ? ITEM_TRACK : ITEM_AUDIBLE;
	}
//	if (ML_GetPathName(song_id, memory_type, ITEM_TRACK, song_info->path_name) == FALSE ) {
	if (ML_GetPathName(song_id, memory_type, item_type, song_info->path_name) == FALSE ) {
		return (FALSE);
	}

	// Retrieve Long filename
	if ((item_type == ITEM_VOICE) || (item_type == ITEM_FMREC) || (item_type == ITEM_PHOTO) || (item_type == ITEM_MVIDEO)) {
		if ((song_info->unicode & BITCHK_SONG_UNICODE) != 0) {
			for (i=0;i<(LEN_OF_NAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD);i++) {
				song_info->long_filename[i] = song_info->song_name[i];
			}
		}
	} else {
		if (ML_GetLongFilename(song_id, memory_type, item_type, song_info->long_filename) == FALSE ) {
			return (FALSE);
		}
	}

	if ( isAudible && song_id >= MAX_NUM_OF_SONG)
		memory_type = MEDIA_TYPE_SD;

	playMode = ( memory_type == MEDIA_TYPE_FLASH ) ? (playMode & 7) : (playMode | 8);
//	lastSongID = song_id;
	lastSongID = ((isAudible * 4096) | song_id);
	if (IsPlayOnTheGo) {
		OnTheGoPlayMedia = memory_type;
		OnTheGoPlayID = lastSongID;
	}
	return (TRUE);
}
/*==================================================================================================
FUNCTION: ML_GetPathName()

DESCRIPTION:
get song path name.

ARGUMENTS PASSED:
uint24 id, uint24 memory_type, uint24 *pathname

RETURN VALUE:
uint24

PRE-CONDITIONS:
None

POST-CONDITIONS:
None

IMPORTANT NOTES:
None

==================================================================================================*/
COMP_OPT uint24 ML_GetPathName( uint24 id, uint24 memory_type, uint24 item_type, uint24 *pathname ) {
//	uint24 i;
//	uint24 j;
	uint24 ndtype;

	pathname[0] = 0x0;	/* browse request */
	if (id >= MAX_NUM_OF_SONG) {
		id -= MAX_NUM_OF_SONG;
		memory_type = MEDIA_TYPE_SD;
	}
#ifdef _AUDIBLE_
	if (item_type == ITEM_AUDIBLE) {
		ndtype = AUDIBLE_PATH_DTYPE;
	} else
#endif
	{
		ndtype = PATH_NAME_DTYPE;
	}
	if (item_type == ITEM_VOICE) {
		ndtype = VOICE_PATH_DTYPE;
	} else if  (item_type == ITEM_FMREC) {
		ndtype = FMREC_PATH_DTYPE;
	} else if  (item_type == ITEM_PHOTO) {
		ndtype = PHOTO_PATH_DTYPE;
	} else if  (item_type == ITEM_MVIDEO) {
		ndtype = MVIDEO_PATH_DTYPE;
	}
	if ( memory_type != MEDIA_TYPE_SD || item_type != ITEM_AUDIBLE ) {
		if (LOCAL_GetLvlOneItem_info(memory_type, item_type, VOID_INDEX, (uint24 *) &num_of_item_y[SONG_SORTLOOP]) == FALSE) {
			return (FALSE);
		}
		if (id >= num_of_item_y[SONG_SORTLOOP]) return (FALSE);
	}
//	ML_MemRead(memory_type, SONG_ARRAY_DTYPE, id, (uint24 *) &j, ML_INDEX_STRUCT_SIZE);
//	ML_MemRead(memory_type, PATH_NAME_DTYPE, j, pathname, ML_PATH_NAME_STRUCT_SIZE);
#ifdef BROWSE_PLAY_MODULE
	if (ML_MemRead_Browse(memory_type, ndtype, id, pathname, ML_PATH_NAME_STRUCT_SIZE) != SUCCESS) {
#else
	if (ML_MemRead(memory_type, ndtype, id, pathname, ML_PATH_NAME_STRUCT_SIZE) != SUCCESS) {
#endif
		/* flash access error */
		return (FALSE);
	}
	return (TRUE);
}

/*==================================================================================================
FUNCTION: ML_GetLongFilename()

DESCRIPTION:
get song path name.

ARGUMENTS PASSED:
uint24 id, uint24 memory_type, uint24 *pathname

RETURN VALUE:
uint24

PRE-CONDITIONS:
None

POST-CONDITIONS:
None

IMPORTANT NOTES:
None

==================================================================================================*/
COMP_OPT uint24 ML_GetLongFilename( uint24 id, uint24 memory_type, uint24 item_type, uint24 *long_filename ) {
	uint24	ndtype = 0;

	long_filename[0] = 0x0;	/* browse request */
	if (id >= MAX_NUM_OF_SONG) {
		id -= MAX_NUM_OF_SONG;
		memory_type = MEDIA_TYPE_SD;
	}
#ifdef _AUDIBLE_
	if (item_type == ITEM_AUDIBLE) {
		ndtype = AUDIBLE_LONG_FILENAME_DTYPE;
	} else
#endif
	{
		ndtype = LONG_FILENAME_DTYPE;
	}
	if ( memory_type != MEDIA_TYPE_SD || item_type != ITEM_AUDIBLE ) {
		if (LOCAL_GetLvlOneItem_info(memory_type, item_type, VOID_INDEX, (uint24 *) &num_of_item_y[SONG_SORTLOOP]) == FALSE) {
			return (FALSE);
		}
		if (id >= num_of_item_y[SONG_SORTLOOP]) return (FALSE);
	}
#ifdef BROWSE_PLAY_MODULE
	if (ML_MemRead_Browse(memory_type, ndtype, id, long_filename, ML_LONG_FILENAME_STRUCT_SIZE) != SUCCESS) {
#else
	if (ML_MemRead(memory_type, ndtype, id, long_filename, ML_LONG_FILENAME_STRUCT_SIZE) != SUCCESS) {
#endif
		/* flash access error */
		return (FALSE);
	}
	return (TRUE);
}

#ifdef SHUFFLE
COMP_OPT void ML_AddToSortedList(uint24* list_ptr, uint24 list_size, uint24 entry) {
	uint24 inserted = 0;
	uint24 tmp, tmpb, i;
	tmp = entry;
	for (i=0; i<list_size; i++)
	{
		if (!inserted)
		{
			if (tmp <list_ptr[i])
			{
				tmpb = list_ptr[i];
				list_ptr[i] = tmp;
				tmp = tmpb;
				inserted = 1;
			}
		}
		else
		{
			tmpb = list_ptr[i];
			list_ptr[i] = tmp;
			tmp = tmpb;
		}
	}
	list_ptr[i] = tmp;
}

COMP_OPT void ML_ShuffleInit(uint24 TOC) {
	int24 i;
	uint24 j, tmp, k;
	uint24 row, col, ind;
	uint24 rows[NumOfLevels];

	rows[0] = 2;
	rows[1] = 3;
	rows[2] = 5;
	rows[3] = 7;

	i = ReadRTC();
	srand( i );

	for(i=0; i<NumOfLevels; i++)
	{
		if (i==0)
			tmp_TOC[i] = TOC;
		else
			tmp_TOC[i] = tmp_TOC[i-1];

//		num_row[i] = i + 2;
		num_row[i] = rows[i];
		num_col[i] = ((uint24)((tmp_TOC[i] + num_row[i] - 1)/num_row[i]));
		tmp_TOC[i] = num_row[i]*num_col[i];
//		tmp = (uint24)rand();
//		offset[i] = tmp - (uint24)(tmp/tmp_TOC[i])*tmp_TOC[i];
		g_tmp_offset[i] = ((uint24)(rand() % tmp_TOC[i])) ;
	}

	Excess_list_size = 0;
	for(i=(int24)(NumOfLevels-1); i>=0; i--)
	{
		if (i)
			j=tmp_TOC[i-1];
		else
			j=TOC;

		while(j<tmp_TOC[i])
		{
			tmp = j;
			for (k=i; k<NumOfLevels;k++)
			{
				col = tmp%num_col[k];
//				col = tmp - (uint24)(tmp/num_col[k])*num_col[k];
				row = tmp/num_col[k];
				ind = col*num_row[k] + row;
				tmp = ((ind+tmp_TOC[k]-g_tmp_offset[k]) % tmp_TOC[k]);
//				tmp2 = (ind+tmp_TOC[k]-offset[k]);
//				tmp = tmp2 - (uint24)(tmp2/tmp_TOC[k]) * tmp_TOC[k];

			}
			ML_AddToSortedList(&(Excess[0]),Excess_list_size++, tmp);
			j++;
		}
	}
}

COMP_OPT uint24 ML_InverseShuffle(uint24 song_num) {
	int24 i;
	uint24 shuffle_order, k, shuffle_index;
	uint24 row, col, ind;
//	uint24 tmp;

	shuffle_index = song_num;
	for (k=0; k<NumOfLevels;k++)
	{
		col = shuffle_index%num_col[k];
//		col = shuffle_index - (uint24)(shuffle_index/num_col[k])*num_col[k];
		row = shuffle_index/num_col[k];
		ind = col*num_row[k] + row;
//		tmp = (ind+tmp_TOC[k]-offset[k]);
//		shuffle_index = tmp - (uint24)(tmp/tmp_TOC[k])*tmp_TOC[k];
		shuffle_index = ((ind+tmp_TOC[k]-g_tmp_offset[k]) % tmp_TOC[k]);
	}

	shuffle_order = shuffle_index;
	for (i=(int24)((int24)Excess_list_size-1); i>=0; i--)
	{
		if (shuffle_order >= Excess[i])
			shuffle_order--;
	}
	return shuffle_order;
}

COMP_OPT void	ML_startShuffle( uint24 *index ) {
	uint24 memory_type;
	ML_ShuffleInit( play_total_merge_num_item );
	memory_type = (playMode & 8);
//	*index = ( !memory_type || (memory_type && lastPlaySongLevel >= STATE_1DAY && lastPlaySongLevel <= STATE_AUDIBLE) ) ? play_flash_window_pos : play_flash_name_list_size + play_sd_window_pos;
	*index = ( !memory_type || (memory_type && lastPlaySongLevel >= STATE_1DAY && lastPlaySongLevel <= STATE_ONTHEGO) ) ? play_flash_window_pos : play_flash_name_list_size + play_sd_window_pos;
	shuffleOrder = ML_InverseShuffle( *index );
}

#ifdef SHUFFLE_NEXT
COMP_OPT uint24	ML_getNextShuffleSong( uint24 direction, uint24 level, RAM_SONG_INFO_T *songStruct, uint24 *songIndex ) {
	uint24 itemType;
	uint24 id;
	uint24 artistID;
	uint24 level1;
	uint24 underArtist;
	uint24 *posPt;
	uint24 lastIndex;
	uint24 mid;
	uint24 lower, upper;

	level1 = isAtLevel1( level );
	underArtist = isUnderArtist( level );

	if ( getLastPlayMode( playMode ) == PLAY_NORMAL ) {
		if ( level1 && level != STATE_TRACK_LVL_1 && level <= STATE_YEAR_LVL_1 ) {

			if ( !(playMode&8) ) {
				itemType = 0;
				posPt = (uint24 *)&play_flash_window_pos;
			}
			else {
				itemType = 1;
				posPt = (uint24 *)&play_sd_window_pos;
			}
			ML_GetItemNameByPosition( 1, currentPlayAllLevel, itemType, *posPt, &name[0], &id );
			ML_GetLvlOneItemSortListSize( MEDIA_TYPE_FLASH, ITEM_TRACK, &play_flash_name_list_size );
			ML_GetLvlOneItemSortListSize( MEDIA_TYPE_SD, ITEM_TRACK, &play_sd_name_list_size );
			play_flash_window_pos = play_sd_window_pos = 0;
			ML_SearchSongID( itemType, STATE_TRACK_LVL_1, id, posPt );
		}
		else if ( ((ML_IsAtSongLevel(level)) | (underArtist-1) ) == 0 ) {
			play_flash_window_pos = ( songIndexInMedia[0] < 0 ) ? 0 : songIndexInMedia[0];
//			play_flash_window_pos = songIndexInMedia[0];
			play_sd_window_pos = ( songIndexInMedia[1] < 0 ) ? 0 : songIndexInMedia[1];
//			play_sd_window_pos = songIndexInMedia[1];
			play_flash_name_list_size = org_play_flash_name_list_size;
			play_sd_name_list_size = org_play_flash_name_list_size;
		}
		if ( isPlayAll )
			currentPlayAllLevel = level;

		ML_startShuffle( songIndex );
	}

	if ( direction == SHUFFLE_NEXT_SONG ) {
		if ( shuffleOrder < play_total_merge_num_item-1 )
			shuffleOrder++;
		else
			shuffleOrder = 0;
	}
	else {
		if ( shuffleOrder > 0 )
			shuffleOrder--;
		else
			shuffleOrder = play_total_merge_num_item - 1;
	}

	*songIndex = ML_Shuffle( shuffleOrder );
	play_window_merge_index = *songIndex;
	if ( play_window_merge_index >= play_flash_name_list_size )	{
		play_window_merge_index -= play_flash_name_list_size;
		play_sd_window_pos = play_window_merge_index;
		itemType = 1;
	}
	else {
		play_flash_window_pos = play_window_merge_index;
		itemType = 0;
	}

	if ( (level1 | (ML_IsAtSongLevel(level)) | (underArtist-1) ) == 0 ) {
		artistID = songPath[itemType] & 0xFFF;
//		GetArtistSongID( itemType, artistID, play_window_merge_index, &id );
		LOCAL_GetArtistSong_info( itemType, artistID, play_window_merge_index, &id );
	}
	else {
		if ( (level1 && level < STATE_YEAR_LVL_1) || level == STATE_MAIN_MENU )
			level = STATE_TRACK_LVL_1;
		ML_GetListAndID( 1, level, itemType, play_window_merge_index, &artistID, &id );
	}

//	ML_ConstructSongStruct( id, itemType, (level/STATE_AUDIBLE), songStruct );
	ML_ConstructSongStruct( id, itemType, (level == STATE_AUDIBLE), songStruct, 0 );
	play_window_merge_index = *songIndex;
	playMode |= 7;

	return 1;
}

COMP_OPT uint24 ML_Shuffle(uint24 shuffle_order) {
	uint24 row, col;
	uint24 song_num, shuffle_index;
	int24 i;


	shuffle_index = shuffle_order;
	for (i = 0; i<(int24)Excess_list_size; i++)
	{
		if (shuffle_index >= Excess[i])
			shuffle_index++;
	}

	song_num = shuffle_index;
	for(i=(int24)(NumOfLevels-1); i>=0; i--)
	{
		song_num = ((song_num + g_tmp_offset[i]) % tmp_TOC[i]);
		row = song_num % num_row[i];
		col = song_num/num_row[i];
		song_num = row*num_col[i] + col;
	}
	return song_num;
}

#endif	// SHUFFLE_NEXT

#endif	// SHUFFLE

#ifdef SHUFFLE_UTILITY
COMP_OPT uint24 ML_SearchSongID( uint24 memory_type, uint24 state, uint24 songID, uint24 *index ) {
	uint24 lower, upper, mid;
	uint24 id, isSearch;
	int24 temp;

	isSearch = 0;
	// binary search the lower index and the upper index of song name
	ML_GetSongInfoByID( memory_type, state, &lower, &upper );

	// Search the name with the same song ID
	// The first existing name in the list is searched by binary search in above function.
	// There maybe some song with the same name, therefore, the linear search below is used
	// to find the exactly index of the song
	while( lower <= upper ) {
		mid = ((upper + lower)>>1);
		ML_GetItemNameByPosition( 1, state, memory_type, mid, &name[1], &id );
		if ( id == songID ) {
			*index = mid;
			isSearch = 1;
			break;
		}
		else if ( id < songID )
			lower = mid + 1;
		else if ( id > songID )
			upper = mid - 1;
	}

	if ( lower == upper ) {
		*index = lower;
		isSearch = 1;
	}

	return isSearch;
}

COMP_OPT uint24 ML_GetSongInfoByID( uint24 memory_type, uint24 state, uint24 *index, uint24 *lastIndex ) {
	uint24 mid;
	uint24 lower;
	uint24 upper;
	uint24 isSearch;
	int24 temp;

	isSearch = 0;

	ML_GetListAndID( 1, state, memory_type, 0, &upper, (uint24*)&temp );
	if ( !upper ) {
		*index = 0;
		return 0;
	}

	lower = 0;
	upper -= 1;

	while( lower <= upper && !isSearch ) {
		mid = ((upper + lower)>>1);
		ML_GetItemNameByPosition( 1, state, memory_type, mid, &name[1], (uint24*)&temp );
		temp = ML_strcmp( &name[0], &name[1] );

		switch ( temp ) {
			case -1:
				upper = mid - 1;
				break;

			case 1:
				lower = mid + 1;
				break;
			default:
				// search the first tested name existing in the list
				*lastIndex = mid;
				while( *lastIndex < upper ) {
					ML_GetItemNameByPosition( 1, state, memory_type, *lastIndex+1, &name[1], (uint24*)&temp );
					temp = ML_strcmp( &name[0], &name[1] );
					if ( !temp )
						*lastIndex = *lastIndex + 1;
					else
						break;
				}
				while( mid > 0 ) {
					ML_GetItemNameByPosition( 1, state, memory_type, mid-1, &name[1], (uint24*)&temp );
					temp = ML_strcmp( &name[0], &name[1] );
					if ( !temp )
						mid--;
					else
						break;
				}
				*index = mid;
				isSearch = 1;
				break;
		}
	}

	if ( !isSearch ) {
		if ( lower == upper )
			*index = lower + 1;
		else
			*index = lower;
	}

	return isSearch;
}
#endif

#ifdef SHUFFLE_UTILITY2
COMP_OPT void	ML_ShuffleToNormal( uint24 *state, uint24 songID ) {
	uint24 memoryType;
	uint24 memoryType2;
	uint24 *posPt;
	uint24 upper;

	// Build the Song Path
	if ( !ML_IsAtSongLevel( *state ) )
		ML_BuildSongPathFromSongID( songID );

	memoryType = playMode / 8;
	// search the index of song from flash or SD
	ML_GetNameByState( songID, *state, memoryType, &name[0] );
	posPt = ( !memoryType ) ? &play_flash_window_pos : &play_sd_window_pos;
	ML_SearchSongID( memoryType, *state, songID, posPt );

	// search the index just greater than the song name from another memory
	memoryType2 = 1 & (~(memoryType&1));
	posPt = ( !memoryType2 ) ? &play_flash_window_pos : &play_sd_window_pos;
	ML_GetSongInfoByID( memoryType2, *state, posPt, &upper );
}

COMP_OPT void	ML_BuildSongPathFromSongID( uint24 songID ) {
	uint24 songPathIndex;
	uint24 itemType;
	uint24 memoryType;
	uint24 memoryType2;
	uint24 itemID;
	uint24 itemID2;
	uint24 index;
	uint24 index2;
	uint24 temp;

	if ( ML_IsAtSongLevel( currentPlayAllLevel ) )
		return;

	songPathIndex = (currentPlayAllLevel&3);
	itemType = (currentPlayAllLevel>>2) + songPathIndex;
	songPathIndex *= 2;

	memoryType = playMode / 8;
	memoryType2 = ( 1 & (~(memoryType&1)) );

	// get item name using current state
	temp = itemType + 5;
	LOCAL_GetLvlOneItem_info(memoryType, temp, songID, &itemID);
	ML_GetItemName(itemID, memoryType, temp, &name[0]);

	// search the index of the item name
	ML_GetSongInfoByID( memoryType, currentPlayAllLevel, &index, &temp );

	// search whether the same name exists in another memory
	if ( ML_GetSongInfoByID( memoryType2, currentPlayAllLevel, &index2, &temp ) == FALSE )
		itemID2 = 0xFFF;
	else
		ML_GetListAndID( 1, currentPlayAllLevel, memoryType2, index2, &temp, &itemID2 );

	// update songPath
	songPath[ songPathIndex+memoryType ] = (index | itemID);
	songPath[ songPathIndex+memoryType2 ] = (index2 | itemID2);

	currentPlayAllLevel++;

	// call itself to down one level
	ML_BuildSongPathFromSongID( songID );
}
#endif	// SHUFFLE_UTILITY2


COMP_OPT void	ML_updateSize( uint24 currentLvl ) {
	uint24 level;
	uint24 id;

	level = currentPlayAllLevel+1-currentLvl;

	if ( hasInsertSD )
		ML_GetListAndID( 1, level, MEDIA_TYPE_SD, 0, &play_sd_name_list_size, &id );
	else
		play_sd_name_list_size = 0;
	ML_GetListAndID( 1, level, MEDIA_TYPE_FLASH, 0, &play_flash_name_list_size, &id );
}

#ifdef PLAY_ALL

COMP_OPT void	ML_buildSongPath( uint24 *state, uint24 startSongPathIndex ) {
	uint24 id1, id2;
	uint24 temp;

	if ( ML_IsAtSongLevel( *state ) )
		return;

	ML_GetNames( 1, *state, 0, 0, &songPath[startSongPathIndex], &songPath[startSongPathIndex+1] );

	if ( hasInsertSD ) {
		temp = ML_updateListPointer( *state, &name[0], &name[1], &id1, &id2 );
		if ( temp == 1 )
			songPath[startSongPathIndex+1] = 0xFFF;
		else if ( temp == 2 )
			songPath[startSongPathIndex] = 0xFFF;
	}
	*state = *state + 1;

	ML_buildSongPath( state, (startSongPathIndex+2) );
}
#endif // PLAY_ALL

#ifdef PLAY_NEXT
COMP_OPT uint24 ML_PlayNextSong( RAM_SONG_INFO_T *songStruct, uint24 *songIndex ) {
	uint24 id;
	uint24 org_flash_window_pos, org_sd_window_pos;
	uint24 playLevel;
	uint24 isRepeat = 0;
	uint24 isLastShuffle = 0;
    uint24 tracknum1, tracknum2;
    BOOL skip = FALSE;

	playLevel = ( isPlayAll ) ? currentPlayAllLevel : lastPlaySongLevel;
	org_flash_window_pos = play_flash_window_pos;
	org_sd_window_pos = play_sd_window_pos;

	if ( getLastPlayMode( playMode ) == PLAY_SHUFFLE ) {
		isLastShuffle = 1;
		play_flash_window_pos = play_sd_window_pos = 0;
		*songIndex = play_window_merge_index = 0;
		playMode = (playMode&0xFFFFF8) | 4;
	}
	else {
        if (g_SortByTracknum && ((playLevel==STATE_ARTIST_LVL_3) || (playLevel==STATE_ALBUM_LVL_2))) {
            ML_GetTracknums (1, playLevel, play_flash_window_pos, play_sd_window_pos, &tracknum1, &tracknum2, &id, &id);

            if (tracknum1 < tracknum2) {
		        play_flash_window_pos++;
                skip = TRUE;
            }
            else if (tracknum1 > tracknum2) {
                play_sd_window_pos++;
                skip = TRUE;
            }
        }

        if (skip == FALSE)
        {
		// get the name of current pointer position
		ML_GetNames( 1, playLevel, play_flash_window_pos, play_sd_window_pos, &id, &id );

		ML_updateListPointer( playLevel, &name[0], &name[1], (uint24 *)&play_flash_window_pos, (uint24 *)&play_sd_window_pos );
	    }
		if ( (play_flash_window_pos) >= play_flash_name_list_size && (play_sd_window_pos) >= play_sd_name_list_size ) {
			if ( isPlayAll /*|| !(playMode & 4)*/ ) {

				play_flash_window_pos = org_flash_window_pos;
				play_sd_window_pos = org_sd_window_pos;
				songStruct->path_name[0] = 0;
				*songIndex = play_window_merge_index;

				return 0;
			}
			else {
				play_flash_window_pos = play_sd_window_pos = 0;
				*songIndex = play_window_merge_index = 0;
				isRepeat = 1;
			}
		}
		playMode = (playMode&0xFFFFF8) | 4;
	}
	ML_GetSongStructByPosition( playLevel, play_flash_window_pos, play_sd_window_pos, songStruct );

	if ( !isRepeat && !isLastShuffle )
		*songIndex = ++play_window_merge_index;

	return 1;
}
#endif // PLAY_NEXT

#ifdef PLAY_NEXT_AND_PREV
COMP_OPT void ML_updateSongPath( FLASH_GROUP_NAME_T *name, uint24 startIndex, uint24 *id1, uint24 *pos1, uint24 *id2, uint24 *pos2 ) {
	int24 temp2;
	uint24 t_id1, t_id2;

	if ( !name[1].name[0] ) {
		t_id2 = 0xFFF;
		t_id1 = *id1;
	}
	else if ( !name[0].name[0] ) {
		t_id1 = 0xFFF;
		t_id2 = *id2;
	}
	else {
		temp2 = ML_strcmp( &name[0], &name[1] );
		switch( temp2 ) {
			case 0:
				t_id1 = *id1;
				t_id2 = *id2;
				break;
			case -1:
				t_id1 = *id1;
				t_id2 = 0xFFF;
				break;
			default:
				t_id1 = 0xFFF;
				t_id2 = *id2;
				break;
		}
	}
	songPath[startIndex] = (*pos1*4096) | t_id1;
	songPath[startIndex+1] = (*pos2*4096) | t_id2;
	/*
	ML_PutData( songPath, (startIndex+1), *pos1 );
	ML_PutData( songPath, (startIndex+3), *pos2 );
	ML_PutData( songPath, (startIndex+2), t_id1 );
	ML_PutData( songPath, (startIndex+4), t_id2 );
	*/
}

COMP_OPT void ML_getWindowPosAndSize( uint24 currentLvl, uint24 treeLvl, uint24 *pos1, uint24 *pos2 ) {
	uint24 temp;

	if ( currentLvl == 1 ) {
		*pos1 = play_flash_window_pos;
		*pos2 = play_sd_window_pos;
	}
	else {
//		temp = ( isUnderArtist( currentPlayAllLevel ) && currentLvl == 2 ) ? 4 : ((treeLvl-currentLvl)<<2);
//		temp = ( (currentLvl == 2) && (isUnderArtist( currentPlayAllLevel )) ) ? 4 : 0;
		temp = ( ( (currentLvl-2) | (isUnderArtist( currentPlayAllLevel )-1) ) == 0 ) ? 4 : 0;

		*pos1 = ML_GetData( songPath, (temp+1) );
		*pos2 = ML_GetData( songPath, (temp+3) );
	}

	ML_updateSize( currentLvl );
}
#endif	// PLAY_NEXT_AND_PREV

// Darwin
#ifdef PLAY_ALL_START
/* new function added 05/01/10 */
COMP_OPT uint24 LOCAL_GetArtistSong_info( uint24 memory_type, uint24 artistID, uint24 sortedListIndex, uint24 *info ) {
	uint24 i;
	uint24 size;
	uint24 album_size;
	uint24 album_id;
	uint24 song_size;

//	if (ML_GetFlashArtistAlbumSortListSize(memory_type, artistID, &album_size) == FALSE) {
	if (LOCAL_GetFlashAssociatedList_info(memory_type, artistID, VOID_INDEX, ITEM_ARTIST, VOID_INDEX, &album_size) == FALSE) {
		return (FALSE);
	}
	size = 0x00;
	for (i=0;i<album_size;i++) {
//		if (ML_GetFlashArtistAlbumID(memory_type, artistID, i, &album_id) == FALSE) {
		if (LOCAL_GetFlashAssociatedList_info(memory_type, artistID, VOID_INDEX, ITEM_ARTIST, i, &album_id) == FALSE) {
			return (FALSE);
		}
//		if (ML_GetFlashArtistAlbumSongSortListSize(memory_type, artistID, album_id, &song_size) == FALSE) {
		if (LOCAL_GetFlashAssociatedList_info(memory_type, artistID, album_id, ITEM_ARTIST, VOID_INDEX, &song_size) == FALSE) {
			return (FALSE);
		}
		if ((sortedListIndex != VOID_INDEX) && (size + song_size) > sortedListIndex) {
			size = sortedListIndex - size;
			break;
		} else {
			size += song_size;
		}
	}
	if (sortedListIndex == VOID_INDEX) {
		*info = size;
		return (TRUE);
	}
//	if (ML_GetFlashArtistAlbumSongID(memory_type, artistID, album_id, size, id ) == FALSE) {
	if (LOCAL_GetFlashAssociatedList_info(memory_type, artistID, album_id, ITEM_ARTIST, size, info) == FALSE) {
		return (FALSE);
	}
	return (TRUE);
}

// Get total number of song under particular Artist
COMP_OPT uint24 GetArtistSongSize( uint24 memory_type, uint24 artistID, uint24 *size ) {
	return (LOCAL_GetArtistSong_info( memory_type, artistID, VOID_INDEX, size));
}

#endif // PLAY_ALL_START

#endif // PLAY_MODULE
