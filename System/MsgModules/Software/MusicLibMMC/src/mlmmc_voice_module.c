/*==================================================================================================

                                         BROWSE MOUDLE

====================================================================================================
                               Perception Digital Confidential Proprietary
                               (c) Copyright PD 2004, All Rights Reserved

MODULE NAME: musiclib_browse_function.c   AUTHOR: William Chan   CREATION DATE:DD/MM/YYYY

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
	<Project_Directory>\system\msgmodules\software\musiclib


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
#define BROWSE_PLAY_MODULE
#define BROWSE_AND_PLAY
#define PREVIOUS_SONG
#define PLAY_PREV
#define BROWSE_MODULE

#define VOICE_MODULE
#include "..\hdr\musiclib_hdr.h"

#ifndef _PC_SIMULATION_
#define _RUNNING_IN_EMBEDDED_
#endif

#ifdef	_RUNNING_IN_EMBEDDED_
#define COMP_OPT _reentrant static
#define COMP_OPT_G
#else
#define COMP_OPT
#define _reentrant
#endif

#pragma optimize 1
//COMP_OPT void ML_itoa( uint24 value, FLASH_GROUP_NAME_T *itemStruct/*, uint24 base*/ );
#include "musiclib_browse_function.c"

/*==================================================================================================
                                      LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
COMP_OPT uint24 ML_VoiceMenu( uint24 item_type, uint24 action, uint24 highlight, uint24 hasSD, FLASH_GROUP_NAME_T *item_list, uint24 *num_item, uint24 *total_num_item, uint24 *isEndList );		// fail return 0; success return 1
COMP_OPT uint24 ML_VoiceDownList( uint24 step, FLASH_GROUP_NAME_T *item_list, uint24 *num_item, uint24 *isEndList ); // fail return 0; success return 1
COMP_OPT uint24 ML_VoiceUpList( uint24 step, FLASH_GROUP_NAME_T *item_list, uint24 *num_item, uint24 *isEndList );
COMP_OPT void ML_ConstructWindow( FLASH_GROUP_NAME_T *item_list, uint24 *num_item );
COMP_OPT uint24 ML_CheckWindowEnd( void );
//COMP_OPT void ML_strcpy( uint24 *dest, uint24 *src );
COMP_OPT uint24	ML_SelectItemNameByPosition( uint24 current_state, uint24 flash_pos, uint24 sd_pos, uint24 *out_flash_pos, uint24 *out_sd_pos, uint24 *id1, uint24 *id2, uint24 *nameIndex );
COMP_OPT void ML_findLastItemPointer( uint24 isSongBrowse, uint24 state, uint24 *pos1, uint24 *id1, uint24 *pos2, uint24 *id2 );
COMP_OPT void	ML_SearchLastWindow( FLASH_GROUP_NAME_T *item_list, uint24 *num_item );

//COMP_OPT void debug_now(int a, int b, int *pt1, int *pt2);
/*==================================================================================================
                                          LOCAL FUNCTIONS
==================================================================================================*/
/* Call this function when change the level of browsing */
COMP_OPT uint24 ML_VoiceMenu( uint24 item_type, uint24 action, uint24 highlight, uint24 hasSD, FLASH_GROUP_NAME_T *item_list, uint24 *num_item, uint24 *total_num_item, uint24 *isEndList ) {
	uint24 temp;
	uint24 i;
	uint24 id, id2;

	hasInsertSD = NO_SD;

	*num_item = 0;
	*total_num_item = 0;

	if ( action == ACTION_MENU || (action == ACTION_BACK && !(current_state&3)) ) {
		last_request_item = 0xFF;
		current_state = STATE_MAIN_MENU;

		return 0;
	}

	if ( action == ACTION_OK ) {
		if ( item_type != last_request_item || current_state == STATE_MAIN_MENU ) {		/* go to level one if browse a new item */
			browsePath[0] = 0xFFFFFF;
			current_state = (item_type<<2);
		}
		else {
			// store current window position
			temp = ((current_state & 3)<<1);
			if ( temp == 4 )
				return 0;
			parent_window_pos[temp] = flash_window_pos;
			parent_window_pos[temp+1] = sd_window_pos;

			ML_GetIDFromCurrentWindow( highlight, &id, &id2 );

			browsePath[temp] = id;
			browsePath[temp+1] = id2;

			current_state++;
		}
	}
	else if ( action == ACTION_BACK ) {
		temp = (((current_state-1) & 3)<<1);
		flash_window_pos = parent_window_pos[temp];
		sd_window_pos = parent_window_pos[temp+1];
		browsePath[temp] = browsePath[temp+1] = 0xFFFFFF;
		current_state--;
	}
	else
		return 0;

	sd_name_list_size = 0;

	/* Get list from flash */
	ML_GetListAndID( 0, current_state, MEDIA_TYPE_FLASH, 0, &flash_name_list_size, &i );

	if ( action != ACTION_BACK )
		flash_window_pos = sd_window_pos = 0;
	/* initialize window */
	for ( i=0;i<g_iBrowseWindowSize;i++ ) {
		merge_id_list_flash[i] = 0xFFFFFF;
		merge_id_list_sd[i] = 0xFFFFFF;
	}
	*total_num_item = 0;

	/* count the number of items in this level */
	if ( ML_IsAtSongLevel( current_state ) )
		*total_num_item = sd_name_list_size + flash_name_list_size;

	total_merge_num_item = *total_num_item;
	window_merge_index = 0;

	/* construct the window at the beginning */
	ML_ConstructWindow( item_list, num_item );

	// check any blank item in current window
	*isEndList = ML_CheckWindowEnd();

	last_request_item = item_type;
	return 1;
}

