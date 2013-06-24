// Filename: system.c   usbmsc version
#include "kernel.h"
#include "project.h"
#include "messages.h"
#include "usbmsc.h"
#include "usb_api.h"
#include "batterycharge.h"
#include "regsdcdc.h"
#include "regsrevision.h"
#include "misc.h"

#include "resource.h"

extern _P WORD * _P _lc_u_p_usbmsc_lcd_battery_start;
extern _Y WORD * _Y _lc_u_y_Usbmsc_lcd_OVLY_copy_start;
extern _Y WORD * _Y _lc_u_y_Usbmsc_lcd_OVLY_copy_end;
extern _Y WORD * _Y _lc_u_y_Usbmsc_lcd_battery_OVLY_copy_start;
extern _Y WORD * _Y _lc_u_y_Usbmsc_lcd_battery_OVLY_copy_end;

extern _reentrant void CopyCodeIntoBootmanagerArea(_Y WORD * _Y pYCopyStart, 
                _P WORD * _P pPCopyStart, WORD wYCopySize);

extern TaskEntry g_DisplayTaskEntry;
extern CircularBufferDescriptor g_DisplayTaskQueueDescriptor;

extern TaskEntry g_USBInsertionMonitor;
extern TaskEntry g_USBMSCHandler0;
#if NUM_REMOVABLE_MEDIA == 1
    #if (MULTI_LUN_BUILD==0)
extern TaskEntry g_USBMSCHandler1;
    #endif
extern TaskEntry g_DeviceInsertion;
#endif

volatile WORD g_wActivityState = SCSI_IDLE;

// This variable was introduced in SDK3.0 to solve refreshing display when using multi writes
// in a system using overlays. Overlays will disturb multi write sequence.
volatile WORD g_wActivityStateMultiWrite = SCSI_IDLE;
BOOL gMscMtpEnable = FALSE;

//The task entry points can be declared here, or from an include file.
void UserInterfaceTask(void);

#define USER_INTERFACE_STACK_SIZE 100
//declare the task stacks.  Each task must have a stack that is tuned to its needs.
WORD g_UserInterfaceStack[USER_INTERFACE_STACK_SIZE];

//declare the task message queues.  Each task muct have a queue that is tuned to its needs.
_circ WORD g_UserInterfaceMessageQueue[32];

CircularBufferDescriptor g_UserInterfaceQueueDescriptor={ g_UserInterfaceMessageQueue,g_UserInterfaceMessageQueue};

//declare each task as a task entry.
TaskEntry g_UserInterfaceTaskEntry = {&g_UserInterfaceQueueDescriptor,  //Message queue associated with the task
                                       0,                               //Pointer to wait on (meaning depends on state)
                                       0,                               //Word to wait on (meaning depends on state)
                                       EVENT_TIMEOUT,                   //State
                                       g_UserInterfaceStack,            //Current stack pointer
                                       0,                               //Task ID
                                       TIMEOUT_IMMEDIATE,               //initial timeout
                                       {0},                             //Region resources
                                       0,                               //Priority
                                       g_UserInterfaceStack,            //stack pointer
                                       USER_INTERFACE_STACK_SIZE,       //stack size
                                       UserInterfaceTask};              //initial entry point

TaskEntry *g_TaskList[]={
#if (NUM_REMOVABLE_MEDIA == 1)
    #if (MULTI_LUN_BUILD==0)
                        &g_USBMSCHandler1,
    #endif
                        &g_DeviceInsertion,

#endif                         
                        &g_USBInsertionMonitor,
                        &g_UserInterfaceTaskEntry,
                        &g_DisplayTaskEntry,
                        &g_USBMSCHandler0,
                         NULL
                        };

//the routing table describes which messages need to go to which queues.
RoutingEntry g_RoutingTable[] = 
{
    {MSG_TYPE_LCD, &g_DisplayTaskQueueDescriptor},
    NULL
};

SyncObject g_Sync_UserScratchY;
_Y WORD g_wUserScratchY[SCRATCH_USER_Y_SIZE];

SyncObject g_Sync_UserScratchY;
_X WORD g_wUserScratchX[SCRATCH_USER_X_SIZE];

BOOL g_bShutdown = FALSE;

