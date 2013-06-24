//  NOTE: This file was generated automatically by rscrenum.pl
//  Do not edit it directly.
//  Created on Mon Jun 30 17:30:17 2008 using ..\output_3500\include\resource.inc as input.



//  NOTE: This file was generated automatically by rscrenum.pl
//  Do not edit it directly.
//  Created on Sat Jun 14 11:51:41 2008 using resource.inc as input.



/////////////////////////////////////////////////////////////////////////////////
//  Copyright(C) SigmaTel, Inc. 2000-2006
//  File        :  resource.inc
//  Description :  Resource stuff
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
//  usbmsc version number            no leading zeros here!
/////////////////////////////////////////////////////////////////////////////////

#define VERSION_MAJOR 3
#define VERSION_MIDDLE 200
#define VERSION_MINOR 910

//Set to 34 if right side up
#define LCD_SEG_OFFSET 0       

/////////////////////////////////////////////////////////////////////////////////
//  Common system resources
/////////////////////////////////////////////////////////////////////////////////

//$FILENAME connectleft.src
#define ConnectImageLeft 1    
//$FILENAME connectright.src
#define ConnectImageRight 2    
//$FILENAME ready.src
#define ReadyImage 3    
//$FILENAME reading.src
#define ReadingImage 4    
//$FILENAME writing.src
#define WritingImage 5    
//$FILENAME notes.src
#define NotesImage 6    
//$FILENAME lowbat.src
#define LowBatImage 7    
//$FILENAME fullspeed.src
#define RSRC_FULL_SPEED 8    
//$FILENAME highspeed.src
#define RSRC_HIGH_SPEED 9    
//$FILENAME lcd_controller_init.src
#define RSRC_LCD_INIT_SEQ 10    
//$FILENAME version.src
#define RSRC_VERSION 11    
//$FILENAME period.src
#define RSRC_PERIOD 12    
//$FILENAME charging.src
#define RSRC_CHARGING 13    
//$FILENAME charge_complete.src
#define RSRC_CHARGE_COMPLETE 14    
//$FILENAME font_table.src
#define RSRC_FONT_TABLE 15    

/////////////////////////////////////////////////////////////////////////////////
//  Battery Status
/////////////////////////////////////////////////////////////////////////////////
//$FILENAME battery_00.src
#define RSRC_BATTERY_00 16    
//$FILENAME battery_01.src
#define RSRC_BATTERY_01 17    
//$FILENAME battery_02.src
#define RSRC_BATTERY_02 18    
//$FILENAME battery_03.src
#define RSRC_BATTERY_03 19    
//$FILENAME battery_04.src
#define RSRC_BATTERY_04 20    
//$FILENAME battery_05.src
#define RSRC_BATTERY_05 21    
//$FILENAME battery_06.src
#define RSRC_BATTERY_06 22    
//$FILENAME battery_07.src
#define RSRC_BATTERY_07 23    
//$FILENAME battery_08.src
#define RSRC_BATTERY_08 24    
//$FILENAME battery_09.src
#define RSRC_BATTERY_09 25    
//$FILENAME battery_10.src
#define RSRC_BATTERY_10 26    

/////////////////////////////////////////////////////////////////////////////////
//  Font
/////////////////////////////////////////////////////////////////////////////////
//$FILENAME font_PGM.src
#define RSRC_PGM_8 27    
//$FILENAME font_SGMs.src
#define RSRC_SGMS_8 28    
//$FILENAME font_script_00.src
#define RSRC_SCRIPT_00_8 29    
//$FILENAME font_scripts.src
#define RSRC_SCRIPTS_8 30    
//$FILENAME font_PDM.src
#define RSRC_PDM 31    
//$FILENAME font_SDMs.src
#define RSRC_SDMS 32    




#define RSRC_MMCMEDIAINIT 0
#define RSRC_MMCMEDIAALLOCATE 0
#define RSRC_MMCMEDIADISCOVERALLOCATION 0
#define RSRC_MMCMEDIAGETMEDIATABLE 0
#define RSRC_MMCMEDIAGETINFO 0
#define RSRC_MMCMEDIAERASE 0

