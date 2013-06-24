;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2003
;
; Filename: ddildl.inc
; Description: 
;///////////////////////////////////////////////////////////////////////////////

    IF (!@def(DDILDL_INC))
DDILDL_INC equ 1

; LOGICAL_MEDIA
LOGICAL_MEDIA_STRUCT_SIZE                               equ 2
LOGICAL_MEDIA_DESCRIPTOR                                equ 0
LOGICAL_MEDIA_API                                       equ 1

; LOGICAL_MEDIA_API
LOGICAL_MEDIA_API_INIT_CODE_BANK                        equ 0
LOGICAL_MEDIA_API_INIT_FUNCTION                         equ 1
LOGICAL_MEDIA_API_ALLOCATE_CODE_BANK                    equ 2
LOGICAL_MEDIA_API_ALLOCATE_FUNCTION                     equ 3
LOGICAL_MEDIA_API_DISCOVER_ALLOCATION_CODE_BANK         equ 4
LOGICAL_MEDIA_API_DISCOVER_ALLOCATION_FUNCTION          equ 5
LOGICAL_MEDIA_API_GET_MEDIA_TABLE_CODE_BANK             equ 6
LOGICAL_MEDIA_API_GET_MEDIA_TABLE_FUNCTION              equ 7
LOGICAL_MEDIA_API_GET_INFO_CODE_BANK                    equ 8
LOGICAL_MEDIA_API_GET_INFO_FUNCTION                     equ 9
LOGICAL_MEDIA_API_ERASE_CODE_BANK                       equ 10
LOGICAL_MEDIA_API_ERASE_FUNCTION                        equ 11
LOGICAL_MEDIA_API_STRUCT_SIZE equ (LOGICAL_MEDIA_API_ERASE_FUNCTION-LOGICAL_MEDIA_API_INIT_CODE_BANK+1)                              

; LOGICAL_DRIVE
LOGICAL_DRIVE_STRUCT_SIZE                               equ 2
LOGICAL_DRIVE_DESCRIPTOR                                equ 0
LOGICAL_DRIVE_API                                       equ 1

; LOGICAL_DRIVE_API
LOGICAL_DRIVE_API_INIT_CODE_BANK                        equ 0
LOGICAL_DRIVE_API_INIT_FUNCTION                         equ 1
LOGICAL_DRIVE_API_GET_INFO_CODE_BANK                    equ 2
LOGICAL_DRIVE_API_GET_INFO_FUNCTION                     equ 3
LOGICAL_DRIVE_API_SET_INFO_CODE_BANK                    equ 4
LOGICAL_DRIVE_API_SET_INFO_FUNCTION                     equ 5
LOGICAL_DRIVE_API_READ_SECTOR_CODE_BANK                 equ 6
LOGICAL_DRIVE_API_READ_SECTOR_FUNCTION                  equ 7
LOGICAL_DRIVE_API_WRITE_SECTOR_CODE_BANK                equ 8
LOGICAL_DRIVE_API_WRITE_SECTOR_FUNCTION                 equ 9
LOGICAL_DRIVE_API_MULTI_WRITE_SETUP_CODE_BANK           equ 10
LOGICAL_DRIVE_API_MULTI_WRITE_SETUP_FUNCTION            equ 11
LOGICAL_DRIVE_API_MULTI_WRITE_SECTOR_CODE_BANK          equ 12
LOGICAL_DRIVE_API_MULTI_WRITE_SECTOR_FUNCTION           equ 13
LOGICAL_DRIVE_API_ERASE_CODE_BANK                       equ 14
LOGICAL_DRIVE_API_ERASE_FUNCTION                        equ 15
LOGICAL_DRIVE_API_FLUSH_CODE_BANK                       equ 16
LOGICAL_DRIVE_API_FLUSH_FUNCTION                        equ 17
LOGICAL_DRIVE_API_STRUCT_SIZE equ (LOGICAL_DRIVE_API_FLUSH_FUNCTION-LOGICAL_DRIVE_API_INIT_CODE_BANK+1)                              

    ENDIF ;IF (!@def(DDILDL_INC))
