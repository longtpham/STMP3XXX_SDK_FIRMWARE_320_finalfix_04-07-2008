// Filename: sed15xxinit.c
// Reviews: DanhNguyen 2008 (X8iTF)

#include "types.h"
#include "regsgpio.h"
#include "messages.h"
#include "project.h"
#include "resource.h"
#include "SysResourceApi.h"
#include "..\displaydriver.h"
#include "SED15xx.h"
#include "..\..\..\System\MsgModules\Hardware\Tuner\STFM1000\STFM1000.h"

extern INT gTUNER_STFM1000;

_reentrant void HALDisplayZStateEnable(void);
_reentrant void HALDisplayZStateDisable(void);

// Description: Detects STFM1000 non-presence via i2c write error. Inits LCD HW.
// Inputs:
// Outputs:
// Notes: Does not support STMP3400 SOC. 
// Reviews: DanhNguyen 2008

#if     ((VBATT_REF)==(REF_2700MV))
#define VBATT_REF_MV 2700
#define BATT_OFFSET 0x56
#elif   ((VBATT_REF)==(REF_2600MV))
#define VBATT_REF_MV 2600
#define BATT_OFFSET 0x56
#elif   ((VBATT_REF)==(REF_2560MV))
#define VBATT_REF_MV 2560
#define BATT_OFFSET 0
#elif   ((VBATT_REF)==(REF_VDDIO))
#define VBATT_REF_MV 3300
#define BATT_OFFSET 0
#elif   ((VBATT_REF)==(REF_5400MV))
#define VBATT_REF_MV 5400
#define BATT_OFFSET 0x56
#elif   ((VBATT_REF)==(REF_5200MV))
#define VBATT_REF_MV 5200
#define BATT_OFFSET 0x56
#elif   ((VBATT_REF)==(REF_5120MV))
#define VBATT_REF_MV 5120
#define BATT_OFFSET 0
#elif   ((VBATT_REF)==(REF_VDDIOx2))
#define VBATT_REF_MV 6600
#define BATT_OFFSET 0
#else
    //Need to define VBATT_REF in battery_config.h, or it won't work.
#endif
#define VBATT_N_LEVELS   (((VBATT_MAX_MV - VBATT_MIN_MV)*(255 + BATT_OFFSET))/VBATT_REF_MV) //# counts in range
#define VBATT_BASE_LEVEL ((VBATT_MIN_MV*(255 + BATT_OFFSET))/VBATT_REF_MV)		            //# counts at 0%

