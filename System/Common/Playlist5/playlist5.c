////////////////////////////////////////////////////////////////////////////////
// Copyright (c) SigmaTel, Inc. 2007
// Filename: playlist5.c
//
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "exec.h"
#include "menumanager.h"
#include "playlist.h"
#include "playerlib.h"
#include "project.h"
#include "stringlib.h"
#include "resource.h"
#include "playlist5.h"
#include "sysmem.h"
#include "fsapi.h"
#include "syscoder.h"
#include "menus.h"

////////////////////////////////////////////////////////////
// Macros
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// extern variables
////////////////////////////////////////////////////////////
extern INT DriveLetter[];
extern int g_RealCurrenNum;

////////////////////////////////////////////////////////////
// prototypes
////////////////////////////////////////////////////////////
static _reentrant void PL5_PL_StartSearch(int direction);

////////////////////////////////////////////////////////////
// glocal variables
////////////////////////////////////////////////////////////
int g_iPlaylistRepeat = PLAYLIST_REPEAT_OFF;
int g_bPlaylistShuffle = FALSE;
int g_bPlaylistShuffleUpdate = FALSE;
int g_iTotalTracks=0; 	//total number of songs in current playset.
int g_iPlaySet=PLAYSET_MUSIC;
int g_RandomOrder[PL5_PL_RAND_BUF_SIZE];
int g_SongIdx = 0;
struct Bookmark	g_MarkerMusic, g_MarkerVoice;

int g_PlayedCnt = 0;
BOOL g_PrevMode = FALSE;
int g_SearchDirc = 0;
int g_RepCnt = 0;
int g_PrevSearchDirc = 0;
BOOL g_bDispRefreshAll = FALSE;

////////////////////////////////////////////////////////////
// funcstions definitions
////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////
//
//>  Name:       INT _reentrant Playlist_SetPlaySet(INT iPlaysetHint, INT iEntry, INT *piExtList)
//
//   Type:         Function
//
//   Description:   Activate a given playset. Returns old playset (PLAYSET_MUSIC or PLAYSET_VOICE)
//                     This determines the tracks which will be played in the current playset.
//
//   Inputs:       INT  iPlayset    The new playset
//                 INT
//
//   Outputs:      INT  old playset
//
//   Notes:        Prepare the first specified media for current track
//
//<
//////////////////////////////////////////////////////
//INT _reentrant Playlist_SetPlaySet(int iPlayset, int b, int *c)
INT _reentrant Playlist_SetPlaySet(INT iPlaysetHint, INT iEntry, INT *piExtList)
//////////////////////////////////////////////////////
{
    union PlaySetHint uPlaySetHint;
    int iOldPlayset = g_iPlaySet;

   uPlaySetHint.I = iPlaysetHint;

    g_iPlaySet = uPlaySetHint.detail.PlaySet;

    //FIXME: codebank call
//    Playlist_SetPlaySetHandle(iPlaysetHint, iEntry, piExtList);

    Playlist_ShuffleList(g_bPlaylistShuffle, 0, 0);	//this will set flags to shuffle or re-order.

    return iOldPlayset;
}

