// File: 		Display.c
// Reviews:	DanhNguyen (06-2008)

/*
there is some code added to display number of refreshes if DISPLAY_MLC_STATUS is defined
*/

#include "types.h"
#include "exec.h"
#include "menumanager.h"
#include "messages.h"
#include "display.h"
#include "resource.h"
#include "menus.h"
#include "eqmenu.h"
#include "musicmenu.h"

#ifndef REMOVE_FM
#include "fmtunermenu.h"
#endif

#include "playerstatemachine.h"
#include "recorderstatemachine.h"
#include "syscoder.h"
#include "sysmem.h"
#include "metadata.h"
#include "project.h"
#ifdef	AUDIBLE
	#include "audiblesongposition.h"
#endif
#ifdef CLCD
#include "colorlcdsal.h"	//include FrameBufferID enum
#endif

#ifdef   USE_PLAYLIST1
#include "playlist1.h"
#else
#ifdef   USE_PLAYLIST2
#include  "playlist2.h"
#else
#ifdef   USE_PLAYLIST3
#include  "playlist3.h"
#include  "playmodemenu.h"
#else
#ifdef   USE_PLAYLIST5
#include  "playlist5.h"
#endif
#endif
#endif
#endif

#include "battery.h"

#ifdef BATTERY_CHARGE
#include "batterycharge.h"
#endif

#include "sysvolume.h"
#include "mainmenu.h" // current menu check for MENU_FMTUNER

///////////////////////////////////////////////////////////////////////////////
//  Prototypes
///////////////////////////////////////////////////////////////////////////////
#if DEBUG
void _reentrant UpdateMLCErrorStatus(void);
extern _reentrant INT Convert_itoa(INT Number,_packed char *string);
#endif

///////////////////////////////////////////////////////////////////////////////
//  Externs
///////////////////////////////////////////////////////////////////////////////
#if DEBUG
extern int g_NumErrors;
extern WORD wResourceTag;
#endif

#ifndef MEDIA_STATE_BITMASK
// Since only 1 removable device for either configuration this value is the same
// for both SM/MMC builds. If more than one removable device, need to add a bit
// for each device and change display function
#define MEDIA_STATE_BITMASK 1
#endif

#pragma optimize 1


_X BOOL g_bSongStringScroll=FALSE;
_X INT  g_iSongStringOffset=0;
_X INT  g_iSongStringLength=0;

_X BOOL g_bArtistStringScroll=FALSE;
_X INT  g_iArtistStringOffset=0;
_X INT  g_iArtistStringLength=0;

_X BOOL g_bAlbumStringScroll=FALSE;
_X INT  g_iAlbumStringOffset=0;
_X INT  g_iAlbumStringLength=0;


_reentrant void ClearRange(int x, int y, int cx, int cy);
_reentrant void DrawBitmap(int x, int y, int iResource);
_reentrant int DisplayContrast( int bDisplay, int iIgnored1, void *pPtr);
_reentrant int DisplayBacklight( int bDisplay, int iIgnored1, void *pPtr);
_reentrant int DisplayTitleBitmap( int bDisplay, int iIgnored1, void *pPtr);
_reentrant int DisplayPlaySet( int bDisplay, int iIgnored2, int *pPtr);


BOOL SecondsToHourMinSecStruct(int iSeconds, struct HourMinSecStruct *pStruct);
WORD _reentrant DisplayGetContrast(void);
_reentrant int GetPlayMode(void);
_asmfunc int SysGetVolume(void);
_asmfunc int EncoderGetStatus(void);
_reentrant int GetABMode(void);

_reentrant BOOL IsEncWarningActive(void); // helper functions for DisplayVoiceMode()
_reentrant int DisplayWarning(void);
_reentrant int DisplayVoiceMode(int bDisplay, int iIgnored, int *pPtr); // DVRWARN

int _reentrant DisplayHoldButtonActivated(void);

#ifdef MMC
extern WORD wMediaState;
#else
WORD wMediaState = FALSE;
#endif
extern WORD Btl_Flags;
extern WORD LowResAdcBattAvg;
extern WORD g_wRawContrast;


#ifdef USE_PLAYLIST3
extern int g_current_index;
extern int g_current_size;
#endif //#ifdef USE_PLAYLIST3

#ifdef USE_PLAYLIST5
extern PL5_PL_QUEUE    g_PL5_PL_queue;
#endif

#ifdef WOW
extern BOOL    g_bWOWEnable;
#endif
struct HourMinSecStruct {
    int m_iHours;
    int m_iMinutes;
    int m_iSeconds;
};


/* These global variables give the direct access to the Tuner data for the
 * current PLL setting. They are either in tunermodule.c or philips5767.c.
 * These external variables are found in the fmtunermenu.c.
 * These are updated the last time the MENU_TUNER_TUNED message
 * was received. The stereobroadcast bit changes with the signal strength.
 */
extern	WORD	g_wTunedFrequency;
extern	BOOL	g_bStereoBroadcast;
//extern  INT     s_iFMSignalStrengthRSSI;
extern void* DisplayQueueDescriptor;
int GetEmptySpace(void*);

_reentrant void SysPostMessageWithChecking(int, ...);

#pragma asm
    include "sysmacro.asm"
    include "const.xref"
    org p,".ptextdisplay2":
    EXTERN FDisplayQueueDescriptor
    EXTERN FGetEmptySpace
    EXTERN SysWaitOnEvent
    EXTERN SysPostMessage
    EXTERN FSysPostMessage

    ;This only does checking on the display message queue.
FSysPostMessageWithChecking
    pop  a
    push a1
    push SSH
    push a1
    move #FDisplayQueueDescriptor,r0
    jsr  FGetEmptySpace             ;see how much is left
    pop  b                         ;get length
    pop  SSH
    cmp  a,b
    jle  _EnoughRoom
    clr a
    move y:<Const_000001,b
    move a,r0
    jsr  SysWaitOnEvent
    jmp FSysPostMessageWithChecking
_EnoughRoom
    jsr  FSysPostMessage
    rts
#pragma endasm

