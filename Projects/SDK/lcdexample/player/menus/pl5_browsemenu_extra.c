#include "types.h"
#include "exec.h"
#include "menumanager.h"
#include "messages.h"
#include "resource.h"
#include "menus.h"
#include "stringlib.h"
#include "fserror.h"
#include "playlist.h"
#include "playlist5.h"

////////////////////////////////////////////////////////////
// Macros
////////////////////////////////////////////////////////////
#define IN              0
#define OUT             1

////////////////////////////////////////////////////////////
// extern variables
////////////////////////////////////////////////////////////
extern _packed char gCurrentWorkingdirectory[];
extern INT g_is_SD_inserted;
////////////////////////////////////////////////////////////
// prototypes
////////////////////////////////////////////////////////////
_reentrant int PL5_Chdir (_packed char *filepath, INT RecordNumber);
_reentrant LONG GetRootdirkey(INT DeviceNumber);
_reentrant INT PL5_BR_VirtualFolder (int direction, int ignore, int * c);

////////////////////////////////////////////////////////////
// glocal variables
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// funcstions definitions
////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------

   Function Name:  static _reentrant void PL5_BR_StackPop (PL5_BR_VIEWSTACK* viewstack, PL5_BR_VIEW* view)
                   static _reentrant void PL5_BR_StackPush (PL5_BR_VIEWSTACK* viewstack, PL5_BR_VIEW* view)

   FunctionType:   Reentrant

   Inputs:        1) pointer to view stack descripter
                  2) pointer to a view.

   Outputs:       None

   Description:  Fulfill basic operations on view stack
----------------------------------------------------------------------------*/
static _reentrant void PL5_BR_StackPop (PL5_BR_VIEWSTACK* viewstack, PL5_BR_VIEW* view)
{
    if (viewstack->bottom > viewstack->top)
        viewstack->bottom --;
    else
        __asm("debug");     // underflow error

    _memcpy(view, viewstack->pos+viewstack->bottom, sizeof(PL5_BR_VIEW));
}

static _reentrant void PL5_BR_StackPush (PL5_BR_VIEWSTACK* viewstack, PL5_BR_VIEW* view)
{
    if (viewstack->bottom >= viewstack->size)
        __asm("debug");       // overflow error
    else
    {
        _memcpy(viewstack->pos+viewstack->bottom, view, sizeof(PL5_BR_VIEW));
        viewstack->bottom++;
    }
}

/*----------------------------------------------------------------------------

   Function Name:  _reentrant int PL5_BR_VirtualFolder (int direction, int ignore, int * c)

   FunctionType:   Reentrant

   Inputs:        1) Pointer to browse descripter
                  2) Direction


   Outputs:       FS_SUCCESS OR error code if error occurs

   Description:  Create a virtual folder for top level's browsing

----------------------------------------------------------------------------*/
_reentrant INT PL5_BR_VirtualFolder (int direction, int ignore, int * c)
{
    PL5_BROWSE * browse;
    PL5_BR_QUEUE * queue;
    PL5_BR_VIEWSTACK * viewstack;
    PL5_BR_VIEW * current;
    PL5_BR_SEARCH * search;
    PL5_FOLDER_RECORD * record;
    int i;

    ignore;

    browse = (PL5_BROWSE*) c;
    queue = browse->queue;
    viewstack = browse->viewstack;
    current = browse->current;

    queue->head = queue->tail = 0;

    for(i=0; i<2; i++)
    {
        record = queue->pos + queue->tail;
        if (i == 1)
#ifdef MMC
        if (g_is_SD_inserted != HAS_SD)
#endif
            break;

        record->iDevice = i;
        record->bIsRoot = TRUE;
        record->bIsDir = TRUE;
        record->iRecordNum = i;
        record->lFastKey = GetRootdirkey(i);
        queue->tail++;
    }

    if (direction == IN)
    {
        viewstack->bottom = viewstack->top = 0;

        _memset(current, 0, sizeof(PL5_BR_VIEW));
        current->foundall = TRUE;
#ifdef MMC
        if (g_is_SD_inserted == HAS_SD)
        {
            current->folders = 2;
            current->pagesize = 2;
        }
        else
#endif
        {
            current->folders = 1;
            current->pagesize = 1;
        }
    }
    else
    {
        PL5_BR_StackPop(viewstack, current);
    }

    g_PL5_iCurFileInCurFolder = 0;

    return PLAYLIST_SUCCESS;
}