//////////////////////////////////////////////////////
//
//>  Name:         void _reentrant Playlist_ShuffleList(INT bTrueFalse, INT iIgnored, INT *pIgnored)
//
//   Type:           Function
//
//   Description:  Shuffles the play queue
//                      Based on bTrueFalse this function sets appropriate flags to inform the playlist code to shuffle or "unshuffle" the tracks.
//
//   Inputs:        BOOL    bTrueFalse  TRUE to shuffle, FALSE to re-order numerically by track
//
//   Outputs:      none
//
//   Notes:         This will shuffle the play queue or unshuffle the play queue.
//                     The algorithm will walk down each element and pick a random element to
//                     swap with it.
//
//<
//////////////////////////////////////////////////////
INT _reentrant Playlist_ShuffleList(INT bTrueFalse, INT iIgnored, INT *pIgnored)
//////////////////////////////////////////////////////
{
    g_bPlaylistShuffle = bTrueFalse;
    g_bPlaylistShuffleUpdate = TRUE;
	if((g_PrevMode != g_bPlaylistShuffle) && (g_bPlaylistShuffle == TRUE))
	{
		g_PrevSearchDirc = 0;
	}
	g_PrevMode = g_bPlaylistShuffle;

    return PLAYLIST_SUCCESS;
}
void _reentrant Playlist_UpdateShuffle(void)
{

	unsigned long rtc;
	int RandNum, ret, QueDist;
	int TmpCurrenPlay, ModSize;
	int i, j, SongDiff;
	int bNext;
	int PlayedCnt = 0, PlayedAllCnt = 0;
	unsigned int LowRtc;
	int SongCnt;
	int RepCnt;
	int AddCnt = 0;
	int TmpQtail,TmpQhead;
	int OrgCurrentPlay;
	int TmpNum;
	ret = 0;

	if(g_bPlaylistShuffle)
	{
		if(g_SearchDirc == -1)
		{
			bNext = -1;
		}
		else
		{
			bNext = 1;
		}

		// Reset random order.
		if(g_SongIdx == 0)
		{
			if(g_wTotalSongCount > PL5_PL_RANDOM_MOD)
				ModSize = PL5_PL_RANDOM_MOD;
			else if(g_wTotalSongCount >= 2)
				ModSize = g_wTotalSongCount - 1;
			else
				ModSize = 1;

			if(g_wTotalSongCount > PL5_PL_RAND_BUF_SIZE)
				g_RepCnt = PL5_PL_RAND_BUF_SIZE;
			else
				g_RepCnt = g_wTotalSongCount;

			g_RandomOrder[0] = (g_PL5_PL_queue.pos + g_PL5_Playback.currentplay)->tracknum;
			for(i = 1; i < g_RepCnt; i++)
			{
			    rtc = SysGetCurrentTime();
				LowRtc = rtc & 0xffffff;
				RandNum = LowRtc % ModSize;
				if(RandNum == 0)
					RandNum = ModSize;
				j = 0;
				while(j != i)
				{
					TmpNum = (g_RandomOrder[i-1] + RandNum);
					if(TmpNum > g_wTotalSongCount)
						TmpNum -= g_wTotalSongCount;
					for(j = 0; j < i; j++)
					{
						// If this random number has exist in g_RandomOrder,plus randnum by 1 and re-judge.
						if(g_RandomOrder[j] == TmpNum)
						{
							RandNum++;
							if(RandNum > ModSize)
								RandNum = 1;
							break;
						}
					}
				}
				g_RandomOrder[i] = TmpNum;
				if(g_RandomOrder[i] > g_wTotalSongCount)
					g_RandomOrder[i] -= g_wTotalSongCount;
			}
			
		}


		if(bNext == 1)
		{
			g_SongIdx++;
			if(g_SongIdx == g_RepCnt)
				g_SongIdx = 0;
//			if(g_PrevSearchDirc == -1)
//				g_SongIdx++;
//			g_PrevSearchDirc = 1;
			
		    PL5_PL_StartSearch(1);
			while((g_PL5_PL_queue.pos + g_PL5_Playback.currentplay)->tracknum != g_RandomOrder[g_SongIdx])
			{
				if(((g_PL5_Playback.currentplay + 1) % g_PL5_PL_queue.size) != g_PL5_PL_queue.tail)
					g_PL5_Playback.currentplay = (g_PL5_Playback.currentplay + 1) % g_PL5_PL_queue.size;
				else
					SysWaitOnEvent(0,0,50);
			}
		}
		else
		{
			g_SongIdx--;
			if(g_SongIdx < 0 )
				g_SongIdx = g_RepCnt - 1;
//			if(g_PrevSearchDirc == 1)
//				g_SongIdx--;
//			g_PrevSearchDirc = -1;
			
		    PL5_PL_StartSearch(0);
			while((g_PL5_PL_queue.pos + g_PL5_Playback.currentplay)->tracknum != g_RandomOrder[g_SongIdx])
			{
				if(g_PL5_Playback.currentplay != g_PL5_PL_queue.head)
					g_PL5_Playback.currentplay = (g_PL5_Playback.currentplay - 1 + g_PL5_PL_queue.size) % g_PL5_PL_queue.size;
				else
					SysWaitOnEvent(0,0,50);
			}
		}

	}




#if 0
    TraverseTreeParams TraverseParams;
    int iPlayset  = g_iPlaySet;

    if(g_bPlaylistShuffleUpdate)
    {
        g_bPlaylistShuffleUpdate = FALSE;
        if(g_iTotalTracks)
        {
            if(g_bPlaylistShuffle)
            {
                //Shuffle the FileEntryPool here
                ShuffleTrackOrder(0,0,0);
            }
            else
            {
                //lets put the list in order
                TraverseParams.iReason = ORDER_TRACKS;
                TraverseParams.iPlayset = iPlayset;
                TraverseParams.iTrackOrder = 0;
                SysCallFunction(RSRC_PLAYLIST5TRAVERSE_CODEBANK,Playlist_TraverseFromRoot,0,0,(INT*) &TraverseParams);
            }
        }
    }
#endif
    ;
}
//////////////////////////////////////////////////////
//
//>  Name:          void _reentrant Playlist_ResetPlayset(void)
//
//   Type:          Function
//
//   Description:   Shuffles the play queue
//
//   Inputs:        BOOL    bTrueFalse  TRUE to shuffle, FALSE to re-order numerically by track
//
//   Outputs:       none
//
//   Notes:			This resets the m_bCurrentPlayset bit for all the files in FileEntryPool
//<
//////////////////////////////////////////////////////
void _reentrant Playlist_ResetPlayset(void)
{
    return;
}

