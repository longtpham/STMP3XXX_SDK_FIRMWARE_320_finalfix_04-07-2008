;
;  NOTE: This file was generated automatically by rscrenum.pl
;  Do not edit it directly.
;  Created on Mon Jun 30 17:30:17 2008 using ..\output_3500\include\resource.inc as input.



;
;  NOTE: This file was generated automatically by rscrenum.pl
;  Do not edit it directly.
;  Created on Sat Jun 14 11:51:41 2008 using resource.inc as input.



;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2006
;
;  File        :  resource.inc
;  Description :  Resource stuff
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;  usbmsc version number            no leading zeros here!
;///////////////////////////////////////////////////////////////////////////////

VERSION_MAJOR           equ         3
VERSION_MIDDLE          equ         200
VERSION_MINOR           equ         910

LCD_SEG_OFFSET        equ     0       ;Set to 34 if right side up

;///////////////////////////////////////////////////////////////////////////////
;  Common system resources
;///////////////////////////////////////////////////////////////////////////////

ConnectImageLeft        equ        1    ;$FILENAME connectleft.src
ConnectImageRight       equ        2    ;$FILENAME connectright.src
ReadyImage              equ        3    ;$FILENAME ready.src
ReadingImage            equ        4    ;$FILENAME reading.src
WritingImage            equ        5    ;$FILENAME writing.src
NotesImage              equ        6    ;$FILENAME notes.src
LowBatImage             equ        7    ;$FILENAME lowbat.src
RSRC_FULL_SPEED         equ        8    ;$FILENAME fullspeed.src
RSRC_HIGH_SPEED         equ        9    ;$FILENAME highspeed.src
RSRC_LCD_INIT_SEQ       equ       10    ;$FILENAME lcd_controller_init.src
RSRC_VERSION            equ       11    ;$FILENAME version.src
RSRC_PERIOD             equ       12    ;$FILENAME period.src
RSRC_CHARGING           equ       13    ;$FILENAME charging.src
RSRC_CHARGE_COMPLETE    equ       14    ;$FILENAME charge_complete.src
RSRC_FONT_TABLE         equ       15    ;$FILENAME font_table.src

;///////////////////////////////////////////////////////////////////////////////
;  Battery Status
;///////////////////////////////////////////////////////////////////////////////
RSRC_BATTERY_00         equ       16    ;$FILENAME battery_00.src
RSRC_BATTERY_01         equ       17    ;$FILENAME battery_01.src
RSRC_BATTERY_02         equ       18    ;$FILENAME battery_02.src
RSRC_BATTERY_03         equ       19    ;$FILENAME battery_03.src
RSRC_BATTERY_04         equ       20    ;$FILENAME battery_04.src
RSRC_BATTERY_05         equ       21    ;$FILENAME battery_05.src
RSRC_BATTERY_06         equ       22    ;$FILENAME battery_06.src
RSRC_BATTERY_07         equ       23    ;$FILENAME battery_07.src
RSRC_BATTERY_08         equ       24    ;$FILENAME battery_08.src
RSRC_BATTERY_09         equ       25    ;$FILENAME battery_09.src
RSRC_BATTERY_10         equ       26    ;$FILENAME battery_10.src

;///////////////////////////////////////////////////////////////////////////////
;  Font
;///////////////////////////////////////////////////////////////////////////////
RSRC_PGM_8              equ       27    ;$FILENAME font_PGM.src
RSRC_SGMS_8             equ       28    ;$FILENAME font_SGMs.src
RSRC_SCRIPT_00_8        equ       29    ;$FILENAME font_script_00.src
RSRC_SCRIPTS_8          equ       30    ;$FILENAME font_scripts.src
RSRC_PDM                equ       31    ;$FILENAME font_PDM.src
RSRC_SDMS               equ       32    ;$FILENAME font_SDMs.src




RSRC_MMCMEDIAINIT                      equ     0
RSRC_MMCMEDIAALLOCATE                  equ     0
RSRC_MMCMEDIADISCOVERALLOCATION        equ     0
RSRC_MMCMEDIAGETMEDIATABLE             equ     0
RSRC_MMCMEDIAGETINFO                   equ     0
RSRC_MMCMEDIAERASE                     equ     0

