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
//////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant Playlist_TraverseFromRoot(INT a, INT b, INT* pPtr)
//
//   Type:          Function
//
//   Description:   A generic traversal algorithm starting from ROOT of each device.
//
//   Inputs:        TraverseTreeParams* TraverseParams - Expects iReason to be filled
//
//   Outputs:       The return value depends on iReason and can be interpreted by the callee. For eg.
//					PLAYLIST_END_TRAVERSAL_SUCCESS - end further traversal (recursion) if iReason is fulfilled.
//					PLAYLIST_END_TRAVERSAL_FAILURE - end further traversal (recursion) if an error encountered.
//					
//   Notes:         
//<
//////////////////////////////////////////////////////
RETCODE _reentrant Playlist_TraverseFromRoot(INT a, INT b, INT* pPtr){

	TraverseTreeParams* TraverseParams;
	int i;
	RETCODE rtn = PLAYLIST_SUCCESS;
	TraverseParams = (TraverseTreeParams*) pPtr;
	a;b;
	for(i=0;i<MAX_LOGICAL_DEVICES;i++)
	{
		TraverseParams->pDirEntry = &g_DirEntryPerDevice[i];
		TraverseParams->iDepth = 0;
		TraverseParams->iDevice = i;
		rtn = Playlist_TraverseDir(a, b, (INT*)TraverseParams);
		if(rtn == PLAYLIST_END_TRAVERSAL_SUCCESS)		//Traverse no more
			break;
	}
	return rtn;
}
//////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant Playlist_TraverseFromRoot(INT a, INT b, INT* pPtr)
//
//   Type:          Function
//
//   Description:   A generic traversal algorithm starting from any given directory entry.
//
//   Inputs:        TraverseTreeParams* TraverseParams - Expects iReason and TraverseParams->pDirEntry to be filled
//
//   Outputs:       The return value depends on iReason and can be interpreted by the callee. For eg.
//					PLAYLIST_END_TRAVERSAL_SUCCESS - end further traversal (recursion) if iReason is fulfilled.
//					PLAYLIST_END_TRAVERSAL_FAILURE - end further traversal (recursion) if an error encountered.
//					
//   Notes:         This will traverse the tree from parent. In the case statements for iReason the developer can introduce API for
//					processing the tree contents as suited. Currently used for selecting tracks, ordering tracks and filling file information.
//<
//////////////////////////////////////////////////////
RETCODE _reentrant Playlist_TraverseDir(INT a, INT b, INT* pPtr){

		//	SGTL-HK 28-10-2004
	DirEntry _X*	pParentDirEntry = NULL;

	TraverseTreeParams* TraverseParams;
	DirEntry _X* pCurrentEntry;
	INT	 iPlayset;
	BOOL bDone = FALSE;
	RETCODE  rtn = PLAYLIST_END_OF_LIST;
	BOOL bInclude = FALSE;
	TraverseParams = (TraverseTreeParams*) pPtr;

	a;b;
	if(TraverseParams)
	{
		if(TraverseParams->pDirEntry)
		{
			switch(TraverseParams->iReason)
			{
			case SELECT_TRACKS:				//Here get directory name and include/exclude voice files
				switch(TraverseParams->iPlayset)
				{
#ifdef PLAYLIST2_MULTIMEDIA					
				case PLAYSET_PHOTO:
					if(TraverseParams->iDepth == DEPTH_PHOTO_DIR)
					{
						if(IsPhotoDir(TraverseParams->pDirEntry))
							bInclude = TRUE;
					}
					//include only "PHOTO\" 
					break;
				case PLAYSET_MVIDEO:
					if(TraverseParams->iDepth == DEPTH_MVIDEO_DIR)
					{
						if(IsMotionVideoDir(TraverseParams->pDirEntry))
							bInclude = TRUE;
					}
					//include only "MVIDEO\"
					break;
#endif
				case PLAYSET_MUSIC:
					if(TraverseParams->iDepth == DEPTH_VOICE_DIR)
					{
						if(! IsVoiceDir(TraverseParams->pDirEntry))
							bInclude = TRUE;
					}
					else
						bInclude = TRUE;
					// include all but VOICE\ 
					break;
				case PLAYSET_VOICE:
					if(TraverseParams->iDepth == DEPTH_VOICE_DIR)
					{
						if(IsVoiceDir(TraverseParams->pDirEntry))
							bInclude = TRUE;
					}
					//include only VOICE\ 
					break;
				case PLAYSET_FAVORITES:
					//include all (decision to include files will be based on the bit set for favorites)
					bInclude = TRUE;
					break;
				}
				if(bInclude == TRUE)
					Playlist_SelectTracks(0,0,(INT*) TraverseParams);
				break;

			case ORDER_TRACKS:
				Playlist_AssignTrackOrder(0,0,(INT*) TraverseParams);
				break;

			case BUILD_FILE_LINKS:
				if((Playlist_LocateFileEntry(0,0,(INT*)TraverseParams)) == PLAYLIST_FILE_LOCATE_SUCCESS)
				{
					rtn = PLAYLIST_END_TRAVERSAL_SUCCESS;
					bDone = TRUE;	//signals end of recursion
				}
				break;


				//	SGTL-HK 28-10-2004
			case BUILD_DIR_LINKS:

				if ((Playlist_LocateDirEntry(0, 0, (INT *)TraverseParams)) == PLAYLIST_FILE_LOCATE_SUCCESS)
				{
					rtn = PLAYLIST_END_TRAVERSAL_SUCCESS;
					bDone = TRUE;							//	Signals end of recursion
				}
				else
				{
					pParentDirEntry = TraverseParams->pDirEntry;
				}
				break;


			case RESTORE_BOOKMARK:		
				if(Playlist_FindBookMarkedSong(a , b, (INT*) TraverseParams) ==  PLAYLIST_FILE_LOCATE_SUCCESS){
					rtn = PLAYLIST_END_TRAVERSAL_SUCCESS;
					bDone = TRUE;
				}
				break;
			}
	//////////////////Generic traversing algo////////////////////////////////////////////////////////////////////////////////
			TraverseParams->pDirEntry = (DirEntry _X*) TraverseParams->pDirEntry->m_pDirContents;
			TraverseParams->iDepth++;
			while(TraverseParams->pDirEntry && !bDone)
			{
				pCurrentEntry = TraverseParams->pDirEntry;
				rtn = Playlist_TraverseDir(a, b, (INT*)TraverseParams);

				//Now handle end of recursion for each case if necessary
				switch(TraverseParams->iReason)
				{
				case SELECT_TRACKS:
					switch(TraverseParams->iPlayset)
					{
#ifdef PLAYLIST2_MULTIMEDIA						
					case PLAYSET_PHOTO:
						break;
					case PLAYSET_MVIDEO:
						break;
#endif						
					case PLAYSET_MUSIC:
						break;
					case PLAYSET_VOICE:
						break;
					case PLAYSET_FAVORITES:
						break;
					}
					break;

				case ORDER_TRACKS:
					break;

				case BUILD_FILE_LINKS:
					if(rtn == PLAYLIST_END_TRAVERSAL_SUCCESS)
					{
						TraverseParams->DirLinks[TraverseParams->iTotalLinks++] = pCurrentEntry;
						bDone = TRUE;
					}
					break;
				

					//	SGTL-HK 28-10-2004
				case BUILD_DIR_LINKS:

					if (rtn == PLAYLIST_END_TRAVERSAL_SUCCESS)
					{
						TraverseParams->DirLinks[TraverseParams->iTotalLinks++] = pParentDirEntry;
						bDone = TRUE;
					}
					break;

				
				case RESTORE_BOOKMARK:
					if(rtn == PLAYLIST_END_TRAVERSAL_SUCCESS)
					{
						bDone = TRUE;
					}
					break;
				}
				TraverseParams->pDirEntry = (DirEntry _X*) pCurrentEntry->m_pNext;
			}
			TraverseParams->iDepth--;
		}
	}
	return rtn;
}
//////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant Playlist_LocateFileEntry(INT a, INT b, INT* pPtr)
//
//   Type:          Function
//
//   Description:   This detects if a FileEntry (TraverseParams->pFileEntry) lies within the specified DirEntry (TraverseParams->pDirEntry)
//
//   Inputs:        TraverseTreeParams* TraverseParams
//
//   Outputs:       PLAYLIST_TRACK_NOT_FOUND if the DirEntry does not contain the specified FileEntry
//					PLAYLIST_FILE_LOCATE_SUCCESS if the DirEntry contains the specified FileEntry.
//					
//   Notes:         This also increments the iTrackNum field of TraverseParams if each compared fileentry belongs to the current playset.
//<
//////////////////////////////////////////////////////
RETCODE _reentrant Playlist_LocateFileEntry(INT a, INT b, INT* pPtr){
	TraverseTreeParams* TraverseParams;
	FileEntry* pFileEntry;
	RETCODE rtn = PLAYLIST_TRACK_NOT_FOUND;
	TraverseParams = (TraverseTreeParams*) pPtr;

	if(TraverseParams && TraverseParams->pDirEntry)
	{
		pFileEntry = (FileEntry*) TraverseParams->pDirEntry->m_pFileContents;
		while(pFileEntry)
		{
			if(pFileEntry->m_bCurrentPlayset)
				TraverseParams->iTrackNum++;

			if(TraverseParams->pFileEntry == pFileEntry)
			{
				rtn = PLAYLIST_FILE_LOCATE_SUCCESS;
				break;
			}
			pFileEntry = (FileEntry*)pFileEntry->m_pNext;
		}
	}
	return rtn;
}
//////////////////////////////////////////////////////
//
//>  Name:          Playlist_LocateDirEntry(INT a, INT b, INT* pPtr)
//
//   Type:          Function
//
//   Description:   SGTL-HK 28-10-2004
//
//   Inputs:        
//
//   Outputs:       
//					
//   Notes:         
//<
//////////////////////////////////////////////////////

