#include "exec.h"
#include "types.h"
#include "menumanager.h"
#include "messages.h"
#include "resource.h"
#include "buttons.h"
#include "wowmenu.h"
#include "display.h"
#include "displaylists.h"
#include "menus.h"
#include "mainmenu.h"
#include "playerstatemachine.h"
#include "const.h"
#include "wow_ed.h"
#include "settingsmenu.h"
#include "sysresourceapi.h"
#include "metadata.h"
extern void DisablePluginModule(int *);
extern void EnablePluginModule(int *);

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int _reentrant WOWMenu(int a, int b, int *pPtr)
//
//   Type:          Function (SRS WOW settings menu entry point)
//
//   Description:   This function is the enrty point for the WOW settings menu manager.
//
//   Inputs:        None
//
//   Outputs:       Menu to enter upon exiting settings menu
//                      if PH_MENU: return to music menu
//                      else : return to main menu selection
//
//   Notes:         
//
//<
//////////////////////////////////////////////////////////////////////////////////////////
int _reentrant WowMenu(int a, int b, int *pPtr)
{
    BOOL bDone = FALSE;
    BOOL bSendToPlayerStateMachine = FALSE;

    union DisplayHints DisplayHint;

    INT iHighLightedItem=WOWMENU_FIRST;
    INT iEvent;
    INT iNextMenu = MENU_WOW;

///////////////////////////////////////////////////////////////////////////
//These data structures are used by the DisplayPagedList function to create the 
//the main menu lists.  see displaylists.h for more info
///////////////////////////////////////////////////////////////////////////

    struct PagedListStructure MenuListInfo;     // m_iNormalDisplayMsg,m_iSelectedDisplayMsg
                                                // m_iItems, MenuItem *m_pItems

    struct MenuItem aItems[WOWMENU_COUNT];      // m_iPage, m_iResource, m_ixPos, m_iyPos


    // filling the structure like this takes more p mem, but it's overlaid -- not Y sys mem
    MenuListInfo.m_iNormalDisplayMsg = LCD_PRINT_STRING_RSRC;
    MenuListInfo.m_iSelectedDisplayMsg = LCD_PRINT_STRING_INV_RSRC;
    MenuListInfo.m_iItems = WOWMENU_COUNT;


    // filling the structure like this takes more p mem, but it's overlaid -- not Y sys mem
    for (a = 0; a < WOWMENU_COUNT; a++)
    {   // m_ipage is 0 for all menu labels
        aItems[a].m_iPage = const_zero;
    }

    // load resource numbers
    aItems[MENU_BRIGHTNESS].m_iResource = RSRC_STRING_WOW_BRIGHTNESS;
    aItems[MENU_TRUBASS].m_iResource = RSRC_STRING_WOW_TRUBASS;
    aItems[MENU_WOWVOLUME].m_iResource = RSRC_STRING_WOW_VOLUME;
    aItems[WOWMENU_EXIT].m_iResource = RSRC_STRING_EXIT_MENU;
    for (a = 0; a < WOWMENU_COUNT; a++)
    {   // m_ixPos is 0 for all menu labels -- start at same X loc
        aItems[a].m_ixPos = const_zero;
    }
    // load Y position  m_iyPos -- each line is 8 pixels lower
    aItems[MENU_BRIGHTNESS].m_iyPos =  const_24;
    aItems[MENU_TRUBASS].m_iyPos =     const_32;
    aItems[MENU_WOWVOLUME].m_iyPos =   const_32+8;
    aItems[WOWMENU_EXIT].m_iyPos =     const_32+16;

    MenuListInfo.m_pItems = aItems;
	//////////////////////////////////////////////////////////////////
	///Load Wow Control code
	//////////////////////////////////////////////////////////////////
    // this is required to get the message into the playerstatemachine
    // casted to avoid pointer mismatch warnings
    pPtr = (int *) &gEventInfo;

    a;b;  // avoid warnings for unused

    //we need to refresh the screen since entering the menu
    DisplayHint.I = SETTINGS_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);

    while(!bDone) 
    {   
        //mask out all display hints that aren't aimed toward the settings menu
        DisplayHint.I &= (SETTINGS_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));

        //wait until a message or button happens.
        iEvent = SysWaitOnEvent(EVENT_MESSAGE|EVENT_BUTTON,&gEventInfo.msg,DisplayHint.I ? 1: -1);

        //assume every message goes to the player state machine
        bSendToPlayerStateMachine = TRUE;


        //handle the event
        switch(iEvent)
        {
        case EVENT_TIMER:
            //no other events left to process, so lets update the display
            if(DisplayHint.I)
                SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHint.I,1,0);
            SysPostMessage(6,LCD_CLEAR_RANGE,0,16,98,5*8);
            SysCallFunction(RSRC_DISPLAY_LISTS_CODE_BANK,DisplayPagedList,iHighLightedItem,BITMAP_SETTINGS_TITLE_NUM,(int*)&MenuListInfo);

            DisplayHint.I = 0;//clear out the bits  
            bSendToPlayerStateMachine = FALSE;//don't need to send this to the player state machine
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
                    //don't want to send these to the player state machine (that would switch songs)
                    bSendToPlayerStateMachine=FALSE;
                    iHighLightedItem++;
                    if(iHighLightedItem>WOWMENU_LAST)
                        iHighLightedItem = WOWMENU_FIRST;
                    DisplayHint.bits.Misc = TRUE;
                    break;

                case PR_RW:
                case PH_RW:
                    //don't want to send these to the player state machine (that would switch songs)
                    bSendToPlayerStateMachine=FALSE;
                    iHighLightedItem--;
                    if(iHighLightedItem<WOWMENU_FIRST)
                        iHighLightedItem = WOWMENU_LAST;
                    DisplayHint.bits.Misc = TRUE;
                    break;

                case PR_MENU:
                    // go to selected submenu
                    switch (iHighLightedItem)
                    {
                    case MENU_BRIGHTNESS:
                        iHighLightedItem=WOWBrightnessMenu();
                        break;
                    case MENU_TRUBASS:
                        WOWControl(TRUBASS);

                        iHighLightedItem=MENU_TRUBASS;
                        break;
                    case MENU_WOWVOLUME:
                        WOWControl(VOLUME);

                        iHighLightedItem=MENU_WOWVOLUME;
                        break;
                    case WOWMENU_EXIT:
                        bDone = TRUE;
                        break;
                    }
                    DisplayHint.I |= SETTINGS_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);

                    if (g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC == TRUE)
                    {   // did fast escape from sub menu ?
                        bDone = TRUE; 
                        iNextMenu = MENU_MUSIC;
                    }
                    break;

                case PH_MENU:
                    // allow user to escape to music mode
                    bDone= TRUE;
                    // set flag so PR_MENU in music mode will ignore
                    g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
                    break;

                case PH_STOP:
                    //if the STOP button is held down, lets call the shutdown menu
                    SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,ShutdownMenu,USERSHUTDOWN,0,0);
                    // when returns update display  -- if doesn't return, powered down
                    DisplayHint.I |= (SETTINGS_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
                    break;
            }
        } // end switch(iEvent)
        if(bSendToPlayerStateMachine)
            DisplayHint.I |= SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,pPtr);
    } // end while (bDone)

    return iNextMenu;
}




