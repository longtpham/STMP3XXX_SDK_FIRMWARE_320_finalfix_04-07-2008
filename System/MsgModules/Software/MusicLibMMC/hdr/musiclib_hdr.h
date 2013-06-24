#ifndef MUSICLIB_HDR_H
#define MUSICLIB_HDR_H

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================

                                        General Description

====================================================================================================

                               Sigmatel Inc Confidential Proprietary
                               (c) Copyright Sigmatel Inc 2004, All Rights Reserved

HEADER NAME: mf_sf_filename.h   AUTHOR: Developer Name      CREATION DATE: DD/MM/YYYY

PRODUCT NAMES: All

GENERAL DESCRIPTION:

   General description of this grouping of functions.

Portability: All


Revision History:

                         Modification        Tracking
Author                       Date             Number           Description of Changes
---------------------    ------------        ----------        -------------------------------------
Developer Name            DD/MM/YYYY         PDaaaxxxxx        brief description of changes made


====================================================================================================
                                            DESCRIPTION
====================================================================================================

GLOBAL FUNCTIONS:
   MF_global_func_name()

TRACEABILITY MATRIX:
   None

==================================================================================================*/

/*==================================================================================================
                                           INCLUDE FILES
==================================================================================================*/
#include "..\ghdr\musiclib_ghdr.h"

#ifdef _PC_SIMULATION_
#include	<stdio.h>
#include	<stdlib.h>
#endif
/*==================================================================================================
								Conditional Compilation Directives
==================================================================================================*/
/* To specify the music library is run in the player */
#define _ADD_ENTRY_TO_LIBRARY_ALOG_IN_PLAYER_

#define	_24_bit_word_

/*==================================================================================================
						Debug compile only Conditional Compilation Directives
==================================================================================================*/
#if 0
/* debug name display/order 05/02/15 sorting is incorrect when Flash and SD is merged */
/* print song id (insert order) in song name */
/* name format: "XXXNAME": XXX=3 digit id # : NAME=original name */
/* name format: "YXXNAME": if flash then Y=0:if SD then Y=1 : XX=2 digit id # : NAME=original name */
#define _PRINTSONGID_
#define _PRINTSONGID_UNICODE_	/* if ou want to debug unicode as well, but code space may not available */
#endif

/*==================================================================================================
                                             CONSTANTS
==================================================================================================*/
/* maximum number of artist */
#define MAX_NUM_OF_ARTIST (MAX_NUM_OF_SONG)
/* maximum number of album */
#define MAX_NUM_OF_ALBUM (MAX_NUM_OF_SONG)
/* maximum number of genre */
#define MAX_NUM_OF_GENRE (MAX_NUM_OF_SONG)
/* maximum number of year */
#define MAX_NUM_OF_YEAR (MAX_NUM_OF_SONG)
/* number of bit in one DSP word */
#define NUM_OF_BIT_IN_ONE_WORD (24)
/* number of bit for one item = log2(MAX_NUM_OF_SONG), corresponding to the absolute
number of song (2048) supported set above */
#define NUM_OF_BIT_FOR_ONE_ITEM (12)
/* number of item in one DSP word */
#define NUM_OF_ITEM_IN_ONE_WORD (NUM_OF_BIT_IN_ONE_WORD/NUM_OF_BIT_FOR_ONE_ITEM)
/* in the flash child array for one group, there will be two (2) information, one is
number of child for that group ID, another information is the child ID(s)*/
#define NUM_OF_LIST_IN_CHILD_ARRAY (2)

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
                                               MACROS
==================================================================================================*/

/*==================================================================================================
                                               ENUMS
==================================================================================================*/
#ifdef _PC_SIMULATION_
#define _X
/* refer to:\view\player\inc\types.h */
#define SUCCESS (0)
#endif

#define	TRUE	1
#define	FALSE	0

#define	EOSTR	0x00

#define BITSET_NAME_ALLASCII	(0x0)
#define BITSET_NAME_UNICODE		(0x01)
#define BITSET_NAME_MIXCODE		(0x02)

/* ram_artist_marktable = id0, ram_album_marktable = id1, ram_genre_marktable = id2, id3 reserved for song, ram_year_marktable = id4 */
//#define MAX_NUM_OF_GROUP	0x6	/* include ram_song_marktable */
#define MAX_NUM_OF_GROUP	0x5	/* include ram_song_marktable */

