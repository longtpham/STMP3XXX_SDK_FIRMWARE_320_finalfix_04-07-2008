#include "types.h"
#include "exec.h"
#include "menumanager.h"
#include "messages.h"
#include "resource.h"
#include "buttons.h"
#include "menus.h"
#include "fsapi.h"
#include "fserror.h"
#include "display.h"
#include "stringlib.h"
#include "playerlib.h"
#include "playlist5.h"
#include "playlist5_browsemenu.h"
#include "persist_bits.h"
////////////////////////////////////////////////////////////
// Macros
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// extern variables
////////////////////////////////////////////////////////////
extern unsigned int _Y g_wTotalSongCount;
extern _X INT g_iPlayerState;

////////////////////////////////////////////////////////////
// prototypes
////////////////////////////////////////////////////////////
static _reentrant void PL5_PL_QueuePushBack (PL5_FOLDER_RECORD * record, int tracknum);
static _reentrant void PL5_PL_QueuePushFront (PL5_FOLDER_RECORD * record, int tracknum);
static _reentrant void PL5_Playback_Init(PL5_BROWSE * browse, int mode);
static _reentrant void DisplayWarning(PL5_BR_VIEW * current, PL5_FOLDER_RECORD * record);
static _reentrant void DisplayYesorNo(int yes_or_no);
_reentrant int PL5_BR_PlayAll(int a, int b, int * c);
_reentrant int PL5_BR_Play(int a, int b, int * c);
_reentrant int PL5_BR_Delete(int ignore1, int ignore2, int * c);
_reentrant int PL5_BR_SubmenuExit(int ignore1, int ignore2, int * ignore3);
_reentrant int PL5_BR_BrowseHighlight (int direction, int iIgnored1, int * c);
_reentrant void PL5_BR_StartSearch (INT direction, INT iIgnored2, INT* c);
////////////////////////////////////////////////////////////
// glocal variables
////////////////////////////////////////////////////////////
INT g_LastCurrentMenu=MENU_MUSIC;


////////////////////////////////////////////////////////////
// funcstions definitions
////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------

   Function Name:  _reentrant int PL5_BrowseSubMenu (int a, int b, int * c)

   FunctionType:   Reentrant

   Inputs:        1) pointer to browse descriptor

   Outputs:       None

   Description:  Submenu of seletect item. Called by browsemenu

