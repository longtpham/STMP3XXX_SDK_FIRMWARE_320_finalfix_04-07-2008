#include "types.h"
#include "exec.h"
#include "fsapi.h"
#include "fserror.h"
#include "stringlib.h"
#include "playlist.h"
#include "extern.h"
#include "playlist5.h"

#pragma optimize 1
////////////////////////////////////////////////////////////
// extern variables
////////////////////////////////////////////////////////////
extern unsigned int _Y g_wCurrentSongNumber;
extern unsigned int _Y g_wTotalSongCount;
extern INT g_is_SD_inserted;
////////////////////////////////////////////////////////////
// prototypes
////////////////////////////////////////////////////////////
_reentrant INT PL5_Chdir (_packed char *filepath, INT RecordNumber);
_reentrant INT PL5_PL_CdSearching_Fast(void);
static _reentrant INT PL5_PL_CdSearching_Slow (PL5_PL_FOLDER * folder);
static _reentrant BOOL PL5_PL_NextFile (void);
static _reentrant BOOL PL5_PL_NextFolder (void);
static _reentrant BOOL PL5_PL_PrevFile (void);
static _reentrant BOOL PL5_PL_PrevFolder (void);
static _reentrant BOOL PL5_PL_LastItem (void);
static _reentrant BOOL PL5_PL_LastItemInLast (void);
static _reentrant BOOL PL5_PL_Count (void);
_reentrant void PL5_PL_QueuePushBack (PL5_FOLDER_RECORD * record);
_reentrant void PL5_PL_QueuePushFront (PL5_FOLDER_RECORD * record);
static _reentrant BOOL PL5_PL_ValidFile (Finddata * pFinddata, BOOL counting);
static _reentrant BOOL PL5_PL_ValidFolder (Finddata * pFinddata, BOOL counting);

////////////////////////////////////////////////////////////
// glocal variables
////////////////////////////////////////////////////////////
PL5_PLAYBACK    g_PL5_Playback;
PL5_PL_QUEUE    g_PL5_PL_queue;
PL5_PL_SEARCH   g_PL5_PL_search;
PL5_PL_QITEM    g_PL5_PL_buffer[PL5_PL_QUEUE_SIZE];
PL5_PL_FOLDER   g_PL5_PL_searchingfolders[MAX_DIR_DEPTH+1];
_packed BYTE    g_PL5_PL_searchingpath[MAX_FILENAME_LENGTH];
PL5_PL_FOLDER   g_PL5_PL_searchingfolders_cnt[MAX_DIR_DEPTH+1];
_packed BYTE    g_PL5_PL_searchingpath_cnt[MAX_FILENAME_LENGTH];
int             g_piPlayExtSupported[MAX_EXTS];

int g_iRandomNum;

PL5_FOLDER_RECORD g_PL5_PlaybackTmpBfr[PL5_TMP_BUFFER_SIZE];
int g_PL5_PlaybackTmpBfrIdx;

int g_RealCurrenNum = 0;
extern int g_RandomOrder[PL5_PL_RAND_BUF_SIZE];
extern int g_SongIdx;


////////////////////////////////////////////////////////////
// funcstions definitions
////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------

   Function Name:  _reentrant INT PL5_PL_CdSearching_Fast(void)

   FunctionType:   Reentrant

   Inputs:        None

   Outputs:       FS_SUCCESS OR error code if error occurs

   Description:  Try to fast change directory to searching folder. If current working directory is
                 right the distination, do nothing and return; if current working directory is the
                 a parrent of destination, Chdir to destination using relativce path; otherwise,
                 Chdir to destination using full path.

----------------------------------------------------------------------------*/
_reentrant INT PL5_PL_CdSearching_Fast(void)
{
    PL5_PL_FOLDER * tPath;
    _packed BYTE * sPath;
    int top, depth;
    int ret = FS_SUCCESS;

    if (g_PL5_PL_search.phase == PL5_COUNT)
    {
        tPath = g_PL5_PL_search.tPath_cnt;
        sPath = g_PL5_PL_search.sPath_cnt;
        depth = g_PL5_PL_search.depth_cnt;
    }
    else
    {
        tPath = g_PL5_PL_search.tPath;
        sPath = g_PL5_PL_search.sPath;
        depth = g_PL5_PL_search.depth;
    }

    if (depth < 2)
    {
        __asm("debug"); // depth can not be smaller than 2.
    }

    if (packed_strcmp((_packed BYTE*)gCurrentWorkingdirectory, sPath) != 0)
    {
        _packed char tmppath1[SFN_LENGTH];
        _packed char tmppath2[SFN_LENGTH];
        _packed char byte1, byte2;
        int offset1, offset_dest1, offset2, offset_dest2;
        int k;

        offset1 = offset_dest1 = offset2 = offset_dest2 = 0;
        k = 1;
        byte1 = byte2 = 1;
        while ((byte1 != 0) || (byte2 != 0))
        {
            while (byte1 != 0)
            {
                byte1 = GetChar((_packed char*)gCurrentWorkingdirectory, &offset1);
                if ((byte1 == 0x2f) || (byte1 == 0))
                    break;
                PutChar(tmppath1, &offset_dest1,byte1);
            }
            PutChar(tmppath1, &offset_dest1, 0);    //null terminator

            while (byte2 != 0)
            {
                byte2 = GetChar((_packed char *)sPath, &offset2);
                if ((byte2 == 0x2f) || (byte2 == 0))
                    break;
                PutChar(tmppath2, &offset_dest2,byte2);
            }
            PutChar(tmppath2, &offset_dest2, 0);    //null terminator

            if (packed_strcmp((_packed BYTE*)tmppath1, (_packed BYTE*)tmppath2) == 0)
            {
                offset_dest1 = offset_dest2 = 0;
                k++;
            }
            else
            {
                // if gCurrentWorkingdirectory is a parrent of browsing path, we change direcotry to
                // browsing directory using relative path, otherwise using full path.
                if (offset_dest1 != 1)
                    k = 1;  // gCurrentWorkingdirectory is not parrent of browsing path
                break;
            }
        }

        for( ; k < depth; k++)
        {
            ret = PL5_PL_CdSearching_Slow(tPath+k);
            if (ret != FS_SUCCESS)
                break;
        }
    }

    return ret;
}


/*----------------------------------------------------------------------------

   Function Name:  static _reentrant INT PL5_PL_CdSearching_Slow (PL5_PL_FOLDER * folder)

   FunctionType:   Reentrant

   Inputs:        1) pointer to folder descriptor

   Outputs:       FS_SUCCESS OR error code if error occurs

   Description:  Change directory to searching folder. Precondition is that the destination
                folder is a subfolder of current working directory
----------------------------------------------------------------------------*/
static _reentrant INT PL5_PL_CdSearching_Slow (PL5_PL_FOLDER * folder)
{
    _packed char pszPath[SFN_LENGTH];
    PL5_FOLDER_RECORD * record;

    record = &(folder->record);
    if (Playlist_GetSFN (record, (_packed BYTE *)pszPath) != PLAYLIST_SUCCESS)
    {
        __asm("debug");
    }
    return PL5_Chdir(pszPath, record->iRecordNum);
}


