#include "types.h"
#include "exec.h"
#include "menumanager.h"
#include "messages.h"
#include "resource.h"
#include "buttons.h"
#include "menus.h"
#include "display.h"
#include "playerlib.h"
#include "playlist.h"
#include "playlist5.h"
#include "playlist5_browsemenu.h"
#include "font.h"

////////////////////////////////////////////////////////////
// Macros
////////////////////////////////////////////////////////////
// Comment below line, if do not display total count
#define PL5_BR_DISTOTAL    0

// Comment below line, if display items page by page.
//#define LINE_BY_LINE    1

////////////////////////////////////////////////////////////
// extern variables
////////////////////////////////////////////////////////////
extern INT g_is_SD_inserted;
extern BOOL g_bDispRefreshAll;
////////////////////////////////////////////////////////////
// prototypes
////////////////////////////////////////////////////////////
_reentrant int PL5_BR_VirtualFolder (int direction, int ignore, int * c);
_reentrant int PL5_BR_BrowseHighlight (int direction, int iIgnored1, int * c);
_reentrant void PL5_BR_StartSearch (INT direction, INT b, INT* c);
_reentrant INT Chdir(_packed char *filepath);
void _reentrant DisplayView(int x, int y, PL5_BROWSE * browse);
_reentrant int PL5_BrowseSubMenu (int a, int b, int * c);
////////////////////////////////////////////////////////////
// glocal variables
////////////////////////////////////////////////////////////
int scrolling_offset;
e_PL5BrowseType g_ePL5BrowseType;
int g_iTitleResource;
////////////////////////////////////////////////////////////
// funcstions definitions
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int _reentrant BrowseMenu(int a, int b, int *pPtr)
//
//   Type:          Function (code bank callable)
//
//   Description:   This function is the browse menu
//
//   Inputs:        none used.
//
//   Outputs:       messages
//
//   Notes:
//<
////////////////////////////////////////////////////////////////////////////////
int _reentrant BrowseMenu(int a, int b, int *pPtr)
{
	union DisplayHints DisplayHint;
	int  iEvent;
    BOOL bDone = FALSE;
    INT  iNextMenu = MENU_MAIN;
    BOOL bSendToPlayerStateMachine;
    BOOL bVolume = FALSE;
#if PL5_BR_DISTOTAL
    BOOL bFoundAll = FALSE;
#endif

    PL5_BROWSE g_browse;
    PL5_BR_QUEUE g_queue;
    PL5_BR_VIEWSTACK g_viewstack;
    PL5_BR_VIEW g_current;
    PL5_BR_SEARCH g_search;
    PL5_FOLDER_RECORD buffer[PL5_BR_QUEUE_SIZE];
    PL5_BR_VIEW stack[PL5_BR_VIEWSTACK_SIZE];
    _packed BYTE browsingpath[MAX_FILENAME_LENGTH];
    INT extsupported[MAX_EXTS];

    int y, highlight;

	a, b;//here to prevent parameter not used messages.

    // this is required to get the message into the playerstatemachine
	// casted to avoid pointer mismatch warnings
	pPtr = (int *) &gEventInfo;

    SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_ANIMATE);

    //Disable playback background count for quick response
    SysPostMessage(2, PL5_PL_MSG_DISCNT);
	SysWaitOnEvent(0, 0, 0);
	
    /* Initialize browse parameters */
    g_browse.queue = &g_queue;
    g_browse.viewstack = &g_viewstack;
    g_browse.current = &g_current;
    g_browse.search = &g_search;
    g_browse.path = browsingpath;
    g_browse.type = g_ePL5BrowseType;

    g_queue.pos = buffer;
    g_queue.size = PL5_BR_QUEUE_SIZE;
    g_queue.head = g_queue.tail = 0;

    g_viewstack.pos = stack;
    g_viewstack.size = PL5_BR_VIEWSTACK_SIZE;
    g_viewstack.top = 0;
    g_viewstack.bottom = 0;

    _memset(&g_search, 0, sizeof(PL5_BR_SEARCH));
    g_search.piExtSupported = extsupported;

    switch (g_ePL5BrowseType)
    {
        case PL5_MUSICTYPE:
            extsupported[0] = MP3_FILE_EXT;
            extsupported[1] = WMA_FILE_EXT;
            extsupported[2] = WAV_FILE_EXT;
            extsupported[3] = 0;          // the end
            break;
        case PL5_VOICETYPE:
            extsupported[0] = MP3_FILE_EXT;
            extsupported[1] = WAV_FILE_EXT;
            extsupported[2] = 0;          // the end
            break;
#ifndef REMOVE_FM
        case PL5_FMTYPE:
            extsupported[0] = MP3_FILE_EXT;
            extsupported[1] = WAV_FILE_EXT;
            extsupported[2] = 0;          // the end
            break;
#endif
        case PL5_LINEINTYPE:
            extsupported[0] = MP3_FILE_EXT;
            extsupported[1] = WAV_FILE_EXT;
            extsupported[2] = 0;          // the end
            break;
#ifdef JPEG_APP
        case PL5_PHOTOTYPE:
            extsupported[0] = JPG_FILE_EXT;
            extsupported[1] = BMP_FILE_EXT;
            extsupported[2] = 0;          // the end
            break;
#endif
#ifdef MOTION_VIDEO
        case PL5_VIDEOTYPE:
            extsupported[0] = SMV_FILE_EXT;
            extsupported[1] = 0;          // the end
            break;
#endif
#ifdef AUDIBLE
        case PL5_AUDIBLETYPE:
            extsupported[0] = AUD_FILE_EXT;
            extsupported[1] = 0;          // the end
            break;
#endif
        default:
            // extsupported[0] is zero means *.*
            extsupported[0] = 0;          // the end
    }

    /* Initialize current view. Virtual folder is 1st folder to browse. */
    SysCallFunction(RSRC_PL5_BROWSEMENU_EXTRA, PL5_BR_VirtualFolder, 0, 0, (int*)&g_browse);