int _reentrant WOWBrightnessMenu (void)
{
    
    BOOL bDone = FALSE;
    BOOL bSendToPlayerStateMachine = FALSE;
    int *pPtr;    
    union DisplayHints DisplayHint;

    INT iHighLightedItem=sWowChannel.mBrightness;
    INT iEvent,a;

///////////////////////////////////////////////////////////////////////////
//These data structures are used by the DisplayPagedList function to create the 
//the main menu lists.  see displaylists.h for more info
///////////////////////////////////////////////////////////////////////////

    struct PagedListStructure MenuListInfo;     // m_iNormalDisplayMsg,m_iSelectedDisplayMsg
                                                // m_iItems, MenuItem *m_pItems

    struct MenuItem aItems[WOWBRIGHT_COUNT];

    pPtr = (int *) &gEventInfo;
    iHighLightedItem=sWowChannel.mBrightness;

    // filling the structure like this takes more p mem, but it's overlaid -- not Y sys mem
    MenuListInfo.m_iNormalDisplayMsg = LCD_PRINT_STRING_RSRC;
    MenuListInfo.m_iSelectedDisplayMsg = LCD_PRINT_STRING_INV_RSRC;
    MenuListInfo.m_iItems = WOWBRIGHT_COUNT;

    // filling the structure like this takes more p mem, but it's overlaid -- not Y sys mem
    for (a = 0; a < WOWBRIGHT_COUNT; a++)
    {   // m_ipage is 0 for all menu labels
        aItems[a].m_iPage = const_zero;
    }

    // load resource numbers
    aItems[WOWBRIGHT_LOW].m_iResource = RSRC_STRING_WOW_BRIGHT_LOW;
    aItems[WOWBRIGHT_MID].m_iResource = RSRC_STRING_WOW_BRIGHT_MID;
    aItems[WOWBRIGHT_HIGH].m_iResource = RSRC_STRING_WOW_BRIGHT_HIGH;

    for (a = 0; a < WOWBRIGHT_COUNT; a++)
    {   // m_ixPos is 0 for all menu labels -- start at same X loc
        aItems[a].m_ixPos = const_zero;
    }
    // load Y position  m_iyPos -- each line is 8 pixels lower
    aItems[WOWBRIGHT_LOW].m_iyPos = const_16;
    aItems[WOWBRIGHT_MID].m_iyPos = const_24;
    aItems[WOWBRIGHT_HIGH].m_iyPos = const_32;

    MenuListInfo.m_pItems = aItems;
    //we need to refresh the screen since entering the menu
    DisplayHint.I = SETTINGS_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);

    while(!bDone) 
    {   
        //wait until a message or button happens.
        DisplayHint.I &= (SETTINGS_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
        iEvent = SysWaitOnEvent(EVENT_MESSAGE|EVENT_BUTTON,&gEventInfo.msg,DisplayHint.I ? 1 : -1);

        //handle that EVENT_MESSAGE or EVENT_BUTTON
        bSendToPlayerStateMachine = TRUE;
        switch(iEvent)
        {
        case EVENT_TIMER:
            SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHint.I,1,0);
            SysCallFunction(RSRC_DISPLAY_LISTS_CODE_BANK,DisplayPagedList,iHighLightedItem,0,(int*)&MenuListInfo);
            DisplayHint.I = 0;
            bSendToPlayerStateMachine = FALSE;
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
                    bSendToPlayerStateMachine = FALSE;
                    iHighLightedItem=NextBrightness(INCREMENT);
                    DisplayHint.bits.Misc= TRUE;
                    break;

                case PR_RW:
                case PH_RW:
                    bSendToPlayerStateMachine = FALSE;
                    iHighLightedItem=NextBrightness(DECREMENT);
                    DisplayHint.bits.Misc= TRUE;
                    break;

                case PH_MENU:
                    // allow user to escape to music mode
                    // set flag so PR_MENU in music mode will ignore
                    g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
                    // fall through
                case PR_MENU:
                    bDone= TRUE;
                    bSendToPlayerStateMachine = FALSE;
                    break;

                case PH_STOP:
                    //if the STOP button is held down, lets call the shutdown menu
                    bSendToPlayerStateMachine = FALSE;
                    SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,ShutdownMenu,USERSHUTDOWN,0,0);
                    // when returns update display  -- if doesn't return, powered down
                    DisplayHint.I |= (SETTINGS_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
                    break;
            }
        } // end switch(iEvent)
        if(bSendToPlayerStateMachine)
            DisplayHint.I |= SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,pPtr);
    } // end while (bDone)

    return MENU_BRIGHTNESS;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int _reentrant NextBrightness(int iDirection)
