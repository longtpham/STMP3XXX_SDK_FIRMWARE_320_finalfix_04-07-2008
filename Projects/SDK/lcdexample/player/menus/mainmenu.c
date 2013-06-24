// Filename: 	MainMenu.c (LCD version)
// Reviews: 	DanhNguyen (06-2008)

#include "exec.h"
#include "types.h"
#include "menumanager.h"
#include "messages.h"
#include "metadata.h"
#include "resource.h"
#include "buttons.h"
#include "menus.h"
#include "display.h"
#include "displaylists.h"
#include "mainmenu.h"
#include "pwrsettingsmenu.h"
#include "const.h"
#include "playmodemenu.h"
#include "hwequ.h"
#include "musicmenu.h"
#include "playerstatemachine.h"
#include "sysequ.h"
#include "sysmem.h"
#include "project.h"
#include "lowresolutionadc.h"
#include "battery.h"
#include "sysvolume.h"
#include "Fsproj.h"
#include "persist_bits.h"
#ifdef   USE_PLAYLIST1
#include "playlist1.h"
#else
#ifdef USE_PLAYLIST2
#include "playlist2.h"
#include "sysspeed.h"
#else
#ifdef USE_PLAYLIST3
#include "voicemenu.h"
#include "playlist3.h"
#include "sysspeed.h"
#include "filesystem.h"
#include "fstypes.h"
#include "musiclib_ghdr.h"
#include "stringlib.h"
#include "syscoder.h"
#pragma asm
	nolist
	include "sysresources.inc"
	include "sysequ.inc"
	list
#pragma endasm
#else
#ifdef  USE_PLAYLIST5
#include "playlist5.h"
#include "sysspeed.h"
#include "filesystem.h"
#endif
#endif
#endif
#endif
#ifdef WOW
#include "wowmenu.h"
#include "sysresource.h"
#endif

#define AUTO_FOLDER_SIZE    18

#include "eqmenu.h"

#ifdef SPECTRUM_ANAL
#include "spectrogram.h"
#endif

#include "recordsettingsmenu.h"
#include "playerlib.h"
#include "fsapi.h"
#include "battery_config.h"
#include "micbias.h"

#ifdef WOW
#include "wow_ed.h"
#endif

#if defined(TEST)
#include "testmenu.h"
//#include "playerlib.h"
#endif

#pragma asm
    nolist
    include "msgequ.inc"
    include "sysmacro.asm"
    list
#pragma endasm

#ifdef AUDIBLE
#include "audiblesongposition.h"
#endif

#define SHUTDOWN_COUNT 6

WORD g_wLastButton=0xffffff;
#ifdef USE_PLAYLIST3
int g_current_index;
int g_current_size;
extern long FSFreeSpace(int);
_packed BYTE g_strMusicLib_FilePath[] = "/:ASUM.CIBIL\0\0\0";
_packed BYTE g_strMusicLib_SecTable_FilePath[] = "/:ASUM.CICES\0\0\0";

int g_iFileHandle;
int g_isRebuildSecTableFile;
DIR_DATE dirdate = {7,7,2000};
DIR_TIME dirtime = {0,0,9};
int g_ML_save_on_exit = 0;
unsigned int g_rsrc_TimeDate_CodeBank = RSRC_TIMEDATE_MENU_CODE_BANK;
unsigned int g_rsrc_Flash_Error = RSRC_FLASH_ERROR;

#pragma asm
	if @def('FUNCLET')
	global	RSRC_FUNCLET_LOCAL_GET_LVL_ONE_ITEM_INFO
	global	RSRC_FUNCLET_LOCAL_GET_FLASH_ASSOCIATED_LIST_INFO
	global	RSRC_FUNCLET_ML_RESUMEPLAYSTATE
    global	RSRC_FUNCLET_ML_UPDATE_ON_THE_GO
    global	RSRC_FUNCLET_ML_CH_ON_THE_GO
    global	RSRC_FUNCLET_ML_LOAD_ON_THE_GO
    global	RSRC_FUNCLET_BUILD_INIT_AND_USE_CACHE
    global	RSRC_FUNCLET_BUILD_FLUSH_AND_CLOSE_CACHE
    global	RSRC_FUNCLET_ML_MOVE_POINTER_BACKWARD
    global	RSRC_FUNCLET_ML_FIND_LAST_ITEM_POINTER
    global	RSRC_FUNCLET_ML_START_SHUFFLE
    global	RSRC_FUNCLET_ML_GET_FLASH_DIR_ASSOC_LIST_ID
    global	RSRC_FUNCLET_ML_GET_FLASH_DIR_ASSOC_LIST_ROOT_ID
    global	RSRC_FUNCLET_ML_GET_FLASH_DIR_ASSOC_LIST_SIZE
    global	RSRC_FUNCLET_ML_GET_ID_BY_STATE_AND_POSITION
    global	RSRC_FUNCLET_ML_START_PLAY_DIR_SONG_INIT
    global	RSRC_FUNCLET_ML_SHUFFLE_NEXT_DIR_SONG
    global	RSRC_FUNCLET_ML_BROWSE_MENU
    global	RSRC_FUNCLET_ML_START_PLAY_ALL_SONG
    global	RSRC_FUNCLET_ML_PLAY_ALL_NEXT_SONG
    global	RSRC_FUNCLET_ML_PLAY_PREVIOUS_SONG
    global	RSRC_FUNCLET_ML_PLAY_ALL_PREVIOUS_SONG
    global	RSRC_FUNCLET_ML_PLAY_PREVIOUS_DIR_SONG
    global	RSRC_FUNCLET_ML_SHUFFLE_NEXT_SONG
    global	RSRC_FUNCLET_ML_PLAY_NEXT_SONG
	endif
#pragma endasm

#ifdef MMC
#pragma asm
	if @def('FUNCLET')
	global	RSRC_FUNCLET_ML_RESUMEPLAYSTATE
	endif
#pragma endasm
#endif

extern _packed BYTE	DirPath[];
extern _reentrant int write_detection_access(int option);
#endif

// This g_DataTagName MUST be the same as the one in ststore.c
// This file is used to inform the MTP database that a folder
// is a data folder and not a media folder.  The PHOTO and
// MVIDEO folders need to contain this hidden file.
_packed char g_DataTagName[10] = "ad_.atgat"; // _data.tag

INT g_DriveFlushCnt = 0; // To count the DriveFlush() calls.

extern WORD _Y Btl_Flags;
extern INT _Y g_iSysSaveSettingsDone;
extern _asmfunc void DecoderForceInit(void);
extern _asmfunc void SysLoadFATWrite(void);
extern _asmfunc INT SysUnLoadFATWrite(void);
extern void SaveUserTime();

_reentrant int CreateDataFolder(_packed BYTE *ChkDirectory);
extern g_iMediaErrorFlag;
extern g_FSinitErrorCode;
extern INT  FSInit(INT _X *bufx, INT _Y *bufy, INT maxdevices, INT maxhandles, INT maxcaches);
extern _reentrant INT Computefreeluster(INT DeviceNumber);
void _reentrant CheckRemovableMedia(void);
extern BOOL bMMCInitDone;

// used by SysWaitOnEvent to store the retrieved events
//   iEvent = SysWaitOnEvent(EVENT_MESSAGE|EVENT_BUTTON,&EventInfo,-1);
// If iEvent == EVENT_BUTTON, the first word contains the button event number, the rest are unused
// If iEvent == EVENT_MESSAGE, the entire message is returned
// this Union helps identify which parts of the returned buffer should be used
union EventTypes gEventInfo;

// Initialize to somthing -- used when no settings are saved
// if settings are saved, value will be overwritten during SysGetSettings
#ifdef USE_PLAYLIST5
INT g_iCurrentMenu=MENU_MAIN_EXIT;
#else
INT g_iCurrentMenu=MENU_MUSIC;
#endif
extern _reentrant void  FSClearDriveBuf(INT DriveNumber,INT maxhandles, INT maxcaches);
extern RETCODE _reentrant DriveFlush(WORD wLogDriveNumber);

struct Bitfield g_MenuFlags = {0};

extern void DisablePluginModule(int *);
extern void EnablePluginModule(int *);
extern int g_iContrastPercentage;
extern WORD g_wNumMedia;
extern int CleanDataStoreFlag;
extern int LicenseDeleted;


//FAT_WarningMessage
INT		g_iCheckDiskError;

#ifndef REMOVE_FM
#ifdef CLCD_16BIT   
extern BOOL g_blIsTB2Fm;
#endif
#endif

#ifdef	AUDIBLE
extern _X AudibleFlags_t g_AudibleFlags;
extern void _reentrant AudibleStopAndSave(INT fStop);
#endif

///////////////////////////////////////////////////////////////////////////
#ifdef USE_PLAYLIST3
extern BOOL g_Rebuild;
extern RAM_SONG_INFO_T song_info;
extern int g_file_time;
extern _packed BYTE g_LastEncName[30];

void _reentrant ML_voice_engine_init(void)
{
#pragma asm
	include "resource.inc"
	org     p,".ptextmainmenu":
	extern	F_lc_u_e_MusicLibVoice_P
	extern	F_lc_u_b_MusicLibVoice_P
	extern SysLoadResource

	; Load the MusicLib voice build engine into P memory
	move    #>RSRC_MUSICLIB_VOICE_APP,x0                      ; Resource number
	move    #>RSRC_TYPE_DATA,x1                        ; Resource type is data
	move    #>(F_lc_u_e_MusicLibVoice_P-F_lc_u_b_MusicLibVoice_P)*3,y1 ; overlay area sz
	move    #>F_lc_u_b_MusicLibVoice_P,y0                      ; Address of the overlay area
	move    #TARGET_MEM_P,a1                           ; Overlay area is in P memory
	jsr     SysLoadResource
#pragma endasm
}

