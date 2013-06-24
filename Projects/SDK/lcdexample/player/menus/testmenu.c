////////////////////////////////////////////////////////////////////////////////
// SigmaTel Inc. Copyright 2003-2005
// Filename:    testmenu.c
// Description: generate record test files
////////////////////////////////////////////////////////////////////////////////

// *************** CHANGE THIS VALUE AND RECOMPILE IF YOU WANT SHORTER/LONGER FILES **************
#define RECORD_TEST_TIME 30   // represents the number of seconds (approximately) per file created
// ***********************************************************************************************

#include "exec.h"                                       
#include "types.h"
#include "menumanager.h"
#include "messages.h"
#include "resource.h"
#include "buttons.h"
#include "testmenu.h"
#include "display.h"
#include "displaylists.h"
#include "menus.h"
#include "mainmenu.h"
#include "const.h"
#include "recordsettingsmenu.h"
#include "voicemenu.h"
#include "filesystem.h"
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

BOOL g_bRecordTest = FALSE;
INT  g_iRecordTestLength = RECORD_TEST_TIME;
extern int g_ADCsource;
extern INT g_iPlaySet;
INT g_iTimeSetting = SEC_FIRST;
extern g_iEncCurrentSeconds; // new line required sinced AJ moved def to sysmem

void _reentrant FillTestSettingsPagedList(int iSource, struct PagedListStructure *pListStructure, struct MenuItem *pItems);
_reentrant void TestSetupItemsPerSource(struct RecorderSettings *pSource,struct MenuItem *pItems); 
void _reentrant CopyTestItems(WORD iItems, struct MenuItem *pItems, struct MenuItem _P *pSource);
void _reentrant FillTesTRecordSettingsHeadingsPagedList(struct PagedListStructure *pListStructure, struct MenuItem *pItems);
   
_P struct MenuItem TestHeadingItems[]={
    { 0,RSRC_STRING_RECORDTEST_MENU           ,0  ,24},
    { 0,RSRC_STRING_ENCODER                   ,0  ,32},
    { 0,RSRC_STRING_DEVICE                    ,0  ,40},
    { 0,RSRC_STRING_DURATION                  ,0  ,48} 
    };

