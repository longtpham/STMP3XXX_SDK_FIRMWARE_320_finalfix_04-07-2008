;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000
;
; FAT Test Resource stuff
;///////////////////////////////////////////////////////////////////////////////

    IF (!@def(resources))
resources equ 1

LCD_SEG_OFFSET equ      $000000 
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
RSRC_NANDMEDIAINIT                      equ     9 ;$FILENAME mediastartup.src
RSRC_NANDDATADRIVEINIT                  equ     10 ;$FILENAME nanddatadriveinit.src
RSRC_NANDDATADRIVEREADSECTOR            equ     11 ;$FILENAME nanddatadrivereadsector.src
RSRC_MMCMEDIAINIT                      equ     12 ;$FILENAME mmcmediastartup.src
RSRC_MMCMEDIAGETINFO                   equ     13 ;$FILENAME mmcinfo.src
RSRC_MMCMEDIAERASE                     equ     14 ;$FILENAME mmcerase.src
RSRC_MMCDATADRIVEREADSECTOR            equ     15 ;$FILENAME mmcreadwrite.src
RSRC_MMCENUMERATE                      equ     16 ;$FILENAME mmcenumerate.src
RSRC_MMC_RESETDEVICE                   equ     17 ;$FILENAME mmcresetdevice.src
RSRC_MMC_PROCESSCSD                    equ     18 ;$FILENAME mmcprocesscsd.src
RSRC_MMC_PROCESSCID                    equ     19 ;$FILENAME mmcprocesscid.src
RSRC_MMC_DETECTPRESENCE                equ     20 ;$FILENAME mmcdetectpresence.src
RSRC_DISPLAY_MODULE     equ    $ffff    ;BOGUS number that isn't ever used for this application.

 
RSRC_NANDMEDIAALLOCATE                  equ     0
RSRC_NANDMEDIADISCOVERALLOCATION        equ     RSRC_NANDMEDIAINIT
RSRC_NANDMEDIAGETMEDIATABLE             equ     RSRC_NANDMEDIAINIT
RSRC_NANDMEDIAGETINFO                   equ     RSRC_NANDMEDIAINIT
RSRC_NANDMEDIAERASE                     equ     0

RSRC_NANDDATADRIVEGETINFO               equ     RSRC_NANDDATADRIVEREADSECTOR
RSRC_NANDDATADRIVESETINFO               equ     0
RSRC_NANDDATADRIVEWRITESECTOR           equ     RSRC_NANDDATADRIVEREADSECTOR
RSRC_NANDDATADRIVEMULTIWRITESETUP       equ     0 
RSRC_NANDDATADRIVEMULTIWRITESECTOR      equ     0 
RSRC_NANDDATADRIVEERASE                 equ     0 
RSRC_NANDDATADRIVEFLUSH                 equ     RSRC_NANDDATADRIVEREADSECTOR


RSRC_NANDSYSTEMDRIVEINIT                equ     RSRC_NANDMEDIAINIT 
RSRC_NANDSYSTEMDRIVEGETINFO             equ     RSRC_NANDMEDIAINIT  
RSRC_NANDSYSTEMDRIVESETINFO             equ     0  
RSRC_NANDSYSTEMDRIVEREADSECTOR          equ     0 
RSRC_NANDSYSTEMDRIVEWRITESECTOR         equ     0 
RSRC_NANDSYSTEMDRIVEMULTIWRITESETUP     equ     0 
RSRC_NANDSYSTEMDRIVEMULTIWRITESECTOR    equ     0 
RSRC_NANDSYSTEMDRIVEERASE               equ     0 
RSRC_NANDSYSTEMDRIVEFLUSH               equ     0 


RSRC_MMCMEDIAALLOCATE                  equ     RSRC_MMCMEDIAINIT
RSRC_MMCMEDIADISCOVERALLOCATION        equ     RSRC_MMCMEDIAINIT
RSRC_MMCMEDIAGETMEDIATABLE             equ     RSRC_MMCMEDIAGETINFO

RSRC_MMCDATADRIVEINIT                  equ     RSRC_MMCMEDIAINIT 
RSRC_MMCDATADRIVEGETINFO               equ     RSRC_MMCMEDIAGETINFO
RSRC_MMCDATADRIVESETINFO               equ     RSRC_MMCMEDIAGETINFO
RSRC_MMCDATADRIVEWRITESECTOR           equ     RSRC_MMCDATADRIVEREADSECTOR
RSRC_MMCDATADRIVEMULTIWRITESETUP       equ     RSRC_MMCDATADRIVEREADSECTOR 
RSRC_MMCDATADRIVEMULTIWRITESECTOR      equ     RSRC_MMCDATADRIVEREADSECTOR 
RSRC_MMCDATADRIVEERASE                 equ     RSRC_MMCMEDIAERASE 
RSRC_MMCDATADRIVEFLUSH                 equ     RSRC_MMCMEDIAINIT

RSRC_FUNCLET_MMCPROCESSCID2            equ     0

RSRC_FUNCLET_CHIPSERIALNUMBERINIT          equ 0
RSRC_FUNCLET_INTERNALMEDIASERIALNUMBERINIT equ 0
RSRC_FUNCLET_CHIPGETSERIALNUMBER           equ 0
RSRC_FUNCLET_CREATENULLSERIALNUMBER        equ 0
RSRC_FUNCLET_CONVERTHEXTOASCII             equ 0

RSRC_MMCSERIALNUMBERINIT            equ     0 	
RSRC_MMC_CHECKWRITEPROTECT 			equ     0 	

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