void _reentrant ML_building_engine_init(void)
{
#pragma asm
	include "resource.inc"
	org     p,".ptextmainmenu":
	extern F_lc_u_e_MusicLibBuild_P
	extern F_lc_u_b_MusicLibBuild_P
	extern SysLoadResource

	; Load the MusicLib building engine into P memory
	move    #>RSRC_MUSICLIB_BUILD_MODULE,x0                      ; Resource number
	move    #>RSRC_TYPE_DATA,x1                        ; Resource type is data
	move    #>(F_lc_u_e_MusicLibBuild_P-F_lc_u_b_MusicLibBuild_P)*3,y1 ; overlay area sz
	move    #>F_lc_u_b_MusicLibBuild_P,y0                      ; Address of the overlay area
	move    #TARGET_MEM_P,a1                           ; Overlay area is in P memory
	jsr     SysLoadResource
#pragma endasm
}

RETCODE _reentrant TryToRegenerateEmptyMusicLib(long iMusicLib_FileSize, _packed BYTE *g_strFilePath, uint24 FileSize_in_Sector)
{
    #if (LEVEL2_TABLE_SIZE < WORD_PER_SECTOR)
        #error LEVEL2_TABLE_SIZE < WORD_PER_SECTOR
    #endif
	extern LONG _X l2_sector_table[];

	long iFreeSpace;
	WORD wTemp;
	RETCODE rtCode = SUCCESS;
	uint24 _X * sec_temp_buf = (uint24 _X *)l2_sector_table;

	for (wTemp=0; wTemp<WORD_PER_SECTOR; wTemp++) {
		sec_temp_buf[wTemp] = 0;
	}
	if (rtCode == SUCCESS) {
		iFreeSpace = FSFreeSpace(MUSICLIB_STDRIVE_NUMBER);
		if (iFreeSpace > iMusicLib_FileSize) {
			g_iFileHandle = FSFileCreate(g_strFilePath,MUSICLIB_STDRIVE_NUMBER);
			if (g_iFileHandle < 0) {
				rtCode = MUSICLIB_FAT_ERROR_CREATE_FILE;
			} else {
				for (wTemp=0;wTemp<FileSize_in_Sector;wTemp++) {
					FSFileWrite(BYTE_PER_SECTOR,g_iFileHandle,X_MEMORY,-1,(void *)sec_temp_buf);
				}
				filesetattrib(g_iFileHandle,SYSTEM+HIDDEN);
				if (FSFileClose(g_iFileHandle) < 0)
					rtCode = MUSICLIB_FAT_ERROR_CREATE_FILE;
				filesetdate((_packed char *) g_strFilePath,MODIFICATION_DATE,&dirdate,&dirtime);
				filesetdate((_packed char *) g_strFilePath,MODIFICATION_TIME,&dirdate,&dirtime);
			}
		} else {
			rtCode = MUSICLIB_FAT_ERROR_NOT_ENOUGH_MEDIA_SPACE;
		}
	}
	return rtCode;
}
#endif // #ifdef USE_PLAYLIST3

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          void _reentrant UserTask(void)
//
//   Type:          Function (main user menu entry point)
//
//   Description:   This function is the menu manager main entry point.
//
//   Inputs:        None
//
//   Outputs:       None
//
//   Notes:         This function must not be returned from.  The SysWaitOnEvent(0,0,-1) at the
//                  end of this function will essentially terminate the user task and wait for
//                  the kernel to shut the player down.
//<
//////////////////////////////////////////////////////////////////////////////////////////



void _reentrant UserTask(int a, int b, int *pPtr)
{

    int _reentrant (*pMenu)(int,int,int*);
    int MMCDriveNumber;
    BOOL    bRebuildMTPDatabase = FALSE;
    union DisplayHints DisplayHint;
    _packed BYTE ChkDirectory[AUTO_FOLDER_SIZE];

    BOOL bDone = FALSE;
    BOOL bSendToPlayerStateMachine = FALSE;

    INT iSelectedItem=g_iCurrentMenu;

    // Initialize to saved main menu setting, initalized by SysGetSettings
    INT iHighLightedItem=g_iCurrentMenu;

    INT iEvent;
    INT iResource=RSRC_MAIN_MENU_CODE_BANK;

	INT iSaveRecSource;

    INT iLastPage = 0; // used to determine if we should clear the screen

#ifdef USE_PLAYLIST3
	long iFileSize;
	RETCODE rtCode = SUCCESS;
	long iMusicLib_FileSize = MUSICLIB_FILESIZE_IN_BYTE;	/* _must_use_ long iMusicLib_FileSize to pass parameter */
	DIR_DATE dirdate2;
	DIR_TIME dirtime2;
#endif

///////////////////////////////////////////////////////////////////////////
//These data structures are used by the DisplayPagedList function to create the
//the main menu lists.  see displaylists.h for more info
///////////////////////////////////////////////////////////////////////////
    struct PagedListStructure MainMenuListInfo; // m_iNormalDisplayMsg,m_iSelectedDisplayMsg
                                                // m_items, MenuItem *m_pItems

    struct MenuItem aItems[MAINMENU_COUNT]; // m_iPage, m_iResource, m_ixPos, m_iyPos

/****
    THIS METHOD MAKES A CONST SECTION FOR EACH CONST
    struct PagedListStructure MainMenuListInfo ={
        LCD_PRINT_RANGE_RSRC,
        LCD_PRINT_RANGE_INV_RSRC,
        MAINMENU_COUNT,
        NULL};

    struct MenuItem aItems[MAINMENU_COUNT]={
        {0,RSRC_STRING_MUSIC_MENU,0,24},
        {0,RSRC_STRING_VOICE_MENU,0,32},
        {0,RSRC_STRING_FMTUNER_MENU,0,40},
        {0,RSRC_STRING_SETTINGS_MENU,0,48},
        };

*/
        MMCDriveNumber = g_wNumMedia-1;
    // fast fade out the audio.
    // avoid pops when depressing FW and RW before playing any track
    SysPostMessage(3,MIXER_MASTER_FADE_OUT,0);

    SysWaitOnEvent(0,0,1);
    SysWaitOnEvent(0,0,1);
    SysPostMessage(3,LCD_SET_CONTRAST,g_iContrastPercentage);
#ifndef USE_PLAYLIST3
	// Initialize playlist at this point, if not building with external media.
	// Otherwise, let the message handlers do it, to avoid time-consuming redundant
	// initialization.
//#ifndef USE_PLAYLIST5
#ifndef MMC
    SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_Initialize,0,0,0);
#endif
//#endif
#endif // #ifndef USE_PLAYLIST3

//HWJ 1-3-06    HW_GP0DOR.B.B16 = 0;
    // filling the structure like this takes more p mem, but it's overlaid -- not Y sys mem
    MainMenuListInfo.m_iNormalDisplayMsg = LCD_PRINT_STRING_RSRC;
    MainMenuListInfo.m_iSelectedDisplayMsg = LCD_PRINT_STRING_INV_RSRC;
    MainMenuListInfo.m_iItems = MAINMENU_COUNT;

    // filling the structure like this takes more p mem, but it's overlaid -- not Y sys mem
    for (a = 0; a < MAINMENU_COUNT; a++)
	{   // m_ipage is 0 for all menu labels
        aItems[a].m_ixPos = 0;
        aItems[a].m_iPage = a / MAINMENU_PAGE1_COUNT;
        aItems[a].m_iyPos = 16 + (a % MAINMENU_PAGE1_COUNT) * 8;
    }

    // load resource numbers
    aItems[MENU_MUSIC].m_iResource = RSRC_STRING_MUSIC_MENU;
#ifdef JPEG_APP
	aItems[MENU_JPEG_DISPLAY].m_iResource = RSRC_STRING_JPEG_DISPLAY_MENU;
#endif
#ifdef MOTION_VIDEO
    aItems[MENU_MVIDEO].m_iResource = RSRC_STRING_MVIDEO_MENU;
#endif
    aItems[MENU_VOICE].m_iResource = RSRC_BROWSE_VOICE;
#ifdef USE_PLAYLIST5
#ifndef REMOVE_FM
    aItems[MENU_FMREC].m_iResource = RSRC_STRING_FM_REC;
#endif
    aItems[MENU_LINEIN].m_iResource = RSRC_STRING_LINEIN_REC;
#ifdef AUDIBLE
    aItems[MENU_AUDIBLE].m_iResource = RSRC_STRING_AUDIBLE_MENU;
#endif
#endif  // #ifdef USE_PLAYLIST5
#ifdef USE_PLAYLIST3
#ifdef AUDIBLE
    aItems[MENU_AUDIBLE].m_iResource = RSRC_STRING_AUDIBLE_MENU;
#endif
#endif
#ifndef REMOVE_FM
    aItems[MENU_FMTUNER].m_iResource = RSRC_STRING_FMTUNER_MENU;
#endif
    aItems[MENU_SETTINGS].m_iResource = RSRC_STRING_SETTINGS_MENU;
    aItems[MENU_DELETE].m_iResource = RSRC_STRING_DELETE_MENU;
    aItems[MENU_ABOUT].m_iResource = RSRC_STRING_ABOUT_MENU;
    aItems[MENU_RECORD].m_iResource = RSRC_STRING_RECORD_MENU;
    aItems[MENU_TIME_DATE].m_iResource = RSRC_STRING_TIME_DATE_MENU;
