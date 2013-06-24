#include "types.h"
#include "messages.h"
#include "project.h"
#include "resource.h"
#include "SysResourceApi.h"
//#include "display.h"
#include "displaydriver.h"
#include "s6b1713.h"



_reentrant void HALDisplayInit(void)
{
    int i=0;
	//LCD Reset Pin
    LCD_RESET_GPDO  = FALSE;	//GPIO Data Out Register       
    LCD_RESET_GPDOE = TRUE;     //GPIO Data Out Enable Register
    LCD_RESET_GPEN  = TRUE;     //GPIO Enable Register         
    LCD_RESET_GPPWR = TRUE;     //GPIO Power Register          
    //LCD RS
    LCD_RS_GPDO     = TRUE;		
    LCD_RS_GPDOE    = TRUE;
    LCD_RS_GPPWR    = TRUE; 
    LCD_RS_GPEN     = TRUE;
	//LCD Chip Select(CS)
    LCD_CS_GPDO     = TRUE;
    LCD_CS_GPDOE    = TRUE;
    LCD_CS_GPPWR    = TRUE;
    LCD_CS_GPEN     = TRUE;
	//LCD_E
    LCD_E_GPDO    = TRUE;
    LCD_E_GPDOE   = TRUE;
    LCD_E_GPPWR   = TRUE;
    LCD_E_GPEN    = TRUE;
	//LCD_Back_Light
    LCD_BL_GPDO     = TRUE;
    LCD_BL_GPDOE    = TRUE;
    LCD_BL_GPPWR    = TRUE;
    LCD_BL_GPEN     = TRUE;
    //LCD_DATA
    
    if (gTUNER_STFM1000)
	{    
//	#ifdef TUNER_STFM1000
	    LCD_DATA_D00 = 1;
	    LCD_DATA_D03 = 1;
	    LCD_DATA_D04 = 1;
//	#endif   
	}   
    LCD_DATA_DO     = 0xff;
    LCD_DATA_DOE    = 0xff;
    LCD_DATA_EN     = 0xff;
    LCD_DATA_PWR    = 0xff;
    ///////////////////////////////////////////////////////////////////////////////////////////
    for(i=0; i<20; i++) _MultiNop();    //delay
    LCD_RESET_GPDO  = TRUE;

	//Init command
	HALDisplaySendCommand(LCD_RESET);
	HALDisplaySendCommand(LCD_DISPLAY_OFF);
	HALDisplaySendCommand(LCD_POWER_CONTROL);
	HALDisplaySendCommand(LCD_DISPLAY_START_LINE);
	HALDisplaySendCommand(LCD_DISPLAY_MODE);
	HALDisplaySendCommand(LCD_ENTIRE_DISPLAY);
	HALDisplaySendCommand(LCD_REG_RES_RATIO);
	HALDisplaySendCommand(0xA0);
	HALDisplaySendCommand(0x81);
	HALDisplaySendCommand(0x14);
	HALDisplaySendCommand(0xAF);
	HALDisplaySendCommand(0xE0);
	HALDisplaySendCommand(LCD_FLIP_VERTICAL);
	HALDisplaySendCommand(LCD_FLIP_HOTIZONTAL);	
	HALDisplaySendCommand(LCD_DISPLAY_ON);
	HALDisplaySetContrast(g_iContrastPercentage);
}
