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
#define PLAY_ALL_START
#define BROWSE_AND_PLAY
#define PLAY_NEXT
#define SHUFFLE
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
COMP_OPT uint24 ML_StartPlaySong( uint24 highlight, RAM_SONG_INFO_T *songStruct, uint24 *songIndex, uint24 *totalSong );
COMP_OPT uint24 ML_StartPlayAllSong( uint24 hasSD, RAM_SONG_INFO_T *songStruct, uint24 *songIndex, uint24 *totalSong );
COMP_OPT uint24 LOCAL_GetArtistSong_info( uint24 memory_type, uint24 artistID, uint24 sortedListIndex, uint24 *info );
COMP_OPT uint24 GetArtistSongSize( uint24 memory_type, uint24 artistID, uint24 *size );
//COMP_OPT uint24 GetArtistSongID( uint24 memory_type, uint24 artistID, uint24 sortedListIndex, uint24 *id );
COMP_OPT void	ML_buildSongPath( uint24 *state, uint24 startSongPathIndex );
COMP_OPT void ML_SetShuffle( uint24 mode );
////COMP_OPT void	ML_ResumePlayState( RAM_SONG_INFO_T *songStruct, uint24 *songIndex, uint24 *totalSong );
//COMP_OPT void	ML_ResumePlayState( uint24 hasSD, RAM_SONG_INFO_T *songStruct, uint24 *songIndex, uint24 *totalSong );
_reentrant void ML_ResumePlayState( uint24 hasSD, RAM_SONG_INFO_T *songStruct, uint24 *songIndex, uint24 *totalSong );

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
COMP_OPT uint24 ML_StartPlayAllSong( uint24 hasSD, RAM_SONG_INFO_T *songStruct, uint24 *songIndex, uint24 *totalSong ) {

	uint24 i;
	uint24 temp;
	uint24 temp2;
	uint24 temp3;
	uint24 (*funcPt)( uint24, uint24, uint24* );

	hasInsertSD = hasSD;

	lastPlaySongLevel = current_state;
		
	play_flash_window_pos = 0;
	play_sd_window_pos = 0;
	play_window_merge_index = 0;
	play_flash_name_list_size = 0;
	play_sd_name_list_size = 0;

	if (lastPlaySongLevel == STATE_ONTHEGO) {
		IsPlayOnTheGo = TRUE;
	} else {
		IsPlayOnTheGo = FALSE;
	}

	if ( current_state >= STATE_GENRE_LVL_1 && isAtLevel1(current_state) ) {
		currentPlayAllLevel = STATE_TRACK_LVL_1;		
	}
	else {
		currentPlayAllLevel = current_state;		
	}

	playAllLevel = 2 + isUnderArtist( currentPlayAllLevel ) - (current_state&3);
	ML_selectItemInfoByState( 0, current_state, MEDIA_TYPE_FLASH, &songPath[4], &songPath[0], &songPath[2] );
	ML_selectItemInfoByState( 0, current_state, MEDIA_TYPE_SD, &songPath[4], &songPath[1], &songPath[3] );
	
	if ( ML_IsAtSongLevel( currentPlayAllLevel ) ) {
		ML_updateSize( 1 );
	}
	else {
		temp3 = isAtLevel1( currentPlayAllLevel );		
		if ( temp3 ) {
			funcPt = ML_GetLvlOneItemSortListSize;
			temp = temp2 = ITEM_TRACK;
		}
		else { 
			funcPt = GetArtistSongSize;
			temp = ML_GetData( songPath, 0 );
			temp2 = ML_GetData( songPath, 2 );
		}

		if ( temp != 0xFFF )
			funcPt( MEDIA_TYPE_FLASH, temp, &play_flash_name_list_size );
			
		if ( hasInsertSD && temp2 != 0xFFF )
			funcPt( MEDIA_TYPE_SD, temp2, &play_sd_name_list_size );		
	}
	play_total_merge_num_item = play_flash_name_list_size + play_sd_name_list_size;

	isPlayAll = 1;
	*songIndex = play_window_merge_index;
	*totalSong = play_total_merge_num_item;
	
	org_play_flash_name_list_size = play_flash_name_list_size;
	org_play_sd_name_list_size = play_sd_name_list_size;

	temp3 = getCurrentPlayMode( playMode );
	if ( temp3 != PLAY_SHUFFLE ) {
		temp2 = ( isAtLevel1( currentPlayAllLevel ) ) ? 0 : 2;
		/* Travse the tree to the song level */
		ML_buildSongPath( &currentPlayAllLevel, temp2 );		
		ML_GetSongStructByPosition( currentPlayAllLevel, play_flash_window_pos, play_sd_window_pos, songStruct );				
//		playMode = 4;
		playMode = (playMode&0xFFFFF8) | 4;
	}
	else {
		temp = ( isAtLevel1( lastPlaySongLevel ) ) ? STATE_TRACK_LVL_1 : lastPlaySongLevel;
		
		temp2 = ( play_flash_name_list_size > 0 ) ? 0 : 1;

		if ( ((playAllLevel-2) | (isUnderArtist( lastPlaySongLevel )-1) ) == 0 ){
			temp3 = songPath[temp2] & 0xFFF;
			//GetArtistSongID( temp2, temp3, 0, &i );
			LOCAL_GetArtistSong_info(temp2, temp3, 0, &i);
		}
		else
			ML_GetListAndID( 1, temp, temp2, 0, &temp3, &i );
			
		ML_ConstructSongStruct( i, temp2, 0, songStruct, 0 );
		ML_startShuffle( songIndex );
		currentPlayAllLevel = lastPlaySongLevel;
		
		playMode |= 7;			// set back to suffle mode
	}

	for ( i=0;i<5;i++ ) {
		org_songPath[i] = songPath[i];	
	}		
	org_currentPlayAllLevel = currentPlayAllLevel;

//	songIndexInMedia[0] = songIndexInMedia[1] = 0;

	temp = (playMode>>3);
	temp2 = ( temp == 0 ) ? 1 : 0;
	
//	songIndexInMedia[ temp2 ] = ( temp == 0 ) ? org_play_sd_name_list_size : org_play_flash_name_list_size;	
	songIndexInMedia[ temp2 ] = -1;
	songIndexInMedia[temp] = 0;
		
	return 1;
}