#ifdef SPECTRUM_ANAL
    aItems[MENU_SPECTROGRAM].m_iResource = RSRC_STRING_SPECTROGRAM_MENU;
#endif
    aItems[MENU_MAIN_EXIT].m_iResource = RSRC_STRING_EXIT_MENU;

    MainMenuListInfo.m_pItems = aItems;

    // this is required to get the message into the playerstatemachine
    // casted to avoid pointer mismatch warnings
    pPtr = (int *) &gEventInfo;

    // init to clear display when call RefreshDisplay
    DisplayHint.I = MAIN_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);

///////////////////////////////////////////////////////////////////////////
    //This wait is here to allow the button ISR to recognize that the hold button is
    //pressed or not.
    SysWaitOnEvent(0,0,500);

/////////////////  HOLD BUTTON PROCESSING ////////////////////////////////
    // check if hold button was on upon powerup -- and shutdown
    // doing this here because buttons not intialized when display splashscreen
    // could change order of button init

    // (Btl_Flags & HOLD_BUTTON_REPORTED_BITMASK)
    // TRUE if the hold button is activated
    // FALSE if the hold button is not activated

//FAT_WarningMessage
		if (g_iCheckDiskError)
		{
			SysPostMessage(6, LCD_CLEAR_RANGE, 0, 0, 100, 60);
			SysPostMessage(7, LCD_PRINT_NUMBER, 0, 0, 99, 6, '0');
			SysWaitOnEvent(0, 0, 1000);
			ShutdownMenu(FORCESHUTDOWN, 0, 0);
		}


	// Check if there is a media error or filesystem error then display the error
	if ((g_iMediaErrorFlag != 0)||(g_FSinitErrorCode))
	{
	  	SysPostMessage(6,LCD_CLEAR_RANGE,0,0,LCD_SIZE_X,LCD_SIZE_Y);

		#if (NUM_REMOVABLE_MEDIA > 0) // There is at least one external media

            //check whether error is with internal drive or external driive.
            //According to that display the messsage.
		   if (g_FSinitErrorCode &
              (ERROR_ON_INTERNAL_MEDIA_SHIFTED | INTERNAL_MEDIA_NOT_SIGMATEL_FORMAT_SHIFTED))
				SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,8,RSRC_INTERNAL_MEDIA_ERROR1);

		   else	if (g_FSinitErrorCode & ERROR_ON_EXTERNAL_MEDIA)
				SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,RSRC_EXTERNAL_MEDIA_ERROR1);

			else if (g_FSinitErrorCode & EXTERNAL_MEDIA_NOT_SIGMATEL_FORMAT)
				SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,8,RSRC_EXTERNAL_MEDIA_ERROR2);

		#else // NAND only
			if (g_FSinitErrorCode & (ERROR_ON_INTERNAL_MEDIA | INTERNAL_MEDIA_NOT_SIGMATEL_FORMAT))
				SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,8,RSRC_INTERNAL_MEDIA_ERROR1);
		#endif

		SysWaitOnEvent(0,0,4000);  // show for n milliseconds
		ShutdownMenu(FORCESHUTDOWN,0,0);
	}

    if(Btl_Flags & HOLD_BUTTON_REPORTED_BITMASK)
    {  //Display what we need to show the player is locked, then get out
        SysPostMessage(6,LCD_CLEAR_RANGE,0,0,LCD_SIZE_X,LCD_SIZE_Y);
        SysPostMessage(5,LCD_PRINT_RANGE_RSRC,LOCKED_SCREEN_X_POS,LOCKED_SCREEN_Y_POS,RSRC_LOCKED_SCREEN);
        SysWaitOnEvent(0,0,1000);  // show for 1 second
        ShutdownMenu(FORCESHUTDOWN,0,0);
    }
///////////////////////////////////////////////////////////////////////////



///////////////// BATTERY CHECK SETUP /////////////////////////////////////
    // Force the battery status display to update every 2 seconds
    SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_BATT_CHK,0,2000,MENU_BATTERY_CHNG);

#if	0
    // Init the Low Resolution Analog to Digital Converter (LRADC) Channel: BATTery
//    SysLowResolutionADCInit(BATT, OFF, VBATT_REF); // should remove line since done in MiscProject.c, else 6464 defect occurs
///////////////////////////////////////////////////////////////////////////

///////////////// BATTERY BROWNOUT SETUP /////////////////////////////////////
    // Setup Battery brownout detection -- shuts down device if batteries too low
//    SysLRADCBrownoutInit((WORD)(BATT_BROWNOUT)); // should remove line since done in MiscProject.c, else 6464 defect occurs.
#endif
///////////////////////////////////////////////////////////////////////////

//~~~~~~~~~~~~~~~~~~~~~~~~~~~  EFFECTS PROCESSING ~~~~~~~~~~~~~~~~~~~~~~~~~
// Select either GEQ or WOW -- currenly not enough MIPs to do both
// WOW is added to the EQ button toggle to enable/disable WOW if WOW is selected
#ifndef WOW
// To enable GEQ and disable WOW:
    //SysPostMessage(3,GEQ_SET_EQ,ENABLE);

////////////////////////   SETUP EQ FILTER   //////////////////////////////
	// Set Q=0.99 and center frequencies of: 62,250,1k,4k,16k (Hz)
	SysPostMessage(12,GEQ_SET_PARAM,0.99,0.99,0.99,0.99,0.99,62,250,1000,4000,16000);
///////////////////////////////////////////////////////////////////////////
#else

    SysPostMessage(3,GEQ_SET_EQ,DISABLE);
    EnablePluginModule(&WowModuleBase);
	 //At this point wowctrl program can be safely assumed to be loaded in the memory and any code in that file can be called directly.
    SysCallFunction(RSRC_WOW_CTRL,SetProcessEnable,(int)&sWowChannel,(int)TRUE,0);
    g_bWOWEnable = TRUE;
    SysCallFunction(RSRC_WOW_MENU_CODE_BANK,WowInit,0,0,0);
#endif
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    SysWaitOnEvent(0,0,1);  // process all messages before starting (makes room in the soft timer msg queue)

///////////////// AUTOSHUTDOWN SETUP /////////////////////////////////////
    // setup auto shutdown if enabled setting saved
    if (g_wPwrSettings != PWR_DISABLE)
    {
        g_MenuFlags.MENU_FLAG_POWER_DOWN_ENABLED = TRUE;
        // setup g_wPowerOffTrigger
        SysCallFunction(RSRC_PWRSETTINGS_MENU_CODE_BANK,SetPwrSetting,g_wPwrSettings,0,0);
    }
    UpdateAutoShutdownTimer();
///////////////////////////////////////////////////////////////////////////


///////////////  UPDATE SYSTEM SAVED SETTINGS ////////////////////////////
#ifdef USE_PLAYLIST3
	// Let the other messages queues be cleared first
	SysWaitOnEvent(0,0,0);
	// Suspending the auto power down timer and battery check timer to prevent
	//  sudden power down in long sessions of music library rebuild
	// Suspending the animation timer off to save system resource
	SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_BATT_CHK);
	SysWaitOnEvent(0,0,0);
	SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_AUTO_SHUTDOWN);
	SysWaitOnEvent(0,0,0);
	SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_ANIMATE);
	SysWaitOnEvent(0,0,0);

	SysSpeedIncrease(SPEED_WMA, SPEED_CLIENT_PLAYLIST1);
#if 0
    if(g_DriveFlushCnt == 0)
    {
        // To avoid copying updates to a new UBlock which takes much time in some particular conditions.(This problem only happen once after Data Format.) --17/01-07
        DriveFlush(0);

  		// Just let DriveFlush() here be called once. The value of g_DriveFlushCnt will be kept by using SaveRange until next Data Format.	--17/01-07
        g_DriveFlushCnt++;
    }
#else
    DriveFlush(0);