//////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant Playlist_MarkCurrentSongPlayed(int iIgnored1,int iIgnored2,int *pIgnored)
//
//   Type:          Function
//
//   Description:   required function that marks the current song played
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         This entry point is simply to satisfy the linker.  Some
//                  playlist implementations may need to mark songs played, but ours
//                  does not need to do this.   The 'end of the list' is when we
//                  reach the end of the list, not when all the songs are played.
//<
//////////////////////////////////////////////////////
RETCODE _reentrant Playlist_MarkCurrentSongPlayed(int iIgnored1, int iIgnored2, int *pIgnored)
{
//////////////////////////////////////////////////////
    return PLAYLIST_SUCCESS;
}



//////////////////////////////////////////////////////
//
//>  Name:         RETCODE _reentrant Playlist_GetNextSongFileInfo(INT iMode, INT iUnused, SONGFILEINFO*SongInfo)
//
//   Type:            Function
//
//   Description:   Gets the song file info  (this is a required entry point)
//                       This function advances the playlist to the next song in the current playset.
//
//   Inputs:        SONGFILEINFO* SongInfo     the pointer to the structure passed in by the PlayerLibrary.
//                                                               the PlayerLibrary must initialize some parts of the structure:
//                                                               m_wBufferLength and m_pFilename
//                      iMode                                 next song parameter For e.g. button press, end of song etc.
//
//   Outputs:      PLAYLIST_SUCCESS if SONGFILEINFO * SongInfo can be properly filled out, otherwise
//                     PLAYLIST_END_OF_LIST
//                     RETCODE (PLAYLIST_TRACK_NOT_FOUND or PLAYLIST_END_OF_LIST or PLAYLIST_SUCCESS)
//
//   Notes:
//<
//////////////////////////////////////////////////////
RETCODE _reentrant Playlist_GetNextSongFileInfo(INT iMode, INT iUnused, INT*pPtr)
{
    SONGFILEINFO*SongInfo = (SONGFILEINFO*)pPtr;
    RETCODE rtn = PLAYLIST_SUCCESS;

    if(g_wTotalSongCount)
    {   //if we have any tracks at all
        if((g_iPlaylistRepeat!=PLAYLIST_REPEAT_ONE) ||
                (iMode==NEXTSONG_PLAY_BUTTON) ||
                ((g_iPlaylistRepeat==PLAYLIST_REPEAT_ONE) && g_wCurrentSongBad) ||
                (iMode==NEXTSONG_FFWD))
        {//if we're not in repeat once mode, lets increment.
            //or, force an increment if NEXT button pressed, or FFWD over boundary

        }

		if(!((g_iPlaylistRepeat == PLAYLIST_REPEAT_ONE) && (iMode !=NEXTSONG_PLAY_BUTTON)))
		{
			if(g_bPlaylistShuffle)
			{
//				if(!(g_PL5_Playback.foundall && (g_wTotalSongCount == 1)))
//				{
//			        while (((g_PL5_PlaybackQtail - g_PlaybackCurrentPlay + PL5_PL_QUEUE_SIZE) % PL5_PL_QUEUE_SIZE) <= PL5_PL_RANDOM_MOD)
//			        {
//						if(g_PL5_PL_search.status == PL5_STATE_IDLE)
//							SysPostMessage(2, PL5_PL_SEARCH_FORWARD);
//			            SysWaitOnEvent(0,0,100);
//			        }
//
//					g_SearchDirc = 1;
//					if(g_PrevSearchDirc != g_SearchDirc)
//					{
//						_memset(g_SongPlayed, 0, sizeof(LONG) * PL5_PL_RAND_BUF_SIZE);
//						g_SongPlayedIdx = 0;
//						g_PlayedCnt = 0;
//						if(g_PL5_PlaybackQueue[g_PlaybackCurrentPlay].file.lFastKey != 0)
//						{
//							g_SongPlayed[g_SongPlayedIdx] = g_PL5_PlaybackQueue[g_PlaybackCurrentPlay].file.lFastKey;
//							g_SongPlayedIdx++;
//							g_PlayedCnt++;
//						}
//					}
//					g_PrevSearchDirc = g_SearchDirc;
//				    Playlist_UpdateShuffle();
//				}
				g_SearchDirc = 1;
				Playlist_UpdateShuffle();
			}
			else
			{
#ifdef JPEG_APP				 
					SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_JPEG_DECODER_CALLBACK);
					SysWaitOnEvent(0,0,0);
#endif					
			    PL5_PL_StartSearch(1);
#ifdef JPEG_APP	
				if(g_iPlaySet == PLAYSET_PHOTO)
				{
				   	SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_JPEG_DECODER_CALLBACK,0,TIMER_JPEG_DECODER_TIMEOUT_MS,MENU_MSG_JPEG_CALLBACK);
					SysWaitOnEvent(0,0,0);
				}
#endif					
			}
		}

        //Lets now get the current song file info.  if we get an error condition
        //we'll pass it on, but we won't override an end of list from above
        if(Playlist_GetCurrentSongFileInfo(0,0,(void*)SongInfo)!=PLAYLIST_SUCCESS)
            rtn = PLAYLIST_END_OF_LIST;
    }
    else  // no tracks.
    {
        rtn = PLAYLIST_TRACK_NOT_FOUND;
    }

    return rtn;
}

