;
;  NOTE: This file was generated automatically by rscrenum.pl
;  Do not edit it directly.
;  Created on Tue Apr 30 17:17:50 2002 using resource.inc as input.



;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000
;
; Resource stuff for lcd display and media mapper
;///////////////////////////////////////////////////////////////////////////////

    IF (!@def(resources))
resources equ 1

;///////////////////////////////////////////////////////////////////////////////
;  Common system resources
;///////////////////////////////////////////////////////////////////////////////

RSRC_LCD_INIT_SEQ       equ        1    ;$FILENAME system_lcd_init_seq.src
RSRC_FONT_TABLE         equ        2    ;$FILENAME font_table.src
RSRC_PDM                equ        3    ;$FILENAME iso8859_PDM.src
RSRC_SDMS               equ        4    ;$FILENAME iso8859_SDMs.src
RSRC_PGM_8              equ        5    ;$FILENAME sigmatel_8_PGM.src
RSRC_SGMS_8             equ        6    ;$FILENAME sigmatel_8_SGMs.src
RSRC_SCRIPTS_8          equ        7    ;$FILENAME sigmatel_8_scripts.src
RSRC_SCRIPT_00_8        equ        8    ;$FILENAME sigmatel_8_script_00.src
RSRC_DISPLAY_MODULE	equ    $ffff    ;bogus number inserted to make it compile fine

RSRC_NANDMEDIAINIT                      equ     0
RSRC_NANDMEDIAALLOCATE                  equ     0
RSRC_NANDMEDIADISCOVERALLOCATION        equ     0
RSRC_NANDMEDIAGETMEDIATABLE             equ     0
RSRC_NANDMEDIAGETINFO                   equ     0
RSRC_NANDMEDIAERASE                     equ     0

RSRC_NANDDATADRIVEINIT                  equ     0
RSRC_NANDDATADRIVEGETINFO               equ     0
RSRC_NANDDATADRIVESETINFO               equ     0
RSRC_NANDDATADRIVEREADSECTOR            equ     0
RSRC_NANDDATADRIVEWRITESECTOR           equ     0 
RSRC_NANDDATADRIVEMULTIWRITESETUP       equ     0 
RSRC_NANDDATADRIVEMULTIWRITESECTOR      equ     0 
RSRC_NANDDATADRIVEERASE                 equ     0 
RSRC_NANDDATADRIVEFLUSH                 equ     0 

RSRC_NANDSYSTEMDRIVEINIT                equ     0 
RSRC_NANDSYSTEMDRIVEGETINFO             equ     0  
RSRC_NANDSYSTEMDRIVESETINFO             equ     0  
RSRC_NANDSYSTEMDRIVEREADSECTOR          equ     0 
RSRC_NANDSYSTEMDRIVEWRITESECTOR         equ     0 
RSRC_NANDSYSTEMDRIVEMULTIWRITESETUP     equ     0 
RSRC_NANDSYSTEMDRIVEMULTIWRITESECTOR    equ     0 
RSRC_NANDSYSTEMDRIVEERASE               equ     0 
RSRC_NANDSYSTEMDRIVEFLUSH               equ     0 

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

RSRC_FUNCLET_CHIPSERIALNUMBERINIT          equ 0
RSRC_FUNCLET_INTERNALMEDIASERIALNUMBERINIT equ 0
RSRC_FUNCLET_CHIPGETSERIALNUMBER           equ 0
RSRC_FUNCLET_CREATENULLSERIALNUMBER        equ 0
RSRC_FUNCLET_CONVERTHEXTOASCII             equ 0

RSRC_NANDDATADRIVEDUMBMULTIWRITESETUP    	equ      0
RSRC_NANDDATADRIVEDUMBMULTIWRITESECTOR  	equ      0

RSRC_NANDDATADRIVECACHEMULTIWRITESETUP    	equ    	 0
RSRC_NANDDATADRIVECACHEMULTIWRITESECTOR 	equ      0

RSRC_FUNCLET_NANDDATADRIVEINITWRAPPED    	equ      0
RSRC_FUNCLET_BUILDDATAREGIONSLIST    		equ      0
RSRC_FUNCLET_RESTOREBBTABLE    			equ      0
RSRC_FUNCLET_MMC_TEST_ERASE    			equ      0
RSRC_FUNCLET_MMC_HALINIT    			equ      0

    ENDIF ;IF (!@def(resources))