#endif
	SysLoadFATWrite();
	g_current_index = 0;
	g_iFileHandle = -1;
	g_isRebuildSecTableFile = 1;
	g_iFileHandle = Fopen((_packed char *) g_strMusicLib_FilePath, (_packed char *)"r");
	if (g_iFileHandle < 0)
	{
        // for clear screen before rendering message
      	SysPostMessage(6,LCD_CLEAR_RANGE,0,0,LCD_SIZE_X,LCD_SIZE_Y);

		SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,RSRC_BUILD_ML);
		SysWaitOnEvent(0,0,0);
		rtCode = TryToRegenerateEmptyMusicLib(iMusicLib_FileSize, g_strMusicLib_FilePath, MUSICLIB_FILESIZE_IN_SECTOR);	/* _must_use_ long iMusicLib_FileSize to pass parameter */
		FSFileDelete(g_strMusicLib_SecTable_FilePath, 0);
	}
	else
	{
		iFileSize = FSFileSize(g_iFileHandle);
		FSFileClose(g_iFileHandle);
		g_iFileHandle = -1;
		if (iFileSize != iMusicLib_FileSize)
		{
			if (FSFileDelete(g_strMusicLib_FilePath, 0) == -1)
			{
				// Failed to delete
				rtCode = MUSICLIB_FAT_ERROR_DELETE_FILE;
			}
			else
			{
				rtCode = TryToRegenerateEmptyMusicLib(iMusicLib_FileSize, g_strMusicLib_FilePath, MUSICLIB_FILESIZE_IN_SECTOR);	/* _must_use_ long iMusicLib_FileSize to pass parameter */
				FSFileDelete(g_strMusicLib_SecTable_FilePath, 0);
			}
		}
		else
		{
			if ((g_iFileHandle = Fopen((_packed char *) g_strMusicLib_FilePath, (_packed char *)"r")) < 0)
			{
				rtCode = MUSICLIB_FAT_ERROR_OPEN_FILE;
			}
			else
			{
				filegetdate(g_iFileHandle, MODIFICATION_DATE, &dirdate2, &dirtime2);
				filegetdate(g_iFileHandle, MODIFICATION_TIME, &dirdate2, &dirtime2);
				Fclose(g_iFileHandle);
				if ((dirdate.Year != dirdate2.Year) || (dirdate.Month != dirdate2.Month) ||
					(dirdate.Day != dirdate2.Day))
				{
					if (FSFileDelete(g_strMusicLib_FilePath, 0) == -1)
					{
						// Failed to delete
						rtCode = MUSICLIB_FAT_ERROR_DELETE_FILE;
					}
					else
					{
						SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,RSRC_BUILD_ML);
						SysWaitOnEvent(0,0,0);
						rtCode = TryToRegenerateEmptyMusicLib(iMusicLib_FileSize, g_strMusicLib_FilePath, MUSICLIB_FILESIZE_IN_SECTOR);	/* _must_use_ long iMusicLib_FileSize to pass parameter */
						FSFileDelete(g_strMusicLib_SecTable_FilePath, 0);
					}
				}
			}
		}
	}
	if (rtCode == 0)
	{
		//g_iFileHandle = FSFileOpen(g_strMusicLib_SecTable_FilePath,READ,MUSICLIB_STDRIVE_NUMBER);
		g_iFileHandle = Fopen((_packed char *) g_strMusicLib_SecTable_FilePath, (_packed char *)"r");
		if (g_iFileHandle < 0)
		{
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,RSRC_BUILD_ML);
			SysWaitOnEvent(0,0,0);
			rtCode = TryToRegenerateEmptyMusicLib(SEC_TABLE_FILESIZE_IN_BYTE, g_strMusicLib_SecTable_FilePath, NUM_OF_LEVEL2_SEC);
		}
		else
		{
			iFileSize = FSFileSize(g_iFileHandle);
			FSFileClose(g_iFileHandle);
			g_iFileHandle = -1;
			if (iFileSize != SEC_TABLE_FILESIZE_IN_BYTE)
			{
				// Sample code demostrates getNoOfFreeSector
				if (FSFileDelete(g_strMusicLib_SecTable_FilePath, 0) == -1)
				{
					// Failed to delete
					rtCode = MUSICLIB_FAT_ERROR_DELETE_FILE;
				}
				else
				{
					SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,RSRC_BUILD_ML);
					SysWaitOnEvent(0,0,0);
					rtCode = TryToRegenerateEmptyMusicLib(SEC_TABLE_FILESIZE_IN_BYTE, g_strMusicLib_SecTable_FilePath, NUM_OF_LEVEL2_SEC);
				}
			}
			else
			{
				if ((g_iFileHandle = Fopen((_packed char *) g_strMusicLib_SecTable_FilePath, (_packed char *)"r")) < 0)
				{
					rtCode = MUSICLIB_FAT_ERROR_OPEN_FILE;
				}
				else
				{
					filegetdate(g_iFileHandle, MODIFICATION_DATE, &dirdate2, &dirtime2);
					filegetdate(g_iFileHandle, MODIFICATION_TIME, &dirdate2, &dirtime2);
					Fclose(g_iFileHandle);
					if ((dirdate.Year == dirdate2.Year) && (dirdate.Month == dirdate2.Month) &&
						(dirdate.Day == dirdate2.Day))
					{
						// Correct musiclib
						//SysPostMessage(7,LCD_PRINT_NUMBER,0,0,167,4,'0');
						g_isRebuildSecTableFile = 0;
					}
					else
					{
						if (FSFileDelete(g_strMusicLib_SecTable_FilePath, 0) == -1)
						{
							// Failed to delete
							rtCode = MUSICLIB_FAT_ERROR_DELETE_FILE;
						}
						else
						{
							rtCode = TryToRegenerateEmptyMusicLib(SEC_TABLE_FILESIZE_IN_BYTE, g_strMusicLib_SecTable_FilePath, NUM_OF_LEVEL2_SEC);
						}
					}
				}
			}
		}
	}
	if (rtCode)// !SUCCESS // == MUSICLIB_FAT_ERROR_NOT_ENOUGH_MEDIA_SPACE)
	{
		g_ML_save_on_exit = 0;
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC,2,0,RSRC_BUILD_ML_WARNING);
		//SysWaitOnEvent(EVENT_MESSAGE,&gEventInfo.msg,5000);
		SysWaitOnEvent(0,0,0);
		ShutdownMenu(FORCESHUTDOWN,0,0);
	}

	ML_building_engine_init();
	if (g_isRebuildSecTableFile) {
		SysCallFunction(RSRC_MUSICLIB_BUILD_FUNCTION_SEC,ML_BuildSecTableFile,0,0,NULL);
		filesetdate((_packed char *) g_strMusicLib_SecTable_FilePath,MODIFICATION_DATE,&dirdate,&dirtime);
		filesetdate((_packed char *) g_strMusicLib_SecTable_FilePath,MODIFICATION_TIME,&dirdate,&dirtime);
		g_isRebuildSecTableFile = 0;
	} else {
		SysCallFunction(RSRC_MUSICLIB_BUILD_FUNCTION_SEC,ML_Buildl2SecTable,0,0,NULL);
	}

	g_ML_save_on_exit = 1;
	SysUnLoadFATWrite();	// This can be put somewhere better. Siukoon just put here for test.
	SysSpeedClockFree(SPEED_CLIENT_PLAYLIST1);

    // Music library rebuild completed, resume the suspended timers
	SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_ANIMATE,0,ANIMATE_PERIOD,MENU_MSG_ANIMATE);
	SysWaitOnEvent(0,0,0);
	SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_BATT_CHK,0,2000,MENU_BATTERY_CHNG);
	SysWaitOnEvent(0,0,0);
	UpdateAutoShutdownTimer();
#ifndef MMC
	// Rebuild the music library at this point by forcing a device enumeration,
	//  if not building with external media.
	// Otherwise, let the message handlers do it, to avoid time-consuming redundant
	//  initialization.

	// Added by William Chan (17-8-2005)
	g_Rebuild = TRUE;	// To force the rebuild process
	// end

	SysPostMessage(2,PARSER_DEVICE_ENUMERATE);
	SysWaitOnEvent(0,0,0);
#endif // #ifndef MMC
#endif  // #ifdef USE_PLAYLIST3

    // Contrast setting restored during LCDinit
    //auto-create some folders for faster boots
    SysSpeedIncrease(SPEED_WMA, SPEED_CLIENT_PLAYLIST1);
    SysLoadFATWrite();

    ((int*)ChkDirectory)[(AUTO_FOLDER_SIZE/3)-2] = (int)'a' + (((int)':') << 8) + (((int)'/') << 16);
    ((int*)ChkDirectory)[(AUTO_FOLDER_SIZE/3)-1] = 0;
#ifdef JPEG_APP
    ((int*)ChkDirectory)[0] = (int)'a' + (((int)':') << 8) + (((int)'/') << 16);
    ((int*)ChkDirectory)[1] = (int)'P' + (((int)'H') << 8) + (((int)'O') << 16);
    ((int*)ChkDirectory)[2] = (int)'T' + (((int)'O') << 8) + (((int)'\0') << 16);
    ((int*)ChkDirectory)[3] = 0;
    bRebuildMTPDatabase = CreateDataFolder(ChkDirectory);
    FSChangeDir(&ChkDirectory[(AUTO_FOLDER_SIZE/3)-2],0);
#endif
#ifdef MOTION_VIDEO
    ((int*)ChkDirectory)[0] = (int)'a' + (((int)':') << 8) + (((int)'/') << 16);
    ((int*)ChkDirectory)[1] = (int)'M' + (((int)'V') << 8) + (((int)'I') << 16);
    ((int*)ChkDirectory)[2] = (int)'D' + (((int)'E') << 8) + (((int)'O') << 16);
    ((int*)ChkDirectory)[3] = 0;
    bRebuildMTPDatabase = CreateDataFolder(ChkDirectory);
    FSChangeDir(&ChkDirectory[(AUTO_FOLDER_SIZE/3)-2],0);
