;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000
;
; LCD Test 
;///////////////////////////////////////////////////////////////////////////////

;    IF (!@def(resources))
;resources equ 1

LCD_SEG_OFFSET equ      $000000

RSRC_STRING             equ        1    ;$FILENAME string_data.src
RSRC_LCD_INIT_SEQ       equ        2    ;$FILENAME system_lcd_init_seq.src
RSRC_SPLASH_SCREEN      equ        3    ;$FILENAME st_bw1.src
RSRC_FONT_TABLE         equ        4    ;$FILENAME font_table.src
RSRC_PDM                equ        5    ;$FILENAME iso8859_PDM.src
RSRC_SDMS               equ        6    ;$FILENAME iso8859_SDMs.src
RSRC_PGM_8              equ        7    ;$FILENAME sigmatel_8_PGM.src
RSRC_SGMS_8             equ        8    ;$FILENAME sigmatel_8_SGMs.src
RSRC_SCRIPTS_8          equ        9    ;$FILENAME sigmatel_8_scripts.src
RSRC_SCRIPT_00_8        equ       10    ;$FILENAME sigmatel_8_script_00.src
RSRC_PDM_2              equ       11    ;$FILENAME stmp1_PDM.src  
RSRC_SDMS_2             equ       12    ;$FILENAME stmp1_SDMs.src 
RSRC_PGM_2              equ       13    ;$FILENAME stmp1_PGM.src
RSRC_SGMS_2             equ       14    ;$FILENAME stmp1_SGMs.src
RSRC_SCRIPTS_2          equ       15    ;$FILENAME stmp1_scripts.src
RSRC_SCRIPT_00_2        equ       16    ;$FILENAME stmp1_script_00.src
RSRC_DISPLAY_MODULE     equ    $ffff    ;bogus number inserted to make it compile fine, doesn't have a file associated

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

;    ENDIF ;IF (!@def(resources))