/*----------------------------------------------------------------------------

   Function Name:  _reentrant void PL5_PL_Do_Search (void)

   FunctionType:   Reentrant

   Inputs:        None

   Outputs:       None

   Description:  A playback search state machine

----------------------------------------------------------------------------*/
_reentrant void PL5_PL_Do_Search (void)
{
    BOOL end = FALSE;
    int result;

    /* Change to searching folder */
    if (PL5_PL_CdSearching_Fast() != FS_SUCCESS)
        __asm("debug");

    /* do search */
    do {
        switch (g_PL5_PL_search.phase)
        {
            case PL5_NEXT_FILE:
                end = PL5_PL_NextFile();
                break;
            case PL5_PREV_FILE:
                end = PL5_PL_PrevFile();
                break;
            case PL5_NEXT_FOLDER:
                end = PL5_PL_NextFolder();
                break;
            case PL5_PREV_FOLDER:
                end = PL5_PL_PrevFolder();
                break;
            case PL5_LAST:
                end = PL5_PL_LastItem();
                break;
            case PL5_LASTINLAST:
                end = PL5_PL_LastItemInLast();
                break;
            case PL5_COUNT:
                end = PL5_PL_Count();
                break;
            default:
                __asm("debug");     // Unsupported phase
                end = TRUE;
                break;
        }
    } while (g_PL5_PL_search.foregnd && !end);

    if (end == TRUE)
    {
        if (g_PL5_Playback.foundall)
            g_PL5_PL_search.status = PL5_STATE_IDLE;
        else
        {
            g_PL5_PL_search.phase = PL5_COUNT;
        }
        g_PL5_PL_search.foregnd = FALSE;
    }

	// To prevent g_wTotalSongCount equal to 0 when message MENU_MSG_PL5_PLAY_SONG was post.
	if((g_PL5_Playback.total != 0) || (g_PL5_Playback.foundall == TRUE) || (g_PL5_PL_queue.tail == g_PL5_PL_queue.head))
	    g_wTotalSongCount = g_PL5_Playback.total;
}

/*----------------------------------------------------------------------------

   Function Name:  static _reentrant BOOL PL5_PL_NextFile (void)

   FunctionType:   Reentrant

   Inputs:        None

   Outputs:       search complete or not

   Description:  search next file

----------------------------------------------------------------------------*/
static _reentrant BOOL PL5_PL_NextFile (void)
{
    PL5_PL_FOLDER * current;
    Finddata finddata;
    int find_handle ;
    _packed char sFileSpec[4];
    PL5_FOLDER_RECORD tmp_dirrec;
    LONG iFinishTime = 0;
    BOOL end = FALSE;
    BOOL nextfolder = FALSE;
	int margin;

	// When init root, the g_PL5_PL_queue.size is set as 2 for searching a file only.
	if(g_PL5_PL_queue.size == 2)
		margin = 0;
	else
		margin = PL5_PL_NEXT_MARGIN;
    current = g_PL5_PL_search.tPath + g_PL5_PL_search.depth - 1;    // current searching folder

    ((int*)(sFileSpec))[0] = 0x2a2e2a; // "*.*"
    ((int*)(sFileSpec))[1] = 0; //null terminator

    tmp_dirrec.iDevice = current->record.iDevice;
    tmp_dirrec.bIsRoot = FALSE;
    tmp_dirrec.bIsDir = FALSE;

    if (current->firstfile < 0)
    {
        // No valid file
        nextfolder = TRUE;
    }
    else
    {
        if (!g_PL5_PL_search.foregnd)
            iFinishTime = SysGetCurrentTime() + PL5_PL_SEARCH_TIME_SLICE;

        _memset(&finddata,0,sizeof(finddata));
        finddata.device = current->record.iDevice;
        finddata.startrecord = g_PL5_PL_search.start;
        find_handle = FindFirst(&finddata, sFileSpec);

        if (find_handle <= 0)
        {
            Fclose(find_handle);
            if (current->firstfile == 0)
                current->firstfile = -1;
            nextfolder = TRUE;
        }
        else
        {
            while(1)
            {
                if (PL5_PL_ValidFile(&finddata, FALSE))
                {
                    if (((g_PL5_PL_queue.tail > g_PL5_Playback.currentplay) && ((g_PL5_PL_queue.tail - g_PL5_Playback.currentplay) >= (margin+1)))
                     || ((g_PL5_PL_queue.tail < g_PL5_Playback.currentplay) && ((g_PL5_PL_queue.tail + g_PL5_PL_queue.size - g_PL5_Playback.currentplay) >= (margin+1))))
                    {
                        // Enough items have been found, don't insert this item to playback queue and complete search.
                        end = TRUE;
                        Fclose(find_handle);
                        break;
                    }
					
                    g_PL5_PL_search.tracknum ++;

                    if (current->firstfile == 0)
                        current->firstfile = finddata.startrecord - 1;
                    if (current->lastfile < (finddata.startrecord - 1))
                        current->lastfile = finddata.startrecord - 1;

					if((g_PL5_PL_search.tracknum > g_PL5_Playback.total) && g_PL5_Playback.foundall)
						g_PL5_PL_search.tracknum -= g_PL5_Playback.total;
					
					if((!g_bPlaylistShuffle) || (g_PL5_PL_search.tracknum == g_RandomOrder[g_SongIdx]))
					{
	                    // insert this item into playback queue
	                    tmp_dirrec.iRecordNum = finddata.startrecord - 1;
	                    tmp_dirrec.lFastKey = finddata.Key;
	                    PL5_PL_QueuePushBack (&tmp_dirrec);
					}
						
    
                }
                else if (PL5_PL_ValidFolder(&finddata, FALSE))
                {
                    if (current->firstfolder == 0)
                        current->firstfolder = finddata.startrecord - 1;
                    if (current->lastfolder < (finddata.startrecord - 1))
                        current->lastfolder = finddata.startrecord - 1;
                }

                /* check timeout if this is a background search. */
                if (!g_PL5_PL_search.foregnd && (SysGetCurrentTime() >= iFinishTime))
                {
                    // timeout occur, search module must release execution for other module
                    // save next start point
                    g_PL5_PL_search.start = finddata.startrecord;
                    Fclose(find_handle);
                    break;
                }

                //now get the next record
                if((finddata.startrecord >= MAX_RECORDS_PER_DIRECTORY) || (FindNext(find_handle, &finddata) < 0))
                {
                    // No next file
                    nextfolder = TRUE;
                    if (current->firstfile == 0)
                        current->firstfile = -1;
                    if (current->firstfolder == 0)
                        current->firstfolder = -1;
                    Fclose(find_handle);
                    break;
                }
            }
        }
    }

    if (nextfolder)
    {
        // No valid file.
        current->foundall = TRUE;
#ifdef JPEG_APP	
		if(g_ePL5BrowseType == PL5_PHOTOTYPE)
			g_PL5_Playback.mode = PL5_MODE_PLAYFILES;
#endif
#ifdef MOTION_VIDEO		
		if(g_ePL5BrowseType == PL5_VIDEOTYPE)
			g_PL5_Playback.mode = PL5_MODE_PLAYFILES;
#endif
        if (g_PL5_Playback.mode == PL5_MODE_PLAYFOLDERS)
        {   // This mode plays files and subfolders in selected folder.
            g_PL5_PL_search.start = current->firstfolder;
            g_PL5_PL_search.phase = PL5_NEXT_FOLDER;
        }
        else
        {   // This mode play files in selected folder only.
            if (current->firstfile >= 0)
            {   // there is at least one valid file in the folder
                g_PL5_PL_search.start = current->firstfile;
                g_PL5_PL_search.phase = PL5_NEXT_FILE;
            }
            else
            {   // No valid file in the folder, search complete.
                g_PL5_Playback.foundall = TRUE;
                g_PL5_Playback.total = 0;
                end = TRUE;
            }

            // search wrap around occur, reset tracknum to 0
            g_PL5_PL_search.tracknum = 0;
        }
    }

    return end;
}


