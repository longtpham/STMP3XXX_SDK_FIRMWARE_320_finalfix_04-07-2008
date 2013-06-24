
include $(ETC_PATH)\registers.mk

MODULELIST= $(MODULELIST)\
    $(OUTPUT_RSRC_PATH)\MMCDD.src


ifdef MAKE_FUNCLET_RSRC
FUNCLETSRCLIST =    $(FUNCLETSRCLIST)\
        $(OUTPUT_RSRC_PATH)\Funclet_MmcEnumeratePhysicalDevice.src\
        $(OUTPUT_RSRC_PATH)\Funclet_ResetDevice.src\
        $(OUTPUT_RSRC_PATH)\Funclet_ProcessCsd.src\
        $(OUTPUT_RSRC_PATH)\Funclet_ProcessPartition.src\
        $(OUTPUT_RSRC_PATH)\Funclet_DoesMbrExist.src\
        $(OUTPUT_RSRC_PATH)\Funclet_ExtractPartitionParameters.src\
        $(OUTPUT_RSRC_PATH)\Funclet_PartitionEntryCopy.src
endif

ifdef D_MMC

#This is the makefile for the MultiMediaCard (MMC) device driver.


# The MMC_WRITE_BUILD switch forces the MMCDDHeader to use the correct write
#  function
AS_OPTS=$(AS_OPTS) -DMMC_WRITE_BUILD
CC_OPTS=$(CC_OPTS) -DMMC_WRITE_BUILD



OBJLIST = $(OBJLIST)\
         $(MMCDDLIST)

MMCDDLIST= $(SPIDD) $(MMCDD)

SPIDD= \
    $(OBJ_PATH)\spistructs.obj\
    $(OBJ_PATH)\spihal.obj\
    $(OBJ_PATH)\spihalasm.obj

MMCDD= \            
    $(OBJ_PATH)\mmcsalwrite.obj\
    $(OBJ_PATH)\mmchalspi.obj\
    $(OBJ_PATH)\mmchalspiwrite.obj\
    $(OBJ_PATH)\mmchalspidetect.obj\
    $(OBJ_PATH)\mmcsal.obj\
    $(OBJ_PATH)\mmcsalstructs.obj\
    $(OBJ_PATH)\mmcddheader.obj

#///////////////////////////////////////////////////////////////////////////////
# SPI device driver (used by the MMC device driver)
#///////////////////////////////////////////////////////////////////////////////

"$(OBJ_PATH)\spistructs.obj" : $(DDI_SPI_PATH)\spistructs.c
"$(OBJ_PATH)\spistructs.obj" : $(OUTPUT_INCLUDE_PATH)\regsspi.h
"$(OBJ_PATH)\spistructs.obj" : $(OUTPUT_INCLUDE_PATH)\regsgpio.h

"$(OBJ_PATH)\spihal.obj"     : $(DDI_SPI_PATH)\spihal.c         

$(OBJ_PATH)\spihalasm.obj    : $(OUTPUT_INCLUDE_PATH)\regsspi.h
$(OBJ_PATH)\spihalasm.obj    : $(OUTPUT_INCLUDE_PATH)\regsgpio.h
$(OBJ_PATH)\spihalasm.obj    : $(DDI_SPI_PATH)\spihalasm.asm    
$(OBJ_PATH)\spihalasm.obj    : $(OUTPUT_INCLUDE_PATH)\regsspi.h
$(OBJ_PATH)\spihalasm.obj    : $(OUTPUT_INCLUDE_PATH)\regsgpio.h

#///////////////////////////////////////////////////////////////////////////////
# MMC device driver
#///////////////////////////////////////////////////////////////////////////////

"$(OBJ_PATH)\mmchalspi.obj"       : $(DDI_MMC_PATH)\SPI\mmchalspi.c
"$(OBJ_PATH)\mmchalspiwrite.obj"  : $(DDI_MMC_PATH)\SPI\mmchalspiwrite.c
"$(OBJ_PATH)\mmchalspidetect.obj" : $(DDI_MMC_PATH)\SPI\mmchalspidetect.c
"$(OBJ_PATH)\mmcsal.obj"          : $(DDI_MMC_PATH)\mmcsal.c
"$(OBJ_PATH)\mmcsalwrite.obj"     : $(DDI_MMC_PATH)\mmcsalwrite.c
"$(OBJ_PATH)\mmcsalstructs.obj"   : $(DDI_MMC_PATH)\mmcsalstructs.c
"$(OBJ_PATH)\mmcddheader.obj"     : $(DDI_MMC_PATH)\mmcddheader.asm
"$(OUTPUT_RSRC_PATH)\MMCDD.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract MMCDD 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MMCDD_Header_p,.ptextspihal,.ptextmmcsal,.ptextmmchalspi,SPI_HAL_ASM_p,.ptextmmcsalwrite,.ptextmmchalspiwrite -f$(OUTPUT_PATH)\$(PROJ).map






"$(OUTPUT_RSRC_PATH)\Funclet_MmcEnumeratePhysicalDevice.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_ResetDevice.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_ProcessCsd.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_ProcessPartition.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_DoesMbrExist.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_ExtractPartitionParameters.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_PartitionEntryCopy.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclets
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -L$(LOC_PATH)\funclet_mmc.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\

else 

#this section is run if MMC is not being built.  Its required to fill all the holes in the resource.bin
# need to build null resources 
"$(OUTPUT_RSRC_PATH)\Funclet_MmcEnumeratePhysicalDevice.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(RELBASE_PATH)\resources\misc\null_data.src $(OUTPUT_RSRC_PATH)\Funclet_MmcEnumeratePhysicalDevice.src 
"$(OUTPUT_RSRC_PATH)\Funclet_ResetDevice.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(RELBASE_PATH)\resources\misc\null_data.src $(OUTPUT_RSRC_PATH)\Funclet_ResetDevice.src
"$(OUTPUT_RSRC_PATH)\Funclet_ProcessCsd.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(RELBASE_PATH)\resources\misc\null_data.src $(OUTPUT_RSRC_PATH)\Funclet_ProcessCsd.src
"$(OUTPUT_RSRC_PATH)\Funclet_ProcessPartition.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(RELBASE_PATH)\resources\misc\null_data.src $(OUTPUT_RSRC_PATH)\Funclet_ProcessPartition.src
"$(OUTPUT_RSRC_PATH)\Funclet_DoesMbrExist.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(RELBASE_PATH)\resources\misc\null_data.src $(OUTPUT_RSRC_PATH)\Funclet_DoesMbrExist.src
"$(OUTPUT_RSRC_PATH)\Funclet_ExtractPartitionParameters.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(RELBASE_PATH)\resources\misc\null_data.src $(OUTPUT_RSRC_PATH)\Funclet_ExtractPartitionParameters.src
"$(OUTPUT_RSRC_PATH)\Funclet_PartitionEntryCopy.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(RELBASE_PATH)\resources\misc\null_data.src $(OUTPUT_RSRC_PATH)\Funclet_PartitionEntryCopy.src

"$(OUTPUT_RSRC_PATH)\MMCDD.src" : $(OUTPUT_PATH)\$(PROJ).abs
    @echo Creating MMCDD.SRC placeholder.
    @copy /Y $(RESOURCE_PATH)\misc\null_data.src $(OUTPUT_RSRC_PATH)\MMCDD.src
endif          