RETCODE _reentrant Playlist_LocateDirEntry(INT a, INT b, INT* pPtr)
{
	TraverseTreeParams*		TraverseParams;
	DirEntry _X*	pDirEntry;
	RETCODE		rtn = PLAYLIST_TRACK_NOT_FOUND;

	TraverseParams = (TraverseTreeParams *)pPtr;

	if (TraverseParams && TraverseParams->pDirEntry)
	{
		pDirEntry = (DirEntry _X *)TraverseParams->pDirEntry;

		while (pDirEntry)
		{
			if (TraverseParams->pFileEntry == (FileEntry *)pDirEntry)
			{
				rtn = PLAYLIST_FILE_LOCATE_SUCCESS;
				break;
			}
			pDirEntry = (DirEntry _X*)pDirEntry->m_pNext;
		}
	}
	return rtn;
}
  

//////////////////////////////////////////////////////
//
//>  Name:          Playlist_FindBookMarkedSong(INT a, INT b, INT* pPtr)
//
//   Type:          Function
//
//   Description:   searched though files in a directory to match TraverseParams->iTrackNum to local variable a 
//
//   Inputs:        TraverseTreeParams* TraverseParams
//					a = track number to match
//
//   Outputs:       PLAYLIST_TRACK_NOT_FOUND if the DirEntry does not contain the specified FileEntry
//					PLAYLIST_FILE_LOCATE_SUCCESS if the DirEntry contains the specified FileEntry.
//					
//   Notes:         This is used to increment TraverseParams->iTrackNum until it matched a.  TraverseParams->iTrackNum is
//					incremented whenever a file in the current playset is found.
//<
//////////////////////////////////////////////////////
RETCODE _reentrant Playlist_FindBookMarkedSong(INT a, INT b, INT* pPtr){
	TraverseTreeParams* TraverseParams;
	FileEntry* pFileEntry;
	RETCODE rtn = PLAYLIST_TRACK_NOT_FOUND;
	TraverseParams = (TraverseTreeParams*) pPtr;

	if(TraverseParams && TraverseParams->pDirEntry){	//order is impt here TraverseParams must be checked first
		pFileEntry = (FileEntry*) TraverseParams->pDirEntry->m_pFileContents;
		while(pFileEntry && (rtn == PLAYLIST_TRACK_NOT_FOUND)){		//traverse till end or we find the song
			if(pFileEntry->m_bCurrentPlayset){					
				TraverseParams->iTrackNum++;
				if(a == TraverseParams->iTrackNum){	//only check if file is in playset otherwise we would get wrong order number									
					TraverseParams->iTrackOrder = pFileEntry->m_iTrackOrder;		//this will be sent all the way back up to
					rtn = PLAYLIST_FILE_LOCATE_SUCCESS;								//Playlist_SetPlayset.
				}
			}
			pFileEntry = (FileEntry*)pFileEntry->m_pNext;
		}
	}
	return rtn;
}