/*----------------------------------------------------------------------------

   Function Name:  static _reentrant BOOL PL5_PL_NextFolder (void)

   FunctionType:   Reentrant

   Inputs:        None

   Outputs:       search complete or not

   Description:  search next folder. This is called when we need search next file
                in other folder.

----------------------------------------------------------------------------*/
static _reentrant BOOL PL5_PL_NextFolder (void)
{
    PL5_PL_FOLDER * current;
    Finddata finddata;
    int find_handle ;
    _packed char sFileSpec[4];
    PL5_FOLDER_RECORD tmp_dirrec;
    LONG iFinishTime = 0;
    BOOL end = FALSE;
    BOOL nextfolder = FALSE;

    current = g_PL5_PL_search.tPath + g_PL5_PL_search.depth - 1;    // current searching folder

    ((int*)(sFileSpec))[0] = 0x2a2e2a; // "*.*"
    ((int*)(sFileSpec))[1] = 0; //null terminator

    if (current->firstfolder < 0)
    {
        // No valid folder
        nextfolder = TRUE;
    }
    else
    {
        if (!g_PL5_PL_search.foregnd)
            iFinishTime = SysGetCurrentTime() + PL5_PL_SEARCH_TIME_SLICE;

        _memset(&finddata,0,sizeof(finddata));
        finddata.device = current->record.iDevice;
        finddata.startrecord = g_PL5_PL_search.start;
        find_handle = FindFirst(&finddata, sFileSpec);

        if (find_handle <= 0)
        {
            Fclose(find_handle);
            nextfolder = TRUE;
        }
        else
        {
            while(1)
            {
                if (PL5_PL_ValidFolder(&finddata, FALSE))
                {
                    /* Next valid folder found. Chdir to next folder and update search to search next file. */
                    Fclose(find_handle);
                    tmp_dirrec.iDevice = finddata.device;
                    tmp_dirrec.bIsRoot = FALSE;
                    tmp_dirrec.bIsDir = TRUE;
                    tmp_dirrec.iRecordNum = finddata.startrecord - 1;
                    tmp_dirrec.lFastKey = finddata.Key;
                    current++;
                    _memset(current, 0, sizeof(PL5_PL_FOLDER));
                    current->record = tmp_dirrec;
                    if (PL5_PL_CdSearching_Slow(current) == FS_SUCCESS)
                    {
                        packed_strcpy (g_PL5_PL_search.sPath, (_packed BYTE*)gCurrentWorkingdirectory);
                        g_PL5_PL_search.depth ++;
                        g_PL5_PL_search.phase = PL5_NEXT_FILE;
                        g_PL5_PL_search.start = 0;
                    }
                    else
                    {
                        __asm("debug");
                        return TRUE;
                    }
                    break;
                }

                /* check timeout if this is a background search. */
                if (!g_PL5_PL_search.foregnd && (SysGetCurrentTime() >= iFinishTime))
                {
                    // timeout occur, search module must release execution for other module
                    // save next start point
                    g_PL5_PL_search.start = finddata.startrecord;
                    Fclose(find_handle);
                    break;
                }

                //now get the next record
                if((finddata.startrecord >= MAX_RECORDS_PER_DIRECTORY) || (FindNext(find_handle, &finddata) < 0))
                {
                    // No next file
                    Fclose(find_handle);
                    nextfolder = TRUE;
                    break;
                }
            }
        }
    }

    if (nextfolder)
    {   // there is no next subfolder in current folder, we will return to parrent folder to repeat searching
        if (g_PL5_PL_search.depth  > (g_PL5_PL_search.top+1))
        {
            if (g_PL5_PL_search.depth == 2)
            {   // current is in root folder
                if (current->record.iRecordNum == 0)
                {   // a: drive completed. We will enter b: drive if b: drive exist, otherwise search complete.
#ifdef MMC
                    if (g_is_SD_inserted == HAS_SD)
                    {
                        tmp_dirrec.iDevice = 1;
                        tmp_dirrec.bIsRoot = TRUE;
                        tmp_dirrec.bIsDir = TRUE;
                        tmp_dirrec.iRecordNum = 1;
                        tmp_dirrec.lFastKey = GetRootdirkey(1);
                        _memset(current, 0, sizeof(PL5_PL_FOLDER));
                        current->record = tmp_dirrec;
                        if (PL5_PL_CdSearching_Slow(current) == FS_SUCCESS)
                        {
                            packed_strcpy(g_PL5_PL_search.sPath, (_packed BYTE*)gCurrentWorkingdirectory);
                            g_PL5_PL_search.phase = PL5_NEXT_FILE;
                            g_PL5_PL_search.start = 0;
                        }
                        else
                        {
                            __asm("debug");
                            return TRUE;
                        }
                    }
                    else
#endif
			        {   // Here is the end of a: driver, search from the first file again.
			            if(g_PL5_PL_queue.head != g_PL5_PL_queue.tail)
						{
				            // Here is the end of a: driver, search from the first file again.
				        	g_PL5_PL_search.tracknum = 0;
				            g_PL5_PL_search.start = current->firstfile;
				            g_PL5_PL_search.phase = PL5_NEXT_FILE;
	                    }
						else
			            {   // No valid file in the folder, search complete.
			                g_PL5_Playback.foundall = TRUE;
			                g_PL5_Playback.total = 0;
			                end = TRUE;
			            }
                    }
                }
#ifdef MMC				
                else
                {   // b: drive completed. search from the first file of a: driver again..
		            if(g_PL5_PL_queue.head != g_PL5_PL_queue.tail)
					{
	                    tmp_dirrec.iDevice = 0;
	                    tmp_dirrec.bIsRoot = TRUE;
	                    tmp_dirrec.bIsDir = TRUE;
	                    tmp_dirrec.iRecordNum = 0;
	                    tmp_dirrec.lFastKey = GetRootdirkey(0);
	                    _memset(current, 0, sizeof(PL5_PL_FOLDER));
	                    current->record = tmp_dirrec;
	                    if (PL5_PL_CdSearching_Slow(current) == FS_SUCCESS)
	                    {
	                        packed_strcpy(g_PL5_PL_search.sPath, (_packed BYTE*)gCurrentWorkingdirectory);
				        	g_PL5_PL_search.tracknum = 0;
	                        g_PL5_PL_search.phase = PL5_NEXT_FILE;
	                        g_PL5_PL_search.start = 0;
	                    }
	                    else
	                    {
	                        __asm("debug");
	                        return TRUE;
		                }
                	}
					else
		            {   // No valid file in the folder, search complete.
		                g_PL5_Playback.foundall = TRUE;
		                g_PL5_Playback.total = 0;
		                end = TRUE;
		            }
                }
#endif				
            }
            else
            {   // return to parrent folder and search next folder.
                g_PL5_PL_search.depth --;
                ((int*)(sFileSpec))[0] = 0x002e2e;
                if (PL5_Chdir(sFileSpec, 0) != FS_SUCCESS)
                    __asm("debug");
                packed_strcpy(g_PL5_PL_search.sPath, (_packed BYTE*)gCurrentWorkingdirectory);
                g_PL5_PL_search.start = current->record.iRecordNum + 1;
            }
        }
        else
        {
			/*
			// Here is the end of top folder, search from the first file again.
        	if(current->firstfile >= 0)
        	{
	        	g_PL5_PL_search.tracknum = 0;
	            g_PL5_PL_search.start = current->firstfile;
	            g_PL5_PL_search.phase = PL5_NEXT_FILE;
        	}
            else
            {   // No valid file in the folder, search complete.
                g_PL5_Playback.foundall = TRUE;
                g_PL5_Playback.total = 0;
                end = TRUE;
            }
            */
            if(g_PL5_PL_queue.head == g_PL5_PL_queue.tail)
            {   // No valid file in the folder, search complete.
                g_PL5_Playback.foundall = TRUE;
                g_PL5_Playback.total = 0;
                end = TRUE;
            }
			else
			{
	        	g_PL5_PL_search.tracknum = 0;
	            g_PL5_PL_search.start = current->firstfile;
	            g_PL5_PL_search.phase = PL5_NEXT_FILE;
			}
			
        }

        if (end)
        {   // search wrap around occur, reset tracknum to 0
            g_PL5_PL_search.tracknum = 0;
        }
    }

    return end;
}