/* Constant for UI State */
#define  STATE_MAIN_MENU   0xA8  /* Main Menu */
#define  STATE_ARTIST_LVL_1  0x00  /* Show Aritst Name */
#define  STATE_ARTIST_LVL_2  0x01  /* Show Album Name for particular Artist */
#define  STATE_ARTIST_LVL_3  0x02  /* Show Song Name for particular Artist */
#define  STATE_ALBUM_LVL_1  0x04  /* Show Ablum Name */
#define  STATE_ALBUM_LVL_2  0x05  /* Show Song Name for particular Album */
#define  STATE_GENRE_LVL_1  0x08  /* Show Genre Name */
#define  STATE_GENRE_LVL_2  0x09  /* Show Song Name for particular Genre */
#define  STATE_TRACK_LVL_1  0x0C  /* Show Song Name */
#define  STATE_YEAR_LVL_1  0x10  /* Show Year */
#define  STATE_YEAR_LVL_2  0x11  /* Show Song Name for paritcular Year */
#define	 STATE_1DAY	   0x28	 /* Show Song in New Music -- 1 Day */
#define	 STATE_1WEEK	   0x2C	 /* Show Song in New Music -- 1 Week */
#define	 STATE_1MONTH	   0x30	 /* Show Song in New Music -- 1 Month */
#define	 STATE_AUDIBLE	   0x34  /* Show Song in Spocken Word */
#define	 STATE_ONTHEGO	   0x38  /* Show Song in On The Go */
#define	 STATE_VOICE	   0x3C  /* Show Song in Voice */
#define	 STATE_FMREC	   0x40  /* Show Song in FM Rec */
#define  STATE_PHOTO		0x44  /* Show Picture in Photo */
#define  STATE_MVIDEO		0x48  /* Show Movie in MVideo */
#define  STATE_PLAY_SONG   0xAB  /* Play Song */
#define  STATE_PLAY_ALL   0xAC  /* Play All Song */

#define ML_SONG_NAME_STRUCT_SIZE		(LEN_OF_NAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD + 1)
#define ML_PATH_NAME_STRUCT_SIZE		((LEN_OF_FILEPATH_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD)+2)
#define ML_LONG_FILENAME_STRUCT_SIZE	(LEN_OF_LONG_FILENAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD)
#define ML_LIST_STRUCT_SIZE				(MAX_NUM_OF_SONG)	/* this is no checked now */
#define ML_INDEX_STRUCT_SIZE			(1)	/* this is no checked now */
#define ML_VALUE_STRUCT_SIZE			(1)	/* this is no checked now */

#define ML_VOICE_MARKTABLE_STRUCT_SIZE	((NUM_OF_MARK_TABLE_ENTRY*NUM_OF_MARK_TABLE_COL)+(NUM_OF_MARK_TABLE_ENTRY*(LEN_OF_SUB_STRING_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD))+1)	// 949 // +1 for num_of info /* this size refer to musiclib_voice_function.c */
#define ML_VOICE_INFO_STRUCT_SIZE		((MAX_NUM_OF_VOICE/NUM_OF_ITEM_IN_ONE_WORD)+2)	// 502 /* this size refer to musiclib_voice_function.c */

#define ARTIST_SORTLOOP			(0)	/* this is the loop id of gp_count variable, _do_not_change_it_ */
#define ALBUM_SORTLOOP			(1)	/* this is the loop id of gp_count variable, _do_not_change_it_ */
#define GENRE_SORTLOOP			(2)	/* this is the loop id of gp_count variable, _do_not_change_it_ */
#define SONG_SORTLOOP			(3)	/* this is the loop id of gp_count variable, _do_not_change_it_ */
#define YEAR_SORTLOOP			(4)	/* this is the loop id of gp_count variable, _do_not_change_it_ */
#define TRACK_SORTLOOP			(5)	/* this is the loop id of gp_count variable, _do_not_change_it_ */

#define VOID_INDEX		0x00FFFFL

#define LEN_OF_ID3V1_GENRE		(0xF)	/* max len of genre in table, _may_not_have_terminator_ */

#define NumOfLevels	4

#define		ON_THE_GO_TEST			0
#define		ON_THE_GO_ADD			1
#define		ON_THE_GO_DEL			2
#define		ON_THE_GO_DEL_ALL		3

