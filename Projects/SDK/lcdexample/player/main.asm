;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2006
;
;  File        : main.asm
;  Description : LCD based reference design (Doesn't support MMC/SD. Supports DRM9 & DRM10)
;///////////////////////////////////////////////////////////////////////////////

    section     EXEC_main

;///////////////////////////////////////////////////////////////////////////////
;  Macros
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "sysmacro.asm"
    list

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "hwequ.inc"
    include "sysequ.inc"
    include "sysspeed.inc"
    include "sysresources.inc"
    include "buttons.inc"
    include "resource.inc"
    include "msgequ.inc"
    include "sysresources.inc"
    include "MediaDetection.inc"
    include "systemdefs.inc"
    include "systimers.inc"  
    list

;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

    global  Exec
    global  EmptyFunction

    ; The constants below in resource.inc, are reference as extern by other modules.
    ; Therefore, they are made global here to allow the locater to resolve the references

    ; The constants below in systimers.inc, are referenced as extern by systimers.xref.
    ; Therefore, they are made global here to allow the locater to resolve the references
    global  DECODER_TIMER_NUM
    global  BUTTON_TIMER_NUM 
    global  SYS_TIMER_NUM  
    
    global  RemovableMediaTable
    global  FRemovableMediaTable

    ; required for syscoder to link
    global  EncoderResourcePtr  
    global  MODULE_NUM_CAPTURE       
    global  CaptureQueueDescriptor  
    global  MsgEncoderStop          
    global  EncoderProcessPtr       

    global  VERSION_MAJOR,VERSION_MIDDLE,VERSION_MINOR
    global  Fg_iMediaErrorFlag 
;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "sysmisc.xref"
    include "stackmem.xref"
    include "systime.xref"
    include "sysirq.xref"
    include "button.xref"
    include "const.xref"
    list

    extern    SysResetAllTimers
    extern    SysDefaultAllVectors    
    extern    SysErrorInit
    ; These are in SysSpeed.asm
    extern    SysSpeedSysStartUp
    extern    SysSpeedResetAPI
    extern    SysSpeedBoot
    extern    SysSpeedSysShutDown
    ; These are in SysExecSpeedClient.asm
    extern    SetExecState
    extern    ExecSpeedAndBtnCheck
	extern    FLoadUserTime

    extern    FMediaInit
    extern    FMediaDiscoverAllocation
    extern    FDriveInit
    extern    SysLoadFATWrite
    extern    SysUnLoadFATWrite
    extern    FComputefreeluster
    ; these are moved to syshwinit.asm
    extern    PowerInit
    extern    AnalogInit 

    extern    F_lc_u_eRestoreSysDrive_P

    ; these are moved to sysusb.asm
    extern    DetectUsb
    extern    UsbDetectInit
    extern    FServiceDCDC
	extern    FInit5VSense

    ; moved to sysirq.asm
    extern    IcollClear
        
    ; moved to sysvoice.asm
;    extern    InitVoiceNumber          
    
    extern    InitModules
    extern    ExecuteModules

    extern    ModuleSetTimer

    
    extern    FDisplaySplashScreen
    extern    SysPostMessage

  if @def('MMC')
;    extern    FInitRemovableMedia
    extern    FCheckRemovableMedia
	extern    FMMCInitDetectionPins
  endif

    extern    CheckHeadPhoneShort

    extern    SysLoadSettings

    extern    SysSpeedSysShutDown

 if (@DEF('FUNCLET'))           
    extern    SysCallFunclet
 endif

    extern    FMiscProjectInit
    extern    FFSInit
    extern    InitRTC

    extern    Fbufx
    extern    Fbufy
    extern    Fmaxdevices
    extern    Fmaxhandles
    extern    Fmaxcaches

   global   FileSystemMode
   global   Fg_wFileSystemMode,FglbwFileSystemMode



    extern    CheckDiskLoad
    extern    FTestSkipCheckDisk
    if !@def('D3410')
    extern    FStickyBitInit
    endif

	extern	  FChipUpdateCapability
    extern    FChipGetCustID
    
    extern    FSysSetResourceTag
    extern    Fg_wPlayerResourceTag
    extern    FFSDriveInit
  
    extern    FSetCWDHandle
    if @def('JANUS')   
    extern    Fg_JanusDrive
    extern    JanusInit
    endif
    extern    FSysGetDesiredResourceTag
    extern    FRestoreDriveFromBackup

;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////
   
;///////////////////////////////////////////////////////////////////////////////
;   X Memory
;///////////////////////////////////////////////////////////////////////////////

    org     x,"EXEC_main_X":

;///////////////////////////////////////////////////////////////////////////////
;   Y Memory
;///////////////////////////////////////////////////////////////////////////////

    org     y,"EXEC_main_Y":


SmartMediaInserted      dc  0       ;Bit 0 -> 0 : Smart Media Not Present
                                        ;      -> 1 : Smart Media Present  
                                        ;Bit 1 -> 0 : No Debounce In Progress
                                        ;         1 : Debounce In Progress
PreviousTimeHigh        dc  0       ;Starting time for smart media sense debounce process
PreviousTimeLow         dc  0

Fg_iMediaErrorFlag  dc  0

Fg_wFileSystemMode                 ;For C usage
FglbwFileSystemMode		   ;Fattest uses
FileSystemMode      dc  0
DeviceNum           ds 1

;///////////////////////////////////////////////////////////////////////////////
;   P Memory
;///////////////////////////////////////////////////////////////////////////////
    org     p,"EXEC_Vectors_P":

    jmp     Exec

    org    p,"EXEC_main_P":

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           Exec
;   Type:           Function
;   Description:    Static entry point for the player code (LCD ver). This is also
;                   the player main loop
;   Inputs:         none
;   Outputs:        none
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
Exec
    
 comment ~    
; used for timing regs
      bset #HW_GP_B16_BITPOS,x:HW_GP0PWR    ; pwrpads
      bset #HW_GP_B16_BITPOS,x:HW_GP0ENR    ; enable
      bset #HW_GP_B16_BITPOS,x:HW_GP0DOER   ; output
      bset #HW_GP_B16_BITPOS,x:HW_GP0DOR    ; make it high 


      bset #HW_GP_B17_BITPOS,x:HW_GP0PWR    ; pwrpads
      bset #HW_GP_B17_BITPOS,x:HW_GP0ENR    ; enable
      bset #HW_GP_B17_BITPOS,x:HW_GP0DOER   ; output
      bclr #HW_GP_B17_BITPOS,x:HW_GP0DOR    ; make it low 
 
 ;STMP 9222/9223 use B15 as CPLD related to STFM1000 development is no longer needed
      ; STFM CPLD enable/disable.  This should be setup as a GPIO input ASAP after bootup
      bclr #HW_GP_B15_BITPOS,x:HW_GP1DOER    ; make it an input/tri-state
      bset #HW_GP_B15_BITPOS,x:HW_GP1ENR    ; enable as GPIO
      bclr #HW_GP_B15_BITPOS,x:HW_GP1DOR    ; default to low when enabled
~

; DEBUG ONLY - HW_GPIO_036 36-24 = 12 => HW_GP_B12_BITPOS on HW_GP1*
;       bset #HW_GP_B12_BITPOS,x:HW_GP1PWR    ; pwrpads
;       bset #HW_GP_B12_BITPOS,x:HW_GP1ENR    ; enable
;       bset #HW_GP_B12_BITPOS,x:HW_GP1DOER   ; output
;       bclr #HW_GP_B12_BITPOS,x:HW_GP1DOR    ; make it low  


   ; ReEnable ONCE for 3410/35xx debug build. Allows improved debugging 
   ; of 3410/35xx. In debugger, type reset and go instead of reloading abs repeatedly. 
   ;
   if (@def('DEBUG')) ; Better to ReadModifyWrite.
    move    #>$ff8fff,x0                    
    move    x0,x:$fffe
   endif

    ; Init the Stack Pointer
    move    y:<Const_000000,sp

    ; init the L1 data stack
    move    #L1DataStack,r7
    move    y:<Const_ffffff,m7
    

    ; Prefill the entire vector table with defaults per build type. 
    jsr     SysDefaultAllVectors
    jsr     SysErrorInit                     ; Init vectors for the fatal errors

	jsr		FChipUpdateCapability			 ; ChipUpdateCapability Initializes chip hardware capabilities
        
    jsr     PowerInit                        ; Must be called before SysSpeedSysStartUp
    jsr     SysResetAllTimers                ; Must be called before SysSpeedSysStartUp
    jsr     SysAllLinear
   
    ; Imported to be called here ealy so we can use persistant bits early.  may 19'06.
    jsr     FStickyBitInit ; StickyBitInit removes SoftReset so copy controller can run. What else is needed to read sticky bit? 
  
  ;debug ; A good boxview starting point. Uncomment this to use boxview with player. 

    ; IF PERSISTANT BIT SAYS RUN FROM 2ND RESOURCE DRIVE, SET TAG NOW TO THE TAG FOR EMERGENCY VER OF RESOURCE DRIVE. 
    jsr     FSysGetDesiredResourceTag ; SysGetDesiredResourceTag returns tag in A1 as required for input to SysSetResourceTag
    jsr     FSysSetResourceTag ; SysSetResourceTag sets working resource tag, and clear resource drive number

    ; Init the Media
    clr     a
    jsr     FMediaInit
       ;debugne  ; halt if non zero return 

    clr     a
    jsr     FMediaDiscoverAllocation
       ;debugne  ; halt if on zero return

    ; Media Discover Allocation not required with MMC.
    
    ; init both resource.bin drives now. 2nd one used only for emergency if needed. 
    ;   See def in sysmacro.asm. Depends on ddiproject.c drive array!
    GetLogicalDriveNumForResourceBin 1
    jsr     FDriveInit  ; inits the redundant resource.bin drive specified in input register A1   
   if (@DEF('DEBUG'))   
    debugne                ; halt on error return
   endif

    GetLogicalDriveNumForResourceBin 2
    jsr     FDriveInit  ; inits the redundant resource.bin drive specified in input register A1   
   if (@DEF('DEBUG'))   
    debugne                ; halt on error return
   endif

    GetLogicalDriveNumForResourceBin 3
    jsr     FDriveInit  ; inits the master redundant resource.bin drive specified in input register A1   
   if (@DEF('DEBUG'))   
    debugne                ; halt on error return
   endif

    ; Clear all interrupts and Initialize Timer (SysSpeed requires time api)
    jsr     SysResetAllInterrupts  ; move these 2 jsr's here OUT of funclet overlay
    jsr     SysTimeInit            ; "


    jsr     StartProject    ; now a funclet and absorbed code from main

    ; --------------------  SUPER LOOP ----------------------------

_again

    ; Check for external Media insertion/removal
  if @def('MMC')  
    jsr     FCheckRemovableMedia
  endif
    
    ;Check for headphone short -- if there was an irqa, service headphones
    jsr     CheckHeadPhoneShort 
            
    ;~~~~~~~~~~~~~~~~~~~~~~~~~~~
    ; Prepare bitflags in ExecStateBits for call to ExecSpeedAndBtnCheck
    jsr     SetExecState

    ; Conditionally Increase/free the clock and notify menus on btn event for Exec
    ; y:ExecStateBits ready: only IDLE_TIMER_ACTIVE bit is modified by this call
    jsr     ExecSpeedAndBtnCheck 
    ;~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    ;ServiceDCDC should be called before DetectUSB
    jsr     FServiceDCDC
    ; Check for USB insertion
    jsr     DetectUsb

    ; Execute the modules
    jsr     ExecuteModules
    ; If no module ran, the wait instruction below effectively reduces average DSP current
    ; extending continuous audio decode battery life by 5% (1 to 2.5 hrs). 
    tst     a               
    jne     _again

    ; Suspend processor activity until next unmasked interrupt. Gates off DSP clock input.

;; Turn on the bit if the processor is idle - this can be used to determne if the clock speed can be reduced.   
;;;    bset #HW_GP_B12_BITPOS,x:HW_GP1DOR    ; Measure idle time - make it high - usual state with pullup resistor
    wait ;Removal of this instruction improves the 0dB test's non-critical 20Log(NoiseFloor) 
;;;    bclr #HW_GP_B12_BITPOS,x:HW_GP1DOR    ; make it low   

    ; A-weighted by 5dB (test excludes harmonic noise) during 1kHz 0dB MP3 test. 
    ; With wait instruction, results stay within SigmaTel audio spec.  

    ; do it forever
    jmp     _again


;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           StartProject
;
;   Type:           Function
;
;   Description:    Initialization procedures.
;
;   Inputs:         none
;
;   Outputs:        none
;                   
;   Notes:          none
;<
;///////////////////////////////////////////////////////////////////////////////
StartProject    

 if (@DEF('FUNCLET'))           

    ;If using funclet, function will be executed via funclet kernel
    CallFunclet RSRC_FUNCLET_STARTPROJECT

    org p,"SYSFUNCLET_StartProject_P":
    
    dc      RSRC_FUNCLET_STARTPROJECT           ; Funclet Resource number

 endif
	

    ;// Call the miscellaneous project init routine. 
    ;// Initializes battery monitoring (Needed before SysSpeedBoot else slower boot)
    jsr     FMiscProjectInit ; always returns SUCCESS 

	; Initialize 5V to DCDC Power Transfer
	jsr		FInit5VSense

    jsr     SysSpeedResetAPI    ; Init Speed Client API data structures
    ; Initialize voltage and speed: Must select IDLE as first stage out of reset 
    ; so correct order is performed in sysspeed.
    ; Do not use SysSpeedIncrease() for this initial speed change.
    ; Note: All other speed changes except system shutdown should use  
    ; SysSpeedIncrease() and SysSpeedClockFree() to modify speed.
    jsr     SysSpeedSysStartUp   

    ;SysSpeedBoot sets the speed to SPEED_MAX _MP3ENCODE or _PEAK (& does nothing else).
    ;We call this so that subsequent player startup calls are quicker.  
    jsr     SysSpeedBoot ; Changes fDCLK & Vddd to one of highest 3 settings. 
  
   ; This routine calls ExecuteModules, so module init will run here also.
   ; Initial contrast is default in system_lcd_init_seq.src
   ; Saved contrast will be adjusted during menus
   jsr     FDisplaySplashScreen

  if @def('MMC')
    ; Stmp00004363  Removed MediaInit(1) here
    ; Init hardware to detect presence of MMC Media
    jsr     FMMCInitDetectionPins ; void return
  endif    

    ;must call SysLoadFATWrite, because the data drive init stuff can call
    ;some items that require the write device driver to be loaded
    jsr     SysLoadFATWrite
 ;debug ; testpoint for profiling RestoreDriveFromBackup
    ;--------------------------------------------------
    ; If we've booted from the 3rd resource.bin, then one of the primary resource.bins had a resource load error
    ; so DriveErase it to remove read disturbances and restore it from the backup resource.bin.
    move    y:Fg_wPlayerResourceTag,b
asm_DRIVE_TAG_RESOURCE2_BIN equ $22 ; see ddi_ldldefs.h if you must change one, change this and that file. 
    move    #>asm_DRIVE_TAG_RESOURCE2_BIN,y1 ;check if this is the master emergency copy of resource.bin in use currently.
    cmp     y1,b
    jne     SkipSysDriveRestore ; skips if not running from master backup resource.bin
 ;debug
     
    move    #>RSRC_RESTORESYSDRIVE_P_CODE,x0   ; Resource number
    move    #>RSRC_TYPE_DATA,x1         ; Resource type is data
    move    #>(F_lc_u_eRestoreSysDrive_P-F_lc_u_ModuleRegion_b)*3,y1
    move    #>F_lc_u_ModuleRegion_b,y0    ; Address of the overlay area
    move    #TARGET_MEM_P,a1            ; Overlay area is in P memory
    jsr     SysLoadResource
    ;debug

    ; If persistant bit was set to force bootup from redundant resource drive, 
    ; we have done so. If that occurred, now erase the original resource drive 
    ; to remove read disturbance bit errors and rewrite it, clear persistant bit, 
    ; then run from original resource drive on next boot.    
    GetLogicalDriveNumForResourceBin 1
    push    a1
    GetLogicalDriveNumForResourceBin 3
    move    a1,b ; source drive in b1 as required by c compiler
    clr     a
    pop     a1   ; destination drive in a1 as required by c compiler
    jsr     FRestoreDriveFromBackup    ; RestoreDriveFromBackup.c
    ; RESTOREDRIVEFROMBACKUP CAN RETURN QUICK DUE TO LOW BATTERY CONDITION.
    jne    _RebootToRestoreMap           ; Don't reboot or try to restore other drive if restore returned err 
        ;----------- Now restore resource 2 drive from master resource 3 drive 
        GetLogicalDriveNumForResourceBin 2
        push    a1
        GetLogicalDriveNumForResourceBin 3
        move    a1,b ; source drive in b1 as required by c compiler
        clr     a
        pop     a1   ; destination drive in a1 as required by c compiler
        jsr     FRestoreDriveFromBackup    ; RestoreDriveFromBackup.c
        jne    _RebootToRestoreMap          ; 
        jsr     FMarkSystemDriveClean     ; RestoreDriveFromBackup.c::IndicateDrivesRestored REBOOTS INTENTIONALLY
        ;// Reboot from original resource.bin now
_RebootToRestoreMap
        jmp FUnconditionalReboot ; to restore the mem map after loading RSRC_RESTORESYSDRIVE_P_CODE
SkipSysDriveRestore:
    ; -------------------------------------------------------
    ; Only want to DriveInit the onboard NAND flash. 
    ; MMC datadrive DriveInit later after presence check in menus\playerstatemachine.c

    clr     A            ;initialize the DATA drive (logical Drive 0 as input param in A)
    jsr     FDriveInit
    ; Only want to initialize NAND. MMC done in menus\playerstatemachine.c

    ; --------------------------
    ; File System Init: Only want to initialize NAND (Drive 0). MMC done in menus.
    ; In previous asm FSInit b=0 meant init both media. Now it inits up to maxdevices parm
    clr     b                  
    move    #Fbufx,r0
    move    #Fbufy,r4
;; Feb 12 2007: always use the Fmaxdevices since FSInit only prepare buffers for file system
;  if @def('MMC')  ;
;	; FSInit only the NAND data drive. MMC data drive is FSInit in menus\playerstatemachine.c
;    move    #>1,A	; Num devices/datadrives for FSInit to consider
;  else	
    move    y:Fmaxdevices,A
;  endif	 
    move    y:Fmaxhandles,B
    move    y:Fmaxcaches,x0
    jsr     FFSInit    					; Initialize file system buffers
    clr     A    
    jsr     FFSDriveInit
    jcc     _NoErrorOnMedia             ;If carry bit no set, will continue
    bset    #0,Fg_iMediaErrorFlag       ;Set error flag
   ;debug ; error occurred during FSInit. Is there an err code in A?
_NoErrorOnMedia
   ;----------------------------
;    ; Exported to be called ealier than here so we can use persist bits earlier.  may 19'06.
;    ; Initialize the Sticky Bit copy controller for NON 3410 builds.
;    if !@def('D3410')
;     jsr     FStickyBitInit
;    endif      
    ;----------------------------
    ; These next four lines were necessary for the MTP db sticky bit to transfer across applications
    bclr    #3,X:HW_RTC_PERSIST0   ; May 2006 warning: this operation is less robust than calling API ClearStickyBit
    ; For the 35xx...
    ; Enable XTAL on power down, to preserve RTC value and persistent bits
    bset    #5,X:HW_DCDC_PERSIST             ; Set the SLEEP_XTAL_ENABLE bit in the DCDC persistent shadow register
    bset    #10,X:HW_DCDC_PERSIST            ; Update the master DCDC persistent register in the clock domain
    bclr    #10,X:HW_DCDC_PERSIST
    ;-----------------------
 if @def('TUNER_STFM1000')   
; if !@def('SUPPORT_MARGINAL_XTALS')
 if 0 ;By default, disable this power savings which causes boot failure for marginal xtal cases without stfm present. 
    ; now only do this in stfm1000 boards since marginal crystals don't produce a solid clock when this segment is present. 
    ; This segment's purpose is only to help stfm1000 boards save power, and its by DW and JaH. 
    ; STFM1000 is enabled by default in the sdk. If you don't have stfm1000, you can remove these lines by enabling 
    ; this marginal define in lcdexample\project.inc to work with marginal low voltage xtals. It's enabled by default. stmp00012148
    ; Set DCDC_PERSIST register bit[2:1], we have to set XTAL_BIAS_DOWN1 first, then XTAL_BIAS_DOWN0
    bset    #2,X:HW_DCDC_PERSIST             ; Set the XTAL_BIAS_DOWN1
    bset    #10,X:HW_DCDC_PERSIST            ; Update the master DCDC persistent register in the clock domain
    bclr    #10,X:HW_DCDC_PERSIST
    bset    #1,X:HW_DCDC_PERSIST             ; Set the XTAL_BIAS_DOWN0
    bset    #10,X:HW_DCDC_PERSIST            ; Update the master DCDC persistent register in the clock domain
    bclr    #10,X:HW_DCDC_PERSIST
 endif
 endif
    ;-----------------------
    ; Inserted may 10 2006 as patch for released SDK 2.611 and 2.610 to make clock signal from stfm1000 to 35xx more stable in liion case. 
    ; Any later  2.6xx sdk will include this solution. 
    ; This 3 line insertion is PCN25. An SDK3.110 patch was also done and this was integrated into SDK3.120 during its development. Any later SDK3.x>=3.111 will have this.
 if @def('TUNER_STFM1000') 
  if @def('BATTERY_TYPE_LI_ION')    
    bset    #4,X:HW_DCDC_PERSIST            ; Set XTAL_TRIM1 to Add 4pf cap to XTALo/XTALi
    bset    #10,X:HW_DCDC_PERSIST           ; For LIION build only with STFM1000 FM device
    bclr    #10,X:HW_DCDC_PERSIST           ; This fixes trap to Illegal Instruction when Recording FM
  endif
 endif      
    
    clr     a                           ; 0 is nand flash
    jsr     FTestSkipCheckDisk 

	clr A
	jsr     FSetCWDHandle
	
    ; No MMC CheckDisk until MMC media init in playerstatemachine.
    ;----------------------------
    ; Compute free clusters only for device 0. Multinand also only needs 1 call.    

	clr A
	jsr  FComputefreeluster
    ; in sdk 2.6xx, MMC Computefreeluster is called in playerstatemachine, not here in main.asm. 
    ;-----------------------------
    ;the follow has been commented to because it is now being saved in settings.dat
	jsr     FLoadUserTime   ; Load User time may create file, if it is absent.

    ;unload the FATWrite (since we no longer need the datadrive write driver loaded
    ;FATWrite was needed for checkdisk since we may have had to write the FAT table / datadrive.
    ; Call SysUnLoadFATWrite before loading Janus (stmp6689)
	jsr  SysUnLoadFATWrite
    ;-----------------------------
    ; debug ;testpoint
    if @def('JANUS') 
        jsr     LoadJanusSystem    ; janus DRM10 not supported in sdk 2.6xx
        jsr     LoadJanusYData
        clr     a
		if @def('MMC')
			move    #>2,a 
		else
			move    #>1,a 
		endif
        jsr     JanusInit
    endif
    ;-----------------------------    
    jsr     SysLoadSettings ; loads static data values from hidden attribute file settings.dat on user data drive



;    bset #HW_GP_B16_BITPOS,x:HW_GP0DOR

    ;set the speed back to idle, so that analog init doesn't get screwed up
    jsr     SysSpeedSysStartUp

    ; Init the buttons
    jsr     ButtonInit
    
    ; turn on the buttons
    jsr     ButtonsOn
	
    ; considered moving earlier may 19 2006 but had to stay here since its a funclet (needs resource.bin DriveInit 1st)
    ; Init the Real Time Clock  (has a 1msec counter of size 48 bits)
    jsr     InitRTC
    
    ; Run some more inits
    ;jsr     AnalogInit  ;stmp7523 use funclet ver instead since semi-static locator may not have occurred.
    CallFunclet RSRC_FUNCLET_ANALOGINIT 
 

    jsr     UsbDetectInit
    ;-----------------------------
    ; GP32 or CF_A0. This used as MMC_DETECT bit. STMP9220 Let pin float high to reduce USB suspend current
    if !@def('MMC') 
      	bclr #HW_GP_B8_BITPOS,x:HW_GP1ENR	;; tri-state the pin
    endif

    rts


 if (@DEF('FUNCLET'))
 
    ;If using funclet, switch counter back to original here
    org    p,"EXEC_main_P":                             

 endif


;///////////////////////////////////////////////////////////////////////////////
;>  Name:           EmptyFunction
;   Type:           Function
;   Description:    This function does nothing
;   Inputs:         none
;   Outputs:        none
;   Notes:          If a module does not have a background processing
;                   routine, then the address of this function is put
;                   in the module table entry.
;<
;///////////////////////////////////////////////////////////////////////////////

EmptyFunction
    rts





    endsec
