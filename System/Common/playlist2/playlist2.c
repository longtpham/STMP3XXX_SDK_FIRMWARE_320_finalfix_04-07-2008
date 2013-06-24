////////////////////////////////////////////////////////////////////////////////
// Copyright (c) SigmaTel, Inc. 2003
// Filename: playlist2.c
//
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "menumanager.h"
#include "playlist.h"       //required include file
#include "playerlib.h"
#include "project.h"
#include "stringlib.h"
#include "resource.h"
#include "sysspeed.h"
#include "playlist2.h"
#include "playlist2internal.h"
#include "sysmem.h"
#include "FileSystem.h"
#include "fserror.h"
#include "FileSpec.h"
#include "playlist.h"
#include "syscoder.h"

///////////////Extern FAT32 functions/////////////////////////////////
extern _reentrant INT Chdir(_packed char *filepath);
extern _reentrant INT FindFirst(Finddata *_finddata,_packed char *FileName);
extern _reentrant INT FindNext(INT HandleNumber,Finddata *_finddata);
extern _reentrant INT Fclose(INT HandleNumber);
extern _reentrant INT GetShortfilename(LONG Key,INT *Buffer);
extern _reentrant LONG GetRootdirkey(INT DeviceNumber);
extern _reentrant INT FastOpen(LONG Key,_packed char *mode);
extern _reentrant INT ConstructLongFileName(INT HandleNumber, INT RecordNumber, INT *LFNBuffer);
extern INT DriveLetter[];
//////////////////////////////////////////////////////////////////////
int g_iPlaylistRepeat = PLAYLIST_REPEAT_OFF;
int g_bPlaylistShuffle = FALSE;
int g_bPlaylistShuffleUpdate = FALSE;
int g_iTotalTracks=0; 	//total number of songs in current playset.
int g_iPlaySet=PLAYSET_MUSIC;
DWORD dStart, dEnd, dDiff;	//for timing measurements
//////////////////////////////////////////////////////
//
//>  Name:          INT _reentrant Playlist_SetPlaySet(INT iPlayset, INT unused, INT *pUnused)
//
//   Type:          Function
//
//   Description:   Activate a given playset. Returns old playset (PLAYSET_MUSIC or PLAYSET_VOICE)
//
//   Inputs:        INT     iPlayset    The new playset
//					INT		bResetBookmark - TRUE when stop is pressed twice to reset the bookmark
//
//   Outputs:       INT                 old playset
//
//   Notes:         This selects and orders tracks in FileEntryPool, and shuffles it 
//                  based on the current shuffle & playset settings.
//<
//////////////////////////////////////////////////////
INT _reentrant Playlist_SetPlaySet(INT iPlayset, INT bResetBookmark, INT *pUnused)
//////////////////////////////////////////////////////
{
	_packed BYTE pszFullFilename[MAX_FILENAME_LENGTH];
	TraverseTreeParams TraverseParams;
    int iOldPlayset = g_iPlaySet;
    pUnused;
    g_iPlaySet = iPlayset;//PLAYSET_MUSIC or PLAYSET_VOICE.
	g_iTotalTracks = 0;
	
	Playlist_ResetPlayset();
	TraverseParams.iReason = SELECT_TRACKS;
	TraverseParams.iPlayset = iPlayset;
	SysCallFunction(RSRC_PLAYLIST2TRAVERSE_CODEBANK,Playlist_TraverseFromRoot,0,0,(INT*) &TraverseParams);

	TraverseParams.iReason = ORDER_TRACKS;
	TraverseParams.iPlayset = iPlayset;
	TraverseParams.iTrackOrder = 0;
	SysCallFunction(RSRC_PLAYLIST2TRAVERSE_CODEBANK,Playlist_TraverseFromRoot,0,0,(INT*) &TraverseParams);

    Playlist_ShuffleList(g_bPlaylistShuffle,0,0);	//this will set flags to shuffle or re-order.

	if(g_iTotalTracks){
#ifndef USE_PLAYLIST3
		g_wTotalSongCount = (WORD)g_iTotalTracks;	
#endif
		SysCallFunction(RSRC_PLAYLIST2TRAVERSE_CODEBANK,Playlist_RestoreBookmark,bResetBookmark,0,0);
	}
	else{
#ifndef USE_PLAYLIST3
		g_wTotalSongCount = 0;
#endif
		g_CurrentTrack.iTrackOrder=0;
	}
	g_CurrentTrack.EntryType = TYPE_FILE;
	return iOldPlayset;
}