----------------------------------------------------------------------------*/
_reentrant int PL5_BrowseSubMenu (int a, int b, int * c)
{
	PL5_BROWSE * browse;
	PL5_BR_QUEUE * queue;
	PL5_BR_VIEW * current;
	PL5_FOLDER_RECORD * record;
	union DisplayHints DisplayHint;
	int * pPtr = (int *) &gEventInfo;
	INT  iEvent;
    BOOL bDone = FALSE;
    BOOL bSendToPlayerStateMachine=FALSE;
    int iNextMenu = MENU_BROWSE;
    PL5_BR_SUBMENU_ITEM aItems[4];
    int highlighted, numitems, i;

    browse = (PL5_BROWSE *)c;
    queue = browse->queue;
    current = browse->current;
    record = queue->pos + (current->top_index_in_queue + current->highlight) % queue->size;

    _memset(aItems, 0, sizeof(PL5_BR_SUBMENU_ITEM)*4);

    switch (browse->type)
    {
        case PL5_DELETETYPE:
            if (record->bIsRoot)
            {   // root directory can not be deleted.
                aItems[0].rsrc = RSRC_STRING_EXIT_MENU;
                aItems[0].pfunction = PL5_BR_SubmenuExit;
                numitems = 1;
            }
            else
            {
                aItems[0].rsrc = RSRC_STRING_DELETE_MENU;
                aItems[0].pfunction = PL5_BR_Delete;
                aItems[1].rsrc = RSRC_STRING_EXIT_MENU;
                aItems[1].pfunction = PL5_BR_SubmenuExit;
                numitems = 2;
            }
            break;
        case PL5_MUSICTYPE:
#ifdef AUDIBLE
        case PL5_AUDIBLETYPE:
#endif
     		if(record->bIsDir)
 		    {
 	            aItems[0].rsrc = RSRC_STRING_PLAY_ALL;
 	            aItems[0].pfunction = PL5_BR_PlayAll;
 	            aItems[1].rsrc = RSRC_STRING_PLAY;
 	            aItems[1].pfunction = PL5_BR_Play;
 	            aItems[2].rsrc = RSRC_STRING_EXIT_MENU;
 	            aItems[2].pfunction = PL5_BR_SubmenuExit;
 	            numitems = 3;
 		    }
 		    else
 		    {
 	            aItems[0].rsrc = RSRC_STRING_PLAY;
 	            aItems[0].pfunction = PL5_BR_Play;
 	            aItems[1].rsrc = RSRC_STRING_EXIT_MENU;
 	            aItems[1].pfunction = PL5_BR_SubmenuExit;
 	            numitems = 2;
 		    }
            break;
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
            // these types have their songs in appointed folder under root.
            if (record->bIsRoot)
            {
                aItems[0].rsrc = RSRC_STRING_PLAY;
                aItems[0].pfunction = PL5_BR_Play;
                aItems[1].rsrc = RSRC_STRING_EXIT_MENU;
                aItems[1].pfunction = PL5_BR_SubmenuExit;
                numitems = 2;
            }
    		else
    		{
         		if((record->bIsDir) 
#ifdef JPEG_APP
					&& (browse->type != PL5_PHOTOTYPE) 
#endif
#ifdef MOTION_VIDEO
					&& (browse->type != PL5_VIDEOTYPE)
#endif			
				  )
     		    {
     	            aItems[0].rsrc = RSRC_STRING_PLAY_ALL;
     	            aItems[0].pfunction = PL5_BR_PlayAll;
     	            aItems[1].rsrc = RSRC_STRING_PLAY;
     	            aItems[1].pfunction = PL5_BR_Play;
     	            aItems[2].rsrc = RSRC_STRING_EXIT_MENU;
     	            aItems[2].pfunction = PL5_BR_SubmenuExit;
     	            numitems = 3;
     		    }
     		    else
     		    {
     	            aItems[0].rsrc = RSRC_STRING_PLAY;
     	            aItems[0].pfunction = PL5_BR_Play;
     	            aItems[1].rsrc = RSRC_STRING_EXIT_MENU;
     	            aItems[1].pfunction = PL5_BR_SubmenuExit;
     	            numitems = 2;
     		    }
    		}
            break;
        default:
            aItems[0].rsrc = RSRC_STRING_EXIT_MENU;
            aItems[0].pfunction = PL5_BR_SubmenuExit;
            numitems = 1;
    }

    highlighted = 0;

    //Lets initially clear screen and display all
    DisplayHint.I = COMMON_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);

    while(!bDone)
	{
        DisplayHint.I &= (COMMON_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS)|(1<<DISPLAY_MISC_BITPOS));

        //wait on either a EVENT_BUTTON or EVENT_MESSAGE
        iEvent = SysWaitOnEvent(EVENT_MESSAGE|EVENT_BUTTON,&gEventInfo.msg,DisplayHint.I ? 1: -1);

        bSendToPlayerStateMachine=FALSE;
		switch(iEvent)
		{
			case EVENT_TIMER:
			    SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHint.I,1,0);
                SysPostMessage(6,LCD_CLEAR_RANGE,BROWSE_VIEW_X_POS,BROWSE_VIEW_Y_POS,SONG_TITLE_X_SIZE,4 * MENU_ITEM_HEIGHT);
                for (i=0; i<numitems; i++)
	            {
                    if (i != highlighted)
                        SysPostMessage(5, LCD_PRINT_STRING_RSRC, BROWSE_VIEW_X_POS, BROWSE_VIEW_Y_POS+i*8,aItems[i].rsrc);
                    else
                        SysPostMessage(5, LCD_PRINT_STRING_INV_RSRC, BROWSE_VIEW_X_POS, BROWSE_VIEW_Y_POS+i*8,aItems[i].rsrc);
                }

                DisplayHint.I = 0;

				break;
    		case EVENT_MESSAGE:
	    		switch (gEventInfo.Message.wMsgCommand)
		    	{
			        case MENU_MSG_MEDIA_CHANGED:
				        //g_MenuFlags.MENU_FLAG_RETURN_TO_MUSIC = TRUE;
    				    bDone = TRUE;
	    			    break;
//	    			case MENU_MSG_MEDIA_OVER_CUR:
//	    			    // TODO:
//	    			    break;
				    default:
					    bSendToPlayerStateMachine = TRUE;
				  	    break;
		    	}
			    break;
	        case EVENT_BUTTON:
	            // button pressed, so restart timer if enabled
    	        UpdateAutoShutdownTimer();
#ifdef BACKLIGHT
			    // turn on backlight when button is pressed.
			    if (g_iBackLightState==BACKLIGHT_ON)
			    {
    	    	    SysPostMessage(2,LCD_BACKLIGHT_ON);
            	    SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_BACKLIGHT,1,BACKLIGHT_TIME,MENU_MSG_TURN_OFF_BACKLIGHT);
            	    SysWaitOnEvent(0,0,0);
 		    	}
