#This is the makefile for the SmartMedia device driver.
#
#To add the SmartMedia device driver to a project
#   1. Include this makefile in the project makefile
#   2. Add $(SMDDLIST) to the list of linker dependencies



OBJLIST = $(OBJLIST)\
          $(SMDDLIST)

MODULELIST = $(MODULELIST)\
             $(OUTPUT_RSRC_PATH)\SMDDHigh.src


SMDDLIST= $(SMDD_LOW) $(SMDD_HIGH) $(SMDD_PARTIAL)
    
SMDD_LOW= \
    "$(OBJ_PATH)\FlashReadPageRdntAreaSync.obj"  \
    "$(OBJ_PATH)\MDTUtils.obj"        \
    "$(OBJ_PATH)\convertcs.obj"       \
    "$(OBJ_PATH)\PackUtils.obj"       \
    "$(OBJ_PATH)\GetPageRdntArea.obj"   \
    "$(OBJ_PATH)\GetSMStatus.obj"    \
    "$(OBJ_PATH)\HALUtils.obj"       \
    "$(OBJ_PATH)\smartmediadetection.obj"\
    "$(OBJ_PATH)\smartmediastructs.obj" \
    "$(OBJ_PATH)\smartmediaserialnumber.obj"
     

SMDD_HIGH= \
    "$(OBJ_PATH)\SMDDHeader.obj" \
    "$(OBJ_PATH)\FlashEraseRawBlock.obj" \
    "$(OBJ_PATH)\FlashWritePageRdntAreaSync.obj" \
    "$(OBJ_PATH)\MarkBadBlk.obj" \
    "$(OBJ_PATH)\CopyUtils.obj" \
    "$(OBJ_PATH)\FlashWriteRawPageSync.obj" \
    "$(OBJ_PATH)\FlashRelocateBlock.obj" \
    "$(OBJ_PATH)\FlashWritePageSync.obj" \
    "$(OBJ_PATH)\FlashWritePageSeq.obj" \
    "$(OBJ_PATH)\ZoneMapUtils_Write.obj" \
    "$(OBJ_PATH)\PutPageSync.obj" \
    "$(OBJ_PATH)\PutPageRdntArea.obj" \
    "$(OBJ_PATH)\EraseBlock.obj" \
    "$(OBJ_PATH)\FlashWriteSectorSync.obj" \
    "$(OBJ_PATH)\ECCUtils.obj"        \
    "$(OBJ_PATH)\ECCTable.obj" \
    "$(OBJ_PATH)\Buffer.obj"      \
    "$(OBJ_PATH)\MDTable.obj"     \
    "$(OBJ_PATH)\CISIDI.obj"         \
    "$(OBJ_PATH)\Log2PhyTable.obj"    \
    "$(OBJ_PATH)\StreamUtils.obj" \
    "$(OBJ_PATH)\FlashReadFirstGoodPageSync.obj" \
    "$(OBJ_PATH)\ValidateMedia.obj"   \
    "$(OBJ_PATH)\ProcessPT.obj"     \
    "$(OBJ_PATH)\ProcessID.obj"      \
    "$(OBJ_PATH)\DevInstalled.obj"     \
    "$(OBJ_PATH)\MakeZoneMap.obj"      \
    "$(OBJ_PATH)\Log2PhyUtils.obj"     \
    "$(OBJ_PATH)\RdntAreaUtils.obj"   \
    "$(OBJ_PATH)\ZoneMapUtils.obj"    \
    "$(OBJ_PATH)\DataUtils.obj"       \
    "$(OBJ_PATH)\datautils_write.obj" \
    "$(OBJ_PATH)\ErrorUtils.obj"      \
    "$(OBJ_PATH)\FlashReadPageSync.obj"   \
    "$(OBJ_PATH)\FlashReadRawPageSync.obj"  \
    "$(OBJ_PATH)\FlashReadSector.obj"   \
    "$(OBJ_PATH)\FlashGetSerialNumber.obj"   \
    "$(OBJ_PATH)\FlashReadSectorSync.obj"   \
    "$(OBJ_PATH)\Sec2LogUtils.obj"      \
    "$(OBJ_PATH)\FlashDriverInit.obj"   \
    "$(OBJ_PATH)\QueryUtils.obj"     \
    "$(OBJ_PATH)\GetPageSync.obj"     \
    "$(OBJ_PATH)\GetSMID.obj"     \
    "$(OBJ_PATH)\ResetMedia.obj"    \
    $(OBJ_PATH)\smdd_gpio.obj\
    "$(OBJ_PATH)\FlashReadFatSectorSync.obj"

ifdef MEDIA_DDI_PARTIAL_SEQ_ENABLE
SMDD_PARTIAL =\
    "$(OBJ_PATH)\flashwritepagepartialseqsync.obj"
endif

