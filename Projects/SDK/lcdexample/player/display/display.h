#ifndef _DISPLAY_H
#define _DISPLAY_H

//Display bits
#define DISPLAY_CLEAR_DISPLAY_BITPOS        0

#define DISPLAY_ENCODER_TRACK_TIME_BITPOS   1
#define DISPLAY_ENCODER_TRACK_NAME_BITPOS   2
#define DISPLAY_RECORD_MODE_BITPOS          3

//This bit (DISPLAY_MISC_BITPOS) is set to allow DisplayHints to be non zero, but does not map
//to anything in UpdateDisplay();  (meant to be used to indicate that specific
//things that are specific for that menu.
#define DISPLAY_MISC_BITPOS                 4

#define DISPLAY_FMTUNER_FREQUENCY           5
#define DISPLAY_FMTUNER_PRESET              6
#define DISPLAY_FMTUNER_STRENGTH            7

#define DISPLAY_PLAYSTATE_BITPOS             9
#define DISPLAY_AB_ICON_BITPOS              10
#define DISPLAY_PLAYMODE_BITPOS             11
#define DISPLAY_PLAYSET_BITPOS              12

//Playback info
#define DISPLAY_EQ_BITPOS                   13
#define DISPLAY_VOLUME_BITPOS               14

//Track Info
#define DISPLAY_SONGARTISTALBUM_BITPOS      15
#define DISPLAY_SONGTITLE_BITPOS            16
#define DISPLAY_CURRENTTRACK_BITPOS         17
#define DISPLAY_TRACKTIME_BITPOS            18
#define DISPLAY_TOTALTRACKTIME_BITPOS        8
#define DISPLAY_BITRATE_BITPOS              19

//Device status info
#define DISPLAY_LOCK_BITPOS                 20
#define DISPLAY_DISK_BITPOS                 21
#define DISPLAY_BATTERY_BITPOS              22
#define DISPLAY_ALBUMART_BITPOS              23

#ifdef PLAYER_STRESS
// this may not work since album art uses bit 23 so we may need a new word variable for this player stress test bit below.
#define DISPLAY_STRESSTEST_BITPOS            23
#endif


union DisplayHints
{
    struct {
        int ClearDisplay        :1;//0
        int EncoderTrackTime    :1;//1
        int EncoderTrackName    :1;//2
        int RecordMode          :1;//3
        int Misc                :1;//4
        int FMTunerFrequency    :1;//5
        int FMTunerPreset       :1;//6
        int FMTunerStrength     :1;//7
        int TotalTrackTime      :1;//8
        int PlayState           :1;//9
        int ABIcon              :1;//10
        int PlayMode            :1;//11
        int PlaySet             :1;//12
        int EQ                  :1;//13
        int Volume              :1;//14
        int SongArtistAlbum     :1;//15
        int SongTitle           :1;//16
        int CurrentTrack        :1;//17
        int TrackTime           :1;//18
        int Bitrate             :1;//19
        int LockIcon            :1;//20
        int Disk                :1;//21
        int Battery             :1;//22
#ifdef PLAYER_STRESS
        int StressTest         :1;//23  // this might not work since bit 23 is already taken by album art. 23 is last bit in word.
#else
        int AlbumArt            :1;//23
#endif
    } bits;
    WORD I;
};

// setup default display for all menus
// can be adjusted in each menus as required.
#define COMMON_DISPLAY_ALL  (\
                                (1<<DISPLAY_BATTERY_BITPOS)|\
                                (1<<DISPLAY_LOCK_BITPOS)|\
                                (1<<DISPLAY_VOLUME_BITPOS)|\
                                (1<<DISPLAY_DISK_BITPOS)|\
                                (1<<DISPLAY_PLAYSET_BITPOS)|\
                                (1<<DISPLAY_AB_ICON_BITPOS)|\
                                (1<<DISPLAY_EQ_BITPOS)|\
                                (1<<DISPLAY_PLAYMODE_BITPOS)|\
                                (1<<DISPLAY_PLAYSTATE_BITPOS)\
                                )

#define SPECTROGRAM_MENU_DISPLAY_ALL  (\
                                (1<<DISPLAY_LOCK_BITPOS)|\
                                (1<<DISPLAY_DISK_BITPOS)|\
                                (1<<DISPLAY_PLAYSET_BITPOS)|\
                                (1<<DISPLAY_AB_ICON_BITPOS)|\
                                (1<<DISPLAY_EQ_BITPOS)|\
                                (1<<DISPLAY_PLAYMODE_BITPOS)|\
                                (1<<DISPLAY_PLAYSTATE_BITPOS)\
                                )