#endif
	            switch(gEventInfo.Button.wButtonEvent)
	            {
		            case PH_STOP:
		                //if the STOP button is held down, lets call the shutdown menu
		                SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,ShutdownMenu,USERSHUTDOWN,0,0);
		                // if returns update display  -- if doesn't return, powered down
		                DisplayHint.I |= (COMMON_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
		                break;
		            case PR_MENU:
		            	iNextMenu = MENU_MAIN;
				        bDone = TRUE;
		            	break;
		            case PR_MODE:   //case PR_MENU:
		                // Operate selected option
                        iNextMenu = (aItems[highlighted].pfunction)(0,0,(int*)browse);
                        bDone = TRUE;
		                break;
		            case PR_FF:
		                // one line forward
		                if (highlighted < (numitems-1))
		                {
		                    highlighted++;
		                    DisplayHint.bits.Misc = TRUE;
                        }
		                break;
		            case PR_RW:
		                // one line back off
		                if (highlighted > 0)
		                {
		                    highlighted--;
		                    DisplayHint.bits.Misc = TRUE;
                        }
		                break;
					case PH_MENU:
						// allow user to escape to music mode
						// set flag so PR_MENU in music mode will ignore
						iNextMenu = MENU_MUSIC;
						g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
						bDone = TRUE;
						break;
					case PR_PLAY:
					case PR_STOP:
					    bSendToPlayerStateMachine = TRUE;
					    break;
		            default:
						break;
	            }
	            break;

	        default:
	            bSendToPlayerStateMachine = TRUE;
	            break;
		}

        if(bSendToPlayerStateMachine)
            DisplayHint.I |= SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,pPtr);

 	}

 	return iNextMenu;
}

/*----------------------------------------------------------------------------

   Function Name:  _reentrant int PL5_BR_PlayAll(int a, int b, int * c)

   FunctionType:   Reentrant

   Inputs:        1) pointer to browse descriptor

   Outputs:       Next menu

   Description:  Play all songs and all subfolders in current folder.

----------------------------------------------------------------------------*/
_reentrant int PL5_BR_PlayAll(int a, int b, int * c)
{
	PL5_BROWSE * browse;
	PL5_BR_VIEWSTACK * viewstack;
	PL5_BR_VIEW * current;
	BOOL highlight_0 = FALSE;

    browse = (PL5_BROWSE*)c;
    viewstack = browse->viewstack;
    current = browse->current;

    if (viewstack->bottom == 0)
    {   // play all in virtual folder
		current->highlight = 0; 	// If hilight point to b: driver, let it point to a: driver.
        SysCallFunction(RSRC_PL5_BROWSEMENU_EXTRA, PL5_BR_BrowseHighlight, 0, 0, (int*)browse);
		g_PL5_PL_search.top = 0;	// If virtual folder,set top as 0.
    }
	else
	{
		g_PL5_PL_search.top = viewstack->bottom;
	}

    PL5_Playback_Init(browse, PL5_MODE_PLAYFOLDERS);


#ifdef JPEG_APP
	if(browse->type != PL5_PHOTOTYPE)
#endif
#ifdef MOTION_VIDEO
	if(browse->type != PL5_VIDEOTYPE)
#endif
	g_LastCurrentMenu = g_iCurrentMenu;
    g_MenuFlags.MENU_FLAG_RETURN_TO_MUSIC = TRUE;

	return MENU_MUSIC;
}

/*----------------------------------------------------------------------------

   Function Name:  _reentrant int PL5_BR_Play(int a, int b, int * c)

   FunctionType:   Reentrant

   Inputs:        1) pointer to browse descriptor

   Outputs:       Next menu

   Description:  if selected is a folder, play selected folder;
                 if selected is a file, play files in current folder not including subfolders.

----------------------------------------------------------------------------*/
_reentrant int PL5_BR_Play(int a, int b, int * c)
{
	PL5_BROWSE * browse;
	PL5_BR_QUEUE * queue;
	PL5_BR_VIEWSTACK * viewstack;
	PL5_BR_VIEW * current;
    PL5_FOLDER_RECORD * record;
    int highlight;

    browse = (PL5_BROWSE*)c;
    queue = browse->queue;
    viewstack = browse->viewstack;
    current = browse->current;
    highlight = (current->top_index_in_queue + current->highlight) % queue->size;
    record = queue->pos + highlight;

	if ((record->bIsDir) || (viewstack->bottom == 0))
	{
        SysCallFunction(RSRC_PL5_BROWSEMENU_EXTRA, PL5_BR_BrowseHighlight, 0, 0, (int*)browse);	// viewstack->bottom++;
		g_PL5_PL_search.top = viewstack->bottom;	
	    PL5_Playback_Init(browse, PL5_MODE_PLAYFOLDERS);
	}
	else
	{	// Highlight is a file			
		g_PL5_PL_search.top = viewstack->bottom;		
        PL5_Playback_Init(browse, PL5_MODE_PLAYFILES);
	}

#ifdef JPEG_APP
	if(browse->type != PL5_PHOTOTYPE)
#endif
#ifdef MOTION_VIDEO
	if(browse->type != PL5_VIDEOTYPE)
#endif
	g_LastCurrentMenu = g_iCurrentMenu;
	g_MenuFlags.MENU_FLAG_RETURN_TO_MUSIC = TRUE;

	return MENU_MUSIC;
}