#///////////////////////////////////////////////////////////////////////////////
# Lower part of SmartMedia device driver (stays permanently in memory)
# These files are needed to support SysLoadResource & media insertion/removal
#///////////////////////////////////////////////////////////////////////////////

$(OBJ_PATH)\smdd_gpio.obj           : $(DDI_SM_PATH)\fat\hal\smdd_gpio.asm
"$(OBJ_PATH)\FlashReadPageRdntAreaSync.obj" :"$(DDI_SM_PATH)\fat\ddi\FlashReadPageRdntAreaSync.asm"
"$(OBJ_PATH)\MDTUtils.obj"              :"$(DDI_SM_PATH)\fat\ddi\MDTUtils.asm"
"$(OBJ_PATH)\convertcs.obj"             :"$(DDI_SM_PATH)\fat\ddi\convertcs.asm"
"$(OBJ_PATH)\PackUtils.obj"             :"$(DDI_SM_PATH)\fat\ddi\PackUtils.asm" 
"$(OBJ_PATH)\GetPageRdntArea.obj"       :"$(DDI_SM_PATH)\fat\hal\GetPageRdntArea.asm" 
"$(OBJ_PATH)\GetSMStatus.obj"           :"$(DDI_SM_PATH)\fat\hal\GetSMStatus.asm" 
"$(OBJ_PATH)\HALUtils.obj"              :"$(DDI_SM_PATH)\fat\hal\HALUtils.asm" 
"$(OBJ_PATH)\smartmediadetection.obj"   :"$(DDI_SM_PATH)\smartmediadetection.asm" $(COMMON_PROJ_PATH)\project.inc

"$(OBJ_PATH)\smartmediaserialnumber.obj"   :"$(DDI_SM_PATH)\smartmediaserialnumber.c" 


#///////////////////////////////////////////////////////////////////////////////
# Upper part of SmartMedia device driver (loaded from a resource)
#///////////////////////////////////////////////////////////////////////////////

