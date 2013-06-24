    IF (!@def(resources_mmcddi))
resources_mmcddi equ 1
RSRC_MMCMEDIAINIT                                   equ     0    ;$FILENAME mmcmediastartup.src     
RSRC_MMCMEDIAALLOCATE                               equ     RSRC_MMCMEDIAINIT          
RSRC_MMCMEDIADISCOVERALLOCATION                     equ     RSRC_MMCMEDIAINIT          
RSRC_MMCMEDIAGETINFO                                equ     0     ;$FILENAME mmcinfo.src             
RSRC_MMCMEDIAGETMEDIATABLE                          equ     RSRC_MMCMEDIAGETINFO       
RSRC_MMCMEDIAERASE                                  equ     0     ;$FILENAME mmcerase.src            

RSRC_MMCDATADRIVEINIT                               equ     RSRC_MMCMEDIAINIT          
RSRC_MMCDATADRIVEGETINFO                            equ     RSRC_MMCMEDIAGETINFO       
RSRC_MMCDATADRIVESETINFO                            equ     RSRC_MMCMEDIAGETINFO       
RSRC_MMCDATADRIVEREADSECTOR                         equ     0     ;$FILENAME mmcreadwrite.src        
RSRC_MMCDATADRIVEWRITESECTOR                        equ     RSRC_MMCDATADRIVEREADSECTOR
RSRC_MMCDATADRIVEMULTIWRITESETUP                    equ     RSRC_MMCDATADRIVEREADSECTOR
RSRC_MMCDATADRIVEMULTIWRITESECTOR                   equ     RSRC_MMCDATADRIVEREADSECTOR
RSRC_MMCDATADRIVEERASE                              equ     RSRC_MMCMEDIAERASE         
RSRC_MMCDATADRIVEFLUSH                              equ     RSRC_MMCMEDIAINIT          
  ENDIF ;IF (!@def(resources_mmcddi))