//////////////////////////////////////////////////////
//
//>  Name:          void _reentrant Playlist_SelectTracks(INT a, INT b, INT* pPtr)
//
//   Type:          Function
//
//   Description:   Mark the tracks that need to be included in the current playset
//
//   Inputs:        TraverseTreeParams* TraverseParams
//
//   Outputs:       Marks the m_bAllocated bit of all the tracks in TraverseParams->pDirEntry.
//   Notes:         This API should be called based on the playset i.e. in music playset, the callee has to exclude VOICE\ before calling this function.
//					Case statements can be added to selectively add tracks within directories for special features like favorites.
//<
//////////////////////////////////////////////////////
void _reentrant Playlist_SelectTracks(INT a, INT b, INT* pPtr){
	TraverseTreeParams* TraverseParams;
	FileEntry* pFileEntry;
	TraverseParams = (TraverseTreeParams*)pPtr;
	if(TraverseParams)
	{
		if(TraverseParams->pDirEntry)
		{
			pFileEntry = (FileEntry*) TraverseParams->pDirEntry->m_pFileContents;
			while(pFileEntry)
			{
#ifdef PLAYLIST2_MULTIMEDIA
				if(TraverseParams->iPlayset==PLAYSET_PHOTO && pFileEntry->m_iMediaType==ENTRY_IS_PHOTO)
				{
					pFileEntry->m_bCurrentPlayset = TRUE;
					g_iTotalTracks++;
				}
				else if(TraverseParams->iPlayset==PLAYSET_MVIDEO && pFileEntry->m_iMediaType==ENTRY_IS_MVIDEO)
				{
					pFileEntry->m_bCurrentPlayset = TRUE;
					g_iTotalTracks++;
				}
				else if(TraverseParams->iPlayset!=PLAYSET_PHOTO && TraverseParams->iPlayset!=PLAYSET_MVIDEO 
							&& pFileEntry->m_iMediaType==ENTRY_IS_AUDIO)
				{
#endif
				pFileEntry->m_bCurrentPlayset = TRUE;
				    g_iTotalTracks++;
#ifdef PLAYLIST2_MULTIMEDIA
				}
#endif
				pFileEntry = (FileEntry*)pFileEntry->m_pNext;
			}
		}
	}
}
//////////////////////////////////////////////////////
//
//>  Name:          reentrant BOOL IsVoiceDir(_packed BYTE *pszName)
//
//   Type:          Function
//
//   Description:   used to simplify the code for building the
//                    current play queue
//
//   Inputs:        packed string        
//
//   Outputs:       TRUE if voice Directory else FALSE
//
//   Notes:         This function returns TRUE if the directory 
//                    string passed in is VOICE\
//
//                  
//<
//////////////////////////////////////////////////////
BOOL _reentrant  IsVoiceDir(DirEntry _X* pDirEntry)
{
    BOOL rtn = FALSE;
    _packed BYTE szVoice[7];
	_packed BYTE buffer[SFN_LENGTH];
	buffer[0] = 0;

    ((int*)szVoice)[0] = 0x494f56;  // IOV
    ((int*)szVoice)[1] = 0x2f4543;  //  EC
    ((int*)szVoice)[2] = 0;         // /0 string terminator
	
	if(pDirEntry)
	{
		if(Playlist_GetSFN(TYPE_DIR, (INT) pDirEntry,(INT*)buffer) == PLAYLIST_SUCCESS)
		{
			if(!packed_strcmp(buffer,szVoice))
			   rtn = TRUE;
		}
	}
    return(rtn);
}