RETCODE _reentrant Playlist_GetCurrentSongFileInfo(INT iUnused1, INT iUnused2, INT *pPtr)
{
    SONGFILEINFO* SongInfo = (SONGFILEINFO*)pPtr;
    RETCODE rtn = PLAYLIST_TRACK_NOT_FOUND;
    _packed BYTE pszBuffer[SFN_LENGTH];
    int i;
    int depth;
    PL5_FOLDER_RECORD * tmp;


    pszBuffer[0] = 0;
    SongInfo->m_pFilename[0]=0;//null out the filename (so the packed_strncat's will work right)

	g_bDispRefreshAll = TRUE;

	// If playback queue is empty,return PLAYLIST_TRACK_NOT_FOUND;
	if(g_PL5_PL_queue.head == g_PL5_PL_queue.tail)
	{
		return PLAYLIST_TRACK_NOT_FOUND;
	}

    depth = (g_PL5_PL_queue.pos + g_PL5_Playback.currentplay)->depth;
    for(i=1; i<depth; i++)
    {
        tmp = &((g_PL5_PL_queue.pos + g_PL5_Playback.currentplay)->folder[i].record);
        if (Playlist_GetSFN(tmp, pszBuffer) != PLAYLIST_SUCCESS)
            __asm("debug");
        packed_strcat(SongInfo->m_pFilename,pszBuffer);
    }

    tmp = &((g_PL5_PL_queue.pos + g_PL5_Playback.currentplay)->file);
    if(Playlist_GetSFN(tmp, pszBuffer)== PLAYLIST_SUCCESS)
    {
        packed_strcat(SongInfo->m_pFilename, pszBuffer);
        //SongInfo->m_wTrack = (WORD) pTrackInfo->iTrackNum;
        //SongInfo->m_wDeviceID = pTrackInfo->tFolderRecord[i].iDevice;
    }
    else
        __asm("debug");

	// The tracknum may be less than 0 during previous searching,reset tracknum if foundall be true.
	if((((g_PL5_PL_queue.pos + g_PL5_Playback.currentplay)->tracknum) <= 0) && g_PL5_Playback.foundall)
		(g_PL5_PL_queue.pos + g_PL5_Playback.currentplay)->tracknum += g_PL5_Playback.total;
	
    g_wCurrentSongNumber = (g_PL5_PL_queue.pos + g_PL5_Playback.currentplay)->tracknum;

    return PLAYLIST_SUCCESS;
}