#endif
    //auto create VOICE directory
	((int*)ChkDirectory)[0] = (int)'a' + (((int)':') << 8) + (((int)'/') << 16);
    ((int*)ChkDirectory)[1] = (int)'V' + (((int)'O') << 8) + (((int)'I') << 16);
    ((int*)ChkDirectory)[2] = (int)'C' + (((int)'E') << 8) + (((int)'\0') << 16);
    ((int*)ChkDirectory)[3] = 0;
    if(FSChangeDir(ChkDirectory,0)<0)
    {
        FSCreateDir(ChkDirectory,0);
        bRebuildMTPDatabase = TRUE;
    }

    //auto create LINEIN directory
	((int*)ChkDirectory)[0] = (int)'a' + (((int)':') << 8) + (((int)'/') << 16);
    ((int*)ChkDirectory)[1] = (int)'L' + (((int)'I') << 8) + (((int)'N') << 16);
    ((int*)ChkDirectory)[2] = (int)'E' + (((int)'-') << 8) + (((int)'I') << 16);
    ((int*)ChkDirectory)[3] = (int)'N' + (((int)'\0') << 8) + (((int)'\0') << 16);
    if(FSChangeDir(ChkDirectory,0)<0)
    {
        FSCreateDir(ChkDirectory,0);
        bRebuildMTPDatabase = TRUE;
    }

    //auto create FM directory
	((int*)ChkDirectory)[0] = (int)'a' + (((int)':') << 8) + (((int)'/') << 16);
    ((int*)ChkDirectory)[1] = (int)'F' + (((int)'M') << 8) + (((int)'\0') << 16);
    ((int*)ChkDirectory)[2] = 0;
    ((int*)ChkDirectory)[3] = 0;
    if(FSChangeDir(ChkDirectory,0)<0)
    {
        FSCreateDir(ChkDirectory,0);
        bRebuildMTPDatabase = TRUE;
    }

    FSChangeDir(&ChkDirectory[(AUTO_FOLDER_SIZE/3)-2],0);

    if (bRebuildMTPDatabase != FALSE)
    {
        // clear MTP database clean flag; a new object(s) has being created, which needs to be accounted for
        ClearStickyBit((volatile _X WORD *)&HW_RTC_MTP_DB_CLEAN_FLAG_REG, HW_RTC_PERSISTANT1_MTP_DB_CLEAN_FLAG_BITPOS);
        bRebuildMTPDatabase = FALSE;
    }

    SysUnLoadFATWrite();	// This can be put somewhere better. Siukoon just put here for test.
    SysSpeedClockFree(SPEED_CLIENT_PLAYLIST1);

#ifndef USE_PLAYLIST3
    // update parser with saved playmode setting
    SysCallFunction(RSRC_PLAYMODE_MENU_CODE_BANK,SendPlayMode,g_iPlayModeSetting,0,0);
	SysCallFunction(RSRC_PLAYERLIB_EXTRA_CODE_BANK,PlayerLib_GetCurrentSong,0,0,0);
#endif //#ifndef USE_PLAYLIST3

    // Check to see if the saved user volume is > than the maximum allowed
#ifdef MAX_RESTORED_VOLUME
    if (g_iUserVolume > MAX_RESTORED_VOLUME)
        g_iUserVolume = MAX_RESTORED_VOLUME;
#endif
    // Update Volume from the restored User Level
    SysUpdateVolume();

    SysCallFunction(RSRC_EQ_MENU_CODE_BANK,SendEQ, g_iEqSetting,0,0);
///////////////////////////////////////////////////////////////////////////
	#if defined(TEST)
    // Automated audio record test invoked from build command "TEST".
	// Wait for MMC to be detected, if present.
        SysWaitOnEvent(0,0,4000);
    #ifdef MMC
		CheckRemovableMedia();
#if 0
        MediaInit(1);   // shouldn't run DriveInit if MediaInit fails.
        DriveInit(1);
#else
        if( MediaInit(MMCDriveNumber) != SUCCESS )  // MMC is media 1
        {   ;// SystemHalt(); _asm(" nop"); //testpoint
        }
        else // MediaInit success, so run DriveInit for MMC
        {   if( DriveInit(MMCDriveNumber) != SUCCESS )  // MMC is drive 1.
            {    ;//SystemHalt(); // comment out this line for release
            }
        }
#endif
		if (bMMCInitDone)
		{
                // Feb 8 2007: FSDriveInit() may call WriteSector()
                SysLoadFATWrite();
 			   	if (FSDriveInit(MMCDriveNumber) == 0)
				{
                  FSClearDriveBuf(MMCDriveNumber,maxhandles,maxcaches);
					#if (NUM_REMOVABLE_MEDIA > 0) // There is at least one external media
                        Computefreeluster(MMCDriveNumber);
					#endif
				}
                SysUnLoadFATWrite();
                /*
 			   	if (FSDriveInit(MMCDriveNumber) == 0)
				{
                  FSClearDriveBuf(MMCDriveNumber,maxhandles,maxcaches);
					#if (NUM_REMOVABLE_MEDIA > 0) // There is at least one external media
                        SysLoadFATWrite();
                        Computefreeluster(MMCDriveNumber);
                        SysUnLoadFATWrite();
					#endif
				}
				*/
		}
	#endif

		//***** Recording test
		SysWaitOnEvent(EVENT_BUTTON,&gEventInfo.msg,0);
		if (gEventInfo.Button.wButtonEvent == PH_REC_TEST)
		{
			if (SysCallFunction(RSRC_TEST_MENU_CODE_BANK,TestMenu,0,0,0) == MENU_RECORDTEST)
				SysCallFunction(RSRC_TEST_MENU_CODE_BANK,RecordTestMenu,0,0,0);
		}
#ifndef USE_PLAYLIST3
		SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_SetPlaySet,Playlist_GetPlaySet(),TRUE,0);
#endif
		SysCallFunction(RSRC_PLAYERLIB_EXTRA_CODE_BANK,PlayerLib_GetCurrentSong,FALSE,0,0);
	#endif
    ///////////////////// End TEST menu block. /////////////////////////////////

    #if (ENABLE_INTERNALLY_GENERATED_MICBIAS && KEEP_MIC_BIAS_ENABLED)
    // Pass hw config values from player's project.inc file
    EnableStmpGeneratedMicBias(LRADC_NUM_FOR_MIC_BIAS, MICBIAS_CONFIGURED_RESISTOR_KOHMS);
    #endif

#ifndef REMOVE_FM
#ifdef CLCD_16BIT   
    if( g_blIsTB2Fm == FALSE )
    {
        SysPostMessage(2, LCD_16BIT_ON );
    }
    else
    {
        SysPostMessage(2, LCD_16BIT_OFF );
    }
#endif
#endif

    SysWaitOnEvent(0,0,1);  // process all messages before starting
#ifdef USE_PLAYLIST3
	//blocking the first time boot up do not go to play music menu
	g_MenuFlags.MENU_FLAG_RETURN_TO_MUSIC = TRUE;