#ifdef PLAYLIST2_MULTIMEDIA
//////////////////////////////////////////////////////
//
//>  Name:          reentrant BOOL IsPhotoDir(_packed BYTE *pszName)
//
//   Type:          Function
//
//   Description:   used to simplify the code for building the
//                    current play queue
//
//   Inputs:        packed string        
//
//   Outputs:       TRUE if photo Directory else FALSE
//
//   Notes:         This function returns TRUE if the directory 
//                    string passed in is PHOTO\
//
//                  
//<
//////////////////////////////////////////////////////
BOOL _reentrant  IsPhotoDir(DirEntry _X* pDirEntry)
{
    BOOL rtn = FALSE;
    _packed BYTE szPhoto[7];
	_packed BYTE buffer[SFN_LENGTH];
	buffer[0] = 0;

    ((int*)szPhoto)[0] = 'P' | 'H' << 8 | 'O' << 16;
    ((int*)szPhoto)[1] = 'T' | 'O' << 8 | '~' << 16;
    ((int*)szPhoto)[2] = 0;         // /0 string terminator
	
	if(pDirEntry)
	{
		if(Playlist_GetSFN(TYPE_DIR, (INT) pDirEntry,(INT*)buffer) == PLAYLIST_SUCCESS)
		{
			((INT*)buffer)[2] = 0;
			if(!packed_strcmp(buffer,szPhoto))
			   rtn = TRUE;

		    ((int*)szPhoto)[1] = 'T' | 'O' << 8 | '/' << 16;
			if(!packed_strcmp(buffer,szPhoto))
			   rtn = TRUE;
		}
	}
    return(rtn);
}

