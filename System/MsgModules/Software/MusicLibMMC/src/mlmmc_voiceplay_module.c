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
#define BROWSE_AND_PLAY
#define PLAY_NEXT
#define BROWSE_PLAY_MODULE
#include "..\hdr\musiclib_hdr.h"

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
#define VOICE_PLAY
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
COMP_OPT uint24 ML_StartPlayVoice( uint24 highlight, RAM_SONG_INFO_T *songStruct, uint24 *songIndex, uint24 *totalSong );

//COMP_OPT void debug_now(int a, int b, int *pt1, int *pt2);
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
COMP_OPT uint24 ML_StartPlayVoice( uint24 highlight, RAM_SONG_INFO_T *songStruct, uint24 *voiceIndex, uint24 *totalVoice ) {
	uint24 i;
	uint24 songID, songID2;
	uint24 item_type;
	uint24 temp;
	uint24 isPlayShuffle = 0;

	isPlayAll = 0;

	/* store folder path */
	item_type = current_state>>2;

	songPath[4] = item_type;
	for ( i=0;i<4;i++ )
		songPath[i] = browsePath[i];

	// store the starting pointers of two list for restore the display after playback
	play_flash_window_pos = flash_window_pos;
	play_sd_window_pos = sd_window_pos;
	play_flash_name_list_size = flash_name_list_size;
	play_sd_name_list_size = sd_name_list_size;

	lastPlaySongLevel = current_state;
	play_total_merge_num_item = total_merge_num_item;
	play_window_merge_index = window_merge_index;

	IsPlayOnTheGo = FALSE;
	/*
	if ( getCurrentPlayMode( playMode ) == PLAY_SHUFFLE ) {
		isPlayShuffle = 1;
//		playMode = (playMode&0xFFFFF8) | 4;	// change to normal mode temporary to let PlayNextSong work properly
//		playMode = 4;		// change to normal mode temporary to let PlayNextSong work properly
	}
	*/
	playMode = (playMode&0xFFFFF8) | 4;	// change to normal mode temporary to let PlayNextSong work properly

	if ( highlight == 0 ) {
		ML_GetIDFromCurrentWindow( highlight, &songID, &songID2 );
		temp = (songID & 0xFFF);

		if (item_type == ITEM_VOICE) {
			ML_ConstructSongStruct( temp, MEDIA_TYPE_FLASH, FALSE, songStruct, 1 );
		} else if (item_type == ITEM_FMREC) {
			ML_ConstructSongStruct( temp, MEDIA_TYPE_FLASH, FALSE, songStruct, 2 );
		} else if (item_type == ITEM_PHOTO) {
			ML_ConstructSongStruct( temp, MEDIA_TYPE_FLASH, FALSE, songStruct, 3 );
		} else {
			ML_ConstructSongStruct( temp, MEDIA_TYPE_FLASH, FALSE, songStruct, 4 );
		}
	}
	else {
		for ( i=0;i<highlight;i++ ) {
			ML_PlayNextSong( songStruct, voiceIndex );
		}
	}

/*
	if ( isPlayShuffle ) {
		ML_startShuffle( songIndex );
		playMode |= 7;			// set back to suffle mode
//		playMode = 7;			// set back to suffle mode
	}
//	else {
//		playMode = 4;
//		playMode = (playMode&0xFFFFF8) | 4;
//	}
*/

	*voiceIndex = play_window_merge_index;
	*totalVoice = play_total_merge_num_item;

	return 1;
}

#ifdef _PC_SIMULATION_
#else
/*==================================================================================================
FUNCTION: MusicLibPlayModuleInit()

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
int _reentrant MusicLibVoicePlayModuleInit(struct ModuleEntry *pModuleTableEntry)
{

//	currentSongID = 0xFFFFFF;
//	currentSongID_SD = 0xFFFFFF;

	//Tell ModuleTableEntry.wait to call this module's process function next when it has a msg
	//returnMask.I=0x000000 | 2; // EVENT_MESSAGE
	return (0x000000 | 2);
	//return returnMask.I;
}

/*==================================================================================================
FUNCTION: MusicLibPlayModuleProcessQueue()

DESCRIPTION:
   MusicLib Play Module ProcessQueue, UI interface for all musiclib playing function.

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
int _reentrant MusicLibVoicePlayModuleProcessQueue(union WaitMask Signals,struct MessageQueueDescriptor * pQueue)
{
	union WaitMask returnMask;
	struct CMessage  Msg;

	uint24	songIndex, totalSong;
	int24 Parm1, Parm2, Parm3;

	returnMask.I=0x000000 | 2;//Initialize it to signal us if we have a message

	//handle incoming messages
	if(Signals.B.m_bMessage) {
		while(GetMessage(pQueue,&Msg)) {
			Parm1 = Msg.m_uMsg[1];
			Parm2 = Msg.m_uMsg[2];
			Parm3 = Msg.m_uMsg[3];

	            switch(Msg.m_uMsg[0])
	            {
				case MUSICLIB_START_PLAY_VOICE:
				    	ML_StartPlayVoice(Parm1, &song_info, &songIndex, &totalSong );
				    	*((uint24*)Parm2) = (uint24)totalSong;
				    	*((uint24*)Parm3) = (uint24)songIndex;
				    	g_current_play_all = FALSE;
					break;
				/*
				case MUSICLIB_PLAY_CURRENT_SIZE:
				    *((uint24*)Parm1) = (uint24)play_total_merge_num_item;
					break;
				*/
				default:
					break;
				}

		}
	}
	return returnMask.I;
}

#endif