#endif // #ifdef USE_PLAYLIST3

    while(1) // forever....shutdown menu will handle shutdown, main will handle brownout
    {
        while(iSelectedItem!=MENU_MAIN)
        {//This will let menus go directory to other menus without returning to the main menu

            // initialize Menu function so nothing will be called unless case exists
            pMenu = NULL;
#ifdef USE_PLAYLIST5
            g_iTitleResource = aItems[iHighLightedItem].m_iResource;
#endif
            switch(iSelectedItem)
            {
#ifdef USE_PLAYLIST3
			case MENU_MAIN_EXIT:
			case MENU_AB:
				g_MenuFlags.MENU_FLAG_RETURN_TO_MUSIC = TRUE;
			case MENU_MUSIC:
				//goto playmusic menu
				g_iCurrentMenu  = MENU_MUSIC;
				if ((!g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC) && (!g_MenuFlags.MENU_FLAG_RETURN_TO_MUSIC)) {
					SysCallFunction(RSRC_PLAYMUSIC_MENU_CODE_BANK,PlayMusicMenu,0,0,0);
				}

				if ((g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC) || (g_MenuFlags.MENU_FLAG_RETURN_TO_MUSIC))
				{
					iResource = RSRC_MUSIC_MENU_CODE_BANK;
					pMenu = MusicMenu;
				}
				else
					DisplayHint.I = MAIN_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
				break;
			case MENU_VOICE:
				g_iCurrentMenu  = MENU_VOICE;
				SysCallFunction(RSRC_BROWSE_MENU_CODE_BANK,BrowseMenu,ITEM_VOICE,aItems[iHighLightedItem].m_iResource,0);

				if ((g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC) || (g_MenuFlags.MENU_FLAG_RETURN_TO_MUSIC))
				{
					g_iCurrentMenu  = MENU_MUSIC;
					iResource = RSRC_MUSIC_MENU_CODE_BANK;
					pMenu = MusicMenu;
				}
				else if (g_MenuFlags.MENU_FLAG_ESCAPE_TO_RECORD)
				{
					iResource = RSRC_VOICE_MENU_CODE_BANK;
					pMenu = VoiceMenu;
				}
				else
					DisplayHint.I = MAIN_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
				break;
#ifdef AUDIBLE
			case MENU_AUDIBLE:
				g_iCurrentMenu  = MENU_AUDIBLE;
    	      	SysCallFunction(RSRC_BROWSE_MENU_CODE_BANK,BrowseMenu,ITEM_AUDIBLE,aItems[iHighLightedItem].m_iResource,0);

				if ((g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC) || (g_MenuFlags.MENU_FLAG_RETURN_TO_MUSIC))
				{
					g_iCurrentMenu  = MENU_MUSIC;
					iResource = RSRC_MUSIC_MENU_CODE_BANK;
					pMenu = MusicMenu;
				}
				else
					DisplayHint.I = MAIN_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
				break;
#endif //#ifdef AUDIBLE
#endif  // #ifdef USE_PLAYLIST3
#ifdef  USE_PLAYLIST2
            case MENU_MAIN_EXIT:
	     case MENU_AB:
            case MENU_MUSIC:
                g_iCurrentMenu  = MENU_MUSIC;
                iResource = RSRC_MUSIC_MENU_CODE_BANK;
                pMenu = MusicMenu;
                break;
            case MENU_VOICE:
                g_iCurrentMenu  = MENU_VOICE;
                iResource = RSRC_VOICE_MENU_CODE_BANK;
                pMenu = VoiceMenu;
                break;
#endif  // #ifdef USE_PLAYLIST2
#ifndef USE_PLAYLIST5
#ifdef MOTION_VIDEO
            case MENU_MVIDEO:
                g_iCurrentMenu  = MENU_MVIDEO;
                iResource = RSRC_MOTION_VIDEO_MENU_CODE_BANK;
                pMenu = MotionVideoMenu;
                break;
#endif
#ifdef JPEG_APP
            case MENU_JPEG_DISPLAY:
                g_iCurrentMenu  = MENU_JPEG_DISPLAY;
                iResource = RSRC_JPEG_DISPLAY_MENU_CODE_BANK;
                pMenu = JpegDisplayMenu;
                break;
#endif
#endif	// #ifndef USE_PLAYLIST5
#ifdef  USE_PLAYLIST5
			case MENU_MAIN_EXIT:
				g_MenuFlags.MENU_FLAG_RETURN_TO_MUSIC = TRUE;
				if(g_iPlaySet !=PLAYSET_MUSIC)
				{
                        SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_Initialize,0,0,0);
                        SysCallFunction(RSRC_PLAYERLIB_EXTRA_CODE_BANK,PlayerLib_GetCurrentSong,0,0,0);
                }
			case MENU_MUSIC:
			    g_iCurrentMenu = MENU_MUSIC;
				if (g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC || g_MenuFlags.MENU_FLAG_RETURN_TO_MUSIC)
				{
					iResource = RSRC_MUSIC_MENU_CODE_BANK;
					pMenu = MusicMenu;
				}
				else
				{
      			    // set browse music
                    g_ePL5BrowseType = PL5_MUSICTYPE;
                    iResource = RSRC_PL5BROWSE_MENU_CODEBANK;
                    pMenu = BrowseMenu;
				}
			    break;
			case MENU_VOICE:
			    g_iCurrentMenu = MENU_VOICE;
				if (g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC || g_MenuFlags.MENU_FLAG_RETURN_TO_MUSIC)
				{
					g_iCurrentMenu  = MENU_MUSIC;
				}
				else if (g_MenuFlags.MENU_FLAG_ESCAPE_TO_RECORD)
				{
					iResource = RSRC_VOICE_MENU_CODE_BANK;
					pMenu = VoiceMenu;
				}
				else
				{
			        // set browse voice
                    g_ePL5BrowseType = PL5_VOICETYPE;
                    iResource = RSRC_PL5BROWSE_MENU_CODEBANK;
                    pMenu = BrowseMenu;
                }
			    break;
#ifndef REMOVE_FM
			case MENU_FMREC:
			    g_iCurrentMenu = MENU_FMREC;
			    // set browse FM
                g_ePL5BrowseType = PL5_FMTYPE;
                iResource = RSRC_PL5BROWSE_MENU_CODEBANK;
                pMenu = BrowseMenu;
			    break;
#endif
			case MENU_LINEIN:
			    g_iCurrentMenu = MENU_LINEIN;
			    // set browse line-in
                g_ePL5BrowseType = PL5_LINEINTYPE;
                iResource = RSRC_PL5BROWSE_MENU_CODEBANK;
                pMenu = BrowseMenu;
			    break;
#ifdef JPEG_APP
            case MENU_JPEG_DISPLAY:
                g_iCurrentMenu = MENU_JPEG_DISPLAY;
                // set browse photo
                g_ePL5BrowseType = PL5_PHOTOTYPE;
                iResource = RSRC_PL5BROWSE_MENU_CODEBANK;
                pMenu = BrowseMenu;
                iSelectedItem=SysCallFunction(iResource,pMenu,0,0,0);
                if (iSelectedItem != MENU_MAIN)
                {
                g_iCurrentMenu  = MENU_JPEG_DISPLAY;
                iResource = RSRC_JPEG_DISPLAY_MENU_CODE_BANK;
                pMenu = JpegDisplayMenu;
                }
                else
                {
                    DisplayHint.I = MAIN_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
                    pMenu = NULL;
                }
                break;
#endif
#ifdef MOTION_VIDEO
            case MENU_MVIDEO:
                g_iCurrentMenu = MENU_MVIDEO;
                // set browse mvideo
                g_ePL5BrowseType = PL5_VIDEOTYPE;
                iResource = RSRC_PL5BROWSE_MENU_CODEBANK;
                pMenu = BrowseMenu;
                iSelectedItem=SysCallFunction(iResource,pMenu,0,0,0);
                if (iSelectedItem != MENU_MAIN)
                {
                g_iCurrentMenu  = MENU_MVIDEO;
                iResource = RSRC_MOTION_VIDEO_MENU_CODE_BANK;
                pMenu = MotionVideoMenu;
                }
                else
                {
                    DisplayHint.I = MAIN_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
                    pMenu = NULL;
                }
                break;
#endif
#ifdef AUDIBLE
			case MENU_AUDIBLE:
				g_iCurrentMenu  = MENU_AUDIBLE;
                // set browse audible
                g_ePL5BrowseType = PL5_AUDIBLETYPE;
                iResource = RSRC_PL5BROWSE_MENU_CODEBANK;
                pMenu = BrowseMenu;
				break;
#endif
#endif  // #ifdef  USE_PLAYLIST5
#ifndef REMOVE_FM
            case MENU_FMTUNER:
                g_iCurrentMenu  = MENU_FMTUNER;
#ifdef FMTUNER
                iResource = RSRC_FMTUNER_MENU_CODE_BANK;
                pMenu = FMTunerMenu;
#endif
                break;
#endif
            case MENU_SETTINGS:
                g_iCurrentMenu  = MENU_SETTINGS;
                iResource = RSRC_SETTINGS_MENU_CODE_BANK;
                pMenu = SettingsMenu;
                break;
#ifdef USE_PLAYLIST5
            case MENU_DELETE:
                g_iCurrentMenu  = MENU_DELETE;
                // set browse delete
                g_ePL5BrowseType = PL5_DELETETYPE;
                iResource = RSRC_PL5BROWSE_MENU_CODEBANK;
                pMenu = BrowseMenu;
                break;
#else
            case MENU_DELETE:
                g_iCurrentMenu  = MENU_DELETE;
                iResource = RSRC_DELETE_MENU_CODE_BANK;
                pMenu = DeleteMenu;
                break;
#endif
            case MENU_ABOUT:
                g_iCurrentMenu = MENU_ABOUT;
                iResource = RSRC_ABOUT_MENU_CODE_BANK;
                pMenu = AboutMenu;
                break;
            case MENU_RECORD: //sdk2.1
				g_iCurrentMenu = MENU_RECORD;
//stmp6930 move test for FM tuner source to FMTunerMenu
				//Just record here.  No need to enter Voice Menu via
#ifdef CLCD_16BIT
                if(g_ADCsource == SOURCE_FM)
                {
                    SysPostMessage(2,LCD_16BIT_OFF);
                    SysWaitOnEvent(0,0,1);
                }
#endif //#ifdef CLCD_16BIT
            	SysCallFunction(RSRC_VOICE_MENU_CODE_BANK,RecordVoiceFile,0,0,0);
#ifndef REMOVE_FM
#ifdef CLCD_16BIT
                if(g_ADCsource == SOURCE_FM)
                {
                    if( g_blIsTB2Fm == FALSE )
                    {
                        SysPostMessage(2,LCD_16BIT_ON);
                    }
                    else
                    {
                    }
                    SysWaitOnEvent(0,0,1);
                }
#endif //#ifdef CLCD_16BIT
#endif
				DisplayHint.I = (MAIN_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
#ifdef USE_PLAYLIST3
				if (g_ADCsource == SOURCE_FM)
					SysCallFunction(RSRC_VOICE_MENU_CODE_BANK,AddVoiceEntry,REC_FMREC_TYPE,0,0);
				else if (g_ADCsource == SOURCE_LINEIN)
				{
				    if ((MODE_ALBUM == g_RecorderSettings[SOURCE_LINEIN].m_iMode)
				        && (ENCODER_MP3 == g_RecorderSettings[SOURCE_LINEIN].m_EncoderNo))
				    {
                        SysCallFunction(RSRC_MAIN_MENU_CODE_BANK, ML_building_engine_init,0,0,0);
                        //SysLoadFATWrite();
                        g_Rebuild = TRUE;
                        SysCallFunction(RSRC_PLAYLIST2INIT_CODEBANK, BuildLINEINFilePath,0,0,0);
                        g_Rebuild = FALSE;
                        // SysUnLoadFATWrite();
                        SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK, ML_browsing_app_init,0,0,0);
                    }
				    else
					SysCallFunction(RSRC_VOICE_MENU_CODE_BANK,AddVoiceEntry,REC_PHOTO_TYPE,0,0);
				}
				else
					SysCallFunction(RSRC_VOICE_MENU_CODE_BANK,AddVoiceEntry,REC_VOICE_TYPE,0,0);
#endif	// #ifdef USE_PLAYLIST3
                break;

            case MENU_TIME_DATE: //sdk2.1
				g_iCurrentMenu = MENU_TIME_DATE;
            	iResource = RSRC_TIMEDATE_MENU_CODE_BANK;
                pMenu = TimeDateMenu;
                break;

#ifdef SPECTRUM_ANAL
       	case MENU_SPECTROGRAM:
                 g_iCurrentMenu =  MENU_SPECTROGRAM;
                 iResource      =  RSRC_SPECTROGRAM_MENU_CODE_BANK;
                 pMenu          =  SpectrogramMenu;
                break;
#endif
		case MENU_SHUTDOWN:
			ShutdownMenu(FORCESHUTDOWN,0,0);
			break;
			
            	default:    // this should not happen
                _asm( " error"); //consider change to " error" macro which is debug or reset per buildtype
                break;
            }

            // set the highlighted menu as the selected menu, for return
            iHighLightedItem = g_iCurrentMenu;

            if(pMenu)
            {
                // called menu controls next menu with return value
                // if return MENU_MAIN, while loop exits and allows user to continue
                //     traversing the main menu -- typical operation
                // other return values are limited to those used in the
                //     above switch(iSelectedItem) and will allow menu changes
                //     without selection from mainmenu display
                if (iSelectedItem == MENU_RECORD)
                        iSelectedItem=SysCallFunction(iResource,pMenu,0,MENU_RECORD,0);
                else
                        iSelectedItem=SysCallFunction(iResource,pMenu,0,0,0);

                // did we do a fast escape to music
                if (g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC)
                {
                   iSelectedItem = MENU_MUSIC;
#ifdef USE_PLAYLIST5				   
					if(g_iPlaySet == PLAYSET_PHOTO)
					{
						SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_JPEG_DECODER_CALLBACK);
						SysWaitOnEvent(0,0,0);
						iSelectedItem = MENU_MAIN_EXIT;
					}
					else if(g_iPlaySet != PLAYSET_MUSIC)//else if(g_iPlaySet == PLAYSET_MVIDEO)
					{
						iSelectedItem = MENU_MAIN_EXIT;
					}
#endif					
                }
#ifdef USE_PLAYLIST5
				else if((g_iPlaySet == PLAYSET_PHOTO) || (g_iPlaySet == PLAYSET_MVIDEO))
				{
					SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_Initialize,0,0,0);
				}
