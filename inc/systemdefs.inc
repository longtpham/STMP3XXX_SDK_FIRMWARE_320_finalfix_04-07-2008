;/************************SystemDefs.h****************************/

 IF !@DEF('SYSTEM_Definitions')
   DEFINE   SYSTEM_Definitions	'1'

; *** Memory Map ***
   DEFINE   DDP						''
   DEFINE   FSP						''
   DEFINE   DDX						''
   DEFINE   FSX						''
   DEFINE   DDY						''
   DEFINE   FSY						''
   DEFINE   FSL						''
   DEFINE   LIBX						''
   DEFINE   LIBY						''
   DEFINE   LIBP						''
   DEFINE   LIBL						''
   DEFINE   FUX						''
   DEFINE   FUY						''
   DEFINE   FUP						''


; *** Buffer lengths: use even multiples of 3 ***
   DEFINE   ByteSize             '8'                                 ; bits
   DEFINE   DSPWordSize	         '3'                                 ; bytes


; //*** Hardcore constant information
   DEFINE BackslashCharacter	    '$00005c'	; '\'
   DEFINE ForwardslashCharacter	    '$00002f'	; '/'

                                                          ; NOTE: We use forward slashes, internally,
                                                          ; as directory separators, instead of 
                                                          ; backslashes.  This is done to avoid
                                                          ; problems that arise with certain Asian
                                                          ; character sets, where the DBCS Code Page
                                                          ; encoding uses a backslash as the Trailing Byte
                                                          ; of some double-byte characters.  As far as
                                                          ; we can tell, a forward slash is never used
                                                          ; as a Trailing Byte, so it makes a safe
                                                          ; separator character for paths.
   DEFINE DirectorySeparator        '$00002f'   ; '/'

   DEFINE PeriodCharacter           '$00002e'	; '.'
   DEFINE ParentDirectoryString     '$002e2e'	; '..'

   DEFINE CurrentDirectoryString    "PeriodCharacter"
   DEFINE SpaceCharacter		      '$000020'	; ' '
   DEFINE DSPSpaceWord	 	         '$202020'	; '   '



; *** System definitions ***
   DEFINE   NULL                    '$000000'
   
; *** Application Layer Definitions ***
	DEFINE wordsBufSize '512'

; *** Device Status Defineitions returned by device driver init***

   DEFINE   DEVICE_INSTALLED		'0'
   DEFINE   DEVICE_INSERTED		'1'
   DEFINE   DEVICE_NOT_FOUND		'2'
   DEFINE   DEVICE_INVALID		'$ffffff'

	DEFINE	FILE_SYS_MODE_BIT	'0'			;If set, this bit informs the file system that write functions are loaded

; *** Device driver read type

    DEFINE  READ_TYPE_NORMAL      '0'
    DEFINE  READ_TYPE_FAT         '1'
    DEFINE  READ_TYPE_RAW         '2'

; *** Device driver write type

    if (@def('MEDIA_DDI_PARTIAL_SEQ_ENABLE'))
        DEFINE  WRITE_TYPE_RANDOM                   '0'
        DEFINE  WRITE_TYPE_SEQ_FIRST                '1'
        DEFINE  WRITE_TYPE_SEQ_NEXT                 '2'
        DEFINE  WRITE_TYPE_RESET_CLEAN_UP           '3'
        DEFINE  WRITE_TYPE_PARTIAL_SEQ_FIRST        '4'
        DEFINE  WRITE_TYPE_PARTIAL_SEQ_NEXT         '5'
        DEFINE  WRITE_TYPE_PARTIAL_SEQ_LAST         '6'
        if (@def('MEDIA_DDI_RAW_SECTOR_SERVER_ENABLE'))
            DEFINE  WRITE_TYPE_RANDOM_RAW               '7'
            DEFINE  WRITE_TYPE_SEQ_FIRST_RAW            '8'
            DEFINE  WRITE_TYPE_SEQ_NEXT_RAW             '9'
            DEFINE  WRITE_TYPE_PARTIAL_SEQ_FIRST_RAW    '10'
            DEFINE  WRITE_TYPE_PARTIAL_SEQ_NEXT_RAW     '11'
            DEFINE  WRITE_TYPE_PARTIAL_SEQ_LAST_RAW     '12'
        endif
    else
        DEFINE  WRITE_TYPE_RANDOM                   '0'
        DEFINE  WRITE_TYPE_SEQ_FIRST                '1'
        DEFINE  WRITE_TYPE_SEQ_NEXT                 '2'
        if (@def('MEDIA_DDI_RAW_SECTOR_SERVER_ENABLE'))
            DEFINE  WRITE_TYPE_RANDOM_RAW               '3'
            DEFINE  WRITE_TYPE_SEQ_FIRST_RAW            '4'
            DEFINE  WRITE_TYPE_SEQ_NEXT_RAW             '5'
        endif
    endif

    DEFINE  MEDIA_SIZE_TOTAL_SECTORS    '0'
    DEFINE  MEDIA_SIZE_TOTAL_BYTES      '1'
    DEFINE  MEDIA_SIZE_BYTES_PER_SECTOR '2'
    DEFINE  MEDIA_SIZE_IN_MEGABYTES     '3'     

; This file is local to each project
;    include "filesystem.inc"

 ENDIF