extern int g_iCommonCommands;
extern int g_iUpdaterCommands;
extern int g_iUnknownCommands;

extern void USBLCDDisplayInit(void);
extern void USBLCDCheckForTransfers(void);
extern void USBLCDIdle(void);
extern void USBLCDReading(void);
extern void USBLCDWriting(void);
//extern void USBLCDCheckBrownout(void);
extern void USBLCDDisplayBatteryLevel(void);
extern void USBLCDCheckBatteryLevel(void);

extern void _reentrant Init5VSense(void);
extern void _reentrant ServiceDCDC(void);
////////////////////////////////////////////////////////////////////////////////
// Defines
//#define USB_THROUGHPUT          1

#ifdef  USB_THROUGHPUT
  #define UPDATE_RATE           1000
#else
  #define UPDATE_RATE           100
#endif
#define HALF_SEC_UPDATE_RATE    (500/UPDATE_RATE)

#ifndef USB_THROUGHPUT
// Function Description: 
// Inputs:  no parameters
// Returns: no register returns
// Notes:   
void UserInterfaceTask(void)
{
    USHORT      usStatus;
    //int iTransferRate; // commented out since unused
    WORD State=SCSI_IDLE;
    //Message Msg; // commented out since unused
    int iDelay = HALF_SEC_UPDATE_RATE;
	BOOL bNeedToReEnable = FALSE;
    unsigned int CopySize;

	Init5VSense();
    USBLCDDisplayInit();
    SysWait(250);

    while(1)
    {
        usb_device_get_status(USB_STATUS, &usStatus);

        if(usStatus == USB_STATE_SUSPENDED)
        {
            SysWait(UPDATE_RATE);
        }
        else
        {
            SysPostMessage(2,LCD_END_FRAME);
            SysWait(UPDATE_RATE);
            SysPostMessage(2,LCD_BEGIN_FRAME);
            USBLCDCheckForTransfers();

            //if(g_bServiceDCDC) //note mtp system.c has this flag check. May be needed here
                ServiceDCDC();  //Service DCDC converter

        #ifdef BATTERY_CHARGE
            // For 3500 Battery Monitoring must be done only during battery charging
            // The rest of the time the system operates from USB +5V
            // and battery monitoring does not make any sense.
            // Also, battery brownout always triggered if sys operates w/ no battery.

            //Only charge if we're in high usb current mode. March 11 2005 addition
            if(usb_get_current_limit() <= 100) 
            {    BatteryChargeDisableCharging(TRUE); // until next stmp bootup
            }
            else
            {   if(usStatus !=  USB_STATE_CONFIGURED)
                {   if(bNeedToReEnable == FALSE)
                    {   BatteryChargeDisableCharging(FALSE);
                        bNeedToReEnable = TRUE;
                    }
                }
                else
                {   if(bNeedToReEnable == TRUE)
                    {   BatteryChargeEnableCharging();
                        bNeedToReEnable = FALSE;
                    }               
                    BatteryChargeStateMachine();
                }
            }            
        #endif

        #if defined(BATTERY_CHARGE) || defined(DEVICE_3410) || defined(DEVICE_3310)
            // I only want to update the Battery icon once per 1/2 second
            // but always check and average battery level.
            CopySize = (unsigned int)(&_lc_u_y_Usbmsc_lcd_battery_OVLY_copy_end-&_lc_u_y_Usbmsc_lcd_battery_OVLY_copy_start);
                
            CopyCodeIntoBootmanagerArea((_Y WORD *)&_lc_u_y_Usbmsc_lcd_battery_OVLY_copy_start, 
                                        (_P WORD * _P)&_lc_u_p_usbmsc_lcd_battery_start, CopySize);
  
            USBLCDCheckBatteryLevel();
            if (iDelay-- <= 0)
            {
                USBLCDDisplayBatteryLevel();
                  //On 35xx (has charger), always connected to usb 5v, so never batt brown out
                  #if defined(DEVICE_3410) || defined(DEVICE_3310)
                USBLCDCheckBrownout();
                  #endif
                iDelay = HALF_SEC_UPDATE_RATE;
            }

            CopySize = (unsigned int)(&_lc_u_y_Usbmsc_lcd_OVLY_copy_end-&_lc_u_y_Usbmsc_lcd_OVLY_copy_start);
                
            CopyCodeIntoBootmanagerArea((_Y WORD *)&_lc_u_y_Usbmsc_lcd_OVLY_copy_start, 
                                        (_P WORD * _P)&_lc_u_p_usbmsc_lcd_battery_start, CopySize);
  
#endif
            

            if(State!= g_wActivityState )
            {
                State = g_wActivityState;
                switch(State)
                {
                case SCSI_IDLE:
                    USBLCDIdle();
                    break;
                case SCSI_READING:
                    USBLCDReading();
                    break;
                case SCSI_WRITING:
                    USBLCDWriting();
                    break;
                default :
                    USBLCDIdle();
                }
            }
            
        }
    }
    
}
#endif // ifndef USB_THROUGHPUT

