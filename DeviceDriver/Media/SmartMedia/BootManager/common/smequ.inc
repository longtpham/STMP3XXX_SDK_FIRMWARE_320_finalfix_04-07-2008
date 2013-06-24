;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000
;
; STMP SmartMedia        
;///////////////////////////////////////////////////////////////////////////////

    IF (!@def(smequ))
smequ   equ     1

; SmartMedia SmReadParams and SmWriteParams
SM_RWP_ADDRLOW               equ     0
SM_RWP_ADDRHIGH              equ     1
SM_RWP_PAGEBUFADDR           equ     2
SM_RWP_NUMBYTES              equ     3
SM_RWP_CARDINFO              equ     4
SM_RWP_FLAGS                 equ     5
SM_RWP_STATUS                equ     6

SM_ICMD_MASK                equ     $000100
SM_ADDRLOWBYTE_MASK         equ     $FFFF00
SM_ADDRLOWBYTEUPPER_MASK    equ     $0000FF

SM_BUFSHIFT                 equ     $000080
SM_NUMBYTESSHIFT            equ     $000020

SM_RWP_FLAGS_ECCEN_MASK      equ     $000003
SM_RWP_FLAGS_ECCEN00         equ     $000000
SM_RWP_FLAGS_ECCEN01         equ     $000001
SM_RWP_FLAGS_ECCENLOW        equ     0
SM_RWP_FLAGS_ECCENHIGH       equ     1
SM_RWP_FLAGS_IDFEN           equ     2
SM_RWP_FLAGS_BSBEN           equ     3
SM_RWP_FLAGS_RTNERROR        equ     4

; SmartMedia SmCardnInfo
SM_CI_CARDNUM               equ     0
SM_CI_ID                    equ     1
SM_CI_PAGESIZE              equ     2
SM_CI_BLOCKSIZE             equ     3
SM_CI_BLOCKBYTESIZE         equ     4
SM_CI_CARDSIZE              equ     5
SM_CI_CARDMB                equ     6
SM_CI_STATUS                equ     7

; SmartMedia Device Codes for SM_CI_ID
SM_ID_MASK                  equ     $00FF00    ;mask off the manufacturer id
SM_ID_1a                    equ     $006E00
SM_ID_1b                    equ     $00E800
SM_ID_1c                    equ     $00EC00
SM_ID_2                     equ     $00EA00
SM_ID_4a                    equ     $00E300
SM_ID_4b                    equ     $00E500
SM_ID_8                     equ     $00E600
SM_ID_16                    equ     $007300
SM_ID_32                    equ     $007500
SM_ID_64                    equ     $007600
SM_ID_128                   equ     $007900

; SmReadIdParams
SM_RID_PAGEBUFADDR          equ     0
SM_RID_CARDINFO             equ     1
SM_RID_STATUS               equ     2

; SmStatusReadParams
SM_SR_PAGEBUFADDR           equ     0
SM_SR_CARDINFO              equ     1
SM_SR_STATUS                equ     2

; SmBlockEraseParams
SM_BE_ADDRLOW               equ     0
SM_BE_ADDRHIGH              equ     1
SM_BE_PAGEBUFADDR           equ     2
SM_BE_CARDINFO              equ     3
SM_BE_STATUS                equ     4

SM_IDFZEROERRORCOUNT        equ     4
SM_BSBZEROERRORCOUNT        equ     2

; Card Status
SM_CS_PASSFAIL_BIT          equ     0
SM_CS_READYBUSY_BIT         equ     6
SM_CS_WRITEPROTECT_BIT      equ     7

FLH_PAGEBUFSIZE         equ     $00B0
SM_PAGEBUFSIZE          equ     $00B0
; !!!!!!!!!!!!! We're assuming SmartMedia >= 4MB !!!!!!!!!!!!!!!!!!!!!!!!
SM_PAGESIZE             equ     $0200  ;4 MBytes and up
SM_PAGESIZE2            equ     $0100  ;2 MBytes and under
SM_PAGESPERBLOCK16      equ     32
SM_PAGESPERBLOCK8       equ     16
SM_PHYSBLOCKSIZE16      equ     SM_PAGESIZE*SM_PAGESPERBLOCK16
SM_PHYSBLOCKSIZE8       equ     SM_PAGESIZE*SM_PAGESPERBLOCK8
SM_PHYSBLOCKSIZE2       equ     SM_PAGESIZE2*SM_PAGESPERBLOCK8
SM_PAGESTATE_A          equ     1
SM_PAGESTATE_B          equ     2
SM_PAGESTATE_C          equ     3


;ASM Only
    if ("SMD"=="SM")
SM_TRANS_TIMEOUT        equ     $000004
SM_BUSY_TIMEOUT        equ      $000004
    else
SM_TRANS_TIMEOUT        equ     $7FFFFF
SM_BUSY_TIMEOUT        equ      $7FFFFF
    endif
;endasm    


SM_STATUS_BYTES         equ     $AC     ; PageBuf offset $AC (172).
                                    ;   CIS/IDI Field:
                                    ;      byte 0 = Invalid Data Flag
                                    ;      byte 1 = $ff
                                    ;   All other pages:
                                    ;      byte 0 = Data Status Byte
                                    ;      byte 1 = Block Status Byte
                                    ; For byte 0, if 4 or more bits are zero
                                    ;   then data is invalid
                                    ; For byte 1, if 2 or more bits are zero
                                    ;   the block is invalid

SM_INFO_CARD_DETECTED               equ     0
SM_INFO_READ_ID_TIMEOUT             equ     1
SM_INFO_READ_TIMEOUT                equ     2
SM_INFO_END_OF_BLOCKS               equ     3
SM_INFO_READ_ID_BAD_PROGRAMMING     equ     4
SM_INFO_READ_BAD_PROGRAMMING        equ     5

    ENDIF

