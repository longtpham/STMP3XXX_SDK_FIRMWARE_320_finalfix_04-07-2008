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
#define PLAY_NEXT
#define PLAY_NEXT_AND_PREV
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
COMP_OPT uint24 ML_PlayAllNextSong( RAM_SONG_INFO_T *songStruct, uint24 *songIndex );
COMP_OPT uint24	ML_MoveDownSong( uint24 currentLvl, uint24 treeLvl, FLASH_GROUP_NAME_T *name, RAM_SONG_INFO_T *songStruct, uint24 *songIndex );

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
/* find the next song in normal play mode */
COMP_OPT uint24	ML_MoveDownSong( uint24 currentLvl, uint24 treeLvl, FLASH_GROUP_NAME_T *name, RAM_SONG_INFO_T *songStruct, uint24 *songIndex ) {
	uint24 pos1, pos2;
	uint24 id1;
	uint24 id2;
	uint24 temp;
	uint24 funcPtParam_2;
    uint24 tracknum1, tracknum2;
    BOOL skip = FALSE;

	ML_getWindowPosAndSize( currentLvl, treeLvl, &pos1, &pos2 );

	if ( currentLvl == 1 )
		temp = ML_PlayNextSong( songStruct, songIndex );
	else {
		funcPtParam_2 = currentPlayAllLevel+1-currentLvl;

        if (g_SortByTracknum && ((funcPtParam_2==STATE_ARTIST_LVL_3) || (funcPtParam_2==STATE_ALBUM_LVL_2))) {
            ML_GetTracknums (1, funcPtParam_2, pos1, pos2, &tracknum1, &tracknum2, &id1, &id2);

            if (tracknum1 < tracknum2) {
		        pos1++;
                skip = TRUE;
            }
            else if (tracknum1 > tracknum2) {
                pos2++;
                skip = TRUE;
            }
        }

        if (skip == FALSE)
        {
		ML_GetNames( 1, funcPtParam_2, pos1, pos2, &id1, &id2 );

		ML_updateListPointer( funcPtParam_2, &name[0], &name[1], &pos1, &pos2 );
	    }
		if ( pos1 >= play_flash_name_list_size && pos2 >= play_sd_name_list_size )
			temp = 0;
		else {
			ML_GetNames( 1, funcPtParam_2, pos1, pos2, &id1, &id2 );
//			temp = ( (currentLvl == 2) && (isUnderArtist( currentPlayAllLevel )) ) ? 2 : 0;
			temp = ( ( (currentLvl-2) | (isUnderArtist( currentPlayAllLevel )-1) ) == 0 ) ? 2 : 0;
			ML_updateSongPath( name, temp, &id1, &pos1, &id2, &pos2 );

			temp = 1;
		}
	}

	if ( currentLvl < treeLvl && !temp ) {
		if ( ML_MoveDownSong( (currentLvl+1), treeLvl, name, songStruct, songIndex ) ) {
			// update the first item under updated parent
			ML_updateSize( currentLvl );

			pos1 = pos2 = 0;
			if ( currentLvl != 1 ) {
				temp = currentPlayAllLevel+1-currentLvl;
				// get the last items' name from two lists
				ML_GetNames( 1, temp, pos1, pos2, &id1, &id2 );
//				temp = ( (currentLvl == 2) && (isUnderArtist( currentPlayAllLevel )) ) ? 2 : 0;
				temp = ( ( (currentLvl-2) | (isUnderArtist( currentPlayAllLevel )-1) ) == 0 ) ? 2 : 0;
				ML_updateSongPath( name, temp, &id1, &pos1, &id2, &pos2 );
			}
			else {
				play_flash_window_pos = pos1;
				play_sd_window_pos = pos2;
				if ( play_window_merge_index < play_total_merge_num_item-1 )
					*songIndex = ++play_window_merge_index;
				else
					*songIndex = play_window_merge_index = 0;
				ML_GetSongStructByPosition( currentPlayAllLevel, play_flash_window_pos, play_sd_window_pos, songStruct );
			}

			return 1;
		}
	}
	else if ( currentLvl == treeLvl && !temp ) {
		for ( temp=0;temp<5;temp++ ) {
			songPath[temp] = org_songPath[temp];
		}

		play_flash_name_list_size = org_play_flash_name_list_size;
		play_sd_name_list_size = org_play_sd_name_list_size;
		play_flash_window_pos = play_sd_window_pos = 0;
		currentPlayAllLevel = org_currentPlayAllLevel;
		if ( currentLvl == 1 ) {
			*songIndex = play_window_merge_index = 0;
			ML_GetSongStructByPosition( currentPlayAllLevel, play_flash_window_pos, play_sd_window_pos, songStruct );
		}

		return 1;
	}
	return 1;
}

COMP_OPT uint24 ML_PlayAllNextSong( RAM_SONG_INFO_T *songStruct, uint24 *songIndex ) {
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
		for ( temp=0;temp<5;temp++ ) {
			org_songPath[temp] = songPath[temp];
		}
		org_currentPlayAllLevel = currentPlayAllLevel;
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
		ret = ML_MoveDownSong( 1, playAllLevel, name, songStruct, songIndex );
		temp = playMode>>3;

		size = ( !temp ) ? &org_play_flash_name_list_size : &org_play_sd_name_list_size;
		if ( songIndexInMedia[temp] < (*size-1) )
			songIndexInMedia[temp]++;
		else
			songIndexInMedia[temp] = 0;
	}

 	return ret;
}

/*==================================================================================================
FUNCTION: MusicLibPlayAllNextModuleInit()

DESCRIPTION:
   MusicLib PlayAllNext Module Init.

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
int _reentrant MusicLibPlayAllNextModuleInit(struct ModuleEntry *pModuleTableEntry)
{

	//Tell ModuleTableEntry.wait to call this module's process function next when it has a msg
	//returnMask.I=0x000000 | 2; // EVENT_MESSAGE
	return (0x000000 | 2);
	//return returnMask.I;
}

/*==================================================================================================
FUNCTION: MusicLibPlayAllNextModuleProcessQueue()

DESCRIPTION:
   MusicLib PlayAllNext Module ProcessQueue, UI interface for all musiclib playing function.

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
int _reentrant MusicLibPlayAllNextModuleProcessQueue(union WaitMask Signals,struct MessageQueueDescriptor * pQueue)
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
				case MUSICLIB_PLAYNEXTSONG:
					if (g_current_play_all)
						ML_PlayAllNextSong(&song_info, (uint24*)Parm1);
					else
						ML_PlayNextSong(&song_info, (uint24*)Parm1);
					break;
				default:
					break;
				}
		}
	}
	return returnMask.I;
}