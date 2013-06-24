;////////////////////////////////////////////////////////////////////////////////
;// Copyright(C) SigmaTel, Inc. 2004
;//
;// Filename: ChkdskLoad.c
;// Description: Load Checkdisk from resource file into Application space.
;// To do:
;////////////////////////////////////////////////////////////////////////////////

;////////////////////////////////////////////////////////////////////////////////
;//   Includes and external references
;////////////////////////////////////////////////////////////////////////////////

  nolist
    include "resource.inc"
    include "sysequ.inc"
    include "sysresources.inc"
    list

    global  CheckDiskLoad

    extern  F_lc_u_b_CheckdiskOvl,F_lc_u_b_chkdsk_Y_buf
    extern  F_lc_u_e_CheckdiskOvl,F_lc_u_e_chkdsk_Y_buf
    extern  SysLoadResource

SIZECHKDSKMODULEY    equ     $1fff
SIZECHKDSKMODULEP    equ     $6000

    extern SaveSystemStack
    extern SysLoadResource
    extern RestoreSystemStack
    extern UserScratchY


    org     p,"CheckDiskLoad_P":
;////////////////////////////////////////////////////////////////////////////////
;//
;//>  Name:          CheckDiskLoad
;//
;//  Type:           Function
;//
;//  Description:    Loads checkdisk into P App space from resource file.
;//                  
;//  Inputs:         none
;//
;//  Outputs:        none
;//                  
;//  Notes:          
;//<
;////////////////////////////////////////////////////////////////////////////////
CheckDiskLoad:
    
    ; save off stack
    move    #_stackSaved,r6              ; setup the return address
    move    #UserScratchY,r5             ; pointer to the SP local save
    jmp     SaveSystemStack              ; save the hardware stack
_stackSaved

    ;Load checkdisk p memory module
    move    #>RSRC_CHECKDISK,x0       ; Resource number
    move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
    move    #3*(F_lc_u_e_CheckdiskOvl-F_lc_u_b_CheckdiskOvl),y1          ; Size of the overlay area
    move    #>F_lc_u_b_CheckdiskOvl,y0        ; Address of the overlay area
    move    #>TARGET_MEM_P,a1               ; Overlay area is in P memory
    jsr     SysLoadResource

    ;Load checkdisk y memory module
    move    #>RSRC_CHKDSK_YMEM,x0       ; Resource number
    move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
    move    #3*(F_lc_u_e_chkdsk_Y_buf-F_lc_u_b_chkdsk_Y_buf),y1          ; Size of the overlay area
    move    #>F_lc_u_b_chkdsk_Y_buf,y0        ; Address of the overlay area
    move    #>TARGET_MEM_Y,a1               ; Overlay area is in P memory
    jsr     SysLoadResource

    ; restore stack
_restoreStack
    move    #UserScratchY,r5             ; pointer to the SP local save
    move    #_stackRestored,r6           ; setup the return address
    jmp     RestoreSystemStack           ; restore the hardware stack
_stackRestored

    rts