//
//   Type:          Function (NOT code bank callable)
//
//   Description:   This updates the brightness setting and rolls it over if necessary
//
//   Inputs:        none (all parameters are ignored)
//
//   Outputs:       Calls WOW SetBrightness API
//
//   Notes:         
//<
//////////////////////////////////////////////////////////////////////////////////////////
int _reentrant NextBrightness(int iDirection)
{
    g_wWowBrightness = sWowChannel.mBrightness;

    if (iDirection == INCREMENT)
    {
        g_wWowBrightness += 1;
        if(g_wWowBrightness > WOWBRIGHT_LAST)    //check for roll over
            g_wWowBrightness = WOWBRIGHT_FIRST;

    }
    else
    {  
        g_wWowBrightness -= 1;
        if(g_wWowBrightness < WOWBRIGHT_FIRST)   //check for roll over
            g_wWowBrightness = WOWBRIGHT_LAST;
    }
    SysCallFunction(RSRC_WOW_CTRL,SetBrightness,(int)&sWowChannel,(int)g_wWowBrightness,0);

    // need this when change sample rate -- recall an init function 
	wowInit.brightness = g_wWowBrightness;

    return g_wWowBrightness;
} 


/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int _reentrant WOWControl(int type)
//
//   Type:          Function (NOT code bank callable)
//
//   Description:   This updates the brightness setting and rolls it over if necessary
//
//   Inputs:        none (all parameters are ignored)
//
//   Outputs:       Calls WOW SetTruBassControl API
//
//   Notes:         
//<
/////////////////////////////////////////////////////////////////////////////////////////
int _reentrant WOWControl (int type)
{
    
    int DisplayValue;
    int *pPtr;

	union DisplayHints DisplayHint;
	INT  iEvent;
    BOOL bDone = FALSE;
    BOOL bSendToPlayerStateMachine;

    g_wWowTruBass = sWowChannel.mTruBassControl;
    g_wWowVolume = sWowChannel.mWowControl;

    // this is required to get the message into the playerstatemachine
	// casted to avoid pointer mismatch warnings
	pPtr = (int *) &gEventInfo;

    if (type == TRUBASS)
        DisplayValue = g_wWowTruBass/WOWTRUBASS_STEPSIZE;
    else
        DisplayValue = g_wWowVolume/WOWVOLUME_STEPSIZE;
    

    //Lets initially clear screen and display all
    DisplayHint.I = SETTINGS_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);

    while(!bDone)
	{
        
        DisplayHint.I &= (SETTINGS_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
        //wait on either a EVENT_BUTTON or EVENT_MESSAGE
        iEvent = SysWaitOnEvent(EVENT_MESSAGE|EVENT_BUTTON,&gEventInfo.msg,DisplayHint.I ? 1 : -1);        

        //This block is for handling the player state machine related messages.  
        bSendToPlayerStateMachine=TRUE;
		switch(iEvent)
		{
			case EVENT_TIMER:
                SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHint.I,1,0);
                SysCallFunction(RSRC_DISPLAY_CODE_BANK,DisplayBar,TRUE,DisplayValue,0);

                //after displaying, clear all display hints
                DisplayHint.I=0;
                bSendToPlayerStateMachine=FALSE;
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

	            //somebody pressed a button.
	            //remember the last button event (mostly for coming out of FFWD and RWND)		
	            switch(g_wLastButton = gEventInfo.Button.wButtonEvent)
	            {
		            case PH_FF:
		            case PR_FF:
                        bSendToPlayerStateMachine=FALSE;
						DisplayHint.I |= SETTINGS_MENU_DISPLAY_ALL;
                        if (type == TRUBASS)
                        {   
                            if (g_wWowTruBass <= (WOWTRUBASS_MAX-WOWTRUBASS_STEPSIZE))
                                g_wWowTruBass += WOWTRUBASS_STEPSIZE;
							
                            SysCallFunction(RSRC_WOW_CTRL,SetTruBassControl,(int)&sWowChannel,(int)g_wWowTruBass,0);

                            DisplayValue = g_wWowTruBass/WOWTRUBASS_STEPSIZE;
                            // need this when change sample rate -- recall an init function                            
                            wowInit.trubassControl = g_wWowTruBass;
                        }
                        else
                        {
                            if (g_wWowVolume <= (WOWTRUBASS_MAX-WOWVOLUME_STEPSIZE))
                                g_wWowVolume += WOWVOLUME_STEPSIZE;

                            SysCallFunction(RSRC_WOW_CTRL,SetWowControl,(int)&sWowChannel,(int)g_wWowVolume,0);

                            DisplayValue = g_wWowVolume/WOWVOLUME_STEPSIZE;
                            // need this when change sample rate -- recall an init function
                            wowInit.wowControl = g_wWowVolume;
                        }
		                break;
		            case PH_RW:
		            case PR_RW:
                        bSendToPlayerStateMachine=FALSE;
						DisplayHint.I |= SETTINGS_MENU_DISPLAY_ALL;

                        if (type == TRUBASS)
                        {
                            if (g_wWowTruBass >= WOWTRUBASS_STEPSIZE)
                                g_wWowTruBass -= WOWTRUBASS_STEPSIZE;
                            SysCallFunction(RSRC_WOW_CTRL,SetTruBassControl,(int)&sWowChannel,(int)g_wWowTruBass,0);

                            DisplayValue = g_wWowTruBass/WOWTRUBASS_STEPSIZE;
                            // need this when change sample rate -- recall an init function                            
                            wowInit.trubassControl = g_wWowTruBass;
                        }
                        else
                        {
                            if (g_wWowVolume >= WOWVOLUME_STEPSIZE)
                                g_wWowVolume -= WOWVOLUME_STEPSIZE;
                            SysCallFunction(RSRC_WOW_CTRL,SetWowControl,(int)&sWowChannel,(int)g_wWowVolume,0);

                            DisplayValue = g_wWowVolume/WOWVOLUME_STEPSIZE;
                            // need this when change sample rate -- recall an init function
                            wowInit.wowControl = g_wWowVolume;
                        }    
                        break;

                    case PH_STOP:
		                //if the STOP button is held down, lets call the shutdown menu
		                SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,ShutdownMenu,USERSHUTDOWN,0,0);
		                // if returns update display  -- if doesn't return, powered down
		                DisplayHint.I |= (SETTINGS_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS)); 
                        bSendToPlayerStateMachine=FALSE;
		                break;
					case PH_MENU:
						// allow user to escape to music mode
						// set flag so PR_MENU in music mode will ignore
						g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
		            case PR_MENU:
		                //defaults to main menu when exiting.  No need to set a new menu.
		                bDone = TRUE;
                        bSendToPlayerStateMachine=FALSE;
						break;
	            }// end switch (button)
	            break;
		}	// end switch (iEvent)
        if(bSendToPlayerStateMachine)
            DisplayHint.I |= SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,pPtr);

 	} // end while (!bDone)
 	 
    return type;
}