//-----------------------------------------------
#ifdef JPEG_ALBUM_ART
#define ALBUMART_SET_MASK_IF_APPROPRIATE (1<<DISPLAY_ALBUMART_BITPOS)
#else
#define ALBUMART_SET_MASK_IF_APPROPRIATE (0)
#endif

#ifdef PLAYER_STRESS
#define PLAYER_STRESSTEST_SET_MASK_IF_APPROPRIATE (1<<DISPLAY_STRESSTEST_BITPOS)
#else
#define PLAYER_STRESSTEST_SET_MASK_IF_APPROPRIATE (0)
#endif


#define MUSIC_MENU_DISPLAY_ALL  (\
                                (COMMON_DISPLAY_ALL)|\
                                (ALBUMART_SET_MASK_IF_APPROPRIATE)|\
                                (1<<DISPLAY_PLAYSTATE_BITPOS)|\
                                (1<<DISPLAY_VOLUME_BITPOS)|\
                                (1<<DISPLAY_SONGARTISTALBUM_BITPOS)|\
                                (1<<DISPLAY_SONGTITLE_BITPOS)|\
                                (1<<DISPLAY_CURRENTTRACK_BITPOS)|\
                                (1<<DISPLAY_TRACKTIME_BITPOS)|\
                                (1<<DISPLAY_TOTALTRACKTIME_BITPOS)\
                                )

#define MUSIC_MENU_DISPLAY_NEW_TRACK (\
                                (ALBUMART_SET_MASK_IF_APPROPRIATE)|\
                                (PLAYER_STRESSTEST_SET_MASK_IF_APPROPRIATE)|\
                                (1<<DISPLAY_SONGARTISTALBUM_BITPOS)|\
                                (1<<DISPLAY_SONGTITLE_BITPOS)|\
                                (1<<DISPLAY_CURRENTTRACK_BITPOS)|\
                                (1<<DISPLAY_TRACKTIME_BITPOS)|\
                                (1<<DISPLAY_TOTALTRACKTIME_BITPOS)|\
                                (1<<DISPLAY_AB_ICON_BITPOS)|\
                                (1<<DISPLAY_PLAYSTATE_BITPOS)|\
                                (1<<DISPLAY_PLAYSET_BITPOS)\
                                )
//-----------------------------------------------


#define MVIDEO_MENU_DISPLAY_ALL ((1<<DISPLAY_CURRENTTRACK_BITPOS)|(1<<DISPLAY_SONGTITLE_BITPOS))


#define VOICE_MENU_DISPLAY_ALL  (\
                                COMMON_DISPLAY_ALL|\
                                (1<<DISPLAY_PLAYMODE_BITPOS)|\
                                (1<<DISPLAY_PLAYSTATE_BITPOS)|\
                                (1<<DISPLAY_VOLUME_BITPOS)|\
                                (1<<DISPLAY_SONGARTISTALBUM_BITPOS)|\
                                (1<<DISPLAY_SONGTITLE_BITPOS)|\
                                (1<<DISPLAY_CURRENTTRACK_BITPOS)|\
                                (1<<DISPLAY_TOTALTRACKTIME_BITPOS)|\
                                (1<<DISPLAY_TRACKTIME_BITPOS)\
                                )


#define RECORDING_DISPLAY_ALL   (\
                                COMMON_DISPLAY_ALL|\
                                (1<<DISPLAY_ENCODER_TRACK_TIME_BITPOS)|\
                                (1<<DISPLAY_ENCODER_TRACK_NAME_BITPOS)|\
                                (1<<DISPLAY_PLAYSTATE_BITPOS)|\
                                (1<<DISPLAY_VOLUME_BITPOS)|\
                                (1<<DISPLAY_RECORD_MODE_BITPOS)\
                                )

#ifdef USE_PLAYLIST3
#define BROWSE_MENU_DISPLAY_ALL  (\
                                (1<<DISPLAY_BATTERY_BITPOS)|\
                                (1<<DISPLAY_LOCK_BITPOS)|\
                                (1<<DISPLAY_DISK_BITPOS)|\
                                (1<<DISPLAY_PLAYSET_BITPOS)|\
                                (1<<DISPLAY_MISC_BITPOS)\
                                )
