////////////////////////////////////////////////////////////////////////////////
// Copyright (c) SigmaTel, Inc. 2003
// Filename: playlist1.c
//
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "menumanager.h"
#include "playlist.h"       //required include file
#include "playerlib.h"
//#include "io.h"
#include "project.h"
#include "stringlib.h"
#include "resource.h"
#include "syscoder.h"
#include "sysspeed.h"
#ifndef BATTERY_CHARGE	
#include "s6b33b0a.h"
#endif
#include "playlist1.h"
#include "playlist1internal.h"
#include "sysmem.h"
#include "longfilenames.h"
#include "FileSpec.h"
#include "fserror.h"


extern UCS3* g_wSongTitle;
extern BOOL g_bInitBookmarkActive;
extern _reentrant INT FindNext(INT HandleNumber,Finddata *_finddata);
extern _reentrant INT FindFirst(Finddata *_finddata,_packed char *FileName);
extern _reentrant INT Fclose(INT HandleNumber);
extern _reentrant INT GetCWDHandle(void);
extern INT _reentrant ConstructLongFileName(INT HandleNumber, INT RecordNumber, INT *LFNBuffer);

int g_iPlaylistRepeat = PLAYLIST_REPEAT_OFF;
BOOL g_bPlaylistShuffle = FALSE;
int g_bPlaylistShuffleUpdate = FALSE;

int g_iInternalTrack=0;
int g_iTotalTracks=0;

int g_iPlaySet=PLAYSET_MUSIC;

struct Bookmark g_MarkerMusic, g_MarkerVoice;
//this will save the num and position of the song selected for bookmark
_reentrant WORD rand(void);
void _reentrant Playlist_FillPlayQueue(void);

RETCODE _reentrant Playlist_PopulateSongFileInfo(SONGFILEINFO*SongInfo,struct FileEntry *pCurrentEntry);


RETCODE _reentrant Playlist_ResetList(void);

