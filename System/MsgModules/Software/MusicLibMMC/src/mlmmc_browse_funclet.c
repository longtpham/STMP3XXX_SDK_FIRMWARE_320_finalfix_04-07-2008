/*==================================================================================================

MODULE DESCRIPTION

====================================================================================================
 Confidential Proprietary
(c) Copyright 2004, All Rights Reserved

MODULE NAME: musiclib_browse_function_extra.c   AUTHOR: Developer Name   CREATION DATE:DD/MM/YYYY

PRODUCT NAMES: All

GENERAL DESCRIPTION:

General description of this grouping of functions.

Document Reference
----------------------------------------
Music Library Software Design Specification
Version : 01.01
Date :


Problem Reporting Instructions
----------------------------------------
Problems or corrections to this document must be reported by mailing to
 with subject format:
[MUSICLIB]<space><title>


Subsystem/Module
----------------------------------------
Firmware player part


Platforms Requiring This Change/Design
----------------------------------------
SigmaTel MP3 flash-based player


Initial Model(s) Requiring This Change/Design:
----------------------------------------
1)	Date :


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
<Project_Directory>\system\msgmodules\software\MusicLib_Player\src\musiclib_browse_function_extra.c


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

#define BROWSE_PLAY_MODULE

#include "..\ghdr\musiclib_ghdr.h"
#include "..\hdr\musiclib_flash_function.h"

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
_reentrant uint24 LOCAL_GetLvlOneItem_info( uint24 memory_type, uint24 item_type, uint24 sortedListIndex, uint24 *info );
_reentrant uint24 LOCAL_GetFlashAssociatedList_info( uint24 memory_type, uint24 lvl1_id, uint24 albumID, uint24 item_type, uint24 sortedListIndex, uint24 *info );

/*==================================================================================================
LOCAL VARIABLES
==================================================================================================*/
extern ON_THE_FLY_LIST_T _X	on_the_fly_list;

/*==================================================================================================
GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
LOCAL FUNCTIONS
==================================================================================================*/
static int dummy_function(void)
{
	return 0;
}

#ifdef FUNCLET
#pragma asm
    extern	SysCallFunclet
#pragma endasm
#endif

/* num_of_X, sorted ID */
#ifdef FUNCLET
    #pragma asm
        FLOCAL_GetLvlOneItem_info:

        nolist
        include "sysmacro.asm"
        list

        ;If using funclet, function will be executed via funclet kernel
        extern	RSRC_FUNCLET_LOCAL_GET_LVL_ONE_ITEM_INFO
        CallFunclet RSRC_FUNCLET_LOCAL_GET_LVL_ONE_ITEM_INFO

        org p,"SYSFUNCLET_LOCAL_GET_LVL_ONE_ITEM_INFO_P":
        dc      RSRC_FUNCLET_LOCAL_GET_LVL_ONE_ITEM_INFO
    #pragma endasm
_reentrant uint24 LOCAL_GetLvlOneItem_info_Funclet( uint24 memory_type, uint24 item_type, uint24 sortedListIndex, uint24 *info )
#else
_reentrant uint24 LOCAL_GetLvlOneItem_info( uint24 memory_type, uint24 item_type, uint24 sortedListIndex, uint24 *info )
#endif
{
	uint24 dtype;
	uint24 sdtype;
	uint24 itype;
	uint24 temp;
	uint24 i;

	//	*info = 0x0;			/* browse request */
	if (memory_type >= NUM_OF_MEDIA) return (FALSE);
	dtype = NUM_OF_SONG_DTYPE;
	itype = SONG_SORTLOOP;

	// Modified by William
	if ( item_type <= 4 && item_type != ITEM_TRACK ) {
		dtype = item_type + NUM_OF_ARTIST_DTYPE;
		itype = item_type;
		sdtype = item_type * 5 + ARTIST_SORT_DTYPE;
	}
	else if ( item_type == ITEM_TRACK || (item_type >=ITEM_SONG_INFO_ARTIST && item_type <=ITEM_SONG_INFO_YEAR) ) {
		sdtype = ( item_type == ITEM_TRACK ) ? SONG_ARRAY_DTYPE : (item_type - ITEM_SONG_INFO_ARTIST + SONG_INFO_ARTIST_DTYPE);
	}
	else {
		if (memory_type != MEDIA_TYPE_FLASH) {
			if (sortedListIndex == VOID_INDEX) {
				return (FALSE);
			} else {
				*info = 0x00;	/* list is merged */
				return (TRUE);
			}
		}
		if (item_type == ITEM_ON_THE_GO) {
			if (sortedListIndex == VOID_INDEX) {
				*info = on_the_fly_list.num_of_id;
			} else if (sortedListIndex < on_the_fly_list.num_of_id) {
				*info = on_the_fly_list.store_list[sortedListIndex];
			} else {
				return (FALSE);
			}
			return (TRUE);
		} else if (item_type == ITEM_VOICE) {
			dtype = NUM_OF_VOICE_DTYPE;
			sdtype = VOICE_ARRAY_DTYPE;
		} else if (item_type == ITEM_FMREC) {
			dtype = NUM_OF_FMREC_DTYPE;
			sdtype = FMREC_ARRAY_DTYPE;
		} else if (item_type == ITEM_PHOTO) {
			dtype = NUM_OF_PHOTO_DTYPE;
			sdtype = PHOTO_ARRAY_DTYPE;
		} else if (item_type == ITEM_MVIDEO) {
			dtype = NUM_OF_MVIDEO_DTYPE;
			sdtype = MVIDEO_ARRAY_DTYPE;
		} else {
			temp = item_type - ITEM_1DAY;
			dtype = temp + NUM_OF_NEW_1DAY_DTYPE;
			sdtype = temp + NEW_1DAY_DTYPE;
		}
	}

	if (ML_MemRead_Browse(memory_type, dtype, dtype, (uint24 *) &num_of_item_y[itype], ML_VALUE_STRUCT_SIZE) != SUCCESS) {
		/* flash access error */
		return (FALSE);
	}
	if (sortedListIndex == VOID_INDEX) {
		*info = num_of_item_y[itype];
		return (TRUE);
	}

	if (sortedListIndex < num_of_item_y[itype]) {
		if (ML_MemRead_Browse(memory_type, sdtype, sortedListIndex, info, ML_INDEX_STRUCT_SIZE) != SUCCESS) {
			/* flash access error */
			return (FALSE);
		}
	} else {
		return (FALSE);
	}

	return (TRUE);
}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextmlmmc_browse_funclet":
    #pragma endasm