//#if defined(MMC)
//    // Stop the decoder if needed. STMP Issue 2375 -------
//    // Needed here to display stop icon.
//    if(!(g_wDecoderSR&DECODER_STOPPED))
//    {
//        SysPostMessage(2,DECODER_STOP);
//        while(!(g_wDecoderSR&DECODER_STOPPED))
//        {
//            SysWaitOnEvent(0,0,2);
//        }
//    }
//    DisplayHint.bits.PlayState = TRUE;  // Will update play state icon to stopped
//#endif      //-------- STMP 2375

    //Lets initially clear screen and display all
    DisplayHint.I = BROWSE_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);

    while(!bDone)
	{
        DisplayHint.I &= (BROWSE_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));

        highlight = (g_current.top_index_in_queue + g_current.highlight) % g_queue.size;

        //wait on either a EVENT_BUTTON or EVENT_MESSAGE
        iEvent = SysWaitOnEvent(EVENT_MESSAGE|EVENT_BUTTON,&gEventInfo.msg,DisplayHint.I ? 1: -1);

        bSendToPlayerStateMachine=TRUE;

		switch(iEvent)
		{
			case EVENT_TIMER:
				if(g_bDispRefreshAll)
				{	// Once got a new current song, refresh all
					DisplayHint.I |=BROWSE_MENU_DISPLAY_ALL | (1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
					g_bDispRefreshAll = FALSE;
				}
                SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHint.I,1,0);
			    if (DisplayHint.bits.ClearDisplay)
			    {  // if cleared display, redisplay info
			        DisplayView(BROWSE_VIEW_X_POS, BROWSE_VIEW_Y_POS, &g_browse);
			    }
                else
                {
    		        // update display of highlight line
                    y = BROWSE_VIEW_Y_POS + (g_current.highlight+1) * MENU_ITEM_HEIGHT;
                    DisplayLine (BROWSE_VIEW_X_POS, y, &g_current, &g_queue, highlight);
#if PL5_BR_DISTOTAL
                    // update display of total count
                    if (!bFoundAll)
                    {
                        if (g_current.foundall)
                        {
                            bFoundAll = TRUE;
                        }

                        SysPostMessage(7,LCD_PRINT_NUMBER,BROWSE_VIEW_X_POS+60,BROWSE_VIEW_Y_POS,
                            (g_current.folders+g_current.files),4,'0');

                        SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_ANIMATE,1,ANIMATE_PERIOD,MENU_MSG_ANIMATE);

                        SysWaitOnEvent(0,0,0);
                    }
#endif
                }

                DisplayHint.I=0;
	            bSendToPlayerStateMachine = FALSE;
				break;
    		case EVENT_MESSAGE:
	    		switch (gEventInfo.Message.wMsgCommand)
		    	{
			        case MENU_MSG_MEDIA_CHANGED:
				        g_MenuFlags.MENU_FLAG_RETURN_TO_MUSIC = TRUE;
				        iNextMenu = MENU_MAIN;
    				    bDone = TRUE;
	    			    break;
		    	    case MENU_MSG_ANIMATE:
		    	        if (g_current.foundall)
		    	        {
			    	        scrolling_offset++;
                        }

				        DisplayHint.bits.Misc = TRUE;
    				    bSendToPlayerStateMachine = FALSE;
	    			    break;
		    	}
			    break;
	        case EVENT_BUTTON:
	            // button pressed, so restart timer if enabled
    	        UpdateAutoShutdownTimer();