//////////////////////////////////////////////////////
//
//>  Name:         RETCODE _reentrant Playlist_GetPreviousSongFileInfo(int,int,SONGFILEINFO*SongInfo)
//
//   Type:            Function
//
//   Description:   Gets the song file info  (this is a required entry point)
//                       This function advances the playlist to the previous song in the current playet.
//
//   Inputs:         SONGFILEINFO *      SongInfo        the pointer to the structure passed in by the PlayerLibrary.
//                                                      the PlayerLibrary must initialize some parts of the structure:
//                                                      m_wBufferLength and m_pFilename
//
//   Outputs:      PLAYLIST_SUCCESS if SONGFILEINFO * SongInfo can be properly filled out, otherwise
//                     PLAYLIST_END_OF_LIST
//                     RETCODE (PLAYLIST_TRACK_NOT_FOUND or PLAYLIST_END_OF_LIST or PLAYLIST_SUCCESS)
//
//   Notes:
//<
//////////////////////////////////////////////////////
RETCODE _reentrant Playlist_GetPreviousSongFileInfo(INT iUnused1, INT iUnused2, INT *pPtr)
{
    SONGFILEINFO*SongInfo = (SONGFILEINFO*)pPtr;
    RETCODE rtn = PLAYLIST_END_OF_LIST;

//    if(g_wTotalSongCount)
//    {
//
//
//		if(g_bPlaylistShuffle)
//		{
//			if(!(g_PL5_Playback_Search_FileCnt.foundall && (g_wTotalSongCount == 1)))
//			{
//		        while (((g_PlaybackCurrentPlay - g_PL5_PlaybackQhead + PL5_PL_QUEUE_SIZE) % PL5_PL_QUEUE_SIZE) < PL5_PL_RANDOM_MOD)
//		        {
//					if(g_PL5_PL_search.status == PL5_STATE_IDLE)
//						SysPostMessage(2, PL5_PL_SEARCH_BACKWARD);
//		            SysWaitOnEvent(0,0,50);
//		        }
//				// Get random file.
//				g_SearchDirc = -1;
//				if(g_PrevSearchDirc != g_SearchDirc)
//				{
//					_memset(g_SongPlayed, 0, sizeof(LONG) * PL5_PL_RAND_BUF_SIZE);
//					g_SongPlayedIdx = 0;
//					g_PlayedCnt = 0;
//					if(g_PL5_PlaybackQueue[g_PlaybackCurrentPlay].file.lFastKey != 0)
//					{
//						g_SongPlayed[g_SongPlayedIdx] = g_PL5_PlaybackQueue[g_PlaybackCurrentPlay].file.lFastKey;
//						g_SongPlayedIdx++;
//						g_PlayedCnt++;
//					}
//				}
//				g_PrevSearchDirc = g_SearchDirc;
//			    Playlist_UpdateShuffle();
//			}
//
//            // post a message to playback background search
////            SysPostMessage(2, PL5_PL_SEARCH_BACKWARD);
//		}
//		else
//		{
//	        if (g_PlaybackCurrentPlay == g_PL5_PlaybackQhead)
//	        {
//	            // post a message to playback background search
//	            SysPostMessage(2, PL5_PL_SEARCH_BACKWARD);
//	        }
//
//	        while (g_PlaybackCurrentPlay == g_PL5_PlaybackQhead)
//	        {
//	            SysWaitOnEvent(0,0,100);
//	        }
//
//	        // there is a previous one in playback queue
//	        g_PlaybackCurrentPlay = (g_PlaybackCurrentPlay+PL5_PL_QUEUE_SIZE-1)%PL5_PL_QUEUE_SIZE;
//			g_RealCurrenNum--;
//			if(g_PL5_Playback_Search_FileCnt.foundall && g_RealCurrenNum < 0)
//			{
//				while(g_RealCurrenNum < 0)
//					g_RealCurrenNum += g_wTotalSongCount;
//			}
//
//			g_wCurrentSongNumber = g_RealCurrenNum;
//		}
//
//        // TODO: update g_wCurrentSongNumber according to playmode
//
//        rtn = Playlist_GetCurrentSongFileInfo(0,0,(void*)SongInfo);
//    }
//    else  // no tracks.
//    {
//        rtn = PLAYLIST_TRACK_NOT_FOUND;
//    }

    if(g_PL5_PL_queue.head != g_PL5_PL_queue.tail)	
    {   //if we have any tracks at all

//		if(!(g_iPlaylistRepeat == PLAYLIST_REPEAT_ONE))
		{
			if(g_bPlaylistShuffle)
			{
				g_SearchDirc = -1;
				Playlist_UpdateShuffle();
			}
			else
			{
#ifdef JPEG_APP				 
					SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_JPEG_DECODER_CALLBACK);
					SysWaitOnEvent(0,0,0);
#endif					
			    PL5_PL_StartSearch(0);
#ifdef JPEG_APP
				if(g_iPlaySet == PLAYSET_PHOTO)
				{
				   	SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_JPEG_DECODER_CALLBACK,0,TIMER_JPEG_DECODER_TIMEOUT_MS,MENU_MSG_JPEG_CALLBACK);
					SysWaitOnEvent(0,0,0);
				}
#endif					
			}
		}

        //Lets now get the current song file info.  if we get an error condition
        //we'll pass it on, but we won't override an end of list from above
        rtn = Playlist_GetCurrentSongFileInfo(0,0,(void*)SongInfo);
    }
    else  // no tracks.
    {
        rtn = PLAYLIST_TRACK_NOT_FOUND;
    }

    return rtn;

}

