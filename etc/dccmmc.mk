#This is the makefile for the MultiMediaCard (MMC) command handler
#
#To include the MMC command handler in a DCC project
#   1. Include this makefile in the project makefile
#   2. Add $(DCCMMCLIST) to the list of linker dependencies


# The mmchalspiwrite.c file must build before mmchalspi.c
DCCMMCLIST   = \
            $(OBJ_PATH)\dctlmmc.obj\
            $(OBJ_PATH)\mmcsal.obj\
            $(OBJ_PATH)\mmcsalstructs.obj\
            $(OBJ_PATH)\mmcerase.obj\
            $(OBJ_PATH)\mmcasmhelper.obj\
            $(OBJ_PATH)\mmchalspiwrite.obj\
            $(OBJ_PATH)\mmchalspi.obj\
            $(OBJ_PATH)\mmchalspidetect.obj\
            $(OBJ_PATH)\spihal.obj\
            $(OBJ_PATH)\spistructs.obj\
            $(OBJ_PATH)\spihalasm.obj\
            $(OBJ_PATH)\mmcddheader.obj
            


#///////////////////////////////////////////////////////////////////////////////
# MultiMediaCard (MMC) command handler files
#///////////////////////////////////////////////////////////////////////////////

"$(OBJ_PATH)\dctlmmc.obj" : $(DCCDEVMMC_PATH)\dctlmmc.asm

"$(OBJ_PATH)\mmchalspiwrite.obj" : $(DDI_MMC_PATH)\SPI\mmchalspiwrite.c

"$(OBJ_PATH)\mmcsal.obj" : $(DDI_MMC_PATH)\mmcsal.c

"$(OBJ_PATH)\mmcsalstructs.obj" : $(DDI_MMC_PATH)\mmcsalstructs.c

"$(OBJ_PATH)\mmcerase.obj" : $(DDI_MMC_PATH)\mmcerase.c

"$(OBJ_PATH)\mmcasmhelper.obj" : $(DDI_MMC_PATH)\mmcasmhelper.c

"$(OBJ_PATH)\mmchalspi.obj" : $(DDI_MMC_PATH)\SPI\mmchalspi.c

"$(OBJ_PATH)\mmchalspidetect.obj" : $(DDI_MMC_PATH)\SPI\mmchalspidetect.c

"$(OBJ_PATH)\spihal.obj" : $(DDI_SPI_PATH)\spihal.c

"$(OBJ_PATH)\spistructs.obj" : $(DDI_SPI_PATH)\spistructs.c

$(OBJ_PATH)\spihalasm.obj : $(DDI_SPI_PATH)\spihalasm.asm

$(OBJ_PATH)\mmcddheader.obj : $(DDI_MMC_PATH)\mmcddheader.asm