COMP_OPT uint24 ML_StartPlaySong( uint24 highlight, RAM_SONG_INFO_T *songStruct, uint24 *songIndex, uint24 *totalSong ) {
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
	
	if (lastPlaySongLevel == STATE_ONTHEGO) {
		IsPlayOnTheGo = TRUE;
	} else {
		IsPlayOnTheGo = FALSE;
	}
	if ( getCurrentPlayMode( playMode ) == PLAY_SHUFFLE ) {
		isPlayShuffle = 1;
//		playMode = (playMode&0xFFFFF8) | 4;	// change to normal mode temporary to let PlayNextSong work properly
//		playMode = 4;		// change to normal mode temporary to let PlayNextSong work properly
	}

	playMode = (playMode&0xFFFFF8) | 4;	// change to normal mode temporary to let PlayNextSong work properly
	
	if ( highlight == 0 ) {		
		ML_GetIDFromCurrentWindow( highlight, &songID, &songID2 );				
		temp = (songID & 0xFFF);		

		if ( temp != 0xFFF ) {
			item_type = MEDIA_TYPE_FLASH;
		}
		else {
			item_type = MEDIA_TYPE_SD;
			temp = songID2 & 0xFFF;
		}
		//ML_ConstructSongStruct( temp, item_type, (lastPlaySongLevel/STATE_AUDIBLE), songStruct );
		//ML_ConstructSongStruct( temp, item_type, 0, songStruct );
		ML_ConstructSongStruct( temp, item_type, (lastPlaySongLevel == STATE_AUDIBLE), songStruct, 0 );
	}
	else {		
		for ( i=0;i<highlight;i++ ) {
			ML_PlayNextSong( songStruct, songIndex );
		}
	}

	if ( isPlayShuffle ) {
		ML_startShuffle( songIndex );
		playMode |= 7;			// set back to suffle mode
//		playMode = 7;			// set back to suffle mode
	}
//	else {
//		playMode = 4;
//		playMode = (playMode&0xFFFFF8) | 4;
//	}

	*songIndex = play_window_merge_index;
	*totalSong = play_total_merge_num_item;

	return 1;
}