"$(OBJ_PATH)\SMDDHeader.obj"         : "$(DDI_SM_PATH)\fat\SMDDHeader.asm" 
"$(OBJ_PATH)\FlashEraseRawBlock.obj" : "$(DDI_SM_PATH)\fat\ddi\FlashEraseRawBlock.asm" 
"$(OBJ_PATH)\FlashWritePageRdntAreaSync.obj" :"$(DDI_SM_PATH)\fat\ddi\FlashWritePageRdntAreaSync.asm" 
"$(OBJ_PATH)\MarkBadBlk.obj"         : "$(DDI_SM_PATH)\fat\ddi\MarkBadBlk.asm" 
"$(OBJ_PATH)\CopyUtils.obj"          : "$(DDI_SM_PATH)\fat\ddi\CopyUtils.asm" 
"$(OBJ_PATH)\FlashWriteRawPageSync.obj" :"$(DDI_SM_PATH)\fat\ddi\FlashWriteRawPageSync.asm" 
"$(OBJ_PATH)\FlashRelocateBlock.obj" : "$(DDI_SM_PATH)\fat\ddi\FlashRelocateBlock.asm" 
"$(OBJ_PATH)\FlashWritePageSync.obj" : "$(DDI_SM_PATH)\fat\ddi\FlashWritePageSync.asm" 
"$(OBJ_PATH)\FlashWritePageSeq.obj"  : "$(DDI_SM_PATH)\fat\ddi\FlashWritePageSeq.asm" 
"$(OBJ_PATH)\flashwritepagepartialseqsync.obj":"$(DDI_SM_PATH)\fat\ddi\flashwritepagepartialseqsync.asm" 
"$(OBJ_PATH)\ZoneMapUtils_Write.obj" : "$(DDI_SM_PATH)\fat\ddi\ZoneMapUtils_Write.asm" 
"$(OBJ_PATH)\PutPageSync.obj"        : "$(DDI_SM_PATH)\fat\hal\PutPageSync.asm" 
"$(OBJ_PATH)\PutPageRdntArea.obj"    : "$(DDI_SM_PATH)\fat\hal\PutPageRdntArea.asm" 
"$(OBJ_PATH)\EraseBlock.obj"         : "$(DDI_SM_PATH)\fat\hal\EraseBlock.asm" 
"$(OBJ_PATH)\FlashWriteSectorSync.obj":"$(DDI_SM_PATH)\fat\sal\FlashWriteSectorSync.asm" 
"$(OBJ_PATH)\ECCUtils.obj"              :"$(DDI_SM_PATH)\fat\ddi\ECCUtils.asm" 
"$(OBJ_PATH)\ECCTable.obj"              :"$(DDI_SM_PATH)\fat\ddi\ECCTable.asm" 
"$(OBJ_PATH)\Buffer.obj"                :"$(DDI_SM_PATH)\fat\ddi\Buffer.ASM" 
"$(OBJ_PATH)\MDTable.obj"               :"$(DDI_SM_PATH)\fat\ddi\MDTable.asm" 
"$(OBJ_PATH)\CISIDI.obj"                :"$(DDI_SM_PATH)\fat\ddi\CISIDI.asm" 
"$(OBJ_PATH)\Log2PhyTable.obj"          :"$(DDI_SM_PATH)\fat\ddi\Log2PhyTable.asm" 
"$(OBJ_PATH)\StreamUtils.obj"           :"$(DDI_SM_PATH)\fat\ddi\StreamUtils.asm" 
"$(OBJ_PATH)\FlashReadFirstGoodPageSync.obj" :"$(DDI_SM_PATH)\fat\ddi\FlashReadFirstGoodPageSync.asm" 
"$(OBJ_PATH)\ValidateMedia.obj"         :"$(DDI_SM_PATH)\fat\ddi\ValidateMedia.asm" 
"$(OBJ_PATH)\ProcessPT.obj"             :"$(DDI_SM_PATH)\fat\ddi\ProcessPT.asm" 
"$(OBJ_PATH)\ProcessID.obj"             :"$(DDI_SM_PATH)\fat\ddi\ProcessID.asm" 
"$(OBJ_PATH)\DevInstalled.obj"          :"$(DDI_SM_PATH)\fat\ddi\DevInstalled.asm" 
"$(OBJ_PATH)\MakeZoneMap.obj"           :"$(DDI_SM_PATH)\fat\ddi\MakeZoneMap.asm" 
"$(OBJ_PATH)\Log2PhyUtils.obj"          :"$(DDI_SM_PATH)\fat\ddi\Log2PhyUtils.asm" 
"$(OBJ_PATH)\RdntAreaUtils.obj"         :"$(DDI_SM_PATH)\fat\ddi\RdntAreaUtils.asm" 
"$(OBJ_PATH)\ZoneMapUtils.obj"          :"$(DDI_SM_PATH)\fat\ddi\ZoneMapUtils.asm" 
"$(OBJ_PATH)\DataUtils.obj"             :"$(DDI_SM_PATH)\fat\ddi\DataUtils.asm" 
"$(OBJ_PATH)\datautils_write.obj"       :"$(DDI_SM_PATH)\fat\ddi\datautils_write.asm" 
"$(OBJ_PATH)\ErrorUtils.obj"            :"$(DDI_SM_PATH)\fat\ddi\ErrorUtils.asm" 
"$(OBJ_PATH)\FlashReadPageSync.obj"     :"$(DDI_SM_PATH)\fat\ddi\FlashReadPageSync.asm" 
"$(OBJ_PATH)\FlashReadRawPageSync.obj"  :"$(DDI_SM_PATH)\fat\ddi\FlashReadRawPageSync.asm" 
"$(OBJ_PATH)\FlashReadSector.obj"       :"$(DDI_SM_PATH)\fat\sal\FlashReadSector.asm" 
"$(OBJ_PATH)\FlashGetSerialNumber.obj"  :"$(DDI_SM_PATH)\fat\sal\FlashGetSerialNumber.asm" 
"$(OBJ_PATH)\FlashReadSectorSync.obj"   :"$(DDI_SM_PATH)\fat\sal\FlashReadSectorSync.asm" 
"$(OBJ_PATH)\Sec2LogUtils.obj"          :"$(DDI_SM_PATH)\fat\sal\Sec2LogUtils.asm" 
"$(OBJ_PATH)\FlashDriverInit.obj"       :"$(DDI_SM_PATH)\fat\sal\FlashDriverInit.asm" 
"$(OBJ_PATH)\QueryUtils.obj"            :"$(DDI_SM_PATH)\fat\sal\QueryUtils.asm" 
"$(OBJ_PATH)\GetPageSync.obj"           :"$(DDI_SM_PATH)\fat\hal\GetPageSync.asm" 
"$(OBJ_PATH)\GetSMID.obj"               :"$(DDI_SM_PATH)\fat\hal\GetSMID.asm" 
"$(OBJ_PATH)\ResetMedia.obj"            :"$(DDI_SM_PATH)\fat\hal\ResetMedia.asm" 
"$(OBJ_PATH)\FlashReadFatSectorSync.obj" :"$(DDI_SM_PATH)\fat\sal\FlashReadFatSectorSync.asm" 
"$(OBJ_PATH)\smartmediastructs.obj"     :"$(DDI_SM_PATH)\smartmediastructs.c" $(FAT12_COMMON_DEPENDANCIES)


#extraction part.

"$(OUTPUT_RSRC_PATH)\SMDDHigh.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract SMDD High
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SMDD_Header_p,SMDD_High_p -f$(OUTPUT_PATH)\$(PROJ).map
