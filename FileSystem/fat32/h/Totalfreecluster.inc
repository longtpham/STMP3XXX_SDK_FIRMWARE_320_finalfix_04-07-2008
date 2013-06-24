;***********************************************************************
;SigmaTel, Inc
;$Archive: /Mp3Encoder/Algorithms/Encoder/Mp3Enc/src/inc/GrInfo.inc $
;$Revision: 4 $
;$Date: 11/01/03 6:05p $
;Description:GrInfo.inc  
;Notes:	This is the main file. 		
;***********************************************************************/

MEDIATABLE_BYTESPERSECTOR               equ         0
MEDIATABLE_SECTORSPERCLUSTER            equ         MEDIATABLE_BYTESPERSECTOR+1
MEDIATABLE_RSVDSECTORS                  equ         MEDIATABLE_SECTORSPERCLUSTER+1
MEDIATABLE_NOOFFATS                     equ         MEDIATABLE_RSVDSECTORS+1
MEDIATABLE_MAXROOTDIRENTRIES            equ         MEDIATABLE_NOOFFATS+1
MEDIATABLE_TOTALSECTORS                 equ         MEDIATABLE_MAXROOTDIRENTRIES+1
MEDIATABLE_FATSIZE                      equ         MEDIATABLE_TOTALSECTORS+2
MEDIATABLE_ROOTDIRCLUSTER               equ         MEDIATABLE_FATSIZE+2
MEDIATABLE_NEXTFREECLUSTER              equ         MEDIATABLE_ROOTDIRCLUSTER+2
MEDIATABLE_TOTALFREECLUSTERS            equ         MEDIATABLE_NEXTFREECLUSTER+2 
MEDIATABLE_ROOTDIRSECTORS               equ         MEDIATABLE_TOTALFREECLUSTERS+2
MEDIATABLE_FIRSTDATASECTOR              equ         MEDIATABLE_ROOTDIRSECTORS+1
MEDIATABLE_FATTYPE                      equ         MEDIATABLE_FIRSTDATASECTOR+1
MEDIATABLE_TOTALNOOFCLUSTERS            equ         MEDIATABLE_FATTYPE+1 
MEDIATABLE_CLUSTERMASK                  equ         MEDIATABLE_TOTALNOOFCLUSTERS+2
MEDIATABLE_CLUSTERSHIFT                 equ         MEDIATABLE_CLUSTERMASK+1
MEDIATABLE_SECTORSHIFT                  equ         MEDIATABLE_CLUSTERSHIFT+1
MEDIATABLE_SECTORMASK                   equ         MEDIATABLE_SECTORSHIFT+1

X_MEMORY                                 equ       0
FAT32                                    equ       2
FATTYPE                                  equ       1
LENGTH_OF_MEDIATABLE_STRUCT              equ      24
READSECTOR_FAIL                          equ     -16417
NORMALTYPE                               equ       0