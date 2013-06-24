#ifndef _RECORDSETTINGS_MENU_H
#define _RECORDSETTINGS_MENU_H

// menus in settings menu
// defines order of selection
#define RECORDSETTINGSMENU_FIRST        0
#define RECORDSETTINGSMENU_SOURCE       0
#define RECORDSETTINGSMENU_ENCODER      1
#define RECORDSETTINGSMENU_CHANNELS     2
#define RECORDSETTINGSMENU_DESTINATION  3
#define RECORDSETTINGSMENU_SAMPLERATE   4
#define RECORDSETTINGSMENU_BITRATE      5
#define RECORDSETTINGSMENU_MODE         6
#define RECORDSETTINGSMENU_EXIT         7
#define RECORDSETTINGSMENU_LAST         7
#define RECORDSETTINGSMENU_COUNT  (RECORDSETTINGSMENU_LAST+1)

#define SOURCE_FIRST        0
#define SOURCE_MIC          0
#define SOURCE_FM       	1	   
#define SOURCE_LINEIN       2	   //Used as line-in 1 for 144 pin package
#ifdef FMTUNER_ON_LINE2_IN	   
	#define SOURCE_LAST     SOURCE_LINEIN	 	    
#else
	#define SOURCE_LAST     SOURCE_LINEIN //SOURCE_FM		//if using the 100 pin package, skip Line-In
#endif
  
#define SOURCE_COUNT        (SOURCE_LAST+1)

#define ENCODER_FIRST       0
#define ENCODER_IMADPCM     0
#define ENCODER_MSADPCM     1
#define ENCODER_PCM         2
#ifdef TEST
    #ifdef MP3_ENCODE
        #define ENCODER_MP3         3
        #define ENCODER_ALL			4
        #define ENCODER_LAST        4
    #else
        #define ENCODER_MP3         (WORD)-1
        #define ENCODER_LAST        3
        #define ENCODER_ALL			3
    #endif
#else
	#ifdef MP3_ENCODE
		#define ENCODER_MP3         3
		#define ENCODER_LAST        3
	#else
		#define ENCODER_MP3         (WORD)-1
		#define ENCODER_LAST        2
	#endif
#endif
#define IMADPCM             0x11
#define MSADPCM             2
#define WPCM                1
#define ENCODER_COUNT       (ENCODER_LAST+1)

#define MODE_FIRST       0  // this number represents the # channels too
#define MODE_ALBUM       0
#define MODE_SONG        1
#define MODE_LAST        1
#define MODE_COUNT      (MODE_LAST+1)


#define CHANNELS_FIRST      1  // this number represents the # channels too
#define CHANNELS_MONO       1
#define CHANNELS_STEREO     2
#define CHANNELS_LAST       2
#define CHANNELS_COUNT      (CHANNELS_LAST)

#define DESTINATION_FIRST       0
#define DESTINATION_INTERNAL    0
#define DESTINATION_EXTERNAL    1    
#ifdef TEST
	#define DESTINATION_ALL         2 
	#define DESTINATION_LAST        2
	#define DESTINATION_COUNT       (DESTINATION_COUNT+1)
#else
	#define DESTINATION_LAST        1
	#define DESTINATION_COUNT       (DESTINATION_COUNT+1)
#endif

#define SAMPLE_RATE_FIRST     0
#define SAMPLE_RATE_8000HZ    0
#define SAMPLE_RATE_11025HZ   1
#define SAMPLE_RATE_16000HZ   2
#define SAMPLE_RATE_22050HZ   3
#define SAMPLE_RATE_32000HZ   4
#define SAMPLE_RATE_44100HZ   5
#define SAMPLE_RATE_48000HZ   6
#define SAMPLE_RATE_LAST      6
#define SAMPLE_RATE_COUNT     (SAMPLE_RATE_ADPCM_LAST+1)

#define BITRATE_FIRST     1
#define BITRATE_32000HZ   1
#define BITRATE_40000HZ   2
#define BITRATE_48000HZ   3
#define BITRATE_56000HZ   4
#define BITRATE_64000HZ   5
#define BITRATE_80000HZ   6
#define BITRATE_96000HZ   7
#define BITRATE_112000HZ   8
#define BITRATE_128000HZ   9
#define BITRATE_160000HZ   10
#define BITRATE_192000HZ   11
#define BITRATE_224000HZ   12
#define BITRATE_256000HZ   13
#define BITRATE_320000HZ   14
#define BITRATE_LAST      14

#define  BITS_FIRST     0
#define  BITS_4         0    
#define  BITS_8         1
#define  BITS_16        2
#define  BITS_24        3
#define  BITS_LAST      3
#define  BITS_COUNT     (BITS_LAST +1)


extern int g_ADCsource;

//if you change the size of this structure, you MUST change 
//the saverange macro in recordsettingsmenu.c (very bottom).
struct RecorderSettings
{
    WORD m_Encoder;
    WORD m_EncoderNo;
    WORD m_iChannels;
    WORD m_iDestinationDevice;
    WORD m_iSampleRateInHz;
    WORD m_iBitRateInKbps;
	WORD m_iMode;
    WORD m_ibits;
};

extern struct RecorderSettings g_RecorderSettings[];

#endif