#else
#ifdef USE_PLAYLIST5
#if 0
#define BROWSE_MENU_DISPLAY_ALL (\
                                 COMMON_DISPLAY_ALL|\
                                (ALBUMART_SET_MASK_IF_APPROPRIATE)|\
                                (1<<DISPLAY_PLAYSTATE_BITPOS)|\
                                (1<<DISPLAY_VOLUME_BITPOS)|\
                                (1<<DISPLAY_CURRENTTRACK_BITPOS)|\
                                (1<<DISPLAY_MISC_BITPOS)\
                                )
#else
#define BROWSE_MENU_DISPLAY_ALL (\
                                 COMMON_DISPLAY_ALL|\
                                (ALBUMART_SET_MASK_IF_APPROPRIATE)|\
                                (1<<DISPLAY_PLAYSTATE_BITPOS)|\
                                (1<<DISPLAY_VOLUME_BITPOS)|\
                                (1<<DISPLAY_MISC_BITPOS)\
                                )
#endif
#endif  // #ifdef USE_PLAYLIST5
#endif  // #ifdef USE_PLAYLIST3



#define MAIN_MENU_DISPLAY_ALL           (COMMON_DISPLAY_ALL|(1<<DISPLAY_MISC_BITPOS))

#define EQ_MENU_DISPLAY_ALL             (\
                                        COMMON_DISPLAY_ALL|\
                                        (1<<DISPLAY_EQ_BITPOS)|\
                                        (1<<DISPLAY_VOLUME_BITPOS)\
                                        )     // (SDK2.520)

#define PM_MENU_DISPLAY_ALL             (COMMON_DISPLAY_ALL|(1<<DISPLAY_PLAYMODE_BITPOS)|(1<<DISPLAY_MISC_BITPOS))
#define PWR_MENU_DISPLAY_ALL            (COMMON_DISPLAY_ALL|(1<<DISPLAY_MISC_BITPOS))
#define SETTINGS_MENU_DISPLAY_ALL       (COMMON_DISPLAY_ALL|(1<<DISPLAY_MISC_BITPOS))
#define JPEG_DISPLAY_MENU_DISPLAY_ALL       (COMMON_DISPLAY_ALL|(1<<DISPLAY_MISC_BITPOS))
#define DELETE_MENU_DISPLAY_ALL         (COMMON_DISPLAY_ALL|(1<<DISPLAY_MISC_BITPOS))
#define ABOUT_MENU_DISPLAY_ALL          (COMMON_DISPLAY_ALL)
#define ERASE_MENU_DISPLAY_ALL          ((1<<DISPLAY_BATTERY_BITPOS)|(1<<DISPLAY_LOCK_BITPOS))

#define CONTRAST_MENU_DISPLAY_ALL       (COMMON_DISPLAY_ALL|(1<<DISPLAY_MISC_BITPOS))

#define TIME_DATE_MENU_DISPLAY_ALL	0
#define RECORDTEST_DISPLAY_ALL       (\
	                                (1<<DISPLAY_ENCODER_TRACK_TIME_BITPOS)|\
									(1<<DISPLAY_ENCODER_TRACK_NAME_BITPOS)|\
									(1<<DISPLAY_PLAYSTATE_BITPOS)|\
	                                (1<<DISPLAY_RECORD_MODE_BITPOS)|\
                                    (1<<DISPLAY_MISC_BITPOS)\
									)

#define SPLASH_MENU_DISPLAY_ALL         (COMMON_DISPLAY_ALL|(1<<DISPLAY_MISC_BITPOS))
// used during track update to force total time update
#define AUTOUPDATE		FALSE
#define FORCEUPDATE		TRUE


// based on font used for title/artist display
#define DEFAULT_CHAR_WIDTH  5


// LCD dimensions
#ifdef LDS514_LCD
#define   LCD_SIZE_X       96
#define   LCD_SIZE_Y       64
#endif

#ifdef ILI814_LCD
#define   LCD_SIZE_X       96
#define   LCD_SIZE_Y       64
#endif

#ifdef ML9341_LCD
#define   LCD_SIZE_X       96
#define   LCD_SIZE_Y       96
#endif

#ifdef SSD1332_LCD
#define   LCD_SIZE_X       96
#define   LCD_SIZE_Y       64
#endif

#ifdef S6B33B0A_LCD
#define   LCD_SIZE_X       128
#define   LCD_SIZE_Y       159
#endif

#ifdef SED15XX_LCD
#define LCD_SIZE_X                      128
#define LCD_SIZE_Y                      64
#endif