/*----------------------------------------------------------------------------

   Function Name:  static _reentrant BOOL PL5_PL_PrevFile (void)

   FunctionType:   Reentrant

   Inputs:        None

   Outputs:       search complete or not

   Description:  search previous file

----------------------------------------------------------------------------*/
static _reentrant BOOL PL5_PL_PrevFile (void)
{
    PL5_PL_FOLDER * current;
    Finddata finddata;
    int find_handle ;
    _packed char sFileSpec[4];
    PL5_FOLDER_RECORD tmp_dirrec;
    LONG iFinishTime = 0;
    BOOL end = FALSE;
    BOOL found = FALSE;
    BOOL prevfolder = FALSE;

    current = g_PL5_PL_search.tPath + g_PL5_PL_search.depth - 1;    // current searching folder

    ((int*)(sFileSpec))[0] = 0x2a2e2a; // "*.*"
    ((int*)(sFileSpec))[1] = 0; //null terminator

    if (current->firstfile < 0)
    {
        // No valid file
        prevfolder = TRUE;
    }
    else
    {
        if (!g_PL5_PL_search.foregnd)
            iFinishTime = SysGetCurrentTime() + PL5_PL_SEARCH_TIME_SLICE;

        tmp_dirrec.iDevice = current->record.iDevice;
        tmp_dirrec.bIsRoot = FALSE;
        tmp_dirrec.bIsDir = FALSE;

        while(!found)
        {
            _memset(&finddata,0,sizeof(finddata));
            finddata.device = current->record.iDevice;
            if (g_PL5_PL_search.start > (current->firstfile + PL5_TMP_BUFFER_SIZE))
                finddata.startrecord = g_PL5_PL_search.start - PL5_TMP_BUFFER_SIZE;
            else
                finddata.startrecord = current->firstfile;

            find_handle = FindFirst(&finddata, sFileSpec);
            if (find_handle <= 0)
            {
                __asm("debug"); // code should not reach here. A FS error occur
                Fclose(find_handle);
                return TRUE;
            }

            while(1)
            {
                if (finddata.startrecord > g_PL5_PL_search.start)
                {
                    if (g_PL5_PlaybackTmpBfrIdx == 0)
                    {
                        if (g_PL5_PL_search.start > (current->firstfile + PL5_TMP_BUFFER_SIZE))
                        {
                            // no valid previous file found in the last PL5_TMP_BUFFER_SIZE records
                            // reset start point to more previous
                            g_PL5_PL_search.start -= PL5_TMP_BUFFER_SIZE;

                            /* check timeout if this is a background search. */
                            if (!g_PL5_PL_search.foregnd && (SysGetCurrentTime() >= iFinishTime))
                            {
                                // timeout occur, search module must release execution for other module
                                found = TRUE;      // set found TRUE to exit while loop.
                            }
                        }
                        else
                        {
                            // no valid previous file any more
                            prevfolder = TRUE;
                            found = TRUE;   // set found TRUE to exit while loop.
                        }
                    }
                    else
                    {   // At least one previous file was found.
                        found = TRUE;
                    }

                    Fclose(find_handle);
                    break;
                }

                if (PL5_PL_ValidFile(&finddata, FALSE))
                {
                    tmp_dirrec.iRecordNum = finddata.startrecord - 1;
                    tmp_dirrec.lFastKey = finddata.Key;
                    // Insert to the end of the temp queue
                    g_PL5_PlaybackTmpBfr[g_PL5_PlaybackTmpBfrIdx++] = tmp_dirrec;
                    if (g_PL5_PlaybackTmpBfrIdx > PL5_TMP_BUFFER_SIZE)
                    {
                        __asm("debug");     // Code should not reach here. An unknown error occur
                        Fclose(find_handle);
                        g_PL5_PlaybackTmpBfrIdx = 0;
                        return TRUE;
                    }
                }

                //now get the next record
                if(FindNext(find_handle, &finddata) < 0)
                {
                    //no next entry
                    found = TRUE;
                    finddata.startrecord = g_PL5_PL_search.start + 1;
                }
            }
        }
    }

    if (found)
    {
		int LastCurrentplay;
		// g_PL5_Playback.currentplay minused one before search.
		LastCurrentplay = (g_PL5_Playback.currentplay + 1) % g_PL5_PL_queue.size;
        while(g_PL5_PlaybackTmpBfrIdx != 0)
        {
            if (((g_PL5_PL_queue.head < LastCurrentplay) && ((LastCurrentplay - g_PL5_PL_queue.head) >= PL5_PL_PREV_MARGIN))
             || ((g_PL5_PL_queue.head > LastCurrentplay) && ((LastCurrentplay + g_PL5_PL_queue.size - g_PL5_PL_queue.head) >= PL5_PL_PREV_MARGIN)))
            {   // Enough previous files have been found, search complete.
                g_PL5_PlaybackTmpBfrIdx = 0;
                end = TRUE;
                break;
            }

            g_PL5_PL_search.tracknum--;
            g_PL5_PlaybackTmpBfrIdx--;
			
			if((g_PL5_PL_search.tracknum <= 0) && g_PL5_Playback.foundall)
				g_PL5_PL_search.tracknum += g_PL5_Playback.total;
			
			if((!g_bPlaylistShuffle) || (g_PL5_PL_search.tracknum == g_RandomOrder[g_SongIdx]))
			{
                // insert this item into playback queue
	            PL5_PL_QueuePushFront(&g_PL5_PlaybackTmpBfr[g_PL5_PlaybackTmpBfrIdx]);
				// If next searched previous file will fill to the place of currentplay,stop filling.
		        if (((((g_PL5_Playback.currentplay + 1) % g_PL5_PL_queue.size) == g_PL5_PL_queue.head) && (g_PL5_PlaybackTmpBfrIdx > 0))
						|| g_bPlaylistShuffle)
		        {
	                g_PL5_PlaybackTmpBfrIdx = 0;
					end = TRUE;
					break;
		        }
			}
			else if(g_PL5_PlaybackTmpBfrIdx == 0)
			{
				g_PL5_PL_search.start = g_PL5_PlaybackTmpBfr[0].iRecordNum;
			}
        }
    }
    if (prevfolder)
    {   // there is no previous file in current folder, we will return to parrent folder to repeat searching
        if (g_PL5_Playback.mode == PL5_MODE_PLAYFOLDERS)
        {   // This mode plays files and subfolders in selected folder.
            if (g_PL5_PL_search.depth  > (g_PL5_PL_search.top+1))
            {
                if (g_PL5_PL_search.depth == 2)
                {   // current is in root folder
                    if (current->record.iRecordNum == 0)
                    {   // a: drive completed. Search from last in last of b: driver.
#ifdef MMC
	                    if (g_is_SD_inserted == HAS_SD)
	                    {
	                        tmp_dirrec.iDevice = 1;
	                        tmp_dirrec.bIsRoot = TRUE;
	                        tmp_dirrec.bIsDir = TRUE;
	                        tmp_dirrec.iRecordNum = 1;
	                        tmp_dirrec.lFastKey = GetRootdirkey(1);
	                        _memset(current, 0, sizeof(PL5_PL_FOLDER));
	                        current->record = tmp_dirrec;
	                        if (PL5_PL_CdSearching_Slow(current) == FS_SUCCESS)
	                        {
	                            packed_strcpy(g_PL5_PL_search.sPath, (_packed BYTE*)gCurrentWorkingdirectory);
//								if(g_PL5_Playback.foundall)
//									g_PL5_PL_search.tracknum = g_PL5_Playback.total + 1;
	                            g_PL5_PL_search.phase = PL5_LASTINLAST;
	                            g_PL5_PL_search.start = 0;
	                        }
	                        else
	                        {
	                            __asm("debug");
	                            return TRUE;
	                        }
	                    }
						else
#endif
						{
//							if(g_PL5_Playback.foundall)
//								g_PL5_PL_search.tracknum = g_PL5_Playback.total + 1;
                            g_PL5_PL_search.phase = PL5_LASTINLAST;
                            g_PL5_PL_search.start = 0;
						}
                    }
#ifdef MMC
                    else
                    {   // b: drive completed. We will enter a: drive.
                        tmp_dirrec.iDevice = 0;
                        tmp_dirrec.bIsRoot = TRUE;
                        tmp_dirrec.bIsDir = TRUE;
                        tmp_dirrec.iRecordNum = 0;
                        tmp_dirrec.lFastKey = GetRootdirkey(0);
                        _memset(current, 0, sizeof(PL5_PL_FOLDER));
                        current->record = tmp_dirrec;
                        if (PL5_PL_CdSearching_Slow(current) == FS_SUCCESS)
                        {
                            packed_strcpy(g_PL5_PL_search.sPath, (_packed BYTE*)gCurrentWorkingdirectory);
                            g_PL5_PL_search.phase = PL5_LASTINLAST;
                            g_PL5_PL_search.start = 0;
                        }
                        else
                        {
                            __asm("debug");
                            return TRUE;
                        }
                    }
#endif					
                }
                else
                {   // return to parrent folder and search previous folder.
                    g_PL5_PL_search.depth --;
                    ((int*)(sFileSpec))[0] = 0x002e2e;
                    if (PL5_Chdir(sFileSpec, 0) != FS_SUCCESS)
                        __asm("debug");
                    packed_strcpy(g_PL5_PL_search.sPath, (_packed BYTE*)gCurrentWorkingdirectory);
                    g_PL5_PL_search.phase = PL5_PREV_FOLDER;
                    g_PL5_PL_search.start = current->record.iRecordNum;
                }
            }
            else
            {
				// Here is the begining of top folder, search from the last file in last. 
//				if(g_PL5_Playback.foundall)
//					g_PL5_PL_search.tracknum = g_PL5_Playback.total + 1;
                g_PL5_PL_search.phase = PL5_LASTINLAST;
                g_PL5_PL_search.start = 0;
            }
        }
        else
        {   // This mode play files in selected folder only.
            if (current->firstfile >= 0)
            {
				// Set the tracknum as the total+1.
//				if(g_PL5_Playback.foundall)
//					g_PL5_PL_search.tracknum = g_PL5_Playback.total + 1;
				
                if (current->foundall)
                {
                    g_PL5_PL_search.start = current->lastfile + 1;
//                    end = TRUE;
                }
                else
                {
                    g_PL5_PL_search.phase = PL5_LAST;
                    g_PL5_PL_search.start = current->lastfile + 1;
                }
            }
            else
                end = TRUE;
        }
    }


    return end;
}