#ifdef BACKLIGHT
			    // turn on backlight when button is pressed.
			    if (g_iBackLightState == BACKLIGHT_ON)
			    {
    	    	    SysPostMessage(2,LCD_BACKLIGHT_ON);
            	    SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_BACKLIGHT,1,BACKLIGHT_TIME,MENU_MSG_TURN_OFF_BACKLIGHT);
            	    SysWaitOnEvent(0,0,0);
 		    	}
#endif
                bSendToPlayerStateMachine = FALSE;

	            //somebody pressed a button.
	            //remember the last button event (mostly for coming out of FFWD and RWND)
	            switch(g_wLastButton = gEventInfo.Button.wButtonEvent)
	            {
		            case PH_STOP:
		                //if the STOP button is held down, lets call the shutdown menu
		                SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,ShutdownMenu,USERSHUTDOWN,0,0);
		                // if returns update display  -- if doesn't return, powered down
		                DisplayHint.I |= (BROWSE_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
		                break;
		            case PR_MENU:
		            	//return to mainmenu
		            	iNextMenu = MENU_MAIN;
		            	bDone = TRUE;
		            	break;
		            case PR_MODE:
		                // switch to submenu
#ifdef JPEG_APP
		                if((g_browse.viewstack->bottom < 1) && (g_ePL5BrowseType == PL5_PHOTOTYPE))
							break;
#endif						
#ifdef MOTION_VIDEO
		                if((g_browse.viewstack->bottom < 1) && (g_ePL5BrowseType == PL5_VIDEOTYPE))
							break;
#endif						
						if(g_browse.queue->head == g_browse.queue->tail)
							break;
						
                        iNextMenu = SysCallFunction(RSRC_PL5BROWSE_SUBMENU_CODEBANK,PL5_BrowseSubMenu,
                                        0,0,(int*)&g_browse);

                        if (iNextMenu != MENU_BROWSE)
                            bDone = TRUE;

                        DisplayHint.I = BROWSE_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
		                break;
					case PR_PLAY:
//					    // play highlighted file/folder
//						if((queue.pos+highlight)->bIsDir)
//						{
//						    iNextMenu = SysCallFunction(RSRC_PL5BROWSE_SUBMENU_CODEBANK,PL5_Browse_PlayAllSubFolder,0,0,&browse);
//						}
//						else
//						{
//							iNextMenu = SysCallFunction(RSRC_PL5BROWSE_SUBMENU_CODEBANK,PL5_Browse_PlayAll,0,0,&browse);
//						}
//						bDone = TRUE;
						break;
		            case PR_FF:
		            case PH_FF:
		                // Down the list
                        BrowseView_Down(&g_browse);
		                break;
		            case PR_RW:
		            case PH_RW:
		                // Up the list
                        BrowseView_Up(&g_browse);
		                break;
		            case PR_VOL_UP:
		                if (!bVolume && InValidRange(&g_queue, highlight))
		                {
       		                // go to subfolder
      	                    SysCallFunction(RSRC_PL5_BROWSEMENU_EXTRA, PL5_BR_BrowseHighlight, 0, 0, (int*)&g_browse);
                            DisplayView(BROWSE_VIEW_X_POS, BROWSE_VIEW_Y_POS, &g_browse);
#if PL5_BR_DISTOTAL
                            bFoundAll = FALSE;
#endif
                        }
		                else
		                    bVolume = FALSE;
		                break;
		            case PR_VOL_DOWN:
		                if (!bVolume)
		                {
       		                // go to parent folder
                            iNextMenu = SysCallFunction(RSRC_PL5_BROWSEMENU_EXTRA, PL5_BR_BrowseHighlight, 1, 0, (int*)&g_browse);
                            if (iNextMenu == MENU_MAIN)
                            {
                                bDone = TRUE;
                            }
                            else
                            {
                                DisplayView(BROWSE_VIEW_X_POS, BROWSE_VIEW_Y_POS, &g_browse);
#if PL5_BR_DISTOTAL
                                bFoundAll = FALSE;
#endif
                            }
                        }
                        else
                            bVolume = FALSE;
		                break;
					case PH_MENU:
						// allow user to escape to music mode
						// set flag so PR_MENU in music mode will ignore
						iNextMenu = MENU_MUSIC;
						g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
						bDone = TRUE;
						break;
					case PR_RECORD:
				        if (g_browse.type == PL5_VOICETYPE)
				        {
        					iNextMenu = MENU_VOICE;
        					g_MenuFlags.MENU_FLAG_ESCAPE_TO_RECORD = TRUE;
		        			bDone = TRUE;
				        }
				        break;
				    case PH_VOL_UP:
				    case PH_VOL_DOWN:
		                // PH_VOL_UP, PH_VOL_DOWN to adjust volume
				        bVolume = TRUE;
		            default:
		                bSendToPlayerStateMachine = TRUE;
						break;
	            }
	            break;
		}

        if(bSendToPlayerStateMachine)
            DisplayHint.I |= SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,pPtr);

 	}

    /* Reset browse search to initial state when exit browse menu. */
    SysPostMessage(3, PL5_BR_MSG_RESET, (int)(&g_browse));

    // resume playback background count.
    SysPostMessage(2, PL5_PL_MSG_ENACNT);
  	SysWaitOnEvent(0, 0, 0);
    /* Playback queue may be out of date, reinitialize it. */
    if (g_Rebuild)
    {
        SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_Initialize,0,0,0);
        SysCallFunction(RSRC_PLAYERLIB_EXTRA_CODE_BANK,PlayerLib_GetCurrentSong,0,0,0);
    }


    return iNextMenu;
}