/*----------------------------------------------------------------------------

   Function Name:  _reentrant int PL5_BR_Delete(int ignore1, int ignore2, int * c)

   FunctionType:   Reentrant

   Inputs:        1) pointer to browse descriptor

   Outputs:       Next menu

   Description:  Delete selected item. We also delete repeated items of selected one in the queue.

----------------------------------------------------------------------------*/
_reentrant int PL5_BR_Delete(int ignore1, int ignore2, int * c)
{
    PL5_BROWSE * browse;
    PL5_BR_QUEUE * queue;
    PL5_BR_VIEW * current;
    PL5_BR_SEARCH * search;
    PL5_FOLDER_RECORD * record;
    int highlight;
    union DisplayHints DisplayHint;
    int * pPtr = (int *) &gEventInfo;
    int  iEvent;
    BOOL bDone = FALSE;
    BOOL bSendToPlayerStateMachine;
    int iNextMenu = MENU_BROWSE;
    int  yes_or_no = 0;
    int iError = NOERROR;
    _packed char name[MAX_FILENAME_LENGTH];


    browse = (PL5_BROWSE *) c;
    queue = browse->queue;
    current = browse->current;
    search = browse->search;
    highlight = (current->top_index_in_queue + current->highlight) % queue->size;
    record = queue->pos + highlight;

    if (Playlist_GetSFN(record,(_packed BYTE*)name) != PLAYLIST_SUCCESS)
        __asm("debug");

    DisplayHint.I = COMMON_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);

    while(!bDone)
    {
        DisplayHint.I &= (COMMON_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS)|(1<<DISPLAY_MISC_BITPOS));

        //wait on either a EVENT_BUTTON or EVENT_MESSAGE
        iEvent = SysWaitOnEvent(EVENT_MESSAGE|EVENT_BUTTON,&gEventInfo.msg,DisplayHint.I ? 1: -1);

        bSendToPlayerStateMachine=FALSE;
	    switch(iEvent)
	    {
		    case EVENT_TIMER:
                if (DisplayHint.I & (1<<DISPLAY_CLEAR_DISPLAY_BITPOS))
                {
                    DisplayWarning(current, record);
                    DisplayYesorNo(yes_or_no);
                }
                else if (DisplayHint.I & (1<<DISPLAY_MISC_BITPOS))
                    DisplayYesorNo(yes_or_no);

                DisplayHint.I = 0;
			    break;
            case EVENT_MESSAGE:
	            switch (gEventInfo.Message.wMsgCommand)
		        {
			        case MENU_MSG_MEDIA_CHANGED:
			            //g_MenuFlags.MENU_FLAG_RETURN_TO_MUSIC = TRUE;
    			        bDone = TRUE;
	    		        break;
	    		    case MENU_MSG_MEDIA_OVER_CUR:
	    		        // TODO:
	    		        break;
	    		    default:
	    		        bSendToPlayerStateMachine=TRUE;
	    		        break;
		        }
			    break;
	        case EVENT_BUTTON:
	            // button pressed, so restart timer if enabled
    	        UpdateAutoShutdownTimer();
#ifdef BACKLIGHT
		        // turn on backlight when button is pressed.
		        if (g_iBackLightState==BACKLIGHT_ON)
		        {
    	   	        SysPostMessage(2,LCD_BACKLIGHT_ON);
           	        SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_BACKLIGHT,1,BACKLIGHT_TIME,MENU_MSG_TURN_OFF_BACKLIGHT);
           	        SysWaitOnEvent(0,0,0);
 		   	    }