#define LCD_SIZE_ROW                     8

#define TOP_OF_SCREEN           0
#define LEFT_OF_SCREEN          0

#ifdef LDS514_LCD
#define   SCREEN_WIDTH       96
#define   SCREEN_HEIGHT       64
#endif

#ifdef ILI814_LCD
#define   SCREEN_WIDTH       96
#define   SCREEN_HEIGHT       64
#endif

#ifdef ML9341_LCD
#define   SCREEN_WIDTH       96
#define   SCREEN_HEIGHT       96
#endif

#ifdef SSD1332_LCD
#define   SCREEN_WIDTH       96
#define   SCREEN_HEIGHT       64
#endif

#ifdef S6B33B0A_LCD
#define   SCREEN_WIDTH       128
#define   SCREEN_HEIGHT       159
#endif

#ifdef SED15XX_LCD
#define SCREEN_WIDTH            128
#define SCREEN_HEIGHT           64
#endif

#define CHAR_SIZE_X             6
#define CHAR_SIZE_Y             8

// Physical rows are numbered from 0 to LCD_SIZE_Y/LCD_SIZE_ROW
#define LCD_FIRST_ROW_NUM                1
#define LCD_FIRST_ROW_POS_Y=LCD_FIRST_ROW_NUM*LCD_SIZE_ROW  // skipping the first row

// Top row in order from left to right
#define PLAY_STATE_ICON_X_POS   LEFT_OF_SCREEN+0
#define PLAY_STATE_ICON_Y_POS   TOP_OF_SCREEN+0
#define PLAY_STATE_ICON_X_SIZE  12
#define PLAY_STATE_ICON_Y_SIZE  8

#define REPEAT_ICON_X_POS       LEFT_OF_SCREEN+13
#define REPEAT_ICON_Y_POS       TOP_OF_SCREEN+0
#define REPEAT_ICON_X_SIZE      12
#define REPEAT_ICON_Y_SIZE      8

#define SHUFFLE_ICON_X_POS      LEFT_OF_SCREEN+25
#define SHUFFLE_ICON_Y_POS      TOP_OF_SCREEN+0
#define SHUFFLE_ICON_X_SIZE     5     // includes white space on left side of .bmp
#define SHUFFLE_ICON_Y_SIZE     8

#define AB_ICON_X_POS           LEFT_OF_SCREEN+31
#define AB_ICON_Y_POS           TOP_OF_SCREEN+0
#define AB_ICON_X_SIZE          12
#define AB_ICON_Y_SIZE          8

#ifdef	AUDIBLE
#define SECNAV_ICON_X_POS       LEFT_OF_SCREEN+31
#define SECNAV_ICON_Y_POS       TOP_OF_SCREEN+0
#define SECNAV_ICON_X_SIZE      12
#define SECNAV_ICON_Y_SIZE      8
#endif

#define EQ_ICON_X_POS           LEFT_OF_SCREEN+44
#define EQ_ICON_Y_POS           TOP_OF_SCREEN+0
#define EQ_ICON_X_SIZE          22
#define EQ_ICON_Y_SIZE          8

#ifdef WOW
#define WOW_X_POS               EQ_ICON_X_POS
#define WOW_Y_POS               EQ_ICON_Y_POS
#endif


// Media icon
#define DISK_X_POS              LEFT_OF_SCREEN+66
#define DISK_Y_POS              TOP_OF_SCREEN
#define DISK_X_SIZE             8
#define DISK_Y_SIZE             8

// Hold icon
#define LOCK_ICON_X_POS         LEFT_OF_SCREEN+73
#define LOCK_ICON_Y_POS         TOP_OF_SCREEN
#define LOCK_ICON_X_SIZE        8
#define LOCK_ICON_Y_SIZE        8

#define BATTERY_ICON_X_POS      LEFT_OF_SCREEN+110
#define BATTERY_ICON_Y_POS      TOP_OF_SCREEN+0
#define BATTERY_ICON_X_SIZE     16
#define BATTERY_ICON_Y_SIZE     8
#define BATTERY_RESOURCE        RSRC_BATTERY_00

// Second row from left to right
#define VOLUME_ICON_X_POS       LEFT_OF_SCREEN+0
#define VOLUME_ICON_Y_POS       TOP_OF_SCREEN+8
#define VOLUME_ICON_X_SIZE      48
#define VOLUME_ICON_Y_SIZE      8
#define VOLUME_RESOURCE         RSRC_ICON_VOL_00
#define VOLUME_NUM_ICONS        27