COMP_OPT void	ML_saveOriginalIDList( uint24 *org_flash_window_pos, uint24 *org_sd_window_pos, uint24 *org_list ) {
	uint24 i;

	*org_flash_window_pos = flash_window_pos;
	*org_sd_window_pos = sd_window_pos;

	for ( i=0;i<g_iBrowseWindowSize;i++ ) {
		org_list[i] = merge_id_list_flash[i];
		org_list[i+g_iBrowseWindowSize] = merge_id_list_sd[i];
	}
}

COMP_OPT void	ML_restoreOriginalIDList( uint24 *org_flash_window_pos, uint24 *org_sd_window_pos, uint24 *org_list ) {
	uint24 i;

	flash_window_pos = *org_flash_window_pos;
	sd_window_pos = *org_sd_window_pos;

	for ( i=0;i<g_iBrowseWindowSize;i++ ) {
		merge_id_list_flash[i] = org_list[i];
		merge_id_list_sd[i] = org_list[i+g_iBrowseWindowSize];
	}
}

/* move down the window */
COMP_OPT uint24 ML_VoiceDownList( uint24 step, FLASH_GROUP_NAME_T *item_list, uint24 *num_item, uint24 *isEndList ) {

	uint24 i;
	uint24 org_flash_window_pos, org_sd_window_pos;
	int24 tmp;
	uint24 org_merge_id_list[(MAX_BROWSE_WINDOW_SIZE<<1)];
#if 1
	uint24 id[2], pos[2];
	uint24 outputPos[2];
	uint24 outputId[2];
	uint24 temp1;
#endif
	org_flash_window_pos = flash_window_pos;
	org_sd_window_pos = sd_window_pos;
	*num_item = 0;

	ML_saveOriginalIDList( &org_flash_window_pos, &org_sd_window_pos, org_merge_id_list );

	for ( i=0;i<step;i++ ) {
#if 0
		/* get the first item in two list */
		ML_GetNames( 0, current_state, flash_window_pos, sd_window_pos, (uint24*)&tmp, (uint24*)&tmp );

		ML_updateListPointer( current_state, &name[0], &name[1], &flash_window_pos, &sd_window_pos );
#endif
		/* check the window whether moved out of range */
		if ( (((merge_id_list_flash[1] & 0xFFF)-0xFFF) | ((merge_id_list_sd[1] & 0xfff)-0xFFF)) == 0 ) {
			ML_restoreOriginalIDList( &org_flash_window_pos, &org_sd_window_pos, org_merge_id_list );

			*num_item = 0;
			return 0;
		}
#if 1
		// shift the 2nd to 4th item up to 1 position
		for ( tmp=0;tmp<g_iBrowseWindowSize-1;tmp++ ) {
			merge_id_list_flash[tmp] = merge_id_list_flash[tmp+1];
			merge_id_list_sd[tmp] = merge_id_list_sd[tmp+1];

			if ( item_list ) {
				ML_strcpy( item_list[tmp].name, item_list[tmp+1].name );
				item_list[tmp].unicode = item_list[tmp+1].unicode;
				if ( item_list[tmp].name[0] )
					*num_item = *num_item + 1;
			}
		}

		// Fill the last item
		pos[0] = merge_id_list_flash[g_iBrowseWindowSize-1] / 4096;
		pos[1] = merge_id_list_sd[g_iBrowseWindowSize-1] / 4096;

		// find the pointers of the next item from flash and SD list
		for ( tmp=0;tmp<2;tmp++ ) {
			if ( ML_SelectItemNameByPosition( current_state, pos[0], pos[1], &outputPos[0], &outputPos[1], &outputId[0], &outputId[1], &temp1 ) ) {
				if ( !tmp ) {
					pos[0] = outputPos[0];
					pos[1] = outputPos[1];
				}
				id[0] = outputId[0];
				id[1] = outputId[1];
			}
			else {
				id[0] = id[1] = 0xFFFFFF;
				if ( item_list )
					item_list[g_iBrowseWindowSize-1].name[0] = 0;
				break;
			}
		}

		if ( item_list && name[temp1].name[0] ) {
			ML_strcpy( item_list[g_iBrowseWindowSize-1].name, name[temp1].name );
			item_list[g_iBrowseWindowSize-1].unicode = name[temp1].unicode;
			*num_item = *num_item + 1;
		}

		merge_id_list_flash[g_iBrowseWindowSize-1] = (pos[0]*4096) | id[0];
		merge_id_list_sd[g_iBrowseWindowSize-1] = (pos[1]*4096) | id[1];

		flash_window_pos = merge_id_list_flash[0] / 4096;
		sd_window_pos = merge_id_list_sd[0] / 4096;
#else

		/* construct window in new position */
		ML_ConstructWindow( item_list, num_item );
#endif
		if ( ML_IsAtSongLevel( current_state ) )
			window_merge_index++;
	}


	// check any blank item in current window
	*isEndList = ML_CheckWindowEnd();

	return 1;
}

