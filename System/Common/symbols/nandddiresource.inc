    IF (!@def(resources_nandddi))
resources_nandddi equ 1

RSRC_NANDMEDIAINIT						equ     0 ;$FILENAME mediastartup.src
RSRC_NANDMEDIADISCOVERALLOCATION        equ     RSRC_NANDMEDIAINIT
RSRC_NANDMEDIAGETINFO                   equ     RSRC_NANDMEDIAINIT
RSRC_NANDSYSTEMDRIVEINIT                equ     RSRC_NANDMEDIAINIT 
RSRC_NANDSYSTEMDRIVEGETINFO             equ     RSRC_NANDMEDIAINIT  
RSRC_NANDMEDIAALLOCATE                  equ     RSRC_NANDMEDIAINIT
RSRC_NANDMEDIAERASE                     equ     RSRC_NANDMEDIAINIT
RSRC_NANDMEDIAGETMEDIATABLE             equ     RSRC_NANDMEDIAINIT

RSRC_NANDDATADRIVEINIT					equ     0 ;$FILENAME nanddatadriveinit.src
RSRC_NANDDATADRIVESETINFO               equ     RSRC_NANDDATADRIVEINIT
;RSRC_NANDDATADRIVEGETINFO               equ     RSRC_NANDDATADRIVEINIT
RSRC_NANDDATADRIVEERASE                 equ     RSRC_NANDDATADRIVEINIT 

RSRC_NANDDATADRIVEREADSECTOR			equ     0 ;$FILENAME nanddatadrivereadsector.src
RSRC_NANDDATADRIVEWRITESECTOR           equ     RSRC_NANDDATADRIVEREADSECTOR
RSRC_NANDDATADRIVEGETINFO               equ     RSRC_NANDDATADRIVEREADSECTOR


RSRC_NANDSYSTEMDRIVESETINFO             equ     0 ;$FILENAME nandsystemdrivesetinfo.src  
RSRC_NANDSYSTEMDRIVEWRITESECTOR         equ     RSRC_NANDSYSTEMDRIVESETINFO 
RSRC_NANDSYSTEMDRIVEMULTIWRITESETUP     equ     RSRC_NANDSYSTEMDRIVESETINFO 
RSRC_NANDSYSTEMDRIVEMULTIWRITESECTOR    equ     RSRC_NANDSYSTEMDRIVESETINFO 
RSRC_NANDSYSTEMDRIVEERASE               equ     RSRC_NANDSYSTEMDRIVESETINFO 
RSRC_NANDSYSTEMDRIVEFLUSH               equ     RSRC_NANDSYSTEMDRIVESETINFO 

RSRC_NANDDATADRIVEFLUSH                 equ     0
RSRC_NANDDATADRIVEDUMBMULTIWRITESETUP   equ     0
RSRC_NANDDATADRIVEDUMBMULTIWRITESECTOR  equ     0
RSRC_NANDDATADRIVECACHEMULTIWRITESETUP  equ     0
RSRC_NANDDATADRIVECACHEMULTIWRITESECTOR equ     0
RSRC_NANDSYSTEMDRIVEREADSECTOR          equ     0 

  ENDIF ;IF (!@def(resources_nandddi))
