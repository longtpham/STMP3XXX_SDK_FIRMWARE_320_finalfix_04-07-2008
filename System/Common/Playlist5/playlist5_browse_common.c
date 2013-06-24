#include "types.h"
#include "exec.h"
#include "fsapi.h"
#include "fserror.h"
#include "stringlib.h"
#include "playlist.h"
#include "extern.h"
#include "playlist5.h"

////////////////////////////////////////////////////////////
// Macros
////////////////////////////////////////////////////////////
enum {
    NOTFOUND = 0,
    TIMEOUT,
    FOUND
};

////////////////////////////////////////////////////////////
// extern variables
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// prototypes
////////////////////////////////////////////////////////////
_reentrant INT PL5_Chdir (_packed char *filepath, INT RecordNumber);
_reentrant INT PL5_BR_CdBrowsing_Fast (PL5_BROWSE * browse);
_reentrant INT PL5_BR_CdBrowsing_Slow(PL5_BR_VIEW * view);
_reentrant int PL5_BR_GetSameDepth(_packed BYTE * dst);
static _reentrant BOOL Enough_Items(PL5_BR_QUEUE* queue, PL5_BR_VIEW* current, BOOL searchdir);
static _reentrant BOOL PL5_BR_NextFolder(PL5_BROWSE* browse);
static _reentrant BOOL PL5_BR_NextFile(PL5_BROWSE* browse);
static _reentrant BOOL PL5_BR_PrevFolder(PL5_BROWSE* browse);
static _reentrant BOOL PL5_BR_PrevFile(PL5_BROWSE* browse);
static _reentrant BOOL PL5_BR_LastItem(PL5_BROWSE* browse);
static _reentrant void PL5_BR_QueuePushBack (PL5_BR_QUEUE * queue, PL5_FOLDER_RECORD * record);
static _reentrant void PL5_BR_QueuePushFront (PL5_BR_QUEUE * queue, PL5_FOLDER_RECORD * record);
static _reentrant BOOL PL5_BR_ValidFile (Finddata * pFinddata, PL5_BROWSE * browse);
static _reentrant BOOL PL5_BR_ValidFolder (Finddata * pFinddata, PL5_BROWSE * browse);

////////////////////////////////////////////////////////////
// glocal variables
////////////////////////////////////////////////////////////
// g_PL5_BrowseTmpBfr and g_PL5_BrowseTmpBfrIdx are used by backforward search
// pushfront g_PL5_BrowseTmpBfr untill the record is equal to the g_PL5_BrowseQhead, then pushfront each g_PL5_BrowseTmpBfr item into browse queue
PL5_FOLDER_RECORD g_PL5_BrowseTmpBfr[PL5_TMP_BUFFER_SIZE];
int g_PL5_BrowseTmpBfrIdx;
int g_PL5_iCurFileInCurFolder;
extern e_PL5BrowseType g_ePL5BrowseType;

////////////////////////////////////////////////////////////
// funcstions definitions
////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------

   Function Name:  _reentrant void PL5_BR_Do_Search (PL5_BROWSE* browse)

   FunctionType:   Reentrant

   Inputs:        1) Pointer to browse descripter

   Outputs:       None

   Description:  A browse search state machine

----------------------------------------------------------------------------*/
_reentrant void PL5_BR_Do_Search (PL5_BROWSE* browse)
{
    PL5_BR_SEARCH* search;
    BOOL end = FALSE;
    int result;

    search = browse->search;

    /* Change to browsing folder */
    if (PL5_BR_CdBrowsing_Fast(browse) != FS_SUCCESS)
        __asm("debug");

    /* do search */
    do {
        switch (search->phase)
        {
            case PL5_NEXT_FILE:
                end = PL5_BR_NextFile(browse);
                break;
            case PL5_PREV_FILE:
                end = PL5_BR_PrevFile(browse);
                break;
            case PL5_NEXT_FOLDER:
                end = PL5_BR_NextFolder(browse);
                break;
            case PL5_PREV_FOLDER:
                end = PL5_BR_PrevFolder(browse);
                break;
            case PL5_LAST:
                end = PL5_BR_LastItem(browse);
                break;
            default:
                __asm("debug");     // Unsupported phase
                end = TRUE;
                break;
        }
    } while (search->foregnd && !end);


    if (end)
    {
        // search complete
        if (browse->current->foundall)
        {
            search->status = PL5_STATE_IDLE;
        }
        else
        {
            // to get total count
            search->phase = PL5_LAST;
        }
        search->foregnd = FALSE;
    }
}