#endif

		        switch(gEventInfo.Button.wButtonEvent)
    		    {
                    case PR_FF:
                    case PR_RW:
                        yes_or_no++;
                        DisplayHint.bits.Misc = TRUE;
                        break;
        		    case PR_MENU:
        		        if (yes_or_no & 1)
        		        {
                        	// stop the decoder
                            SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,PlayerLib_SetState,DECODER_STATE_STOP,TRUE,0);
                            g_iPlayerState = DECODER_STOPPED;

	                        g_iPlaylistRepeat = PLAYLIST_REPEAT_OFF;  // so that cycling of files is successful.
	                        g_bPlaylistShuffle = FALSE;

                            // stop browse search
                            SysPostMessage(3, PL5_BR_MSG_STOP, (int)browse);
                            SysWaitOnEvent(0,0,0);

            		        SysLoadFATWrite();
            		        if (record->bIsDir)
            		        {
            		            iError = Rmdir(name);
            		            if (iError == DIR_NOT_EMPTY)
            		                iError = DeleteTree(name);
            		        }
            		        else
            		        {
        	    	            iError = Fremove(name);
        	    	        }
        	    	        if (iError < 0)
        		                __asm("debug");
                            iError = SysUnLoadFATWrite();

        		            if (iError != SUCCESS)
        		                __asm("debug");
        		            else
                            {   // delete file/folder successfully.
                                /* update browse queue */
                                int i, j, k, step, top_recnum;

                                if (current->pagesize > 1)
                                {
                                    if(current->highlight == 0)
                                        top_recnum = (queue->pos + (current->top_index_in_queue + 1) % queue->size)->iRecordNum;
                                    else
                                        top_recnum = (queue->pos + current->top_index_in_queue)->iRecordNum;
                                }

                                k = (queue->pos + highlight)->iRecordNum;                   // record number of highlight line of view

                                for(i=queue->head, step=0; i!=queue->tail; i=(i+1)%queue->size)
                                {
                                    if (queue->pos[i].iRecordNum != k)
                                    {
                                        if (step > 0)
                                        {
                                            queue->pos[(i + queue->size - step) % queue->size] = queue->pos[i];
                                        }
                                    }
                                    else
                                        step++;
                                }
                                queue->tail = (queue->tail + queue->size - step) % queue->size;

                                /* update browse view */
                                current->firstfile = current->firstfolder = current->lastfile = current->lastfolder = current->folders = current->files = 0;
                                current->start_cnt = 0;
                                current->foundall = FALSE;
                                search->phase = PL5_LAST;
                                search->foregnd = TRUE;
                                SysPostMessage(3, PL5_BR_MSG_START, (int)browse);   // start browse search. This message must have SysWaitOnEvent followed immediately.
                                SysWaitOnEvent(0,0,0);

                                if (current->pagesize == 1)
                                {
                                    SysCallFunction(RSRC_PL5_BROWSEMENU_EXTRA, PL5_BR_StartSearch, 0, 0, (int*)browse);
                                    if (queue->tail > queue->head)
                                        k = queue->tail - queue->head;
                                    else
                                        k = queue->tail + queue->size - queue->head;

                                    if (k >= PL5_BR_PAGE_SIZE)
                                        k = PL5_BR_PAGE_SIZE;

                                    if (queue->tail > queue->head)
                                        current->top_index_in_queue = queue->tail - k;
                                    else
                                        current->top_index_in_queue = queue->tail + queue->size - k;
                                    current->pagesize = k;
                                    current->highlight = k - 1;
                                }
                                else
                                {
                                    SysCallFunction(RSRC_PL5_BROWSEMENU_EXTRA, PL5_BR_StartSearch, FORWARD, FOREGROUND, (int*)browse);
                                    for (i=queue->head; i!=queue->tail; i=(i+1)%queue->size)
                                    {
                                        if (queue->pos[i].iRecordNum == top_recnum)
                                            break;
                                    }
                                    current->top_index_in_queue = i;

                                    for (k=0; i!=queue->tail; i=j)
                                    {
                                        j = (i + 1) % queue->size;
                                        k++;
                                        if ((!((queue->pos+i)->bIsDir) && (queue->pos+j)->bIsDir)
                                        || (((queue->pos+i)->bIsDir == (queue->pos+j)->bIsDir) && ((queue->pos+i)->iRecordNum >= (queue->pos+j)->iRecordNum)))
                                        {   // case of wrap around
                                            break;
                                        }
                                    }

                                    if (k > PL5_BR_PAGE_SIZE)
                                        k = PL5_BR_PAGE_SIZE;

                                    current->pagesize = k;
                                    if (current->highlight >= k)
                                        current->highlight = k - 1;
                                }

                                /* Re-initialize playlist. */
                                g_Rebuild = TRUE;

                                // Clear MTP database clean flag; an object is being deleted, which needs to be accounted for
                                ClearStickyBit((volatile _X WORD *)&HW_RTC_MTP_DB_CLEAN_FLAG_REG, HW_RTC_PERSISTANT1_MTP_DB_CLEAN_FLAG_BITPOS);
                            }
        		        }

		      		    bDone = TRUE;
			            break;
		       	    case PH_MENU:
			            // allow user to escape to music mode
        			    bDone = TRUE;
        			    iNextMenu = MENU_MUSIC;
		       		    // set flag so PR_MENU in music mode will ignore
			            g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
        			    break;
		       	    case PH_STOP:
			            //if the STOP button is held down, lets call the shutdown menu
        			    SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,ShutdownMenu,USERSHUTDOWN,0,0);
		       		    break;
			    }
	            break;
	    }

        if(bSendToPlayerStateMachine)
            DisplayHint.I |= SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,pPtr);
    }

    return iNextMenu;
}

