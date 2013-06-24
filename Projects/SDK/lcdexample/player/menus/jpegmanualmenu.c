////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2004-2007
//
// File        : jpegmanualmenu.c
// Description : Jpeg manual menu
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "exec.h"
#include "menumanager.h"
#include "messages.h"
#include "resource.h"
#include "buttons.h"
#include "syscoder.h"
#include "sysmem.h"
#include "musicmenu.h"
#include "menus.h"
#include "mainmenu.h"
#include "display.h"
#include "playerlib.h"
#include "playerstatemachine.h"
#include "jpegdisplaymenu.h"
#include "deletemenu.h"
#ifdef   USE_PLAYLIST1
#include "playlist1.h"
#else
#ifdef   USE_PLAYLIST2
#include  "playlist2.h"
#else
#ifdef   USE_PLAYLIST3
#include  "playlist3.h"
#include  "jpegfileutil.h"
#else
#ifdef  USE_PLAYLIST5
#include  "playlist5.h"
#endif
#endif
#endif
#endif
#include "stringlib.h"
#include "hwequ.h"
#include "JpegDecodeDisplay.h"
#include "sysspeed.h"

#define PAN_AMOUNT 60

extern JpegControl CurrentJpeg;

void _reentrant StartJpegCallbackTimer(void);
void _reentrant KillJpegCallbackTimer(void);
void _reentrant Jpeg_LoadDecoder(void);
void _reentrant Jpeg_Helper( _packed BYTE* szFilename, INT iCleanScreen );

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int _reentrant JpegManualMenu(int a, int b, int *pPtr)
//
//   Type:          Function (user menu)
//
//   Description:   This function is the jpeg manual menu
//
//   Inputs:        none used.
//
//   Outputs:       value of which menu to return to
//
//   Notes:         This function handles the jpeg manual menu.  It loops until a reason
//                  to exit is reached (PH_STOP held long enough will return MENU_EXIT, and
//                  PR_MENU will return MENU_MAIN)
//
//                  The basic loop is...
//
//                  while (not done)
//                  {
//                      Update display if necessary
//                      Wait for event
//                      Pass event to player state handler
//                      handle event locally
//                  }
//
//                  Sometimes an event may be handled entirely in the player state handler,
//                  sometimes entirely locally, sometimes in both places.
//<
//////////////////////////////////////////////////////////////////////////////////////////
int _reentrant JpegManualMenu(int a, int b, int *pPtr)
{

    union DisplayHints DisplayHint;
	WORD bDecodeProgress = FALSE;

    INT  iEvent;
    BOOL bDone = FALSE;
    INT  iNewMenu = MENU_MANUAL_MODE;
    BOOL bSendToPlayerStateMachine = FALSE;

	_packed BYTE szFilename[MAX_FILENAME_LENGTH];
	SONGFILEINFO SongInfo;
	SongInfo.m_wBufferLength = MAX_FILENAME_LENGTH;
	SongInfo.m_pFilename = szFilename;
    // this is required to get the message into the playerstatemachine
    // casted to avoid pointer mismatch warnings
    pPtr = (int *) &gEventInfo;

    a;b;//here to prevent parameter not used messages.

     //set our current menu
    g_iCurrentMenu = MENU_MANUAL_MODE;

#ifdef BACKLIGHT
	// keep backlight always on.
	if (g_iBackLightState == BACKLIGHT_ON)	{
        SysPostMessage(2,LCD_BACKLIGHT_ON);
        SysWaitOnEvent(0,0,0);
	}
#endif

#ifdef USE_PLAYLIST3
	if(JpegFile_GetCurrentSongFileInfo(0,0,(INT*)&SongInfo) != PLAYLIST_SUCCESS)
#else
	if( SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetCurrentSongFileInfo, 0, 0, (INT*)&SongInfo ) != PLAYLIST_SUCCESS )
