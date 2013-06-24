;///////////////////////////////////////////////////////////////////////////////
; CopyRight (c) SigmaTel, Inc. 2003
; Filename: sysfatwrite.asm
;///////////////////////////////////////////////////////////////////////////////
    section SYSFUN_SysFatWrite

    opt     op

    nolist
    include "systemdefs.inc"
    include "sysresources.inc"
    include "sysequ.inc"
    include "sysmacro.asm"
    include "resource.inc"
	include "const.xref"
    list
    


    global  SysLoadFATWrite
    global  SysUnLoadFATWrite


    extern    SysLoadResource
    extern    FileSystemMode
    extern    FFlushCache
	extern    FDriveFlush
	extern    FUpdateFSInfo

    extern    F_lc_u_bFATWrite_P
    extern    F_lc_u_eFATWrite_P

    EXTERN    RSRC_FATWRITE_P_CODE
    EXTERN    RSRC_FUNCLET_SYSLOADFATWRITE

    if (@DEF('FUNCLET'))           
       extern    SysCallFunclet
    endif

    org p,"SysFATWrite_p":

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysLoadFATWrite
;
;   Type:           Function
;
;   Description:    Loads the FAT12 Write code & parameters
;
;   Inputs:         none
;
;   Outputs:        none
;                   
;   Notes:          Sets the File System Mode to WRITE
;<
;///////////////////////////////////////////////////////////////////////////////
SysLoadFATWrite

    if (@DEF('FUNCLET'))           
   
       ;If using funclet, function will be executed via funclet kernel
       CallFunclet RSRC_FUNCLET_SYSLOADFATWRITE
   
       org p,"SYSFUNCLET_SysLoadFATWrite_p":
       
       dc      RSRC_FUNCLET_SYSLOADFATWRITE           ; Funclet Resource number
   
    endif

    ; Load the FAT Write P memory
    move    #>RSRC_FATWRITE_P_CODE,x0   ; Resource number
    move    #>RSRC_TYPE_DATA,x1         ; Resource type is data
    move    #>(F_lc_u_eFATWrite_P-F_lc_u_bFATWrite_P)*3,y1
    move    #>F_lc_u_bFATWrite_P,y0    ; Address of the overlay area
    move    #TARGET_MEM_P,a1            ; Overlay area is in P memory
    jsr     SysLoadResource


    ; Set FileSystemMode to indicate that FAT Write code is loaded    
    move    #FileSystemMode,r1
    nop
    bset    #FILE_SYS_MODE_BIT,y:(r1)       

    rts


    if (@DEF('FUNCLET'))
    
       ;If using funclet, switch counter back to original here
       org    p,"SysFATWrite_p":                             
   
    endif


;///////////////////////////////////////////////////////////////////////////////
;> Name: SysUnLoadFATWrite
;
;  Type: Function
;  Description: 
;  Inputs: 
;  Outputs: 
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////
SysUnLoadFATWrite
	jsr		FUpdateFSInfo

    jsr     FFlushCache
    push a1     ; save error code for MMC write protect
    clr A
  ;DS Based on advice from Akshay, the call to DriveFlush is removed, to reduce the time delay between songs.
  ;None of the team members recollect why it was put in here to start with. 
  ;jsr FDriveFlush  ; TOVERIFY3    
  if @def('MMC')
    move    y:<Const_000001,a
    jsr FDriveFlush
  endif
    move    #FileSystemMode,r1
    nop
    bclr    #FILE_SYS_MODE_BIT,y:(r1)       ;Clears the file system write
    pop a1     ; restore error code for MMC write protect
    rts


    endsec
