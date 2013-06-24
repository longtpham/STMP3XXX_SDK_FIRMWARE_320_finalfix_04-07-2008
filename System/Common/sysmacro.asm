;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2001
;
;  File        : sysmacro.asm
;  Description : Misc System Macros
;///////////////////////////////////////////////////////////////////////////////
 IF !@DEF('SysMacro_asm')
    define   SysMacro_asm    '1'

    page    255,255,0

;///////////////////////////////////////////////////////////////////////////////
;   Macros
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////
        
;///////////////////////////////////////////////////////////////////////////////
;   X Memory
;///////////////////////////////////////////////////////////////////////////////
        
;///////////////////////////////////////////////////////////////////////////////
;   Y Memory
;///////////////////////////////////////////////////////////////////////////////
        
;///////////////////////////////////////////////////////////////////////////////
;   P Memory
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           push
;
;   Type:           Macro
;
;   Description:    Push a 24-bit value onto the data stack
;
;   Inputs:         Usage : push reg
;                   reg = any core register that is 24-bits or less
;
;   Outputs:        none
;                   
;   Notes:          For every push, there must be a corresponding pop to keep
;                   data stack balanced.
;
;   SeeAlso:        pop
;<
;///////////////////////////////////////////////////////////////////////////////
push    macro   reg
    move    reg,y:(r7)+
    endm

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           pop
;
;   Type:           Macro
;
;   Description:    Pop a 24-bit value from the data stack
;
;   Inputs:         Usage : pop reg
;                   reg = any core register that is 24-bits or less
;
;   Outputs:        none
;                   
;   Notes:          For every pop, there must be a corresponding push to keep
;                   data stack balanced.
;
;   SeeAlso:        push
;<
;///////////////////////////////////////////////////////////////////////////////
pop     macro   reg
    move    y:-(r7),reg
    endm

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           CallFunclet
;
;   Type:           Macro
;
;   Description:    Loads and Executes the funclet defined by the Resource argument
;
;   Inputs:         Resource = Resource number for the specific funclet.
;
;   Outputs:        none
;                   
;   Notes:          Jumps to SysCallFunclet.   TODO: change to a label instead of hard addresses like $80
;<
;///////////////////////////////////////////////////////////////////////////////
; if ((@DEF('STMP_BUILD_PLAYER'))||(@DEF('PLAYER'))) ; sdk3.110 and before did this. 3.120 add short jump for mtp.
 if !(@DEF('UPDATER'))
CallFunclet   macro Resource
    jsr       $80
    DC       Resource  ; 1 word
 endm    
CallFunclet2   macro Resource
 if ((@DEF('STMP_BUILD_PLAYER'))||(@DEF('PLAYER')))
    jsr       $82
    DC       Resource  ; 1 word
 else ;;; MTP
    jsr       $80
    DC       Resource  ; 1 word
 endif
    endm    
 else
CallFunclet   macro Resource
    jsr       $80
    DC       Resource  ; 1 word
    endm    
CallFunclet2   macro Resource
    jsr       $80
    DC       Resource  ; 1 word
    endm    
 endif