#ifndef REMOVE_FM
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int DisplayFMTunerFrequency(int bDisplay, int iIgnored2, int *pPtr)
//
//   Type:          Function
//
//   Description:   Updates the display with FM frequency in MHz, Px preset,
//                  and Mono/Stereo indicator
//
//   Inputs:        bDisplay--display or not
//                  iIgnored2-- ignored
//                  pPtr --currently ignored
//
//  Notes:          Some of these functions do not clear the range.  prior to drawing for memory
//                  savings.  See the code to verify that bDisplay is actually used.
//<
////////////////////////////////////////////////////////////////////////////////
_reentrant int DisplayFMTunerFrequency(int bDisplay, int iIgnored2, int *pPtr)
{
    iIgnored2;pPtr;

    ClearRange(FM_TUNER_FREQUENCY_X_POS,FM_TUNER_FREQUENCY_Y_POS,
               FM_TUNER_FREQUENCY_X_SIZE,FM_TUNER_FREQUENCY_Y_SIZE+30);

    if(bDisplay)
    {
#if 1
        /* display preset index number and tuned frequency on the first line */
        if (g_wCurrentPreset)
		{
        	SysPostMessageWithChecking(5,LCD_PRINT_STRING_ADDR,FM_TUNER_PRESET_X_POS,FM_TUNER_PRESET_Y_POS,"P");
        	SysPostMessageWithChecking(7,LCD_PRINT_NUMBER,FM_TUNER_PRESET_X_POS+CHAR_SIZE_X,FM_TUNER_PRESET_Y_POS,g_wCurrentPreset,2,'0');
		}
#else
        SysPostMessageWithChecking(7,LCD_PRINT_NUMBER,FM_TUNER_PRESET_X_POS,FM_TUNER_PRESET_Y_POS,s_iFMSignalStrengthRSSI,3,' ');

#endif
#ifdef SD_USE_50KHZ_TUNING_GRID
        SysPostMessageWithChecking(7,LCD_PRINT_NUMBER,FM_TUNER_FREQUENCY_X_POS,FM_TUNER_FREQUENCY_Y_POS,((g_wCurrentFrequency)/1000),3,' ');
        SysPostMessageWithChecking(7,LCD_PRINT_NUMBER,FM_TUNER_FRAC_FREQUENCY_X_POS,FM_TUNER_FREQUENCY_Y_POS,((g_wCurrentFrequency)/10)%100,2,'0');
        SysPostMessageWithChecking(5,LCD_PRINT_STRING_ADDR,FM_TUNER_FREQUENCY_DOT_X_POS,FM_TUNER_FREQUENCY_Y_POS,".");
        /* show the stereo indicator on the third line */
		//Todo show stereo all the timeSysPostMessageWithChecking(5,LCD_PRINT_STRING_RSRC,FM_TUNER_PILOT_X_POS,FM_TUNER_PILOT_Y_POS,g_bStereoBroadcast ? RSRC_STRING_STEREO : RSRC_STRING_MONO);
		SysPostMessageWithChecking(5,LCD_PRINT_STRING_RSRC,FM_TUNER_PILOT_X_POS,FM_TUNER_PILOT_Y_POS,g_bStereoBroadcast ? RSRC_STRING_STEREO : RSRC_STRING_MONO);
#else
        SysPostMessageWithChecking(7,LCD_PRINT_NUMBER,FM_TUNER_FREQUENCY_X_POS,FM_TUNER_FREQUENCY_Y_POS,((g_wCurrentFrequency+50)/1000),3,' ');
        SysPostMessageWithChecking(7,LCD_PRINT_NUMBER,FM_TUNER_FRAC_FREQUENCY_X_POS,FM_TUNER_FREQUENCY_Y_POS,((g_wCurrentFrequency+50)/100)%10,1,' ');
        SysPostMessageWithChecking(5,LCD_PRINT_STRING_ADDR,FM_TUNER_FREQUENCY_DOT_X_POS,FM_TUNER_FREQUENCY_Y_POS,".");
        /* show the stereo indicator on the third line */
		//Todo show stereo all the timeSysPostMessageWithChecking(5,LCD_PRINT_STRING_RSRC,FM_TUNER_PILOT_X_POS,FM_TUNER_PILOT_Y_POS,g_bStereoBroadcast ? RSRC_STRING_STEREO : RSRC_STRING_MONO);
		SysPostMessageWithChecking(5,LCD_PRINT_STRING_RSRC,FM_TUNER_PILOT_X_POS,FM_TUNER_PILOT_Y_POS,g_bStereoBroadcast ? RSRC_STRING_STEREO : RSRC_STRING_MONO);
#endif
    }

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int DisplayFMTunerPreset(int bDisplay, int iIgnored2, int *pPtr)
//
//   Type:          Function
//
//   Description:   Updates the display
//
//   Inputs:        bDisplay--display or not
//                  iIgnored2-- ignored
//                  pPtr --currently ignored
//
//  Notes:          Some of these functions do not clear the range.  prior to drawing for memory
//                  savings.  See the code to verify that bDisplay is actually used.
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant int DisplayFMTunerPreset(int bDisplay, int iIgnored2, int *pPtr)
{
    iIgnored2;pPtr;
/*
   ClearRange(FM_TUNER_PRESET_X_POS,FM_TUNER_PRESET_Y_POS,
               FM_TUNER_PRESET_X_SIZE,FM_TUNER_PRESET_Y_SIZE);

    if(bDisplay&&g_iCurrentPreset)
    {
        SysPostMessageWithChecking(7,LCD_PRINT_NUMBER,FM_TUNER_PRESET_X_POS,FM_TUNER_PRESET_Y_POS,g_iCurrentPreset,2,'0');
    }
*/
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int DisplayFMTunerStrength(int bDisplay, int iIgnored2, int *pPtr)
//
//   Type:          Function
//
//   Description:   Updates the display
//
//   Inputs:        bDisplay--display or not
//                  iIgnored2-- ignored
//                  pPtr --currently ignored
//
//  Notes:          Some of these functions do not clear the range.  prior to drawing for memory
//                  savings.  See the code to verify that bDisplay is actually used.
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant int DisplayFMTunerStrength(int bDisplay, int iIgnored2, int *pPtr)
{
    iIgnored2;pPtr;
    return 0;
}

#endif
_reentrant void DisplayTime(int x, int y, struct HourMinSecStruct  *pStruct)
{
    SysPostMessageWithChecking(7,LCD_PRINT_TIME_LONG,x,y,
        pStruct->m_iHours,pStruct->m_iMinutes,pStruct->m_iSeconds);
}


#ifdef PLAYER_STRESS
extern INT g_iNumberOfResourceDriveRefreshes;
/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int DisplayNumberOfRefresh( int bDisplay, int iIgnored2, int *pPtr)
//
//   Type:          Function
//
//   Description:   Updates the display
//
//   Inputs:        bDisplay--display or not
//                  iIgnored2-- ignored
//                  pPtr --currently ignored
//
//  Notes:          Some of these functions do not clear the range.  prior to drawing for memory
//                  savings.  See the code to verify that bDisplay is actually used.
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant int DisplayNumberOfRefresh( int bDisplay, int iIgnored2, int *pPtr)
{
    iIgnored2;//to prevent the "parameter not used" warning
    pPtr;



    ClearRange(LEFT_OF_SCREEN, STRESS_TEST_Y_POS,SCREEN_WIDTH, STRESS_TEST_Y_SIZE);
    if(bDisplay)
    {
        SysPostMessageWithChecking(7,LCD_PRINT_NUMBER,STRESS_TEST_X_POS,STRESS_TEST_Y_POS,g_iNumberOfResourceDriveRefreshes,4,' ');
        //could print the total time y:SysTime has uptime since player booted.

    }
    return 0;
}
#endif


/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int DisplayEncoderTrackTime( int bDisplay, int iIgnored2, int *pPtr)
//
//   Type:          Function
//
//   Description:   Updates the display
//
//   Inputs:        bDisplay--display or not
//                  iIgnored2-- ignored
//                  pPtr --currently ignored
//
//  Notes:          Some of these functions do not clear the range.  prior to drawing for memory
//                  savings.  See the code to verify that bDisplay is actually used.
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant int DisplayEncoderTrackTime( int bDisplay, int iIgnored2, int *pPtr)
{
    iIgnored2;//to prevent the "parameter not used" warning
    pPtr;



    ClearRange(LEFT_OF_SCREEN, REC_CURR_TIME_Y_POS,SCREEN_WIDTH, REC_CURR_TIME_Y_SIZE);
    if(bDisplay)
    {
        struct HourMinSecStruct tmpStruct;
        SecondsToHourMinSecStruct(g_iEncCurrentSeconds, &tmpStruct);
        DisplayTime(REC_CURR_TIME_X_POS,REC_CURR_TIME_Y_POS,&tmpStruct);
        //print the total time
        SecondsToHourMinSecStruct(g_iEncTotalSeconds, &tmpStruct);
        DisplayTime(REC_TOTAL_TIME_X_POS,REC_TOTAL_TIME_Y_POS,&tmpStruct);
        //print the remaining time.
        SecondsToHourMinSecStruct(g_iEncTotalSeconds-g_iEncCurrentSeconds, &tmpStruct);
        DisplayTime(REC_TOTAL_TIME_X_POS,REC_TOTAL_TIME_Y_POS+8,&tmpStruct);

    }
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int DisplayEncoderTrackName( int bDisplay, int iIgnored2, int *pPtr)
//
//   Type:          Function
//
//   Description:   Updates the display
//
//   Inputs:        bDisplay--display or not
//                  iIgnored2-- ignored
//                  pPtr --currently ignored
//
//  Notes:          Some of these functions do not clear the range.  prior to drawing for memory
//                  savings.  See the code to verify that bDisplay is actually used.
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant int DisplayEncoderTrackName( int bDisplay, int iIgnored2, int *pPtr)
{
    iIgnored2;//to prevent the "parameter not used" warning
    pPtr;

    if(bDisplay)
        SysPostMessageWithChecking(5,LCD_PRINT_STRING_ADDR,SONG_TITLE_X_POS,SONG_TITLE_Y_POS,g_EncFileNameString);
    return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int DisplayLockIcon( int bDisplay, int iIgnored2, int *pPtr)
//
//   Type:          Function
//
//   Description:   Updates the display
//
//   Inputs:        bDisplay--display or not
//                  iIgnored2-- ignored
//                  pPtr --currently ignored
//
//  Notes:          Some of these functions do not clear the range.  prior to drawing for memory
//                  savings.  See the code to verify that bDisplay is actually used.
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant void DisplayLockIcon( int bDisplay, int iIgnored2, int *pPtr)
{
    iIgnored2;//to prevent the "parameter not used" warning
    pPtr;

    if(DisplayHoldButtonActivated()&&bDisplay)
    {   // display HOLD ICON
        DrawBitmap(LOCK_ICON_X_POS,LOCK_ICON_Y_POS,RSRC_LOCK_ICON);
    }
    else
    {
        if(bDisplay)
            ClearRange(LOCK_ICON_X_POS,LOCK_ICON_Y_POS,LOCK_ICON_X_SIZE,LOCK_ICON_Y_SIZE);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int DisplayDisk( int bDisplay, int iIgnored2, int *pPtr)
//
//   Type:          Function
//
//   Description:   Updates the removable media icon
//
//   Inputs:        bDisplay--display or not
//                  iIgnored2-- ignored
//                  pPtr --currently ignored
//
//  Notes:
//
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant int DisplayDisk( int bDisplay, int iIgnored2, int *pPtr)
{
    iIgnored2;
    pPtr;


    if(bDisplay && (wMediaState & MEDIA_STATE_BITMASK))
    {   // if bit set, media inserted
        DrawBitmap(DISK_X_POS,DISK_Y_POS,RSRC_DISK_ICON);
    }
    else
    {//disk isn't inserted, or we're cleaning the screen up
        ClearRange(DISK_X_POS,DISK_Y_POS,DISK_X_SIZE,DISK_Y_SIZE);
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int DisplayTrackTime( int bDisplay, int iIgnored2, int *pPtr)
//
//   Type:          Function
//
//   Description:   Updates the display
//
//   Inputs:        bDisplay--display or not
//                  ForceTotalTimeUpdate -- FORCEUPDATE : forces total track time to be updated
//                                          AUTOUPDATE:   only update total track time if new track
//                  pPtr --currently ignored
//
//  Notes:          Some of these functions do not clear the range.  prior to drawing for memory
//                  savings.  See the code to verify that bDisplay is actually used.
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant int DisplayTrackTime( int bDisplay, int ForceTotalTimeUpdate, int *pPtr)
{
    int iVBRFlag=0;
    static WORD stc_iLastTotalMin = 0xFFFFFF;
    static WORD stc_iLastTotalSec = 0xFFFFFF;

    pPtr;

    if(bDisplay)
    {

        // Init the iVBRFlag to 0 if not (yet) VBR, to 1 if VBR discovered.
        if((iVBRFlag = g_wDecoderSR) & DECODER_VBRFlag)
        {    iVBRFlag = 1;
        }
        else
        {    iVBRFlag = 0;
        }

        if ((g_wSongTotalMinutes == 0) && (g_wSongTotalSeconds == 0))
        {   // force to update if switch between music/voice and track stopped
            ForceTotalTimeUpdate = FORCEUPDATE;
        }

        if (ForceTotalTimeUpdate == FORCEUPDATE)
        {
            stc_iLastTotalMin=0xFFFFFF;
            stc_iLastTotalSec=0xFFFFFF;
        }

        // if this is a new song, clear the entire line.
        if ((stc_iLastTotalMin != g_wSongTotalMinutes) || (stc_iLastTotalSec != g_wSongTotalSeconds))
        {
            ClearRange(CLR_TRACK_TIME_X_POS, CLR_TRACK_TIME_Y_POS,
                       CLR_TRACK_TIME_X_SIZE, CLR_TRACK_TIME_Y_SIZE);
        }
#ifdef USE_PLAYLIST3
        if(g_current_size)
#else
        if(g_wTotalSongCount)
#endif
        {
            //print the current time -
            // determine whether to display hours (Stmp00002965/Stmp00002970)
            if (g_wSongTotalMinutes < 60)
            {
                ClearRange(TRACK_CURR_TIME_X_POS,TRACK_CURR_TIME_Y_POS,TRACK_CURR_TIME_X_SIZE,TRACK_CURR_TIME_Y_SIZE);

                SysPostMessageWithChecking(6,LCD_PRINT_TIME,TRACK_CURR_TIME_X_POS,TRACK_CURR_TIME_Y_POS,
                                           g_wSongCurrentMinutes,g_wSongCurrentSeconds);

                if ((stc_iLastTotalMin != g_wSongTotalMinutes) || (stc_iLastTotalSec != g_wSongTotalSeconds))
                {   // only print the total time if it's new song
                    SysPostMessageWithChecking(6,LCD_PRINT_TIME,TRACK_TOTAL_TIME_X_POS,TRACK_TOTAL_TIME_Y_POS,
                                               g_wSongTotalMinutes,g_wSongTotalSeconds);

					// save for next time
                    stc_iLastTotalMin = g_wSongTotalMinutes;
                    stc_iLastTotalSec = g_wSongTotalSeconds;

                    //print the '/'
                    SysPostMessageWithChecking(5,LCD_PRINT_UNICODE_CHAR,TRACK_SLASH_X_POS,TRACK_SLASH_Y_POS,'/');
				}

            } else // display hours, minutes, seconds.
            {
                struct HourMinSecStruct CurrentStruct,TotalStruct;

                CurrentStruct.m_iSeconds = (INT) g_wSongCurrentSeconds;
                CurrentStruct.m_iHours = (INT) (g_wSongCurrentMinutes / 60);
                CurrentStruct.m_iMinutes = (INT) (g_wSongCurrentMinutes % 60);

                TotalStruct.m_iSeconds = (INT) g_wSongTotalSeconds;
                TotalStruct.m_iHours = (INT) (g_wSongTotalMinutes / 60);
                TotalStruct.m_iMinutes = (INT) (g_wSongTotalMinutes % 60);

                // Display the current time using 1 digit hours - 0:00:00
                SysPostMessageWithChecking(7, LCD_PRINT_TIME_L_1DIG_HR,
                                           HR_TRACK_CURR_TIME_X_POS, HR_TRACK_CURR_TIME_Y_POS,
                                           CurrentStruct.m_iHours, CurrentStruct.m_iMinutes, CurrentStruct.m_iSeconds);
                //total time
                if ((stc_iLastTotalMin != g_wSongTotalMinutes) || (stc_iLastTotalSec != g_wSongTotalSeconds))
                {   // only print the total time if it's new song
                    SysPostMessageWithChecking(7, LCD_PRINT_TIME_L_1DIG_HR,
                                               HR_TRACK_TOTAL_TIME_X_POS, HR_TRACK_TOTAL_TIME_Y_POS,
                                               TotalStruct.m_iHours, TotalStruct.m_iMinutes, TotalStruct.m_iSeconds);

                    // save for next time
                    stc_iLastTotalMin = g_wSongTotalMinutes;
                    stc_iLastTotalSec = g_wSongTotalSeconds;
                    //print the '/'
                    SysPostMessageWithChecking(5,LCD_PRINT_UNICODE_CHAR,HR_TRACK_SLASH_X_POS,HR_TRACK_SLASH_Y_POS,'/');
                }
            }
            //print VBR if VBR discovered
            if(iVBRFlag)
            {  DrawBitmap(VBR_FLAG_X_POS,VBR_FLAG_Y_POS,RSRC_VBR_BITMAP);
            }
            else
            {  ClearRange(VBR_FLAG_X_POS,VBR_FLAG_Y_POS,VBR_FLAG_X_SIZE,VBR_FLAG_Y_SIZE);
            }

        }
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int DisplayShutdownProgress( int bDisplay, int iProgress, int *pPtr)
//
//   Type:          Function
//
//   Description:   Updates the display
//
//   Inputs:        bDisplay--display or not
//                  iProgress--Steps toward completion
//                  pPtr --currently ignored
//
//  Notes:          Some of these functions do not clear the range.  prior to drawing for memory
//                  savings.  See the code to verify that bDisplay is actually used.
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant int DisplayShutdownProgress( int bDisplay, int iProgress, int *pPtr)
{
    int display_bar=0;
    pPtr;

    //DisplayClearDisplay(0,0,0);
    iProgress >>= 1;    // to slow down the power down speed. Must be cooperative with mainmenu.c

    if(bDisplay)
    {
        switch(iProgress) //sdk2.1, to show shutdown progress bar.
        {
        case 0:
            DisplayClearDisplay(0,0,0);
        case 1:
            display_bar=RSRC_PDOWN_STATUS_1;
            break;
        case 2:
            display_bar=RSRC_PDOWN_STATUS_2;
            break;
        case 3:
            display_bar=RSRC_PDOWN_STATUS_3;
            break;
        case 4:
            display_bar=RSRC_PDOWN_STATUS_4;
            break;
        case 5:
            display_bar=RSRC_PDOWN_STATUS_5;
            break;
        case 6:
            display_bar=RSRC_PDOWN_STATUS_6;
            break;
        default:
            break; //this should not happen.

        }

        DrawBitmap(SHUTDOWN_STRING_X_POS,SHUTDOWN_STRING_Y_POS,
            SHUTDOWN_STRING_RESOURCE);

        DrawBitmap(SHUTDOWN_PROGRESS_X_POS,SHUTDOWN_PROGRESS_Y_POS,
            display_bar);

    }
    return 0;
}

_reentrant int DisplayDefragmentstore( int bDisplay, int iProgress, int *pPtr)
{
     int display_bar=0;
     pPtr;
     DisplayClearDisplay(0,0,0);


        DrawBitmap(DEFRAGMENT_STRING_X_POS,DEFRAGMENT_STRING_Y_POS,
            DEFRAGMENT_STRING_RESOURCE);

	 SysPostMessageWithChecking(5,LCD_PRINT_STRING_RSRC,12,34,RSRC_STRING_WORKING);
    return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int DisplayCurrentTrack( int bDisplay, int iIgnored2, int *pPtr)
//
//   Type:          Function
//
//   Description:   Updates the display
//
//   Inputs:        bDisplay--display or not
//                  iIgnored2-- ignored
//                  pPtr --currently ignored
//
//  Notes:          Some of these functions do not clear the range.  prior to drawing for memory
//                  savings.  See the code to verify that bDisplay is actually used.
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant int DisplayCurrentTrack( int bDisplay, int iIgnored2, int *pPtr)
{

    iIgnored2;//to prevent the "parameter not used" warning
    pPtr;
     ClearRange(TRACK_NUMBER_X_POS,TRACK_NUMBER_Y_POS,
                TRACK_NUMBER_X_SIZE+TRACK_COUNT_X_SIZE,
                TRACK_NUMBER_Y_SIZE);

    if(bDisplay)
    {
        //print "Song:"
#ifndef USE_PLAYLIST3
        if(Playlist_GetPlaySet() == PLAYSET_VOICE)
            SysPostMessageWithChecking(5,LCD_PRINT_STRING_RSRC,0,TRACK_NUMBER_Y_POS,RSRC_STRING_VOICE_COLON);
        else
#endif	// #ifdef USE_PLAYLIST3
            SysPostMessageWithChecking(5,LCD_PRINT_STRING_RSRC,0,TRACK_NUMBER_Y_POS,RSRC_STRING_SONG_COLON);
#ifdef USE_PLAYLIST3
        //print the current song #
        if (g_current_size)
        	SysPostMessageWithChecking(7,LCD_PRINT_NUMBER,TRACK_NUMBER_X_POS,TRACK_NUMBER_Y_POS,g_current_index+1,4,'0');
        else
        	SysPostMessageWithChecking(7,LCD_PRINT_NUMBER,TRACK_NUMBER_X_POS,TRACK_NUMBER_Y_POS,0,4,'0');
      //print the total song count
        SysPostMessageWithChecking(7,LCD_PRINT_NUMBER,TRACK_COUNT_X_POS,TRACK_COUNT_Y_POS,g_current_size,4,'0');
#else	// #ifdef USE_PLAYLIST3
        //print the current song #
#ifdef USE_PLAYLIST5
		if(g_PL5_PL_queue.head == g_PL5_PL_queue.tail)	// g_wTotalSongCount == 0
	        SysPostMessageWithChecking(7,LCD_PRINT_NUMBER,TRACK_NUMBER_X_POS,TRACK_NUMBER_Y_POS,0,4,'0');
		else
		{
			if((g_wCurrentSongNumber <= 4096) && (g_wCurrentSongNumber > 0))
		        SysPostMessageWithChecking(7,LCD_PRINT_NUMBER,TRACK_NUMBER_X_POS,TRACK_NUMBER_Y_POS,g_wCurrentSongNumber,4,'0');
			else
			{
                INT pString[2];
	            pString[0] = 0x2d2d2d;
	            pString[1] = 0x00002d;
				SysPostMessage(5, LCD_PRINT_STRING_ADDR, TRACK_NUMBER_X_POS, TRACK_NUMBER_Y_POS, (INT)pString);
				SysWaitOnEvent(0,0,0);
			}
		}
        //print the total song count
        SysPostMessageWithChecking(7,LCD_PRINT_NUMBER,TRACK_COUNT_X_POS,TRACK_COUNT_Y_POS,g_wTotalSongCount,4,'0');
		if(!g_PL5_Playback.foundall)
		{
			SysPostMessageWithChecking(7,LCD_PRINT_UNICODE_CHAR,TRACK_COUNT_X_POS + 24,TRACK_COUNT_Y_POS,'+',1,'0');
		}
		else
		{
			ClearRange(TRACK_COUNT_X_POS + 24, TRACK_COUNT_Y_POS, 16, TRACK_NUMBER_Y_SIZE);
		}
#else
        SysPostMessageWithChecking(7,LCD_PRINT_NUMBER,TRACK_NUMBER_X_POS,TRACK_NUMBER_Y_POS,g_wCurrentSongNumber,3,'0');
        //print the total song count
        SysPostMessageWithChecking(7,LCD_PRINT_NUMBER,TRACK_COUNT_X_POS,TRACK_COUNT_Y_POS,g_wTotalSongCount,3,'0');
#endif

#endif	// #ifdef USE_PLAYLIST3
        //print the slash
    	SysPostMessageWithChecking(5,LCD_PRINT_UNICODE_CHAR,TRACK_NUMBER_X_POS+24,TRACK_NUMBER_Y_POS,'/');
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int DisplayEQIcon( int bDisplay, int iEqIcon, int *pPtr)
//
//   Type:          Function
//
//   Description:   Updates the display
//
//   Inputs:        bDisplay--display or not
//                  iEqIcon--ignored
//                  pPtr --currently ignored
//                  Uses global g_iEqSetting which must be one of EQ_NORMAL EQ_ROCK
//                                                     EQ_JAZZ EQ_CLASSIC EQ_POP
//
//   Outputs:        0 for supported g_iEqSetting to display.
//                  -1 for unsupported g_iEqSetting. EQ icon display cleared.
//
//
//  Notes:          Some of these functions do not clear the range.  prior to drawing for memory
//                  savings.  See the code to verify that bDisplay is actually used.
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant int DisplayEQIcon( int bDisplay, int iEqIcon, int *pPtr)
{
    int iRetCode = 0;
    pPtr;

    if(bDisplay)
    {
#ifndef WOW

        switch (g_iEqSetting)
        {
            case EQ_NORMAL:
                iEqIcon =  RSRC_EQ_CLEAR_ICON;
                break;
            case EQ_ROCK:
                iEqIcon =  RSRC_ROCK_ICON;
                break;
            case EQ_JAZZ:
                iEqIcon =  RSRC_JAZZ_ICON;
                break;
            case EQ_CLASSIC:
                iEqIcon =  RSRC_CLASSIC_ICON;
                break;
            case EQ_POP:
                iEqIcon =  RSRC_POP_ICON;
                break;
			case EQ_CUSTOM:      // (SDK2.520)
				iEqIcon = RSRC_CUSTOM_ICON;
            	break;
            default:
                // g_iEqSetting had an unsupported value, so
                // Ensures iEqIcon is a valid eq bitmap RSRC for DrawBitMap.
                iEqIcon =  RSRC_EQ_CLEAR_ICON;
                iRetCode = -1;
                break;
        }

        DrawBitmap(EQ_ICON_X_POS, EQ_ICON_Y_POS, iEqIcon);
#else
        if (g_bWOWEnable)
            DrawBitmap(WOW_X_POS,WOW_Y_POS,RSRC_WOW_ICON);
        else
            DrawBitmap(WOW_X_POS,WOW_Y_POS,RSRC_EQ_CLEAR_ICON);
#endif


    }
    return iRetCode;
}


/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int DisplayBar( int bDisplay, int iIgnored1, void *pPtr)
//
//   Type:          Function
//
//   Description:   Updates the generic bar display (10 steps)
//
//   Inputs:        bDisplay--display or not
//                  iIgnored2-- ignored
//                  pPtr --currently ignored
//
//  Notes:          Some of these functions do not clear the range.  prior to drawing for memory
//                  savings.  See the code to verify that bDisplay is actually used.
//<
//////////////////////////////////////////////////////////////////////////////////////////
#ifdef WOW
_reentrant int DisplayBar( int bDisplay, int step, void *pPtr)
{
    int iResource;
    pPtr;
    if(bDisplay)
    {
        DrawBitmap(CONTRAST_X_POS,CONTRAST_Y_POS,RSRC_CONTRAST_FRAME);
        iResource = RSRC_CONTRAST_LEVEL_0 + step;
        if(iResource < RSRC_CONTRAST_LEVEL_0)
            iResource = RSRC_CONTRAST_LEVEL_0;
        else if(iResource > RSRC_CONTRAST_LEVEL_10)
            iResource = RSRC_CONTRAST_LEVEL_10;
        DrawBitmap(CONTRAST_X_POS,CONTRAST_Y_POS,iResource);

    }

    return 0;
}
#endif

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int DisplayPlayMode( int bDisplay, int iIgnored2, int *pPtr)
//
//   Type:          Function
//
//   Description:   Updates the display
//
//   Inputs:        bDisplay--display or not
//                  iIgnored2-- ignored
//                  pPtr --currently ignored
//
//  Notes:          Some of these functions do not clear the range.  prior to drawing for memory
//                  savings.  See the code to verify that bDisplay is actually used.
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant int DisplayPlayMode( int bDisplay, int iIgnored2, int *pPtr)
{ //Displays shuffle/repeat
    int iShuffleResource=0;
    int iRepeatResource=0;
    iIgnored2;//to prevent the "parameter not used" warning
    pPtr;

    ClearRange(REPEAT_ICON_X_POS,REPEAT_ICON_Y_POS,
               REPEAT_ICON_X_SIZE,REPEAT_ICON_Y_SIZE);
    ClearRange(SHUFFLE_ICON_X_POS,SHUFFLE_ICON_Y_POS,
               SHUFFLE_ICON_X_SIZE,SHUFFLE_ICON_Y_SIZE);
    if(bDisplay)
    {
#ifdef USE_PLAYLIST3
        switch(g_iRepeatSetting)
        {
            case REPEAT_ALL:
                iRepeatResource = RSRC_REPEAT_ALL_ICON;
                break;
            case REPEAT_ONE:
                iRepeatResource = RSRC_REPEAT_SONG_ICON;
                break;
        }
        if(g_iShuffleSetting)
            iShuffleResource = RSRC_SHUFFLE_ICON;
#else	// #ifdef USE_PLAYLIST3
        switch(g_iPlaylistRepeat)
        {
            case PLAYLIST_REPEAT_ALL:
                iRepeatResource = RSRC_REPEAT_ALL_ICON;
                break;
            case PLAYLIST_REPEAT_ONE:
                iRepeatResource = RSRC_REPEAT_SONG_ICON;
                break;
        }
        if(g_bPlaylistShuffle)
#ifdef USE_PLAYLIST5
            iShuffleResource = RSRC_RANDOM_ICON;
#else
            iShuffleResource = RSRC_SHUFFLE_ICON;
#endif
#endif	// #ifdef USE_PLAYLIST3
        if(iShuffleResource)
            DrawBitmap(SHUFFLE_ICON_X_POS,SHUFFLE_ICON_Y_POS,iShuffleResource);
        if(iRepeatResource)
            DrawBitmap(REPEAT_ICON_X_POS,REPEAT_ICON_Y_POS,iRepeatResource);
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Internal Helper functions for DisplayVoiceMode().          DVRWARN

//
// Function:    _reentrant BOOL IsEncWarningActive()
// Description: Checks x:EncoderIsrSr bit for low space warning which is set by encoder
// Inputs:      None
// Returns:     TRUE if warning active, otherwise FALSE.
//
_reentrant BOOL IsEncWarningActive(void)
{

    if (g_wEncoderIsrSR & ENCODER_WARN_LOW_SPACE)
    {    return TRUE;
    }
    else
    {    return FALSE;
    }
}

//
// Function: _reentrant DisplayWarning()
// Purpose:  Displays low record memory warning bitmap such as: "Warning" or "Device Full!"
//
_reentrant int DisplayWarning(void)
{
    ClearRange( VOICE_SPACE_WARN_X_POS,  VOICE_SPACE_WARN_Y_POS,
                VOICE_SPACE_WARN_X_SIZE, VOICE_SPACE_WARN_Y_SIZE);

    // Possible future enhancement. FLASHING_DISPLAY_MEM_WARNING undefined for initial version.
  #if FLASHING_DISPLAY_MEM_WARNING
    if(g_iVoiceBitmap&0x01)
    {
        DrawBitmap(VOICE_ANIMATION_X_POS+4, VOICE_ANIMATION_Y_POS, RSRC_WARNING);
    }
    else
    {
        DrawBitmap(VOICE_ANIMATION_X_POS+4, VOICE_ANIMATION_Y_POS+16, RSRC_DEVICE_FULL);
    }
  #else
    // Messages to LCD are:     "Warning"  and  "Device Full!"
    // DrawBitmap(VOICE_SPACE_WARN_X_POS+4, VOICE_SPACE_WARN_Y_POS, RSRC_WARNING); // Note y pos
    DrawBitmap(VOICE_SPACE_WARN_X_POS, VOICE_SPACE_WARN_Y_POS, RSRC_DEVICE_FULL);
  #endif

    return 0;
} 
/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int DisplayVoiceMode(int bDisplay, int iIgnored, int *pPtr)
//
//   Type:          Function
//
//   Description:   Conditionally updates the display with a warning for record media full
//
//   Inputs:        bDisplay--display or not
//                  iIgnored2-- ignored
//                  pPtr --currently ignored
//
//   Outputs:       0 for successful display
//<
//////////////////////////////////////////////////////////////////////////////////////////

_reentrant int DisplayVoiceMode(int bDisplay, int iIgnored, int *pPtr)
{
    pPtr; iIgnored; // prevent unused warning

    // Without voice (mic audio) animation.

    if( bDisplay )
    {
        if( g_bEncoderLowSpace && IsEncWarningActive() )
        {   DisplayWarning();
        }
    }
    else
    {   ClearRange(VOICE_SPACE_WARN_X_POS, VOICE_SPACE_WARN_Y_POS,
                   VOICE_SPACE_WARN_X_SIZE,VOICE_SPACE_WARN_Y_SIZE);
    }

#if 0
    // Possible alternate ver: enhanced future version would display voice animation during record.

    if(bDisplay &&	((g_wEncoderSR&ENCODER_RECORDING) || !(g_wDecoderCSR & DECODER_STOPPED)) )
    {
        if( (g_wEncoderSR&ENCODER_RECORDING) && g_bEncoderLowSpace )
        {   DisplayWarning();
        }
        else
        {
            // DrawBitmap(VOICE_ANIMATION_X_POS,VOICE_ANIMATION_Y_POS,g_iVoiceBitmap); // Future
        }
    }
    else
    {   ClearRange(VOICE_ANIMATION_X_POS, VOICE_ANIMATION_Y_POS,
                   VOICE_ANIMATION_X_SIZE,VOICE_ANIMATION_Y_SIZE);
    }
#endif

    return 0;
}



/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int DisplayVolume( int bDisplay, int iIgnored2, int *pPtr)
//
//   Type:          Function
//
//   Description:   Updates the display
//
//   Inputs:        bDisplay--display or not
//                  iIgnored2-- ignored
//                  pPtr --currently ignored
//
//  Notes:          Some of these functions do not clear the range.  prior to drawing for memory
//                  savings.  See the code to verify that bDisplay is actually used.
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant int DisplayVolume( int bDisplay, int iIgnored2, int *pPtr)
{
    int StepsPerIcon = 1,IconNumber;
    iIgnored2;//to prevent the "parameter not used" warning
    pPtr;


    IconNumber = (g_iUserVolume * VOLUME_NUM_ICONS) / (g_iVolumeSteps) ;
    if(bDisplay)
    {
        //DrawBitmap(VOLUME_ICON_X_POS,VOLUME_ICON_Y_POS,VOLUME_RESOURCE + IconNumber);
        if(g_iVolume_Control_Mode==FALSE)
        	SysPostMessageWithChecking(5,LCD_PRINT_RANGE_RSRC,VOLUME_ICON_X_POS,VOLUME_ICON_Y_POS,VOLUME_RESOURCE + IconNumber);
	else
		SysPostMessageWithChecking(5,LCD_PRINT_RANGE_INV_RSRC,VOLUME_ICON_X_POS,VOLUME_ICON_Y_POS,VOLUME_RESOURCE + IconNumber);
        
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int DisplayBatteryLevel( int bDisplay, int iIgnored2, int *pPtr)
//
//   Type:          Function
//
//   Description:   Updates the battery level indicatordisplay
//
//   Inputs:        bDisplay--display or not
//                  battlevel-- not input, variable used in function
//                  pPtr --currently ignored
//
//  Notes:          11 battery levels are supported: 0 is clear and 10 is full
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant int DisplayBatteryLevel( int bDisplay, int battlevel, int *pPtr)
{
    int iBatteryResource = 0;
    pPtr;

    if(bDisplay)
    {
#ifdef BATTERY_CHARGE
        if(BatteryChargeIsCharging())
        {
            iBatteryResource = RSRC_BATTERY_CHARGING;
        }
        else
#endif
        {
            // no rounding is currently supported
            // don't use resource outside of 10 available bitmaps
            if ((battlevel=SysBatteryGetLevel()) < 10)
                battlevel = 0;
            else if (battlevel > 100)
                battlevel = 10;
            else
                battlevel /= 10;
            iBatteryResource = BATTERY_RESOURCE+battlevel;

        }
        DrawBitmap(BATTERY_ICON_X_POS,BATTERY_ICON_Y_POS,iBatteryResource);
    }

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int DisplayContrast( int bDisplay, int iIgnored1, void *pPtr)
//
//   Type:          Function
//
//   Description:   Updates the contrast display
//
//   Inputs:        bDisplay--display or not
//                  iIgnored2-- ignored
//                  pPtr --currently ignored
//
//  Notes:          Some of these functions do not clear the range.  prior to drawing for memory
//                  savings.  See the code to verify that bDisplay is actually used.
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant int DisplayContrast( int bDisplay, int iIgnored1, void *pPtr)
{
    int iResource;
    pPtr;
    if(bDisplay)
    {
        DrawBitmap(CONTRAST_TITLE_X_POS,CONTRAST_TITLE_Y_POS,RSRC_CONTRAST_TITLE);
        DrawBitmap(CONTRAST_X_POS,CONTRAST_Y_POS,RSRC_CONTRAST_FRAME);
        iResource = RSRC_CONTRAST_LEVEL_0 + DisplayGetContrast()/10;
        if(iResource < RSRC_CONTRAST_LEVEL_0)
            iResource = RSRC_CONTRAST_LEVEL_0;
        else if(iResource > RSRC_CONTRAST_LEVEL_10)
            iResource = RSRC_CONTRAST_LEVEL_10;
        DrawBitmap(CONTRAST_X_POS,CONTRAST_Y_POS,iResource);
    }

/////////////
// calibration debug -- display bar will not be accurate when this is enabled
#ifdef CONTRAST_CALIBRATION
    SysPostMessageWithChecking(7,LCD_PRINT_NUMBER,0,TRACK_COUNT_Y_POS,DisplayGetContrast(),3,'0');
#endif
/////////////

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int DisplayBacklight( int bDisplay, int iIgnored1, void *pPtr)
//
//   Type:          Function
//
//   Description:   Updates the Backlight Menu display
//
//   Inputs:        bDisplay--display or not
//                  iIgnored2-- ignored
//                  pPtr --currently ignored
//
//  Notes:          Some of these functions do not clear the range.  prior to drawing for memory
//                  savings.  See the code to verify that bDisplay is actually used.
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant int DisplayBacklight( int bDisplay, int iIgnored1, void *pPtr)
{
    int iResource;
    pPtr;
    if(bDisplay)
    {
        DrawBitmap(BACKLIGHT_TITLE_X_POS,BACKLIGHT_TITLE_Y_POS,RSRC_BACKLIGHT_TITLE);

        if(g_iBackLightState == BACKLIGHT_ON)
			DrawBitmap(BACKLIGHT_STATE_X_POS,BACKLIGHT_STATE_Y_POS,RSRC_BACKLIGHT_STATE_ON);
		else
			DrawBitmap(BACKLIGHT_STATE_X_POS,BACKLIGHT_STATE_Y_POS,RSRC_BACKLIGHT_STATE_OFF);

    }

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int DisplayPlaySet( int bDisplay, int iIgnored2, int *pPtr)
//
//   Type:          Function
//
//   Description:   Updates the display with the icon indicating
//                  music playset, voice playset, or FM Tuner mode
//
//   Inputs:        bDisplay--display or not
//                  iIgnored2-- ignored
//                  pPtr --currently ignored
//
//  Notes:          Some of these functions do not clear the range.  prior to drawing for memory
//                  savings.  See the code to verify that bDisplay is actually used.
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant int DisplayPlaySet( int bDisplay, int iIgnored2, int *pPtr)
{

    if(bDisplay)
    {
        if( g_iCurrentMenu == MENU_FMTUNER ) // if in FM menu, display FM icon
        {
            // Note: at this time, LCD_PRINT_STRING_ADDR only displays one character.
            SysPostMessageWithChecking(5, LCD_PRINT_STRING_ADDR,
                                       PLAYSET_X_POS-8, PLAYSET_Y_POS, "F");
            SysPostMessageWithChecking(5, LCD_PRINT_STRING_ADDR,
                                       PLAYSET_X_POS-3, PLAYSET_Y_POS, "M");
        } 
      	else if( g_iCurrentMenu == MENU_VOICE)
        {
            SysPostMessageWithChecking(5, LCD_PRINT_STRING_ADDR,
                                       PLAYSET_X_POS-8, PLAYSET_Y_POS, " ");
            SysPostMessageWithChecking(5, LCD_PRINT_STRING_ADDR,
                                       PLAYSET_X_POS-3, PLAYSET_Y_POS, "V");
        }
	else if (g_iAB_Control_Mode ==TRUE)
		{
		SysPostMessageWithChecking(5, LCD_PRINT_STRING_ADDR,
                                       PLAYSET_X_POS-8, PLAYSET_Y_POS, "A");
              SysPostMessageWithChecking(5, LCD_PRINT_STRING_ADDR,
                                       PLAYSET_X_POS-0, PLAYSET_Y_POS, "B");
		}
	else
        {
            SysPostMessageWithChecking(5, LCD_PRINT_STRING_ADDR,
                                       PLAYSET_X_POS-8, PLAYSET_Y_POS, " ");
            SysPostMessageWithChecking(5, LCD_PRINT_STRING_ADDR,
                                       PLAYSET_X_POS-3, PLAYSET_Y_POS, "M");
        }

//Uncoment to display PlaySet by bitmap file
/*#ifndef USE_PLAYLIST3
        if(Playlist_GetPlaySet()==PLAYSET_VOICE)
        {
            DrawBitmap(PLAYSET_X_POS,PLAYSET_Y_POS,RSRC_VOICE_MODE_ICON);
        } else
#endif // #ifndef USE_PLAYLIST3
        {
            DrawBitmap(PLAYSET_X_POS,PLAYSET_Y_POS,RSRC_MUSIC_MODE_ICON);
        }	
*/
    }
    return 0;
}

#ifdef	AUDIBLE
/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int DisplaySecNavIcon( int bDisplay, int iIgnored2, int *pPtr)
//
//   Type:          Function
//
//   Description:   Updates the display
//
//   Inputs:        bDisplay--display or not
//                  iIgnored2-- ignored
//                  pPtr --currently ignored
//
//  Notes:          Some of these functions do not clear the range.  prior to drawing for memory
//                  savings.  See the code to verify that bDisplay is actually used.
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant int DisplaySecNavIcon( int bDisplay, int iIgnored2, int *pPtr)
{
    int uResource;

    uResource =0;
    iIgnored2;//to prevent the "parameter not used" warning
    pPtr;

    if(bDisplay)
    {
        ClearRange(SECNAV_ICON_X_POS,SECNAV_ICON_Y_POS,SECNAV_ICON_X_SIZE,SECNAV_ICON_Y_SIZE);
		if ( g_AudibleFlags.bits.ChapterNavMode )
			DrawBitmap(SECNAV_ICON_X_POS,SECNAV_ICON_Y_POS,RSRC_AUDIBLE_SECNAV_ICON);
    }
    return 0;
}
#endif	// AUDIBLE

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int DisplayABIcon( int bDisplay, int iIgnored2, int *pPtr)
//
//   Type:          Function
//
//   Description:   Updates the display
//
//   Inputs:        bDisplay--display or not
//                  iIgnored2-- ignored
//                  pPtr --currently ignored
//
//  Notes:          Some of these functions do not clear the range.  prior to drawing for memory
//                  savings.  See the code to verify that bDisplay is actually used.
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant int DisplayABIcon( int bDisplay, int iIgnored2, int *pPtr)
{
    int uResource;

    uResource =0;
    iIgnored2;//to prevent the "parameter not used" warning
    pPtr;

#ifdef	AUDIBLE
	if(g_AudibleFlags.bits.IsAudibleFile)
		return DisplaySecNavIcon( bDisplay, iIgnored2, pPtr );
#endif

    if(bDisplay)
    {
        ClearRange(AB_ICON_X_POS,AB_ICON_Y_POS,AB_ICON_X_SIZE,AB_ICON_Y_SIZE);
        switch(GetABMode())
        {
        case DECODER_A_SET:
            uResource = RSRC_AB_MARK_A_ICON;
            break;
        case DECODER_B_SET:
            uResource = RSRC_AB_MARK_B_ICON;
            break;
        }
        if(uResource)
            DrawBitmap(AB_ICON_X_POS,AB_ICON_Y_POS,uResource);

    }
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int DisplayPlayState( int bDisplay, int iIgnored2, int *pPtr)
//
//   Type:          Function
//
//   Description:   Updates the display
//
//   Inputs:        bDisplay--display or not
//                  iIgnored2-- ignored
//                  pPtr --currently ignored
//
//  Notes:          Some of these functions do not clear the range.  prior to drawing for memory
//                  savings.  See the code to verify that bDisplay is actually used.
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant int DisplayPlayState( int bDisplay, int iIgnored2, int *pPtr)
{
    int uResource;
    iIgnored2;//to prevent the "parameter not used" warning
    pPtr;

    uResource = RSRC_STOP_ICON_WITH_BORDER;

    //Clear the range.
    ClearRange(PLAY_STATE_ICON_X_POS,PLAY_STATE_ICON_Y_POS,
               PLAY_STATE_ICON_X_SIZE,PLAY_STATE_ICON_Y_SIZE);

    if(bDisplay)
    {
        if(g_iSeeking==SEEK_FFWD)
        {
            uResource = RSRC_FFWD_ICON_WITH_BORDER;
        }
        else if(g_iSeeking==SEEK_RWND)
        {
            uResource = RSRC_RWND_ICON_WITH_BORDER;
        }
        else
        {
            switch(GetPlayMode())
            {
            case DECODER_PLAYING:
                uResource = RSRC_PLAY_ICON_WITH_BORDER;
                break;
            case DECODER_PAUSED:
                uResource = RSRC_PAUSE_ICON_WITH_BORDER;
                break;
//Stop is defaulted above.  No need to put these...
//            case DECODER_STOPPED:
//                uResource = RSRC_STOP_ICON_WITH_BORDER;
//                break;
            case DECODER_LOOKING_FOR_SYNC:
                uResource = RSRC_PAUSE_ICON_WITH_BORDER;
                break;
            default:
                //?  Print out a question mark maybe?
                break;
            }
        }
        if(EncoderGetStatus()&ENCODER_RECORDING)
        {
            uResource = RSRC_RECORD_ICON_WITH_BORDER;
        }
        if(EncoderGetStatus()&ENCODER_PAUSED)
        {
            uResource = RSRC_PAUSED_RECORD_ICON_BORDER;
        }
        DrawBitmap(PLAY_STATE_ICON_X_POS,PLAY_STATE_ICON_Y_POS,uResource);
    }
    return bDisplay;
}


/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int DisplaySongTitle( int bDisplay, int iIgnored2, int *pPtr)
//
//   Type:          Function
//
//   Description:   Updates the display
//
//   Inputs:        bDisplay--display or not
//                  iIgnored2-- ignored
//                  pPtr --currently ignored
//
//  Notes:          Some of these functions do not clear the range.  prior to drawing for memory
//                  savings.  See the code to verify that bDisplay is actually used.
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant int DisplaySongTitle( int bDisplay, int iIgnored2, int *pPtr)
{
    iIgnored2;//to prevent the "parameter not used" warning
    pPtr;

    if(bDisplay )
    {
#ifdef CLCD
        SysPostMessageWithChecking(7,LCD_CLEAR_RANGE_BUFFER,0,0,128,10,SONG_TITLE_BUFFER);
#ifdef USE_PLAYLIST3
        if(g_current_size)
#else	// #ifdef USE_PLAYLIST3
#ifdef USE_PLAYLIST5
		if(g_PL5_PL_queue.head != g_PL5_PL_queue.tail)	
#else
		if(g_wTotalSongCount)
#endif			
#endif	// #ifdef USE_PLAYLIST3
        {
        	SysPostMessageWithChecking(6,LCD_PRINT_STRING_UNICODE_ADDR_BUFFER, SONG_TITLE_X_POS-g_iSongStringOffset, 1,g_wSongTitle, SONG_TITLE_BUFFER);
        }
#ifdef USE_PLAYLIST5
        else if(!g_PL5_Playback.foundall)
        {
             SysPostMessageWithChecking(6,LCD_PRINT_STRING_RSRC_BUFFER,SONG_TITLE_X_POS,1,RSRC_STRING_SEARCHING,SONG_TITLE_BUFFER);	// "Searching..."
        }
#endif
        else
        {
             SysPostMessageWithChecking(6,LCD_PRINT_STRING_RSRC_BUFFER,SONG_TITLE_X_POS,1,RSRC_STRING_NO_FILES,SONG_TITLE_BUFFER);
        }
#else
        ClearRange(SONG_TITLE_X_POS,SONG_TITLE_Y_POS,SONG_TITLE_X_SIZE,SONG_TITLE_Y_SIZE);
#ifdef USE_PLAYLIST3
        if(g_current_size)
#else	// #ifdef USE_PLAYLIST3
#ifdef USE_PLAYLIST5
		if(g_PL5_PL_queue.head != g_PL5_PL_queue.tail)	
#else
		if(g_wTotalSongCount)
#endif			
#endif	// #ifdef USE_PLAYLIST3
        {
            SysPostMessageWithChecking(5,LCD_PRINT_STRING_UNICODE_ADDR,
                SONG_TITLE_X_POS-g_iSongStringOffset,
                SONG_TITLE_Y_POS,g_wSongTitle);
        }
#ifdef USE_PLAYLIST5
        else if(!g_PL5_Playback.foundall)
        {
            SysPostMessageWithChecking(5,LCD_PRINT_STRING_RSRC,SONG_TITLE_X_POS,SONG_TITLE_Y_POS,RSRC_STRING_SEARCHING);	// "Searching..."
        }
#endif
        else
        {
            SysPostMessageWithChecking(5,LCD_PRINT_STRING_RSRC,SONG_TITLE_X_POS,SONG_TITLE_Y_POS,RSRC_STRING_NO_FILES);
        }
#endif


    }
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int DisplaySongArtistAlbum( int bDisplay, int iIgnored2, int *pPtr)
//
//   Type:          Function
//
//   Description:   Updates the display
//
//   Inputs:        bDisplay--display or not
//                  iIgnored2-- ignored
//                  pPtr --currently ignored
//
//  Notes:          Some of these functions do not clear the range.  prior to drawing for memory
//                  savings.  See the code to verify that bDisplay is actually used.
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant int DisplaySongArtistAlbum( int bDisplay, int iIgnored2, int *pPtr)
{
    iIgnored2;//to prevent the "parameter not used" warning
    pPtr;
    if(bDisplay)
    {
        ClearRange(SONG_ARTIST_X_POS,SONG_ARTIST_Y_POS,SONG_ARTIST_X_SIZE,SONG_ARTIST_Y_SIZE);
        ClearRange(SONG_ALBUM_X_POS,SONG_ALBUM_Y_POS,SONG_ALBUM_X_SIZE,SONG_ALBUM_Y_SIZE);

#ifdef USE_PLAYLIST3
        if(g_current_size)
#else	// #ifdef USE_PLAYLIST3
//        if(g_wTotalSongCount)
#ifdef USE_PLAYLIST5
		if(g_PL5_PL_queue.head != g_PL5_PL_queue.tail)	
#else
		if(g_wTotalSongCount)
#endif			
#endif	// #ifdef USE_PLAYLIST3
        {
            SysPostMessageWithChecking(5,LCD_PRINT_STRING_UNICODE_ADDR,
                SONG_ARTIST_X_POS-g_iArtistStringOffset,
                SONG_ARTIST_Y_POS,g_wSongArtist);
            SysPostMessageWithChecking(5,LCD_PRINT_STRING_UNICODE_ADDR,
                SONG_ALBUM_X_POS-g_iAlbumStringOffset,
                SONG_ALBUM_Y_POS,g_wSongAlbum);
        }
    }

    return 0;
}

#if DEBUG
void _reentrant UpdateMLCErrorStatus(void)
{
    _packed char Number[12];
	BYTE MLC_ERROR_STATUS_MSG[] = "Err=0  ,Drv=0x  ";
    int Length;
    int i;

    Length = Convert_itoa(g_NumErrors, Number);
    for (i = 0; i < Length; i++)
    {
        MLC_ERROR_STATUS_MSG[4+i] = Number[i];
    }

    switch(wResourceTag)
    {
        case 0x02:
            MLC_ERROR_STATUS_MSG[14] = '0';
            MLC_ERROR_STATUS_MSG[15] = '2';
            break;
        case 0x12:
            MLC_ERROR_STATUS_MSG[14] = '1';
            MLC_ERROR_STATUS_MSG[15] = '2';
            break;
        case 0x22:
            MLC_ERROR_STATUS_MSG[14] = '2';
            MLC_ERROR_STATUS_MSG[15] = '2';
            break;
        default:
            MLC_ERROR_STATUS_MSG[13] = ' '; // Remove the 'x' in 0x.  This will be dec, not hex.
            Length = Convert_itoa(wResourceTag, Number);
            for (i = 0; i < Length; i++)
            {
                MLC_ERROR_STATUS_MSG[12+i] = Number[i];
            }
    }
    for (i = 0; i < 16; i++) g_wSongAlbum[i] = MLC_ERROR_STATUS_MSG[i];

}
#endif



/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int DisplayClearDisplay( int iIgnored1, int iIgnored2, int *pPtr)
//
//   Type:          Function
//
//   Description:   Updates the display
//
//   Inputs:        bDisplay--display or not
//                  iIgnored2-- ignored
//                  pPtr --currently ignored
//
//  Notes:          Some of these functions do not clear the range.  prior to drawing for memory
//                  savings.  See the code to verify that bDisplay is actually used.
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant void DisplayClearDisplay( int iIgnored1, int iIgnored2, int *pPtr)
{
    iIgnored1;
    iIgnored2;
    pPtr;
    ClearRange(0,0,LCD_SIZE_X,LCD_SIZE_Y);
}

// debug routine to display battery voltage in place of track time
// helpful in monitoring low battery
_reentrant int DisplayBattVolt (int a, int b, int c)
{

   ClearRange(0,TRACK_CURR_TIME_Y_POS,LCD_SIZE_X,8);
   SysPostMessageWithChecking(7,LCD_PRINT_NUMBER,5,TRACK_CURR_TIME_Y_POS,LowResAdcBattAvg,3,'0');

   return 0;

}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int RefreshDisplay( int uDisplayHint, int bDisplay, int *pPtr)
//
//   Type:          Function (code bank callable)
//
//   Description:   This function takes hints and updates the display based on those hints
//
//   Inputs:        uDisplayHint--24 bit bitmap that indicates which items need updating
//                  bDisplay    --value passed into each call to indicate whether the value should
//                                be updated or just cleared.
//                  pPtr        --currently ignored.
//
//   Outputs:       Messages to the LCD module.
//
//   Notes:         In many cases, the run of this will overload the LCD module with too many
//                  messages.  If this is happening, a SysWaitOnEvent(0,0,1) will give the LCD
//                  module time to run and empty its queue before continuing.
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant int RefreshDisplay( int uDisplayHint, int bDisplay, int *pPtr)
{
    union DisplayHints hints;
    int i;
    pPtr;//to prevent the "parameter not used" warning
    hints.I = uDisplayHint;


    //This message will cause the frame buffer not to update the LCD controller
    //until the LCD_END_FRAME message at the bottom.  This controls 'flicker' on the
    //screen updates that we have to break up into smaller chunks because of the size
    //of the Display module's message queue (preventing overflows)
    SysPostMessageWithChecking(2,LCD_BEGIN_FRAME);
    //In the case of the directtolcd driver, this message is ignored.

    // This clear should be first so we can next use hints to write a fresh display
    if(hints.bits.ClearDisplay)
        DisplayClearDisplay(0,0,0);

    // system icons
    if(hints.bits.Volume)
        DisplayVolume(bDisplay,0,0);

    if(hints.bits.Battery)
        DisplayBatteryLevel(bDisplay,0,0);

    if(hints.bits.Disk)
        DisplayDisk(bDisplay,0,0);

    if(hints.bits.LockIcon)
        DisplayLockIcon(bDisplay,0,0);

#if DISPLAY_MLC_STATUS
    UpdateMLCErrorStatus();
    DisplaySongArtistAlbum(bDisplay,0,0);
#else
    if(hints.bits.SongArtistAlbum)
        DisplaySongArtistAlbum(bDisplay,0,0);
#endif
    // track info displays
    if(hints.bits.SongTitle)
        DisplaySongTitle(bDisplay,0,0);


    if(hints.bits.CurrentTrack)
        DisplayCurrentTrack(bDisplay,0,0);
    // TrackTime and TotalTrackTime are used to reduce flicker during time update
    // by not updating total track time unless it changes
    if(hints.bits.TrackTime)
        DisplayTrackTime(bDisplay,AUTOUPDATE,0);
        // debug: display average batt voltage in cnts instead of tracktime
        // switch comments on display calls to enable debug feature
        //DisplayBattVolt(bDisplay,0,0);

    if (hints.bits.TotalTrackTime)
        DisplayTrackTime(bDisplay,FORCEUPDATE,0);

    if(hints.bits.ABIcon)
        DisplayABIcon(bDisplay,0,0);


    if(hints.bits.PlayState)
        DisplayPlayState(bDisplay,0,0);

    if(hints.bits.PlayMode)
        DisplayPlayMode(bDisplay,0,0);


    if(hints.bits.EQ)
        DisplayEQIcon(bDisplay,0,0);

    // Display icon indicating if FM mode or voice or music files selected
    if(hints.bits.PlaySet)
        DisplayPlaySet(bDisplay,0,0);

    if(hints.bits.RecordMode)  // DVRWARN
        DisplayVoiceMode(bDisplay,0,0);
        // Conditionally displays low mem warning per encoder-detected record memory remaining
        // Future version may also display voice animation during voice record.

    if(hints.bits.EncoderTrackTime)
        DisplayEncoderTrackTime(bDisplay,0,0);

    if(hints.bits.EncoderTrackName)
        DisplayEncoderTrackName(bDisplay,0,0);

#ifndef REMOVE_FM
    if(hints.bits.FMTunerFrequency)
        DisplayFMTunerFrequency(bDisplay,0,0);

    if(hints.bits.FMTunerPreset)
        DisplayFMTunerPreset(bDisplay,0,0);

    if(hints.bits.FMTunerStrength)
        DisplayFMTunerStrength(bDisplay,0,0);
#endif

#ifdef JPEG_ALBUM_ART
	if (hints.bits.AlbumArt)
		SysPostMessageWithChecking(6, LCD_CLEAR_RANGE, ALBUM_ART_X_POS, ALBUM_ART_Y_POS, ALBUM_ART_X_SIZE, ALBUM_ART_Y_SIZE);
#endif

#ifdef PLAYER_STRESS
        if(hints.bits.StressTest)
            DisplayNumberOfRefresh(bDisplay,0,0); // show number of resource.bin refreshes on lcd
#endif

    SysPostMessageWithChecking(2,LCD_END_FRAME);

    return uDisplayHint;
}


/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int GetPlayMode(void)
//
//   Type:          Function
//
//   Description:   Gets the playmode and returns it to the display code
//
//   Inputs:        none
//
//   Outputs:       DECODER_PLAYING,_STOPPED,_LOOKING_FOR_SYNC,_PAUSED.
//
//   Notes:
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant int GetPlayMode(void)
{
   int rtn;

    if(g_wDecoderSR&DECODER_PLAYING)
        rtn = DECODER_PLAYING;
    else if (g_wDecoderSR&DECODER_STOPPED)
        rtn = DECODER_STOPPED;
    else if (g_wDecoderSR&DECODER_LOOKING_FOR_SYNC)
        rtn = DECODER_LOOKING_FOR_SYNC;
    else if (g_wDecoderSR&(DECODER_PAUSED))
        rtn = DECODER_PAUSED;
    else
        rtn = 0;
    return rtn;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int GetABMode(void)
//
//   Type:          Function
//
//   Description:   Gets the AB state and returns it to the display code
//
//   Inputs:        none
//
//   Outputs:       DECODER_B_SET, or DECODER_A_SET
//
//   Notes:
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant int GetABMode(void)
{
    int rtn;
    if (g_wDecoderCSR&(DECODER_B_SET))
        rtn = DECODER_B_SET;
    else if (g_wDecoderCSR&DECODER_A_SET)
        rtn = DECODER_A_SET;
    else
        return 0;
    return rtn;
}


/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int DisplayHoldButtonActivated(void)
//
//   Type:          Function
//
//   Description:   Returns TRUE if the hold button is activated, FALSE if not
//
//   Inputs:        None
//
//   Outputs:       TRUE/FALSE
//
//   Notes:
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant int DisplayHoldButtonActivated(void)
{
#define HOLD_BUTTON_REPORTED_BITMASK    4
    return Btl_Flags&HOLD_BUTTON_REPORTED_BITMASK;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant void ClearRange(int x, int y, int cx, int cy)
//
//   Type:          Function
//
//   Description:   Clears a range on the LCD
//
//   Inputs:        x   -- x position of the top left corner
//                  y   -- y position of the top left corner
//                  cx  -- X size of the rectangle to clear
//                  cy  -- Y size of the rectangle to clear
//
//   Outputs:       none
//
//   Notes:         This is used to wrap the SysPostMessageWithChecking and save a little bit of memory.
//                  (Pushing 6,LCD_CLEAR_RANGE onto the stack takes several words, doing it so
//                  much makes consolidation beneficial)
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant void ClearRange(int x, int y, int cx, int cy)
{
    SysPostMessageWithChecking(6,LCD_CLEAR_RANGE,x,y,cx,cy);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant void DrawBitmap(int x, int y, int iResource)
//
//   Type:          Function
//
//   Description:   Draws a bitmap resource on the LCD at specified x,y coordinates
//
//   Inputs:        x        -- x position of the top left corner
//                  y        -- y position of the top left corner
//                  iResource-- Resource number of the bitmap to display
//
//   Outputs:       none
//
//   Notes:         This is used to wrap the SysPostMessageWithChecking and save a little bit of memory.
//                  (Pushing 5,LCD_PRINT_RANGE_RSRC onto the stack takes several words, doing it so
//                  much makes consolidation beneficial)
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant void DrawBitmap(int x, int y, int iResource)
{
    SysPostMessageWithChecking(5,LCD_PRINT_RANGE_RSRC,x,y,iResource);
}

BOOL SecondsToHourMinSecStruct(int iSeconds, struct HourMinSecStruct *pStruct)
{
    BOOL rtn = FALSE;
    if(iSeconds>=0)
    {
        pStruct->m_iSeconds = iSeconds % 60;
        iSeconds /= 60;
        pStruct->m_iMinutes = iSeconds % 60;
        pStruct->m_iHours  = iSeconds / 60;
        rtn = TRUE;
    }
    return rtn;
}
