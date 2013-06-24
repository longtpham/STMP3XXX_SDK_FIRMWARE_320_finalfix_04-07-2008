#ifndef _MENU_H
#define _MENU_H

#include "types.h"

#define TIMER_ANIMATE           0
#define TIMER_AUTO_SHUTDOWN     1
#define TIMER_BATT_CHK          2
#define TIMER_SONG_CHANGE       3
#define TIMER_TUNER				4
#define TIMER_BACKLIGHT			5
#define TIMER_FFRWND    	    6
#define TIMER_BATTERY_CHARGER   7
#define TIMER_TIMEDATE			8
#define TIMER_JPEG_DECODER_CALLBACK 6 //shared with TIMER_FFRWND since they won't use together
#define TIMER_APIC_UPDATE		9

#define MENU_MSG_ANIMATE            MENU_LAST_MSG_ID+1
#define MENU_MSG_AUTOSHUTDOWN       MENU_LAST_MSG_ID+2
#define MENU_SONG_CHANGE_TIMEOUT    MENU_LAST_MSG_ID+3
#define MENU_TUNER_READY			MENU_LAST_MSG_ID+4
#define MENU_MSG_TURN_OFF_BACKLIGHT MENU_LAST_MSG_ID+5
#define MENU_MSG_SEND_FF            MENU_LAST_MSG_ID+6
#define MENU_MSG_SEND_RWND          MENU_LAST_MSG_ID+7
#define MENU_BATTERY_CHARGER_TIMEOUT  MENU_LAST_MSG_ID+8
#define MENU_MSG_REFRESH_TIMEDATE	MENU_LAST_MSG_ID+9
#ifdef USE_PLAYLIST3
#define MENU_PAGE_ITEM_COUNT	(4)
#define MENU_ITEM_HEIGHT		(8)
#define MENU_ITEM_X_OFFSET	(0)
#define MENU_ITEM_Y_OFFSET	(16)

#define PLAYLIST_ENABLE		(0)
#endif
#define ANIMATE_PERIOD		(100)
#define MENU_MSG_JPEG_CALLBACK		  MENU_LAST_MSG_ID+10
#define MENU_JPEG_THUMBNAIL_CURSOR  MENU_LAST_MSG_ID+11
#define MENU_JPEG_NEXT_PICTURE		MENU_LAST_MSG_ID+12
#define MENU_MSG_APIC_UPDATE		MENU_LAST_MSG_ID+13
#ifdef USE_PLAYLIST5
#define MENU_MSG_PL5_PLAY_SONG		MENU_LAST_MSG_ID+14
#endif

#define MENU_BATTERY_CHARGER_TIMEOUT  MENU_LAST_MSG_ID+8
#define FILE_NAME_BUFFER_SIZE  7
#define TIMER_JPEG_DECODER_TIMEOUT_MS 5

// used with shutdown menu
// FORCESHUTDOWN does not allow abort
// USERSHUTDOWN allows user abort if PH_STOP is not held long enough
#define FORCESHUTDOWN			TRUE
#define USERSHUTDOWN 			FALSE

// if low battery display low battery message
#define LOWBATT					TRUE
#define REGBATT					FALSE

//Backlight Define Statements
#ifdef CLCD
#ifdef CLCD_16BIT
#define BACKLIGHT_CONTROL_REGISTER              HW_PWM_CH0AR.I
#define BACKLIGHT_CONTROL_REGISTER_ENABLE       HW_GP0ENR.B.B9	//PWM2
#define BACKLIGHT_CONTROL_REGISTER_DIRECTION    HW_GP0DOER.B.B9
#define BACKLIGHT_ON                            0x06C000
#define BACKLIGHT_OFF                           0x024000
#else
#define BACKLIGHT_CONTROL_REGISTER              HW_PWM_CH2AR.I
#define BACKLIGHT_CONTROL_REGISTER_ENABLE       HW_GP0ENR.B.B9	//PWM2
#define BACKLIGHT_CONTROL_REGISTER_DIRECTION    HW_GP0DOER.B.B9
#define BACKLIGHT_ON                            0x006000
#define BACKLIGHT_OFF                           0x002000
#endif
#else
#define BACKLIGHT_CONTROL_REGISTER              HW_GP0DOR.B.B9
#define BACKLIGHT_CONTROL_REGISTER_ENABLE       HW_GP0ENR.B.B9
#define BACKLIGHT_CONTROL_REGISTER_DIRECTION    HW_GP0DOER.B.B9
#define BACKLIGHT_ON                            TRUE
#define BACKLIGHT_OFF                           FALSE
#endif
#define BACKLIGHT_TIME					        5000		//5 seconds

// used with NextEQ / NextPlayMode to indicate direction
#define INCREMENT 1
#define DECREMENT 0

//These two sit outside the boundary of MENU_FIRST and MENU_LAST simply because they're
//states that aren't included in the displayed list.
#define MENU_EXIT       101
#define MENU_MAIN       100

// menu flags
#define MENU_FLAG_POWER_DOWN_ENABLED 	B0	// when clear, disable tracking
#define MENU_FLAG_ESCAPE_TO_MUSIC		B1	// when set, escape all menu nesting to music
#define MENU_FLAG_SAVE_CHANGES          B2  // true if fast escape interrupted by save changes?
#if (defined USE_PLAYLIST3) || (defined USE_PLAYLIST5)
#define MENU_FLAG_ESCAPE_TO_RECORD		B2	// when set, escape all menu nesting to voice/fm menu and start recording
#define MENU_FLAG_RETURN_TO_MUSIC		B3	// when set, escape all menu nesting to music
#endif

