////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2004-2007
//
// File        : jpegthumbnailmenu.c
// Description : Jpeg thumb nail menu
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
#ifdef   USE_PLAYLIST3
#include "jpegfileutil.h"
#endif
#include "sysspeed.h"

#define PAN_AMOUNT 60

void _reentrant StartJpegCallbackTimer(void);
void _reentrant KillJpegCallbackTimer(void);
void _reentrant Jpeg_LoadDecoder(void);
void _reentrant Jpeg_Helper( _packed BYTE* szFilename, INT iCleanScreen );

#if (LCD_SIZE_X/THUMBNAIL_X) < (THUMBNAIL_X_OFFSET * 2)
#error "x offset in thumbnail mode is too large"
#endif
#if (LCD_SIZE_Y/THUMBNAIL_Y) < (THUMBNAIL_Y_OFFSET * 2)
#error "y offset in thumbnail mode is too large"
#endif

void _reentrant RemovePath( void* szFilename )
{
	int a,b,c;

	a = packed_strlen( szFilename );
	for( b = a - 1; b >= 0; b-- )
		if( packed_get( szFilename, b ) == '/' )
		{
			b++;
			for( c = 0; c < a - b; c++ )
				packed_set( szFilename, c, packed_get( szFilename, b + c ) );
			packed_set( szFilename, c, '\0' );
			return;
		}
}

void _reentrant UpdateThumbnailXY( INT iOffset )
{
	INT x,y,Lcd_sizeY,Lcd_sizeX;
    x = (iOffset % THUMBNAIL_X) * (LCD_SIZE_X / THUMBNAIL_X);
    y = (iOffset / THUMBNAIL_X) * (LCD_SIZE_Y / THUMBNAIL_Y);
     Lcd_sizeX=LCD_SIZE_X / THUMBNAIL_X;
     if((LCD_SIZE_X%THUMBNAIL_X)!=0)
           Lcd_sizeX++;
           
     Lcd_sizeY=LCD_SIZE_Y / THUMBNAIL_Y;
     if((LCD_SIZE_Y%THUMBNAIL_Y)!=0)
           Lcd_sizeY++;
    Jpeg_UpdateDisplayParams(THUMBNAIL_X_OFFSET+x,
                             THUMBNAIL_Y_OFFSET+y,
		Lcd_sizeX - THUMBNAIL_X_OFFSET*2,
		Lcd_sizeY - THUMBNAIL_Y_OFFSET*2);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int _reentrant JpegThumbnailMenu(int a, int b, int *pPtr)
//
//   Type:          Function (user menu)
//
//   Description:   This function is the jpeg thumbnail menu
//
//   Inputs:        none used.
//
//   Outputs:       value of which menu to return to
//
//   Notes:         This function handles the jpeg thumbnail menu.  It loops until a reason
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
int _reentrant JpegThumbnailMenu(int a, int b, int *pPtr)
{

    union DisplayHints DisplayHint;
	WORD bDecodeProgress = FALSE;

    INT  iEvent;
    BOOL bDone = FALSE;
    INT  iNewMenu = MENU_MANUAL_MODE;
    BOOL bSendToPlayerStateMachine = FALSE;
	INT x;
	INT y;
	INT iThumbnailOffset;
	INT iSelectedOffset;
	INT iPreviousSelectOffset;
	int i;

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
	if( JpegFile_GetCurrentSongFileInfo( 0, 0, (INT*)&SongInfo ) != PLAYLIST_SUCCESS )
#else
	if( SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetCurrentSongFileInfo, 0, 0, (INT*)&SongInfo ) != PLAYLIST_SUCCESS )
#endif	
		return MENU_MAIN;
    
	SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_ANIMATE);
	SysWaitOnEvent(0,0,0);
	SysSpeedIncrease(SPEED_PEAK, SPEED_CLIENT_JPEG_DEC);

	x = 0;
	y = 0;
	iThumbnailOffset = 0;
	iSelectedOffset = -1;
	iPreviousSelectOffset = -1;

	//*** sequence to be followed before each new file is decoded and displayed ***
	Jpeg_LoadDecoder();
	Jpeg_FillRectangleColor(0,0,LCD_SIZE_X,LCD_SIZE_Y,RGB(0xFF,0xFF,0xFF));
	a = SysCallFunction(RSRC_PLAYLIST2INIT_CODEBANK, Playlist_GetFileExtention, 0, 0, (INT*)szFilename);
	if (a == JPG_FILE_EXT)
		Jpeg_SetFile(szFilename, 0);
	else
		BMP_SetFile(szFilename, 0);
	Jpeg_DisplayGrayScale(FALSE);
	UpdateThumbnailXY(iThumbnailOffset);
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
                            bDecodeProgress = FALSE;
						    //either we reached end of file or there was an error in decoding 
						    if( iThumbnailOffset < (THUMBNAIL_X * THUMBNAIL_Y - 1)
#ifdef USE_PLAYLIST5
							    && g_wCurrentSongNumber < g_wTotalSongCount )