////////////////////////////////////////////////////////////////////////////////
#ifdef USB_THROUGHPUT
// Function Description: 
// Inputs:  no parameters
// Returns: no register returns
// Notes:   
void UserInterfaceTask(void)
{
    int iTransferRate;
    WORD State=SCSI_IDLE;
    //Message Msg;
    int iDelay = HALF_SEC_UPDATE_RATE;
	BOOL bNeedToReEnable = FALSE;
    USHORT usStatus;

    SysPostMessage(5,LCD_CLEAR_RANGE,0,0,98,64);
    SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,ReadyImage);
    SysWait(250);
    while(1)
    {
#ifdef BATTERY_CHARGE
        //Only charge if we're in high usb current mode. March 11 2005 addition
        if(usb_get_current_limit() <= 100) 
        {    BatteryChargeDisableCharging(TRUE); // until next stmp bootup
        }
        else
        {   usb_device_get_status(USB_STATUS, &usStatus);

		if(usStatus !=  USB_STATE_CONFIGURED)
		{
			if(bNeedToReEnable == FALSE)
			{
				BatteryChargeDisableCharging(FALSE);
				bNeedToReEnable = TRUE;
			}
		}
            else //Only charge if we're in high usb current mode. March 11 2005 additional check
		{
			if(bNeedToReEnable == TRUE)
			{
			    BatteryChargeEnableCharging();
				bNeedToReEnable = FALSE;
			}	
                BatteryChargeStateMachine();                
             }
		}
#endif
        SysWait(UPDATE_RATE);
        if(State!= g_wActivityState )
        {
            State = g_wActivityState;
            switch(State)
            {
            case SCSI_IDLE:
                SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,ReadyImage);
                break;
            case SCSI_READING:
                SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,ReadingImage);
                break;
            case SCSI_WRITING:
                SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,WritingImage);
                break;
            }
        }
        
        {   USHORT usStatus;  // fixes stmp 9226 and 8777 (0 or 2 usb unplug tries out of 20 would not 
                              // boot player & mtp was still running due to rare irq loss)
            SysDelayMs(200);
            usb_device_get_status(USB_STATUS_CONNECTION,&usStatus); //we could just check the usb 5v bit instead of calling this.
            if( usStatus == USB_DISCONNECTED )
            {  
            	SysDelay(200); // msec 
                SystemShutdown();// we could use pragma asm to jump to the missed isr instead which resets also.
                SystemReset();
            }
        }

        iTransferRate = g_lBulkInBytes / (1+g_lLastBulkInTime-g_lFirstBulkInTime);
        SysPostMessage(7,LCD_PRINT_NUMBER,0,16,iTransferRate,5,' ');
        iTransferRate = g_lBulkOutBytes / (1+g_lLastBulkOutTime-g_lFirstBulkOutTime);
        SysPostMessage(7,LCD_PRINT_NUMBER,0,24,iTransferRate,5,' ');

        SysWait(0);
        SysPostMessage(7,LCD_PRINT_NUMBER, 0,40,g_iCommonCommands,5,' ');
        SysPostMessage(7,LCD_PRINT_NUMBER,32,40,g_iUpdaterCommands,5,' ');
        SysPostMessage(7,LCD_PRINT_NUMBER,64,40,g_iUnknownCommands,5,' ');
    }
}
#endif