/* move up the window */
COMP_OPT uint24 ML_VoiceUpList( uint24 step, FLASH_GROUP_NAME_T *item_list, uint24 *num_item, uint24 *isEndList ) {

	uint24 i;
	uint24 tmp;
	uint24 id1, id2, index;
	uint24 org_flash_window_pos, org_sd_window_pos;
	uint24 org_merge_id_list[(MAX_BROWSE_WINDOW_SIZE<<1)];

	ML_saveOriginalIDList( &org_flash_window_pos, &org_sd_window_pos, org_merge_id_list );

	for ( i=0;i<step;i++ ) {
		*num_item = 0;
		/* check the window whether at the beginning */
		if ( (flash_window_pos | sd_window_pos) == 0 )
			ML_SearchLastWindow( item_list, num_item );
		else
		{
			// shift the 1st to 3rd item down to 1 position
			for ( tmp=g_iBrowseWindowSize-1;tmp>0;tmp-- ) {
				merge_id_list_flash[tmp] = merge_id_list_flash[tmp-1];
				merge_id_list_sd[tmp] = merge_id_list_sd[tmp-1];

				if ( item_list ) {
					ML_strcpy( item_list[tmp].name, item_list[tmp-1].name );
					item_list[tmp].unicode = item_list[tmp-1].unicode;
					if ( item_list[tmp].name[0] ) {
						*num_item = *num_item + 1;
					}
				}
			}

			ML_movePointerBackward( 0, current_state, (uint24 *)&flash_window_pos, flash_name_list_size, (uint24 *)&sd_window_pos, sd_name_list_size );

			if ( item_list ) {
				ML_SelectItemNameByPosition( current_state, flash_window_pos, sd_window_pos, &org_flash_window_pos, &org_sd_window_pos, &id1, &id2, &index );
				ML_strcpy( item_list[ 0 ].name, name[index].name );
				item_list[0].unicode = name[index].unicode;
				*num_item = *num_item + 1;
				merge_id_list_flash[0] = (flash_window_pos*4096) | id1;
				merge_id_list_sd[0] = (sd_window_pos*4096) | id2;
			}

			if ( ML_IsAtSongLevel( current_state ) )
				window_merge_index--;
		}
	}
	*isEndList = ( (flash_window_pos | sd_window_pos) == 0 ) ? 0 : 1;
	return 1;
}

