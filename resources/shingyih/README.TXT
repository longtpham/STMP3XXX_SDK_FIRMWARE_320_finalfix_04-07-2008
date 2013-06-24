About LCD init file and contrast setting.   
WARNING: if your new LCD is blank or unreliable, check your physical ribbon cable connections (clamp type connector).
------------------------------------------------------------------------------
1. LCD init file select:
In \resources\shingyih\ the default LCD init sequence file supports the G098064-41 LCD module (engr board revH lcd card). 
Note that the \resources\shingyih\system_lcd_init_seq.src (working copy) is the new ver.
You can also replace this init file from subdirectory \Old_Type_LCD\ if you have quantities of the previous default 
LCD & if you see issues with the new init sequence (unexpected).
The default init file is now for the NEW LCD but also works for the OLD LCD.
------------------------------------------------------------------------------
2. LCD contrast value setting
The LCD_MAX_CONTRAST/LCD_MIN_CONTRAST should be set 
\Mp3Firmware\Projects\SDK\lcdexample\project.inc and in several standalone tests.

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

;////////////////////////////////////////////////////////////////////////////////
; The default value of the lcd contrast in % of range
;   the default value is used when no settings.dat is available
;////////////////////////////////////////////////////////////////////////////////
DEFAULT_CONTRAST    equ  40 ; % of range is default value (was 60%)


;Rebuild the player firmware with the "ALL" command line build argument.  