#endif
		return MENU_MAIN;

	SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_ANIMATE);
	SysWaitOnEvent(0,0,0);
	SysSpeedIncrease(SPEED_PEAK, SPEED_CLIENT_JPEG_DEC);

	//*** sequence to be followed before each new file is decoded and displayed ***
	Jpeg_LoadDecoder();
	Jpeg_FillRectangleColor(0,0,LCD_X_SIZE,LCD_Y_SIZE,RGB(0xFF,0xFF,0xFF));
	a = SysCallFunction(RSRC_PLAYLIST2INIT_CODEBANK, Playlist_GetFileExtention, 0, 0, (INT*)szFilename);
	if (a == JPG_FILE_EXT)
		Jpeg_SetFile(szFilename, 0);
	else
		BMP_SetFile(szFilename, 0);
	Jpeg_DisplayGrayScale(FALSE);
	Jpeg_UpdateDisplayParams(0,0,LCD_X_SIZE,LCD_Y_SIZE);
	Jpeg_InitDecoder(); //This will update the dimensions of input image in JpegApp structure
    Jpeg_ResetDecoder(); //stmp00012780

	//*** start the callback timer ***
    StartJpegCallbackTimer();
	bDecodeProgress = TRUE; //To keep calling Jpeg Decode Display.

    //Lets initially clear the entire screen and items that will be commonly displayed during Jpeg operation -
    //like battery, MMC, hold position.
    //DisplayHint.I = 1<<DISPLAY_CLEAR_DISPLAY_BITPOS;
    DisplayHint.I = 0;  // clear screen has been done by Jpeg_FillRectangleColor() previously

    while(!bDone)
    {

        //keep only display hints we're interested in displaying in this menu.
        DisplayHint.I &= 1<<DISPLAY_CLEAR_DISPLAY_BITPOS;

        iEvent = SysWaitOnEvent(EVENT_MESSAGE|EVENT_BUTTON,&gEventInfo.msg, DisplayHint.I? 1 : -1);

        //Lets assume we do want to send all these to the player state machine
        bSendToPlayerStateMachine = TRUE;

        switch(iEvent)
        {
        case EVENT_TIMER:
            //we've gotten a timeout event, so we've processed all our other events and
            //should update the display
            if(DisplayHint.I)
                SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHint.I,1,0);
            DisplayHint.I = 0;//clear out the bits
            bSendToPlayerStateMachine = FALSE;//don't need to send this to the player state machine
            break;
        case EVENT_MESSAGE:
            switch(gEventInfo.Message.wMsgCommand)
            {
				case MENU_MSG_JPEG_CALLBACK:
					if(bDecodeProgress)
					{
						if(Jpeg_DecodeDisplay() != JPEG_SUCCESS ||
                           Jpeg_DecodeDisplay() != JPEG_SUCCESS ||
                           Jpeg_DecodeDisplay() != JPEG_SUCCESS)
						{
							//either we reached end of file or there was an error in decoding
							bDecodeProgress = FALSE;
						}
					}
					bSendToPlayerStateMachine = FALSE;
                    break;
				case MENU_MSG_TURN_OFF_BACKLIGHT:
                    bSendToPlayerStateMachine = FALSE;
                    break;
                case MENU_MSG_MEDIA_CHANGED:
					// Stop the timer to prevent queue overflow during device enumeration
					KillJpegCallbackTimer();
                    iNewMenu = MENU_MAIN;
                	bDone = TRUE;
                	break;
            }
            break;
        case EVENT_BUTTON:
	        // button pressed, so restart timer if enabled
    	    UpdateAutoShutdownTimer();
            bSendToPlayerStateMachine = FALSE;

            //remember the last button event (mostly for coming out of FFWD and RWND)
            switch(g_wLastButton = gEventInfo.Button.wButtonEvent)
            {
            case PR_AB:
#ifdef USE_PLAYLIST5
			    if( g_wCurrentSongNumber > 0 )
#else
                if( g_CurrentTrack.iTrackOrder > 1 )
#endif
                {
#ifdef USE_PLAYLIST3
					JpegFile_GetPreviousSongFileInfo(0, g_CurrentTrack.iTrackOrder - 2, (INT*)&SongInfo );
#else
#ifdef USE_PLAYLIST5
    				SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetPreviousSongFileInfo, NEXTSONG_PLAY_BUTTON, 0, (INT*)&SongInfo);
#else
    				SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetPreviousSongFileInfo, 0, g_CurrentTrack.iTrackOrder - 2, (INT*)&SongInfo );
#endif
#endif
    			}
                Jpeg_Helper( szFilename, 1 );
				bDecodeProgress = TRUE;
                break;
			case PR_FF:
				if(CurrentJpeg.DecParams.zoom_no == 1 || CurrentJpeg.DecParams.zoom_no > 10)
				{
#ifdef USE_PLAYLIST3
					JpegFile_GetNextSongFileInfo(0,0,(INT*)&SongInfo);
#else
#ifdef USE_PLAYLIST5
				    SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_BATT_CHK);
					SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_AUTO_SHUTDOWN);
				    SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_ANIMATE);
					SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_JPEG_DECODER_CALLBACK);
					SysWaitOnEvent(0,0,0);
#endif
            	    SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetNextSongFileInfo, NEXTSONG_PLAY_BUTTON, 0, (INT*)&SongInfo );
#ifdef USE_PLAYLIST5
				    SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_ANIMATE,0,ANIMATE_PERIOD,MENU_MSG_ANIMATE);
					SysWaitOnEvent(0,0,0);
					SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_BATT_CHK,0,2000,MENU_BATTERY_CHNG);
					SysWaitOnEvent(0,0,10);
				   	SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_JPEG_DECODER_CALLBACK,0,TIMER_JPEG_DECODER_TIMEOUT_MS,MENU_MSG_JPEG_CALLBACK);
					SysWaitOnEvent(0,0,0);
					// button pressed, so restart timer if enabled
					UpdateAutoShutdownTimer();