/*----------------------------------------------------------------------------

   Function Name:  static _reentrant BOOL PL5_BR_NextFolder(PL5_BROWSE* browse)

   FunctionType:   Reentrant

   Inputs:        1) Pointer to browse descripter

   Outputs:       search complete or not

   Description:  Search next folder

----------------------------------------------------------------------------*/
static _reentrant BOOL PL5_BR_NextFolder(PL5_BROWSE* browse)
{
    PL5_BR_QUEUE * queue;
    PL5_BR_VIEW * current;
    PL5_BR_SEARCH * search;
    PL5_FOLDER_RECORD tmp_dirrec;
    Finddata finddata;
    int find_handle;
    _packed char sFileSpec[4];
    LONG iFinishTime = 0;
    BOOL end = FALSE;

    queue = browse->queue;
    search = browse->search;
    current = browse->current;

    if (current->firstfolder < 0)
    {
        // No valid folder, then search next file
        search->phase = PL5_NEXT_FILE;
        search->start = current->firstfile;
        return FALSE;
    }

    tmp_dirrec.iDevice = current->folder.iDevice;
    tmp_dirrec.bIsRoot = FALSE;
    tmp_dirrec.bIsDir = TRUE;

    ((int*)(sFileSpec))[0] = 0x2a2e2a;  // "*.*"
    ((int*)(sFileSpec))[1] = 0;         //null terminator

    /* set timeout if it is background search */
    if (!search->foregnd)
        iFinishTime = SysGetCurrentTime() + PL5_BR_SEARCH_TIME_SLICE;

    /* set search start point */
    _memset(&finddata,0,sizeof(finddata));
    finddata.device = current->folder.iDevice;
    finddata.startrecord = search->start;

    //get the first that matches our specifications
    find_handle = FindFirst(&finddata, sFileSpec);

    if (find_handle <= 0)
    {
        // No next item, start to search file
        if (current->firstfolder == 0)
        {
            // No valid folder was found
            current->firstfolder = -1;
        }
        search->phase = PL5_NEXT_FILE;
        search->start = current->firstfile;
        Fclose(find_handle);
        return FALSE;
    }

    while(1)
    {
        if (PL5_BR_ValidFolder(&finddata, browse))
        {
            if (current->firstfolder == 0)
                current->firstfolder = finddata.startrecord - 1;
            if (current->lastfolder < (finddata.startrecord-1))
                current->lastfolder = finddata.startrecord - 1;

            if (Enough_Items(queue, current, FORWARD))
            {
                // Enough items have been found, search complete
                Fclose(find_handle);
                return TRUE;
            }
#if (defined JPEG_APP) || (defined MOTION_VIDEO)
			if(!((0
#ifdef JPEG_APP				
				||(g_ePL5BrowseType == PL5_PHOTOTYPE) 
#endif
#ifdef MOTION_VIDEO
				 || (g_ePL5BrowseType == PL5_VIDEOTYPE)
#endif
				) && (browse->viewstack->bottom > 1)))
#endif				
			{
	            // insert this item into browse queue
	            tmp_dirrec.iRecordNum = finddata.startrecord - 1;
	            tmp_dirrec.lFastKey = finddata.Key;
	            PL5_BR_QueuePushBack (queue, &tmp_dirrec);
			}
        }
        else if (PL5_BR_ValidFile(&finddata, browse))
        {
            // update firstfile for starting searching file later quickly
            if (current->firstfile == 0)
                current->firstfile = finddata.startrecord - 1;
        }

        /* check timeout if this is a background search. */
        if (!search->foregnd && (SysGetCurrentTime() >= iFinishTime))
        {
            // timeout occur, search module must release execution for other module
            // save next start point
            search->start = finddata.startrecord;
            Fclose(find_handle);
            return FALSE;
        }

        //now get the next record
        //_memset(&finddata, 0, sizeof(finddata));
        if((finddata.startrecord >= MAX_RECORDS_PER_DIRECTORY) || (FindNext(find_handle, &finddata) < 0))
        {
            // No next folder, start to search file
            search->phase = PL5_NEXT_FILE;
            search->start = current->firstfile;
            Fclose(find_handle);
            return FALSE;
        }
    }
}