#define PLAYSET_X_POS           SCREEN_WIDTH-8
#define PLAYSET_Y_POS           TOP_OF_SCREEN+8

//Lyrics Position
#define LYRICS_DISPLAY_X_POS	LEFT_OF_SCREEN
#define LYRICS_DISPLAY_Y_POS	TOP_OF_SCREEN+16
#define LYRICS_DISPLAY_HEIGHT		10

// Music menu
#define SONG_TITLE_X_POS        LEFT_OF_SCREEN+0
#define SONG_TITLE_Y_POS        TOP_OF_SCREEN+16
#define SONG_TITLE_X_SIZE       SCREEN_WIDTH
#define SONG_TITLE_Y_SIZE       10

#define SONG_ARTIST_X_POS       LEFT_OF_SCREEN
#define SONG_ARTIST_Y_POS       TOP_OF_SCREEN+26
#define SONG_ARTIST_X_SIZE      SCREEN_WIDTH
#define SONG_ARTIST_Y_SIZE      10

#define SONG_ALBUM_X_POS       LEFT_OF_SCREEN
#define SONG_ALBUM_Y_POS       TOP_OF_SCREEN+36
#define SONG_ALBUM_X_SIZE      SCREEN_WIDTH
#define SONG_ALBUM_Y_SIZE      10

#ifdef USE_PLAYLIST5
#define TRACK_NUMBER_X_POS      LEFT_OF_SCREEN+28
#define TRACK_NUMBER_Y_POS      TOP_OF_SCREEN+56
#define TRACK_NUMBER_X_SIZE     6*4
#define TRACK_NUMBER_Y_SIZE     8

#define TRACK_COUNT_X_POS       TRACK_NUMBER_X_POS+TRACK_NUMBER_X_SIZE+10
#define TRACK_COUNT_Y_POS       TOP_OF_SCREEN+56
#define TRACK_COUNT_X_SIZE      6*4
#define TRACK_COUNT_Y_SIZE      8

#define TRACK_DIVIDE_X TRACK_NUMBER_X_POS+TRACK_COUNT_X_SIZE + 2
#else
#define TRACK_NUMBER_X_POS      LEFT_OF_SCREEN+34
#define TRACK_NUMBER_Y_POS      TOP_OF_SCREEN+56
#define TRACK_NUMBER_X_SIZE     6*3
#define TRACK_NUMBER_Y_SIZE     8

#define TRACK_COUNT_X_POS       TRACK_NUMBER_X_POS+TRACK_NUMBER_X_SIZE+10
#define TRACK_COUNT_Y_POS       TOP_OF_SCREEN+56
#define TRACK_COUNT_X_SIZE      6*3
#define TRACK_COUNT_Y_SIZE      8

#define TRACK_DIVIDE_X  TRACK_NUMBER_X_POS+24
#endif


#define TRACK_CURR_TIME_X_POS       LEFT_OF_SCREEN+28
#define TRACK_CURR_TIME_Y_POS       TOP_OF_SCREEN+48
#define TRACK_CURR_TIME_X_SIZE      25
#define TRACK_CURR_TIME_Y_SIZE      8

#define TRACK_SLASH_X_SIZE          5
#define TRACK_SLASH_X_POS           TRACK_TOTAL_TIME_X_POS-TRACK_SLASH_X_SIZE
#define TRACK_SLASH_Y_POS           TRACK_CURR_TIME_Y_POS

#define TRACK_TOTAL_TIME_X_POS      LEFT_OF_SCREEN+60
#define TRACK_TOTAL_TIME_Y_POS      TRACK_CURR_TIME_Y_POS
#define TRACK_TOTAL_TIME_X_SIZE     28
#define TRACK_TOTAL_TIME_Y_SIZE     8

#define VBR_FLAG_X_POS      LEFT_OF_SCREEN+80
#define VBR_FLAG_Y_POS      8
#define VBR_FLAG_X_SIZE     20
#define VBR_FLAG_Y_SIZE     8
#ifdef JPEG_ALBUM_ART
// Album art display parameters:
#define	ALBUM_ART_X_POS		TOP_OF_SCREEN
#define	ALBUM_ART_Y_POS		80
#define	ALBUM_ART_X_SIZE	SCREEN_WIDTH
#define	ALBUM_ART_Y_SIZE	(SCREEN_HEIGHT-ALBUM_ART_Y_POS)
#endif

