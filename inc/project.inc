;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2001
;
;  Filename: project.inc
;  Description: 
;///////////////////////////////////////////////////////////////////////////////

    if (!@def(_PROJECT_INC))
_PROJECT_INC                equ     1

    include "hwequ.inc"
;///////////////////////////////////////////////////////////////////////////////
; SmartMedia/NAND defs
SM_MAX_PHYSICAL_DEVICES     equ     1           ;This is max physical devices. 
                                                ;1 Internal devices can be made of
                                                ;several chips
NUM_SM_EXTERNAL		        equ	    0
SM_MAX_PARTITIONS           equ     4
SM_INTERNAL_CHIPS           equ     1           ;Number of chips forming the
                                                ;internal physical device
SM_REMOVABLE_DEVICE_NUM     equ     1

; If SMARTMEDIA_DETECT_ACTIVE_HIGH is defined:
;   SmartMedia will be detected as inserted if the GPIO pin reads 1 and removed if it reads 0.  
; If SMARTMEDIA_DETECT_ACTIVE_HIGH is not defined ** comment out next line **
;   SmartMedia will be detected as inserted if the GPIO pin reads 0 and removed if it reads 1.
;*** comment out if active high ****
;SMARTMEDIA_DETECT_ACTIVE_HIGH  equ     1

;///////////////////////////////////////////////////////////////////////////////
; Sysloadresources defs
SM_MAX_RESOURCE_DEVICES     equ     2

;///////////////////////////////////////////////////////////////////////////////
; MMC defs
MMC_MAX_PHYSICAL_DEVICES    equ     0
MMC_MAX_PARTITIONS          equ     0
MMC_MAX_LOGICAL_DEVICES     equ     MMC_MAX_PHYSICAL_DEVICES*MMC_MAX_PARTITIONS

;///////////////////////////////////////////////////////////////////////////////
; SPI defs
SPI_MAX_DEVICES             equ     MMC_MAX_PHYSICAL_DEVICES

;///////////////////////////////////////////////////////////////////////////////
; Global media defs
MAX_PHYSICAL_DEVICES        equ     SM_MAX_PHYSICAL_DEVICES+SM_MAX_PHYSICAL_DEVICES
MAX_LOGICAL_DEVICES         equ     SM_MAX_PHYSICAL_DEVICES+MMC_MAX_LOGICAL_DEVICES
MAX_HANDLES                 equ     2


;//////////////////////////
 if (@def('DEMO_HW'))
; this is the setting for ET301 demos
LCD_MAX_CONTRAST  	equ $32	
LCD_MIN_CONTRAST  	equ $1E
 else
 if (@def('LDS514_LCD'))
LCD_MAX_CONTRAST	equ 210
LCD_MIN_CONTRAST	equ 160	
 endif

 if (@def('ILI814_LCD'))
LCD_MAX_CONTRAST	equ 210
LCD_MIN_CONTRAST	equ 160	
 endif

 if (@def('ML9341_LCD'))
LCD_MAX_CONTRAST	equ 210
LCD_MIN_CONTRAST	equ 160	
 endif

 if (@def('SSD1332_LCD'))
LCD_MAX_CONTRAST	equ 210
LCD_MIN_CONTRAST	equ 160	
 endif

 if (@def('S6B33B0A_LCD'))
LCD_MAX_CONTRAST	equ 210
LCD_MIN_CONTRAST	equ 160	
 endif

 if (@def('SED15XX_LCD'))
; Next 3 line comment was for OLD shingyi LCD: (default LCD before sdk2.6)
; Engineering board regs support range [17-37].
;   Engineering board DC/DC support range [24-46]. 
;   One default contrast range [24-42] works for both.
;
; The 3 sets of contrast ranges below are updated for SDK2.6 to support either 
; new or old ShingYih LCDs. "File" refers to \resource\shingyih\system_lcd_init_seq.src
;
; 3/10/05 - Use one of the 2 next contrast ranges depending on which
; ShingYih LCD you have.  See \resources\shingyi\readme.txt 
;
; Optimal for NEW LCD with NEW file (SDK2.6 default)  
; G098064-41 LCD module (present on engr board revH LCD card)
LCD_MAX_CONTRAST equ 39
LCD_MIN_CONTRAST equ 17
;-----
; Near optimal for OLD LCD with NEW file. 
; NOT optimal for the new lcd but pretty good. So u may use this if u want 1 build for both LCDs.
;LCD_MAX_CONTRAST equ 32
;LCD_MIN_CONTRAST equ 14

;=====
; Optimal for OLD ShingYih LCD with OLD file (as sdk2.521)
; Suggest going with default new init file & values above for your LCD instead of this historic ver.
;LCD_MAX_CONTRAST equ 42
;LCD_MIN_CONTRAST equ 24 
 endif

 endif

;////////////////////////////////////////////////////////////////////////////////
; The default value of the lcd contrast in % of range
;   the default value is used when no settings.dat is available
;////////////////////////////////////////////////////////////////////////////////
DEFAULT_CONTRAST    equ  40 ; % of range is default value




;///////////////////////////////////////////////////////////////////////////////
; FMTUNER GPIO defines
; Select either GP51 or GP37 for FM_WE
FM_WE_on_GP37	equ	1
;FM_WE_on_GP51	equ	1

    if (@def('REVB_ENGR_BD'))
    include "hwequ.inc"
HW_FM_TUNER_DATA_ENABLE_R               equ HW_GP0ENR
HW_FM_TUNER_CLOCK_ENABLE_R              equ HW_GP0ENR
HW_FM_TUNER_WE_ENABLE_R               	equ HW_GP1ENR

FM_DATA_bitpos          		equ     19 
FM_CLK_bitpos           		equ     18      
FM_WE_bitpos            		equ     13
   endif

FM_BAND_LOW_LIMIT	equ	87400
FM_BAND_HIGH_LIMIT	equ	108100
FM_SCAN_LOW_LIMIT	equ	87500
FM_SCAN_HIGH_LIMIT	equ	108000
FM_STATION_LEVEL_OK	equ	5
FM_TUNER_IF_COUNT_HIGH	equ	62
FM_TUNER_IF_COUNT_LOW	equ	49
FM_TUNER_XTAL		equ	1

NUM_LOGICAL_MEDIA       equ     1

    endif ; if (!@def(_PROJECT_INC))
