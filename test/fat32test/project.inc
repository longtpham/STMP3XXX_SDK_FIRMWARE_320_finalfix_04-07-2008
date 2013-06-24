;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2004
;
;  Filename: project.inc
;  Description: 
;///////////////////////////////////////////////////////////////////////////////

    if (!@def(_PROJECT_INC))
_PROJECT_INC                equ     1

 include "hwequ.inc"

;///////////////////////////////////////////////////////////////////////////////
; BOOT SPEED CONFIG & ASSOCIATED BATTERY VOLTAGE CHECK ENABLE
;///////////////////////////////////////////////////////////////////////////////
; Users can enable one of ATTEMPT_FAST_BOOT or FASTEST boot or neither project define below by defining
; values as 0 or 1. 0 results in the boot speed used in previous SDKs.
ATTEMPT_FAST_BOOT    equ 0
ATTEMPT_FASTEST_BOOT equ 0   
; Changing this define to 0 removes Vbat threshholding & unconditionally gives fast boot config above- 
; Also the tests should clear the fast boot config defines to link & boot at normal speed.
SPEED_BOOT_BATTERY_CHECK    equ 0


 
;///////////////////////////////////////////////////////////////////////////////
; media devices
NUM_REMOVABLE_MEDIA         equ     1


;///////////////////////////////////////////////////////////////////////////////
; SmartMedia/NAND defs
SM_MAX_PHYSICAL_DEVICES     equ     1
SM_MAX_PARTITIONS           equ     4
SM_INTERNAL_CHIPS           equ     1
SM_REMOVABLE_DEVICE_NUM     equ     1
; Number of removable SmartMedia
NUM_SM_EXTERNAL		    equ	    0

NUM_LOGICAL_MEDIA           equ     1

;///////////////////////////////////////////////////////////////////////////////
; Sysloadresources defs
SM_MAX_RESOURCE_DEVICES     equ     1

;///////////////////////////////////////////////////////////////////////////////
; MMC defs
MMC_MAX_PHYSICAL_DEVICES    equ     1
MMC_MAX_PARTITIONS          equ     1
MMC_MAX_LOGICAL_DEVICES     equ     MMC_MAX_PHYSICAL_DEVICES*MMC_MAX_PARTITIONS

;///////////////////////////////////////////////////////////////////////////////
; SPI defs
SPI_MAX_DEVICES             equ     MMC_MAX_PHYSICAL_DEVICES

;///////////////////////////////////////////////////////////////////////////////
; Global media defs
MAX_PHYSICAL_DEVICES        equ     SM_MAX_PHYSICAL_DEVICES+MMC_MAX_PHYSICAL_DEVICES
MAX_LOGICAL_DEVICES         equ     SM_MAX_PHYSICAL_DEVICES+MMC_MAX_LOGICAL_DEVICES
MAX_HANDLES                 equ     2
MAX_REMOVABLE_DEVICES	    equ	    NUM_SM_EXTERNAL+MMC_MAX_PHYSICAL_DEVICES

;//////////////////////////
  if (@def('DEMO_HW'))
 ; this is the setting for ET301 demos; Next 2 line have not been updated for new Shingyi LCD (June6'05)
 LCD_MAX_CONTRAST    equ $32 
 LCD_MIN_CONTRAST    equ $1E
  else
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


;////////////////////////////////////////////////////////////////////////////////
; The default value of the lcd contrast in % of range
;   the default value is used when no settings.dat is available and for the dcc
;////////////////////////////////////////////////////////////////////////////////
DEFAULT_CONTRAST    equ  40 ; % of range is default value


    endif ; if (!@def(_PROJECT_INC))
