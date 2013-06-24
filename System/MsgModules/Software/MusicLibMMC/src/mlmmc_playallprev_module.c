/*==================================================================================================

                                         MODULE DESCRIPTION

====================================================================================================
                               Perception Digital Confidential Proprietary
                               (c) Copyright PD 2004, All Rights Reserved

MODULE NAME: musiclib_play_function.c   AUTHOR: Developer Name   CREATION DATE:DD/MM/YYYY

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
	<Project_Directory>\system\msgmodules\software\MusicLib_Player\src\musiclib_song_function.c


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
#define PLAY_MODULE
#define PLAY_ALL
#define PLAY_PREV
#define PLAY_NEXT_AND_PREV
#define PREVIOUS_SONG
#define BROWSE_PLAY_MODULE
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
#include "musiclib_browse_function.c"
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
//COMP_OPT void ML_movePointerBackward( uint24 isSongBrowse, uint24 state, uint24 *id1, uint24 *flash_pos, uint24 flash_size, uint24 *id2, uint24 *sd_pos, uint24 sd_size );
COMP_OPT uint24 ML_PlayPreviousSong( RAM_SONG_INFO_T *songStruct, uint24 *songIndex );
COMP_OPT uint24 ML_PlayAllPreviousSong( RAM_SONG_INFO_T *songStruct, uint24 *songIndex );
COMP_OPT uint24	ML_MoveUpSong( uint24 currentLvl, uint24 treeLvl, FLASH_GROUP_NAME_T *name, RAM_SONG_INFO_T *songStruct, uint24 *songIndex );

/*==================================================================================================
                                          LOCAL VARIABLES
==================================================================================================*/
extern RAM_SONG_INFO_T song_info;

/*==================================================================================================
                                          GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
                                          LOCAL FUNCTIONS
==================================================================================================*/
COMP_OPT void	ML_getPrviousLastItem( uint24 currentLvl, uint24 treeLvl, uint24 pos1, uint24 pos2, uint24 *songIndex, RAM_SONG_INFO_T *songStruct ) {
	uint24 temp;
	uint24 id1, id2;

	temp = currentPlayAllLevel+1-currentLvl;
	ML_findLastItemPointer( 1, temp, &pos1, &id1, &pos2, &id2 );
	if ( currentLvl != 1 ) {
//		temp = ( isUnderArtist( currentPlayAllLevel ) && treeLvl == 2 ) ? 4 : (((treeLvl-currentLvl)<<2));
		temp = ( ( (currentLvl-2) | (isUnderArtist( currentPlayAllLevel )-1) ) == 0 ) ? 2 : 0;
		ML_updateSongPath( name, temp, &id1, &pos1, &id2, &pos2 );
	}
	else {
		play_flash_window_pos = pos1;
		play_sd_window_pos = pos2;
		if ( play_window_merge_index > 0 )
			*songIndex = --play_window_merge_index;
		else {
			*songIndex = play_total_merge_num_item - 1;
			play_window_merge_index = *songIndex;
		}

		ML_GetSongStructByPosition( currentPlayAllLevel, play_flash_window_pos, play_sd_window_pos, songStruct );
	}
}

/* find the previous in normal play mode */
COMP_OPT uint24	ML_MoveUpSong( uint24 currentLvl, uint24 treeLvl, FLASH_GROUP_NAME_T *name, RAM_SONG_INFO_T *songStruct, uint24 *songIndex ) {

	uint24 pos1, pos2;
	uint24 id1;
	uint24 id2;
	int24 temp;
	uint24 funcPtParam_1;

	ML_getWindowPosAndSize( currentLvl, treeLvl, &pos1, &pos2 );

	if ( currentLvl < treeLvl && (pos1|pos2) == 0 ) {
		if ( ML_MoveUpSong( (currentLvl+1), treeLvl, name, songStruct, songIndex ) ) {
			// find the last item in current level

			// get the list size under the new parent
			ML_updateSize( currentLvl );

			ML_getPrviousLastItem( currentLvl, treeLvl, pos1, pos2, songIndex, songStruct );
			return 1;
		}
	}
	else if ( ((currentLvl - treeLvl) | pos1 | pos2) == 0 ) {
		ML_updateSize( currentLvl );
		ML_getPrviousLastItem( currentLvl, treeLvl, pos1, pos2, songIndex, songStruct );
		return 1;
	}
	else if ( (pos1 | pos2) != 0 ) {
		if ( currentLvl == 1 ) {
			return ML_PlayPreviousSong( songStruct, songIndex );
		}
		else {
			// get previous item
			temp = 0;
			id1 = id2 = 0xFFF;
			name[0].name[0] = name[1].name[0] = 0;
			funcPtParam_1 = currentPlayAllLevel+1-currentLvl;

			ML_movePointerBackward( 1, funcPtParam_1, &pos1, play_flash_name_list_size, &pos2, play_sd_name_list_size );
			ML_GetNames( 1, funcPtParam_1, pos1, pos2, &id1, &id2 );
			temp = ( ( (currentLvl-2) | (isUnderArtist( currentPlayAllLevel )-1) ) == 0 ) ? 2 : 0;
//			temp = ( (currentLvl == 2) && (isUnderArtist( currentPlayAllLevel )) ) ? 2 : 0;
			ML_updateSongPath( name, temp, &id1, &pos1, &id2, &pos2 );

			return 1;
		}
	}
	return 1;
}