;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           error
;
;   Type:           Macro
;
;   Description:    Fatal system error macro.  This macro works in conjunction
;                   with contents of syserror.asm.  Depending upon the build
;                   arguments DEBUG, BETA, or RETAIL, this will either produce
;                   a "debug" statement or a JMP to an error handling reset routine.
;
;   Inputs:         none 
;   Outputs:        none                   
;   Notes:          Can use debugOrContinue or ccerror for conditional halts. 
;                   SystemHalt() C macro uses this asm macro.
;<
;///////////////////////////////////////////////////////////////////////////////
error   macro
  if (@DEF('BETA')||@DEF('RETAIL')||@DEF('TRACEBUFFER'))
    jmp $000C ;Use vector at P:000C to shutdown & restart the STMP device. 
              ; Jmp to addr in low 12 bits is 1 word smaller than jsr. (changed apr 6 '05)
  else
    debug
  endif ; if ( @DEF('BETA')||@DEF('RETAIL') 
    endm

;///////////////////////////////////////////////////////////////////////////////
;
;   Name:           debugOrContinue
;   Type:           Macro
;   Description:    System error macro. Depending upon the build
;                   arguments DEBUG, BETA, or RETAIL, this will either produce
;                   a "debug" statement or nothing to allow continued execution.
;   Inputs:         none 
;   Outputs:        none
;   Notes:          none
;
;///////////////////////////////////////////////////////////////////////////////
debugOrContinue   macro
  if (@DEF('DEBUG'))
    debug
    nop
  endif 
    endm

;///////////////////////////////////////////////////////////////////////////////
;
;   Name:           debugOrShutdown
;   Type:           Macro
;   Description:    System error macro. Depending upon the build
;                   arguments DEBUG, BETA, or RETAIL, this will either produce
;                   a "debug" statement or Prevent RETAIL build from hanging on 
;                   debug line by attempting graceful shutdown.
;   Inputs:         none 
;   Outputs:        none
;   Notes:          See SystemShutdownFinal in SystemAPI.asm
;
;///////////////////////////////////////////////////////////////////////////////
;debugOrShutdown   macro
;  if (@DEF('DEBUG'))
;    debug
;    nop
;  else  ; ( @DEF('BETA')||@DEF('RETAIL')
;    jmp SystemShutdownFinal                ; reference not resolved
;  endif 
;    endm


;///////////////////////////////////////////////////////////////////////////////
;> Name:            ccerror
;
;  Type:            Macro
;  Description:     Conditional fatal system error macro.  If the indicated
;                   condition code ("CC") is met, signal the error.  (See "error",
;                   above for more info on error hanlding.)
;                   The allowed condition condition codes are
;
;                       cc, cs, ec, eq, es, ge, gt, lc, le, ls, lt, 
;                       mi, ne, nr, pl, nn 
;
;  Inputs:          condition
;
;  Outputs:         none
;
;  Notes:           The normal way of using these macros is shown below:
;
;                       <some operation which sets/clears a condition code>
;                       ccerror cs             ; Error if carry is set
;                       <normal continuation>
;
;                   This can replace old code of the form:
;
;                       <some operation which sets/clears a condition code>
;                       jcc     _OK
;                       error
;                   _OK
;                       <normal continuation>
;
;                   Depending on the BETA/RETAIL flags, the first operation can
;                   save one or two words of program memory over the second method.
;<
;///////////////////////////////////////////////////////////////////////////////
ccerror macro   condition
  if (@DEF('BETA')||@DEF('RETAIL'))
    js\condition $000C
  else
    debug\condition
  endif ; if ( @DEF('BETA')||@DEF('RETAIL') 
    endm

;///////////////////////////////////////////////////////////////////////////////
;> Name: GetLogicalDriveNumForResourceBin
;
; Description: Used for our RestoreDriveFromBackup routine.
; INPUT: 1 for primary resource.bin
;        2 for secondary resource.bin
;        3 for master redundant resource.bin that is used to restore 1 and 2 at power up if needed.
;
; NOTE: If the drive arrays is modified in ddiproject.c then the values below will
;       have to change. Inspect that file to verify these values are correct.
;
;<
;///////////////////////////////////////////////////////////////////////////////
GetLogicalDriveNumForResourceBin macro drive
    if (drive==0)
    fail    "drive=0 invalid value used in GetLogicalDriveNumForresourceBin macro"
    endif

    if (drive>3)
    fail    "drive>3 invalid value used in GetLogicalDriveNumForresourceBin macro"
    endif

    if (@DEF('MMC'))

        if drive==1
             move   #>6,a
        endif
    
        if drive==2
             move   #>7,a
        endif

        if drive==3
             move   #>8,a
        endif
    else

        if drive==1
             move   #>5,a
        endif
    
        if drive==2
             move   #>6,a
        endif

        if drive==3
             move   #>7,a
        endif
    endif
 endm

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SaveRange
;
;   Type:           Macro
;
;   Description:    This is a supporting macro for saving memory locations to
;                   the settings.dat file at shutdown, for later restoration
;                   at system startup.
;
;   Inputs:         Usage : SaveRange Label,Length
;                        
;   Outputs:        
;                   
;   Notes:          This macro automatically determines the memory space of the
;                   label specified.  It then constructs a command that is 
;                   written to settings.dat by SysSaveSettings, and is
;                   understood by the SysLoadSettings function to restore the
;                   memory locations.  For the SaveRange macro to be recognized 
;                   by the SysSaveSettings function.  The section name where the
;                   SaveRange command is used, must be included in the 
;                   Settings_Y cluster in the stmp3400.dsc file.  The makefile
;                   will then extract these commands as a resource to be used
;                   by SysSaveSettings and SysLoadSettings.
;
;   SeeAlso:        SysLoadSettings, SysSaveSettings, SaveRangeX, 
;                   SaveRangeY, SaveRangeP, SaveRangeL
;
;<
;///////////////////////////////////////////////////////////////////////////////
SaveRange macro Label,Length
    if @MSP(Label)==0  ;None
    fail    "Invalid label used in SaveRange macro"
    dc      Label
    endif ; if @MSP(Label)==0

    if @MSP(Label)==1  ;X
    dc      $800000|Label
    endif ; if @MSP(Label)==1

    if @MSP(Label)==2  ;Y
    dc      $400000|Label
    endif ; if @MSP(Label)==2

    if @MSP(Label)==3  ;L
    dc      $200000|Label
    endif ; if @MSP(Label)==3

    if @MSP(Label)==4  ;P
    dc      $100000|Label
    endif ; if @MSP(Label)==4

    if @MSP(Label)==5  ;E
    fail    "Invalid label used in SaveRange macro"
    dc      Label
    endif ; if @MSP(Label)==5

    dc      Length*3
    endm

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SaveRangeY
;
;   Type:           Macro
;
;   Description:    This is a supporting macro for saving memory locations to
;                   the settings.dat file at shutdown, for later restoration
;                   at system startup.
;
;   Inputs:         Usage : SaveRangeY Label,Length
;                        
;   Outputs:        
;                   
;   Notes:          Refer to the SaveRange macro for more details.  This macro
;                   can take a numeric address, or a label, but will only
;                   create a save memory command that indicates Y memory
;                   to be saved.
;
;   SeeAlso:        SysLoadSettings, SysSaveSettings, SaveRange, 
;                   SaveRangeX, SaveRangeP, SaveRangeL
;
;<
;///////////////////////////////////////////////////////////////////////////////
SaveRangeY macro Address,Length
    dc      $400000|Address
    dc      Length*3
    endm

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SaveRangeX
;
;   Type:           Macro
;
;   Description:    This is a supporting macro for saving memory locations to
;                   the settings.dat file at shutdown, for later restoration
;                   at system startup.
;
;   Inputs:         Usage : SaveRangeX Label,Length
;                        
;   Outputs:        
;                   
;   Notes:          Refer to the SaveRange macro for more details.  This macro
;                   can take a numeric address, or a label, but will only
;                   create a save memory command that indicates X memory
;                   to be saved.
;
;   See Also:       SysLoadSettings, SysSaveSettings, SaveRange, 
;                   SaveRangeY, SaveRangeP, SaveRangeL
;
;<
;///////////////////////////////////////////////////////////////////////////////
SaveRangeX macro Address,Length
    dc      $800000|Address
    dc      Length*3
    endm

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SaveRangeL
;
;   Type:           Macro
;
;   Description:    This is a supporting macro for saving memory locations to
;                   the settings.dat file at shutdown, for later restoration
;                   at system startup.
;
;   Inputs:         Usage : SaveRangeL Label,Length
;                        
;   Outputs:        
;                   
;   Notes:          Refer to the SaveRange macro for more details.  This macro
;                   can take a numeric address, or a label, but will only
;                   create a save memory command that indicates L memory
;                   to be saved.
;
;   SeeAlso:        SysLoadSettings, SysSaveSettings, SaveRange, 
;                   SaveRangeX, SaveRangeY, SaveRangeP
;
;<
;///////////////////////////////////////////////////////////////////////////////
SaveRangeL macro Address,Length
    dc      $200000|Address
    dc      Length*3
    endm

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SaveRangeP
;
;   Type:           Macro
;
;   Description:    This is a supporting macro for saving memory locations to
;                   the settings.dat file at shutdown, for later restoration
;                   at system startup.
;
;   Inputs:         Usage : SaveRangeP Label,Length
;                        
;   Outputs:        
;                   
;   Notes:          Refer to the SaveRange macro for more details.  This macro
;                   can take a numeric address, or a label, but will only
;                   create a save memory command that indicates P memory
;                   to be saved.
;
;   SeeAlso:        SysLoadSettings, SysSaveSettings, SaveRange, 
;                   SaveRangeX, SaveRangeY, SaveRangeL
;
;<
;///////////////////////////////////////////////////////////////////////////////
SaveRangeP macro Address,Length
    dc      $100000|Address
    dc      Length*3
    endm


;///////////////////////////////////////////////////////////////////////////////
;       BUTTON MACROS
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           DEFINE_ROW
;
;   Type:           Macro
;
;   Description:    This is a supporting macro for defining the GPIO setup for 
;                   the scan matrix.
;
;   Inputs:         Usage : DEFINE_ROW RowNumber,WhichGPIORegister,WhichBit
;                        
;   Outputs:        Sets up equates required for data structures in button.asm
;                   
;   Notes:          Must be used in conjuction with DEFINE_COLUMN, DEFINE_SCAN_BUTTON_EVENT,
;                   DEFINE_NON_SCAN_BUTTON,DEFINE_NON_SCAN_BUTTON_EVENT, and DEFINE_MAPPED_EVENT to
;                   completely define the players button scan matrix, non scan buttons, and
;                   how these buttons map to events as recieved by the menus.  There must be one of these
;                   macro definitions for each row in the scan matrix.
;
;   SeeAlso:        DEFINE_COLUMN, DEFINE_SCAN_BUTTON_EVENT,DEFINE_NON_SCAN_BUTTON,
;                   DEFINE_NON_SCAN_BUTTON_EVENT, and DEFINE_MAPPED_EVENT
;
;<
;///////////////////////////////////////////////////////////////////////////////

DEFINE_ROW macro RowNumber,WhichGPIO,WhichBit
    BUTTON_GPIO_ENABLE_ROW_\RowNumber:  equ HW_\WhichGPIO\ENR
    BUTTON_GPIO_SENSE_ROW_\RowNumber:   equ HW_\WhichGPIO\DIR
    BUTTON_BITPOS_ROW_\RowNumber:       equ 1<<\WhichBit
    endm

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           DEFINE_COLUMN
;
;   Type:           Macro
;
;   Description:    This is a supporting macro for defining the GPIO setup for 
;                   the scan matrix.
;
;   Inputs:         Usage : DEFINE_COLUMN ColumnNumber,WhichGPIORegister,WhichBit
;                        
;   Outputs:        Sets up equates required for data structures in button.asm
;                   
;   Notes:          Must be used in conjuction with DEFINE_COLUMN, DEFINE_SCAN_BUTTON_EVENT,
;                   DEFINE_NON_SCAN_BUTTON,DEFINE_NON_SCAN_BUTTON_EVENT, and DEFINE_MAPPED_EVENT to
;                   completely define the players button scan matrix, non scan buttons, and
;                   how these buttons map to events as recieved by the menus.  There must be one
;                   of these macro definitions for each column in the scan matrix.
;
;   SeeAlso:        DEFINE_COLUMN, DEFINE_SCAN_BUTTON_EVENT,DEFINE_NON_SCAN_BUTTON,
;                   DEFINE_NON_SCAN_BUTTON_EVENT, and DEFINE_MAPPED_EVENT
;
;<
;///////////////////////////////////////////////////////////////////////////////
DEFINE_COLUMN macro ColumnNumber,WhichGPIO,WhichBit
    BUTTON_GPIO_ENABLE_COLUMN_\ColumnNumber:        equ HW_\WhichGPIO\ENR
    BUTTON_GPIO_DATA_OUT_COLUMN_\ColumnNumber:      equ HW_\WhichGPIO\DOR
    BUTTON_GPIO_OUTPUT_ENABLE_COLUMN_\ColumnNumber: equ HW_\WhichGPIO\DOER
    BUTTON_BITPOS_COLUMN_\ColumnNumber:             equ 1<<\WhichBit
    endm

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           DEFINE_SCAN_BUTTON_EVENT
;
;   Type:           Macro
;
;   Description:    This is a supporting macro for defining the mapping between a scan
;                   matrix location and a button event.
;
;   Inputs:         Usage : DEFINE_SCAN_BUTTON_EVENT Column,Row,EventNumber,ButtonName.  
;                       Column is in the format COL0,COL1,COL2, etc.  
;                       Row is in the format ROW0,ROW1,etc. 
;                       The EventNumber is an integer with a single bit set that is unique to all button events
;                       ButtonName is a label by which this button event can be referenced in the event mapping.
;                        
;   Outputs:        Sets up equates required for data structures in button.asm
;                   
;   Notes:          Must be used in conjuction with DEFINE_COLUMN, DEFINE_SCAN_BUTTON_EVENT,
;                   DEFINE_NON_SCAN_BUTTON,DEFINE_NON_SCAN_BUTTON_EVENT, and DEFINE_MAPPED_EVENT to
;                   completely define the players button scan matrix, non scan buttons, and
;                   how these buttons map to events as recieved by the menus. This macro sets up buttons for each
;                   spot in the button scan matrix.  Each place in the matrix must be defined, even if no
;                   physical button exists in it.  Use $000000 for placeholders.
;
;   SeeAlso:        DEFINE_COLUMN, DEFINE_SCAN_BUTTON_EVENT,DEFINE_NON_SCAN_BUTTON,
;                   DEFINE_NON_SCAN_BUTTON_EVENT, and DEFINE_MAPPED_EVENT
;
;<
;///////////////////////////////////////////////////////////////////////////////
DEFINE_SCAN_BUTTON_EVENT macro Column,Row,EventNumber,ButtonName
    BUTTON_EVENT_\Column\Row:  equ \EventNumber
    ButtonName: equ \EventNumber
    endm 

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           DEFINE_NON_SCAN_BUTTON
;
;   Type:           Macro
;
;   Description:    This is a supporting macro for defining a non scan button.
;
;   Inputs:         Usage : DEFINE_NON_SCAN_BUTTON NSNumber,WhichGPIO,WhichBit.  
;                       NSNumber is a unique number (starting at 0) and counting up to BUTTON_NON_SCAN_BUTTONS-1
;                       WhichGPIO is the GPIO register used
;                       WhichBit is the bit in this register.
;                        
;   Outputs:        Sets up equates required for data structures in button.asm
;                   
;   Notes:          Must be used in conjuction with DEFINE_COLUMN, DEFINE_SCAN_BUTTON_EVENT,
;                   DEFINE_NON_SCAN_BUTTON,DEFINE_NON_SCAN_BUTTON_EVENT, and DEFINE_MAPPED_EVENT to
;                   completely define the players button scan matrix, non scan buttons, and
;                   how these buttons map to events as recieved by the menus. This macro sets up non scan
;                   buttons.  Use DEFINE_NON_SCAN_BUTTON_EVENT to define the event generated by this non scan
;                   button.
;
;   SeeAlso:        DEFINE_COLUMN, DEFINE_SCAN_BUTTON_EVENT,DEFINE_NON_SCAN_BUTTON,
;                   DEFINE_NON_SCAN_BUTTON_EVENT, and DEFINE_MAPPED_EVENT
;
;<
;///////////////////////////////////////////////////////////////////////////////
DEFINE_NON_SCAN_BUTTON macro NSNumber,WhichGPIO,WhichBit
    BUTTON_GPIO_ENABLE_NON_SCAN_\NSNumber:  equ HW_\WhichGPIO\ENR
    BUTTON_GPIO_SENSE_NON_SCAN_\NSNumber:   equ HW_\WhichGPIO\DIR
    BUTTON_BITPOS_NON_SCAN_\NSNumber:       equ 1<<\WhichBit
    BUTTON_REAL_BITPOS_NON_SCAN_\NSNumber:  equ \WhichBit
    endm

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           DEFINE_NON_SCAN_BUTTON_EVENT
;
;   Type:           Macro
;
;   Description:    This is a supporting macro for defining a non scan button.
;
;   Inputs:         Usage : DEFINE_NON_SCAN_BUTTON_EVENT Number,EventNumber,ButtonName  
;                       Number is a unique number (starting at 0) and counting up to BUTTON_NON_SCAN_BUTTONS-1
;                       EventNumber is an integer with 1 bit set that is unique to all other buttons
;                       ButtonName is a label later referenced in the event mapping
;                        
;   Outputs:        Sets up equates required for data structures in button.asm
;                   
;   Notes:          Must be used in conjuction with DEFINE_COLUMN, DEFINE_SCAN_BUTTON_EVENT,
;                   DEFINE_NON_SCAN_BUTTON,DEFINE_NON_SCAN_BUTTON_EVENT, and DEFINE_MAPPED_EVENT to
;                   completely define the players button scan matrix, non scan buttons, and
;                   how these buttons map to events as recieved by the menus. This macro sets up non scan
;                   buttons.  
;
;   SeeAlso:        DEFINE_COLUMN, DEFINE_SCAN_BUTTON_EVENT,DEFINE_NON_SCAN_BUTTON,
;                   DEFINE_NON_SCAN_BUTTON_EVENT, and DEFINE_MAPPED_EVENT
;
;<
;///////////////////////////////////////////////////////////////////////////////
DEFINE_NON_SCAN_BUTTON_EVENT macro Number,EventNumber,ButtonName
    BUTTON_EVENT_NONSCAN\Number:  equ \EventNumber
    ButtonName: equ \EventNumber
    endm 



;///////////////////////////////////////////////////////////////////////////////
;> Name: DEFINE_LRADC_BUTTON
;
;  Type: Macro
;
;  Description: This is a supporting macro for defining an Low Resolution ADC (LRADC)-based button
;
;  Inputs:      Usage : DEFINE_LRADC_BUTTON LBNumber,UpperLimitVoltage
;
;  Outputs:     Sets up equates required for data structures in button.asm
;
;  Notes:       Must be used in conjuction with DEFINE_LRADC_BUTTON_EVENT, and DEFINE_MAPPED_EVENT to
;                   completely define the player's LRADC buttons and how these buttons map to 
;                   events as recieved by the menus. This macro sets up LRADC buttons.  Use 
;                   DEFINE_LRADC_BUTTON_EVENT to define the event generated by this LRADC button.
;
;   SeeAlso:        DEFINE_LRADC_BUTTON_EVENT, DEFINE_COLUMN, DEFINE_SCAN_BUTTON_EVENT,
;                   DEFINE_NON_SCAN_BUTTON, DEFINE_NON_SCAN_BUTTON_EVENT, and DEFINE_MAPPED_EVENT
;<
;///////////////////////////////////////////////////////////////////////////////
DEFINE_LRADC_BUTTON     macro   num,level
    BUTTON_LRADC_LEVEL_\num:    equ level
    endm



;///////////////////////////////////////////////////////////////////////////////
;> Name: DEFINE_LRADC_BUTTON_EVENT
;
;  Type: Macro
;
;  Description:     This is a supporting macro for defining a Low Resolution ADC (LRADC)-based button.
;
;  Inputs:          Usage : DEFINE_LRADC_BUTTON_EVENT Number,EventNumber,ButtonName  
;                       Number is a unique number (starting at 0) and counting up to BUTTON_LRADC_BUTTONS-1
;                       EventNumber is an integer with 1 bit set that is unique to all other buttons
;                       ButtonName is a label later referenced in the event mapping
 
;
;  Outputs:         Sets up equates required for data structures in button.asm
;
;  Notes:           Must be used in conjuction with DEFINE_LRADC_BUTTON, and DEFINE_MAPPED_EVENT to
;                   completely define the player's LRADC buttons and how these buttons map to events 
;                   as recieved by the menus. This macro sets up LRADC buttons.
;
;
;   SeeAlso:        DEFINE_LRADC_BUTTON, DEFINE_COLUMN, DEFINE_SCAN_BUTTON_EVENT,DEFINE_NON_SCAN_BUTTON,
;                   DEFINE_NON_SCAN_BUTTON_EVENT, and DEFINE_MAPPED_EVENT
;<
;///////////////////////////////////////////////////////////////////////////////
DEFINE_LRADC_BUTTON_EVENT      macro   num,EventNumber,ButtonName
    BUTTON_EVENT_LRADC_BUTTON\num: equ \EventNumber
    ButtonName:     equ     \EventNumber   
    endm
  


;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           DEFINE_MAPPED_EVENT
;
;   Type:           Macro
;
;   Description:    This is a supporting macro for defining button events.
;
;   Inputs:         Usage : DEFINE_MAPPED_EVENT Number,Action,ButtonNames 
;                       Number is a unique number (starting at 0) and counting up to BUTTON_MAPPED_EVENTS-1
;                       Action is either PRESS_RELEASE or PRESS_HOLD
;                       ButtonNames is a collection of labels defined for scan or non scan buttons that relate
;                           to this button event.
;                        
;   Outputs:        Sets up equates required for data structures in button.asm
;                   
;   Notes:          Must be used in conjuction with DEFINE_COLUMN, DEFINE_SCAN_BUTTON_EVENT,
;                   DEFINE_NON_SCAN_BUTTON,DEFINE_NON_SCAN_BUTTON_EVENT, and DEFINE_MAPPED_EVENT to
;                   completely define the players button scan matrix, non scan buttons, and
;                   how these buttons map to events as recieved by the menus. This macro sets up the mapping
;                   between scan and non scan buttons, and the eventual event number that is sent to the menus.
;
;   SeeAlso:        DEFINE_COLUMN, DEFINE_SCAN_BUTTON_EVENT,DEFINE_NON_SCAN_BUTTON,
;                   DEFINE_NON_SCAN_BUTTON_EVENT, and DEFINE_MAPPED_EVENT
;
;<
;///////////////////////////////////////////////////////////////////////////////
DEFINE_MAPPED_EVENT macro Number,Action,ButtonNames
    
  IF (@SCP("Action","PRESS_RELEASE"))
    BUTTON_EVENT_\Number:  equ ButtonNames
  else 
    if (@SCP("Action","PRESS_HOLD"))
        BUTTON_EVENT_\Number:  equ $800000|ButtonNames
    else
        ERROR "Unknown Action for DEFINE_MAPPED_EVENT" 
    endif
  endif 
    endm


;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           DEFINE_2BUTTON_MAPPED_EVENT
;
;   Type:           Macro
;
;   Description:    This is a supporting macro for defining button events.
;
;   Inputs:         Usage : DEFINE_2BUTTON_MAPPED_EVENT Number,Action,Event1,Event2 
;                       Number is a unique number (starting at 0) and counting up to BUTTON_MAPPED_EVENTS-1
;                       Action is either PRESS_RELEASE or PRESS_HOLD
;                       Event1 is 1 of 2 previously defined button events
;                       Event2 is 2 of 2 previously defined button events
;                        
;   Outputs:        Sets up equates required for data structures in button.asm
;                   
;   Notes:          Must be used in conjuction with DEFINE_COLUMN, DEFINE_SCAN_BUTTON_EVENT,
;                   DEFINE_NON_SCAN_BUTTON,DEFINE_NON_SCAN_BUTTON_EVENT, and DEFINE_MAPPED_EVENT to
;                   completely define the players button scan matrix, non scan buttons, and
;                   how these buttons map to events as recieved by the menus. This macro sets up the mapping
;                   between scan and non scan buttons, and the eventual event number that is sent to the menus.
;
;   SeeAlso:        DEFINE_COLUMN, DEFINE_SCAN_BUTTON_EVENT,DEFINE_NON_SCAN_BUTTON,
;                   DEFINE_NON_SCAN_BUTTON_EVENT, and DEFINE_MAPPED_EVENT
;
;<
;///////////////////////////////////////////////////////////////////////////////
DEFINE_2BUTTON_MAPPED_EVENT macro Number,Action,Event1,Event2
    
  IF (@SCP("Action","PRESS_RELEASE"))
    BUTTON_EVENT_\Number:  equ BUTTON_EVENT_\Event1|BUTTON_EVENT_\Event2
  else 
    if (@SCP("Action","PRESS_HOLD"))
        BUTTON_EVENT_\Number:  equ BUTTON_EVENT_\Event1|BUTTON_EVENT_\Event2|$800000
    else
        ERROR "Unknown Action for DEFINE_MAPPED_EVENT" 
    endif
  endif 
  endm

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           StackCheckIn
;
;   Type:           Macro
;
;   Description:    Records the current stack pointer on the stack
;
;   Inputs:         none
;                        
;   Outputs:        places the current PC and the current software stack pointer on the stack
;   
;   Notes:          This does not modify any registers (except r7)                
;
;   SeeAlso:        FStackCheckIn
;
;<
;///////////////////////////////////////////////////////////////////////////////
StackCheckIn macro
  if (@DEF('DEBUG'))
    extern FStackCheckIn
    jsr FStackCheckIn
  endif
  endm

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           StackCheckOut
;
;   Type:           Macro
;
;   Description:    Verifies that the stack is still balanced
;
;   Inputs:         none
;                        
;   Outputs:        none (though hits a debug if the stack is unbalanced)
;   
;   Notes:          This does not modify any registers (except r7) 
;
;   SeeAlso:        FStackCheckOut
;
;<
;///////////////////////////////////////////////////////////////////////////////
StackCheckOut macro
  if (@DEF('DEBUG'))
    extern FStackCheckOut
    jsr FStackCheckOut
  endif 
  endm


;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           InstrumentPushContext 
;
;   Type:           Macro
;
;   Description:    calls the instrument functions specified.
;
;   Inputs:         new context
;                   new value
;                        
;   Outputs:        
;   
;   Notes:          This does not modify any registers  
;
;   SeeAlso:        
;
;<
;///////////////////////////////////////////////////////////////////////////////
InstrumentPushContext macro context,value
  if (@DEF('USE_INSTRUMENTATION'))
    extern FInstrumentPushContext
    extern PushAandB
    extern PopAandB
    jsr PushAandB
    move    #>context,a
    move    #>value,b
    jsr FInstrumentPushContext
    jsr PopAandB
  endif 
  endm

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           InstrumentPopContext 
;
;   Type:           Macro
;
;   Description:    calls the instrument functions specified.
;
;   Inputs:         none
;                        
;   Outputs:        none
;   
;   Notes:          This does not modify any registers  
;
;   SeeAlso:        
;
;<
;///////////////////////////////////////////////////////////////////////////////
InstrumentPopContext macro 
  if (@DEF('USE_INSTRUMENTATION'))
    extern FInstrumentPopContext
    jsr FInstrumentPopContext
  endif 
  endm

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           InstrumentSetValue 
;
;   Type:           Macro
;
;   Description:    calls the instrument functions specified.
;
;   Inputs:         Value
;                        
;   Outputs:        none
;   
;   Notes:          This does not modify any registers  
;
;   SeeAlso:        
;
;<
;///////////////////////////////////////////////////////////////////////////////
InstrumentSetValue macro value
  if (@DEF('USE_INSTRUMENTATION'))
    extern FInstrumentSetValue
    jsr PushA
    move #>value,a
    jsr FInstrumentSetValue
    jsr PopA
  endif 
  endm


;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           InstrumentSetEvent 
;
;   Type:           Macro
;
;   Description:    calls the instrument functions specified.
;
;   Inputs:         Value
;                        
;   Outputs:        none
;   
;   Notes:          This does not modify any registers  
;
;   SeeAlso:        
;
;<
;///////////////////////////////////////////////////////////////////////////////
InstrumentSetEvent macro event
  if (@DEF('USE_INSTRUMENTATION'))
    extern FInstrumentSetEvent
    jsr PushA
    move #>event,a
    jsr FInstrumentSetEvent
    jsr PopA
  endif 
  endm

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           InstrumentClearEvent 
;
;   Type:           Macro
;
;   Description:    calls the instrument functions specified.
;
;   Inputs:         event
;                        
;   Outputs:        none
;   
;   Notes:          This does not modify any registers  
;
;   SeeAlso:        
;
;<
;///////////////////////////////////////////////////////////////////////////////
InstrumentClearEvent macro event
  if (@DEF('USE_INSTRUMENTATION'))
    extern FInstrumentClearEvent
    jsr PushA
    move #>event,a
    jsr FInstrumentClearEvent
    jsr PopA
  endif 
  endm

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           InstrumentToggleEvent 
;
;   Type:           Macro
;
;   Description:    calls the instrument functions specified.
;
;   Inputs:         event
;                        
;   Outputs:        none
;   
;   Notes:          This does not modify any registers  
;
;   SeeAlso:        
;
;<
;///////////////////////////////////////////////////////////////////////////////
InstrumentToggleEvent macro event
  if (@DEF('USE_INSTRUMENTATION'))
    extern FInstrumentToggleEvent
    jsr PushA
    move #>event,a
    jsr FInstrumentToggleEvent
    jsr PopA
  endif 
  endm

  endif ; IF !@DEF('SysMacro_asm')
