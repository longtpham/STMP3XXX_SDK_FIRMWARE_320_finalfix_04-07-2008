#This is the makefile for the MMC Data Drive
#

DDIMMCDATDATADRIVE_LIBNAME     =   dd_mmc_data_drive.a

MEDIATYPEMMC_PATH       = $(ROOT)\devicedriver\media\MediaTypeMMC
DDIMMC_PATH             = $(MEDIATYPEMMC_PATH)\DDI
DDIMMCDATDATADRIVE_PATH = $(DDIMMC_PATH)\DataDrive
DDIMMCCOMMON_PATH        = $(DDIMMC_PATH)\Common
HALMMC_PATH              = $(MEDIATYPEMMC_PATH)\HAL
HALMMCSPI_PATH       	 = $(HALMMC_PATH)\SPI
MEDIA_PATH               = $(ROOT)\devicedriver\media

DDIMMC_INCLUDEPATH              = $(DDIMMC_PATH)\include

INC_ALL=$(INC_ALL);$(DDIMMC_INCLUDEPATH);$(DDIMMCDATDATADRIVE_PATH);$(DDILDL_INCLUDE_PATH)
INC_ALL_C=$(INC_ALL_C) -Wc-I$(DDIMMC_INCLUDEPATH) -Wc-I$(DDILDL_INCLUDE_PATH)

DRIVERLIBS  =   $(DRIVERLIBS) \
                $(DDIMMCDATDATADRIVELIB)
		
LK_OPTS =  $(LK_OPTS)\
                -u FMMCDataDriveApi

ifdef D_PLIBS                
DDIMMCDATDATADRIVELIB   =   $(OBJ_PATH)\$(DDIMMCDATDATADRIVE_LIBNAME)
else
DDIMMCDATDATADRIVELIB   =   $(LIB_PATH)\$(DEVICE)\$(DDIMMCDATDATADRIVE_LIBNAME)
endif


# Considered Inserting MMCEraseFunctions.obj in list Dec20 2005 for sdk2.61 fat32test. 
# It was already compiled in this mk file.
#     Expected to not affect player since linker only links parts of libs used. See alt solution below.

DDIMMCDATDATADRIVELIST  = 	$(OBJ_PATH)\MMCDataDriveErase.obj \
				$(OBJ_PATH)\MMCDataDriveGetInfo.obj \
				$(OBJ_PATH)\MMCDataDriveInit.obj \
				$(OBJ_PATH)\MMCDataDriveMultiWrite.obj \
				$(OBJ_PATH)\MMCDataDriveReadSector.obj \
				$(OBJ_PATH)\MMCDataDriveSetInfo.obj \
				$(OBJ_PATH)\MMCDataDriveWriteSector.obj \
				$(OBJ_PATH)\MMCDataDriveFlush.obj \
				$(OBJ_PATH)\MMCFunctions.obj \
				$(OBJ_PATH)\MMCHalSpiWrite.obj \
                                $(OBJ_PATH)\MMCDataDriveDescriptorsInit.obj

#///////////////////////////////////////////////////////////////////////////////
# Dependencies
#///////////////////////////////////////////////////////////////////////////////
DDIMMCDATDATADRIVE_DEPENDENCIES= \
    "$(DDIMMC_INCLUDEPATH)\MMCDdi.h" \
    "$(DDIMMC_INCLUDEPATH)\MMCCommon.h" \
    "$(OUTPUT_INCLUDE_PATH)\errors.h"
    
# The SDK projects do not include sources for the libs, so the ifndef
# below allows us to exclude the dependancies below, but use the make
# variables above
ifdef D_PLIBS

$(DDIMMCDATDATADRIVELIB) : $(DDIMMCDATDATADRIVELIST)
    @echo Creating IEEE archive library file for DriveTypeMMC Data Drive $@
        @$(AR_EXE) -r $(DDIMMCDATDATADRIVELIB) $(DDIMMCDATDATADRIVELIST)

#///////////////////////////////////////////////////////////////////////////////
# Device driver files
#///////////////////////////////////////////////////////////////////////////////
"$(OBJ_PATH)\MMCDataDriveErase.obj"   	: "$(DDIMMCDATDATADRIVE_PATH)\MMCDataDriveErase.c" $(DDIMMCDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\MMCDataDriveGetInfo.obj"   : "$(DDIMMCDATDATADRIVE_PATH)\MMCDataDriveGetInfo.c" $(DDIMMCDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\MMCDataDriveInit.obj"   	: "$(DDIMMCDATDATADRIVE_PATH)\MMCDataDriveInit.c" $(DDIMMCDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\MMCDataDriveMultiWrite.obj"   : "$(DDIMMCDATDATADRIVE_PATH)\MMCDataDriveMultiWrite.c" $(DDIMMCDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\MMCDataDriveReadSector.obj"   : "$(DDIMMCDATDATADRIVE_PATH)\MMCDataDriveReadSector.c" $(DDIMMCDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\MMCDataDriveSetInfo.obj"      : "$(DDIMMCDATDATADRIVE_PATH)\MMCDataDriveSetInfo.c" $(DDIMMCDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\MMCDataDriveWriteSector.obj"  : "$(DDIMMCDATDATADRIVE_PATH)\MMCDataDriveWriteSector.c" $(DDIMMCDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\MMCDataDriveFlush.obj"  : "$(DDIMMCDATDATADRIVE_PATH)\MMCDataDriveFlush.c" $(DDIMMCDATDATADRIVE_DEPENDENCIES)

"$(OBJ_PATH)\MMCFunctions.obj"         : "$(DDIMMCCOMMON_PATH)\MMCFunctions.c" $(DDIMMCDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\MMCEraseFunctions.obj"    : "$(DDIMMCCOMMON_PATH)\MMCEraseFunctions.c" $(DDIMMCDATDATADRIVE_DEPENDENCIES)
# original line location of mmcerasefunctions.obj compile

"$(OBJ_PATH)\MMCHalSpiWrite.obj"    : "$(HALMMCSPI_PATH)\MMCHalSpiWrite.c" $(DDIMMCDATDATADRIVE_DEPENDENCIES)

"$(OBJ_PATH)\MMCDataDriveDescriptorsInit.obj"   : "$(DDIMMCDATDATADRIVE_PATH)\MMCDataDriveDescriptorsInit.asm" $(DDIMMCDATDATADRIVE_DEPENDENCIES)

else
ifdef MAKE_FAT32TEST
# fat32test needs this object and wanted it without asking for PLIBS so it's down here now (always compiled in mmcdatadrive.mk). 
"$(OBJ_PATH)\MMCEraseFunctions.obj"    : "$(DDIMMCCOMMON_PATH)\MMCEraseFunctions.c" $(DDIMMCDATDATADRIVE_DEPENDENCIES)
endif
endif