RSRC_MMCDATADRIVEINIT                  equ     0
RSRC_MMCDATADRIVEGETINFO               equ     0
RSRC_MMCDATADRIVESETINFO               equ     0
RSRC_MMCDATADRIVEREADSECTOR            equ     0
RSRC_MMCDATADRIVEWRITESECTOR           equ     0 
RSRC_MMCDATADRIVEMULTIWRITESETUP       equ     0 
RSRC_MMCDATADRIVEMULTIWRITESECTOR      equ     0 
RSRC_MMCDATADRIVEERASE                 equ     0 
RSRC_MMCDATADRIVEFLUSH                 equ     0 


RSRC_RAMMEDIAINIT                      equ     0
RSRC_RAMMEDIAALLOCATE                  equ     0
RSRC_RAMMEDIADISCOVERALLOCATION        equ     0
RSRC_RAMMEDIAGETMEDIATABLE             equ     0
RSRC_RAMMEDIAGETINFO                   equ     0
RSRC_RAMMEDIAERASE                     equ     0

RSRC_RAMDATADRIVEINIT                  equ     0
RSRC_RAMDATADRIVEGETINFO               equ     0
RSRC_RAMDATADRIVESETINFO               equ     0
RSRC_RAMDATADRIVEREADSECTOR            equ     0
RSRC_RAMDATADRIVEWRITESECTOR           equ     0
RSRC_RAMDATADRIVEMULTIWRITESETUP       equ     0
RSRC_RAMDATADRIVEMULTIWRITESECTOR      equ     0
RSRC_RAMDATADRIVEERASE                 equ     0
RSRC_RAMDATADRIVEFLUSH                 equ     0

RSRC_RAMSYSTEMDRIVEINIT                equ     0
RSRC_RAMSYSTEMDRIVEGETINFO             equ     0
RSRC_RAMSYSTEMDRIVESETINFO             equ     0
RSRC_RAMSYSTEMDRIVEREADSECTOR          equ     0
RSRC_RAMSYSTEMDRIVEWRITESECTOR         equ     0
RSRC_RAMSYSTEMDRIVEMULTIWRITESETUP     equ     0
RSRC_RAMSYSTEMDRIVEMULTIWRITESECTOR    equ     0
RSRC_RAMSYSTEMDRIVEERASE               equ     0
RSRC_RAMSYSTEMDRIVEFLUSH               equ     0

RSRC_NANDMEDIAINIT                              equ     0
RSRC_NANDMEDIAALLOCATE                          equ     0
RSRC_NANDMEDIADISCOVERALLOCATION                equ     0
RSRC_NANDMEDIAGETMEDIATABLE                     equ     0
RSRC_NANDMEDIAGETINFO                           equ     0
RSRC_NANDMEDIAERASE                             equ     0

RSRC_NANDDATADRIVEINIT                          equ     0
RSRC_NANDDATADRIVEGETINFO                       equ     0
RSRC_NANDDATADRIVESETINFO                       equ     0
RSRC_NANDDATADRIVEREADSECTOR                    equ     0
RSRC_NANDDATADRIVEWRITESECTOR                   equ     0 
RSRC_NANDDATADRIVEDUMBMULTIWRITESETUP           equ     0 
RSRC_NANDDATADRIVEDUMBMULTIWRITESECTOR          equ     0 
RSRC_NANDDATADRIVECACHEMULTIWRITESETUP          equ     0 
RSRC_NANDDATADRIVECACHEMULTIWRITESECTOR         equ     0 
RSRC_NANDDATADRIVEERASE                         equ     0 
RSRC_NANDDATADRIVEFLUSH                         equ     0 

RSRC_NANDSYSTEMDRIVEINIT                        equ     0 
RSRC_NANDSYSTEMDRIVEGETINFO                     equ     0  
RSRC_NANDSYSTEMDRIVESETINFO                     equ     0  
RSRC_NANDSYSTEMDRIVEREADSECTOR                  equ     0 
RSRC_NANDSYSTEMDRIVEWRITESECTOR                 equ     0 
RSRC_NANDSYSTEMDRIVEMULTIWRITESETUP             equ     0 
RSRC_NANDSYSTEMDRIVEMULTIWRITESECTOR            equ     0 
RSRC_NANDSYSTEMDRIVEERASE                       equ     0 
RSRC_NANDSYSTEMDRIVEFLUSH                       equ     0 