/*----------------------------------------------------------------------------

   Function Name:  static _reentrant BOOL PL5_BR_NextFile(PL5_BROWSE* browse)

   FunctionType:   Reentrant

   Inputs:        1) Pointer to browse descripter

   Outputs:       Search complete or not

   Description:  Search next file

----------------------------------------------------------------------------*/
static _reentrant BOOL PL5_BR_NextFile(PL5_BROWSE* browse)
{
    PL5_BR_QUEUE * queue;
    PL5_BR_VIEW * current;
    PL5_BR_SEARCH * search;
    PL5_FOLDER_RECORD tmp_dirrec;
    Finddata finddata;
    int find_handle;
    _packed char sFileSpec[4];
    LONG iFinishTime = 0;

    queue = browse->queue;
    search = browse->search;
    current = browse->current;

    if (current->firstfile < 0)
    {
        // No valid file, search complete
        return TRUE;
    }

    tmp_dirrec.iDevice = current->folder.iDevice;
    tmp_dirrec.bIsRoot = FALSE;
    tmp_dirrec.bIsDir = FALSE;

    ((int*)(sFileSpec))[0] = 0x2a2e2a; // "*.*"
    ((int*)(sFileSpec))[1] = 0; //null terminator

    /* set timeout if it is background search */
    if (!search->foregnd)
        iFinishTime = SysGetCurrentTime() + PL5_BR_SEARCH_TIME_SLICE;

    /* set search start point. */
    _memset(&finddata,0,sizeof(finddata));
    finddata.device = current->folder.iDevice;
    finddata.startrecord = search->start;

    //get the first that matches our specifications
    find_handle = FindFirst(&finddata, sFileSpec);

    if (find_handle <= 0)
    {
        // No next file, search complete.
        if (current->firstfile == 0)
            current->firstfile = -1;
        Fclose(find_handle);
        return TRUE;
    }

    while(find_handle > 0)
    {
        if (PL5_BR_ValidFile(&finddata, browse))
        {
            if (current->lastfile < (finddata.startrecord-1))
                current->lastfile = finddata.startrecord - 1;

            if (Enough_Items(queue, current, FORWARD))
            {
                // Enough items have been found, search complete.
                Fclose(find_handle);
                return TRUE;
            }

            // insert this item into browse queue
            tmp_dirrec.iRecordNum = finddata.startrecord - 1;
            tmp_dirrec.lFastKey = finddata.Key;
            PL5_BR_QueuePushBack (queue, &tmp_dirrec);
        }

        /* check timeout if this is a background search. */
        if (!search->foregnd && (SysGetCurrentTime() >= iFinishTime))
        {
            // timeout occur, search module must release execution for other module
            // save next start point
            search->start = finddata.startrecord;
            Fclose(find_handle);
            return FALSE;
        }

        //now get the next record
        //_memset(&finddata, 0, sizeof(finddata));
        if((finddata.startrecord >= MAX_RECORDS_PER_DIRECTORY) || (FindNext(find_handle, &finddata) < 0))
        {
            // No next file
            Fclose(find_handle);
            return TRUE;
        }
    }

    return TRUE;
}