/*----------------------------------------------------------------------------

   Function Name:  static _reentrant BOOL PL5_PL_PrevFolder (void)

   FunctionType:   Reentrant

   Inputs:        1) pointer to playback descriptor

   Outputs:       search complete or not

   Description:  search previous folder. This functions is called when need search
                previous file in other folder.

----------------------------------------------------------------------------*/
static _reentrant BOOL PL5_PL_PrevFolder (void)
{
    PL5_PL_FOLDER * current;
    Finddata finddata;
    int find_handle ;
    _packed char sFileSpec[4];
    PL5_FOLDER_RECORD tmp_dirrec;
    LONG iFinishTime = 0;
    BOOL end = FALSE;
    BOOL found = FALSE;
    BOOL prevfolder = FALSE;

    current = g_PL5_PL_search.tPath + g_PL5_PL_search.depth - 1;    // current searching folder

    ((int*)(sFileSpec))[0] = 0x2a2e2a; // "*.*"
    ((int*)(sFileSpec))[1] = 0; //null terminator

    if (current->firstfolder < 0)
    {
        __asm("debug");     // Code should not reach here.
        return TRUE;
    }

    if (!g_PL5_PL_search.foregnd)
        iFinishTime = SysGetCurrentTime() + PL5_PL_SEARCH_TIME_SLICE;

    tmp_dirrec.iDevice = current->record.iDevice;
    tmp_dirrec.bIsRoot = FALSE;
    tmp_dirrec.bIsDir = TRUE;

    while(!found)
    {
        _memset(&finddata,0,sizeof(finddata));
        finddata.device = current->record.iDevice;
        if (g_PL5_PL_search.start > (current->firstfolder + PL5_TMP_BUFFER_SIZE)) 
            finddata.startrecord = g_PL5_PL_search.start - PL5_TMP_BUFFER_SIZE;
        else
            finddata.startrecord = current->firstfolder;

        find_handle = FindFirst(&finddata, sFileSpec);
        if (find_handle <= 0)
        {
            __asm("debug"); // code should not reach here. A FS error occur
            Fclose(find_handle);
            return TRUE;
        }

        while(1)
        {
            if (finddata.startrecord > g_PL5_PL_search.start)
            {
                if (g_PL5_PlaybackTmpBfrIdx == 0)
                {
                    if (g_PL5_PL_search.start > (current->firstfolder + PL5_TMP_BUFFER_SIZE))
                    {
                        // no valid previous folder found in the last PL5_TMP_BUFFER_SIZE records
                        // reset start point to more previous
                        g_PL5_PL_search.start -= PL5_TMP_BUFFER_SIZE;

                        /* check timeout if this is a background search. */
                        if (!g_PL5_PL_search.foregnd && (SysGetCurrentTime() >= iFinishTime))
                        {
                            // timeout occur, search module must release execution for other module
                            found = TRUE;   // set found TRUE to exit while loop.
                        }
                    }
                    else
                    {
                        // no valid previous folder any more
                        prevfolder = TRUE;
                        found = TRUE;   // set found TRUE to exit while loop.
                    }
                }
                else
                {   // At least one previous folder was found.
                    found = TRUE;
                }

                Fclose(find_handle);
                break;
            }

            if (PL5_PL_ValidFolder(&finddata, FALSE))
            {
                tmp_dirrec.iRecordNum = finddata.startrecord - 1;
                tmp_dirrec.lFastKey = finddata.Key;
                // Insert to the end of the temp queue
                g_PL5_PlaybackTmpBfr[g_PL5_PlaybackTmpBfrIdx++] = tmp_dirrec;
                if (g_PL5_PlaybackTmpBfrIdx > PL5_TMP_BUFFER_SIZE)
                {
                    __asm("debug");     // Code should not reach here. An unknown error occur
                    Fclose(find_handle);
                    g_PL5_PlaybackTmpBfrIdx = 0;
                    return TRUE;
                }
            }

            //now get the next record
            if(FindNext(find_handle, &finddata) < 0)
            {
                //no next entry
                __asm("debug"); // Code should not reach here.
                g_PL5_PlaybackTmpBfrIdx = 0;
                Fclose(find_handle);
                return TRUE;
            }
        }
    }

    if (found)
    {
        if (g_PL5_PlaybackTmpBfrIdx != 0)
        {
            current++;
            _memset(current, 0, sizeof(PL5_PL_FOLDER));
            current->record = g_PL5_PlaybackTmpBfr[(g_PL5_PlaybackTmpBfrIdx - 1)];
            if (PL5_PL_CdSearching_Slow(current) == FS_SUCCESS)
            {
				g_PL5_PlaybackTmpBfrIdx = 0;
                packed_strcpy(g_PL5_PL_search.sPath, (_packed BYTE*)gCurrentWorkingdirectory);
                g_PL5_PL_search.depth ++;
                g_PL5_PL_search.phase = PL5_LASTINLAST;
                g_PL5_PL_search.start = 0;
            }
            else
            {
                __asm("debug");
                return TRUE;
            }
        }
        else
        {
            g_PL5_PL_search.phase = PL5_PREV_FILE;
            g_PL5_PL_search.start = current->lastfile + 1;
        }
    }

    return end;
}

