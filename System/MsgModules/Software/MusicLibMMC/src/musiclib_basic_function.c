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
	<Project_Directory>\system\msgmodules\software\MusicLib_Player\src\musiclib_basic_function.c


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


Notes:

This version use static ram as temp_name_buf[], compilable on tasking.

====================================================================================================
                                            DESCRIPTION
====================================================================================================

GLOBAL FUNCTIONS:
   MF_global_func_name()

called by UI, flush sorted and associate list to flash when no more song needed to be added
COMP_OPT void FlushLibraryToFlash(void) {
	for each group X in (artist,album,genre) {
		flush ram_song_group.sorted_list to flash_song_group.sorted_list flash;
		flush ram_X_group.sorted_list to flash_X_group.sorted_list;
		do a counting on ram_X_group.associated_list to check to number of child and song ID associated with that group;
		flush the counted result to flash_X_group.associated_list;
		flush num_of_song to flash_num_of_song;
		flush num_of_artist to flash_num_of_artist;
		flush num_of_album to flash_num_of_album;
		flush num_of_genre to flash_num_of_genre;
	}
}

Item: The global variable, int24 *g_wSongTitle, int24 *g_wSongArtist, int24
*g_wSongAlbum, int24 *g_wSongGenre, int24 *g_wSongYear, int24 *g_wSongPath
and int g_Unicode are grouped into one struct RAM_SONG_INFO_T, see section
5.4.1
New function is added to flush the music library to flash after all the
songs are added.

COMP_OPT void AddEntryToLibrary(RAM_SONG_INFO_T *song_info, int16 option) {
	num_of_song++;
	flush song_info.song_name to flash_song_name[Media_type][num_of_song].name;
	flush song_info.g_unicode to flash_song_name[Media_type][num_of_song].g_unicode;
	flush song_info.path_name to flash_path_name[num_of_song];
	func(sort the song name) {
		locate the start and end index in the ram_song_group.mark_table_index
		using binary search in the mark table, by comparing the new song name with
		ram_song_group.mark_table_string;

		load the song names from flash indicated by the start and end index to
		temp_name_buf in sorted order;

		start binary search in temp_name_buf to locate the exact position for
		putting the new song ID (ie, num_of_song) in;
		insert the new song ID (ie, num_of_song) into ram_song_group.sorted_list
		in proper location;

		insert the new track number to ram_song_group.associate_list[num_of_song];

		func(update the mark table) {
			increase the distance of specific mark entry;

			if the distance is larger than threshold, split the entries in the
			mark table into half;
		}
	}

	for each group X in (artist,album,genre) {
		locate the start and end index in the ram_X_group.mark_table_index using a
		binary search in the mark table, by comparing the new group X name with
		ram_X_group.mark_table_string;

		load the group X names from flash indicated by the start and end index to temp_name_buf;

		start binary search in ram_X_group.sorted_list to locate the exact
		position for putting the group ID in;

		if the group ID does not exist before {
			num_of_X++;

			insert the group ID (num_of_X) into ram_X_group.sorted_list in proper
			location if it does not exist before;

			flush song_info.X_name to flash_X_name[num_of_X];

			flush song_info.g_unicode to flash_X_name[num_of_song].g_unicode;

			func(update the mark table) {
				increase the distance of specific mark entry;

				if the distance is larger than threshold, split the entries in the
				mark table into half.
			}
		}
		for artist group, the associate list contains the artist ID;
		for album group, the associate list contains the album ID;
		for genre group, the associate list contains the genre ID

		insert the new or existing group ID into ram_X_group.associated_list[num_of_song];
	}
}




LOCAL  FUNCTIONS:
   mf_local_func_name()  

====================================================================================================


TRACEABILITY MATRIX:
   None

==================================================================================================*/

/*==================================================================================================
                                           INCLUDE FILES
==================================================================================================*/
#include	"..\hdr\musiclib_hdr.h"

#ifdef _PC_SIMULATION_
#if 1
#define	_ASSERT_
#endif