COMP_OPT void ML_SetShuffle( uint24 mode ) {
	playMode = (playMode & 0xFFFFFE) | ( mode & 1 );
}

#ifdef FUNCLET
    #pragma asm
        FML_ResumePlayState:

        nolist
        include "sysmacro.asm"
        list

        ;If using funclet, function will be executed via funclet kernel
        extern	RSRC_FUNCLET_ML_RESUMEPLAYSTATE
        CallFunclet RSRC_FUNCLET_ML_RESUMEPLAYSTATE

        org p,"SYSFUNCLET_ML_RESUMEPLAYSTATE_P":
        dc      RSRC_FUNCLET_ML_RESUMEPLAYSTATE
    #pragma endasm
_reentrant void ML_ResumePlayState_Funclet( uint24 hasSD, RAM_SONG_INFO_T *songStruct, uint24 *songIndex, uint24 *totalSong ) {
#else
//COMP_OPT void	ML_ResumePlayState( RAM_SONG_INFO_T *songStruct, uint24 *songIndex, uint24 *totalSong ) {
_reentrant void ML_ResumePlayState( uint24 hasSD, RAM_SONG_INFO_T *songStruct, uint24 *songIndex, uint24 *totalSong ) {
#endif
	uint24 i;
	uint24 state;
	uint24 playLevel;
	uint24 memory_type;
	uint24 isAudible;
	uint24 id;

	// test resume if insert SD and last song is under flash memory
	uint24	tempCurrentPlayAllLevel;
	uint24	tempPlayFlashNameListSize;
	uint24 	currentMode;
	uint24 	lastMode;
	uint24	usualcase;
	uint24	temp;	
	uint24	tempSongPath[5];

	memory_type = playMode / 8;

	// SD card is Removed
	if ( ((hasInsertSD-1)|hasSD) == 0 ) {			
		hasInsertSD = NO_SD;
		currentMode = playMode & 1;
		lastMode = playMode & 2;
		
		// remove SD information in songPath
		songPath[1] = songPath[3] = 0x000FFF;
		/* 
			usualcase : handle playall condition, 
			set TRUE if 1) playall+PLAY_NORMAL or 
			2) playall+(just set PLAY_SHUFFLE, but power down without actually playing next song) 
			ie. variables still with PLAY_NORMAL values 
		*/
		usualcase = ( (!isPlayAll || currentMode != PLAY_NORMAL) && (!isPlayAll || currentMode == PLAY_NORMAL || lastMode == PLAY_SHUFFLE) );
		
		if ( usualcase ) {
			if ( lastPlaySongLevel <= 0x11 ||lastPlaySongLevel == STATE_MAIN_MENU )
				play_total_merge_num_item -= play_sd_name_list_size;		
			else {
				ML_GetListAndID( 0, lastPlaySongLevel, MEDIA_TYPE_FLASH, 0, &temp, &i );
				play_flash_name_list_size = temp;
				play_total_merge_num_item = temp;				
			}
		}
		else
			play_total_merge_num_item -= org_play_sd_name_list_size;

		// Cancel SD card info		
		play_sd_name_list_size = 0;
		org_play_sd_name_list_size = 0;
		play_sd_window_pos = 0;		
		
		if ( !usualcase ) {
			// find the first song in play all mode
			tempCurrentPlayAllLevel = currentPlayAllLevel;
			tempPlayFlashNameListSize = play_flash_name_list_size;
			for ( temp=0;temp<5;temp++ ) 
				tempSongPath[temp] = songPath[temp];	
			if ( lastPlaySongLevel >= STATE_GENRE_LVL_1 && isAtLevel1(lastPlaySongLevel) ) {
				currentPlayAllLevel = STATE_TRACK_LVL_1;		
			}
			else {
				currentPlayAllLevel = lastPlaySongLevel;
			}
			temp = ( isAtLevel1( currentPlayAllLevel ) ) ? 0 : 2;
//			currentPlayAllLevel = lastPlaySongLevel;
			ML_buildSongPath( &currentPlayAllLevel, temp );
			for ( temp=0;temp<5;temp++ ) {
				org_songPath[temp] = songPath[temp];	
				songPath[temp] = tempSongPath[temp];
			}
			org_currentPlayAllLevel = currentPlayAllLevel;
			currentPlayAllLevel = tempCurrentPlayAllLevel;
			play_flash_name_list_size = tempPlayFlashNameListSize;
			songIndexInMedia[1] = 0;
			play_window_merge_index = songIndexInMedia[0];
		}
		else if ( currentMode == PLAY_NORMAL || (currentMode == PLAY_SHUFFLE && lastMode == PLAY_NORMAL) ) {
			play_window_merge_index = play_flash_window_pos;
		}
	} else {
		hasInsertSD = HAS_SD;
	}

	////////////////////////////////////////////////
//	playLevel = ( isPlayAll ) ? currentPlayAllLevel : lastPlaySongLevel;	
//debug_now( playLevel, playMode, 0, 0 );	
	if ( (playMode&3) == 3 ) {
		ML_startShuffle( songIndex );
		currentPlayAllLevel = lastPlaySongLevel;			
		play_window_merge_index = *songIndex;
	}
	
	id = lastSongID & 0xFFF;
	isAudible = lastSongID / 4096;
	if ( isAudible )
		memory_type = MEDIA_TYPE_FLASH;
//	ML_ConstructSongStruct( lastSongID, memory_type, songStruct );
	ML_ConstructSongStruct( id, memory_type, isAudible, songStruct, 0 );
	
	*songIndex = play_window_merge_index;
	*totalSong = play_total_merge_num_item;
}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextmlmmc_play_module":
    #pragma endasm
#endif

#ifdef _PC_SIMULATION_
void ML_Play_Init() {
//	currentSongID = 0xFFFFFF;
//	currentSongID_SD = 0xFFFFFF;
	isPlayAll = 0;	
	playMode = 4;
}
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
int _reentrant MusicLibPlayModuleInit(struct ModuleEntry *pModuleTableEntry)
{

//	currentSongID = 0xFFFFFF;
//	currentSongID_SD = 0xFFFFFF;
	isPlayAll = 0;
	playMode = 4;
	g_current_play_all = FALSE;
	
	lastSongID = 0;
	play_flash_window_pos = 0;
	play_flash_name_list_size = 0;
	lastPlaySongLevel = STATE_TRACK_LVL_1;
	play_total_merge_num_item = 0;
	play_window_merge_index = 0;	
	IsPlayOnTheGo = FALSE;
	
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
int _reentrant MusicLibPlayModuleProcessQueue(union WaitMask Signals,struct MessageQueueDescriptor * pQueue)
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
   				case MUSICLIB_START_PLAY_ALL_SONG:
				    	ML_StartPlayAllSong(g_is_SD_inserted, &song_info, &songIndex, (uint24*)Parm1 );		
				    	g_current_play_all = TRUE;
					break;				

				case MUSICLIB_START_PLAY_SONG:
				    	ML_StartPlaySong(Parm1, &song_info, &songIndex, &totalSong );
				    	*((uint24*)Parm2) = (uint24)totalSong;
				    	*((uint24*)Parm3) = (uint24)songIndex;
				    	g_current_play_all = FALSE;
					break;
				case MUSICLIB_PLAY_SETSHUFFLE:
					ML_SetShuffle( (uint24)Parm1 );
					break;
				case MUSICLIB_PLAY_RESUME:
//					ML_ResumePlayState( &song_info, (uint24*)Parm2/*&songIndex*/, (uint24*)Parm1/*&totalSong*/ );
					ML_ResumePlayState( Parm3, &song_info, (uint24*)Parm2/*&songIndex*/, (uint24*)Parm1/*&totalSong*/ );
//					*((uint24*)Parm1) = (uint24)totalSong;
//					*((uint24*)Parm2) = (uint24)songIndex;

					break;
				case MUSICLIB_PLAY_CURRENT_SIZE:
				    *((uint24*)Parm1) = (uint24)play_total_merge_num_item;
					break;					
				default: 
					break;
				}

		}
	}    
	return returnMask.I;
}

#endif