/*----------------------------------------------------------------------------

   Function Name:  static _reentrant BOOL PL5_PL_LastItem (void)

   FunctionType:   Reentrant

   Inputs:        None

   Outputs:       search complete or not

   Description:  findout firstfile, firstfolder, lastfile, lastfolder in current folder.
                 This functions is called when wrap around occur during search of previous file
                 in one folder.
----------------------------------------------------------------------------*/
static _reentrant BOOL PL5_PL_LastItem (void)
{
    PL5_PL_FOLDER * current;
    Finddata finddata;
    int find_handle ;
    _packed char sFileSpec[4];
    LONG iFinishTime = 0;

    current = g_PL5_PL_search.tPath + g_PL5_PL_search.depth - 1;    // current searching folder

    if (!g_PL5_PL_search.foregnd)
        iFinishTime = SysGetCurrentTime() + PL5_PL_SEARCH_TIME_SLICE;

    ((int*)(sFileSpec))[0] = 0x2a2e2a; // "*.*"
    ((int*)(sFileSpec))[1] = 0; //null terminator

    _memset(&finddata,0,sizeof(finddata));
    finddata.device = current->record.iDevice;
    finddata.startrecord = g_PL5_PL_search.start;
    find_handle = FindFirst(&finddata, sFileSpec);

    if (find_handle <= 0)
    {
        current->firstfile = -1;
        current->firstfolder = -1;
        current->foundall = TRUE;
    }
    else
    {
        while(1)
        {
            if (PL5_PL_ValidFile(&finddata, FALSE))
            {
                if (current->firstfile == 0)
                    current->firstfile = finddata.startrecord - 1;
                if (current->lastfile < (finddata.startrecord - 1))
                    current->lastfile = finddata.startrecord - 1;
            }
            else if (PL5_PL_ValidFolder(&finddata, FALSE))
            {
                if (current->firstfolder == 0)
                    current->firstfolder = finddata.startrecord - 1;
                if (current->lastfolder < (finddata.startrecord - 1))
                    current->lastfolder = finddata.startrecord - 1;
            }

            /* check timeout if this is a background search. */
            if (!g_PL5_PL_search.foregnd && (SysGetCurrentTime() >= iFinishTime))
            {
                // timeout occur, search module must release execution for other module
                // save next start point
                g_PL5_PL_search.start = finddata.startrecord;
                Fclose(find_handle);
                return FALSE;
            }

            //now get the next record
            if((finddata.startrecord >= MAX_RECORDS_PER_DIRECTORY) || (FindNext(find_handle, &finddata) < 0))
            {
                // No next file
                Fclose(find_handle);
                break;
            }
        }
    }

    g_PL5_PL_search.phase = PL5_PREV_FILE;
    g_PL5_PL_search.start = current->lastfile + 1;

    return FALSE;
}

/*----------------------------------------------------------------------------

   Function Name:  static _reentrant BOOL PL5_PL_LastItemInLast (void)

   FunctionType:   Reentrant

   Inputs:        None

   Outputs:       search complete or not

   Description:  findout firstfile, firstfolder, lastfile, lastfolder of the last subfolder in each level.
                 This function is called when wrap around occur during search of previous file in folders.

----------------------------------------------------------------------------*/
static _reentrant BOOL PL5_PL_LastItemInLast (void)
{
    PL5_PL_FOLDER * current;
    PL5_FOLDER_RECORD tmp_dirrec;
    Finddata finddata;
    int find_handle ;
    _packed char sFileSpec[4];
    LONG iFinishTime = 0;
    LONG fastkey = 0;
    BOOL prevfolder  = FALSE;

    current = g_PL5_PL_search.tPath + g_PL5_PL_search.depth - 1;    // current searching folder

    if (!g_PL5_PL_search.foregnd)
        iFinishTime = SysGetCurrentTime() + PL5_PL_SEARCH_TIME_SLICE;

    ((int*)(sFileSpec))[0] = 0x2a2e2a; // "*.*"
    ((int*)(sFileSpec))[1] = 0; //null terminator

    _memset(&finddata,0,sizeof(finddata));
    finddata.device = current->record.iDevice;
    finddata.startrecord = g_PL5_PL_search.start;
    find_handle = FindFirst(&finddata, (_packed char*) sFileSpec);

    if (find_handle <= 0)
    {
        if (current->firstfile == 0)
            current->firstfile = -1;
        if (current->firstfolder == 0)
            current->firstfolder = -1;
    }

    while(find_handle > 0)
    {
        if (PL5_PL_ValidFile(&finddata, FALSE))
        {
            if (current->firstfile == 0)
                current->firstfile = finddata.startrecord - 1;
            if (current->lastfile < (finddata.startrecord - 1))
                current->lastfile = finddata.startrecord - 1;
        }
        else if (PL5_PL_ValidFolder(&finddata, FALSE))
        {
            if (current->firstfolder == 0)
                current->firstfolder = finddata.startrecord - 1;
            if (current->lastfolder <= (finddata.startrecord - 1))
            {
                current->lastfolder = finddata.startrecord - 1;
                fastkey = finddata.Key;
            }
        }

        /* check timeout if this is a background search. */
        if (!g_PL5_PL_search.foregnd && (SysGetCurrentTime() >= iFinishTime))
        {
            // timeout occur, search module must release execution for other module
            // save next start point
            g_PL5_PL_search.start = finddata.startrecord;
            Fclose(find_handle);
            return FALSE;
        }

        //now get the next record
        if((finddata.startrecord >= MAX_RECORDS_PER_DIRECTORY) || (FindNext(find_handle, &finddata) < 0))
        {
            // No next file
            Fclose(find_handle);
            break;
        }
    }

    if (current->firstfolder > 0)
    {   // There is valid folder. Enter the last valid folder and repeat searching.
        tmp_dirrec.iDevice = current->record.iDevice;
        tmp_dirrec.bIsRoot = FALSE;
        tmp_dirrec.bIsDir = TRUE;
        tmp_dirrec.iRecordNum = current->lastfolder;
        tmp_dirrec.lFastKey = fastkey;
        current++;
        _memset(current, 0, sizeof(PL5_PL_FOLDER));
        current->record = tmp_dirrec;
        if (PL5_PL_CdSearching_Slow(current) == FS_SUCCESS)
        {
            packed_strcpy(g_PL5_PL_search.sPath, (_packed BYTE*)gCurrentWorkingdirectory);
            g_PL5_PL_search.depth ++;
            g_PL5_PL_search.start = 0;
        }
        else
        {
            __asm("debug");
            return TRUE;
        }
    }
    else
    {   // No valid folder.
        g_PL5_PL_search.phase = PL5_PREV_FILE;
        g_PL5_PL_search.start = current->lastfile + 1;
    }

    return FALSE;
}