//////////////////////////////////////////////////////
//
//>  Name:          _reentrant RETCODE Playlist_GetLFN(INT iEntryType,INT b,INT* pName)
//
//   Type:            Function
//
//   Description:   This uses the FastKey of the parent directory and the record number of pEntry to extract the LFN.
//
//   Inputs:          Entry Type - TYPE_FILE or TYPE_DIR
//
//   Outputs:        The long file name in pName.
//
//   Notes:
//<
//////////////////////////////////////////////////////
_reentrant RETCODE Playlist_GetLFN(INT pFastKey,int RecordNum,INT* pName)
{
	INT iDirHandle;
	RETCODE rtn = PLAYLIST_TRACK_NOT_FOUND;
	LONG fastkey = *((LONG*)pFastKey);

	if((iDirHandle = FastOpen(fastkey,(_packed char*)"d")) >= 0)
	{
		if(ConstructLongFileName(iDirHandle, RecordNum, (INT*) pName) > 0)
			rtn = PLAYLIST_SUCCESS;
        Fclose(iDirHandle);
	}

	return rtn;
}

//////////////////////////////////////////////////////
//
//>  Name:         RETCODE _reentrant Playlist_Initialize(int a, int b, int *c)
//
//   Type:            Function
//
//   Description:   This function is called at power up and any time it necessary to rebuild the media contents.
//                       In playlist5, we just show the root directory after power up
//
//   Inputs:
//
//   Outputs:       PLAYLIST_SUCCESS
//
//   Notes:          This should ideally be in playlist5init.c. But, leaving it here
//					since other codebanks call this using RSRC_PLAYLIST_CODEBANK
//<
//////////////////////////////////////////////////////

RETCODE _reentrant Playlist_Initialize(INT a, INT b, INT *c)
{

    SysCallFunction(RSRC_PLAYLIST2INIT_CODEBANK,Playlist_InitRoot, a, b, c);

    return PLAYLIST_SUCCESS;
}

