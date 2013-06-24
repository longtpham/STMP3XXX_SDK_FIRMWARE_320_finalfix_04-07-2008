#include "kernel.h"
#include "project.h"
#include "messages.h"
#include "resource.h"
#include "battery_config.h"
#include "battery.h"
#include "batterycharge.h"
#include "usb_api.h"

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
#define   DISPLAY_RANGE_X       98
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

#define   BATTERY_ICON_X_POS      80
#define   BATTERY_ICON_Y_POS      4
#define   BATTERY_ICON_X_SIZE     16
#define   BATTERY_ICON_Y_SIZE     8
#define   BATTERY_RESOURCE        RSRC_BATTERY_00


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

void USBLCDDisplayInit();
void USBLCDCheckForTransfers(void);
void USBLCDIdle(void);
void USBLCDReading(void);
void USBLCDWriting(void);
void USBLCDShiftNotes(void);
void USBLCDClearNotes(void);
void USBLCDCheckBrownout(void);
void USBLCDLowBattery(void);
void USBLCDDoNotDisconnect(void);
void USBLCDCheckBatteryLevel(void);

extern volatile INT g_DB_Dirty;
extern volatile INT g_stor_dirty;

NoteStruct zNoteData = {DISPLAY_NOTES_MIN_X, Reading, FALSE};
//WORD g_SysBrownout=0;
WORD g_wBatLevelSegment;


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          DisplayInit
//
//   Type:          Function
//
//   Description:   Initialize the LCD display with the static icons.
//
//   Inputs:        None
//
//   Outputs:       None 
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
void USBLCDDisplayInit(void)
{
  SysPostMessage(5,LCD_CLEAR_RANGE,0,0,DISPLAY_RANGE_X,DISPLAY_RANGE_Y);
  SysPostMessage(5,LCD_PRINT_RANGE_RSRC,DISPLAY_TEXT_X,DISPLAY_TEXT_Y,ReadyImage);
  SysPostMessage(5,LCD_PRINT_RANGE_RSRC,CONNECT_LEFT_X,CONNECT_LEFT_Y,ConnectImageLeft);
  SysPostMessage(5,LCD_PRINT_RANGE_RSRC,CONNECT_RIGHT_X,CONNECT_RIGHT_Y,ConnectImageRight);

  SysWait(0);     // Breather to allow Post queue to empty.
  SysPostMessage(5,LCD_PRINT_RANGE_RSRC,DISP_VERS_TEXT_X,DISP_VERS_Y,RSRC_VERSION);
  SysWait(0);     // Breather to allow Post queue to empty.

  SysPostMessage(7,LCD_PRINT_NUMBER,    DISP_VERS_MAJ_X,    DISP_VERS_Y,    VERSION_MAJOR,3,SPACE_CHAR);
  SysWait(0);     // Breather to allow Post queue to empty.
  SysPostMessage(5,LCD_PRINT_RANGE_RSRC,DISP_VERS_PER1_X,   DISP_VERS_Y,    RSRC_PERIOD);
  SysWait(0);     // Breather to allow Post queue to empty.
  SysPostMessage(7,LCD_PRINT_NUMBER,    DISP_VERS_MIDDLE_X, DISP_VERS_Y,    VERSION_MIDDLE,3,ZERO_CHAR);
  SysWait(0);     // Breather to allow Post queue to empty.
  SysPostMessage(5,LCD_PRINT_RANGE_RSRC,DISP_VERS_PER2_X,   DISP_VERS_Y,    RSRC_PERIOD);
  SysWait(0);     // Breather to allow Post queue to empty.
  SysPostMessage(7,LCD_PRINT_NUMBER    ,DISP_VERS_MIN_X,    DISP_VERS_Y,    VERSION_MINOR,3,ZERO_CHAR);
  SysWait(0);     // Breather to allow Post queue to empty.
  USBLCDIdle();
//    SysPostMessage(5,LCD_PRINT_RANGE_RSRC,DISP_VERS_PER_X,DISP_VERS_Y,LowBatImage);
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          USBLCDIdle
//
//   Type:          Function
//
//   Description:   Display Idle Text
//
//   Inputs:        None.
//
//   Outputs:       None 
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
void USBLCDIdle(void)
{
    USHORT usStatus;
    WORD wBitmap;

    usb_device_get_status(USB_STATUS_DEVICE_SPEED, &usStatus);

    if(usStatus == USB_SPEED_HIGH)
    {
        wBitmap = RSRC_HIGH_SPEED;
    }
    else
    {
        wBitmap = RSRC_FULL_SPEED;
    }

  SysPostMessage(5,LCD_PRINT_RANGE_RSRC,DISPLAY_TEXT_X,DISPLAY_TEXT_Y,ReadyImage);
  SysPostMessage(5,LCD_PRINT_RANGE_RSRC,DISPLAY_SPEED_X,DISPLAY_SPEED_Y,wBitmap);
  USBLCDClearNotes();
  zNoteData.wStateImage = ReadyImage;
  zNoteData.bShowNotes = FALSE;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          USBLCDReading
//
//   Type:          Function
//
//   Description:   Display Reading Text & Notes when reading from media.
//
//   Inputs:        None.
//
//   Outputs:       None 
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
void USBLCDReading(void)
{

  SysPostMessage(5,LCD_PRINT_RANGE_RSRC,DISPLAY_TEXT_X,DISPLAY_TEXT_Y,ReadingImage);
  USBLCDClearNotes();
  SysWait(0);     // Breather to allow Post queue to empty.
  zNoteData.wNoteLocation = DISPLAY_NOTES_MAX_X;
  zNoteData.wStateImage = ReadingImage;
  zNoteData.bShowNotes = TRUE;
  USBLCDShiftNotes();
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          USBLCDWriting
//
//   Type:          Function
//
//   Description:   Display Writing Text & Notes when writing to media.
//
//   Inputs:        None.
//
//   Outputs:       None 
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
void USBLCDWriting(void)
{
  SysPostMessage(5,LCD_PRINT_RANGE_RSRC,DISPLAY_TEXT_X,DISPLAY_TEXT_Y,WritingImage);
  USBLCDClearNotes();
  SysWait(0);     // Breather to allow Post queue to empty.
  zNoteData.wNoteLocation = DISPLAY_NOTES_MIN_X;
  zNoteData.wStateImage = WritingImage;
  zNoteData.bShowNotes = TRUE;
  USBLCDShiftNotes();
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          USBLCDCheckForTransfers
//
//   Type:          Function
//
//   Description:   If transfers are occuring, shift notes.
//
//   Inputs:        None.
//
//   Outputs:       None 
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
void USBLCDCheckForTransfers(void)
{
  if (zNoteData.bShowNotes)
  {
    USBLCDShiftNotes();
  }
}


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          ShiftNotes
//
//   Type:          Function
//
//   Description:   Shift the musical notes across the screen.
//
//   Inputs:        bReadWrite        0=Read, 1=Write
//                  *pwNoteLocation   The lower left location of the musical
//                                    notes.
//
//   Outputs:       None 
//
//   Notes:         The musical notes will slide across the display.
//<
////////////////////////////////////////////////////////////////////////////////
void USBLCDShiftNotes(void)
{
  WORD wLocation = zNoteData.wNoteLocation;

  USBLCDClearNotes();
  if (zNoteData.wStateImage == ReadingImage)          // Reading...
  {
    wLocation -= NOTE_SHIFT_AMOUNT;
    wLocation = (wLocation > DISPLAY_NOTES_MIN_X) ? wLocation : DISPLAY_NOTES_MAX_X;
  } else if (zNoteData.wStateImage == WritingImage)   // Writing...
  {
    wLocation += NOTE_SHIFT_AMOUNT;
    wLocation = (wLocation < DISPLAY_NOTES_MAX_X) ? wLocation : DISPLAY_NOTES_MIN_X;
  }
  zNoteData.wNoteLocation = wLocation;
  SysPostMessage(5,LCD_PRINT_RANGE_RSRC,wLocation,DISPLAY_NOTES_Y,NotesImage);
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          ClearNotes
//
//   Type:          Function
//
//   Description:   Clear the last musical note from the screen.
//
//   Inputs:        None
//
//   Outputs:       None 
//
//   Notes:         wNoteLocation   The lower left location of the musical notes.
//<
////////////////////////////////////////////////////////////////////////////////
void USBLCDClearNotes(void)
{
  SysPostMessage(6,LCD_CLEAR_RANGE,zNoteData.wNoteLocation,DISPLAY_NOTES_Y,NOTE_X_WIDTH,NOTE_X_HEIGHT);
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          USBLCDCheckDBStoreDirty
//
//   Type:          Function
//
//   Description:   If Store or Database is dirty, display message asking user 
//                  not to disconnect USB connection.  The message says
//                  "Do Not Disconnect", but since there is only one connection,
//                  the meaning of the warning should be clear.
//
//   Inputs:        None.
//
//   Outputs:       None 
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
void USBLCDCheckDBStoreDirty(void)
{
  if ((g_DB_Dirty) || (g_stor_dirty))
  {
    USBLCDDoNotDisconnect();
  }
  else
  {
    SysPostMessage(5,LCD_PRINT_RANGE_RSRC,DISPLAY_TEXT_X,DISPLAY_TEXT_Y,ReadyImage);
  }
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
//>  Name:          USBLCDDoNotDisconnect
//
//   Type:          Function
//
//   Description:   Display Warning Not To Disconnect Device from USB.
//
//   Inputs:        None.
//
//   Outputs:       None 
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
void USBLCDDoNotDisconnect(void)
{
  // Display Low Battery Image - same place as Ready, Reading, and Writing.
  SysPostMessage(5,LCD_PRINT_RANGE_RSRC,DISPLAY_TEXT_X,DISPLAY_TEXT_Y,DoNotDisconnectImage);
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
void USBLCDDisplayBatteryLevel(void)
{
    BOOL bDisplayBattery = FALSE;
    static WORD wBlinkTime = 0;
    WORD wResource;


    // Always check & average the battery level, but don't always display it.
#ifdef BATTERY_CHARGE
    if(BatteryChargeIsCharging() && ( !BatteryChargeIsTrickleCharging() ))
    {                                // Trickle charging displys as FULL batt.
        wResource = BATTERY_RESOURCE+wBlinkTime;
        if(wResource > RSRC_BATTERY_10)
            wResource = RSRC_BATTERY_10;
    }
    else
#endif
    {
        wResource = BATTERY_RESOURCE+g_wBatLevelSegment;
    }

    SysPostMessage(5,LCD_PRINT_RANGE_RSRC,BATTERY_ICON_X_POS,BATTERY_ICON_Y_POS,wResource);
        
    if (g_wBatLevelSegment < 1) // < 10% - show low battery.
    {
        if (wBlinkTime > 5)
        {
            USBLCDLowBattery();
        } else
        {
            SysPostMessage(5,LCD_PRINT_RANGE_RSRC,DISPLAY_TEXT_X,DISPLAY_TEXT_Y,zNoteData.wStateImage);
        }
    }
    if (wBlinkTime++ > 10)
        wBlinkTime = 0;

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
