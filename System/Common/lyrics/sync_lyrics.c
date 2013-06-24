///////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2004-2006
//
// Filename: sync_lyrics.c
// Description: Sync. Lyrics
///////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Includes
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "exec.h"
#include "stringlib.h"
#include "hwequ.h"
#include "project.h"
#include "FileSystem.h"
#include "synclyrics.h"
#include "resource.h"
#include "menumanager.h"
#ifdef USE_PLAYLIST1
#include "playlist.h"
#include "playlist1internal.h"
#else
#ifdef USE_PLAYLIST2
#include "playlist2.h"
#include "playlist2internal.h"
#else
#ifdef USE_PLAYLIST3
#include "playlist3.h"
#else
#ifdef USE_PLAYLIST5
#include "playlist.h"
#include "playlist5.h"
#endif
#endif
#endif
#endif

////////////////////////////////////////////////////////////////////////////////
//  X Memory
////////////////////////////////////////////////////////////////////////////////
_X WORD g_wID3V2_Core_CodeBank = RSRC_ID3V2_LYRICS_CODEBANK;
_X WORD g_wLYRICS3_Core_CodeBank = RSRC_LYRICS3_CODEBANK;
_X WORD g_wLRC_Core_CodeBank = RSRC_LRC_CODEBANK;
_X WORD g_wWMA_Core_CodeBank = RSRC_WMA_LYRICS_CODEBANK;

////////////////////////////////////////////////////////////////////////////////
//  P Memory
////////////////////////////////////////////////////////////////////////////////
void _reentrant LyricsSetTimer(WORD wPeriod)
{
	SysPostMessage(6, SOFT_TIMER_SET_TIMER, TIMER_LYRICS_UPDATE, 1, wPeriod, MENU_MSG_LYRICS_UPDATE);
}

void _reentrant LyricsKillTimer(void)
{
	SysPostMessage(3, SOFT_TIMER_KILL_TIMER, TIMER_LYRICS_UPDATE);
}

RETCODE _reentrant LyricsGetLFN(WORD *pName, INT iBufLength)
{
#ifdef USE_PLAYLIST1
	return (SysCallFunction(RSRC_PLAYLIST_CODEBANK, Playlist_LFNGetFileName, g_iInternalTrack, 0, (void *)pName));
#else
#ifdef USE_PLAYLIST2
	return (SysCallFunction(RSRC_PLAYLIST_CODEBANK, Playlist_GetLFN, TYPE_FILE, (INT)g_CurrentTrack.pFileEntry,(INT *)pName));
#else
#ifdef USE_PLAYLIST3
	return (SysCallFunction(RSRC_PLAYLIST_CODEBANK, Playlist_GetLFN, TYPE_FILE, 0,(INT *)pName));
#else
#ifdef USE_PLAYLIST5
	PL5_PL_QITEM * tmp;
	int recordnum;
	LONG fastkey;
	tmp = (g_PL5_Playback.queue)->pos + g_PL5_Playback.currentplay;
	recordnum = tmp->file.iRecordNum;
	fastkey = (tmp->folder + tmp->depth - 1)->record.lFastKey;
	return SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetLFN,(INT)(&fastkey),
                recordnum,(INT*)pName)  ;   // is there a LFN
#else
	return (-1);
#endif
#endif
#endif
#endif
}


//If it is Playlist5, return 1
////If it is not playlist5, return 0
INT _reentrant LRCOpenFile_PL5_YesOrNot(void)
{
#ifdef USE_PLAYLIST5
    return 1;
#else
    return 0;
#endif
}
extern _packed char namebuffer[];	// declared in fsutilsc.c, V1.05

#ifdef FUNCLET
    #pragma asm
       FGet_LRCDirName_PL5:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel

        CallFunclet2 RSRC_FUNCLET_GET_LRC_DIR_NAME_PL5
        org p,"SYSFUNCLET_GET_LRC_DIR_NAME_PL5_P":
        dc      RSRC_FUNCLET_GET_LRC_DIR_NAME_PL5
    #pragma endasm
_reentrant INT Get_LRCDirName_PL5_Funclet(void)
#else
_reentrant INT Get_LRCDirName_PL5(void)
#endif
{
#ifdef USE_PLAYLIST5
    PL5_PL_QITEM * tmp;
    PL5_PL_FOLDER folder;
    _packed BYTE pszBuffer[SFN_LENGTH];
	WORD *LRCFileName = (WORD *)g_wSongTitle;
	WORD *LRCDirName = (WORD *)namebuffer;
	INT i;

	LRCDirName[0]=0;
	tmp = (g_PL5_Playback.queue)->pos + g_PL5_Playback.currentplay;

	for(i = 1; i < tmp->depth; i++)
    {
         if (Playlist_GetSFN(&(tmp->folder[i].record), pszBuffer) != PLAYLIST_SUCCESS)
			return -1;
        packed_strcat((_packed BYTE *)LRCDirName,pszBuffer); // Get the full path
    }
	return 1;
#else
	return 1;
#endif
}

