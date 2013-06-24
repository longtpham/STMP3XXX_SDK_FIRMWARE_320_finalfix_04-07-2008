#include "project.h"
#include "playerstatemachine.h"
#include "syscoder.h"
#include "playerlib.h"
#include "font.h"
#include "metadata.h"
#include "battery.h"
#include "sysvolume.h"
#include "filesystem.h"
#include "types.h"
#include "exec.h"
#include "menumanager.h"
#include "messages.h"
#include "resource.h"
#include "buttons.h"
#include "syscoder.h"
#include "sysmem.h"
#include "spectrogram.h"
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
#include "eqmenu.h"

/// ///////////////////////////////////////////////////////////////////////////////////////////
////
//>  Name:          int _reentrant SpectrogramMenu(int a, int b, int *pPtr)
//
//   Type:          Function (user menu)
//
//   Description:   This function is the SPECTROGRAM menu
//
//   Inputs:        none used.
//
//   Outputs:       value of which menu to return to
//
//   Notes:         This function handles the spectrogram menu.  It loops until a reason
//                  to exit is reached (PH_STOP held long enough will return MENU_EXIT, and
//                  PR_MENU will return MENU_MAIN)
//
//                  The basic loop is...
//
//                  while (not done)
//                  {
//                      Update display if necessary
//                      Wait for event
//                      Pass event to player state handler
//                      handle event locally
//                  }
//                  
//                  Sometimes an event may be handled entirely in the player state handler,
//                  sometimes entirely locally, sometimes in both places.
//<
//////////////////////////////////////////////////////////////////////////////////////////
int _reentrant SpectrogramMenu(int a, int b, int *pPtr)
{
    union DisplayHints DisplayHint;
    INT iEvent;
    BOOL bDone = FALSE;
    INT  iNewMenu = MENU_MAIN;
    BOOL bSendToPlayerStateMachine = FALSE;

    int i=0;

     // this is required to get the message into the playerstatemachine
    // casted to avoid pointer mismatch warnings
     pPtr = (int *) &gEventInfo;


    a;b;//here to prevent parameter not used messages.

     //set our current menu
    g_iCurrentMenu = MENU_SPECTROGRAM;
    DisplayHint.I = 0;

    SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK, InitPlayerStateMachine,0,0,0);

    //   Clear the display
    //SysCallFunction(RSRC_DISPLAY_CODE_BANK,DisplayClearDisplay,0,0,0);
    SysPostMessage(6,LCD_CLEAR_RANGE,0,0,LCD_SIZE_X,LCD_SIZE_Y);
    DisplayHint.I = SPECTROGRAM_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);


    
    while(!bDone)
    {
        DisplayHint.I &= (SPECTROGRAM_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));

        iEvent = SysWaitOnEvent(EVENT_MESSAGE|EVENT_BUTTON,&gEventInfo.msg, DisplayHint.I? 1 : -1);

        SysPostMessage(6,LCD_CLEAR_RANGE,0,LCD_SIZE_Y-SPECTRO_RANGE,LCD_SIZE_X,LCD_SIZE_Y);
        // display EVENT_MESSAGE or EVENT_BUTTON
                                                                                                                       			
        SysPostMessage(5, LCD_DISPLAY_HISTOGRAM,SpectralValArray_ch0,NUM_BANDS,LCD_START);
        SysPostMessage(5, LCD_DISPLAY_HISTOGRAM,SpectralValArray_ch1,NUM_BANDS,LCD_START+42);

        bSendToPlayerStateMachine = TRUE;


        switch(iEvent)
        {

        case EVENT_TIMER:
            //no other events left to process, so lets update the display
            if(DisplayHint.I)
                SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHint.I,1,0);
                 DisplayHint.I = 0; //clear out the bits
                 bSendToPlayerStateMachine = FALSE;  
            break;
               
        case EVENT_BUTTON:
             switch(gEventInfo.Button.wButtonEvent)
            {
            case PR_MENU:

                bDone = TRUE;
                bSendToPlayerStateMachine = FALSE;
                break;

            case PH_STOP:
                //if the STOP button is held down, lets call the shutdown menu
                SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,ShutdownMenu,USERSHUTDOWN,0,0);
                // if returns update display  -- if doesn't return, powered down
                DisplayHint.I |= (SPECTROGRAM_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS)); 
                bSendToPlayerStateMachine = FALSE;
                break;
                
            case PH_MENU:
				// Allow user to escape to Music mode
				// Set flag so PR_MENU in Music mode will ignore
				g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
				bDone = TRUE;
				bSendToPlayerStateMachine = FALSE;
				break;

    default:
           break;

            }
             break;
    
        }  

        if(bSendToPlayerStateMachine)
        {    //only send events to the player state machine if necessary (to avoid code bank swapping)
            DisplayHint.I |= SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,pPtr);
             //or the display hints returned with the current collection of hints
        }      
    }
   return iNewMenu;
}





    #pragma asm
        org y,".Xhistogram":
Fhisto
histo    
        dc        $000001
        
Fhistolength
       dc         $0                ; height specified here
       dc        $ffffff
       dc        $ffffff
       dc        $ffffff
       dc        $ffffff
       dc        $ffffff
       dc        $ffffff



    #pragma endasm     


 