/*----------------------------------------------------------------------------

   Function Name:  _reentrant BOOL PL5_PL_Count (void)

   FunctionType:   Reentrant

   Inputs:        None

   Outputs:       search complete or not

   Description:  count total files

----------------------------------------------------------------------------*/
static _reentrant BOOL PL5_PL_Count (void)
{
    PL5_PL_FOLDER * current;
    Finddata finddata;
    int find_handle ;
    _packed char sFileSpec[4];
    _packed char sParrent[3];
    PL5_FOLDER_RECORD tmp_dirrec;
    LONG iFinishTime = 0;
    BOOL end = FALSE;
    BOOL timeout = FALSE;
    BOOL bToParrent = FALSE;

    ((int*)(sFileSpec))[0] = 0x2a2e2a;  // "*.*"
    ((int*)(sFileSpec))[1] = 0; //null terminator
    ((int*)(sParrent))[0] = 0x002e2e;   // ".."

    if (!g_PL5_PL_search.foregnd)
        iFinishTime = SysGetCurrentTime() + PL5_PL_SEARCH_TIME_SLICE;

    while(!timeout)
    {
        current = g_PL5_PL_search.tPath_cnt + g_PL5_PL_search.depth_cnt - 1;    // current searching folder

        _memset(&finddata,0,sizeof(finddata));
        finddata.device = current->record.iDevice;
        finddata.startrecord = g_PL5_PL_search.start_cnt;
        find_handle = FindFirst(&finddata, sFileSpec);

        if (find_handle <= 0)
        {
            bToParrent = TRUE;
        }
        else
        {
            while(1)
            {
                // if this is a valid folder, Chdir to this folder and repeat count
                if ((g_PL5_Playback.mode == PL5_MODE_PLAYFOLDERS) && PL5_PL_ValidFolder(&finddata, TRUE))
                {
                    tmp_dirrec.iDevice = finddata.device;
                    tmp_dirrec.bIsRoot = FALSE;
                    tmp_dirrec.bIsDir = TRUE;
                    tmp_dirrec.iRecordNum = finddata.startrecord - 1;
                    tmp_dirrec.lFastKey = finddata.Key;
                    current++;
                    _memset(current, 0, sizeof(PL5_PL_FOLDER));
                    current->record = tmp_dirrec;
                    if (PL5_PL_CdSearching_Slow(current) == FS_SUCCESS)
                    {
                        packed_strcpy (g_PL5_PL_search.sPath_cnt, (_packed BYTE*)gCurrentWorkingdirectory);
                        g_PL5_PL_search.depth_cnt ++;
                        g_PL5_PL_search.start_cnt = 0;
                    }
                    else
                    {
                        __asm("debug");
                        Fclose(find_handle);
                        return TRUE;
                    }
                    timeout = TRUE;
                    break;
                }

                if (PL5_PL_ValidFile(&finddata, TRUE))
                    g_PL5_Playback.total ++;

                /* check timeout if this is a background search. */
                if (!g_PL5_PL_search.foregnd && (SysGetCurrentTime() >= iFinishTime))
                {
                    // timeout occur, search module must release execution for other module
                    // save next start point
                    g_PL5_PL_search.start_cnt = finddata.startrecord;
                    timeout = TRUE;
                    break;
                }

                //now get the next record
                if((finddata.startrecord >= MAX_RECORDS_PER_DIRECTORY) || (FindNext(find_handle, &finddata) < 0))
                {
                    // No next file in this folder, Chdir to parrent folder and continue count
                    bToParrent = TRUE;
                    break;
                }
            }
        }

        Fclose(find_handle);

        if (bToParrent)
        {
            if ((g_PL5_Playback.mode == PL5_MODE_PLAYFOLDERS) && (g_PL5_PL_search.depth_cnt  > (g_PL5_PL_search.top+1)))
            {   // return to parrent folder and continue search if it is in play folder mode
                // and current folder is not the top
                if (g_PL5_PL_search.depth_cnt == 2)
                {   // current is in root folder
                    if (current->record.iRecordNum == 0)
                    {   // a: drive completed. We will enter b: drive if b: drive exist, otherwise search complete.
#ifdef MMC
                        if (g_is_SD_inserted == HAS_SD)
                        {
                            tmp_dirrec.iDevice = 1;
                            tmp_dirrec.bIsRoot = TRUE;
                            tmp_dirrec.bIsDir = TRUE;
                            tmp_dirrec.iRecordNum = 1;
                            tmp_dirrec.lFastKey = GetRootdirkey(1);
                            _memset(current, 0, sizeof(PL5_PL_FOLDER));
                            current->record = tmp_dirrec;
                            if (PL5_PL_CdSearching_Slow(current) == FS_SUCCESS)
                            {
                                packed_strcpy(g_PL5_PL_search.sPath_cnt, (_packed BYTE*)gCurrentWorkingdirectory);
                                g_PL5_PL_search.start_cnt = 0;
                            }
                            else
                            {
                                __asm("debug");
                                return TRUE;
                            }
                        }
                        else
#endif
                        {
                            end = TRUE;
                        }
                    }
                    else
                    {   // b: drive completed. Search complete.
                        end = TRUE;
                    }
                }
                else
                {
                    g_PL5_PL_search.depth_cnt --;
                    if (PL5_Chdir(sParrent, 0) != FS_SUCCESS)
                        __asm("debug");
                    packed_strcpy(g_PL5_PL_search.sPath_cnt, (_packed BYTE*)gCurrentWorkingdirectory);
                    g_PL5_PL_search.start_cnt = current->record.iRecordNum + 1;
                }
            }
            else
            {   // it is in play files mode or current folder already reach the top
                end = TRUE;
            }

            timeout = TRUE;
        }
    }

    if (end)
    {
        g_PL5_Playback.foundall = TRUE;
    }

    return end;
}


/*----------------------------------------------------------------------------

   Function Name:  static _reentrant void PL5_PL_QueuePushBack (PL5_FOLDER_RECORD * record)
                   static _reentrant void PL5_PL_QueuePushFront (PL5_FOLDER_RECORD * record)

   FunctionType:   Reentrant

   Inputs:        1) pointer to folder record

   Outputs:       None

   Description:  Fulfill basic operations on playback queue
----------------------------------------------------------------------------*/
#ifdef FUNCLET
    #pragma asm
        FPL5_PL_QueuePushBack:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet2 RSRC_FUNCLET_PL5_PL_QUEUEPUSHBACK

        org p,"SYSFUNCLET_PL5_PL_QUEUEPUSHBACK_P":
        dc      RSRC_FUNCLET_PL5_PL_QUEUEPUSHBACK
    #pragma endasm
_reentrant void PL5_PL_QueuePushBack_Funclet (PL5_FOLDER_RECORD * record)
#else
_reentrant void PL5_PL_QueuePushBack (PL5_FOLDER_RECORD * record)
#endif
{
    PL5_PL_QITEM * item = g_PL5_PL_queue.pos + g_PL5_PL_queue.tail;

    _memcpy(item->folder, g_PL5_PL_search.tPath, sizeof(PL5_PL_FOLDER)*g_PL5_PL_search.depth);
    item->depth = g_PL5_PL_search.depth;
    item->file = *record;
    item->tracknum = g_PL5_PL_search.tracknum;

    g_PL5_PL_queue.tail = (g_PL5_PL_queue.tail + 1) % g_PL5_PL_queue.size;
    if (g_PL5_PL_queue.tail == g_PL5_PL_queue.head)
        g_PL5_PL_queue.head = (g_PL5_PL_queue.head + 1) % g_PL5_PL_queue.size;
}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextplaylist5_playback_common":
    #pragma endasm
#endif



#ifdef FUNCLET
    #pragma asm
        FPL5_PL_QueuePushFront:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet2 RSRC_FUNCLET_PL5_PL_QUEUEPUSHFRONT

        org p,"SYSFUNCLET_PL5_PL_QUEUEPUSHFRONT_P":
        dc      RSRC_FUNCLET_PL5_PL_QUEUEPUSHFRONT
    #pragma endasm