#define RSRC_MMCDATADRIVEINIT 0
#define RSRC_MMCDATADRIVEGETINFO 0
#define RSRC_MMCDATADRIVESETINFO 0
#define RSRC_MMCDATADRIVEREADSECTOR 0
#define RSRC_MMCDATADRIVEWRITESECTOR 0 
#define RSRC_MMCDATADRIVEMULTIWRITESETUP 0 
#define RSRC_MMCDATADRIVEMULTIWRITESECTOR 0 
#define RSRC_MMCDATADRIVEERASE 0 
#define RSRC_MMCDATADRIVEFLUSH 0 


#define RSRC_RAMMEDIAINIT 0
#define RSRC_RAMMEDIAALLOCATE 0
#define RSRC_RAMMEDIADISCOVERALLOCATION 0
#define RSRC_RAMMEDIAGETMEDIATABLE 0
#define RSRC_RAMMEDIAGETINFO 0
#define RSRC_RAMMEDIAERASE 0

#define RSRC_RAMDATADRIVEINIT 0
#define RSRC_RAMDATADRIVEGETINFO 0
#define RSRC_RAMDATADRIVESETINFO 0
#define RSRC_RAMDATADRIVEREADSECTOR 0
#define RSRC_RAMDATADRIVEWRITESECTOR 0
#define RSRC_RAMDATADRIVEMULTIWRITESETUP 0
#define RSRC_RAMDATADRIVEMULTIWRITESECTOR 0
#define RSRC_RAMDATADRIVEERASE 0
#define RSRC_RAMDATADRIVEFLUSH 0

#define RSRC_RAMSYSTEMDRIVEINIT 0
#define RSRC_RAMSYSTEMDRIVEGETINFO 0
#define RSRC_RAMSYSTEMDRIVESETINFO 0
#define RSRC_RAMSYSTEMDRIVEREADSECTOR 0
#define RSRC_RAMSYSTEMDRIVEWRITESECTOR 0
#define RSRC_RAMSYSTEMDRIVEMULTIWRITESETUP 0
#define RSRC_RAMSYSTEMDRIVEMULTIWRITESECTOR 0
#define RSRC_RAMSYSTEMDRIVEERASE 0
#define RSRC_RAMSYSTEMDRIVEFLUSH 0

#define RSRC_NANDMEDIAINIT 0
#define RSRC_NANDMEDIAALLOCATE 0
#define RSRC_NANDMEDIADISCOVERALLOCATION 0
#define RSRC_NANDMEDIAGETMEDIATABLE 0
#define RSRC_NANDMEDIAGETINFO 0
#define RSRC_NANDMEDIAERASE 0

#define RSRC_NANDDATADRIVEINIT 0
#define RSRC_NANDDATADRIVEGETINFO 0
#define RSRC_NANDDATADRIVESETINFO 0
#define RSRC_NANDDATADRIVEREADSECTOR 0
#define RSRC_NANDDATADRIVEWRITESECTOR 0 
#define RSRC_NANDDATADRIVEDUMBMULTIWRITESETUP 0 
#define RSRC_NANDDATADRIVEDUMBMULTIWRITESECTOR 0 
#define RSRC_NANDDATADRIVECACHEMULTIWRITESETUP 0 
#define RSRC_NANDDATADRIVECACHEMULTIWRITESECTOR 0 
#define RSRC_NANDDATADRIVEERASE 0 
#define RSRC_NANDDATADRIVEFLUSH 0 

#define RSRC_NANDSYSTEMDRIVEINIT 0 
#define RSRC_NANDSYSTEMDRIVEGETINFO 0  
#define RSRC_NANDSYSTEMDRIVESETINFO 0  
#define RSRC_NANDSYSTEMDRIVEREADSECTOR 0 
#define RSRC_NANDSYSTEMDRIVEWRITESECTOR 0 
#define RSRC_NANDSYSTEMDRIVEMULTIWRITESETUP 0 
#define RSRC_NANDSYSTEMDRIVEMULTIWRITESECTOR 0 
#define RSRC_NANDSYSTEMDRIVEERASE 0 
#define RSRC_NANDSYSTEMDRIVEFLUSH 0 