/*----------------------------------------------------------------------------

   Function Name:  void _reentrant DisplayLine (int x, int y, PL5_BR_VIEW * view, PL5_BR_QUEUE * queue, int offset)

   FunctionType:   Reentrant

   Inputs:        1) x, y of display position
                  2) highlight or not
                  3) a pointer to folder record.

   Outputs:       None

   Description:  Display one folder record at specified position

----------------------------------------------------------------------------*/
void _reentrant DisplayLine (int x, int y, PL5_BR_VIEW * view, PL5_BR_QUEUE * queue, int offset)
{
    _packed BYTE pString[MAX_FILENAME_LENGTH];
    UCS3 pUString[LFN_LENGTH];
    BOOL unicode = FALSE;
    PL5_FOLDER_RECORD * record;
    int ret = PLAYLIST_TRACK_NOT_FOUND;
    int highlight;

    /* Check if the content of this line is valid. */
    if (!InValidRange(queue, offset))
        return;

    highlight  = (view->highlight+view->top_index_in_queue)%queue->size;

    /* Clear this line first. */
    SysPostMessage(6,LCD_CLEAR_RANGE,x,y,SONG_TITLE_X_SIZE, MENU_ITEM_HEIGHT);

    record = queue->pos+offset;

    if (record->bIsDir)
    {
        pString[0] = 0x2B;  //  '+'
        SysPostMessage(5,LCD_PRINT_STRING_ADDR,x+6,y,pString);
        SysWaitOnEvent(0,0,0);  // allow pString reusable for other content.
    }

    /* Display LFN, and display SFN if LFN does not exist. */

    if (!record->bIsRoot)
    {
        LONG fastkey = 0;

        fastkey = view->folder.lFastKey;
        ret = SysCallFunction(RSRC_PLAYLIST_CODEBANK, Playlist_GetLFN, (INT)(&fastkey), record->iRecordNum, (INT *)pUString);
        if (ret == PLAYLIST_SUCCESS)
        {
            unicode = TRUE;
        }
    }

    if (ret != PLAYLIST_SUCCESS)
    {
        if (Playlist_GetSFN(record, pString) != PLAYLIST_SUCCESS)
            __asm("debug");     // A FS error occur
    }


    if (offset == (view->highlight+view->top_index_in_queue)%queue->size)
    {   // this is the highlight line
        int length,length2;
        int i = 0;

        if (unicode)
            length = GetTextWidthAddressUnicode(pUString);
        else
            length = GetTextWidthAddressDBCS(pString);


        if ((length+12) > SCREEN_WIDTH)
        {
            // scroll display the highlighted line
            SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_ANIMATE,1,ANIMATE_PERIOD,MENU_MSG_ANIMATE);

            x -= scrolling_offset;
            if (x < 12)
            {
                while (1)
                {
                    if (unicode)
                        length2=GetTextWidthAddressUnicode(pUString+i);
                    else
                        length2=GetTextWidthAddressDBCS(pString+i);
                    if ((length - length2 + x) > 12)
                        break;
                    i++;
                }
                if (length2 == 0)
                {
                    scrolling_offset = 12 - SCREEN_WIDTH;
                }
                x = 12;
            }
        }
        else
        {
            scrolling_offset = -15;
            x += 12;
        }
        if (unicode)
            SysPostMessage(5,LCD_PRINT_STRING_UNICODE_INV_ADDR,x,y,pUString+i);
        else
            SysPostMessage(5,LCD_PRINT_STRING_INV_ADDR,x,y,pString+i);
    }
    else
    {
        x += 12;

        if (unicode)
            SysPostMessage(5,LCD_PRINT_STRING_UNICODE_ADDR,x,y,pUString);
        else
            SysPostMessage(5,LCD_PRINT_STRING_ADDR,x,y,pString);
    }

    SysWaitOnEvent(0,0,0);  // display content in local pString

}