_reentrant void PL5_PL_QueuePushFront_Funclet (PL5_FOLDER_RECORD * record)
#else
_reentrant void PL5_PL_QueuePushFront (PL5_FOLDER_RECORD * record)
#endif
{
    PL5_PL_QITEM * item;

    g_PL5_PL_queue.head = (g_PL5_PL_queue.head + g_PL5_PL_queue.size - 1) % g_PL5_PL_queue.size;
    if (g_PL5_PL_queue.head == g_PL5_PL_queue.tail)
        g_PL5_PL_queue.tail = (g_PL5_PL_queue.tail + g_PL5_PL_queue.size - 1) % g_PL5_PL_queue.size;


    item = g_PL5_PL_queue.pos + g_PL5_PL_queue.head;
    _memcpy(item->folder, g_PL5_PL_search.tPath, sizeof(PL5_PL_FOLDER)*g_PL5_PL_search.depth);
    item->depth = g_PL5_PL_search.depth;
    item->file = *record;
    item->tracknum = g_PL5_PL_search.tracknum;
}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextplaylist5_playback_common":
    #pragma endasm
#endif

/*----------------------------------------------------------------------------

   Function Name:  static _reentrant BOOL PL5_PL_ValidFile (Finddata * pFinddata, BOOL counting)

   FunctionType:   Reentrant

   Inputs:        1) pointer to finddata which is got by findfirst()/findnext()
                  2) counting total number or not

   Outputs:       Valid file or Not

   Description:   determine if the find item is a valid file for current playback
----------------------------------------------------------------------------*/
static _reentrant BOOL PL5_PL_ValidFile (Finddata * pFinddata, BOOL counting)
{
	BOOL ret = FALSE;
	WORD wFileExtension;
	INT depth;
	INT * supported = g_PL5_PL_search.piExtSupported;

    depth = counting ? g_PL5_PL_search.depth_cnt : g_PL5_PL_search.depth;

    if (((pFinddata->attrib & (ATTR_UNACCEPTABLE|ATTR_ARCHIVE|ATTR_DIRECTORY)) != ATTR_ARCHIVE)
     || (pFinddata->startrecord >= MAX_RECORDS_PER_DIRECTORY))
        return ret;

    if (*supported == 0)    //*.*
    {   // Except hidden, system file or volumeID
        if (!(pFinddata->attrib & (ATTR_HIDDEN|ATTR_SYSTEM|ATTR_VOLUME_ID)))
            ret = TRUE;
    }
    else if ((g_PL5_Playback.type == PL5_MUSICTYPE)
#ifdef AUDIBLE
        || (g_PL5_Playback.type == PL5_AUDIBLETYPE)
#endif
        || (depth > 2))
	{
        /* MUSIC type and AUDIBLE type allow valid file in any foler, but for other types,
           valid file can only be in special folder under the root directory.
           Checking valid folder is done by PL5_PL_ValidFolder(), and should be before
           PL5_PL_ValidFile(), so we think we already in valid folder here if it is not in
           the root directory.
        */

    	//Get the file extension
	    wFileExtension = PL5_GetFileExtention((_packed BYTE*)pFinddata->name);

        while(*supported != 0)
        {
            if (wFileExtension == *supported)
            {
                ret = TRUE;
                break;
            }
            supported++;
        }
    }

    return ret;
}

/*----------------------------------------------------------------------------

   Function Name:  static _reentrant BOOL PL5_PL_ValidFolder (Finddata * pFinddata, BOOL counting)

   FunctionType:   Reentrant

   Inputs:        1) pointer to finddata which is got by findfirst()/findnext()
                  2) counting total number or not

   Outputs:       Valid file or Not

   Description:   determine if the find item is a valid folder for current playback
----------------------------------------------------------------------------*/
static _reentrant BOOL PL5_PL_ValidFolder (Finddata * pFinddata, BOOL counting)
{
    BOOL ret = FALSE;
    int depth;

    depth = counting ? g_PL5_PL_search.depth_cnt : g_PL5_PL_search.depth;

    if (((pFinddata->attrib & (ATTR_UNACCEPTABLE|ATTR_DIRECTORY)) != ATTR_DIRECTORY)
     || (pFinddata->startrecord >= MAX_RECORDS_PER_DIRECTORY))
        return ret;

    if (depth < (MAX_DIR_DEPTH+1))
    {   // we are not in the deepest folder
        ret = TRUE;

        if (depth == 2)
        {   // we are in the root directory
            _packed BYTE voicefolder[SFN_LENGTH];
#ifndef REMOVE_FM
            _packed BYTE fmrecfolder[SFN_LENGTH];
#endif
            _packed BYTE lineinfolder[SFN_LENGTH];
#ifdef JPEG_APP
            _packed BYTE photofolder[SFN_LENGTH];
#endif
#ifdef MOTION_VIDEO
            _packed BYTE videofolder[SFN_LENGTH];
#endif

            ((int*)voicefolder)[0] = 0x494f56;      //VOI
            ((int*)voicefolder)[1] = 0x004543;      //CE
            ((int*)voicefolder)[2] = 0;
#ifndef REMOVE_FM
            ((int*)fmrecfolder)[0] = 0x004d46;      //FM
            ((int*)fmrecfolder)[1] = 0;
#endif
            ((int*)lineinfolder)[0] = 0x4e494c;     //LIN
            ((int*)lineinfolder)[1] = 0x492d45;     //E-I
            ((int*)lineinfolder)[2] = 0x00004e;     //N
#ifdef JPEG_APP
            ((int*)photofolder)[0] = 0x4f4850;      //PHO
            ((int*)photofolder)[1] = 0x004f54;      //TO
#endif
#ifdef MOTION_VIDEO
            ((int*)videofolder)[0] = 0x49564d;      //MVI
            ((int*)videofolder)[1] = 0x4f4544;      //DEO
            ((int*)videofolder)[2] = 0;
#endif

            switch (g_PL5_Playback.type)
            {
                case PL5_MUSICTYPE:
#ifdef AUDIBLE
                case PL5_AUDIBLETYPE:
#endif
                    if ((packed_strcmp((_packed BYTE*)pFinddata->name, voicefolder) == 0)
#ifndef REMOVE_FM
                       || (packed_strcmp((_packed BYTE*)pFinddata->name, fmrecfolder) == 0)
    #endif
                       || (packed_strcmp((_packed BYTE*)pFinddata->name, lineinfolder) == 0)
    #ifdef JPEG_APP
                       || (packed_strcmp((_packed BYTE*)pFinddata->name, photofolder) == 0)
    #endif
    #ifdef MOTION_VIDEO
                       || (packed_strcmp((_packed BYTE*)pFinddata->name, videofolder) == 0)
    #endif
                    )
                    ret = FALSE;
                    break;
                case PL5_VOICETYPE:
                    if (packed_strcmp((_packed BYTE*)pFinddata->name, voicefolder) != 0)
                        ret = FALSE;
                    break;
#ifndef REMOVE_FM
                case PL5_FMTYPE:
                    if (packed_strcmp((_packed BYTE*)pFinddata->name, fmrecfolder) != 0)
                        ret = FALSE;
                    break;
#endif
                case PL5_LINEINTYPE:
                    if (packed_strcmp((_packed BYTE*)pFinddata->name, lineinfolder) != 0)
                        ret = FALSE;
                    break;
#ifdef JPEG_APP
                case PL5_PHOTOTYPE:
                    if (packed_strcmp((_packed BYTE*)pFinddata->name, photofolder) != 0)
                        ret = FALSE;
                    break;
#endif
#ifdef MOTION_VIDEO
                case PL5_VIDEOTYPE:
                    if (packed_strcmp((_packed BYTE*)pFinddata->name, videofolder) != 0)
                        ret = FALSE;
                    break;
#endif
            }
        }
    }

    return ret;
}
//#pragma endoptimize