//////////////////////////////////////////////////////
//
//>  Name:          reentrant BOOL IsMotionVideoDir(_packed BYTE *pszName)
//
//   Type:          Function
//
//   Description:   used to simplify the code for building the
//                    current play queue
//
//   Inputs:        packed string
//
//   Outputs:       TRUE if photo Directory else FALSE
//
//   Notes:         This function returns TRUE if the directory
//                    string passed in is PHOTO\
//
//
//<
//////////////////////////////////////////////////////
BOOL _reentrant  IsMotionVideoDir(DirEntry _X* pDirEntry)
{
    BOOL rtn = FALSE;
    _packed BYTE szPhoto[7];
	_packed BYTE buffer[SFN_LENGTH];
	buffer[0] = 0;

    ((int*)szPhoto)[0] = 'M' | 'V' << 8 | 'I' << 16;
    ((int*)szPhoto)[1] = 'D' | 'E' << 8 | 'O' << 16;
    ((int*)szPhoto)[2] = '~';         // /0 string terminator

	if(pDirEntry)
	{
		if(Playlist_GetSFN(TYPE_DIR, (INT) pDirEntry,(INT*)buffer) == PLAYLIST_SUCCESS)
		{
			((INT*)buffer)[2] = '~';
			if(!packed_strcmp(buffer,szPhoto))
			   rtn = TRUE;
		}
	}

    return(rtn);
}

