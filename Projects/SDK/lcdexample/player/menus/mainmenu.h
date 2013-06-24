#ifndef _MAIN_MENU_H
#define _MAIN_MENU_H

// menus in mainmenu
#define MAINMENU_FIRST  0

enum _MENU_ID
{
	MENU_MUSIC = 0,
#ifdef JPEG_APP
	MENU_JPEG_DISPLAY,
#endif
#ifdef MOTION_VIDEO
	MENU_MVIDEO,
#endif
	MENU_VOICE,

/*This version does not use PL5
#ifdef USE_PLAYLIST5
#ifndef REMOVE_FM
    MENU_FMREC,
#endif
    MENU_LINEIN,
#ifdef AUDIBLE
	MENU_AUDIBLE,
#endif
#endif  // #ifdef USE_PLAYLIST5
*/

#ifdef USE_PLAYLIST3
#ifdef AUDIBLE
	MENU_AUDIBLE,
#endif
#endif
#ifndef REMOVE_FM
	MENU_FMTUNER,
#endif
	MENU_RECORD,
	MENU_SETTINGS,
	MENU_SHUTDOWN,
	MENU_TIME_DATE,
	MENU_AB,
	MENU_DELETE,
	MENU_ABOUT,
#ifdef SPECTRUM_ANAL
	MENU_SPECTROGRAM,
#endif
	MENU_MAIN_EXIT
};


#define MAINMENU_LAST    MENU_MAIN_EXIT
#define MAINMENU_COUNT  (MAINMENU_LAST+1)

#ifdef S6B33B0A_LCD
#define MAINMENU_PAGE1_COUNT	MAINMENU_COUNT
#endif

#ifdef SED15XX_LCD
#define MAINMENU_PAGE1_COUNT	4
#endif


// Media error constants
// Each device gets 5 bits for error codes, in the global g_FSinitErrorCode.
// If there is no external device, the internal device uses bits 0-4.  If there
// is an external device, the internal device is shifted up 5 bits (to bits 5-9),
// and the external device uses bits 0-4.  (More than one external device is not
// supported in the current code.)

#define	ERROR_ON_INTERNAL_MEDIA		0x0F
#define	INTERNAL_MEDIA_NOT_SIGMATEL_FORMAT	0x010

#define	ERROR_ON_EXTERNAL_MEDIA				0x0E
#define	EXTERNAL_MEDIA_NOT_SIGMATEL_FORMAT	0x010

#define	ERROR_ON_INTERNAL_MEDIA_SHIFTED		0x01E0
#define	INTERNAL_MEDIA_NOT_SIGMATEL_FORMAT_SHIFTED	0x020

#ifdef USE_PLAYLIST3
extern INT  g_current_index;
extern INT  g_current_size;
extern _packed BYTE g_strMusicLib_FilePath[];
extern _packed BYTE g_strMusicLib_SecTable_FilePath[];
extern INT  g_iFileHandle;
extern INT  g_ML_save_on_exit;
extern WORD g_rsrc_TimeDate_CodeBank;
#endif  // USE_PLAYLIST3

////////////////////////////////////////////////////////////////////////////////
//  Prototypes
////////////////////////////////////////////////////////////////////////////////
#ifdef USE_PLAYLIST3
void _reentrant ML_building_engine_init(void);
#endif  // USE_PLAYLIST3

#endif
