// CopyRight (C) SigmaTel, Inc. 2002-2004
// Filename:    recordsettingsmenu.c
// Description: 
//

#include "exec.h"
#include "types.h"
#include "menumanager.h"
#include "messages.h"
#include "resource.h"
#include "buttons.h"
#include "settingsmenu.h"
#include "recordsettingsmenu.h"
#include "display.h"
#include "displaylists.h"
#include "menus.h"
#include "mainmenu.h"
#include "playerstatemachine.h"
#include "const.h"
#include "hwequ.h"
#include "project.h"
#include "sysmem.h"

#define RECORD_SETTINGS_DISPLAY_ALL (1<<DISPLAY_CLEAR_DISPLAY_BITPOS) |\
                                    (1<<DISPLAY_PLAYSTATE_BITPOS)     |\
                                    (1<<DISPLAY_AB_ICON_BITPOS)|\
                                    (1<<DISPLAY_LOCK_BITPOS)|\
                                    (1<<DISPLAY_MISC_BITPOS)|\
                                    (1<<DISPLAY_DISK_BITPOS)|\
                                    (1<<DISPLAY_BATTERY_BITPOS)


_P struct MenuItem HeadingItems[]={
    { 0,RSRC_STRING_SOURCE      ,0  ,24},
    { 0,RSRC_STRING_ENCODER     ,0  ,32},
    { 0,RSRC_STRING_CHANNELS    ,0  ,40},
    { 0,RSRC_STRING_DEVICE      ,0  ,48},
    { 0,RSRC_STRING_SAMPLE_RATE ,0  ,56},
#ifdef S6B33B0A_LCD
    { 0,RSRC_STRING_BIT_RATE    ,0  ,64},
    { 0,RSRC_STRING_MODE        ,0  ,72},
    { 0,RSRC_STRING_SPACES      ,0  ,80}
#endif

#ifdef SED15XX_LCD
    { 1,RSRC_STRING_BIT_RATE    ,0  ,32},
    { 1,RSRC_STRING_MODE        ,0  ,40},
    { 1,RSRC_STRING_SPACES      ,0  ,48}
#endif    
    };

_P struct MenuItem SelectionItems[]={
    { 0,RSRC_STRING_SOURCE      ,48  ,24},
    { 0,RSRC_STRING_ENCODER     ,48  ,32},
    { 0,RSRC_STRING_CHANNELS    ,48  ,40},
    { 0,RSRC_STRING_DEVICE      ,48  ,48},
    { 0,RSRC_STRING_SAMPLE_RATE ,48  ,56},
#ifdef S6B33B0A_LCD
    { 0,RSRC_STRING_BIT_RATE    ,48  ,64},
    { 0,RSRC_STRING_SONG        ,48  ,72},
    { 0,RSRC_STRING_EXIT_MENU   ,48  ,80}
#endif

#ifdef SED15XX_LCD
    { 1,RSRC_STRING_BIT_RATE    ,48  ,32},
    { 1,RSRC_STRING_SONG        ,48  ,40},
    { 1,RSRC_STRING_EXIT_MENU   ,48  ,48}
#endif    
    };

struct RecorderSettings g_RecorderSettings[3]={{RSRC_ENC_ADPCM_MOD_CODE,ENCODER_FIRST,CHANNELS_MONO,DESTINATION_INTERNAL,8000,1,128,MODE_SONG}, //for MIC
                                               {RSRC_ENC_ADPCM_MOD_CODE,ENCODER_FIRST,CHANNELS_STEREO,DESTINATION_INTERNAL,44100,1,128,MODE_SONG},   //for FM
                                               {RSRC_ENC_ADPCM_MOD_CODE,ENCODER_FIRST,CHANNELS_STEREO,DESTINATION_INTERNAL,44100,1,128,MODE_SONG}}; //for Line-in
// controls which setting is shown first in menu
int g_iSourceSetting = SOURCE_MIC;

// default source
int g_ADCsource=SOURCE_MIC;
int g_ADCNumChannels = CHANNELS_MONO;

extern g_wEncoderMode;
// only support 16 bit PCM
WORD g_PCMBits = 16;

#ifdef CLCD
int prev_x_pos[RECORDSETTINGSMENU_COUNT];   // for settingsmenu displaylist
#endif