/*----------------------------------------------------------------------------

   Function Name:  void _reentrant DisplayPage(int x, int y, PL5_BROWSE * browse)

   FunctionType:   Reentrant

   Inputs:        1) Pointer to browse descripter

   Outputs:       None

   Description:  Display a page

----------------------------------------------------------------------------*/
void _reentrant DisplayPage(int x, int y, PL5_BROWSE * browse)
{
    PL5_BR_QUEUE * queue;
    PL5_BR_VIEW * current;
    int i, offset;

    queue = browse->queue;
    current = browse->current;

    // pause background search for quick refresh display
    SysPostMessage(3, PL5_BR_MSG_PAUSE, browse);
    SysPostMessage(2, PL5_PL_MSG_PAUSE);
    SysWaitOnEvent(0,0,0);

    scrolling_offset = -15;  // reset scrolling

    offset = current->top_index_in_queue;
    for (i=0; i<current->pagesize; i++)
    {
        DisplayLine(x, y, current, queue, offset);
        y += MENU_ITEM_HEIGHT;
        offset = (offset+1)%queue->size;
    }

    if (i < PL5_BR_PAGE_SIZE)
    {
        SysPostMessage(6,LCD_CLEAR_RANGE,x,y,SONG_TITLE_X_SIZE,(PL5_BR_PAGE_SIZE-i) * MENU_ITEM_HEIGHT);
    }

    // resume search status
    SysPostMessage(3, PL5_BR_MSG_RESUME, browse);
    SysPostMessage(2, PL5_PL_MSG_RESUME);
    SysWaitOnEvent(0,0,0);
}

