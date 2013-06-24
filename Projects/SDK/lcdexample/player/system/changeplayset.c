////////////////////////////////////////////////////////////////////////////////
// CopyRight (C) SigmaTel, Inc. 2002-2003
// Filename: ChangePlaySet.c (LCD player)
//
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "exec.h"
#include "menumanager.h"
#include "syscoder.h"
#include "sysmem.h"
#include "playerlib.h"
#ifdef   USE_PLAYLIST1
#include "playlist1.h"
#else
#ifdef   USE_PLAYLIST2
#include  "playlist2.h"
#else
#ifdef   USE_PLAYLIST3
#include  "playlist3.h"
#else
#ifdef   USE_PLAYLIST5
#include  "playlist5.h"
#endif
#endif
#endif
#endif

#include "resource.h"

#include "playmodemenu.h"
#include "menus.h"

#ifdef FUNCLET
 #pragma asm
    extern SysCallFunclet
 #pragma endasm
#endif

INT g_iPlayModeToRestore = PM_NORMAL;

static void do_nothing(void) {      // This function is here to make the dang FUNCLET, below, work right.
                                    // Without this here, the compiler switches back to ".ptext..." as soon
                                    // as it hits some code.  With this here, the "org p,"SYSFUNCLET_..._P"
                                    // stays in effect.
                                    // The function is declared "static" so that it's invisible outside this file.
  return;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant void ChangePlaySet(INT mode)
//
//   Type:          Function -- System memory so all menus can use without copying
//
//   Description:   This function changes the playset if different from current and
//				    lets parser module run, so will load the music/voice parser.
//                  Also conditionally saves or restores the playmode per playset.
//
//   Inputs:        playset to use
//
//   Outputs:       Selection to highlight upon exiting settings sub menu
//                      if MENU_EXIT: return to main menu selection
//						else return to settings menu with returned menu highlighted
//
//   Notes:         Return value from sub menu must be supported menu item in settingsmenu
//
//<
//////////////////////////////////////////////////////////////////////////////////////////
#ifdef FUNCLET
    #pragma asm
        global FChangePlaySet
        FChangePlaySet:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_CHANGE_PLAYSET

        org p,"SYSFUNCLET_ChangePlaySet_P":
        dc      RSRC_FUNCLET_CHANGE_PLAYSET
    #pragma endasm

_reentrant void ChangePlaySet_Funclet (INT iPlayset)
{
#else
_reentrant void ChangePlaySet(INT iPlayset)
{
#endif
#ifndef USE_PLAYLIST3
    int iCurrentPlayset;
    extern int g_iPlaySet;
	iCurrentPlayset = SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetPlaySet,0,0,0);
	if(iPlayset != iCurrentPlayset)	//if truly changing the playset
	{
		// always stop even if same mode, adpcm decoder could be playing when need to switch to encoder
        SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,PlayerLib_SetState,DECODER_STATE_STOP,TRUE,0);

        //~~~~~~~~~~~~~~~~~ Issue #3695: save & restore playmode with specific playset changes
        if( iCurrentPlayset == PLAYSET_MUSIC )  // if leaving music playset,
        {
            g_iPlayModeToRestore = g_iPlayModeSetting; //save playmode to restore later.

            if( iPlayset == PLAYSET_VOICE ) // if entering voice set,
            {   if( g_iPlayModeSetting > LAST_VOICE_PM ) // ensure voiceset compatible playmode
                {   g_iPlayModeSetting = PM_NORMAL;
                    SysCallFunction(RSRC_PLAYMODE_MENU_CODE_BANK,SendPlayMode,g_iPlayModeSetting,0,0);
                }
            }
        }
        else // not leaving music playset
        {   //if entering music playset and playmode change needed, restore playmode.
            if(  (iPlayset == PLAYSET_MUSIC) && (g_iPlayModeToRestore != g_iPlayModeSetting) )
            {
                g_iPlaySet = iPlayset;// Must update playset before SendPlayMode call.
                g_iPlayModeSetting = g_iPlayModeToRestore;  // restore it.  Notify decoder
                SysCallFunction(RSRC_PLAYMODE_MENU_CODE_BANK,SendPlayMode,g_iPlayModeSetting,0,0);
            }
        }//~~~~~~~~~~~~~~~~~

        SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_SetPlaySet,iPlayset,0,0);
		//now go to bookmarked file for new PLAYSET. Playerlib_GetBookmarkSongInfo will call GetCurrentSongInfo
        SysCallFunction(RSRC_PLAYERLIB_EXTRA_CODE_BANK,PlayerLib_GetBookmarkSongInfo,iPlayset,0,0);
	}
#endif
}
#ifdef FUNCLET
    #pragma asm
        ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
        org p,".ptextchangeplayset":
    #pragma endasm
#endif