/*
This menu is set up to provide the user a method to choose the different settings associated with the recording functionality.


The general makeup of the screen will be as follows:

  /----------------------
  /
  /       RECORD SETTINGS
  /
  / Source:         MIC/FM/LINEIN
  / Encoder:        ADPCM
  / Channels:       MONO/STEREO
  / Destination:    INTERNAL/EXTERNAL
  / Sample Rate:    4 kHz/8 kHz/16kHz
  /----------------------

  Each option will be a seperate paged list:
    SourceListInfo
    EncoderListInfo
    ChannelListInfo
    DestinationListInfo
    SampleRateListInfo

  Encoder, Channel, Destination, SampleRate will all be settings for each source option 
  (allowing the user to set the settings for both FM recording and MIC recording)

  Because of this, when the source changes, the paged lists for the other options will have to be
  modified for the current settings for that source.

  Also, when the encoder is changed, it is possible that the available sample rates will change, or that mono/stereo 
  is not available, so these options will have to be re-initialized.

  This menu currently only supports changing from MIC to FM, changing the destination from internal to external, and 
  modifying the sample rate.

*/


/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          void _reentrant CopyItems(WORD iItems, struct MenuItem *pItems, struct MenuItem _P *pSource)
//
//   Type:          Function (settings menu entry point)
//
//   Description:   This copies a list of items from P memory (overlayed) to Y memory (where it is used by the displaylists functions)
//
//   Inputs:        iItems          Number of items to copy
//                  pListStruct     Pointer to the PagedListStructure to be filled.
//                  pItems          Pointer to the list of items to be set up.
//
//   Outputs:       None
//
//   Notes:         This is used to fill out the block of information required to display the menu
//
//<
//////////////////////////////////////////////////////////////////////////////////////////
void _reentrant CopyItems(WORD iItems, struct MenuItem *pItems, struct MenuItem _P *pSource)
{
    WORD i;

    WORD *pPtr = (WORD*)pItems;
    WORD _P *pSrc = (WORD _P *)pSource;

    for(i=0;i<sizeof(struct MenuItem)*iItems;i++)
        *pPtr++ = *pSrc++;
}


/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          void _reentrant FillRecordSettingsPagedList(struct PagedListStructure *pListStructure, struct MenuItem *pItems)
//
//   Type:          Function (settings menu entry point)
//
//   Description:   This function is used to initialize the paged list structure for the headings column
//

//   Inputs:        pListStruct     Pointer to the PagedListStructure to be filled.
//                  pItems          Pointer to the list of items to be set up.
//
//   Outputs:       None
//
//   Notes:         This is used to fill out the block of information required to display the menu
//
//<
//////////////////////////////////////////////////////////////////////////////////////////
void _reentrant FillRecordSettingsHeadingsPagedList(struct PagedListStructure *pListStructure, struct MenuItem *pItems)
{
    pListStructure->m_iNormalDisplayMsg   =LCD_PRINT_STRING_RSRC;
    pListStructure->m_iSelectedDisplayMsg =LCD_PRINT_STRING_RSRC;
    pListStructure->m_pItems = pItems;
    pListStructure->m_iItems = RECORDSETTINGSMENU_COUNT;
    CopyItems(RECORDSETTINGSMENU_COUNT, pItems, HeadingItems);
}