// Recording Display
#define REC_CURR_TIME_X_POS       LEFT_OF_SCREEN
#define REC_CURR_TIME_Y_POS       TOP_OF_SCREEN+48
#define REC_CURR_TIME_X_SIZE      12
#define REC_CURR_TIME_Y_SIZE      8

#define REC_TOTAL_TIME_X_POS      LEFT_OF_SCREEN+45
#define REC_TOTAL_TIME_Y_POS      TOP_OF_SCREEN+48
#define REC_TOTAL_TIME_X_SIZE     28
#define REC_TOTAL_TIME_Y_SIZE     8

// Playback Display with hours added.
#ifdef LDS514_LCD
#define HR_TRACK_CURR_TIME_X_POS  	LEFT_OF_SCREEN+16
#endif

#ifdef ILI814_LCD
#define HR_TRACK_CURR_TIME_X_POS  	LEFT_OF_SCREEN+16
#endif

#ifdef ML9341_LCD
#define HR_TRACK_CURR_TIME_X_POS  	LEFT_OF_SCREEN+16
#endif

#ifdef SSD1332_LCD
#define HR_TRACK_CURR_TIME_X_POS  	LEFT_OF_SCREEN+16
#endif

#ifdef S6B33B0A_LCD
#define HR_TRACK_CURR_TIME_X_POS  	LEFT_OF_SCREEN+16
#endif

#ifdef SED15XX_LCD
#define HR_TRACK_CURR_TIME_X_POS  	LEFT_OF_SCREEN+16
#endif

#define HR_TRACK_CURR_TIME_Y_POS 	TOP_OF_SCREEN+48
#define HR_TRACK_CURR_TIME_X_SIZE 	28
#define HR_TRACK_CURR_TIME_Y_SIZE 	8

#define HR_TRACK_TOTAL_TIME_X_POS 	LEFT_OF_SCREEN+59
#define HR_TRACK_TOTAL_TIME_Y_POS  	TRACK_CURR_TIME_Y_POS
#define HR_TRACK_TOTAL_TIME_X_SIZE 	28
#define HR_TRACK_TOTAL_TIME_Y_SIZE 	8

//Clear entire Track Time when song changes.
#define CLR_TRACK_TIME_X_POS 		LEFT_OF_SCREEN
#define CLR_TRACK_TIME_Y_POS  		TRACK_CURR_TIME_Y_POS
#define CLR_TRACK_TIME_X_SIZE 		SCREEN_WIDTH
#define CLR_TRACK_TIME_Y_SIZE 		8

#define HR_TRACK_SLASH_X_SIZE 		3
#define HR_TRACK_SLASH_X_POS    	HR_TRACK_TOTAL_TIME_X_POS-HR_TRACK_SLASH_X_SIZE
#define HR_TRACK_SLASH_Y_POS      	HR_TRACK_CURR_TIME_Y_POS

 //DVRWARN
#define VOICE_SPACE_WARN_X_POS      LEFT_OF_SCREEN
#define VOICE_SPACE_WARN_Y_POS      SONG_TITLE_Y_POS
#define VOICE_SPACE_WARN_X_SIZE     93
#define VOICE_SPACE_WARN_Y_SIZE     16

//Shutdown
#define SHUTDOWN_PROGRESS_X_POS     LEFT_OF_SCREEN+24
#define SHUTDOWN_PROGRESS_Y_POS     TOP_OF_SCREEN+32
#define SHUTDOWN_PROGRESS_X_SIZE    59
#define SHUTDOWN_PROGRESS_Y_SIZE    16
#define SHUTDOWN_PROGRESS_RESOURCE  RSRC_PDOWN_STATUS_0

#define SHUTDOWN_STRING_X_POS       LEFT_OF_SCREEN+10
#define SHUTDOWN_STRING_Y_POS       TOP_OF_SCREEN+16
#define SHUTDOWN_STRING_X_SIZE      59
#define SHUTDOWN_STRING_Y_SIZE      16
#define SHUTDOWN_STRING_RESOURCE    RSRC_PDOWN_HOLD_STRING

#define DEFRAGMENT_STRING_X_POS       LEFT_OF_SCREEN+0
#define DEFRAGMENT_STRING_Y_POS       TOP_OF_SCREEN+16
#define DEFRAGMENT_STRING_X_SIZE      59
#define DEFRAGMENT_STRING_Y_SIZE      16
#define DEFRAGMENT_STRING_RESOURCE    RSRC_DEFRAGMENT_STORE_STRING