/*----------------------------------------------------------------------------

   Function Name:  static _reentrant void PL5_PL_StartSearch(int direction)

   FunctionType:   Reentrant

   Inputs:        1) search direction. 1: forward;  0: backward

   Outputs:       None

   Description:  Search previous folder

----------------------------------------------------------------------------*/
static _reentrant void PL5_PL_StartSearch(int direction)
{
    PL5_FOLDER_RECORD * tmp_rec;
    int head, tail;
    int depth, i;
    BOOL foregnd = FALSE;
    _packed BYTE pszBuffer[SFN_LENGTH];

	// Wait until last search message completed.
	while((g_PL5_PL_search.status == PL5_STATE_RUN) && (g_PL5_PL_search.phase != PL5_COUNT))
		SysWaitOnEvent(0,0,50);

    head = g_PL5_PL_queue.head;
    tail = (g_PL5_PL_queue.tail + g_PL5_PL_queue.size - 1) % g_PL5_PL_queue.size;

    if (direction == 1)
    {   // search forward
        depth = (g_PL5_PL_queue.pos + tail)->depth;

        g_PL5_PL_search.phase = PL5_NEXT_FILE;

        g_PL5_Playback.currentplay = (g_PL5_Playback.currentplay + 1) % g_PL5_PL_queue.size;

        if (g_PL5_Playback.currentplay == g_PL5_PL_queue.tail)
        {
            // there is no next file in playback queue.
            // Set foreground search for quick response and it will be background after search complete.
            g_PL5_PL_search.foregnd = TRUE;
        }
        _memcpy(g_PL5_PL_search.tPath, (g_PL5_PL_queue.pos + tail)->folder, sizeof(PL5_PL_FOLDER) * depth);
        g_PL5_PL_search.sPath[0] = 0;
        for(i=1; i<depth; i++)
        {
            tmp_rec = &((g_PL5_PL_queue.pos + tail)->folder[i].record);
            if (Playlist_GetSFN(tmp_rec, pszBuffer) != PLAYLIST_SUCCESS)
                __asm("debug");
            packed_strcat(g_PL5_PL_search.sPath,pszBuffer);
        }
        g_PL5_PL_search.depth = depth;
        g_PL5_PL_search.start = (g_PL5_PL_queue.pos + tail)->file.iRecordNum + 1;
        g_PL5_PL_search.tracknum = (g_PL5_PL_queue.pos + tail)->tracknum;
    }
    else
    {   // search backward
        depth = (g_PL5_PL_queue.pos + head)->depth;

        g_PL5_PL_search.phase = PL5_PREV_FILE;
		
		
        if (g_PL5_Playback.currentplay == head)
        {
            // there is no previous file in playback queue.
            // Set foreground search for quick response and it will be background after search complete.
            g_PL5_PL_search.foregnd = TRUE;
        }
        g_PL5_Playback.currentplay = (g_PL5_Playback.currentplay - 1 + g_PL5_PL_queue.size) % g_PL5_PL_queue.size;
		
        _memcpy(g_PL5_PL_search.tPath, (g_PL5_PL_queue.pos + head)->folder, sizeof(PL5_PL_FOLDER) * depth);
        g_PL5_PL_search.sPath[0] = 0;
        for(i=1; i<depth; i++)
        {
            tmp_rec = &((g_PL5_PL_queue.pos + head)->folder[i].record);
            if (Playlist_GetSFN(tmp_rec, pszBuffer) != PLAYLIST_SUCCESS)
                __asm("debug");
            packed_strcat(g_PL5_PL_search.sPath,pszBuffer);
        }
        g_PL5_PL_search.depth = depth;
        g_PL5_PL_search.start = (g_PL5_PL_queue.pos + head)->file.iRecordNum;
        g_PL5_PL_search.tracknum = (g_PL5_PL_queue.pos + head)->tracknum;
    }

    if (g_PL5_PL_search.foregnd)
    {
        // Disable softtimers to prevent message queue overflow.
        foregnd = TRUE;
#ifdef JPEG_APP
        if (g_iPlaySet == PLAYSET_PHOTO)
        {
    	    SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_JPEG_DECODER_CALLBACK);
	    }
	    else
#endif
            SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_ANIMATE);
        SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_BATT_CHK);
        SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_AUTO_SHUTDOWN);
        SysWaitOnEvent(0,0,0);

    }

    SysPostMessage(2, PL5_PL_MSG_START);
    SysWaitOnEvent(0,0,0);

    if (foregnd)
    {
        // Resume softtimers status
#ifdef JPEG_APP
        if (g_iPlaySet == PLAYSET_PHOTO)
        {
		    SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_JPEG_DECODER_CALLBACK,0,TIMER_JPEG_DECODER_TIMEOUT_MS,MENU_MSG_JPEG_CALLBACK);
            SysWaitOnEvent(0,0,0);
        }
		else
#endif
		{
    	    SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_ANIMATE,0,ANIMATE_PERIOD,MENU_MSG_ANIMATE);
	    	SysWaitOnEvent(0,0,0);
        }
        SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_BATT_CHK,0,2000,MENU_BATTERY_CHNG);
        SysWaitOnEvent(0,0,10);
        // button pressed, so restart timer if enabled
        UpdateAutoShutdownTimer();
    }
}