/*----------------------------------------------------------------------------

   Function Name:  static _reentrant BOOL PL5_BR_PrevFolder(PL5_BROWSE* browse)

   FunctionType:   Reentrant

   Inputs:        1) Pointer to browse descripter

   Outputs:       search complete or not

   Description:  Search previous folder

----------------------------------------------------------------------------*/
static _reentrant BOOL PL5_BR_PrevFolder(PL5_BROWSE* browse)
{
    PL5_BR_QUEUE * queue;
    PL5_BR_VIEW * current;
    PL5_BR_SEARCH * search;
    PL5_FOLDER_RECORD tmp_dirrec;
    Finddata finddata;
    int find_handle;
    _packed char sFileSpec[4];
    LONG iFinishTime = 0;
    BOOL end = FALSE;
    BOOL found = FALSE;

    queue = browse->queue;
    search = browse->search;
    current = browse->current;

    if (current->firstfolder < 0)
    {
        // No valid folder.
        __asm("debug"); // code should not reach here.
        return TRUE;
    }


    tmp_dirrec.iDevice = current->folder.iDevice;
    tmp_dirrec.bIsRoot = FALSE;
    tmp_dirrec.bIsDir = TRUE;


    ((int*)(sFileSpec))[0] = 0x2a2e2a; // "*.*"
    ((int*)(sFileSpec))[1] = 0; //null terminator

    /* set timeout if it is background search */
    if (!search->foregnd)
        iFinishTime = SysGetCurrentTime() + PL5_BR_SEARCH_TIME_SLICE;

    while(!found)
    {
        /* set search start point */
        _memset(&finddata,0,sizeof(finddata));
        finddata.device = current->folder.iDevice;
        if (search->start > (current->firstfolder + PL5_TMP_BUFFER_SIZE))
            finddata.startrecord = search->start - PL5_TMP_BUFFER_SIZE;
        else
            finddata.startrecord = current->firstfolder;

        //get the first that matches our specifications
        find_handle = FindFirst(&finddata, sFileSpec);

        if (find_handle <= 0)
        {
            __asm("debug");     // Code should not reach here. A FS error occur
            Fclose(find_handle);
            g_PL5_BrowseTmpBfrIdx = 0;
            return TRUE;
        }

        while(1)
        {
            if (finddata.startrecord > search->start)
            {
                while(g_PL5_BrowseTmpBfrIdx != 0)
                {   // At least one item was found
                    if (Enough_Items(queue, current, BACKWARD))
                    {
                        // Enough previous folders are found, search complete
                        g_PL5_BrowseTmpBfrIdx = 0;
                        end = TRUE;
                        found = TRUE;
                        break;
                    }
                    else
                    {
                        g_PL5_BrowseTmpBfrIdx--;
						

	                    PL5_BR_QueuePushFront(queue, &g_PL5_BrowseTmpBfr[g_PL5_BrowseTmpBfrIdx]);

                    }
                }

                if (!found)
                {
                    if (search->start > (current->firstfolder + PL5_TMP_BUFFER_SIZE))
                    {   // search does not reach the top, then reset start point and continue search previous
                        search->start -= PL5_TMP_BUFFER_SIZE;
                        /* check timeout if this is a background search. */
                        if (!search->foregnd && (SysGetCurrentTime() >= iFinishTime))
                        {   // timeout occur, search module must release execution for other module
                            found = TRUE;
                        }
                    }
                    else
                    {   // no valid previous folder found any more, search complete.
                        end = TRUE;
                        found = TRUE;
                    }
                }

                Fclose(find_handle);
                break;
            }

            if (PL5_BR_ValidFolder(&finddata, browse))
            {
                tmp_dirrec.iRecordNum = finddata.startrecord - 1;
                tmp_dirrec.lFastKey = finddata.Key;
                // Insert to the end of the temp queue
                g_PL5_BrowseTmpBfr[g_PL5_BrowseTmpBfrIdx++] = tmp_dirrec;
                if (g_PL5_BrowseTmpBfrIdx > PL5_TMP_BUFFER_SIZE)
                {
                    __asm("debug");     // Code should not reach here. An unknown error occur
                    Fclose(find_handle);
                    g_PL5_BrowseTmpBfrIdx = 0;
                    return TRUE;
                }
            }

            //now get the next record
            if((finddata.startrecord >= MAX_RECORDS_PER_DIRECTORY) || (FindNext(find_handle, &finddata) < 0))
            {   // No next folder
                finddata.startrecord = search->start+1;
            }
        }
    }

    return end;
}


