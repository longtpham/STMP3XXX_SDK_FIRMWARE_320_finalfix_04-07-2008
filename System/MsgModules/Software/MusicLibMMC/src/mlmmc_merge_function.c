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
	<Project_Directory>\system\msgmodules\software\MusicLib_Player\src\musiclib_merge_function.c


Portability: All

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
#pragma optimize 1
#else
#define COMP_OPT
#endif

#define MERGE_FUNCTION

#include	"..\hdr\musiclib_hdr.h"
#ifndef _PC_SIMULATION_
#include ".\musiclib_basic_function.c"


#define _USEFLASH_WRITE_	/* only the build functions need to use ML_MemWrite() */
#define USE_FUNC_GET_MAP_SEC

#ifdef _USE_FS_SECTAB_
#include "..\..\..\..\inc\fserror.h"
#include "..\..\..\..\filesystem\fat32\h\extern.h"
#endif
#else
#include	"..\hdr\musiclib_flash_function.h"
#endif	/* #ifndef _PC_SIMULATION_ */

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
#if 1
#define _SAVENAMESHORTCUT_		/* optimise: quit copy on '\0' when short name encounter*/
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
#define	ASSOC_COUNT_LIST		(0)
#define	ASSOC_CID_LIST			(1)

//#define	INDEX_EOF	0xFFF

#define UINT24_STR_CMPTYPE		(0)
#define UINT24_VAL_CMPTYPE		(1)
//#define UINT24_MARKTAB_CMPTYPE	(2)
//#define UINT24_PATH_CMPTYPE		(3)
#define ITEMCMP_ITEM01_EQUAL	(0x00)
#define ITEMCMP_ITEM0_SMALL		(0x00FFFF)
#define ITEMCMP_ITEM0_LARGE		(0x01)

#ifdef _USE_FS_SECTAB_
#define SECTOR_ADD_SUCCESS				0
#define SECTOR_ADD_ERROR_FSEEK			-1
#define SECTOR_ADD_ERROR_READ_MODE		-2
#define SECTOR_ADD_ERROR_FILE_SIZE		-3
#define SECTOR_ADD_ERROR_OUTPUT			-4
#define SECTOR_ADD_ERROR_INCOMPLETE		-5
#endif

#define MERGE_NXPOS_COL			(0)
#define MERGE_MEDIA_COL			(1)

/*==================================================================================================
                                     LOCAL STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
                                     LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
                                          LOCAL VARIABLES
==================================================================================================*/
/* RAM */
/* temporary buffer to store the names for exact position search in the link list */
/* as temp buf for on_the_fly_list path */
uint24 temp_merge_buf[NUM_OF_MEDIA][LEN_OF_NAME_IN_BYTE];	/* use as general temp string buffer */

uint24 num_of_merge[NUM_OF_MEDIA][MAX_NUM_OF_GROUP];
//uint24	temp_list_buf[DISTANCE_IN_MARK_TABLE][3];
//uint24	insert_name[LEN_OF_NAME_IN_BYTE];	/* as temp buf for on_the_fly_list path */

/* variables in _X */
FLASH_GROUP_NAME_T _X flash_merge_name[NUM_OF_MEDIA];
uint24 _X temp_merge[NUM_OF_MEDIA][2][MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD];
//uint24 _X temp_song_info_name[LEN_OF_FLASH_NAME_BUF];

#ifdef _PC_DEBUGPRINT_
uint24	w;		/* debug use */
uint24	y;		/* debug use */
#endif

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
#ifndef _PC_SIMULATION_
COMP_OPT uint24 ChkTermInWord(uint24 myword) {
	return (((myword & 0xFF0000) == 0x00) || ((myword & 0x00FF00) == 0x00) || ((myword & 0x0000FF) == 0x00));
}
#endif

COMP_OPT void ML_cpy_flash_merge_name(FLASH_GROUP_NAME_T _X *dbuf, FLASH_GROUP_NAME_T *sbuf)
{
	uint24 i;
	
	for (i=0;i<LEN_OF_NAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD;i++) {
		dbuf->name[i] = sbuf->name[i];
	}
	dbuf->unicode = sbuf->unicode;
}

