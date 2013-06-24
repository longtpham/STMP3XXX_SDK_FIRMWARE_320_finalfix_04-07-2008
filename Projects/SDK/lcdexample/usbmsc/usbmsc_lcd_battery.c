//Reviews by DanhNguyen (06-2008)
//For JVJ Mp3 design.

#include "kernel.h"
#include "project.h"
#include "messages.h"
#include "resource.h"
#include "battery_config.h"
#include "battery.h"
#include "batterycharge.h"
#include "usb_api.h"
#include "regsgpio.h"

// Bitmap defines for display
#define   CONNECT_LEFT_X        0
#define   CONNECT_LEFT_Y        8

#define   CONNECT_RIGHT_X       48
#define   CONNECT_RIGHT_Y       CONNECT_LEFT_Y

#define   DISPLAY_TEXT_X        0
#define   DISPLAY_TEXT_Y        48

#define   DISPLAY_SPEED_X        8
#define   DISPLAY_SPEED_Y        16

#define   DISP_VERS_Y           4
#define   DISP_VERS_TEXT_X      0
#define   DISP_VERS_MAJ_X       16
#define   DISP_VERS_PER1_X      DISP_VERS_MAJ_X + 18
#define   DISP_VERS_MIDDLE_X    DISP_VERS_PER1_X + 4
#define   DISP_VERS_PER2_X      DISP_VERS_MIDDLE_X + 18
#define   DISP_VERS_MIN_X       DISP_VERS_PER2_X + 4

#ifdef S6B33B0A_LCD
#define   DISPLAY_RANGE_X       128
#define   DISPLAY_RANGE_Y       160
#endif

#ifdef SED15XX_LCD
#define   DISPLAY_RANGE_X       128
#define   DISPLAY_RANGE_Y       64
#endif

#define   LOC_X_MAX_LEFT        38
#define   LOC_X_MAX_RIGHT       60

#define   DISPLAY_NOTES_MIN_X   LOC_X_MAX_LEFT
#define   DISPLAY_NOTES_MAX_X   LOC_X_MAX_RIGHT
#define   DISPLAY_NOTES_Y       24
#define   NOTE_X_CLEAR_WIDTH    DISPLAY_NOTES_MAX_X-DISPLAY_NOTES_MIN_X+NOTE_X_WIDTH
#define   NOTE_X_WIDTH          6
#define   NOTE_X_HEIGHT         8
#define   NOTE_SHIFT_AMOUNT     1

#define   DISP_LO_BATT_X        8
#define   DISP_LO_BATT_Y        76

#define   BATTERY_ICON_X_POS      110
#define   BATTERY_ICON_Y_POS      0
#define   BATTERY_ICON_X_SIZE     16
#define   BATTERY_ICON_Y_SIZE     8
#define   BATTERY_RESOURCE        RSRC_BATTERY_00

//#define   SPACE_CHAR            0x20
//#define   ZERO_CHAR             0x30

enum {
  Idle,
  Reading,
  Writing
};

typedef struct _NoteStruct 
{
  WORD  wNoteLocation;
  WORD  wStateImage;
  BOOL  bShowNotes;
} NoteStruct;

void USBLCDCheckBrownout(void);
void USBLCDLowBattery(void);
void USBLCDCheckBatteryLevel(void);

extern NoteStruct zNoteData;
//extern WORD g_SysBrownout;
extern WORD g_wBatLevelSegment;

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          USBLCDCheckBrownout
//
//   Type:          Function
//
//   Description:   Display Low Battery Warning on LCD if battery brownout level reached
//
//   Inputs:        None.
//
//   Outputs:       None 
//
//   Notes:         currently just a stub for usbmsc app
//<
////////////////////////////////////////////////////////////////////////////////
void USBLCDCheckBrownout(void)
{
  // For Mass Storage Branch, power always comes from USB 5V supply.
  // Also, g_SysBrownout is currently never set in the entire sdk tree.
#if 0
  // TODO - Add handler to check brownouts.  Only hook is in here for now
  // Display Low battery indicator if batt brownout bit was flagged.
  if (g_SysBrownout & BROWNOUT_LEVEL1_DETECTED) 
  {
    USBLCDLowBattery();
  }
#endif
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          USBLCDLowBattery
//
//   Type:          Function
//
//   Description:   Display Low Battery Warning.
//
//   Inputs:        None.
//
//   Outputs:       None 
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////

void USBLCDLowBattery(void)
{
  // Display Low Battery Image - same place as Ready, Reading, and Writing.
  SysPostMessage(5,LCD_PRINT_RANGE_RSRC,DISPLAY_TEXT_X,DISPLAY_TEXT_Y,LowBatImage);
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          USBLCDDisplayBatteryLevel
//
//   Type:          Function
//
//   Description:   Display Battery Level Icon.
//
//   Inputs:        None
//
//   Outputs:       None 
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
//GPIO43 Charge status read back from ltc3455
#ifndef CHRG_STATUS_GPDI
#define CHRG_STATUS_GPDI      HW_GP1DIR.B.B19
#define CHRG_STATUS_GPDOE     HW_GP1DOER.B.B19
#define CHRG_STATUS_GPEN      HW_GP1ENR.B.B19
#define CHRG_STATUS_GPPWR     HW_GP1PWR.B.B19
#endif 

void USBLCDDisplayBatteryLevel(void)
{
    BOOL bDisplayBattery = FALSE;
    static WORD wBlinkTime = 0;
    WORD wResource;

	// Initialize register for reading Charge Status
	CHRG_STATUS_GPDOE = FALSE; //use pin as an input
	CHRG_STATUS_GPEN=TRUE;	//use the pin as GPIO
	CHRG_STATUS_GPPWR=TRUE;	//Power up the pin

	if(CHRG_STATUS_GPDI==FALSE)
	{  
		wResource = BATTERY_RESOURCE+wBlinkTime;
        	if(wResource > RSRC_BATTERY_10)
            		wResource = RSRC_BATTERY_10;
		if (wBlinkTime++ > 10)
			wBlinkTime = 0;
		SysPostMessage(5, LCD_PRINT_RANGE_RSRC,0,0,RSRC_CHARGING);
	}
	else
	{
		wResource = RSRC_BATTERY_10;
		SysPostMessage(5, LCD_PRINT_RANGE_RSRC,0,0,RSRC_CHARGE_COMPLETE);
	}
	SysPostMessage(5, LCD_PRINT_RANGE_RSRC,BATTERY_ICON_X_POS,BATTERY_ICON_Y_POS,wResource);
	
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          USBLCDCheckBatteryLevel
//
//   Type:          Function
//
//   Description:   Check Battery Level.
//
//   Inputs:        None
//
//   Outputs:       None 
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
void USBLCDCheckBatteryLevel(void)
{

	g_wBatLevelSegment =  SysBatteryGetLevel();

}
