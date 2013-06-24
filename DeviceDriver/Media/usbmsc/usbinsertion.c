
#include "kernel.h"
#include "usb_api.h"
#include "misc.h"
#include "usbmsc.h"
#include "ddildl.h"
#include "regsgpflash.h"
#include "regsusb20.h"
#include "ipr.h"
#include "regscore.h"

#ifndef USB_WATCH_CALL_BACK_DELAY
#define USB_WATCH_CALL_BACK_DELAY 200
#endif 

extern WORD g_wUsbMscNumDevices;


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          USBInsertionMonitorTask
//
//   Type:          Function
//
//   Description:   SCMOS USB disconnect/suspend monitor
//
//   Inputs:        none
//
//   Outputs:       STRULE
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////

void _reentrant USBInsertionMonitorTask(void)
{
    USHORT usStatus;
    BOOL bInserted = FALSE;
    WORD i,j;
	WORD iWaitCount;

    // Check for USB disconnect
    while(1)
    {
        usb_device_get_status(USB_STATUS_CONNECTION,&usStatus);

        switch(usStatus)
        {
        case USB_CONNECTED:
            bInserted=TRUE;
            break;
        default:
            if(bInserted)//if we've been inserted, and now we're not, lets shut down
            {
//              #ifdef DEVICE_3410         // 3410 still has power when removed from usb.
              #ifdef CHKDSK
                  UsbMscCheckDiskAll();
              #endif
//              #endif
              
              // TODO - put this back in later if needed
              // Clear the display
//              ClearDisplay();
        // We need to immediately pull Write Protect low to protect the NANDs and
        // to reset the Renesas part (same line).
        HW_GPFLASH_CSR1R.B.WP0 = 0;

			  // We need to clean up any media transactions that may be hanging
              // As best we can :)
			  // stmp4770 and stmp4795, Remove drive flush on USB disconnect for NAND devices.
			  // Drive flush still necessary for MMC media, to properly close multi-writes
			  // that may be pending between SCSI commands.
              for(i=0;i<g_wUsbMscNumDevices;i++)
              {                  
                for(j=0;j<UsbMscDevice[i].wNumLunsSupported;j++)
                {
                  if(UsbMscDevice[i].Lun[j].bMediaIsRemovable == TRUE)
                  {
                    DriveFlush(UsbMscDevice[i].Lun[j].wFileSystemDriveNumber);
                  } 
                }               
              }
              
              //Do not leave.  Wait for 5V to be removed.
	          //Monitor D+/- and reset if it returns.
    	      HW_USBCSR.B.PLUGGEDIN_EN = 1;

			  iWaitCount = 0;
			  //ensure 5V Disconnect IRQ is enabled
			  SysSetIrqLevel(HW_SR_IM_L2_SETMASK);

			  while(HW_USBCSR.B.VBUSSENSE)
			  {
                  //if USB is plugged-in.  Break loop and reset to return to USBMSC.
                  if(HW_USBCSR.B.PLUGGEDIN)
                  {
                     break;
                  }


			      //wait a bit
			      SysWait(200);

				  if(iWaitCount > 10)
				  {
					 break;		// if we waited a couple secs, time to break
				  }
				  else
				  {
				  	iWaitCount++;	//increment wait count
				  }

			   }                     
			  SysWait(200);

              // Shut down
              SystemShutdown();

              // Reset the device
              SystemReset();
            }
        }
        SysWait(USB_WATCH_CALL_BACK_DELAY);
    }    
}

#ifndef INSERTION_MONITOR_TASK_STACK_SIZE
#define INSERTION_MONITOR_TASK_STACK_SIZE 500
#endif

WORD g_USBInsertionMonitorTaskStack[INSERTION_MONITOR_TASK_STACK_SIZE];
WORD g_USBInsertionMonitorTaskStackEnd; 


void _reentrant USBInsertionMonitorTask(void);
_circ WORD g_USBInsertionMonitorTaskMessageQueue[16];
CircularBufferDescriptor g_USBInsertionMonitorTaskMessageQueueDescriptor={ g_USBInsertionMonitorTaskMessageQueue,
                                                                            g_USBInsertionMonitorTaskMessageQueue};

TaskEntry g_USBInsertionMonitor = {&g_USBInsertionMonitorTaskMessageQueueDescriptor,//message queue
                                    NULL,                                           //Waiting on ptr
                                    0,                                              //Waiting on value
                                    EVENT_TIMEOUT,                                  //State
                                    g_USBInsertionMonitorTaskStack,                 //Current stack ptr
                                    0,                                              //Task ID
                                    TIMEOUT_IMMEDIATE,                              //Initial timeout
                                    {0},                                            //Region requirements table
                                    0,                                              //Priority
                                    g_USBInsertionMonitorTaskStack,                 //Initial stack ptr (used in init)
                                    INSERTION_MONITOR_TASK_STACK_SIZE,              //stack size
                                    USBInsertionMonitorTask};                       //entry point