/*----------------------------------------------------------------------------

   Function Name:  static _reentrant BOOL PL5_BR_PrevFile(PL5_BROWSE* browse)

   FunctionType:   Reentrant

   Inputs:        1) Pointer to browse descripter

   Outputs:       search complete or not

   Description:  Search previous file

----------------------------------------------------------------------------*/
static _reentrant BOOL PL5_BR_PrevFile(PL5_BROWSE* browse)
{
    PL5_BR_QUEUE * queue;
    PL5_BR_VIEW * current;
    PL5_BR_SEARCH * search;
    PL5_FOLDER_RECORD tmp_dirrec;
    Finddata finddata;
    int find_handle;
    _packed char sFileSpec[4];
    LONG iFinishTime = 0;
    BOOL end = FALSE;
    BOOL found = FALSE;

    queue = browse->queue;
    search = browse->search;
    current = browse->current;

    if (current->firstfile < 0)
    {
        __asm("debug");     // Code should not reach here, an unknown error occur
        return TRUE;
    }


    tmp_dirrec.iDevice = current->folder.iDevice;
    tmp_dirrec.bIsRoot = FALSE;
    tmp_dirrec.bIsDir = FALSE;


    ((int*)(sFileSpec))[0] = 0x2a2e2a; // "*.*"
    ((int*)(sFileSpec))[1] = 0; //null terminator

    /* set timeout if it is background search */
    if (!search->foregnd)
        iFinishTime = SysGetCurrentTime() + PL5_BR_SEARCH_TIME_SLICE;

    while(!found)
    {
        /* set search start point */
        _memset(&finddata,0,sizeof(finddata));
        finddata.device = current->folder.iDevice;
        if (search->start > (current->firstfile + PL5_TMP_BUFFER_SIZE))
            finddata.startrecord = search->start - PL5_TMP_BUFFER_SIZE;
        else
            finddata.startrecord = current->firstfile;

        //get the first that matches our specifications
        find_handle = FindFirst(&finddata, sFileSpec);

        if (find_handle <= 0)
        {
            __asm("debug");     // Code should not reach here. A FS error occur
            Fclose(find_handle);
            g_PL5_BrowseTmpBfrIdx = 0;
            return TRUE;
        }

        while(1)
        {
            if (finddata.startrecord > search->start)
            {
                while (g_PL5_BrowseTmpBfrIdx != 0)
                {
                    if (Enough_Items(queue, current, BACKWARD))
                    {
                        // Enough previous folders are found, search complete
                        g_PL5_BrowseTmpBfrIdx = 0;
                        end = TRUE;
                        found = TRUE;
                        break;
                    }

                    g_PL5_BrowseTmpBfrIdx--;
                    PL5_BR_QueuePushFront(queue, &g_PL5_BrowseTmpBfr[g_PL5_BrowseTmpBfrIdx]);
                }

                if (!found)
                {
                    if (search->start > (current->firstfile + PL5_TMP_BUFFER_SIZE))
                    {   // search does not reach the top, then reset start point and continue search previous
                        search->start -= PL5_TMP_BUFFER_SIZE;
                        /* check timeout if this is a background search. */
                        if (!search->foregnd && (SysGetCurrentTime() >= iFinishTime))
                        {
                            // timeout occur, search module must release execution for other module
                            found = TRUE;   // set found TRUE to exit while loop
                        }
                    }
                    else
                    {   // no valid previous file found any more.
                        // because we list folders before files, we already know firstfolder and lastfolder here.
#if (defined JPEG_APP) || (defined MOTION_VIDEO)
						if((0
#ifdef JPEG_APP				
							||(g_ePL5BrowseType == PL5_PHOTOTYPE) 
#endif
#ifdef MOTION_VIDEO
							 || (g_ePL5BrowseType == PL5_VIDEOTYPE)
#endif
							) && (browse->viewstack->bottom > 1))
						{
							current->firstfolder = -1;
						}
#endif				
                        
                        if (current->firstfolder < 0)
                        {   // we need to know the position of last file.
                            if (!current->foundall)
                            {
                                search->phase = PL5_LAST;
                                search->foregnd = TRUE;
                                PL5_BR_LastItem(browse);
                                search->foregnd = FALSE;
                            }
                            search->phase = PL5_PREV_FILE;
                            search->start = current->lastfile + 1;
                        }
                        else
                        {
                            search->phase = PL5_PREV_FOLDER;
                            search->start = current->lastfolder + 1;
                        }

                        found = TRUE;   // set found TRUE to exit while loop
                    }
                }

                Fclose(find_handle);
                break;
            }


            if (PL5_BR_ValidFile(&finddata, browse))
            {
                tmp_dirrec.iRecordNum = finddata.startrecord - 1;
                tmp_dirrec.lFastKey = finddata.Key;
                // Insert to the end of the temp queue
                g_PL5_BrowseTmpBfr[g_PL5_BrowseTmpBfrIdx++] = tmp_dirrec;
                if (g_PL5_BrowseTmpBfrIdx > PL5_TMP_BUFFER_SIZE)
                {
                    __asm("debug");     // Code should not reach here. An unknown error occur
                    Fclose(find_handle);
                    g_PL5_BrowseTmpBfrIdx = 0;
                    return TRUE;
                }
            }

            //now get the next record
            if((finddata.startrecord >= MAX_RECORDS_PER_DIRECTORY) || (FindNext(find_handle, &finddata) < 0))
            {   // No next file
                finddata.startrecord = search->start + 1;
            }
        }
    }

    return end;
}