#endif

/* Associated size, Associated ID */
#ifdef FUNCLET
    #pragma asm
        FLOCAL_GetFlashAssociatedList_info:

        nolist
        include "sysmacro.asm"
        list

        ;If using funclet, function will be executed via funclet kernel
        extern	RSRC_FUNCLET_LOCAL_GET_FLASH_ASSOCIATED_LIST_INFO
        CallFunclet RSRC_FUNCLET_LOCAL_GET_FLASH_ASSOCIATED_LIST_INFO

        org p,"SYSFUNCLET_LOCAL_GET_FLASH_ASSOCIATED_LIST_INFO_P":
        dc      RSRC_FUNCLET_LOCAL_GET_FLASH_ASSOCIATED_LIST_INFO
    #pragma endasm
_reentrant uint24 LOCAL_GetFlashAssociatedList_info_Funclet( uint24 memory_type, uint24 lvl1_id, uint24 albumID, uint24 item_type, uint24 sortedListIndex, uint24 *info )
#else
_reentrant uint24 LOCAL_GetFlashAssociatedList_info( uint24 memory_type, uint24 lvl1_id, uint24 albumID, uint24 item_type, uint24 sortedListIndex, uint24 *info )
#endif
{
	uint24 i;
	uint24 j;
	uint24 k;
	uint24 pos;
	uint24 dtype;
	uint24 idtype;

	if (memory_type >= NUM_OF_MEDIA) return (FALSE);
	dtype = ARTIST_ASSOC_COUNT_DTYPE + item_type * 5;
	idtype = dtype + 1;
	if (LOCAL_GetLvlOneItem_info(memory_type, item_type, VOID_INDEX, &i) == FALSE) {
		return (FALSE);
	}
	if (lvl1_id >= i) return (FALSE);

	pos = 0;
	if (lvl1_id > 0) {
		if (ML_MemRead_Browse(memory_type, dtype, (lvl1_id-1), &pos, ML_INDEX_STRUCT_SIZE) != SUCCESS) {
			/* flash access error */
			return (FALSE);
		}
	}
	if (ML_MemRead_Browse(memory_type, dtype, lvl1_id, &k, ML_INDEX_STRUCT_SIZE) != SUCCESS) {
		/* flash access error */
		return (FALSE);
	}
	k -= pos;
	if (albumID == VOID_INDEX) {
		if (sortedListIndex == VOID_INDEX) {
			*info = k;
			return (TRUE);
		}
		if (sortedListIndex >= k) return (FALSE);
		pos += sortedListIndex;
		if (ML_MemRead_Browse(memory_type, idtype, pos, info, ML_INDEX_STRUCT_SIZE) != SUCCESS) {
			/* flash access error */
			return (FALSE);
		}
		return (TRUE);
	} else {
		if (LOCAL_GetLvlOneItem_info(memory_type, ITEM_ALBUM, VOID_INDEX, &i) == FALSE) {
			return (FALSE);
		}
		if (albumID >= i) return (FALSE);
	}

	for (i=0;i<k;i++) {
		if (ML_MemRead_Browse(memory_type, idtype, pos, &j, ML_INDEX_STRUCT_SIZE) != SUCCESS) {
			/* flash access error */
			return (FALSE);
		}
		if (j == albumID) {
			break;
		}
		pos++;
	}
	if (i == k) return (FALSE);

	i = 0;
	if (pos > 0) {
		if (ML_MemRead_Browse(memory_type, ARTIST_SUBASSOC_COUNT_DTYPE, (pos-1), &i, ML_INDEX_STRUCT_SIZE) != SUCCESS) {
			/* flash access error */
			return (FALSE);
		}
	}
	if (ML_MemRead_Browse(memory_type, ARTIST_SUBASSOC_COUNT_DTYPE, pos, &k, ML_INDEX_STRUCT_SIZE) != SUCCESS) {
		/* flash access error */
		return (FALSE);
	}
	k -= i;
	if (sortedListIndex == VOID_INDEX) {
		*info = k;
		return (TRUE);
	}
	if (sortedListIndex >= k) return (FALSE);
	i += sortedListIndex;
	if (ML_MemRead_Browse(memory_type, ARTIST_SUBASSOC_CID_DTYPE, i, info, ML_INDEX_STRUCT_SIZE) != SUCCESS) {
		/* flash access error */
		return (FALSE);
	}
	return (TRUE);
}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextmlmmc_browse_funclet":
    #pragma endasm
#endif
