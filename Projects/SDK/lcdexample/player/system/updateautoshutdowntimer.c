#include "menus.h"
#include "sysmem.h"
#include "messages.h"
#include "exec.h"
#include "playerstatemachine.h"
#include "recorderstatemachine.h"
#include "pwrsettingsmenu.h"

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:         void _reentrant UpdateAutoShutdownTimer (void)
//
//   Type:         Function (system memory)
//
//   Description:  This function updates the autoshutdowntimer if enabled
//
//   Inputs:       none used.
//
//   Outputs:      softtimer message to set or kill timer based on encoder and decoder status.
//
//   Notes:        uses g_wMenuFlags.MENU_FLAG_POWER_DOWN_ENABLED to determine if 
//                 autoshutdown feature is implemented
//<
//////////////////////////////////////////////////////////////////////////////////////////


void _reentrant UpdateAutoShutdownTimer (void)
{
    if (g_MenuFlags.MENU_FLAG_POWER_DOWN_ENABLED)
    {
        // only "kick" the power down timer if enabled & not playing or recording
        if( (g_wDecoderSR & DECODER_STOPPED) && (g_wEncoderSR & ENCODER_STOPPED) )
        {            
            SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_AUTO_SHUTDOWN,
                1,g_wPowerOffTrigger,MENU_AUTO_SHUTDOWN);
        }
        else
        {   // Enc and Dec are not 'both STOPPED', so kill the auto shutdown timer
            SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_AUTO_SHUTDOWN);
        }
    }
}