/*----------------------------------------------------------------------------

   Function Name:  static _reentrant BOOL PL5_BR_LastItem(PL5_BROWSE* browse)

   FunctionType:   Reentrant

   Inputs:        1) Pointer to browse descripter

   Outputs:       search complete or not

   Description:  traverse through current folder to find out last file,
                 last folder, first file, first folder and file count and folder count

----------------------------------------------------------------------------*/
static _reentrant BOOL PL5_BR_LastItem(PL5_BROWSE* browse)
{
    PL5_BR_VIEW * current;
    PL5_BR_SEARCH * search;
    Finddata finddata;
    int find_handle;
    _packed char sFileSpec[4];
    LONG iFinishTime = 0;

    search = browse->search;
    current = browse->current;

    if ((current->firstfile == -1) && (current->firstfolder = -1))
    {
        // No valid item in this folder, search complete
        current->foundall = TRUE;
        return TRUE;
    }

    ((int*)(sFileSpec))[0] = 0x2a2e2a; // "*.*"
    ((int*)(sFileSpec))[1] = 0; //null terminator

    /* set timeout if it is background search */
    if (!search->foregnd)
        iFinishTime = SysGetCurrentTime() + PL5_BR_SEARCH_TIME_SLICE;

    /* set search start point */
    _memset(&finddata,0,sizeof(finddata));
    finddata.device = current->folder.iDevice;
    finddata.startrecord = current->start_cnt;

    //get the first that matches our specifications
    find_handle = FindFirst(&finddata, sFileSpec);

    if (find_handle <= 0)
    {
        // No valid item in this folder, search complete
        current->firstfile = -1;
        current->firstfolder = -1;
        Fclose(find_handle);
        return TRUE;
    }

    while(1)
    {
        if (PL5_BR_ValidFile(&finddata, browse))
        {
            if (current->firstfile == 0)
                current->firstfile = finddata.startrecord - 1;
            if (current->lastfile < (finddata.startrecord-1))
                current->lastfile = finddata.startrecord - 1;
            current->files++;
        }
        else if (PL5_BR_ValidFolder(&finddata, browse))
        {
            if (current->firstfolder == 0)
                current->firstfolder = finddata.startrecord - 1;
            if (current->lastfolder < (finddata.startrecord-1))
                current->lastfolder = finddata.startrecord - 1;
            current->folders++;
        }

        /* check timeout if this is a background search. */
        if (!search->foregnd && (SysGetCurrentTime() >= iFinishTime))
        {
            // timeout occur, search module must release execution for other module
            // save next start point
            current->start_cnt = finddata.startrecord;
            Fclose(find_handle);
            return FALSE;
        }

        //now get the next record
        //_memset(&finddata, 0, sizeof(finddata));
        if((finddata.startrecord >= MAX_RECORDS_PER_DIRECTORY) || (FindNext(find_handle, &finddata) < 0))
        {
            // No next item, search complete
            Fclose(find_handle);
            if (current->files == 0)
                current->firstfile = -1;
            if (current->folders == 0)
                current->firstfolder = -1;
			
#if (defined JPEG_APP) || (defined MOTION_VIDEO)
						if((0
#ifdef JPEG_APP				
							||(g_ePL5BrowseType == PL5_PHOTOTYPE) 
#endif
#ifdef MOTION_VIDEO
							 || (g_ePL5BrowseType == PL5_VIDEOTYPE)
#endif
							) && (browse->viewstack->bottom > 1))
						{
							current->firstfolder = -1;
							current->lastfolder = 0;
							current->folders = 0;
						}
#endif				

            current->foundall = TRUE;
            return TRUE;
        }
    }
}