/*----------------------------------------------------------------------------

   Function Name:  void _reentrant DisplayViewTitle(int x, int y, PL5_BROWSE * browse)

   FunctionType:   Reentrant

   Inputs:        1) Pointer to browse descripter

   Outputs:       None

   Description:  Display view title. For view of virtual folder, the title is menu title,
                 otherwise it is current folder name.

----------------------------------------------------------------------------*/
void _reentrant DisplayViewTitle(int x, int y, PL5_BROWSE * browse)
{
    _packed BYTE pString[MAX_FILENAME_LENGTH];
    UCS3 pUString[LFN_LENGTH];
    PL5_BR_VIEWSTACK * viewstack;
    PL5_BR_VIEW * current;
    LONG fastkey = 0;
    INT tmp = PLAYLIST_SUCCESS + 1; // tmp != PLAYLIST_SUCCESS by default

    viewstack = browse->viewstack;
    current = browse->current;

    // Clear this line first
    SysPostMessage(6,LCD_CLEAR_RANGE,x,y,SONG_TITLE_X_SIZE,(PL5_BR_PAGE_SIZE+1) * MENU_ITEM_HEIGHT);

    pString[0] = 0x00002D;  //  '-'
    SysPostMessage(5,LCD_PRINT_STRING_ADDR,x,y,pString);
    SysWaitOnEvent(0,0,0);  // allow pString reusable for other content.
    x += 6;

    if (browse->viewstack->bottom == 0)
    {   // It is a view of Virtual folder, display MENU title
        SysPostMessage(5,LCD_PRINT_STRING_RSRC,x,y,g_iTitleResource);
    }
    else
    {   // It is a view of real folder, display folder's name (LFN or SFN)
        if (browse->viewstack->bottom > 1)
        {   // It is not in root directory.
            fastkey = (viewstack->pos+viewstack->bottom - 1)->folder.lFastKey;

            if (!current->folder.bIsRoot &&
                ((tmp = SysCallFunction(RSRC_PLAYLIST_CODEBANK, Playlist_GetLFN,
                (INT)(&fastkey), current->folder.iRecordNum, (INT *)pUString)) == PLAYLIST_SUCCESS))
            {
                SysPostMessage(5,LCD_PRINT_STRING_UNICODE_ADDR,x+6,y,pUString);
            }
        }

        if (tmp != PLAYLIST_SUCCESS)
        {
            if (Playlist_GetSFN(&(current->folder), pString) != PLAYLIST_SUCCESS)
                __asm("debug");     // A FS error occur
            SysPostMessage(5,LCD_PRINT_STRING_ADDR,x+6,y,pString);
        }
    }
    SysWaitOnEvent(0,0,0);  // display contents in local pString[] before exit this function.
}

/*----------------------------------------------------------------------------

   Function Name:  void _reentrant DisplayView(int x, int y, PL5_BROWSE * browse)

   FunctionType:   Reentrant

   Inputs:        1) Pointer to browse descripter

   Outputs:       None

   Description:  Display a view. View consists of title and page, and page consists of lines.

----------------------------------------------------------------------------*/
void _reentrant DisplayView(int x, int y, PL5_BROWSE * browse)
{
    int i,temp;
    int qhead,qtail;

    // Display view title first
    DisplayViewTitle(x, y, browse);

    y+=MENU_ITEM_HEIGHT;

    // Display page
    DisplayPage(x, y, browse);
}

/*----------------------------------------------------------------------------

   Function Name:  _reentrant BOOL InValidRange(PL5_BR_QUEUE * queue, int offset)

   FunctionType:   Reentrant

   Inputs:        1) Pointer to queue descripter
                  2) offset in the queue buffer

   Outputs:       None

   Description:  chech if offset is in valid range of the queue

----------------------------------------------------------------------------*/
_reentrant BOOL InValidRange(PL5_BR_QUEUE * queue, int offset)
{
    if (queue->tail > queue->head)
    {
        return (offset>=queue->head && offset<queue->tail);
    }
    else if (queue->tail < queue->head)
    {
        return (!(offset>=queue->tail && offset<queue->head));
    }
    else
        return FALSE;
}