//Contrast Display
#define CONTRAST_TITLE_X_POS        LEFT_OF_SCREEN+0
#define CONTRAST_TITLE_Y_POS        TOP_OF_SCREEN+16

#define CONTRAST_X_POS              LEFT_OF_SCREEN+8
#define CONTRAST_Y_POS              TOP_OF_SCREEN+32
#define CONTRAST_X_SIZE             96
#define CONTRAST_Y_SIZE             8
#define CONTRAST_RESOURCE           RSRC_CONTRAST_LEVEL_0

//Backlight Display
#define BACKLIGHT_TITLE_X_POS        LEFT_OF_SCREEN+0
#define BACKLIGHT_TITLE_Y_POS        TOP_OF_SCREEN+24
#define BACKLIGHT_STATE_X_POS        LEFT_OF_SCREEN+0
#define BACKLIGHT_STATE_Y_POS        (LCD_FIRST_ROW_NUM+4)*LCD_SIZE_ROW

//settings title display
#define SETTINGS_TITLE_X_POS        LEFT_OF_SCREEN+0
#define SETTINGS_TITLE_Y_POS        TOP_OF_SCREEN+8
//jpeg display title display
#define JPEG_DISPLAY_TITLE_X_POS        LEFT_OF_SCREEN+0
#define JPEG_DISPLAY_TITLE_Y_POS        TOP_OF_SCREEN+8
//erase files title display
#define ERASE_TITLE_X_POS           LEFT_OF_SCREEN+0
#define ERASE_TITLE_Y_POS           TOP_OF_SCREEN+8

// Splashscreen
#ifdef LDS514_LCD
#define SPLASH_SCREEN_X_POS         LEFT_OF_SCREEN
#define SPLASH_SCREEN_Y_POS         TOP_OF_SCREEN
#define SPLASH_SCREEN_X_SIZE        96
#define SPLASH_SCREEN_Y_SIZE        64
#define LOCKED_SCREEN_X_POS         LEFT_OF_SCREEN+8
#define LOCKED_SCREEN_Y_POS         TOP_OF_SCREEN+24
#endif

#ifdef ILI814_LCD
#define SPLASH_SCREEN_X_POS         LEFT_OF_SCREEN
#define SPLASH_SCREEN_Y_POS         TOP_OF_SCREEN
#define SPLASH_SCREEN_X_SIZE        96
#define SPLASH_SCREEN_Y_SIZE        64
#define LOCKED_SCREEN_X_POS         LEFT_OF_SCREEN+8
#define LOCKED_SCREEN_Y_POS         TOP_OF_SCREEN+24
#endif

#ifdef ML9341_LCD
#define SPLASH_SCREEN_X_POS         LEFT_OF_SCREEN
#define SPLASH_SCREEN_Y_POS         TOP_OF_SCREEN
#define SPLASH_SCREEN_X_SIZE        96
#define SPLASH_SCREEN_Y_SIZE        96
#define LOCKED_SCREEN_X_POS         LEFT_OF_SCREEN+8
#define LOCKED_SCREEN_Y_POS         TOP_OF_SCREEN+24
#endif

#ifdef SSD1332_LCD
#define SPLASH_SCREEN_X_POS         LEFT_OF_SCREEN
#define SPLASH_SCREEN_Y_POS         TOP_OF_SCREEN
#define SPLASH_SCREEN_X_SIZE        96
#define SPLASH_SCREEN_Y_SIZE        64
#define LOCKED_SCREEN_X_POS         LEFT_OF_SCREEN+8
#define LOCKED_SCREEN_Y_POS         TOP_OF_SCREEN+24
#endif

#ifdef S6B33B0A_LCD
#define SPLASH_SCREEN_X_POS         LEFT_OF_SCREEN
#define SPLASH_SCREEN_Y_POS         TOP_OF_SCREEN + 16
#define SPLASH_SCREEN_X_SIZE        80
#define SPLASH_SCREEN_Y_SIZE        48
#define LOCKED_SCREEN_X_POS         LEFT_OF_SCREEN+8
#define LOCKED_SCREEN_Y_POS         TOP_OF_SCREEN+24
#endif