//////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant IsBitmapFile(_packed Byte *pFileName)
//
//   Type:          Function
//
//   Description:   used to simplify the code for building the current play queue
//
//   Inputs:        _packed BYTE *pFileName  8.3    
//
//   Outputs:       TRUE or FALSE
//
//   Notes:         This function encapsulates the logic for 'jpeg' vs. 'bmp' files so that
//                  the developer could easily change the requirements for what is a bitmap file or not.
//
//                  In this implementation:
//                      a bitmap file is a bmp file
//                      a non-voice file is jpeg file 
//<
//////////////////////////////////////////////////////
BOOL _reentrant IsBitmapFile(_packed BYTE *pFileName)
{
    BOOL rtn = FALSE;
    if(SysCallFunction(RSRC_PLAYLIST2INIT_CODEBANK,Playlist_GetFileExtention,0,0,(INT*)pFileName) == BMP_FILE_EXT)
       rtn = TRUE;
    return (rtn);
}
#endif // PLAYLIST2_MULTIMEDIA
//////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant IsVoiceFile(_packed Byte *pFileName)
//
//   Type:          Function
//
//   Description:   used to simplify the code for building the current play queue
//
//   Inputs:        _packed BYTE *pFileName  8.3    
//
//   Outputs:       TRUE or FALSE
//
//   Notes:         This function encapsulates the logic for 'voice' vs. 'music' files so that
//                  the developer could easily change the requirements for what is a voice file or not.
//
//                  In this implementation:
//                      a voice file is a wav file that resides in the /voice/ directory on either device 
//                      a non-voice file is any other playable file 
//                  Other possible implementations could be:
//                      a voice file is any wav file
//                      a music file is all others
//<
//////////////////////////////////////////////////////
BOOL _reentrant IsVoiceFile(_packed BYTE *pFileName)
{
    BOOL rtn = FALSE;
    if(SysCallFunction(RSRC_PLAYLIST2INIT_CODEBANK,Playlist_GetFileExtention,0,0,(INT*)pFileName) == WAV_FILE_EXT)
       rtn = TRUE;
    return (rtn);
}
//////////////////////////////////////////////////////
//
//>  Name:          void _reentrant Playlist_SelectTracks(INT a, INT b, INT* pPtr)
//
//   Type:          Function
//
//   Description:   Assign track order to selected tracks
//
//   Inputs:        TraverseTreeParams* TraverseParams
//
//   Outputs:       none
//   Notes:         Track order is assigned in the sorted order starting from TraverseParams->iTrackOrder.
//<
//////////////////////////////////////////////////////
void _reentrant Playlist_AssignTrackOrder(INT a, INT b, INT* pPtr){

	TraverseTreeParams* TraverseParams;
	FileEntry* pFileEntry;
	TraverseParams = (TraverseTreeParams*) pPtr;
	if(TraverseParams)
	{
		if(TraverseParams->pDirEntry)
		{
			pFileEntry = (FileEntry*) TraverseParams->pDirEntry->m_pFileContents;
			while(pFileEntry)
			{
				if(pFileEntry->m_bAllocated && pFileEntry->m_bCurrentPlayset)
					pFileEntry->m_iTrackOrder = ++(TraverseParams->iTrackOrder); //start track order from one
				pFileEntry = (FileEntry*)pFileEntry->m_pNext;
			}
		}
	}
}

//////////////////////////////////////////////////////
//
//>  Name:          void _reentrant Playlist_RestoreBookmark(void)
//
//   Type:          Function
//
//   Description:   sets the current song to the one bookmarked when switching between playsets
//
//   Inputs:        INT bResetBookmark - used to reset bookmark when STOP is pressed twice.
//
//   Outputs:       none
//
//   Notes:         This function is only called from Playlist_SetPlaySet.                 
//<
//////////////////////////////////////////////////////
void _reentrant Playlist_RestoreBookmark(INT bResetBookmark, INT iIgnored1, INT* pIgnored){
	int i;		
	TraverseTreeParams TraverseParams;

	switch(g_iPlaySet)	//the bookmarks are updated in Playlist_GetCurrentSongFileInfo()
	{
	case PLAYSET_MUSIC:
		if(bResetBookmark == TRUE){
			g_MarkerMusic.m_iTracknum = 1;	
			i=1;
		}
		else{
			i = g_MarkerMusic.m_iTracknum;	
			}
		break;
	case PLAYSET_VOICE:
		if(bResetBookmark == TRUE){
			g_MarkerVoice.m_iTracknum = 1;
			i=1;
		}
		else{
			i = g_MarkerVoice.m_iTracknum;		
		}
		break;
#ifdef PLAYLIST2_MULTIMEDIA		
	case PLAYSET_PHOTO:
		if(bResetBookmark == TRUE){
			g_MarkerPhoto.m_iTracknum = 1;
			i=1;
		}
		else{
			i = g_MarkerPhoto.m_iTracknum;
		}
		break;
	case PLAYSET_MVIDEO:
		if(bResetBookmark == TRUE){
			g_MarkerMVideo.m_iTracknum = 1;
			i=1;
		}
		else{
			i = g_MarkerMVideo.m_iTracknum;
		}
		break;
#endif
	default:
		i=0;
		break;
	}				
	g_CurrentTrack.iTrackOrder=1;		//start at the first track.
	if(i){	//unless we need to restore a bookmark
		TraverseParams.iReason = RESTORE_BOOKMARK;
		TraverseParams.iTrackNum = 0;
		TraverseParams.iTrackOrder = 0;	
		if(Playlist_TraverseFromRoot(i,0,(INT*) &TraverseParams) == PLAYLIST_END_TRAVERSAL_SUCCESS)
			g_CurrentTrack.iTrackOrder = TraverseParams.iTrackOrder;
	}	
}