/*----------------------------------------------------------------------------

   Function Name:  _reentrant INT PL5_BR_CdBrowsing_Fast (PL5_BROWSE * browse)

   FunctionType:   Reentrant

   Inputs:        1) pointer to browse descripter

   Outputs:       FS_SUCCESS OR error code if error occurs

   Description:  Try to fast change directory to browsing folder. If current working directory is
                 right the distination, do nothing and return; if current working directory is the
                 a parrent of destination, Chdir to destination using relativce path; otherwise,
                 Chdir to destination using full path.

----------------------------------------------------------------------------*/
_reentrant INT PL5_BR_CdBrowsing_Fast (PL5_BROWSE * browse)
{
    int ret = FS_SUCCESS;

    if (packed_strcmp((_packed BYTE*)gCurrentWorkingdirectory, browse->path) != 0)
    {
        PL5_BR_VIEWSTACK * viewstack;
        PL5_BR_VIEW * current;
        int k;

        viewstack = browse->viewstack;
        current = browse->current;

        k = PL5_BR_GetSameDepth(browse->path);

        for(k; k < viewstack->bottom; k++)
        {
            ret = PL5_BR_CdBrowsing_Slow(viewstack->pos+k);
            if (ret != FS_SUCCESS)
                return ret;
        }

		if(k == viewstack->bottom)
	        ret = PL5_BR_CdBrowsing_Slow(current);
    }

    return ret;
}

_reentrant int PL5_BR_GetSameDepth(_packed BYTE * dst)
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
            byte2 = GetChar((_packed char *)dst, &offset2);
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

    return k;
}

/*----------------------------------------------------------------------------

   Function Name:  static _reentrant INT PL5_BR_CdBrowsing_Slow (PL5_BR_VIEW * view)

   FunctionType:   Reentrant

   Inputs:        1) pointer to view descripter

   Outputs:       FS_SUCCESS OR error code if error occurs

   Description:  Change directory to browsing folder. Precondition is that the destination
                folder is a subfolder of current working directory
----------------------------------------------------------------------------*/
_reentrant INT PL5_BR_CdBrowsing_Slow (PL5_BR_VIEW * view)
{
    _packed char pszPath[SFN_LENGTH];
    PL5_FOLDER_RECORD * record;

    record = &(view->folder);
    if (Playlist_GetSFN (record, (_packed BYTE *)pszPath) != PLAYLIST_SUCCESS)
    {
        __asm("debug");
    }

    return PL5_Chdir(pszPath, record->iRecordNum);
}


/*----------------------------------------------------------------------------

   Function Name:  static _reentrant void PL5_BR_QueuePushBack (PL5_BR_QUEUE * queue, PL5_FOLDER_RECORD * record)
                   static _reentrant void PL5_BR_QueuePushFront (PL5_BR_QUEUE * queue, PL5_FOLDER_RECORD * record)

   FunctionType:   Reentrant

   Inputs:        1) pointer to view stack descripter
                  2) pointer to a view.

   Outputs:       None

   Description:  Fulfill basic operations on view stack
----------------------------------------------------------------------------*/
static _reentrant void PL5_BR_QueuePushBack (PL5_BR_QUEUE * queue, PL5_FOLDER_RECORD * record)
{
    _memcpy(queue->pos+queue->tail, record, sizeof(PL5_FOLDER_RECORD));

    queue->tail = (queue->tail + 1) % queue->size;

    if (queue->tail == queue->head)
        queue->head = (queue->head + 1) % queue->size;
}

static _reentrant void PL5_BR_QueuePushFront (PL5_BR_QUEUE * queue, PL5_FOLDER_RECORD * record)
{
    queue->head = (queue->head + queue->size - 1) % queue->size;

    if (queue->head == queue->tail)
        queue->tail = (queue->tail + queue->size - 1) % queue->size;

    _memcpy(queue->pos+queue->head, record, sizeof(PL5_FOLDER_RECORD));
}