#else
							    && g_CurrentTrack.iTrackOrder < g_iTotalTracks )
#endif							    
						    {
							    //if the updated position is valid, draw the thumbnail
#ifdef USE_PLAYLIST3    
                                if( JpegFile_GetNextSongFileInfo(0, 0, (INT*)&SongInfo) == PLAYLIST_SUCCESS )
#else
							    if( SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetNextSongFileInfo, 
								    0, 0, (INT*)&SongInfo ) == PLAYLIST_SUCCESS )
#endif								    
                                    {
                                        UpdateThumbnailXY( ++iThumbnailOffset );
                                        Jpeg_Helper( szFilename, 0 );
                                        bDecodeProgress = TRUE;
                                    }
						    }
						    else if( iSelectedOffset == -2 )
						    {
#ifdef USE_PLAYLIST3    
                                JpegFile_GetPreviousSongFileInfo( 0, iThumbnailOffset - 1, (INT*)&SongInfo );
#else
#ifdef USE_PLAYLIST5
								for(i = 0; i < (iThumbnailOffset); i++)
								{
									SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetPreviousSongFileInfo, 0, 0, (INT*)&SongInfo );
								}
#else
								SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetPreviousSongFileInfo, 0, iThumbnailOffset - 1, (INT*)&SongInfo );
#endif
#endif							    
                                iSelectedOffset = 0;
    							SysPostMessage(2, MENU_JPEG_THUMBNAIL_CURSOR );
						    }
						    else if( iSelectedOffset == -3 )
						    {
							    iSelectedOffset = iThumbnailOffset;
							    SysPostMessage(2, MENU_JPEG_THUMBNAIL_CURSOR );
					        }
                        }
					}
                    bSendToPlayerStateMachine = FALSE;
                    break;
				case MENU_JPEG_THUMBNAIL_CURSOR:
					if( iPreviousSelectOffset != -1 )
					{ //erase the previous cursor
						SysPostMessage( 6, LCD_CLEAR_RANGE, 
							//(LCD_SIZE_X / THUMBNAIL_X) * (iPreviousSelectOffset % THUMBNAIL_X), 
							0,
							(LCD_SIZE_Y / THUMBNAIL_Y) * (iPreviousSelectOffset / THUMBNAIL_X + 1) - CHAR_SIZE_Y,
							//(LCD_SIZE_X / THUMBNAIL_X), 
							LCD_SIZE_X,
							CHAR_SIZE_Y );
					}
					iPreviousSelectOffset = iSelectedOffset;
					//draw cursor
					if( iSelectedOffset >= 0 )
					{
						RemovePath( (void*)szFilename );
						SysPostMessage(5, LCD_PRINT_STRING_ADDR, 
							(LCD_SIZE_X / THUMBNAIL_X) * (iSelectedOffset % THUMBNAIL_X), 
							(LCD_SIZE_Y / THUMBNAIL_Y) * (iSelectedOffset / THUMBNAIL_X + 1) - CHAR_SIZE_Y,
							szFilename );
					}
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
            case PH_STOP:   // when back from shut down menu, as if AB button is pressed to refresh the jpegs.
                //if the STOP button is held down, lets call the shutdown menu
                SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,ShutdownMenu,USERSHUTDOWN,0,0);
                // if returns update display  -- if doesn't return, powered down
                //DisplayHint.I |= 1<<DISPLAY_CLEAR_DISPLAY_BITPOS; 
                //break;

            case PR_AB:
#ifdef USE_PLAYLIST5
                if( g_wCurrentSongNumber > 1 )
#else
                if( g_CurrentTrack.iTrackOrder > 1 )
#endif
                {
#ifdef USE_PLAYLIST3    
					JpegFile_GetPreviousSongFileInfo( 0, g_CurrentTrack.iTrackOrder - 2, (INT*)&SongInfo );
#else
#ifdef USE_PLAYLIST5
					SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetPreviousSongFileInfo, 0, 0, (INT*)&SongInfo );
#else
				    SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetPreviousSongFileInfo, 0, g_CurrentTrack.iTrackOrder - 2, (INT*)&SongInfo );
#endif
#endif			
                }else
                {
#ifdef USE_PLAYLIST3    
					JpegFile_GetCurrentSongFileInfo( 0, 0, (INT*)&SongInfo );
#else
				    SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetCurrentSongFileInfo, 0, 0, (INT*)&SongInfo );
#endif			
                }	
                Jpeg_Helper( szFilename, 1 );
                iSelectedOffset = -1;
				x = 0;
				y = 0;
				iThumbnailOffset = 0;
                UpdateThumbnailXY( iThumbnailOffset );
                Jpeg_Helper( szFilename, 1 );
				bDecodeProgress = TRUE;
                break;
			case PR_STOP:
				if( iSelectedOffset < 0 )
				{
#ifdef USE_PLAYLIST3    
					JpegFile_GetPreviousSongFileInfo( 0, iThumbnailOffset - 1, (INT*)&SongInfo );
#else
#ifdef USE_PLAYLIST5
					for(i = 0; i < (iThumbnailOffset); i++)
					{
						SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetPreviousSongFileInfo, 0, 0, (INT*)&SongInfo );
					}
#else
					SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetPreviousSongFileInfo, 0, iThumbnailOffset - 1, (INT*)&SongInfo );
#endif
#endif					

					iSelectedOffset = 0;
					SysPostMessage(2, MENU_JPEG_THUMBNAIL_CURSOR );
                    bDecodeProgress = FALSE;
				}
				else
				{
					if( iSelectedOffset < iThumbnailOffset )
					{	
#ifdef USE_PLAYLIST3    
						JpegFile_GetNextSongFileInfo( 0, iThumbnailOffset - iSelectedOffset - 1, (INT*)&SongInfo );
#else
						SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetNextSongFileInfo, 0, iThumbnailOffset - iSelectedOffset - 1, (INT*)&SongInfo );
#endif						
                    }
					if( iThumbnailOffset < iSelectedOffset )
					{	
#ifdef USE_PLAYLIST3    
						JpegFile_GetPreviousSongFileInfo( 0, iSelectedOffset - iThumbnailOffset - 1, (INT*)&SongInfo );
#else
#ifdef USE_PLAYLIST5
						for(i = 0; i < (iSelectedOffset - iThumbnailOffset); i++)
						{
							SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetPreviousSongFileInfo, 0, 0, (INT*)&SongInfo );
						}
#else
						SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetPreviousSongFileInfo, 0, iSelectedOffset - iThumbnailOffset - 1, (INT*)&SongInfo );
#endif
#endif						
                    }
                    if( iThumbnailOffset == iSelectedOffset )
                    {   
#ifdef USE_PLAYLIST3    
						JpegFile_GetCurrentSongFileInfo( 0, 0, (INT*)&SongInfo );                    
#else
                        SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetCurrentSongFileInfo, 0, 0, (INT*)&SongInfo );                    
#endif                        
                    }
					iSelectedOffset = -1;
					SysPostMessage(2, MENU_JPEG_THUMBNAIL_CURSOR );
					if( iThumbnailOffset < THUMBNAIL_X * THUMBNAIL_Y - 1 
#ifdef USE_PLAYLIST5
						&& g_wCurrentSongNumber < g_wTotalSongCount )