COMP_OPT uint24	ML_SelectItemNameByPosition( uint24 current_state, uint24 flash_pos, uint24 sd_pos, uint24 *out_flash_pos, uint24 *out_sd_pos, uint24 *id1, uint24 *id2, uint24 *nameIndex ) {
	uint24 ret;
	uint24 temp;

	ML_GetNames( 0, current_state, flash_pos, sd_pos, id1, id2 );

	if ( (name[0].name[0]|name[1].name[0]) == 0 ) {
		*out_flash_pos = *out_sd_pos = *id1 = *id2 = 0xFFF;
		*nameIndex = 0xFFF;
		ret = 0;
	}
	else {
		temp = ML_updateListPointer( current_state, &name[0], &name[1], &flash_pos, &sd_pos );
		if ( temp == 3 )
			*nameIndex = 0;
		else {
			if ( temp == 1 ) {
				*id2 = 0xFFF;
				*nameIndex = 0;
			}
			else {
				*id1 = 0xFFF;
				*nameIndex = 1;
			}
		}

		*out_flash_pos = flash_pos;
		*out_sd_pos = sd_pos;

		ret = 1;
	}

	return ret;
}

/* Construct a window using the items in flash and items in SD */
COMP_OPT void ML_ConstructWindow( FLASH_GROUP_NAME_T *item_list, uint24 *num_item ) {

	uint24 i, offset1, offset2;
	uint24 id;
	uint24 id2;
	uint24 pos;
	uint24 pos2;
	uint24 flash_pos, sd_pos;
	uint24 nameIndex;

	*num_item = 0;
	offset1 = offset2 = 0;
	for ( i=0;i<g_iBrowseWindowSize;i++ ) {
		flash_pos = flash_window_pos + offset1;
		sd_pos = sd_window_pos + offset2;

		if ( !ML_SelectItemNameByPosition( current_state, flash_pos, sd_pos, &pos, &pos2, &id, &id2, &nameIndex ) )
			merge_id_list_flash[i] = merge_id_list_sd[i] = 0xFFFFFF;
		else {
			offset1 = pos - flash_window_pos;
			offset2 = pos2 - sd_window_pos;
			ML_strcpy( item_list[ *num_item ].name, name[nameIndex].name );
			item_list[ *num_item ].unicode = name[nameIndex].unicode;

			*num_item = *num_item + 1;
			merge_id_list_flash[i] = (flash_pos*4096) | id;
			merge_id_list_sd[i] = (sd_pos*4096) | id2;
		}
	}
}

COMP_OPT uint24 ML_CheckWindowEnd( void ) {
	uint24 org_flash_window_pos, org_sd_window_pos;
	uint24 tmp;
	uint24 i, new_pos, new_pos2, id1, id2, index;

	// check the current window whether fully filled
	if ( (((merge_id_list_flash[g_iBrowseWindowSize-1] & 0xFFF)-0xFFF) | ((merge_id_list_sd[g_iBrowseWindowSize-1] & 0xfff)-0xFFF)) == 0 )
		return 0;			// tell UI module that the window is at the end

	org_flash_window_pos = merge_id_list_flash[g_iBrowseWindowSize-1] / 4096;
	org_sd_window_pos = merge_id_list_sd[g_iBrowseWindowSize-1] / 4096;

	tmp = 1;
	for ( i=0;i<2;i++ ) {
		tmp = tmp & ML_SelectItemNameByPosition( current_state, org_flash_window_pos, org_sd_window_pos, &new_pos, &new_pos2, &id1, &id2, &index );
		org_flash_window_pos = new_pos;
		org_sd_window_pos = new_pos2;
	}

	return tmp;
}

COMP_OPT void ML_Fill_Browse_Content(int24* p_buffer, FLASH_GROUP_NAME_T *p_item, uint24 num_item)
{
	int24 i,j;
	if (p_buffer)
	{
		for (i=0; i<(int24)num_item; i++)
		{
			p_buffer[i*21] = p_item[i].unicode;
			for (j=0; j<20; j++)
				p_buffer[i*21+j+1] = p_item[i].name[j] & 0xffffff;
		}
		while (i<g_iBrowseWindowSize)
		{
			p_buffer[i*21] = 0;
			p_buffer[i*21+1] = 0;
			i++;
		}
	}
}

COMP_OPT void ML_Get_Browse_Content(int24* p_buffer, FLASH_GROUP_NAME_T *p_item)
{
	int24 i,j;
	if (p_buffer)
	{
		for (i=0; i<g_iBrowseWindowSize; i++)
		{
			p_item[i].unicode = p_buffer[i*21];
			for (j=0; j<20; j++)
				p_item[i].name[j] = p_buffer[i*21+j+1];
		}
	}
}