#endif

                //we need to clear the screen since entering the menu
                DisplayHint.I = MAIN_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
            }
            else
            {   // pMenu == NULL if another menu is not selected
                // just keep going
                iSelectedItem = MENU_MAIN;
            }

        } // end while (iSelectedItem != MENU_MAIN)

        //if we've gotten here, we're back in the main menu, so lets handle events
        //that pertain to the main menu.
        g_iCurrentMenu = MENU_MAIN;
        pMenu = NULL;

        DisplayHint.I &= (MAIN_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));

        //wait until a message or button happens.
        iEvent = SysWaitOnEvent(EVENT_MESSAGE|EVENT_BUTTON,&gEventInfo.msg,DisplayHint.I ? 1 : -1);

        bSendToPlayerStateMachine = TRUE;

        //display EVENT_MESSAGE or EVENT_BUTTON
        switch(iEvent)
        {
        case EVENT_TIMER:
	/*Uncomment to see list menu
            if (iLastPage != aItems[iHighLightedItem].m_iPage)
            {
                // remember this page
                iLastPage = aItems[iHighLightedItem].m_iPage;
                // clear the display
                DisplayHint.I |= (MAIN_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
            }
            SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHint.I,1,0);
            SysCallFunction(RSRC_DISPLAY_LISTS_CODE_BANK,DisplayPagedList,iHighLightedItem,0,(int*)&MainMenuListInfo);
            */          
	
	//View list menu with bitmap
		DisplayHint.I |= (MAIN_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
		SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHint.I,1,0);
           switch(iHighLightedItem)
           {
	    	case MENU_MUSIC:
			g_iAB_Control_Mode = FALSE;
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,16,RSRC_MENU_MUSIC+g_iLanguage);
			break;
		case MENU_VOICE:
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,16,RSRC_MENU_VOICE+g_iLanguage);
			break;
	    	case MENU_FMTUNER:
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,16,RSRC_MENU_FMTUNER+g_iLanguage);
			break;
		case MENU_RECORD:
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,16,RSRC_MENU_RECORD+g_iLanguage);
			break;
		case MENU_SETTINGS:
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,16,RSRC_MENU_SETTINGS+g_iLanguage);
			break;
		case MENU_SHUTDOWN:
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,16,RSRC_MENU_SHUTDOWN+g_iLanguage);
			break;
	    	case MENU_TIME_DATE:
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,16,RSRC_MENU_CLOCK+g_iLanguage);
			break;
		case MENU_AB:
			g_iAB_Control_Mode = TRUE;
		   	SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,16,RSRC_MENU_AB+g_iLanguage);
			break;
           	case MENU_DELETE:
		   	SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,16,RSRC_MENU_DELETE+g_iLanguage);
			break;
           	case MENU_ABOUT:
		   	SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,16,RSRC_MENU_ABOUT+g_iLanguage);
			break;
	    	case MENU_MAIN_EXIT:
			g_iAB_Control_Mode = FALSE;
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,16,RSRC_MENU_EXIT+g_iLanguage);
			break;
		default:
			break;           	
           }			  
		   
            bSendToPlayerStateMachine = FALSE;
            DisplayHint.I = 0;
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
            	case PH_PLAY:
		case PR_MENU:
		case PR_RV:
		case PH_RV:
			bSendToPlayerStateMachine=FALSE;
			break;
			
                case PR_FF:
                case PH_FF:
                    bSendToPlayerStateMachine = FALSE;
                    DisplayHint.bits.Misc = TRUE;
                    iHighLightedItem++;
                    if(iHighLightedItem > MAINMENU_LAST)
                    {   // roll over
                        iHighLightedItem = MAINMENU_FIRST;
                    }
                    break;

                case PR_RW:
                case PH_RW:
                    bSendToPlayerStateMachine = FALSE;
                    DisplayHint.bits.Misc = TRUE;
                    iHighLightedItem--;
                    if(iHighLightedItem < MAINMENU_FIRST)
                    {   // roll over
                        iHighLightedItem = MAINMENU_LAST;
                    }
                    break;

                case PR_PLAY:
                    //causing selected item to change will have the loop above enter that menu
                    //until it returns MENU_MAIN.
                    iSelectedItem = iHighLightedItem;
                    bSendToPlayerStateMachine = FALSE;
                    break;

                case PR_STOP:
                    if((g_wDecoderSR & DECODER_STOPPED) != 0)
                        bSendToPlayerStateMachine = FALSE;
                    break;

                case PH_MENU:
                    // set flag so PR_MENU in music mode will ignore
                    g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
                    iSelectedItem = MENU_MUSIC;
                    bSendToPlayerStateMachine = FALSE;
                    break;


                case PH_STOP:
        #ifdef JANUS
                    CleanDataStoreFlag = 1;
		#endif
                    //if the STOP button is held down, lets call the shutdown menu
                    ShutdownMenu(USERSHUTDOWN,0,0);
                    // when returns update display  -- if doesn't return, powered down
                    DisplayHint.I |= (MAIN_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
                    bSendToPlayerStateMachine = FALSE;
                    break;
            }
        }
        if(bSendToPlayerStateMachine)
            DisplayHint.I |= SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,pPtr);

		//***** kill timer
		SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_ANIMATE);
    } // end while (1)
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int CreateDataFolder(_packed BYTE *ChkDirectory)
//
//   Type:          Function
//
//   Description:   Since our MTP device does not report that we support JPEG or SMV
//                  formats the folders that hold these files have to show up in the
//                  DATA folder.  This is done by setting their MTP database
//                  non-consumable property to 1.  When data folders are added in
//                  MTP mode the firmware places a hidden file in the root folder to
//                  indicate that it's a data folder.  That way when the MTP
//                  database has to be rebuilt it can check for that file in all root
//                  folders and know if it's a data folder or a media folder.
//
//                  This function is used to create the data folders and add that
//                  special file to the folder.  This is only needed for root folders.
//                  All subfolders will have the same non-consumbable property as
//                  their parent.
//
//   Outputs:       int
//                  TRUE if the folder was created and the MTP database needs rebuilding
//                  FALSE if the folder already exists
//
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant int CreateDataFolder(_packed BYTE *ChkDirectory)
{
    int FileHandle;
    INT AttValue;
    int Result;
    int FileCreateFlag[3];

    Result = FALSE;
    if(FSChangeDir(ChkDirectory,0)<0)
    {

        FSCreateDir(ChkDirectory,0);
        FSChangeDir(ChkDirectory,0);

        FileCreateFlag[0] = (int)'w';
        FileCreateFlag[1] = (int)'+';
        FileCreateFlag[2] = 0;
        FileHandle = Fopen(g_DataTagName, (_packed char *) FileCreateFlag);

        // Give the file Hidden attributes so it does
        // not show up in the data folder in Explorer.
        AttValue = filegetattrib ((_packed char *)g_DataTagName);
        filesetattrib ((int)FileHandle, AttValue | ATTR_HIDDEN);

        Fclose(FileHandle);
        Result = TRUE;
    }

    return Result;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int ShutdownMenu( int iIgnored1, int iIgnored2, int *pPtr)
//
//   Type:          Function (menu)
//
//   Description:   This simple menu handles the wait for shutdown while
//                  the stop button is pressed and held.
//
//   Inputs:        pPtr is a pointer to a struct CMessage buffer to recieve messages into
//
//   Outputs:       BOOL
//                  TRUE if the player should shutdown
//                  FALSE if the shutdown activity was aborted.
//
//   Notes:         If any button activity happens other than PH_STOP, this menu
//                  exits and returns FALSE.
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant int ShutdownMenu( int bForceShutdown, int bLowBatt, int *pPtr)
//////////////////////////////////////////////////////////////////////
{
    // wForceShutdown is really a BOOL, but menu function prototypes are fixed
    BOOL bDone     = FALSE;
    INT  iShutdownCount=0;
    BOOL bSendToPlayerStateMachine=FALSE;
    INT iEvent;

    // This is required to get the message into the playerstatemachine
    // casted to avoid pointer mismatch warnings
    pPtr = (int *) &gEventInfo;

    while((!bDone) && (!bForceShutdown))
    {
        iEvent = SysWaitOnEvent(EVENT_MESSAGE|EVENT_BUTTON,&gEventInfo.msg,-1);
        // Above will return when we get a message, or an interval of 1 sec
        // has passed since we last returned.

        bSendToPlayerStateMachine = TRUE;
        //This block is for handling the player state machine related messages.

        switch(iEvent)
        {
            case EVENT_BUTTON:
                switch(gEventInfo.Button.wButtonEvent)
                {
                case PH_PLAY:
                case PH_STOP:
                    bSendToPlayerStateMachine = FALSE;
                    iShutdownCount++;  // each count = rate of button isr
                    //if(iShutdownCount > SHUTDOWN_COUNT)   // too fast if no clear display. Must be cooperative with display.c
                    if((iShutdownCount >> 1) > SHUTDOWN_COUNT)
                    {
                        bForceShutdown = TRUE;
				       #ifdef JANUS
    				    if ((CleanDataStoreFlag == 1) && (LicenseDeleted==1))
 				        {
						    DisplayClear();
							SysCallFunction(RSRC_DISPLAY_CODE_BANK,DisplayDefragmentstore,TRUE,
	                            0,0);
						 }
					   #endif
                    } else
                        SysCallFunction(RSRC_DISPLAY_CODE_BANK,DisplayShutdownProgress,TRUE,
                            iShutdownCount,0);
                    break;
                case PR_STOP:
                    bSendToPlayerStateMachine = FALSE;
                    // Falls through to set bDone
                default:
                    bDone = TRUE;
                    break;
                }
                break;
        }
        if(bSendToPlayerStateMachine)
            SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,pPtr);
    } // end while

    if (bForceShutdown)
    {
        //////////////////////////////////////////////////////////////////////
        //One time shutdown stuff needs to go here                          //
        //////////////////////////////////////////////////////////////////////
  #ifdef USE_PLAYLIST3
        write_detection_access(3);		//added by samuel on 31/1/05 for flash write detection
  #endif

        if(g_wDecoderSR & DECODER_STOPPED)
            DecoderForceInit();

        SysPostMessage(2,DECODER_STOP);
        SysPostMessage(2,ENCODER_STOP);
        if (bLowBatt)
        {   // display low battery message
            DisplayClear();
            DisplayLowBattery();
            SysWaitOnEvent(0,0,700);  // let user see it
        }
	else
		{
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,RSRC_JVJ_SHUTDOWN_PLAYER+g_iLanguage);
		SysWaitOnEvent(0,0,700);  // let user see it
		}

#ifdef	AUDIBLE
		// If an Audible file, save the playback position now.
		if ( g_AudibleFlags.bits.IsAudibleFile ) {
			SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK, AudibleStopAndSave, FALSE, 0, 0 );
		}