_reentrant void HALDisplayInit(void)
{	
    int 	i=0;
    //WORD 	LCDInitSequence[30];
    int batt_avg=15;

//HW_DCDC_TBR.B.DCDC1_STOPCLK =TRUE;
//HW_DCDC_TBR.B.DCDC2_STOPCLK=TRUE;

   #ifdef STFM1000_LCD
  	STFM1000TestPresence();	// For Player & USBMSC, set gTUNER_STFM1000 if STFM1000 present 
   #else
	gTUNER_STFM1000 = 1;	// Default to STFM1000 LCD configuration for Tests
   #endif

    // TO ALWAYS FORCE OLD LCD BOARD LAYOUT, DEFINE THIS MACRO IN LINE BELOW. (test apps need this)
//#define NON_STFM1000_LCD 1
   #ifdef NON_STFM1000_LCD
	gTUNER_STFM1000 = 0;	// Use NON STFM1000 LCD configuration for Tests
   #endif

   		//Keep PWRON pin of ltc3455 high to keep power
		PWR_ON_GPDO = TRUE;
		PWR_ON_GPDOE = TRUE;
		PWR_ON_GPEN= TRUE;
		PWR_ON_GPPWR=TRUE;
		
	   	//turn on vcc 9v for oled
		LCD_VCC_GPDO = TRUE;
		LCD_VCC_GPDOE = TRUE;
		LCD_VCC_GPEN=TRUE;
		LCD_VCC_GPPWR=TRUE;

    //set up the GPIO required, start with the reset pin
    LCD_RESET_GPDO  = FALSE;
    LCD_RESET_GPDOE = TRUE;
    LCD_RESET_GPEN  = TRUE;
    LCD_RESET_GPPWR = TRUE;

    LCD_DC_GPDO     = TRUE;
    LCD_DC_GPDOE    = TRUE;
    LCD_DC_GPPWR    = TRUE;
    LCD_DC_GPEN     = TRUE;

    LCD_CS1_GPDO     = TRUE;
    LCD_CS1_GPDOE    = TRUE;
    LCD_CS1_GPPWR    = TRUE;
    LCD_CS1_GPEN     = TRUE;

    LCD_RDE_GPDO    = TRUE;
    LCD_RDE_GPDOE   = TRUE;
    LCD_RDE_GPPWR   = TRUE;
    LCD_RDE_GPEN    = TRUE;

    LCD_RW_GPDO     = TRUE;
    LCD_RW_GPDOE    = TRUE;
    LCD_RW_GPPWR    = TRUE;
    LCD_RW_GPEN     = TRUE;

	if (gTUNER_STFM1000)
	{  LCD_DATA_D00        =  TRUE;       
	    LCD_DATA_D00_GPDOE  =  TRUE; 
	    LCD_DATA_D00_GPEN   =  TRUE; 	  
	    LCD_DATA_D00_GPPWR  = TRUE;    
	
	    LCD_DATA_D03       =  TRUE;     
	    LCD_DATA_D03_GPDOE =  TRUE; 
	    LCD_DATA_D03_GPEN  =  TRUE; 
	    LCD_DATA_D03_GPPWR = TRUE;    
	
	    LCD_DATA_D04       =  TRUE; 
	    LCD_DATA_D04_GPDOE =  TRUE; 	    
	    LCD_DATA_D04_GPEN  =  TRUE;   	    
		LCD_DATA_D04_GPPWR = TRUE; 	

	  	LCD_DATA_DO     = 0xe6;    // 1110 0110 except bits 0, 3, 4  
	    LCD_DATA_DOE    = 0xe6;     	// 1110 0110 except bits 0, 3, 4
	    LCD_DATA_EN     = 0xe6;     	// 1110 0110 except bits 0, 3, 4	 
	    LCD_DATA_PWR    = 0xff;
	}	 
	else // non STFM1000 layout
    {   	LCD_DATA_DO     = 0xff;    
	    	LCD_DATA_DOE    = 0xff;
	    	LCD_DATA_EN     = 0xff;
    		LCD_DATA_PWR    = 0xff;
     }
    //wait for some cycles to let /RES become stable    
/*
#ifdef BACKLIGHT
	LCD_RW_GPEN = TRUE;
	LCD_RW_GPDO = TRUE;
#endif

//      Old Method
//    CSysLoadResource(RSRC_LCD_INIT_SEQ, RSRC_TYPE_DATA, LCDInitSequence,TARGET_MEM_Y,30*3);

    SysLoadResource(RSRC_LCD_INIT_SEQ, RS_ROOT_HANDLE, RSRC_TYPE_DATA,
                        LCDInitSequence,TARGET_MEM_Y,30*3);

    LCDInitSequence[LCDInitSequence[i++]+1]=0;//put a zero in the last item, just to make sure our loop below will terminate
    */

    for(i=0; i<200; i++) _MultiNop();    //delay
    
    LCD_RESET_GPDO  = TRUE;

    //fire out the setup stuff
    /*
    while(LCDInitSequence[i])
    {
        HALDisplaySendCommand(LCDInitSequence[i++]);
    }

    HALDisplaySetContrast(g_iContrastPercentage);
    */
        LCD_RW_GPDO     = FALSE;
    		
	HALDisplaySendCommand(0xAE);	//Display OFF
	HALDisplaySendCommand(0x81);	//Set Contrast Control
	HALDisplaySendCommand(0xFF);	//Max Current
    	HALDisplaySendCommand(0xA8);	//Set Duty    	
	HALDisplaySendCommand(0x3F);	//Duty 63
	HALDisplaySendCommand(0xA0);	//Set Segment Re-map
	HALDisplaySendCommand(0xD3);	//Display Offset	
    	HALDisplaySendCommand(0x00);	//Set 00
    	
	HALDisplaySendCommand(0x40);	//Display Startline
	HALDisplaySendCommand(0xC8);	//Set COM Output Scan Direction
	HALDisplaySendCommand(0xDA);	//Set COM pins Hardware Configuration	
    	HALDisplaySendCommand(0x12);	//Set COM pins Hardware Configuration
	HALDisplaySendCommand(0xD9);	//Set Precharge
	HALDisplaySendCommand(0xF1);	//precharge==FH, discharge==1H
	HALDisplaySendCommand(0xDB);	//Set VcomH	
    	HALDisplaySendCommand(0x49);	//VcomH==73
	HALDisplaySendCommand(0xA4);	//Normal Mode
	HALDisplaySendCommand(0xA6);	//No Inverse
	HALDisplaySendCommand(0xAF);	//Display ON
	
}

// Description:
// Inputs:
// Outputs:
// Notes: 
_reentrant void HALDisplayZStateEnable(void)
{
//    LCD_RESET_GPEN  = FALSE;
    LCD_DC_GPEN     = FALSE;
    LCD_CS1_GPEN    = FALSE;
    LCD_RDE_GPEN    = FALSE;
    LCD_RW_GPEN     = FALSE;
    LCD_DATA_EN     = 0x00;
}

// Description:
// Inputs:
// Outputs:
// Notes: 
_reentrant void HALDisplayZStateDisable(void)
{
//    LCD_RESET_GPEN  = TRUE;
    LCD_DC_GPEN     = TRUE;
    LCD_CS1_GPEN    = TRUE;
    LCD_RDE_GPEN    = TRUE;
    LCD_RW_GPEN     = TRUE;
    LCD_DATA_EN     = 0xff;
}