#ifdef	_ASSERT_
#include	<assert.h>
#endif

#define COMP_OPT_G
#endif

/*==================================================================================================
								Conditional Compilation Directives
==================================================================================================*/

#if 1
#define _SKIP_PERPOSITION_
#endif

#if 1
#define _SORTBY_CAPITAL_
#endif

#if 1
#define _USE_SMALLEND_	/* you _must_use_ this ! since the big endien code is not fullly implemented */  
#endif

/*==================================================================================================
                                         LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
                                         LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
                                         LOCAL ENUMS
==================================================================================================*/
#define SIZE_OF_THE_STR		4
#define SIZE_OF_A_STR		2
#define SIZE_OF_AN_STR		3

/*==================================================================================================
                                     LOCAL STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
                                     LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
                                          LOCAL VARIABLES
==================================================================================================*/
#ifndef _COMP_USE_VARIABLE_
#define _COMP_USE_VARIABLE_

#ifndef VOICE_FUNCTION
#ifdef MERGE_FUNCTION
static uint24	macro_temp_var;		/* counter used in macro functions */
#else
COMP_OPT_G uint24	macro_temp_var;		/* counter used in macro functions */

/*==================================================================================================
                                          GLOBAL VARIABLES
==================================================================================================*/

/* RAM */
COMP_OPT_G uint24 _X num_of_item[MAX_NUM_OF_GROUP];
#ifdef _PC_SIMULATION_
COMP_OPT_G uint24 num_of_item_y[MAX_NUM_OF_GROUP];
#else
extern uint24 num_of_item_y[MAX_NUM_OF_GROUP];
#endif

/* Flash */
COMP_OPT_G FLASH_GROUP_NAME_T flash_group_name_y;
#endif
#endif /* _MERGE_FUNCTION_ */
#endif	/* VOICE_FUNCTION */
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