/*----------------------------------------------------------------------------

   Function Name:  static _reentrant void DisplayWarning(PL5_BR_VIEW * current, PL5_FOLDER_RECORD * record)

   FunctionType:   Reentrant

   Inputs:        1) pointer to current view
                  2) pointer to selected record

   Outputs:       Next menu

   Description:  Delete selected item. We also delete repeated items of selected one in the queue.

----------------------------------------------------------------------------*/
static _reentrant void DisplayWarning(PL5_BR_VIEW * current, PL5_FOLDER_RECORD * record)
{
    _packed char name[MAX_FILENAME_LENGTH];
    UCS3 pUString[LFN_LENGTH];
    BOOL unicode = FALSE;

    if (Playlist_GetSFN(record,(_packed BYTE*)name) != PLAYLIST_SUCCESS)
        __asm("debug");

    if (SysCallFunction(RSRC_PLAYLIST_CODEBANK, Playlist_GetLFN, (INT)(&(current->folder.lFastKey)), record->iRecordNum, (INT *)pUString)
            == PLAYLIST_SUCCESS)
        unicode = TRUE;

    SysPostMessage(5,LCD_PRINT_RANGE_RSRC,WARNING_MSG_LINE1_X,WARNING_MSG_LINE1_Y,RSRC_WARNING_MSG_LINE1);
    if (record->bIsDir)
        SysPostMessage(5,LCD_PRINT_RANGE_RSRC,WARNING_MSG_LINE2_X,WARNING_MSG_LINE2_Y,RSRC_WARNING_MSG_ALL_LINE2);
    else
        SysPostMessage(5,LCD_PRINT_RANGE_RSRC,WARNING_MSG_LINE2_X,WARNING_MSG_LINE2_Y,RSRC_WARNING_MSG_LINE2);

    SysPostMessage(6,LCD_CLEAR_RANGE,WARNING_MSG_FILE_X,WARNING_MSG_FILE_Y,SONG_TITLE_X_SIZE,SONG_TITLE_Y_SIZE);
    if (unicode)
        SysPostMessage(5,LCD_PRINT_STRING_UNICODE_ADDR,WARNING_MSG_FILE_X,WARNING_MSG_FILE_Y,pUString);
    else
        SysPostMessage(5,LCD_PRINT_STRING_ADDR,WARNING_MSG_FILE_X,WARNING_MSG_FILE_Y,name);

    SysWaitOnEvent(0,0,0);      // Display local string before exit this function.
}


/*----------------------------------------------------------------------------

   Function Name:  static _reentrant void DisplayYesorNo(int yes_or_no)

   FunctionType:   Reentrant

   Inputs:        1) pointer to browse descriptor

   Outputs:       Next menu

   Description:  Delete selected item. We also delete repeated items of selected one in the queue.

----------------------------------------------------------------------------*/
static _reentrant void DisplayYesorNo(int yes_or_no)
{
   	if (yes_or_no & 1)
        SysPostMessage(5,LCD_PRINT_RANGE_RSRC,WARNING_MSG_YES_X,WARNING_MSG_YES_Y,RSRC_DELETE_YES);
	else
        SysPostMessage(5,LCD_PRINT_RANGE_RSRC,WARNING_MSG_YES_X,WARNING_MSG_YES_Y,RSRC_DELETE_NO);
}

/*----------------------------------------------------------------------------

   Function Name:  _reentrant int PL5_BR_SubmenuExit(int ignore1, int ignore2, int * ignore3)

   FunctionType:   Reentrant

   Inputs:        1) pointer to browse descriptor

   Outputs:       Next menu

   Description:  Exit submenu to browse menu.

----------------------------------------------------------------------------*/
_reentrant int PL5_BR_SubmenuExit(int ignore1, int ignore2, int * ignore3)
{
    return MENU_BROWSE;
}