//////////////////////////////////////////////////////
//
//>  Name:          INT _reentrant Playlist_SetPlaySet(INT iPlayset, INT unused, INT *pUnused)
//
//   Type:          Function
//
//   Description:   Activate a given playset. Returns old playset (PLAYSET_MUSIC or PLAYSET_VOICE)
//
//   Inputs:        INT     iPlayset    The new playset
//
//   Outputs:       INT                 old playset
//
//   Notes:         This refills the play queue, and shuffles it 
//                  based on the current shuffle & playset settings.  
//<
//////////////////////////////////////////////////////
INT _reentrant Playlist_SetPlaySet(INT iPlayset, INT unused, INT *pUnused)
//////////////////////////////////////////////////////
{
    int iOldPlayset = g_iPlaySet;
    unused;pUnused;

    g_iPlaySet = iPlayset;//PLAYSET_MUSIC or PLAYSET_VOICE.
    //rebuild the play list based on this.
    Playlist_FillPlayQueue();

    //reset some counters to the first song.
    g_iInternalTrack = 0;
    g_iInternalList[0]=0;  
    // DEFECT #2881 FIX: Above line stomps the playlist 0th element overwriting a valid
    // song number if shuffle was done previously. Solution chosen is to sort 
    // the list from within the shuffle function before shuffling. 

    //call the shuffle algorithm (if we're not in voice mode)
    Playlist_ShuffleList(g_bPlaylistShuffle && (g_iPlaySet != PLAYSET_VOICE),0,0);

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
void _reentrant Playlist_ShuffleList(INT bTrueFalse, INT iIgnored, INT *pIgnored)
//////////////////////////////////////////////////////
{
    g_bPlaylistShuffle = bTrueFalse;
    g_bPlaylistShuffleUpdate = TRUE;
}



void _reentrant Playlist_UpdateShuffle(void)
{
    if(g_bPlaylistShuffleUpdate)
    {
        g_bPlaylistShuffleUpdate = FALSE;
        if(g_iTotalTracks)
        {
            if(g_bPlaylistShuffle)
            {
                int i;
                int iRandTrackIndex;
                int tmpEntry;
                int iRandNum;

                // Must reorder internal list before shuffle due to Playlist_SetPlaySet 
                // clearing element 0. This maps 1 to 1 to the ordered play list
                for(i=0; i < g_iTotalTracks; i++)
                {   g_iInternalList[i] = i;
                }

                //lets shuffle the list
                for(i=0;i<g_iTotalTracks;i++)
                {
                    iRandNum = rand();   // testpoint for rand value
                    iRandTrackIndex = iRandNum % g_iTotalTracks;
                    tmpEntry = g_iInternalList[i];
                    g_iInternalList[i] = g_iInternalList[iRandTrackIndex];
                    g_iInternalList[iRandTrackIndex] = tmpEntry;
                }
                //start that list at 0.
                g_iInternalTrack = 0;
            }
            else
            {//lets put the list in order
                int i;
                //lets put the internal track to the current track.
                g_iInternalTrack = g_iInternalList[g_iInternalTrack];
                //the internal list maps 1 to 1 to the ordered play list
                for(i=0;i<g_iTotalTracks;i++)
                    g_iInternalList[i]=i;
            }
			//now go to bookmarked track only if we are in the process of initialising the playlist.
			if(g_bInitBookmarkActive){
				int iTracknum = 0;
				switch(g_iPlaySet)
				{
					
				case PLAYSET_MUSIC:
					iTracknum = g_MarkerMusic.m_iTracknum;
					break;
				case PLAYSET_VOICE:
					iTracknum = g_MarkerVoice.m_iTracknum;
					break;
				}

				SysCallFunction(RSRC_PLAYERLIB_EXTRA_CODE_BANK,PlayerLib_GotoTrack,iTracknum,0,0);
			}


        }
    }
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
//>  Name:          RETCODE _reentrant Playlist_PopulateSongFileInfo(SONGFILEINFO*SongInfo,struct FileEntry *pCurrentEntry)
//
//   Type:          Function
//
//   Description:   This function encapsulates what is necessary to populate the SONGFILEINFO structure
//                  with information obtained from the current entry.  
//
//   Inputs:        SONGFILEINFO *      SongInfo        the pointer to the structure passed in by the PlayerLibrary.
//                                                      the PlayerLibrary must initialize some parts of the structure:
//                                                      m_wBufferLength and m_pFilename.        
//                  struct FileEntry*   pCurrentEntry   The pointer to the file entry chosen
//
//   Outputs:       a properly filled out SONGFILEINFO structure.   
//
//   Notes:         The filename returned must be a fully specified path name.  
//                  In order to get the fully specified path name, we walk up the 
//                  content tree going to each parent in turn, keeping a list of 
//                  elements we touch.   Once we reach the root (the element with 
//                  no parent), we then go through this list of elements and add 
//                  their filename to the fully qualified filename.
//<
//////////////////////////////////////////////////////
RETCODE _reentrant Playlist_PopulateSongFileInfo(SONGFILEINFO*SongInfo,struct FileEntry *pCurrentEntry)
//////////////////////////////////////////////////////
{
    RETCODE rtn = PLAYLIST_SUCCESS;

    int iPathPieceIndex = 0;
    struct FileEntry * pPathPieces[_MAX_DIR_DEPTH];

    SongInfo->m_pFilename[0]=0;//null out the filename (so the packed_strncat's will work right)
    
    SongInfo->m_wDeviceID = pCurrentEntry->m_iDevice;
    SongInfo->m_wTrack = g_iInternalList[g_iInternalTrack]+1;//the track number is 1 based

    //this block of code will generate an array of file entries/nodes in the content tree that walk up to the root.
    while(pCurrentEntry)
    {//note, you could exceed the array of pPathPieces with this! (no bounds checking)
        pPathPieces[iPathPieceIndex++] = pCurrentEntry;
        pCurrentEntry = pCurrentEntry->m_pContainer;
        if(iPathPieceIndex >= _MAX_DIR_DEPTH)
        {
            rtn = PLAYLIST_END_OF_LIST;
            iPathPieceIndex=0;
            break;
        }
    }

    //we traverse this array backwards to build up the fully specified path name.
    while(iPathPieceIndex)
    {
        register bufferlength = SongInfo->m_wBufferLength;
        iPathPieceIndex --;
        if(bufferlength  > packed_strlen(pPathPieces[iPathPieceIndex]->m_pszFilename))
        {//only do the strcat if we have room
            packed_strcat(SongInfo->m_pFilename,pPathPieces[iPathPieceIndex]->m_pszFilename);
            bufferlength -= packed_strlen(SongInfo->m_pFilename);
        }
        else
        {//we should handle this some how...
            rtn = PLAYLIST_END_OF_LIST;
            break;
        }
    }
    return rtn;
}


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
{
    SONGFILEINFO*SongInfo = (SONGFILEINFO*)pPtr;
    RETCODE rtn = PLAYLIST_END_OF_LIST;

    Playlist_UpdateShuffle();
    if(g_iTotalTracks)
    {//if we have any tracks at all
        if((g_iPlaylistRepeat!=PLAYLIST_REPEAT_ONE)|
           (iMode==NEXTSONG_PLAY_BUTTON)|
           (g_iPlaylistRepeat==PLAYLIST_REPEAT_ONE && g_wCurrentSongBad)|
           (iMode==NEXTSONG_FFWD))
        {//if we're not in repeat once mode, lets increment.
         //or, force an increment if NEXT button pressed, or FFWD over boundary
            g_iInternalTrack++;
        }
        if(g_iInternalTrack>=g_iTotalTracks)
        {//if we've hit the end of the list, start back over at the beginning
            g_iInternalTrack = 0;
            if((g_iPlaylistRepeat==PLAYLIST_REPEAT_ALL)|
              (iMode==NEXTSONG_PLAY_BUTTON)|
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

        //Lets now get the current song file info.  if we get an error condition
        //we'll pass it on, but we won't override an end of list from above
        if(Playlist_GetCurrentSongFileInfo(0,0,(void*)SongInfo)==PLAYLIST_END_OF_LIST) 
            rtn = PLAYLIST_END_OF_LIST;
    } else  // no tracks.
    {
      rtn = PLAYLIST_TRACK_NOT_FOUND;
    }

    return rtn;
}

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
//                  PLAYLIST_END_OF_LIST        
//
//   Notes:         
//<
//////////////////////////////////////////////////////
RETCODE _reentrant Playlist_GetCurrentSongFileInfo(INT iUnused1, INT iUnused2, INT *pPtr)
{
    SONGFILEINFO*SongInfo = (SONGFILEINFO*)pPtr;
    RETCODE rtn=PLAYLIST_END_OF_LIST;
    Playlist_UpdateShuffle();
    if(g_iTotalTracks){
        rtn = Playlist_PopulateSongFileInfo(SongInfo,g_PlayList[g_iInternalList[g_iInternalTrack]]);
		//now set bookmark to current song
		SysCallFunction(RSRC_PLAYERLIB_EXTRA_CODE_BANK,PlayerLib_SetBookmark,g_iPlaySet,g_iInternalList[g_iInternalTrack],0);
	}


    return rtn;
}

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
{
    SONGFILEINFO*SongInfo = (SONGFILEINFO*)pPtr;
    RETCODE rtn = PLAYLIST_END_OF_LIST;
    Playlist_UpdateShuffle();
    if(g_iTotalTracks)
    {
        if(g_iInternalTrack == 0)
            g_iInternalTrack = g_iTotalTracks;
        g_iInternalTrack--;

        rtn = Playlist_GetCurrentSongFileInfo(0,0,(void*)SongInfo);
    } else  // no tracks.
    {
      rtn = PLAYLIST_TRACK_NOT_FOUND;
    }
    return rtn;
}

//////////////////////////////////////////////////////
//
//>  Name:          _reentrant struct FileEntry *Playlist_GetFileEntry(void)
//
//   Type:          Function
//
//   Description:   gets an empty file entry from the pool
//
//   Inputs:        none
//
//   Outputs:       pointer to a unused file entry, or NULL if none exist
//
//   Notes:         
//<
//////////////////////////////////////////////////////
_reentrant struct FileEntry *Playlist_GetFileEntry(void)
//////////////////////////////////////////////////////
{
    int i;
    struct FileEntry *pEntry =NULL;
    for(i=0;i<PLAYLIST_MAX_FILES;i++)
    {
        if(g_FileEntryPool[i].m_iDecoder == FILE_ENTRY_UNUSED)
        {
            pEntry = &g_FileEntryPool[i];
            pEntry->m_pNext = NULL;
            pEntry->m_pContents = NULL;

            break;
        }
    }
    return pEntry;
}



//////////////////////////////////////////////////////
//
//>  Name:          _reentrant void Playlist_PopulateDirectory(struct FileEntry *pEntry, int depth)
//
//   Type:          Function
//
//   Description:   recursively populates the content tree starting at the entry pointed to by pEntry.
//
//   Inputs:        struct FileEntry *pEntry        points to a directory node to parse/populate
//                  int depth                       used to prevent it from scanning so deep it overflows some buffers
//
//   Outputs:       if the input has any contents, they will be included in the m_pContents node chain
//
//   Notes:         This function is a true reentrant/recursive function.   It will build a content tree
//                  onto the current file entry from the directory pointed toward in the file entry.
//              
//<
//////////////////////////////////////////////////////
#define ATTR_UNACCEPTABLE  ATTR_HIDDEN|ATTR_SYSTEM|ATTR_VOLUME_ID
_reentrant void Playlist_PopulateDirectory(struct FileEntry *pEntry, int depth)
{
    int iDevice = pEntry->m_iDevice;
    INT FindHandle = -1;
    if(FSChangeDir(pEntry->m_pszFilename,iDevice)>=0)
    {//change into the directory named in the filename string
        int bFirst = TRUE;
        int bSkipDir = FALSE;  // used to skip directories
        struct FileEntry *pCurrentEntry=pEntry;
        _packed BYTE sFileSpec[4];
        Finddata finddata;
        int i;
        ((int*)(sFileSpec))[0]=0x2a2e2a; //*.*
        ((int*)(sFileSpec))[1]=0; //null terminator

        finddata.device = iDevice;
        finddata.startrecord = 0;//start at the 0th record.

        //get the first that matches our specifications
///       if ((FindHandle = FindFirst(&finddata,(_packed char *)sFileSpec)) < 0)
////            return;
      FindHandle = FindFirst(&finddata,(_packed char *)sFileSpec);
       
      if(FindHandle == NO_MATCHING_RECORD)
          FindHandle=0;
      else if(FindHandle <0)
            return;
                      
       if(FindHandle > 0)
       {
        while(1)
        {//while our find handle is valid, keep getting more files
            if(!(finddata.attrib &(ATTR_UNACCEPTABLE)))
            {//if the attributes of the file found are acceptable (this will skip hidden, system, or volume id entries.  It also skips long file entries
                struct FileEntry *pNewEntry;
                int iFileNameLength;
                int i;
                _packed BYTE szTrash[9];

                pNewEntry = Playlist_GetFileEntry();
                if(pNewEntry)
                {//only parse further if we have a valid entry.  If we run out...we stop
                    pNewEntry->m_iFCBEntry = finddata.startrecord-1; //startrecord points one past the record we want
                    if(finddata.attrib&ATTR_DIRECTORY)
                    {   // Check subdirectory for MAC trash cans
                        // OS9 /TRASH/
                        // OSX /TRASHE~1/

                        //                      A R T
                        ((int*)szTrash)[0] = 0x415254;
                        //                        H S
                        ((int*)szTrash)[1] = 0x004853;
                        if(!packed_strcmp((_packed BYTE *)finddata.name,szTrash))
                        {  // OS9 trash directory
                            bSkipDir = TRUE; 
                        }
                        else 
                        {
                            //                      A R T
                            // ((int*)szTrash)[0] = 0x415254; // same as before
                            //                      E H S
                            ((int*)szTrash)[1] = 0x454853;
                            //                        1 ~
                            ((int*)szTrash)[2] = 0x00317E;
                            if(!packed_strcmp((_packed BYTE *)finddata.name,szTrash))
                            {   // OSX trash directory
                                bSkipDir = TRUE; 
                            }                                                             
                        }

                        if (!bSkipDir)
                        {
                            //if we're a valid directory, set the decoder to directory type
                            pNewEntry->m_iDecoder = FILE_ENTRY_DIR;
                        }
                        bSkipDir = FALSE;
                    }
                    else
                    {//otherwise, find out what the decoder would be.  We currently do this based on the extension.  The metadata functions
                        //might be a better solution.
                        int iDotIndex;
                        //lets guess about the dot index
                        iFileNameLength = packed_strlen((_packed BYTE *)finddata.name);
                        iDotIndex = iFileNameLength -4;
                        if(iDotIndex >= 0 )
                        { 
                            int iExtension=0;
                            //now look forward until we find the dot
                            while(packed_get((_packed BYTE *)finddata.name,iDotIndex++)!='.')
                            {
                                if(iDotIndex > iFileNameLength)
                                    break;
                            }

                            i=0;
                            while (iDotIndex < iFileNameLength)
                            {//ok, we've found the dot...lets copy the extension
                                packed_set((void*)&iExtension,i++,packed_get((_packed BYTE *)finddata.name,iDotIndex++));
                            }
                            //we've copied the extension, lets compare it to our known extensions.
                            switch(iExtension)
                            {
                                case 0x33504d://"MP3" in dislexic
                                    pNewEntry->m_iDecoder = RSRC_DECMOD_CODE;
                                    break;
                                case 0x564157://"WAV" in dislexic
                                    pNewEntry->m_iDecoder = RSRC_DEC_ADPCM_MOD_CODE;
                                    break;
                                case 0x414d57://"WMA" in dislexic
                                    pNewEntry->m_iDecoder = RSRC_WMADEC_CODE;
                                    break;

                            }

                            // if it's a Mac resource file associated with audio file remove it from the playlist
                            // if <= MacBytes AND SFN starts with a "_" AND audio file extension -- assume it's a Mac resource file
                            if(pNewEntry->m_iDecoder)
                            {   // if there's a decoder assigned, it's an audio file
                                if (packed_get((_packed BYTE *)finddata.name,0) == '_')
                                { // first character is _, so check size
                                    if (finddata.FileSize <= MAC_RESOURCE_NUM_BYTES)
                                    { // if <= 1kB, remove from playlist
                                        pNewEntry->m_iDecoder = FILE_ENTRY_UNUSED;
                                    }
                                }
                            } // end of Mac resource check

                        } // end of audio file check 
                    }   

                    if(pNewEntry->m_iDecoder !=FILE_ENTRY_UNUSED)
                    {//if it actually has a decoder (i.e. a file we can use)
                        struct FileEntry **hPreviousLink; 
                        pNewEntry->m_iDevice=iDevice;

                        packed_strcpy(pNewEntry->m_pszFilename,(_packed BYTE *)finddata.name);
                        if(pNewEntry->m_iDecoder==FILE_ENTRY_DIR)
                        {   //lets put the trailing slash onto the directory
                            i = 0x00002f;// equal to a forward slash.
                            packed_strcat(pNewEntry->m_pszFilename,(void*)&i);
                        }

                        hPreviousLink = &(pEntry->m_pContents);
                        pCurrentEntry = pEntry->m_pContents;
                        while(1) 
                        {//while we haven't found a place for it
                            if(pCurrentEntry)
                            {//if we have current file entry pointer pointer...see if our new one is before it
                                if((pCurrentEntry->m_iDecoder == pNewEntry->m_iDecoder) && packed_strcmp(pNewEntry->m_pszFilename,pCurrentEntry->m_pszFilename)<0)
                                    break;
                                if(pCurrentEntry->m_iDecoder==FILE_ENTRY_DIR && pNewEntry->m_iDecoder!=FILE_ENTRY_DIR )
                                    break;
                                hPreviousLink = &(pCurrentEntry->m_pNext);
                                pCurrentEntry = pCurrentEntry->m_pNext;
                            }
                            else
                            {//no, we're at the end of the list, lets break and insert the new file entry pointer
                                break;
                            }
                        }
                        //if we've gotten here, we've found where we want to insert it
                        *hPreviousLink = pNewEntry;
                        pNewEntry->m_pNext = pCurrentEntry;
                        pNewEntry->m_pContainer = pEntry;
                    }
                }
            }
            //now get the next FCB entry
            if(FindNext(FindHandle,&finddata)<0)
            {//if there's no next entry, lets close it out.
                break;
            }
        }
      }
        //go back and populate all of those entries that happen to be directories.
        pCurrentEntry = pEntry->m_pContents;
        while(pCurrentEntry)
        {
            if((pCurrentEntry->m_iDecoder == FILE_ENTRY_DIR) && (depth < _MAX_DIR_DEPTH))
            {
                Playlist_PopulateDirectory(pCurrentEntry, depth+1);
            }
            pCurrentEntry= pCurrentEntry->m_pNext;
        }
        
        //go back up one level so the filesystem is in the same state as when we entered it
        //                         . .
        ((int*)(sFileSpec))[0]=0x002e2e; 
        FSChangeDir(sFileSpec,iDevice);
    }
    else
    {
        pEntry->m_iDecoder=FILE_ENTRY_UNUSED;//return it to the pool since we can't change directories to it
    }
    Fclose(FindHandle);

}


//////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant Playlist_IsVoiceFile(struct FileEntry *pEntry)
//
//   Type:          Function
//
//   Description:   used to simplify the code for building the current play queue
//
//   Inputs:        struct FileEntry *pEntry        
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
BOOL _reentrant Playlist_IsVoiceFile(struct FileEntry *pEntry)
{
    BOOL rtn = FALSE;

    if(pEntry->m_iDecoder == RSRC_DEC_ADPCM_MOD_CODE)
    {//make sure its a wave
        if(pEntry->m_pContainer)
        {//make sure it has a container (This will cull the '/' directory)
            if(pEntry->m_pContainer->m_pContainer)
            {//make sure the container has a container--this would be ensure any file is at least one down
                if(!pEntry->m_pContainer->m_pContainer->m_pContainer)
                {//this ensures that this file is no more than one directory level deep
                    _packed BYTE szVoice[7];
                    //                      i o v
                    ((int*)szVoice)[0] = 0x494f56;
                    //                      \ e c
                    ((int*)szVoice)[1] = 0x2f4543;
                    ((int*)szVoice)[2] = 0;
                    if(!packed_strcmp(pEntry->m_pContainer->m_pszFilename,szVoice))
                    {//make sure its parent is the /VOICE/ directory
                        rtn = TRUE;
                    }
                }
            }
        }
    }
    
    return rtn;
}



//////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant Playlist_AddToQueue(struct FileEntry *pEntry)
//
//   Type:          Function
//
//   Description:   Adds to the play queue this entry if it is a song, or all of its 
//                  contents (recursively) if it is a directory
//
//   Inputs:        struct FileEntry *pEntry        
//
//   Outputs:       PLAYLIST_SUCCESS or !PLAYLIST_SUCCESS
//
//   Notes:         
//<
//////////////////////////////////////////////////////
RETCODE _reentrant Playlist_AddToQueue(struct FileEntry *pEntry)
{
    RETCODE rtn = !PLAYLIST_SUCCESS;
    if(g_iTotalTracks < PLAYLIST_MAX_FILES && pEntry->m_iDecoder != FILE_ENTRY_UNUSED)
    {//make sure we have room in the list and this file entry is either a directory or a song
        if(pEntry->m_iDecoder==FILE_ENTRY_DIR)
        {//if its a directory, go through all of its contents and add them to the list
            struct FileEntry *pCurrentEntry = pEntry->m_pContents;
            while(pCurrentEntry)
            {
                Playlist_AddToQueue(pCurrentEntry);
                pCurrentEntry = pCurrentEntry->m_pNext;
            }
        }
        else
        {
            int bVoice  = Playlist_IsVoiceFile(pEntry);
            if((g_iPlaySet == PLAYSET_MUSIC && !bVoice) ||
               (g_iPlaySet == PLAYSET_VOICE &&  bVoice)   )
            {//if we're in music mode, don't include voice files, if we're not in music mode, only include voice files.
                g_PlayList[g_iTotalTracks]= pEntry;
                g_iTotalTracks++;
            }
        }
            
        rtn = PLAYLIST_SUCCESS;
    }
    return rtn;
}

//////////////////////////////////////////////////////
//
//>  Name:          void _reentrant Playlist_BuildContentTree(void)
//
//   Type:          Function
//
//   Description:   clears out the old content tree and then cycles through 
//                  each logical device to build the new content tree.
//
//   Inputs:        
//
//   Outputs:       
//
//   Notes:         This function will change the current directory on each 
//                  device to the root.
//<
//////////////////////////////////////////////////////
void _reentrant Playlist_BuildContentTree(void)
{
    int i;

    // Use C Speed client  Increase call. Defines are in SysSpeed.inc. Sysspeed.h is generated
    SysSpeedIncrease(SPEED_MAX, SPEED_CLIENT_PLAYLIST1);

    //first lets clear out our list
    for(i=0;i<PLAYLIST_MAX_FILES;i++)
    {
        g_FileEntryPool[i].m_iDecoder=FILE_ENTRY_UNUSED;
    }

    for(i=0;i<MAX_LOGICAL_DEVICES;i++)
    {//cycle through the logical devices

        //initialize each device with the root directory.
        ((int*)(g_FileEntryPerDevice[i].m_pszFilename))[0] = 0x00002f;//"\"
        g_FileEntryPerDevice[i].m_iDevice = i;
        g_FileEntryPerDevice[i].m_pContainer = NULL;
        g_FileEntryPerDevice[i].m_pContents  = NULL;
        g_FileEntryPerDevice[i].m_pNext      = NULL;

        if(FSChangeDir(g_FileEntryPerDevice[i].m_pszFilename,i)>=0)
        {//change to the root, if successful, populate the content tree from that node
            g_FileEntryPerDevice[i].m_iDecoder=FILE_ENTRY_DIR;
            Playlist_PopulateDirectory(&g_FileEntryPerDevice[i],0);
        }
        else
        {//otherwise, mark it as unused.
            g_FileEntryPerDevice[i].m_iDecoder=FILE_ENTRY_UNUSED;
        }
    }


    SysSpeedClockFree(SPEED_CLIENT_PLAYLIST1); // C Speed client frees the shared speed resource  
#ifndef BATTERY_CHARGE	
	PWMB = PWM_SETUPB;
#endif	
}


//////////////////////////////////////////////////////
//
//>  Name:          void _reentrant Playlist_FillPlayQueue(void)
//
//   Type:          Function
//
//   Description:   cycles through each logical device and add the 
//                  top directory to the queue
//
//   Inputs:        
//
//   Outputs:       
//
//   Notes:         
//<
//////////////////////////////////////////////////////
void _reentrant Playlist_FillPlayQueue(void)
{
    int i;
    //empty out the play queue
    g_iTotalTracks=0;
    //cycle through each logical device and add songs in that branch to the queue
    for(i=0;i<MAX_LOGICAL_DEVICES;i++)
    {
        Playlist_AddToQueue(&g_FileEntryPerDevice[i]);
    }
    //lets start at the first song.
    g_iInternalTrack = 0;

    g_wTotalSongCount = (WORD)g_iTotalTracks;
}


//////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant Playlist_Initialize(void)
//
//   Type:          Function
//
//   Description:   
//
//   Inputs:        
//
//   Outputs:       PLAYLIST_SUCCESS
//
//   Notes:         
//<
//////////////////////////////////////////////////////
#ifdef TUNER_STFM1000
RETCODE _reentrant Playlist_Initialize(int bDontGetCurrentSong, int b, int *c)
{
    int i;
    b;c;
#else
RETCODE _reentrant Playlist_Initialize(int a, int b, int *c)
{
    int i;
    a;b;c;
#endif

    Playlist_BuildContentTree();
    Playlist_SetPlaySet(g_iPlaySet,0,0);
#ifdef TUNER_STFM1000
	if (!bDontGetCurrentSong)
#endif

	SysCallFunction(RSRC_PLAYERLIB_EXTRA_CODE_BANK,PlayerLib_GetBookmarkSongInfo,g_iPlaySet,0,0);
	//no need for lower call. Above call will take care of PlayerLib_GetCurrentSong and 
	//decide whether it needs to go to bookmarked song or not.
    //SysCallFunction(RSRC_PLAYERLIB_EXTRA_CODE_BANK,PlayerLib_GetCurrentSong,0,0,0);

    return PLAYLIST_SUCCESS;
}

//////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant Playlist_LFNGetFileName()
//
//   Type:          Function
//
//   Description:   
//
//   Inputs:        1. Track Number
//                  2. Pointer to UCS3 buffer: Long File Name will be copied here!
//
//   Outputs:       Length of string: 0-255
//
//   Notes:         1) 8.3 checksum is not checked on LFN Entries to verify...
//                  2) callable from SysCallFunction
//<
//////////////////////////////////////////////////////

RETCODE _reentrant Playlist_LFNGetFileName(int TrackNumber, int b, UCS3 *LFNFileName){

   int retcode;
   int CurrentDirHandleNum;
   LFN_FILE_LOCATOR LFNFileLocator;
   struct FileEntry *LFNFile = 0;

   /* obtain the correct entry in the playlist */
   LFNFile = g_PlayList[g_iInternalList[TrackNumber]];

   /* get the correct device */
   LFNFileLocator.bDevice = LFNFile->m_iDevice;
   LFNFileLocator.wFCBEntry = LFNFile->m_iFCBEntry;   

   /* go to the directory where the file exists */
   if(ChangeDIRtoFileEntryDir(LFNFile, (_packed BYTE *) LFNFileName) != PLAYLIST_SUCCESS){   /* using same buffer for the path as for the long file name. */
      LFNFileName[0] = 0;     /* failed to change directory so display SFN instead */
      return 0;
   }

   /* Get the Current Dir handle */
   CurrentDirHandleNum = GetCWDHandle();
   if(0 > CurrentDirHandleNum)
   {
      /* Could not get the Current Dir handle */
      return(0);
   }

   /* Construct the filename */
   retcode = ConstructLongFileName(CurrentDirHandleNum, 
                                    LFNFile->m_iFCBEntry, (INT *)LFNFileName);
   if(0 >= retcode){
      Fclose(CurrentDirHandleNum);
      /* Unable to Construct */
      return(0);
   }

   /* return the handle */
   Fclose(CurrentDirHandleNum);

   /* Sucessful */
   return(CurrentDirHandleNum);
}







//////////////////////////////////////////////////////
//
//>  Name:          WORD ChangeDIRtoFileEntryDir(struct FileEntry *pCurrentEntry)
//
//   Type:          Function
//
//   Description:   indexed starting 0
//
//   Inputs:        
//
//   Outputs:       returns PLAYLIST_SUCCESS if Dir is changed else PLAYLIST_END_OF_LIST
//
//   Notes: zero is returned if the cahracter is 0xFFFF         
//<
//////////////////////////////////////////////////////
RETCODE ChangeDIRtoFileEntryDir(struct FileEntry *pCurrentEntry, _packed BYTE * buffer)
{
    int iPathPieceIndex = 0;
    _packed BYTE bTrailingSlash[] = "\0\0/";	
    RETCODE rtn;
    struct FileEntry * pPathPieces[_MAX_DIR_DEPTH];	//buffer to store the directory names in
    BYTE iDevice = pCurrentEntry->m_iDevice;
    pCurrentEntry = pCurrentEntry->m_pContainer;	//point to the directory of which the file is in
    buffer[0] = 0x000000;    
    //this block of code will generate an array of file entries/nodes in the content tree that walk up to the root.
    while(pCurrentEntry)  //while we are not at the root level
    {//note, you could exceed the array of pPathPieces with this! (no bounds checking)
        pPathPieces[iPathPieceIndex++] = pCurrentEntry;
        pCurrentEntry = pCurrentEntry->m_pContainer;
        if(iPathPieceIndex >= _MAX_DIR_DEPTH)
        {
            rtn = PLAYLIST_END_OF_LIST;
            iPathPieceIndex=0;
			break;
        }
    }

    //we traverse this array backwards to build up the fully specified path name.
    while(iPathPieceIndex)
    {
        //directory SFN already end in a '/' in the playlist structure
        iPathPieceIndex --;	  //decremet b/c of the post increments from above
		packed_strcat(buffer,pPathPieces[iPathPieceIndex]->m_pszFilename);
    }
	FSChangeDir(buffer,iDevice);
	return PLAYLIST_SUCCESS;
}
#pragma asm
    nolist
        include "resource.inc"
        include "sysmacro.asm"
    list
    org y,"Player_settings_Y":
    extern y:Fg_RandWorkingValue

    SaveRange Fg_RandWorkingValue,1
    
#pragma endasm