/*----------------------------------------------------------------------------

   Function Name:  _reentrant INT PL5_BR_BrowseHighlight (INT direction, INT iIgnored1, INT* c)

   FunctionType:   Reentrant

   Inputs:        1) Pointer to browse descripter
                  2) Direction - Into or Outof a folder


   Outputs:       next menu

   Description:  Browse into the highlighted folder or out of current folder
----------------------------------------------------------------------------*/
_reentrant INT PL5_BR_BrowseHighlight (INT direction, INT iIgnored1, INT* c)
{
    PL5_BROWSE * browse;
    PL5_BR_QUEUE * queue;
    PL5_BR_VIEWSTACK * viewstack;
    PL5_BR_VIEW * current;
    PL5_BR_SEARCH * search;
    PL5_BR_VIEW * tmpview;
    PL5_FOLDER_RECORD * record;
    _packed BYTE pszPath[SFN_LENGTH];
    PL5_FOLDER_RECORD tmp_rec;
    int i, j, k, highlight;
    int result;

    iIgnored1;

    browse = (PL5_BROWSE *)c;
    queue = browse->queue;
    viewstack = browse->viewstack;
    current = browse->current;
    search = browse->search;

    highlight = (current->top_index_in_queue + current->highlight) % queue->size;
    record = queue->pos + highlight;

    if (direction == IN)
    {
        if ((record->bIsDir == FALSE) || (viewstack->bottom == viewstack->size))
        {
            // if highlight is a file or reach MAX directory depth (i.e viewstack is full),
            // do nothing and return.
            return MENU_BROWSE;
        }

        /* Chdir into highlight folder */
        if (viewstack->bottom > 0)
        {
            SysPostMessage(4, PL5_BR_MSG_CDBROWSING, (int)browse, (int)(&result));  // Chdir to browsing folder
            SysWaitOnEvent(0,0,0);
        }
        if (result != FS_SUCCESS)
        {
            __asm("debug");
            return MENU_BROWSE;         // A FS error occur
        }

        if (Playlist_GetSFN (record, pszPath) != PLAYLIST_SUCCESS)
        {
            __asm("debug");             // A FS error occur
        }

		browse->search->phase = PL5_NEXT_FOLDER;
        SysPostMessage(3, PL5_BR_MSG_START, (int)browse);   // start browse search here to ensure the searching current directory is right.
        SysWaitOnEvent(0,0,0);
        if (PL5_Chdir ((_packed char*)pszPath, record->iRecordNum) != FS_SUCCESS)   // Chdir to highlight folder
        {
            __asm("debug");
            return MENU_BROWSE;         // A FS error occur
        }

        /* update browse parameters */
        // set up queue parameters
        queue->head = queue->tail = 0;
        // set up view parameters
        PL5_BR_StackPush(viewstack, current);
        _memset(current, 0, sizeof(PL5_BR_VIEW));
        _memcpy(&(current->folder), record, sizeof(PL5_FOLDER_RECORD));
        // set up search parameters
        search->phase = PL5_NEXT_FOLDER;
        search->start = 0;
    }
    else
    {   // direction == OUT
        /* update browse */
        if ((viewstack->bottom == viewstack->top) || (viewstack->bottom == 0))
        {   // if already reach the top. Exit to browse menu.
            return MENU_MAIN;
        }
        else if (viewstack->bottom > 1)
        {   // else if the parrent is not virtual folder, Chdir out of current folder.
			browse->search->phase = PL5_NEXT_FOLDER;
	        SysPostMessage(3, PL5_BR_MSG_START, (int)browse);   // start browse search here to ensure the searching current directory is right.
	        SysWaitOnEvent(0,0,0);
            ((int*)pszPath)[0] = 0x002e2e;
            if (PL5_Chdir((_packed char*)pszPath, record->iRecordNum) != FS_SUCCESS)
            {
                __asm("debug");
                return MENU_BROWSE;      // A FS error occur
            }
            // setup view parameters
            PL5_BR_StackPop(viewstack, current);
            current->pagesize = 0;
            // setup search parametres
            queue->head = queue->tail = current->top_index_in_queue;
            search->phase = PL5_NEXT_FOLDER;
            search->start = current->top_recordnum;
        }
        else if (viewstack->bottom == 1)
        {   // else if the parrent is virtual folder
            PL5_BR_VirtualFolder(OUT, 0, (int*)browse);
        }
    }

    /* save browsing path. */
    packed_strcpy(browse->path, (_packed BYTE*)gCurrentWorkingdirectory);

    /* update browse queue for display. */
    if (viewstack->bottom != 0)
    {
        // set search to foreground for quick response and it will be reset to background after search complete.
        search->foregnd = TRUE;
        SysPostMessage(3, PL5_BR_MSG_START, (int)browse);   // start browse search. This message must have SysWaitOnEvent followed immediately.
        SysWaitOnEvent(0, 0, 0);

            // we are going to a new folder, we need calculate new page size and top position
            for (i=queue->head, k=0; i != queue->tail; i=j)
            {
                j = (i + 1) % queue->size;
                k++;
                if ((!((queue->pos+i)->bIsDir) && (queue->pos+j)->bIsDir)
                 || (((queue->pos+i)->bIsDir == (queue->pos+j)->bIsDir) && ((queue->pos+i)->iRecordNum >= (queue->pos+j)->iRecordNum)))
                {
                    // case of wrap around
                    break;
                }
            }

            if (k > PL5_BR_PAGE_SIZE)
                k = PL5_BR_PAGE_SIZE;

            current->pagesize = k;
            current->top_recordnum = (queue->pos + current->top_index_in_queue)->iRecordNum;

            g_PL5_iCurFileInCurFolder = (queue->pos + current->top_index_in_queue)->bIsDir ? 0 : 1;
        }


    return MENU_BROWSE;
}