#ifdef SED15XX_LCD
#define SPLASH_SCREEN_X_POS         LEFT_OF_SCREEN
#define SPLASH_SCREEN_Y_POS         TOP_OF_SCREEN 
#define SPLASH_SCREEN_X_SIZE        128
#define SPLASH_SCREEN_Y_SIZE        64    
#define LOCKED_SCREEN_X_POS         LEFT_OF_SCREEN+8
#define LOCKED_SCREEN_Y_POS         TOP_OF_SCREEN+24
#endif

// FM tuner
#define FM_TUNER_FREQUENCY_X_POS    LEFT_OF_SCREEN+30
#define FM_TUNER_FREQUENCY_Y_POS    TOP_OF_SCREEN+24
#define FM_TUNER_FREQUENCY_X_SIZE   CHAR_SIZE_X*10
#define FM_TUNER_FREQUENCY_Y_SIZE   CHAR_SIZE_Y
#define FM_TUNER_FREQUENCY_DOT_X_POS    FM_TUNER_FREQUENCY_X_POS+CHAR_SIZE_X*3
#define FM_TUNER_FRAC_FREQUENCY_X_POS    FM_TUNER_FREQUENCY_DOT_X_POS+3

#define FM_TUNER_PRESET_X_POS       LEFT_OF_SCREEN+30
#define FM_TUNER_PRESET_Y_POS       TOP_OF_SCREEN+34
#define FM_TUNER_PRESET_X_SIZE      CHAR_SIZE_X*5
#define FM_TUNER_PRESET_Y_SIZE      CHAR_SIZE_Y

#define FM_TUNER_PILOT_X_POS       LEFT_OF_SCREEN+30
#define FM_TUNER_PILOT_Y_POS       TOP_OF_SCREEN+44

// About menu
#define ABOUT_TITLE_X_POS           LEFT_OF_SCREEN+0
#define ABOUT_TITLE_Y_POS           TOP_OF_SCREEN+LCD_SIZE_ROW
// starting row -- use offsets for the rest
#define ABOUT_LINE_LABEL_X_POS      LEFT_OF_SCREEN+0
#define ABOUT_LINE_LABEL_Y_POS      ABOUT_TITLE_Y_POS+(2*LCD_SIZE_ROW)

#ifdef PLAYER_STRESS
#define STRESS_TEST_X_POS       LEFT_OF_SCREEN
#define STRESS_TEST_Y_POS       TOP_OF_SCREEN+40
#define STRESS_TEST_X_SIZE      SCREEN_WIDTH
#define STRESS_TEST_Y_SIZE      8
#endif


// Delete Menu
#define WARNING_MSG_LINE1_X         LEFT_OF_SCREEN
#define WARNING_MSG_LINE1_Y         (LCD_FIRST_ROW_NUM+1)*LCD_SIZE_ROW
#define WARNING_MSG_LINE2_X         LEFT_OF_SCREEN
#define WARNING_MSG_LINE2_Y         (LCD_FIRST_ROW_NUM+2)*LCD_SIZE_ROW
#define WARNING_MSG_FILE_X          LEFT_OF_SCREEN
#define WARNING_MSG_FILE_Y          (LCD_FIRST_ROW_NUM+4)*LCD_SIZE_ROW
#define WARNING_MSG_YES_X           LEFT_OF_SCREEN
#define WARNING_MSG_YES_Y           (LCD_FIRST_ROW_NUM+6)*LCD_SIZE_ROW
#define DELETE_FAIL_MSG_X           LEFT_OF_SCREEN
#define DELETE_FAIL_MSG_Y           (LCD_FIRST_ROW_NUM+6)*LCD_SIZE_ROW

_reentrant void DisplayClear (void);
_reentrant void DisplayLowBattery (void);

_reentrant int DisplayContrast( int bDisplay, int iIgnored1, void *pPtr);

_reentrant int DisplayBacklight( int bDisplay, int iIgnored1, void *pPtr);
#ifdef WOW
_reentrant int DisplayBar( int bDisplay, int step, void *pPtr);
#endif
extern _X BOOL g_bSongStringScroll;
extern _X INT  g_iSongStringOffset;
extern _X INT  g_iSongStringLength;

extern _X BOOL g_bArtistStringScroll;
extern _X INT  g_iArtistStringOffset;
extern _X INT  g_iArtistStringLength;


extern _X BOOL g_bAlbumStringScroll;
extern _X INT  g_iAlbumStringOffset;
extern _X INT  g_iAlbumStringLength;
extern int g_iVolume_Control_Mode;
extern int g_iAB_Control_Mode;


#endif //_DISPLAY_H