#else
						&& g_CurrentTrack.iTrackOrder < g_iTotalTracks )
#endif
                    {
                        UpdateThumbnailXY( iThumbnailOffset );
                        Jpeg_Helper( szFilename, 0 );
                        bDecodeProgress = TRUE;
                    }
				}
				break;
			case PR_RW:
#ifdef USE_PLAYLIST5
				if( iSelectedOffset > 0 && g_wCurrentSongNumber != 1 )
#else
				if( iSelectedOffset > 0 && g_CurrentTrack.iTrackOrder != 1 )
#endif
				{
					iSelectedOffset--;
#ifdef USE_PLAYLIST3    
					JpegFile_GetPreviousSongFileInfo( 0, 0, (INT*)&SongInfo );
#else
					SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetPreviousSongFileInfo, 0, 0, (INT*)&SongInfo );
#endif					
					SysPostMessage(2, MENU_JPEG_THUMBNAIL_CURSOR );
				}
				else if( iSelectedOffset == 0 )
				{
					iThumbnailOffset = 0;
					iSelectedOffset = -3;
					SysPostMessage(2, MENU_JPEG_THUMBNAIL_CURSOR );
				}

				if( iSelectedOffset < 0 )
				{
#ifdef USE_PLAYLIST5
					if( g_wCurrentSongNumber <= THUMBNAIL_X * THUMBNAIL_Y )
					    b = g_wTotalSongCount % 4;
					else 
						b = 4;
#else
					if( g_CurrentTrack.iTrackOrder <= THUMBNAIL_X * THUMBNAIL_Y )
					    b = g_iTotalTracks % 4;
					else 
						b = 4;
#endif
                    if(iSelectedOffset != -3)
#ifdef USE_PLAYLIST5
                    {	if((g_wCurrentSongNumber >= 5)&&(g_wCurrentSongNumber <= 7))
    					    b = g_wCurrentSongNumber - 4;
                    }else
                    {	if((g_wCurrentSongNumber >= 2)&&(g_wCurrentSongNumber <= 4))
    					    b = g_wCurrentSongNumber - 1;
                    }
#else
                    {	if((g_CurrentTrack.iTrackOrder >= 5)&&(g_CurrentTrack.iTrackOrder <= 7))
    					    b = g_CurrentTrack.iTrackOrder - 4;
                    }else
                    {	if((g_CurrentTrack.iTrackOrder >= 2)&&(g_CurrentTrack.iTrackOrder <= 4))
    					    b = g_CurrentTrack.iTrackOrder - 1;
                    }
#endif
                    if(b == 0) 
                        b = 4;						
#ifdef USE_PLAYLIST3    
					JpegFile_GetPreviousSongFileInfo( 0, iThumbnailOffset + b - 1, (INT*)&SongInfo );
#else
#ifdef USE_PLAYLIST5
					for(i = 0; i < (iThumbnailOffset + b); i++)
					{
						SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetPreviousSongFileInfo, 0, 0, (INT*)&SongInfo );
					}
#else
					SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetPreviousSongFileInfo, 0, iThumbnailOffset + b - 1, (INT*)&SongInfo );
#endif
#endif					
					x = 0;
					y = 0;
					iThumbnailOffset = 0;
                    UpdateThumbnailXY( iThumbnailOffset );
                    Jpeg_Helper( szFilename, 1 );
					bDecodeProgress = TRUE;
				}
				break;
			case PR_FF:
				if( iSelectedOffset >= 0 && iSelectedOffset < (THUMBNAIL_X * THUMBNAIL_Y-1) 
#ifdef USE_PLAYLIST5
					&& (g_wCurrentSongNumber) != g_wTotalSongCount )
#else
					&& g_CurrentTrack.iTrackOrder != g_iTotalTracks )
#endif
				{
					iSelectedOffset++;
#ifdef USE_PLAYLIST3    
					JpegFile_GetNextSongFileInfo( NEXTSONG_PLAY_BUTTON, 0, (INT*)&SongInfo );
#else
					SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetNextSongFileInfo, NEXTSONG_PLAY_BUTTON, 0, (INT*)&SongInfo );