#endif
#endif
                    Jpeg_Helper( szFilename, 1 );
					bDecodeProgress = TRUE;
				}
				else
				{
                    Jpeg_Helper( NULL, 1 );
					Jpeg_PanImageRight(PAN_AMOUNT);
	 				bDecodeProgress = TRUE;
				}
                break;
			case PR_RW:
				if(CurrentJpeg.DecParams.zoom_no == 1 || CurrentJpeg.DecParams.zoom_no > 10)
				{
#ifdef USE_PLAYLIST3
					JpegFile_GetPreviousSongFileInfo(0, 0, (INT*)&SongInfo );
#else
#ifdef USE_PLAYLIST5
				    SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_BATT_CHK);
					SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_AUTO_SHUTDOWN);
				    SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_ANIMATE);
					SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_JPEG_DECODER_CALLBACK);
					SysWaitOnEvent(0,0,0);
#endif
					SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetPreviousSongFileInfo, 0, 0, (INT*)&SongInfo );

#ifdef USE_PLAYLIST5
				    SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_ANIMATE,0,ANIMATE_PERIOD,MENU_MSG_ANIMATE);
					SysWaitOnEvent(0,0,0);
					SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_BATT_CHK,0,2000,MENU_BATTERY_CHNG);
					SysWaitOnEvent(0,0,10);
				   	SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_JPEG_DECODER_CALLBACK,0,TIMER_JPEG_DECODER_TIMEOUT_MS,MENU_MSG_JPEG_CALLBACK);
					SysWaitOnEvent(0,0,0);
					// button pressed, so restart timer if enabled
					UpdateAutoShutdownTimer();
#endif

#endif
                    Jpeg_Helper( szFilename, 1 );
					bDecodeProgress = TRUE;
				}
				else
				{
                    Jpeg_Helper( NULL, 1 );
					Jpeg_PanImageLeft(PAN_AMOUNT);
	 				bDecodeProgress = TRUE;
				}
				break;
			case PR_PLAY:
                Jpeg_Helper( NULL, 1 );
				Jpeg_PanImageUp(PAN_AMOUNT);
	 			bDecodeProgress = TRUE;
				break;

			case PR_STOP:
                Jpeg_Helper( NULL, 1 );
				Jpeg_PanImageDown(PAN_AMOUNT);
	 			bDecodeProgress = TRUE;
				break;

			case PR_VOL_UP:
               if (CurrentJpeg.DecParams.zoom_no <= 10 )
               {
                Jpeg_Helper( NULL, 1 );
				Jpeg_ZoomIn();
	 			bDecodeProgress = TRUE;
                }
				break;
			case PR_VOL_DOWN:
				if(CurrentJpeg.DecParams.zoom_no > 1 )
                {
                    Jpeg_Helper( NULL, 1 );
				    Jpeg_ZoomOut();
	 			    bDecodeProgress = TRUE;
                }
				else
                {
                    Jpeg_StopDecoder();
                    iNewMenu = MENU_THUMBNAIL_MODE;
                    bDone = TRUE;
                }
				break;

            case PR_ERASE:
                Jpeg_Helper( NULL, 1 );
                CurrentJpeg.DecParams.zoom_no = 1;
 	 			bDecodeProgress = TRUE;
                break;

            case PH_STOP:
                //if the STOP button is held down, lets call the shutdown menu
                SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,ShutdownMenu,USERSHUTDOWN,0,0);
                //// if returns update display  -- if doesn't return, powered down
                //DisplayHint.I |= 1<<DISPLAY_CLEAR_DISPLAY_BITPOS;
                Jpeg_Helper( NULL, 1 );
 	 			bDecodeProgress = TRUE;
                break;

            case PR_RECORD:
                iNewMenu = MENU_SLIDESHOW_MODE;
				Jpeg_StopDecoder();
                bDone = TRUE;
                break;

            case PR_MENU:
                iNewMenu = MENU_JPEG_DISPLAY_EXIT;
				Jpeg_StopDecoder();
                bDone= TRUE;
                break;

            default:

                break;
            }
            break;
        }
        if(bSendToPlayerStateMachine)
        {//only send events to the player state machine if necessary (to avoid code bank swapping)
            DisplayHint.I |= SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,pPtr);
            //or the display hints returned with the current collection of hints
        }

    }
	//*****

#ifdef BACKLIGHT
	// turn on backlight when button is pressed.
	if (g_iBackLightState==BACKLIGHT_ON)
	{
    	SysPostMessage(2,LCD_BACKLIGHT_ON);
	    SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_BACKLIGHT,1,BACKLIGHT_TIME,MENU_MSG_TURN_OFF_BACKLIGHT);
        SysWaitOnEvent(0,0,0);
	}
#endif

	//***** stop call back function for the decoder ***
	KillJpegCallbackTimer();
	SysSpeedClockFree(SPEED_CLIENT_JPEG_DEC);

    //don't exit player state machine, cause we want to keep playing when in main menu.
    //sdk2.1
    return iNewMenu;
}


