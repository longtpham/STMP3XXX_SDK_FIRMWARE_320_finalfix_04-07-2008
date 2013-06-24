;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2003
;
;  File        : syssettings.asm
;  Description : System Settings Routines
;  Notes       : This file contains 3 different P memory sections.
;                SYSFUN_SysSettings_Sys_P is in system memory (always resident)
;                SYSAPP_SysSettings_Init_P & SYSAPP_SysSettings_Shutdown_P are 
;                  in application space (overlaid)                
;///////////////////////////////////////////////////////////////////////////////

    section SYSFUN_SysSettings

    opt     mex

;///////////////////////////////////////////////////////////////////////////////
;   Macros
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "sysmacro.asm"
    list

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////

    nolist
	include "project.inc"
    include "systemdefs.inc"
    include "sysresources.inc"
    include "resource.inc"
    include "sysequ.inc"
    include "filesystem.inc"    ; file system error codes
    list


;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

    global    SysLoadSettings
    global    SysSaveSettings
    global    Fg_iSysSaveSettingsDone

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "const.xref"
    list
    extern    FSysBatteryGetLevel
    extern    FileSystemMode
    extern    SysLoadResource
    extern    SaveSystemStack
    extern    RestoreSystemStack
    extern    SysFlushCoderMemory
    
    extern    FILECreate
    extern    FILEOpen
    extern    FileRead
    extern    FileWrite
    extern    FILEClose
    extern    FILEAttribute
    extern    FILEAttributeClear
    extern    FILEDelete
    extern    FDriveFlush
    extern    UserScratchY
    extern    FFlushCache
	extern    FSaveUserTime

    ; locator generated labels used when loading resources    

 if (@DEF('FUNCLET'))           
    extern    SysCallFunclet
 endif
    extern SysLoadFATWrite
    extern SysUnLoadFATWrite
;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////

; Store the system settings on this device
Device          equ     0        
                
; File attributes
READONLY        equ     $000001
HIDDEN          equ     $000002
SYSTEM          equ     $000004    

READ            equ     1
WRITE           equ     2
; Settings Commands (These follow the filesystem complex pointer scheme)
SAVE_RANGE_MASK equ     $F00000
SAVE_RANGE_ERR  equ     $000000
SAVE_RANGE_P    equ     $100000
SAVE_RANGE_L    equ     $200000
SAVE_RANGE_Y    equ     $400000
SAVE_RANGE_X    equ     $800000
SAVE_RANGE_END  equ     $F00000


    if (!@def(BATT_SAFETY_MARGIN))
; BATT_SAFETY_MARGIN:  percentage value used in SysSaveSettings to determine
;   if enough power is left to attempt a settings save
BATT_SAFETY_MARGIN  equ     10
    endif 

;///////////////////////////////////////////////////////////////////////////////
;  X memory
;///////////////////////////////////////////////////////////////////////////////

    org x,"SYSFUN_SysSettings_X":

;///////////////////////////////////////////////////////////////////////////////
;  Y memory
;///////////////////////////////////////////////////////////////////////////////

    org y,"SYSFUN_SysSettings_Y":

; The name of the file to store the system settings        
;FileName  dcb   'ES'++DirectorySeparator,'ITT','SGN','AD.',0++0++'T'   ; "/SETTINGS.DAT
FileName  dcb   'ES'++DirectorySeparator,'ITT','SGN','AD.',0++0++'T'   ; "/SETTINGS.DAT
;FileName  dcb   'TES','NIT','.SG','TAD'   ; "/SETTINGS.DAT


; Buffer for FileRead and FileWrite
Buffer    dc    0
          dc    0

;Storage for the stack pointer register
OldSP     dc    0       

; Flag indicates that routine has completed.
Fg_iSysSaveSettingsDone  dc  0

    org y,"SYSFUN_SysSettings_Commands_Y":

    dc      SAVE_RANGE_END


;///////////////////////////////////////////////////////////////////////////////
;   P Memory
;///////////////////////////////////////////////////////////////////////////////

   org    p,".ptextSysSettings":                             

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysLoadSettings
;
;   Type:           Function
;
;   Description:    Opens the file settings.dat and decodes its contents to 
;                   restore memory locations based upon the contents of
;                   settings.dat.
;
;   Inputs:         The file "settings.dat" must be at the root location of
;                   device 0 media.
;
;   Outputs:        Memory locations restored to values indicated by contents
;                   of settings.dat.
;                   
;   Notes:          
;
;   SeeAlso:        SysSaveSettings, SaveRange, SaveRangeX, SaveRangeY,
;                   SaveRangeP, SaveRangeL
;<
;///////////////////////////////////////////////////////////////////////////////
SysLoadSettings
    if (@DEF('FUNCLET'))           
   
       ;If using funclet, function will be executed via funclet kernel
       CallFunclet RSRC_FUNCLET_SYSLOADSETTINGS
   
       org p,"SYSFUNCLET_SysLoadSettings":
       
       dc      RSRC_FUNCLET_SYSLOADSETTINGS       ; Funclet Resource number
   
    endif

    ; Save the system stack
    move    #_StackSaved,r6             ; setup the return address
    move    #OldSP,r5                   ; pointer to the SP local save
    jmp     SaveSystemStack             ; save the hardware stack