COMP_OPT void ML_GetTotalVoice( uint24 item_type, uint24 *flash_song, uint24 *sd_song, uint24 hasSD ) {
	uint24 id;

	*sd_song = 0;

	ML_GetListAndID( 0, item_type, MEDIA_TYPE_FLASH, 0, flash_song, &id );
}

COMP_OPT void	ML_SearchLastWindow( FLASH_GROUP_NAME_T *item_list, uint24 *num_item ) {
	uint24 id1, id2;
	uint24 i, temp;

	ML_findLastItemPointer( 0, current_state, (uint24 *)&flash_window_pos, &id1, (uint24 *)&sd_window_pos, &id2 );
	ML_ConstructWindow( item_list, num_item );
	for ( i=0;i<g_iBrowseWindowSize-1;i++ ) {
		ML_VoiceUpList( 1, item_list, num_item, &temp );
	}

	if (total_merge_num_item > g_iBrowseWindowSize)
		window_merge_index = total_merge_num_item - g_iBrowseWindowSize;
	else
		window_merge_index = 0;
}

#ifdef _PC_SIMULATION_
void ML_Init() {
	last_request_item = 0xFF;
	hasInsertSD = 0;
	current_state = STATE_MAIN_MENU;
}
#else
/*==================================================================================================
FUNCTION: MusicLibBrowseModuleInit()

DESCRIPTION:
MusicLib Browse Module Init.

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
int _reentrant MusicLibVoiceModuleInit(struct ModuleEntry *pModuleTableEntry)
{
	int i;
	union WaitMask returnMask;

#if 0
	last_request_item = 0xFF;
	current_state = STATE_MAIN_MENU;
#endif

	//Tell ModuleTableEntry.wait to call this module's process function next when it has a msg
	//returnMask.I=0x000000 | 2; // EVENT_MESSAGE
	return (0x000000 | 2);
	//return returnMask.I;
}

/*==================================================================================================
FUNCTION: MusicLibBrowseModuleProcessQueue()

DESCRIPTION:
MusicLib Browse Module ProcessQueue, UI interface for all musiclib browse function.

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
int _reentrant MusicLibVoiceModuleProcessQueue(union WaitMask Signals,struct MessageQueueDescriptor * pQueue)
{
	union WaitMask returnMask;
	struct CMessage  Msg;

	uint24 	total_item;
	uint24	num_item;
	uint24	temp;

	int24 Parm1, Parm2, Parm3, Parm4, Parm5;

	returnMask.I=0x000000 | 2;//Initialize it to signal us if we have a message

	//handle incoming messages
	if(Signals.B.m_bMessage) {
		while(GetMessage(pQueue,&Msg)) {
			Parm1 = Msg.m_uMsg[1];
			Parm2 = Msg.m_uMsg[2];
			Parm3 = Msg.m_uMsg[3];
			Parm4 = Msg.m_uMsg[4];
			Parm5 = Msg.m_uMsg[5];

			switch(Msg.m_uMsg[0])
			{
			case MUSICLIB_VOICE_UPMENU:
				ML_VoiceMenu( Parm2, ACTION_BACK, Parm3, g_is_SD_inserted, browse_item_name, &num_item, &total_item, (uint24*)&temp );
				*((uint24*)Parm1) = (uint24)num_item;
				break;
			case MUSICLIB_VOICE_BROWSEMENU:
				ML_VoiceMenu( Parm2, ACTION_OK, Parm3, g_is_SD_inserted, browse_item_name, &num_item, &total_item, (uint24*)Parm4/*bHaveNext*/ );
				*((uint24*)Parm1) = (uint24)num_item;
				break;
			case MUSICLIB_VOICE_UPLIST:
				ML_VoiceUpList( Parm1, browse_item_name, (uint24*)Parm3, (uint24*)Parm2 );
				break;
			case MUSICLIB_VOICE_DOWNLIST:
				ML_VoiceDownList( Parm1, browse_item_name, &num_item, (uint24*)Parm2/*bHaveNext*/);
				break;
			case MUSICLIB_VOICE_VOICENUMBER:
				ML_GetTotalVoice( Parm4, (uint24*)Parm1, (uint24*)Parm2, (uint24)Parm3 );
				break;
			default:
				break;
			}
		}
	}
	return returnMask.I;
}

/*================================================================================================*/
#endif /* _PC_SIMULATION_ */