_P struct MenuItem TestSelectionItems[]={
    { 0,RSRC_STRING_RECORD_MENU               ,48  ,24},
    { 0,RSRC_STRING_ENCODER                   ,48  ,32},
    { 0,RSRC_STRING_DEVICE                    ,48  ,40},
    { 0,RSRC_STRING_SPACES                    ,48  ,48} 
    };

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int _reentrant testMenu(int a, int b, int *pPtr)
//
//   Type:          Function (test menu entry point)
//
//   Description:   This function is the enrty point for the record test menu manager.
//
//   Inputs:        None used.
//
//   Outputs:       Menu to enter music menu
//                      
//
//   Notes:         
//////////////////////////////////////////////////////////////////////////////////////////
int _reentrant TestMenu(int a, int b, int *pPtr)
{
    BOOL bDone = FALSE;
    union DisplayHints DisplayHint;
    INT iEvent;
    INT iSource = g_ADCsource;
    INT iSelectedItem = MENU_RECORDTEST;
///////////////////////////////////////////////////////////////////////////
//These data structures are used by the DisplayPagedList function to create the 
//the menu lists.  see displaylists.h for more info
///////////////////////////////////////////////////////////////////////////
    //struct PagedListStructure PagedListInfo;     

    struct PagedListStructure MenuListInfo;     // m_iNormalDisplayMsg,m_iSelectedDisplayMsg
                                                // m_iItems, MenuItem *m_pItems
    struct MenuItem aItems[TESTMENU_COUNT];   // m_iPage, m_iResource, m_ixPos, m_iyPos

    a;b; 
    pPtr = (int *) &gEventInfo; // avoid warnings for unused
//
    //we need to refresh the screen since entering the menu
    DisplayHint.I = (1<<DISPLAY_MISC_BITPOS)|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);

    while(!bDone) 
    {   
        //mask out all display hints that aren't aimed toward the settings menu
        DisplayHint.I &= ((1<<DISPLAY_MISC_BITPOS)|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));

        //wait until a message or button happens.
        iEvent = SysWaitOnEvent(EVENT_MESSAGE|EVENT_BUTTON,&gEventInfo.msg,DisplayHint.I ? 1: -1);

        //handle the event
        switch(iEvent)
        {
        case EVENT_TIMER:
            if(DisplayHint.I)
            {
                SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHint.I,1,0);
                FillTesTRecordSettingsHeadingsPagedList(&MenuListInfo,aItems);
                SysCallFunction(RSRC_DISPLAY_LISTS_CODE_BANK,DisplayPagedList,iSelectedItem,BITMAP_TEST_TITLE_NUM,(int*)&MenuListInfo);
                FillTestSettingsPagedList(iSource,&MenuListInfo,aItems);
                SysCallFunction(RSRC_DISPLAY_LISTS_CODE_BANK,DisplayPagedList,iSelectedItem,BITMAP_TEST_TITLE_NUM,(int*)&MenuListInfo);
                DisplayHint.I = 0;
            }
            break;
        case EVENT_BUTTON:
            // button pressed, so restart timer if enabled
            UpdateAutoShutdownTimer();

                switch(gEventInfo.Button.wButtonEvent)
                {
                case PR_FF:
                case PH_FF:
                    
                    iSelectedItem++;
                    if(iSelectedItem>TESTMENU_LAST)
                        iSelectedItem = TESTMENU_FIRST;
                    DisplayHint.bits.Misc = TRUE;
                    DisplayHint.I = (1<<DISPLAY_MISC_BITPOS)|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
                    break;

                case PR_RW:
                case PH_RW:
                    
                    iSelectedItem--;
                    if(iSelectedItem<TESTMENU_FIRST)
                        iSelectedItem = TESTMENU_LAST;
                    DisplayHint.bits.Misc = TRUE;
                    DisplayHint.I = (1<<DISPLAY_MISC_BITPOS)|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
                    break;

                case PR_MENU:
                    DisplayHint.bits.Misc = TRUE;
                    switch(iSelectedItem)
                    {
                        case MENU_RECORDTEST: 
                    bDone= TRUE;
                    break;

                        case MENU_DEVICE: 
#if (NUM_REMOVABLE_MEDIA > 0)
// for players with external media
                            g_RecorderSettings[g_ADCsource].m_iDestinationDevice++;           //select m_iDestinationDevice
                            if(g_RecorderSettings[g_ADCsource].m_iDestinationDevice>DESTINATION_LAST)
                                g_RecorderSettings[g_ADCsource].m_iDestinationDevice = DESTINATION_FIRST;
#else
                            g_RecorderSettings[iSource].m_iDestinationDevice = DESTINATION_INTERNAL;
#endif
                        DisplayHint.I = (1<<DISPLAY_MISC_BITPOS)|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
                        break;

                        case MENU_ENCODER:
                            g_RecorderSettings[g_ADCsource].m_EncoderNo++;                      //select m_Encoder
                            if(g_RecorderSettings[g_ADCsource].m_EncoderNo > ENCODER_LAST)
                            g_RecorderSettings[g_ADCsource].m_EncoderNo = ENCODER_FIRST;
                            DisplayHint.I = (1<<DISPLAY_MISC_BITPOS)|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
                        break;
                        case MENU_TIMEDURATION: 
                             g_iTimeSetting += 1;             //select time duration
                             if(g_iTimeSetting > SEC_LAST)    
                                  g_iTimeSetting=SEC_FIRST;
                            DisplayHint.I = (1<<DISPLAY_MISC_BITPOS)|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
                            break;
                        default://Not Implemented Yet
                            break;
                    }
                    break;
                case PH_STOP:
                    //if the STOP button is held down, lets call the shutdown menu
                    SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,ShutdownMenu,USERSHUTDOWN,0,0);
                    break;
            }// end button
        } // end switch(iEvent)
    } // end while (bDone)
    
    return iSelectedItem;

}
/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          void _reentrant CopyTestItems(WORD iItems, struct MenuItem *pItems, struct MenuItem _P *pSource)
//
//   Type:          Function (settings for testmenu entry point)
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
void _reentrant CopyTestItems(WORD iItems, struct MenuItem *pItems, struct MenuItem _P *pSource)
{
    WORD i;

    WORD *pPtr = (WORD*)pItems;
    WORD _P *pSrc = (WORD _P *)pSource;

    for(i=0;i<sizeof(struct MenuItem)*iItems;i++)
        *pPtr++ = *pSrc++;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          void _reentrant FillTesTRecordSettingsHeadingsPagedList(struct PagedListStructure *pListStructure, struct MenuItem *pItems)
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
void _reentrant FillTesTRecordSettingsHeadingsPagedList(struct PagedListStructure *pListStructure, struct MenuItem *pItems)
{
    pListStructure->m_iNormalDisplayMsg   =LCD_PRINT_STRING_RSRC;
    pListStructure->m_iSelectedDisplayMsg =LCD_PRINT_STRING_RSRC;
    pListStructure->m_pItems = pItems;
    pListStructure->m_iItems = TESTMENU_COUNT;
    CopyTestItems(TESTMENU_COUNT, pItems, TestHeadingItems);
}


/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          void _reentrant FillTestSettingsPagedList(int iSource, struct PagedListStructure *pListStructure, struct MenuItem *pItems)
//
//   Type:          Function (settings menu entry point)
//
//   Description:   This function is the enrty point for the settings menu manager.
//
//   Inputs:        iSource         Which source we're using (MIC or FM) 
//                  pListStruct     Pointer to the PagedListStructure to be filled.
//                  pItems          Pointer to the list of items to be set up.
//
//   Outputs:       None
//
//   Notes:         This is used to fill out the block of information required to display the menu
//
//<
//////////////////////////////////////////////////////////////////////////////////////////
void _reentrant FillTestSettingsPagedList(int iSource, struct PagedListStructure *pListStructure, struct MenuItem *pItems)
{
    //int i;
    //int y;
    pListStructure->m_iNormalDisplayMsg   =LCD_PRINT_STRING_RSRC;
    pListStructure->m_iSelectedDisplayMsg =LCD_PRINT_STRING_INV_RSRC;
    pListStructure->m_pItems = pItems;
    pListStructure->m_iItems = TESTMENU_COUNT;

    CopyTestItems(TESTMENU_COUNT, pItems, TestSelectionItems);
   iSource = SOURCE_LINEIN;
   TestSetupItemsPerSource(&g_RecorderSettings[iSource],pItems);
   g_ADCsource = iSource;       // Save the source in the global variable.
}





/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant void TestSetupItemsPerSource(struct RecorderSettings *pSource,struct MenuItem *pItems) 
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
_reentrant void TestSetupItemsPerSource(struct RecorderSettings *pSource,struct MenuItem *pItems) 
{
    WORD *pPtr = (WORD*)pItems;

    pPtr+=5;//skip past the first entry, and align with the resource number

    switch(pSource->m_EncoderNo)
    {
        default:
        case ENCODER_ALL:
            *pPtr=  RSRC_STRING_ALL;
            break;
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

    switch(pSource->m_iDestinationDevice)
    {

        case DESTINATION_INTERNAL:
            *pPtr = RSRC_STRING_INTERNAL;
            break;
        case DESTINATION_EXTERNAL:
            *pPtr = RSRC_STRING_EXTERNAL;
            break;
        case DESTINATION_ALL:
            *pPtr = RSRC_STRING_ALL;
            break;

    }        
    pPtr+=sizeof(struct MenuItem);
    switch (g_iTimeSetting)
    {
        case SEC_5:
            *pPtr = RSRC_STRING_TIME5;
            g_iRecordTestLength=5;
            break;
        case SEC_10:     
            *pPtr = RSRC_STRING_TIME10;
            g_iRecordTestLength=10;
            break;
        case SEC_30:
            *pPtr = RSRC_STRING_TIME30;
            g_iRecordTestLength=30;
            break;
        case SEC_60:
            *pPtr = RSRC_STRING_TIME60;
            g_iRecordTestLength=60;
            break;
        case SEC_300:
            *pPtr = RSRC_STRING_TIME300;
            g_iRecordTestLength=300;
            break;
         case SEC_600:
            *pPtr = RSRC_STRING_TIME600;
            g_iRecordTestLength=600;
            break;
        default:
            break;
    }
    pPtr+=sizeof(struct MenuItem);


}


void _reentrant RecordTestMenu(void)
{
    WORD wSaveSampleRate;
    WORD wSaveBits;
    WORD wSaveEncoderNo;
    WORD wSaveChannels;
    WORD wDestinationDevice;
    WORD wsaveBRate;
    WORD wRate;
    INT iSaveRecSource;
    INT iLoopCount1, iLoopCount2, iLoopCount3;
    INT iDestinationDevice = 0;
    INT iInitdevice = 0;
    INT iMaxdevice= 0;
    INT iStart=0;
    INT iEnd=0;

    g_bRecordTest = TRUE;
    //save temp copy of global source
    wSaveChannels =         g_RecorderSettings[g_ADCsource].m_iChannels;
    wSaveSampleRate =       g_RecorderSettings[g_ADCsource].m_iSampleRateInHz;
    wSaveBits =             g_RecorderSettings[g_ADCsource].m_ibits;
    wSaveEncoderNo =        g_RecorderSettings[g_ADCsource].m_EncoderNo;
    wDestinationDevice =    g_RecorderSettings[g_ADCsource].m_iDestinationDevice;
    wsaveBRate =            g_RecorderSettings[g_ADCsource].m_iBitRateInKbps;
    iSaveRecSource =        g_ADCsource;    
    //Handle the selected item in test menu.    
    switch(g_RecorderSettings[g_ADCsource].m_iDestinationDevice)
    {
        case DESTINATION_INTERNAL:
            iInitdevice = 0;
            iMaxdevice = 1;
            break;
        case DESTINATION_EXTERNAL:
            iInitdevice = 1;
            iMaxdevice = 2;
            break;
        case DESTINATION_ALL:
            iInitdevice = 0;
            iMaxdevice = 2;
            break;
    }        
    switch(g_RecorderSettings[g_ADCsource].m_EncoderNo)
    {
        default:
        case ENCODER_ALL:
            iStart= 1;
            iEnd = 5;
            break;
        case ENCODER_IMADPCM:
            iStart= 2;
            iEnd = 3;
            break;
        case ENCODER_MSADPCM:
            iStart= 1;
            iEnd = 2;
            break;
        case ENCODER_PCM:
            iStart= 3;
            iEnd = 4;
            break;
        case ENCODER_MP3:
            iStart= 4;
            iEnd = 5;
        break;
    }

    g_ADCsource = SOURCE_LINEIN;
    SysPostMessage(6,LCD_CLEAR_RANGE,0,0,LCD_SIZE_X,LCD_SIZE_Y);// Clear range

    for (iDestinationDevice = iInitdevice; iDestinationDevice <iMaxdevice; iDestinationDevice++)
    {
        g_RecorderSettings[g_ADCsource].m_iDestinationDevice = iDestinationDevice;
        for ( iLoopCount1 = 1; iLoopCount1 <3; iLoopCount1++)
        {
            g_RecorderSettings[g_ADCsource].m_iChannels = iLoopCount1;//Mono=1, Stereo=2
             for ( iLoopCount2 = iStart; iLoopCount2 <iEnd; iLoopCount2++)
            {
                if ( iLoopCount2 == 1 )
                    g_RecorderSettings[g_ADCsource].m_EncoderNo = ENCODER_MSADPCM;
                else if ( iLoopCount2 == 2 )
                    g_RecorderSettings[g_ADCsource].m_EncoderNo = ENCODER_IMADPCM;
                else if ( iLoopCount2 == 3 )
                    g_RecorderSettings[g_ADCsource].m_EncoderNo = ENCODER_PCM;
                else if ( iLoopCount2 == 4 )
                    g_RecorderSettings[g_ADCsource].m_EncoderNo = ENCODER_MP3;
    
                if (iLoopCount2!=4)
                {
                for ( iLoopCount3 = 1; iLoopCount3 <(2+2*((iLoopCount2==3)? 1: 0)); iLoopCount3++)
                {
                    if  ( iLoopCount2 == 3 )
                        g_RecorderSettings[g_ADCsource].m_ibits = 8*iLoopCount3;//8,16,24 bits for PCM
                    else
                        g_RecorderSettings[g_ADCsource].m_ibits = 4;//4 bits for IM-ADPCM and MS-ADPCM
    
                    g_RecorderSettings[g_ADCsource].m_iSampleRateInHz = 8000;
                    RecordTestFile();
                    g_RecorderSettings[g_ADCsource].m_iSampleRateInHz = 11025;
                    RecordTestFile();
                    g_RecorderSettings[g_ADCsource].m_iSampleRateInHz = 16000;
                    RecordTestFile();
    
                    if (( g_RecorderSettings[g_ADCsource].m_ibits == 24 )&& 
                        ( g_RecorderSettings[g_ADCsource].m_iChannels == 2 ))
                    {;}//Not support now
                    else
                    {
                        g_RecorderSettings[g_ADCsource].m_iSampleRateInHz = 22050;
                        RecordTestFile();
                    }
                    if (( g_RecorderSettings[g_ADCsource].m_ibits == 24 )&& 
                        ( g_RecorderSettings[g_ADCsource].m_iChannels == 2 ))
                    {;}//Not support now
                    else
                    {
                        g_RecorderSettings[g_ADCsource].m_iSampleRateInHz = 32000;
                        RecordTestFile();
                    }
                    if ( g_RecorderSettings[g_ADCsource].m_ibits == 24) 
                    {;}//Not support now
                    else
                    { 
                        g_RecorderSettings[g_ADCsource].m_iSampleRateInHz = 44100;
                        RecordTestFile();
                    }
                    if ( g_RecorderSettings[g_ADCsource].m_ibits == 24 ||
                        (g_RecorderSettings[g_ADCsource].m_ibits == 16 && 
                            ( g_RecorderSettings[g_ADCsource].m_iChannels == 2 )))
                    {;}//Not support now
                    else
                    {
                        g_RecorderSettings[g_ADCsource].m_iSampleRateInHz = 48000;
                        RecordTestFile();
                    }
                }//iLoopCount3
                }//else MP3 encoder format
                else
                {    //set up sample rate as 32khz and bitrate as 96kbps.
                     g_RecorderSettings[g_ADCsource].m_iSampleRateInHz = 32000;
                     while(1)
                     {
                        g_RecorderSettings[g_ADCsource].m_iBitRateInKbps = 96;
                        RecordTestFile();

                            while(1)
                            {
                                WORD wBRate = g_RecorderSettings[g_ADCsource].m_iBitRateInKbps;
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
                                     if((g_RecorderSettings[g_ADCsource].m_iSampleRateInHz==32000) && 
                                     (g_RecorderSettings[g_ADCsource].m_iChannels == CHANNELS_MONO)) 
                                     {
                                      break;
                                     }
                                     else 
                                        wBRate = 256;
                                else if (wBRate==256)
                                wBRate = 320;
                                else if (wBRate==320)
                                break;
                                g_RecorderSettings[g_ADCsource].m_iBitRateInKbps = wBRate;
                                RecordTestFile();
                             } //while loop of bitrate ends
                        wRate = g_RecorderSettings[g_ADCsource].m_iSampleRateInHz;
                        if(wRate==32000)
                        wRate =  44100;
                        else if(wRate==44100)
                        wRate = 48000;
                        else if(wRate==48000)
                        break;
                        g_RecorderSettings[g_ADCsource].m_iSampleRateInHz = wRate;

                     }//whileloop of sample rate ends
                } //else loop ends
            }//iLoopCount2
        }//iLoopCount1
    }//iDestinationDevice
    
    //restore global record source
    g_ADCsource = iSaveRecSource;   
    g_RecorderSettings[g_ADCsource].m_EncoderNo = wSaveEncoderNo;
    g_RecorderSettings[g_ADCsource].m_ibits = wSaveBits;
    g_RecorderSettings[g_ADCsource].m_iSampleRateInHz = wSaveSampleRate;
    g_RecorderSettings[g_ADCsource].m_iChannels = wSaveChannels;
    g_RecorderSettings[g_ADCsource].m_iDestinationDevice = wDestinationDevice;
    g_RecorderSettings[g_ADCsource].m_iBitRateInKbps = wsaveBRate;
    g_bRecordTest = FALSE;
}


// Used these for required P code space reduction. Each of these is called several times. 
// Shrinks testmenu to prevent growth into conflict with semi-static sections in same overlay.
void _reentrant LcdPrintStringRsrc15_0(int rsrc)
{    SysPostMessage(5,LCD_PRINT_STRING_RSRC,15,0,rsrc);
}
//
void _reentrant LcdPrintStringRsrc70_0(int rsrc)
{   SysPostMessage(5,LCD_PRINT_STRING_RSRC,70,0,rsrc);
}
//
void _reentrant LcdPrintStringRsrc60_0(int rsrc)
{   SysPostMessage(5,LCD_PRINT_STRING_RSRC,60,0,rsrc);
}

////////////////////////////////////////////////////////////////////////////////
// Function:    void _reentrant RecordTestFile(void);
// Inputs:      no register inputs
//              globals: g_RecorderSettings g_iPlaySet
// Outputs:     no register outputs
// Description: Record one test file and display encoder format, bitrate, 
//              and samplerate while recording
//
void _reentrant RecordTestFile(void)
{
    INT iEvent;
    INT iCount = 0;
    union DisplayHints DisplayHints;
    BOOL bDone = FALSE;
    WORD wSeparator = DIRECTORY_SEPARATOR;
    BOOL bSendToRecorderStateMachine;
    INT  iDestination = g_RecorderSettings[g_ADCsource].m_iDestinationDevice;
    INT  iEncodertype = g_RecorderSettings[g_ADCsource].m_EncoderNo;
    _packed BYTE RecordDirectory[MAX_FOLDER_SIZE];
    _packed BYTE *pDirPtr;
    _packed BYTE wTempStr[MAX_FOLDER_SIZE];


    SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,ExitPlayerStateMachine,1,0,0); 

    pDirPtr = RecordDirectory;
    pDirPtr[0] = 0;
    //Add Slash
    packed_strcat(pDirPtr,(_packed BYTE*)(&wSeparator));        
    //Load Proper Directory

    ((WORD*)wTempStr)[0]=REC_TEST_0;    //word 0 (3 chars)
    ((WORD*)wTempStr)[1]=REC_TEST_1;      //word 1

    packed_strcat(pDirPtr,(_packed BYTE*)(wTempStr));//add the directory name
    packed_strcat(pDirPtr,(_packed BYTE*)(&wSeparator)); //add the trailing slash

    // Attempt to change to the Voice directory
    if(FSChangeDir(RecordDirectory,iDestination)<0)
    { 
        //couldn't change to the voice directory, lets try to create it.
        SysLoadFATWrite();
        //end slash must be removed so re-initialize RecordDirectory and pointer.  
        pDirPtr = RecordDirectory;
        pDirPtr[0] = 0;
        //Re-build RecordDirectory w/o end slash
        packed_strcat(pDirPtr,(_packed BYTE*)(&wSeparator));
        packed_strcat(pDirPtr,(_packed BYTE*)(wTempStr));//add the directory name

        if(FSCreateDir(RecordDirectory,iDestination)<0)
        {//this verifies the creation did not fail.
            bDone = TRUE;
        }
        SysUnLoadFATWrite();
    }

    if(!bDone)//if bDone is already TRUE, then we can't open the file.
    {
        g_iPlaySet = PLAYSET_VOICE;
        if(iEncodertype != ENCODER_MP3)
        { 
        // Display recording parameters
        // Rate
        if (g_RecorderSettings[g_ADCsource].m_iSampleRateInHz == 8000)
            //SysPostMessage(5,LCD_PRINT_STRING_RSRC,15,0,RSRC_STRING_8000HZ);
            LcdPrintStringRsrc15_0(RSRC_STRING_8000HZ);  
        else if (g_RecorderSettings[g_ADCsource].m_iSampleRateInHz == 11025)
            //SysPostMessage(5,LCD_PRINT_STRING_RSRC,15,0,RSRC_STRING_11025HZ);
            LcdPrintStringRsrc15_0(RSRC_STRING_11025HZ);
        else if (g_RecorderSettings[g_ADCsource].m_iSampleRateInHz == 16000)
            //SysPostMessage(5,LCD_PRINT_STRING_RSRC,15,0,RSRC_STRING_16000HZ);
            LcdPrintStringRsrc15_0(RSRC_STRING_16000HZ);
        else if (g_RecorderSettings[g_ADCsource].m_iSampleRateInHz == 22050)
            //SysPostMessage(5,LCD_PRINT_STRING_RSRC,15,0,RSRC_STRING_22050HZ);
            LcdPrintStringRsrc15_0(RSRC_STRING_22050HZ);
        else if (g_RecorderSettings[g_ADCsource].m_iSampleRateInHz == 32000)
            //SysPostMessage(5,LCD_PRINT_STRING_RSRC,15,0,RSRC_STRING_32000HZ);
            LcdPrintStringRsrc15_0(RSRC_STRING_32000HZ);
        else if (g_RecorderSettings[g_ADCsource].m_iSampleRateInHz == 44100)
            //SysPostMessage(5,LCD_PRINT_STRING_RSRC,15,0,RSRC_STRING_44100HZ);
            LcdPrintStringRsrc15_0(RSRC_STRING_44100HZ);
        else
            //SysPostMessage(5,LCD_PRINT_STRING_RSRC,15,0,RSRC_STRING_48000HZ);
            LcdPrintStringRsrc15_0(RSRC_STRING_48000HZ);
        // Bits
            SysPostMessage(6,LCD_CLEAR_RANGE,60,0,25,8);// Clear range
        if (g_RecorderSettings[g_ADCsource].m_ibits == 4)
            //SysPostMessage(5,LCD_PRINT_STRING_RSRC,70,0,RSRC_STRING_4);
            LcdPrintStringRsrc70_0(RSRC_STRING_4);
        else  if (g_RecorderSettings[g_ADCsource].m_ibits == 8)
            //SysPostMessage(5,LCD_PRINT_STRING_RSRC,70,0,RSRC_STRING_8);
            LcdPrintStringRsrc70_0(RSRC_STRING_8);
        else  if (g_RecorderSettings[g_ADCsource].m_ibits == 16)
            //SysPostMessage(5,LCD_PRINT_STRING_RSRC,70,0,RSRC_STRING_16);
            LcdPrintStringRsrc70_0(RSRC_STRING_16);
        else  if (g_RecorderSettings[g_ADCsource].m_ibits == 24)
            //SysPostMessage(5,LCD_PRINT_STRING_RSRC,70,0,RSRC_STRING_24);
            LcdPrintStringRsrc70_0(RSRC_STRING_24);
        SysPostMessage(5,LCD_PRINT_STRING_RSRC,82,0,RSRC_STRING_BITS);
        // Format
        SysPostMessage(6,LCD_CLEAR_RANGE,0,8,55,8);// Clear range
        if (g_RecorderSettings[g_ADCsource].m_EncoderNo == ENCODER_MSADPCM)
            SysPostMessage(5,LCD_PRINT_STRING_RSRC,0,8,RSRC_STRING_MSADPCM);
        else  if (g_RecorderSettings[g_ADCsource].m_EncoderNo == ENCODER_IMADPCM)
            SysPostMessage(5,LCD_PRINT_STRING_RSRC,0,8,RSRC_STRING_IMADPCM);
        else
            SysPostMessage(5,LCD_PRINT_STRING_RSRC,0,8,RSRC_STRING_PCM);
        }
        else
        {
            // Display recording parameters for Mp3 encoder
            //clear the title
           // SysPostMessage(6,LCD_CLEAR_RANGE,SONG_TITLE_X_POS,SONG_TITLE_Y_POS,SONG_TITLE_X_SIZE,SONG_TITLE_Y_SIZE);
            //Bitrate
            SysPostMessage(6,LCD_CLEAR_RANGE,60,0,38,8);// Clear range
            if(g_RecorderSettings[g_ADCsource].m_iBitRateInKbps == 96)
                //SysPostMessage(5,LCD_PRINT_STRING_RSRC,60,0,RSRC_STRING_96KBPS);    
                LcdPrintStringRsrc60_0(RSRC_STRING_96KBPS); 
            else if(g_RecorderSettings[g_ADCsource].m_iBitRateInKbps == 112)
                //SysPostMessage(5,LCD_PRINT_STRING_RSRC,60,0,RSRC_STRING_112KBPS);
                LcdPrintStringRsrc60_0(RSRC_STRING_112KBPS);
            else if(g_RecorderSettings[g_ADCsource].m_iBitRateInKbps == 128)
                //SysPostMessage(5,LCD_PRINT_STRING_RSRC,60,0,RSRC_STRING_128KBPS);
                LcdPrintStringRsrc60_0(RSRC_STRING_128KBPS);
            else if(g_RecorderSettings[g_ADCsource].m_iBitRateInKbps == 160)
                //SysPostMessage(5,LCD_PRINT_STRING_RSRC,60,0,RSRC_STRING_160KBPS);
                LcdPrintStringRsrc60_0(RSRC_STRING_160KBPS);
            else if(g_RecorderSettings[g_ADCsource].m_iBitRateInKbps == 192)
                //SysPostMessage(5,LCD_PRINT_STRING_RSRC,60,0,RSRC_STRING_192KBPS);
                LcdPrintStringRsrc60_0(RSRC_STRING_96KBPS);
            else if(g_RecorderSettings[g_ADCsource].m_iBitRateInKbps == 224)
                //SysPostMessage(5,LCD_PRINT_STRING_RSRC,60,0,RSRC_STRING_224KBPS);
                LcdPrintStringRsrc60_0(RSRC_STRING_224KBPS);
            else if(g_RecorderSettings[g_ADCsource].m_iBitRateInKbps == 256)
                //SysPostMessage(5,LCD_PRINT_STRING_RSRC,60,0,RSRC_STRING_256KBPS);
                LcdPrintStringRsrc60_0(RSRC_STRING_256KBPS);
            else 
                //SysPostMessage(5,LCD_PRINT_STRING_RSRC,60,0,RSRC_STRING_320KBPS);
                LcdPrintStringRsrc60_0(RSRC_STRING_320KBPS);

             //sample rate
            if (g_RecorderSettings[g_ADCsource].m_iSampleRateInHz == 32000)
                //SysPostMessage(5,LCD_PRINT_STRING_RSRC,15,0,RSRC_STRING_32000HZ);
                LcdPrintStringRsrc15_0(RSRC_STRING_32000HZ);
            else if (g_RecorderSettings[g_ADCsource].m_iSampleRateInHz == 44100)
                //SysPostMessage(5,LCD_PRINT_STRING_RSRC,15,0,RSRC_STRING_44100HZ);
                LcdPrintStringRsrc15_0(RSRC_STRING_44100HZ);
            else
                //SysPostMessage(5,LCD_PRINT_STRING_RSRC,15,0,RSRC_STRING_48000HZ);
                LcdPrintStringRsrc15_0(RSRC_STRING_48000HZ);
            //Format MP3
            SysPostMessage(6,LCD_CLEAR_RANGE,0,8,55,8);// Clear range
            SysPostMessage(5,LCD_PRINT_STRING_RSRC,0,8,RSRC_STRING_MP3);

         
         }
        // Channels
        if (g_RecorderSettings[g_ADCsource].m_iChannels == 1)
            SysPostMessage(5,LCD_PRINT_STRING_RSRC,60,8,RSRC_STRING_MONO);
        else
            SysPostMessage(5,LCD_PRINT_STRING_RSRC,60,8,RSRC_STRING_STEREO);
        // Input resource
        SysPostMessage(5,LCD_PRINT_STRING_RSRC,60,16,RSRC_STRING_LINEIN);
        SysCallFunction(RSRC_RECORDER_STATE_MACHINE_CODE_BANK,InitRecorderStateMachine,0,0,0);
        DisplayHints.I = RECORDTEST_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);


        while(!bDone)
        {       
            DisplayHints.I &= (RECORDTEST_DISPLAY_ALL);
            iEvent = SysWaitOnEvent(EVENT_MESSAGE|EVENT_BUTTON,&gEventInfo.msg,DisplayHints.I ? 1 : -1);
            // Above call returns when we get a message or button event
            
            bSendToRecorderStateMachine = TRUE; // send events to recorder state machine by default
            switch(iEvent)
            {
                case EVENT_TIMER:
                    SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHints.I,TRUE,0);
                    DisplayHints.I= 0; 
                    bSendToRecorderStateMachine = FALSE;

                    if (g_iEncCurrentSeconds > g_iRecordTestLength)  // Record about 10 (default) seconds
                        bDone = TRUE;
                    break;
                        
                case EVENT_BUTTON:
                    // button pressed, so restart timer if enabled
                    UpdateAutoShutdownTimer();

                    //remember the last button event (mostly for coming out of FFWD and RWND)
                    switch(g_wLastButton = gEventInfo.Button.wButtonEvent)
                    {
                        case PR_MENU:
                        case PR_STOP:
                            bDone = TRUE;
                            break;
                        default:
                            break;
                    }
                break;

            } // end switch(iEvent)           
            
            if(bSendToRecorderStateMachine)
            {
                DisplayHints.I |= SysCallFunction(RSRC_RECORDER_STATE_MACHINE_CODE_BANK,HandleRecorderStateMachine,iEvent,0,(int *) &gEventInfo);
            }
        } // end while(!bDone)
        SysCallFunction(RSRC_RECORDER_STATE_MACHINE_CODE_BANK,ExitRecorderStateMachine,0,0,0);
    }
    g_iPlaySet = PLAYSET_MUSIC;
}