_StackSaved
            
    ; Open the system settings file for reading
    move    #>Device,a2
    move    #>FileName,a1
    bset    #22,a1                      ; Y memory space
    move    #>READ,a0

    jsr     FILEOpen
    jcs     _Exit
    move    a1,a2                       ; Put the handle into A2

    ; Read the software version from the systems settings file        
    move    #>6,a1
    move    #0,a0
    bset    #22,a0
    move    #>Buffer,r0
    move    y:<Const_ffffff,m0
    push    a2
    jsr     FileRead
    pop     a2
    jcs     _Close

    ; Verify that the software version matches the current software
    move    #>VERSION_MAJOR,x0
    move    y:Buffer,b
    cmp     x0,b
    jne     _Close
    move    #>VERSION_MINOR,x0
    move    y:Buffer+1,b
    cmp     x0,b
    jne     _Close

_NextCmd
    ; Read the next command from the systems settings file        
    move    #>3,a1
    move    #>0,a0
    bset    #22,a0
    move    #>Buffer,r0          ; Starting address target.
    move    y:<Const_ffffff,m0
    push    a2
    jsr     FileRead
    pop     a2
    jcs     _Close

    ; Check if it is the End command
    move    #>SAVE_RANGE_MASK,x0
    move    y:Buffer,b
    and     x0,b
    cmp     x0,b
    jeq     _Close
    
    ; Read the number of bytes from the systems settings file        
    ; Number of bytes to read from file. Must be <= 3 since 1 word left at Buffer+1.
    move    #>3,a1                  
    move    #>0,a0
    bset    #22,a0
    move    #>Buffer+1,r0               ; Word address to read file bytes into.
    move    y:<Const_ffffff,m0
    push    a2                          ; File handle
    jsr     FileRead
    pop     a2
    jcs     _Close
        
    ; Read the settings data from the systems settings file        
    move    y:Buffer,b
    move    b,r0
    move    #>$ff0000,x0
    and     x0,B
    move    B1,a0
                     ; Read the file bytes into this address. 
    ; Above line causes Y mem corruption if settings.dat is not deleted on each firmware update.
    move    y:Buffer+1,a1               ; Number of bytes to read from file
              ; Starting address target.
    move    y:<Const_ffffff,m0
    push    a2                          ; File handle
    jsr     FileRead
    pop     a2
    jcs     _Close

    jmp     _NextCmd

_Close
    ; Close the system settings file
    jsr     FILEClose

_Exit
    ; Restore the system stack
    move    #OldSP,r5                   ; pointer to the SP local save
    move    #_Return,r6                 ; setup the return address
    jmp     RestoreSystemStack          ; restore the hardware stack
_Return

    rts
                                
    org p,".ptextSysSettings":

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysSaveSettings
;
;   Type:           Function
;
;   Description:    Creates the file settings.dat and stores commands created
;                   by the SaveRange macros to settings.dat. 
;
;   Inputs:         SaveRange macros must exist for SysSaveSettings to save
;                   the commands to settings.dat.
;
;   Outputs:        A settings.dat file at the root location of device 0 media.
;
;                   
;   Notes:          Settings file creation is not attempted if deletion attempt    
;                   returns any error code other than ERR_FILE_NOT_FOUND.
;
;   SeeAlso:        SysLoadSettings, SaveRange, SaveRangeX, SaveRangeY,
;                   SaveRangeP, SaveRangeL
;<
;///////////////////////////////////////////////////////////////////////////////
SysSaveSettings
    if (@DEF('FUNCLET'))           
   
       ;If using funclet, function will be executed via funclet kernel
       CallFunclet RSRC_FUNCLET_SYSSAVESETTINGS
   
       org p,"SYSFUNCLET_SysSaveSettings":
       
       dc      RSRC_FUNCLET_SYSSAVESETTINGS       ; Funclet Resource number
   
    endif

    ; Save the system stack
    move    #_StackSaved,r6             ; setup the return address
    move    #OldSP,r5                   ; pointer to the SP local save
    jmp     SaveSystemStack             ; save the hardware stack