/*----------------------------------------------------------------------------

   Function Name:  void _reentrant BrowseView_Down(PL5_BROWSE * browse)

   FunctionType:   Reentrant

   Inputs:        1) Pointer to browse descripter

   Outputs:       None

   Description:  Move down the list

----------------------------------------------------------------------------*/
void _reentrant BrowseView_Down(PL5_BROWSE* browse)
{
    PL5_BR_VIEW * current;
    PL5_BR_QUEUE * queue;
    int highlight, next;
    BOOL wrap_around = FALSE;
    BOOL new_page = FALSE;
    int i, j;

    if ((browse->viewstack->bottom == 1) && (browse->type != PL5_MUSICTYPE)
#ifdef AUDIBLE
     && (browse->type != PL5_AUDIBLETYPE)
#endif
     && (browse->type != PL5_DELETETYPE))
    {
        // these are cases that only one item in current view.
        return;
    }

    current = browse->current;
    queue = browse->queue;

    highlight   = (current->top_index_in_queue + current->highlight) % queue->size;
    next        = (current->top_index_in_queue + current->highlight + 1) % queue->size;
#ifdef MMC
    i = current->highlight;
#endif
    SysCallFunction(RSRC_PL5_BROWSEMENU_EXTRA, PL5_BR_StartSearch, FORWARD, BACKGROUND, (int*)browse);
    if (browse->viewstack->bottom == 0)
    {   // case of virtual folder
#ifdef MMC
        if (g_is_SD_inserted == HAS_SD)
        {
            current->highlight = (i + 1) & 1;
            /* Update display */
            DisplayPage(BROWSE_PAGE_X_POS, BROWSE_PAGE_Y_POS, browse);
        }
#endif
        return;
    }

    if (InValidRange(queue, next))
    {
		PL5_FOLDER_RECORD* curr_rec;
		PL5_FOLDER_RECORD* next_rec;

		curr_rec = queue->pos+highlight;
		next_rec = queue->pos+next;

        /* set new top_index_in_queue and new highlight line offset */
        if (current->highlight == (current->pagesize - 1))
        {
		    if ((!curr_rec->bIsDir && next_rec->bIsDir)
		    || ((curr_rec->bIsDir == next_rec->bIsDir) && (curr_rec->iRecordNum >= next_rec->iRecordNum)))
		    {
		        // case of wrap around
		        wrap_around = TRUE;
		        new_page = TRUE;
		        current->top_recordnum = (queue->pos+next)->iRecordNum;
		        current->highlight = 0;
		    }
#ifdef LINE_BY_LINE
		    else
		    {
                current->top_index_in_queue = (current->top_index_in_queue + 1) % queue->size;
                current->top_recordnum = (queue->pos+current->top_index_in_queue)->iRecordNum;
            }
#else
            new_page = TRUE;
#endif
        }
        else
            current->highlight ++;

        /* set new page size */
        if (new_page)
        {
            current->top_index_in_queue = next;
            current->highlight = 0;
            current->top_recordnum = (queue->pos+current->top_index_in_queue)->iRecordNum;
            current->pagesize = 1;

            for(i=0; i<2; i++)
            {
                if (next < queue->tail)
                    j = queue->tail - next;
                else
                    j = queue->tail + queue->size - next;

                if (j >= PL5_BR_PAGE_SIZE)
                {
                    j = PL5_BR_PAGE_SIZE;
                    break;
                }
                else
                    SysCallFunction(RSRC_PL5_BROWSEMENU_EXTRA, PL5_BR_StartSearch, FORWARD, FOREGROUND, (int*)browse);
            }

            for (i=1, curr_rec = queue->pos + next; i<j; i++)
            {
                next = (next + 1) % queue->size;
                next_rec = queue->pos + next;

		        if ((!curr_rec->bIsDir && next_rec->bIsDir)
		        || ((curr_rec->bIsDir == next_rec->bIsDir) && (curr_rec->iRecordNum >= next_rec->iRecordNum)))
		        {
		            // found the point of wrap around
		            break;
		        }
                curr_rec = next_rec;
            }

            current->pagesize = i;
        }

        /* update current file number in current folder */
        if (curr_rec->bIsDir)
		    g_PL5_iCurFileInCurFolder = 1;
		else if (wrap_around)
	        g_PL5_iCurFileInCurFolder = 1;
	    else
            g_PL5_iCurFileInCurFolder ++;

        /* Update display */
#ifdef LINE_BY_LINE
        if (current->highlight == (current->pagesize - 1))
#else
        if (new_page)
#endif
            DisplayPage(BROWSE_PAGE_X_POS, BROWSE_PAGE_Y_POS, browse);
        else
        {
            i = (current->top_index_in_queue + current->highlight) % queue->size;
            j = BROWSE_PAGE_Y_POS+MENU_ITEM_HEIGHT*(current->highlight);
            DisplayLine(BROWSE_PAGE_X_POS, j, current, queue, i);
            i = (i + queue->size - 1) % queue->size;
            j -= MENU_ITEM_HEIGHT;
            DisplayLine(BROWSE_PAGE_X_POS, j, current, queue, i);
        }
    }
    else
    {
        // we reach here may be because there is no valid items in current folder,
        // or the highlight is the last valid item and no next valid item is found so far,
        // so we just do nothing and return.
        ;
    }
}