/*================================================================================================*/
/*================================================================================================*/
#ifdef BROWSE_PLAY_MODULE
_reentrant uint24 ML_strhead_pos_BROWSE(uint24 unicode, const uint24 _X *scr) {
#else
#ifdef VOICE_FUNCTION
COMP_OPT uint24 ML_strhead_pos_Voice(uint24 unicode, const uint24 _X *scr) {
#else
COMP_OPT uint24 ML_strhead_pos(uint24 unicode, const uint24 _X *scr) {
#endif
#endif
#ifdef _SKIP_PERPOSITION_
uint24	byte0;
uint24	byte1;
uint24	byte2;

#ifdef _USE_SMALLEND_
	byte0 = (scr[0] & 0x0000FF);
#endif
	/* chk zero length str */
	if (((unicode == BITSET_NAME_ALLASCII) && (byte0 == 0x000000)) || (scr[0] == EOSTR)) {
		return (0);
	}
	if (unicode == BITSET_NAME_ALLASCII) {
#ifdef _USE_SMALLEND_
		byte1 = (scr[0] & 0x00FF00);
		byte2 = (scr[0] & 0xFF0000);
		//if (((cpysize >= SIZE_OF_THE_STR) && ((scr[1] & 0x0000FF) == 0x20)) &&
		if (((scr[1] & 0x0000FF) == 0x20) &&
			((byte0 == 0x000054) || (byte0 == 0x000074)) &&
			((byte1 == 0x004800) || (byte1 == 0x006800)) &&
			((byte2 == 0x450000) || (byte2 == 0x650000))) {
			/* "the " filter */
			return (SIZE_OF_THE_STR);
		//} else if (((cpysize >= SIZE_OF_A_STR) && (byte1 == 0x002000)) &&
		} else if ((byte1 == 0x002000) &&
					((byte0 == 0x000041) || (byte0 == 0x000061))) {
			/* "a " filter */
			return (SIZE_OF_A_STR);
		//} else if (((cpysize >= SIZE_OF_AN_STR) && (byte2 == 0x200000)) &&
		} else if ((byte2 == 0x200000) &&
					((byte0 == 0x000041) || (byte0 == 0x000061)) &&
					((byte1 == 0x004E00) || (byte1 == 0x006E00))) {
			/* "an " filter */
			return (SIZE_OF_AN_STR);
		}
#else /* !(_USE_SMALLEND_) */
		/* not implemented */
#ifdef _ASSERT_
		assert(0);
#endif
#endif /* !(_USE_SMALLEND_) */
	} else {
#ifdef _USE_SMALLEND_
		//if (((cpysize >= SIZE_OF_THE_STR) && (scr[3] == 0x000020)) &&
		if ((scr[3] == 0x000020) &&
			((scr[0] == 0x54) || (scr[0] == 0x74)) &&
			((scr[1] == 0x48) || (scr[1] == 0x68)) &&
			((scr[2] == 0x45) || (scr[2] == 0x65))) {
			/* "the " filter */
			return (SIZE_OF_THE_STR);
		//} else if (((cpysize >= SIZE_OF_A_STR) && (scr[1] == 0x000020)) &&
		} else if ((scr[1] == 0x000020) &&
					((scr[0] == 0x41) || (scr[0] == 0x61))) {
			/* "a " filter */
			return (SIZE_OF_A_STR);
		//} else if (((cpysize >= SIZE_OF_AN_STR) && (scr[2] == 0x000020)) &&
		} else if ((scr[2] == 0x000020) &&
					((scr[0] == 0x41) || (scr[0] == 0x61)) &&
					((scr[1] == 0x4E) || (scr[1] == 0x6E))) {
			/* "an " filter */			
			return (SIZE_OF_AN_STR);
		}
#else /* !(_USE_SMALLEND_) */
		/* not implemented */
#ifdef _ASSERT_
		assert(0);
#endif
#endif /* !(_USE_SMALLEND_) */
	}
#endif	/*	_SKIP_PERPOSITION_ */

	return (0);	/* for start pos */
}

/*================================================================================================*/
#ifdef BROWSE_PLAY_MODULE
_reentrant uint24 ML_extract_byte_BROWSE(uint24 unicode, const uint24 _X *scr, uint24 start_pos) {
#else
#ifdef VOICE_FUNCTION
COMP_OPT uint24 ML_extract_byte_Voice(uint24 unicode, const uint24 _X *scr, uint24 start_pos) {
#else
COMP_OPT uint24 ML_extract_byte(uint24 unicode, const uint24 _X *scr, uint24 start_pos) {
#endif
#endif
uint24	j;
uint24	k;

uint24	byteout;

	if (start_pos >= LEN_OF_NAME_IN_BYTE) return (EOSTR);
	if (unicode == BITSET_NAME_ALLASCII) {
		j = start_pos/NUM_OF_BYTE_IN_ONE_WORD;
		k = start_pos - (j*NUM_OF_BYTE_IN_ONE_WORD);
		if (k == 0x00) {
#ifdef _USE_SMALLEND_
			byteout = (scr[j] & 0x0000FF);
#else
			byteout = ((scr[j] & 0xFF0000)>>16);
#endif
		} else if (k == 0x01) {
			byteout = ((scr[j] & 0x00FF00)>>8);
		//} else if (k == 0x02) {	// >24 bits
		} else {	//24 bits only
#ifdef _USE_SMALLEND_
			byteout = ((scr[j] & 0xFF0000)>>16);
#else
			byteout = (scr[j] & 0x0000FF);
#endif
		}
	} else {
	/* not unicode */
		if (start_pos >= (LEN_OF_NAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD)) {
			return (EOSTR);
		} else {
			byteout = scr[start_pos];
		}
	}
#ifdef _SORTBY_CAPITAL_
	/* convert to ASCII capital letters */
	if ((byteout > 0x60) && (byteout < 0x7B)) {
		byteout -= 0x20;
	}
#endif

	return (byteout);
}

#ifndef BROWSE_PLAY_MODULE
/*==================================================================================================
FUNCTION: ML_extract_string()

DESCRIPTION:
   extract the ALL_ASCII string from 1 word to 3 word, but direct copy the unicode/mix type string.

ARGUMENTS PASSED:
   uint24 unicode, uint24 *tgr, const uint24 *scr, uint24 cpysize
   
RETURN VALUE:
   copied size
   
PRE-CONDITIONS:
   tgr space _must_be_bigger_than LEN_OF_NAME_IN_BYTE.

POST-CONDITIONS:
   None

IMPORTANT NOTES:
   spysize < LEN_OF_NAME_IN_BYTE
   (cpysize=num of word to read, should not bigger than LEN_OF_NAME_IN_BYTE)
==================================================================================================*/
#ifdef VOICE_FUNCTION
COMP_OPT uint24 ML_extract_string_Voice(uint24 unicode, uint24 *tgr, const uint24 _X *scr, uint24 cpysize) {
#else
COMP_OPT uint24 ML_extract_string(uint24 unicode, uint24 *tgr, const uint24 _X *scr, uint24 cpysize) {
#endif
uint24	j;
uint24	k;
#if 1

	/* this will cause mem overwritten if tgr space is smaller than cpysize */
#ifdef VOICE_FUNCTION
	j = ML_strhead_pos_Voice(unicode, scr);
#else
	j = ML_strhead_pos(unicode, scr);
#endif
	for (k=0;k<(cpysize-j);k++) {
#ifdef VOICE_FUNCTION
		tgr[k] = ML_extract_byte_Voice(unicode, scr, (j+k));
#else
		tgr[k] = ML_extract_byte(unicode, scr, (j+k));
#endif
		if (tgr[k] == EOSTR) return (k);
	}
	if (k < cpysize) {
		tgr[k] = EOSTR;
	}

#else

uint24	byte0;
uint24	byte1;
uint24	byte2;

#ifdef _USE_SMALLEND_
	byte0 = (scr[0] & 0x0000FF);
#endif
	/* chk zero length str */
	if (((unicode == BITSET_NAME_ALLASCII) && (byte0 == 0x000000)) || (scr[0] == EOSTR)) {
		tgr[0] = EOSTR;
		return (0);
	}
	k = 0;
	j = 0;

#ifdef _SKIP_PERPOSITION_
	if (unicode == BITSET_NAME_ALLASCII) {
#ifdef _USE_SMALLEND_
		byte1 = (scr[0] & 0x00FF00);
		byte2 = (scr[0] & 0xFF0000);
		//if (((cpysize >= SIZE_OF_THE_STR) && ((scr[1] & 0x0000FF) == 0x20)) &&
		if (((scr[1] & 0x0000FF) == 0x20) &&
			((byte0 == 0x000054) || (byte0 == 0x000074)) &&
			((byte1 == 0x004800) || (byte1 == 0x006800)) &&
			((byte2 == 0x450000) || (byte2 == 0x650000))) {
			/* "the " filter */
			j = 2;
			cpysize -= SIZE_OF_THE_STR;
			tgr[k++] = ((scr[1] & 0x00FF00)>>8);
			if (k >= cpysize) return (k);
			tgr[k++] = ((scr[1] & 0xFF0000)>>16);
			if (k >= cpysize) return (k);
#ifdef _SAVENAMESHORTCUT_
			if (ChkTermInWord(scr[1])) return (k);
#endif
		//} else if (((cpysize >= SIZE_OF_A_STR) && (byte1 == 0x002000)) &&
		} else if ((byte1 == 0x002000) &&
					((byte0 == 0x000041) || (byte0 == 0x000061))) {
			/* "a " filter */
			j = 1;
			cpysize -= SIZE_OF_A_STR;
			tgr[k++] = (byte2>>16);
			if (k >= cpysize) return (k);
#ifdef _SAVENAMESHORTCUT_
			if (ChkTermInWord(scr[0])) return (k);
#endif
		//} else if (((cpysize >= SIZE_OF_AN_STR) && (byte2 == 0x200000)) &&
		} else if ((byte2 == 0x200000) &&
					((byte0 == 0x000041) || (byte0 == 0x000061)) &&
					((byte1 == 0x004E00) || (byte1 == 0x006E00))) {
			/* "an " filter */			
			j = 1;
			cpysize -= SIZE_OF_AN_STR;
		}
#else /* !(_USE_SMALLEND_) */
		/* not implemented */
#ifdef _ASSERT_
		assert(0);
#endif
#endif /* !(_USE_SMALLEND_) */
	} else {
#ifdef _USE_SMALLEND_
		//if (((cpysize >= SIZE_OF_THE_STR) && (scr[3] == 0x000020)) &&
		if ((scr[3] == 0x000020) &&
			((scr[0] == 0x54) || (scr[0] == 0x74)) &&
			((scr[1] == 0x48) || (scr[1] == 0x68)) &&
			((scr[2] == 0x45) || (scr[2] == 0x65))) {
			/* "the " filter */
			j = SIZE_OF_THE_STR;
		//} else if (((cpysize >= SIZE_OF_A_STR) && (scr[1] == 0x000020)) &&
		} else if ((scr[1] == 0x000020) &&
					((scr[0] == 0x41) || (scr[0] == 0x61))) {
			/* "a " filter */
			j = SIZE_OF_A_STR;
		//} else if (((cpysize >= SIZE_OF_AN_STR) && (scr[2] == 0x000020)) &&
		} else if ((scr[2] == 0x000020) &&
					((scr[0] == 0x41) || (scr[0] == 0x61)) &&
					((scr[1] == 0x4E) || (scr[1] == 0x6E))) {
			/* "an " filter */			
			j = SIZE_OF_AN_STR;
		}
#else /* !(_USE_SMALLEND_) */
		/* not implemented */
#ifdef _ASSERT_
		assert(0);
#endif
#endif /* !(_USE_SMALLEND_) */
	}
#endif	/*	_SKIP_PERPOSITION_ */

	while (j<LEN_OF_NAME_IN_BYTE) {
		if (unicode == BITSET_NAME_ALLASCII) {
#ifdef _USE_SMALLEND_
			tgr[k++] = (scr[j] & 0x0000FF);
#else
			tgr[k++] = ((scr[j] & 0xFF0000)>>16);
#endif
			if (k >= cpysize) break;
			tgr[k++] = ((scr[j] & 0x00FF00)>>8);
			if (k >= cpysize) break;
#ifdef _USE_SMALLEND_
			tgr[k++] = ((scr[j] & 0xFF0000)>>16);
#else
			tgr[k++] = (scr[j] & 0x0000FF);
#endif
			if (k >= cpysize) break;
#ifdef _SAVENAMESHORTCUT_
			if (ChkTermInWord(scr[j])) break;
#endif
		} else {
		/* not unicode */
			tgr[k++] = scr[j];
			if (k >= cpysize) break;
#ifdef _SAVENAMESHORTCUT_
			if (scr[j] == EOSTR) break;
#endif
		}
		j++;
	}
#ifdef _SORTBY_CAPITAL_
	/* convert to ASCII capital letters */
	for (j=0;j<cpysize;j++) {
		if ((tgr[j] > 0x60) && (tgr[j] < 0x7B)) {
			tgr[j] -= 0x20;
		}
	}
#endif

#endif
	return (k);
}
#endif
/*================================================================================================*/
#ifdef BROWSE_PLAY_MODULE
_reentrant void ML_list_get_val_X_Browse(uint24 _X *uint24_p, uint24 pos, uint24 *val) {
#else
COMP_OPT void ML_list_get_val_X(uint24 _X *uint24_p, uint24 pos, uint24 *val) {
#endif
	//*val = uint24_p[(macro_temp_var = pos/NUM_OF_ITEM_IN_ONE_WORD)]; 
	*val = uint24_p[(pos/NUM_OF_ITEM_IN_ONE_WORD)]; 
	if ((pos & 0x01) == 0x01) { 
		*val = (*val & 0x000FFF); 
	} else { 
		*val = ((*val & 0xFFF000)>>12); 
	}
}

/*================================================================================================*/