_StackSaved
    ; Check the battery lagniappe and if we don't have enough juice then
    ;   skip the save
    jsr     FSysBatteryGetLevel
    move    #>BATT_SAFETY_MARGIN,x0
    cmp     x0,a
   	jlt     _Exit2


    ; Load the FAT write code
    jsr     SysLoadFATWrite

            
    ; Clear the file attributes of read only, system and hidden
    move    #>Device,a2
    move    #>FileName,a1
    bset    #22,a1                      ; Y memory space
    move    #>(READONLY|SYSTEM|HIDDEN),a0
    jsr     FILEAttributeClear
                    

            
    clr a
    ; Create a new system settings file
    move    #>Device,a2
    move    #>FileName,a1
    bset    #22,a1                      ; Y memory space
    jsr     FILECreate                      
    jcs     _Exit
    move    a1,a2                       ; Put the handle into A2

    ; Put the current software version into the buffer
    move    #>VERSION_MAJOR,a0
    move    a0,y:Buffer
    move    #>VERSION_MINOR,a0
    move    a0,y:Buffer+1

    ; Write the software version to the systems settings file        
    move    #>6,a1
    move    #0,a0
    bset    #22,a0
    move    #>Buffer,r0
    move    y:<Const_ffffff,m0
    push    a2
    jsr     FileWrite
    pop     a2
    jcs     _Close

    ; Load the settings commands resource
    move    #>RSRC_SETTINGS_COMMANDS,x0 ; Resource number
    move    #>RSRC_TYPE_DATA,x1         ; Resource type is data
    move    #>SCRATCH_USER_Y_SIZE*3,y1 
    move    #>UserScratchY,y0    ; Address of the overlay area
    move    #TARGET_MEM_Y,a1            ; Overlay area is in Y memory
    push    a2
    jsr     SysLoadResource
    pop     a2

    ; Set the command pointer to the first command
    move    #>UserScratchY,r1
    move    y:<Const_ffffff,m1
        
_NextCmd        
    ; Get the next command
    move    y:(r1),b

    ; Check that the command is valid
    move    #>SAVE_RANGE_MASK,x0
    and     x0,b
    jne     _CheckEnd
    
    ; Invalid memory space for settings command
    error
    nop
    
_CheckEnd
    ; Check if this is the End command
    cmp     x0,b
    jeq     _WriteEndCommand        

    ; Write the settings command to the systems settings file        
    move    #>6,a1
    move    r1,r0
    move    #0,a0
    bset    #22,a0
    move    y:<Const_ffffff,m0
    push    r1
    push    a2
    jsr     FileWrite
    pop     a2
    pop     r1
    jcs     _Close

    ; Write the settings data to the systems settings file
    move    y:(r1)+,b
    move    b,r0
    move    #>$ff0000,x0
    and     x0,B
    move    B1,a0
    move    y:(r1)+,a1
    move    y:<Const_ffffff,m0
    push    r1
    push    a2
    jsr     FileWrite
    pop     a2
    pop     r1
    jcs     _Close
    
    jmp     _NextCmd

_WriteEndCommand
    ; Write the end command to the systems settings file        
    move    #>3,a1
    move    r1,r0
    move    #0,a0
    bset    #22,a0
    move    y:<Const_ffffff,m0
    push    a2
    jsr     FileWrite
    pop     a2

_Close
    ; Close the system settings file
    jsr     FILEClose
    
    ; Set the file attributes to read only, system and hidden
    move    #>Device,a2
    move    #>FileName,a1
    bset    #22,a1                      ; Y memory space
    move    #>(READONLY|SYSTEM|HIDDEN),a0
    jsr     FILEAttribute
    
_Exit
	
   	jsr     FSaveUserTime
   	; Unload the FAT write code
   	jsr     SysUnLoadFATWrite

_Exit2 
     ; Restore the system stack
    move    #OldSP,r5                   ; pointer to the SP local save
    move    #_Return,r6                 ; setup the return address
    jmp     RestoreSystemStack          ; restore the hardware stack

_Return

    push    a1
    move    y:<Const_000001,a1
    move    a1,Y:Fg_iSysSaveSettingsDone  
    pop     a1
    ; Flag notifies others that settings are saved. Main menu clears display now.


    rts

    if (@DEF('FUNCLET'))
    
       ;If using funclet, switch counter back to original here
       org    p,"SYSFUN_SysSettings_P":                             
   
    endif

    endsec