#endif
#ifndef REMOVE_FM
        SysPostMessage(2,TUNER_POWER_OFF);	// powerdown the FM Tuner
#endif
#ifdef USE_PLAYLIST3
		SysWaitOnEvent(0,0,0);  // let user see it
		if (g_ML_save_on_exit == 1) {
			ML_building_engine_init();
			SysLoadFATWrite();
			ML_SaveOnTheGo();
			SysUnLoadFATWrite();
		}
#endif // #ifdef USE_PLAYLIST3
        while(!((g_wDecoderSR & DECODER_STOPPED)))
            SysWaitOnEvent(0,0,100);  // let user see it

        #ifdef JANUS
        if ((CleanDataStoreFlag == 1) && (LicenseDeleted==1))
        {
            JanusCleanDataStore();
            CleanDataStoreFlag = 0;
            LicenseDeleted=0;
        }
        #endif


        //////////////////////////////////////////////////////////////////////
        // Note: Don't DisplayClear() until the settings file is saved!!
        // Ask the system to shut down and halt the user interface thread.
        //SysLoadFATWrite();    // Don't need to call SysLoadFATWrite() if SaveUserTime() is moved out.

         /*
        Call moved to function SysSaveSettings
        SaveUserTime();
        */
        SysPostMessage(2,SYSTEM_SHUTDOWN_FINAL);

        while ( g_iSysSaveSettingsDone == 0 )
        {   SysWaitOnEvent(0,0,2);
        }
        // Settings file has been saved to persistent storage media.
        DisplayClear();               // LCD module clears display after settings saved.

        SysWaitOnEvent(0,0,-1);       // Wait on nothing. Sleep forever.
        //////////////////////////////////////////////////////////////////////
    }
    return 0;
}



/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int SplashScreen( int iResource, int iTime, int *pPtr)
//
//   Type:          Function (menu)
//
//   Description:   Display the given resource for iTime millisec
//
//
//   Inputs:        iResource is the resource to display
//                  iTime is display time in millisec to splash the resource onscreen
//                  pPtr is a pointer to a struct CMessage buffer to recieve messages into
//
//   Outputs:       0
//
//   Notes:
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant int SplashScreen( int iResource, int iTime, int *pPtr)
//////////////////////////////////////////////////////////////////////
{
    BOOL bDone = FALSE;
    BOOL bSendToPlayerStateMachine;
    union DisplayHints DisplayHint;
    LONG iFinishTime = (LONG)iTime + SysGetCurrentTime();
    INT   iTimeToWait;
    int iEvent;

    DisplayHint.I = 0xffffff;
    while(!bDone)
    {

        //keep only display hints we're interested in displaying in this menu.
        DisplayHint.I &= (SPLASH_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
        if(DisplayHint.I)
        {
            SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHint.I,1,0);
            SysPostMessage(5,LCD_PRINT_RANGE_RSRC,SPLASH_SCREEN_X_POS,SPLASH_SCREEN_Y_POS,iResource);
            DisplayHint.I = 0;//clear out the bits
            SysWaitOnEvent(0,0,1);//need this wait here so the splash screen comes up ASAP.
            // If a lot is happening in the menus, the display won't get to run
            // until everything is finished in the menus. By then the timeout
            // is just about expired.
        }
        iTimeToWait = (LONG)(iFinishTime - (LONG)SysGetCurrentTime());
        if(iTimeToWait > 0)
        {
            iEvent = SysWaitOnEvent(EVENT_MESSAGE,&gEventInfo.msg, iTimeToWait);

            //Lets assume we do want to send all these to the player state machine

            switch(iEvent)
            {
            case EVENT_TIMER:
                // Don't need to send this to the player state machine
                bSendToPlayerStateMachine = FALSE;
                bDone = TRUE;
                break;
            case EVENT_MESSAGE:
                bSendToPlayerStateMachine = TRUE;
                break;
            default:
                bSendToPlayerStateMachine = FALSE;
                break;
            }
            if(bSendToPlayerStateMachine)
            {//only send events to the player state machine if necessary (to avoid code bank swapping)
                DisplayHint.I |= SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,(void*)&gEventInfo.msg);
                //or the display hints returned with the current collection of hints
            }
        }
        else
        {
            bDone = TRUE;
        }

    }
    return 0;

}
#ifdef USE_PLAYLIST3
#pragma asm
    nolist
        include "resource.inc"
        include "sysmacro.asm"
    list
    org y,"Player_settings_Y":
    extern y:FhasInsertSD
    extern y:FsongPath
    extern y:Fplay_flash_name_list_size
    extern y:Fplay_sd_name_list_size
    extern y:Forg_songPath
    extern y:Forg_play_flash_name_list_size
    extern y:Forg_play_sd_name_list_size
    extern y:Forg_currentPlayAllLevel
    extern y:Fplay_flash_window_pos
    extern y:Fplay_sd_window_pos
    extern y:Fplay_total_merge_num_item
    extern y:Fplay_window_merge_index
    extern y:FplayAllLevel
    extern y:FisPlayAll
    extern y:FlastPlaySongLevel
    extern y:FcurrentPlayAllLevel
    extern y:FplayMode
    extern y:FlastSongID
    extern y:Fg_current_play_all
    extern y:FsongIndexInMedia

    extern y:Fg_DriveFlushCnt

    SaveRange FhasInsertSD,1
    SaveRange FsongPath,5
    SaveRange Fplay_flash_name_list_size,1
    SaveRange Fplay_sd_name_list_size,1
    SaveRange Forg_songPath,5
    SaveRange Forg_play_flash_name_list_size,1
    SaveRange Forg_play_sd_name_list_size,1
    SaveRange Forg_currentPlayAllLevel,1
    SaveRange Fplay_flash_window_pos,1
    SaveRange Fplay_sd_window_pos,1
    SaveRange Fplay_total_merge_num_item,1
    SaveRange Fplay_window_merge_index,1
    SaveRange FplayAllLevel,1
    SaveRange FisPlayAll,1
    SaveRange FlastPlaySongLevel,1
    SaveRange FcurrentPlayAllLevel,1
    SaveRange FplayMode,1
    SaveRange FlastSongID,1
    SaveRange Fg_current_play_all,1
    SaveRange FsongIndexInMedia,2

    SaveRange Fg_DriveFlushCnt,1

#pragma endasm
#endif