/*----------------------------------------------------------------------------

   Function Name:  static _reentrant void PL5_Playback_Init(PL5_BROWSE * browse, int mode)

   FunctionType:   Reentrant

   Inputs:        1) pointer to browse descriptor
                  2) playback mode

   Outputs:       None

   Description:  Initialize playback search, playback queue and decoder state before play selected item.

----------------------------------------------------------------------------*/
static _reentrant void PL5_Playback_Init(PL5_BROWSE * browse, int mode)
{
    PL5_BR_QUEUE * queue;
    PL5_BR_VIEWSTACK * viewstack;
    PL5_BR_VIEW * current;
    PL5_BR_SEARCH * search;
	PL5_FOLDER_RECORD * record;
	PL5_FOLDER_RECORD * tmp_rec;
	PL5_PL_QITEM * tmp_qitem;
	PL5_PL_FOLDER * tmp_PL_folder;
	PL5_BR_VIEW * tmp_view;

    int i, j, k, highlight, TmpTracknum;

    queue = browse->queue;
    viewstack = browse->viewstack;
    current = browse->current;
    search = browse->search;
    highlight = (current->top_index_in_queue + current->highlight) % queue->size;
    record = queue->pos + highlight;

    /* stop browse background search. */
    SysPostMessage(3, PL5_BR_MSG_STOP, (int)browse);

    /* stop decoder. */
    SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,PlayerLib_SetState,DECODER_STATE_STOP,TRUE,0);
    g_iPlayerState = DECODER_STOPPED;

    /* Initialize playback descriptor. */
    g_PL5_Playback.type = browse->type;
    g_PL5_Playback.mode = mode;
    g_PL5_Playback.foundall = FALSE;
    g_PL5_Playback.currentplay = 0;
    g_PL5_Playback.total = 0;

    /* Initialize playback search parameters. */
    g_PL5_PL_search.phase = PL5_NEXT_FILE;
    g_PL5_PL_search.foregnd = FALSE;

    for(i=0; i<MAX_EXTS; i++)
    {
        g_PL5_PL_search.piExtSupported[i] = search->piExtSupported[i];
    }
    for(i=0; i<viewstack->bottom; i++)
    {
        tmp_PL_folder = g_PL5_PL_search.tPath+i;
        tmp_view = viewstack->pos+i;

        tmp_PL_folder->record = tmp_view->folder;
        tmp_PL_folder->firstfile = tmp_view->firstfile;
        tmp_PL_folder->firstfolder = tmp_view->firstfolder;
        tmp_PL_folder->lastfile = tmp_view->lastfile;
        tmp_PL_folder->lastfolder = tmp_view->lastfolder;
        tmp_PL_folder->foundall = tmp_view->foundall;
    }
    tmp_PL_folder = g_PL5_PL_search.tPath+i;
    tmp_PL_folder->record = current->folder;
    tmp_PL_folder->firstfile = current->firstfile;
    tmp_PL_folder->firstfolder = current->firstfolder;
    tmp_PL_folder->lastfile = current->lastfile;
    tmp_PL_folder->lastfolder = current->lastfolder;
    tmp_PL_folder->foundall = current->foundall;
    packed_strcpy(g_PL5_PL_search.sPath, browse->path);
    g_PL5_PL_search.depth = i + 1;


    /* Initialize playback queue. */
    g_PL5_PL_queue.head = g_PL5_PL_queue.tail = 0;
    i = j = 0;
	if (g_PL5_iCurFileInCurFolder == 0)
		g_PL5_iCurFileInCurFolder ++;
	TmpTracknum = 0;
    do
    {   // do loop
        //      copy files in browse queue to playback queue
        // end loop if playback queue is full or all files in browse queue are copied.
        k = (highlight + i) % queue->size;
        if (k != queue->tail)
        {
            tmp_rec = queue->pos + k;
            if (!tmp_rec->bIsDir)
            {
            	PL5_PL_QueuePushBack (tmp_rec, g_PL5_iCurFileInCurFolder + TmpTracknum);
				TmpTracknum++;
            }	
            i ++;
        }
        else
            i += queue->size;

        if (g_PL5_PL_queue.head == (g_PL5_PL_queue.tail + 1) % g_PL5_PL_queue.size)
            break;

        k = (highlight + queue->size - j) % queue->size;
        if (k != queue->head)
        {
            j ++;
            tmp_rec = queue->pos + (highlight + queue->size - j) % queue->size;
            if (!tmp_rec->bIsDir)
                PL5_PL_QueuePushFront (tmp_rec, g_PL5_iCurFileInCurFolder - TmpTracknum);
        }
        else
            j += queue->size;
    } while ((g_PL5_PL_queue.head != (g_PL5_PL_queue.tail + 1) % g_PL5_PL_queue.size)
          && (i <= queue->size || j <= queue->size));

	// Prevent re-copy the same songs
	for(k = 0; k < g_PL5_PL_queue.tail; k++)
	{
		if((g_PL5_PL_queue.pos + k)->file.iRecordNum >= (g_PL5_PL_queue.pos + k + 1)->file.iRecordNum)
		{
			g_PL5_PL_queue.tail = k + 1;
			break;
		}
	}

	// Prevent re-copy the same songs
	if(g_PL5_PL_queue.head)
	{
		for(k = g_PL5_PL_queue.head; k < (g_PL5_PL_queue.size - 1); k++)
		{
			if((g_PL5_PL_queue.pos + k)->file.iRecordNum >= (g_PL5_PL_queue.pos + k + 1)->file.iRecordNum)
				g_PL5_PL_queue.head = k + 1;
		}
	}

    /* Set playback search start point. */
    if (g_PL5_PL_queue.tail == g_PL5_PL_queue.head)
    {
        g_PL5_PL_search.tracknum = 0;
        g_PL5_PL_search.start = current->firstfile;
        g_PL5_PL_search.foregnd = TRUE;
        i = g_PL5_PL_queue.size;
        g_PL5_PL_queue.size = 2;
        SysPostMessage(2, PL5_PL_MSG_START);
        SysWaitOnEvent(0,0,0);
        g_PL5_PL_queue.size = i;
    }
    else
    {   // One song is ready for playback
        tmp_qitem = g_PL5_PL_queue.pos + (g_PL5_PL_queue.tail + g_PL5_PL_queue.size - 1) % g_PL5_PL_queue.size;
        g_PL5_PL_search.tracknum = tmp_qitem->tracknum;
        g_PL5_PL_search.start = tmp_qitem->file.iRecordNum + 1;
    }

    /* Initialize playback counting parameters */
    if (g_PL5_PL_queue.tail == g_PL5_PL_queue.head)
    {
        g_wTotalSongCount = g_PL5_Playback.total = 0;
        g_PL5_Playback.foundall = TRUE;
        SysPostMessage(2, PL5_PL_MSG_STOP);
    }
    else
    {
        SysPostMessage(2, MENU_MSG_PL5_PLAY_SONG);
        // reset start point of counting
        tmp_qitem = g_PL5_PL_queue.pos + (g_PL5_PL_queue.tail + g_PL5_PL_queue.size - 1) % g_PL5_PL_queue.size;
        _memcpy(g_PL5_PL_search.tPath_cnt, g_PL5_PL_search.tPath, sizeof(PL5_PL_FOLDER) * (MAX_DIR_DEPTH+1));
        packed_strcpy(g_PL5_PL_search.sPath_cnt, g_PL5_PL_search.sPath);
        g_PL5_PL_search.depth_cnt = g_PL5_PL_search.depth;
        g_PL5_PL_search.start_cnt = 0;
		g_PL5_Playback.total = 0;
		g_PL5_Playback.foundall = FALSE;
        g_wTotalSongCount = tmp_qitem->tracknum;
		g_PL5_PL_search.status = PL5_STATE_RUN;

        if (g_PL5_PL_queue.head != (g_PL5_PL_queue.tail + 1) % g_PL5_PL_queue.size)
        {
            // continue to search more next file if playback queue has space
            g_PL5_PL_search.phase = PL5_NEXT_FILE;
	        g_PL5_PL_search.start = tmp_qitem->file.iRecordNum + 1;
            SysPostMessage(2, PL5_PL_MSG_START);
//	        SysWaitOnEvent(0,0,0);
        }
    }
    SysWaitOnEvent(0,0,0);
}