COMP_OPT uint24 ML_PlayPreviousSong( RAM_SONG_INFO_T *songStruct, uint24 *songIndex ) {
	int24 temp;
	uint24 id;
	uint24 playLevel;
	uint24 isLastShuffle;

	isLastShuffle = 0;
	playLevel = ( isPlayAll ) ? currentPlayAllLevel : lastPlaySongLevel;

	temp = 0;

	if ( getCurrentPlayMode( playMode ) == PLAY_SHUFFLE ) {
		isLastShuffle = 1;
		play_flash_window_pos = play_sd_window_pos = 0;
		*songIndex = play_window_merge_index = 0;
		playMode = (playMode&0xFFFFF8) | 4;
	}
	else {
		if ( (play_flash_window_pos | play_sd_window_pos) == 0 ) {
			if ( isPlayAll /*|| !(playMode & 4)*/ ) {
				*songIndex = play_window_merge_index;
				songStruct->path_name[0] = 0;
				return 0;
			}
			else {
				ML_findLastItemPointer( 1, playLevel, (uint24 *)&play_flash_window_pos, &id, (uint24 *)&play_sd_window_pos, (uint24*)&temp );
				ML_GetSongStructByPosition( playLevel, play_flash_window_pos, play_sd_window_pos, songStruct );
				*songIndex = play_total_merge_num_item - 1;
				play_window_merge_index = *songIndex;
				return 1;
			}
		}

		ML_movePointerBackward( 1, playLevel, (uint24 *)&play_flash_window_pos, play_flash_name_list_size, (uint24 *)&play_sd_window_pos, play_sd_name_list_size );

		playMode = (playMode&0xFFFFF8) | 4;
	}

	ML_GetSongStructByPosition( playLevel, play_flash_window_pos, play_sd_window_pos, songStruct );

	if ( !isLastShuffle )
		*songIndex = --play_window_merge_index;
	return 1;
}

COMP_OPT uint24 ML_PlayAllPreviousSong( RAM_SONG_INFO_T *songStruct, uint24 *songIndex ) {
	uint24 ret;
	uint24 temp;
	uint24 temp2;
	uint24 *size;

	if ( getLastPlayMode( playMode ) == PLAY_SHUFFLE ) {
		play_flash_window_pos = play_sd_window_pos = 0;
		*songIndex = play_window_merge_index = 0;
		if ( currentPlayAllLevel >= STATE_GENRE_LVL_1 && isAtLevel1(currentPlayAllLevel) )
			currentPlayAllLevel = STATE_TRACK_LVL_1;
		else
			currentPlayAllLevel = lastPlaySongLevel;

		temp = ( isAtLevel1( currentPlayAllLevel ) ) ? 0 : 2;
		ML_buildSongPath( &currentPlayAllLevel, temp );
		org_currentPlayAllLevel = currentPlayAllLevel;
		for ( temp=0;temp<5;temp++ ) {
			org_songPath[temp] = songPath[temp];
		}
//		songIndexInMedia[0] = songIndexInMedia[1] = 0;
		ret = ML_GetSongStructByPosition( currentPlayAllLevel, play_flash_window_pos, play_sd_window_pos, songStruct );
		playMode = (playMode&0xFFFFF8) | 4;
		temp = (playMode>>3);
		temp2 = ( temp == 0 ) ? 1 : 0;

//		songIndexInMedia[ temp2 ] = ( temp == 0 ) ? org_play_sd_name_list_size : org_play_flash_name_list_size;
		songIndexInMedia[ temp2 ] = -1;
		songIndexInMedia[temp] = 0;
	}
	else {
		ret = ML_MoveUpSong( 1, playAllLevel, name, songStruct, songIndex );
		temp = playMode>>3;

		size = ( !temp ) ? &org_play_flash_name_list_size : &org_play_sd_name_list_size;
		if ( songIndexInMedia[temp] > 0 )
			songIndexInMedia[temp]--;
		else
			songIndexInMedia[temp] = *size - 1;
	}

	return ret;
}

/*==================================================================================================
FUNCTION: MusicLibPlayAllPrevModuleInit()

DESCRIPTION:
   MusicLib Play Module Init.

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
int _reentrant MusicLibPlayAllPrevModuleInit(struct ModuleEntry *pModuleTableEntry)
{

	//Tell ModuleTableEntry.wait to call this module's process function next when it has a msg
	//returnMask.I=0x000000 | 2; // EVENT_MESSAGE
	return (0x000000 | 2);
	//return returnMask.I;
}

/*==================================================================================================
FUNCTION: MusicLibPlayAllPrevModuleProcessQueue()

DESCRIPTION:
   MusicLib PlayAllPrev Module ProcessQueue, UI interface for all musiclib playing function.

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
int _reentrant MusicLibPlayAllPrevModuleProcessQueue(union WaitMask Signals,struct MessageQueueDescriptor * pQueue)
{
union WaitMask returnMask;
struct CMessage  Msg;

int24 Parm1, Parm2;

	returnMask.I=0x000000 | 2;//Initialize it to signal us if we have a message

	//handle incoming messages
	if(Signals.B.m_bMessage) {
		while(GetMessage(pQueue,&Msg)) {
			Parm1 = Msg.m_uMsg[1];
			Parm2 = Msg.m_uMsg[2];

	            switch(Msg.m_uMsg[0])
	            {
				case MUSICLIB_PLAYPREVIOUSSONG:
					if (g_current_play_all)
						ML_PlayAllPreviousSong(&song_info, (uint24*)Parm1);
					else
						ML_PlayPreviousSong(&song_info, (uint24*)Parm1);
					break;
				default:
					break;
				}
		}
	}
	return returnMask.I;
}