/*----------------------------------------------------------------------------

   Function Name:  void _reentrant BrowseView_Up(PL5_BROWSE * browse)

   FunctionType:   Reentrant

   Inputs:        1) Pointer to browse descripter

   Outputs:       None

   Description:  Move up the list

----------------------------------------------------------------------------*/
void _reentrant BrowseView_Up(PL5_BROWSE * browse)
{
    PL5_BR_VIEW * current;
    PL5_BR_QUEUE * queue;
    PL5_FOLDER_RECORD * record;
    int highlight, prev;
    BOOL wrap_around = FALSE;
    BOOL new_page = FALSE;
    int i,j,k;

    current = browse->current;
    queue = browse->queue;

    if ((browse->viewstack->bottom == 1) && (browse->type != PL5_MUSICTYPE)
#ifdef AUDIBLE
     && (browse->type != PL5_AUDIBLETYPE)
#endif
     && (browse->type != PL5_DELETETYPE))
    {
        // these are cases that only one item in current view.
        return;
    }

    highlight   = (current->top_index_in_queue + current->highlight) % queue->size;
    prev        = (current->top_index_in_queue + current->highlight + queue->size - 1) % queue->size;

#ifdef MMC
    i = current->highlight;
#endif
    SysCallFunction(RSRC_PL5_BROWSEMENU_EXTRA, PL5_BR_StartSearch, BACKWARD, BACKGROUND, (int*)browse);
    if (browse->viewstack->bottom == 0)
    {   // case of virtual folder
#ifdef MMC
        if (g_is_SD_inserted == HAS_SD)
        {
            current->highlight = (i - 1) & 1;
            /* Update display */
            DisplayPage(BROWSE_PAGE_X_POS, BROWSE_PAGE_Y_POS, browse);
        }
#endif
        return;
    }

    if (InValidRange(queue, prev))
    {
        PL5_FOLDER_RECORD* curr_rec;
        PL5_FOLDER_RECORD* prev_rec;

		curr_rec = queue->pos+highlight;
		prev_rec = queue->pos+prev;

        /* set new top_index_in_queue and new highlight line offset */
        if (current->highlight == 0)
        {
            if ((curr_rec->bIsDir && !prev_rec->bIsDir)
             || ((curr_rec->bIsDir == prev_rec->bIsDir) && (curr_rec->iRecordNum <= prev_rec->iRecordNum)))
            {
                // case of wrap around
                wrap_around = TRUE;
                new_page = TRUE;
            }
#ifdef LINE_BY_LINE
            else
            {
                current->top_index_in_queue = prev;
                current->top_recordnum = (queue->pos+prev)->iRecordNum;
                if (current->pagesize < PL5_BR_PAGE_SIZE)
                    current->pagesize++;
            }
#else
            new_page = TRUE;
#endif
        }
        else
            current->highlight --;

        /* set new page size */
        if (new_page)
        {
            current->top_index_in_queue = prev;
            current->pagesize = 1;
            if (wrap_around)
            {
                k = (current->files + current->folders) % PL5_BR_PAGE_SIZE;
                if (k == 0)
                    k = PL5_BR_PAGE_SIZE;
            }
            else
                k = PL5_BR_PAGE_SIZE;

            if (prev >= queue->head)
                j = prev - queue->head + 1;
            else
                j = prev + queue->size - queue->head + 1;

            if (j < k)
                SysCallFunction(RSRC_PL5_BROWSEMENU_EXTRA, PL5_BR_StartSearch, BACKWARD, FOREGROUND, (int*)browse);

            current->pagesize = k;
            current->top_index_in_queue = (prev + queue->size + 1 - k) % queue->size;
            current->top_recordnum = (queue->pos+current->top_index_in_queue)->iRecordNum;
            current->highlight = k - 1;
        }

        /* update current file number in current folder */
        curr_rec = queue->pos + (current->top_index_in_queue +  current->highlight) % queue->size;
        if (curr_rec->bIsDir)
            g_PL5_iCurFileInCurFolder = 0;
        else if (wrap_around)
            g_PL5_iCurFileInCurFolder = current->files;
        else
            g_PL5_iCurFileInCurFolder --;

        /* Update display */
#ifdef LINE_BY_LINE
        if (current->highlight == 0)
#else
        if (new_page)
#endif
            DisplayPage(BROWSE_PAGE_X_POS, BROWSE_PAGE_Y_POS, browse);
        else
        {
            i = (current->top_index_in_queue + current->highlight) % queue->size;
            j = BROWSE_PAGE_Y_POS+MENU_ITEM_HEIGHT*(current->highlight);
            DisplayLine(BROWSE_PAGE_X_POS, j, current, queue, i);
            i = (i + 1) % queue->size;
            j += MENU_ITEM_HEIGHT;
            DisplayLine(BROWSE_PAGE_X_POS, j, current, queue, i);
        }
    }
    else
    {
        // we reach here may be because there is no valid items in current folder,
        // or the highlight is the first valid item and no prev valid item is found so far,
        // so we just do nothing and return.
        ;
    }
}