int _reentrant WowInit(int iUnused1,int iUnused2, int *pPtr )
{

	/* Initialize the controls and I/O based on the command line */
	wowInit.numSample = BLOCK_SAMPLES;
	wowInit.wowEnable = g_bWOWEnable;
	wowInit.highBitRate = g_wWowHighBitRate;
	wowInit.brightness = g_wWowBrightness;

	wowInit.speakerSize = g_wWowSpeakerSize;  
   	wowInit.trubassControl = g_wWowTruBass;
    wowInit.wowControl = g_wWowVolume;
   	wowInit.stateBuffer = stateBuffer;

	wowInit.tempBuffer = wow_tempBuffer;

	LoadWOWResource(RSRC_WOW44K_Y,RSRC_TYPE_DATA,
	(_lc_u_e_wow_coeff_Y-_lc_u_b_wow_coeff_Y)*3,
		(int)_lc_u_b_wow_coeff_Y,
			TARGET_MEM_Y);

	SysCallFunction(RSRC_WOW44K,WOW_Init44k,(int)&sWowChannel,(int)&wowInit,0);
    wow_init_done = 1;
	return 0;    
}

//Set properties from the values obtained in the metadata functions
int _reentrant WowSetProperties(int iUnused1, int iUnused2, int *pPtr){
	
	FILE_META_DATA *MetaData;
	MetaData = (FILE_META_DATA *) pPtr;
	wowInit.wowEnable = g_bWOWEnable;

	if ((MetaData->bVBR) || (MetaData->wBitRate < 65536))
    {
        g_wWowHighBitRate = FALSE;
        SysCallFunction(RSRC_WOW_CTRL,SetHighBitRate,(int)&sWowChannel,(int)g_wWowHighBitRate,0);
    }
    else
    {
        g_wWowHighBitRate = TRUE;
        SysCallFunction(RSRC_WOW_CTRL,SetHighBitRate,(int)&sWowChannel,(int)g_wWowHighBitRate,0);
    }
    // need this when change sample rate -- recall an init function
    wowInit.highBitRate = g_wWowHighBitRate;

    /// dynamic adjustment for different input sample rates
    switch (MetaData->wSampRate)
    {
    case SR_48KHZ:  
		LoadWOWResource(RSRC_WOW48K_Y,RSRC_TYPE_DATA,
		(_lc_u_e_wow_coeff_Y-_lc_u_b_wow_coeff_Y)*3,
			(int)_lc_u_b_wow_coeff_Y,
				TARGET_MEM_Y);
        SysCallFunction(RSRC_WOW48K,WOW_Init48k,(int)&sWowChannel,(int)&wowInit,0);
        break;

    case SR_44KHZ:  
    default:
		LoadWOWResource(RSRC_WOW44K_Y,RSRC_TYPE_DATA,
		(_lc_u_e_wow_coeff_Y-_lc_u_b_wow_coeff_Y)*3,
			(int)_lc_u_b_wow_coeff_Y,
				TARGET_MEM_Y);
        SysCallFunction(RSRC_WOW44K,WOW_Init44k,(int)&sWowChannel,(int)&wowInit,0);
        break;

    case SR_32KHZ:  
		LoadWOWResource(RSRC_WOW32K_Y,RSRC_TYPE_DATA,
		(_lc_u_e_wow_coeff_Y-_lc_u_b_wow_coeff_Y)*3,
			(int)_lc_u_b_wow_coeff_Y,
				TARGET_MEM_Y);
        SysCallFunction(RSRC_WOW32K,WOW_Init32k,(int)&sWowChannel,(int)&wowInit,0);
        break;

    case SR_24KHZ:  
		LoadWOWResource(RSRC_WOW24K_Y,RSRC_TYPE_DATA,
		(_lc_u_e_wow_coeff_Y-_lc_u_b_wow_coeff_Y)*3,
			(int)_lc_u_b_wow_coeff_Y,
				TARGET_MEM_Y);
        SysCallFunction(RSRC_WOW24K,WOW_Init24k,(int)&sWowChannel,(int)&wowInit,0);
        break;

    case SR_22KHZ:  
		LoadWOWResource(RSRC_WOW22K_Y,RSRC_TYPE_DATA,
		(_lc_u_e_wow_coeff_Y-_lc_u_b_wow_coeff_Y)*3,
			(int)_lc_u_b_wow_coeff_Y,
				TARGET_MEM_Y);
        SysCallFunction(RSRC_WOW22K,WOW_Init22k,(int)&sWowChannel,(int)&wowInit,0);
        break;
    
    case SR_16KHZ:  
		LoadWOWResource(RSRC_WOW16K_Y,RSRC_TYPE_DATA,
		(_lc_u_e_wow_coeff_Y-_lc_u_b_wow_coeff_Y)*3,
			(int)_lc_u_b_wow_coeff_Y,
				TARGET_MEM_Y);
        SysCallFunction(RSRC_WOW16K,WOW_Init16k,(int)&sWowChannel,(int)&wowInit,0);
        break;

    case SR_12KHZ:  
    case SR_11KHZ:  
		LoadWOWResource(RSRC_WOW11K_Y,RSRC_TYPE_DATA,
		(_lc_u_e_wow_coeff_Y-_lc_u_b_wow_coeff_Y)*3,
			(int)_lc_u_b_wow_coeff_Y,
				TARGET_MEM_Y);
        SysCallFunction(RSRC_WOW11K,WOW_Init11k,(int)&sWowChannel,(int)&wowInit,0);
        break;

    case SR_8KHZ:  
		LoadWOWResource(RSRC_WOW8K_Y,RSRC_TYPE_DATA,
		(_lc_u_e_wow_coeff_Y-_lc_u_b_wow_coeff_Y)*3,
			(int)_lc_u_b_wow_coeff_Y,
				TARGET_MEM_Y);
        SysCallFunction(RSRC_WOW8K,WOW_Init8k,(int)&sWowChannel,(int)&wowInit,0);
        break;
    }

/*	if(g_bWOWEnable)
	{
		EnablePluginModule(&WowModuleBase);
		SysCallFunction(RSRC_WOW_CTRL,SetProcessEnable,(int)&sWowChannel,(int)TRUE,0);
	}
	else
	{
		DisablePluginModule(&WowModuleBase);
		SysCallFunction(RSRC_WOW_CTRL,SetProcessEnable,(int)&sWowChannel,(int)FALSE,0);
	}
*/
	
	return 0;
}
