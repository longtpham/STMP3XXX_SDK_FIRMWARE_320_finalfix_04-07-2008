////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2003
//
// Filename:    displaytask_led.c
// Description: LED display for USBMSC.
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//   Includes and external references
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "kernel.h"
#include "messages.h"
#include "regsgpio.h"
#include "display_usb_led.h"


////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
INT   g_bToggle=0, g_iOnTime=0, g_iOffTime=0,g_bEnableHeartBeat=0;
INT   g_iTimerTick;

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          DisplayTask
//
//   Type:          Function
//
//   Description:   LED is either blinking, on or off.
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant DisplayTask(void)
{
    Message  Msg;
    unsigned long    lTimeout = -1;
    INT              error;
    WORD             wReturnCode;
    int i;
    INT Param1,Param2,Param3,Param4,Param5;//these are here as code optimizations

    USBLedInit();
    while(1)
    {
        wReturnCode = SysWaitOnMessage(-1,&Msg);

        if (EVENT_MESSAGE == wReturnCode)
        {
            Param1 = Msg.m_wParams[0];
            Param2 = Msg.m_wParams[1];
            Param3 = Msg.m_wParams[2];    
            Param4 = Msg.m_wParams[3];    
            Param5 = Msg.m_wParams[4];    
    
            switch(Msg.m_wMessage)
            {
            case LED_RESET:
                g_iOnTime = 0;
                g_iOffTime = -1;
                USBLedState(1);       // Turn LED on ON.
                g_bEnableHeartBeat = FALSE;
                break;
            case LED_CONTROL:
                g_iOnTime = Param2;
                g_iOffTime = Param3;
                g_iTimerTick = g_iOffTime;
                g_bToggle = FALSE;
                USBLedState(0);       // Turn LED off - pulsed in heartbeat.c
                g_bEnableHeartBeat = TRUE;
                break;
            default://unhandled messages
                SystemHalt();   // For debugging
                break;
            }
        } 
                  
    }
}
