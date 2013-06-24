////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2004-2006
//
// File        : musicmenu_extra.c
// Description : extra modules for music menu
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
#include "playerstatemachine.h"
#include "deletemenu.h"
#ifdef   USE_PLAYLIST1
#include "playlist1.h"
#else
#ifdef   USE_PLAYLIST2
#include  "playlist2.h"
#else
#ifdef   USE_PLAYLIST3
#include  "playlist3.h"
#endif
#endif
#endif
#include "stringlib.h"
#include "hwequ.h"
BOOL g_AlbumArtInProgress;
#ifdef USE_PLAYLIST3
extern BOOL bPlayVoice;
extern void _reentrant ML_browsing_app_init(void);
#endif //#ifdef USE_PLAYLIST3

#if !(defined JPEG_ALBUM_ART) && !(defined SYNC_LYRICS)
int _reentrant Mucismenu_extra_dummy(INT a, INT b, INT *pc)
{
    a;b;pc;
    return(0);
}
#endif

#ifdef SYNC_LYRICS
#include "synclyrics.h"
#include "displaydrv.h"
#include "font.h"

#define LYRICS_DISPLAY_THRESHOLD	1500

extern void _reentrant LyricsSetTimer(WORD wPeriod);
_asmfunc void strcpyDstYSrcX(UCS3 _Y * _Y pDest, UCS3 _X * _X pSource);

_asmfunc LONG Mp3DecGetSongPos(void);
extern LONG	Mp3DecoderStartFrameFilePosition;
extern WORD g_wBitRateFound;
extern WORD g_wAccumulatedMSecs;
extern WORD g_wAccumulatedSecs;
extern WORD	g_wVBRSignature;

static _reentrant DWORD LyricsGetCurrentTimeInMs(void)	{
	DWORD	dwCurrentBytePointer;
	DWORD	dwMilliSeconds;

	if (g_iSongType == MP3_TYPE) {
		if (g_wVBRSignature) {
			dwMilliSeconds = (DWORD)g_wAccumulatedSecs * 1000;
			dwMilliSeconds += g_wAccumulatedMSecs;
		} else {
			dwCurrentBytePointer = Mp3DecGetSongPos();
			if (dwCurrentBytePointer >= Mp3DecoderStartFrameFilePosition)
				dwCurrentBytePointer -= Mp3DecoderStartFrameFilePosition;
			dwMilliSeconds = (dwCurrentBytePointer * 8) / g_wBitRateFound;
		}
	} else {
		// Calculated by current minutes and seconds
		dwMilliSeconds = (g_wSongCurrentMinutes * 60 + g_wSongCurrentSeconds) * 1000;
	}

	return dwMilliSeconds;
}

