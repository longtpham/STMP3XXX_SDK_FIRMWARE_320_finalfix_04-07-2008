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
#define PLAY_ALL_START
#define SHUFFLE
#define SHUFFLE_NEXT
#define	SHUFFLE_UTILITY
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
COMP_OPT void	ML_ShuffleNextSong( uint24 direction, RAM_SONG_INFO_T *songStruct, uint24 *songIndex );
//COMP_OPT void	ML_SetGetPlayState( uint24 action, PLAY_SONG_STATE_T *songState );

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
COMP_OPT void	ML_ShuffleNextSong( uint24 direction, RAM_SONG_INFO_T *songStruct, uint24 *songIndex ) {
	ML_getNextShuffleSong( direction, lastPlaySongLevel, songStruct, songIndex );	
}
/*==================================================================================================
FUNCTION: MusicLibShuffleModuleInit()

DESCRIPTION:
   MusicLib Shuffle Module Init.

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
int _reentrant MusicLibShuffleModuleInit(struct ModuleEntry *pModuleTableEntry)
{
	//Tell ModuleTableEntry.wait to call this module's process function next when it has a msg
	//returnMask.I=0x000000 | 2; // EVENT_MESSAGE
	return (0x000000 | 2);
	//return returnMask.I;  
}

/*==================================================================================================
FUNCTION: MusicLibShuffleModuleProcessQueue()

DESCRIPTION:
   MusicLib Shuffle Module ProcessQueue, UI interface for all musiclib playing function.

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
int _reentrant MusicLibShuffleModuleProcessQueue(union WaitMask Signals,struct MessageQueueDescriptor * pQueue)
{
	union WaitMask returnMask; 
	struct CMessage  Msg;
	
	uint24	songIndex, totalSong;
	int24 Parm1, Parm2;

	returnMask.I=0x000000 | 2;//Initialize it to signal us if we have a message

	//handle incoming messages
	if(Signals.B.m_bMessage) {
		while(GetMessage(pQueue,&Msg)) {
			Parm1 = Msg.m_uMsg[1];
			Parm2 = Msg.m_uMsg[2];
			
			switch(Msg.m_uMsg[0]) {	            					    	           	
	            		case MUSICLIB_SHUFFLE_NEXT:
	            			ML_ShuffleNextSong( Parm1, &song_info, (uint24*)Parm2 );
	            			break;

				default: 
					break;
			}
		}    
	}
	return returnMask.I;
}