#ifndef _PC_SIMULATION_
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
		/*
		case	UINT24_MARKTAB_CMPTYPE:
			cmpsize = (LEN_OF_SUB_STRING_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD);
			break;
		case	UINT24_PATH_CMPTYPE:
			cmpsize = (LEN_OF_FILEPATH_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD);
			break;
		*/
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
#endif
/*==================================================================================================
FUNCTION: 

DESCRIPTION:
   Description of this specific function.

  Called by UI, the merge the music library tables album,
artist, genre, song and year.

ARGUMENTS PASSED:
   
RETURN VALUE:
   
PRE-CONDITIONS:
   None

POST-CONDITIONS:
   None

IMPORTANT NOTES:
   None

==================================================================================================*/
void ML_MergeLibraryTables(void) {
uint24 i;
uint24 j;
uint24 k;
uint24 r;
//uint24 s;
//uint24 t;
uint24 u;
uint24 m;
uint24 n;

uint24 count[NUM_OF_MEDIA];
FLASH_GROUP_NAME_T flash_merge_name_y;
uint24 small_mitem;

//RAM_SONG_INFO_T song_info;
FLASH_GROUP_NAME_T nameitem;
//uint24 sorted_list[MAX_NUM_OF_SONG/NUM_OF_ITEM_IN_ONE_WORD];
//uint24 size;
uint24 pos;
uint24 opposite_pos;
uint24 gp_count;
uint24 dtype;
uint24 sdtype;
uint24 ndtype;
uint24 mgtype;

	for(gp_count=0;gp_count<(MAX_NUM_OF_GROUP-1);gp_count++) {
		if (gp_count != SONG_SORTLOOP) continue;
		switch (gp_count) {
			case	ARTIST_SORTLOOP:
				dtype = NUM_OF_ARTIST_DTYPE;
				sdtype = ARTIST_SORT_DTYPE;
				ndtype = ARTIST_NAME_DTYPE;
				mgtype = ARTIST_MERGE_DTYPE;
				break;
			case	ALBUM_SORTLOOP:
				dtype = NUM_OF_ALBUM_DTYPE;
				sdtype = ALBUM_SORT_DTYPE;
				ndtype = ALBUM_NAME_DTYPE;
				mgtype = ALBUM_MERGE_DTYPE;
				break;
			case	GENRE_SORTLOOP:
				dtype = NUM_OF_GENRE_DTYPE;
				sdtype = GENRE_SORT_DTYPE;
				ndtype = GENRE_NAME_DTYPE;
				mgtype = GENRE_MERGE_DTYPE;
				break;
			case	YEAR_SORTLOOP:
				dtype = NUM_OF_YEAR_DTYPE;
				sdtype = YEAR_SORT_DTYPE;
				ndtype = SONG_YEAR_DTYPE;
				mgtype = YEAR_MERGE_DTYPE;
				break;
			//case	SONG_SORTLOOP:
			default:
				dtype = NUM_OF_SONG_DTYPE;
				sdtype = SONG_ARRAY_DTYPE;
				ndtype = SONG_NAME_DTYPE;
				mgtype = SONG_MERGE_DTYPE;
				break;
		};	
		ML_MemRead(MEDIA_TYPE_FLASH, dtype, dtype, (uint24 *) &num_of_merge[MEDIA_TYPE_FLASH][gp_count], ML_VALUE_STRUCT_SIZE);
		//printf("\nnum_of_song=%d", num_of_merge[MEDIA_TYPE_FLASH][SONG_SORTLOOP]);
		//printf("\nSorted ram_song_name(compared string)=");
		ML_MemRead(MEDIA_TYPE_SD, dtype, dtype, (uint24 *) &num_of_merge[MEDIA_TYPE_SD][gp_count], ML_VALUE_STRUCT_SIZE);
		//printf("\nnum_of_song=%d", num_of_merge[MEDIA_TYPE_FLASH][SONG_SORTLOOP]);
		//printf("\nSorted ram_song_name(compared string)=");

		count[MEDIA_TYPE_FLASH] = 0;
		count[MEDIA_TYPE_SD] = 0;
		if ((num_of_merge[MEDIA_TYPE_FLASH][gp_count] > 0) && (num_of_merge[MEDIA_TYPE_SD][gp_count] > 0)) {
			while ((count[MEDIA_TYPE_FLASH]<num_of_merge[MEDIA_TYPE_FLASH][gp_count])
					&& (count[MEDIA_TYPE_SD]<num_of_merge[MEDIA_TYPE_SD][gp_count])) {
				ML_MemRead(MEDIA_TYPE_FLASH, sdtype, count[MEDIA_TYPE_FLASH], (uint24 *) &j, ML_INDEX_STRUCT_SIZE);
				//printf("\nID=%d\t", j);
				ML_MemRead(MEDIA_TYPE_FLASH, ndtype, j, (uint24 *) &flash_merge_name_y, ML_SONG_NAME_STRUCT_SIZE);
				ML_cpy_flash_merge_name((FLASH_GROUP_NAME_T _X *) &flash_merge_name[MEDIA_TYPE_FLASH], (FLASH_GROUP_NAME_T *) &flash_merge_name_y);
				ML_extract_string(flash_merge_name[MEDIA_TYPE_FLASH].unicode, 
						temp_merge_buf[MEDIA_TYPE_FLASH], 
						flash_merge_name[MEDIA_TYPE_FLASH].name, 
						LEN_OF_NAME_IN_BYTE);
				/*
				printf("[");
				for (k=0;k<LEN_OF_NAME_IN_BYTE;k++) {
					printf("%c", temp_merge_buf[k]);
			#ifdef _SAVENAMESHORTCUT_
					if (temp_merge_buf[k] == 0) break;
			#endif
				}
				printf("]");
				*/
				ML_MemRead(MEDIA_TYPE_SD, sdtype, count[MEDIA_TYPE_SD], (uint24 *) &j, ML_INDEX_STRUCT_SIZE);
				//printf("\nID=%d\t", j);
				ML_MemRead(MEDIA_TYPE_SD, ndtype, j, (uint24 *) &flash_merge_name_y, ML_SONG_NAME_STRUCT_SIZE);
				ML_cpy_flash_merge_name((FLASH_GROUP_NAME_T _X *) &flash_merge_name[MEDIA_TYPE_SD], (FLASH_GROUP_NAME_T *) &flash_merge_name_y);
				ML_extract_string(flash_merge_name[MEDIA_TYPE_SD].unicode, 
						temp_merge_buf[MEDIA_TYPE_SD], 
						flash_merge_name[MEDIA_TYPE_SD].name, 
						LEN_OF_NAME_IN_BYTE);
				/*
				printf("[");
				for (k=0;k<LEN_OF_NAME_IN_BYTE;k++) {
					printf("%c", temp_merge_buf[k]);
			#ifdef _SAVENAMESHORTCUT_
					if (temp_merge_buf[k] == 0) break;
			#endif
				}
				printf("]");
				*/
				if (ItemCmp(UINT24_STR_CMPTYPE, temp_merge_buf[MEDIA_TYPE_FLASH], temp_merge_buf[MEDIA_TYPE_SD]) == ITEMCMP_ITEM0_LARGE) {
					ML_list_put_val_X(temp_merge[MEDIA_TYPE_SD][MERGE_NXPOS_COL], count[MEDIA_TYPE_SD], count[MEDIA_TYPE_FLASH]);
					count[MEDIA_TYPE_SD]++;
				} else {
					ML_list_put_val_X(temp_merge[MEDIA_TYPE_FLASH][MERGE_NXPOS_COL], count[MEDIA_TYPE_FLASH], count[MEDIA_TYPE_SD]);
					count[MEDIA_TYPE_FLASH]++;
				}
			}
		}	
		for (k=count[MEDIA_TYPE_FLASH];k<num_of_merge[MEDIA_TYPE_FLASH][gp_count];k++) {
			ML_list_put_val_X(temp_merge[MEDIA_TYPE_FLASH][MERGE_NXPOS_COL], k, num_of_merge[MEDIA_TYPE_SD][gp_count]);
		}
		for (k=count[MEDIA_TYPE_SD];k<num_of_merge[MEDIA_TYPE_SD][gp_count];k++) {
			ML_list_put_val_X(temp_merge[MEDIA_TYPE_SD][MERGE_NXPOS_COL], k, num_of_merge[MEDIA_TYPE_FLASH][gp_count]);
		}
		
		ML_MemWrite(MEDIA_TYPE_FLASH, mgtype, num_of_merge[MEDIA_TYPE_FLASH][gp_count], (uint24 _X *)&temp_merge[MEDIA_TYPE_FLASH], ML_LIST_STRUCT_SIZE);
		ML_MemWrite(MEDIA_TYPE_SD, mgtype, num_of_merge[MEDIA_TYPE_SD][gp_count], (uint24 _X *)&temp_merge[MEDIA_TYPE_SD], ML_LIST_STRUCT_SIZE);
	}
#if 0
	for(gp_count=0;gp_count<(MAX_NUM_OF_GROUP-1);gp_count++) {
		if (gp_count == SONG_SORTLOOP) continue;
		/* album */
		/*
		ML_GetLvlOneItemSortListSize(0, ITEM_ALBUM, &j);
		printf("\nML_GetLvlOneItemSortListSize(0, ITEM_ALBUM, j)=%d", j);
		*/
		switch (gp_count) {
			/*
			case	ITEM_TRACK:
				sdtype = SONG_ARRAY_DTYPE;
				break;
			*/
			case	ARTIST_SORTLOOP:
				dtype = NUM_OF_ARTIST_DTYPE;
				sdtype = ARTIST_SORT_DTYPE;
				break;
			case	ALBUM_SORTLOOP:
				dtype = NUM_OF_ALBUM_DTYPE;
				sdtype = ALBUM_SORT_DTYPE;
				break;
			case	GENRE_SORTLOOP:
				dtype = NUM_OF_GENRE_DTYPE;
				sdtype = GENRE_SORT_DTYPE;
				break;
			case	YEAR_SORTLOOP:
				dtype = NUM_OF_YEAR_DTYPE;
				sdtype = YEAR_SORT_DTYPE;
				break;
			};	
		if (ML_MemRead(MEDIA_TYPE_FLASH, dtype, dtype, (uint24 *) &num_of_merge[MEDIA_TYPE_FLASH][gp_count], ML_VALUE_STRUCT_SIZE) != SUCCESS) {
			/* flash access error */
			return (FALSE);
		}

		if (ML_MemRead(MEDIA_TYPE_SD, dtype, dtype, (uint24 *) &num_of_merge[MEDIA_TYPE_SD][gp_count], ML_VALUE_STRUCT_SIZE) != SUCCESS) {
			/* flash access error */
			return (FALSE);
		}

	if (sortedListIndex < num_of_item_y[itype]) {
		#ifdef BROWSE_PLAY_MODULE
		if (ML_MemRead_Browse(memory_type, sdtype, sortedListIndex, info, ML_INDEX_STRUCT_SIZE) != SUCCESS) {
		#else
		if (ML_MemRead(memory_type, sdtype, sortedListIndex, info, ML_INDEX_STRUCT_SIZE) != SUCCESS) {
		#endif
			/* flash access error */
			return (FALSE);
		}
	} else {
		return (FALSE);
	}

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
#endif

#if 0
	ML_MemRead(Media_type, NUM_OF_YEAR_DTYPE, NUM_OF_YEAR_DTYPE, (uint24 *) &num_of_merge[MEDIA_TYPE_FLASH][4], ML_VALUE_STRUCT_SIZE);
	printf("\nnum_of_year=%d", num_of_merge[MEDIA_TYPE_FLASH][4]);
	printf("\nSorted ram_song_year(compared string)=");
	for (i=0;i<num_of_merge[MEDIA_TYPE_FLASH][4];i++) {
		ML_MemRead(Media_type, YEAR_SORT_DTYPE, i, (uint24 *) &j, ML_INDEX_STRUCT_SIZE);
		printf("\nID=%d\t", j);
		ML_MemRead(Media_type, SONG_YEAR_DTYPE, j, (uint24 *) &flash_merge_name, ML_SONG_NAME_STRUCT_SIZE);
		printf("[%d]", flash_merge_name.name[0]);
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
#endif
}

/*================================================================================================*/

#if 0
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