_reentrant int DisplaySongLyrics( int bDisplay, int iIgnored2, int *pPtr)
{
	extern _reentrant BITMAP *  GetUnicodeCharacterBitmap(WORD c);
	WORD len, dst, last_src, src, y, last_dst, width;
	WORD ch;
	BITMAP *pBITMAP;
	UCS3 DisplayBuf[MAX_LYRICS_STRING];
	UCS3 RowBuf[LYRICS_ROW_BUF_SIZE];
	DWORD	dwSongElapsedTime, dwFirstLyricsTime;
#ifdef CLCD
	INT		iBitmapWidth;
	UCS3	wStringBuf[2];
#endif
    iIgnored2;//to prevent the "parameter not used" warning
    pPtr;

    if(bDisplay)
    {
		if (LyricsIsValid() == TRUE && g_iPlayerState == DECODER_PLAYING)	{
			// Check whether this lyrics should be updated or not
			dwSongElapsedTime = LyricsGetCurrentTimeInMs();
			dwFirstLyricsTime = LyricsGetFirstLyricsTime();
			if (dwFirstLyricsTime > dwSongElapsedTime) {
				if ((dwFirstLyricsTime - dwSongElapsedTime) >= LYRICS_DISPLAY_THRESHOLD) {
					LyricsSetTimer(dwFirstLyricsTime - dwSongElapsedTime);
					return FALSE;
				}
			}

			// Copy the string for display
			strcpyDstYSrcX(DisplayBuf, g_pSongLyrics);
			// Get next lyrics and set timer according to the time stamp.
			if (g_iSeeking == 0)
				SysCallFunction(RSRC_LYRICS_API_CODEBANK,LyricsGetNext,0,0,0);
			else
				SysCallFunction(RSRC_LYRICS_API_CODEBANK,LyricsSkipping,0,0,0);
			// Display lyrics and wrap text
			SysPostMessage(6,LCD_CLEAR_RANGE,LYRICS_DISPLAY_X_POS,LYRICS_DISPLAY_Y_POS,LCD_SIZE_X,LYRICS_DISPLAY_HEIGHT*3);
			len = strlen(DisplayBuf);
			width = 0;
			dst = 0;
			y = 0;
			last_src = 0;
			last_dst = 0;
#ifdef CLCD
			wStringBuf[1] = 0;
#endif
			for (src = 0; src <= len;)	{
				do	{
					ch = DisplayBuf[src++];
					if (src > len)
						break;
				}while(ch == 0xa || ((dst == 0) && ((ch == 0x20) || ch == 0xd)));// Skip return and 1st space
				if (ch == 0x20 || ch == '(' || ch == '[' || ch == '-' || ch == 0xd || ch > 0xFF)	{
					last_src = src - 1;		// Point to space character
					last_dst = dst;
				}
				if (ch != 0xd)	{
					if (ch != 0xa)	{
#ifdef CLCD
						wStringBuf[0] = ch;
						width += GetTextWidthAddressUnicode(wStringBuf);
#else
						pBITMAP = GetUnicodeCharacterBitmap(ch);
						width += pBITMAP->m_iWidth;
#endif
					}
					RowBuf[dst++] = ch;
				}
				else
					RowBuf[dst++] = 0x0;
				last_dst = dst; 
				if (ch == 0 || width > LCD_SIZE_X || dst >= LYRICS_ROW_BUF_SIZE || ch == 0xd)	{
					if (ch == 0)
						last_dst = dst;
					if((width > LCD_SIZE_X)||(dst >= LYRICS_ROW_BUF_SIZE))
					{
					    last_src = src - 1;		// Point to space character
						last_dst = dst - 1;
					}
					src = last_src;
					RowBuf[last_dst] = 0x0;
		            SysPostMessage(5,LCD_PRINT_STRING_UNICODE_ADDR,LYRICS_DISPLAY_X_POS,LYRICS_DISPLAY_Y_POS+y*LYRICS_DISPLAY_HEIGHT,RowBuf);
		            SysWaitOnEvent(0,0,0);
		            if (ch == 0)
		            	break;
		            dst = 0;
					last_dst = 0;
		            width = 0;
		            y++;
		            if (y >= 3)
		            	break;
		        }
			}
			SysWaitOnEvent(0,0,0);
			return TRUE;
		}
	}
	return FALSE;
}
#endif

#ifdef JPEG_ALBUM_ART
#include "JpegDecodeDisplay.h"
#include "sysspeed.h"
#include "playerlib.h"
#include "..\..\..\..\..\system\common\metadata\attachedpictureframe.h"

extern _X BOOL	g_bApicFrameFound;					// declared in "attachedpictureframe.c"
extern _X BYTE	g_btMIMEType;
int _reentrant DisplayAlbumArtClose(int a, int b, int *pPtr);
void _reentrant StartJpegCallbackTimer(void);		// defined in "jpegdisplaycommon.c"
void _reentrant KillJpegCallbackTimer(void);
void _reentrant Jpeg_LoadDecoder(void);
extern _asmfunc void DecoderForceInit(void);		// defined in "syscoder.asm"
RETCODE _reentrant ApicGetPicturePostion(INT a, INT b, INT *pDataOffset);	// defined in "attachedpictureframe.c"