/*----------------------------------------------------------------------------

   Function Name:  static _reentrant BOOL PL5_BR_ValidFile (Finddata * pFinddata, PL5_BROWSE * browse)

   FunctionType:   Reentrant

   Inputs:        1) pointer to finddata which is got by findfirst()/findnext()
                  2) pointer to browse descriptor.

   Outputs:       Valid file or Not

   Description:   determine if the find item is a valid file for current browse
----------------------------------------------------------------------------*/
static _reentrant BOOL PL5_BR_ValidFile (Finddata * pFinddata, PL5_BROWSE * browse)
{
	BOOL ret = FALSE;
	WORD wFileExtension;
	INT * supported = browse->search->piExtSupported;

    if (((pFinddata->attrib & (ATTR_UNACCEPTABLE|ATTR_ARCHIVE|ATTR_DIRECTORY)) != ATTR_ARCHIVE)
     || (pFinddata->startrecord >= MAX_RECORDS_PER_DIRECTORY))
        return ret;

    if (*supported == 0)    //*.*
    {
        // Except hidden, system file or volumeID
        if (!(pFinddata->attrib&(ATTR_HIDDEN|ATTR_SYSTEM|ATTR_VOLUME_ID)))
            ret = TRUE;
    }
    else if ((browse->type == PL5_MUSICTYPE)
#ifdef AUDIBLE
        || (browse->type == PL5_AUDIBLETYPE)
#endif
        || (browse->viewstack->bottom > 1))
	{
        /* MUSIC type and AUDIBLE type allow valid file in any foler, but for other types,
           valid file can only be in special folder under the root directory.
           Checking valid folder is done by PL5_BR_ValidFolder(), and should be before
           PL5_BR_ValidFile(), so we think we already in valid folder here if it is not in
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

   Function Name:  static _reentrant BOOL PL5_BR_ValidFolder (Finddata * pFinddata, PL5_BROWSE * browse)

   FunctionType:   Reentrant

   Inputs:        1) pointer to finddata which is got by findfirst()/findnext()
                  2) pointer to browse descriptor.

   Outputs:       Valid file or Not

   Description:   determine if the find item is a valid folder for current browse
----------------------------------------------------------------------------*/
static _reentrant BOOL PL5_BR_ValidFolder (Finddata * pFinddata, PL5_BROWSE * browse)
{
    BOOL ret = FALSE;

    if (((pFinddata->attrib & (ATTR_UNACCEPTABLE|ATTR_DIRECTORY)) != ATTR_DIRECTORY)
     || (pFinddata->startrecord >= MAX_RECORDS_PER_DIRECTORY))
        return ret;

    if (browse->viewstack->bottom < browse->viewstack->size)
    {
        // we are not in the deepest folder
        ret = TRUE;

        if (browse->viewstack->bottom == 1)
        {
            // we are in the root directory
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

            switch (browse->type)
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

/*----------------------------------------------------------------------------

   Function Name:  static _reentrant BOOL Enough_Items(PL5_BR_QUEUE* queue, PL5_BR_VIEW* current, BOOL searchdir)

   FunctionType:   Reentrant

   Inputs:        1) pointer to browse queue descriptor.
                  2) pointer to current view descriptor.
                  3) search direction

   Outputs:       Enough found or Not

   Description:   determine if enough items are found already.
----------------------------------------------------------------------------*/
static _reentrant BOOL Enough_Items(PL5_BR_QUEUE* queue, PL5_BR_VIEW* current, BOOL searchdir)
{
    BOOL rtn = FALSE;

    if (searchdir == FORWARD)
    {
        if (queue->tail == (current->top_index_in_queue + current->pagesize + PL5_BR_PAGE_SIZE) % queue->size)
            rtn = TRUE;
        else if (((queue->tail + 1) % queue->size == queue->head)
              && (queue->head == current->top_index_in_queue))
            rtn = TRUE;
    }
    else
    {
        if(queue->head == (current->top_index_in_queue + queue->size - PL5_BR_PAGE_SIZE)%queue->size)
            rtn = TRUE;
        else if (((queue->tail + 1) % queue->size == queue->head)
              && (queue->tail == (current->top_index_in_queue + current->pagesize) % queue->size))
            rtn = TRUE;
    }

    return rtn;
}