#endif				
					SysPostMessage(2, MENU_JPEG_THUMBNAIL_CURSOR );
				}
				else if( iSelectedOffset == (THUMBNAIL_X * THUMBNAIL_Y-1) 
#ifdef USE_PLAYLIST5
					|| (g_wCurrentSongNumber) == g_wTotalSongCount )
#else
					|| g_CurrentTrack.iTrackOrder == g_iTotalTracks )
#endif
				{
					iThumbnailOffset = iSelectedOffset;
					if(iSelectedOffset >= 0)
					    iSelectedOffset = -2;
					SysPostMessage(2, MENU_JPEG_THUMBNAIL_CURSOR );
				}

				if( iSelectedOffset < 0 )
				{
#ifdef USE_PLAYLIST5
					if( ((g_wCurrentSongNumber) + THUMBNAIL_X * THUMBNAIL_Y - iThumbnailOffset) > g_wTotalSongCount )
						a = g_wTotalSongCount - (g_wCurrentSongNumber+1);
#else
					if( (g_CurrentTrack.iTrackOrder + THUMBNAIL_X * THUMBNAIL_Y - iThumbnailOffset) > g_iTotalTracks )
						a = g_iTotalTracks - g_CurrentTrack.iTrackOrder;
#endif
					else
						a = THUMBNAIL_X * THUMBNAIL_Y - iThumbnailOffset - 1;
#ifdef USE_PLAYLIST3    
					JpegFile_GetNextSongFileInfo( NEXTSONG_PLAY_BUTTON, a, (INT*)&SongInfo );
#else
					SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetNextSongFileInfo, NEXTSONG_PLAY_BUTTON, a, (INT*)&SongInfo );
#endif					
					x = 0;
					y = 0;
					iThumbnailOffset = 0;
                    UpdateThumbnailXY( iThumbnailOffset );
                    Jpeg_Helper( szFilename, 1 );
					bDecodeProgress = TRUE;
				}
				break;
           
            case PR_VOL_UP:
                iNewMenu = MENU_MANUAL_MODE;
				if( iSelectedOffset < 0 )
				{	
#ifdef USE_PLAYLIST3    
					JpegFile_GetPreviousSongFileInfo( 0, iThumbnailOffset - 1, (INT*)&SongInfo );
#else
#ifdef USE_PLAYLIST5
					for(i = 0; i < (iThumbnailOffset); i++)
					{
						SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetPreviousSongFileInfo, 0, 0, (INT*)&SongInfo );
					}
#else
					SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetPreviousSongFileInfo, 0, iThumbnailOffset - 1, (INT*)&SongInfo );
#endif
#endif					
				}
                Jpeg_StopDecoder();
                bDone = TRUE;
                break;

            case PR_RECORD:
                iNewMenu = MENU_SLIDESHOW_MODE;
				if( iSelectedOffset < 0 )
				{   
#ifdef USE_PLAYLIST3    
					JpegFile_GetPreviousSongFileInfo( 0, iThumbnailOffset - 1, (INT*)&SongInfo );
#else
#ifdef USE_PLAYLIST5
					for(i = 0; i < (iThumbnailOffset); i++)
					{
						SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetPreviousSongFileInfo, 0, 0, (INT*)&SongInfo );
					}
#else
					SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetPreviousSongFileInfo, 0, iThumbnailOffset - 1, (INT*)&SongInfo );
#endif
#endif					
				}
				Jpeg_StopDecoder();
                bDone = TRUE;
                break;

            case PR_MENU:
                iNewMenu = MENU_JPEG_DISPLAY_EXIT;
                if( iSelectedOffset < 0 )
				{    
#ifdef USE_PLAYLIST3    
					JpegFile_GetPreviousSongFileInfo( 0, iThumbnailOffset - 1, (INT*)&SongInfo );
#else
#ifdef USE_PLAYLIST5
					for(i = 0; i < (iThumbnailOffset); i++)
					{
						SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetPreviousSongFileInfo, 0, 0, (INT*)&SongInfo );
					}
#else
					SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetPreviousSongFileInfo, 0, iThumbnailOffset - 1, (INT*)&SongInfo );
#endif
#endif	
				}
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


