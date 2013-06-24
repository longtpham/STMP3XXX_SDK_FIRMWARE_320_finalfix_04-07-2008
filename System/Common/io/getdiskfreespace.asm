;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000/2001
;
; 
;///////////////////////////////////////////////////////////////////////////////



;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////
    nolist

    include "regscore.inc"
    include "sysmacro.asm"
    list
    page    132,60,0,0,0


    EXTERN   pMT
    EXTERN   pFAT
    EXTERN   FATcxsFree
    EXTERN   MT_BytesPerSector
    EXTERN   MT_SectPerCluster
    EXTERN   DevTable

    GLOBAL  FGetDiskFreeSpace
    GLOBAL  DiskBytesFree

    OPT OP

    org p,".ptextGetDiskFreeSpace":
;///////////////////////////////////////////////////////////////////////////////
;
;   FGetDiskFreeSpace
;       long GetDiskFreeSpace(int iDevice);
;
;       returns the free space in bytes on device iDevice
;
;   DiskBytesFree          
;       Entry:
;           a2 = disk number
;       Exit
;           a1 = num bytes high
;           a0 = num bytes low
;           b1 = number of free clusters
;           b0 = number of bytes per cluster
;///////////////////////////////////////////////////////////////////////////////
FGetDiskFreeSpace
    move    a1,a2
DiskBytesFree
    ; find the pointers to the Media & FAT tables for this drive
    ; TODO - need error checking on device if no drive present!
    move    a2,n0
    move    #DevTable,r0                ; get the Device Table
    move    #pFAT,n2                    ; offset to DCT FAT pointer
    move    y:(r0+n0),r2                ; get the Device Control Table pointer
    nop
    move    y:(r2+n2),r0                ; get the FAT table pointer
    move    #pMT,n2                     ; offset to the DCT MT pointer
    nop
    move    y:(r2+n2),r1                ; get the Media table pointer
    ; now calculate the number of bytes per cluster
    move    #MT_BytesPerSector,n1
    nop
    move    y:(r1+n1),x0                ; get number of bytes per sector
    move    #MT_SectPerCluster,n1
    nop
    move    y:(r1+n1),x1                ; get number of sectors per cluster
    mpy     x0,x1,a
    asr     a
    push    a0                          ; temp save bytes per cluster
    move    a0,x1                       ; get it ready for later
    ; now get number of free clusters & calculate number of free bytes
    move    #FATcxsFree,n0
    nop
    clr     b           y:(r0+n0),x0
    push    x0                          ; temp save the num free clusters
    jclr    #23,x0,_not_too_big
    bclr    #23,x0                      ; yoink
    move    #>$000100,b1                ; account for $800000*512=$000100000000
_not_too_big
    mpy     x0,x1,a
    asr     a
    add     b,a                         ; total number of free bytes
    pop     b1                          ; return number of free clusters
    pop     b0                          ; return number of bytes per cluster
    bclr    #HW_SR_C_BITPOS,sr
    rts