/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant void SetupItemsPerSource(struct RecorderSettings *pSource,struct MenuItem *pItems) 
//
//   Type:          Function (settings menu entry point)
//
//   Description:   This function is a helper function for FillRecordSettingsPagedList(), it sets up 
//                  the values in the item list based on what is stored in the RecorderSettings structure
//
//   Inputs:        pSource         Pointer to the Recorder settings structure to use
//                  pItems          Pointer to the list of items to be set up.
//
//   Outputs:       None
//
//   Notes:         
//
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant void SetupItemsPerSource(struct RecorderSettings *pSource,struct MenuItem *pItems) 
{
    WORD *pPtr = (WORD*)pItems;

    pPtr+=5;//skip past the first entry, and align with the resource number

    switch(pSource->m_EncoderNo)
    {
        default:
        case ENCODER_IMADPCM:
            *pPtr=  RSRC_STRING_IMADPCM;
            break;
        case ENCODER_MSADPCM:
           *pPtr=  RSRC_STRING_MSADPCM;
            break;
        case ENCODER_PCM:
           *pPtr=  RSRC_STRING_PCM;
            break;
        case ENCODER_MP3:
            *pPtr=  RSRC_STRING_MP3;
        break;
    }
    pPtr+=sizeof(struct MenuItem);

    switch(pSource->m_iChannels)
    {
        case CHANNELS_MONO:
            *pPtr = RSRC_STRING_MONO;
            break;
        case CHANNELS_STEREO:
            *pPtr = RSRC_STRING_STEREO;
            break;
    }
    pPtr+=sizeof(struct MenuItem);

    switch(pSource->m_iDestinationDevice)
    {
        case DESTINATION_INTERNAL:
            *pPtr = RSRC_STRING_INTERNAL;
            break;
        case DESTINATION_EXTERNAL:
            *pPtr = RSRC_STRING_EXTERNAL;
            break;
    }
    pPtr+=sizeof(struct MenuItem);


    switch(pSource->m_iSampleRateInHz)
    {
        case 8000:
            *pPtr = RSRC_STRING_8000HZ;
            break;
        case 11025:
            *pPtr = RSRC_STRING_11025HZ;
            break;
        case 16000:
            *pPtr = RSRC_STRING_16000HZ;
            break;
        case 22050:
            *pPtr = RSRC_STRING_22050HZ;
            break;
        case 32000:
            *pPtr = RSRC_STRING_32000HZ;
            break;
        case 44100:
            *pPtr = RSRC_STRING_44100HZ;
            break;
        case 48000:
            *pPtr = RSRC_STRING_48000HZ;
            break;
    }
    pPtr+=sizeof(struct MenuItem);

    switch(pSource->m_iBitRateInKbps)
    {
        case 96:
            *pPtr = RSRC_STRING_96KBPS;
            break;
        case 112:
            *pPtr = RSRC_STRING_112KBPS;
            break;
        case 128:
            *pPtr = RSRC_STRING_128KBPS;
            break;
        case 160:
            *pPtr = RSRC_STRING_160KBPS;
            break;
        case 192:
            *pPtr = RSRC_STRING_192KBPS;
            break;
        case 224:
            *pPtr = RSRC_STRING_224KBPS;
            break;
        case 256:
            *pPtr = RSRC_STRING_256KBPS;
            break;
        case 320:
            *pPtr = RSRC_STRING_320KBPS;
            break;
        default:
            *pPtr = RSRC_STRING_HZ;
            break;
        
    }

    pPtr+=sizeof(struct MenuItem);

    switch(pSource->m_iMode)
    {
        case MODE_ALBUM:
            *pPtr = RSRC_STRING_ALBUM;
            break;
        case MODE_SONG:
            *pPtr = RSRC_STRING_SONG;
            break;
    }
    pPtr+=sizeof(struct MenuItem);


    *pPtr = RSRC_STRING_EXIT_MENU;

}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          void _reentrant FillRecordSettingsPagedList(int iSource, struct PagedListStructure *pListStructure, struct MenuItem *pItems)
//
//   Type:          Function (settings menu entry point)
//
//   Description:   This function is the enrty point for the settings menu manager.
//
//   Inputs:        iSource         Which source we're using (MIC/FM/LINEIN) 
//                  pListStruct     Pointer to the PagedListStructure to be filled.
//                  pItems          Pointer to the list of items to be set up.
//
//   Outputs:       None
//
//   Notes:         This is used to fill out the block of information required to display the menu
//
//<
//////////////////////////////////////////////////////////////////////////////////////////
void _reentrant FillRecordSettingsPagedList(int iSource, struct PagedListStructure *pListStructure, struct MenuItem *pItems)
{
    int i;
    int y;
    pListStructure->m_iNormalDisplayMsg   =LCD_PRINT_STRING_RSRC;
    pListStructure->m_iSelectedDisplayMsg =LCD_PRINT_STRING_INV_RSRC;
    pListStructure->m_pItems = pItems;
    pListStructure->m_iItems = RECORDSETTINGSMENU_COUNT;

    CopyItems(RECORDSETTINGSMENU_COUNT, pItems, SelectionItems);

    if(iSource == SOURCE_MIC)
        pItems[0].m_iResource=  RSRC_STRING_MICROPHONE;        
    else if(iSource == SOURCE_LINEIN)
        pItems[0].m_iResource=  RSRC_STRING_LINEIN;
    else
        pItems[0].m_iResource=  RSRC_STRING_FM;

    SetupItemsPerSource(&g_RecorderSettings[iSource],pItems);
   g_ADCsource = iSource;       // Save the source in the global variable.
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int _reentrant RecordSettingsMenu(int a, int b, int *pPtr)
//
//   Type:          Function (settings menu entry point)
//
//   Description:   This function is the enrty point for the record settings menu manager.
//
//   Inputs:        None
//
//   Outputs:       
//
//   Notes:         
//
//<
//////////////////////////////////////////////////////////////////////////////////////////

int _reentrant RecordSettingsMenu(int a, int b, int *pPtr)
{
    BOOL bDone = FALSE;
    BOOL bDisplayTitle = TRUE;
    BOOL bSendToPlayerStateMachine = FALSE;

    union DisplayHints DisplayHint;
    INT iSelectedItem=RECORDSETTINGSMENU_SOURCE;
    INT iEvent;
    INT iSource = g_ADCsource;
    INT iNextMenu = MENU_MAIN;  // default return (main menu)

///////////////////////////////////////////////////////////////////////////
//These data structures are used by the DisplayPagedList function to create the 
//the main menu lists.  see displaylists.h for more info
///////////////////////////////////////////////////////////////////////////

    struct PagedListStructure PagedListInfo;     
    
//    Make these as large as the largest MenuItem list you need.  We're going to re-use it for each sub list to save memory
    struct MenuItem Items[8];
#ifdef USE_PLAYLIST3
    struct MenuItem SelectionItems[8];
#endif //#ifdef USE_PLAYLIST3
    
    pPtr = (int *) &gEventInfo;

    a;b;  // avoid warnings for unused

    //we need to refresh the screen since entering the menu
    DisplayHint.I = RECORD_SETTINGS_DISPLAY_ALL;

#ifdef CLCD
    // initialized the x_pos array.
    for(iSelectedItem = 0; iSelectedItem < RECORDSETTINGSMENU_COUNT; iSelectedItem++)
        prev_x_pos[iSelectedItem] = 0;
    iSelectedItem = RECORDSETTINGSMENU_SOURCE; // restore the init value
#endif
    while(!bDone) 
    {           
        //mask out any hints that got added that aren't on our screen
        DisplayHint.I &= (RECORD_SETTINGS_DISPLAY_ALL);

        //wait until a message or button happens.
        iEvent = SysWaitOnEvent(EVENT_MESSAGE|EVENT_BUTTON,&gEventInfo.msg, DisplayHint.I ? 1 : -1 );

        bSendToPlayerStateMachine=TRUE;

        //handle that EVENT_MESSAGE or EVENT_BUTTON
        switch(iEvent)
        {
        case EVENT_TIMER:
            //we processed all the events, and got a timeout so time to update the display if necessary
            if(DisplayHint.I)
            {
                SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay, DisplayHint.I,1,0);
            }
#ifndef CLCD
            SysPostMessage(6,LCD_CLEAR_RANGE,0,24,98,40);
#endif	        	
            FillRecordSettingsHeadingsPagedList(&PagedListInfo,Items);
            if(bDisplayTitle == TRUE)
            {   SysCallFunction(RSRC_DISPLAY_LISTS_CODE_BANK,DisplayPagedList,iSelectedItem,BITMAP_SETTINGS_TITLE_NUM,(int*)&PagedListInfo);
                bDisplayTitle = FALSE;
            }else
            {   SysCallFunction(RSRC_DISPLAY_LISTS_CODE_BANK,DisplayPagedList,iSelectedItem,0,(int*)&PagedListInfo);
            }
#ifdef USE_PLAYLIST3
            FillRecordSettingsPagedList(iSource,&PagedListInfo,SelectionItems);
#else //#ifdef USE_PLAYLIST3
            FillRecordSettingsPagedList(iSource,&PagedListInfo,Items);
#endif //#ifdef USE_PLAYLIST3
            SysCallFunction(RSRC_DISPLAY_LISTS_CODE_BANK,DisplayPagedList,iSelectedItem,BITMAP_RECORD_SETTINGS_TITLE_NUM,(int*)&PagedListInfo);
            DisplayHint.I = 0;
            break;
        case EVENT_MESSAGE:
            switch(gEventInfo.Message.wMsgCommand)
            {
            }
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
 
                case PR_FF:
                case PH_FF:
                    DisplayHint.bits.Misc = TRUE;
                    bSendToPlayerStateMachine = FALSE;//don't want to skip to next song or fast forward
                    iSelectedItem++;
                    if(iSelectedItem>RECORDSETTINGSMENU_LAST)
                        iSelectedItem = RECORDSETTINGSMENU_FIRST;
                    break;

                case PR_RW:
                case PH_RW:
                    DisplayHint.bits.Misc = TRUE;
                    bSendToPlayerStateMachine = FALSE;//don't want to skip to prev song or rewind
                    iSelectedItem--;
                    if(iSelectedItem<RECORDSETTINGSMENU_FIRST)
                        iSelectedItem = RECORDSETTINGSMENU_LAST;
                    break;

                case PR_MENU:
                    DisplayHint.bits.Misc = TRUE;
                    switch(iSelectedItem)
                    {
                    case RECORDSETTINGSMENU_DESTINATION:
#ifdef TUNER_STFM1000
                        if (iSource == SOURCE_FM)
                        {
                            g_RecorderSettings[iSource].m_iDestinationDevice = DESTINATION_INTERNAL;
                        }
                        else
#endif
                        {
#if (NUM_REMOVABLE_MEDIA > 0)
// for players with external media
                            g_RecorderSettings[iSource].m_iDestinationDevice++;
                            if(g_RecorderSettings[iSource].m_iDestinationDevice>DESTINATION_LAST)
                                g_RecorderSettings[iSource].m_iDestinationDevice = DESTINATION_FIRST;
#else
                            g_RecorderSettings[iSource].m_iDestinationDevice = DESTINATION_INTERNAL;
#endif
                        }
                        break;

                    case RECORDSETTINGSMENU_SAMPLERATE:
#ifdef TUNER_STFM1000
                        if (iSource == SOURCE_FM)
                        {
                            // force FM to 44.1 KHz
                            g_RecorderSettings[iSource].m_iSampleRateInHz = 44100;
                            break;
                        }
#endif
                        if (g_RecorderSettings[iSource].m_EncoderNo != ENCODER_MP3)
                        {
                            WORD wRate = g_RecorderSettings[iSource].m_iSampleRateInHz;
                            if(wRate==8000)
                                wRate = 11025;
                            else if (wRate==11025)
                                wRate = 16000;
                            else if (wRate==16000)
                                wRate = 22050;
                            else if (wRate==22050)
                                wRate = 32000;
                            else if (wRate==32000)
// Set line below to 1 to allow 44.1/48kHz voice record. This is the only change needed to reenable it.
#if 0 
                                wRate = 8000;
#else
//////////////////////////////// WARNING !!! //////////////////////////////////
// The following code was removed due to poor
// record quality at those rates (dropouts).
//////////////////////////////// WARNING !!! //////////////////////////////////
                                wRate = 44100;
                            else if (wRate==44100) // if 44.1kS pcm stereo force low rate
                                if((g_RecorderSettings[iSource].m_EncoderNo == ENCODER_PCM)
                                                &&
                                    (g_RecorderSettings[iSource].m_iChannels == CHANNELS_STEREO))
                                {       
                                        wRate = 8000;       //fix for defect 3719. Think of a better approach than this.
                                }
                                else  wRate = 48000;
                              
                            else 
                                wRate = 8000;               // default sample rate
#endif                                
                            g_RecorderSettings[iSource].m_iSampleRateInHz = wRate;
                        }
                        else { // ENCODER_MP3
                           
                             WORD wRate = g_RecorderSettings[iSource].m_iSampleRateInHz;
	                             if(wRate==32000)
	                                wRate =  44100;
	                             else if(wRate==44100)
	                                wRate = 48000;
	                             else if(wRate==48000)
							          if((g_RecorderSettings[iSource].m_iChannels == CHANNELS_MONO) &&
							                 (g_RecorderSettings[iSource].m_iBitRateInKbps > 224))
									  {
									  wRate = 44100;
									  }
									  else
	                                  wRate = 32000;
                            g_RecorderSettings[iSource].m_iSampleRateInHz = wRate;
                        }
                        break;

                    case RECORDSETTINGSMENU_BITRATE:
                        if (g_RecorderSettings[iSource].m_EncoderNo == ENCODER_MP3)
                        {
                             WORD wBRate = g_RecorderSettings[iSource].m_iBitRateInKbps;
                             if(wBRate==96)
                                wBRate = 112;
                             else if (wBRate==112)
                                wBRate = 128;
                             else if (wBRate==128)
                                wBRate = 160;
                             else if (wBRate==160)
                                wBRate = 192;
                             else if (wBRate==192)
                                wBRate = 224;
                             else if (wBRate==224)
						         if((g_RecorderSettings[iSource].m_iSampleRateInHz==32000) && 
						         (g_RecorderSettings[iSource].m_iChannels == CHANNELS_MONO)) 
								 {
                                	wBRate = 96;
								 }
								 else // allows 256kbps if was not (32khz mono)
#if 0                                 // Nov4 '04 disable these due to flash filewrite latency overflows. MLC & some type2 flash products can enable this.
									wBRate = 256;
                             else if (wBRate==256)
                                wBRate = 320;
                             else if (wBRate==320)
#endif
                                    wBRate = 96;

                             g_RecorderSettings[iSource].m_iBitRateInKbps = wBRate;
                        }
                        break;

                    case RECORDSETTINGSMENU_CHANNELS:
#ifdef TUNER_STFM1000
                        if (iSource == SOURCE_FM)
                        {
                            // force FM to stereo channels
                            g_RecorderSettings[iSource].m_iChannels = CHANNELS_STEREO;
                            break;
                        }
                        else 
#endif
                        if((iSource!=SOURCE_MIC)) {
                            if((g_RecorderSettings[iSource].m_EncoderNo == ENCODER_PCM)
                                            &&
                                (g_RecorderSettings[iSource].m_iSampleRateInHz == 48000))
                            {       
                                    g_RecorderSettings[iSource].m_iChannels = CHANNELS_MONO;
                                                //fix for defect 3719. Think of a better approach than this.
                            }
							else if((g_RecorderSettings[iSource].m_EncoderNo == ENCODER_MP3) &&
									(g_RecorderSettings[iSource].m_iSampleRateInHz == 32000) &&
									(g_RecorderSettings[iSource].m_iBitRateInKbps >224))
									{
										g_RecorderSettings[iSource].m_iChannels = CHANNELS_STEREO;
									}
                            else
                            {
								
                                g_RecorderSettings[iSource].m_iChannels++;
                                if(g_RecorderSettings[iSource].m_iChannels>CHANNELS_LAST)
                                    g_RecorderSettings[iSource].m_iChannels = CHANNELS_FIRST;
                            }
                        
                        }        
                        else {    
                            // Mic can only handle a single channel
                            g_RecorderSettings[iSource].m_iChannels = CHANNELS_MONO;
                        }
                        break;

                     case RECORDSETTINGSMENU_ENCODER:
                        g_RecorderSettings[iSource].m_EncoderNo++;                      //m_Encoder

#ifdef TUNER_STFM1000
                        if((iSource==SOURCE_FM) && (g_RecorderSettings[iSource].m_EncoderNo >= ENCODER_PCM))
                        {
                            // STFM1000 will not encode MP3 or PCM, advance past PCM and MP3                        
                            g_RecorderSettings[iSource].m_EncoderNo = ENCODER_FIRST;
                        }    
#endif
                        if(g_RecorderSettings[iSource].m_EncoderNo > ENCODER_LAST)
                            g_RecorderSettings[iSource].m_EncoderNo = ENCODER_FIRST;

                        if((g_RecorderSettings[iSource].m_EncoderNo==ENCODER_IMADPCM) ||(g_RecorderSettings[iSource].m_EncoderNo== ENCODER_MSADPCM))
						{
#ifdef TUNER_STFM1000
                            if(iSource == SOURCE_FM)
                            {
							    g_RecorderSettings[iSource].m_iSampleRateInHz = 44100;
						    }
                            else
#endif
                            {
                                g_RecorderSettings[iSource].m_ibits = 4;
							    g_RecorderSettings[iSource].m_iSampleRateInHz = 8000;
                            }
                        }
                        else if (g_RecorderSettings[iSource].m_EncoderNo==ENCODER_PCM) // PCM
                            {
                                g_RecorderSettings[iSource].m_ibits = g_PCMBits;
                                //if settings.dat has saved sample rate and NumChannels for adpcm earlier 
                                //it might conflict with non-supported PCM types. So take care of this here.
                                //if stereo and 48000 both are selected then just change sample rate to 44100.
                                //workaround for defect #3719
                                if( (g_RecorderSettings[iSource].m_iSampleRateInHz == 48000)
                                                    &&
                                    (g_RecorderSettings[iSource].m_iChannels == CHANNELS_STEREO))
                                    
                                    g_RecorderSettings[iSource].m_iSampleRateInHz = 44100;
                        
                            }
                        

                        else if (g_RecorderSettings[iSource].m_EncoderNo==ENCODER_MP3) // MP3 sample rate
                            {
                                g_RecorderSettings[iSource].m_iSampleRateInHz = 32000;
                                g_RecorderSettings[iSource].m_iBitRateInKbps    = 96;
                             }

                        if (g_RecorderSettings[iSource].m_EncoderNo != ENCODER_MP3)
                                g_RecorderSettings[iSource].m_iBitRateInKbps = '-';

                        break;             
             
                    case RECORDSETTINGSMENU_SOURCE:
                        iSource++;
#ifdef REMOVE_FM
                        if(iSource == SOURCE_FM) iSource++;
#endif                        
                        if(iSource > SOURCE_LAST)
                            iSource = SOURCE_FIRST;
                        
#ifdef TUNER_STFM1000
                        // STFM1000 remove MP3 encoding from FM
                        if((iSource==SOURCE_FM) && (g_RecorderSettings[iSource].m_EncoderNo >= ENCODER_PCM))
                        {
                            g_RecorderSettings[iSource].m_EncoderNo = ENCODER_FIRST;
                            g_RecorderSettings[iSource].m_iDestinationDevice = DESTINATION_INTERNAL;
                        }
#endif
                        
                        break;
                        
				    case RECORDSETTINGSMENU_MODE:
						g_RecorderSettings[iSource].m_iMode++;
						if(g_RecorderSettings[iSource].m_iMode > MODE_LAST)
							g_RecorderSettings[iSource].m_iMode = MODE_FIRST;

						if(g_RecorderSettings[iSource].m_iMode == MODE_ALBUM)
						  	g_wEncoderMode =1;
						else
						  	g_wEncoderMode =0;						
				     	break;

                    case RECORDSETTINGSMENU_EXIT:
                        bDone = TRUE;
                        break;
                        
                    default://Not Implemented Yet
                        break;
                    }
                    break;

				case PR_STOP:
					if((g_wDecoderSR & DECODER_STOPPED) != 0)
						bSendToPlayerStateMachine = FALSE;
					else
						DisplayHint.I |= (RECORD_SETTINGS_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
					break;
            
				case PH_STOP:
					//if the STOP button is held down, lets call the shutdown menu
					SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,ShutdownMenu,USERSHUTDOWN,0,0);
					// when returns update display  -- if doesn't return, powered down
					DisplayHint.I |= (RECORD_SETTINGS_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
					break;

                case PH_MENU:
                    // allow user to escape to music mode
                    bDone= TRUE;
                    // set flag so PR_MENU in music mode will ignore
                    g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
                    break;
            }
        } // end switch(iEvent)
        if(bSendToPlayerStateMachine)
            DisplayHint.I |= SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,pPtr);

    } // end while (bDone)

    g_iSourceSetting=iSource;

    return MENU_RECORD_SETTINGS;

}

// Use SaveRange macro to save a copy of global variable addresses as well as
// reserve space for their values in section "Player_settings_Y".  All of which
// will be used at shutdown to save user settings in the Settings.dat file.
// Defect 5750 corrected by saving g_wEncoderMode
#pragma asm
    nolist
        include "resource.inc"
        include "sysmacro.asm"
    list
    org y,"Player_settings_Y":
    extern y:Fg_RecorderSettings
    extern y:Fg_ADCsource
        ;The below hard coded number is based on the structure size, and how many elements.
        ;This will have to change if the structure size or elements changes
    SaveRange Fg_RecorderSettings,8*3
    SaveRange Fg_ADCsource,1
    SaveRange Fg_wEncoderMode,1
#pragma endasm
 