// defined here for local use
int _reentrant DisplayAlbumArtInit(int a, int b, int *pPtr)
{
	DWORD	dwDataOffset;

#ifdef BACKLIGHT
	// keep backlight always on.
	if (g_iBackLightState == BACKLIGHT_ON) {
        SysPostMessage(2,LCD_BACKLIGHT_ON);
	    SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_BACKLIGHT,1,BACKLIGHT_TIME,MENU_MSG_TURN_OFF_BACKLIGHT);
        SysWaitOnEvent(0,0,0);
	}
#endif
	// Get picture's file position
	SysCallFunction(RSRC_APIC_FRAME_CODEBANK, ApicGetPicturePostion, 0, 0, (INT *)&dwDataOffset);

	SysSpeedIncrease(SPEED_PEAK, SPEED_CLIENT_JPEG_DEC);

	//*** sequence to be followed before each new file is decoded and displayed ***
	SysCallFunction(RSRC_ALBUM_ART_MENU_CODE_BANK, Jpeg_LoadDecoder, 0, 0, NULL);

	if (g_btMIMEType == MIME_TYPE_BMP)
		b = BMP_SetFile(g_CurrentSongName, (WORD)dwDataOffset);
	else
		b = Jpeg_SetFile(g_CurrentSongName, (WORD)dwDataOffset);

	if (b == JPEG_SUCCESS) {
		Jpeg_DisplayGrayScale(FALSE);
		Jpeg_UpdateDisplayParams(ALBUM_ART_X_POS, ALBUM_ART_Y_POS, ALBUM_ART_X_SIZE, ALBUM_ART_Y_SIZE);
		if (Jpeg_InitDecoder() == JPEG_SUCCESS) {	//This will update the dimensions of input image in JpegApp structure
		    Jpeg_ResetDecoder(); //stmp00012780
			//*** start the callback timer ***
			SysCallFunction(RSRC_ALBUM_ART_MENU_CODE_BANK, StartJpegCallbackTimer, 0, 0, NULL);
			g_AlbumArtInProgress =  TRUE;
			return SUCCESS;
		}
	}

     g_AlbumArtInProgress = TRUE;
     DisplayAlbumArtClose(0,0,0);
	return ERROR;
}

int _reentrant DisplayAlbumArtClose(int a, int b, int *pPtr)
{
	if(g_AlbumArtInProgress)
	{
		//***** stop call back function for the decoder ***
		Jpeg_StopDecoder();
		SysCallFunction(RSRC_ALBUM_ART_MENU_CODE_BANK, KillJpegCallbackTimer, 0, 0, NULL);
		SysSpeedClockFree(SPEED_CLIENT_JPEG_DEC);
		g_AlbumArtInProgress =  FALSE;

		// Reload audio decoder
		DecoderForceInit();
		SysPostMessage(2, DECODER_RESET);

#ifdef USE_PLAYLIST3
		SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,ML_browsing_app_init,0,0,0);
#endif
		// Resume playback
		SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK, PlayerLib_SetState, DECODER_STATE_PLAY, TRUE, NULL);
	}
	return 0;
}

//this function just stops the album art without resuming playback
int _reentrant StopAlbumArtProcess(int a, int b, int *pPtr)
{
	//***** stop call back function for the decoder ***
	if(g_AlbumArtInProgress)
	{
		Jpeg_StopDecoder();
		SysCallFunction(RSRC_ALBUM_ART_MENU_CODE_BANK, KillJpegCallbackTimer, 0, 0, NULL);
		SysSpeedClockFree(SPEED_CLIENT_JPEG_DEC);
		g_AlbumArtInProgress =  FALSE;
		// Reload audio decoder
		DecoderForceInit();
		SysPostMessage(2, DECODER_RESET);

#ifdef USE_PLAYLIST3
		SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,ML_browsing_app_init,0,0,0);
#endif

	}
	return 0;
}

#endif