//////////////////////////////////////////////////////
//
//>  Name:          void _reentrant Playlist_ShuffleList(INT bTrueFalse, INT iIgnored, INT *pIgnored)
//
//   Type:          Function
//
//   Description:   Shuffles the play queue
//
//   Inputs:        BOOL    bTrueFalse  TRUE to shuffle, FALSE to re-order numerically by track
//
//   Outputs:       none
//
//   Notes:         This will shuffle the play queue or unshuffle the play queue.
//                  The algorithm will walk down each element and pick a random element to
//                  swap with it.
//<
//////////////////////////////////////////////////////
INT _reentrant Playlist_ShuffleList(INT bTrueFalse, INT iIgnored, INT *pIgnored)
//////////////////////////////////////////////////////
{
	g_bPlaylistShuffle = bTrueFalse;
    g_bPlaylistShuffleUpdate = TRUE;
	return PLAYLIST_SUCCESS;
}
void _reentrant Playlist_UpdateShuffle(void)
{
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
				SysCallFunction(RSRC_PLAYLIST2TRAVERSE_CODEBANK,Playlist_TraverseFromRoot,0,0,(INT*) &TraverseParams);
            }
			g_CurrentTrack.iTrackOrder = g_CurrentTrack.pFileEntry->m_iTrackOrder;
        }
    }
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
void _reentrant Playlist_ResetPlayset(){
	int i;
	for(i=0;i<MAX_NUM_FILES;i++)
		if(g_FileEntryPool[i].m_bAllocated)
			g_FileEntryPool[i].m_bCurrentPlayset = FALSE;
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
//>  Name:          RETCODE _reentrant Playlist_BuildFileLinks(FileEntry* pFileEntry,TraverseTreeParams* TraverseParams)
//
//   Type:          Function
//
//   Description:   This function encapsulates what is necessary to populate TraverseParams with information necessary to
//					build the complete file path.
//
//
//   Inputs:        TraverseTreeParams * TraverseParams - Pointer passed by GetCurrentSongInfo (or any other function requiring filepath information)
//							
//                  FileEntry*   pFileEntry - The pointer to the file entry for which the information is requested
//
//   Outputs:       A properly filled out TraverseParams structure.
//					PLAYLIST_SUCCESS if FileEntry is located correctly. PLAYLIST_TRACK_NOT_FOUND if FileEntry is not located or invalid.
//
//   Notes:         The TraverseParams returned will contain the directory links which lie between the pFileEntry 
//					and the ROOT. TrackNum is also filled while traversing, which may or may not be used by the callee.
//                  In order to get the directory links, we traverse from ROOT to detect where the FileEntry is located.
//					Then the DirLinks and iTotalLinks information is filled.
//					DirLinks will contain *only* the links which fall between the ROOT and the FileEntry. It will not contain the 
//					ROOT as it can be obtained by the m_iDevice field in the FileEntry.
//
//<
//////////////////////////////////////////////////////
RETCODE _reentrant Playlist_BuildFileLinks(FileEntry* pFileEntry,TraverseTreeParams* TraverseParams){
	int i,j;
	RETCODE rtn = PLAYLIST_TRACK_NOT_FOUND;
	if(pFileEntry && TraverseParams)
	{
		TraverseParams->iReason = BUILD_FILE_LINKS;
		TraverseParams->iTrackNum = 0;
		for(i=0;i<_MAX_DIR_DEPTH;i++)
			TraverseParams->DirLinks[i] = 0;
		TraverseParams->iTotalLinks = 0;
		TraverseParams->pFileEntry = pFileEntry;	//FileEntry to locate and build
		if(SysCallFunction(RSRC_PLAYLIST2TRAVERSE_CODEBANK,Playlist_TraverseFromRoot,0,0,(INT*) TraverseParams) == PLAYLIST_END_TRAVERSAL_SUCCESS)
		{
			rtn = PLAYLIST_SUCCESS;
		}
	}
	return rtn;
}

#ifdef PLAYLIST2_MULTIMEDIA
//////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant Playlist_GetNextSongFileInfo(INT iMode, INT iUnused, SONGFILEINFO*SongInfo)
//
//   Type:          Function
//
//   Description:   Gets the song file info  (this is a required entry point)
//
//   Inputs:        SONGFILEINFO *      SongInfo        the pointer to the structure passed in by the PlayerLibrary.
//                                                      the PlayerLibrary must initialize some parts of the structure:
//                                                      m_wBufferLength and m_pFilename        
//                  INT                 iRepeat         Repeat time for this operation. If it is zero, only advance one song  
//
//   Outputs:       PLAYLIST_SUCCESS if SONGFILEINFO * SongInfo can be properly filled out, otherwise
//                  PLAYLIST_END_OF_LIST        
//
//   Notes:         
//<
//////////////////////////////////////////////////////
RETCODE _reentrant Playlist_GetNextSongFileInfo(INT iMode, INT iRepeat, INT*pPtr)
#else
//////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant Playlist_GetNextSongFileInfo(INT iMode, INT iUnused, SONGFILEINFO*SongInfo)
//
//   Type:          Function
//
//   Description:   Gets the song file info  (this is a required entry point)
//
//   Inputs:        SONGFILEINFO *      SongInfo        the pointer to the structure passed in by the PlayerLibrary.
//                                                      the PlayerLibrary must initialize some parts of the structure:
//                                                      m_wBufferLength and m_pFilename        
//
//   Outputs:       PLAYLIST_SUCCESS if SONGFILEINFO * SongInfo can be properly filled out, otherwise
//                  PLAYLIST_END_OF_LIST        
//
//   Notes:         
//<
//////////////////////////////////////////////////////
RETCODE _reentrant Playlist_GetNextSongFileInfo(INT iMode, INT iUnused, INT*pPtr)
#endif
{
    SONGFILEINFO*SongInfo = (SONGFILEINFO*)pPtr;
    RETCODE rtn = PLAYLIST_END_OF_LIST;

    Playlist_UpdateShuffle();
    if(g_iTotalTracks)
    {//if we have any tracks at all
#ifdef PLAYLIST2_MULTIMEDIA
		while( iRepeat >= 0 )
		{
			iRepeat--;
#endif			
        if((g_iPlaylistRepeat!=PLAYLIST_REPEAT_ONE)||
           (iMode==NEXTSONG_PLAY_BUTTON)||
           ((g_iPlaylistRepeat==PLAYLIST_REPEAT_ONE) && g_wCurrentSongBad)||
           (iMode==NEXTSONG_FFWD))
        {//if we're not in repeat once mode, lets increment.
         //or, force an increment if NEXT button pressed, or FFWD over boundary
            g_CurrentTrack.iTrackOrder++;
        }
        if(g_CurrentTrack.iTrackOrder > g_iTotalTracks)
        {//if we've hit the end of the list, start back over at the beginning
            g_CurrentTrack.iTrackOrder = 1;
			if((g_iPlaylistRepeat==PLAYLIST_REPEAT_ALL)||
			  (iMode==NEXTSONG_PLAY_BUTTON)||
			   (iMode==NEXTSONG_FFWD))
				rtn = PLAYLIST_SUCCESS;//if we're in repeat all mode, lets return success
                                       //so the player library will keep playing.
                                       // Also, return success if decoder was playing and NEXT was pressed
                                       // or we FFWD'd over a song boundary.
        }
        else
        {//we're in the middle of the list, lets return success
            rtn = PLAYLIST_SUCCESS;
        }
#ifdef PLAYLIST2_MULTIMEDIA
		}
#endif		

        //Lets now get the current song file info.  if we get an error condition
        //we'll pass it on, but we won't override an end of list from above
        if(Playlist_GetCurrentSongFileInfo(0,0,(void*)SongInfo)!=PLAYLIST_SUCCESS) 
            rtn = PLAYLIST_END_OF_LIST;
    } else  // no tracks.
    {
      rtn = PLAYLIST_TRACK_NOT_FOUND;
    }

    return rtn;

}

#ifdef PLAYLIST2_MULTIMEDIA
//////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant Playlist_GetCurrentSongFileInfo(int,int,SONGFILEINFO*SongInfo)
//
//   Type:          Function
//
//   Description:   Gets the song file info  (this is a required entry point)
//
//   Inputs:        INT					iOffset         the offset track order of the desired song from the current song
//					SONGFILEINFO *      SongInfo        the pointer to the structure passed in by the PlayerLibrary.
//                                                      the PlayerLibrary must initialize some parts of the structure:
//                                                      m_wBufferLength and m_pFilename        
//
//   Outputs:       PLAYLIST_SUCCESS if SONGFILEINFO * SongInfo can be properly filled out, otherwise
//                  PLAYLIST_TRACK_NOT_FOUND        
//
//   Notes:         This fills the information for the current track. 
//					First the Directory links between the current track and ROOT are determined by calling PopulateSongFileInfo
//					Next, A complete file path is built by concatenating ROOT + DirLinks returned by PopulateSongFileInfo + File Entry in question.
//					g_CurrentTrack structure is also updated.
//<
//////////////////////////////////////////////////////
RETCODE _reentrant Playlist_GetCurrentSongFileInfo(INT iUnused1, INT iOffset, INT *pPtr)
#else
//////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant Playlist_GetCurrentSongFileInfo(int,int,SONGFILEINFO*SongInfo)
//
//   Type:          Function
//
//   Description:   Gets the song file info  (this is a required entry point)
//
//   Inputs:        SONGFILEINFO *      SongInfo        the pointer to the structure passed in by the PlayerLibrary.
//                                                      the PlayerLibrary must initialize some parts of the structure:
//                                                      m_wBufferLength and m_pFilename        
//
//   Outputs:       PLAYLIST_SUCCESS if SONGFILEINFO * SongInfo can be properly filled out, otherwise
//                  PLAYLIST_TRACK_NOT_FOUND        
//
//   Notes:         This fills the information for the current track. 
//					First the Directory links between the current track and ROOT are determined by calling PopulateSongFileInfo
//					Next, A complete file path is built by concatenating ROOT + DirLinks returned by PopulateSongFileInfo + File Entry in question.
//					g_CurrentTrack structure is also updated.
//<
//////////////////////////////////////////////////////
RETCODE _reentrant Playlist_GetCurrentSongFileInfo(INT iUnused1, INT iUnused2, INT *pPtr)
#endif
{
	SONGFILEINFO*SongInfo = (SONGFILEINFO*)pPtr;
	FileEntry* pFileEntry;
	int i,j;
	RETCODE rtn = PLAYLIST_TRACK_NOT_FOUND;
	_packed BYTE pszBuffer[SFN_LENGTH];
	TraverseTreeParams TraverseParams;
	pszBuffer[0] = 0;
	SongInfo->m_pFilename[0]=0;//null out the filename (so the packed_strncat's will work right)	
#ifdef PLAYLIST2_MULTIMEDIA
	if((g_CurrentTrack.iTrackOrder + iOffset) >= 0 && (g_CurrentTrack.iTrackOrder+iOffset) <= g_iTotalTracks)
#else
	if(g_CurrentTrack.iTrackOrder <= g_iTotalTracks)
#endif
	{
#ifdef PLAYLIST2_MULTIMEDIA
		pFileEntry = Playlist_GetFileEntry(g_CurrentTrack.iTrackOrder + iOffset);
#else
		pFileEntry = Playlist_GetFileEntry(g_CurrentTrack.iTrackOrder);
#endif
		if(pFileEntry)
		{
			if(Playlist_BuildFileLinks(pFileEntry,&TraverseParams) == PLAYLIST_SUCCESS)
			{
				//fill SongInfo structure with the path information
				//first get root
				Playlist_GetRootString((_packed BYTE*)SongInfo->m_pFilename,TraverseParams.iDevice);
				i = TraverseParams.iTotalLinks-1;
				while(!(i<0))	//Get intermediate directory names
				{
					if(Playlist_GetSFN(TYPE_DIR, (INT) TraverseParams.DirLinks[i--], (INT*)pszBuffer) == PLAYLIST_SUCCESS)
					{
						packed_strcat(SongInfo->m_pFilename,pszBuffer);
					}
					else
					{
						return rtn;	//invalid path, so exit
					}
				}
				//now strcat the filename
				if(Playlist_GetSFN(TYPE_FILE, (INT) pFileEntry, (INT*)pszBuffer)== PLAYLIST_SUCCESS)
				{
					packed_strcat(SongInfo->m_pFilename,pszBuffer);
					SongInfo->m_wTrack = (WORD) TraverseParams.iTrackNum;
					SongInfo->m_wDeviceID = pFileEntry->m_iDevice;
					//Now fill the g_CurrentTrack structure, for later use
					g_CurrentTrack.pFileEntry = pFileEntry;
					g_CurrentTrack.iTrackNum = TraverseParams.iTrackNum;
					i = TraverseParams.iTotalLinks-1;
					g_CurrentTrack.iTotalLinks = TraverseParams.iTotalLinks;
					
					while(!(i<0))
					{
						g_CurrentTrack.DirLinks[i] = TraverseParams.DirLinks[i];
						i--;
					}
					switch(g_iPlaySet)	//update the bookmark
					{
						case PLAYSET_MUSIC:
							g_MarkerMusic.m_iTracknum = g_CurrentTrack.iTrackNum;	
							break;
						case PLAYSET_VOICE:
							g_MarkerVoice.m_iTracknum = g_CurrentTrack.iTrackNum;		
							break;		
#ifdef PLAYLIST2_MULTIMEDIA									
						case PLAYSET_PHOTO:
							g_MarkerPhoto.m_iTracknum = g_CurrentTrack.iTrackNum;
							break;
						case PLAYSET_MVIDEO:
							g_MarkerMVideo.m_iTracknum = g_CurrentTrack.iTrackNum;
							break;
#endif							
					}					
					
					rtn = PLAYLIST_SUCCESS;
				}
				else
					return rtn;
			}
		}
	}
	return rtn;;
}

#ifdef PLAYLIST2_MULTIMEDIA									
//////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant Playlist_GetPreviousSongFileInfo(int,int,SONGFILEINFO*SongInfo)
//
//   Type:          Function
//
//   Description:   Gets the song file info  (this is a required entry point)
//
//   Inputs:        SONGFILEINFO *      SongInfo        the pointer to the structure passed in by the PlayerLibrary.
//                                                      the PlayerLibrary must initialize some parts of the structure:
//                                                      m_wBufferLength and m_pFilename        
//                  INT                 iRepeat         Repeat time for this operation. If it is zero, only advance one song  
//
//   Outputs:       PLAYLIST_SUCCESS if SONGFILEINFO * SongInfo can be properly filled out, otherwise
//                  PLAYLIST_END_OF_LIST        
//
//   Notes:         
//<
//////////////////////////////////////////////////////
RETCODE _reentrant Playlist_GetPreviousSongFileInfo(INT iUnused1, INT iRepeat, INT *pPtr) 
#else
//////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant Playlist_GetPreviousSongFileInfo(int,int,SONGFILEINFO*SongInfo)
//
//   Type:          Function
//
//   Description:   Gets the song file info  (this is a required entry point)
//
//   Inputs:        SONGFILEINFO *      SongInfo        the pointer to the structure passed in by the PlayerLibrary.
//                                                      the PlayerLibrary must initialize some parts of the structure:
//                                                      m_wBufferLength and m_pFilename        
//
//   Outputs:       PLAYLIST_SUCCESS if SONGFILEINFO * SongInfo can be properly filled out, otherwise
//                  PLAYLIST_END_OF_LIST        
//
//   Notes:         
//<
//////////////////////////////////////////////////////
RETCODE _reentrant Playlist_GetPreviousSongFileInfo(INT iUnused1, INT iUnused2, INT *pPtr)
#endif
{
    SONGFILEINFO*SongInfo = (SONGFILEINFO*)pPtr;
    RETCODE rtn = PLAYLIST_END_OF_LIST;
    Playlist_UpdateShuffle();
    if(g_iTotalTracks)
    {
#ifdef PLAYLIST2_MULTIMEDIA									
		while( iRepeat >= 0 )
		{
			iRepeat--;
#endif
        if(g_CurrentTrack.iTrackOrder <= 1)
            g_CurrentTrack.iTrackOrder = g_iTotalTracks;
        else
        	g_CurrentTrack.iTrackOrder--;
#ifdef PLAYLIST2_MULTIMEDIA									
	    }
#endif

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
//>  Name:          _reentrant FileEntry *Playlist_GetFileEntry(INT iTrackOrder)
//
//   Type:          Function
//
//   Description:   return the FileEntry holding the specified track order
//
//   Inputs:        TrackOrder
//
//   Outputs:       FileEntry holding the specified track order
//
//   Notes:         
//<
//////////////////////////////////////////////////////
FileEntry* _reentrant Playlist_GetFileEntry(INT iTrackOrder){
	int i;
	BOOL bFileFound = FALSE;
	for(i=0;i<MAX_NUM_FILES;i++)
		if(g_FileEntryPool[i].m_bAllocated && g_FileEntryPool[i].m_bCurrentPlayset)
			{
				if( g_FileEntryPool[i].m_iTrackOrder == iTrackOrder)
				{
					bFileFound= TRUE;
					break;
				}
			}
	
	if(bFileFound)
		return(&g_FileEntryPool[i]);
	else 
		return NULL;

}

//////////////////////////////////////////////////////
//
//>  Name:          INT _reentrant Playlist_GetDirEntry(INT iUnused1, INT iUnused2, INT *pCurrentTrack)
//
//   Type:          Function
//
//   Description:   return the DirEntry holding the Current Track
//
//   Inputs:        Ptr of "g_CurrentTrack"
//
//   Outputs:       DirEntry holding the Current Track
//
//   Notes:         Developed By SGTL-HK 28-10-2004
//<
//////////////////////////////////////////////////////

INT _reentrant Playlist_GetDirEntry(INT iUnused1, INT iUnused2, INT *pCurrentTrack)
{
	EntryAccessInfo		*pTrack = (EntryAccessInfo *)pCurrentTrack;

	if (pTrack->iTotalLinks == 0)
		return (INT)(&g_DirEntryPerDevice[pTrack->pFileEntry->m_iDevice]);
	else
		return (INT)(pTrack->DirLinks[0]);
}

_reentrant RETCODE Playlist_GetLFN(INT iEntryType,INT pEntry,INT* pName){
	TraverseTreeParams TraverseParams;
	DirEntry _X* pDirEntry;
	FileEntry* pFileEntry;
	DWORD dwFastKey;
    LONG DirSector;
	INT i;
	INT iDirHandle;
	RETCODE rtn = PLAYLIST_TRACK_NOT_FOUND;
	switch(iEntryType)
	{
	case TYPE_DIR:

		pDirEntry = (DirEntry _X*)pEntry;
		//here locate the pDirEntry using TraverseParams.


			//	SGTL-HK 28-10-2004
		TraverseParams.iReason = BUILD_DIR_LINKS;

		for (i = 0; i < _MAX_DIR_DEPTH; i++)
			TraverseParams.DirLinks[i] = 0;

		TraverseParams.iTotalLinks = 0;
		TraverseParams.pFileEntry = (FileEntry*)pEntry;

		if (SysCallFunction(RSRC_PLAYLIST2TRAVERSE_CODEBANK, Playlist_TraverseFromRoot, 0, 0, (INT *)&TraverseParams) == PLAYLIST_END_TRAVERSAL_SUCCESS)
		{
			//TraverseParams.iTotalLinks--;
			if (TraverseParams.iTotalLinks == 0)
			{
					//	It is the Root Directory, No LFN can be found
				break;
			}

			if (TraverseParams.iTotalLinks == 1)
			{
				dwFastKey = (DWORD)GetRootdirkey(pDirEntry->m_iDevice);
			}
			else
			{
				DirSector = (LONG)TraverseParams.DirLinks[0]->m_iDirSector1 | (((LONG)TraverseParams.DirLinks[0]->m_iDirSector2) << 21); // TOVERIFY2: check 31 or 23?
				dwFastKey = Playlist_GenerateFastKey(pDirEntry->m_iDevice, DirSector, TraverseParams.DirLinks[0]->m_iDirOffset);
			}

			if (!((iDirHandle = FastOpen((LONG)dwFastKey, (_packed char*)"d")) < 0))
			{
				if (ConstructLongFileName(iDirHandle, pDirEntry->m_iRecordNum, (INT*) pName) > 0)
					rtn = PLAYLIST_SUCCESS;
				Fclose(iDirHandle);
			}
		}
		break;   
	case TYPE_FILE:
		pFileEntry = (FileEntry*)pEntry;
		//next, locate the track and links
		TraverseParams.iReason = BUILD_FILE_LINKS;
		for(i=0;i<_MAX_DIR_DEPTH;i++)
			TraverseParams.DirLinks[i] = 0;
		TraverseParams.iTotalLinks = 0;
		TraverseParams.pFileEntry = pFileEntry;	//FileEntry to locate and build
		if(SysCallFunction(RSRC_PLAYLIST2TRAVERSE_CODEBANK,Playlist_TraverseFromRoot,0,0,(INT*) &TraverseParams) == PLAYLIST_END_TRAVERSAL_SUCCESS)
		{
			if(TraverseParams.iTotalLinks == 0) //i.e. no sub directories
				dwFastKey = (DWORD)GetRootdirkey(pFileEntry->m_iDevice);
			else
            
            {
                DirSector=(LONG)TraverseParams.DirLinks[0]->m_iDirSector1 | (((LONG)TraverseParams.DirLinks[0]->m_iDirSector2) << 21);
				dwFastKey = Playlist_GenerateFastKey(pFileEntry->m_iDevice,DirSector,
																TraverseParams.DirLinks[0]->m_iDirOffset);
             }                                                   
			if(!((iDirHandle = FastOpen((LONG)dwFastKey ,(_packed char*)"d")) <0))
			{
				if(ConstructLongFileName(iDirHandle, pFileEntry->m_iRecordNum, (INT*) pName) > 0)
					rtn = PLAYLIST_SUCCESS;
				Fclose(iDirHandle);
			}
		}
		break;
	}
	return rtn;
}
_reentrant INT Playlist_LocateEntryFromName(INT iUnused1, INT iUnused2, INT* pPtr){
	unsigned char i;
	void* pEntry = NULL;
	_packed BYTE* pName = (_packed BYTE*) pPtr;
	
	i = packed_get(pName, packed_strlen(pName)-1);
	if(i==DIR_SEPARATOR)
		pEntry = (void*) Playlist_LocateDirEntryFromName(0,0,(INT*)pName);
	else
		pEntry = (void*) Playlist_LocateFileEntryFromName(0,0,(INT*)pName);
	
	return (INT)pEntry;
}
_reentrant INT Playlist_LocateDirEntryFromName(INT iUnused1, INT iUnused2, INT* pPtr){
	int i,ByteIndex,iDevice, iStringLength;
	BYTE ByteIn;
	DirEntry _X* pEntry;
	_packed BYTE LinkName[SFN_LENGTH];
	_packed BYTE buffer[SFN_LENGTH];
	_packed BYTE* pFullName = (_packed BYTE*)pPtr;
	pEntry = NULL;
	iStringLength = packed_strlen(pFullName);
	if((iStringLength>MAX_FILENAME_LENGTH) || (iStringLength<=0))
		return NULL;
	else
	{
		ByteIndex  = 0;
		iDevice = -1;
		for(i=0;i<MAX_LOGICAL_DEVICES;i++)
		{
			LinkName[0] = NULL;
			if(Playlist_GetRootString(LinkName,i) == PLAYLIST_SUCCESS)
			{
				if(packed_strncmp(LinkName,pFullName,3)==3)	// compare first 3 characters for a:\ or b:\ 
				{
					iDevice = i;
					ByteIndex = 3;
					pEntry = &g_DirEntryPerDevice[i];
					break;
				}
			}
		}
		if(pEntry)		//if we got a valid device
		{
			while(ByteIndex<iStringLength)
			{
				LinkName[0]=0;
				do
				{
					ByteIn = packed_get(pFullName,ByteIndex);
					packed_strcat(LinkName,(_packed BYTE*) &ByteIn);
					ByteIndex++;
					if(ByteIndex>MAX_FILENAME_LENGTH)
					{
						pEntry = NULL;
						break;
					}
				}while(ByteIn != DIR_SEPARATOR && ByteIn != NULL);	//till we reach a '\' or NULL
				
				if(pEntry)
					pEntry = (DirEntry _X*) pEntry->m_pDirContents;

				while(pEntry && *LinkName)	//if there is a valid entry and a valid link name
				{
					if(Playlist_GetSFN(TYPE_DIR,(INT) pEntry, (INT*)buffer)== PLAYLIST_SUCCESS)
					{
						if(packed_strcmp(LinkName,buffer) == 0)
							break;
						else
							pEntry = (DirEntry _X*) pEntry->m_pNext;
					}
					else
					{
						pEntry = NULL;	//could not get name, so exit
						break;
					}
				}
				if(!pEntry)	//missing link, exit with NULL
					break;
			}
		}
	}
	return (INT) pEntry;
}

_reentrant RETCODE Playlist_LocateFileEntryFromName(INT iUnused1, INT iUnused2, INT* pPtr){
	FileEntry* pFileEntry;
	pFileEntry = NULL;
	return (INT) pFileEntry;
}
//////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant Playlist_Initialize(void)
//
//   Type:          Function
//
//   Description:   
//
#ifdef TUNER_STFM1000
//   Inputs:        bDontGetCurrentSong - non zero load the decoder and get ready to play the song
//                  for Steely Dan FM tuner only rebuild the play list
#else
//   Inputs:        
#endif
//
//   Outputs:       PLAYLIST_SUCCESS
//
//   Notes:         This should ideally be in playlist2init.c. But, leaving it here
//					since other codebanks call this using RSRC_PLAYLIST_CODEBANK
//<
//////////////////////////////////////////////////////
#ifdef TUNER_STFM1000
RETCODE _reentrant Playlist_Initialize(int bDontGetCurrentSong, int b, int *c)
{
	b;c;
#else


RETCODE _reentrant Playlist_Initialize(int a, int b, int *c)
{
	a;b;c;
#endif
	g_iTotalFiles = 0;	//debug purposes;
	g_iTotalDir = 0;
    SysCallFunction(RSRC_PLAYLIST2INIT_CODEBANK,Playlist_BuildContentTree,0,0,0);
    Playlist_SetPlaySet(g_iPlaySet,0,0);
#ifdef TUNER_STFM1000
	if (!bDontGetCurrentSong)
#endif
	SysCallFunction(RSRC_PLAYERLIB_EXTRA_CODE_BANK,PlayerLib_GetCurrentSong,0,0,0);
	return PLAYLIST_SUCCESS;
}