/*----------------------------------------------------------------------------

   Function Name:  _reentrant void PL5_BR_StartSearch (INT direction, INT b, INT* c)

   FunctionType:   Reentrant

   Inputs:        1) Pointer to browse descripter
                  2) Direction - search forward or search backward


   Outputs:      None

   Description:  Set search parameters and then start background search. This function will set search start point
                referring to queue head and queue tail, so it is necessary to make sure there are valid contents in queue.

----------------------------------------------------------------------------*/
_reentrant void PL5_BR_StartSearch (INT direction, INT b, INT* c)
{
    PL5_BROWSE * browse;
    PL5_BR_QUEUE * queue;
    PL5_BR_VIEW * current;
    PL5_BR_SEARCH * search;
    PL5_FOLDER_RECORD * head;
    PL5_FOLDER_RECORD * tail;
    BOOL foregnd;
    BOOL skip = FALSE;


    browse = (PL5_BROWSE*) c;
    queue = browse->queue;
    current = browse->current;
    search = browse->search;
    foregnd = (BOOL)b;

    if (queue->head == queue->tail)
    {
        // No valid item, do nothing and return.
        return;
    }

    head = queue->pos + queue->head;
    tail = queue->pos + (queue->tail + queue->size - 1) % queue->size;

    if (browse->viewstack->bottom == 0)
    {
        // case of virtual foler
        int i = current->highlight;
        PL5_BR_VirtualFolder(IN, 0, (int*)browse);
        current->highlight = i;

        return;
    }

    if (browse->viewstack->bottom == 1)
    {
        switch (browse->type)
        {
            case PL5_VOICETYPE:
#ifndef REMOVE_FM
            case PL5_FMTYPE:
#endif
            case PL5_LINEINTYPE:
#ifdef JPEG_APP
            case PL5_PHOTOTYPE:
#endif
#ifdef MOTION_VIDEO
            case PL5_VIDEOTYPE:
#endif
                // In these cases, the only one valid folder should be already listed.
                // For quick response, do not search any more.
                return;
        }
    }


    /* setup search parameters */
    if ((direction == FORWARD)
     && (((search->phase != PL5_NEXT_FOLDER) && (search->phase != PL5_NEXT_FILE))
        || (search->status != PL5_STATE_RUN)
        || (search->foregnd != foregnd)))
    {   // if the search direction is to search next, but the browse search is not searching next file or folder,
        // reset search parameters and then start search.
        if (tail->bIsDir)
        {   // tail is a folder
            if (current->foundall)
            {   // lastfolder is known, so we do boundary detection for quick response
                if (tail->iRecordNum == current->lastfolder)
                {
                    if (current->firstfile >= 0)
                    {
                        search->start = current->firstfile;
                        search->phase = PL5_NEXT_FILE;
                    }
                    else
                    {
                        search->start = current->firstfolder;
                        search->phase = PL5_NEXT_FOLDER;
                    }
                }
                else
                {
                    search->start = tail->iRecordNum + 1;
                    search->phase = PL5_NEXT_FOLDER;
                }
            }
            else
            {   // lastfolder is unknown. Lastfolder is necessary for boundary detection, but for quick response,
                // we do boundary detection in back ground.
                search->start = tail->iRecordNum + 1;
                search->phase = PL5_NEXT_FOLDER;
            }
        }
        else
        {   // tail is a file
            if (current->foundall)
            {   // lastfile is known, so we do boundary detection for quick response
                if (tail->iRecordNum == current->lastfile)
                {   // case of search wrap around
                    if  (current->firstfolder >= 0)
                    {
                        search->start = current->firstfolder;
                        search->phase = PL5_NEXT_FOLDER;
                    }
                    else
                    {
                        search->start = current->firstfile;
                        search->phase = PL5_NEXT_FILE;
                    }
                }
                else
                {   // lastfile is unknown. Lastfile is necessary for boundary detection, but for quick response,
                    // we do boundary detection in back ground.
                    search->start = tail->iRecordNum + 1;
                    search->phase = PL5_NEXT_FILE;
                }
            }
            else
            {
                search->start = tail->iRecordNum + 1;
                search->phase = PL5_NEXT_FILE;
            }
        }
    }
    else if ((direction == BACKWARD)
          && (((search->phase != PL5_PREV_FOLDER) && (search->phase != PL5_PREV_FILE))
            || (search->status != PL5_STATE_RUN)
            || (search->foregnd != foregnd)))
    {   // if the search direction is to search previous, but the browse search is not searching previous file or folder,
        // reset search parameters
        if (head->bIsDir)
        {   // head is a folder
            if (head->iRecordNum == current->firstfolder)
            {   // case of search wrap around
                // for prev search, boundary detection here is necessary.
                if (!current->foundall)
                {   // we need to know the position of first file, last folder and last file for boundary detection.
                    // So we must set search foreground here and it will be reset to background after search complete.
                    search->phase = PL5_LAST;
                    search->foregnd = TRUE;
                    SysPostMessage(3, PL5_BR_MSG_START, (int)browse);   // start browse search. This message must have SysWaitOnEvent followed immediately.
                    SysWaitOnEvent(0,0,0);
                }

                if (current->firstfile >= 0)
                {
                    search->start = current->lastfile + 1;
                    search->phase = PL5_PREV_FILE;
                }
                else
                {   // No valid file in current folder
                    search->start = current->lastfolder + 1;
                    search->phase = PL5_PREV_FOLDER;
                }

                if (current->top_index_in_queue == queue->head)
                {   // Because for the 1st time browse previous, there are no previous items in the queue,
                    // we have to set search foreground to get previous items and it will be reset to background after search complete.
                    search->foregnd = TRUE;
                    SysPostMessage(3, PL5_BR_MSG_START, (int)browse);   // start browse search. This message must have SysWaitOnEvent followed immediately.
                    SysWaitOnEvent(0,0,0);
                    skip = TRUE;
                }
            }
            else
            {
                search->start = head->iRecordNum;
                search->phase = PL5_PREV_FOLDER;
            }
        }
        else
        {   // head is a file
            if (head->iRecordNum == current->firstfile)
            {   // for prev search, boundary detection here is necessary.
                // because we list folders before files, we already know the value of firstfolder and lastfolder here
                if (current->firstfolder >= 0)
                {
                    search->start = current->lastfolder + 1;
                    search->phase = PL5_PREV_FOLDER;
                }
                else
                {   // No valid folder
                    if (!current->foundall)
                    {   // we need to know lastfile for later use.
                        // So we must set search foreground here and it will be reset to background after search complete.
                        search->phase = PL5_LAST;
                        search->foregnd = TRUE;
                        SysPostMessage(3, PL5_BR_MSG_START, (int)browse);   // start browse search. This message must have SysWaitOnEvent followed immediately.
                        SysWaitOnEvent(0,0,0);
                    }
                    search->start = current->lastfile + 1;
                    search->phase = PL5_PREV_FILE;
                }

                if (current->top_index_in_queue == queue->head)
                {   // Because for the 1st time browse previous, there are no previous items in the queue,
                    // we have to set search foreground to get previous items and it will be reset to background after search complete.
                    search->foregnd = TRUE;
                    SysPostMessage(3, PL5_BR_MSG_START, (int)browse);   // start browse search. This message must have SysWaitOnEvent followed immediately.
                    SysWaitOnEvent(0,0,0);
                    skip = TRUE;
                }
            }
            else
            {
                search->start = head->iRecordNum;
                search->phase = PL5_PREV_FILE;
            }
        }
    }
    else
        return;

    if (!skip)
    {
        // !!! Be careful to make sure search parameters have been set properly here.
        search->foregnd = foregnd;
        SysPostMessage(3, PL5_BR_MSG_START, (int)browse);   // start browse search. This message must have SysWaitOnEvent followed immediately.
            SysWaitOnEvent(0,0,0);
    }
}