#define LANGUAGES_ENG	0
#define LANGUAGES_VIE	1
#define LANGUAGES_FIRST	LANGUAGES_ENG
#define LANGUAGES_LAST		LANGUAGES_VIE

// 1 word menu variable -- bit flags
extern struct Bitfield g_MenuFlags;

// This data type is declared in mainmenu.c, but is externed here for all other menus.
extern union EventTypes gEventInfo;

//each menu should set g_iCurrentMenu to make sure the menu icon shows up correctly.
extern INT g_iCurrentMenu;

//Backlight State
extern INT g_iBackLightState;

//Language state
extern INT g_iLanguage;

//Export all Menu prototypes for calls allowed from other code banks
void _reentrant UserTask(int a, int b, int *pPtr);
_reentrant INT ShutdownMenu( INT iIgnored1, INT iIgnored2, INT *pPtr);
_reentrant INT SplashScreen( INT iResource, INT iMilliseconds, INT *pPtr);
int _reentrant MusicMenu(INT a, INT b, INT *c);
#ifdef USE_PLAYLIST3
int _reentrant PlayMusicMenu(INT a, INT b, INT *c);
int _reentrant NewMusicMenu(INT a, INT b, INT *c);
int _reentrant BrowseMenu(INT a, INT b, INT *c);
#endif // #ifdef USE_PLAYLIST3
#ifdef MOTION_VIDEO
int _reentrant MotionVideoMenu(INT a, INT b, INT *c);
#endif
#ifdef JPEG_APP
int _reentrant JpegDisplayMenu(int a, int b, int *pPtr);
int _reentrant JpegManualMenu(int a, int b, int *pPtr);
int _reentrant JpegSlideshowMenu(int a, int b, int *pPtr);
int _reentrant JpegThumbnailMenu(int a, int b, int *pPtr);
#endif
int _reentrant SpectrogramMenu(INT a, INT b, INT *pPtr);
int _reentrant VoiceMenu(int a, int b, int *c);
int _reentrant SettingsMenu(int a, int b, int *pPtr);
int _reentrant EqMenu(int a, int b, int *pPtr);
int _reentrant SendEQ(int iCurrentEQ, int b, int *pPtr);

int _reentrant PlayModeMenu(int a, int b, int *pPtr);
int _reentrant NextPlayMode(int iDirection, int b, int *c);
int _reentrant ContrastMenu(int a, int b, int *pPtr);
int _reentrant AboutMenu(int a, int b, int *pPtr);
#ifdef USE_PLAYLIST5
int _reentrant BrowseMenu(INT a, INT b, INT *c);
#endif
int _reentrant DeleteMenu(int a, int b, int *pPtr);
int _reentrant PwrSavingsMenu(int a, int b, int *pPtr);
int _reentrant BackLightMenu(int a, int b, int *pPtr);
int _reentrant RecordSettingsMenu(int a, int b, int *pPtr);
int _reentrant TimeDateMenu(int a, int b, int *pPtr);
int _reentrant SetTimeMenu(int a, int b, int *pPtr);
int _reentrant SetDateMenu(int a, int b, int *pPtr);

int _reentrant TestMenu(int a, int b, int *pPtr);
void _reentrant RecordTestMenu(void);

int _reentrant FMTunerMenu(int a, int b, int *c);
int _reentrant InitPlayerStateMachine(int iEvent, int iIgnored, int *pMessagePtr);
int _reentrant HandlePlayerStateMachine(int iEvent, int iIgnored, int *pMessagePtr);
int _reentrant ExitPlayerStateMachine(int iEvent, int iIgnored, int *pMessagePtr);
_reentrant int InitRecorderStateMachine(int EncodingType, int InputSource, int *c);
_reentrant int HandleRecorderStateMachine(int a, int b, int *c);
_reentrant int ExitRecorderStateMachine(int InputSource, int b, int *c);
int _reentrant NextEQ(INT iDirection, INT b, INT *c);
_reentrant INT RecordVoiceFile(INT a, INT b, INT *pPtr);
_reentrant int RefreshDisplay           ( int iDisplayHint, int a, int *pPtr);
_reentrant void DisplayClearDisplay      ( int iDisplayHint, int a, int *pPtr);
_reentrant int DisplayEQIcon            ( int iDisplayHint, int a, int *pPtr);
_reentrant int DisplayVolume            ( int iDisplayHint, int a, int *pPtr);
_reentrant int DisplayShutdownProgress  ( int iDisplayHint, int a, int *pPtr);
_reentrant int DisplayDefragmentstore  ( int iDisplayHint, int a, int *pPtr);
_reentrant int DisplayKickOff           ( int iDisplayHint, int a, int *pPtr);
_reentrant int DisplayKickOffLocked     ( int iDisplayHint, int a, int *pPtr);
_reentrant void DisplayLockIcon          ( int iDisplayHint, int a, int *pPtr);
_reentrant void ChangePlaySet(INT mode);
void _reentrant UpdateAutoShutdownTimer (void);
int _reentrant SetPwrSetting (int iCurrentPwrSetting, int b, int *pPtr);
int  _reentrant SendPlayMode(int iCurrentPlayMode, int b, int *pPtr);
_reentrant int DeleteFilePrompt(int a,int b, int *pPtr);
#endif