/*----------------------------------------------------------------------------

   Function Name:  static _reentrant void PL5_PL_QueuePushBack (PL5_FOLDER_RECORD * record, int tracknum)
                   static _reentrant void PL5_PL_QueuePushFront (PL5_FOLDER_RECORD * record, int tracknum)

   FunctionType:   Reentrant

   Inputs:        1) pointer to folder record
                  2) track number

   Outputs:       None

   Description:  Fulfill basic operations on playback queue
----------------------------------------------------------------------------*/
static _reentrant void PL5_PL_QueuePushBack (PL5_FOLDER_RECORD * record, int tracknum)
{
    PL5_PL_QITEM * item = g_PL5_PL_queue.pos + g_PL5_PL_queue.tail;

    _memcpy(item->folder, g_PL5_PL_search.tPath, sizeof(PL5_PL_FOLDER)*g_PL5_PL_search.depth);
    item->depth = g_PL5_PL_search.depth;
    item->file = *record;
    item->tracknum = tracknum;

    g_PL5_PL_queue.tail = (g_PL5_PL_queue.tail + 1) % g_PL5_PL_queue.size;
    if (g_PL5_PL_queue.tail == g_PL5_PL_queue.head)
        g_PL5_PL_queue.head = (g_PL5_PL_queue.head + 1) % g_PL5_PL_queue.size;
}

static _reentrant void PL5_PL_QueuePushFront (PL5_FOLDER_RECORD * record, int tracknum)
{
    PL5_PL_QITEM * item;

    g_PL5_PL_queue.head = (g_PL5_PL_queue.head + g_PL5_PL_queue.size - 1) % g_PL5_PL_queue.size;
    if (g_PL5_PL_queue.head == g_PL5_PL_queue.tail)
        g_PL5_PL_queue.tail = (g_PL5_PL_queue.tail + g_PL5_PL_queue.size - 1) % g_PL5_PL_queue.size;


    item = g_PL5_PL_queue.pos + g_PL5_PL_queue.head;
    _memcpy(item->folder, g_PL5_PL_search.tPath, sizeof(PL5_PL_FOLDER)*g_PL5_PL_search.depth);
    item->depth = g_PL5_PL_search.depth;
    item->file = *record;
    item->tracknum = tracknum;
}