/*==================================================================================================
                                     STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/* if you change this struct define also change #define ON_THE_FLY_STRUCT_SIZE		(NUM_OF_SONG_IN_ON_THE_FLY+1) */
typedef struct {
	/* storage list, it is the list storing songID for NUM_OF_SONG_IN_ON_THE_FLY */
	uint24 store_list[NUM_OF_SONG_IN_ON_THE_FLY];
	uint24 num_of_id;	/* num_of_id _must_ locate at end of store_list position, for flash_function access */
} ON_THE_FLY_LIST_T;

/*==================================================================================================
                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
/* RAM */
extern uint24 _X num_of_item[MAX_NUM_OF_GROUP];
extern uint24 num_of_item_y[MAX_NUM_OF_GROUP];

#ifdef _USE_FS_MUSICLIB_
// Siukoon 2005-02-28
extern long FSFreeSpace(int);
extern _packed BYTE g_strMusicLib_FilePath[];
extern _packed BYTE g_strMusicLib_SecTable_FilePath[];
extern int g_iFileHandle;
#endif

/*================================================================================================*/
/*==================================================================================================
                                        FUNCTION PROTOTYPES
==================================================================================================*/
/*==================================================================================================
FUNCTION: utility macro functions for:
		1) check string terminator
		2) list array operation

DESCRIPTION:
		1) check string terminator
		2) list array operation

ARGUMENTS PASSED:

RETURN VALUE:

PRE-CONDITIONS:
   None

POST-CONDITIONS:
   None

IMPORTANT NOTES:
   None

==================================================================================================*/
#ifdef	_24_bit_word_
//extern uint24 macro_temp_var;
//#define ChkTermInWord(myword) (((myword & 0xFF0000) == 0x00) || ((myword & 0x00FF00) == 0x00) || ((myword & 0x0000FF) == 0x00))

/*
#define IncWordVal(uint24_p, pos, val) macro_temp_var = pos/NUM_OF_ITEM_IN_ONE_WORD; if ((pos & 0x01) == 0x01) { uint24_p[macro_temp_var] = ((uint24_p[macro_temp_var] & 0xFFF000) | ((uint24_p[macro_temp_var] & 0x000FFF) + (val & 0x000FFF))); } else { uint24_p[macro_temp_var] = (((((uint24_p[macro_temp_var] & 0xFFF000)>>12) + (val & 0x000FFF))<<12) | (uint24_p[macro_temp_var] & 0x000FFF)); }
#define	get_val(uint24_p, pos, val) val = uint24_p[(macro_temp_var = pos/NUM_OF_ITEM_IN_ONE_WORD)]; if ((pos & 0x01) == 0x01) { val = (val & 0x000FFF); } else { val = ((val & 0xFFF000)>>12); }
#define	put_val(uint24_p, pos, val) macro_temp_var = pos/NUM_OF_ITEM_IN_ONE_WORD; if ((pos & 0x01) == 0x01) { uint24_p[macro_temp_var] = ((uint24_p[macro_temp_var] & 0xFFF000) | (val & 0x000FFF)); } else { uint24_p[macro_temp_var] = (((val & 0x000FFF)<<12) | (uint24_p[macro_temp_var] & 0x000FFF)); }
*/
#endif

#ifdef BROWSE_PLAY_MODULE
_reentrant uint24 ML_strhead_pos_BROWSE(uint24 unicode, const uint24 _X *scr);
_reentrant uint24 ML_extract_byte_BROWSE(uint24 unicode, const uint24 _X *scr, uint24 start_pos);
_reentrant void ML_list_get_val_X_Browse(uint24 _X *uint24_p, uint24 pos, uint24 *val);
#else
#ifndef _PC_SIMULATION_
COMP_OPT uint24 ML_strhead_pos(uint24 unicode, const uint24 _X *scr);
COMP_OPT uint24 ML_extract_byte(uint24 unicode, const uint24 _X *scr, uint24 start_pos);
COMP_OPT uint24 ML_extract_string(uint24 unicode, uint24 *tgr, const uint24 _X *scr, uint24 cpysize);
#endif
#endif
/*================================================================================================*/
#ifdef __cplusplus
}
#endif
#endif  /* MUSICLIB_HDR_H */
